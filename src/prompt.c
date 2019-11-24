#include "prompt.h"

//Private functions
void parseInputCommand(DataTape *data);
void pushOutputIndex(Prompt *prompt, DataTape *data);

//Constructor
void p_Prompt(Prompt *prompt)
{
  prompt->mHeadPosition = 0;
  prompt->mHistoryLength = 16;
  prompt->mHistory = malloc(sizeof(Command) * prompt->mHistoryLength);
  //Room for 128 char input and null byte
  prompt->mInputBuff = malloc(sizeof(char) * 129);
  prompt->mOutputListLen = 8;
  prompt->mOutputList = malloc(sizeof(int) * prompt->mOutputListLen);
  for (int i = 0; i < prompt->mOutputListLen; i++)
  {
    prompt->mOutputList[i] = -1;
  }
  prompt->mOutputListIndex = -1;
}

//Destructor
void p_DelPrompt(Prompt *prompt)
{
  for (int i = 0; i < prompt->mHistoryLength; i++)
  {
    free(prompt->mHistory[i].mCommand);
  }
  free(prompt->mHistory);
  free(prompt->mInputBuff);
  free(prompt->mOutputList);
}

//Reset output list and get user input into the buffer
void p_getPromptInput(Prompt *prompt)
{
  printf("@ ");

  //Try and get input
  if (fgets(prompt->mInputBuff, 128, stdin))
  {
    //Input is parsed after the the screen is refreshed
  }
  else
  {
    printf("Error getting input from user, exiting\n");
  }
}

void p_parseInput(Prompt *prompt, DataTape *data)
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
      case ',':
        //Get a byte of input from the user and save it in the tape
        parseInputCommand(data);
        break;
      case '.':
        //Get the byte currently output and print it to stdout
        pushOutputIndex(prompt, data);
        break;
      default:
        //do nothing
        break;
    }
  }
}

//Print the output list pushed to from the last command, and reset as you go
//Filled from the brainfuck '.' command
void p_printOutputList(Prompt *prompt, DataTape *data)
{
  int i;
  for (i = 0; i < prompt->mOutputListLen && prompt->mOutputList[i] != -1; i++)
  {
    putchar(data->mData[prompt->mOutputList[i]]);
    prompt->mOutputList[i] = -1;
  }
  if (i > 0)
  {
    printf("\n");
  }
  prompt->mOutputListIndex = -1;
}

//Gets data for the ',' brainfuck command
inline void parseInputCommand(DataTape *data)
{
  printf("\n# ");
  char c = getchar();
  data->mData[data->mDataIndex] = c;
}

//Pushes index onto the output array for the '.' brainfuck command
inline void pushOutputIndex(Prompt *prompt, DataTape *data)
{
  prompt->mOutputListIndex++;
  if (prompt->mOutputListIndex == prompt->mOutputListLen)
  {
    prompt->mOutputListLen *= 2;
    prompt->mOutputList = realloc(prompt->mOutputList, prompt->mOutputListLen);
  }
  prompt->mOutputList[prompt->mOutputListIndex] = data->mDataIndex;
}
