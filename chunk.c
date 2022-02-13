#include <stdio.h>
#include <stdlib.h>
#include <sndfile.h>
#include <string.h>

int
main(int argc, char** argv){
	if(argc < 2){
		printf("ERROR: Not enough arguments\n");
		printf("Usage: chunk [input file path]\n");
		exit(1);
	}

	const char* dir_name = "./A11_chunks";
	const char* orig_file_name = "A11_T863_HR1U_CH01";
	const char* mission_start = "324-12-15-58";
	const char* mission_end = "325-11-58-00";
	const char* file_in_path = argv[1];

	SNDFILE* file;
	SF_INFO info;
	short* data;

	file = sf_open(file_in_path, SFM_READ, &info);
	data = malloc(sizeof(short) * info.frames * info.channels);

	printf("Reading...\n");
	if( !sf_readf_short(file, data, info.frames) ){
		printf("Couldn't read file.");
		exit(-1);
	}

	unsigned long window = info.samplerate * 60 * 30; 
	unsigned long ind = 0;
	short* buffer = malloc(sizeof(short) * window);
	int num_original_samples = info.frames;
	info.frames = (int) window;
	int count = 0;
	int done = 0;

	while(done){
		char out_file_name[250];
		sprintf(out_file_name, "%s/%s_%s_%s-%d.wav", dir_name, orig_file_name, mission_start, mission_end, count);

		if(ind + window > num_original_samples){
			window = num_original_samples - ind;
			info.frames = num_original_samples - ind;
			done = 1;
		}

		memcpy(buffer, data + ind, window * sizeof(short));

		SNDFILE* outfile = sf_open(out_file_name, SFM_WRITE, &info);
		sf_write_short(outfile, buffer, window);
		sf_write_sync(outfile);
		sf_close(outfile);

		ind += window;
	}
	sf_close(file);
}
