# Description 
The two major custom tflite operators that needed these scripts come also bundled and modified for tensorflow lite micro.

Though there isn't much changed, as most of the code is purely based on the theoretical side of the ops, these files have been used to test the hooks.

# Contents
## audio_spectrogram.cc
Post this inside tflite/kernels. (You can also backup the old file)
To register the kernel: (__unchanged namespace__)
```
  resolver.AddCustom("AudioSpectrogram",
                     tflite::ops::custom::Register_AUDIO_SPECTROGRAM());
```
## mfcc.cc
Post this inside tflite/kernels. (You can also backup the old file)
To register the kernel: (__unchanged namespace__)
```
resolver.AddCustom("Mfcc", tflite::ops::custom::Register_MFCC());
```

## audio_spectrogram model 
