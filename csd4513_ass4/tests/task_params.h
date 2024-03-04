#ifndef TASK_PARAMS_H
#define TASK_PARAMS_H

#define __NR_set_task_params    341
#define __NR_get_task_params    342

#define set_task_params(char group_name, int member_id) syscall(__NR_set_task_params, group_name, member_id)
#define get_task_params(params) syscall(__NR_get_task_params, params)

typedef struct d_params {
        char group_name;
        int member_id;
} d_params;