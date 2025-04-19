#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <assert.h>

typedef enum { OUTLOOK, TOWN} location_t;
const char* location_string[2] = { "Outlook", "Town" };

struct bridge_t {
  int num_on_bridge[2];
  int waiting[2];
  location_t direction;
  pthread_mutex_t lock;
  pthread_cond_t cond[2];
} bridge; // global variable for bridge

struct thread_data {
  int id;
  int num_trips;
  location_t start_destination;
};

void enter_bridge(location_t dir) {
  pthread_mutex_lock(&bridge.lock);
  while (bridge.num_on_bridge[(dir + 1) % 2] > 0 ||
         (bridge.direction != dir && bridge.num_on_bridge[dir] == 0 && bridge.waiting[(dir + 1) % 2] > 0)) {
    bridge.waiting[dir]++;
    pthread_cond_wait(&bridge.cond[dir], &bridge.lock);
    bridge.waiting[dir]--;
  }
  bridge.direction = dir;
  bridge.num_on_bridge[dir]++;
  pthread_mutex_unlock(&bridge.lock);
}

void leave_bridge(location_t dir) {
  pthread_mutex_lock(&bridge.lock);
  bridge.num_on_bridge[dir]--;
  if (bridge.num_on_bridge[dir] == 0) {
    pthread_cond_broadcast(&bridge.cond[(dir + 1) % 2]);
  }
  pthread_mutex_unlock(&bridge.lock);
}

void *Move(void *args){
  struct thread_data* data = (struct thread_data*) args;
  int id = data->id;
  location_t destination = data->start_destination;

  for (int i = 0; i < data->num_trips; i++) {
    enter_bridge(destination);

    printf("Tourist %d takes their %d/%d trip towards %s\n", 
      id, i, data->num_trips, location_string[destination]);

    assert(bridge.num_on_bridge[(destination+1)%2] == 0);

    sleep(1);

    leave_bridge(destination);

    assert(bridge.num_on_bridge[destination] >= 0); 
    assert(bridge.num_on_bridge[(destination+1)%2] == 0); 

    sleep(1);
    destination = (destination + 1) % 2;
  }

  return NULL;
}

int main(int argc, char** argv) {

  if (argc != 3) {
    printf("usage: %s <NumThreads> <NumTrips>\n", argv[0]);
    exit(0);
  }

  int N = strtol(argv[1], NULL, 10);
  int ntrips = strtol(argv[2], NULL, 10);

  bridge.direction = OUTLOOK;
  bridge.num_on_bridge[OUTLOOK] = 0;
  bridge.num_on_bridge[TOWN] = 0;
  bridge.waiting[OUTLOOK] = 0;
  bridge.waiting[TOWN] = 0;
  pthread_mutex_init(&(bridge.lock), NULL);
  pthread_cond_init(&(bridge.cond[OUTLOOK]), NULL);
  pthread_cond_init(&(bridge.cond[TOWN]), NULL);

  pthread_t* threads = malloc(sizeof(pthread_t) * N);
  struct thread_data* data = malloc(sizeof(struct thread_data) * N);
  for (int i = 0; i < N; i++) {
    data[i].id = i;
    data[i].num_trips = ntrips;
    data[i].start_destination = i < (N+1)/2 ? OUTLOOK : TOWN;
    pthread_create(&threads[i], NULL, Move, &data[i]);
  }

  for (int i = 0; i < N; i++) {
    pthread_join(threads[i], NULL);
  }

  free(threads);
  free(data);
  pthread_mutex_destroy(&bridge.lock);
  pthread_cond_destroy(&bridge.cond[OUTLOOK]);
  pthread_cond_destroy(&bridge.cond[TOWN]);
  return 0;
}
