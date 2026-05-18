#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_HANDS 1000000
#define BET 1

// Simule une main de video poker sans garder de cartes (draw aléatoire)
int simulate_hand(void) {
    int deck[52];
    int hand[5];
    int holdMask[5] = {DISCARD, DISCARD, DISCARD, DISCARD, DISCARD};
    int result;

    gameCreateDeck(deck);
    gameDealInitialHand(deck, hand);
    // On ne garde aucune carte
    gameDrawSelectedCards(deck, hand, holdMask);
    result = gameCheckHandWinOrLose(hand);
    return result;
}

int main(void) {
    long long total_bet = 0;
    long long total_return = 0;
    int i, win;
    srand((unsigned int)time(NULL));

    for(i = 0; i < N_HANDS; i++) {
        win = simulate_hand();
        total_bet += BET;
        total_return += win;
    }
    double rtp = 100.0 * (double)total_return / (double)total_bet;
    printf("Simulation sur %d mains (aucune carte gardée):\n", N_HANDS);
    printf("Total misé: %lld\n", total_bet);
    printf("Total retour: %lld\n", total_return);
    printf("Taux de retour (RTP): %.2f %%\n", rtp);
    printf("Perte moyenne par main: %.4f\n", (double)(total_bet-total_return)/N_HANDS);
    return 0;
}
