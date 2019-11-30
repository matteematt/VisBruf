#include "settings.h"

//Private functions
static void parseFilenameFlag(Settings *settings, int argIndx, int argc, char** argv);

void s_Settings(Settings *settings)
{
  settings->mReadFile = 0;
  settings->mIsError = false;
}

void s_DelSettings(Settings *settings)
{
  if (settings->mReadFile != 0)
  {
    free(settings->mReadFile);
    settings->mReadFile = 0;
  }
}

void s_ParseArgs(Settings *settings, int argc, char **argv)
{
  //Loop though all the input args (except the first which is the binary name)
  for (int i = 1; i < argc; i++)
  {
    if (strncmp(argv[i], "-f", 2) == 0) 
    {
      parseFilenameFlag(settings, i, argc, argv);
    }
  }
}

//Some input args come in pairs such as -f <filename>
static void parseFilenameFlag(Settings *settings, int argIndx, int argc, char** argv)
{
  if (argIndx == argc - 1) 
  {
    printf("ERROR: Expecting a filename after -f\n");
    settings->mIsError = true;
    return;
  }  

  if (access(argv[argIndx + 1], F_OK) != -1)
  {
    //File exists
    settings->mReadFile = malloc(sizeof(char) * (strlen(argv[argIndx + 1]) + 1));
    strcpy(settings->mReadFile, argv[argIndx + 1]);
  }
  else
  {
    printf("ERROR: Invalid filename \"%s\" supplied after argument -f\n",
        argv[argIndx + 1]);
    settings->mIsError = true;
  }
}
