/***
 * Copyright (c) 2020 Duality Technologies, Inc.
 * Licensed under the Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International Public License <https://creativecommons.org/licenses/by-nc-sa/4.0/legalcode>
 * See the LICENSE.md file for the full text of the license.
 * If you share the Licensed Material (including in modified form) you must include the above attribution in the copy you share.
 ***/
/*

Implementation for the Chi-Square GWAS solution described in
"Secure large-scale genome-wide association studies using homomorphic encryption"
by Marcelo Blatt, Alexander Gusev, Yuriy Polyakov, and Shafi Goldwasser

Command to execute the prototype
./demo-chi2 --SNPdir "../data" --SNPfilename "random_sample" --pvalue "pvalue.txt" --runtime "result.txt" --samplesize="200" --snps="16384"

*/

#include <getopt.h>
#include <numeric>
#include <cmath>

#include "palisade.h"

using namespace std;
using namespace lbcrypto;

const double EPSILON = 1.0E-08;

void RunChi2(const string &SNPDir, const string &SNPFileName,
		const string &pValue, const string &Runtime, const string &SampleSize, const string &SNPs);

Ciphertext<DCRTPoly> BinaryTreeAdd(std::vector<Ciphertext<DCRTPoly>> &vector);

void ReadSNPFile(vector<string>& headers, std::vector<std::vector<double>> & dataColumns, std::vector<double> &y, string dataFileName, size_t N, size_t M);

double BS(double z) {
	double y = exp(-z*z/2);
	return sqrt(1-y) * (31*y/200 - 341*y*y/8000) / sqrt(M_PI);
}

double normalCFD(double value) { return 0.5 * erfc(-value * M_SQRT1_2); }

double sf(double value) { return 1 - normalCFD(value); }

static bool Equal(double a, double b) {   return (EPSILON > fabs(a-b)); }

static bool Less(double a, double b) {  return ((a-b) < (-EPSILON)); }

static bool Greater(double a, double b) {   return ((a-b) > EPSILON); }

double IncompleteGamma(double val, double p);

int main(int argc, char **argv) {

	int opt;

	PalisadeParallelControls.Enable();

	static struct option long_options[] =
	  {
		/* These options dont set a flag.
		   We distinguish them by their indices. */
		{"SNPdir",  	required_argument, 			0, 'S'},
		{"SNPfilename",  	required_argument, 			0, 's'},
		{"pvalue",  	required_argument, 			0, 'p'},
		{"runtime",  	required_argument, 			0, 'r'},
		{"samplesize",  	required_argument, 			0, 'N'},
		{"snps",  	required_argument, 			0, 'M'},
		{0, 0, 0, 0}
	  };

	/* getopt_long stores the option index here. */
	int option_index = 0;

	string SNPDir;
	string SNPFileName;
	string pValue;
	string Runtime;
	string SampleSize;
	string SNPs;

	while ((opt = getopt_long(argc, argv, "S:s:p:r:N:M", long_options, &option_index)) != -1) {
		switch (opt)
		{
			case 'S':
				SNPDir = optarg;
				break;
			case 's':
				SNPFileName = optarg;
				break;
			case 'p':
				pValue = optarg;
				break;
			case 'r':
				Runtime = optarg;
				break;
			case 'N':
				SampleSize = optarg;
				break;
			case 'M':
				SNPs = optarg;
				break;
			default: /* '?' */
			  std::cerr<< "Usage: "<<argv[0]<<" <arguments> " <<std::endl
				   << "arguments:" <<std::endl
				   << "  -S --SNPdir SNP file directory"  <<std::endl
				   << "  -s --SNPfilename SNP file name"  <<std::endl
				   << "  -o --pvalue p-values file"  <<std::endl
				   << "  -r --runtime runtime output file name"  <<std::endl
				   << "  -N --samplesize number of individuals"  <<std::endl
			   	   << "  -M --snps number of SNPs"  <<std::endl;
			  exit(EXIT_FAILURE);
		}
	}

	RunChi2(SNPDir, SNPFileName, pValue, Runtime, SampleSize, SNPs);

	return 0;

}

void RunChi2(const string &SNPDir,
		const string &SNPFileName, const string &pValue, const string &Runtime, const string &SampleSize, const string &SNPs) {

	TimeVar t;
	TimeVar tAll;

	TIC(tAll);

	double keyGenTime(0.0);
	double encryptionTime(0.0);
	double computationTime(0.0);
	double decryptionTime(0.0);
	double endToEndTime(0.0);

	std::cout << "\n======CHI-SQUARE SOLUTION========\n" << std::endl;

	vector<string> headers1;
	vector<string> headersS;

	std::vector<double> yData;
	std::vector<std::vector<double>> sData;

	size_t N = std::stoi(SampleSize);
	size_t M = std::stoi(SNPs);

	ReadSNPFile(headersS,sData,yData,SNPDir + "/" + SNPFileName,N,M);

	usint m = 16384;

	usint init_size = 4;
	usint dcrtBits = 54;

	double scalingFactor = 2.5e-6;

	CryptoContext<DCRTPoly> cc =
		CryptoContextFactory<DCRTPoly>::genCryptoContextCKKS(
			   init_size-1,
			   dcrtBits,
			   0,
			   HEStd_128_classic,
			   m/2, /*ringDimension*/
			   APPROXRESCALE,
			   BV,
			   3, /*numLargeDigits*/
			   2, /*maxDepth*/
			   dcrtBits, /*firstMod*/
			   0,
			   OPTIMIZED);

	cc->Enable(ENCRYPTION);
	cc->Enable(SHE);
	cc->Enable(LEVELEDSHE);

	std::cout << "\nNumber of Individuals = " << sData.size() << std::endl;
	std::cout << "Number of SNPs = " << sData[0].size() << std::endl;

	TIC(t);

	auto keyPair = cc->KeyGen();

	keyGenTime = TOC(t);

	TIC(t);

	size_t sizeS = (size_t)std::ceil((double)sData[0].size()/(m/4));

	std::vector<std::vector<std::vector<std::complex<double>>>> sDataArray(sizeS);

	for(size_t s = 0; s < sizeS; s++)
		sDataArray[s] = std::vector<std::vector<std::complex<double>>>(sData.size());

	for (size_t i=0; i < sData.size(); i++){

		for(size_t s = 0; s < sizeS; s++)
			sDataArray[s][i] = std::vector<std::complex<double>>(sData[i].size());

		size_t counter = 0;

		for (size_t j=0; j<sData[i].size(); j++) {
			if ((j>0) && (j%(m/4)==0))
				counter++;
			sDataArray[counter][i][j%(m/4)] = sData[i][j];
		}
	}

	//std::cerr << " sData = " << sDataArray << std::endl;

	std::vector<std::vector<Ciphertext<DCRTPoly>>> S(sizeS);
	std::vector<Ciphertext<DCRTPoly>> Y(N);

	for (size_t i = 0; i < sizeS; i++)
		S[i] = std::vector<Ciphertext<DCRTPoly>>(N);

	//Encryption of single-integer ciphertexts
#pragma omp parallel for
	for (size_t i=0; i<N; i++){
		for (size_t s=0; s < sizeS; s++){
			Plaintext sTemp = cc->MakeCKKSPackedPlaintext(sDataArray[s][i]);
			S[s][i] = cc->Encrypt(keyPair.publicKey, sTemp);
		}
		Plaintext sTemp2 = cc->MakeCKKSPackedPlaintext(std::vector<std::complex<double>>(m/4,yData[i]));
		Y[i] = cc->Encrypt(keyPair.publicKey, sTemp2);
	}

	encryptionTime = TOC(t);

	TIC(t);

	Plaintext d = cc->MakeCKKSPackedPlaintext(std::vector<std::complex<double>>(m/4,2*N));
	d->SetFormat(EVALUATION);

	Plaintext dScaled = cc->MakeCKKSPackedPlaintext(std::vector<std::complex<double>>(m/4,2*N*scalingFactor));
	dScaled->SetFormat(EVALUATION);


// LB: Partial LINE 5 ************************
//  ??
    std::vector<Ciphertext<DCRTPoly>> ySum = Y;
	auto yU = BinaryTreeAdd(ySum);
// LB: END Partial LINE 5 ************************************************


	std::vector<Ciphertext<DCRTPoly>> chiD(sizeS);
	std::vector<Ciphertext<DCRTPoly>> chiN(sizeS);

	std::vector<Ciphertext<DCRTPoly>> orD(sizeS);
	std::vector<Ciphertext<DCRTPoly>> orN(sizeS);

	for (size_t s = 0; s < sizeS; s++) {


// LB: LINE 8 ************************
//  Compute multiplication portion of dot product yT*Sj
		std::vector<Ciphertext<DCRTPoly>> ySMult(N);
#pragma omp parallel for
		for(size_t i = 0; i < N; i++) {
			ySMult[i] = cc->EvalMultNoRelin(S[s][i],Y[i]);
		}
//  Compute addition portion of dot product yT*Sj
		auto n11 = BinaryTreeAdd(ySMult);
// LB: END LINE 8 ************************************************


		n11 = cc->ModReduce(n11);


// LB: LINE 9 ************************
//  ????
		auto c1 = BinaryTreeAdd(S[s]);
// LB: END LINE 9 ************************************************


// LB: Partial LINE 5 ************************
//  Compute 2*result of summation (which is yU)
		auto r1 = cc->EvalAdd(yU,yU);
// LB: END Partial Line 5 ************************************************


// LB: LINE 6 ************************
//  Compute r1Scaled
		auto r1Scaled = cc->EvalMult(r1,scalingFactor);
		r1Scaled = cc->ModReduce(r1Scaled);
// LB: END LINE 6 ************************************************


// LB: LINE 10 ************************
//  Compute c1Scaled
		auto c1Scaled = cc->EvalMult(c1,scalingFactor);
		c1Scaled = cc->ModReduce(c1Scaled);
// LB: END LINE 10 ************************************************


		r1 = cc->LevelReduce(r1,nullptr);
		c1 = cc->LevelReduce(c1,nullptr);


// LB: LINE 11 ************************
// chi2Numerator = (n11*dScaled - c1*r1Scaled)^2
        // numerator
		auto mult1 = cc->EvalMult(n11,dScaled);         //n11*dScaled
		auto mult2 = cc->EvalMultNoRelin(c1,r1Scaled);  //c1*r1Scaled

		auto chiN1 = cc->EvalSub(mult1,mult2);          //subtract
		chiN1 = cc->ModReduce(chiN1);

		chiN[s] = cc->EvalMultNoRelin(chiN1,chiN1);     //square
// LB: END LINE 11 ************************************************

   
// LB: LINE 12 ************************
//  chi2Denominator = c1*(dScaled*c1Scaled) * r1 * (dScaled - r1Scaled)
        // denominator
		auto chiD1 = cc->EvalMultNoRelin(c1,cc->EvalSub(dScaled,c1Scaled)); //c1*(dScaled*c1Scaled)
		chiD1 = cc->ModReduce(chiD1);

		auto chiD2 = cc->EvalMultNoRelin(r1,cc->EvalSub(dScaled,r1Scaled)); //r1 * (dScaled - r1Scaled)
		chiD2 = cc->ModReduce(chiD2);

		chiD[s] = cc->EvalMultNoRelin(chiD1,chiD2);                         //multiply
// LB: END LINE 12 ************************************************


// LB: Make n11Scalled ************************
		auto n11Scaled = cc->EvalMult(n11,scalingFactor);
		n11Scaled = cc->ModReduce(n11Scaled);


// LB: LINE 15 ************************
//  orDenominator = (c1 - n11) * (r1Scalled - n11Scalled)
		// denominator
		auto or2 = cc->EvalSub(c1,n11);                 //c1 - n11
		or2 = cc->LevelReduce(or2,nullptr);

		r1Scaled = cc->LevelReduce(r1Scaled,nullptr);
		auto or3 = cc->EvalSub(r1Scaled,n11Scaled);     //r1Scalled - n11Scalled

		orD[s] = cc->EvalMultNoRelin(or2,or3);          //multiply
// LB: END LINE 15 ************************************************


// LB: LINE 14 ************************
//  orNumerator = n11 * (n11Scaled - r1Scaled - c1Scaled + dScaled)
		// numerator
		auto or1 = cc->EvalSub(n11Scaled,r1Scaled);     //n11Scaled - r1Scaled
		c1Scaled = cc->LevelReduce(c1Scaled,nullptr);   
		or1 = cc->EvalSub(or1,c1Scaled);                //- c1Scaled
		or1 = cc->EvalAdd(or1,dScaled);                 //+ dScaled

		n11 = cc->LevelReduce(n11,nullptr);
		orN[s] = cc->EvalMultNoRelin(n11,or1);          //* n11
// LB: END LINE 14 ************************************************
	}


// **********************
// LB: ENDS COMPUTATION, SENDS CIPHERTEXTS BACK TO SERVER TO FINISH CALCULATION (e.g., Division)
	computationTime = TOC(t);

	TIC(t);

	std::vector<Plaintext> pN(sizeS);
	std::vector<Plaintext> pD(sizeS);
	std::vector<Plaintext> oddN(sizeS);
	std::vector<Plaintext> oddD(sizeS);

	for (size_t s = 0; s < sizeS; s++) {
		cc->Decrypt(keyPair.secretKey, chiN[s] , &(pN[s]));
		cc->Decrypt(keyPair.secretKey, chiD[s] , &(pD[s]));
		cc->Decrypt(keyPair.secretKey, orN[s] , &(oddN[s]));
		cc->Decrypt(keyPair.secretKey, orD[s] , &(oddD[s]));
	}

	decryptionTime = TOC(t);

	std::vector<double> chival(headersS.size());
	std::vector<double> pval(headersS.size());
	std::vector<double> odds(headersS.size());

	for (size_t s = 0; s < sizeS; s++) {
		for (size_t i = 0; i < m/4; i++) {
			if (s*m/4 + i < headersS.size()) {

				chival[s*m/4 + i] = pN[s]->GetCKKSPackedValue()[i].real()*2*N/pD[s]->GetCKKSPackedValue()[i].real();
				if (chival[s*m/4 + i] < 0)
					chival[s*m/4 + i] = 0;
				pval[s*m/4 + i] = (double)1-IncompleteGamma(chival[s*m/4 + i]/2,0.5);
				if (pval[s*m/4 + i] < 0)
					pval[s*m/4 + i] = 1e-15;
				else
					if (pval[s*m/4 + i]==0)
						pval[s*m/4 + i] = BS(sqrt(chival[s*m/4 + i]));

				odds[s*m/4 + i] = oddN[s]->GetCKKSPackedValue()[i].real()/oddD[s]->GetCKKSPackedValue()[i].real();

			}
		}
	}

    ofstream myfile;
    myfile.open(SNPDir + "/" + pValue);
    myfile.precision(10);
    for(uint32_t i = 0; i < headersS.size(); i++) {
    	myfile << headersS[i] << "\t" << pval[i] << std::endl;
    }
    myfile.close();

    ofstream myfile2;
    myfile2.open(SNPDir + "/" + "odds.txt");
    myfile2.precision(10);
    for(uint32_t i = 0; i < headersS.size(); i++) {
    	myfile2 << headersS[i] << "\t" << odds[i] << std::endl;
    }
    myfile2.close();

    ofstream myfile3;
    myfile3.open(SNPDir + "/" + "chi2.txt");
    myfile3.precision(10);
    for(uint32_t i = 0; i < headersS.size(); i++) {
    	myfile3 << headersS[i] << "\t" << chival[i] << std::endl;
    }
    myfile3.close();

	std::cout << "\nKey Generation Time: \t\t" << keyGenTime/1000 << " s" << std::endl;
	std::cout << "Encoding and Encryption Time: \t" << encryptionTime/1000 << " s" << std::endl;
	std::cout << "Computation Time: \t\t" << computationTime/1000 << " s" << std::endl;
	std::cout << "Decryption & Decoding Time: \t" << decryptionTime/1000 << " s" << std::endl;

	endToEndTime = TOC(tAll);

    std::cout << "\nEnd-to-end Runtime: \t\t" << endToEndTime/1000 << " s" << std::endl;

    ofstream myfileRuntime;
    myfileRuntime.open(SNPDir + "/" + Runtime);
    myfileRuntime << "Key Generation Time: \t\t" << keyGenTime/1000 << " s" << std::endl;
    myfileRuntime << "Encoding and Encryption Time: \t" << encryptionTime/1000 << " s" << std::endl;
    myfileRuntime << "Computation Time: \t\t" << computationTime/1000 << " s" << std::endl;
    myfileRuntime << "Decryption & Decoding Time: \t" << decryptionTime/1000 << " s" << std::endl;
    myfileRuntime << "End-to-end Runtime: \t\t" << endToEndTime/1000 << " s" << std::endl;
    myfileRuntime.close();

}

Ciphertext<DCRTPoly> BinaryTreeAdd(std::vector<Ciphertext<DCRTPoly>> &vector) {

	auto cc = vector[0]->GetCryptoContext();

	for(size_t j = 1; j < vector.size(); j=j*2) {
		for(size_t i = 0; i<vector.size(); i = i + 2*j) {
			if ((i+j)<vector.size())
				vector[i] = cc->EvalAdd(vector[i],vector[i+j]);
		}
	}

	Ciphertext<DCRTPoly> result(new CiphertextImpl<DCRTPoly>(*(vector[0])));

	return result;

}

double IncompleteGamma(double val, double p)
{
    if( !Greater(val, 0) || !Greater(p, 0) )
        return 0;

    double expValue = p*log(val) - val - lgamma(p);
    if( Less(expValue, log(1.0E-37)) ) // undeflow
        return 0;

    double factor = exp(expValue);
    if( !Greater(val, 1) || Less(val, p) )
    {
        double igamma = 1;
        double term = 1;

        for( int i = 1; Greater(term, EPSILON); ++i )
        {
            term *= (val/(p+i));
            igamma += term;
        }

        return (igamma*factor/p);
    }

    double pn[6] = { 1, val, val+1, val*(2+val-p) };
    double upperIncGamma = pn[2]/pn[3];

    for( int j = 1; ; ++j )
    {
        double a = (j+1)*2 + val- p;
        double b = (1 + j - p)*j;
        pn[4] = a*pn[2] - b*pn[0];
        pn[5] = a*pn[3] - b*pn[1];

        if( !Equal(pn[5], 0) )
        {
            double rn = pn[4]/pn[5];
            double diff = fabs(upperIncGamma - rn);
            if( !Greater(diff, EPSILON) && !Greater(diff, (EPSILON*rn)) )
                return (1 - factor*upperIncGamma);

            upperIncGamma = rn;
        }

        for( int i = 0; i < 4; i++ )
            pn[i] = pn[i+2];

        if( !Greater(1.0E+37, fabs(pn[4])) ) // overflow
        {
            for( int i = 0; i < 4; i++ )
                pn[i] = pn[i] / 1.0E+37;
        }
    }

    return 0;
}

void ReadSNPFile(vector<string>& headers, std::vector<std::vector<double>> & dataColumns, std::vector<double> &y,
                 string dataFileName, size_t N, size_t M)
{

	uint32_t cols = 0;

	string fileName = dataFileName + ".csv";

	std::cerr << "file name = " << fileName << std::endl;

	ifstream file(fileName);
	string line, value;

	if(file.good()) {

		getline(file, line);
		cols = std::count(line.begin(), line.end(), ',') + 1;
		stringstream ss(line);
		vector<string> result;

		size_t tempcounter = 0;

		for(uint32_t i = 0; i < cols; i++) {
			string substr;
			getline(ss, substr, ',');
			if ((substr != "") && (i>4) && (i<M+5)) {
				headers.push_back(substr);
				tempcounter++;
			}
		}

		cols = tempcounter;

	}

	size_t counter = 0;
	while((file.good()) && (counter < N)) {
		getline(file, line);
		uint32_t curCols = std::count(line.begin(), line.end(), ',') + 1;
		if (curCols > 2) {
			stringstream ss(line);
			for(uint32_t i = 0; i < 5; i++) {
				string substr;
				getline(ss, substr, ',');
				if ((i==1))
					y.push_back(std::stod(substr));
			}
			std::vector<double> row(cols);
			for(uint32_t i = 5; i < cols + 5; i++) {
				string substr;
				getline(ss, substr, ',');
				if (i < M+5)
				{
					double val;
					val = std::stod(substr);
					row[i-5] = val;
				}
			}
			dataColumns.push_back(row);
		}
		counter++;
	}

	file.close();

    std::cout << "Read in data: ";
	std::cout << dataFileName << std::endl;
}


