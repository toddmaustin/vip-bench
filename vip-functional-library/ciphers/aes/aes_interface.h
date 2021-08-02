#ifndef _AES_INTERFACE_H
#define _AES_INTERFACE_H

// #include "../../interface/interface.h"
#include "../../interface/parameters.h"
#include <iostream>
#include <bitset>
// #include "tinyCrypt_without_Git/lib/include/tinycrypt/aes.h"
#include "aes128ni.h"


#define NUM_OF_NIST_KEYS 16
#define NUM_OF_FIXED_KEYS 128

//extern struct tc_aes_key_sched_struct s_g;


void aes128_set_key(bit128_t);

bit128_t aes128_encrypt_128(bit128_t);

bit128_t aes128_decrypt_128(bit128_t);

#endif
