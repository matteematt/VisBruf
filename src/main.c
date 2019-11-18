#include <stdio.h>
#include <stdbool.h>

#include "tickerDisplay.h"
#include "dataTape.h"

int main(int argc, char **argv)
{
  printf("Hello Moto\n");

  TickerDisplay display = {.mScrollDepth = 0, .mTickerHeight = 6}; 

  DataTape data;
  dt_DataTape(&data);

  bool isRunning = true;

  char inputBuff[64];

  while (isRunning)
  {
    td_clearTTY();

    printf("@ ");
    
    //Try and get input
    if (fgets(inputBuff, sizeof inputBuff, stdin))
    {

    }
    else
    {
      printf("Error getting input from user, exiting\n");
    }
  }

  return 0;
}
