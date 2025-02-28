#include "BareConfig.h"
#include "sys_menu.h"
#include "periphconfig.h"

#define RunTimeFaultCheck_ENABLE 0	/*使能故障巡检，300ms周期*/

void Bare_Begin(void)
{
	#if RunTimeFaultCheck_ENABLE
		static u8 RunTimeFaultCheck_TtemNow = 1;	/*故障巡检当前检查的项目，可以定位*/
		const u8 RunTimeFaultCheck_TtemMaxNum = 3;	/*故障巡检的最大项目数量*/
	#endif
	
	#if ((SYSTEM_FSMC_ENABLE) && (SYSTEM_FSMC_use4LCD)) && ((STM32F103xG) || (STM32F103xE))
	
	#else
		char* Head_buf = {"For Test!"};
		DrawPageHead((u8*)Head_buf);
	#endif
	BACK_COLOR = BLACK;
	for(;;)
	{
		#if SYS_SUPPORT_LWIP && NO_SYS
			lwip_pkt_handle();
		#endif
		/*注意：这里尽量不要用10毫秒中断标志！这里只用100毫秒、300毫秒、1秒和一分钟的中断标志，够用了*/
		if(Timer_IT_flags._10msec_flag == TRUE)
		{
			Timer_IT_flags._10msec_flag = FALSE;
			
			#if SYS_SUPPORT_LWIP
				lwip_periodic_handle();
			#endif
		}
		
		
		/*____________________________________100ms周期执行：可以为按键检测和处理等________________________________________*/
		if(Timer_IT_flags._100msec_flag == TRUE)
        {
            Timer_IT_flags._100msec_flag = FALSE;
			
			/*100ms周期要做的事情*/
			
			/*串口回显*/
			u16 num4uart1;
			char* buf4uart1 = mymalloc(InrRAM,(USART1_RX_BUF_MaxNum > USART1_RX_FIFO_MaxNum ? USART1_RX_BUF_MaxNum : USART1_RX_FIFO_MaxNum));
			if(buf4uart1 != NULL)
			{
				if(sys_USART1_RX_Fetch(FALSE, buf4uart1,&num4uart1) == ReturnOK)
				{
					printf_uart(UART1,"%s-%d",buf4uart1,num4uart1);
				}
			}
			myfree(InrRAM,buf4uart1);
			
			
		}
		/*____________________________________300ms周期执行：通常为运行时错误自检以及处理，刷屏等__________________________________________*/
		if(Timer_IT_flags._300msec_flag == TRUE)
		{
			Timer_IT_flags._300msec_flag = FALSE;
			
			/*集中故障巡检START*/
			#if RunTimeFaultCheck_ENABLE
				switch(RunTimeFaultCheck_TtemNow)
				{
					case 1:	
							//形式参考-> if( 执行检查的函数 ！= ReturnOK ) FaultASSERT(Init_OK_Num);
							break;
					case 2:	
							break;
					case 3:	
							break;
					default:break;
				}
				if(++RunTimeFaultCheck_TtemNow > RunTimeFaultCheck_TtemMaxNum)
					RunTimeFaultCheck_TtemNow = 1;
			#endif
			/*集中故障巡检END*/
			
			/*300ms周期要做的事情*/
			
			/*读入器件信息*/
			
			/*刷新界面信息*/
			
			keyProcess();	//菜单框架里的输入接管和获取函数
			menuProcess();	//菜单框架里的按照输入执行功能函数
			TestLED_Ctrl = !TestLED_Ctrl;
		}
		
		/*____________________________________1s周期执行：通常为运行状态指示，滴答心跳提醒等等__________________________________________*/
		if(Timer_IT_flags._1sec_flag == TRUE)
		{
			Timer_IT_flags._1sec_flag = FALSE;
			
			/*1s周期要做的事情*/
			
			//显示RTC时间
			
			char RTC_buf[50];
			#if SYSTEM_RTC_ENABLE
				sprintf(RTC_buf,"%d-%d-%d   %d-%d-%d",calendar.w_year,calendar.w_month,calendar.w_date,calendar.hour,calendar.min,calendar.sec);
				printf_uart(UART1,"%s",RTC_buf);
			#endif
			POINT_COLOR = BLUE;
			sprintf(RTC_buf,"%4ds",sys_GetsysRunTime(NULL,NULL,NULL));
			LCD_ShowString(5,20,16,(u8*)RTC_buf);
			
			
			//显示启动次数
			#if SYSTEM_FLASH_IAP_ENABLE
				sprintf(RTC_buf,"%d",StartUpTimes);
				LCD_ShowString(35,20,16,(u8*)RTC_buf);
			#endif
			
		}
		
		if(Timer_IT_flags._1min_flag == TRUE)
		{
			Timer_IT_flags._1min_flag = FALSE;
			
			/*1min周期要做的事情*/
			
		}
	}
}


