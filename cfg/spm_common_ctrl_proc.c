/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_common_ctrl_proc.c
  版 本 号   : 初稿
  作    者   : lh09
  生成日期   : 2012年6月8日
  最近修改   :
  功能描述   : 处理设置常用控制命令
  函数列表   :
  修改历史   :
  1.日    期   : 2012年6月8日
    作    者   : lh09
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include "hardw_public.h"

#ifdef SPU
extern NBB_VOID spm_hardw_setsfpen(IN unsigned char ucPort,
                                  IN unsigned char ucEnable);

extern NBB_INT spm_format_laserstate(IN NBB_BYTE ucCardId, 
    IN NBB_BYTE ucPortId,
    IN NBB_BYTE ucLaserState,
    IN NBB_BYTE ucCtrltype);      

extern NBB_BYTE spm_hardw_getportnum(IN NBB_BYTE ucSubCardId);
#endif  

#ifdef SRC
extern NBB_INT spm_clk_card_switch(NBB_VOID);//主备切换

#endif

/*****************************************************************************
 函 数 名  : spm_send_laser_ctl_cfg
 功能描述  : 组织子卡常用控制命令IPS消息
 输入参数  : ATG_DCI_CMD_COMMON_CTRL *pstCommonCtrl  
             NBB_BYTE ucSubCardNo                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年8月14日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_laser_ctl_cfg(ATG_DCI_CMD_COMMON_CTRL *pstCommonCtrl, NBB_BYTE ucSubCardNo NBB_CCXT_T NBB_CXT)

{
    NBB_BYTE ucMessage[80];
    NBB_INT iRv = 0;
    ATG_SBI_CMD_COMMON_CTRL *pSbiCommonCtrl;
    
    NBB_TRC_ENTRY("spm_send_laser_ctl_cfg");
    
    if (pstCommonCtrl == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_send_laser_ctl_cfg(pstCommonCtrl==NULL)"));

        OS_PRINTF("***ERROR***:spm_send_laser_ctl_cfg(pstCommonCtrl==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_send_laser_ctl_cfg(pstCommonCtrl==NULL)\n");
        
        iRv = ERROR;
        goto EXIT_LABEL;
    }

    if (SHARED.sub_card_cb[ucSubCardNo-1] != NULL)
    {
        pSbiCommonCtrl = (ATG_SBI_CMD_COMMON_CTRL*) NBB_GET_BUFFER(NBB_NULL_HANDLE,
                                           NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CMD_COMMON_CTRL)),
                                           0,
                                           NBB_RETRY_DATA | NBB_BUF_PKT); 

        if (NULL == pSbiCommonCtrl)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_send_laser_ctl_cfg(pSbiCommonCtrl==NULL)"));

            OS_PRINTF("***ERROR***:spm_send_laser_ctl_cfg(pSbiCommonCtrl==NULL)\n");

            OS_SPRINTF(ucMessage, "***ERROR***:spm_send_laser_ctl_cfg(pSbiCommonCtrl==NULL)\n");
            
            iRv = ERROR;
            
            goto EXIT_LABEL;
        }
        /*************************************************************************/
        /* 初始化消息。                                                          */
        /*************************************************************************/
        NBB_ZERO_IPS(pSbiCommonCtrl);        

        pSbiCommonCtrl->ips_hdr.ips_type = IPS_ATG_SBI_CMD_COMMON_CTRL;
        pSbiCommonCtrl->key = 1;
        OS_MEMCPY(pSbiCommonCtrl->cmd_buffer, pstCommonCtrl->cmd_buffer, sizeof(NBB_BYTE)*ATG_DCI_CMD_MAX_LEN);


        spm_snd_sbi_ips(SHARED.sub_card_cb[ucSubCardNo-1], pSbiCommonCtrl, &(pSbiCommonCtrl->ips_hdr) NBB_CCXT);
    }
        
    EXIT_LABEL: NBB_TRC_EXIT();
    
    return iRv;
}
/*****************************************************************************
 函 数 名  : spm_rcv_dci_cmd_clock_ctrl
 功能描述  : 处理时钟控制命令
 输入参数  : ATG_DCI_CMD_CLK_CTRL *pstClockCtrl NBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年4月16日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_cmd_clock_ctrl(ATG_DCI_CMD_CLK_CTRL *pstClockCtrl NBB_CCXT_T NBB_CXT)
{
 
    NBB_BYTE *pucNoActiveDataStart = NULL;
    NBB_INT iRv = 0;
    /* 获取的子配置 */
    ATG_DCI_CLK_CTRL_NO_ACTIVATE *pstNoActiveData = NULL;
    ATG_DCI_CLK_CTRL_PAUSE_DATA *pstPauseData = NULL;
    ATG_DCI_CLK_CTRL_SWITCH_DATA *psSwitchData = NULL;
    ATG_DCI_CLK_CTRL_CLEAR_DATA *pstClearData = NULL;
    
    NBB_TRC_ENTRY("spm_rcv_dci_cmd_clock_ctrl"); 
    
    #ifdef SRC
    
    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstClockCtrl != NULL);  
    
    /* 计算第一个entry的地址。*/
    pucNoActiveDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstClockCtrl,
        &pstClockCtrl->no_activate_data);

    /* 首地址为NULL，异常 */
    if (pucNoActiveDataStart == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "pucbasicDataStart is NULL."));
        goto EXIT_LABEL;
    } 
    else
    {
        pstNoActiveData = (ATG_DCI_CLK_CTRL_NO_ACTIVATE*)pucNoActiveDataStart;
        /* 接收到不激活命令 */
        if(0 == pstNoActiveData->activate_flag)
        {
          iRv = spm_clk_card_switch();
        }
                
    }

    #endif
    EXIT_LABEL : NBB_TRC_EXIT();
}
/*****************************************************************************
 函 数 名  : spm_rcv_dci_cmd_common_ctrl
 功能描述  : 处理设置常用控制命令
 输入参数  : ATG_DCI_STATUS_RQ *pstStatusRq   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月8日
    作    者   : lh09
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_cmd_common_ctrl(ATG_DCI_CMD_COMMON_CTRL *pstCommonCtrl NBB_CCXT_T NBB_CXT)
{
    NBB_BYTE *cmd_buffer;
    NBB_USHORT cmd_type, cmd_len;
    NBB_BYTE ucCardId = 0;
    NBB_BYTE ucPortId = 0;
    NBB_BYTE ucEnable = 0;
    NBB_LONG msecs=0;
    NBB_BYTE ucc3unit = 0;
    NBB_CHAR ucmessage[SPM_MSG_INFO_LEN];
    NBB_INT ret = 0;    
    
    NBB_TRC_ENTRY("spm_rcv_dci_cmd_common_ctrl");    

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstCommonCtrl != NULL);

    cmd_buffer = pstCommonCtrl->cmd_buffer;
    NBB_GET_SHORT(cmd_type, cmd_buffer);
    NBB_GET_SHORT(cmd_len, cmd_buffer + SPM_SUB_CONF_TYPE_SIZE);

    /* 处理单板复位命令. */
    if (SPM_CTRL_TYPE_BOARD_RESET == cmd_type)
    {
        /* 严格检查，字节4：命令：01=复位. */
        if (0x01 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 3))
        {
		OS_PRINTF(" spm_rcv_cmd_ctrl system reboot");
		
            BmuReLoadBoardSoft(*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1));
	      /* auto system reboot. */
	      /* modify by ljuan,BmuReLoadBoardSoft就是处理复位 */
	      //system("reboot");
        }
    }
    
    /* 处理激光器开关控制命令 gaos 8.13*/ 
    else if (SPM_CTRL_TYPE_LASER_SWITCH == cmd_type)
    {
        ucCardId = *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1);
        ucPortId = *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 2);
        ucEnable = *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 3);

        #ifdef SPU

        /* 作假，先发送BFD DOWN消息 */
        bfd_shutdown_port(ucCardId, ucPortId, ucEnable);
        
        /* 严格检查，字节2：命令：0=主板. */
        if (0x0 == ucCardId)
        {
            spm_hardw_setsfpen(ucPortId,ucEnable);
        }
        else // 子卡
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,ucCardId NBB_CCXT);
        }

        /* 记录log，并上报状态变化 */
        spm_format_laserstate(ucCardId, ucPortId, ucEnable, 0);
        #endif
    }
    /* 处理LINE侧外环控制命令 gaos 8.22*/ 
    else if(SPM_CTRL_TYPE_LINE_OUT_RING == cmd_type)
    {
        /* 严格检查，字节2：命令：0=主板. */    
        if ((0x0 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
                (0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
                    (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
          #ifdef SPU
          spm_hardw_setC3PhyRemoteLoopBack(*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1),
                                            *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 2),
                                            *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 3));
          
          #endif                
        }

    }
    /* 处理SYS侧内环控制命令 gaos 8.22*/     
    else if(SPM_CTRL_TYPE_SYS_IN_RING == cmd_type)
    {
        /* 严格检查，字节2：命令：0=主板. */     
        if ((0x0 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
                (0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
                    (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
          #ifdef SPU
          spm_hardw_setC3PhyLocalLoopBack(*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1),
                                            *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 2),
                                            *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 3));
          
          #endif                
        }
    }
    /* 处理子卡线路环回控制命令 5-9 gaos 8.22*/         
    else if(SPM_CTRL_TYPE_SUBCARD_LINE_OUT_RING == cmd_type)
    {
        /* 字节2：命令：1/2=子卡1/子卡2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }

    /*32）E1口线路环回命令 33）E1口设备环回命令*/
    else if((SPM_CTRL_TYPE_E1_LINE == cmd_type) || (cmd_type == SPM_CTRL_TYPE_E1_SYS))
    {
        /* 字节2：命令：1/2=子卡1/子卡2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)) ||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,
                    *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE_SUBCARD_SYS_IN_RING == cmd_type)
    {
        /* 字节2：命令：1/2=子卡1/子卡2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE__ALL_LINE_OUT_RING == cmd_type)
    {
        /* 字节2：命令：1/2=子卡1/子卡2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE__ALL_LINE_SYS_IN_RING == cmd_type)
    {
        /* 字节2：命令：1/2=子卡1/子卡2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE__BRANCH_LINE_SYS_IN_RING == cmd_type)
    {
        /* 字节2：命令：1/2=子卡1/子卡2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE__FE1600_UNSHUTDOWN == cmd_type)
    {        
        /* 撤销芯片隔离 */ 
        #ifdef SRC
        spm_hardw_dfe(); 
        #endif

        /* 通知FPGA配置已经下发完成 */
        spm_bfd_cpu_work();
    }
    
    else if(SPM_CTRL_TYPE__PWPING_SEND_ENABLE == cmd_type)
    {        
        /* PW PING ECHO包上送使能、去使能命令*/ 
        #ifdef SPU
        
        //调用API函数进行配置
        for (ucc3unit = 0; ucc3unit < SHARED.c3_num; ucc3unit++)
        {
            ret = fhdrv_psn_l3_set_pw_ping_trans_enable(ucc3unit, 
                *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE));
            OS_SPRINTF(ucmessage, 
                "fhdrv_psn_l3_set_pw_ping_trans_enable(): ucc3unit=%d enable==%d ret=%d\n", 
                ucc3unit, *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE), ret);               
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucmessage);
        }
        #endif
    }
    
    else if(SPM_CTRL_TYPE__CFG_CLEARALL == cmd_type)
    {

        /* 清除配置完成之后，会统一起定时器来自举，
        在清配置过程中，防止定时器导致的自举消息上送。 */
        if(SHARED.proc_timer.pending)
        {
            printf("spm_rcv_cmd_ctrl : start clean cfg, cancel timer\n");
            NBB_CANCEL_TIMER(&(SHARED.proc_timer));
        }
    
        /* 清除配置控制命令 */
        msecs = NBB_GET_MTIME();
        printf("spm_rcv_cmd_ctrl : clean cfg start at %ld ms\n", msecs);
        /* add by gaos,配置透传子卡,llgao ces子卡清配置需求 */
        spm_send_laser_ctl_cfg(pstCommonCtrl, 1 NBB_CCXT);
        spm_send_laser_ctl_cfg(pstCommonCtrl, 2 NBB_CCXT);  
        
        spm_free_cfgmem();
        
        msecs = NBB_GET_MTIME();
        printf("spm_rcv_cmd_ctrl : clean app end at %ld ms\n", msecs);  
        
        /* 清除驱动接口 */
    #ifdef SPU
        ApiC3ClearTbl(BCMC31);
        if ( SHARED.c3_num == 2 )
        ApiC3ClearTbl(BCMC32);
        ApiAradClearAllCfg();
    #endif
    #if SRC
        ApiDfeClearAllCfg(0);
    #endif
    #if SNU
        ApiDfeClearAllCfg(0);
    #endif
    #ifdef SPU

    /*应用层在驱动清完vpn配置时，初始化L2,L3VPN*/
    spm_init_drv_vpn();
    #endif

    
        almpm_del_all();
        spm_proc_hw_initok();

        /* 启动一次性发送自举消息的定时器，延时20秒上送自举消息 */
        if (SHARED.proc_timer_clearcfg_active == FALSE)
        {
            //SHARED.proc_timer_clearcfg_active = TRUE;
            //NBB_SET_TIMER(&(SHARED.proc_timer_clearcfg));
            printf("clear cfg end, start boot timer.....\n");
        }
        else
        {
            printf("clear cfg end, already start boot timer.....\n");

        }
        
        msecs = NBB_GET_MTIME();
        printf("spm_rcv_cmd_ctrl : clean cfg end at %ld ms\n", msecs);
        
    }
    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_send_laserctl_auto
 功能描述  : 单盘自动关断激光器的时候发送ips给子卡的接口
 输入参数  : NBB_BYTE ucCardId : 0/1/2     
             NBB_BYTE ucPortId : 1-40     
             NBB_BYTE ucLaserState : 0/1 = 关/开 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月21日
    作    者   : 刘娟
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_laserctl_ips_auto(NBB_BYTE ucCardId, NBB_BYTE ucPortId, NBB_BYTE ucLaserState NBB_CCXT_T NBB_CXT)
{
    ATG_DCI_CMD_COMMON_CTRL stData;
    NBB_BYTE ucSlot = 0;

    printf("DEBUG(%s)card%d-port%d laserstate:%d\n", __FUNCTION__, ucCardId, ucPortId, ucLaserState);
    if (ucCardId == 0)
    {
        if ((ucPortId == 0) || (ucPortId > MAX_LINEPORT_NUM))
        {
            printf("ERROR(%s-%d):card%d port%d param error!\n",__FILE__, __LINE__, ucCardId, ucPortId);
            return ERROR;
        }
    }
    else if ((ucCardId == 1) || (ucCardId == 2))
    {
        if ((ucPortId == 0) || (ucPortId > MAX_SONCARD_PORT))
        {
            printf("ERROR(%s-%d):card%d port%d param error!\n",__FILE__, __LINE__, ucCardId, ucPortId);
            return ERROR;
        }
    }
    else
    {
        printf("ERROR(%s-%d):card%dparam error!\n",__FILE__, __LINE__, ucCardId);
        return ERROR;
    }

    if (ucLaserState != 0)
    {
        ucLaserState = 1;
    }
    
    spm_hardw_getslot(&ucSlot);

    memset(&stData, 0, sizeof(ATG_DCI_CMD_COMMON_CTRL));

/* 字节1~2：子配置类型值（Type）：固定为1
   字节3~4：子配置长度（Length）：4
   子配置内容（Value）：
   字节1：槽位号：=1-10
   字节2：子卡号：0/1/2=主板/子卡1/子卡2
   字节3：端口号：=1-20
   字节4：命令：01/00=开/关 */
    stData.cmd_buffer[0] = 0;
    stData.cmd_buffer[1] = SPM_CTRL_TYPE_LASER_SWITCH;
    stData.cmd_buffer[2] = 0;
    stData.cmd_buffer[3] = 4;
    stData.cmd_buffer[4] = ucSlot;
    stData.cmd_buffer[5] = ucCardId;
    stData.cmd_buffer[6] = ucPortId;
    stData.cmd_buffer[7] = ucLaserState;

    spm_send_laser_ctl_cfg(&stData, ucCardId NBB_CCXT);

    return OK;
}

/*****************************************************************************
 函 数 名  : spm_send_laserctl_ips_auto_n
 功能描述  : 分解多条scoket消息成多个单条ips消息
 输入参数  : NBB_USHORT usLen                    
             NBB_BYTE *ucBuf NBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月26日
    作    者   : ljuan
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_laserctl_ips_auto_n(NBB_USHORT usLen, NBB_BYTE *ucBuf NBB_CCXT_T NBB_CXT)
{
    NBB_BYTE ucCardId = 0;
    NBB_BYTE ucPortId = 0;
    NBB_BYTE ucLaserState = 0;
    NBB_USHORT usCnt = usLen;

    if ((NULL == ucBuf) || ((usLen % 3) != 0) || (usLen == 0))
    {
        return ERROR;
    }

    for (usCnt = 0; usCnt < (usLen / 3); usCnt++)
    {
        ucCardId = *(ucBuf + (usCnt * 3));
        ucPortId = *(ucBuf + ((usCnt * 3) + 1));
        ucLaserState = *(ucBuf + ((usCnt * 3) + 2));

        if (ucLaserState != 0)
        {
            ucLaserState = 1;
        }

        if (ucCardId != 0)
        {
            spm_send_laserctl_ips_auto(ucCardId, ucPortId, ucLaserState);
        }
    }

    return OK;
}

/*****************************************************************************
 函 数 名  : spm_send_laserctl_socket
 功能描述  : 发送socket消息给母板spm
 输入参数  : NBB_BYTE ucCardId      
             NBB_BYTE ucPortId      
             NBB_BYTE ucLaserState  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月21日
    作    者   : 刘娟
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_laserctl_socket(NBB_BYTE ucCardId, NBB_BYTE ucPortId, NBB_BYTE ucLaserState NBB_CCXT_T NBB_CXT)
{
    NBB_BYTE ucSocketData[11] = {0};
    
    if (ucCardId == 0)
    {
        if ((ucPortId == 0) || (ucPortId > MAX_LINEPORT_NUM))
        {
            printf("ERROR(%s-%d):card%d port%d param error!\n",__FILE__, __LINE__, ucCardId, ucPortId);
            return ERROR;
        }
    }
    else if ((ucCardId == 1) || (ucCardId == 2))
    {
        if ((ucPortId == 0) || (ucPortId > MAX_SONCARD_PORT))
        {
            printf("ERROR(%s-%d):card%d port%d param error!\n",__FILE__, __LINE__, ucCardId, ucPortId);
            return ERROR;
        }
    }
    else
    {
        printf("ERROR(%s-%d):card%dparam error!\n",__FILE__, __LINE__, ucCardId);
        return ERROR;
    }

    if (ucLaserState != 0)
    {
        ucLaserState = 1;
    }

    /* 两个字节的协议版本号 */
    ucSocketData[0] = 0;
    ucSocketData[1] = 0x6;

    /* 两个字节的条目数 */
    ucSocketData[2] = 0;
    ucSocketData[3] = 0x1;

    /* 两个字节的信息类型 */
    ucSocketData[4] = 0;
    ucSocketData[5] = 0x12;

    /* 两个字节的信息长度 */
    ucSocketData[6] = 0;
    ucSocketData[7] = 3;

    /* 第一个字节0/1/2=母卡/子卡1/子卡2，第二个字节端口号（从1开始），第三个
       字节开关状态（0/1 =关/开） */
    ucSocketData[8] = ucCardId;
    ucSocketData[9] = ucPortId;
    ucSocketData[10] = ucLaserState;

    SendData(0x7f000001, 4, 0x101200, 11, &ucSocketData[0]);

    return OK;
}

/*****************************************************************************
 函 数 名  : spm_send_laserctl_socket_n
 功能描述  : 批量激光器开关信息用一条socket发送
 输入参数  : NBB_USHORT usLen                    
             NBB_BYTE *ucBuf NBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月26日
    作    者   : ljuan
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_laserctl_socket_n(NBB_USHORT usLen, NBB_BYTE *ucBuf NBB_CCXT_T NBB_CXT)
{
    NBB_BYTE *ucSckData = NULL;
    NBB_USHORT usCnt = 0;

    ucSckData = (NBB_BYTE *)malloc(8 + usLen);

    if (ucSckData == NULL)
    {
        return ERROR;
    }
    
    /* 两个字节的协议版本号 */
    *ucSckData = 0;
    *(ucSckData + 1) = 0x6;

    /* 两个字节的条目数 */
    *(ucSckData + 2) = 0;
    *(ucSckData + 3) = 0x1;

    /* 两个字节的信息类型 */
    *(ucSckData + 4) = 0;
    *(ucSckData + 5) = 0x12;

    /* 两个字节的信息长度 */
    *(ucSckData + 6) = 0;
    *(ucSckData + 7) = usLen;

    memcpy((ucSckData + 8), ucBuf, usLen);

    SendData(0x7f000001, 5, 0x101200, (8 + usLen), ucSckData);

    if( NULL != ucSckData )
    {
        free(ucSckData);
    }
    ucSckData = NULL;

    return OK;
}


/*****************************************************************************
 函 数 名  : spm_set_lasersw
 功能描述  : 配置激光器开关，并上报开关状态
 输入参数  : NBB_BYTE ucCardId : 0/1/2     
             NBB_BYTE ucPortId : 1-40     
             NBB_BYTE ucLaserState : 0/1 = 关/开 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月20日
    作    者   : 刘娟
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_lasersw(NBB_BYTE ucCardId, NBB_BYTE ucPortId, NBB_BYTE ucLaserState)
{
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_BYTE ucSlot = 0;

    if (ucCardId == 0)
    {
        if ((ucPortId == 0) || (ucPortId > MAX_LINEPORT_NUM))
        {
            printf("ERROR(%s-%d):card%d port%d param error!\n",__FILE__, __LINE__, ucCardId, ucPortId);
            return ERROR;
        }
    }
    else if ((ucCardId == 1) || (ucCardId == 2))
    {
        if ((ucPortId == 0) || (ucPortId > MAX_SONCARD_PORT))
        {
            printf("ERROR(%s-%d):card%d port%d param error!\n",__FILE__, __LINE__, ucCardId, ucPortId);
            return ERROR;
        }
    }
    else
    {
        printf("ERROR(%s-%d):card%dparam error!\n",__FILE__, __LINE__, ucCardId);
        return ERROR;
    }

    if (ucLaserState != 0)
    {
        ucLaserState = 1;
    }

#ifdef SPU
    if (0x0 == ucCardId)
    {
        /* 非子母卡的，直接操作激光器 */
        spm_hardw_setsfpen(ucPortId, ucLaserState);
    }
    else /* 子卡的激光器，发送socket消息给SPM,由spm发送ips给子卡进程，操作激
            光器 */
    {
        spm_send_laserctl_socket(ucCardId, ucPortId, ucLaserState NBB_CCXT);
    }

    /* 记录log并上报状态变化 */
    spm_format_laserstate(ucCardId, ucPortId, ucLaserState, 1);
#endif
    
    return OK;
}

/*****************************************************************************
 函 数 名  : spm_set_lasersw_n
 功能描述  : aps批量控制激光器开关
 输入参数  : NBB_USHORT usLen  
             NBB_BYTE *ucBuf   
             存储顺序为 子卡+端口+激光器状态，比如：关子卡1的port1/2/3，开子卡2的port 4/5/6接口
                usLen: 18
                buf: data[18] = {1,1,0,1,2,0,1,3,0,2,4,1,2,5,1,2,6,1}
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月26日
    作    者   : ljuan
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_lasersw_n(NBB_USHORT usLen, NBB_BYTE *ucBuf)
{
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    NBB_BYTE ucCardId = 0;
    NBB_BYTE ucPortId = 0;
    NBB_BYTE ucLaserState = 0;
    NBB_USHORT usCnt = 0;

    if ((NULL == ucBuf) || ((usLen % 3) != 0) || (usLen == 0))
    {
        return ERROR;
    }

#ifdef SPU
    spm_send_laserctl_socket_n(usLen, ucBuf);
    
    /* 非子母卡的，直接操作激光器 */
    for (usCnt = 0; usCnt < (usLen / 3); usCnt++)
    {
        ucCardId = *(ucBuf + (usCnt * 3));
        ucPortId = *(ucBuf + ((usCnt * 3) + 1));
        ucLaserState = *(ucBuf + ((usCnt * 3) + 2));

        if (ucLaserState != 0)
        {
            ucLaserState = 1;
        }

        if (0x0 == ucCardId)
        {
            spm_hardw_setsfpen(ucPortId, ucLaserState);
        }
        
        /* 记录log并上报状态变化 */
        spm_format_laserstate(ucCardId, ucPortId, ucLaserState, 1);
    }
#endif
}

/*****************************************************************************
 函 数 名  : spm_set_Interframegap
 功能描述  : 设置端口帧间隔
 输入参数  : NBB_UINT uiIpg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年12月5日
    作    者   : 刘娟
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_set_Interframegap(NBB_UINT uiIpg)
{
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    NBB_BYTE ucPort = 0;
    NBB_BYTE ucCounter = 0;
    NBB_BYTE ucCnt = 0;
    NBB_BYTE ucCard = 0;
    SPM_PORT_STRUCT stPort;

#ifdef SPU
    ucPort = spm_hardw_getportnum(0);

    if(ucPort != 0)
    {
        for (ucCounter = 0; ucCounter < ucPort; ucCounter++)
        {
            spm_hardw_getlocallineport(0, (ucCounter + 1), &stPort);
            ApiC3SetInterframeGap(stPort.ucUnit, stPort.ucPort, uiIpg);
        }
    }
    else
    {
        for (ucCnt = 0; ucCnt < MAX_SONCARD_NUM; ucCnt++)
        {
            ucPort = spm_hardw_getportnum(ucCnt + 1);
            if (ucPort != 0)
            {
                for (ucCounter = 0; ucCounter < ucPort; ucCounter++)
                {
                    spm_hardw_getlocallineport((ucCnt + 1), (ucCounter + 1), &stPort);
                    ApiC3SetInterframeGap(stPort.ucUnit, stPort.ucPort, uiIpg);
                }
            }
        }
    }
#endif    
}
