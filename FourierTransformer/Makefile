test_fast_fourier_transformer: test_fast_fourier_transformer.o fast_fourier_transformer.o audio_file_manipulator.o
	gcc -o test_fast_fourier_transformer test_fast_fourier_transformer.o fast_fourier_transformer.o audio_file_manipulator.o

test_fast_fourier_transformer.o: test_fast_fourier_transformer.c
	gcc -c test_fast_fourier_transformer.c

fast_fourier_transformer.o: fast_fourier_transformer.c
	gcc -c fast_fourier_transformer.c

audio_file_manipulator.o: ../AudioFileManipulator/audio_file_manipulator.c
	gcc -c ../AudioFileManipulator/audio_file_manipulator.c