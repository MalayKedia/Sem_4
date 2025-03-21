#include <pthread.h>

typedef struct zemaphore {
} zem_t;

void zem_init(zem_t *, int);
void zem_up(zem_t *);
void zem_down(zem_t *);
