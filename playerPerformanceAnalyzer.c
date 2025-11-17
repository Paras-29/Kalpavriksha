#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "Players_data.h"

#define MAX_NAME 50
#define MAX_TEAM_NAME 50
#define MAX_ROLE_NAME 50
#define MIN_TEAM_ID 1
#define MAX_TEAM_ID 10
#define MIN_PLAYER 11
#define MAX_PLAYER 50

const float total_percent = 100.0f;

typedef struct PlayerNode {
    int playerId;
    char name[MAX_NAME];
    char teamName[MAX_NAME];
    char role[MAX_ROLE_NAME];
    int totalRuns;
    float battingAverage;
    float strikeRate;
    int wickets;
    float economyRate;
    float performanceIndex;
    struct PlayerNode *next;
} PlayerNode;

typedef struct {
    int teamId;
    char name[MAX_NAME];
    int totalPlayers;
    float avgBattingStrikeRate;
    PlayerNode *playersHead;
} Team;

Team teamList[MAX_TEAM_NAME];

bool stringCompare(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

void stringCopy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void safeStringCopy(char *dest, const char *src, int maxSize) {
    int i = 0;
    while (i < maxSize - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void swapTeams(Team *a, Team *b) {
    Team temp = *a;
    *a = *b;
    *b = temp;
}

void swapPlayersandPerformanceIndex(PlayerNode **a, PlayerNode **b) {
    PlayerNode *temp = *a;
    *a = *b;
    *b = temp;
}

void removeNewline(char *s) {
    int i = 0;
    while (s[i] != '\0') {
        if (s[i] == '\n') {
            s[i] = '\0';
            return;
        }
        i++;
    }
}

bool isValidTeamId(int id) {
    return id >= MIN_TEAM_ID && id <= MAX_TEAM_ID;
}

bool isValidTeamSize(int teamId) {
    int count = teamList[teamId - 1].totalPlayers;
    return count >= MIN_PLAYER && count <= MAX_PLAYER;
}


float calculatePerformance(PlayerNode *p) {
    if (stringCompare(p->role, "Batsman")){
        return (p->battingAverage * p->strikeRate) / total_percent;
    }
    else if (stringCompare(p->role, "Bowler")){
        return (p->wickets * 2) + (total_percent - p->economyRate);
    }
    else{
        return ((p->battingAverage * p->strikeRate) / total_percent) + (p->wickets * 2);
    } 
}

void initializeteamList() {
    static const char *teamNames[10] = {
        "India", "Australia", "England", "South Africa", "New Zealand",
        "Pakistan", "Sri Lanka", "Bangladesh", "West Indies", "Afghanistan"
    };

    for (int i = 0; i < 10; i++) {
        teamList[i].teamId = i + 1;
        stringCopy(teamList[i].name, teamNames[i]);
        teamList[i].name[MAX_NAME - 1] = '\0';
        teamList[i].totalPlayers = 0;
        teamList[i].avgBattingStrikeRate = 0.0f;
        teamList[i].playersHead = NULL;
    }
}

void insertPlayerIntoTeam(PlayerNode *newNode, int teamId) {
    if (!isValidTeamId(teamId)){
        printf("Invalid Team ID.\n");
        return;
    }
    newNode->next = teamList[teamId - 1].playersHead;
    teamList[teamId - 1].playersHead = newNode;
    teamList[teamId - 1].totalPlayers++;
}

bool isPlayerIdExistAcrossAllTeams(int playerId) {
    for (int t = 0; t < 10; t++) {
        PlayerNode *player = teamList[t].playersHead;
        while (player) {
            if (player->playerId == playerId){
                return true;
            } 
            player = player->next;
        }
    }
    return false;
}

void loadInitialPlayers() {
    int n = (int)(sizeof(players) / sizeof(players[0]));

    for (int i = 0; i < n; i++) {
        PlayerNode *node = (PlayerNode *)malloc(sizeof(PlayerNode));
        if (!node) { 
            printf("Memory Allocation Failed\n");
            return;
        }

        node->playerId = players[i].id;
        safeStringCopy(node->name, players[i].name, MAX_NAME);

        node->name[MAX_NAME - 1] = '\0';
        safeStringCopy(node->teamName, players[i].team, MAX_NAME);

        node->teamName[MAX_NAME - 1] = '\0';
        safeStringCopy(node->role, players[i].role, MAX_ROLE_NAME);

        node->role[sizeof(node->role) - 1] = '\0';
        node->totalRuns = players[i].totalRuns;
        node->battingAverage = players[i].battingAverage;
        node->strikeRate = players[i].strikeRate;
        node->wickets = players[i].wickets;
        node->economyRate = players[i].economyRate;
        node->performanceIndex = calculatePerformance(node);
        node->next = NULL;

        int teamIndex = -1;
        for (int t = 0; t < 10; t++) {
            if (stringCompare(players[i].team, teamList[t].name)) {
                teamIndex = t;
                break;
            }
        }

        if (teamIndex != -1) {
            insertPlayerIntoTeam(node, teamIndex + 1);
        } else {
            free(node);
        }
    }
}

void updateteamListStrikeRate(int teamId) {
    if (!isValidTeamId(teamId)){
        printf("Invalid Team ID.\n");
        return;
    }
    Team *team = &teamList[teamId - 1];

    PlayerNode *player = team->playersHead;
    float totalSR = 0.0f;
    int count = 0;

    while (player) {
        if (stringCompare(player->role, "Batsman") || stringCompare(player->role, "All-rounder")) {
            totalSR += player->strikeRate;
            count++;
        }
        player = player->next;
    }

    if (count > 0){
        team->avgBattingStrikeRate = totalSR / count;
    }
    else{
        team->avgBattingStrikeRate = 0.0f;
    }
}

void addNewPlayer() {
    int teamId;
    printf("Enter Team ID: ");
    scanf("%d", &teamId);
    while(getchar()!='\n'); 

    if (!isValidTeamId(teamId)){
        printf("Invalid Team ID.\n");
        return;
    }

    if (!isValidTeamSize(teamId)) {
        printf("Invalid Size: Players must be betwwen 11 to 50");
        return;   
    }

    PlayerNode *p = (PlayerNode *)malloc(sizeof(PlayerNode));
    if (!p) { 
        printf("Memory Allocation failed\n");
        return;
    }

    printf("Player ID: ");
    scanf("%d", &p->playerId);
    while(getchar()!='\n');

    if (isPlayerIdExistAcrossAllTeams(p->playerId)) {
        printf("Player ID %d already exists. Aborting add.\n", p->playerId);
        free(p);
        return;
    }

    printf("Name: ");
    fgets(p->name, MAX_NAME, stdin);
    removeNewline(p->name);

    printf("Role (1-Batsman,2-Bowler,3-All-rounder): ");
    int role;
    scanf("%d", &role);
    while(getchar()!='\n');

    if (role == 1) {
        stringCopy(p->role, "Batsman");
    }
    else if (role == 2) {
        stringCopy(p->role, "Bowler");
    }
    else {
        stringCopy(p->role, "All-rounder");
    }

    printf("Total Runs: "); scanf("%d", &p->totalRuns);
    printf("Batting Average: "); scanf("%f", &p->battingAverage);
    printf("Strike Rate: "); scanf("%f", &p->strikeRate);
    printf("Wickets: "); scanf("%d", &p->wickets);
    printf("Economy Rate: "); scanf("%f", &p->economyRate);

    safeStringCopy(p->teamName, teamList[teamId - 1].name, MAX_NAME);
    p->teamName[MAX_NAME - 1] = '\0';
    p->performanceIndex = calculatePerformance(p);
    p->next = NULL;

    insertPlayerIntoTeam(p, teamId);
    updateteamListStrikeRate(teamId);

    printf("Player added successfully to Team %s!\n", teamList[teamId - 1].name);
}

void displaySpecificTeamALLPlayers() {
    int id;
    printf("Enter Team ID: ");
    scanf("%d", &id);
    while(getchar()!='\n');

    if (!isValidTeamId(id)){
        printf("Invalid Team ID.\n");
        return;
    }

    updateteamListStrikeRate(id);

    Team *t = &teamList[id - 1];
    PlayerNode *p = t->playersHead;

    printf("\nPlayers of Team %s:\n", t->name);
    printf("=============================================\n");

    while (p) {
        printf("%-4d %-20s %-12s  PI: %.2f\n",p->playerId, p->name, p->role, p->performanceIndex);
        p = p->next;
    }

    printf("Total Players: %d\n", t->totalPlayers);
    printf("Avg Batting Strike Rate: %.2f\n", t->avgBattingStrikeRate);
}

void avgBattingStrikeRateSortList() {
    for (int i = 0; i < 10; i++){
        updateteamListStrikeRate(i + 1);
    }

    Team sortedTeams[10];
    for (int i = 0; i < 10; i++){
        sortedTeams[i] = teamList[i];
    }

    for (int i = 0; i < 9; i++) {
        for (int j = i + 1; j < 10; j++) {
            if (sortedTeams[j].avgBattingStrikeRate > sortedTeams[i].avgBattingStrikeRate) {
                swapTeams(&sortedTeams[i], &sortedTeams[j]);
            }
        }
    }

    printf("\nteamList Sorted by Avg Batting Strike Rate:\n");
    for (int i = 0; i < 10; i++){
        printf("%d  %-15s  SR: %.2f  Players: %d\n",
               sortedTeams[i].teamId, sortedTeams[i].name,
               sortedTeams[i].avgBattingStrikeRate, sortedTeams[i].totalPlayers);
    }
}

void displayTopKPlayer() {
    int teamId, roleChoice, K;

    printf("Enter Team ID: ");
    scanf("%d", &teamId);
    while(getchar()!='\n');

    if (!isValidTeamId(teamId)){
        printf("Invalid Team ID.\n");
        return;
    }

    printf("Role (1-Batsman,2-Bowler,3-All-rounder): ");
    scanf("%d", &roleChoice);
    while(getchar()!='\n'); 

    printf("Enter K: ");
    scanf("%d", &K);
    while(getchar()!='\n');

    char roleName[20];
    if (roleChoice == 1) {
        stringCopy(roleName, "Batsman");
    }
    else if (roleChoice == 2) {
        stringCopy(roleName, "Bowler");
    }
    else {
        stringCopy(roleName, "All-rounder");
    }

    PlayerNode *arr[200];
    int count = 0;

    PlayerNode *p = teamList[teamId - 1].playersHead;
    while (p) {
        if (stringCompare(p->role, roleName) && count < 200){
            arr[count++] = p;
        }
        p = p->next;
    }

    for (int i = 0; i < K && i < count; i++) {
        int maxIdx = i;
        for (int j = i + 1; j < count; j++) {
            if (arr[j]->performanceIndex > arr[maxIdx]->performanceIndex){
                maxIdx = j;
            }
        }
        swapPlayersandPerformanceIndex(&arr[i], &arr[maxIdx]);
    }

    printf("\nTop %d %s of Team %s:\n", K, roleName, teamList[teamId - 1].name);
    for (int i = 0; i < K && i < count; i++){
        printf("%-4d %-20s PI: %.2f\n", arr[i]->playerId, arr[i]->name, arr[i]->performanceIndex);
    }
}

void displaySpecificRolePlayer() {
    int choice;
    printf("Role (1-Batsman,2-Bowler,3-All-rounder): ");
    scanf("%d", &choice);
    while(getchar()!='\n');

    char roleName[20];
    if (choice == 1) {
        stringCopy(roleName, "Batsman");
    }
    else if (choice == 2) {
        stringCopy(roleName, "Bowler");
    }
    else {
        stringCopy(roleName, "All-rounder");
    }

    PlayerNode *list[500];
    int count = 0;

    for (int t = 0; t < 10; t++) {
        PlayerNode *player = teamList[t].playersHead;
        while (player) {
            if (stringCompare(player->role, roleName) && count < 500){
                list[count++] = player;
            }
            player = player->next;
        }
    }

    for (int i = 0; i < count; i++) {
        for (int j = i + 1; j < count; j++) {
            if (list[j]->performanceIndex > list[i]->performanceIndex) {
                swapPlayersandPerformanceIndex(&list[i], &list[j]);
            }
        }
    }

    printf("\nAll %s across all teamList (sorted by PI desc):\n", roleName);
    for (int i = 0; i < count; i++){
        printf("%-4d %-20s %-12s PI: %.2f\n",list[i]->playerId, list[i]->name, list[i]->teamName, list[i]->performanceIndex);
    }
}

void updateAllTeamsStrikeRate() {
    for (int t = MIN_TEAM_ID; t <= MAX_TEAM_ID; t++){
        updateteamListStrikeRate(t);
    }
}

void freeAllPlayers() {
    for (int t = 0; t < 10; t++) {
        PlayerNode *cur = teamList[t].playersHead;
        while (cur) {
            PlayerNode *next = cur->next;
            free(cur);
            cur = next;
        }
        teamList[t].playersHead = NULL;
        teamList[t].totalPlayers = 0;
    }
}

void printMenu(){
    printf("\n=============================\n");
    printf(" ICC ODI Player Analyzer\n");
    printf("=============================\n");
    printf("1. Add Player\n");
    printf("2. Display Team Players\n");
    printf("3. Display teams by Strike Rate\n");
    printf("4. Top K Players of a Team\n");
    printf("5. Display Players by Role\n");
    printf("6. Exit\n");
}

void initialiInitialData(){

    initializeteamList();
    loadInitialPlayers();
    updateAllTeamsStrikeRate();
}

void runPlayerPerformanceAnalyzer(){
    initialiInitialData();
    
    int choice;
    while (1) {
        printMenu();

        printf("Enter choice: ");
        scanf("%d", &choice); 
        while(getchar()!='\n'); 

        switch (choice) {
            case 1: 
                addNewPlayer(); 
                break;
            case 2:    
                displaySpecificTeamALLPlayers(); 
                break;
            case 3: 
                avgBattingStrikeRateSortList(); 
                break;
            case 4: 
                displayTopKPlayer(); 
                break;
            case 5: 
                displaySpecificRolePlayer(); 
                break;
            case 6: 
                printf("Existing Program...");
                freeAllPlayers();
                return;
            default: 
                printf("Invalid choice!\n");
        }
    }
}

int main() {
    runPlayerPerformanceAnalyzer();
    return 0;
}
