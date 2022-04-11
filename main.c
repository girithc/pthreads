#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
 


int servers;
int customers;
int ticket = 0;
int *buffer;  
int id = 0;

pthread_mutex_t customer_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t customer_avail_cond = PTHREAD_COND_INITIALIZER;
int customer_avail = 0;
 
// +++++ pthread APIs: +++++ 
// pthread_mutex_t  lock1 = PTHREAD_MUTEX_INITIALIZER; 
// pthread_cond_t   cond1 = PTHREAD_COND_INITIALIZER; 
// pthread_cond_wait(&cond1, &lock1); 
// pthread_cond_signal(&cond1); 
// pthread_mutex_lock(&locl1);  

// pthread_mutex_unlock(&lock1);


typedef struct lunch{
  int customers;
  int servers;
  int ticket;
} lunch;

typedef struct threadC{
  int ticket;
  int id;
} threadC;

typedef struct threadS{
  int ticket;
  int id;
} threadS;

void lunch_init(struct lunch * lunch)
{
  lunch->customers = 0;
  lunch->servers = 0;
  lunch->ticket = 0;
}

void* lunch_wait_turn(void * args)
{

  struct lunch *this_lunch = (struct lunch*) args;
  //acquire lock
  (*this_lunch).ticket++;
  pthread_mutex_lock(&customer_lock);
  //customer available
  customer_avail = 1;
  pthread_cond_signal(&customer_avail_cond);
  pthread_mutex_unlock(&customer_lock);
  return NULL;  
}

void* lunch_wait_customer(void* args)
{
  struct lunch *this_lunch = (struct lunch*) args;
  
  pthread_mutex_lock(&customer_lock);
  while(customer_avail == 0)
  {
    pthread_cond_wait(&customer_avail_cond, &customer_lock);
  }
  pthread_mutex_unlock(&customer_lock);
  printf("Ticket No: %d\n", (*this_lunch).ticket);
  return NULL;
}

int lunch_get_ticket(struct lunch * lunch)
{
  return lunch->ticket;
}

 int main(int argc, char *argv[]) {
    if (argc != 3) {
		fprintf(stderr, "usage: %s <number of customers> <number of servers>\n", argv[0]);
		exit(1);
    }
	  customers = atoi(argv[1]);
    servers = atoi(argv[2]);
    buffer = (int *) malloc(servers * sizeof(int));

    struct lunch lunch;
    lunch_init(&lunch);

    //printf("%d \n", lunch.customers);
    //printf("%d, %d \n", customers, servers);
    //int rc;

    pthread_t pC;
    pthread_t pS;

    //create customer threads
    
    if(pthread_create(&pC, NULL,lunch_wait_turn, (void*)&lunch ) != 0)
    {
      perror("Error 1");
    }
    
    if(pthread_create(&pC, NULL,lunch_wait_customer, (void*)&lunch  ) != 0)
    {
      perror("Error 2");
    }
    

    pthread_exit(NULL);
    

    return 0;
}
