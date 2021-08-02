#ifndef TEST_UTILS_H_
#define TEST_UTILS_H_
#include <stdint.h>
#include "../../interface/bit_t.h"

static inline void print_ciphertext(bit128_t ciphertext){
    for(int i=0;i<16;i++){
        printf("%02x",ciphertext.value[i]);
    }
}
static inline void print_plaintext(bit128_t plaintext){
    for(int i=0;i<16;i++){
        printf("%02x",plaintext.value[i]);
    }
}
static inline void print_secret(bit128_t SECRET_KEY){
    for(int i=0;i<16;i++){
        printf("%02x",SECRET_KEY.value[i]);
    }
}



#endif