/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_classify.c
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
extern NBB_BYTE acl_pri_setting;

/* ȫ�ִ�ӡ����acl_pri_setting */
extern NBB_BYTE qos_acl_action_print;

/* ȫ�ִ�ӡ����acl_pri_setting */
extern NBB_BYTE qos_log_cfg_print;


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
SPM_QOS_CLASSIFY_CB *spm_qos_find_classify_cb(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_CLASSIFY_CB *pcb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.qos_classify_tree, &index);
    if (NULL == pcb)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return pcb;
}

/*****************************************************************************
   �� �� ��  : spm_connect_classify_cnt
   ��������  : classify���ü�����1
   �������  : classify��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_connect_classify_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_CLASSIFY_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_connect_classify_cnt err : ret = %ld, "
                  "classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_connect_classify_cnt err : ret = %ld, "
                   "classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.qos_classify_tree, &index);
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_connect_classify_cnt err : can't find classify cfg. "
                  "ret = %ld, classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_connect_classify_cnt err : can't find classify cfg. "
                   "ret = %ld, classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    if (NULL == pcb->basic_cfg_cb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_connect_classify_cnt err : basic cfg is NULL. "
                  "ret = %ld, classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_connect_classify_cnt err : basic cfg is NULL. "
                   "ret = %ld, classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    (pcb->cnt)++;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_classify_cnt
   ��������  : classify���ü�����1
   �������  : classify��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_disconnect_classify_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_CLASSIFY_CB *pcb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_classify_cnt err : ret=%ld, "
                  "classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_classify_cnt err : ret=%ld, "
                   "classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    for(pcb = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((SHARED.g_classify_id_instance[index].classify_group));
        pcb != NULL; pcb = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(pcb->spm_classify_group_node))
    {
        if (NULL == pcb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_disconnect_classify_cnt err : basic cfg is NULL. "
                      "ret=%ld, classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
            OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_classify_cnt err : basic cfg is NULL. "
                       "ret=%ld, classify_index = %lud\n\n",__FUNCTION__,__LINE__,ret,index);
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
            printf("%s line=%d spm_disconnect_classify_cnt err : classify cnt is 0. "
                      "ret=%ld, classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
            OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_classify_cnt err : classify cnt is 0. "
                       "ret=%ld, classify_index = %lu\n\n",__FUNCTION__,__LINE__,ret,index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
        (pcb->cnt)--;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#if 1


#ifdef PTN690

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
SPM_QOS_POLICY_CLASSIFY_CB* spm_alloc_policy_classify_cb(
    SPM_QOS_POLICY_CLASSIFY_KEY* pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_CLASSIFY_CB *pcb = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pkey)
    {
        goto EXIT_LABEL;
    }
    pcb = (SPM_QOS_POLICY_CLASSIFY_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_POLICY_CLASSIFY_CB),
              NBB_NORETRY_ACT,MEM_SPM_CLASSIFY_POLICY_CB);
    if(NULL == pcb)
    {
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(pcb, 0, sizeof(SPM_QOS_POLICY_CLASSIFY_CB));
    OS_MEMCPY(&(pcb->policy_classify_key), pkey, sizeof(SPM_QOS_POLICY_CLASSIFY_KEY));
    AVLL_INIT_NODE(pcb->tree_node);
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pcb);
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_free_policy_classify_cb(SPM_QOS_POLICY_CLASSIFY_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    //SPM_QOS_POLICY_CLASSIFY_CB *pcb = NULL;
    
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
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_POLICY_CLASSIFY_CB), MEM_SPM_CLASSIFY_POLICY_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_CLASSIFY_POLICY_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_VOID spm_del_policy_classify_mem(SPM_QOS_CLASSIFY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL != pcb)
    {
        for(cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pcb->spm_classify_policy_tree); 
            cfg_cb != NULL;
            cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pcb->spm_classify_policy_tree))
        {
            AVLL_DELETE(pcb->spm_classify_policy_tree, cfg_cb->tree_node);
            spm_free_policy_classify_cb(cfg_cb NBB_CCXT); 
        }
    }

    NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_VOID spm_del_policy_classify_drive(SPM_QOS_CLASSIFY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL != pcb)
    {
        for(cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pcb->spm_classify_policy_tree); cfg_cb != NULL;
            cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(pcb->spm_classify_policy_tree,cfg_cb->tree_node))
        {
            
        }
    }

    NBB_TRC_EXIT();
    return;
}

#endif


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_qos_apply_classfiy_rule_id(NBB_ULONG *pid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    
    //NBB_ULONG cnt = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ����������ȫ��rule ID */
    for (id = 0; id < MAX_FLOW_CLASSIFY_RULE_ID; id++)
    {
        if (0 == SHARED.flow_rule_id[id])
        {
            *pid = id + 1;
            SHARED.flow_rule_id[id] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;
            goto EXIT_LABEL;
        }
    }
    ret = ATG_DCI_RC_UNSUCCESSFUL;
    printf("%s line=%d spm_qos_apply_classfiy_rule_id err : ret=%lu, "
              "classfiy_rule_Id=%lu\n\n",__FUNCTION__,__LINE__,ret,*pid);
    OS_SPRINTF(ucMessage,"%s line=%d spm_qos_apply_classfiy_rule_id err : ret=%lu, "
               "classfiy_rule_Id=%lu\n\n",__FUNCTION__,__LINE__,ret,*pid);
    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
			       QOS_ERROR_STRING,ret,
			       ucMessage,__FUNCTION__,"LINE","",
			       0,0,__LINE__,0)); 

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_qos_free_classfiy_rule_id(NBB_ULONG pid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_ULONG id = 0;
    NBB_ULONG ret = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == pid)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_qos_free_classfiy_rule_id err : ret=%lu, "
                  "classify_rule_id=%lu\n\n",__FUNCTION__,__LINE__,ret,pid);
        OS_SPRINTF(ucMessage,"%s line=%d spm_qos_free_classfiy_rule_id err : ret=%lu, "
                   "classfiy_rule_Id=%lu\n\n",__FUNCTION__,__LINE__,ret,pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    if (0 != SHARED.flow_rule_id[pid - 1])
    {
        SHARED.flow_rule_id[pid - 1] = 0;
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_qos_free_classfiy_rule_id err : classfiy_rule_Id is not in use. "
                  "ret=%lu, classfiy_rule_Id=%lu\n\n",__FUNCTION__,__LINE__,ret,pid);
        OS_SPRINTF(ucMessage,"%s line=%d spm_qos_free_classfiy_rule_id err : classfiy_rule_Id is not in use. "
                   "ret=%lu, classfiy_rule_Id=%lu\n\n",__FUNCTION__,__LINE__,ret,pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
SPM_QOS_CLASSIFY_CB *spm_alloc_classify_cb(ATG_DCI_QOS_CLASSIFY_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_CLASSIFY_CB *pstTbl = NULL;
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pkey)
    {
        printf("ATG_DCI_QOS_CLASSIFY_KEY == NULL\n");
        goto EXIT_LABEL;
    }

    pstTbl = (SPM_QOS_CLASSIFY_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_CLASSIFY_CB),
              NBB_NORETRY_ACT,MEM_SPM_CLASSIFY_CB);
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_CLASSIFY_CB));
    OS_MEMCPY(&(pstTbl->classify_key),pkey,sizeof(ATG_DCI_QOS_CLASSIFY_KEY));
    AVLL_INIT_NODE(pstTbl->spm_classify_node);
    NBB_INIT_LQE(pstTbl->spm_classify_group_node, pstTbl);

#ifdef PTN690
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_POLICY_CLASSIFY_CB, policy_classify_key);
    AVLL_INIT_TREE(pstTbl->spm_classify_policy_tree,spm_qos_policy_classify_comp,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_POLICY_CLASSIFY_CB,tree_node));
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_free_classify_cb(SPM_QOS_CLASSIFY_CB *pstTbl NBB_CCXT_T NBB_CXT)
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
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_CLASSIFY_CB), MEM_SPM_CLASSIFY_CB);

#ifdef PTN690

   /*************************************************** 
    * ��ɾ��classifyģ��֮ǰ,��Ҫ��ɾ��classifyģ���� *
    * ����������(spm_classify_policy_tree)�еĽڵ�. *
    ***************************************************/
    spm_del_policy_classify_mem(pstTbl NBB_CCXT);
#endif

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (NULL != pstTbl->basic_cfg_cb)
    {
        pstTbl->match_type = MATCH_ERROR;
        NBB_MM_FREE(pstTbl->basic_cfg_cb, MEM_SPM_CLASSIFY_BASIC_CB);
        pstTbl->basic_cfg_cb = NULL;
    }
    if (NULL != pstTbl->Port_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->Port_cfg_cb, MEM_SPM_CLASSIFY_PORT_CB);
        pstTbl->Port_cfg_cb = NULL;
    }
    if (NULL != pstTbl->Eth_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->Eth_cfg_cb, MEM_SPM_CLASSIFY_ETH_CB);
        pstTbl->Eth_cfg_cb = NULL;
    }
    if (NULL != pstTbl->ipv4_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv4_cfg_cb, MEM_SPM_CLASSIFY_IPV4_CB);
        pstTbl->ipv4_cfg_cb = NULL;
    }
    if (NULL != pstTbl->ipTcp_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipTcp_cfg_cb, MEM_SPM_CLASSIFY_IPTCP_CB);
        pstTbl->ipTcp_cfg_cb = NULL;
    }
    if (NULL != pstTbl->ipv6_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv6_cfg_cb, MEM_SPM_CLASSIFY_IPV6_CB);
        pstTbl->ipv6_cfg_cb = NULL;
    }
    if (NULL != pstTbl->noIp_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->noIp_cfg_cb, MEM_SPM_CLASSIFY_NOIP_CB);
        pstTbl->noIp_cfg_cb = NULL;
    }
    if (NULL != pstTbl->ipUdp_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipUdp_cfg_cb, MEM_SPM_CLASSIFY_IPUDP_CB);
        pstTbl->ipUdp_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_CLASSIFY_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif 


#if 2

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_set_classify_basic_cfg(SPM_QOS_CLASSIFY_CB *pc,
    NBB_ULONG oper,
    ATG_DCI_QOS_CLASSIFY_BASIC_DATA **pcb,
    NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_BASIC_DATA *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            printf("%s line=%d spm_set_classify_basic_cfg err : buf is NULL. "
                      "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_classify_basic_cfg err : buf is NULL. "
                       "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_BASIC_DATA *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_BASIC_DATA),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_BASIC_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_BASIC_DATA));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_check_classify_id_cfg(NBB_LONG classify_id,NBB_BYTE match_type NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /* ƥ������Ϊ��,��ʾ��classifyid������ӵ� */
    if(MATCH_ERROR == SHARED.g_classify_id_instance[classify_id].match_type)
    {
        switch(match_type)
        {
           case MATCH_ETH_HEADER:
           SHARED.g_classify_id_instance[classify_id].match_type = MATCH_ETH_HEADER;
           break;

           case MATCH_ACL_ID:
           SHARED.g_classify_id_instance[classify_id].match_type = MATCH_ACL_ID;
           break;

           case MATCH_PORT:
           SHARED.g_classify_id_instance[classify_id].match_type = MATCH_PORT;
           break;  

           case MATCH_IPV4:

           case MATCH_IP_TCP:

           case MATCH_IP_UDP:

           case MATCH_IPV6:

           case MATCH_NOIP:
           SHARED.g_classify_id_instance[classify_id].match_type = MATCH_IP_TCP;
           break;

           default:
           {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("%s line=%d spm_check_classify_id_cfg err : match_type error. "
                          "ret = %ld, classify_id = %ld, match_type = %d\n\n",__FUNCTION__,
                          __LINE__,ret,classify_id,match_type);
                OS_SPRINTF(ucMessage,"%s line=%d spm_check_classify_id_cfg err : match_type error. "
                           "ret = %ld, classify_id = %ld, match_type = %d\n\n",__FUNCTION__,
                           __LINE__,ret,classify_id,match_type);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            			       QOS_ERROR_STRING,ret,
            			       ucMessage,__FUNCTION__,"LINE","",
            			       0,0,__LINE__,0)); 
                goto EXIT_LABEL;
           }
        }
    }
    else if(MATCH_ETH_HEADER == SHARED.g_classify_id_instance[classify_id].match_type)
    {
        if(MATCH_ETH_HEADER != match_type)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_check_classify_id_cfg err : match_type "
                      "isn't MATCH_ETH_HEADER. ret = %ld, classify_id = %ld, match_type = %d\n\n",
                      __FUNCTION__,__LINE__,ret,classify_id,match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_check_classify_id_cfg err : match_type "
                       "isn't MATCH_ETH_HEADER. ret = %ld, classify_id = %ld, match_type = %d\n\n",
                       __FUNCTION__,__LINE__,ret,classify_id,match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
    }
    else if(MATCH_IP_TCP == SHARED.g_classify_id_instance[classify_id].match_type)
    {
        if(MATCH_ETH_HEADER >= match_type)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_check_classify_id_cfg err : match_type "
                      "isn't MATCH_IP_HIGH. ret = %ld, classify_id = %ld, match_type = %d\n\n",
                      __FUNCTION__,__LINE__,ret,classify_id,match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_check_classify_id_cfg err : match_type "
                       "isn't MATCH_IP_HIGH. ret = %ld, classify_id = %ld, match_type = %d\n\n",
                       __FUNCTION__,__LINE__,ret,classify_id,match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
    }
    else if(MATCH_PORT == SHARED.g_classify_id_instance[classify_id].match_type)
    {
        if(MATCH_PORT != match_type)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_check_classify_id_cfg err : match_type "
                      "isn't MATCH_PORT. ret = %ld, classify_id = %ld, match_type = %d\n\n",
                      __FUNCTION__,__LINE__,ret,classify_id,match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_check_classify_id_cfg err : match_type "
                       "isn't MATCH_PORT. ret = %ld, classify_id = %ld, match_type = %d\n\n",
                       __FUNCTION__,__LINE__,ret,classify_id,match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


#ifdef PTN690

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_VOID spm_verify_policy_classify_mem(SPM_QOS_CLASSIFY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL != pcb)
    {
        for(cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pcb->spm_classify_policy_tree); cfg_cb != NULL;
            cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(pcb->spm_classify_policy_tree,cfg_cb->tree_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify policy classify cb %p", cfg_cb));
            NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_POLICY_CLASSIFY_CB),
                              MEM_SPM_CLASSIFY_POLICY_CB);
        }
    }

    NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_update_classify_port_cfg(SPM_QOS_CLASSIFY_CB *pstCb,
    NBB_ULONG oper,NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_PORT_DATA *ptemp = NULL;
    NBB_USHORT UsSvlan = 0;
    SPM_PORT_INFO_CB pstPortInfo = {0};
    ACL_RULE_T acl_rule;
    ACL_RULE_T *pstAcl = &acl_rule;
    NBB_USHORT unit = 0;
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;

    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

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
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ptemp = (ATG_DCI_QOS_CLASSIFY_PORT_DATA *)buf;
        OS_MEMSET(pstAcl,0,sizeof(ACL_RULE_T));

       /*************************************************************** 
        * ���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���� *
        * ���ڵ��ʾ��classify�������õ�policy����δ���󶨵�intf��. *
        ***************************************************************/
        cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstCb->spm_classify_policy_tree);
        if(NULL == cfg_cb)
        {
            if(NULL == pstCb->Port_cfg_cb)
            {
                pstCb->Port_cfg_cb = (ATG_DCI_QOS_CLASSIFY_PORT_DATA *)NBB_MM_ALLOC(      \
                        sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA), NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_PORT_CB);
                if (NULL == pstCb->Port_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pstCb->Port_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA));
        }
        else
        {
        /*
            if(NULL == ptemp)
            {
               printf("NULL == pstClassify->Port_cfg_cb");
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
        */
            OS_MEMSET(&pstPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
            ret = spm_get_portid_from_logical_port_index(ptemp->index, &pstPortInfo NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_get_portid_from_logical_port_index_error_log(ptemp->index,
                        __FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }

            /* VE�� */
            if((0 == pstPortInfo.slot_id) && (3 == pstPortInfo.port_type))
            {
                /* ����VE�� */
                if(0 == pstPortInfo.svlan)
                {
                   pstAcl->tAclKey.ePortType = ACL_L2VE;
                   pstAcl->tAclKey.port = pstPortInfo.port_id;
                   pstAcl->tAclMask.ePortType = 0xffff;
                   pstAcl->tAclMask.port      = 0xffff;
                }

                /* ����VE�� */
                else
                {
                   pstAcl->tAclKey.ePortType = ACL_L3VE;
                   pstAcl->tAclKey.port = pstPortInfo.port_id;
                   pstAcl->tAclMask.ePortType = 0xffff;
                   pstAcl->tAclMask.port      = 0xffff;
                }
            }

            /* ��ͨlogic��lag�� */
            else
            {
                /* ����� */
                if(0 == pstPortInfo.svlan)
                {
                    ret = spm_get_vlan_from_logical_port_index(ptemp->index, &UsSvlan);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_get_vlan_from_logical_port_index_error_log(ptemp->index,
                                __FUNCTION__,__LINE__,ret NBB_CCXT);
                        goto EXIT_LABEL;
                    }
                    if(0 != UsSvlan)
                    {
                        pstAcl->tAclKey.oVid  = UsSvlan;
                        pstAcl->tAclMask.oVid = 0x0fff;
                    }
                }

                /* ����� */
                else
                {
                    pstAcl->tAclKey.oVid  = pstPortInfo.svlan;
                    pstAcl->tAclMask.oVid = 0x0fff;
                }
                pstAcl->tAclKey.ePortType  = ACL_PORT;
                pstAcl->tAclKey.port       = pstPortInfo.port_id;
                pstAcl->tAclMask.ePortType = 0xffff;
                pstAcl->tAclMask.port      = 0xffff;     
            }

           /******************************************************************* 
            * 1.���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���ڽ� *
            *   ���ʾ��classify�������õ�policy�����Ѿ��󶨵���intf��,��ʱ *
            *   ��Ҫ���������������Ľӿڽ��µ�classify����д��acl������.      *
            * 2.Ŀǰclassify��ʵ�ַ�����:��ÿ������д��C3����acl������,д��� *
            *   ���������᷵��һ����classify��������Ӧ��posid.                *
            * 3.ÿ��acl������������������:posid(��Ӧclassify����)��behavior *
            *   ģ������Ӧ��meterͰģ��id��aclid+ruleid.                      *
            *******************************************************************/
            for(; cfg_cb != NULL; cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(
                   pstCb->spm_classify_policy_tree,cfg_cb->tree_node))
            {
                for(unit = 0;unit < SHARED.c3_num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = fhdrv_acl_update_rule(unit,cfg_cb->posid,pstAcl);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_acl_update_rule_error_log(unit,cfg_cb->posid,
                            cfg_cb->policy_classify_key.acl_id,cfg_cb->rule_id,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);
                        goto EXIT_LABEL;
                    }
#endif
                }
            }
            if(NULL != pstCb->Port_cfg_cb)
            {
                OS_MEMCPY(pstCb->Port_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_update_classify_eth_cfg(SPM_QOS_CLASSIFY_CB *pstCb,
    NBB_ULONG oper,NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_ETH_HEADER *ptemp = NULL;
    ACL_RULE_T acl_rule;
    ACL_RULE_T *pstAcl = &acl_rule;
    NBB_USHORT unit = 0;
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;

    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

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
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ptemp = (ATG_DCI_QOS_CLASSIFY_ETH_HEADER *)buf;
        OS_MEMSET(pstAcl,0,sizeof(ACL_RULE_T));

       /*************************************************************** 
        * ���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���� *
        * ���ڵ��ʾ��classify�������õ�policy����δ���󶨵�intf��. *
        ***************************************************************/
        cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstCb->spm_classify_policy_tree);
        if(NULL == cfg_cb)
        {
            if(NULL == pstCb->Eth_cfg_cb)
            {
                pstCb->Eth_cfg_cb = (ATG_DCI_QOS_CLASSIFY_ETH_HEADER *)NBB_MM_ALLOC(      \
                        sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER), NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_ETH_CB);
                if (NULL == pstCb->Eth_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pstCb->Eth_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER));
        }
        else
        {
         /*
            if(NULL == ptemp)
            {
               printf("NULL == pstClassify->Eth_cfg_cb");
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
         */
            OS_MEMCPY(&(pstAcl->tAclKey.smac[0]), &(ptemp->src_mac[0]), ATG_DCI_MAC_LEN);
            OS_MEMCPY(&(pstAcl->tAclKey.dmac[0]), &(ptemp->dst_mac[0]), ATG_DCI_MAC_LEN);
            OS_MEMCPY(&(pstAcl->tAclMask.smac[0]), &(ptemp->src_mac_mask[0]), ATG_DCI_MAC_LEN);
            OS_MEMCPY(&(pstAcl->tAclMask.dmac[0]), &(ptemp->dst_mac_mask[0]), ATG_DCI_MAC_LEN);
            pstAcl->tAclKey.ovlan_pri = ptemp->vlan_prio;
            pstAcl->tAclMask.ovlan_pri = ptemp->vlan_prio_mask;
            pstAcl->tAclKey.ethType  = 0x0800;
            pstAcl->tAclMask.ethType = 0xffff;

           /******************************************************************* 
            * 1.���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���ڽ� *
            *   ���ʾ��classify�������õ�policy�����Ѿ��󶨵���intf��,��ʱ *
            *   ��Ҫ���������������Ľӿڽ��µ�classify����д��acl������.      *
            * 2.Ŀǰclassify��ʵ�ַ�����:��ÿ������д��C3����acl������,д��� *
            *   ���������᷵��һ����classify��������Ӧ��posid.                *
            * 3.ÿ��acl������������������:posid(��Ӧclassify����)��behavior *
            *   ģ������Ӧ��meterͰģ��id��aclid+ruleid.                      *
            *******************************************************************/
            for(;cfg_cb != NULL;cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(
                    pstCb->spm_classify_policy_tree,cfg_cb->tree_node))
            {
                
                for(unit = 0;unit < SHARED.c3_num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = fhdrv_acl_update_rule(unit,cfg_cb->posid,pstAcl);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_acl_update_rule_error_log(unit,cfg_cb->posid,
                            cfg_cb->policy_classify_key.acl_id,cfg_cb->rule_id,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);
                        goto EXIT_LABEL;
                    }
#endif
                }
            }
            if(NULL != pstCb->Eth_cfg_cb)
            {
                OS_MEMCPY(pstCb->Eth_cfg_cb, ptemp,
                    sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_update_classify_ipv4_cfg(SPM_QOS_CLASSIFY_CB *pstCb,
    NBB_ULONG oper,NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *ptemp = NULL;
    ACL_RULE_T acl_rule;
    ACL_RULE_T *pstAcl = &acl_rule;
    NBB_USHORT unit = 0;
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;

    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

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
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ptemp = (ATG_DCI_QOS_CLASSIFY_IPV4_FRAME*)buf;
        OS_MEMSET(pstAcl,0,sizeof(ACL_RULE_T));

       /*************************************************************** 
        * ���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���� *
        * ���ڵ��ʾ��classify�������õ�policy����δ���󶨵�intf��. *
        ***************************************************************/
        cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstCb->spm_classify_policy_tree);
        if(NULL == cfg_cb)
        {
            if(NULL == pstCb->ipv4_cfg_cb)
            {
                pstCb->ipv4_cfg_cb = (ATG_DCI_QOS_CLASSIFY_IPV4_FRAME*)NBB_MM_ALLOC(      \
                        sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME), NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPV4_CB);
                if (NULL == pstCb->ipv4_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pstCb->ipv4_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME));
        }
        else
        {
         /*
            if(NULL == ptemp)
            {
               printf("NULL == pstClassify->ipv4_cfg_cb");
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
         */
            pstAcl->tAclKey.dip = ptemp->dst_ip;
            pstAcl->tAclMask.dip = ptemp->dst_ip_mask;
            pstAcl->tAclKey.sip = ptemp->src_ip;
            pstAcl->tAclMask.sip = ptemp->src_ip_mask;
            pstAcl->tAclKey.tos = ptemp->dscp;
            pstAcl->tAclMask.tos = ptemp->dscp_mask;
            pstAcl->tAclKey.l3Protocol = ptemp->l3_protocol;
            pstAcl->tAclMask.l3Protocol = ptemp->l3_protocol_mask;
            pstAcl->tAclKey.ethType = 0x0800;
            pstAcl->tAclMask.ethType = 0xffff;
            pstAcl->tAclKey.ipHeaderMF = ptemp->mf;
            pstAcl->tAclMask.ipHeaderMF = ptemp->mf_mask;
            pstAcl->tAclKey.ipHeaderDF = ptemp->df;
            pstAcl->tAclMask.ipHeaderDF = ptemp->df_mask;
            pstAcl->tAclKey.l4SrcPort = ptemp->src_port;
            pstAcl->tAclMask.l4SrcPort = ptemp->src_port_mask;
            pstAcl->tAclKey.l4DstPort = ptemp->dst_port;
            pstAcl->tAclMask.l4DstPort = ptemp->dst_port_mask;
            
            //pstAcl->flags |= ACL_COUNTER_ON;
            if((1 == ptemp->l3_protocol)
                && (0xff == ptemp->l3_protocol_mask)
                && (0 != ptemp->payload_mask[0]))
            {
                pstAcl->tAclKey.icmpType = ptemp->payload[0];
                pstAcl->tAclMask.icmpType = ptemp->payload_mask[0];
            }
            if((1 == ptemp->l3_protocol)
                && (0xff == ptemp->l3_protocol_mask)
                && (0 != ptemp->payload_mask[1]))
            {
                pstAcl->tAclKey.icmpCode = ptemp->payload[1];
                pstAcl->tAclMask.icmpCode = ptemp->payload_mask[1];
            }

           /******************************************************************* 
            * 1.���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���ڽ� *
            *   ���ʾ��classify�������õ�policy�����Ѿ��󶨵���intf��,��ʱ *
            *   ��Ҫ���������������Ľӿڽ��µ�classify����д��acl������.      *
            * 2.Ŀǰclassify��ʵ�ַ�����:��ÿ������д��C3����acl������,д��� *
            *   ���������᷵��һ����classify��������Ӧ��posid.                *
            * 3.ÿ��acl������������������:posid(��Ӧclassify����)��behavior *
            *   ģ������Ӧ��meterͰģ��id��aclid+ruleid.                      *
            *******************************************************************/
            for(;cfg_cb != NULL;cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(
                    pstCb->spm_classify_policy_tree,cfg_cb->tree_node))
            {
                
                for(unit = 0;unit < SHARED.c3_num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = fhdrv_acl_update_rule(unit,cfg_cb->posid,pstAcl);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_acl_update_rule_error_log(unit,cfg_cb->posid,
                            cfg_cb->policy_classify_key.acl_id,cfg_cb->rule_id,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);
                        goto EXIT_LABEL;
                    }
#endif
                }
            }
            if(NULL != pstCb->ipv4_cfg_cb)
            {
                OS_MEMCPY(pstCb->ipv4_cfg_cb, ptemp,
                    sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_update_classify_iptcp_cfg(SPM_QOS_CLASSIFY_CB *pstCb,
    NBB_ULONG oper,NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_TCP *ptemp = NULL;
    ACL_RULE_T acl_rule;
    ACL_RULE_T *pstAcl = &acl_rule;
    NBB_BYTE tcp_flag = 0;
    NBB_USHORT unit = 0;
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;

    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

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
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ptemp = (ATG_DCI_QOS_CLASSIFY_IPV4_TCP*)buf;
        OS_MEMSET(pstAcl,0,sizeof(ACL_RULE_T));

       /*************************************************************** 
        * ���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���� *
        * ���ڵ��ʾ��classify�������õ�policy����δ���󶨵�intf��. *
        ***************************************************************/
        cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstCb->spm_classify_policy_tree);
        if(NULL == cfg_cb)
        {
            if(NULL == pstCb->ipTcp_cfg_cb)
            {
              pstCb->ipTcp_cfg_cb = (ATG_DCI_QOS_CLASSIFY_IPV4_TCP*)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPTCP_CB);
                if (NULL == pstCb->ipTcp_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pstCb->ipTcp_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP));
        }
        else
        {
         /*
            if(NULL == ptemp)
            {
               printf("NULL == pstClassify->ipTcp_cfg_cb");
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
         */
            pstAcl->tAclKey.dip = ptemp->dst_ip;
            pstAcl->tAclMask.dip = ptemp->dst_ip_mask;
            pstAcl->tAclKey.sip = ptemp->src_ip;
            pstAcl->tAclMask.sip = ptemp->src_ip_mask;
            pstAcl->tAclKey.tos = (ptemp->dscp);
            pstAcl->tAclMask.tos = (ptemp->dscp_mask);
            pstAcl->tAclKey.l3Protocol = 6;
            pstAcl->tAclMask.l3Protocol = 0xff;
            pstAcl->tAclKey.ethType = 0x0800;
            pstAcl->tAclMask.ethType = 0xffff;
            pstAcl->tAclKey.l4SrcPort = ptemp->src_port;
            pstAcl->tAclMask.l4SrcPort = ptemp->src_port_mask;
            pstAcl->tAclKey.l4DstPort = ptemp->dst_port;
            pstAcl->tAclMask.l4DstPort = ptemp->dst_port_mask; 
            OS_MEMCPY(&tcp_flag,&(ptemp->tcp_flag),1);
            pstAcl->tAclKey.tcp_flag = tcp_flag;
            pstAcl->tAclMask.tcp_flag = ptemp->tcp_flag_mask;
            
           /******************************************************************* 
            * 1.���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���ڽ� *
            *   ���ʾ��classify�������õ�policy�����Ѿ��󶨵���intf��,��ʱ *
            *   ��Ҫ���������������Ľӿڽ��µ�classify����д��acl������.      *
            * 2.Ŀǰclassify��ʵ�ַ�����:��ÿ������д��C3����acl������,д��� *
            *   ���������᷵��һ����classify��������Ӧ��posid.                *
            * 3.ÿ��acl������������������:posid(��Ӧclassify����)��behavior *
            *   ģ������Ӧ��meterͰģ��id��aclid+ruleid.                      *
            *******************************************************************/
            for(;cfg_cb != NULL;cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(
                    pstCb->spm_classify_policy_tree,cfg_cb->tree_node))
            {
                
                for(unit = 0;unit < SHARED.c3_num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = fhdrv_acl_update_rule(unit,cfg_cb->posid,pstAcl);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_acl_update_rule_error_log(unit,cfg_cb->posid,
                            cfg_cb->policy_classify_key.acl_id,cfg_cb->rule_id,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);
                        goto EXIT_LABEL;
                    }
#endif
                }
            }
            if(NULL != pstCb->ipTcp_cfg_cb)
            {
                OS_MEMCPY(pstCb->ipTcp_cfg_cb, ptemp,
                    sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_update_classify_ipudp_cfg(SPM_QOS_CLASSIFY_CB *pstCb,
    NBB_ULONG oper,NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_UDP *ptemp = NULL;
    ACL_RULE_T acl_rule;
    ACL_RULE_T *pstAcl = &acl_rule;
    NBB_USHORT unit = 0;
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;

    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

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
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ptemp = (ATG_DCI_QOS_CLASSIFY_IPV4_UDP*)buf;
        OS_MEMSET(pstAcl,0,sizeof(ACL_RULE_T));

       /*************************************************************** 
        * ���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���� *
        * ���ڵ��ʾ��classify�������õ�policy����δ���󶨵�intf��. *
        ***************************************************************/
        cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstCb->spm_classify_policy_tree);
        if(NULL == cfg_cb)
        {
            if(NULL == pstCb->ipUdp_cfg_cb)
            {
              pstCb->ipUdp_cfg_cb = (ATG_DCI_QOS_CLASSIFY_IPV4_UDP*)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPUDP_CB);
                if (NULL == pstCb->ipUdp_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pstCb->ipUdp_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP));
        }
        else
        {
         /*
            if(NULL == ptemp)
            {
               printf("NULL == pstClassify->ipUdp_cfg_cb");
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
         */
            pstAcl->tAclKey.ethType = 0x0800;
            pstAcl->tAclMask.ethType = 0xffff;
            pstAcl->tAclKey.dip = ptemp->dst_ip;
            pstAcl->tAclMask.dip = ptemp->dst_ip_mask;
            pstAcl->tAclKey.sip = ptemp->src_ip;
            pstAcl->tAclMask.sip = ptemp->src_ip_mask;
            pstAcl->tAclKey.tos = (ptemp->dscp);
            pstAcl->tAclMask.tos = (ptemp->dscp_mask);
            pstAcl->tAclKey.l3Protocol = 0x11;
            pstAcl->tAclMask.l3Protocol = 0xff;
            pstAcl->tAclKey.l4SrcPort = ptemp->src_port;
            pstAcl->tAclMask.l4SrcPort = ptemp->src_port_mask;
            pstAcl->tAclKey.l4DstPort = ptemp->dst_port;
            pstAcl->tAclMask.l4DstPort = ptemp->dst_port_mask;
            
           /******************************************************************* 
            * 1.���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���ڽ� *
            *   ���ʾ��classify�������õ�policy�����Ѿ��󶨵���intf��,��ʱ *
            *   ��Ҫ���������������Ľӿڽ��µ�classify����д��acl������.      *
            * 2.Ŀǰclassify��ʵ�ַ�����:��ÿ������д��C3����acl������,д��� *
            *   ���������᷵��һ����classify��������Ӧ��posid.                *
            * 3.ÿ��acl������������������:posid(��Ӧclassify����)��behavior *
            *   ģ������Ӧ��meterͰģ��id��aclid+ruleid.                      *
            *******************************************************************/
            for(;cfg_cb != NULL;cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(
                    pstCb->spm_classify_policy_tree,cfg_cb->tree_node))
            {
                
                for(unit = 0;unit < SHARED.c3_num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = fhdrv_acl_update_rule(unit,cfg_cb->posid,pstAcl);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_acl_update_rule_error_log(unit,cfg_cb->posid,
                            cfg_cb->policy_classify_key.acl_id,cfg_cb->rule_id,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);
                        goto EXIT_LABEL;
                    }
#endif
                }
            }
            if(NULL != pstCb->ipUdp_cfg_cb)
            {
                OS_MEMCPY(pstCb->ipUdp_cfg_cb, ptemp,
                    sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_update_classify_ipv6_cfg(SPM_QOS_CLASSIFY_CB *pstCb,
    NBB_ULONG oper,NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *ptemp = NULL;
    ACL_RULE_T acl_rule;
    ACL_RULE_T *pstAcl = &acl_rule;
    NBB_USHORT unit = 0;
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;

    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

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
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ptemp = (ATG_DCI_QOS_CLASSIFY_IPV6_FRAME*)buf;
        OS_MEMSET(pstAcl,0,sizeof(ACL_RULE_T));

       /*************************************************************** 
        * ���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���� *
        * ���ڵ��ʾ��classify�������õ�policy����δ���󶨵�intf��. *
        ***************************************************************/
        cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstCb->spm_classify_policy_tree);
        if(NULL == cfg_cb)
        {
            if(NULL == pstCb->ipv6_cfg_cb)
            {
                pstCb->ipv6_cfg_cb = (ATG_DCI_QOS_CLASSIFY_IPV6_FRAME*)NBB_MM_ALLOC(    \
                        sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME), NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPV6_CB);
                if (NULL == pstCb->ipv6_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pstCb->ipv6_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME));
        }
        else
        {
         /*
            if(NULL == ptemp)
            {
               printf("NULL == pstClassify->ipv6_cfg_cb");
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
         */
            pstAcl->tAclKey.ethType = 0x86dd;
            pstAcl->tAclMask.ethType = 0xffff;
            OS_MEMCPY(pstAcl->tAclKey.dipv6,ptemp->dst_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
            ret = spm_get_ipv6_mask(ptemp->dst_ip_mask_len,(NBB_ULONG*)&(pstAcl->tAclMask.dipv6) NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            OS_MEMCPY(pstAcl->tAclKey.sipv6,ptemp->src_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
            ret = spm_get_ipv6_mask(ptemp->src_ip_mask_len,(NBB_ULONG*)&(pstAcl->tAclMask.sipv6) NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            pstAcl->tAclKey.l3Protocol = ptemp->l3_protocol;
            pstAcl->tAclMask.l3Protocol = ptemp->l3_protocol_mask;
            pstAcl->tAclKey.l4SrcPort = ptemp->src_port;
            pstAcl->tAclMask.l4SrcPort = ptemp->src_port_mask;
            pstAcl->tAclKey.l4DstPort = ptemp->dst_port;
            pstAcl->tAclMask.l4DstPort = ptemp->dst_port_mask;
            pstAcl->tAclKey.tos = ptemp->tos;
            pstAcl->tAclMask.tos = ptemp->tos_mask;
            
           /******************************************************************* 
            * 1.���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���ڽ� *
            *   ���ʾ��classify�������õ�policy�����Ѿ��󶨵���intf��,��ʱ *
            *   ��Ҫ���������������Ľӿڽ��µ�classify����д��acl������.      *
            * 2.Ŀǰclassify��ʵ�ַ�����:��ÿ������д��C3����acl������,д��� *
            *   ���������᷵��һ����classify��������Ӧ��posid.                *
            * 3.ÿ��acl������������������:posid(��Ӧclassify����)��behavior *
            *   ģ������Ӧ��meterͰģ��id��aclid+ruleid.                      *
            *******************************************************************/
            for(;cfg_cb != NULL;cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(
                    pstCb->spm_classify_policy_tree,cfg_cb->tree_node))
            {
                
                for(unit = 0;unit < SHARED.c3_num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = fhdrv_acl_update_rule(unit,cfg_cb->posid,pstAcl);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_acl_update_rule_error_log(unit,cfg_cb->posid,
                            cfg_cb->policy_classify_key.acl_id,cfg_cb->rule_id,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);
                        goto EXIT_LABEL;
                    }
#endif
                }
            }
            if(NULL != pstCb->ipv6_cfg_cb)
            {
                OS_MEMCPY(pstCb->ipv6_cfg_cb, ptemp,
                    sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_update_classify_noip_cfg(SPM_QOS_CLASSIFY_CB *pstCb,
    NBB_ULONG oper,NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_NONE_IP *ptemp = NULL;
    ACL_RULE_T acl_rule;
    ACL_RULE_T *pstAcl = &acl_rule;
    NBB_USHORT unit = 0;
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;

    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

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
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ptemp = (ATG_DCI_QOS_CLASSIFY_NONE_IP*)buf;
        OS_MEMSET(pstAcl,0,sizeof(ACL_RULE_T));

       /*************************************************************** 
        * ���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���� *
        * ���ڵ��ʾ��classify�������õ�policy����δ���󶨵�intf��. *
        ***************************************************************/
        cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstCb->spm_classify_policy_tree);
        if(NULL == cfg_cb)
        {
            if(NULL == pstCb->noIp_cfg_cb)
            {
                pstCb->noIp_cfg_cb = (ATG_DCI_QOS_CLASSIFY_NONE_IP*)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_NOIP_CB);
                if (NULL == pstCb->noIp_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pstCb->noIp_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP));
        }
        else
        {
         /*
            if(NULL == ptemp)
            {
               printf("NULL == pstClassify->noIp_cfg_cb");
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
         */
            pstAcl->tAclKey.ethType = ptemp->etype;
            pstAcl->tAclMask.ethType = ptemp->etype_mask;
            OS_MEMCPY(&(pstAcl->tAclKey.customerData6),&(ptemp->payload[0]),4);
            OS_MEMCPY(&(pstAcl->tAclKey.customerData5),&(ptemp->payload[4]),4);
            OS_MEMCPY(&(pstAcl->tAclKey.customerData4),&(ptemp->payload[8]),4);
            OS_MEMCPY(&(pstAcl->tAclMask.customerData6),&(ptemp->payload_mask[0]),4);
            OS_MEMCPY(&(pstAcl->tAclMask.customerData5),&(ptemp->payload_mask[4]),4);
            OS_MEMCPY(&(pstAcl->tAclMask.customerData4),&(ptemp->payload_mask[8]),4);
            
           /******************************************************************* 
            * 1.���classify��(spm_classify_policy_tree)���Ƿ��нڵ�,���ڽ� *
            *   ���ʾ��classify�������õ�policy�����Ѿ��󶨵���intf��,��ʱ *
            *   ��Ҫ���������������Ľӿڽ��µ�classify����д��acl������.      *
            * 2.Ŀǰclassify��ʵ�ַ�����:��ÿ������д��C3����acl������,д��� *
            *   ���������᷵��һ����classify��������Ӧ��posid.                *
            * 3.ÿ��acl������������������:posid(��Ӧclassify����)��behavior *
            *   ģ������Ӧ��meterͰģ��id��aclid+ruleid.                      *
            *******************************************************************/
            for(;cfg_cb != NULL;cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(
                    pstCb->spm_classify_policy_tree,cfg_cb->tree_node))
            {
                
                for(unit = 0;unit < SHARED.c3_num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = fhdrv_acl_update_rule(unit,cfg_cb->posid,pstAcl);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_acl_update_rule_error_log(unit,cfg_cb->posid,
                            cfg_cb->policy_classify_key.acl_id,cfg_cb->rule_id,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);
                        goto EXIT_LABEL;
                    } 
#endif
                }
            }
            if(NULL != pstCb->ipv6_cfg_cb)
            {
                OS_MEMCPY(pstCb->ipv6_cfg_cb, ptemp,
                    sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_VOID spm_rcv_dci_set_classify(ATG_DCI_SET_QOS_CLASSIFY *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_QOS_CLASSIFY_KEY ulkey = {0};
    SPM_QOS_CLASSIFY_CB *pstCb = NULL;
    
    //SPM_QOS_ACL_CB *aclCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    
    //NBB_BYTE match_type = MATCH_ERROR;
    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucPortDataStart = NULL;
    NBB_BYTE *pucEthDataStart = NULL;
    NBB_BYTE *pucIpv4DataStart = NULL;
    NBB_BYTE *pucIpTcpDataStart = NULL;
    NBB_BYTE *pucIpv6DataStart = NULL;
    NBB_BYTE *pucNoIpDataStart = NULL;
    NBB_BYTE *pucIpUdpDataStart = NULL;

    /* �޻������� */
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPort = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperEth = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpTcp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperNoIp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpUdp = ATG_DCI_OPER_NULL;

    ATG_DCI_QOS_CLASSIFY_BASIC_DATA *pstBasicDate = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstSetIps) || (pstSetIps->key.qos_classify_index >= MAX_FLOW_CLASSIFY_PRI))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        goto EXIT_LABEL;
    }

    /* ���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK,�����һ��������ʧ��,����ΪFAIL */
    pstSetIps->return_code = ATG_DCI_RC_OK;
    OS_MEMCPY(&ulkey,&(pstSetIps->key),sizeof(ATG_DCI_QOS_CLASSIFY_KEY));
    pstCb = AVLL_FIND(SHARED.qos_classify_tree, &ulkey);


    /* ��ȡ�����ò��� */
    ulOperBasic = pstSetIps->oper_basic;
    ulOperPort = pstSetIps->oper_port; /* ���ڶ˿� */
    ulOperEth = pstSetIps->oper_eth_header;
    ulOperIpv4 = pstSetIps->oper_ipv4_frame;
    ulOperIpTcp = pstSetIps->oper_ipv4_tcp;
    ulOperIpv6 = pstSetIps->oper_ipv6_frame;
    ulOperNoIp = pstSetIps->oper_none_ip;
    ulOperIpUdp = pstSetIps->oper_ipv4_udp;

    /* ��ȡ�����õ�ƫ�Ƶ�ַ */
    pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->basic_data);
    pucPortDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->port_data);
    pucEthDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->eth_header_data);
    pucIpv4DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_frame_data);
    pucIpTcpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_tcp_data);
    pucIpv6DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv6_frame_data);
    pucNoIpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->none_ip_data);
    pucIpUdpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_udp_data);

    /* ����ת�� */
    pstBasicDate = (ATG_DCI_QOS_CLASSIFY_BASIC_DATA*)pucBasicDataStart;

    /* ips��Ϣ����Ϊɾ�� */
    if (TRUE == pstSetIps->delete_struct)
    {
        spm_dbg_record_qos_classify_head(&ulkey,SPM_OPER_DEL NBB_CCXT);
        
        /* �����Ŀ������,��ɾ�� ����OK*/
        if (NULL == pstCb)
        {
            goto EXIT_LABEL;
        }
        
        /* ����,ɾ�� */
        else
        {
            /* �ж��Ƿ����� */
            if(NULL != AVLL_FIRST(pstCb->spm_classify_policy_tree))
            {
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }

            /* ��classify����������ɾ��classify����Ľڵ���Ϣ */
            AVLL_DELETE(SHARED.qos_classify_tree, pstCb->spm_classify_node);
            NBB_REMOVE_FROM_LIST(pstCb->spm_classify_group_node);
            
            /* �ͷ�classify�������ڴ�ռ� */
            spm_free_classify_cb(pstCb NBB_CCXT);
            pstCb = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST(
                (SHARED.g_classify_id_instance[ulkey.qos_classify_index].classify_group));

            /************************************************* 
             * ��ʾclassify_id����Ӧ���������Ѿ�û��classify *
             * ������,��ʱ��Ҫ��classify_id��ƥ���������.   *
             *************************************************/       
            if(NULL == pstCb)
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_ERROR;
            }
        }
    }

    /* ���»����� */
    else
    {
        
        /*���*/
        if(NULL == pstCb)
        {
            spm_dbg_record_qos_classify_head(&ulkey,SPM_OPER_ADD NBB_CCXT);

            pstCb = spm_alloc_classify_cb(&ulkey NBB_CCXT);
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /***************************************************************************/
            /*                        ���ﲢδʵ��������                             */
            /***************************************************************************/

            //coverity[no_effect_test]
            rv = AVLL_INSERT(SHARED.qos_classify_tree, pstCb->spm_classify_node);
            NBB_INSERT_BEFORE(SHARED.g_classify_id_instance[ulkey.qos_classify_index].classify_group,
                    pstCb->spm_classify_group_node);

        }

        /* ���� */
        else
        {
            spm_dbg_record_qos_classify_head(&ulkey,SPM_OPER_UPD NBB_CCXT);      
        } 

        ret = spm_set_classify_basic_cfg(pstCb, ulOperBasic,&(pstCb->basic_cfg_cb),pucBasicDataStart NBB_CCXT);
        pstSetIps->basic_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            goto EXIT_LABEL;
        }

        if(NULL != pstCb->basic_cfg_cb)
        {
            /* ������������ģ������ */
            switch(pstCb->basic_cfg_cb->match_type)
            {
                case MATCH_PORT:
                ret = spm_update_classify_port_cfg(pstCb,ulOperPort,pucPortDataStart NBB_CCXT);
                pstSetIps->port_return_code = ret;
                if(ATG_DCI_RC_OK == ret)
                {
                    pstCb->match_type = MATCH_PORT;
                    SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_PORT;
                }
                break;

                case MATCH_ETH_HEADER:
                ret = spm_update_classify_eth_cfg(pstCb,ulOperEth,pucEthDataStart NBB_CCXT);
                pstSetIps->eth_header_return_code = ret;
                if(ATG_DCI_RC_OK == ret)
                {
                    pstCb->match_type = MATCH_ETH_HEADER;
                    SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_ETH_HEADER;
                }
                break;

                case MATCH_IPV4:
                ret = spm_update_classify_ipv4_cfg(pstCb,ulOperIpv4,pucIpv4DataStart NBB_CCXT);
                pstSetIps->ipv4_frame_return_code = ret;
                if(ATG_DCI_RC_OK == ret)
                {
                    pstCb->match_type = MATCH_IP_TCP;
                    SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                }
                break;

                case MATCH_IP_TCP:
                ret = spm_update_classify_iptcp_cfg(pstCb,ulOperIpTcp,pucIpTcpDataStart NBB_CCXT);
                pstSetIps->ipv4_tcp_return_code = ret;
                if(ATG_DCI_RC_OK == ret)
                {
                    pstCb->match_type = MATCH_IP_TCP;
                    SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                }
                break;

                case MATCH_IP_UDP:
                ret = spm_update_classify_ipudp_cfg(pstCb,ulOperIpUdp,pucIpUdpDataStart NBB_CCXT);
                pstSetIps->ipv4_udp_return_code = ret;
                if(ATG_DCI_RC_OK == ret)
                {
                    pstCb->match_type = MATCH_IP_TCP;
                    SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                }
                break;

                case MATCH_IPV6:
                ret = spm_update_classify_ipv6_cfg(pstCb,ulOperIpv6,pucIpv6DataStart NBB_CCXT);
                pstSetIps->ipv6_frame_return_code = ret;
                if(ATG_DCI_RC_OK == ret)
                {
                    pstCb->match_type = MATCH_IP_TCP;
                    SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                }
                break;

                case MATCH_NOIP:
                ret = spm_update_classify_noip_cfg(pstCb,ulOperNoIp,pucNoIpDataStart NBB_CCXT);
                pstSetIps->none_ip_return_code = ret;
                if(ATG_DCI_RC_OK == ret)
                {
                    pstCb->match_type = MATCH_IP_TCP;
                    SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                }
                break;

                default:    /* 690��֧��classify����aclid */
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                pstCb->match_type = MATCH_ERROR;
                printf("match_type=%d ERROR\n",pstCb->basic_cfg_cb->match_type);
                break;
            }
        }
        pstSetIps->return_code = ret;
        
    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}

#else

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_set_classify_port_cfg(NBB_ULONG oper,
    ATG_DCI_QOS_CLASSIFY_PORT_DATA **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_PORT_DATA *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            printf("%s line=%d spm_set_classify_port_cfg err : buf is NULL. "
                      "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_classify_port_cfg err : buf is NULL. "
                       "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_PORT_DATA *)buf;
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_PORT_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_PORT_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA));
        }
        break;

        default:
        break;
    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#ifdef R8000_V3R2
/*****************************************************************************
   �� �� ��  : spm_set_classify_portvlan_cfg
   ��������  : �����������ÿ�portvlanƥ��
   �������  : 
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2015��8��1�� ���ڶ�
   ��    ��  : tianf
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_classify_portvlan_cfg(NBB_ULONG oper,
    ATG_DCI_QOS_CLASSIFY_PORT_VLAN **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_PORT_VLAN *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            goto EXIT_LABEL;
        }
              
        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_PORT_VLAN *)buf;
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_PORT_VLAN *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_PORT_VLAN),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_PORTVLAN_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_PORT_VLAN));
        }
        break;
        
        default:
        break;

    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}

#endif
/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_set_classify_eth_cfg(NBB_ULONG oper, 
    ATG_DCI_QOS_CLASSIFY_ETH_HEADER **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_ETH_HEADER *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            printf("%s line=%d spm_set_classify_eth_cfg err : buf is NULL. "
                      "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_classify_eth_cfg err : buf is NULL. "
                       "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_ETH_HEADER *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_ETH_HEADER *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER),
                        NBB_NORETRY_ACT,MEM_SPM_CLASSIFY_ETH_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_set_classify_ipv4_cfg(NBB_ULONG oper,
    ATG_DCI_QOS_CLASSIFY_IPV4_FRAME **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            printf("%s line=%d spm_set_classify_ipv4_cfg err : buf is NULL. "
                      "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_classify_ipv4_cfg err : buf is NULL. "
                       "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPV4_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_set_classify_iptcp_cfg(NBB_ULONG oper, 
    ATG_DCI_QOS_CLASSIFY_IPV4_TCP **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_TCP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            printf("%s line=%d spm_set_classify_iptcp_cfg err : buf is NULL. "
                      "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_classify_iptcp_cfg err : buf is NULL. "
                       "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_IPV4_TCP *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_IPV4_TCP *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPTCP_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_set_classify_ipv6_cfg(NBB_ULONG oper,
    ATG_DCI_QOS_CLASSIFY_IPV6_FRAME **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            printf("%s line=%d spm_set_classify_ipv6_cfg err : buf is NULL. "
                      "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_classify_ipv6_cfg err : buf is NULL. "
                       "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPV6_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }   
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_set_classify_noip_cfg(NBB_ULONG oper, 
    ATG_DCI_QOS_CLASSIFY_NONE_IP **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_NONE_IP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            printf("%s line=%d spm_set_classify_noip_cfg err : buf is NULL. "
                      "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_classify_noip_cfg err : buf is NULL. "
                       "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_NONE_IP *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_NONE_IP *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_NOIP_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }  
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_LONG spm_set_classify_ipudp_cfg(NBB_ULONG oper,
    ATG_DCI_QOS_CLASSIFY_IPV4_UDP **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_UDP *ptemp = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

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
            printf("%s line=%d spm_set_classify_ipudp_cfg err : buf is NULL. "
                      "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_classify_ipudp_cfg err : buf is NULL. "
                       "ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_QOS_CLASSIFY_IPV4_UDP *)buf;

            /***************************************************************************/
            /*                          ������                                         */
            /***************************************************************************/
            if (NULL == *pcb)
            {
                *pcb = (ATG_DCI_QOS_CLASSIFY_IPV4_UDP *)NBB_MM_ALLOC(sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP),
                        NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPUDP_CB);
                if (NULL == *pcb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }  
            }
            OS_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP));
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
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_VOID spm_rcv_dci_set_classify(ATG_DCI_SET_QOS_CLASSIFY *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    ATG_DCI_QOS_CLASSIFY_KEY ulkey = {0};
    SPM_QOS_CLASSIFY_CB *pstCb = NULL;
    SPM_QOS_ACL_CB *aclCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;
    NBB_BYTE match_type = MATCH_ERROR;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucPortDataStart = NULL;
    NBB_BYTE *pucEthDataStart = NULL;
    NBB_BYTE *pucIpv4DataStart = NULL;
    NBB_BYTE *pucIpTcpDataStart = NULL;
    NBB_BYTE *pucIpv6DataStart = NULL;
    NBB_BYTE *pucNoIpDataStart = NULL;
    NBB_BYTE *pucIpUdpDataStart = NULL;
    NBB_BYTE *pucPortVlanDataStart = NULL;

    /* �޻������� */
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPort = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperEth = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpTcp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperNoIp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpUdp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPortVlan = ATG_DCI_OPER_NULL;


    ATG_DCI_QOS_CLASSIFY_BASIC_DATA *pstBasicDate = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstSetIps) || (pstSetIps->key.qos_classify_index >= MAX_FLOW_CLASSIFY_PRI))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_rcv_dci_set_classify err : ATG_DCI_SET_QOS_CLASSIFY "
                  "*pstSetIps is NULL. ret = %d\n\n",__FUNCTION__,__LINE__,ret);
        OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_classify err : ATG_DCI_SET_QOS_CLASSIFY "
                   "*pstSetIps is NULL. ret = %d\n\n",__FUNCTION__,__LINE__,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }

    /* ���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK,�����һ��������ʧ��,����ΪFAIL */
    pstSetIps->return_code = ATG_DCI_RC_OK;
    OS_MEMCPY(&ulkey,&(pstSetIps->key),sizeof(ATG_DCI_QOS_CLASSIFY_KEY));
    pstCb = AVLL_FIND(SHARED.qos_classify_tree, &ulkey);
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }

    /* ��ȡ�����ò��� */
    ulOperBasic = pstSetIps->oper_basic;
    ulOperPort = pstSetIps->oper_port; /* ���ڶ˿� */
    ulOperEth = pstSetIps->oper_eth_header;
    ulOperIpv4 = pstSetIps->oper_ipv4_frame;
    ulOperIpTcp = pstSetIps->oper_ipv4_tcp;
    ulOperIpv6 = pstSetIps->oper_ipv6_frame;
    ulOperNoIp = pstSetIps->oper_none_ip;
    ulOperIpUdp = pstSetIps->oper_ipv4_udp;
#ifdef R8000_V3R2
    ulOperPortVlan = pstSetIps->oper_port_vlan;
#endif
    /* ��ȡ�����õ�ƫ�Ƶ�ַ */
    pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->basic_data);
    pucPortDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->port_data);
    pucEthDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->eth_header_data);
    pucIpv4DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_frame_data);
    pucIpTcpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_tcp_data);
    pucIpv6DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv6_frame_data);
    pucNoIpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->none_ip_data);
    pucIpUdpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_udp_data);
#ifdef R8000_V3R2
    pucPortVlanDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->port_vlan_data);
#endif
    /* ����ת�� */
    pstBasicDate = (ATG_DCI_QOS_CLASSIFY_BASIC_DATA*)pucBasicDataStart;

    /* ips��Ϣ����Ϊɾ�� */
    if (TRUE == pstSetIps->delete_struct)
    {
        /* �����Ŀ������,��ɾ�� */
        if (ucIfExist == QOS_UNEXIST)
        {
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            printf("%s line=%d spm_rcv_dci_set_classify del err : no classify cfg. "
                      "ret = %d, classify_id =%d, if_match_id = %d\n\n",__FUNCTION__,__LINE__,
                      pstSetIps->return_code,ulkey.qos_classify_index,ulkey.if_match_id);
            OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_classify del err : no classify cfg. "
                       "ret = %d, classify_id =%d, if_match_id = %d\n\n",__FUNCTION__,__LINE__,
                       pstSetIps->return_code,ulkey.qos_classify_index,ulkey.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
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
                printf("%s line=%d spm_rcv_dci_set_classify del err : classify is being used. "
                          "ret = %d, classify_id =%d, if_match_id = %d\n\n",__FUNCTION__,__LINE__,
                          pstSetIps->return_code,ulkey.qos_classify_index,ulkey.if_match_id);
                OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_classify del err : classify is being used. "
                           "ret = %d, classify_id =%d, if_match_id = %d\n\n",__FUNCTION__,__LINE__,
                           pstSetIps->return_code,ulkey.qos_classify_index,ulkey.if_match_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            			       QOS_ERROR_STRING,ret,
            			       ucMessage,__FUNCTION__,"LINE","",
            			       0,0,__LINE__,0)); 
                goto EXIT_LABEL;
            }

            /* �ͷŸ����������ȫ��rule_id */
            spm_qos_free_classfiy_rule_id(pstCb->rule_id NBB_CCXT);
            AVLL_DELETE(SHARED.qos_classify_tree, pstCb->spm_classify_node);
            NBB_REMOVE_FROM_LIST(pstCb->spm_classify_group_node);

            /* �ͷ�classify�������ڴ�ռ� */
            spm_free_classify_cb(pstCb NBB_CCXT);

            pstCb = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST(
                (SHARED.g_classify_id_instance[ulkey.qos_classify_index].classify_group));
            if(NULL == pstCb)
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_ERROR;
            }
        }
    }

    /* ���»����� */
    else
    {
        if(NULL == pstBasicDate)
        {
            pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
            goto EXIT_LABEL;
        }
        if(MAX_FLOW_CLASSIFY_PRI <= ulkey.qos_classify_index)
        {
            pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
            printf("%s line=%d spm_rcv_dci_set_classify err : classifyid out "
                      "of range. ret = %d, classify_id =%d, if_match_id = %d, match_type = %d\n\n",
                      __FUNCTION__,__LINE__,pstSetIps->return_code,ulkey.qos_classify_index,
                      ulkey.if_match_id,pstBasicDate->match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_classify err : classifyid out "
                       "of range. ret = %d, classify_id =%d, if_match_id = %d, match_type = %d\n\n",
                       __FUNCTION__,__LINE__,pstSetIps->return_code,ulkey.qos_classify_index,
                       ulkey.if_match_id,pstBasicDate->match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* �ж�ͬһ��classify_id��ƥ�������Ƿ�һ��:��Ϊƥ����̫��ͷ/IP/ƥ��ACL_ID */
        /*ret = spm_check_classify_id_cfg(ulkey.qos_classify_index,pstBasicDate->match_type NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }*/

        /* �����Ŀ������,�������ڴ�ռ䱣������ */
        if (ucIfExist == QOS_UNEXIST)
        {
            pstCb = spm_alloc_classify_cb(&ulkey NBB_CCXT);
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

        if(NULL == pstCb)
        {
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                          ��������                                       */
        /***************************************************************************/
        ret = spm_set_classify_basic_cfg(pstCb, ulOperBasic,&(pstCb->basic_cfg_cb),pucBasicDataStart NBB_CCXT);
        pstSetIps->basic_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                              ���ڶ˿�ƥ������                           */
        /***************************************************************************/
        if((NULL != pucPortDataStart) && (ATG_DCI_OPER_NULL != ulOperPort))
        {
            ret = spm_set_classify_port_cfg(ulOperPort, &(pstCb->Port_cfg_cb) , pucPortDataStart NBB_CCXT);
            pstSetIps->port_return_code = ret;
            if (ATG_DCI_RC_OK != ret)
            {
                ips_ret = ret;
            }
            else
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_PORT;
                match_type = MATCH_PORT;
            }
        }

        /***************************************************************************/
        /*                              ������̫������                             */
        /***************************************************************************/
        if((NULL != pucEthDataStart) && (ATG_DCI_OPER_NULL != ulOperEth))
        {
            ret = spm_set_classify_eth_cfg(ulOperEth, &(pstCb->Eth_cfg_cb), pucEthDataStart NBB_CCXT);
            pstSetIps->eth_header_return_code = ret;
            if (ATG_DCI_RC_OK != ret)
            {
                ips_ret = ret;
            }
            else
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_ETH_HEADER;
                match_type = MATCH_ETH_HEADER;
            }
        }

        /***************************************************************************/
        /*                                  ����ipv4                               */
        /***************************************************************************/
        if((NULL != pucIpv4DataStart) && (ATG_DCI_OPER_NULL != ulOperIpv4))
        {
            ret = spm_set_classify_ipv4_cfg(ulOperIpv4,&(pstCb->ipv4_cfg_cb),pucIpv4DataStart NBB_CCXT);
            pstSetIps->ipv4_frame_return_code = ret;
            if (ATG_DCI_RC_OK != ret)
            {
                ips_ret = ret;
            }
            else
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                match_type = MATCH_IP_TCP;
            }
        }

        /***************************************************************************/
        /*                          ����tcp                                        */
        /***************************************************************************/
        if((NULL != pucIpTcpDataStart) && (ATG_DCI_OPER_NULL != ulOperIpTcp))
        {
            ret = spm_set_classify_iptcp_cfg(ulOperIpTcp,&(pstCb->ipTcp_cfg_cb),pucIpTcpDataStart NBB_CCXT);
            pstSetIps->ipv4_tcp_return_code = ret;
            if (ATG_DCI_RC_OK != ret)
            {
                ips_ret = ret;
            }
            else
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                match_type = MATCH_IP_TCP;
            }
        }

        /***************************************************************************/
        /*                          ����ipv6                                       */
        /***************************************************************************/
        if((NULL != pucIpv6DataStart) && (ATG_DCI_OPER_NULL != ulOperIpv6))
        {
            ret = spm_set_classify_ipv6_cfg(ulOperIpv6,&(pstCb->ipv6_cfg_cb),pucIpv6DataStart NBB_CCXT);
            pstSetIps->ipv6_frame_return_code = ret;
            if (ATG_DCI_RC_OK != ret)
            {
                ips_ret = ret;
            }
            else
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                match_type = MATCH_IP_TCP;
            }
        }

        /***************************************************************************/
        /*                          ���ڷ�IP                                       */
        /***************************************************************************/
        if((NULL != pucNoIpDataStart) && (ATG_DCI_OPER_NULL != ulOperNoIp))
        {
            ret = spm_set_classify_noip_cfg(ulOperNoIp,&(pstCb->noIp_cfg_cb),pucNoIpDataStart NBB_CCXT);
            pstSetIps->none_ip_return_code = ret;
            if (ATG_DCI_RC_OK != ret)
            {
                ips_ret = ret;
            }
            else
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                match_type = MATCH_IP_TCP;
            }
        }

        /***************************************************************************/
        /*                          ����IPUDP                                      */
        /***************************************************************************/
        if((NULL != pucIpUdpDataStart) && (ATG_DCI_OPER_NULL != ulOperIpUdp))
        {
            ret = spm_set_classify_ipudp_cfg(ulOperIpUdp,&(pstCb->ipUdp_cfg_cb),pucIpUdpDataStart NBB_CCXT);
            pstSetIps->ipv4_udp_return_code = ret;
            if (ATG_DCI_RC_OK != ret)
            {
                ips_ret = ret;
            }
            else
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_IP_TCP;
                match_type = MATCH_IP_TCP;
            }
        }
#ifdef R8000_V3R2

        /***************************************************************************/
        /*                          ����PORTVLAN                                      */
        /***************************************************************************/
        if((NULL != pucPortVlanDataStart) && (ATG_DCI_OPER_NULL != ulOperPortVlan))
        {
            ret = spm_set_classify_portvlan_cfg(ulOperPortVlan,&(pstCb->portVlan_cfg_cb),pucPortVlanDataStart NBB_CCXT);
            pstSetIps->port_vlan_return_code = ret;
            if (ATG_DCI_RC_OK != ret)
            {
                ips_ret = ret;
            }
            else
            {
                SHARED.g_classify_id_instance[ulkey.qos_classify_index].match_type = MATCH_PORT;
                match_type = MATCH_PORT;
            }
        }
#endif

        pstSetIps->return_code = ips_ret;
        if(ATG_DCI_RC_OK != ips_ret)
        {
            printf("%s line=%d spm_rcv_dci_set_classify err : classify cfg failed"
                      "ret = %d, classify_id =%d, if_match_id = %d, match_type = %d\n\n",
                      __FUNCTION__,__LINE__,pstSetIps->return_code,ulkey.qos_classify_index,
                      ulkey.if_match_id,pstCb->basic_cfg_cb->match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_classify err : classify cfg failed"
                       "ret = %d, classify_id =%d, if_match_id = %d, match_type = %d\n\n",
                       __FUNCTION__,__LINE__,pstSetIps->return_code,ulkey.qos_classify_index,
                       ulkey.if_match_id,pstCb->basic_cfg_cb->match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            if(ucIfExist == QOS_UNEXIST)
            {
                spm_free_classify_cb(pstCb NBB_CCXT);
            }
            goto EXIT_LABEL;
        }

        /* �������������Ŀ,���뵽���� */
        if (ucIfExist == QOS_UNEXIST)
        {
            /* ÿ��classify���򶼻����һ��ȫ�ֵ�rule_id��Ψһ�ı�ʾ�������� */
            ret = spm_qos_apply_classfiy_rule_id(&(pstCb->rule_id) NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_free_classify_cb(pstCb NBB_CCXT);
                goto EXIT_LABEL;
            }
            pstCb->match_type = match_type;
            
            //coverity[no_effect_test]
            AVLL_INSERT(SHARED.qos_classify_tree, pstCb->spm_classify_node);
            NBB_INSERT_BEFORE(SHARED.g_classify_id_instance[ulkey.qos_classify_index].classify_group,
                    pstCb->spm_classify_group_node);
        }
    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}

#endif


/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : �ͷ��ڴ�������bukectͰ��Դ
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
NBB_VOID spm_qos_clear_all_classify(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_CLASSIFY_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_CLASSIFY_CB*)AVLL_FIRST(v_spm_shared->qos_classify_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_CLASSIFY_CB*)AVLL_FIRST(v_spm_shared->qos_classify_tree))
    {
        NBB_REMOVE_FROM_LIST(cfg_cb->spm_classify_group_node);
        AVLL_DELETE(v_spm_shared->qos_classify_tree, cfg_cb->spm_classify_node);
        spm_free_classify_cb(cfg_cb NBB_CCXT);   
    }
    return;   
}

#endif
