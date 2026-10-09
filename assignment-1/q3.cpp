#include <iostream>
#include <string>
using namespace std;

struct Job
{
    int jobId;
    int remainingTime;
    int burstTime;      // original burst time, useful for reporting
    int completionTime; // filled in when job finishes
    Job *next;

    Job(int id, int burst)
    {
        jobId = id;
        remainingTime = burst;
        burstTime = burst;
        completionTime = -1;
        next = nullptr;
    }
};

class GPUScheduler
{
private:
    Job *current; // points to the "current" job in the circle
    int jobCount;
    int clock; // simulated time elapsed so far
    
    //added these to keep track of completion time for each job
    int *completedIds;
    int *completedTimes;
    int completedCount;
    int totalJobs;

public:
    GPUScheduler()
    {
        current = nullptr;
        jobCount = 0;
        clock = 0;
        
        completedIds = nullptr;
        completedTimes = nullptr;
        completedCount = 0;
        totalJobs = 0;
    }

    ~GPUScheduler()
    {
        // TODO: careful cleanup of a CIRCULAR list.
        if (current == nullptr)
            return;
            
        int i = 0;
        
        Job *temp = current;
        
        while  (i < jobCount) {
            Job *temp1 = temp->next; 
            delete temp;
            temp = temp1;
            i++;
        }
        
        delete[] completedIds;
        delete[] completedTimes;

    }
    //helper function to set the total number of jobs
    void setTotalJobs(int n)
    {
        totalJobs = n;
        completedIds = new int[n];
        completedTimes = new int[n];
    }

    bool isEmpty()
    {
        return jobCount == 0;
    }

    void addJob(int jobId, int burstTime)
    {
        // TODO: create a new Job node and insert it into the circular list
        Job *newJob = new Job(jobId, burstTime);
        //checks if the list is empty
        //if empty, inserts the job and makes it point towards itself
        if(isEmpty()) {
            newJob->next = newJob;
            current = newJob;
        }
        else{
            Job *temp = current;
            while (temp->next != current) {
                temp = temp->next;
            }
            newJob->next = current;
            temp->next = newJob;
        }
        jobCount++;
    }

    void removeJob(int jobId)
    {
        // TODO: find the node with this jobId anywhere in the circle and
        // remove it, relinking neighbors so the circle stays intact.
        if (isEmpty()) {
            cout << "NO JOB FOUND!!" << endl;
            return;
        }
        //if first node
        else if (current->jobId == jobId) {
            //if only one node
            if ( current->next == current) {
                Job *toDelete = current;
                current = nullptr;
                delete toDelete;
                jobCount--;
            }
            else {
                Job *toDelete = current;
                Job *temp = current;
                //the temp variable traverses and stops before current
                while (temp->next != current) {
                    temp = temp->next;
                }
                //current is moved to the next place as well as the temp
                current = current->next;
                temp->next = current;
                //deleting the todelete which points to ex current 
                delete toDelete;
                //reducing job count
                jobCount--;
            }
            
        }
        else {
            Job *temp = current;
            while (temp->next != current) {
                //stopping before the job to be deleted/removed
                if (temp->next->jobId == jobId) {
                    //initializing a pointer to the job to be deleted, 
                    Job *toDelete = temp->next;
                    //moving the temp next to the job 
                    //thats next to the one going to be deleted
                    temp->next = temp->next->next;
                    //deleting the job to be deelted
                    delete toDelete;
                    //reducing job count
                    jobCount--;
                    //RETREATING, KAAM HO GYA
                    return;
                }
                temp = temp->next;
            }
            
        }
    }

    // Runs one time slice for the current job. Prints what happened.
    // Returns false if the scheduler is empty (nothing to run).
    bool runNextSlice(int quantum)
    {
        // TODO: Give the current job one turn on the GPU, then advance to
        // the next job in the circle. See the Context & Concept /
        // Problem Statement notes for Q3 in the assignment doc for the
        // nuances to consider (partial slices, job completion, removing
        // the last remaining job, etc).
        // Return true if a job was run this call, false if the scheduler
        // was empty.
        
        //if empty, exit 
        if (isEmpty()) {
            return false;
        }
        
        int time_used = 0;
        
        //specific for every job's iteration
        if (current->remainingTime < quantum) 
            time_used = current->remainingTime;
        else
            time_used = quantum;
            
        current->remainingTime -= time_used;
        //clock evaluates completion time for every job plus
        //at the end, its the total simulation time.
        clock += time_used;
        
        if (current->remainingTime == 0) {
            //print which job just finished
            cout << "Job " << current->jobId << " finished" << endl;
            //completion time, current clock time kay equal hai
            current->completionTime = clock;
            
            //jis sequence mein jobs complete hon gi
            //us sequence mein unki ids store hon gi
            completedIds[completedCount] = current->jobId;
            
            //this array stores completion time, har job ka
            completedTimes[completedCount] = current->completionTime;
            completedCount++;
            
            if (current->next == current) {
                // only job just finished
                delete current;
                current = nullptr;
            }
            else {
                Job *toDelete = current;
                Job *temp = current;
                //the temp variable traverses and stops before current
                while (temp->next != current) {
                    temp = temp->next;
                }
                //current is moved to the next place as well as the temp
                current = current->next;
                temp->next = current;
                //deleting the todelete which points to ex current 
                delete toDelete;
                
            }
            //reducing job count
            jobCount--;
            
        }
        else {
            cout << "Running job " << current->jobId << ", remaining: " << current->remainingTime << endl;
            current = current->next;
        }
        return true;
    }
    

    void simulateAll(int quantum)
    {
        while (!isEmpty())
        {
            runNextSlice(quantum);
        }
        cout << "Total simulation time: " << clock << endl;
        for (int i = 0; i < completedCount; i++) {
            cout << "Job " << completedIds[i] << " completed at time " 
            << completedTimes[i] << endl;
        }

    }
};

int main()
{
    int n;
    cout << "Enter number of jobs to be created: " << endl;
    cin >> n;
    cout << endl;
    

    GPUScheduler scheduler;
    scheduler.setTotalJobs(n);
    
    for (int i = 0; i < n; i++)
    {
        int id, burst;
        cout << "Enter Job's id and burst: " << endl;
        cin >> id >> burst;
        scheduler.addJob(id, burst);
    }

    int quantum;
    cout << "Enter quantum" << endl;
    cin >> quantum;

    scheduler.simulateAll(quantum);

    return 0;
}
