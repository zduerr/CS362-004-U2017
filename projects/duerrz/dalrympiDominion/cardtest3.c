#include "dominion.h"
#include "dominion_helpers.h"
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
    printf("---TEST Great Hall---\n");
    struct gameState *state = NULL;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
                 sea_hag, great_hall, smithy};
    const int player = 0;

    printf("Test +1 Card with only Great Hall in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = great_hall;
    state->handCount[player] = 1;
    cardEffect(great_hall, 0, 0, 0, state, 0, 0);
    printResult(1, state->handCount[player]);

    printf("Test +1 Card with multiple cards in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = great_hall;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    cardEffect(great_hall, 0, 0, 0, state, 0, 0);
    printResult(2, state->handCount[player]);

    printf("Test +1 Action with zero actions left: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = great_hall;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    state->numActions = 0;
    cardEffect(great_hall, 0, 0, 0, state, 0, 0);
    printResult(1, state->numActions);

    printf("Test +1 Action with 1 action left: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = great_hall;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    state->numActions = 1;
    cardEffect(great_hall, 0, 0, 0, state, 0, 0);
    printResult(2, state->numActions);

    printf("Test +1 Actions with multiple action left: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = great_hall;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    state->numActions = 3;
    cardEffect(great_hall, 0, 0, 0, state, 0, 0);
    printResult(4, state->numActions);

}