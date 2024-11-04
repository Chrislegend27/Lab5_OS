// C program for implementation of Simulation 
#include<stdio.h> 
#include<limits.h>
#include<stdlib.h>
#include "process.h"
#include "util.h"
#include <stdbool.h>  // Include this for bool, true, and false



// Function to find the waiting time for all  
// processes
void findWaitingTimeRR(ProcessType plist[], int n, int quantum)
{
    int rem_bt[n];  // Array to store remaining burst time for each process
    for (int i = 0; i < n; i++) {
        rem_bt[i] = plist[i].bt;  // Initialize remaining burst time with burst times
        plist[i].wt = 0;           // Initialize waiting times to 0
    }

    int t = 0;  // Current time

    // Continue until all processes are completed
    while (1) {
        bool done = true;

        // Traverse all processes one by one in a round-robin manner
        for (int i = 0; i < n; i++) {
            // If burst time of a process is greater than 0, then it needs to be processed further
            if (rem_bt[i] > 0) {
                done = false;  // There is at least one process remaining

                if (rem_bt[i] > quantum) {
                    // Increase time by quantum and decrease burst time by quantum
                    t += quantum;
                    rem_bt[i] -= quantum;
                } else {
                    // If remaining burst time is less than or equal to quantum, process finishes in this cycle
                    t += rem_bt[i];
                    plist[i].wt = t - plist[i].bt;  // Waiting time = current time - burst time
                    rem_bt[i] = 0;                  // Process is now completed
                }
            }
        }

        // If all processes are done
        if (done == true)
            break;
    }
}


// Function to find the waiting time for all  
// processes 
void findWaitingTimeSJF(ProcessType plist[], int n)
{
    int remainingTime[n];
    for (int i = 0; i < n; i++) {
        remainingTime[i] = plist[i].bt;
    }

    int completed = 0, currentTime = 0;
    int minIndex;

    while (completed != n) {
        minIndex = -1;
        int minRemainingTime = INT_MAX;

        // Find the process with the smallest remaining time that has arrived
        for (int i = 0; i < n; i++) {
            if (plist[i].art <= currentTime && remainingTime[i] > 0 && remainingTime[i] < minRemainingTime) {
                minRemainingTime = remainingTime[i];
                minIndex = i;
            }
        }

        // If no process is found, increment time and continue
        if (minIndex == -1) {
            currentTime++;
            continue;
        }

        // Execute the selected process for one time unit
        remainingTime[minIndex]--;
        currentTime++;

        // If a process is completed
        if (remainingTime[minIndex] == 0) {
            completed++;
            int completionTime = currentTime;
            plist[minIndex].wt = completionTime - plist[minIndex].art - plist[minIndex].bt;
            if (plist[minIndex].wt < 0) {
                plist[minIndex].wt = 0; // Waiting time cannot be negative
            }
        }
    }
}


// Function to find the waiting time for all  
// processes 
void findWaitingTime(ProcessType plist[], int n)
{ 
    // waiting time for first process is 0, or the arrival time if not 
    plist[0].wt = 0 +  plist[0].art; 
  
    // calculating waiting time 
    for (int  i = 1; i < n ; i++ ) 
        plist[i].wt =  plist[i-1].bt + plist[i-1].wt; 
} 
  
// Function to calculate turn around time 
void findTurnAroundTime( ProcessType plist[], int n)
{ 
    // calculating turnaround time by adding bt[i] + wt[i] 
    for (int  i = 0; i < n ; i++) 
        plist[i].tat = plist[i].bt + plist[i].wt; 
} 
  
// Function to sort the Process acc. to priority
// Function to sort the Process according to priority
int my_comparer(const void *this, const void *that)
{
    const ProcessType *p1 = (const ProcessType *)this;
    const ProcessType *p2 = (const ProcessType *)that;

    if (p1->pri < p2->pri) {
        return -1; // p1 has a higher priority
    } else if (p1->pri > p2->pri) {
        return 1; // p2 has a higher priority
    } else {
        return 0; // Both have the same priority
    }
}


//Function to calculate average time 
void findavgTimeFCFS( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTime(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nFCFS\n");
}

//Function to calculate average time 
void findavgTimeSJF( ProcessType plist[], int n) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeSJF(plist, n); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nSJF\n");
}

//Function to calculate average time 
void findavgTimeRR( ProcessType plist[], int n, int quantum) 
{ 
    //Function to find waiting time of all processes 
    findWaitingTimeRR(plist, n, quantum); 
  
    //Function to find turn around time for all processes 
    findTurnAroundTime(plist, n); 
  
    //Display processes along with all details 
    printf("\n*********\nRR Quantum = %d\n", quantum);
}

//Function to calculate average time 
void findavgTimePriority(ProcessType plist[], int n) 
{ 
    // Step 1: Sort the processes by priority
    qsort(plist, n, sizeof(ProcessType), my_comparer);

    // Step 2: Apply FCFS to calculate waiting time and turnaround time
    findWaitingTime(plist, n);       // Calculates waiting times based on sorted order
    findTurnAroundTime(plist, n);    // Calculates turnaround times based on waiting times

    // Displaying results
    printf("\n*********\nPriority\n");
}

void printMetrics(ProcessType plist[], int n)
{
    int total_wt = 0, total_tat = 0; 
    float awt, att;
    
    printf("\tProcesses\tBurst time\tWaiting time\tTurn around time\n"); 
  
    // Calculate total waiting time and total turn  
    // around time 
    for (int  i=0; i<n; i++) 
    { 
        total_wt = total_wt + plist[i].wt; 
        total_tat = total_tat + plist[i].tat; 
        printf("\t%d\t\t%d\t\t%d\t\t%d\n", plist[i].pid, plist[i].bt, plist[i].wt, plist[i].tat); 
    } 
  
    awt = ((float)total_wt / (float)n);
    att = ((float)total_tat / (float)n);
    
    printf("\nAverage waiting time = %.2f", awt); 
    printf("\nAverage turn around time = %.2f\n", att); 
} 

ProcessType * initProc(char *filename, int *n) 
{
  	FILE *input_file = fopen(filename, "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    ProcessType *plist = parse_file(input_file, n);
  
    fclose(input_file);
  
    return plist;
}
  
// Driver code 
int main(int argc, char *argv[]) 
{ 
    int n; 
    int quantum = 2;

    ProcessType *proc_list;
  
    if (argc < 2) {
		   fprintf(stderr, "Usage: ./schedsim <input-file-path>\n");
		   fflush(stdout);
		   return 1;
	   }
    
  // FCFS
    n = 0;
    proc_list = initProc(argv[1], &n);
  
    findavgTimeFCFS(proc_list, n);
    
    printMetrics(proc_list, n);
  
  // SJF
    n = 0;
    proc_list = initProc(argv[1], &n);
   
    findavgTimeSJF(proc_list, n); 
   
    printMetrics(proc_list, n);
  
  // Priority
    n = 0; 
    proc_list = initProc(argv[1], &n);
    
    findavgTimePriority(proc_list, n); 
    
    printMetrics(proc_list, n);
    
  // RR
    n = 0;
    proc_list = initProc(argv[1], &n);
    
    findavgTimeRR(proc_list, n, quantum); 
    
    printMetrics(proc_list, n);
    
    return 0; 
} 