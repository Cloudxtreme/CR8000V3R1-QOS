/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_pm.c
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



/* twamp_ipv4�� */
AVLL_TREE g_twamp_ipv4_tree;

/* twamp_ipv6�� */
AVLL_TREE g_twamp_ipv6_tree;

#ifdef PTN690

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
NBB_VOID spm_hqos_add_vc_pmline(ATG_DCI_VC_KEY *pkey,NBB_ULONG voq)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_VC stKeyVc;

    if(NULL != pkey)
    {
        stKeyVc.iVcId 	 = pkey->vc_id;
        stKeyVc.iPeerip  = pkey->peer_ip;
        stKeyVc.usVcType = pkey->vc_type;
#if defined (SPU) || defined (PTN690_CES)
        ret = almpm_addline_hqos_vc(stKeyVc,voq);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_addline_vc_log(pkey,voq,__FUNCTION__,__LINE__,ret);
        }
    }

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
NBB_VOID spm_hqos_del_vc_pmline(ATG_DCI_VC_KEY *pkey)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_VC stKeyVc;

    if(NULL != pkey)
    {
        stKeyVc.iVcId 	 = pkey->vc_id;
        stKeyVc.iPeerip  = pkey->peer_ip;
        stKeyVc.usVcType = pkey->vc_type;
#if defined (SPU) || defined (PTN690_CES)
        ret = almpm_delline_hqos_vc(stKeyVc);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_delline_vc_log(pkey,__FUNCTION__,__LINE__,ret);
        }
    }

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
NBB_VOID spm_hqos_add_vrf_pmline(SPM_QOS_VRF_INSTANSE_KEY *pkey,NBB_ULONG voq)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_L3VPN stKeyL3vpn;

    if(NULL != pkey)
    {
        stKeyL3vpn.usVrfid  = pkey->vrf_id;
		stKeyL3vpn.uiPeerip = pkey->peer_ip;
#if defined (SPU) || defined (PTN690_CES)
		ret = almpm_addline_hqos_l3vpn(stKeyL3vpn, voq);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_addline_vrf_log(pkey,voq,__FUNCTION__,__LINE__,ret);
        }
    }
   
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
NBB_VOID spm_hqos_del_vrf_pmline(SPM_QOS_VRF_INSTANSE_KEY *pkey )
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_L3VPN stKeyL3vpn;
    
    if(NULL != pkey)
    {
        stKeyL3vpn.usVrfid  = pkey->vrf_id;
		stKeyL3vpn.uiPeerip = pkey->peer_ip;
#if defined (SPU) || defined (PTN690_CES)
		ret = almpm_delline_hqos_l3vpn(stKeyL3vpn);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_delline_vrf_log(pkey,__FUNCTION__,__LINE__,ret);
		}
    }

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
NBB_VOID spm_hqos_add_intf_pmline(NBB_ULONG logic_key,NBB_ULONG voq  )
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_INTF stKeyIntf;
    
    if(0 != logic_key)
    {
		stKeyIntf.uiPortIndex = logic_key;
#if defined (SPU) || defined (PTN690_CES)
        ret = almpm_addline_hqos_intf(stKeyIntf,voq);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_addline_intf_log(logic_key,voq,__FUNCTION__,__LINE__,ret);
        }
    }

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
NBB_VOID spm_hqos_del_intf_pmline(NBB_ULONG logic_key )
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_INTF stKeyIntf;
    
    if(0 != logic_key)
    {
		stKeyIntf.uiPortIndex = logic_key;
#if defined (SPU) || defined (PTN690_CES)
		ret = almpm_delline_hqos_intf(stKeyIntf);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_delline_intf_log(logic_key,__FUNCTION__,__LINE__,ret);
		}
    }

    return;
}

#endif




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
NBB_LONG spm_get_poscnt_from_aclkey(NBB_ULONG ulAcl_Id, NBB_ULONG ulRule_Id, 
    NBB_ULONG *posid NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = 0;
    SPM_QOS_ACL_CB *pcb = NULL;
    
    for (pcb = (SPM_QOS_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_acl_tree);
         pcb != NULL;
         pcb = (SPM_QOS_ACL_CB*) AVLL_NEXT(v_spm_shared->qos_acl_tree,
                       pcb->spm_acl_node))  
     {
        if((ulAcl_Id == pcb->acl_key.acl_id) && (ulRule_Id == pcb->acl_key.rule_id))
        {
            *posid = pcb->posid;
            return ret;
        }
     }

    return -1;    
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
NBB_INT spm_twamp_ipv4_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_TWAMP_IPV4_KEY *key1 = (ATG_DCI_TWAMP_IPV4_KEY *)keyarg1;
    ATG_DCI_TWAMP_IPV4_KEY *key2 = (ATG_DCI_TWAMP_IPV4_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ushort(&(key1->vrf_id), &(key2->vrf_id));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->sip), &(key2->sip));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&key1->dip, &key2->dip);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->sport, &key2->sport);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->dport, &key2->dport);
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
NBB_INT spm_twamp_ipv6_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 )
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_TWAMP_IPV6_KEY *key1 = (ATG_DCI_TWAMP_IPV6_KEY *)keyarg1;
    ATG_DCI_TWAMP_IPV6_KEY *key2 = (ATG_DCI_TWAMP_IPV6_KEY *)keyarg2;
    NBB_INT ret = 0;
    NBB_INT i = 0;

    ret = compare_ushort(&(key1->vrf_id), &(key2->vrf_id));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    for(i = 0;i < ATG_DCI_IPV6_LEN;i++)
    {
        ret = compare_ulong(&(key1->sip[i]), &(key2->sip[i]));
        if (0 != ret)
        {
            goto EXIT_LABEL;
        }
        ret = compare_ulong(&(key1->dip[i]), &(key2->dip[i]));
        if (0 != ret)
        {
            goto EXIT_LABEL;
        }
    }
    ret = compare_ushort(&key1->sport, &key2->sport);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->dport, &key2->dport);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
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
NBB_LONG spm_free_twamp_ipv4_cb(SPM_TWAMP_IPV4_CB *pstTbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_TRC_ENTRY("spm_free_twamp_ipv4_cb");

    /* �������ָ�������Ч */
    NBB_ASSERT(NULL != pstTbl);

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_TWAMP_IPV4_CB), MEM_SPM_TWAMP_IPV4_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_TWAMP_IPV4_CB);
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
NBB_LONG spm_free_twamp_ipv6_cb(SPM_TWAMP_IPV6_CB *pstTbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_TRC_ENTRY("spm_free_twamp_ipv6_cb");

    /* �������ָ�������Ч */
    NBB_ASSERT(NULL != pstTbl);

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pstTbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_TWAMP_IPV6_CB), MEM_SPM_TWAMP_IPV6_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_TWAMP_IPV6_CB);
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
SPM_TWAMP_IPV4_CB* spm_alloc_twamp_ipv4_cb(ATG_DCI_TWAMP_IPV4_KEY *ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_TWAMP_IPV4_CB *pstTbl = NULL; 
    NBB_TRC_ENTRY("spm_alloc_twamp_ipv4_cb");
    if(NULL == ulkey)
    {
        goto EXIT_LABEL;
    } 
    pstTbl = (SPM_TWAMP_IPV4_CB*)NBB_MM_ALLOC(sizeof(SPM_TWAMP_IPV4_CB),
                        NBB_NORETRY_ACT,MEM_SPM_TWAMP_IPV4_CB);
    if(NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(pstTbl, 0, sizeof(SPM_TWAMP_IPV4_CB));
    OS_MEMCPY(&(pstTbl->key), ulkey, sizeof(ATG_DCI_TWAMP_IPV4_KEY));
    
    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_twamp_ipv4_node);
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return(pstTbl);
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
SPM_TWAMP_IPV6_CB *spm_alloc_twamp_ipv6_cb(ATG_DCI_TWAMP_IPV6_KEY *ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_TWAMP_IPV6_CB *pstTbl = NULL;
    
    NBB_TRC_ENTRY("spm_alloc_twamp_ipv6_cb");

    if(NULL == ulkey)
    {
        goto EXIT_LABEL;
    }
    
    pstTbl = (SPM_TWAMP_IPV6_CB *)NBB_MM_ALLOC(sizeof(SPM_TWAMP_IPV6_CB),
                        NBB_NORETRY_ACT,
                        MEM_SPM_TWAMP_IPV6_CB);
    if(NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(pstTbl, 0, sizeof(SPM_TWAMP_IPV6_CB));
    OS_MEMCPY(&(pstTbl->key), ulkey, sizeof(ATG_DCI_TWAMP_IPV6_KEY));
    
    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_twamp_ipv6_node);
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return(pstTbl);
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
NBB_LONG spm_set_twamp_ipv4_driver(SPM_TWAMP_IPV4_CB *pstCb)
{
    NBB_LONG ret = 0;
    NBB_LONG unit = 0;
    TWAMP_ACL_T twampacl = {0};
    
    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL == pstCb)
    {
        printf("**TWAMP IPV4 ERROR**%s,%d pstCb == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"TWAMP IPV4 IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }

    twampacl.vrfId     = pstCb->key.vrf_id;
    twampacl.ethType   = 0x0800;
    twampacl.sip       = pstCb->key.sip;
    twampacl.dip       = pstCb->key.dip;
    twampacl.l4SrcPort = pstCb->key.sport;
    twampacl.l4DstPort = pstCb->key.dport;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetTwampAcl(unit, &twampacl);
        if(ATG_DCI_RC_OK != ret)
        {
            printf("**SET TWAMP IPV4 ERROR**ApiC3SetTwampAcl ERROR=%ld\n",ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					"ApiC3SetTwampAcl",ret,
    					"unit","","","",
    					unit,0,0,0));
            goto EXIT_LABEL;
        }
#endif
    }
    
    /* ���������ɹ��󣬽���������ֵ��д��twamp_ipv4���Ľڵ��� */
    pstCb->posId   = twampacl.posId;
    pstCb->groupId = twampacl.groupId;
    pstCb->entryId = twampacl.entryId;
    
    /*�쳣����*/
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
NBB_LONG spm_del_twamp_ipv4_driver(SPM_TWAMP_IPV4_CB *pstCb )
{
    NBB_LONG ret = 0;
    NBB_LONG unit = 0;
    TWAMP_ACL_T twampacl = {0};
    
    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL == pstCb)
    {
        printf("**TWAMP IPV4 ERROR**%s,%d pstCb == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"TWAMP IPV4 IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }
    twampacl.vrfId     = pstCb->key.vrf_id;
    twampacl.ethType   = 0x0800;
    twampacl.sip       = pstCb->key.sip;
    twampacl.dip       = pstCb->key.dip;
    twampacl.l4SrcPort = pstCb->key.sport;
    twampacl.l4DstPort = pstCb->key.dport;
    twampacl.posId     = pstCb->posId;
    twampacl.groupId   = pstCb->groupId;
    twampacl.entryId   = pstCb->entryId;
    
    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3DelTwampAcl(unit, &twampacl);
        if(ATG_DCI_RC_OK != ret)
        {
            printf("**DEL TWAMP IPV4 ERROR**ApiC3DelTwampAcl ERROR=%ld\n",ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					"ApiC3DelTwampAcl",ret,
    					"unit","","","",
    					unit,0,0,0));
            goto EXIT_LABEL;
        }
#endif
    }
    
    /*�쳣����*/
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
NBB_VOID spm_rcv_dci_set_twamp_ipv4(ATG_DCI_SET_TWAMP_IPV4* pstSetIps)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_TWAMP_IPV4_KEY ulkey = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_TWAMP_IPV4_CB *pstCb = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    /*�߼���������*/
    NBB_TRC_ENTRY(__FUNCTION__);
    if(NULL == pstSetIps)
    {
        printf("**TWAMP IPV4 ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"TWAMP IPV4 IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }

    /* ���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL */
    pstSetIps->return_code = ATG_DCI_RC_OK;
    OS_MEMCPY(&ulkey, &(pstSetIps->key), sizeof(ATG_DCI_TWAMP_IPV4_KEY));
    pstCb = AVLL_FIND(g_twamp_ipv4_tree, &ulkey);

    /*ips��ϢΪɾ��*/
    if (TRUE == pstSetIps->delete_struct)
    {
        /* �����Ŀ�����ڣ���ɾ�� */
        if (NULL == pstCb)
        {
            printf("***DEL TWAMP_IPV4 ERROR:CAN'T FIND TWAMP_IPV4 NODE***\n"
                "%s %s,%d,vrf_id=%d,sip=0x%lx,dip=0x%lx,sport=0x%x,dport=0x%x,\n\n",
                QOS_CFG_STRING,__FUNCTION__,__LINE__,
                ulkey.vrf_id,ulkey.sip,ulkey.dip,ulkey.sport,ulkey.dport);
            OS_SPRINTF(ucMessage,"***DEL TWAMP_IPV4 ERROR:CAN'T FIND TWAMP_IPV4 NODE***\n"
                "%s %s,%d,vrf_id=%d,sip=0x%lx,dip=0x%lx,sport=0x%x,dport=0x%x,\n\n",
                QOS_CFG_STRING,__FUNCTION__,__LINE__,
                ulkey.vrf_id,ulkey.sip,ulkey.dip,ulkey.sport,ulkey.dport);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }

        /* ���ڣ�ɾ�� */
        else
        {
            ret = spm_del_twamp_ipv4_driver(pstCb NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
            AVLL_DELETE(g_twamp_ipv4_tree, pstCb->spm_twamp_ipv4_node);
            
            /* �ͷ�twamp_ipv4�������ڴ�ռ� */
            spm_free_twamp_ipv4_cb(pstCb NBB_CCXT);
        }
    }
    
    /*���ӻ��߸���*/
    else
    {
    
        /* �����Ŀ�����ڣ��������ڴ�ռ䱣������ */
        if (NULL == pstCb)
        {
        
            /* ����һ������Ŀ���ڴ�ռ� */
            pstCb = spm_alloc_twamp_ipv4_cb(&ulkey NBB_CCXT);
            
            /*�����ڴ�ʧ��*/
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_ALLOC_ERROR;
                goto EXIT_LABEL;
            }
            ret = spm_set_twamp_ipv4_driver(pstCb NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_free_twamp_ipv4_cb(pstCb NBB_CCXT);
                pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
                goto EXIT_LABEL;
            }
            
            //coverity[no_effect_test]
            rv = AVLL_INSERT(g_twamp_ipv4_tree, pstCb->spm_twamp_ipv4_node);           
        }
        else
        {
            printf("***ADD TWAMP IPV4:THE SAME TWAMP_IPV4 CFG***%s,%d\n",__FUNCTION__,__LINE__);
        }   
    }
    
    /*�쳣����*/
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
NBB_VOID spm_rcv_dci_set_twamp_ipv6(ATG_DCI_SET_TWAMP_IPV6* pstSetIps)
{
    /*�߼���������*/
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"TWAMP IPV6 IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }
    else
    {
        pstSetIps->return_code = ATG_DCI_RC_OK;
        printf("**TWAMP IPV6 ERROR:Currently not supported IPV6**%s,%d\n",__FUNCTION__,__LINE__);
    }
    
    /*�쳣����*/
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
NBB_VOID spm_qos_clear_all_twamp_ipv4_cb()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_TWAMP_IPV4_CB *cfg_cb = NULL;


    for (cfg_cb = (SPM_TWAMP_IPV4_CB*) AVLL_FIRST(g_twamp_ipv4_tree);cfg_cb != NULL;
         cfg_cb = (SPM_TWAMP_IPV4_CB*) AVLL_FIRST(g_twamp_ipv4_tree))
    {
        AVLL_DELETE(g_twamp_ipv4_tree, cfg_cb->spm_twamp_ipv4_node);
        spm_free_twamp_ipv4_cb(cfg_cb);  
    }

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
NBB_VOID spm_qos_clear_all_twamp_ipv6_cb()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_TWAMP_IPV6_CB *cfg_cb = NULL;


    for (cfg_cb = (SPM_TWAMP_IPV6_CB*) AVLL_FIRST(g_twamp_ipv6_tree);cfg_cb != NULL;
         cfg_cb = (SPM_TWAMP_IPV6_CB*) AVLL_FIRST(g_twamp_ipv6_tree))
    {
        AVLL_DELETE(g_twamp_ipv6_tree, cfg_cb->spm_twamp_ipv6_node);
        spm_free_twamp_ipv6_cb(cfg_cb);  
    }

    return;
}

NBB_VOID spm_init_twamp()
{
    NBB_BUF_SIZE avll_key_offset;
    
    /* twamp_ipv4����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_TWAMP_IPV4_CB, key);
    AVLL_INIT_TREE(g_twamp_ipv4_tree, spm_twamp_ipv4_comp,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_TWAMP_IPV4_CB,spm_twamp_ipv4_node));

    /* twamp_ipv6����ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_TWAMP_IPV6_CB, key);
    AVLL_INIT_TREE(g_twamp_ipv6_tree, spm_twamp_ipv6_comp,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_TWAMP_IPV6_CB, spm_twamp_ipv6_node));
}


NBB_VOID spm_twamp_ipv4_cb_verfify()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_TWAMP_IPV4_CB *pstCb = NULL;

    NBB_TRC_ENTRY("spm_twamp_ipv4_cb_verfify");

    for (pstCb = (SPM_TWAMP_IPV4_CB*) AVLL_FIRST(SHARED.twamp_ipv4_tree);pstCb != NULL;
         pstCb = (SPM_TWAMP_IPV4_CB*) AVLL_NEXT(SHARED.twamp_ipv4_tree,pstCb->spm_twamp_ipv4_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify twamp_ipv4_Cb %p", pstCb));
        NBB_VERIFY_MEMORY(pstCb, sizeof(SPM_TWAMP_IPV4_CB), MEM_SPM_TWAMP_IPV4_CB);
    }

    NBB_TRC_EXIT();

    return;
}

NBB_VOID spm_twamp_ipv6_cb_verfify()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_TWAMP_IPV6_CB *pstCb = NULL;

    NBB_TRC_ENTRY("spm_twamp_ipv6_cb_verfify");

    for (pstCb = (SPM_TWAMP_IPV6_CB*)AVLL_FIRST(SHARED.twamp_ipv6_tree);pstCb != NULL;
         pstCb = (SPM_TWAMP_IPV6_CB*)AVLL_NEXT(SHARED.twamp_ipv6_tree,pstCb->spm_twamp_ipv6_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify twamp_ipv6_Cb %p", pstCb));
        NBB_VERIFY_MEMORY(pstCb, sizeof(SPM_TWAMP_IPV6_CB), MEM_SPM_TWAMP_IPV6_CB);
    }

    NBB_TRC_EXIT();

    return;
}