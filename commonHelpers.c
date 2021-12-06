#define _CRT_SECURE_NO_WARNINGS
#include<stdio.h>
#include<time.h>
#include"commonHelpers.h"
#include<string.h>

int currentYear(void)
{
  time_t currentTime = time(NULL);
  return localtime(&currentTime)->tm_year + 1900;
}

void clearStandardInputBuffer(void)
{
  while (getchar() != '\n')
  {
    ;
  }
}

int getInteger() {
  while (1) {
    int tempInt;
    char tempChar = 'x';
    scanf("%d%c", &tempInt, &tempChar);

    if (tempChar != '\n') {
      clearStandardInputBuffer();
      printf("ERROR: Value must be an integer: ");
    }
    else {
      return tempInt;
    }
  }
}

int getPositiveInteger()
{
  while (1) {
    int tempInt;
    char tempChar = 'x';
    scanf("%d%c", &tempInt, &tempChar);

    if (tempChar != '\n') {
      clearStandardInputBuffer();
      printf("ERROR: Value must be an integer: ");
    }
    else {
      if (tempInt > 0) {
        return tempInt;
      }
      else {
        printf("ERROR: Value must be a positive integer greater than zero: ");
      }
    }
  }
}

int getIntFromRange(int lowerBound, int upperBound) {
  while (1) {
    int tempInt;
    char tempChar = 'x';
    scanf("%d%c", &tempInt, &tempChar);

    if (tempChar != '\n') {
      clearStandardInputBuffer();
      printf("ERROR: Value must be an integer: ");
    }
    else {
      if (tempInt >= lowerBound && tempInt <= upperBound) {
        return tempInt;
      }
      else {
        printf("ERROR: Value must be between %d and %d inclusive: ", lowerBound, upperBound);
      }
    }
  }
}

double getDouble() {
  while (1) {
    double tempDouble;
    char tempChar = 'x';
    scanf("%lf%c", &tempDouble, &tempChar);

    if (tempChar != '\n') {
      clearStandardInputBuffer();
      printf("ERROR: Value must be a double floating-point number: ");
    }
    else {
      return tempDouble;
    }
  }
}

double getPositiveDouble() {
  while (1) {
    double tempDouble;
    char tempChar = 'x';
    scanf("%lf%c", &tempDouble, &tempChar);

    if (tempChar != '\n') {
      clearStandardInputBuffer();
      printf("ERROR: Value must be a double floating-point number: ");
    }
    else {
      if (tempDouble > 0) {
        return tempDouble;
      }
      else {
        printf("ERROR: Value must be a positive double floating-point number: ");
      }
    }
  }
}

char getCharOption(char str[]) {
  while (1) {
    char c;
    char tempChar;

    scanf("%c%c", &c, &tempChar);
    int isMatched = 0;
    int i = 0;

    if (tempChar == '\n') {
      while (1) {
        if (str[i] == '\0') break;

        if (str[i] == c) {
          isMatched = 1;
          break;
        }
        i++;
      }
    }
    else {
      clearStandardInputBuffer();
    }
    if (isMatched == 0) {
      printf("ERROR: Character must be one of [%s]: ", str);
    }
    else if (isMatched == 1) {
      return c;
    }
  }
  clearStandardInputBuffer();
}

void getCString(char* c, int lowerBound, int upperBound) {
  while (1) {
    char str[1000];
    scanf("%[^\n]s", str);

    int len = strlen(str);
    if (len >= lowerBound && len <= upperBound) {
      strcpy(c, str);
      clearStandardInputBuffer();
      break;
    }
    else {
      if (lowerBound == upperBound) {
        printf("ERROR: String length must be exactly %d chars: ", upperBound);
      }
      else if (len > upperBound) {
        printf("ERROR: String length must be no more than %d chars: ", upperBound);
      }
      else {
        printf("ERROR: String length must be between %d and %d chars: ", lowerBound, upperBound);
      }
      clearStandardInputBuffer();
    }
  }
}

void getPassword(char* c) {
  char str[1000];
  scanf("%[^\n]s", str);

  strcpy(c, str);
  clearStandardInputBuffer();
}
