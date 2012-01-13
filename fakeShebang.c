#include <stdlib.h>
#include <string.h>
#include <stdio.h>

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
		sysCallLen += strlen(argv[i]) + 1;
	}
	sysCallLen += strlen(executable)+1+strlen(script);
	sysCall = (char*)malloc(sysCallLen*sizeof(char));

	// Copy the executable string, a space, and the script string, updating an offset along the way
	strncpy(sysCall, executable, strlen(executable));
	sysCallOffset += strlen(executable);
	strncpy(sysCall+sysCallOffset, " ", 1);
	sysCallOffset++;
	strncpy(sysCall+sysCallOffset, script, strlen(script));
	sysCallOffset += strlen(script);

	// Copy all arguments for the sysCall and shift the offset accordingly (TODO: Handle arguments that need escaping)
	for(i = 1; i < argc; i++) {
		strncpy(sysCall+sysCallOffset, " ", 1);
		sysCallOffset++;
		strncpy(sysCall+sysCallOffset, argv[i], strlen(argv[i]));
		sysCallOffset += strlen(argv[i]);
	}

	// And run!
	return system(sysCall);	
}
