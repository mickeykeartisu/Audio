#ifndef AUDIO_FILE_MANIPULATOR_H_
#define AUDIO_FILE_MANIPULATOR_H_

/* 必要なモジュールをロード */
#include <stdio.h>

/* 音声ファイルに関する構造体 */
typedef struct Audio_File_Manipulator{
    const char* audio_file_path;    // 音声ファイルのパス
    const char* mode;               // ファイルを操作する際のモード
    FILE* audio_file;               // 音声ファイルを扱うポインタ
}AUDIO_FILE_MANIPULATOR;

void open_audio_file(AUDIO_FILE_MANIPULATOR* audio_file_manipulator);   // 音声ファイルを開く関数
void close_audio_file(AUDIO_FILE_MANIPULATOR* audio_file_manipulator);  // 音声ファイルを閉じる関数
void display_audio_file_properties(AUDIO_FILE_MANIPULATOR audio_file_manipulator);  // 音声ファイルの構造体のプロパティを表示する関数

#endif