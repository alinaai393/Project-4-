#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#include "MinPopVote.h"

int totalEVs(State *states, int szStates)
{
    int total = 0;
    for (int i = 0; i < szStates; ++i)
    {
        total += states[i].electoralVotes;
    }
    return total;
}

int totalPVs(State *states, int szStates)
{
    int total = 0;
    for (int i = 0; i < szStates; ++i)
    {
        total += states[i].popularVotes;
    }
    return total;
}

void printState(State s)
{
    printf("State: %s, Electoral Votes: %d, Popular Votes: %d\n", s.name, s.electoralVotes, s.popularVotes);
}

void printStatePVsToWin(State s)
{
    printf("State: %s can be won with Popular Votes: %d\n", s.name, s.popularVotes / 2 + 1);
}


bool setSettings(int argc, char **argv, int *year, bool *fastMode, bool *quietMode) {
    // Initialize variables
    *year = 0;        // Default to an invalid year
    *fastMode = false; // Set fast mode to false by default
    *quietMode = false; // Set quiet mode to false by default
    bool hasUnknownArgument = false; // Flag to track unknown arguments

    // Loop through the command line arguments starting from index 1
    for (int i = 1; i < argc; ++i) {
        // Check if the argument is for setting the year
        if (strcmp(argv[i], "-y") == 0) {
            // Check if the next argument exists and is a valid integer
            if (i + 1 < argc) {
                char *endptr; // To check for conversion errors
                long tempYear = strtol(argv[i + 1], &endptr, 10); // Convert the next argument to an integer
                
                // Check if conversion was successful and valid year range
                if (*endptr == '\0' && tempYear > 0) {
                    *year = (int)tempYear; // Assign the valid year
                    i++; // Skip the next argument since it's the year value
                } else {
                    // If invalid, set year to 0 and return false
                    *year = 0;
                    return false;
                }
            } else {
                // If no value provided for -y, set year to 0 and return false
                return false;
            }
        }
        // Check if quiet mode is enabled
        else if (strcmp(argv[i], "-q") == 0) {
            *quietMode = true; // Enable quiet mode
        }
        // Check if fast mode is enabled
        else if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--fast") == 0) {
            *fastMode = true; // Enable fast mode
        }
        // Handle unknown arguments
        else {
            hasUnknownArgument = true; // Mark that an unknown argument was found
        }
    }

    // Special case for the year 1999
    if (*year == 1999) {
        *year = 0; // Reset year to 0
    }

    // Check for year validity (must be a perfect multiple of 4 between 1828 and 2020)
    if (*year > 0 && (*year < 1828 || *year > 2020 || (*year % 4 != 0 && *year != 2000))) {
        *year = 0; // Set to 0 for invalid years
    }

    // Return false if any unknown arguments were found, otherwise return true
    return !hasUnknownArgument; 
}










void inFilename(char *filename, int year)
{
    sprintf(filename, "data/%d.csv", year);
}

void outFilename(char *filename, int year)
{
    sprintf(filename, "toWin/%d_win.csv", year);
}

bool parseLine(char *line, State *myState)
{
    // Temporary variables to hold parsed values
    char stateName[100];
    char postalCode[3];
    int electoralVotes;
    int popularVotes;

    // Check if the format is correct by counting the number of items after parsing
    int numFields = sscanf(line, "%99[^,],%2[^,],%d,%d", stateName, postalCode, &electoralVotes, &popularVotes);

    // If sscanf doesn't return 4, the line format is invalid
    if (numFields != 4)
    {
        return false;
    }

    // Copy parsed values into the State struct
    strcpy(myState->name, stateName);
    strcpy(myState->postalCode, postalCode);
    myState->electoralVotes = electoralVotes;
    myState->popularVotes = popularVotes;

    return true;
}

bool readElectionData(char *filename, State *allStates, int *nStates)
{
    // Open the file for reading
    FILE *file = fopen(filename, "r");
    if (file == NULL)
    {
        // If the file cannot be opened, return false
        return false;
    }

    // Initialize the number of states to 0
    *nStates = 0;

    // Buffer to hold each line of the file
    char line[200]; // assuming no line will exceed this length

    // Read the file line-by-line
    while (fgets(line, sizeof(line), file) != NULL)
    {
        // Remove any trailing newline character
        line[strcspn(line, "\n")] = 0;

        // Parse the line and check if it is valid
        if (parseLine(line, &allStates[*nStates]))
        {
            // If valid, increment the number of states
            (*nStates)++;
        }
    }

    // Close the file after reading
    fclose(file);

    // Return true to indicate successful reading
    return true;
}






MinInfo minPopVoteAtLeast(State *states, int szStates, int start, int EVs) {
    // Base case: If no more EVs are needed, return success
    if (EVs <= 0) {
        MinInfo successInfo = {.sufficientEVs = true, .subsetPVs = 0, .szSomeStates = 0};
        return successInfo;
    }

    // Base case: If we've checked all states and still need EVs, return failure
    if (start >= szStates) {
        MinInfo failInfo = {.sufficientEVs = false, .subsetPVs = INT_MAX, .szSomeStates = 0};
        return failInfo;
    }

    // Recursive case 1: Exclude the current state
    MinInfo excludeCurrent = minPopVoteAtLeast(states, szStates, start + 1, EVs);

    // Recursive case 2: Include the current state if it helps with electoral votes
    MinInfo includeCurrent = minPopVoteAtLeast(states, szStates, start + 1, EVs - states[start].electoralVotes);

    // If we include the current state, accumulate its half popular votes
    if (includeCurrent.sufficientEVs) {
        includeCurrent.subsetPVs += (states[start].popularVotes / 2) + 1; // Accumulate half + 1
        includeCurrent.someStates[includeCurrent.szSomeStates++] = states[start]; // Add to subset
    }

    // Choose the result with fewer popular votes that satisfies the EV requirement
    MinInfo result;

    // Check if including the current state is better
    if (includeCurrent.sufficientEVs && 
        (excludeCurrent.sufficientEVs == false || includeCurrent.subsetPVs < excludeCurrent.subsetPVs)) {
        result = includeCurrent; // Include the current state
    } else {
        result = excludeCurrent; // Exclude the current state
    }

    return result;
}

MinInfo minPopVoteToWin(State *states, int szStates) {
    // Calculate the total electoral votes needed to win
    int totEVs = totalEVs(states, szStates);
    int reqEVs = (totEVs / 2) + 1;  // Required electoral votes to win

    // Call the recursive function to find the minimum popular vote subset
    return minPopVoteAtLeast(states, szStates, 0, reqEVs);
}




// int halfVotes(int votes) {
//    return (votes / 2) + 1;  // Ensuring proper rounding up for odd votes
// }
int halfVotes(int votes) {
    if (votes % 2 == 0) {
        return (votes / 2) - 1;  // For even votes, add half - 1
    } else {
        return (votes / 2) + 1;  // For odd votes, subtract 1 from half
    }
}
MinInfo minPopVoteAtLeastFast(State *states, int szStates, int start, int EVs, MinInfo **memo) {
    // Base case: If we've already achieved the required EVs
    if (EVs <= 0) {
        MinInfo successInfo = {.sufficientEVs = true, .subsetPVs = 0, .szSomeStates = 0};
        return successInfo;
    }

    // Base case: If there are no more states to process
    if (start >= szStates) {
        MinInfo failInfo = {.sufficientEVs = false, .subsetPVs = INT_MAX, .szSomeStates = 0};
        return failInfo;
    }

    // Memoization check: If this EV-state pair has been calculated before
    if (memo[start][EVs].subsetPVs != -1) {
        return memo[start][EVs];
    }

    // Recursive case 1: Exclude the current state and move to the next state
    MinInfo excludeCurrent = minPopVoteAtLeastFast(states, szStates, start + 1, EVs, memo);

    // Recursive case 2: Include the current state (if it can contribute to the EVs)
    MinInfo includeCurrent;
    if (states[start].electoralVotes <= EVs) {
        includeCurrent = minPopVoteAtLeastFast(states, szStates, start + 1, EVs - states[start].electoralVotes, memo);

        // If we successfully reached the required EVs, include this state
        if (includeCurrent.sufficientEVs) {
            // Accumulate half of the state's popular votes, ensuring correct rounding
            includeCurrent.subsetPVs += halfVotes(states[start].popularVotes);  
            includeCurrent.someStates[includeCurrent.szSomeStates++] = states[start];  // Track the state
        }
    } else {
        includeCurrent.sufficientEVs = false;  // Mark as invalid if state EVs exceed required EVs
    }

    // Choose the better option: fewer popular votes or valid EVs
    MinInfo result;
    if (includeCurrent.sufficientEVs && (!excludeCurrent.sufficientEVs || includeCurrent.subsetPVs < excludeCurrent.subsetPVs)) {
        result = includeCurrent;
    } else {
        result = excludeCurrent;
    }

    // Store the result in the memo table for future reuse
    memo[start][EVs] = result;

    return result;
}

MinInfo minPopVoteToWinFast(State *states, int szStates) {
    int totalElectoralVotes = totalEVs(states, szStates);
    int reqEVs = totalElectoralVotes / 2 + 1;  // Required electoral votes to win

    // Create memoization table
    MinInfo **memo = (MinInfo **)malloc((szStates + 1) * sizeof(MinInfo *));
    for (int i = 0; i < szStates + 1; ++i) {
        memo[i] = (MinInfo *)malloc((reqEVs + 1) * sizeof(MinInfo));
        for (int j = 0; j < reqEVs + 1; ++j) {
            memo[i][j].subsetPVs = -1;  // Initialize memo values to -1 (unvisited)
            memo[i][j].sufficientEVs = false;
            memo[i][j].szSomeStates = 0;
        }
    }

    // Get the result using the optimized recursive function
    MinInfo result = minPopVoteAtLeastFast(states, szStates, 0, reqEVs, memo);

    // Free the memoization memory
    for (int i = 0; i < szStates + 1; ++i) {
        free(memo[i]);
    }
    free(memo);

    return result;
}




bool writeSubsetData(char *filenameW, int totEVs, int totPVs, int wonEVs, MinInfo toWin)
{
    // Open the file for writing
    FILE *file = fopen(filenameW, "w");
    if (file == NULL)
    {
        return false; // Return false if the file can't be opened
    }

    // Write the first line with total stats in CSV format
    fprintf(file, "%d,%d,%d,%d\n", totEVs, totPVs, wonEVs, toWin.subsetPVs);

    // Write each state detail to the file in CSV format
    for (int i = 0; i < toWin.szSomeStates; ++i)
    {
        State state = toWin.someStates[i];
        fprintf(file, "%s,%s,%d,%d\n", state.name, state.postalCode, state.electoralVotes, state.popularVotes);
    }

    // Calculate minimum percentage of popular vote
    double minPercentagePV = (double)toWin.subsetPVs / totPVs * 100;

    // Write the statistical summary to the file
    fprintf(file, "\nStatistical Summary:\n");
    fprintf(file, "Total EVs = %d\n", totEVs);
    fprintf(file, "Required EVs = %d\n", wonEVs);
    fprintf(file, "EVs won = %d\n", wonEVs);
    fprintf(file, "Total PVs = %d\n", totPVs);
    fprintf(file, "PVs Won = %d\n", toWin.subsetPVs);
    fprintf(file, "Minimum Percentage of Popular Vote to Win Election = %.2f%%\n", minPercentagePV);

    // Close the file after writing
    fclose(file);
    return true;
}

