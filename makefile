clean:
		rm chunk normalize
norm:
		gcc -o normalize normalize.c -lsndfile -Wall

chunk:
		gcc -o chunk chunk.c -lsndfile -Wall
