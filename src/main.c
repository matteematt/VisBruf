#include <stdio.h>
#include <stdbool.h>
//For getting the terminal width
#include <sys/ioctl.h>
#include <unistd.h>

#include "tickerDisplay.h"
#include "dataTape.h"
#include "prompt.h"

int main(int argc, char **argv)
{
  TickerDisplay display = {.mScrollDepth = 0, .mTickerHeight = 6}; 

  DataTape data;
  dt_DataTape(&data);

  Prompt prompt;
  p_Prompt(&prompt);

  bool isRunning = true;

  struct winsize size;

  while (isRunning)
  {
    td_clearTTY();

    //private struct to get the tty size, don't want to put a new one
    //on the stack every function invocation
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);

    td_drawTicker(&display, &data, size.ws_col);

    p_runPrompt(&prompt, &data);
  }

  dt_DelDataTape(&data);
  p_DelPrompt(&prompt);

  return 0;
}
