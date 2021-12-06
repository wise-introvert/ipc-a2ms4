#ifndef COMMONHELPERS_H_
#define COMMONHELPERS_H_

int currentYear(void);
void clearStandardInputBuffer(void);

int getInteger();
int getPositiveInteger();
double getDouble();
double getPositiveDouble();
int getIntFromRange(int lowerBound, int upperBound);
char getCharOption(char str[]);
void getCString(char* c, int lowerBound, int upperBound);
void getPassword(char* c);
#endif
