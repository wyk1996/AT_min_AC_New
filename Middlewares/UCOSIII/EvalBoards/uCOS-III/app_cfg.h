#ifndef  APP_CFG_MODULE_PRESENT
#define  APP_CFG_MODULE_PRESENT
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <includes.h>
/*
*********************************************************************************************************
*                                            TASK PRIORITIES
*********************************************************************************************************/
#define	XY_HY  0
#define	XY_YKC 1
#define	XY_AP  2
#define	XY_MAX 3
#define NET_YX_SELCT XY_HY
//==========上面选择YKC时  选择显示YKC/小鹤/塑云（二维码、IP端口、协议显示）=========//
#define show_YKC   0    //显示YKC
#define show_XH    1    //显示小鹤(有时候3.5kw    有的时候是7kw)
#define show_SY    2    //显示塑云
#define show_TD    3    //显示铁塔
#define show_KL    4    //显示库伦-YKC协议----注：这是库伦YKC用固定的域名登录---20403311456csh
#define show_XY_name  show_XH

//===========汇誉协议选择显示==========//
#define show_HY   0    //显示汇誉(HY)
#define show_JG   1    //显示精工(JG)
#define show_YC   2    //显示YC-上海森通益虫 //IP地址固定写死，不能修改，
#define show_HYXY_name  show_HY

//===========备注==========
#define doguser    0    //0 = 关闭看门狗      1=开启看门狗
#define USE_645    1    //0 = 使用usart3打印  1=使用外部645
#define dwin_showversion  "241029"    //0x000A1112   其中第一个00  其它三个字节0A1112版本10.17.18      //方便查看240131--年月日

#if(NET_YX_SELCT == XY_HY)
#define UPDATA_STATE     1        //1标志支持HY远程升级  0表示不支持HY远程升级
#define WLCARD_STATE     0		  //1表示有白名单卡      0表示不支持白名单卡
#elif(NET_YX_SELCT == XY_YKC)
#define FTPUPDATA_FLAG   1        //1=支持FTP远程升级  0=不支持FTP远程升级 //暂时只有YKC支持FTP,  HY只支持HTTP
#else
#define UPDATA_STATE     0        //1标志支持远程升级  0表示不支持远程升级
#define WLCARD_STATE     0		  //1表示有白名单卡    0表示不支持白名单卡
#endif

#define  APP_CFG_TASK_START_PRIO                         31u
#define	 APP_CFG_TASK_4GRECV_PRIO                        3u   //串口接收优先级要高一点，要第一时间处理串口接收发送过来得数据
#define  APP_CFG_TASK_2_PRIO                             4u
#define  APP_CFG_TASK_CARD_PRIO                   		 30u   //卡任务运行的东西很多  20220805
#define  APP_CFG_TASK_CHARGE_PRIO                   	 6u
#define  APP_CFG_TASK_DWIN_PRIO                   	   	 7u
#define	 APP_CFG_TASK_4GMAIN_PRIO                        8u
#define	 APP_CFG_TASK_4GSEND_PRIO                        10u
#define	 APP_CFG_TASK_dlt645_PRIO                        11u
/*
*********************************************************************************************************
*                                            TASK STACK SIZES
*********************************************************************************************************
*/
#define  APP_CFG_TASK_START_STK_SIZE                     80u     //80U=最大83%
#define  APP_CFG_TASK_CARD_STK_SIZE                      130u    //130U=最大74%
#define  APP_CFG_TASK_CHARGE_STK_SIZE                    100u    //100U=最大67%
#define  APP_CFG_TASK_DWIN_STK_SIZE                      230u    //230U=最大84%(费率查询时2页84%,再大就有可能内存满)
#define  APP_CFG_TASK_4GMAIN_STK_SIZE                    200u    
#define  APP_CFG_TASK_4GRECV_STK_SIZE                    220u    //220U=最大71%(4g接收费率时增加)
#define  APP_CFG_TASK_4GSEND_STK_SIZE                    320u
#if(USE_645 ==1)
#define  APP_CFG_TASK_dlt645_STK_SIZE                     70u    //70U====88%
#endif



/*
*********************************************************************************************************
*                                            TASK STACK SIZES LIMIT
*********************************************************************************************************
*/

#define  APP_CFG_TASK_START_STK_SIZE_PCT_FULL             90u
#define  APP_CFG_TASK_START_STK_SIZE_LIMIT       (APP_CFG_TASK_START_STK_SIZE     * (100u - APP_CFG_TASK_START_STK_SIZE_PCT_FULL))    / 100u
//#define  APP_CFG_TASK_BLINKY_STK_SIZE_LIMIT      (APP_CFG_TASK_BLINKY_STK_SIZE    * (100u - APP_CFG_TASK_START_STK_SIZE_PCT_FULL))    / 100u


/*
*********************************************************************************************************
*                                       TRACE / DEBUG CONFIGURATION
*********************************************************************************************************
*/

#ifndef  TRACE_LEVEL_OFF
#define  TRACE_LEVEL_OFF                0
#endif

#ifndef  TRACE_LEVEL_INFO
#define  TRACE_LEVEL_INFO               1
#endif

#ifndef  TRACE_LEVEL_DBG
#define  TRACE_LEVEL_DBG                2
#endif

#define  APP_CFG_TRACE_LEVEL             TRACE_LEVEL_OFF
#define  APP_CFG_TRACE                   printf

#define  BSP_CFG_TRACE_LEVEL             TRACE_LEVEL_OFF
#define  BSP_CFG_TRACE                   printf

#define  APP_TRACE_INFO(x)               ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_INFO)  ? (void)(APP_CFG_TRACE x) : (void)0)
#define  APP_TRACE_DBG(x)                ((APP_CFG_TRACE_LEVEL >= TRACE_LEVEL_DBG)   ? (void)(APP_CFG_TRACE x) : (void)0)

#define  BSP_TRACE_INFO(x)               ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_INFO) ? (void)(BSP_CFG_TRACE x) : (void)0)
#define  BSP_TRACE_DBG(x)                ((BSP_CFG_TRACE_LEVEL  >= TRACE_LEVEL_DBG)  ? (void)(BSP_CFG_TRACE x) : (void)0)

#endif
