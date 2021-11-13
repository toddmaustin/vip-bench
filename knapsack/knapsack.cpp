//
// A Dynamic Programming based solution for 0-1 Knapsack problem
//
// The All ▲lgorithms library for python
//
// https://allalgorithms.com/dynamic-programming/
// https://github.com/allalgorithms/cpp
//
// Contributed by: Unknown
// Github: Unknown
//
#include <iostream>
using namespace std;

#include "../config.h"

#define N 3
#define W 50
VIP_ENCINT val[N] = {60, 100, 120};
VIP_ENCINT wt[N] = {10, 20, 30};

// A utility function that returns maximum of two integers
VIP_ENCINT
max(VIP_ENCINT a, VIP_ENCINT b)
{
#ifdef VIP_DO_MODE
  VIP_ENCINT retval = VIP_CMOV(a > b, a, b);
  return retval; 
#else /* !VIP_DO_MODE */
  if (a > b)
    return a;
  else
    return b;
#endif /* !VIP_DO_MODE */
}

// Returns the maximum value that can be put in a knapsack of capacity W
void
knapSack(VIP_ENCINT wt[], VIP_ENCINT val[], VIP_ENCINT K[N+1][W+1])
{
  int i, w;

  // Build table K[][] in bottom up manner
  for (i = 0; i <= N; i++)
  {
    for (w = 0; w <= W; w++)
    {
      if (i==0 || w==0)
        K[i][w] = 0;
      else
      {
#ifdef VIP_DO_MODE
        K[i][w] = VIP_CMOV(wt[i-1] <= w, max(val[i-1] + K[i-1][w-wt[i-1]],  K[i-1][w]), K[i-1][w]);
#else /* !VIP_DO_MODE */
        if (wt[i-1] <= w)
          K[i][w] = max(val[i-1] + K[i-1][w-wt[i-1]],  K[i-1][w]);
        else
          K[i][w] = K[i-1][w];
#endif /* !VIP_DO_MODE */
      }
    }
  }
}

int
main(void)
{
  int n = N;
  int w = W;
  VIP_ENCINT K[N+1][W+1];

  {
    Stopwatch s("VIP-Bench knapsack:");

    knapSack(wt, val, K);
  }

	fprintf(stdout, "Max value: %d\n", VIP_DEC(K[n][W]));
	
  fprintf(stdout, "Selected packs:\n");
  while (n != 0)
  {
    if (VIP_DEC(K[n][w]) != VIP_DEC(K[n - 1][w])) {
      fprintf(stdout, "  Package %d with wt=%d and val=%d\n", n, VIP_DEC(wt[n - 1]), VIP_DEC(val[n - 1]));
      w = w - VIP_DEC(wt[n-1]);
    }
    n--;
  }
  return 0;
}

#ifdef notdef
  for (i=0; i <= n; i++)
  {
    for (w=0; w <= W; w++)
    {
      fprintf(stdout, "%3d ", K[i][w]);
    }
    fprintf(stdout, "\n");
  }
#endif /* notdef */
