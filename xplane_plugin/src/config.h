#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

void readConfig();

int getNrOfLines(char* filename);
int parseConfigFile(char* filename);

void debugLog(const char* fmt, ...);
void infoLog(const char* fmt, ...);

#endif
