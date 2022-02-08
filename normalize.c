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

	for(int j = 0; j < arr_size; j++){
		out[j] = (double)factor * data[j];
	}

	return (WaveData) {out, arr_size};
}

int
get_read_size(int num_frames, int frame_rate){
	int factor = 1;
	int curr = factor;
	while(1){
		if(num_frames % curr == 0)
			factor = curr;
		else if(factor > num_frames / 64)
			break;
		curr++;
	}
	return factor;
}


int
main(){
	char* file_name;
	int num_read_frames, count, buffer_size, read_size, curr_val;
	short max;

	SNDFILE* file;
	SF_INFO info;
	short* data_buffer;
	short* data;

	// file_name = "/mnt/c/Users/sesan/Downloads/CH4.wav";
	// file_name = "./violintest.wav";
	file_name = "./test-hotspot.wav";

	file = sf_open(file_name, SFM_READ, &info);
	num_read_frames = 0;

	read_size = (info.frames / info.samplerate);
	read_size = get_read_size(info.frames, info.samplerate);
	buffer_size = info.channels * read_size;

	data_buffer = malloc(sizeof(short) * buffer_size);
	data = malloc(sizeof(short) * info.frames);

	max = 0;
	count = 0;

	printf("Reading...\n");

	while( (num_read_frames = sf_readf_short(file, data_buffer, read_size)) ){
		for(int i = 0; i < buffer_size; i++){
			curr_val = data_buffer[i];
			data[count] = curr_val;
			if(abs(curr_val) > max){
				max = curr_val;
			}
			count++;
		}
	}
	free(data_buffer);

	printf("Normalizing...\n");
	WaveData normed = peak_normalize(data, count, max);

	const char* out_path = "./normalized-11-hours.wav";
	SNDFILE* outfile = sf_open(out_path, SFM_WRITE, &info);
	sf_write_double(outfile, normed.data, normed.count);
	sf_write_sync(outfile);

	sf_close(outfile);
	sf_close(file);

	free(data);
}
