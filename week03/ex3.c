#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_FILE_DATA_SIZE 1024
#define MAX_PATH_LENGTH 2048
#define MAX_FILENAME_LENGTH 63

struct File{
    unsigned int id;
    char name[MAX_FILENAME_LENGTH + 1];
    int size;
    char data[MAX_FILE_DATA_SIZE];
    struct Directory *directory;
};

struct Directory{
    char name[MAX_FILENAME_LENGTH + 1];
    struct File *files;
    struct Directory *directories;
    unsigned char nf;
    unsigned char nd;
    char path[MAX_PATH_LENGTH];

    
};

void overwrite_to_file(struct File* file, const char* str){
    strncpy(file->data, str, MAX_FILE_DATA_SIZE);
    file->size = strlen(file->data);
}

void append_to_file(struct File* file, const char* str){
    strcat(file->data, str);
    file->size = strlen(file->data);
}

void printp_file(struct File* file){
    printf("%s/%s\n", file->directory->path,  file->name);
}


void add_file(struct File *file, struct Directory *dir){
    dir->files = realloc(dir->files, (dir->nf + 1) * sizeof(struct File));
    dir->files[dir->nf] = *file;
    dir->nf++;
    file->directory = dir;
    //files
}

int main(){
    struct Directory root;
    strcpy(root.name, "/");
    root.nf = 0;
    root.nd = 0;
    strcpy(root.path, "/");
    root.files = NULL;
    root.directories = NULL;


    struct Directory home;
    strcpy(home.name, "home");
    home.nf = 0;
    home.nd = 0;
    sprintf(home.path, "/%s", home.name);
    home.files = NULL;
    home.directories = NULL;

    struct Directory bin;
    strcpy(bin.name, "bin");
    bin.nf = 0;
    bin.nd = 0;
    sprintf(bin.path, "/%s", bin.name);
    bin.files = NULL;
    bin.directories = NULL;

    struct File bash;
    bash.id = 1;
    strcpy(bash.name, "bash");
    bash.size = 0;
    strcpy(bash.data, "");
    bash.directory = &bin;
    
    add_file(&bash, &bin);

    struct File ex3_1, ex3_2;
    ex3_1.id = 2;
    strcpy(ex3_1.name, "ex3_1.c");
    strcpy(ex3_1.data, "int printf(const char * format, ...);");
    ex3_1.size = strlen(ex3_1.data);
    ex3_1.directory = &home;
    add_file(&ex3_1, &home);

    ex3_2.id = 2;
    strcpy(ex3_2.name, "ex3_2.c");
    strcpy(ex3_2.data, "//This is a comment in C language");
    ex3_2.size = strlen(ex3_2.data);
    ex3_2.directory = &home;
    add_file(&ex3_2, &home);

    append_to_file(&bash, "Bourne Again Shell!!");

    append_to_file(&ex3_1, "int main(){printf(\"Hello World\")}");

    printp_file(&bash);
    printp_file(&ex3_1);
    printp_file(&ex3_2);

    free(home.files);


    return 0;
}