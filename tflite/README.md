# Description 
The two major custom tflite operators that needed these scripts come also bundled and modified for Tensorflow Lite Micro.

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

_Still in testing_

## audio_spectrogram model 
Model dump containing audio_spectrogram generated from xxd. (With output dimensions hard-coded)

![audio_sepec](./pics/pic_8.png)

Include this into your main.cpp. 

## audio_spectrogram + MFCC model
Model dump containing audio_spectrogram and MFCC generated from xxd. (With output dimensions hard-coded)

![mfccandspec](./pics/pic_9.png)

Include this into your main.cpp. 


# How to generate custom TFLite models (flatbuffers-compiler)
## Why it is needed?
Looking into common.c for TFLite, an important command used as return in `<op_name>::Prepare()` is missing from TFLite Micro : **ResizeTensor**.

Since most of the models will work on static and continous data streams, the output size of tensors inside models will not change. As a result, it is possible to "hard-code" these sizes into the model and evade `return context->ResizeTensor(...)`. The output size usually depends on the operator's parameters or the size of its input (this is generally known). Please make sure to research about the expected output size and type of the operators before changing the models. Otherwise, a test-run with an output->dims->size etc.. printing on Tensorflow Lite should give insight on the expected dims. 


## How is it done?
To change the models manually, Google/Flatbuffers offer a [schema compiler](https://google.github.io/flatbuffers/flatbuffers_guide_using_schema_compiler.html). 

1. First, we train the model and export it as a .tflite binary file. More information [here](https://www.tensorflow.org/lite/guide/get_started)
2. On your Operating System of choice, install [https://github.com/google/flatbuffers](https://github.com/google/flatbuffers). (Can be also installed as a [pk](https://packages.debian.org/sid/devel/flatbuffers-compiler)
. Transform the `.tflite` model into an easily understandble verbose language of choice. We use `.json`:
```
flatc -t --strict -json  --defaults -json  schema_path  -- filename.tflite
```
A `schema.fbs` for Tensorflow Lite can be found and downloaded here [here](https://github.com/tensorflow/tensorflow/blob/master/tensorflow/lite/schema/schema.fbs). Do not modify this file as it is used to "translate". 

3. Inside the filename.json, look for 
```
"subgraphs": [
    {
      "tensors": [
        {
          "shape": [
```
For every tensor, there are mainly a shape, a type, a buffer, and a name. 

To every operator belongs at least 1 input tensor and 1 output tensor. 

You can change names or descriptions, but it is recommended to change as few as possible to respect the schema. 

4. Generate from the `.json` the `.tflite` model again:
```
flatc -b schema_path  filename.json
```

**Original Audio Spectrogram model before transformation:**
![audio_sepec](./pics/pic_9.png)

5. Convert the model into hex:
```
xxd -i model.tflite model.cc
```

6. Exchange the model in your main.cpp with the new one. 


