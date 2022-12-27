#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"
#include "window_function.h"

int main(void)
{
  MONO_PCM pcm;
  int n, k, N;
  double *x_real, *x_imag, *X_real, *X_imag, *w, W_real, W_imag;
  
  wave_read_16bit_mono(&pcm, "sine_500hz.wav");
  
  N = 64; /* DFTのサイズ */
  
  x_real = calloc(N, sizeof(double));
  x_imag = calloc(N, sizeof(double));
  X_real = calloc(N, sizeof(double));
  X_imag = calloc(N, sizeof(double));
  w = calloc(N, sizeof(double));
  
  Hanning_window(w, N); /* ハニング窓 */
  
  /* 波形 */
  for (n = 0; n < N; n++)
  {
    x_real[n] = pcm.s[n] * w[n]; /* x(n)の実数部 */
    x_imag[n] = 0.0; /* x(n)の虚数部 */
  }
  
  /* DFT */
  for (k = 0; k < N; k++)
  {
    for (n = 0; n < N; n++)
    {
      W_real = cos(2.0 * M_PI * k * n / N);
      W_imag = -sin(2.0 * M_PI * k * n / N);
      X_real[k] += W_real * x_real[n] - W_imag * x_imag[n]; /* X(k)の実数部 */
      X_imag[k] += W_real * x_imag[n] + W_imag * x_real[n]; /* X(k)の虚数部 */
    }
  }
  
  /* 周波数特性 */
  for (k = 0; k < N; k++)
  {
    printf("X(%d) = %f+j%f\n", k, X_real[k], X_imag[k]);
  }
  
  free(pcm.s);
  free(x_real);
  free(x_imag);
  free(X_real);
  free(X_imag);
  free(w);
  
  return 0;
}
