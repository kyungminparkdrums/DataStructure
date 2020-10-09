#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define MAX_QUEUE_SIZE 20

typedef struct {
    int id;
    int arrival_time;
    int service_time;
} element;

typedef struct {
    element data[MAX_QUEUE_SIZE];
    int front, rear;
} QueueType;

void error(char*);
void init_queue(QueueType*);
int is_empty(QueueType*);
int is_full(QueueType*);
void enqueue(QueueType*, element);
element dequeue(QueueType*);

int main() {

    int minutes = 60;
    int total_wait = 0;
    int total_customer = 0;
    int service_time[2] = {0};
    int service_customer[2];
    QueueType queue;

    init_queue(&queue);

    srand(time(NULL));
    
    for (int clock = 0; clock < minutes; clock++) {
        printf("\ncurrent time =  %d", clock);
    
        // customer in
        printf("\n->NEW CUSTOMER) ");
        if ((rand()%10) < 3) {
            element customer;

            customer.id = total_customer++;
            customer.arrival_time = clock;
            customer.service_time = rand()%3 + 1;
            enqueue(&queue, customer);

            printf("customer: %d, time: %d, service time: %d, ", customer.id, customer.arrival_time, customer.service_time);
        }
        else
            printf("none, ");

        for (int i=0; i<2; i++) {
            if (i==0) printf("\n");

            // if in service
            if (service_time[i] > 0)  {
                printf("->Desk %d IN SERVICE) serving customer:  %d\n", i, service_customer[i]);
                service_time[i]--;
            } 
            // otherwise
            else {
                if (!is_empty(&queue)) {
                    element customer = dequeue(&queue);
                    
                    service_customer[i] = customer.id;
                    service_time[i] = --customer.service_time;
                    //service_time[i] = customer.service_time;
                    printf("->Desk %d NEW SERVICE) customer %d, start time: %d, wait time: %d\n", i, customer.id, clock, clock - customer.arrival_time);
                    total_wait += clock - customer.arrival_time;
                }
                else
                    if (i==0) printf("->Empty queue\n");
            }
        }
    }

    printf("\nTotal wait time = %d", total_wait);
    printf("\nTotal customer = %d", total_customer);
    printf("\nAverage wait time = %f\n",(float)(total_wait)/total_customer);

    return 0;
}

void error(char* msg) {
    printf("%s\n", msg);
    exit(1);
}

void init_queue(QueueType* q) {
    q->front = q->rear = 0;
}

int is_empty(QueueType* q) {
    return (q->front == q->rear);
}

int is_full(QueueType* q) {
    return ((q->rear +1) % MAX_QUEUE_SIZE == q->front);
}

void enqueue(QueueType* q, element item) {
    if (is_full(q)) {
        error("queue full\n");
    }

    q->rear = (q->rear+1) % MAX_QUEUE_SIZE;
    q->data[q->rear] = item;
}

element dequeue(QueueType* q) {
    if (is_empty(q)) {
        error("queue empty\n");
    }
    
    q->front = (q->front +1) % MAX_QUEUE_SIZE;
    return q->data[q->front];
}


