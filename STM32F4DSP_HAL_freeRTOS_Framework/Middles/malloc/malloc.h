#ifndef __MALLOC_H
#define __MALLOC_H
#include "PeriphConfigCore.h"

/*此文件借鉴 ALIENTEK战舰STM32开发板V3 内存管理 的源代码*/
/*后来又发现，硬石的YSFx系列开发板历程里的内存管理源代码和这里的几乎一模一样，而且都说自己是作者...*/

/*______________________用户配置___________________________*/
#define RAM_Num 		3		//定义支持的SRAM块数
/*如果增添了第四块内存，在下面要照葫芦画瓢增加相应参数信息*/

/*______________________用户配置___________________________*/
//定义内存池标志
#define InrRAM	 		0		//内部内存池

#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM))
	#define ExRAM1   	1		//外部内存池1
#endif

#define InrCCM  		2		//CCM内存池(此部分SRAM仅仅CPU可以访问!!!)
//#define ExRAM2   		3		//外部内存池2（保留项，暂时没用到）

/*______________________用户配置：InrRAM___________________________*/
//InrRAM内存参数设定，InrRAM完全处于内部SRAM里面
#define InrRAM_BLOCK_SIZE			32  	  							//内存块大小为32字节（一般不用动，保持默认就好）
#if VECT_TAB_SRAM	/*程序下载到SRAM内DEBUG，最大程序64KB，内存数据64KB*/
	#define InrRAM_MAX_SIZE			40*1024  							//最大管理内存 40K （根据docs说明文件，为大局考虑，这里不要轻易动！）
#else				/*程序下载到FLASH内固定，最大程序1024KB，内存数据128KB*/
	#define InrRAM_MAX_SIZE			70*1024  							//最大管理内存 70K （根据docs说明文件，为大局考虑，这里不要轻易动！）
#endif
#define InrRAM_ALLOC_TABLE_SIZE	(InrRAM_MAX_SIZE/InrRAM_BLOCK_SIZE) 	//内存表大小（这里就莫要乱动啦）

/*______________________用户配置：ExRAM1___________________________*/
//ExRAM1内存参数设定，ExRAM1的内存池处于外部SRAM里面（对于1M空间的外部RAM，这里保持默认就好）
#define ExRAM1_BLOCK_SIZE			32  	  							//内存块大小为32字节（一般不用动，保持默认就好）
#define ExRAM1_MAX_SIZE			960*1024  								//最大管理内存960K（对于1M空间的外部RAM，这里保持默认就好）
#define ExRAM1_ALLOC_TABLE_SIZE	(ExRAM1_MAX_SIZE/ExRAM1_BLOCK_SIZE) 	//内存表大小，在ExRAM1_MAX_SIZE为960K情况下，这个占30K
/*如果增添了第三块内存，然后要去.c文件修改要修改的部分*/

/*______________________用户配置：InrCCM___________________________*/
//InrCCM内存参数设定，用于管理CCM(特别注意,这部分SRAM,仅CPU可以访问!!)
#define InrCCM_BLOCK_SIZE			32  	  							//内存块大小为32字节
#define InrCCM_MAX_SIZE			60*1024  								//最大管理内存60K（默认不用动）
#define InrCCM_ALLOC_TABLE_SIZE	(InrCCM_MAX_SIZE/InrCCM_BLOCK_SIZE)		//内存表大小


 /*________________________以下默认的不用动________________________*/
//内存管理控制器
struct _m_malloc_dev
{
	void (*init)(u8);					//初始化
	float (*perused)(u8);		  	    	//内存使用率
	u8 	*membase[RAM_Num];				//内存池 管理SRAMBANK个区域的内存
	u16 *memmap[RAM_Num]; 				//内存管理状态表
	u8  memrdy[RAM_Num]; 				//内存管理是否就绪
};
extern struct _m_malloc_dev malloc_dev;	 //在malloc.c里面定义
    
void my_mem_init(u8 memx);				//内存管理初始化函数(外/内部调用)
u32 my_mem_malloc(u8 memx,u32 size);	//内存分配(内部调用)
u8 my_mem_free(u8 memx,u32 offset);		//内存释放(内部调用)
float my_mem_perused(u8 memx);				//获得内存使用率(外/内部调用) 
////////////////////////////////////////////////////////////////////////////////
//用户调用函数
void myfree(u8 memx,void *ptr);  			//内存释放(外部调用)
void *mymalloc(u8 memx,u32 size);			//内存分配(外部调用)
void *myrealloc(u8 memx,void *ptr,u32 size);//重新分配内存(外部调用)
#endif













