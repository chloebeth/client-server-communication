#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h> // for unix
#include<fcntl.h>


int main() {
    int file;
    struct stat file_stat; 
    char * path = "/tmp/p1";
    char input[100];

    // check for the file, if it is not created, create it 
    // if stat returns 0, the file exists
    if (stat(path, &file_stat) != 0) {
       // the file doesnt exist, so create a file for writing
        file =  mkfifo(path, 0777);
        printf("FIFO created.\n");
    } else {
        // the file exists, so open the file for read/write
        file = open(path, O_RDONLY);
    }

    // open the FIFO
    file = open(path, O_RDONLY);

    // read input coming from the client and display it
    while(1) {
        // read the FIFO
        read(file, input, 100);
        printf("Client Sent: %s\n", input);
    }

    // close the FIFO
    close(file);

    return 0; 
}
