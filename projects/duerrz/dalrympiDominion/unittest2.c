#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>


void printResult(int expected, int result) {
    printf(expected == result ? "PASS\n" : "FAIL\n");
}

int cardsMatch(int *expected, int *result, int n) {
    for (int i = 0; i < n; i++) {
        if (expected[i] != result[i]) {
            return 0;
        }
    }
    return 1;
}

int main() {
    int *result;
    const int n = 10;
    printf("---TEST kingdomCards---\n");

    printf("All different cards: ");
    int diffCards[] = {great_hall, minion, steward, adventurer, ambassador, cutpurse, embargo, outpost, salvager,
                       sea_hag};
    result = kingdomCards(great_hall, minion, steward, adventurer, ambassador, cutpurse, embargo, outpost, salvager,
                          sea_hag);
    printResult(1, cardsMatch(diffCards, result, n));
    free(result);

    printf("Sequential cards: ");
    int sequential[] = {great_hall, minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager,
                        sea_hag};
    result = kingdomCards(great_hall, minion, steward, tribute, ambassador, cutpurse, embargo, outpost, salvager,
                          sea_hag);
    printResult(1, cardsMatch(sequential, result, n));
    free(result);

    printf("All the same card: ");
    int same[] = {great_hall, great_hall, great_hall, great_hall, great_hall, great_hall, great_hall, great_hall,
                  great_hall, great_hall};
    result = kingdomCards(great_hall, great_hall, great_hall, great_hall, great_hall, great_hall, great_hall,
                          great_hall, great_hall, great_hall);
    printResult(1, cardsMatch(same, result, n));
    free(result);

    printf("The first and last card in the list: ");
    int firstLast[] = {curse, great_hall, great_hall, great_hall, great_hall, great_hall, great_hall, great_hall,
                       great_hall, treasure_map};
    result = kingdomCards(curse, great_hall, great_hall, great_hall, great_hall, great_hall, great_hall, great_hall,
                          great_hall, treasure_map);
    printResult(1, cardsMatch(firstLast, result, n));
    free(result);
}