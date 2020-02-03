
#include "../src/bbxtool.h"

void testBBXInit(){
    size_t lb[DEFAULT_MAX_DIM]={0,0,0};
    size_t ub[DEFAULT_MAX_DIM]={5,6,7};
    BBX* bbx = initBBX(3,lb,ub);
    printBBXinfo(bbx);
    return;
}

int main(){
    testBBXInit();
}