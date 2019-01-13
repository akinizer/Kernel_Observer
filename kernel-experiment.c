#include <linux/module.h>	
#include <linux/kernel.h>
#include <linux/pid.h>	
#include <linux/moduleparam.h>         
#include <linux/init.h>
#include <linux/stat.h>
#include <linux/sched.h>
#include <linux/highmem.h>
#include <linux/mm.h>
#include <asm/page.h>
#include <linux/sched/task.h>

MODULE_LICENSE("GPL");

static int processid = 0;
static int virtaddr = 0;



module_param(processid, int, 0);
module_param(virtaddr, int, 0);

static int __init start(void) {
	struct task_struct *desired_task;
	desired_task = get_pid_task(find_vpid(processid), PIDTYPE_PID);
	if (desired_task == NULL){
		printk(KERN_INFO "Process not found.", processid);
		return 0;
	}
	if (desired_task->pid == processid){

		printk(KERN_INFO "Process info %d\n",desired_task->pid);

	}
	struct mm_struct *memstrc;
	memstrc = desired_task->mm;
	struct vm_area_struct *vma;
	vma = memstrc->mmap;
	int vma_count = 0;
	struct vm_area_struct *wanted_vma;
	while(vma != NULL){
		virtual_start = vma->vm_start;
		virtual_end = vma->vm_end;
		

		if(virtual_start <= vma->vm_mm->start_stack && virtual_end >= vma->vm_mm->start_stack){
			printk(KERN_INFO "%lx, %lx, %lu - stack\n", virtual_start, virtual_end, (virtual_end - virtual_start)/8);
			vma = vma->vm_next;
			vma_count = vma_count + 1;
			continue;
		}

		else if(virtual_start <= memstrc->end_code && virtual_end >= memstrc->start_code){
			printk(KERN_INFO "%lx, %lx, %lu - code\n", virtual_start, virtual_end, (virtual_end - virtual_start));
		}	
		else if (virtual_start <= memstrc->brk && virtual_end >= memstrc->start_brk){
			printk(KERN_INFO "%lx, %lx, %lu  - heap\n", virtual_start, virtual_end, (virtual_end - virtual_start)); 		
		}
		else if(virtual_start <= memstrc->end_data && virtual_end >= memstrc->start_data){
			printk(KERN_INFO "%lx, %lx, %lu - data\n", virtual_start, virtual_end, (virtual_end - virtual_start));
		}

		else{
			printk(KERN_INFO "%lx, %lx, %lu\n", virtual_start, virtual_end, (virtual_end - virtual_start)); 
		}
		if(virtual_start <= virtaddr && virtual_end >= virtaddr)
			wanted_vma = vma;
	vma_count = vma_count + 1;
	vma = vma->vm_next;
	}


	printk(KERN_INFO "%lu - rss\n", get_mm_rss(memstrc));
	printk(KERN_INFO "%lu - pages mapped\n", memstrc->total_vm);
	printk(KERN_INFO "%d - vm ares used\n", vma_count);
	printk(KERN_INFO "%lx, %lx, %lu - main arguments\n", memstrc->arg_start, memstrc->arg_end, memstrc->arg_end - memstrc->arg_start);
	printk(KERN_INFO "%lx, %lx, %lu - environment variables\n", memstrc->env_start, memstrc->env_end, memstrc->env_end - memstrc->env_start);
	
	long unsigned curr = memstrc->mmap->vm_start;
	long unsigned increment = 549755813888; // 2^39
	pgd_t *pgd; 
	int i;
	printk(KERN_INFO "top entries\n");
	for (i = 0; i < 512; ++i) {
		pgd = pgd_offset(memstrc, curr);
		if (pgd_bad(*pgd) || pgd_none(*pgd)) { // Referrenced from linux documentation
			printk(KERN_ERR "Entry %d, PGD: %lu, bad or doesn't exist\n", i, pgd->pgd);
			break;
		}
		else {
			printk(KERN_INFO "Entry %d - pgd %lu", i, pgd->pgd);
		}
		curr += curr + increment;
	}
	
	return 0;
}

static void __exit terminate(void) {
	printk(KERN_INFO "Terminating the program\n");
}

module_init(start);
module_exit(terminate);
