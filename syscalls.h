#ifndef SYSCALLS
#define SYSCALLS

void *Malloc(size_t);								/*in: bytes to allocate; return: ptr of type void to allocated memory*/
FILE *Fopen(const char *, const char *);			/*in: oath/file, mode; return: file ptr*/
void Fclose(FILE *);								/*in: file ptr*/
size_t Fread(void *, size_t, size_t, FILE *);		/*in: buffer ptr, buffer size, num buffers, input file ptr; return: bytes read*/
size_t Fwrite(const void *, size_t, size_t, FILE *);/*in: buffer ptr, buffer size, numb buffers, output file ptr, return: bytes written*/
int Pipe(int filedes[2]);							/*in: filedes[0] is for the read end and filedes[1] is for the write end, return: 0 on success*/
pid_t Fork();										/*out: negative if creation unsuccessful, 0 if returned to newly created process, positive is id of new child process*/

#endif
