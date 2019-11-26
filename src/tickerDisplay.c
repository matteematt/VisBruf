#include "tickerDisplay.h"

static const int DIGITS_IN_BYTE = 3;

//Private functions declarations
static char *drawTickerHR(int ttyWidth);
static void fillDataBuffer(char* dataBuffer, const DataTape *data, int ttyWidth, 
    int renderRow, bool *outOfBounds);
static void formatCharAsIntToString(char *string, unsigned char data);
static unsigned char getCharInPosition(unsigned char byte, int position);

inline void td_clearTTY(void)
{
  printf("\e[1;1H\e[2J"); 
}

int td_drawTicker(const TickerDisplay *dataTicker, const DataTape *data, int ttyWidth)
{
  //Keep track of how many lines have been drawn
  int drawLines = 2;

  bool outOfBounds = false;

  char *verticalSeperator = drawTickerHR(ttyWidth);

  //string buffer to hold the data row to print
  char *dataBuffer = malloc(sizeof(char) * (ttyWidth + 1));

  printf("\n%s\n", verticalSeperator); 
  for (int i = 0; i < dataTicker->mTickerHeight; i++)
  {
    fillDataBuffer(dataBuffer, data, ttyWidth, i + dataTicker->mScrollDepth, &outOfBounds);
    printf("%s\n", dataBuffer);
    printf("%s\n", verticalSeperator);
    drawLines += 2;
  }
  printf("\n");
  drawLines++;

  free(verticalSeperator);
  free(dataBuffer);

  return drawLines;
}
//Treating the byte as a 8bit number, get the positionth digit from its decimal representatioin
//char 234, position 0 = 4, position 1 = 3, position 2 = 2
static inline unsigned char getCharInPosition(unsigned char byte, int position)
{
  return (unsigned char) (byte / pow(10, position)) % 10;
}

static void formatCharAsIntToString(char *string, unsigned char data)
{
  const int ASCII_OFFSET = 48;
  for (int i = 0; i < DIGITS_IN_BYTE; i++)
  {
    string[i] = getCharInPosition(data, DIGITS_IN_BYTE - 1 - i) + ASCII_OFFSET;
  }
}

static void fillDataBuffer(char* dataBuffer, const DataTape *data, int ttyWidth, 
    int renderRow, bool *outOfBounds)
{
  int colCount = (int) (ttyWidth - 3) / 4;

  char *string = malloc(sizeof(char) * DIGITS_IN_BYTE);

  dataBuffer[0] = ' ';
  dataBuffer[1] = '|';

  for (int i = 0; i < colCount; i++) 
  {
    if (*outOfBounds)
    {
      dataBuffer[2 + i * 4] = ' ';
      dataBuffer[3 + i * 4] = '~';
      dataBuffer[4 + i * 4] = ' ';
      dataBuffer[5 + i * 4] = '|';
    }
    else
    {
      int dataIndex = colCount * renderRow + i;
      formatCharAsIntToString(string, data->mData[dataIndex]);
      dataBuffer[2 + i * 4] = string[0];
      dataBuffer[3 + i * 4] = string[1];
      dataBuffer[4 + i * 4] = string[2];
      dataBuffer[5 + i * 4] = '|';
      if (dataIndex >= data->mDataLen - 1)
      {
        //Next time we will be out of bounds
        *outOfBounds = true;
      }
    }
  }
  dataBuffer[ttyWidth - 1] = ' ';
  dataBuffer[ttyWidth] = '\0';

  free(string);
}

static char *drawTickerHR(int ttyWidth)
{
  char *seperator = malloc(sizeof(char) * (ttyWidth + 1));

  //two chars used for padding, and one for the left-most column
  //then each box takes up two columns
  int colCount = (int) (ttyWidth - 3) / 4;

  seperator[0] = ' ';
  seperator[1] = '+';

  for (int i = 0; i < colCount; i++)
  {
    seperator[2 + i * 4] = '-';
    seperator[3 + i * 4] = '-';
    seperator[4 + i * 4] = '-';
    seperator[5 + i * 4] = '+';
  }
  seperator[ttyWidth - 1] = ' ';
  seperator[ttyWidth] = '\0';

  return seperator;
}
