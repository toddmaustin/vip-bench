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

#define SIZE  20
#define MAX numeric_limits<int>::max()


VIP_ENCINT elements1[SIZE] = {25,97,1,0,36,22,74,3,9,12,30,81,63,148,99,13,64,49,80,15};
VIP_ENCINT elements2[SIZE] = {25,97,1,0,36,22,74,3,99,12,30,81,63,148,99,13,64,49,99,15};

VIP_ENCBOOL
isDistinct(VIP_ENCINT elements[], VIP_ENCINT &dup)
{
#ifdef VIP_DO_MODE
  dup = MAX;

  for (int i=SIZE-1; i >= 0; i--)
  {
    for (int j=0; j < SIZE; j++)
      dup = VIP_CMOV(elements[i] == elements[j] && i!=j, elements[j], dup);
  }

  return (dup == MAX);
#else /* !VIP_DO_MODE */
  VIP_ENCINT tree[SIZE][3];
  dup = MAX;

	for(int i = 0; i < SIZE;i++){
		tree[i][0] = MAX;
		tree[i][1] = MAX;
		tree[i][2] = MAX;
	}	
	
	tree[0][0] = elements[0];
	VIP_ENCINT location = 0;

	for(int i = 1; i < SIZE; i++){
		VIP_ENCINT temp = 0;
		VIP_ENCINT value = tree[0][0];
		VIP_ENCINT left = tree[0][1];
		VIP_ENCINT right = tree[0][2];

		while(value != MAX){
			if(elements[i] > value){
				if(right != MAX){
					value = tree[right][0];
					left = tree[right][1];
					temp = right;
					right = tree[right][2];
				}else{
					tree[location+1][0] = elements[i];
					tree[temp][2] = location + 1;
					location++;
					break;
				}
			}else if(elements[i] < value){
				if(left != MAX){
					value = tree[left][0];
					right = tree[left][2];
					temp = left;
					left = tree[left][1];
				}else{
					tree[temp][1] = location+1;
					tree[location+1][0] = elements[i];
					location++;
					break;
				}
			}else{
        dup = elements[i];
				return false;
			}
		}
	}
	return true;
#endif /* VIP_DO_MODE */
}

int
main(void)
{
  VIP_ENCINT dup;

  if (VIP_DEC(isDistinct(elements1, dup)))
		cout<<"The elements of `elements1' are distinct"<<endl;
	else
		cout<<"The elements of `elements1' are not distinct (e.g., " << VIP_DEC(dup) << " is duplicated)"<<endl;
	
  if (VIP_DEC(isDistinct(elements2, dup)))
		cout<<"The elements of `elements2' are distinct"<<endl;
	else
		cout<<"The elements of `elements2' are not distinct (e.g., " << VIP_DEC(dup) << " is duplicated)"<<endl;

	return 0;
}

