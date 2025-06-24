#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_CANDIDATES 5
#define MAX_NAME_LEN 50
#define MAX_VOTERS 100

typedef struct {
    char name[MAX_NAME_LEN];
    int votes;
} Candidate;

typedef struct {
    int id;
    int hasVoted;
} Voter;

Candidate candidates[MAX_CANDIDATES];
Voter voters[MAX_VOTERS];
int totalCandidates = 0;
int totalVoters = 0;

// Admin credentials
const char ADMIN_USERNAME[] = "admin";
const char ADMIN_PASSWORD[] = "1234";

// Function to add candidates (Admin only)
void addCandidates() {
    printf("Enter number of candidates (max %d): ", MAX_CANDIDATES);
    scanf("%d", &totalCandidates);

    for (int i = 0; i < totalCandidates; i++) {
        printf("Enter name of candidate %d: ", i + 1);
        scanf("%s", candidates[i].name);
        candidates[i].votes = 0;
    }

    FILE *fp = fopen("candidates.txt", "w");
    for (int i = 0; i < totalCandidates; i++) {
        fprintf(fp, "%s %d\n", candidates[i].name, candidates[i].votes);
    }
    fclose(fp);
}

// Load candidates from file
void loadCandidates() {
    FILE *fp = fopen("candidates.txt", "r");
    if (fp == NULL) return;
    totalCandidates = 0;
    while (fscanf(fp, "%s %d", candidates[totalCandidates].name, &candidates[totalCandidates].votes) != EOF) {
        totalCandidates++;
    }
    fclose(fp);
}

// Save candidates to file
void saveCandidates() {
    FILE *fp = fopen("candidates.txt", "w");
    for (int i = 0; i < totalCandidates; i++) {
        fprintf(fp, "%s %d\n", candidates[i].name, candidates[i].votes);
    }
    fclose(fp);
}

// Check if voter already voted
int hasAlreadyVoted(int id) {
    for (int i = 0; i < totalVoters; i++) {
        if (voters[i].id == id) return voters[i].hasVoted;
    }
    return 0;
}

// Mark voter as voted
void markVoted(int id) {
    for (int i = 0; i < totalVoters; i++) {
        if (voters[i].id == id) {
            voters[i].hasVoted = 1;
            return;
        }
    }
    voters[totalVoters].id = id;
    voters[totalVoters].hasVoted = 1;
    totalVoters++;
}

// Cast vote
void castVote() {
    int id;
    printf("Enter your Voter ID (number): ");
    scanf("%d", &id);

    if (hasAlreadyVoted(id)) {
        printf("You have already voted!\n");
        return;
    }

    printf("\n--- Voting Panel ---\n");
    for (int i = 0; i < totalCandidates; i++) {
        printf("%d. %s\n", i + 1, candidates[i].name);
    }

    int choice;
    printf("Enter your choice (1-%d): ", totalCandidates);
    scanf("%d", &choice);

    if (choice >= 1 && choice <= totalCandidates) {
        candidates[choice - 1].votes++;
        markVoted(id);
        saveCandidates();
        printf("Vote casted successfully!\n");
    } else {
        printf("Invalid choice!\n");
    }
}

// Show results
void showResults() {
    printf("\n--- Voting Results ---\n");
    for (int i = 0; i < totalCandidates; i++) {
        printf("%s - %d votes\n", candidates[i].name, candidates[i].votes);
    }
}

// Show winner
void showWinner() {
    int maxVotes = -1, winnerIndex = -1;
    for (int i = 0; i < totalCandidates; i++) {
        if (candidates[i].votes > maxVotes) {
            maxVotes = candidates[i].votes;
            winnerIndex = i;
        }
    }

    printf("\n--- Winner ---\n");
    if (winnerIndex != -1)
        printf("%s wins with %d votes!\n", candidates[winnerIndex].name, maxVotes);
    else
        printf("No votes casted!\n");
}

// Admin login panel
int adminLogin() {
    char username[50], password[50];
    printf("Enter admin username: ");
    scanf("%s", username);
    printf("Enter password: ");
    scanf("%s", password);

    if (strcmp(username, ADMIN_USERNAME) == 0 && strcmp(password, ADMIN_PASSWORD) == 0) {
        return 1;
    } else {
        printf("Access denied.\n");
        return 0;
    }
}

int main() {
    int choice;
    loadCandidates();

    while (1) {
        printf("\n--- Mini Voting System ---\n");
        printf("1. Admin Panel\n");
        printf("2. Cast Vote\n");
        printf("3. Show Results\n");
        printf("4. Show Winner\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            if (adminLogin()) {
                addCandidates();
            }
            break;
        case 2:
            castVote();
            break;
        case 3:
            showResults();
            break;
        case 4:
            showWinner();
            break;
        case 5:
            exit(0);
        default:
            printf("Invalid choice!\n");
        }
    }

    return 0;
}
