#include <sys/time.h>

#define RANDOM_SEQ_MAX 1e9+7
uint_fast32_t random_seq = RANDOM_SEQ_MAX - 1;

uint32_t randlu(uint_fast32_t lower, uint_fast32_t upper)
{
struct timeval tv;
    
    gettimeofday(&tv, NULL);

    ++random_seq;
    random_seq = (random_seq >= RANDOM_SEQ_MAX ? tv.tv_usec : random_seq);

    return lower + (rand() + tv.tv_sec * random_seq) % (upper - lower + 1);
}
