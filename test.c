#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h> 
#include "MinPopVote.h"

bool test_totalEVs()
{
    State aStates[10];
    int res;

    // Set up test data
    aStates[0].electoralVotes = 5;
    aStates[1].electoralVotes = 8;
    aStates[2].electoralVotes = 12;
    aStates[3].electoralVotes = 6;
    aStates[4].electoralVotes = 7;
    aStates[5].electoralVotes = 10;

    // Test for 5 states
    printf("Checking totalEVs() for 5 States:\n");
    res = totalEVs(aStates, 5);
    if (res != 38)
    {
        printf("  individual state EVs are 5, 8, 12, 6, 7\n");
        printf("  expected total EVs = 38, actual total EVs = %d\n", res);
        return false;
    }

    // Test for 6 states
    printf("Checking totalEVs() for 6 States:\n");
    res = totalEVs(aStates, 6);
    if (res != 48)
    {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10\n");
        printf("  expected total EVs = 48, actual total EVs = %d\n", res);
        return false;
    }

    // Additional test cases
    aStates[6].electoralVotes = 20;
    printf("Checking totalEVs() for 7 States:\n");
    res = totalEVs(aStates, 7);
    if (res != 68)
    {
        printf("  individual state EVs are 5, 8, 12, 6, 7, 10, 20\n");
        printf("  expected total EVs = 68, actual total EVs = %d\n", res);
        return false;
    }

    return true;
}

bool test_totalPVs()
{
    State aStates[10];
    int res;

    // Set up test data
    aStates[0].popularVotes = 1000000;
    aStates[1].popularVotes = 500000;
    aStates[2].popularVotes = 2000000;
    aStates[3].popularVotes = 1500000;
    aStates[4].popularVotes = 2500000;
    aStates[5].popularVotes = 300000;

    // Test for 5 states
    printf("Checking totalPVs() for 5 States:\n");
    res = totalPVs(aStates, 5);
    if (res != 7000000)
    {
        printf("  individual state PVs are 1000000, 500000, 2000000, 1500000, 2500000\n");
        printf("  expected total PVs = 7000000, actual total PVs = %d\n", res);
        return false;
    }

    // Test for 6 states
    printf("Checking totalPVs() for 6 States:\n");
    res = totalPVs(aStates, 6);
    if (res != 7300000)
    {
        printf("  individual state PVs are 1000000, 500000, 2000000, 1500000, 2500000, 300000\n");
        printf("  expected total PVs = 7300000, actual total PVs = %d\n", res);
        return false;
    }

    return true;
}

















bool test_setSettings()
{
    int year;
    bool fastMode;
    bool quietMode;

    // Test 1: Valid input with all arguments
    char *args1[] = {"./app.exe", "-f", "-q", "-y", "2020"};
    bool result1 = setSettings(5, args1, &year, &fastMode, &quietMode);
    assert(result1 && year == 2020 && fastMode == true && quietMode == true);
    printf("Test 1 passed!\n");

    // Test 2: Valid input without -f and -q
    char *args2[] = {"./app.exe", "-y", "2016"};
    bool result2 = setSettings(3, args2, &year, &fastMode, &quietMode);
    assert(result2 && year == 2016 && fastMode == false && quietMode == false);
    printf("Test 2 passed!\n");

    // // Test 3: Invalid year (not a leap year)
    // char *args3[] = {"./app.exe", "-y", "2023"}; // This year is invalid
    // bool result3 = setSettings(3, args3, &year, &fastMode, &quietMode);
    // assert(!result3 && year == 0 && fastMode == false && quietMode == false);
    // printf("Test 3 passed!\n");

    // Test 4: Valid input with only -f (fast mode)
    char *args4[] = {"./app.exe", "-f"};
    bool result4 = setSettings(2, args4, &year, &fastMode, &quietMode);
    assert(result4 && year == 0 && fastMode == true && quietMode == false);
    printf("Test 4 passed!\n");

    // Test 5: Valid input with only -q (quiet mode)
    char *args5[] = {"./app.exe", "-q"};
    bool result5 = setSettings(2, args5, &year, &fastMode, &quietMode);
    assert(result5 && year == 0 && fastMode == false && quietMode == true);
    printf("Test 5 passed!\n");

    // Test 6: Invalid argument (-v is invalid)
    char *args6[] = {"./app.exe", "-v", "-y", "2020"};
    bool result6 = setSettings(4, args6, &year, &fastMode, &quietMode);
    assert(!result6); // Should return false due to invalid argument
    printf("Test 6 passed!\n");





   char *args7[] = {"./app.exe", "-y"};
bool result7 = setSettings(2, args7, &year, &fastMode, &quietMode);

// Ensure that the function returns false due to the missing year
assert(!result7);

// Ensure that year is set to 0
assert(year == 0);

// Ensure that fastMode and quietMode remain unchanged (default)
assert(fastMode == false);
assert(quietMode == false);

printf("Test 7 passed!\n");





 // Test 8: Valid year (specifically test for 2000)
char *args8[] = {"./app.exe", "-y", "2000", "-f"}; // This year is valid
bool result8 = setSettings(4, args8, &year, &fastMode, &quietMode);
assert(result8 && year == 2000 && fastMode == true && quietMode == false);
printf("Test 8 passed!\n");

    // Test 9: No arguments, all defaults
    char *args9[] = {"./app.exe"};
    bool result9 = setSettings(1, args9, &year, &fastMode, &quietMode);
    assert(result9 && year == 0 && fastMode == false && quietMode == false);
    printf("Test 9 passed!\n");
   

    return true; // Return true if all tests passed
}


















bool test_inFilename()
{
    char filename[100];
    int year = 2020;

    // Call the inFilename function
    inFilename(filename, year);

    // Check if the filename is correct
    if (strcmp(filename, "data/2020.csv") != 0)
    {
        printf("Test failed! Expected 'data/2020.csv', got '%s'\n", filename);
        return false;
    }

    printf("Test passed for inFilename!\n");
    return true;
}

bool test_outFilename()
{
    char filename[100];
    int year = 2020;

    // Call the outFilename function
    outFilename(filename, year);

    // Check if the filename is correct
    if (strcmp(filename, "toWin/2020_win.csv") != 0)
    {
        printf("Test failed! Expected 'toWin/2020_win.csv', got '%s'\n", filename);
        return false;
    }

    printf("Test passed for outFilename!\n");
    return true;
}

bool test_parseLine()
{
    // Test case 1: Valid input line
    char validLine[] = "Illinois,IL,20,6033744";
    State testState;
    bool result = parseLine(validLine, &testState);

    if (!result)
    {
        printf("Test 1 failed: valid line was not parsed correctly.\n");
        return false;
    }
    if (strcmp(testState.name, "Illinois") != 0 ||
        strcmp(testState.postalCode, "IL") != 0 ||
        testState.electoralVotes != 20 ||
        testState.popularVotes != 6033744)
    {
        printf("Test 1 failed: incorrect values parsed.\n");
        return false;
    }

    // Test case 2: Valid line with newline character
    char validLineWithNewline[] = "California,CA,55,17214326\n";
    result = parseLine(validLineWithNewline, &testState);

    if (!result)
    {
        printf("Test 2 failed: valid line with newline was not parsed correctly.\n");
        return false;
    }
    if (strcmp(testState.name, "California") != 0 ||
        strcmp(testState.postalCode, "CA") != 0 ||
        testState.electoralVotes != 55 ||
        testState.popularVotes != 17214326)
    {
        printf("Test 2 failed: incorrect values parsed.\n");
        return false;
    }

    // Test case 3: Invalid line with missing fields
    char invalidLine[] = "Texas,TX,38";
    result = parseLine(invalidLine, &testState);

    if (result)
    {
        printf("Test 3 failed: invalid line was incorrectly parsed as valid.\n");
        return false;
    }

    // Test case 4: Invalid line with extra commas
    char extraCommasLine[] = "New York,NY,29,8398692,extra";
    result = parseLine(extraCommasLine, &testState);

    if (result)
    {
        printf("Test 4 failed: line with extra commas was incorrectly parsed as valid.\n");
        return false;
    }

    printf("All test cases passed for parseLine().\n");
    return true;
}

bool test_readElectionData()
{
    State allStates[51];
    int nStates;
    char filename[] = "data/2020.csv";

    printf("Checking readElectionData():\n");
    if (!readElectionData(filename, allStates, &nStates))
    {
        printf("Failed to read data from file: %s\n", filename);
        return false;
    }

    printf("Successfully read %d states\n", nStates);

    return nStates > 0;
}









bool test_minPVsSlow()
{
    // Test Case: A small set of states
    State testStates[4] = {
        {"StateA", "A", 5, 60},  // EVs: 5, PVs: 60
        {"StateB", "B", 2, 20},  // EVs: 2, PVs: 20
        {"StateC", "C", 8, 70},  // EVs: 8, PVs: 70
        {"StateD", "D", 3, 30}   // EVs: 3, PVs: 30
    };

    // Expected Result:
    // Minimum subset of states that gives enough electoral votes (>= 9) is:
    // StateB + StateD (2 EVs + 3 EVs) -> Popular Votes = 47
    MinInfo expected;
    expected.subsetPVs = 47;
    expected.szSomeStates = 2;
    expected.sufficientEVs = true;

    // Call the function
    MinInfo result = minPopVoteToWin(testStates, 4);

    // Print and check results
    printf("Test minPopVoteToWin():\n");
    printf("Expected Total PVs: %d, Actual Total PVs: %d\n", expected.subsetPVs, result.subsetPVs);
    printf("Expected Number of States: %d, Actual Number of States: %d\n", expected.szSomeStates, result.szSomeStates);
    printf("Expected Sufficient EVs: %d, Actual Sufficient EVs: %d\n", expected.sufficientEVs, result.sufficientEVs);

    // Validate the results
    bool passed = true;
    if (result.subsetPVs != expected.subsetPVs)
    {
        printf("Total PVs mismatch!\n");
        passed = false;
    }
    if (result.szSomeStates != expected.szSomeStates)
    {
        printf("Number of States mismatch!\n");
        passed = false;
    }
    if (result.sufficientEVs != expected.sufficientEVs)
    {
        printf("Sufficient EVs mismatch!\n");
        passed = false;
    }

    // Optionally, print the subset of states
    printf("Subset of states in the result:\n");
    for (int i = 0; i < result.szSomeStates; i++)
    {
        printState(result.someStates[i]);
    }

    return passed;
}








bool test_minPVsFast()
{
    // Set up a small array of States for testing
    State states[] = {
        {"Alabama", "AL", 9, 1161642},
        {"Alaska", "AK", 3, 179766},
        {"Arizona", "AZ", 11, 1693664},
        {"Arkansas", "AR", 6, 609535}
        // Add more states as needed
    };
    int szStates = sizeof(states) / sizeof(states[0]);

    // Define required electoral votes to win
    int requiredEVs = 270;

    // Call the optimized function
    MinInfo result = minPopVoteAtLeastFast(states, szStates, 0, requiredEVs, NULL); // Ensure you pass the correct parameters

    // Check results and assert expected values
    // Here, replace the expected values with your actual expected results.
    int expectedTotalPVs = 1161642 + 609535;                        // Example calculation, adjust according to your logic
    int expectedTotalEVs = 9 + 6;                                   // Example calculation, adjust according to your logic
    bool expectedSufficientEVs = (expectedTotalEVs >= requiredEVs); // Check if it meets the requirement

    // Validate the results using assertions
    assert(result.subsetPVs == expectedTotalPVs);          // Check if the total PVs is as expected
    assert(result.szSomeStates == 2);                      // Check if the expected size of the subset is correct
    assert(result.sufficientEVs == expectedSufficientEVs); // Check if it has sufficient EVs

    // Print results for visual confirmation (optional)
    printf("Test Results:\n");
    printf("Total Popular Votes: %d (Expected: %d)\n", result.subsetPVs, expectedTotalPVs);
    printf("Total States in Subset: %d (Expected: %d)\n", result.szSomeStates, 2); // Adjust based on your expectations
    printf("Sufficient Electoral Votes: %s (Expected: %s)\n",
           result.sufficientEVs ? "true" : "false", expectedSufficientEVs ? "true" : "false");

    // If all assertions pass, return true
    return true;
}

int main()
{
    printf("Welcome to the Popular Vote Minimizer Testing Suite!\n\n");

    printf("Testing totalEVs()...\n");
    if (test_totalEVs())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing totalPVs()...\n");
    if (test_totalPVs())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing setSettings()...\n");
    if (test_setSettings())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing inFilename()...\n");
    if (test_inFilename())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing outFilename()...\n");
    if (test_outFilename())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing parseLine()...\n");
    if (test_parseLine())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing readElectionData()...\n");
    if (test_readElectionData())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWin()...\n");
    if (test_minPVsSlow())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    printf("Testing minPopVoteToWinFast()...\n");
    if (test_minPVsFast())
    {
        printf("  All tests PASSED!\n");
    }
    else
    {
        printf("  test FAILED.\n");
    }

    return 0;
}
