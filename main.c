#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
 


int servers;
int customers;
int ticket = 1;
int *buffer;  
 
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

void lunch_init(struct lunch * lunch)
{
  lunch->customers = 0;
  lunch->servers = 0;
  lunch->ticket = ticket;
}

void* lunch_wait_turn(struct lunch *lunch, int ticket)
{
  return NULL;
}

void* lunch_wait_customer(struct lunch *lunch)
{
  return NULL;
}

 int main(int argc, char *argv[]) {
    if (argc != 3) {
		fprintf(stderr, "usage: %s <number of customers> <number of servers>\n", argv[0]);
		exit(1);
    }
	  customers = atoi(argv[1]);
    servers = atoi(argv[2]);
    buffer = (int *) malloc(servers * sizeof(int));

    for (int i = 0; i < servers; i++)
      buffer[i] = 1; // start: all servers are available


    pthread_t p1;
    pthread_t p2;
    struct lunch lunch;
    lunch_init(&lunch);
    printf("%d \n", lunch.customers);

    //pthread_create(&p1, NULL, lunch_wait_turn, NULL);

    printf("%d, %d \n", customers, servers);


    return 0;
 }
