#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

//looks through a JSON string and tries to find a certain tag
//returns -1 if not found and 0 if found
//specify the maximum allowed length of the value saved into stringToStoreValue

int jsonTagFinder(char *jsonStringToSearch, char *tagToFind, char *stringToStoreValue, int maxValueLength)
{
	//first see if we can locate tag
	char *tagStart;
	char *currentPosition;
	char *value = NULL;
	int tagLength = strlen(tagToFind);
	tagStart = strstr(jsonStringToSearch, tagToFind);
	if (tagStart == NULL)
	{
		printf("jsonTagFinder, could not locate tag, exiting with return value -1 \n");
		return -1;
	}
	currentPosition = tagStart + tagLength; //move the current position right after the last character in the tag
	if (*currentPosition != '\"' || *currentPosition == NULL)
	{
		printf("jsonTagFinder, tag does not have closing quote, exiting with return value -1 \n");
		return -1;
	}
	currentPosition++; //should be at a ":" now
	if (*currentPosition != ':' || *currentPosition == NULL)
	{
		printf("jsonTagFinder, tag does not have :, exiting with return value -1 \n");
		return -1;
	}
	currentPosition++; //now we are at the value, if its a " keep moving
	if (*currentPosition == '\"')
	{
		currentPosition++;
	}
	if (currentPosition == NULL)
	{
		printf("jsonTagFinder, reached end of string :, exiting with return value -1 \n");
		return -1;
	}
	value = currentPosition; //save the current position as the start of the value
	int counter = 0;
	while (*currentPosition != '\"' && *currentPosition != ',' && *currentPosition != '}' && *currentPosition != NULL)
	{
		currentPosition++;
		if (counter > maxValueLength)
		{
			printf("jsonTagFinder, value length exceeded %d or unable to locate end of value \n", maxValueLength);
			return -1;
		}
		counter++;
	}
	//the value length is the end - the start
	int valueLength = currentPosition - value;
	strncpy(stringToStoreValue, value, valueLength); //copy the found value
	*(stringToStoreValue + valueLength) = NULL;		 // add null terminator
	return 0;
}

int jsonArrayElementFinder(char *jsonStringToSearch, char *tagToFind, char *stringToStoreValue, int maxValueLength, int elementNumberToFind)
{
	//first see if we can locate tag
	char *tagStart;
	char *currentPosition;
	char *value = NULL;
	int tagLength = strlen(tagToFind);
	tagStart = strstr(jsonStringToSearch, tagToFind);
	if (tagStart == NULL)
	{
		printf("jsonTagFinder, could not locate tag, exiting with return value -1 \n");
		return -1;
	}
	currentPosition = tagStart + tagLength; //move the current position right after the last character in the tag
	if (*currentPosition != '\"' || *currentPosition == NULL)
	{
		printf("jsonTagFinder, tag does not have closing quote, exiting with return value -1 \n");
		return -1;
	}
	currentPosition++; //should be at a ":" now
	if (*currentPosition != ':' || *currentPosition == NULL)
	{
		printf("jsonTagFinder, tag does not have :, exiting with return value -1 \n");
		return -1;
	}
	currentPosition++; // should be at the opening bracket of the array now
	if (*currentPosition != '[' || *currentPosition == NULL)
	{
		printf("jsonTagFinder, no opening [ found, exiting with return value -1 \n");
		return -1;
	}
	currentPosition++; // should be at the first element of the array now

	int currentElement = 0;
	while (currentElement < elementNumberToFind && *currentPosition != ']')
	{
		if (*currentPosition == ',')
		{
			currentElement++;
		}
		currentPosition++;
	}
	if (*currentPosition == '\"')
	{
		currentPosition++;
	}
	value = currentPosition; //save the current position as the start of the value
	int counter = 0;
	while (*currentPosition != '\"' && *currentPosition != ',' && *currentPosition != '}' && *currentPosition != ']' && *currentPosition != NULL)
	{
		currentPosition++;
		if (counter > maxValueLength)
		{
			printf("jsonTagFinder, value length exceeded %d or unable to locate end of value \n", maxValueLength);
			return -1;
		}
		counter++;
	}
	//the value length is the end - the start
	int valueLength = currentPosition - value;
	strncpy(stringToStoreValue, value, valueLength); //copy the found value
	*(stringToStoreValue + valueLength) = NULL;		 // add null terminator
	return 0;
}

// saves the json array (must be of numbers) at a given tag in an interger array

int jsonGetArray(char *jsonStringToSearch, char *tagToFind, int *arrayToStoreValues, int maxArrayLength)
{
	//first see if we can locate tag
	char *tagStart;
	char *currentPosition;
	char *value = NULL;
	int tagLength = strlen(tagToFind);
	tagStart = strstr(jsonStringToSearch, tagToFind);
	if (tagStart == NULL)
	{
		printf("jsonGetArray, could not locate tag, exiting with return value -1 \n");
		return -1;
	}
	currentPosition = tagStart + tagLength; //move the current position right after the last character in the tag
	if (*currentPosition != '\"' || *currentPosition == NULL)
	{
		printf("jsonGetArray, tag does not have closing quote, exiting with return value -1 \n");
		return -1;
	}
	currentPosition++; //should be at a ":" now
	if (*currentPosition != ':' || *currentPosition == NULL)
	{
		printf("jsonGetArray, tag does not have :, exiting with return value -1 \n");
		return -1;
	}
	currentPosition++; //now we are at the value, we should see a "[ here
	if (*currentPosition != '[')
	{
		printf("jsonGetArray, tag does not have [, exiting with return value -1 \n");
		return -1;
	}
	currentPosition++;
	if (currentPosition == NULL)
	{
		printf("jsonGetArray, reached end of string :, exiting with return value -1 \n");
		return -1;
	}
	value = currentPosition;

	//now move along the string until we find the first ,
	int currentArrayPosition = 0;
	char *charAfterNumber = NULL;
	while (1)
	{
		if (currentArrayPosition >= maxArrayLength)
		{
			printf("jsonGetArray, there are more array elements in the JSON string than the maxArrayLength of %d, exiting with return value -1 \n", maxArrayLength);
			return -1;
		}
		int insideCounter = 0;
		while (*currentPosition != ',' && *currentPosition != NULL && *currentPosition != ']')
		{
			currentPosition++;
			if (insideCounter > 5)
			{
				printf("jsonGetArray, the number is more than 5 characters long which is larger than the max size of a 16 bit int, exiting with return value -1 \n");
				return -1;
			}
			insideCounter++;
		}
		//printf("currentArrayPosition = %d, value string = [%s], *currentPosition = %c \n",currentArrayPosition,value,*currentPosition);
		if (*currentPosition == ']')
		{ //this is the last value in the array
			//convert string to int
			arrayToStoreValues[currentArrayPosition] = strtol(value, &charAfterNumber, 10);
			break;
		}
		else if (*currentPosition == ',')
		{ //not the last element
			arrayToStoreValues[currentArrayPosition] = strtol(value, &charAfterNumber, 10);
		}
		else
		{ // cant find [ or ,
			printf("jsonGetArray, not able to find a ] or , to mark next number or end of array, exiting with return value -1 \n");
			return -1;
		}
		currentArrayPosition++;
		currentPosition++;
		value = currentPosition;
	}
	return (currentArrayPosition + 1); //return the size of the saved array
}