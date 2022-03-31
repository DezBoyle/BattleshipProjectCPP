CC = g++
CFLAGS = -Wall -Wextra -Werror -O2 -std=c++11
OBJ =
DEPS = Game.h Display.h Player.h Utility.h
MAIN = main.o Game.o Display.o Player.o Utility.o

%.o: %.cpp ${DEPS}
	${CC} -c -o $@ $< ${CFLAGS}

app: ${MAIN} ${OBJ}
	${CC} -o $@ $^ ${CFLAGS}

.PHONY: clean
clean:
	rm *.o app *.out
