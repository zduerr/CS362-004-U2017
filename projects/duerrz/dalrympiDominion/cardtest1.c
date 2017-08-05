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

int playVillage(int player, int handPos, struct gameState* state){
    return fVillage(state, player, handPos);
}

//int cardEffect(int card, int choice1, int choice2, int choice3, struct gameState *state, int handPos, int *bonus)
int main() {
    printf("---TEST Village---\n");
    struct gameState *state = NULL;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
                 sea_hag, tribute, smithy};
    const int player = 0;

    printf("Test +1 Card with only Village in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = village;
    state->handCount[player] = 1;
    playVillage(player, 0, state);
    printResult(1, state->handCount[player]);

    printf("Test +1 Card with multiple cards in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = village;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    playVillage(player, 0, state);
    printResult(2, state->handCount[player]);

    printf("Test +2 Actions with zero actions left: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = village;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    state->numActions = 0;
    playVillage(player, 0, state);
    printResult(2, state->numActions);

    printf("Test +2 Actions with 1 action left: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = village;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    state->numActions = 1;
    playVillage(player, 0, state);
    printResult(3, state->numActions);

    printf("Test +2 Actions with multiple action left: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = village;
    state->hand[player][0] = gardens;
    state->handCount[player] = 2;
    state->numActions = 3;
    playVillage(player, 0, state);
    printResult(4, state->numActions);

    printf("Test discard card that village was discarded: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = village;
    state->numActions = 1;
    playVillage(player, 0, state);
    printResult(1, state->hand[player][0] != village);

    printf("Test correct village card was discarded with multiple in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = village;
    state->hand[player][1] = village;
    state->numActions = 1;
    playVillage(player, 1, state);
    printResult(1, state->hand[player][1] != village);
}