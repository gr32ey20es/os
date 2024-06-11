#include <ncurses.h>

#define SCREEN_BUFFER_REQUEST_MAX 256
#define SCREEN_BUFFER_REQUEST_OUTPUT_MAX 256
typedef struct Screen
{
    WINDOW *request, *schedule;
    struct screen_buffer 
    {
        uint_fast8_t in, out;
        struct screen_request
        {
            uint_fast32_t option;
            char output[SCREEN_BUFFER_REQUEST_OUTPUT_MAX];
        } request[SCREEN_BUFFER_REQUEST_MAX];
    } buffer;

    void (*init)(struct Screen*);
    uint_fast32_t (*setRequest)(struct Screen*, uint_fast32_t , char* );
    uint_fast32_t (*getRequest)(struct Screen*, uint_fast32_t*, char**);
} Screen;

void initScreen(struct Screen* self)
{
uint_fast32_t height, width;
    
    void setScreenBoundary()
    {
        void setWinBoundary(int nlines, int ncols, int begin_y, int begin_x, char* title)
        {
        WINDOW* win = newwin(nlines, ncols, begin_y, begin_x);

            wborder(win, 0, 0, 0, 0, 0, 0, 0, 0);

            waddch (win, ACS_ULCORNER);
            for (uint_fast32_t i = 0; i < 3; ++i)
                waddch(win, ACS_HLINE);
            wprintw(win, title);

            wrefresh(win);

            delwin(win);
        }

        setWinBoundary(height * 0.75, width * 0.75 / 2, height / 8, width / 8, "Request" );
        setWinBoundary(height * 0.75, width * 0.75 / 2, height / 8, width / 2, "Schedule");
    }

    initscr(); 
    getmaxyx(stdscr, height, width);   

    setScreenBoundary();
    self->request  = newwin(height*0.75 - 2, width*0.75/2 - 4, height/8 + 1, width/8 + 2);
    self->schedule = newwin(height*0.75 - 2, width*0.75/2 - 4, height/8 + 1, width/2 + 2);

    cbreak();
    scrollok(self->request,  TRUE);
    scrollok(self->schedule, TRUE);
}

uint_fast32_t setRequestScreen(struct Screen* self, uint_fast32_t option, char* output)
{  
uint_fast8_t* in;
struct screen_request* request;

    in = &(self->buffer.in);
    request = &(self->buffer.request[*in]);
    
    *in = (*in < 255 ? *in + 1 : 0);
    request->option = option;
    strcpy(request->output, output);

    return 0;
}

uint_fast32_t getRequestScreen(struct Screen* self, uint_fast32_t* option, char** output)
{
uint_fast8_t* out;
struct screen_request* request;

    out = &(self->buffer.out);

    if(*out < self->buffer.in)
    {
        request = &(self->buffer.request[*out]);

        *out = (*out < 255 ? *out + 1 : 0);
        *option = request->option;
        *output = request->output;

        return 0;
    }

    return 1;
}

void newScreen(struct Screen* screen)
{
    screen->buffer.in  = 0;
    screen->buffer.out = 0;
    screen->init = initScreen;
    screen->setRequest = setRequestScreen;
    screen->getRequest = getRequestScreen;
}

void handleScreen(struct Screen* screen)
{
uint_fast32_t option;
char* output;
WINDOW** win;
    
    while(1)
    {
        if(!screen->getRequest(screen, &option, &output))
        {
            win = (option == 0 ? &(screen->request) : &(screen->schedule));
            wprintw(*win, output);
            wrefresh(*win);
        }
        usleep(100000);
    }
}
