#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <memory.h>
#include <vector>

using namespace std;

#include "../config.h"
#include "mathlib.h"
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

enum op_t { op_avg, op_stddev };
enum field_t {field_price, field_pricesqft };
enum bedsize_t { bedsize_small, bedsize_large, bedsize_all };

double
filtered_query(KISSDB *pdb, unsigned zipcode, enum op_t op, enum field_t field, enum bedsize_t bedsize, double avg)
{
  struct salesrec_t rec;

  double sum = 0.0;
  unsigned count = 0;

  for (unsigned j=0; ; j++)
  {
    uint64_t key = KEYFMT(zipcode, j);
    if (KISSDB_get(pdb, &key, &rec))
    {
      // no more entries in this zip code
      break;
    }
    // fprintf(stderr, "<zip,i> = <%u,%u>\n", zipcodes[i], j);

    // process this entry
    if ((bedsize == bedsize_all)
        || (bedsize == bedsize_small && rec.beds < 3)
        || (bedsize == bedsize_large && rec.beds >= 3))
    {
      double val;
      count++;
      if (field == field_price)
        val = rec.price;
      else if (field == field_pricesqft)
        val = (double)rec.price/(double)rec.sqft;

      if (op == op_avg)
        sum += val;
      else if (op == op_stddev)
        sum += ((val - avg) * (val - avg));
    }
  }

  // compute the final statistic
  double retval;
  if (op == op_avg && count > 0)
    retval = sum / (double)count;
  else if (op == op_stddev && count > 1)
    retval = mysqrt(sum / (double)(count-1));
  else
    retval = 0.0;

  return retval;
}

void
filtered_query_all(KISSDB *pdb, enum op_t op, enum field_t field, enum bedsize_t bedsize, double avg, vector<double> v)
{
  for (unsigned i=0; i < zipcodes.size(); i++)
  {
    v[i] = filtered_query(pdb, zipcodes[i], op, field, bedsize, avg);
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
    rec.zip = doc.GetCell<unsigned>(0, i);
    rec.beds = doc.GetCell<VIP_ENCUINT>(1, i);
    rec.baths = doc.GetCell<VIP_ENCUINT>(2, i);
    rec.sqft = doc.GetCell<VIP_ENCUINT>(3, i);
    rec.price = doc.GetCell<VIP_ENCDOUBLE>(4, i);

    // compute the key val
    if (currentZip != rec.zip)
    {
      currentZip = rec.zip;
      zipCnt = 0;
    }
    key = KEYFMT(rec.zip, zipCnt);
    zipCnt++;

    // print_salesrec(stderr, &rec);

    if (KISSDB_put(&db, &key, &rec)) {
      printf("KISSDB_put failed (%u)\n", i);
      exit(1);
    }
  }

  printf("INFO: Closing and re-opening database in read-only mode...\n");
  KISSDB_close(&db);

  if (KISSDB_open(&db,"homesales.db", KISSDB_OPEN_MODE_RDONLY, 1024, sizeof(uint64_t), sizeof(rec))) {
    printf("KISSDB_open failed\n");
    exit(1);
  }


  // compute average/stddev price for zip code XXXXXX 
  double avg = filtered_query(&db, 95864, op_avg, field_price, bedsize_all, 0.0);
  fprintf(stdout, "STAT: average price for zipcode 95864: $%.2lf\n", avg);

  double stddev = filtered_query(&db, 95864, op_stddev, field_price, bedsize_all, avg);
  fprintf(stdout, "STAT: average price for zipcode 95864: $%.2lf\n", stddev);

  // compute average/stddev price for all zip codes

  // compute average/stddev price/sqft for zip code XXXXXX 
  avg = filtered_query(&db, 95864, op_avg, field_pricesqft, bedsize_all, 0.0);
  fprintf(stdout, "STAT: average price/sqft for zipcode 95864: $%.2lf\n", avg);

  stddev = filtered_query(&db, 95864, op_stddev, field_pricesqft, bedsize_all, avg);
  fprintf(stdout, "STAT: average price/sqft for zipcode 95864: $%.2lf\n", stddev);


  // compute average/stddev price/sqft for all zip codes

  // find the most expensive zip code in the Sacremento area, by average price/sqft

  // find the least expensive zip code in the Sacremento area, by average home price

  KISSDB_close(&db);

  return 0;
}

