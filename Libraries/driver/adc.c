/**
  ******************************************************************************
  * File   : ADC/3ADCs_DMA/main.c
  * Version: V1.2.8
  * Date   : 2020-11-27
  * Brief  : Main program body
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "at32f4xx.h"
#include "at32_board.h"
#include "adc.h"
#include "DispShowStatus.h"
/** @addtogroup AT32F403A_StdPeriph_Examples
	* @{
	*/

/** @addtogroup ADC_3ADCs_DMA
	* @{
	*/

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_InitType ADC_InitStructure;
DMA_InitType DMA_InitStructure;
__IO uint16_t ADCConvertedValue[100]= {0};
__IO uint16_t ConvertedValue[10]= {0};
/* Private function prototypes -----------------------------------------------*/
void RCC_Configuration_(void);
void GPIO_Configuration(void);
OS_ERR      err;
/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
void adc_init(void)
{
	/* System clocks configuration */
	RCC_Configuration_();

	/* GPIO configuration ------------------------------------------------------*/
	GPIO_Configuration();

	/* DMA1 channel1 configuration ----------------------------------------------*/
	DMA_Reset(DMA1_Channel1);
	DMA_DefaultInitParaConfig(&DMA_InitStructure);
	DMA_InitStructure.DMA_PeripheralBaseAddr    = (uint32_t)&ADC1->RDOR;
	DMA_InitStructure.DMA_MemoryBaseAddr        = (uint32_t)&ConvertedValue;
	DMA_InitStructure.DMA_Direction             = DMA_DIR_PERIPHERALSRC;
	DMA_InitStructure.DMA_BufferSize            = 10;
	DMA_InitStructure.DMA_PeripheralInc         = DMA_PERIPHERALINC_DISABLE;
	DMA_InitStructure.DMA_MemoryInc             = DMA_MEMORYINC_ENABLE;
	DMA_InitStructure.DMA_PeripheralDataWidth   = DMA_PERIPHERALDATAWIDTH_HALFWORD;
	DMA_InitStructure.DMA_MemoryDataWidth       = DMA_MEMORYDATAWIDTH_HALFWORD;
	DMA_InitStructure.DMA_Mode                  = DMA_MODE_CIRCULAR;
	DMA_InitStructure.DMA_Priority              = DMA_PRIORITY_HIGH;
	DMA_InitStructure.DMA_MTOM                  = DMA_MEMTOMEM_DISABLE;
	DMA_Init(DMA1_Channel1, &DMA_InitStructure);
	/* Enable DMA1 channel1 */
	DMA_ChannelEnable(DMA1_Channel1, ENABLE);

	/* ADC1 configuration ------------------------------------------------------*/
	ADC_StructInit(&ADC_InitStructure);
	ADC_InitStructure.ADC_Mode              = ADC_Mode_Independent;
	ADC_InitStructure.ADC_ScanMode          = DISABLE;
	ADC_InitStructure.ADC_ContinuousMode    = ENABLE;
	ADC_InitStructure.ADC_ExternalTrig      = ADC_ExternalTrig_None;
	ADC_InitStructure.ADC_DataAlign         = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NumOfChannel      = 1;
	ADC_Init(ADC1, &ADC_InitStructure);

	/* ADC1 regular channels configuration */
	ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_28_5);

	/* Enable ADC1 DMA */
	ADC_DMACtrl(ADC1, ENABLE);

	/* Enable ADC1 */
	ADC_Ctrl(ADC1, ENABLE);

	/* Enable ADC1 reset calibration register */
	ADC_RstCalibration(ADC1);
	/* Check the end of ADC1 reset calibration register */
	while(ADC_GetResetCalibrationStatus(ADC1));

	/* Start ADC1 calibration */
	ADC_StartCalibration(ADC1);
	/* Check the end of ADC1 calibration */
	while(ADC_GetCalibrationStatus(ADC1));

	/* Start ADC1 Software Conversion */
	ADC_SoftwareStartConvCtrl(ADC1, ENABLE);
}


/**
  * @brief  Configures the different system clocks.
  * @param  None
  * @retval None
  */
void RCC_Configuration_(void)
{
	/* ADCCLK = PCLK2/4 */
	RCC_ADCCLKConfig(RCC_APB2CLK_Div4);

	/* Enable peripheral clocks ------------------------------------------------*/
	/* Enable DMA1 clocks */
	RCC_AHBPeriphClockCmd(RCC_AHBPERIPH_DMA1, ENABLE);

	/* Enable ADC1 and GPIOC clocks */
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_ADC1 | RCC_APB2PERIPH_GPIOA, ENABLE);
}

/**
  * @brief  Configures the different GPIO ports.
  * @param  None
  * @retval None
  */
void GPIO_Configuration(void)
{
	GPIO_InitType GPIO_InitStructure;

	/* Configure PA.0 (ADC Channel0) as analog input -------------------------*/
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_ANALOG;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
}


//PE接地检测
void PE_cheack_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOF, ENABLE);
	GPIO_InitType GPIO_InitStructure;
	GPIO_StructInit(&GPIO_InitStructure);
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_7 | GPIO_Pins_6;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOF, &GPIO_InitStructure);
}

extern uint8_t PE_CHEACK6[40];
extern uint8_t PE_CHEACK7[40];
extern uint8_t ZHANlian_JC[40];
extern uint8_t Short_JC[40];

//接地检测:一个一直高，一个PWM
uint8_t PE_cheack_HL(void)
{
	uint8_t PE_PWMnum[4]= {0};
	for(uint8_t abc = 0; abc<40; abc++)
	{
		if(PE_CHEACK6[abc] == 0)
		{
			PE_PWMnum[0]++;
		} else
		{
			PE_PWMnum[1]++;  //高1
		}

		if(PE_CHEACK7[abc] == 0)
		{
			PE_PWMnum[2]++;
		} else
		{
			PE_PWMnum[3]++;  //高1
		}
	}
	if(((PE_PWMnum[0]>10)&&(PE_PWMnum[3] >30))||((PE_PWMnum[1]>30)&&(PE_PWMnum[2] >10)))
	{
		show_fail(STOP_MAX);
		return TRUE;  //接地正常不显示
	}
	else
	{
		show_fail(PE_Cheack_Fail);//接地异常-显示PE故障
		return FALSE;
	}
}


//接触器是否粘连和短路检测初始化
void PC780_init(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2PERIPH_GPIOC, ENABLE);
	GPIO_InitType GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pins = GPIO_Pins_7 | GPIO_Pins_8 ;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_PP; //推挽输出
	//GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT_OD;
	GPIO_InitStructure.GPIO_MaxSpeed = GPIO_MaxSpeed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_0;  //输出LN短路检测
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure); 
	
	GPIO_InitStructure.GPIO_Pins =  GPIO_Pins_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOC, &GPIO_InitStructure);

	GPIO_ResetBits(GPIOC,GPIO_Pins_7);
	GPIO_ResetBits(GPIOC,GPIO_Pins_8); //初始化时，必须先拉低
}


//检测短路时，投切继电器拉高吸合，检测完毕后，必须尽快拉低
uint8_t short_Cheack(void)
{
	GPIO_SetBits(GPIOC,GPIO_Pins_7); 
	GPIO_SetBits(GPIOC,GPIO_Pins_8); 
	OSTimeDly(40, OS_OPT_TIME_PERIODIC, &timeerr);  //不延迟会错误
	
	uint8_t aa=0,inum=0;
	for(inum=0; inum<40; inum++)
	{
		if(Short_JC[inum] == 1)
		{
			if(aa++>25)
			{
				GPIO_ResetBits(GPIOC,GPIO_Pins_7); //必须拉低
				GPIO_ResetBits(GPIOC,GPIO_Pins_8); 
				show_fail(Out_short);
				return TRUE;  //有短路
			}
		}
	}
	GPIO_ResetBits(GPIOC,GPIO_Pins_7); //拉低
	GPIO_ResetBits(GPIOC,GPIO_Pins_8); 
	return FALSE;
}



//检测继电器是否粘连:原理是：L和N都粘连时才会发生PWM波。检测L：N闭合。检测N时：L路闭合
//LN=4   火线   LN=5 零线
uint8_t zhanlianflag[2]={0};  //0=检测L  1=检测N
uint8_t zhanlian_check(uint8_t LN)
{
	GPIO_ResetBits(GPIOC,GPIO_Pins_7);
	GPIO_ResetBits(GPIOC,GPIO_Pins_8);
	//KEY_OUT1_ON;  //N继电器闭合
	//KEY_OUT1_OFF;  //N继电器断开
	//KEY_OUT2_ON;   //L继电器闭合
	//KEY_OUT2_OFF;  //L继电器断开
	uint8_t aa=0;
	uint8_t inum=0;
	
	if(LN == 4)
	{
	  KEY_OUT1_ON;  //N继电器闭合
	}

	if(LN == 5)
	{
	  KEY_OUT2_ON;   //L继电器闭合
	}
	KEY_OUT1_ON;  //N继电器闭合
	KEY_OUT2_ON;   //L继电器闭合
	
	OSTimeDly(100, OS_OPT_TIME_PERIODIC, &err);   //中间加一个延时
	for(inum=0; inum<40; inum++)
	{
		if(ZHANlian_JC[inum] < 1)
		{
			if(aa++>5)
			{
				//搞一个全局的标志位
				if(LN == 4)
				{
				 zhanlianflag[0] = 1; //
					show_fail(Relay_adhesionL);
				}
				if(LN == 5)
				{
				 zhanlianflag[1] = 1;
					show_fail(Relay_adhesionN);
				}
			}
		}
	}
	//最后全部断开
	KEY_OUT1_OFF;  //N继电器断开
	KEY_OUT2_OFF;  //L继电器断开
	return FALSE;
}
