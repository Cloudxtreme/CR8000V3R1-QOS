/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_pm.c
   版 本 号   : 初稿
   作    者   : zenglu
   生成日期   : 2012年9月18日
   最近修改   :
   功能描述   : QOS处理
   函数列表   :
   修改历史   :
   1.日    期   : 2012年9月18日
    作    者   : zenglu
    修改内容   : 创建文件
******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>



/* twamp_ipv4树 */
AVLL_TREE g_twamp_ipv4_tree;

/* twamp_ipv6树 */
AVLL_TREE g_twamp_ipv6_tree;

#ifdef PTN690

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_hqos_add_vc_pmline(ATG_DCI_VC_KEY *pkey,NBB_ULONG voq)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_VC st_key_vc;

    if(NULL != pkey)
    {
        st_key_vc.iVcId 	 = pkey->vc_id;
        st_key_vc.iPeerip  = pkey->peer_ip;
        st_key_vc.usVcType = pkey->vc_type;
#if defined (SPU) || defined (PTN690_CES)
        ret = almpm_addline_hqos_vc(st_key_vc,voq);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_addline_vc_log(pkey,voq,__FUNCTION__,__LINE__,ret);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_hqos_del_vc_pmline(ATG_DCI_VC_KEY *pkey)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_VC st_key_vc;

    if(NULL != pkey)
    {
        st_key_vc.iVcId 	 = pkey->vc_id;
        st_key_vc.iPeerip  = pkey->peer_ip;
        st_key_vc.usVcType = pkey->vc_type;
#if defined (SPU) || defined (PTN690_CES)
        ret = almpm_delline_hqos_vc(st_key_vc);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_delline_vc_log(pkey,__FUNCTION__,__LINE__,ret);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_hqos_add_vrf_pmline(ATG_DCI_VRF_INSTANCE_KEY *pkey,NBB_ULONG voq)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_L3VPN st_key_l3vpn;

    if(NULL != pkey)
    {
        st_key_l3vpn.usVrfid  = pkey->vrf_id;
		st_key_l3vpn.uiPeerip = pkey->peer_ip;
#if defined (SPU) || defined (PTN690_CES)
		ret = almpm_addline_hqos_l3vpn(st_key_l3vpn, voq);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_addline_vrf_log(pkey,voq,__FUNCTION__,__LINE__,ret);
        }
    }
   
    return;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_hqos_del_vrf_pmline(ATG_DCI_VRF_INSTANCE_KEY *pkey )
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_L3VPN st_key_l3vpn;
    
    if(NULL != pkey)
    {
        st_key_l3vpn.usVrfid  = pkey->vrf_id;
		st_key_l3vpn.uiPeerip = pkey->peer_ip;
#if defined (SPU) || defined (PTN690_CES)
		ret = almpm_delline_hqos_l3vpn(st_key_l3vpn);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_delline_vrf_log(pkey,__FUNCTION__,__LINE__,ret);
		}
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_hqos_add_intf_pmline(NBB_ULONG logic_key,NBB_ULONG voq  )
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_INTF st_key_intf;
    
    if(0 != logic_key)
    {
		st_key_intf.uiPortIndex = logic_key;
#if defined (SPU) || defined (PTN690_CES)
        ret = almpm_addline_hqos_intf(st_key_intf,voq);
#endif
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_almpm_addline_intf_log(logic_key,voq,__FUNCTION__,__LINE__,ret);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_hqos_del_intf_pmline(NBB_ULONG logic_key )
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_LINEKEY_INTF st_key_intf;
    
    if(0 != logic_key)
    {
		st_key_intf.uiPortIndex = logic_key;
#if defined (SPU) || defined (PTN690_CES)
		ret = almpm_delline_hqos_intf(st_key_intf);
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
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_get_poscnt_from_aclkey(NBB_ULONG acl_id, NBB_ULONG rule_id, 
    NBB_ULONG *posid NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = 0;
    SPM_QOS_ACL_CB *pcb = NULL;
    
    for (pcb = (SPM_QOS_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_acl_tree);
         pcb != NULL;
         pcb = (SPM_QOS_ACL_CB*) AVLL_NEXT(v_spm_shared->qos_acl_tree,
                       pcb->spm_acl_node))  
     {
        if((acl_id == pcb->acl_key.acl_id) && (rule_id == pcb->acl_key.rule_id))
        {
            *posid = pcb->posid;
            return ret;
        }
     }

    return -1;    
}




/*****************************************************************************
   函 数 名  : spm_qos_logic_key_compare
   功能描述  : 逻辑端口树的比较函数
   输入参数  :
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
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
   函 数 名  : spm_qos_logic_key_compare
   功能描述  : 逻辑端口树的比较函数
   输入参数  :
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
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
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_twamp_ipv4_cb(SPM_TWAMP_IPV4_CB *pst_tbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_TRC_ENTRY("spm_free_twamp_ipv4_cb");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pst_tbl);

    /*释放内存失败*/
    if (NULL == pst_tbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_TWAMP_IPV4_CB), MEM_SPM_TWAMP_IPV4_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_TWAMP_IPV4_CB);
    pst_tbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_twamp_ipv6_cb(SPM_TWAMP_IPV6_CB *pst_tbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_TRC_ENTRY("spm_free_twamp_ipv6_cb");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pst_tbl);

    /*释放内存失败*/
    if (NULL == pst_tbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_TWAMP_IPV6_CB), MEM_SPM_TWAMP_IPV6_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_TWAMP_IPV6_CB);
    pst_tbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
SPM_TWAMP_IPV4_CB* spm_alloc_twamp_ipv4_cb(ATG_DCI_TWAMP_IPV4_KEY *ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_TWAMP_IPV4_CB *pst_tbl = NULL; 
    NBB_TRC_ENTRY("spm_alloc_twamp_ipv4_cb");
    if(NULL == ulkey)
    {
        goto EXIT_LABEL;
    } 
    pst_tbl = (SPM_TWAMP_IPV4_CB*)NBB_MM_ALLOC(sizeof(SPM_TWAMP_IPV4_CB),
                        NBB_NORETRY_ACT,MEM_SPM_TWAMP_IPV4_CB);
    if(NULL == pst_tbl)
    {
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_TWAMP_IPV4_CB));
    OS_MEMCPY(&(pst_tbl->key), ulkey, sizeof(ATG_DCI_TWAMP_IPV4_KEY));
    
    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_twamp_ipv4_node);
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_tbl);
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
SPM_TWAMP_IPV6_CB *spm_alloc_twamp_ipv6_cb(ATG_DCI_TWAMP_IPV6_KEY *ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_TWAMP_IPV6_CB *pst_tbl = NULL;
    
    NBB_TRC_ENTRY("spm_alloc_twamp_ipv6_cb");

    if(NULL == ulkey)
    {
        goto EXIT_LABEL;
    }
    
    pst_tbl = (SPM_TWAMP_IPV6_CB *)NBB_MM_ALLOC(sizeof(SPM_TWAMP_IPV6_CB),
                        NBB_NORETRY_ACT,
                        MEM_SPM_TWAMP_IPV6_CB);
    if(NULL == pst_tbl)
    {
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_TWAMP_IPV6_CB));
    OS_MEMCPY(&(pst_tbl->key), ulkey, sizeof(ATG_DCI_TWAMP_IPV6_KEY));
    
    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_twamp_ipv6_node);
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_tbl);
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_twamp_ipv4_driver(SPM_TWAMP_IPV4_CB *pst_cb)
{
    NBB_LONG ret = 0;
    NBB_LONG unit = 0;
    TWAMP_ACL_T twampacl = {0};
    
    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL == pst_cb)
    {
        printf("**TWAMP IPV4 ERROR**%s,%d pst_cb == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"TWAMP IPV4 IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }

    twampacl.vrfId     = pst_cb->key.vrf_id;
    twampacl.ethType   = 0x0800;
    twampacl.sip       = pst_cb->key.sip;
    twampacl.dip       = pst_cb->key.dip;
    twampacl.l4SrcPort = pst_cb->key.sport;
    twampacl.l4DstPort = pst_cb->key.dport;

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
    
    /* 调用驱动成功后，将驱动返回值回写到twamp_ipv4树的节点中 */
    pst_cb->posId   = twampacl.posId;
    pst_cb->groupId = twampacl.groupId;
    pst_cb->entryId = twampacl.entryId;
    
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;          
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_del_twamp_ipv4_driver(SPM_TWAMP_IPV4_CB *pst_cb )
{
    NBB_LONG ret = 0;
    NBB_LONG unit = 0;
    TWAMP_ACL_T twampacl = {0};
    
    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL == pst_cb)
    {
        printf("**TWAMP IPV4 ERROR**%s,%d pst_cb == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"TWAMP IPV4 IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }
    twampacl.vrfId     = pst_cb->key.vrf_id;
    twampacl.ethType   = 0x0800;
    twampacl.sip       = pst_cb->key.sip;
    twampacl.dip       = pst_cb->key.dip;
    twampacl.l4SrcPort = pst_cb->key.sport;
    twampacl.l4DstPort = pst_cb->key.dport;
    twampacl.posId     = pst_cb->posId;
    twampacl.groupId   = pst_cb->groupId;
    twampacl.entryId   = pst_cb->entryId;
    
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
    
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;          
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_twamp_ipv4(ATG_DCI_SET_TWAMP_IPV4 *pst_set_ips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_TWAMP_IPV4_KEY ulkey = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_TWAMP_IPV4_CB *pst_cb = NULL;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    /*逻辑处理流程*/
    NBB_TRC_ENTRY(__FUNCTION__);
    if(NULL == pst_set_ips)
    {
        printf("**TWAMP IPV4 ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			"TWAMP IPV4 IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                			"","","","",
                			0,0,0,0));
        goto EXIT_LABEL;
    }

    /* 首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL */
    pst_set_ips->return_code = ATG_DCI_RC_OK;
    OS_MEMCPY(&ulkey, &(pst_set_ips->key), sizeof(ATG_DCI_TWAMP_IPV4_KEY));
    pst_cb = AVLL_FIND(g_twamp_ipv4_tree, &ulkey);

    /*ips消息为删除*/
    if (TRUE == pst_set_ips->delete_struct)
    {
        /* 如果条目不存在，不删除 */
        if (NULL == pst_cb)
        {
            printf("***DEL TWAMP_IPV4 ERROR:CAN'T FIND TWAMP_IPV4 NODE***\n"
                "%s %s,%d,vrf_id=%d,sip=0x%lx,dip=0x%lx,sport=0x%x,dport=0x%x,\n\n",
                QOS_CFG_STRING,__FUNCTION__,__LINE__,
                ulkey.vrf_id,ulkey.sip,ulkey.dip,ulkey.sport,ulkey.dport);
            OS_SPRINTF(uc_message,"***DEL TWAMP_IPV4 ERROR:CAN'T FIND TWAMP_IPV4 NODE***\n"
                "%s %s,%d,vrf_id=%d,sip=0x%lx,dip=0x%lx,sport=0x%x,dport=0x%x,\n\n",
                QOS_CFG_STRING,__FUNCTION__,__LINE__,
                ulkey.vrf_id,ulkey.sip,ulkey.dip,ulkey.sport,ulkey.dport);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
            pst_set_ips->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }

        /* 存在，删除 */
        else
        {
            ret = spm_del_twamp_ipv4_driver(pst_cb NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                pst_set_ips->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
            AVLL_DELETE(g_twamp_ipv4_tree, pst_cb->spm_twamp_ipv4_node);
            
            /* 释放twamp_ipv4表分配的内存空间 */
            spm_free_twamp_ipv4_cb(pst_cb NBB_CCXT);
        }
    }
    
    /*增加或者更新*/
    else
    {
    
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (NULL == pst_cb)
        {
        
            /* 申请一个新条目的内存空间 */
            pst_cb = spm_alloc_twamp_ipv4_cb(&ulkey NBB_CCXT);
            
            /*申请内存失败*/
            if (NULL == pst_cb)
            {
                pst_set_ips->return_code = ATG_DCI_RC_ALLOC_ERROR;
                goto EXIT_LABEL;
            }
            ret = spm_set_twamp_ipv4_driver(pst_cb NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_free_twamp_ipv4_cb(pst_cb NBB_CCXT);
                pst_set_ips->return_code = ATG_DCI_RC_ADD_FAILED;
                goto EXIT_LABEL;
            }
            
            //coverity[no_effect_test]
            rv = AVLL_INSERT(g_twamp_ipv4_tree, pst_cb->spm_twamp_ipv4_node);           
        }
        else
        {
            printf("***ADD TWAMP IPV4:THE SAME TWAMP_IPV4 CFG***%s,%d\n",__FUNCTION__,__LINE__);
        }   
    }
    
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_twamp_ipv6(ATG_DCI_SET_TWAMP_IPV6 *pst_set_ips)
{
    /*逻辑处理流程*/
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pst_set_ips)
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
        pst_set_ips->return_code = ATG_DCI_RC_OK;
        printf("**TWAMP IPV6 ERROR:Currently not supported IPV6**%s,%d\n",__FUNCTION__,__LINE__);
    }
    
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}



/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
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
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
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


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_init_twamp()
{
    NBB_BUF_SIZE avll_key_offset;
    
    /* twamp_ipv4树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_TWAMP_IPV4_CB, key);
    AVLL_INIT_TREE(g_twamp_ipv4_tree, spm_twamp_ipv4_comp,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_TWAMP_IPV4_CB,spm_twamp_ipv4_node));

    /* twamp_ipv6树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_TWAMP_IPV6_CB, key);
    AVLL_INIT_TREE(g_twamp_ipv6_tree, spm_twamp_ipv6_comp,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_TWAMP_IPV6_CB, spm_twamp_ipv6_node));
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_twamp_ipv4_cb_verfify()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_TWAMP_IPV4_CB *pst_cb = NULL;

    NBB_TRC_ENTRY("spm_twamp_ipv4_cb_verfify");

    for (pst_cb = (SPM_TWAMP_IPV4_CB*) AVLL_FIRST(SHARED.twamp_ipv4_tree);pst_cb != NULL;
         pst_cb = (SPM_TWAMP_IPV4_CB*) AVLL_NEXT(SHARED.twamp_ipv4_tree,pst_cb->spm_twamp_ipv4_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify twamp_ipv4_Cb %p", pst_cb));
        NBB_VERIFY_MEMORY(pst_cb, sizeof(SPM_TWAMP_IPV4_CB), MEM_SPM_TWAMP_IPV4_CB);
    }

    NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_twamp_ipv6_cb_verfify()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_TWAMP_IPV6_CB *pst_cb = NULL;

    NBB_TRC_ENTRY("spm_twamp_ipv6_cb_verfify");

    for (pst_cb = (SPM_TWAMP_IPV6_CB*)AVLL_FIRST(SHARED.twamp_ipv6_tree);pst_cb != NULL;
         pst_cb = (SPM_TWAMP_IPV6_CB*)AVLL_NEXT(SHARED.twamp_ipv6_tree,pst_cb->spm_twamp_ipv6_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify twamp_ipv6_Cb %p", pst_cb));
        NBB_VERIFY_MEMORY(pst_cb, sizeof(SPM_TWAMP_IPV6_CB), MEM_SPM_TWAMP_IPV6_CB);
    }

    NBB_TRC_EXIT();

    return;
}