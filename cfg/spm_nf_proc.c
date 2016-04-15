

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
   1.日    期   : 2012年9月18日
    作    者   : zenglu
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

//#define SPU
#define TM

extern unsigned char nf_ctrl_cfg_print_setting; //#define SPU
unsigned int spm_ips_ip_nf_rcv_num = 0;    //记录配置条目数
unsigned int spm_ips_aggr_nf_rcv_num = 0;     //记录收到配置条目数


/*****************************************************************************
   函 数 名  : spm_init_ip_nf_tree
   功能描述  : nf相关树的初始化
   输入参数  :
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/

NBB_VOID spm_init_ip_nf_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_ip_nf_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_IP_NF_CB, ip_nf_key);

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.ip_nf_tree, compare_ulong, (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_IP_NF_CB, ip_nf_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   函 数 名  : spm_init_nf_aggr_tree
   功能描述  : nf相关树的初始化
   输入参数  :
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/

NBB_VOID spm_init_nf_aggr_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_nf_aggr_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_NF_AGGR_CB, nf_aggr_key);

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.nf_aggr_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_NF_AGGR_CB, nf_aggr_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   函 数 名  : spm_free_ip_nf_cb
   功能描述  : nf相关树的初始化
   输入参数  :
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/

NBB_LONG spm_free_ip_nf_cb(SPM_IP_NF_CB *pstTbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_TRC_ENTRY("spm_free_ip_nf_cb");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pstTbl);

    /*释放内存失败*/
    if (NULL == pstTbl)
    {
        NBB_TRC_FLOW((NBB_FORMAT "**FATAL ERROR** pstTbl == NULL"));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_IP_NF_CB), MEM_SPM_IP_NF_CB);

    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_IP_NF_CB at %p", pstTbl));
    NBB_TRC_FLOW((NBB_FORMAT "Free ip netflow key %d", pstTbl->ip_nf_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (NULL != pstTbl->ipv4_dst_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv4_dst_cfg_cb, MEM_SPM_IP_NF_IPV4_DSTHOST_CB);
        pstTbl->ipv4_dst_cfg_cb = NULL;
    }

    if (NULL != pstTbl->ipv4_src_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv4_src_cfg_cb, MEM_SPM_IP_NF_IPV4_SRCIP_CB);
        pstTbl->ipv4_src_cfg_cb = NULL;
    }

    if (NULL != pstTbl->ipv4_version_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv4_version_cfg_cb, MEM_SPM_IP_NF_IPV4_VERSION_CB);
        pstTbl->ipv4_version_cfg_cb = NULL;
    }

    if (NULL != pstTbl->ipv6_dst_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv6_dst_cfg_cb, MEM_SPM_IP_NF_IPV6_DSTHOST_CB);
        pstTbl->ipv6_dst_cfg_cb = NULL;
    }

    if (NULL != pstTbl->ipv6_src_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv6_src_cfg_cb, MEM_SPM_IP_NF_IPV6_SRCIP_CB);
        pstTbl->ipv6_src_cfg_cb = NULL;
    }

    if (NULL != pstTbl->ipv6_version_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->ipv6_version_cfg_cb, MEM_SPM_IP_NF_IPV6_VERSION_CB);
        pstTbl->ipv6_version_cfg_cb = NULL;
    }

    if (NULL != pstTbl->tcp_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->tcp_cfg_cb, MEM_SPM_IP_NF_CHECK_TCP_CB);
        pstTbl->tcp_cfg_cb = NULL;
    }

    if (NULL != pstTbl->timeout_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->timeout_cfg_cb, MEM_SPM_IP_NF_TIMEOUT_CB);
        pstTbl->timeout_cfg_cb = NULL;
    }

    if (NULL != pstTbl->start_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->start_cfg_cb, MEM_SPM_IP_NF_START_CB);
        pstTbl->start_cfg_cb = NULL;
    }

    if (NULL != pstTbl->info_cfg_cb)
    {
        NBB_MM_FREE(pstTbl->info_cfg_cb, MEM_SPM_IP_NF_EXPORT_INFO_CB);
        pstTbl->info_cfg_cb = NULL;
    }

    if (NULL != pstTbl->enable_cfg)
    {
        NBB_MM_FREE(pstTbl->enable_cfg, MEM_SPM_IP_NF_ENABLE_CB);
        pstTbl->enable_cfg = NULL;
    }

    if (NULL != pstTbl->flexnfkey_cfg)
    {
        NBB_MM_FREE(pstTbl->flexnfkey_cfg, MEM_SPM_IP_NF_FLEXNFKEY_CB);
        pstTbl->flexnfkey_cfg = NULL;
    }

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_IP_NF_CB);
    pstTbl = NULL;

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_free_all_ip_nf_cb
   功能描述  : 释放IP流采样配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年05月19日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_all_ip_nf_cb(NBB_CXT_T NBB_CXT)
{
	SPM_IP_NF_CB *pstIpNf = NULL;
	
    for (pstIpNf = (SPM_IP_NF_CB*) AVLL_FIRST(SHARED.ip_nf_tree);
         pstIpNf != NULL;
         pstIpNf = (SPM_IP_NF_CB*) AVLL_FIRST(SHARED.ip_nf_tree))
    {    	
		AVLL_DELETE(SHARED.ip_nf_tree, pstIpNf->ip_nf_node);     	
    	spm_free_ip_nf_cb(pstIpNf NBB_CCXT);
    }
}

/*****************************************************************************
   函 数 名  : spm_free_nf_aggr_cb
   功能描述  : nf相关树的初始化
   输入参数  :
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_free_nf_aggr_cb(SPM_NF_AGGR_CB *pstTbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_TRC_ENTRY("spm_free_nf_aggr_cb");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pstTbl);

    /*释放内存失败*/
    if (NULL == pstTbl)
    {
        NBB_TRC_FLOW((NBB_FORMAT "**FATAL ERROR** pstTbl == NULL"));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_NF_AGGR_CB), MEM_SPM_NF_AGGR_CB);

    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_NF_AGGR_CB at %p", pstTbl));
    NBB_TRC_FLOW((NBB_FORMAT "Free netflow aggr key %d", pstTbl->nf_aggr_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (NULL != pstTbl->aggregation_cfg)
    {
        NBB_MM_FREE(pstTbl->aggregation_cfg, MEM_SPM_NF_AGGR_AGGREGATION_CB);
        pstTbl->aggregation_cfg = NULL;
    }

    if (NULL != pstTbl->export_version_cfg)
    {
        NBB_MM_FREE(pstTbl->export_version_cfg, MEM_SPM_NF_AGGR_EXPORT_VERSION_CB);
        pstTbl->export_version_cfg = NULL;
    }

    if (NULL != pstTbl->ipv4mask_cfg)
    {
        NBB_MM_FREE(pstTbl->ipv4mask_cfg, MEM_SPM_NF_AGGR_IPV4_MASK_CB);
        pstTbl->ipv4mask_cfg = NULL;
    }

    if (NULL != pstTbl->ipv6mask_cfg)
    {
        NBB_MM_FREE(pstTbl->ipv6mask_cfg, MEM_SPM_NF_AGGR_IPV6_MASK_CB);
        pstTbl->ipv6mask_cfg = NULL;
    }

    if (NULL != pstTbl->enable_cfg)
    {
        NBB_MM_FREE(pstTbl->enable_cfg, MEM_SPM_NF_AGGR_ENABLE_CB);
        pstTbl->enable_cfg = NULL;
    }

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_NF_AGGR_CB);
    pstTbl = NULL;

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_free_all_nf_aggr_cb
   功能描述  : 释放流采样聚合配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年05月19日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_all_nf_aggr_cb(NBB_CXT_T NBB_CXT)
{
	SPM_NF_AGGR_CB *pstNfAggr = NULL;
	
    for (pstNfAggr = (SPM_NF_AGGR_CB*) AVLL_FIRST(SHARED.nf_aggr_tree);
         pstNfAggr != NULL;
         pstNfAggr = (SPM_NF_AGGR_CB*) AVLL_FIRST(SHARED.nf_aggr_tree))
    {    	
		AVLL_DELETE(SHARED.nf_aggr_tree, pstNfAggr->nf_aggr_node);     	
    	spm_free_nf_aggr_cb(pstNfAggr NBB_CCXT);
    }
}

/*****************************************************************************
   函 数 名  : spm_alloc_ip_nf_cb
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_IP_NF_CB *spm_alloc_ip_nf_cb()
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_IP_NF_CB *pstTbl = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_alloc_ip_nf_cb");

    /* 分配一个新的DS表配置条目。*/
    pstTbl = (SPM_IP_NF_CB *)NBB_MM_ALLOC(sizeof(SPM_IP_NF_CB),
            NBB_NORETRY_ACT, MEM_SPM_IP_NF_CB);

    /*分配内存失败*/
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始ds配置条目 */
    NBB_MEMSET(pstTbl, 0, sizeof(SPM_IP_NF_CB));
    pstTbl->ip_nf_key = 0;

    /* 建立用于该ds表配置条目的句柄，作为异步消息交换的相关器。*/

    /* 成功分配一个ds配置条目。*/

    /* NBB_TRC_DETAIL((NBB_FORMAT "SPM_QOS_ACTION_CB allocated at %p with handle %#lx",
            pstTbl, pstTbl->spm_action_handle));*/

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->ip_nf_node);

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return(pstTbl);
}

/*****************************************************************************
   函 数 名  : spm_alloc_nf_aggr_cb
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_NF_AGGR_CB *spm_alloc_nf_aggr_cb()
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_NF_AGGR_CB *pstTbl = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_alloc_ip_nf_cb");

    /* 分配一个新的DS表配置条目。*/
    pstTbl = (SPM_NF_AGGR_CB *)NBB_MM_ALLOC(sizeof(SPM_NF_AGGR_CB),
            NBB_NORETRY_ACT, MEM_SPM_NF_AGGR_CB);

    /*分配内存失败*/
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始ds配置条目 */
    NBB_MEMSET(pstTbl, 0, sizeof(SPM_NF_AGGR_CB));
    pstTbl->nf_aggr_key = 0;

    /* 建立用于该ds表配置条目的句柄，作为异步消息交换的相关器。*/

    /* 成功分配一个ds配置条目。*/

    /* NBB_TRC_DETAIL((NBB_FORMAT "SPM_QOS_ACTION_CB allocated at %p with handle %#lx",
            pstTbl, pstTbl->spm_action_handle));*/

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->nf_aggr_node);

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return(pstTbl);
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_ipv4_dst_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_ipv4_dst_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_IPV4_DSTHOST * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_IPV4_DSTHOST *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_ipv4_dst_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_IPV4_DSTHOST *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_ipv4_dst_cfg:\n");
                    NBB_PRINTF("host1_ip_type=%x !!!\n",ptemp->host1_ip_type);
                    NBB_PRINTF("host1_enable_flag=%x !!!\n",ptemp->host1_enable_flag);
                    NBB_PRINTF("host1_udp_port=%x !!!\n",ptemp->host1_udp_port);
                    NBB_PRINTF("host1_ip[0]=%x !!!\n",ptemp->host1_ip[0]);
                    NBB_PRINTF("host1_ip[1]=%x !!!\n",ptemp->host1_ip[1]);
                    NBB_PRINTF("host1_ip[2]=%x !!!\n",ptemp->host1_ip[2]);
                    NBB_PRINTF("host1_ip[3]=%x !!!\n",ptemp->host1_ip[3]);
                    NBB_PRINTF("host1_vrfid=%x !!!\n",ptemp->host1_vrfid);
                    NBB_PRINTF("host2_ip_type=%x !!!\n",ptemp->host2_ip_type);
                    NBB_PRINTF("host2_enable_flag=%x !!!\n",ptemp->host2_enable_flag);
                    NBB_PRINTF("host2_udp_port=%x !!!\n",ptemp->host2_udp_port);
                    NBB_PRINTF("host2_ip[0]=%x !!!\n",ptemp->host2_ip[0]);
                    NBB_PRINTF("host2_ip[1]=%x !!!\n",ptemp->host2_ip[1]);
                    NBB_PRINTF("host2_ip[2]=%x !!!\n",ptemp->host2_ip[2]);
                    NBB_PRINTF("host2_ip[3]=%x !!!\n",ptemp->host2_ip[3]);
                    NBB_PRINTF("host2_vrfid=%x !!!\n",ptemp->host2_vrfid);
                    NBB_PRINTF("\n");
                }   
                
                sprintf(ucMessage,"****IPV4NetFlow输出目的主机 (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_ip_nf_ipv4_dst_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip_type=%x !!!\n",ptemp->host1_ip_type);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_enable_flag=%x !!!\n",ptemp->host1_enable_flag);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_udp_port=%x !!!\n",ptemp->host1_udp_port);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip[0]=%x !!!\n",ptemp->host1_ip[0]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip[1]=%x !!!\n",ptemp->host1_ip[1]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip[2]=%x !!!\n",ptemp->host1_ip[2]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip[3]=%x !!!\n",ptemp->host1_ip[3]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_vrfid=%x !!!\n",ptemp->host1_vrfid);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip_type=%x !!!\n",ptemp->host2_ip_type);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_enable_flag=%x !!!\n",ptemp->host2_enable_flag);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_udp_port=%x !!!\n",ptemp->host2_udp_port);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip[0]=%x !!!\n",ptemp->host2_ip[0]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip[1]=%x !!!\n",ptemp->host2_ip[1]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip[2]=%x !!!\n",ptemp->host2_ip[2]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip[3]=%x !!!\n",ptemp->host2_ip[3]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_vrfid=%x !!!\n",ptemp->host2_vrfid);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_IPV4_DSTHOST *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_IPV4_DSTHOST),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_IPV4_DSTHOST_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的IPV4NetFlow输出目的主机未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:要增加的IPV4NetFlow输出目的主机未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:要增加的IPV4NetFlow输出目的主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        
                        goto EXIT_LABEL;
                    }
                }

                if(*pcb != NULL)
                {
                   //保存数据
                    NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_IPV4_DSTHOST)); 
                }

                

                //NBB_PRINTF("spm_set_ip_nf_ipv4_dst_cfg : ptemp=%x !!!\n",ptemp);
                //NBB_PRINTF("spm_set_ip_nf_ipv4_dst_cfg : *pcb=%x !!!\n",*pcb);
                //NBB_PRINTF("spm_set_ip_nf_ipv4_dst_cfg : host1_ip=%d.%d.%d.%d !!!\n",*((NBB_ULONG*)(*pcb)+1),
                //*((NBB_ULONG*)(*pcb)+2),*((NBB_ULONG*)(*pcb)+3),*((NBB_ULONG*)(*pcb)+4));

            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_ipv4_src_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_ipv4_src_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_IPV4_SRCIP * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_IPV4_SRCIP *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_ipv4_src_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
                
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_IPV4_SRCIP *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_ipv4_src_cfg:\n");
                    NBB_PRINTF("src_ipv4_enable=%x !!!\n",ptemp->src_ipv4_enable);
                    NBB_PRINTF("src_ipv6_enable=%x !!!\n",ptemp->src_ipv6_enable);
                    NBB_PRINTF("ipv4_addr=%x !!!\n",ptemp->ipv4_addr);
                    NBB_PRINTF("ipv6_addr[0]=%x !!!\n",ptemp->ipv6_addr[0]);
                    NBB_PRINTF("ipv6_addr[1]=%x !!!\n",ptemp->ipv6_addr[1]);
                    NBB_PRINTF("ipv6_addr[2]=%x !!!\n",ptemp->ipv6_addr[2]);
                    NBB_PRINTF("ipv6_addr[3]=%x !!!\n",ptemp->ipv6_addr[3]); 
                    NBB_PRINTF("\n");
                } 
                sprintf(ucMessage,"****IPV4NetFlow源IP地址配置 IPV4 SrcIP (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_ip_nf_ipv4_src_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"src_ipv4_enable=%x !!!\n",ptemp->src_ipv4_enable);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"src_ipv6_enable=%x !!!\n",ptemp->src_ipv6_enable);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv4_addr=%x !!!\n",ptemp->ipv4_addr);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv6_addr[0]=%x !!!\n",ptemp->ipv6_addr[0]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv6_addr[1]=%x !!!\n",ptemp->ipv6_addr[1]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv6_addr[2]=%x !!!\n",ptemp->ipv6_addr[2]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv6_addr[3]=%x !!!\n",ptemp->ipv6_addr[3]); 
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_IPV4_SRCIP *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_IPV4_SRCIP),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_IPV4_SRCIP_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        
                        goto EXIT_LABEL;
                    }
                }

                if(*pcb != NULL)
                {
                    //保存数据
                    NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_IPV4_SRCIP));
                }

            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_ipv4_version_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_ipv4_version_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_IPV4_VERSION * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_IPV4_VERSION *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_ipv4_version_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_IPV4_VERSION *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_ipv4_version_cfg:\n");
                    NBB_PRINTF("version=%x !!!\n",ptemp->version);
                    NBB_PRINTF("as_number=%x !!!\n",ptemp->as_number);                 
                    NBB_PRINTF("\n");
                } 
                sprintf(ucMessage,"****IPV4NetFlow输出版本 IPV4 Verson (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_ip_nf_ipv4_version_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"version=%x !!!\n",ptemp->version);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"as_number=%x !!!\n",ptemp->as_number);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
               

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_IPV4_VERSION *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_IPV4_VERSION),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_IPV4_VERSION_CB);

                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_IPV4_VERSION));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_ipv6_dst_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_ipv6_dst_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_IPV6_DSTHOST * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_IPV6_DSTHOST *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_ipv6_dst_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_IPV6_DSTHOST *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_ipv6_dst_cfg:\n");
                    NBB_PRINTF("host1_ip_type=%x !!!\n",ptemp->host1_ip_type);
                    NBB_PRINTF("host1_enable_flag=%x !!!\n",ptemp->host1_enable_flag);
                    NBB_PRINTF("host1_udp_port=%x !!!\n",ptemp->host1_udp_port);
                    NBB_PRINTF("host1_ip[0]=%x !!!\n",ptemp->host1_ip[0]);
                    NBB_PRINTF("host1_ip[1]=%x !!!\n",ptemp->host1_ip[1]);
                    NBB_PRINTF("host1_ip[2]=%x !!!\n",ptemp->host1_ip[2]);
                    NBB_PRINTF("host1_ip[3]=%x !!!\n",ptemp->host1_ip[3]);
                    NBB_PRINTF("host1_vrfid=%x !!!\n",ptemp->host1_vrfid);
                    NBB_PRINTF("host2_ip_type=%x !!!\n",ptemp->host2_ip_type);
                    NBB_PRINTF("host2_enable_flag=%x !!!\n",ptemp->host2_enable_flag);
                    NBB_PRINTF("host2_udp_port=%x !!!\n",ptemp->host2_udp_port);
                    NBB_PRINTF("host2_ip[0]=%x !!!\n",ptemp->host2_ip[0]);
                    NBB_PRINTF("host2_ip[1]=%x !!!\n",ptemp->host2_ip[1]);
                    NBB_PRINTF("host2_ip[2]=%x !!!\n",ptemp->host2_ip[2]);
                    NBB_PRINTF("host2_ip[3]=%x !!!\n",ptemp->host2_ip[3]);
                    NBB_PRINTF("host2_vrfid=%x !!!\n",ptemp->host2_vrfid);
                    NBB_PRINTF("\n");
                }
                sprintf(ucMessage,"****IPV6输出目的主机 IPV6 DstHost (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_ip_nf_ipv6_dst_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip_type=%x !!!\n",ptemp->host1_ip_type);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_enable_flag=%x !!!\n",ptemp->host1_enable_flag);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_udp_port=%x !!!\n",ptemp->host1_udp_port);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip[0]=%x !!!\n",ptemp->host1_ip[0]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip[1]=%x !!!\n",ptemp->host1_ip[1]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip[2]=%x !!!\n",ptemp->host1_ip[2]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_ip[3]=%x !!!\n",ptemp->host1_ip[3]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host1_vrfid=%x !!!\n",ptemp->host1_vrfid);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip_type=%x !!!\n",ptemp->host2_ip_type);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_enable_flag=%x !!!\n",ptemp->host2_enable_flag);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_udp_port=%x !!!\n",ptemp->host2_udp_port);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip[0]=%x !!!\n",ptemp->host2_ip[0]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip[1]=%x !!!\n",ptemp->host2_ip[1]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip[2]=%x !!!\n",ptemp->host2_ip[2]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_ip[3]=%x !!!\n",ptemp->host2_ip[3]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"host2_vrfid=%x !!!\n",ptemp->host2_vrfid);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_IPV6_DSTHOST *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_IPV6_DSTHOST),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_IPV6_DSTHOST_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_IPV6_DSTHOST));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_ipv6_src_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_ipv6_src_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_IPV6_SRCIP * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_IPV6_SRCIP *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_ipv6_src_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_IPV6_SRCIP *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_ipv6_src_cfg:\n");
                    NBB_PRINTF("src_ipv4_enable=%x !!!\n",ptemp->src_ipv4_enable);
                    NBB_PRINTF("src_ipv6_enable=%x !!!\n",ptemp->src_ipv6_enable);
                    NBB_PRINTF("ipv4_addr=%x !!!\n",ptemp->ipv4_addr);
                    NBB_PRINTF("ipv6_addr[0]=%x !!!\n",ptemp->ipv6_addr[0]);
                    NBB_PRINTF("ipv6_addr[1]=%x !!!\n",ptemp->ipv6_addr[1]);
                    NBB_PRINTF("ipv6_addr[2]=%x !!!\n",ptemp->ipv6_addr[2]);
                    NBB_PRINTF("ipv6_addr[3]=%x !!!\n",ptemp->ipv6_addr[3]); 
                    NBB_PRINTF("\n");
                }  

                sprintf(ucMessage,"****IPV6源IP地址配置 IPV6 SrcIP (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_ip_nf_ipv6_src_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"src_ipv4_enable=%x !!!\n",ptemp->src_ipv4_enable);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"src_ipv6_enable=%x !!!\n",ptemp->src_ipv6_enable);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv4_addr=%x !!!\n",ptemp->ipv4_addr);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv6_addr[0]=%x !!!\n",ptemp->ipv6_addr[0]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv6_addr[1]=%x !!!\n",ptemp->ipv6_addr[1]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv6_addr[2]=%x !!!\n",ptemp->ipv6_addr[2]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"ipv6_addr[3]=%x !!!\n",ptemp->ipv6_addr[3]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage); 
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_IPV6_SRCIP *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_IPV6_SRCIP),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_IPV6_SRCIP_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_IPV6_SRCIP));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_ipv6_version_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_ipv6_version_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_IPV6_VERSION * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_IPV6_VERSION *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_ipv6_version_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_IPV6_VERSION *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_ipv6_version_cfg:\n");
                    NBB_PRINTF("version=%x !!!\n",ptemp->version);
                    NBB_PRINTF("as_number=%x !!!\n",ptemp->as_number);                 
                    NBB_PRINTF("\n");
                }

                sprintf(ucMessage,"****IPV6输出版本 IPV6 Verson (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_ip_nf_ipv6_version_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage); 
                sprintf(ucMessage,"version=%x !!!\n",ptemp->version);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage); 
                sprintf(ucMessage,"as_number=%x !!!\n",ptemp->as_number);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);                  
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_IPV6_VERSION *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_IPV6_VERSION),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_IPV6_VERSION_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_IPV6_VERSION));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_tcp_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_tcp_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_CHECK_TCP * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_CHECK_TCP *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_tcp_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_CHECK_TCP *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_tcp_cfg:\n");
                    NBB_PRINTF("enable_flag=%x !!!\n",ptemp->enable_flag);               
                    NBB_PRINTF("\n");
                } 

                sprintf(ucMessage,"****TCP标志检查 CheckTcpFlag (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_ip_nf_tcp_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"enable_flag=%x !!!\n",ptemp->enable_flag); 
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);              
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_CHECK_TCP *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_CHECK_TCP),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_CHECK_TCP_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_CHECK_TCP));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_timeout_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_timeout_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_TIMEOUT * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_TIMEOUT *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_timeout_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_TIMEOUT *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_timeout_cfg:\n");
                    NBB_PRINTF("inactive_interval=%x !!!\n",ptemp->inactive_interval);
                    NBB_PRINTF("active_interval=%x !!!\n",ptemp->active_interval);
                    NBB_PRINTF("bytes_limit=%x !!!\n",ptemp->bytes_limit);
                    NBB_PRINTF("\n");
                }  

                sprintf(ucMessage,"****老化 TimeOut (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_ip_nf_timeout_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"inactive_interval=%x !!!\n",ptemp->inactive_interval);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"active_interval=%x !!!\n",ptemp->active_interval);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"bytes_limit=%x !!!\n",ptemp->bytes_limit);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

              

                
                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_TIMEOUT *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_TIMEOUT),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_TIMEOUT_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_TIMEOUT));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_start_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_start_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_START * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_START *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_start_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_START *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_start_cfg:\n");
                    NBB_PRINTF("enable_flag=%x !!!\n",ptemp->enable_flag);
                    NBB_PRINTF("\n");
                } 

                sprintf(ucMessage,"****流采样进程控制 start(ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                sprintf(ucMessage,"spm_set_ip_nf_start_cfg:\n");
                sprintf(ucMessage,"enable_flag=%x !!!\n",ptemp->enable_flag);
                sprintf(ucMessage,"\n");

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_START *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_START),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_START_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_START));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_export_info_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_export_info_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_EXPORT_INFO * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_EXPORT_INFO *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_export_info_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_EXPORT_INFO *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_export_info_cfg:\n");
                    NBB_PRINTF("vlan_enable_flag=%x !!!\n",ptemp->vlan_enable_flag);
                    NBB_PRINTF("mpls_enable_flag=%x !!!\n",ptemp->mpls_enable_flag);
                    NBB_PRINTF("\n");
                }
                
                sprintf(ucMessage,"****流采样输出信息控制 export info (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                sprintf(ucMessage,"spm_set_ip_nf_export_info_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"vlan_enable_flag=%x !!!\n",ptemp->vlan_enable_flag);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"mpls_enable_flag=%x !!!\n",ptemp->mpls_enable_flag);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_EXPORT_INFO *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_EXPORT_INFO),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_EXPORT_INFO_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_EXPORT_INFO));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_enable_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_enable_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_ENABLE * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_ENABLE *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_enable_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_ENABLE *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_enable_cfg:\n");
                    NBB_PRINTF("enable_flag=%x !!!\n",ptemp->enable_flag);
                    NBB_PRINTF("\n");
                } 

                sprintf(ucMessage,"****灵活流采样使能 Enable (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                sprintf(ucMessage,"spm_set_ip_nf_enable_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"enable_flag=%x !!!\n",ptemp->enable_flag);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_ENABLE *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_ENABLE),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_ENABLE_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_ENABLE));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_ip_nf_flexnfkey_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ip_nf_flexnfkey_cfg(NBB_ULONG oper, ATG_DCI_IP_NF_FLEXNFKEY * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_IP_NF_FLEXNFKEY *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ip_nf_flexnfkey_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_IP_NF_FLEXNFKEY *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_ip_nf_flexnfkey_cfg:\n");
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.src_addr= %d", ptemp->key.src_addr);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.dst_addr= %d ", ptemp->key.dst_addr);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.src_mask= %d ", ptemp->key.src_mask);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.dst_mask= %d ", ptemp->key.dst_mask);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.protocol= %d ", ptemp->key.protocol);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.tos= %d ", ptemp->key.tos);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.src_port= %d ", ptemp->key.src_port);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.dst_port= %d ", ptemp->key.dst_port);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.input_index= %d ", ptemp->key.input_index);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.output_index= %d ", ptemp->key.output_index);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.src_as= %d ", ptemp->key.src_as);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.dst_as= %d ", ptemp->key.dst_as);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.src_pre= %d ", ptemp->key.src_pre);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.dst_pre= %d ", ptemp->key.dst_pre);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.ipv6_flow_label= %d ", ptemp->key.ipv6_flow_label);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.flow_direction= %d ", ptemp->key.flow_direction);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.next_hop= %d ", ptemp->key.next_hop);
                    NBB_PRINTF("RECV FROM BMUTNE ptemp->key.pad= %d ", ptemp->key.pad);
                    NBB_PRINTF("\n");
                } 

                sprintf(ucMessage,"****灵活流采样关键字控制  FlexNfKey (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                sprintf(ucMessage,"spm_set_ip_nf_flexnfkey_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.src_addr= %d", ptemp->key.src_addr);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.dst_addr= %d ", ptemp->key.dst_addr);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.src_mask= %d ", ptemp->key.src_mask);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.dst_mask= %d ", ptemp->key.dst_mask);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.protocol= %d ", ptemp->key.protocol);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.tos= %d ", ptemp->key.tos);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.src_port= %d ", ptemp->key.src_port);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.dst_port= %d ", ptemp->key.dst_port);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.input_index= %d ", ptemp->key.input_index);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.output_index= %d ", ptemp->key.output_index);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.src_as= %d ", ptemp->key.src_as);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.dst_as= %d ", ptemp->key.dst_as);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.src_pre= %d ", ptemp->key.src_pre);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.dst_pre= %d ", ptemp->key.dst_pre);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.ipv6_flow_label= %d ", ptemp->key.ipv6_flow_label);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.flow_direction= %d ", ptemp->key.flow_direction);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.next_hop= %d ", ptemp->key.next_hop);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"RECV FROM BMUTNE ptemp->key.pad= %d ", ptemp->key.pad);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.src_addr= %d", ptemp->key.src_addr);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.dst_addr= %d ", ptemp->key.dst_addr);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.src_mask= %d ", ptemp->key.src_mask);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.dst_mask= %d ", ptemp->key.dst_mask);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.protocol= %d ", ptemp->key.protocol);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.tos= %d ", ptemp->key.tos);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.src_port= %d ", ptemp->key.src_port);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.dst_port= %d ", ptemp->key.dst_port);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.input_index= %d ", ptemp->key.input_index);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.output_index= %d ", ptemp->key.output_index);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.src_as= %d ", ptemp->key.src_as);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.dst_as= %d ", ptemp->key.dst_as);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.src_pre= %d ", ptemp->key.src_pre);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.dst_pre= %d ", ptemp->key.dst_pre);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.ipv6_flow_label= %d ", ptemp->key.ipv6_flow_label);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.flow_direction= %d ", ptemp->key.flow_direction);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.next_hop= %d ", ptemp->key.next_hop);
                //OS_PRINTF("RECV FROM BMUTNE ptemp->key.pad= %d ", ptemp->key.pad);
                    
                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_IP_NF_FLEXNFKEY *)NBB_MM_ALLOC(sizeof(ATG_DCI_IP_NF_FLEXNFKEY),
                            NBB_NORETRY_ACT,
                            MEM_SPM_IP_NF_FLEXNFKEY_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_IP_NF_FLEXNFKEY));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_nf_aggr_aggregation_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_nf_aggr_aggregation_cfg(NBB_ULONG oper, ATG_DCI_NF_AGGR_AGGREGATION * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_NF_AGGR_AGGREGATION *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_nf_aggr_aggregation_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_NF_AGGR_AGGREGATION *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_nf_aggr_aggregation_cfg:\n");
                    NBB_PRINTF("aggr_mode=%x !!!\n",ptemp->aggr_mode);                    
                    NBB_PRINTF("\n");
                } 

                
                sprintf(ucMessage,"****聚合模式 aggregation (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                sprintf(ucMessage,"spm_set_nf_aggr_aggregation_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"aggr_mode=%x !!!\n",ptemp->aggr_mode); 
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);                   
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_NF_AGGR_AGGREGATION *)NBB_MM_ALLOC(sizeof(ATG_DCI_NF_AGGR_AGGREGATION),
                            NBB_NORETRY_ACT,
                            MEM_SPM_NF_AGGR_AGGREGATION_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_NF_AGGR_AGGREGATION));

                //NBB_PRINTF("spm_set_nf_aggr_aggregation_cfg : ptemp=%x !!!\n",ptemp);
                //NBB_PRINTF("spm_set_nf_aggr_aggregation_cfg : *pcb=%x !!!\n",*pcb);
                //NBB_PRINTF("spm_set_nf_aggr_aggregation_cfg : aggr_mode=%d !!!\n",*((NBB_BYTE*)(*pcb)+1));

            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_nf_aggr_version_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_nf_aggr_version_cfg(NBB_ULONG oper, ATG_DCI_NF_AGGR_EXPORT_VERSION * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_NF_AGGR_EXPORT_VERSION *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_nf_aggr_version_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_NF_AGGR_EXPORT_VERSION *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_nf_aggr_version_cfg:\n");
                    NBB_PRINTF("version=%x !!!\n",ptemp->version);
                    NBB_PRINTF("\n");
                }  

                sprintf(ucMessage,"****输出版本 ExportVersion (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"spm_set_nf_aggr_version_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"version=%x !!!\n",ptemp->version);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_NF_AGGR_EXPORT_VERSION *)NBB_MM_ALLOC(sizeof(ATG_DCI_NF_AGGR_EXPORT_VERSION),
                            NBB_NORETRY_ACT,
                            MEM_SPM_NF_AGGR_EXPORT_VERSION_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_NF_AGGR_EXPORT_VERSION));

                //NBB_PRINTF("spm_set_nf_aggr_aggregation_cfg : version=%d !!!\n",*((NBB_BYTE*)(*pcb)+1));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_nf_aggr_mask_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_nf_aggr_ipv4mask_cfg(NBB_ULONG oper, ATG_DCI_NF_AGGR_IPV4_MASK * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_NF_AGGR_IPV4_MASK *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_nf_aggr_ipv4mask_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_NF_AGGR_IPV4_MASK *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_nf_aggr_ipv4mask_cfg:\n");
                    NBB_PRINTF("src_mask_length=%x !!!\n",ptemp->src_mask_length);
                    NBB_PRINTF("dst_mask_length=%x !!!\n",ptemp->dst_mask_length);
                    NBB_PRINTF("\n");
                } 

                sprintf(ucMessage,"****IPV4掩码 Mask (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                sprintf(ucMessage,"spm_set_nf_aggr_ipv4mask_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"src_mask_length=%x !!!\n",ptemp->src_mask_length);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"dst_mask_length=%x !!!\n",ptemp->dst_mask_length);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_NF_AGGR_IPV4_MASK *)NBB_MM_ALLOC(sizeof(ATG_DCI_NF_AGGR_IPV4_MASK),
                            NBB_NORETRY_ACT,
                            MEM_SPM_NF_AGGR_IPV4_MASK_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_NF_AGGR_IPV4_MASK));

                //NBB_PRINTF("spm_set_nf_aggr_aggregation_cfg : src_mask_length=%d !!!\n",*((NBB_BYTE*)(*pcb)));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_nf_aggr_mask_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_nf_aggr_ipv6mask_cfg(NBB_ULONG oper, ATG_DCI_NF_AGGR_IPV6_MASK * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_NF_AGGR_IPV6_MASK *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_nf_aggr_ipv6mask_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_NF_AGGR_IPV6_MASK *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_nf_aggr_ipv6mask_cfg:\n");
                    NBB_PRINTF("src_mask_length=%x !!!\n",ptemp->src_mask_length);
                    NBB_PRINTF("dst_mask_length=%x !!!\n",ptemp->dst_mask_length);
                    NBB_PRINTF("\n");
                }  

                
                sprintf(ucMessage,"****IPV6掩码 Mask (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                sprintf(ucMessage,"spm_set_nf_aggr_ipv6mask_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"src_mask_length=%x !!!\n",ptemp->src_mask_length);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"dst_mask_length=%x !!!\n",ptemp->dst_mask_length);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_NF_AGGR_IPV6_MASK *)NBB_MM_ALLOC(sizeof(ATG_DCI_NF_AGGR_IPV6_MASK),
                            NBB_NORETRY_ACT,
                            MEM_SPM_NF_AGGR_IPV6_MASK_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_NF_AGGR_IPV6_MASK));

                //NBB_PRINTF("spm_set_nf_aggr_aggregation_cfg : src_mask_length=%d !!!\n",*((NBB_BYTE*)(*pcb)));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_set_nf_aggr_enable_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_nf_aggr_enable_cfg(NBB_ULONG oper, ATG_DCI_NF_AGGR_ENABLE * *pcb, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_NF_AGGR_ENABLE *ptemp = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_nf_aggr_enable_cfg");

    /*ips消息操作类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

            break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

            /*ips消息内容为空*/
            if (NULL == buf)
            {
                NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:buf is NULL.\n"));
                
                OS_PRINTF("***ERROR***:buf is NULL.\n");
                    
                sprintf(ucMessage,"	***ERROR***:buf is NULL.\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage))
                
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            
            /*不为空*/
            else
            {
                ptemp = (ATG_DCI_NF_AGGR_ENABLE *)buf;
                if (nf_ctrl_cfg_print_setting == SPM_PRINT_DETAIL)
                {
                    NBB_PRINTF("spm_set_nf_aggr_enable_cfg:\n");
                    NBB_PRINTF("enable_flag=%x !!!\n",ptemp->enable_flag);
                    NBB_PRINTF("\n");
                }  

                sprintf(ucMessage,"****使能 Enable (ADD)**** \n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                sprintf(ucMessage,"spm_set_nf_aggr_enable_cfg:\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"enable_flag=%x !!!\n",ptemp->enable_flag);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                sprintf(ucMessage,"\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                //NBB_TRC_DETAIL((NBB_FORMAT "policy_index = %ld", ptemp->qos_policy_index));

                /***************************************************************************/
                /*调用驱动配置函数                                            */
                /***************************************************************************/

                /***************************************************************************/
                /*      存配置                                           */
                /***************************************************************************/
                if (NULL == *pcb)
                {
                    *pcb = (ATG_DCI_NF_AGGR_ENABLE *)NBB_MM_ALLOC(sizeof(ATG_DCI_NF_AGGR_ENABLE),
                            NBB_NORETRY_ACT,
                            MEM_SPM_NF_AGGR_ENABLE_CB);
                    
                    if(*pcb == NULL)
                    {
                        ret = ATG_DCI_RC_UNSUCCESSFUL;
            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:未能成功申请内存!\n"));
            	
            			OS_PRINTF("	***ERROR***:未能成功申请内存!\n");
            	
            			OS_SPRINTF(ucMessage, "	***ERROR***:主机未能成功申请内存!\n");
            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            	
            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ALLOC_MEMORY_ERROR, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                        goto EXIT_LABEL;
                    }
                }

                //保存数据
                NBB_MEMCPY(*pcb, ptemp, sizeof(ATG_DCI_NF_AGGR_ENABLE));

                //NBB_PRINTF("spm_set_nf_aggr_aggregation_cfg : enable_flag=%d !!!\n",*((NBB_BYTE*)(*pcb)));
            }

            break;
        default:
            break;
    }

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_ipv4_dst_cfg(ATG_DCI_IP_NF_IPV4_DSTHOST *ipv4_dst_cfg_cb)
{
    /*************************************************************************/
    /* Ipv4报文统计信息输出目的主机地址1:主机IP地址类型（Host IpType）：0/1 = IPV4/IPV6. */
    /*                                  使能标志：0/1 = disable/enable. */
    /*                                  主机UDP端口号 UDP Port	(1-9996-65535). */
    /*                                  IP地址.        */
    /*************************************************************************/
    NBB_BYTE host1_ip_type = 0;
    NBB_BYTE host1_enable_flag = 0;
    NBB_USHORT host1_udp_port = 0;
    NBB_ULONG host1_ip[ATG_DCI_IPV6_LEN] = {0, 0, 0, 0};
    NBB_USHORT host1_vrfid = 0;
    int iptype1 = 0;

    NBB_ULONG ret = SUCCESS;

    /*************************************************************************/
    /* Ipv4报文统计信息输出目的主机地址2:主机IP地址类型（Host IpType）：0/1 = IPV4/IPV6. */
    /*                                  使能标志：0/1 = disable/enable. */
    /*                                  主机UDP端口号 UDP Port	(1-9996-65535). */
    /*                                  IP地址.        */
    /*************************************************************************/
    NBB_BYTE host2_ip_type = 0;
    ;
    NBB_BYTE host2_enable_flag = 0;
    NBB_USHORT host2_udp_port = 0;
    NBB_ULONG host2_ip[ATG_DCI_IPV6_LEN] = {0, 0, 0, 0};
    NBB_USHORT host2_vrfid = 0;
    int iptype2 = 0;

    /*查不到acl 配置*/
    if (NULL == ipv4_dst_cfg_cb)
    {
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf ipv4_dst_cfg host1_ip\n");
            
        }      
        goto EXIT_LABEL;
    }
    else
    {

#ifdef SPU
        host1_ip_type = ipv4_dst_cfg_cb->host1_ip_type;
        host1_enable_flag = ipv4_dst_cfg_cb->host1_enable_flag;
        host1_udp_port = ipv4_dst_cfg_cb->host1_udp_port;
        host1_vrfid = ipv4_dst_cfg_cb->host1_vrfid;
        
        if (0 == host1_ip_type)  /*IPV4*/
        {
            iptype1 = NF_IPV4;
            host1_ip[0] = ipv4_dst_cfg_cb->host1_ip[3];
            ret += nfExportEntrySet((unsigned int *)(&host1_ip[0]),
                iptype1,
                (short int)host1_udp_port,
                host1_enable_flag,
                NF_IPV4, (int)host1_vrfid);
        }
        else if (1 == host1_ip_type)  /*IPV6*/
        {
            iptype1 = NF_IPV6;
            host1_ip[0] = ipv4_dst_cfg_cb->host1_ip[0];
            host1_ip[1] = ipv4_dst_cfg_cb->host1_ip[1];
            host1_ip[2] = ipv4_dst_cfg_cb->host1_ip[2];
            host1_ip[3] = ipv4_dst_cfg_cb->host1_ip[3];
            ret += nfExportEntrySet((unsigned int *)host1_ip, iptype1, (short int)host1_udp_port, host1_enable_flag, NF_IPV4, (int)host1_vrfid);
        }

        /*************************************************************************/
        host2_ip_type = ipv4_dst_cfg_cb->host2_ip_type;
        host2_enable_flag = ipv4_dst_cfg_cb->host2_enable_flag;
        host2_udp_port = ipv4_dst_cfg_cb->host2_udp_port;
        host2_vrfid = ipv4_dst_cfg_cb->host2_vrfid;
        
        if (0 == host2_ip_type)  /*IPV4*/
        {
            iptype2 = NF_IPV4;
            host2_ip[0] = ipv4_dst_cfg_cb->host2_ip[3];
            ret += nfExportEntrySet((unsigned int *)(&host2_ip[0]),
                iptype2,
                (short int)host2_udp_port,
                host2_enable_flag,
                NF_IPV4, (int)host2_vrfid);
        }
        else if (1 == host2_ip_type)  /*IPV6*/
        {
            iptype2 = NF_IPV6;
            host2_ip[0] = ipv4_dst_cfg_cb->host2_ip[0];
            host2_ip[1] = ipv4_dst_cfg_cb->host2_ip[1];
            host2_ip[2] = ipv4_dst_cfg_cb->host2_ip[2];
            host2_ip[3] = ipv4_dst_cfg_cb->host2_ip[3];
            ret += nfExportEntrySet((unsigned int *)host2_ip, iptype2, (short int)host2_udp_port, host2_enable_flag, NF_IPV4, (int)host2_vrfid);
        }
#endif
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_ipv4_src_cfg(ATG_DCI_IP_NF_IPV4_SRCIP *ipv4_src_cfg_cb)
{
    
    /*************************************************************************/
    /* 源IPv4地址使能：0/1 = disable/IPV4.                                   */
    /*************************************************************************/
    NBB_BYTE   src_ipv4_enable;

    /*************************************************************************/
    /* 源IPv6使能标志：0/1 = disable /IPV6.                                  */
    /*************************************************************************/
    NBB_BYTE   src_ipv6_enable;

    /*************************************************************************/
    /* 备用.                                                                 */
    /*************************************************************************/
    NBB_BYTE   pad1[2];

    /*************************************************************************/
    /* IPv4地址.                                                             */
    /*************************************************************************/
    NBB_ULONG  ipv4_addr;

    /*************************************************************************/
    /* IPv6地址.                                                             */
    /*************************************************************************/
    
    NBB_ULONG  ipv6_addr[ATG_DCI_IPV6_LEN];
    NBB_ULONG ret = SUCCESS;
    
    /*查不到acl 配置*/
    if (NULL == ipv4_src_cfg_cb)
    {        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf ipv4_src_cfg src1_ip\n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
#ifdef SPU
        src_ipv4_enable = ipv4_src_cfg_cb->src_ipv4_enable;
        src_ipv6_enable = ipv4_src_cfg_cb->src_ipv6_enable;
        ipv4_addr=ipv4_src_cfg_cb->ipv4_addr;
        ipv6_addr[0] = ipv4_src_cfg_cb->ipv6_addr[0];
        ipv6_addr[1] = ipv4_src_cfg_cb->ipv6_addr[1];
        ipv6_addr[2] = ipv4_src_cfg_cb->ipv6_addr[2];
        ipv6_addr[3] = ipv4_src_cfg_cb->ipv6_addr[3];

        ret += nfExportSrcAddrSet((unsigned int *)(&ipv4_addr), NF_IPV4, src_ipv4_enable, NF_IPV4);
        ret += nfExportSrcAddrSet((unsigned int *)ipv6_addr, NF_IPV6, src_ipv6_enable, NF_IPV4);

        /*************************************************************************/
        /*  //根据0710协议删除
           src2_ip_type = ipv4_src_cfg_cb->src2_ip_type;
           src2_enable_flag = ipv4_src_cfg_cb->src2_enable_flag;

           if (0 == src2_ip_type)  //*IPV4*
           {
            iptype2 = NF_IPV4;
            src2_ip[0] = ipv4_src_cfg_cb->src2_ip[3];
            nfExportSrcAddrSet((unsigned int *)(&src2_ip[0]), iptype2, src2_enable_flag);

           }
           else if (1 == src2_ip_type)  //*IPV6*
           {
            iptype2 = NF_IPV6;
            src2_ip[0] = ipv4_src_cfg_cb->src2_ip[0];
            src2_ip[1] = ipv4_src_cfg_cb->src2_ip[1];
            src2_ip[2] = ipv4_src_cfg_cb->src2_ip[2];
            src2_ip[3] = ipv4_src_cfg_cb->src2_ip[3];
            nfExportSrcAddrSet((unsigned int *)src2_ip, iptype2, src2_enable_flag);
           }
         */
#endif
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_ipv4_version_cfg(ATG_DCI_IP_NF_IPV4_VERSION *ipv4_version_cfg_cb)
{
    /*************************************************************************/
    /* 版本 Verson ：5//8/9 =V5/V8/V9.                                       */
    /*************************************************************************/
    int version = 0;

    /*************************************************************************/
    /* 自治域号 ：0/1 = Origin As / Peer As.                                 */
    /*************************************************************************/
    int as_number = 0;
    NBB_LONG ret = SUCCESS;

    /*************************************************************************/

    /*查不到acl 配置*/
    if (NULL == ipv4_version_cfg_cb)
    {
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf ipv4_version_cfg \n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        version = (int)ipv4_version_cfg_cb->version;
        as_number = (int)ipv4_version_cfg_cb->as_number;
        if (((version == 5)  || (9 == version)) && ((0 == as_number) || (1 == as_number)))
        {
#ifdef SPU
            ret += nfIPv4ExportVerSet(version, as_number);
#endif
        }
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_ipv6_dst_cfg(ATG_DCI_IP_NF_IPV6_DSTHOST *ipv6_dst_cfg_cb)
{
    /*************************************************************************/
    /* Ipv6报文统计信息输出目的主机地址1:主机IP地址类型（Host IpType）：0/1 = IPV4/IPV6. */
    /*                                  使能标志：0/1 = disable/enable. */
    /*                                  主机UDP端口号 UDP Port	(1-9996-65535). */
    /*                                  IP地址.        */
    /*************************************************************************/
    NBB_BYTE host1_ip_type = 0;
    NBB_BYTE host1_enable_flag = 0;
    NBB_USHORT host1_udp_port = 0;
    NBB_ULONG host1_ip[ATG_DCI_IPV6_LEN] = {0, 0, 0, 0};
    NBB_USHORT host1_vrfid = 0;
    int iptype1 = 0;
    NBB_LONG ret = SUCCESS;

    /*************************************************************************/
    /* Ipv6报文统计信息输出目的主机地址2:主机IP地址类型（Host IpType）：0/1 = IPV4/IPV6. */
    /*                                  使能标志：0/1 = disable/enable. */
    /*                                  主机UDP端口号 UDP Port	(1-9996-65535). */
    /*                                  IP地址.        */
    /*************************************************************************/
    NBB_BYTE host2_ip_type = 0;
    NBB_BYTE host2_enable_flag = 0;
    NBB_USHORT host2_udp_port = 0;
    NBB_ULONG host2_ip[ATG_DCI_IPV6_LEN] = {0, 0, 0, 0};
    NBB_USHORT host2_vrfid = 0;
    int iptype2 = 0;

    /*查不到acl 配置*/
    if (NULL == ipv6_dst_cfg_cb)
    {       
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf ipv6_dst_cfg host1_ip\n");
        }      
        goto EXIT_LABEL;
    }
    else
    {

#ifdef SPU
        host1_ip_type = ipv6_dst_cfg_cb->host1_ip_type;
        host1_enable_flag = ipv6_dst_cfg_cb->host1_enable_flag;
        host1_udp_port = ipv6_dst_cfg_cb->host1_udp_port;
        host1_vrfid = ipv6_dst_cfg_cb->host1_vrfid;
        
        if (0 == host1_ip_type)  /*IPV4*/
        {
            iptype1 = NF_IPV4;
            host1_ip[0] = ipv6_dst_cfg_cb->host1_ip[3];
            ret += nfExportEntrySet((unsigned int *)(&host1_ip[0]),
                iptype1,
                (short int)host1_udp_port,
                host1_enable_flag,
                NF_IPV6, (int)host1_vrfid);
        }
        else if (1 == host1_ip_type)  /*IPV6*/
        {
            iptype1 = NF_IPV6;
            host1_ip[0] = ipv6_dst_cfg_cb->host1_ip[0];
            host1_ip[1] = ipv6_dst_cfg_cb->host1_ip[1];
            host1_ip[2] = ipv6_dst_cfg_cb->host1_ip[2];
            host1_ip[3] = ipv6_dst_cfg_cb->host1_ip[3];
            ret += nfExportEntrySet((unsigned int *)host1_ip, iptype1, (short int)host1_udp_port, host1_enable_flag, NF_IPV6, (int)host1_vrfid);
        }

        /*************************************************************************/
        host2_ip_type = ipv6_dst_cfg_cb->host2_ip_type;
        host2_enable_flag = ipv6_dst_cfg_cb->host2_enable_flag;
        host2_udp_port = ipv6_dst_cfg_cb->host2_udp_port;
        host2_vrfid = ipv6_dst_cfg_cb->host2_vrfid;
        
        if (0 == host2_ip_type)  /*IPV4*/
        {
            iptype2 = NF_IPV4;
            host2_ip[0] = ipv6_dst_cfg_cb->host2_ip[3];
            ret += nfExportEntrySet((unsigned int *)(&host2_ip[0]),
                iptype2,
                (short int)host2_udp_port,
                host2_enable_flag,
                NF_IPV6, (int)host2_vrfid);
        }
        else if (1 == host2_ip_type)  /*IPV6*/
        {
            iptype2 = NF_IPV6;
            host2_ip[0] = ipv6_dst_cfg_cb->host2_ip[0];
            host2_ip[1] = ipv6_dst_cfg_cb->host2_ip[1];
            host2_ip[2] = ipv6_dst_cfg_cb->host2_ip[2];
            host2_ip[3] = ipv6_dst_cfg_cb->host2_ip[3];
            ret += nfExportEntrySet((unsigned int *)host2_ip, iptype2, (short int)host2_udp_port, host2_enable_flag, NF_IPV6, (int)host2_vrfid);
        }
#endif
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_ipv6_src_cfg(ATG_DCI_IP_NF_IPV6_SRCIP *ipv6_src_cfg_cb)
{
    /*************************************************************************/
    /* 源IPv4地址使能：0/1 = disable/IPV4.                                   */
    /*************************************************************************/
    NBB_BYTE   src_ipv4_enable;

    /*************************************************************************/
    /* 源IPv6使能标志：0/1 = disable /IPV6.                                  */
    /*************************************************************************/
    NBB_BYTE   src_ipv6_enable;

    /*************************************************************************/
    /* 备用.                                                                 */
    /*************************************************************************/
    NBB_BYTE   pad1[2];

    /*************************************************************************/
    /* IPv4地址.                                                             */
    /*************************************************************************/
    NBB_ULONG  ipv4_addr;

    /*************************************************************************/
    /* IPv6地址.                                                             */
    /*************************************************************************/
    NBB_ULONG  ipv6_addr[ATG_DCI_IPV6_LEN];
    NBB_LONG ret = SUCCESS;


    /*查不到acl 配置*/
    if (NULL == ipv6_src_cfg_cb)
    {        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf ipv6_src_cfg src1_ip\n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
#ifdef SPU
        src_ipv4_enable = ipv6_src_cfg_cb->src_ipv4_enable;
        src_ipv6_enable = ipv6_src_cfg_cb->src_ipv6_enable;
        ipv4_addr=ipv6_src_cfg_cb->ipv4_addr;
        ipv6_addr[0] = ipv6_src_cfg_cb->ipv6_addr[0];
        ipv6_addr[1] = ipv6_src_cfg_cb->ipv6_addr[1];
        ipv6_addr[2] = ipv6_src_cfg_cb->ipv6_addr[2];
        ipv6_addr[3] = ipv6_src_cfg_cb->ipv6_addr[3];

        ret += nfExportSrcAddrSet((unsigned int *)(&ipv4_addr), NF_IPV4, src_ipv4_enable, NF_IPV6);
        ret += nfExportSrcAddrSet((unsigned int *)ipv6_addr, NF_IPV6, src_ipv6_enable, NF_IPV6);

        /*************************************************************************/
        /*
           src2_ip_type = ipv6_src_cfg_cb->src2_ip_type;
           src2_enable_flag = ipv6_src_cfg_cb->src2_enable_flag;

           if (0 == src2_ip_type)  //*IPV4*
           {
            iptype2 = NF_IPV4;
            src2_ip[0] = ipv6_src_cfg_cb->src2_ip[3];
            nfExportSrcAddrSet((unsigned int *)(&src2_ip[0]), iptype2, src2_enable_flag);

           }
           else if (1 == src2_ip_type)  //*IPV6*
           {
            iptype2 = NF_IPV6;
            src2_ip[0] = ipv6_src_cfg_cb->src2_ip[0];
            src2_ip[1] = ipv6_src_cfg_cb->src2_ip[1];
            src2_ip[2] = ipv6_src_cfg_cb->src2_ip[2];
            src2_ip[3] = ipv6_src_cfg_cb->src2_ip[3];
            nfExportSrcAddrSet((unsigned int *)src2_ip, iptype2, src2_enable_flag);
           }
         */
#endif
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

   /*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_ipv6_version_cfg(ATG_DCI_IP_NF_IPV6_VERSION *ipv6_version_cfg_cb)
{
    /*************************************************************************/
    /* 版本 Verson ：5//8/9 =V5/V8/V9.                                       */
    /*************************************************************************/
    int version = 0;

    /*************************************************************************/
    /* 自治域号 ：0/1 = Origin As / Peer As.                                 */
    /*************************************************************************/
    int as_number = 0;

    /*************************************************************************/
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == ipv6_version_cfg_cb)
    {        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf ipv6_version_cfg \n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        version = (int)ipv6_version_cfg_cb->version;
        as_number = (int)ipv6_version_cfg_cb->as_number;
        if (((version == 5) || (version == 8) || (9 == version)) && ((0 == as_number) || (1 == as_number)))
        {
#ifdef SPU

            ret = nfIPv6ExportVerSet(version, as_number);
#endif
        }
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_tcp_cfg(ATG_DCI_IP_NF_CHECK_TCP *tcp_cfg_cb)
{
    /* 使能标志：0/1 = disable/enable.                                       */
    /*************************************************************************/
    int enable_flag = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == tcp_cfg_cb)
    {        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf tcp cfg \n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        enable_flag = (int)tcp_cfg_cb->enable_flag;
        if ((0 == enable_flag) || (1 == enable_flag))
        {
#ifdef SPU
            ret =nfCheckTCPFlagSet(enable_flag);
#endif
        }
    }

    EXIT_LABEL:

    return ret ;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_timeout_cfg(ATG_DCI_IP_NF_TIMEOUT *timeout_cfg_cb)
{
    /*************************************************************************/
    /* 不活跃老化时间 inactive-interval（单位：秒）：10-30-600.              */
    /*************************************************************************/
    int inactive_interval = 0;

    /*************************************************************************/
    /* 活跃老化时间 active-interval （单位：分）：10-30-60.                  */
    /*************************************************************************/
    int active_interval = 0;

    /*************************************************************************/
    /* 溢出老化字节数 bytes-limit（单位：MB）：1-1024-4096.                  */
    /*************************************************************************/
    int bytes_limit = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == timeout_cfg_cb)
    {       
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf timeout cfg \n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        inactive_interval = (int)timeout_cfg_cb->inactive_interval;
        active_interval = (int)timeout_cfg_cb->active_interval;
        bytes_limit = (int)timeout_cfg_cb->bytes_limit;
#ifdef SPU
        ret = nfTimeoutAttrSet(inactive_interval, active_interval, bytes_limit);
#endif

    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_start_cfg(ATG_DCI_IP_NF_START *start_cfg_cb)
{
    /* 使能标志：0/1 = disable/enable.                                       */
    /*************************************************************************/
    int enable_flag = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == start_cfg_cb)
    {        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf start cfg \n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        enable_flag = (int)start_cfg_cb->enable_flag;
        if ((0 == enable_flag) || (1 == enable_flag))
        {
#ifdef SPU
            ret = nfStart(enable_flag);
#endif
        }

    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_info_cfg(ATG_DCI_IP_NF_EXPORT_INFO *info_cfg_cb)
{
    /*************************************************************************/
    /* VLAN统计使能标志：0/1 = disable/enable.                               */
    /*************************************************************************/
    int vlan_enable_flag = 0;

    /*************************************************************************/
    /* MPLS统计使能标志：0/1 = disable/enable.                               */
    /*************************************************************************/
    int mpls_enable_flag = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == info_cfg_cb)
    {        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf export info cfg \n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        vlan_enable_flag = (int)info_cfg_cb->vlan_enable_flag;
        mpls_enable_flag = (int)info_cfg_cb->mpls_enable_flag;
        if (((0 == vlan_enable_flag) || (1 == vlan_enable_flag)) && ((0 == mpls_enable_flag)
            || (1 == mpls_enable_flag)))
        {
#ifdef SPU
            ret = nfVlanMplsSwitchSet(vlan_enable_flag, mpls_enable_flag);
#endif
        }

    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_ip_nf_enable_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_enable_cfg(ATG_DCI_IP_NF_ENABLE *enable_cfg_cb)
{
    /* 使能标志：0/1 = disable/enable.                                       */
    /*************************************************************************/
    int enable_flag = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == enable_cfg_cb)
    {        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf enable cfg \n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        enable_flag = (int)enable_cfg_cb->enable_flag;
        if ((0 == enable_flag) || (1 == enable_flag))
        {
#ifdef SPU
            ret = nfFlexSwitchSet(enable_flag);
#endif
        }

    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_ip_nf_flexnfkey_cfg(ATG_DCI_IP_NF_FLEXNFKEY *flexnfkey_cfg_cb)
{
    /*************************************************************************/
    /*  灵活流采样关键字.                                                    */
    /*************************************************************************/
    uint32_t key;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == flexnfkey_cfg_cb)
    {        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf flexnfkey cfg \n");
        }      
        goto EXIT_LABEL;
    }
    else
    {

        //key=flexnfkey_cfg_cb->key;
#ifdef SPU
        key = (uint32_t)((flexnfkey_cfg_cb->key.src_addr ) + (flexnfkey_cfg_cb->key.dst_addr << 1)
            + (flexnfkey_cfg_cb->key.src_mask << 2) + (flexnfkey_cfg_cb->key.dst_mask << 3)
            + (flexnfkey_cfg_cb->key.protocol << 4) + (flexnfkey_cfg_cb->key.tos << 5)
            + (flexnfkey_cfg_cb->key.src_port << 6) + (flexnfkey_cfg_cb->key.dst_port << 7)
            + (flexnfkey_cfg_cb->key.input_index << 8) + (flexnfkey_cfg_cb->key.output_index << 9)
            + (flexnfkey_cfg_cb->key.src_as << 10) + (flexnfkey_cfg_cb->key.dst_as << 11)
            + (flexnfkey_cfg_cb->key.src_pre << 12) + (flexnfkey_cfg_cb->key.dst_pre << 13)
            + (flexnfkey_cfg_cb->key.ipv6_flow_label << 14) + (flexnfkey_cfg_cb->key.flow_direction << 15)
            + (flexnfkey_cfg_cb->key.next_hop << 16) );
        ret = nfFlexKeyGet(key);

#endif

    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_nf_aggr_mode_cfg(ATG_DCI_NF_AGGR_AGGREGATION *aggregation_cfg)
{
    /***************************************************************************/
    /*    聚合模式 ：                                                     */
    uint32_t aggr_mode = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == aggregation_cfg)
    {
        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find nf aggr aggregation cfg\n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        aggr_mode = (uint32_t)aggregation_cfg->aggr_mode;
        if (((aggr_mode >= 1) && (aggr_mode <= 5)) || ((aggr_mode >= 9) && (aggr_mode <= 14)))
        {
#ifdef SPU
            ret = nfAggrModeSet(aggr_mode);
#endif
        }
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_nf_aggr_export_version_cfg(ATG_DCI_NF_AGGR_EXPORT_VERSION *export_version_cfg)
{
    /***************************************************************************/
    /*    聚合模式 ：                                                     */
    int version = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == export_version_cfg)
    {
        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find nf aggr export_version_cfg\n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        version = (int)export_version_cfg->version;
        if ((8 == version) || (9 == version))
        {
#ifdef SPU
            ret = nfIpv4AggreVerSet(version);
#endif
        }
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_nf_aggr_ipv4mask_cfg(ATG_DCI_NF_AGGR_IPV4_MASK *mask_cfg)
{
    /*************************************************************************/
    /* 源掩码长度：1-32.                                                     */
    /*************************************************************************/
    uint32_t src_mask_length = 0;

    /*************************************************************************/
    /* 目的掩码长度：1-32.                                                   */
    /*************************************************************************/
    uint32_t dst_mask_length = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == mask_cfg)
    {
        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find nf aggr ipv4mask cfg\n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        src_mask_length = (uint32_t)mask_cfg->src_mask_length;
        dst_mask_length = (uint32_t)mask_cfg->dst_mask_length;
        if (((src_mask_length >= 1) && (src_mask_length <= 32)) &&
            ((dst_mask_length >= 1) && (dst_mask_length <= 32)))
        {
#ifdef SPU
            ret = nfAggrIpv4MaskSet(src_mask_length, dst_mask_length);
#endif
        }
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_nf_aggr_ipv6mask_cfg(ATG_DCI_NF_AGGR_IPV6_MASK *mask_cfg)
{
    /*************************************************************************/
    /* 源掩码长度：1-32.                                                     */
    /*************************************************************************/
    uint32_t src_mask_length = 0;

    /*************************************************************************/
    /* 目的掩码长度：1-32.                                                   */
    /*************************************************************************/
    uint32_t dst_mask_length = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == mask_cfg)
    {
        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find nf aggr ipv6mask cfg\n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        src_mask_length = (uint32_t)mask_cfg->src_mask_length;
        dst_mask_length = (uint32_t)mask_cfg->dst_mask_length;
        if (((src_mask_length >= 1) && (src_mask_length <= 128)) &&
            ((dst_mask_length >= 1) && (dst_mask_length <= 128)))
        {
#ifdef SPU
            ret = nfAggrIpv6MaskSet(src_mask_length, dst_mask_length);
#endif
        }
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_ULONG spm_proc_nf_aggr_enable_cfg(ATG_DCI_NF_AGGR_ENABLE *enable_cfg)
{
    /***************************************************************************/
    /*    聚合模式 ：                                                     */
    int enable_flag = 0;
    NBB_LONG ret = SUCCESS;

    /*查不到acl 配置*/
    if (NULL == enable_cfg)
    {
        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find nf aggr enable_cfg\n");
        }      
        goto EXIT_LABEL;
    }
    else
    {
        enable_flag = (int)enable_cfg->enable_flag;
        if ((0 == enable_flag) || (1 == enable_flag))
        {
#ifdef SPU
            ret = nfAggrSwitchSet(enable_flag);
#endif
        }
    }

    EXIT_LABEL:

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_proc_ip_nf_cfg(NBB_BYTE type)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_IP_NF_CB *pcb = NULL;
    NBB_LONG index = 0;
    NBB_BYTE fapid = 0;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_LONG ret = SUCCESS;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    spm_hardw_getslot(&fapid);

    OS_MEMSET(ucMessage, 0, SPM_MSG_INFO_LEN);
    
    //NBB_PRINTF("fapid = %x\n ",fapid);
    if (0 == fapid) /*非法*/
    {
        NBB_PRINTF("**ERROR** ip_nf index slot = 0 ");
        goto EXIT_LABEL;
    }
    index = (NBB_LONG)fapid;
    pcb = AVLL_FIND(v_spm_shared->ip_nf_tree, &index);

    /*查不到acl 配置*/
    if (NULL == pcb)
    {
        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find ip nf cfg\n");
        }      
        goto EXIT_LABEL;
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV4_DST == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_ipv4_dst_cfg(pcb->ipv4_dst_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_ipv4_dst_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_ipv4_dst_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV4_SRC == type))
    {
        
        ret = SUCCESS;
        ret = spm_proc_ip_nf_ipv4_src_cfg(pcb->ipv4_src_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_ipv4_src_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_ipv4_src_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV4_VERSION == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_ipv4_version_cfg(pcb->ipv4_version_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_ipv4_version_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_ipv4_version_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV6_DST == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_ipv6_dst_cfg(pcb->ipv6_dst_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_ipv6_dst_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** sspm_proc_ip_nf_ipv6_dst_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV6_SRC == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_ipv6_src_cfg(pcb->ipv6_src_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_ipv6_src_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_ipv6_src_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV6_VERSION == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_ipv6_version_cfg(pcb->ipv6_version_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_ipv6_version_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_ipv6_version_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_CHECK_TCP == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_tcp_cfg(pcb->tcp_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_tcp_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_tcp_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_TIMEOUT == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_timeout_cfg(pcb->timeout_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_timeout_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_timeout_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_START == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_start_cfg(pcb->start_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_start_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_start_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_EXPORT_INFO == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_info_cfg(pcb->info_cfg_cb);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_info_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** sspm_proc_ip_nf_info_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_ENABLE == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_enable_cfg(pcb->enable_cfg);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_enable_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_enable_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_FLEXNFKEY == type))
    {
        ret = SUCCESS;
        ret = spm_proc_ip_nf_flexnfkey_cfg(pcb->flexnfkey_cfg);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_ip_nf_flexnfkey_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_ip_nf_flexnfkey_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }

    //NBB_PRINTF("ip nf cnt = %ld\n", pcb->cnt);

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_proc_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_proc_nf_aggr_cfg(NBB_BYTE type)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_NF_AGGR_CB *pcb = NULL;
    NBB_LONG index = 0;
    NBB_BYTE fapid = 0;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    NBB_ULONG ret =SUCCESS;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    spm_hardw_getslot(&fapid);

    OS_MEMSET(ucMessage, 0, SPM_MSG_INFO_LEN);
    
    if (0 == fapid) /*非法*/
    {
        NBB_PRINTF("**ERROR** nf_aggr index slot = 0 ");
        goto EXIT_LABEL;
    }
    index = (NBB_LONG)fapid;
    pcb = AVLL_FIND(v_spm_shared->nf_aggr_tree, &index);

    /*查不到acl 配置*/
    if (NULL == pcb)
    {
        
        if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
        {
            NBB_PRINTF("can't find nf aggr cfg\n");
        }      
        goto EXIT_LABEL;
    }

    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_MODE == type))
    {
        ret = SUCCESS;
        ret = spm_proc_nf_aggr_mode_cfg(pcb->aggregation_cfg);
        
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_nf_aggr_mode_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_nf_aggr_mode_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_EXPORT_VERSION == type))
    {
        ret = SUCCESS;
        ret = spm_proc_nf_aggr_export_version_cfg(pcb->export_version_cfg);
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_nf_aggr_export_version_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_nf_aggr_export_version_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_IPV4_MASK == type))
    {
        ret = SUCCESS;
        ret = spm_proc_nf_aggr_ipv4mask_cfg(pcb->ipv4mask_cfg);
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_nf_aggr_ipv4mask_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_nf_aggr_ipv4mask_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_IPV6_MASK == type))
    {
        ret = SUCCESS;
        ret = spm_proc_nf_aggr_ipv6mask_cfg(pcb->ipv6mask_cfg);
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_nf_aggr_ipv6mask_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_nf_aggr_ipv6mask_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }
    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_ENABLE == type))
    {
        ret = SUCCESS;
        ret = spm_proc_nf_aggr_enable_cfg(pcb->enable_cfg);
        if(ret != SUCCESS)
        {
            OS_PRINTF("***ERROR*** spm_proc_nf_aggr_enable_cfg  ret = %d\n",ret);
            sprintf(ucMessage,"***ERROR*** spm_proc_nf_aggr_enable_cfg  ret = %d\n",ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
        }
    }

    //NBB_PRINTF("nf aggr cnt = %ld\n", pcb->cnt);

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_rcv_dci_set_ip_nf
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_ip_nf(ATG_DCI_SET_IP_NF *pstSetIps)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = EXIST;
    NBB_ULONG ulkey = 0;
    SPM_IP_NF_CB *pstCb = NULL;
    NBB_VOID *retptr = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	NBB_BYTE ucMessageLen = 0;

    /* IPS消息偏移的首地址 */

    NBB_BYTE *pucIpv4DsthostDataStart = NULL;
    NBB_BYTE *pucIpv4SrcipDataStart = NULL;
    NBB_BYTE *pucIpv4VersionDataStart = NULL;
    NBB_BYTE *pucIpv6DsthostDataStart = NULL;
    NBB_BYTE *pucIpv6SrcipDataStart = NULL;
    NBB_BYTE *pucIpv6VersionDataStart = NULL;
    NBB_BYTE *pucCheckTcpDataStart = NULL;
    NBB_BYTE *pucTimeoutDataStart = NULL;
    NBB_BYTE *pucStartDataStart = NULL;
    NBB_BYTE *pucExportInfoDataStart = NULL;
    NBB_BYTE *pucEnableDataStart = NULL;
    NBB_BYTE *pucFlexnfkeyDataStart = NULL;

    NBB_ULONG ulOperIpv4Dsthost = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4Srcip = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4Version = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6Dsthost = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6Srcip = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6Version = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperCheckTcp = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperTimeout = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperStart = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperExportInfo = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperEnable = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperFlexnfkey = ATG_DCI_OPER_NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    /*逻辑处理流程*/
    NBB_TRC_ENTRY("spm_rcv_dci_set_ip_nf");

    spm_ips_ip_nf_rcv_num++;

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetIps != NULL);

    if(pstSetIps == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:spm_rcv_dci_set_ip_nf(pstSetIps==NULL)\n"));
	
		OS_PRINTF("	***ERROR***:spm_rcv_dci_set_ip_nf(pstSetIps==NULL)\n");
	
		OS_SPRINTF(ucMessage, "	***ERROR***:spm_rcv_dci_set_ip_nf(pstSetIps==NULL)\n");
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
	
		NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL\n", ucMessage));
			
		goto EXIT_LABEL;
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetIps->return_code = ATG_DCI_RC_OK;

    ulkey = pstSetIps->key;

    //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstSetIps->key= %x!!!\n",pstSetIps->key);
    pstCb = AVLL_FIND(SHARED.ip_nf_tree, &ulkey);

    //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstCb=%x !!!\n",pstCb);
    /* 如果条目不存在*/
    if (pstCb == NULL)
    {
        ucIfExist = UNEXIST;
    }

    /* 获取子配置操作 */
    ulOperIpv4Dsthost = pstSetIps->oper_ipv4_dsthost;
    ulOperIpv4Srcip = pstSetIps->oper_ipv4_srcip;
    ulOperIpv4Version = pstSetIps->oper_ipv4_version;
    ulOperIpv6Dsthost = pstSetIps->oper_ipv6_dsthost;
    ulOperIpv6Srcip = pstSetIps->oper_ipv6_srcip;
    ulOperIpv6Version = pstSetIps->oper_ipv6_version;
    ulOperCheckTcp = pstSetIps->oper_check_tcp;
    ulOperTimeout = pstSetIps->oper_timeout;
    ulOperStart = pstSetIps->oper_start;
    ulOperExportInfo = pstSetIps->oper_export_info;
    ulOperEnable = pstSetIps->oper_enable;
    ulOperFlexnfkey = pstSetIps->oper_flexnfkey;

    /*获取子配置的偏移地址*/
    pucIpv4DsthostDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_dsthost_data);
    pucIpv4SrcipDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_srcip_data);
    pucIpv4VersionDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_version_data);
    pucIpv6DsthostDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv6_dsthost_data);
    pucIpv6SrcipDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv6_srcip_data);
    pucIpv6VersionDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv6_version_data);
    pucCheckTcpDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->check_tcp_data);
    pucTimeoutDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->timeout_data);
    pucStartDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->start_data);
    pucExportInfoDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->export_info_data);
    pucEnableDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->enable_data);
    pucFlexnfkeyDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->flexnfkey_data);

    /*消息类型为删除*/
    if (TRUE == pstSetIps->delete_struct)
    {

        /* 如果条目不存在，不删除 */
        if (ucIfExist == UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:delete ip nf key %ld is not in use\n", ulkey));
	
			OS_PRINTF("***ERROR***:delete ip nf key %ld is not in use\n", ulkey);
	
			OS_SPRINTF(ucMessage, "***ERROR***:delete ip nf key %ld is not in use\n", ulkey);
			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
	
			NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, DEL_UNEXIST_CONFIG_ERROR, "DEL_UNEXIST_CONFIG_ERROR", ucMessage));

            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;

            goto EXIT_LABEL;
        }
        
        /* 存在，删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "*****delete ip nf (key = %ld)*****\n", ulkey));
            
            OS_SPRINTF(ucMessage, "*****delete ip nf (key = %ld)*****\n", ulkey);
			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);


            if(NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
            
            if (0 != pstCb->cnt) /*引用计数不为0*/
            {
                
                NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:can't del ip nf key ,cnt = %d\n",pstCb->cnt));
	
                OS_PRINTF("***ERROR***:can't del ip nf key ,cnt = %d\n",pstCb->cnt);

                OS_SPRINTF(ucMessage, "***ERROR***:can't del ip nf key ,cnt = %d\n",pstCb->cnt);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, DEL_UNEXIST_CONFIG_ERROR, "DEL_IP_NF CNT != 0", ucMessage));

                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;

                goto EXIT_LABEL;
            }

            if(pstCb != NULL)
            {
                AVLL_DELETE(SHARED.ip_nf_tree, pstCb->ip_nf_node);

                //释放ipnetflow表分配的内存空间
                ret = spm_free_ip_nf_cb(pstCb);

                if (ATG_DCI_RC_OK != ret) /*释放内存出错*/
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "***FATAL ERROR***:spm_free_ip_nf_cb ret=%ld\n", ret));
                    OS_SPRINTF(ucMessage, "***FATAL ERROR***:spm_free_ip_nf_cb ret=%ld\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, DEL_UNEXIST_CONFIG_ERROR, "DEL_IP_NF CNT != 0", ucMessage));

                    goto EXIT_LABEL;
                }
            }

            
        }
    }
    
    /*增加或者更新*/
    else
    {
        NBB_TRC_DETAIL((NBB_FORMAT "*****add/update ip nf (key=%ld)*****\n", ulkey));

        OS_SPRINTF(ucMessage, "*****add/update ip nf (key = %ld)*****\n", ulkey);
	    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == UNEXIST)
        {

            /* 申请一个新条目的内存空间 */
            pstCb = spm_alloc_ip_nf_cb();

            //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstCb=%x !!!\n",pstCb);
            /*分配内存失败*/
            if (NULL == pstCb)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***:create mem for ip nf key =%ld!\n",ulkey));
                OS_SPRINTF(ucMessage, "***ERROR***:create mem for ip nf key =%ld!\n",ulkey);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;

                goto EXIT_LABEL;
            }

        }

        if(NULL == pstCb)
        {
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }
        
        if (0 != pstCb->cnt) /*被引用*/
        {
            NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***:can't update and add,Cnt = %d\n",pstCb->cnt));
            OS_SPRINTF(ucMessage, "***ERROR***:can't update and add,Cnt = %d\n",pstCb->cnt);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, CONFIG_IS_IN_USE, "CONFIG_IS_IN_USE", ucMessage));

            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;

            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /* IPV4NetFlow输出目的主机 */
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_ipv4_dst_cfg(ulOperIpv4Dsthost, &(pstCb->ipv4_dst_cfg_cb) ,
                pucIpv4DsthostDataStart);

        pstSetIps->ipv4_dsthost_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*IPV4NetFlow源IP地址配置*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_ipv4_src_cfg(ulOperIpv4Srcip, &(pstCb->ipv4_src_cfg_cb) ,

            pucIpv4SrcipDataStart);

        pstSetIps->ipv4_srcip_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*IPV4NetFlow输出版本 */
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_ipv4_version_cfg(ulOperIpv4Version, &(pstCb->ipv4_version_cfg_cb) ,

            pucIpv4VersionDataStart);

        pstSetIps->ipv4_version_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /* IPV6NetFlow输出目的主机 */
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_ipv6_dst_cfg(ulOperIpv6Dsthost, &(pstCb->ipv6_dst_cfg_cb) ,

            pucIpv6DsthostDataStart);

        pstSetIps->ipv6_dsthost_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*IPV6NetFlow源IP地址配置*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_ipv6_src_cfg(ulOperIpv6Srcip,
            &(pstCb->ipv6_src_cfg_cb) ,
            pucIpv6SrcipDataStart);

        pstSetIps->ipv6_srcip_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*IPV6NetFlow输出版本 */
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_ipv6_version_cfg(ulOperIpv6Version, &(pstCb->ipv6_version_cfg_cb) ,

            pucIpv6VersionDataStart);

        pstSetIps->ipv6_version_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*TCP标志检查 */
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_tcp_cfg(ulOperCheckTcp, &(pstCb->tcp_cfg_cb) ,

            pucCheckTcpDataStart);

        pstSetIps->check_tcp_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*老化*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_timeout_cfg(ulOperTimeout, &(pstCb->timeout_cfg_cb) , pucTimeoutDataStart);

        pstSetIps->timeout_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*流采样进程控制*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_start_cfg(ulOperStart, &(pstCb->start_cfg_cb) ,

            pucStartDataStart);

        pstSetIps->start_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*流采样输出信息控制*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_export_info_cfg(ulOperExportInfo, &(pstCb->info_cfg_cb) , pucExportInfoDataStart);

        pstSetIps->export_info_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*流采样进程控制*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_enable_cfg(ulOperEnable, &(pstCb->enable_cfg) ,

            pucEnableDataStart);

        pstSetIps->enable_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*流采样输出信息控制*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_ip_nf_flexnfkey_cfg(ulOperFlexnfkey, &(pstCb->flexnfkey_cfg) , pucFlexnfkeyDataStart);

        pstSetIps->flexnfkey_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

    }

    pstSetIps->return_code = ips_ret;

    /* 如果是新增的条目，插入到树中 */
    if (UNEXIST == ucIfExist)
    {
        pstCb->ip_nf_key = ulkey;

        //coverity[no_effect_test]
        AVLL_INSERT(SHARED.ip_nf_tree, pstCb->ip_nf_node);

        //NBB_PRINTF("spm_rcv_dci_set_ip_nf: retptr=%x !!!\n",retptr);
        //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstCb->ip_nf_node.parent=%x !!!\n",pstCb->ip_nf_node.parent);
        //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstCb->ip_nf_node.left=%x !!!\n",pstCb->ip_nf_node.left);
        //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstCb->ip_nf_node.right=%x !!!\n",pstCb->ip_nf_node.right);
        //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstCb->ip_nf_node.l_height=%x !!!\n",pstCb->ip_nf_node.l_height);
        //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstCb->ip_nf_node.r_height=%x !!!\n",pstCb->ip_nf_node.r_height);
        //NBB_PRINTF("spm_rcv_dci_set_ip_nf: pstCb->ip_nf_key=%x !!!\n",pstCb->ip_nf_key);
    }

    spm_proc_ip_nf_cfg(SPM_NF_ALL);

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_rcv_dci_set_nf_aggr
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_nf_aggr(ATG_DCI_SET_NF_AGGR *pstSetIps)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = EXIST;
    NBB_ULONG ulkey = 0;
    SPM_NF_AGGR_CB *pstCb = NULL;
    NBB_VOID *retptr = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;
    
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;

    /* IPS消息偏移的首地址 */

    NBB_BYTE *pucAggregationDataStart = NULL;
    NBB_BYTE *pucVersionDataStart = NULL;
    NBB_BYTE *pucIpv4MaskDataStart = NULL;
    NBB_BYTE *pucIpv6MaskDataStart = NULL;
    NBB_BYTE *pucEnableDataStart = NULL;

    NBB_ULONG ulOperAggregation = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperVersion = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4Mask = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6Mask = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperEnable = ATG_DCI_OPER_NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    /*逻辑处理流程*/
    NBB_TRC_ENTRY("spm_rcv_dci_set_nf_aggr");
    spm_ips_aggr_nf_rcv_num++;

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetIps != NULL);

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetIps->return_code = ATG_DCI_RC_OK;

    ulkey = pstSetIps->key;

    //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstSetIps->key= %x!!!\n",pstSetIps->key);

    pstCb = AVLL_FIND(SHARED.nf_aggr_tree, &ulkey);

    //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstCb=%x !!!\n",pstCb);
    /* 如果条目不存在*/
    if (pstCb == NULL)
    {
        ucIfExist = UNEXIST;
    }

    /* 获取子配置操作 */
    ulOperAggregation = pstSetIps->oper_aggregation;
    ulOperVersion = pstSetIps->oper_export_version;
    ulOperIpv4Mask = pstSetIps->oper_ipv4_mask;
    ulOperIpv6Mask = pstSetIps->oper_ipv6_mask;
    ulOperEnable = pstSetIps->oper_enable;

    /*获取子配置的偏移地址*/
    pucAggregationDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->aggregation_data);
    pucVersionDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->export_version_data);
    pucIpv4MaskDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv4_mask_data);
    pucIpv6MaskDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->ipv6_mask_data);
    pucEnableDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->enable_data);

    /*消息类型为删除*/
    if (TRUE == pstSetIps->delete_struct)
    {

        /* 如果条目不存在，不删除 */
        if (ucIfExist == UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:delete nf aggr (key = %ld) is not in use\n", ulkey));
            OS_PRINTF("***ERROR***:delete nf aggr (key = %ld) is not in use\n", ulkey);

            OS_SPRINTF(ucMessage, "***ERROR***:delete nf aggr (key = %ld) is not in use\n", ulkey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;

            goto EXIT_LABEL;
        }
        
        /* 存在，删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "delete nf aggr key ;%ld", ulkey));

            if(NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }

            if (0 != pstCb->cnt) /*引用计数不为0*/
            {

                NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:can't del ip nf key ,cnt = %d\n",pstCb->cnt));
	
                OS_PRINTF("***ERROR***:can't del ip nf key ,cnt = %d\n",pstCb->cnt);

                OS_SPRINTF(ucMessage, "***ERROR***:can't del ip nf key ,cnt = %d\n",pstCb->cnt);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, DEL_UNEXIST_CONFIG_ERROR, "DEL_IP_NF CNT != 0", ucMessage));

                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;

                goto EXIT_LABEL;
                
            }

            AVLL_DELETE(SHARED.nf_aggr_tree, pstCb->nf_aggr_node);

            //释放ipnetflow表分配的内存空间
            ret = spm_free_nf_aggr_cb(pstCb);

            if (ATG_DCI_RC_OK != ret) /*释放内存出错*/
            {
                NBB_TRC_DETAIL((NBB_FORMAT "***FATAL ERROR***:spm_free_nf_aggr_cb ret=%ld\n", ret));
                
                OS_SPRINTF(ucMessage, "***FATAL ERROR***:spm_free_nf_aggr_cb ret=%ld\n", ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, 0, "FREE_MEMORY_ERROR", ucMessage));
                
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;

                goto EXIT_LABEL;
            }
        }
    }
    
    /*增加或者更新*/
    else
    {
        NBB_TRC_DETAIL((NBB_FORMAT "*****add/update nf aggr (key=%ld)*****\n", ulkey));

        OS_SPRINTF(ucMessage, "*****add/update nf aggr (key = %ld)*****\n", ulkey);
	    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == UNEXIST)
        {

            /* 申请一个新条目的内存空间 */
            pstCb = spm_alloc_nf_aggr_cb();

            //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstCb=%x !!!\n",pstCb);
            /*分配内存失败*/
            if (NULL == pstCb)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***:create mem for nf aggr key =%ld!\n",ulkey));
                
                OS_SPRINTF(ucMessage, "***ERROR***:create mem for ip nf aggr =%ld!\n",ulkey);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;

                goto EXIT_LABEL;
            }

        }

        if(NULL == pstCb)
        {
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }

        if (0 != pstCb->cnt) /*被引用*/
        {
            NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***:can't update and add,Cnt = %d\n",pstCb->cnt));
            
            OS_SPRINTF(ucMessage, "***ERROR***:can't update and add,Cnt = %d\n",pstCb->cnt);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | IP_NF_PD, 0, "ld d s s", ulkey, CONFIG_IS_IN_USE, "CONFIG_IS_IN_USE", ucMessage));

            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;

            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /* 聚合模式。*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_nf_aggr_aggregation_cfg(ulOperAggregation, &(pstCb->aggregation_cfg) ,

            pucAggregationDataStart);

        pstSetIps->aggregation_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /* 输出版本。 */
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_nf_aggr_version_cfg(ulOperVersion, &(pstCb->export_version_cfg) ,

            pucVersionDataStart);

        pstSetIps->export_version_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /* 掩码. */
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_nf_aggr_ipv4mask_cfg(ulOperIpv4Mask, &(pstCb->ipv4mask_cfg) ,

            pucIpv4MaskDataStart);

        pstSetIps->ipv4_mask_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /* 掩码. */
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_nf_aggr_ipv6mask_cfg(ulOperIpv6Mask, &(pstCb->ipv6mask_cfg) ,

            pucIpv6MaskDataStart);

        pstSetIps->ipv6_mask_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /* 使能。*/
        /***************************************************************************/
        ret = SUCCESS;
        ret = spm_set_nf_aggr_enable_cfg(ulOperEnable, &(pstCb->enable_cfg) ,

            pucEnableDataStart);

        pstSetIps->enable_return_code = ret;

        /*ips消息的返回值*/
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

    }

    pstSetIps->return_code = ips_ret;

    /* 如果是新增的条目，插入到树中 */
    if (UNEXIST == ucIfExist)
    {
        pstCb->nf_aggr_key = ulkey;

        //coverity[no_effect_test]
        AVLL_INSERT(SHARED.nf_aggr_tree, pstCb->nf_aggr_node);

        //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: retptr=%x !!!\n",retptr);
        //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstCb->nf_aggr_node.parent=%x !!!\n",pstCb->nf_aggr_node.parent);
        //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstCb->nf_aggr_node.left=%x !!!\n",pstCb->nf_aggr_node.left);
        //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstCb->nf_aggr_node.right=%x !!!\n",pstCb->nf_aggr_node.right);
        //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstCb->nf_aggr_node.l_height=%x !!!\n",pstCb->nf_aggr_node.l_height);
        //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstCb->nf_aggr_node.r_height=%x !!!\n",pstCb->nf_aggr_node.r_height);
        //NBB_PRINTF("spm_rcv_dci_set_nf_aggr: pstCb->nf_aggr_key=%x !!!\n",pstCb->nf_aggr_key);
    }

    spm_proc_nf_aggr_cfg(SPM_NF_ALL);

    /*异常跳出*/
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return;
}

/*物理接口的函数以及相关代码*/
#if 5

/*****************************************************************************
   函 数 名  : spm_set_ipv4_nf_enable
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/

NBB_LONG spm_set_ipv4_nf_enable(SUB_PORT *sub_port, ATG_DCI_PHY_PORT_IPV4NF_DATA *cfg)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
    int port = 0;
    int ingress_enable = 0;
    int egress_enable = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ipv4_nf_enable");

    if (NULL == cfg)
    {
        NBB_PRINTF("spm_set_ipv4_nf_enable cfg==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    spm_hardw_getslot(&fapid);

    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }
    port = (int)sub_port->port;
    ingress_enable = (int)cfg->in_enable_flag;
    egress_enable = (int)cfg->eg_enable_flag;
#ifdef SPU
    nfIPv4SwitchSet(port, ingress_enable, egress_enable);
#endif

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_set_ipv6_nf_enable
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_ipv6_nf_enable(SUB_PORT *sub_port, ATG_DCI_PHY_PORT_IPV6NF_DATA *cfg)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
    int port = 0;
    int ingress_enable = 0;
    int egress_enable = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_ipv6_nf_enable");

    if (NULL == cfg)
    {
        NBB_PRINTF("spm_set_ipv6_nf_enable cfg==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    spm_hardw_getslot(&fapid);

    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }
    port = (int)sub_port->port;
    ingress_enable = (int)cfg->in_enable_flag;
    egress_enable = (int)cfg->eg_enable_flag;
#ifdef SPU
    nfIPv6SwitchSet(port, ingress_enable, egress_enable);
#endif

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_set_lag_ipv4_nf_enable
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/

NBB_LONG spm_set_lag_ipv4_nf_enable(SUB_PORT *sub_port, ATG_DCI_LAG_IPV4NF_DATA *cfg)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
    int port = 0;
    int ingress_enable = 0;
    int egress_enable = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_lag_ipv4_nf_enable");

    if (NULL == cfg)
    {
        NBB_PRINTF("spm_set_lag_ipv4_nf_enable cfg==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if ((fapid != sub_port->slot) ||(sub_port->port <= 0x80 ))
    {
        goto EXIT_LABEL;
    }
    port = (int)sub_port->port;
    ingress_enable = (int)cfg->ingress_flag;
    egress_enable = (int)cfg->engress_flag;
#ifdef SPU
    nfIPv4SwitchSet_lag(port, ingress_enable, egress_enable);
#endif

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_set_lag_ipv6_nf_enable
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_set_lag_ipv6_nf_enable(SUB_PORT *sub_port, ATG_DCI_LAG_IPV6NF_DATA *cfg)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
    int port = 0;
    int ingress_enable = 0;
    int egress_enable = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_set_lag_ipv6_nf_enable");

    if (NULL == cfg)
    {
        NBB_PRINTF("spm_set_lag_ipv6_nf_enable cfg==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if ((fapid != sub_port->slot) ||(sub_port->port <= 0x80 ))
    {
        goto EXIT_LABEL;
    }
    port = (int)sub_port->port;
    ingress_enable = (int)cfg->ingress_flag;
    egress_enable = (int)cfg->engress_flag;
#ifdef SPU
    nfIPv6SwitchSet_lag(port, ingress_enable, egress_enable);
#endif

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return ret;

}

#endif

/*****************************************************************************
   函 数 名  : spm_rcv_dci_cmd_nf_ctrl
   功能描述  : 处理MAC地址撤销命令的IPS消息
   输入参数  : ATG_DCI_CMD_MAC_WITHDRAW *pstCmdMacWithdraw
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年8月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/

NBB_VOID spm_rcv_dci_cmd_nf_ctrl(ATG_DCI_CMD_NF_CTRL *pstCmdNfCtrl)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;

    NBB_INT ret = SUCCESS;

    ATG_DCI_NF_CTRL_RESET_NF *pstBasicData = NULL;

    NBB_BYTE *pucBasicDataStart = NULL;

    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;

    NBB_BYTE ulNfCtrlResetKey = 0;

    //NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_rcv_dci_cmd_nf_ctrl");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstCmdNfCtrl != NULL);

    ulNfCtrlResetKey = pstCmdNfCtrl->key;

    /* 获取子配置操作 */
    ulOperBasic = pstCmdNfCtrl->oper_reset_nf;

    /***************************************************************************/
    /*                        获取配置，逐条处理                               */
    /***************************************************************************/
    /* 基本配置 */
    if (ulOperBasic == ATG_DCI_OPER_ADD)
    {

        /* 计算第一个entry的地址。*/
        pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstCmdNfCtrl, &pstCmdNfCtrl->reset_nf_data);

        /* 如果指针为NULL，无数据 */
        if (pucBasicDataStart == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  NF ctrl basic_data is NULL."));
            NBB_EXCEPTION((PCT_SPM | 28, 0, "lx", SHARED.spm_index));
        }
        else
        {
            pstBasicData = (ATG_DCI_NF_CTRL_RESET_NF *)pucBasicDataStart;

            if (nf_ctrl_cfg_print_setting == SPM_PRINT_CFG)
            {
                OS_PRINTF("  1）基本配置\n");
                spm_dbg_print_nf_ctrl_basic_cfg(pstBasicData);
            }

            OS_SPRINTF(ucMessage, "  1）基本配置\n");
            BMU_SLOG(BMU_INFO, "log/netflow", ucMessage);

            //打印配置
            NBB_TRC_DETAIL((NBB_FORMAT "  enable flag=%d", pstBasicData->enable_flag));

            NBB_PRINTF("nf ctrl :enable_flag= %ld\n", pstBasicData->enable_flag);
#ifdef SPU
            nfMemReset();
#endif

        }
    }
    else if (ulOperBasic == ATG_DCI_OPER_DEL)
    {
        NBB_EXCEPTION((PCT_SPM | 28, 1, "lx", SHARED.spm_index));
    }
    else if (ulOperBasic == ATG_DCI_OPER_UPDATE)
    {
        NBB_EXCEPTION((PCT_SPM | 28, 2, "lx", SHARED.spm_index));
    }

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_init_global_cb
   功能描述  : 初始化全局配置结构体
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年4月12日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_nf_ctrl_cb()
{
    //NBB_BUF_SIZE avll_key_offset;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_init_nf_ctrl_cb");

    //avll_key_offset = NBB_OFFSETOF(SPM_SWITCH_VC_CB, switch_vc_key);

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    OS_MEMSET(&(SHARED.nf_ctrl_cb), 0, sizeof(SPM_NF_CTRL_CB));

    NBB_TRC_EXIT();
}

