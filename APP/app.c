/*
*********************************************************************************************************
*                                              EXAMPLE CODE
*
*                             (c) Copyright 2013; Micrium, Inc.; Weston, FL
*
*                   All rights reserved.  Protected by international copyright laws.
*                   Knowledge of the source code may not be used to write a similar
*                   product.  This file may only be used in accordance with a license
*                   and should not be redistributed in any way.
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*
*                                            EXAMPLE CODE
*
*                                     ST Microelectronics STM32
*                                              on the
*
*                                           STM3220G-EVAL
*                                         Evaluation Board
*
* Filename      : app.c
* Version       : V1.00
* Programmer(s) : DC
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/

#include  <app_cfg.h>
#include  <includes.h>
#include  "gui.h"
#include "DIALOG.h"
#include "lcdconf.h"

/*
*********************************************************************************************************
*                                            LOCAL DEFINES
*********************************************************************************************************
*/

/*
*********************************************************************************************************
*                                       LOCAL GLOBAL VARIABLES
*********************************************************************************************************
*/
#define VECT_TAB_OFFSET  0x8000 


                                                                /* ----------------- APPLICATION GLOBALS ------------------ */
static  OS_STK         App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE];
static  OS_STK         App_TaskEndTickStk[APP_CFG_TASK_END_TICK_STK_SIZE];
static  OS_STK         App_TaskEndProcStk[APP_CFG_TASK_END_PROC_STK_SIZE];
static  OS_STK         App_TaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE]; //added on  01.15
static  OS_STK         App_TaskKeyStk[APP_CFG_TASK_KEY_STK_SIZE];
static  OS_STK         App_TaskPLCStk[APP_CFG_TASK_PLC_STK_SIZE];
static  OS_STK         App_TaskGMPStk[APP_CFG_TASK_GMP_STK_SIZE];
static  OS_STK         App_TaskPowerStk[APP_CFG_TASK_POWER_STK_SIZE];
static  OS_STK         App_TaskPCStk[APP_CFG_TASK_PC_STK_SIZE];
static  OS_STK         App_TaskRS485Stk[APP_CFG_TASK_RS485_STK_SIZE];

/*
*********************************************************************************************************
*                                         FUNCTION PROTOTYPES
*********************************************************************************************************
*/

static  void           App_TaskStart                (void *p_arg);
static  void           App_TaskGUI                  (void *p_arg); //GUI���� added on 01.15
static  void           App_TaskGMP                  (void *p_arg);
static  void           App_TaskPower                (void *p_arg);

static  void           App_MemAlloc                 (void);
static  void           App_TaskCreate               (void);
static  void           App_EventCreate              (void);


static void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

    /* Configure the Vector Table location add offset address ------------------*/

  SCB->VTOR = FLASH_BASE | VECT_TAB_OFFSET; /* Vector Table Relocation in Internal FLASH */


  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 240;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 5;
  
  HAL_RCC_OscConfig(&RCC_OscInitStruct);
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_3);
}

/*
*********************************************************************************************************
*                                                main()
*
* Description : This is the standard entry point for C code.  It is assumed that your code will call
*               main() once you have performed all necessary initialization.
*
* Arguments   : none
*
* Returns     : none
*********************************************************************************************************
*/

int  main(void)
{
#if (OS_TASK_NAME_EN > 0)
    INT8U  err;
#endif   


    BSP_IntDisAll();                                            /* Disable all interrupts.                              */

    SystemClock_Config();
    
    CPU_Init();                                                 /* Initialize uC/CPU services.                          */
    
    OSInit();                                                   /* Initialize "uC/OS-II, The Real-Time Kernel"          */

    OSTaskCreateExt((void (*)(void *)) App_TaskStart,           /* Create the start task                                */
                    (void           *) 0,
                    (OS_STK         *)&App_TaskStartStk[APP_CFG_TASK_START_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_START_PRIO,
                    (INT16U          ) APP_CFG_TASK_START_PRIO,
                    (OS_STK         *)&App_TaskStartStk[0],
                    (INT32U          ) APP_CFG_TASK_START_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_START_PRIO, "Start", &err);
#endif

    OSStart();                                                  /* Start multitasking (i.e. give control to uC/OS-II)   */
}

/*
*********************************************************************************************************
*                                          STARTUP TASK
*
* Description : This is an example of a startup task.  As mentioned in the book's text, you MUST
*               initialize the ticker only once multitasking has started.
*
* Arguments   : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreate()'.
*
* Returns     : none
*
* Notes       : 1) The first line of code is used to prevent a compiler warning because 'p_arg' is not
*                  used.  The compiler should not generate any code for this statement.
*********************************************************************************************************
*/

WM_HWIN g_hWin_menu;
WM_HWIN g_hWin_para;
WM_HWIN g_hWin_std; //  ͨ�Ź�Լ����


WM_HWIN g_hWin_monitor;  //���
WM_HWIN g_hWin_ReadMeter; //���ó���
WM_HWIN g_hWin_WaveCarrior; // �ز���������
WM_HWIN g_hWin_DataSign; //���ݱ�ʶ
WM_HWIN g_hWin_relay;      //�м̵�ַ����
WM_HWIN g_hWin_msg;       //��Ϣ��־ 

WM_HWIN g_hWin_ReadMeterMsg;//�������Ϣ��¼

WM_HWIN g_hWin_task;//������
WM_HWIN g_hWin_about; //����

WM_HWIN g_hWin_TimeSet; //ʱ������

WM_HWIN g_hWin_mem;
WM_HWIN g_hWin_Err;

WM_HWIN g_hWin_TimeBar;  //��ҳʱ��
WM_HWIN g_hWin_Date;     //��ʾ����
WM_HWIN g_hWin_Input;    //��������С����
WM_HWIN g_hWin_speed;
WM_HWIN g_hWin_AdvanSet;  //�߼�����


//int test_multiedit;

u8 rf_int_status[8];
u8 rf_part_info[8];
u8 rf_device_state[2];
extern u8 g_get_pro[4];

int G_II;

//uc8 g_testString[] = "Hello \xe7\xa1\xae! \n";

//uc16 g_16string [] = {0x53D6, 0x56DE, 0x62,'\r\n',0};

void APP_Shutdown()
{
#if 1
    if(g_sys_control.sysUsbVol)
    {
        return;
    }

    g_sys_control.sysPowerState = SYS_POWER_SHUTDOWN;
    SYS_PWR_OFF();   //3.27
    LCD_BL_OFF();
    LCD_PWR_OFF();   //3.27
    LED_UART_OFF();
    LED_PLC_OFF();
#endif
}

void APP_Sleep(void)
{
#if 1
    g_sys_control.sysPowerState = SYS_POWER_IDLE;
    LCD_BL_OFF();
    LED_On(LED_PWR);
#endif    
}

void APP_Wakeup()
{
#if 0
    g_sys_control.sysPowerState = SYS_POWER_WAKEUP;
    //SYS_PWR_OFF();
    //LCD_BL_ON();
    //LCD_PWR_ON();

    LCD_X_DisplayDriver(0, LCD_X_INITCONTROLLER, 0);
    //LED_UART_OFF();
    //LED_PLC_OFF();
#endif    
}

#if (EWARM_OPTIMIZATION_EN > 0u)
#pragma optimize = low
#endif
void APP_StartButtonTest()
{
    u32 keyCnt = 0;;
    while(1)
    {
        OSTimeDly(20);
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
        {
            keyCnt++;
            if(keyCnt > 80)
            {
                SYS_PWR_ON();
                //g_sys_control.power_stat = SYS_POWER_ON;
                OSTimeDly(20);
                return;                
            }
        }
        else
        {
            APP_Shutdown();
            //g_sys_control.power_stat = SYS_POWER_OFF;
            keyCnt = 0;

            if(GET_USB_VOL() == 0)
            {
                return;
            }
            else
            {
                DEBUG_PRINT(("Power error!\n")); //������
                
                while(1)
                {
                    APP_Shutdown();
                }
            }
        }
    }
}

/*
*********************************************************************************************************
*                                             App_TaskStart()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskStart()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskStart (void *p_arg)
{
    u8 i = 0, n;
    u32 count = 0;


    (void)p_arg; 
    
    //INT8U err;  //�����õ�
    
    DEV_Init();

    BSP_Init();                                                 /* Init BSP fncts.                                          */

    Mem_Init();                                                 /* Initialize Memory managment                              */

    BSP_CPU_TickInit();                                         /* Start Tick Initialization                                */

#if (OS_TASK_STAT_EN > 0)
    OSStatInit();                                               /* Determine CPU capacity                                   */
#endif

    APP_StartButtonTest();
    
    App_EventCreate();                                          /* Create Application Events                                */

    App_TaskCreate();                                           /* Create application Tasks                                 */ 
   
  	GUI_Init();                                                 /* Init the STemWin GUI Library */

	KEY_Init();

    End_Init();

    RF_Init();
    
    BSP_IWDG_Init();  //4.1
    
    DEBUG_PRINT(("OS Tasks Run!\n"));

    //FatFs_Test();
    //BSP_BEEP();
    g_sys_control.led_count = 2;
    while (DEF_TRUE) { 
        /* Task body, always written as an infinite loop.           */

        if(i++ >= 10)
        {
            i = 0;
            RTC_ReadTime(g_rtc_time); 
        }

        for(n = 0; n < 32; n++)
        {
            if((0x01<<n) & g_sys_control.procTask)
            {
                switch(n)
                {
                case 0:
                    MMD_Format_Disk();
                    SYS_DEL_TASK(SYS_TASK_FORMAT_DISK);
                    break;
                }
            }
        }

        if(g_sys_control.led_count)
        {
            g_sys_control.led_count--;

            if(!g_sys_control.led_count)
            {
                LED_KEY_OFF();
            }
        }
        
        if(!(count % 5))
        {
            clr_wdt();
        }

        //g_sys_control.pwrValue = BSP_ADC_ReadPwr();
        //EDIT_SetFloatValue(MMD_GetVoltage(),g_sys_control.pwrValue

        count++;

        OSTimeDlyHMSM(0, 0, 0, 100);
    }
}

/*
*********************************************************************************************************
*                                             App_TaskGUI()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskGUI()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskGUI (void *p_arg)
{
    (void)p_arg;
    WM_HWIN hItem;
    unsigned char timebuf[10];
    unsigned char timebuf_nosec[6];
    unsigned char timebuf_date[11];
    int i = 0;
    int j = 0;//��ȡ��ѹֵ����
    u32 val = 0; //��ѹ
   // int countdown = 240; //��ȡ�ز��ڵ㵹��ʱ240s
    
#if OS_CRITICAL_METHOD == 3u
        OS_CPU_SR  cpu_sr = 0u;
#endif  

    g_hWin_menu = CreatePDA_IconMenu();
    WM_ShowWindow(g_hWin_menu);
   
    g_hWin_msg=CreateMessage();
    WM_HideWindow(g_hWin_msg);
    WM_SetFocus(g_hWin_menu);

    while (DEF_TRUE) {                                          /* Task body, always written as an infinite loop.           */
        GUI_Exec();

        if(g_hWin_monitor > 0)
        {
            if(g_sys_control.sysCtdFlag == COUNTDOWN_ON)
            {
                if(i++ >= 10)
                {
                    i = 0;
                    hItem = MNT_GetTime();
                    EDIT_SetValue(hItem,g_sys_control.sysCtdVal--);
                    if(0 == g_sys_control.sysCtdVal)
                    {
                        g_sys_control.sysCtdFlag == COUNTDOWN_OFF;//���ز��ڵ��ʱ���
                        EDIT_SetValue(hItem,0);
                        hItem = MNT_GetReadNope();
                        BUTTON_SetText(hItem ,ReadNope);
                    }
                }
            }
        }

        if(j < 10)
        {
            val += BSP_ADC_ReadPwr();
            j++;
            if(GPIO_PIN_SET == BSP_Charg())
            {
                TSK_Battery_Charge(j);
            }
            
        }
        else if(j >= 10)
        {
            //G_II = BSP_Charg();
            g_sys_control.pwrValue = val/10;
            if(GPIO_PIN_RESET == BSP_Charg())
            {
                Battery_State(g_sys_control.pwrValue);
            }
            j = 0;
            val = 0;
        }
        memcpy(timebuf,RTC2Text(),10);
        TEXT_SetText(g_hWin_TimeBar,timebuf);
        //memcpy(timebuf_nosec,RTC2Text_NoSec(),6);
        //TEXT_SetText(g_hWin_TimeBar,timebuf_nosec);
        memcpy(timebuf_date,RTC2Text_Date(),11);
        TEXT_SetText(g_hWin_Date,timebuf_date);

        //hItem = WM_GetDialogItem(pMsg->hWin,ID_EDIT_2);
        if(g_hWin_mem>0)
        {
            EDIT_SetFloatValue(MMD_GetVoltage(),((g_sys_control.pwrValue*3.3)/2048));
        }
        
        OSTimeDlyHMSM(0, 0, 0, 100);     
    }
}

/*
*********************************************************************************************************
*                                             App_TaskPower()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskPower()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskPower (void *p_arg)
{
    (void)p_arg;
    
    LED_Off(LED_PWR);
    
    while (DEF_TRUE) {
        if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
        {
            OSTimeDlyHMSM(0, 0, 0, 30);

            if(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
            {
                while(GPIO_PIN_RESET == HAL_GPIO_ReadPin(GPIOG, GPIO_PIN_11))
                {
                    OSTimeDlyHMSM(0, 0, 0, 10);
                }
                
                LCD_BL_OFF();
                LED_On(LED_PWR);
            }
        }

        if(GPIO_PIN_RESET == GET_USB_VOL())
        {
            g_sys_control.sysUsbVol = 1;
        }
        else
        {
            g_sys_control.sysUsbVol = 0;
        }        

        g_sys_control.shutdownTimeout++;
        if(g_sys_control.shutdownTimeout > ((g_sys_register_para.scrTimeout + 120) * 100))
        {
            if((PLC_CMD_TYPE_NODE == g_send_para_pkg.cmdType) ||
               (PLC_CMD_TYPE_R2L == g_send_para_pkg.cmdType))
            {
                LCD_BL_OFF();
            }
            else
            {
                APP_Shutdown();
            }
            
            g_sys_control.shutdownTimeout = 0;
        }
        
        g_sys_control.sleepTimeout++;
        if(g_sys_control.sleepTimeout > ((g_sys_register_para.scrTimeout + 30) * 100))
        {
            APP_Sleep();
            
            g_sys_control.sleepTimeout = 0;
        }        

        OSTimeDly(10);
    }
}

/*
*********************************************************************************************************
*                                             App_TaskGMP()
*
* Description : This task monitors the state of the push buttons and passes messages to AppTaskUserIF()
*
* Argument(s) : p_arg   is the argument passed to 'App_TaskGMP()' by 'OSTaskCreateExt()'.
*
* Return(s)  : none.
*
* Caller(s)  : This is a task.
*
* Note(s)    : none.
*********************************************************************************************************
*/
static  void  App_TaskGMP (void *p_arg)
{
    INT8U err;

    u32   tm;

    WM_HWIN  wh;
    
    (void)p_arg;
    g_sys_control.numMultiedit = 0; //������Ϣ�����ı��Ĵ�ӡ����
    while (DEF_TRUE) {  
        /* Task body, always written as an infinite loop.           */
        if((g_sys_control.guiState == GUI_PLC_MSG_TEST)
            ||(g_sys_control.guiState == GUI_PLC_MSG_READ)
            ||(g_sys_control.guiState == GUI_PLC_MSG_LISTING)
            ||(g_sys_control.guiState == GUI_PLC_MSG_FREQ))
        {
            OSMboxPend(g_sys_control.upMb, 10, &err);        
            if(OS_ERR_NONE == err)
            {
                g_sys_control.numMultiedit++;
                GUI_Msg_Proc();
            }
            else if(OS_ERR_TIMEOUT == err)
            {
                wh = GUI_Get_PROGBAR();
                if(wh != WM_HWIN_NULL)
                {
                    
                    PROGBAR_SetValue(wh, g_sys_control.testProgBarVal);
                    if(g_sys_control.testProgBarVal < 90)
                    {
                       PROGBAR_SetBarColor(wh, 0, GUI_GREEN);
                       g_sys_control.testProgBarVal += 1;
                    }
                }
            }
        }
        else
        {
            OSTimeDly(10);
        }
    }
}

/*
*********************************************************************************************************
*                                      App_EventCreate()
*
* Description : Create the application Events
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TasStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_EventCreate (void)
{    
    g_sem_end = OSSemCreate(0);
    g_sem_plc = OSSemCreate(0);
    g_sem_rf = OSSemCreate(0);
    g_sem_pc = OSSemCreate(0);
    g_sem_rs485 = OSSemCreate(0);
	g_key_control.key_sem = OSSemCreate(0);    
    
    g_sys_control.downMb = OSMboxCreate(NULL); /*������Ϣ�����������͵��Բ����Ľṹ��*/
    g_sys_control.upMb = OSMboxCreate(NULL); /*������Ϣ�����������͵��Բ����Ľṹ��*/        
}


/*
*********************************************************************************************************
*                                      App_TaskCreate()
*
* Description : Create the application tasks.
*
* Argument(s) : none.
*
* Return(s)   : none.
*
* Caller(s)   : App_TasStart()
*
* Note(s)     : none.
*********************************************************************************************************
*/
static  void  App_TaskCreate (void)
{
    INT8U  err;

    
    OSTaskCreateExt((void (*)(void *)) App_TaskGUI,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskGUIStk[APP_CFG_TASK_GUI_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_GUI_PRIO,
                    (INT16U          ) APP_CFG_TASK_GUI_PRIO,
                    (OS_STK         *)&App_TaskGUIStk[0],
                    (INT32U          ) APP_CFG_TASK_GUI_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_GUI_PRIO, "GUI", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskGMP,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskGMPStk[APP_CFG_TASK_GMP_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_GMP_PRIO,
                    (INT16U          ) APP_CFG_TASK_GMP_PRIO,
                    (OS_STK         *)&App_TaskGMPStk[0],
                    (INT32U          ) APP_CFG_TASK_GMP_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));
    
#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_GMP_PRIO, "GMP", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskKey,              
                    (void           *) 0,
                    (OS_STK         *)&App_TaskKeyStk[APP_CFG_TASK_KEY_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_KEY_PRIO,
                    (INT16U          ) APP_CFG_TASK_KEY_PRIO,
                    (OS_STK         *)&App_TaskKeyStk[0],
                    (INT32U          ) APP_CFG_TASK_KEY_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_KEY_PRIO, "Key", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskEndTick,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskEndTickStk[APP_CFG_TASK_END_TICK_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_END_TICK_PRIO,
                    (INT16U          ) APP_CFG_TASK_END_TICK_PRIO,
                    (OS_STK         *)&App_TaskEndTickStk[0],
                    (INT32U          ) APP_CFG_TASK_END_TICK_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_END_TICK_PRIO, "EndTick", &err);
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskEndProc,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskEndProcStk[APP_CFG_TASK_END_PROC_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_END_PROC_PRIO,
                    (INT16U          ) APP_CFG_TASK_END_PROC_PRIO,
                    (OS_STK         *)&App_TaskEndProcStk[0],
                    (INT32U          ) APP_CFG_TASK_END_PROC_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_END_PROC_PRIO, "EndProc", &err);      
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskPLC,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskPLCStk[APP_CFG_TASK_PLC_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_PLC_PRIO,
                    (INT16U          ) APP_CFG_TASK_PLC_PRIO,
                    (OS_STK         *)&App_TaskPLCStk[0],
                    (INT32U          ) APP_CFG_TASK_PLC_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_PLC_PRIO, "PLC", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskPower,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskPowerStk[APP_CFG_TASK_POWER_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_POWER_PRIO,
                    (INT16U          ) APP_CFG_TASK_POWER_PRIO,
                    (OS_STK         *)&App_TaskPowerStk[0],
                    (INT32U          ) APP_CFG_TASK_POWER_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_POWER_PRIO, "Power", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskPC,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskPCStk[APP_CFG_TASK_PC_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_PC_PRIO,
                    (INT16U          ) APP_CFG_TASK_PC_PRIO,
                    (OS_STK         *)&App_TaskPCStk[0],
                    (INT32U          ) APP_CFG_TASK_PC_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_PC_PRIO, "PC", &err);    
#endif

    OSTaskCreateExt((void (*)(void *)) App_TaskRS485,
                    (void           *) 0,
                    (OS_STK         *)&App_TaskRS485Stk[APP_CFG_TASK_RS485_STK_SIZE - 1],
                    (INT8U           ) APP_CFG_TASK_RS485_PRIO,
                    (INT16U          ) APP_CFG_TASK_RS485_PRIO,
                    (OS_STK         *)&App_TaskRS485Stk[0],
                    (INT32U          ) APP_CFG_TASK_RS485_STK_SIZE,
                    (void           *) 0,
                    (INT16U          )(OS_TASK_OPT_STK_CHK | OS_TASK_OPT_STK_CLR));

#if (OS_TASK_NAME_EN > 0)
    OSTaskNameSet(APP_CFG_TASK_PC_PRIO, "RS485", &err);    
#endif
}

