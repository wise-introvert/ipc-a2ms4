#ifndef ACCOUNT_H_
#define ACCOUNT_H_

struct Person
{
    char personName[32];
    int birthYear;
    double householdIncome;
    char country[32];
};

struct UserLogin
{
    char userLoginName[12];
    char password[12];
};

struct Account
{
    int accountNumber;
    char accountType;
    struct Person person;
    struct UserLogin userLogin;
};

void getAccount(struct Account* account);
void getPerson(struct Person* person);
void getUserLogin(struct UserLogin* userLogin);

void updateAccount(struct Account* account);
void updatePerson(struct Person* person);
void updateUserLogin(struct UserLogin* userLogin);

void setUserLoginToSafeState(struct UserLogin* userLogin);
#endif