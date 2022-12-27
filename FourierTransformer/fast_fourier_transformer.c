/* 必要なモジュールをロード */
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include "fast_fourier_transformer.h"

void initialize_complex(double *signal, COMPLEX *complex, size_t fft_point){
    for(unsigned int complex_index = 0; complex_index < fft_point; complex_index++){
        complex[complex_index].real_number = signal[complex_index];
        complex[complex_index].imaginary_number = 0.0;
    }
}

void replace_complex_position(COMPLEX *complex, unsigned int position_x, unsigned int position_y){
    double temporary_real_number = complex[position_x].real_number;
    double temoprary_imaginary_number = complex[position_x].imaginary_number;
    complex[position_x].real_number = complex[position_y].real_number;
    complex[position_x].imaginary_number = complex[position_y].imaginary_number;
    complex[position_y].real_number = temporary_real_number;
    complex[position_y].imaginary_number = temoprary_imaginary_number;
}

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

COMPLEX *IFFT(double *signal, size_t fft_point){
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

double *calculate_amplitude_spectrum(double *signal, size_t fft_point){
    double *amplitude_spectrum = (double *)calloc((int)(fft_point / 2), sizeof(double));
    COMPLEX *complex = fast_fourier_transform(signal, fft_point);
    for(unsigned int amplitude_spectrum_index = 0; amplitude_spectrum_index < fft_point / 2; amplitude_spectrum_index++){
        amplitude_spectrum[amplitude_spectrum_index] = sqrt(pow(complex[amplitude_spectrum_index].real_number, 2.0) + pow(complex[amplitude_spectrum_index].imaginary_number, 2.0));
    }
    free(complex);
    return amplitude_spectrum;
}