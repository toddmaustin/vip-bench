#include <iostream>
#include <iomanip>
#include <cstdint>
#include <vector>

using namespace std;

#include "../config.h"
#include "utils.h"
 
// sizes support 10 (default), 15, 20, 25
#define SIZE 25

// `M × N` matrix
#define M SIZE
#define N SIZE

// matrix showing portion of the screen having different colors
VIP_ENCCHAR mat[M][N] =
{
#if SIZE == 10
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X' }
#elif SIZE == 15
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#elif SIZE == 20
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#elif SIZE == 25
  { 'Y', 'Y', 'Y', 'G', 'G', 'G', 'G', 'G', 'G', 'G', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Y', 'Y', 'Y', 'Y', 'Y', 'Y', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'G', 'X', 'G', 'G', 'G', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'X', 'X', 'W', 'W', 'G', 'G', 'G', 'G', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'R', 'R', 'R', 'R', 'R', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'W', 'W', 'R', 'R', 'G', 'G', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'W', 'R', 'R', 'R', 'R', 'R', 'R', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'B', 'B', 'R', 'R', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'B', 'B', 'B', 'B', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'W', 'B', 'B', 'X', 'X', 'X', 'X', 'X', 'X', 'X', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
  { 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q', 'Q' },
#endif
};
	
//Used to find adjacent values of an element in a matrix
int row[] = { -1, -1, -1, 0, 0, 1, 1, 1 };
int col[] = { -1, 0, 1, -1, 1, -1, 0, 1 }; 
VIP_ENCINT countP = 0;
VIP_ENCINT place = 1;
vector<int> primeNum(M*N,0);
int groupIndex=0;
#ifndef VIP_NA_MODE 
 struct node{
 	VIP_ENCULONG sub_group=1;//points to a value in the array called group which is created below
 	VIP_ENCULONG group = 1;
 	VIP_ENCBOOL assigned = false;
 };
 
//same size as the given matrix
//An element with location(x,y) here represents the element(x,y) in the input matrix
//Holds the groups of the elements in the input matrix
node struct_mat[M][N];

//Can use this function to see the groups and subgroup
void printStructMatrix(node struct_mat[M][N])
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      cout << setw(6) <<"("<<VIP_DEC(struct_mat[i][j].sub_group)<<","<<VIP_DEC(struct_mat[i][j].group)<<")";
    }
    cout << endl;
  }
}

//checks if small is a factor of big
VIP_ENCBOOL isFactor(VIP_ENCDOUBLE big,VIP_ENCDOUBLE small){
	VIP_ENCDOUBLE temp= big/small;
	VIP_ENCINT temp1 = big/small;
	return (temp-temp1)==0;
}

//generates primeNumbers in a random way
void generatePrime(){
	int index=0;
	vector<int> out(M*N,0);
	bool found = false;
	for(int i = 2;true;i++){
		for(int j=0;j<M*N;j++){
			if(primeNum[j]==0){
				break;
			}
			if(i%primeNum[j]==0){
				found=true;
				break;
			}
		}
		if(!found){	
			primeNum[index]=i;
			index++;
			if(index==M*N)
				break;
		}
		found=false;
	}
	index=rand()%(M*N);
	for(int i=0;i<M*N;i++){
		while(out[index]!=0){
			index=rand()%(M*N);
		}
		out[index]=primeNum[i];
	}
	primeNum=out;
}
#endif


void floodfill(VIP_ENCCHAR mat[M][N], VIP_ENCINT x, VIP_ENCINT y, VIP_ENCCHAR replacement){	
	
	#ifndef VIP_NA_MODE
		#define SAFELOC(X, Y) ((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N)
		
		//Used for the condition value in VIP_CMOV		
		VIP_ENCBOOL condition = false;
		//Set to true if at least one element adjacent to the 
		//element in question has been assigned and has the same color
		VIP_ENCBOOL found = false;
		VIP_ENCULONG temp=0;					
		
		for(int i = 0;i < M;i++){
			for(int j=0;j<N;j++){		
				for (int k=0; k < 8; k++){
		      		if (SAFELOC(i+row[k], j+col[k])){
		      			
						//Set to true if at least one element adjacent to the 
						//element in question has been assigned and has the same color	      				
		      			condition=(struct_mat[i+row[k]][j+col[k]].assigned && mat[i][j] == mat[i+row[k]][j+col[k]]);
		      			
		      			//The moment condition is set to true this value become true until it is reset to false later on
		      			found = VIP_CMOV(condition,condition,found);
		   				
		   				//Assigns the group of the current element to the adjacent group if condition is true and
		   				//It is assigned any group yet
		   				struct_mat[i][j].sub_group = VIP_CMOV((condition&&!struct_mat[i][j].assigned),struct_mat[i+row[k]][j+col[k]].sub_group,struct_mat[i][j].sub_group);
		   				struct_mat[i][j].group = VIP_CMOV((condition&&!struct_mat[i][j].assigned),struct_mat[i+row[k]][j+col[k]].group,struct_mat[i][j].group);
		   					   						   					   				
		   				//If the condition is true but the current element is already assigned an element then
		   				//the all element in the adjacent's group is changed to the current elements group		   				
		   				temp = struct_mat[i][j].group*struct_mat[i+row[k]][j+col[k]].group;  				       		
		   				struct_mat[i+row[k]][j+col[k]].group= VIP_CMOV(condition&&struct_mat[i][j].assigned&&!isFactor(struct_mat[i][j].group,struct_mat[i+row[k]][j+col[k]].group)&&struct_mat[i][j].sub_group!=struct_mat[i+row[k]][j+col[k]].sub_group,temp,struct_mat[i+row[k]][j+col[k]].group);
		   				struct_mat[i][j].group= VIP_CMOV(condition&&struct_mat[i][j].assigned&&!isFactor(struct_mat[i][j].group,struct_mat[i+row[k]][j+col[k]].group)&&struct_mat[i][j].sub_group!=struct_mat[i+row[k]][j+col[k]].sub_group,temp,struct_mat[i][j].group);		   			
		   				
		   				temp = VIP_CMOV(struct_mat[i][j].group>struct_mat[i+row[k]][j+col[k]].group,struct_mat[i][j].group,struct_mat[i+row[k]][j+col[k]].group);
		   				struct_mat[i][j].group = VIP_CMOV(condition&&struct_mat[i][j].assigned&&struct_mat[i][j].sub_group==struct_mat[i+row[k]][j+col[k]].sub_group,temp,struct_mat[i][j].group);
		   				struct_mat[i+row[k]][j+col[k]].group = VIP_CMOV(condition&&struct_mat[i][j].assigned&&struct_mat[i][j].sub_group==struct_mat[i+row[k]][j+col[k]].sub_group,temp,struct_mat[i+row[k]][j+col[k]].group);				
		   				
		   				//If conditon was set to true the the current element will be assigned(true)          	
			       		struct_mat[i][j].assigned = VIP_CMOV(condition,(VIP_ENCBOOL)true,struct_mat[i][j].assigned);	       		          	          
		   			}
			  }					  				    		
		    		//If no adjacent value that fullfils the condition is found then it will be assigned a new group
		    		struct_mat[i][j].sub_group = VIP_CMOV(!found,(VIP_ENCULONG)primeNum[groupIndex],struct_mat[i][j].sub_group);
		    		struct_mat[i][j].group = VIP_CMOV(!found,struct_mat[i][j].sub_group,struct_mat[i][j].group);
		    		
		    		//all value are assigned after the finish this phase
		    		struct_mat[i][j].assigned = true;
		    		//found variable is reset for the next iteration
		    		found=false;
		    		groupIndex++;
			}	
		}
		
		//Holds the value of the group to be changed
		VIP_ENCULONG targetGr = 1;
		for (int ix=0; ix < M; ix++){
			for (int iy=0; iy < N; iy++){
				VIP_ENCBOOL _istarget = (x == ix && y == iy);
				targetGr = VIP_CMOV(_istarget,struct_mat[ix][iy].group,targetGr);
				targetGr = VIP_CMOV(condition&&isFactor(struct_mat[ix][iy].group,targetGr),struct_mat[ix][iy].group,targetGr);
				condition = VIP_CMOV(_istarget,(VIP_ENCBOOL)true,condition);

			}
		}
		
		//This loop finds each element that is in the target group and assigns it with the replacemnt color	  
		for(int i=0;i<M;i++){
		  	for(int j=0;j<N;j++){		  	
		  		condition = (isFactor(targetGr,struct_mat[i][j].sub_group));		  		
		  		mat[i][j] = VIP_CMOV(condition,replacement,mat[i][j]); 		
		  	}
		}
	#else
		#define ISSAFE(MAT, X, Y, TARGET)     (((X) >= 0 && (X) < M && (Y) >= 0 && (Y) < N) && mat[(X)][(Y)] == (TARGET))
		VIP_ENCCHAR target = mat[x][y];
 
		mat[x][y] = replacement;

		for (int k = 0; k < 8; k++)
		{
			if (ISSAFE(mat, x + row[k], y + col[k], target))
		    {
			    floodfill(mat, x + row[k], y + col[k], replacement);
    		}
  		}
	#endif
	
}


void
printMatrix(VIP_ENCCHAR mat[M][N])
{
  for (int i = 0; i < M; i++)
  {
    for (int j = 0; j < N; j++)
    {
      cout << setw(3) << VIP_DEC(mat[i][j]);
    }
    cout << endl;
  }
}

int main(){

	VIP_ENCINT x = 3, y = 9;
	VIP_ENCCHAR replacement = 'C';
	#ifndef VIP_NA_MODE
		generatePrime();
	#endif

	cout<<"\nBEFORE flooding `"<<VIP_DEC(replacement)<<"' @ "<<"("<<VIP_DEC(x)<<","<<VIP_DEC(y)<<"):\n";
	printMatrix(mat);
	{
		Stopwatch start("VIP-bench runtime: ");
		floodfill(mat,x, y, replacement);
	}
	cout<<"\nAFTER:"<<endl;	
	printMatrix(mat);
}
