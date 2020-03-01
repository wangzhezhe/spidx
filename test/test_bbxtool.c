
#include "../src/utils/bbxtool.h"

void testBBXInit()
{
    printf("---testBBXInit---\n");
    size_t lb[DEFAULT_MAX_DIM] = {0, 0, 0};
    size_t ub[DEFAULT_MAX_DIM] = {5, 6, 7};
    BBX *bbx = initBBX(3, lb, ub);
    printBBXinfo(bbx);
    return;
}

void testBBXOverlap()
{
    printf("---testBBXOverlap---\n");
    size_t lb[DEFAULT_MAX_DIM] = {0, 0, 0};
    size_t ub[DEFAULT_MAX_DIM] = {5, 6, 7};
    BBX *bbx1 = initBBX(3, lb, ub);

    lb[0] = 1;
    lb[1] = 1;
    lb[2] = 1;
    ub[0] = 3;
    ub[1] = 3;
    ub[2] = 3;
    BBX *bbx2 = initBBX(3, lb, ub);

    BBX *overlapBBX = getOverlapBBX(bbx1, bbx2);
    printBBXinfo(overlapBBX);
    return;
}

int main()
{
    testBBXInit();
    testBBXOverlap();
}