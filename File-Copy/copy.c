/* 
   Name: C.V.Hariharan
   ID: 1610110147
*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <fcntl.h>

int tokennize(char *, char, char **);

size_t size = 0;

int main(int argc, char *argv[])
{
  int fd[2];
  pid_t cpid;

  if(argc < 2)
    printf("Insufficient Arguments. Need src and dest.");
  else
    {
      pipe(fd); /* create pipe */

      FILE *fp = fopen(argv[1],"rb");
      fseek(fp,0,SEEK_END);
      size = ftell(fp);
      fseek(fp,0,SEEK_SET);
      fclose(fp);
      if(size > 0)
	{
	 cpid = fork();
	 if(cpid == -1)
	   {
	     perror("fork");
	     exit(1);
	   }
	 if(cpid == 0)
	   {
	     /*Close the write end*/
	     close(fd[1]);
	     int dest = open(argv[2], O_RDONLY);
	     if(dest == -1)
	       {
		 dest = open(argv[2], O_CREAT | O_WRONLY,766);
		 char *recv_content = malloc((size)*sizeof(char));
		 read(fd[0],recv_content,size);
		 write(dest,recv_content,size);
	       }
	     else
	       {
		 printf("A file with the same name exists. Do you want to overwrite? y/n");
		 char c;
		 scanf("%c", &c);
		 switch(c)
		   {
		   case 'Y':
		   case 'y': dest = open(argv[2], O_CREAT | O_WRONLY,766);
		     char *recv_content = malloc((size)*sizeof(char));
		     read(fd[0],recv_content,size);
		     write(dest,recv_content,size);
		     break;
		   case 'N':
		   case 'n': exit(0);
		   }
	       }
	     close(dest);
	     
	     //fprintf(dest,"%s",recv_content);

	    }
	 else
	   {
	    printf("Size: %d bytes\n", size);
	    close(fd[0]);
	    int src = open(argv[1], O_RDONLY);
	    if(src == -1)
	      {
		printf("No such file exists!");
		exit(0);
	      }
	    char *file_content;
	    file_content = (char *)malloc((size)*sizeof(char));
	    read(src,file_content, size);
	    // printf("%s\n", file_content);
	 
	    
	    write(fd[1], file_content, size);
	    close(src);
	    wait(NULL);
	   }
	}
     
    }
  
  return 0;
}

