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
    int threeminmark = 5*60*info.samplerate;
    long peak_index = -1;
    short peak_value = -1;

	// find max
	for(long i = 0; i < info.frames; i++){
        if(i < threeminmark && data[i]>1000) //make sure we get greatest peak value
           { peak_value = data[i];
            peak_index = i;
        }
		else if(abs(data[i]) > max)
			max = abs(data[i]);
	}

    //trim it first 0-peak_index 

	int buffer_size = info.frames / 8;
	long start_ind = peak_index + 1;
	long end_ind = start_ind + buffer_size;
	SNDFILE* outfile = sf_open(file_out_path, SFM_WRITE, &info);
	int not_finished = 0;

	/* Normalizing in chunks */
	printf("Normalizng..\n");
	while(start_ind < num_frames){
		short* buff = malloc(sizeof(short) * buffer_size);

		if(end_ind + 1 > num_frames){
			not_finished = 1;
			break;
		}
		else
			start_ind = end_ind + 1;

		end_ind = start_ind + buffer_size;
		memcpy(buff, data + start_ind, buffer_size);
		WaveData normed_chunk = peak_normalize(data, buffer_size, max);

		sf_write_double(outfile, normed_chunk.data, buffer_size);
		free(normed_chunk.data);
		free(buff);
	}

	/* if there's more samples, read the rest of them and finish normalizing 
	 * this isn't the best way to do this, change it later */
	if(not_finished){
		long frames_left = num_frames - start_ind - 1;
		short* buff = malloc(sizeof(short) * frames_left);
		memcpy(buff, data + start_ind, frames_left);
		WaveData normed_chunk = peak_normalize(data, frames_left, max);
		sf_write_double(outfile, normed_chunk.data, frames_left);
		free(normed_chunk.data);
		free(buff);
	}

	sf_close(outfile);
	sf_close(file);
	free(data);

	printf("Finished normalizing.\n");
	return 0;
}
