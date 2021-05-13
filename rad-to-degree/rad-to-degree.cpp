/*
 * calculate e=2.718..., using an interative approximation 
 */

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include "../config.h"

#define PI (4 * atan(1)) //Not d-o because its pubic knowledge

VIP_ENCDOUBLE
rad2deg(VIP_ENCDOUBLE rad)
{
  return (180.0 * rad / (PI));
}

VIP_ENCDOUBLE
deg2rad(VIP_ENCDOUBLE deg)
{
  return (PI * deg / 180.0);
}

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
#ifndef PERF_OUTPUT_ONLY
  cout << fixed;
  cout << setprecision(5);
#endif

  for (double x = 0.0; x <= 360.0; x += 1.0)
  {
    VIP_ENCDOUBLE rad_res = VIP_DEC(deg2rad((VIP_ENCDOUBLE)x));
#ifndef PERF_OUTPUT_ONLY
    cout << "INFO: deg2rad(" << x << ") == " << rad_res << endl;
#endif
  }

  for (double x = 0.0; x <= (2 * PI + 1e-6); x += (PI / 180))
  {
    VIP_ENCDOUBLE deg_res = VIP_DEC(rad2deg((VIP_ENCDOUBLE)x));
#ifndef PERF_OUTPUT_ONLY
    cout << "INFO: rad2deg(" << x << ") == " << deg_res << endl;
#endif
  }

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
