CC=gcc
CFLAGS=
LDFLAGS=-lcurl
SOURCES=gspeech.c
EXECUTABLE=gspeech

all: gspeech

gspeech:
	$(CC) $(CFLAGS) $(LDFLAGS) $(SOURCES) -o $(EXECUTABLE)

clean:
	rm $(EXECUTABLE)
