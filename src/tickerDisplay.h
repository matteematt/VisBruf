#ifndef TICKERDISPLAY
#define TICKERDISPLAY

#include <stdio.h>
//For getting the terminal width
#include <sys/ioctl.h>
#include <unistd.h>
/**
  * The width of the terminal window in chars
  * The width of the display
  * The height of the display
  * how many columns down is the top of the ticker displaying
  */
typedef struct TickerDisplay{
  int mTTYWidth;
  int mTickerWidth;
  int mTickerHeight;
  int mScrollDepth;
} TickerDisplay;

void td_clearTTY(void);

//returns the number of rows the function has drawn
int td_drawTicker(const TickerDisplay *dataTicker, const int dataIndex, const struct winsize *size);

#endif
