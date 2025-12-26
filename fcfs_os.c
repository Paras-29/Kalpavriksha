#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_NAME_LENGTH  50
#define HASH_MAP_SIZE  100
#define MAX_LINE_LENGTH 100

void copyString(char *dest, const char *src) {
    int i = 0;

    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    
    dest[i] = '\0';
}

bool isKillCommand(const char *line) {
    return (line[0] == 'K' &&line[1] == 'I' &&line[2] == 'L' &&line[3] == 'L');
}

typedef enum { 
    READY, 
    RUNNING, 
    WAITING, 
    TERMINATED, 
    KILLED 
} PCBState;

typedef struct PCBNode {
    int pid;
    char name[MAX_NAME_LENGTH];
    int burstTime;
    int remainingTime;
    int ioStartTime;
    int ioDuration;
    int ioRemaining;
    int completionTime;
    PCBState state;
    struct PCBNode *next; 
} PCBNode;

typedef struct QueueNode {
    PCBNode *pcb;
    struct QueueNode *next;
} QueueNode;

typedef struct Queue {
    QueueNode *front;
    QueueNode *rear;
    int size;
} Queue;

typedef struct KillEvent {
    int pid;
    int time;
    struct KillEvent *next;
} KillEvent;


PCBNode *hashMap[HASH_MAP_SIZE];
KillEvent *killHead = NULL;


int hashFunction(int pid) { 
    return pid % HASH_MAP_SIZE;
}

void hashmapPut(PCBNode *pcb) {
    int idx = hashFunction(pcb->pid);
    pcb->next = hashMap[idx];
    hashMap[idx] = pcb;
}

PCBNode* hashmapGet(int pid) {
    int idx = hashFunction(pid);
    PCBNode *curr = hashMap[idx];

    while (curr) {
        if (curr->pid == pid) {
            return curr;
        }
        curr = curr->next;
    }

    return NULL;
}

void initQueue(Queue *q) { 
    q->front = q->rear = NULL; 
    q->size = 0; 
}

void enqueue(Queue *q, PCBNode *pcb) {
    QueueNode *node = malloc(sizeof(QueueNode));
    node->pcb = pcb; node->next = NULL;

    if (!q->rear) { 
        q->front = q->rear = node; 
    }
    else { 
        q->rear->next = node; q->rear = node; 
    }

    q->size++;
}

PCBNode* dequeue(Queue *q) {
    if (!q->front) {
        return NULL;
    }

    QueueNode *node = q->front;
    PCBNode *pcb = node->pcb;
    q->front = node->next;

    if (!q->front) {
        q->rear = NULL;
    }

    free(node);
    q->size--;
    return pcb;
}

int removeFromQueue(Queue *q, int pid) {
    QueueNode *curr = q->front, *prev = NULL;

    while (curr) {
        if (curr->pcb->pid == pid) {
            
            if (!prev) {
                q->front = curr->next;
            }
            else {
                prev->next = curr->next;
            }

            if (curr == q->rear) {
                q->rear = prev;
            }

            free(curr);
            q->size--;
            return 1;
        }

        prev = curr;
        curr = curr->next;
    }
    return 0;
}

PCBNode* createPCB(const char *name, int pid, int burst, int ioStart, int ioDur) {
    PCBNode *pcb = malloc(sizeof(PCBNode));
    pcb->pid = pid;

    copyString(pcb->name, name);
    pcb->burstTime = burst;
    pcb->remainingTime = burst;
    pcb->ioStartTime = ioStart;
    pcb->ioDuration = ioDur;
    pcb->ioRemaining = 0;
    pcb->completionTime = 0;
    pcb->state = READY;
    pcb->next = NULL;

    return pcb;
}

void addKillEvent(int pid, int time) {
    KillEvent *event = malloc(sizeof(KillEvent));
    event->pid = pid; event->time = time; 
    event->next = NULL;

    if (!killHead || killHead->time > time) {
        event->next = killHead; 
        killHead = event;
    } 
    else {
        KillEvent *curr = killHead;

        while (curr->next && curr->next->time <= time) {
            curr = curr->next;
        }

        event->next = curr->next; curr->next = event;
    }
}

void readInput(Queue *readyQueue) {
    char line[MAX_LINE_LENGTH];

    while (fgets(line, sizeof(line), stdin)) {

        if (line[0] == '\n') {
            break;
        }

        if (isKillCommand(line)) {
            int pid, time;
            sscanf(line, "KILL %d %d", &pid, &time);
            addKillEvent(pid, time);
        } 
        else {
            char name[MAX_NAME_LENGTH]; 
            int pid, burst, ioStart, ioDur;
            sscanf(line, "%s %d %d %d %d", name, &pid, &burst, &ioStart, &ioDur);
            PCBNode *pcb = createPCB(name, pid, burst, ioStart, ioDur);
            hashmapPut(pcb);
            enqueue(readyQueue, pcb);
        }
    }
}

void processKillEvents(int time, Queue *readyQueue, Queue *waitingQueue, Queue *terminatedQueue, PCBNode **running) {
    while (killHead && killHead->time == time) {
        PCBNode *pcb = hashmapGet(killHead->pid);

        if (pcb && pcb->state != TERMINATED && pcb->state != KILLED) {
            pcb->state = KILLED; 
            pcb->completionTime = time;

            removeFromQueue(readyQueue, pcb->pid);
            removeFromQueue(waitingQueue, pcb->pid);

            if (*running && (*running)->pid == pcb->pid) {
                *running = NULL;
            }

            enqueue(terminatedQueue, pcb);
        }

        KillEvent *tmp = killHead;
        killHead = killHead->next;
        free(tmp);
    }
}

void updateWaitingQueue(Queue *waitingQueue, Queue *readyQueue) {
    QueueNode *curr = waitingQueue->front, *prev = NULL;

    while (curr) {
        PCBNode *pcb = curr->pcb;
        pcb->ioRemaining--;

        if (pcb->ioRemaining <= 0) {
            pcb->state = READY;
            enqueue(readyQueue, pcb);
            QueueNode *toFree = curr;

            if (!prev) {
                waitingQueue->front = curr->next;
            }
            else {
                prev->next = curr->next;
            }

            if (curr == waitingQueue->rear) {
                waitingQueue->rear = prev;
            }

            curr = curr->next;
            free(toFree); 
            waitingQueue->size--;
            continue;
        }
        prev = curr;
        curr = curr->next;
    }
}

void schedule(Queue *readyQueue, Queue *waitingQueue, Queue *terminatedQueue) {
    int time = 0;
    PCBNode *running = NULL;

    while (readyQueue->size > 0 || waitingQueue->size > 0 || running) {
        processKillEvents(time, readyQueue, waitingQueue, terminatedQueue, &running);

        if (!running && readyQueue->size > 0) {
            running = dequeue(readyQueue);

            if (running->state == KILLED) { 
                enqueue(terminatedQueue, running); running = NULL; 
            }
            else {
                running->state = RUNNING;
            }  
        }

        if (running) {
            running->remainingTime--;

            if (running->ioStartTime >= 0 && (running->burstTime - running->remainingTime) == running->ioStartTime &&
                running->ioDuration > 0) {

                running->state = WAITING;
                running->ioRemaining = running->ioDuration;
                enqueue(waitingQueue, running); 
                running = NULL;
            }
            else if (running->remainingTime <= 0) {
                running->state = TERMINATED;
                running->completionTime = time + 1;
                enqueue(terminatedQueue, running); 
                running = NULL;
            }
        }
        updateWaitingQueue(waitingQueue, readyQueue);
        time++;
    }
}

void printResult(Queue *terminatedQueue) {
    int count = 0; 
    QueueNode *curr = terminatedQueue->front;

    while (curr) { 
        count++; curr = curr->next; 
    }

    PCBNode **arr = malloc(sizeof(PCBNode*) * count);
    curr = terminatedQueue->front; 
    int i = 0;

    while (curr) { 
        arr[i++] = curr->pcb; 
        curr = curr->next; 
    }

    for (int a = 0; a < count - 1; a++) {
        for (int b = 0; b < count - a - 1; b++) {

            if (arr[b]->pid > arr[b+1]->pid) { 
                PCBNode *tmp = arr[b]; 
                arr[b] = arr[b+1]; 
                arr[b+1] = tmp; 
            }
        }
    }

    printf("\nPID   Name       CPU   IO   Status          Turnaround   Waiting\n");

    for (int j = 0; j < count; j++) {
        PCBNode *p = arr[j];
        if (p->state == KILLED) {
            printf("%-5d %-10s %-5d %-5d KILLED at %-7d %-12s %-8s\n",p->pid, p->name, p->burstTime, p->ioDuration, p->completionTime, "-", "-");
        }
        else {
            int turnaround = p->completionTime;
            int waiting = turnaround - p->burstTime - p->ioDuration;
            printf("%-5d %-10s %-5d %-5d OK%-12s %-12d %-8d\n",p->pid, p->name, p->burstTime, p->ioDuration, "", turnaround, waiting);
        }
    }
    free(arr);
}

void runFcfsOsScheduling(){
    Queue readyQueue, waitingQueue, terminatedQueue;

    initQueue(&readyQueue); 
    initQueue(&waitingQueue); 
    initQueue(&terminatedQueue);

    for(int i = 0; i < HASH_MAP_SIZE; i++) {
        hashMap[i] = NULL;
    }

    readInput(&readyQueue);
    schedule(&readyQueue, &waitingQueue, &terminatedQueue);
    printResult(&terminatedQueue);
}

int main() {
    runFcfsOsScheduling();
    return 0;
}
