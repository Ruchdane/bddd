#include "log.h"

void printTime(){
	time_t raw;
	struct tm *detailed;
    // Obtain current time
    // `time()` returns the current time of the system as a `time_t` value
    time(&raw);
	detailed = localtime(&raw);
    // Convert to local time format and print to stdout
    printf("[%02d/%02d/%2d %02d:%02d:%02d]",detailed->tm_mday, detailed->tm_mon,detailed->tm_year+ 1900,
	detailed->tm_hour,detailed->tm_min,detailed->tm_sec);
}