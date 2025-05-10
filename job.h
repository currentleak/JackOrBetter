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
#define PiqueChar   0x2660
#define CoeurChar   0x2661
#define CarreauChar 0x2662
#define TrefleChar  0x2663

#define DeuxPique       1
#define TroisPique      2
#define QuartePique     3
#define CinqPique       4
#define SixPique        5
#define SeptPique       6
#define HuitPique       7
#define NeufPique       8
#define DixPique        9
#define JackPique       10
#define DamePique       11
#define RoiPique        12
#define AsPique         13

#define DeuxCoeur       14
#define TroisCoeur      15
#define QuatreCoeur     16
#define CinqCoeur       17
#define SixCoeur        18
#define SeptCoeur       19
#define HuitCoeur       20
#define NeufCoeur       21
#define DixCoeur        22
#define JackCoeur       23
#define DameCoeur       24
#define RoiCoeur        25
#define AsCoeur         26

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
#define HOLD            0

#define SpaceBar        32

void clearScreen(void);
void createDeck(int *deck);
void getHand(int *deck, int *hand);
void getUserChoiceAndDraw(int *deck, int *hand);
void sortHand(int *hand);
int reduceCard(int);
void reduceHand(int *);
int checkHandWinOrLose(int *hand);
int playDouble(int *, int);
int waitButton(void);
void initButton(void);
void closeButton(void);
void printCard(int);
