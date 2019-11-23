#include "tickerDisplay.h"

inline void td_clearTTY(void)
{
  printf("\e[1;1H\e[2J"); 
}

int td_drawTicker(const TickerDisplay *dataTicker, const int dataIndex, const struct winsize *size)
{
  return 2;
}
