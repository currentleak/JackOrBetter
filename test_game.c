#include "game.h"

#include <stdio.h>
#include <stdlib.h>

static void expectEqual(const char *name, int expected, int actual)
{
    if(expected != actual)
    {
        fprintf(stderr, "FAIL: %s (expected %d, got %d)\n", name, expected, actual);
        exit(1);
    }
    printf("PASS: %s\n", name);
}

static void testHandEvaluation(void)
{
    const int aceLowStraight[5] = {30, DeuxCarreau, TroisPique, QuartePique, AsPique};
    const int jackPair[5] = {JackPique, JackCoeur, TroisPique, SixCoeur, 47};
    const int fullHouse[5] = {TroisPique, TroisCoeur, 28, RoiPique, RoiCoeur};

    expectEqual("A2345 should be Straight", Straight, gameCheckHandWinOrLose(aceLowStraight));
    expectEqual("Jack pair should be JackOrBetter", JackOrBetter, gameCheckHandWinOrLose(jackPair));
    expectEqual("Full house detection", FullHouse, gameCheckHandWinOrLose(fullHouse));
}

static void testDoubleResolution(void)
{
    expectEqual("Double high wins", 8, gameResolveDoubleChoice(4, DeuxPique, TroisPique, 'H'));
    expectEqual("Double high loses", 0, gameResolveDoubleChoice(4, TroisPique, DeuxPique, 'H'));
    expectEqual("Double low wins", 8, gameResolveDoubleChoice(4, TroisPique, DeuxPique, 'L'));
    expectEqual("Double low loses", 0, gameResolveDoubleChoice(4, DeuxPique, TroisPique, 'L'));
    expectEqual("Non H/L keeps result", 4, gameResolveDoubleChoice(4, DeuxPique, TroisPique, 'X'));
}

int main(void)
{
    testHandEvaluation();
    testDoubleResolution();
    printf("All tests passed.\n");
    return 0;
}
