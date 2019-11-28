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
  data->mData = NULL;
}

inline void dt_growTape(DataTape *data)
{
  //double the data tape size
  data->mData = realloc(data->mData, sizeof(char) * data->mDataLen << 2);
  //set the new half of the data to be 0
  memset((data->mData + data->mDataLen), 0x00, data->mDataLen);
  //record that the data length has doubled
  data->mDataLen *= 2;
}
