#include "prompt.h"

//Private functions
void parseInputCommand(DataTape *data);
void pushOutputIndex(Prompt *prompt, DataTape *data);
void jumpForward(Prompt *prompt, DataTape *data);
void jumpBackwards(Prompt *prompt, DataTape *data);

//Constructor
void p_Prompt(Prompt *prompt)
{
  prompt->mHeadPosition = 0;
  prompt->mHistoryLength = 16;
  prompt->mHistory = malloc(sizeof(Command) * prompt->mHistoryLength);
  //Room for 128 char input and null byte
  prompt->mInputBuff = malloc(sizeof(char) * 129);
  prompt->mOutputListLen = 8;
  prompt->mOutputList = malloc(sizeof(char) * prompt->mOutputListLen);
  for (int i = 0; i < prompt->mOutputListLen; i++)
  {
    prompt->mOutputList[i] = '\0';
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
  for (
      prompt->mInputIndex = 0;
      prompt->mInputIndex <= 128 && prompt->mInputBuff[prompt->mInputIndex] != '\0'; 
      prompt->mInputIndex++)
  {
    switch (prompt->mInputBuff[prompt->mInputIndex])
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
      case '[':
        //Jump to the command after the matching ] if the current byte is 0
        jumpForward(prompt, data);
        break;
      case ']':
        //Jump to the command after the matching [ if the current byte is non-zero
        jumpBackwards(prompt, data);
        break;
      default:
        //do nothing
        break;
    }
  }
}

//Implement the ] brainfuck command, jump after the matching [ if the current data
//is non-zero
void jumpBackwards(Prompt *prompt, DataTape *data)
{
  if (data->mData[data->mDataIndex] == 0x00)
  {
    return;
  }

  //Move the index to the next command in the input buffer, but account for the [
  prompt->mInputIndex--;
  int matchingBracketIndex = 1;

  while (matchingBracketIndex > 0)
  {
    if (prompt->mInputBuff[prompt->mInputIndex] == ']')
    {
      matchingBracketIndex++;
    }
    if (prompt->mInputBuff[prompt->mInputIndex] == '[')
    {
      matchingBracketIndex--;
    }
    prompt->mInputIndex--;
  }
  //Initially it may seem we would have to move two forward, but as the data pointer
  //moves forward next process loop anyway we would skip a command if we moved forward two
  prompt->mInputIndex++;
}

//Implement the [ brainfuck command, jump after the matching ] if the current data
//is zero
void jumpForward(Prompt *prompt, DataTape *data)
{
  if (data->mData[data->mDataIndex] != 0x00)
  {
    return;
  }

  //Move the index to the next command in the input buffer, but account for the [
  prompt->mInputIndex++;
  int matchingBracketIndex = 1;

  while (matchingBracketIndex > 0)
  {
    if (prompt->mInputBuff[prompt->mInputIndex] == '[')
    {
      matchingBracketIndex++;
    }
    if (prompt->mInputBuff[prompt->mInputIndex] == ']')
    {
      matchingBracketIndex--;
    }
    prompt->mInputIndex++;
  }
  //Have to move one back because the data pointer moves forward automatically next go
  //and if we don't correct we would skip a command
  prompt->mInputIndex--;
}

//Print the output list pushed to from the last command, and reset as you go
//Filled from the brainfuck '.' command
void p_printOutputList(Prompt *prompt, DataTape *data)
{
  int i;
  for (i = 0; i < prompt->mOutputListLen && prompt->mOutputList[i] != '\0'; i++)
  {
    putchar(prompt->mOutputList[i]);
    prompt->mOutputList[i] = '\0';
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
    prompt->mOutputList = realloc(prompt->mOutputList, prompt->mOutputListLen * sizeof(char));
    for (int i = prompt->mOutputListIndex; i < prompt->mOutputListLen; i++)
    {
      prompt->mOutputList[i] = '\0';
    }
  }
  prompt->mOutputList[prompt->mOutputListIndex] = data->mData[data->mDataIndex];
}
