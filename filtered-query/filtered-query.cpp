#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <memory.h>
#include <vector>

using namespace std;

#include "../config.h"
#include "kissdb.h"
#include "rapidcsv.h"

struct salesrec_t {
  VIP_ENCUINT zip;
  VIP_ENCUINT beds;
  VIP_ENCUINT baths;
  VIP_ENCUINT sqft;
  VIP_ENCDOUBLE price;
};

// key format is zip:cnt
#define KEYFMT(zip,cnt)   (((uint64_t)zip << 32) + (uint64_t)cnt)

void
print_salesrec(FILE *fd, struct salesrec_t *rec)
{
  fprintf(fd, "{ zip:%u, beds:%u, baths:%u, sqft:%u, price:$%.2lf }\n", rec->zip, rec->beds, rec->baths, rec->sqft, rec->price);
}

vector<unsigned> zipcodes =
{
  95603, 95608, 95610, 95614, 95619, 95621, 95623, 95624, 95626, 95628,
  95630, 95631, 95632, 95633, 95635, 95648, 95650, 95655, 95660, 95661,
  95662, 95663, 95667, 95670, 95673, 95677, 95678, 95682, 95683, 95690,
  95691, 95693, 95722, 95726, 95742, 95746, 95747, 95757, 95758, 95762,
  95765, 95811, 95814, 95815, 95816, 95817, 95818, 95819, 95820, 95821,
  95822, 95823, 95824, 95825, 95826, 95827, 95828, 95829, 95831, 95832,
  95833, 95834, 95835, 95838, 95841, 95842, 95843, 95864
};

void
filtered_query(KISSDB *pdb, enum { op_avg, op_stddev } op, enum { field} field, vector<double> v)
{
  struct salesrec_t rec;

  for (unsigned i=0; i < zipcodes.size(); i++)
  {
    for (unsigned j=0; ; j++)
    {
      uint64_t key = KEYFMT(zipcodes[i], j);
      if (KISSDB_get(&db, &key, &rec))
      {
        // no more entries in this zip code
        break;
      }
      // fprintf(stderr, "<zip,i> = <%u,%u>\n", zipcodes[i], j);

      // process this entry
      if (filter_fn(&rec))
      {
      }
    }
  }
}

int
main(int argc,char **argv)
{
  KISSDB db;
  KISSDB_Iterator dbi;
  struct salesrec_t rec;

  printf("INFO: Generating homesales.db...\n");

  if (KISSDB_open(&db, "homesales.db", KISSDB_OPEN_MODE_RWREPLACE, 1024, sizeof(uint64_t), sizeof(rec)))
  {
    printf("KISSDB_open failed\n");
    return 1;
  }

  rapidcsv::Document doc("homesales.csv");
  fprintf(stderr, "INFO: data set size: %lu\n", doc.GetRowCount());

  unsigned currentZip = -1;
  unsigned zipCnt = 0;
  uint64_t key = 0;
  for (unsigned i=0; i < doc.GetRowCount(); i++)
  {
    rec.val.zip = doc.GetCell<unsigned>(0, i);
    rec.val.beds = doc.GetCell<VIP_ENCUINT>(1, i);
    rec.val.baths = doc.GetCell<VIP_ENCUINT>(2, i);
    rec.val.sqft = doc.GetCell<VIP_ENCUINT>(3, i);
    rec.val.price = doc.GetCell<VIP_ENCDOUBLE>(4, i);

    // compute the key val
    if (currentZip != rec.val.zip)
    {
      currentZip = rec.val.zip;
      zipCnt = 0;
    }
    key = KEYFMT(rec.val.zip, zipCnt);
    zipCnt++;

    // print_salesrec(stderr, &rec);

    if (KISSDB_put(&db, &key, &rec)) {
      printf("KISSDB_put failed (%u)\n", i);
      exit(1);
    }
  }

#ifdef notdef
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
#endif /* notdef */

  printf("INFO: Closing and re-opening database in read-only mode...\n");
  KISSDB_close(&db);

  if (KISSDB_open(&db,"homesales.db", KISSDB_OPEN_MODE_RDONLY, 1024, sizeof(uint64_t), sizeof(rec))) {
    printf("KISSDB_open failed\n");
    exit(1);
  }


  // compute average/stddev price for zip code XXXXXX 

  // compute average/stddev price for all zip codes

  // compute average/stddev price/sqft for zip code XXXXXX 

  // compute average/stddev price/sqft for all zip codes

  // find the most expensive zip code in the Sacremento area, by average price/sqrt

  // find the least expensive zip code in the Sacremento area, by average home price

#ifdef notdef
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
#endif /* notdef */

  KISSDB_close(&db);

  printf("All tests OK!\n");

  return 0;
}

