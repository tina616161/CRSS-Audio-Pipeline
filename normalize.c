#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <math.h>

typedef struct {
	double* data;
	int count;
} WaveData;

WaveData
peak_normalize(short* data, int arr_size, short max){
	double* out = malloc(sizeof(double) * arr_size);
	double factor = 1.0 / max;

	for(int j = 0; j < arr_size; j++)
		out[j] = (double)factor * data[j];

	return (WaveData) {out, arr_size};
}

int
main(){
	char* file_name;
	SNDFILE* file;
	SF_INFO info;
	short* data;
	short max;

	file_name = "/mnt/c/Users/sesan/Desktop/CH4.wav";
	// read input file
	file = sf_open(file_name, SFM_READ, &info);

	// alloc space for array of samples
	data = malloc(sizeof(short) * info.frames * info.channels);
	max = 0;

	printf("Reading...\n");
	// read the data
	if( !sf_readf_short(file, data, info.frames) ){
		printf("Couldn't read file.");
		exit(-1);
	}

	// find max
	for(int i = 0; i < info.frames; i++){
		if(data[i] > max){
			max = data[i];
		}
	}

	printf("Normalizing...\n");
	// normalize data
	WaveData normed = peak_normalize(data, info.frames, max);

	// output path 
	const char* out_path = "./normalized-11-hours.wav";
	SNDFILE* outfile = sf_open(out_path, SFM_WRITE, &info);
	// write data to file
	sf_write_double(outfile, normed.data, normed.count);
	sf_write_sync(outfile);

	// close file
	sf_close(outfile);
	sf_close(file);
	// free data array
	free(data);

	printf("Done\n");
	exit(0);
}
