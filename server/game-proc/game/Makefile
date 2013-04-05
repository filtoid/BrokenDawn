all:
	g++ -c BDUtils.cpp -o BDUtils.o	
	g++ -c BDGame.cpp -o BDGame.o
	g++ -c BDPlayer.cpp -o BDPlayer.o

	ar rvs libbdgame.a BDGame.o
	ar rvs libbdgame.a BDUtils.o
	ar rvs libbdgame.a BDPlayer.o

	ranlib libbdgame.a
	cp libbdgame.a ../libs/
	cp *.h ../include/
clean:
	rm -f BDGame.o
	rm -f BDUtils.o
	rm -f BDPlayer.o
	rm -f libbdgame.a
