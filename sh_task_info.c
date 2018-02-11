#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/sched.h>
#include <linux/syscalls.h>
#include <linux/file.h> 
#include <linux/fs.h>
#include <linux/fcntl.h> 
#include <asm/uaccess.h> 
#include <uapi/asm-generic/errno-base.h>
#include "taskinfo.h"

void write_to_file(struct file *file, char* data)
{
	file->f_op->write(file, data, strlen(data), &file->f_pos);
}
asmlinkage long sys_sh_task_info(int pid, char* filename) 
{
	if (pid > 32768 || pid <= 0)
		return -EINVAL;
	struct task_struct *task;
	struct file *file;
	char data[500], concat[500];
	loff_t pos = 0;
  	int fd;

  	mm_segment_t old_fs = get_fs();
  	set_fs(KERNEL_DS);

  	fd = sys_open(filename, O_WRONLY|O_CREAT, 0644);
	for_each_process(task)
	{
		if ((int)task->pid == pid)
		{
			printk("Process: %s\n", task->comm);
			strcpy(data, "Process: ");
			strcat(data, task->comm);
			strcat(data, "\n");
			
			printk("PID Number: %ld\n", (long)task->pid);
			strcat(data, "PID NUmber: ");
			sprintf(concat, "%ld\n", (long)task->pid);
			strcat(data, concat);
			
			printk("Process State: %ld\n", (long)task->state);
			strcat(data, "Process State: ");
			sprintf(concat, "%ld\n", (long)task->state);
			strcat(data, concat);
			
			printk("Priority: %ld\n", (long)task->prio);
			strcat(data, "Priority: ");
			sprintf(concat, "%ld\n", (long)task->prio);
			strcat(data, concat);
			
			printk("RT_Priority: %ld\n", (long)task->rt_priority);
			strcat(data, "RT_Priority: ");
			sprintf(concat, "%ld\n", (long)task->rt_priority);
			strcat(data, concat);
			
			printk("Static Priority: %ld\n", (long)task->static_prio);
			strcat(data, "Static Priority: ");
			sprintf(concat, "%ld\n", (long)task->static_prio);
			strcat(data, concat);
			
			printk("Normal Priority: %ld\n", (long)task->normal_prio);
			strcat(data, "Normal Priority: ");
			sprintf(concat, "%ld\n", (long)task->normal_prio);
			strcat(data, concat);
			
			if (fd < 0)
				return -EISDIR;
			file = fget(fd);
			write_to_file(file, data);
		}
		printk("\n\n");
	}
	set_fs(old_fs);
	return 0;
}
