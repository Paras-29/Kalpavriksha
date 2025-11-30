#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define HASH_MAP_SIZE 1000  
#define MAX_CAPACITY 1000
#define MIN_CAPACITY 1

const int MAX_COMMAND_NAME = 20;
const int MAX_VALUE = 100;

typedef struct Node {
    int key;
    char value[MAX_VALUE];
    struct Node *prev;
    struct Node *next;
} Node;

typedef struct HashMap {
    int key;
    Node* node;     
    struct HashMap* next;
} HashMap;

typedef struct LRUCache {
    int capacity;
    int size;

    Node *head;  
    Node *tail;  

    HashMap* map[HASH_MAP_SIZE];
} LRUCache;

LRUCache* cache = NULL;

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

void stringCopy(char *dest, const char *src) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

int hash(int key) {
    return key % HASH_MAP_SIZE;
}

bool isValidCapacity(int capacity){
    return capacity >= MIN_CAPACITY && capacity <= MAX_CAPACITY;   
}


void hmPut(int key, Node* node) {
    int index = hash(key);
    HashMap* newEntry = (HashMap*)malloc(sizeof(HashMap));
    newEntry->key = key;
    newEntry->node = node;
    newEntry->next = cache->map[index];
    cache->map[index] = newEntry;
}

Node* hmGet(int key) {
    int index = hash(key);
    HashMap* entry = cache->map[index];

    while (entry != NULL) {
        if (entry->key == key){
            return entry->node;
        }
        entry = entry->next;
    }
    return NULL;
}

void hmDelete(int key) {
    int index = hash(key);
    HashMap *entry = cache->map[index], *prev = NULL;

    while (entry != NULL) {
        if (entry->key == key) {
            if (prev) {
                prev->next = entry->next;
            }
            else {
                cache->map[index] = entry->next;
            }
            free(entry);
            return;
        }
        prev = entry;
        entry = entry->next;
    }
}

void addToFront(Node* node) {
    node->prev = NULL;
    node->next = cache->head;

    if (cache->head != NULL){
        cache->head->prev = node;
    }

    cache->head = node;

    if (cache->tail == NULL){
        cache->tail = node;
    }
}

void removeNode(Node* node) {
    if (node->prev ){
        node->prev->next = node->next;
    }
    else{
        cache->head = node->next;
    }

    if (node->next){
        node->next->prev = node->prev;
    }
    else{
        cache->tail = node->prev;
    }
}

void moveToFront(Node* node) {
    removeNode(node);
    addToFront(node);
}

void createCache(int capacity) {
    cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->size = 0;
    cache->head = cache->tail = NULL;

    for (int i = 0; i < HASH_MAP_SIZE; i++){
        cache->map[i] = NULL;
    }
}

char* get(int key) {
    Node* node = hmGet(key);
    if (!node){
        return NULL;
    }

    moveToFront(node);
    return node->value;
}

void put(int key, char value[]) {
    Node* node = hmGet(key);

    if (node) {
        stringCopy(node->value, value);
        moveToFront(node);
        return;
    }

    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->key = key;
    stringCopy(newNode->value, value);
    newNode->prev = newNode->next = NULL;

    addToFront(newNode);
    hmPut(key, newNode);
    cache->size++;

    if (cache->size > cache->capacity) {
        Node* lru = cache->tail;
        hmDelete(lru->key);
        removeNode(lru);
        free(lru);
        cache->size--;
    }
}

int main() {
    char command[MAX_COMMAND_NAME];
    int key;
    char value[MAX_VALUE];

    while (1) {
        scanf("%s", command);

        if (stringCompare(command, "createCache")) {
            int capacity;
            scanf("%d", &capacity);
            while(getchar() != '\n');

            if(!isValidCapacity(capacity)){
                printf("Enter the capacity between 1 and 1000: ");
                continue;
            }
            
            createCache(capacity);
        }
        else if (stringCompare(command, "put")) {
            scanf("%d %s", &key, value);
            put(key, value);
        }
        else if (stringCompare(command, "get")) {
            scanf("%d", &key);
            char* result = get(key);

            if (result){
                printf("%s\n",result);
            }
            else{
                printf("NULL\n");
            }
        }
        else if (stringCompare(command, "exit")) {
            break;
        }
    }

    return 0;
}
