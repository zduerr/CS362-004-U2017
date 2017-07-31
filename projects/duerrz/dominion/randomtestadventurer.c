#include "dominion.h"
#include "dominion_helpers.h"
#include "rngs.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>

#define PASS 1
#define FAIL 0

struct adventurerTestResult {
    int adventurerRemoved;
    int otherPlayerCards;
    int moneyAddedToPlayerHand;
    int moneyRemovedFromDeck;
    int handSizeIncreased;
    int nonMoneyCardsDiscarded;
    int discardedCardsReturnedToDiscardDeck;
};

int comp(const void *elementA, const void *elementB) {
    int *a = (int *) elementA;
    int *b = (int *) elementB;
    return *a - *b;
}

void printResult(int expected, int result) {
    printf(expected == result ? "PASS\n" : "FAIL\n");
}

int isMoney(int card){
    return (card == gold) || (card == silver) || (card == copper);
}
// populates the deck w/ n kingdom cards
void populateDeck(int *k, int *deck, int n) {
    int i;
    for (i = 0; i < n; i++) {
        deck[i] = k[i % 10];
    }
}

// check if two decks have the same cards in them, ignoring all money cards
int sameDeckIgnoringMoney(int* this, int*that, int sizeThis, int sizeThat){
    // see if both decks are all money
    int i;
    int thisAllMoney = 1;
    int thatAllMoney = 1;
    for (i = 0; i < sizeThis; ++i) {
        thisAllMoney &= isMoney(this[i]);
    }
    for (i = 0; i < sizeThat; ++i) {
        thatAllMoney &= isMoney(that[i]);
    }
    if (thisAllMoney && thatAllMoney){
        return 1;
    }

    int thisIndex = 0;
    int thatIndex = 0;

    // sort the decks
    qsort((void*) this, (size_t) sizeThis, sizeof(int), comp);
    qsort((void*) that, (size_t) sizeThat, sizeof(int), comp);

    // compare the sorted cards, filtering out the money cards
    while (thisIndex < sizeThis && thatIndex < sizeThat) {
        while(isMoney(this[thisIndex]) && (thisIndex < sizeThis)){
            thisIndex += 1;
        }
        while(isMoney(that[thatIndex]) && thatIndex < sizeThat){
            thatIndex += 1;
        }
        if (this[thisIndex] != that[thatIndex]){
            return 0;
        }
        thisIndex += 1;
        thatIndex += 1;
    }

    // if we reached the end of both decks w/ matching cards, they are the same, ignoring money cards
    return (thisIndex  == sizeThis) && (thatIndex  == sizeThat);
}

// makes sure at least n money cards are in the deck.discard, adding them randomly
void addMoney(int *discard, int discardCount, int *deck, int deckCount, int n) {
    int money[3] = {gold, silver, copper};
    int position = 0;
    int index = 0;
    int added = 0;
    int *target;
    int totalSize = discardCount + deckCount;
    int i;

    for (i = 0; i < discardCount; i++) {
        if (isMoney(discard[i])){
            added += 1;
        }
    }
    for (i = 0; i < deckCount; i++) {
        if (isMoney(deck[i])){
            added += 1;
        }
    }

    while (added < n) {
        position = rand() % totalSize;
        // add the money card to the deck if position is within the deck and it hasn't been used
        if (position >= discardCount) {
            index = position - discardCount;
            target = deck;
        } else {
            index = position;
            target = discard;
        }
        if (!isMoney(target[index])) {
            target[index] = money[rand() % 3];
            added += 1;
        }
    }

}

// returns the number of money cards in the hand
int getMoneyCount(int *hand, int n) {
    int count = 0;
    for (int i = 0; i < n; ++i) {
        if (isMoney(hand[i])) {
            count++;
        }
    }
    return count;
}

// initializes the game and sets the decks to the sizes passed in, adds money coins
void setState(struct gameState *G, int *k, int p, int deckCount, int discardCount, int handCount, int moneyCount,
              int adPos) {

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
    G->hand[p][adPos] = adventurer;

    addMoney(G->discard[p], discardCount, G->deck[p], deckCount, moneyCount);
}

int testAdventurerRemoved(int player, struct gameState *state) {
    int i;

    //test that adventurer was removed from players hand using kingdom set w/ no adventurer card it in to insure no redraw
    int failed = playAdventurer(player, state);
    for (i = 0; i < state->handCount[player]; i++) {
        if (state->hand[player][i] == adventurer) {
            return FAIL;
        }
    }
    return failed ? FAIL : PASS;
}

int testOtherPlayerCards(int player, struct gameState *state) {
    int otherPlayer = player ^1;
    int otherPlayerHandCount = state->handCount[otherPlayer];
    int failed = playAdventurer(player, state);
    return (state->handCount[otherPlayer] != otherPlayerHandCount) || failed ? FAIL : PASS;
}

int testMoneyAddedToPlayerHand(int player, struct gameState *state) {
    int startingMoneyCount = getMoneyCount(state->hand[player], state->handCount[player]);
    int failed = playAdventurer(player, state);
    int endingMoneyCount = getMoneyCount(state->hand[player], state->handCount[player]);
    return (endingMoneyCount != startingMoneyCount + 2) || failed ? FAIL : PASS;
}

int testMoneyRemovedFromDeck(int player, struct gameState *state) {
    int discardCount = getMoneyCount(state->discard[player], state->discardCount[player]);
    int deckCount = getMoneyCount(state->deck[player], state->deckCount[player]);
    int startingMoneyCount = discardCount + deckCount;
    int failed = playAdventurer(player, state);
    int endingMoneyCount = getMoneyCount(state->discard[player], state->discardCount[player]) +
                           getMoneyCount(state->deck[player], state->deckCount[player]);
    return (endingMoneyCount != startingMoneyCount - 2) || failed ? FAIL : PASS;
}

int testHandSizeIncreased(int player, struct gameState *state) {
    int startingHandCount = state->handCount[player];
    int failed = playAdventurer(player, state);
    int endingHandCount = state->handCount[player];
    return (endingHandCount != startingHandCount + 1) || failed ? FAIL : PASS;
}

int testNonMoneyCardsDiscarded(int player, struct gameState* state){
    int startingCount = state->handCount[player];
    int* startingHand = (int*) malloc(sizeof(int) * startingCount);
    memcpy(startingHand, state->hand[player], sizeof(int) * state->handCount[player]);

    int failed = playAdventurer(player, state);
    int endingCount = state->handCount[player];
    int* endingHand = (int*) malloc(sizeof(int) * endingCount);
    memcpy(endingHand, state->hand[player], sizeof(int) * endingCount);
    int sameHand = sameDeckIgnoringMoney(startingHand, endingHand, startingCount, endingCount);

    int result = !sameHand || failed ? FAIL: PASS;
    free(startingHand);
    free(endingHand);

    return result;
}

int testDiscardedCardsReturnedToDiscardDeck(int player, struct gameState* state){
    int i;
    int startingCount = state->deckCount[player] + state->discardCount[player];
    int* startingDeck = (int*) malloc(sizeof(int) * startingCount);
    for (i = 0; i < state->deckCount[player]; i++) {
        startingDeck[i] = state->deck[player][i];
    }
    for (i = state->deckCount[player]; i < startingCount; i++) {
        startingDeck[i] = state->discard[player][i - state->deckCount[player]];
    }

    int failed = playAdventurer(player, state);
    int endingCount = state->deckCount[player] + state->discardCount[player];
    int* endingDeck = (int*) malloc(sizeof(int) * endingCount);
    for (i = 0; i < state->deckCount[player]; i++) {
        endingDeck[i] = state->deck[player][i];
    }
    for (i = state->deckCount[player]; i < endingCount; i++) {
        endingDeck[i] = state->discard[player][i - state->deckCount[player]];
    }

    int sameHand = sameDeckIgnoringMoney(startingDeck, endingDeck, startingCount, endingCount);
    int result = !sameHand || failed ? FAIL: PASS;
    free(startingDeck);
    free(endingDeck);
    return result;
}

int main() {
    int p, adventurerPos, deckCount, discardCount, handCount, moneyCount;
    int k[10] = {adventurer, council_room, feast, gardens, mine, remodel, smithy, village, baron, great_hall};
    int noAdventurerK[10] = {village, council_room, feast, gardens, mine, remodel, sea_hag, smithy, baron, great_hall};
    struct gameState G;
    struct adventurerTestResult result;
    result.adventurerRemoved = result.otherPlayerCards = result.moneyAddedToPlayerHand = result.moneyRemovedFromDeck = result.handSizeIncreased = result.nonMoneyCardsDiscarded = result.discardedCardsReturnedToDiscardDeck = PASS;
    SelectStream(2);
    PutSeed(3);
    srand((unsigned int) time(NULL));

    for (p = 0; p < 2; p++) {
        for (deckCount = 0; deckCount < 20; deckCount++) {
            discardCount = (deckCount < 10) ? 10 - deckCount : 0;
            while (discardCount < 50) {
                for (handCount = 1; handCount < 5; handCount++) {
                    for (moneyCount = 2; moneyCount <= discardCount + deckCount; moneyCount++) {
                        for (adventurerPos = 0; adventurerPos < handCount; adventurerPos++) {
                            setState(&G, noAdventurerK, p, deckCount, discardCount, handCount, moneyCount, adventurerPos);
                            result.adventurerRemoved &= testAdventurerRemoved(p, &G);

                            setState(&G, noAdventurerK, p, deckCount, discardCount, handCount, moneyCount, adventurerPos);
                            result.otherPlayerCards &= testOtherPlayerCards(p, &G);

                            setState(&G, k, p, deckCount, discardCount, handCount, moneyCount, adventurerPos);
                            result.moneyAddedToPlayerHand &= testMoneyAddedToPlayerHand(p, &G);

                            setState(&G, k, p, deckCount, discardCount, handCount, moneyCount, adventurerPos);
                            result.moneyRemovedFromDeck &= testMoneyRemovedFromDeck(p, &G);

                            setState(&G, k, p, deckCount, discardCount, handCount, moneyCount, adventurerPos);
                            result.handSizeIncreased &= testHandSizeIncreased(p, &G);

                            setState(&G, k, p, deckCount, discardCount, handCount, moneyCount, adventurerPos);
                            result.nonMoneyCardsDiscarded &= testNonMoneyCardsDiscarded(p, &G);

                            setState(&G, k, p, deckCount, discardCount, handCount, moneyCount, adventurerPos);

                            result.discardedCardsReturnedToDiscardDeck &= testDiscardedCardsReturnedToDiscardDeck(p, &G);
                        }
                    }
                }
                discardCount++;
            }
        }
    }
    printf("\n---TEST adventurer---\nTest adventurer was discarded: ");
    printResult(PASS, result.adventurerRemoved);
    printf("Test other player deck unaffected: ");
    printResult(PASS, result.otherPlayerCards);
    printf("Test cards added to player hand are money: ");
    printResult(PASS, result.moneyAddedToPlayerHand);
    printf("Money added to player hand was removed from deck: ");
    printResult(PASS, result.moneyRemovedFromDeck);
    printf("Test +1 hand size for player (+2 money, -1 adventurer card): ");
    printResult(PASS, result.handSizeIncreased);
    printf("Cards added to hand that aren't money discarded from hand: ");
    printResult(PASS, result.nonMoneyCardsDiscarded);
    printf("Discarded cards are returned to discard pile: ");
    printResult(PASS, result.discardedCardsReturnedToDiscardDeck);
}