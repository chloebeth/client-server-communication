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
    char output[100];
    int flag = 0; // 0 means dont print, 1 means 

    int size = 4096;

    char send[8] = "06872342";
    char fill[8] = "00000000";

    // create or open the shared memory file for writing 
    file = open(path, O_RDWR, 0666);
    if (file < 0) return 1; // exit if it cant be opened 

    // the output file
    char* map = (char *)mmap(0, 4096, PROT_READ | PROT_WRITE, MAP_SHARED, file, 0);
    if (map == MAP_FAILED) {
        printf("Mapping failed.\n");
        return 1;
    }

    // reaad the input from client
    while(1) {
        flag = 0;
        // read from the shared memory 
        read(file, map, sizeof(file));

        for (int i=0; i<8; i++) { // check the first 8 chars in the map and see if they match the signal
            if (map[i] == send[i]) {
                flag = 1;
            } else if (map[i] != send[i]) {
                flag = 0;
                break;
            } 
        }

        if (flag == 1) {
            printf("Client Sent: %s", map+8);
            flag = 0;
        }

        // overwrite flag 
        for (int i=0; i<8; i++) {
            map[i] = fill[i];
        }
    }
 
    close(file);
    // unmap 
    munmap(map, size);

    return 0; 
}