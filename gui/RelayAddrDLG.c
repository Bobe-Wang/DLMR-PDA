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

// USER START (Optionally insert additional includes)
// USER END

#include "DIALOG.h"
#include "includes.h"

/************************************************
*
*       
*
*       路由表，中继页面
*
*
*************************************************/

#define ID_WINDOW_0 (GUI_ID_USER + 0xA4)
#define ID_LISTVIEW_0 (GUI_ID_USER + 0xA5)

#define ID_TEXT_0 (GUI_ID_USER + 0xA6)
#define ID_EDIT_0 (GUI_ID_USER + 0xA7)

#define ID_BUTTON_0 (GUI_ID_USER + 0xA8)
#define ID_BUTTON_1 (GUI_ID_USER + 0xA9)
#define ID_BUTTON_2 (GUI_ID_USER + 0xAB)
#define ID_BUTTON_3 (GUI_ID_USER + 0xAC)
#define ID_BUTTON_4 (GUI_ID_USER + 0xAD)
#define ID_BUTTON_5 (GUI_ID_USER + 0xAE)




/*********************************************************************
*
*       Static data
*
**********************************************************************
*/





static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
  { WINDOW_CreateIndirect,   NULL,       ID_WINDOW_0,   0,   0,   240, 295, 0, 0x0,  0 },
  { EDIT_CreateIndirect,     NULL,       ID_EDIT_0,     86,  15,  144, 20,  0, 0x64, 0 },  
  { BUTTON_CreateIndirect,   StartRoute, ID_BUTTON_0,   8,   43,  95,  28,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Add,        ID_BUTTON_1,   135, 44,  95,  28,  0, 0x0,  0 },  
  { TEXT_CreateIndirect,     _RelayAddr, ID_TEXT_0,     5,   18,  76,  20,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Delete,     ID_BUTTON_2,   8,   82,  95,  28,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Modify,     ID_BUTTON_5,   135, 82,  95,  28,  0, 0x0,  0 },
  { LISTVIEW_CreateIndirect, NULL,       ID_LISTVIEW_0, 8,   116, 224, 144, 0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Save,       ID_BUTTON_3,   8,   264, 55,  25,  0, 0x0,  0 },
  { BUTTON_CreateIndirect,   Quit,       ID_BUTTON_4,   167, 264, 55,  25,  0, 0x0,  0 },
  
};

/*********************************************************************
*
*       Static code
*
**********************************************************************
*/



/*********************************************************************
*
*       Static code
*
**********************************************************************
*/



WM_HWIN RLY_GetAddr(void)
{
    return WM_GetDialogItem(g_hWin_relay,ID_EDIT_0);
}

WM_HWIN RLY_GetListAddr(void)
{
    return WM_GetDialogItem(g_hWin_relay,ID_LISTVIEW_0);
}




/**************************************************************
*
*                初始化对话框
*
***************************************************************/
static void _init_dialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int i=0;
    int NumItems = 0;
    char RowNum[2]; //缓存行号，用来初始化listview
    
    hItem = WM_GetDialogItem(pMsg->hWin, ID_EDIT_0);
    EDIT_SetText(hItem, "0");
    WM_DisableWindow(hItem);
    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    if(PLC_ROUTE_ON == g_sys_ctrl.sysUseRoute)
    {
        BUTTON_SetText(hItem, AlreadyStart);
    }
    else
    {
        BUTTON_SetText(hItem, AlreadyClosed);
    }

    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_1);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_2);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
     
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_3);
    BUTTON_SetBkColor(hItem,0,GUI_GREEN);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_4);
    BUTTON_SetBkColor(hItem,0,GUI_YELLOW);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    
    hItem=WM_GetDialogItem(pMsg->hWin,ID_BUTTON_5);
    WIDGET_AndState(hItem,WIDGET_STATE_FOCUSSABLE);
    //
    // Initialization of 'Listview'
    //
    hItem = WM_GetDialogItem(pMsg->hWin, ID_LISTVIEW_0);
    LISTVIEW_AddColumn(hItem, 80, RelayNum, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 144, _RelayAddr, GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_SetGridVis(hItem, 0);
    LISTVIEW_SetHeaderHeight(hItem, 25);
    LISTVIEW_SetAutoScrollV(hItem,1);

    for(i = 0; i < g_sys_ctrl.sysAddrLevel; i++)
    {
        LISTVIEW_AddRow(hItem, NULL);
        LISTVIEW_SetItemText(hItem, 0, i, int_to_char(i, RowNum, 10));
        LISTVIEW_SetItemText(hItem, 1, i, (const char*)GUI_hex2MeterAddrStr(g_gui_para.relayAddr[i],6));
    }
}

//获取各个中继地址
void RLY_GetParaAddr(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    u8 *pBuffer;
    //int RowNum; 
    int i ;
    
    hItem = WM_GetDialogItem(pMsg->hWin ,ID_LISTVIEW_0);
    g_sys_ctrl.sysAddrLevel = LISTVIEW_GetNumRows(hItem);

    for(i = 0; i < g_sys_ctrl.sysAddrLevel; i++)
    {
        LISTVIEW_GetItemText(hItem, 1, i, pBuffer, 13);
        GUI_GetMeterAddr(pBuffer,g_gui_para.relayAddr[i]);
    }    
}


//修改中继地址
static void RLY_Modify_Addr(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    char tempBuf[12];
    
    g_sys_ctrl.selectWidget = MODIFY_RELAY_ADDR;
    g_hWin_Input = Create_Edit_Set(g_hWin_relay);
    WM_SetFocus(g_hWin_Input);
}

//增加中继地址，要补全零
static void RLY_AddAddr(WM_MESSAGE * pMsg)
{
    WM_HWIN hItem;
    int RowCount;//listview全部行数
    char tempBuf[16];
        
    hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0);
    RowCount=LISTVIEW_GetNumRows(hItem);
    LISTVIEW_AddRow(hItem, NULL);
    //放到input_Widget中处理增加的地址，这里只增加第一列的序号
    LISTVIEW_SetItemText(hItem, 1, RowCount, "\0");//第1列也就是，中继地址列
    int_to_char((RowCount-1),tempBuf,10); 
    LISTVIEW_SetItemText(hItem, 0, RowCount, tempBuf);
}

//删除中继地址
//删除后，立即重新获取各个中继地址到g_gui_para中，然后再对删除后的地址排序
static void Del_Relay_Addr(WM_MESSAGE *pMsg)
{
    WM_HWIN hItem;
    int     RowNum;//获取listview单元格行序号
    char pTextRow[2];
    int     i;
    ButtonBlink(pMsg,ID_BUTTON_2);
    hItem=WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0);
    RowNum=LISTVIEW_GetSel(hItem);
    LISTVIEW_DeleteRow(hItem,RowNum);
    LISTVIEW_SetSel(hItem,0);
    
    RLY_GetParaAddr(pMsg);  
    for(i = 0; i < g_sys_ctrl.sysAddrLevel; i++)
    {
        //LISTVIEW_AddRow(hItem, NULL);
        LISTVIEW_SetItemText(hItem, 0, i, int_to_char(i, pTextRow, 10));
        LISTVIEW_SetItemText(hItem, 1, i, (const char*)GUI_hex2MeterAddrStr(g_gui_para.relayAddr[i],6));
    }
}




/*********************************************************************
*
*       _cbDialog
*/
static void _cbDialog(WM_MESSAGE * pMsg) {
  WM_HWIN hItem;
  int     NCode;
  int     Id;
  int     RowNum;
  
  //int     RowCount;
  // USER START (Optionally insert additional variables)
  // USER END

  switch (pMsg->MsgId) 
  {
  case WM_INIT_DIALOG:
    GUI_UC_SetEncodeUTF8();
    _init_dialog(pMsg);
    break;
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
  case WM_KEY:
    //if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt)==0)//按键释放的时候再处理消息
    Id  = WM_GetId(pMsg->hWinSrc);
    if((((WM_KEY_INFO*)(pMsg->Data.p))->PressedCnt) == 1)
    {
        switch(((WM_KEY_INFO*)(pMsg->Data.p))->Key)
        {
            case GUI_KEY_YELLOW:
                WM_DeleteWindow(g_hWin_relay);
                g_hWin_relay=HBWIN_NULL;
                WM_SetFocus(g_hWin_std);
                CPT_SetFocus();
                break;

            case GUI_KEY_GREEN:
                RLY_GetParaAddr(pMsg);
                //g_rom_para.
                g_gui_para.cmdType = PLC_CMD_TYPE_ROUTE ;
                //g_sys_ctrl.sysUseRoute = 1;
                WM_DeleteWindow(g_hWin_relay);
                g_hWin_relay=HBWIN_NULL;
                WM_SetFocus(g_hWin_std);
                CPT_SetFocus();
                break;
                
            case GUI_KEY_F1: /*删除*/
                Del_Relay_Addr(pMsg);
                break;
                
           case GUI_KEY_F2: /*修改*/
                hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0);
                if(LISTVIEW_GetNumRows(hItem) > 0)
                {
                    g_sys_ctrl.selectWidget = MODIFY_RELAY_ADDR;
                    g_hWin_Input = Create_Edit_Set(g_hWin_relay);
                    WM_SetFocus(g_hWin_Input);
                }
                break;
                
           case '#':      //按下的时候就直接打开了新的编辑框体
                hItem = WM_GetDialogItem(pMsg->hWin,ID_LISTVIEW_0);
                g_sys_ctrl.sysAddrLevel = LISTVIEW_GetNumRows(hItem);
                if(g_sys_ctrl.sysAddrLevel >= 7)
                {
                    ERR_NOTE(g_hWin_relay,9);
                    return ;
                }
                g_sys_ctrl.selectWidget = ADD_RELAY_ADDR;
                //RLY_AddAddr(pMsg);
                g_hWin_Input = Create_Edit_Set(g_hWin_relay);
                WM_SetFocus(g_hWin_Input);                
                break;
                
           case '*':
              if(PLC_ROUTE_ON == g_sys_ctrl.sysUseRoute)
                {
                    g_sys_ctrl.sysUseRoute = PLC_ROUTE_OFF;//关闭的时候恢复控制字
                    g_gui_para.ctlCode -= g_sys_ctrl.sysAddrLevel * DL645_RELAY_ADDED_VAL;
                    hItem = WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0);
                    BUTTON_SetText(hItem ,AlreadyClosed);
                }
                else if(PLC_ROUTE_OFF == g_sys_ctrl.sysUseRoute)
                {
                    g_sys_ctrl.sysUseRoute = PLC_ROUTE_ON;
                    hItem = WM_GetDialogItem(pMsg->hWin,ID_BUTTON_0);
                    BUTTON_SetText(hItem ,AlreadyStart);
                }
                break;        
        }
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}

/*********************************************************************
*
*       Public code
*
**********************************************************************
*/
/*********************************************************************
*
*       CreateRelayAddr
*/
WM_HWIN CreateRelayAddr(void);
WM_HWIN CreateRelayAddr(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, g_hWin_std, 0, 0);
  return hWin;
}








