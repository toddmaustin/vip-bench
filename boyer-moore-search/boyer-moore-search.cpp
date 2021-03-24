/* C Program for Bad Character Heuristic of Boyer
Moore String Matching Algorithm */
// SOURCE: https://www.geeksforgeeks.org/boyer-moore-algorithm-for-pattern-searching/

# include <limits.h>
# include <string.h>
# include <stdio.h>
#include "../config.h"

using namespace std;

# define NO_OF_CHARS 256



// The preprocessing function for Boyer Moore's
// bad character heuristic
void badCharHeuristic(vector<VIP_ENCCHAR> str, int size, VIP_ENCINT badchar[NO_OF_CHARS])
{
	// Initialize all occurrences as -1
	for(int i = 0; i < NO_OF_CHARS; i++)
  {
		badchar[i] = -1;
  }

	// Fill the actual value of last occurrence of a character
	for(int i = 0; i < size; i++)
  {
#ifndef VIP_DO_MODE
		badchar[(int) VIP_DEC(str[i])] = i;
#else
    VIP_ENCINT badchar_index = (VIP_ENCINT) str[i]; 
    // True data-obliviousness requires us to write to all indexes
    for(int j=0; j<NO_OF_CHARS; j++)
    {
      badchar[j] = VIP_CMOV(j==badchar_index, j, badchar[j]);
    }
#endif
  }
}

/* A pattern searching function that uses Bad
Character Heuristic of Boyer Moore Algorithm */
void search(vector<VIP_ENCCHAR> txt, int n, vector<VIP_ENCCHAR> pat, int m, VIP_ENCBOOL *ret)
{
	VIP_ENCINT badchar[NO_OF_CHARS];

	/* Fill the bad character array by calling
	the preprocessing function badCharHeuristic()
	for given pattern */
	badCharHeuristic(pat, m, badchar);

	VIP_ENCINT s = 0; // s is shift of the pattern with
				     // respect to text
  
	for(int l=0; l<=(n - m); l++)
	{
#ifndef VIP_DO_MODE
    //Early exit if not data-oblivious... 
    if(VIP_DEC(s) > (n-m)) break;
#endif 

		VIP_ENCINT idx = m-1;

		/* Keep reducing index idx of pattern while
		characters of pattern and text are
		matching at this shift s */
#ifndef VIP_DO_MODE
		while(idx >= 0 && pat[VIP_DEC(idx)] == txt[VIP_DEC(s+idx)])
    {
			idx--;
    }
#else
    for(int i=0; i<m; i++)
    {
       // Access: pat[VIP_DEC(idx)]
      VIP_ENCCHAR pat_idx = pat[0];
      for(int j=0; j<m; j++)
        pat_idx = VIP_CMOV(j==idx, pat[j], pat_idx);
      // Access: txt[VIP_DEC(s+idx)]
      VIP_ENCCHAR txt_idx = txt[0];
      for(int j=0; j<n; j++)
        txt_idx = VIP_CMOV(j==s+idx, txt[j], txt_idx);

      idx = idx - VIP_CMOV(idx >= 0 && pat_idx == txt_idx, (VIP_ENCINT)1,  (VIP_ENCINT)0);
    }
#endif

		/* If the pattern is present at current
		shift, then index idx will become -1 after
		the above loop */
    VIP_ENCBOOL cond = (idx < 0);
#ifndef VIP_DO_MODE
    if(VIP_DEC(cond))
    {
      ret[VIP_DEC(s)] = true; 
			s += (s+m < n)? m-badchar[VIP_DEC(txt[VIP_DEC(s+m)])] : 1;
    }
		else 
    {
      VIP_ENCINT s_shift = idx - badchar[VIP_DEC(txt[VIP_DEC(s+idx)])];
			s += 1 > s_shift ? 1 : s_shift;
    }
#else
    // True data-obliviousness requires us to write to all indexes
    for(int i=0; i<n; i++)
    {
      ret[i] = VIP_CMOV(cond && i==s, true, ret[i]);
    }

    // Access: badchar[VIP_DEC(txt[VIP_DEC(s+m)])]
    VIP_ENCINT txt_sm = (VIP_ENCINT) txt[0];
    VIP_ENCINT badchar_txt_sm = badchar[0];
    for(int j=0; j<n; j++)
      txt_sm = VIP_CMOV(j==s+m, txt[j], txt_sm);
    for(int j=0; j<NO_OF_CHARS; j++)
      badchar_txt_sm = VIP_CMOV(j==txt_sm, badchar[j], badchar_txt_sm);

    // Access: badchar[VIP_DEC(txt[VIP_DEC(s+idx)])]
    VIP_ENCINT txt_sidx = (VIP_ENCINT) txt[0];
    VIP_ENCINT badchar_txt_sidx = badchar[0];
    for(int j=0; j<n; j++)
      txt_sidx = VIP_CMOV(j==s+idx, txt[j], txt_sidx);
    for(int j=0; j<NO_OF_CHARS; j++)
      badchar_txt_sidx = VIP_CMOV(j==txt_sidx, badchar[j], badchar_txt_sidx);

    VIP_ENCINT s_if   = VIP_CMOV(s+m < n, m-badchar_txt_sm, 1);
    VIP_ENCINT s_shift = idx - badchar_txt_sidx;
    VIP_ENCINT s_else =  VIP_CMOV(1 > s_shift, 1, s_shift);
    s = s + VIP_CMOV(cond, s_if, s_else);
#endif
	}
}


int
main(void) 
{ 
  VIP_INIT; 

  // Inputs
  char inp_txt[] = "ABAAABCDDABC";
  char inp_pat[] = "ABC";
  int n = strlen(inp_txt); // String lengths are public
  int m = strlen(inp_pat); // String lengths are public
  
  vector<VIP_ENCCHAR> txt(n);
  for (unsigned k=0; k < n; k++)
    txt[k] = inp_txt[k];

  vector<VIP_ENCCHAR> pat(m);
  for (unsigned k=0; k < m; k++)
    pat[k] = inp_pat[k];

  // Return vector
  VIP_ENCBOOL  ret[n];
  for(int i=0; i<n; i++) ret[i] = false; 
	

  // Run search and print results
  search(txt, n, pat, m, ret);
  for(int i=0; i<n; i++)
  {
    if(VIP_DEC(ret[i])) 
    { 
      printf("pattern occurs at shift = %d\n", i);
    }
  }

  return 0;
} 

