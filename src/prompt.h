#ifndef PROMPT
#define PROMPT

#include <stdlib.h>
#include <stdio.h>

#include "dataTape.h"

typedef struct Command {
  unsigned char *mCommand;
  unsigned int mStrLen;
} Command;

typedef struct Prompt{
  Command *mHistory;
  int mHistoryLength;
  int mHeadPosition;
  char *mInputBuff;
} Prompt;

void p_Prompt(Prompt *prompt);
void p_DelPrompt(Prompt *prompt);

void p_runPrompt(Prompt *prompt, DataTape *data);

#endif
