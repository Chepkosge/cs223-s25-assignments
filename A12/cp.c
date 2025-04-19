#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <assert.h>
#include <unistd.h>

#define N 10

//struct for thread args
struct t_data{
  int* items;
  int* num_items;
  int* in;
  int*out;
  pthread_cond_t* full;
  pthread_cond_t* empty;
  pthread_mutex_t* mux;
};

int produce_item() {
    return rand() % 10;
}

void consume_item(int item) {
    printf("Received item: %d\n", item);
}

//producer thread
void* produce(void* t_args){
  struct t_data* data = (struct t_data*)t_args;
  int* in = data->in;
  int* num_items = data->num_items;
  int* items = data->items;
  pthread_cond_t* full = data->full;
  pthread_cond_t* empty = data->empty;
  pthread_mutex_t* mux = data->mux;
  while(1){
    sleep(1);
    pthread_mutex_lock(mux);
    while(*num_items == N){
      pthread_cond_wait(full, mux);
    }
    items[*in] = produce_item();
    *in = (*in+1) % N;
    *num_items += 1;
    pthread_cond_signal(empty);
    pthread_mutex_unlock(mux);
  }
}
//consumer thread
void* consume(void* t_args){
  struct t_data* data = (struct t_data*)t_args;
  int* out = data->out;
  int* num_items = data->num_items;
  int* items = data->items;
  pthread_cond_t* full = data->full;
  pthread_cond_t* empty = data->empty;
  pthread_mutex_t* mux = data->mux;
  while(1){
    pthread_mutex_lock(mux);
    while(*num_items < 1){
      pthread_cond_wait(empty, mux);
    }
    consume_item(items[*out]);
    *out = (*out+1) % N;
    *num_items -= 1;
    pthread_cond_signal(full);
    pthread_mutex_unlock(mux);
  }
}

int main() {
  // todo
int items[N];
int num_items = 0, in = 0,out = 0;
pthread_mutex_t mux = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t full = PTHREAD_COND_INITIALIZER;
pthread_cond_t empty = PTHREAD_COND_INITIALIZER;
pthread_t* thread_array = malloc(sizeof(pthread_t) * 2);
struct t_data* t_args = malloc(sizeof(struct t_data) * 2);
//Intitialize t_args
for(int i = 0; i < 2; i++){
  t_args[i].in = &in;
  t_args[i].out = &out;
  t_args[i].items = items;
  t_args[i].num_items = &num_items;
  t_args[i].full = &full;
  t_args[i].empty = &empty;
  t_args[i].mux = &mux;
}

pthread_create(&thread_array[0], NULL, produce, &t_args[0]);
pthread_create(&thread_array[1], NULL, consume, &t_args[1]);

for(int i = 0; i < 2; i++){
  pthread_join(thread_array[i], NULL);
}
  return 0;
}

