#include "SEALWrapper.h"
#include <iostream>
#include <vector>
#include <utils.h>

using namespace seal;
using namespace std;
using namespace he;
#define PI (4*atan(1))


std::vector<double> fill_rads(int N) {
        std::vector<double> v(N, 0ULL);
        double steps = 2.0*PI/N;

        for (int i = 0; i < N; i++) {
            v[i]=i*steps;
        }
        return v;
    }
std::vector<double> create_projected_vector_r(int N, int size, int min, int max, int copies=1) {
        std::vector<double> v(N, 0ULL);
        double value = rand() % max + min;

        for (int i = 0; i < size; i++) {
            for (int c = 0; c < copies; c++) {
            v[i+c*size] = value;
            }
        }

        return v;
    }
    std::vector<double> create_vector_r(int N, int size, int min, int max, int copies = 1)
    {
        std::vector<double> v(N, 0ULL);

        for (int i = 0; i < size; i++)
        {
            double value = rand() % max + min;
            for (int c = 0; c < copies; c++)
            {
                v[i + c * size] = value;
            }
        }

        return v;
    }
    std::vector<double> create_image_r(int N, int height, int width)
    {
        std::vector<double> v(N, 0ULL);

        for (int i = 0; i < height + 2; i++)
        {
            for (int j = 0; j < width + 2; j++)
            {
                double t = 0;
                if (j > 0 && i > 0 && j <= width && i <= height)
                {
                    t = (double)i;
                }
                v[i * (width + 2) + j] = t;
            }
        }

        return v;
    }
std::vector<double> get_base_img_r(int N, int height, int width){
  std::vector<double> img(N, 0ULL);
  for (int i = 0; i < height; i++){
    for (int j = 0; j < width; j++){
      img[i*width + j] = i*width+j;
    }
  }

  return img;
}


std::vector<double> create_image_r(int N, int height, int width, std::vector<double> img) {
  std::vector<double> v(N, 0ULL);

  for (int i = 0; i < height+2; i++) {
    for (int j = 0; j < width+2; j++) {
      double t = 0;
      if (j > 0 && i > 0 && j <= width && i <= height) {
        //t = (double) i;
        t = (double) img[ (i-1)*width + j-1];
      }
      v[i*(width+2) + j] = t;
    }
  }

  return v;
}

std::vector<double> pt_roberts_cross(int N, int height, int width, std::vector<double> img){
  std::vector<double> output(N, 0ULL);
  double gx, gy, gx2, gy2;;

  for (int i = 0; i < height-1; i++){
    for (int j = 0; j < width-1; j++){
      gx = img[i*width + j]     - img[(i+1)*width + (j+1)];
      gy = img[(i+1)*width + j] - img[i*width + (j+1)];
      gx2 = gx*gx;
      gy2 = gy*gy;
    }
  }

  return output;
}
 
    int main(int argc, char *argv[])
    {

        //command line input
        //command line input
        int iterations = 1;
        for (int i = 1; i < argc; i++)
        {
            if (strcmp(argv[i], "-d") == 0)
            {
                iterations = atoi(argv[i + 1]);
            }
        }
        SEALCipherText::scheme = scheme_type::CKKS;
        EncryptionParameters parms(SEALCipherText::scheme);
        size_t poly_modulus_degree = 16384;
        parms.set_poly_modulus_degree(poly_modulus_degree);
        auto coefMod = CoeffModulus::Create(poly_modulus_degree, {60, 40, 40, 60});
        parms.set_coeff_modulus(coefMod);
        //std::cout<<"CoeffModulus size: "<<coefMod.<<" bits"<<std::endl;
        //parms.set_coeff_modulus(CoeffModulus::Create(poly_modulus_degree,{40,20,20,20,20,20,40}));
        //parms.set_plain_modulus(8192);
        auto context = SEALContext::Create(parms);
        //print_parameters(context);
        KeyGenerator keygen(context);

        PublicKey public_key;   // = keygen.public_key();
        SecretKey secret_key;   // = keygen.secret_key();
        RelinKeys relin_keys;   // = keygen.relin_keys();
        GaloisKeys galois_keys; // = keygen.galois_keys();

        {
            Stopwatch s("KeyGen");
            SEALCipherText::public_key = keygen.public_key();
            SEALCipherText::secret_key = keygen.secret_key();
            SEALCipherText::relin_keys = keygen.relin_keys();
            SEALCipherText::galois_keys = keygen.galois_keys();
        }

        SEALCipherText::encryptor=new Encryptor(context, public_key);
        SEALCipherText::evaluator=new Evaluator(context);
        SEALCipherText::decryptor=new Decryptor(context, secret_key);

        SEALCipherText::ckksEncoder=new CKKSEncoder (context);

        SEALCipherText::context = context;
         
        SEALCipherText::scale = pow(2, 40);
       
        int nSlots = SEALCipherText::ckksEncoder->slot_count();
        std::cout << "Number of Slots:" << nSlots << std::endl;
        
        Stopwatch::numIter=iterations;
        Stopwatch::precision=true;
        //linear regression kernel
        {
            std::vector<double> v0, v1, v2, v3;
            std::vector<double> realRes;
            v0 = create_vector_r(nSlots, nSlots, 0, 4);
            v1 = create_vector_r(nSlots, nSlots, 0, 4);
            v2 = create_projected_vector_r(nSlots, nSlots, 0, 4);
            v3 = create_projected_vector_r(nSlots, nSlots, 0, 4);

            SEALCipherText c0(v0), c1(v1), c2(v2), c3(v3), c4, c5, c_result;
            {
                Stopwatch s("Linear Regression");
                for(int i=0;i<iterations;i++){
                c4 = c2 * c0;
                c5 = c1 - c4;
                c_result = c5 - c3;
                }
            }
            //std::cout<<c_result;
            realRes=(std::vector<double>)c_result;
            int64_t N=nSlots;
            std::vector<double> golden(N, 0);
            double m = v2[0];
            double b = v3[0];
            {
                Stopwatch s("Native Linear Regression");
                for (int i = 0; i < N; i++) {
                    golden[i] += v1[i] - (m * v0[i] + b);
                }
            }
            bool correct = true;
            for (int i = 0; i < N; i++) {
                correct &= (golden[i] == realRes[i]);
                if (!correct && i < 5){
                    std::cout << i << " golden result " << golden[i] << " " << realRes[i] << std::endl;
                }
            }
            std::cout<<"Correct "<<correct<<std::endl;
            int dataMemSize=0;
            
            dataMemSize+=c0.save("he_ciphertext"); 
            dataMemSize+=c1.save("he_ciphertext");  
            dataMemSize+=c2.save("he_ciphertext");  
           
            dataMemSize+=c3.save("he_ciphertext"); 
            dataMemSize+=c4.save("he_ciphertext");  
            dataMemSize+=c5.save("he_ciphertext");  
            dataMemSize+=c_result.save("he_ciphertext");  

            std::cout<<"Total Ciphertext memory footprint: "<<dataMemSize<<std::endl;
        }
        //polynomial regression
        {
            std::vector<double> v0, v1, v2, v3, v4;

            v0 = create_vector_r(nSlots, nSlots, 0, 4);
            v1 = create_vector_r(nSlots, nSlots, 0, 4);
            v2 = create_projected_vector_r(nSlots, nSlots, 0, 4);
            v3 = create_projected_vector_r(nSlots, nSlots, 0, 4);
            v4 = create_projected_vector_r(nSlots, nSlots, 0, 4);
            
            SEALCipherText c0(v0), c1(v1), c2(v2), c3(v3), c4(v4), c5, c6, c7, c8, c_result;
            {
                Stopwatch s("poly_regression");
                for(int i=0;i<iterations;i++){
                c5=c0*c4;
                c6=c5+c3;
                c7=c0*c6;
                c8=c7+c2;
                c_result=c1-c8;
                }
            }
            //std::cout<<c_result<<std::endl;
            // Verify
            double N = nSlots;

            //Plaintext p_result;
            std::vector<double> result;
            result=(std::vector<double>)c_result;
            //decryptor.decrypt(c_result, p_result);
            //batch_encoder.decode(p_result, result);

            std::vector<double> golden(N, 0);
            {

                Stopwatch s("Native polynomial regression:");
                for (int i = 0; i < N; i++) {
                    golden[i] += v1[i] - (v4[0] * v0[i] * v0[i] + v3[0] * v0[i] + v2[0]);
                }
            }
            bool correct = true;
            for (int i = 0; i < N; i++) {
                correct &= (golden[i] == result[i]);
                if (!correct && i < 5)
                std::cout << i << " golden result " << golden[i] << " " << result[i] << std::endl;
            }
            std::cout << "all batches correct " << correct << std::endl;
            c0.save("he_ciphertext");
            c1.save("he_ciphertext");
            c2.save("he_ciphertext"); 
            c3.save("he_ciphertext"); 
            c4.save("he_ciphertext"); 
            c5.save("he_ciphertext");
            c6.save("he_ciphertext");
            c7.save("he_ciphertext");
            c8.save("he_ciphertext");
            c_result.save("he_ciphertext");
        }
        //gradient computation in the x-direction
        {
            int w = 5;
            int width = 3;
            int height = 3;

            std::vector<double> v0;
            v0 = create_image_r(nSlots, height, width);
            SEALCipherText c0(v0),c1,c2,c3,c4,c5,c_result;
            SEALCipherText c0_rot_w, c1_rot_nw,c2_rot_1,c2_rot_n1;
            {
                Stopwatch s("Gradient x");
                for(int i=0;i<iterations;i++){
                c0_rot_w =c0 << w;
                c1 = c0_rot_w + c0;
                c1_rot_nw = c1 >> w;
                c2 = c1_rot_nw + c1;
                c2_rot_1 = c2 << 1;
                c2_rot_n1 = c2 >> 1;
                c_result = c2_rot_1 - c2_rot_n1;}
            }
            
            c0.save("he_ciphertext");
            c1.save("he_ciphertext");
            c2.save("he_ciphertext");
            //c3.save("he_ciphertext");
            //c4.save("he_ciphertext");
            //c5.save("he_ciphertext");
            c_result.save("he_ciphertext");
            c0_rot_w.save("he_ciphertext");
            c1_rot_nw.save("he_ciphertext");
            c2_rot_1.save("he_ciphertext");
            c2_rot_n1.save("he_ciphertext");
        }
        //Roberts Cross
        {
            int w = 5;
            int width = 3;
            int height = 3;

            std::vector<double> v0;
            v0 = create_image_r(poly_modulus_degree, height, width);
            SEALCipherText c0(v0),c1,c2,c3,c4,c5,c_result;
            SEALCipherText c0_rot_w, c0_rot_1,c0_rot_6;
            {
                Stopwatch s("Roberts Cross");
                for(int i=0;i<iterations;i++){
                c0_rot_w =c0 >> w;
                c0_rot_1 = c0 >> 1;
                c1 = c0_rot_w - c0_rot_1;
                c2 = c1 * c1;
                
                c0_rot_6 = c0 >> (w+1); 
                c3 = c0 - c0_rot_6;
                c4 = c3 * c3;
                
                c_result = c2 + c4;}
            }
            

            //save the ciphertexts
            c0.save("he_ciphertext");
            c1.save("he_ciphertext");
            c2.save("he_ciphertext");
            c3.save("he_ciphertext");
            c4.save("he_ciphertext");
            //c5.save("he_ciphertext");
            c_result.save("he_ciphertext");
            c0_rot_w.save("he_ciphertext");
            c0_rot_1.save("he_ciphertext");
            c0_rot_6.save("he_ciphertext");
        }
        //dot product
        {
            
            // Per kernel setup
            std::vector<double> v0, v1, result;

            
            v0 = create_vector_r(nSlots, nSlots, 0, 8);
            v1 = create_vector_r(nSlots, nSlots, 0, 8);

            SEALCipherText c0(v0),c1,c2,c3,c_result;
            SEALCipherText c1_rot_4, c2_rot_1, c3_rot_2;
            {
                Stopwatch s("Dot Product");
                for(int i=0;i<iterations;i++){
                c1=c0*v1;
                c1_rot_4=c1<<4;
                c2=c1+c1_rot_4;
                c2_rot_1=c2<<1;
                c3=c2_rot_1+c2;
                c3_rot_2=c3<<2;
                c_result=c3_rot_2+c3;}
                //c_result=c2_rot_1;
            }
            result=(std::vector<double>)c_result;
            //c0.save("c0.txt");
            // Verify
            int dataN = 8;
            int N = nSlots;
            int batch_size = N / dataN;
            std::vector<double> golden(batch_size, 0);
            {
                Stopwatch s("Native Dot Product");
                for (int i = 0; i < N; i++) {
                int current_batch = i / dataN;
                    golden[current_batch] += v0[i] * v1[i];
                }
            }

            bool correct = true;
            for (int i = 0; i < batch_size; i++) {
                correct &= (golden[i] == result[i * dataN]);
            }

            std::cout << "golden: " << golden[0] << " calculated: " << result[0] << std::endl;
            std::cout << "all batches correct " << correct << std::endl;
            //save the ciphertexts
            c0.save("he_ciphertext");
            c1.save("he_ciphertext");
            c2.save("he_ciphertext");
            c3.save("he_ciphertext");
            c_result.save("he_ciphertext");
            c1_rot_4.save("he_ciphertext");
            c2_rot_1.save("he_ciphertext"); 
            c3_rot_2.save("he_ciphertext");
        }
        //hamming distance
        {
            
            // Per kernel setup
            std::vector<double> v0, v1, d1, result;
           
            v0 = create_vector_r(nSlots, nSlots, 0, 2);
            v1 = create_vector_r(nSlots, nSlots, 0, 2);
            std::vector<double> d0(nSlots, 2);

            SEALCipherText c0(v0),c1(v1),c2,c3,c4,c5,c_result;
            SEALCipherText c2_neg, c4_rot_2, c5_rot_1;
            {
                Stopwatch s("Hamming Distance");
                for(int i=0;i<iterations;i++){
                c2=c1+c0;
                c2_neg=-c2;
                
                c3=c2_neg+d0;
                
                c4=c3*c2;
                c4_rot_2=c4<<2;
                c5=c4_rot_2+c4;
                c5_rot_1=c5<<1;
                c_result=c5_rot_1+c5;}
                
            }
            result=(std::vector<double>)c_result;
            //c0.save("c0.txt");
            
            // Verify
            int dataN = 4;
            int batch_size = nSlots / dataN;
            int64_t N = nSlots;


            std::vector<double> golden(batch_size, 0);
            {
                Stopwatch s("Native Hamming Distance");
                for (int i = 0; i < N; i++) {
                    int current_batch = i / dataN;
                    if (v0[i] != v1[i])
                        golden[current_batch] += 1;
                }
            }

            bool correct = true;
            for (int i = 0; i < batch_size; i++) {
                correct &= ((fabs(golden[i] - result[i * dataN]))<0.1);
                
            }
            std::cout << "Correct " << correct << std::endl;

            //save the ciphertexts
            c0.save("he_ciphertext");
            c1.save("he_ciphertext");
            c2.save("he_ciphertext");
            c3.save("he_ciphertext");
            c4.save("he_ciphertext");
            c5.save("he_ciphertext");
            c_result.save("he_ciphertext");
            c2_neg.save("he_ciphertext");
            c4_rot_2.save("he_ciphertext"); 
            c5_rot_1.save("he_ciphertext");
        }

        //rad-to-degree
        {
            double PI_INV=1.0/PI;

            std::vector<double> v0, results, golden(nSlots,0.0);
            

            v0 = fill_rads(nSlots);

            SEALCipherText c0(v0), c1,c_result;
            
            {
                Stopwatch s("Rad_to_degree");
                for(int i=0;i<iterations;i++){
                c1=c0*180.0;
                c_result=c1*PI_INV;
                }
            }
            results=(std::vector<double>)c_result;
            {
                Stopwatch s("Native Rad-to-Degree");
                for(int iter=0;iter<iterations;iter++)
                for(int i=0;i<nSlots;i++)
                    golden[i]=180.0 * v0[i] / (PI);
            }
            bool correct;
            for(int i=0;i<nSlots;i++)
            {
                correct&=(fabs(golden[i]-results[i])<0.1);
                if(i<5)
                    std::cout<<golden[i]<<"  "<<results[i]<<std::endl;
            }
            std::cout << "Correct " << correct << std::endl;

        }
        //rad-to-degree
        {
            int stepsValue=2;
            double steps=stepsValue;
            double x=1.0, y=1/steps;
            std::vector<double> results, golden(nSlots,0.0);
            SEALCipherText c0(1.0), c1(1.0/steps),c2,c3;
            
            {
                Stopwatch s("Euler-number-approx");
                for(int i=0;i<nSlots;i++){
                        c2=c0;
                        c3=c1;
                for(;steps>0;steps--)
                    c2=c2*c3;
                }
            }
            results=(std::vector<double>)c2;
            {
                Stopwatch s("Native Euler-number-approx");
                for(int iter=0;iter<iterations;iter++)
                {
                    for(int i=0;i<nSlots;i++){
                        steps=stepsValue;
                        golden[i]=0.0;
                        x=1.0;y=1/steps;
                        for(;steps>0;steps--)
                            x=x*y;
                        golden[i]=x;  
                    }
                }
            }
            bool correct;
            for(int i=0;i<nSlots;i++)
            {
                correct&=(fabs(golden[i]-results[i])<0.1);
                if(i<5)
                    std::cout<<golden[i]<<"  "<<results[i]<<std::endl;
            }
            std::cout << "Correct " << correct << std::endl;

        }

        return 0;
    }
