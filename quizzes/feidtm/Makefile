CFLAGS = -Wall -fpic -coverage -lm -std=c99

runtest: testme
	./testme > testme.out

testme: testme.c
	gcc -o testme testme.c $(CFLAGS)

clean:
	rm -f testme *.out *.gcov *.gcda *.gcno

