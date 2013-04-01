all:
	g++ -c BDUtils.cpp -o BDUtils.o	
	g++ -c BDGame.cpp -o BDGame.o
	
	ar rvs libbdgame.a BDGame.o
	ar rvs libbdgame.a BDUtils.o
	ranlib libbdgame.a
	cp libbdgame.a ../libs/
	cp *.h ../include/
clean:
	rm -f BDGame.o
	rm -f BDUtils.o
	rm -f libbdgame.a
