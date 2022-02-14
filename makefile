norm:
	gcc -o process process.c && gcc -o normalize normalize.c -lsndfile -Wall &&  gcc -o chunk chunk.c -lsndfile -Wall
clean:
		rm -f chunk normalize process
