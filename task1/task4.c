#include <linux/fs.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/utsname.h>
#include <linux/module.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/rtc.h>
#include <linux/workqueue.h>
#include <asm/uaccess.h>
#include <linux/delay.h>

static struct task_struct *task_p;
struct timeval tv;
struct rtc_time tm;
int start_time,end_time;

static int my_function(void){
	printk("Hello World!\n");
	do_gettimeofday(&tv);
	//rtc_time_to_tm(tv.tv_sec+8*3600,&tm);
	//printk("The end time is :%d-%d-%d %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
	return (int)tv.tv_usec;	
}

static void print(void)
{	
	int sum = 0,i;
	for(i = 1;i <= 100000; i++)//计算1...100000的和
	{
		sum += i;		
	}
	printk("The sum result is %d\n",sum);
}
static int __init hello_proc_init(void)
{	
	start_time = my_function();	
	print();
	end_time = my_function();
	printk("The time ing is %d us\n",end_time-start_time);//计算时间差
	return 0;
}
static void __exit hello_proc_exit(void)
{
	printk("GoodBye kernel\n");
	
}
MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
