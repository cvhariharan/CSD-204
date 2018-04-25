
#define OPTIMAL 0
#define L_R_U 1
#define F_I_F_O 2

typedef struct
{
  int number;
  int priority;
}Frame;

int optimal(Frame *, int *, int);
int LRU(Frame *, int *, int);
int FIFO();
