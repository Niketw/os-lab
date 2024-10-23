#include "types.h"
#include "stat.h"
#include "user.h"

int main(void)
{
    printf(1, "Unix start time: %d", getyear());
    exit();
}