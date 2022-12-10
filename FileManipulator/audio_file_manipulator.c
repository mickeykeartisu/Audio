/* 必要なモジュールをロード */
#include <stdio.h>
#include <stdlib.h>
#include "audio_file_manipulator.h"

/* 音声ファイルを開く関数 */
void open_audio_file(AUDIO_FILE_MANIPULATOR* audio_file_manipulator){
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

/* 音声ファイルの構造体のプロパティを表示する関数 */
void display_audio_file_properties(AUDIO_FILE_MANIPULATOR audio_file_manipulator){
    printf(" ---------- AUDIO_FILE_MANIPULATOR ---------- \n");
    printf("audio_file_manipulator.audio_file_path : %s\n", audio_file_manipulator.audio_file_path);
    printf("audio_file_manipulator.mode : %s\n", audio_file_manipulator.mode);
    printf("audio_file_manipulator.audio_file address : %p\n", audio_file_manipulator.audio_file);
    printf(" -------------------------------------------- \n\n");
}