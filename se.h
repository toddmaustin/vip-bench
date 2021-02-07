#include "../functional-library/src/datatypes/enc_lib.h"
#include "../functional-library/src/datatypes/enc_string.h"
#include "../functional-library/src/interface/interface.h"
#include "../functional-library/src/interface/parameters.h"

using namespace enc_lib;

void  SEInit(){
    setParameters_XOR(0x0505050505050505, 0x0505050505050505, 3022359314);
}