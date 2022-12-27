/* 必要なモジュールをロード */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "audio_file_manipulator.h"

/* RIFF CHUNKの内容を初期化する関数 */
void initialize_riff_chunk(RIFF_CHUNK *riff_chunk){
    memset(riff_chunk->id, '\0', sizeof(riff_chunk->id));
    memset(riff_chunk->format_type, '\0', sizeof(riff_chunk->format_type));
}

/* FORMAT CHUNKの内容を初期化する関数 */
void initialize_format_chunk(FORMAT_CHUNK *format_chunk){
    memset(format_chunk->id, '\0', sizeof(format_chunk->id));
}

/* DATA CHUNKの内容を初期化する関数 */
void initialize_data_chunk(DATA_CHUNK *data_chunk){
    memset(data_chunk->id, '\0', sizeof(data_chunk->id));
}

/* RIFF CHUNKの内容を読み込む関数 */
void load_riff_chunk(RIFF_CHUNK *riff_chunk, FILE *audio_file){
    initialize_riff_chunk(riff_chunk);
    fread(riff_chunk->id, sizeof(char), (sizeof(riff_chunk->id) - 1), audio_file);
    fread(&riff_chunk->size, sizeof(long), 1, audio_file);
    fread(riff_chunk->format_type, sizeof(char), (sizeof(riff_chunk->format_type) - 1), audio_file);
}

/* FORMAT CHUNKの内容を読み込む関数 */
void load_format_chunk(FORMAT_CHUNK *format_chunk, FILE *audio_file){
    initialize_format_chunk(format_chunk);
    fread(format_chunk->id, sizeof(char), (sizeof(format_chunk->id) - 1), audio_file);
    fread(&format_chunk->size, sizeof(long), 1, audio_file);
    fread(&format_chunk->compression_code, sizeof(short), 1, audio_file);
    fread(&format_chunk->channel, sizeof(short), 1, audio_file);
    fread(&format_chunk->sample_rate, sizeof(long), 1, audio_file);
    fread(&format_chunk->bytes_per_second, sizeof(long), 1, audio_file);
    fread(&format_chunk->block_size, sizeof(short), 1, audio_file);
    fread(&format_chunk->bits_per_sample, sizeof(short), 1, audio_file);
}

/* DATA CHUNKの内容を読み込む関数 */
void load_data_chunk(DATA_CHUNK *data_chunk, FILE *audio_file){
    initialize_data_chunk(data_chunk);
    fread(data_chunk->id, sizeof(char), (sizeof(data_chunk->id) - 1), audio_file);
    fread(&data_chunk->size, sizeof(long), 1, audio_file);
}

/* HEADER CHUNKの内容を読み込む関数 */
void load_header_chunk(HEADER_CHUNK *header_chunk, FILE *audio_file){
    load_riff_chunk(&header_chunk->riff_chunk, audio_file);
    load_format_chunk(&header_chunk->format_chunk, audio_file);
    load_data_chunk(&header_chunk->data_chunk, audio_file);
}

/* MonoralPCM音源データを読み込む関数 */
void load_monoral_pulse_code_modulation(MONORAL_PULSE_CODE_MODULATION *monoral_pulse_code_modulation, FILE *audio_file){
    monoral_pulse_code_modulation->length = monoral_pulse_code_modulation->header_chunk.data_chunk.size / monoral_pulse_code_modulation->header_chunk.format_chunk.block_size;
    monoral_pulse_code_modulation->signal = (double *)calloc(monoral_pulse_code_modulation->length, sizeof(double));
    if(monoral_pulse_code_modulation->header_chunk.format_chunk.bits_per_sample == 16){
        for (unsigned int signal_index = 0; signal_index < monoral_pulse_code_modulation->length; signal_index++){
            short temporary_signal = 0;
            fread(&temporary_signal, (monoral_pulse_code_modulation->header_chunk.format_chunk.bits_per_sample / sizeof(double)), 1, audio_file);
            monoral_pulse_code_modulation->signal[signal_index] = (double)temporary_signal;
        }
    }
}

/* 音声信号を絶対値を計算して更新する関数 */
void calculate_double_signal_absolute(double *signal, size_t signal_length){
    for (unsigned int signal_index = 0; signal_index < signal_length; signal_index++){
        signal[signal_index] = (signal[signal_index] >= 0) ? signal[signal_index] : -signal[signal_index];
    }
}

/* 音声信号の最大値を計算する関数 */
double calculate_double_max_value(double *signal, size_t signal_length){
    double max_value = 0.0;
    for (unsigned int signal_index = 0; signal_index < signal_length; signal_index++){
        if(max_value < signal[signal_index]){
            max_value = signal[signal_index];
        }
    }
    return max_value;
}

/* 音声信号全体に対して割り算を行う関数 */
void calculate_double_signal_divide(double *signal, double value, size_t signal_length){
    for (unsigned int signal_index = 0; signal_index < signal_length; signal_index++){
        signal[signal_index] /= value;
    }
}

/* 音声信号を正規化する関数 */
void normalize_monoral_pulse_code_modulation_signal(MONORAL_PULSE_CODE_MODULATION *monoral_pulse_code_modulation){
    double *temporary_signal = (double *)calloc(monoral_pulse_code_modulation->length, sizeof(double));
    memcpy(temporary_signal, monoral_pulse_code_modulation->signal, monoral_pulse_code_modulation->length);
    calculate_double_signal_absolute(temporary_signal, monoral_pulse_code_modulation->length);
    double max_value = calculate_double_max_value(temporary_signal, monoral_pulse_code_modulation->length);
    free(temporary_signal);
    calculate_double_signal_divide(monoral_pulse_code_modulation->signal, max_value, monoral_pulse_code_modulation->length);
}

/* 音声ファイルを開く関数 */
void open_audio_file(AUDIO_FILE_MANIPULATOR *audio_file_manipulator){
    audio_file_manipulator->audio_file = fopen(audio_file_manipulator->audio_file_path, audio_file_manipulator->mode);
    if(audio_file_manipulator->audio_file == NULL){
        printf("次のファイルを開く事が出来ませんでした.\n");
        display_audio_file_properties(*audio_file_manipulator);
        exit(EXIT_FAILURE);
    }
}

/* 音声ファイルを閉じる関数 */
void close_audio_file(AUDIO_FILE_MANIPULATOR* audio_file_manipulator){
    if(audio_file_manipulator->audio_file != NULL){
        if(fclose(audio_file_manipulator->audio_file) != 0){
            printf("ファイルを正しく開く事が出来ませんでした.");
            display_audio_file_properties(*audio_file_manipulator);
            exit(EXIT_FAILURE);
        }
        audio_file_manipulator->audio_file = NULL;
    }
}

/* MonoralPCMの音データのメモリを解放する関数 */
void free_monoral_pulse_code_modulation(MONORAL_PULSE_CODE_MODULATION *monoral_pulse_code_modulation){
    free(monoral_pulse_code_modulation->signal);
}

/* RIFF CHUNKの内容を表示する関数 */
void display_riff_chunk(RIFF_CHUNK riff_chunk){
    printf(" ---------------- RIFF CHUNK ---------------- \n");
    printf("id : %s\n", riff_chunk.id);
    printf("size : %d [Byte]\n", riff_chunk.size);
    printf("format_type : %s\n", riff_chunk.format_type);
    printf(" -------------------------------------------- \n\n");
}

/* FORMAT CHUNKの内容を表示する関数 */
void display_format_chunk(FORMAT_CHUNK format_chunk){
    printf(" --------------- FORMAT CHUNK --------------- \n");
    printf("id : %s\n", format_chunk.id);
    printf("size : %ld [Byte]\n", format_chunk.size);
    printf("compression_code : %d\n", format_chunk.compression_code);
    printf("channel : %d [channel]\n", format_chunk.channel);
    printf("sample_rate : %ld [Hz]\n", format_chunk.sample_rate);
    printf("bytes_per_second : %ld [Byte/Second]\n", format_chunk.bytes_per_second);
    printf("block_size : %d [Byte/sample]\n", format_chunk.block_size);
    printf("bits_per_sample : %d [bit/sample]\n", format_chunk.bits_per_sample);
    printf(" -------------------------------------------- \n\n");
}

/* DATA CHUNKの内容を表示する関数 */
void display_data_chunk(DATA_CHUNK data_chunk){
    printf(" ---------------- DATA CHUNK ---------------- \n");
    printf("id : %s\n", data_chunk.id);
    printf("size : %d [Byte]\n", data_chunk.size);
    printf(" -------------------------------------------- \n\n");
}

/* HEADER CHUNKの内容を表示する関数 */
void display_header_chunk(HEADER_CHUNK header_chunk){
    display_riff_chunk(header_chunk.riff_chunk);
    display_format_chunk(header_chunk.format_chunk);
    display_data_chunk(header_chunk.data_chunk);
}

/* 音声ファイルの構造体のプロパティを表示する関数 */
void display_audio_file_properties(AUDIO_FILE_MANIPULATOR audio_file_manipulator){
    printf(" ---------- AUDIO_FILE_MANIPULATOR ---------- \n");
    printf("audio_file_manipulator.audio_file_path : %s\n", audio_file_manipulator.audio_file_path);
    printf("audio_file_manipulator.mode : %s\n", audio_file_manipulator.mode);
    printf("audio_file_manipulator.audio_file address : %p\n", audio_file_manipulator.audio_file);
    printf(" -------------------------------------------- \n\n");
}

/* MonoralPCMデータを表示する関数 */
void display_monoral_pulse_code_modulation(MONORAL_PULSE_CODE_MODULATION monoral_pulse_code_modulation){
    display_header_chunk(monoral_pulse_code_modulation.header_chunk);
    printf(" ------- MONORAL PULSE CODE MODULATION ------ \n");
    for (unsigned int signal_index = 0; signal_index < monoral_pulse_code_modulation.length; signal_index++){
        printf("index : %d, value : %lf\n", signal_index, monoral_pulse_code_modulation.signal[signal_index]);
    }
    printf(" -------------------------------------------- \n\n");
}