#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <string.h>

#include <pthread.h>
#include <sys/wait.h>
#include <sys/mman.h>

#include "../utils/randlu.h"
uint32_t randlu(uint_fast32_t, uint_fast32_t);

#include "../utils/screen.h"
struct Screen;

#include "../sched/cfq.h"
struct SchedulerCFQ;
void handleSchedulerCFQ(SchedulerCFQ*, Screen*);

#define PROCESS_NUM 3
#define REQUEST_MAX 10

void program(Screen*, SchedulerCFQ*, uint_fast8_t*);
void process(Screen*, SchedulerCFQ*, uint_fast8_t*);
void request(Screen*, SchedulerCFQ*, uint_fast8_t*);

int main()
{
Screen* screen = (Screen*) mmap (NULL, sizeof(Screen), 
    PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0
);
SchedulerCFQ* scheduler = (SchedulerCFQ*) mmap(NULL, sizeof(SchedulerCFQ),
    PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0
);
uint_fast8_t* procnum_ptr = (uint_fast8_t*) mmap(NULL, sizeof(uint_fast8_t),
    PROT_READ | PROT_WRITE, MAP_SHARED | MAP_ANONYMOUS, -1, 0
);
 
    newScreen(screen);
    screen->init(screen);

    newSchedulerCFQ(scheduler);

    *procnum_ptr = PROCESS_NUM;
    program(screen, scheduler, procnum_ptr);

    return 0;
}

void program(Screen* screen, SchedulerCFQ* scheduler, uint_fast8_t* procnum_ptr)
{
pid_t pid = fork();
     
    if(pid == 0)        process(screen, scheduler, procnum_ptr);
    else if (pid > 0)   handleScreen(screen);
    else                exit(1);
}

void process(Screen* screen, SchedulerCFQ* scheduler, uint_fast8_t* procnum_ptr)
{
pid_t pid = fork();
     
    if(pid == 0)        request(screen, scheduler, procnum_ptr);
    else if (pid > 0)   handleSchedulerCFQ(scheduler, screen);
    else                exit(1);   
}

void request(Screen* screen, SchedulerCFQ* scheduler, uint_fast8_t* procnum_ptr)
{
pid_t pids[PROCESS_NUM], pids_top=0, pid, _pid;
uint_fast8_t process_num;
uint_fast8_t request_num;
uint_fast32_t requests[9];
char output[256];
uint_fast8_t ptr;

    process_num = (*procnum_ptr < UINT8_MAX ? *procnum_ptr : UINT8_MAX);

    for(uint_fast8_t i = 1; i < process_num; ++i) fork(); 
    pid = fork();

    if (pid == 0)
    {
        pids[pids_top++] = _pid = getpid();
            
        sprintf(output, "Process %d: Created!\n", _pid);
        screen->setRequest(screen, 0, output);

        while(1)
        {
            sleep(randlu(5, 30));

            sprintf(output, "Process %d: Requested [", _pid);
            ptr = 24 + (_pid > 999);

            request_num = randlu(3, 7);
            for(uint_fast8_t i = 0; i < request_num; ++i)
            {
                requests[i] = randlu(0, 99);
                ptr += sprintf(&output[ptr], " %d ", requests[i]);
            }
            output[ptr] = ']';
            output[++ptr] = '\n';
            output[++ptr] = 0x00;

            screen->setRequest(screen, 0, output);
            scheduler->setRequest(scheduler, screen, _pid, requests, request_num);    
            
            kill(_pid, SIGSTOP);
        }
    }
    else if (pid > 0)
        for (int i = 0; i < process_num; ++i) wait(NULL);
    else
    {
        screen->setRequest(screen, 0, "Error: Create process fail!");
        while(pids_top > 0)
        {
            pid = pids[--pids_top];
            kill(pid, SIGKILL);
        }

        exit(1);
    }
}
