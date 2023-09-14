#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct list_node_s {
    int data;
    struct list_node_s* next;
};

struct list_node_s* head_p;
int n;
int16_t m;

int num_member, num_insert, num_delete;

void RunPrograme(int case_num, int sample_size);
int16_t GetRandomNumber();
void PopulateList(struct list_node_s* head_p, int n);
int Member(int value, struct list_node_s* head_p);
int Insert(int value, struct list_node_s** head_pp);
int Delete(int value, struct list_node_s** head_pp);
void DeleteList(struct list_node_s** head_pp);

int main(int argc, char* argv[]){
    srand(time(NULL));

    n = 1000;
    m = 10000;

    int sample_size = strtod(argv[1], NULL);

    RunPrograme(1, sample_size);
    RunPrograme(1, sample_size);
    RunPrograme(1, sample_size);
    RunPrograme(1, sample_size);
    RunPrograme(2, sample_size);
    RunPrograme(2, sample_size);
    RunPrograme(2, sample_size);
    RunPrograme(2, sample_size);
    RunPrograme(3, sample_size);
    RunPrograme(3, sample_size);
    RunPrograme(3, sample_size);
    RunPrograme(3, sample_size);

    return 0;
}

void RunPrograme(int case_num, int sample_size){
    clock_t start_time, end_time;
    char filename[50];

    float m_member[] = {0.99, 0.90, 0.50};
    float m_insert[] = {0.005, 0.05, 0.25};
    float m_delete[] = {0.005, 0.05, 0.25};

    num_member = m*m_member[case_num-1];
    num_insert = m*m_insert[case_num-1];
    num_delete = m*m_delete[case_num-1];

    sprintf(filename, "./output/serial_case%d_threads1.csv", case_num);
    FILE *fp = fopen(filename,"w");
    fprintf(fp,"n, time(ms)\n");

    for (int j=0; j<sample_size; j++){
        head_p = malloc(sizeof(struct list_node_s));
        PopulateList(head_p, n);

        start_time = clock();// Record the start time

        for (int i=0; i<num_member; i++){
            Member(GetRandomNumber(), head_p);
        }

        for (int i=0; i<num_insert; i++){
            Insert(GetRandomNumber(), &head_p);
        }

        for (int i=0; i<num_delete; i++){
            Delete(GetRandomNumber(), &head_p);
        }

        end_time = clock(); // Record the end time
        double cpu_time_used = ((double) (end_time - start_time)) / (CLOCKS_PER_SEC/1000); // Calculate time used in seconds

        fprintf(fp,"%d, %f\n", j, cpu_time_used);
        DeleteList(&head_p);
    }

    fclose(fp);
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
        if (Member(value, head_p)){
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

int Member(int value, struct list_node_s* head_p) {
    struct list_node_s* curr_p = head_p;

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

void DeleteList(struct list_node_s** head_pp){
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* next_p = NULL;

    while (curr_p != NULL){
        next_p = curr_p->next;
        free(curr_p);
        curr_p = next_p;
    }
}