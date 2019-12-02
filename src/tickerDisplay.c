#include "tickerDisplay.h"

//Maximum number of decimal digitis of a byte
static const int DIGITS_IN_BYTE = 3;
//The size required for a ANSI escape seq colour code and reset \x1b[36m\x1b[0m
static const int ANSI_COLOUR_SIZE = 9;

static const char *ANSI_CYAN = "\x1b[36m";
static const char *ANSI_RESET = "\x1b[0m";

//Private functions declarations
static char *drawTickerHR(int ttyWidth);
static void fillDataBuffer(char* dataBuffer, const DataTape *data, int colCount,
    int renderRow, bool *outOfBounds);
static void formatCharAsIntToString(char *string, unsigned char data);
static unsigned char getCharInPosition(unsigned char byte, int position);
static void colourSelectedCell(char *dataBuffer, int selectedCol,
    int dataStringLenRequirement);
static void createAddressRangeString(char *string, const TickerDisplay *display, int ttyWidth);

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

  //Maximum size that the string would need to be to hold data to print to the tty
  //Including colour escape sequences, newline, null byte
  int dataStringLenRequirement = ttyWidth + ANSI_COLOUR_SIZE + 2;

  //string buffer to hold the data row to print
  char *dataBuffer = malloc(sizeof(char) * dataStringLenRequirement);

  //The number of colums (data) to print
  int colCount = (int) (ttyWidth - 3) / 4;

  //get what line the currently selcted cell is
  int selectedLine = (int) data->mDataIndex / colCount;

  printf("\n%s\n", verticalSeperator);
  int renderRow;
  for (int i = 0; i < dataTicker->mTickerHeight; i++)
  {
    renderRow = i + dataTicker->mScrollDepth;
    memset(dataBuffer, 0x00, dataStringLenRequirement);
    fillDataBuffer(dataBuffer, data, colCount, renderRow, &outOfBounds);
    //If this is the selcted row then we want to colour in the selected cell
    if (selectedLine == renderRow)
    {
      //If protects against 0/0 erors
      int selectedCol = (selectedLine == 0) ? data->mDataIndex
        : data->mDataIndex % selectedLine;
      colourSelectedCell(dataBuffer, selectedCol, dataStringLenRequirement);
    }
    printf("%s\n", dataBuffer);
    printf("%s\n", verticalSeperator);
    drawLines += 2;
  }

  memset(dataBuffer, 0x00, dataStringLenRequirement);
  createAddressRangeString(dataBuffer, dataTicker, ttyWidth);
  printf("%s\n", dataBuffer);
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

//Do this logic in the loop instead, take the calcualion of colcount to before the loop
static void colourSelectedCell(char *dataBuffer, int selectedCol,
    int dataStringLenRequirement)
{
  //This is the char between the final digit of the selcted cell and the wall |
  int startPtr = 5 + selectedCol * 4;
  //Move this memory up be four bytes
  memmove((dataBuffer + startPtr + 4), (dataBuffer + startPtr),
      (dataStringLenRequirement - startPtr));
  //Copy the four bytes of the ANSI_RESET code
  memcpy((dataBuffer + startPtr), ANSI_RESET, 4);
  //This is the char between the first digit of the selcte cell and the wall |
  startPtr = 2 + selectedCol * 4;
  //Move this memory up by five bytes
  memmove((dataBuffer + startPtr + 5), (dataBuffer + startPtr),
      (dataStringLenRequirement - startPtr));
  //Copy in the five bytes for the cyan ansi colour code
  memcpy((dataBuffer + startPtr), ANSI_CYAN, 5);
}

static void fillDataBuffer(char* dataBuffer, const DataTape *data, int colCount,
    int renderRow, bool *outOfBounds)
{
  //Hold the digits for the data
  char *string = malloc(sizeof(char) * DIGITS_IN_BYTE);

  dataBuffer[0] = ' ';
  dataBuffer[1] = '|';

  int i = 0;
  for (; i < colCount; i++)
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
  //Newline is at the end of the chars written, calculated using the inverse of the
  //colcount, and extra for the next index
  int newlineIndex = (int) (i + 4) * 4;
  dataBuffer[newlineIndex] = '\n';

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

static void createAddressRangeString(char *string, const TickerDisplay *display, int ttyWidth)
{
  //The number of colums (data) to print
  int colCount = (int) (ttyWidth - 3) / 4;

  int firstIndex = colCount * display->mScrollDepth;
  int finalIndex = firstIndex + (colCount * display->mTickerHeight);

  const int OFFSET = 1;

  sprintf(&string[OFFSET], "Viewing addresses 0x%d to 0x%d\n", firstIndex, finalIndex);
  memset(string, 0x20, OFFSET);
}
