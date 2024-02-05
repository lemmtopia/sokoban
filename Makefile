build:
	gcc src/*.c -o sokoban -lSDL2 -lSDL2_image

run:
	./sokoban

clean:
	rm ./sokoban
