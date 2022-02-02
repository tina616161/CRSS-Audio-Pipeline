from pydub import AudioSegment, effects
import sys
import matplotlib.pyplot as plt

if __name__ == "__main__":
    assert len(sys.argv) >= 2

    file_name = sys.argv[1]

    # read file
    sound = AudioSegment.from_wav(file_name)
    # normalize
    normalized = effects.normalize(sound)
    # down sample file
    down_sampled = normalized.set_frame_rate(8000)

    '''
    arr = down_sampled.get_array_of_samples()
    # graphs audio samples and shows hotspots
    plt.plot(list(range(len(arr))), arr)
    plt.show()
    '''
