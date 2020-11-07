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
pthread_mutex_t mutex_allied_starfighters = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_enemy_starfighters = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex_pilots = PTHREAD_MUTEX_INITIALIZER;
sem_t sem_end_condition;
sem_t sem_lose_condition;
sem_t sem_starfighters_ready_to_fly;
sem_t sem_win_condition;
unsigned int num_allied_starfighters = INITIAL_NUM_STARFIGHTERS;
unsigned int num_enemy_starfighters = NUM_ENEMY_STARFIGHTERS;
unsigned int num_pilots = NUM_PILOT_THREADS;

// Function headers:
boolean check_event_outcome_with_probability(double);
unsigned int roll_dice_with_N_sides(unsigned int);
void bury_pilot(pilot_args*);
void check_end_condition(void);
void decrease_enemy_starfighters(unsigned int);
void fight_against_enemies(pilot_args*);
void land_starfighter(pilot_args*);
void take_off_on_starfigther(pilot_args*);
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
  sem_init(&sem_starfighters_ready_to_fly, 0, INITIAL_NUM_STARFIGHTERS);
  sem_init(&sem_end_condition, 0, 0);
  sem_init(&sem_lose_condition, 0, 0);
  sem_init(&sem_win_condition, 0, 0);

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

  // Wait for the war to end.
  sem_wait(&sem_end_condition);

  // Check to see if the war was won.
  if(sem_trywait(&sem_win_condition) == 0) {
    pthread_mutex_lock(&mutex_pilots);
      printf(
        "The war was WON! There were %u pilots who survived.\n", num_pilots
      );
    pthread_mutex_unlock(&mutex_pilots);
  }

  else if(sem_trywait(&sem_lose_condition) == 0)
    printf("The war was LOST!\n");

  // Finish execution.
  return 0;

}

// Function implementations:
boolean check_event_outcome_with_probability(double event_probability) {
  if(roll_dice_with_N_sides(100) > (event_probability * 100))
    return FALSE;
  else
    return TRUE;
}

unsigned int roll_dice_with_N_sides(unsigned int sides) {
  if(sides == 0)
    return 0;
  else
    return (rand() % sides) + 1;
}

void bury_pilot(pilot_args *pilot_info) {

  // Bury the pilot.
  pthread_mutex_lock(&mutex_pilots);
    num_pilots -= 1;
  pthread_mutex_unlock(&mutex_pilots);

  // Account for the destroyed starfighter.
  pthread_mutex_lock(&mutex_allied_starfighters);
    num_allied_starfighters -= 1;
  pthread_mutex_unlock(&mutex_allied_starfighters);

  // Check to see if the war ended.
  check_end_condition();

  printf("Pilot %s (#%u) died in battle!\n", pilot_info->name, pilot_info->id);

}

void check_end_condition(void) {

  // Variable declaration.
  boolean lose_condition = FALSE, win_condition = FALSE;

  // Pilots win when the number of enemy starfighters reaches 0.
  pthread_mutex_lock(&mutex_enemy_starfighters);
    if(num_enemy_starfighters == 0) {
      win_condition = TRUE;
      sem_post(&sem_win_condition);
    }
  pthread_mutex_unlock(&mutex_enemy_starfighters);

  // Enemies win when the number of pilots or allied starfighters reaches 0.
  pthread_mutex_lock(&mutex_pilots);
  pthread_mutex_lock(&mutex_allied_starfighters);
    if((num_pilots == 0 || num_allied_starfighters == 0) && !win_condition) {
      lose_condition = TRUE;
      sem_post(&sem_lose_condition);
    }
  pthread_mutex_unlock(&mutex_allied_starfighters);
  pthread_mutex_unlock(&mutex_pilots);

  // Signal end condition.
  if(lose_condition || win_condition)
    sem_post(&sem_end_condition);

}

void decrease_enemy_starfighters(unsigned int num_enemies_destroyed) {
  pthread_mutex_lock(&mutex_enemy_starfighters);
    if(num_enemies_destroyed > num_enemy_starfighters)
      num_enemy_starfighters = 0;
    else
      num_enemy_starfighters -= num_enemies_destroyed;
  pthread_mutex_unlock(&mutex_enemy_starfighters);
}

void fight_against_enemies(pilot_args *pilot_info) {

  // Variable declaration.
  unsigned int num_enemies_destroyed;

  // Fighting against enemies takes time.
  sleep(roll_dice_with_N_sides(12) + 2);

  // The pilot destroys a number of enemy starfighters when fighting.
  num_enemies_destroyed = roll_dice_with_N_sides(MAX_ENEMIES_DESTROYED_PER_RUN);
  decrease_enemy_starfighters(num_enemies_destroyed);

  printf(
    "Pilot %s (#%u) destroyed %u enemies!\n",
    pilot_info->name, pilot_info->id, num_enemies_destroyed
  );

}

void land_starfighter(pilot_args *pilot_info) {

  printf(
    "Pilot %s (#%u) returned from battle!\n", pilot_info->name, pilot_info->id
  );

  // Check to see if the war ended.
  check_end_condition();

  // Return the starfighter.
  sem_post(&sem_starfighters_ready_to_fly);

}

void take_off_on_starfigther(pilot_args *pilot_info) {

  // Acquire starfighter.
  sem_wait(&sem_starfighters_ready_to_fly);

  printf(
    "Pilot %s (#%u) took off on a starfighter!\n",
    pilot_info->name, pilot_info->id
  );
  
}


void * engineer(void *args) {

  // Placeholder.
  while(TRUE) {
    sleep(5);
  }

}

void * pilot(void *args) {

  // Variable declaration.
  boolean pilot_died_on_this_run;
  pilot_args pilot_information;

  pilot_information.id = ((pilot_args*) args)->id;
  strcpy(
    (char *) &pilot_information.name,
    (char *) &(((pilot_args*) args)->name)
  );

  printf("Pilot #%u: %s\n", pilot_information.id, pilot_information.name);

  // Main thread loop.
  while(TRUE) {

    // Desync threads.
    sleep(roll_dice_with_N_sides(10) + 4);

    take_off_on_starfigther(&pilot_information);
    fight_against_enemies(&pilot_information);

    // Check if the pilot made it out alive.
    pilot_died_on_this_run = \
      check_event_outcome_with_probability(PILOT_DEATH_PROBABILITY);

    if(pilot_died_on_this_run) {
      bury_pilot(&pilot_information);
      pthread_exit((void*) 1);
    }

    else
      land_starfighter(&pilot_information);

  }

}
