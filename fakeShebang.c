#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int escapedSize(const char* toEscape) {
	int i, newSize = strlen(toEscape);
	// Check if escaping is needed at all
	if(strchr(toEscape, ' ') != NULL) {
		newSize += 2;
		// Check if we need to escape any quotes
		if(strchr(toEscape, '"') != NULL) {
			for(i = 0; i < strlen(toEscape); i++) {
				if(*(toEscape+i) == '"') {
					newSize++;
				}
			}
		}
	}
	return newSize;
}

const char* escapeString(const char* toEscape) {
	int i, escapedLength = escapedSize(toEscape);
	char* escapedString;
	if(strchr(toEscape, ' ') != NULL) {
		escapedString = (char*)malloc((escapedLength+1)*sizeof(char));
		*escapedString = '"';
		*(escapedString+escapedLength) = '"';
		if(strchr(toEscape, '"') != NULL) {
			for(i = 0; i < escapedLength-2; i++) {
				if(*(toEscape+i) == '"') {
					*(escapedString+i) = '\\';
					i++;
					*(escapedString+i) = '"';
				} else {
					*(escapedString+i) = *(toEscape+i);
				}
			}
		} else {
			strncpy(escapedString+1, toEscape, strlen(toEscape));
		}
		return escapedString;
	} else {
		return toEscape;
	}
}

int main(int argc, char* argv[]) {
	// Declare vars
	const char *script = "./node_modules/test_script.js";
	char *executable, *sysCall;
	FILE *scriptFilePointer;
	int i, sysCallLen = 0, sysCallOffset = 0;

	// Allocate 1KB to store the executable name (that should be enough)
	executable = (char*)malloc(1024*sizeof(char));

	// Open the script, grab the executable, and close (TODO: Handle error grabbing file)
	scriptFilePointer = fopen(script, "r");
	fscanf(scriptFilePointer, "#!/usr/bin/env %s\n", executable);
	fclose(scriptFilePointer);

	// Calculate the total sysCall string length and allocate
	for(i = 1; i < argc; i++) {
		sysCallLen += escapedSize(argv[i]) + 1;
	}
	sysCallLen += escapedSize(executable)+1+escapedSize(script);
	sysCall = (char*)malloc(sysCallLen*sizeof(char));

	// Copy the executable string, a space, and the script string, updating an offset along the way
	strncpy(sysCall, escapeString(executable), escapedSize(executable));
	sysCallOffset += escapedSize(executable);
	strncpy(sysCall+sysCallOffset, " ", 1);
	sysCallOffset++;
	strncpy(sysCall+sysCallOffset, escapeString(script), escapedSize(script));
	sysCallOffset += escapedSize(script);

	// Copy all arguments for the sysCall and shift the offset accordingly
	for(i = 1; i < argc; i++) {
		strncpy(sysCall+sysCallOffset, " ", 1);
		sysCallOffset++;
		strncpy(sysCall+sysCallOffset, escapeString(argv[i]), escapedSize(argv[i]));
		sysCallOffset += escapedSize(argv[i]);
	}
	
	// Terminate the string
	strncpy(sysCall+sysCallOffset, "\0", 1);

	// And run!
	return system(sysCall);	
}
