// Concurrent starfighters - Main function.

// Includes:
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>
#include <unistd.h>

// User includes:
#include "name_generator.h"

// Macros:
#define DEBUG_LEVEL 0
#define INITIAL_NUM_STARFIGHTERS 10
#define MAX_ENEMIES_DESTROYED_PER_RUN 4
#define NUM_ENEMY_STARFIGHTERS 100
#define NUM_PILOT_THREADS 10
#define PILOT_DEATH_PROBABILITY 0.2

// Type declarations:
typedef enum {
  FALSE = 0,
  TRUE = 1
} boolean;

typedef struct {
  unsigned int id;
  char name[FULL_NAME_SIZE];
} pilot_args;

// Global variables:
pthread_mutex_t mutex_enemy_starfighters = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_pilots = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_starfighters;
unsigned int num_enemy_starfighters = NUM_ENEMY_STARFIGHTERS;
unsigned int num_pilots = NUM_PILOT_THREADS;

// Function headers:
void* engineer(void*);
void* pilot(void*);

// Main function:
int main(int argc, char const *argv[]) {

  // Variable declaration.
  int i;
  pilot_args pilot_thread_args[NUM_PILOT_THREADS];
  pthread_t pilot_thread_ids[NUM_PILOT_THREADS];

  // Initialize random number generator.
  srand((unsigned int) time(NULL));

  // Initialize semaphores.
  sem_init(&sem_starfighters, 0, INITIAL_NUM_STARFIGHTERS);

  // Create pilot threads.
  for(i = 0; i < NUM_PILOT_THREADS; i++) {

    // Generate pilot thread args.
    pilot_thread_args[i].id = i + 1;
    generate_name((char *) &pilot_thread_args[i].name);

    // Create thread.
    pthread_create(&pilot_thread_ids[i], NULL, pilot, &pilot_thread_args[i]);

    // Small desync to avoid similar pilot args.
    usleep(500);

  }

  // Join all threads.
  for(i = 0; i < NUM_PILOT_THREADS; i++) {
    pthread_join(pilot_thread_ids[i], NULL);
  }

  // Check to see if the war was won.
  pthread_mutex_lock(&mutex_pilots);
  pthread_mutex_lock(&mutex_enemy_starfighters);
    if(num_enemy_starfighters == 0)
      printf(
        "The war was WON! There were %u pilots who survived.\n",
        num_pilots
      );
    else
      printf("The war was LOST! All pilots were killed!\n");
  pthread_mutex_unlock(&mutex_enemy_starfighters);
  pthread_mutex_unlock(&mutex_pilots);

  // Finish execution.
  return 0;

}

// Function implementations:
void * engineer(void *args) {

  // Placeholder.
  while(TRUE) {
    sleep(5);
  }

}

void * pilot(void *args) {

  // Variable declaration.
  boolean still_alive, end_condition = FALSE;
  pilot_args pilot_information;
  unsigned int enemies_destroyed;

  pilot_information.id = ((pilot_args*) args)->id;
  strcpy(
    (char *) &pilot_information.name,
    (char *) &(((pilot_args*) args)->name)
  );

  printf("Pilot #%u: %s\n", pilot_information.id, pilot_information.name);

  while(TRUE) {

    // Desync threads.
    sleep((rand() % 10) + 5);

    // Check end condition (pilots win when the number of enemies reaches 0).
    pthread_mutex_lock(&mutex_enemy_starfighters);
      if(num_enemy_starfighters == 0)
        end_condition = TRUE;
    pthread_mutex_unlock(&mutex_enemy_starfighters);

    // If war ended, the fight ends here.
    if(end_condition)
      pthread_exit(0);

    // Else, the pilot gets back to the fight by acquiring a starfighter.
    sem_wait(&sem_starfighters);

    printf(
      "Pilot %s (#%u) took off on a starship!\n",
      pilot_information.name, pilot_information.id
    );

    // War wait time.
    sleep((rand() % 12) + 3);

    // Chalk up how many enemies the pilot destroyed.
    enemies_destroyed = (rand() % MAX_ENEMIES_DESTROYED_PER_RUN) + 1;

    pthread_mutex_lock(&mutex_enemy_starfighters);
      if(enemies_destroyed > num_enemy_starfighters)
        num_enemy_starfighters = 0;
      else
        num_enemy_starfighters -= enemies_destroyed;
    pthread_mutex_unlock(&mutex_enemy_starfighters);

    // Check if the pilot made it out alive.
    still_alive = rand() % 100 >= PILOT_DEATH_PROBABILITY * 100 ? TRUE : FALSE;

    if(!still_alive) {
      // Update the number of pilots.
      pthread_mutex_lock(&mutex_pilots);
        if(num_pilots > 0)
          num_pilots -= 1;
      pthread_mutex_unlock(&mutex_pilots);

      // Bid farewell to the pilot and his(her) ship.
      printf(
        "Pilot %s (#%u) died in battle!\n",
        pilot_information.name, pilot_information.id
      );

      pthread_exit((void*) 1);
    }

    else {
      // Hail the returning pilot.
      printf(
        "Pilot %s (#%u) returned from battle after destorying %u enemies!\n",
        pilot_information.name, pilot_information.id, enemies_destroyed
      );

      // Return the starfighter.
      sem_post(&sem_starfighters);
    }

  }

}
