/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_hqos_slot.c
   �� �� ��   : ����
   ��    ��   : zenglu
   ��������   : 2012��9��18��
   ����޸�   :
   ��������   : QOS����
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��9��18��
    ��    ��   : zenglu
    �޸�����   : �����ļ�
******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/* ȫ�ִ�ӡ����qos_acl_action_print */
extern NBB_BYTE qos_acl_action_print;

/* ȫ�ִ�ӡ����qos_acl_action_print */
extern NBB_BYTE qos_log_cfg_print;


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
SPM_HQOS_SLOT_CB *spm_alloc_hqos_slot_cb(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_SLOT_CB *pstTbl = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    pstTbl = (SPM_HQOS_SLOT_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_SLOT_CB),
              NBB_NORETRY_ACT, MEM_SPM_HQOS_CB);
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstTbl, 0, sizeof(SPM_HQOS_SLOT_CB));
    pstTbl->index = 1;
    AVLL_INIT_NODE(pstTbl->spm_hqos_slot_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return pstTbl;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_free_hqos_slot_cb(SPM_HQOS_SLOT_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != pstTbl);

    if (NULL == pstTbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_HQOS_SLOT_CB), MEM_SPM_HQOS_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    if (NULL != pstTbl->data)
    {
        NBB_MM_FREE(pstTbl->data, MEM_SPM_HQOS_SLOT_CB);
        pstTbl->data = NULL;
    }

    NBB_MM_FREE(pstTbl, MEM_SPM_HQOS_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_hqos_slot_cfg(NBB_ULONG oper, ATG_DCI_HQOS_SLOT_DATA **pcb, 
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_HQOS_SLOT_DATA *ptemp = NULL;
    NBB_BYTE i = 0;
    NBB_BYTE hqos_slot = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips��Ϣ���� */
    switch (oper)
    {
        /* ���� */
        case  ATG_DCI_OPER_UPDATE:
        break;

        /* ɾ�� */
        case  ATG_DCI_OPER_DEL:
        break;

        /* ���� */
        case  ATG_DCI_OPER_ADD:

        /* ��Ϣ����Ϊ�� */
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_HQOS_SLOT_DATA *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_HQOS_SLOT_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_HQOS_SLOT_DATA),
                        NBB_NORETRY_ACT, MEM_SPM_HQOS_SLOT_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_HQOS_SLOT_DATA));
            for (i = 0; i < ATG_DCI_HQOS_SLOT_NUM; i++)
            {
                if ((0 != ptemp->enable_switch[i].enable) && (hqos_slot < MAX_HQOS_SLOT_NUM))
                {
                    g_hqos_port_index[hqos_slot++] = i + 1;
                }
            }
        }
        break;

        default:
        break;
    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_hqos(ATG_DCI_SET_HQOS *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_HQOS_SLOT_CB *pstCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_BYTE *pucDataStart = NULL;
    NBB_ULONG ulOper = ATG_DCI_OPER_NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }

    pstSetIps->return_code = ATG_DCI_RC_OK;
    ulkey = pstSetIps->key;
    if (1 != ulkey)
    {
        pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    pstCb = AVLL_FIND(SHARED.hqos_tree, &ulkey);
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }

    ulOper = pstSetIps->oper_hqos_slot;
    pucDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->hqos_slot_data);

    /* ips��ϢΪɾ�� */
    if (TRUE == pstSetIps->delete_struct)
    {
        /* �����Ŀ������,��ɾ�� */
        if (ucIfExist == QOS_UNEXIST)
        {
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }

        /* ����,ɾ�� */
        else
        {
            if(NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }

            /* ������ */
            if (0 != pstCb->cnt)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }

            AVLL_DELETE(SHARED.hqos_tree, pstCb->spm_hqos_slot_node);
            ret = spm_free_hqos_slot_cb(pstCb NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }

    }

    /* ���ӻ��߸��� */
    else
    {

        /* �����Ŀ������,�������ڴ�ռ䱣������ */
        if (ucIfExist == QOS_UNEXIST)
        {
            pstCb = spm_alloc_hqos_slot_cb(NBB_CXT);
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }

        /* ������ */
        if ((NULL != pstCb) && (0 != pstCb->cnt))
        {
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        
/*
        if(NULL == pstCb)
        {
            goto EXIT_LABEL;
        }
*/

        ret = spm_set_hqos_slot_cfg(ulOper, &(pstCb->data) , pucDataStart NBB_CCXT);
        pstSetIps->hqos_slot_return_code = ret;

        /* �������������Ŀ,���뵽���� */
        if (QOS_UNEXIST == ucIfExist)
        {
            pstCb->index = ulkey;

            rv = AVLL_INSERT(SHARED.hqos_tree, pstCb->spm_hqos_slot_node);
        }
    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_clear_all_hqos_slot(NBB_CXT_T NBB_CXT)
{
    SPM_HQOS_SLOT_CB *cfg_cb = NULL;


    for(cfg_cb = (SPM_HQOS_SLOT_CB*) AVLL_FIRST(v_spm_shared->hqos_tree); cfg_cb != NULL;
        cfg_cb = (SPM_HQOS_SLOT_CB*) AVLL_FIRST(v_spm_shared->hqos_tree))
    {
        AVLL_DELETE(v_spm_shared->hqos_tree, cfg_cb->spm_hqos_slot_node);
        spm_free_hqos_slot_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}

