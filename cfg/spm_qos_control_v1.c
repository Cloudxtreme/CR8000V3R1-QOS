/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_control.c
   �� �� ��   : ����
   ��    ��   : zenglu
   ��������   : 2012��9��18��
   ����޸�   :
   ��������   : QOS����
   �����б�   :
   �޸���ʷ   :
   ��    ��   : 2012��9��18��
   ��    ��   : zenglu
   �޸�����   : �����ļ�

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/* ȫ�ִ�ӡ����acl_pri_setting */
extern NBB_BYTE qos_defend_cfg_print;


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_control_bandwidth_cfg(NBB_ULONG oper,
    ATG_DCI_PORT_BANDWIDTH *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);


    if((ATG_DCI_RC_OK != qos_defend_cfg_print) && (NULL != pcb))
    {
        printf("%s %s,line=%d bandwidth=%lu\n\n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,pcb->bandwidth);
        OS_SPRINTF(ucMessage,"%s %s,%d bandwidth=%lu\n\n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,pcb->bandwidth);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if(ATG_DCI_OPER_ADD == oper)
    {  
        if(NULL == pcb)
        {
            printf("%s,%s,line=%d param==NULL\n",QOS_ERROR_STRING,__FUNCTION__,__LINE__);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			"**QOS ERROR** param==NULL",ATG_DCI_RC_ADD_FAILED,
                    			"","","","",
                    			0,0,0,0));
            ret = ATG_DCI_RC_ADD_FAILED;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetOFPBandWidth(0,QOS_RCU_PORT,pcb->bandwidth);
#endif
        if(0 != ret)
        {
            printf("%s,%s,line=%d ApiAradSetOFPBandWidth bandwidth=%lu,ret=%ld\n",
                QOS_ERROR_STRING,__FUNCTION__,__LINE__,pcb->bandwidth,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"**QOS ERROR** param==NULL",ATG_DCI_RC_ADD_FAILED,
                			"","","","",
                			0,0,0,0));
            goto EXIT_LABEL;
        }  
    }
    

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_control_que_bandwidth_cfg(NBB_ULONG oper,
    ATG_DCI_QUEUE_BANDWIDTH *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_RC_OK != qos_defend_cfg_print) && (NULL != pcb))
    {
        printf("%s %s,line=%d queue_id=%d,cir=%lu,pir=%lu,cbs=%lu,pbs=%lu\n\n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,pcb->queue_id,
            pcb->cir,pcb->pir,pcb->cbs,pcb->pbs);
        OS_SPRINTF(ucMessage,"%s %s,%d queue_id=%d,cir=%lu,pir=%lu,cbs=%lu,pbs=%lu\n\n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,pcb->queue_id,
            pcb->cir,pcb->pir,pcb->cbs,pcb->pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if(ATG_DCI_OPER_ADD == oper)
    { 
        if(NULL == pcb)
        {
            printf("%s,%s,line=%d param==NULL\n",QOS_ERROR_STRING,__FUNCTION__,__LINE__);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			"**QOS ERROR** param==NULL",ATG_DCI_RC_ADD_FAILED,
                    			"","","","",
                    			0,0,0,0));
            ret = ATG_DCI_RC_ADD_FAILED;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueCirEir(0,QOS_RCU_PORT,pcb->queue_id,pcb->cir,
                pcb->pir - pcb->cir, pcb->cbs, pcb->pbs);
#endif
        if(0 != ret)
        {
            printf("%s,%s,line=%d ApiAradSetPortQueueCirEir queue_id=%d,"
                "cir=%lu,pir=%lu,cbs=%lu,pbs=%lu,ret=%ld\n",
                QOS_ERROR_STRING,__FUNCTION__,__LINE__,pcb->queue_id,pcb->cir,
                pcb->pir,pcb->cbs,pcb->pbs,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"**QOS ERROR** ApiAradSetPortQueueCirEir",ret,
                			"queue_id","cir","pir","cbs",
                			pcb->queue_id,pcb->cir,pcb->pir,pcb->cbs));
            goto EXIT_LABEL;
        }  
    }
 
    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_rcu_pir_control(ATG_DCI_SET_SIGNALING_CTRL *pstSetIps)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT i = 0;

    ATG_DCI_PORT_BANDWIDTH *pstBandwitchData = NULL;
    ATG_DCI_QUEUE_BANDWIDTH *pstQueBandwitchData = NULL;
    NBB_BYTE *pucBandwidthDataStart = NULL;
    NBB_BYTE *pucQueBandwidthDataStart = NULL;
    NBB_ULONG ulOperBandwidth = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperQueBandwidth = ATG_DCI_OPER_NULL;

    /*�߼���������*/
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }
    if(TRUE == pstSetIps->delete_struct)
    {
        goto EXIT_LABEL;
    }
    if(pstSetIps->key != v_spm_shared->local_slot_id)
    {
        goto EXIT_LABEL;
    }

    /*��ȡ���õĲ�����*/
    ulOperBandwidth = pstSetIps->oper_port_bandwidth;
    ulOperQueBandwidth = pstSetIps->oper_queue_bandwidth;

    /*��ȡ���õ�ƫ�Ƶ�ַ*/
    pucBandwidthDataStart = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->port_bandwidth_data);
    pucQueBandwidthDataStart = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->queue_bandwidth_data);
    pstBandwitchData         = (ATG_DCI_PORT_BANDWIDTH*)pucBandwidthDataStart;
    pstQueBandwitchData      = (ATG_DCI_QUEUE_BANDWIDTH*)pucQueBandwidthDataStart;
    
    ret = spm_set_control_bandwidth_cfg(ulOperBandwidth,pstBandwitchData);
    if(ATG_DCI_RC_OK != ret)
    {
        pstSetIps->port_return_code = ret;
        pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
    }
    
    for(i = 0; i< pstSetIps->queue_bandwidth_num; i++)
    {
        pstQueBandwitchData = (ATG_DCI_QUEUE_BANDWIDTH*)(pucQueBandwidthDataStart +         \
                (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_QUEUE_BANDWIDTH))) * i);
        ret = spm_set_control_que_bandwidth_cfg(ulOperQueBandwidth,pstQueBandwitchData);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->queue_return_code[i] = ret;
            pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
        }
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}

