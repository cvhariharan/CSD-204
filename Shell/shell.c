#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

int storeCommand(char *);
int tokennize(char *, char, char *b[]);
void checkAmpersand(int *, char *);
int main(int argc, char *argv[])
{
  pid_t pid;
  char *allArgs;
  char *args[40];
  char *token;
  int dontWait = 0;
  const char *delim = " ";
  allArgs = (char *)malloc(80*sizeof(char));
  token  = (char *)malloc(15*sizeof(char));

  while(1)
    {
      int i = 0;
      printf(" shell> ");
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
      /*for(int m=0; m<i; m++)
      printf("%s\n",args[m]);*/
      checkAmpersand(&dontWait,args[--i]); //Sends the last argument to the function
      
      pid = fork();

      if(pid < 0)
	fprintf(stderr, "Could not create a child process");

      else if(pid == 0)
	{ //Code that will be executed by the child process
	  printf("%s\n",args[0]);
	  execvp(args[0],args);
	  exit(1);
	}
      else if(pid > 0) 
	{
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
    *dontWait = 1;
}

int tokennize(char *input,char delim, char *output[])
{
  int i = 0,m = 0,n = 0; 
  char *temp;
  char *start = input;
  for(;i < strlen(input);i++)
    {
      if(*(input+i) == delim)
	{
	  temp = (char *)malloc(i*sizeof(char));
	  temp = start;
	  temp[i] = '\0';
	  output[m] = temp;
	  start = input+1+i;
	  m++;
	  /* temp[n] = *(input+i);
	     n++;*/
	}
      /*else
	{
	  temp[n] = '\0';
	  
	  output[m] = (char *)malloc(25*sizeof(char));
	  output[m] = temp;
	  printf("%s\n",temp);
	  m++;
	  n=0;
	  }*/
      
    }
  return m;
  }
