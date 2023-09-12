#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

int thread_count;
int n;
int16_t m;
float m_member, m_insert, m_delete;
int num_member, num_insert, num_delete;
int num_member_each, num_insert_each, num_delete_each;
struct list_node_s* head_p ;

struct list_node_s {
    int data;
    struct list_node_s* next;
};

void RunPrograme(int num_threads);
void* DoOperations(void* rank);
int16_t GetRandomNumber();
void PopulateList(struct list_node_s* head_p, int n);
int Member(int value, struct list_node_s** head_p);
int Insert(int value, struct list_node_s** head_pp);

pthread_mutex_t list_mutex;

long thread;
pthread_t* thread_handles;

int main(int argc, char* argv[]){
    pthread_mutex_init(&list_mutex, NULL);

    srand(time(NULL));

    int sample_size = strtod(argv[1], NULL);

    n = 1000;
    m = 10000;

    float m_member[] = {0.99, 0.90, 0.50};
    float m_insert[] = {0.005, 0.05, 0.25};
    float m_delete[] = {0.005, 0.05, 0.25};
    int thread_count[] = {1, 2, 4, 8};

    PopulateList(head_p, n);
    // PrintList(head_p);

    clock_t start_time, end_time;
    double cpu_time_used;
    FILE *fp;
    char filename[50];

    for (int t=0; t<4; t++){
        for (int i=0; i<3; i++){
            num_member = m*m_member[i];
            num_insert = m*m_insert[i];
            num_delete = m*m_delete[i];
            num_member_each = num_member/thread_count[t];
            num_insert_each = num_insert/thread_count[t];
            num_delete_each = num_delete/thread_count[t];
            sprintf(filename, "./output/parallel_one_mutex_case%d_threads%d.csv", i+1, thread_count[t]);
            fp = fopen(filename,"w");
            fprintf(fp,"n, time(ms)\n");
            for (int j=0; j<sample_size; j++){
                head_p = malloc(sizeof(struct list_node_s));
                thread_handles = malloc(thread_count[t]*sizeof(pthread_t));
                start_time = clock();// Record the start time
                RunPrograme(thread_count[t]);
                end_time = clock(); // Record the end time
                cpu_time_used = ((double) (end_time - start_time)) / (CLOCKS_PER_SEC/1000); // Calculate time used in seconds
                fprintf(fp,"%d, %f\n", j, cpu_time_used);
                free(head_p);
                free(thread_handles);
            }
            fclose(fp);
        }
    }

    pthread_mutex_destroy(&list_mutex);

    return 0;
}

void RunPrograme(int num_threads){

    for (thread=0; thread<num_threads; thread++){
        pthread_create(&thread_handles[thread], NULL, DoOperations, (void*) thread);
    }

    for (thread=0; thread<num_threads; thread++){
        pthread_join(thread_handles[thread], NULL);
    }
}

int16_t GetRandomNumber() {
    int16_t max_value = __INT16_MAX__;
    int16_t value = rand() % (max_value + 1);
    return value;
}

void PopulateList(struct list_node_s* head_p, int n) {
    int i = 0;
    
    while(i < n-1){
        int16_t value = GetRandomNumber();
        if (Member(value, &head_p)){
            continue;
        }
        Insert(value, &head_p);
        i++;
    }
}

void PrintList(struct list_node_s* head_p) {
    struct list_node_s* curr_p = head_p;
    int i=0;
    while (curr_p != NULL){
        printf("Node %d: %d\n", i, curr_p->data);
        i++;
        curr_p = curr_p->next;
    }
}

int Member(int value, struct list_node_s** head_p) {
    struct list_node_s* curr_p = *head_p;

    while (curr_p != NULL && curr_p->data < value){
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value){
        return 0;
    } else {
        return 1;
    }
}

int Insert(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;
    struct list_node_s* temp_p;

    while (curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p == NULL || curr_p->data > value){
        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if (pred_p == NULL){
            *head_pp = temp_p;
        } else {
            pred_p->next = temp_p;
        }
        return 1;
    } else {
        return 0;
    }
}

int Delete(int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p = NULL;

    while (curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if (curr_p != NULL && curr_p->data == value){
        if (pred_p == NULL){
            *head_pp = curr_p->next;
        } else {
            pred_p->next = curr_p->next;
        }
        free(curr_p);
        return 1;
    } else {
        return 0;
    }
}

void* DoOperations(void* rank){
    // Create arrays to store the functions and their respective counts
    int (*functions[])() = {Member, Insert, Delete};
    int counts[] = {0, 0, 0};
    int num_each[] = {num_member_each, num_insert_each, num_delete_each};
    
    int totalCalls = num_member_each + num_insert_each + num_delete_each; // Set desired total number of calls
    int remainingCalls = totalCalls;

    while (remainingCalls > 0) {
        // Generate a random index to select a function
        int randomIndex = rand() % 3; // 3 is the number of functions

        // Check if the selected function has been called enough times
        if (counts[randomIndex] < num_each[randomIndex]) {
            pthread_mutex_lock(&list_mutex);
            functions[randomIndex](GetRandomNumber(), &head_p); // Call the function
            pthread_mutex_unlock(&list_mutex);
            counts[randomIndex]++;   // Increment the count
            remainingCalls--;
        }
    }

    return NULL;
}
