#ifndef FAST_FOURIER_TRANSFORMER_H_
#define FAST_FOURIER_TRANSFORMER_H_

/* 複素数を扱う構造体 */
typedef struct Complex{
    double real_number; // 実部
    double imaginary_number;    // 虚部
}COMPLEX;

void initialize_complex(double *signal, COMPLEX *complex, size_t fft_point);    // 複素数の値を初期化する関数
void replace_complex_position(COMPLEX *complex, unsigned int position_x, unsigned int position_y); // 複素数の位置を置き換える関数

size_t calculate_fft_point(size_t signal_length);   // fftを行う際のポイント数を計算する関数

COMPLEX *fast_fourier_transform(double *signal, size_t fft_point); // 高速フーリエ変換を行う関数
COMPLEX *inverse_fast_fourier_transform(double *signal, size_t fft_point);  // 逆高速フーリエ変換を行う関数

double *calculate_amplitude_spectrum(double *signal, size_t fft_point); // 振幅スペクトルを計算する関数
double *calculate_power_spectrum(double *signal, size_t fft_point); // パワースペクトルを計算する関数

void display_complex(COMPLEX *complex, size_t fft_point);   // 複素数のポインタを画面に表示する関数
void display_amplitude_spectrum(double *amplitude_spectrum, size_t fft_point);  // 振幅スペクトルを画面に表示する関数
void display_power_spectrum(double *power_spectrum, size_t fft_point);  // パワースペクトルを画面に表示する関数

#endif