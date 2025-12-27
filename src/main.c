#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TEAMS 20
#define MAX_MATCHES 100
#define NAME_LEN 30

typedef struct {
    char name[NAME_LEN];
    int wins, losses, draws, points;
    int runsScored, runsConceded;
    double oversFaced, oversBowled;
} Team;

typedef struct {
    int t1, t2;
    int score1, score2;
    double overs1, overs2;
    int played;
} Match;

Team teams[MAX_TEAMS];
int numTeams = 0;
Match fixtures[MAX_MATCHES];
int numMatches = 0;

double calcNRR(Team *t) {
    double rrScored = (t->oversFaced > 0) ? (double)t->runsScored / t->oversFaced : 0;
    double rrConceded = (t->oversBowled > 0) ? (double)t->runsConceded / t->oversBowled : 0;
    return rrScored - rrConceded;
}

void addTeams() {
    printf("Enter number of teams: ");
    scanf("%d", &numTeams);

    for(int i = 0; i < numTeams; i++) {
        printf("Enter name of team %d: ", i + 1);
        scanf("%s", teams[i].name);

        teams[i].wins = teams[i].losses = teams[i].draws = teams[i].points = 0;
        teams[i].runsScored = teams[i].runsConceded = 0;
        teams[i].oversFaced = teams[i].oversBowled = 0.0;
    }
    printf("Teams added successfully.\n");
}

void generateFixtures() {
    numMatches = 0;
    for(int i = 0; i < numTeams; i++) {
        for(int j = i + 1; j < numTeams; j++) {
            fixtures[numMatches].t1 = i;
            fixtures[numMatches].t2 = j;
            fixtures[numMatches].played = 0;
            numMatches++;
        }
    }

    srand(time(NULL));
    for(int i = numMatches - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        Match temp = fixtures[i];
        fixtures[i] = fixtures[j];
        fixtures[j] = temp;
    }
    printf("Fixtures generated.\n");
}

void displayFixtures() {
    printf("\n----- Fixtures -----\n");

    for(int i = 0; i < numMatches; i++) {
        printf("Match %d: %s vs %s",
               i + 1,
               teams[fixtures[i].t1].name,
               teams[fixtures[i].t2].name);

        if(fixtures[i].played) {
            printf(" | Score: %d-%d",
                   fixtures[i].score1,
                   fixtures[i].score2);

            if(fixtures[i].score1 > fixtures[i].score2)
                printf(" | Winner: %s", teams[fixtures[i].t1].name);
            else if(fixtures[i].score2 > fixtures[i].score1)
                printf(" | Winner: %s", teams[fixtures[i].t2].name);
            else
                printf(" | Result: DRAW");
        } else {
            printf(" | Upcoming");
        }

        printf("\n");
    }
}

void updateMatch() {
    displayFixtures();

    int m;
    printf("\nEnter match number to update: ");
    scanf("%d", &m);
    m--;

    if(m < 0 || m >= numMatches || fixtures[m].played) {
        printf("Invalid or already played match.\n");
        return;
    }

    printf("Runs scored by %s: ", teams[fixtures[m].t1].name);
    scanf("%d", &fixtures[m].score1);
    printf("Overs faced by %s: ", teams[fixtures[m].t1].name);
    scanf("%lf", &fixtures[m].overs1);

    printf("Runs scored by %s: ", teams[fixtures[m].t2].name);
    scanf("%d", &fixtures[m].score2);
    printf("Overs faced by %s: ", teams[fixtures[m].t2].name);
    scanf("%lf", &fixtures[m].overs2);

    fixtures[m].played = 1;

    Team *t1 = &teams[fixtures[m].t1];
    Team *t2 = &teams[fixtures[m].t2];

    t1->runsScored += fixtures[m].score1;
    t1->runsConceded += fixtures[m].score2;
    t1->oversFaced += fixtures[m].overs1;
    t1->oversBowled += fixtures[m].overs2;

    t2->runsScored += fixtures[m].score2;
    t2->runsConceded += fixtures[m].score1;
    t2->oversFaced += fixtures[m].overs2;
    t2->oversBowled += fixtures[m].overs1;

    if(fixtures[m].score1 > fixtures[m].score2) {
        t1->wins++;
        t1->points += 2;
        t2->losses++;
    } else if(fixtures[m].score2 > fixtures[m].score1) {
        t2->wins++;
        t2->points += 2;
        t1->losses++;
    } else {
        t1->draws++;
        t2->draws++;
        t1->points++;
        t2->points++;
    }

    printf("Match updated successfully.\n");
}

int cmp(const void *a, const void *b) {
    Team *t1 = (Team*)a;
    Team *t2 = (Team*)b;

    if(t1->points != t2->points)
        return t2->points - t1->points;

    double nrr1 = calcNRR(t1);
    double nrr2 = calcNRR(t2);

    if(nrr2 > nrr1) return 1;
    if(nrr2 < nrr1) return -1;
    return 0;
}

void displayPoints() {
    Team temp[MAX_TEAMS];
    memcpy(temp, teams, sizeof(Team) * numTeams);

    qsort(temp, numTeams, sizeof(Team), cmp);

    printf("\nTeam\tW\tL\tD\tPts\tNRR\n");
    for(int i = 0; i < numTeams; i++) {
        printf("%s\t%d\t%d\t%d\t%d\t%.2lf\n",
               temp[i].name,
               temp[i].wins,
               temp[i].losses,
               temp[i].draws,
               temp[i].points,
               calcNRR(&temp[i]));
    }
}

void saveTournament() {
    FILE *f = fopen("tournament.txt", "w");
    if(!f) return;

    fprintf(f, "%d\n", numTeams);
    for(int i = 0; i < numTeams; i++) {
        fprintf(f, "%s %d %d %d %d %d %d %.1lf %.1lf\n",
                teams[i].name,
                teams[i].wins,
                teams[i].losses,
                teams[i].draws,
                teams[i].points,
                teams[i].runsScored,
                teams[i].runsConceded,
                teams[i].oversFaced,
                teams[i].oversBowled);
    }

    fprintf(f, "%d\n", numMatches);
    for(int i = 0; i < numMatches; i++) {
        fprintf(f, "%d %d %d %d %.1lf %.1lf %d\n",
                fixtures[i].t1,
                fixtures[i].t2,
                fixtures[i].score1,
                fixtures[i].score2,
                fixtures[i].overs1,
                fixtures[i].overs2,
                fixtures[i].played);
    }

    fclose(f);
    printf("Tournament saved.\n");
}

int loadTournament() {
    FILE *f = fopen("tournament.txt", "r");
    if(!f) return 0;

    fscanf(f, "%d", &numTeams);
    for(int i = 0; i < numTeams; i++) {
        fscanf(f, "%s %d %d %d %d %d %d %lf %lf",
               teams[i].name,
               &teams[i].wins,
               &teams[i].losses,
               &teams[i].draws,
               &teams[i].points,
               &teams[i].runsScored,
               &teams[i].runsConceded,
               &teams[i].oversFaced,
               &teams[i].oversBowled);
    }

    fscanf(f, "%d", &numMatches);
    for(int i = 0; i < numMatches; i++) {
        fscanf(f, "%d %d %d %d %lf %lf %d",
               &fixtures[i].t1,
               &fixtures[i].t2,
               &fixtures[i].score1,
               &fixtures[i].score2,
               &fixtures[i].overs1,
               &fixtures[i].overs2,
               &fixtures[i].played);
    }

    fclose(f);
    return 1;
}

int main() {
    int choice;

    if(!loadTournament()) {
        addTeams();
        generateFixtures();
    }

    while(1) {
        printf("\n1. Update Match");
        printf("\n2. Display Fixtures");
        printf("\n3. Points Table");
        printf("\n4. Save & Exit");
        printf("\nEnter choice: ");
        scanf("%d", &choice);

        switch(choice) {
            case 1: updateMatch(); break;
            case 2: displayFixtures(); break;
            case 3: displayPoints(); break;
            case 4: saveTournament(); return 0;
            default: printf("Invalid choice.\n");
        }
    }
}
