#ifndef _AES_INTERFACE_CPP
#define _AES_INTERFACE_CPP

#include "aes_interface.h"


// struct tc_aes_key_sched_struct s_g;
struct aes128 ctx[1];

void aes128_set_key(bit128_t key_pass){
 	// tc_aes128_set_encrypt_key(&s_g, SECRET_KEY.value);
    const unsigned char key[] = {
        0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6,
        0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c
    };
    aes128_init(ctx, key);
}


bit128_t aes128_encrypt_128(bit128_t plaintext){
	
	// uint8_t temp_ciphertext[NUM_OF_NIST_KEYS];
	// tc_aes_encrypt(temp_ciphertext, plaintext.value, &s_g);

	unsigned char ct[AES128_BLOCKLEN] = {0};
	aes128_encrypt(ctx, ct, plaintext.value);

	
    return bit128_t(ct);
}


bit128_t aes128_decrypt_128(bit128_t ciphertext){

	// uint8_t temp_plaintext[NUM_OF_NIST_KEYS];
	// tc_aes_decrypt(temp_plaintext, ciphertext.value, &s_g);
	
	unsigned char pt[AES128_BLOCKLEN] = {0};
	aes128_decrypt(ctx, pt, ciphertext.value);

	
    return bit128_t(pt);
}


#endif
