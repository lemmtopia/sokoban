build:
	gcc src/*.c -o sokoban -lSDL2

run:
	./sokoban

clean:
	rm ./sokoban
