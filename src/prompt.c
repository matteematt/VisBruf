#include "prompt.h"

//Private functions
void parseInput(Prompt *prompt, DataTape *data);

void p_Prompt(Prompt *prompt)
{
  prompt->mHeadPosition = 0;
  prompt->mHistoryLength = 16;
  prompt->mHistory = malloc(sizeof(Command) * prompt->mHistoryLength);
  prompt->mInputBuff = malloc(sizeof(char) * 129);
}

void p_DelPrompt(Prompt *prompt)
{
  for (int i = 0; i < prompt->mHistoryLength; i++)
  {
    free(prompt->mHistory[i].mCommand);
  }
  free(prompt->mHistory);
  free(prompt->mInputBuff);
}

void p_runPrompt(Prompt *prompt, DataTape *data)
{
  printf("@ ");

  //Try and get input
  if (fgets(prompt->mInputBuff, 128, stdin))
  {
    parseInput(prompt, data);
  }
  else
  {
    printf("Error getting input from user, exiting\n");
  }
}

void parseInput(Prompt *prompt, DataTape *data)
{
  int i;
  for (i = 0; i <= 128 && prompt->mInputBuff[i] != '\0'; i++)
  {
    switch (prompt->mInputBuff[i])
    {
      case '+':
        //Increment currently selected byte
        data->mData[data->mDataIndex]++;
        break;
      case '-':
        //Decrement currently selected byte
        data->mData[data->mDataIndex]--;
        break;
      case '<':
        //Decrement the data pointer
        data->mDataIndex--;
        if (data->mDataIndex < 0)
        {
          data->mDataIndex = 0;
        }
        break;
      case '>':
        //Increment the data pointer
        data->mDataIndex++;
        if (data->mDataIndex == data->mDataLen)
        {
          dt_growTape(data);
        }
        break;
      default:
        //do nothing
        break;
    }
  }
}
