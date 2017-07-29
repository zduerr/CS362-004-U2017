#include "dominion.h"
#include "rngs.h"
#include <stdio.h>
#include <stdlib.h>

struct gameState* refreshGameState(struct gameState *state) {
    if (state) {
        free(state);
    }
    state = (struct gameState *) malloc(sizeof(struct gameState));
    return state;
}

void printResult(int expected, int result) {
    printf(expected == result ? "PASS\n" : "FAIL\n");
}

int main() {
    printf("---TEST Smithy---\n");
    struct gameState *state = NULL;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
                 sea_hag, tribute, smithy};
    const int player = 0;

    printf("Test +3 Cards with only Smithy in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = smithy;
    state->handCount[player] = 1;
    playSmithy(player, 0, state);
    printResult(3, state->handCount[player]);

    printf("Test +3 Cards with multiple cards in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = smithy;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    playSmithy(player, 0, state);
    printResult(4, state->handCount[player]);

    printf("Test discard card that Smithy was discarded: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = gardens;
    state->hand[player][1] = smithy;
    state->numActions = 1;
    playSmithy(player, 1, state);
    printResult(1, state->hand[player][1] != smithy);


    printf("Test correct Smithy card was discarded with multiple in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = smithy;
    state->hand[player][1] = smithy;
    state->numActions = 1;
    playSmithy(player, 1, state);
    printResult(1, state->hand[player][2] != smithy);

}