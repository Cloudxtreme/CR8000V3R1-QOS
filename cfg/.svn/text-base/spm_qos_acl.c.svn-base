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

/* 全局打印变量acl_pri_setting */
extern NBB_BYTE acl_pri_setting;

/* 全局打印变量qos_acl_action_print */
extern NBB_BYTE qos_acl_action_print;

/* 全局打印变量qos_log_cfg_print */
extern NBB_BYTE qos_log_cfg_print;


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
NBB_ULONG spm_get_ipv6_mask(NBB_BYTE maskLen, NBB_ULONG *maskIpv6 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0; 
    NBB_BYTE m = 0;
    NBB_BYTE n = 0;
    NBB_ULONG *temp32;
    temp32 = maskIpv6;

    NBB_TRC_ENTRY(__FUNCTION__);
    
    if (maskLen > 128)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    m = maskLen / 32;
    n = maskLen % 32;
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
   函 数 名  : spm_qos_find_acl_cb
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
SPM_QOS_ACL_CB *spm_qos_find_acl_cb(ATG_DCI_ACL_KEY *key NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_ACL_CB *pcb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == key)
    {
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.qos_acl_tree, key);
    if (NULL == pcb)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return pcb;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_acl_cnt
   功能描述  : acl引用计数减1
   输入参数  : acl的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_disconnect_acl_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_ACL_CB *pcb = NULL;
    NBB_LONG flag = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        goto EXIT_LABEL;
    }
    for (pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(SHARED.g_acl_id_instance[index].acl_group);
         pcb != NULL; pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pcb->spm_acl_group_node))
    {
        flag = ATG_DCI_RC_UNSUCCESSFUL;
        if (0 == pcb->cnt)
        {
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS ACL disconnect",ATG_DCI_RC_UNSUCCESSFUL,
						   "acl group","cnt","","",index,pcb->cnt,0,0));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        (pcb->cnt)--;
    }
    if (ATG_DCI_RC_OK == flag)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_connect_acl_cnt
   功能描述  : acl引用计数减1
   输入参数  : acl的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_connect_acl_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_ACL_CB *pcb = NULL;
    NBB_LONG flag = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        goto EXIT_LABEL;
    }
    for (pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(SHARED.g_acl_id_instance[index].acl_group);
         pcb != NULL; pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pcb->spm_acl_group_node))
    {
        flag = ATG_DCI_RC_UNSUCCESSFUL;
        (pcb->cnt)++;
    }
    if (ATG_DCI_RC_OK == flag)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


#if 1

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
SPM_QOS_ACL_CB *spm_alloc_acl_cb(ATG_DCI_ACL_KEY* pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_ACL_CB *pstTbl = NULL;

    //NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pkey)
    {
        goto EXIT_LABEL;
    }

    pstTbl = (SPM_QOS_ACL_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_ACL_CB),NBB_NORETRY_ACT,MEM_SPM_ACL_CB);
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_ACL_CB));
    OS_MEMCPY(&(pstTbl->acl_key),pkey,sizeof(ATG_DCI_ACL_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_acl_node);
    NBB_INIT_LQE(pstTbl->spm_acl_group_node, pstTbl);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
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
NBB_LONG spm_free_acl_cb(SPM_QOS_ACL_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != pstTbl);

    if (NULL == pstTbl)
    {
        printf(("**FATAL ERROR** SPM_QOS_ACL_CB == NULL"));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_ACL_CB), MEM_SPM_ACL_CB);

    /***************************************************************************/
    /* 释放基本配置数据块。                                                    */
    /***************************************************************************/
    if (NULL != pstTbl->basic_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->basic_cfg_cb, MEM_SPM_ACL_BASIC_CB);
        pstTbl->basic_cfg_cb = NULL;
    }
    if (NULL != pstTbl->Port_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->Port_cfg_cb, MEM_SPM_ACL_PORT_CB);
        pstTbl->Port_cfg_cb = NULL;
    }
    if (NULL != pstTbl->Eth_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->Eth_cfg_cb, MEM_SPM_ACL_ETH_CB);
        pstTbl->Eth_cfg_cb = NULL;
    }
    if (NULL != pstTbl->ipv4_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv4_cfg_cb, MEM_SPM_ACL_IPV4_CB);
        pstTbl->ipv4_cfg_cb = NULL;
    }
    if (NULL != pstTbl->ipTcp_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipTcp_cfg_cb, MEM_SPM_ACL_IPTCP_CB);
        pstTbl->ipTcp_cfg_cb = NULL;
    }
    if (NULL != pstTbl->ipv6_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv6_cfg_cb, MEM_SPM_ACL_IPV6_CB);
        pstTbl->ipv6_cfg_cb = NULL;
    }
    if (NULL != pstTbl->noIp_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->noIp_cfg_cb, MEM_SPM_ACL_NOIP_CB);
        pstTbl->noIp_cfg_cb = NULL;
    }
    if (NULL != pstTbl->ipUdp_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipUdp_cfg_cb, MEM_SPM_ACL_IPUDP_CB);
        pstTbl->ipUdp_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_ACL_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 2

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
NBB_LONG spm_check_acl_id_cfg(NBB_LONG acl_id,NBB_BYTE match_type NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    
    NBB_TRC_ENTRY(__FUNCTION__);

#if 0
    /* 匹配类型为空,表示该acl_id是新添加的 */
    if(MATCH_ERROR == SHARED.g_acl_id_instance[acl_id].match_type)
    {
        switch(match_type)
        {
           case MATCH_PORT:
           SHARED.g_acl_id_instance[acl_id].match_type = MATCH_PORT;
           break;  

           case MATCH_ETH_HEADER:
           SHARED.g_acl_id_instance[acl_id].match_type = MATCH_ETH_HEADER;
           break;

           case MATCH_IPV4:

           case MATCH_IP_TCP:

           case MATCH_IP_UDP:

           case MATCH_IPV6:

           case MATCH_NOIP:
           SHARED.g_acl_id_instance[acl_id].match_type = MATCH_IP_TCP;
           break;

           default:
           {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                NBB_TRC_FLOW((NBB_FORMAT "%s line=%d spm_check_acl_id_cfg err : match_type error. "
                              "ret = %d, acl_id = %d, match_type = %d\n\n",__FUNCTION__,
                              __LINE__,ret,acl_id,match_type));
                printf("%s line=%d spm_check_acl_id_cfg err : match_type error. "
                          "ret = %d, acl_id = %d, match_type = %d\n\n",__FUNCTION__,
                          __LINE__,ret,acl_id,match_type);
                OS_SPRINTF(ucMessage,"%s line=%d spm_check_acl_id_cfg err : match_type error. "
                           "ret = %d, acl_id = %d, match_type = %d\n\n",__FUNCTION__,
                           __LINE__,ret,acl_id,match_type);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            			       QOS_ERROR_STRING,ret,
            			       ucMessage,__FUNCTION__,"LINE","",
            			       0,0,__LINE__,0)); 
                goto EXIT_LABEL;
           }
        }
    }
    else if(MATCH_ETH_HEADER == SHARED.g_acl_id_instance[acl_id].match_type)
    {
        if(MATCH_ETH_HEADER != match_type)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            NBB_TRC_FLOW((NBB_FORMAT "%s line=%d spm_check_acl_id_cfg err : match_type "
                          "isn't MATCH_ETH_HEADER. ret = %d, acl_id = %d, match_type = %d\n\n",
                          __FUNCTION__,__LINE__,ret,acl_id,match_type));
            printf("%s line=%d spm_check_acl_id_cfg err : match_type "
                      "isn't MATCH_ETH_HEADER. ret = %d, acl_id = %d, match_type = %d\n\n",
                      __FUNCTION__,__LINE__,ret,acl_id,match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_check_acl_id_cfg err : match_type "
                       "isn't MATCH_ETH_HEADER. ret = %d, acl_id = %d, match_type = %d\n\n",
                       __FUNCTION__,__LINE__,ret,acl_id,match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
    }
    else if(MATCH_IP_TCP == SHARED.g_acl_id_instance[acl_id].match_type)
    {
        if(MATCH_ETH_HEADER >= match_type)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            NBB_TRC_FLOW((NBB_FORMAT "%s line=%d spm_check_acl_id_cfg err : match_type "
                          "isn't MATCH_IP_HIGH. ret = %d, acl_id = %d, match_type = %d\n\n",
                          __FUNCTION__,__LINE__,ret,acl_id,match_type));
            printf("%s line=%d spm_check_acl_id_cfg err : match_type "
                      "isn't MATCH_IP_HIGH. ret = %d, acl_id = %d, match_type = %d\n\n",
                      __FUNCTION__,__LINE__,ret,acl_id,match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_check_acl_id_cfg err : match_type "
                       "isn't MATCH_IP_HIGH. ret = %d, acl_id = %d, match_type = %d\n\n",
                       __FUNCTION__,__LINE__,ret,acl_id,match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
    }
    else if(MATCH_PORT == SHARED.g_acl_id_instance[acl_id].match_type)
    {
        if(MATCH_PORT != match_type)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            NBB_TRC_FLOW((NBB_FORMAT "%s line=%d spm_check_acl_id_cfg err : match_type "
                          "isn't MATCH_PORT. ret = %d, acl_id = %d, match_type = %d\n\n",
                          __FUNCTION__,__LINE__,ret,acl_id,match_type));
            printf("%s line=%d spm_check_acl_id_cfg err : match_type "
                      "isn't MATCH_PORT. ret = %d, acl_id = %d, match_type = %d\n\n",
                      __FUNCTION__,__LINE__,ret,acl_id,match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_check_acl_id_cfg err : match_type "
                       "isn't MATCH_PORT. ret = %d, acl_id = %d, match_type = %d\n\n",
                       __FUNCTION__,__LINE__,ret,acl_id,match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
    }
#endif

    if(MATCH_PORT == match_type)
    {
        if((acl_id < 1) || (acl_id > 900))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_check_acl_id_cfg err : acl_id out of range. "
                      "ret = %ld, acl_id = %ld, match_type = %d\n\n",__FUNCTION__,__LINE__,
                      ret,acl_id,match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_check_acl_id_cfg err : acl_id out of range. "
                       "ret = %ld, acl_id = %ld, match_type = %d\n\n",__FUNCTION__,__LINE__,
                       ret,acl_id,match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
    }
    else if(MATCH_ETH_HEADER == match_type)
    {
        if((acl_id < 1000) || (acl_id > 1900))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_check_acl_id_cfg err : acl_id out of range. "
                      "ret = %ld, acl_id = %ld, match_type = %d\n\n",__FUNCTION__,__LINE__,
                      ret,acl_id,match_type);
            OS_SPRINTF(ucMessage,"%s line=%d spm_check_acl_id_cfg err : acl_id out of range. "
                       "ret = %ld, acl_id = %ld, match_type = %d\n\n",__FUNCTION__,__LINE__,
                       ret,acl_id,match_type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
    }

#if 0
    else if(MATCH_IPV4 == match_type)
    {
        if((acl_id < 2000) ||(acl_id > 2999))
        {
            printf("QOS spm_check_acl_id=%d,match_type=%d\n",acl_id,match_type);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS spm_check_acl_id",ATG_DCI_RC_UNSUCCESSFUL,
						"acl_id","MATCH_IPV4","","",
						acl_id,match_type,0,0));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }
    else if((MATCH_IP_TCP == match_type)||
            (MATCH_IP_UDP == match_type)||
            (MATCH_IPV6 == match_type))
    {
        if((acl_id < 3000) ||(acl_id >=4900))
        {
            printf("QOS spm_check_acl_id=%d,match_type=%d\n",acl_id,match_type);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS spm_check_acl_id",ATG_DCI_RC_UNSUCCESSFUL,
						"acl_id","MATCH_IPv6","","",
						acl_id,match_type,0,0));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }
    else if(MATCH_NOIP == match_type)
    {
        if((acl_id < 4000) ||(acl_id > 4999))
        {
            printf("QOS spm_check_acl_id=%d,match_type=%d\n",acl_id,match_type);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS spm_check_acl_id",ATG_DCI_RC_UNSUCCESSFUL,
						"acl_id","MATCH_NOIP","","",
						acl_id,match_type,0,0));
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }
    else
    {
        printf("QOS spm_check_acl_id match_type=%d ERROR\n",match_type);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					"QOS spm_check_acl_id",ATG_DCI_RC_UNSUCCESSFUL,
					"acl_id","MATCH_TYPE ERROR","","",
					acl_id,match_type,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


 /*****************************************************************************
   函 数 名  : spm_alloc_acl_cb
   功能描述  : ACL匹配端口的绑定关系建立与删除
   输入参数  : ATG_DCI_RC_UNSUCCESSFUL/ATG_DCI_RC_OK 
                                  增加/删除
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_port_acl(NBB_ULONG uiPortIndex,NBB_LONG action,ACL_T *pstAcl NBB_CCXT_T NBB_CXT)
{
	NBB_INT ret = ATG_DCI_RC_OK;
	SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
	SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;
	NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    NBB_BYTE unit = 0;

	NBB_TRC_ENTRY(__FUNCTION__);

    pstLogicalPort = AVLL_FIND(v_spm_shared->logical_port_tree, &uiPortIndex);
    if ((pstLogicalPort == NULL) || (NULL == pstAcl))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 非本槽位直接跳出 */
    if(pstLogicalPort->slot_id != SHARED.local_slot_id)
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    for(pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);pstTerminalIfCb != NULL;
        pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(             \
                           pstLogicalPort->terminal_if_tree,pstTerminalIfCb->spm_terminal_if_node))
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetIntfIngressAcl(unit,pstTerminalIfCb->intf_pos_id,action,pstAcl->mAclId);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("QOS AclId=%d,Rule_id=%d ApiC3SetIntfIngressAcl ERROR=%d\n",
                        pstAcl->mAclId,pstAcl->mRuleId,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        					   "QOS ApiC3SetIntfIngressAcl",ret,
        					   "acl_id","Rule_id","unit","intf_pos_id",
        					   pstAcl->mAclId,pstAcl->mRuleId,unit,
        					   pstTerminalIfCb->intf_pos_id));
                goto EXIT_LABEL;
            }
#endif
		}
	}
	if(ucIfExist == ATG_DCI_UNEXIST)
	{
		ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS EEROR**uiPortIndex=%lu posid is not find\n",uiPortIndex);
		goto EXIT_LABEL;
	}

	EXIT_LABEL : NBB_TRC_EXIT();
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
NBB_LONG spm_del_acl_driver(ATG_DCI_ACL_KEY ulkey,SPM_QOS_ACL_CB *pstCb NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

	NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_del_acl_driver err : SPM_QOS_ACL_CB *pstCb is NULL. "
                  "ret = %d, acl_id = %lu, rule_id = %lu\n\n",__FUNCTION__,__LINE__,ret,
                  ulkey.acl_id,ulkey.rule_id);
        OS_SPRINTF(ucMessage,"%s line=%d spm_del_acl_driver err : SPM_QOS_ACL_CB *pstCb is NULL. "
                   "ret = %d, acl_id = %lu, rule_id = %lu\n\n",__FUNCTION__,__LINE__,ret,
                   ulkey.acl_id,ulkey.rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }
    acl.mAclId = ulkey.acl_id;
    acl.mRuleId = ulkey.rule_id; 
    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3DelAcl(unit, &acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
            goto EXIT_LABEL;
        }
#endif
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
NBB_LONG spm_set_acl_port_cfg(NBB_ULONG oper,ATG_DCI_ACL_PORT_DATA *ptemp,
        ACL_T *pstAcl,SPM_QOS_ACL_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    //SPM_PORT_INFO_CB sub_port = {0};
    //ACL_T acl = {0};
    //NBB_USHORT pri = 0;
    NBB_BYTE unit = 0;
    SPM_PORT_INFO_CB pstPortInfo = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == ptemp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_acl_port_cfg err : ATG_DCI_ACL_PORT_DATA "
                      "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_acl_port_cfg err : ATG_DCI_ACL_PORT_DATA "
                       "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
             goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            OS_MEMSET(&pstPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
            ret = spm_get_portid_from_logical_port_index(ptemp->index, &pstPortInfo NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                spm_get_portid_from_logical_port_index_error_log(ptemp->index,
                        __FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }

            /* VE口 */
            if((0 == pstPortInfo.slot_id) && (3 == pstPortInfo.port_type))
            {
                /* 二层VE口 */
                if(0 == pstPortInfo.svlan)
                {
                   pstAcl->tAclRule.tAclKey.ePortType = ACL_L2VE;
                   pstAcl->tAclRule.tAclKey.port = pstPortInfo.port_id;
                   pstAcl->tAclRule.tAclMask.ePortType = 0xffff;
                   pstAcl->tAclRule.tAclMask.port      = 0xffff;
                }

                /* 三层VE口 */
                else
                {
                   pstAcl->tAclRule.tAclKey.ePortType = ACL_L3VE;
                   pstAcl->tAclRule.tAclKey.port = pstPortInfo.port_id;
                   pstAcl->tAclRule.tAclMask.ePortType = 0xffff;
                   pstAcl->tAclRule.tAclMask.port      = 0xffff;
                }
            }

            /* 普通logic或lag口 */
            else
            {
                if(0 != pstPortInfo.svlan)
                {
                    pstAcl->tAclRule.tAclKey.oVid  = pstPortInfo.svlan;
                    pstAcl->tAclRule.tAclMask.oVid = 0x0fff;
                }
                if(0 != pstPortInfo.cvlan)
                {
                    pstAcl->tAclRule.tAclKey.iVid  = pstPortInfo.cvlan;
                    pstAcl->tAclRule.tAclMask.iVid = 0x0fff;
                }
                pstAcl->tAclRule.tAclKey.ePortType  = ACL_PORT;
                pstAcl->tAclRule.tAclKey.port       = pstPortInfo.port_id;
                pstAcl->tAclRule.tAclMask.ePortType = 0xffff;
                pstAcl->tAclRule.tAclMask.port      = 0xffff;    
#ifdef R8000
                if(0 != pstPortInfo.slot_id)
                {
                    pstAcl->tAclRule.tAclKey.slot       = pstPortInfo.slot_id;
                    pstAcl->tAclRule.tAclMask.slot      = 0xffff;
                } 
#endif
            }

            /* 打印出acl模板的动作 */
            if(0 != qos_acl_action_print)
            {
                spm_api_c3_set_acl_action_log(pstAcl,pcb,__FUNCTION__,__LINE__ NBB_CCXT);
            }
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetAcl(unit, pstAcl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_set_acl_error_log(unit, pstAcl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
#endif
            }
#if 0
            ret = spm_set_port_acl(ptemp->index, ATG_DCI_RC_UNSUCCESSFUL, pstAcl NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                for(unit = 0;unit < SHARED.c3_num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    //coverity[dead_error_condition]
                    ApiC3DelAcl(unit, pstAcl);
#endif
                }
                goto EXIT_LABEL;
            }
#endif
            if (NULL == pcb->Port_cfg_cb)
            {
                pcb->Port_cfg_cb = (ATG_DCI_ACL_PORT_DATA*)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_PORT_DATA),
                                    NBB_NORETRY_ACT, MEM_SPM_ACL_PORT_CB);
                if(NULL == pcb->Port_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->Port_cfg_cb,ptemp,sizeof(ATG_DCI_ACL_PORT_DATA));         
            pcb->match_tpye = MATCH_PORT;
        }
        break;

        default:
        break;
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


#ifdef R8000_V3R2
/*****************************************************************************
   函 数 名  : spm_set_acl_portvlan_cfg
   功能描述  : 基于 PORT+VLAN的ACL
   输入参数  : 
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2015年8月1日 星期二
   作    者  : tianf
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_acl_portvlan_cfg(NBB_ULONG oper, ATG_DCI_ACL_PORT_VLAN *ptemp,
    ACL_T *pstAcl,SPM_QOS_ACL_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:
            
        /* 消息内容为空 */
        if ((NULL == ptemp) || (NULL == pstAcl) || (NULL == pcb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            goto EXIT_LABEL;
        }
        
        /* 消息内容不为空 */
        else
        {

            pstAcl->tAclRule.tAclKey.oVid = ptemp->svlan_id;
            pstAcl->tAclRule.tAclKey.iVid = ptemp->cvlan_id;
            
            pstAcl->tAclRule.tAclMask.oVid = ptemp->svlan_mask;
            pstAcl->tAclRule.tAclMask.iVid = ptemp->cvlan_mask;

            /* 打印出acl模板的动作 */
            if(0 != qos_acl_action_print)
            {
                spm_api_c3_set_acl_action_log(pstAcl,pcb,__FUNCTION__,__LINE__ NBB_CCXT);
                printf("ACL_PORT_VLAN_CFG:\n");
                printf("SVLAN_ID = %d,SVLAN_MASK = 0x%x;\n",ptemp->svlan_id,ptemp->svlan_mask);
                printf("CVLAN_ID = %d,CVLAN_MASK = 0x%x.\n",ptemp->cvlan_id,ptemp->cvlan_mask);
            }
            
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                ret = ApiC3SetAcl(unit, pstAcl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_set_acl_error_log(unit, pstAcl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
#endif
            }
        
            if (NULL == pcb->portVlan_cfg_cb)
            {
                pcb->portVlan_cfg_cb = (ATG_DCI_ACL_PORT_VLAN*)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_PORT_VLAN),
                                    NBB_NORETRY_ACT, MEM_SPM_ACL_PORTVLAN_CB);
                if(NULL == pcb->portVlan_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->portVlan_cfg_cb,ptemp,sizeof(ATG_DCI_ACL_PORT_VLAN));         
            pcb->match_tpye = MATCH_PORT;
        }
        break;

        default:
        break;
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}

#endif

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
NBB_LONG spm_set_acl_eth_cfg(NBB_ULONG oper,ATG_DCI_ACL_ETH_HEADER *ptemp,
    ACL_T *pstAcl,SPM_QOS_ACL_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    
    //NBB_ULONG counter_id = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == ptemp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_acl_eth_cfg err : ATG_DCI_ACL_ETH_HEADER "
                      "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_acl_eth_cfg err : ATG_DCI_ACL_ETH_HEADER "
                       "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.smac[0]), &(ptemp->src_mac[0]), ATG_DCI_MAC_LEN);
            OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.dmac[0]), &(ptemp->dst_mac[0]), ATG_DCI_MAC_LEN);
            OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.smac[0]), &(ptemp->src_mac_mask[0]), ATG_DCI_MAC_LEN);
            OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.dmac[0]), &(ptemp->dst_mac_mask[0]), ATG_DCI_MAC_LEN);
            pstAcl->tAclRule.tAclKey.ovlan_pri = ptemp->vlan_prio;
            pstAcl->tAclRule.tAclMask.ovlan_pri = ptemp->vlan_prio_mask;
#ifdef R8000_V3R2
            pstAcl->tAclRule.tAclKey.ivlan_pri = ptemp->cvlan_prio;
            pstAcl->tAclRule.tAclMask.ivlan_pri = ptemp->cvlan_prio_mask;
#endif
            pstAcl->tAclRule.tAclKey.ethType = 0x0800;
            pstAcl->tAclRule.tAclMask.ethType = 0xffff;
            pstAcl->flags |= ACL_COUNTER_ON;

            /* 打印出acl模板的动作 */
            if(0 != qos_acl_action_print)
            {
                spm_api_c3_set_acl_action_log(pstAcl,pcb,__FUNCTION__,__LINE__ NBB_CCXT);
            }
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetAcl(unit, pstAcl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_set_acl_error_log(unit, pstAcl,__FUNCTION__,__LINE__,ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
#endif
            }

            /* 配置驱动成功存储配置 */
            if (NULL == pcb->Eth_cfg_cb)
            {
                pcb->Eth_cfg_cb = (ATG_DCI_ACL_ETH_HEADER*)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_ETH_HEADER),
                                 NBB_NORETRY_ACT, MEM_SPM_ACL_ETH_CB);
                if(NULL == pcb->Eth_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->Eth_cfg_cb,ptemp,sizeof(ATG_DCI_ACL_ETH_HEADER));         
            pcb->match_tpye = MATCH_ETH_HEADER; 
        }
        break;

        default:
        break;
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
NBB_LONG spm_set_acl_ipv4_cfg(NBB_ULONG oper,ATG_DCI_ACL_IPV4_FRAME *ptemp,
    ACL_T *pstAcl,SPM_QOS_ACL_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == ptemp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_acl_ipv4_cfg err : ATG_DCI_ACL_IPV4_FRAME "
                      "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_acl_ipv4_cfg err : ATG_DCI_ACL_IPV4_FRAME "
                       "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            if (6 == ptemp->l3_protocol)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            pstAcl->tAclRule.tAclKey.dip = ptemp->dst_ip;
            pstAcl->tAclRule.tAclMask.dip = ptemp->dst_ip_mask;
            pstAcl->tAclRule.tAclKey.sip = ptemp->src_ip;
            pstAcl->tAclRule.tAclMask.sip = ptemp->src_ip_mask;
            pstAcl->tAclRule.tAclKey.tos = (ptemp->dscp);/*主控是按照tos下发给我们的*/
            pstAcl->tAclRule.tAclMask.tos = (ptemp->dscp_mask);
            pstAcl->tAclRule.tAclKey.l3Protocol = ptemp->l3_protocol;
            pstAcl->tAclRule.tAclMask.l3Protocol = ptemp->l3_protocol_mask;
            pstAcl->tAclRule.tAclKey.ethType = 0x0800;
            pstAcl->tAclRule.tAclMask.ethType = 0xffff;
            pstAcl->tAclRule.tAclKey.ipHeaderMF = ptemp->mf;
            pstAcl->tAclRule.tAclMask.ipHeaderMF = ptemp->mf_mask;
            pstAcl->tAclRule.tAclKey.ipHeaderDF = ptemp->df;
            pstAcl->tAclRule.tAclMask.ipHeaderDF = ptemp->df_mask;
            pstAcl->tAclRule.tAclKey.l4SrcPort = ptemp->src_port;
            pstAcl->tAclRule.tAclMask.l4SrcPort = ptemp->src_port_mask;
            pstAcl->tAclRule.tAclKey.l4DstPort = ptemp->dst_port;
            pstAcl->tAclRule.tAclMask.l4DstPort = ptemp->dst_port_mask;
            pstAcl->flags |= ACL_COUNTER_ON;
            if((1 == ptemp->l3_protocol) && (0xff == ptemp->l3_protocol_mask) && (0 != ptemp->payload_mask[0]))
            {
                pstAcl->tAclRule.tAclKey.icmpType = ptemp->payload[0];
                pstAcl->tAclRule.tAclMask.icmpType = ptemp->payload_mask[0];
            }
            if((1 == ptemp->l3_protocol) && (0xff == ptemp->l3_protocol_mask) && (0 != ptemp->payload_mask[1]))
            {
                pstAcl->tAclRule.tAclKey.icmpCode = ptemp->payload[1];
                pstAcl->tAclRule.tAclMask.icmpCode = ptemp->payload_mask[1];
            }

            /* 打印出acl模板的动作 */
            if(0 != qos_acl_action_print)
            {
                spm_api_c3_set_acl_action_log(pstAcl,pcb,__FUNCTION__,__LINE__ NBB_CCXT);
            }
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetAcl(unit, pstAcl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_set_acl_error_log(unit, pstAcl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
#endif
            }

            /* 配置驱动成功存储配置 */
            if (NULL == pcb->ipv4_cfg_cb)
            {
                pcb->ipv4_cfg_cb = (ATG_DCI_ACL_IPV4_FRAME*)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_IPV4_FRAME),
                                    NBB_NORETRY_ACT, MEM_SPM_ACL_IPV4_CB);
                if(NULL == pcb->ipv4_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->ipv4_cfg_cb,ptemp,sizeof(ATG_DCI_ACL_IPV4_FRAME));
            pcb->match_tpye = MATCH_IP_TCP;
        }
        break;

        default:
        break;
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
NBB_LONG spm_set_acl_iptcp_cfg(NBB_ULONG oper,ATG_DCI_ACL_IPV4_TCP *ptemp,
    ACL_T *pstAcl,SPM_QOS_ACL_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_BYTE tcp_flag = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == ptemp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_acl_iptcp_cfg err : ATG_DCI_ACL_IPV4_TCP "
                      "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_acl_iptcp_cfg err : ATG_DCI_ACL_IPV4_TCP "
                       "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            pstAcl->tAclRule.tAclKey.dip = ptemp->dst_ip;
            pstAcl->tAclRule.tAclMask.dip = ptemp->dst_ip_mask;
            pstAcl->tAclRule.tAclKey.sip = ptemp->src_ip;
            pstAcl->tAclRule.tAclMask.sip = ptemp->src_ip_mask;
            pstAcl->tAclRule.tAclKey.tos = (ptemp->dscp);/*主控是按照tos下发给我们的*/
            pstAcl->tAclRule.tAclMask.tos = (ptemp->dscp_mask);
            pstAcl->tAclRule.tAclKey.l3Protocol = 6;
            pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
            pstAcl->tAclRule.tAclKey.ethType = 0x0800;
            pstAcl->tAclRule.tAclMask.ethType = 0xffff;
            pstAcl->tAclRule.tAclKey.l4SrcPort = ptemp->src_port;
            pstAcl->tAclRule.tAclMask.l4SrcPort = ptemp->src_port_mask;
            pstAcl->tAclRule.tAclKey.l4DstPort = ptemp->dst_port;
            pstAcl->tAclRule.tAclMask.l4DstPort = ptemp->dst_port_mask;  
            OS_MEMCPY(&tcp_flag,&(ptemp->tcp_flag),1);
            pstAcl->tAclRule.tAclKey.tcp_flag = tcp_flag;
            pstAcl->tAclRule.tAclMask.tcp_flag = ptemp->tcp_flag_mask;
            pstAcl->flags |= ACL_COUNTER_ON;

            /* 打印出acl模板的动作 */
            if(0 != qos_acl_action_print)
            {
                spm_api_c3_set_acl_action_log(pstAcl,pcb,__FUNCTION__,__LINE__ NBB_CCXT);
            }
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetAcl(unit, pstAcl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_set_acl_error_log(unit, pstAcl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
#endif
            }

            /* 配置驱动成功存储配置 */
            if (NULL == pcb->ipTcp_cfg_cb)
            {
                pcb->ipTcp_cfg_cb = (ATG_DCI_ACL_IPV4_TCP*)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_IPV4_TCP),
                                     NBB_NORETRY_ACT, MEM_SPM_ACL_IPTCP_CB);
                if(NULL == pcb->ipTcp_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->ipTcp_cfg_cb,ptemp,sizeof(ATG_DCI_ACL_IPV4_TCP));
            pcb->match_tpye = MATCH_IP_TCP;
        }
        break;

        default:
        break;
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
NBB_LONG spm_set_acl_ipv6_cfg(NBB_ULONG oper,ATG_DCI_ACL_IPV6_FRAME *ptemp,
    ACL_T *pstAcl,SPM_QOS_ACL_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == ptemp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_acl_ipv6_cfg err : ATG_DCI_ACL_IPV6_FRAME "
                      "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_acl_ipv6_cfg err : ATG_DCI_ACL_IPV6_FRAME "
                       "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
            pstAcl->tAclRule.tAclMask.ethType = 0xffff;
            OS_MEMCPY(pstAcl->tAclRule.tAclKey.dipv6, ptemp->dst_ip, ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
            ret = spm_get_ipv6_mask(ptemp->dst_ip_mask_len,
                (NBB_ULONG*)&(pstAcl->tAclRule.tAclMask.dipv6) NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            OS_MEMCPY(pstAcl->tAclRule.tAclKey.sipv6, ptemp->src_ip, ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
            ret = spm_get_ipv6_mask(ptemp->src_ip_mask_len, 
                (NBB_ULONG*)&(pstAcl->tAclRule.tAclMask.sipv6) NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            pstAcl->tAclRule.tAclKey.l3Protocol = ptemp->l3_protocol;
            pstAcl->tAclRule.tAclMask.l3Protocol = ptemp->l3_protocol_mask;
            pstAcl->tAclRule.tAclKey.l4SrcPort = ptemp->src_port;
            pstAcl->tAclRule.tAclMask.l4SrcPort = ptemp->src_port_mask;
            pstAcl->tAclRule.tAclKey.l4DstPort = ptemp->dst_port;
            pstAcl->tAclRule.tAclMask.l4DstPort = ptemp->dst_port_mask;
            pstAcl->tAclRule.tAclKey.tos = ptemp->tos;
            pstAcl->tAclRule.tAclMask.tos = ptemp->tos_mask;
            pstAcl->flags |= ACL_COUNTER_ON;

            /* 打印出acl模板的动作 */
            if(0 != qos_acl_action_print)
            {
                spm_api_c3_set_acl_action_log(pstAcl,pcb,__FUNCTION__,__LINE__ NBB_CCXT);
            }
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetAcl(unit, pstAcl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_set_acl_error_log(unit, pstAcl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
#endif
            }

            /* 配置驱动成功存储配置 */
            if (NULL == pcb->ipv6_cfg_cb)
            {
                pcb->ipv6_cfg_cb = (ATG_DCI_ACL_IPV6_FRAME*)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_IPV6_FRAME),
                                    NBB_NORETRY_ACT, MEM_SPM_ACL_IPV6_CB);
                if(NULL == pcb->ipv6_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->ipv6_cfg_cb,ptemp,sizeof(ATG_DCI_ACL_IPV6_FRAME));
            pcb->match_tpye = MATCH_IP_TCP;
        }
        break;

        default:
        break;
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
NBB_LONG spm_set_acl_noip_cfg(NBB_ULONG oper,ATG_DCI_ACL_NONE_IP *ptemp,
    ACL_T *pstAcl,SPM_QOS_ACL_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == ptemp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_acl_noip_cfg err : ATG_DCI_ACL_NONE_IP "
                      "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_acl_noip_cfg err : ATG_DCI_ACL_NONE_IP "
                       "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            pstAcl->tAclRule.tAclKey.ethType = ptemp->etype;
            pstAcl->tAclRule.tAclMask.ethType = ptemp->etype_mask;
            OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData6),&(ptemp->payload[0]),4);
            OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData5),&(ptemp->payload[4]),4);
            OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData4),&(ptemp->payload[8]),4);
            OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData6),&(ptemp->payload_mask[0]),4);
            OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData5),&(ptemp->payload_mask[4]),4);
            OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData4),&(ptemp->payload_mask[8]),4);
            pstAcl->flags |= ACL_COUNTER_ON;

            /* 打印出acl模板的动作 */
            if(0 != qos_acl_action_print)
            {
                spm_api_c3_set_acl_action_log(pstAcl,pcb,__FUNCTION__,__LINE__ NBB_CCXT);
            }
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetAcl(unit, pstAcl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_set_acl_error_log(unit, pstAcl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
#endif
            }

            /* 配置驱动成功存储配置 */
            if (NULL == pcb->noIp_cfg_cb)
            {
                pcb->noIp_cfg_cb = (ATG_DCI_ACL_NONE_IP*)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_NONE_IP),
                                    NBB_NORETRY_ACT, MEM_SPM_ACL_NOIP_CB);
                if(NULL == pcb->noIp_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->noIp_cfg_cb,ptemp,sizeof(ATG_DCI_ACL_NONE_IP));
            pcb->match_tpye = MATCH_IP_TCP;
        }
        break;

        default:
        break;
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
NBB_LONG spm_set_acl_ipudp_cfg(NBB_ULONG oper,ATG_DCI_ACL_IPV4_UDP *ptemp,
    ACL_T *pstAcl,SPM_QOS_ACL_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == ptemp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_acl_ipudp_cfg err : ATG_DCI_ACL_IPV4_UDP "
                      "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_acl_ipudp_cfg err : ATG_DCI_ACL_IPV4_UDP "
                       "*ptemp is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            pstAcl->tAclRule.tAclKey.ethType = 0x0800;
            pstAcl->tAclRule.tAclMask.ethType = 0xffff;
            pstAcl->tAclRule.tAclKey.dip = ptemp->dst_ip;
            pstAcl->tAclRule.tAclMask.dip = ptemp->dst_ip_mask;
            pstAcl->tAclRule.tAclKey.sip = ptemp->src_ip;
            pstAcl->tAclRule.tAclMask.sip = ptemp->src_ip_mask;
            pstAcl->tAclRule.tAclKey.tos = (ptemp->dscp);/*主控是按照tos下发给我们的*/
            pstAcl->tAclRule.tAclMask.tos = (ptemp->dscp_mask);
            pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
            pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
            pstAcl->tAclRule.tAclKey.l4SrcPort = ptemp->src_port;
            pstAcl->tAclRule.tAclMask.l4SrcPort = ptemp->src_port_mask;
            pstAcl->tAclRule.tAclKey.l4DstPort = ptemp->dst_port;
            pstAcl->tAclRule.tAclMask.l4DstPort = ptemp->dst_port_mask;
            pstAcl->flags |= ACL_COUNTER_ON;

            /* 打印出acl模板的动作 */
            if(0 != qos_acl_action_print)
            {
                spm_api_c3_set_acl_action_log(pstAcl,pcb,__FUNCTION__,__LINE__ NBB_CCXT);
            }
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetAcl(unit, pstAcl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_set_acl_error_log(unit, pstAcl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
#endif
            }

            /* 配置驱动成功存储配置 */
            if (NULL == pcb->ipUdp_cfg_cb)
            {
                pcb->ipUdp_cfg_cb = (ATG_DCI_ACL_IPV4_UDP*)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_IPV4_UDP),
                                     NBB_NORETRY_ACT, MEM_SPM_ACL_IPUDP_CB);
                if(NULL == pcb->ipUdp_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->ipUdp_cfg_cb,ptemp,sizeof(ATG_DCI_ACL_IPV4_UDP));
            pcb->match_tpye = MATCH_IP_TCP;
        }
        break;

        default:
        break;
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_alloc_acl_cb
   功能描述  : 检测ACL是否存在被实例化的情况
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
NBB_LONG spm_check_acl_list(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_ACL_CB *join_cb = NULL;
    NBB_BYTE flag = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    for(join_cb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST((SHARED.g_acl_id_instance[index].acl_group));
        join_cb != NULL; join_cb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(join_cb->spm_acl_group_node))
    {
        flag = ATG_DCI_RC_UNSUCCESSFUL;

        /* ACL 被实例化是不允许插入的 */
        if (ATG_DCI_RC_OK != join_cb->cnt)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }

    /* 空 */
    if (ATG_DCI_RC_OK == flag)
    {
        goto EXIT_LABEL;
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
NBB_VOID spm_rcv_dci_set_acl(ATG_DCI_SET_ACL *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    ATG_DCI_ACL_KEY ulkey = {0};
    SPM_QOS_ACL_CB *pstCb = NULL;
    ACL_T acl = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;

    //NBB_LONG ips_ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucPortDataStart = NULL;
    NBB_BYTE *pucEthDataStart = NULL;
    NBB_BYTE *pucIpv4DataStart = NULL;
    NBB_BYTE *pucIpTcpDataStart = NULL;
    NBB_BYTE *pucIpv6DataStart = NULL;
    NBB_BYTE *pucNoIpDataStart = NULL;
    NBB_BYTE *pucIpUdpStart = NULL;
#ifdef R8000
    NBB_BYTE *pucPortVlanStart = NULL;
#endif

    /* 无基本配置 */
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPort = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperEth = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpTcp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperNoIp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpUdp = ATG_DCI_OPER_NULL;
#ifdef R8000
    NBB_ULONG ulOperPortVlan = ATG_DCI_OPER_NULL;
#endif

    ATG_DCI_ACL_BASIC_DATA *pstBasicDate = NULL;
    ATG_DCI_ACL_PORT_DATA *pstPortDate = NULL;
    ATG_DCI_ACL_ETH_HEADER *pstEthDate = NULL;
    ATG_DCI_ACL_IPV4_FRAME *pstIpv4Date = NULL;
    ATG_DCI_ACL_IPV4_TCP *pstIpTcpcDate = NULL;
    ATG_DCI_ACL_IPV6_FRAME *pstIpv6Date = NULL;
    ATG_DCI_ACL_NONE_IP *pstNoIpDate = NULL;
    ATG_DCI_ACL_IPV4_UDP *pstIpUdpDate = NULL;
#ifdef R8000_V3R2
    ATG_DCI_ACL_PORT_VLAN *pstPortVlanDate = NULL;
#endif

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps || ulkey.acl_id >= MAX_ACL_CFG_NUM)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_rcv_dci_set_acl err : ATG_DCI_SET_ACL "
                  "*pstSetIps is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
        OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_acl err : ATG_DCI_SET_ACL "
                   "*pstSetIps is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 
        goto EXIT_LABEL;
    }

    pstSetIps->return_code = ATG_DCI_RC_OK;
    OS_MEMSET(&acl, 0, sizeof(ACL_T));
    OS_MEMCPY(&ulkey, &(pstSetIps->key), sizeof(ATG_DCI_ACL_KEY));
    pstCb = AVLL_FIND(SHARED.qos_acl_tree, &ulkey);
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }

    /* 获取子配置操作 */
    ulOperBasic = pstSetIps->oper_basic;
    ulOperPort = pstSetIps->oper_port;
    ulOperEth = pstSetIps->oper_eth_header;
    ulOperIpv4 = pstSetIps->oper_ipv4_frame;
    ulOperIpTcp = pstSetIps->oper_ipv4_tcp;
    ulOperIpv6 = pstSetIps->oper_ipv6_frame;
    ulOperNoIp = pstSetIps->oper_none_ip;
    ulOperIpUdp = pstSetIps->oper_ipv4_udp;
#ifdef R8000_V3R2
    ulOperPortVlan = pstSetIps->oper_port_vlan;
#endif

    /* 获取子配置的偏移地址 */
    pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->basic_data);
    pucPortDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->port_data);
    pucEthDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->eth_header_data);
    pucIpv4DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_frame_data);
    pucIpTcpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_tcp_data);
    pucIpv6DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv6_frame_data);
    pucNoIpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->none_ip_data);
    pucIpUdpStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_udp_data);
#ifdef R8000_V3R2
    pucPortVlanStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->port_vlan_data);
#endif

    pstBasicDate = (ATG_DCI_ACL_BASIC_DATA*)pucBasicDataStart;
    pstPortDate = (ATG_DCI_ACL_PORT_DATA*)pucPortDataStart;
    pstEthDate = (ATG_DCI_ACL_ETH_HEADER*)pucEthDataStart;
    pstIpv4Date = (ATG_DCI_ACL_IPV4_FRAME*)pucIpv4DataStart;
    pstIpTcpcDate = (ATG_DCI_ACL_IPV4_TCP*)pucIpTcpDataStart;
    pstIpv6Date = (ATG_DCI_ACL_IPV6_FRAME*)pucIpv6DataStart;
    pstNoIpDate = (ATG_DCI_ACL_NONE_IP*)pucNoIpDataStart;
    pstIpUdpDate = (ATG_DCI_ACL_IPV4_UDP*)pucIpUdpStart;
#ifdef R8000_V3R2
    pstPortVlanDate = (ATG_DCI_ACL_PORT_VLAN *)pucPortVlanStart;
#endif

    /* ips消息类型为删除 */
    if (TRUE == pstSetIps->delete_struct)
    {
        if (ucIfExist == QOS_UNEXIST)
        {
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            printf("%s line=%d spm_rcv_dci_set_acl del err : no acl cfg. "
                      "ret = %ld, acl_id =%lu, rule_id = %lu\n\n",__FUNCTION__,__LINE__,
                      pstSetIps->return_code,ulkey.acl_id,ulkey.rule_id);
            OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_acl del err : no acl cfg. "
                       "ret = %ld, acl_id =%lu, rule_id = %lu\n\n",__FUNCTION__,__LINE__,
                       pstSetIps->return_code,ulkey.acl_id,ulkey.rule_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }

        /* 存在,删除 */
        else
        {
            if(NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
            if(NULL != AVLL_FIRST(SHARED.g_acl_id_instance[ulkey.acl_id].instance_tree))
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                printf("%s line=%d spm_rcv_dci_set_acl del err : acl is being used. "
                          "ret = %ld, acl_id =%lu, rule_id = %lu\n\n",__FUNCTION__,__LINE__,
                          pstSetIps->return_code,ulkey.acl_id,ulkey.rule_id);
                OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_acl del err : acl is being used. "
                           "ret = %ld, acl_id =%lu, rule_id = %lu\n\n",__FUNCTION__,__LINE__,
                           pstSetIps->return_code,ulkey.acl_id,ulkey.rule_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            			       QOS_ERROR_STRING,ret,
            			       ucMessage,__FUNCTION__,"LINE","",
            			       0,0,__LINE__,0)); 
                goto EXIT_LABEL;
            }
            spm_del_acl_driver(ulkey,pstCb NBB_CCXT);
            AVLL_DELETE(SHARED.qos_acl_tree, pstCb->spm_acl_node);
            NBB_REMOVE_FROM_LIST(pstCb->spm_acl_group_node);
            ret = spm_free_acl_cb(pstCb NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }
    }

    /* 更新或增加 */
    else
    {   
        /* ACL被引用且配置已存在不能修改 */
        if ((NULL != AVLL_FIRST(SHARED.g_acl_id_instance[ulkey.acl_id].instance_tree)) && (QOS_UNEXIST != ucIfExist)) 
        {
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_rcv_dci_set_acl err : acl is being used. "
                      "ret = %ld, acl_id =%lu, rule_id = %lu\n\n",__FUNCTION__,__LINE__,
                      pstSetIps->return_code,ulkey.acl_id,ulkey.rule_id);
            OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_acl err : acl is being used. "
                       "ret = %ld, acl_id =%lu, rule_id = %lu\n\n",__FUNCTION__,__LINE__,
                       pstSetIps->return_code,ulkey.acl_id,ulkey.rule_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			       QOS_ERROR_STRING,ret,
        			       ucMessage,__FUNCTION__,"LINE","",
        			       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
        if(ATG_DCI_OPER_ADD == ulOperBasic)
        {
            if (NULL == pstBasicDate)/*消息内容为空*/
            {
                pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                printf("%s line=%d spm_rcv_dci_set_acl err : Acl basic cfg "
                          "pstBasicDate is NULL. ret = %ld, acl_id =%lu, rule_id = %lu\n\n",
                          __FUNCTION__,__LINE__,ret,ulkey.acl_id,ulkey.rule_id);
                OS_SPRINTF(ucMessage,"%s line=%d spm_rcv_dci_set_acl err : Acl basic cfg "
                           "pstBasicDate is NULL. ret = %ld, acl_id =%lu, rule_id = %lu\n\n",
                           __FUNCTION__,__LINE__,ret,ulkey.acl_id,ulkey.rule_id);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            			       QOS_ERROR_STRING,ret,
            			       ucMessage,__FUNCTION__,"LINE","",
            			       0,0,__LINE__,0)); 
                goto EXIT_LABEL;
            }
            ret = spm_check_acl_id_cfg(ulkey.acl_id,pstBasicDate->match_type NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            if (QOS_UNEXIST == ucIfExist)
            {
                /* 申请一个新条目的内存空间 */
                pstCb = spm_alloc_acl_cb(&ulkey NBB_CCXT);
                if (NULL == pstCb)
                {
                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            if((NULL != pstCb) && (NULL == pstCb->basic_cfg_cb))
            {
                pstCb->basic_cfg_cb = (ATG_DCI_ACL_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_ACL_BASIC_DATA),
                                      NBB_NORETRY_ACT, MEM_SPM_ACL_BASIC_CB);
                if(NULL == pstCb->basic_cfg_cb)
                {
                    pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_free_acl_cb(pstCb NBB_CCXT);
                    goto EXIT_LABEL;
                }
            } 
            if(NULL != pstCb)
            {
                OS_MEMCPY(pstCb->basic_cfg_cb,pstBasicDate,sizeof(ATG_DCI_ACL_BASIC_DATA));
            }
        }

        /*else
        {
            printf("***ERROR***:ulOperBasic ERROR ACL ID = %d,RULE ID = %d ",ulkey.acl_id,ulkey.rule_id);
            pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }*/
        if(NULL == pstCb)
        {
            pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*acl已存在,删除*/
        if(0 != pstCb->posid)
        {
            spm_del_acl_driver(ulkey,pstCb NBB_CCXT);
        }

        acl.mAclId = ulkey.acl_id;
        acl.mRuleId = ulkey.rule_id;

        if(0 != acl_pri_setting)/*ACL的优先级*/
        {
            acl.priority = ulkey.rule_id;
        }
        if(0 == pstCb->basic_cfg_cb->action)
        {
            acl.eAclAction = ACL_ACTION_DROP;
        }
        else if(1 == pstCb->basic_cfg_cb->action)
        {
            acl.eAclAction = ACL_ACTION_PASS;
        }
        else
        {
            pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        switch(pstCb->basic_cfg_cb->match_type) /* 调用驱动进行模板配置 */
        {
            case MATCH_PORT:
            ret = spm_set_acl_port_cfg(ulOperPort,pstPortDate,&acl,pstCb NBB_CCXT);
            pstSetIps->port_return_code = ret;
            break;

            case MATCH_ETH_HEADER:
            ret = spm_set_acl_eth_cfg(ulOperEth,pstEthDate,&acl,pstCb NBB_CCXT);
            pstSetIps->eth_header_return_code = ret;
            break;

            case MATCH_IPV4:
            ret = spm_set_acl_ipv4_cfg(ulOperIpv4,pstIpv4Date,&acl,pstCb NBB_CCXT);
            pstSetIps->ipv4_frame_return_code = ret;
            break;

            case MATCH_IP_TCP:
            ret = spm_set_acl_iptcp_cfg(ulOperIpTcp,pstIpTcpcDate,&acl,pstCb NBB_CCXT);
            pstSetIps->ipv4_tcp_return_code = ret;
            break;

            case MATCH_IP_UDP:
            ret = spm_set_acl_ipudp_cfg(ulOperIpUdp,pstIpUdpDate,&acl,pstCb NBB_CCXT);
            pstSetIps->ipv4_udp_return_code = ret;
            break;

            case MATCH_IPV6:
            ret = spm_set_acl_ipv6_cfg(ulOperIpv6,pstIpv6Date,&acl,pstCb NBB_CCXT);
            pstSetIps->ipv6_frame_return_code = ret;
            break;

            case MATCH_NOIP:
            ret = spm_set_acl_noip_cfg(ulOperNoIp, pstNoIpDate,&acl,pstCb NBB_CCXT);
            pstSetIps->none_ip_return_code = ret;
            break;
            
#ifdef R8000_V3R2
            case MATCH_PORT_VLAN:
            ret = spm_set_acl_portvlan_cfg(ulOperPortVlan,pstPortVlanDate,&acl,pstCb NBB_CCXT);
            pstSetIps->port_vlan_return_code = ret;
            break;
#endif

            default:
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("match_type=%d ERROR\n",pstCb->basic_cfg_cb->match_type);
            break;
        }
        pstSetIps->return_code = ret;
        if((ATG_DCI_RC_OK != ret) && (QOS_UNEXIST == ucIfExist))
        {
            spm_free_acl_cb(pstCb NBB_CCXT);
            goto EXIT_LABEL;
        }

        pstCb->posid = acl.posId;

        /* 如果是新增的条目,插入到树和相同acl_id的链表中 */
        if(QOS_UNEXIST == ucIfExist)
        {
            SHARED.g_acl_id_instance[ulkey.acl_id].match_type = pstCb->match_tpye;
            NBB_INSERT_BEFORE(SHARED.g_acl_id_instance[ulkey.acl_id].acl_group, pstCb->spm_acl_group_node);
            
            //coverity[no_effect_test]
            rv = AVLL_INSERT(SHARED.qos_acl_tree, pstCb->spm_acl_node);
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
NBB_VOID spm_qos_clear_all_acl(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_ACL_CB *cfg_cb = NULL;

    for(cfg_cb = (SPM_QOS_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_acl_tree);cfg_cb != NULL;
        cfg_cb = (SPM_QOS_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_acl_tree))
    {
        NBB_REMOVE_FROM_LIST(cfg_cb->spm_acl_group_node);
        AVLL_DELETE(v_spm_shared->qos_acl_tree, cfg_cb->spm_acl_node);
        spm_free_acl_cb(cfg_cb NBB_CCXT);   
    }
    return;   
}

#endif


#if 3

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
SPM_QOS_LOGIC_ACL_CB *spm_alloc_logic_acl_cb(SPM_QOS_LOGIC_ACL_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_ACL_CB *pstTbl = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pkey);

    if (NULL == pkey)
    {
        goto EXIT_LABEL;
    }
    pstTbl = (SPM_QOS_LOGIC_ACL_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_LOGIC_ACL_CB),
              NBB_NORETRY_ACT, MEM_SPM_QOS_ACL_LOGIC_CB);
    if (NULL == pstTbl)
    {
        goto EXIT_LABEL;
    }

    /* 初始逻辑配置块配置条目 */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_LOGIC_ACL_CB));
    OS_MEMCPY(&(pstTbl->key),pkey,sizeof(SPM_QOS_LOGIC_ACL_KEY));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_acl_node);
    AVLL_INIT_NODE(pstTbl->spm_acl_id_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
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
NBB_LONG spm_free_logic_acl_cb(SPM_QOS_LOGIC_ACL_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != pstTbl);

    if (NULL == pstTbl)
    {
        printf("**QOS ERROR**%s,%d,pstTbl==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS spm_free_logic_acl_cb",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_LOGIC_ACL_CB), MEM_SPM_QOS_ACL_LOGIC_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_ACL_LOGIC_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_add_logic_acl
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
NBB_LONG spm_add_logic_acl(ATG_DCI_LOG_PORT_TRAFFIC_FILTER *cfg, 
    SUB_PORT *sub_port NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    //SPM_QOS_ACL_CB *pB = NULL; 
#if defined (SPU) || defined (PTN690_CES)
    NBB_BYTE unit = 0;
#endif

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != sub_port);
    NBB_ASSERT(NULL != cfg);

    /* 参数无效 */
    if ((NULL == sub_port) || (NULL == cfg))
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS spm_del_logic_acl",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",
						0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ACL group*/
    if (0 == cfg->acl_id)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
#if defined (SPU) || defined (PTN690_CES)
    if(0 != sub_port->slot)
    {
        if(0 == cfg->direction)/*入方向*/
        {
           ret = ApiC3SetIntfIngressAcl(sub_port->unit,sub_port->posid,1,cfg->acl_id);
        }
        else if(1 == cfg->direction)
        {
           ret = ApiC3SetIntfEgressAcl(sub_port->unit,sub_port->posid,1,cfg->acl_id);      
        }
    }
    else
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
            if(0 == cfg->direction)/*入方向*/
            {
               ret += ApiC3SetIntfIngressAcl(unit,sub_port->posid,1,cfg->acl_id);
            }
            else if(1 == cfg->direction)
            {
               ret += ApiC3SetIntfEgressAcl(unit,sub_port->posid,1,cfg->acl_id);       
            }
        }
    }

    if (ATG_DCI_RC_OK != ret)
    {
        printf("**FATAL ERROR**acl id=%lu, ret =%ld ApiC3SetIntfAcl\n",cfg->acl_id, ret);

        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS set acl",ret,
						"AclId","unit","posid","direction",
						cfg->acl_id,sub_port->unit,sub_port->posid,cfg->direction));
        goto EXIT_LABEL;
    }
#endif

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
NBB_LONG spm_del_logic_acl(ATG_DCI_LOG_PORT_TRAFFIC_FILTER *cfg,
    SUB_PORT *sub_port NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    //SPM_QOS_ACL_CB *pB = NULL;
    //NBB_LONG flag = ATG_DCI_RC_OK;
#if defined (SPU) || defined (PTN690_CES)
    NBB_BYTE unit = 0;
#endif

    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != cfg);
    NBB_ASSERT(NULL != sub_port);

    if ((NULL == cfg) || (NULL == sub_port))
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS spm_del_logic_acl",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

#if defined (SPU) || defined (PTN690_CES)
    if(0 != sub_port->slot)
    {
        if(0 == cfg->direction)/*入方向*/
        {
           ret = ApiC3SetIntfIngressAcl(sub_port->unit,sub_port->posid,0,cfg->acl_id);
        }
        else if(1 == cfg->direction)
        {
           ret = ApiC3SetIntfEgressAcl(sub_port->unit,sub_port->posid,0,cfg->acl_id);      
        }
    }
    else
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
            if(0 == cfg->direction)/*入方向*/
            {
               ret += ApiC3SetIntfIngressAcl(sub_port->unit,sub_port->posid,0,cfg->acl_id);
            }
            else if(1 == cfg->direction)
            {
               ret += ApiC3SetIntfEgressAcl(sub_port->unit,sub_port->posid,0,cfg->acl_id);      
            }
        }
    }

    if (ATG_DCI_RC_OK != ret)
    {
        printf("**FATAL ERROR**acl id=%lu, ret =%ld spm_del_logic_acl\n",cfg->acl_id, ret);

        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS set acl",ret,
						"AclId","unit","posid","direction",
						cfg->acl_id,sub_port->unit,sub_port->posid,cfg->direction));
        goto EXIT_LABEL;
    }
#endif 

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
NBB_LONG spm_add_logic_acl_node(NBB_ULONG key,
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *cfg,SUB_PORT *sub_port NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_ACL_CB *join_node = NULL;
    NBB_ULONG index = 0;
    NBB_BYTE direct = 0;
    SPM_QOS_LOGIC_ACL_KEY log_key = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == sub_port) || (NULL == cfg) || (0 == key))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_log_cfg_print)
    {
        printf("%s %s,%d log key=%lu,port=%d,posid=%ld,acl_id=%lu,direction=%s\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,
                  key,sub_port->port,sub_port->posid,cfg->acl_id,
                  (cfg->direction)?"出":"入");
        OS_SPRINTF(ucMessage,"%s %s,%d log key=%lu,port=%d,posid=%ld,acl_id=%lu,direction=%s\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,
                   key,sub_port->port,sub_port->posid,cfg->acl_id,
                   (cfg->direction)?"出":"入");
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }
    index = cfg->acl_id;
    direct = cfg->direction;
    log_key.index = key;    /* 逻辑端口index */
    log_key.id = cfg->classify_id;
    log_key.svlan = cfg->svlan;
    log_key.cvlan = cfg->cvlan;
    join_node = AVLL_FIND(SHARED.qos_filter_tree,&log_key);

    /***************************************************************************/
    /* 										增加操作                            */
    /***************************************************************************/
    if ((NULL == join_node) && (0 != index))
    {
        join_node = spm_alloc_logic_acl_cb(&log_key NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_logic_acl(cfg, sub_port NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_free_logic_acl_cb(join_node NBB_CCXT);
            goto EXIT_LABEL;
        }

        OS_MEMCPY(&(join_node->filter),cfg,sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
        OS_MEMCPY(&(join_node->sub_port),sub_port,sizeof(SUB_PORT));
        
        //coverity[no_effect_test]
        rv = AVLL_INSERT(SHARED.qos_filter_tree, join_node->spm_acl_node);

		/* 绑定了acl规则的逻辑端口树,key值是逻辑端口index及VLAN等 */
        //coverity[no_effect_test]
        rv = AVLL_INSERT(SHARED.g_acl_id_instance[index].instance_tree, join_node->spm_acl_id_node);
        goto EXIT_LABEL;
    }

    /* 更新---每个逻辑端口index只允许绑定一个acl_id */
    else if ((NULL != join_node) && (0 != join_node->filter.acl_id) && (0 != index) && 
            ((join_node->filter.acl_id != index) || (join_node->filter.direction != direct)))
    {
        ret = spm_del_logic_acl(&(join_node->filter),sub_port NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_add_logic_acl(cfg, sub_port NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        /* 
           instance_tree : acl被逻辑端口所引用的引用树,一个acl可以被多个逻辑端口所引用;
                           该树中保存的是,该acl被哪些逻辑端口所引用.
        */
        if(NULL != AVLL_FIND(SHARED.g_acl_id_instance[join_node->filter.acl_id].instance_tree,
            &log_key))
        {
            AVLL_DELETE(SHARED.g_acl_id_instance[join_node->filter.acl_id].instance_tree, join_node->spm_acl_id_node);
        }
        
        //coverity[no_effect_test]
        rv = AVLL_INSERT(SHARED.g_acl_id_instance[index].instance_tree, join_node->spm_acl_id_node);
        OS_MEMCPY(&(join_node->filter),cfg,sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
        goto EXIT_LABEL;
    }

    /* 相同配置 */
    else if ((NULL != join_node) && (0 != join_node->filter.acl_id) && (0 != index) && 
            ((join_node->filter.acl_id == index) && (join_node->filter.direction == direct)))
    {
        printf("**QOS ERROR**%s,%d,the same cfg\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"logic acl the same cfg",ATG_DCI_RC_OK,
						"acl_index","direct","","",
						join_node->filter.acl_id,join_node->filter.direction,0,0));
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }

    /* 删除配置 */
    else if ((NULL != join_node) && (0 != join_node->filter.acl_id) && (0 == index))
    {
        ret = spm_del_logic_acl(&(join_node->filter),sub_port NBB_CCXT);
        AVLL_DELETE(SHARED.qos_filter_tree, join_node->spm_acl_node);
        if(NULL != AVLL_FIND(SHARED.g_acl_id_instance[join_node->filter.acl_id].instance_tree,
            &log_key))
        {
            AVLL_DELETE(SHARED.g_acl_id_instance[join_node->filter.acl_id].instance_tree, join_node->spm_acl_id_node);
        }
        spm_free_logic_acl_cb(join_node NBB_CCXT);
        goto EXIT_LABEL;
    }

    /* 错误的配置 */
    else
    {
        printf("**QOS ERROR**%s,%d,acl index config missmatch\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"logic acl config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
						"acl index",(NULL == join_node)?"no cfg direct=":"find cfg direct=",
						"logic key","classify_id",
						cfg->acl_id,direct,key,cfg->classify_id));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
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
NBB_LONG spm_qos_clear_logic_acl(NBB_LONG logic_key NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
    SPM_QOS_LOGIC_ACL_CB *next_cfg_cb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    cfg_cb = (SPM_QOS_LOGIC_ACL_CB*) AVLL_FIRST(SHARED.qos_filter_tree);
    while(cfg_cb != NULL)
    {
        next_cfg_cb = (SPM_QOS_LOGIC_ACL_CB*) AVLL_NEXT(SHARED.qos_filter_tree,cfg_cb->spm_acl_node);
        if(cfg_cb->key.index != logic_key)
        {
            cfg_cb = next_cfg_cb;
            continue;
        }

/*  
#if defined (SPU) || defined (PTN690_CES)
        if(0 == cfg_cb->filter.direction) 
        {
           ret = ApiC3SetIntfIngressAcl(cfg_cb->sub_port.unit,cfg_cb->sub_port.posid,0,cfg_cb->filter.acl_id);
        }
        else if(1 == cfg_cb->filter.direction)
        {
           ret = ApiC3SetIntfEgressAcl(cfg_cb->sub_port.unit,cfg_cb->sub_port.posid,0,cfg_cb->filter.acl_id);     
        }
        if(ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**ret=%d,%s line=%d logic_key=%d,posid=%d,acl id=%d\n",
                      ret,__FUNCTION__,__LINE__,logic_key,cfg_cb->sub_port.posid,cfg_cb->filter.acl_id);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS spm_qos_clear_logic_acl",ret,
						   "logic_key","direct","posid","acl_id",
						   logic_key,cfg_cb->filter.direction,cfg_cb->sub_port.posid,cfg_cb->filter.acl_id));
            rv = ret;
        }
#endif 
*/
        AVLL_DELETE(SHARED.qos_filter_tree, cfg_cb->spm_acl_node);
        if(NULL != AVLL_FIND(SHARED.g_acl_id_instance[cfg_cb->filter.acl_id].instance_tree,
            &(cfg_cb->key)))
        {
            AVLL_DELETE(SHARED.g_acl_id_instance[cfg_cb->filter.acl_id].instance_tree, cfg_cb->spm_acl_id_node);
        }
        spm_free_logic_acl_cb(cfg_cb NBB_CCXT);  
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
NBB_VOID spm_qos_clear_all_logic_acl(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;


    for(cfg_cb = (SPM_QOS_LOGIC_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_filter_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_LOGIC_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_filter_tree))
    {      
        AVLL_DELETE(v_spm_shared->qos_filter_tree, cfg_cb->spm_acl_node);
        if(NULL != AVLL_FIND(v_spm_shared->g_acl_id_instance[cfg_cb->filter.acl_id].instance_tree,
            &(cfg_cb->key)))
        {
            AVLL_DELETE(v_spm_shared->g_acl_id_instance[cfg_cb->filter.acl_id].instance_tree,cfg_cb->spm_acl_id_node);
        }
        spm_free_logic_acl_cb(cfg_cb NBB_CCXT);
    }
    

    return;   
}


#endif










