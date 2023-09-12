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

void DoOperations();
int16_t GetRandomNumber();
void PopulateList(struct list_node_s* head_p, int n);
int Member(int value, struct list_node_s* head_p);
int Insert(int value, struct list_node_s** head_pp);
int Delete(int value, struct list_node_s** head_pp);

int main(int argc, char* argv[]){
    srand(time(NULL));

    n = 1000;
    m = 10000;

    float m_member[] = {0.99, 0.90, 0.50};
    float m_insert[] = {0.005, 0.05, 0.25};
    float m_delete[] = {0.005, 0.05, 0.25};

    int sample_size = strtod(argv[1], NULL);

    struct list_node_s* head_p = malloc(sizeof(struct list_node_s));
    PopulateList(head_p, n);
    // PrintList(head_p);

    clock_t start_time, end_time;
    double cpu_time_used;
    FILE *fp;
    char filename[50];

    for (int i=0; i<3; i++){
        num_member = m*m_member[i];
        num_insert = m*m_insert[i];
        num_delete = m*m_delete[i];
        sprintf(filename, "./output/serial_case%d.csv", i+1);
        fp = fopen(filename,"w");
        fprintf(fp,"n, time(ms)\n");
        for (int j=0; j<sample_size; j++){
            start_time = clock();// Record the start time
            DoOperations();
            end_time = clock(); // Record the end time
            cpu_time_used = ((double) (end_time - start_time)) / (CLOCKS_PER_SEC/1000); // Calculate time used in seconds
            fprintf(fp,"%d, %f\n", j, cpu_time_used);
        }
        fclose(fp);
    }


    return 0;
}

void DoOperations(){
    for (int i=0; i<num_member; i++){
        Member(GetRandomNumber(), head_p);
    }

    for (int i=0; i<num_insert; i++){
        Insert(GetRandomNumber(), &head_p);
    }

    for (int i=0; i<num_delete; i++){
        Delete(GetRandomNumber(), &head_p);
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