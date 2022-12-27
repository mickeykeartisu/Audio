#ifndef AUDIO_FILE_MANIPULATOR_H_
#define AUDIO_FILE_MANIPULATOR_H_

/* 必要なモジュールをロード */
#include <stdio.h>

/* RIFF CHUNKに関する構造体 */
typedef struct Riff_Chunk{
    char id[4 + 1];
    size_t size;
    char format_type[4 + 1];
}RIFF_CHUNK;

/* FORMAT CHUNKに関する構造体 */
typedef struct Fortmat_Chunk{
    char id[4 + 1];
    size_t size;
    unsigned short compression_code;
    unsigned short channel;
    unsigned long sample_rate;
    unsigned long bytes_per_second;
    unsigned short block_size;
    unsigned short bits_per_sample;
}FORMAT_CHUNK;

/* DATA CHUNKに関する構造体 */
typedef struct Data_Chunk{
    char id[4 + 1];
    size_t size;
}DATA_CHUNK;

/* CHUNKを纏めたHEADER CHUNKに関する構造体 */
typedef struct Header_Chunk{
    RIFF_CHUNK riff_chunk;
    FORMAT_CHUNK format_chunk;
    DATA_CHUNK data_chunk;
}HEADER_CHUNK;

/* 音声ファイルに関する構造体 */
typedef struct Audio_File_Manipulator{
    const char* audio_file_path;    // 音声ファイルのパス
    const char* mode;               // ファイルを操作する際のモード
    FILE* audio_file;               // 音声ファイルを扱うポインタ
}AUDIO_FILE_MANIPULATOR;

/* Monoral信号のPCMに関する構造体 */
typedef struct Monoral_Pulse_Code_Modulation{
    HEADER_CHUNK header_chunk;
    size_t length;
    double *signal;
} MONORAL_PULSE_CODE_MODULATION;

void initialize_riff_chunk(RIFF_CHUNK *riff_chunk); // RIFF CHUNKの内容を初期化する関数
void initialize_format_chunk(FORMAT_CHUNK *format_chunk);   // FORMAT CHUNKの内容を初期化する関数
void initialize_data_chunk(DATA_CHUNK *data_chunk); // DATA CHUNKの内容を初期化する関数

void load_format_chunk(FORMAT_CHUNK *format_chunk, FILE *audio_file);   // FORMAT CHUNKの内容を読み込む関数
void load_data_chunk(DATA_CHUNK *data_chunk, FILE *audio_file); // DATA CHUNKの内容を読み込む関数
void load_header_chunk(HEADER_CHUNK *header_chunk, FILE *audio_file);   // HEADER CHUNKの内容を読み込む関数
void load_monoral_pulse_code_modulation(MONORAL_PULSE_CODE_MODULATION *monoral_pulse_code_modulation, FILE *audio_file);  // MonoralPCM音源データを読み込む関数

void calculate_double_signal_absolute(double *signal, size_t signal_length);    // 音声信号を絶対値を計算して更新する関数
void normalize_monoral_pulse_code_modulation_signal(MONORAL_PULSE_CODE_MODULATION *monoral_pulse_code_modulation);  // 音声信号を正規化する関数
double calculate_double_max_value(double *signal, size_t signal_length);    // 音声信号の最大値を計算する関数
void calculate_double_signal_divide(double *signal, double value, size_t signal_length);  // 音声信号全体に対して割り算を行う関数

void open_audio_file(AUDIO_FILE_MANIPULATOR *audio_file_manipulator);  // 音声ファイルを開く関数
void close_audio_file(AUDIO_FILE_MANIPULATOR *audio_file_manipulator); // 音声ファイルを閉じる関数

void free_monoral_pulse_code_modulation(MONORAL_PULSE_CODE_MODULATION *monoral_pulse_code_modulation);   // MonoralPCMの音データのメモリを解放する関数

void display_riff_chunk(RIFF_CHUNK riff_chunk);    // RIFF CHUNKの内容を表示する関数
void display_format_chunk(FORMAT_CHUNK format_chunk);   // FORMAT CHUNKの内容を表示する関数
void display_data_chunk(DATA_CHUNK data_chunk);    // DATA CHUNKの内容を表示する関数
void display_header_chunk(HEADER_CHUNK header_chunk);   // HEADER CHUNKの内容を表示する関数
void display_audio_file_properties(AUDIO_FILE_MANIPULATOR audio_file_manipulator); // 音声ファイルの構造体のプロパティを表示する関数
void display_monoral_pulse_code_modulation(MONORAL_PULSE_CODE_MODULATION monoraL_pulse_code_modulation);    // MonoralPCMデータを表示する関数

#endif