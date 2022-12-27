#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
  MONO_PCM pcm0, pcm1;
  int n, i;
  double f0, gain;
  
  pcm0.fs = 44100; /* 標本化周波数 */
  pcm0.bits = 16; /* 量子化精度 */
  pcm0.length = pcm0.fs * 1; /* 音データの長さ */
  pcm0.s = calloc(pcm0.length, sizeof(double)); /* 音データ */
  
  f0 = 500.0; /* 基本周波数 */
  
  /* 基本音を含む音 */
  for (i = 1; i <= 44; i++)
  {
    for (n = 0; n < pcm0.length; n++)
    {
      pcm0.s[n] += sin(2.0 * M_PI * i * f0 * n / pcm0.fs);
    }
  }
  
  gain = 0.01; /* ゲイン */
  
  for (n = 0; n < pcm0.length; n++)
  {
    pcm0.s[n] *= gain;
  }
  
  wave_write_16bit_mono(&pcm0, "ex4_4a.wav");
  
  pcm1.fs = 44100; /* 標本化周波数 */
  pcm1.bits = 16; /* 量子化精度 */
  pcm1.length = pcm1.fs * 1; /* 音データの長さ */
  pcm1.s = calloc(pcm1.length, sizeof(double)); /* 音データ */
  
  f0 = 500.0; /* 基本周波数 */
  
  /* 基本音を含まない音 */
  for (i = 2; i <= 44; i++)
  {
    for (n = 0; n < pcm1.length; n++)
    {
      pcm1.s[n] += sin(2.0 * M_PI * i * f0 * n / pcm1.fs);
    }
  }
  
  gain = 0.01; /* ゲイン */
  
  for (n = 0; n < pcm1.length; n++)
  {
    pcm1.s[n] *= gain;
  }
  
  wave_write_16bit_mono(&pcm1, "ex4_4b.wav");
  
  free(pcm0.s);
  free(pcm1.s);
  
  return 0;
}
