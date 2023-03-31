#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>
#include <thread>

using namespace std;

#include "../config.h"
#include "utils.h"
//
// sizes support 10 (default), 15, 20, 25
#define SIZE 10

// `M × N` matrix
#define M SIZE
#define N SIZE
int BITMAPS = 2;

typedef vector<VIP_ENCUINT64> VIP_Enc_Group_t;
VIP_Enc_Group_t initGroup(BITMAPS, 0); //{BITMAPS x 0}
struct node
{
	VIP_Enc_Group_t group = initGroup;
};

/********************************** Used for printing and debugging purposes **************************************/
void dev_printMatrix(VIP_ENCCHAR mat[M][N], VIP_ENCINT x, VIP_ENCINT y, VIP_ENCCHAR replacement)
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (VIP_DEC(x) == i && VIP_DEC(y) == j && VIP_DEC(mat[i][j]) != VIP_DEC(replacement))
			{
				cout << "\033[1;33m" << setw(3) << VIP_DEC(mat[i][j]) << "\033[0m";
			}
			else if (VIP_DEC(mat[i][j]) == VIP_DEC(mat[VIP_DEC(x)][VIP_DEC(y)]) && VIP_DEC(mat[i][j]) != VIP_DEC(replacement))
			{
				cout << "\033[1;31m" << setw(3) << VIP_DEC(mat[i][j]) << "\033[0m";
			}
			else if (VIP_DEC(mat[i][j]) == VIP_DEC(replacement))
			{
				cout << "\033[1;32m" << setw(3) << VIP_DEC(mat[i][j]) << "\033[0m";
			}
			else
			{
				cout << setw(3) << VIP_DEC(mat[i][j]);
			}
		}
		cout << endl;
	}
}
void dev_printRawMatrix(VIP_ENCCHAR mat[M][N])
{
	cout << "{ ";
	for (int i = 0; i < M; i++)
	{
		cout << "{ ";
		for (int j = 0; j < N; j++)
		{
			cout << "'" << VIP_DEC(mat[i][j]) << "', ";
		}
		cout << "},";
		cout << endl;
	}
	cout << "};";
}

void dev_clearScreen(int delay)
{
	std::this_thread::sleep_for(std::chrono::milliseconds(delay));
	cout << "\033[H\033[2J\033[3J";
}
string dev_readableNumber(long n)
{
	string num = to_string(n);
	string res = "";

	int size = num.size();

	for (int i = num.size() - 1; i >= 0; i--)
	{
		res = num[i] + res;
		if ((size - i) % 3 == 0 && i != 0)
			res = "," + res;
	}
	return res;
}
string dev_intToBinary(uint64_t n)
{
	if (!n)
		return "0";
	string binary = "";
	while (n)
	{
		if (n & 1)
			binary = "1" + binary;
		else
			binary = "0" + binary;

		n >>= 1;
	}
	return binary;
}

int dev_binaryToInt(string binary)
{
	return std::stoi(binary, nullptr, 2);
}

// Can use this function to see groups
void dev_printStructMatrix(node struct_mat[M][N])
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			// needs to be updated
			cout << setw(6) << "(" << dev_intToBinary(VIP_DEC(struct_mat[i][j].group[1])) << "),";
		}
		cout << endl;
	}
}
/******************************************************************************************************************/

/******************************* Helper Functions for tests ******************************************************/

bool dev_compareMatrix(VIP_ENCCHAR mat1[M][N], char mat2[M][N])
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			if (VIP_DEC(mat1[i][j]) != mat2[i][j])
			{
				return false;
			}
		}
	}
	return true;
}
void dev_copyMatrix(char mat[M][N], VIP_ENCCHAR copy[M][N])
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			copy[i][j] = mat[i][j];
		}
	}
}
void dev_copyMatrixNA(char mat[M][N], char copy[M][N])
{
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			copy[i][j] = mat[i][j];
		}
	}
}
/******************************************************************************************************************/

inline VIP_ENCBOOL haveIntersection(VIP_Enc_Group_t bin1, VIP_Enc_Group_t bin2)
{
	VIP_ENCBOOL intersect = false;
	for (int i = 0; i < BITMAPS; i++)
	{
		intersect = intersect || ((bin1[i] & bin2[i]) != 0);
	}

	return intersect;
}
inline VIP_ENCUINT64 combine(VIP_ENCUINT64 bin1, VIP_ENCUINT64 bin2)
{
	return bin1 | bin2;
}

#ifdef VIP_DO_MODE
void
floodfillDO(VIP_ENCCHAR mat[M][N], VIP_ENCINT x, VIP_ENCINT y, VIP_ENCCHAR replacement)
{
	int row[] = {-1, -1, -1, 0, 0, 1, 1, 1, 0};
	int col[] = {-1, 0, 1, -1, 1, -1, 0, 1, 0};
	node struct_mat[M][N];
	VIP_ENCUINT64 currId = 1;
	VIP_ENCINT bitMapIdx = 0;

#define SAFELOC(X, Y) ((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N)

	VIP_ENCBOOL matchFound = false;

	// Forward pass
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{

			node *cell = &(struct_mat[i][j]);
			VIP_Enc_Group_t commonGroup = cell->group;

			// Forward read pass
			for (int k = 0; k < 4; k++)
			{

				if (SAFELOC(i + row[k], j + col[k]))
				{
					node *adjCell = &(struct_mat[i + row[k]][j + col[k]]);

					VIP_ENCBOOL match = (mat[i][j] == mat[i + row[k]][j + col[k]]);

					matchFound = match || matchFound;

					for (int b = 0; b < BITMAPS; b++)
					{
						commonGroup[b] = VIP_CMOV(match, combine(commonGroup[b], (adjCell->group)[b]), commonGroup[b]);
					}
				}
			}

			cell->group = commonGroup;

			// Forward write pass
			for (int k = 0; k < 4; k++)
			{
				if (SAFELOC(i + row[k], j + col[k]))
				{
					node *adjCell = &(struct_mat[i + row[k]][j + col[k]]);

					VIP_ENCBOOL match = (mat[i][j] == mat[i + row[k]][j + col[k]]);

					for (int b = 0; b < BITMAPS; b++)
					{
						(adjCell->group)[b] = VIP_CMOV(match, (commonGroup)[b], (adjCell->group)[b]);
					}
				}
			}

			VIP_ENCBOOL update_current_id = !matchFound && (currId == 0) && (bitMapIdx + 1 < BITMAPS);

			currId = VIP_CMOV(update_current_id, (uint64_t)1, currId);
			// currId = currId + (((currId + 1) << ((((-currId) | currId) >> (64 - 1)) & 1)) & 1);

			bitMapIdx += VIP_ENCINT(VIP_CMOV(update_current_id, (VIP_ENCINT)1, (VIP_ENCINT)0));

			for (int b = 0; b < BITMAPS; b++)
			{

				cell->group[b] = VIP_CMOV(b == bitMapIdx && !matchFound, currId, cell->group[b]);
			}

			currId = currId << VIP_CMOV(!matchFound, (VIP_ENCUINT64)1, (VIP_ENCUINT64)0);

			matchFound = false;
		}
	}

	VIP_Enc_Group_t targetGr = initGroup;

	// Reverse pass
	for (int i = M - 1; i >= 0; i--)
	{
		for (int j = N - 1; j >= 0; j--)
		{
			node *cell = &(struct_mat[i][j]);
			VIP_Enc_Group_t commonGroup = cell->group;

			// Reverse read pass
			for (int k = 0; k < 8; k++)
			{
				if (SAFELOC(i + row[k], j + col[k]))
				{
					node *adjCell = &(struct_mat[i + row[k]][j + col[k]]);

					// Check if current position matches with the cell
					VIP_ENCBOOL match = (mat[i][j] == mat[i + row[k]][j + col[k]]);

					for (int b = 0; b < BITMAPS; b++)
					{
						commonGroup[b] = VIP_CMOV(match, combine(commonGroup[b], (adjCell->group)[b]), commonGroup[b]);
					}
				}
			}

			// Reverse write pass
			for (int k = 0; k <= 8; k++)
			{
				if (SAFELOC(i + row[k], j + col[k]))
				{
					node *adjCell = &(struct_mat[i + row[k]][j + col[k]]);

					VIP_ENCBOOL match = (mat[i][j] == mat[i + row[k]][j + col[k]]);

					VIP_ENCBOOL _istarget = (x == i + row[k]) && (y == j + col[k]);
					for (int b = 0; b < BITMAPS; b++)
					{
						(adjCell->group)[b] = VIP_CMOV(match, commonGroup[b], (adjCell->group)[b]);
						// Oblivious access to get target group
						targetGr[b] = VIP_CMOV(_istarget, (adjCell->group)[b], targetGr[b]);
					}
				}
			}
		}
	}

	// Target Group Update pass: This pass is required for correctness [Verified - MZD]
	for (int ix = 0; ix < M; ix++)
	{
		for (int iy = 0; iy < N; iy++)
		{

			node *cell = &struct_mat[ix][iy];

			VIP_ENCBOOL cond = haveIntersection(cell->group, targetGr);
			// if group contains target group
			for (int b = 0; b < BITMAPS; b++)
			{
				targetGr[b] = VIP_CMOV(cond, combine(cell->group[b], targetGr[b]), targetGr[b]);
			}
		}
	}

	// Coloring pass
	for (int i = 0; i < M; i++)
	{
		for (int j = 0; j < N; j++)
		{
			VIP_ENCBOOL flood = haveIntersection(targetGr, struct_mat[i][j].group);
			mat[i][j] = VIP_CMOV(flood, replacement, mat[i][j]);
		}
	}
}
#endif /* VIP_DO_MODE */

void floodfillNA(char mat[M][N], int x, int y, char replacement)
{
	int row[] = {-1, -1, -1, 0, 0, 1, 1, 1};
	int col[] = {-1, 0, 1, -1, 1, -1, 0, 1};
#define ISSAFE(MAT, X, Y, TARGET) (((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N) && mat[(X)][(Y)] == (TARGET))

	// get the target color
	char target = mat[x][y];

	// replace the current pixel color with that of replacement
	mat[x][y] = replacement;

	// process all eight adjacent pixels of the current pixel and
	// recur for each valid pixel
	for (int k = 0; k < 8; k++)
	{
		// if the adjacent pixel at position `(x + row[k], y + col[k])` is
		// a valid pixel and has the same color as that of the current pixel
		if (ISSAFE(mat, x + row[k], y + col[k], target))
		{
			floodfillNA(mat, x + row[k], y + col[k], replacement);
		}
	}
}

// matrix showing portion of the screen having different colors
VIP_ENCCHAR mat[M][N] =
{
#if SIZE == 10
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X' }
#elif SIZE == 15
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#elif SIZE == 20
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#elif SIZE == 25
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#endif
};

void singleTest()
{
	VIP_ENCCHAR replacement = 'C';
	VIP_ENCINT x = 0, y = 1;
	char mat[M][N] =
		{
			{'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'X'},
			{'O', 'X', 'O', 'O', 'O', 'X', 'O', 'O', 'O', 'X'},
			{'X', 'O', 'O', 'O', 'X', 'O', 'O', 'O', 'O', 'X'},
			{'O', 'X', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'X'},
			{'X', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O', 'O'},
			{'O', 'X', 'O', 'X', 'O', 'O', 'O', 'O', 'X', 'O'},
			{'O', 'O', 'O', 'O', 'O', 'O', 'X', 'O', 'O', 'X'},
			{'O', 'O', 'O', 'O', 'O', 'X', 'O', 'O', 'X', 'O'},
			{'O', 'O', 'O', 'O', 'O', 'O', 'O', 'X', 'X', 'O'},
			{'O', 'O', 'O', 'X', 'O', 'O', 'O', 'X', 'O', 'X'},
		};
	VIP_ENCCHAR copy1[M][N];
	char copy2[M][N];
	dev_copyMatrix(mat, copy1);
	dev_copyMatrixNA(mat, copy2);

	// cout << "\nNA - Before:" << endl;
	// dev_printMatrix(copy2, x, y, replacement);

	cout << "\nBefore:" << endl;
	dev_printMatrix(copy1, x, y, replacement);

	// bool inputSame = dev_compareMatrix(copy1, copy2);
	// auto inputSameText = inputSame ? "\033[1;32mPASS\033[0m" : "\033[1;31mFAIL\033[0m";
	// cout << "\nSame Input: " << inputSameText << endl;

	// for NA flood fill
	floodfillNA(copy2, VIP_DEC(x), VIP_DEC(y), VIP_DEC(replacement));
	// cout << "\nNA - After:" << endl;
	// dev_printMatrix(copy2, x, y, replacement);

  {
    Stopwatch s("VIP_Bench Runtime");

#if defined(VIP_NA_MODE)
    // replace the target color with a replacement color using DFS
	  floodfillNA(copy1, x, y, replacement);
#elif defined(VIP_DO_MODE)
    // replace the target color with a replacement color using DFS
	  floodfillDO(copy1, x, y, replacement);
#endif /* VIP_NA_MODE */
  }
	cout << "\nAfter:" << endl;
	dev_printMatrix(copy1, x, y, replacement);

	bool equal = dev_compareMatrix(copy1, copy2);
	auto equalText = equal ? "\033[1;32mPASS\033[0m" : "\033[1;31mFAIL\033[0m";
	cout << "\nCorrectness: " << equalText << endl;
}

#if TESTS
void allTest()
{

	VIP_ENCCHAR replacement = 'C';
#ifndef VIP_NA_MODE

#endif

	// testing
	int failCount = 0;
	int overflow = 0;
	int total = 0;
	auto seed = time(NULL);
	mysrand(seed);

	long testMatriceCount = 10000;

	for (int run = 0; run < testMatriceCount; run++)
	{
		VIP_ENCCHAR mat[M][N];

		// randomly populate
		for (int row = 0; row < M; row++)
		{
			for (int col = 0; col < N; col++)
			{
				VIP_ENCCHAR randChar = (myrand() > RAND_MAX / 2) ? 'O' : 'X';
				mat[row][col] = randChar;
			}
		}

		// run for every possible target

		for (int i = 0; i < M; i++)
		{
			for (int j = 0; j < N; j++)
			{
				total++;

				VIP_ENCINT x = i, y = j;

				VIP_ENCCHAR copy1[M][N];
				VIP_ENCCHAR copy2[M][N];
				dev_copyMatrix(mat, copy1);
				dev_copyMatrix(mat, copy2);

				// for NA flood fill
				floodfillNA(copy2, x, y, replacement);

#ifdef VIP_DO_MODE
				// for DO flood fill
				int ret = floodfillDO(copy1, x, y, replacement);
#else
				// for NA flood fill
				int ret = floodfillNA(copy1, x, y, replacement);
#endif

				if (ret == 0)
				{

					bool equal = dev_compareMatrix(copy1, copy2);

					if (!equal)
					{
						failCount++;
						cout << x << " " << y << endl;
						dev_printRawMatrix(mat);
						return;
					}
				}
				else
				{
					overflow += 1;
				}
				// cin.get();
				dev_clearScreen(0);
				long totalTestCount = M * N * testMatriceCount;
				cout << "\n\033[1;36mRunning " << dev_readableNumber(totalTestCount) << " tests on " << M << "x" << N << " matrices using " << BITMAPS << " long integers!\033[0m\n";
				int completed = total * 100 / double(totalTestCount);
				cout << "\n\033[1;33mCompleted: " << completed << "% \033[0m\n";
				cout << "\n\033[1;33mRandom Matrices tested: " << dev_readableNumber(run + 1) << " \033[0m\n";
				cout << "\n\033[1;33mTotal: " << dev_readableNumber(total) << " \033[0m\n";
				cout << "\n\033[1;32mPassed: " << dev_readableNumber(total - overflow - failCount) << " \033[0m\n";
				cout << "\n\033[1;31mFailed " << dev_readableNumber(failCount) << "\033[0m\n";
				cout << "\n\033[1;35mOverflowed: " << dev_readableNumber(overflow) << "\033[0m\n";
			}
		}
	}
}
#endif /* TESTS */

int main()
{
	singleTest();
	// allTest();

	return 0;
}
