#ifndef SETTINGS
#define SETTINGS

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stdio.h>
#include <unistd.h>

/**
  * Holds current user settings about the prompt
  * Some are set through command line arguments
  */

/**
  * Pointer to a file to read in, else NULL
  */
typedef struct Settings 
{
  bool mIsRunning;
  bool mIsError;
  bool mIsSimpleMode;
  char *mReadFile;
} Settings;

//"Constructor/Destructor"
void s_Settings(Settings *settings);
void s_DelSettings(Settings *settings);

//Parse command line arguments and update settings
void s_ParseArgs(Settings *settings, int argc, char **argv);

#endif
