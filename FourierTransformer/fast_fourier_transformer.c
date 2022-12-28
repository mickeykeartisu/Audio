/* 必要なモジュールをロード */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "fast_fourier_transformer.h"

/* 複素数の値を初期化する関数 */
void initialize_complex(double *signal, COMPLEX *complex, size_t fft_point){
    for(unsigned int complex_index = 0; complex_index < fft_point; complex_index++){
        complex[complex_index].real_number = signal[complex_index];
        complex[complex_index].imaginary_number = 0.0;
    }
}

/* 複素数の位置を置き換える関数 */
void replace_complex_position(COMPLEX *complex, unsigned int position_x, unsigned int position_y){
    double temporary_real_number = complex[position_x].real_number;
    double temoprary_imaginary_number = complex[position_x].imaginary_number;
    complex[position_x].real_number = complex[position_y].real_number;
    complex[position_x].imaginary_number = complex[position_y].imaginary_number;
    complex[position_y].real_number = temporary_real_number;
    complex[position_y].imaginary_number = temoprary_imaginary_number;
}

/* fftを行う際のポイント数を計算する関数 */
size_t calculate_fft_point(size_t signal_length){
    unsigned int fft_point = 1;
    while(fft_point < signal_length){
        fft_point *= 2;
    }
    return fft_point;
}

/* 高速フーリエ変換を行う関数 */
COMPLEX *fast_fourier_transform(double *signal, size_t fft_point){
    int *index;
    unsigned int number_of_stage = log2((double)fft_point); // fftの階段数
    COMPLEX *complex = (COMPLEX *)malloc(sizeof(COMPLEX) * fft_point);
    initialize_complex(signal, complex, fft_point);

    /* バタフライ演算 */
    for (unsigned int stage = 1; stage <= number_of_stage; stage++){
        for (unsigned int i = 0; i < pow(2.0, stage - 1); i++){
            for (unsigned int j = 0; j < pow(2.0, number_of_stage - stage); j++){
                unsigned int n = pow(2.0, number_of_stage - stage + 1) * i + j;
                unsigned int m = pow(2.0, number_of_stage - stage) + n;
                unsigned int r = pow(2.0, stage - 1) * j;
                double a_real = complex[n].real_number;
                double a_imag = complex[n].imaginary_number;
                double b_real = complex[m].real_number;
                double b_imag = complex[m].imaginary_number;
                double c_real = cos((2.0 * M_PI * r) / fft_point);
                double c_imag = -sin((2.0 * M_PI * r) / fft_point);

                if (stage < number_of_stage){
                    complex[n].real_number = a_real + b_real;
                    complex[n].imaginary_number = a_imag + b_imag;
                    complex[m].real_number = (a_real - b_real) * (c_real) - (a_imag - b_imag) * c_imag;
                    complex[m].imaginary_number = (a_imag - b_imag) * c_real + (a_real - b_real) * c_imag;
                }
                else{
                    complex[n].real_number = a_real + b_real;
                    complex[n].imaginary_number = a_imag + b_imag;
                    complex[m].real_number = a_real - b_real;
                    complex[m].imaginary_number = a_imag - b_imag;
                }
            }
        }
    }

    /* インデックスの並び替えのためのテーブルの作成 */
    index = calloc(fft_point, sizeof(int));
    for (unsigned int stage = 1; stage <= number_of_stage; stage++){
        for (unsigned int i = 0; i < pow(2.0, stage - 1); i++){
            index[(unsigned int)pow(2.0, stage - 1) + i] = index[i] + pow(2.0, number_of_stage - stage);
        }
    }

    /* インデックスの並び替え */
    for (unsigned int k = 0; k < fft_point; k++){
        if (index[k] > k){
            replace_complex_position(complex, index[k], k);
        }
    }

    free(index);
    return complex;
}

/* 逆高速フーリエ変換を行う関数 */
COMPLEX *inverse_fast_fourier_transform(double *signal, size_t fft_point){
    int *index;
    unsigned int number_of_stage = log2((double)fft_point); // fftの階段数
    COMPLEX *complex = (COMPLEX *)malloc(sizeof(COMPLEX) * fft_point);
    initialize_complex(signal, complex, fft_point);

    /* バタフライ演算 */
    for (unsigned int stage = 1; stage <= number_of_stage; stage++){
        for (unsigned int i = 0; i < pow(2.0, stage - 1); i++){
            for (unsigned int j = 0; j < pow(2.0, number_of_stage - stage); j++){
                unsigned int n = pow(2.0, number_of_stage - stage + 1) * i + j;
                unsigned int m = pow(2.0, number_of_stage - stage) + n;
                unsigned int r = pow(2.0, stage - 1) * j;
                double a_real = complex[n].real_number;
                double a_imag = complex[n].imaginary_number;
                double b_real = complex[m].real_number;
                double b_imag = complex[m].imaginary_number;
                double c_real = cos((2.0 * M_PI * r) / fft_point);
                double c_imag = -sin((2.0 * M_PI * r) / fft_point);
                if (stage < number_of_stage){
                    complex[n].real_number = a_real + b_real;
                    complex[n].imaginary_number = a_imag + b_imag;
                    complex[m].real_number = (a_real - b_real) * c_real - (a_imag - b_imag) * c_imag;
                    complex[m].imaginary_number = (a_imag - b_imag) * c_real + (a_real - b_real) * c_imag;
                }
                else{
                    complex[n].real_number = a_real + b_real;
                    complex[n].imaginary_number = a_imag + b_imag;
                    complex[m].real_number = a_real - b_real;
                    complex[m].imaginary_number = a_imag - b_imag;
                }
            }
        }
    }

    /* インデックスの並び替えのためのテーブルの作成 */
    index = calloc(fft_point, sizeof(int));
    for (unsigned int stage = 1; stage <= number_of_stage; stage++){
        for (unsigned int i = 0; i < pow(2.0, stage - 1); i++){
            index[(unsigned int)pow(2.0, stage - 1) + i] = index[i] + pow(2.0, number_of_stage - stage);
        }
    }

    /* インデックスの並び替え */
    for (unsigned int k = 0; k < fft_point; k++){
        if (index[k] > k){
            replace_complex_position(complex, index[k], k);
        }
    }

    /* 計算結果をNで割る */
    for (unsigned int complex_index = 0; complex_index < fft_point; complex_index++){
        complex[complex_index].real_number /= fft_point;
        complex[complex_index].imaginary_number /= fft_point;
    }

    free(index);
    return complex;
}

/* 振幅スペクトルを計算する関数 */
double *calculate_amplitude_spectrum(double *signal, size_t fft_point){
    double *amplitude_spectrum = (double *)calloc((int)(fft_point / 2), sizeof(double));
    COMPLEX *complex = fast_fourier_transform(signal, fft_point);
    for(unsigned int amplitude_spectrum_index = 0; amplitude_spectrum_index < fft_point / 2; amplitude_spectrum_index++){
        amplitude_spectrum[amplitude_spectrum_index] = sqrt(pow(complex[amplitude_spectrum_index].real_number, 2.0) + pow(complex[amplitude_spectrum_index].imaginary_number, 2.0));
    }
    free(complex);
    return amplitude_spectrum;
}

/* パワースペクトルを計算する関数 */
double *calculate_power_spectrum(double *signal, size_t fft_point){
    double *amplitude_spectrum = calculate_amplitude_spectrum(signal, fft_point);
    for(unsigned int amplitude_spectrum_index = 0; amplitude_spectrum_index < (fft_point / 2); amplitude_spectrum_index++){
        amplitude_spectrum[amplitude_spectrum_index] = pow(amplitude_spectrum[amplitude_spectrum_index], 2.0);
    }
    return amplitude_spectrum;
}

/* 複素数のポインタを画面に表示する関数 */
void display_complex(COMPLEX *complex, size_t fft_point){
    for(unsigned int complex_index = 0; complex_index < (fft_point / 2); complex_index++){
        printf("index : %d, real_number : %lf, imaginary_number : %lf\n", complex_index, complex[complex_index].real_number, complex[complex_index].imaginary_number);
    }
}

/* 振幅スペクトルを画面に表示する関数 */
void display_amplitude_spectrum(double *amplitude_spectrum, size_t fft_point){
    for(unsigned int amplitude_spectrum_index = 0; amplitude_spectrum_index < (fft_point / 2); amplitude_spectrum_index++){
        printf("index : %d, amplitude_spectrum : %lf\n", amplitude_spectrum_index, amplitude_spectrum[amplitude_spectrum_index]);
    }
}

/* パワースペクトルを画面に表示する関数 */
void display_power_spectrum(double *power_spectrum, size_t fft_point){
    for(unsigned int power_spectrum_index = 0; power_spectrum_index < (fft_point / 2); power_spectrum_index++){
        printf("index : %d, power_spectrum : %lf\n", power_spectrum_index, power_spectrum[power_spectrum_index]);
    }
}