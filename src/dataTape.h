#include <stdlib.h>

/**
  * mData is a pointer to a chars representing the data
  * mDataIndex is the integer index of the data pointer
  * mDataLen is the current max length of the mData array, once exceeded
  * the array then doubles in length
  */
typedef struct DataTape {
  char *mData;
  int mDataIndex;
  int mDataLen;
} DataTape;

void dt_DataTape(DataTape *data);

void dt_DelDataTape(DataTape *data);
