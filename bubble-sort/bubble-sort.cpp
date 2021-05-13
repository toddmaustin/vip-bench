#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// include build configuration defines
#include "../config.h"

#define DATASET_SIZE 256
VIP_ENCINT data[DATASET_SIZE];

// total swaps executed so far
unsigned long swaps = 0;

void print_data(VIP_ENCINT *data, unsigned size)
{
  fprintf(stdout, "DATA DUMP:\n");
  for (unsigned i = 0; i < size; i++)
    fprintf(stdout, "  data[%u] = %d\n", i, VIP_DEC(data[i]));
}

void bubblesort(VIP_ENCINT *data, unsigned size)
{
  for (unsigned i = 0; i < size - 1; i++)
  {
#ifndef VIP_DO_MODE
    bool swapped = false;
#endif /* !VIP_DO_MODE */
    for (unsigned j = 0; j < size - 1; j++)
    {
#ifndef VIP_DO_MODE
      if (data[j] > data[j + 1])
      {
        VIP_ENCINT tmp = data[j];
        data[j] = data[j + 1];
        data[j + 1] = tmp;
        swapped = true;
        swaps++;
      }
#else  /* VIP_DO_MODE */
      VIP_ENCBOOL do_swap = data[j] > data[j + 1];
      VIP_ENCINT tmp = data[j];
      data[j] = VIP_CMOV(do_swap, data[j + 1], data[j]);
      data[j + 1] = VIP_CMOV(do_swap, tmp, data[j + 1]);
      swaps++;
#endif /* VIP_DO_MODE */
    }
#ifndef VIP_DO_MODE
    // done?
    if (!swapped)
      break;
#endif /* !VIP_DO_MODE */
  }
}

int main(void)
{
  // initialize the privacy enhanced execution target
  VIP_INIT;

  // AJK
  uint64_t perf_cmds = 0;
  uint64_t perf_idle = 0;
  uint64_t perf_prep = 0;
  uint64_t perf_ex = 0;
  uint64_t perf_wait = 0;
  uint64_t perf_skip = 0;

  OZonePerfClear();
  perf_cmds = OZonePerfCmds();
  perf_idle = OZonePerfIdle();
  perf_prep = OZonePerfPrep();
  perf_ex = OZonePerfEx();
  perf_wait = OZonePerfWait();
  perf_skip = OZonePerfSkipped();
  fprintf(stdout,
          "INITIAL PERFORMANCE STATE:\n %lu cmds executed.\n%lu idle cycles.\n%lu prep cycles.\n%lu ex cycles.\n%lu wait cycles.\n%lu skipped states.\n",
          perf_cmds, perf_idle, perf_prep, perf_ex, perf_wait, perf_skip);
  OZonePerfClear();

  // initialize the pseudo-RNG
  srand(42);
  // srand(time(NULL));

  // initialize the array to sort
  for (unsigned i = 0; i < DATASET_SIZE; i++)
    data[i] = rand();
#ifndef PERF_OUTPUT_ONLY
  print_data(data, DATASET_SIZE);
#endif
  bubblesort(data, DATASET_SIZE);
#ifndef PERF_OUTPUT_ONLY
  print_data(data, DATASET_SIZE);
#endif

  // check the array
  for (unsigned i = 0; i < DATASET_SIZE - 1; i++)
  {
    if (VIP_DEC(data[i]) > VIP_DEC(data[i + 1]))
    {
      fprintf(stdout, "ERROR: data is not properly sorted.\n");
      return -1;
    }
  }

#ifndef PERF_OUTPUT_ONLY
  fprintf(stdout, "INFO: %lu swaps executed.\n", swaps);
  fprintf(stdout, "INFO: data is properly sorted.\n");
#endif
  perf_cmds = OZonePerfCmds();
  perf_idle = OZonePerfIdle();
  perf_prep = OZonePerfPrep();
  perf_ex = OZonePerfEx();
  perf_wait = OZonePerfWait();
  perf_skip = OZonePerfSkipped();

  fprintf(stdout,
          "PERFORMANCE METRICS:\n %lu cmds executed.\n%lu idle cycles.\n%lu prep cycles.\n%lu ex cycles.\n%lu wait cycles.\n%lu skipped states.\n",
          perf_cmds, perf_idle, perf_prep, perf_ex, perf_wait, perf_skip);

  return 0;
}
