/**
  ******************************************************************************
  * @file    usb_hcd.c
  * @author  MCD Application Team
  * @version V2.2.1
  * @date    17-March-2018
  * @brief   Host Interface Layer
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2015 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                      <http://www.st.com/SLA0044>
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "usb_core.h"
#include "usb_hcd.h"
#include "usb_conf.h"
#include "usb_bsp.h"


/** @addtogroup USB_OTG_DRIVER
  * @{
  */
  
/** @defgroup USB_HCD 
  * @brief This file is the interface between EFSL ans Host mass-storage class
  * @{
  */


/** @defgroup USB_HCD_Private_Defines
  * @{
  */ 
/**
  * @}
  */ 
 

/** @defgroup USB_HCD_Private_TypesDefinitions
  * @{
  */ 
/**
  * @}
  */ 



/** @defgroup USB_HCD_Private_Macros
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_HCD_Private_Variables
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_HCD_Private_FunctionPrototypes
  * @{
  */ 
/**
  * @}
  */ 


/** @defgroup USB_HCD_Private_Functions
  * @{
  */ 

/**
  * @brief  HCD_Init 
  *         Initialize the HOST portion of the driver.
  * @param  pdev: Selected device
  * @param  base_address: OTG base address
  * @retval Status
  */
uint32_t HCD_Init(USB_OTG_CORE_HANDLE *pdev , 
                  USB_OTG_CORE_ID_TypeDef coreID)
{
  uint8_t i = 0;
  pdev->host.ConnSts = 0;
  
  for (i= 0; i< USB_OTG_MAX_TX_FIFOS; i++)
  {
  pdev->host.ErrCnt[i]  = 0;
  pdev->host.XferCnt[i]   = 0;
  pdev->host.HC_Status[i]   = HC_IDLE;
  }
  pdev->host.hc[0].max_packet  = 8; 

  USB_OTG_SelectCore(pdev, coreID);
#ifndef DUAL_ROLE_MODE_ENABLED
  USB_OTG_DisableGlobalInt(pdev);
  USB_OTG_CoreInit(pdev);

  /* Force Host Mode*/
  USB_OTG_SetCurrentMode(pdev , HOST_MODE);
  USB_OTG_CoreInitHost(pdev);
  USB_OTG_EnableGlobalInt(pdev);
#endif
   
  return 0;
}


/**
  * @brief  HCD_GetCurrentSpeed
  *         Get Current device Speed.
  * @param  pdev : Selected device
  * @retval Status
  */

uint32_t HCD_GetCurrentSpeed (USB_OTG_CORE_HANDLE *pdev)
{    
    USB_OTG_HPRT0_TypeDef  HPRT0;
    HPRT0.d32 = USB_OTG_READ_REG32(pdev->regs.HPRT0);
    
    return HPRT0.b.prtspd;
}

/**
  * @brief  HCD_ResetPort
  *         Issues the reset command to device
  * @param  pdev : Selected device
  * @retval Status
  */
uint32_t HCD_ResetPort(USB_OTG_CORE_HANDLE *pdev)
{
  /*
  Before starting to drive a USB reset, the application waits for the OTG 
  interrupt triggered by the denounce done bit (DBCDNE bit in OTG_FS_GOTGINT), 
  which indicates that the bus is stable again after the electrical denounce 
  caused by the attachment of a pull-up resistor on DP (FS) or DM (LS).
  */
  
  USB_OTG_ResetPort(pdev); 
  return 0;
}

/**
  * @brief  HCD_IsDeviceConnected
  *         Check if the device is connected.
  * @param  pdev : Selected device
  * @retval Device connection status. 1 -> connected and 0 -> disconnected
  * 
  */
uint32_t HCD_IsDeviceConnected(USB_OTG_CORE_HANDLE *pdev)
{
  return (pdev->host.ConnSts);
}


/**
  * @brief  HCD_IsPortEnabled 
  *         This function checks if port is enabled
  * @param  pdev : Selected device
  * @retval Frame number
  * 
  */
uint32_t HCD_IsPortEnabled(USB_OTG_CORE_HANDLE *pdev)
{
  return (pdev->host.PortEnabled);
}

/**
  * @brief  HCD_GetCurrentFrame 
  *         This function returns the frame number for sof packet
  * @param  pdev : Selected device
  * @retval Frame number
  * 
  */
uint32_t HCD_GetCurrentFrame (USB_OTG_CORE_HANDLE *pdev) 
{
 return (USB_OTG_READ_REG32(&pdev->regs.HREGS->HFNUM) & 0xFFFF) ;
}

/**
  * @brief  HCD_GetURB_State 
  *         This function returns the last URBstate
  * @param  pdev: Selected device
  * @retval URB_STATE
  * 
  */
USB_OTG_URBStateTypeDef HCD_GetURB_State (USB_OTG_CORE_HANDLE *pdev , uint8_t ch_num) 
{
  return pdev->host.URB_State[ch_num] ;
}

/**
  * @brief  HCD_GetXferCnt 
  *         This function returns the last URBstate
  * @param  pdev: Selected device
  * @retval No. of data bytes transferred
  * 
  */
uint32_t HCD_GetXferCnt (USB_OTG_CORE_HANDLE *pdev, uint8_t ch_num) 
{
  return pdev->host.XferCnt[ch_num] ;
}



/**
  * @brief  HCD_GetHCState 
  *         This function returns the HC Status 
  * @param  pdev: Selected device
  * @retval HC_STATUS
  * 
  */
USB_OTG_HCStateTypeDef HCD_GetHCState (USB_OTG_CORE_HANDLE *pdev ,  uint8_t ch_num) 
{
  return pdev->host.HC_Status[ch_num] ;
}

/**
  * @brief  HCD_HC_Init 
  *         This function prepare a HC and start a transfer
  * @param  pdev: Selected device
  * @param  hc_num: Channel number 
  * @retval status 
  */
uint32_t HCD_HC_Init (USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num) 
{
  return USB_OTG_HC_Init(pdev, hc_num);  
}

/**
  * @brief  HCD_SubmitRequest 
  *         This function prepare a HC and start a transfer
  * @param  pdev: Selected device
  * @param  hc_num: Channel number 
  * @retval status
  */
uint32_t HCD_SubmitRequest (USB_OTG_CORE_HANDLE *pdev , uint8_t hc_num) 
{
  
  pdev->host.URB_State[hc_num] =   URB_IDLE;  
  pdev->host.hc[hc_num].xfer_count = 0 ;
  return USB_OTG_HC_StartXfer(pdev, hc_num);
}


/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
