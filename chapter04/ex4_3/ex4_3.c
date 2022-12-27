#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "fft.h"

int main(void)
{
  MONO_PCM pcm;
  int n, k, N;
  double *x_real, *x_imag;
  
  wave_read_16bit_mono(&pcm, "sine_500hz.wav");
  
  N = 64; /* DFT�̃T�C�Y */
  
  x_real = calloc(N, sizeof(double));
  x_imag = calloc(N, sizeof(double));
  
  /* �g�` */
  for (n = 0; n < N; n++)
  {
    x_real[n] = pcm.s[n]; /* x(n)�̎����� */
    x_imag[n] = 0.0; /* x(n)�̋����� */
  }
  
  FFT(x_real, x_imag, N); /* FFT�̌v�Z���ʂ�x_real��x_imag�ɏ㏑������� */
  
  /* ���g������ */
  for (k = 0; k < N; k++)
  {
    printf("X(%d) = %f+j%f\n", k, x_real[k], x_imag[k]);
  }
  
  free(pcm.s);
  free(x_real);
  free(x_imag);
  
  return 0;
}
