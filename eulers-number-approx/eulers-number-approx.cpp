/*
 * calculate e=2.718..., using an interative approximation 
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
using namespace std;

#include "../config.h"

int main(void)
{
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

  double steps = 10000000; /* STEPS is usually a very large number eg 10000000 */
  VIP_ENCDOUBLE x, y;

  y = (VIP_ENCDOUBLE)1.0 + (1.0 / steps);
  x = (VIP_ENCDOUBLE)1.0;

  for (; steps > 0; steps--)
  {
    x = x * y;
  }
#ifndef PERF_OUTPUT_ONLY
  cout << "INFO: EulersNumberApprox(" << steps << ") == " << VIP_DEC(x) << endl;
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
  fprintf(stdout,
          "PERFORMANCE METRICS (formatted):\n%lu\n%lu\n%lu\n%lu\n%lu\n%lu\n",
          perf_cmds, perf_idle, perf_prep, perf_ex, perf_wait, perf_skip);

  return 0;
}
