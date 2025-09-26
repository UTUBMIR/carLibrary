#include <cs50.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include <errno.h>
#include <time.h>
#include "car.h"

char* baseUrl;
int baseUrlLength;

unsigned int writeCallback(void* ptr, unsigned int size, unsigned int nmemb, void* userdata) {
    struct memory* mem = (struct memory*)userdata;
    unsigned int total = size * nmemb;
    char* temp = realloc(mem->buffer, mem->size + total + 1);
    if (!temp) return 0;

    mem->buffer = temp;
    memcpy(mem->buffer + mem->size, ptr, total);
    mem->size += total;
    mem->buffer[mem->size] = '\0';
    return total;
}

responseData get(const char* path) {
    unsigned int full_len = strlen(baseUrl) + strlen(path) + 1;
    char* url = malloc(full_len);
    snprintf(url, full_len, "%s%s", baseUrl, path);

    CURL* curl;
    CURLcode res;
    long http_code = 0;
    struct memory mem = { malloc(1), 0 };
    mem.buffer[0] = '\0';

    responseData resp;
    resp.code = 0;
    resp.text = NULL;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &mem);

        res = curl_easy_perform(curl);
        if (res == CURLE_OK) {
            curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &http_code);
            resp.code = (int)http_code;
            resp.text = mem.buffer;
        } else {
            resp.code = -1;
            free(mem.buffer);
            resp.text = NULL;
        }

        curl_easy_cleanup(curl);
    }
    curl_global_cleanup();
    free(url);
    return resp;
}

// request command with format
responseData request(const char* format, ...) {
    va_list args;
    va_start(args, format);

    char* url = malloc(baseUrlLength + 60);
    vsprintf(url, format, args);

    responseData r = get(url);
    free(url);
    va_end(args);

    return r;
}

void setUrl(char* url) {
    baseUrl = url;
    baseUrlLength = strlen(url);
}

// stops program by specified amount of milliseconds
int sleepm(long msec) {
    struct timespec ts;
    int res;

    if (msec < 0)
    {
        errno = EINVAL;
        return -1;
    }

    ts.tv_sec = msec / 1000;
    ts.tv_nsec = (msec % 1000) * 1000000;

    do {
        res = nanosleep(&ts, &ts);
    } while (res && errno == EINTR);

    return res;
}

// sets speed for left and right motor, from -90 up to 90
int drive(int left, int right) {
    responseData r = request("/drive?left=%i&right=%i", left, right);
    free(r.text);

    return r.code;
}

// set speed for left and right motor, from -90 up to 90. timer specifies milliseconds untill stop
int driveWithTimer(int left, int right, int timer) {
    responseData r = request("/drive?left=%i&right=%i&timer=%i", left, right, timer);
    free(r.text);

    return r.code;
}


// turns servor for the radar to look at the specified angle
int look(int angle) {
    responseData r = request("/look?angle=%i", angle);
    free(r.text);

    return r.code;
}

// returns distance from radar to obstacle, and -1 if request failed
int watch() {
    responseData r = request("/watch");

    int result = r.code == 200 ? atoi(r.text) : -1;
    free(r.text);

    return result;
}