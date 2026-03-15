#include <stdio.h>
#include <string.h>
#include "voting.h"

static Dish dishes[MAX_DISHES];
static int dishCount = 0;

// New variables for history tracking
static int history[MAX_HISTORY];
static int historyCount = 0;

void initializeMenu(const char *menu[], int size) {
    if (size > MAX_DISHES) size = MAX_DISHES;
    dishCount = size;
    for (int i = 0; i < dishCount; ++i) {
        dishes[i].id = i + 1;
        strncpy(dishes[i].name, menu[i], MAX_NAME_LEN - 1);
        dishes[i].name[MAX_NAME_LEN - 1] = '\0';
        dishes[i].votes = 0;
    }
}

void displayMenu(void) {
    printf("\n=== Mess Menu ===\n");
    for (int i = 0; i < dishCount; ++i) {
        printf("%d. %s\n", dishes[i].id, dishes[i].name);
    }
}

void addVote(int dishId) {
    if (dishId < 1 || dishId > dishCount) {
        printf("Invalid Dish ID!\n");
        return;
    }
    
    dishes[dishId - 1].votes++;
    
    if (historyCount < MAX_HISTORY) {
        history[historyCount] = dishId - 1; 
        historyCount++;
    }
    
    printf("Vote added for %s \n", dishes[dishId - 1].name);
}

void displayCounts(void) {
    printf("\n--- Current Vote Counts ---\n");
    for (int i = 0; i < dishCount; ++i) {
        printf("%d. %-20s : %d votes\n", dishes[i].id, dishes[i].name, dishes[i].votes);
    }
}

char* getWinner(void) {
    int maxVotes = -1;
    char *winner = NULL;
    for (int i = 0; i < dishCount; ++i) {
        if (dishes[i].votes > maxVotes) {
            maxVotes = dishes[i].votes;
            winner = dishes[i].name;
        }
    }
    return winner;
}

void resetVotes(void) {
    for (int i = 0; i < dishCount; ++i) {
        dishes[i].votes = 0;
    }
    historyCount = 0; 
    printf("All votes and history have been reset.\n");
}

void rankTopDishes(void) {
    Dish tempDishes[MAX_DISHES];
    Dish swap;
    
    for(int i = 0; i < dishCount; ++i) {
        tempDishes[i] = dishes[i];
    }

    for (int i = 0; i < dishCount - 1; ++i) {
        for (int j = 0; j < dishCount - i - 1; ++j) {
            if (tempDishes[j].votes < tempDishes[j + 1].votes) {
                swap = tempDishes[j];
                tempDishes[j] = tempDishes[j + 1];
                tempDishes[j + 1] = swap;
            }
        }
    }

    printf("\n--- Top Ranked Dishes ---\n");
    for (int i = 0; i < dishCount; ++i) {
        printf("Rank %d: %s (%d votes)\n", i + 1, tempDishes[i].name, tempDishes[i].votes);
    }
}

void viewHistory(void) {
    printf("\n--- Voting History ---\n");
    if (historyCount == 0) {
        printf("No votes cast yet.\n");
        return;
    }
    
    for (int i = 0; i < historyCount; ++i) {
        int dishIndex = history[i];
        printf("Vote %d: %s\n", i + 1, dishes[dishIndex].name);
    }
}
