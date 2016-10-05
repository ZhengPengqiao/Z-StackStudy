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


将DHT11.h,DHT11.c导入工程

在按键中调用温湿度函数
void SampleApp_HandleKeys( uint8 shift, uint8 keys ) 
{
      (void)shift;  // Intentionally unreferenced parameter
      
      if ( keys & HAL_KEY_SW_6 ) //S1
      {
              HalLedSet(HAL_LED_1,HAL_LED_MODE_TOGGLE);//反转小灯
              sendTemp();
      }
}


向串口发送函数
void sendTemp()
{
    uint8 strData[20];
    uint8 temp[3]; 
    uint8 humidity[3];   

    
    DHT11();             //获取温湿度

    //将温湿度的转换成字符串
    temp[0]=wendu_shi+0x30;
    temp[1]=wendu_ge+0x30;
    humidity[0]=shidu_shi+0x30;
    humidity[1]=shidu_ge+0x30;
    
    osal_memcpy(strData,"TEMP:",5);
    osal_memcpy(&strData[5],temp,2);
    osal_memcpy(&strData[7],"   ",3);
    osal_memcpy(&strData[10],"Hum:",4);
    osal_memcpy(&strData[14],humidity,2);
    strData[16] = (uint8)'\n';
    HalUARTWrite(0,strData, 16);
    HalUARTWrite(0,"\n", 1); 
}
