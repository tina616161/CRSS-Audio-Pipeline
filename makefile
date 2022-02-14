norm:
		gcc -o normalize normalize.c -lsndfile -Wall &&  gcc -o chunk chunk.c -lsndfile -Wall
clean:
		rm -f chunk normalize
