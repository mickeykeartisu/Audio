# Audio
## リポジトリについて
+ このリポジトリは音声に関するデータをC言語で簡単に扱えるようにしたライブラリである.

## 使い方について
### AudioFileManipulatorについて
+ 音声の中でも特に音声ファイルの入出力に関する関数や構造体が定義されている.
+ 構造体について
    + AUDIO_FILE_MANIPULATOR : 音声ファイルに関する構造体
        + const char* audio_file_path : 音声ファイルのパスを扱う変数
        + const char* mode : 音声ファイルを操作する際のモードを扱う変数
        + FILE* audio_file : 音声ファイルを扱うポインタ変数
+ 関数について
    + void open_audio_file(AUDIO_FILE_MANIPULATOR* audio_file_manipulator) : 音声ファイルを開く関数
    + void close_audio_file(AUDIO_FILE_MANIPULATOR* audio_file_manipulator) : 音声ファイルを閉じる関数
    + void display_audio_file_manipulator_properties(AUDIO_FILE_MANIPULATOR audio_file_manipulator) : 音声ファイルの構造体のプロパティを表示する関数
    