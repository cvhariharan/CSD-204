#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<stdlib.h>
#include<string.h>

#define T_SIZE 15
#define ARG_SIZE 80
#define MAX_HISTORY 100

int storeCommand(char *);
int tokennize(char *, char, char **);
void checkAmpersand(int *, char *);
int loadLatestCommands(char **);//t,char *);
//t checkInternalCommands(char *,char *)

int main(int argc, char *argv[])
{
  pid_t pid;
  char *allArgs;
  char *args[ARG_SIZE/2],*comms[MAX_HISTORY];
  char *token;
  int dontWait = 0;
  const char *delim = " ";
  allArgs = (char *)malloc(ARG_SIZE*sizeof(char));
  while(1)
    {
      int i = 0;
      printf("Hari's Shell> ");
      gets(allArgs);
      storeCommand(allArgs);
      //Split the command line arguments
      i = tokennize(allArgs,' ',args); //Returns the index of last element + 1
      args[i] = NULL;
      checkAmpersand(&dontWait,args[--i]); //Sends the last argument to the function
      int m = loadLatestCommands(comms);
      /*if(!checkInternalCommands(allArgs,token))
	{
	  allArgs = token;
	  }*/
      if(strcmp(args[0],"history") == 0)
	{
	  for(int p = m-2; p >= 0; p--) //If p = --m then history command will also be printed
	    {
	      if((m-p) == 10)
		break;
	      else
		printf("%s", comms[p]);
    
	    }
	}
      else if(strcmp(args[0], "exit") == 0)
	exit(0);

      else if(strcmp(args[0], "!!") == 0)
	{
	  printf("Not yet implemented.\n");
	}
      
      else
	{
	  pid = fork();

	  if(pid < 0)
	    fprintf(stderr, "Could not create a child process");

	  else if(pid == 0)
	    { //Code that will be executed by the child process
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
		  continue;
		}
	    }
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

int loadLatestCommands(char *comms[])//t single, char *com)
{
  FILE *fp = fopen("commands.txt","r");
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
  //com = (char *)malloc(n*sizeof(char));
  /**/
  /*
   if(comms[num] != NULL)
	com = comms[num];
   else
	printf("No command found at that index.");
    }
  else
    {
      com = comms[--i];
      }*/
  fclose(fp);
  return i;
}

void checkAmpersand(int *dontWait, char *lastArg)
{
  //Checks the last character in the last argument for ampersand
  int lastIndex = strlen(lastArg);
  //printf("%d %c\n",lastIndex,lastArg[--lastIndex]);
  if(lastArg[--lastIndex] == '&')
    {
      *dontWait = 1;
    }
}

int tokennize(char *input,char delim, char *output[])
{
  int i = 0,m = 0,n = 0,delimFound = 0,previousDelim = 0; 
  char *temp;
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

/*int checkInternalCommands(char *arg, char *toExec)
{
  char *args[ARG_SIZE];
  int i = tokennize(arg,' ',args);
  toExec = (char *)malloc(T_SIZE*sizeof(char));
  if(strcmp(args[0],"history") == 0)
    {
      showLatestCommands(10,0,NULL);
      toExec = NULL;
      return 1;
    }
  if(args[0] == '!')
    {
      if(args[1] == '!')
	showLatestCommands(0,2,toExec);
      else
	{
	  int i = atoi(args+1);
	  showLatestCommands(i,1,toExec);
	}
    }
  return 0;
  }*/
