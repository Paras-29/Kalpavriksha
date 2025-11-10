#include<stdio.h>
#include <stdlib.h>
#include<stdbool.h>
#include<string.h>

#define BLOCK_SIZE 512
#define NUM_BLOCKS 5000
#define MAX_NAME 50
#define TOTAL_PERCENT 100.0
#define MAX_CMD_NAME 30

char virtualDisk[NUM_BLOCKS][BLOCK_SIZE];

typedef struct FreeBlock {
    int index;
    struct FreeBlock* next;
    struct FreeBlock* prev;
} FreeBlock;

FreeBlock* freeHead = NULL;
FreeBlock* freeTail = NULL;

typedef struct FileNode {
    char fileName[MAX_NAME];
    bool isDirectory;
    int size;
    int blocksUsed;
    int *blockPointers;
    struct FileNode* parent;
    struct FileNode* child;
    struct FileNode* next;
    struct FileNode* prev;
} FileNode;

FileNode* root = NULL;
FileNode* cwd = NULL;

int stringLength(const char *s) {
    int i = 0;
    int count=0;
    while (s[i] != '\0'){
        count++;
        i++;
    } 
    return count;
}

void stringCopy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

bool stringCompare(const char *a, const char *b) {
    int i = 0;
    while (a[i] != '\0' && b[i] != '\0') {
        if (a[i] != b[i]) {
            return false;
        }
        i++;
    }
    return (a[i] == '\0' && b[i] == '\0');
}

void stringConcate(char *dest, const char *src) {
    int i = 0;
    while (dest[i] != '\0'){
        i++;
    }

    int j = 0;
    while (src[j] != '\0') {
        dest[i + j] = src[j];
        j++;
    }
    dest[i + j] = '\0';
}

void stringCopyUptoLimit(char *dest, const char *src, int limit) {
    int i;
    for (i = 0; i < limit-1 && src[i] != '\0'; i++){
        dest[i] = src[i];
    }
    
    dest[i] = '\0';
}

void removeNewLine(char *content){
    int length = stringLength(content);
    if (length > 0 && content[length - 1] == '\n') {
        content[length - 1] = '\0';
    }
}

void copyBytes(char *dest, const char *src, int count) {
    for (int i = 0; i < count; i++) {
        dest[i] = src[i];
    }
}

void initFreeBlocks() {
    for (int i = 0; i < NUM_BLOCKS; i++) {
        FreeBlock* newFreeBlock = (FreeBlock*)malloc(sizeof(FreeBlock));
        newFreeBlock->index = i;
        newFreeBlock->next = NULL;

        if (freeHead == NULL) {  
            newFreeBlock->prev = NULL;
            freeHead = freeTail = newFreeBlock; 
            continue;                   
        }

        newFreeBlock->prev = freeTail;   
        freeTail->next = newFreeBlock;   
        freeTail = newFreeBlock;        
    }
}

FreeBlock* allocateFreeBlock() {
    if (freeHead == NULL){ 
        return NULL;
    }
    FreeBlock* block = freeHead;
    freeHead = freeHead->next;

    if(freeHead != NULL) {
        freeHead->prev = NULL;
    }
    else {
        freeTail = NULL;
    }
    return block;
}

void freeBlock(int index) {
    FreeBlock* block = (FreeBlock*)malloc(sizeof(FreeBlock));
    block->index = index;
    block->next = NULL;

    if (freeTail != NULL) {
        block->prev = freeTail;
        freeTail->next = block;
        freeTail = block;
    } 
    else {
        block->prev = NULL;
        freeHead = freeTail = block;
    }
}

void initializeVFS() {
    initFreeBlocks();
    
    root = (FileNode*)malloc(sizeof(FileNode));
    stringCopy(root->fileName, "/");
    root->isDirectory = true;
    root->parent = NULL;
    root->child = NULL;
    root->next = root;
    root->prev = root;
    cwd = root;
    printf("Compact VFS - ready. Type 'exit' to quit.\n");
}

FileNode* findNode(FileNode* dir, const char* name) {
    if (dir->child == NULL) {
        return NULL;
    }
    FileNode* temp = dir->child;
    do {
        if (stringCompare(temp->fileName, name)){
            return temp;
        }
        temp = temp->next;
    } while (temp != dir->child);
    return NULL;
}

void insertChild(FileNode* parent, FileNode* childNode) {
    if (parent->child == NULL) {
        parent->child = childNode;
        childNode->next = childNode;
        childNode->prev = childNode;
    } 
    else {
        FileNode* tail = parent->child->prev;
        tail->next = childNode;
        childNode->prev = tail;
        childNode->next = parent->child;
        parent->child->prev = childNode;
    }
}

void mkdirCommand(char* name) {
    if (findNode(cwd, name)) {
        printf("Name already exists in current directory. \n");
        return;
    }
    FileNode* newDirectory = (FileNode*)malloc(sizeof(FileNode));
    stringCopy(newDirectory->fileName, name);
    newDirectory->isDirectory = true;
    newDirectory->child = NULL;
    newDirectory->parent = cwd;
    insertChild(cwd, newDirectory);
    printf("Directory '%s' created successfully.\n", name);
}

void rmdirCommand(const char *name){

    if (stringCompare(cwd->fileName, name)) {
        printf("Cannot remove current directory.\n");
        return;
    }

    FileNode* dir = findNode(cwd, name);
    if (!dir) {
        printf("Directory not found.\n");
        return;
    }
    if (dir->child != NULL) {
        printf("Directory not empty.\n");
        return;
    }

    if (dir->next == dir) {
        cwd->child = NULL;
    } else {
        if (cwd->child == dir){
            cwd->child = dir->next;
        }
        dir->prev->next = dir->next;
        dir->next->prev = dir->prev;
    }

    free(dir);
    printf("Directory '%s' removed.\n", name);
}

void createCommand(const char *name){
    if (findNode(cwd, name)) {
        printf("Name already exists in current directory. \n");
        return;
    }
    FileNode* newFile = (FileNode*)malloc(sizeof(FileNode));
    stringCopy(newFile->fileName, name);

    newFile->isDirectory = false;
    newFile->size = 0;
    newFile->blocksUsed = 0;
    newFile->blockPointers = NULL; 
    newFile->parent = cwd;

    insertChild(cwd, newFile);
    printf("File '%s' created successfully.\n", name);
}

void writeCommand(const char* name, const char* content) {
    FileNode* file = findNode(cwd, name);

    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }

    int newDataLen = stringLength(content);
    int totalSize = file->size + newDataLen;
    int oldBlocks = file->blocksUsed;
    int newBlocksNeeded = (totalSize + BLOCK_SIZE - 1) / BLOCK_SIZE;

    int freeCount = 0;
    for (FreeBlock* temp = freeHead; temp != NULL; temp = temp->next) {
        freeCount++;
    }

    if (freeCount < (newBlocksNeeded - oldBlocks)) {
        printf("Not enough free blocks.\n");
        return;
    }

    file->blockPointers = (int*)realloc(file->blockPointers, newBlocksNeeded * sizeof(int));

    for (int i = oldBlocks; i < newBlocksNeeded; i++) {
        FreeBlock* block = allocateFreeBlock();
        if (!block) {
            printf("Disk full.\n");
            return;
        }
        file->blockPointers[i] = block->index;
        free(block);
    }

    int written = 0;
    int offsetInLastBlock = file->size % BLOCK_SIZE;

    if (oldBlocks > 0 && offsetInLastBlock > 0) {
        int space = BLOCK_SIZE - offsetInLastBlock;
        int toCopy = (newDataLen < space) ? newDataLen : space;
        copyBytes(virtualDisk[file->blockPointers[oldBlocks - 1]] + offsetInLastBlock, content, toCopy);
        written += toCopy;
    }

    for (int i = oldBlocks; i < newBlocksNeeded && written < newDataLen; i++) {
        int remaining = newDataLen - written;
        int toCopy = (remaining < BLOCK_SIZE) ? remaining : BLOCK_SIZE;
        copyBytes(virtualDisk[file->blockPointers[i]], content + written, toCopy);
        written += toCopy;
    }

    file->size = totalSize;
    file->blocksUsed = newBlocksNeeded;

    printf("Data written successfully (size=%d bytes).\n", newDataLen);
}

void readCommand(const char* name) {
    FileNode* file = findNode(cwd, name);
    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }

    int remaining = file->size;
    for (int i = 0; i < file->blocksUsed; i++) {
        int toPrint = remaining < BLOCK_SIZE ? remaining : BLOCK_SIZE;
        printf("%.*s", toPrint, virtualDisk[file->blockPointers[i]]);
        remaining -= toPrint;
    }
    printf("\n");
}

void deleteCommand(char* name) {
    FileNode* file = findNode(cwd, name);
    if (!file || file->isDirectory) {
        printf("File not found.\n");
        return;
    }

    for (int i = 0; i < file->blocksUsed; i++){
        freeBlock(file->blockPointers[i]);
    }
    free(file->blockPointers);

    if (file->next == file){
        cwd->child = NULL;
    }
    else {
        if (cwd->child == file) {
            cwd->child = file->next;
        }
        file->prev->next = file->next;
        file->next->prev = file->prev;
    }
    free(file);
    printf("File deleted successfully.\n");
}

void lsCommand() {
    if (cwd->child == NULL) {
        printf("(empty)\n");
        return;
    }
    FileNode* temp = cwd->child;
    do {
        if (temp->isDirectory ){
            printf("%s/\n", temp->fileName);
        }
        else{
            printf("%s\n", temp->fileName);
        }
        temp = temp->next;
    } while (temp != cwd->child);
}

void cdCommand(char* name) {
    if (stringCompare(name, "..")) {
        if (cwd->parent != NULL) {
            cwd = cwd->parent;
        }
        printf("Moved to %s\n", cwd->fileName);
        return;
    }

    FileNode* dir = findNode(cwd, name);
    if (!dir || !dir->isDirectory) {
        printf("Directory not found.\n");
        return;
    }
    cwd = dir;
    printf("Moved to /%s\n", cwd->fileName);
}

void pwdCommand() {
    char path[512] = "";
    FileNode* temp = cwd;

    while (temp != NULL) {
        char tempName[60];
        stringCopy(tempName, temp->fileName);
        if (stringCompare(tempName, "/")) {
            break;
        } 
        char newPath[512];
        stringCopy(newPath, "/");
        stringConcate(newPath, tempName);
        if (stringLength(path) > 0){
            stringConcate(newPath, path);  
        }
        stringCopy(path, newPath);
        temp = temp->parent;
    }
    printf("/%s\n", path);
}


void dfCommand() {
    int freeBlocks = 0;
    FreeBlock* temp = freeHead;
    while (temp != NULL) {
        freeBlocks++;
        temp = temp->next;
    }

    int usedBlock = NUM_BLOCKS - freeBlocks;
    float percent = ((float)usedBlock / NUM_BLOCKS) * TOTAL_PERCENT;

    printf("Total Blocks: %d\n", NUM_BLOCKS);
    printf("Used Blocks: %d\n", usedBlock);
    printf("Free Blocks: %d\n", freeBlocks);
    printf("Disk Usage: %.2f%%\n", percent);
}

void freeAllBlocks() {
    FreeBlock* temp = freeHead;
    while (temp != NULL) {
        FreeBlock* next = temp->next;
        free(temp);
        temp = next;
    }
}

void freeNodes(FileNode* dir) {
    if (dir == NULL) {
        return;
    }
    if (dir->child != NULL) {
        FileNode* temp = dir->child;
        do {
            FileNode* next = temp->next;
            freeNodes(temp);
            temp = next;
        } while (temp != dir->child);
    }
    if (!dir->isDirectory && dir->blockPointers){
        free(dir->blockPointers);
    }
    free(dir);
}

void exitCommand() {
    freeAllBlocks();
    freeNodes(root);
    printf("Memory released. Exiting program...\n");
}

void runVirtualFileSystem(){
    initializeVFS();

    char command[MAX_CMD_NAME], name[MAX_NAME], content[BLOCK_SIZE];

    while (1) {
        printf("%s > ", cwd->fileName);
        scanf("%s", command);

        if (stringCompare(command, "mkdir")) {
            scanf("%s", name);
            mkdirCommand(name);
        } 
        else if (stringCompare(command, "rmdir")) {
            scanf("%s", name);
            rmdirCommand(name);
        }
        else if (stringCompare(command, "create")) {
            scanf("%s", name);
            createCommand(name);
        }
        else if (stringCompare(command, "write")) {
            scanf("%s", name);
            getchar();
            fgets(content , sizeof(content),stdin);
            removeNewLine(content);
            writeCommand(name,content);
        }
        else if (stringCompare(command, "read")) {
            scanf("%s", name);
            readCommand(name);
        }
        else if (stringCompare(command, "delete")) {
            scanf("%s", name);
            deleteCommand(name);
        }
        else if (stringCompare(command, "ls")) {
            lsCommand();
        }
        else if (stringCompare(command, "cd")) {
            scanf("%s", name);
            cdCommand(name);
        }
        else if (stringCompare(command, "pwd")) {
            pwdCommand();
        }
        else if (stringCompare(command, "df")) {
            dfCommand();
        }
        else if (stringCompare(command, "exit")) {
            exitCommand();
            return;
        }
        else {
            printf("Unknown command.\n");
        }
    }
}

int main(){
    runVirtualFileSystem();
    return 0;
}