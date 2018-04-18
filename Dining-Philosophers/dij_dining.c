#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

void *philosopher(void *);
int global_pid = 0;
int *forks; //1 if used for each fork
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t flock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t *lock;
pthread_cond_t *cond;
int n; //Number of philosophers
int *eaten;
int flag = 1;
void main()
{
  while(1)
    {
      int i,j;
      char c;
      scanf("%d",&n);
      pthread_t philosophers[n];
      forks = (int *)malloc(n*sizeof(int));
      cond = (pthread_cond_t *)malloc(n*sizeof(pthread_cond_t));
      lock = (pthread_mutex_t *)malloc(n*sizeof(pthread_mutex_t));
      eaten = (int *)malloc(n*sizeof(int));
      for(i=0;i<n;i++)
	{
	  pthread_create(&philosophers[i],NULL,philosopher,NULL);
	  forks[i] = 0;
	  eaten[i] = 0;
	  pthread_mutex_init(&lock[i], NULL);
	  pthread_cond_init(&cond[i], NULL);
       
	}
      for(i=0;i<n;i++)
	{
	  pthread_join(philosophers[i], NULL);
	}

      for(i=0;i<n;i++)
	{
	  printf("Philosopher %d Time: %d\n",i,eaten[i]);
	}
      printf("Do you want to continue?Y/n\n");
      scanf(" %c",&c);
      if(c == 'Y' || c == 'y')
	{
	  global_pid = 0;
	  continue;
	}
      else if(c == 'N' || c == 'n')
	exit(0);
      else
	printf("Incorrect choice!\n");
    }
}

void pickup_forks(int philosopher_id)
{
  int fork1 = philosopher_id;
  int fork2 = (philosopher_id + n-1)%n;
  int small_fork = (fork1 < fork2)?fork1:fork2;
  int large_fork = (fork1 > fork2)?fork1:fork2;
  pthread_mutex_lock(&mlock);
  
  while(forks[small_fork] != 0)
    pthread_cond_wait(&cond[small_fork],&mlock);
  forks[small_fork] = 1;
  printf("Philosopher %d got fork %d\n",philosopher_id,small_fork);
  
  //pthread_mutex_lock(&lock[large_fork]);
  
  while(forks[large_fork] != 0)
    pthread_cond_wait(&cond[large_fork],&mlock);
  forks[large_fork] = 1;
  printf("Philosopher %d got fork %d\n",philosopher_id,large_fork);
  pthread_mutex_unlock(&mlock);
  // pthread_mutex_unlock(&lock[small_fork]);  
  

  
}

void return_forks(int philosopher_id)
{
  int fork1 = philosopher_id;
  int fork2 = (philosopher_id + n-1)%n;
  int small_fork = (fork1 < fork2)?fork1:fork2;
  int large_fork = (fork1 > fork2)?fork1:fork2;
  pthread_mutex_lock(&mlock);
  
  forks[small_fork] = 0;
  pthread_cond_signal(&cond[small_fork]);
  printf("Philosopher %d released fork %d\n",philosopher_id,small_fork);
  
  //pthread_mutex_lock(&lock[large_fork]);
  
  forks[large_fork] = 0;
  pthread_cond_signal(&cond[large_fork]);
  printf("Philosopher %d released fork %d\n",philosopher_id,large_fork);
  pthread_mutex_unlock(&mlock);
  //pthread_mutex_unlock(&lock[small_fork]);  
  
}

void *philosopher(void *p)
{
  pthread_mutex_lock(&flock);
  int p_id = global_pid;
  global_pid++;
  pthread_mutex_unlock(&flock);
  char c;
  //printf("Philosopher %d\n",*id);
   srand(time(NULL));
   while(1)
     {
       int r = (rand() % 3)+1;
      printf("Philosopher %d is thinking\n",p_id);
      sleep(r);
      pickup_forks(p_id);
      //pthread_mutex_lock(&mlock);
      printf("Philosopher %d is eating\n",p_id);
      eaten[p_id]+=r;
      
      for(int i=0;i<n;i++)
	{
	  if(eaten[i] == 0)
	    flag = 0;
	}
      
      //pthread_mutex_unlock(&mlock);
      return_forks(p_id);
      if(flag)
	{
	  return 0;
	}
      flag = 1;
     }
}
