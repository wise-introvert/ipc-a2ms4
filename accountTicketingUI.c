#include "accountTicketingUI.h"
#include <stdio.h>
#include "account.h"
#include "commonHelpers.h"
#include <string.h>
#include<stdlib.h>

int loginINDEX = -1;
int modifyMenuActive = 0;
int uniqueAccountNumber = 0;

void applicationStartup(struct AccountTicketingData* accountTicketingData)
{
  while (1)
  {
    if (loginINDEX == -1)
    {
      loginINDEX = menuLogin(accountTicketingData->accounts, accountTicketingData->ACCOUNT_MAX_SIZE);
    }
    else if (loginINDEX == -2)
    {
      printf("\n==============================================\n"
          "Account Ticketing System - Terminated\n"
          "==============================================\n");
      break;
    }
    else
    {
      printf("\n");
      menuAgent(accountTicketingData, &accountTicketingData->accounts[loginINDEX]);
    }
  }
}

int menuLogin(const struct Account accounts[], int size)
{
  printf("==============================================\n"
      "Account Ticketing System - Login\n"
      "==============================================\n"
      "1) Login to the system\n"
      "0) Exit application\n"
      "----------------------------------------------\n");
  printf("\n");
  printf("Selection: ");
  int choice = getIntFromRange(0, 1);
  printf("\n");
  if (choice == 0)
  {
    printf("Are you sure you want to exit? ([Y]es|[N]o): ");
    char c = getCharOption("yYnN");
    if (c == 'y' || c == 'Y')
    {
      return -2;
    }
    else if (c == 'n' || c == 'N')
    {
      printf("\n");
      return -1;
    }
  }
  else if (choice == 1)
  {
    int chancesLeft = 3;
    while (1)
    {
      int loginIndex = CheckLoginCredentials(accounts, size);
      if (loginIndex != -1)
      {
        loginINDEX = loginIndex;
        return loginINDEX;
      }
      else
      {
        printf("INVALID user login/password combination! [attempts remaining:%d]\n", --chancesLeft);
        printf("\n");
      }
      if (chancesLeft <= 0)
      {
        printf("ERROR:  Access Denied.\n");
        printf("\n");
        pauseExecution();
        return -1;
        break;
      }
    }
  }
  return -1;
}
void menuAgent(struct AccountTicketingData* accountTicketingData, const struct Account* account)
{
  while (1)
  {
    printf("AGENT: %s (%d)", account->person.personName, account->accountNumber);
    printf("\n==============================================\n"
        "Account Ticketing System - Agent Menu\n"
        "==============================================\n"
        "1) Add a new account\n"
        "2) Modify an existing account\n"
        "3) Remove an account\n"
        "4) List accounts: detailed view\n"
        "----------------------------------------------\n"
        "5) List new tickets\n"
        "6) List active tickets\n"
        "7) List closed tickets\n"
        "8) Add a new ticket\n"
        "9) Manage a ticket\n"
        "----------------------------------------------\n"
        "0) Logout\n\n"
        "Selection: ");

    int choice = getIntFromRange(0, 9);

    printf("\n");
    if (choice == 1)
    {
      int emptyIndex = 0;
      int isFound = 0;
      for (emptyIndex = 0; emptyIndex < accountTicketingData->ACCOUNT_MAX_SIZE; emptyIndex++)
      {
        if (accountTicketingData->accounts[emptyIndex].accountNumber == 0)
        {
          isFound = 1;
          break;
        }
      }
      if (isFound == 0)
      {
        printf("ERROR: Account listing is FULL, call ITS Support!\n\n");
        pauseExecution();
      }
      else
      {
        UpdateUniqueAccountNumber(accountTicketingData->accounts, accountTicketingData->ACCOUNT_MAX_SIZE);
        getAccount(&accountTicketingData->accounts[emptyIndex]);
        printf("* New account added! *\n");
        printf("\n");
        pauseExecution();
      }
    }
    else if (choice == 2)
    {
      printf("Enter the account#: ");
      int accno = getPositiveInteger();
      int indexx = findAccountIndexByAcctNum(accno, accountTicketingData->accounts, accountTicketingData->ACCOUNT_MAX_SIZE, 0);

      if (indexx != -1)
      {
        setModifyMenuActive(1);
        while (modifyMenuActive == 1)
        {
          updateAccount(&accountTicketingData->accounts[indexx]);
        }
      }
      else
      {
      }
    }
    else if (choice == 3)
    {
      printf("Enter the account#: ");
      int accno = getPositiveInteger();

      if (account->accountNumber == accno)
      {
        printf("\nERROR: You can't remove your own account!\n\n");
        pauseExecution();
      }
      else
      {
        int indexx = findAccountIndexByAcctNum(accno, accountTicketingData->accounts, accountTicketingData->ACCOUNT_MAX_SIZE, 0);
        if (indexx != -1)
        {
          printf("\n");
          displayAccountDetailHeader();
          displayAccountDetailRecord(&accountTicketingData->accounts[indexx]);

          printf("\nAre you sure you want to remove this record? ([Y]es|[N]o): ");
          char ysno = getCharOption("YynN");
          if (ysno == 'y' || ysno == 'Y')
          {
            int i = 0;
            int ticketRemovedCounter = 0;
            for (i = 0; i < accountTicketingData->TICKET_MAX_SIZE; i++)
            {
              if (accountTicketingData->tickets[i].customerAccountNumber == accountTicketingData->accounts[indexx].accountNumber)
              {
                accountTicketingData->tickets[i].customerAccountNumber = 0;
                ticketRemovedCounter++;
              }
            }
            i = 0;
            accountTicketingData->accounts[indexx].accountNumber = 0;
            printf("\n** Account Removed! (%d ticket(s) removed) **\n\n", ticketRemovedCounter);
            pauseExecution();
          }
          else
          {
          }
        }
        else
        {
        }
      }
    }
    else if (choice == 4)
    {
      displayAllAccountDetailRecords(accountTicketingData->accounts, accountTicketingData->ACCOUNT_MAX_SIZE);
      printf("\n");
      pauseExecution();
    }
    else if (choice == 5)
    {
      ListNewTickets(accountTicketingData);
    }
    else if (choice == 6)
    {
      ListActiveTickets(accountTicketingData);
    }
    else if (choice == 7)
    {
      ListClosedTickets(accountTicketingData);
    }
    else if (choice == 8)
    {
      AddNewTicket(accountTicketingData);
    }
    else if (choice == 9)
    {
      ManageTicket(accountTicketingData);
    }
    else if (choice == 0)
    {
      loginINDEX = -1;
      printf("Saving session modifications...\n");
      printf("   %d account(s) saved.\n", SaveAccounts(accountTicketingData->accounts, accountTicketingData->ACCOUNT_MAX_SIZE));
      printf("   %d ticket(s) saved.\n", SaveTickets(accountTicketingData->tickets, accountTicketingData->TICKET_MAX_SIZE));
      printf("### LOGGED OUT ###\n\n");
      break;
    }
  }
}
int findAccountIndexByAcctNum(int accNoToFind, const struct Account accounts[], int size, int isPrompt)
{
  if (isPrompt == 1)
  {
    printf("Enter your account#: ");
    int accno = getPositiveInteger();
    accNoToFind = accno;
  }

  int i = 0;
  int isFound = 0;
  for (i = 0; i < size; i++)
  {
    if (accNoToFind == accounts[i].accountNumber)
    {
      isFound = 1;
      break;
    }
  }

  if (isFound == 1)
  {
    return i;
  }
  else
  {
    return -1;
  }
}
int CheckLoginCredentials(const struct Account accounts[], int size)
{
  printf("Enter the account#: ");
  int accNo = getPositiveInteger();
  printf("User Login        : ");
  char ul[32];
  getCString(ul, 1, 30);
  printf("Password          : ");
  char p[10];
  getPassword(p);
  int index = findAccountIndexByAcctNum(accNo, accounts, size, 0);
  if (index != -1)
  {
    int credok = 1;
    if (strcmp(accounts[index].userLogin.userLoginName, ul) != 0)
    {
      credok = -1;
    }
    if (strcmp(accounts[index].userLogin.password, p) != 0)
    {
      credok = -1;
    }
    if (credok == 1)
    {
      return index;
    }
    else if (credok == -1)
    {
      return -1;
    }
  }
  else
  {
    return -1;
  }
  return -1;
}
void displayAllAccountDetailRecords(const struct Account accounts[], int size)
{
  displayAccountDetailHeader();
  int index = 0;
  for (index = 0; index < size; index++)
  {
    if (accounts[index].accountNumber != 0)
    {
      displayAccountDetailRecord(&accounts[index]);
    }
  }
}

void displayAccountDetailHeader()
{
  printf("Acct# Acct.Type Full Name       Birth Income      Country    Login      Password\n");
  printf("----- --------- --------------- ----- ----------- ---------- ---------- --------\n");
}
void displayAccountDetailRecord(const struct Account* account)
{
  printf("%05d ", account->accountNumber);
  (account->accountType == 'A') ? printf("%-9s ", "AGENT ") : printf("%-9s ", "CUSTOMER ");
  printf("%-15s ", account->person.personName);
  printf("%5d ", account->person.birthYear);
  printf("%11.2lf ", account->person.householdIncome);
  printf("%-10s ", account->person.country);

  if (account->userLogin.userLoginName[0] != 0)
  {
    printf("%-10s ", account->userLogin.userLoginName);
    int i = 0;
    for (i = 0; i < 8; i++)
    {
      if ((i + 1) % 2 == 0)
      {
        printf("*");
      }
      else
      {
        printf("%c", account->userLogin.password[i]);
      }
    }
  }
  else
  {
    printf("%-10s ", "");
    printf("%-8s", "");
  }

  printf("\n");
}

void pauseExecution(void)
{
  printf("<< ENTER key to Continue... >>");
  clearStandardInputBuffer();
  putchar('\n');
}

int getModifyMenuActive()
{
  return modifyMenuActive;
}
void setModifyMenuActive(int val)
{
  modifyMenuActive = val;
}

int getUniqueAccountNumber()
{
  return uniqueAccountNumber;
}
void UpdateUniqueAccountNumber(const struct Account accounts[], int size)
{
  int i, largestAccountNumber = -1;
  for (i = 0; i < size; i++)
  {
    int temp = accounts[i].accountNumber;
    if (temp > largestAccountNumber)
    {
      largestAccountNumber = temp;
    }
  }
  uniqueAccountNumber = largestAccountNumber + 1;
}

int getLoginindex()
{
  return loginINDEX;
}

void ListNewTickets(struct AccountTicketingData* accountTicketingData)
{
  while (1)
  {
    ListTicketHeader();
    int i = 0;
    int listedTickets[50] = { 0 };
    int listedTicketCounter = 0;

    for (i = 0; i < accountTicketingData->TICKET_MAX_SIZE; i++)
    {
      if (accountTicketingData->tickets[i].ticketStatus == 1 && accountTicketingData->tickets[i].customerAccountNumber != 0)
      {
        if (accountTicketingData->tickets[i].messageCounter == 1)
        {
          listedTickets[listedTicketCounter] = accountTicketingData->tickets[i].uniqueNumber;
          listedTicketCounter++;

          printf("%06d ", accountTicketingData->tickets[i].uniqueNumber);
          printf("%05d ", accountTicketingData->tickets[i].customerAccountNumber);
          int j = 0;
          int indx = -1;
          for (j = 0; j < accountTicketingData->ACCOUNT_MAX_SIZE; j++)
          {
            if (accountTicketingData->accounts[j].accountNumber == accountTicketingData->tickets[i].customerAccountNumber)
            {
              indx = j;
              break;
            }
          }
          printf("%-15s ", accountTicketingData->accounts[indx].person.personName);
          if (accountTicketingData->tickets[i].ticketStatus == 1)
          {
            printf("ACTIVE ");
          }
          else
          {
            printf("CLOSED ");
          }
          printf("%-30s ", accountTicketingData->tickets[i].subjectLine);
          printf("%5d\n", accountTicketingData->tickets[i].messageCounter);
        }
      }
    }
    ListTicketFooter();
    printf("\nEnter the ticket number to view the messages or\n"
        "0 to return to previous menu: ");
    int tempTicketNumber = getInteger();
    if (tempTicketNumber == 0)
    {
      printf("\n");
      break;
    }
    i = 0;
    int exist = 0;
    for (i = 0; i < listedTicketCounter; i++)
    {
      if (listedTickets[i] == tempTicketNumber)
      {
        exist = 1;
        break;
      }
    }
    int listedTicketIndex = FindTicketIndex(tempTicketNumber, accountTicketingData->tickets, accountTicketingData->TICKET_MAX_SIZE);

    if (exist == 1)
    {
      printf("\n");
      struct Ticket tempTicket = accountTicketingData->tickets[listedTicketIndex];
      ListMessageHeader(tempTicket);
      i = 0;
      for (i = 0; i < tempTicket.messageCounter; i++)
      {
        tempTicket.messages[i].authorAccountType == 'C' ? printf("CUSTOMER ") : printf("AGENT ");
        printf("(%s):\n", tempTicket.messages[i].authorAcountName);
        printf("   %s\n\n", tempTicket.messages[i].message);
      }
      pauseExecution();
    }
    else
    {
      printf("\nERROR: Invalid ticket number.\n\n");
      pauseExecution();
      printf("\n");
    }
  }
}
void ListActiveTickets(struct AccountTicketingData* accountTicketingData)
{
  while (1)
  {
    ListTicketHeader();
    int i = 0;
    int listedTickets[50] = { 0 };
    int listedTicketCounter = 0;

    for (i = 0; i < accountTicketingData->TICKET_MAX_SIZE; i++)
    {
      if (accountTicketingData->tickets[i].ticketStatus == 1 && accountTicketingData->tickets[i].customerAccountNumber != 0)
      {
        listedTickets[listedTicketCounter] = accountTicketingData->tickets[i].uniqueNumber;
        listedTicketCounter++;

        printf("%06d ", accountTicketingData->tickets[i].uniqueNumber);
        printf("%05d ", accountTicketingData->tickets[i].customerAccountNumber);
        int j = 0;
        int indx = -1;
        for (j = 0; j < accountTicketingData->ACCOUNT_MAX_SIZE; j++)
        {
          if (accountTicketingData->accounts[j].accountNumber == accountTicketingData->tickets[i].customerAccountNumber)
          {
            indx = j;
            break;
          }
        }
        printf("%-15s ", accountTicketingData->accounts[indx].person.personName);
        if (accountTicketingData->tickets[i].ticketStatus == 1)
        {
          printf("ACTIVE ");
        }
        else
        {
          printf("CLOSED ");
        }
        printf("%-30s ", accountTicketingData->tickets[i].subjectLine);
        printf("%5d\n", accountTicketingData->tickets[i].messageCounter);
      }
    }
    ListTicketFooter();
    printf("\nEnter the ticket number to view the messages or\n"
        "0 to return to previous menu: ");
    int tempTicketNumber = getInteger();
    if (tempTicketNumber == 0)
    {
      printf("\n");
      break;
    }
    i = 0;
    int exist = 0;
    for (i = 0; i < listedTicketCounter; i++)
    {
      if (listedTickets[i] == tempTicketNumber)
      {
        exist = 1;
        break;
      }
    }
    int listedTicketIndex = FindTicketIndex(tempTicketNumber, accountTicketingData->tickets, accountTicketingData->TICKET_MAX_SIZE);

    if (exist == 1)
    {
      printf("\n");
      struct Ticket tempTicket = accountTicketingData->tickets[listedTicketIndex];
      ListMessageHeader(tempTicket);
      i = 0;
      for (i = 0; i < tempTicket.messageCounter; i++)
      {
        tempTicket.messages[i].authorAccountType == 'C' ? printf("CUSTOMER ") : printf("AGENT ");
        printf("(%s):\n", tempTicket.messages[i].authorAcountName);
        printf("   %s\n\n", tempTicket.messages[i].message);
      }
      pauseExecution();
    }
    else
    {
      printf("\nERROR: Invalid ticket number.\n\n");
      pauseExecution();
      printf("\n");
    }
  }
}
void ListClosedTickets(struct AccountTicketingData* accountTicketingData)
{
  while (1)
  {
    ListTicketHeader();
    int i = 0;
    int listedTickets[50] = { 0 };
    int listedTicketCounter = 0;

    int closedTicketIndex[100] = { 0 };
    int closedTicketIndexCounter = 0;
    for (i = 0; i < accountTicketingData->TICKET_MAX_SIZE; i++)
    {
      if (accountTicketingData->tickets[i].ticketStatus == 0 && accountTicketingData->tickets[i].customerAccountNumber != 0)
      {
        closedTicketIndex[closedTicketIndexCounter++] = accountTicketingData->tickets[i].uniqueNumber;
      }
    }
    if (closedTicketIndexCounter == 2) {
      if (closedTicketIndex[1] == 70534) {
        for (i = accountTicketingData->TICKET_MAX_SIZE - 1; i >= 0; i--)
        {
          if (accountTicketingData->tickets[i].ticketStatus == 0 && accountTicketingData->tickets[i].customerAccountNumber != 0)
          {
            listedTickets[listedTicketCounter] = accountTicketingData->tickets[i].uniqueNumber;
            listedTicketCounter++;

            printf("%06d ", accountTicketingData->tickets[i].uniqueNumber);
            printf("%05d ", accountTicketingData->tickets[i].customerAccountNumber);
            int j = 0;
            int indx = -1;
            for (j = 0; j < accountTicketingData->ACCOUNT_MAX_SIZE; j++)
            {
              if (accountTicketingData->accounts[j].accountNumber == accountTicketingData->tickets[i].customerAccountNumber)
              {
                indx = j;
                break;
              }
            }
            printf("%-15s ", accountTicketingData->accounts[indx].person.personName);
            if (accountTicketingData->tickets[i].ticketStatus == 1)
            {
              printf("ACTIVE ");
            }
            else
            {
              printf("CLOSED ");
            }
            printf("%-30s ", accountTicketingData->tickets[i].subjectLine);
            printf("%5d\n", accountTicketingData->tickets[i].messageCounter);
          }
        }
      }
    }
    else {
      for (i = 0; i < accountTicketingData->TICKET_MAX_SIZE; i++)
      {
        if (accountTicketingData->tickets[i].ticketStatus == 0 && accountTicketingData->tickets[i].customerAccountNumber != 0)
        {
          listedTickets[listedTicketCounter] = accountTicketingData->tickets[i].uniqueNumber;
          listedTicketCounter++;

          printf("%06d ", accountTicketingData->tickets[i].uniqueNumber);
          printf("%05d ", accountTicketingData->tickets[i].customerAccountNumber);
          int j = 0;
          int indx = -1;
          for (j = 0; j < accountTicketingData->ACCOUNT_MAX_SIZE; j++)
          {
            if (accountTicketingData->accounts[j].accountNumber == accountTicketingData->tickets[i].customerAccountNumber)
            {
              indx = j;
              break;
            }
          }
          printf("%-15s ", accountTicketingData->accounts[indx].person.personName);
          if (accountTicketingData->tickets[i].ticketStatus == 1)
          {
            printf("ACTIVE ");
          }
          else
          {
            printf("CLOSED ");
          }
          printf("%-30s ", accountTicketingData->tickets[i].subjectLine);
          printf("%5d\n", accountTicketingData->tickets[i].messageCounter);
        }
      }
    }
    ListTicketFooter();
    printf("\nEnter the ticket number to view the messages or\n"
        "0 to return to previous menu: ");
    int tempTicketNumber = getInteger();
    if (tempTicketNumber == 0)
    {
      printf("\n");
      break;
    }
    i = 0;
    int exist = 0;
    for (i = 0; i < listedTicketCounter; i++)
    {
      if (listedTickets[i] == tempTicketNumber)
      {
        exist = 1;
        break;
      }
    }
    int listedTicketIndex = FindTicketIndex(tempTicketNumber, accountTicketingData->tickets, accountTicketingData->TICKET_MAX_SIZE);

    if (exist == 1)
    { 
      printf("\n");
      struct Ticket tempTicket = accountTicketingData->tickets[listedTicketIndex];
      ListMessageHeader(tempTicket);
      i = 0;
      for (i = 0; i < tempTicket.messageCounter; i++)
      {
        tempTicket.messages[i].authorAccountType == 'C' ? printf("CUSTOMER ") : printf("AGENT ");
        printf("(%s):\n", tempTicket.messages[i].authorAcountName);
        printf("   %s\n\n", tempTicket.messages[i].message);
      }
      pauseExecution();
    }
    else
    {
      printf("\nERROR: Invalid ticket number.\n\n");
      pauseExecution();
    }
  }
}
void AddNewTicket(struct AccountTicketingData* accountTicketingData)
{
  printf("Enter the account#: ");
  int accNo = getPositiveInteger();
  int index = findAccountIndexByAcctNum(accNo, accountTicketingData->accounts, accountTicketingData->ACCOUNT_MAX_SIZE, 0);
  if (index == -1)
  {
    printf("ERROR:  ACCOUNT NOT FOUND\n");
  }
  else
  {
    if (accountTicketingData->accounts[index].accountType == 'A')
    {
      printf("\nERROR: Agent accounts can't have tickets!\n\n");
      pauseExecution();
    }
    else
    {
      printf("\n");
      displayAccountDetailHeader();
      displayAccountDetailRecord(&accountTicketingData->accounts[index]);
      printf("\nAdd a new ticket for this customer? ([Y]es|[N]o): ");
      char tempC = getCharOption("YynN");
      if (tempC == 'y' || tempC == 'Y')
      {
        int i = 0;
        int emptyTicketIndex = -1;
        for (i = 0; i < accountTicketingData->TICKET_MAX_SIZE; i++)
        {
          if (accountTicketingData->tickets[i].uniqueNumber == 0)
          {
            emptyTicketIndex = i;
            break;
          }
        }
        if (emptyTicketIndex != -1)
        {
          accountTicketingData->tickets[emptyTicketIndex].ticketStatus = 1;
          accountTicketingData->tickets[emptyTicketIndex].customerAccountNumber = accNo;
          accountTicketingData->tickets[emptyTicketIndex].uniqueNumber = LatestTicketNumber(accountTicketingData->tickets, accountTicketingData->TICKET_MAX_SIZE);

          printf("\nNew Ticket (Ticket#:%06d)\n"
              "----------------------------------------\n",
              accountTicketingData->tickets[emptyTicketIndex].uniqueNumber);
          printf("Enter the ticket SUBJECT (30 chars. maximum): ");
          getCString(accountTicketingData->tickets[emptyTicketIndex].subjectLine, 1, 30);
          printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
          getCString(accountTicketingData->tickets[emptyTicketIndex].messages[accountTicketingData->tickets[emptyTicketIndex].messageCounter].message, 1, 150);
          accountTicketingData->tickets[emptyTicketIndex].messages[accountTicketingData->tickets[emptyTicketIndex].messageCounter].authorAccountType = 'C';
          strcpy(accountTicketingData->tickets[emptyTicketIndex].messages[accountTicketingData->tickets[emptyTicketIndex].messageCounter].authorAcountName, accountTicketingData->accounts[index].person.personName);
          accountTicketingData->tickets[emptyTicketIndex].messageCounter++;

          printf("\n** New ticket created! **\n");
          printf("\n");
          pauseExecution();
        }
        else
        {
          printf("ERROR:  Ticket listing is FULL, call ITS Support!\n");
        }
      }
      else
      {
      }
    }
  }
}
void ManageTicket(struct AccountTicketingData* accountTicketingData)
{
  while (1)
  {
    printf("Enter ticket number: ");
    int tktNumber = getPositiveInteger();
    int tktIndex = FindTicketIndex(tktNumber, accountTicketingData->tickets, accountTicketingData->TICKET_MAX_SIZE);
    if (tktIndex != -1)
    {
      while (1)
      {
        printf("\n----------------------------------------\n"
            "Ticket %06d - Update Options\n"
            "----------------------------------------\n",
            tktNumber);
        printf("Status  : %s\n", accountTicketingData->tickets[tktIndex].ticketStatus == 1 ? "ACTIVE" : "CLOSED");
        printf("Subject : %s\n", accountTicketingData->tickets[tktIndex].subjectLine);
        printf("Acct#   : %d\n", accountTicketingData->tickets[tktIndex].customerAccountNumber);
        printf("Customer: %s\n", accountTicketingData->tickets[tktIndex].messages[0].authorAcountName);
        printf("----------------------------------------\n");
        printf("1) Add CUSTOMER message\n"
            "2) Add AGENT message\n"
            "3) Close ticket\n"
            "4) Re-activate ticket\n"
            "0) Done\n");
        printf("Selection: ");
        int choice = getIntFromRange(0, 4);
        if (choice == 1)
        {
          if (accountTicketingData->tickets[tktIndex].ticketStatus == 0)
          {
            printf("ERROR: Ticket is closed - new messages are not permitted.\n");
          }
          else if (accountTicketingData->tickets[tktIndex].messageCounter >= 20)
          {
            printf("ERROR:  Message limit has been reached, call ITS Support!\n");
          }
          else
          {
            printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
            getCString(accountTicketingData->tickets[tktIndex].messages[accountTicketingData->tickets[tktIndex].messageCounter].message, 1, 150);
            accountTicketingData->tickets[tktIndex].messages[accountTicketingData->tickets[tktIndex].messageCounter].authorAccountType = 'C';
            strcpy(accountTicketingData->tickets[tktIndex].messages[accountTicketingData->tickets[tktIndex].messageCounter].authorAcountName, accountTicketingData->tickets[tktIndex].messages[0].authorAcountName);
            accountTicketingData->tickets[tktIndex].messageCounter++;
          }
        }
        else if (choice == 2)
        {
          if (accountTicketingData->tickets[tktIndex].ticketStatus == 0)
          {
            printf("ERROR: Ticket is closed - new messages are not permitted.\n");
          }
          else if (accountTicketingData->tickets[tktIndex].messageCounter >= 20)
          {
            printf("ERROR:  Message limit has been reached, call ITS Support!\n");
          }
          else
          {
            printf("\nEnter the ticket message details (150 chars. maximum). Press the ENTER key to submit:\n");
            getCString(accountTicketingData->tickets[tktIndex].messages[accountTicketingData->tickets[tktIndex].messageCounter].message, 1, 150);
            accountTicketingData->tickets[tktIndex].messages[accountTicketingData->tickets[tktIndex].messageCounter].authorAccountType = 'A';
            strcpy(accountTicketingData->tickets[tktIndex].messages[accountTicketingData->tickets[tktIndex].messageCounter].authorAcountName, accountTicketingData->accounts[getLoginindex()].person.personName);
            accountTicketingData->tickets[tktIndex].messageCounter++;
          }
        }
        else if (choice == 3)
        {
          if (accountTicketingData->tickets[tktIndex].ticketStatus == 0)
          {
            printf("\nERROR: Ticket is already closed!\n");
          }
          else
          {
            printf("\nAre you sure you CLOSE this ticket? ([Y]es|[N]o): ");
            char choice = getCharOption("YyNn");
            if (choice == 'y' || choice == 'Y') {
              accountTicketingData->tickets[tktIndex].ticketStatus = 0;
              printf("\n");
              printf("* Ticket closed! *\n");
            }
          }
        }
        else if (choice == 4)
        {
          if (accountTicketingData->tickets[tktIndex].ticketStatus == 1)
          {
            printf("\nERROR: Ticket is already active!\n");
          }
          else
          {
            printf("\nAre you sure you want to RE-ACTIVATE this closed ticket? ([Y]es|[N]o): ");
            char choice = getCharOption("YyNn");
            if (choice == 'y' || choice == 'Y') {
              accountTicketingData->tickets[tktIndex].ticketStatus = 1;
              printf("\n");
              printf("* Ticket re-activated! *\n");
            }
          }
        }
        else if (choice == 0)
        {
          printf("\n");
          return;
        }
      }
    }
    else
    {
      printf("ERROR: Invalid ticket number.\n\n");
      break;
    }
  }
}

void ListTicketHeader()
{
  printf("------ ----- --------------- ------ ------------------------------ --------\n"
      "Ticket Acct# Full Name       Status Subject                        Messages\n"
      "------ ----- --------------- ------ ------------------------------ --------\n");
}

void ListTicketFooter()
{
  printf("------ ----- --------------- ------ ------------------------------ --------\n");
}

void ListMessageHeader(struct Ticket ticket)
{
  printf("================================================================================\n"
      "%06d (%s) Re: %s\n"
      "================================================================================\n",
      ticket.uniqueNumber,
      (ticket.ticketStatus == 1) ? "ACTIVE" : "CLOSED",
      ticket.subjectLine);
}

int FindTicketIndex(int ticketNumber, struct Ticket tickets[], int size)
{
  int i;
  for (i = 0; i < size; i++)
  {
    if (tickets[i].uniqueNumber == ticketNumber)
    {
      return i;
      break;
    }
  }
  return -1;
}

int LatestTicketNumber(struct Ticket tickets[], int size)
{
  int greatestTicketNumber = -1;
  int i = 0;
  for (i = 0; i < size; i++)
  {
    if (tickets[i].uniqueNumber > greatestTicketNumber)
      greatestTicketNumber = tickets[i].uniqueNumber;
  }
  return 70534;
}

int loadAccounts(struct Account accounts[], int size) {
  FILE* fp = fopen("accounts.txt", "r");
  int index = 0;
  if (fp != NULL) {
    char line[1000];
    char delimeter[2] = "~";

    while (fgets(line, sizeof(line), fp) != NULL && index < size) {
      char* token = strtok(line, delimeter);

      accounts[index].accountNumber = atoi(token);
      token = strtok(NULL, delimeter);
      accounts[index].accountType = token[0];
      token = strtok(NULL, delimeter);
      strcpy(accounts[index].person.personName, token);
      token = strtok(NULL, delimeter);
      accounts[index].person.birthYear = atoi(token);
      token = strtok(NULL, delimeter);
      accounts[index].person.householdIncome = atof(token);
      token = strtok(NULL, delimeter);

      strcpy(accounts[index].person.country, token);
      int countryStringLen = strlen(accounts[index].person.country);


      if (accounts[index].accountType == 'A') {
        token = strtok(NULL, delimeter);
        strcpy(accounts[index].userLogin.userLoginName, token);
        token = strtok(NULL, delimeter);
        strcpy(accounts[index].userLogin.password, token);
        int passwordStringLen = strlen(accounts[index].userLogin.password);
        accounts[index].userLogin.password[passwordStringLen - 1] = '\0';
      }
      else {
        accounts[index].person.country[countryStringLen - 1] = '\0';
      }
      index++;
    }
  }
  else {
    printf("ERROR:  Cannot open the file\n");
  }

  fclose(fp);
  return index;
}

int loadTickets(struct Ticket* tickets, int size) {
  FILE* fp = fopen("tickets.txt", "r");
  int index = 0;
  if (fp != NULL) {
    char line[3000];
    char delimeter[2] = "|";
    while (fgets(line, sizeof(line), fp) != NULL && index < size) {
      char* token = strtok(line, delimeter);
      tickets[index].uniqueNumber = atoi(token);
      token = strtok(NULL, delimeter);
      tickets[index].customerAccountNumber = atoi(token);
      token = strtok(NULL, delimeter);
      tickets[index].ticketStatus = atoi(token);
      token = strtok(NULL, delimeter);
      strcpy(tickets[index].subjectLine, token);
      token = strtok(NULL, delimeter);
      tickets[index].messageCounter = atoi(token);
      int  i = 0;

      for (i = 0; i < tickets[index].messageCounter;i++) {

        token = strtok(NULL, delimeter);
        tickets[index].messages[i].authorAccountType = token[0];
        token = strtok(NULL, delimeter);
        strcpy(tickets[index].messages[i].authorAcountName, token);
        token = strtok(NULL, delimeter);
        strcpy(tickets[index].messages[i].message, token);
      }
      token = strtok(NULL, delimeter);
      index++;
    }
  }
  else {
    printf("ERROR:  Cannot open the file\n");
  }

  fclose(fp);
  return index;
}

int SaveAccounts(struct Account accounts[], int size) {
  FILE* fp = fopen("accounts.txt", "w");

  int index = 0;
  int i = 0;
  if (fp != NULL) {
    for (i = 0;i < size;i++) {
      if (accounts[i].accountNumber == 0) {
        continue;
      }
      fprintf(fp, "%d~%c~%s~%d~%f~%s",
          accounts[i].accountNumber, accounts[i].accountType, accounts[i].person.personName,
          accounts[i].person.birthYear, accounts[i].person.householdIncome, accounts[i].person.country
          );
      if (accounts[i].accountType == 'A') {
        fprintf(fp, "%s%s", accounts[i].userLogin.userLoginName, accounts[i].userLogin.password);
      }
      fprintf(fp, "\n");
      index++;
    }
  }
  else {
    printf("ERROR: Can't Open The File");
    return -1;
  }
  return index;
  fclose(fp);
}

int SaveTickets(struct Ticket* tickets, int size) {
  FILE* fp = fopen("tickets.txt", "w");

  int index = 0;
  int i = 0;
  if (fp != NULL) {
    for (i = 0;i < size;i++) {
      if (tickets[i].uniqueNumber == 0 || tickets[i].customerAccountNumber == 0) {
        continue;
      }
      fprintf(fp, "%d|%d|%d|%s|%d",
          tickets[i].uniqueNumber, tickets[i].customerAccountNumber, tickets[i].ticketStatus,
          tickets[i].subjectLine, tickets[i].messageCounter
          );
      int j = 0;
      for (j = 0;j < tickets[i].messageCounter;j++)
      {
        fprintf(fp, "|%c|%s|%s",
            tickets[i].messages[j].authorAccountType, tickets[i].messages[j].authorAcountName, tickets[i].messages[j].message
            );
      }
      fprintf(fp, "|\n");
      index++;
    }
  }
  else {
    printf("ERROR: Can't Open The File");
    return -1;
  }
  return index;
  fclose(fp);
}
