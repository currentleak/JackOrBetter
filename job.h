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

// Card MAP

#define DeuxCoeur       1
#define TroisCoeur      2
#define QuatreCoeur     3
#define CinqCoeur       4
#define SixCoeur        5
#define SeptCoeur       6
#define HuitCoeur       7
#define NeufCoeur       8
#define DixCoeur        9
#define JackCoeur       10
#define DameCoeur       11
#define RoiCoeur        12
#define AsCoeur         13

#define DeuxPique       14

#define RoiPique        25
#define AsPique         26

#define DeuxCarreau     27

#define RoiCarreau      38
#define AsCarreau       39

#define DeuxTrefle      40

#define RoiTrefle       51
#define AsTrefle        52



// hand win 
#define RoyalFlush      250
#define StraightFlush   50
#define FourOfAKind     25
#define FullHouse       9
#define Flush           6
#define Straight        4
#define ThreeOfAKind    3
#define TwoPairs        2
#define JackOrBetter    1
#define Lose            0

#define BLANK_CARD      99
#define SHUFFLE_COUNT   1000
#define DISCARD         1
#define KEEP            0


void createDeck(int *deck);
void getHand(int *deck, int *hand);
void getUserChoiceAndDraw(int *deck, int *hand);
void sortHand(int *hand);
void reduceHand(int *);
int checkHandWinOrLose(int *hand);
int playDouble(int *);
