#include <stdio.h>
#include <stdlib.h>
#include "voting.h"

int main(void) {
    const char *menu[] = {
        "Pasta",
        "Pizza",
        "Burger",
        "Kati Roll",
        "Fried Rice",
        "Noodles",
        "Chole Bhature"
    };
    int menuSize = sizeof(menu) / sizeof(menu[0]);
    int choice, dishId;

    initializeMenu(menu, menuSize);

    while (1) {
        printf("\n=== Mess Menu Voting System ===\n");
        printf("1. View Menu\n");
        printf("2. Add Vote (by Dish ID)\n");
        printf("3. Display Vote Counts\n");
        printf("4. Display Winner\n");
        printf("5. Rank Top Dishes\n");
        printf("6. View Voting History\n");
        printf("7. Reset Votes\n");
        printf("8. Exit\n");
        printf("Enter your choice: ");

        if (scanf("%d", &choice) != 1) {
            int c;
            while ((c = getchar()) != '\n' && c != EOF) {}
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        switch (choice) {
            case 1:
                displayMenu();
                break;

            case 2:
                displayMenu();
                printf("Enter Dish ID to vote: ");
                if (scanf("%d", &dishId) != 1) {
                    int c;
                    while ((c = getchar()) != '\n' && c != EOF) {}
                    printf("Invalid input. Vote cancelled.\n");
                } else {
                    addVote(dishId);
                }
                break;

            case 3:
                displayCounts();
                break;

            case 4: {
                char *winner = getWinner();
                if (winner)
                    printf("\n Winning Dish: %s \n", winner);
                else
                    printf("No votes yet!\n");
                break;
            }

            case 5:
                rankTopDishes();
                break;

            case 6:
                viewHistory();
                break;

            case 7:
                resetVotes();
                break;

            case 8:
                printf("Exiting..\n");
                return 0;

            default:
                printf("Invalid choice! Try again.\n");
        }
    }

    return 0;
}
