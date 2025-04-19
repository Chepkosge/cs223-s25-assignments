#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>

struct account {
  float balance;
  pthread_mutex_t lock;
  pthread_cond_t give;//added
};

struct thread_data {
  struct account* fromAccount;
  struct account* toAccount;
  float amount;
};

void *Transfer(void *args){
  struct thread_data* data = (struct thread_data*) args;
  struct account* fromAcct = data->fromAccount; 
  struct account* toAcct = data->toAccount; 
  float amt = data->amount;

  for (int i = 0; i < 1000; i++) {
      pthread_mutex_lock(&(fromAcct->lock));
      while(fromAcct->balance < amt){
        pthread_cond_wait(&(fromAcct->give), &(fromAcct->lock));
      }
      fromAcct->balance -= amt;
      pthread_mutex_unlock(&(fromAcct->lock));

      pthread_mutex_lock(&(toAcct->lock));
      toAcct->balance += amt;
      pthread_cond_signal(&(toAcct->give));
      pthread_mutex_unlock(&(toAcct->lock));
  }
  return NULL;
}

int main() {
  struct account A, B;
  A.balance = 5000;
  B.balance = 5000;
  pthread_mutex_init(&(A.lock), NULL);
  pthread_mutex_init(&(B.lock), NULL);
  pthread_cond_init(&(A.give), NULL);
  pthread_cond_init(&(B.give), NULL);
  printf("Starting balance A: %.2f\n", A.balance);
  printf("Starting balance B: %.2f\n", B.balance);

  struct thread_data dataA;
  dataA.fromAccount = &B;
  dataA.toAccount = &A;
  dataA.amount = 1000;


  struct thread_data dataB;
  dataB.fromAccount = &A;
  dataB.toAccount = &B;
  dataB.amount = 1000;

  pthread_t thread1, thread2;
  pthread_create(&thread1, NULL, Transfer, &dataA);
  pthread_create(&thread2, NULL, Transfer, &dataB);

  pthread_join(thread1, NULL);
  pthread_join(thread2, NULL);

  // Should be the same because we transfer the same amount between both
  printf("Ending balance A: %.2f\n", A.balance);
  printf("Ending balance B: %.2f\n", B.balance);

  pthread_mutex_destroy(&A.lock);
  pthread_mutex_destroy(&B.lock);
  pthread_cond_destroy(&A.give);
  pthread_cond_destroy(&B.give);
  return 0;
}

