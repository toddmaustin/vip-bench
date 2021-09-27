#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include "../config.h"
#include "../common/utils.h"
#include <vector>

 VIP_VEC  create_image(int N, int height, int width)
    {
        VIP_VEC  v(N, 0ULL);

        for (int i = 0; i < height + 2; i++)
        {
            for (int j = 0; j < width + 2; j++)
            {
                VIP_VEC_TYPE t = 0;
                if (j > 0 && i > 0 && j <= width && i <= height)
                {
                    t = (int64_t)i;
                }
                v[i * (width + 2) + j] = t;
            }
        }

        return v;
    }
 

int
main(void) 
{ 
  VIP_INIT; 
  int poly_modulus_degree = 8192;

    /*** Initialize data vectors, similar to HE ***/
    /**********************************************/
    int w = 5;
    int width = 3;
    int height = 3;
    VIP_VEC v0, result;
    VIP_VEC pt_result(poly_modulus_degree,0);
    v0=create_image(poly_modulus_degree,  height,width);           //[Mod!] Init by passing by reference
             
    /*** Benchmark ***/
    
#if defined(VIP_ENC_MODE)
    /*****************/
    VIP_ENCINT c0(v0, (int)poly_modulus_degree), c1, c2, c3, c4, c5,c_result;
    VIP_ENCINT c0_rot_w, c1_rot_nw,c2_rot_1,c2_rot_n1 ;
    {
        Stopwatch s("Gradient X");
        c0_rot_w =c0 << w;
        c1 = c0_rot_w + c0;
        c1_rot_nw = c1 >> w;
        c2 = c1_rot_nw + c1;
        c2_rot_1 = c2 << 1;
        c2_rot_n1 = c2 >> 1;
        c_result = c2_rot_1 - c2_rot_n1;
    }
    result=(VIP_VEC)c_result; //decrypt result

    for(int i=0; i < result.size(); i++)
        std::cout << result.at(i) << ' ';

#else
    /*** Native/DO mode  ***/
    /************************************************/
             
    {
        Stopwatch s("Native Gradient X");
        //for(int iter=0;iter<iterations;iter++){
            for (int i = 0; i < height+2; i++) {
                for (int j = 0; j < width+2; j++) {
                int ii = i*(width+2) + j;
                int temp_out = 0;

                // Top left
                if (ii-6 > 0){
                    temp_out -= v0[ii-6];
                }
                // Top right
                if (ii-4 > 0){
                    temp_out += v0[ii-4];
                }

                // Middle left
                if(ii-1 > 0){
                    temp_out = temp_out - 2*v0[ii-1];
                }
                // Middle right
                temp_out = temp_out + 2*v0[ii+1];

                // Bottom left
                temp_out = temp_out - v0[ii+4];
                // Bottom right
                temp_out = temp_out + v0[ii+6];

                pt_result[ii] = temp_out;
                // cout << result[ii] << " ";
                cout << temp_out << " ";
                }
                //cout << endl;
                    
        }
    }            
#endif
    // bool correct = true;
   
    // std::cout<<"Correct "<<correct<<std::endl;

    return 0;
}