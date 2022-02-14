# CRSS Audio Processing Pipeline

## Requirements
- `python3`
- `pydub==0.25.1`
- `SpeechRecognition==3.8.1`
- `libsndfile`
- [ffmpeg](https://www.ffmpeg.org/download.html)

## Running the pipeline
- `make`
- `./process [input] [mission] [tape no.] [recorder no.] [channel no.] [mission start] [mission end]`
- mission start and ends should be in this format:
  - `[day]-[hour]-[minute]-[seconds]`
- creates a directory called `A[mission number]_chunks` and saves normalized, downsampled, 30 minute chunks in this folder
  - assumes that this directory doesn't already exist, throws an error if it already exists

## Chunking
`python3 chunk_.py 11-03303.mp3 10`
- first arg is file name
- second arg is chunk size in minutes

## Normalizing and Downsampling
- run `make` to build
- `./normalize [input file path] [output file path]`
- normalizes and downsamples wav file

## 
- `11-03303.mp3` contains audio from apollo 11 mission
- `normalize.py` normalizes and downsamples audio
- `speech_to_text.py` finds hotspots, saves hotspots as wavefile, and converts recognized speech in the hotspots to text
