#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <math.h>

typedef struct {
	double* data;
	long count;
} WaveData;

WaveData
peak_normalize(short* data, long arr_size, short max){
	double factor = 1.0 / max;
	double* out = malloc(sizeof(double) * arr_size);

	if(out == NULL){
		printf("Malloc error. Not enough memory");
		exit(-1);
	}

	for(long j = 0; j < arr_size; j++)
		out[j] = (double) factor * data[j];

	return (WaveData) {out, arr_size};
}


int
main(int argc, char** argv){
	if(argc < 3){
		printf("ERROR: Not enough arguments\n");
		printf("Usage: normalize [input file path] [output file path]\n");
		exit(1);
	}

	const char* file_in_path = argv[1];
	const char* file_out_path = argv[2];

	SNDFILE* file;
	SF_INFO info;
	short* data;
	short max;


	// read input file
	file = sf_open(file_in_path, SFM_READ, &info);

	// alloc space for array of samples
	data = malloc(sizeof(short) * info.frames * info.channels);
	max = 0;
	long num_frames = info.frames;

	printf("Reading...\n");
	// read the data
	if( !sf_readf_short(file, data, info.frames) ){
		printf("Couldn't read file.");
		exit(-1);
	}

	// find max
	for(long i = 0; i < info.frames; i++){
		if(abs(data[i]) > max)
			max = abs(data[i]);
	}

	int quarter_size = info.frames / 8;
	long start_ind = 0;
	long end_ind = start_ind + quarter_size;
	SNDFILE* outfile = sf_open(file_out_path, SFM_WRITE, &info);

	/* Normalizing in chunks */
	printf("Normalzing..\n");
	while(start_ind < num_frames){
		short* buff = malloc(sizeof(short) * quarter_size);
		start_ind = end_ind + 1;
		end_ind = start_ind + quarter_size;

		if(start_ind + quarter_size > num_frames)
			break;

		memcpy(buff, data + start_ind, quarter_size);
		WaveData normed_chunk = peak_normalize(data, quarter_size, max);

		sf_write_double(outfile, normed_chunk.data, quarter_size);

		free(normed_chunk.data);
		free(buff);
	}

	sf_close(outfile);
	sf_close(file);
	free(data);

	printf("Finished normalizing.\n");
	return 0;
}
