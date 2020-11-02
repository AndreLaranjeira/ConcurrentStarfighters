// Concurrent starfighters - Name generator module - Source code.

// Header include:
#include "name_generator.h"

// Private variables:
const char first_names[FIRST_NAME_ARRAY_SIZE][NAME_SIZE] = {
  "Agatha",
  "Anna",
  "Andrew",
  "Arthur",
  "Bianca",
  "Ben",
  "Celine",
  "Dereck",
  "Esther",
  "Evelyn",
  "Gustav",
  "Hugo",
  "Igor",
  "Jack",
  "James",
  "John",
  "Katheryn",
  "Kyle",
  "Mia",
  "Mike",
  "Owen",
  "Peter",
  "Sarah",
  "Saul",
  "Theresa",
  "Thomas",
  "Tobias",
  "Victor",
  "Wallace",
  "Zack"
};

const char middle_names[MIDDLE_NAME_ARRAY_SIZE][NAME_SIZE] = {
  "A.",
  "B.",
  "C.",
  "D.",
  "E.",
  "F.",
  "G.",
  "H.",
  "I.",
  "J.",
  "K.",
  "L.",
  "M.",
  "N.",
  "O.",
  "P.",
  "Q.",
  "R.",
  "S.",
  "T.",
  "U.",
  "V.",
  "W.",
  "X.",
  "Y.",
  "Z."
};

const char last_names[LAST_NAME_ARRAY_SIZE][NAME_SIZE] = {
  "Ayre",
  "Ashen",
  "Barnes",
  "Bishop",
  "Carter",
  "Dashiell",
  "Estevez",
  "Fisher",
  "Golds",
  "Hidenburg",
  "Johnson",
  "Li",
  "Lockwood",
  "Marks",
  "Matterhorn",
  "Meadows",
  "O'Hare",
  "Polar",
  "Rayleight",
  "Reese",
  "Salazar",
  "Silva",
  "Slate",
  "Thompson",
  "Watts",
  "Wells",
  "Wolf",
  "Wu",
  "Zachary",
  "Zollen"
};

// Function implementations:
void generate_name(char *destination) {
  char name_generated[FULL_NAME_SIZE], space[2]=" \0", terminator='\0';

  strcpy((char *) &name_generated, first_names[rand() % FIRST_NAME_ARRAY_SIZE]);
  strcat((char *) &name_generated, (char *) &space);
  strcat((char *) &name_generated, middle_names[rand() % MIDDLE_NAME_ARRAY_SIZE]);
  strcat((char *) &name_generated, (char *) &space);
  strcat((char *) &name_generated, last_names[rand() % LAST_NAME_ARRAY_SIZE]);
  strcat((char *) &name_generated, &terminator);

  strcpy(destination, (char *) &name_generated);
}
