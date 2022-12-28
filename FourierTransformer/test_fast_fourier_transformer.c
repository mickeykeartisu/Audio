/* load modules */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>

#include "../AudioFileManipulator/audio_file_manipulator.h"
#include "fast_fourier_transformer.h"

/* conduct main function */
int main(int argc, char *argv[]){
    /* set audio file path and mode */
    const char *audio_file_path = "D:/Audio/chapter01/ex1_1/a.wav";
    const char *mode = "rb";

    /* open audio file and confirm properties */
    AUDIO_FILE_MANIPULATOR audio_file_manipulator = {audio_file_path, mode};
    open_audio_file(&audio_file_manipulator);
    display_audio_file_properties(audio_file_manipulator);

    /* load Header Chunk and confirm properties */
    HEADER_CHUNK header_chunk;
    load_header_chunk(&header_chunk, audio_file_manipulator.audio_file);
    display_header_chunk(header_chunk);

    /* format : WAVE, compression_code : linear, channel : monoral */
    if ((strcmp(header_chunk.riff_chunk.format_type, "WAVE") == 0) && (header_chunk.format_chunk.compression_code == 1) && (header_chunk.format_chunk.channel == 1)){
        MONORAL_PULSE_CODE_MODULATION monoral_pulse_code_modulation = {header_chunk};
        load_monoral_pulse_code_modulation(&monoral_pulse_code_modulation, audio_file_manipulator.audio_file);
        normalize_monoral_pulse_code_modulation_signal(&monoral_pulse_code_modulation);

        size_t fft_point = calculate_fft_point(monoral_pulse_code_modulation.length);
        double *power_spectrum = calculate_power_spectrum(monoral_pulse_code_modulation.signal, fft_point);
        // display_power_spectrum(power_spectrum, fft_point);

        free_monoral_pulse_code_modulation(&monoral_pulse_code_modulation);
        free(power_spectrum);
    }
    close_audio_file(&audio_file_manipulator);

    return EXIT_SUCCESS;
}