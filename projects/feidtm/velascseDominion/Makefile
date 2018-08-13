CFLAGS = -Wall -fpic -coverage -lm -std=c99

rngs.o: rngs.h rngs.c
	gcc -c rngs.c -g  $(CFLAGS)

dominion.o: dominion.h dominion.c rngs.o
	gcc -c dominion.c -g  $(CFLAGS)

unittesthelpers.o: unittest_helpers.c dominion.o rngs.o
	gcc -c unittest_helpers.c -g $(CFLAGS)

unittest1: unittest1.c unittest_helpers.o dominion.o rngs.o
	gcc -o unittest1 -g unittest1.c unittest_helpers.o dominion.o rngs.o $(CFLAGS)

unittest2: unittest2.c unittest_helpers.o dominion.o rngs.o
	gcc -o unittest2 -g unittest2.c unittest_helpers.o dominion.o rngs.o $(CFLAGS)

unittest3: unittest3.c unittest_helpers.o dominion.o rngs.o
	gcc -o unittest3 -g unittest3.c unittest_helpers.o dominion.o rngs.o $(CFLAGS)

unittest4: unittest4.c unittest_helpers.o dominion.o rngs.o
	gcc -o unittest4 -g unittest4.c unittest_helpers.o dominion.o rngs.o $(CFLAGS)

unittests: unittest1 unittest2 unittest3 unittest4

cardtest1: cardtest1.c unittest_helpers.o dominion.o rngs.o
	gcc -o cardtest1 -g cardtest1.c unittest_helpers.o dominion.o rngs.o $(CFLAGS)

cardtest2: cardtest2.c unittest_helpers.o dominion.o rngs.o
	gcc -o cardtest2 -g cardtest2.c unittest_helpers.o dominion.o rngs.o $(CFLAGS)

cardtest3: cardtest3.c unittest_helpers.o dominion.o rngs.o
	gcc -o cardtest3 -g cardtest3.c unittest_helpers.o dominion.o rngs.o $(CFLAGS)

cardtest4: cardtest4.c unittest_helpers.o dominion.o rngs.o
	gcc -o cardtest4 -g cardtest4.c unittest_helpers.o dominion.o rngs.o $(CFLAGS)

cardtests: cardtest1 cardtest2 cardtest3 cardtest4

unittestresults: unittests cardtests
	./unittest1 > unittestresults.out
	./unittest2 >> unittestresults.out
	./unittest3 >> unittestresults.out
	./unittest4 >> unittestresults.out
	./cardtest1 >> unittestresults.out
	./cardtest2 >> unittestresults.out
	./cardtest3 >> unittestresults.out
	./cardtest4 >> unittestresults.out
	gcov -b dominion.c >> unittestresults.out

randomtest_helpers.o:  randomtest_helpers.c unittest_helpers.o dominion.o rngs.o
	gcc -c randomtest_helpers.c -g $(CFLAGS)

randomtestcard1: randomtestcard1.c randomtest_helpers.o unittest_helpers.o dominion.o rngs.o
	gcc -o randomtestcard1 -g randomtestcard1.c randomtest_helpers.o unittest_helpers.o dominion.o rngs.o $(CFLAGS)

randomtestcard2: randomtestcard2.c randomtest_helpers.o unittest_helpers.o dominion.o rngs.o
	gcc -o randomtestcard2 -g randomtestcard2.c randomtest_helpers.o unittest_helpers.o dominion.o rngs.o $(CFLAGS)

randomtestadventurer: randomtestadventurer.c randomtest_helpers.o unittest_helpers.o dominion.o rngs.o
	gcc -o randomtestadventurer -g randomtestadventurer.c randomtest_helpers.o unittest_helpers.o dominion.o rngs.o $(CFLAGS)

randomtests: randomtestcard1 randomtestcard2 randomtestadventurer

randomtestresults: randomtests
	./randomtestcard1 > randomtestcard1.out
	gcov -b dominion.c >> randomtestcard1.out
	./randomtestcard2 > randomtestcard2.out
	gcov -b dominion.c >> randomtestcard2.out
	./randomtestadventurer > randomtestadventurer.out
	gcov -b dominion.c >> randomtestadventurer.out

all: unittestresults randomtestresults

clean:
	rm -f *.o playdom.exe playdom player player.exe
	rm -f *.gcov *.gcda *.gcno *.so *.out testDrawCard testDrawCard.exe
	rm -f unittest1 unittest2 unittest3 unittest4
	rm -f cardtest1 cardtest2 cardtest3 cardtest4
	rm -f randomtestcard1 randomtestcard2 randomtestadventurer
