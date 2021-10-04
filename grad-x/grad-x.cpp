#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <iomanip>
#include <math.h>
using namespace std;

#include "../config.h"
#include "../common/utils.h"
#include <vector>

 vector<int64_t>  create_image(int N, int height, int width)
    {
        vector<int64_t>  v(N, 0ULL);

        for (int i = 0; i < height + 2; i++)
        {
            for (int j = 0; j < width + 2; j++)
            {
                int64_t t = 0;
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
    vector<int64_t> v0;
    vector<int64_t> pt_result(poly_modulus_degree,0);
    v0=create_image(poly_modulus_degree,  height,width);           //[Mod!] Init by passing by reference
             
    /*** Benchmark ***/
#if defined(VIP_ENC_MODE)
    VIP_VEC_ENCINT c0(v0, (int)poly_modulus_degree), c1, c2, c3, c4, c5,c_result;
    VIP_VEC_ENCINT c0_rot_w, c1_rot_nw,c2_rot_1,c2_rot_n1 ;
#else 
    VIP_VEC_ENCINT c0((int)poly_modulus_degree), c1, c2, c3, c4, c5,c_result;
    //copy v0 into c0 seperately
    std::copy(v0.begin(), v0.end(), std::begin(c0));    
    VIP_VEC_ENCINT c0_rot_w, c1_rot_nw,c2_rot_1,c2_rot_n1 ;
#endif
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

    std::valarray<int64_t> result=VIP_DEC(c_result); //decrypt result
    for(int i=0; i < result.size(); i++)
        std::cout << result[i] << ' ';
    std::cout << std::endl; 


    return 0;
}