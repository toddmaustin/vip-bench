#pragma once
#include "HEAAN/HEAAN.h"
#include <complex>

//this is HEAANs cipher text wraper
class HEAANCipherText{
    public:
    HEAANCipherText();
    HEAANCipherText(Scheme *nScheme);
    HEAANCipherText(Scheme *nScheme, int nlogp);
    HEAANCipherText(Scheme *nScheme, HEAANCipherText c1) ;
    HEAANCipherText(Scheme *nScheme, HEAANCipherText c1, long nlogp);
    HEAANCipherText( HEAANCipherText &c1);
    HEAANCipherText(double d1);
    HEAANCipherText(std::vector<double> d1);
    HEAANCipherText(double d1, int size);
    HEAANCipherText(std::vector<double> d1, int size);
    ~HEAANCipherText();

    operator HEAANCipherText(); 
    operator std::vector<double> ();
    operator double ();

    HEAANCipherText& operator = (HEAANCipherText  &c1);
    
    HEAANCipherText& operator += (HEAANCipherText  &c1);
    
    HEAANCipherText& operator += (const double  &c1);
    
    HEAANCipherText& operator -= (HEAANCipherText  &c1);
    
    HEAANCipherText& operator -= (const double  &c1);
    
    HEAANCipherText& operator *= (HEAANCipherText  &c1);
    
    HEAANCipherText& operator *= (const double  &c1);
    
    HEAANCipherText& operator - ();    //negation
    
    HEAANCipherText& operator - (HEAANCipherText  &c1);
    
    HEAANCipherText& operator - (double const &p1);
    
    friend HEAANCipherText& operator -(const double &p1,  HEAANCipherText &c2);
    
    HEAANCipherText& operator + (HEAANCipherText  &c1);
   
    HEAANCipherText& operator + (double const &p1);
    
    friend  HEAANCipherText& operator + (const double &p1,  HEAANCipherText &c2);
    
    HEAANCipherText& operator * (HEAANCipherText  &c1);
    
    HEAANCipherText& operator * (double const  &c1);

    HEAANCipherText& operator * (std::vector<double> const  &d1);
    
    friend HEAANCipherText& operator * (double const  &p1,  HEAANCipherText &c1);

    friend HEAANCipherText& operator ^(HEAANCipherText c1,int i1);
        
    friend HEAANCipherText& operator ^(HEAANCipherText c1,double i1);


    void operator <<=(unsigned long i1);

    void operator >>=(unsigned long i1);

    HEAANCipherText& operator <<(unsigned long i1);

    HEAANCipherText& operator >>(unsigned long i1);

    friend std::ostream& operator <<(std::ostream &out, HEAANCipherText &c1);    
    void decrypt()
    {
        
    }
    void getHEAANCipherText(HEAANCipherText &res)
    {
        res.cipher.copy(this->cipher);
    }
    void setHEAANCipherText(HEAANCipherText res)
    {
        this->cipher.copy(res.cipher);
    }
    static SecretKey* secret_key;
    static Scheme *scheme;
    static long workingLogq,logp, logq, logT,nSlots;
    Ciphertext cipher;
    private:
        //Ciphertext cipher;
        //long logp, logq;
        //Scheme *scheme;

};

