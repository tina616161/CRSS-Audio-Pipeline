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

	unsigned long window = info.samplerate * 60 * 30; // number of samples in 30 minutes of audio
	unsigned long ind = 0;
	unsigned long ind2;
	short* buffer = malloc(sizeof(short) * window);
	info.frames = (int) window;

	while(1){
		char out_file_name[200];
		out_file_name = "./chunks/chunk";

		strcat(out_file_name, "1.wav");
		break;

		/* memcpy(buffer, data + ind, window * sizeof(short)); */
		/* SNDFILE* outfile = sf_open("./chunk1.wav", SFM_WRITE, &info); */
		/* sf_write_short(outfile, buffer, window); */
		/* sf_write_sync(outfile); */
		/* sf_close(outfile); */
		/* break; */
	}
	sf_close(file);
}
