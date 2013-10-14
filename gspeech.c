/*
 * gspeech.c
 *
 * Copyright 2013 Sal Skare <salskare@gmail.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 *
 *
 */
#include <stdio.h>
#include <curl/curl.h>
#include <stdlib.h>
#include <string.h>

char* getaudio(int* filesize, char* inputfile);

size_t parse_json( void *ptr, size_t size, size_t nmemb, void *stream)		//function to parse returned json for recognized text
{
    char* tmp = malloc(size * nmemb);
    if(tmp == NULL)
        return 1;
    strcpy(tmp, ptr);

    char prev = tmp[0];
    char next = tmp[1];
    int i = 1;
    while(next != '\0')
    {
        if(prev == '[' && next == '{')
        {
            i = i + 14;
            char *message = malloc(size * nmemb);
            int j = 0;
            while(tmp[i] != '"')
            {
                message[j] = tmp[i];
                i++;
                j++;
            }
            message[j] = '\0';
            printf("%s\n", message);
            free(message);
            break;
        }
        prev = tmp[i];
        i++;
        next = tmp[i];
    }
    free(tmp);
}

int main(int argc, char *argv[])
{
    if(argc == 3)
    {
        system(argv[2]);
    }
    else if(argc != 2)
    {
        printf("Usage: %s FILE [COMMAND]\n", argv[0]);
        return 1;
    }
    CURL *curl;
    CURLcode res;

    int postsize;
    char* topost = getaudio(&postsize, argv[1]);
    if(topost == NULL)
        return 1;

    /* In windows, this will init the winsock stuff */
    curl_global_init(CURL_GLOBAL_ALL);

    /* get a curl handle */
    curl = curl_easy_init();
    if(curl) {

        //set some custom headers
        struct curl_slist *chunk = NULL;
        chunk = curl_slist_append(chunk, "Content-Type: audio/x-flac; rate=16000");
        chunk = curl_slist_append(chunk, "User-Agent: Mozilla/5.0");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, chunk);

        //Set URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://www.google.com/speech-api/v1/recognize?lang=en-en");

        /* Now specify the POST data */
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, topost);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, postsize);

        /* Perform the request, res will get the return code */
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, parse_json);
        res = curl_easy_perform(curl);

        /* always cleanup */
        free(topost);
        curl_slist_free_all(chunk);
        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    return 0;
}

char* getaudio(int* filesize, char* inputfile)		//function to open audio file
{
    char *buffer;
    FILE *fh = fopen(inputfile, "rb");
    if ( fh != NULL )
    {
        fseek(fh, 0L, SEEK_END);
        long s = ftell(fh);
        rewind(fh);
        buffer = malloc(s);
        if ( buffer != NULL )
        {
            fread(buffer, s, 1, fh);
            fclose(fh);
            fh = NULL;
            *filesize = s;
        }
        if (fh != NULL) fclose(fh);
    }
    else
    {
        fprintf(stderr, "File \"%s\" not found\n", inputfile);
        return NULL;
    }
    return buffer;
}
