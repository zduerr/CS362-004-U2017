#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>


#define PASS 1
#define FAIL 0

struct villageTestResult {
    int villageRemoved;
    int cardDrawn;
    int otherPlayerCards;
    int actionGained;
};

int playVillage(int player, int handPos, struct gameState* state){
    return fVillage(state, player, handPos);
}

void printResult(int expected, int result) {
    printf(expected == result ? "PASS\n" : "FAIL\n");
}

// populates the deck w/ n kingdom cards
void populateDeck(int *k, int *deck, int n) {
    int i;
    for (i = 0; i < n; i++) {
        deck[i] = k[i % 10];
    }
}

// initializes the game and sets the decks to the sizes passed in
void setState(struct gameState *G, int *k, int p, int deckCount, int discardCount, int handCount, int villagePos) {
    memset(G, 23, sizeof(struct gameState));
    initializeGame(2, k, 1, G);

    G->deckCount[p] = deckCount;
    memset(G->deck[p], 0, sizeof(int) * deckCount);
    populateDeck(k, G->deck[p], deckCount);

    G->discardCount[p] = discardCount;
    memset(G->discard[p], 0, sizeof(int) * discardCount);
    populateDeck(k, G->discard[p], discardCount);

    G->handCount[p] = handCount;
    memset(G->hand[p], 0, sizeof(int) * handCount);
    populateDeck(k, G->hand[p], handCount);
    G->hand[p][villagePos] = village;
}

int testVillageRemoved(int player, int villagePos, struct gameState *state) {
    int i;

    //test that village was removed from players hand using kingdom set w/ no village card it in to insure no redraw
    int failed = playVillage(player, villagePos, state);
    for (i = 0; i < state->handCount[player]; i++) {
        if (state->hand[player][i] == village) {
            return FAIL;
        }
    }
    return failed ? FAIL : PASS;
}

int testCardDrawn(int player, int villagePos, struct gameState *state) {
    int handCount = state->handCount[player];
    int failed = playVillage(player, villagePos, state);
    int countOff = (state->handCount[player] != handCount);
    return (countOff || failed) ? FAIL : PASS;
}

int testOtherPlayerCards(int player, int villagePos, struct gameState *state) {
    int otherPlayer = player ^1;
    int otherPlayerHandCount = state->handCount[otherPlayer];
    int failed = playVillage(player, villagePos, state);
    int countOff = (state->handCount[otherPlayer] != otherPlayerHandCount);
    return (countOff || failed) ? FAIL : PASS;
}

int testActionGained(int player, int villagePos, struct gameState *state) {
    int playerActions = state->numActions;
    int failed = playVillage(player, villagePos, state);
    int actionsOff = (state->numActions != playerActions + 2);
    return (actionsOff || failed) ? FAIL : PASS;
}

int main() {
    int p, villagePos, deckCount, discardCount, handCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, village, smithy, baron, great_hall};
    int noVillageK[10] = {adventurer, council_room, feast, gardens, mine, remodel, sea_hag, smithy, baron, great_hall};
    struct gameState G;
    struct villageTestResult result;
    result.cardDrawn = result.villageRemoved = result.actionGained = result.otherPlayerCards = PASS;
    SelectStream(2);
    PutSeed(3);

    for (p = 0; p < 2; p++) {
        for (deckCount = 0; deckCount < 50; deckCount++) {
            discardCount = (deckCount < 10) ? 10 - deckCount : 0;
            while (discardCount < 50) {
                for (handCount = 1; handCount < 2; handCount++) {
                    for (villagePos = 0; villagePos < handCount; villagePos++) {
                        setState(&G, noVillageK, p, deckCount, discardCount, handCount, villagePos);
                        result.villageRemoved &= testVillageRemoved(p, villagePos, &G);

                        setState(&G, k, p, deckCount, discardCount, handCount, villagePos);
                        result.cardDrawn &= testCardDrawn(p, villagePos, &G);

                        setState(&G, k, p, deckCount, discardCount, handCount, villagePos);
                        result.actionGained &= testActionGained(p, villagePos, &G);

                        setState(&G, k, p, deckCount, discardCount, handCount, villagePos);
                        result.otherPlayerCards &= testOtherPlayerCards(p, villagePos, &G);
                    }
                }
                discardCount++;
            }
        }
    }
    printf("---TEST Village---\nTest village was discarded: ");
    printResult(PASS, result.villageRemoved);
    printf("Test +1 card added to hand: ");
    printResult(PASS, result.cardDrawn);
    printf("Test other player deck unaffected: ");
    printResult(PASS, result.otherPlayerCards);
    printf("Test +1 actions added for player: ");
    printResult(PASS, result.actionGained);
}