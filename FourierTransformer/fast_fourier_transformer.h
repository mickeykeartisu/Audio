#ifndef FAST_FOURIER_TRANSFORMER_H_
#define FAST_FOURIER_TRANSFORMER_H_

/* 必要なモジュールをロード */


/* 複素数を扱う構造体 */
typedef struct Complex{
    double real_number; // 実部
    double imaginary_number;    // 虚部
}COMPLEX;

COMPLEX *fast_fourier_transform(double *signal, size_t fft_point);  // 高速フーリエ変換を行う関数
double *calculate_amplitude_spectrum(double *signal, size_t fft_point); // 振幅スペクトルを計算する関数

#endif