#ifndef VOTING_H
#define VOTING_H

#define MAX_DISHES 10
#define MAX_NAME_LEN 50
#define MAX_HISTORY 100

typedef struct {
    int id;
    char name[MAX_NAME_LEN];
    int votes;
} Dish;

void initializeMenu(const char *menu[], int size);
void displayMenu(void);
void addVote(int dishId);
void displayCounts(void);
char* getWinner(void);
void resetVotes(void);
void rankTopDishes(void);
void viewHistory(void);

#endif
