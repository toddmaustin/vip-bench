

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
  









  /*** GWAS COMPUTATION ***/
  //Calculate constant scalar r1
  VIP_ENCDOUBLE r1 = 0;
  cout << sData.size() << endl;
  cout << sData[0].size() << endl;
  cout << sData.size() << endl;
  for (int i=0; i<yData.size(); i++)
  {
      r1 = r1 + 2*(yData[i]*yData[i]);
  }

  for (int i=0; i<M; i++) //Loop over M SNPs
  {
    //Calculate scalar n_11
    VIP_ENCDOUBLE n_11 = 0;
    for (int j=0; j<N; j++)
    {
      n_11 = n_11 + yData[j]*sData[j][i];
    }
    //Calculate scalar c1
    VIP_ENCDOUBLE c1 = 0; 
    for (int j=0; j<N; j++)
    {
      c1 = c1 + sData[j][i];  // Sum all values of SNP[i] for the N indivuduals
    }
    
    //Calculate chi2 and or for SNP[i]
    VIP_ENCDOUBLE chi2_i_num = 2*n_11*N - c1*r1;
    chi2_i_num = 2*N*chi2_i_num*chi2_i_num; //Instead of using pow...
    VIP_ENCDOUBLE chi2_i = (chi2_i_num)/((c1*(2*N - c1)*r1 * (2*N - r1)));      
    cout << "INFO: chi2(" << headersS[i] << ") == " << VIP_DEC(chi2_i) << endl;

    VIP_ENCDOUBLE or_i = (n_11*(n_11 - r1 - c1 + 2*N))/((c1 - n_11)*(r1 - n_11));
    cout << "INFO: or(" << headersS[i] << ") == " << VIP_DEC(or_i) << endl;
  }
  /*** END GWAS COMPUTATION ***/


  return 0;
} 

