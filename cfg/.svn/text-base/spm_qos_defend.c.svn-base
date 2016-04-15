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


/* 外部打印开关全局变量 */
extern NBB_BYTE acl_pri_setting;

/* 外部打印开关全局变量 */
extern NBB_BYTE qos_defend_cfg_print;



/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
SPM_QOS_DEFEND_POLICY_CB * spm_alloc_defend_policy_cb(NBB_ULONG ulkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DEFEND_POLICY_CB *pstTbl = NULL;
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_defend_policy_cb");

    if((0 == ulkey) || (ulkey > 50)) /*超出范围*/
    {
       printf("**QOS ERROR**%s,%d policy index=%d\n",__FUNCTION__,__LINE__,ulkey);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            		   "QOS spm_defend_set_acl_car",ATG_DCI_RC_UNSUCCESSFUL,
            		   "policy index","","0","",ulkey,0,0,0)); 
        goto EXIT_LABEL;
    }

    /* 分配一个新的DS表配置条目。*/
    pstTbl = (SPM_QOS_DEFEND_POLICY_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DEFEND_POLICY_CB),
              NBB_NORETRY_ACT, MEM_SPM_DEFEND_POLICY_CB);
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始defend策略配置条目 */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DEFEND_POLICY_CB));
    pstTbl->policy_key = ulkey;

    /*默认给用户队列桶优先级赋值防止只配置防攻击开关导致业务不正常*/
    pstTbl->usr_pri = 2;
    pstTbl->usr_que_pri = 2;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_defend_policy_node);

    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DEFEND_APPERC_CB, apperc_key);
    AVLL_INIT_TREE(pstTbl->apperc_tree, spm_qos_defend_apperc_comp,(NBB_USHORT)avll_key_offset,
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DEFEND_APPERC_CB, spm_defend_apperc_node));


    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DEFEND_USER_DEF_CB, user_def_key);
    AVLL_INIT_TREE(pstTbl->user_def_tree, spm_qos_defend_usr_def_comp,(NBB_USHORT)avll_key_offset,
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DEFEND_USER_DEF_CB, spm_defend_user_def_node));

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
SPM_QOS_DEFEND_CB * spm_alloc_defend_cb(NBB_ULONG ulkey NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DEFEND_CB *pstTbl = NULL;
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_defend_policy_cb");

    pstTbl = (SPM_QOS_DEFEND_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DEFEND_CB),
              NBB_NORETRY_ACT, MEM_SPM_DEFEND_CB);
    if (pstTbl == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始ds配置条目 */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_DEFEND_CB));
    pstTbl->key = ulkey;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_defend_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_free_defend_policy_cb(SPM_QOS_DEFEND_POLICY_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_APPERC_CB *pcb = NULL;
    SPM_QOS_DEFEND_USER_DEF_CB *npb = NULL;

    NBB_TRC_ENTRY("spm_free_defend_policy_cb");

    NBB_ASSERT(NULL != pstTbl);

    if (NULL == pstTbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DEFEND_POLICY_CB), MEM_SPM_DEFEND_POLICY_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    for (pcb = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_FIRST(pstTbl->apperc_tree); pcb != NULL;
         pcb = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_FIRST(pstTbl->apperc_tree))
    {
        AVLL_DELETE(pstTbl->apperc_tree, pcb->spm_defend_apperc_node);
        NBB_MM_FREE(pcb, MEM_SPM_DEFEND_APPERC_CB);
    }


    for (npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pstTbl->user_def_tree); npb != NULL;
         npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pstTbl->user_def_tree))
    {
        AVLL_DELETE(pstTbl->user_def_tree, npb->spm_defend_user_def_node);
        NBB_MM_FREE(npb, MEM_SPM_DEFEND_USR_DEF_CB);
    }
    
    NBB_MM_FREE(pstTbl, MEM_SPM_DEFEND_POLICY_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_free_defend_cb(SPM_QOS_DEFEND_CB *pstTbl NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    AVLL_NODE node;
    NBB_TRC_ENTRY("spm_free_defend_cb");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pstTbl);

    /*释放内存失败*/
    if (NULL == pstTbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_DEFEND_CB), MEM_SPM_DEFEND_CB);
    NBB_MM_FREE(pstTbl, MEM_SPM_DEFEND_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_del_acl(NBB_ULONG acl_id,NBB_ULONG rule_id NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};

    NBB_TRC_ENTRY("spm_defend_del_acl");

    if(0 == rule_id)
    {
        goto EXIT_LABEL;
    }

    acl.mAclId = acl_id;
    acl.mRuleId = rule_id;
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
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_qos_del_meter(NBB_ULONG *meter_id NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    METER_CFG_T meter = {0};

    NBB_TRC_ENTRY("spm_qos_del_meter");

    if(0 != *meter_id)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]       
            ret = ApiC3DelMeter(unit,*meter_id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_meter_error_log(unit,*meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }
#endif
        }
        ret = spm_qos_free_meter_id(meter_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_free_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_qos_set_meter(NBB_ULONG cir,NBB_ULONG pir,
    NBB_ULONG cbs,NBB_ULONG pbs,NBB_ULONG *meterFlag,NBB_ULONG *meter_id NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    METER_CFG_T meter = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_qos_set_meter");

    /*CIR == 0代表不限速，不进行meter配置*/
    if(0 != cir)
    {
        if(0 == *meter_id)
        {
            *meterFlag = 1;
            ret = spm_qos_apply_meter_id(meter_id NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_qos_apply_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }
        }

        meter.cir = cir;
        meter.pireir = pir;

        /* 单盘协议的单位是KB,驱动的单位是Byte */
        meter.cbs = cbs * 1024;
        meter.pbsebs = pbs * 1024;
        meter.meterId = *meter_id;
        meter.dropRed = 1;
#if 0
        RFC_2697,          /* RFC_2697*/
        RFC_2698,          /* RFC_2698*/
        RFC_4115,          /* RFC_4115*/
        MEF                   /* MEF*/
#endif
        meter.eMeterType = RFC_2697;/*暂时写死*/
        
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetMeter(unit,&meter);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**ApiC3SetMeter ERROR=%d\n",ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        					   "QOS ApiC3SetMeter",ret,
        					   "meterId","","unit","",meter.meterId,0,unit,0));
                OS_SPRINTF(ucMessage,"%s %s,%d : ret=%d,QOS ApiC3SetMeter.\n"
                           "meterId=%d,unit=%d.\n\n",QOS_CFG_STRING,
                           __FUNCTION__,__LINE__,ret,meter.meterId,unit);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
                if(0 != *meterFlag)
                {
                    ret = spm_qos_free_meter_id(meter_id NBB_CCXT);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_qos_free_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
                    } 
                }
                goto EXIT_LABEL;
            }
#endif
        }
    }
    else 
    {
        ret = spm_qos_del_meter(meter_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*返回值=ATG_DCI_RC_OK本槽位实例化*/
NBB_LONG spm_defend_check_instance(SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE slot = 0;

    NBB_TRC_ENTRY("spm_defend_check_instance");

    if(NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**%s,%d defend policy==NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            		   "QOS defend policy==NULL",ATG_DCI_RC_UNSUCCESSFUL,
            		   "","","0","",0,0,0,0));
        goto EXIT_LABEL;
    }
    slot = SHARED.local_slot_id;
    if(ATG_DCI_RC_OK == pcb->defend_slot[slot])/*未被实例化*/
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*flag = ATG_DCI_RC_OK白名单*/
NBB_LONG spm_defend_del_acl_car(NBB_LONG flag,NBB_ULONG *meter_id NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    METER_CFG_T meter = {0};

    NBB_TRC_ENTRY("spm_defend_del_acl_car");
    
    if(0 != *meter_id)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            if(ATG_DCI_RC_OK != flag)
            {
                //coverity[dead_error_condition]
                ret = ApiC3SetSlotBlackListMeter(unit,*meter_id,0);
            }
            else
            {
                //coverity[dead_error_condition]
                ret = ApiC3SetSlotWhiteListMeter(unit,*meter_id,0); 
            }
            if(ATG_DCI_RC_OK != ret)
            {
                 printf("**QOS ERROR**%s meter_id =%d\n",flag?"black":"white",*meter_id);
                 NBB_EXCEPTION((PCT_SPM| QOS_PD, 2,  "s d s s s s d d d d", 
                     			"QOS slot meter",ret,
                     			"flag","disable","unit","meter_id",flag,0,unit,*meter_id));
                 goto EXIT_LABEL;
            }
#endif
        }

        
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]   
            ret = ApiC3DelMeter(unit,*meter_id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_meter_error_log(unit,*meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }
#endif
        }
        ret = spm_qos_free_meter_id(meter_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_free_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*flag=ATG_DCI_RC_OK白名单*/
NBB_LONG spm_defend_set_acl_car(NBB_LONG flag,NBB_ULONG cir,
    NBB_ULONG pir,NBB_ULONG cbs,NBB_ULONG pbs,NBB_ULONG *meter_id NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    METER_CFG_T meter = {0};

    NBB_TRC_ENTRY("spm_defend_set_acl_car");

    if(0 != cir)
    {
        ret = spm_qos_apply_meter_id(meter_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_apply_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
            goto EXIT_LABEL;
        }

        meter.cir = cir;
        meter.pireir = pir;

        /*单盘协议的单位是KB，驱动的单位是Byte*/
        meter.cbs = cbs * 1024;
        meter.pbsebs = pbs * 1024;
        meter.meterId = *meter_id;
        meter.dropRed = 1;
#if 0
        RFC_2697,          /* RFC_2697*/
        RFC_2698,          /* RFC_2698*/
        RFC_4115,          /* RFC_4115*/
        MEF                   /* MEF*/
#endif
        meter.eMeterType = RFC_2697;/*暂时写死*/
        
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetMeter(unit,&meter);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**ApiC3SetMeter ERROR=%d\n",ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        					   "QOS ApiC3SetMeter",ret,
        					   "flag","","unit","",flag,0,unit,0));
                ret = spm_qos_free_meter_id(meter_id NBB_CCXT);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_qos_free_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
                }
                goto EXIT_LABEL;
            }
#endif
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            if(ATG_DCI_RC_OK != flag)
            {
                //coverity[dead_error_condition]
                ret += ApiC3SetSlotBlackListMeter(unit,meter.meterId,1);
            }
            else
            {
                //coverity[dead_error_condition]
                ret += ApiC3SetSlotWhiteListMeter(unit,meter.meterId,1);
            }
            if(ATG_DCI_RC_OK != ret)
            {
                 printf("**QOS ERROR**%s meter_id =%d\n",flag?"black":"white",meter.meterId);
                 NBB_EXCEPTION((PCT_SPM| QOS_PD, 2,  "s d s s s s d d d d", 
                     			"QOS slot meter",ret,
                     			"flag","enable","unit","meter_id",
                     			flag,0,unit,meter.meterId));
                 goto EXIT_LABEL;
            }
#endif 
        }
        
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_apperswitch(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_APPERC_S *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_LONG switch_flag = 0;
    NBB_ULONG mAclId = 0;

    NBB_TRC_ENTRY("spm_defend_set_apperswitch");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    mAclId = (pcb->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            
            ret = ApiC3SetSlotRelativeAclEnable(unit,0);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotRelativeAclEnable %s,ret=%d\n",
                          __FUNCTION__,__LINE__,switch_flag?"开":"关",ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotUserAclEnable",ret,
                			   switch_flag?"开":"关","unit","","",0,unit,0,0));
                goto EXIT_LABEL;
            }
#endif
        }
        pcb->apperc_policy_switch.switch_flag = 1;/*设置为关*/
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*消息内容为空*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*消息内容不为空*/
        else
        {       
            /* 防攻击策略模板未被引用的情况下无作用 */
            if(ATG_DCI_RC_OK == pcb->defend_slot[SHARED.local_slot_id])
            {
                goto EXIT_LABEL;
            }
            switch_flag = (0 == buf->switch_flag)?1:0;
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                
                ret = ApiC3SetSlotRelativeAclEnable(unit,switch_flag);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotRelativeAclEnable %s,ret=%d\n",
                              __FUNCTION__,__LINE__,switch_flag?"开":"关",ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotUserAclEnable",ret,
                    			   switch_flag?"开":"关","unit","","",0,unit,0,0));
                    goto EXIT_LABEL;
                }
#endif
                if(1 == switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    ret = ApiC3SetSlotRelativeAcl(unit,mAclId,pcb->apper_pri);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        printf("**QOS ERROR**,%s,%d ApiC3SetSlotRelativeAcl acl id=%d,pri=%d,ret=%d\n",
                                  __FUNCTION__,__LINE__,mAclId,pcb->apper_pri,ret);
                        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                        			   "QOS ApiC3SetSlotRelativeAcl",ret,
                        			   "acl id","pri","","",mAclId,pcb->apper_pri,0,0));
                        goto EXIT_LABEL;
                    }
#endif
                }
            }
            
            //coverity[bad_sizeof]  
            NBB_MEMCPY(&(pcb->apperc_policy_switch),buf,sizeof(ATG_DCI_DEFEND_POLICY_APPERC_S));
        }
        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_del_white_cfg(SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_defend_del_white_cfg");

    if(NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d white policy==NULL\n",
                  __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS white policy buf==NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "0","","","",0,0,0,0));
        goto EXIT_LABEL;
    }
    ret = spm_defend_del_acl_car(ATG_DCI_RC_OK,&(pcb->white_meter) NBB_CCXT);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
 
    /**********************反刷配置不需要*********************/
    rv = spm_defend_check_instance(pcb NBB_CCXT);
    if(ATG_DCI_RC_OK == rv)
    {  
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotWhiteAcl(unit,0,pcb->white_policy.prio,pcb->white_policy.queue_prio);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS DEL ERROR**,%s,%d ApiC3SetSlotWhiteAcl acl=0,ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotWhiteAcl",ret,
                			   "acl id","pri","queue_prio","defend policy index",
                			   0,pcb->white_policy.prio,
                			   pcb->white_policy.queue_prio,pcb->policy_key));
            }
#endif
        }
    }
    OS_MEMSET(&(pcb->white_policy),0,sizeof(ATG_DCI_DEFEND_POLICY_WHITE));
    pcb->white_meter = 0;
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_add_white_cfg(ATG_DCI_DEFEND_POLICY_WHITE *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_defend_add_white_cfg");

    if(NULL == buf)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d white policy buf==NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS white policy buf==NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index","","","",pcb->policy_key,0,0,0));
        goto EXIT_LABEL;
    }
    rv = NBB_MEMCMP(buf,&(pcb->white_policy),sizeof(ATG_DCI_DEFEND_POLICY_WHITE));
    if(0 != rv)
    {
        ret = spm_defend_del_acl_car(ATG_DCI_RC_OK,&(pcb->white_meter) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        /*RC_2697单速率三色桶，默认pir为0，cbs与pbs相等.*/
        ret = spm_defend_set_acl_car(ATG_DCI_RC_OK,buf->cir,0,buf->cbs,buf->cbs,&(pcb->white_meter) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    /**********************反刷配置*********************/
    rv = spm_defend_check_instance(pcb NBB_CCXT);
    if(ATG_DCI_RC_OK == rv)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotWhiteAcl(unit,buf->acl_index,
                buf->prio,buf->queue_prio);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotWhiteAcl acl=%d,ret=%d\n",
                          __FUNCTION__,__LINE__,buf->acl_index,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotWhiteAcl",ret,
                			   "acl id=%d","pri","queue_prio","defend policy index",
                			   buf->acl_index,buf->prio,buf->queue_prio,pcb->policy_key));
            }
#endif
        }
    }
    
    //coverity[bad_sizeof]  
    NBB_MEMCPY(&(pcb->white_policy),buf,sizeof(ATG_DCI_DEFEND_POLICY_WHITE));   

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_white(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_WHITE *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_defend_set_white");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
            ret = spm_defend_del_white_cfg(pcb NBB_CCXT);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
            ret = spm_defend_add_white_cfg(buf,pcb NBB_CCXT);
        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_del_black_cfg(SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_defend_del_black_cfg");

    if(NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d black policy==NULL\n",
                  __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS black policy buf==NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "0","","","",0,0,0,0));
        goto EXIT_LABEL;
    }
    ret = spm_defend_del_acl_car(ATG_DCI_RC_UNSUCCESSFUL,&(pcb->black_meter) NBB_CCXT);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    /**********************反刷配置不需要*********************/
    rv = spm_defend_check_instance(pcb NBB_CCXT);
    if(ATG_DCI_RC_OK == rv)
    {  
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotBlackAcl(unit,0,pcb->black_policy.prio,pcb->black_policy.queue_prio);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS DEL ERROR**,%s,%d ApiC3SetSlotBlackAcl acl=0,ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotBlackAcl",ret,
                		       "acl id","pri","queue_prio","defend policy index",
                			   0,pcb->black_policy.prio,pcb->black_policy.queue_prio,pcb->policy_key));
            }
#endif
        }
    }
    OS_MEMSET(&(pcb->black_policy),0,sizeof(ATG_DCI_DEFEND_POLICY_BLACK) NBB_CCXT);
    pcb->black_meter = 0;
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_add_black_cfg(ATG_DCI_DEFEND_POLICY_BLACK *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_defend_add_black_cfg");

    if(NULL == buf)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d black policy buf==NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS black policy buf==NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index","","","",pcb->policy_key,0,0,0));
        goto EXIT_LABEL;
    }
    rv = NBB_MEMCMP(buf,&(pcb->black_policy),sizeof(ATG_DCI_DEFEND_POLICY_BLACK));
    if(0 != rv)
    {
        ret = spm_defend_del_acl_car(ATG_DCI_RC_UNSUCCESSFUL,&(pcb->black_meter) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        /*RC_2697单速率三色桶，默认pir为0，cbs与pbs相等.*/
        ret = spm_defend_set_acl_car(ATG_DCI_RC_UNSUCCESSFUL,buf->cir,0,buf->cbs,buf->cbs,&(pcb->black_meter) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    /**********************反刷配置*********************/
    rv = spm_defend_check_instance(pcb NBB_CCXT);
    if(ATG_DCI_RC_OK == rv)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotBlackAcl(unit,buf->acl_index,buf->prio,buf->queue_prio);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotBlackAcl acl=%d,ret=%d\n",
                          __FUNCTION__,__LINE__,buf->acl_index,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotBlackAcl",ret,
                			   "acl id=%d","pri","queue_prio","defend policy index",
                			   buf->acl_index,buf->prio,buf->queue_prio,pcb->policy_key));
            }
#endif
        }
    }
    
    //coverity[bad_sizeof]  
    NBB_MEMCPY(&(pcb->black_policy),buf,sizeof(ATG_DCI_DEFEND_POLICY_BLACK));

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_black(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_BLACK *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_defend_set_black");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
            ret = spm_defend_del_black_cfg(pcb NBB_CCXT);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
            ret = spm_defend_add_black_cfg(buf,pcb NBB_CCXT);
        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_del_apper_acl(NBB_ULONG acl_id,NBB_ULONG rule_id NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};
    NBB_USHORT i = 0;

    NBB_TRC_ENTRY("spm_defend_del_apper_acl");

    if(0 == rule_id)
    {
        goto EXIT_LABEL;
    }

    acl.mAclId = acl_id;
    if(rule_id == DEFEND_DEFAULT)
    {
        acl.mRuleId = DEFEND_DEFAULT_RULE_ID;
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelAcl(unit, &acl);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
            }
#endif
        }
        goto EXIT_LABEL;
    }

    for(i = 0;(rule_id <= SIGNAL_NUM) && (i < SHARED.qos_defend_num[rule_id]);i++)
    {
        acl.mRuleId = SHARED.qos_defend_offset[rule_id] + i;
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
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_ldpudp(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ldp");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4udp组播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 0x0286;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6udp组播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 0x0286;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_ldptcp(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ldp");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 0x0286;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 0x0286;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 0x0286;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 0x0286;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_snmp(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_snmp");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4udp*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 161;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6udp*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 161;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_radius(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_radius");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4udp*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 6;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 49;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6udp*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 6;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 49;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_bgpv4(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_bgpv4");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 179;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 179;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_bgpv6(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_bgpv6");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];
    
    /***************tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 179;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 179;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_telnet_client(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_telnet_client");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];
    
    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 23;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 2650;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 3000;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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
  
    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 23;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 2650;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 3000;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_telnet_service(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_telnet_service");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 23;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 2650;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 3000;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 23;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 2650;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 3000;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
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
NBB_LONG spm_defend_apper_ssh_client(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ssh_client");   

    rule_id = SHARED.qos_defend_offset[l3_protcal];
    
    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 22;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 6000;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 22;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 6000;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_ssh_service(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ssh_service");   

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 22;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 6000;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 22;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0X86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 6000;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x6;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_dhcp(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_dhcp");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4 udp client*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 0x44;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4 udp serve*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 0x43;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_tacacs(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_tacacs");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4 udp *********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 1812;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4 udp *********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 1813;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6 udp *********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 1812;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6 udp *********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 1813;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_dhcpv6(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_dhcp");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4 udp client*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 546;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV4 udp serve*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 547;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_tcpsyn(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_tcpsyn");

    rule_id = SHARED.qos_defend_offset[l3_protcal];
    
    /***************IPV4*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 6;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.tcp_flag = 0x2;
    pstAcl->tAclRule.tAclMask.tcp_flag = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 6;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.tcp_flag = 0x2;
    pstAcl->tAclRule.tAclMask.tcp_flag = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;  
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_ftp_service(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ftp_service");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 20;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x06;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 21;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x06;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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
  
    /***************IPV6*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 20;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x06;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4DstPort = 21;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x06;
    pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}



/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_ftp_client(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ftp_client");  

    rule_id = SHARED.qos_defend_offset[l3_protcal];

    /***************IPV4*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 20;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x06;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x0800;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 21;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x06;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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
  
    /***************IPV6*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 20;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x06;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l4SrcPort = 21;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x06;
    pstAcl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}



/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_mld(ACL_T *pstAcl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_mld");

    rule_id = SHARED.qos_defend_offset[l3_protcal];
    
    /***************IPV6*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x3a;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.icmpType = 0x82;
    pstAcl->tAclRule.tAclMask.icmpType = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x3a;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.icmpType = 0x83;
    pstAcl->tAclRule.tAclMask.icmpType = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x3a;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.icmpType = 0x84;
    pstAcl->tAclRule.tAclMask.icmpType = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    /***************IPV6*********************/
    OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pstAcl->eAclAction = ACL_ACTION_PASS;
    pstAcl->mRuleId = rule_id++;
    pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
    pstAcl->tAclRule.tAclMask.ethType = 0xffff;
    pstAcl->tAclRule.tAclKey.l3Protocol = 0x3a;
    pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
    pstAcl->tAclRule.tAclKey.icmpType = 0x8f;
    pstAcl->tAclRule.tAclMask.icmpType = 0xff;
    pstAcl->phb_cos = (NBB_BYTE)que_pri;
    pstAcl->flags |= ACL_SET_PHB;

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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;  
}



/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_apper_acl_cfg(ACL_T *pstAcl,NBB_USHORT type,
    NBB_USHORT queue_prio NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_defend_apper_acl_cfg");

    if(type <= SIGNAL_NUM)
    {
        pstAcl->mRuleId = SHARED.qos_defend_offset[type];
    }
    
    switch(type)
    {
        case DEFEND_OSPF:
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x59;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_ISIS:
        pstAcl->tAclRule.tAclKey.customerData6 = 0xfefe0000;
        pstAcl->tAclRule.tAclMask.customerData6 = 0xffff0000;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_BGP:
        ret = spm_defend_apper_bgpv4(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_LDPUDP:
        ret = spm_defend_apper_ldpudp(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_LDP:
        ret = spm_defend_apper_ldptcp(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_RSVP:
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x2e;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_RIP:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_MSDP:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_PIM:
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x67;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_SNMP:
        ret = spm_defend_apper_snmp(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_RADIUS:
        ret = spm_defend_apper_radius(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_LSP_PING:
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x11;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->tAclRule.tAclKey.l4DstPort = 3503;
        pstAcl->tAclRule.tAclMask.l4DstPort = 0xffff;
        pstAcl->tAclRule.tAclKey.dip = 0x7f000000;
        pstAcl->tAclRule.tAclMask.dip = 0xffffff00;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_IGMP:
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x2;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_ICMP:
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x1;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_VRRP:
        pstAcl->tAclRule.tAclKey.ethType = 0x0800;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x70;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->tAclRule.tAclKey.dip = 0xE0000012;
        pstAcl->tAclRule.tAclMask.dip = 0xffffffff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_DHCP:
        ret = spm_defend_apper_dhcp(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_LACP:
        pstAcl->tAclRule.tAclKey.ethType = 0x8809;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.customerData6  = 0x01000000;
        pstAcl->tAclRule.tAclMask.customerData6 = 0xff000000;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_BFD:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_TACACS:
        ret = spm_defend_apper_tacacs(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_NTP:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_FTP_S:
        ret = spm_defend_apper_ftp_service(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_FTP_C:
        ret = spm_defend_apper_ftp_client(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_TELNET_S:
        ret = spm_defend_apper_telnet_service(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_TELNET_C:
        ret = spm_defend_apper_telnet_client(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_SSH_S:
        ret = spm_defend_apper_ssh_service(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_SSH_C:
        ret = spm_defend_apper_ssh_client(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_OSPFV3:
        pstAcl->tAclRule.tAclKey.ethType = 0x86dd;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x59;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_MLD:
        ret = spm_defend_apper_mld(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_BGPV6:
        ret = spm_defend_apper_bgpv6(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_RIPNG:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_ICMPV6:
        pstAcl->tAclRule.tAclKey.ethType = 0x86DD;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x3A;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_VRRPV6:
        pstAcl->tAclRule.tAclKey.ethType = 0x86DD;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x70;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_DHCPV6:
        ret = spm_defend_apper_dhcpv6(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_PIMV6:
        pstAcl->tAclRule.tAclKey.ethType = 0x86DD;
        pstAcl->tAclRule.tAclMask.ethType = 0xffff;
        pstAcl->tAclRule.tAclKey.l3Protocol = 0x67;
        pstAcl->tAclRule.tAclMask.l3Protocol = 0xff;
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_UNKNOWN_ARP:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_UNKNOWN_MULICAST:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_TCPSYN:
        ret = spm_defend_apper_tcpsyn(pstAcl,type,queue_prio NBB_CCXT);
        goto EXIT_LABEL;
        break;

        case DEFEND_DEFAULT:
        OS_MEMSET(&(pstAcl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
        OS_MEMSET(&(pstAcl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
        pstAcl->phb_cos = (NBB_BYTE)queue_prio;
        pstAcl->flags |= ACL_SET_PHB;
        pstAcl->mRuleId = DEFEND_DEFAULT_RULE_ID;
        pstAcl->eAclAction = ACL_ACTION_DROP;
        break;

        default:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;    
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

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_add_apper_acl(ATG_DCI_DEFEND_POLICY_APPERC_P *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb,NBB_ULONG *meter_id NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG meterFlag = 0;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};

    NBB_TRC_ENTRY("spm_defend_add_apper_acl");

    if(NULL == buf)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d appre policy buf==NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS appre policy buf==NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index=%d","","","",pcb->policy_key,0,0,0));
        goto EXIT_LABEL;
    }

    /*RC_2697单速率三色桶，默认pir为0，cbs与pbs相等.*/
    ret = spm_qos_set_meter(buf->cir,0,buf->cbs,buf->cbs,&meterFlag,meter_id NBB_CCXT);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    OS_MEMSET(&acl,0,sizeof(ACL_T));
    if(0 != *meter_id)
    {
        acl.meterIdx = *meter_id;
        acl.flags |= ACL_METER_ON;
    }

    acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    acl.eAclAction = ACL_ACTION_PASS;
    if(0 != acl_pri_setting)/*ACL的优先级*/
    {
        acl.priority = buf->l3_protoc;
    }
    ret = spm_defend_apper_acl_cfg(&acl,buf->l3_protoc,buf->queue_prio NBB_CCXT);
    if(ATG_DCI_RC_OK != ret)
    {
        ret = ATG_DCI_RC_MOD_FAILED;
        printf("**UPDATE QUEUE PRIO ERROR**,%s,%d\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS UPDATE QUEUE PRIO ERROR",ATG_DCI_RC_MOD_FAILED,
					   "policy index","l3_protoc","prio","queue_prio",
					   pcb->policy_key,buf->l3_protoc,buf->prio,buf->prio));

       /***************************************************************************
        *如果meter_id是本次新申请的，那么当联动策略的acl写入硬件失败后，需要将此次*
        *申请的meter_id释放。若meter_id不是本次新申请的，即使acl写入失败也不必释放*
        ***************************************************************************/
        if(0 != meterFlag)
        {
            ret = spm_qos_del_meter(meter_id NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS DEL METER FAIL**%s,%d, meter_id=%d spm_qos_del_meter ERROR=%d,\n",
                          __FUNCTION__,__LINE__,*meter_id,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        					   "QOS spm_qos_del_meter",ret,
        					   "","","unit","meter_id",0,0,unit,*meter_id));
            }
        }
        goto EXIT_LABEL;
    } 

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


NBB_LONG spm_defend_del_apper_cfg(ATG_DCI_DEFEND_POLICY_APPERC_P *buf,
        SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_APPERC_CB *pstAp = NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG acl_id = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_defend_del_apper_cfg");

    if(NULL == buf)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d apper policy buf==NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS apper policy buf==NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index=%d","","","",pcb->policy_key,0,0,0));
        goto EXIT_LABEL;
    }

    acl_id = (pcb->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    pstAp = AVLL_FIND(pcb->apperc_tree, buf);
    if(NULL == pstAp)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS CAN'T FIND APPER_TREE'S NODE, CAN'T DEL. JUST PRINT THE CONFIGURATION.\n"
                   "policy_index = %d, l3_protoc = %d.\n\n",__FUNCTION__,__LINE__,pcb->policy_key,buf->l3_protoc);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }       
    else
    {
        ret = spm_qos_del_meter(&(pstAp->meter_id) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_defend_del_apper_acl(acl_id,pstAp->apperc_key.l3_protoc NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(pcb->apperc_tree, pstAp->spm_defend_apperc_node);
        NBB_MM_FREE(pstAp,MEM_SPM_DEFEND_APPERC_CB);
        pstAp = NULL;
    }  

    /************不需要反刷*************/ 
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_add_apper_cfg(ATG_DCI_DEFEND_POLICY_APPERC_P *buf,
        SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG instanceFlag = ATG_DCI_RC_OK;
    NBB_LONG appre_acl_id = 0;
    ACL_T acl = {0};
    SPM_QOS_DEFEND_APPERC_CB *pstAp  = NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG meter_id = 0;

    NBB_TRC_ENTRY("spm_defend_add_apper_cfg");

    if(NULL == buf)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d apper policy buf==NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS apper policy buf==NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index=%d","","","",pcb->policy_key,0,0,0));
        goto EXIT_LABEL;
    }
    if(((buf->l3_protoc) > SIGNAL_NUM) && (0xff != buf->l3_protoc))
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d l3_protoc =%d out of rang,policy index=%d\n",
                  __FUNCTION__,__LINE__,buf->l3_protoc,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS l3_protoc out of rang",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index=%d","l3_protoc","","",
					   pcb->policy_key,buf->l3_protoc,0,0));
        goto EXIT_LABEL;
    }

    /*判断防攻击策略是否被该slot绑定*/
    instanceFlag = spm_defend_check_instance(pcb NBB_CCXT);
    appre_acl_id = (pcb->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    pstAp = AVLL_FIND(pcb->apperc_tree, buf);
    if(NULL == pstAp)
    {
        ret = spm_defend_add_apper_acl(buf,pcb,&meter_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        if(/*(ATG_DCI_RC_OK == instanceFlag) && */(pcb->apper_pri != buf->prio))
        {
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotRelativeAcl(unit,appre_acl_id,buf->prio);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotRelativeAcl appre_acl_id=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,appre_acl_id,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotRelativeAcl",ret,
                    			   "acl id=%d","pri","queue_prio","defend policy index",
                    			   appre_acl_id,buf->prio,buf->queue_prio,pcb->policy_key));
                }
#endif
            }
        }
        pstAp = (SPM_QOS_DEFEND_APPERC_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_DEFEND_APPERC_CB),
                 NBB_NORETRY_ACT, MEM_SPM_DEFEND_APPERC_CB);
        AVLL_INIT_NODE(pstAp->spm_defend_apperc_node);
        
        //coverity[bad_sizeof]  
        NBB_MEMCPY(&(pstAp->apperc_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_APPERC_P));
        pstAp->meter_id = meter_id;
        pcb->apper_pri = buf->prio;
        pcb->apper_que_pri = buf->queue_prio;
        
        //coverity[no_effect_test]
        AVLL_INSERT(pcb->apperc_tree, pstAp->spm_defend_apperc_node);
    }  
    else
    {
        rv = NBB_MEMCMP(&(pstAp->apperc_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_APPERC_P));
        if(0 != rv)
        {
            if(pcb->apper_pri != buf->prio)
            {
                pcb->apper_pri = buf->prio;
                
                /*if(ATG_DCI_RC_OK == instanceFlag)*/
                {
                    for(unit = 0;unit < SHARED.c3_num;unit++)
                    {
#if defined (SPU) || defined (PTN690_CES)

                        //coverity[dead_error_condition]
                        ret = ApiC3SetSlotRelativeAcl(unit,appre_acl_id,buf->prio);
                        if(ATG_DCI_RC_OK != ret)
                        {
                            printf("**QOS ERROR**,%s,%d ApiC3SetSlotRelativeAcl appre_acl_id=%d,ret=%d\n",
                                      __FUNCTION__,__LINE__,appre_acl_id,ret);
                            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                            			   "QOS ApiC3SetSlotRelativeAcl",ret,
                            			   "acl id=%d","pri","queue_prio","defend policy index",
                            			   appre_acl_id,buf->prio,buf->queue_prio,pcb->policy_key));
                        }
#endif
                    }
                }
            }
            ret = spm_defend_add_apper_acl(buf,pcb,&(pstAp->meter_id) NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("**DEFEND UPDATE FUNCTION FAILED**,%s,%d\n",__FUNCTION__,__LINE__);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					       "DEFEND UPDATE FUNCTION FAILED",ATG_DCI_RC_UNSUCCESSFUL,
    					       "policy index","l3_protoc","prio","queue_prio",
    					       pcb->policy_key,buf->l3_protoc,buf->prio,buf->queue_prio));
                goto EXIT_LABEL;
            }
            
            //coverity[bad_sizeof]   
            NBB_MEMCPY(&(pstAp->apperc_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_APPERC_P));
        }
    }  

    /************不需要反刷*******************************/ 
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}



/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_apperceive(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_APPERC_P *buf,
        SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    SPM_QOS_DEFEND_APPERC_CB *pstAp = NULL;

    NBB_TRC_ENTRY("spm_defend_set_apperceive");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    switch (oper)/*ips消息类型*/
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = spm_defend_del_apper_cfg(buf,pcb NBB_CCXT);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
        ret = spm_defend_add_apper_cfg(buf,pcb NBB_CCXT);
        break;

        default:
        break;
    }       

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_add_usr_acl(ATG_DCI_DEFEND_POLICY_USER_DF *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb,NBB_ULONG *meter_id NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG meterFlag = 0;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_defend_add_usr_acl");

    if(NULL == buf)
    {
        printf("**QOS ERROR**,%s,%d usr policy buf==NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS usr policy buf = NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index","","","",pcb->policy_key,0,0,0));
        OS_SPRINTF(ucMessage,"%s %s,%d policy index=%d.\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,pcb->policy_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /*RC_2697单速率三色桶，默认pir为0，cbs与pbs相等.*/
    ret = spm_qos_set_meter(buf->cir,0,buf->cbs,buf->cbs,&meterFlag,meter_id NBB_CCXT);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(&acl,0,sizeof(ACL_T));
    if(0 != *meter_id)
    {
        acl.meterIdx = *meter_id;
        acl.flags |= ACL_METER_ON;
    }
    acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
    acl.mRuleId = buf->rule_id;
    if(0 == buf->action)
    {
        acl.eAclAction = ACL_ACTION_PASS;
    }
    else
    {
        acl.eAclAction = ACL_ACTION_DROP;
    }
    if(0 != acl_pri_setting)/*ACL的优先级*/
    {
        acl.priority = buf->rule_id;
    }
    if(0 == buf->ip_type)
    {
        acl.tAclRule.tAclKey.ethType = 0x0800;
        acl.tAclRule.tAclKey.sip = buf->ip_src[3];
        acl.tAclRule.tAclKey.dip = buf->ip_dst[3];
        acl.tAclRule.tAclMask.sip = (NBB_ULONG)(0xffffffff << (32 - (buf->srcip_mask_len)));
        acl.tAclRule.tAclMask.dip = (NBB_ULONG)(0xffffffff << (32 - (buf->dstip_mask_len)));
        acl.tAclRule.tAclMask.ethType = 0xffff;
    }
    else
    {
        acl.tAclRule.tAclKey.ethType = 0x86dd;
        
        //coverity[bad_sizeof]  
        NBB_MEMCPY(acl.tAclRule.tAclKey.sipv6,&(buf->ip_src[0]), ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        NBB_MEMCPY(acl.tAclRule.tAclKey.dipv6,&(buf->ip_dst[0]), ATG_DCI_IPV6_LEN * sizeof(NBB_ULONG));
        spm_get_ipv6_mask(buf->srcip_mask_len, (NBB_ULONG*)&(acl.tAclRule.tAclMask.sipv6) NBB_CCXT);
        spm_get_ipv6_mask(buf->dstip_mask_len, (NBB_ULONG*)&(acl.tAclRule.tAclMask.dipv6) NBB_CCXT);
        acl.tAclRule.tAclMask.ethType = 0xffff; 
    }
    acl.tAclRule.tAclKey.l3Protocol = buf->l3protoco;
    acl.tAclRule.tAclKey.l4SrcPort = buf->port_src;
    acl.tAclRule.tAclKey.l4DstPort = buf->port_dst;
    acl.tAclRule.tAclMask.l3Protocol = buf->l3protoco_mask;
    acl.tAclRule.tAclMask.l4SrcPort = buf->port_src_mask;
    acl.tAclRule.tAclMask.l4DstPort = buf->port_dst_mask;
    acl.phb_cos = buf->queue_prio;
    acl.flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, &acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);

           /***************************************************************************
            *如果meter_id是本次新申请的，那么当自定义策略acl写入硬件失败后，需要将此次*
            *申请的meter_id释放。若meter_id不是本次新申请的，即使acl写入失败也不必释放*
            ***************************************************************************/
            if(0 != meterFlag)
            {
                ret = spm_qos_del_meter(meter_id NBB_CCXT);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS DEL METER FAIL**%s,%d, meter_id=%d spm_qos_del_meter ERROR=%d,\n",
                              __FUNCTION__,__LINE__,*meter_id,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            					   "QOS spm_qos_del_meter",ret,
            					   "","","unit","meter_id",0,0,unit,*meter_id));
                    OS_SPRINTF(ucMessage,"%s %s,%d : ret=%d,QOS spm_qos_del_meter.\n"
                               "unit=%d,meter_id=%d.\n\n",QOS_CFG_STRING,
                               __FUNCTION__,__LINE__,ret,*meter_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
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
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_del_usr_define_cfg(ATG_DCI_DEFEND_POLICY_USER_DF *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_USER_DEF_CB *pstAp = NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG acl_id = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_defend_del_usr_define_cfg");

    if(NULL == buf)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d nusr policy buf==NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS nusr policy buf = NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index","","","",pcb->policy_key,0,0,0));
        goto EXIT_LABEL;
    }

    acl_id = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
    pstAp = AVLL_FIND(pcb->user_def_tree, buf);
    if(NULL == pstAp)
    {
        OS_SPRINTF(ucMessage,"%s,%d : QOS CAN'T FIND USER_DEF_TREE'S NODE, CAN'T DEL. JUST PRINT THE CONFIGURATION.\n"
                   "policy_index = %d, rule_id = %d.\n\n",__FUNCTION__,__LINE__,pcb->policy_key,buf->rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }       
    else
    {
        ret = spm_qos_del_meter(&(pstAp->meter_id) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_defend_del_acl(acl_id,pstAp->user_def_key.rule_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(pcb->user_def_tree, pstAp->spm_defend_user_def_node);
        NBB_MM_FREE(pstAp,MEM_SPM_DEFEND_USR_DEF_CB);
        pstAp = NULL;
    }  

    /************不需要反刷*******************************/ 
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_add_usr_define_cfg(ATG_DCI_DEFEND_POLICY_USER_DF *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG instanceFlag = ATG_DCI_RC_OK;
    NBB_LONG usr_acl_id = 0;
    ACL_T acl = {0};
    SPM_QOS_DEFEND_USER_DEF_CB *pstUsr  = NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG meter_id = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_defend_add_usr_define_cfg");

    if(NULL == buf)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d usr policy buf = NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS usr policy buf = NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index","","","",pcb->policy_key,0,0,0));
        OS_SPRINTF(ucMessage,"%s %s,%d policy index=%d.\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,pcb->policy_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    /*判断防攻击策略是否被该slot绑定*/
    instanceFlag = spm_defend_check_instance(pcb NBB_CCXT);
    usr_acl_id = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
    pstUsr = AVLL_FIND(pcb->user_def_tree, buf);
    if(NULL == pstUsr)
    {
        /*申请meter_id,同时将acl规则写入硬件中*/
        ret = spm_defend_add_usr_acl(buf,pcb,&meter_id NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        if(/*(ATG_DCI_RC_OK == instanceFlag) && */(pcb->usr_pri != buf->prio))
        {
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotUserAcl(unit,usr_acl_id,buf->prio);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotUserAcl usr_acl_id=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,usr_acl_id,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotUserAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   usr_acl_id,buf->prio,buf->queue_prio,pcb->policy_key));
                }
#endif
            }
        }
        pstUsr = (SPM_QOS_DEFEND_USER_DEF_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_DEFEND_USER_DEF_CB),
                  NBB_NORETRY_ACT, MEM_SPM_DEFEND_USR_DEF_CB);
        AVLL_INIT_NODE(pstUsr->spm_defend_user_def_node);
        
        //coverity[bad_sizeof]  
        NBB_MEMCPY(&(pstUsr->user_def_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_USER_DF));
        pstUsr->meter_id = meter_id;
        pcb->usr_pri = buf->prio;
        pcb->usr_que_pri = buf->queue_prio;
        
        //coverity[no_effect_test]
        AVLL_INSERT(pcb->user_def_tree, pstUsr->spm_defend_user_def_node);
    }       
    else
    {
        rv = NBB_MEMCMP(&(pstUsr->user_def_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_USER_DF));
        if(0 != rv)
        {
            if(pcb->usr_pri != buf->prio)
            {
                pcb->usr_pri = buf->prio;
                
                /*if(ATG_DCI_RC_OK == instanceFlag)*/
                {
                    for(unit = 0;unit < SHARED.c3_num;unit++)
                    {
#if defined (SPU) || defined (PTN690_CES)

                        //coverity[dead_error_condition]
                        ret = ApiC3SetSlotUserAcl(unit,usr_acl_id,buf->prio);
                        if(ATG_DCI_RC_OK != ret)
                        {
                            printf("**QOS ERROR**,%s,%d ApiC3SetSlotUserAcl usr_acl_id=%d,ret=%d\n",
                                      __FUNCTION__,__LINE__,usr_acl_id,ret);
                            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                            			   "QOS ApiC3SetSlotUserAcl",ret,
                            			   "acl id","pri","queue_prio","defend policy index",
                            			   usr_acl_id,buf->prio,buf->queue_prio,pcb->policy_key));
                        }
#endif
                    }
                }
            }
            ret = spm_defend_add_usr_acl(buf,pcb,&(pstUsr->meter_id) NBB_CCXT);
            if(ATG_DCI_RC_OK != ret)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                printf("**DEFEND UPDATE FUNCTION FAILED**,%s,%d\n",__FUNCTION__,__LINE__);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					       "DEFEND UPDATE FUNCTION FAILED",ATG_DCI_RC_UNSUCCESSFUL,
    					       "policy index","rule_id","prio","queue_prio",
    					       pcb->policy_key,buf->rule_id,buf->prio,buf->queue_prio));
                goto EXIT_LABEL;
            }
            
            //coverity[bad_sizeof]  
            NBB_MEMCPY(&(pstUsr->user_def_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_USER_DF));
        }
    }  

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;   
}

 /*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_usr_define(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_USER_DF *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    SPM_QOS_DEFEND_USER_DEF_CB *pstUsr = NULL;

    NBB_TRC_ENTRY("spm_defend_set_usr_define");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = spm_defend_del_usr_define_cfg(buf,pcb NBB_CCXT);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
        ret = spm_defend_add_usr_define_cfg(buf,pcb NBB_CCXT);
        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_totalpkt(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_TOTAL_PKT *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_BYTE slot = 0;
    
    NBB_TRC_ENTRY("spm_defend_set_totalpkt");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
            
        //ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*消息内容为空*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*消息内容不为空*/
        else
        {
        
            /*调用驱动配置*/
            slot = SHARED.local_slot_id;
            if(ATG_DCI_RC_OK != pcb->defend_slot[slot])
            {
#ifndef SNU
                ret = ApiAradSetOFPBandWidth(0,QOS_RCU_PORT,buf->pir);
#endif
                if(0 != ret)
                {
                    printf("%s %d set rcu port bandwidth=%d EROOR\n",__FUNCTION__,__LINE__,buf->pir);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					           "QOS set rcu port bandwidth",ret,
    					           "bandwidth","","","",buf->pir,0,0,0));
                    goto EXIT_LABEL;
                }
            }
            
            //coverity[bad_sizeof]  
            NBB_MEMCPY(&(pcb->total_pkt),buf,sizeof(ATG_DCI_DEFEND_POLICY_TOTAL_PKT));
        }
        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_abnormal(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_ABNORMAL *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};
    NBB_ULONG rule_id = DEFEND_ABNORMAL_RULE_OFFSET;
    NBB_USHORT i = 0;

    NBB_TRC_ENTRY("spm_defend_set_abnormal");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

        /*开关已经关闭*/
        if(0 == pcb->abnormal_switch.switch_flag)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
            /* wangheng 2013-11-29 */
            for(i = DEFEND_ABNORMAL_RULE_OFFSET;i < DEFEND_ABNORMAL_RULE_OFFSET + DEFEND_ABNORMAL_RULE_NUM;i++)
            {
                acl.mRuleId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
                acl.mRuleId = i;
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                rv = ApiC3DelAcl(unit,&acl);
                if(ATG_DCI_RC_OK != rv)
                {
                    spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                    ret = rv;
                } 
#endif
            }
        }
        NBB_MEMSET(&(pcb->abnormal_switch),0,sizeof(ATG_DCI_DEFEND_POLICY_ABNORMAL));
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*消息内容为空*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*消息内容不为空*/
        else
        {
            /****************************************************/
            /*广播地址或子网广播地址的ICMP echo request报文；*/
            /****************************************************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x0800;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 1;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.icmpType = 8;
            acl.tAclRule.tAclMask.icmpType = 0xff;
            acl.tAclRule.tAclKey.dip = 0xffffffff;
            acl.tAclRule.tAclMask.dip = 0xffffffff;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    }  
#endif
                }
            }

            /****************TCP标志位非法**********************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x0800;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 6;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.tcp_flag = 0x3f;
            acl.tAclRule.tAclMask.tcp_flag = 0xff;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
            }

            /****************TCP标志位非法**********************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x0800;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 6;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.tcp_flag = 0;
            acl.tAclRule.tAclMask.tcp_flag = 0xff;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    }  
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
            }

            /****************TCP标志位非法**********************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x0800;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 6;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.tcp_flag = 3;
            acl.tAclRule.tAclMask.tcp_flag = 0xff;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)      

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    }  
#endif
                }
            }  
        }
        if(ATG_DCI_RC_OK == ret)
        {
            //coverity[bad_sizeof]  
            NBB_MEMCPY(&(pcb->abnormal_switch),buf,sizeof(ATG_DCI_DEFEND_POLICY_ABNORMAL));
        }
        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_fragment(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_FRAGMENT *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};

    NBB_TRC_ENTRY("spm_defend_set_fragment");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
            NBB_MEMSET(&(pcb->fragment_switch),0,sizeof(ATG_DCI_DEFEND_POLICY_FRAGMENT));
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*消息内容为空*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*消息内容不为空*/
        else
        {
            //coverity[bad_sizeof]  
            NBB_MEMCPY(&(pcb->fragment_switch),buf,sizeof(ATG_DCI_DEFEND_POLICY_FRAGMENT));
        }
        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  



/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_udp(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_UDP_PKT *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};
    NBB_ULONG rule_id = DEFEND_UDP_RULE_OFFSET;
    NBB_USHORT i = 0;

    NBB_TRC_ENTRY("spm_defend_set_udp");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
        if(0 == pcb->udp_swtich.switch_flag)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
            for(i = DEFEND_UDP_RULE_OFFSET;i < DEFEND_UDP_RULE_OFFSET + DEFEND_UDP_RULE_NUM;i++)
            {
                acl.mRuleId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
                acl.mRuleId = i;
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                rv = ApiC3DelAcl(unit,&acl);
                if(ATG_DCI_RC_OK != rv)
                {
                    spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                    ret = rv;
                }  
#endif
            }
        }
        if(ATG_DCI_RC_OK == ret)
        {
            NBB_MEMSET(&(pcb->udp_swtich),0,sizeof(ATG_DCI_DEFEND_POLICY_UDP_PKT));
        }
        
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        else
        {
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x0800;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 0x11;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.l4DstPort = 7;
            acl.tAclRule.tAclMask.l4DstPort = 0XFF;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
            }

            /*************************************************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x0800;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 0x11;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.l4DstPort = 13;
            acl.tAclRule.tAclMask.l4DstPort = 0XFF;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    }  
#endif
                }
            }

            /*************************************************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x0800;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 0x11;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.l4DstPort = 19;
            acl.tAclRule.tAclMask.l4DstPort = 0XFF;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
            }

            /*************************************************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x86dd;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 0x11;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.l4DstPort = 7;
            acl.tAclRule.tAclMask.l4DstPort = 0XFF;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    }  
#endif
                }
            }

            /**********************IPV6***********************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x86dd;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 0x11;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.l4DstPort = 13;
            acl.tAclRule.tAclMask.l4DstPort = 0XFF;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    }   
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
            }

            /**********************IPV6***********************/
            OS_MEMSET(&acl,0,sizeof(ACL_T));
            acl.mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = rule_id++;
            acl.eAclAction = ACL_ACTION_DROP;
            acl.tAclRule.tAclKey.ethType = 0x86dd;
            acl.tAclRule.tAclMask.ethType = 0xffff;
            acl.tAclRule.tAclKey.l3Protocol = 0x11;
            acl.tAclRule.tAclMask.l3Protocol = 0xff;
            acl.tAclRule.tAclKey.l4DstPort = 19;
            acl.tAclRule.tAclMask.l4DstPort = 0XFF;

            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
                if(0 != buf->switch_flag)
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv = ApiC3SetAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv)
                    {
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
                else
                {
#if defined (SPU) || defined (PTN690_CES)

                    //coverity[dead_error_condition]
                    rv  = ApiC3DelAcl(unit, &acl);
                    if(ATG_DCI_RC_OK != rv )
                    {
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv NBB_CCXT);
                        ret = rv;
                    } 
#endif
                }
            }

            if(ATG_DCI_RC_OK == ret)
            {
                //coverity[bad_sizeof]  
                NBB_MEMCPY(&(pcb->udp_swtich),buf,sizeof(ATG_DCI_DEFEND_POLICY_UDP_PKT));
            }
        }
        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  



/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_defend_set_tcpsys(NBB_ULONG oper,ATG_DCI_DEFEND_POLICY_TCPSYS *buf,
    SPM_QOS_DEFEND_POLICY_CB *pcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};

    NBB_TRC_ENTRY("spm_defend_set_tcpsys");

    if(NULL == pcb)
    {
        printf("QOS ERROR pcb==null\n");
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
            ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
            NBB_MEMSET(&(pcb->tcp_switch),0,sizeof(ATG_DCI_DEFEND_POLICY_TCPSYS));
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*消息内容为空*/
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*消息内容不为空*/
        else
        {
            //coverity[bad_sizeof]  
            NBB_MEMCPY(&(pcb->tcp_switch),buf,sizeof(ATG_DCI_DEFEND_POLICY_TCPSYS));
        }

        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}  


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_del_defend_driver(SPM_QOS_DEFEND_POLICY_CB *pstPol NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_INT rv = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_BYTE slot = 0;
    SPM_QOS_DEFEND_APPERC_CB *pcb = NULL;
    SPM_QOS_DEFEND_USER_DEF_CB *npb = NULL;
    ACL_T acl = {0};
    NBB_ULONG apr_acl = 0;
    NBB_ULONG usr_acl = 0;
    NBB_USHORT i = 0;

	NBB_TRC_ENTRY("spm_del_defend_driver");

    if(NULL == pstPol)
    {
        printf("**QOS ERROR**%s,%d cfg == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS SPM_QOS_DEFEND_POLICY_CB = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    slot = SHARED.local_slot_id;
    
    /*if(ATG_DCI_RC_OK != pstPol->defend_slot[slot])
    {
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }*/

    apr_acl = (pstPol->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    usr_acl = (pstPol->policy_key) + DEFEND_ACL_ID_USR_OFFSET;  

    /*************删除udp abnormal开关创建的ACL***************/
    for(unit = 0;(unit < SHARED.c3_num) && (0 != pstPol->abnormal_switch.switch_flag);unit++)
    {
        for(i = DEFEND_ABNORMAL_RULE_OFFSET;i < DEFEND_ABNORMAL_RULE_OFFSET + DEFEND_ABNORMAL_RULE_NUM;i++)
        {
            acl.mAclId = (pstPol->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = i;
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelAcl(unit,&acl);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                rv = ret;
            }  
#endif
        }
    }
    for(unit = 0;(unit < SHARED.c3_num) && (0 != pstPol->udp_swtich.switch_flag);unit++)
    {
        for(i = DEFEND_UDP_RULE_OFFSET;i < DEFEND_UDP_RULE_OFFSET + DEFEND_UDP_RULE_NUM;i++)
        {
            acl.mAclId = (pstPol->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = i;
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelAcl(unit,&acl);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                rv = ret;
            }  
#endif
        }
    }

    /******************删除METER******************************/
    for (pcb = (SPM_QOS_DEFEND_APPERC_CB*)AVLL_FIRST(pstPol->apperc_tree); pcb != NULL;
         pcb = (SPM_QOS_DEFEND_APPERC_CB*)AVLL_NEXT(pstPol->apperc_tree, pcb->spm_defend_apperc_node))
    {
        if(0 != pcb->meter_id)
        {
            for(unit = 0; unit < SHARED.c3_num; unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3DelMeter(unit,pcb->meter_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_del_meter_error_log(unit,pcb->meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3DelMeter",ret,
                    			   "l3_protoc","pri","queue_prio","defend policy index",
                    			   pcb->apperc_key.l3_protoc,pstPol->apper_pri,
                    			   pcb->apperc_key.queue_prio,pstPol->policy_key));
                    rv = ret;
                } 
                else
                {
                    ret = spm_qos_free_meter_id(&(pcb->meter_id) NBB_CCXT);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_qos_free_meter_error_log(&(pcb->meter_id),__FUNCTION__,__LINE__,ret NBB_CCXT);
                        rv = ret;
                    }
                }
#endif
            }
        }

        ret = spm_defend_del_apper_acl(apr_acl,pcb->apperc_key.l3_protoc NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }


/**********************************新用户自定义************************************/
    for (npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pstPol->user_def_tree); npb != NULL;
         npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_NEXT(pstPol->user_def_tree,npb->spm_defend_user_def_node))
    {
        if(0 != npb->meter_id)
        {
            for(unit = 0;unit < SHARED.c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3DelMeter(unit,npb->meter_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_api_c3_del_meter_error_log(unit,npb->meter_id,__FUNCTION__,__LINE__,ret NBB_CCXT);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3DelMeter",ret,
                    			   "rule_id","pri","queue_prio","defend policy index",
                    			   npb->user_def_key.rule_id,pstPol->usr_pri,
                    			   npb->user_def_key.queue_prio,pstPol->policy_key));
                    rv = ret;
                }  
                else
                {
                    ret = spm_qos_free_meter_id(&(npb->meter_id) NBB_CCXT);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_qos_free_meter_error_log(&(npb->meter_id),__FUNCTION__,__LINE__,ret NBB_CCXT);
                        rv = ret;
                    }
                }
#endif
            }
        }
        
        acl.mAclId = usr_acl;
        acl.mRuleId = npb->user_def_key.rule_id;
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelAcl(unit,&acl);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret NBB_CCXT);
                rv = ret;
            }  
#endif
        }
    }

/***************************白名单*************************************/
    if(0 != pstPol->white_meter)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotWhiteListMeter(unit,pstPol->white_meter,0);  
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotWhiteListMeter meter id=%d,ret=%d\n",
                          __FUNCTION__,__LINE__,pstPol->white_meter,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotWhiteListMeter",ret,
                			   "meter id","white meter","disable","defend policy index",
                			   pstPol->white_meter,0,0,pstPol->policy_key));
                rv = ret;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelMeter(unit,pstPol->white_meter);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_meter_error_log(unit,pstPol->white_meter,__FUNCTION__,__LINE__,ret NBB_CCXT);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3DelMeter",ret,
                			   "meter id","white meter","","defend policy index",
                			   pstPol->white_meter,0,0,pstPol->policy_key));
                rv = ret;
            }   
            else
            {
                ret = spm_qos_free_meter_id(&(pstPol->white_meter) NBB_CCXT);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_qos_free_meter_error_log(&(pstPol->white_meter),__FUNCTION__,__LINE__,ret NBB_CCXT);
                    rv = ret;
                }
            }
#endif
        }
    }
    if(0 != pstPol->black_meter)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotBlackListMeter(unit,pstPol->black_meter,0); 
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotBlackListMeter meter id=%d,ret=%d\n",
                          __FUNCTION__,__LINE__,pstPol->black_meter,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotBlackListMeter",ret,
                			   "meter id","black meter","disable","defend policy index",
                			   pstPol->black_meter,0,0,pstPol->policy_key));
                rv = ret;
            }
#endif 
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelMeter(unit,pstPol->black_meter);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_meter_error_log(unit,pstPol->black_meter,__FUNCTION__,__LINE__,ret NBB_CCXT);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3DelMeter",ret,
                			   "meter id","black meter","","defend policy index",
                			   pstPol->black_meter,0,0,pstPol->policy_key));
                rv = ret;
            }
            else
            {
                ret = spm_qos_free_meter_id(&(pstPol->black_meter) NBB_CCXT);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_qos_free_meter_error_log(&(pstPol->black_meter),__FUNCTION__,__LINE__,ret NBB_CCXT);
                    rv = ret;
                }
            }
#endif
        }
    }
    pstPol->defend_slot[slot] = ATG_DCI_RC_OK;

    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}

/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_VOID spm_rcv_dci_set_defend_policy(ATG_DCI_SET_DEFEND_POLICY* pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_QOS_DEFEND_POLICY_CB *pstCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT i = 0;

    NBB_BYTE *pucApperSwitchDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE *pucWhiteDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE *pucBlackDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE *pucApperceiveDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE *pucNUserDefDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE *pucTotlPktDataStart = ATG_DCI_OPER_NULL;   
    NBB_BYTE *pucAbnormalDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE *pucFragmentDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE *pucUdpPktDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE *pucTcpsysDataStart = ATG_DCI_OPER_NULL;

    NBB_ULONG ulOperApperSwitch = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperWhite = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperBlack = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperApperceive = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperNUserDef = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperTotlPkt = ATG_DCI_OPER_NULL;   
    NBB_ULONG ulOperAbnormal = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperFragment = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperUdpPkt = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperTcpsys = ATG_DCI_OPER_NULL;

    ATG_DCI_DEFEND_POLICY_APPERC_S *pstApperSwitchData = NULL;
    ATG_DCI_DEFEND_POLICY_WHITE *pstWhiteData = NULL;    
    ATG_DCI_DEFEND_POLICY_BLACK *pstBlackData = NULL;
    ATG_DCI_DEFEND_POLICY_APPERC_P *pstApperceiveData = NULL;
    ATG_DCI_DEFEND_POLICY_USER_DF *pstNUserDefData = NULL;
    ATG_DCI_DEFEND_POLICY_TOTAL_PKT *pstTotlPktData = NULL;
    ATG_DCI_DEFEND_POLICY_ABNORMAL *pstAbnormalData = NULL;
    ATG_DCI_DEFEND_POLICY_FRAGMENT *pstFragmentData = NULL;
    ATG_DCI_DEFEND_POLICY_UDP_PKT *pstUdpPktData = NULL;
    ATG_DCI_DEFEND_POLICY_TCPSYS *pstTcpsysData = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }

    pstSetIps->return_code = ATG_DCI_RC_OK;
    ulkey = pstSetIps->key;
    pstCb = AVLL_FIND(SHARED.qos_defend_policy_tree, &ulkey);
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }

    ulOperApperSwitch = pstSetIps->oper_apperceive_s;
    ulOperWhite = pstSetIps->oper_white;
    ulOperBlack = pstSetIps->oper_black;
    ulOperApperceive = pstSetIps->oper_apperceive_p;
    ulOperNUserDef = pstSetIps->oper_user_define;
    ulOperTotlPkt = pstSetIps->oper_total_packet;
    ulOperAbnormal = pstSetIps->oper_abnormal_packet;
    ulOperFragment = pstSetIps->oper_fragment_flood;
    ulOperUdpPkt = pstSetIps->oper_udp_packet;
    ulOperTcpsys = pstSetIps->oper_tcpsys_flood;

    pucApperSwitchDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->apperceive_s_data);
    pucWhiteDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->white_data);
    pucBlackDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->black_data);
    pucApperceiveDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->apperceive_p_data);
    pucNUserDefDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->user_define_data);
    pucTotlPktDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->total_packet_data);
    pucAbnormalDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->abnormal_packet_data);
    pucFragmentDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->fragment_flood_data);
    pucUdpPktDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->udp_packet_data);
    pucTcpsysDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->tcpsys_flood_data);

    pstApperSwitchData = (ATG_DCI_DEFEND_POLICY_APPERC_S*)pucApperSwitchDataStart;
    pstWhiteData = (ATG_DCI_DEFEND_POLICY_WHITE*)pucWhiteDataStart;    
    pstBlackData = (ATG_DCI_DEFEND_POLICY_BLACK*)pucBlackDataStart;
    pstApperceiveData = (ATG_DCI_DEFEND_POLICY_APPERC_P*)pucApperceiveDataStart;
    pstNUserDefData = (ATG_DCI_DEFEND_POLICY_USER_DF*)pucNUserDefDataStart;
    pstTotlPktData = (ATG_DCI_DEFEND_POLICY_TOTAL_PKT*)pucTotlPktDataStart;
    pstAbnormalData = (ATG_DCI_DEFEND_POLICY_ABNORMAL*)pucAbnormalDataStart;
    pstFragmentData = (ATG_DCI_DEFEND_POLICY_FRAGMENT*)pucFragmentDataStart;
    pstUdpPktData = (ATG_DCI_DEFEND_POLICY_UDP_PKT*)pucUdpPktDataStart;
    pstTcpsysData = (ATG_DCI_DEFEND_POLICY_TCPSYS*)pucTcpsysDataStart;

    /* ips消息为删除 */
    if (TRUE == pstSetIps->delete_struct)
    {
        if (ucIfExist == QOS_UNEXIST)
        {
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
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

            /* 被引用 */
            /*if (0 != pstCb->cnt)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }*/
            spm_del_defend_driver(pstCb NBB_CCXT);
            AVLL_DELETE(SHARED.qos_defend_policy_tree, pstCb->spm_defend_policy_node);

            /* 释放POLICY表分配的内存空间 */
            ret = spm_free_defend_policy_cb(pstCb NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                pstSetIps->return_code = ATG_DCI_RC_ALLOC_ERROR;
                goto EXIT_LABEL;
            }
        }
    }

    /*增加或者更新*/
    else
    {
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == QOS_UNEXIST)
        {
            /* 申请一个新条目的内存空间 */
            pstCb = spm_alloc_defend_policy_cb(ulkey NBB_CCXT);
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }
        if(NULL == pstCb)
        {
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        
        /*if (0 != pstCb->cnt) 
        {
            pstSetIps->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }*/
        ret = spm_defend_set_apperswitch(ulOperApperSwitch,pstApperSwitchData,pstCb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->apperceive_s_return_code = ret;
        }
        ret = spm_defend_set_white(ulOperWhite,pstWhiteData,pstCb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->white_return_code = ret;
        }
        ret = spm_defend_set_black(ulOperBlack,pstBlackData,pstCb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->black_return_code = ret;
        }
        for( i = 0;i < pstSetIps->apperceive_p_num;i++)
        {
            if(NULL != pstApperceiveData)
            {
                pstApperceiveData = (ATG_DCI_DEFEND_POLICY_APPERC_P *)
                    (pucApperceiveDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_DEFEND_POLICY_APPERC_P))) * i);
                ret = spm_defend_set_apperceive(ulOperApperceive,pstApperceiveData,pstCb NBB_CCXT);
                if(ATG_DCI_RC_OK != ret)
                {
                    pstSetIps->return_code = ret;
                    pstSetIps->apperceive_p_return_code[i] = ret;
                }
            }
        }

        for( i = 0;i < pstSetIps->user_define_num;i++)
        {
            if(NULL != pstNUserDefData)
            {
                pstNUserDefData = (ATG_DCI_DEFEND_POLICY_USER_DF *)
                    (pucNUserDefDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_DEFEND_POLICY_USER_DF))) * i);
                ret = spm_defend_set_usr_define(ulOperNUserDef,pstNUserDefData,pstCb NBB_CCXT);
                if(ATG_DCI_RC_OK != ret)
                {
                    pstSetIps->return_code = ret;
                    pstSetIps->user_define_return_code = ret;
                }
            }
        }
        ret = spm_defend_set_totalpkt(ulOperTotlPkt,pstTotlPktData,pstCb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->total_packet_return_code = ret;
        }
        ret = spm_defend_set_abnormal(ulOperAbnormal,pstAbnormalData,pstCb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->abnormal_packet_return_code = ret;
        }
        ret = spm_defend_set_fragment(ulOperFragment,pstFragmentData,pstCb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->fragment_flood_return_code = ret;
        }
        ret = spm_defend_set_udp(ulOperUdpPkt,pstUdpPktData,pstCb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->udp_packet_return_code = ret;
        }
        ret = spm_defend_set_tcpsys(ulOperTcpsys,pstTcpsysData,pstCb NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->tcpsys_flood_return_code = ret;
        }

        /* 如果是新增的条目，插入到树中 */
        if (QOS_UNEXIST == ucIfExist)
        {
            //coverity[no_effect_test]
            AVLL_INSERT(SHARED.qos_defend_policy_tree, pstCb->spm_defend_policy_node);
        }
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_VOID spm_qos_clear_all_defend_policy(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_DEFEND_POLICY_CB *cfg_cb = NULL;


    for (cfg_cb = (SPM_QOS_DEFEND_POLICY_CB*) AVLL_FIRST(v_spm_shared->qos_defend_policy_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DEFEND_POLICY_CB*) AVLL_FIRST(v_spm_shared->qos_defend_policy_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_defend_policy_tree, cfg_cb->spm_defend_policy_node);
        spm_free_defend_policy_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_set_slot_policy_cfg(NBB_ULONG slot,NBB_ULONG policy_index NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_POLICY_CB *pstPol = NULL;
    NBB_ULONG appre_acl_id = 0;
    NBB_ULONG usr_acl_id = 0;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(0 == policy_index) /*不处理为0的直接存储*/
    {
        goto EXIT_LABEL;
    }
    pstPol = AVLL_FIND(SHARED.qos_defend_policy_tree,&policy_index);
    if(NULL == pstPol)
    {
        printf("**QOS ERROR**,%s,%d  cant't find policy index=%d\n",
                  __FUNCTION__,__LINE__,policy_index);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			   "QOS  cant't find defend policy index",ATG_DCI_RC_UNSUCCESSFUL,
        			   "policy index","slot","","",policy_index,slot,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    appre_acl_id = (policy_index) + DEFEND_ACL_ID_APPRE_OFFSET;
    usr_acl_id = (policy_index) + DEFEND_ACL_ID_USR_OFFSET;
    if(slot == SHARED.local_slot_id)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            if( 0 == pstPol->apperc_policy_switch.switch_flag)
            {
                //coverity[dead_error_condition]
                ret = ApiC3SetSlotRelativeAclEnable(unit,1);
            }
            else
            {
                //coverity[dead_error_condition]
                ret = ApiC3SetSlotRelativeAclEnable(unit,0);    
            }
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotRelativeAclEnable ret=%d\n",
                          __FUNCTION__,__LINE__,pstPol->white_policy.acl_index,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotRelativeAclEnable",ret,
                			   "","","","defend policy index",
                			   0,0,0,pstPol->policy_key));
                rv += ret;
            } 
#endif
            if(0 != pstPol->apper_pri)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotRelativeAcl(unit,appre_acl_id,pstPol->apper_pri);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotRelativeAcl acl=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,usr_acl_id,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotRelativeAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   appre_acl_id,pstPol->apper_pri,
                    			   pstPol->apper_que_pri,policy_index));
                    rv += ret;
                }
#endif
            }  
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotWhiteAclEnable(unit,1);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotWhiteAclEnable enable,ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotWhiteAclEnable",ret,
                			   "enable","unit","","",
                			   0,unit,0,0));
                rv += ret;
            }
#endif
            if((0 != pstPol->white_policy.prio) && (0 != pstPol->white_policy.queue_prio))
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotWhiteAcl(unit,pstPol->white_policy.acl_index,
                      pstPol->white_policy.prio,pstPol->white_policy.queue_prio);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotWhiteAcl acl=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,pstPol->white_policy.acl_index,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotWhiteAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   pstPol->white_policy.acl_index,pstPol->white_policy.prio,
                    			   pstPol->white_policy.queue_prio,policy_index));
                    rv += ret;
                }
#endif
            }
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotBlackAclEnable(unit,1);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotBlackAclEnable enable,ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotBlackAclEnable",ret,
                			   "enable","unit","","",0,unit,0,0));
                rv += ret;
            }
#endif
            if((0 != pstPol->black_policy.prio) && (0 != pstPol->black_policy.queue_prio))
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotBlackAcl(unit,pstPol->black_policy.acl_index,
                      pstPol->black_policy.prio,pstPol->black_policy.queue_prio);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotBlackAcl acl=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,pstPol->black_policy.acl_index,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotBlackAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   pstPol->black_policy.acl_index,pstPol->black_policy.prio,
                    			   pstPol->black_policy.queue_prio,policy_index));
                    rv += ret;
                }
#endif
            }
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotUserAclEnable(unit,1);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotUserAclEnable enable,ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotUserAclEnable",ret,
                			   "enable","unit","","",0,unit,0,0));
                rv += ret;
            }
#endif
            if(0 != pstPol->usr_pri)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotUserAcl(unit,usr_acl_id,pstPol->usr_pri);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotUserAcl acl=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,usr_acl_id,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotUserAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   usr_acl_id,pstPol->usr_pri,
                    			   pstPol->usr_que_pri,policy_index));
                    rv += ret;
                }  
#endif
            }
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotSccAclEnable(unit,1);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotSccAclEnable enable,ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotSccAclEnable",ret,
                			   "enable","unit","","",0,unit,0,0));
                rv += ret;
            }
#endif
        }
        if(ATG_DCI_RC_OK == rv)
        {
            pstPol->defend_slot[slot] = ATG_DCI_RC_UNSUCCESSFUL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;   
}



/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_del_slot_driver(SPM_QOS_DEFEND_POLICY_CB *pstPol NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_INT rv = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_BYTE slot = 0;
    NBB_ULONG apr_acl = 0;
    NBB_ULONG usr_acl = 0;

	NBB_TRC_ENTRY("spm_del_slot_driver");

    if(NULL == pstPol)
    {
        printf("**QOS ERROR**%s,%d cfg == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS SPM_QOS_DEFEND_POLICY_CB = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    apr_acl = (pstPol->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    usr_acl = (pstPol->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
    slot = SHARED.local_slot_id;
    if(ATG_DCI_RC_OK != pstPol->defend_slot[slot])
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
           ret = ApiC3SetSlotRelativeAclEnable(unit,0);

        /* 在disable时张正辉已将所有联动相关的数据全部清除,无需再调用下面的函数,否则执行结果会产生错误 */
        /* ret = ApiC3SetSlotRelativeAcl(unit,0,pstPol->apper_pri); */
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS CLEAR ERROR**,%s,%d : ApiC3SetSlotRelativeAcl acl_id=0 ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotRelativeAcl",ret,
                			   "","acl_id","apper_pri","defend policy index",
                			   0,0,pstPol->apper_pri,pstPol->policy_key));
                rv += ret;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotWhiteAclEnable(unit,0);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS CLEAR ERROR**,%s,%d : ApiC3SetSlotWhiteAcl acl_id=0 ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotWhiteAclEnable",ret,
                			   "acl_id","prio","queue_prio","defend policy index",
                			   0,pstPol->white_policy.prio,pstPol->white_policy.queue_prio,pstPol->policy_key));
                rv += ret;
            }
#endif
           
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotBlackAclEnable(unit,0);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS CLEAR ERROR**,%s,%d : ApiC3SetSlotBlackAcl acl_id=0 ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotBlackAcl",ret,
                			   "acl_id","prio","queue_prio","defend policy index",
                			   0,pstPol->black_policy.prio,pstPol->black_policy.queue_prio,pstPol->policy_key));
                rv += ret;
            }
#endif

#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotUserAclEnable(unit,0);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS CLEAR ERROR**,%s,%d : ApiC3SetSlotUserAcl acl_id=0 ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotUserAcl",ret,
                			   "","acl_id","usr_pri","defend policy index",
                			   0,0,pstPol->usr_pri,pstPol->policy_key));
                rv += ret;
            }
#endif

#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotSccAclEnable(unit,0);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotSccAclEnable disable,ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotSccAclEnable",ret,
                			   "disable","unit","","",0,unit,0,0));
                rv += ret;
            }
#endif
        }
        pstPol->defend_slot[slot] = ATG_DCI_RC_OK;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;   
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_set_slot_defend_arp(NBB_BYTE ulkey,NBB_BYTE old_switch,
    NBB_BYTE new_switch NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_set_slot_defend_arp");

    if((ulkey == SHARED.local_slot_id) && (old_switch != new_switch))
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {		
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
	        ret = ApiC3SetSlotGuard(unit,new_switch);
	        if(ATG_DCI_RC_OK != ret)
	        {
	            printf("**QOS ERROR**,%s,%d slot=%d arpdefendswitch=%d ApiC3SetSlotGuard ret=%d\n",
	                      __FUNCTION__,__LINE__,ulkey,new_switch,ret);
	            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
	            			   "QOS ApiC3SetSlotGuard",ret,
	            			   "","old_switch","slot","arpdefendswitch",
	            			   0,old_switch,ulkey,new_switch));
			}
#endif
		}
	}
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_set_slot_defend_policy(NBB_BYTE ulkey,NBB_ULONG old_policy,
    NBB_ULONG new_policy NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_POLICY_CB *pstPol = NULL;

    NBB_TRC_ENTRY("spm_set_slot_defend_policy");

    if(ulkey == SHARED.local_slot_id)
    {
       if((0 == old_policy) && (new_policy == old_policy))/*不配置*/
       {
            goto EXIT_LABEL;
       }
       else if((0 == old_policy) && (0 != new_policy))/*增加*/
       {
            ret = spm_set_slot_policy_cfg(ulkey,new_policy NBB_CCXT);
            goto EXIT_LABEL;
       }
       else if((0 != old_policy) && (0 == new_policy))/*删除*/
       {
            pstPol = AVLL_FIND(SHARED.qos_defend_policy_tree,&old_policy);
            ret = spm_del_slot_driver(pstPol NBB_CCXT);
            goto EXIT_LABEL;
       }
       else
       {
            printf("**QOS ERROR**,%s,%d slot=%d,old_policy=%d,new_policy=%d cfg missmatch\n",
	                  __FUNCTION__,__LINE__,ulkey,old_policy,new_policy);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            			   "QOS DEFEND miss match",ret,
            			   "","old_policy","slot","new_policy",0,old_policy,ulkey,new_policy));
            ret = ATG_DCI_RC_ADD_FAILED;
            goto EXIT_LABEL;
       }
	}
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_LONG spm_set_slot_defend_cfg(NBB_ULONG oper, 
    SPM_QOS_DEFEND_CB *pcb, ATG_DCI_DEFEND_SWITCH_BASIC *buf NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY("spm_set_slot_defend_cfg");

    if((NULL == pcb) || (NULL == buf))
    {
        ret = ATG_DCI_RC_ADD_FAILED;
        goto EXIT_LABEL;
    }

    /*ips消息操作类型*/
    switch(oper)
    {
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        case  ATG_DCI_OPER_ADD:          
        rv = spm_set_slot_defend_arp(pcb->key,pcb->basic.arp_defend_switch,
             buf->arp_defend_switch  NBB_CCXT);
        if(ATG_DCI_RC_OK == rv)
        {
            pcb->basic.arp_defend_switch = buf->arp_defend_switch;
        }
        else
        {
            ret = rv;
        }
        rv = spm_set_slot_defend_policy(pcb->key,pcb->basic.defend_switch_index,
             buf->defend_switch_index  NBB_CCXT);
        if(ATG_DCI_RC_OK == rv)
        {
            pcb->basic.defend_switch_index = buf->defend_switch_index;
        }
        else
        {
            ret = rv;
        }
        break;
        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_VOID spm_rcv_dci_set_slot_defend(ATG_DCI_SET_DEFEND_SWITCH* pstSetIps NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE ucIfExist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_QOS_DEFEND_CB *pstCb = NULL;
    SPM_QOS_DEFEND_POLICY_CB *pstPol = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_BYTE *pucBasicDataStart = ATG_DCI_OPER_NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;

    ATG_DCI_DEFEND_SWITCH_BASIC *pstBasichData = NULL;

    /*逻辑处理流程*/
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pstSetIps)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }

    /* 首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL */
    pstSetIps->return_code = ATG_DCI_RC_OK;
    ulkey = pstSetIps->key;
    pstCb = AVLL_FIND(SHARED.qos_defend_tree, &ulkey);

    /* 如果条目不存在 */
    if (pstCb == NULL)
    {
        ucIfExist = QOS_UNEXIST;
    }

    ulOperBasic = pstSetIps->oper_basic;
    pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->basic_data);
    pstBasichData = (ATG_DCI_DEFEND_SWITCH_BASIC*)pucBasicDataStart;
    if((ATG_DCI_RC_OK != qos_defend_cfg_print) && (NULL != pstBasichData))
    {
        printf("%s %s,%d slot=%d defend_switch_index=%d arp_defend_switch=%d\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,ulkey,pstBasichData->defend_switch_index,
                  pstBasichData->arp_defend_switch);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d defend_switch_index=%d arp_defend_switch=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,ulkey,pstBasichData->defend_switch_index,
                   pstBasichData->arp_defend_switch);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if(TRUE == pstSetIps->delete_struct)
    {
        if (ucIfExist == QOS_UNEXIST)
        {
            pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
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

            if(0 != pstCb->basic.defend_switch_index)
            {
                pstPol = AVLL_FIND(SHARED.qos_defend_policy_tree,&(pstCb->basic.defend_switch_index));
                spm_del_slot_driver(pstPol NBB_CCXT);
            } 
            if(0 != pstCb->basic.arp_defend_switch)
            {
                for(unit = 0;unit < SHARED.c3_num;unit++)
		        {		
#if defined (SPU) || defined (PTN690_CES)
			        ApiC3SetSlotGuard(unit,0);
#endif
				}
            }
            AVLL_DELETE(SHARED.qos_defend_tree, pstCb->spm_defend_node);
            ret = spm_free_defend_cb(pstCb NBB_CCXT);
            if (ATG_DCI_RC_OK != ret)
            {
                pstSetIps->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
        }
    }
    else
    {
        if (NULL == pstCb)
        {
            pstCb = spm_alloc_defend_cb(ulkey NBB_CCXT);
            if (NULL == pstCb)
            {
                pstSetIps->return_code = ATG_DCI_RC_ADD_FAILED;
                goto EXIT_LABEL;
            }
            
            //coverity[no_effect_test]
            AVLL_INSERT(SHARED.qos_defend_tree, pstCb->spm_defend_node);      
        }
        ret = spm_set_slot_defend_cfg(ulOperBasic,pstCb,pstBasichData NBB_CCXT);      
        pstSetIps->return_code = ret;
    }

    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;    
}


/*****************************************************************************
   函 数 名  : 
   功能描述  : 
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
NBB_VOID spm_qos_clear_all_defend_switch(NBB_CXT_T NBB_CXT)
{
    SPM_QOS_DEFEND_CB *cfg_cb = NULL;


    for (cfg_cb = (SPM_QOS_DEFEND_CB*) AVLL_FIRST(v_spm_shared->qos_defend_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DEFEND_CB*) AVLL_FIRST(v_spm_shared->qos_defend_tree))
    {
        AVLL_DELETE(v_spm_shared->qos_defend_tree, cfg_cb->spm_defend_node);
        spm_free_defend_cb(cfg_cb NBB_CCXT);   
    }

    return;   
}

#ifdef R8000_V3R2

/*****************************************************************************
   函 数 名  : spm_attack_set_sampleratio
   功能描述  : 设置攻击性能统计记录开关
   输入参数  : 
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2015年8月15日 
   作    者  : tianf
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_attack_set_recordswitch(NBB_ULONG oper, AGGRESSIVITY_SUB_CFG_ONE *ptemp NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG recordswitch = 0;
    NBB_ULONG sampleratio = 0;
    NBB_BYTE unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == ptemp)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
        if(qos_defend_cfg_print != ATG_DCI_RC_OK)
        {
            printf("AttackRecordSwitch = %d,",ptemp->attack_count_switch);        
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES) 
            ret = fhdrv_psn_get_slot_attack_sampler(unit, &recordswitch, &sampleratio);
            if(ret != ATG_DCI_RC_OK)
            {
                spm_api_c3_get_slot_attact_error_log(unit,&recordswitch,&sampleratio);
                goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_psn_set_slot_attack_sampler(unit,ptemp->attack_count_switch,sampleratio);
            if(ret != ATG_DCI_RC_OK)
            {
                spm_api_c3_set_slot_attact_error_log(unit,ptemp->attack_count_switch,sampleratio,
                                __FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }
#endif
        }

        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_attack_set_sampleratio
   功能描述  : 设置攻击性能统计采样比
   输入参数  : 
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2015年8月15日 
   作    者  : tianf
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_attack_set_sampleratio(NBB_ULONG oper, AGGRESSIVITY_SUB_CFG_TWO *ptemp NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG recordswitch = 0;
    NBB_ULONG sampleratio = 0;
    NBB_BYTE unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == ptemp)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
        if(qos_defend_cfg_print != ATG_DCI_RC_OK)
        {
            printf("SampleRatio = %d.\n",ptemp->attach_sample_rate);        
        }
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_psn_get_slot_attack_sampler(unit, &recordswitch, &sampleratio);
            if(ret != ATG_DCI_RC_OK)
            {
                spm_api_c3_get_slot_attact_error_log(unit,&recordswitch,&sampleratio);
                goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_psn_set_slot_attack_sampler(unit,recordswitch,ptemp->attach_sample_rate);
            if(ret != ATG_DCI_RC_OK)
            {
                spm_api_c3_set_slot_attact_error_log(unit,recordswitch,ptemp->attach_sample_rate,
                                __FUNCTION__,__LINE__,ret NBB_CCXT);
                goto EXIT_LABEL;
            }
#endif
        }

        break;

        default:
        break;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_attack_set_alarmthreshold
   功能描述  : 设置攻击性能统计告警阈值
   输入参数  : 
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2015年8月15日 
   作    者  : tianf
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_attack_set_alarmthreshold(NBB_ULONG oper, AGGRESSIVITY_SUB_CFG_THREE *ptemp NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == ptemp)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*ips消息类型*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
        if(qos_defend_cfg_print != ATG_DCI_RC_OK)
        {
            printf("AlarmThreshold = %ld,CountTimeInterval = %d.\n",
            ptemp->alarm_threshold,ptemp->count_time_interval);        
        }
        ret = almpm_set_attack_alm_threshold(1,
                                        ptemp->alarm_threshold,
                                        ptemp->count_time_interval);
        if(ret != ATG_DCI_RC_OK)
        {
            spm_hqos_almpm_attack_error_log(1,
                                ptemp->alarm_threshold,
                                ptemp->count_time_interval,
                                __FUNCTION__,__LINE__,ret NBB_CCXT);
        }
        break;

        default:
        break;
    }


    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_rcv_dci_set_attact_cfg
   功能描述  : 设置攻击性能统计接收配置处理
   输入参数  : 
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2015年8月15日 
   作    者  : tianf
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_slot_attact(ATG_DCI_SET_AGGRESSIVITY *pstSetIps NBB_CCXT_T NBB_CXT)
{

    /***************************************************************************/
    /* Local Variables                  NBB_CCXT_T NBB_CXT                                       */
    /***************************************************************************/  
    NBB_ULONG slotId = 0;
    NBB_BYTE unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucSampleRecordDataStart = NULL;
    NBB_BYTE *pucSampleRatioDataStart = NULL;
    NBB_BYTE *pucAlarmThresholdDataStart = NULL;

    /* 无基本配置 */
    NBB_ULONG ulOperSampleRecord = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperSampleRatio = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperAlarmThreshold = ATG_DCI_OPER_NULL;

    AGGRESSIVITY_SUB_CFG_ONE *pstSampleRecord = NULL;
    AGGRESSIVITY_SUB_CFG_TWO *pstSampleRatio = NULL;
    AGGRESSIVITY_SUB_CFG_THREE *pstAlarmThreshold = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

 

    if(NULL == pstSetIps)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        spm_qos_param_error_log(__FUNCTION__,__LINE__ NBB_CCXT);
        goto EXIT_LABEL;
    }

     /*获取本槽位ID*/
    slotId = SHARED.local_slot_id;

    if(slotId != pstSetIps->key.slot_id)
    {
        goto EXIT_LABEL;   
    }

    /* 首先将IPS消息的返回值设置为OK,如果有一个子配置失败,则置为FAIL */
    pstSetIps->return_code = ATG_DCI_RC_OK;

    /* 获取配置的操作码 */
    ulOperSampleRecord = pstSetIps->oper_sub_one;
    ulOperSampleRatio = pstSetIps->oper_sub_two;
    ulOperAlarmThreshold = pstSetIps->oper_sub_three;

    /* 获取配置的偏移地址 */
    pucSampleRecordDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->sub_one_data);
    pucSampleRatioDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->sub_two_data);
    pucAlarmThresholdDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetIps, &pstSetIps->sub_three_data);


    pstSampleRecord = (AGGRESSIVITY_SUB_CFG_ONE*)pucSampleRecordDataStart;
    pstSampleRatio = (AGGRESSIVITY_SUB_CFG_TWO*)pucSampleRatioDataStart;
    pstAlarmThreshold = (AGGRESSIVITY_SUB_CFG_THREE*)pucAlarmThresholdDataStart;


    /* ips消息类型为删除 */
    if (TRUE == pstSetIps->delete_struct)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_psn_set_slot_attack_sampler(unit,0,0); 
            if(ATG_DCI_RC_OK != ret)
            {
                pstSetIps->return_code = ret;
                spm_api_c3_set_slot_attact_error_log(unit,0,0,
                                                    __FUNCTION__,__LINE__,ret NBB_CCXT);


            }
#endif
        }
        
        ret = almpm_set_attack_alm_threshold(0,0,0);
        if(ATG_DCI_RC_OK != ret)
        {
        
            pstSetIps->return_code = ret;
            spm_hqos_almpm_attack_error_log(0,0,0,
                                __FUNCTION__,__LINE__,ret NBB_CCXT);

        }
    }
    else
    {
        /*采样记录开关*/
        ret = spm_attack_set_recordswitch(ulOperSampleRecord,pstSampleRecord);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->sub_one_return_code = ret;
        }

        /*采样比*/
        ret = spm_attack_set_sampleratio(ulOperSampleRatio,pstSampleRatio);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->sub_two_return_code = ret;
        }


        /*告警阈值*/
        ret = spm_attack_set_alarmthreshold(ulOperAlarmThreshold,pstAlarmThreshold);
        if(ATG_DCI_RC_OK != ret)
        {
            pstSetIps->return_code = ret;
            pstSetIps->sub_three_return_code = ret;
        }
        
    }


    
    /* 异常跳出 */
    EXIT_LABEL:NBB_TRC_EXIT();
    return;

}

#endif

