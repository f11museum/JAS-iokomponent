#define _GNU_SOURCE
#include <stdio.h>
#include "jasiokomp.h"
#include "config.h"

//master_struct masters[MAXMASTERS];

char filename[512];

void readConfig() {
    debugLog("readConfig \n");
    //FILE* configFile;
    char lDirName[512];
    // find out plugin path
    XPLMPluginID lMyID = XPLMGetMyID();
    XPLMGetPluginInfo(lMyID, NULL, lDirName, NULL, NULL);
    char* lFilePart = XPLMExtractFileAndPath(lDirName);
    const char* lSep = XPLMGetDirectorySeparator();
    if (lFilePart != NULL) {
        *lFilePart = 0;
        strcat(lDirName, lSep);
    }
    strcat(lDirName, "..");
    strcat(lDirName, lSep);
    sprintf(filename, "%slampor.txt", lDirName);

    debugLog("read file %s \n", filename);

    int nrOfLines = 0;
    //XPLMDebugString("jasiokomp.readConfig: getNrOfLines\n");
    nrOfLines = getNrOfLines(filename);
    debugLog("nrOfLines %d\n", nrOfLines);

    createArrays(nrOfLines);

    int nrOfDR = 0;
    //XPLMDebugString("jasiokomp.readConfig: getNrOfLines\n");
    nrOfDR = parseConfigFile(filename);
    debugLog("nrOfDR %d\n", nrOfDR);

    debugLog("jasiokomp:config: Done\n");
}

void parseLine(char* line) {
    if (line[0] == '#' || line[0] == '\n' || line[0] == '\r' || line[0] == '/') {
        return;
    }

    char name[256];
    char type[32];
    char sys[256];
    char lp[256];
    char es[256];
    char desc[512];
    int res2 = sscanf(line, "%s\t%s\t%s\t%s\t%s\t%s", name, type, sys, lp, es, desc);

    if (res2 >= 5) {
        debugLog("found dataref: name %s\t type %s\t sys %s\t lp %s\t es %s\t desc %s\n", name, type, sys, lp, es, desc);
        if (strncmp("lampa", type, 5) == 0) {

            createIOKompInt(name, type, sys, lp, es);
        }
        if (strncmp("knapp", type, 5) == 0) {

            createIOKompButton(name, type, sys, lp, es);
        }
    }
}

#if defined(WINDOWS) || defined(WINDOWS64)
/*
 POSIX getline replacement for non-POSIX systems (like Windows)
 Differences:
     - the function returns int64_t instead of ssize_t
     - does not accept NUL characters in the input file
 Warnings:
     - the function sets EINVAL, ENOMEM, EOVERFLOW in case of errors. The above are not defined by ISO C17,
     but are supported by other C compilers like MSVC
 */
int64_t getline(char** restrict line, size_t* restrict len, FILE* restrict fp) {
    // Check if either line, len or fp are NULL pointers
    if (line == NULL || len == NULL || fp == NULL) {
        errno = EINVAL;
        return -1;
    }
    // Use a chunk array of 128 bytes as parameter for fgets
    char chunk[128];

    // Allocate a block of memory for *line if it is NULL or smaller than the chunk array
    if (*line == NULL || *len < sizeof(chunk)) {
        *len = sizeof(chunk);
        if ((*line = malloc(*len)) == NULL) {
            errno = ENOMEM;
            return -1;
        }
    }
    // "Empty" the string
    (*line)[0] = '\0';

    while (fgets(chunk, sizeof(chunk), fp) != NULL) {
        // Resize the line buffer if necessary
        size_t len_used = strlen(*line);
        size_t chunk_used = strlen(chunk);

        if (*len - len_used < chunk_used) {
            // Check for overflow
            if (*len > SIZE_MAX / 2) {
                errno = EOVERFLOW;
                return -1;
            } else {
                *len *= 2;
            }

            if ((*line = realloc(*line, *len)) == NULL) {
                errno = ENOMEM;
                return -1;
            }
        }
        // Copy the chunk to the end of the line buffer
        memcpy(*line + len_used, chunk, chunk_used);
        len_used += chunk_used;
        (*line)[len_used] = '\0';

        // Check if *line contains '\n', if yes, return the *line length
        if ((*line)[len_used - 1] == '\n') {
            return len_used;
        }
    }

    return -1;
}
#endif

int getNrOfLines(char* filename) {
    int banan = 0;
    FILE* configFile;
    if ((configFile = fopen(filename, "r")) == NULL) {
        debugLog("Error! opening configfile\n");
        return 0;
    } else {

        char* line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, configFile)) != -1) {
            banan++;
            //parseLine(line);
        }
        fclose(configFile);
        return banan;
    }
    return banan;
}

int parseConfigFile(char* filename) {
    int banan = 0;
    FILE* configFile;
    if ((configFile = fopen(filename, "r")) == NULL) {
        debugLog("Error! opening configfile\n");
        return 0;
    } else {

        char* line = NULL;
        size_t len = 0;
        ssize_t read;

        while ((read = getline(&line, &len, configFile)) != -1) {
            banan++;
            parseLine(line);
        }
        fclose(configFile);
        return banan;
    }
    return banan;
}

void debugLog(const char* fmt, ...) {
#ifdef DEBUGLOG
    char buffer[1024];
    // if I ever send debug string longer than 1024 bytes - "HELIHUD: ",
    // I will never find this error why application crashes :-)
    va_list ap;
    va_start(ap, fmt);
    strcpy(buffer, "jasiokomp:  ");
    vsprintf(buffer + 9, fmt, ap);
    va_end(ap);
    XPLMDebugString(buffer);
#endif
}

void infoLog(const char* fmt, ...) {
    char buffer[1024];
    // if I ever send debug string longer than 1024 bytes - "HELIHUD: ",
    // I will never find this error why application crashes :-)
    va_list ap;
    va_start(ap, fmt);
    strcpy(buffer, "jasiokomp:  ");
    vsprintf(buffer + 9, fmt, ap);
    va_end(ap);
    XPLMDebugString(buffer);
}