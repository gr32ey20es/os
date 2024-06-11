#define PROCESS_MAX 256
#define SCHED_BUFFER_REQUEST_MAX 256

typedef struct SchedulerCFQ
{
    uint_fast8_t process_num;
    struct schedulerCFQ_buffer
    {
        uint_fast8_t  top;
        uint_fast16_t process_id;
        uint_fast32_t request[SCHED_BUFFER_REQUEST_MAX];
    } buffer[PROCESS_MAX];

    void (*print)(struct SchedulerCFQ*, Screen*);
    uint_fast32_t (*setRequest)
    (
     struct SchedulerCFQ*, Screen*,
     uint_fast16_t, uint_fast32_t*, uint_fast8_t
    );
    uint_fast32_t (*getRequest)(struct SchedulerCFQ*, uint_fast8_t, uint_fast32_t*);
} SchedulerCFQ;

void printSchedulerCFQ(struct SchedulerCFQ* self, Screen* screen)
{
struct schedulerCFQ_buffer* buffer;
char output[256];
uint_fast8_t ptr;

    for(uint_fast8_t i = 0; i < self->process_num; ++i)
    {
        buffer = &(self->buffer[i]);
        sprintf(output, "Process %d: [", buffer->process_id);
        ptr = 14 + (buffer->process_id > 999);

        for(uint_fast8_t j = 0; j < buffer->top; ++j)
            ptr += sprintf(&output[ptr], " %d ", buffer->request[j]);

        output[ptr] = ']';
        output[++ptr] = '\n';
        output[++ptr] = 0x00;
        screen->setRequest(screen, 1, output);
    }
    screen->setRequest(screen, 1, "\n");
}

uint_fast32_t setRequestSchedulerCFQ
(
 struct SchedulerCFQ* self, Screen* screen, 
 uint_fast16_t process_id, uint_fast32_t* requests, uint_fast8_t request_num
) 
{
uint_fast8_t i = 0;
struct schedulerCFQ_buffer* buffer;

    int requestCompare(const void* req0, const void* req1)
    {
        uint_fast32_t* x = (uint_fast32_t*) req0;
        uint_fast32_t* y = (uint_fast32_t*) req1;
        return (*y > *x) ? 1 : -1;
    }

    for(i = 0; i < self->process_num; ++i)
        if (self->buffer[i].process_id == process_id)
            break;
    
    buffer = &(self->buffer[i]);
    if (i >= self->process_num) self->process_num = i + 1;

    buffer->top = request_num;
    buffer->process_id = process_id;
    for(i = 0; i < request_num; ++i)
        buffer->request[i] = requests[i];

    qsort(buffer->request, request_num, sizeof(uint_fast32_t), requestCompare);

    self->print(self, screen);
    return 0;
}
uint_fast32_t getRequestSchedulerCFQ(struct SchedulerCFQ* self, uint_fast8_t index, uint_fast32_t* requests)
{
struct schedulerCFQ_buffer* buffer = &(self->buffer[index]);
uint_fast8_t request_num = buffer->top;
    
    request_num = request_num < 4 ? request_num : 4;  // request_slice = 4;
    for (uint_fast8_t i = 0; i < request_num; ++i)
        requests[i] = buffer->request[buffer->top - i - 1];

    buffer->top -= request_num;

    return request_num;
}

void newSchedulerCFQ(struct SchedulerCFQ* scheduler)
{
    scheduler->process_num = 0;
    scheduler->print = printSchedulerCFQ;
    scheduler->setRequest = setRequestSchedulerCFQ;
    scheduler->getRequest = getRequestSchedulerCFQ;
}

void handleSchedulerCFQ(struct SchedulerCFQ* scheduler, Screen* screen)
{
uint_fast32_t request[4];
uint_fast32_t request_num;
char output[256];
uint_fast8_t ptr;

    sleep(10);
    while(1)
    {
        for(uint_fast8_t i = 0; i < scheduler->process_num; ++i)
        {
            if(scheduler->buffer[i].top)
            {
                sleep((scheduler->buffer[i].top < 4 ? scheduler->buffer[i].top : 4));
                request_num = scheduler->getRequest(scheduler, i, request);

                sprintf(output, "Process %d: _Received [", scheduler->buffer[i].process_id);
                ptr = 24 + (scheduler->buffer[i].process_id > 999);

                for(uint_fast8_t j = 0; j < request_num; ++j)
                    ptr += sprintf(&output[ptr], " %d ", request[j]);

                output[ptr] = ']';
                output[++ptr] = '\n';
                output[++ptr] = 0x00;

                screen->setRequest(screen, 0, output);
                scheduler->print(scheduler, screen);

                sleep(1);
                if(scheduler->buffer[i].top == 0)
                    kill(scheduler->buffer[i].process_id, SIGCONT); 
            }
        }
        sleep(1);
    }
}
