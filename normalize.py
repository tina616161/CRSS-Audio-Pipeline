from pydub import AudioSegment, effects

sound = AudioSegment.from_mp3("./11-03303_chunks/0-10.mp3")
normed = effects.normalize(sound)
lp_filter = effects.low_pass_filter(normed, 60) # low pass filter to remove white noise
lp_filter.export("out.mp3", format="mp3")
