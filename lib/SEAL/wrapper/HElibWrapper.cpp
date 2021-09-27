#include "HElibWrapper.h"
//#include "benchmark_framework/utilities.h"
#include "HElib/FHE.h"
#include "HElib/EncryptedArray.h"
#include <NTL/lzz_pEXFactoring.h>

       using namespace HEbenchmark;
        
        FHEPubKey* HElibCipherText::publicKey=NULL;
        
        FHESecKey* HElibCipherText::secretKey=NULL;
        
        EncryptedArray* HElibCipherText::ea=NULL;

        EncryptedArrayCx* HElibCipherText::eacx=NULL;

         bool HElibCipherText::isCKKS=false;

        HElibCipherText::HElibCipherText()//:DataType()
        {
                ///batchingEnabled=false;
                if(publicKey!=NULL)
                {
                    cipher=new Ctxt(*publicKey);
                }
                else
                {
                    std::cout<<"PublicKey is not generated or assigned."<<std::endl;
                    exit(1);
                }
        }
        HElibCipherText::HElibCipherText(long const i1)//:DataType(i1)
        {
            
            std::vector<long>  repI1;
         
            if(publicKey!=NULL && ea != NULL)
                {
                    cipher=new Ctxt(*publicKey);
                    repI1.resize(ea->size(),i1);
                }
                else
                {
                    std::cout<<"PublicKey is not generated or assigned."<<std::endl;
                    exit(1);
                }
            
            ea->encrypt(*cipher,*publicKey,repI1);
        }
        HElibCipherText::HElibCipherText(double d1)//:DataType(d1)
        {
            std::vector<cx_double> repD1;
            
            if(publicKey!=NULL&& eacx != NULL)
                {
                    cipher=new Ctxt(*publicKey);
                    repD1.resize(eacx->size(),d1);
                }
                else
                {
                    std::cout<<"PublicKey is not generated or assigned."<<std::endl;
                    exit(1);
                }
            
            eacx->encrypt(*cipher,*publicKey,repD1);
        }
        HElibCipherText::HElibCipherText(std::vector<long>& i1)//:DataType(i1)
        {
             
            if(publicKey!=NULL && ea != NULL)
            {
                cipher=new Ctxt(*publicKey);
            }
            else
            {
                std::cout<<"PublicKey is not generated or assigned."<<std::endl;
                exit(1);
            }
            ea->encrypt(*cipher,*publicKey,i1);
        }
        
        HElibCipherText::HElibCipherText(std::vector<double>& d1)//:DataType(d1)
        {
            std::vector<cx_double> repD1;
            
            if(publicKey!=NULL && eacx!=NULL)
            {
                cipher=new Ctxt(*publicKey);
                repD1.resize(eacx->size());
                for(int i=0;i<repD1.size();i++)
                {
                    repD1.push_back(d1[i]);
                }
            }
            else
            {
                std::cout<<"PublicKey is not generated or assigned."<<std::endl;
                exit(1);
            }
            eacx->encrypt(*cipher,*publicKey,d1);
                     
        }
        

        HElibCipherText::HElibCipherText(std::vector<cx_double>& d1)
        {
            if(publicKey!=NULL && eacx!=NULL)
            {
                cipher=new Ctxt(*publicKey);
            }
            else
            {
                std::cout<<"PublicKey is not generated or assigned."<<std::endl;
                exit(1);
            }
            eacx->encrypt(*cipher,*publicKey,d1);
        }
        //HElibCipherText::HElibCipherText(Plaintext plain)
        //{
            
        //}
         HElibCipherText::HElibCipherText(const HElibCipherText &c1)//:DataType(c1)
        {
            if(publicKey!=NULL)
                {
                    cipher=new Ctxt(*publicKey);
                }
                else
                {
                    std::cout<<"PublicKey is not generated or assigned."<<std::endl;
                    exit(1);
                }
            *cipher=*c1.cipher;
        } 
        HElibCipherText::~HElibCipherText()
        {
                //cipher->release();
        }
        //bool HElibCipherText::isBatchingEnabled() {return batchingEnabled;}
        HElibCipherText::operator HElibCipherText() {return (*this);}

         HElibCipherText& HElibCipherText::operator = (const HElibCipherText &c1)
        {
            
            *cipher=*(c1.cipher);
            return *this;
        } 
        /* HElibCipherText HElibCipherText::operator = (HElibCipherText &c1)
        {
            *cipher=*c1.cipher;
            return *this;
        } */
       HElibCipherText& HElibCipherText::operator += (HElibCipherText &c1)
        {
            *cipher+=*c1.cipher;
            return *this;
        }
        HElibCipherText& HElibCipherText::operator += (long i1)
        {
            std::vector<long>  repI1;
            repI1.resize(ea->size(),i1);
            NTL::ZZX p;
            ea->encode(p,repI1);
            cipher->addConstant(p);
            return *this;
        }
       /* HElibCipherText& HElibCipherText::operator += (Plaintext &p1)
        {
            if(scheme==scheme_type::CKKS)
            {
                p1.scale()=cipher.scale();
                evaluator->mod_switch_to_inplace(p1,cipher.parms_id());
            }
            evaluator->add_plain_inplace(cipher,p1);
            return *this;
        }
         */
         HElibCipherText& HElibCipherText::operator += (double d1)
        {
            //std::vector<cx_double>  repD1;
            //repI1.resize(ea->size(),d1);
            
            //NTL::ZZX p;
            //ea->encode(p,repI1);
            cipher->addConstantCKKS(d1);
            return *this;
            
        }
        HElibCipherText& HElibCipherText::operator -= (HElibCipherText &c1)
        {
            *cipher-=*c1.cipher;
            return *this;
        }
     /*   HElibCipherText& HElibCipherText::operator -= (Plaintext &p1)
        {
            if(scheme==scheme_type::CKKS)
            {
                p1.scale()=cipher.scale();
                evaluator->mod_switch_to_inplace(p1,cipher.parms_id());
            }
            evaluator->sub_plain_inplace(cipher,p1);
            return *this;
        }*/

        HElibCipherText& HElibCipherText::operator -= (long i1)
        {
            std::vector<long>  repI1;
            repI1.resize(ea->size(),-i1);
            NTL::ZZX p;
            ea->encode(p,repI1);
            cipher->addConstant(p);
            return *this;
        }

        HElibCipherText& HElibCipherText::operator -= (double d1)
        {
            cipher->addConstantCKKS(-d1);
            return *this;
        }


       HElibCipherText& HElibCipherText::operator *= (HElibCipherText &c1)
        {
            *cipher*=*c1.cipher;
            return *this;
        }
        
        HElibCipherText& HElibCipherText::operator *= (long i1)
        {
            
            std::vector<long>  repI1;
            repI1.resize(ea->size(),-i1);
            NTL::ZZX p;
            ea->encode(p,repI1);
            cipher->multByConstant(p);
            return *this;
        }
        HElibCipherText& HElibCipherText::operator *= (double d1)
        {
            cipher->multByConstantCKKS(d1);
            return *this;
        }
        HElibCipherText& HElibCipherText::operator -()
        {
            cipher->negate();
            return *this;
        }
         HElibCipherText& HElibCipherText::operator -(HElibCipherText &c1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            *res-=c1;
            return *res;
            
        }
         
        HElibCipherText& HElibCipherText::operator -(long i1)
        {
            
            HElibCipherText *res=new HElibCipherText(*this);
            *res-=i1;
            return *res;
        }
        HElibCipherText& HElibCipherText::operator -(double d1)
        {
            
            HElibCipherText *res=new HElibCipherText(*this);
            *res-=d1;
            return *res;
        }
       
        HElibCipherText& operator -(double d1, HElibCipherText &c1)
        {
            HElibCipherText *res=new HElibCipherText(d1);
            *res-=c1;
            return *res;
        }
        HElibCipherText& HElibCipherText::operator +(HElibCipherText &c1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            *res+=c1;
            return *res;
        }
         
        HElibCipherText& HElibCipherText::operator +(long i1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            *res+=i1;
            return *res;
        }

        HElibCipherText& HElibCipherText::operator +(double d1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            *res+=d1;
            return *res;
        }
        
        HElibCipherText& operator +(double d1, HElibCipherText &c1)
        {
            HElibCipherText *res=new HElibCipherText(d1);
            *res+=c1;
            return *res;
        }
        HElibCipherText& HElibCipherText::operator *(HElibCipherText &c1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            *res*=c1;
            return *res;
        }
        HElibCipherText& HElibCipherText::operator *(long i1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            *res*=i1;
            return *res;
        }
         HElibCipherText& HElibCipherText::operator *(double d1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            *res+=d1;
            return *res;
        }

        HElibCipherText& HElibCipherText::operator *(std::vector<long> i1)
        {
            HElibCipherText *res= new HElibCipherText(i1);
            *res*=*this;
            return *res;
        }

        HElibCipherText& HElibCipherText::operator *(std::vector<double> d1)
        {
           HElibCipherText *res= new HElibCipherText(d1);
            *res*=*this;
            return *res;
        }

        HElibCipherText& operator *(long i1, HElibCipherText &c1)
        {
            HElibCipherText *res= new HElibCipherText(i1);
            *res*=c1;
            return *res;
        }
         HElibCipherText& operator *(double d1, HElibCipherText &c1)
        {
            
            HElibCipherText *res= new HElibCipherText(d1);
            *res*=c1;
            return *res;
        }

        void HElibCipherText::operator <<=(long i1)
        {
            if(isCKKS)
                eacx->rotate(*this->cipher,-i1);
            else
                ea->rotate(*this->cipher,-i1);
            
        }

        void HElibCipherText::operator >>=(long i1)
        {
            if(isCKKS)
                eacx->rotate(*this->cipher,i1);
            else
                ea->rotate(*this->cipher,i1);
        }
        
        HElibCipherText& HElibCipherText::operator <<(long i1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            if(isCKKS)
                eacx->rotate(*res->cipher,-i1);
            else
                ea->rotate(*res->cipher,-i1);
           return *res;
        }

        HElibCipherText& HElibCipherText::operator >>(long i1)
        {
            
            HElibCipherText *res=new HElibCipherText(*this);
            if(isCKKS)
                eacx->rotate(*res->cipher,i1);
            else
                ea->rotate(*res->cipher,i1);
            return *res;
        }
        HElibCipherText& HElibCipherText::operator <(long i1)
        {
            HElibCipherText *res=new HElibCipherText(*this);
            if(isCKKS)
                eacx->shift(*res->cipher,-i1);
            else
                ea->shift(*res->cipher,-i1);
           return *res;
        }

        HElibCipherText& HElibCipherText::operator >(long i1)
        {
            
            HElibCipherText *res=new HElibCipherText(*this);
            if(isCKKS)
                eacx->shift(*res->cipher,i1);
            else
                ea->shift(*res->cipher,i1);
            return *res;
        }
        /*
        void operator <<(Plaintext p1,int i1)
        {
             if(HElibCipherText::galois_keys!=NULL)
            {
                 
                 if(HElibCipherText::scheme==scheme_type::CKKS)
                {
                   std::cout<<"Only supported in BFV scheme."<<std::endl;
                }
                else if (HElibCipherText::scheme==scheme_type::BFV)
                {
                    //HElibCipherText* res=new HElibCipherText();
                    //HElibCipherText::evaluator->rotate_vector_inplace(p1,-i1,*HElibCipherText::galois_keys);
                 
                } 
                
            }
            else
            {
                std::cout<<"Galois_keys need to be created before using this operator."<<std::endl;
                exit(1);
            }
        }
        HElibCipherText& operator ^(HElibCipherText c1,int i1)
        {
                 HElibCipherText* res=new HElibCipherText();
                 c1.evaluator->exponentiate(c1.cipher,i1,*HElibCipherText::relin_keys,res->cipher);
                 return *res;
        }
        HElibCipherText& operator ^(HElibCipherText c1,double i1)
        {
                 HElibCipherText* res=new HElibCipherText();
                 c1.evaluator->exponentiate(c1.cipher,(int)i1,*HElibCipherText::relin_keys,res->cipher);
                 return *res;
        }
        void operator >>(Plaintext p1,int i1);
       */
        std::ostream& operator <<(std::ostream &out, HElibCipherText &c1)
        {
            

               if(c1.isCKKS)
                {
                    std::vector<cx_double> result;
                    {
                        Stopwatch s("Decrypt");
            
                        HElibCipherText::eacx->decrypt(*c1.cipher,*c1.secretKey,result);
                        out<<result[0];
                    }
                }
                else
                {
                    std::vector<long> result;
                    {
                        Stopwatch s("Decrypt");
            
                        HElibCipherText::ea->decrypt(*c1.cipher,*c1.secretKey,result);
                    }
                    out<<result[0];
                }
            
            //encoder.decode_int32(result);
            out.flush();
           return out;
        }
