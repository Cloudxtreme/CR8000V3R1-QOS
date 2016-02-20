/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_action.c
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
#include <spm_qos_action_v1.h>

/* meter-id */
NBB_BYTE g_qos_flow_meter_id[MAX_METER_NUM];

/* color-id */
NBB_BYTE g_qos_color_action_id[MAX_COLOR_ACTION_NUM];

/* behavior树 */
AVLL_TREE g_qos_action_tree;

#ifdef PTN690

/* pw-car树 */
AVLL_TREE g_qos_pw_car_tree;
#endif

/* behavior模块全局打印开关 */
NBB_BYTE g_qos_behavior_print = ATG_DCI_RC_OK;

/* behavior模块全局LOG开关 */
NBB_BYTE g_qos_behavior_log   = ATG_DCI_RC_UNSUCCESSFUL;

#if 1

/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : action引用计数减1
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_qos_apply_meter_id(NBB_ULONG *pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_ULONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    for (id = MAX_PHYSIC_PORT_NUM * 3; id < MAX_METER_NUM; id++)
    {
        /* 有空闲ID */
        if (0 == g_qos_flow_meter_id[id])
        {
            *pid = id + 1;
            g_qos_flow_meter_id[id] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;
            goto EXIT_LABEL;
        }
    }
    ret = ATG_DCI_RC_UNSUCCESSFUL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : action引用计数减1
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_qos_free_meter_id(NBB_ULONG *pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_UNSUCCESSFUL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (*pid < MAX_PHYSIC_PORT_NUM * 3 + 1)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (0 != g_qos_flow_meter_id[*pid - 1])
    {
        g_qos_flow_meter_id[*pid - 1] = 0;
        *pid = 0;
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    else
    {
        ret = ATG_DCI_RC_DEL_FAILED;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
SPM_QOS_ACTION_CB *spm_alloc_action_cb(NBB_ULONG ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_ACTION_CB *pstcb = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    pstcb = (SPM_QOS_ACTION_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_ACTION_CB), 
                                     NBB_NORETRY_ACT, MEM_SPM_ACTION_CB);
    if (pstcb == NULL)
    {
        goto EXIT_LABEL;
    }
    OS_MEMSET(pstcb, 0, sizeof(SPM_QOS_ACTION_CB));
    pstcb->action_key = ulkey;
    AVLL_INIT_NODE(pstcb->spm_action_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return pstcb;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_action_cb(SPM_QOS_ACTION_CB *pstcb)
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
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_ACTION_CB), MEM_SPM_ACTION_CB);

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (NULL != pstcb->basic_cfg_cb)
    {
        NBB_MM_FREE(pstcb->basic_cfg_cb, MEM_SPM_ACTION_BASIC_CB);
        pstcb->basic_cfg_cb = NULL;
    }

    if (NULL != pstcb->supervise_cfg_cb)
    {           
        NBB_MM_FREE(pstcb->supervise_cfg_cb, MEM_SPM_ACTION_SUPERVISE_CB);
        pstcb->supervise_cfg_cb = NULL;
    }
    if (NULL != pstcb->shaping_cfg_cb)
    {
        NBB_MM_FREE(pstcb->shaping_cfg_cb, MEM_SPM_ACTION_SHAPING_CB);
        pstcb->shaping_cfg_cb = NULL;
    }
    if (NULL != pstcb->schedule_cfg_cb)
    {
        NBB_MM_FREE(pstcb->schedule_cfg_cb, MEM_SPM_ACTION_SCHEDULE_CB);
        pstcb->schedule_cfg_cb = NULL;
    }
    if (NULL != pstcb->flow_act_cfg_cb)
    {
        NBB_MM_FREE(pstcb->flow_act_cfg_cb, MEM_SPM_ACTION_FLOW_ACT_CB);
        pstcb->flow_act_cfg_cb = NULL;
    }
    if (NULL != pstcb->flow_samping_cfg_cb)
    {
        NBB_MM_FREE(pstcb->flow_samping_cfg_cb, MEM_SPM_ACTION_FLOW_SAMPL_CB);
        pstcb->flow_samping_cfg_cb = NULL;
    }
    if (NULL != pstcb->redirect_cfg_cb)
    {
        NBB_MM_FREE(pstcb->redirect_cfg_cb, MEM_SPM_ACTION_REDIRECT_CB);
        pstcb->redirect_cfg_cb = NULL;
    }
    if (NULL != pstcb->policy_nhi_cfg_cb)
    {
        NBB_MM_FREE(pstcb->policy_nhi_cfg_cb, MEM_SPM_ACTION_POLCY_RT_CB);
        pstcb->policy_nhi_cfg_cb = NULL;
    }
    if (NULL != pstcb->phb_cfg_cb)
    {
        NBB_MM_FREE(pstcb->phb_cfg_cb, MEM_SPM_ACTION_PHB_CB);
        pstcb->phb_cfg_cb = NULL;
    }
    if (NULL != pstcb->domain_cfg_cb)
    {
        NBB_MM_FREE(pstcb->domain_cfg_cb, MEM_SPM_ACTION_DOMAIN_CB);
        pstcb->domain_cfg_cb = NULL;
    }
    if (NULL != pstcb->urpf_cfg_cb)
    {
        NBB_MM_FREE(pstcb->urpf_cfg_cb, MEM_SPM_ACTION_URPF_CB);
        pstcb->urpf_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_ACTION_CB);
    pstcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 2

/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : BEHAVIOR中流量监管用来给复杂流分类限速;
               流量整型用来给hqos中tunnel限速
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_meter(ATG_DCI_QOS_BEHAVIOR_SUPERVISE *pstcb,
    NBB_ULONG *meter_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    METER_CFG_T meter = {0};
    NBB_BYTE meterflag = 0;
    NBB_USHORT unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (PTN690_CES)
    NBB_LONG rv = ATG_DCI_RC_OK;
#endif

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 第一次申请meter,若已经申请了meter_id,此次仅更新流量监管的配置则不需要重复申请 */
    if(0 == *meter_id)
    {
        meterflag = 1;
        ret = spm_qos_apply_meter_id(meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_apply_meter_error_log(meter_id, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
    }
    meter.cir = pstcb->cir;
    meter.pireir = pstcb->pir;
    meter.cbs = pstcb->cbs * 1024;
    meter.pbsebs = pstcb->pbs * 1024;
    meter.meterId = *meter_id;
    meter.dropRed = 1;

#if 0
    RFC_2697,          /* RFC_2697 */
    RFC_2698,          /* RFC_2698 */
    RFC_4115,          /* RFC_4115 */
    MEF                /* MEF      */
#endif

    if(3 == pstcb->mode)
    {
         meter.eMeterType = RFC_2697;
    }
    else if(2 == pstcb->mode)
    {
        meter.eMeterType = RFC_2698;
    }
    else
    {
        meter.eMeterType = MEF;
    }
    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetMeter(unit,&meter);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_meter_error_log(unit, &meter, __FUNCTION__, __LINE__, ret);
            if(1 == meterflag)
            {
                rv = spm_qos_free_meter_id(meter_id);
                if(ATG_DCI_RC_OK != rv)
                {
                    spm_qos_free_meter_error_log(meter_id, __FUNCTION__, __LINE__, rv);
                }
            }
            goto EXIT_LABEL;
        }
#endif
    }
            
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;           
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_shaping_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_SHAPPING *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->shaping_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->shaping_cfg_cb, MEM_SPM_ACTION_SHAPING_CB);
            pcb->shaping_cfg_cb = NULL;
        }

        /* 更新或新增子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_SHAPPING *)buf;
            if ((ptemp->cir) > (ptemp->pir))
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:pir=%lu must be greater than cir=%lu.\n", __FUNCTION__, 
                       __LINE__, ptemp->pir, ptemp->cir);
                OS_SPRINTF(ucmessage,"***err***%s,%d:pir=%lu must be greater than cir=%lu.\n", 
                           __FUNCTION__, __LINE__, ptemp->pir, ptemp->cir);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp->pir -= ptemp->cir;
            if (NULL == pcb->shaping_cfg_cb)
            {
                pcb->shaping_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_SHAPPING *)NBB_MM_ALLOC(   \
                                                  sizeof(ATG_DCI_QOS_BEHAVIOR_SHAPPING),
                                            NBB_NORETRY_ACT, MEM_SPM_ACTION_SHAPING_CB);
                if (NULL == pcb->shaping_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                } 
            }
            OS_MEMCPY(pcb->shaping_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_SHAPPING));
            ptemp->pir += ptemp->cir;
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_supervise_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT unit = 0;
    NBB_LONG   ret  = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_SUPERVISE *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->supervise_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            spm_del_action_driver(pcb);
            NBB_MM_FREE(pcb->supervise_cfg_cb, MEM_SPM_ACTION_SUPERVISE_CB);
            pcb->supervise_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_SUPERVISE *)buf;
            if (NULL == pcb->supervise_cfg_cb)
            {
                pcb->supervise_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_SUPERVISE *)NBB_MM_ALLOC( \
                                                  sizeof(ATG_DCI_QOS_BEHAVIOR_SUPERVISE),
                                           NBB_NORETRY_ACT, MEM_SPM_ACTION_SUPERVISE_CB);
                if (NULL == pcb->supervise_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }

            /* 更新或新增meterid */
            ret = spm_set_action_meter(ptemp, &(pcb->meter_id));
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            OS_MEMCPY(pcb->supervise_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_SUPERVISE));
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_schedule_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_SCHEDULE *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->schedule_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->schedule_cfg_cb, MEM_SPM_ACTION_SCHEDULE_CB);
            pcb->schedule_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_SCHEDULE *)buf;

            /* 只支持WFQ模式 */
            if ((0 == ptemp->mode) || (0 == ptemp->weight))
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:schedule_cfg only support wfq-mode.\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:schedule_cfg only support wfq-mode.\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            if (NULL == pcb->schedule_cfg_cb)
            {
                pcb->schedule_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_SCHEDULE *)NBB_MM_ALLOC(   \
                                                    sizeof(ATG_DCI_QOS_BEHAVIOR_SCHEDULE),
                                             NBB_NORETRY_ACT, MEM_SPM_ACTION_SCHEDULE_CB);
                if (NULL == pcb->schedule_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->schedule_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_SCHEDULE));
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_flow_act_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->flow_act_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->flow_act_cfg_cb, MEM_SPM_ACTION_FLOW_ACT_CB);
            pcb->flow_act_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *)buf;
            if (NULL == pcb->flow_act_cfg_cb)
            {
                pcb->flow_act_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *)NBB_MM_ALLOC(   \
                                                   sizeof(ATG_DCI_QOS_BEHAVIOR_FLOW_ACT),
                                            NBB_NORETRY_ACT, MEM_SPM_ACTION_FLOW_ACT_CB);
                if (NULL == pcb->flow_act_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->flow_act_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_FLOW_ACT));
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_flow_samp_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->flow_samping_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->flow_samping_cfg_cb, MEM_SPM_ACTION_FLOW_SAMPL_CB);
            pcb->flow_samping_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *)buf;
            if (NULL == pcb->flow_samping_cfg_cb)
            {
                pcb->flow_samping_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *)NBB_MM_ALLOC( \
                                                     sizeof(ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL),
                                              NBB_NORETRY_ACT, MEM_SPM_ACTION_FLOW_SAMPL_CB);
                if (NULL == pcb->flow_samping_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->flow_samping_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL));
        } 
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_redirect_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_REDIRECT *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->redirect_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->redirect_cfg_cb, MEM_SPM_ACTION_REDIRECT_CB);
            pcb->redirect_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_REDIRECT *)buf;
            if (NULL == pcb->redirect_cfg_cb)
            {
                pcb->redirect_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_REDIRECT *)NBB_MM_ALLOC(   \
                                                   sizeof(ATG_DCI_QOS_BEHAVIOR_REDIRECT),
                                            NBB_NORETRY_ACT, MEM_SPM_ACTION_REDIRECT_CB);
                if(NULL == pcb->redirect_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->redirect_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_REDIRECT));
        } 
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_policy_nhi_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_POLICY_RT *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->policy_nhi_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->policy_nhi_cfg_cb, MEM_SPM_ACTION_POLCY_RT_CB);
            pcb->policy_nhi_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_POLICY_RT *)buf;
            if (NULL == pcb->policy_nhi_cfg_cb)
            {
                pcb->policy_nhi_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_POLICY_RT *)NBB_MM_ALLOC(    \
                                                      sizeof(ATG_DCI_QOS_BEHAVIOR_POLICY_RT),
                                                NBB_NORETRY_ACT, MEM_SPM_ACTION_POLCY_RT_CB);
                if(NULL == pcb->policy_nhi_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->policy_nhi_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_POLICY_RT));
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_phb_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_PHB *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->phb_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->phb_cfg_cb, MEM_SPM_ACTION_PHB_CB);
            pcb->phb_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_PHB *)buf;
            if (NULL == pcb->phb_cfg_cb)
            {
                pcb->phb_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_PHB *)NBB_MM_ALLOC( \
                                            sizeof(ATG_DCI_QOS_BEHAVIOR_PHB),
                                     NBB_NORETRY_ACT, MEM_SPM_ACTION_PHB_CB);
                if(NULL == pcb->phb_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->phb_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_PHB));
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_domain_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_DOMAIN *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->domain_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->domain_cfg_cb, MEM_SPM_ACTION_DOMAIN_CB);
            pcb->domain_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            ptemp = (ATG_DCI_QOS_BEHAVIOR_DOMAIN *)buf;
            if (NULL == pcb->domain_cfg_cb)
            {
                pcb->domain_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_DOMAIN *)NBB_MM_ALLOC(   \
                                                 sizeof(ATG_DCI_QOS_BEHAVIOR_DOMAIN),
                                          NBB_NORETRY_ACT, MEM_SPM_ACTION_DOMAIN_CB);
                if(NULL == pcb->domain_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->domain_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_DOMAIN));
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_urpf_cfg(NBB_ULONG oper, SPM_QOS_ACTION_CB *pcb, 
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_URPF *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if((ATG_DCI_OPER_ADD != oper) && (ATG_DCI_OPER_DEL != oper))
    {
        goto EXIT_LABEL;
    }
    else
    {
        if(NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        /* 删除子tlv */
        if(ATG_DCI_OPER_DEL == oper)
        {
            if(NULL == pcb->urpf_cfg_cb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL; 
            }
            NBB_MM_FREE(pcb->urpf_cfg_cb, MEM_SPM_ACTION_URPF_CB);
            pcb->urpf_cfg_cb = NULL;
        }

        /* 新增或更新子tlv */
        else
        {
            if(NULL == buf)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ptemp = (ATG_DCI_QOS_BEHAVIOR_URPF *)buf;
            if (NULL == pcb->urpf_cfg_cb)
            {
                pcb->urpf_cfg_cb = (ATG_DCI_QOS_BEHAVIOR_URPF *)NBB_MM_ALLOC(   \
                                               sizeof(ATG_DCI_QOS_BEHAVIOR_URPF),
                                        NBB_NORETRY_ACT, MEM_SPM_ACTION_URPF_CB);
                if(NULL == pcb->urpf_cfg_cb)
                {
                    ret = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
            }
            OS_MEMCPY(pcb->urpf_cfg_cb, ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_URPF));
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_que_shape_cfg(NBB_ULONG oper,
    ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE pcb[ATG_DCI_QOS_BEHAVIOR_SHAPE_NUM],
    NBB_USHORT num, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT i = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(ATG_DCI_OPER_UPDATE != oper)
    {
        goto EXIT_LABEL;
    }
    else
    {
        if((NULL == buf) || (ATG_DCI_QOS_BEHAVIOR_SHAPE_NUM < num))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        for (i = 0; i < num; i++)
        {
            ptemp = (ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE *)(buf +    \
                     NBB_ALIGN_OFFSET(sizeof(ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE)) * i);
            if(NULL == ptemp)
            {
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
            else
            {
                ptemp->pir -= ptemp->cir;
                OS_MEMCPY(&pcb[i], ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE));
                ptemp->pir += ptemp->cir;
            }
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_que_schedul_cfg(NBB_ULONG oper,
    ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL pcb[ATG_DCI_QOS_BEHAVIOR_SCHEDL_NUM], 
    NBB_USHORT num, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT i = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(ATG_DCI_OPER_UPDATE != oper)
    {
        goto EXIT_LABEL;
    }
    else
    {
        if((NULL == buf) || (ATG_DCI_QOS_BEHAVIOR_SCHEDL_NUM < num))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }

        for (i = 0; i < num; i++)
        {
            ptemp = (ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL *)(buf +   \
                     NBB_ALIGN_OFFSET(sizeof(ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL)) * i); 
            if(NULL == ptemp)
            {
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
            else
            {
                OS_MEMCPY(&pcb[i], ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL));
            }
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_action_que_congst_cfg(NBB_ULONG oper, 
    ATG_DCI_QOS_BEHAVIOR_QUE_CONGST pcb[ATG_DCI_QOS_BEHAVIOR_CONGST_NUM], 
    NBB_USHORT num, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT i = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    ATG_DCI_QOS_BEHAVIOR_QUE_CONGST *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(ATG_DCI_OPER_UPDATE != oper)
    {
        goto EXIT_LABEL;
    }
    else
    {
        if((NULL == buf) || (ATG_DCI_QOS_BEHAVIOR_CONGST_NUM < num))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;
        }
        for (i = 0; i < num; i++)
        {
            ptemp = (ATG_DCI_QOS_BEHAVIOR_QUE_CONGST *)(buf +   \
                     NBB_ALIGN_OFFSET(sizeof(ATG_DCI_QOS_BEHAVIOR_QUE_CONGST)) * i);
            if(NULL == ptemp)
            {
               ret = ATG_DCI_RC_UNSUCCESSFUL;
               goto EXIT_LABEL;
            }
            else
            {
               OS_MEMCPY(&pcb[i], ptemp, sizeof(ATG_DCI_QOS_BEHAVIOR_QUE_CONGST)); 
            }
        }
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_del_action_driver(SPM_QOS_ACTION_CB *pstcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT unit = 0;
    NBB_LONG   ret  = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pstcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(0 != pstcb->meter_id)
    {
        for(unit = 0; unit < SHARED.c3_num; unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelMeter(unit,pstcb->meter_id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_meter_error_log(unit, pstcb->meter_id, 
                                          __FUNCTION__, __LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }

        ret = spm_qos_free_meter_id(&(pstcb->meter_id));
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_free_meter_error_log(&(pstcb->meter_id), 
                                __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
    }


    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_action(ATG_DCI_SET_QOS_BEHAVIOR *pstsetips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ulkey = 0;
    SPM_QOS_ACTION_CB *pstcb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;
    NBB_USHORT shape_num = 0;
    NBB_USHORT schedule_num = 0;
    NBB_USHORT congest_num = 0;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucshapingdatastart    = NULL;
    NBB_BYTE *pucsupervisedatastart  = NULL;
    NBB_BYTE *pucscheduledatastart   = NULL;
    NBB_BYTE *pucactdatastart        = NULL;
    NBB_BYTE *pucsamplingdatastart   = NULL;
    NBB_BYTE *pucredirectdatastart   = NULL;
    NBB_BYTE *pucnhidatastart        = NULL;
    NBB_BYTE *pucphbdatastart        = NULL;
    NBB_BYTE *pucdomaindatastart     = NULL;
    NBB_BYTE *pucurpfdatastart       = NULL;
    NBB_BYTE *pucqueshapingdatastart = NULL;
    NBB_BYTE *pucquescheduldatastart = NULL;
    NBB_BYTE *pucquecongstdatastart  = NULL;

    NBB_ULONG ulopershaping    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulopersupervise  = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperschedule   = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperact        = ATG_DCI_OPER_NULL;
    NBB_ULONG ulopersampling   = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperredirect   = ATG_DCI_OPER_NULL;
    NBB_ULONG ulopernhi        = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperphb        = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperdomain     = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperurpf       = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperqueshaping = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperqueschedul = ATG_DCI_OPER_NULL;
    NBB_ULONG uloperquecongst  = ATG_DCI_OPER_NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstsetips)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    shape_num    = pstsetips->queue_shape_num;
    schedule_num = pstsetips->queue_schedule_num;
    congest_num  = pstsetips->queue_congestion_num;

    /* 获取子配置操作 */
    ulopershaping    = pstsetips->oper_shapping;
    ulopersupervise  = pstsetips->oper_supervise;
    uloperschedule   = pstsetips->oper_schedule;
    uloperact        = pstsetips->oper_flow_action;
    ulopersampling   = pstsetips->oper_flow_sampling;
    uloperredirect   = pstsetips->oper_redirect;
    ulopernhi        = pstsetips->oper_policy_route;
    uloperphb        = pstsetips->oper_phb;
    uloperdomain     = pstsetips->oper_domain;
    uloperurpf       = pstsetips->oper_urpf;
    uloperqueshaping = pstsetips->oper_queue_shape;
    uloperqueschedul = pstsetips->oper_queue_schedule;
    uloperquecongst  = pstsetips->oper_queue_congestion;

    /* 获取子配置的偏移地址 */
    pucshapingdatastart    = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->shapping_data);
    pucsupervisedatastart  = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->supervise_data);
    pucscheduledatastart   = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->schedule_data);
    pucactdatastart        = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->flow_action_data);
    pucsamplingdatastart   = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->flow_sampling_data);
    pucredirectdatastart   = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->redirect_data);
    pucnhidatastart        = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->policy_route_data);
    pucphbdatastart        = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->phb_data);
    pucdomaindatastart     = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->domain_data);
    pucurpfdatastart       = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->urpf_data);
    pucqueshapingdatastart = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->queue_shape_data);
    pucquescheduldatastart = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->queue_schedule_data);
    pucquecongstdatastart  = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pstsetips, &pstsetips->queue_congestion_data);

    pstsetips->return_code = ATG_DCI_RC_OK;
    ulkey = pstsetips->key;
    pstcb = AVLL_FIND(g_qos_action_tree, &(pstsetips->key));
    if(TRUE == pstsetips->delete_struct)
    {
        /* 错误删除 */
        if(NULL == pstcb)
        {
            pstsetips->return_code = ATG_DCI_RC_DEL_FAILED;
            printf("***err***%s,%d:no bid=%lu cfg, delete err.\n\n", __FUNCTION__, 
                   __LINE__, pstsetips->key);
            OS_SPRINTF(ucmessage,"***err***%s,%d:no bid=%lu cfg, delete err.\n\n", 
                       __FUNCTION__, __LINE__, pstsetips->key);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
            goto EXIT_LABEL;  
        }
        else
        {
           /****************************************************************
            * 删除流量监管时的meter,删除驱动信息不判断返回值;BEHAVIOR中    *
            * 流量监管用来给复杂流分类限速；流量整型用来给hqos中tunnel限速 * 
            ****************************************************************/
            spm_del_action_driver(pstcb);
            AVLL_DELETE(g_qos_action_tree, pstcb->spm_action_node);
            ret = spm_free_action_cb(pstcb);
            if (ATG_DCI_RC_OK != ret)
            {
                pstsetips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }
    }
    else
    {
        if(NULL == pstcb)
        {
            pstcb = spm_alloc_action_cb(pstsetips->key);
            if (NULL == pstcb)
            {
                pstsetips->return_code = ATG_DCI_RC_ALLOC_ERROR;
                printf("***err***%s,%d:spm_alloc_action_cb failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_action_cb failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            else
            {
                rv = AVLL_INSERT(g_qos_action_tree, pstcb->spm_action_node);
            }
        }

        /***************************************************************************/
        /*                             流量整形                                    */
        /***************************************************************************/
        ret = spm_set_action_shaping_cfg(ulopershaping, pstcb, pucshapingdatastart);
        pstsetips->shapping_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                             流量监管                                    */
        /***************************************************************************/
        ret = spm_set_action_supervise_cfg(ulopersupervise, pstcb, pucsupervisedatastart);
        pstsetips->supervise_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                             调度策略                                    */
        /***************************************************************************/
        ret = spm_set_action_schedule_cfg(uloperschedule, pstcb, pucscheduledatastart);
        pstsetips->schedule_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                               流动作                                    */
        /***************************************************************************/
        ret = spm_set_action_flow_act_cfg(uloperact, pstcb, pucactdatastart);
        pstsetips->flow_action_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                              流采样                                     */
        /***************************************************************************/
        ret = spm_set_action_flow_samp_cfg(ulopersampling, pstcb, pucsamplingdatastart);
        pstsetips->flow_sampling_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                             重定向                                      */
        /***************************************************************************/
        ret = spm_set_action_redirect_cfg(uloperredirect, pstcb, pucredirectdatastart);
        pstsetips->redirect_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                             策略路由                                    */
        /***************************************************************************/
        ret = spm_set_action_policy_nhi_cfg(ulopernhi, pstcb, pucnhidatastart);
        pstsetips->policy_route_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                                  PHB                                    */
        /***************************************************************************/
        ret = spm_set_action_phb_cfg(uloperphb, pstcb, pucphbdatastart);
        pstsetips->phb_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                                      域                                 */
        /***************************************************************************/
        ret = spm_set_action_domain_cfg(uloperdomain, pstcb, pucdomaindatastart);
        pstsetips->domain_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                                  urpf                                   */
        /***************************************************************************/
        ret = spm_set_action_urpf_cfg(uloperurpf, pstcb, pucurpfdatastart);
        pstsetips->urpf_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                                  队列整形                               */
        /***************************************************************************/
        ret = spm_set_action_que_shape_cfg(uloperqueshaping, pstcb->que_shaping_cfg_cb, 
                                                    shape_num, pucqueshapingdatastart);
        pstsetips->queue_shape_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }
        else if(ATG_DCI_OPER_UPDATE == uloperqueshaping)
        {
            pstcb->que_shape_num = shape_num;
        }

        /***************************************************************************/
        /*                                  队列调度                               */
        /***************************************************************************/
        ret = spm_set_action_que_schedul_cfg(uloperqueschedul, pstcb->que_schedul_cfg_cb, 
                                                   schedule_num, pucquescheduldatastart);
        pstsetips->queue_schedule_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }
        else if(ATG_DCI_OPER_UPDATE == uloperqueschedul)
        {
            pstcb->que_schedul_num = schedule_num;
        }

        /***************************************************************************/
        /*                                  队列拥塞                               */
        /***************************************************************************/
        ret = spm_set_action_que_congst_cfg(uloperquecongst, pstcb->que_congst_cfg_cb, 
                                                  congest_num, pucquecongstdatastart);
        pstsetips->queue_congestion_return_code = ret;
        if (ATG_DCI_RC_OK != ret)
        {
            ips_ret = ret;
        }
        else if(ATG_DCI_OPER_UPDATE == uloperquecongst)
        {
           pstcb->que_congest_num = congest_num; 
        }

        pstsetips->return_code = ips_ret;
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_qos_clear_all_action(void)
{
    SPM_QOS_ACTION_CB *cfg_cb = NULL;

    for (cfg_cb = (SPM_QOS_ACTION_CB*)AVLL_FIRST(g_qos_action_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_ACTION_CB*)AVLL_FIRST(g_qos_action_tree))
    {
        spm_del_action_driver(cfg_cb);
        AVLL_DELETE(g_qos_action_tree, cfg_cb->spm_action_node);
        spm_free_action_cb(cfg_cb);
        cfg_cb = NULL;
    }

    return;   
}


#ifdef PTN690

/*****************************************************************************
   函 数 名  : spm_alloc_qos_pw_car_cb
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
SPM_QOS_PW_CAR_CB *spm_alloc_qos_pw_car_cb(NBB_ULONG log_key)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_PW_CAR_CB *pstcb = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if (0 == log_key)
    {
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 分配一个新的逻辑配置块qos表配置条目 */
    pstcb = (SPM_QOS_PW_CAR_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_PW_CAR_CB), 
                                 NBB_NORETRY_ACT, MEM_SPM_QOS_PW_CAR_CB);
    if (NULL == pstcb)
    {
        goto EXIT_LABEL;
    }

    /* 初始逻辑配置块配置条目 */
    OS_MEMSET(pstcb, 0, sizeof(SPM_QOS_PW_CAR_CB));
    pstcb->log_key = log_key;
    AVLL_INIT_NODE(pstcb->spm_qos_pw_car_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return pstcb;
}


/*****************************************************************************
   函 数 名  : spm_free_qos_pw_car_cb
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_qos_pw_car_cb(SPM_QOS_PW_CAR_CB *pstcb)
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
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstcb, sizeof(SPM_QOS_PW_CAR_CB), MEM_SPM_QOS_PW_CAR_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstcb, MEM_SPM_QOS_PW_CAR_CB);
    pstcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : action引用计数减1
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_qos_free_color_action_id(NBB_ULONG *pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_UNSUCCESSFUL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((*pid > MAX_COLOR_ACTION_NUM) || (*pid < 1))
    {
        goto EXIT_LABEL;
    }
    
    if (0 != g_qos_color_action_id[*pid - 1])
    {
        g_qos_color_action_id[*pid - 1] = 0;
        *pid = 0;
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
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
NBB_LONG spm_qos_del_color_action(NBB_ULONG *id)
{
    NBB_USHORT unit = 0;
    NBB_LONG   ret  = ATG_DCI_RC_OK;

    if((NULL != id) && (0 != *id))
    {
        for(unit = 0; unit < v_spm_shared->c3_num; unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_qos_clear_policer_action_template(unit, *id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_clear_policer_color_action_template_error_log(unit, 
                                     *id, __FUNCTION__, __LINE__, ret);
            }
#endif
        }
        spm_qos_free_color_action_id(id);
    }

    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_qos_set_pw_color_action(ATG_DCI_LOG_PORT_CAR_DATA *pw_car,
    NBB_ULONG *color_id)
{
    NBB_USHORT unit = 0;
    NBB_LONG colorflag = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (PTN690_CES)
    NBB_LONG rv  = ATG_DCI_RC_OK;
#endif
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    POLICER_ACTION_TEMPLATE_T color_action = {0};
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if((NULL == color_id) || (NULL == pw_car))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }  

    /* 申请色感知动作模板的模板id,目前最多只能申请4000个色感知动作模板 */
    if(0 == *color_id)
    {
        /* 1表示color_id是由本次申请,当写驱动失败后需要释放该id资源 */
        colorflag = 1;
        ret = spm_qos_apply_color_action_id(color_id);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_apply_meter_error_log(color_id, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
    }

    NBB_MEMSET(&color_action, 0, sizeof(POLICER_ACTION_TEMPLATE_T));

    color_action.policer_action_id = *color_id;

    /* action : 0 / 1 / 2 = pass / drop / markphb */
    if(2 == pw_car->green_action.action)
    {
        color_action.chg_gre_pri_ena = 1;
        color_action.gre_pri = pw_car->green_action.markphb;
    }
    else if(1 == pw_car->green_action.action)
    {
        color_action.chg_gre_pri_ena = 0;
        color_action.gre_drop_ena    = 1;
    }
    else if(0 == pw_car->green_action.action)
    {
        color_action.chg_gre_pri_ena = 0;
        color_action.gre_drop_ena    = 0;
    }

    /* action : 0 / 1 / 2 = pass / drop / markphb */
    if(2 == pw_car->yellow_action.action)
    {
        color_action.chg_yel_pri_ena = 1;
        color_action.yel_pri = pw_car->yellow_action.markphb;
    }
    else if(1 == pw_car->yellow_action.action)
    {
        color_action.chg_yel_pri_ena = 0;
        color_action.yel_drop_ena    = 1;
    }
    else if(0 == pw_car->yellow_action.action)
    {
        color_action.chg_yel_pri_ena = 0;
        color_action.yel_drop_ena    = 0;
    }

    /* action : 0 / 1 / 2 = pass / drop / markphb */
    if(2 == pw_car->red_action.action)
    {
        color_action.chg_red_pri_ena = 1;
        color_action.red_pri = pw_car->red_action.markphb;
    }
    else if(1 == pw_car->red_action.action)
    {
        color_action.chg_red_pri_ena = 0;
        color_action.red_drop_ena    = 1;
    }
    else if(0 == pw_car->red_action.action)
    {
        color_action.chg_red_pri_ena = 0;
        color_action.red_drop_ena    = 0;
    }

    /* 将色感知动作模板写入C3驱动中 */
    for(unit = 0; unit < v_spm_shared->c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = fhdrv_qos_set_policer_action_template(unit, &color_action);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_set_policer_action_template_error_log(unit, &color_action,
                                             __FUNCTION__, __LINE__, ret);

            /* 表示color_id是此时申请的,写驱动失败时需要释放该id资源. */
            if(1 == colorflag)
            {
                rv = spm_qos_free_color_action_id(color_id);
                if(ATG_DCI_RC_OK != rv)
                {
                    spm_qos_free_meter_error_log(color_id, __FUNCTION__, __LINE__, rv);
                }
            }
            goto EXIT_LABEL;
        }
#endif  
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_set_qos_pw_meter(ATG_DCI_LOG_PORT_CAR_DATA *pstcb,
       NBB_ULONG *meter_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    METER_CFG_T meter  = {0};
    NBB_USHORT unit = 0;
    NBB_USHORT meterflag = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (PTN690_CES)
    NBB_LONG rv = ATG_DCI_RC_OK;
#endif
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == meter_id) || (NULL == pstcb) || (0 == pstcb->mode))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;  
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    if (0 == *meter_id)
    {
        /* meterflag赋值为1表示meterid是此时申请的,在meter写入驱动失败时需要释放该id资源. */
        meterflag = 1;
        ret = spm_qos_apply_meter_id(meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_apply_meter_error_log(meter_id, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
    }

    meter.meterId = *meter_id;
    meter.cir     = pstcb->cir;
    meter.pireir  = pstcb->pir;
    meter.cbs     = pstcb->cbs * 1024;
    meter.pbsebs  = pstcb->pbs * 1024;
    meter.dropRed = 1;

    if(3 == pstcb->mode)
    {
         meter.eMeterType = RFC_2697;
    }
    else if(2 == pstcb->mode)
    {
        meter.eMeterType = RFC_2698;
    }
    else
    {
        meter.eMeterType = MEF;
    }

    /* 色盲模式 */
    if(0 == pstcb->cm_color)
    {
         meter.colorBlind = 0;
    }
    else
    {
         meter.colorBlind = 1;
    }

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetMeter(unit,&meter);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_meter_error_log(unit, &meter, __FUNCTION__, __LINE__, ret);

            /* 表示meterid是此时申请的,meter写入驱动失败,此时需要释放该id资源. */
            if(1 == meterflag)
            {
                rv = spm_qos_free_meter_id(meter_id);
                if(ATG_DCI_RC_OK != rv)
                {
                    spm_qos_free_meter_error_log(meter_id, __FUNCTION__, __LINE__, rv);
                }
            }
            goto EXIT_LABEL;
        }
#endif
    }
            
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;           
}


/*****************************************************************************
   函 数 名  : spm_add_pw_car_driver
   功能描述  : 当flag不为0时,则删除txlsp的car功能
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_add_pw_car_driver(NBB_ULONG log_id,NBB_ULONG meter_id,
       NBB_ULONG color_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_LOGICAL_PORT_CB *pstlogicalport = NULL;
    SPM_TERMINAL_IF_CB *pstterminalifcb = NULL;
  
    NBB_TRC_ENTRY(__FUNCTION__);

    if ((0 == log_id) || (0 == meter_id) || (0 == color_id))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstlogicalport  = AVLL_FIND(SHARED.logical_port_tree, &log_id);
    if((NULL == pstlogicalport) || (NULL == pstlogicalport->phy_cfg_cb) || \
       (NULL == pstlogicalport->basic_cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 该逻辑端口下所有的intf都需要绑定meter */
    for(pstterminalifcb = (SPM_TERMINAL_IF_CB*)AVLL_FIRST(pstlogicalport->terminal_if_tree); 
        pstterminalifcb != NULL; pstterminalifcb = (SPM_TERMINAL_IF_CB*)AVLL_NEXT(\
        pstlogicalport->terminal_if_tree, pstterminalifcb->spm_terminal_if_node))
    {
        /* intf上绑定meterid */
        for(unit = 0; unit < SHARED.c3_num; unit++)
        {
#if defined (SPU) || defined (PTN690_CES)          
            ret = ApiC3SetIntfMeter(unit, pstterminalifcb->intf_pos_id, 1, meter_id);
            if (ATG_DCI_RC_OK != ret)
            {
                printf("***err***%s,%d:ApiC3SetIntfMeter failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:ApiC3SetIntfMeter failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
#endif
        } 

        /* intf上绑定color action id */
        for(unit = 0; unit < SHARED.c3_num; unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_intf_set_policer_action(unit, pstterminalifcb->intf_pos_id, 1, color_id);
            if (ATG_DCI_RC_OK != ret)
            {
                printf("***err***%s,%d:ApiC3SetIntfMeter failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:ApiC3SetIntfMeter failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
#endif
        } 
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_action_cnt
   功能描述  : 释放内存和申请的bukect桶资源
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_qos_del_txlsp_pw_meter(NBB_ULONG *meter_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == meter_id) || (0 == *meter_id))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;  
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    for(unit = 0; unit < SHARED.c3_num; unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3DelMeter(unit,*meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_del_meter_error_log(unit, *meter_id, 
                              __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    ret = spm_qos_free_meter_id(meter_id);
    if(ATG_DCI_RC_OK != ret)
    {
        spm_qos_free_meter_error_log(meter_id, 
                 __FUNCTION__, __LINE__, ret);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_del_pw_car_driver
   功能描述  : 当flag不为0时,则删除txlsp的car功能
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_del_pw_car_driver(NBB_ULONG log_id, NBB_ULONG meter_id,
       NBB_ULONG color_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_USHORT unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];
    SPM_LOGICAL_PORT_CB *pstlogicalport = NULL;
    SPM_TERMINAL_IF_CB *pstterminalifcb = NULL;
  
    NBB_TRC_ENTRY(__FUNCTION__);

    if ((0 == log_id) || (0 == meter_id) || (0 == color_id))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    pstlogicalport  = AVLL_FIND(SHARED.logical_port_tree, &log_id);
    if((NULL == pstlogicalport) || (NULL == pstlogicalport->phy_cfg_cb) || \
       (NULL == pstlogicalport->basic_cfg_cb))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 该逻辑端口下所有的intf都需要去绑定meter_id和color_action_id */
    for(pstterminalifcb = (SPM_TERMINAL_IF_CB*)AVLL_FIRST(pstlogicalport->terminal_if_tree); 
        pstterminalifcb != NULL; pstterminalifcb = (SPM_TERMINAL_IF_CB*)AVLL_NEXT( \
        pstlogicalport->terminal_if_tree, pstterminalifcb->spm_terminal_if_node))
    {
        /* intf上去绑定meter_id */
        for(unit = 0; unit < SHARED.c3_num; unit++)
        {
#if defined (SPU) || defined (PTN690_CES)           
            ret = ApiC3SetIntfMeter(unit, pstterminalifcb->intf_pos_id, 0, 0);
            if (ATG_DCI_RC_OK != ret)
            {
                printf("***err***%s,%d:ApiC3SetIntfMeter failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:ApiC3SetIntfMeter failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
#endif
        }

        /* intf上去绑定color_action_id */
        for(unit = 0; unit < SHARED.c3_num; unit++)
        {
#if defined (SPU) || defined (PTN690_CES)       
            ret = fhdrv_intf_set_policer_action(unit, pstterminalifcb->intf_pos_id, 0, 0);
            if (ATG_DCI_RC_OK != ret)
            {
                printf("***err***%s,%d:fhdrv_intf_set_policer_action failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:fhdrv_intf_set_policer_action failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
#endif
        } 
    }

    /* 删除驱动并释放color_action_id资源 */
    ret = spm_qos_del_color_action(&color_id);
    if(ATG_DCI_RC_OK != ret)
    {
        spm_qos_free_meter_error_log(&color_id, __FUNCTION__, __LINE__, ret);
        goto EXIT_LABEL;
    }

    /* 删除驱动并释放meter_id资源 */
    ret = spm_qos_del_txlsp_pw_meter(&meter_id);
    if(ATG_DCI_RC_OK != ret)
    {
        spm_qos_free_meter_error_log(&meter_id, __FUNCTION__, __LINE__, ret);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_add_pw_meter_qos_node
   功能描述  : 当flag不为0时,则删除txlsp的car功能
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_add_pw_car_qos_node(NBB_ULONG log_id, ATG_DCI_VC_KEY *pw_key,
    SUB_PORT *sub_port, ATG_DCI_LOG_PORT_CAR_DATA *pw_car)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    SPM_QOS_PW_CAR_CB  *join_node = NULL;
    NBB_CHAR ucmessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == pw_car) || (NULL == pw_key) || (0 == log_id) || (NULL == sub_port))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        OS_SPRINTF(ucmessage,"***err***%s,%d:input param err.\n\n", __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        goto EXIT_LABEL;
    }

    /* 判断本槽位 */
    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != g_qos_behavior_print)
    {
        printf("%s %s,%d log_index=%lu,slot=%d,port=%d,vc_id=%lu,vc_type=%d,peer_ip=0x%lx,meter.mode=%d.\n"
               "meter.color=%d,meter.cir=%ldMbps,meter.pir=%ldMbps,meter.cbs=%dKb,meter.pbs=%dKb.\n\n",
               QOS_CFG_STRING, __FUNCTION__, __LINE__, log_id, sub_port->slot, sub_port->port, pw_key->vc_id, 
               pw_key->vc_type, pw_key->peer_ip, pw_car->mode, pw_car->cm_color, (pw_car->cir/1000), 
               (pw_car->pir/1000), pw_car->cbs, pw_car->pbs);
        OS_SPRINTF(ucmessage,"%s %s,%d log_index=%lu,slot=%d,port=%d,vc_id=%lu,vc_type=%d,peer_ip=0x%lx,"
               "meter.mode=%d.\nmeter.color=%d,meter.cir=%ldMbps,meter.pir=%ldMbps,meter.cbs=%dKb,"
               "meter.pbs=%dKb.\n\n",QOS_CFG_STRING, __FUNCTION__, __LINE__, log_id, sub_port->slot, 
               sub_port->port, pw_key->vc_id, pw_key->vc_type, pw_key->peer_ip, pw_car->mode, pw_car->cm_color, 
               (pw_car->cir/1000), (pw_car->pir/1000), pw_car->cbs, pw_car->pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
    }

    join_node = AVLL_FIND(g_qos_pw_car_tree, &log_id);

    /* 该逻辑端口上没有绑定car功能 */
    if (NULL == join_node)
    {
        /* 未开启meter功能,配置出错,直接跳出 */
        if(0 == pw_car->mode)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            printf("***err***%s,%d:not open meter-function, cfg failed.\n\n", __FUNCTION__, 
                   __LINE__);
            OS_SPRINTF(ucmessage,"***err***%s,%d:not open meter-function, cfg failed.\n\n", 
                       __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
        }

        /* 逻辑端口上绑定car功能 */
        else
        {
            join_node = spm_alloc_qos_pw_car_cb(log_id);
            if (NULL == join_node)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("***err***%s,%d:spm_alloc_qos_pw_car_cb failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_alloc_qos_pw_car_cb failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ret = spm_set_qos_pw_meter(pw_car, &(join_node->meter_id));
            if (ATG_DCI_RC_OK != ret)
            {
                spm_free_qos_pw_car_cb(join_node);
                printf("***err***%s,%d:spm_set_qos_pw_meter failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_set_qos_pw_meter failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ret = spm_qos_set_pw_color_action(pw_car, &(join_node->color_action_id));
            if (ATG_DCI_RC_OK != ret)
            {
                spm_qos_del_txlsp_pw_meter(&(join_node->meter_id));
                spm_free_qos_pw_car_cb(join_node);
                printf("***err***%s,%d:spm_qos_set_pw_color_action failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_qos_set_pw_color_action failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            ret = spm_add_pw_car_driver(log_id, join_node->meter_id, join_node->color_action_id);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_del_pw_car_driver(log_id, join_node->meter_id, join_node->color_action_id);
                spm_free_qos_pw_car_cb(join_node);
                printf("***err***%s,%d:spm_add_pw_car_driver failed.\n\n", __FUNCTION__, __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_add_pw_car_driver failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }

            OS_MEMCPY(&(join_node->pw_car), pw_car, sizeof(ATG_DCI_LOG_PORT_CAR_DATA));
            OS_MEMCPY(&(join_node->vc_key), pw_key, sizeof(ATG_DCI_VC_KEY));
            rv = AVLL_INSERT(g_qos_pw_car_tree, join_node->spm_qos_pw_car_node); 
        }
    }

    /* 该逻辑端口上之前已经绑定了car功能 */
    else
    {
        /* 删除操作,删除操作时不检查free/del函数的返回值,默认删除成功 */
        if(0 == pw_car->mode)
        {
            spm_del_pw_car_driver(log_id, join_node->meter_id, join_node->color_action_id);
            AVLL_DELETE(g_qos_pw_car_tree, join_node->spm_qos_pw_car_node);
            spm_free_qos_pw_car_cb(join_node);
        }

        /* 更新meter的配置内容 */
        else
        {
            ret = spm_set_qos_pw_meter(pw_car, &(join_node->meter_id));
            if (ATG_DCI_RC_OK != ret)
            {
                printf("***err***%s,%d:spm_set_qos_pw_meter failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_set_qos_pw_meter failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            ret = spm_qos_set_pw_color_action(pw_car, &(join_node->color_action_id));
            if (ATG_DCI_RC_OK != ret)
            {
                printf("***err***%s,%d:spm_qos_set_pw_color_action failed.\n\n", __FUNCTION__, 
                       __LINE__);
                OS_SPRINTF(ucmessage,"***err***%s,%d:spm_qos_set_pw_color_action failed.\n\n", 
                           __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucmessage);
                goto EXIT_LABEL;
            }
            OS_MEMCPY(&(join_node->pw_car),pw_car,sizeof(ATG_DCI_LOG_PORT_CAR_DATA));
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_qos_clear_all_pw_car
   功能描述  : 当flag不为0时,则删除txlsp的car功能
   输入参数  : action的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_qos_clear_all_pw_car(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_PW_CAR_CB *cfg_cb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;

    for(cfg_cb = (SPM_QOS_PW_CAR_CB*) AVLL_FIRST(g_qos_pw_car_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_PW_CAR_CB*) AVLL_FIRST(g_qos_pw_car_tree))
    {
        AVLL_DELETE(g_qos_pw_car_tree, cfg_cb->spm_qos_pw_car_node);
        spm_qos_del_color_action(&(cfg_cb->color_action_id));
        spm_del_action_driver(cfg_cb);
        spm_free_qos_pw_car_cb(cfg_cb);
        cfg_cb = NULL;
    }

    return; 
}

#endif

#endif

#if 4

/*****************************************************************************
   函 数 名  : spm_qos_behavior_init
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
NBB_LONG spm_qos_behavior_init(void)
{
    NBB_BUF_SIZE avll_key_offset;

    OS_MEMSET(g_qos_flow_meter_id,   0, MAX_METER_NUM);
    OS_MEMSET(g_qos_color_action_id, 0, MAX_COLOR_ACTION_NUM);

    /* ACTION树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_ACTION_CB, action_key);
    AVLL_INIT_TREE(g_qos_action_tree, compare_ulong, 
                  (NBB_USHORT)avll_key_offset, 
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_ACTION_CB, spm_action_node));
#ifdef PTN690

    /* QOS pw car树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_PW_CAR_CB, log_key);
    AVLL_INIT_TREE(g_qos_pw_car_tree, compare_ulong, 
                  (NBB_USHORT)avll_key_offset, 
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_PW_CAR_CB, spm_qos_pw_car_node));
#endif
}


/*****************************************************************************
   函 数 名  : spm_ds_init
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
NBB_VOID spm_cfg_action_cb_verify(void)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_ACTION_CB *action_cb = NULL;
    ATG_DCI_QOS_BEHAVIOR_BASIC_DATA *basic_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_SUPERVISE *supervise_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_SHAPPING *shaping_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_SCHEDULE *schedule_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *flow_act_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *flow_samping_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_REDIRECT *redirect_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_POLICY_RT *policy_nhi_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_PHB *phb_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_DOMAIN *domain_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_URPF *urpf_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE que_shaping_cfg_cb[MAX_PHB_NUM];
    ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL que_schedul_cfg_cb[MAX_PHB_NUM];
    ATG_DCI_QOS_BEHAVIOR_QUE_CONGST que_congst_cfg_cb[MAX_PHB_NUM];

    NBB_TRC_ENTRY("spm_cfg_action_cb_verify");

    for (action_cb = (SPM_QOS_ACTION_CB*)AVLL_FIRST(g_qos_action_tree); action_cb != NULL;
         action_cb = (SPM_QOS_ACTION_CB*)AVLL_NEXT(g_qos_action_tree, action_cb->spm_action_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify action_cb %p", action_cb));
        NBB_VERIFY_MEMORY(action_cb, sizeof(SPM_QOS_ACTION_CB), MEM_SPM_ACTION_CB);

        basic_cfg_cb = action_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_BASIC_DATA),
                              MEM_SPM_ACTION_BASIC_CB);
        }

        supervise_cfg_cb = action_cb->supervise_cfg_cb;
        if(supervise_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify supervise_cfg_cb cb %p", supervise_cfg_cb));
            NBB_VERIFY_MEMORY(supervise_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_SUPERVISE),
                              MEM_SPM_ACTION_SUPERVISE_CB);
        }

        shaping_cfg_cb = action_cb->shaping_cfg_cb;
        if(shaping_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify shaping_cfg_cb cb %p", shaping_cfg_cb));
            NBB_VERIFY_MEMORY(shaping_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_SHAPPING),
                              MEM_SPM_ACTION_SHAPING_CB);
        }

        schedule_cfg_cb = action_cb->schedule_cfg_cb;
        if(schedule_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify schedule_cfg_cb cb %p", schedule_cfg_cb));
            NBB_VERIFY_MEMORY(schedule_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_SCHEDULE),
                              MEM_SPM_ACTION_SCHEDULE_CB);
        }

        flow_act_cfg_cb = action_cb->flow_act_cfg_cb;
        if(flow_act_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify flow_act_cfg_cb cb %p", flow_act_cfg_cb));
            NBB_VERIFY_MEMORY(flow_act_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_FLOW_ACT),
                              MEM_SPM_ACTION_FLOW_ACT_CB);
        }

        flow_samping_cfg_cb = action_cb->flow_samping_cfg_cb;
        if(flow_samping_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify flow_samping_cfg_cb cb %p", flow_samping_cfg_cb));
            NBB_VERIFY_MEMORY(flow_samping_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL),
                              MEM_SPM_ACTION_FLOW_SAMPL_CB);
        }

        redirect_cfg_cb = action_cb->redirect_cfg_cb;
        if(redirect_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify redirect_cfg_cb cb %p", redirect_cfg_cb));
            NBB_VERIFY_MEMORY(redirect_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_REDIRECT),
                              MEM_SPM_ACTION_REDIRECT_CB);
        }

        policy_nhi_cfg_cb = action_cb->policy_nhi_cfg_cb;
        if(policy_nhi_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify policy_nhi_cfg_cb cb %p", policy_nhi_cfg_cb));
            NBB_VERIFY_MEMORY(policy_nhi_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_POLICY_RT),
                              MEM_SPM_ACTION_POLCY_RT_CB);
        }

        phb_cfg_cb = action_cb->phb_cfg_cb;
        if(phb_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify phb_cfg_cb cb %p", phb_cfg_cb));
            NBB_VERIFY_MEMORY(redirect_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_PHB),
                              MEM_SPM_ACTION_PHB_CB);
        }

        domain_cfg_cb = action_cb->domain_cfg_cb;
        if(domain_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify domain_cfg_cb cb %p", domain_cfg_cb));
            NBB_VERIFY_MEMORY(domain_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_DOMAIN),
                              MEM_SPM_ACTION_DOMAIN_CB);
        }

        urpf_cfg_cb = action_cb->urpf_cfg_cb;
        if(urpf_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify urpf_cfg_cb cb %p", urpf_cfg_cb));
            NBB_VERIFY_MEMORY(urpf_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_URPF),
                              MEM_SPM_ACTION_URPF_CB);
        }
        
    }

    NBB_TRC_EXIT();

    return;
}

#ifdef PTN690

/*****************************************************************************
   函 数 名  : spm_cfg_pw_car_cb_verify
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
NBB_VOID spm_cfg_pw_car_cb_verify(void)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_PW_CAR_CB  *cfg_cb  = NULL;

    NBB_TRC_ENTRY("spm_cfg_pw_car_cb_verify");

    for (cfg_cb = (SPM_QOS_PW_CAR_CB*)AVLL_FIRST(g_qos_pw_car_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_PW_CAR_CB*)AVLL_NEXT(g_qos_pw_car_tree, cfg_cb->spm_qos_pw_car_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_PW_CAR_CB), MEM_SPM_QOS_PW_CAR_CB);
    }

    NBB_TRC_EXIT();

    return;
}

#endif

#endif
