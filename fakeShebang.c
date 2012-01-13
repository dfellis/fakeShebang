#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int main(int argc, char* argv[]) {
	// Declare vars
	const char *script = "./node_modules/test_script.js";
	char *executable, *sysCall;
	FILE *scriptFilePointer;

	// Allocate 1KB to store the executable name (that should be enough)
	executable = (char*)malloc(1024*sizeof(char));

	// Open the script, grab the executable, and close (TODO: Handle error grabbing file)
	scriptFilePointer = fopen(script, "r");
	fscanf(scriptFilePointer, "#!/usr/bin/env %s\n", executable);
	fclose(scriptFilePointer);

	// Build the sysCall string
	sysCall = (char*)malloc((strlen(executable)+strlen(script)+1)*sizeof(char));
	strncpy(sysCall, executable, strlen(executable));
	strncpy(sysCall+strlen(executable), " ", 1);
	strncpy(sysCall+strlen(executable)+1, script, strlen(script));

	// And run!
	return system(sysCall);	
}
