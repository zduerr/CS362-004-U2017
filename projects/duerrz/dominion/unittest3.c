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
    printf("---TEST fullDeckCount---\n");
    struct gameState* state = NULL;
    int k[10] = {adventurer, gardens, embargo, village, minion, mine, cutpurse,
                 sea_hag, tribute, smithy};
    const int player = 1;

    printf("Empty hand, deck, and discard pile: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->deckCount[player] = state->handCount[player] = state->discardCount[player] = 0;
    printResult(0, fullDeckCount(player, adventurer, state));

    printf("Card not one of the 10 chosen for the game: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->deckCount[player] = state->handCount[player] = state->discardCount[player] = 1;
    state->discard[player][0] = state->hand[player][0] = state->deck[player][0] = gardens;
    printResult(0, fullDeckCount(player, estate, state));

    printf("Multiple copies of card found in hand: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->hand[player][0] = state->hand[player][1] = gardens;
    state->hand[player][2] = village;
    state->handCount[player] = 3;
    state->deckCount[player] = state->discardCount[player] = 0;
    printResult(2, fullDeckCount(player, gardens, state));

    printf("Multiple copies of card found in deck: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->deck[player][0] = state->deck[player][1] = gardens;
    state->deck[player][2] = village;
    state->deckCount[player] = 3;
    state->handCount[player] = state->discardCount[player] = 0;
    printResult(2, fullDeckCount(player, gardens, state));

    printf("Multiple copies of card found in discard: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->discard[player][0] = state->discard[player][1] = gardens;
    state->discard[player][2] = village;
    state->discardCount[player] = 3;
    state->handCount[player] = state->deckCount[player] = 0;
    printResult(2, fullDeckCount(player, gardens, state));

    printf("Copies of card found in multiple locations: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->discard[player][0] = state->discard[player][1] = gardens;
    state->discard[player][2] = state->hand[player][0] = state->deck[player][0] = village;
    state->discardCount[player] = 3;
    state->handCount[player] = state->deckCount[player] = 1;
    printResult(3, fullDeckCount(player, village, state));

    printf("No copies of card the card in any location: ");
    state = refreshGameState(state);
    initializeGame(2, k, 2, state);
    state->discard[player][0] = state->discard[player][1] = gardens;
    state->discard[player][2] = state->hand[player][0] = state->deck[player][0] = village;
    state->discardCount[player] = 3;
    state->handCount[player] = state->deckCount[player] = 1;
    printResult(0, fullDeckCount(player, embargo, state));
}