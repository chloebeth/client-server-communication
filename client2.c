#include<stdio.h>
#include<sys/ipc.h>
#include<sys/mman.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h> // for unix
#include<fcntl.h>


int main() { 
    int file;
    char * path = "/tmp/p2";
    char input[100];
    char send[8] = "06872342";

    int size = 4096;

    // get the user input and send the chars to the memory mapped file 
    // so the server may read the input
    while(1) {
        // create or open the shared memory file for writing 
        file = open(path, O_RDWR | O_CREAT, 0666);
        if (file < 0) return 1; // exit if it cant be opened 

        // get the user input (only accepts 100 chars)
        char input[size];
        fgets(input, size, stdin);

        ftruncate(file, size);

        // the output file
        char* map = (char*)mmap(NULL, size+1, PROT_READ | PROT_WRITE, MAP_SHARED | MAP_FILE, file, 0);

        if (map == MAP_FAILED) {
            printf("Mapping failed.\n");
            close(file);
            return 1;
        }

        // write the send signal so the server knows to print a new message 
        for (int i=0; i<8; i++) {
            map[i] = send[i];
        }

        // convert the input to all uppercase 
        for(int i=8;i<100;i++){ 
            map[i] = toupper(input[i-8]);
        }

        close(file);
        // unmap 
        munmap(map, size);
    }

    return 0; 
}