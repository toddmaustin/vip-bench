#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "utils.h"

// include build configuration defines
#include "../config.h"

// supported sizes: 256 (default), 512, 1024, 2048
#define DATASET_SIZE 256
VIP_ENCINT data[DATASET_SIZE];

// total swaps executed so far
unsigned long swaps = 0;


void
print_data(VIP_ENCINT *data, unsigned size)
{
  fprintf(stdout, "DATA DUMP:\n");
  for (unsigned i=0; i < size; i++)
    fprintf(stdout, "  data[%u] = %d\n", i, VIP_DEC(data[i]));
}

void
bubblesort(VIP_ENCINT *data, unsigned size)
{
  for (unsigned i=0; i < size-1; i++)
  {
#ifndef VIP_DO_MODE
    bool swapped = false;
#endif /* !VIP_DO_MODE */
    for (unsigned j=0; j < size-1; j++)
    {
#ifndef VIP_DO_MODE
      if (data[j] > data[j+1])
      {
        VIP_ENCINT tmp = data[j];
        data[j] = data[j+1];
        data[j+1] = tmp;
        swapped = true;
        swaps++;
      }
#else /* VIP_DO_MODE */
      VIP_ENCBOOL do_swap = data[j] > data[j+1];
      VIP_ENCINT tmp = data[j];
      data[j] = VIP_CMOV(do_swap, data[j+1], data[j]);
      data[j+1] = VIP_CMOV(do_swap, tmp, data[j+1]);
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

static void
show_usage(char *name)
{
  fprintf(stderr, "USAGE: %s <out-file-name> <sample-size>\n", name);
}


int
main(int argc, char* argv[])
{
  // initialize the privacy enhanced execution target
  VIP_INIT;

  // initialize the pseudo-RNG
  mysrand(42);
  // mysrand(time(NULL));

  if (argc != 3) {
      show_usage(argv[0]);
      return 1;
  }

  char *outFile = argv[1];
  uchar *outFile = argv[1];

int main(int argc, char* argv[])
{
    std::vector <std::string> sources;
    std::string destination;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if ((arg == "-h") || (arg == "--help")) {
            show_usage(argv[0]);
            return 0;
        } else if ((arg == "-d") || (arg == "--destination")) {
            if (i + 1 < argc) { // Make sure we aren't at the end of argv!
                destination = argv[i++]; // Increment 'i' so we don't get the argument as the next argv[i].
            } else { // Uh-oh, there was no argument to the destination option.
                  std::cerr << "--destination option requires one argument." << std::endl;
                return 1;
            }  
        } else {
            sources.push_back(argv[i]);
        }
    }
    return move(sources, destination);

  // initialize the array to sort
  for (unsigned i=0; i < DATASET_SIZE; i++)
    data[i] = myrand();
  print_data(data, DATASET_SIZE);

  {
    Stopwatch s("VIP_Bench Runtime");
    bubblesort(data, DATASET_SIZE);
  }
  print_data(data, DATASET_SIZE);

  // check the array
  for (unsigned i=0; i < DATASET_SIZE-1; i++)
  {
    if (VIP_DEC(data[i]) > VIP_DEC(data[i+1]))
    {
      fprintf(stdout, "ERROR: data is not properly sorted.\n");
      return -1;
    }
  }
  fprintf(stderr, "INFO: %lu swaps executed.\n", swaps);
  fprintf(stdout, "INFO: data is properly sorted.\n");
  return 0;
}
