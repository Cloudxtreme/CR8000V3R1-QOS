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

/* 全局打印变量qos_vc_cfg_print */
extern NBB_BYTE acl_pri_setting;

/* 全局打印变量qos_vc_cfg_print */
extern NBB_BYTE qos_acl_action_print;

/* 全局打印变量qos_vc_cfg_print */
extern NBB_BYTE qos_log_cfg_print;

/***************************************************************************/
/*   gloabl function                                                       */
/***************************************************************************/

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
SPM_QOS_POLICY_CB *spm_qos_find_policy_cb(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_CB *pcb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    pcb = AVLL_FIND(SHARED.qos_policy_tree, &index);
    if (NULL == pcb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return pcb;
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
NBB_LONG spm_disconnect_policy_cnt(NBB_ULONG index,NBB_BYTE wred_flag NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *pcb = NULL;
    SPM_QOS_POLICY_C2B_CB *c2b = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_policy_cnt err : ret = %ld, "
                  "policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_policy_cnt err : ret = %ld, "
                   "policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.qos_policy_tree, &index);
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_policy_cnt err : can't find policy cfg. "
                  "ret=%ld, policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_policy_cnt err : can't find policy cfg. "
                   "ret=%ld, policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    for (c2b = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pcb->c2b_tree); c2b != NULL;
         c2b = (SPM_QOS_POLICY_C2B_CB *)AVLL_NEXT(pcb->c2b_tree, c2b->spm_policy_c2b_node))
    {
        ret = spm_disconnect_action_cnt(c2b->c2b_key.qos_behavior_index,wred_flag NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        if ((NULL != pcb->basic_cfg_cb) && (0 != pcb->basic_cfg_cb->mode))
        {
            ret = spm_disconnect_classify_cnt(c2b->c2b_key.qos_classify_index NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;

                /* 异常退出 */
                goto EXIT_LABEL;
            }
        }
    }
    if (0 == pcb->cnt)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_disconnect_policy_cnt err : policy cnt is 0. "
                  "ret=%ld, policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_disconnect_policy_cnt err : policy cnt is 0. "
                   "ret=%ld, policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    (pcb->cnt)--;

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
NBB_LONG spm_connect_policy_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *pcb = NULL;
    SPM_QOS_POLICY_C2B_CB *c2b = NULL;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_connect_policy_cnt err : ret = %ld, "
                  "policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_connect_policy_cnt err : ret = %ld, "
                   "policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    pcb = AVLL_FIND(SHARED.qos_policy_tree, &index);
    if (NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_connect_policy_cnt err : can't find policy cfg. "
                  "ret = %ld, policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        OS_SPRINTF(ucMessage,"%s line=%d spm_connect_policy_cnt err : can't find policy cfg. "
                   "ret = %ld, policy_index = %ld\n\n",__FUNCTION__,__LINE__,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    for (c2b = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pcb->c2b_tree); c2b != NULL;
         c2b = (SPM_QOS_POLICY_C2B_CB *)AVLL_NEXT(pcb->c2b_tree, c2b->spm_policy_c2b_node))
    {
        ret = spm_connect_action_cnt(c2b->c2b_key.qos_behavior_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        if ((NULL != pcb->basic_cfg_cb) && (0 != pcb->basic_cfg_cb->mode))
        {
            ret = spm_connect_classify_cnt(c2b->c2b_key.qos_classify_index NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;

                /* 异常退出 */
                goto EXIT_LABEL;
            }
        }
    }
    (pcb->cnt)++;

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}




/***************************************************************************/
/*   help function                                                         */
/***************************************************************************/
#if 1

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
NBB_LONG spm_apply_classify_high_acl_id(NBB_ULONG *pid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    for (id = CLASSIFY_HIGH_ACL_ID; id < CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM; id++)
    {
        if (ATG_DCI_RC_OK == SHARED.g_classify_high_acl_id[id - CLASSIFY_HIGH_ACL_ID]) /*有空闲ID*/
        {
            *pid = id;
            SHARED.g_classify_high_acl_id[id - CLASSIFY_HIGH_ACL_ID] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
    }
    ret = ATG_DCI_RC_UNSUCCESSFUL;
    printf("%s line=%d spm_apply_classify_high_acl_id err: ret=%ld, "
              "g_classify_high_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,*pid);
    OS_SPRINTF(ucMessage,"%s line=%d spm_apply_classify_high_acl_id err: ret=%ld, "
               "g_classify_high_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,*pid);
    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
			       QOS_ERROR_STRING,ret,
			       ucMessage,__FUNCTION__,"LINE","",
			       0,0,__LINE__,0)); 

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
NBB_LONG spm_apply_classify_port_acl_id(NBB_ULONG *pid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    for (id = CLASSIFY_PORT_ACL_ID; id < CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM; id++)
    {
        if (ATG_DCI_RC_OK == SHARED.g_classify_port_acl_id[id - CLASSIFY_PORT_ACL_ID]) /*有空闲ID*/
        {
            *pid = id;
            SHARED.g_classify_port_acl_id[id - CLASSIFY_PORT_ACL_ID] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
    }
    ret = ATG_DCI_RC_UNSUCCESSFUL;
    printf("%s line=%d spm_apply_classify_port_acl_id err: ret=%ld, "
              "g_classify_port_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,*pid);
    OS_SPRINTF(ucMessage,"%s line=%d spm_apply_classify_port_acl_id err: ret=%ld, "
               "g_classify_port_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,*pid);
    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
			       QOS_ERROR_STRING,ret,
			       ucMessage,__FUNCTION__,"LINE","",
			       0,0,__LINE__,0)); 

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
NBB_LONG spm_apply_classify_eth_acl_id(NBB_ULONG *pid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    for (id = CLASSIFY_ETH_ACL_ID; id < CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM; id++)
    {
        if (ATG_DCI_RC_OK == SHARED.g_classify_eth_acl_id[id - CLASSIFY_ETH_ACL_ID]) /*有空闲ID*/
        {
            *pid = id;
            SHARED.g_classify_eth_acl_id[id - CLASSIFY_ETH_ACL_ID] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
    }
    ret = ATG_DCI_RC_UNSUCCESSFUL;
    printf("%s line=%d spm_apply_classify_eth_acl_id err: ret=%ld, "
              "g_classify_eth_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,*pid);
    OS_SPRINTF(ucMessage,"%s line=%d spm_apply_classify_eth_acl_id err: ret=%ld, "
               "g_classify_eth_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,*pid);
    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
			       QOS_ERROR_STRING,ret,
			       ucMessage,__FUNCTION__,"LINE","",
			       0,0,__LINE__,0)); 

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
NBB_LONG spm_release_classify_high_acl_id(NBB_ULONG pid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((CLASSIFY_HIGH_ACL_ID > pid) || (pid >= (CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM)))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_release_classify_high_acl_id err:out of range. "
                  "ret=%ld, g_classify_high_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        OS_SPRINTF(ucMessage,"%s line=%d spm_release_classify_high_acl_id err:out of range. "
                   "ret=%ld, g_classify_high_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    if (ATG_DCI_RC_OK != SHARED.g_classify_high_acl_id[pid - CLASSIFY_HIGH_ACL_ID])
    {
        SHARED.g_classify_high_acl_id[pid - CLASSIFY_HIGH_ACL_ID] = ATG_DCI_RC_OK;

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_release_classify_high_acl_id err:high classify acl id is not in use. "
                  "ret=%ld, g_classify_high_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        OS_SPRINTF(ucMessage,"%s line=%d spm_release_classify_high_acl_id err:high classify acl id is not in use. "
                   "ret=%ld, g_classify_high_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
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
NBB_LONG spm_release_classify_port_acl_id(NBB_ULONG pid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((CLASSIFY_PORT_ACL_ID > pid) || (pid >= (CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM)))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_release_classify_port_acl_id err:out of range. "
                  "ret=%ld, g_classify_port_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        OS_SPRINTF(ucMessage,"%s line=%d spm_release_classify_port_acl_id err:out of range. "
                   "ret=%ld, g_classify_port_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    if (ATG_DCI_RC_OK != SHARED.g_classify_port_acl_id[pid - CLASSIFY_PORT_ACL_ID])
    {
        SHARED.g_classify_port_acl_id[pid - CLASSIFY_PORT_ACL_ID] = ATG_DCI_RC_OK;

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_release_classify_port_acl_id err:port classify acl id is not in use. "
                  "ret=%ld, g_classify_port_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        OS_SPRINTF(ucMessage,"%s line=%d spm_release_classify_port_acl_id err:port classify acl id is not in use. "
                   "ret=%ld, g_classify_port_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
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
NBB_LONG spm_release_classify_eth_acl_id(NBB_ULONG pid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((CLASSIFY_ETH_ACL_ID > pid) || (pid >= (CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM)))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_release_classify_eth_acl_id err:out of range. "
                  "ret=%ld, g_classify_eth_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        OS_SPRINTF(ucMessage,"%s line=%d spm_release_classify_eth_acl_id err:out of range. "
                   "ret=%ld, g_classify_eth_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    if (ATG_DCI_RC_OK != SHARED.g_classify_eth_acl_id[pid - CLASSIFY_ETH_ACL_ID])
    {
        SHARED.g_classify_eth_acl_id[pid - CLASSIFY_ETH_ACL_ID] = ATG_DCI_RC_OK;

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    else
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_release_classify_eth_acl_id err:eth classify acl id is not in use. "
                  "ret=%ld, g_classify_eth_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        OS_SPRINTF(ucMessage,"%s line=%d spm_release_classify_eth_acl_id err:eth classify acl id is not in use. "
                   "ret=%ld, g_classify_eth_acl_id=%ld\n\n",__FUNCTION__,__LINE__,ret,pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,__FUNCTION__,"LINE","",
    			       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL;
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
NBB_LONG spm_release_policy_acl_id(NBB_ULONG acl_id NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG ret = ATG_DCI_RC_OK;
    
    if((acl_id >= CLASSIFY_PORT_ACL_ID) && (acl_id < CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM))
    {
        spm_release_classify_port_acl_id(acl_id NBB_CCXT);    
    }
    else if((acl_id >= CLASSIFY_ETH_ACL_ID) && (acl_id < CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM))
    {
        spm_release_classify_eth_acl_id(acl_id NBB_CCXT); 
    } 
    else if((acl_id >= CLASSIFY_HIGH_ACL_ID) && (acl_id < CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM))
    {
        spm_release_classify_high_acl_id(acl_id NBB_CCXT);    
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
SPM_QOS_POLICY_C2B_CB* spm_alloc_c2b_cb(ATG_DCI_QOS_POLICY_CB2QOS_DATA *ulkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_C2B_CB *pstC2B = NULL;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == ulkey)
    {
    
        /* 异常退出 */
        goto EXIT_LABEL;
    }
    pstC2B = (SPM_QOS_POLICY_C2B_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_POLICY_C2B_CB),
              NBB_NORETRY_ACT,MEM_SPM_POLICY_C2B_CB);
    if(NULL == pstC2B)
    {

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(pstC2B, 0, sizeof(SPM_QOS_POLICY_C2B_CB));
    OS_MEMCPY(&(pstC2B->c2b_key), ulkey, sizeof(ATG_DCI_QOS_POLICY_CB2QOS_DATA));
    AVLL_INIT_NODE(pstC2B->spm_policy_c2b_node);

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstC2B);
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
SPM_QOS_POLICY_CB *spm_alloc_policy_cb(NBB_ULONG ulkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_CB *pstTbl = NULL;
    NBB_BUF_SIZE avll_key_offset;
    NBB_USHORT i = 0;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    pstTbl = (SPM_QOS_POLICY_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_POLICY_CB),
              NBB_NORETRY_ACT,MEM_SPM_POLICY_CB);
    if (pstTbl == NULL)
    {

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_POLICY_CB));
    pstTbl->policy_key = ulkey;

    AVLL_INIT_NODE(pstTbl->spm_policy_node);

    /* c2b树的初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_POLICY_C2B_CB, c2b_key);
    AVLL_INIT_TREE(pstTbl->c2b_tree,spm_qos_c2b_comp,(NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_POLICY_C2B_CB, spm_policy_c2b_node));

    /* 引用classify规则的逻辑端口树的初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, key);
    AVLL_INIT_TREE(pstTbl->instance_tree,compare_ulong,(NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, spm_classify_id_node));
    
#ifdef PTN690
    for(i = 0;i < POLICY_C2B_INSTANCE_MAX;i++)
    {
        avll_key_offset = NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, key);
        AVLL_INIT_TREE(pstTbl->c2b_instance_tree[i].instance_tree,spm_qos_logic_key_compare,(NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_FLOW_CLASSIFY_CB, spm_classify_id_node));
    }  
#endif

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
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
NBB_LONG spm_free_policy_c2b_cb(SPM_QOS_POLICY_C2B_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    //SPM_QOS_POLICY_C2B_CB *pcb = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != pstTbl);

    if (NULL == pstTbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_POLICY_C2B_CB), MEM_SPM_POLICY_C2B_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_POLICY_C2B_CB);
    pstTbl = NULL;

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
NBB_LONG spm_free_policy_cb(SPM_QOS_POLICY_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_C2B_CB *pcb = NULL;
    
    //SPM_QOS_FLOW_CLASSIFY_CB *cfg_cb = NULL;
    //NBB_USHORT i = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != pstTbl);

    if (NULL == pstTbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_POLICY_CB), MEM_SPM_POLICY_CB);

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    if (NULL != pstTbl->basic_cfg_cb)
    {         
        NBB_MM_FREE(pstTbl->basic_cfg_cb, MEM_SPM_POLICY_BASIC_CB);
        pstTbl->basic_cfg_cb = NULL;
    }
#if 0
#ifdef PTN690

    /* 在删除policy策略模板之前,先检查非共享模式policy引用树(c2b_instance_tree)是否为空 */
    for(i = 0;i < POLICY_C2B_INSTANCE_MAX;i++)
    {
        for(cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(pstTbl->c2b_instance_tree[i].instance_tree);
            NULL != cfg_cb;
            cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(pstTbl->c2b_instance_tree[i].instance_tree))
        {
            AVLL_DELETE(pstTbl->c2b_instance_tree[i].instance_tree,cfg_cb->spm_classify_id_node);
            
            /* 清内存?*/
        }
    } 
#endif

    /* 在删除policy策略模板之前,先检查共享模式policy引用树(instance_tree)是否为空 */
    for(cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(pstTbl->instance_tree);
        NULL != cfg_cb;
        cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(pstTbl->instance_tree))
    {
        AVLL_DELETE(pstTbl->instance_tree,cfg_cb->spm_classify_id_node);
    }   
#endif

    /* 删除policy模板中的cb对信息 */
    for(pcb = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pstTbl->c2b_tree); pcb != NULL;
        pcb = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pstTbl->c2b_tree))
    {
        SHARED.g_classify_id_instance[pcb->c2b_key.qos_classify_index].match_type = MATCH_ERROR;
        AVLL_DELETE(pstTbl->c2b_tree, pcb->spm_policy_c2b_node);
        NBB_MM_FREE(pcb, MEM_SPM_POLICY_C2B_CB);
    }

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_POLICY_CB);
    pstTbl = NULL;

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


/***************************************************************************/
/*   ips handle function                                                   */
/***************************************************************************/
#if 2

#ifdef PTN690

/*ATG_DCI_RC_OK表示没有被占用*/

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
NBB_LONG spm_check_policy_instanced(SPM_QOS_POLICY_CB *pstCb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL != pstCb)
    {
        if(0 != pstCb->cnt)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }

       /************************************************************* 
        * POLICY_C2B_INSTANCE_MAX:表示policy策略允许被绑定的最大次  *
        * 数(128)的倍数.一个meter桶最多可以被绑定128,因此一个policy *
        * 允许被绑定的最大次数为128*POLICY_C2B_INSTANCE_MAX次.      *
        *************************************************************/
        for(i = 0; i < POLICY_C2B_INSTANCE_MAX;i++)
        {
            /* 非共享模式policy是否被绑定 */
            if(NULL != AVLL_FIRST(pstCb->c2b_instance_tree[i].instance_tree))
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;

                /* 异常退出 */
                goto EXIT_LABEL;
            }
        }

        /* 共享模式policy是否被绑定 */
        if(NULL != AVLL_FIRST(pstCb->instance_tree))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
    } 

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*删除树的连接关系*/
#if 0
NBB_VOID spm_del_policy_instance_tree_node(SPM_QOS_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    SPM_QOS_FLOW_CLASSIFY_CB *cfg_cb = NULL;
    NBB_ULONG i = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);
    
    if(NULL != pcb)
    {
        for(i = 0; i < POLICY_C2B_INSTANCE_MAX;i++)
        {
            for(cfg_cb =(SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_FIRST(
                pcb->c2b_instance_tree[i].instance_tree);
                cfg_cb != NULL;cfg_cb = (SPM_QOS_POLICY_SUPERVISE_CB*)AVLL_NEXT(
                pcb->c2b_instance_tree[i].instance_tree,cfg_cb->spm_classify_id_node))
            {
                AVLL_DELETE(pcb->c2b_instance_tree[i].instance_tree,cfg_cb->spm_classify_id_node);
            }
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}

NBB_LONG spm_del_policy_c2b_drive(NBB_ULONG oper,
    ATG_DCI_QOS_POLICY_BASIC_DATA **pcb, NBB_BYTE *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *pstCb = NULL;
    SPM_QOS_POLICY_C2B_CB *pstC2B = NULL;
    SPM_QOS_CLASSIFY_CB *pstClassify = NULL;
    SPM_QOS_ACTION_CB *pstAction = NULL;
    SPM_QOS_POLICY_CLASSIFY_CB *pstPlicyClassify = NULL;
    SPM_QOS_ACL_CB* pstAcl = NULL;
    NBB_USHORT unit = 0;
    NBB_LONG acl_flag = 0;
    NBB_ULONG classify_index = 0;
    NBB_ULONG action_index = 0;


    NBB_TRC_ENTRY(__FUNCTION__);

    
            
    for(pstC2B = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pstCb->c2b_tree); pstC2B != NULL;
                pstC2B = (SPM_QOS_POLICY_C2B_CB *)AVLL_NEXT(pstCb->c2b_tree, pstC2B->spm_policy_c2b_node))
    {
        classify_index = pstC2B->c2b_key.qos_classify_index;
        action_index = pstC2B->c2b_key.qos_behavior_index;

        /* 删除ACL如果是引用ACL不删除只解除action动作的绑定未做 */
        for (pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((SHARED.g_classify_id_instance[classify_index].classify_group));
             pstClassify != NULL; pstClassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(pstClassify->spm_classify_group_node))
        {
            for(pstPlicyClassify = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstClassify->spm_classify_policy_tree);
                pstPlicyClassify != NULL;
                pstPlicyClassify = (SPM_QOS_POLICY_CLASSIFY_CB *)AVLL_NEXT(
                    pstClassify->spm_classify_policy_tree, pstPlicyClassify->tree_node))
            {
                
            }
            acl.mAclId = pstCb->acl_id;
            acl.mRuleId = pstClassify->rule_id;
            if(NULL != pstClassify->basic_cfg_cb)
            {   
               /*********************************************************************** 
                * 此时Classify规则的匹配模式是匹配AclId;若Policy策略中有Classify规则  *
                * 引用Acl,则该Policy策略中只有唯一的一个C2B对,该C2B对中ClassifyId只有 *
                * 唯一的一条Classify规则,该规则是引用Acl的.                           *
                ***********************************************************************/
                if(MATCH_ACL_ID == pstClassify->basic_cfg_cb->match_type) 
                {
                    for(pstAcl = (SPM_QOS_ACL_CB*)NBB_NEXT_IN_LIST((SHARED.g_acl_id_instance[pstClassify->basic_cfg_cb->acl_id].acl_group));
                        pstAcl != NULL; pstAcl= (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pstAcl->spm_acl_group_node))
                    {
                        acl.mAclId = pstAcl->acl_key.acl_id;
                        acl.mRuleId = pstAcl->acl_key.rule_id;

                        /* 恢复动作 */
                        if(0 == pstAcl->basic_cfg_cb->action)
                        {
                            acl.eAclAction = ACL_ACTION_DROP;
                        }
                        else if(1 == pstAcl->basic_cfg_cb->action)
                        {
                            acl.eAclAction = ACL_ACTION_PASS;
                        }
                        for(unit = 0; unit < SHARED.c3_num;unit++)
                        {
#if defined (SPU) || defined (PTN690_CES)
                            /* 若C2B对中Classify是引用Acl的,在删除Policy时需要将Acl还原 */
                            //coverity[dead_error_condition]
                            ret = ApiC3SetAcl(unit,&acl);
                            if(ATG_DCI_RC_OK != ret)
                            {
                                spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }
#endif
                        } 
                    }
                    acl_flag = ATG_DCI_RC_UNSUCCESSFUL;
                }

                /* 此时classify的匹配模式不是匹配AclId,与基础配置里的匹配类型(以太网头/IP等等)相关 */
                else
                {
                    for(unit = 0; unit < SHARED.c3_num;unit++)
                    {
#if defined (SPU) || defined (PTN690_CES)
                        //coverity[dead_error_condition]
                        ret = ApiC3DelAcl(unit,&acl);
                        if(ATG_DCI_RC_OK != ret)
                        {
                            spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }
#endif
                    } 
                }
            }
        }
    } 

    /* 多c2b对模式,同时classify规则的匹配模式是非引用AclId的模式:释放为policy分配的aclid */
    if((0 != pstCb->basic_cfg_cb->mode)&&(ATG_DCI_RC_OK == acl_flag))
    {
        
        if((pstCb->acl_id >= CLASSIFY_PORT_ACL_ID) && (pstCb->acl_id < CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM))
        {
            spm_release_classify_port_acl_id(pstCb->acl_id NBB_CCXT);    
        }
        else if((pstCb->acl_id >= CLASSIFY_ETH_ACL_ID) && (pstCb->acl_id < CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM))
        {
            spm_release_classify_eth_acl_id(pstCb->acl_id NBB_CCXT); 
        } 
        else if((pstCb->acl_id >= CLASSIFY_HIGH_ACL_ID) &&( pstCb->acl_id < CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM))
        {
            spm_release_classify_high_acl_id(pstCb->acl_id NBB_CCXT);    
        }
    }

            
    

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


NBB_VOID spm_del_policy_drive(SPM_QOS_POLICY_CB *pstCb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *pstCb = NULL;
    SPM_QOS_POLICY_C2B_CB *pstC2B = NULL;
    SPM_QOS_CLASSIFY_CB *pstClassify = NULL;
    SPM_QOS_ACTION_CB *pstAction = NULL;
    SPM_QOS_POLICY_CLASSIFY_CB *pstPlicyClassify = NULL;
    NBB_ULONG classify_index = 0;
    NBB_ULONG action_index = 0;


    NBB_TRC_ENTRY(__FUNCTION__);

    
            
    for(pstC2B = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pstCb->c2b_tree); pstC2B != NULL;
                pstC2B = (SPM_QOS_POLICY_C2B_CB *)AVLL_NEXT(pstCb->c2b_tree, pstC2B->spm_policy_c2b_node))
    {
        classify_index = pstC2B->c2b_key.qos_classify_index;
        action_index = pstC2B->c2b_key.qos_behavior_index;

        /* 删除ACL如果是引用ACL不删除只解除action动作的绑定未做 */
        for (pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((SHARED.g_classify_id_instance[classify_index].classify_group));
             pstClassify != NULL; pstClassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(pstClassify->spm_classify_group_node))
        {
            for(pstPlicyClassify = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pstClassify->spm_classify_policy_tree);
                pstPlicyClassify != NULL;
                pstPlicyClassify = (SPM_QOS_POLICY_CLASSIFY_CB *)AVLL_NEXT(
                    pstClassify->spm_classify_policy_tree, pstPlicyClassify->tree_node))
            {
                
            }
            acl.mAclId = pstCb->acl_id;
            acl.mRuleId = pstClassify->rule_id;
            if(NULL != pstClassify->basic_cfg_cb)
            {   
               /*********************************************************************** 
                * 此时Classify规则的匹配模式是匹配AclId;若Policy策略中有Classify规则  *
                * 引用Acl,则该Policy策略中只有唯一的一个C2B对,该C2B对中ClassifyId只有 *
                * 唯一的一条Classify规则,该规则是引用Acl的.                           *
                ***********************************************************************/
                if(MATCH_ACL_ID == pstClassify->basic_cfg_cb->match_type) 
                {
                    for(pstAcl = (SPM_QOS_ACL_CB*)NBB_NEXT_IN_LIST((SHARED.g_acl_id_instance[pstClassify->basic_cfg_cb->acl_id].acl_group));
                        pstAcl != NULL; pstAcl= (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pstAcl->spm_acl_group_node))
                    {
                        acl.mAclId = pstAcl->acl_key.acl_id;
                        acl.mRuleId = pstAcl->acl_key.rule_id;

                        /* 恢复动作 */
                        if(0 == pstAcl->basic_cfg_cb->action)
                        {
                            acl.eAclAction = ACL_ACTION_DROP;
                        }
                        else if(1 == pstAcl->basic_cfg_cb->action)
                        {
                            acl.eAclAction = ACL_ACTION_PASS;
                        }
                        for(unit = 0; unit < SHARED.c3_num;unit++)
                        {
#if defined (SPU) || defined (PTN690_CES)
                            /* 若C2B对中Classify是引用Acl的,在删除Policy时需要将Acl还原 */
                            //coverity[dead_error_condition]
                            ret = ApiC3SetAcl(unit,&acl);
                            if(ATG_DCI_RC_OK != ret)
                            {
                                spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }
#endif
                        } 
                    }
                    acl_flag = ATG_DCI_RC_UNSUCCESSFUL;
                }

                /* 此时classify的匹配模式不是匹配AclId,与基础配置里的匹配类型(以太网头/IP等等)相关 */
                else
                {
                    for(unit = 0; unit < SHARED.c3_num;unit++)
                    {
#if defined (SPU) || defined (PTN690_CES)
                        //coverity[dead_error_condition]
                        ret = ApiC3DelAcl(unit,&acl);
                        if(ATG_DCI_RC_OK != ret)
                        {
                            spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }
#endif
                    } 
                }
            }
        }
    } 

    /* 多c2b对模式,同时classify规则的匹配模式是非引用AclId的模式:释放为policy分配的aclid */
    if((0 != pstCb->basic_cfg_cb->mode)&&(ATG_DCI_RC_OK == acl_flag))
    {
        
        if((pstCb->acl_id >= CLASSIFY_PORT_ACL_ID) && (pstCb->acl_id < CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM))
        {
            spm_release_classify_port_acl_id(pstCb->acl_id NBB_CCXT);    
        }
        else if((pstCb->acl_id >= CLASSIFY_ETH_ACL_ID) && (pstCb->acl_id < CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM))
        {
            spm_release_classify_eth_acl_id(pstCb->acl_id NBB_CCXT); 
        } 
        else if((pstCb->acl_id >= CLASSIFY_HIGH_ACL_ID) && (pstCb->acl_id < CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM))
        {
            spm_release_classify_high_acl_id(pstCb->acl_id NBB_CCXT);    
        }
    }

            
    

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}
#endif

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
NBB_LONG spm_set_policy_basic_cfg(SPM_QOS_POLICY_CB *pstCb,NBB_ULONG oper, 
            ATG_DCI_QOS_POLICY_BASIC_DATA *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:

        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == buf)
        {
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            /* 判断policy策略模板是否被引用 */
            ret  = spm_check_policy_instanced(pstCb NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                ret = ATG_DCI_RC_UNSUCCESSFUL;

                /* 异常退出 */
                goto EXIT_LABEL;
            }

            /***************************************************************************/
            /*                          存配置                                         */
            /***************************************************************************/
            if (NULL == pstCb->basic_cfg_cb)
            {
                pstCb->basic_cfg_cb = (ATG_DCI_QOS_POLICY_BASIC_DATA *)NBB_MM_ALLOC(    \
                        sizeof(ATG_DCI_QOS_POLICY_BASIC_DATA), NBB_NORETRY_ACT, MEM_SPM_POLICY_BASIC_CB);
                if (NULL == pstCb->basic_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;

                    /* 异常退出 */
                    goto EXIT_LABEL;
                } 
            }
            OS_MEMCPY(pstCb->basic_cfg_cb, buf, sizeof(ATG_DCI_QOS_POLICY_BASIC_DATA));
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
NBB_LONG spm_set_policy_c2b_cfg(SPM_QOS_POLICY_CB *pstCb,NBB_ULONG oper,
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_C2B_CB *pstC2B = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstCb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    
    //coverity[var_deref_model]
    pstC2B = AVLL_FIND(pstCb->c2b_tree, buf);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        break;

        case  ATG_DCI_OPER_DEL:
        if (NULL == buf)
        {
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }

        spm_dbg_record_qos_policy_c2b_head(buf,SPM_OPER_DEL NBB_CCXT);

        /* 存在则进行删除处理 */
        if(NULL != pstC2B)
        {
            /* 目前暂不支持在已经绑定的policy策略中删除其中的cb对 */
            ret  = spm_check_policy_instanced(pstCb NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                ret = ATG_DCI_RC_UNSUCCESSFUL;

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            else
            {
                AVLL_DELETE(pstCb->c2b_tree, pstC2B->spm_policy_c2b_node);
                spm_free_policy_c2b_cb(pstC2B NBB_CCXT);
                pstC2B = NULL;
            } 
        }
        break;
        
        case  ATG_DCI_OPER_ADD:
        if (NULL == buf)
        {
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }

        spm_dbg_record_qos_policy_c2b_head(buf,SPM_OPER_ADD NBB_CCXT);

        /* 新增CB对 */
        if(NULL == pstC2B)
        {

            /* 目前暂不支持在已经绑定的policy策略中添加新的cb对 */
            ret  = spm_check_policy_instanced(pstCb NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                ret = ATG_DCI_RC_UNSUCCESSFUL;

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            else
            {
                pstC2B = spm_alloc_c2b_cb(buf NBB_CCXT);
                if(NULL == pstC2B)
                {
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    ret = ATG_DCI_RC_UNSUCCESSFUL;

                    /* 异常退出 */
                    goto EXIT_LABEL;
                }

                rv = AVLL_INSERT(pstCb->c2b_tree, pstC2B->spm_policy_c2b_node);
            }
        }

        /* 相同的配置 */
        else
        {
            spm_dbg_record_qos_policy_c2b_head(buf,SPM_OPER_UPD NBB_CCXT);
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
NBB_VOID spm_rcv_dci_set_policy(ATG_DCI_SET_QOS_POLICY *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ulkey = 0;
    SPM_QOS_POLICY_CB *pstCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    ATG_DCI_QOS_POLICY_BASIC_DATA *pstBasicData = NULL;
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *pstC2bData = NULL;
    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucC2bDataStart = NULL;
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperC2b = ATG_DCI_OPER_NULL;
    NBB_USHORT c2bnum = 0;
    NBB_USHORT i = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    pstSetIps->return_code = ATG_DCI_RC_OK;

    ulkey = pstSetIps->key;
    c2bnum = pstSetIps->cb2qos_num; 

    /* 获取配置的操作码 */
    ulOperBasic = pstSetIps->oper_basic;
    ulOperC2b = pstSetIps->oper_cb2qos;

    /* 获取配置的偏移地址 */
    pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->basic_data);
    pucC2bDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->cb2qos_data);
    pstBasicData = (ATG_DCI_QOS_POLICY_BASIC_DATA *)pucBasicDataStart;
    pstC2bData = (ATG_DCI_QOS_POLICY_CB2QOS_DATA*)pucC2bDataStart;
        
    pstCb = AVLL_FIND(SHARED.qos_policy_tree, &ulkey);

    /* ips消息为删除 */
    if (TRUE == pstSetIps->delete_struct)
    {
        spm_dbg_record_qos_policy_head(&ulkey,SPM_OPER_DEL NBB_CCXT);
        
        /* 如果条目不存在,不删除 返回OK*/
        if (NULL == pstCb)
        {

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        
        /* 存在,删除 */
        else
        {
            /* 判断是否被引用 */
            ret  = spm_check_policy_instanced(pstCb NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            AVLL_DELETE(SHARED.qos_policy_tree, pstCb->spm_policy_node);
            spm_free_policy_cb(pstCb NBB_CCXT);
            pstCb = NULL;
        }
    }
    
    /* 增加或者更新 */
    else
    {
        /* 增加 */
        if (NULL == pstCb)
        {
            spm_dbg_record_qos_policy_head(&ulkey,SPM_OPER_ADD NBB_CCXT);
            pstCb = spm_alloc_policy_cb(ulkey NBB_CCXT);
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            
            //coverity[no_effect_test]
            rv = AVLL_INSERT(SHARED.qos_policy_tree, pstCb->spm_policy_node);
        }

        /* 更新 */
        else
        {
            spm_dbg_record_qos_policy_head(&ulkey,SPM_OPER_UPD NBB_CCXT);
        }

        /* 基本配置 */
        ret = spm_set_policy_basic_cfg(pstCb,ulOperBasic,pstBasicData NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }

        /* C2B配置 */
        for (i = 0; ((i < c2bnum) && (NULL != pucC2bDataStart)); i++)
        {
            pstC2bData = (ATG_DCI_QOS_POLICY_CB2QOS_DATA *)
               (pucC2bDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_QOS_POLICY_CB2QOS_DATA))) * i);
            ret = spm_set_policy_c2b_cfg(pstCb,ulOperC2b,pstC2bData NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            }
            else
            {
                if(ATG_DCI_OPER_DEL == ulOperC2b)
                {
                    (pstCb->c2b_num)--; 
                }
                else
                {
                    (pstCb->c2b_num)++; 
                }
            }
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}

#else

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
NBB_LONG spm_set_policy_by_acl(SPM_QOS_POLICY_CB *pstPolicy,
    SPM_QOS_ACTION_CB *pstAction NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_ACL_CB *pstAcl = NULL;
    ACL_T acl = {0};
    NBB_USHORT unit = 0;
    NBB_ULONG nhi = 0;
    SPM_PORT_INFO_CB stPortInfo = {0};
    NBB_USHORT i = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstPolicy)
    {
         printf("pstPolicy == NULL\n");
         NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
             			"QOS pstPolicy == NULL",ATG_DCI_RC_UNSUCCESSFUL,
             			"","","","",0,0,0,0));
         ret = ATG_DCI_RC_UNSUCCESSFUL;

         /* 异常退出 */
         goto EXIT_LABEL; 
    }
    if(NULL == pstAction)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line = %d spm_set_policy_by_acl err : pstAction "
                  "is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
        OS_SPRINTF(ucMessage,"%s line = %d spm_set_policy_by_acl err : pstAction "
                   "is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        		       QOS_ERROR_STRING,ret,
        		       ucMessage,__FUNCTION__,"LINE","",
        		       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL; 
    }
    for(pstAcl = (SPM_QOS_ACL_CB*)NBB_NEXT_IN_LIST((SHARED.g_acl_id_instance[pstPolicy->acl_id].acl_group));
        pstAcl != NULL; pstAcl = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pstAcl->spm_acl_group_node))
    {
        acl.mAclId = pstAcl->acl_key.acl_id;
        acl.mRuleId = pstAcl->acl_key.rule_id;
        if(NULL == pstAction->flow_act_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_policy_by_acl err : flow_act_cfg_cb "
                      "of pstAction is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_by_acl err : flow_act_cfg_cb "
                       "of pstAction is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            		       QOS_ERROR_STRING,ret,
            		       ucMessage,__FUNCTION__,"LINE","",
            		       0,0,__LINE__,0));

            /* 异常退出 */
            goto EXIT_LABEL; 
        }
        if(0 == pstAction->flow_act_cfg_cb->action)
        {
            acl.eAclAction = ACL_ACTION_DROP;
        }
        else if(1 == pstAction->flow_act_cfg_cb->action)
        {
            acl.eAclAction = ACL_ACTION_PASS;
        }

        /* 重定向 */
        else if(3 == pstAction->flow_act_cfg_cb->action)
        {
            if(NULL == pstAction->redirect_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("%s line=%d spm_set_policy_by_acl err : redirect_cfg_cb "
                          "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", __FUNCTION__,
                          __LINE__, ret, pstAction->action_key);
                OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_by_acl err : redirect_cfg_cb "
                           "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", __FUNCTION__,
                           __LINE__, ret, pstAction->action_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                		       QOS_ERROR_STRING,ret,
                		       ucMessage,__FUNCTION__,"LINE","",
                		       0,0,__LINE__,0)); 

                /* 异常退出 */
                goto EXIT_LABEL;
            }

            /* 关闭重定向 */
            if(0 == pstAction->redirect_cfg_cb->action)
            {
                acl.eAclAction = ACL_ACTION_PASS;
            }

            /* IP静荷重定向到VRF */
            else if(1 == pstAction->redirect_cfg_cb->action)
            {
                acl.eAclAction = ACL_ACTION_VRF;
                acl.vrfId = pstAction->redirect_cfg_cb->vrf_d;
            }

            /* 原始报文重定向到端口 */
            else if(2 == pstAction->redirect_cfg_cb->action)
            {
                if(0 == pstAction->redirect_cfg_cb->out_port_index)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    printf("%s line=%d spm_set_policy_by_acl err : redirect_cfg_cb "
                              "of pstAction out_port_index is 0. ret = %ld, action_id = %ld\n\n", 
                              __FUNCTION__, __LINE__, ret, pstAction->action_key);
                    OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_by_acl err : redirect_cfg_cb "
                               "of pstAction out_port_index is 0. ret = %ld, action_id = %ld\n\n", 
                               __FUNCTION__, __LINE__, ret, pstAction->action_key);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    		       QOS_ERROR_STRING,ret,
                    		       ucMessage,__FUNCTION__,"LINE","",
                    		       0,0,__LINE__,0)); 

                    /* 异常退出 */
                    goto EXIT_LABEL;
                }
                ret = spm_get_portid_from_logical_port_index(   \
                            pstAction->redirect_cfg_cb->out_port_index,&stPortInfo NBB_CCXT);
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_get_portid_from_logical_port_index_error_log(pstAction->redirect_cfg_cb->out_port_index,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);

                    /* 异常退出 */
                    goto EXIT_LABEL;
                }
                acl.slot = stPortInfo.slot_id;
                acl.cardPort = stPortInfo.port_id;
                acl.eAclAction = ACL_ACTION_REDIRECTION;
            }
        }

        /* PolicyRouting策略路由 */
        else if(4 == pstAction->flow_act_cfg_cb->action)
        {
            acl.eAclAction = ACL_ACTION_NHI;
            if(NULL == pstAction->policy_nhi_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("%s line=%d spm_set_policy_by_acl err : policy_nhi_cfg_cb "
                          "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", __FUNCTION__,
                          __LINE__, ret, pstAction->action_key);
                OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_by_acl err : policy_nhi_cfg_cb "
                           "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", __FUNCTION__,
                           __LINE__, ret, pstAction->action_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                		       QOS_ERROR_STRING,ret,
                		       ucMessage,__FUNCTION__,"LINE","",
                		       0,0,__LINE__,0)); 

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            for(i = 0;i < pstAction->policy_nhi_cfg_cb->nxhop_num;i++)
            {
                /* IPV4 */
                if(0 == pstAction->policy_nhi_cfg_cb->nxhp[i].ip_type)
                {
                    ret = spm_l3_lookupuni(0, &(pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_ip[3]),
                    pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,&nhi NBB_CCXT);
                }

                /* IPV6 */
                else
                {
                    ret = spm_l3_lookupuni(1, &(pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_ip[0]),
                    pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,&nhi NBB_CCXT);
                }
                if(ATG_DCI_RC_OK != ret)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_l3_lookupuni_error_log(acl.mAclId,acl.mRuleId,pstAction->action_key,__FUNCTION__,
                           __LINE__,pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,ret NBB_CCXT);

                    /* 异常退出 */
                    goto EXIT_LABEL;
                }
                acl.nhIdx = nhi;
            }
        }   

        /* 配置动作 */
        if(0 != pstAction->meter_id)
        {
            acl.meterIdx = pstAction->meter_id;
            acl.flags |= ACL_METER_ON;

            if(NULL == pstAction->supervise_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            if(1 == pstAction->supervise_cfg_cb->cm_color)
            {
                acl.flags |= ACL_COLOR_AWARE;
            }
            if(2 == pstAction->supervise_cfg_cb->green_action.action)
            {
                acl.greenPhb = pstAction->supervise_cfg_cb->green_action.markphb;
                acl.flags |= ACL_GREEN_PHB;
            }
            if(2 == pstAction->supervise_cfg_cb->yellow_action.action)
            {
                acl.yellowPhb = pstAction->supervise_cfg_cb->yellow_action.markphb;
                acl.flags |= ACL_YELLOW_PHB;
            }
            if(2 == pstAction->supervise_cfg_cb->red_action.action)
            {
                acl.redPhb = pstAction->supervise_cfg_cb->red_action.markphb;
                acl.flags |= ACL_RED_PHB;
            }
            else if(1 == pstAction->supervise_cfg_cb->red_action.action)
            {
                acl.flags |= ACL_DROP_RED;
            }
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetAcl(unit, &acl);
            if(ATG_DCI_RC_OK != ret)
            {
                 spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);

                 /* 异常退出 */
                 goto EXIT_LABEL;
            }
#endif
        }
        
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
NBB_LONG spm_set_policy_classify_acl(SPM_QOS_CLASSIFY_CB *pstClassify,
    SPM_QOS_ACTION_CB *pstAction,ACL_T *pstAcl NBB_CCXT_T NBB_CXT) 
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT unit = 0;
    NBB_ULONG aclid = 0;
    SPM_PORT_INFO_CB stPortInfo = {0};
    NBB_ULONG nhi = 0;
    NBB_USHORT i = 0;
    NBB_BYTE tcp_flag = 0;
    SPM_PORT_INFO_CB pstPortInfo = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstClassify) || (NULL == pstAcl) || (NULL == pstAction))
    {
        printf("NULL == pstClassify or pstAction");
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    if(NULL == pstClassify->basic_cfg_cb)
    {
        printf("NULL == pstClassify->basic_cfg_cb");
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /* 异常退出 */
        goto EXIT_LABEL;
    }
    switch(pstClassify->basic_cfg_cb->match_type)
    {

        /* classify规则目前已取消匹配端口的功能 */
        case MATCH_PORT:
        if(NULL == pstClassify->Port_cfg_cb)
        {
           printf("NULL == pstClassify->Eth_cfg_cb");
           ret = ATG_DCI_RC_UNSUCCESSFUL;

           /* 异常退出 */
           goto EXIT_LABEL;
        }
        OS_MEMSET(&pstPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
        ret = spm_get_portid_from_logical_port_index(pstClassify->Port_cfg_cb->index, &pstPortInfo NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_get_portid_from_logical_port_index_error_log(pstClassify->Port_cfg_cb->index,
                    __FUNCTION__,__LINE__,ret NBB_CCXT);

            /* 异常退出 */
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
        break;

        case MATCH_ETH_HEADER:
        if(NULL == pstClassify->Eth_cfg_cb)
        {
           printf("NULL == pstClassify->Eth_cfg_cb");
           ret = ATG_DCI_RC_UNSUCCESSFUL;

           /* 异常退出 */
           goto EXIT_LABEL;
        }
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.smac[0]), &(pstClassify->Eth_cfg_cb->src_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.dmac[0]), &(pstClassify->Eth_cfg_cb->dst_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.smac[0]), &(pstClassify->Eth_cfg_cb->src_mac_mask[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.dmac[0]), &(pstClassify->Eth_cfg_cb->dst_mac_mask[0]), ATG_DCI_MAC_LEN);
        pstAcl->tAclRule.tAclKey.ovlan_pri = pstClassify->Eth_cfg_cb->vlan_prio;
        pstAcl->tAclRule.tAclMask.ovlan_pri = pstClassify->Eth_cfg_cb->vlan_prio_mask;
#ifdef R8000_V3R2
        pstAcl->tAclRule.tAclKey.ivlan_pri = pstClassify->Eth_cfg_cb->cvlan_prio;
        pstAcl->tAclRule.tAclMask.ivlan_pri = pstClassify->Eth_cfg_cb->cvlan_prio_mask;
#endif
        pstAcl->tAclRule.tAclKey.ethType  = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_IPV4:
        if(NULL == pstClassify->ipv4_cfg_cb)
        {
           printf("NULL == pstClassify->ipv4_cfg_cb");
           ret = ATG_DCI_RC_UNSUCCESSFUL;

           /* 异常退出 */
           goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.dip = pstClassify->ipv4_cfg_cb->dst_ip;
        pstAcl->tAclRule.tAclMask.dip = pstClassify->ipv4_cfg_cb->dst_ip_mask;
        pstAcl->tAclRule.tAclKey.sip = pstClassify->ipv4_cfg_cb->src_ip;
        pstAcl->tAclRule.tAclMask.sip = pstClassify->ipv4_cfg_cb->src_ip_mask;
        pstAcl->tAclRule.tAclKey.tos = (pstClassify->ipv4_cfg_cb->dscp);
        pstAcl->tAclRule.tAclMask.tos = (pstClassify->ipv4_cfg_cb->dscp_mask);
        pstAcl->tAclRule.tAclKey.l3Protocol = pstClassify->ipv4_cfg_cb->l3_protocol;
        pstAcl->tAclRule.tAclMask.l3Protocol = pstClassify->ipv4_cfg_cb->l3_protocol_mask;
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.ipHeaderMF = pstClassify->ipv4_cfg_cb->mf;
        pstAcl->tAclRule.tAclMask.ipHeaderMF = pstClassify->ipv4_cfg_cb->mf_mask;
        pstAcl->tAclRule.tAclKey.ipHeaderDF = pstClassify->ipv4_cfg_cb->df;
        pstAcl->tAclRule.tAclMask.ipHeaderDF = pstClassify->ipv4_cfg_cb->df_mask;
        pstAcl->tAclRule.tAclKey.l4SrcPort = pstClassify->ipv4_cfg_cb->src_port;
        pstAcl->tAclRule.tAclMask.l4SrcPort = pstClassify->ipv4_cfg_cb->src_port_mask;
        pstAcl->tAclRule.tAclKey.l4DstPort = pstClassify->ipv4_cfg_cb->dst_port;
        pstAcl->tAclRule.tAclMask.l4DstPort = pstClassify->ipv4_cfg_cb->dst_port_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        if((1 == pstClassify->ipv4_cfg_cb->l3_protocol) 
            && (0xff == pstClassify->ipv4_cfg_cb->l3_protocol_mask) 
            && (0 != pstClassify->ipv4_cfg_cb->payload_mask[0])) 
        {
            pstAcl->tAclRule.tAclKey.icmpType = pstClassify->ipv4_cfg_cb->payload[0];
            pstAcl->tAclRule.tAclMask.icmpType = pstClassify->ipv4_cfg_cb->payload_mask[0];
        }
        if((1 == pstClassify->ipv4_cfg_cb->l3_protocol) 
            && (0xff == pstClassify->ipv4_cfg_cb->l3_protocol_mask)  
            && (0 != pstClassify->ipv4_cfg_cb->payload_mask[1])) 
        { 
            pstAcl->tAclRule.tAclKey.icmpCode = pstClassify->ipv4_cfg_cb->payload[1];
            pstAcl->tAclRule.tAclMask.icmpCode = pstClassify->ipv4_cfg_cb->payload_mask[1];
        }
        break;

        case MATCH_IP_TCP:
        if(NULL == pstClassify->ipTcp_cfg_cb)
        {
            printf("NULL == pstClassify->ipTcp_cfg_cb");
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.dip = pstClassify->ipTcp_cfg_cb->dst_ip;
        pstAcl->tAclRule.tAclMask.dip = pstClassify->ipTcp_cfg_cb->dst_ip_mask;
        pstAcl->tAclRule.tAclKey.sip = pstClassify->ipTcp_cfg_cb->src_ip;
        pstAcl->tAclRule.tAclMask.sip = pstClassify->ipTcp_cfg_cb->src_ip_mask;
        pstAcl->tAclRule.tAclKey.tos = (pstClassify->ipTcp_cfg_cb->dscp);
        pstAcl->tAclRule.tAclMask.tos = (pstClassify->ipTcp_cfg_cb->dscp_mask);
        pstAcl->tAclRule.tAclKey.l3Protocol = 6;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l4SrcPort = pstClassify->ipTcp_cfg_cb->src_port;
        pstAcl->tAclRule.tAclMask.l4SrcPort = pstClassify->ipTcp_cfg_cb->src_port_mask;
        pstAcl->tAclRule.tAclKey.l4DstPort = pstClassify->ipTcp_cfg_cb->dst_port;
        pstAcl->tAclRule.tAclMask.l4DstPort = pstClassify->ipTcp_cfg_cb->dst_port_mask; 
        OS_MEMCPY(&tcp_flag,&(pstClassify->ipTcp_cfg_cb->tcp_flag),1);
        pstAcl->tAclRule.tAclKey.tcp_flag = tcp_flag;
        pstAcl->tAclRule.tAclMask.tcp_flag = pstClassify->ipTcp_cfg_cb->tcp_flag_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_IP_UDP:
        if(NULL == pstClassify->ipUdp_cfg_cb)
        {
            printf("NULL == pstClassify->ipUdp_cfg_cb");
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.dip = pstClassify->ipUdp_cfg_cb->dst_ip;
        pstAcl->tAclRule.tAclMask.dip = pstClassify->ipUdp_cfg_cb->dst_ip_mask;
        pstAcl->tAclRule.tAclKey.sip = pstClassify->ipUdp_cfg_cb->src_ip;
        pstAcl->tAclRule.tAclMask.sip = pstClassify->ipUdp_cfg_cb->src_ip_mask;
        pstAcl->tAclRule.tAclKey.tos = (pstClassify->ipUdp_cfg_cb->dscp);
        pstAcl->tAclRule.tAclMask.tos = (pstClassify->ipUdp_cfg_cb->dscp_mask);
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->tAclRule.tAclKey.l4SrcPort = pstClassify->ipUdp_cfg_cb->src_port;
        pstAcl->tAclRule.tAclMask.l4SrcPort = pstClassify->ipUdp_cfg_cb->src_port_mask;
        pstAcl->tAclRule.tAclKey.l4DstPort = pstClassify->ipUdp_cfg_cb->dst_port;
        pstAcl->tAclRule.tAclMask.l4DstPort = pstClassify->ipUdp_cfg_cb->dst_port_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_IPV6:
        if(NULL == pstClassify->ipv6_cfg_cb)
        {
            printf("NULL == pstClassify->ipv6_cfg_cb");
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        OS_MEMCPY(pstAcl->tAclRule.tAclKey.dipv6,pstClassify->ipv6_cfg_cb->dst_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(pstClassify->ipv6_cfg_cb->dst_ip_mask_len,
            (NBB_ULONG*)&(pstAcl->tAclRule.tAclMask.dipv6) NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        OS_MEMCPY(pstAcl->tAclRule.tAclKey.sipv6,pstClassify->ipv6_cfg_cb->src_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(pstClassify->ipv6_cfg_cb->src_ip_mask_len,
            (NBB_ULONG*)&(pstAcl->tAclRule.tAclMask.sipv6) NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.l3Protocol = pstClassify->ipv6_cfg_cb->l3_protocol;
        pstAcl->tAclRule.tAclMask.l3Protocol = pstClassify->ipv6_cfg_cb->l3_protocol_mask;
        pstAcl->tAclRule.tAclKey.l4SrcPort = pstClassify->ipv6_cfg_cb->src_port;
        pstAcl->tAclRule.tAclMask.l4SrcPort = pstClassify->ipv6_cfg_cb->src_port_mask;
        pstAcl->tAclRule.tAclKey.l4DstPort = pstClassify->ipv6_cfg_cb->dst_port;
        pstAcl->tAclRule.tAclMask.l4DstPort = pstClassify->ipv6_cfg_cb->dst_port_mask;
        pstAcl->tAclRule.tAclKey.tos = pstClassify->ipv6_cfg_cb->tos;
        pstAcl->tAclRule.tAclMask.tos = pstClassify->ipv6_cfg_cb->tos_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_NOIP:
        if(NULL == pstClassify->noIp_cfg_cb)
        {
            printf("NULL == pstClassify->noIp_cfg_cb\n");
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.ethType = pstClassify->noIp_cfg_cb->etype;
        pstAcl->tAclRule.tAclMask.ethType = pstClassify->noIp_cfg_cb->etype_mask;
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData6),&(pstClassify->noIp_cfg_cb->payload[0]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData5),&(pstClassify->noIp_cfg_cb->payload[4]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData4),&(pstClassify->noIp_cfg_cb->payload[8]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData6),&(pstClassify->noIp_cfg_cb->payload_mask[0]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData5),&(pstClassify->noIp_cfg_cb->payload_mask[4]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData4),&(pstClassify->noIp_cfg_cb->payload_mask[8]),4);
        pstAcl->flags |= ACL_COUNTER_ON;
        break;
#ifdef R8000_V3R2
        case MATCH_PORT_VLAN:
        if(NULL == pstClassify->portVlan_cfg_cb)
        {
            printf("NULL == pstClassify->portVlan_cfg_cb\n");
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.oVid = pstClassify->portVlan_cfg_cb->svlan_id;
        pstAcl->tAclRule.tAclMask.oVid = pstClassify->portVlan_cfg_cb->svlan_mask;
        pstAcl->tAclRule.tAclKey.iVid = pstClassify->portVlan_cfg_cb->cvlan_id;
        pstAcl->tAclRule.tAclMask.iVid = pstClassify->portVlan_cfg_cb->cvlan_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;
#endif      
        default:
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /* 异常退出 */
        goto EXIT_LABEL;
        break;
    }

    /* C2B对中与classify配对的action模块中,复杂流分类的动作flow_act_cfg_cb不允许为空 */
    if(NULL == pstAction->flow_act_cfg_cb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_set_policy_classify_acl err : flow_act_cfg_cb "
                  "of pstAction is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
        OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_classify_acl err : flow_act_cfg_cb "
                   "of pstAction is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        		       QOS_ERROR_STRING,ret,
        		       ucMessage,__FUNCTION__,"LINE","",
        		       0,0,__LINE__,0)); 

        /* 异常退出 */
        goto EXIT_LABEL; 
    }
    if(0 == pstAction->flow_act_cfg_cb->action)
    {
        pstAcl->eAclAction = ACL_ACTION_DROP;
    }
    else if(1 == pstAction->flow_act_cfg_cb->action)
    {
        pstAcl->eAclAction = ACL_ACTION_PASS;
    }
    else if(3 == pstAction->flow_act_cfg_cb->action)
    {
        /* 当动作是重定向时,重定向配置块不允许为空 */
        if(NULL == pstAction->redirect_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_policy_classify_acl err : redirect_cfg_cb "
                      "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", __FUNCTION__,
                      __LINE__, ret, pstAction->action_key);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_classify_acl err : redirect_cfg_cb "
                       "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", __FUNCTION__,
                       __LINE__, ret, pstAction->action_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            		       QOS_ERROR_STRING,ret,
            		       ucMessage,__FUNCTION__,"LINE","",
            		       0,0,__LINE__,0)); 

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        if(0 == pstAction->redirect_cfg_cb->action)
        {
            pstAcl->eAclAction = ACL_ACTION_PASS;
        }
        else if(1 == pstAction->redirect_cfg_cb->action)
        {
            pstAcl->eAclAction = ACL_ACTION_VRF;
            pstAcl->vrfId = pstAction->redirect_cfg_cb->vrf_d;
        }
        else if(2 == pstAction->redirect_cfg_cb->action)
        {
            if(0 == pstAction->redirect_cfg_cb->out_port_index)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("%s line=%d spm_set_policy_classify_acl err : redirect_cfg_cb "
                          "of pstAction out_port_index is 0. ret = %ld, action_id = %ld\n\n", 
                          __FUNCTION__, __LINE__, ret, pstAction->action_key);
                OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_classify_acl err : redirect_cfg_cb "
                           "of pstAction out_port_index is 0. ret = %ld, action_id = %ld\n\n", 
                           __FUNCTION__, __LINE__, ret, pstAction->action_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                		       QOS_ERROR_STRING,ret,
                		       ucMessage,__FUNCTION__,"LINE","",
                		       0,0,__LINE__,0)); 

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            ret = spm_get_portid_from_logical_port_index(       \
                        pstAction->redirect_cfg_cb->out_port_index,&stPortInfo NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                spm_get_portid_from_logical_port_index_error_log(pstAction->redirect_cfg_cb->out_port_index,
                        __FUNCTION__,__LINE__,ret NBB_CCXT);

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            pstAcl->slot = stPortInfo.slot_id;
            pstAcl->cardPort = stPortInfo.port_id; 
            pstAcl->eAclAction = ACL_ACTION_REDIRECTION;
        }
    }
    else if(4 == pstAction->flow_act_cfg_cb->action)
    {
        pstAcl->eAclAction = ACL_ACTION_NHI;
        if(NULL == pstAction->policy_nhi_cfg_cb)
        {   
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_policy_classify_acl err : policy_nhi_cfg_cb "
                      "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", 
                      __FUNCTION__, __LINE__, ret, pstAction->action_key);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_classify_acl err : policy_nhi_cfg_cb "
                       "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", 
                       __FUNCTION__, __LINE__, ret, pstAction->action_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            		       QOS_ERROR_STRING,ret,
            		       ucMessage,__FUNCTION__,"LINE","",
            		       0,0,__LINE__,0)); 

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        for(i = 0;i < pstAction->policy_nhi_cfg_cb->nxhop_num;i++)
        {
            if(0 == pstAction->policy_nhi_cfg_cb->nxhp[i].ip_type)
            {
                ret = spm_l3_lookupuni(0, &(pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_ip[3]),
                pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,&nhi NBB_CCXT);
            }
            else
            {
                ret = spm_l3_lookupuni(1, &(pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_ip[0]),
                pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,&nhi NBB_CCXT);
            }
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                spm_l3_lookupuni_error_log(pstAcl->mAclId,pstAcl->mRuleId,pstAction->action_key,__FUNCTION__,
                       __LINE__,pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,ret NBB_CCXT);

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            pstAcl->nhIdx = nhi;
        }
    }

    /* 配置动作 */
    if(0 != pstAction->meter_id)
    {
        pstAcl->meterIdx = pstAction->meter_id;
        pstAcl->flags |= ACL_METER_ON;

        if(NULL == pstAction->supervise_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /* 异常退出 */
            goto EXIT_LABEL;
        }
        if(1 == pstAction->supervise_cfg_cb->cm_color)
        {
            pstAcl->flags |= ACL_COLOR_AWARE;
        }
        if(2 == pstAction->supervise_cfg_cb->green_action.action)
        {
            pstAcl->greenPhb = pstAction->supervise_cfg_cb->green_action.markphb;
            pstAcl->flags |= ACL_GREEN_PHB;
        }
        if(2 == pstAction->supervise_cfg_cb->yellow_action.action)
        {
            pstAcl->yellowPhb = pstAction->supervise_cfg_cb->yellow_action.markphb;
            pstAcl->flags |= ACL_YELLOW_PHB;
        }
        if(2 == pstAction->supervise_cfg_cb->red_action.action)
        {
            pstAcl->redPhb = pstAction->supervise_cfg_cb->red_action.markphb;
            pstAcl->flags |= ACL_RED_PHB;
        }
        else if(1 == pstAction->supervise_cfg_cb->red_action.action)
        {
            pstAcl->flags |= ACL_DROP_RED;
        }
    }
    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pstAcl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pstAcl, __FUNCTION__, __LINE__, ret NBB_CCXT);

            /* 异常退出 */
            goto EXIT_LABEL;
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
NBB_LONG spm_set_policy_c2b_rule(SPM_QOS_POLICY_CB *pstPolicy,
    NBB_ULONG classify_index,NBB_ULONG action_index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_CLASSIFY_CB *pstClassify = NULL;
    SPM_QOS_ACTION_CB *pstAction = NULL;
    SPM_QOS_ACL_CB *pstAcl = NULL;
    ACL_T acl = {0};
    NBB_USHORT unit = 0;
    NBB_ULONG aclid = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstPolicy || classify_index >= MAX_FLOW_CLASSIFY_PRI)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line = %d spm_set_policy_c2b_rule err : policy "
                  "cfg is NULL. ret = %ld\n\n", __FUNCTION__, __LINE__, ret);
        OS_SPRINTF(ucMessage,"%s line = %d spm_set_policy_c2b_rule err : policy "
                   "cfg is NULL. ret = %ld\n\n", __FUNCTION__, __LINE__, ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        		       QOS_ERROR_STRING,ret,
        		       ucMessage,__FUNCTION__,"LINE","",
        		       0,0,__LINE__,0)); 

        /* 异常退出 */
         goto EXIT_LABEL; 
    }
    pstAction = AVLL_FIND(SHARED.qos_action_tree,&action_index);
    if(NULL == pstAction)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line = %d spm_set_policy_c2b_rule err : can't find "
                  "action cfg. ret = %ld, action_id = %ld\n\n", __FUNCTION__, __LINE__, 
                  ret, action_index);
        OS_SPRINTF(ucMessage,"%s line = %d spm_set_policy_c2b_rule err : can't find "
                   "action cfg. ret = %ld, action_id = %ld\n\n", __FUNCTION__, __LINE__, 
                   ret, action_index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        		       QOS_ERROR_STRING,ret,
        		       ucMessage,__FUNCTION__,"LINE","",
        		       0,0,__LINE__,0));

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /* 在链表中查出ClassifyId所包含的所有规则 */
    for (pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((     \
                                SHARED.g_classify_id_instance[classify_index].classify_group)); 
         pstClassify != NULL; 
         pstClassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(pstClassify->spm_classify_group_node))
    {
    
        /* 每个classify规则的基本配置不允许为空,它指明了该规则的匹配类型(以太网头/TCP_IP等)等 */
        if(NULL == (pstClassify->basic_cfg_cb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line = %d  spm_set_policy_c2b_rule  err : classfiy basic "
                      "cfg is NULL. ret = %ld, ClassfiyId = %ld, if_match_id = %ld\n\n", __FUNCTION__,
                      __LINE__, ret, classify_index, pstClassify->classify_key.if_match_id);
            OS_SPRINTF(ucMessage,"%s line = %d  spm_set_policy_c2b_rule  err : classfiy basic "
                       "cfg is NULL. ret = %ld, ClassfiyId = %ld, if_match_id = %ld\n\n", __FUNCTION__,
                       __LINE__, ret, classify_index, pstClassify->classify_key.if_match_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            	           QOS_ERROR_STRING,ret,
            	           ucMessage,__FUNCTION__,"LINE","",
            	           0,0,__LINE__,0)); 

            /* 异常退出 */
            goto EXIT_LABEL; 
        }
        
       /******************************************************************** 
        * 1.Policy策略中的C2B对引用Acl时不需分配AclId;对于Policy策略来说,  *
        *   如果C2B中有某条Classify规则是引用Acl的,那么该Classifyid中只允  *
        *   许包含一条Classify规则,不允许包含N条规则,该限制是由上层网管来  *
        *   实现的;                                                        *
        * 2.如果某个AclId被Policy策略中C2B对的Classify规则所引用,那么该Acl *
        *   不允许绑定到intf上,这一点也是由上层网管来保证的;               *
        * 3.在删除Policy策略时,如果C2B对是引用Acl的,此时还需要还原Acl规则, *
        *   还原后的Acl可以绑定在intf上,并在Acl模块中更新或删除等;         *
        * 4.之所以需要还原Acl是因为C2B对引用Acl时，会将C2B对中的Action内容 *
        *   写入Acl中,当删除Policy时需要将这部分Action的配置内容删除。     *
        ********************************************************************/
        if(MATCH_ACL_ID == pstClassify->basic_cfg_cb->match_type)
        {
            pstPolicy->acl_id = pstClassify->basic_cfg_cb->acl_id;

            /* 更新C2B对中所引用的AclId规则的内容:将Action中的配置也写入C3的Acl中 */
            ret = spm_set_policy_by_acl(pstPolicy,pstAction NBB_CCXT);

            /* 异常退出 */
            goto EXIT_LABEL;
        }

        /**********************************************************************
         * 1.非引用Acl的情况,第一次存储Policy数据时需要给Policy申请一个AclId; *
         * 2.每个policy策略只有一个aclid,虽然policy中有多个cb对,会对应着多条  *
         *   classify规则,但这些classify规则都公用一个aclid;                  *
         * 3.每个classify规则系统会为其分配一个全局唯一的ruleid来指代该规则.  *
         **********************************************************************/
        if( 0 == pstPolicy->acl_id)
        {
            /* Classif规则是匹配端口 */
            if(MATCH_PORT == pstClassify->basic_cfg_cb->match_type || 
               MATCH_PORT_VLAN == pstClassify->basic_cfg_cb->match_type)
            {
                ret = spm_apply_classify_port_acl_id(&aclid NBB_CCXT);    
            }

            /* Classif规则是以太网头 */
            else if(MATCH_ETH_HEADER == pstClassify->basic_cfg_cb->match_type)
            {
                ret = spm_apply_classify_eth_acl_id(&aclid NBB_CCXT);    
            }

            /* Classif规则是匹配IP类型 */
            else
            {
                ret = spm_apply_classify_high_acl_id(&aclid NBB_CCXT);
            }
            if(ATG_DCI_RC_OK != ret)
            {

                /* 异常退出 */
                goto EXIT_LABEL;
            }
            pstPolicy->acl_id = aclid;
        }
        acl.mAclId = pstPolicy->acl_id;
        acl.mRuleId = pstClassify->rule_id;

        /*ACL的优先级*/
        if(0 != acl_pri_setting)
        {
            acl.priority = pstClassify->rule_id;
        }

        ret = spm_set_policy_classify_acl(pstClassify,pstAction,&acl NBB_CCXT);

        /* 配置其中一个失败,需要反刷成功的ACL没有做 */
        if(ATG_DCI_RC_OK != ret)
        { 

            if(MATCH_PORT == pstClassify->basic_cfg_cb->match_type ||
                MATCH_PORT_VLAN == pstClassify->basic_cfg_cb->match_type)
            {
                spm_release_classify_port_acl_id(pstPolicy->acl_id NBB_CCXT);    
            }
            else if(MATCH_ETH_HEADER == pstClassify->basic_cfg_cb->match_type)
            {
                spm_release_classify_eth_acl_id(pstPolicy->acl_id NBB_CCXT);    
            }
            else
            {
                spm_release_classify_high_acl_id(pstPolicy->acl_id NBB_CCXT);
            }
            pstPolicy->acl_id = 0;
            
            /* 异常退出 */
            goto EXIT_LABEL;      
        }
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
NBB_VOID spm_rcv_dci_set_policy(ATG_DCI_SET_QOS_POLICY *pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_QOS_POLICY_CB *pstCb = NULL;
    SPM_QOS_POLICY_C2B_CB *pstC2B = NULL;
    SPM_QOS_CLASSIFY_CB *pstClassify = NULL;
    SPM_QOS_ACTION_CB *pstAction = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;
    NBB_USHORT unit = 0;
    ACL_T acl = {0};
    SPM_QOS_ACL_CB *pstAcl = NULL;
    NBB_ULONG classify_index = 0;
    NBB_BYTE acl_flag = ATG_DCI_RC_OK; /*引用ACL标志位*/
    ATG_DCI_QOS_POLICY_BASIC_DATA *pstBasicData = NULL;
    ATG_DCI_QOS_POLICY_CB2QOS_DATA *pstC2bData = NULL;
    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucC2bDataStart = NULL;
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperC2b = ATG_DCI_OPER_NULL;
    NBB_USHORT c2bnum = 0;
    NBB_SHORT c2bret[ATG_DCI_QOS_POLICY_CB2QOS_NUM] = {0};
    NBB_USHORT i = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        goto EXIT_LABEL;
    }

    pstSetIps->return_code = ATG_DCI_RC_OK;
    OS_MEMSET(&acl, 0, sizeof(ACL_T));
    ulkey = pstSetIps->key;
    pstCb = AVLL_FIND(SHARED.qos_policy_tree, &ulkey);
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }
    c2bnum = pstSetIps->cb2qos_num;
    ulOperBasic = pstSetIps->oper_basic;
    ulOperC2b = pstSetIps->oper_cb2qos;
    pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->basic_data);
    pucC2bDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->cb2qos_data);
    pstBasicData = (ATG_DCI_QOS_POLICY_BASIC_DATA *)pucBasicDataStart;

    /* ips消息为删除:如果条目不存在,不删除;若存在,则删除 */
    if (TRUE == pstSetIps->delete_struct)
    {
        if (ucIfExist == QOS_UNEXIST)
        {
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            goto EXIT_LABEL;
        }
        else
        {
            if(NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
            if(0 != pstCb->cnt) /* 被引用 */
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                goto EXIT_LABEL;
            }
            if(NULL != AVLL_FIRST(pstCb->instance_tree)) /* 复杂流分类被引用 */
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                goto EXIT_LABEL;
            }
            for(pstC2B = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pstCb->c2b_tree); pstC2B != NULL;
                pstC2B = (SPM_QOS_POLICY_C2B_CB *)AVLL_NEXT(pstCb->c2b_tree, pstC2B->spm_policy_c2b_node))
            {
                classify_index = pstC2B->c2b_key.qos_classify_index;

                /* 删除ACL如果是引用ACL不删除只解除action动作的绑定未做 */
                for (pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((     \
                                    SHARED.g_classify_id_instance[classify_index].classify_group)); 
                     pstClassify != NULL; 
                     pstClassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(pstClassify->spm_classify_group_node))
                {
                    acl.mAclId = pstCb->acl_id;
                    acl.mRuleId = pstClassify->rule_id;
                    if(NULL != pstClassify->basic_cfg_cb)
                    {   
                    
                       /*********************************************************************** 
                        * 此时Classify规则的匹配模式是匹配AclId;若Policy策略中有Classify规则  *
                        * 引用Acl,则该Policy策略中只有唯一的一个C2B对,该C2B对中ClassifyId只有 *
                        * 唯一的一条Classify规则,该规则是引用Acl的.                           *
                        ***********************************************************************/
                        if(MATCH_ACL_ID == pstClassify->basic_cfg_cb->match_type) 
                        {
                            for(pstAcl = (SPM_QOS_ACL_CB*)NBB_NEXT_IN_LIST((        \
                                           SHARED.g_acl_id_instance[pstClassify->basic_cfg_cb->acl_id].acl_group));
                                pstAcl != NULL; 
                                pstAcl = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pstAcl->spm_acl_group_node))
                            {
                                acl.mAclId = pstAcl->acl_key.acl_id;
                                acl.mRuleId = pstAcl->acl_key.rule_id;
                                if(0 == pstAcl->basic_cfg_cb->action) /* 恢复动作 */
                                {
                                    acl.eAclAction = ACL_ACTION_DROP;
                                }
                                else if(1 == pstAcl->basic_cfg_cb->action)
                                {
                                    acl.eAclAction = ACL_ACTION_PASS;
                                }
                                for(unit = 0; unit < SHARED.c3_num;unit++)
                                {
#if defined (SPU) || defined (PTN690_CES)

                                    /* 若C2B对中Classify是引用Acl的,在删除Policy时需要将Acl还原 */
                                    ret = ApiC3SetAcl(unit,&acl);
                                    if(ATG_DCI_RC_OK != ret)
                                    {
                                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                                        pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                    }
#endif
                                } 
                            }
                            acl_flag = ATG_DCI_RC_UNSUCCESSFUL;
                        }

                        /* 此时classify的匹配模式不是匹配AclId,与基础配置里的匹配类型(以太网头/IP等等)相关 */
                        else
                        {
                            for(unit = 0; unit < SHARED.c3_num;unit++)
                            {
#if defined (SPU) || defined (PTN690_CES)
                                ret = ApiC3DelAcl(unit,&acl);
                                if(ATG_DCI_RC_OK != ret)
                                {
                                    spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                }
#endif
                            } 
                        }
                    }
                }
            } 

            /* 多c2b对模式,同时classify规则的匹配模式是非引用AclId的模式:释放为policy分配的aclid */
            if((0 != pstCb->basic_cfg_cb->mode) && (ATG_DCI_RC_OK == acl_flag))
            {
                if((pstCb->acl_id >= CLASSIFY_PORT_ACL_ID) && 
                   (pstCb->acl_id < CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM))
                {
                    spm_release_classify_port_acl_id(pstCb->acl_id NBB_CCXT);    
                }
                else if((pstCb->acl_id >= CLASSIFY_ETH_ACL_ID) && 
                        (pstCb->acl_id < CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM))
                {
                    spm_release_classify_eth_acl_id(pstCb->acl_id NBB_CCXT); 
                } 
                else if((pstCb->acl_id >= CLASSIFY_HIGH_ACL_ID) && 
                        (pstCb->acl_id < CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM))
                {
                    spm_release_classify_high_acl_id(pstCb->acl_id NBB_CCXT);    
                }
            }
            AVLL_DELETE(SHARED.qos_policy_tree, pstCb->spm_policy_node);
            ret = spm_free_policy_cb(pstCb NBB_CCXT); /* 释放POLICY表分配的内存空间 */
            if (ATG_DCI_RC_OK != ret)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            pstCb = NULL;
        }
    } 
    else /* 增加或者更新 */
    {
        if (ucIfExist != QOS_UNEXIST)
        {
            if (0 != pstCb->cnt)  /* 被引用 */
            {
                pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                goto EXIT_LABEL;
            }
            if(NULL != AVLL_FIRST(pstCb->instance_tree)) /* c2b中的Classify被引用不能更新 */
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                goto EXIT_LABEL;
            }
        }    
        switch (ulOperBasic)  /* 将policy的基本配置写入内存中 */
        {
            case  ATG_DCI_OPER_UPDATE:
            pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
            break;

            case  ATG_DCI_OPER_DEL:
            pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
            break;

            case  ATG_DCI_OPER_ADD:
            if(NULL == pstBasicData)
            {
                pstSetIps->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;  
            }
            if (ucIfExist == QOS_UNEXIST) /* 如果条目不存在,新申请内存空间保存数据 */
            {
                pstCb = spm_alloc_policy_cb(ulkey NBB_CCXT); /* 申请一个新条目的内存空间 */
                if (NULL == pstCb)
                {
                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            if(NULL == pstCb->basic_cfg_cb) /* policy模式有调度节点/多C2B对模式 */
            {
                pstCb->basic_cfg_cb = (ATG_DCI_QOS_POLICY_BASIC_DATA *)NBB_MM_ALLOC(    \
                                       sizeof(ATG_DCI_QOS_POLICY_BASIC_DATA), NBB_NORETRY_ACT, MEM_SPM_POLICY_BASIC_CB);
                if (NULL == pstCb->basic_cfg_cb)
                {
                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }  
            OS_MEMCPY(pstCb->basic_cfg_cb,pstBasicData,sizeof(ATG_DCI_QOS_POLICY_BASIC_DATA));
            break;

            default:
            break;
        }
        if (ATG_DCI_QOS_POLICY_CB2QOS_NUM <= c2bnum) /* CB对处理 */
        {
            /* 将64个C2B对的返回值全部赋值为失败 */
            OS_MEMSET(&(pstSetIps->cb2qos_return_code[0]),ATG_DCI_RC_UNSUCCESSFUL,
                        sizeof(NBB_SHORT)*ATG_DCI_QOS_POLICY_CB2QOS_NUM);
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            if(QOS_UNEXIST == ucIfExist)
            {
                spm_free_policy_cb(pstCb NBB_CCXT);
                pstCb = NULL;
            } 
            goto EXIT_LABEL;
        }
        for (i = 0; ((i < c2bnum) && (NULL != pucC2bDataStart)); i++) /* 对policy策略中的C2B对进行操作:删除/增加 */
        {
            pstC2bData = (ATG_DCI_QOS_POLICY_CB2QOS_DATA *)
               (pucC2bDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_QOS_POLICY_CB2QOS_DATA))) * i);

            if(NULL == pstC2bData)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                if(QOS_UNEXIST == ucIfExist)
                {
                    spm_free_policy_cb(pstCb NBB_CCXT);
                    pstCb = NULL;
                }
                goto EXIT_LABEL;
            }

           /**************************************************************************************** 
            *  C2B对的操作类型:更新(暂时无效)、删除和增加;虽然一个policy策略中最多可能有64个C2B对, *
            *  但对于同一个policy策略来说,它其中所有的C2B对操作类型是相同的:要么同为删除,要么同为  *
            *  增加操作.在同一个policy策略中不可能出现某个/些C2B对的操作类型为增加,另外一个/些C2B  *
            *  对的操作类型为删除的情况.                                                           *
            ****************************************************************************************/
            switch (ulOperC2b)
            {
                case  ATG_DCI_OPER_UPDATE:
                break;
                case  ATG_DCI_OPER_DEL: /* 删除C2B对 */
                if(NULL == pstCb)
                {
                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                pstC2B = AVLL_FIND(pstCb->c2b_tree, pstC2bData);
                if (NULL == pstC2B)
                {
                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                pstAction = AVLL_FIND(SHARED.qos_action_tree,&(pstC2B->c2b_key.qos_behavior_index));
                if(NULL == pstAction)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    goto EXIT_LABEL;
                }
                if(0 != pstCb->basic_cfg_cb->mode) /* policy策略为多C2B对模式 */
                {
                    /* C2B对中的C指的是ClassifyId,每个ClassifyId中允许包含N条Classify规则 */
                    for (pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((     \
                             SHARED.g_classify_id_instance[pstC2B->c2b_key.qos_classify_index].classify_group));
                         pstClassify != NULL; 
                         pstClassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(pstClassify->spm_classify_group_node))
                    {
                        
                        if(NULL == pstClassify->basic_cfg_cb) /* classify规则的匹配类型的基础配置不能为空 */
                        {
                            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL; 
                            pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                            spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                            goto EXIT_LABEL;
                        }

                        /* 每个policy策略只有一个aclid,其中所有C2B对中的classify都公用该aclid */
                        acl.mAclId = pstCb->acl_id;

                     /************************************************************************** 
                      * 系统会为每个classify规则分配一个唯一代表它的RuleId;虽然policy策略C2B对 * 
                      * 中所有的classify的aclid相同,但RuleId是唯一的,可以区分每个classify规则  * 
                      **************************************************************************/
                        acl.mRuleId = pstClassify->rule_id;
                        for(unit = 0;unit < SHARED.c3_num;unit++)
                        {                      
#if defined (SPU) || defined (PTN690_CES)
                            ret += ApiC3DelAcl(unit,&acl);
                            if(ATG_DCI_RC_OK != ret)
                            {
                                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }
#endif  
                        }
                    } 
                }

             /****************************************************************** 
              * 删除C3驱动中ClassifyId包含的规则后,将ClassifyId的匹配类型清零; *
              * 同一个ClassifyId中的所有规则必须是相同的匹配类型:以太网头/IP等 *
              ******************************************************************/
                SHARED.g_classify_id_instance[pstC2B->c2b_key.qos_classify_index].match_type = MATCH_ERROR;
                AVLL_DELETE(pstCb->c2b_tree, pstC2B->spm_policy_c2b_node);
                NBB_MM_FREE(pstC2B,MEM_SPM_POLICY_C2B_CB);
                pstC2B = AVLL_FIRST(pstCb->c2b_tree); /* 清除CB对时，如果全部清空要释放ACL资源 */
                if(NULL == pstC2B)
                {
                    spm_release_policy_acl_id(pstCb->acl_id NBB_CCXT);
                    pstCb->acl_id = 0;
                }
                break;

                case  ATG_DCI_OPER_ADD: /* 增加C2B对 */

               /****************************************************
                * 当policy策略为多C2B对模式时,classifyid不允许为零;*
                * 当policy策略为调度节点模式时,classifyid字段为零。*
                ****************************************************/
                if ((NULL != pstCb) && (NULL != pstCb->basic_cfg_cb) && 
                    (0 == pstC2bData->qos_classify_index) &&  
                    (0 != pstCb->basic_cfg_cb->mode))
                {
                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    if(QOS_UNEXIST == ucIfExist)
                    {
                        spm_free_policy_cb(pstCb NBB_CCXT);
                        pstCb = NULL;
                    }
                    goto EXIT_LABEL;
                }

                /* 无论Policy策略是何种模式,C2B对中的BehaviorId都不允许为零 */
                if (0 == pstC2bData->qos_behavior_index)
                {
                    pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                    if(QOS_UNEXIST == ucIfExist)
                    {
                        spm_free_policy_cb(pstCb NBB_CCXT);
                        pstCb = NULL;
                    }
                    goto EXIT_LABEL;
                }
                if((NULL != pstCb) && (NULL != pstCb->basic_cfg_cb) 
                    && (0 != pstCb->basic_cfg_cb->mode)) /* 多C2B对模式 */
                {
                    ret = spm_set_policy_c2b_rule(pstCb, pstC2bData->qos_classify_index,
                                                  pstC2bData->qos_behavior_index NBB_CCXT);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                        if(QOS_UNEXIST == ucIfExist)
                        {
                            spm_free_policy_cb(pstCb NBB_CCXT);
                            pstCb = NULL;
                        }
                        goto EXIT_LABEL;
                    }
                }
                else /*调度节点模式*/
                {
                    pstAction = AVLL_FIND(SHARED.qos_action_tree,&(pstC2bData->qos_behavior_index));
                    if(NULL == pstAction)
                    {
                        pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                        if(QOS_UNEXIST == ucIfExist)
                        {
                            spm_free_policy_cb(pstCb NBB_CCXT);
                            pstCb = NULL;
                        }
                        goto EXIT_LABEL;  
                    }

                  /**************************************************************** 
                   * Policy策略中的meter来自behavior中的流量监管,用于调度节点处对 *
                   * 流量进行限速;behavior中的流量整型,用于在hqos中对流量进行限速.*
                   ****************************************************************/
                    //coverity[var_deref_op]
                    pstCb->meter_id = pstAction->meter_id; 
                }
                
                //coverity[var_deref_model]
                pstC2B = AVLL_FIND(pstCb->c2b_tree, pstC2bData);
                if(NULL == pstC2B)
                {
                    /* 如果是新增的C2B对,则将该C2B对作为节点插入到policy的C2B对树中 */
                    pstC2B = spm_alloc_c2b_cb(pstC2bData NBB_CCXT);
                    if(NULL != pstC2B)
                    {
                        rv = AVLL_INSERT(pstCb->c2b_tree, pstC2B->spm_policy_c2b_node);
                    }
                }  
                break;

                default:
                break;
            }
            pstSetIps->cb2qos_return_code[i] = ATG_DCI_RC_OK;
        }

        /* policy策略中对C2B对的处理过程结束,将C2B对的数量存入policy策略结构体中 */
        if(NULL != pstCb)
        {
            pstCb->c2b_num = pstSetIps->cb2qos_num;
        }
        if ((QOS_UNEXIST == ucIfExist) && (NULL != pstCb)) /* 如果是新增的条目,插入到树中 */
        {
            pstCb->policy_key = ulkey;
            rv = AVLL_INSERT(SHARED.qos_policy_tree, pstCb->spm_policy_node);
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}

#endif

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
NBB_VOID spm_qos_clear_all_policy(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_POLICY_CB *cfg_cb = NULL;

    for (cfg_cb = (SPM_QOS_POLICY_CB*) AVLL_FIRST(v_spm_shared->qos_policy_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_POLICY_CB*) AVLL_FIRST(v_spm_shared->qos_policy_tree))
     {
        AVLL_DELETE(v_spm_shared->qos_policy_tree, cfg_cb->spm_policy_node);
        spm_free_policy_cb(cfg_cb NBB_CCXT);   
     }

    return;   
}

#endif





/*应用函数*/
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
SPM_QOS_FLOW_CLASSIFY_CB *spm_alloc_qos_flow_classify_cb(
    SPM_QOS_LOGIC_INTF_KEY *pkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_FLOW_CLASSIFY_CB *pstTbl = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != pkey);

    if (NULL == pkey)
    {
        printf("**QOS ERROR**%s,%d,key==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS spm_alloc_qos_flow_classify_cb",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /* 分配一个新的逻辑配置块qos表配置条目 */
    pstTbl = (SPM_QOS_FLOW_CLASSIFY_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_FLOW_CLASSIFY_CB),
              NBB_NORETRY_ACT, MEM_SPM_QOS_FLOW_CLASSIFY_CB);
    if (NULL == pstTbl)
    {
    
        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /* 初始逻辑配置块配置条目 */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_FLOW_CLASSIFY_CB));
    OS_MEMCPY(&(pstTbl->key), pkey, sizeof(SPM_QOS_LOGIC_INTF_KEY));
    AVLL_INIT_NODE(pstTbl->spm_qos_flow_classify_node);
    AVLL_INIT_NODE(pstTbl->spm_classify_id_node);

    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return pstTbl;
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
NBB_LONG spm_free_flow_classify_cb(SPM_QOS_FLOW_CLASSIFY_CB *pstTbl NBB_CCXT_T NBB_CXT)
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
					   "QOS spm_free_flow_classify_cb",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /* 异常退出 */
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_FLOW_CLASSIFY_CB), MEM_SPM_QOS_FLOW_CLASSIFY_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstTbl, MEM_SPM_QOS_FLOW_CLASSIFY_CB);
    pstTbl = NULL;


    /* 异常退出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


#if defined (CR8000_SMART) || defined (PTN690)
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
NBB_LONG spm_qos_set_color_action(NBB_ULONG *id,ATG_DCI_QOS_BEHAVIOR_SUPERVISE *cfg NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT unit = 0;
    POLICER_ACTION_TEMPLATE_T color_action;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == cfg) || (NULL == id))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }  

    NBB_MEMSET(&color_action, 0, sizeof(POLICER_ACTION_TEMPLATE_T));

    /* 添加 */
    if(0 == *id)
    {
       /* 申请色感知动作模板的模板id,目前最多只能申请4000个色感知动作模板 */
        ret = spm_qos_apply_color_action_id(id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        color_action.policer_action_id = *id;

        /* action : 0 / 1 / 2 = pass / drop / markphb */
        if(2 == cfg->green_action.action)
        {
            color_action.chg_gre_pri_ena = 1;
            color_action.gre_pri = cfg->green_action.markphb;
        }
        else if(1 == cfg->green_action.action)
        {
            color_action.chg_gre_pri_ena = 0;
            color_action.gre_drop_ena = 1;
        }
        else if(0 == cfg->green_action.action)
        {
            color_action.chg_gre_pri_ena = 0;
            color_action.gre_drop_ena = 0;
        }

        /* action : 0 / 1 / 2 = pass / drop / markphb */
        if(2 == cfg->yellow_action.action)
        {
            color_action.chg_yel_pri_ena = 1;
            color_action.yel_pri = cfg->yellow_action.markphb;
        }
        else if(1 == cfg->yellow_action.action)
        {
            color_action.chg_yel_pri_ena = 0;
            color_action.yel_drop_ena = 1;
        }
        else if(0 == cfg->yellow_action.action)
        {
            color_action.chg_yel_pri_ena = 0;
            color_action.yel_drop_ena = 0;
        }

        /* action : 0 / 1 / 2 = pass / drop / markphb */
        if(2 == cfg->red_action.action)
        {
            color_action.chg_red_pri_ena = 1;
            color_action.red_pri = cfg->red_action.markphb;
        }
        else if(1 == cfg->red_action.action)
        {
            color_action.chg_red_pri_ena = 0;
            color_action.red_drop_ena = 1;
        }
        else if(0 == cfg->red_action.action)
        {
            color_action.chg_red_pri_ena = 0;
            color_action.red_drop_ena = 0;
        }

        /* 将色感知动作模板写入C3驱动中 */
        for(unit = 0; unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_qos_set_policer_action_template(unit,&color_action);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_qos_free_color_action_id(id NBB_CCXT);
                spm_set_policer_action_template_error_log(unit,&color_action,
                    __FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }
#endif  
        }
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
NBB_LONG spm_qos_del_color_action(NBB_ULONG *id NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT unit = 0;
    
    //POLICER_ACTION_TEMPLATE_T color_action;

    if((NULL != id) && (0 != *id))
    {
        for(unit = 0; unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_qos_clear_policer_action_template(unit,*id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_clear_policer_color_action_template_error_log(unit,*id,
                    __FUNCTION__,__LINE__,ret NBB_CCXT);
            }
#endif
        }
        spm_qos_free_color_action_id(id NBB_CCXT);
    }

    return ret;
}

#endif


#ifdef PTN690

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
NBB_LONG spm_qos_set_bucket(NBB_ULONG id,ATG_DCI_QOS_BEHAVIOR_SUPERVISE* cfg NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT unit = 0;
    METER_CFG_T bucket;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == cfg)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }  

    bucket.cbs = cfg->cbs * 1024;
    bucket.pbsebs = cfg->pbs * 1024;
    bucket.cir = cfg->cir ;
    bucket.pireir = cfg->pir;

   /********************************* 
    * meter桶的模板id,非meter实例id *
    * 实例id = 模板id + offset      *
    *********************************/
    bucket.meterId = id;
    bucket.dropRed = 1;

    if(0 == cfg->cm_color)
    {
        bucket.colorBlind = 0;
    }
    else
    {
        bucket.colorBlind = 1;
    }
    
    if(3 == cfg->mode)
    {
         bucket.eMeterType = RFC_2697;
    }
    else if(2 == cfg->mode)
    {
        bucket.eMeterType = RFC_2698;
    }
    else
    {
        bucket.eMeterType = MEF;
    }

    for(unit = 0; unit < v_spm_shared->c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        /* 将meter桶的信息写入C3驱动中 */
        ret = fhdrv_qos_set_policer_bucket_template(unit,&bucket);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_set_policer_bucket_template_error_log(unit,&bucket,
                __FUNCTION__,__LINE__,ret NBB_CCXT);
            goto EXIT_LABEL;
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
NBB_LONG spm_qos_del_bucket(NBB_ULONG *id NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT unit = 0;

    if((NULL != id) && (0 != *id))
    {
        for(unit = 0; unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_qos_clear_policer_bucket_template(unit,*id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_clear_policer_bucket_template_error_log(unit,*id,
                    __FUNCTION__,__LINE__,ret NBB_CCXT);
            }
#endif
        }

        //spm_qos_free_meter_bucket_Id(id NBB_CCXT);
    }  

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
NBB_LONG spm_apply_policy_acl_id(SPM_QOS_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG acl_id = 0;
    NBB_LONG ret = 0;
    SPM_QOS_POLICY_C2B_CB *c2b = NULL;
    NBB_ULONG classify_id = 0;
    SPM_QOS_CLASSIFY_CB *pstClassify = NULL;

    if(NULL != pcb)
    {
       /*************************************************
        * 查找policy策略模板中是否含有cb对的配置;默认同 *
        * 一policy模板中所有cb对的classify匹配类型相同. *
        *************************************************/
        c2b = AVLL_FIRST(pcb->c2b_tree);
        if(NULL != c2b)
        {
            classify_id = c2b->c2b_key.qos_classify_index;
            pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST(
                (SHARED.g_classify_id_instance[classify_id].classify_group));
            if(NULL != pstClassify)
            {
                if(MATCH_PORT == pstClassify->match_type)
                {
                    ret = spm_apply_classify_port_acl_id(&acl_id NBB_CCXT);   
                }
                else if(MATCH_ETH_HEADER == pstClassify->match_type)
                {
                    ret = spm_apply_classify_eth_acl_id(&acl_id NBB_CCXT); 
                }
                else if(MATCH_IP_TCP == pstClassify->match_type)
                {
                    ret = spm_apply_classify_high_acl_id(&acl_id NBB_CCXT); 
                }
                else
                {
                    spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
                    acl_id = 0;
                }
                if(ATG_DCI_RC_OK != ret)
                {
                    acl_id = 0;
                }
            }
        }      
    }

    return acl_id;
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
NBB_LONG spm_del_c2b_acl(NBB_ULONG policy_id,NBB_ULONG acl_id,
    NBB_ULONG classify_id,NBB_ULONG action_id NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    ACL_T acl = {0};
    NBB_USHORT unit = 0;
    SPM_QOS_CLASSIFY_CB *pstClassify = NULL;
    SPM_QOS_ACTION_CB *pstAction = NULL;
    SPM_QOS_POLICY_SUPERVISE_CB *policy_supevise = NULL;
    SPM_QOS_POLICY_SUPERVISE_KEY policy_supervise_key;
    SPM_QOS_POLICY_CLASSIFY_CB *policy_classify = NULL;
    SPM_QOS_POLICY_CLASSIFY_KEY policy_classify_key;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /* 查找behavior和classify模块配置是否为空 */
    pstAction = AVLL_FIND(SHARED.qos_action_tree,&(action_id));
    pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((SHARED.g_classify_id_instance[classify_id].classify_group));
    if((NULL == pstAction) || (NULL == pstClassify))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    
    NBB_MEMSET(&acl,0,sizeof(ACL_T));

    /* policy策略模板绑定时所分配的acl_id */
    acl.mAclId = acl_id;

   /********************************************************************* 
    * 1.删除METER桶以及与behavior模块中策略流量监管树之间的插入关系.    *
    * 2.在policy绑定到intf上时,会将policy中cb对信息写入内存和驱动中.    *
    * 3.当写入成功后,会将METER桶相关信息(policy_id、classify_id、acl_id *
    *   及meter_bucket_id)插入behavior模块的策略流量监管树中.           *
    * 4.在一个policy策略模板中,一个behavior模块可能会对应着多个classify *
    *   模板,因此behavior模块中策略流量监管树的key需要包含classify_id,  *
    *   用于指明此时在policy中behavior分别与哪些classify模块组合成cb对. *
    *********************************************************************/
    if(NULL != pstAction)
    {
       /***************************************************************************** 
        * 策略流量监管节点存放的信息:meter桶的模板id、behavior模板被引用的policyid、*
        * 与behavior模板一起组合成cb对的classifyid、policy模板被绑定所分配的acl_id. *
        *****************************************************************************/
        policy_supervise_key.policy_id   = policy_id;
        policy_supervise_key.classify_id = classify_id;
        policy_supervise_key.acl_id      = acl_id;
        policy_supevise = AVLL_FIND(pstAction->spm_qos_supervise_tree,
                            &policy_supervise_key);
        if(NULL != policy_supevise)
        {
            spm_qos_del_bucket(&(policy_supevise->meter_bucket_id) NBB_CCXT);
            AVLL_DELETE(pstAction->spm_qos_supervise_tree,
                  policy_supevise->policy_supervise_node);
            spm_free_policy_supervise_cb(policy_supevise NBB_CCXT);
            policy_supevise = NULL;
        }
    }

   /********************************************************************* 
    * 1.删除ACL RULE以及与classify模块中策略classify树之间的插入关系.   *
    * 2.在policy绑定到intf上时,会将policy中cb对信息写入内存和驱动中.    *
    * 3.当写入成功后,会将与classify相关信息(policy_id、acl_id、rule_id及*
    *   posid)插入classify模块中策略classify树中.                       *
    * 4.在一个policy策略模板中,一个classify模块只可能对应着一个behavior *
    *   模板即一个policy的N个cb对中每个classifyid都是唯一的.因此classify*
    *   模块中策略classify树的key不含classify_id,树中的节点表示classify *
    *   模板被哪些policy策略模板(policy模板可能对应不同的aclid)所引用.  *
    *********************************************************************/
    for( ; pstClassify != NULL;  
            pstClassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(
            pstClassify->spm_classify_group_node))
    {
       /********************************************************************* 
        * 1.策略classify节点存放的信息:if-match规则被引用的policyid、policy *
        *   模板被绑定所分配的acl_id、成功写入acl规则后驱动返回的posid、该条*
        *   if-match规则所对应的acl规则的rule_id.                           *
        * 2.同一个classify模板可能会被多个policy策略所引用,在同一个policy策 *
        *   略中允许一个behavior对应多个classify,但是不允许多个behavior对应 *
        *   同一个classify,即一个policy的N个cb对中每个classifyid都是唯一的. * 
        *********************************************************************/
        policy_classify_key.policy_id = policy_id;
        policy_classify_key.acl_id    = acl_id;
        policy_classify = AVLL_FIND(pstClassify->spm_classify_policy_tree,
                                   &policy_classify_key);
        if(NULL != policy_classify)
        {
            acl.mRuleId = policy_classify->rule_id;
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3DelAcl(unit, &acl);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, 
                              __LINE__, ret NBB_CCXT);
                }
#endif
            }
            AVLL_DELETE(pstClassify->spm_classify_policy_tree,
                              policy_classify->tree_node);
            spm_free_policy_classify_cb(policy_classify NBB_CCXT);
            policy_classify = NULL;
        }
    }

   /******************************************************************* 
    * 1.最后释放色感知动作模板,behavior模板的策略流量监管树中所有节点 *
    *   共有同一个色感知动作模板.                                     *
    * 2.只有在behavior模板的策略流量监管树中没有任何节点后,才将色感知 *
    *   动作模板删除.                                                 *
    * 3.在单盘协议中,色感知动作模板只是流量监管子配置块中的一部分,如果*
    *   behavior模板的策略流量监管树为空,表示流量监管子配置块的所有配 *
    *   置数据被清空,那么此时也需要将色感知动作模板删除.              *
    *******************************************************************/
    if(NULL == AVLL_FIRST(pstAction->spm_qos_supervise_tree))
    {
       spm_qos_del_color_action(&(pstAction->color_action_id) NBB_CCXT );
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;  
}

/*****************************************************************************
   函 数 名  : spm_del_acl_by_policy
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
NBB_LONG spm_del_acl_by_policy(SPM_QOS_POLICY_CB *pcb,NBB_ULONG acl_id NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_C2B_CB *c2b_cb = NULL;
    NBB_ULONG classify_id = 0;
    NBB_ULONG action_id = 0;
    NBB_ULONG policy_id = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pcb))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    policy_id = pcb->policy_key;

    for(c2b_cb = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pcb->c2b_tree); c2b_cb != NULL;
        c2b_cb = (SPM_QOS_POLICY_C2B_CB *)AVLL_NEXT(pcb->c2b_tree, c2b_cb->spm_policy_c2b_node))
    {
        classify_id = c2b_cb->c2b_key.qos_classify_index;
        action_id = c2b_cb->c2b_key.qos_behavior_index;
        
       /*********************************************************** 
        * 删除cb对中内存和驱动中的信息,包括:meter桶模板、色感知动 *
        * 作模板、策略流量监管节点、策略classify节点、acl规则等.  *
        ***********************************************************/
        ret = spm_del_c2b_acl(policy_id,acl_id,classify_id,action_id  NBB_CCXT);
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
NBB_LONG spm_create_acl_rule_by_c2b(SPM_QOS_CLASSIFY_CB *pstClassify,
    SPM_QOS_ACTION_CB *pstAction,NBB_ULONG aclid,NBB_ULONG rule_id,
    NBB_ULONG bucket_id,NBB_ULONG *posid NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT unit = 0;
    SPM_PORT_INFO_CB stPortInfo = {0};
    NBB_ULONG nhi = 0;
    NBB_USHORT i = 0;
    NBB_BYTE tcp_flag = 0;
    NBB_USHORT UsSvlan = 0;
    ACL_T acl;
    ACL_T *pstAcl = NULL;
    
    //POLICER_ACTION_TEMPLATE_T color_action;
    SPM_PORT_INFO_CB pstPortInfo = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pstClassify) || (NULL == pstAction))
    {
        printf("NULL == pstClassify or pstAction");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(NULL == pstClassify->basic_cfg_cb)
    {
        printf("NULL == pstClassify->basic_cfg_cb");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    NBB_MEMSET(&acl,0,sizeof(ACL_T));
    pstAcl = &acl;

    pstAcl->mAclId = aclid;
    pstAcl->mRuleId = rule_id;
    if(0 != acl_pri_setting)
    {
        pstAcl->priority = rule_id;
    }
    
    switch(pstClassify->basic_cfg_cb->match_type)
    {
        case MATCH_PORT:
        if(NULL == pstClassify->Port_cfg_cb)
        {
           printf("NULL == pstClassify->Port_cfg_cb");
           ret = ATG_DCI_RC_UNSUCCESSFUL;
           goto EXIT_LABEL;
        }
        OS_MEMSET(&pstPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
        ret = spm_get_portid_from_logical_port_index(pstClassify->Port_cfg_cb->index, &pstPortInfo NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_get_portid_from_logical_port_index_error_log(pstClassify->Port_cfg_cb->index,
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
            /* 二层口 */
            if(0 == pstPortInfo.svlan)
            {
                ret = spm_get_vlan_from_logical_port_index(pstClassify->Port_cfg_cb->index, &UsSvlan);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_get_vlan_from_logical_port_index_error_log(pstClassify->Port_cfg_cb->index,
                            __FUNCTION__,__LINE__,ret NBB_CCXT);
                    goto EXIT_LABEL;
                }
                if(0 != UsSvlan)
                {
                    pstAcl->tAclRule.tAclKey.oVid  = UsSvlan;
                    pstAcl->tAclRule.tAclMask.oVid = 0x0fff;
                }
            }

            /* 三层口 */
            else
            {
                pstAcl->tAclRule.tAclKey.oVid  = pstPortInfo.svlan;
                pstAcl->tAclRule.tAclMask.oVid = 0x0fff;
            }
            pstAcl->tAclRule.tAclKey.ePortType  = ACL_PORT;
            pstAcl->tAclRule.tAclKey.port       = pstPortInfo.port_id;
            pstAcl->tAclRule.tAclMask.ePortType = 0xffff;
            pstAcl->tAclRule.tAclMask.port      = 0xffff;     
        }
        break;

        case MATCH_ETH_HEADER:
        if(NULL == pstClassify->Eth_cfg_cb)
        {
           printf("NULL == pstClassify->Eth_cfg_cb");
           ret = ATG_DCI_RC_UNSUCCESSFUL;
           goto EXIT_LABEL;
        }
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.smac[0]), &(pstClassify->Eth_cfg_cb->src_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.dmac[0]), &(pstClassify->Eth_cfg_cb->dst_mac[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.smac[0]), &(pstClassify->Eth_cfg_cb->src_mac_mask[0]), ATG_DCI_MAC_LEN);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.dmac[0]), &(pstClassify->Eth_cfg_cb->dst_mac_mask[0]), ATG_DCI_MAC_LEN);
        pstAcl->tAclRule.tAclKey.ovlan_pri = pstClassify->Eth_cfg_cb->vlan_prio;
        pstAcl->tAclRule.tAclMask.ovlan_pri = pstClassify->Eth_cfg_cb->vlan_prio_mask;
        pstAcl->tAclRule.tAclKey.ethType  = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_IPV4:
        if(NULL == pstClassify->ipv4_cfg_cb)
        {
           printf("NULL == pstClassify->ipv4_cfg_cb");
           ret = ATG_DCI_RC_UNSUCCESSFUL;
           goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.dip = pstClassify->ipv4_cfg_cb->dst_ip;
        pstAcl->tAclRule.tAclMask.dip = pstClassify->ipv4_cfg_cb->dst_ip_mask;
        pstAcl->tAclRule.tAclKey.sip = pstClassify->ipv4_cfg_cb->src_ip;
        pstAcl->tAclRule.tAclMask.sip = pstClassify->ipv4_cfg_cb->src_ip_mask;
        pstAcl->tAclRule.tAclKey.tos = (pstClassify->ipv4_cfg_cb->dscp);
        pstAcl->tAclRule.tAclMask.tos = (pstClassify->ipv4_cfg_cb->dscp_mask);
        pstAcl->tAclRule.tAclKey.l3Protocol = pstClassify->ipv4_cfg_cb->l3_protocol;
        pstAcl->tAclRule.tAclMask.l3Protocol = pstClassify->ipv4_cfg_cb->l3_protocol_mask;
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.ipHeaderMF = pstClassify->ipv4_cfg_cb->mf;
        pstAcl->tAclRule.tAclMask.ipHeaderMF = pstClassify->ipv4_cfg_cb->mf_mask;
        pstAcl->tAclRule.tAclKey.ipHeaderDF = pstClassify->ipv4_cfg_cb->df;
        pstAcl->tAclRule.tAclMask.ipHeaderDF = pstClassify->ipv4_cfg_cb->df_mask;
        pstAcl->tAclRule.tAclKey.l4SrcPort = pstClassify->ipv4_cfg_cb->src_port;
        pstAcl->tAclRule.tAclMask.l4SrcPort = pstClassify->ipv4_cfg_cb->src_port_mask;
        pstAcl->tAclRule.tAclKey.l4DstPort = pstClassify->ipv4_cfg_cb->dst_port;
        pstAcl->tAclRule.tAclMask.l4DstPort = pstClassify->ipv4_cfg_cb->dst_port_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        if((1 == pstClassify->ipv4_cfg_cb->l3_protocol) 
            && (0xff == pstClassify->ipv4_cfg_cb->l3_protocol_mask) 
            && (0 != pstClassify->ipv4_cfg_cb->payload_mask[0])) 
        {
            pstAcl->tAclRule.tAclKey.icmpType = pstClassify->ipv4_cfg_cb->payload[0];
            pstAcl->tAclRule.tAclMask.icmpType = pstClassify->ipv4_cfg_cb->payload_mask[0];
        }
        if((1 == pstClassify->ipv4_cfg_cb->l3_protocol) 
            && (0xff == pstClassify->ipv4_cfg_cb->l3_protocol_mask) 
            && (0 != pstClassify->ipv4_cfg_cb->payload_mask[1])) 
        {
            pstAcl->tAclRule.tAclKey.icmpCode = pstClassify->ipv4_cfg_cb->payload[1];
            pstAcl->tAclRule.tAclMask.icmpCode = pstClassify->ipv4_cfg_cb->payload_mask[1];
        }
        break;

        case MATCH_IP_TCP:
        if(NULL == pstClassify->ipTcp_cfg_cb)
        {
            printf("NULL == pstClassify->ipTcp_cfg_cb");
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.dip = pstClassify->ipTcp_cfg_cb->dst_ip;
        pstAcl->tAclRule.tAclMask.dip = pstClassify->ipTcp_cfg_cb->dst_ip_mask;
        pstAcl->tAclRule.tAclKey.sip = pstClassify->ipTcp_cfg_cb->src_ip;
        pstAcl->tAclRule.tAclMask.sip = pstClassify->ipTcp_cfg_cb->src_ip_mask;
        pstAcl->tAclRule.tAclKey.tos = (pstClassify->ipTcp_cfg_cb->dscp);
        pstAcl->tAclRule.tAclMask.tos = (pstClassify->ipTcp_cfg_cb->dscp_mask);
        pstAcl->tAclRule.tAclKey.l3Protocol = 6;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l4SrcPort = pstClassify->ipTcp_cfg_cb->src_port;
        pstAcl->tAclRule.tAclMask.l4SrcPort = pstClassify->ipTcp_cfg_cb->src_port_mask;
        pstAcl->tAclRule.tAclKey.l4DstPort = pstClassify->ipTcp_cfg_cb->dst_port;
        pstAcl->tAclRule.tAclMask.l4DstPort = pstClassify->ipTcp_cfg_cb->dst_port_mask; 
        OS_MEMCPY(&tcp_flag,&(pstClassify->ipTcp_cfg_cb->tcp_flag),1);
        pstAcl->tAclRule.tAclKey.tcp_flag = tcp_flag;
        pstAcl->tAclRule.tAclMask.tcp_flag = pstClassify->ipTcp_cfg_cb->tcp_flag_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_IP_UDP:
        if(NULL == pstClassify->ipUdp_cfg_cb)
        {
            printf("NULL == pstClassify->ipUdp_cfg_cb");
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.dip = pstClassify->ipUdp_cfg_cb->dst_ip;
        pstAcl->tAclRule.tAclMask.dip = pstClassify->ipUdp_cfg_cb->dst_ip_mask;
        pstAcl->tAclRule.tAclKey.sip = pstClassify->ipUdp_cfg_cb->src_ip;
        pstAcl->tAclRule.tAclMask.sip = pstClassify->ipUdp_cfg_cb->src_ip_mask;
        pstAcl->tAclRule.tAclKey.tos = (pstClassify->ipUdp_cfg_cb->dscp);
        pstAcl->tAclRule.tAclMask.tos = (pstClassify->ipUdp_cfg_cb->dscp_mask);
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->tAclRule.tAclKey.l4SrcPort = pstClassify->ipUdp_cfg_cb->src_port;
        pstAcl->tAclRule.tAclMask.l4SrcPort = pstClassify->ipUdp_cfg_cb->src_port_mask;
        pstAcl->tAclRule.tAclKey.l4DstPort = pstClassify->ipUdp_cfg_cb->dst_port;
        pstAcl->tAclRule.tAclMask.l4DstPort = pstClassify->ipUdp_cfg_cb->dst_port_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_IPV6:
        if(NULL == pstClassify->ipv6_cfg_cb)
        {
            printf("NULL == pstClassify->ipv6_cfg_cb");
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        OS_MEMCPY(pstAcl->tAclRule.tAclKey.dipv6,pstClassify->ipv6_cfg_cb->dst_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(pstClassify->ipv6_cfg_cb->dst_ip_mask_len,
            (NBB_ULONG*)&(pstAcl->tAclRule.tAclMask.dipv6) NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        OS_MEMCPY(pstAcl->tAclRule.tAclKey.sipv6,pstClassify->ipv6_cfg_cb->src_ip,ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        ret = spm_get_ipv6_mask(pstClassify->ipv6_cfg_cb->src_ip_mask_len,
            (NBB_ULONG*)&(pstAcl->tAclRule.tAclMask.sipv6) NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.l3Protocol = pstClassify->ipv6_cfg_cb->l3_protocol;
        pstAcl->tAclRule.tAclMask.l3Protocol = pstClassify->ipv6_cfg_cb->l3_protocol_mask;
        pstAcl->tAclRule.tAclKey.l4SrcPort = pstClassify->ipv6_cfg_cb->src_port;
        pstAcl->tAclRule.tAclMask.l4SrcPort = pstClassify->ipv6_cfg_cb->src_port_mask;
        pstAcl->tAclRule.tAclKey.l4DstPort = pstClassify->ipv6_cfg_cb->dst_port;
        pstAcl->tAclRule.tAclMask.l4DstPort = pstClassify->ipv6_cfg_cb->dst_port_mask;
        pstAcl->tAclRule.tAclKey.tos = pstClassify->ipv6_cfg_cb->tos;
        pstAcl->tAclRule.tAclMask.tos = pstClassify->ipv6_cfg_cb->tos_mask;
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        case MATCH_NOIP:
        if(NULL == pstClassify->noIp_cfg_cb)
        {
            printf("NULL == pstClassify->noIp_cfg_cb\n");
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        pstAcl->tAclRule.tAclKey.ethType = pstClassify->noIp_cfg_cb->etype;
        pstAcl->tAclRule.tAclMask.ethType = pstClassify->noIp_cfg_cb->etype_mask;
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData6),&(pstClassify->noIp_cfg_cb->payload[0]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData5),&(pstClassify->noIp_cfg_cb->payload[4]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclKey.customerData4),&(pstClassify->noIp_cfg_cb->payload[8]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData6),&(pstClassify->noIp_cfg_cb->payload_mask[0]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData5),&(pstClassify->noIp_cfg_cb->payload_mask[4]),4);
        OS_MEMCPY(&(pstAcl->tAclRule.tAclMask.customerData4),&(pstClassify->noIp_cfg_cb->payload_mask[8]),4);
        pstAcl->flags |= ACL_COUNTER_ON;
        break;

        default:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;
    }

    /* C2B对中与classify配对的action模块中,复杂流分类的动作flow_act_cfg_cb不允许为空 */
    if(NULL == pstAction->flow_act_cfg_cb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("%s line=%d spm_set_policy_classify_acl err : flow_act_cfg_cb "
                  "of pstAction is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
        OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_classify_acl err : flow_act_cfg_cb "
                   "of pstAction is NULL. ret = %ld\n\n",__FUNCTION__,__LINE__,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        		       QOS_ERROR_STRING,ret,
        		       ucMessage,__FUNCTION__,"LINE","",
        		       0,0,__LINE__,0)); 
        goto EXIT_LABEL; 
    }
    if(0 == pstAction->flow_act_cfg_cb->action)
    {
        pstAcl->eAclAction = ACL_ACTION_DROP;
    }
    else if(1 == pstAction->flow_act_cfg_cb->action)
    {
        pstAcl->eAclAction = ACL_ACTION_PASS;
    }
    else if(3 == pstAction->flow_act_cfg_cb->action)
    {
        /* 当动作是重定向时,重定向配置块不允许为空 */
        if(NULL == pstAction->redirect_cfg_cb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_policy_classify_acl err : redirect_cfg_cb "
                      "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", __FUNCTION__,
                      __LINE__, ret, pstAction->action_key);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_classify_acl err : redirect_cfg_cb "
                       "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", __FUNCTION__,
                       __LINE__, ret, pstAction->action_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            		       QOS_ERROR_STRING,ret,
            		       ucMessage,__FUNCTION__,"LINE","",
            		       0,0,__LINE__,0)); 

            goto EXIT_LABEL;
        }
        if(0 == pstAction->redirect_cfg_cb->action)
        {
            pstAcl->eAclAction = ACL_ACTION_PASS;
        }
        else if(1 == pstAction->redirect_cfg_cb->action)
        {
            pstAcl->eAclAction = ACL_ACTION_VRF;
            pstAcl->vrfId = pstAction->redirect_cfg_cb->vrf_d;
        }
        else if(2 == pstAction->redirect_cfg_cb->action)
        {
            if(0 == pstAction->redirect_cfg_cb->out_port_index)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("%s line=%d spm_set_policy_classify_acl err : redirect_cfg_cb "
                          "of pstAction out_port_index is 0. ret = %ld, action_id = %ld\n\n", 
                          __FUNCTION__, __LINE__, ret, pstAction->action_key);
                OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_classify_acl err : redirect_cfg_cb "
                           "of pstAction out_port_index is 0. ret = %ld, action_id = %ld\n\n", 
                           __FUNCTION__, __LINE__, ret, pstAction->action_key);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                		       QOS_ERROR_STRING,ret,
                		       ucMessage,__FUNCTION__,"LINE","",
                		       0,0,__LINE__,0)); 
                goto EXIT_LABEL;
            }
            ret = spm_get_portid_from_logical_port_index(   \
                            pstAction->redirect_cfg_cb->out_port_index,&stPortInfo NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                spm_get_portid_from_logical_port_index_error_log(pstAction->redirect_cfg_cb->out_port_index,
                        __FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }
            pstAcl->slot = stPortInfo.slot_id;
            pstAcl->cardPort = stPortInfo.port_id;
            pstAcl->eAclAction = ACL_ACTION_REDIRECTION;
        }
    }
    else if(4 == pstAction->flow_act_cfg_cb->action)
    {
        pstAcl->eAclAction = ACL_ACTION_NHI;
        if(NULL == pstAction->policy_nhi_cfg_cb)
        {   
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("%s line=%d spm_set_policy_classify_acl err : policy_nhi_cfg_cb "
                      "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", 
                      __FUNCTION__, __LINE__, ret, pstAction->action_key);
            OS_SPRINTF(ucMessage,"%s line=%d spm_set_policy_classify_acl err : policy_nhi_cfg_cb "
                       "of pstAction is NULL. ret = %ld, action_id = %ld\n\n", 
                       __FUNCTION__, __LINE__, ret, pstAction->action_key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            		       QOS_ERROR_STRING,ret,
            		       ucMessage,__FUNCTION__,"LINE","",
            		       0,0,__LINE__,0)); 
            goto EXIT_LABEL;
        }
        for(i = 0;i < pstAction->policy_nhi_cfg_cb->nxhop_num;i++)
        {
            if(0 == pstAction->policy_nhi_cfg_cb->nxhp[i].ip_type)
            {
                ret = spm_l3_lookupuni(0, &(pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_ip[3]),
                pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,&nhi NBB_CCXT);
            }
            else
            {
                ret = spm_l3_lookupuni(1, &(pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_ip[0]),
                pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,&nhi NBB_CCXT);
            }
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                spm_l3_lookupuni_error_log(pstAcl->mAclId,pstAcl->mRuleId,pstAction->action_key,__FUNCTION__,
                       __LINE__,pstAction->policy_nhi_cfg_cb->nxhp[i].nxhop_port_index,ret NBB_CCXT);
                goto EXIT_LABEL;
            }
            pstAcl->nhIdx = nhi;
        }
    }

    /* 配置meter桶和动作*/
    if(NULL != pstAction->supervise_cfg_cb)
    {
        /* meter桶模板 */
        pstAcl->meterIdx = bucket_id;
        pstAcl->flags |= ACL_METER_ON;
        if(0 != pstAction->color_action_id)
        {
            /* 色感知动作模板 */
            pstAcl->policer_action_id = pstAction->color_action_id;
        }
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
        else
        {
            /* 写入成功则返回该条acl rule规则所对应的posid */
            *posid = pstAcl->posId;
        }
#endif
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名  : spm_create_c2b_acl
   功能描述  : 处理一个cb对中的信息并写入驱动中
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
NBB_LONG spm_create_c2b_acl(NBB_ULONG policy_id,NBB_ULONG acl_id,NBB_ULONG *rule_id,
    NBB_ULONG classify_id,NBB_ULONG action_id NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_ULONG bucket_id = 0;
    NBB_ULONG posid = 0;
    SPM_QOS_CLASSIFY_CB *pstClassify = NULL;
    SPM_QOS_ACTION_CB *pstAction = NULL;
    SPM_QOS_POLICY_SUPERVISE_KEY policy_supervise_key;
    SPM_QOS_POLICY_CLASSIFY_KEY policy_classify_key;
    SPM_QOS_POLICY_SUPERVISE_CB *policy_supevise = NULL;
    SPM_QOS_POLICY_CLASSIFY_CB *policy_classify = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(classify_id >= MAX_FLOW_CLASSIFY_PRI)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 查找behavior模板配置和classify链表中所有的classify规则配置 */
    pstAction = AVLL_FIND(SHARED.qos_action_tree,&(action_id));
    pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((SHARED.g_classify_id_instance[classify_id].classify_group));
    if((NULL == pstAction) || (NULL == pstClassify) || (NULL == rule_id))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

   /********************************************************** 
    * (开始处理cb对中behavior模板的内容)                     *
    * 将behavior模板中流量监管子配置块的内容写入驱动中,包括: *
    * 色感知动作模板、meter桶模板、以及将策略流量监管节点插  *
    * 入behavior模块的监管树(spm_qos_supervise_tree)中.      *
    **********************************************************/
    if((NULL != pstAction->supervise_cfg_cb) && (0 != pstAction->supervise_cfg_cb->mode))
    {
       /************************************************************** 
        * 申请色感知动作模板的模板id,并将色感知动作模板写入C3驱动中; *
        * 其中:目前驱动由于资源限制,最多只能申请4000个色感知动作模板 *
        **************************************************************/
        ret = spm_qos_set_color_action(&(pstAction->color_action_id),
              pstAction->supervise_cfg_cb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        
       /************************************************************* 
        * 申请meter桶的模板id,目前最多只能申请512个meter桶;新方案将 *
        * behavior动作模板拆分为了色感知动作模板和meter桶模板两部分.*
        *************************************************************/
        ret = spm_qos_apply_meter_bucket_id(&bucket_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        /* 调用C3驱动将cir、pir等值写入meter桶的模板id中 */
        ret = spm_qos_set_bucket(bucket_id,pstAction->supervise_cfg_cb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_free_meter_bucket_id(&bucket_id NBB_CCXT);
            goto EXIT_LABEL;
        }

        /* 调用驱动正确,则插入树中 */
        else
        {
            policy_supervise_key.policy_id = policy_id;
            policy_supervise_key.classify_id = classify_id;
            policy_supervise_key.acl_id = acl_id;

           /***************************************************************************** 
            * 策略流量监管节点存放的信息:meter桶的模板id、behavior模板被引用的policyid、*
            * 与behavior模板一起组合成cb对的classifyid、policy模板被绑定所分配的acl_id. *
            *****************************************************************************/
            policy_supevise = spm_alloc_policy_supervise_cb(&policy_supervise_key NBB_CCXT);
            if(NULL != policy_supevise)
            {
                policy_supevise->meter_bucket_id = bucket_id;

                /* 将策略流量监管节点插入behavior模块的监管树(spm_qos_supervise_tree)中 */
                //coverity[no_effect_test]
                rv = AVLL_INSERT(pstAction->spm_qos_supervise_tree,
                        policy_supevise->policy_supervise_node);
            }                
        }        
    }

    /* 开始处理cb对中classify模板的内容 */
    for ( ; pstClassify != NULL; pstClassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(
            pstClassify->spm_classify_group_node))
    {   
        /* rule_id对应着policy的cb中所有的if-match规则 */
        (*rule_id)++;

         /* posid是成功写入acl规则后驱动的返回值 */
         ret = spm_create_acl_rule_by_c2b(pstClassify,pstAction,acl_id,
                (*rule_id),bucket_id,&posid NBB_CCXT);
         if(ATG_DCI_RC_OK != ret)
         {
            goto EXIT_LABEL;
         }
         else
         {
            policy_classify_key.policy_id = policy_id;
            policy_classify_key.acl_id    = acl_id;

           /********************************************************************* 
            * 1.策略classify节点存放的信息:if-match规则被引用的policyid、policy *
            *   模板被绑定所分配的acl_id、成功写入acl规则后驱动返回的posid、该条*
            *   if-match规则所对应的acl规则的rule_id.                           *
            * 2.同一个classify模板可能会被多个policy策略所引用,在同一个policy策 *
            *   略中允许一个behavior对应多个classify,多少不允许多个behavior对应 *
            *   同一个classify,即一个policy的N个cb对中每个classifyid都是唯一的. * 
            *********************************************************************/
            policy_classify = spm_alloc_policy_classify_cb(&policy_classify_key NBB_CCXT);
            if(NULL != policy_classify)
            {
               policy_classify->posid = posid;
               policy_classify->rule_id = *rule_id;
               rv = AVLL_INSERT((pstClassify->spm_classify_policy_tree),
                                     (policy_classify->tree_node));
            } 
         }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_create_acl_by_policy
   功能描述  : 申请acl_id并将policy策略模板中cb对的信息写入驱动中
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
NBB_LONG  spm_create_acl_by_policy(SPM_QOS_POLICY_CB *pcb,
    NBB_ULONG *acl_id NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG id = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_C2B_CB *c2b_cb = NULL;
    NBB_ULONG classify_id = 0;
    NBB_ULONG action_id = 0;
    NBB_ULONG policy_id = 0;
    NBB_ULONG old_rule = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pcb) || (NULL == acl_id))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 申请policy策略所对应的acl_id,为0表示申请失败 */
    id = spm_apply_policy_acl_id(pcb);
    if(0 == id)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    policy_id = pcb->policy_key;
    old_rule  = pcb->rule_resouce;
    for(c2b_cb = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pcb->c2b_tree); c2b_cb != NULL;
        c2b_cb = (SPM_QOS_POLICY_C2B_CB *)AVLL_NEXT(pcb->c2b_tree, c2b_cb->spm_policy_c2b_node))
    {
        classify_id = c2b_cb->c2b_key.qos_classify_index;
        action_id   = c2b_cb->c2b_key.qos_behavior_index;

        /* 将classify对应的规则与behavior对应的meter动作一起写入C3的acl驱动中 */
        ret = spm_create_c2b_acl(policy_id,id,&(pcb->rule_resouce),
                    classify_id, action_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
           /*********************************************************** 
            * 删除cb对中内存和驱动中的信息,包括:meter桶模板、色感知动 *
            * 作模板、策略流量监管节点、策略classify节点、acl规则等.  *
            ***********************************************************/
            spm_del_acl_by_policy(pcb,id NBB_CCXT);
            spm_release_policy_acl_id(id NBB_CCXT);

            /* cb对写入驱动失败后,将rule_resouce还原为成功写入驱动中的acl规则条目数 */
            pcb->rule_resouce = old_rule;
            goto EXIT_LABEL;
        }
    }

    *acl_id = id;
         
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*返回1-128*/

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
NBB_ULONG  spm_apply_policy_intf_offset(NBB_BYTE *resoure NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG i = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL != resoure)
    {
        for(i = 1;i < POLICY_C2B_INSTANCE_NUM + 1;i++)
        {
            if(ATG_DCI_RC_OK == resoure[i])
            {
                resoure[i] = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return i;
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
NBB_VOID  spm_release_policy_intf_offset(NBB_BYTE *resoure,NBB_ULONG id NBB_CCXT_T NBB_CXT)
{   
    //NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL != resoure) && (id < POLICY_C2B_INSTANCE_NUM + 1))
    {
        if(ATG_DCI_RC_OK != resoure[id])
        {
            resoure[id] = ATG_DCI_RC_OK;
        }
    }
    
    //EXIT_LABEL: NBB_TRC_EXIT();
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
NBB_LONG spm_apply_policy_acl_drive(
    SPM_QOS_POLICY_CB *pcb,NBB_ULONG *acl_id,NBB_USHORT *intf_offset,
    NBB_USHORT *sz_offset NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT i = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pcb) || (NULL == pcb->basic_cfg_cb) || 
        (NULL == acl_id) || (NULL == intf_offset) || (NULL == sz_offset))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 非共享模式 */
    if(0 == pcb->basic_cfg_cb->share)
    {
        /* POLICY_C2B_INSTANCE_MAX:一个polic策略模板允许被绑定的最大次数(128次)的倍数 */
        for(i = 0; i < POLICY_C2B_INSTANCE_MAX;i++)
        {
            /* 硬件资源已经充足,表示该policy策略已经被intf绑定过了 */
            if((0 != pcb->c2b_instance_tree[i].acl_id) && 
               (pcb->c2b_instance_tree[i].cnt < POLICY_C2B_INSTANCE_NUM))
            {
               *acl_id = pcb->c2b_instance_tree[i].acl_id;

               /* 申请intf_offset,不会对驱动和内存进行任何操作 */
               *intf_offset = spm_apply_policy_intf_offset(
                    &(pcb->c2b_instance_tree[i].offset[0]) NBB_CCXT);

               /* cnt:表示该policy策略模板所分配的第i个acl_id被intf绑定的次数 */
               (pcb->c2b_instance_tree[i].cnt)++;
               *sz_offset = i;
               goto EXIT_LABEL;
            }
        }
        
        /* 开辟新的硬件资源 */
        for(i = 0; i < POLICY_C2B_INSTANCE_MAX;i++)
        {
           /************************************************************* 
            * 1.当一个policy策略模板被绑定少于128次时,只对应一个acl_id; *
            *   当绑定次数多于128次时,就会申请新的acl_id.因此一个policy *
            *   策略模板根据它被绑定次数的多少可能会对应多个acl_id值.   *
            * 2.POLICY_C2B_INSTANCE_MAX即一个polic策略模板允许被绑定的  *
            *   最大次数(128次)的倍数,亦即一个policy策略模板最多可能对  *
            *   应上POLICY_C2B_INSTANCE_MAX个acl_id值.                  *
            *************************************************************/
            if(0 == pcb->c2b_instance_tree[i].acl_id)
            {
               /*************************************************************************
                * 1.此时acl_id是上话复杂流分类节点中intf绑定的policy模板所对应的acl_id. *
                * 2.调用该函数的目的是申请acl_id,并将policy策略模板中的所有cb对信息存入 *
                *   内存和C3芯片驱动中.                                                 *
                * 3.写入内存和驱动成功后,再将相关的数据信息存入behavior模板的策略流量监 *
                *   管树和classify模板中的策略classify树中.用于表示cb对中classify模板id *
                *   和behavior模板id与policy_id、acl_id、meter桶模板、acl规则posid等信息*
                *   之间的对应关系.                                                     *
                *************************************************************************/
                ret = spm_create_acl_by_policy(pcb,acl_id NBB_CCXT);
                if(ATG_DCI_RC_OK != ret)
                {
                   goto EXIT_LABEL;
                }

                /* 保存数据和写驱动正常 */
                else
                {
                   /************************************************* 
                    * 申请policy策略模板绑定时的meter桶模板的offset *
                    * 同一个policy所对应的acl_id最多允许被绑定128次.*
                    *************************************************/
                    *intf_offset = spm_apply_policy_intf_offset(
                        &(pcb->c2b_instance_tree[i].offset[0]) NBB_CCXT);

                   /***************************************************************
                    * intf_offset:该intf绑定policy时,使用的meter桶模板的offset值; *
                    * sz_offset  :该intf绑定policy时,使用的是policy策略模板所分配 *
                    *             的第几个acl_id号.一个policy策略模板最多只能对应 *
                    *             POLICY_C2B_INSTANCE_MAX个acl_id号.              *
                    * acl_id     :该intf绑定policy时,policy策略模板所分配的acl_id.*
                    ***************************************************************/
                    *sz_offset = i;
                    pcb->c2b_instance_tree[i].acl_id = *acl_id;

                    /* policy策略模板所分配的acl_id被intf绑定的次数 */
                    (pcb->c2b_instance_tree[i].cnt)++;
                    goto EXIT_LABEL;
                }
            }
        }
        
        /* 资源不足 */
        ret = ATG_DCI_RC_NO_RESOURCE;
        goto EXIT_LABEL;
    }
    else
    {
        if(0 == pcb->acl_id)
        {
            ret = spm_create_acl_by_policy(pcb, acl_id NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
               goto EXIT_LABEL;
            }
            else
            {
                *intf_offset = 1;
                *sz_offset   = 0;
                pcb->acl_id  = *acl_id;
                goto EXIT_LABEL;
            }
        }
        else
        {
            *intf_offset = 1;
            *sz_offset = 0;
            *acl_id = pcb->acl_id;
        }
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
NBB_VOID spm_release_policy_acl_drive(
    SPM_QOS_POLICY_CB *pcb,NBB_ULONG acl_id,NBB_ULONG intf_offset,
    NBB_ULONG sz_offset NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_FLOW_CLASSIFY_CB *cfg = NULL;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == pcb) || (NULL == pcb->basic_cfg_cb) || 
        (sz_offset >= POLICY_C2B_INSTANCE_MAX) || 
        (intf_offset > POLICY_C2B_INSTANCE_NUM))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 非共享模式 */
    if(0 == pcb->basic_cfg_cb->share)
    {
         /* 释放offset资源,不调用任何驱动 */
        spm_release_policy_intf_offset(&(pcb->c2b_instance_tree[sz_offset].offset[0]),
                        intf_offset NBB_CCXT);

        /* cnt:该policy策略所对应的第sz_offset个acl_id被intf所绑定的次数 */
        if(pcb->c2b_instance_tree[sz_offset].cnt > 0)
        {
            (pcb->c2b_instance_tree[sz_offset].cnt)--;
        }

        /* 查找此时policy策略模板是否被intf绑定 */
        cfg = AVLL_FIRST(pcb->c2b_instance_tree[sz_offset].instance_tree);

       /*********************************************************** 
        * 此时policy策略模板并没有被任何intf所绑定.若此时policy策 *
        * 略模板还被其他intf绑定,则不对内存和驱动做任何删除操作.  * 
        ***********************************************************/
        if(NULL == cfg)
        {
            /* 此时policy策略引用计数cnt必须为0,不然程序有问题 */
            if(0 == pcb->c2b_instance_tree[sz_offset].cnt)
            {
               /*********************************************************** 
                * 删除cb对中内存和驱动中的信息,包括:meter桶模板、色感知动 *
                * 作模板、策略流量监管节点、策略classify节点、acl规则等.  *
                ***********************************************************/
                spm_del_acl_by_policy(pcb,acl_id NBB_CCXT);

                /* 释放policy策略所分配的acl_id,不对驱动和内存进行任何操作 */
                spm_release_policy_acl_id(acl_id NBB_CCXT);
                pcb->c2b_instance_tree[sz_offset].acl_id = 0;
            }
            else
            {
                spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
            }
        } 
        goto EXIT_LABEL;
    }

    /* 共享模式 */
    else
    {
        cfg = AVLL_FIRST(pcb->instance_tree);
        if(NULL == cfg)/* 原来没有绑定关系 */
        {
           spm_del_acl_by_policy(pcb,acl_id NBB_CCXT);
           spm_release_policy_acl_id(acl_id NBB_CCXT);
           pcb->acl_id = 0;
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
NBB_LONG spm_add_flow_classify_drive(
    SPM_QOS_FLOW_CLASSIFY_CB* pcb NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *pB = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pcb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 查找policy策略模板配置,其中模式必须是多cb对模式 */
    pB = spm_qos_find_policy_cb(pcb->qos_cfg.qos_policy_index NBB_CCXT);
    if ((NULL == pB) || (NULL == pB->basic_cfg_cb) || 
        (0 == pB->basic_cfg_cb->mode))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

   /******************************************************************** 
    * pcb->intf_offset:该intf绑定policy时,使用的meter桶模板的offset值; *
    * pcb->sz_offset  :该intf绑定policy时,使用的是policy策略模板所分配 *
    *                  的第几个acl_id号.一个policy策略模板最多只能对应 *
    *                  POLICY_C2B_INSTANCE_MAX个acl_id号.              *
    * pcb->acl_id     :该intf绑定policy时,policy策略模板所分配的acl_id.*
    ********************************************************************/
    ret = spm_apply_policy_acl_drive(pB,&(pcb->acl_id),&(pcb->intf_offset),
                &(pcb->sz_offset) NBB_CCXT);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

#if defined (SPU) || defined (PTN690_CES)

    /* 将meter桶模板的offset值、intf的posid、policy策略模板所分配的acl_id绑定 */
    ret = fhdrv_intf_set_policer_bucket_offset(pcb->sub_port.unit,
          pcb->sub_port.posid,pcb->intf_offset,pcb->acl_id);

    /* 若绑定失败,则将前面写入驱动中的policy策略中cb对的信息全部清除 */
    if(ATG_DCI_RC_OK != ret)
    {
        spm_release_policy_acl_drive(pB,(pcb->acl_id),(pcb->intf_offset),
            (pcb->sz_offset) NBB_CCXT);
        spm_intf_set_policer_bucket_offset_error_log(pcb->sub_port.unit,
            pcb->sub_port.posid,pcb->intf_offset,pcb->acl_id,
            __FUNCTION__,__LINE__,ret NBB_CCXT);
        goto EXIT_LABEL;
    }
#endif

    /* 调用驱动成功以后增加intf与policy策略模板树之间的绑定关系 */

    /* 非共享模式 */
    if(0 == pB->basic_cfg_cb->share)
    {
        /* 查寻policy策略模板被哪些intf所绑定 */
        if(NULL == AVLL_FIND(pB->c2b_instance_tree[pcb->sz_offset].instance_tree,
                             &(pcb->key)))
        {
            rv = AVLL_INSERT(pB->c2b_instance_tree[pcb->sz_offset].instance_tree,
                             pcb->spm_classify_id_node);
        }
    }

    /* 共享模式 */
    else
    {
        if(NULL == AVLL_FIND(pB->instance_tree,&(pcb->key)))
        {
            rv = AVLL_INSERT(pB->instance_tree,pcb->spm_classify_id_node);
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_del_flow_classify_drive
   功能描述  : 删除intf上话复杂流分类节点信息
   输入参数  : SPM_QOS_FLOW_CLASSIFY_CB:intf上话复杂流分类节点信息
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_del_flow_classify_drive(
    SPM_QOS_FLOW_CLASSIFY_CB* pcb NBB_CCXT_T NBB_CXT)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *pB = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pcb)
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    pB = spm_qos_find_policy_cb(pcb->qos_cfg.qos_policy_index NBB_CCXT);
    if ((NULL == pB) || (NULL == pB->basic_cfg_cb) || 
        (0 == pB->basic_cfg_cb->mode))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

#if defined (SPU) || defined (PTN690_CES)

    /* 删除intf与acl、bucket_offset之间的绑定关系 */
    ret = fhdrv_intf_set_policer_bucket_offset(pcb->sub_port.unit,
          pcb->sub_port.posid,0,0);
    if(ATG_DCI_RC_OK != ret)
    {
        spm_intf_set_policer_bucket_offset_error_log(pcb->sub_port.unit,
            pcb->sub_port.posid,0,0,
            __FUNCTION__,__LINE__,ret NBB_CCXT);
    }
#endif   

    /* 非共享模式,删除policy策略模板中intf上话复杂流分类树的绑定关系 */
    if(0 == pB->basic_cfg_cb->share)
    {
        if(NULL != AVLL_FIND(pB->c2b_instance_tree[pcb->sz_offset].instance_tree,
               &(pcb->key)))
        {
            AVLL_DELETE(pB->c2b_instance_tree[pcb->sz_offset].instance_tree,
                pcb->spm_classify_id_node);
        }
    }

    /* 共享模式 */
    else
    {
        if(NULL != AVLL_FIND(pB->instance_tree,&(pcb->key)))
        {
            AVLL_DELETE(pB->instance_tree,pcb->spm_classify_id_node);
        }
    }

   /********************************************************* 
    * 删除policy策略模板中cb对的信息,包括: 释放intf_offset、*
    * 引用计数值自减、删除驱动中behavior及classify的配置等. *
    *********************************************************/
    spm_release_policy_acl_drive(pB,(pcb->acl_id),(pcb->intf_offset),
            (pcb->sz_offset) NBB_CCXT);

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
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *classify_qos, SUB_PORT *sub_port NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_FLOW_CLASSIFY_CB *join_node = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    
    //SPM_QOS_POLICY_CB *pB = NULL;
    //SPM_QOS_POLICY_CB *pB_old = NULL;
    NBB_ULONG index = 0;
    SPM_QOS_LOGIC_INTF_KEY log_key = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((0 == key) || (NULL == classify_qos) || (NULL == sub_port))
    {
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_log_cfg_print)
    {
        printf("%s %s,%d log_key=%ld,unit=%d,slot=%d,port=%d,posid=%ld,policy_index=%ld,"
               "svlan=%d\n\n",QOS_CFG_STRING,__FUNCTION__,__LINE__,
               key,sub_port->unit,sub_port->slot,sub_port->port,sub_port->posid,
               classify_qos->qos_policy_index,classify_qos->svlan);
        OS_SPRINTF(ucMessage,"%s %s,%d log_key=%ld,unit=%d,slot=%d,port=%d,posid=%ld,policy_index=%ld,"
                   "svlan=%d\n\n",QOS_CFG_STRING,__FUNCTION__,__LINE__,key,sub_port->unit,
                   sub_port->slot,sub_port->port,sub_port->posid,classify_qos->qos_policy_index,
                   classify_qos->svlan);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }
    index = classify_qos->qos_policy_index;
    log_key.svlan = classify_qos->svlan;
    log_key.cvlan = classify_qos->cvlan;
    log_key.index = key;

    /* 查找该intf上是否已经绑定了policy策略 */
    join_node = AVLL_FIND(v_spm_shared->qos_flow_classify_tree, &log_key);

    /* 增加 */
    if(NULL == join_node)
    {
        /* 删除不存在的配置 */
        if(0 == index)
        {
            goto EXIT_LABEL;
        }
        
       /************************************************************************** 
        * 申请intf上话复杂流分类节点.在intf与policy策略模板绑定成功后,会将该节点 *
        * 分别插入intf的上话复杂流分类的引用树(qos_flow_classify_tree)和policy策 *
        * 略模板的引用树(c2b_instance_tree[i].instance_tree)中.                  *
        **************************************************************************/
        join_node = spm_alloc_qos_flow_classify_cb(&log_key NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        OS_MEMCPY(&(join_node->sub_port),sub_port,sizeof(SUB_PORT));
        OS_MEMCPY(&(join_node->qos_cfg),classify_qos,sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));

        /* 将policy策略模板写入内存和驱动中,写入成功后将其与intf绑定 */
        ret = spm_add_flow_classify_drive(join_node NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_free_flow_classify_cb(join_node NBB_CCXT);
            join_node = NULL;
            goto EXIT_LABEL;
        }

       /******************************************************************** 
        * policy策略模板与intf绑定成功后,将存放有相关绑定信息的上话复杂流  *
        * 分类节点(SPM_QOS_FLOW_CLASSIFY_CB)插入到intf的上话复杂流分类的引 *
        * 用树(qos_flow_classify_tree)中,到此整个policy策略的绑定过程结束. *
        ********************************************************************/

        //coverity[no_effect_test]
        rv = AVLL_INSERT(SHARED.qos_flow_classify_tree, join_node->spm_qos_flow_classify_node);    
    }
    else 
    {
        /* policy策略模板索引值为0,表示删除配置 */
        if(0 == index)
        {
            spm_del_flow_classify_drive(join_node NBB_CCXT);
            AVLL_DELETE(SHARED.qos_flow_classify_tree, join_node->spm_qos_flow_classify_node);
            spm_free_flow_classify_cb(join_node NBB_CCXT);
            join_node = NULL;
        }

        /* 配置相同 */
        else if(join_node->qos_cfg.qos_policy_index == index)
        {
        }

        /* 配置不同,更新操作 */
        else
        {
            spm_del_flow_classify_drive(join_node NBB_CCXT);
            OS_MEMCPY(&(join_node->qos_cfg),classify_qos,
                sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
            spm_add_flow_classify_drive(join_node NBB_CCXT);
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#else


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
NBB_LONG spm_add_flow_classify(SPM_QOS_POLICY_CB *pB, SUB_PORT *sub_port NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != sub_port);
    NBB_ASSERT(NULL != pB);

    if ((NULL == sub_port) || (NULL == pB))
    {
        printf("**QOS ERROR**%s,%d,key==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS spm_add_flow_classify",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));        
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 模式错误 */
    if (0 == (pB->basic_cfg_cb->mode))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(0 != sub_port->slot)
    {
#if defined (SPU) || defined (PTN690_CES)
       ret = ApiC3SetIntfIngressAcl(sub_port->unit,sub_port->posid,1,pB->acl_id);
       if (ATG_DCI_RC_OK != ret)
       {
            printf("**ERROR**acl id=%ld, ret =%ld ApiC3SetIntfIngressAcl",pB->acl_id, ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					   "QOS set acl",ret,"AclId","unit","posid","policy index",
    					   pB->acl_id,sub_port->unit,sub_port->posid,pB->policy_key));
            goto EXIT_LABEL;
       } 
#endif
    }
    else
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
           ret = ApiC3SetIntfIngressAcl(unit,sub_port->posid,1,pB->acl_id);
           if (ATG_DCI_RC_OK != ret)
           {
                printf("**ERROR**acl id=%ld, ret =%ld ApiC3SetIntfIngressAcl",pB->acl_id, ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        					   "QOS set acl",ret,"AclId","unit","posid","policy index",
        					   pB->acl_id,unit,sub_port->posid,pB->policy_key));
                goto EXIT_LABEL;
           } 
#endif
        }
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
NBB_LONG spm_del_flow_classify(SPM_QOS_POLICY_CB *pB, SUB_PORT *sub_port NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    NBB_ASSERT(NULL != pB);

    if (NULL == pB)
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS spm_del_flow_classify",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0)); 
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 模式错误 */
    if (0 == pB->basic_cfg_cb->mode)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(0 != sub_port->slot)
    {
#if defined (SPU) || defined (PTN690_CES)
       ret = ApiC3SetIntfIngressAcl(sub_port->unit,sub_port->posid,0,pB->acl_id);
       if (ATG_DCI_RC_OK != ret)
       {
            printf("**ERROR**del flow classify acl id=%ld, ret =%ld ApiC3SetIntfIngressAcl",pB->acl_id, ret);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					   "QOS set del flow classify",ret,
    					   "AclId","unit","posid","policy index",
    					   pB->acl_id,sub_port->unit,sub_port->posid,pB->policy_key));
            goto EXIT_LABEL;
       } 
#endif
    }
    else
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
           ret = ApiC3SetIntfIngressAcl(unit,sub_port->posid,0,pB->acl_id);
           if (ATG_DCI_RC_OK != ret)
           {
                printf("**ERROR**del flow classify acl id=%ld, ret =%ld ApiC3SetIntfIngressAcl",pB->acl_id, ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS set del flow classify",ret,
                			   "AclId","unit","posid","policy index",
                			   pB->acl_id,unit,sub_port->posid,pB->policy_key));
                goto EXIT_LABEL;
           }
#endif
        }
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
NBB_LONG spm_add_logic_flow_classify_node(NBB_ULONG key,
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *classify_qos, SUB_PORT *sub_port NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_FLOW_CLASSIFY_CB *join_node = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_QOS_POLICY_CB *pB = NULL;
    SPM_QOS_POLICY_CB *pB_old = NULL;
    NBB_ULONG index = 0;
    SPM_QOS_LOGIC_INTF_KEY log_key = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((0 == key) || (NULL == classify_qos) || (NULL == sub_port))
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
        printf("%s %s,%d log key=%ld,port=%d,posid=%ld,qos_policy_index=%ld,"
                  "svlan=%d\n\n",QOS_CFG_STRING,__FUNCTION__,__LINE__,
                  key,sub_port->port,sub_port->posid,classify_qos->qos_policy_index,
                  classify_qos->svlan);
        OS_SPRINTF(ucMessage,"%s %s,%d log key=%ld,port=%d,posid=%ld,qos_policy_index=%ld,"
                   "svlan=%d\n\n",QOS_CFG_STRING,__FUNCTION__,__LINE__,
                   key,sub_port->port,sub_port->posid,classify_qos->qos_policy_index,
                   classify_qos->svlan);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }
    index = classify_qos->qos_policy_index;
    log_key.svlan = classify_qos->svlan;
    log_key.cvlan = classify_qos->cvlan;
    log_key.index = key;
    join_node = AVLL_FIND(SHARED.qos_flow_classify_tree, &log_key);

    /***************************************************************************/
    /*                                  增加操作                               */
    /***************************************************************************/
    if ((NULL == join_node) && (0 != index))
    {
        join_node = spm_alloc_qos_flow_classify_cb(&log_key NBB_CCXT);
        if (NULL == join_node)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        pB = spm_qos_find_policy_cb(index NBB_CCXT);
        if ((NULL == pB) || (NULL == pB->basic_cfg_cb))
        {
            spm_free_flow_classify_cb(join_node NBB_CCXT);
            join_node = NULL;
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_flow_classify(pB, sub_port NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            ret = spm_free_flow_classify_cb(join_node NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            join_node = NULL;
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        join_node->policy_index = index;
        join_node->acl_id = pB->acl_id;
        OS_MEMCPY(&(join_node->sub_port),sub_port,sizeof(SUB_PORT));
        
        //coverity[no_effect_test]
        rv = AVLL_INSERT(SHARED.qos_flow_classify_tree, join_node->spm_qos_flow_classify_node);
        rv = AVLL_INSERT(pB->instance_tree, join_node->spm_classify_id_node);
        goto EXIT_LABEL;
    }

    /* 更新 */
    else if ((NULL != join_node) && (0 != join_node->policy_index) && 
            (join_node->policy_index != index) && (0 != index))
    {           
        pB = spm_qos_find_policy_cb(index NBB_CCXT);
        if ((NULL == pB) || (NULL == pB->basic_cfg_cb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_add_flow_classify(pB, sub_port NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }    
        pB_old = spm_qos_find_policy_cb(join_node->policy_index NBB_CCXT);
        if ((NULL == pB) || (NULL == pB->basic_cfg_cb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        join_node->policy_index = index;
        join_node->acl_id = pB->acl_id;
        if(NULL != AVLL_FIND(pB_old->instance_tree, &log_key))
        {
            AVLL_DELETE(pB_old->instance_tree, join_node->spm_classify_id_node);
        } 
        
        //coverity[no_effect_test]
        rv = AVLL_INSERT(pB->instance_tree, join_node->spm_classify_id_node);
        goto EXIT_LABEL;
    }

    /* 相同的配置 */
    else if ((NULL != join_node) && (0 != join_node->policy_index) && 
            (join_node->policy_index == index) && (0 != index))
    {
        printf("**QOS ERROR**%s,%d,the same cfg\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "logic flow classify the same cfg",ATG_DCI_RC_OK,
					   "index","policy_index","key","",
					   index,join_node->policy_index,key,0));
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }

    /* 删除配置 */
    else if ((NULL != join_node) && (0 != join_node->policy_index) && (0 == index))
    {
        pB = spm_qos_find_policy_cb(join_node->policy_index NBB_CCXT);
        if ((NULL == pB) || (NULL == pB->basic_cfg_cb))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        spm_del_flow_classify(pB,sub_port NBB_CCXT);
        AVLL_DELETE(SHARED.qos_flow_classify_tree, join_node->spm_qos_flow_classify_node);
        if(NULL != AVLL_FIND(pB->instance_tree, &log_key))
        {
            AVLL_DELETE(pB->instance_tree, join_node->spm_classify_id_node);
        }
        spm_free_flow_classify_cb(join_node NBB_CCXT);
        join_node = NULL;
        goto EXIT_LABEL;
    }

    /* 错误的配置 */
    else 
    {
        printf("**QOS ERROR**%s,%d,logic flow classify config missmatch\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS logic flow classify config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
						"policy index","svlan","logic index",
						(NULL == join_node)?"no cfg cvlan=":"find cfg cvlan=",
						index,log_key.svlan,key,log_key.cvlan));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif

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
NBB_LONG spm_qos_clear_logic_classify(NBB_LONG logic_key NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_FLOW_CLASSIFY_CB *cfg_cb = NULL;
    SPM_QOS_FLOW_CLASSIFY_CB *next_cfg_cb = NULL;
    SPM_QOS_POLICY_CB *pB = NULL;

    //NBB_TRC_ENTRY(__FUNCTION__);

    cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*) AVLL_FIRST(SHARED.qos_flow_classify_tree);
    while(cfg_cb != NULL)
    {
        next_cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*)AVLL_NEXT(     \
                        SHARED.qos_flow_classify_tree,cfg_cb->spm_qos_flow_classify_node);

        if(cfg_cb->key.index != logic_key)
        {
            cfg_cb = next_cfg_cb;
            continue;
        }

        pB = spm_qos_find_policy_cb(cfg_cb->policy_index NBB_CCXT);
        if(NULL != pB)
        {
#ifdef PTN690
            spm_del_flow_classify_drive(cfg_cb NBB_CCXT);
#else
            if(NULL != AVLL_FIND(pB->instance_tree, &(cfg_cb->key)))
            {
                AVLL_DELETE(pB->instance_tree, cfg_cb->spm_classify_id_node);
            }
#endif
        }
        AVLL_DELETE(SHARED.qos_flow_classify_tree, cfg_cb->spm_qos_flow_classify_node);
        spm_free_flow_classify_cb(cfg_cb NBB_CCXT);  
        cfg_cb = next_cfg_cb;
    }

    //EXIT_LABEL: NBB_TRC_EXIT();
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
NBB_VOID spm_qos_clear_all_logic_classify(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_FLOW_CLASSIFY_CB *cfg_cb = NULL;

#if 0
    SPM_QOS_POLICY_CB *pB = NULL;
#endif

    for(cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*) AVLL_FIRST(v_spm_shared->qos_flow_classify_tree);cfg_cb != NULL;
        cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*) AVLL_FIRST(v_spm_shared->qos_flow_classify_tree))
    {
#if 0
        pB = spm_qos_find_policy_cb(cfg_cb->policy_index NBB_CCXT);
        if (NULL == pB)
        {
            printf("**QOS ERROR**%s line=%d logic_key=%d policy_index=%d can't find policy\n",
                      __FUNCTION__,__LINE__,cfg_cb->key.index,cfg_cb->policy_index);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					   "QOS can't find policy cfg",ATG_DCI_RC_UNSUCCESSFUL,
    					   "logic_key","policy index","posid","acl_id",
    					   cfg_cb->key.index,cfg_cb->policy_index,
    					   cfg_cb->sub_port.posid,cfg_cb->acl_id));
        }
        else
        {
#ifdef  PTN690
            if(AVLL_IN_TREE(cfg_cb->spm_classify_id_node))
            {
                /* 非共享模式 */
                if(0 == pB->basic_cfg_cb->share)
                {
                    AVLL_DELETE(pB->c2b_instance_tree[cfg_cb->sz_offset].instance_tree,
                                cfg_cb->spm_classify_id_node);
                }

                /* 共享模式 */
                else
                {
                    AVLL_DELETE(pB->instance_tree, cfg_cb->spm_classify_id_node);
                }
            }
#else
            if(NULL != AVLL_IN_TREE(cfg_cb->spm_classify_id_node))
            {
                AVLL_DELETE(pB->instance_tree, cfg_cb->spm_classify_id_node);
            }
#endif
            else
            {
                printf("**QOS ERROR**%s line=%d logic_key=%d,policy_index=%d,policy_share=%d,sz_offset=%d:can't "
                       "find node in tree of instance_tree.\n",__FUNCTION__,__LINE__,cfg_cb->key.index,
                       cfg_cb->policy_index,pB->basic_cfg_cb->share,cfg_cb->sz_offset);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        					   "QOS can't find policy instance_tree node",ATG_DCI_RC_UNSUCCESSFUL,
        					   "logic_key","policy index","policy_share","sz_offset",
        					   cfg_cb->key.index,cfg_cb->policy_index,
        					   pB->basic_cfg_cb->share,cfg_cb->sz_offset));
                continue;
            }
        }
#endif      
        AVLL_DELETE(v_spm_shared->qos_flow_classify_tree, cfg_cb->spm_qos_flow_classify_node);
        spm_free_flow_classify_cb(cfg_cb NBB_CCXT);
    }

    return; 
}

#endif


