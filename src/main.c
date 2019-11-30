#include <stdio.h>
#include <stdbool.h>
//For getting the terminal width
#include <sys/ioctl.h>
#include <unistd.h>

#include "tickerDisplay.h"
#include "dataTape.h"
#include "prompt.h"
#include "settings.h"

int main(int argc, char **argv)
{
  Settings settings;
  s_Settings(&settings);
  s_ParseArgs(&settings, argc, argv);

  if (settings.mIsError)
  {
    return 1;
  }

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

    ioctl(STDOUT_FILENO, TIOCGWINSZ, &size);
    td_drawTicker(&display, &data, size.ws_col);

    p_printOutputList(&prompt, &data);
    p_getPromptInput(&prompt, &settings);
    p_parseInput(&prompt, &data);

  }

  dt_DelDataTape(&data);
  p_DelPrompt(&prompt);
  s_DelSettings(&settings);

  return 0;
}
