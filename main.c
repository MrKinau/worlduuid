#include <stdio.h>
#include <stdlib.h>
#include <libgen.h>
#include <string.h>
#include <uuid/uuid.h>
#include <unistd.h>
#include <dirent.h>

#ifdef WIN32
#include <io.h>
#define F_OK 0
#define access _access
#endif

void printWorldUUID(char* pathToUid) {
    uuid_t uuid;

    FILE *fp = fopen(pathToUid, "r");
    fread(&uuid, sizeof(uuid_t), 1, fp);
    fclose(fp);

    char* uuidAsStr = malloc(38);
    uuid_unparse(uuid, uuidAsStr);

    char* worldName = malloc(strlen(pathToUid) + 1);
    strncpy(worldName, pathToUid, strlen(pathToUid) - 7);
    worldName[strlen(pathToUid) - 7] = '\0';
    worldName = basename(worldName);
    unsigned long worldNameLen = strlen(worldName);
    unsigned long spacerLen = (36 - worldNameLen - 2) / 2;
    printf("\n");
    for (int i = 0; i < spacerLen; i++)
        printf("=");
    printf(" %s ", worldName);
    if (worldNameLen % 2 != 0)
        printf(" ");
    for (int i = 0; i < spacerLen; i++)
        printf("=");
    printf("\n%s\n", uuidAsStr);
    for (int i = 0; i < 36; i++)
        printf("=");
    printf("\n");
}

int main(int argc, char **argv) {
    if (argc <= 1) {
        printf(".%s <FILE>\n", argv[0]);
        return 1;
    }

    char *path = argv[1];
    unsigned long pathLen = strlen(path);
    int counter = 0;

    DIR* d;
    struct dirent* dir;
    d = opendir(path);
    if (d) {
        char *newPath = malloc(pathLen + 9);
        basename(path);
        strncpy(newPath, path, pathLen);
        strcat(newPath, "/uid.dat");

        // Predict it's a server folder with multiple worlds
        if (access(newPath, F_OK) != 0) {
            while ((dir = readdir(d)) != NULL) {
                char* subDirPath = malloc(strlen(path) + strlen(dir->d_name) + 2);
                strcpy(subDirPath, path);
                strcat(subDirPath, "/");
                strcat(subDirPath, dir->d_name);

                DIR* subDir = opendir(subDirPath);
                struct dirent* subDirent;
                if (subDir) {
                    while ((subDirent = readdir(subDir)) != NULL) {
                        if (strcmp(subDirent->d_name, "uid.dat") == 0) {
                            char* subNewPath = malloc(strlen(subDirPath) + strlen(subDirent->d_name) + 2);
                            strcpy(subNewPath, subDirPath);
                            strcat(subNewPath, "/");
                            strcat(subNewPath, subDirent->d_name);
                            printWorldUUID(subNewPath);
                            counter++;
                            break;
                        }
                    }
                    closedir(subDir);
                }
            }
        } else {
            printWorldUUID(newPath);
            counter++;
        }
        closedir(d);

        if (counter <= 0)
            printf("No worlds could be found!\n");
        else
            printf("\n");
        return 0;
    }

    if (access(path, F_OK) != 0) {
        printf("This path does not exist!\n");
        return 1;
    } else {
        printWorldUUID(path);
        printf("\n");
    }

    return 0;
}