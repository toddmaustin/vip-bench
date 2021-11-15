#include <stdio.h>
#include <stdlib.h>

#include "../config.h"
#include "panTompkins.h"

int
main(int argc, char **argv)
{
  if (argc != 3)
  {
    fprintf(stderr, "USAGE: qrs-detect <infile> <outfile>\n");
    exit(1);
  }

  char *infile = argv[1];
  char *outfile = argv[2];

  init(infile, outfile);
  panTompkins();
  return 0;
}

