/*
 *hashTab Supporting Functions
 * Program 2 assignment
 * CSE109 Fall 2021
 * student ID: swk324
 * student Last name: Kravitz
 * student First name: Steven
 * github ID: 78743146
 */
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include "syscalls.h"
#ifdef DEBUG
#define DEBUG 1
#else
#define DEBUG 0
#endif

//Wrapper function for malloc that will throw an error in the case that the output of the call to malloc is null
void *Malloc(size_t bytes) {
	void *pointer = malloc(bytes);
	if(pointer == NULL) {
		fprintf(stderr, "malloc error(%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
	return pointer;
}

//wrapper for fread that throws an erorr if the output of the fread is equal to 0, if there is an end-of-file indicator, or if there is an error indicator not equal to 0
size_t Fread(void *ptr, size_t size, size_t numbuff, FILE *stream) {
	size_t elements = fread(ptr, size, numbuff, stream);
	if(elements == 0 && !feof(stream) && ferror(stream)) {
		fprintf(stderr, "fread error(%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
	return elements;
}

//wrapper for fwrite that throws an error if the output of the fwrite is equal to 0
size_t Fwrite(const void *ptr, size_t size, size_t numbuff, FILE *stream) {
	size_t elements = fwrite(ptr, size, numbuff, stream);
	if(elements == 0) {
		if(ferror(stream)) {
			fprintf(stderr, "fwrite error(%d): %s\n", errno, strerror(errno));
			exit(errno);
		}
	}
	return elements;
}

//wrapper for fopen that will throw an error if the file is not able to be opened, otherwise the space is freed
FILE *Fopen(const char *filename, const char *mode) {
	FILE *file = fopen(filename, mode);
	if(file == NULL) {
		fprintf(stderr, "fopen error (%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
	return file;
}

//wrappper for fclose that throws an error if the fclose function is unsuccessful in closing the file
void Fclose(FILE *stream) {
	if (stream && fclose(stream)) {
		fprintf(stderr, "fclose error(%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
}

//wrapper for pipe to check if it returns an error
int Pipe(int filedes[2]) {
	int p;
	if((p = pipe(filedes)) == -1) {
		fprintf(stderr, "pipe error (%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
	return p;
}

//wrapper for fork to check if it returns an error
pid_t Fork() {
	pid_t pid;
	if((pid = fork()) == -1) {
		fprintf(stderr, "fork error (%d): %s\n", errno, strerror(errno));
		exit(errno);
	}
	return (pid);
}
