/*****************************************************************************
*  文件名称  ： 最简实验例程1
*    作者    ： 郑朋桥
*    时间    ： 2016/8/18
******************************************************************************/
#include "OSAL.h"
#include "ZDApp.h"
#include "SampleApp.h"

/* HAL */
#include "hal_led.h"
#include "hal_key.h"
#include "hal_uart.h"
#include "hal_adc.h"
#include "MT_UART.h"


void SampleApp_HandleKeys( uint8 shift, uint8 keys );
void sendTemp();
/******************************************
*              
*     函数名称：SampleApp_Init
*     函数功能：应用层初始化
*
*******************************************/
void SampleApp_Init( uint8 task_id )
{ 
    /*  串口操作 */
    MT_UartInit(); //配置串口
    MT_UartRegisterTaskID(task_id); //打开串口
    RegisterForKeys( task_id ); // 登记所有的按键事件
}


/******************************************
*              
*     函数名称：SampleApp_ProcessEvent
*     函数功能：应用层任务处理函数
*
*******************************************/
uint16 SampleApp_ProcessEvent( uint8 task_id, uint16 events )
{
      afIncomingMSGPacket_t *MSGpkt;
      (void)task_id;  // Intentionally unreferenced parameter

      if ( events & SYS_EVENT_MSG ) //接收系统消息再进行判断
      {
            MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( task_id );
            while ( MSGpkt )
            {
              switch ( MSGpkt->hdr.event )
              {        
                   case KEY_CHANGE://按键事件
                    SampleApp_HandleKeys(((keyChange_t *)MSGpkt)->state, ((keyChange_t *)MSGpkt)->keys );
                   break;
              }

              // Release the memory 
              osal_msg_deallocate( (uint8 *)MSGpkt );

              // Next - if one is available 
              MSGpkt = (afIncomingMSGPacket_t *)osal_msg_receive( task_id );
            }

            // return unprocessed events 
            return (events ^ SYS_EVENT_MSG);
      }
      
      return 0;
}


/******************************************
*              
*     函数名称：SampleApp_HandleKeys
*     函数功能：按键事件处理函数
*
*******************************************/
void SampleApp_HandleKeys( uint8 shift, uint8 keys ) 
{
      (void)shift;  // Intentionally unreferenced parameter
      
      if ( keys & HAL_KEY_SW_6 ) //S1
      {
              HalLedSet(HAL_LED_1,HAL_LED_MODE_TOGGLE);//反转小灯
              sendTemp();
      }
}


void sendTemp()
{
    unsigned char adc_val = 0;
    unsigned char val[3];
    
    HalAdcInit();
    adc_val = HalAdcRead(HAL_ADC_CHANNEL_0,HAL_ADC_RESOLUTION_8);
    val[0] = adc_val % 100 / 10 + '0';
    val[1] = adc_val % 10 + '0';
   
    HalUARTWrite(0 ,  val , 2);
}
