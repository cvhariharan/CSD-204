#include<stdio.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>

typedef struct
{
  int number;
  int priority;
}Frame;

void retrieve(Frame *, int *, int);
int hit(Frame *, int);
int optimal(Frame *, int *, int);



int fn = 3;
int MAX_PAGES = 20;
int hit_counter = 0;
int miss_counter = 0;
int next_frame = 0;

void main()
{
  int i = 0;
  Frame *frames;
  int pages[] = {7,0,1,2,0,3,0,4,2,3,0,3,0,3,2,1,2,0,1,7,0,1}; //{1,4,4,3,8,10,14,19,4,6,8,15,16,2,15,19,0,2,9,3};
  frames = (Frame *)malloc(fn*sizeof(Frame *));
  MAX_PAGES = sizeof(pages)/sizeof(int);
  for(i=0;i<fn;i++)
    frames[i].number = MAX_PAGES+1;
  
  for(i=0;i<MAX_PAGES;i++)
    {
      retrieve(frames,pages,i);
    }
  for(i=0;i<fn;i++)
    {
      printf("%d\n",frames[i].number);
      
    }
  printf("Hits: %d, Misses: %d\n",hit_counter,miss_counter);
}

void retrieve(Frame frames[], int pages[],int index)
{
  if(hit(frames,pages[index]))
    {
      hit_counter++;
      printf("Hit %d\n",pages[index]);
    }
  else
    {
      miss_counter++;
      if(next_frame < fn)
	{
	  frames[next_frame].number = pages[index];
	  printf("Victimq: %d\n",next_frame);
	  next_frame++;
	}
      else
	{
	  int victim = optimal(frames,pages,index);
	  printf("Victim: %d\n",victim);
	  frames[victim].number = pages[index];
	}
     
    }
    
}


int hit(Frame frames[], int p)
{
  //int fn = sizeof(frames)/sizeof(int),i;
  int i;
  for(i=0;i<fn;i++)
    {
      if(frames[i].number == p)
	return 1;
    }
  return 0;
}

int optimal(Frame frames[], int pages[],int index)
{
  //int fn = sizeof(frames)/sizeof(int),i,j;
  int pn = MAX_PAGES,i,j;
  int greatest=0,victim=0;
  for(i=0;i<fn;i++)
    {
      for(j=index;j<pn;j++)
	{
	  if(frames[i].number == pages[j])
	    {
	      if(j > greatest)
		{
		  greatest = j;
		  victim = i;
		}
	      break;
	    }
	}
      //No hit for the page
      if(j == pn)
	{
	  victim = i;
	  break;
	}
    }
  return victim;
}

