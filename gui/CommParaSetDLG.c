/*********************************************************************
*                                                                    *
*                SEGGER Microcontroller GmbH & Co. KG                *
*        Solutions for real time microcontroller applications        *
*                                                                    *
**********************************************************************
*                                                                    *
* C-file generated by:                                               *
*                                                                    *
*        GUI_Builder for emWin version 5.22                          *
*        Compiled Jul  4 2013, 15:16:01                              *
*        (c) 2013 Segger Microcontroller GmbH & Co. KG               *
*                                                                    *
**********************************************************************
*                                                                    *
*        Internet: www.segger.com  Support: support@segger.com       *
*                                                                    *
**********************************************************************
*/
#include "DIALOG.h"
#include "includes.h"
//#include "phrase.h"

/*********************************************************************
*
*       Defines
*
**********************************************************************
*/

#define ID_WINDOW_0       (GUI_ID_USER + 0x6C)


#define ID_TEXT_0         (GUI_ID_USER + 0x6D)
#define ID_TEXT_1         (GUI_ID_USER + 0x6E)
#define ID_TEXT_2         (GUI_ID_USER + 0x6F)
#define ID_TEXT_3         (GUI_ID_USER + 0x70)
#define ID_TEXT_4         (GUI_ID_USER + 0x71)
#define ID_TEXT_5         (GUI_ID_USER + 0x72)
#define ID_TEXT_6         (GUI_ID_USER + 0x73)
#define ID_TEXT_7         (GUI_ID_USER + 0x74)
#define ID_TEXT_8         (GUI_ID_USER + 0x75)

#define ID_EDIT_0     (GUI_ID_USER + 0x76)
#define ID_EDIT_1     (GUI_ID_USER + 0x77)
#define ID_EDIT_2     (GUI_ID_USER + 0x78)
#define ID_EDIT_3     (GUI_ID_USER + 0x79)
#define ID_EDIT_4     (GUI_ID_USER + 0x7A)
#define ID_EDIT_5     (GUI_ID_USER + 0x7B)
#define ID_EDIT_6     (GUI_ID_USER + 0x7C)
#define ID_EDIT_7     (GUI_ID_USER + 0x7D)

#define ID_BUTTON_0   (GUI_ID_USER + 0x80)
#define ID_BUTTON_1   (GUI_ID_USER + 0x81)
#define ID_BUTTON_2   (GUI_ID_USER + 0x82)
#define ID_BUTTON_3   (GUI_ID_USER + 0x83)
#define ID_BUTTON_4   (GUI_ID_USER + 0x84)

#define ID_LISTBOX_0      (GUI_ID_USER + 0x85)

//char Edit0_Text[10]; //edit文本字符串
//char Edit1_Text[10];
//char Edit2_Text[10];

static int key_press_cnt = 0;//按键次数，按上下键盘的时候对应不同的button

/*********************************************************************
*
*       _aDialogCreate
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {

  { WINDOW_CreateIndirect,  "cps_new",    ID_WINDOW_0, 0,   0,   240, 295, 0, 0x0, 0 },
  { TEXT_CreateIndirect,    Protocal,     ID_TEXT_0,   8,   42,  110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect,    Channel,      ID_TEXT_1,   8,   69,  110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect,    BaudRate,     ID_TEXT_2,   8,   99,  110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect,    StopBit,      ID_TEXT_3,   8,   126, 110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect,    PassWord,     ID_TEXT_4,   8,   152, 110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect,    OperateCode,  ID_TEXT_5,   8,   180, 120, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect,    RevDataDelay, ID_TEXT_6,   8,   208, 110, 20, 0, 0x0, 0 },
  { TEXT_CreateIndirect,    BetweenAct,   ID_TEXT_7,   8,   236, 110, 20, 0, 0x0, 0 },

  { BUTTON_CreateIndirect,  TimeSet,      ID_BUTTON_0, 8,   10,  92,  25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,  AdvanceSet,   ID_BUTTON_1, 128, 10,  103,  25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,  Save,         ID_BUTTON_2, 9,   264, 55,  25, 0, 0x0, 0 },
  { BUTTON_CreateIndirect,  Quit,         ID_BUTTON_3, 172, 263, 55,  25, 0, 0x0, 0 }, 

  { EDIT_CreateIndirect,    NULL,         ID_EDIT_0,   128, 42,  103, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect,    NULL,         ID_EDIT_1,   128, 67,  103, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect,    NULL,         ID_EDIT_2,   128, 96,  103, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect,    NULL,         ID_EDIT_3,   128, 122, 103, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect,    NULL,         ID_EDIT_4,   128, 150, 103, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect,    NULL,         ID_EDIT_5,   128, 176, 103, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect,    NULL,         ID_EDIT_6,   128, 204, 103, 20, 0, 0x64, 0 },
  { EDIT_CreateIndirect,    NULL,         ID_EDIT_7,   128, 231, 103, 20, 0, 0x64, 0 },

};




//向上选择
void CPS_SelEdt_Up(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    if(key_press_cnt == 0)
    {
        hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_7);
        WM_SetFocus(hItem);
        
        key_press_cnt = 7;
    }
    else
    {
        key_press_cnt--;
        hItem=WM_GetDialogItem(pMsg->hWin,(ID_EDIT_0+key_press_cnt));
        WM_SetFocus(hItem);
    }
}



//向下选择
void CPS_SelEdt_Down(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    if(key_press_cnt == 7)
    {
        hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_0);
        WM_SetFocus(hItem);

        key_press_cnt = 0;
    }
    else
    {
        key_press_cnt++;
        hItem=WM_GetDialogItem(pMsg->hWin,(ID_EDIT_0+key_press_cnt));
        WM_SetFocus(hItem);
    }
}




void CPS_Color_Change(void)
{
    WM_HWIN hItem;
    int i;
    for(i=0;i<9;i++)
    {
        hItem=WM_GetDialogItem(g_hWin_para,ID_EDIT_0+i);
        if(WM_HasFocus(hItem)==1)
        {
            EDIT_SetBkColor(hItem,0,GUI_GREEN);
        }
        else if(WM_HasFocus(hItem)==0)
        {
            EDIT_SetBkColor(hItem,0,0xC0C0C0);
        }

    }
}
/*********************************************
*
*   打开一个新的页面，再返回到当前页面的时候
*   可以聚焦到之前的的控件上
**********************************************/
void CPS_SetFocus(void)
{
    WM_HWIN hItem;
    hItem = WM_GetDialogItem(g_hWin_para,(ID_EDIT_0 + key_press_cnt));
    WM_SetFocus(hItem);
}


WM_HWIN CPS_Set_Proto(void)
{
    //WM_HWIN hItem;
    return  WM_GetDialogItem(g_hWin_para,ID_EDIT_0);
    //EDIT_SetText(hItem,pText);
}

WM_HWIN CPS_Set_Channel(void)
{
    return  WM_GetDialogItem(g_hWin_para,ID_EDIT_1);
}

WM_HWIN CPS_Set_BaudRate(void)
{
    return  WM_GetDialogItem(g_hWin_para,ID_EDIT_2);
}


WM_HWIN CPS_Set_StopBit(void)
{
   return  WM_GetDialogItem(g_hWin_para,ID_EDIT_3);
}

WM_HWIN CPS_GetPwd(void)
{
   return  WM_GetDialogItem(g_hWin_para,ID_EDIT_4);
}


WM_HWIN CPS_GetOperator(void)
{
    return  WM_GetDialogItem(g_hWin_para,ID_EDIT_5);
}

WM_HWIN CPS_Set_RevDelay(void)
{
    return  WM_GetDialogItem(g_hWin_para,ID_EDIT_6);
}

WM_HWIN CPS_Set_BetweenAct(void)
{
    return  WM_GetDialogItem(g_hWin_para,ID_EDIT_7);
}



//初始化对话框
static void _init_dialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int i;
    char tmpBuf[30];
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_0);
    BUTTON_SetBkColor(hItem,0,GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_1);
    BUTTON_SetBkColor(hItem,0,GUI_CYAN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_2);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    hItem = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_3);
    BUTTON_SetBkColor(hItem,0,GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);

    for(i=0;i<8;i++)
    {
        hItem=WM_GetDialogItem(pMsg->hWin,ID_EDIT_0+i);
        WM_DisableWindow(hItem);
    }
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    switch(g_rom_para.plcProtocol)
    {
        case DL_T_07:
            EDIT_SetText(hItem, "DL645-07");
            break;
        case DL_T_97:
            EDIT_SetText(hItem, "DL645-97");
    }
    

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_1);
    switch(g_rom_para.channel)
    {
        case CHANNEL_IR:
            EDIT_SetText(hItem,Infrared);
            break;
          
        case CHANNEL_PLC:
            EDIT_SetText(hItem,WaveCarrier);
            break;
         
        case CHANNEL_WIRELESS:
            EDIT_SetText(hItem,ChannelWireless);
            break;
    }
    //EDIT_SetBkColor(hItem,0,GUI_GREEN);
    

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_2);
    //EDIT_SetText(hItem, "115200");
    switch(g_rom_para.baudrate)
    {
        case BAUD_RATE_1200:
            EDIT_SetText(hItem,"1200");
            break;
        case BAUD_RATE_1500:
            EDIT_SetText(hItem,"1500");
            break;
        case BAUD_RATE_2400:
            EDIT_SetText(hItem,"2400");
            break;
        case BAUD_RATE_4800:
            EDIT_SetText(hItem,"4800");
            break;
        case BAUD_RATE_9600:
            EDIT_SetText(hItem,"9600");
            break;
    }
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_3);
    //EDIT_SetText(hItem, "stop");
    switch(g_rom_para.stopbit)
    {
        case ONE_STOPBIT:
            EDIT_SetText(hItem, "1");
            break;
        case TWO_STOPBIT:
            EDIT_SetText(hItem, "2");
            break;     
    }

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_4);
    //int_to_char(g_rom_para.scrTimeout,tmpBuf,10);
    EDIT_SetText(hItem, "123456");

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_5);
    //int_to_char(g_rom_para.meterPassword,tmpBuf,10);
    EDIT_SetText(hItem, "00000000");

    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_6);
    int_to_char(g_rom_para.recvDelayTime,tmpBuf,10);
    EDIT_SetText(hItem, tmpBuf); 
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_7);
    int_to_char(g_rom_para.execInterval,tmpBuf,10);
    EDIT_SetText(hItem, tmpBuf);

    //hItem = WM_GetDialogItem(pMsg->hWin,ID_EDIT_0);
    //WM_SetFocus(hItem);

}








/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  WM_HWIN hListBox;
  int     NCode;
  int     Id;
  int     sel_num;//获取到选择的规约参数保存到发送参数的包里
  int     key_num;
  //int     i;
  
  key_num=((WM_KEY_INFO *)(pMsg->Data.p))->Key;
  
  switch (pMsg->MsgId) {
  case WM_INIT_DIALOG:
    GUI_UC_SetEncodeUTF8();
     _init_dialog(pMsg);
     //CPS_Color_Change(pMsg);
    break;
     
  case WM_KEY:
      Id  = WM_GetId(pMsg->hWinSrc);
      if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) == 1)
      {
          switch(((WM_KEY_INFO*)(pMsg->Data.p))->Key)
          {
               
               case GUI_KEY_YELLOW:
                   WM_DeleteWindow(g_hWin_para);
                   g_hWin_para=HBWIN_NULL;
                   WM_SetFocus(g_hWin_menu);
                   WM_ShowWindow(g_hWin_TimeBar);
                   WM_ShowWindow(g_hWin_Date);
                   key_press_cnt=0;
                   break;
                   
               case GUI_KEY_GREEN:  /*  保存数据  */
                   //para_store(pMsg);
                   TSK_Set_Protocol_Text();
                   DEV_Parameters_Write();
                   break;
                   
               case GUI_KEY_F1:
                   g_hWin_TimeSet=CreateTimeSet();
                   WM_SetFocus(g_hWin_TimeSet);
                   hItem = TMS_Get_Year();
                   WM_SetFocus(hItem);
                   TMS_Color_Change();
                   break;
                   
               case GUI_KEY_F2:
                   g_hWin_AdvanSet= CreateAdvanceSet(g_hWin_para);
                   WM_SetFocus(g_hWin_AdvanSet);
                   hItem = ADS_GetSrcOutTime();
                   ADS_SetFocus();
                   ADS_Color_Change();
                   break;
#if 0
               case GUI_KEY_TAB:
                      CPS_Color_Change();
                      break;
#endif
               case GUI_KEY_UP:
                   CPS_SelEdt_Up(pMsg);
                   CPS_Color_Change();
                   break;
                  
               case GUI_KEY_DOWN:
                   CPS_SelEdt_Down(pMsg);
                   CPS_Color_Change();
                   break;
               case GUI_KEY_ENTER:
                   switch(Id)
                   {
                       case ID_EDIT_0:
                           g_sys_ctrl.selectWidget=LISTBOX_PROTOCOL;
                           g_hWin_Input=Create_ListBox_Set(g_hWin_para);                   
                           WM_SetFocus(g_hWin_Input);
                           break;
                           
                       case ID_EDIT_1:
                           g_sys_ctrl.selectWidget=LISTBOX_CHANNEL;
                           g_hWin_Input=Create_ListBox_Set(g_hWin_para);
                           WM_SetFocus(g_hWin_Input);
                           break;
                           
                       case ID_EDIT_2:
                           g_sys_ctrl.selectWidget=LISTBOX_BAUDRATE;
                           g_hWin_Input=Create_ListBox_Set(g_hWin_para);
                           WM_SetFocus(g_hWin_Input);
                           break;
                           
                       case ID_EDIT_3:
                           g_sys_ctrl.selectWidget=LISTBOX_STOPBIT;
                           g_hWin_Input=Create_ListBox_Set(g_hWin_para);
                           WM_SetFocus(g_hWin_Input);
                           break;
                           
                       case ID_EDIT_4:
                           g_sys_ctrl.selectWidget = EDIT_PASSWORD;
                           g_hWin_Input=Create_Edit_Set(g_hWin_AdvanSet);
                           WM_SetFocus(g_hWin_Input);  
                           break;
                           
                       case ID_EDIT_5:
                           g_sys_ctrl.selectWidget = EDIT_OPCODE;
                           g_hWin_Input=Create_Edit_Set(g_hWin_para);
                           WM_SetFocus(g_hWin_Input);
                           break;
                           
                       case ID_EDIT_6:
                           g_sys_ctrl.selectWidget=EDIT_RECV_DELAY;
                           g_hWin_Input=Create_Edit_Set(g_hWin_para);
                           WM_SetFocus(g_hWin_Input);
                           break;
                           
                       case ID_EDIT_7:
                           g_sys_ctrl.selectWidget=EDIT_BETWEEN_ACT;
                           g_hWin_Input=Create_Edit_Set(g_hWin_para);
                           WM_SetFocus(g_hWin_Input);
                           break;                
                     }  
                     break;
               default: break;                                                                  
          }
      }
    break;
/********************************************************************/
    
  case WM_MSG_CLOSE:
    WM_DeleteWindow(g_hWin_para);
	break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


/*********************************************************************
*
*       CreateCommParaSet
*/
WM_HWIN CreateCommParaSet(void);
WM_HWIN CreateCommParaSet(void) {
  WM_HWIN hWin;
  WM_HWIN hItem;
  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_menu, 0, 0);
  //hItem = WM_GetDialogItem(hWin,ID_EDIT_0);
  //WM_SetFocus(hItem);
  return hWin;
}



/*************************** End of file ****************************/




