import speech_recognition as sr

from pydub import AudioSegment
from pydub import effects
from pydub import silence

import os

r = sr.Recognizer()
filename = "./test-hotspot.wav"

sound = AudioSegment.from_wav(filename)
chunks = silence.split_on_silence(sound, min_silence_len=500, silence_thresh=sound.dBFS-0, keep_silence=500)
os.mkdir("footest")

for ind, chunk in enumerate(chunks):
    file_name = "./{}/{}-silence-split.wav".format("footest", ind)

    normed = effects.normalize(chunk)
    lp_filter = effects.low_pass_filter(normed, 60) # low pass filter to remove white noise
    lp_filter.export(file_name, format="wav")

    with sr.AudioFile(file_name) as data:
        try:
            text = r.recognize_google(r.record(data))
            print(text)
        except sr.UnknownValueError as e:
            pass
