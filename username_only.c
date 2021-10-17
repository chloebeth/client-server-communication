#include<stdio.h>
#include<ctype.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<string.h>
#include<unistd.h> // for unix
#include<fcntl.h>


int main() { 
    char line[4096];


    // open the two processes, one to grab all info and one to cut
    // to get all users 
    FILE *file_ps_aux = popen("ps aux", "r");
    FILE *file_cut = popen("cut -d ' ' -f 1", "w");


    while (fgets(line, 4096, file_ps_aux)) {
        fprintf(file_cut, "%s", line);
    }

    pclose(file_ps_aux);
    pclose(file_cut);

    return 0; 
}
