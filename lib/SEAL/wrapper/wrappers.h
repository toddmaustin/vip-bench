#pragma once
class DataType{

    protected:
        //a pointer to the data holder should be included in the derived class
        //Ctxt* cipher=NULL;
        
    public:

        // Common data the is used to execute computation on the data should be declared here.
        // An example for a HElib FHE lib is given below
        /*  
            static FHEPubKey* publicKey;
            
            static FHESecKey* secretKey;
            
            static EncryptedArray* ea;
            
            static EncryptedArrayCx* eacx;

            static bool isCKKS;
        */
        
        DataType(){}
        
        /*DataType(long const i1){}
        
        DataType(std::vector<int64_t>& i1){}

        DataType(std::vector<uint64_t>& i1){}

        DataType(double d1){}

        DataType(std::vector<double>& d1){}
        
        DataType(DataType &c1){}*/
        
        virtual ~DataType() {}

        operator DataType();

        DataType& operator = (DataType &c1);
       
        DataType& operator += (DataType &c1);

        DataType& operator += (long p1);

        DataType& operator += (double p1);
        
        DataType& operator -= (DataType &c1);

        DataType& operator -= (long p1);

        DataType& operator -= (double p1);
       
        DataType& operator *= (DataType &c1);

        DataType& operator *= (long p1);

        DataType& operator *= (double p1);
        
        DataType& operator -();
        
        DataType& operator -(DataType &c1);
        
        DataType& operator -(long d1);
        
        DataType& operator -(double d1);
        
        friend DataType& operator -(double d1, DataType &c1);
        
        DataType& operator +(DataType &c1);
                
        DataType& operator +(long i1);
        
        DataType& operator +(double d1);
        
        friend DataType& operator +(double d1, DataType &c1);
        
        DataType& operator *(DataType &c1);
        
        DataType& operator *(long i1);

        DataType& operator *(double d1);

        //rotate operators
        void operator <<=(long i1);

        void operator >>=(long i1);

        DataType& operator <<(long i1);

        DataType& operator >>(long i1);

        //shift operators
        DataType& operator <(long i1);

        DataType& operator >(long i1);

        friend DataType& operator ^(DataType c1,int i1);
        
        friend DataType& operator ^(DataType c1,double i1);
        
        friend DataType& operator *(int i1, DataType &c1);
        
        friend DataType& operator *(double d1, DataType &c1);
        
        friend std::ostream& operator <<(std::ostream &out, DataType &c1);

};