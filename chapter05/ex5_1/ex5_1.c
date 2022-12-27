#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
  MONO_PCM pcm;
  int n;
  double *a0, f0;
  
  pcm.fs = 44100; /* �W�{�����g�� */
  pcm.bits = 16; /* �ʎq�����x */
  pcm.length = pcm.fs * 4; /* ���f�[�^�̒��� */
  pcm.s = calloc(pcm.length, sizeof(double)); /* ���f�[�^ */
  
  a0 = calloc(pcm.length, sizeof(double));
  
  /* �U���̎��ԃG���x���[�v */
  a0[0] = 0.5;
  a0[pcm.length - 1] = 0.0;
  for (n = 0; n < pcm.length; n++)
  {
    a0[n] = a0[0] + (a0[pcm.length - 1] - a0[0]) * n / (pcm.length - 1);
  }
  
  f0 = 500.0; /* ���g�� */
  
  for (n = 0; n < pcm.length; n++)
  {
    pcm.s[n] = a0[n] * sin(2.0 * M_PI * f0 * n / pcm.fs);
  }
  
  wave_write_16bit_mono(&pcm, "ex5_1.wav");
  
  free(pcm.s);
  free(a0);
  
  return 0;
}
