#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "wave.h"

int main(void)
{
  MONO_PCM pcm;
  int n;
  double a, f0;
  
  pcm.fs = 44100; /* �W�{�����g�� */
  pcm.bits = 16; /* �ʎq�����x */
  pcm.length = pcm.fs * 1; /* ���f�[�^�̒��� */
  pcm.s = calloc(pcm.length, sizeof(double)); /* ���f�[�^ */
  
  a = 0.1; /* �U�� */
  f0 = 500.0; /* ���g�� */
  
  /* �T�C���g */
  for (n = 0; n < pcm.length; n++)
  {
    pcm.s[n] = a * sin(2.0 * M_PI * f0 * n / pcm.fs);
  }
  
  wave_write_16bit_mono(&pcm, "ex2_1.wav");
  
  free(pcm.s);
  
  return 0;
}
