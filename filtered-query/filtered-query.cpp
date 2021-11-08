#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <memory.h>
#include <vector>

using namespace std;

#include "kissdb.h"
#include "rapidcsv.h"


int
main(int argc,char **argv)
{
  uint64_t i,j;
  uint64_t v[8];
  KISSDB db;
  KISSDB_Iterator dbi;
  char got_all_values[10000];
  int q;

  printf("Reading homesales.csv...\n");
  rapidcsv::Document doc("homesales.csv");

  vector<float> col = doc.GetColumn<float>("price");
  cout << "Read " << col.size() << " values." << std::endl;

  printf("Opening new empty database test.db...\n");

  if (KISSDB_open(&db,"test.db",KISSDB_OPEN_MODE_RWREPLACE,1024,8,sizeof(v))) {
    printf("KISSDB_open failed\n");
    return 1;
  }

  printf("Adding and then re-getting 10000 64-byte values...\n");

  for(i=0;i<10000;++i) {
    for(j=0;j<8;++j)
      v[j] = i;
    if (KISSDB_put(&db,&i,v)) {
      printf("KISSDB_put failed (%" PRIu64 ")\n",i);
      return 1;
    }
    memset(v,0,sizeof(v));
    if ((q = KISSDB_get(&db,&i,v))) {
      printf("KISSDB_get (1) failed (%" PRIu64 ") (%d)\n",i,q);
      return 1;
    }

    for(j=0;j<8;++j) {
      if (v[j] != i) {
        printf("KISSDB_get (1) failed, bad data (%" PRIu64 ")\n",i);
        return 1;
      }
    }
  }

  printf("Getting 10000 64-byte values...\n");

  for(i=0;i<10000;++i) {
    if ((q = KISSDB_get(&db,&i,v))) {
      printf("KISSDB_get (2) failed (%" PRIu64 ") (%d)\n",i,q);
      return 1;
    }
    for(j=0;j<8;++j) {
      if (v[j] != i) {
        printf("KISSDB_get (2) failed, bad data (%" PRIu64 ")\n",i);
        return 1;
      }
    }
  }

  printf("Closing and re-opening database in read-only mode...\n");

  KISSDB_close(&db);

  if (KISSDB_open(&db,"test.db",KISSDB_OPEN_MODE_RDONLY,1024,8,sizeof(v))) {
    printf("KISSDB_open failed\n");
    return 1;
  }

  printf("Getting 10000 64-byte values...\n");

  for(i=0;i<10000;++i) {
    if ((q = KISSDB_get(&db,&i,v))) {
      printf("KISSDB_get (3) failed (%" PRIu64 ") (%d)\n",i,q);
      return 1;
    }
    for(j=0;j<8;++j) {
      if (v[j] != i) {
        printf("KISSDB_get (3) failed, bad data (%" PRIu64 ")\n",i);
        return 1;
      }
    }
  }

  printf("Iterator test...\n");

  KISSDB_Iterator_init(&db,&dbi);
  i = 0xdeadbeef;
  memset(got_all_values,0,sizeof(got_all_values));
  while (KISSDB_Iterator_next(&dbi,&i,&v) > 0) {
    if (i < 10000)
      got_all_values[i] = 1;
    else {
      printf("KISSDB_Iterator_next failed, bad data (%" PRIu64 ")\n",i);
      return 1;
    }
  }
  for(i=0;i<10000;++i) {
    if (!got_all_values[i]) {
      printf("KISSDB_Iterator failed, missing value index %" PRIu64 "\n",i);
      return 1;
    }
  }

  KISSDB_close(&db);

  printf("All tests OK!\n");

  return 0;
}

