/*
 ELEMENT DISTINCTNESS ALGORITHM
 There are multiple ways of detecting whether the elements are distinct or not.
 One of the ways is to sort the elements in the array and check if the elements next to each other
 are equal or not.
 On the other hand an n^2 loop can be used to select an element from the array and check if the
 the element exists.
 The approach used in this algorithm is adding the elements in an array implementation of a binary tree
 If the elements already exists in the tree then the elements are not distinct else it is. The function
 performing the algorithm is isDistinct function.
*/
#include <iostream>
#include <limits>

#include "../config.h"

using namespace std;

#define SIZE  256
#define MAX numeric_limits<int>::max()

VIP_ENCINT elements1[SIZE] = {
  25,97,1,0,36,22,74,3,9,12,30,81,63,148,99,13,64,49,80,15,
  125,197,101,100,136,122,174,103,109,112,130,181,163,1148,199,113,164,149,180,115,
  225,297,201,200,236,222,274,203,209,212,230,281,263,2148,299,213,264,249,280,215,
  325,397,301,300,336,322,374,303,309,312,330,381,363,3148,399,313,364,349,380,315,
  425,497,401,400,436,422,474,403,409,412,430,481,463,4148,499,413,464,449,480,415,
  525,597,501,500,536,522,574,503,509,512,530,581,563,5148,599,513,564,549,580,515,
  625,697,601,600,636,622,674,603,
  1554, 1521, 1372, 1616, 1996, 1677, 1283, 1821, 1730, 1725, 1072, 1358, 1726, 1733, 1643,
  1484, 1127, 1671, 1582, 1259, 1734, 1440, 1961, 1669, 1859, 1417, 1625, 1056, 1003, 1370,
  1665, 1842, 1132, 1870, 1382, 1389, 1257, 1851, 1598, 1742, 1575, 1175, 1426, 1718, 1775,
  1926, 1378, 1432, 1910, 1247, 1919, 1558, 1287, 1062, 1868, 1736, 1149, 1270, 1066, 1646,
  1526, 1070, 1281, 1242, 1204, 1054, 1363, 1343, 1988, 1967, 1962, 1267, 1794, 1636, 1475,
  1121, 1771, 1688, 1640, 1114, 1986, 1181, 1028, 1328, 1055, 1569, 1384, 1620, 1564, 1561,
  1217, 1970, 1819, 1211, 1462, 1192, 1316, 1401, 1865, 1846, 1482, 1787, 1530, 1137, 1944,
  1707, 1860, 1377, 1155, 1544, 1604, 1385, 1449, 1039, 1152, 1117, 1840, 1632, 1531, 1837,
  1359, 1138, 1236, 1246, 1683, 1935, 1814, 1408
};
VIP_ENCINT elements2[SIZE] = {
  25,97,1,0,36,22,74,3,99,12,30,81,63,148,99,13,64,49,99,15,
  125,197,101,100,136,122,174,103,199,112,130,181,163,1148,119,113,164,149,199,115,
  225,297,201,200,236,222,274,203,299,212,230,281,263,2148,229,213,264,249,299,215,
  325,397,301,300,336,322,374,303,399,312,330,381,363,3148,339,313,364,349,399,315,
  425,497,401,400,436,422,474,403,409,412,430,481,463,4148,499,413,464,449,480,415,
  525,597,501,500,536,522,574,503,509,512,530,581,563,5148,599,513,564,549,580,515,
  625,697,601,600,636,622,674,603,
  1554, 1521, 1372, 1616, 1996, 1677, 1283, 1821, 1730, 1725, 1072, 1358, 1726, 1733, 1643,
  1484, 1127, 1671, 1582, 1259, 1734, 1440, 1961, 1669, 1859, 1417, 1625, 1056, 1003, 1370,
  1665, 1842, 1132, 1870, 1382, 1389, 1257, 1851, 1598, 1742, 1575, 1175, 1426, 1718, 1775,
  1926, 1378, 1432, 1910, 1247, 1919, 1558, 1287, 1062, 1868, 1736, 1149, 1270, 1066, 1646,
  1526, 1070, 1281, 1242, 1204, 1054, 1363, 1343, 1988, 1967, 1962, 1267, 1794, 1636, 1475,
  1121, 1771, 1688, 1640, 1114, 1986, 1181, 1028, 1328, 1055, 1569, 1384, 1620, 1564, 1561,
  1217, 1970, 1819, 1211, 1462, 1192, 1316, 1401, 1865, 1846, 1482, 1787, 1530, 1137, 1944,
  1707, 1860, 1377, 1155, 1544, 1604, 1385, 1449, 1039, 1152, 1117, 1840, 1632, 1531, 1837,
  1359, 1138, 1236, 1246, 1683, 1935, 1814, 1408
};

// given an array arr of length n, this code sorts it in place
// all indices run from 0 to n-1
void
bitonicsort(VIP_ENCINT *data, unsigned size)
{
  for (unsigned k = 2; k <= size; k <<= 1) // k is doubled every iteration
  { 
    for (unsigned j = k/2; j > 0; j >>= 1) // j is halved at every iteration, with truncation of fractional parts
    { 
      for (unsigned i = 0; i < size; i++)
      { 
        unsigned l = (i ^ j);
#ifndef VIP_DO_MODE 
        if ((l > i) && ((((i & k) == 0) && (data[i] > data[l])) || (((i & k) != 0) && (data[i] < data[l]))) )
        { 
          VIP_ENCINT tmp = data[i];
          data[i] = data[l];
          data[l] = tmp;
        }
#else /* VIP_DO_MODE */
        VIP_ENCBOOL _pred = ((l > i) && ((((i & k) == 0) && (data[i] > data[l])) || (((i & k) != 0) && (data[i] < data[l]))) );
        VIP_ENCINT tmp = data[i]; 
        data[i] = VIP_CMOV(_pred, data[l], data[i]);
        data[l] = VIP_CMOV(_pred, tmp, data[l]);
#endif /* VIP_DO_MODE */
      }
    }
  }
}

VIP_ENCBOOL
isDistinct(VIP_ENCINT elements[], VIP_ENCINT &dup)
{
  // sort the data
  bitonicsort(elements, SIZE);

  // detect dups
#ifndef VIP_DO_MODE 
  dup = MAX;
  for (unsigned i=0; i < SIZE-1; i++)
  {
    if (elements[i] == elements[i+1])
    {
      dup = elements[i];
      return false;
    }
  }
  return true;
#else /* VIP_DO_MODE */
  VIP_ENCBOOL distinct = true;
  dup = MAX;
  for (int i=SIZE-2; i >= 0; i--)
  {
    VIP_ENCBOOL _pred = (elements[i] == elements[i+1]);
    dup = VIP_CMOV(_pred, elements[i], dup);
    distinct = VIP_CMOV(_pred, (VIP_ENCBOOL)false, distinct);
  }
  return distinct;
#endif /* VIP_DO_MODE */
}

int
main(void)
{
  VIP_ENCINT dup1, dup2;

  bool res1, res2;

  {
    Stopwatch s("VIP Distinctness check:");

    res1 = VIP_DEC(isDistinct(elements1, dup1));
    res2 = VIP_DEC(isDistinct(elements2, dup2));
  }

  if (res1)
		cout<<"The elements of `elements1' are distinct"<<endl;
	else
		cout<<"The elements of `elements1' are not distinct (e.g., " << VIP_DEC(dup1) << " is duplicated)"<<endl;
	
  if (res2)
		cout<<"The elements of `elements2' are distinct"<<endl;
	else
		cout<<"The elements of `elements2' are not distinct (e.g., " << VIP_DEC(dup2) << " is duplicated)"<<endl;

	return 0;
}

