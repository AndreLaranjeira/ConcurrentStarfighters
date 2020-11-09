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
  "Doyle",
  "Erik",
  "Esther",
  "Evelyn",
  "Gustav",
  "Hugo",
  "Igor",
  "Jack",
  "James",
  "John",
  "Jonathan",
  "Katheryn",
  "Kyle",
  "Martin",
  "Mia",
  "Mike",
  "Nathalie",
  "Nick",
  "Oliver",
  "Owen",
  "Peter",
  "Rebecca",
  "Robert",
  "Sarah",
  "Saul",
  "Theresa",
  "Thomas",
  "Tobias",
  "Victor",
  "Wallace",
  "Yan",
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
  "Ashen",
  "Ayre",
  "Barnes",
  "Bishop",
  "Burns",
  "Carter",
  "Cobalt",
  "Cruz",
  "Dashiell",
  "D'Alessandro",
  "Estevez",
  "Falls",
  "Finch",
  "Fisher",
  "Golds",
  "Hidenburg",
  "Kurts",
  "Johnson",
  "Lars",
  "Li",
  "Lockwood",
  "Marks",
  "Matterhorn",
  "Meadows",
  "O'Hare",
  "Potts",
  "Price",
  "Rayleight",
  "Reese",
  "Richardson",
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
  char name_generated[FULL_NAME_SIZE], space[2]=" \0";

  strcpy((char *) &name_generated, first_names[rand() % FIRST_NAME_ARRAY_SIZE]);
  strcat((char *) &name_generated, (char *) &space);
  strcat((char *) &name_generated, middle_names[rand() % MIDDLE_NAME_ARRAY_SIZE]);
  strcat((char *) &name_generated, (char *) &space);
  strcat((char *) &name_generated, last_names[rand() % LAST_NAME_ARRAY_SIZE]);
  name_generated[FULL_NAME_SIZE - 1] = '\0';

  strcpy(destination, (char *) &name_generated);
}
