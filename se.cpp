#ifndef SE_CPP
#define SE_CPP
/*
#include "../functional-library/src/datatypes/enc_lib.h"
#include "../functional-library/src/datatypes/enc_string.h"
#include "../functional-library/src/interface/interface.h"
#include "../functional-library/src/interface/parameters.h"
*/
#include "se.h"


//using namespace enc_lib;

void  SEInit(){
    enc_lib::setParameters_XOR(0x0505050505050505, 0x0505050505050505, 3022359314);
}

#endif
