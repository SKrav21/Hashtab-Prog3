#ifndef GLOBAL_DEFS
#define GLOBAL_DEFS

#define INITIALBUCKETS       100 
#define PERCENTAGETHRESHOLD   75 /* resize if exceeded */
#define CHAININGTHRESHOLD      5 /* resize if exceeded */
#define RESIZEPERCENTAGE      50

#include <stdint.h>    /* for uint*_t types */

struct node {
  uint64_t key;
  int value;
  struct node *next;
};
typedef struct node Node;

#endif
