#ifndef ACCOUNT_TICKETING_UI_H_
#define ACCOUNT_TICKETING_UI_H_

#include"account.h"
#include "ticket.h"

struct AccountTicketingData
{
  struct Account* accounts;
  const int ACCOUNT_MAX_SIZE;

  struct Ticket* tickets;
  const int TICKET_MAX_SIZE;
};

int loadAccounts(struct Account* accounts, int size);
int loadTickets(struct Ticket* tickets, int size);

void applicationStartup(struct AccountTicketingData* AccountTicketingData);
int menuLogin(const struct Account accounts[], int size);
void menuAgent(struct AccountTicketingData* accountTicketingData, const struct Account* account);
int findAccountIndexByAcctNum(int accNoToFind, const struct Account accounts[], int size, int isPrompt);
void displayAllAccountDetailRecords(const struct Account accounts[], int size);
void pauseExecution(void);
int CheckLoginCredentials(const struct Account accounts[], int size);

void displayAccountDetailHeader();
void displayAccountDetailRecord(const struct Account* account);

int getModifyMenuActive();
void setModifyMenuActive(int val);

int getUniqueAccountNumber();
void UpdateUniqueAccountNumber();

void ListNewTickets(struct AccountTicketingData* accountTicketingData);
void ListActiveTickets(struct AccountTicketingData* accountTicketingData);
void ListClosedTickets(struct AccountTicketingData* accountTicketingData);
void AddNewTicket(struct AccountTicketingData* accountTicketingData);
void ManageTicket(struct AccountTicketingData* accountTicketingData);
void ListTicketHeader();
void ListTicketFooter();
void ListMessageHeader(struct Ticket ticket);
int FindTicketIndex(int ticketNumber, struct Ticket tickets[], int size);
int LatestTicketNumber(struct Ticket tickets[], int size);
int SaveAccounts(struct Account accounts[], int size);
int SaveTickets(struct Ticket* tickets, int size);

int getLoginindex();

#endif
