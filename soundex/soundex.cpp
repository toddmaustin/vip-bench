#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string>
#include <vector>
using namespace std;

#include "utils.h"

// include build configuration defines
#include "../config.h"

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

vector<string> namesA = {"Johnson", "Adams", "Davis", "Simons", "Richards", "Taylor", "Carter", "Stevenson", "Taylor", "Smith", "McDonald", "Harris", "Sim", "Williams", "Baker", "Wells", "Fraser", "Jones", "Wilks", "Hunt", "Sanders", "Parsons", "Robson", "Harker"};
vector<string> namesB = {"Jonson", "Addams", "Davies", "Simmons", "Richardson", "Tailor", "Chater", "Stephenson", "Naylor", "Smythe", "MacDonald", "Harrys", "Sym", "Wilson", "Barker", "Wills", "Frazer", "Johns", "Wilkinson", "Hunter", "Saunders", "Pearson", "Robertson", "Parker"};

void
string_to_soundex(string& name, string& sndex)
{
  int si = 1;
  int c;

  //                 ABCDEFGHIJKLMNOPQRSTUVWXYZ
  char mappings[] = "01230120022455012623010202";

  sndex[0] = toupper(name[0]);

  for (unsigned i = 1, len = name.size(); i < len; i++)
  {
    c = (int)toupper(name[i]) - 65;

    if (c >= 0 && c <= 25)
    {
      if (mappings[c] != '0')
      {
        if (mappings[c] != sndex[si-1])
        {
          sndex[si] = mappings[c];
          si++;
        }

        if (si > 3)
          break;
      }
    }
  }

  if (si <= 3)
  {
    while(si <= 3)
    {
      sndex[si] = '0';
      si++;
    }
  }
}


int
main(void)
{
  // initialize the privacy enhanced execution target
  VIP_INIT;

  vector<string> sndexA(namesA.size());
  vector<string> sndexB(namesB.size());

  {
    Stopwatch s("VIP_Bench Runtime");
    for (unsigned i = 0; i < sndexA.size())
    {
      string_to_soundex(namesA[i], sndexA[i]);
      string_to_soundex(namesB[i], sndexB[i]);
    }
  }

  // print the results
  for (unsigned i=0; i < sndexA.size(); i++)
  {
  }

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


#ifdef notdef
void main(void)
{
    puts("-----------------");
    puts("| codedrome.com |");
    puts("| Soundex       |");
    puts("-----------------\n");

    char* names1[] = {"Johnson", "Adams", "Davis", "Simons", "Richards", "Taylor", "Carter", "Stevenson", "Taylor", "Smith", "McDonald", "Harris", "Sim", "Williams", "Baker", "Wells", "Fraser", "Jones", "Wilks", "Hunt", "Sanders", "Parsons", "Robson", "Harker"};

    char* names2[] = {"Jonson", "Addams", "Davies", "Simmons", "Richardson", "Tailor", "Chater", "Stephenson", "Naylor", "Smythe", "MacDonald", "Harrys", "Sym", "Wilson", "Barker", "Wills", "Frazer", "Johns", "Wilkinson", "Hunter", "Saunders", "Pearson", "Robertson", "Parker"};

    int namecount = sizeof(names1) / sizeof(names1[0]);

    char s1[] = "     ";
    char s2[] = "     ";

    for(int i = 0; i < namecount; i++)
    {
        soundex(names1[i], s1);
        soundex(names2[i], s2);

        printf("%-20s%-6s%-20s%-6s\n", names1[i], s1, names2[i], s2);
    }
}
#endif
