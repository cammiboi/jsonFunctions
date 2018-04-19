#ifndef JSONFUNCTIONS_H
#define JSONFUNCTIONS_H

#include <string.h>

//looks through a JSON string and tries to find a certain tag
//returns -1 if not found and 0 if found
//specify the maximum allowed length of the value saved into stringToStoreValue

int jsonTagFinder(char *jsonStringToSearch, char *tagToFind, char *stringToStoreValue, int maxValueLength);

// saves the json array (must be of numbers) at a given tag in an interger array 

int jsonGetArray(char *jsonStringToSearch, char *tagToFind, int *arrayToStoreValues, int maxArrayLength);

#endif