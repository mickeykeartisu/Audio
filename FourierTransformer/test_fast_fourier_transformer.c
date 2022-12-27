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
        MONORAL_PULSE_CODE_MODULATION monoral_pulse_code_modulation;
        monoral_pulse_code_modulation.header_chunk = header_chunk;
        load_monoral_pulse_code_modulation(&monoral_pulse_code_modulation, audio_file_manipulator.audio_file);
        normalize_monoral_pulse_code_modulation_signal(&monoral_pulse_code_modulation);
        size_t fft_point = pow(2.0, 10);
        double *amplitude_spectrum = calculate_amplitude_spectrum(monoral_pulse_code_modulation.signal, fft_point);
        for(unsigned int amplitude_spectrum_index = 0; amplitude_spectrum_index < (fft_point / 2); amplitude_spectrum_index++){
            printf("index : %d, amplitude : %lf\n", amplitude_spectrum_index, amplitude_spectrum[amplitude_spectrum_index]);
        }
        
        free_monoral_pulse_code_modulation(&monoral_pulse_code_modulation);
        free(amplitude_spectrum);
    }
    close_audio_file(&audio_file_manipulator);

    return EXIT_SUCCESS;
}