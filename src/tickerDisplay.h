#include <stdio.h>

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

void td_drawTicker(const int dataPtr, const char const *dataTicker);
