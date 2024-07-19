"""
Format WAV files to mono 11kHz and 16 bits per sample here: https://playback.fm/audio-converter
"""
import wave
import struct

def wav_to_c_array(input_file, output_file):
    # Open the WAV file
    with wave.open(input_file, 'rb') as wav_file:
        # Check if the sample rate is 11kHz and bits per sample is 16
        if wav_file.getframerate() != 11025 or wav_file.getsampwidth() != 2:
            print("Input WAV file must have a sample rate of 11kHz and 16 bits per sample.")
            return

        # Read all the frames from the WAV file
        frames = wav_file.readframes(wav_file.getnframes())

        # Convert frames to a list of 16-bit unsigned integers
        samples = struct.unpack('<' + 'h' * (len(frames) // 2), frames)
        uint16_samples = [((sample + 32768) & 0xffff) for sample in samples]

        # Write the C array to the output file
        with open(output_file, 'w') as output:
            output.write("const uint16_t wav_data[] = {\n")
            for sample in uint16_samples:
                output.write(f"\t{sample},\n")
            output.write("};\n")

        print("Conversion completed successfully.")

# Example usage:
input_file = "audio.wav" # Path to 11kHz 16-bit mono WAV file
output_file = "audio_array.hpp"  # Output file with audio C array
wav_to_c_array(input_file, output_file)