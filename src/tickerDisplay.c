#include "tickerDisplay.h"

void td_clearTTY(void)
{
  printf("\e[1;1H\e[2J"); 
}
