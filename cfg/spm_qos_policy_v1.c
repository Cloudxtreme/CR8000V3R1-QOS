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

/* 外部全局树 */
extern AVLL_TREE g_qos_action_tree;

/* 外部全局树 */
extern SPM_QOS_CLASSIFY_ID_CB g_classify_id_instance[MAX_FLOW_CLASSIFY_PRI];

/* policy树 */
AVLL_TREE g_qos_policy_tree;

/* 上话复杂流分类树 */
AVLL_TREE g_qos_flow_classify_tree;

/* policy模块全局打印开关 */
NBB_BYTE g_qos_policy_print = ATG_DCI_RC_OK;

/* policy模块全局LOG开关 */
NBB_BYTE g_qos_policy_log   = ATG_DCI_RC_UNSUCCESSFUL;


#if 1

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
NBB_INT spm_qos_c2b_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2)
{
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *key1 = (ATG_DCI_QOS_POLICY_CB2QOS_DATA *)keyarg1;
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *key2 = (ATG_DCI_QOS_POLICY_CB2QOS_DATA *)keyarg2;
    NBB_INT ret = 0;
    ret = compare_ulong(&key1->qos_classify_index, &key2->qos_classify_index);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ulong(&key1->qos_behavior_index, &key2->qos_behavior_index);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    EXIT_LABEL:
    return ret;
}


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
NBB_LONG spm_release_policy_acl_id(NBB_ULONG acl_id)
{
    NBB_ULONG ret = ATG_DCI_RC_OK;
    
    if((acl_id >= CLASSIFY_PORT_ACL_ID) && (acl_id < CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM))
    {
        spm_release_classify_port_acl_id(acl_id);    
    }
    else if((acl_id >= CLASSIFY_ETH_ACL_ID) && (acl_id < CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM))
    {
        spm_release_classify_eth_acl_id(acl_id); 
    } 
    else if((acl_id >= CLASSIFY_HIGH_ACL_ID) && (acl_id < CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM))
    {
        spm_release_classify_high_acl_id(acl_id);    
    }

    return ret;
}

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
SPM_QOS_POLICY_C2B_CB *spm_alloc_c2b_cb(ATG_DCI_QOS_POLICY_CB2QOS_DATA *ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_C2B_CB *pstc2b = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == ulkey)
    {
        goto EXIT_LABEL;
    }

    pstc2b = (SPM_QOS_POLICY_C2B_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_POLICY_C2B_CB), 
                                         NBB_NORETRY_ACT, MEM_SPM_POLICY_C2B_CB);
    if(NULL == pstc2b)
    {
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(pstc2b, 0, sizeof(SPM_QOS_POLICY_C2B_CB));
    OS_MEMCPY(&(pstc2b->c2b_key), ulkey, sizeof(ATG_DCI_QOS_POLICY_CB2QOS_DATA));
    AVLL_INIT_NODE(pstc2b->spm_policy_c2b_node);

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return pstc2b;
}


/*****************************************************************************
   函 数 名  : spm_alloc_policy_cb
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
SPM_QOS_POLICY_CB *spm_alloc_policy_cb(NBB_ULONG ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT i = 0;
    NBB_BUF_SIZE avll_key_offset;
    SPM_QOS_POLICY_CB *pstcb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    pstcb = (SPM_QOS_POLICY_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_POLICY_CB),
                                    NBB_NORETRY_ACT, MEM_SPM_POLICY_CB);
    if (pstcb == NULL)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstcb, 0, sizeof(SPM_QOS_POLICY_CB));
    pstcb->policy_key = ulkey;

    pstcb->match_type = MATCH_ERROR;

    AVLL_INIT_NODE(pstcb->spm_policy_node);

    /* c2b树的初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_POLICY_C2B_CB, c2b_key);
    AVLL_INIT_TREE(pstcb->c2b_tree, spm_qos_c2b_comp, 
                  (NBB_USHORT)avll_key_offset,
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_POLICY_C2B_CB, spm_policy_c2b_node));

    /* 引用classify规则的逻辑端口树的初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, key);
    AVLL_INIT_TREE(pstcb->instance_tree, compare_ulong, 
                  (NBB_USHORT)avll_key_offset, 
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, spm_classify_id_node));

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstcb);
}


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
NBB_LONG spm_free_policy_c2b_cb(SPM_QOS_POLICY_C2B_CB *pstcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pstcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_POLICY_C2B_CB), MEM_SPM_POLICY_C2B_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_POLICY_C2B_CB);
    pstcb = NULL;

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_free_policy_cb(SPM_QOS_POLICY_CB *pstcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_C2B_CB *pcb = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pstcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_POLICY_CB), MEM_SPM_POLICY_CB);

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    if (NULL != pstcb->basic_cfg_cb)
    {         
        NBB_MM_FREE(pstcb->basic_cfg_cb, MEM_SPM_POLICY_BASIC_CB);
        pstcb->basic_cfg_cb = NULL;
    }

    /* 删除policy模板中的cb对信息 */
    for(pcb = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pstcb->c2b_tree); 
        pcb != NULL;
        pcb = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pstcb->c2b_tree))
    {
        AVLL_DELETE(pstcb->c2b_tree, pcb->spm_policy_c2b_node);
        NBB_MM_FREE(pcb, MEM_SPM_POLICY_C2B_CB);
    }

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_POLICY_CB);
    pstcb = NULL;

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 2

/*****************************************************************************
   函 数 名  : spm_check_policy_c2b_cfg
   功能描述  : 检查policy中cb对配置是否为空
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
NBB_LONG spm_check_policy_c2b_cfg(SPM_QOS_POLICY_CB *pstcb)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_ACTION_CB     *pstaction   = NULL;
    SPM_QOS_CLASSIFY_CB   *pstclassify = NULL;
    SPM_QOS_POLICY_C2B_CB *pstc2b      = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    for(pstc2b = (SPM_QOS_POLICY_C2B_CB*)AVLL_FIRST(pstcb->c2b_tree); pstc2b != NULL; 
        pstc2b = (SPM_QOS_POLICY_C2B_CB*)AVLL_NEXT(pstcb->c2b_tree, pstc2b->spm_policy_c2b_node))
    {
        /* 判断c2b中是否出现c或b模块空配置的情况 */
        pstaction   = AVLL_FIND(g_qos_action_tree, &(pstc2b->c2b_key.qos_behavior_index));
        pstclassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST(   \
                       g_classify_id_instance[pstc2b->c2b_key.qos_classify_index].classify_group);
        if((NULL != pstaction) && (NULL != pstclassify))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;  
}


/*****************************************************************************
   函 数 名  : spm_apply_policy_acl_id
   功能描述  : 申请policy模板所对应的acl_id
   输入参数  : policy模板配置的指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_ULONG spm_apply_policy_acl_id(NBB_BYTE matchtype)
{
    NBB_ULONG aclid = 0;
    NBB_LONG  ret   = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    if(MATCH_PORT == matchtype)
    {
        ret = spm_apply_classify_port_acl_id(&aclid);   
    }
    else if(MATCH_ETH_HEADER == matchtype)
    {
        ret = spm_apply_classify_eth_acl_id(&aclid); 
    }
    else if(MATCH_IP_TCP == matchtype)
    {
        ret = spm_apply_classify_high_acl_id(&aclid); 
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
    }
    if(ATG_DCI_RC_OK != ret)
    {
        aclid = 0;
    }

    return aclid;
}


/*****************************************************************************
   函 数 名  : spm_set_policy_match_type
   功能描述  : 判断policy的匹配类型
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
NBB_LONG spm_set_policy_match_type(SPM_QOS_POLICY_CB *pstcb, 
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *buf, NBB_BYTE *oldtype)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_ACTION_CB   *pstaction   = NULL;
    SPM_QOS_CLASSIFY_CB *pstclassify = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf) || (NULL == oldtype))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        /* 多cb对模式时,classify-id取值范围1-1999 */
        if((0 == buf->qos_classify_index) || (MAX_FLOW_CLASSIFY_PRI <= buf->qos_classify_index))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:cid <1-1999> while pid=%lu is c2b-mode.\n", __FUNCTION__, 
                   __LINE__, pstcb->policy_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:cid <1-1999> while pid=%lu is c2b-mode.\n", 
                       __FUNCTION__, __LINE__, pstcb->policy_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        } 
    }

    /* 记录policy模块初始匹配类型及cb对的匹配类型 */
    *oldtype    = pstcb->match_type;
    pstaction   = AVLL_FIND(g_qos_action_tree, &(buf->qos_behavior_index));
    pstclassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST(   \
                   g_classify_id_instance[buf->qos_classify_index].classify_group);
    if((NULL != pstaction) && (NULL != pstclassify))
    {
        if(MATCH_ERROR == pstcb->match_type)
        {
            pstcb->match_type = g_classify_id_instance[buf->qos_classify_index].match_type;
        }
        else
        {
            if(pstcb->match_type != g_classify_id_instance[buf->qos_classify_index].match_type)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:type of pid=%lu and bid=%lu/cid=%lu mismatch.\n", __FUNCTION__, 
                       __LINE__, pstcb->policy_key, buf->qos_behavior_index, buf->qos_classify_index);
                OS_SPRINTF(ucmessage,"***err***%s,%d:type of pid=%lu and bid=%lu/cid=%lu mismatch.\n", 
                           __FUNCTION__, __LINE__, pstcb->policy_key, buf->qos_behavior_index, 
                           buf->qos_classify_index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
        }
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:no bid=%lu/cid=%lu cfg, add pid=%lu c2b err.\n", __FUNCTION__, 
               __LINE__, buf->qos_behavior_index, buf->qos_classify_index, pstcb->policy_key);
        OS_SPRINTF(ucmessage,"***err***%s,%d:no bid=%lu/cid=%lu cfg, add pid=%lu c2b err.\n", 
                   __FUNCTION__, __LINE__, buf->qos_behavior_index, buf->qos_classify_index, 
                   pstcb->policy_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;  
}


/*****************************************************************************
   函 数 名  : spm_apply_policy_acl_ruleid
   功能描述  : 查找policy模板删除cb对中内存和驱动中的信息
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
NBB_ULONG  spm_apply_policy_acl_ruleid(NBB_BYTE *resoure)
{
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT ruleid = 0;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL == resoure)
    {
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    for(i = 0; i < POLICY_C2B_ACL_RULE_MAX; i++)
    {
        if(0xFF != (resoure[i] && 0xFF))
        {
            for(j = 0; j < 8; j++)
            {
                if(0x00 == (resoure[i] && (0x01 << j)))
                {
                    resoure[i] |= (0x01 << j);
                    ruleid = (i * 8) + j + 1;
                    goto EXIT_LABEL;
                }
            }
        }
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ruleid;
}


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
NBB_LONG  spm_release_policy_acl_ruleid(NBB_BYTE *resoure, NBB_ULONG ruleid)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);
    
    if((NULL == resoure) || (0 == ruleid) || (6800 < ruleid))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    if(0x01 == (resoure[(ruleid - 1) / 8] && (0x01 << ((ruleid - 1) % 8))))
    {
        resoure[(ruleid - 1) / 8] &= (0xFE << ((ruleid - 1) % 8));
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_del_acl_rule_driver(NBB_ULONG aclid, NBB_ULONG ruleid)
{
    ACL_T    acl  = {0};
    NBB_BYTE unit = 0;
    NBB_INT  ret  = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

	NBB_TRC_ENTRY(__FUNCTION__);

    if((0 == aclid) || (6000 < aclid) || (0 == ruleid) || (6800 < ruleid))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    NBB_MEMSET(&acl, 0, sizeof(ACL_T));
    acl.mAclId  = aclid;
    acl.mRuleId = ruleid;
    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3DelAcl(unit, &acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);
        }
#endif
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_set_policy_classify_acl(SPM_QOS_CLASSIFY_CB *pstclassify,
    SPM_QOS_ACTION_CB *pstaction, NBB_ULONG aclid, NBB_ULONG ruleid, NBB_ULONG *posid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    ACL_T acl;
    NBB_USHORT i  = 0;
    NBB_ULONG nhi = 0;
    NBB_USHORT unit = 0;
    ACL_T *pstacl   = NULL;
    NBB_BYTE tcp_flag  = 0;
    NBB_USHORT ussvlan = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_PORT_INFO_CB stportinfo = {0};
    SPM_PORT_INFO_CB pstportinfo = {0};
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstclassify) || (NULL == pstaction))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pstclassify->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    NBB_MEMSET(&acl, 0, sizeof(ACL_T));
    pstacl = &acl;
    pstacl->mAclId  = aclid;
    pstacl->mRuleId = ruleid;

    switch(pstclassify->basic_cfg_cb->match_type)
    {

        /* classify规则目前已取消匹配端口的功能 */
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
               pstacl->tAclRule.tAclKey.ePortType = ACL_L3VE;
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
                    pstacl->tAclRule.tAclKey.oVid  = ussvlan;
                    pstacl->tAclRule.tAclMask.oVid = 0x0fff;
                }
            }

            /* 三层口 */
            else
            {
                if(0 != pstportinfo.svlan)
                {
                    pstacl->tAclRule.tAclKey.oVid  = pstportinfo.svlan;
                    pstacl->tAclRule.tAclMask.oVid = 0x0fff;
                }
            }
            pstacl->tAclRule.tAclKey.ePortType  = ACL_PORT;
            pstacl->tAclRule.tAclKey.port       = pstportinfo.port_id;
            pstacl->tAclRule.tAclMask.ePortType = 0xffff;
            pstacl->tAclRule.tAclMask.port      = 0xffff;     
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
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.smac[0]), &(pstclassify->Eth_cfg_cb->src_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.dmac[0]), &(pstclassify->Eth_cfg_cb->dst_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.smac[0]), &(pstclassify->Eth_cfg_cb->src_mac_mask[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.dmac[0]), &(pstclassify->Eth_cfg_cb->dst_mac_mask[0]), ATG_DCI_MAC_LEN);
        pstacl->tAclRule.tAclKey.ovlan_pri = pstclassify->Eth_cfg_cb->vlan_prio;
        pstacl->tAclRule.tAclMask.ovlan_pri = pstclassify->Eth_cfg_cb->vlan_prio_mask;
        pstacl->tAclRule.tAclKey.ethType  = 0x0800;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;
        pstacl->flags |= ACL_COUNTER_ON;
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
        pstacl->tAclRule.tAclKey.dip = pstclassify->ipv4_cfg_cb->dst_ip;
        pstacl->tAclRule.tAclMask.dip = pstclassify->ipv4_cfg_cb->dst_ip_mask;
        pstacl->tAclRule.tAclKey.sip = pstclassify->ipv4_cfg_cb->src_ip;
        pstacl->tAclRule.tAclMask.sip = pstclassify->ipv4_cfg_cb->src_ip_mask;
        pstacl->tAclRule.tAclKey.tos = (pstclassify->ipv4_cfg_cb->dscp);
        pstacl->tAclRule.tAclMask.tos = (pstclassify->ipv4_cfg_cb->dscp_mask);
        pstacl->tAclRule.tAclKey.l3Protocol = pstclassify->ipv4_cfg_cb->l3_protocol;
        pstacl->tAclRule.tAclMask.l3Protocol = pstclassify->ipv4_cfg_cb->l3_protocol_mask;
        pstacl->tAclRule.tAclKey.ethType = 0x0800;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;
        pstacl->tAclRule.tAclKey.ipHeaderMF = pstclassify->ipv4_cfg_cb->mf;
        pstacl->tAclRule.tAclMask.ipHeaderMF = pstclassify->ipv4_cfg_cb->mf_mask;
        pstacl->tAclRule.tAclKey.ipHeaderDF = pstclassify->ipv4_cfg_cb->df;
        pstacl->tAclRule.tAclMask.ipHeaderDF = pstclassify->ipv4_cfg_cb->df_mask;
        pstacl->tAclRule.tAclKey.l4SrcPort = pstclassify->ipv4_cfg_cb->src_port;
        pstacl->tAclRule.tAclMask.l4SrcPort = pstclassify->ipv4_cfg_cb->src_port_mask;
        pstacl->tAclRule.tAclKey.l4DstPort = pstclassify->ipv4_cfg_cb->dst_port;
        pstacl->tAclRule.tAclMask.l4DstPort = pstclassify->ipv4_cfg_cb->dst_port_mask;
        pstacl->flags |= ACL_COUNTER_ON;
        if((1 == pstclassify->ipv4_cfg_cb->l3_protocol) 
            && (0xff == pstclassify->ipv4_cfg_cb->l3_protocol_mask) 
            && (0 != pstclassify->ipv4_cfg_cb->payload_mask[0])) 
        {
            pstacl->tAclRule.tAclKey.icmpType = pstclassify->ipv4_cfg_cb->payload[0];
            pstacl->tAclRule.tAclMask.icmpType = pstclassify->ipv4_cfg_cb->payload_mask[0];
        }
        if((1 == pstclassify->ipv4_cfg_cb->l3_protocol) 
            && (0xff == pstclassify->ipv4_cfg_cb->l3_protocol_mask)  
            && (0 != pstclassify->ipv4_cfg_cb->payload_mask[1])) 
        { 
            pstacl->tAclRule.tAclKey.icmpCode = pstclassify->ipv4_cfg_cb->payload[1];
            pstacl->tAclRule.tAclMask.icmpCode = pstclassify->ipv4_cfg_cb->payload_mask[1];
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
        pstacl->tAclRule.tAclKey.dip = pstclassify->ipTcp_cfg_cb->dst_ip;
        pstacl->tAclRule.tAclMask.dip = pstclassify->ipTcp_cfg_cb->dst_ip_mask;
        pstacl->tAclRule.tAclKey.sip = pstclassify->ipTcp_cfg_cb->src_ip;
        pstacl->tAclRule.tAclMask.sip = pstclassify->ipTcp_cfg_cb->src_ip_mask;
        pstacl->tAclRule.tAclKey.tos = (pstclassify->ipTcp_cfg_cb->dscp);
        pstacl->tAclRule.tAclMask.tos = (pstclassify->ipTcp_cfg_cb->dscp_mask);
        pstacl->tAclRule.tAclKey.l3Protocol = 6;
        pstacl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstacl->tAclRule.tAclKey.ethType = 0x0800;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;
        pstacl->tAclRule.tAclKey.l4SrcPort = pstclassify->ipTcp_cfg_cb->src_port;
        pstacl->tAclRule.tAclMask.l4SrcPort = pstclassify->ipTcp_cfg_cb->src_port_mask;
        pstacl->tAclRule.tAclKey.l4DstPort = pstclassify->ipTcp_cfg_cb->dst_port;
        pstacl->tAclRule.tAclMask.l4DstPort = pstclassify->ipTcp_cfg_cb->dst_port_mask; 
        OS_MEMCPY(&tcp_flag, &(pstclassify->ipTcp_cfg_cb->tcp_flag), 1);
        pstacl->tAclRule.tAclKey.tcp_flag = tcp_flag;
        pstacl->tAclRule.tAclMask.tcp_flag = pstclassify->ipTcp_cfg_cb->tcp_flag_mask;
        pstacl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_IP_UDP:
        if(NULL == pstclassify->ipUdp_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_IPV4_UDP of cid=%lu/iid=%lu param err.\n", __FUNCTION__, 
                   __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_IPV4_UDP of cid=%lu/iid=%lu param err.\n", 
                       __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        pstacl->tAclRule.tAclKey.ethType = 0x0800;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;
        pstacl->tAclRule.tAclKey.dip = pstclassify->ipUdp_cfg_cb->dst_ip;
        pstacl->tAclRule.tAclMask.dip = pstclassify->ipUdp_cfg_cb->dst_ip_mask;
        pstacl->tAclRule.tAclKey.sip = pstclassify->ipUdp_cfg_cb->src_ip;
        pstacl->tAclRule.tAclMask.sip = pstclassify->ipUdp_cfg_cb->src_ip_mask;
        pstacl->tAclRule.tAclKey.tos = (pstclassify->ipUdp_cfg_cb->dscp);
        pstacl->tAclRule.tAclMask.tos = (pstclassify->ipUdp_cfg_cb->dscp_mask);
        pstacl->tAclRule.tAclKey.l3Protocol = 0x11;
        pstacl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstacl->tAclRule.tAclKey.l4SrcPort = pstclassify->ipUdp_cfg_cb->src_port;
        pstacl->tAclRule.tAclMask.l4SrcPort = pstclassify->ipUdp_cfg_cb->src_port_mask;
        pstacl->tAclRule.tAclKey.l4DstPort = pstclassify->ipUdp_cfg_cb->dst_port;
        pstacl->tAclRule.tAclMask.l4DstPort = pstclassify->ipUdp_cfg_cb->dst_port_mask;
        pstacl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_IPV6:
        if(NULL == pstclassify->ipv6_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_IPV6_FRAME of cid=%lu/iid=%lu param err.\n", __FUNCTION__, 
                   __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_IPV6_FRAME of cid=%lu/iid=%lu param err.\n", 
                       __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        pstacl->tAclRule.tAclKey.ethType = 0x86dd;
        pstacl->tAclRule.tAclMask.ethType = 0xffff;
        OS_MEMCPY(pstacl->tAclRule.tAclKey.dipv6,pstclassify->ipv6_cfg_cb->dst_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(pstclassify->ipv6_cfg_cb->dst_ip_mask_len,
            (NBB_ULONG*)&(pstacl->tAclRule.tAclMask.dipv6));
        if (ATG_DCI_RC_OK != ret)
        {
            /* 异常退出 */
            goto EXIT_LABEL;
        }
        OS_MEMCPY(pstacl->tAclRule.tAclKey.sipv6,pstclassify->ipv6_cfg_cb->src_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(pstclassify->ipv6_cfg_cb->src_ip_mask_len,
            (NBB_ULONG*)&(pstacl->tAclRule.tAclMask.sipv6));
        if (ATG_DCI_RC_OK != ret)
        {
            /* 异常退出 */
            goto EXIT_LABEL;
        }
        pstacl->tAclRule.tAclKey.l3Protocol = pstclassify->ipv6_cfg_cb->l3_protocol;
        pstacl->tAclRule.tAclMask.l3Protocol = pstclassify->ipv6_cfg_cb->l3_protocol_mask;
        pstacl->tAclRule.tAclKey.l4SrcPort = pstclassify->ipv6_cfg_cb->src_port;
        pstacl->tAclRule.tAclMask.l4SrcPort = pstclassify->ipv6_cfg_cb->src_port_mask;
        pstacl->tAclRule.tAclKey.l4DstPort = pstclassify->ipv6_cfg_cb->dst_port;
        pstacl->tAclRule.tAclMask.l4DstPort = pstclassify->ipv6_cfg_cb->dst_port_mask;
        pstacl->tAclRule.tAclKey.tos = pstclassify->ipv6_cfg_cb->tos;
        pstacl->tAclRule.tAclMask.tos = pstclassify->ipv6_cfg_cb->tos_mask;
        pstacl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_NOIP:
        if(NULL == pstclassify->noIp_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:CLASSIFY_NONE_IP of cid=%lu/iid=%lu param err.\n", __FUNCTION__, 
                   __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:CLASSIFY_NONE_IP of cid=%lu/iid=%lu param err.\n", 
                       __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        pstacl->tAclRule.tAclKey.ethType = pstclassify->noIp_cfg_cb->etype;
        pstacl->tAclRule.tAclMask.ethType = pstclassify->noIp_cfg_cb->etype_mask;
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData6),&(pstclassify->noIp_cfg_cb->payload[0]),4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData5),&(pstclassify->noIp_cfg_cb->payload[4]),4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclKey.customerData4),&(pstclassify->noIp_cfg_cb->payload[8]),4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData6),&(pstclassify->noIp_cfg_cb->payload_mask[0]),4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData5),&(pstclassify->noIp_cfg_cb->payload_mask[4]),4);
        OS_MEMCPY(&(pstacl->tAclRule.tAclMask.customerData4),&(pstclassify->noIp_cfg_cb->payload_mask[8]),4);
        pstacl->flags |= ACL_COUNTER_ON;
        break;

        default:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:match-type=%d of cid=%lu/iid=%lu err.\n", __FUNCTION__, __LINE__, 
               pstclassify->match_type, pstclassify->classify_key.qos_classify_index, 
               pstclassify->classify_key.if_match_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:match-type=%d of cid=%lu/iid=%lu err.\n", __FUNCTION__, __LINE__, 
                   pstclassify->match_type, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
        break;
    }

    if(0 == pstaction->flow_act_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_DROP;
    }
    else if(1 == pstaction->flow_act_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_PASS;
    }
    else if(3 == pstaction->flow_act_cfg_cb->action)
    {
        /* 当动作是重定向时,重定向配置块不允许为空 */
        if(NULL == pstaction->redirect_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:redirect_cfg of bid=%lu param err.\n", __FUNCTION__, __LINE__, 
                   pstaction->action_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:redirect_cfg of bid=%lu param err.\n", __FUNCTION__, 
                       __LINE__, pstaction->action_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        if(0 == pstaction->redirect_cfg_cb->action)
        {
            pstacl->eAclAction = ACL_ACTION_PASS;
        }
        else if(1 == pstaction->redirect_cfg_cb->action)
        {
            pstacl->eAclAction = ACL_ACTION_VRF;
            pstacl->vrfId = pstaction->redirect_cfg_cb->vrf_d;
        }
        else if(2 == pstaction->redirect_cfg_cb->action)
        {
            if(0 == pstaction->redirect_cfg_cb->out_port_index)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:out_port_index of redirect_cfg in bid=%lu is 0, param err.\n", 
                       __FUNCTION__, __LINE__, pstaction->action_key);
                OS_SPRINTF(ucmessage,"***err***%s,%d:out_port_index of redirect_cfg in bid=%lu is 0, "
                           "param err.\n", __FUNCTION__, __LINE__, pstaction->action_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ret = spm_get_portid_from_logical_port_index(pstaction->redirect_cfg_cb->out_port_index, &stportinfo);
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index err.\n", 
                       __FUNCTION__, __LINE__, pstaction->redirect_cfg_cb->out_port_index);
                OS_SPRINTF(ucmessage,"***err***%s,%d:intf-log=%lu spm_get_portid_from_logical_port_index"
                           " err.\n", __FUNCTION__, __LINE__, pstaction->redirect_cfg_cb->out_port_index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            pstacl->slot = stportinfo.slot_id;
            pstacl->cardPort = stportinfo.port_id; 
            pstacl->eAclAction = ACL_ACTION_REDIRECTION;
        }
    }
    else if(4 == pstaction->flow_act_cfg_cb->action)
    {
        pstacl->eAclAction = ACL_ACTION_NHI;
        if(NULL == pstaction->policy_nhi_cfg_cb)
        {   
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:policy_nhi_cfg of bid=%lu param err.\n", __FUNCTION__, __LINE__, 
                   pstaction->action_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:policy_nhi_cfg of bid=%lu param err.\n", __FUNCTION__, 
                       __LINE__, pstaction->action_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        for(i = 0;i < pstaction->policy_nhi_cfg_cb->nxhop_num;i++)
        {
            if(0 == pstaction->policy_nhi_cfg_cb->nxhp[i].ip_type)
            {
                ret = spm_l3_lookupuni(0, &(pstaction->policy_nhi_cfg_cb->nxhp[i].nxhop_ip[3]),
                                 pstaction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index, &nhi);
            }
            else
            {
                ret = spm_l3_lookupuni(1, &(pstaction->policy_nhi_cfg_cb->nxhp[i].nxhop_ip[0]),
                                 pstaction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index, &nhi);
            }
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:nhi-key=%lu spm_l3_lookupuni err.\n", __FUNCTION__, __LINE__, 
                       pstaction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index);
                OS_SPRINTF(ucmessage,"***err***%s,%d:nhi-key=%lu spm_l3_lookupuni err.\n", __FUNCTION__, 
                           __LINE__, pstaction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            pstacl->nhIdx = nhi;
        }
    }

    /* 配置动作 */
    if(NULL != pstaction->supervise_cfg_cb)
    {
        pstacl->meterIdx = pstaction->meter_id;
        pstacl->flags   |= ACL_METER_ON;
        if(1 == pstaction->supervise_cfg_cb->cm_color)
        {
            pstacl->flags |= ACL_COLOR_AWARE;
        }
        if(2 == pstaction->supervise_cfg_cb->green_action.action)
        {
            pstacl->greenPhb = pstaction->supervise_cfg_cb->green_action.markphb;
            pstacl->flags |= ACL_GREEN_PHB;
        }
        if(2 == pstaction->supervise_cfg_cb->yellow_action.action)
        {
            pstacl->yellowPhb = pstaction->supervise_cfg_cb->yellow_action.markphb;
            pstacl->flags |= ACL_YELLOW_PHB;
        }
        if(2 == pstaction->supervise_cfg_cb->red_action.action)
        {
            pstacl->redPhb = pstaction->supervise_cfg_cb->red_action.markphb;
            pstacl->flags |= ACL_RED_PHB;
        }
        else if(1 == pstaction->supervise_cfg_cb->red_action.action)
        {
            pstacl->flags |= ACL_DROP_RED;
        }
    }

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstacl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pstacl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
        else
        {
            /* 写入成功则返回该条acl rule规则所对应的posid */
            *posid = pstacl->posId;
        }
#endif
    }

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


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
NBB_LONG spm_set_policy_basic_cfg(SPM_QOS_POLICY_CB *pstcb, NBB_ULONG oper, 
    ATG_DCI_QOS_POLICY_BASIC_DATA *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(ATG_DCI_OPER_ADD != oper)
    {
        goto EXIT_LABEL;
    }
    else
    {
        if((NULL == pstcb) || (NULL == buf))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 新增配置 */
        if (NULL == pstcb->basic_cfg_cb)
        {
            pstcb->basic_cfg_cb = (ATG_DCI_QOS_POLICY_BASIC_DATA *)NBB_MM_ALLOC(    \
                                               sizeof(ATG_DCI_QOS_POLICY_BASIC_DATA), 
                                           NBB_NORETRY_ACT, MEM_SPM_POLICY_BASIC_CB);
            if(NULL == pstcb->basic_cfg_cb)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                goto EXIT_LABEL;
            }
        }
        OS_MEMCPY(pstcb->basic_cfg_cb, buf, sizeof(ATG_DCI_QOS_POLICY_BASIC_DATA));
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_create_c2b_acl
   功能描述  : 处理一个cb对中的信息并写入驱动中
   输入参数  : policy模板的index
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
   注    意  : 允许cb对中c或b模块为空,此时仅保存policy与intf绑定.
               但是当下次新增c模块的配置时,不允许修改classify的匹配类型.
*****************************************************************************/
NBB_LONG spm_create_c2b_acl(SPM_QOS_POLICY_CB *policycb, NBB_ULONG classify_id, 
    NBB_ULONG action_id)
{
    NBB_ULONG ruleid = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_ULONG posid = 0;
    NBB_CHAR  ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_CLASSIFY_CB *pstclassify = NULL;
    SPM_QOS_ACTION_CB   *pstaction   = NULL;
    SPM_QOS_POLICY_CLASSIFY_CB *policy_classify = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == policycb) || (MAX_FLOW_CLASSIFY_PRI <= classify_id))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == policycb->basic_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:no pid=%lu basic-cfg.\n", __FUNCTION__, __LINE__, policycb->policy_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no pid=%lu basic-cfg.\n", __FUNCTION__, __LINE__, 
                       policycb->policy_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        else
        {
            /* policy中模式必须是多cb对模式 */
            if(0 == policycb->basic_cfg_cb->mode)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:pid=%lu isn't c2b-mode.\n", __FUNCTION__, __LINE__, policycb->policy_key);
                OS_SPRINTF(ucmessage,"***err***%s,%d:pid=%lu isn't c2b-mode.\n", __FUNCTION__, __LINE__, 
                           policycb->policy_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }
    }

    /* 查找behavior模板配置和classify链表中所有的classify规则配置 */
    pstaction = AVLL_FIND(g_qos_action_tree, &action_id);
    pstclassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST(g_classify_id_instance[classify_id].classify_group);
    if((NULL == pstaction) || (NULL == pstclassify))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:no bid=%lu or cid=%lu cfg.\n", __FUNCTION__, __LINE__, action_id, classify_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:no bid=%lu or cid=%lu cfg.\n", __FUNCTION__, __LINE__, action_id, 
                   classify_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pstaction->flow_act_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:bid=%lu no flow_act_cfg.\n", __FUNCTION__, __LINE__, action_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:bid=%lu no flow_act_cfg.\n", __FUNCTION__, __LINE__, 
                       action_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }  
    }

    /* 开始处理cb对中classify模板的内容 */
    for ( ; pstclassify != NULL; pstclassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(
            pstclassify->spm_classify_group_node))
    {   
        /* 申请ruleid */
        ruleid = spm_apply_policy_acl_ruleid(policycb->ruleid);
        if(0 == ruleid)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:spm_apply_policy_acl_ruleid failed.\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:spm_apply_policy_acl_ruleid failed.\n", __FUNCTION__, 
                       __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        
        /* posid是成功写入acl规则后驱动的返回值 */
        ret = spm_set_policy_classify_acl(pstclassify, pstaction, policycb->acl_id, ruleid, &posid);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_release_policy_acl_ruleid(policycb->ruleid, ruleid);
            printf("***err***%s,%d:cid=%lu/iid=%lu spm_set_policy_classify_acl failed.\n", __FUNCTION__, 
                   __LINE__, pstclassify->classify_key.qos_classify_index, 
                   pstclassify->classify_key.if_match_id);
            OS_SPRINTF(ucmessage,"***err***%s,%d:cid=%lu/iid=%lu spm_set_policy_classify_acl failed.\n", 
                       __FUNCTION__, __LINE__, pstclassify->classify_key.qos_classify_index, 
                       pstclassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        else
        {
            /* 申请内存失败,配置回滚并退出报错 */
            policy_classify = spm_alloc_policy_classify_cb(policycb->policy_key);
            if(NULL == policy_classify)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                spm_del_acl_rule_driver(policycb->acl_id, ruleid);
                spm_release_policy_acl_ruleid(policycb->ruleid, ruleid);
                printf("***err***%s,%d:spm_alloc_policy_classify_cb failed.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_policy_classify_cb failed.\n", __FUNCTION__, 
                           __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            else
            {
                /* posid是写rule规则时驱动的返回值 */
                policy_classify->posid   = posid;
                policy_classify->rule_id = ruleid;
                policy_classify->acl_id  = policycb->acl_id;
                policy_classify->behavior_id = action_id;
                rv = AVLL_INSERT(pstclassify->spm_classify_policy_tree, policy_classify->tree_node); 
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_del_c2b_acl
   功能描述  : 查找policy模板中cb对配置是否为空,然后清除驱动
               和内存中保存的与cb对有关的信息。 
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
NBB_LONG spm_del_c2b_acl(SPM_QOS_POLICY_CB *policycb, NBB_ULONG classify_id)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_CLASSIFY_CB *pstclassify = NULL;
    SPM_QOS_POLICY_CLASSIFY_CB *classifysup = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == policycb) || (MAX_FLOW_CLASSIFY_PRI <= classify_id))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* policy中不可能出现多个相同的classify */
    for(pstclassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST(   \
                       g_classify_id_instance[classify_id].classify_group); 
        pstclassify != NULL;  
        pstclassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST(pstclassify->spm_classify_group_node))
    {
        classifysup = AVLL_FIND(pstclassify->spm_classify_policy_tree, &(policycb->policy_key));
        if(NULL != classifysup)
        {
            spm_del_acl_rule_driver(policycb->acl_id, classifysup->rule_id);
            spm_release_policy_acl_ruleid(policycb->ruleid, classifysup->rule_id);
            AVLL_DELETE(pstclassify->spm_classify_policy_tree, classifysup->tree_node);
            spm_free_policy_classify_cb(classifysup);
            classifysup = NULL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;  
}


/*****************************************************************************
   函 数 名  : spm_add_policy_multi_c2b_cfg
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
NBB_LONG spm_add_policy_multi_c2b_cfg(SPM_QOS_POLICY_CB *pstcb, 
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;

   // NBB_BYTE c2btype = MATCH_ERROR; 
    SPM_QOS_FLOW_CLASSIFY_CB *flownode = NULL;
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
    else
    {
        /* 多cb对模式时,classify-id取值范围1-1999 */
        if((0 == buf->qos_classify_index) || (MAX_FLOW_CLASSIFY_PRI <= buf->qos_classify_index))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:cid <1-1999> while pid=%lu is c2b-mode.\n", __FUNCTION__, 
                   __LINE__, pstcb->policy_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:cid <1-1999> while pid=%lu is c2b-mode.\n", 
                       __FUNCTION__, __LINE__, pstcb->policy_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        } 
    }

    /* 表示之前policy中不存在c2b的配置信息,或第i颗树上无节点 */
    if(0 == pstcb->acl_id)
    {
        pstcb->acl_id = spm_apply_policy_acl_id(pstcb->match_type);
        if(0 == pstcb->acl_id)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:spm_apply_policy_acl_id failed.\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:spm_apply_policy_acl_id failed.\n", __FUNCTION__, 
                       __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 将classify对应的规则与behavior对应的meter动作一起写入C3的acl驱动中 */
        ret = spm_create_c2b_acl(pstcb, buf->qos_classify_index, buf->qos_behavior_index);
        if(ATG_DCI_RC_OK != ret)
        {
            /* 写驱动失败,进行配置回滚 */
            spm_del_c2b_acl(pstcb, buf->qos_classify_index);
            spm_release_policy_acl_id(pstcb->acl_id);
            pstcb->acl_id = 0;
            printf("***err***%s,%d:pid=%lu(cid=%lu/bid=%lu) spm_create_c2b_acl failed.\n", __FUNCTION__, 
                   __LINE__, pstcb->policy_key, buf->qos_classify_index);
            OS_SPRINTF(ucmessage,"***err***%s,%d:pid=%lu(cid=%lu/bid=%lu) spm_create_c2b_acl failed.\n", 
                       __FUNCTION__, __LINE__, pstcb->policy_key, buf->qos_classify_index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 若policy被intf引用,则写驱动成功后更新各intf上的aclid,因为此次分配的aclid可能已经改变 */
        for(flownode = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(pstcb->instance_tree); 
            flownode != NULL; flownode = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_NEXT(  \
            pstcb->instance_tree, flownode->spm_classify_id_node))
        {
            flownode->acl_id = pstcb->acl_id;
            rv = spm_add_flow_classify(pstcb, &(flownode->sub_port));
            if(ATG_DCI_RC_OK != rv)
            {
                /* 仅记录绑定失败的intf信息 */
                printf("***err***%s,%d:intf-log=%lu spm_add_flow_classify failed.\n", __FUNCTION__, 
                       __LINE__, flownode->key.index);
                OS_SPRINTF(ucmessage,"***err***%s,%d:intf-log=%lu spm_add_flow_classify failed.\n", 
                           __FUNCTION__, __LINE__, flownode->key.index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            }
        }
    }

    /* 表示之前policy中存在c2b的配置信息 */
    else
    {
        /* 将classify对应的规则与behavior对应的meter动作一起写入C3的acl驱动中 */
        ret = spm_create_c2b_acl(pstcb, buf->qos_classify_index, buf->qos_behavior_index);
        if(ATG_DCI_RC_OK != ret)
        {
            /* 写驱动失败,进行配置回滚 */
            spm_del_c2b_acl(pstcb, buf->qos_classify_index);
            printf("***err***%s,%d:pid=%lu(cid=%lu/bid=%lu) spm_create_c2b_acl failed.\n", __FUNCTION__, 
                   __LINE__, pstcb->policy_key, buf->qos_classify_index);
            OS_SPRINTF(ucmessage,"***err***%s,%d:pid=%lu(cid=%lu/bid=%lu) spm_create_c2b_acl failed.\n", 
                       __FUNCTION__, __LINE__, pstcb->policy_key, buf->qos_classify_index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_del_policy_multi_c2b_cfg
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
NBB_LONG spm_del_policy_multi_c2b_cfg(SPM_QOS_POLICY_CB *pstcb, 
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG  ret = ATG_DCI_RC_OK;
    NBB_CHAR  ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }
    else
    {
        /* 多cb对模式时,classify-id取值范围1-1999 */
        if((0 == buf->qos_classify_index) || (MAX_FLOW_CLASSIFY_PRI <= buf->qos_classify_index))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:cid <1-1999> while pid=%lu is c2b-mode.\n", __FUNCTION__, 
                   __LINE__, pstcb->policy_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:cid <1-1999> while pid=%lu is c2b-mode.\n", 
                       __FUNCTION__, __LINE__, pstcb->policy_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        } 
    }

    /* 删除cb对的acl驱动.当policy中cb对为空时释放aclid,但并不解绑intf与policy的绑定关系 */
    spm_del_c2b_acl(pstcb, buf->qos_classify_index);
    if(NULL == AVLL_FIRST(pstcb->c2b_tree))
    {
        pstcb->match_type = MATCH_ERROR;
        spm_release_policy_acl_id(pstcb->acl_id);
        pstcb->acl_id = 0;
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_update_policy_multi_c2b_mode
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
NBB_LONG spm_set_policy_multi_c2b_mode(SPM_QOS_POLICY_CB *pstcb, 
    NBB_ULONG oper, ATG_DCI_QOS_POLICY_CB2QOS_DATA *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_BYTE oldtype = MATCH_ERROR;   
    SPM_QOS_POLICY_C2B_CB *pstc2b = NULL;
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
    else
    {
        /* 多cb对模式时,classify-id取值范围1-1999 */
        if((0 == buf->qos_classify_index) || (MAX_FLOW_CLASSIFY_PRI <= buf->qos_classify_index))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:cid <1-1999> while pid=%lu is c2b-mode.\n", __FUNCTION__, 
                   __LINE__, pstcb->policy_key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:cid <1-1999> while pid=%lu is c2b-mode.\n", 
                       __FUNCTION__, __LINE__, pstcb->policy_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        } 
    }

    /* 执行删除c2b对配置的操作 */
    if(ATG_DCI_OPER_DEL == oper)
    {
        pstc2b = AVLL_FIND(pstcb->c2b_tree, buf);

        /* c2b配置为空,删除失败 */
        if(NULL == pstc2b)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:pid=%lu no c2b-cfg(bid=%lu/cid=%lu), del err.\n", __FUNCTION__, 
                   __LINE__, pstcb->policy_key, buf->qos_behavior_index, buf->qos_classify_index);
            OS_SPRINTF(ucmessage,"***err***%s,%d:pid=%lu no c2b-cfg(bid=%lu/cid=%lu), del err.\n", 
                       __FUNCTION__, __LINE__, pstcb->policy_key, buf->qos_behavior_index, 
                       buf->qos_classify_index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* c2b配置不为空,执行删除c2b配置的操作 */
        else
        {
            AVLL_DELETE(pstcb->c2b_tree, pstc2b->spm_policy_c2b_node);
            spm_free_policy_c2b_cb(pstc2b);
            spm_del_policy_multi_c2b_cfg(pstcb, buf);
        } 
    }

    /* 执行增加或更新c2b对配置的操作 */
    else
    {
        /* 未找到配置块表示新增c2b对配置;否则表示c2b对配置相同 */
        pstc2b = AVLL_FIND(pstcb->c2b_tree, buf);
        if(NULL == pstc2b)
        {
            /* 新增cb对之前判断cb对的匹配类型与policy的配置类型是否一致,不一致则退出 */
            ret = spm_set_policy_match_type(pstcb, buf, &oldtype);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;  
            }
        
            /* 申请c2b节点 */
            pstc2b = spm_alloc_c2b_cb(buf);
            if(NULL == pstc2b)
            {
                /* 内存申请失败,回滚policy的匹配类型 */
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                pstcb->match_type = oldtype;
                printf("***err***%s,%d:spm_alloc_c2b_cb failed.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_c2b_cb failed.\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            rv = AVLL_INSERT(pstcb->c2b_tree, pstc2b->spm_policy_c2b_node);
            ret = spm_add_policy_multi_c2b_cfg(pstcb, buf);
            if(ATG_DCI_RC_OK != ret)
            {
                /* 执行失败,则进行配置回滚 */
                pstcb->match_type = oldtype;
                AVLL_DELETE(pstcb->c2b_tree, pstc2b->spm_policy_c2b_node);
                spm_free_policy_c2b_cb(pstc2b);
                goto EXIT_LABEL;
            }
        }

        /* 相同的c2b配置,不处理更新操作 */
        else
        {
#if 0
            printf("%s,%d:update same cid=%lu/bid=%lu in pid=%lu.\n\n", __FUNCTION__, __LINE__, 
                   buf->qos_classify_index, buf->qos_behavior_index, pstcb->policy_key);
            OS_SPRINTF(ucmessage,"%s,%d:update same cid=%lu/bid=%lu in pid=%lu.\n\n", __FUNCTION__, 
                       __LINE__, buf->qos_classify_index, buf->qos_behavior_index, pstcb->policy_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
#endif
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_set_policy_c2b_cfg
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
NBB_LONG spm_set_policy_c2b_cfg(SPM_QOS_POLICY_CB *pstcb, NBB_ULONG oper,
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if((NULL == pstcb) || (NULL == buf))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, 
                       __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        else
        {
            if(NULL == pstcb->basic_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, 
                           __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            else
            {
                /* V3R1只支持多cb对模式 */
                if(0 == pstcb->basic_cfg_cb->mode)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    printf("***err***%s,%d:policy only support multi-cb mode.\n", __FUNCTION__, __LINE__);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:policy only support multi-cb mode.\n", 
                               __FUNCTION__, __LINE__);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
            }
        }

        ret = spm_set_policy_multi_c2b_mode(pstcb, oper, buf);
        if(ATG_DCI_RC_OK != ret)
        {
            printf("***err***%s,%d:pid=%lu(cid=%lu/bid=%lu) spm_set_policy_multi_c2b_mode failed.\n", 
                   __FUNCTION__, __LINE__, pstcb->policy_key, buf->qos_classify_index, 
                   buf->qos_behavior_index);
            OS_SPRINTF(ucmessage,"***err***%s,%d:pid=%lu(cid=%lu/bid=%lu) "
                       "spm_set_policy_multi_c2b_mode failed.\n\n", __FUNCTION__, __LINE__, 
                       pstcb->policy_key, buf->qos_classify_index, buf->qos_behavior_index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_VOID spm_rcv_dci_set_policy(ATG_DCI_SET_QOS_POLICY *pstsetips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT i    = 0;
    NBB_ULONG ulkey = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_BYTE *pucc2bdatastart     = NULL;
    SPM_QOS_POLICY_CB *pstcb      = NULL;
    NBB_BYTE *pucbasicdatastart   = NULL;
    SPM_QOS_POLICY_C2B_CB *pstc2b = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    NBB_ULONG uloperc2b   = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperbasic = ATG_DCI_OPER_NULL;
    ATG_DCI_QOS_POLICY_BASIC_DATA *pstbasicdata = NULL;
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *pstc2bdata  = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstsetips)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstsetips->return_code = ATG_DCI_RC_OK;
    uloperbasic = pstsetips->oper_basic;
    uloperc2b   = pstsetips->oper_cb2qos;
    pucbasicdatastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->basic_data);
    pucc2bdatastart   = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->cb2qos_data);
    pstbasicdata = (ATG_DCI_QOS_POLICY_BASIC_DATA *)pucbasicdatastart;
    ulkey = pstsetips->key;
    pstcb = AVLL_FIND(g_qos_policy_tree, &ulkey);
    if (TRUE == pstsetips->delete_struct)
    {
        /* 错误删除 */
        if(NULL == pstcb)
        {
            pstsetips->return_code = ATG_DCI_RC_DEL_FAILED;
            printf("***err***%s,%d:no pid=%lu cfg, del err.\n", __FUNCTION__, __LINE__, ulkey);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no pid=%lu cfg, del err.\n", __FUNCTION__, 
                       __LINE__, ulkey);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除操作 */
        else
        {
            if(NULL != AVLL_FIRST(pstcb->instance_tree))
            {
                /* 当policy被intf引用时,不允许删除 */
                pstsetips->return_code = ATG_DCI_RC_DEL_FAILED;
                printf("***err***%s,%d:pid=%lu being used, del err.\n", __FUNCTION__, __LINE__, ulkey);
                OS_SPRINTF(ucmessage,"***err***%s,%d:pid=%lu being used, del err.\n", __FUNCTION__, 
                           __LINE__, ulkey);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
        
            /* 删除驱动,释放c_supervise存储空间 */
            for(pstc2b = (SPM_QOS_POLICY_C2B_CB*)AVLL_FIRST(pstcb->c2b_tree); 
                pstc2b != NULL;
                pstc2b = (SPM_QOS_POLICY_C2B_CB*)AVLL_NEXT(pstcb->c2b_tree, pstc2b->spm_policy_c2b_node))
            {
                spm_del_c2b_acl(pstcb, pstc2b->c2b_key.qos_classify_index);
            } 

            /* 释放aclid */
            spm_release_policy_acl_id(pstcb->acl_id);
            AVLL_DELETE(g_qos_policy_tree, pstcb->spm_policy_node);
            spm_free_policy_cb(pstcb);
            pstcb = NULL;
        }
    }
    else
    {
        /* 新增操作 */
        if(NULL == pstcb)
        {
            pstcb = spm_alloc_policy_cb(ulkey);
            if (NULL == pstcb)
            {
                pstsetips->return_code = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:spm_alloc_policy_cb failed.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_policy_cb failed.\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* policy基本配置 */
            ret = spm_set_policy_basic_cfg(pstcb, uloperbasic, pstbasicdata);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_free_policy_cb(pstcb NBB_CCXT);
                pstsetips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstsetips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:spm_set_policy_basic_cfg failed.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_set_policy_basic_cfg failed.\n", __FUNCTION__, 
                           __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* 将policy配置块插入树中 */
            rv = AVLL_INSERT(g_qos_policy_tree, pstcb->spm_policy_node);
        }

        /* 更新操作 */
        else
        {
            /* policy基本配置 */
            ret = spm_set_policy_basic_cfg(pstcb, uloperbasic, pstbasicdata);
            if(ATG_DCI_RC_OK != ret)
            {
                pstsetips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstsetips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:spm_set_policy_basic_cfg failed.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_set_policy_basic_cfg failed.\n", __FUNCTION__, 
                           __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
        }

        /* 新增和更新都执行相同的C2B配置操作 */
        for(i = 0; i < pstsetips->cb2qos_num; i++)
        {
            /* 执行失败或配置为空,不能删除policy配置块,因为删除后可能会导致配置不同步 */
            pstc2bdata = (ATG_DCI_QOS_POLICY_CB2QOS_DATA *)(pucc2bdatastart + 
                          NBB_ALIGN_OFFSET(sizeof(ATG_DCI_QOS_POLICY_CB2QOS_DATA)) * i);
            if(NULL == pstc2bdata)
            {
                pstsetips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstsetips->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

            ret = spm_set_policy_c2b_cfg(pstcb, uloperc2b, pstc2bdata);
            if(ATG_DCI_RC_OK != ret)
            {
                pstsetips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstsetips->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}


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
NBB_VOID spm_qos_clear_all_policy(void)
{
    SPM_QOS_POLICY_CB   *policycb = NULL;
    SPM_QOS_POLICY_C2B_CB *pstc2b = NULL;

    for(policycb = (SPM_QOS_POLICY_CB*)AVLL_FIRST(g_qos_policy_tree); policycb != NULL;
        policycb = (SPM_QOS_POLICY_CB*)AVLL_FIRST(g_qos_policy_tree))
    {
        for(pstc2b = (SPM_QOS_POLICY_C2B_CB*)AVLL_FIRST(policycb->c2b_tree); 
            pstc2b != NULL;
            pstc2b = (SPM_QOS_POLICY_C2B_CB*)AVLL_NEXT(policycb->c2b_tree, pstc2b->spm_policy_c2b_node))
        {
            /* 删除acl驱动并释放c_supervise存储空间 */
            spm_del_c2b_acl(policycb, pstc2b->c2b_key.qos_classify_index);
        } 

        spm_release_policy_acl_id(policycb->acl_id);
        AVLL_DELETE(g_qos_policy_tree, policycb->spm_policy_node);
        spm_free_policy_cb(policycb);
        policycb = NULL;
    }

    return;   
}

#endif


#if 3

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
SPM_QOS_FLOW_CLASSIFY_CB *spm_qos_alloc_flow_classify_cb(SPM_QOS_LOGIC_INTF_KEY *pkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_FLOW_CLASSIFY_CB *pstcb = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pkey)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, 
                   __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 分配一个新的逻辑配置块qos表配置条目 */
    pstcb = (SPM_QOS_FLOW_CLASSIFY_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_FLOW_CLASSIFY_CB),
                                        NBB_NORETRY_ACT, MEM_SPM_QOS_FLOW_CLASSIFY_CB);
    if (NULL == pstcb)
    {
        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /* 初始逻辑配置块配置条目 */
    OS_MEMSET(pstcb, 0, sizeof(SPM_QOS_FLOW_CLASSIFY_CB));
    OS_MEMCPY(&(pstcb->key), pkey, sizeof(SPM_QOS_LOGIC_INTF_KEY));
    AVLL_INIT_NODE(pstcb->spm_qos_flow_classify_node);
    AVLL_INIT_NODE(pstcb->spm_classify_id_node);

    /* 异常退出 */
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
NBB_LONG spm_qos_free_flow_classify_cb(SPM_QOS_FLOW_CLASSIFY_CB *pstcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pstcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, 
                   __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_FLOW_CLASSIFY_CB), MEM_SPM_QOS_FLOW_CLASSIFY_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_QOS_FLOW_CLASSIFY_CB);
    pstcb = NULL;

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_add_flow_classify
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
NBB_LONG spm_add_flow_classify(SPM_QOS_POLICY_CB *policycb, SUB_PORT *port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == port) || (NULL == policycb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, 
                   __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

#if defined (SPU) || defined (PTN690_CES)
    ret = ApiC3SetIntfIngressAcl(port->unit, port->posid, 1, policycb->acl_id);
    if (ATG_DCI_RC_OK != ret)
    {
        printf("***err***%s,%d:ApiC3SetIntfIngressAcl intf-posid=%ld add pid=%lu(aid=%lu) failed.\n", 
               __FUNCTION__, __LINE__, port->posid, policycb->policy_key, policycb->acl_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:ApiC3SetIntfIngressAcl intf-posid=%ld add pid=%lu(aid=%lu) "
                   "failed.\n", __FUNCTION__, __LINE__, port->posid, policycb->policy_key, policycb->acl_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    } 
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_del_flow_classify(SPM_QOS_POLICY_CB *policycb, SUB_PORT *port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == port) || (NULL == policycb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

#if defined (SPU) || defined (PTN690_CES)
    ret = ApiC3SetIntfIngressAcl(port->unit, port->posid, 0, policycb->acl_id);
    if (ATG_DCI_RC_OK != ret)
    {
        printf("***err***%s,%d:ApiC3SetIntfIngressAcl intf-posid=%ld del pid=%lu(aid=%lu) failed.\n", 
               __FUNCTION__, __LINE__, port->posid, policycb->policy_key, policycb->acl_id);
        OS_SPRINTF(ucmessage,"***err***%s,%d:ApiC3SetIntfIngressAcl intf-posid=%ld del pid=%lu(aid=%lu) "
                   "failed.\n", __FUNCTION__, __LINE__, port->posid, policycb->policy_key, policycb->acl_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    } 
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
NBB_LONG spm_add_logic_flow_classify_node(NBB_ULONG key,
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *classify_qos, SUB_PORT *sub_port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *policycb  = NULL;
    SPM_QOS_POLICY_CB *policyold = NULL;
    SPM_QOS_LOGIC_INTF_KEY log_key = {0};
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_FLOW_CLASSIFY_CB *join_node = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((0 == key) || (NULL == classify_qos) || (NULL == sub_port))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 判断本槽位 */
    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }
    else
    {
        /* 判断是否存在policy配置 */
        policycb = AVLL_FIND(g_qos_policy_tree, &(classify_qos->qos_policy_index));
        if(NULL == policycb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:no pid=%lu cfg.\n", __FUNCTION__, __LINE__, classify_qos->qos_policy_index);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no pid=%lu cfg.\n", __FUNCTION__, __LINE__, 
                       classify_qos->qos_policy_index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
    }

    if(ATG_DCI_RC_OK != g_qos_policy_print)
    {
        printf("%s %s,%d log-intf=%lu,unit=%d,slot=%d,port=%d,svlan=%d,intf-posid=%ld,"
               "policy_index=%lu.\n\n", QOS_CFG_STRING, __FUNCTION__, __LINE__, key, 
               sub_port->unit, sub_port->slot, sub_port->port, classify_qos->svlan, 
               sub_port->posid, classify_qos->qos_policy_index);
        OS_SPRINTF(ucmessage,"%s %s,%d log-intf=%lu,unit=%d,slot=%d,port=%d,svlan=%d,"
                   "intf-posid=%ld,policy_index=%lu.\n\n", QOS_CFG_STRING, __FUNCTION__, 
                   __LINE__, key, sub_port->unit, sub_port->slot, sub_port->port, 
                   classify_qos->svlan, sub_port->posid, classify_qos->qos_policy_index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
    }

    log_key.svlan = classify_qos->svlan;
    log_key.cvlan = classify_qos->cvlan;
    log_key.index = key;
    join_node = AVLL_FIND(g_qos_flow_classify_tree, &log_key);
    if(NULL == join_node)
    {
        /* 删除错误:该intf上并未绑定policy,无法删除 */
        if(0 == classify_qos->qos_policy_index)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:no pid in log-intf=%lu, del failed.\n", __FUNCTION__, __LINE__, key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no pid in log-intf=%lu, del failed.\n", __FUNCTION__, 
                       __LINE__, key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 新增绑定 */
        else
        {
            /* 打印log信息 */
            printf("#####(ADD, log-intf=%ld/policyid=%lu)#####\n", key, classify_qos->qos_policy_index);
            OS_SPRINTF(ucmessage,"#####(ADD, log-intf=%ld/policyid=%lu)#####\n", key, classify_qos->qos_policy_index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);

            join_node = spm_qos_alloc_flow_classify_cb(&log_key);
            if (NULL == join_node)
            {
                ret = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:spm_qos_alloc_flow_classify_cb failed.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_qos_alloc_flow_classify_cb failed.\n", __FUNCTION__, 
                           __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            /* intf上绑定policy,若policy中没有c2b,则此时aclid为0 */
            ret = spm_add_flow_classify(policycb, sub_port);
            if(ATG_DCI_RC_OK != ret)
            {
                /* 绑定失败时回收内存 */
                spm_qos_free_flow_classify_cb(join_node);
                printf("***err***%s,%d:log-intf=%ld/pid=%lu spm_add_flow_classify failed.\n", __FUNCTION__, 
                       __LINE__, key, classify_qos->qos_policy_index);
                OS_SPRINTF(ucmessage,"***err***%s,%d:log-intf=%ld/pid=%lu spm_add_flow_classify failed.\n", 
                           __FUNCTION__, __LINE__, key, classify_qos->qos_policy_index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;  
            }
            
            OS_MEMCPY(&(join_node->sub_port), sub_port, sizeof(SUB_PORT));
            join_node->acl_id = policycb->acl_id;
            join_node->policy_index = policycb->policy_key;

            //coverity[no_effect_test]
            rv = AVLL_INSERT(policycb->instance_tree, join_node->spm_classify_id_node);
            rv = AVLL_INSERT(g_qos_flow_classify_tree, join_node->spm_qos_flow_classify_node);
        }
    }
    else
    {
        /* 删除操作 */
        if(0 == classify_qos->qos_policy_index)
        {
            /* 打印log信息 */
            printf("#####(DEL, log-intf=%ld/policyid=%lu)#####\n", key, classify_qos->qos_policy_index);
            OS_SPRINTF(ucmessage,"#####(ADD, log-intf=%ld/policyid=%lu)#####\n", key, 
                       classify_qos->qos_policy_index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);

            spm_del_flow_classify(policycb, sub_port);
            AVLL_DELETE(policycb->instance_tree, join_node->spm_classify_id_node);
            AVLL_DELETE(g_qos_flow_classify_tree, join_node->spm_qos_flow_classify_node);
            spm_qos_free_flow_classify_cb(join_node);
            join_node = NULL;
        }

        /* 更新操作 */
        else
        {
            /* 若sub_port的内容不相同则直接报错并退出.0表示相同,非0表示不相同. */
            if(0 != OS_MEMCMP(&(join_node->sub_port), sub_port, sizeof(SUB_PORT)))
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:sub_port changed, param err.\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:sub_port changed, param err.\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            else
            {
                /* 不存在老的policy配置 */
                policyold = AVLL_FIND(g_qos_policy_tree, &(join_node->policy_index));
                if(NULL == policyold)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    printf("***err***%s,%d:no pid=%lu cfg.\n", __FUNCTION__, __LINE__, 
                           join_node->policy_index);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:no pid=%lu cfg.\n", __FUNCTION__, 
                               __LINE__, join_node->policy_index);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;
                }
            }
        
            /* 绑定相同的policyid,直接过滤,不做处理 */
            if(join_node->policy_index == classify_qos->qos_policy_index)
            {
                /* 打印log信息 */
                printf("#####(UPDATE:SAME CFG, log-intf=%ld/policyid=%lu)#####\n", key, 
                       classify_qos->qos_policy_index);
                OS_SPRINTF(ucmessage,"#####(UPDATE:SAME CFG, log-intf=%ld/policyid=%lu)#####\n", 
                           key, classify_qos->qos_policy_index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            }

            /* 配置不同,更新操作 */
            else
            {
                /* 打印log信息 */
                printf("#####(UPDATE, log-intf=%ld/policyid=%lu)#####\n", key, 
                       classify_qos->qos_policy_index);
                OS_SPRINTF(ucmessage,"#####(UPDATE, log-intf=%ld/policyid=%lu)#####\n", key, 
                           classify_qos->qos_policy_index);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);

                /* intf上更新policy,若policy中没有c2b,则此时aclid为0 */
                ret = spm_add_flow_classify(policycb, sub_port);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("***err***%s,%d:log-intf=%ld/pid=%lu spm_add_flow_classify failed.\n", 
                           __FUNCTION__, __LINE__, key, classify_qos->qos_policy_index);
                    OS_SPRINTF(ucmessage,"***err***%s,%d:log-intf=%ld/pid=%lu spm_add_flow_classify"
                               " failed.\n", __FUNCTION__, __LINE__, key, classify_qos->qos_policy_index);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                    goto EXIT_LABEL;  
                }

                AVLL_DELETE(policyold->instance_tree, join_node->spm_classify_id_node);
                join_node->acl_id = policycb->acl_id;
                join_node->policy_index = policycb->policy_key;

                //coverity[no_effect_test]
                rv = AVLL_INSERT(policycb->instance_tree, join_node->spm_classify_id_node);
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_qos_clear_logic_classify
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
NBB_LONG spm_qos_clear_logic_classify(NBB_LONG logic_key)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *policycb = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_QOS_FLOW_CLASSIFY_CB *flowcb   = NULL;
    SPM_QOS_FLOW_CLASSIFY_CB *flownext = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    flowcb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(g_qos_flow_classify_tree);
    while(flowcb != NULL)
    {
        flownext = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_NEXT(     \
                    g_qos_flow_classify_tree, flowcb->spm_qos_flow_classify_node);
        if(flowcb->key.index != logic_key)
        {
            flowcb = flownext;
            continue;
        }

        policycb = AVLL_FIND(g_qos_policy_tree, &(flowcb->policy_index));
        if(NULL == policycb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:no pid=%lu cfg.\n", __FUNCTION__, __LINE__, flowcb->policy_index);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no pid=%lu cfg.\n", __FUNCTION__, __LINE__, 
                       flowcb->policy_index);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        spm_del_flow_classify(policycb, &(flowcb->sub_port));
        AVLL_DELETE(policycb->instance_tree, flowcb->spm_classify_id_node);
        AVLL_DELETE(g_qos_flow_classify_tree, flowcb->spm_qos_flow_classify_node);
        spm_qos_free_flow_classify_cb(flowcb);  
        flowcb = flownext;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;  
}


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
NBB_VOID spm_qos_clear_all_logic_classify(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_CB *policycb = NULL;
    SPM_QOS_FLOW_CLASSIFY_CB *flowcb = NULL;

    for(flowcb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(g_qos_flow_classify_tree); flowcb != NULL;
        flowcb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(g_qos_flow_classify_tree))
    {
        policycb = AVLL_FIND(g_qos_policy_tree, &(flowcb->policy_index));
        if(NULL != policycb)
        {
            spm_del_flow_classify(policycb, &(flowcb->sub_port));
            AVLL_DELETE(policycb->instance_tree, flowcb->spm_classify_id_node);
        }

        AVLL_DELETE(g_qos_flow_classify_tree, flowcb->spm_qos_flow_classify_node);
        spm_qos_free_flow_classify_cb(flowcb);
        flowcb = NULL;
    }

    return; 
}

#endif


#if 4

/*****************************************************************************
   函 数 名  : spm_qos_classify_init
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
NBB_LONG spm_qos_policy_init(void)
{
    NBB_BUF_SIZE avll_key_offset;

    /* POLICY树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_POLICY_CB, policy_key);
    AVLL_INIT_TREE(g_qos_policy_tree, compare_ulong, 
                  (NBB_USHORT)avll_key_offset, 
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_POLICY_CB, spm_policy_node));

    /* 上话复杂流分类树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, key);
    AVLL_INIT_TREE(g_qos_flow_classify_tree, spm_qos_logic_key_compare, 
                  (NBB_USHORT)avll_key_offset, 
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, spm_qos_flow_classify_node));
}


/*****************************************************************************
   函 数 名  : spm_cfg_policy_cb_verify
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
NBB_VOID spm_cfg_policy_cb_verify(void)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_POLICY_CB *policy_cb = NULL;
    SPM_QOS_POLICY_C2B_CB *c2b_cb = NULL;
    ATG_DCI_QOS_POLICY_BASIC_DATA *basic_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_policy_cb_verify");

    for (policy_cb = (SPM_QOS_POLICY_CB*)AVLL_FIRST(g_qos_policy_tree);
         policy_cb != NULL;
         policy_cb = (SPM_QOS_POLICY_CB*)AVLL_NEXT(g_qos_policy_tree,
                      policy_cb->spm_policy_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify policy_cb %p", policy_cb));
        NBB_VERIFY_MEMORY(policy_cb, sizeof(SPM_QOS_POLICY_CB), MEM_SPM_POLICY_CB);

        basic_cfg_cb = policy_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_QOS_POLICY_BASIC_DATA),
                              MEM_SPM_POLICY_BASIC_CB);
        }

        for (c2b_cb = (SPM_QOS_POLICY_C2B_CB*) AVLL_FIRST(policy_cb->c2b_tree);
             (NULL != c2b_cb);
             c2b_cb = (SPM_QOS_POLICY_C2B_CB*) AVLL_NEXT(policy_cb->c2b_tree,
                       c2b_cb->spm_policy_c2b_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify c2b_cb %p", c2b_cb));
            NBB_VERIFY_MEMORY(c2b_cb, sizeof(SPM_QOS_POLICY_C2B_CB), MEM_SPM_POLICY_C2B_CB);
        }     
    }

    NBB_TRC_EXIT();

    return;
}


/*****************************************************************************
   函 数 名  : spm_cfg_policy_cb_verify
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
NBB_VOID spm_cfg_logic_classify_cb_verify(void)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_FLOW_CLASSIFY_CB *cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_logic_classify_cb_verify");

    for (cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(g_qos_flow_classify_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_NEXT(g_qos_flow_classify_tree,
                   cfg_cb->spm_qos_flow_classify_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_FLOW_CLASSIFY_CB), MEM_SPM_QOS_FLOW_CLASSIFY_CB);
    }

    NBB_TRC_EXIT();

    return;
}

#endif
