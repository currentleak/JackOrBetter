/* ========================================
 *
 * Copyright kco
 * All Rights Reserved
 * UNPUBLISHED, LICENSED SOFTWARE.
 *
 * CONFIDENTIAL AND PROPRIETARY INFORMATION
 * WHICH IS THE PROPERTY OF kco.
 *
 * ========================================
*/
#include "job.h"

#include <time.h>
#include <stdlib.h>
#include <conio.h>
#include <stdio.h>
//#include "ncurses.h"
#include <wchar.h>
#include <locale.h>


int main(void)
{
    int deck[52];
    int hand[5];
    int result;

    srand(time(NULL));      // init random

    setlocale(LC_CTYPE, "");
    wchar_t star = 0x2660;
    wprintf(L"%lc\n", star);


    do
    {
        createDeck(deck);

        printf("\nSelect Card(s) to Hold, Press 'Space Bar' to Draw");

        getHand(deck, hand);
        getUserChoiceAndDraw(deck, hand);
        sortHand(hand);
        result = checkHandWinOrLose(hand);
        printf("\nResult= %d", result);

        if(result != Lose)
        {
            int key;
            printf("\nPress 'D' to Double or any other Key to Keep");
            key = _getch();
            if((key == 'D') || (key =='d'))
            {
                result = result * playDouble(deck);
                printf("\nResult= %d", result);
            }
        }
        printf("\nPress 'Space Bar' to Play Again or any other Key to Quit");
    }
    while(_getch() == 32); // ASCII 'Space Bar'

    printf("\npoker END\n");    
    return 0;
}

void createDeck(int *deck)
{
    for(int i=0; i<52; i++) // create deck
    {
        deck[i] = i+1;
    }

    int TempoCardValue;
    int CardPosition1, CardPosition2;
    for(int i=0; i<SHUFFLE_COUNT; i++) // shuffle deck
    {
        CardPosition1 = rand() % 52;  
        TempoCardValue = deck[CardPosition1];
        CardPosition2 = rand() % 52;  
        deck[CardPosition1] = deck[CardPosition2];
        deck[CardPosition2] = TempoCardValue; 
    }
}

void getHand(int *deck, int *hand)
{
    printf("\nhand 1st : ");
    for(int i=0; i<5; i++)
    {
        hand[i] = deck[i+1];  // +1 burn first card
        printf("%2d ", hand[i]);
    }
}

void getUserChoiceAndDraw(int *deck, int *hand)
{
    // get card position to hold from player
    int posDrawCard[5];
    int h[5] = {DISCARD, DISCARD, DISCARD, DISCARD, DISCARD};
    int selection;
    do 
    {
        selection = _getch();
        switch (selection)
        {
            case 49:  // ASCII 1
                h[0] = !h[0];
                break;
            case 50:  // ASCII 2
                h[1] = !h[1];
                break;
            case 51:  // ASCII 3
                h[2] = !h[2];
                break;
            case 52:  // ASCII 4
                h[3] = !h[3];
                break;
            case 53:  // ASCII 5
                h[4] = !h[4];
                break;
            default :
                break;
        }
    }
    while(selection != 32);  // ASCII 'Space Bar'
    // draw new card
    int drawCardNumber = 7; // 1 burn, 5 cards, +1burn
    int i;
    //printf("\rhand: ");
    printf("\nhand 2nd : ");
    for(i = 0; i < 5; i++)
    {
        if(h[i] == DISCARD)
        {
            hand[i] = deck[drawCardNumber];
            drawCardNumber++;
        }
        printf("%2d ", hand[i]);
    }
}

void sortHand(int *hand)
{
    int i, j, temp;
    printf("\nhand sort: ");
    for (i = 0; i < 4; i++) {
        for (j = 0; j < 4 - i; j++) {
            if (hand[j] > hand[j + 1]) {
                // Swap arr[j] and arr[j + 1]
                temp = hand[j];
                hand[j] = hand[j + 1];
                hand[j + 1] = temp;
            }
        }
    }
    for(i = 0; i < 5; i++)
    {
        printf("%2d ", hand[i]);
    } 
}

void reduceHand(int *hand)
{
    int i;
    printf("\nhand red.: ");
    for(i = 0; i < 5; i++)
    {
        if(hand[i] > AsCarreau)
        {
            hand[i] = hand[i] - AsCarreau;
        }
        else if(hand[i] > AsPique)
        {
            hand[i] = hand[i] - AsPique;
        }
        else if(hand[i] > AsCoeur)
        {
            hand[i] = hand[i] - AsCoeur;
        }
    }
    for(i = 0; i < 5; i++)
    {
        printf("%2d ", hand[i]);
    } 
}

int checkHandWinOrLose(int *hand)
{
    int result = Lose;

// flush
    if(hand[0] >= DeuxCoeur && hand[4] <= AsCoeur)
        result = Flush;
    else if(hand[0] >= DeuxPique && hand[4] <= AsPique)
        result = Flush;
    else if(hand[0] >= DeuxCarreau && hand[4] <= AsCarreau)
        result = Flush;
    else if(hand[0] >= DeuxTrefle && hand[4] <= AsTrefle)
        result = Flush;
// straight flush
    if(result == Flush) 
    {
        if ((hand[0] == hand[1] + 1) && (hand[1] == hand[2] + 1) && (hand[2] == hand[3] + 1) && (hand[3] == hand[4] + 1)) 
        {
            result = StraightFlush;
// Royal    
            if(hand[4] == AsCoeur || hand[4] == AsPique || hand[4] == AsCarreau || hand[4] == AsTrefle)
            {
                result = RoyalFlush;
            }
        }
    }

// Reduced Hand
    reduceHand(hand);
    sortHand(hand);
// Jack or Better 
    int i;
    for(i = 0; i < 4; i++)
    {
        if(hand[i] == hand[i+1])
        {
            if(hand[i] > DixCoeur)
            {
                result = JackOrBetter;
            }
        }
    }
// two pairs
    if(hand[0] == hand[1])
    {
        if((hand[2] == hand[3]) || (hand[3] == hand[4]))
        {
            result = TwoPairs;
        }
    }
    else if (hand[1] == hand[2])
    {
        if(hand[3] == hand[4])
        {
            result = TwoPairs;
        }
    }
// Three of a kind
    for(i = 0; i < 3; i++)
    {
        if((hand[i] == hand[i+1]) && (hand[i+1] == hand[i+2]))
        {
            result = ThreeOfAKind;
        }
    }
// Four of a kind
    for(i = 0; i < 2; i++)
    {
        if((hand[i] == hand[i+1]) && (hand[i+1] == hand[i+2]) && (hand[i+2] == hand[i+3]))
        {
            result = FourOfAKind;
        }
    }
// Straight
    if((hand[0] == hand[1] + 1) && (hand[1] == hand[2] + 1) && (hand[2] == hand[3] + 1) && (hand[3] == hand[4] + 1))
    {
        result = Straight;
    }
// Full House
    if(((hand[0] == hand[1]) && (hand[1] == hand[2]) && (hand[3] == hand[4])) ||
     ((hand[0] == hand[1]) && (hand[2] == hand[3]) && (hand[3] == hand[4])) )
    {
        result = FullHouse;
    }

    switch (result)
    {
    case JackOrBetter:
        printf("JackOrBetter");
        break;
    case TwoPairs:
        printf("TwoPairs");
        break;
    case ThreeOfAKind:
        printf("ThreeOfAKind");
        break;
    case Straight:
        printf("Straight");
        break;
    case Flush:
        printf("Flush");
        break;
    case FullHouse:
        printf("FullHouse");
        break;
    case FourOfAKind:
        printf("FourOfAKind");
        break;
    case StraightFlush:
        printf("StraightFlush");
        break;
    case RoyalFlush:
        printf("RoyalFlush");
        break;
    default:
        printf("Lose");
        break;
    }
 

    return result;
}

int playDouble(int *deck)
{
    int key;
    int result = 1;
    createDeck(deck);
    printf("\nPress 'H'igh or 'L'ow, any other key to keep");
    printf("\ndouble: %2d ", deck[1]);
    key = _getch();
    printf(" --%2d ", deck[3]);
    if((key == 'H') || (key =='h'))
    {
        if(deck[1] < deck[3])
            result = 2;
        else
            result = 0;
    }   
    else if((key == 'L') || (key =='l'))
    {
        if(deck[1] > deck[3])
            result = 2;
        else
            result = 0;

    }
    return result;
}