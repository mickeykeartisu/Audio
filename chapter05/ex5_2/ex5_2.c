#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
  MONO_PCM pcm;
  int n;
  double a0, *f0, *g0;
  
  pcm.fs = 44100; /* �W�{�����g�� */
  pcm.bits = 16; /* �ʎq�����x */
  pcm.length = pcm.fs * 4; /* ���f�[�^�̒��� */
  pcm.s = calloc(pcm.length, sizeof(double)); /* ���f�[�^ */
  
  a0 = 0.5; /* �U�� */
  
  f0 = calloc(pcm.length, sizeof(double));
  g0 = calloc(pcm.length, sizeof(double));
  
  /* ���g���̎��ԃG���x���[�v */
  f0[0] = 2500.0;
  f0[pcm.length - 1] = 1500.0;
  for (n = 0; n < pcm.length; n++)
  {
    f0[n] = f0[0] + (f0[pcm.length - 1] - f0[0]) * n / (pcm.length - 1);
  }
  for (n = 0; n < pcm.length; n++)
  {
    g0[n] = f0[0] * n + (f0[pcm.length - 1] - f0[0]) * n * n / (pcm.length - 1) / 2.0;
  }
  
  
  for (n = 0; n < pcm.length; n++)
  {
    pcm.s[n] = a0 * sin(2.0 * M_PI * g0[n] / pcm.fs);
  }
  
  wave_write_16bit_mono(&pcm, "ex5_2.wav");
  
  free(pcm.s);
  free(f0);
  free(g0);
  
  return 0;
}
