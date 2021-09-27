#include "wrapper/SEALWrapper.h"

using namespace he;
 /*
Helper function: Prints the parameters in a SEALContext.
*/
inline void print_parameters(std::shared_ptr<seal::SEALContext> context)
{
    // Verify parameters
    if (!context)
    {
        throw std::invalid_argument("context is not set");
    }
    auto &context_data = *context->key_context_data();

    /*
    Which scheme are we using?
    */
    std::string scheme_name;
    switch (context_data.parms().scheme())
    {
    case seal::scheme_type::BFV:
        scheme_name = "BFV";
        break;
    case seal::scheme_type::CKKS:
        scheme_name = "CKKS";
        break;
    default:
        throw std::invalid_argument("unsupported scheme");
    }
    std::cout << "/" << std::endl;
    std::cout << "| Encryption parameters :" << std::endl;
    std::cout << "|   scheme: " << scheme_name << std::endl;
    std::cout << "|   poly_modulus_degree: " <<
        context_data.parms().poly_modulus_degree() << std::endl;

    /*
    Print the size of the true (product) coefficient modulus.
    */
    std::cout << "|   coeff_modulus size: ";
    std::cout << context_data.total_coeff_modulus_bit_count() << " (";
    auto coeff_modulus = context_data.parms().coeff_modulus();
    std::size_t coeff_mod_count = coeff_modulus.size();
    for (std::size_t i = 0; i < coeff_mod_count - 1; i++)
    {
        std::cout << coeff_modulus[i].bit_count() << " + ";
    }
    std::cout << coeff_modulus.back().bit_count();
    std::cout << ") bits" << std::endl;

    /*
    For the BFV scheme print the plain_modulus parameter.
    */
    if (context_data.parms().scheme() == seal::scheme_type::BFV)
    {
        std::cout << "|   plain_modulus: " << context_data.
            parms().plain_modulus().value() << std::endl;
    }

    std::cout << "\\" << std::endl;
}


void  SEAL_CKKS_Init(){
        int scaleBits=40;
        SEALCipherText::scheme=scheme_type::CKKS;
        EncryptionParameters parms(SEALCipherText::scheme);
        size_t poly_modulus_degree = 16384;
        parms.set_poly_modulus_degree(poly_modulus_degree);
        auto coefMod=CoeffModulus::Create(poly_modulus_degree,{60,scaleBits,scaleBits,60});
        parms.set_coeff_modulus(coefMod);

        auto context = SEALContext::Create(parms);
        SEALCipherText::context=context;

        KeyGenerator keygen(SEALCipherText::context);
        
        SEALCipherText::public_key=keygen.public_key();
        SEALCipherText::secret_key=keygen.secret_key();
        SEALCipherText::relin_keys=keygen.relin_keys();
        SEALCipherText::galois_keys=keygen.galois_keys();

        SEALCipherText::evaluator=new Evaluator(context);
        SEALCipherText::decryptor=new Decryptor(context, SEALCipherText::secret_key);
        SEALCipherText::encryptor=new Encryptor(context, SEALCipherText::public_key);
        SEALCipherText::scale=pow(2,scaleBits);
        SEALCipherText::ckksEncoder=new CKKSEncoder(context);

        if(SEALCipherText::encryptor==NULL)
            std::cout<<"Failed to allocate space for the encrypter"<<std::endl;
}
void SEAL_BFV_Init()
{
    SEALCipherText::scheme=scheme_type::BFV;
    EncryptionParameters parms(SEALCipherText::scheme);
   
        size_t poly_modulus_degree = 8192;
        parms.set_poly_modulus_degree(poly_modulus_degree);
        parms.set_coeff_modulus(CoeffModulus::BFVDefault(poly_modulus_degree));
        parms.set_plain_modulus(PlainModulus::Batching(poly_modulus_degree,20));
        auto context = SEALContext::Create(parms);
        
        SEALCipherText::context=context;
        KeyGenerator keygen(context);

        SEALCipherText::public_key=keygen.public_key();
        SEALCipherText::secret_key=keygen.secret_key();
        SEALCipherText::relin_keys=keygen.relin_keys();
        SEALCipherText::galois_keys=keygen.galois_keys();

        SEALCipherText::evaluator=new Evaluator(context);
        SEALCipherText::decryptor=new Decryptor(context, SEALCipherText::secret_key);
        SEALCipherText::encryptor=new Encryptor(context, SEALCipherText::public_key);
        SEALCipherText::intEncoder=new IntegerEncoder(context);
        SEALCipherText::batchEncoder=new BatchEncoder(context);    
}


#define enc_char    char
#define enc_int     SEALCipherText
#define enc_uint    SEALCipherText
#define enc_bool    bool
#define enc_float   float
#define enc_double  SEALCipherText
