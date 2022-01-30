import math
import os
from pydub import AudioSegment
from pydub.utils import make_chunks

# cconvert minutes to milliseconds
min_to_mil = lambda x : x * 60 * 1000

# chunks file, and stores all chunks in a directory
# chunk names follow this format: ./{original file name}_chunks/{start min}-{end min}.mp3
def chunk_file(file_name: str, chunk_size: int):
    data = AudioSegment.from_mp3(file_name)
    chunks = make_chunks(data, min_to_mil(chunk_size))
    folder_name = "{}_chunks".format(file_name.split(".")[0])
    os.makedirs(folder_name)
    for ind, chunk in enumerate(chunks):
        end = round(data.duration_seconds / 60,1) if ind == len(chunks) - 1 else (ind + 1) * chunk_size
        chunk.export("./{}/{}-{}.mp3".format(folder_name, ind * chunk_size, end), format='mp3')

chunk_file('11-03303.mp3', 10)
