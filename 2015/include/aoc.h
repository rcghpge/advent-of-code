#ifndef AOC_H
#define AOC_H

#include <stddef.h> // For size_t

/**
 * Callback function to write data to a file.
 * This is used by libcurl to handle the response body.
 *
 * @param contents Pointer to the data to be written.
 * @param size Size of a single data item.
 * @param nmemb Number of data items.
 * @param userp User-provided pointer to the file stream.
 * @return Total number of bytes written.
 */
size_t write_callback(void *contents, size_t size, size_t nmemb, void *userp);

/**
 * Fetches input data for a specific day and saves it to the specified directory.
 *
 * @param directory Directory where the input file will be saved.
 * @param year The year of the Advent of Code challenge.
 * @param day The day of the challenge (1-25).
 * @param session_cookie Session cookie for authentication on the Advent of Code website.
 */
void fetch_day_data(const char *directory, int year, int day, const char *session_cookie);

#endif // AOC_H

