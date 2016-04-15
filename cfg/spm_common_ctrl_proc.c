/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_common_ctrl_proc.c
  �� �� ��   : ����
  ��    ��   : lh09
  ��������   : 2012��6��8��
  ����޸�   :
  ��������   : �������ó��ÿ�������
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��6��8��
    ��    ��   : lh09
    �޸�����   : �����ļ�

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
extern NBB_INT spm_clk_card_switch(NBB_VOID);//�����л�

#endif

/*****************************************************************************
 �� �� ��  : spm_send_laser_ctl_cfg
 ��������  : ��֯�ӿ����ÿ�������IPS��Ϣ
 �������  : ATG_DCI_CMD_COMMON_CTRL *pstCommonCtrl  
             NBB_BYTE ucSubCardNo                    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��8��14��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

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
        /* ��ʼ����Ϣ��                                                          */
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
 �� �� ��  : spm_rcv_dci_cmd_clock_ctrl
 ��������  : ����ʱ�ӿ�������
 �������  : ATG_DCI_CMD_CLK_CTRL *pstClockCtrl NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��4��16��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_cmd_clock_ctrl(ATG_DCI_CMD_CLK_CTRL *pstClockCtrl NBB_CCXT_T NBB_CXT)
{
 
    NBB_BYTE *pucNoActiveDataStart = NULL;
    NBB_INT iRv = 0;
    /* ��ȡ�������� */
    ATG_DCI_CLK_CTRL_NO_ACTIVATE *pstNoActiveData = NULL;
    ATG_DCI_CLK_CTRL_PAUSE_DATA *pstPauseData = NULL;
    ATG_DCI_CLK_CTRL_SWITCH_DATA *psSwitchData = NULL;
    ATG_DCI_CLK_CTRL_CLEAR_DATA *pstClearData = NULL;
    
    NBB_TRC_ENTRY("spm_rcv_dci_cmd_clock_ctrl"); 
    
    #ifdef SRC
    
    /* �������ָ�������Ч */
    NBB_ASSERT(pstClockCtrl != NULL);  
    
    /* �����һ��entry�ĵ�ַ��*/
    pucNoActiveDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstClockCtrl,
        &pstClockCtrl->no_activate_data);

    /* �׵�ַΪNULL���쳣 */
    if (pucNoActiveDataStart == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "pucbasicDataStart is NULL."));
        goto EXIT_LABEL;
    } 
    else
    {
        pstNoActiveData = (ATG_DCI_CLK_CTRL_NO_ACTIVATE*)pucNoActiveDataStart;
        /* ���յ����������� */
        if(0 == pstNoActiveData->activate_flag)
        {
          iRv = spm_clk_card_switch();
        }
                
    }

    #endif
    EXIT_LABEL : NBB_TRC_EXIT();
}
/*****************************************************************************
 �� �� ��  : spm_rcv_dci_cmd_common_ctrl
 ��������  : �������ó��ÿ�������
 �������  : ATG_DCI_STATUS_RQ *pstStatusRq   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��8��
    ��    ��   : lh09
    �޸�����   : �����ɺ���

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

    /* �������ָ�������Ч */
    NBB_ASSERT(pstCommonCtrl != NULL);

    cmd_buffer = pstCommonCtrl->cmd_buffer;
    NBB_GET_SHORT(cmd_type, cmd_buffer);
    NBB_GET_SHORT(cmd_len, cmd_buffer + SPM_SUB_CONF_TYPE_SIZE);

    /* �����帴λ����. */
    if (SPM_CTRL_TYPE_BOARD_RESET == cmd_type)
    {
        /* �ϸ��飬�ֽ�4�����01=��λ. */
        if (0x01 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 3))
        {
		OS_PRINTF(" spm_rcv_cmd_ctrl system reboot");
		
            BmuReLoadBoardSoft(*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1));
	      /* auto system reboot. */
	      /* modify by ljuan,BmuReLoadBoardSoft���Ǵ���λ */
	      //system("reboot");
        }
    }
    
    /* �����������ؿ������� gaos 8.13*/ 
    else if (SPM_CTRL_TYPE_LASER_SWITCH == cmd_type)
    {
        ucCardId = *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1);
        ucPortId = *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 2);
        ucEnable = *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 3);

        #ifdef SPU

        /* ���٣��ȷ���BFD DOWN��Ϣ */
        bfd_shutdown_port(ucCardId, ucPortId, ucEnable);
        
        /* �ϸ��飬�ֽ�2�����0=����. */
        if (0x0 == ucCardId)
        {
            spm_hardw_setsfpen(ucPortId,ucEnable);
        }
        else // �ӿ�
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,ucCardId NBB_CCXT);
        }

        /* ��¼log�����ϱ�״̬�仯 */
        spm_format_laserstate(ucCardId, ucPortId, ucEnable, 0);
        #endif
    }
    /* ����LINE���⻷�������� gaos 8.22*/ 
    else if(SPM_CTRL_TYPE_LINE_OUT_RING == cmd_type)
    {
        /* �ϸ��飬�ֽ�2�����0=����. */    
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
    /* ����SYS���ڻ��������� gaos 8.22*/     
    else if(SPM_CTRL_TYPE_SYS_IN_RING == cmd_type)
    {
        /* �ϸ��飬�ֽ�2�����0=����. */     
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
    /* �����ӿ���·���ؿ������� 5-9 gaos 8.22*/         
    else if(SPM_CTRL_TYPE_SUBCARD_LINE_OUT_RING == cmd_type)
    {
        /* �ֽ�2�����1/2=�ӿ�1/�ӿ�2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }

    /*32��E1����·�������� 33��E1���豸��������*/
    else if((SPM_CTRL_TYPE_E1_LINE == cmd_type) || (cmd_type == SPM_CTRL_TYPE_E1_SYS))
    {
        /* �ֽ�2�����1/2=�ӿ�1/�ӿ�2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)) ||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,
                    *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE_SUBCARD_SYS_IN_RING == cmd_type)
    {
        /* �ֽ�2�����1/2=�ӿ�1/�ӿ�2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE__ALL_LINE_OUT_RING == cmd_type)
    {
        /* �ֽ�2�����1/2=�ӿ�1/�ӿ�2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE__ALL_LINE_SYS_IN_RING == cmd_type)
    {
        /* �ֽ�2�����1/2=�ӿ�1/�ӿ�2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE__BRANCH_LINE_SYS_IN_RING == cmd_type)
    {
        /* �ֽ�2�����1/2=�ӿ�1/�ӿ�2 */
        if ((0x1 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1))||
            (0x2 == *(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1)))
        {
            spm_send_laser_ctl_cfg(pstCommonCtrl,*(cmd_buffer + SPM_SUB_CONF_TYPE_SIZE + SPM_SUB_CONF_LENGTH_SIZE + 1) NBB_CCXT);
            
        }
    }
    else if(SPM_CTRL_TYPE__FE1600_UNSHUTDOWN == cmd_type)
    {        
        /* ����оƬ���� */ 
        #ifdef SRC
        spm_hardw_dfe(); 
        #endif

        /* ֪ͨFPGA�����Ѿ��·���� */
        spm_bfd_cpu_work();
    }
    
    else if(SPM_CTRL_TYPE__PWPING_SEND_ENABLE == cmd_type)
    {        
        /* PW PING ECHO������ʹ�ܡ�ȥʹ������*/ 
        #ifdef SPU
        
        //����API������������
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

        /* ����������֮�󣬻�ͳһ��ʱ�����Ծ٣�
        �������ù����У���ֹ��ʱ�����µ��Ծ���Ϣ���͡� */
        if(SHARED.proc_timer.pending)
        {
            printf("spm_rcv_cmd_ctrl : start clean cfg, cancel timer\n");
            NBB_CANCEL_TIMER(&(SHARED.proc_timer));
        }
    
        /* ������ÿ������� */
        msecs = NBB_GET_MTIME();
        printf("spm_rcv_cmd_ctrl : clean cfg start at %ld ms\n", msecs);
        /* add by gaos,����͸���ӿ�,llgao ces�ӿ����������� */
        spm_send_laser_ctl_cfg(pstCommonCtrl, 1 NBB_CCXT);
        spm_send_laser_ctl_cfg(pstCommonCtrl, 2 NBB_CCXT);  
        
        spm_free_cfgmem();
        
        msecs = NBB_GET_MTIME();
        printf("spm_rcv_cmd_ctrl : clean app end at %ld ms\n", msecs);  
        
        /* ��������ӿ� */
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

    /*Ӧ�ò�����������vpn����ʱ����ʼ��L2,L3VPN*/
    spm_init_drv_vpn();
    #endif

    
        almpm_del_all();
        spm_proc_hw_initok();

        /* ����һ���Է����Ծ���Ϣ�Ķ�ʱ������ʱ20�������Ծ���Ϣ */
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
 �� �� ��  : spm_send_laserctl_auto
 ��������  : �����Զ��ضϼ�������ʱ����ips���ӿ��Ľӿ�
 �������  : NBB_BYTE ucCardId : 0/1/2     
             NBB_BYTE ucPortId : 1-40     
             NBB_BYTE ucLaserState : 0/1 = ��/�� 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��11��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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

/* �ֽ�1~2������������ֵ��Type�����̶�Ϊ1
   �ֽ�3~4�������ó��ȣ�Length����4
   ���������ݣ�Value����
   �ֽ�1����λ�ţ�=1-10
   �ֽ�2���ӿ��ţ�0/1/2=����/�ӿ�1/�ӿ�2
   �ֽ�3���˿ںţ�=1-20
   �ֽ�4�����01/00=��/�� */
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
 �� �� ��  : spm_send_laserctl_ips_auto_n
 ��������  : �ֽ����scoket��Ϣ�ɶ������ips��Ϣ
 �������  : NBB_USHORT usLen                    
             NBB_BYTE *ucBuf NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��26��
    ��    ��   : ljuan
    �޸�����   : �����ɺ���

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
 �� �� ��  : spm_send_laserctl_socket
 ��������  : ����socket��Ϣ��ĸ��spm
 �������  : NBB_BYTE ucCardId      
             NBB_BYTE ucPortId      
             NBB_BYTE ucLaserState  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��11��21��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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

    /* �����ֽڵ�Э��汾�� */
    ucSocketData[0] = 0;
    ucSocketData[1] = 0x6;

    /* �����ֽڵ���Ŀ�� */
    ucSocketData[2] = 0;
    ucSocketData[3] = 0x1;

    /* �����ֽڵ���Ϣ���� */
    ucSocketData[4] = 0;
    ucSocketData[5] = 0x12;

    /* �����ֽڵ���Ϣ���� */
    ucSocketData[6] = 0;
    ucSocketData[7] = 3;

    /* ��һ���ֽ�0/1/2=ĸ��/�ӿ�1/�ӿ�2���ڶ����ֽڶ˿ںţ���1��ʼ����������
       �ֽڿ���״̬��0/1 =��/���� */
    ucSocketData[8] = ucCardId;
    ucSocketData[9] = ucPortId;
    ucSocketData[10] = ucLaserState;

    SendData(0x7f000001, 4, 0x101200, 11, &ucSocketData[0]);

    return OK;
}

/*****************************************************************************
 �� �� ��  : spm_send_laserctl_socket_n
 ��������  : ����������������Ϣ��һ��socket����
 �������  : NBB_USHORT usLen                    
             NBB_BYTE *ucBuf NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��26��
    ��    ��   : ljuan
    �޸�����   : �����ɺ���

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
    
    /* �����ֽڵ�Э��汾�� */
    *ucSckData = 0;
    *(ucSckData + 1) = 0x6;

    /* �����ֽڵ���Ŀ�� */
    *(ucSckData + 2) = 0;
    *(ucSckData + 3) = 0x1;

    /* �����ֽڵ���Ϣ���� */
    *(ucSckData + 4) = 0;
    *(ucSckData + 5) = 0x12;

    /* �����ֽڵ���Ϣ���� */
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
 �� �� ��  : spm_set_lasersw
 ��������  : ���ü��������أ����ϱ�����״̬
 �������  : NBB_BYTE ucCardId : 0/1/2     
             NBB_BYTE ucPortId : 1-40     
             NBB_BYTE ucLaserState : 0/1 = ��/�� 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��11��20��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
        /* ����ĸ���ģ�ֱ�Ӳ��������� */
        spm_hardw_setsfpen(ucPortId, ucLaserState);
    }
    else /* �ӿ��ļ�����������socket��Ϣ��SPM,��spm����ips���ӿ����̣�������
            ���� */
    {
        spm_send_laserctl_socket(ucCardId, ucPortId, ucLaserState NBB_CCXT);
    }

    /* ��¼log���ϱ�״̬�仯 */
    spm_format_laserstate(ucCardId, ucPortId, ucLaserState, 1);
#endif
    
    return OK;
}

/*****************************************************************************
 �� �� ��  : spm_set_lasersw_n
 ��������  : aps�������Ƽ���������
 �������  : NBB_USHORT usLen  
             NBB_BYTE *ucBuf   
             �洢˳��Ϊ �ӿ�+�˿�+������״̬�����磺���ӿ�1��port1/2/3�����ӿ�2��port 4/5/6�ӿ�
                usLen: 18
                buf: data[18] = {1,1,0,1,2,0,1,3,0,2,4,1,2,5,1,2,6,1}
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��26��
    ��    ��   : ljuan
    �޸�����   : �����ɺ���

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
    
    /* ����ĸ���ģ�ֱ�Ӳ��������� */
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
        
        /* ��¼log���ϱ�״̬�仯 */
        spm_format_laserstate(ucCardId, ucPortId, ucLaserState, 1);
    }
#endif
}

/*****************************************************************************
 �� �� ��  : spm_set_Interframegap
 ��������  : ���ö˿�֡���
 �������  : NBB_UINT uiIpg  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��12��5��
    ��    ��   : ����
    �޸�����   : �����ɺ���

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
