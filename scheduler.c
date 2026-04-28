#include <stdio.h>
#include <stdlib.h>

#define MAX 50

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int completion;
    int waiting;
    int turnaround;
    int response;
    int started;
} Process;

void input(Process p[], int n) {
    for(int i=0;i<n;i++){
        p[i].pid = i+1;
        printf("Process %d Arrival Time: ", i+1);
        scanf("%d", &p[i].arrival);
        printf("Process %d Burst Time: ", i+1);
        scanf("%d", &p[i].burst);
        p[i].remaining = p[i].burst;
        p[i].started = 0;
    }
}

void print_metrics(Process p[], int n) {
    float avg_wt=0, avg_tat=0;

    printf("\nPID\tAT\tBT\tCT\tTAT\tWT\tRT\n");
    for(int i=0;i<n;i++){
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;

        avg_wt += p[i].waiting;
        avg_tat += p[i].turnaround;

        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
            p[i].pid, p[i].arrival, p[i].burst,
            p[i].completion, p[i].turnaround,
            p[i].waiting, p[i].response);
    }

    printf("\nAverage Waiting Time: %.2f", avg_wt/n);
    printf("\nAverage Turnaround Time: %.2f\n", avg_tat/n);
}

void gantt_chart(int timeline[], int size) {
    printf("\nGantt Chart:\n|");
    for(int i=0;i<size;i++){
        printf(" P%d |", timeline[i]);
    }
    printf("\n0");
    for(int i=1;i<=size;i++){
        printf("   %d", i);
    }
    printf("\n");
}

void fcfs(Process p[], int n) {
    int time = 0;
    int timeline[500], k=0;

    for(int i=0;i<n;i++){
        if(time < p[i].arrival)
            time = p[i].arrival;

        p[i].response = time - p[i].arrival;

        for(int j=0;j<p[i].burst;j++){
            timeline[k++] = p[i].pid;
        }

        time += p[i].burst;
        p[i].completion = time;
    }

    gantt_chart(timeline, k);
    print_metrics(p, n);
}

void sjf(Process p[], int n) {
    int completed = 0, time = 0, min_idx;
    int timeline[500], k=0;

    while(completed < n){
        int min = 1e9;
        min_idx = -1;

        for(int i=0;i<n;i++){
            if(p[i].arrival <= time && p[i].remaining > 0 && p[i].remaining < min){
                min = p[i].remaining;
                min_idx = i;
            }
        }

        if(min_idx == -1){
            time++;
            continue;
        }

        if(!p[min_idx].started){
            p[min_idx].response = time - p[min_idx].arrival;
            p[min_idx].started = 1;
        }

        p[min_idx].remaining--;
        timeline[k++] = p[min_idx].pid;
        time++;

        if(p[min_idx].remaining == 0){
            p[min_idx].completion = time;
            completed++;
        }
    }

    gantt_chart(timeline, k);
    print_metrics(p, n);
}

void round_robin(Process p[], int n, int quantum) {
    int time = 0, completed = 0;
    int timeline[500], k=0;

    while(completed < n){
        int executed = 0;

        for(int i=0;i<n;i++){
            if(p[i].arrival <= time && p[i].remaining > 0){

                if(!p[i].started){
                    p[i].response = time - p[i].arrival;
                    p[i].started = 1;
                }

                int exec_time = (p[i].remaining > quantum) ? quantum : p[i].remaining;

                for(int j=0;j<exec_time;j++){
                    timeline[k++] = p[i].pid;
                }

                time += exec_time;
                p[i].remaining -= exec_time;
                executed = 1;

                if(p[i].remaining == 0){
                    p[i].completion = time;
                    completed++;
                }
            }
        }

        if(!executed) time++;
    }

    gantt_chart(timeline, k);
    print_metrics(p, n);
}

int main() {
    int n, choice, quantum;

    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process p[MAX];
    input(p, n);

    printf("\n1. FCFS\n2. SJF (Preemptive)\n3. Round Robin\nChoose: ");
    scanf("%d", &choice);

    if(choice == 3){
        printf("Enter Time Quantum: ");
        scanf("%d", &quantum);
    }

    switch(choice){
        case 1: fcfs(p, n); break;
        case 2: sjf(p, n); break;
        case 3: round_robin(p, n, quantum); break;
        default: printf("Invalid choice\n");
    }

    return 0;
}
