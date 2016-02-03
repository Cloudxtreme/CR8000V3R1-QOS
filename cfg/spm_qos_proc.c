/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_proc.c
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
#include <TmApi.h>

//static NBB_LONG sampling_percent[] = {1,2,10,100,1000,10000};

/* ȫ�ִ�ӡ����qos_acl_action_print */
extern NBB_BYTE qos_phy_cfg_print;

/* ȫ�ִ�ӡ����qos_acl_action_print */
extern NBB_BYTE qos_log_cfg_print;


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
NBB_INT spm_qos_logic_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_INTF_KEY *key1 = (SPM_QOS_LOGIC_INTF_KEY *)keyarg1;
    SPM_QOS_LOGIC_INTF_KEY *key2 = (SPM_QOS_LOGIC_INTF_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&key1->index, &key2->index NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->svlan, &key2->svlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->cvlan, &key2->cvlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_logic_flow_key_compare
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
NBB_INT spm_qos_logic_flow_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_FLOW_KEY *key1 = (SPM_QOS_LOGIC_FLOW_KEY *)keyarg1;
    SPM_QOS_LOGIC_FLOW_KEY *key2 = (SPM_QOS_LOGIC_FLOW_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&key1->index, &key2->index NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->svlan, &key2->svlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->cvlan, &key2->cvlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_logic_flow_key_compare
   ��������  : �߼��˿������ıȽϺ���
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
NBB_INT spm_qos_port_wred_key_compare(NBB_VOID *keyarg1, 
    NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_PORT_WRED_KEY *key1 = (SPM_PORT_WRED_KEY *)keyarg1;
    SPM_PORT_WRED_KEY *key2 = (SPM_PORT_WRED_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&key1->index, &key2->index NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_byte(&key1->cos, &key2->cos NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_logic_key_compare
   ��������  : LSP_TX���ıȽϺ���
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
NBB_INT spm_qos_lsp_tx_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    CRTXLSP_KEY *key1 = (CRTXLSP_KEY *)keyarg1;
    CRTXLSP_KEY *key2 = (CRTXLSP_KEY *)keyarg2;
    NBB_INT rv = 0;

    rv = compare_ulong(&key1->ingress, &key2->ingress NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->egress, &key2->egress NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->tunnelid, &key2->tunnelid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->lspid, &key2->lspid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return(rv);
}


/*****************************************************************************
   �� �� ��  : spm_qos_c2b_comp
   ��������  : c2b�����ıȽϺ���
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
NBB_INT spm_qos_c2b_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *key1 = (ATG_DCI_QOS_POLICY_CB2QOS_DATA *)keyarg1;
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *key2 = (ATG_DCI_QOS_POLICY_CB2QOS_DATA *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&key1->qos_classify_index, &key2->qos_classify_index NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&key1->qos_behavior_index, &key2->qos_behavior_index NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_acl_comp
   ��������  : acl���ıȽϺ���
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
NBB_INT spm_qos_acl_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_ACL_KEY *key1 = (ATG_DCI_ACL_KEY *)keyarg1;
    ATG_DCI_ACL_KEY *key2 = (ATG_DCI_ACL_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->acl_id), &(key2->acl_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->rule_id), &(key2->rule_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


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
NBB_INT spm_qos_logic_acl_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_ACL_KEY *key1 = (SPM_QOS_LOGIC_ACL_KEY *)keyarg1;
    SPM_QOS_LOGIC_ACL_KEY *key2 = (SPM_QOS_LOGIC_ACL_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->index), &(key2->index) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_byte(&(key1->id), &(key2->id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->svlan, &key2->svlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->cvlan, &key2->cvlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


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
NBB_INT spm_twamp_ipv4_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_TWAMP_IPV4_KEY *key1 = (ATG_DCI_TWAMP_IPV4_KEY *)keyarg1;
    ATG_DCI_TWAMP_IPV4_KEY *key2 = (ATG_DCI_TWAMP_IPV4_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ushort(&(key1->vrf_id), &(key2->vrf_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->sip), &(key2->sip) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&key1->dip, &key2->dip NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->sport, &key2->sport NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->dport, &key2->dport NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


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
NBB_INT spm_twamp_ipv6_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_TWAMP_IPV6_KEY *key1 = (ATG_DCI_TWAMP_IPV6_KEY *)keyarg1;
    ATG_DCI_TWAMP_IPV6_KEY *key2 = (ATG_DCI_TWAMP_IPV6_KEY *)keyarg2;
    NBB_INT ret = 0;
    NBB_INT i = 0;

    ret = compare_ushort(&(key1->vrf_id), &(key2->vrf_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    for(i = 0;i < ATG_DCI_IPV6_LEN;i++)
    {
        ret = compare_ulong(&(key1->sip[i]), &(key2->sip[i]) NBB_CCXT);
        if (0 != ret)
        {
            goto EXIT_LABEL;
        }
        ret = compare_ulong(&(key1->dip[i]), &(key2->dip[i]) NBB_CCXT);
        if (0 != ret)
        {
            goto EXIT_LABEL;
        }
    }
    ret = compare_ushort(&key1->sport, &key2->sport NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->dport, &key2->dport NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}



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
NBB_INT spm_qos_defend_apperc_comp(NBB_VOID *keyarg1, 
    NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_DEFEND_POLICY_APPERC_P *key1 = (ATG_DCI_DEFEND_POLICY_APPERC_P *)keyarg1;
    ATG_DCI_DEFEND_POLICY_APPERC_P *key2 = (ATG_DCI_DEFEND_POLICY_APPERC_P *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->l3_protoc), &(key2->l3_protoc) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


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
NBB_INT spm_qos_defend_usr_def_comp(NBB_VOID *keyarg1,
    NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_DEFEND_POLICY_USER_DF *key1 = (ATG_DCI_DEFEND_POLICY_USER_DF*)keyarg1;
    ATG_DCI_DEFEND_POLICY_USER_DF *key2 = (ATG_DCI_DEFEND_POLICY_USER_DF *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->rule_id), &(key2->rule_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


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
NBB_INT spm_qos_classify_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_QOS_CLASSIFY_KEY *key1 = (ATG_DCI_QOS_CLASSIFY_KEY *)keyarg1;
    ATG_DCI_QOS_CLASSIFY_KEY *key2 = (ATG_DCI_QOS_CLASSIFY_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->qos_classify_index), &(key2->qos_classify_index) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->if_match_id), &(key2->if_match_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}

#ifdef PTN690

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
NBB_INT spm_qos_policy_supervise_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_SUPERVISE_KEY *key1 = (SPM_QOS_POLICY_SUPERVISE_KEY *)keyarg1;
    SPM_QOS_POLICY_SUPERVISE_KEY *key2 = (SPM_QOS_POLICY_SUPERVISE_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->policy_id), &(key2->policy_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->classify_id), &(key2->classify_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->acl_id), &(key2->acl_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    EXIT_LABEL:
    return ret;
}

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
NBB_INT spm_qos_policy_classify_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_CLASSIFY_KEY *key1 = (SPM_QOS_POLICY_CLASSIFY_KEY *)keyarg1;
    SPM_QOS_POLICY_CLASSIFY_KEY *key2 = (SPM_QOS_POLICY_CLASSIFY_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->policy_id), &(key2->policy_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->acl_id), &(key2->acl_id) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}

#endif

/*****************************************************************************
   �� �� ��  : spm_port_wred_comp
   ��������  :
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
NBB_INT spm_port_wred_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_PORT_WRED_KEY *key1 = (SPM_PORT_WRED_KEY *)keyarg1;
    SPM_PORT_WRED_KEY *key2 = (SPM_PORT_WRED_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->index), &(key2->index) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_byte(&(key1->cos), &(key2->cos) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_ilm_comp
   ��������  : ilm���ıȽϺ���
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
NBB_INT spm_qos_ilm_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    SPM_QOS_ILM_KEY *lsp_key1;
    SPM_QOS_ILM_KEY *lsp_key2;

    lsp_key1 = (SPM_QOS_ILM_KEY *)(keyarg1);
    lsp_key2 = (SPM_QOS_ILM_KEY *)(keyarg2);

    if (lsp_key1->inlabel == lsp_key2->inlabel)
    {
        return 0;
    }
    if (lsp_key1->flag == lsp_key2->flag)
    {
        return 0;
    }
    if (lsp_key1->nextip == lsp_key2->nextip)
    {
        return 0;
    }
    if (lsp_key1->nextport == lsp_key2->nextport)
    {
        return 0;
    }

    return 1;
}


/*****************************************************************************
   �� �� ��  : spm_qos_ilm_comp
   ��������  : ilm���ıȽϺ���
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
NBB_INT spm_qos_vrf_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = 0;
    SPM_QOS_VRF_INSTANSE_KEY *key1;
    SPM_QOS_VRF_INSTANSE_KEY *key2;

    key1 = (SPM_QOS_VRF_INSTANSE_KEY *)(keyarg1);
    key2 = (SPM_QOS_VRF_INSTANSE_KEY *)(keyarg2);

    ret = compare_ushort(&key1->vrf_id, &key2->vrf_id NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&key1->peer_ip, &key2->peer_ip NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&key1->label, &key2->label NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_ftn_comp
   ��������  : ftn���ıȽϺ���
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
NBB_INT spm_qos_ftn_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    FTN_KEY *key1 = (FTN_KEY *)keyarg1;
    FTN_KEY *key2 = (FTN_KEY *)keyarg2;
    NBB_INT rv = 0;

    rv = compare_ushort(&key1->vrfid, &key2->vrfid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->fec, &key2->fec NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_byte(&key1->mask, &key2->mask NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return rv;
}

/*****************************************************************************
   �� �� ��  : spm_qos_rx_lsp_comp
   ��������  : rx_lsp���ıȽϺ���
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
NBB_INT spm_qos_rx_lsp_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    SPM_QOS_DS_RXLSP_KEY *lsp_key1;
    SPM_QOS_DS_RXLSP_KEY *lsp_key2;

    lsp_key1 = (SPM_QOS_DS_RXLSP_KEY *)(keyarg1);
    lsp_key2 = (SPM_QOS_DS_RXLSP_KEY *)(keyarg2);

    if (lsp_key1->rxlspkey.ingress == lsp_key2->rxlspkey.ingress
        && lsp_key1->rxlspkey.egress == lsp_key2->rxlspkey.egress
        && lsp_key1->rxlspkey.tunnelid == lsp_key2->rxlspkey.tunnelid
        && lsp_key1->rxlspkey.lspid == lsp_key2->rxlspkey.lspid
        && lsp_key1->flag == lsp_key2->flag)
    {
        return 0;
    }

    return 1;
}


/*****************************************************************************
   �� �� ��  : spm_qos_tx_lsp_comp
   ��������  : tx_lsp���ıȽϺ���
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
NBB_INT spm_qos_tx_lsp_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    CRTXLSP_KEY *lsp_key1;
    CRTXLSP_KEY *lsp_key2;
    NBB_INT ret = 0;

    lsp_key1 = (CRTXLSP_KEY *)(keyarg1);
    lsp_key2 = (CRTXLSP_KEY *)(keyarg2);

    ret = compare_ulong(&(lsp_key1->ingress), &(lsp_key2->ingress) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(lsp_key1->egress), &(lsp_key2->egress) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(lsp_key1->lspid), &(lsp_key2->lspid) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(lsp_key1->tunnelid), &(lsp_key2->tunnelid) NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_tunnel_comp
   ��������  : tunnel���ıȽϺ���
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
NBB_INT spm_qos_tunnel_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    SPM_QOS_TUNNEL_KEY *key1 = (SPM_QOS_TUNNEL_KEY *)keyarg1;
    SPM_QOS_TUNNEL_KEY *key2 = (SPM_QOS_TUNNEL_KEY *)keyarg2;
    NBB_INT rv = 0;

    rv = compare_ushort(&key1->type, &key2->type NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ushort(&key1->flag, &key2->flag NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    if(1 == key1->type)
    {
        rv = spm_qos_tx_lsp_comp(&key1->tx_lsp, &key2->tx_lsp NBB_CCXT);
        if (rv != 0)
        {
            goto EXIT_LABEL;
        }

    }
    else
    {
        rv = spm_qos_ftn_comp(&key1->ftn, &key2->ftn NBB_CCXT);
        if (rv != 0)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL:
    return rv;
}


#if 1

/*****************************************************************************
   �� �� ��  : spm_init_hqos_port_cb
   ��������  : hqos������ĳ�ʼ��
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
NBB_VOID spm_init_hqos_port_cb(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG i = 0;
    NBB_LONG j = 0;
    NBB_BUF_SIZE avll_key_offset;

    /* ��ʼ��HQOS LSP ��Դ */
    SHARED.hqos_lspId = 0;
    OS_MEMSET(&(SHARED.hqos_lsp_pool[0]), 0, sizeof(HQOS_MAX_LSP_ID));

    /* ��ʼ��HQOS PW ��Դ */
    SHARED.hqos_lspId = 0;
    OS_MEMSET(&(SHARED.hqos_pw_pool[0]), 0, sizeof(HQOS_MAX_PW_ID));

    /* ���ж˿�HQOS���ĳ�ʼ�� */
    for(j = 0;j < MAX_SLOT_NUM;j++)
    {
       for (i = 0; i < MAX_PHYSIC_PORT_NUM; i++)
       {
            SHARED.qos_port_cb[j][i].port_key = i;

            avll_key_offset = NBB_OFFSETOF(SPM_HQOS_LSP_TX_CB, lsp_tx_key);
            AVLL_INIT_TREE(SHARED.qos_port_cb[j][i].lsp_tree, spm_qos_tunnel_comp,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_LSP_TX_CB, spm_hqos_lsp_tx_node));

            avll_key_offset = NBB_OFFSETOF(SPM_HQOS_VRF_CB, vrf_key);
            AVLL_INIT_TREE(SHARED.qos_port_cb[j][i].pw_vrf_tree, spm_qos_vrf_comp,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_VRF_CB, spm_hqos_vrf_node));

            avll_key_offset = NBB_OFFSETOF(SPM_HQOS_VC_CB, vc_key);
            AVLL_INIT_TREE(SHARED.qos_port_cb[j][i].pw_vc_tree, spm_vc_key_compare,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_VC_CB, spm_hqos_vc_node));

            avll_key_offset = NBB_OFFSETOF(SPM_HQOS_LOG_GROUP_CB, index);
            AVLL_INIT_TREE(SHARED.qos_port_cb[j][i].group_tree, compare_ulong,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_LOG_GROUP_CB, spm_hqos_group_node));
        } 
    }

    return;
}


/*****************************************************************************
   �� �� ��  : spm_qos_init
   ��������  : qos������ĳ�ʼ��
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
NBB_VOID spm_qos_init(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BUF_SIZE avll_key_offset;
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    OS_MEMSET(&(SHARED.flow_meter_id[0]), 0, MAX_METER_NUM);

    OS_MEMSET(&(SHARED.g_classify_high_acl_id[0]), 0, MAX_CLASSIFY_HIGH_ACL_ID_NUM);
    
    OS_MEMSET(&(SHARED.g_classify_eth_acl_id[0]), 0, MAX_CLASSIFY_ETH_ACL_ID_NUM);

    OS_MEMSET(&(SHARED.g_classify_port_acl_id[0]), 0, MAX_CLASSIFY_PORT_ACL_ID_NUM);

    /* QOS L3 INTF�߼��˿�����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_PORT_WRED_CB, port_wred_key);
    AVLL_INIT_TREE(SHARED.port_wred_tree, spm_qos_port_wred_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_PORT_WRED_CB, spm_port_wred_node));

    /* QOS L3 INTF�߼��˿�����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_LOGIC_INTF_CB, logic_key);
    AVLL_INIT_TREE(SHARED.qos_ds_logic_intf_tree, spm_qos_logic_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_LOGIC_INTF_CB, spm_qos_logic_node));

    /* QOS L2 flow�߼��˿�����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_LOGIC_FLOW_CB, flow_key);
    AVLL_INIT_TREE(SHARED.qos_ds_logic_flow_tree, spm_qos_logic_flow_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_LOGIC_FLOW_CB, spm_qos_logic_node));

    /* QOS vc ����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_VC_CB, vc_key);
    AVLL_INIT_TREE(SHARED.qos_ds_vc_tree, spm_vc_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_VC_CB, spm_qos_vc_node));

    /* QOS vrf ����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_VRF_CB, vrf_key);
    AVLL_INIT_TREE(SHARED.qos_ds_vrf_tree, spm_qos_vrf_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_VRF_CB, spm_qos_vrf_node));

    /* QOS ILM ����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_ILM_CB, ilm_key);
    AVLL_INIT_TREE(SHARED.qos_ds_ilm_tree, spm_qos_ilm_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_ILM_CB, spm_qos_ilm_node));

    /* QOS ftn ����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_FTN_CB, ftn_key);
    AVLL_INIT_TREE(SHARED.qos_ds_ftn_tree, spm_qos_ftn_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_FTN_CB, spm_qos_ftn_node));

    /* QOS rx lsp����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_RX_LSP_CB, rx_lsp_key);
    AVLL_INIT_TREE(SHARED.qos_ds_rx_lsp_tree, spm_qos_rx_lsp_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_RX_LSP_CB, spm_qos_rx_lsp_node));

    /* QOS tx lsp����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_TX_LSP_CB, tx_lsp_key);
    AVLL_INIT_TREE(SHARED.qos_ds_tx_lsp_tree, spm_qos_tx_lsp_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_TX_LSP_CB, spm_qos_tx_lsp_node));

    /* QOS tunnel����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_TUNNEL_CB, tunnel_key);
    AVLL_INIT_TREE(SHARED.qos_ds_tunnel_tree, spm_qos_tunnel_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_TUNNEL_CB, spm_qos_tunnel_node));

#ifdef PTN690

    /* QOS txlsp car����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_TXLSP_CAR_CB, txlsp_key);
    AVLL_INIT_TREE(SHARED.qos_txlsp_car_tree, spm_qos_tx_lsp_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_TXLSP_CAR_CB, spm_qos_txlsp_car_node));

#endif
    /* QOS pw car����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_PW_CAR_CB, log_key);
    AVLL_INIT_TREE(SHARED.qos_pw_car_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_PW_CAR_CB, spm_qos_pw_car_node));

    /* DS ����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_DS_CB, ds_key);
    AVLL_INIT_TREE(SHARED.ds_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_DS_CB, spm_ds_node));

    /* DS������ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_DS_DOMAIN_CB, ds_domain_key);
    AVLL_INIT_TREE(SHARED.ds_domain_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_DS_DOMAIN_CB, spm_ds_domain_node));

    /* DS univp ����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DS_LOGIC_UNIVP_CB, logic_index);
    AVLL_INIT_TREE(SHARED.qos_ds_logic_univp_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DS_LOGIC_UNIVP_CB, spm_qos_logic_univp_node));

    /* ��������������ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, key);
    AVLL_INIT_TREE(SHARED.qos_flow_classify_tree, spm_qos_logic_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, spm_qos_flow_classify_node));

    /* �ϻ��û�QOS���� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_UP_USR_CB, key);
    AVLL_INIT_TREE(SHARED.qos_up_usr_tree, spm_qos_logic_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_UP_USR_CB, spm_qos_up_usr_node));

    /* �ϻ������QOS���� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_FLOW_UP_USR_CB, key);
    AVLL_INIT_TREE(SHARED.qos_flow_up_usr_tree, spm_qos_logic_flow_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_FLOW_UP_USR_CB, spm_qos_flow_up_usr_node));

    /* �߼��˿�filter����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_LOGIC_ACL_CB, key);
    AVLL_INIT_TREE(SHARED.qos_filter_tree, spm_qos_logic_acl_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_LOGIC_ACL_CB, spm_acl_node));

    /* ACL����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_ACL_CB, acl_key);
    AVLL_INIT_TREE(SHARED.qos_acl_tree, spm_qos_acl_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_ACL_CB, spm_acl_node));

    /* ACL���ʼ�� */
    for (i = 0; i < MAX_ACL_CFG_NUM; i++)
    {
        NBB_INIT_ROOT(SHARED.g_acl_id_instance[i].acl_group);

        avll_key_offset = NBB_OFFSETOF(SPM_QOS_LOGIC_ACL_CB, key);
        AVLL_INIT_TREE(SHARED.g_acl_id_instance[i].instance_tree, spm_qos_logic_acl_comp,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_LOGIC_ACL_CB,spm_acl_id_node));

        SHARED.g_acl_id_instance[i].match_type = MATCH_ERROR;
    }

    /* CLASSIFY���ʼ�� */
    for (i = 0; i < MAX_FLOW_CLASSIFY_PRI; i++)
    {
        NBB_INIT_ROOT(SHARED.g_classify_id_instance[i].classify_group);

        avll_key_offset = NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, key);
        AVLL_INIT_TREE(SHARED.g_classify_id_instance[i].instance_tree, compare_ulong,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB,spm_classify_id_node));

        SHARED.g_classify_id_instance[i].match_type = MATCH_ERROR;
    }

    /* CLASSIFY����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_CLASSIFY_CB, classify_key);
    AVLL_INIT_TREE(SHARED.qos_classify_tree, spm_qos_classify_comp,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_CLASSIFY_CB,spm_classify_node));

    /* ACTION����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_ACTION_CB, action_key);
    AVLL_INIT_TREE(SHARED.qos_action_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_ACTION_CB,spm_action_node));

    /* POLICY����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_POLICY_CB, policy_key);
    AVLL_INIT_TREE(SHARED.qos_policy_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_POLICY_CB,spm_policy_node));

#ifdef R8000
    /* ����������ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DEFEND_POLICY_CB, policy_key);
    AVLL_INIT_TREE(SHARED.qos_defend_policy_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DEFEND_POLICY_CB,spm_defend_policy_node));

    /* ��ŷ������������� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DEFEND_CB, key);
    AVLL_INIT_TREE(SHARED.qos_defend_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DEFEND_CB,spm_defend_node));
#endif

    /* �û�������ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_USER_GROUP_CB,user_group_key);
    AVLL_INIT_TREE(SHARED.qos_user_group_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_USER_GROUP_CB,spm_qos_user_group_node));

    avll_key_offset = NBB_OFFSETOF(SPM_HQOS_SLOT_CB, index);
    AVLL_INIT_TREE(SHARED.hqos_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_SLOT_CB,spm_hqos_slot_node));

    avll_key_offset = NBB_OFFSETOF(SPM_QOS_WRED_CB, wred_key);
    AVLL_INIT_TREE(SHARED.qos_wred_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_WRED_CB,spm_wred_node));

    /* twamp_ipv4����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_TWAMP_IPV4_CB, key);
    AVLL_INIT_TREE(SHARED.twamp_ipv4_tree, spm_twamp_ipv4_comp,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_TWAMP_IPV4_CB,spm_twamp_ipv4_node));

    /* twamp_ipv6����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_TWAMP_IPV6_CB, key);
    AVLL_INIT_TREE(SHARED.twamp_ipv6_tree, spm_twamp_ipv6_comp,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_TWAMP_IPV6_CB, spm_twamp_ipv6_node));

    spm_init_hqos_port_cb(NBB_CXT);

    /**********��������ʼ��***********/
    OS_MEMSET(&(SHARED.qos_defend_num[0]),1,SIGNAL_NUM + 1);
    SHARED.qos_defend_num[0] = 0;
    SHARED.qos_defend_num[DEFEND_LDPUDP] = 2;
    SHARED.qos_defend_num[DEFEND_LDP] = 4;
    SHARED.qos_defend_num[DEFEND_BGP] = 2;
    SHARED.qos_defend_num[DEFEND_BGPV6] = 2;
    SHARED.qos_defend_num[DEFEND_DHCP] = 2;
    SHARED.qos_defend_num[DEFEND_DHCPV6] = 2;
    SHARED.qos_defend_num[DEFEND_SNMP] = 2;
    SHARED.qos_defend_num[DEFEND_FTP_S] = 4;
    SHARED.qos_defend_num[DEFEND_FTP_C] = 4;
    SHARED.qos_defend_num[DEFEND_TELNET_S] = 6;
    SHARED.qos_defend_num[DEFEND_TELNET_C] = 6;
    SHARED.qos_defend_num[DEFEND_SSH_S] = 4;
    SHARED.qos_defend_num[DEFEND_SSH_C] = 4;
    SHARED.qos_defend_num[DEFEND_MLD] = 4;
    SHARED.qos_defend_num[DEFEND_TCPSYN] = 2;  
    SHARED.qos_defend_num[DEFEND_TACACS] = 4;
    SHARED.qos_defend_num[DEFEND_RADIUS] = 2;
    
    SHARED.qos_defend_offset[0] = 1;
    for(i = DEFEND_OSPF; i < SIGNAL_NUM + 1;i++)
    {
        SHARED.qos_defend_offset[i] = SHARED.qos_defend_offset[i - 1] + SHARED.qos_defend_num[i - 1];
    }

    //EXIT_LABEL: 
    NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   �� �� ��  : spm_ds_init
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
NBB_LONG spm_ds_init(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_BYTE i = 0;
    NBB_BYTE j = 0;
    NBB_BYTE k = 0;
    NBB_USHORT dsptr = 0;
    NBB_BYTE ucC3Num = 0;

    spm_hardw_getc3chipnum(&ucC3Num);
    printf("c3_num=%d\n",ucC3Num);
    ret = spm_ds_pri2phb_map_init();
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    dsptr = PRI2PHB_ASSIGN_OFFSET;

    /* ��ʼ��PRI��PHB��ָ��dsptr��Χ16-39 */
    for (i = 0; i < MAX_PHB_NUM; i++)
    {
        for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
        {
            for (k = 0; k < MAX_VLAN_PRI_NUM; k++)
            {
                for(unit = 0;unit < ucC3Num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetPriToPhb(unit, dsptr, k, i, j);
#endif 
                    if (ATG_DCI_RC_OK != ret)
                    {
                        goto EXIT_LABEL;
                    }
                }
            }
            for (k = 0; k < MAX_DSCP_PRI_NUM; k++)
            {
                for(unit = 0;unit < ucC3Num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetDscpToPhb(unit, dsptr, k, i, j);                 
#endif 
                    if (ATG_DCI_RC_OK != ret)
                    {
                        goto EXIT_LABEL;
                    }
                }
            }
            for (k = 0; k < MAX_EXP_PRI_NUM; k++)
            {
                for(unit = 0;unit < ucC3Num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetExpToPhb(unit, dsptr, k, i, j);            
#endif
                    if (ATG_DCI_RC_OK != ret)
                    {
                        goto EXIT_LABEL;
                    }
                }
            }
            dsptr++;
        }
    }

    ret = spm_ds_phb2pri_map_init();
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��PHB��PRI��ָ��dsptr��Χ16-79 */
    dsptr = PHB2PRI_ASSIGN_OFFSET;
    for (k = 0; k < MAX_VLAN_PRI_NUM; k++,dsptr++)
    {
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
            {           
                for(unit = 0;unit < ucC3Num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetPhbToPri(unit, dsptr, i, j, k);               
#endif
                    if (ATG_DCI_RC_OK != ret)
                    {
                       goto EXIT_LABEL;
                    } 
                }
            }
        }
    }

    dsptr = PHB2PRI_ASSIGN_OFFSET;
    for (k = 0; k < MAX_DSCP_PRI_NUM; k++,dsptr++)
    {
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
            {           
                for(unit = 0;unit < ucC3Num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetPhbToDscp(unit, dsptr, i, j, k);            
#endif
                    if (ATG_DCI_RC_OK != ret)
                    {
                       goto EXIT_LABEL;
                    }  
                } 
            }
        }
    }

    dsptr = PHB2PRI_ASSIGN_OFFSET;
    for (k = 0; k < MAX_EXP_PRI_NUM; k++,dsptr++)
    {
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
            {           
                for(unit = 0;unit < ucC3Num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret += ApiC3SetPhbToExp(unit, dsptr, i, j, k);             
#endif
                    if (ATG_DCI_RC_OK != ret)
                    {
                       goto EXIT_LABEL;
                    }  
                }  
            }
        }
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_qos_voq_init
   ��������  : arad����Ĭ��ȫ���ӹ�ϵ�ĳ�ʼ������
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
NBB_LONG spm_qos_voq_init()
{
    NBB_LONG slot = 0;
    NBB_LONG portnum = 0;
    NBB_BYTE fapid = 0; /* ����λ */
    NBB_LONG baseQueue = 0;
    NBB_LONG baseVCId = 0;
    NBB_INT rv = 0;
    
    //NBB_USHORT i = 0;
    //NBB_ULONG buf_size = 0;

    NBB_ULONG offset = MAX_SLOT_NUM * NUM_COS;

    /* ��ȡ�������ڵĲ�λ�� */
    spm_hardw_getslot(&fapid);
    fapid--;

    /********************************************************************************/ 
    /* �ڱ��̽������˿ڵ�ȫ�����ڲ�λ(��������λ)��VOQ���ӹ�ϵ,��������ڵ�ITM����; */
    /* ��ʱ���̽�����Ϊҵ��������,ҵ����ܴӱ��̵�����˿��н���,����������(����  */
    /* ����)���п�����Ϊҵ��ĳ�����.                                               */
    /********************************************************************************/
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
        baseQueue = offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        baseVCId = fapid * NUM_COS + offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;

        for (slot = 0; slot < MAX_SLOT_NUM; slot++) /* R8000�е�arad���֧��32����λ */
        {
            if (fapid == slot)  /* ����λ�����ڶ˿ڲ�����VOQ */
            {
#ifdef PTN690 
                if(PTN_690_PORT_OFFSET == portnum)
                {
#if defined (SPU) || defined (SRC)
                    ApiAradCheckPortValid(0, 0, &rv);
                    if (0 == rv)
                    {
                        baseQueue += NUM_COS;
                        continue;
                    }
#endif
                }
                else
                {
#if defined (SPU) || defined (SRC)
                    ApiAradCheckPortValid(0, portnum, &rv);
                    if (0 == rv)
                    {
                        baseQueue += NUM_COS;
                        continue;
                    }
#endif
                }
#else 
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, portnum, &rv);
                if (0 == rv)
                {
                    baseQueue += NUM_COS;
                    continue;
                }
#endif
#endif
            }
#ifdef PTN690
            if(PTN_690_PORT_OFFSET == portnum) /* ARAD cpu�˿ں� */
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupItm(0, baseQueue, NUM_COS, slot + 1, 0, baseVCId);
                if (rv != 0)
                {
                    printf("Error! ApiSetupItm err: %d (baseQueue=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                    goto EXIT_LABEL;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupItm(0, baseQueue, NUM_COS, slot + 1, portnum, baseVCId);
                if (rv != 0)
                {
                    printf("Error! ApiSetupItm err: %d (baseQueue=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                    goto EXIT_LABEL;
                }
#endif
            }
#else
#if defined (SPU) || defined (SRC)
            rv = ApiSetupItm(0, baseQueue, NUM_COS, slot + 1, portnum, baseVCId);
            if (rv != 0)
            {
                printf("Error! ApiSetupItm err: %d (baseQueue=%ld,slot=%ld,port=%ld,"
                       "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                goto EXIT_LABEL;
            }
#endif
#endif  

#if 0       /*���������޸�voq��Ĭ�϶��л���ֵ*/
            for(i = 0; i < 5;i++)
            {   

                buf_size = 1*1024*1024;
 
#ifdef PTN690
                if(PTN_690_PORT_OFFSET == portnum) /* ARAD cpu�˿ں� */
                {
#if defined (SPU) || defined (SRC)
                    rv = ApiAradSetQueueTailDrop(0, baseQueue, i, -1, buf_size);
#endif
                }
                else
                {
#if defined (SPU) || defined (SRC)
                    rv = ApiAradSetQueueTailDrop(0, baseQueue, i, -1, buf_size);
#endif
                }
#else
#if defined (SPU) || defined (SRC)
                rv = ApiAradSetQueueTailDrop(0, baseQueue, i, -1, buf_size);
#endif
#endif
                if (rv != 0)
                {
                    printf("Error! ApiAradSetQueueTailDrop err: %d (baseQueue=%ld,cos=%ld)\n", rv, baseQueue, i);
                    goto EXIT_LABEL;
                }
            }
#endif
            baseQueue += NUM_COS;
        }
    }

    /* ��ʼ���˿�:Ϊ������˿ڽ���Enhanced HR��FQ(ȫSP) */
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
#ifdef PTN690 
        if(PTN_690_PORT_OFFSET == portnum)
        {
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, 0, &rv);
            if (0 == rv)
            {
                continue;
            }
#endif
        }
        else
        {
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, portnum, &rv);
            if (0 == rv)
            {
                continue;
            }
#endif
        }
#else 
#if defined (SPU) || defined (SRC)
        ApiAradCheckPortValid(0, portnum, &rv);
        if (0 == rv)
        {
            continue;
        }
#endif
#endif
#ifdef PTN690
        if(PTN_690_PORT_OFFSET == portnum)
        {
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseSeEtm(0, 0);
            if (rv != 0)
            {
                printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
                goto EXIT_LABEL;
            }
#endif
        }
        else
        {
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseSeEtm(0, portnum);
            if (rv != 0)
            {
                printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
                goto EXIT_LABEL;
            }
#endif
        }
#else
#if defined (SPU) || defined (SRC)
        rv = ApiSetupBaseSeEtm(0, portnum);
        if (rv != 0)
        {
            printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
            goto EXIT_LABEL;
        }
#endif
#endif
    }

    /************************************************************************/ 
    /* ����ȫ������̵����̸��˿ڵ���VC�����̸����˿��������VOQ֮��Ĺ���; */
    /* ��ʱ���̽�����Ϊҵ��ĳ����̣�������(��������)��������Ϊҵ��������.*/
    /************************************************************************/
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
        baseQueue = fapid * NUM_COS + offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        baseVCId = offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        for (slot = 0; slot < MAX_SLOT_NUM; slot++)
        {
#ifdef PTN690 
            if(PTN_690_PORT_OFFSET == portnum)
            {
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, 0, &rv);
                if (0 == rv)
                {
                    baseVCId += NUM_COS;
                    continue;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, portnum, &rv);
                if (0 == rv)
                {
                    baseVCId += NUM_COS;
                    continue;
                }
#endif
            }
#else 
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, portnum, &rv);
            if (0 == rv)
            {
                baseVCId += NUM_COS;
                continue;
            }
#endif
#endif
#ifdef PTN690
            if(PTN_690_PORT_OFFSET == portnum) /* arad CPU�˿ں� */
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupBaseEtm(0, baseQueue, NUM_COS, slot + 1, 0, baseVCId);
                if (rv != 0)    
                {
                    printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                    goto EXIT_LABEL;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupBaseEtm(0, baseQueue, NUM_COS, slot + 1, portnum, baseVCId);
                if (rv != 0)    
                {
                    printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                    goto EXIT_LABEL;
                }
#endif
            }
#else
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseEtm(0, baseQueue, NUM_COS, slot + 1, portnum, baseVCId);
            if (rv != 0)    
            {
                printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                       "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                goto EXIT_LABEL;
            }
#endif
#endif
            baseVCId += NUM_COS;
        }
    }

#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return rv;
}


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
NBB_LONG spm_init_hqos_lsp()
{
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (SRC)
    NBB_INT  rv  = ATG_DCI_RC_OK;
#endif
    NBB_ULONG i;

    for(i = MIN_PHYSIC_PORT; i < MAX_PHYSIC_PORT; i++)
    {
#if defined (SPU) || defined (SRC)
        ApiAradCheckPortValid(0, i, &rv);
        if (0 == rv)
        {
            continue;
        }
#endif
#ifdef PTN690
        if(PTN_690_PORT_OFFSET < i)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosLspCreate(UNIT_0, i, i - MIN_PHYSIC_PORT + 1);
            if (ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**%s,line=%d,port=%ld,lsp id=%ld\n",
                          __FUNCTION__,__LINE__,i,i - MIN_PHYSIC_PORT + 1);
                return ret;
            }
#endif
        }
#else
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspCreate(UNIT_0, i, i - MIN_PHYSIC_PORT + 1);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,line=%d,port=%ld,lsp id=%ld\n",
                      __FUNCTION__,__LINE__,i,i - MIN_PHYSIC_PORT + 1);
            return ret; 
        }
#endif
#endif
    }
    
    return ret;
}

/*SPU�̵���*/
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
NBB_LONG spm_init_rcu_cir_pir()
{
    NBB_LONG ret = 0;
#if defined (SPU) || defined (SRC)
    NBB_LONG cir[NUM_COS] = {390,390,2730,390,1560,3719,4950,1900};
    NBB_LONG pir[NUM_COS] = {10000,390,10000,8000,16000,16000,16000,16000};
#endif
    NBB_LONG i = 0;

#ifdef PTN690
#if defined (SPU) || defined (SRC)
    ret = ApiAradSetOFPBandWidth(0,QOS_RCU_PORT, 4 * 1024);
    if(0 != ret)
    {
        printf("set rcu port bandwidth = 4M EROOR\n");
        goto EXIT_LABEL;
    }
#endif
#else
#if defined (SPU) || defined (SRC)
    ret = ApiAradSetOFPBandWidth(0,QOS_RCU_PORT, 16 * 1024);
    if(0 != ret)
    {
        printf("set rcu port bandwidth = 16M EROOR\n");
        goto EXIT_LABEL;
    }
#endif
#endif
    for(i = 0; i < NUM_COS;i++)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueCirEir(0,QOS_RCU_PORT,i,cir[i],pir[i] - cir[i],1024,1024);
        if(0 != ret)
        {
            printf("set rcu cos = %ld cir eir EROOR\n",i);
            goto EXIT_LABEL;
        }
#endif
    }

    ret = spm_init_hqos_lsp();

#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return ret;
}

/*SRC�̵���*/
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
NBB_LONG spm_init_arad_cpu_cir_pir()
{
    NBB_LONG ret = 0;
    NBB_LONG i = 0;
 
    for(i = 0; i < NUM_COS;i++)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueCirEir(0,0,i,0,6 * 1024,1024,1024);
        if(0 != ret)
        {
            printf("set cpu cos =%ld eir 6M EROOR=%ld\n",i,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    //ret = spm_init_hqos_lsp();
#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return ret;
}

#endif


#if 5

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
NBB_LONG spm_set_ipv4_sampler(SUB_PORT *sub_port, ATG_DCI_PHY_PORT_IPNF_SAMPLER *cfg NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_ULONG cntId = 0;
    NBB_USHORT port_id = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ipv4_sampler");

    if((NULL == cfg) || (NULL == sub_port))
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS ipv4_sampler",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                  "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                  "eg_mode=%s,eg_sample_ratio=%d\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  (cfg->in_enable_flag)?"enable":"disable",
                  (cfg->in_mode)?"fix":"random",cfg->in_sample_ratio,
                  (cfg->eg_enable_flag)?"enable":"disable",
                  (cfg->eg_mode)?"fix":"random",cfg->eg_sample_ratio);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                   "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                   "eg_mode=%s,eg_sample_ratio=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   (cfg->in_enable_flag)?"enable":"disable",
                   (cfg->in_mode)?"fix":"random",cfg->in_sample_ratio,
                   (cfg->eg_enable_flag)?"enable":"disable",
                   (cfg->eg_mode)?"fix":"random",cfg->eg_sample_ratio);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }
    if ((sub_port->port&0x60) != 0)
    {
        port_id = sub_port->port&0x1f;
    }
    else
    {
        port_id = ((sub_port->port&0x60)*(sub_port->port&0x1f));
    }

    cntId = (sub_port->slot - 1) * 50 + port_id + 1024;

#if defined (SPU) || defined (PTN690_CES)
    rv = ApiC3SetFlowSample(sub_port->unit,sub_port->port, 0, cfg->in_enable_flag, cfg->in_sample_ratio,cntId);
#endif  
    if(ATG_DCI_RC_OK != rv)
    {
        printf("%s %d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",
                  __FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    				   "QOS ipv4_sampler",rv,"phyPort","unit","in_enable_flag","in_sample_ratio",
    				   sub_port->port,sub_port->unit,cfg->in_enable_flag,cfg->in_sample_ratio));
        OS_SPRINTF(ucMessage,"%s %s,%d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = rv;
    }
     
#if defined (SPU) || defined (PTN690_CES)
    rv = ApiC3SetFlowSample(sub_port->unit,sub_port->port, 1, cfg->eg_enable_flag, cfg->eg_sample_ratio,cntId);
#endif
    if(ATG_DCI_RC_OK != rv)
    {
        printf("EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",sub_port->unit,rv,sub_port->port);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    				   "QOS ipv4_sampler",rv,"phyPort","unit","eg_enable_flag","eg_sample_ratio",
    				   sub_port->port,sub_port->unit,cfg->in_enable_flag,cfg->in_sample_ratio));
        OS_SPRINTF(ucMessage,"%s %s,%d EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = rv;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_set_lag_sampler(SUB_PORT *sub_port, ATG_DCI_LAG_IPNF_SAMPLER_DATA *cfg NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_ULONG cntId = 0;
    NBB_USHORT port_id = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_set_lag_sampler");

    if(NULL == cfg)
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS lag_sampler",ATG_DCI_RC_UNSUCCESSFUL,
					   "phyPort","cfg==null","","",sub_port->port,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,LINE=%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                  "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                  "eg_mode=%s,eg_sample_ratio=%d\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  (cfg->ingress_flag)?"enable":"disable",
                  (cfg->ingress_mode)?"fix":"random",cfg->ingress_sampling,
                  (cfg->engress_flag)?"enable":"disable",
                  (cfg->engress_mode)?"fix":"random",cfg->engress_sampling);
        OS_SPRINTF(ucMessage,"%s %s,LINE=%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                   "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                   "eg_mode=%s,eg_sample_ratio=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   (cfg->ingress_flag)?"enable":"disable",
                   (cfg->ingress_mode)?"fix":"random",cfg->ingress_sampling,
                   (cfg->engress_flag)?"enable":"disable",
                   (cfg->engress_mode)?"fix":"random",cfg->engress_sampling);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if((sub_port->slot != 0) || (sub_port->port < 0x80))
    {
        printf("%s %s,LINE=%d,spm_set_lag_sampler ERROR\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__);
        OS_SPRINTF(ucMessage,"%s %s,LINE=%d,spm_set_lag_sampler ERROR\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if ((sub_port->port&0x60) != 0)
    {
        port_id = sub_port->port&0x1f;
    }
    else
    {
        port_id = ((sub_port->port&0x60) * (sub_port->port&0x1f));
    }
    
    cntId = (sub_port->slot - 1) * 50 + port_id + 1024;
    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        rv = ApiC3SetFlowSample(unit,sub_port->port, 0, cfg->ingress_flag, cfg->ingress_sampling,cntId);
#endif
        if(ATG_DCI_RC_OK != rv)
        {
            printf("ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",unit,rv,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				   "QOS ipv6_sampler",rv,"phyPort","unit","in_enable_flag","in_sample_ratio",
        				   sub_port->port,unit,cfg->ingress_flag,cfg->ingress_sampling));
            OS_SPRINTF(ucMessage,"%s %s,%d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,unit,rv,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = rv;
        }
       
#if defined (SPU) || defined (PTN690_CES)
        rv = ApiC3SetFlowSample(unit,sub_port->port, 1, cfg->engress_flag, cfg->engress_sampling,cntId);
#endif
        if(ATG_DCI_RC_OK != rv)
        {
            printf("EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",unit,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				   "QOS ipv6_sampler",rv,"phyPort","unit","eg_enable_flag","eg_sample_ratio",
        				   sub_port->port,unit,cfg->engress_flag,cfg->engress_sampling));
            OS_SPRINTF(ucMessage,"%s %s,%d EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,unit,rv,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = rv;
        } 
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_set_port_flood_control(SUB_PORT *sub_port,
    NBB_BYTE bc_swtich,
    NBB_BYTE mc_swtich,
    NBB_BYTE fd_swtich,
    NBB_ULONG bc_bandwith,
    NBB_ULONG mc_bandwith,
    NBB_ULONG fd_bandwith NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
    METER_CFG_T meter = {0};
    
    /*�������ۺ�Ĭ����д��ֵ������mantis������*/
    NBB_ULONG default_cbs = 2048;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_cir_pir");

    if (NULL == sub_port)
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "%s,%s,%s\n"
                  "bc_bandwith=%ld,mc_bandwith=%ld,fd_bandwith=%ld\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  bc_swtich?"�㲥���ƿ�":"�㲥���ƹ�",
                  mc_swtich?"�鲥���ƿ�":"�鲥���ƹ�",
                  fd_swtich?"�鷺���ƿ�":"�鷺���ƹ�",
                  bc_bandwith,mc_bandwith,fd_bandwith);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "%s,%s,%s\n"
                   "bc_bandwith=%ld,mc_bandwith=%ld,fd_bandwith=%ld\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   bc_swtich?"�㲥���ƿ�":"�㲥���ƹ�",
                   mc_swtich?"�鲥���ƿ�":"�鲥���ƹ�",
                   fd_swtich?"�鷺���ƿ�":"�鷺���ƹ�",
                   bc_bandwith,mc_bandwith,fd_bandwith);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if (bc_swtich + mc_swtich + fd_swtich)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFCEnable(sub_port->unit, sub_port->port, 1);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3;
        meter.eMeterType = RFC_2697;
        meter.cir = bc_bandwith;
        meter.pireir = bc_bandwith;
        meter.cbs = default_cbs;
        meter.pbsebs = default_cbs;
        meter.dropRed = 1;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meterId","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, BC_CONTROL, bc_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3 + 1;
        meter.cir = mc_bandwith;
        meter.pireir = mc_bandwith;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meterId","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, MC_CONTROL, mc_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meter id","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3 + 2;
        meter.cir = fd_bandwith;
        meter.pireir = fd_bandwith;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, UC_CONTROL, fd_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meter id","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
    }
    else
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFCEnable(sub_port->unit, sub_port->port, 0);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,
						   "port","unit","ApiC3SetPortFCEnable","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_set_port_cir_pir(SUB_PORT *sub_port, NBB_LONG cos,
    ATG_DCI_PHY_FLOW_SHAPE_QUEUE *shape NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
#if defined (SPU) || defined (SRC)
    NBB_LONG unit = 0;
#endif
    NBB_LONG eir;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_cir_pir");

    if ((NULL == sub_port) || (NULL == shape) || (cos > 7))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",
						0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "cos=%ld,cir=%ld,pir=%ld,cbs=%d,pbs=%d\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                  cos,shape->cir,shape->pir,shape->cbs,shape->pbs);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "cos=%ld,cir=%ld,pir=%ld,cbs=%d,pbs=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                   cos,shape->cir,shape->pir,shape->cbs,shape->pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }
    if (shape->pir < shape->cir)
    {
        printf("**QOS ERROR**%s,%d,pir<cir\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "pir","cir","port","cos",
					   shape->pir,shape->cir,sub_port->port,cos));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(25 * 100 * 1000 == shape->pir)
    {
        shape->pir += 10000;
    }

    eir = shape->pir - shape->cir;

#if defined (SPU) || defined (SRC)
    ret = ApiAradSetPortQueueCirEir(unit, sub_port->port + PTN_690_PORT_OFFSET,
                            cos, shape->cir, eir, shape->cbs, shape->pbs);
#endif
    if (ATG_DCI_RC_OK != ret)
    {
        printf("%s %s,%d ret=%ld port=%d cos=%ld ApiAradSetPortQueueCirEir\n",
                  QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                  sub_port->port + PTN_690_PORT_OFFSET,cos);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS port cir pir",ret,"slot","port","cos","",
					   sub_port->slot,sub_port->port + PTN_690_PORT_OFFSET,cos,0));
        OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d cos=%ld ApiAradSetPortQueueCirEir\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                   sub_port->port + PTN_690_PORT_OFFSET,cos);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_set_port_sp_wfq(SUB_PORT *sub_port, NBB_BYTE cos, 
    ATG_DCI_PHY_PORT_SCHEDULE_QUEUE *port_schedule NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (SRC)
    NBB_LONG unit = 0;
#endif
    NBB_BYTE fapid = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_sp_wfq");

    if ((NULL == sub_port) || (NULL == port_schedule) || (cos > 7))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "cos=%d,mode=%s,weight=%d\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                  cos,(port_schedule->mode)?"WFQ":"SP",port_schedule->weight);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "cos=%d,mode=%s,weight=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                   cos,(port_schedule->mode)?"WFQ":"SP",port_schedule->weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }

    if (0 == port_schedule->mode)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueSP(unit, sub_port->port + PTN_690_PORT_OFFSET, cos, 7 - cos);
#endif
    }
    else
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueWfqWeight(unit, sub_port->port + PTN_690_PORT_OFFSET, cos, port_schedule->weight);
#endif
    }
    if (ATG_DCI_RC_OK != ret)
    {
        printf("%s %s,%d ret=%ld port=%d cos=%d mode=%d set sp or wfq err\n",
                  QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                  sub_port->port + PTN_690_PORT_OFFSET,cos,port_schedule->mode);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS port sp wfq",ret,"slot","port","cos","mode",
					   sub_port->slot,sub_port->port,cos,port_schedule->mode));
        OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d cos=%d mode=%d set sp or wfq err\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                   sub_port->port + PTN_690_PORT_OFFSET,cos,port_schedule->mode);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_add_port_wred(SPM_PORT_WRED_KEY *key, SUB_PORT *sub_port,
    ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (SRC)
    NBB_LONG unit = 0;
#endif
    NBB_ULONG voq = 0;
    NBB_ULONG offset = MAX_SLOT_NUM * NUM_COS;
    SPM_QOS_WRED_CB *green_cb = NULL;
    SPM_QOS_WRED_CB *yellow_cb = NULL;
    SPM_QOS_WRED_CB *red_cb = NULL;

    NBB_TRC_ENTRY("spm_add_port_wred");

    if ((NULL == sub_port) || (NULL == key) || (NULL == cfg))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* ȫ������:������������϶������õ������̳��˿���8�����е�ӵ��������� */
    voq = offset * (sub_port->port) + VOQ_OFFSET + 8 * (sub_port->slot) - 8;
    if (WRED_MODE_QUE_DROP == cfg->mode)
    {
#if defined (SPU) || defined (SRC)
        ApiAradSetQueueTailDrop(unit, voq, key->cos, -1, 10 * 1024 * 1024);
        ret = ApiSetQueueWred(unit,voq,key->cos,0,-1,100,100,0);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
#endif
        goto EXIT_LABEL;
    }
    else if (WRED_MODE_COL_BIND == cfg->mode)
    {
        green_cb = spm_qos_find_wred_cb(cfg->green NBB_CCXT);
        if ((NULL == green_cb) || (NULL == green_cb->basic_cfg_cb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetQueueWred(unit, voq, key->cos, 1, -1,
                                  green_cb->basic_cfg_cb->end_threshold,
                                  green_cb->basic_cfg_cb->start_threshold, 
                                  green_cb->basic_cfg_cb->discard_percent);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,%d,ret =%d ApiAradSetQueueWred\n",
                      __FUNCTION__,__LINE__,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "set port wred",ret,"voq","cos","color","",
						   voq,key->cos,-1,0));
            goto EXIT_LABEL;
        }
#endif
    }
    else if (WRED_MODE_COLOR == cfg->mode)
    {
        green_cb = spm_qos_find_wred_cb(cfg->green NBB_CCXT);
        if ((NULL == green_cb) || (NULL == green_cb->basic_cfg_cb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetQueueWred(unit, voq, key->cos, 1, QOS_COLOR_GREEN, 
                                  green_cb->basic_cfg_cb->end_threshold,
                                  green_cb->basic_cfg_cb->start_threshold, 
                                  green_cb->basic_cfg_cb->discard_percent);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,%d,ret =%d ApiAradSetQueueWred\n",
                      __FUNCTION__,__LINE__,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "set port wred",ret,"voq","cos","color","",
						   voq,key->cos,0,0));
            goto EXIT_LABEL;
        }
#endif
        yellow_cb = spm_qos_find_wred_cb(cfg->yellow NBB_CCXT);
        if ((NULL == yellow_cb) || (NULL == yellow_cb->basic_cfg_cb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetQueueWred(unit, voq, key->cos, 1, QOS_COLOR_YELLOW, 
                                  yellow_cb->basic_cfg_cb->end_threshold,
                                  yellow_cb->basic_cfg_cb->start_threshold, 
                                  yellow_cb->basic_cfg_cb->discard_percent);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,%d,ret =%d ApiAradSetQueueWred\n",
                __FUNCTION__,__LINE__,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "set port wred",ret,"voq","cos","color","",
						   voq,key->cos,1,0));
            goto EXIT_LABEL;
        }
#endif
        red_cb = spm_qos_find_wred_cb(cfg->red NBB_CCXT);
        if ((NULL == red_cb) || (NULL == red_cb->basic_cfg_cb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetQueueWred(unit, voq, key->cos, 1, QOS_COLOR_RED, 
                                  red_cb->basic_cfg_cb->end_threshold,
                                  red_cb->basic_cfg_cb->start_threshold, 
                                  red_cb->basic_cfg_cb->discard_percent);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,%d,ret =%d ApiAradSetQueueWred\n",
                   __FUNCTION__,__LINE__,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "set port wred",ret,"voq","cos","color","",
						   voq,key->cos,2,0));
            goto EXIT_LABEL;
        }
#endif
    }

    /*
    if (WRED_MODE_COLOR == cfg->mode)
    {
        if((NULL != green_cb)&&(NULL != yellow_cb)&&(NULL != red_cb))
        {
            (green_cb->cnt)++;
            (yellow_cb->cnt)++;
            (red_cb->cnt)++;
        }
    }
    else if (WRED_MODE_COL_BIND == cfg->mode)
    {
        if(NULL != green_cb)
        {
            (green_cb->cnt)++;
        }
    }
    */

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_del_port_wred(SPM_PORT_WRED_KEY *key, SUB_PORT *sub_port, 
    ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (SRC)
    NBB_LONG unit = 0;
#endif
    NBB_ULONG voq = 0;
    NBB_ULONG offset = MAX_SLOT_NUM * NUM_COS;
    SPM_QOS_WRED_CB *green_cb = NULL;
    SPM_QOS_WRED_CB *yellow_cb = NULL;
    SPM_QOS_WRED_CB *red_cb = NULL;

    NBB_TRC_ENTRY("spm_del_port_wred");

    if ((NULL == sub_port) || (NULL == cfg))
    {
        printf("spm_del_port_wred sub_port or cfg == NULL");
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    voq = offset * (sub_port->port) + VOQ_OFFSET + 8 * (sub_port->slot) - 8;
    if (WRED_MODE_QUE_DROP == cfg->mode)
    {

#if defined (SPU) || defined (SRC)
        ApiAradSetQueueTailDrop(unit, voq, key->cos, -1, 10 * 1024 * 1024);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }
    else if (WRED_MODE_COL_BIND == cfg->mode)
    {
        green_cb = spm_qos_find_wred_cb(cfg->green NBB_CCXT);
        if (NULL == green_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        if (NULL == green_cb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ApiAradSetQueueWred(unit, voq, key->cos, 0, -1, 
                            green_cb->basic_cfg_cb->end_threshold,
                            green_cb->basic_cfg_cb->start_threshold, 
                            green_cb->basic_cfg_cb->discard_percent);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,%d,ret =%d ApiAradSetQueueWred\n",
                      __FUNCTION__,__LINE__,ret);  
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "set port wred",ATG_DCI_RC_UNSUCCESSFUL,
						   "voq","cos","color","",voq,key->cos,-1,0));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }
    else if (WRED_MODE_COLOR == cfg->mode)
    {
        green_cb = spm_qos_find_wred_cb(cfg->green NBB_CCXT);
        if (NULL == green_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        if (NULL == green_cb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ApiAradSetQueueWred(unit, voq, key->cos, 0, QOS_COLOR_GREEN, 
                            green_cb->basic_cfg_cb->end_threshold,
                            green_cb->basic_cfg_cb->start_threshold, 
                            green_cb->basic_cfg_cb->discard_percent);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,%d,ret =%d ApiAradSetQueueWred\n",
                      __FUNCTION__,__LINE__,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "set port wred",ATG_DCI_RC_UNSUCCESSFUL,
						   "voq","cos","color","",voq,key->cos,0,0));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        yellow_cb = spm_qos_find_wred_cb(cfg->yellow NBB_CCXT);
        if (NULL == yellow_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        if (NULL == yellow_cb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ApiAradSetQueueWred(unit, voq, key->cos, 0, QOS_COLOR_YELLOW, 
                            yellow_cb->basic_cfg_cb->end_threshold,
                            yellow_cb->basic_cfg_cb->start_threshold, 
                            yellow_cb->basic_cfg_cb->discard_percent);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,%d,ret =%d ApiAradSetQueueWred\n",
                      __FUNCTION__,__LINE__,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "set port wred",ATG_DCI_RC_UNSUCCESSFUL,
						   "voq","cos","color","",voq,key->cos,1,0));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        red_cb = spm_qos_find_wred_cb(cfg->red NBB_CCXT);
        if (NULL == red_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        if (NULL == red_cb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (SRC)
        ApiAradSetQueueWred(unit, voq, key->cos, 0, QOS_COLOR_RED, 
                            red_cb->basic_cfg_cb->end_threshold,
                            red_cb->basic_cfg_cb->start_threshold, 
                            red_cb->basic_cfg_cb->discard_percent);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,%d,ret =%d ApiAradSetQueueWred\n",
                      __FUNCTION__,__LINE__,ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "set port wred",ATG_DCI_RC_UNSUCCESSFUL,
						   "voq","cos","color","",voq,key->cos,2,0));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_alloc_port_wred_cb
   ��������  :
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
SPM_PORT_WRED_CB *spm_alloc_port_wred_cb(SPM_PORT_WRED_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_PORT_WRED_CB *pstTbl = NULL;

    NBB_TRC_ENTRY("spm_alloc_port_wred_cb");

    NBB_ASSERT(NULL != pkey);

    if (NULL == pkey)
    {
        printf("**QOS ERROR**%s,%d,key==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS spm_alloc_port_wred_cb",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_PORT_WRED_CB *)NBB_MM_ALLOC(sizeof(SPM_PORT_WRED_CB),
              NBB_NORETRY_ACT, MEM_SPM_PORT_WRED_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_PORT_WRED_CB));
    OS_MEMCPY(&(pstTbl->port_wred_key), pkey, sizeof(SPM_PORT_WRED_KEY));
    AVLL_INIT_NODE(pstTbl->spm_port_wred_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return pstTbl;
}


/*****************************************************************************
   �� �� ��  : spm_free_port_wred_cb
   ��������  :
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
NBB_LONG spm_free_port_wred_cb(SPM_PORT_WRED_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY("spm_free_port_wred_cb");

    NBB_ASSERT(NULL != pstTbl);

    if (NULL == pstTbl)
    {
        printf("**QOS ERROR**%s,%d,pstTbl == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "free port wred",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_PORT_WRED_CB), MEM_SPM_PORT_WRED_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_PORT_WRED_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_disconnect_cfg_wred_cnt(ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY("spm_disconnect_cfg_wred_cnt");

    if (NULL == cfg)
    {
        goto EXIT_LABEL;
    }
    if (WRED_MODE_QUE_DROP == cfg->mode)
    {
        goto EXIT_LABEL;
    }
    else if (WRED_MODE_COL_BIND == cfg->mode)
    {
        ret = spm_disconnect_wred_cnt(cfg->green);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }
    else if (WRED_MODE_COLOR == cfg->mode)
    {
        ret = spm_disconnect_wred_cnt(cfg->green);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_wred_cnt(cfg->yellow);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_wred_cnt(cfg->red);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_set_port_wred
   ��������  :������̽���WRED����
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
NBB_LONG spm_set_port_wred(SPM_PORT_WRED_KEY *key, SUB_PORT *sub_port,
    ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_PORT_WRED_CB *join_node = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != sub_port);
    NBB_ASSERT(NULL != key);

    /* ָ��Ϊ�� */
    if ((NULL == sub_port) || (NULL == key) || (NULL == cfg))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    join_node = AVLL_FIND(SHARED.port_wred_tree, key);

    /***************************************************************************/
    /*                                  ���Ӳ���                               */
    /***************************************************************************/
    if ((NULL == join_node) && (NULL != cfg))
    {
        join_node = spm_alloc_port_wred_cb(key NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_port_wred(key, sub_port, cfg NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_free_port_wred_cb(join_node NBB_CCXT);
            goto EXIT_LABEL;
        }
        OS_MEMCPY(&(join_node->cfg), cfg, sizeof(ATG_DCI_PHY_PORT_CONGEST_QUEUE));
        rv = AVLL_INSERT(SHARED.port_wred_tree, join_node->spm_port_wred_node);
        goto EXIT_LABEL;
    }

    /* ���� */
    else if ((NULL != join_node) && (NULL != cfg) && 
            (NBB_MEMCMP(&(join_node->cfg), cfg, sizeof(ATG_DCI_PHY_PORT_CONGEST_QUEUE))))
    {

        ret = spm_add_port_wred(key, sub_port, cfg NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        /*
        ret = spm_disconnect_cfg_wred_cnt(cfg NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        */
        OS_MEMCPY(&(join_node->cfg), cfg, sizeof(ATG_DCI_PHY_PORT_CONGEST_QUEUE));
        goto EXIT_LABEL;
    }

    /* Ĭ������ */
    /*else if ((NULL != join_node) && (NULL == cfg))
    {
        ret = spm_del_port_wred(key, sub_port, &(join_node->cfg) NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_disconnect_cfg_wred_cnt(&(join_node->cfg) NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(SHARED.port_wred_tree, join_node->spm_port_wred_node);
        ret = spm_free_port_wred_cb(join_node NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        goto EXIT_LABEL;
    }*/

    /* ��ͬ�����ò����ش��� */
    else if ((NULL != join_node) && (NULL != cfg) && 
            (0 == NBB_MEMCMP(&(join_node->cfg), cfg, sizeof(ATG_DCI_PHY_PORT_CONGEST_QUEUE))))
    {
        /*printf("**QOS ERROR**%s,%d,the same cfg\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS port wred the same cfg",ATG_DCI_RC_OK,
					   "sub_port.slot","sub_port.port","key->cos","logic key",
					   sub_port->slot,sub_port->port,key->cos,key->index));
        goto EXIT_LABEL;*/
    }

    /* ��������� */
    else
    {
        printf("**QOS ERROR**%s,%d,port wred config missmatch\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS port wred",ATG_DCI_RC_UNSUCCESSFUL,
						(NULL == join_node)?"no cfg":"find cfg","sub_port.port","key->cos","logic key",
						sub_port->slot,sub_port->port,key->cos,key->index));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_VOID spm_qos_clear_all_port_wred(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_PORT_WRED_CB *cfg_cb = NULL;


    for(cfg_cb = (SPM_PORT_WRED_CB*) AVLL_FIRST(v_spm_shared->port_wred_tree); cfg_cb != NULL;
        cfg_cb = (SPM_PORT_WRED_CB*) AVLL_FIRST(v_spm_shared->port_wred_tree))
    {
        AVLL_DELETE(v_spm_shared->port_wred_tree, cfg_cb->spm_port_wred_node);
        spm_free_port_wred_cb(cfg_cb NBB_CCXT);
    }

    return; 
}

#endif


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
NBB_VOID spm_qos_clear_all_cfg(NBB_CXT_T NBB_CXT)
{
    NBB_ULONG i = 0;
#ifdef PTN690
    NBB_USHORT unit = 0;
#endif  
    //NBB_LONG ret = ATG_DCI_RC_OK;

    /*ʵ��������*/
    spm_qos_clear_all_logic_acl(NBB_CXT);
    spm_qos_clear_all_logic_classify(NBB_CXT);
    spm_qos_clear_all_logic_up_usr(NBB_CXT);
    spm_qos_clear_all_flow_up_usr(NBB_CXT);
    spm_qos_clear_all_ds_logic_flow(NBB_CXT);
    spm_qos_clear_all_ds_logic_intf(NBB_CXT);
    spm_qos_clear_all_ds_vc(NBB_CXT);
    spm_qos_clear_all_univp_cb(NBB_CXT);
    spm_qos_clear_all_ds_vrf(NBB_CXT);
    spm_qos_clear_all_ds_ilm(NBB_CXT);
    spm_qos_clear_all_ds_rxlsp(NBB_CXT);
    spm_qos_clear_all_ds_txlsp(NBB_CXT);
 #ifdef PTN690
    spm_qos_clear_all_txlsp_car(NBB_CXT);
    spm_qos_clear_all_pw_car(NBB_CXT);
 #endif
    spm_hqos_clear_all(NBB_CXT);


    /*ģ������*/
    spm_qos_clear_all_acl(NBB_CXT);
    spm_qos_clear_all_policy(NBB_CXT);
    spm_qos_clear_all_classify(NBB_CXT);
    spm_qos_clear_all_action(NBB_CXT);
#ifdef R8000
    spm_qos_clear_all_defend_policy(NBB_CXT);
    spm_qos_clear_all_defend_switch(NBB_CXT);
#endif
    spm_qos_clear_all_ds(NBB_CXT);
    spm_qos_clear_all_ds_domain(NBB_CXT);
    spm_qos_clear_all_usr_group(NBB_CXT);
    spm_qos_clear_all_hqos_slot(NBB_CXT);
    spm_qos_clear_all_wred(NBB_CXT);
    spm_qos_clear_all_port_wred(NBB_CXT);
    spm_qos_clear_all_twamp_ipv4_cb(NBB_CXT);
    spm_qos_clear_all_twamp_ipv6_cb(NBB_CXT);

    OS_MEMSET(&(v_spm_shared->flow_meter_id[0]), 0, MAX_METER_NUM);
    OS_MEMSET(&(v_spm_shared->g_classify_high_acl_id[0]), 0, MAX_CLASSIFY_HIGH_ACL_ID_NUM);
    OS_MEMSET(&(v_spm_shared->g_classify_eth_acl_id[0]), 0, MAX_CLASSIFY_ETH_ACL_ID_NUM);
    OS_MEMSET(&(v_spm_shared->g_classify_port_acl_id[0]), 0, MAX_CLASSIFY_PORT_ACL_ID_NUM);

    /**********��������ʼ��***********/
    OS_MEMSET(&(v_spm_shared->qos_defend_num[0]),1,SIGNAL_NUM + 1);
    v_spm_shared->qos_defend_num[0] = 0;
    v_spm_shared->qos_defend_num[DEFEND_LDPUDP] = 2;
    v_spm_shared->qos_defend_num[DEFEND_LDP] = 4;
    v_spm_shared->qos_defend_num[DEFEND_BGP] = 2;
    v_spm_shared->qos_defend_num[DEFEND_BGPV6] = 2;
    v_spm_shared->qos_defend_num[DEFEND_DHCP] = 2;
    v_spm_shared->qos_defend_num[DEFEND_DHCPV6] = 2;
    v_spm_shared->qos_defend_num[DEFEND_SNMP] = 2;
    v_spm_shared->qos_defend_num[DEFEND_FTP_S] = 4;
    v_spm_shared->qos_defend_num[DEFEND_FTP_C] = 4;
    v_spm_shared->qos_defend_num[DEFEND_TELNET_S] = 6;
    v_spm_shared->qos_defend_num[DEFEND_TELNET_C] = 6;
    v_spm_shared->qos_defend_num[DEFEND_SSH_S] = 4;
    v_spm_shared->qos_defend_num[DEFEND_SSH_C] = 4;
    v_spm_shared->qos_defend_num[DEFEND_MLD] = 4;
    v_spm_shared->qos_defend_num[DEFEND_TCPSYN] = 2;  
    v_spm_shared->qos_defend_num[DEFEND_TACACS] = 4;
    v_spm_shared->qos_defend_num[DEFEND_RADIUS] = 2;
    
    v_spm_shared->qos_defend_offset[0] = 1;

    for(i = DEFEND_OSPF; i < SIGNAL_NUM + 1;i++)
    {
        v_spm_shared->qos_defend_offset[i] = v_spm_shared->qos_defend_offset[i - 1]     \
                                + v_spm_shared->qos_defend_num[i - 1];
    }
#ifdef SPU
    adatpter_reinit_aradport();
#endif
#if defined (SPU) || defined (SRC)
    fhdrv_fap_qos_clear_all_cfg(0);
#endif
    
    /* ��ɾ����Ӧ�ò���QoS�����ú�,ǿɾ��������QoS��ص�����,��ʱ����һ�д��� */ 
#ifdef PTN690
    for(unit = 0; unit < v_spm_shared->c3_num; unit++)
    { 
#if defined (SPU) || defined (PTN690_CES)        
        fhdrv_qos_clear_all_qos_configuration(unit);
#endif
    }
#endif

    return; 
}

#endif


#if 0
NBB_VOID dbg_get_mem(NBB_LONG *mem)
{
    struct sysinfo s_info;
    int error;

   /* error = sysinfo(&s_info);
    printf("\n\ncode error=%d\n",error);
    printf("Uptime = %ds\nLoad: 1 min%d / 5 min %d / 15 min %d\n"
          "RAM: total %dk / free %dk / shared %dk\n"
          "Memory in buffers = %d\nSwap:total %d / free %d\n"
          "Number of processes = %d\n",
          s_info.uptime, s_info.loads[0],
          s_info.loads[1], s_info.loads[2],
          s_info.totalram/1024, s_info.freeram/1024,
          s_info.totalswap, s_info.freeswap,
          s_info.procs ); 

    *mem = (int)(s_info.freeram/1024);*/
    return;
}

NBB_LONG dbg_hqos_lsp(NBB_USHORT port)
{
   NBB_TRC_ENTRY("dbg_hqos_lsp");
   
   NBB_USHORT i = 0;
   NBB_USHORT j = 0;
   NBB_USHORT k = 0;
   NBB_LONG voq = 7000;
   NBB_LONG ret = 0;
   for(i = 2;i < 1001;i++)
   {
#if defined (SPU) || defined (SRC)
     ret = ApiAradHqosLspCreate(0,port+PTN_690_PORT_OFFSET,i);
#endif
     if(0 != ret)
     {
        printf("**ERROR** ret =%d,lsp id=%d ApiAradHqosLspCreate", ret,i);
        break;     
     }
   }
   EXIT_LABEL: NBB_TRC_EXIT();
   
   return ret;
}

NBB_LONG dbg_hqos_pw(NBB_USHORT port)
{
   NBB_TRC_ENTRY("dbg_hqos_pw");
   
   NBB_USHORT i = 0;
   NBB_LONG ret = 0;
   for(i = 2;i < 1001;i++)
   {
     sleep(1);
#if defined (SPU) || defined (SRC)
     ret=ApiAradHqosPwCreate(0,port+PTN_690_PORT_OFFSET,i,2*i-1,0);
#endif
     if(0 != ret)
     {
        printf( "ret =%d,pw id=%,ApiAradHqosPwCreate",ret,2*i-1);
        break;     
     }
     //sleep(1);
#if defined (SPU) || defined (SRC)
     ret=ApiAradHqosPwCreate(0,port+PTN_690_PORT_OFFSET,i,2*i,0);
#endif
     if(0 != ret)
     {
        printf("ret =%d,pw id=%,ApiAradHqosPwCreate",ret,2*i);
        break;     
     }

   }

   EXIT_LABEL: NBB_TRC_EXIT();
   return ret;
}

NBB_LONG dbg_hqos_voq(NBB_USHORT port)
{

   NBB_TRC_ENTRY("dbg_hqos_voq");
   
   NBB_USHORT i = 0;
   NBB_LONG voq = 16000;
   NBB_LONG ret = 0;
   for(i = 2;i < 1001;i++)
   {
#if defined (SPU) || defined (SRC)
     ret = ApiAradHqosTrafficSet(0,voq,8,7,port+PTN_690_PORT_OFFSET,voq,2*i-1);
#endif
     if(0 != ret)
     {
        printf( "ret =%d,pw id=%,voq=%d,ApiAradHqosTrafficSet",ret,2*i-1,voq);
        break;     
     }
     voq+=8;
#if defined (SPU) || defined (SRC)
     ret = ApiAradHqosTrafficSet(0,voq,8,7,port+PTN_690_PORT_OFFSET,voq,2*i);
#endif
     if(0 != ret)
     {
        printf("ret =%d,pw id=%,voq=%d,ApiAradHqosTrafficSet",ret,2*i,voq);
        break;     
     }
     voq+=8; 
   }

   EXIT_LABEL: NBB_TRC_EXIT();
   return ret;
}


#endif



 





