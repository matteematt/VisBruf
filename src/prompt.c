#include "prompt.h"

//Private functions
static void parseInputCommand(DataTape *data);
static void pushOutputIndex(Prompt *prompt, DataTape *data);
static void jumpForward(Prompt *prompt, DataTape *data);
static void jumpBackwards(Prompt *prompt, DataTape *data);
static void parseVisBrufCommand(Prompt *prompt, DataTape *data, Settings *settings,
    TickerDisplay *displayTicker);

//Linux kernel cannot accept more than 4094 input from stdin
static const int INPUT_BUF_LEN = 4094;

//Constructor
void p_Prompt(Prompt *prompt)
{
  prompt->mHeadPosition = 0;
  prompt->mHistoryLength = 16;
  prompt->mHistory = malloc(sizeof(Command) * prompt->mHistoryLength);
  //Room for INPUT_BUF_LEN char input and null byte
  prompt->mInputBuff = calloc(INPUT_BUF_LEN + 1, sizeof(char));
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
//If a user has requsted to load a file read that into input instead
void p_getPromptInput(Prompt *prompt, Settings *settings)
{
  if (settings->mReadFile == 0)
  {
    printf("@ ");

    //Try and get input
    if (fgets(prompt->mInputBuff, INPUT_BUF_LEN, stdin))
    {
      //Input is parsed after the the screen is refreshed
    }
    else
    {
      printf("Error getting input from user, exiting\n");
    }
  }
  else
  {
    //Read in requested file into the buffer
    FILE *file = fopen(settings->mReadFile, "rb");
    //Put the file pointer to the end of the file
    fseek(file, 0, SEEK_END);
    //Get the position of the file pointer (hense getting the filesize)
    long fileSize = ftell(file);
    //rewind the file pointer so we can start reading from the file
    fseek(file, 0, SEEK_SET);

    prompt->mInputBuff = realloc(prompt->mInputBuff, sizeof(char) * (fileSize + 1));
    fread(prompt->mInputBuff, 1, fileSize, file);
    fclose(file);

    //Add null byte
    prompt->mInputBuff[fileSize] = '\0';

    //Read in file so clear filename so we show the user the prompt next time
    free(settings->mReadFile);
    settings->mReadFile = 0;
  }
}

void p_parseInput(Prompt *prompt, DataTape *data, Settings *settings,
    TickerDisplay *displayTicker)
{
  for (
      prompt->mInputIndex = 0;
      prompt->mInputBuff[prompt->mInputIndex] != '\0';
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
      case '@':
        //If not in simple mode this is the start of a command, so process it as such
        //No more brainfuck commands are parse after a visbruf command
        if (!settings->mIsSimpleMode)
        {
          parseVisBrufCommand(prompt, data, settings, displayTicker);
          return;
        }
        break;
      default:
        //do nothing
        break;
    }
  }
}

//Implement the ] brainfuck command, jump after the matching [ if the current data
//is non-zero
static void jumpBackwards(Prompt *prompt, DataTape *data)
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
static void jumpForward(Prompt *prompt, DataTape *data)
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
static inline void parseInputCommand(DataTape *data)
{
  printf("\n# ");
  char c = getchar();
  data->mData[data->mDataIndex] = c;
}

//Pushes index onto the output array for the '.' brainfuck command
static inline void pushOutputIndex(Prompt *prompt, DataTape *data)
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

//Handles commands built into the promot that are not part of brainfuck
static void parseVisBrufCommand(Prompt *prompt, DataTape *data, Settings *settings,
    TickerDisplay *displayTicker)
{
  //Need a buffer to do string comparisons
  const int MAX_COMMAND_LEN = 11;
  char *commandBuffer = malloc(sizeof(char) * (MAX_COMMAND_LEN + 1));

  for (
      prompt->mInputIndex = 0;
      prompt->mInputBuff[prompt->mInputIndex] != '\0';
      prompt->mInputIndex++
      )
  {
    strncpy(commandBuffer, &prompt->mInputBuff[prompt->mInputIndex], MAX_COMMAND_LEN);
    commandBuffer[MAX_COMMAND_LEN] = '\0';

    if (strncmp(commandBuffer, "@reset", 6) == 0)
    {
      data->mDataIndex = 0;
      memset(data->mData, 0x00, data->mDataLen);
      prompt->mInputIndex += 5;
    }
    else if (strncmp(commandBuffer, "@quit", 5) == 0)
    {
      settings->mIsRunning = false;
      free(commandBuffer);
      return;
    }
    else if (strncmp(commandBuffer, "@naddress", 9) == 0)
    {
      int newOffset;
      if (sscanf(commandBuffer, "@naddress%d", &newOffset) == 1)
      {
        displayTicker->mScrollDepth += newOffset;
      }
      else
      {
        displayTicker->mScrollDepth++;
      }
    }
    else if (strncmp(commandBuffer, "@paddress", 9) == 0)
    {
      int newOffset;
      if (sscanf(commandBuffer, "@paddress%d", &newOffset) == 1)
      {
        displayTicker->mScrollDepth -= newOffset;
      }
      else
      {
        displayTicker->mScrollDepth--;
      }
      if (displayTicker->mScrollDepth < 0)
      {
        displayTicker->mScrollDepth = 0;
      }
    }
    else if (strncmp(commandBuffer, "@memrows", 8) == 0)
    {
      int newSize;
      if (sscanf(commandBuffer, "@memrows%d", &newSize) == 1)
      {
        displayTicker->mTickerHeight = newSize > -1 ? newSize : 0;
      }
      else
      {
        printf("Usage @memrowsDD where DD is two digits of size\n");
      }
    }
  }

  free(commandBuffer);
}
