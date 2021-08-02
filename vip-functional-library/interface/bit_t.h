#ifndef _UINT128_T_H
#define _UINT128_T_H
#include <stdlib.h> 
#include <bitset>
#include <string.h>
#include <iostream>

typedef struct bit128_t{

public: //SHOULD BE PRIVATE
    uint8_t value[16]; 
    /* THE bit128_t class stores values according to the following structure: 
    *
    * value[16] = [MSB] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [xxx] [LSB]
    *               0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15                                                                                                                   
    *             [------------------UPPER 64b------------------] [------------------LOWER 64b-------------------] 
    * 
    * FOR 64-bit values, like INT
    * value[16] = [INT] [INT] [INT] [INT] [INT] [INT] [INT] [INT] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad]
    *               0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15                                                                                                                   
    *             [------------------UPPER 64b------------------] [------------------LOWER 64b-------------------] 
    * 
    * FOR 8-bit values, like BOOL/CHAR
    * value[16] = [B/C] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad] [pad]
    *               0     1     2     3     4     5     6     7     8     9    10    11    12    13    14    15                                                                                                                   
    *             [------------------UPPER 64b------------------] [------------------LOWER 64b-------------------] 
    *             [---]
    *               ^ UPPER 8b
    */

private:
    char bit_str[129]; //128 bit-characters, followed by '\0'
    char hex_str[33];  //16  hex-characters, followed by '\0'


public:
    /******** CONSTRUCTORS ********/
    bit128_t(){
        for(int i=0; i<16; i++){
            value[i] = 0x00; //0000 0000
        } 
    }
    bit128_t(uint64_t upper_value, uint64_t lower_pad){
        init(upper_value, lower_pad);
    }
    bit128_t(uint8_t upper_value, uint8_t* lower_pad){
        /* Store upper_value in MSB/Index 0 */
        value[0]= upper_value;
        /* Store lower_pad in 8-bit segments */
        for(int i=15; i>=1 ; i--){
            value[i] = lower_pad[i-1];
        } 
    }
    bit128_t(uint8_t* full_value){
        /* Store full_value in 8-bit segments */
        for(int i=0; i<16; i++){
            value[i] = full_value[i];
        } 
    }
    void init(uint64_t upper_value, uint64_t lower_pad){
        /* Break and store upper_value in 8-bit segments */
        for(int i=7; i>=0; i--){
            value[i] = upper_value & 0xFF ; 
            upper_value = upper_value >> 8;
        }
        /* Break and store lower_pad in 8-bit segments */
        for(int i=15; i>=8; i--){
            value[i] = lower_pad & 0xFF;
            lower_pad = lower_pad >> 8;
        } 
    }

    /******** VALUE ACCESS FUNCTIONS ********/
    uint64_t getUpperValue_64b(){
        uint64_t ret = 0;
        for(int i=7; i>=0; i--){
            ret = ret + ((uint64_t)value[8-i-1]);
            if(i != 0){ret = ret << 8;} 
        }
        return ret;        
    }

    uint32_t getUpperValue_32b(){
        uint32_t ret = 0;
        for(int i=4; i>=0; i--){
            ret = ret + ((uint32_t)value[8-i-1]);
            if(i != 0){ret = ret << 8;} 
        }
        return ret;        
    }
    uint32_t getMidValue_32b(){
        uint32_t ret = 0;
        for(int i=7; i>=4; i--){
            ret = ret + ((uint32_t)value[8-i-1]);
            if(i != 0){ret = ret << 8;} 
        }
        return ret;        
    }

    uint64_t getUpperValue_8b(){
        return value[0];
    }


    uint64_t getLowerValue_64b(){
        uint64_t ret = 0;
        for(int i=15; i>=8; i--){
            ret = ret + ((uint64_t)value[16-(i-7)]);
            if(i != 8){ret = ret << 8;} 
        }
        return ret;      
    }

    // uint32_t getUpperValue_32b(){
    //     uint32_t ret = 0;
    //     for(int i=3; i>=0; i--){
    //         ret = ret + ((uint32_t)value[8-i-1]);
    //         if(i != 0){ret = ret << 8;} 
    //     }
    //     return ret;        
    // }
    // uint32_t getMidUpperValue_32b(){
    //     uint32_t ret = 0;
    //     for(int i=7; i>=4; i--){
    //         ret = ret + ((uint32_t)value[8-i-1]);
    //         if(i != 0){ret = ret << 8;} 
    //     }
    //     return ret;        
    // }
    // uint32_t getMidLowerValue_32b(){
    //     uint32_t ret = 0;
    //     for(int i=11; i>=8; i--){
    //         ret = ret + ((uint32_t)value[8-i-1]);
    //         if(i != 0){ret = ret << 8;} 
    //     }
    //     return ret;        
    // }
    // uint32_t getLowerValue_32b(){
    //     uint32_t ret = 0;
    //     for(int i=15; i>=12; i--){
    //         ret = ret + ((uint32_t)value[8-i-1]);
    //         if(i != 0){ret = ret << 8;} 
    //     }
    //     return ret;        
    // }

    /******** PRINT UTILITIES ********/
    char* get_bit_cstr(){
        char* dest = bit_str; 
        for(int i=0; i<16; i++){
            // Convert uint8_t to bits
            std::bitset<8> x(value[i]); 
            const char* mystr = x.to_string().c_str();
            // Copy bits into destination
            strncpy(dest, mystr, 8);
            dest += 8;
        }
        bit_str[128] = '\0';
        return bit_str;
    }

    char* get_hex_cstr(){
        uint8_t* values = value; 
        int val_len = 16;
        char* dest = hex_str; 
        while(val_len--){
            // Convert uint8_t into hex and copy into destination
            sprintf(dest, "%02x", *values);
            dest += 2;
            ++values;
        }
        hex_str[32] = '\0';
        return hex_str;
    }

    void print_hex(){
        char* to_print = get_hex_cstr();
        fprintf(stderr,"\t0x");
        for(int i=0; i<32; i+=2){
            fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
        }
        fprintf(stderr,"\n");
    }

    void print_hex_parted_32(){
        char* to_print = get_hex_cstr();
        fprintf(stderr,"0x");
        for(int i=0; i<8; i+=2){
            fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
        }
        fprintf(stderr,"\n");
        fprintf(stderr,"0x");
        for(int i=8; i<16; i+=2){
            fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
        }
        fprintf(stderr,"\n");
        fprintf(stderr,"0x");
        for(int i=16; i<24; i+=2){
            fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
        }
        fprintf(stderr,"\n");
        fprintf(stderr,"0x");
        for(int i=24; i<32; i+=2){
            fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
        }
        fprintf(stderr,"\n");
    }

    // void print_int_parted_32(){
    //     char* to_print = get_hex_cstr();
    //     for(int i=0; i<8; i+=2){
    //         fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
    //     }
    //     fprintf(stderr,"\n");
    //     for(int i=8; i<16; i+=2){
    //         fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
    //     }
    //     fprintf(stderr,"\n");
    //     for(int i=16; i<24; i+=2){
    //         fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
    //     }
    //     fprintf(stderr,"\n");
    //     for(int i=24; i<32; i+=2){
    //         fprintf(stderr," %c%c", to_print[i], to_print[i+1]);
    //     }
    //     fprintf(stderr,"\n");

    // }

    void print_bin(){
        char* to_print = get_bit_cstr();
        printf("\t  ");
        for(int i=0; i<16; i++){
            printf(" ");
            for(int j=0; j<8; j++){
                // Lazy about typing, so doing a double loop here :) 
                printf("%c", to_print[(8*i) + j]);
            }
        }
        printf("\n");
    }

    void print_bin_parted_32(){
        char* to_print = get_bit_cstr();
        for(int i=0; i<4; i++){
            for(int j=0; j<8; j++){
                // Lazy about typing, so doing a double loop here :) 
                fprintf(stderr,"%c", to_print[(8*i) + j]);
            }
        }
        fprintf(stderr,"\n");
        for(int i=4; i<8; i++){
            for(int j=0; j<8; j++){
                // Lazy about typing, so doing a double loop here :) 
                fprintf(stderr,"%c", to_print[(8*i) + j]);
            }
        }
        fprintf(stderr,"\n");
        for(int i=8; i<12; i++){
            for(int j=0; j<8; j++){
                // Lazy about typing, so doing a double loop here :) 
                fprintf(stderr,"%c", to_print[(8*i) + j]);
            }
        }
        fprintf(stderr,"\n");
        for(int i=12; i<16; i++){
            for(int j=0; j<8; j++){
                // Lazy about typing, so doing a double loop here :) 
                fprintf(stderr,"%c", to_print[(8*i) + j]);
            }
        }
        fprintf(stderr,"\n");
    }

    //Print as UNSINGED INT
    void print_parted_32(){
        uint32_t ret = 0;
        for(int i=0; i<4; i++){
            ret = ret + ((uint64_t)value[i]);
            if(i != 3){ret = ret << 8;} 
        }
        fprintf(stderr, "%u\n", ret);
        ret = 0;
        for(int i=4; i<8; i++){
            ret = ret + ((uint64_t)value[i]);
            if(i != 7){ret = ret << 8;} 
        }
        fprintf(stderr, "%u\n", ret);
        ret = 0;
        for(int i=8; i<12; i++){
            ret = ret + ((uint64_t)value[i]);
            if(i != 11){ret = ret << 8;} 
        }
        fprintf(stderr, "%u\n", ret);
        ret = 0;
        for(int i=12; i<16; i++){
            ret = ret + ((uint64_t)value[i]);
            if(i != 15){ret = ret << 8;} 
        }
        fprintf(stderr, "%u\n", ret);
        ret = 0;
    }



} bit128_t;


// typedef struct bit64_t{
//     uint8_t value[8]; //Index 0 contains LSB
//     char str[65]; //LSB stoerd at index 64

// public:
//     //Constructors
//     bit64_t(){
//         for(int i=0; i<8; i++){
//             value[i] = 0x00;
//         } 
//     }
//     bit64_t(uint64_t upper, uint8_t lower){
//         initByte(upper, lower);
//     }

//     void initByte(uint64_t upper, uint8_t lower){
//         value[0] = lower;
//         for(int i=1; i<7; i++){ //Skip MSB of Upper!
//             value[i] = upper & 0xFF;
//             upper = upper >> 8;
//         } 
//     }

//     void init(uint64_t upper, uint64_t lower){
//         for(int i=0; i<8; i++){
//             value[i] = lower & 0xFF ;
//             lower = lower >> 8;
//         }
//         for(int i=8; i<16; i++){
//             value[i] = upper & 0xFF;
//             upper = upper >> 8;
//         } 
//     }

//     char* c_str(){
//         for(int i=0; i<8; i++){
//             const char* i_str = std::bitset<8>(value[i]).to_string().c_str(); //LSB stored at index 7
//             for (int j=0; j<8; j++){
//                 str[(8*(8-i-1)) +j] = i_str[j];
//             }
//         }  
//         str[64] = '\0';
//         return str;
//     }

//     // uint64_t getLower(){
//     //     uint64_t ret = 0;
//     //     for(int i=0; i<8; i++){
//     //         if(i != 0){ret = ret << 8;} //Shift previous addition
//     //         ret = ret + ((uint64_t)value[8-i-1]); 
//     //     }
//     //     return ret;
//     // }

//     uint8_t getLowerByte(){
//         return value[0];
//     }

//     // uint64_t getUpper(){
//     //     uint64_t ret = 0;
//     //     for(int i=8; i<16; i++){
//     //         ret = ret + value[i];
//     //         ret = ret << 8;
//     //     }
//     //     return ret;
//     // }

// } bit64_t;


#endif