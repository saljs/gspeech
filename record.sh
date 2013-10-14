#!/bin/env bash

#exapmle script to record audio using arecord and sox. To use with gspeech:
#	gspeech message.flac ./record.sh

arecord -f cd -t wav tmp.wav
sox tmp.wav message.flac rate 16k
rm tmp.wav
