#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>


#define PASS 1
#define FAIL 0

struct smithyTestResult {
    int smithyRemoved;
    int cardsDrawn;
    int otherPlayerCards;
};

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
void setState(struct gameState *G, int *k, int p, int deckCount, int discardCount, int handCount, int smithyPos) {
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
    G->hand[p][smithyPos] = smithy;
}

int testSmithyRemoved(int player, int smithyPos, struct gameState *state) {
    int i;

    //test that smithy was removed from players hand using kingdom set w/ no smithy card it in to insure no redraw
    int failed = playSmithy(player, smithyPos, state);
    for (i = 0; i < state->handCount[player]; i++) {
        if (state->hand[player][i] == smithy) {
            return FAIL;
        }
    }
    return failed ? FAIL : PASS;
}

int testCardsDrawn(int player, int smithyPos, struct gameState *state) {
    int handCount = state->handCount[player];
    int failed = playSmithy(player, smithyPos, state);
    return (state->handCount[player] != handCount + 2) || failed ? FAIL : PASS;
}

int testOtherPlayerCards(int player, int smithyPos, struct gameState *state) {
    int otherPlayer = player ^1;
    int otherPlayerHandCount = state->handCount[otherPlayer];
    int failed = playSmithy(player, smithyPos, state);
    return (state->handCount[otherPlayer] != otherPlayerHandCount) || failed ? FAIL : PASS;
}

int main() {
    int p, smithyPos, deckCount, discardCount, handCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    int noSmithyK[10] = {adventurer, council_room, feast, gardens, mine, remodel, sea_hag, village, baron, great_hall};
    struct gameState G;
    struct smithyTestResult result;
    result.cardsDrawn = result.smithyRemoved = result.otherPlayerCards = PASS;
    SelectStream(2);
    PutSeed(3);

    for (p = 0; p < 2; p++) {
        for (deckCount = 0; deckCount < 50; deckCount++) {
            discardCount = (deckCount < 10) ? 10 - deckCount : 0;
            while (discardCount < 50) {
                for (handCount = 1; handCount < 2; handCount++) {
                    for (smithyPos = 0; smithyPos < handCount; smithyPos++) {
                        setState(&G, noSmithyK, p, deckCount, discardCount, handCount, smithyPos);
                        result.smithyRemoved &= testSmithyRemoved(p, smithyPos, &G);

                        setState(&G, k, p, deckCount, discardCount, handCount, smithyPos);
                        result.cardsDrawn &= testCardsDrawn(p, smithyPos, &G);

                        setState(&G, k, p, deckCount, discardCount, handCount, smithyPos);
                        result.otherPlayerCards &= testOtherPlayerCards(p, smithyPos, &G);
                    }
                }
                discardCount++;
            }
        }
    }
    printf("---TEST Smithy---\nTest smithy was discarded: ");
    printResult(PASS, result.smithyRemoved);
    printf("Test +3 cards added to hand: ");
    printResult(PASS, result.cardsDrawn);
    printf("Test other player deck unaffected: ");
    printResult(PASS, result.otherPlayerCards);
}