/* 
Written By - C.V.Hariharan
Date - 07/02/2018
*/

#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define T_SIZE 15
#define ARG_SIZE 80
#define MAX_HISTORY 200

int storeCommand(char *);
int tokennize(char *, char, char **);
void checkAmpersand(int *, char *);
int loadLatestCommands(char **);
int executeCommand(char *);

int nosComs = 0; //Number of commands stored in the file
char *comms[MAX_HISTORY]; //Commands stored in the file

int main(int argc, char *argv[])
{
  
  char *allArgs;
  int run = 1;
  allArgs = (char *)malloc(ARG_SIZE*sizeof(char));
  while(run)
    {
      int i = 0;
      printf("Hari's Shell> ");
      gets(allArgs);
      nosComs = loadLatestCommands(comms);
      run = executeCommand(allArgs);
    }
  return 0;
}

int executeCommand(char *command) 
{
  pid_t pid;
  int i=0;
  char *args[ARG_SIZE/2];
  char *token;
  int dontWait = 0;
  const char *delim = " ";
  char *allArgs;
  allArgs = (char *)malloc(strlen(command));
  
  strcpy(allArgs, command); //To prevent tokennize function from breaking the string
  if(strlen(command) > 1)
    {
      storeCommand(command); //Store only non-empty commands
      comms[nosComs] = allArgs; //Add the command to the commands array (missing the \n)
      nosComs++;
    }
  if(allArgs[strlen(allArgs)-1] == '\n')
    {
      allArgs[strlen(allArgs)-1] = '\0'; //Remove the \n character when reading from the file.
    }
  //printf("Command: %s\n", allArgs);
  i = tokennize(allArgs,' ',args);//Returns the index of last element + 1
  args[i] = NULL;
  checkAmpersand(&dontWait,args[--i]); //Sends the last argument to the function
  if(strcmp(args[0],"history") == 0)
    {
      for(int p = nosComs-1; p >= 0; p--) //If p = --nosComs then history command will also be printed
	{
	  if((nosComs-p) == 11)
	    break;
	  else
	    printf("%d. %s\n", p, comms[p]);
    	}
    }
  else if(strcmp(args[0], "exit") == 0)
    return 0;
      
  else if(args[0][0] == '!')
    {
      if(args[0][1] == '!')
	{
	  if(nosComs >= 2)
	    executeCommand(comms[nosComs-2]);//nosComs-1 has !! and this leads to an infinite recursion
	  else
	    printf("No commands found in the history!\n");
	}

      else
	{
	  char *num[2];
	  tokennize(args[0],'!',num);
	  int index = atoi(num[1]);
	  if(comms[index] != NULL)
	    {
	      executeCommand(comms[index]);
	    }
	  else
	    {
	      printf("No such command found!\n");
	    }
	}
    }
      
  else
    {
      pid = fork();

      if(pid < 0)
	fprintf(stderr, "Could not create a child process");

      else if(pid == 0)
	{
	  //Code that will be executed by the child process
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
	    {
	      dontWait = 0; //Reset for the next child
	      return 1;
	    }
	}
    }
  return 1;
}

int storeCommand(char *command)
{
  FILE *fp = fopen("commands.txt","a+");
  fprintf(fp,"%s\n",command);
  fclose(fp);
}

int loadLatestCommands(char *comms[])
{
  FILE *fp;
  if(fp = fopen("commands.txt","r"))
    {
      ssize_t m;
      char *line = NULL;
      size_t n = 0;
      int i = 0,p = 0;
      while((m = getline(&line,&n,fp))!= -1)
	{
	  comms[i] = (char *)malloc(n*sizeof(char));
	  comms[i] = line;
	  line = NULL;
	  i++;
	}
      fclose(fp);

      return i;
    }
  return 0;
}

void checkAmpersand(int *dontWait, char *lastArg)
{
  //Checks the last character in the last argument for ampersand
  int lastIndex = strlen(lastArg);
  if(lastArg[--lastIndex] == '&')
    {
      *dontWait = 1;
    }
}

int tokennize(char *input,char delim, char *output[])
{
  //Tokennizes the input using the delim and stores the tokens in output
  int i = 0,m = 0,n = 0,delimFound = 0,previousDelim = 0; 
  char *temp;
  char *start = input;
  for(;i < strlen(input);i++)
    {
      if(*(input+i) == delim)
	{
	  delimFound = 1;
	  //Creates a pointer whose length is the length of the token and points it to the token in the input
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

