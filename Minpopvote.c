#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "MinPopVote.h"

int totalEVs(State* states, int szStates) {
    int sum = 0;
    for(int i = 0; i < szStates; i++) {
        sum += states[i].electoralVotes;
    }
    return sum;
}

int totalPVs(State* states, int szStates) {
    int sum = 0;
    for(int i = 0; i < szStates; i++) {
        sum += states[i].popularVotes;
    }
    return sum;
}

bool isValidYear(int year) {
    return (year >= 1828 && year <= 2020 && year % 4 == 0);
}

bool setSettings(int argc, char** argv, int* year, bool* fastMode, bool* quietMode) {
    *year = 0;
    *fastMode = false;
    *quietMode = false;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-y") == 0) {
            if (i + 1 < argc) {
                char* nextArg = argv[i + 1];
                if (nextArg[0] != '-') {
                    int parsedYear = atoi(nextArg);
                    if (isValidYear(parsedYear)) {
                        *year = parsedYear;
                    } else {
                        *year = 0;
                    }
                    i++;
                } else {
                    *year = 0;
                }
            } else {
                return false;
            }
        } 
        else if (strcmp(argv[i], "-f") == 0) {
            if (!(i > 1 && strcmp(argv[i - 1], "-y") == 0)) {
                *fastMode = true;
            }
        } 
        else if (strcmp(argv[i], "-q") == 0) {
            if (!(i > 1 && strcmp(argv[i - 1], "-y") == 0)) {
                *quietMode = true;
            }
        } 
        else {
            return false;
        }
    }

    return true;
}


void inFilename(char* filename, int year) {
    sprintf(filename, "data/%d.csv", year);
}

void outFilename(char* filename, int year) {
    sprintf(filename, "toWin/%d_win.csv", year);
}

bool parseLine(char* line, State* myState) {
    char tempLine[256];
    strncpy(tempLine, line, 255);
    tempLine[255] = '\0';

    char* token = strtok(tempLine, ",");
    if(token == NULL) return false;
    strcpy(myState->name, token);

    token = strtok(NULL, ",");
    if(token == NULL) return false;
    strcpy(myState->postalCode, token);

    token = strtok(NULL, ",");
    if(token == NULL) return false;
    myState->electoralVotes = atoi(token);

    token = strtok(NULL, ",\n");
    if(token == NULL) return false;
    myState->popularVotes = atoi(token);

    token = strtok(NULL, ",");
    if(token != NULL) return false;

    return true;
}

bool readElectionData(char* filename, State* allStates, int* nStates) {
    *nStates = 0;

    FILE* fp = fopen(filename, "r");
    if(fp == NULL) {
        return false;
    }

    char line[256];
    while(fgets(line, sizeof(line), fp) != NULL) {
        if(strlen(line) <= 1) continue;

        State tempState;
        bool valid = parseLine(line, &tempState);
        if(valid) {
            if(*nStates < 51) {
                allStates[*nStates] = tempState;
                (*nStates)++;
            } else {
                fclose(fp);
                return false;
            }
        } else {
            fclose(fp);
            return false;
        }
    }

    fclose(fp);
    return true;
}

MinInfo minPopVoteAtLeast(State* states, int szStates, int start, int EVs) {
    MinInfo res;

    if (EVs <= 0) {
        res.szSomeStates = 0;
        res.subsetPVs = 0;
        res.sufficientEVs = true;
        return res;
    }

    if (start >= szStates) {
        res.szSomeStates = 0;
        res.subsetPVs = 0;
        res.sufficientEVs = false;
        return res;
    }

    MinInfo include = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);
    if(include.sufficientEVs) {
        include.subsetPVs += (states[start].popularVotes / 2) + 1;
        if(include.szSomeStates < 51) {
            include.someStates[include.szSomeStates] = states[start];
            include.szSomeStates++;
        }
    }

    MinInfo exclude = minPopVoteAtLeast(states, szStates, start + 1, EVs);

    if(include.sufficientEVs && exclude.sufficientEVs) {
        if(include.subsetPVs < exclude.subsetPVs) {
            res = include;
        } else {
            res = exclude;
        }
    } else if(include.sufficientEVs) {
        res = include;
    } else {
        res = exclude;
    }

    return res;
}

MinInfo minPopVoteToWin(State* states, int szStates) {
    int totEVs = totalEVs(states, szStates);
    int reqEVs = totEVs / 2 + 1;
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}

MinInfo minPopVoteAtLeastFast(State* states, int szStates, int start, int EVs, MinInfo** memo) {
    if (EVs <= 0) {
        MinInfo res;
        res.szSomeStates = 0;
        res.subsetPVs = 0;
        res.sufficientEVs = true;
        return res;
    }

    if (start >= szStates) {
        MinInfo res;
        res.szSomeStates = 0;
        res.subsetPVs = 0;
        res.sufficientEVs = false;
        return res;
    }

    if(EVs >= 0 && start < szStates && memo[start][EVs].subsetPVs != -1) {
        return memo[start][EVs];
    }

    MinInfo include = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);
    if(include.sufficientEVs) {
        include.subsetPVs += (states[start].popularVotes / 2) + 1;
        if(include.szSomeStates < 51) {
            include.someStates[include.szSomeStates] = states[start];
            include.szSomeStates++;
        }
    }

    MinInfo exclude = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);

    MinInfo res;
    if(include.sufficientEVs && exclude.sufficientEVs) {
        if(include.subsetPVs < exclude.subsetPVs) {
            res = include;
        } else {
            res = exclude;
        }
    } else if(include.sufficientEVs) {
        res = include;
    } else {
        res = exclude;
    }

    memo[start][EVs] = res;

    return res;
}

MinInfo minPopVoteToWinFast(State* states, int szStates) {
    int totEVs = totalEVs(states, szStates);
    int reqEVs = totEVs / 2 + 1;

    MinInfo** memo = (MinInfo**)malloc((szStates + 1) * sizeof(MinInfo*));
    if(memo == NULL) {
        MinInfo empty;
        empty.szSomeStates = 0;
        empty.subsetPVs = 0;
        empty.sufficientEVs = false;
        return empty;
    }
    for(int i = 0; i <= szStates; i++) {
        memo[i] = (MinInfo*)malloc((reqEVs + 1) * sizeof(MinInfo));
        if(memo[i] == NULL) {
            for(int k = 0; k < i; k++) {
                free(memo[k]);
            }
            free(memo);
            MinInfo empty;
            empty.szSomeStates = 0;
            empty.subsetPVs = 0;
            empty.sufficientEVs = false;
            return empty;
        }
        for(int j = 0; j <= reqEVs; j++) {
            memo[i][j].subsetPVs = -1;
            memo[i][j].szSomeStates = 0;
            memo[i][j].sufficientEVs = false;
        }
    }

    MinInfo res = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);

    for(int i = 0; i <= szStates; i++) {
        free(memo[i]);
    }
    free(memo);

    return res;
}

bool writeSubsetData(char* filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin) {
    FILE* fp = fopen(filenameW, "w");
    if(fp == NULL) {
        return false;
    }

    fprintf(fp, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);

    for(int i = toWin.szSomeStates - 1; i >= 0; i--) {
        int minPVs = (toWin.someStates[i].popularVotes / 2) + 1;
        fprintf(fp, "%s,%s,%d,%d\n", toWin.someStates[i].name, toWin.someStates[i].postalCode,
                toWin.someStates[i].electoralVotes, minPVs);
    }

    fclose(fp);
    return true;
}
