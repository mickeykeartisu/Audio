/* 必要なモジュールをロード */
#include <stdlib.h>
#include <stdio.h>
#include "audio_file_manipulator.h"

/* メイン関数 */
int main(int argc, char* argv[]){
    const char* audio_file_path = "D:/アカペラ/JP-act/花企画/2022_12_7/Mixdown/花 4/花 4.wav";
    const char* mode = "rb";

    AUDIO_FILE_MANIPULATOR audio_file_manipulator = {audio_file_path, mode};
    open_audio_file(&audio_file_manipulator);
    display_audio_file_properties(audio_file_manipulator);
    close_audio_file(&audio_file_manipulator);

    getchar();
    return EXIT_SUCCESS;
}