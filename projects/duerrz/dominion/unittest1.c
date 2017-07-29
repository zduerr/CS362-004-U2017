#include "dominion.h"
#include <stdio.h>
#include <stdlib.h>

struct gameState* refreshGameState(struct gameState *state) {
    if (state) {
        free(state);
    }
    state = (struct gameState *) malloc(sizeof(struct gameState));

    for (int i = 0; i < 25; i++) {
        state->supplyCount[i] = 1;
    }
    return state;
}

void printResult(int expected, int result){
    printf(expected == result ? "PASS\n" : "FAIL\n");
}

int main() {
    struct gameState* state = refreshGameState(NULL);
    printf ("---TEST isGameOver---\n");

    printf ("1 Province card left: ");
    state->supplyCount[province] = 1;
    printResult(0, isGameOver(state));
    state = refreshGameState(state);

    printf("More than 1 Province card left: ");
    state->supplyCount[province] = 4;
    printResult(0, isGameOver(state));
    state = refreshGameState(state);

    printf("0 Province cards left: ");
    state->supplyCount[province] = 0;
    printResult(1, isGameOver(state));
    state = refreshGameState(state);

    printf("0 supply piles depleted: ");
    printResult(0, isGameOver(state));
    state = refreshGameState(state);

    printf("All supply piles depleted: ");
    for (int i = 0; i < 25; i++){
        state->supplyCount[i] = 0;
    }
    printResult(1, isGameOver(state));
    state = refreshGameState(state);

    printf("3 supply piles depleted: ");
    state->supplyCount[2] = state->supplyCount[1] = state->supplyCount[15] = 0;
    printResult(1, isGameOver(state));
    state = refreshGameState(state);

    printf("less than 3 supply piles depleted: ");
    state->supplyCount[11] = state->supplyCount[16] = 0;
    printResult(0, isGameOver(state));
    state = refreshGameState(state);

    printf("more than 3 supply piles depleted: ");
    for (int i = 0; i < 25; i++){
        state->supplyCount[i] = 0;
    }
    state->supplyCount[0] = state->supplyCount[24] = 1;
    printResult(1, isGameOver(state));
    state = refreshGameState(state);

    if (state) {
        free(state);
    }
}