import torchaudio

def confirm_signal_shape(signal):
    print(signal.shape)

def test():
    audio_path = "D:/Audio/chapter01/ex1_1/a.wav"
    signal, sample_rate = torchaudio.load(audio_path)
    confirm_signal_shape(signal=signal)

if __name__ == "__main__":
    test()