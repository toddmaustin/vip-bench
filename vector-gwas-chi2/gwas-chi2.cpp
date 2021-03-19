

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <iomanip>
#include <math.h>
#include <string>
#include <iostream>
#include <fstream>
#include <getopt.h>
#include <numeric>
#include <cmath>
#include <bits/stdc++.h>
#include <stdint.h>
using namespace std;

#include "../config.h"


VIP_VEC_ENCDOUBLE BinaryTreeAdd(vector<VIP_VEC_ENCDOUBLE> &vector) 
{
	for(size_t j = 1; j < vector.size(); j=j*2) {
		for(size_t i = 0; i<vector.size(); i = i + 2*j) {
			if ((i+j)<vector.size())
				vector[i] = vector[i]+vector[i+j];
		}
	}

  VIP_VEC_ENCDOUBLE ret = vector[0];
	return ret;
}


int
main(void) 
{ 
  VIP_INIT; 


  /*** REVISIT FILE IO ***/
  // Hard coding command line ops 
  string filename = "snps.csv";
  string SampleSize =  "200";//"200";
  string SNPs =        "16384";//"16384";
  vector<string>                headersS;
  vector<VIP_ENCDOUBLE>         yData;
  vector<vector<VIP_ENCDOUBLE>> sData;

  int N = stoi(SampleSize);  // 200
  int M = stoi(SNPs);        // 16384...
  
  uint32_t cols = 0;
  ifstream file(filename);
  string line, value;
  
  if(file.good()) 
  {
    getline(file, line);
	cols = count(line.begin(), line.end(), ',') + 1;
	stringstream ss(line);
	vector<string> result;

	int tempcounter = 0;

	for(uint32_t i = 0; i < cols; i++) 
    {
	  string substr;
      getline(ss, substr, ',');
      if ((substr != "") && (i>4) && (i<M+5)) 
      {
		headersS.push_back(substr);
		tempcounter++;
	  }
	}
	cols = tempcounter;
  }
    

	int counter = 0;
	while((file.good()) && (counter < N)) {
		getline(file, line);
		uint32_t curCols = std::count(line.begin(), line.end(), ',') + 1;
		if (curCols > 2) {  // i don't get this
			stringstream ss(line); // line from file
			for(uint32_t i = 0; i < 5; i++) {
				string substr;
				getline(ss, substr, ',');
				if ((i==1))
					yData.push_back((VIP_ENCDOUBLE)std::stod(substr));
			}
			std::vector<VIP_ENCDOUBLE> row(cols);
			for(uint32_t i = 5; i < cols + 5; i++) {
				string substr;
				getline(ss, substr, ',');
				if (i < M+5)
				{
					row[i-5] = std::stod(substr);
				}
			}
			sData.push_back(row);
		}
		counter++;
	}
	file.close();

  /*** END REVISIT FILE IO ***/
  


  /*** Initialized PACKED Vectors for SIMD Computation ***/
  int max_vector_len = 4096; 
	int sizeS = (size_t)std::ceil((double)sData[0].size()/(max_vector_len)); //# of SNPS divided by # of slots

	vector<vector<VIP_VEC_ENCDOUBLE>> sDataArray(sizeS);

	for(size_t s = 0; s < sizeS; s++)
		sDataArray[s] = vector<VIP_VEC_ENCDOUBLE>(sData.size());

	for (size_t i=0; i < sData.size(); i++){

		for(size_t s = 0; s < sizeS; s++)
			sDataArray[s][i] = VIP_VEC_ENCDOUBLE(sData[i].size());

		size_t counter = 0;

		for (size_t j=0; j<sData[i].size(); j++) {
			if ((j>0) && (j%(max_vector_len)==0)){
				counter++;
      }
			sDataArray[counter][i][j%(max_vector_len)] = sData[i][j];
		}
	}

  // I think this is redundant, but used by palisade version...
  vector<vector<VIP_VEC_ENCDOUBLE>> S(sizeS);
	vector<VIP_VEC_ENCDOUBLE> Y(N);

	for (size_t i = 0; i < sizeS; i++)
		S[i] = vector<VIP_VEC_ENCDOUBLE>(N);

	for (size_t i=0; i<N; i++){
		for (size_t s=0; s < sizeS; s++){
			S[s][i] = sDataArray[s][i];
		}
		Y[i] = VIP_VEC_ENCDOUBLE(yData[i], max_vector_len);
	}
  /*** END Initialized PACKED Vectors... ***/



  /*** GWAS COMPUTATION ***/
  //Calculate constant d
	VIP_VEC_ENCDOUBLE d = VIP_VEC_ENCDOUBLE((2*N), max_vector_len); //[2N, 2N, 2N, ...]

  //Calculate r1
  vector<VIP_VEC_ENCDOUBLE> ySum = Y;
	VIP_VEC_ENCDOUBLE yU = BinaryTreeAdd(ySum);
  VIP_VEC_ENCDOUBLE r1 = yU + yU;

	for (int s = 0; s < sizeS; s++)   //Loop over s groups of M/s SNPs
  {
    //Calculate VECTOR n_11
		vector<VIP_VEC_ENCDOUBLE> ySMult(N);
		for(int i = 0; i < N; i++) 
    {
			ySMult[i] = (S[s][i])*Y[i]; 
		}
		VIP_VEC_ENCDOUBLE n_11 = BinaryTreeAdd(ySMult); 

    //Calculate VECTOR c1
    VIP_VEC_ENCDOUBLE c1 = BinaryTreeAdd(S[s]); 


    // DO NOT REMOVE BELOW OR THINGS BREAK
    // cout << endl;
    // DO NOT REMOVE ABOVE OR THINGS BREAK


    //Calculate chi2 and or for SNP vector
    VIP_VEC_ENCDOUBLE chi2_num = (n_11*d) - (c1*r1);
    VIP_VEC_ENCDOUBLE chi2a_num = d*chi2_num*chi2_num; //Instead of using pow...
    VIP_VEC_ENCDOUBLE chi2_s = (chi2a_num)/((c1*(d - c1)*r1 * (d - r1)));      

    VIP_VEC_ENCDOUBLE or_s = (n_11*(n_11 - r1 - c1 + d))/((c1 - n_11)*(r1 - n_11));

    for(int i=0; i<max_vector_len; i++)
    {
        cout << "INFO: chi2(" << headersS[(s*max_vector_len) + i] << ") == " << VIP_DEC(chi2_s[i]) << endl;
        cout << "INFO: or("   << headersS[(s*max_vector_len) + i] << ") == " << VIP_DEC(or_s[i]) << endl;

    }
  }
  /*** END GWAS COMPUTATION ***/


  return 0;
} 

