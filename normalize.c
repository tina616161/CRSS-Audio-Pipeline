#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>
#include <math.h>

/* typedef struct { */
/* 	short* data; */
/* 	long count; */
/* } WaveData; */

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

WaveData
downsample(short* data, long arr_size, int original_sample_rate, int new_sample_rate){
	printf("%ld\n", arr_size);
	unsigned long long new_size;
	double ratio, ind;
	short* out;
	long count;
	
	new_size = (unsigned long long)arr_size * (unsigned long long)new_sample_rate / (unsigned long long)original_sample_rate;
	ratio = original_sample_rate / new_sample_rate;
	count = 0;
	ind = 0;

	out = malloc(sizeof(short) * new_size);
	if(out == NULL) printf("malloc error\n");

	while(ind < arr_size){
		if(!(ind == (long)ind))
			out[count] = (short) (data[(long)ind] + data[(long)ind + 1]) / 2;
		else
			out[count] = (short) data[(long)ind];
		ind += ratio;
		count++;
	}

	return (WaveData) {out, new_size};
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


	/* printf("Normalizing...\n"); */
	/* WaveData output = peak_normalize(data, info.frames, max); */

	/* /\* printf("Downsampling...\n"); *\/ */
	/* /\* output = downsample(output.data, info.frames, info.samplerate, 8000); *\/ */

	/* /\* info.samplerate = 8000; *\/ */
	/* info.frames = output.count; */
	/* info.channels = 1; */

	/* // output path */
	/* SNDFILE* outfile = sf_open(file_out_path, SFM_WRITE, &info); */
	/* // write data to file */
	/* if(sf_writef_short(outfile, output.data, output.count) != info.frames){ */
	/* 	printf("Write error.\n"); */
	/* } */
	/* sf_write_sync(outfile); */

	/* // close file */
	/* sf_close(outfile); */
	/* sf_close(file); */
	// free data array
	free(data);

	printf("Finished normalizing.\n");
	return 0;
}
