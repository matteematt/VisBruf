#include "dataTape.h"

void dt_DataTape(DataTape *data)
{
  data->mDataIndex = 0;
  data->mDataLen = 8;
  data->mData = calloc(data->mDataLen, sizeof(char));
}

void dt_DelDataTape(DataTape *data)
{
  free(data->mData);
}
