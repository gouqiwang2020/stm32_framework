#include "malloc.h"	    

/*此文件借鉴 ALIENTEK战舰STM32开发板V3 内存管理 的源代码*/
/*后来又发现，硬石的YSFx系列开发板历程里的内存管理源代码和这里的几乎一模一样，而且都说自己是作者...*/

//内存池(32字节对齐)（真正储存东西的地方）
__align(32) u8 mem1base[InrRAM_MAX_SIZE];														//内部SRAM内存池
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM)) && ((STM32F103xG) || (STM32F103xE))
	__align(32) u8 mem2base[ExRAM1_MAX_SIZE] __attribute__((at(0X68000000)));					//外部SRAM内存池
#else
	__align(32) u8 mem2base[1]; //防止报错
#endif

//内存管理表
u16 mem1mapbase[InrRAM_ALLOC_TABLE_SIZE];													//内部SRAM内存池MAP
#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4SRAM)) && ((STM32F103xG) || (STM32F103xE))
	u16 mem2mapbase[ExRAM1_ALLOC_TABLE_SIZE] __attribute__((at(0X68000000 + ExRAM1_MAX_SIZE)));	//外部SRAM内存池MAP
#else 
	u16 mem2mapbase[1]; //防止报错
#endif

//内存管理参数	   
const u32 memtblsize[RAM_Num] = {InrRAM_ALLOC_TABLE_SIZE,	ExRAM1_ALLOC_TABLE_SIZE};		//存储各个内存表的大小参数
const u32 memblksize[RAM_Num] = {InrRAM_BLOCK_SIZE,			ExRAM1_BLOCK_SIZE};				//存储各个内存块的分块大小参数
const u32 memsize[RAM_Num]	= 	{InrRAM_MAX_SIZE,			ExRAM1_MAX_SIZE};				//存储各个内存块的字节数总大小参数


//内存管理控制器
struct _m_malloc_dev malloc_dev =
{
	my_mem_init,				//内存初始化
	my_mem_perused,				//内存使用率
	mem1base,mem2base,			//内存池
	mem1mapbase,mem2mapbase,	//内存管理状态表
	0,0,  		 				//内存管理未就绪
};

//内存管理初始化  
//memx:所属内存块
void my_mem_init(u8 memx)  
{  
    mymemset(malloc_dev.memmap[memx], 	0,	memtblsize[memx]*2	);	//内存状态表数据清零  
	mymemset(malloc_dev.membase[memx], 	0,	memsize[memx]		);	//内存池所有数据清零  
	malloc_dev.memrdy[memx] = 1;									//内存管理初始化OK
}  
//获取内存使用率
//memx:所属内存块
//返回值:使用率(0~100)
u8 my_mem_perused(u8 memx)  
{  
    u32 used = 0;  
    u32 i;  
    for(i=0;i<memtblsize[memx];i++)  
    {  
        if(malloc_dev.memmap[memx][i])
			used++; 
    } 
    return ((used*100)/(memtblsize[memx]));  
}  
//内存分配(内部调用)
//memx:所属内存块
//size:要分配的内存大小(字节)
//返回值:0XFFFFFFFF,代表错误;其他,内存偏移地址 
u32 my_mem_malloc(u8 memx,u32 size)  
{  
    signed long offset = 0;  
    u32 nmemb;		//需要的内存块数  
	u32 cmemb = 0;	//连续空内存块数
    u32 i;  
    if(!malloc_dev.memrdy[memx])malloc_dev.init(memx);//未初始化,先执行初始化 
    if(size == 0)return 0XFFFFFFFF;		//不需要分配
    nmemb = size/memblksize[memx];  	//获取需要分配的连续内存块数
    if(size%memblksize[memx])nmemb++;  
    for(offset=memtblsize[memx]-1;offset>=0;offset--)//搜索整个内存控制区  
    {     
		if(!malloc_dev.memmap[memx][offset])cmemb++;//连续空内存块数增加
		else cmemb = 0;								//连续内存块清零
		if(cmemb==nmemb)							//找到了连续nmemb个空内存块
		{
            for(i=0;i<nmemb;i++)  					//标注内存块非空 
            {  
                malloc_dev.memmap[memx][offset+i] = nmemb;  
            }  
            return (offset*memblksize[memx]);//返回偏移地址  
		}
    }  
    return 0XFFFFFFFF;//未找到符合分配条件的内存块  
}  
//释放内存(内部调用) 
//memx:所属内存块
//offset:内存地址偏移
//返回值:0,释放成功;1,释放失败;  
u8 my_mem_free(u8 memx,u32 offset)  
{  
    int i;  
    if(!malloc_dev.memrdy[memx])//未初始化,先执行初始化
	{
		malloc_dev.init(memx);    
        return 1;//未初始化  
    }  
    if(offset<memsize[memx])//偏移在内存池内. 
    {  
        int index=offset/memblksize[memx];			//偏移所在内存块号码  
        int nmemb=malloc_dev.memmap[memx][index];	//内存块数量
        for(i=0;i<nmemb;i++)  						//内存块清零
        {  
            malloc_dev.memmap[memx][index+i] = 0;  
        }  
        return 0;  
    }else return 2;//偏移超区了.  
}  
//释放内存(外部调用) 
//memx:所属内存块
//ptr:内存首地址 
void myfree(u8 memx,void *ptr)  
{  
	u32 offset;   
	if(ptr == NULL)return;//地址为0.  
 	offset = (u32)ptr-(u32)malloc_dev.membase[memx];     
    my_mem_free(memx,offset);	//释放内存
}  
//分配内存(外部调用)
//memx:所属内存块
//size:内存大小(字节)
//返回值:分配到的内存首地址.
void *mymalloc(u8 memx,u32 size)  
{  
    u32 offset;   
	offset = my_mem_malloc(memx,size);  
    if(offset == 0XFFFFFFFF)return NULL;  
    else return (void*)((u32)malloc_dev.membase[memx] + offset);  
}  
//重新分配内存(外部调用)
//memx:所属内存块
//*ptr:旧内存首地址
//size:要分配的内存大小(字节)
//返回值:新分配到的内存首地址.
void *myrealloc(u8 memx,void *ptr,u32 size)  
{  
    u32 offset;    
    offset=my_mem_malloc(memx,size);   	
    if(offset==0XFFFFFFFF)return NULL;     
    else  
    {  									   
	    mymemcpy((void*)((u32)malloc_dev.membase[memx]+offset),ptr,size);	//拷贝旧内存内容到新内存   
        myfree(memx,ptr);  											  		//释放旧内存
        return (void*)((u32)malloc_dev.membase[memx]+offset);  				//返回新内存首地址
    }  
}












