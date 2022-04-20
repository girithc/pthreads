/*
Name: Girith Choudhary
Class: CS301
Project: 3 Multithreading in C
The program simulates a scenario of a restaraunt with customers and servers. 
A mutex lock and two conditional variables are used to prevent race conditions 
and help in sychronization.
*/
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include "mytime.h"
 


int servers;
int customers;
int ticket = 0;
int serversNo;  
int customer_avail = 0;
int server_avail = 0;
int serving_ticket = 0;
pthread_mutex_t lunch_lock = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t customer_avail_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t server_avail_cond = PTHREAD_COND_INITIALIZER;

 
// +++++ pthread APIs: +++++ 
// pthread_mutex_t  lock1 = PTHREAD_MUTEX_INITIALIZER; 
// pthread_cond_t   cond1 = PTHREAD_COND_INITIALIZER; 
// pthread_cond_wait(&cond1, &lock1); 
// pthread_cond_signal(&cond1); 
// pthread_mutex_lock(&locl1);  

// pthread_mutex_unlock(&lock1);


typedef struct lunch{
  int ticket;
  int customerNo;
} lunch;

void Show_serving(int number)
{
  //"Now Serving" screen output
  printf("Serving <%d> \n", number);
}


void lunch_init()
{
  customers = 0;
  servers = 0;
}

void * lunch_get_ticket(void * args)
{
  //Debug
  printf("<%lu Customer> enter <lunch_get_ticket>\n", (long)pthread_self());

  // Param:args -> lunch
  struct lunch *this_lunch = (struct lunch*) args;

  //acquire mutex
  pthread_mutex_lock(&lunch_lock);

  //update this_lunch ticket field
  this_lunch->ticket = ticket + 1;
  ticket = ticket + 1;
  printf("<%lu Customer> get ticket <%d>\n",(long)pthread_self(), this_lunch->ticket);
  //leave mutex
  pthread_mutex_unlock(&lunch_lock);

  //Random Sleep using mytime()
  sleep(mytime(2,5));

  //Debug
  printf("<%lu Customer> leave <lunch_get_ticket>\n", (long)pthread_self());

  //exit thread
  pthread_exit(0);
}

void* lunch_wait_turn(void * args)
{
  
  // Param:args -> lunch
  struct lunch *this_lunch = (struct lunch*) args;

  //Debug
  printf("<%lu Customer> enter <lunch_wait_turn> with <%d>\n", (long)pthread_self(), this_lunch->ticket);

  //acquire mutex
  pthread_mutex_lock(&lunch_lock);

  //check if server is available and ticket is gtu to the lunch ticket
  while(server_avail == 0 && ticket < this_lunch->ticket)
  {
    //wait if conditions not satisfied
    pthread_cond_wait(&server_avail_cond, &lunch_lock);
  }

  serving_ticket = this_lunch->ticket;
  pthread_cond_signal(&customer_avail_cond);

  customer_avail --;

  //leave mutex
  pthread_mutex_unlock(&lunch_lock);

  //"Now Serving" screen output
  Show_serving(this_lunch->ticket);

  //Debug
  printf("<%lu Customer> leave <lunch_wait_turn>\n", (long)pthread_self());
  
  //exit pthread
  pthread_exit(0);
  
}

void* lunch_wait_customer(void* args)
{
  //Debug
  printf("<%lu Server> enter <lunch_wait_customer>\n", (long)pthread_self());

  // Param:args -> lunch
  struct lunch *this_lunch = (struct lunch*) args;

  //acquire  mutex
  pthread_mutex_lock(&lunch_lock);

  serving_ticket++;
  serversNo ++;

  //"Now Serving" screen output
  Show_serving(this_lunch->ticket);

  //check if server is available and ticket is gtu to the lunch ticket
  while(customer_avail == 0)
  {
    //wait if conditions not satisfied
    pthread_cond_wait(&customer_avail_cond, &lunch_lock);
  }

  serving_ticket = this_lunch->ticket;
  pthread_cond_signal(&server_avail_cond);
  serversNo--;

  //Debug
  printf("<%lu Server> service number <%d>\n", (long)pthread_self(), serving_ticket);


  //leave mutex
  pthread_mutex_unlock(&lunch_lock);

  //"Now Serving" screen output
  Show_serving(this_lunch->ticket);

  //Debug
  printf("<%lu Server> leave <lunch_wait_customer>\n", (long)pthread_self());
  
  //exit pthread
  pthread_exit(0);
}


 int main(int argc, char *argv[]) {

    lunch_init();
    if (argc != 3) {
		fprintf(stderr, "usage: %s <number of customers> <number of servers>\n", argv[0]);
		exit(1);
    }
	  customers = atoi(argv[1]);
    servers = atoi(argv[2]);
    customer_avail = customers;
    
    serversNo = servers;

    struct lunch lunch[customers];
    int serverIndex = 0;

    //printf("%d \n", lunch.customers);
    //printf("%d, %d \n", customers, servers);
    //int rc;

    pthread_t pC[customers];
    pthread_t pS[servers];

    //create customer threads
    for (int i = 0; i < customers; i++)
    {
      if(pthread_create(&pC[i], NULL,lunch_get_ticket, (void*)&lunch[i] ) != 0)
      {
        perror("Error 1");
      }

      if(pthread_create(&pC[i], NULL,lunch_wait_turn, (void*)&lunch[i] ) != 0)
      {
        perror("Error 2");
      }
      serverIndex = i % servers;
      if(pthread_create(&pS[serverIndex], NULL,lunch_wait_customer, (void*)&lunch[serverIndex] ) != 0)
      {
        perror("Error 3");
      }
      
    }

    for (int i = 0; i < customers; i++) {
      pthread_join(pC[i], NULL);

      if(i < servers)
      {
        pthread_join(pS[i], NULL);
      }
      
    }
    
    //pthread_exit(NULL);
    

    return 0;
}
