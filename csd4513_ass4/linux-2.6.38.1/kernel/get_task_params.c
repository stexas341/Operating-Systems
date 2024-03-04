#include <linux/kernel.h>
#include <asm/current.h>
#include <asm/uaccess.h>
#include <../include/asm-generic/errno-base.h>
#include <../include/linux/sched.h>
#include <../include/linux/d_params.h>

asmlinkage long sys_get_task_params(struct d_params * params){
	printk("get_task_params is called \n Stelios Sfakianakis csd4513");
	struct d_params param;
	int return_val

	if (!access_ok(VERIFY_READ, params, sizeof(struct d_params))){
	 return EINVAL;
	}
	if (!params) return EINVAL;


	if (!access_ok(VERIFY_WRITE, params sizeof(*params))){
		return EINVAL;
	}
	

	param.group_name = params->group_name;
	param.member_id = params->member_id;

	return_val = copy_to_user(params, &param, sizeof(struct d_params));
	
	return 0;}
