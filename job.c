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
#include <stdio.h>
//#include <conio.h>
#include <unistd.h>
//#include "ncurses.h"
#include <wchar.h>
#include <locale.h>
#include <termios.h> // POSIX Terminal Control Definitions 

struct termios stdinSettingsOld;

int main(void)
{
    int deck[52];
    int hand[5];
    int result;

    srand(time(NULL));      // init random
    setlocale(LC_CTYPE, "");
    initButton();

    do
    {
        
        createDeck(deck);

        wprintf(L"\nSelect Card(s) to Hold, Press 'Space Bar' to Draw");

        getHand(deck, hand);
        getUserChoiceAndDraw(deck, hand);
        result = checkHandWinOrLose(hand);
        wprintf(L"\nResult= %d", result);

        if(result != Lose)
        {
            int key;
            wprintf(L"\nPress 'D' to Double or any other Key to Keep");
            key = waitButton();
            if((key == 'D') || (key =='d'))
            {
                result = result * playDouble(deck);
                wprintf(L"\nResult= %d", result);
            }
        }
        wprintf(L"\nPress 'Space Bar' to Play Again or any other Key to Quit");
    }
    while(waitButton() == 32); // ASCII 'Space Bar'

    wprintf(L"\npoker END\n");
    closeButton();
    return 0;
}

void createDeck(int *deck)
{
    for(int i=0; i<52; i++) // create deck
    {
        deck[i] = i+1; // deck = 1 to 52
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
    wprintf(L"\n            1     2     3     4     5");
    wprintf(L"\nhand 1st : ");
    for(int i=0; i<5; i++)
    {
        hand[i] = deck[i+1];  // +1 burn first card
        printCard(hand[i]);
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
        selection = waitButton();
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
    wprintf(L"\nhand 2nd : ");
    for(i = 0; i < 5; i++)
    {
        if(h[i] == DISCARD)
        {
            hand[i] = deck[drawCardNumber];
            drawCardNumber++;
        }
        printCard(hand[i]);
    }
}

void sortHand(int *hand)
{
    int i, j, temp;
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
    /*wprintf(L"\nhand sort: ");
    for(i = 0; i < 5; i++)
    {
        wprintf(L"%2d ", hand[i]);
    }*/ 
}

int reduceCard (int card)
{
        if(card > AsCarreau)
        {
            card = card - AsCarreau;
        }
        else if(card > AsCoeur)
        {
            card = card - AsCoeur;
        }
        else if(card > AsPique)
        {
            card = card - AsPique;
        }
}
void reduceHand(int *hand)
{
    int i;
    for(i = 0; i < 5; i++)
    {
        hand[i] = reduceCard(hand[i]);
    }
    /*wprintf(L"\nhand red.: ");
    for(i = 0; i < 5; i++)
    {
        wprintf(L"%2d ", hand[i]);
    }*/ 
}

int checkHandWinOrLose(int *hand)
{
    int result = Lose;
    sortHand(hand);
// flush
    if(hand[0] >= DeuxPique && hand[4] <= AsPique)
        result = Flush;
    else if(hand[0] >= DeuxCoeur && hand[4] <= AsCoeur)
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
            if(hand[4] == AsPique || hand[4] == AsCoeur || hand[4] == AsCarreau || hand[4] == AsTrefle)
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
            if(hand[i] > DixPique)
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
        wprintf(L"JackOrBetter");
        break;
    case TwoPairs:
        wprintf(L"TwoPairs");
        break;
    case ThreeOfAKind:
        wprintf(L"ThreeOfAKind");
        break;
    case Straight:
        wprintf(L"Straight");
        break;
    case Flush:
        wprintf(L"Flush");
        break;
    case FullHouse:
        wprintf(L"FullHouse");
        break;
    case FourOfAKind:
        wprintf(L"FourOfAKind");
        break;
    case StraightFlush:
        wprintf(L"StraightFlush");
        break;
    case RoyalFlush:
        wprintf(L"RoyalFlush");
        break;
    default:
        wprintf(L"Lose");
        break;
    }
 

    return result;
}

int playDouble(int *deck)
{
    int key;
    int result = 1;
    createDeck(deck);
    wprintf(L"\nPress 'H'igh or 'L'ow, any other key to keep");
    wprintf(L"\ndouble: %2d ", deck[1]);
    key = waitButton();
    wprintf(L" --%2d ", deck[3]);
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

int waitButton()
{
    return getc(stdin);
    //char ch;
    //scanf("%c", &ch);
    //return (int)ch;

}

void initButton()
{
    // Setting the Attributes of the serial port using termios structure 
	struct termios stdinSettings;	// Create the structure 
	tcgetattr(STDIN_FILENO, &stdinSettings);	// Get the current attributes of the Serial port 
    stdinSettingsOld = stdinSettings;

	stdinSettings.c_lflag &= ~(ECHO | ECHOE | ISIG);  // Disable echo, Disable signal 
    stdinSettings.c_lflag &= ~(ICANON);  // Non Cannonical mode
	//stdinSettings.c_oflag &= ~OPOST;	// No Output Processing

	// Setting Time outs 
	stdinSettings.c_cc[VMIN] = 1; // Read at least X character(s) 
	stdinSettings.c_cc[VTIME] = 0; // Wait 30 for 3sec (0 for indefinetly) 

	if((tcsetattr(STDIN_FILENO, TCSANOW, &stdinSettings)) != 0) // Set the attributes to the termios structure
		wprintf(L"\n  Erreur! configuration des attributs du port serie");
}

void closeButton()
{
    tcsetattr(STDIN_FILENO, TCSANOW, &stdinSettingsOld);
}

void printCard(int card)
{
    int reducedCard = reduceCard(card);

    if(reducedCard == AsPique)
    {
        wprintf(L"AS");
    }
    else if(reducedCard == RoiPique)
    {
        wprintf(L" K");
    }
    else if(reducedCard == DamePique)
    {
        wprintf(L" D");
    }
    else if(reducedCard == JackPique)
    {
        wprintf(L" J");
    }
    else
        wprintf(L"%2d", reducedCard + 1); // value 1 in the deck is the card 2, value 13 is the ACE


    if(card <= AsPique)
    {
        wprintf(L"%lc...", PiqueChar);
    }
    else if(card <= AsCoeur)
    {
        card = card - AsPique;
        wprintf(L"%lc...", CoeurChar);
    }
    else if(card <= AsCarreau)
    {
        card = card - AsCoeur;
        wprintf(L"%lc...", CarreauChar);
    }
    else if(card <= AsTrefle)
    {
        card = card - AsCarreau;
        wprintf(L"%lc...", TrefleChar);
    }
    
}