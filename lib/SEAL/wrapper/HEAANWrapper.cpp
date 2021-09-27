//#include "benchmark_framework/utilities.h"
#include "HEAANWrapper.h"
#include "HEAAN/HEAAN.h"

SecretKey* HEAANCipherText::secret_key=NULL;
Scheme* HEAANCipherText::scheme=NULL;
long HEAANCipherText::workingLogq=0;
long HEAANCipherText::logp=0;
long HEAANCipherText::logq=0;
long HEAANCipherText::logT=1;
long HEAANCipherText::nSlots=1;

HEAANCipherText::HEAANCipherText()
{

}

HEAANCipherText::HEAANCipherText(Scheme *nScheme)
{
        scheme=nScheme;
}
HEAANCipherText::HEAANCipherText(Scheme *nScheme, int nlogp)
{
        scheme=nScheme;
        logp=nlogp;
}
HEAANCipherText::HEAANCipherText(Scheme *nScheme, HEAANCipherText c1) 
{
    cipher.copy(c1.cipher);
    scheme=nScheme;
}
HEAANCipherText::HEAANCipherText(Scheme *nScheme, HEAANCipherText c1, long nlogp) 
{
    cipher.copy(c1.cipher);
    scheme=nScheme;
    logp=nlogp;
}
HEAANCipherText::HEAANCipherText( HEAANCipherText &c1)
{
    cipher.copy(c1.cipher);
    scheme=c1.scheme;
    logp=c1.logp;
} 
HEAANCipherText::HEAANCipherText(double d1)
{
    scheme->encryptSingle(cipher,d1,logp,workingLogq);
}
    
HEAANCipherText::HEAANCipherText(std::vector<double> d1)
{
         complex<double>* dvec = new complex<double>[d1.size()];
         for(int i=0;i<d1.size();i++)
            dvec[i]=d1[i];
        scheme->encrypt(cipher,dvec,nSlots,logp,workingLogq);

}
HEAANCipherText::HEAANCipherText(double d1, int size)
{
    scheme->encryptSingle(cipher,d1,logp,workingLogq);
}
    
HEAANCipherText::HEAANCipherText(std::vector<double> d1, int size)
{
    std::cout<<"Input vecSize:"<<nSlots<<"\nvec[0]:"<<d1[0]<<"\nvec[n-1]:"<<d1[nSlots-1]<<std::endl;
         complex<double>* dvec = new complex<double>[nSlots];
         std::cout<<"Input vecSize:"<<nSlots<<"\nvec[0]:"<<d1[0]<<"\nvec[n-1]:"<<d1[nSlots-1];
         for(int i=0;i<nSlots;i++)
        { 
            dvec[i]=complex<double>(d1[i],0.0);
            std::cout<<"dvec["<<i<<"] = "<<dvec[i]<<"\td1["<<i<<"] = "<<d1[i]<<std::endl;
        }

        std::cout<<"\nComplex vecSize:"<<nSlots<<"\nvec[0]:"<<dvec[0]<<"\nvec[n-1]:"<<dvec[nSlots-1]<<std::endl;
        std::cout<<"Encryption Parameters:\nlogp:"<<logp<<"\nlogq:"<<workingLogq<<std::endl;
        std::cout<<"Ciphertext Parameters:\nlogp:"<<cipher.logp<<"\nlogq:"<<cipher.logq<<std::endl;
        scheme->encrypt(cipher,dvec,nSlots,logp,workingLogq);

}
HEAANCipherText::~HEAANCipherText()
{
    
}
HEAANCipherText::operator HEAANCipherText() { return (*this);}

HEAANCipherText::operator std::vector<double> ()
{
    std::vector<double> res(nSlots,0.0);
    complex<double>* dvec = scheme->decrypt(*secret_key, cipher);
    for(int i=0;i<nSlots;i++)
        res[i]=dvec[i].real();
    return res;
}
HEAANCipherText:: operator double ()
{
   complex<double> res= scheme->decryptSingle(*secret_key, cipher);
   return res.real();

}
HEAANCipherText& HEAANCipherText::operator = (HEAANCipherText  &c1)
{
    
    cipher.copy(c1.cipher);
    scheme=c1.scheme;
    return *this;
}
/*  HEAANCipherText& HEAANCipherText::operator = (HEAANCipherText  c1)
{
    
    cipher.copy(c1.cipher);
    scheme=c1.scheme;
    logp=c1.logp;
    return *this;
}
    */
HEAANCipherText& HEAANCipherText::operator += (HEAANCipherText  &c1)
{
    if(cipher.logq>c1.cipher.logq)
        scheme->modDownByAndEqual(cipher,c1.cipher.logp);
    else if(cipher.logq<c1.cipher.logq)
        scheme->modDownByAndEqual(c1.cipher,cipher.logp);
    
    c1.scheme->addAndEqual(cipher,c1.cipher);
    scheme=c1.scheme;
    return *this;
}
HEAANCipherText& HEAANCipherText::operator += (const double  &c1)
{
    
    scheme->addConstAndEqual(cipher,c1,logp);
    
    return *this;
}
HEAANCipherText& HEAANCipherText::operator -= (HEAANCipherText  &c1)
{
    if(cipher.logq>c1.cipher.logq)
        scheme->modDownByAndEqual(cipher,c1.cipher.logp);
    else if(cipher.logq<c1.cipher.logq)
        scheme->modDownByAndEqual(c1.cipher,cipher.logp);
    
    c1.scheme->subAndEqual(cipher,c1.cipher);
    scheme=c1.scheme;
    return *this;
}
HEAANCipherText& HEAANCipherText::operator -= (const double  &c1)
{
    
    scheme->addConstAndEqual(cipher,-c1,logp);
    
    return *this;
}
HEAANCipherText& HEAANCipherText::operator *= (HEAANCipherText  &c1)
{
    if(cipher.logq>c1.cipher.logq)
        scheme->modDownByAndEqual(cipher,c1.cipher.logp);
    else if(cipher.logq<c1.cipher.logq)
        scheme->modDownByAndEqual(c1.cipher,cipher.logp);
    
    scheme->multAndEqual(cipher,c1.cipher);
    scheme->reScaleByAndEqual(cipher,logp);
    
    return *this;
}
HEAANCipherText& HEAANCipherText::operator *= (const double  &c1)
{
    //printf("c1: %f\n",c1);
    scheme->multByConstAndEqual(cipher,c1,logp);
    scheme->reScaleByAndEqual(cipher,logp);
    
    return *this;
}
HEAANCipherText& HEAANCipherText::operator - ()    //negation
{
    HEAANCipherText res=*this;
    //scheme->negateAndEqual(this->cipher);

    scheme->negateAndEqual(res.cipher);
    return res;
}
HEAANCipherText& HEAANCipherText::operator - (HEAANCipherText  &c1)
{
    //cout<<"N:"<<cipher.logq<<" "<<c1.cipher.logq<<endl;
    long bitsDown=abs(cipher.logq-c1.cipher.logq);
    if(cipher.logq>c1.cipher.logq)
        scheme->modDownByAndEqual(cipher,bitsDown);
    else if(cipher.logq<c1.cipher.logq)
        scheme->modDownByAndEqual(c1.cipher,bitsDown);
    HEAANCipherText *res= new HEAANCipherText(scheme,logp);
    scheme->sub(res->cipher,this->cipher,c1.cipher);
    return *res;
}
HEAANCipherText& HEAANCipherText::operator - (double const &p1)
{
    HEAANCipherText *res=new HEAANCipherText(this->scheme,this->logp);
    scheme->addConst(res->cipher,this->cipher,-p1,logp);
    
    return *res;
}
HEAANCipherText& operator -(const double &p1,  HEAANCipherText &c2)
{
    //std::cout<<"*********operator - ************"<<std::endl;
    //std::cout<<"C2: "<<c2<<" "<<"p1: "<<p1<<std::endl;
    HEAANCipherText *res=new HEAANCipherText(c2.scheme,c2.logp); 
    c2.scheme->negate(res->cipher,c2.cipher);
    
    //std::cout<<"C2: "<<c2<<" "<<"res->cipher: "<<*res<<std::endl;
    c2.scheme->addConstAndEqual(res->cipher,p1,c2.cipher.logp);
    //std::cout<<"C2: "<<c2<<" "<<"p1: "<<p1<<" "<<"res->cipher: "<<*res<<std::endl;
    //std::cout<<"*********operator - ends *******"<<std::endl;
    return *res; 
}
HEAANCipherText& HEAANCipherText::operator + (HEAANCipherText  &c1)
{
    //std::cout<<"*********operator + ************"<<std::endl;
    //std::cout<<*this<<"("<<this->cipher.logq<<")"<<" + "<<c1<<"("<<c1.cipher.logq<<")"<<std::endl;
    HEAANCipherText *res=new HEAANCipherText(this->scheme,this->logp);
    int logq1=cipher.logq;
    int logq2=c1.cipher.logq;
    int bitsDown=abs(logq1-logq2);
    bool isC1=false;
    if(logq1>logq2){
        res->cipher.copy(cipher);
        scheme->modDownByAndEqual(res->cipher,bitsDown);
        //std::cout<<"1. " <<cipher.logq<<" "<<res->cipher.logq<<std::endl;
    }
    else if(logq1<logq2){
        isC1=true;
        res->cipher.copy(c1.cipher);
        //std::cout<<"2. "<<cipher.logq<<" "<<c1.cipher.logq<<" logq_diff:"<<logp_diff<<" -> "<<res->cipher.logq<<std::endl;
        scheme->modDownByAndEqual(res->cipher,bitsDown);
        //std::cout<<"2. "<<cipher.logq<<" "<<c1.cipher.logq<<" logq_diff:"<<logp_diff<<" -> "<<res->cipher.logq<<std::endl;
    }
    else
    {
        res->cipher.copy(c1.cipher);
        isC1=true;
    }
    
    HEAANCipherText *temp = (isC1)? this : &c1;
    long tmpLogq=((isC1)?cipher.logq:c1.cipher.logq);
    //std::cout<<*res<<"("<<res->cipher.logq<<")"<<" + "<<*temp<<"("<<temp->cipher.logq<<")"<<std::endl;
    //temp = (isC1) ? &c1 : this;
    scheme->addAndEqual(res->cipher,temp->cipher);
    //scheme->reScaleByAndEqual(res->cipher,logp);
    //scheme->addAndEqual(res->cipher,this->cipher);
    //std::cout<<cipher.logq<<" "<<c1.cipher.logq<<" "<<res->cipher.logq<<" "<<*res<<" isC1: "<<((isC1)?"true":"false")<<std::endl;
    //std::cout<<"*********operator + ends *******"<<std::endl;
    return *res;
}
HEAANCipherText& HEAANCipherText::operator + (double const &p1)
{
    HEAANCipherText *res=new HEAANCipherText(scheme,logp);
    this->scheme->addConst(res->cipher,this->cipher,p1,logp);
    
    return *res;
}
HEAANCipherText& operator + (const double &p1,  HEAANCipherText &c2)
{
    HEAANCipherText *res=new HEAANCipherText(c2.scheme,c2.logp);
    c2.scheme->addConst(res->cipher,c2.cipher,p1,c2.logp);
    return *res; 
}
HEAANCipherText& HEAANCipherText::operator * (HEAANCipherText  &c1)
{
    //std::cout<<"********* operator * ************"<<std::endl;
    //std::cout<<"this: "<<*this<<" c1: "<<c1<<std::endl;
    //std::cout<<cipher.logq<<" "<<c1.cipher.logq<<std::endl;
    /* if(cipher.logq>c1.cipher.logq){
        scheme->modDownByAndEqual(cipher,c1.cipher.logp);
         std::cout<<"1. " <<cipher.logq<<" "<<c1.cipher.logq<<std::endl;
    }
    else if(cipher.logq<c1.cipher.logq){
        scheme->modDownByAndEqual(c1.cipher,cipher.logp);
        std::cout<<"2. " <<cipher.logq<<" "<<c1.cipher.logq<<std::endl;
    }
 */
    HEAANCipherText *res=new HEAANCipherText(this->scheme,this->logp);
    int bitsDown=cipher.logq-c1.cipher.logq;
    bool isC1=false;
    if(bitsDown>0){
        res->cipher.copy(cipher);
        scheme->modDownByAndEqual(res->cipher,bitsDown);
        //scheme->reScaleByAndEqual(cipher,logp);
        //std::cout<<"1. " <<cipher.logq<<" "<<res->cipher.logq<<std::endl;
    }
    else if(bitsDown<0){
        isC1=true;
        res->cipher.copy(c1.cipher);
        scheme->modDownByAndEqual(res->cipher,-bitsDown);
        //scheme->reScaleByAndEqual(c1.cipher,logp);
      //  std::cout<<"2. " <<res->cipher.logq<<" "<<c1.cipher.logq<<std::endl;
    }
    else
    {
        res->cipher.copy(c1.cipher);
        isC1=true;
    }
    HEAANCipherText *tempPtr= isC1? this: &c1;

    
    //std::cout<<*res<<" * "<<*tempPtr<<" "<<std::endl;
    if(this == &c1)
        scheme->squareAndEqual(res->cipher);
    else
        scheme->multAndEqual(res->cipher,tempPtr->cipher);
       //(isC1)? scheme->multAndEqual(res->cipher,this->cipher):scheme->multAndEqual(res->cipher,c1.cipher);
    //std::cout<<"Rescaling Res: "<<c1.cipher.logq<<" "<<res->cipher.logq<<" "<<*res<<std::endl;
    scheme->reScaleByAndEqual(res->cipher,logp);
    //std::cout<<cipher.logq<<" "<<c1.cipher.logq<<" "<<res->cipher.logq<<" "<<*res<<std::endl;
    //std::cout<<"********* operator * ends *******"<<std::endl;
    return *res;//HEAANCipherText(this->scheme,res);
}
HEAANCipherText& HEAANCipherText::operator * (double const  &c1)
{
    HEAANCipherText *res=new HEAANCipherText(this->scheme,this->logp);
    scheme->multByConst(res->cipher,this->cipher,c1,logp);
    scheme->reScaleByAndEqual(res->cipher,logp);
    return *res;
}
 HEAANCipherText& HEAANCipherText::operator * (std::vector<double> const  &c1)
{
    std::complex<double>* vec=new std::complex<double>[c1.size()];
    for(int i=0;i<c1.size();i++)
        vec[i]=c1[i];
    HEAANCipherText *res=new HEAANCipherText(this->scheme,this->logp);
    scheme->multByConstVec(res->cipher,this->cipher,vec,logp);
    scheme->reScaleByAndEqual(res->cipher,logp);
    return *res;
} 
HEAANCipherText& operator * (double const  &p1,  HEAANCipherText &c1)
{
    HEAANCipherText *res=new HEAANCipherText(c1.scheme,c1.logp);//printf("Line 95\n");
    c1.scheme->multByConst(res->cipher,c1.cipher,p1,c1.logp);//printf("Line 96\n");
    c1.scheme->reScaleByAndEqual(res->cipher,c1.logp);//printf("Line 97\n");
    return *res;
}

HEAANCipherText& operator ^(HEAANCipherText c1,int i1)
{}
        
HEAANCipherText& operator ^(HEAANCipherText c1,double i1)
{}
void HEAANCipherText::operator <<=(unsigned long i1)
{
    scheme->leftRotateFastAndEqual(this->cipher,i1);
}

void HEAANCipherText::operator >>=(unsigned long i1)
{
    scheme->rightRotateFastAndEqual(this->cipher,i1);
}

HEAANCipherText& HEAANCipherText::operator <<(unsigned long i1)
{
    HEAANCipherText *res=new HEAANCipherText(scheme,logp);//printf("Line 95\n");
    scheme->leftRotateFast(res->cipher,this->cipher,i1);//printf("Line 96\n");
    return *res;
}

HEAANCipherText& HEAANCipherText::operator >>(unsigned long i1)
{
    HEAANCipherText *res=new HEAANCipherText(scheme,logp);//printf("Line 95\n");
    scheme->rightRotateFast(res->cipher,this->cipher,i1);//printf("Line 96\n");
    return *res;
}
std::ostream& operator <<(std::ostream &out, HEAANCipherText &c1)
{
     complex<double>* dvec3 = c1.scheme->decrypt(*c1.secret_key, c1.cipher);
     out<<dvec3[0].real();

    return out;
}