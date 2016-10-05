这工程只是利用Zigbee提供的接口来实现按键控制LED灯。
当按下S1按键时，将反转Led1小灯。



要在初始化时注册按键事件，这样操作系统才会在有按键事件时通知我们。
/******************************************
*              
*     函数名称：SampleApp_Init
*     函数功能：应用层初始化
*
*******************************************/
void SampleApp_Init( uint8 task_id )
{ 
    RegisterForKeys( task_id ); // 登记所有的按键事件
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
              HalLedSet(HAL_LED_1,HAL_LED_MODE_TOGGLE); //反转小灯
      }
}



使用用户事件：
1：定义事件
#define LED_TOGGLE_EVT                3
2：处理事件
if ( events & LED_TOGGLE_EVT )
{
    HalLedSet(HAL_LED_1,HAL_LED_MODE_TOGGLE);//反转小灯
    return events ^ LED_TOGGLE_EVT; //清除事件
}
3：启动事件
if ( keys & HAL_KEY_SW_6 ) //S1
{
        //启动事件
        osal_set_event( current_ID, LED_TOGGLE_EVT );
}