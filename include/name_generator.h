// Concurrent starfighters - Name generator module - Header file.

// Define guard:
#ifndef CONCURRENT_STARFIGHTERS_NAME_GENERATOR_H_
#define CONCURRENT_STARFIGHTERS_NAME_GENERATOR_H_

// Includes:
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Macros:
#define FIRST_NAME_ARRAY_SIZE 30
#define FULL_NAME_SIZE (NAME_SIZE)*3+3
#define LAST_NAME_ARRAY_SIZE 30
#define MIDDLE_NAME_ARRAY_SIZE 26
#define NAME_SIZE 15

// Function headers:
void generate_name(char*);

#endif // CONCURRENT_STARFIGHTERS_NAME_GENERATOR_H_
