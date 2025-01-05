#include <curl/curl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include "aoc.h"

#ifdef STANDALONE_AOC
int main(int argc, char *argv[]) {
    printf("Info: Starting aoc...\n");

    if (argc < 3) {
        fprintf(stderr, "Usage: %s <day1 day2 ... | all> <year>\n", argv[0]);
        return 1;
    }

    const char *session_cookie = getenv("AOC_SESSION_COOKIE");
    if (!session_cookie) {
        fprintf(stderr, "Error: AOC_SESSION_COOKIE environment variable is not set.\n");
        return 1;
    }

    int year = atoi(argv[argc - 1]);
    if (year < 2000 || year > 3000) {
        fprintf(stderr, "Error: Invalid year: %d. Please provide a valid year.\n", year);
        return 1;
    }

    // Ensure the "data" directory exists
    struct stat st;
    if (stat("data", &st) == -1) {
        mkdir("data", 0700);
        printf("Info: Created directory: data\n");
    } else {
        printf("Info: Directory 'data' already exists. Skipping creation.\n");
    }

    if (strcmp(argv[1], "all") == 0) {
        for (int day = 1; day <= 25; ++day) {
            fetch_day_data("data", year, day, session_cookie);
        }
    } else {
        for (int i = 1; i < argc - 1; ++i) {
            int day = atoi(argv[i]);
            if (day < 1 || day > 25) {
                fprintf(stderr, "Error: Invalid day: %d. Day must be between 1 and 25.\n", day);
                continue;
            }
            fetch_day_data("data", year, day, session_cookie);
        }
    }

    printf("Info: Exiting aoc\n");
    return 0;
}
#endif

// Callback function to write data to a file
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp) {
    FILE *file = (FILE *)userp;
    return fwrite(contents, size, nmemb, file);
}

// Fetches input data and saves it to the specified directory
void fetch_day_data(const char *directory, int year, int day, const char *session_cookie) {
    CURL *curl;
    CURLcode res;
    char url[256];
    char filename[256];
    FILE *file;

    snprintf(url, sizeof(url), "https://adventofcode.com/%d/day/%d/input", year, day);
    snprintf(filename, sizeof(filename), "%s/%d.txt", directory, day);

    // Check if the file already exists
    file = fopen(filename, "rb");
    if (file) {
        fclose(file);
        printf("Info: File '%s' already exists. Skipping fetch.\n", filename);
        return;
    }

    file = fopen(filename, "wb");
    if (!file) {
        perror("Error: Failed to open file");
        return;
    }

    curl = curl_easy_init();
    if (!curl) {
        fprintf(stderr, "Error: Failed to initialize CURL.\n");
        fclose(file);
        return;
    }

    struct curl_slist *headers = NULL;
    char cookie_header[256];
    snprintf(cookie_header, sizeof(cookie_header), "Cookie: session=%s", session_cookie);

    headers = curl_slist_append(headers, cookie_header);
    curl_easy_setopt(curl, CURLOPT_URL, url);
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, file);
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        fprintf(stderr, "Error: curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    } else {
        printf("Info: Fetched and saved input data for Day %d to '%s'.\n", day, filename);
    }

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    fclose(file);
}

