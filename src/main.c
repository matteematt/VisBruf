#include <stdio.h>
#include <stdbool.h>
//For getting the terminal width
#include <sys/ioctl.h>
#include <unistd.h>

#include "tickerDisplay.h"
#include "dataTape.h"

int main(int argc, char **argv)
{
  printf("Hello Moto\n");

  TickerDisplay display = {.mScrollDepth = 0, .mTickerHeight = 6}; 

  DataTape data;
  dt_DataTape(&data);
  dt_DelDataTape(&data);
  dt_DelDataTape(&data);

  bool isRunning = true;

  char inputBuff[64];

  struct winsize size;

  while (isRunning)
  {
    td_clearTTY();

    //private struct to get the tty size, don't want to put a new one
    //on the stack every function invocation
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    printf("Width %d, Height %d\n", size.ws_col, size.ws_row);

    td_drawTicker(&display, &data, size.ws_col);

    printf("@ ");

    //Try and get input
    if (fgets(inputBuff, sizeof inputBuff, stdin))
    {

    }
    else
    {
      printf("Error getting input from user, exiting\n");
    }

    //Display the ticker display
  }

  dt_DelDataTape(&data);

  return 0;
}
