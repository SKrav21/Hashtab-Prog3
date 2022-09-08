/*
 * hashTab Supporting Functions
 * Program 2 assignment
 * CSE109 Fall 2021
 * student ID: swk324
 * student Last name: Kravitz
 * student First name: Steven
 * gitHub ID: 78743146
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include "global_defs.h"
#include "hashTab_funcs.h"
#include "syscalls.h"
#ifndef HASHTAB_FUNCS
#define HASHTAB_FUNCS
#endif
static size_t TotalBuckets = INITIALBUCKETS;
static size_t WorstChainLength = 0;
static size_t BucketsUsed = 0;

/* Function to parse through the command line argument and ensure that the file is properly inputted in the correct format
 */
void parseArgs(int argc, char *argv[]) {
	int inputSpecified = 0;
	if(argc == 1) {//as per the reference, if the user does not include any args, the help is printed
		help();
	}
	for(int i = 1; i < argc; i++) {
		if((strcmp(argv[i], "-i") == 0) || (strcmp(argv[i], "--input") == 0)) { 
			if (inputSpecified == 0) {
				i++;
				if(i < argc) {
					inputSpecified++; 
				}
				else {//error printed if there is -i but no file
					fprintf(stderr, "No argument after '-i|--input'\n");
					exit(203);
				}
			}
			else {//error specified if the user specifies more than one input
				fprintf(stderr, "Input file already specified\n");
				exit (202); 
			}
		}
		else if((strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0)) {
			help();
			exit(0);
		}
		else{//error printed if incorrect specifier used
			fprintf(stderr, "Incorrect/unexpected argument entered\n");
			exit(201);
		}
	}
}

/* Function that creates the hashTable
 * If the hashtable inputted is null, it creates a blank one, otherwise, it calls the copyHashTable function to copy it over (with the size of numbuckets)
 */
Node **createHashTable(Node **hashTable, size_t numBuckets) {
	size_t temp = TotalBuckets;
	TotalBuckets = numBuckets;
	if(hashTable == NULL) {
		hashTable = (Node **)Malloc(sizeof(Node) * numBuckets);
		for(size_t i = 0; i < numBuckets; i++) {//to avoid memory leaks
			free(hashTable[i]);
			hashTable[i] = NULL;
		}
	}
	else { 
		Node **oldHashTable = hashTable;
		hashTable = (Node **)Malloc(sizeof(Node) * numBuckets);
		for(size_t j = 0; j < numBuckets; j++) {//to avoid memory leaks
			free(hashTable[j]);
			hashTable[j] = NULL;
		}
		copyHashTable(hashTable, oldHashTable, temp);
	}
	return hashTable;
}

/* Function to copy over the contents of the hashTable to the new hashTable
 */
void copyHashTable(Node **newHashTable, Node **currentHashTable, size_t numBuckets) {
	for(size_t i = 0; i < numBuckets; i++) {//loops through all buckets
		Node *nodePointer = currentHashTable[i];
		while(nodePointer) {//loop through entire bucket to create contents in newHashTable
			create(newHashTable, nodePointer->key, nodePointer->value);
			Node *temp = nodePointer;
			temp = nodePointer;
			nodePointer = nodePointer->next;
			free(temp);
			temp = NULL;
		}
	}
	freeHashTable(currentHashTable);
}

/* Function that hashes the keys using a djb2 hashing algorithm
 */
uint32_t hash(uint64_t key) {
	uint32_t hashingValue = 5381;
	union {
		uint64_t num;
		uint8_t c[8];
	} u;
	u.num = key;
	for(size_t i = 0; i < sizeof(uint64_t); i++) {
		hashingValue = ((hashingValue << 5) + hashingValue) ^ u.c[i];
	}
	return hashingValue;
}

/* Function to add the nodes to the table or to update existing nodes
 */
int create(Node **hashTable, uint64_t key, int value) {
	uint32_t i = (uint32_t)hash(key) % (uint32_t)TotalBuckets;
	Node *newNodePointer = Malloc(sizeof(Node));
	newNodePointer->value = value;
	newNodePointer->key = key;
	newNodePointer->next = NULL;
	Node *currentNodePointer = hashTable[i];
	if(currentNodePointer == NULL) {//if there are no collisions
		hashTable[i] = newNodePointer;
	}
	else {//if there is a collision
		if(currentNodePointer->key == key) {//if the first node in the bucket matches the key, it is updated
			currentNodePointer->value = value;
			return 1;
		}
		while(currentNodePointer->next != NULL) {//itterates through the bucket
			if(currentNodePointer->key == key) {//if a node in the bucket has a matching key, it is updated
				currentNodePointer->value = value;
				return 1;
			}
			currentNodePointer = currentNodePointer->next;
		}
		currentNodePointer->next = newNodePointer;//if there are no updates required, the new node is added to the end of the bucket
	}
	newNodePointer = NULL;
	free(newNodePointer);
	return 0;
}
\
/* the functionality is EXACTLY the same as with create except the desired return values are swapped.
 * to make the code more readable, i just swapped the return values instead of pasting in additional unneccesary code
 */	
int update(Node **hashTable, uint64_t key, int value) {
	return create(hashTable, key, value) ? 0 : 1;//Pro level coding right here.
}
/*
 * Function for searching a key and, if found, setting it to the value pointer and returning 0
 * if not found, it returns 2 to signify error
 */
int read(Node **hashTable, uint64_t key, int *value) {
	uint32_t bucketIndex = (uint32_t)hash(key) % (uint32_t)TotalBuckets;
	if(hashTable[bucketIndex] != NULL) {//will only try to read if the bucket isn't null
		Node *nodePointer = hashTable[bucketIndex];
		while(nodePointer->next != NULL) {//itterates through the bucket
			if(nodePointer->key == key) {//if the key matches, will read the value
				*value = nodePointer->value;
				return 0;
		}
			nodePointer = nodePointer->next;
		}
	}
	return 2;
}
/* Function to delete Nodes from the hashTable
 */
int delete(Node **hashTable, uint64_t key) {
	uint32_t bucketIndex = (uint32_t)hash(key) % (uint32_t)TotalBuckets;
	if(hashTable[bucketIndex] != NULL) {//will only try to delete if the bucket isn't null
		Node *currentNodePointer = hashTable[bucketIndex];
		if(currentNodePointer->next == NULL) {//if the bucket only has one node
			if(currentNodePointer->key == key) {//checks if node matches, if so, it is deleted
				free(currentNodePointer);
				hashTable[bucketIndex] = NULL;
				return 0;			
			}
		}
		else {//if the bucket has more than one node
			Node *previousNodePointer;
			if(currentNodePointer->key == key) {//first checks if the first node matches, and if so, deletes it
				hashTable[bucketIndex] = currentNodePointer->next;
				currentNodePointer = NULL;
				free(currentNodePointer);
				return 0;
			}
			else {//if the first node doesn't match
				while(currentNodePointer->next != NULL) {//itterates through the remainder of the bucket
					previousNodePointer = currentNodePointer;
					currentNodePointer = currentNodePointer->next;
					if(currentNodePointer->key == key) {//if a node matches, it is deleted and the previous node's next is set to the next node
						previousNodePointer->next = currentNodePointer->next;
						currentNodePointer = NULL;
						free(currentNodePointer);
						return 0;
					}
				}
			}
		}
	}
	return 2;
}	

/* Function that parses the arguements from the input file and calls the CRUD functions
 */
Node **runHashCommands(Node **hashTable, FILE *commands) {
	char *buffer = NULL;
	size_t length = 0;
	ssize_t numchars = 0;
	char option;
	uint64_t key;
	int value;	
	while((numchars = getline(&buffer, &length, commands)) != -1) {//for every line of the file
		int numArgs = sscanf(buffer, "%c %lu %d", &option, &key, &value);//parses the file
		if (numArgs > 1 && numArgs < 4) {//ensures that it is a function and not a comment or a broken line
			if(option == 'c') {//create
				if(create(hashTable, key, value) == 1) {
					fprintf(stdout, "Warning, unable to create Node for key %lu and value %d.\n", key, value);
				}
			}
			else if(option == 'r') {//read
				if(read(hashTable, key, &value) == 0) {
					fprintf(stdout,"Key %lu has value %d.\n", key, value);
				}
				else {
					fprintf(stdout, "Warning, unable to read the value for key %lu.\n", key);
				}
			}
			else if(option == 'u') {//update
				if(update(hashTable, key, value) == 1) {
					fprintf(stdout, "Warning, unable to update the Node with key %lu.\n", key);
				}
			}
			else if(option == 'd') {//delete
				if(delete(hashTable, key) == 2) {
					fprintf(stdout, "warning, unable to delete the Node with key %lu.\n", key);
				}
			}
			else{//error
				fprintf(stderr, "Warning: unable to read the value for key %lu.\n", key);
			}
			if(option == 'c' || option == 'u') {//only checks for a resize if create or update
				getHashTableStats(hashTable);
				//line below checks if either of the thresholds are exceeded
				if((((float)BucketsUsed / (float)TotalBuckets) > ((float)PERCENTAGETHRESHOLD / 100)) || ((float)WorstChainLength >(float)CHAININGTHRESHOLD)) {
					float floatBuckets = TotalBuckets + ((float)TotalBuckets * (RESIZEPERCENTAGE / 100.0)) + 0.9999;
					size_t numBuckets = (size_t)floatBuckets;
					fprintf(stdout, "Stats before resizing: ");
					printHashTableStats();
					hashTable = createHashTable(hashTable, numBuckets);//rehashes the hashTab
					getHashTableStats(hashTable);
					fprintf(stdout, "Stats after resizing: ");
					printHashTableStats();
				}
			}
		}
	}
	getHashTableStats(hashTable);
	fprintf(stdout, "Stats at end: ");//prints the stats at the end
	printHashTableStats();
	if(buffer) free(buffer);//frees the buffer
	return hashTable;
}

/* Function to get the stats of the hashTable to calculate the modifications that occured
 */
void getHashTableStats(Node **hashTable) {
	size_t tempChainLength = 0;//used for chain length of each buffer
	size_t tempWorst = 0;//used for what is (at the time) the worst chain length
	BucketsUsed = 0;
	for(size_t i = 0; i < TotalBuckets; i++) {//runs through the buckets
		if(hashTable[i] != NULL) {//checks if bucket is null
			BucketsUsed++;
			tempChainLength = 1;
			Node *nodePointer = hashTable[i];
			while(nodePointer->next != NULL) {//itterates through the bucket
				nodePointer = nodePointer->next;
				tempChainLength++;
			}
		}
		if(tempChainLength > tempWorst) {
			tempWorst = tempChainLength;
		}
	}
	WorstChainLength = tempWorst;
}
/* prints out the stats of the hashTable
 */
void printHashTableStats() {
	float percentUsed = ((float)BucketsUsed / (float)TotalBuckets) * 100;
	fprintf(stdout, "%zu Buckets, %%%.0f of buckets used, worst chain length = %zu.\n", TotalBuckets, percentUsed, WorstChainLength);
}

/* Frees the memory allocated to a hashTable
 */
void freeHashTable(Node **hashTable) {
	for(size_t j = 0; j < TotalBuckets; j++) {//for each bucket
		if(hashTable[j] != NULL) {//frees it
			hashTable[j] = NULL;
		}
		free(hashTable[j]);	
	}		
}

/* Prints out the help message
 */
void help() {
	fprintf(stdout, "\nhashTab creates, retrieves, updates, and deletes key-value pairs in a hash table.\n");
	fprintf(stdout, "Keys can be any integer in the range [0, 18446744073709551615] and values can be\n");
	fprintf(stdout, "any integer in the range [-2147483648, 214783647]. Commands to perform the above\n");
	fprintf(stdout, "actions may be specified in the input text file as follows.\n");
	fprintf(stdout, "\n# format is: action key [value] [#comment]\n");
	fprintf(stdout, "# action is one of the following characters: c r u d\n");
	fprintf(stdout, "c key value # if key already in table, value will be updated and a warning printed\n");
	fprintf(stdout, "r key       # if key not in the table, a warning will be printed, otherwise value\n");
	fprintf(stdout, "u key value # if key not in the table, value will be stored and a warning printed\n");
	fprintf(stdout, "d key       # if key not in the table, a warning will be printed\n");
	fprintf(stdout, "\nThe table auto-resizes to maintain \"optimum\" performance. Run program as follows.\n");
	fprintf(stdout, "\nhashTab -i|--input <input file>\n");
	fprintf(stdout, "        or\n");
	fprintf(stdout, "hashTab [-h]|--help]\n\n");
	exit(0);
}

