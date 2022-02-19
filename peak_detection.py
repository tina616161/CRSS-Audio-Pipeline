from __future__ import print_function
import librosa
import numpy as np
import datetime
import matplotlib.pyplot as plt
print("enter audio file.")
audiofilename = input()
y,sr = librosa.load(audiofilename)
duration = librosa.get_duration(y)
onset_env = librosa.onset.onset_strength(y=y, sr=sr,
                                         hop_length=512,
                                         aggregate=np.median)
peaks = librosa.util.peak_pick(onset_env,
                               pre_max=3,
                               post_max=3, pre_avg=3, post_avg=5, delta=0.5, wait=10)
print('Peaks detected at: ', librosa.frames_to_time(peaks, sr=sr))

times = librosa.times_like(onset_env, sr=sr, hop_length=512)
