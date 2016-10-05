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
#include "MT_UART.h"
#include "OSAL_Nv.h"

#define MY_DEFINE_UART_PORT 0  //自定义串口号（0，1）
#define RX_MAX_LENGTH 20       //接收缓冲区最大值：20个字节
uint8 RX_BUFFER[RX_MAX_LENGTH]; //接收缓冲区
void UartCallBackFunction(uint8 port, uint8 event); //回调函数声明，定义在最后面
void SampleApp_HandleKeys( uint8 shift, uint8 keys );

/*   配置串口      */
halUARTCfg_t uartConfig; //定义串口配置结构体变量；
void Uart_Config(void); //函数声明；


/*****************************************************************************
*  函数名称  ： Uart_Config
*  函数介绍  ：配置串口
*            ：
*    参数    ： 无
*   返回值   ： 无
******************************************************************************/

void Uart_Config(void) 
{
  uartConfig.configured = TRUE; //允许配置
  uartConfig.baudRate = HAL_UART_BR_115200; //波特率
  uartConfig.flowControl = FALSE;
  uartConfig.flowControlThreshold = 64;
  uartConfig.rx.maxBufSize = 128;
  uartConfig.tx.maxBufSize = 218;
  uartConfig.idleTimeout = 6;
  uartConfig.intEnable = TRUE;
  uartConfig.callBackFunc = UartCallBackFunction;
}


/******************************************
*              
*     函数名称：SampleApp_Init
*     函数功能：应用层初始化
*
*******************************************/
void SampleApp_Init( uint8 task_id )
{ 
      
    /*  串口操作  */
    Uart_Config(); //配置串口
    HalUARTOpen(MY_DEFINE_UART_PORT , &uartConfig); //打开串口
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
      }
}

/*****************************************************************************
*  函数名称  ： UartCallBackFunction
*  函数介绍  ： 串口回调函数
*            ：
*    参数    ： port:串口号
*            ： event:事件
*   返回值   ： 无
******************************************************************************/
static void UartCallBackFunction(uint8 port , uint8 event)
{
  uint8 r_val;
  uint8 w_val = 24;
  uint8 r_s[3];
  uint8 RX_Length = 0; //接收到字符串大小
  RX_Length = Hal_UART_RxBufLen(MY_DEFINE_UART_PORT); //读取接收字符串大小；
  if(RX_Length != 0) //有数据存在
  {
            //读取串口数据；
        HalUARTRead(MY_DEFINE_UART_PORT , RX_BUFFER , RX_Length); //读取字符串
        if(osal_memcmp(RX_BUFFER,"nvinit",6))  //如果字符串前六个为"nvinit"
        {
          osal_nv_item_init(0x1200,1,NULL);
          HalUARTWrite(0,"InitOK\n",7);
        }
        else if(osal_memcmp(RX_BUFFER,"nvread",6))//如果字符串前六个为"nvread"
        {
          osal_nv_read(0x1200,0,1,&r_val);
          r_s[0] = r_val/10 + 0x30;
          r_s[1] = r_val%10 + 0x30;
          r_s[2] = '\n';
          HalUARTWrite(0,r_s,3);
          HalUARTWrite(0,"readOK\n",7);
        }
        else if(osal_memcmp(RX_BUFFER,"nvwrite",6))//如果字符串前六个为"nvwrite"
        {
          if( ('0' <= RX_BUFFER[7]) &&  (RX_BUFFER[7] <= '9') ) //使用格式为nvwrite+数字，否则为错误
          {
            w_val = (RX_BUFFER[7]-'0') * 10;
            w_val += RX_BUFFER[8]-'0';
            osal_nv_write(0x1200,0,1,&w_val);
            HalUARTWrite(0,"writeOK\n",8);
          }
          else
          {
            HalUARTWrite(0,"err\n",4);
          }
        }
        
        //发送回给电脑,使用 hal_uart.h 的接口函数：
        HalUARTWrite(MY_DEFINE_UART_PORT ,  RX_BUFFER , RX_Length);//回显
        
        HalUARTWrite(MY_DEFINE_UART_PORT ,  "\n" , 1);
  }
}