#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define T_SIZE 15
#define ARG_SIZE 80

int storeCommand(char *);
int tokennize(char *, char, char *b[]);
void checkAmpersand(int *, char *);
int main(int argc, char *argv[])
{
  pid_t pid;
  char *allArgs;
  char *args[ARG_SIZE/2];
  char *token;
  int dontWait = 0;
  const char *delim = " ";
  allArgs = (char *)malloc(ARG_SIZE*sizeof(char));
  token  = (char *)malloc(T_SIZE*sizeof(char));

  while(1)
    {
      int i = 0;
      printf("Shell> ");
      gets(allArgs);
      storeCommand(allArgs);
      //Split the command line arguments
      /*token = strtok(allArgs,delim);
      while(token != NULL)
	{
	  args[i] = (char *)malloc(25*sizeof(char));
	  args[i] = token;
	  token = strtok(NULL,delim);
	  i++;
	  }
	  args[i] = NULL;*/
      //ToDo - Release unused memory in args array
      i = tokennize(allArgs,' ',args); //Returns the index of last element + 1
      args[i] = NULL;
      checkAmpersand(&dontWait,args[--i]); //Sends the last argument to the function

      

      pid = fork();

      if(pid < 0)
	fprintf(stderr, "Could not create a child process");

      else if(pid == 0)
	{ //Code that will be executed by the child process
	  // printf("%s\n",args[0]);
	  for(int m = 0;m<i;m++)
	    printf("%s\n",args[m]);
	  execvp(args[0],args);
	  exit(1);
	}
      else 
	{
	  if(!dontWait)
	    {
	      wait(NULL);
	    }
	  else
	    continue;
	}
	
    }
  return 0;
}

int storeCommand(char *command)
{
  FILE *fp = fopen("commands.txt","a+");
  fprintf(fp,"%s\n",command);
  fclose(fp);
}

void checkAmpersand(int *dontWait, char *lastArg)
{
  //Checks the last character in the last argument for ampersand
  int lastIndex = strlen(lastArg); //Index of the terminating char
  if(lastArg[--lastIndex] == "&")
    {
      printf("%c\n",*(lastArg+lastIndex));
      *dontWait = 1;
    }
}

int tokennize(char *input,char delim, char *output[])
{
  int i = 0,m = 0,n = 0,delimFound = 0,previousDelim = 0; 
  char *temp;
  /*input = (char *)malloc((strlen(text)+2)*sizeof(char));
  input = text;
  input[strlen(text)+1] = '*'; //Delim is added as the last character to allow for tokennization
  input[strlen(text)+2] = '\0';*/
  //printf("%s\n",input);
  char *start = input;
  for(;i < strlen(input);i++)
    {
      if(*(input+i) == delim)
	{
	  delimFound = 1;
	  temp = (char *)malloc(i*sizeof(char));
	  temp = start;
	  temp[i] = '\0';
	  output[m] = temp;
	  start = input+1+i;
	  previousDelim = i+1; //The index just after the previous delim index
	  m++;
	}
    }
  if(delimFound)
    {
      temp = (char *)malloc((i-previousDelim)*sizeof(char));
      temp = input+previousDelim;
      output[m] = temp;
      return ++m;
    }
  else
    {
      output[0] = input;
      return 1;
    }
  }
