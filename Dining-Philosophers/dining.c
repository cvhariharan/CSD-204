#include<stdio.h>
#include<pthread.h>
#include<sys/types.h>
#include<unistd.h>
#include<fcntl.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define THINKING 0
#define EATING 1
#define HUNGRY 2

void check(int);
void *philosopher(void *);
int global_pid = 0;
int *forks; //1 if used for each fork
pthread_mutex_t mlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t rlock = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t tlock = PTHREAD_MUTEX_INITIALIZER; 
pthread_mutex_t *lock;
pthread_mutex_t flock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t *cond;
int n = 0; //Number of philosophers
int *eaten;
int *states;
int flag = 1;
void main()
{
  while(1)
    {
      int i,j;
      char c;
      if(n == 0)
	scanf("%d",&n);
      pthread_t philosophers[n];
      forks = (int *)malloc(n*sizeof(int));
      states = (int *)malloc(n*sizeof(int));
      cond = (pthread_cond_t *)malloc(n*sizeof(pthread_cond_t));
      lock = (pthread_mutex_t *)malloc(n*sizeof(pthread_mutex_t));
      eaten = (int *)malloc(n*sizeof(int));
      for(i=0;i<n;i++)
	{
	  states[i] =  THINKING;
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
      printf("Do you want to continue?Y/n: ");
      scanf(" %c", &c);
      if(c == 'Y' || c == 'y')
	{
	  global_pid = 0;
	  continue;
	}
      else
	exit(0);
    }
}

void pickup_forks(int philosopher_id)
{
  pthread_mutex_lock(&mlock);
  states[philosopher_id] = HUNGRY;
  check(philosopher_id);
  while(states[philosopher_id] != EATING)
    pthread_cond_wait(&cond[philosopher_id],&mlock);
  pthread_mutex_unlock(&mlock);
  
}

void return_forks(int p_id)
{
  pthread_mutex_lock(&rlock);
  states[p_id] = THINKING;
  check((p_id+1)%n);
  check((p_id+n-1)%n);
  pthread_mutex_unlock(&rlock);
  //pthread_mutex_unlock(&lock[small_fork]);  
  
}

void *philosopher(void *p)
{
  pthread_mutex_lock(&tlock);
  int p_id = global_pid++;
  pthread_mutex_unlock(&tlock);
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
	  flag *= eaten[i];
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

void check(int p_id)
{
  pthread_mutex_lock(&flock);
  if(states[(p_id+1)%n] != EATING && states[(p_id+n-1)%n] != EATING && states[p_id] == HUNGRY)
    {
      states[p_id] = EATING;
      pthread_cond_signal(&cond[p_id]);
    }
  pthread_mutex_unlock(&flock);
}
