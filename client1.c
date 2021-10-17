#include<stdio.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h> // for unix
#include<fcntl.h>


int main() { 
    int file;
    struct stat file_stat; 
    char input[100];

    // check for the file, if it is not created, create it 
    // if stat returns 0, the file exists
    if (stat("/tmp/p1", &file_stat) != 0) {
       // the file doesnt exist, so create a file for writing
        mkfifo("/tmp/p1", 0666);
        printf("FIFO created.\n");
    } else {
        // the file exists, so open the file for read/write
        file = open("/tmp/p1", O_WRONLY);
    }

    // get the user input and send the chars to the FIFO so the 
    // server may read the input
    while(1) {
        // get the user input (only accepts 100 chars)
        fgets(input, 100, stdin);

        // convert the input to all uppercase 
        for(int i=0;i<strlen(input);i++){
            input[i] = toupper(input[i]);
        }

        // write the user input to the FIFO
        write(file, input, strlen(input));
        printf("Client sent a message to the server.\n");
    }

    // close the FIFO
    close(file);

    return 0; 
}
