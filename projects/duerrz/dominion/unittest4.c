#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

void printResult(int expected, int result) {
    printf(expected == result ? "PASS\n" : "FAIL\n");
}

int main() {
    int cards[] = {curse, estate, duchy, province, copper, silver, gold, adventurer, council_room, feast, gardens, mine,
                   remodel, smithy, village, baron, great_hall, minion, steward, tribute, ambassador, cutpurse, embargo,
                   outpost, salvager, sea_hag, treasure_map};
    char* names [] = {"curse", "estate", "duchy", "province", "copper", "silver", "gold", "adventurer", "council_room", "feast", "gardens", "mine",
                      "remodel", "smith", "village", "baron", "great_hall", "minion", "steward", "tribute", "ambassador", "cutpurse", "embargo",
                      "outpost", "salvager", "sea_hag", "treasure_map"};
    int expected[] = {0, 2, 5, 8, 0, 3, 6, 6, 5, 4, 4, 5, 4, 4, 3, 4, 3, 5, 3, 5, 3, 4, 2, 5, 4, 4, 4};
    int n = 27;
    printf("---TEST getCost---\n");
    printf("Each card in the deck returns the proper value cost: \n");
    for (int i = 0; i < n; i++){
        printf(names[i]);
        printf(": ");
        printResult(expected[i], getCost(cards[i]));
    }
}