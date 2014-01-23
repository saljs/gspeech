gspeech
=======

A simple speech recognition program using Google's Web Speech API

Usage: gspeech FILE [COMMAND]

Example: gspeech message.flac ./record.sh


Notes:
FILE should be of type audio/x-flac with a sampling rate of 16000

COMMAND is executed before anything else, it's purpose is to run a script to record an audio file

gspeech needs the libcurl, and libcurl-devel packages installed


Credits:
As of yet the program is written only by me, Sal Skare. If you're intrested in contributing, I can be contacted at salskare@gmail.com
