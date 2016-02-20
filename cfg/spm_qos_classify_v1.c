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
#include <spm_qos_classify_v1.h>

/* �ⲿȫ���� */
extern AVLL_TREE g_qos_action_tree;

/* �ⲿȫ���� */
extern AVLL_TREE g_qos_policy_tree;

/* classify�� */
AVLL_TREE g_qos_classify_tree;

/* classify�������� */
SPM_QOS_CLASSIFY_ID_CB g_classify_id_instance[MAX_FLOW_CLASSIFY_PRI];

/* ƥ��˿�aclid */
NBB_BYTE g_classify_port_acl_id[MAX_CLASSIFY_PORT_ACL_ID_NUM];

/* �߼�aclid */
NBB_BYTE g_classify_high_acl_id[MAX_CLASSIFY_HIGH_ACL_ID_NUM];

/* ƥ����̫��aclid */
NBB_BYTE g_classify_eth_acl_id[MAX_CLASSIFY_ETH_ACL_ID_NUM];

/* classifyģ��ȫ�ִ�ӡ���� */
NBB_BYTE g_qos_classify_print = ATG_DCI_RC_OK;

/* classifyģ��ȫ��LOG���� */
NBB_BYTE g_qos_classify_log   = ATG_DCI_RC_UNSUCCESSFUL;


#if 1

/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : �߼��˿����ıȽϺ���
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_INT spm_qos_classify_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_QOS_CLASSIFY_KEY *key1 = (ATG_DCI_QOS_CLASSIFY_KEY *)keyarg1;
    ATG_DCI_QOS_CLASSIFY_KEY *key2 = (ATG_DCI_QOS_CLASSIFY_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->qos_classify_index), &(key2->qos_classify_index));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->if_match_id), &(key2->if_match_id));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_find_policy_cb
   ��������  : ����policyģ���������
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_apply_classify_high_acl_id(NBB_ULONG *pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_LONG ret = ATG_DCI_RC_UNSUCCESSFUL;

    NBB_TRC_ENTRY(__FUNCTION__);

    for (id = CLASSIFY_HIGH_ACL_ID; id < CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM; id++)
    {
        /* �п���ID */
        if (ATG_DCI_RC_OK == g_classify_high_acl_id[id - CLASSIFY_HIGH_ACL_ID])
        {
            *pid = id;
            g_classify_high_acl_id[id - CLASSIFY_HIGH_ACL_ID] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;
            goto EXIT_LABEL;
        }
    }

    /* �쳣�˳� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return  ret;
}


/*****************************************************************************
   �� �� ��  : spm_find_policy_cb
   ��������  : ����policyģ���������
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_apply_classify_port_acl_id(NBB_ULONG *pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_LONG ret = ATG_DCI_RC_UNSUCCESSFUL;

    NBB_TRC_ENTRY(__FUNCTION__);

    for (id = CLASSIFY_PORT_ACL_ID; id < CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM; id++)
    {
        /* �п���ID */
        if (ATG_DCI_RC_OK == g_classify_port_acl_id[id - CLASSIFY_PORT_ACL_ID])
        {
            *pid = id;
            g_classify_port_acl_id[id - CLASSIFY_PORT_ACL_ID] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;
            goto EXIT_LABEL;
        }
    }

    /* �쳣�˳� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_find_policy_cb
   ��������  : ����policyģ���������
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_apply_classify_eth_acl_id(NBB_ULONG *pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id  = 0;
    NBB_ULONG ret = ATG_DCI_RC_UNSUCCESSFUL;

    NBB_TRC_ENTRY(__FUNCTION__);

    for (id = CLASSIFY_ETH_ACL_ID; id < CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM; id++)
    {
        /* �п���ID */
        if (ATG_DCI_RC_OK == g_classify_eth_acl_id[id - CLASSIFY_ETH_ACL_ID])
        {
            *pid = id;
            g_classify_eth_acl_id[id - CLASSIFY_ETH_ACL_ID] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;
            goto EXIT_LABEL;
        }
    }

    /* �쳣�˳� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_find_policy_cb
   ��������  : ����policyģ���������
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_release_classify_high_acl_id(NBB_ULONG pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((CLASSIFY_HIGH_ACL_ID > pid) || (pid >= (CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM)))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu out of range.\n", __FUNCTION__, __LINE__, pid);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu out of range.\n", __FUNCTION__, __LINE__, pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_classify_high_acl_id[pid - CLASSIFY_HIGH_ACL_ID])
    {
        g_classify_high_acl_id[pid - CLASSIFY_HIGH_ACL_ID] = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu not in use.\n\n", __FUNCTION__, __LINE__, pid);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu not in use.\n\n", __FUNCTION__, __LINE__, pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* �쳣�˳� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_find_policy_cb
   ��������  : ����policyģ���������
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_release_classify_port_acl_id(NBB_ULONG pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((CLASSIFY_PORT_ACL_ID > pid) || (pid >= (CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM)))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu out of range.\n", __FUNCTION__, __LINE__, pid);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu out of range.\n", __FUNCTION__, __LINE__, pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_classify_port_acl_id[pid - CLASSIFY_PORT_ACL_ID])
    {
        g_classify_port_acl_id[pid - CLASSIFY_PORT_ACL_ID] = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu not in use.\n\n", __FUNCTION__, __LINE__, pid);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu not in use.\n\n", __FUNCTION__, __LINE__, pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* �쳣�˳� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_find_policy_cb
   ��������  : ����policyģ���������
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_release_classify_eth_acl_id(NBB_ULONG pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((CLASSIFY_ETH_ACL_ID > pid) || (pid >= (CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM)))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu out of range.\n", __FUNCTION__, __LINE__, pid);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu out of range.\n", __FUNCTION__, __LINE__, pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_classify_eth_acl_id[pid - CLASSIFY_ETH_ACL_ID])
    {
        g_classify_eth_acl_id[pid - CLASSIFY_ETH_ACL_ID] = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu not in use.\n", __FUNCTION__, __LINE__, pid);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu not in use.\n", __FUNCTION__, __LINE__, pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* �쳣�˳� */
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
SPM_QOS_POLICY_CLASSIFY_CB* spm_alloc_policy_classify_cb(NBB_ULONG key)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_CLASSIFY_CB *pcb = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    pcb = (SPM_QOS_POLICY_CLASSIFY_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_POLICY_CLASSIFY_CB),
                                          NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_POLICY_CB);
    if(NULL == pcb)
    {
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(pcb, 0, sizeof(SPM_QOS_POLICY_CLASSIFY_CB));
    pcb->supervise_key = key;
    AVLL_INIT_NODE(pcb->tree_node);
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return pcb;
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
NBB_LONG spm_free_policy_classify_cb(SPM_QOS_POLICY_CLASSIFY_CB *pstcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pstcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_POLICY_CLASSIFY_CB), MEM_SPM_CLASSIFY_POLICY_CB);

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_CLASSIFY_POLICY_CB);
    pstcb = NULL;

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
SPM_QOS_CLASSIFY_CB *spm_alloc_classify_cb(ATG_DCI_QOS_CLASSIFY_KEY *pkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BUF_SIZE avll_key_offset;
    SPM_QOS_CLASSIFY_CB *pstcb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pkey)
    {
        printf("ATG_DCI_QOS_CLASSIFY_KEY == NULL\n");
        goto EXIT_LABEL;
    }

    pstcb = (SPM_QOS_CLASSIFY_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_CLASSIFY_CB),
                                      NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_CB);
    if(pstcb == NULL)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstcb, 0, sizeof(SPM_QOS_CLASSIFY_CB));
    OS_MEMCPY(&(pstcb->classify_key), pkey, sizeof(ATG_DCI_QOS_CLASSIFY_KEY));
    AVLL_INIT_NODE(pstcb->spm_classify_node);
    NBB_INIT_LQE(pstcb->spm_classify_group_node, pstcb);

    /* ��ʼ��classify���ÿ��е�spm_classify_policy_tree�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_POLICY_CLASSIFY_CB, supervise_key);
    AVLL_INIT_TREE(pstcb->spm_classify_policy_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset, 
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_POLICY_CLASSIFY_CB, tree_node));

    EXIT_LABEL: NBB_TRC_EXIT();
    return pstcb;
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
NBB_LONG spm_free_classify_cb(SPM_QOS_CLASSIFY_CB *pstcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pstcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_CLASSIFY_CB), MEM_SPM_CLASSIFY_CB);

   /*************************************************** 
    * ��ɾ��classifyģ��֮ǰ,��Ҫ��ɾ��classifyģ���� *
    * ����������(spm_classify_policy_tree)�еĽڵ�. *
    ***************************************************/
    spm_del_classify_rule_cfg(pstcb);

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (NULL != pstcb->basic_cfg_cb)
    {
        pstcb->match_type = MATCH_ERROR;
        NBB_MM_FREE(pstcb->basic_cfg_cb, MEM_SPM_CLASSIFY_BASIC_CB);
        pstcb->basic_cfg_cb = NULL;
    }
    if (NULL != pstcb->Port_cfg_cb)
    {
        NBB_MM_FREE(pstcb->Port_cfg_cb, MEM_SPM_CLASSIFY_PORT_CB);
        pstcb->Port_cfg_cb = NULL;
    }
    if (NULL != pstcb->Eth_cfg_cb)
    {
        NBB_MM_FREE(pstcb->Eth_cfg_cb, MEM_SPM_CLASSIFY_ETH_CB);
        pstcb->Eth_cfg_cb = NULL;
    }
    if (NULL != pstcb->ipv4_cfg_cb)
    {
        NBB_MM_FREE(pstcb->ipv4_cfg_cb, MEM_SPM_CLASSIFY_IPV4_CB);
        pstcb->ipv4_cfg_cb = NULL;
    }
    if (NULL != pstcb->ipTcp_cfg_cb)
    {
        NBB_MM_FREE(pstcb->ipTcp_cfg_cb, MEM_SPM_CLASSIFY_IPTCP_CB);
        pstcb->ipTcp_cfg_cb = NULL;
    }
    if (NULL != pstcb->ipv6_cfg_cb)
    {
        NBB_MM_FREE(pstcb->ipv6_cfg_cb, MEM_SPM_CLASSIFY_IPV6_CB);
        pstcb->ipv6_cfg_cb = NULL;
    }
    if (NULL != pstcb->noIp_cfg_cb)
    {
        NBB_MM_FREE(pstcb->noIp_cfg_cb, MEM_SPM_CLASSIFY_NOIP_CB);
        pstcb->noIp_cfg_cb = NULL;
    }
    if (NULL != pstcb->ipUdp_cfg_cb)
    {
        NBB_MM_FREE(pstcb->ipUdp_cfg_cb, MEM_SPM_CLASSIFY_IPUDP_CB);
        pstcb->ipUdp_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_CLASSIFY_CB);
    pstcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_free_classify_rule_tlv
   ��������  : ��ȡipv6��ַ������
   �������  : ���볤��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_free_classify_rule_tlv(SPM_QOS_CLASSIFY_CB *pstcb, NBB_LONG retstatus)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE match_type = 0;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* rule������µĹ�����,MATCH_PORT/MATCH_ETH_HEADER��match_type�����ܸı� */
    if(pstcb->match_type != pstcb->basic_cfg_cb->match_type)
    {
        /* ��ʾ���²����ɹ�,ɾ����ʼmatch_type����tlv�ռ� */
        if(ATG_DCI_RC_OK == retstatus)
        {
            match_type = pstcb->match_type;
        }

        /* ��ʾ���²���ʧ��,ɾ�����¹�����������ڴ�ռ� */
        else
        {
            match_type = pstcb->basic_cfg_cb->match_type;
        }

        switch(match_type)
        {
            case MATCH_IPV4:
            if (NULL != pstcb->ipv4_cfg_cb)
            {
                NBB_MM_FREE(pstcb->ipv4_cfg_cb, MEM_SPM_CLASSIFY_IPV4_CB);
                pstcb->ipv4_cfg_cb = NULL;
            }
            break;

            case MATCH_IP_TCP:
            if (NULL != pstcb->ipTcp_cfg_cb)
            {
                NBB_MM_FREE(pstcb->ipTcp_cfg_cb, MEM_SPM_CLASSIFY_IPTCP_CB);
                pstcb->ipTcp_cfg_cb = NULL;
            }
            break;

            case MATCH_IP_UDP:
            if (NULL != pstcb->ipUdp_cfg_cb)
            {
                NBB_MM_FREE(pstcb->ipUdp_cfg_cb, MEM_SPM_CLASSIFY_IPUDP_CB);
                pstcb->ipUdp_cfg_cb = NULL;
            }
            break;

            case MATCH_IPV6:
            if (NULL != pstcb->ipv6_cfg_cb)
            {
                NBB_MM_FREE(pstcb->ipv6_cfg_cb, MEM_SPM_CLASSIFY_IPV6_CB);
                pstcb->ipv6_cfg_cb = NULL;
            }
            break;

            case MATCH_NOIP:
            if (NULL != pstcb->noIp_cfg_cb)
            {
                NBB_MM_FREE(pstcb->noIp_cfg_cb, MEM_SPM_CLASSIFY_NOIP_CB);
                pstcb->noIp_cfg_cb = NULL;
            }
            break;

            /* match_type��ȡֵ���� */
            default:
            break;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif 


#if 2

/*****************************************************************************
   �� �� ��  : spm_check_classify_matchtype
   ��������  : ��ȡipv6��ַ������
   �������  : ���볤��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_check_classify_match_type(ATG_DCI_QOS_CLASSIFY_KEY classifykey, 
    NBB_BYTE match_type)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG  ret = ATG_DCI_RC_OK;
    NBB_CHAR  ucmessage[QOS_MSG_INFO_LEN];
    NBB_ULONG if_match_id = classifykey.if_match_id;
    NBB_ULONG classify_id = classifykey.qos_classify_index;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((0 == classify_id) || (0 == if_match_id) || (MAX_FLOW_CLASSIFY_PRI <= classify_id))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* ����ת��match_type��ֵ */
    switch(match_type)
    {
        case MATCH_PORT:
        match_type = MATCH_PORT;
        break;

        case MATCH_ETH_HEADER:
        match_type = MATCH_ETH_HEADER;
        break;

        case MATCH_IPV4:

        case MATCH_IP_TCP:

        case MATCH_IP_UDP:

        case MATCH_IPV6:

        case MATCH_NOIP:
        match_type = MATCH_IP_TCP;
        break;

        /* match_type��ȡֵ����(��֧��classify����aclid) */
        default:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:match-type=%d of cid=%lu/iid=%lu err.\n", __FUNCTION__, 
               __LINE__, match_type, classify_id, if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:match-type=%d of cid=%lu/iid=%lu err.\n", 
                   __FUNCTION__, __LINE__, match_type, classify_id, if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* ��classifyid�������� */
    if(MATCH_ERROR == g_classify_id_instance[classify_id].match_type)
    {
        g_classify_id_instance[classify_id].match_type = match_type;
    }
    else
    {
        /* classifyid������if-match��match-type������ͬ,���򱨴� */
        if(match_type != g_classify_id_instance[classify_id].match_type)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:match-type of cid=%lu/iid=%lu can't be changed.\n", 
                   __FUNCTION__, __LINE__, classify_id, if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:match-type of cid=%lu/iid=%lu can't "
                       "be changed.\n", __FUNCTION__, __LINE__, classify_id, if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_basic_cfg
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
NBB_LONG spm_set_classify_basic_cfg(SPM_QOS_CLASSIFY_CB *pstcb, 
    NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_CLASSIFY_BASIC_DATA *pstbasicdate = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* ACL_BASIC_DATA��tlv�Ķ���ֻ����ΪADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:basic-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->classify_key.qos_classify_index, 
               pstcb->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:basic-oper of cid=%lu/iid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, 
                   pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstbasicdate = (ATG_DCI_QOS_CLASSIFY_BASIC_DATA*)buf;

    /* ��֧��classify������aclid */
    ret = spm_check_classify_match_type(pstcb->classify_key, pstbasicdate->match_type);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    /* ����if-match���� */
    if(NULL == pstcb->basic_cfg_cb)
    {
       /************************************************************ 
        * ipv4/ipv6/noip/iptcp/ipudp�������C3��������ͬһ��aclid. *
        * ����ʱ��Ҫ��match_type���ȴ���,�ڸ��²���ʱ����º��ֵ  *
        * ���бȽ�,���ھ����Ƿ���Ҫfree��alloc�ڴ�ռ�.            *
        ************************************************************/
        pstcb->match_type   = pstbasicdate->match_type;
        pstcb->basic_cfg_cb = (ATG_DCI_QOS_CLASSIFY_BASIC_DATA *)NBB_MM_ALLOC(  \
                               sizeof(ATG_DCI_QOS_CLASSIFY_BASIC_DATA),
                               NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_BASIC_CB);
        if(NULL == pstcb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }
    }

    OS_MEMCPY(pstcb->basic_cfg_cb, pstbasicdate, sizeof(ATG_DCI_QOS_CLASSIFY_BASIC_DATA));

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_port_cfg
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
NBB_LONG spm_set_classify_port_cfg(SPM_QOS_CLASSIFY_CB *pstcb, 
    NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_CLASSIFY_PORT_DATA *pstportdate = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* CLASSIFY_PORT_DATA��tlv�Ķ���ֻ����ΪADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:port-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->classify_key.qos_classify_index, 
               pstcb->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:port-oper of cid=%lu/iid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, 
                   pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstportdate = (ATG_DCI_QOS_CLASSIFY_PORT_DATA *)buf;
    if(NULL == pstcb->Port_cfg_cb)
    {
        pstcb->Port_cfg_cb = (ATG_DCI_QOS_CLASSIFY_PORT_DATA*)NBB_MM_ALLOC( \
                              sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA), 
                              NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_PORT_CB);
        if(NULL == pstcb->Port_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }
    }
    OS_MEMCPY(pstcb->Port_cfg_cb, pstportdate, sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA));

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_eth_cfg
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
NBB_LONG spm_set_classify_eth_cfg(SPM_QOS_CLASSIFY_CB *pstcb, 
    NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_ETH_HEADER *pstethdate = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* CLASSIFY_ETH_HEADER��tlv�Ķ���ֻ����ΪADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:eth-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", __FUNCTION__, 
               __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:eth-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", 
                   __FUNCTION__, __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstethdate = (ATG_DCI_QOS_CLASSIFY_ETH_HEADER*)buf;
    if (NULL == pstcb->Eth_cfg_cb)
    {
        pstcb->Eth_cfg_cb = (ATG_DCI_QOS_CLASSIFY_ETH_HEADER*)NBB_MM_ALLOC(     \
                             sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER), 
                             NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_ETH_CB);
        if (NULL == pstcb->Eth_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }
    }
    OS_MEMCPY(pstcb->Eth_cfg_cb, pstethdate, sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER));

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_ipv4_cfg
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
NBB_LONG spm_set_classify_ipv4_cfg(SPM_QOS_CLASSIFY_CB *pstcb, 
    NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *pstipv4date = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* CLASSIFY_IPV4_FRAME��tlv�Ķ���ֻ����ΪADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:ipv4-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", __FUNCTION__, 
               __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:ipv4-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", 
                   __FUNCTION__, __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstipv4date = (ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *)buf;
    if (NULL == pstcb->ipv4_cfg_cb)
    {
        pstcb->ipv4_cfg_cb = (ATG_DCI_QOS_CLASSIFY_IPV4_FRAME*)NBB_MM_ALLOC(    \
                              sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME),
                              NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPV4_CB);
        if (NULL == pstcb->ipv4_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }
    }
    OS_MEMCPY(pstcb->ipv4_cfg_cb, pstipv4date, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME));

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_iptcp_cfg
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
NBB_LONG spm_set_classify_iptcp_cfg(SPM_QOS_CLASSIFY_CB *pstcb, 
    NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_TCP *psttcpdate = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* CLASSIFY_IPV4_TCP��tlv�Ķ���ֻ����ΪADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:tcp-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", __FUNCTION__, 
               __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:tcp-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", 
                   __FUNCTION__, __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    psttcpdate = (ATG_DCI_QOS_CLASSIFY_IPV4_TCP*)buf;
    if (NULL == pstcb->ipTcp_cfg_cb)
    {
        pstcb->ipTcp_cfg_cb = (ATG_DCI_QOS_CLASSIFY_IPV4_TCP*)NBB_MM_ALLOC( \
                               sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP),
                               NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPTCP_CB);
        if (NULL == pstcb->ipTcp_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }
    }
    OS_MEMCPY(pstcb->ipTcp_cfg_cb, psttcpdate, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP));

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_ipv6_cfg
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
NBB_LONG spm_set_classify_ipv6_cfg(SPM_QOS_CLASSIFY_CB *pstcb, 
    NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *pstipv6date = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* CLASSIFY_IPV6_FRAME��tlv�Ķ���ֻ����ΪADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:ipv6-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", __FUNCTION__, 
               __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:ipv6-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", 
                   __FUNCTION__, __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    
    pstipv6date = (ATG_DCI_QOS_CLASSIFY_IPV6_FRAME*)buf;
    if (NULL == pstcb->ipv6_cfg_cb)
    {
        pstcb->ipv6_cfg_cb = (ATG_DCI_QOS_CLASSIFY_IPV6_FRAME*)NBB_MM_ALLOC(    \
                              sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME),
                              NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPV6_CB);
        if (NULL == pstcb->ipv6_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }   
    }
    OS_MEMCPY(pstcb->ipv6_cfg_cb, pstipv6date, sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME));

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_noip_cfg
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
NBB_LONG spm_set_classify_noip_cfg(SPM_QOS_CLASSIFY_CB *pstcb, 
    NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_NONE_IP *pstnoipdate = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* CLASSIFY_NONE_IP��tlv�Ķ���ֻ����ΪADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:noip-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", __FUNCTION__, 
               __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:noip-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", 
                   __FUNCTION__, __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstnoipdate = (ATG_DCI_QOS_CLASSIFY_NONE_IP*)buf;
    if (NULL == pstcb->noIp_cfg_cb)
    {
        pstcb->noIp_cfg_cb = (ATG_DCI_QOS_CLASSIFY_NONE_IP*)NBB_MM_ALLOC(   \
                              sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP),
                              NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_NOIP_CB);
        if (NULL == pstcb->noIp_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }  
    }
    OS_MEMCPY(pstcb->noIp_cfg_cb, pstnoipdate, sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP));

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_ipudp_cfg
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
NBB_LONG spm_set_classify_ipudp_cfg(SPM_QOS_CLASSIFY_CB *pstcb, 
    NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_QOS_CLASSIFY_IPV4_UDP *pstudpdate = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* CLASSIFY_IPV4_UDP��tlv�Ķ���ֻ����ΪADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:udp-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", __FUNCTION__, 
               __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:udp-oper of cid=%lu/iid=%lu can only be ATG_DCI_OPER_ADD.\n", 
                   __FUNCTION__, __LINE__, pstcb->classify_key.qos_classify_index, pstcb->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstudpdate = (ATG_DCI_QOS_CLASSIFY_IPV4_UDP*)buf;
    if (NULL == pstcb->ipUdp_cfg_cb)
    {
        pstcb->ipUdp_cfg_cb = (ATG_DCI_QOS_CLASSIFY_IPV4_UDP*)NBB_MM_ALLOC( \
                               sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP),
                               NBB_NORETRY_ACT, MEM_SPM_CLASSIFY_IPUDP_CB);
        if (NULL == pstcb->ipUdp_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }  
    }
    OS_MEMCPY(pstcb->ipUdp_cfg_cb, pstudpdate, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP));

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_del_rule_behavior_supervise
   ��������  : ����policyģ����cb�������Ƿ�Ϊ��,Ȼ���������
               ���ڴ��б������cb���йص���Ϣ�� 
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_del_rule_policy_supervise(SPM_QOS_POLICY_CLASSIFY_CB *classifysup, 
    NBB_BYTE lastifmatch)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *policycb = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == classifysup)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    policycb = AVLL_FIND(g_qos_policy_tree, &(classifysup->supervise_key));
    if(NULL == policycb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:no pid=%lu cfg.\n\n", __FUNCTION__, __LINE__, 
               classifysup->supervise_key);
        OS_SPRINTF(ucmessage,"***err***%s,%d:no pid=%lu cfg.\n\n", __FUNCTION__, 
                   __LINE__, classifysup->supervise_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
    }
    else
    {
        if(NULL == policycb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:no pid=%lu basic-cfg.\n\n", __FUNCTION__, __LINE__, 
                   classifysup->supervise_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no pid=%lu basic-cfg.\n\n", __FUNCTION__, 
                       __LINE__, classifysup->supervise_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        else
        {
            /* policy��ģʽ�����Ƕ�cb��ģʽ */
            if(0 == policycb->basic_cfg_cb->mode)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:pid=%lu isn't c2b-mode.\n\n", __FUNCTION__, __LINE__, 
                       classifysup->supervise_key);
                OS_SPRINTF(ucmessage,"***err***%s,%d:pid=%lu isn't c2b-mode.\n\n", __FUNCTION__, 
                           __LINE__, classifysup->supervise_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
    }

    /* ɾ���������ͷ�ruleid */
    spm_del_acl_rule_driver(classifysup->acl_id, classifysup->rule_id);
    spm_release_policy_acl_ruleid(policycb->ruleid, classifysup->rule_id);

    /* ��ʾɾ���ĸ�if-match��c�е����һ��if-match */
    if(ATG_DCI_RC_OK != lastifmatch)
    {
       /******************************************************************************
        * 1.��ʾ��ʱpolicy���Ѳ�����c2b������Ϣ�����Ѵ��ڵ�c2b��c/bģ�������Ϊ��,�� *
        *   ʱ���ͷ�aclid;                                                           *
        * 2.aclid��intf��policyid��intf�İ󶨹�ϵ����spm_add_logic_flow_classify_node*
        *   ��������ɾ��.                                                            *
        *                                                                            *
        * ע��:                                                                      *
        * ��ɾ����if-matchʱ,�жϸ�if-match�Ƿ�������policy�е����һ��if-match.     *
        * ����policy�󶨵������,ɾ��c1�е�if-match������c1������if-match,��ʱ������ *
        *   if-match�����ò���д����;����ʱ�ٴ�ɾ��c2�е�if-matchʱ,�ᵼ�´�ʱɾ���� *
        *   if-match����������policy�е����һ��if-match,����policy��Ӧacl��ȫ������ *
        *   �Ѿ���ɾ����ȫ,��ʱ�������ͷ�aclid,������ܻ����,�˴���Ҫ�����ϸ����.  *
        ******************************************************************************/
        ret = spm_check_policy_c2b_cfg(policycb);
        if(ATG_DCI_RC_OK == ret)
        {
           /********************************************************************** 
            * ɾ����intf�󶨵�policy�����һ��cb��ʱ,��ɾ���������ͷ�aclid.����, *
            * policy����Ӧ��aclid��intf֮��İ󶨹�ϵ��policyid��intf���ʱɾ��. *
            **********************************************************************/
            policycb->match_type = MATCH_ERROR;
            spm_release_policy_acl_id(policycb->acl_id);
            policycb->acl_id = 0;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_del_classify_rule_cfg
   ��������  : ����policyģ���������
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_del_classify_rule_cfg(SPM_QOS_CLASSIFY_CB *pstclassify)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ifmatchid  = 0;
    NBB_ULONG classifyid = 0;
    NBB_BYTE lastifmatch = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_POLICY_CLASSIFY_CB *classifysup = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstclassify)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* �ж��Ƿ���c�е����һ��if-match */
    ifmatchid  = pstclassify->classify_key.if_match_id;
    classifyid = pstclassify->classify_key.qos_classify_index;
    if(NULL == NBB_NEXT_IN_LIST(g_classify_id_instance[classifyid].classify_group))    
    {
        /* ������if-match��cģ���е����һ��if-match */
        lastifmatch = 1;
        g_classify_id_instance[classifyid].match_type = MATCH_ERROR;
        printf("#####Del the last if-match(cid=%lu/iid=%lu)#####\n", classifyid, ifmatchid);
        OS_SPRINTF(ucmessage,"#####Del the last if-match(cid=%lu/iid=%lu)#####\n", 
                   classifyid, ifmatchid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
    }

    /* �жϸ�if-match�е�classifysup���Ƿ�Ϊ��,����Ϊ������Ҫɾ������ */
    for(classifysup = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(  \
                       pstclassify->spm_classify_policy_tree); 
        classifysup != NULL; 
        classifysup = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(  \
                       pstclassify->spm_classify_policy_tree))
    {
        /* ɾ�������ͷ�ruleid,���ж��Ƿ���Ҫ�ͷ�policy��aclid */
        spm_del_rule_policy_supervise(classifysup, lastifmatch);
        AVLL_DELETE(pstclassify->spm_classify_policy_tree, classifysup->tree_node);
        spm_free_policy_classify_cb(classifysup);
        classifysup = NULL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   �� �� ��  : spm_write_classify_rule_by_c2b
   ��������  : ����policyģ���������
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_add_classify_rule_cfg(SPM_QOS_CLASSIFY_CB *pstclassify, 
    SPM_QOS_POLICY_CLASSIFY_CB *policy_classify)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG posid  = 0;
    NBB_ULONG ruleid = 0;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *policycb  = NULL;
    SPM_QOS_ACTION_CB *pstaction = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstclassify) || (NULL == policy_classify))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pstclassify->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, 
                       __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    /* ���Ҹ�policyģ��,��ʱ��ģ��һ������,���򱨴� */
    policycb = AVLL_FIND(g_qos_policy_tree, &(policy_classify->supervise_key));
    if(NULL == policycb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:no policyid=%lu cfg.\n\n", __FUNCTION__, __LINE__, 
               policy_classify->supervise_key);
        OS_SPRINTF(ucmessage,"***err***%s,%d:no policyid=%lu cfg.\n\n", __FUNCTION__, 
                   __LINE__, policy_classify->supervise_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL; 
    }
    else
    {
        if(NULL == policycb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:no pid=%lu basic-cfg.\n\n", __FUNCTION__, __LINE__, 
                   policy_classify->supervise_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no pid=%lu basic-cfg.\n\n", __FUNCTION__, 
                       __LINE__, policy_classify->supervise_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        else
        {
            /* policy��ģʽ�����Ƕ�cb��ģʽ */
            if(0 == policycb->basic_cfg_cb->mode)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:pid=%lu isn't c2b-mode.\n\n", __FUNCTION__, __LINE__, 
                       policy_classify->supervise_key);
                OS_SPRINTF(ucmessage,"***err***%s,%d:pid=%lu isn't c2b-mode.\n\n", __FUNCTION__, 
                           __LINE__, policy_classify->supervise_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
    }

    /* c2b���е�behaviorģ�������� */
    pstaction = AVLL_FIND(g_qos_action_tree, &(policy_classify->behavior_id));
    if(NULL == pstaction)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:no bid=%lu cfg with cid=%lu in pid=%lu.\n\n", __FUNCTION__, 
               __LINE__, policy_classify->behavior_id, pstclassify->classify_key.qos_classify_index, 
               policy_classify->supervise_key);
        OS_SPRINTF(ucmessage,"***err***%s,%d:no bid=%lu cfg with cid=%lu in pid=%lu.\n\n", 
                   __FUNCTION__, __LINE__, policy_classify->behavior_id, 
                   pstclassify->classify_key.qos_classify_index, policy_classify->supervise_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;  
    }
    else
    {
        if(NULL == pstaction->flow_act_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:no flow_act_cfg of bid=%lu/cid=%lu in pid=%lu.\n\n", __FUNCTION__, 
                   __LINE__, policy_classify->behavior_id, pstclassify->classify_key.qos_classify_index, 
                   policy_classify->supervise_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no flow_act_cfg of bid=%lu/cid=%lu in pid=%lu.\n\n", 
                       __FUNCTION__, __LINE__, policy_classify->behavior_id, 
                       pstclassify->classify_key.qos_classify_index, policy_classify->supervise_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    /* ����ruleid */
    ruleid = spm_apply_policy_acl_ruleid(policycb->ruleid);
    if(0 == ruleid)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:spm_apply_policy_acl_ruleid failed.\n\n", __FUNCTION__, 
               __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:spm_apply_policy_acl_ruleid failed.\n\n", 
                   __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* posid�ǳɹ�д��acl����������ķ���ֵ */
    ret = spm_set_policy_classify_acl(pstclassify, pstaction, policy_classify->acl_id, ruleid, &posid);
    if(ATG_DCI_RC_OK != ret)
    {
        spm_release_policy_acl_ruleid(policycb->ruleid, ruleid);
        printf("***err***%s,%d:cid=%lu/iid=%lu spm_set_policy_classify_acl failed.\n\n", 
               __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
               pstclassify->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:cid=%lu/iid=%lu spm_set_policy_classify_acl failed."
                   "\n\n", __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        /* posid��дrule����ʱ�����ķ���ֵ */
        policy_classify->posid   = posid;
        policy_classify->rule_id = ruleid;
        rv = AVLL_INSERT(pstclassify->spm_classify_policy_tree, policy_classify->tree_node);
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   �� �� ��  : spm_update_classify_rule_cfg
   ��������  : ����if-matchƥ�����
   �������  : policyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_update_classify_rule_cfg(SPM_QOS_CLASSIFY_CB *pstclassify, 
    SPM_QOS_POLICY_CLASSIFY_CB *policy_classify)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ACL_RULE_T aclrule;
    NBB_USHORT unit = 0;
    NBB_BYTE  tcp_flag = 0;
    NBB_USHORT ussvlan = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_PORT_INFO_CB pstportinfo = {0};
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstclassify) || (NULL == policy_classify))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pstclassify->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, 
                       __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        else
        {
            if(0 == policy_classify->posid)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:update cid=%lu/iid=%lu failed.\n\n", __FUNCTION__, 
                       __LINE__, pstclassify->classify_key.qos_classify_index,
                       pstclassify->classify_key.if_match_id);
                OS_SPRINTF(ucmessage,"***err***%s,%d:update cid=%lu/iid=%lu failed.\n\n", 
                           __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index,
                           pstclassify->classify_key.if_match_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;  
            }
        }
    }

    OS_MEMSET(&aclrule, 0, sizeof(ACL_RULE_T));
    switch(pstclassify->basic_cfg_cb->match_type)
    {
        case MATCH_PORT:
        if(NULL == pstclassify->Port_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_PORT_DATA of cid=%lu/iid=%lu param err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_PORT_DATA of cid=%lu/iid=%lu "
                       "param err.\n", __FUNCTION__, __LINE__, 
                       pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        OS_MEMSET(&pstportinfo, 0, sizeof(SPM_PORT_INFO_CB));
        ret = spm_get_portid_from_logical_port_index(pstclassify->Port_cfg_cb->index, &pstportinfo);
        if(ATG_DCI_RC_OK != ret)
        {
            printf("***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->Port_cfg_cb->index);
            OS_SPRINTF(ucmessage,"***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index"
                       " err.\n", __FUNCTION__, __LINE__, pstclassify->Port_cfg_cb->index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* VE�� */
        if((0 == pstportinfo.slot_id) && (3 == pstportinfo.port_type))
        {
            /* ����VE�� */
            if(0 == pstportinfo.svlan)
            {
               aclrule.tAclKey.ePortType = ACL_L2VE;
               aclrule.tAclKey.port = pstportinfo.port_id;
               aclrule.tAclMask.ePortType = 0xffff;
               aclrule.tAclMask.port      = 0xffff;
            }

            /* ����VE�� */
            else
            {
               aclrule.tAclKey.ePortType = ACL_L3VE;
               aclrule.tAclKey.port = pstportinfo.port_id;
               aclrule.tAclMask.ePortType = 0xffff;
               aclrule.tAclMask.port      = 0xffff;
            }
        }

        /* ��ͨlogic��lag�� */
        else
        {
            /* ����� */
            if(0 == pstportinfo.svlan)
            {
                ret = spm_get_vlan_from_logical_port_index(pstclassify->Port_cfg_cb->index, &ussvlan);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("***err***%s,%d:intf-log=%lu spm_get_vlan_from_logical_port_index err.\n", 
                           __FUNCTION__, __LINE__, pstclassify->Port_cfg_cb->index);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-log=%lu spm_get_vlan_from_logical_port_index"
                               " err.\n", __FUNCTION__, __LINE__, pstclassify->Port_cfg_cb->index);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
                if(0 != ussvlan)
                {
                    aclrule.tAclKey.oVid  = ussvlan;
                    aclrule.tAclMask.oVid = 0x0fff;
                }
            }

            /* ����� */
            else
            {
                aclrule.tAclKey.oVid  = pstportinfo.svlan;
                aclrule.tAclMask.oVid = 0x0fff;
            }
            aclrule.tAclKey.ePortType  = ACL_PORT;
            aclrule.tAclKey.port       = pstportinfo.port_id;
            aclrule.tAclMask.ePortType = 0xffff;
            aclrule.tAclMask.port      = 0xffff;     
        }
        break;

        case MATCH_ETH_HEADER:
        if(NULL == pstclassify->Eth_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_ETH_HEADER of cid=%lu/iid=%lu param err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_ETH_HEADER of cid=%lu/iid=%lu "
                       "param err.\n", __FUNCTION__, __LINE__, 
                       pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        OS_MEMCPY(&(aclrule.tAclKey.smac[0]), &(pstclassify->Eth_cfg_cb->src_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(aclrule.tAclKey.dmac[0]), &(pstclassify->Eth_cfg_cb->dst_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(aclrule.tAclMask.smac[0]), &(pstclassify->Eth_cfg_cb->src_mac_mask[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(aclrule.tAclMask.dmac[0]), &(pstclassify->Eth_cfg_cb->dst_mac_mask[0]), ATG_DCI_MAC_LEN);
        aclrule.tAclKey.ovlan_pri = pstclassify->Eth_cfg_cb->vlan_prio;
        aclrule.tAclMask.ovlan_pri = pstclassify->Eth_cfg_cb->vlan_prio_mask;
        aclrule.tAclKey.ethType  = 0x0800;
        aclrule.tAclMask.ethType = 0xffff;
        break;

        case MATCH_IPV4:
        if(NULL == pstclassify->ipv4_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_IPV4_FRAME of cid=%lu/iid=%lu param err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_IPV4_FRAME of cid=%lu/iid=%lu "
                       "param err.\n", __FUNCTION__, __LINE__, 
                       pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        aclrule.tAclKey.dip = pstclassify->ipv4_cfg_cb->dst_ip;
        aclrule.tAclMask.dip = pstclassify->ipv4_cfg_cb->dst_ip_mask;
        aclrule.tAclKey.sip = pstclassify->ipv4_cfg_cb->src_ip;
        aclrule.tAclMask.sip = pstclassify->ipv4_cfg_cb->src_ip_mask;
        aclrule.tAclKey.tos = (pstclassify->ipv4_cfg_cb->dscp);
        aclrule.tAclMask.tos = (pstclassify->ipv4_cfg_cb->dscp_mask);
        aclrule.tAclKey.l3Protocol = pstclassify->ipv4_cfg_cb->l3_protocol;
        aclrule.tAclMask.l3Protocol = pstclassify->ipv4_cfg_cb->l3_protocol_mask;
        aclrule.tAclKey.ethType = 0x0800;
        aclrule.tAclMask.ethType = 0xffff;
        aclrule.tAclKey.ipHeaderMF = pstclassify->ipv4_cfg_cb->mf;
        aclrule.tAclMask.ipHeaderMF = pstclassify->ipv4_cfg_cb->mf_mask;
        aclrule.tAclKey.ipHeaderDF = pstclassify->ipv4_cfg_cb->df;
        aclrule.tAclMask.ipHeaderDF = pstclassify->ipv4_cfg_cb->df_mask;
        aclrule.tAclKey.l4SrcPort = pstclassify->ipv4_cfg_cb->src_port;
        aclrule.tAclMask.l4SrcPort = pstclassify->ipv4_cfg_cb->src_port_mask;
        aclrule.tAclKey.l4DstPort = pstclassify->ipv4_cfg_cb->dst_port;
        aclrule.tAclMask.l4DstPort = pstclassify->ipv4_cfg_cb->dst_port_mask;
        if((1 == pstclassify->ipv4_cfg_cb->l3_protocol)         && 
           (0xff == pstclassify->ipv4_cfg_cb->l3_protocol_mask) && 
           (0 != pstclassify->ipv4_cfg_cb->payload_mask[0]))
        {
            aclrule.tAclKey.icmpType = pstclassify->ipv4_cfg_cb->payload[0];
            aclrule.tAclMask.icmpType = pstclassify->ipv4_cfg_cb->payload_mask[0];
        }
        if((1 == pstclassify->ipv4_cfg_cb->l3_protocol)         && 
           (0xff == pstclassify->ipv4_cfg_cb->l3_protocol_mask) && 
           (0 != pstclassify->ipv4_cfg_cb->payload_mask[1]))
        {
            aclrule.tAclKey.icmpCode = pstclassify->ipv4_cfg_cb->payload[1];
            aclrule.tAclMask.icmpCode = pstclassify->ipv4_cfg_cb->payload_mask[1];
        }
        break;

        case MATCH_IP_TCP:
        if(NULL == pstclassify->ipTcp_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_IPV4_TCP of cid=%lu/iid=%lu param err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_IPV4_TCP of cid=%lu/iid=%lu "
                       "param err.\n", __FUNCTION__, __LINE__, 
                       pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        aclrule.tAclKey.dip = pstclassify->ipTcp_cfg_cb->dst_ip;
        aclrule.tAclMask.dip = pstclassify->ipTcp_cfg_cb->dst_ip_mask;
        aclrule.tAclKey.sip = pstclassify->ipTcp_cfg_cb->src_ip;
        aclrule.tAclMask.sip = pstclassify->ipTcp_cfg_cb->src_ip_mask;
        aclrule.tAclKey.tos = (pstclassify->ipTcp_cfg_cb->dscp);
        aclrule.tAclMask.tos = (pstclassify->ipTcp_cfg_cb->dscp_mask);
        aclrule.tAclKey.l3Protocol = 6;
        aclrule.tAclMask.l3Protocol = 0xff;
        aclrule.tAclKey.ethType = 0x0800;
        aclrule.tAclMask.ethType = 0xffff;
        aclrule.tAclKey.l4SrcPort = pstclassify->ipTcp_cfg_cb->src_port;
        aclrule.tAclMask.l4SrcPort = pstclassify->ipTcp_cfg_cb->src_port_mask;
        aclrule.tAclKey.l4DstPort = pstclassify->ipTcp_cfg_cb->dst_port;
        aclrule.tAclMask.l4DstPort = pstclassify->ipTcp_cfg_cb->dst_port_mask; 
        OS_MEMCPY(&tcp_flag,&(pstclassify->ipTcp_cfg_cb->tcp_flag),1);
        aclrule.tAclKey.tcp_flag = tcp_flag;
        aclrule.tAclMask.tcp_flag = pstclassify->ipTcp_cfg_cb->tcp_flag_mask;
        break;

        case MATCH_IP_UDP:
        if(NULL == pstclassify->ipUdp_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_IPV4_UDP of cid=%lu/iid=%lu param err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_IPV4_UDP of cid=%lu/iid=%lu "
                       "param err.\n", __FUNCTION__, __LINE__, 
                       pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        aclrule.tAclKey.ethType = 0x0800;
        aclrule.tAclMask.ethType = 0xffff;
        aclrule.tAclKey.dip = pstclassify->ipUdp_cfg_cb->dst_ip;
        aclrule.tAclMask.dip = pstclassify->ipUdp_cfg_cb->dst_ip_mask;
        aclrule.tAclKey.sip = pstclassify->ipUdp_cfg_cb->src_ip;
        aclrule.tAclMask.sip = pstclassify->ipUdp_cfg_cb->src_ip_mask;
        aclrule.tAclKey.tos = (pstclassify->ipUdp_cfg_cb->dscp);
        aclrule.tAclMask.tos = (pstclassify->ipUdp_cfg_cb->dscp_mask);
        aclrule.tAclKey.l3Protocol = 0x11;
        aclrule.tAclMask.l3Protocol = 0xff;
        aclrule.tAclKey.l4SrcPort = pstclassify->ipUdp_cfg_cb->src_port;
        aclrule.tAclMask.l4SrcPort = pstclassify->ipUdp_cfg_cb->src_port_mask;
        aclrule.tAclKey.l4DstPort = pstclassify->ipUdp_cfg_cb->dst_port;
        aclrule.tAclMask.l4DstPort = pstclassify->ipUdp_cfg_cb->dst_port_mask;
        break;

        case MATCH_IPV6:
        if(NULL == pstclassify->ipv6_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_IPV6_FRAME of cid=%lu/iid=%lu param err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_IPV6_FRAME of cid=%lu/iid=%lu "
                       "param err.\n", __FUNCTION__, __LINE__, 
                       pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        aclrule.tAclKey.ethType = 0x86dd;
        aclrule.tAclMask.ethType = 0xffff;
        OS_MEMCPY(aclrule.tAclKey.dipv6,pstclassify->ipv6_cfg_cb->dst_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(pstclassify->ipv6_cfg_cb->dst_ip_mask_len,
            (NBB_ULONG*)&(aclrule.tAclMask.dipv6));
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        OS_MEMCPY(aclrule.tAclKey.sipv6,pstclassify->ipv6_cfg_cb->src_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(pstclassify->ipv6_cfg_cb->src_ip_mask_len,
            (NBB_ULONG*)&(aclrule.tAclMask.sipv6));
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        aclrule.tAclKey.l3Protocol = pstclassify->ipv6_cfg_cb->l3_protocol;
        aclrule.tAclMask.l3Protocol = pstclassify->ipv6_cfg_cb->l3_protocol_mask;
        aclrule.tAclKey.l4SrcPort = pstclassify->ipv6_cfg_cb->src_port;
        aclrule.tAclMask.l4SrcPort = pstclassify->ipv6_cfg_cb->src_port_mask;
        aclrule.tAclKey.l4DstPort = pstclassify->ipv6_cfg_cb->dst_port;
        aclrule.tAclMask.l4DstPort = pstclassify->ipv6_cfg_cb->dst_port_mask;
        aclrule.tAclKey.tos = pstclassify->ipv6_cfg_cb->tos;
        aclrule.tAclMask.tos = pstclassify->ipv6_cfg_cb->tos_mask;
        break;

        case MATCH_NOIP:
        if(NULL == pstclassify->noIp_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_NONE_IP of cid=%lu/iid=%lu param err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_NONE_IP of cid=%lu/iid=%lu "
                       "param err.\n", __FUNCTION__, __LINE__, 
                       pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        aclrule.tAclKey.ethType  = pstclassify->noIp_cfg_cb->etype;
        aclrule.tAclMask.ethType = pstclassify->noIp_cfg_cb->etype_mask;
        OS_MEMCPY(&(aclrule.tAclKey.customerData6),&(pstclassify->noIp_cfg_cb->payload[0]),4);
        OS_MEMCPY(&(aclrule.tAclKey.customerData5),&(pstclassify->noIp_cfg_cb->payload[4]),4);
        OS_MEMCPY(&(aclrule.tAclKey.customerData4),&(pstclassify->noIp_cfg_cb->payload[8]),4);
        OS_MEMCPY(&(aclrule.tAclMask.customerData6),&(pstclassify->noIp_cfg_cb->payload_mask[0]),4);
        OS_MEMCPY(&(aclrule.tAclMask.customerData5),&(pstclassify->noIp_cfg_cb->payload_mask[4]),4);
        OS_MEMCPY(&(aclrule.tAclMask.customerData4),&(pstclassify->noIp_cfg_cb->payload_mask[8]),4);
        break;

        default:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:match-type=%d of cid=%lu/iid=%lu err.\n", __FUNCTION__, 
               __LINE__, pstclassify->match_type, pstclassify->classify_key.qos_classify_index, 
               pstclassify->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:match-type=%d of cid=%lu/iid=%lu err.\n", 
                   __FUNCTION__, __LINE__, pstclassify->match_type, 
                   pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = fhdrv_acl_update_rule(unit, policy_classify->posid, &aclrule);
        if(ATG_DCI_RC_OK != ret)
        {
            printf("***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) fhdrv_acl_update_rule err.\n", 
                   __FUNCTION__, __LINE__, policy_classify->posid, policy_classify->acl_id, 
                   policy_classify->rule_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) "
                       "fhdrv_acl_update_rule err.\n", __FUNCTION__, __LINE__, policy_classify->posid, 
                       policy_classify->acl_id, policy_classify->rule_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
#endif
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   �� �� ��  : spm_set_classify_rule_cfg
   ��������  : ����classifyid�Ƿ�policy����,ͬʱpolicyid�Ƿ�intf����
   �������  : ���볤��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_set_classify_rule_cfg(SPM_QOS_CLASSIFY_CB *classifycb, 
    NBB_BYTE opertype)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG classifyid = 0;
    NBB_LONG  rv  = ATG_DCI_RC_OK;
    NBB_LONG  ret = ATG_DCI_RC_OK;
    NBB_CHAR  ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_ACTION_CB   *pstaction = NULL;
    SPM_QOS_CLASSIFY_CB *oldclassifycb = NULL;
    SPM_QOS_POLICY_CLASSIFY_CB *classifysup = NULL;
    SPM_QOS_POLICY_CLASSIFY_CB *policy_classify = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == classifycb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if((0 == classifycb->classify_key.qos_classify_index) || 
           (MAX_FLOW_CLASSIFY_PRI <= classifycb->classify_key.qos_classify_index))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, 
                       __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        else
        {
            /* ����:����/���� */
            if((ATG_DCI_OPER_UPDATE != opertype) && (ATG_DCI_OPER_ADD != opertype))
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, 
                           __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
        }
    }

   /*************************************************************************************************
    * ����if-match:                                                                                 *
    * ��c/b��ϳ�cb�Ժ�,ɾ��c�е����һ��if-match��������Ҫ�·�ɾ��cb�Ե�����,�����ʱc�е�����Ϊ��.*
    * ��policy��intf��ʱ,��ʱ����c������if-matchֻ�Ὣ��if-match�������ڴ���,�����Ὣ��д��������.*
    *************************************************************************************************/
    if(ATG_DCI_OPER_ADD == opertype)
    {
       /******************************************************************************************  
        * �ж�c���Ƿ����if-match,��������д������classifysup��;�������ڽ�����������if-match����.*
        * ��ʱ���policy(���c���)��intf��,��������ز�δ��ɾ�����һ��if-match��ɾ��cb�İ� *
        * ��ϵ,������Ȼ��ʱ��cΪ��,��Ӧ�ò������if-match����,����д����.                        *
        ******************************************************************************************/
        classifyid    = classifycb->classify_key.qos_classify_index;
        oldclassifycb = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(   \
                         g_classify_id_instance[classifyid].classify_group);
        if(NULL != oldclassifycb)
        {
            for(policy_classify = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(  \
                                   oldclassifycb->spm_classify_policy_tree); 
                policy_classify != NULL; 
                policy_classify = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(   \
                                   oldclassifycb->spm_classify_policy_tree, \
                                   policy_classify->tree_node))
            {
                classifysup = spm_alloc_policy_classify_cb(policy_classify->supervise_key);
                if(NULL == classifysup)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    printf("***err***%s,%d:spm_alloc_policy_classify_cb failed.\n\n", __FUNCTION__, 
                           __LINE__);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_policy_classify_cb failed.\n\n", 
                               __FUNCTION__, __LINE__);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL; 
                }
                else
                {
                    classifysup->acl_id      = policy_classify->acl_id;
                    classifysup->behavior_id = policy_classify->behavior_id;
                }

                /* ����rule����,��ʱ���б������¼,���˳�����ѭ�� */
                ret = spm_add_classify_rule_cfg(classifycb, classifysup);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("***err***%s,%d:add cid=%lu/iid=%lu in pid=%lu failed.\n\n", 
                           __FUNCTION__, __LINE__, classifycb->classify_key.qos_classify_index, 
                           classifycb->classify_key.if_match_id, policy_classify->supervise_key);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:add cid=%lu/iid=%lu in pid=%lu failed.\n\n", 
                               __FUNCTION__, __LINE__, classifycb->classify_key.qos_classify_index, 
                               classifycb->classify_key.if_match_id, policy_classify->supervise_key);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    spm_free_policy_classify_cb(classifysup);
                    classifysup = NULL;
                    continue;
                }
                else
                {
                    /* д�ɹ�������� */
                    rv = AVLL_INSERT(classifycb->spm_classify_policy_tree, classifysup->tree_node);
                }
            }
        }
        else
        {
            /* ��֮ǰ������if-match,Ӧ�ò��ʱ���������ò�д����.�˴���ִ���κβ���. */
        }
    }

    /* ����if-match */
    else
    {
        for(policy_classify = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(classifycb->spm_classify_policy_tree); 
            policy_classify != NULL; policy_classify = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(   \
            classifycb->spm_classify_policy_tree, policy_classify->tree_node))
        {
            /* ����rule����,��ʱ���б������¼���� */
            ret = spm_update_classify_rule_cfg(classifycb, policy_classify);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("***err***%s,%d:update cid=%lu/iid=%lu in pid=%lu failed.\n\n", __FUNCTION__, 
                       __LINE__, classifycb->classify_key.qos_classify_index, 
                       classifycb->classify_key.if_match_id, policy_classify->supervise_key);
                OS_SPRINTF(ucmessage,"***err***%s,%d:update cid=%lu/iid=%lu in pid=%lu failed.\n\n", 
                           __FUNCTION__, __LINE__, classifycb->classify_key.qos_classify_index, 
                           classifycb->classify_key.if_match_id, policy_classify->supervise_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage); 
            }
        }
    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_rcv_dci_set_classify
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
NBB_VOID spm_rcv_dci_set_classify(ATG_DCI_SET_QOS_CLASSIFY *pstsetips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_QOS_CLASSIFY_CB *pstcb = NULL;
    ATG_DCI_QOS_CLASSIFY_KEY ulkey = {0};
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *pucbasicdatastart = NULL;
    NBB_BYTE *pucportdatastart  = NULL;
    NBB_BYTE *pucethdatastart   = NULL;
    NBB_BYTE *pucipv4datastart  = NULL;
    NBB_BYTE *puciptcpdatastart = NULL;
    NBB_BYTE *pucipv6datastart  = NULL;
    NBB_BYTE *pucnoipdatastart  = NULL;
    NBB_BYTE *pucipudpdatastart = NULL;

    NBB_ULONG uloperbasic = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperport  = ATG_DCI_OPER_NULL;
    NBB_ULONG ulopereth   = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperipv4  = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperiptcp = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperipv6  = ATG_DCI_OPER_NULL;
    NBB_ULONG ulopernoip  = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperipudp = ATG_DCI_OPER_NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstsetips)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if((0 == pstsetips->key.qos_classify_index) || 
           (MAX_FLOW_CLASSIFY_PRI <= pstsetips->key.qos_classify_index))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    /* ��ȡ�����ò��� */
    uloperbasic = pstsetips->oper_basic;
    uloperport  = pstsetips->oper_port;
    ulopereth   = pstsetips->oper_eth_header;
    uloperipv4  = pstsetips->oper_ipv4_frame;
    uloperiptcp = pstsetips->oper_ipv4_tcp;
    uloperipv6  = pstsetips->oper_ipv6_frame;
    ulopernoip  = pstsetips->oper_none_ip;
    uloperipudp = pstsetips->oper_ipv4_udp;

    /* ��ȡ�����õ�ƫ�Ƶ�ַ */
    pucbasicdatastart = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->basic_data);
    pucportdatastart  = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->port_data);
    pucethdatastart   = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->eth_header_data);
    pucipv4datastart  = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->ipv4_frame_data);
    puciptcpdatastart = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->ipv4_tcp_data);
    pucipv6datastart  = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->ipv6_frame_data);
    pucnoipdatastart  = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->none_ip_data);
    pucipudpdatastart = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->ipv4_udp_data);

    pstsetips->return_code = ATG_DCI_RC_OK;
    OS_MEMCPY(&ulkey, &(pstsetips->key), sizeof(ATG_DCI_QOS_CLASSIFY_KEY));
    pstcb = AVLL_FIND(g_qos_classify_tree, &ulkey);
    if(NULL == pstcb)
    {
        /* ����ɾ�� */
        if(TRUE == pstsetips->delete_struct)
        {
            pstsetips->return_code = ATG_DCI_RC_DEL_FAILED;
            printf("***err***%s,%d:no cid=%lu/iid=%lu cfg, delete err.\n\n", __FUNCTION__, 
                   __LINE__, ulkey.qos_classify_index, ulkey.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no cid=%lu/iid=%lu cfg, delete err.\n\n", 
                       __FUNCTION__, __LINE__, ulkey.qos_classify_index, ulkey.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;  
        }

        /* �������� */
        else
        {
            pstcb = spm_alloc_classify_cb(&ulkey);
            if(NULL == pstcb)
            {
                pstsetips->return_code = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:spm_alloc_classify_cb failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_classify_cb failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ret = spm_set_classify_basic_cfg(pstcb, uloperbasic, pucbasicdatastart);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_free_classify_cb(pstcb);
                pstsetips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstsetips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:spm_set_classify_basic_cfg failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_set_classify_basic_cfg failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* ʹ�ø��º��match_type����ֵ���ж�����,�������� */
            switch(pstcb->basic_cfg_cb->match_type)
            {
                case MATCH_PORT:
                ret = spm_set_classify_port_cfg(pstcb, uloperport, pucportdatastart);
                pstsetips->port_return_code = ret;
                break;

                case MATCH_ETH_HEADER:
                ret = spm_set_classify_eth_cfg(pstcb, ulopereth, pucethdatastart);
                pstsetips->eth_header_return_code = ret;
                break;

                case MATCH_IPV4:
                ret = spm_set_classify_ipv4_cfg(pstcb, uloperipv4, pucipv4datastart);
                pstsetips->ipv4_frame_return_code = ret;
                break;

                case MATCH_IP_TCP:
                ret = spm_set_classify_iptcp_cfg(pstcb, uloperiptcp, puciptcpdatastart);
                pstsetips->ipv4_tcp_return_code = ret;
                break;

                case MATCH_IP_UDP:
                ret = spm_set_classify_ipudp_cfg(pstcb, uloperipudp, pucipudpdatastart);
                pstsetips->ipv4_udp_return_code = ret;
                break;

                case MATCH_IPV6:
                ret = spm_set_classify_ipv6_cfg(pstcb, uloperipv6, pucipv6datastart);
                pstsetips->ipv6_frame_return_code = ret;
                break;

                case MATCH_NOIP:
                ret = spm_set_classify_noip_cfg(pstcb, ulopernoip, pucnoipdatastart);
                pstsetips->none_ip_return_code = ret;
                break;

                /* ��ʱ��֧��classify����aclid */
                default:
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:match-type=%d of cid=%lu/iid=%lu mismatch.\n", __FUNCTION__, 
                       __LINE__, pstcb->basic_cfg_cb->match_type, ulkey.qos_classify_index, 
                       ulkey.if_match_id);
                OS_SPRINTF(ucmessage,"***err***%s,%d:match-type=%d of cid=%lu/iid=%lu mismatch.\n", 
                           __FUNCTION__, __LINE__, pstcb->basic_cfg_cb->match_type, 
                           ulkey.qos_classify_index, ulkey.if_match_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* ����ʧ�ܺ�ɾ��������ڴ�ռ� */
            if(ATG_DCI_RC_OK != ret)
            {
                pstsetips->return_code = ret;
                spm_free_classify_cb(pstcb);
                goto EXIT_LABEL;
            }
            else
            {
                /* ����,д����(���жϷ���ֵ),������classifysup���� */
                rv = spm_set_classify_rule_cfg(pstcb, ATG_DCI_OPER_ADD);
                rv = AVLL_INSERT(g_qos_classify_tree, pstcb->spm_classify_node);
                NBB_INSERT_BEFORE(g_classify_id_instance[ulkey.qos_classify_index].classify_group,
                                  pstcb->spm_classify_group_node);
            }
        }
    }
    else
    {
        /* ɾ������ */
        if(TRUE == pstsetips->delete_struct)
        {
            /* ��classify����������ɾ��classify����Ľڵ���Ϣ */
            AVLL_DELETE(g_qos_classify_tree, pstcb->spm_classify_node);
            NBB_REMOVE_FROM_LIST(pstcb->spm_classify_group_node);

            /* ɾ������������ü��ͷ��ڴ� */
            spm_del_classify_rule_cfg(pstcb);
            spm_free_classify_cb(pstcb);
            pstcb = NULL;
        }

        /* ���²��� */
        else
        {
            ret = spm_set_classify_basic_cfg(pstcb, uloperbasic, pucbasicdatastart);
            if(ATG_DCI_RC_OK != ret)
            {
                pstsetips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstsetips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:spm_set_classify_basic_cfg failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_set_classify_basic_cfg failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* ʹ�ø��º��match_type����ֵ���ж�����,�������� */
            switch(pstcb->basic_cfg_cb->match_type)
            {
                case MATCH_PORT:
                ret = spm_set_classify_port_cfg(pstcb, uloperport, pucportdatastart);
                pstsetips->port_return_code = ret;
                break;

                case MATCH_ETH_HEADER:
                ret = spm_set_classify_eth_cfg(pstcb, ulopereth, pucethdatastart);
                pstsetips->eth_header_return_code = ret;
                break;

                case MATCH_IPV4:
                ret = spm_set_classify_ipv4_cfg(pstcb, uloperipv4, pucipv4datastart);
                pstsetips->ipv4_frame_return_code = ret;
                break;

                case MATCH_IP_TCP:
                ret = spm_set_classify_iptcp_cfg(pstcb, uloperiptcp, puciptcpdatastart);
                pstsetips->ipv4_tcp_return_code = ret;
                break;

                case MATCH_IP_UDP:
                ret = spm_set_classify_ipudp_cfg(pstcb, uloperipudp, pucipudpdatastart);
                pstsetips->ipv4_udp_return_code = ret;
                break;

                case MATCH_IPV6:
                ret = spm_set_classify_ipv6_cfg(pstcb, uloperipv6, pucipv6datastart);
                pstsetips->ipv6_frame_return_code = ret;
                break;

                case MATCH_NOIP:
                ret = spm_set_classify_noip_cfg(pstcb, ulopernoip, pucnoipdatastart);
                pstsetips->none_ip_return_code = ret;
                break;

                /* ��ʱ��֧��classify����aclid */
                default:
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:match-type=%d of cid=%lu/iid=%lu mismatch.\n", __FUNCTION__, 
                       __LINE__, pstcb->basic_cfg_cb->match_type, ulkey.qos_classify_index, 
                       ulkey.if_match_id);
                OS_SPRINTF(ucmessage,"***err***%s,%d:match-type=%d of cid=%lu/iid=%lu mismatch.\n", 
                           __FUNCTION__, __LINE__, pstcb->basic_cfg_cb->match_type, 
                           ulkey.qos_classify_index, ulkey.if_match_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            if(ATG_DCI_RC_OK != ret)
            {
               /*********************************************************************
                * ����rule����δ�ı�match_type�Ҹ��¹���ʧ��ʱ,���������ò���.ͬʱ, *
                * ����rule����ʱ,MATCH_PORT/MATCH_ETH_HEADER��match_type�����ܸı�. *
                *********************************************************************/
                if(pstcb->match_type != pstcb->basic_cfg_cb->match_type)
                {
                   /*****************************************************
                    * �����µ�rule����ı���match_type�Ҹ��¹���ʧ��ʱ, *
                    * �򽫸��¹�����������ڴ�ռ��ͷŲ���ԭmatch_type. *
                    *****************************************************/
                    spm_free_classify_rule_tlv(pstcb, ret);
                    pstcb->basic_cfg_cb->match_type = pstcb->match_type;
                }
                pstsetips->return_code = ret;
                goto EXIT_LABEL;
            }
            else
            {
                /* ��������(���жϷ���ֵ,Ĭ�ϸ��³ɹ�) */
                spm_set_classify_rule_cfg(pstcb, ATG_DCI_OPER_UPDATE);

               /******************************************************
                * ����ǰ��match_type:pstcb->match_type               *
                * ���º��match_type:pstcb->basic_cfg_cb->match_type *
                * �����µ�rule����ı���match_type�Ҹ��¹��̳ɹ�ʱ,  *
                * �ͷų�ʼmatch_type����tlv�ռ䲢����match_type��ֵ. *
                ******************************************************/
                if(pstcb->match_type != pstcb->basic_cfg_cb->match_type)
                {
                    spm_free_classify_rule_tlv(pstcb, ret);
                    pstcb->match_type = pstcb->basic_cfg_cb->match_type;
                }
            }
        }
    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
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
NBB_VOID spm_qos_clear_all_classify(void)
{
    SPM_QOS_CLASSIFY_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_CLASSIFY_CB*)AVLL_FIRST(g_qos_classify_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_CLASSIFY_CB*)AVLL_FIRST(g_qos_classify_tree))
    {
        NBB_REMOVE_FROM_LIST(cfg_cb->spm_classify_group_node);
        AVLL_DELETE(g_qos_classify_tree, cfg_cb->spm_classify_node);
        spm_free_classify_cb(cfg_cb);   
    }

    return;   
}

#endif


#if 3

/*****************************************************************************
   �� �� ��  : spm_qos_classify_init
   ��������  : Ĭ�ϵ�DSӳ���ϵ��ʼ��
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���

*****************************************************************************/
NBB_LONG spm_qos_classify_init(void)
{
    NBB_ULONG i = 0;
    NBB_BUF_SIZE avll_key_offset;

    OS_MEMSET(g_classify_high_acl_id, 0, MAX_CLASSIFY_HIGH_ACL_ID_NUM);
    OS_MEMSET(g_classify_eth_acl_id,  0, MAX_CLASSIFY_ETH_ACL_ID_NUM);
    OS_MEMSET(g_classify_port_acl_id, 0, MAX_CLASSIFY_PORT_ACL_ID_NUM);

    /* CLASSIFY���ʼ�� */
    for (i = 0; i < MAX_FLOW_CLASSIFY_PRI; i++)
    {
        NBB_INIT_ROOT(g_classify_id_instance[i].classify_group);

        avll_key_offset = NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, key);
        AVLL_INIT_TREE(g_classify_id_instance[i].instance_tree, compare_ulong, 
                      (NBB_USHORT)avll_key_offset, 
                      (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, spm_classify_id_node));
        g_classify_id_instance[i].match_type = MATCH_ERROR;
    }

    /* CLASSIFY����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_CLASSIFY_CB, classify_key);
    AVLL_INIT_TREE(g_qos_classify_tree, spm_qos_classify_comp, 
                  (NBB_USHORT)avll_key_offset, 
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_CLASSIFY_CB, spm_classify_node));
}


/*****************************************************************************
   �� �� ��  : spm_cfg_classify_cb_verify
   ��������  : Ĭ�ϵ�DSӳ���ϵ��ʼ��
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���

*****************************************************************************/
NBB_VOID spm_cfg_classify_cb_verify(void)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_CLASSIFY_CB *acl_cb = NULL;

    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;

    ATG_DCI_QOS_CLASSIFY_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_PORT_DATA *port_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_ETH_HEADER *eth_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *ipv4_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_TCP *iptcp_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *ipv6_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_NONE_IP *noip_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_UDP *ipudp_cfg_cb;

    NBB_TRC_ENTRY("spm_cfg_classify_cb_verify");

    for (acl_cb = (SPM_QOS_CLASSIFY_CB*)AVLL_FIRST(g_qos_classify_tree); acl_cb != NULL;
         acl_cb = (SPM_QOS_CLASSIFY_CB*)AVLL_NEXT(g_qos_classify_tree, acl_cb->spm_classify_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify acl_cb %p", acl_cb));
        NBB_VERIFY_MEMORY(acl_cb, sizeof(SPM_QOS_CLASSIFY_CB), MEM_SPM_CLASSIFY_CB);

        for(cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(acl_cb->spm_classify_policy_tree); cfg_cb != NULL;
            cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(acl_cb->spm_classify_policy_tree,cfg_cb->tree_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify policy classify cb %p", cfg_cb));
            NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_POLICY_CLASSIFY_CB),
                              MEM_SPM_CLASSIFY_POLICY_CB);
        }

        basic_cfg_cb = acl_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_BASIC_DATA),
                              MEM_SPM_CLASSIFY_BASIC_CB);
        }

        port_cfg_cb = acl_cb->Port_cfg_cb;
        if(port_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify Port_cfg_cb cb %p", port_cfg_cb));
            NBB_VERIFY_MEMORY(port_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA),
                              MEM_SPM_CLASSIFY_PORT_CB);
        }

        eth_cfg_cb = acl_cb->Eth_cfg_cb;
        if(eth_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify Eth_cfg_cb cb %p", eth_cfg_cb));
            NBB_VERIFY_MEMORY(eth_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER),
                              MEM_SPM_CLASSIFY_ETH_CB);
        }

        ipv4_cfg_cb = acl_cb->ipv4_cfg_cb;
        if(ipv4_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_cfg_cb cb %p", ipv4_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME),
                              MEM_SPM_CLASSIFY_IPV4_CB);
        }

        iptcp_cfg_cb = acl_cb->ipTcp_cfg_cb;
        if(iptcp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipTcp_cfg_cb cb %p", iptcp_cfg_cb));
            NBB_VERIFY_MEMORY(iptcp_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP),
                              MEM_SPM_CLASSIFY_IPTCP_CB);
        }

        ipv6_cfg_cb = acl_cb->ipv6_cfg_cb;
        if(ipv6_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_cfg_cb cb %p", ipv6_cfg_cb));
            NBB_VERIFY_MEMORY(ipv6_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME),
                              MEM_SPM_CLASSIFY_IPV6_CB);
        }

        noip_cfg_cb = acl_cb->noIp_cfg_cb;
        if(noip_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify noIp_cfg_cb cb %p", noip_cfg_cb));
            NBB_VERIFY_MEMORY(noip_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP),
                              MEM_SPM_CLASSIFY_NOIP_CB);
        }

        ipudp_cfg_cb = acl_cb->ipUdp_cfg_cb;
        if(ipudp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipUdp_cfg_cb cb %p", ipudp_cfg_cb));
            NBB_VERIFY_MEMORY(ipudp_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP),
                              MEM_SPM_CLASSIFY_IPUDP_CB);
        }
        
    }

    NBB_TRC_EXIT();

    return;
}

#endif
