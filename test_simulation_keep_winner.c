#include "game.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define N_HANDS 1000000
#define BET 1

static int choose_double_key_from_card(int card)
{
    int rank = gameReduceCard(card) + 1; // 2..14 (A=14)

    if(rank <= 6)
    {
        return 'H';
    }

    if(rank >= 9)
    {
        return 'L';
    }

    return 'K'; // keep between 7 and 8
}

static int play_double_strategy(const int *deck, int result)
{
    int cardNumber = 13; // same sequence as game flow

    while(result != 0)
    {
        int key;

        if((cardNumber + 1) > 51)
        {
            break;
        }

        key = choose_double_key_from_card(deck[cardNumber]);
        if(key == 'K')
        {
            break;
        }

        result = gameResolveDoubleChoice(result, deck[cardNumber], deck[cardNumber + 1], key);
        cardNumber = cardNumber + 1;

        if(cardNumber > 50)
        {
            break;
        }
    }

    return result;
}

// Marque dans holdMask les cartes qui forment la combinaison gagnante (Jack or Better ou mieux)
void mark_winning_cards(const int *hand, int *holdMask) {
    int reduced[5];
    int i;
    int counts[15] = {0}; // 2..A (1..13)
    int isPair = 0, isTrips = 0, isQuads = 0;
    int pairValue = 0, tripsValue = 0, quadsValue = 0;
    int pairCount = 0;
    // Réduire les cartes à leur valeur (2=1, ..., A=13)
    for(i=0;i<5;i++) {
        reduced[i] = gameReduceCard(hand[i]);
        counts[reduced[i]]++;
    }
    // Initialiser à DISCARD
    for(i=0;i<5;i++) holdMask[i] = DISCARD;
    // Chercher les combinaisons
    for(i=1;i<=13;i++) {
        if(counts[i]==4) { isQuads=1; quadsValue=i; }
        if(counts[i]==3) { isTrips=1; tripsValue=i; }
        if(counts[i]==2) { pairCount++; isPair=1; pairValue=i; }
    }
    // Quads
    if(isQuads) {
        for(i=0;i<5;i++) if(reduced[i]==quadsValue) holdMask[i]=HOLD;
        return;
    }
    // Full house
    if(isTrips && pairCount>=1) {
        for(i=0;i<5;i++) if(reduced[i]==tripsValue || counts[reduced[i]]==2) holdMask[i]=HOLD;
        return;
    }
    // Trips
    if(isTrips) {
        for(i=0;i<5;i++) if(reduced[i]==tripsValue) holdMask[i]=HOLD;
        return;
    }
    // Two pairs
    if(pairCount==2) {
        for(i=0;i<5;i++) if(counts[reduced[i]]==2) holdMask[i]=HOLD;
        return;
    }
    // One pair Jacks or better
    if(isPair && pairCount==1 && pairValue>=10) {
        for(i=0;i<5;i++) if(reduced[i]==pairValue) holdMask[i]=HOLD;
        return;
    }
    // Straight, flush, straight flush, royal flush : garder tout
    // (On suppose que la main est déjà triée)
    int isStraight=1, isFlush=1, suit0=(hand[0]-1)/13;
    for(i=1;i<5;i++) if(reduced[i]!=reduced[i-1]+1) isStraight=0;
    for(i=1;i<5;i++) if((hand[i]-1)/13!=suit0) isFlush=0;
    if(isStraight || isFlush) {
        for(i=0;i<5;i++) holdMask[i]=HOLD;
        return;
    }
    // Sinon, rien à garder
}

int simulate_hand_keep_winner(void) {
    int deck[52];
    int hand[5];
    int holdMask[5];
    int result;

    gameCreateDeck(deck);
    gameDealInitialHand(deck, hand);
    mark_winning_cards(hand, holdMask);
    gameDrawSelectedCards(deck, hand, holdMask);
    result = gameCheckHandWinOrLose(hand);
    return result;
}

int simulate_hand_keep_winner_with_double(void) {
    int deck[52];
    int hand[5];
    int holdMask[5];
    int result;

    gameCreateDeck(deck);
    gameDealInitialHand(deck, hand);
    mark_winning_cards(hand, holdMask);
    gameDrawSelectedCards(deck, hand, holdMask);

    result = gameCheckHandWinOrLose(hand);
    if(result != Lose)
    {
        result = play_double_strategy(deck, result);
    }

    return result;
}

int main(void) {
    long long total_bet = 0;
    long long total_return = 0;
    long long total_return_with_double = 0;
    int i, win;
    int win_with_double;
    srand((unsigned int)time(NULL));

    for(i = 0; i < N_HANDS; i++) {
        win = simulate_hand_keep_winner();
        win_with_double = simulate_hand_keep_winner_with_double();
        total_bet += BET;
        total_return += win;
        total_return_with_double += win_with_double;
    }

    {
        double rtp = 100.0 * (double)total_return / (double)total_bet;
        double rtp_with_double = 100.0 * (double)total_return_with_double / (double)total_bet;

        printf("Simulation sur %d mains (on ne garde que les cartes gagnantes du 1er tirage):\n", N_HANDS);
        printf("Total misé: %lld\n", total_bet);
        printf("Total retour: %lld\n", total_return);
        printf("Taux de retour (RTP): %.2f %%\n", rtp);
        printf("Perte moyenne par main: %.4f\n", (double)(total_bet-total_return)/N_HANDS);

        printf("\nSimulation avec double sur mains gagnantes:\n");
        printf("Regle double: H si carte <= 6, L si carte >= 9, garder si 7-8\n");
        printf("Total retour (avec double): %lld\n", total_return_with_double);
        printf("Taux de retour (RTP, avec double): %.2f %%\n", rtp_with_double);
        printf("Perte moyenne par main (avec double): %.4f\n", (double)(total_bet-total_return_with_double)/N_HANDS);
    }

    return 0;
}
