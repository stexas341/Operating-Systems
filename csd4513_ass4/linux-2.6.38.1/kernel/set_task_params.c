#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/current.h>
#include <asm-generic/errno-base.h>

asmlinkage long sys_set_task_params(char group_name,int member_id){
	if((group_name >= 'A' && group_name <='Z') &&  member_id){
		current->group_name = group_name;
		current-> member_id = member_id;
		return 0;
	}
	else
		return EINVAL;
	printk("Set_task_params is called \n Stelios Sfakianakis csd4513");
}
