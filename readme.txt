调用hal_adc.c文件中的函数得到adc值，
HalAdcRead(HAL_ADC_CHANNEL_0,HAL_ADC_RESOLUTION_8);
函数的参数在hal_adc.h中定义

/* 分辨率 */
#define HAL_ADC_RESOLUTION_8       0x01
#define HAL_ADC_RESOLUTION_10      0x02
#define HAL_ADC_RESOLUTION_12      0x03
#define HAL_ADC_RESOLUTION_14      0x04

/* 通道 */
#define HAL_ADC_CHANNEL_0          0x00
#define HAL_ADC_CHANNEL_1          0x01
#define HAL_ADC_CHANNEL_2          0x02
#define HAL_ADC_CHANNEL_3          0x03
#define HAL_ADC_CHANNEL_4          0x04
#define HAL_ADC_CHANNEL_5          0x05
#define HAL_ADC_CHANNEL_6          0x06
#define HAL_ADC_CHANNEL_7          0x07

#define HAL_ADC_CHN_AIN0    0x00    /* AIN0 */
#define HAL_ADC_CHN_AIN1    0x01    /* AIN1 */
#define HAL_ADC_CHN_AIN2    0x02    /* AIN2 */
#define HAL_ADC_CHN_AIN3    0x03    /* AIN3 */
#define HAL_ADC_CHN_AIN4    0x04    /* AIN4 */
#define HAL_ADC_CHN_AIN5    0x05    /* AIN5 */
#define HAL_ADC_CHN_AIN6    0x06    /* AIN6 */
#define HAL_ADC_CHN_AIN7    0x07    /* AIN7 */
#define HAL_ADC_CHN_A0A1    0x08    /* AIN0,AIN1 */
#define HAL_ADC_CHN_A2A3    0x09    /* AIN2,AIN3 */
#define HAL_ADC_CHN_A4A5    0x0a    /* AIN4,AIN5 */
#define HAL_ADC_CHN_A6A7    0x0b    /* AIN6,AIN7 */
#define HAL_ADC_CHN_GND     0x0c    /* GND */
#define HAL_ADC_CHN_VREF    0x0d    /* Positive voltage reference */
#define HAL_ADC_CHN_TEMP    0x0e    /* Temperature sensor */
#define HAL_ADC_CHN_VDD3    0x0f    /* VDD/3 */
#define HAL_ADC_CHN_BITS    0x0f    /* Bits [3:0] */

#define HAL_ADC_CHANNEL_TEMP       HAL_ADC_CHN_TEMP
#define HAL_ADC_CHANNEL_VDD        HAL_ADC_CHN_VDD3   /* channel VDD divided by 3 */

/* Vdd Limits */
#define HAL_ADC_VDD_LIMIT_0        0x00
#define HAL_ADC_VDD_LIMIT_1        0x01
#define HAL_ADC_VDD_LIMIT_2        0x02
#define HAL_ADC_VDD_LIMIT_3        0x03
#define HAL_ADC_VDD_LIMIT_4        0x04
#define HAL_ADC_VDD_LIMIT_5        0x05
#define HAL_ADC_VDD_LIMIT_6        0x06
#define HAL_ADC_VDD_LIMIT_7        0x07

/* 参考电压 */
#define HAL_ADC_REF_125V          0x00    /* Internal Reference (1.25V-CC2430)(1.15V-CC2530) */
#define HAL_ADC_REF_AIN7          0x40    /* AIN7 Reference */
#define HAL_ADC_REF_AVDD          0x80    /* AVDD_SOC Pin Reference */
#define HAL_ADC_REF_DIFF          0xc0    /* AIN7,AIN6 Differential Reference */
#define HAL_ADC_REF_BITS          0xc0    /* Bits [7:6] */


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
