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
NBB_BYTE g_acl_pri_set;

/* 外部打印开关全局变量 */
NBB_BYTE g_qos_defend_cfg_print;


/***************************************************************************/
/* 存放防攻击的每个协议的rule个数*/
/***************************************************************************/
NBB_BYTE g_qos_defend_num[SIGNAL_NUM + 1];

/***************************************************************************/
/* 存放防攻击的每个协议的rule偏移*/
/***************************************************************************/
NBB_USHORT g_qos_defend_offset[SIGNAL_NUM + 1];


/***************************************************************************/
/* 存放policy qos配置*/
/***************************************************************************/
AVLL_TREE g_qos_defend_policy_tree;

/***************************************************************************/
/* 存放policy qos配置*/
/***************************************************************************/
AVLL_TREE g_qos_defend_tree;

static char *defend_protcal_string[] =
{
 "空","OSPF","ISIS","BGP","LDP","LDP(UDP)","RSVP","RIP","MSDP","PIM","SNMP",
 "RADIUS","LSP PING","IGMP","ICMP","VRRP","DHCP","LACP","BFD","TACACS","NTP",
 "FTP S","FTP C","TELNET S","TELNET C","SSH S","SSH C","OSPFV3","MLD","BGPV6","RIPNG",
 "ICMPV6","VRRPV6","DHCPV6","PIMV6","UNKOWN ARP","UNKOWN MULICAST","TCP SYN","空"
 "RIPV1","RIPng","SNMP","NTP","TELNET","SSH","空","空","空","空"
};


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
NBB_VOID spm_print_ipv4(NBB_ULONG ip)
{
    if (0 == ip)
    {
        printf("ip == 0\n");
        return;
    }
    printf("%ld.%ld.%ld.%ld\n", ((ip >> 24) & 0x00ff),
            ((ip >> 16) & 0x00ff), ((ip >> 8) & 0x00ff), (ip & 0x00ff));
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
NBB_VOID spm_print_ipv6(NBB_ULONG *ip)
{
    NBB_BYTE i;

    if (NULL == ip)
    {
        printf("ip == 0\n");
        return;
    }
    for (i = 0; i < 4; i++)
    {
        printf("%ld.%ld.%ld.%ld\n", ((*ip >> 24) & 0x00ff),
                ((*ip >> 16) & 0x00ff), ((*ip >> 8) & 0x00ff), (*ip & 0x00ff));
        ip++;
    }
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
SPM_QOS_DEFEND_POLICY_CB * spm_alloc_defend_policy_cb(NBB_ULONG ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DEFEND_POLICY_CB *pst_tbl = NULL;
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
    pst_tbl = (SPM_QOS_DEFEND_POLICY_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DEFEND_POLICY_CB),
              NBB_NORETRY_ACT, MEM_SPM_DEFEND_POLICY_CB);
    if (pst_tbl == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始defend策略配置条目 */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_QOS_DEFEND_POLICY_CB));
    pst_tbl->policy_key = ulkey;

    /*默认给用户队列桶优先级赋值防止只配置防攻击开关导致业务不正常*/
    pst_tbl->usr_pri = 2;
    pst_tbl->usr_que_pri = 2;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_defend_policy_node);

    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DEFEND_APPERC_CB, apperc_key);
    AVLL_INIT_TREE(pst_tbl->apperc_tree, spm_qos_defend_apperc_comp,(NBB_USHORT)avll_key_offset,
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DEFEND_APPERC_CB, spm_defend_apperc_node));


    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DEFEND_USER_DEF_CB, user_def_key);
    AVLL_INIT_TREE(pst_tbl->user_def_tree, spm_qos_defend_usr_def_comp,(NBB_USHORT)avll_key_offset,
                  (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DEFEND_USER_DEF_CB, spm_defend_user_def_node));

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
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
SPM_QOS_DEFEND_CB * spm_alloc_defend_cb(NBB_ULONG ulkey)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_DEFEND_CB *pst_tbl = NULL;
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_defend_policy_cb");

    pst_tbl = (SPM_QOS_DEFEND_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_DEFEND_CB),
              NBB_NORETRY_ACT, MEM_SPM_DEFEND_CB);
    if (pst_tbl == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始ds配置条目 */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_QOS_DEFEND_CB));
    pst_tbl->key = ulkey;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_defend_node);

    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
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
NBB_LONG spm_free_defend_policy_cb(SPM_QOS_DEFEND_POLICY_CB *pst_tbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_APPERC_CB *pcb = NULL;
    SPM_QOS_DEFEND_USER_DEF_CB *npb = NULL;

    NBB_TRC_ENTRY("spm_free_defend_policy_cb");

    NBB_ASSERT(NULL != pst_tbl);

    if (NULL == pst_tbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_QOS_DEFEND_POLICY_CB), MEM_SPM_DEFEND_POLICY_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    for (pcb = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_FIRST(pst_tbl->apperc_tree); pcb != NULL;
         pcb = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_FIRST(pst_tbl->apperc_tree))
    {
        AVLL_DELETE(pst_tbl->apperc_tree, pcb->spm_defend_apperc_node);
        NBB_MM_FREE(pcb, MEM_SPM_DEFEND_APPERC_CB);
    }


    for (npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pst_tbl->user_def_tree); npb != NULL;
         npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pst_tbl->user_def_tree))
    {
        AVLL_DELETE(pst_tbl->user_def_tree, npb->spm_defend_user_def_node);
        NBB_MM_FREE(npb, MEM_SPM_DEFEND_USR_DEF_CB);
    }
    
    NBB_MM_FREE(pst_tbl, MEM_SPM_DEFEND_POLICY_CB);
    pst_tbl = NULL;

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
NBB_LONG spm_free_defend_cb(SPM_QOS_DEFEND_CB *pst_tbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    AVLL_NODE node;
    NBB_TRC_ENTRY("spm_free_defend_cb");

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
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_QOS_DEFEND_CB), MEM_SPM_DEFEND_CB);
    NBB_MM_FREE(pst_tbl, MEM_SPM_DEFEND_CB);
    pst_tbl = NULL;

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
NBB_LONG spm_defend_del_acl(NBB_ULONG acl_id,NBB_ULONG rule_id)
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
            spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_qos_del_meter(NBB_ULONG *meter_id)
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
                spm_api_c3_del_meter_error_log(unit,*meter_id,__FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
        ret = spm_qos_free_meter_id(meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_free_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret);
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
    NBB_ULONG cbs,NBB_ULONG pbs,NBB_ULONG *meter_flag,NBB_ULONG *meter_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    METER_CFG_T meter = {0};
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_qos_set_meter");

    /*CIR == 0代表不限速，不进行meter配置*/
    if(0 != cir)
    {
        if(0 == *meter_id)
        {
            *meter_flag = 1;
            ret = spm_qos_apply_meter_id(meter_id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_qos_apply_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret);
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
                OS_SPRINTF(uc_message,"%s %s,%d : ret=%d,QOS ApiC3SetMeter.\n"
                           "meterId=%d,unit=%d.\n\n",QOS_CFG_STRING,
                           __FUNCTION__,__LINE__,ret,meter.meterId,unit);
                BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
                if(0 != *meter_flag)
                {
                    ret = spm_qos_free_meter_id(meter_id);
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_qos_free_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret);
                    } 
                }
                goto EXIT_LABEL;
            }
#endif
        }
    }
    else 
    {
        ret = spm_qos_del_meter(meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*返回值=ATG_DCI_RC_OK本槽位实例化*/
NBB_LONG spm_defend_check_instance(SPM_QOS_DEFEND_POLICY_CB *pcb)
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
NBB_LONG spm_defend_del_acl_car(NBB_LONG flag,NBB_ULONG *meter_id)
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
                spm_api_c3_del_meter_error_log(unit,*meter_id,__FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
        ret = spm_qos_free_meter_id(meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_free_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret);
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*flag=ATG_DCI_RC_OK白名单*/
NBB_LONG spm_defend_set_acl_car(NBB_LONG flag,NBB_ULONG cir,
    NBB_ULONG pir,NBB_ULONG cbs,NBB_ULONG pbs,NBB_ULONG *meter_id )
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
        ret = spm_qos_apply_meter_id(meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_qos_apply_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret);
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
                ret = spm_qos_free_meter_id(meter_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_qos_free_meter_error_log(meter_id,__FUNCTION__,__LINE__,ret);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
NBB_LONG spm_defend_del_white_cfg(SPM_QOS_DEFEND_POLICY_CB *pcb)
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
    ret = spm_defend_del_acl_car(ATG_DCI_RC_OK,&(pcb->white_meter));
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
 
    /**********************反刷配置不需要*********************/
    rv = spm_defend_check_instance(pcb);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
        ret = spm_defend_del_acl_car(ATG_DCI_RC_OK,&(pcb->white_meter));
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        /*RC_2697单速率三色桶，默认pir为0，cbs与pbs相等.*/
        ret = spm_defend_set_acl_car(ATG_DCI_RC_OK,buf->cir,0,buf->cbs,buf->cbs,&(pcb->white_meter));
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    /**********************反刷配置*********************/
    rv = spm_defend_check_instance(pcb);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
            ret = spm_defend_del_white_cfg(pcb);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
            ret = spm_defend_add_white_cfg(buf,pcb);
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
NBB_LONG spm_defend_del_black_cfg(SPM_QOS_DEFEND_POLICY_CB *pcb)
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
    ret = spm_defend_del_acl_car(ATG_DCI_RC_UNSUCCESSFUL,&(pcb->black_meter));
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    /**********************反刷配置不需要*********************/
    rv = spm_defend_check_instance(pcb);
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
    OS_MEMSET(&(pcb->black_policy),0,sizeof(ATG_DCI_DEFEND_POLICY_BLACK));
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
        ret = spm_defend_del_acl_car(ATG_DCI_RC_UNSUCCESSFUL,&(pcb->black_meter));
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        /*RC_2697单速率三色桶，默认pir为0，cbs与pbs相等.*/
        ret = spm_defend_set_acl_car(ATG_DCI_RC_UNSUCCESSFUL,buf->cir,0,buf->cbs,buf->cbs,&(pcb->black_meter));
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    /**********************反刷配置*********************/
    rv = spm_defend_check_instance(pcb);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
            ret = spm_defend_del_black_cfg(pcb);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
            ret = spm_defend_add_black_cfg(buf,pcb);
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
NBB_LONG spm_defend_del_apper_acl(NBB_ULONG mAclId,NBB_ULONG rule_id)
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

    acl.mAclId = mAclId;
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
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);
            }
#endif
        }
        goto EXIT_LABEL;
    }

    for(i = 0;(rule_id <= SIGNAL_NUM) && (i < g_qos_defend_num[rule_id]);i++)
    {
        acl.mRuleId = g_qos_defend_offset[rule_id] + i;
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelAcl(unit, &acl);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_ldpudp(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ldp");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4udp组播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 0x0286;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6udp组播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 0x0286;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_ldptcp(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ldp");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 0x0286;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 0x0286;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 0x0286;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 0x0286;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_snmp(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_snmp");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4udp*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 161;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6udp*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 161;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_radius(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_radius");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4udp*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 6;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 49;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6udp*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 6;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 49;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_bgpv4(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_bgpv4");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 179;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 179;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_bgpv6(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_bgpv6");  

    rule_id = g_qos_defend_offset[l3_protcal];
    
    /***************tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 179;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 179;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_telnet_client(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_telnet_client");  

    rule_id = g_qos_defend_offset[l3_protcal];
    
    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 23;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 2650;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 3000;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }
  
    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 23;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 2650;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 3000;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_telnet_service(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_telnet_service");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 23;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 2650;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 3000;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 23;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 2650;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 3000;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_ssh_client(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ssh_client");   

    rule_id = g_qos_defend_offset[l3_protcal];
    
    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 22;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 6000;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 22;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 6000;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_ssh_service(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ssh_service");   

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 22;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 6000;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 22;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6tcp单播*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0X86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 6000;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x6;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_dhcp(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_dhcp");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4 udp client*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 0x44;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4 udp serve*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 0x43;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_tacacs(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_tacacs");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4 udp *********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 1812;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4 udp *********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 1813;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6 udp *********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 1812;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6 udp *********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 1813;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_dhcpv6(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_dhcp");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4 udp client*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 546;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV4 udp serve*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 547;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_tcpsyn(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_tcpsyn");

    rule_id = g_qos_defend_offset[l3_protcal];
    
    /***************IPV4*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 6;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.tcp_flag = 0x2;
    pst_acl->tAclRule.tAclMask.tcp_flag = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 6;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.tcp_flag = 0x2;
    pst_acl->tAclRule.tAclMask.tcp_flag = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_ftp_service(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ftp_service");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 20;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x06;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 21;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x06;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }
  
    /***************IPV6*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 20;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x06;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4DstPort = 21;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x06;
    pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_ftp_client(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_ftp_client");  

    rule_id = g_qos_defend_offset[l3_protcal];

    /***************IPV4*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 20;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x06;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x0800;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 21;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x06;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }
  
    /***************IPV6*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 20;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x06;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l4SrcPort = 21;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x06;
    pst_acl->tAclRule.tAclMask.l4SrcPort = 0xffff;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_mld(ACL_T *pst_acl,NBB_USHORT l3_protcal,
    NBB_USHORT que_pri)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_ULONG rule_id = 0;

    NBB_TRC_ENTRY("spm_defend_apper_mld");

    rule_id = g_qos_defend_offset[l3_protcal];
    
    /***************IPV6*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x3a;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.icmpType = 0x82;
    pst_acl->tAclRule.tAclMask.icmpType = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x3a;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.icmpType = 0x83;
    pst_acl->tAclRule.tAclMask.icmpType = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x3a;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.icmpType = 0x84;
    pst_acl->tAclRule.tAclMask.icmpType = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /***************IPV6*********************/
    OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
    OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
    pst_acl->eAclAction = ACL_ACTION_PASS;
    pst_acl->mRuleId = rule_id++;
    pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
    pst_acl->tAclRule.tAclMask.ethType = 0xffff;
    pst_acl->tAclRule.tAclKey.l3Protocol = 0x3a;
    pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
    pst_acl->tAclRule.tAclKey.icmpType = 0x8f;
    pst_acl->tAclRule.tAclMask.icmpType = 0xff;
    pst_acl->phb_cos = (NBB_BYTE)que_pri;
    pst_acl->flags |= ACL_SET_PHB;

    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)

        //coverity[dead_error_condition]
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
NBB_LONG spm_defend_apper_acl_cfg(ACL_T *pst_acl,NBB_USHORT type,
    NBB_USHORT queue_prio)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_defend_apper_acl_cfg");

    if(type <= SIGNAL_NUM)
    {
        pst_acl->mRuleId = g_qos_defend_offset[type];
    }
    
    switch(type)
    {
        case DEFEND_OSPF:
        pst_acl->tAclRule.tAclKey.ethType = 0x0800;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x59;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_ISIS:
        pst_acl->tAclRule.tAclKey.customerData6 = 0xfefe0000;
        pst_acl->tAclRule.tAclMask.customerData6 = 0xffff0000;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_BGP:
        ret = spm_defend_apper_bgpv4(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_LDPUDP:
        ret = spm_defend_apper_ldpudp(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_LDP:
        ret = spm_defend_apper_ldptcp(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_RSVP:
        pst_acl->tAclRule.tAclKey.ethType = 0x0800;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x2e;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
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
        pst_acl->tAclRule.tAclKey.ethType = 0x0800;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x67;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_SNMP:
        ret = spm_defend_apper_snmp(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_RADIUS:
        ret = spm_defend_apper_radius(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_LSP_PING:
        pst_acl->tAclRule.tAclKey.ethType = 0x0800;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x11;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->tAclRule.tAclKey.l4DstPort = 3503;
        pst_acl->tAclRule.tAclMask.l4DstPort = 0xffff;
        pst_acl->tAclRule.tAclKey.dip = 0x7f000000;
        pst_acl->tAclRule.tAclMask.dip = 0xffffff00;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_IGMP:
        pst_acl->tAclRule.tAclKey.ethType = 0x0800;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x2;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_ICMP:
        pst_acl->tAclRule.tAclKey.ethType = 0x0800;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x1;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_VRRP:
        pst_acl->tAclRule.tAclKey.ethType = 0x0800;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x70;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->tAclRule.tAclKey.dip = 0xE0000012;
        pst_acl->tAclRule.tAclMask.dip = 0xffffffff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_DHCP:
        ret = spm_defend_apper_dhcp(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_LACP:
        pst_acl->tAclRule.tAclKey.ethType = 0x8809;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.customerData6  = 0x01000000;
        pst_acl->tAclRule.tAclMask.customerData6 = 0xff000000;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_BFD:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_TACACS:
        ret = spm_defend_apper_tacacs(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_NTP:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_FTP_S:
        ret = spm_defend_apper_ftp_service(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_FTP_C:
        ret = spm_defend_apper_ftp_client(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_TELNET_S:
        ret = spm_defend_apper_telnet_service(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_TELNET_C:
        ret = spm_defend_apper_telnet_client(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_SSH_S:
        ret = spm_defend_apper_ssh_service(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_SSH_C:
        ret = spm_defend_apper_ssh_client(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_OSPFV3:
        pst_acl->tAclRule.tAclKey.ethType = 0x86dd;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x59;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_MLD:
        ret = spm_defend_apper_mld(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_BGPV6:
        ret = spm_defend_apper_bgpv6(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_RIPNG:
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
        break;

        case DEFEND_ICMPV6:
        pst_acl->tAclRule.tAclKey.ethType = 0x86DD;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x3A;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_VRRPV6:
        pst_acl->tAclRule.tAclKey.ethType = 0x86DD;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x70;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        break;

        case DEFEND_DHCPV6:
        ret = spm_defend_apper_dhcpv6(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_PIMV6:
        pst_acl->tAclRule.tAclKey.ethType = 0x86DD;
        pst_acl->tAclRule.tAclMask.ethType = 0xffff;
        pst_acl->tAclRule.tAclKey.l3Protocol = 0x67;
        pst_acl->tAclRule.tAclMask.l3Protocol = 0xff;
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
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
        ret = spm_defend_apper_tcpsyn(pst_acl,type,queue_prio);
        goto EXIT_LABEL;
        break;

        case DEFEND_DEFAULT:
        OS_MEMSET(&(pst_acl->tAclRule.tAclKey),0,sizeof(ACL_KEY_T));
        OS_MEMSET(&(pst_acl->tAclRule.tAclMask),0,sizeof(ACL_KEY_T));
        pst_acl->phb_cos = (NBB_BYTE)queue_prio;
        pst_acl->flags |= ACL_SET_PHB;
        pst_acl->mRuleId = DEFEND_DEFAULT_RULE_ID;
        pst_acl->eAclAction = ACL_ACTION_DROP;
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
        ret = ApiC3SetAcl(unit, pst_acl);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_acl_error_log(unit, pst_acl, __FUNCTION__, __LINE__, ret);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb,NBB_ULONG *meter_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG meter_flag = 0;
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
    ret = spm_qos_set_meter(buf->cir,0,buf->cbs,buf->cbs,&meter_flag,meter_id);
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
    if(0 != g_acl_pri_set)/*ACL的优先级*/
    {
        acl.priority = buf->l3_protoc;
    }
    ret = spm_defend_apper_acl_cfg(&acl,buf->l3_protoc,buf->queue_prio);
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
        if(0 != meter_flag)
        {
            ret = spm_qos_del_meter(meter_id);
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
NBB_LONG spm_defend_del_apper_cfg(ATG_DCI_DEFEND_POLICY_APPERC_P *buf,
        SPM_QOS_DEFEND_POLICY_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_APPERC_CB *pst_ap = NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG mAclId = 0;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

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

    mAclId = (pcb->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    pst_ap = AVLL_FIND(pcb->apperc_tree, buf);
    if(NULL == pst_ap)
    {
        OS_SPRINTF(uc_message,"%s,%d : QOS CAN'T FIND APPER_TREE'S NODE, CAN'T DEL. JUST PRINT THE CONFIGURATION.\n"
                   "policy_index = %d, l3_protoc = %d.\n\n",__FUNCTION__,__LINE__,pcb->policy_key,buf->l3_protoc);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }       
    else
    {
        ret = spm_qos_del_meter(&(pst_ap->meter_id));
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_defend_del_apper_acl(mAclId,pst_ap->apperc_key.l3_protoc);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(pcb->apperc_tree, pst_ap->spm_defend_apperc_node);
        NBB_MM_FREE(pst_ap,MEM_SPM_DEFEND_APPERC_CB);
        pst_ap = NULL;
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
        SPM_QOS_DEFEND_POLICY_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG instance_flag = ATG_DCI_RC_OK;
    NBB_LONG appre_acl_id = 0;
    ACL_T acl = {0};
    SPM_QOS_DEFEND_APPERC_CB *pst_ap  = NULL;
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
    instance_flag = spm_defend_check_instance(pcb);
    appre_acl_id = (pcb->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    pst_ap = AVLL_FIND(pcb->apperc_tree, buf);
    if(NULL == pst_ap)
    {
        ret = spm_defend_add_apper_acl(buf,pcb,&meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        if(/*(ATG_DCI_RC_OK == instance_flag) && */(pcb->apper_pri != buf->prio))
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
        pst_ap = (SPM_QOS_DEFEND_APPERC_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_DEFEND_APPERC_CB),
                 NBB_NORETRY_ACT, MEM_SPM_DEFEND_APPERC_CB);
        AVLL_INIT_NODE(pst_ap->spm_defend_apperc_node);
        
        //coverity[bad_sizeof]  
        NBB_MEMCPY(&(pst_ap->apperc_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_APPERC_P));
        pst_ap->meter_id = meter_id;
        pcb->apper_pri = buf->prio;
        pcb->apper_que_pri = buf->queue_prio;
        
        //coverity[no_effect_test]
        AVLL_INSERT(pcb->apperc_tree, pst_ap->spm_defend_apperc_node);
    }  
    else
    {
        rv = NBB_MEMCMP(&(pst_ap->apperc_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_APPERC_P));
        if(0 != rv)
        {
            if(pcb->apper_pri != buf->prio)
            {
                pcb->apper_pri = buf->prio;
                
                /*if(ATG_DCI_RC_OK == instance_flag)*/
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
            ret = spm_defend_add_apper_acl(buf,pcb,&(pst_ap->meter_id));
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
            NBB_MEMCPY(&(pst_ap->apperc_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_APPERC_P));
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
        SPM_QOS_DEFEND_POLICY_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    SPM_QOS_DEFEND_APPERC_CB *pst_ap = NULL;

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
        ret = spm_defend_del_apper_cfg(buf,pcb);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
        ret = spm_defend_add_apper_cfg(buf,pcb);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb,NBB_ULONG *meter_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG meter_flag = 0;
    NBB_BYTE unit = 0;
    ACL_T acl = {0};
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_defend_add_usr_acl");

    if(NULL == buf)
    {
        printf("**QOS ERROR**,%s,%d usr policy buf==NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS usr policy buf = NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index","","","",pcb->policy_key,0,0,0));
        OS_SPRINTF(uc_message,"%s %s,%d policy index=%d.\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,pcb->policy_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        goto EXIT_LABEL;
    }

    /*RC_2697单速率三色桶，默认pir为0，cbs与pbs相等.*/
    ret = spm_qos_set_meter(buf->cir,0,buf->cbs,buf->cbs,&meter_flag,meter_id);
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
    if(0 != g_acl_pri_set)/*ACL的优先级*/
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
        spm_get_ipv6_mask(buf->srcip_mask_len, (NBB_ULONG*)&(acl.tAclRule.tAclMask.sipv6));
        spm_get_ipv6_mask(buf->dstip_mask_len, (NBB_ULONG*)&(acl.tAclRule.tAclMask.dipv6));
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
            spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);

           /***************************************************************************
            *如果meter_id是本次新申请的，那么当自定义策略acl写入硬件失败后，需要将此次*
            *申请的meter_id释放。若meter_id不是本次新申请的，即使acl写入失败也不必释放*
            ***************************************************************************/
            if(0 != meter_flag)
            {
                ret = spm_qos_del_meter(meter_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS DEL METER FAIL**%s,%d, meter_id=%d spm_qos_del_meter ERROR=%d,\n",
                              __FUNCTION__,__LINE__,*meter_id,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
            					   "QOS spm_qos_del_meter",ret,
            					   "","","unit","meter_id",0,0,unit,*meter_id));
                    OS_SPRINTF(uc_message,"%s %s,%d : ret=%d,QOS spm_qos_del_meter.\n"
                               "unit=%d,meter_id=%d.\n\n",QOS_CFG_STRING,
                               __FUNCTION__,__LINE__,ret,*meter_id);
                    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_USER_DEF_CB *pst_ap = NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG mAclId = 0;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

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

    mAclId = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
    pst_ap = AVLL_FIND(pcb->user_def_tree, buf);
    if(NULL == pst_ap)
    {
        OS_SPRINTF(uc_message,"%s,%d : QOS CAN'T FIND USER_DEF_TREE'S NODE, CAN'T DEL. JUST PRINT THE CONFIGURATION.\n"
                   "policy_index = %d, rule_id = %d.\n\n",__FUNCTION__,__LINE__,pcb->policy_key,buf->rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }       
    else
    {
        ret = spm_qos_del_meter(&(pst_ap->meter_id));
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        ret = spm_defend_del_acl(mAclId,pst_ap->user_def_key.rule_id);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        AVLL_DELETE(pcb->user_def_tree, pst_ap->spm_defend_user_def_node);
        NBB_MM_FREE(pst_ap,MEM_SPM_DEFEND_USR_DEF_CB);
        pst_ap = NULL;
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG instance_flag = ATG_DCI_RC_OK;
    NBB_LONG usr_acl_id = 0;
    ACL_T acl = {0};
    SPM_QOS_DEFEND_USER_DEF_CB *pst_usr  = NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG meter_id = 0;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_defend_add_usr_define_cfg");

    if(NULL == buf)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        printf("**QOS ERROR**,%s,%d usr policy buf = NULL,policy index=%d\n",
                  __FUNCTION__,__LINE__,pcb->policy_key);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS usr policy buf = NULL",ATG_DCI_RC_UNSUCCESSFUL,
					   "policy index","","","",pcb->policy_key,0,0,0));
        OS_SPRINTF(uc_message,"%s %s,%d policy index=%d.\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,pcb->policy_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        goto EXIT_LABEL;
    }

    /*判断防攻击策略是否被该slot绑定*/
    instance_flag = spm_defend_check_instance(pcb);
    usr_acl_id = (pcb->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
    pst_usr = AVLL_FIND(pcb->user_def_tree, buf);
    if(NULL == pst_usr)
    {
        /*申请meter_id,同时将acl规则写入硬件中*/
        ret = spm_defend_add_usr_acl(buf,pcb,&meter_id);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

        if(/*(ATG_DCI_RC_OK == instance_flag) && */(pcb->usr_pri != buf->prio))
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
        pst_usr = (SPM_QOS_DEFEND_USER_DEF_CB*)NBB_MM_ALLOC(sizeof(SPM_QOS_DEFEND_USER_DEF_CB),
                  NBB_NORETRY_ACT, MEM_SPM_DEFEND_USR_DEF_CB);
        AVLL_INIT_NODE(pst_usr->spm_defend_user_def_node);
        
        //coverity[bad_sizeof]  
        NBB_MEMCPY(&(pst_usr->user_def_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_USER_DF));
        pst_usr->meter_id = meter_id;
        pcb->usr_pri = buf->prio;
        pcb->usr_que_pri = buf->queue_prio;
        
        //coverity[no_effect_test]
        AVLL_INSERT(pcb->user_def_tree, pst_usr->spm_defend_user_def_node);
    }       
    else
    {
        rv = NBB_MEMCMP(&(pst_usr->user_def_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_USER_DF));
        if(0 != rv)
        {
            if(pcb->usr_pri != buf->prio)
            {
                pcb->usr_pri = buf->prio;
                
                /*if(ATG_DCI_RC_OK == instance_flag)*/
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
            ret = spm_defend_add_usr_acl(buf,pcb,&(pst_usr->meter_id));
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
            NBB_MEMCPY(&(pst_usr->user_def_key),buf,sizeof(ATG_DCI_DEFEND_POLICY_USER_DF));
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    SPM_QOS_DEFEND_USER_DEF_CB *pst_usr = NULL;

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
        ret = spm_defend_del_usr_define_cfg(buf,pcb);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:
        ret = spm_defend_add_usr_define_cfg(buf,pcb);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
                    spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
                    spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_set_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
                        spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, rv);
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
    SPM_QOS_DEFEND_POLICY_CB *pcb)
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
NBB_LONG spm_del_defend_driver(SPM_QOS_DEFEND_POLICY_CB *pst_pol)
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

    if(NULL == pst_pol)
    {
        printf("**QOS ERROR**%s,%d cfg == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS SPM_QOS_DEFEND_POLICY_CB = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    slot = SHARED.local_slot_id;
    
    /*if(ATG_DCI_RC_OK != pst_pol->defend_slot[slot])
    {
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }*/

    apr_acl = (pst_pol->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    usr_acl = (pst_pol->policy_key) + DEFEND_ACL_ID_USR_OFFSET;  

    /*************删除udp abnormal开关创建的ACL***************/
    for(unit = 0;(unit < SHARED.c3_num) && (0 != pst_pol->abnormal_switch.switch_flag);unit++)
    {
        for(i = DEFEND_ABNORMAL_RULE_OFFSET;i < DEFEND_ABNORMAL_RULE_OFFSET + DEFEND_ABNORMAL_RULE_NUM;i++)
        {
            acl.mAclId = (pst_pol->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = i;
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelAcl(unit,&acl);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);
                rv = ret;
            }  
#endif
        }
    }
    for(unit = 0;(unit < SHARED.c3_num) && (0 != pst_pol->udp_swtich.switch_flag);unit++)
    {
        for(i = DEFEND_UDP_RULE_OFFSET;i < DEFEND_UDP_RULE_OFFSET + DEFEND_UDP_RULE_NUM;i++)
        {
            acl.mAclId = (pst_pol->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
            acl.mRuleId = i;
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelAcl(unit,&acl);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);
                rv = ret;
            }  
#endif
        }
    }

    /******************删除METER******************************/
    for (pcb = (SPM_QOS_DEFEND_APPERC_CB*)AVLL_FIRST(pst_pol->apperc_tree); pcb != NULL;
         pcb = (SPM_QOS_DEFEND_APPERC_CB*)AVLL_NEXT(pst_pol->apperc_tree, pcb->spm_defend_apperc_node))
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
                    spm_api_c3_del_meter_error_log(unit,pcb->meter_id,__FUNCTION__,__LINE__,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3DelMeter",ret,
                    			   "l3_protoc","pri","queue_prio","defend policy index",
                    			   pcb->apperc_key.l3_protoc,pst_pol->apper_pri,
                    			   pcb->apperc_key.queue_prio,pst_pol->policy_key));
                    rv = ret;
                } 
                else
                {
                    ret = spm_qos_free_meter_id(&(pcb->meter_id));
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_qos_free_meter_error_log(&(pcb->meter_id),__FUNCTION__,__LINE__,ret);
                        rv = ret;
                    }
                }
#endif
            }
        }

        ret = spm_defend_del_apper_acl(apr_acl,pcb->apperc_key.l3_protoc);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }


/**********************************新用户自定义************************************/
    for (npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pst_pol->user_def_tree); npb != NULL;
         npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_NEXT(pst_pol->user_def_tree,npb->spm_defend_user_def_node))
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
                    spm_api_c3_del_meter_error_log(unit,npb->meter_id,__FUNCTION__,__LINE__,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3DelMeter",ret,
                    			   "rule_id","pri","queue_prio","defend policy index",
                    			   npb->user_def_key.rule_id,pst_pol->usr_pri,
                    			   npb->user_def_key.queue_prio,pst_pol->policy_key));
                    rv = ret;
                }  
                else
                {
                    ret = spm_qos_free_meter_id(&(npb->meter_id));
                    if(ATG_DCI_RC_OK != ret)
                    {
                        spm_qos_free_meter_error_log(&(npb->meter_id),__FUNCTION__,__LINE__,ret);
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
                spm_api_c3_del_acl_error_log(unit, &acl, __FUNCTION__, __LINE__, ret);
                rv = ret;
            }  
#endif
        }
    }

/***************************白名单*************************************/
    if(0 != pst_pol->white_meter)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotWhiteListMeter(unit,pst_pol->white_meter,0);  
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotWhiteListMeter meter id=%d,ret=%d\n",
                          __FUNCTION__,__LINE__,pst_pol->white_meter,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotWhiteListMeter",ret,
                			   "meter id","white meter","disable","defend policy index",
                			   pst_pol->white_meter,0,0,pst_pol->policy_key));
                rv = ret;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelMeter(unit,pst_pol->white_meter);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_meter_error_log(unit,pst_pol->white_meter,__FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3DelMeter",ret,
                			   "meter id","white meter","","defend policy index",
                			   pst_pol->white_meter,0,0,pst_pol->policy_key));
                rv = ret;
            }   
            else
            {
                ret = spm_qos_free_meter_id(&(pst_pol->white_meter));
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_qos_free_meter_error_log(&(pst_pol->white_meter),__FUNCTION__,__LINE__,ret);
                    rv = ret;
                }
            }
#endif
        }
    }
    if(0 != pst_pol->black_meter)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotBlackListMeter(unit,pst_pol->black_meter,0); 
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**,%s,%d ApiC3SetSlotBlackListMeter meter id=%d,ret=%d\n",
                          __FUNCTION__,__LINE__,pst_pol->black_meter,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotBlackListMeter",ret,
                			   "meter id","black meter","disable","defend policy index",
                			   pst_pol->black_meter,0,0,pst_pol->policy_key));
                rv = ret;
            }
#endif 
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3DelMeter(unit,pst_pol->black_meter);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_del_meter_error_log(unit,pst_pol->black_meter,__FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3DelMeter",ret,
                			   "meter id","black meter","","defend policy index",
                			   pst_pol->black_meter,0,0,pst_pol->policy_key));
                rv = ret;
            }
            else
            {
                ret = spm_qos_free_meter_id(&(pst_pol->black_meter));
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_qos_free_meter_error_log(&(pst_pol->black_meter),__FUNCTION__,__LINE__,ret);
                    rv = ret;
                }
            }
#endif
        }
    }
    pst_pol->defend_slot[slot] = ATG_DCI_RC_OK;

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
NBB_VOID spm_rcv_dci_set_defend_policy(ATG_DCI_SET_DEFEND_POLICY *pst_setips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE uc_exist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_QOS_DEFEND_POLICY_CB *pst_cb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT i = 0;

    NBB_BYTE *puc_apperswitch_datastart = ATG_DCI_OPER_NULL;
    NBB_BYTE *puc_white_datastart = ATG_DCI_OPER_NULL;
    NBB_BYTE *puc_blackdatastart = ATG_DCI_OPER_NULL;
    NBB_BYTE *puc_apperceive_datastart = ATG_DCI_OPER_NULL;
    NBB_BYTE *puc_userdef_datastart = ATG_DCI_OPER_NULL;
    NBB_BYTE *puc_totl_pkt_datastart = ATG_DCI_OPER_NULL;   
    NBB_BYTE *puc_abnormal_datastart = ATG_DCI_OPER_NULL;
    NBB_BYTE *puc_fragment_datastart = ATG_DCI_OPER_NULL;
    NBB_BYTE *puc_udppkt_datastart = ATG_DCI_OPER_NULL;
    NBB_BYTE *puc_tcpsys_datastart = ATG_DCI_OPER_NULL;

    NBB_ULONG ul_oper_apperswitch = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_white = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_black = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_apperceive = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_userdef = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_totlpkt = ATG_DCI_OPER_NULL;   
    NBB_ULONG ul_oper_abnormal = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_fragment = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_udppkt = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_tcpsys = ATG_DCI_OPER_NULL;

    ATG_DCI_DEFEND_POLICY_APPERC_S *pst_apperswitchdata = NULL;
    ATG_DCI_DEFEND_POLICY_WHITE *pst_whitedata = NULL;    
    ATG_DCI_DEFEND_POLICY_BLACK *pst_blackdata = NULL;
    ATG_DCI_DEFEND_POLICY_APPERC_P *pst_apperceivedata = NULL;
    ATG_DCI_DEFEND_POLICY_USER_DF *pst_userdefdata = NULL;
    ATG_DCI_DEFEND_POLICY_TOTAL_PKT *pst_totlpktdata = NULL;
    ATG_DCI_DEFEND_POLICY_ABNORMAL *pst_abnormaldata = NULL;
    ATG_DCI_DEFEND_POLICY_FRAGMENT *pst_fragmentdata = NULL;
    ATG_DCI_DEFEND_POLICY_UDP_PKT *pst_udppktdata = NULL;
    ATG_DCI_DEFEND_POLICY_TCPSYS *pst_tcpsysdata = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pst_setips)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }

    pst_setips->return_code = ATG_DCI_RC_OK;
    ulkey = pst_setips->key;
    pst_cb = AVLL_FIND(g_qos_defend_policy_tree, &ulkey);
    if (pst_cb == NULL)
    {
        uc_exist = QOS_UNEXIST;
    }

    ul_oper_apperswitch = pst_setips->oper_apperceive_s;
    ul_oper_white = pst_setips->oper_white;
    ul_oper_black = pst_setips->oper_black;
    ul_oper_apperceive = pst_setips->oper_apperceive_p;
    ul_oper_userdef = pst_setips->oper_user_define;
    ul_oper_totlpkt = pst_setips->oper_total_packet;
    ul_oper_abnormal = pst_setips->oper_abnormal_packet;
    ul_oper_fragment = pst_setips->oper_fragment_flood;
    ul_oper_udppkt = pst_setips->oper_udp_packet;
    ul_oper_tcpsys = pst_setips->oper_tcpsys_flood;

    puc_apperswitch_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->apperceive_s_data);
    puc_white_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->white_data);
    puc_blackdatastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->black_data);
    puc_apperceive_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->apperceive_p_data);
    puc_userdef_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->user_define_data);
    puc_totl_pkt_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->total_packet_data);
    puc_abnormal_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->abnormal_packet_data);
    puc_fragment_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->fragment_flood_data);
    puc_udppkt_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->udp_packet_data);
    puc_tcpsys_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->tcpsys_flood_data);

    pst_apperswitchdata = (ATG_DCI_DEFEND_POLICY_APPERC_S*)puc_apperswitch_datastart;
    pst_whitedata = (ATG_DCI_DEFEND_POLICY_WHITE*)puc_white_datastart;    
    pst_blackdata = (ATG_DCI_DEFEND_POLICY_BLACK*)puc_blackdatastart;
    pst_apperceivedata = (ATG_DCI_DEFEND_POLICY_APPERC_P*)puc_apperceive_datastart;
    pst_userdefdata = (ATG_DCI_DEFEND_POLICY_USER_DF*)puc_userdef_datastart;
    pst_totlpktdata = (ATG_DCI_DEFEND_POLICY_TOTAL_PKT*)puc_totl_pkt_datastart;
    pst_abnormaldata = (ATG_DCI_DEFEND_POLICY_ABNORMAL*)puc_abnormal_datastart;
    pst_fragmentdata = (ATG_DCI_DEFEND_POLICY_FRAGMENT*)puc_fragment_datastart;
    pst_udppktdata = (ATG_DCI_DEFEND_POLICY_UDP_PKT*)puc_udppkt_datastart;
    pst_tcpsysdata = (ATG_DCI_DEFEND_POLICY_TCPSYS*)puc_tcpsys_datastart;

    /* ips消息为删除 */
    if (TRUE == pst_setips->delete_struct)
    {
        if (uc_exist == QOS_UNEXIST)
        {
            pst_setips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /* 存在,删除 */
        else
        {
            if(NULL == pst_cb)
            {
                pst_setips->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }

            /* 被引用 */
            /*if (0 != pst_cb->cnt)
            {
                pst_setips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }*/
            spm_del_defend_driver(pst_cb );
            AVLL_DELETE(g_qos_defend_policy_tree, pst_cb->spm_defend_policy_node);

            /* 释放POLICY表分配的内存空间 */
            ret = spm_free_defend_policy_cb(pst_cb );
            if (ATG_DCI_RC_OK != ret)
            {
                pst_setips->return_code = ATG_DCI_RC_ALLOC_ERROR;
                goto EXIT_LABEL;
            }
        }
    }

    /*增加或者更新*/
    else
    {
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (uc_exist == QOS_UNEXIST)
        {
            /* 申请一个新条目的内存空间 */
            pst_cb = spm_alloc_defend_policy_cb(ulkey );
            if (NULL == pst_cb)
            {
                pst_setips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }
        if(NULL == pst_cb)
        {
            pst_setips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        
        /*if (0 != pst_cb->cnt) 
        {
            pst_setips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }*/
        ret = spm_defend_set_apperswitch(ul_oper_apperswitch,pst_apperswitchdata,pst_cb );
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->apperceive_s_return_code = ret;
        }
        ret = spm_defend_set_white(ul_oper_white,pst_whitedata,pst_cb );
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->white_return_code = ret;
        }
        ret = spm_defend_set_black(ul_oper_black,pst_blackdata,pst_cb );
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->black_return_code = ret;
        }
        for( i = 0;i < pst_setips->apperceive_p_num;i++)
        {
            if(NULL != pst_apperceivedata)
            {
                pst_apperceivedata = (ATG_DCI_DEFEND_POLICY_APPERC_P *)
                    (puc_apperceive_datastart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_DEFEND_POLICY_APPERC_P))) * i);
                ret = spm_defend_set_apperceive(ul_oper_apperceive,pst_apperceivedata,pst_cb );
                if(ATG_DCI_RC_OK != ret)
                {
                    pst_setips->return_code = ret;
                    pst_setips->apperceive_p_return_code[i] = ret;
                }
            }
        }

        for( i = 0;i < pst_setips->user_define_num;i++)
        {
            if(NULL != pst_userdefdata)
            {
                pst_userdefdata = (ATG_DCI_DEFEND_POLICY_USER_DF *)
                    (puc_userdef_datastart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_DEFEND_POLICY_USER_DF))) * i);
                ret = spm_defend_set_usr_define(ul_oper_userdef,pst_userdefdata,pst_cb );
                if(ATG_DCI_RC_OK != ret)
                {
                    pst_setips->return_code = ret;
                    pst_setips->user_define_return_code = ret;
                }
            }
        }
        ret = spm_defend_set_totalpkt(ul_oper_totlpkt,pst_totlpktdata,pst_cb );
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->total_packet_return_code = ret;
        }
        ret = spm_defend_set_abnormal(ul_oper_abnormal,pst_abnormaldata,pst_cb );
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->abnormal_packet_return_code = ret;
        }
        ret = spm_defend_set_fragment(ul_oper_fragment,pst_fragmentdata,pst_cb );
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->fragment_flood_return_code = ret;
        }
        ret = spm_defend_set_udp(ul_oper_udppkt,pst_udppktdata,pst_cb );
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->udp_packet_return_code = ret;
        }
        ret = spm_defend_set_tcpsys(ul_oper_tcpsys,pst_tcpsysdata,pst_cb );
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->tcpsys_flood_return_code = ret;
        }

        /* 如果是新增的条目，插入到树中 */
        if (QOS_UNEXIST == uc_exist)
        {
            //coverity[no_effect_test]
            AVLL_INSERT(g_qos_defend_policy_tree, pst_cb->spm_defend_policy_node);
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
NBB_VOID spm_qos_clear_all_defend_policy()
{
    SPM_QOS_DEFEND_POLICY_CB *cfg_cb = NULL;


    for (cfg_cb = (SPM_QOS_DEFEND_POLICY_CB*) AVLL_FIRST(g_qos_defend_policy_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DEFEND_POLICY_CB*) AVLL_FIRST(g_qos_defend_policy_tree))
    {
        AVLL_DELETE(g_qos_defend_policy_tree, cfg_cb->spm_defend_policy_node);
        spm_free_defend_policy_cb(cfg_cb );   
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
NBB_LONG spm_set_slot_policy_cfg(NBB_ULONG slot,NBB_ULONG policy_index )
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_POLICY_CB *pst_pol = NULL;
    NBB_ULONG appre_acl_id = 0;
    NBB_ULONG usr_acl_id = 0;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(0 == policy_index) /*不处理为0的直接存储*/
    {
        goto EXIT_LABEL;
    }
    pst_pol = AVLL_FIND(g_qos_defend_policy_tree,&policy_index);
    if(NULL == pst_pol)
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
            if( 0 == pst_pol->apperc_policy_switch.switch_flag)
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
                          __FUNCTION__,__LINE__,pst_pol->white_policy.acl_index,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotRelativeAclEnable",ret,
                			   "","","","defend policy index",
                			   0,0,0,pst_pol->policy_key));
                rv += ret;
            } 
#endif
            if(0 != pst_pol->apper_pri)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotRelativeAcl(unit,appre_acl_id,pst_pol->apper_pri);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotRelativeAcl acl=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,usr_acl_id,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotRelativeAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   appre_acl_id,pst_pol->apper_pri,
                    			   pst_pol->apper_que_pri,policy_index));
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
            if((0 != pst_pol->white_policy.prio) && (0 != pst_pol->white_policy.queue_prio))
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotWhiteAcl(unit,pst_pol->white_policy.acl_index,
                      pst_pol->white_policy.prio,pst_pol->white_policy.queue_prio);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotWhiteAcl acl=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,pst_pol->white_policy.acl_index,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotWhiteAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   pst_pol->white_policy.acl_index,pst_pol->white_policy.prio,
                    			   pst_pol->white_policy.queue_prio,policy_index));
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
            if((0 != pst_pol->black_policy.prio) && (0 != pst_pol->black_policy.queue_prio))
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotBlackAcl(unit,pst_pol->black_policy.acl_index,
                      pst_pol->black_policy.prio,pst_pol->black_policy.queue_prio);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotBlackAcl acl=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,pst_pol->black_policy.acl_index,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotBlackAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   pst_pol->black_policy.acl_index,pst_pol->black_policy.prio,
                    			   pst_pol->black_policy.queue_prio,policy_index));
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
            if(0 != pst_pol->usr_pri)
            {
#if defined (SPU) || defined (PTN690_CES)

                //coverity[dead_error_condition]
                ret = ApiC3SetSlotUserAcl(unit,usr_acl_id,pst_pol->usr_pri);
                if(ATG_DCI_RC_OK != ret)
                {
                    printf("**QOS ERROR**,%s,%d ApiC3SetSlotUserAcl acl=%d,ret=%d\n",
                              __FUNCTION__,__LINE__,usr_acl_id,ret);
                    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                    			   "QOS ApiC3SetSlotUserAcl",ret,
                    			   "acl id","pri","queue_prio","defend policy index",
                    			   usr_acl_id,pst_pol->usr_pri,
                    			   pst_pol->usr_que_pri,policy_index));
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
            pst_pol->defend_slot[slot] = ATG_DCI_RC_UNSUCCESSFUL;
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
NBB_LONG spm_del_slot_driver(SPM_QOS_DEFEND_POLICY_CB *pst_pol)
{
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_INT rv = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    NBB_BYTE slot = 0;
    NBB_ULONG apr_acl = 0;
    NBB_ULONG usr_acl = 0;

	NBB_TRC_ENTRY("spm_del_slot_driver");

    if(NULL == pst_pol)
    {
        printf("**QOS ERROR**%s,%d cfg == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS SPM_QOS_DEFEND_POLICY_CB = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    apr_acl = (pst_pol->policy_key) + DEFEND_ACL_ID_APPRE_OFFSET;
    usr_acl = (pst_pol->policy_key) + DEFEND_ACL_ID_USR_OFFSET;
    slot = SHARED.local_slot_id;
    if(ATG_DCI_RC_OK != pst_pol->defend_slot[slot])
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
           ret = ApiC3SetSlotRelativeAclEnable(unit,0);

        /* 在disable时张正辉已将所有联动相关的数据全部清除,无需再调用下面的函数,否则执行结果会产生错误 */
        /* ret = ApiC3SetSlotRelativeAcl(unit,0,pst_pol->apper_pri); */
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS CLEAR ERROR**,%s,%d : ApiC3SetSlotRelativeAcl mAclId=0 ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotRelativeAcl",ret,
                			   "","mAclId","apper_pri","defend policy index",
                			   0,0,pst_pol->apper_pri,pst_pol->policy_key));
                rv += ret;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotWhiteAclEnable(unit,0);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS CLEAR ERROR**,%s,%d : ApiC3SetSlotWhiteAcl mAclId=0 ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotWhiteAclEnable",ret,
                			   "mAclId","prio","queue_prio","defend policy index",
                			   0,pst_pol->white_policy.prio,pst_pol->white_policy.queue_prio,pst_pol->policy_key));
                rv += ret;
            }
#endif
           
#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotBlackAclEnable(unit,0);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS CLEAR ERROR**,%s,%d : ApiC3SetSlotBlackAcl mAclId=0 ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotBlackAcl",ret,
                			   "mAclId","prio","queue_prio","defend policy index",
                			   0,pst_pol->black_policy.prio,pst_pol->black_policy.queue_prio,pst_pol->policy_key));
                rv += ret;
            }
#endif

#if defined (SPU) || defined (PTN690_CES)

            //coverity[dead_error_condition]
            ret = ApiC3SetSlotUserAclEnable(unit,0);
            if(ATG_DCI_RC_OK != ret)
            {
                printf("**QOS CLEAR ERROR**,%s,%d : ApiC3SetSlotUserAcl mAclId=0 ret=%d\n",
                          __FUNCTION__,__LINE__,ret);
                NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                			   "QOS ApiC3SetSlotUserAcl",ret,
                			   "","mAclId","usr_pri","defend policy index",
                			   0,0,pst_pol->usr_pri,pst_pol->policy_key));
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
        pst_pol->defend_slot[slot] = ATG_DCI_RC_OK;
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
    NBB_BYTE new_switch)
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
    NBB_ULONG new_policy )
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    SPM_QOS_DEFEND_POLICY_CB *pst_pol = NULL;

    NBB_TRC_ENTRY("spm_set_slot_defend_policy");

    if(ulkey == SHARED.local_slot_id)
    {
       if((0 == old_policy) && (new_policy == old_policy))/*不配置*/
       {
            goto EXIT_LABEL;
       }
       else if((0 == old_policy) && (0 != new_policy))/*增加*/
       {
            ret = spm_set_slot_policy_cfg(ulkey,new_policy );
            goto EXIT_LABEL;
       }
       else if((0 != old_policy) && (0 == new_policy))/*删除*/
       {
            pst_pol = AVLL_FIND(g_qos_defend_policy_tree,&old_policy);
            ret = spm_del_slot_driver(pst_pol );
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
    SPM_QOS_DEFEND_CB *pcb, ATG_DCI_DEFEND_SWITCH_BASIC *buf)
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
             buf->arp_defend_switch  );
        if(ATG_DCI_RC_OK == rv)
        {
            pcb->basic.arp_defend_switch = buf->arp_defend_switch;
        }
        else
        {
            ret = rv;
        }
        rv = spm_set_slot_defend_policy(pcb->key,pcb->basic.defend_switch_index,
             buf->defend_switch_index );
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
NBB_VOID spm_rcv_dci_set_slot_defend(ATG_DCI_SET_DEFEND_SWITCH *pst_setips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE uc_exist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    SPM_QOS_DEFEND_CB *pst_cb = NULL;
    SPM_QOS_DEFEND_POLICY_CB *pst_pol = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];
    NBB_BYTE *puc_basic_datastart = ATG_DCI_OPER_NULL;
    NBB_BYTE unit = 0;
    NBB_ULONG ul_oper_basic = ATG_DCI_OPER_NULL;

    ATG_DCI_DEFEND_SWITCH_BASIC *pst_basichdata = NULL;

    /*逻辑处理流程*/
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pst_setips)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }

    /* 首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL */
    pst_setips->return_code = ATG_DCI_RC_OK;
    ulkey = pst_setips->key;
    pst_cb = AVLL_FIND(g_qos_defend_tree, &ulkey);

    /* 如果条目不存在 */
    if (pst_cb == NULL)
    {
        uc_exist = QOS_UNEXIST;
    }

    ul_oper_basic = pst_setips->oper_basic;
    puc_basic_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->basic_data);
    pst_basichdata = (ATG_DCI_DEFEND_SWITCH_BASIC*)puc_basic_datastart;
    if((ATG_DCI_RC_OK != g_qos_defend_cfg_print) && (NULL != pst_basichdata))
    {
        printf("%s %s,%d slot=%d defend_switch_index=%d arp_defend_switch=%d\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,ulkey,pst_basichdata->defend_switch_index,
                  pst_basichdata->arp_defend_switch);
        OS_SPRINTF(uc_message,"%s %s,%d slot=%d defend_switch_index=%d arp_defend_switch=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,ulkey,pst_basichdata->defend_switch_index,
                   pst_basichdata->arp_defend_switch);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }

    if(TRUE == pst_setips->delete_struct)
    {
        if (uc_exist == QOS_UNEXIST)
        {
            pst_setips->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }

        /* 存在,删除 */
        else
        {
            if(NULL == pst_cb)
            {
                pst_setips->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }

            if(0 != pst_cb->basic.defend_switch_index)
            {
                pst_pol = AVLL_FIND(g_qos_defend_policy_tree,&(pst_cb->basic.defend_switch_index));
                spm_del_slot_driver(pst_pol);
            } 
            if(0 != pst_cb->basic.arp_defend_switch)
            {
                for(unit = 0;unit < SHARED.c3_num;unit++)
		        {		
#if defined (SPU) || defined (PTN690_CES)
			        ApiC3SetSlotGuard(unit,0);
#endif
				}
            }
            AVLL_DELETE(g_qos_defend_tree, pst_cb->spm_defend_node);
            ret = spm_free_defend_cb(pst_cb);
            if (ATG_DCI_RC_OK != ret)
            {
                pst_setips->return_code = ATG_DCI_RC_DEL_FAILED;
                goto EXIT_LABEL;
            }
        }
    }
    else
    {
        if (NULL == pst_cb)
        {
            pst_cb = spm_alloc_defend_cb(ulkey);
            if (NULL == pst_cb)
            {
                pst_setips->return_code = ATG_DCI_RC_ADD_FAILED;
                goto EXIT_LABEL;
            }
            
            //coverity[no_effect_test]
            AVLL_INSERT(g_qos_defend_tree, pst_cb->spm_defend_node);      
        }
        ret = spm_set_slot_defend_cfg(ul_oper_basic,pst_cb,pst_basichdata);      
        pst_setips->return_code = ret;
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
NBB_VOID spm_qos_clear_all_defend_switch()
{
    SPM_QOS_DEFEND_CB *cfg_cb = NULL;


    for (cfg_cb = (SPM_QOS_DEFEND_CB*) AVLL_FIRST(g_qos_defend_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DEFEND_CB*) AVLL_FIRST(g_qos_defend_tree))
    {
        AVLL_DELETE(g_qos_defend_tree, cfg_cb->spm_defend_node);
        spm_free_defend_cb(cfg_cb);   
    }

    return;   
}



#if 20

/*****************************************************************************
   函 数 名  : qos_show_defend_switch
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
NBB_VOID  qos_show_defend_switch(NBB_ULONG slot)
{
    SPM_QOS_DEFEND_CB *pst_cb = NULL;

    pst_cb = AVLL_FIND(g_qos_defend_tree, &slot);

    if(NULL == pst_cb)
    {
        printf("can't find defend switch cfg\n");
        return;
    }

    printf("slot=%ld,defend policy idex=%ld,arp=%s\n\n",slot,
        pst_cb->basic.defend_switch_index,pst_cb->basic.arp_defend_switch?"开":"关");

    return;
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
NBB_VOID  qos_show_defend_policy(NBB_ULONG index)
{
    SPM_QOS_DEFEND_POLICY_CB *pst_cb = NULL;
    SPM_QOS_DEFEND_APPERC_CB  *pst_ap = NULL;
    SPM_QOS_DEFEND_USER_DEF_CB *pst_usr = NULL;
    NBB_USHORT mAclId = 0;
    NBB_USHORT rule_id = 0;
    NBB_USHORT num = 0;
    NBB_BYTE flag = 0;
    NBB_BYTE slot = 0;
    NBB_USHORT i = 0;


    pst_cb = AVLL_FIND(g_qos_defend_policy_tree, &index);

    if(NULL == pst_cb)
    {
        printf("can't find defend policy cfg\n");
        return;
    }

    slot = v_spm_shared->local_slot_id;
    flag = pst_cb->defend_slot[slot];
    printf("local_slot=%d,defend state=%s\n",slot,(flag)?"开":"关");
    printf("apperc_policy_switch=%s\n\n",(pst_cb->apperc_policy_switch.switch_flag)?"关":"开");
    printf("white mAclId=%ld,prio=%d,queue_prio=%d,cir=%ld,cbs=%d,meter=%ld\n\n",
        pst_cb->white_policy.acl_index,
        pst_cb->white_policy.prio,
        pst_cb->white_policy.queue_prio,
        pst_cb->white_policy.cir,
        pst_cb->white_policy.cbs,
        pst_cb->white_meter);
    printf("black mAclId=%ld,prio=%d,queue_prio=%d,cir=%ld,cbs=%d,meter=%ld\n\n",
        pst_cb->black_policy.acl_index,
        pst_cb->black_policy.prio,
        pst_cb->black_policy.queue_prio,
        pst_cb->black_policy.cir,
        pst_cb->black_policy.cbs,
        pst_cb->black_meter);

    for (pst_ap = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_FIRST(pst_cb->apperc_tree); pst_ap != NULL;
         pst_ap = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_NEXT(pst_cb->apperc_tree, pst_ap->spm_defend_apperc_node))
    {
        mAclId = (index) + DEFEND_ACL_ID_APPRE_OFFSET;
        
        if(pst_ap->apperc_key.l3_protoc <= SIGNAL_NUM)
        {
            rule_id = g_qos_defend_offset[pst_ap->apperc_key.l3_protoc];
            num = g_qos_defend_num[pst_ap->apperc_key.l3_protoc];
            printf("apper acl id=%d,rule id=%d,num=%d\n",mAclId,rule_id,num);
            printf("l3_protoc=%s,prio=%d,queue_prio=%d,cir=%ld,cbs=%d,meter=%ld\n\n",
            defend_protcal_string[pst_ap->apperc_key.l3_protoc],
            pst_ap->apperc_key.prio,
            pst_ap->apperc_key.queue_prio,
            pst_ap->apperc_key.cir,
            pst_ap->apperc_key.cbs,
            pst_ap->meter_id);  
        }
        else
        {
            rule_id = DEFEND_DEFAULT_RULE_ID;
            num = 1;
            printf("apper acl id=%d,rule id=%d,num=%d\n",mAclId,rule_id,num);
            printf("l3_protoc=%s,prio=%d,queue_prio=%d,cir=%ld,cbs=%d,meter=%ld\n\n",
            "DEFAULT_RULE",
            pst_ap->apperc_key.prio,
            pst_ap->apperc_key.queue_prio,
            pst_ap->apperc_key.cir,
            pst_ap->apperc_key.cbs,
            pst_ap->meter_id);  
        }
              
    }

    for (pst_usr = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pst_cb->user_def_tree); pst_usr != NULL;
         pst_usr = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_NEXT(pst_cb->user_def_tree,pst_usr->spm_defend_user_def_node))
    {
        mAclId = (index) + DEFEND_ACL_ID_USR_OFFSET;
        rule_id = pst_usr->user_def_key.rule_id;
        printf("usr acl id=%d,rule id=%d\n",mAclId,rule_id);
        printf("action=%s,prio=%d,queue_prio=%d,l3protocol=%d,l3protocol_mask=0x%x\n "
        "port_dst=%d,port_dst_mask=0x%x,port_src=%d,port_src_mask=0x%x,cir=%ld,cbs=%d,meter=%ld\n",
           (pst_usr->user_def_key.action)?"拒绝":"通过",
            pst_usr->user_def_key.prio,
            pst_usr->user_def_key.queue_prio,
            pst_usr->user_def_key.l3protoco,
            pst_usr->user_def_key.l3protoco_mask,
            pst_usr->user_def_key.port_dst,
            pst_usr->user_def_key.port_dst_mask,
            pst_usr->user_def_key.port_src,
            pst_usr->user_def_key.port_src_mask,
            pst_usr->user_def_key.cir,
            pst_usr->user_def_key.cbs,
            pst_usr->meter_id); 
        if(0 == pst_usr->user_def_key.ip_type)
        {
           printf("dst ipv4 address mask_len=%d,dst ipv4 address:",pst_usr->user_def_key.dstip_mask_len);
           spm_print_ipv4(pst_usr->user_def_key.ip_dst[3]);
           printf("src ipv4 address mask_len=%d,src ipv4 address:",pst_usr->user_def_key.srcip_mask_len);
           spm_print_ipv4(pst_usr->user_def_key.ip_src[3]);
        }
        else
        {
           printf("dst ipv6 address mask_len=%d,dst ipv6 address:",pst_usr->user_def_key.dstip_mask_len);

           /* ipv4的地址存放在数组的最高位 */
           spm_print_ipv6(&(pst_usr->user_def_key.ip_dst[0]));
           printf("src ipv6 address mask_len=%d,src ipv6 address:",pst_usr->user_def_key.srcip_mask_len);
           spm_print_ipv6(&(pst_usr->user_def_key.ip_src[0]));
           printf("\n");
        }
    }

    mAclId = (index) + DEFEND_ACL_ID_USR_OFFSET;
    printf("abnormal_switch=%s,acl id=%d,start rule id=%d,rule num=%d\n\n",
        pst_cb->abnormal_switch.switch_flag?"开":"关",mAclId,
        DEFEND_ABNORMAL_RULE_OFFSET,DEFEND_ABNORMAL_RULE_NUM);

    printf("UDP switch=%s,acl id=%d,start rule id=%d,rule num=%d\n\n",
        pst_cb->udp_swtich.switch_flag?"开":"关",mAclId,
        DEFEND_UDP_RULE_OFFSET,DEFEND_UDP_RULE_NUM);

    printf("total_packet_signaling_bandwidth=%ld kpbs\n\n",pst_cb->total_pkt.pir);

    printf("start defend slot=");

    for(i = 1; i< MAX_SLOT_NUM + 1;i++)
    {        
        if(ATG_DCI_RC_OK != pst_cb->defend_slot[i])
        {
            printf(" %d",i);
        } 
    }
    printf("\n\n");
    return;
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
NBB_VOID  qos_show_defend_policy_usernum(NBB_ULONG index)
{
    SPM_QOS_DEFEND_POLICY_CB *pst_cb = NULL;
    SPM_QOS_DEFEND_USER_DEF_CB *pst_usr = NULL;
    NBB_ULONG count = 0;


    pst_cb = AVLL_FIND(g_qos_defend_policy_tree, &index);
    if(NULL == pst_cb)
    {
        printf("can't find defend policy cfg\n");
        return;
    }
    for (pst_usr = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pst_cb->user_def_tree); pst_usr != NULL;
         pst_usr = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_NEXT(pst_cb->user_def_tree,pst_usr->spm_defend_user_def_node))
    {
        count++;
    }
    printf("Total defend_policy_userdefind_num = %ld.\n\n",count);
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
NBB_VOID  qos_show_defend_policy_userdefind(NBB_ULONG index, NBB_ULONG RuleId)
{
    SPM_QOS_DEFEND_POLICY_CB *pst_cb = NULL;
    SPM_QOS_DEFEND_USER_DEF_CB *pst_usr = NULL;
    NBB_USHORT mAclId = 0;
    NBB_USHORT rule_id = 0;
    NBB_BYTE flag = 0;
    NBB_BYTE slot = 0;
    NBB_USHORT i = 0;


    pst_cb = AVLL_FIND(g_qos_defend_policy_tree, &index);

    if(NULL == pst_cb)
    {
        printf("can't find defend policy cfg\n");
        return;
    }
    slot = v_spm_shared->local_slot_id;
    flag = pst_cb->defend_slot[slot];
    printf("local_slot=%d,defend state=%s\n",slot,(flag)?"开":"关");
    for (pst_usr = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(pst_cb->user_def_tree); pst_usr != NULL;
         pst_usr = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_NEXT(pst_cb->user_def_tree,pst_usr->spm_defend_user_def_node))
    {
        if(RuleId == pst_usr->user_def_key.rule_id)
        {
            mAclId = (index) + DEFEND_ACL_ID_USR_OFFSET;
            rule_id = pst_usr->user_def_key.rule_id;
            printf("usr acl id=%d,rule id=%d\n",mAclId,rule_id);
            printf("action=%s,prio=%d,queue_prio=%d,l3protocol=%d,l3protocol_mask=0x%x\n "
            "port_dst=%d,port_dst_mask=0x%x,port_src=%d,port_src_mask=0x%x,cir=%ld,cbs=%d,meter=%ld\n",
               (pst_usr->user_def_key.action)?"拒绝":"通过",
                pst_usr->user_def_key.prio,
                pst_usr->user_def_key.queue_prio,
                pst_usr->user_def_key.l3protoco,
                pst_usr->user_def_key.l3protoco_mask,
                pst_usr->user_def_key.port_dst,
                pst_usr->user_def_key.port_dst_mask,
                pst_usr->user_def_key.port_src,
                pst_usr->user_def_key.port_src_mask,
                pst_usr->user_def_key.cir,
                pst_usr->user_def_key.cbs,
                pst_usr->meter_id); 
            if(0 == pst_usr->user_def_key.ip_type)
            {
               printf("dst ipv4 address mask_len=%d,dst ipv4 address:",pst_usr->user_def_key.dstip_mask_len);
               spm_print_ipv4(pst_usr->user_def_key.ip_dst[3]);
               printf("src ipv4 address mask_len=%d,src ipv4 address:",pst_usr->user_def_key.srcip_mask_len);
               spm_print_ipv4(pst_usr->user_def_key.ip_src[3]);
            }
            else
            {
               printf("dst ipv6 address mask_len=%d,dst ipv6 address:",pst_usr->user_def_key.dstip_mask_len);

               /* ipv4的地址存放在数组的最高位 */
               spm_print_ipv6(&(pst_usr->user_def_key.ip_dst[0]));
               printf("src ipv6 address mask_len=%d,src ipv6 address:",pst_usr->user_def_key.srcip_mask_len);
               spm_print_ipv6(&(pst_usr->user_def_key.ip_src[0]));
               printf("\n");
            }
        break;
        } 
    }
    printf("start defend slot=");
    for(i = 1; i< MAX_SLOT_NUM + 1;i++)
    {        
        if(ATG_DCI_RC_OK != pst_cb->defend_slot[i])
        {
            printf(" %d",i);
        } 
    }
    printf("\n\n");

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
NBB_VOID qos_defend_help()
{
    NBB_CHAR **ppc_msg;

    static NBB_CHAR *s_help_msg[] = {       
    "qos_show_defend_switch(slot)",                     "show slot defend policy index",
    "qos_show_defend_policy(index)",                    "show defend policy cfg",
    "qos_show_defend_policy_userdefind(index,RuleId)",  "show userdefine cfg of defend policy",
    "qos_show_defend_policy_userdefind(index)",         "show total number of userdefine cfg",
    NULL
    };

    printf("\n");

    for (ppc_msg = s_help_msg; *ppc_msg != NULL; ppc_msg += 2)
    {
        if (strlen(*(ppc_msg)) > 45)
        {
            printf(" %s %s\n", *ppc_msg, *(ppc_msg + 1));
        }
        else
        {
            printf(" %-40s %s\n", *ppc_msg, *(ppc_msg + 1));
        }
    }

    printf("\n");
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
NBB_VOID spm_cfg_defend_policy_cb_verify()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DEFEND_POLICY_CB *cfg_cb = NULL;
    SPM_QOS_DEFEND_APPERC_CB *pcb = NULL;
    SPM_QOS_DEFEND_USER_DEF_CB *npb = NULL;


    NBB_TRC_ENTRY("spm_cfg_defend_policy_cb_verify");

    for (cfg_cb = (SPM_QOS_DEFEND_POLICY_CB*) AVLL_FIRST(g_qos_defend_policy_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DEFEND_POLICY_CB*) AVLL_NEXT(g_qos_defend_policy_tree,
                       cfg_cb->spm_defend_policy_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify SPM_QOS_DEFEND_POLICY_CB %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_DEFEND_POLICY_CB), MEM_SPM_DEFEND_POLICY_CB);


        for (pcb = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_FIRST(cfg_cb->apperc_tree);
            pcb != NULL;
            pcb = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_NEXT(cfg_cb->apperc_tree, pcb->spm_defend_apperc_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify SPM_QOS_DEFEND_APPERC_CB %p", pcb));
            NBB_VERIFY_MEMORY(pcb, sizeof(SPM_QOS_DEFEND_APPERC_CB),MEM_SPM_DEFEND_APPERC_CB);
        }


        for (npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(cfg_cb->user_def_tree); npb != NULL;
             npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_NEXT(cfg_cb->user_def_tree, npb->spm_defend_user_def_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify SPM_QOS_DEFEND_USER_DEF_CB %p", npb));
            NBB_VERIFY_MEMORY(npb, sizeof(SPM_QOS_DEFEND_USER_DEF_CB),MEM_SPM_DEFEND_USR_DEF_CB);
        }
    }

    NBB_TRC_EXIT();

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
NBB_VOID spm_cfg_defend_cb_verify()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DEFEND_CB *cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_defend_cb_verify");

    for (cfg_cb = (SPM_QOS_DEFEND_CB*) AVLL_FIRST(g_qos_defend_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DEFEND_CB*) AVLL_NEXT(g_qos_defend_tree,
                       cfg_cb->spm_defend_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_DEFEND_CB), MEM_SPM_DEFEND_CB);
    }

    NBB_TRC_EXIT();

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
void spm_qos_defend_verify()
{
    spm_cfg_defend_policy_cb_verify();
    spm_cfg_defend_cb_verify();
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
void spm_qos_clear_defend_cfg()
{
    spm_qos_clear_all_defend_policy();
    spm_qos_clear_all_defend_switch();
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
void spm_qos_defend_init()
{
    NBB_BUF_SIZE avll_key_offset;
    NBB_ULONG i = 0;

    /* 防攻击树初始化 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DEFEND_POLICY_CB, policy_key);
    AVLL_INIT_TREE(g_qos_defend_policy_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DEFEND_POLICY_CB,spm_defend_policy_node));

    /* 存放防攻击开关配置 */
    avll_key_offset = NBB_OFFSETOF(SPM_QOS_DEFEND_CB, key);
    AVLL_INIT_TREE(g_qos_defend_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_QOS_DEFEND_CB,spm_defend_node));

    /**********防攻击初始化***********/
    OS_MEMSET(&(g_qos_defend_num[0]),1,SIGNAL_NUM + 1);
    g_qos_defend_num[0] = 0;
    g_qos_defend_num[DEFEND_LDPUDP] = 2;
    g_qos_defend_num[DEFEND_LDP] = 4;
    g_qos_defend_num[DEFEND_BGP] = 2;
    g_qos_defend_num[DEFEND_BGPV6] = 2;
    g_qos_defend_num[DEFEND_DHCP] = 2;
    g_qos_defend_num[DEFEND_DHCPV6] = 2;
    g_qos_defend_num[DEFEND_SNMP] = 2;
    g_qos_defend_num[DEFEND_FTP_S] = 4;
    g_qos_defend_num[DEFEND_FTP_C] = 4;
    g_qos_defend_num[DEFEND_TELNET_S] = 6;
    g_qos_defend_num[DEFEND_TELNET_C] = 6;
    g_qos_defend_num[DEFEND_SSH_S] = 4;
    g_qos_defend_num[DEFEND_SSH_C] = 4;
    g_qos_defend_num[DEFEND_MLD] = 4;
    g_qos_defend_num[DEFEND_TCPSYN] = 2;  
    g_qos_defend_num[DEFEND_TACACS] = 4;
    g_qos_defend_num[DEFEND_RADIUS] = 2;
    
    g_qos_defend_offset[0] = 1;
    for(i = DEFEND_OSPF; i < SIGNAL_NUM + 1;i++)
    {
        g_qos_defend_offset[i] = g_qos_defend_offset[i - 1] + g_qos_defend_num[i - 1];
    }
}

#endif

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
NBB_LONG spm_attack_set_recordswitch(NBB_ULONG oper, AGGRESSIVITY_SUB_CFG_ONE *ptemp)
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
        if(g_qos_defend_cfg_print != ATG_DCI_RC_OK)
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
                                __FUNCTION__,__LINE__,ret);
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
NBB_LONG spm_attack_set_sampleratio(NBB_ULONG oper, AGGRESSIVITY_SUB_CFG_TWO *ptemp)
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
        if(g_qos_defend_cfg_print != ATG_DCI_RC_OK)
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
                                __FUNCTION__,__LINE__,ret);
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
NBB_LONG spm_attack_set_alarmthreshold(NBB_ULONG oper, AGGRESSIVITY_SUB_CFG_THREE *ptemp)
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
        if(g_qos_defend_cfg_print != ATG_DCI_RC_OK)
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
                                __FUNCTION__,__LINE__,ret);
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
NBB_VOID spm_rcv_dci_set_slot_attact(ATG_DCI_SET_AGGRESSIVITY *pst_setips)
{

    /***************************************************************************/
    /* Local Variables                                    */
    /***************************************************************************/  
    NBB_ULONG slot_id = 0;
    NBB_BYTE unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *puc_samplerecord_datastart = NULL;
    NBB_BYTE *puc_sampleratio_datastart = NULL;
    NBB_BYTE *puc_alarmthreshold_datastart = NULL;

    /* 无基本配置 */
    NBB_ULONG ul_oper_samplerecord = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_opersampleratio = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_alarmthreshold = ATG_DCI_OPER_NULL;

    AGGRESSIVITY_SUB_CFG_ONE *pst_samplerecord = NULL;
    AGGRESSIVITY_SUB_CFG_TWO *pst_sampleratio = NULL;
    AGGRESSIVITY_SUB_CFG_THREE *pst_alarmthreshold = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

 

    if(NULL == pst_setips)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        spm_qos_param_error_log(__FUNCTION__,__LINE__);
        goto EXIT_LABEL;
    }

     /*获取本槽位ID*/
    slot_id = SHARED.local_slot_id;

    if(slot_id != pst_setips->key.slot_id)
    {
        goto EXIT_LABEL;   
    }

    /* 首先将IPS消息的返回值设置为OK,如果有一个子配置失败,则置为FAIL */
    pst_setips->return_code = ATG_DCI_RC_OK;

    /* 获取配置的操作码 */
    ul_oper_samplerecord = pst_setips->oper_sub_one;
    ul_opersampleratio = pst_setips->oper_sub_two;
    ul_oper_alarmthreshold = pst_setips->oper_sub_three;

    /* 获取配置的偏移地址 */
    puc_samplerecord_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->sub_one_data);
    puc_sampleratio_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->sub_two_data);
    puc_alarmthreshold_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->sub_three_data);


    pst_samplerecord = (AGGRESSIVITY_SUB_CFG_ONE*)puc_samplerecord_datastart;
    pst_sampleratio = (AGGRESSIVITY_SUB_CFG_TWO*)puc_sampleratio_datastart;
    pst_alarmthreshold = (AGGRESSIVITY_SUB_CFG_THREE*)puc_alarmthreshold_datastart;


    /* ips消息类型为删除 */
    if (TRUE == pst_setips->delete_struct)
    {
        for(unit = 0;unit < SHARED.c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = fhdrv_psn_set_slot_attack_sampler(unit,0,0); 
            if(ATG_DCI_RC_OK != ret)
            {
                pst_setips->return_code = ret;
                spm_api_c3_set_slot_attact_error_log(unit,0,0,
                                                    __FUNCTION__,__LINE__,ret);


            }
#endif
        }
        
        ret = almpm_set_attack_alm_threshold(0,0,0);
        if(ATG_DCI_RC_OK != ret)
        {
        
            pst_setips->return_code = ret;
            spm_hqos_almpm_attack_error_log(0,0,0,
                                __FUNCTION__,__LINE__,ret);

        }
    }
    else
    {
        /*采样记录开关*/
        ret = spm_attack_set_recordswitch(ul_oper_samplerecord,pst_samplerecord);
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->sub_one_return_code = ret;
        }

        /*采样比*/
        ret = spm_attack_set_sampleratio(ul_opersampleratio,pst_sampleratio);
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->sub_two_return_code = ret;
        }


        /*告警阈值*/
        ret = spm_attack_set_alarmthreshold(ul_oper_alarmthreshold,pst_alarmthreshold);
        if(ATG_DCI_RC_OK != ret)
        {
            pst_setips->return_code = ret;
            pst_setips->sub_three_return_code = ret;
        }
        
    }


    
    /* 异常跳出 */
    EXIT_LABEL:NBB_TRC_EXIT();
    return;

}

#endif

