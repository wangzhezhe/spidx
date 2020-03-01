#ifndef BBXTOOL_H
#define BBXTOOL_H

#ifdef __cplusplus
extern "C"
{
#endif

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>

//if this number is modified, the for loop part also need to be modified
#define DEFAULT_MAX_DIM 9
#define DEFAULT_3D 3

#define MAX(a, b) ((a > b) ? a : b)
#define MIN(a, b) ((a < b) ? a : b)

  // the bound value for specific dimention
  typedef struct Bound
  {
    uint64_t m_lb;
    uint64_t m_ub;
  }Bound;

  // the bbox for the application domain
  // the lb/ub represents the lower bound and the upper bound of every dimention
  // Attention, if there is 1 elements for every dimention, the lower bound is
  // same with the upper bound
  typedef struct BBXByBound
  {
    // the value of the m_dims represent the valid dimentions for this bounding
    uint16_t m_dims;
    // the default sequence is x-y-z
    Bound *BoundList[DEFAULT_MAX_DIM];
  }BBX;

  static inline BBX *initBBX(uint16_t dimNum, uint64_t indexlb[DEFAULT_MAX_DIM], uint64_t indexub[DEFAULT_MAX_DIM])
  {
    BBX *bbx = (BBX *)malloc(sizeof(BBX));
    bbx->m_dims = dimNum;
    // if there is only one dim, the second and third value will be the 0
    int i = 0;
    for (i = 0; i < dimNum; i++)
    {
      Bound *b = (Bound *)malloc(sizeof(Bound));
      b->m_lb = indexlb[i];
      b->m_ub = indexub[i];
      bbx->BoundList[i] = b;
    }
    return bbx;
  }

  static inline void deleteBBX(BBX *bbx)
  {
    if (bbx == NULL)
    {
      return;
    }

    int i = 0;
    for (i = 0; i < bbx->m_dims; i++)
    {
      free(bbx->BoundList[i]);
    }
    return;
  }

  static inline void printBBXinfo(BBX *bbx)
  {
    int i = 0;
    for (i = 0; i < bbx->m_dims; i++)
    {
      fprintf(stdout, "dim id: %d, lb: %d, ub %d\n", i, bbx->BoundList[i]->m_lb, bbx->BoundList[i]->m_ub);
    }
    return;
  }

  //if the bound of b is out of the bound of the a
  //return 0, no overlap
  //return 1, there is overlap
  static inline int ifOutofBound(Bound *a, Bound *b)
  {
    if (( b->m_ub < a->m_lb ) || ( b->m_lb > a->m_ub ))
    {
      return 0;
    }
    if(( b->m_lb < a->m_lb ) || ( b->m_ub > a->m_ub )){
      return 0;
    }
    
  }

  static inline Bound *getOverlapBound(Bound *a, Bound *b)
  {
    if ((a->m_ub < b->m_lb) || (a->m_lb > b->m_ub))
    {
      return NULL;
    }

    Bound *overlapBound = (Bound *)malloc(sizeof(Bound));
    overlapBound->m_lb = MAX(a->m_lb, b->m_lb);
    overlapBound->m_ub = MIN(a->m_ub, b->m_ub);

    return overlapBound;
  }

  //if the bbx "b" is out of the bound of the "a"
  static inline int ifOutofBBX(BBX *a, BBX *b)
  {
    uint64_t aDim = a->m_dims;
    uint64_t bDim = b->m_dims;
    if (aDim != bDim)
    {
      fprintf(stderr,
              "error: the dimention of two bounding box is not equal with each other %d vs %d\n",
              aDim, bDim);

      return 0;
    }

    if (aDim == 0 || aDim > DEFAULT_MAX_DIM || bDim == 0 || bDim > DEFAULT_MAX_DIM)
    {

      fprintf(stderr,
              "error: the dimention of bounding box can not be 0 or larger than 3, %d vs %d\n",
              aDim, bDim);

      return 0;
    }
    int i = 0;
    for (i = 0; i < aDim; i++)
    {
      int ifdimOverlap = ifOutofBound(a->BoundList[i], b->BoundList[i]);
      if (ifdimOverlap == 0)
      {
        return 0;
      }
    }
    return 1;
  }

  static inline BBX *getOverlapBBX(BBX *a, BBX *b)
  {
    uint64_t aDim = a->m_dims;
    uint64_t bDim = b->m_dims;

    if (aDim != bDim)
    {
      fprintf(stderr,
              "error: the dimention of two bounding box is not equal with each other %d vs %d\n",
              aDim, bDim);

      return NULL;
    }

    if (aDim == 0 || aDim > DEFAULT_MAX_DIM || bDim == 0 || bDim > DEFAULT_MAX_DIM)
    {

      fprintf(stderr,
              "error: the dimention of bounding box can not be 0 or larger than 3, %d vs %d\n",
              aDim, bDim);

      return NULL;
    }

    BBX *overlapBBX = (BBX *)malloc(sizeof(BBX));
    overlapBBX->m_dims = aDim;
    int i = 0;
    for (i = 0; i < aDim; i++)
    {
      Bound *bound = getOverlapBound(a->BoundList[i], b->BoundList[i]);

      if (bound == NULL)
      {
        // if there is not matching for any dimention, return NULL
        return NULL;
      }
      else
      {
        overlapBBX->BoundList[i] = bound;
      }
    }

    return overlapBBX;
  }

#ifdef __cplusplus
}
#endif

#endif

/*TODO, transform the following functions into C style
void getIndexlb(int *indexlb)
{
  for (int i = 0; i < m_dims; i++)
  {
    indexlb[i] = BoundList[i]->m_lb;
  }
  return indexlb;
}

void getIndexub(int *indexub)
{
  for (int i = 0; i < m_dims; i++)
  {
    indexub[i] = BoundList[i]->m_ub;
  }
  return indexub;
}

size_t getElemNum()
{
  size_t elemNum = 1;
  for (int i = 0; i < m_dims; i++)
  {
    elemNum = elemNum * (BoundList[i]->m_ub - BoundList[i]->m_lb + 1);
  }
  return elemNum;
}

void printBBXinfo()
{
  for (int i = 0; i < m_dims; i++)
  {
    printf("dim id: %d, lb: %d, ub %d\n", i, BoundList[i]->m_lb, BoundList[i]->m_ub);
    fflush(stdout);
  }
}

// calculate the physical position in BBX , for example, the lb is (5,0,0) and
// the ub is (10,0,0) if the coordinates is the (6,0,0) the index is the 1
// the sequency in memroy is x-y-z
size_t getPhysicalIndex(size_t coordim, int *coordinates)
{
  if (coordim != m_dims)
  {
    throw std::runtime_error("the dimension of the coordinates should same with the BBX");
  }
  int coordinatesNonoffset[DEFAULT_MAX_DIM] = {0};
  int shape[DEFAULT_MAX_DIM] = {0};
  for (int i = 0; i < m_dims; i++)
  {
    if (coordinates[i] > BoundList[i]->m_ub || coordinates[i] < BoundList[i]->m_lb)
    {
      throw std::runtime_error("the coordinates beyond the boundry of the BBX");
    }
    coordinatesNonoffset[i] = coordinates[i] - BoundList[i]->m_lb;
    shape[i] = BoundList[i]->m_ub - BoundList[i]->m_lb + 1;
  }

  size_t index = coordinatesNonoffset[2] * shape[1] * shape[0] +
                 coordinatesNonoffset[1] * shape[0] + coordinatesNonoffset[0];

  return index;
}





inline BBX *trimOffset(BBX *a, std::array<int, DEFAULT_MAX_DIM> offset)
{

  std::array<int, DEFAULT_MAX_DIM> indexlb = a->getIndexlb();
  std::array<int, DEFAULT_MAX_DIM> indexub = a->getIndexub();

  for (int i = 0; i < a->m_dims; i++)
  {
    indexlb[i] = indexlb[i] - offset[i];
    indexub[i] = indexub[i] - offset[i];
  }

  BBX *trimbbx = new BBX(a->m_dims, indexlb, indexub);

  return trimbbx;
}
*/
