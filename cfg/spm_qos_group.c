/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_group.c
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

/* ȫ�ִ�ӡ����qos_log_cfg_print */
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
NBB_LONG spm_disconnect_usr_group_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_USER_GROUP_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_usr_group_cnt err : ret = %ld, "
                  "usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_usr_group_cnt err : ret = %ld, "
                   "usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.qos_user_group_tree, &index);
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_usr_group_cnt err : can't find usr group cfg. "
                  "ret=%ld, usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_usr_group_cnt err : can't find usr group cfg. "
                   "ret=%ld, usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    if (0 == pcb->cnt)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_usr_group_cnt err : usr group cnt is 0. "
                  "ret=%ld, usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_usr_group_cnt err : usr group cnt is 0. "
                   "ret=%ld, usr_group_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        goto EXIT_LABEL;
    }
    (pcb->cnt)--;

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
NBB_LONG spm_set_up_user_group_cfg(NBB_ULONG oper, ATG_DCI_USER_GROUP_UP_QOS **pcb, 
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_USER_GROUP_UP_QOS *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ��Ϣ���� */
    switch (oper)
    {
        /* ���� */
        case  ATG_DCI_OPER_UPDATE:
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        break;

        /* ɾ�� */
        case  ATG_DCI_OPER_DEL:
        NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        break;

        /* ���� */
        case  ATG_DCI_OPER_ADD:

        /* ips��Ϣ����Ϊ�� */
        if (NULL == buf)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }

        /* ��Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_USER_GROUP_UP_QOS *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_USER_GROUP_UP_QOS *)NBB_MM_ALLOC(sizeof(ATG_DCI_USER_GROUP_UP_QOS),
                        NBB_NORETRY_ACT, MEM_SPM_UP_USER_GROUP_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_USER_GROUP_UP_QOS));
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
NBB_VOID spm_rcv_dci_set_user_group(ATG_DCI_SET_USER_GROUP *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_QOS_USER_GROUP_CB *pstCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;

    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucUpDataStart = NULL;
    NBB_BYTE *pucDownDataStart = NULL;

    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperUp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperDown = ATG_DCI_OPER_NULL;

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
    pstCb = AVLL_FIND(SHARED.qos_user_group_tree, &ulkey);
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }

    ulOperBasic = pstSetIps->oper_basic;
    ulOperUp = pstSetIps->oper_up_qos;
    ulOperDown = pstSetIps->oper_down_qos;
    pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->basic_data);
    pucUpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->up_qos_data);
    pucDownDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->down_qos_data);

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
            AVLL_DELETE(SHARED.qos_user_group_tree, pstCb->spm_qos_user_group_node);
            spm_free_user_group_cb(pstCb NBB_CCXT);
        }
    }

    /* ���»������� */
    else
    {
        /* �����Ŀ������,�������ڴ�ռ䱣������ */
        if (ucIfExist == QOS_UNEXIST)
        {
            pstCb = spm_alloc_user_group_cb(ulkey NBB_CCXT);
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }

        /* ������ */
        if ((NULL != pstCb) && (0 != pstCb->cnt))
        {
            pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
            goto EXIT_LABEL;
        }

/*
        if(NULL == pstCb)
        {
            goto EXIT_LABEL;
        }
*/

        /***************************************************************************/
        /*                                 �ϻ��û�������                          */
        /***************************************************************************/
        ret = spm_set_up_user_group_cfg(ulOperUp, &(pstCb->up_cfg_cb), pucUpDataStart NBB_CCXT);
        pstSetIps->up_qos_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                              �ϻ��û�������                             */
        /***************************************************************************/
        ret = spm_set_down_user_group_cfg(ulOperDown, pstCb, pucDownDataStart NBB_CCXT);
        pstSetIps->down_qos_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        pstSetIps->return_code = ips_ret;

        /* �������������Ŀ,���뵽���� */
        if (QOS_UNEXIST == ucIfExist)
        {
            pstCb->user_group_key = ulkey;
            
            //coverity[no_effect_test]
            rv = AVLL_INSERT(SHARED.qos_user_group_tree, pstCb->spm_qos_user_group_node);
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
NBB_VOID spm_qos_clear_all_usr_group(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_USER_GROUP_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_USER_GROUP_CB*) AVLL_FIRST(v_spm_shared->qos_user_group_tree);cfg_cb != NULL;
        cfg_cb = (SPM_QOS_USER_GROUP_CB*) AVLL_FIRST(v_spm_shared->qos_user_group_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_user_group_tree, cfg_cb->spm_qos_user_group_node);
        spm_free_user_group_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}
