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

int isTreasure(int card){
    return card == silver || card == gold || card == copper;
}

int playAdventurer(int player, struct gameState* state){
    return fAdventurer(state, player);
}

int main() {
    printf("---TEST Adventurer---\n");
    struct gameState *state = NULL;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
                 sea_hag, great_hall, smithy};
    const int player = 0;

    printf("Test two treasure cards are added to hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = adventurer;
    state->handCount[player] = 1;
    playAdventurer(player, state);
    printResult(1, isTreasure(state->hand[player][0]) && isTreasure(state->hand[player][1]));

    printf("Test two treasure cards are added to hand when deck does not have two remaining: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = adventurer;
    state->handCount[player] = 1;
    state->discard[player][0] = state->discard[player][0] = silver;
    for (int i = 0; i < 10; i++){
        state->deck[player][i] = 0;
    }
    playAdventurer(player, state);
    printResult(1, isTreasure(state->hand[player][0]) && isTreasure(state->hand[player][1]));

    printf("Test non-treasure cards drawn were put in discard pile: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = adventurer;
    state->handCount[player] = 1;
    state->deck[player][0] = embargo;
    state->deck[player][1] = state->deck[player][2] = copper;
    state->discardCount[player] = 0;
    playAdventurer(player, state);
    printResult(embargo, state->discard[player][0]);

    printf("Test drawn cards were removed from deck: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = adventurer;
    state->handCount[player] = 1;
    state->deck[player][0] = state->deck[player][1] = copper;
    state->deck[player][2] = minion;
    state->deckCount[player] = 3;
    playAdventurer(player, state);
    printResult(minion, state->deck[player][0]);

    printf("Test copper is added when top two cards in deck  are copper: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = adventurer;
    state->handCount[player] = 1;
    state->deck[player][0] = state->deck[player][1] = copper;
    playAdventurer(player, state);
    printResult(1, (state->deck[player][0] == copper) && (state->deck[player][1] == copper));

    printf("Test silver is added when top two cards in deck are silver: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = adventurer;
    state->handCount[player] = 1;
    state->deck[player][0] = state->deck[player][1] = silver;
    playAdventurer(player, state);
    printResult(1, (state->deck[player][0] == silver) && (state->deck[player][1] == silver));

    printf("Test gold is added when top two cards in deck are gold: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = adventurer;
    state->handCount[player] = 1;
    state->deck[player][0] = state->deck[player][1] = gold;
    playAdventurer(player, state);
    printResult(1, (state->deck[player][0] == gold) && (state->deck[player][1] == gold));
}