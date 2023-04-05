#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// include build configuration defines
#include "../config.h"

void
slice_n_dice(FILE *outfile, uint64_t samples)
{
  VIP_ENCINT x;
  uint64_t count = 0;

  // continue this test sequence until 
  while (count < samples)
  {

    fprintf(stdout, "INFO: Running the too-many-0s test...\n");
    for (unsigned i=0; i < 10000; i++)
    {
      x = 1;
      VIP_EMITCT(outfile, x, count, samples, done);
    }

    fprintf(stdout, "INFO: Running the fixed-stride test...\n");
    x = 0;
    for (unsigned i=0; i < 10000; i++)
    {
      x++;
      VIP_EMITCT(outfile, x, count, samples, done);
    }

  }

done:
  assert (count == samples);
  return;
}

int
main(int argc, char* argv[])
{
  // initialize the privacy enhanced execution target
  VIP_INIT;

  // initialize the pseudo-RNG
  mysrand(42);
  // mysrand(time(NULL));

  if (argc != 3)
  {
    fprintf(stderr, "USAGE: %s <out-file-name> <sample-size>\n", argv[0]);
    exit(1);
  }

  char *outname = argv[1];
  uint64_t samples = strtoull(argv[2], NULL, 10);;

  fprintf(stdout, "INFO: writing `%lu' samples to file `%s'\n", samples, outname);

  // open the sample file
  FILE *outfile = fopen(outname, "w");
  if (!outfile)
  {
    fprintf(stderr, "ERROR: cannot open file `%s'\n", outname);
    exit(1);
  }

  // write the DIEHARDER RNG sample file header
  fprintf(outfile, "# ciphertext samples from VIP-Bench slice-n-dice\n");
  fprintf(outfile, "type: d\n");
  fprintf(outfile, "count: %lu\n", samples);
  fprintf(outfile, "numbit: 32\n");

  {
    Stopwatch s("VIP_Bench Runtime");
    slice_n_dice(outfile, samples);
  }
  fprintf(stdout, "INFO: slice-n-dice ciphertext sampling is complete.\n");

  // close the ciphertext sample file
  fclose(outfile);
  return 0;
}
