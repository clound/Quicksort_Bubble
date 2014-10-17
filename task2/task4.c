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
#define NUM 1000000
#define SIZE 76
static struct task_struct *task_p;
struct timeval tv;
struct rtc_time tm;
long start_time,end_time;

static int my_function(void){
	printk("Hello World!\n");
	do_gettimeofday(&tv);
	//rtc_time_to_tm(tv.tv_sec+8*3600,&tm);
	//printk("The end time is :%d-%d-%d %d:%d:%d\n",tm.tm_year+1900,tm.tm_mon+1,tm.tm_mday,tm.tm_hour,tm.tm_min,tm.tm_sec);
	return (long)tv.tv_sec;	
}

static int print(void)
{	
	struct file* fp;
	mm_segment_t fs;
	loff_t pos;
	char ch;
	char buf[SIZE] = {'\0'};
	int i;
	for(i = 0;i < SIZE;i++){//写入buf（0～z）
		buf[i] = i + '0';
	}
	buf[SIZE-1] = '\n';
	fp = filp_open("/home/output_file",O_RDWR|O_CREAT,0);
	if(IS_ERR(fp)){
		printk("open fail");
		return -1;
	}
	fs = get_fs();//得到模式
	set_fs(KERNEL_DS);//设置内核模式
	pos = 0;
	for(i = 0; i < NUM; i++){//写入100000次
		vfs_write(fp,buf,SIZE,&pos);
	}
	filp_close(fp,NULL);
	set_fs(fs);

}
static int __init hello_proc_init(void)
{	
	start_time = my_function();	
	print();
	end_time = my_function();
	printk("The time ing is %ld s\n",end_time-start_time);//算出时间差
	return 0;
}
static void __exit hello_proc_exit(void)
{
	printk("GoodBye kernel\n");
	
}
MODULE_LICENSE("GPL");
module_init(hello_proc_init);
module_exit(hello_proc_exit);
