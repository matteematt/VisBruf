#ifndef PROMPT
#define PROMPT

#include <stdlib.h>
#include <stdio.h>

#include "dataTape.h"

typedef struct Command {
  unsigned char *mCommand;
  unsigned int mStrLen;
} Command;

/**
  * Dynamic history of commands so we can go back throughtr them
  * the current max length of this dynamic array, wioll double in size when full
  * the index of the current latests command
  * A string which is the current input command, before its saved to the command array
  * A list of ints which are indexes to put the chars after the prompt is run, -1 means dont print
  * The max length of this dynamic int array, will double in size when full
  * The current index that points to the most recently pushed output index
  */
typedef struct Prompt{
  Command *mHistory;
  int mHistoryLength;
  int mHeadPosition;
  char *mInputBuff;
  int *mOutputList;
  int mOutputListLen;
  int mOutputListIndex;
} Prompt;

void p_Prompt(Prompt *prompt);
void p_DelPrompt(Prompt *prompt);

void p_getPromptInput(Prompt *prompt);
void p_parseInput(Prompt *prompt, DataTape *data);
void p_printOutputList(Prompt *prompt, DataTape *data);

#endif
