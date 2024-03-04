#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <sys/syscalls.h>
#include "task_params.h"

int main(void) {
    printf("Trap to kernel level\n");
    syscall(341, 'A', 2);
    struct d_params param;
    syscall(342,&param );
    printf("Group name: %s and Member id: %d",param.group_name,param.member_id);
    printf("Back to user level\n");
    return 1;
}