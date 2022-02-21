#include <iostream>
#include <iomanip>
using namespace std;

#include "../config.h"
 
// `M × N` matrix
#define M 10
#define N 10
 
// Below arrays detail all eight possible movements
int row[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int col[] = { -1, 0, 1, -1, 1, -1, 0, 1 };
 
// check if it is possible to go to pixel `(x, y)` from the
// current pixel. The function returns false if the pixel
// has a different color, or it's not a valid pixel
#define SAFELOC(X, Y)     ((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N)
#define ISSAFE(MAT, X, Y, TARGET)     (((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N) && mat[(X)][(Y)] == (TARGET))
 
// Flood fill using DFS
void
floodfill(VIP_ENCCHAR mat[M][N], VIP_ENCINT x, VIP_ENCINT y, VIP_ENCCHAR replacement)
{
#ifdef VIP_DO_MODE
  // get the target color, and set the first replacement
  VIP_ENCCHAR target = '\0';
  for (int ix=0; ix < M; ix++)
  {
    for (int iy=0; iy < N; iy++)
    {
      VIP_ENCBOOL _istarget = (x == ix && y == iy);
      target = VIP_CMOV(_istarget, mat[ix][iy], target);
      mat[ix][iy] = VIP_CMOV(_istarget, replacement, mat[ix][iy]);
    }
  }

  // for as many times as worst-case runtime (probably not this bad!)
  for (int i=0; i < N*M; i++)
  {
    // for all x values
    for (int ix=0; ix < M; ix++)
    {
      // for all y values
      for (int iy=0; iy < N; iy++)
      {
        // check if we see a flood value 
        VIP_ENCBOOL _atreplace = (mat[ix][iy] == replacement);

        // for all immediately surrounding pixels
        for (int k=0; k < 8; k++)
        {
          // valid adjacent location?
          if (SAFELOC(ix+row[k], iy+col[k]))
          {
            // if the target pixel is adjacent, replace it
            VIP_ENCBOOL _floodable = _atreplace && (mat[ix+row[k]][iy+col[k]] == target);
            mat[ix+row[k]][iy+col[k]] = VIP_CMOV(_floodable, replacement, mat[ix+row[k]][iy+col[k]]);
          }
        }
      }
    }
  }
#else /* !VIP_DO_MODE */
  // get the target color
  VIP_ENCCHAR target = mat[x][y];
 
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
      floodfill(mat, x + row[k], y + col[k], replacement);
    }
  }
#endif /* VIP_DO_MODE */
}
 
// Utility function to print a matrix
void
printMatrix(VIP_ENCCHAR mat[M][N])
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      cout << setw(3) << VIP_DEC(mat[i][j]);
    }
    cout << endl;
  }
}
 
int
main()
{
  // matrix showing portion of the screen having different colors
  VIP_ENCCHAR mat[M][N] =
  {
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
  };
 
  // start node
  VIP_ENCINT x = 3, y = 9;    // having a target color `X`

  // replacement color, NOTE: must be a new color!
  VIP_ENCCHAR replacement = 'C';

  // print the colors before replacement
  printf("\nBEFORE flooding `%c' @ (%d,%d):\n", VIP_DEC(replacement), VIP_DEC(x), VIP_DEC(y)); printMatrix(mat);

  {
    Stopwatch s("VIP_Bench Runtime");

    // replace the target color with a replacement color using DFS
    floodfill(mat, x, y, replacement);
  }

  // print the colors after replacement
  printf("\nAFTER:\n"); printMatrix(mat);

  return 0;
}

