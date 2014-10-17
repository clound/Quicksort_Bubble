#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <asm/uaccess.h>
#include <linux/sched.h>
#include <linux/timer.h>
#include <linux/kthread.h>
#include <linux/delay.h>
#include <linux/string.h>
#include <linux/rtc.h>
int start_time,end_time;
char buf[1000] = {'\0'};
char get_buf[20];
struct timeval tv;
struct rtc_time tm;
int start_time,end_time;
int num[100];
static int my_function(void){//得取系统时间
	do_gettimeofday(&tv);
	rtc_time_to_tm(tv.tv_sec+8*3600,&tm);
	return (int)tv.tv_usec;	
}
//将数字字符串转换成数字
static int atoi(char a[],int n)
{
	int ret = a[n-1] - '0';
	int i = n-2;
	int carry = 1;
 	for(;i >= 0; i--){
		carry *= 10;
		ret += (a[i]-'0')*carry;
	}
	return ret;
}
static void bubblesort(int num[],int k)//冒泡算法
{
	int i,j,temp;	
	for(i = 0;i < k;i++){
	 	for(j = 1;j < k-i;j++){
			if(num[j-1] > num[j]){
				temp = num[j-1];
				num[j-1] = num[j];
				num[j] = temp;		
			}
		}
	}
	
		
}
// static int partition(int num[],int left,int right)//取中间值
// {
// 	int i = left;
// 	int j = right;
// 	int temp = num[i];
// 	while(i < j)
// 	{
// 		while(i < j && num[j] >= temp)
// 			j--;
// 		if(i < j)
// 			num[i] = num[j];
// 		while(i < j && num[i] <= temp)
// 			i++;
// 		if(i < j)
// 			num[j] = num[i];
// 	}
// 	num[i] = temp;
// 	return i;
// }
// static int quicksort(int num[],int left,int right){//快排
// 	int dp;
// 	if(left < right){
// 		dp = partition(num,left,right);
// 		quicksort(num,left,dp-1);//递归快排
// 		quicksort(num,dp+1,right);	
// 	}
// }
static void readfile(void)//读文件中的数值
{
	struct file* fp;
	mm_segment_t fs;
	loff_t pos;
	fp = filp_open("/home/file",O_RDWR,0);
	if(IS_ERR(fp)){
		printk("open fail");
		return -1;
	}
	fs = get_fs();
	set_fs(KERNEL_DS);
	pos = 0;
	vfs_read(fp,buf,sizeof(buf),&pos);
	printk("read num is: %s\n",buf);
	filp_close(fp,NULL);
	set_fs(fs);
}
static int __init
init_kthread(void)
{
	int length,atoi_ret;
	int count = 0, i = 0,j = 0;
	int  m;
	readfile();
	while(buf[i] != '\0'){//读取转化为数值
		if(buf[i] != ' '){
			get_buf[j++] = buf[i];
			i++; 			
		}
		else{
			length = strlen(get_buf);
			atoi_ret = atoi(get_buf,length);//转换
			num[count++] = atoi_ret;
			memset(get_buf,'\0',20);//清空中防止不能覆盖下次的数值
			j = 0;i++;
		}
		
	}
	start_time = my_function();
	bubblesort(num,count);
	end_time = my_function();
	printk("The time survival is %d us\n",end_time-start_time);//冒泡的时间差
	for(i = 0; i < count; i++)
		printk(" After bubblesort num[%d] = %d\n",i,num[i]);
	// start_time = my_function();
	// quicksort(num,0,count);
	// end_time = my_function();
	// printk("The time survival is %d us\n",end_time-start_time);//快排的时间差
	// printk("count is %d\n",count);
	//  for(m = 0 ;m < count; m++)
	//  	printk(" After quicksort num[%d]= %d\n",m,num[m]);
	 
	return 0;
}
module_init(init_kthread);

static void __exit
exit_kthread(void)
{
	printk("GoodBye kernel\n");
}
module_exit(exit_kthread);
MODULE_LICENSE("GPL");


