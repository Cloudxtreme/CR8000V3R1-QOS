/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_proc.c
   版 本 号   : 初稿
   作    者   : zenglu
   生成日期   : 2012年9月18日
   最近修改   :
   功能描述   : QOS处理
   函数列表   :
   修改历史   :
   日    期   : 2012年9月18日
   作    者   : zenglu
   修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/* acl树 */
AVLL_TREE g_qos_acl_tree;

/* 包过滤器树 */
AVLL_TREE g_qos_filter_tree;

/* acl绑定树 */
SPM_QOS_ACL_ID_CB g_acl_id_instance[MAX_ACL_CFG_NUM];

/* acl模块全局打印开关 */
NBB_BYTE g_qos_acl_print = ATG_DCI_RC_OK;

/* acl模块全局LOG开关 */
NBB_BYTE g_qos_acl_log   = ATG_DCI_RC_UNSUCCESSFUL;


/*****************************************************************************
   函 数 名  : spm_find_policy_cb
   功能描述  : 查找policy模板相关配置
   输入参数  : policy模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
/*int fhdrv_acl_update_rule(int unit, uint32 acl_pos_id, ACL_RULE_T *pt_acl_rule)
{

}*/


#if 1

/*****************************************************************************
   函 数 名  : spm_qos_acl_comp
   功能描述  : acl树的比较函数
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
NBB_INT spm_qos_acl_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ATG_DCI_ACL_KEY *key1 = (ATG_DCI_ACL_KEY *)keyarg1;
    ATG_DCI_ACL_KEY *key2 = (ATG_DCI_ACL_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->acl_id), &(key2->acl_id));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&(key1->rule_id), &(key2->rule_id));
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
NBB_INT spm_qos_logic_acl_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_ACL_KEY *key1 = (SPM_QOS_LOGIC_ACL_KEY *)keyarg1;
    SPM_QOS_LOGIC_ACL_KEY *key2 = (SPM_QOS_LOGIC_ACL_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&(key1->index), &(key2->index));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_byte(&(key1->id), &(key2->id));
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->svlan, &key2->svlan);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->cvlan, &key2->cvlan);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_get_ipv6_mask
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_ULONG spm_get_ipv6_mask(NBB_BYTE masklen, NBB_ULONG *maskipv6)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0; 
    NBB_BYTE m = 0;
    NBB_BYTE n = 0;
    NBB_ULONG *temp32;
    NBB_LONG ret = ATG_DCI_RC_OK;
    temp32 = maskipv6;

    NBB_TRC_ENTRY(__FUNCTION__);
    
    if (masklen > 128)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    m = masklen / 32;
    n = masklen % 32;
    for (i = 0; i < m; i++)
    {
        temp32[i] = 0xffffffff;
    }
    for (i = 0; i < n; i++)
    {
        temp32[m] |= (0x1 << (31 - i));
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_alloc_acl_cb
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
SPM_QOS_ACL_CB *spm_alloc_acl_cb(ATG_DCI_ACL_KEY *pkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_ACL_CB *pstcb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pkey)
    {
        goto EXIT_LABEL;
    }

    pstcb = (SPM_QOS_ACL_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_ACL_CB), NBB_NORETRY_ACT, MEM_SPM_ACL_CB);
    if (pstcb == NULL)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstcb, 0, sizeof(SPM_QOS_ACL_CB));
    OS_MEMCPY(&(pstcb->acl_key),pkey,sizeof(ATG_DCI_ACL_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstcb->spm_acl_node);
    NBB_INIT_LQE(pstcb->spm_acl_group_node, pstcb);

    EXIT_LABEL: NBB_TRC_EXIT();
    return pstcb;
}


/*****************************************************************************
   函 数 名  : spm_alloc_acl_cb
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_acl_cb(SPM_QOS_ACL_CB *pstcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pstcb)
    {
        printf(("**FATAL ERROR** SPM_QOS_ACL_CB == NULL"));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_ACL_CB), MEM_SPM_ACL_CB);

    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    if (NULL != pstcb->basic_cfg_cb)
    {
        NBB_MM_FREE(pstcb->basic_cfg_cb, MEM_SPM_ACL_BASIC_CB);
        pstcb->basic_cfg_cb = NULL;
    }
    if (NULL != pstcb->Port_cfg_cb)
    {
        NBB_MM_FREE(pstcb->Port_cfg_cb, MEM_SPM_ACL_PORT_CB);
        pstcb->Port_cfg_cb = NULL;
    }
    if (NULL != pstcb->Eth_cfg_cb)
    {
        NBB_MM_FREE(pstcb->Eth_cfg_cb, MEM_SPM_ACL_ETH_CB);
        pstcb->Eth_cfg_cb = NULL;
    }
    if (NULL != pstcb->ipv4_cfg_cb)
    {
        NBB_MM_FREE(pstcb->ipv4_cfg_cb, MEM_SPM_ACL_IPV4_CB);
        pstcb->ipv4_cfg_cb = NULL;
    }
    if (NULL != pstcb->ipTcp_cfg_cb)
    {
        NBB_MM_FREE(pstcb->ipTcp_cfg_cb, MEM_SPM_ACL_IPTCP_CB);
        pstcb->ipTcp_cfg_cb = NULL;
    }
    if (NULL != pstcb->ipv6_cfg_cb)
    {
        NBB_MM_FREE(pstcb->ipv6_cfg_cb, MEM_SPM_ACL_IPV6_CB);
        pstcb->ipv6_cfg_cb = NULL;
    }
    if (NULL != pstcb->noIp_cfg_cb)
    {
        NBB_MM_FREE(pstcb->noIp_cfg_cb, MEM_SPM_ACL_NOIP_CB);
        pstcb->noIp_cfg_cb = NULL;
    }
    if (NULL != pstcb->ipUdp_cfg_cb)
    {
        NBB_MM_FREE(pstcb->ipUdp_cfg_cb, MEM_SPM_ACL_IPUDP_CB);
        pstcb->ipUdp_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_ACL_CB);
    pstcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_alloc_logic_acl_cb
   功能描述  : 分配逻辑端口下包过滤空间
   输入参数  : acl的key
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
SPM_QOS_LOGIC_ACL_CB *spm_alloc_logic_acl_cb(SPM_QOS_LOGIC_ACL_KEY *pkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_ACL_CB *pstcb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pkey)
    {
        goto EXIT_LABEL;
    }

    pstcb = (SPM_QOS_LOGIC_ACL_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_LOGIC_ACL_CB), 
                                  NBB_NORETRY_ACT, MEM_SPM_QOS_ACL_LOGIC_CB);
    if (NULL == pstcb)
    {
        goto EXIT_LABEL;
    }

    /* 初始逻辑配置块配置条目 */
    OS_MEMSET(pstcb, 0, sizeof(SPM_QOS_LOGIC_ACL_CB));
    OS_MEMCPY(&(pstcb->key),pkey,sizeof(SPM_QOS_LOGIC_ACL_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstcb->spm_acl_node);
    AVLL_INIT_NODE(pstcb->spm_acl_id_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return pstcb;
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 释放逻辑端口下INTF DS节点
   输入参数  : 逻辑端口指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_logic_acl_cb(SPM_QOS_LOGIC_ACL_CB *pstcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pstcb)
    {
        printf("**QOS ERROR**%s,%d,pstcb==null\n",__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_LOGIC_ACL_CB), MEM_SPM_QOS_ACL_LOGIC_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_QOS_ACL_LOGIC_CB);
    pstcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 2

/*****************************************************************************
   函 数 名  : spm_free_acl_rule_tlv
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_acl_rule_tlv(SPM_QOS_ACL_CB *pstcb, NBB_LONG retstatus)
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

    /* 表示更新操作成功,删除初始match_type的子tlv空间 */
    if(ATG_DCI_RC_OK == retstatus)
    {
        match_type = pstcb->match_tpye;
    }

    /* 表示更新操作失败,删除更新过程中申请的内存空间 */
    else
    {
        match_type = pstcb->basic_cfg_cb->match_type;
    }

    switch(match_type)
    {
        case MATCH_IPV4:
        if (NULL != pstcb->ipv4_cfg_cb)
        {
            NBB_MM_FREE(pstcb->ipv4_cfg_cb, MEM_SPM_ACL_IPV4_CB);
            pstcb->ipv4_cfg_cb = NULL;
        }
        break;

        case MATCH_IP_TCP:
        if (NULL != pstcb->ipTcp_cfg_cb)
        {
            NBB_MM_FREE(pstcb->ipTcp_cfg_cb, MEM_SPM_ACL_IPTCP_CB);
            pstcb->ipTcp_cfg_cb = NULL;
        }
        break;

        case MATCH_IP_UDP:
        if (NULL != pstcb->ipUdp_cfg_cb)
        {
            NBB_MM_FREE(pstcb->ipUdp_cfg_cb, MEM_SPM_ACL_IPUDP_CB);
            pstcb->ipUdp_cfg_cb = NULL;
        }
        break;

        case MATCH_IPV6:
        if (NULL != pstcb->ipv6_cfg_cb)
        {
            NBB_MM_FREE(pstcb->ipv6_cfg_cb, MEM_SPM_ACL_IPV6_CB);
            pstcb->ipv6_cfg_cb = NULL;
        }
        break;

        case MATCH_NOIP:
        if (NULL != pstcb->noIp_cfg_cb)
        {
            NBB_MM_FREE(pstcb->noIp_cfg_cb, MEM_SPM_ACL_NOIP_CB);
            pstcb->noIp_cfg_cb = NULL;
        }
        break;

        /* match_type的取值错误 */
        default:
        break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_check_aclid_matchtype
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_check_aclid_matchtype(ATG_DCI_ACL_KEY aclkey, NBB_BYTE match_type)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG  ret = ATG_DCI_RC_OK;
    NBB_ULONG acl_id  = aclkey.acl_id;
    NBB_ULONG rule_id = aclkey.rule_id;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((0 == acl_id) || (0 == rule_id))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 首先转换match_type的值 */
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

        /* match_type的取值错误 */
        default:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:match_type=%d of aclid=%lu/ruleid=%lu err.\n", __FUNCTION__, 
               __LINE__, match_type, acl_id, rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:match_type=%d of aclid=%lu/ruleid=%lu err.\n", 
                   __FUNCTION__, __LINE__, match_type, acl_id, rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 当该aclid是新增的,需要判断该aclid与match-type是否匹配 */
    if(MATCH_ERROR == g_acl_id_instance[acl_id].match_type)
    {
        if(MATCH_PORT == match_type)
        {
            if((acl_id < 1) || (acl_id > 900))
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:aclid=%lu/ruleid=%lu and match-type=%d mismatch either.\n", 
                       __FUNCTION__, __LINE__, acl_id, rule_id, match_type);
                OS_SPRINTF(ucmessage,"***err***%s,%d:aclid=%lu/ruleid=%lu and match-type=%d mismatch "
                           "either.\n", __FUNCTION__, __LINE__, acl_id, rule_id, match_type);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
        else if(MATCH_ETH_HEADER == match_type)
        {
            if((acl_id < 1000) || (acl_id > 1900))
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:aclid=%lu/ruleid=%lu and match-type=%d mismatch either.\n", 
                       __FUNCTION__, __LINE__, acl_id, rule_id, match_type);
                OS_SPRINTF(ucmessage,"***err***%s,%d:aclid=%lu/ruleid=%lu and match-type=%d mismatch "
                           "either.\n", __FUNCTION__, __LINE__, acl_id, rule_id, match_type);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }

        /* 经过转换后,match_type只能是:MATCH_PORT/MATCH_ETH_HEADER/MATCH_IP_TCP */
        else
        {
            if((acl_id < 2000) || (acl_id > 4900))
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:aclid=%lu/ruleid=%lu and match-type=%d mismatch either.\n", 
                       __FUNCTION__, __LINE__, acl_id, rule_id, match_type);
                OS_SPRINTF(ucmessage,"***err***%s,%d:aclid=%lu/ruleid=%lu and match-type=%d mismatch "
                           "either.\n", __FUNCTION__, __LINE__, acl_id, rule_id, match_type);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
        g_acl_id_instance[acl_id].match_type = match_type;
    }
    else
    {
        /* acl中所有rule的match-type必须相同,否则报错 */
        if(match_type != g_acl_id_instance[acl_id].match_type)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:match-type of aclid=%lu/ruleid=%lu can't be changed.\n", 
                   __FUNCTION__, __LINE__, acl_id, rule_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:match-type of aclid=%lu/ruleid=%lu can't "
                       "be changed.\n", __FUNCTION__, __LINE__, acl_id, rule_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_acl_basic_cfg
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_update_acl_basic_cfg(SPM_QOS_ACL_CB *pstcb, NBB_ULONG oper, 
    NBB_BYTE *buf)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_ACL_BASIC_DATA *pstbasicdate = NULL;
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

    /* ACL_BASIC_DATA子tlv的动作只允许为ADD */
    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:basic-oper of aid=%lu/rid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:basic-oper of aid=%lu/rid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                   pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 动作action : 0/1 = Drop丢弃/Permit通过 */
    pstbasicdate = (ATG_DCI_ACL_BASIC_DATA*)buf;
    if((0x00 != pstbasicdate->action) && (0x01 != pstbasicdate->action))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:action=%d parameter err.\n", __FUNCTION__, 
               __LINE__, pstbasicdate->action);
        OS_SPRINTF(ucmessage,"***err***%s,%d:action=%d parameter err.\n", 
                   __FUNCTION__, __LINE__, pstbasicdate->action);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    
    ret = spm_check_aclid_matchtype(pstcb->acl_key, pstbasicdate->match_type);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    /* 新增rule规则 */
    if(NULL == pstcb->basic_cfg_cb)
    {
       /************************************************************ 
        * ipv4/ipv6/noip/iptcp/ipudp规则对于C3驱动属于同一类aclid. *
        * 新增时需要将match_type首先存入,在更新操作时与更新后的值  *
        * 进行比较,用于决定是否需要free或alloc内存空间.            *
        ************************************************************/
        pstcb->match_tpye   = pstbasicdate->match_type;
        pstcb->basic_cfg_cb = (ATG_DCI_ACL_BASIC_DATA *)NBB_MM_ALLOC(   \
                                          sizeof(ATG_DCI_ACL_BASIC_DATA),
                                  NBB_NORETRY_ACT, MEM_SPM_ACL_BASIC_CB);
        if(NULL == pstcb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_ALLOC_ERROR;
            goto EXIT_LABEL;
        }
    }

    /* 更新rule规则 */
    else
    {
        /* 在acl被绑定的情况下,更新rule规则时action动作不允许改变,否则报错 */
        if((pstcb->basic_cfg_cb->action != pstbasicdate->action) && 
           (NULL != AVLL_FIRST(g_acl_id_instance[pstcb->acl_key.acl_id].instance_tree)))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:can't change basic-action of aid=%lu/rid=%lu.\n", 
                   __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:can't change basic-action of aid=%lu/"
                       "rid=%lu.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                       pstcb->acl_key.rule_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    /* 将更新后的match_type和action字段写入basic-cfg子tlv中 */
    OS_MEMCPY(pstcb->basic_cfg_cb, pstbasicdate, sizeof(ATG_DCI_ACL_BASIC_DATA));

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_acl_port_cfg
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_update_acl_port_cfg(SPM_QOS_ACL_CB *pstcb, NBB_ULONG oper, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_ACL_PORT_DATA *ptemp = NULL;
    NBB_USHORT ussvlan = 0;
    SPM_PORT_INFO_CB pstportinfo = {0};
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_USHORT unit = 0;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
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

    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:port-oper of aid=%lu/rid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:port-oper of aid=%lu/rid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                   pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    ptemp = (ATG_DCI_ACL_PORT_DATA *)buf;

   /********************************************************* 
    * 检查acl绑定树(g_acl_id_instance[i].instance_tree)中是 *
    * 否含有节点, 若不含节点表示该acl规则未被绑定到intf上.  *
    *********************************************************/
    cfg_cb = AVLL_FIRST(g_acl_id_instance[pstcb->acl_key.acl_id].instance_tree);

    /* 表示该aclid未被defend-policy引用,也未被intf引用 */
    if((0 == g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
       (NULL == cfg_cb))
    {
        if(NULL == pstcb->Port_cfg_cb)
        {
            pstcb->Port_cfg_cb = (ATG_DCI_ACL_PORT_DATA *)NBB_MM_ALLOC(      \
                                                sizeof(ATG_DCI_ACL_PORT_DATA), 
                                        NBB_NORETRY_ACT, MEM_SPM_ACL_PORT_CB);
            if (NULL == pstcb->Port_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
        OS_MEMCPY(pstcb->Port_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_PORT_DATA));
    }

   /******************************************************************
    * 当aclid已被intf绑定时,不允许defend-policy再引用该aclid;同理,   *
    * 当aclid已被defend-policy引用时,不允许再将该aclid其绑定到intf上.*
    ******************************************************************/
    else if((0 != g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
            (NULL != cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu/rid=%lu can't be used for both intf and "
               "defend-policy.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
               pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu/rid=%lu can't be used for "
                   "both intf and defend-policy.\n", __FUNCTION__, __LINE__, 
                   pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

   /***************************************************** 
    * 表示该aclid未被defend-policy引用,但已被intf引用;  *
    * 或表示该aclid已被defend-policy引用,但未被intf引用.*
    *****************************************************/
    else
    {
        OS_MEMSET(pstacl, 0, sizeof(ACL_T));
        OS_MEMSET(&pstportinfo, 0, sizeof(SPM_PORT_INFO_CB));

       /******************************************************************* 
        * NULL表示在绑定的情况下,新增rule规则.在基于MATCH_PORT时,NULL不可 *
        * 能表示更新的rule规则改变了match-type值,只可能表示新增rule规则.  *
        *******************************************************************/
        if(NULL == pstcb->Port_cfg_cb)
        {
            pstcb->Port_cfg_cb = (ATG_DCI_ACL_PORT_DATA *)NBB_MM_ALLOC(      \
                                                sizeof(ATG_DCI_ACL_PORT_DATA), 
                                        NBB_NORETRY_ACT, MEM_SPM_ACL_PORT_CB);
            if (NULL == pstcb->Port_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            pstacl->mAclId  = pstcb->acl_key.acl_id;
            pstacl->mRuleId = pstcb->acl_key.rule_id;

            /* 0/1 = Drop丢弃/Permit通过 */
            if(0 == pstcb->basic_cfg_cb->action)
            {
                pstacl->eAclAction = ACL_ACTION_DROP;
            }
            else
            {
                pstacl->eAclAction = ACL_ACTION_PASS;
            }
        }

        ret = spm_get_portid_from_logical_port_index(ptemp->index, &pstportinfo);
        if(ATG_DCI_RC_OK != ret)
        {
            printf("***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index err.\n", 
                   __FUNCTION__, __LINE__, ptemp->index);
            OS_SPRINTF(ucmessage,"***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index"
                       " err.\n", __FUNCTION__, __LINE__, ptemp->index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* VE口 */
        if((0 == pstportinfo.slot_id) && (3 == pstportinfo.port_type))
        {
            /* 二层VE口 */
            if(0 == pstportinfo.svlan)
            {
               pstacl->tAclRule.tAclKey.ePortType = ACL_L2VE;
               pstacl->tAclRule.tAclKey.port = pstportinfo.port_id;
               pstacl->tAclRule.tAclMask.ePortType = 0xffff;
               pstacl->tAclRule.tAclMask.port = 0xffff;
            }

            /* 三层VE口 */
            else
            {
               pstacl->tAclRule.tAclKey.ePortType = ACL_L3VE;
               pstacl->tAclRule.tAclKey.port = pstportinfo.port_id;
               pstacl->tAclRule.tAclMask.ePortType = 0xffff;
               pstacl->tAclRule.tAclMask.port = 0xffff;
            }
        }

        /* 普通logic或lag口 */
        else
        {
            /* 二层口 */
            if(0 == pstportinfo.svlan)
            {
                ret = spm_get_vlan_from_logical_port_index(ptemp->index, &ussvlan);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index err.\n", 
                           __FUNCTION__, __LINE__, ptemp->index);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index"
                               " err.\n", __FUNCTION__, __LINE__, ptemp->index);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
                if(0 != ussvlan)
                {
                    pstacl->tAclRule.tAclKey.oVid  = ussvlan;
                    pstacl->tAclRule.tAclMask.oVid = 0x0fff;
                }
            }

            /* 三层口 */
            else
            {
                pstacl->tAclRule.tAclKey.oVid  = pstportinfo.svlan;
                pstacl->tAclRule.tAclMask.oVid = 0x0fff;
            }
            pstacl->tAclRule.tAclKey.ePortType  = ACL_PORT;
            pstacl->tAclRule.tAclKey.port       = pstportinfo.port_id;
            pstacl->tAclRule.tAclMask.ePortType = 0xffff;
            pstacl->tAclRule.tAclMask.port      = 0xffff;     
        }

        /* 绑定状态下,新增acl规则 */
        if(0 == pstcb->posid)
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetAcl(unit, pstacl);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    spm_api_c3_set_acl_error_log(unit, pstacl, 
                                 __FUNCTION__, __LINE__, ret);
                    goto EXIT_LABEL;
                }
                else
                {
                    pstcb->posid = pstacl->posId;
                }
#endif
            }
        }

        /* 绑定状态下,更新acl规则 */
        else
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = fhdrv_acl_update_rule(unit, pstcb->posid, &(pstacl->tAclRule));
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    printf("***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) fhdrv_acl_update_rule err.\n", 
                           __FUNCTION__, __LINE__, pstcb->posid, pstcb->acl_key.acl_id, 
                           pstcb->acl_key.rule_id);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) "
                               "fhdrv_acl_update_rule err.\n", __FUNCTION__, __LINE__, pstcb->posid, 
                               pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
#endif
            }
        }
        OS_MEMCPY(pstcb->Port_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_PORT_DATA));
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_acl_eth_cfg
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_update_acl_eth_cfg(SPM_QOS_ACL_CB *pstcb, NBB_ULONG oper, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_ACL_ETH_HEADER *ptemp = NULL;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_USHORT unit = 0;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
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

    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:eth-oper of aid=%lu/rid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:eth-oper of aid=%lu/rid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                   pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    ptemp = (ATG_DCI_ACL_ETH_HEADER *)buf;

   /********************************************************* 
    * 检查acl绑定树(g_acl_id_instance[i].instance_tree)中是 *
    * 否含有节点, 若不含节点表示该acl规则未被绑定到intf上.  *
    *********************************************************/
    cfg_cb = AVLL_FIRST(g_acl_id_instance[pstcb->acl_key.acl_id].instance_tree);

    /* 表示该aclid未被defend-policy引用,也未被intf引用 */
    if((0 == g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
       (NULL == cfg_cb))
    {
        if(NULL == pstcb->Eth_cfg_cb)
        {
            pstcb->Eth_cfg_cb = (ATG_DCI_ACL_ETH_HEADER *)NBB_MM_ALLOC(      \
                                               sizeof(ATG_DCI_ACL_ETH_HEADER), 
                                         NBB_NORETRY_ACT, MEM_SPM_ACL_ETH_CB);
            if (NULL == pstcb->Eth_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
        OS_MEMCPY(pstcb->Eth_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_ETH_HEADER));
    }

   /******************************************************************
    * 当aclid已被intf绑定时,不允许defend-policy再引用该aclid;同理,   *
    * 当aclid已被defend-policy引用时,不允许再将该aclid其绑定到intf上.*
    ******************************************************************/
    else if((0 != g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
            (NULL != cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu/rid=%lu can't be used for both intf and "
               "defend-policy.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
               pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu/rid=%lu can't be used for "
                   "both intf and defend-policy.\n", __FUNCTION__, __LINE__, 
                   pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

   /***************************************************** 
    * 表示该aclid未被defend-policy引用,但已被intf引用;  *
    * 或表示该aclid已被defend-policy引用,但未被intf引用.*
    *****************************************************/
    else
    {
        OS_MEMSET(pstacl, 0, sizeof(ACL_T));

       /********************************************************************* 
        * NULL表示在绑定的情况下,新增rule规则.在基于MATCH_ETH_HEADER时,NULL *
        * 不可能表示更新的rule规则改变了match-type值,只可能表示新增rule规则.*
        *********************************************************************/
        if(NULL == pstcb->Eth_cfg_cb)
        {
            pstcb->Eth_cfg_cb = (ATG_DCI_ACL_ETH_HEADER *)NBB_MM_ALLOC(      \
                                               sizeof(ATG_DCI_ACL_ETH_HEADER), 
                                         NBB_NORETRY_ACT, MEM_SPM_ACL_ETH_CB);
            if (NULL == pstcb->Eth_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            pstacl->mAclId  = pstcb->acl_key.acl_id;
            pstacl->mRuleId = pstcb->acl_key.rule_id;
            pstacl->flags  |= ACL_COUNTER_ON;

            /* 0/1 = Drop丢弃/Permit通过 */
            if(0 == pstcb->basic_cfg_cb->action)
            {
                pstacl->eAclAction = ACL_ACTION_DROP;
            }
            else
            {
                pstacl->eAclAction = ACL_ACTION_PASS;
            }
        }
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.smac[0]), &(ptemp->src_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.dmac[0]), &(ptemp->dst_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.smac[0]), &(ptemp->src_mac_mask[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.dmac[0]), &(ptemp->dst_mac_mask[0]), ATG_DCI_MAC_LEN);
        pstacl->tAclRule.tAclKey.ovlan_pri  = ptemp->vlan_prio;
        pstacl->tAclRule.tAclMask.ovlan_pri = ptemp->vlan_prio_mask;
        pstacl->tAclRule.tAclKey.ethType  = 0x0800;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;

        /* 绑定或引用状态下,新增acl规则 */
        if(0 == pstcb->posid)
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetAcl(unit, pstacl);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    spm_api_c3_set_acl_error_log(unit, pstacl, 
                                 __FUNCTION__, __LINE__, ret);
                    goto EXIT_LABEL;
                }
                else
                {
                    pstcb->posid = pstacl->posId;
                }
#endif
            }
        }

        /* 绑定或引用状态下,更新acl规则 */
        else
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = fhdrv_acl_update_rule(unit, pstcb->posid, &(pstacl->tAclRule));
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    printf("***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) fhdrv_acl_update_rule err.\n", 
                           __FUNCTION__, __LINE__, pstcb->posid, pstcb->acl_key.acl_id, 
                           pstcb->acl_key.rule_id);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) "
                               "fhdrv_acl_update_rule err.\n", __FUNCTION__, __LINE__, pstcb->posid, 
                               pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
#endif
            }
        }
        OS_MEMCPY(pstcb->Eth_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_ETH_HEADER));
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_acl_ipv4_cfg
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_update_acl_ipv4_cfg(SPM_QOS_ACL_CB *pstcb, NBB_ULONG oper, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_ACL_IPV4_FRAME *ptemp = NULL;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_USHORT unit = 0;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
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

    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:ipv4-oper of aid=%lu/rid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:ipv4-oper of aid=%lu/rid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                   pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    ptemp = (ATG_DCI_ACL_IPV4_FRAME*)buf;

   /********************************************************* 
    * 检查acl绑定树(g_acl_id_instance[i].instance_tree)中是 *
    * 否含有节点, 若不含节点表示该acl规则未被绑定到intf上.  *
    *********************************************************/
    cfg_cb = AVLL_FIRST(g_acl_id_instance[pstcb->acl_key.acl_id].instance_tree);

    /* 表示该aclid未被defend-policy引用,也未被intf引用 */
    if((0 == g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
       (NULL == cfg_cb))
    {
        /* NULL表示在未绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->ipv4_cfg_cb)
        {
            pstcb->ipv4_cfg_cb = (ATG_DCI_ACL_IPV4_FRAME*)NBB_MM_ALLOC(      \
                                               sizeof(ATG_DCI_ACL_IPV4_FRAME), 
                                        NBB_NORETRY_ACT, MEM_SPM_ACL_IPV4_CB);
            if (NULL == pstcb->ipv4_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
        OS_MEMCPY(pstcb->ipv4_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_IPV4_FRAME));
    }

   /******************************************************************
    * 当aclid已被intf绑定时,不允许defend-policy再引用该aclid;同理,   *
    * 当aclid已被defend-policy引用时,不允许再将该aclid其绑定到intf上.*
    ******************************************************************/
    else if((0 != g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
            (NULL != cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu/rid=%lu can't be used for both intf and "
               "defend-policy.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
               pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu/rid=%lu can't be used for "
                   "both intf and defend-policy.\n", __FUNCTION__, __LINE__, 
                   pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

   /***************************************************** 
    * 表示该aclid未被defend-policy引用,但已被intf引用;  *
    * 或表示该aclid已被defend-policy引用,但未被intf引用.*
    *****************************************************/
    else
    {
        OS_MEMSET(pstacl, 0, sizeof(ACL_T));

        /* NULL表示在绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->ipv4_cfg_cb)
        {
            pstcb->ipv4_cfg_cb = (ATG_DCI_ACL_IPV4_FRAME*)NBB_MM_ALLOC(      \
                                               sizeof(ATG_DCI_ACL_IPV4_FRAME), 
                                        NBB_NORETRY_ACT, MEM_SPM_ACL_IPV4_CB);
            if (NULL == pstcb->ipv4_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* 0表示在绑定情况下新增rule规则 */
            if(0 == pstcb->posid)
            {
                pstacl->mAclId  = pstcb->acl_key.acl_id;
                pstacl->mRuleId = pstcb->acl_key.rule_id;
                pstacl->flags  |= ACL_COUNTER_ON;

                /* 0/1 = Drop丢弃/Permit通过 */
                if(0 == pstcb->basic_cfg_cb->action)
                {
                    pstacl->eAclAction = ACL_ACTION_DROP;
                }
                else
                {
                    pstacl->eAclAction = ACL_ACTION_PASS;
                }
            }
        }
        pstacl->tAclRule.tAclKey.dip  = ptemp->dst_ip;
        pstacl->tAclRule.tAclMask.dip = ptemp->dst_ip_mask;
        pstacl->tAclRule.tAclKey.sip  = ptemp->src_ip;
        pstacl->tAclRule.tAclMask.sip = ptemp->src_ip_mask;
        pstacl->tAclRule.tAclKey.tos  = ptemp->dscp;
        pstacl->tAclRule.tAclMask.tos = ptemp->dscp_mask;
        pstacl->tAclRule.tAclKey.l3Protocol  = ptemp->l3_protocol;
        pstacl->tAclRule.tAclMask.l3Protocol = ptemp->l3_protocol_mask;
        pstacl->tAclRule.tAclKey.ethType  = 0x0800;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;
        pstacl->tAclRule.tAclKey.ipHeaderMF  = ptemp->mf;
        pstacl->tAclRule.tAclMask.ipHeaderMF = ptemp->mf_mask;
        pstacl->tAclRule.tAclKey.ipHeaderDF  = ptemp->df;
        pstacl->tAclRule.tAclMask.ipHeaderDF = ptemp->df_mask;
        pstacl->tAclRule.tAclKey.l4SrcPort   = ptemp->src_port;
        pstacl->tAclRule.tAclMask.l4SrcPort  = ptemp->src_port_mask;
        pstacl->tAclRule.tAclKey.l4DstPort   = ptemp->dst_port;
        pstacl->tAclRule.tAclMask.l4DstPort  = ptemp->dst_port_mask;
        if((0x01 == ptemp->l3_protocol)     && 
           (0x00 != ptemp->payload_mask[0]) && 
           (0xff == ptemp->l3_protocol_mask) )
        {
            pstacl->tAclRule.tAclKey.icmpType  = ptemp->payload[0];
            pstacl->tAclRule.tAclMask.icmpType = ptemp->payload_mask[0];
        }
        if((0x01 == ptemp->l3_protocol)     && 
           (0x00 != ptemp->payload_mask[1]) && 
           (0xff == ptemp->l3_protocol_mask))
        {
            pstacl->tAclRule.tAclKey.icmpCode  = ptemp->payload[1];
            pstacl->tAclRule.tAclMask.icmpCode = ptemp->payload_mask[1];
        }
        if(0 == pstcb->posid)
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetAcl(unit, pstacl);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    spm_api_c3_set_acl_error_log(unit, pstacl, 
                                 __FUNCTION__, __LINE__, ret);
                    goto EXIT_LABEL;
                }
                else
                {
                    pstcb->posid = pstacl->posId;
                }
#endif
            }
        }
        else
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = fhdrv_acl_update_rule(unit, pstcb->posid, &(pstacl->tAclRule));
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    printf("***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) fhdrv_acl_update_rule err.\n", 
                           __FUNCTION__, __LINE__, pstcb->posid, pstcb->acl_key.acl_id, 
                           pstcb->acl_key.rule_id);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) "
                               "fhdrv_acl_update_rule err.\n", __FUNCTION__, __LINE__, pstcb->posid, 
                               pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
#endif
            }
        }

        OS_MEMCPY(pstcb->ipv4_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_IPV4_FRAME));
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_acl_iptcp_cfg
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_update_acl_iptcp_cfg(SPM_QOS_ACL_CB *pstcb, NBB_ULONG oper, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_ACL_IPV4_TCP *ptemp = NULL;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_BYTE   tcp_flag = 0;
    NBB_USHORT unit = 0;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
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

    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:iptcp-oper of aid=%lu/rid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:iptcp-oper of aid=%lu/rid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                   pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    ptemp = (ATG_DCI_ACL_IPV4_TCP*)buf;

   /********************************************************* 
    * 检查acl绑定树(g_acl_id_instance[i].instance_tree)中是 *
    * 否含有节点, 若不含节点表示该acl规则未被绑定到intf上.  *
    *********************************************************/
    cfg_cb = AVLL_FIRST(g_acl_id_instance[pstcb->acl_key.acl_id].instance_tree);

    /* 表示该aclid未被defend-policy引用,也未被intf引用 */
    if((0 == g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
       (NULL == cfg_cb))
    {
        /* NULL表示在未绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->ipTcp_cfg_cb)
        {
            pstcb->ipTcp_cfg_cb = (ATG_DCI_ACL_IPV4_TCP*)NBB_MM_ALLOC(  \
                                            sizeof(ATG_DCI_ACL_IPV4_TCP), 
                                  NBB_NORETRY_ACT, MEM_SPM_ACL_IPTCP_CB);
            if (NULL == pstcb->ipTcp_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
        OS_MEMCPY(pstcb->ipTcp_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_IPV4_TCP));
    }

   /******************************************************************
    * 当aclid已被intf绑定时,不允许defend-policy再引用该aclid;同理,   *
    * 当aclid已被defend-policy引用时,不允许再将该aclid其绑定到intf上.*
    ******************************************************************/
    else if((0 != g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
            (NULL != cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu/rid=%lu can't be used for both intf and "
               "defend-policy.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
               pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu/rid=%lu can't be used for "
                   "both intf and defend-policy.\n", __FUNCTION__, __LINE__, 
                   pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

   /***************************************************** 
    * 表示该aclid未被defend-policy引用,但已被intf引用;  *
    * 或表示该aclid已被defend-policy引用,但未被intf引用.*
    *****************************************************/
    else
    {
        OS_MEMSET(pstacl, 0, sizeof(ACL_T));

        /* NULL表示在绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->ipTcp_cfg_cb)
        {
            pstcb->ipTcp_cfg_cb = (ATG_DCI_ACL_IPV4_TCP*)NBB_MM_ALLOC(  \
                                            sizeof(ATG_DCI_ACL_IPV4_TCP), 
                                  NBB_NORETRY_ACT, MEM_SPM_ACL_IPTCP_CB);
            if (NULL == pstcb->ipTcp_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* 0表示在绑定情况下新增rule规则 */
            if(0 == pstcb->posid)
            {
                pstacl->mAclId  = pstcb->acl_key.acl_id;
                pstacl->mRuleId = pstcb->acl_key.rule_id;
                pstacl->flags  |= ACL_COUNTER_ON;

                /* 0/1 = Drop丢弃/Permit通过 */
                if(0 == pstcb->basic_cfg_cb->action)
                {
                    pstacl->eAclAction = ACL_ACTION_DROP;
                }
                else
                {
                    pstacl->eAclAction = ACL_ACTION_PASS;
                }
            }
        }
        pstacl->tAclRule.tAclKey.dip  = ptemp->dst_ip;
        pstacl->tAclRule.tAclMask.dip = ptemp->dst_ip_mask;
        pstacl->tAclRule.tAclKey.sip  = ptemp->src_ip;
        pstacl->tAclRule.tAclMask.sip = ptemp->src_ip_mask;
        pstacl->tAclRule.tAclKey.tos  = (ptemp->dscp);
        pstacl->tAclRule.tAclMask.tos = (ptemp->dscp_mask);
        pstacl->tAclRule.tAclKey.l3Protocol  = 6;
        pstacl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstacl->tAclRule.tAclKey.ethType    = 0x0800;
        pstacl->tAclRule.tAclMask.ethType   = 0xffff;
        pstacl->tAclRule.tAclKey.l4SrcPort  = ptemp->src_port;
        pstacl->tAclRule.tAclMask.l4SrcPort = ptemp->src_port_mask;
        pstacl->tAclRule.tAclKey.l4DstPort  = ptemp->dst_port;
        pstacl->tAclRule.tAclMask.l4DstPort = ptemp->dst_port_mask; 
        OS_MEMCPY(&tcp_flag, &(ptemp->tcp_flag), 1);
        pstacl->tAclRule.tAclKey.tcp_flag  = tcp_flag;
        pstacl->tAclRule.tAclMask.tcp_flag = ptemp->tcp_flag_mask;

        if(0 == pstcb->posid)
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetAcl(unit, pstacl);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    spm_api_c3_set_acl_error_log(unit, pstacl, 
                                 __FUNCTION__, __LINE__, ret);
                    goto EXIT_LABEL;
                }
                else
                {
                    pstcb->posid = pstacl->posId;
                }
#endif
            }
        }
        else
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = fhdrv_acl_update_rule(unit, pstcb->posid, &(pstacl->tAclRule));
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    printf("***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) fhdrv_acl_update_rule err.\n", 
                           __FUNCTION__, __LINE__, pstcb->posid, pstcb->acl_key.acl_id, 
                           pstcb->acl_key.rule_id);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) "
                               "fhdrv_acl_update_rule err.\n", __FUNCTION__, __LINE__, pstcb->posid, 
                               pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
#endif
            }
        }

        OS_MEMCPY(pstcb->ipTcp_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_IPV4_TCP));
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_acl_ipudp_cfg
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_update_acl_ipudp_cfg(SPM_QOS_ACL_CB *pstcb, NBB_ULONG oper, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_ACL_IPV4_UDP *ptemp = NULL;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_USHORT unit = 0;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
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

    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:ipudp-oper of aid=%lu/rid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:ipudp-oper of aid=%lu/rid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                   pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    ptemp = (ATG_DCI_ACL_IPV4_UDP*)buf;

   /********************************************************* 
    * 检查acl绑定树(g_acl_id_instance[i].instance_tree)中是 *
    * 否含有节点, 若不含节点表示该acl规则未被绑定到intf上.  *
    *********************************************************/
    cfg_cb = AVLL_FIRST(g_acl_id_instance[pstcb->acl_key.acl_id].instance_tree);

    /* 表示该aclid未被defend-policy引用,也未被intf引用 */
    if((0 == g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
       (NULL == cfg_cb))
    {
        /* NULL表示在未绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->ipUdp_cfg_cb)
        {
            pstcb->ipUdp_cfg_cb = (ATG_DCI_ACL_IPV4_UDP*)NBB_MM_ALLOC(  \
                                            sizeof(ATG_DCI_ACL_IPV4_UDP), 
                                  NBB_NORETRY_ACT, MEM_SPM_ACL_IPUDP_CB);
            if (NULL == pstcb->ipUdp_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }

        OS_MEMCPY(pstcb->ipUdp_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_IPV4_UDP));
    }

   /******************************************************************
    * 当aclid已被intf绑定时,不允许defend-policy再引用该aclid;同理,   *
    * 当aclid已被defend-policy引用时,不允许再将该aclid其绑定到intf上.*
    ******************************************************************/
    else if((0 != g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
            (NULL != cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu/rid=%lu can't be used for both intf and "
               "defend-policy.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
               pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu/rid=%lu can't be used for "
                   "both intf and defend-policy.\n", __FUNCTION__, __LINE__, 
                   pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

   /***************************************************** 
    * 表示该aclid未被defend-policy引用,但已被intf引用;  *
    * 或表示该aclid已被defend-policy引用,但未被intf引用.*
    *****************************************************/
    else
    {
        OS_MEMSET(pstacl, 0, sizeof(ACL_T));

        /* NULL表示在绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->ipUdp_cfg_cb)
        {
            pstcb->ipUdp_cfg_cb = (ATG_DCI_ACL_IPV4_UDP*)NBB_MM_ALLOC(  \
                                            sizeof(ATG_DCI_ACL_IPV4_UDP), 
                                  NBB_NORETRY_ACT, MEM_SPM_ACL_IPUDP_CB);
            if (NULL == pstcb->ipUdp_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* 0表示在绑定情况下新增rule规则 */
            if(0 == pstcb->posid)
            {
                pstacl->mAclId  = pstcb->acl_key.acl_id;
                pstacl->mRuleId = pstcb->acl_key.rule_id;
                pstacl->flags  |= ACL_COUNTER_ON;

                /* 0/1 = Drop丢弃/Permit通过 */
                if(0 == pstcb->basic_cfg_cb->action)
                {
                    pstacl->eAclAction = ACL_ACTION_DROP;
                }
                else
                {
                    pstacl->eAclAction = ACL_ACTION_PASS;
                }
            }
        }
        pstacl->tAclRule.tAclKey.ethType  = 0x0800;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;
        pstacl->tAclRule.tAclKey.dip  = ptemp->dst_ip;
        pstacl->tAclRule.tAclMask.dip = ptemp->dst_ip_mask;
        pstacl->tAclRule.tAclKey.sip  = ptemp->src_ip;
        pstacl->tAclRule.tAclMask.sip = ptemp->src_ip_mask;
        pstacl->tAclRule.tAclKey.tos  = (ptemp->dscp);
        pstacl->tAclRule.tAclMask.tos = (ptemp->dscp_mask);
        pstacl->tAclRule.tAclKey.l3Protocol  = 0x11;
        pstacl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstacl->tAclRule.tAclKey.l4SrcPort  = ptemp->src_port;
        pstacl->tAclRule.tAclMask.l4SrcPort = ptemp->src_port_mask;
        pstacl->tAclRule.tAclKey.l4DstPort  = ptemp->dst_port;
        pstacl->tAclRule.tAclMask.l4DstPort = ptemp->dst_port_mask;
        
        if(0 == pstcb->posid)
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetAcl(unit, pstacl);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    spm_api_c3_set_acl_error_log(unit, pstacl, 
                                 __FUNCTION__, __LINE__, ret);
                    goto EXIT_LABEL;
                }
                else
                {
                    pstcb->posid = pstacl->posId;
                }
#endif
            }
        }
        else
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = fhdrv_acl_update_rule(unit, pstcb->posid, &(pstacl->tAclRule));
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    printf("***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) fhdrv_acl_update_rule err.\n", 
                           __FUNCTION__, __LINE__, pstcb->posid, pstcb->acl_key.acl_id, 
                           pstcb->acl_key.rule_id);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) "
                               "fhdrv_acl_update_rule err.\n", __FUNCTION__, __LINE__, pstcb->posid, 
                               pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
#endif
            }
        }

        OS_MEMCPY(pstcb->ipUdp_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_IPV4_UDP));
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_acl_ipv6_cfg
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_update_acl_ipv6_cfg(SPM_QOS_ACL_CB *pstcb, NBB_ULONG oper, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_ACL_IPV6_FRAME *ptemp = NULL;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_USHORT unit = 0;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
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

    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:ipv6-oper of aid=%lu/rid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:ipv6-oper of aid=%lu/rid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                   pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    ptemp = (ATG_DCI_ACL_IPV6_FRAME*)buf;

   /********************************************************* 
    * 检查acl绑定树(g_acl_id_instance[i].instance_tree)中是 *
    * 否含有节点, 若不含节点表示该acl规则未被绑定到intf上.  *
    *********************************************************/
    cfg_cb = AVLL_FIRST(g_acl_id_instance[pstcb->acl_key.acl_id].instance_tree);

    /* 表示该aclid未被defend-policy引用,也未被intf引用 */
    if((0 == g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
       (NULL == cfg_cb))
    {
        /* NULL表示在未绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->ipv6_cfg_cb)
        {
            pstcb->ipv6_cfg_cb = (ATG_DCI_ACL_IPV6_FRAME*)NBB_MM_ALLOC(    \
                                             sizeof(ATG_DCI_ACL_IPV6_FRAME), 
                                      NBB_NORETRY_ACT, MEM_SPM_ACL_IPV6_CB);
            if (NULL == pstcb->ipv6_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
        OS_MEMCPY(pstcb->ipv6_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_IPV6_FRAME));
    }

   /******************************************************************
    * 当aclid已被intf绑定时,不允许defend-policy再引用该aclid;同理,   *
    * 当aclid已被defend-policy引用时,不允许再将该aclid其绑定到intf上.*
    ******************************************************************/
    else if((0 != g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
            (NULL != cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu/rid=%lu can't be used for both intf and "
               "defend-policy.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
               pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu/rid=%lu can't be used for "
                   "both intf and defend-policy.\n", __FUNCTION__, __LINE__, 
                   pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

   /***************************************************** 
    * 表示该aclid未被defend-policy引用,但已被intf引用;  *
    * 或表示该aclid已被defend-policy引用,但未被intf引用.*
    *****************************************************/
    else
    {
        OS_MEMSET(pstacl, 0, sizeof(ACL_T));

        /* NULL表示在绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->ipv6_cfg_cb)
        {
            pstcb->ipv6_cfg_cb = (ATG_DCI_ACL_IPV6_FRAME*)NBB_MM_ALLOC(    \
                                             sizeof(ATG_DCI_ACL_IPV6_FRAME), 
                                      NBB_NORETRY_ACT, MEM_SPM_ACL_IPV6_CB);
            if (NULL == pstcb->ipv6_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* 0表示在绑定情况下新增rule规则 */
            if(0 == pstcb->posid)
            {
                pstacl->mAclId  = pstcb->acl_key.acl_id;
                pstacl->mRuleId = pstcb->acl_key.rule_id;
                pstacl->flags  |= ACL_COUNTER_ON;

                /* 0/1 = Drop丢弃/Permit通过 */
                if(0 == pstcb->basic_cfg_cb->action)
                {
                    pstacl->eAclAction = ACL_ACTION_DROP;
                }
                else
                {
                    pstacl->eAclAction = ACL_ACTION_PASS;
                }
            }
        }
        pstacl->tAclRule.tAclKey.ethType  = 0x86dd;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;
        OS_MEMCPY(pstacl->tAclRule.tAclKey.dipv6, ptemp->dst_ip, ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(ptemp->dst_ip_mask_len, (NBB_ULONG*)&(pstacl->tAclRule.tAclMask.dipv6));
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        OS_MEMCPY(pstacl->tAclRule.tAclKey.sipv6, ptemp->src_ip, ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(ptemp->src_ip_mask_len, (NBB_ULONG*)&(pstacl->tAclRule.tAclMask.sipv6));
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        pstacl->tAclRule.tAclKey.l3Protocol  = ptemp->l3_protocol;
        pstacl->tAclRule.tAclMask.l3Protocol = ptemp->l3_protocol_mask;
        pstacl->tAclRule.tAclKey.l4SrcPort   = ptemp->src_port;
        pstacl->tAclRule.tAclMask.l4SrcPort  = ptemp->src_port_mask;
        pstacl->tAclRule.tAclKey.l4DstPort   = ptemp->dst_port;
        pstacl->tAclRule.tAclMask.l4DstPort  = ptemp->dst_port_mask;
        pstacl->tAclRule.tAclKey.tos = ptemp->tos;
        pstacl->tAclRule.tAclMask.tos = ptemp->tos_mask;

        if(0 == pstcb->posid)
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetAcl(unit, pstacl);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    spm_api_c3_set_acl_error_log(unit, pstacl, 
                                 __FUNCTION__, __LINE__, ret);
                    goto EXIT_LABEL;
                }
                else
                {
                    pstcb->posid = pstacl->posId;
                }
#endif
            }
        }
        else
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = fhdrv_acl_update_rule(unit, pstcb->posid, &(pstacl->tAclRule));
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    printf("***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) fhdrv_acl_update_rule err.\n", 
                           __FUNCTION__, __LINE__, pstcb->posid, pstcb->acl_key.acl_id, 
                           pstcb->acl_key.rule_id);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) "
                               "fhdrv_acl_update_rule err.\n", __FUNCTION__, __LINE__, pstcb->posid, 
                               pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
#endif
            }
        }

        OS_MEMCPY(pstcb->ipv6_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_IPV6_FRAME));
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_acl_noip_cfg
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_update_acl_noip_cfg(SPM_QOS_ACL_CB *pstcb, NBB_ULONG oper, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_ACL_NONE_IP *ptemp = NULL;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_USHORT unit = 0;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
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

    if(ATG_DCI_OPER_ADD != oper)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:noip-oper of aid=%lu/rid=%lu can only be ATG_DCI_OPER_ADD.\n", 
               __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:noip-oper of aid=%lu/rid=%lu can only be "
                   "ATG_DCI_OPER_ADD.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
                   pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    ptemp = (ATG_DCI_ACL_NONE_IP*)buf;

   /********************************************************* 
    * 检查acl绑定树(g_acl_id_instance[i].instance_tree)中是 *
    * 否含有节点, 若不含节点表示该acl规则未被绑定到intf上.  *
    *********************************************************/
    cfg_cb = AVLL_FIRST(g_acl_id_instance[pstcb->acl_key.acl_id].instance_tree);

    /* 表示该aclid未被defend-policy引用,也未被intf引用 */
    if((0 == g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
       (NULL == cfg_cb))
    {
        /* NULL表示在未绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->noIp_cfg_cb)
        {
            pstcb->noIp_cfg_cb = (ATG_DCI_ACL_NONE_IP*)NBB_MM_ALLOC(    \
                                             sizeof(ATG_DCI_ACL_NONE_IP), 
                                   NBB_NORETRY_ACT, MEM_SPM_ACL_NOIP_CB);
            if (NULL == pstcb->noIp_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
        OS_MEMCPY(pstcb->noIp_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_NONE_IP));
    }

   /******************************************************************
    * 当aclid已被intf绑定时,不允许defend-policy再引用该aclid;同理,   *
    * 当aclid已被defend-policy引用时,不允许再将该aclid其绑定到intf上.*
    ******************************************************************/
    else if((0 != g_acl_id_instance[pstcb->acl_key.acl_id].defend_aclid_cnt) && 
            (NULL != cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:aid=%lu/rid=%lu can't be used for both intf and "
               "defend-policy.\n", __FUNCTION__, __LINE__, pstcb->acl_key.acl_id, 
               pstcb->acl_key.rule_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu/rid=%lu can't be used for "
                   "both intf and defend-policy.\n", __FUNCTION__, __LINE__, 
                   pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

   /***************************************************** 
    * 表示该aclid未被defend-policy引用,但已被intf引用;  *
    * 或表示该aclid已被defend-policy引用,但未被intf引用.*
    *****************************************************/
    else
    {
        OS_MEMSET(pstacl, 0, sizeof(ACL_RULE_T));

        /* NULL表示在绑定的情况下,新增rule规则或更新的rule规则改变了match-type值 */
        if(NULL == pstcb->noIp_cfg_cb)
        {
            pstcb->noIp_cfg_cb = (ATG_DCI_ACL_NONE_IP*)NBB_MM_ALLOC(    \
                                             sizeof(ATG_DCI_ACL_NONE_IP), 
                                   NBB_NORETRY_ACT, MEM_SPM_ACL_NOIP_CB);
            if (NULL == pstcb->noIp_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* 0表示在绑定的情况下新增rule规则 */
            if(0 == pstcb->posid)
            {
                pstacl->mAclId  = pstcb->acl_key.acl_id;
                pstacl->mRuleId = pstcb->acl_key.rule_id;
                pstacl->flags  |= ACL_COUNTER_ON;

                /* 0/1 = Drop丢弃/Permit通过 */
                if(0 == pstcb->basic_cfg_cb->action)
                {
                    pstacl->eAclAction = ACL_ACTION_DROP;
                }
                else
                {
                    pstacl->eAclAction = ACL_ACTION_PASS;
                }
            }
        }
        pstacl->tAclRule.tAclKey.ethType  = ptemp->etype;
        pstacl->tAclRule.tAclMask.ethType = ptemp->etype_mask;
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData6),  &(ptemp->payload[0]), 4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData5),  &(ptemp->payload[4]), 4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData4),  &(ptemp->payload[8]), 4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData6), &(ptemp->payload_mask[0]), 4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData5), &(ptemp->payload_mask[4]), 4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData4), &(ptemp->payload_mask[8]), 4);

        if(0 == pstcb->posid)
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetAcl(unit, pstacl);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    spm_api_c3_set_acl_error_log(unit, pstacl, 
                                 __FUNCTION__, __LINE__, ret);
                    goto EXIT_LABEL;
                }
                else
                {
                    pstcb->posid = pstacl->posId;
                }
#endif
            }
        }
        else
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = fhdrv_acl_update_rule(unit, pstcb->posid, &(pstacl->tAclRule));
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    printf("***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) fhdrv_acl_update_rule err.\n", 
                           __FUNCTION__, __LINE__, pstcb->posid, pstcb->acl_key.acl_id, 
                           pstcb->acl_key.rule_id);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:intf-posid=%lu(aid=%lu/rid=%lu) "
                               "fhdrv_acl_update_rule err.\n", __FUNCTION__, __LINE__, pstcb->posid, 
                               pstcb->acl_key.acl_id, pstcb->acl_key.rule_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
#endif
            }
        }

        OS_MEMCPY(pstcb->ipv6_cfg_cb, ptemp, sizeof(ATG_DCI_ACL_NONE_IP));
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_alloc_acl_cb
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_acl(ATG_DCI_SET_ACL *pstsetips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT unit = 0;
    ACL_T      acl  = {0};
    ATG_DCI_ACL_KEY ulkey = {0};
    SPM_QOS_ACL_CB *pstcb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucbasicdatastart = NULL;
    NBB_BYTE *pucportdatastart  = NULL;
    NBB_BYTE *pucethdatastart   = NULL;
    NBB_BYTE *pucipv4datastart  = NULL;
    NBB_BYTE *puciptcpdatastart = NULL;
    NBB_BYTE *pucipv6datastart  = NULL;
    NBB_BYTE *pucnoipdatastart  = NULL;
    NBB_BYTE *pucipudpstart     = NULL;

    /* 无基本配置 */
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

    /* 获取子配置操作 */
    uloperbasic = pstsetips->oper_basic;
    uloperport  = pstsetips->oper_port;
    ulopereth   = pstsetips->oper_eth_header;
    uloperipv4  = pstsetips->oper_ipv4_frame;
    uloperiptcp = pstsetips->oper_ipv4_tcp;
    uloperipv6  = pstsetips->oper_ipv6_frame;
    ulopernoip  = pstsetips->oper_none_ip;
    uloperipudp = pstsetips->oper_ipv4_udp;

    /* 获取子配置的偏移地址 */
    pucbasicdatastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->basic_data);
    pucportdatastart  = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->port_data);
    pucethdatastart   = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->eth_header_data);
    pucipv4datastart  = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->ipv4_frame_data);
    puciptcpdatastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->ipv4_tcp_data);
    pucipv6datastart  = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->ipv6_frame_data);
    pucnoipdatastart  = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->none_ip_data);
    pucipudpstart     = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->ipv4_udp_data);

    pstsetips->return_code = ATG_DCI_RC_OK;
    OS_MEMCPY(&ulkey, &(pstsetips->key), sizeof(ATG_DCI_ACL_KEY));
    pstcb = AVLL_FIND(g_qos_acl_tree, &ulkey);
    if(NULL == pstcb)
    {
        /* 错误配置块 */
        if (TRUE == pstsetips->delete_struct)
        {
            pstsetips->return_code = ATG_DCI_RC_DEL_FAILED;
            printf("***err***%s,%d:no aid=%lu/rid=%lu cfg can be deleted.\n", __FUNCTION__, 
                   __LINE__, ulkey.acl_id, ulkey.rule_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no aid=%lu/rid=%lu cfg can be deleted.\n", 
                       __FUNCTION__, __LINE__, ulkey.acl_id, ulkey.rule_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;  
        }

        /* 新增rule规则 */
        else
        {
            pstcb = spm_alloc_acl_cb(&ulkey NBB_CCXT);
            if (NULL == pstcb)
            {
                pstsetips->return_code = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:spm_alloc_acl_cb failed.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_acl_cb failed.\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            ret = spm_update_acl_basic_cfg(pstcb, uloperbasic, pucbasicdatastart);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_free_acl_cb(pstcb);
                pstsetips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstsetips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:spm_update_acl_basic_cfg failed.\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_update_acl_basic_cfg failed.\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* 使用更新后的match_type类型值做判断条件 */
            switch(pstcb->basic_cfg_cb->match_type)
            {
                case MATCH_PORT:
                ret = spm_update_acl_port_cfg(pstcb, uloperport, pucportdatastart);
                pstsetips->port_return_code = ret;
                break;

                case MATCH_ETH_HEADER:
                ret = spm_update_acl_eth_cfg(pstcb, ulopereth, pucethdatastart);
                pstsetips->eth_header_return_code = ret;
                break;

                case MATCH_IPV4:
                ret = spm_update_acl_ipv4_cfg(pstcb, uloperipv4, pucipv4datastart);
                pstsetips->ipv4_frame_return_code = ret;
                break;

                case MATCH_IP_TCP:
                ret = spm_update_acl_iptcp_cfg(pstcb, uloperiptcp, puciptcpdatastart);
                pstsetips->ipv4_tcp_return_code = ret;
                break;

                case MATCH_IP_UDP:
                ret = spm_update_acl_ipudp_cfg(pstcb, uloperipudp, pucipudpstart);
                pstsetips->ipv4_udp_return_code = ret;
                break;

                case MATCH_IPV6:
                ret = spm_update_acl_ipv6_cfg(pstcb, uloperipv6, pucipv6datastart);
                pstsetips->ipv6_frame_return_code = ret;
                break;

                case MATCH_NOIP:
                ret = spm_update_acl_noip_cfg(pstcb, ulopernoip, pucnoipdatastart);
                pstsetips->none_ip_return_code = ret;
                break;

                default:
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:match_type=%d of aid=%lu mismatch.\n", __FUNCTION__, 
                       __LINE__, pstcb->basic_cfg_cb->match_type, ulkey.acl_id);
                OS_SPRINTF(ucmessage,"***err***%s,%d:match_type=%d of aid=%lu mismatch.\n",
                           __FUNCTION__, __LINE__, pstcb->basic_cfg_cb->match_type, ulkey.acl_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                break;
            }

            /* 新增失败后删除申请的内存空间 */
            if(ATG_DCI_RC_OK != ret)
            {
                pstsetips->return_code = ret;
                spm_free_acl_cb(pstcb);
                printf("***err***%s,%d:store aid=%lu/rid=%lu cfg failed.\n", __FUNCTION__, 
                       __LINE__, ulkey.acl_id, ulkey.rule_id);
                OS_SPRINTF(ucmessage,"***err***%s,%d:store aid=%lu/rid=%lu cfg failed.\n", 
                           __FUNCTION__, __LINE__, ulkey.acl_id, ulkey.rule_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            else
            {
                //coverity[no_effect_test]
                rv = AVLL_INSERT(g_qos_acl_tree, pstcb->spm_acl_node);
                NBB_INSERT_BEFORE(g_acl_id_instance[ulkey.acl_id].acl_group, pstcb->spm_acl_group_node);
            }
        }
    }
    else
    {
        /* 删除配置块 */
        if (TRUE == pstsetips->delete_struct)
        {
            /* posid不为0表示该规则被成功写入了驱动中 */
            if(0 != pstcb->posid)
            {
                OS_MEMSET(&acl, 0, sizeof(ACL_T));
                acl.mAclId  = pstcb->acl_key.acl_id;
                acl.mRuleId = pstcb->acl_key.rule_id;

                for(unit = 0; unit < SHARED.c3_num; unit++)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    ret = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        ret = ATG_DCI_RC_DRIVER_FAILED;
                        spm_api_c3_del_acl_error_log(unit, &acl, 
                                   __FUNCTION__, __LINE__, ret);
                    }
#endif
                }
            }
            AVLL_DELETE(g_qos_acl_tree, pstcb->spm_acl_node);
            NBB_REMOVE_FROM_LIST(pstcb->spm_acl_group_node);
            spm_free_acl_cb(pstcb);

            /* 该aclid的rule规则全部删完了以后,初始化aclid链表的match_type */
            if(NBB_EMPTY_LIST(g_acl_id_instance[ulkey.acl_id].acl_group))
            {
                g_acl_id_instance[ulkey.acl_id].match_type = MATCH_ERROR;
            }
        }

        /* 更新rule规则 */
        else
        {
            ret = spm_update_acl_basic_cfg(pstcb, uloperbasic, pucbasicdatastart);
            if(ATG_DCI_RC_OK != ret)
            {
                pstsetips->return_code = ret;
                pstsetips->basic_return_code = ret;
                printf("***err***%s,%d:spm_update_acl_basic_cfg failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_update_acl_basic_cfg failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* 使用新的match_type值做判断条件 */
            switch(pstcb->basic_cfg_cb->match_type)
            {
                case MATCH_PORT:
                ret = spm_update_acl_port_cfg(pstcb, uloperport, pucportdatastart);
                pstsetips->port_return_code = ret;
                break;

                case MATCH_ETH_HEADER:
                ret = spm_update_acl_eth_cfg(pstcb, ulopereth, pucethdatastart);
                pstsetips->eth_header_return_code = ret;
                break;

                case MATCH_IPV4:
                ret = spm_update_acl_ipv4_cfg(pstcb, uloperipv4, pucipv4datastart);
                pstsetips->ipv4_frame_return_code = ret;
                break;

                case MATCH_IP_TCP:
                ret = spm_update_acl_iptcp_cfg(pstcb, uloperiptcp, puciptcpdatastart);
                pstsetips->ipv4_tcp_return_code = ret;
                break;

                case MATCH_IP_UDP:
                ret = spm_update_acl_ipudp_cfg(pstcb, uloperipudp, pucipudpstart);
                pstsetips->ipv4_udp_return_code = ret;
                break;

                case MATCH_IPV6:
                ret = spm_update_acl_ipv6_cfg(pstcb, uloperipv6, pucipv6datastart);
                pstsetips->ipv6_frame_return_code = ret;
                break;

                case MATCH_NOIP:
                ret = spm_update_acl_noip_cfg(pstcb, ulopernoip, pucnoipdatastart);
                pstsetips->none_ip_return_code = ret;
                break;

                default:
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:match_type=%d of aid=%lu mismatch.\n", __FUNCTION__, 
                       __LINE__, pstcb->basic_cfg_cb->match_type, ulkey.acl_id);
                OS_SPRINTF(ucmessage,"***err***%s,%d:match_type=%d of aid=%lu mismatch.\n",
                           __FUNCTION__, __LINE__, pstcb->basic_cfg_cb->match_type, ulkey.acl_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                break;
            }
            if(ATG_DCI_RC_OK != ret)
            {
               /*********************************************************************
                * 更新rule规则未改变match_type且更新过程失败时,不会有配置残留.同时, *
                * 更新rule规则时,MATCH_PORT/MATCH_ETH_HEADER的match_type不可能改变. *
                *********************************************************************/
                if(pstcb->match_tpye != pstcb->basic_cfg_cb->match_type)
                {
                   /*****************************************************
                    * 当更新的rule规则改变了match_type且更新过程失败时, *
                    * 则将更新过程中申请的内存空间释放并还原match_type. *
                    *****************************************************/
                    spm_free_acl_rule_tlv(pstcb, ret);
                    pstcb->basic_cfg_cb->match_type = pstcb->match_tpye;
                }
                pstsetips->return_code = ret;
                printf("***err***%s,%d:store aid=%lu/rid=%lu cfg failed.\n", __FUNCTION__, 
                       __LINE__, ulkey.acl_id, ulkey.rule_id);
                OS_SPRINTF(ucmessage,"***err***%s,%d:store aid=%lu/rid=%lu cfg failed.\n", 
                           __FUNCTION__, __LINE__, ulkey.acl_id, ulkey.rule_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            else
            {
               /******************************************************
                * 更新前的match_type:pstcb->match_tpye               *
                * 更新后的match_type:pstcb->basic_cfg_cb->match_type *
                * 当更新的rule规则改变了match_type且更新过程成功时,  *
                * 释放初始match_type的子tlv空间并更新match_type的值. *
                ******************************************************/
                if(pstcb->match_tpye != pstcb->basic_cfg_cb->match_type)
                {
                    spm_free_acl_rule_tlv(pstcb, ret);
                    pstcb->match_tpye = pstcb->basic_cfg_cb->match_type;
                }
            }
        }
    }

    /* 异常跳出 */
    EXIT_LABEL:NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   函 数 名  : spm_alloc_acl_cb
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_qos_clear_all_acl(void)
{
    SPM_QOS_ACL_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_ACL_CB*)AVLL_FIRST(g_qos_acl_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_ACL_CB*)AVLL_FIRST(g_qos_acl_tree))
    {
        NBB_REMOVE_FROM_LIST(cfg_cb->spm_acl_group_node);
        AVLL_DELETE(g_qos_acl_tree, cfg_cb->spm_acl_node);
        spm_free_acl_cb(cfg_cb);   
    }
    return;   
}

#endif


#if 3

/*****************************************************************************
   函 数 名  : spm_set_acl_rule_port_cfg
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_rule_port_cfg(SPM_QOS_ACL_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_BYTE unit = 0;
    NBB_USHORT ussvlan = 0;
    NBB_LONG ret  = ATG_DCI_RC_OK;
    SPM_PORT_INFO_CB pstportinfo = {0};
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 消息内容为空 */
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb->Port_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:ATG_DCI_ACL_PORT_DATA of acl-rule err.\n", __FUNCTION__, 
                   __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:ATG_DCI_ACL_PORT_DATA of acl-rule err.\n", 
                       __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    OS_MEMSET(pstacl, 0, sizeof(ACL_T));
    pstacl->mAclId  = pcb->acl_key.acl_id;
    pstacl->mRuleId = pcb->acl_key.rule_id;

    /* 0/1 = Drop丢弃/Permit通过 */
    if(0 == pcb->basic_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_DROP;
    }
    else
    {
        pstacl->eAclAction = ACL_ACTION_PASS;
    }
    OS_MEMSET(&pstportinfo, 0, sizeof(SPM_PORT_INFO_CB));
    ret = spm_get_portid_from_logical_port_index(pcb->Port_cfg_cb->index, &pstportinfo);
    if(ATG_DCI_RC_OK != ret)
    {
        ret = ATG_DCI_RC_ARP_NO_PORT;
        printf("***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index err.\n", 
               __FUNCTION__, __LINE__, pcb->Port_cfg_cb->index);
        OS_SPRINTF(ucmessage,"***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index"
                   " err.\n", __FUNCTION__, __LINE__, pcb->Port_cfg_cb->index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* VE口 */
    if((0 == pstportinfo.slot_id) && (3 == pstportinfo.port_type))
    {
        /* 二层VE口 */
        if(0 == pstportinfo.svlan)
        {
           pstacl->tAclRule.tAclKey.ePortType = ACL_L2VE;
           pstacl->tAclRule.tAclKey.port = pstportinfo.port_id;
           pstacl->tAclRule.tAclMask.ePortType = 0xffff;
           pstacl->tAclRule.tAclMask.port      = 0xffff;
        }

        /* 三层VE口 */
        else
        {
           pstacl->tAclRule.tAclKey.ePortType  = ACL_L3VE;
           pstacl->tAclRule.tAclKey.port = pstportinfo.port_id;
           pstacl->tAclRule.tAclMask.ePortType = 0xffff;
           pstacl->tAclRule.tAclMask.port      = 0xffff;
        }
    }

    /* 普通logic或lag口 */
    else
    {
        /* 二层口 */
        if(0 == pstportinfo.svlan)
        {
            ret = spm_get_vlan_from_logical_port_index(pcb->Port_cfg_cb->index, &ussvlan);
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_ARP_NO_PORT;
                printf("***err***%s,%d:intf-log=%lu spm_get_vlan_from_logical_port_index err.\n", 
                       __FUNCTION__, __LINE__, pcb->Port_cfg_cb->index);
                OS_SPRINTF(ucmessage,"***err***%s,%d:intf-log=%lu spm_get_vlan_from_logical_port_index"
                           " err.\n", __FUNCTION__, __LINE__, pcb->Port_cfg_cb->index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            if(0 != ussvlan)
            {
                pstacl->tAclRule.tAclKey.oVid  = ussvlan;
                pstacl->tAclRule.tAclMask.oVid = 0x0fff;
            }
        }

        /* 三层口 */
        else
        {
            pstacl->tAclRule.tAclKey.oVid  = pstportinfo.svlan;
            pstacl->tAclRule.tAclMask.oVid = 0x0fff;
        }
        pstacl->tAclRule.tAclKey.ePortType  = ACL_PORT;
        pstacl->tAclRule.tAclKey.port       = pstportinfo.port_id;
        pstacl->tAclRule.tAclMask.ePortType = 0xffff;
        pstacl->tAclRule.tAclMask.port      = 0xffff;     
    }

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstacl);
        if(ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_DRIVER_FAILED;
            printf("***err***%s,%d:aid=%d/rid=%d ApiC3SetAcl err.\n", __FUNCTION__, 
                   __LINE__, pstacl->mAclId, pstacl->mRuleId);
            OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%d/rid=%d ApiC3SetAcl err.\n", 
                       __FUNCTION__, __LINE__, pstacl->mAclId, pstacl->mRuleId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        else
        {
            pcb->posid = pstacl->posId;
        }
#endif
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_set_acl_rule_eth_cfg
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_rule_eth_cfg(SPM_QOS_ACL_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE unit = 0;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_LONG ret  = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 消息内容为空 */
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb->Eth_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:ATG_DCI_ACL_ETH_HEADER of acl-rule err.\n", __FUNCTION__, 
                   __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:ATG_DCI_ACL_ETH_HEADER of acl-rule err.\n", 
                       __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    OS_MEMSET(pstacl, 0, sizeof(ACL_T));
    pstacl->mAclId  = pcb->acl_key.acl_id;
    pstacl->mRuleId = pcb->acl_key.rule_id;

    /* 0/1 = Drop丢弃/Permit通过 */
    if(0 == pcb->basic_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_DROP;
    }
    else
    {
        pstacl->eAclAction = ACL_ACTION_PASS;
    }
    OS_MEMCPY(&(pstacl->tAclRule.tAclKey.smac[0]), &(pcb->Eth_cfg_cb->src_mac[0]), ATG_DCI_MAC_LEN);
    OS_MEMCPY(&(pstacl->tAclRule.tAclKey.dmac[0]), &(pcb->Eth_cfg_cb->dst_mac[0]), ATG_DCI_MAC_LEN);
    OS_MEMCPY(&(pstacl->tAclRule.tAclMask.smac[0]), &(pcb->Eth_cfg_cb->src_mac_mask[0]), ATG_DCI_MAC_LEN);
    OS_MEMCPY(&(pstacl->tAclRule.tAclMask.dmac[0]), &(pcb->Eth_cfg_cb->dst_mac_mask[0]), ATG_DCI_MAC_LEN);
    pstacl->tAclRule.tAclKey.ovlan_pri  = pcb->Eth_cfg_cb->vlan_prio;
    pstacl->tAclRule.tAclMask.ovlan_pri = pcb->Eth_cfg_cb->vlan_prio_mask;
    pstacl->tAclRule.tAclKey.ethType  = 0x0800;
    pstacl->tAclRule.tAclMask.ethType = 0xffff;
    pstacl->flags |= ACL_COUNTER_ON;

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstacl);
        if(ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_DRIVER_FAILED;
            spm_api_c3_set_acl_error_log(unit, pstacl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
        else
        {
            pcb->posid = pstacl->posId;
        }
#endif
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_set_acl_rule_ipv4_cfg
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_rule_ipv4_cfg(SPM_QOS_ACL_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE unit = 0;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 消息内容为空 */
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb->ipv4_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:ATG_DCI_ACL_IPV4_FRAME of acl-rule err.\n", __FUNCTION__, 
                   __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:ATG_DCI_ACL_IPV4_FRAME of acl-rule err.\n", 
                       __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    if (6 == pcb->ipv4_cfg_cb->l3_protocol)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:ipv4_cfg_cb->l3_protocol should be not 6.\n", __FUNCTION__, 
               __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:ipv4_cfg_cb->l3_protocol should be not 6.\n", 
                   __FUNCTION__, __LINE__, ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstacl, 0, sizeof(ACL_T));
    pstacl->mAclId  = pcb->acl_key.acl_id;
    pstacl->mRuleId = pcb->acl_key.rule_id;

    /* 0/1 = Drop丢弃/Permit通过 */
    if(0 == pcb->basic_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_DROP;
    }
    else
    {
        pstacl->eAclAction = ACL_ACTION_PASS;
    }
    pstacl->tAclRule.tAclKey.dip  = pcb->ipv4_cfg_cb->dst_ip;
    pstacl->tAclRule.tAclMask.dip = pcb->ipv4_cfg_cb->dst_ip_mask;
    pstacl->tAclRule.tAclKey.sip  = pcb->ipv4_cfg_cb->src_ip;
    pstacl->tAclRule.tAclMask.sip = pcb->ipv4_cfg_cb->src_ip_mask;

    /* 主控是按照tos下发给我们的 */
    pstacl->tAclRule.tAclKey.tos  = (pcb->ipv4_cfg_cb->dscp);
    pstacl->tAclRule.tAclMask.tos = (pcb->ipv4_cfg_cb->dscp_mask);
    pstacl->tAclRule.tAclKey.l3Protocol  = pcb->ipv4_cfg_cb->l3_protocol;
    pstacl->tAclRule.tAclMask.l3Protocol = pcb->ipv4_cfg_cb->l3_protocol_mask;
    pstacl->tAclRule.tAclKey.ethType  = 0x0800;
    pstacl->tAclRule.tAclMask.ethType = 0xffff;
    pstacl->tAclRule.tAclKey.ipHeaderMF  = pcb->ipv4_cfg_cb->mf;
    pstacl->tAclRule.tAclMask.ipHeaderMF = pcb->ipv4_cfg_cb->mf_mask;
    pstacl->tAclRule.tAclKey.ipHeaderDF  = pcb->ipv4_cfg_cb->df;
    pstacl->tAclRule.tAclMask.ipHeaderDF = pcb->ipv4_cfg_cb->df_mask;
    pstacl->tAclRule.tAclKey.l4SrcPort   = pcb->ipv4_cfg_cb->src_port;
    pstacl->tAclRule.tAclMask.l4SrcPort  = pcb->ipv4_cfg_cb->src_port_mask;
    pstacl->tAclRule.tAclKey.l4DstPort   = pcb->ipv4_cfg_cb->dst_port;
    pstacl->tAclRule.tAclMask.l4DstPort  = pcb->ipv4_cfg_cb->dst_port_mask;
    pstacl->flags |= ACL_COUNTER_ON;
    if((0x01 == pcb->ipv4_cfg_cb->l3_protocol)      && 
       (0xff == pcb->ipv4_cfg_cb->l3_protocol_mask) && 
       (0x00 != pcb->ipv4_cfg_cb->payload_mask[0]))
    {
        pstacl->tAclRule.tAclKey.icmpType  = pcb->ipv4_cfg_cb->payload[0];
        pstacl->tAclRule.tAclMask.icmpType = pcb->ipv4_cfg_cb->payload_mask[0];
    }
    if((0x01 == pcb->ipv4_cfg_cb->l3_protocol)      && 
       (0xff == pcb->ipv4_cfg_cb->l3_protocol_mask) && 
       (0x00 != pcb->ipv4_cfg_cb->payload_mask[1]))
    {
        pstacl->tAclRule.tAclKey.icmpCode  = pcb->ipv4_cfg_cb->payload[1];
        pstacl->tAclRule.tAclMask.icmpCode = pcb->ipv4_cfg_cb->payload_mask[1];
    }

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstacl);
        if(ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_DRIVER_FAILED;
            spm_api_c3_set_acl_error_log(unit, pstacl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
        else
        {
            pcb->posid = pstacl->posId;
        }
#endif
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_set_acl_rule_iptcp_cfg
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_rule_iptcp_cfg(SPM_QOS_ACL_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE unit = 0;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_BYTE tcp_flag = 0;
    NBB_LONG ret  = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 消息内容为空 */
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb->ipTcp_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:ATG_DCI_ACL_IPV4_TCP of acl-rule err.\n", __FUNCTION__, 
                   __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:ATG_DCI_ACL_IPV4_TCP of acl-rule err.\n", 
                       __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    OS_MEMSET(pstacl, 0, sizeof(ACL_T));
    pstacl->mAclId  = pcb->acl_key.acl_id;
    pstacl->mRuleId = pcb->acl_key.rule_id;

    /* 0/1 = Drop丢弃/Permit通过 */
    if(0 == pcb->basic_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_DROP;
    }
    else
    {
        pstacl->eAclAction = ACL_ACTION_PASS;
    }
    pstacl->tAclRule.tAclKey.dip  = pcb->ipTcp_cfg_cb->dst_ip;
    pstacl->tAclRule.tAclMask.dip = pcb->ipTcp_cfg_cb->dst_ip_mask;
    pstacl->tAclRule.tAclKey.sip  = pcb->ipTcp_cfg_cb->src_ip;
    pstacl->tAclRule.tAclMask.sip = pcb->ipTcp_cfg_cb->src_ip_mask;

    /* 主控是按照tos下发给我们的 */
    pstacl->tAclRule.tAclKey.tos  = (pcb->ipTcp_cfg_cb->dscp);
    pstacl->tAclRule.tAclMask.tos = (pcb->ipTcp_cfg_cb->dscp_mask);
    pstacl->tAclRule.tAclKey.l3Protocol  = 6;
    pstacl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstacl->tAclRule.tAclKey.ethType  = 0x0800;
    pstacl->tAclRule.tAclMask.ethType = 0xffff;
    pstacl->tAclRule.tAclKey.l4SrcPort  = pcb->ipTcp_cfg_cb->src_port;
    pstacl->tAclRule.tAclMask.l4SrcPort = pcb->ipTcp_cfg_cb->src_port_mask;
    pstacl->tAclRule.tAclKey.l4DstPort  = pcb->ipTcp_cfg_cb->dst_port;
    pstacl->tAclRule.tAclMask.l4DstPort = pcb->ipTcp_cfg_cb->dst_port_mask;  
    OS_MEMCPY(&tcp_flag, &(pcb->ipTcp_cfg_cb->tcp_flag), 1);
    pstacl->tAclRule.tAclKey.tcp_flag  = tcp_flag;
    pstacl->tAclRule.tAclMask.tcp_flag = pcb->ipTcp_cfg_cb->tcp_flag_mask;
    pstacl->flags |= ACL_COUNTER_ON;

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstacl);
        if(ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_DRIVER_FAILED;
            spm_api_c3_set_acl_error_log(unit, pstacl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
        else
        {
            pcb->posid = pstacl->posId;
        }
#endif
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_set_acl_rule_ipv6_cfg
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_rule_ipv6_cfg(SPM_QOS_ACL_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret  = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 消息内容为空 */
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb->ipv6_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:ATG_DCI_ACL_IPV6_FRAME of acl-rule err.\n", __FUNCTION__, 
                   __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:ATG_DCI_ACL_IPV6_FRAME of acl-rule err.\n", 
                       __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    OS_MEMSET(pstacl, 0, sizeof(ACL_T));
    pstacl->mAclId  = pcb->acl_key.acl_id;
    pstacl->mRuleId = pcb->acl_key.rule_id;

    /* 0/1 = Drop丢弃/Permit通过 */
    if(0 == pcb->basic_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_DROP;
    }
    else
    {
        pstacl->eAclAction = ACL_ACTION_PASS;
    }
    pstacl->tAclRule.tAclKey.ethType  = 0x86dd;
    pstacl->tAclRule.tAclMask.ethType = 0xffff;
    OS_MEMCPY(pstacl->tAclRule.tAclKey.dipv6, pcb->ipv6_cfg_cb->dst_ip, 
              ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
    ret = spm_get_ipv6_mask(pcb->ipv6_cfg_cb->dst_ip_mask_len,
              (NBB_ULONG*)&(pstacl->tAclRule.tAclMask.dipv6));
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    OS_MEMCPY(pstacl->tAclRule.tAclKey.sipv6, pcb->ipv6_cfg_cb->src_ip, 
              ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
    ret = spm_get_ipv6_mask(pcb->ipv6_cfg_cb->src_ip_mask_len, 
              (NBB_ULONG*)&(pstacl->tAclRule.tAclMask.sipv6));
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    pstacl->tAclRule.tAclKey.l3Protocol  = pcb->ipv6_cfg_cb->l3_protocol;
    pstacl->tAclRule.tAclMask.l3Protocol = pcb->ipv6_cfg_cb->l3_protocol_mask;
    pstacl->tAclRule.tAclKey.l4SrcPort   = pcb->ipv6_cfg_cb->src_port;
    pstacl->tAclRule.tAclMask.l4SrcPort  = pcb->ipv6_cfg_cb->src_port_mask;
    pstacl->tAclRule.tAclKey.l4DstPort   = pcb->ipv6_cfg_cb->dst_port;
    pstacl->tAclRule.tAclMask.l4DstPort  = pcb->ipv6_cfg_cb->dst_port_mask;
    pstacl->tAclRule.tAclKey.tos  = pcb->ipv6_cfg_cb->tos;
    pstacl->tAclRule.tAclMask.tos = pcb->ipv6_cfg_cb->tos_mask;
    pstacl->flags |= ACL_COUNTER_ON;

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstacl);
        if(ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_DRIVER_FAILED;
            spm_api_c3_set_acl_error_log(unit, pstacl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
        else
        {
            pcb->posid = pstacl->posId;
        }
#endif
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_set_acl_rule_ipudp_cfg
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_rule_ipudp_cfg(SPM_QOS_ACL_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 消息内容为空 */
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb->ipUdp_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:ATG_DCI_ACL_IPV4_UDP of acl-rule err.\n", __FUNCTION__, 
                   __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:ATG_DCI_ACL_IPV4_UDP of acl-rule err.\n", 
                       __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    OS_MEMSET(pstacl, 0, sizeof(ACL_T));
    pstacl->mAclId  = pcb->acl_key.acl_id;
    pstacl->mRuleId = pcb->acl_key.rule_id;
    pstacl->tAclRule.tAclKey.ethType  = 0x0800;
    pstacl->tAclRule.tAclMask.ethType = 0xffff;

    /* 0/1 = Drop丢弃/Permit通过 */
    if(0 == pcb->basic_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_DROP;
    }
    else
    {
        pstacl->eAclAction = ACL_ACTION_PASS;
    }
    pstacl->tAclRule.tAclKey.dip  = pcb->ipUdp_cfg_cb->dst_ip;
    pstacl->tAclRule.tAclMask.dip = pcb->ipUdp_cfg_cb->dst_ip_mask;
    pstacl->tAclRule.tAclKey.sip  = pcb->ipUdp_cfg_cb->src_ip;
    pstacl->tAclRule.tAclMask.sip = pcb->ipUdp_cfg_cb->src_ip_mask;

    /* 主控是按照tos下发给我们的 */
    pstacl->tAclRule.tAclKey.tos  = (pcb->ipUdp_cfg_cb->dscp);
    pstacl->tAclRule.tAclMask.tos = (pcb->ipUdp_cfg_cb->dscp_mask);
    pstacl->tAclRule.tAclKey.l3Protocol  = 0x11;
    pstacl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstacl->tAclRule.tAclKey.l4SrcPort  = pcb->ipUdp_cfg_cb->src_port;
    pstacl->tAclRule.tAclMask.l4SrcPort = pcb->ipUdp_cfg_cb->src_port_mask;
    pstacl->tAclRule.tAclKey.l4DstPort  = pcb->ipUdp_cfg_cb->dst_port;
    pstacl->tAclRule.tAclMask.l4DstPort = pcb->ipUdp_cfg_cb->dst_port_mask;
    pstacl->flags |= ACL_COUNTER_ON;

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstacl);
        if(ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_DRIVER_FAILED;
            spm_api_c3_set_acl_error_log(unit, pstacl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
        else
        {
            pcb->posid = pstacl->posId;
        }
#endif
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_set_acl_rule_noip_cfg
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_rule_noip_cfg(SPM_QOS_ACL_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE unit = 0;
    ACL_T  acl    = {0};
    ACL_T *pstacl = &acl;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 消息内容为空 */
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb->noIp_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:ATG_DCI_ACL_NONE_IP of acl-rule err.\n", __FUNCTION__, 
                   __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:ATG_DCI_ACL_NONE_IP of acl-rule err.\n", 
                       __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    OS_MEMSET(pstacl, 0, sizeof(ACL_T));
    pstacl->mAclId  = pcb->acl_key.acl_id;
    pstacl->mRuleId = pcb->acl_key.rule_id;

    /* 0/1 = Drop丢弃/Permit通过 */
    if(0 == pcb->basic_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_DROP;
    }
    else
    {
        pstacl->eAclAction = ACL_ACTION_PASS;
    }
    pstacl->tAclRule.tAclKey.ethType  = pcb->noIp_cfg_cb->etype;
    pstacl->tAclRule.tAclMask.ethType = pcb->noIp_cfg_cb->etype_mask;
    OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData6),  &(pcb->noIp_cfg_cb->payload[0]), 4);
    OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData5),  &(pcb->noIp_cfg_cb->payload[4]), 4);
    OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData4),  &(pcb->noIp_cfg_cb->payload[8]), 4);
    OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData6), &(pcb->noIp_cfg_cb->payload_mask[0]), 4);
    OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData5), &(pcb->noIp_cfg_cb->payload_mask[4]), 4);
    OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData4), &(pcb->noIp_cfg_cb->payload_mask[8]), 4);
    pstacl->flags |= ACL_COUNTER_ON;

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstacl);
        if(ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_DRIVER_FAILED;
            spm_api_c3_set_acl_error_log(unit, pstacl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
        else
        {
            pcb->posid = pstacl->posId;
        }
#endif
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_del_acl_rule_group_cfg
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_del_acl_rule_group_cfg(NBB_ULONG aclid)
{
    SPM_QOS_ACL_CB *pcb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    ACL_T    acl = {0};
    NBB_USHORT unit = 0;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* aclid未被绑定时,若该aclid中存在rule规则,删除驱动;若不存在规则,直接退出,并不会报错 */
    for (pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(g_acl_id_instance[aclid].acl_group);
         pcb != NULL; pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pcb->spm_acl_group_node))
    {
        /* posid不为0表示该规则被成功写入了驱动中 */
        if(0 != pcb->posid)
        {
            OS_MEMSET(&acl, 0, sizeof(ACL_T));
            acl.mAclId  = pcb->acl_key.acl_id;
            acl.mRuleId = pcb->acl_key.rule_id;

            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3DelAcl(unit, &acl);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_DRIVER_FAILED;
                    spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);
                }
#endif
            }
        }
        pcb->posid = 0;
    }

    /* 异常跳出 */
    EXIT_LABEL:NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_add_logic_acl_node
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_rule_group_cfg(NBB_ULONG aclid)
{
    SPM_QOS_ACL_CB *pcb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* aclid中存在规则时,写驱动;若不存在规则,直接退出,并不会报错 */
    for (pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(g_acl_id_instance[aclid].acl_group);
         pcb != NULL; pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pcb->spm_acl_group_node))
    {
        /* 调用驱动进行模板配置 */
        switch(pcb->basic_cfg_cb->match_type)
        {
            case MATCH_PORT:
            ret = spm_set_acl_rule_port_cfg(pcb);
            break;

            case MATCH_ETH_HEADER:
            ret = spm_set_acl_rule_eth_cfg(pcb);
            break;

            case MATCH_IPV4:
            ret = spm_set_acl_rule_ipv4_cfg(pcb);
            break;

            case MATCH_IP_TCP:
            ret = spm_set_acl_rule_iptcp_cfg(pcb);
            break;

            case MATCH_IP_UDP:
            ret = spm_set_acl_rule_ipudp_cfg(pcb);
            break;

            case MATCH_IPV6:
            ret = spm_set_acl_rule_ipv6_cfg(pcb);
            break;

            case MATCH_NOIP:
            ret = spm_set_acl_rule_noip_cfg(pcb);
            break;

            default:
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:match_type=%d of aid=%lu/rid=%lu mismatch.\n", __FUNCTION__, 
                   __LINE__, pcb->basic_cfg_cb->match_type, aclid, pcb->acl_key.rule_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:match_type=%d of aid=%lu/rid=%lu mismatch.\n", 
                       __FUNCTION__, __LINE__, pcb->basic_cfg_cb->match_type, aclid, 
                       pcb->acl_key.rule_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
            break;
        }
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    /* 异常跳出 */
    EXIT_LABEL:NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_del_aclid_of_intf
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_del_aclid_in_intf(SPM_QOS_LOGIC_ACL_CB *join_node)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == join_node)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input parameter err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input parameter err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

#if defined (SPU) || defined (PTN690_CES)

    /* 入口intf */
    if(0 == join_node->filter.direction)
    {
       ret = ApiC3SetIntfIngressAcl(join_node->sub_port.unit, join_node->sub_port.posid, 
                                                           0, join_node->filter.acl_id);
    }

    /* 出口intf */
    else
    {
       ret = ApiC3SetIntfEgressAcl(join_node->sub_port.unit, join_node->sub_port.posid, 
                                                          0, join_node->filter.acl_id);      
    }

    if (ATG_DCI_RC_OK != ret)
    {
        ret = ATG_DCI_RC_DRIVER_FAILED;
        printf("***err***%s,%d:%s del acl in log-intf=%lu failed.\n", __FUNCTION__, __LINE__, 
              (join_node->filter.direction) ? "ApiC3SetIntfEgressAcl" : "ApiC3SetIntfIngressAcl", 
               join_node->key.index);
        OS_SPRINTF(ucmessage,"***err***%s,%d:%s del acl in log-intf=%lu failed.\n", __FUNCTION__, 
                   __LINE__, (join_node->filter.direction) ? "ApiC3SetIntfEgressAcl" : 
                   "ApiC3SetIntfIngressAcl", join_node->key.index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
#endif 

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_add_aclid_to_intf
   功能描述  : 
   输入参数  :index:ACL ID,direct:方向
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_add_aclid_to_intf(ATG_DCI_LOG_PORT_TRAFFIC_FILTER *cfg, SUB_PORT *sub_port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == sub_port) || (NULL == cfg))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input parameter err.\n",__FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input parameter err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

#if defined (SPU) || defined (PTN690_CES)

    /* 入口intf */
    if(0 == cfg->direction)
    {
       ret = ApiC3SetIntfIngressAcl(sub_port->unit, sub_port->posid, 1, cfg->acl_id);
    }

    /* 出口intf */
    else
    {
       ret = ApiC3SetIntfEgressAcl(sub_port->unit, sub_port->posid, 1, cfg->acl_id);      
    }

    if (ATG_DCI_RC_OK != ret)
    {
        ret = ATG_DCI_RC_DRIVER_FAILED;
        printf("***err***%s,%d:%s add aid=%lu to intf-posid=%ld failed.\n", __FUNCTION__, 
               __LINE__, (cfg->direction) ? "ApiC3SetIntfEgressAcl" : "ApiC3SetIntfIngressAcl", 
               cfg->acl_id, sub_port->posid);
        OS_SPRINTF(ucmessage,"***err***%s,%d:%s add aid=%lu to intf-posid=%ld failed.\n", 
                   __FUNCTION__, __LINE__, (cfg->direction) ? "ApiC3SetIntfEgressAcl" : 
                   "ApiC3SetIntfIngressAcl", cfg->acl_id, sub_port->posid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_add_logic_acl_node
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_add_logic_acl_node(NBB_ULONG key,
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *cfg, SUB_PORT *sub_port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_QOS_LOGIC_ACL_KEY log_key = {0};
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_LOGIC_ACL_CB *join_node = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == sub_port) || (NULL == cfg) || (0 == key))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:Input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:Input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 判断是否是本槽位 */
    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_qos_acl_print)
    {
        printf("%s,%d:log-intf=%lu,phy-port=%d,intf-posid=%ld,aclid=%lu,direction=%s.\n",
               __FUNCTION__, __LINE__, key, sub_port->port, sub_port->posid, cfg->acl_id, 
              (cfg->direction) ? "出" : "入");
        OS_SPRINTF(ucmessage,"%s,%d:log-intf=%lu,phy-port=%d,intf-posid=%ld,aclid=%lu,"
                   "direction=%s.\n", __FUNCTION__, __LINE__, key, sub_port->port, 
                   sub_port->posid, cfg->acl_id, (cfg->direction) ? "出" : "入");
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
    }

    if((0x00 != cfg->direction) && (0x01 != cfg->direction))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:direction=%d parameter err.\n\n",__FUNCTION__, 
               __LINE__, cfg->direction);
        OS_SPRINTF(ucmessage,"***err***%s,%d:direction=%d parameter err.\n\n", 
                   __FUNCTION__, __LINE__, cfg->direction);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

   /************************************************************** 
    * classify_id的目的在于允许intf上入/出方向上各绑定多个aclid, *
    * 但是目前cr8k/ptn690/f1k上intf入/出方向上只能绑定一个aclid. *
    **************************************************************/
#if 0
    log_key.id = cfg->classify_id;
#else
    log_key.id = cfg->direction;
#endif

    /* 逻辑端口index */
    log_key.index = key;
    log_key.svlan = cfg->svlan;
    log_key.cvlan = cfg->cvlan;

    /* 判断该intf上是否绑定了acl */
    join_node = AVLL_FIND(g_qos_filter_tree, &log_key);
    if(NULL == join_node)
    {
        /* 删除错误:该intf上并未绑定aclid,无法删除 */
        if(0 == cfg->acl_id)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:no aclid in log-intf=%lu,del failed.\n", __FUNCTION__, 
                   __LINE__, key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no aclid in log-intf=%lu,del failed.\n", 
                       __FUNCTION__, __LINE__, key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 增加绑定 */
        else
        {
            /* 已经被defend-policy引用过的aclid不允许再绑定到intf上 */
            if(0 != g_acl_id_instance[cfg->acl_id].defend_aclid_cnt)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:aid=%lu has been used for defend-policy,instance "
                       "log-intf=%lu failed.\n", __FUNCTION__, __LINE__, cfg->acl_id, key);
                OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu has been used for defend-policy,"
                           "instance log-intf=%lu failed.\n", __FUNCTION__, __LINE__, 
                           cfg->acl_id, key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        
            /* 申请内存空间,失败则退出报错 */
            join_node = spm_alloc_logic_acl_cb(&log_key);
            if (NULL == join_node)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:spm_alloc_logic_acl_cb failed.\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_logic_acl_cb failed.\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        
            /* 判断该aclid是否已被其他的intf绑定过了,如果已被绑定,则不用再次写C3驱动 */
            if(NULL == AVLL_FIRST(g_acl_id_instance[cfg->acl_id].instance_tree))
            {
                /* 将aclid中的所有规则写入C3驱动中 */
                ret = spm_set_acl_rule_group_cfg(cfg->acl_id);
                if (ATG_DCI_RC_OK != ret)
                {
                   /******************************************* 
                    * 当acl规则写驱动时,只要其中某条写入失败, *
                    * 就将该aclid中所有成功写入的规则全部删除.* 
                    *******************************************/
                    spm_free_logic_acl_cb(join_node);
                    spm_del_acl_rule_group_cfg(cfg->acl_id);
                    printf("***err***%s,%d:spm_set_acl_rule_group_cfg failed.\n", __FUNCTION__, 
                           __LINE__);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:spm_set_acl_rule_group_cfg failed.\n", 
                               __FUNCTION__, __LINE__);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
                ret = spm_add_aclid_to_intf(cfg, sub_port);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_free_logic_acl_cb(join_node);
                    spm_del_acl_rule_group_cfg(cfg->acl_id);
                    printf("***err***%s,%d:spm_add_aclid_to_intf failed.\n", __FUNCTION__, 
                           __LINE__);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:spm_add_aclid_to_intf failed.\n", 
                               __FUNCTION__, __LINE__);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
            }
            else
            {
                ret = spm_add_aclid_to_intf(cfg, sub_port);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_free_logic_acl_cb(join_node);
                    printf("***err***%s,%d:spm_add_aclid_to_intf failed.\n", __FUNCTION__, 
                           __LINE__);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:spm_add_aclid_to_intf failed.\n", 
                               __FUNCTION__, __LINE__);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(&(join_node->filter), cfg, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
            OS_MEMCPY(&(join_node->sub_port), sub_port, sizeof(SUB_PORT));

           /*********************************************************************
            * SHARED.qos_filter_tree:该树表示本槽位上的intf分别绑定了哪个aclid. *
            * SHARED.g_acl_id_instance[aclid].instance_tree:                    *
            *                          该树表示指定的aclid被绑定在了哪些intf上. *
            *********************************************************************/
            //coverity[no_effect_test]
            rv = AVLL_INSERT(g_qos_filter_tree, join_node->spm_acl_node);

            //coverity[no_effect_test]
            rv = AVLL_INSERT(g_acl_id_instance[cfg->acl_id].instance_tree, join_node->spm_acl_id_node);
            goto EXIT_LABEL;
        }
    }
    else
    {
        /* 正常删除绑定关系 */
        if(0 == cfg->acl_id)
        {
            ret = spm_del_aclid_in_intf(join_node);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("***err***%s,%d:spm_del_aclid_in_intf failed.\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_del_aclid_in_intf failed.\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            AVLL_DELETE(g_qos_filter_tree, join_node->spm_acl_node);
            AVLL_DELETE(g_acl_id_instance[join_node->filter.acl_id].instance_tree, join_node->spm_acl_id_node);

            /* 若此时该aclid未被任何其他intf绑定,则删除C3驱动中的acl规则 */
            if(NULL == AVLL_FIRST(g_acl_id_instance[join_node->filter.acl_id].instance_tree))
            {
                spm_del_acl_rule_group_cfg(join_node->filter.acl_id);
            }
            spm_free_logic_acl_cb(join_node);
            goto EXIT_LABEL;
        }

        /* 更新绑定 */
        else
        {
            /* 已经被defend-policy引用过的aclid不允许再绑定到intf上 */
            if(0 != g_acl_id_instance[cfg->acl_id].defend_aclid_cnt)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:aid=%lu has been used for defend-policy,instance "
                       "log-intf=%lu failed.\n\n", __FUNCTION__, __LINE__, cfg->acl_id, key);
                OS_SPRINTF(ucmessage,"***err***%s,%d:aid=%lu has been used for defend-policy,"
                           "instance log-intf=%lu failed.\n\n", __FUNCTION__, __LINE__, 
                           cfg->acl_id, key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        
            /* 绑定相同的aclid,直接过滤,不做处理 */
            if(cfg->acl_id == join_node->filter.acl_id)
            {
                printf("%s,%d:same aid=%lu updated in log-intf=%lu.\n\n", __FUNCTION__, 
                       __LINE__, cfg->acl_id, join_node->key.index);
                OS_SPRINTF(ucmessage,"%s,%d:same aid=%lu updated in log-intf=%lu.\n\n", 
                           __FUNCTION__, __LINE__, cfg->acl_id, join_node->key.index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }

            /* 为避免解绑后,新的aclid写入失败导致配置不能还原,因此首先需要确保写驱动成功. */
            else
            {
                /* 判断新的aclid是否已被别的intf绑定过了,如果已被绑定,则不用再次写C3驱动 */
                if(NULL == AVLL_FIRST(g_acl_id_instance[cfg->acl_id].instance_tree))
                {
                    /* 将aclid中的所有规则写入C3驱动中 */
                    ret = spm_set_acl_rule_group_cfg(cfg->acl_id);
                    if (ATG_DCI_RC_OK != ret)
                    {
                       /******************************************* 
                        * 当acl规则写驱动时,只要其中某条写入失败, *
                        * 就将该aclid中所有成功写入的规则全部删除.* 
                        *******************************************/
                        spm_del_acl_rule_group_cfg(cfg->acl_id);
                        printf("***err***%s,%d:spm_set_acl_rule_group_cfg failed.\n", __FUNCTION__, 
                               __LINE__);
                        OS_SPRINTF(ucmessage,"***err***%s,%d:spm_set_acl_rule_group_cfg failed.\n", 
                                   __FUNCTION__, __LINE__);
                        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                        goto EXIT_LABEL;
                    }
                }
            
                /* 解除老aclid与intf的绑定关系 */
                ret = spm_del_aclid_in_intf(join_node);
                if(ATG_DCI_RC_OK != ret)
                {
                    /* 解绑失败,则删除之前成功写入驱动中的acl规则 */
                    if(NULL == AVLL_FIRST(g_acl_id_instance[cfg->acl_id].instance_tree))
                    {
                        spm_del_acl_rule_group_cfg(cfg->acl_id);
                    }
                    printf("***err***%s,%d:spm_del_aclid_in_intf failed.\n", __FUNCTION__, 
                           __LINE__);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:spm_del_aclid_in_intf failed.\n", 
                               __FUNCTION__, __LINE__);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }

                /* 建立新aclid与intf的绑定关系 */
                ret = spm_add_aclid_to_intf(cfg, sub_port);
                if (ATG_DCI_RC_OK != ret)
                {
                    /* 新aclid绑定失败时,进行配置还原(此时默认能成功还原) */
                    spm_add_aclid_to_intf(&(join_node->filter), sub_port);
                    if(NULL == AVLL_FIRST(g_acl_id_instance[cfg->acl_id].instance_tree))
                    {
                        spm_del_acl_rule_group_cfg(cfg->acl_id);
                    }
                    printf("***err***%s,%d:spm_add_aclid_to_intf failed.\n", __FUNCTION__, 
                           __LINE__);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:spm_add_aclid_to_intf failed.\n", 
                               __FUNCTION__, __LINE__);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
                
                AVLL_DELETE(g_acl_id_instance[join_node->filter.acl_id].instance_tree, 
                                                     join_node->spm_acl_id_node);

                /* 若此时老aclid已未被任何intf绑定,则删除C3驱动中的acl规则 */
                if(NULL == AVLL_FIRST(g_acl_id_instance[join_node->filter.acl_id].instance_tree))
                {
                    spm_del_acl_rule_group_cfg(join_node->filter.acl_id);
                }

                //coverity[no_effect_test]
                rv = AVLL_INSERT(g_acl_id_instance[cfg->acl_id].instance_tree, join_node->spm_acl_id_node);
                OS_MEMCPY(&(join_node->filter), cfg, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                goto EXIT_LABEL;
            }
        }    
    }

    /* 异常跳出 */
    EXIT_LABEL:NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_alloc_acl_cb
   功能描述  : 删除逻辑接口下所有的包过滤配置
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_qos_clear_logic_acl(NBB_LONG logic_key)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
    SPM_QOS_LOGIC_ACL_CB *next_cfg_cb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    cfg_cb = (SPM_QOS_LOGIC_ACL_CB*)AVLL_FIRST(g_qos_filter_tree);
    while(cfg_cb != NULL)
    {
        next_cfg_cb = (SPM_QOS_LOGIC_ACL_CB*)AVLL_NEXT(g_qos_filter_tree, cfg_cb->spm_acl_node);
        if(cfg_cb->key.index != logic_key)
        {
            cfg_cb = next_cfg_cb;
            continue;
        }

        AVLL_DELETE(g_qos_filter_tree, cfg_cb->spm_acl_node);
        if(NULL != AVLL_FIND(g_acl_id_instance[cfg_cb->filter.acl_id].instance_tree,
            &(cfg_cb->key)))
        {
            AVLL_DELETE(g_acl_id_instance[cfg_cb->filter.acl_id].instance_tree, cfg_cb->spm_acl_id_node);
        }
        spm_free_logic_acl_cb(cfg_cb);  
        cfg_cb = next_cfg_cb;
     }
    
    NBB_TRC_EXIT();
    return rv;   
}


/*****************************************************************************
   函 数 名  : spm_alloc_acl_cb
   功能描述  : 获取ipv6地址的掩码
   输入参数  : 掩码长度
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_qos_clear_all_logic_acl(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_LOGIC_ACL_CB*)AVLL_FIRST(g_qos_filter_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_LOGIC_ACL_CB*)AVLL_FIRST(g_qos_filter_tree))
    {      
        AVLL_DELETE(g_qos_filter_tree, cfg_cb->spm_acl_node);
        if(NULL != AVLL_FIND(g_acl_id_instance[cfg_cb->filter.acl_id].instance_tree,
            &(cfg_cb->key)))
        {
            AVLL_DELETE(g_acl_id_instance[cfg_cb->filter.acl_id].instance_tree,cfg_cb->spm_acl_id_node);
        }
        spm_free_logic_acl_cb(cfg_cb);
    }

    return;   
}

#endif


#if 4

/*****************************************************************************
   函 数 名  : spm_qos_acl_init
   功能描述  : 默认的DS映射关系初始化
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
NBB_LONG spm_qos_acl_init(void)
{
    NBB_ULONG i = 0;
    NBB_BUF_SIZE avll_key_offset;

    /* ACL树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_ACL_CB, acl_key);
    AVLL_INIT_TREE(g_qos_acl_tree, spm_qos_acl_comp, 
                  (NBB_USHORT)avll_key_offset, 
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_ACL_CB, spm_acl_node));

    /* 逻辑端口filter树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_LOGIC_ACL_CB, key);
    AVLL_INIT_TREE(g_qos_filter_tree, spm_qos_logic_acl_comp, 
                  (NBB_USHORT)avll_key_offset, 
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_LOGIC_ACL_CB, spm_acl_node));

    /* ACL组初始化 */
    for (i = 0; i < MAX_ACL_CFG_NUM; i++)
    {
        NBB_INIT_ROOT(g_acl_id_instance[i].acl_group);

        avll_key_offset = NBB_OFFSETOF(SPM_QOS_LOGIC_ACL_CB, key);
        AVLL_INIT_TREE(g_acl_id_instance[i].instance_tree, spm_qos_logic_acl_comp, 
                      (NBB_USHORT)avll_key_offset, 
                      (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_LOGIC_ACL_CB, spm_acl_id_node));
        g_acl_id_instance[i].match_type = MATCH_ERROR;
        g_acl_id_instance[i].defend_aclid_cnt = 0;
    }
}


/*****************************************************************************
   函 数 名  : spm_cfg_acl_cb_verify
   功能描述  : 默认的DS映射关系初始化
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
NBB_VOID spm_cfg_acl_cb_verify(void)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_ACL_CB *acl_cb = NULL;

    ATG_DCI_ACL_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_ACL_PORT_DATA *port_cfg_cb;

    ATG_DCI_ACL_ETH_HEADER *eth_cfg_cb;

    ATG_DCI_ACL_IPV4_FRAME *ipv4_cfg_cb;

    ATG_DCI_ACL_IPV4_TCP *iptcp_cfg_cb;

    ATG_DCI_ACL_IPV6_FRAME *ipv6_cfg_cb;

    ATG_DCI_ACL_NONE_IP *noip_cfg_cb;

    ATG_DCI_ACL_IPV4_UDP *ipudp_cfg_cb;

    NBB_TRC_ENTRY("spm_cfg_acl_cb_verify");

    for (acl_cb = (SPM_QOS_ACL_CB*)AVLL_FIRST(g_qos_acl_tree); acl_cb != NULL;
         acl_cb = (SPM_QOS_ACL_CB*)AVLL_NEXT(g_qos_acl_tree, acl_cb->spm_acl_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify acl_cb %p", acl_cb));
        NBB_VERIFY_MEMORY(acl_cb, sizeof(SPM_QOS_ACL_CB), MEM_SPM_ACL_CB);

        basic_cfg_cb = acl_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_ACL_BASIC_DATA),
                              MEM_SPM_ACL_BASIC_CB);
        }

        port_cfg_cb = acl_cb->Port_cfg_cb;
        if(port_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify Port_cfg_cb cb %p", port_cfg_cb));
            NBB_VERIFY_MEMORY(port_cfg_cb, sizeof(ATG_DCI_ACL_PORT_DATA),
                              MEM_SPM_ACL_PORT_CB);
        }

        eth_cfg_cb = acl_cb->Eth_cfg_cb;
        if(eth_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify Eth_cfg_cb cb %p", eth_cfg_cb));
            NBB_VERIFY_MEMORY(eth_cfg_cb, sizeof(ATG_DCI_ACL_ETH_HEADER),
                              MEM_SPM_ACL_ETH_CB);
        }

        ipv4_cfg_cb = acl_cb->ipv4_cfg_cb;
        if(ipv4_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_cfg_cb cb %p", ipv4_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_cfg_cb, sizeof(ATG_DCI_ACL_IPV4_FRAME),
                              MEM_SPM_ACL_IPV4_CB);
        }

        iptcp_cfg_cb = acl_cb->ipTcp_cfg_cb;
        if(iptcp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipTcp_cfg_cb cb %p", iptcp_cfg_cb));
            NBB_VERIFY_MEMORY(iptcp_cfg_cb, sizeof(ATG_DCI_ACL_IPV4_TCP),
                              MEM_SPM_ACL_IPTCP_CB);
        }

        ipv6_cfg_cb = acl_cb->ipv6_cfg_cb;
        if(ipv6_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_cfg_cb cb %p", ipv6_cfg_cb));
            NBB_VERIFY_MEMORY(ipv6_cfg_cb, sizeof(ATG_DCI_ACL_IPV6_FRAME),
                              MEM_SPM_ACL_IPV6_CB);
        }

        noip_cfg_cb = acl_cb->noIp_cfg_cb;
        if(noip_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify noIp_cfg_cb cb %p", noip_cfg_cb));
            NBB_VERIFY_MEMORY(noip_cfg_cb, sizeof(ATG_DCI_ACL_NONE_IP),
                              MEM_SPM_ACL_NOIP_CB);
        }

        ipudp_cfg_cb = acl_cb->ipUdp_cfg_cb;
        if(ipudp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipUdp_cfg_cb cb %p", ipudp_cfg_cb));
            NBB_VERIFY_MEMORY(ipudp_cfg_cb, sizeof(ATG_DCI_ACL_IPV4_UDP),
                              MEM_SPM_ACL_IPUDP_CB);
        }
        
    }

    NBB_TRC_EXIT();

    return;
}


/*****************************************************************************
   函 数 名  : spm_cfg_logic_acl_cb_verify
   功能描述  : 默认的DS映射关系初始化
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
NBB_VOID spm_cfg_logic_acl_cb_verify(void)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_logic_acl_cb_verify");

    for (cfg_cb = (SPM_QOS_LOGIC_ACL_CB*)AVLL_FIRST(g_qos_filter_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_LOGIC_ACL_CB*)AVLL_NEXT(g_qos_filter_tree, cfg_cb->spm_acl_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_LOGIC_ACL_CB), MEM_SPM_QOS_ACL_LOGIC_CB);
    }

    NBB_TRC_EXIT();

    return;
}

#endif
