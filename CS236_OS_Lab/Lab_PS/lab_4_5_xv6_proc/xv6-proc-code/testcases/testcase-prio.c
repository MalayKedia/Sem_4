#include "types.h"
#include "stat.h"
#include "user.h"
#include "processInfo.h"
int main(void)
{
    for(int i=1; i<10; i++){
        setprio(i);
        printf(1, "Priority : %d\n", getprio());
    }
    exit();
}