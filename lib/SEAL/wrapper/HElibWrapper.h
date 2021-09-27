#pragma once
#include "HElib/FHE.h"
#include "HElib/EncryptedArray.h"
#include <NTL/lzz_pEXFactoring.h>
//#include "wrappers.h"

class HElibCipherText/*:public DataType*/{

    private:

        Ctxt* cipher=NULL;

    public:

        static FHEPubKey* publicKey;
        
        static FHESecKey* secretKey;
        
        static EncryptedArray* ea;
        
        static EncryptedArrayCx* eacx;

        static bool isCKKS;
        
        HElibCipherText();
        
        HElibCipherText(long const i1);
        
        HElibCipherText(std::vector<int64_t>& i1);

        HElibCipherText(std::vector<uint64_t>& i1);

        HElibCipherText(std::vector<double>& d1);

        HElibCipherText(std::vector<cx_double>& d1);
        
        HElibCipherText(double d1);
                
        //HElibCipherText(Plaintext plain);
        
        HElibCipherText(const HElibCipherText &c1);
        
        ~HElibCipherText();

        bool isBatchingEnabled();

        operator HElibCipherText();

        HElibCipherText& operator = (const HElibCipherText &c1);

       // HElibCipherText operator = (HElibCipherText &c1);
       
        HElibCipherText& operator += (HElibCipherText &c1);
        
        //HElibCipherText& operator += (Plaintext &p1);

        HElibCipherText& operator += (long p1);

        HElibCipherText& operator += (double p1);
        
        HElibCipherText& operator -= (HElibCipherText &c1);
        
       // HElibCipherText& operator -= (Plaintext &p1);

        HElibCipherText& operator -= (long p1);

        HElibCipherText& operator -= (double p1);
       
        HElibCipherText& operator *= (HElibCipherText &c1);
        
        //HElibCipherText& operator *= (Plaintext &p1);

        HElibCipherText& operator *= (long p1);

        HElibCipherText& operator *= (double p1);
        
        HElibCipherText& operator -();
        
        HElibCipherText& operator -(HElibCipherText &c1);
        
       // HElibCipherText& operator -(Plaintext &p1);
       HElibCipherText& operator -(long d1);
        
        HElibCipherText& operator -(double d1);

        HElibCipherText& operator -(std::vector<double> d1);
        
        //friend HElibCipherText& operator -(Plaintext &p1, HElibCipherText &c1);
        
        friend HElibCipherText& operator -(double d1, HElibCipherText &c1);
        
        HElibCipherText& operator +(HElibCipherText &c1);
                
        //HElibCipherText& operator +(Plaintext &p1);

        HElibCipherText& operator +(long i1);
        
        HElibCipherText& operator +(double d1);
        
        //friend HElibCipherText& operator +(Plaintext &p1, HElibCipherText &c1);
        
        friend HElibCipherText& operator +(double d1, HElibCipherText &c1);
        
        HElibCipherText& operator *(HElibCipherText &c1);
        
        //HElibCipherText& operator *(Plaintext &p1);

        HElibCipherText& operator *(long i1);

        HElibCipherText& operator *(double d1);

        HElibCipherText& operator *(std::vector<long> d1);
        
        HElibCipherText& operator *(std::vector<double> d1);

        //rotate operators
        void operator <<=(long i1);

        void operator >>=(long i1);

        //rotate operators
        HElibCipherText& operator <<(long i1);

        HElibCipherText& operator >>(long i1);

        //shift operators
        HElibCipherText& operator <(long i1);

        HElibCipherText& operator >(long i1);

        friend HElibCipherText& operator ^(HElibCipherText c1,int i1);
        
        friend HElibCipherText& operator ^(HElibCipherText c1,double i1);
        
        //friend void operator <<(Plaintext p1,int i1);

        //friend void operator >>(Plaintext p1,int i1);


        
        //friend HElibCipherText& operator *(Plaintext &p1, HElibCipherText &c1);
        
        friend HElibCipherText& operator *(int i1, HElibCipherText &c1);
        
        friend HElibCipherText& operator *(double d1, HElibCipherText &c1);
        

        friend std::ostream& operator <<(std::ostream &out, HElibCipherText &c1);

};