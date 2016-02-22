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


#include <nbase.h>
#include <spmincl.h>
#include <TmApi.h>


/* 全局打印变量qos_phy_cfg_print */
NBB_BYTE qos_phy_cfg_print = ATG_DCI_RC_OK;

/*****************************************************************************
   函 数 名  : spm_qos_logic_flow_key_compare
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
NBB_INT spm_qos_logic_flow_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_FLOW_KEY *key1 = (SPM_QOS_LOGIC_FLOW_KEY *)keyarg1;
    SPM_QOS_LOGIC_FLOW_KEY *key2 = (SPM_QOS_LOGIC_FLOW_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&key1->index, &key2->index NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->svlan, &key2->svlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_ushort(&key1->cvlan, &key2->cvlan NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_qos_logic_flow_key_compare
   功能描述  : 逻辑端口流树的比较函数
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
NBB_INT spm_qos_port_wred_key_compare(NBB_VOID *keyarg1, 
    NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_PORT_WRED_KEY *key1 = (SPM_PORT_WRED_KEY *)keyarg1;
    SPM_PORT_WRED_KEY *key2 = (SPM_PORT_WRED_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&key1->index, &key2->index NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }
    ret = compare_byte(&key1->cos, &key2->cos NBB_CCXT);
    if (0 != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_qos_logic_key_compare
   功能描述  : LSP_TX树的比较函数
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
NBB_INT spm_qos_lsp_tx_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    CRTXLSP_KEY *key1 = (CRTXLSP_KEY *)keyarg1;
    CRTXLSP_KEY *key2 = (CRTXLSP_KEY *)keyarg2;
    NBB_INT rv = 0;

    rv = compare_ulong(&key1->ingress, &key2->ingress NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->egress, &key2->egress NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->tunnelid, &key2->tunnelid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->lspid, &key2->lspid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return(rv);
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
NBB_INT spm_qos_logic_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_LOGIC_INTF_KEY *key1 = (SPM_QOS_LOGIC_INTF_KEY *)keyarg1;
    SPM_QOS_LOGIC_INTF_KEY *key2 = (SPM_QOS_LOGIC_INTF_KEY *)keyarg2;
    NBB_INT ret = 0;

    ret = compare_ulong(&key1->index, &key2->index);
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

#if 1




/*****************************************************************************
   函 数 名  : spm_qos_init
   功能描述  : qos相关树的初始化
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
void spm_qos_init()
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY(__FUNCTION__);
    spm_qos_policy_init();
    spm_qos_acl_init();
    spm_qos_behavior_init();
    spm_qos_classify_init();
    spm_qos_defend_init();



    //EXIT_LABEL: 
    NBB_TRC_EXIT();
    return;
}




/*****************************************************************************
   函 数 名  : spm_qos_voq_init
   功能描述  : arad驱动默认全连接关系的初始化函数
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
NBB_LONG spm_qos_voq_init()
{
    NBB_LONG slot = 0;
    NBB_LONG portnum = 0;
    NBB_BYTE fapid = 0; /* 本槽位 */
    NBB_LONG baseQueue = 0;
    NBB_LONG baseVCId = 0;
    NBB_INT rv = 0;
    
    //NBB_USHORT i = 0;
    //NBB_ULONG buf_size = 0;

    NBB_ULONG offset = MAX_SLOT_NUM * NUM_COS;

    /* 获取单盘所在的槽位号 */
    spm_hardw_getslot(&fapid);
    fapid--;

    /********************************************************************************/ 
    /* 在本盘建立各端口到全部出口槽位(包括本槽位)的VOQ连接关系,即建立入口的ITM功能; */
    /* 此时本盘将会作为业务的入口盘,业务可能从本盘的任意端口中进来,所有其他盘(包括  */
    /* 本盘)都有可能作为业务的出口盘.                                               */
    /********************************************************************************/
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
        baseQueue = offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        baseVCId = fapid * NUM_COS + offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;

        for (slot = 0; slot < MAX_SLOT_NUM; slot++) /* R8000中的arad最多支持32个槽位 */
        {
            if (fapid == slot)  /* 本槽位不存在端口不建立VOQ */
            {
#ifdef PTN690 
                if(PTN_690_PORT_OFFSET == portnum)
                {
#if defined (SPU) || defined (SRC)
                    ApiAradCheckPortValid(0, 0, &rv);
                    if (0 == rv)
                    {
                        baseQueue += NUM_COS;
                        continue;
                    }
#endif
                }
                else
                {
#if defined (SPU) || defined (SRC)
                    ApiAradCheckPortValid(0, portnum, &rv);
                    if (0 == rv)
                    {
                        baseQueue += NUM_COS;
                        continue;
                    }
#endif
                }
#else 
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, portnum, &rv);
                if (0 == rv)
                {
                    baseQueue += NUM_COS;
                    continue;
                }
#endif
#endif
            }
#ifdef PTN690
            if(PTN_690_PORT_OFFSET == portnum) /* ARAD cpu端口号 */
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupItm(0, baseQueue, NUM_COS, slot + 1, 0, baseVCId);
                if (rv != 0)
                {
                    printf("Error! ApiSetupItm err: %d (baseQueue=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                    goto EXIT_LABEL;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupItm(0, baseQueue, NUM_COS, slot + 1, portnum, baseVCId);
                if (rv != 0)
                {
                    printf("Error! ApiSetupItm err: %d (baseQueue=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                    goto EXIT_LABEL;
                }
#endif
            }
#else
#if defined (SPU) || defined (SRC)
            rv = ApiSetupItm(0, baseQueue, NUM_COS, slot + 1, portnum, baseVCId);
            if (rv != 0)
            {
                printf("Error! ApiSetupItm err: %d (baseQueue=%ld,slot=%ld,port=%ld,"
                       "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                goto EXIT_LABEL;
            }
#endif
#endif  

#if 0       /*补丁程序，修改voq的默认队列缓存值*/
            for(i = 0; i < 5;i++)
            {   

                buf_size = 1*1024*1024;
 
#ifdef PTN690
                if(PTN_690_PORT_OFFSET == portnum) /* ARAD cpu端口号 */
                {
#if defined (SPU) || defined (SRC)
                    rv = ApiAradSetQueueTailDrop(0, baseQueue, i, -1, buf_size);
#endif
                }
                else
                {
#if defined (SPU) || defined (SRC)
                    rv = ApiAradSetQueueTailDrop(0, baseQueue, i, -1, buf_size);
#endif
                }
#else
#if defined (SPU) || defined (SRC)
                rv = ApiAradSetQueueTailDrop(0, baseQueue, i, -1, buf_size);
#endif
#endif
                if (rv != 0)
                {
                    printf("Error! ApiAradSetQueueTailDrop err: %d (baseQueue=%ld,cos=%ld)\n", rv, baseQueue, i);
                    goto EXIT_LABEL;
                }
            }
#endif
            baseQueue += NUM_COS;
        }
    }

    /* 初始化端口:为子输出端口建立Enhanced HR和FQ(全SP) */
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
#ifdef PTN690 
        if(PTN_690_PORT_OFFSET == portnum)
        {
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, 0, &rv);
            if (0 == rv)
            {
                continue;
            }
#endif
        }
        else
        {
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, portnum, &rv);
            if (0 == rv)
            {
                continue;
            }
#endif
        }
#else 
#if defined (SPU) || defined (SRC)
        ApiAradCheckPortValid(0, portnum, &rv);
        if (0 == rv)
        {
            continue;
        }
#endif
#endif
#ifdef PTN690
        if(PTN_690_PORT_OFFSET == portnum)
        {
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseSeEtm(0, 0);
            if (rv != 0)
            {
                printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
                goto EXIT_LABEL;
            }
#endif
        }
        else
        {
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseSeEtm(0, portnum);
            if (rv != 0)
            {
                printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
                goto EXIT_LABEL;
            }
#endif
        }
#else
#if defined (SPU) || defined (SRC)
        rv = ApiSetupBaseSeEtm(0, portnum);
        if (rv != 0)
        {
            printf("Error! ApiSetupBaseSeEtm err: %d(port=%ld)\n", rv, portnum);
            goto EXIT_LABEL;
        }
#endif
#endif
    }

    /************************************************************************/ 
    /* 建立全部入口盘到本盘各端口到的VC及本盘各出端口与入口盘VOQ之间的关联; */
    /* 此时本盘将会作为业务的出口盘，所有盘(包括本盘)都可能作为业务的入口盘.*/
    /************************************************************************/
    for (portnum = MIN_PHYSIC_PORT; portnum < MAX_PHYSIC_PORT; portnum++)
    {
        baseQueue = fapid * NUM_COS + offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        baseVCId = offset * (portnum - PTN_690_PORT_OFFSET) + VOQ_OFFSET;
        for (slot = 0; slot < MAX_SLOT_NUM; slot++)
        {
#ifdef PTN690 
            if(PTN_690_PORT_OFFSET == portnum)
            {
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, 0, &rv);
                if (0 == rv)
                {
                    baseVCId += NUM_COS;
                    continue;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                ApiAradCheckPortValid(0, portnum, &rv);
                if (0 == rv)
                {
                    baseVCId += NUM_COS;
                    continue;
                }
#endif
            }
#else 
#if defined (SPU) || defined (SRC)
            ApiAradCheckPortValid(0, portnum, &rv);
            if (0 == rv)
            {
                baseVCId += NUM_COS;
                continue;
            }
#endif
#endif
#ifdef PTN690
            if(PTN_690_PORT_OFFSET == portnum) /* arad CPU端口号 */
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupBaseEtm(0, baseQueue, NUM_COS, slot + 1, 0, baseVCId);
                if (rv != 0)    
                {
                    printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                    goto EXIT_LABEL;
                }
#endif
            }
            else
            {
#if defined (SPU) || defined (SRC)
                rv = ApiSetupBaseEtm(0, baseQueue, NUM_COS, slot + 1, portnum, baseVCId);
                if (rv != 0)    
                {
                    printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                           "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                    goto EXIT_LABEL;
                }
#endif
            }
#else
#if defined (SPU) || defined (SRC)
            rv = ApiSetupBaseEtm(0, baseQueue, NUM_COS, slot + 1, portnum, baseVCId);
            if (rv != 0)    
            {
                printf("Error! ApiSetupBaseEtm err: %d(voq=%ld,slot=%ld,port=%ld,"
                       "vc=%ld)\n", rv,baseQueue, slot + 1, portnum, baseVCId);
                goto EXIT_LABEL;
            }
#endif
#endif
            baseVCId += NUM_COS;
        }
    }

#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return rv;
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
NBB_LONG spm_init_hqos_lsp()
{
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (SRC)
    NBB_INT  rv  = ATG_DCI_RC_OK;
#endif
    NBB_ULONG i;

    for(i = MIN_PHYSIC_PORT; i < MAX_PHYSIC_PORT; i++)
    {
#if defined (SPU) || defined (SRC)
        ApiAradCheckPortValid(0, i, &rv);
        if (0 == rv)
        {
            continue;
        }
#endif
#ifdef PTN690
        if(PTN_690_PORT_OFFSET < i)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosLspCreate(UNIT_0, i, i - MIN_PHYSIC_PORT + 1);
            if (ATG_DCI_RC_OK != ret)
            {
                printf("**QOS ERROR**%s,line=%d,port=%ld,lsp id=%ld\n",
                          __FUNCTION__,__LINE__,i,i - MIN_PHYSIC_PORT + 1);
                return ret;
            }
#endif
        }
#else
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspCreate(UNIT_0, i, i - MIN_PHYSIC_PORT + 1);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("**QOS ERROR**%s,line=%d,port=%ld,lsp id=%ld\n",
                      __FUNCTION__,__LINE__,i,i - MIN_PHYSIC_PORT + 1);
            return ret; 
        }
#endif
#endif
    }
    
    return ret;
}


/*SPU盘调用*/
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
NBB_LONG spm_init_rcu_cir_pir()
{
    NBB_LONG ret = 0;
#if defined (SPU) || defined (SRC)
    NBB_LONG cir[NUM_COS] = {390,390,2730,390,1560,3719,4950,1900};
    NBB_LONG pir[NUM_COS] = {10000,390,10000,8000,16000,16000,16000,16000};
#endif
    NBB_LONG i = 0;

#ifdef PTN690
#if defined (SPU) || defined (SRC)
    ret = ApiAradSetOFPBandWidth(0,QOS_RCU_PORT, 4 * 1024);
    if(0 != ret)
    {
        printf("set rcu port bandwidth = 4M EROOR\n");
        goto EXIT_LABEL;
    }
#endif
#else
#if defined (SPU) || defined (SRC)
    ret = ApiAradSetOFPBandWidth(0,QOS_RCU_PORT, 16 * 1024);
    if(0 != ret)
    {
        printf("set rcu port bandwidth = 16M EROOR\n");
        goto EXIT_LABEL;
    }
#endif
#endif
    for(i = 0; i < NUM_COS;i++)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueCirEir(0,QOS_RCU_PORT,i,cir[i],pir[i] - cir[i],1024,1024);
        if(0 != ret)
        {
            printf("set rcu cos = %ld cir eir EROOR\n",i);
            goto EXIT_LABEL;
        }
#endif
    }

    //ret = spm_init_hqos_lsp();

#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return ret;
}

/*SRC盘调用*/
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
NBB_LONG spm_init_arad_cpu_cir_pir()
{
    NBB_LONG ret = 0;
    NBB_LONG i = 0;
 
    for(i = 0; i < NUM_COS;i++)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueCirEir(0,0,i,0,6 * 1024,1024,1024);
        if(0 != ret)
        {
            printf("set cpu cos =%ld eir 6M EROOR=%ld\n",i,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    //ret = spm_init_hqos_lsp();
#if defined (SPU) || defined (SRC)
    EXIT_LABEL:
#endif
    return ret;
}

#endif




#if 5

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
NBB_LONG spm_set_ipv4_sampler(SUB_PORT *sub_port, ATG_DCI_PHY_PORT_IPNF_SAMPLER *cfg)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_ULONG cntId = 0;
    NBB_USHORT port_id = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_ipv4_sampler");

    if((NULL == cfg) || (NULL == sub_port))
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS ipv4_sampler",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                  "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                  "eg_mode=%s,eg_sample_ratio=%d\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  (cfg->in_enable_flag)?"enable":"disable",
                  (cfg->in_mode)?"fix":"random",cfg->in_sample_ratio,
                  (cfg->eg_enable_flag)?"enable":"disable",
                  (cfg->eg_mode)?"fix":"random",cfg->eg_sample_ratio);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                   "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                   "eg_mode=%s,eg_sample_ratio=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   (cfg->in_enable_flag)?"enable":"disable",
                   (cfg->in_mode)?"fix":"random",cfg->in_sample_ratio,
                   (cfg->eg_enable_flag)?"enable":"disable",
                   (cfg->eg_mode)?"fix":"random",cfg->eg_sample_ratio);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if((0 != sub_port->slot) && (sub_port->slot != v_spm_shared->local_slot_id))
    {
        goto EXIT_LABEL;
    }
    if ((sub_port->port&0x60) != 0)
    {
        port_id = sub_port->port&0x1f;
    }
    else
    {
        port_id = ((sub_port->port&0x60)*(sub_port->port&0x1f));
    }

    cntId = (sub_port->slot - 1) * 50 + port_id + 1024;

#if defined (SPU) || defined (PTN690_CES)
    rv = ApiC3SetFlowSample(sub_port->unit,sub_port->port, 0, cfg->in_enable_flag, cfg->in_sample_ratio,cntId);
#endif  
    if(ATG_DCI_RC_OK != rv)
    {
        printf("%s %d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",
                  __FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    				   "QOS ipv4_sampler",rv,"phyPort","unit","in_enable_flag","in_sample_ratio",
    				   sub_port->port,sub_port->unit,cfg->in_enable_flag,cfg->in_sample_ratio));
        OS_SPRINTF(ucMessage,"%s %s,%d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = rv;
    }
     
#if defined (SPU) || defined (PTN690_CES)
    rv = ApiC3SetFlowSample(sub_port->unit,sub_port->port, 1, cfg->eg_enable_flag, cfg->eg_sample_ratio,cntId);
#endif
    if(ATG_DCI_RC_OK != rv)
    {
        printf("EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",sub_port->unit,rv,sub_port->port);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    				   "QOS ipv4_sampler",rv,"phyPort","unit","eg_enable_flag","eg_sample_ratio",
    				   sub_port->port,sub_port->unit,cfg->in_enable_flag,cfg->in_sample_ratio));
        OS_SPRINTF(ucMessage,"%s %s,%d EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__,sub_port->unit,rv,sub_port->port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = rv;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
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
NBB_LONG spm_set_lag_sampler(SUB_PORT *sub_port, ATG_DCI_LAG_IPNF_SAMPLER_DATA *cfg)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_ULONG cntId = 0;
    NBB_USHORT port_id = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY("spm_set_lag_sampler");

    if(NULL == cfg)
    {
        printf("**QOS ERROR**%s,%d,cfg==null\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS lag_sampler",ATG_DCI_RC_UNSUCCESSFUL,
					   "phyPort","cfg==null","","",sub_port->port,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,LINE=%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                  "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                  "eg_mode=%s,eg_sample_ratio=%d\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  (cfg->ingress_flag)?"enable":"disable",
                  (cfg->ingress_mode)?"fix":"random",cfg->ingress_sampling,
                  (cfg->engress_flag)?"enable":"disable",
                  (cfg->engress_mode)?"fix":"random",cfg->engress_sampling);
        OS_SPRINTF(ucMessage,"%s %s,LINE=%d slot=%d,port=%d,ovlan=%d,in_enable_flag=%s\n"
                   "in_mode=%s,in_sample_ratio=%d,eg_enable_flag=%s\n"
                   "eg_mode=%s,eg_sample_ratio=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   (cfg->ingress_flag)?"enable":"disable",
                   (cfg->ingress_mode)?"fix":"random",cfg->ingress_sampling,
                   (cfg->engress_flag)?"enable":"disable",
                   (cfg->engress_mode)?"fix":"random",cfg->engress_sampling);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if((sub_port->slot != 0) || (sub_port->port < 0x80))
    {
        printf("%s %s,LINE=%d,spm_set_lag_sampler ERROR\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__);
        OS_SPRINTF(ucMessage,"%s %s,LINE=%d,spm_set_lag_sampler ERROR\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if ((sub_port->port&0x60) != 0)
    {
        port_id = sub_port->port&0x1f;
    }
    else
    {
        port_id = ((sub_port->port&0x60) * (sub_port->port&0x1f));
    }
    
    cntId = (sub_port->slot - 1) * 50 + port_id + 1024;
    for(unit = 0;unit < SHARED.c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        rv = ApiC3SetFlowSample(unit,sub_port->port, 0, cfg->ingress_flag, cfg->ingress_sampling,cntId);
#endif
        if(ATG_DCI_RC_OK != rv)
        {
            printf("ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",unit,rv,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				   "QOS ipv6_sampler",rv,"phyPort","unit","in_enable_flag","in_sample_ratio",
        				   sub_port->port,unit,cfg->ingress_flag,cfg->ingress_sampling));
            OS_SPRINTF(ucMessage,"%s %s,%d ING ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,unit,rv,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = rv;
        }
       
#if defined (SPU) || defined (PTN690_CES)
        rv = ApiC3SetFlowSample(unit,sub_port->port, 1, cfg->engress_flag, cfg->engress_sampling,cntId);
#endif
        if(ATG_DCI_RC_OK != rv)
        {
            printf("EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n",unit,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				   "QOS ipv6_sampler",rv,"phyPort","unit","eg_enable_flag","eg_sample_ratio",
        				   sub_port->port,unit,cfg->engress_flag,cfg->engress_sampling));
            OS_SPRINTF(ucMessage,"%s %s,%d EGR ApiC3SetFlowSample unit=%d, ret=%ld,port=%d\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,unit,rv,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            ret = rv;
        } 
    }

    EXIT_LABEL: NBB_TRC_EXIT();
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
NBB_LONG spm_set_port_flood_control(SUB_PORT *sub_port,
    NBB_BYTE bc_swtich,
    NBB_BYTE mc_swtich,
    NBB_BYTE fd_swtich,
    NBB_ULONG bc_bandwith,
    NBB_ULONG mc_bandwith,
    NBB_ULONG fd_bandwith)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
    METER_CFG_T meter = {0};
    
    /*驱动讨论后默认填写的值出问题mantis给驱动*/
    NBB_ULONG default_cbs = 2048;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_cir_pir");

    if (NULL == sub_port)
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "%s,%s,%s\n"
                  "bc_bandwith=%ld,mc_bandwith=%ld,fd_bandwith=%ld\n\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port,sub_port->ovlan,
                  bc_swtich?"广播抑制开":"广播抑制关",
                  mc_swtich?"组播抑制开":"组播抑制关",
                  fd_swtich?"洪泛抑制开":"洪泛抑制关",
                  bc_bandwith,mc_bandwith,fd_bandwith);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "%s,%s,%s\n"
                   "bc_bandwith=%ld,mc_bandwith=%ld,fd_bandwith=%ld\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port,sub_port->ovlan,
                   bc_swtich?"广播抑制开":"广播抑制关",
                   mc_swtich?"组播抑制开":"组播抑制关",
                   fd_swtich?"洪泛抑制开":"洪泛抑制关",
                   bc_bandwith,mc_bandwith,fd_bandwith);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    if (bc_swtich + mc_swtich + fd_swtich)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFCEnable(sub_port->unit, sub_port->port, 1);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3;
        meter.eMeterType = RFC_2697;
        meter.cir = bc_bandwith;
        meter.pireir = bc_bandwith;
        meter.cbs = default_cbs;
        meter.pbsebs = default_cbs;
        meter.dropRed = 1;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meterId","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, BC_CONTROL, bc_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3 + 1;
        meter.cir = mc_bandwith;
        meter.pireir = mc_bandwith;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meterId","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#endif

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, MC_CONTROL, mc_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meter id","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }

        meter.meterId = sub_port->port * 3 + 2;
        meter.cir = fd_bandwith;
        meter.pireir = fd_bandwith;

#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetMeter(sub_port->unit, &meter);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d meterId=%ld ApiC3SetMeter\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port,meter.meterId);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFC(sub_port->unit, sub_port->port, UC_CONTROL, fd_swtich, meter.meterId);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__,ret,sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,"port","unit","meter id","",
						   sub_port->port,sub_port->unit,meter.meterId,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFC\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
    }
    else
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPortFCEnable(sub_port->unit, sub_port->port, 0);
#endif
        if (ATG_DCI_RC_OK != ret)
        {
            printf("%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n",
                      QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						   "QOS port flood",ret,
						   "port","unit","ApiC3SetPortFCEnable","",
						   sub_port->port,sub_port->unit,0,0));
            OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d ApiC3SetPortFCEnable\n\n",
                       QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, sub_port->port);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();
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
NBB_LONG spm_set_port_cir_pir(SUB_PORT *sub_port, NBB_LONG cos,
    ATG_DCI_PHY_FLOW_SHAPE_QUEUE *shape)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
#if defined (SPU) || defined (SRC)
    NBB_LONG unit = 0;
#endif
    NBB_LONG eir;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_cir_pir");

    if ((NULL == sub_port) || (NULL == shape) || (cos > 7))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",
						0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "cos=%ld,cir=%ld,pir=%ld,cbs=%d,pbs=%d\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                  cos,shape->cir,shape->pir,shape->cbs,shape->pbs);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "cos=%ld,cir=%ld,pir=%ld,cbs=%d,pbs=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                   cos,shape->cir,shape->pir,shape->cbs,shape->pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }
    if (shape->pir < shape->cir)
    {
        printf("**QOS ERROR**%s,%d,pir<cir\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "pir","cir","port","cos",
					   shape->pir,shape->cir,sub_port->port,cos));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if(25 * 100 * 1000 == shape->pir)
    {
        shape->pir += 10000;
    }

    eir = shape->pir - shape->cir;

#if defined (SPU) || defined (SRC)
    ret = ApiAradSetPortQueueCirEir(unit, sub_port->port + PTN_690_PORT_OFFSET,
                            cos, shape->cir, eir, shape->cbs, shape->pbs);
#endif
    if (ATG_DCI_RC_OK != ret)
    {
        printf("%s %s,%d ret=%ld port=%d cos=%ld ApiAradSetPortQueueCirEir\n",
                  QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                  sub_port->port + PTN_690_PORT_OFFSET,cos);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS port cir pir",ret,"slot","port","cos","",
					   sub_port->slot,sub_port->port + PTN_690_PORT_OFFSET,cos,0));
        OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d cos=%ld ApiAradSetPortQueueCirEir\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                   sub_port->port + PTN_690_PORT_OFFSET,cos);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
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
NBB_LONG spm_set_port_sp_wfq(SUB_PORT *sub_port, NBB_BYTE cos, 
    ATG_DCI_PHY_PORT_SCHEDULE_QUEUE *port_schedule)
{
    NBB_LONG ret = ATG_DCI_RC_OK;
#if defined (SPU) || defined (SRC)
    NBB_LONG unit = 0;
#endif
    NBB_BYTE fapid = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_set_port_sp_wfq");

    if ((NULL == sub_port) || (NULL == port_schedule) || (cos > 7))
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
					   "","","","",0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_phy_cfg_print)
    {
        printf("%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                  "cos=%d,mode=%s,weight=%d\n",
                  QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                  sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                  cos,(port_schedule->mode)?"WFQ":"SP",port_schedule->weight);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%d,ovlan=%d\n"
                   "cos=%d,mode=%s,weight=%d\n\n",
                   QOS_CFG_STRING,__FUNCTION__,__LINE__,sub_port->slot,
                   sub_port->port + PTN_690_PORT_OFFSET,sub_port->ovlan,
                   cos,(port_schedule->mode)?"WFQ":"SP",port_schedule->weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }

    spm_hardw_getslot(&fapid);
    if (fapid != sub_port->slot)
    {
        goto EXIT_LABEL;
    }

    if (0 == port_schedule->mode)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueSP(unit, sub_port->port + PTN_690_PORT_OFFSET, cos, 7 - cos);
#endif
    }
    else
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetPortQueueWfqWeight(unit, sub_port->port + PTN_690_PORT_OFFSET, cos, port_schedule->weight);
#endif
    }
    if (ATG_DCI_RC_OK != ret)
    {
        printf("%s %s,%d ret=%ld port=%d cos=%d mode=%d set sp or wfq err\n",
                  QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                  sub_port->port + PTN_690_PORT_OFFSET,cos,port_schedule->mode);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					   "QOS port sp wfq",ret,"slot","port","cos","mode",
					   sub_port->slot,sub_port->port,cos,port_schedule->mode));
        OS_SPRINTF(ucMessage,"%s %s,%d ret=%ld port=%d cos=%d mode=%d set sp or wfq err\n\n",
                   QOS_ERROR_STRING,__FUNCTION__,__LINE__, ret, 
                   sub_port->port + PTN_690_PORT_OFFSET,cos,port_schedule->mode);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}





#endif

#if 6

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
void spm_qos_clear_all_cfg()
{
    spm_qos_clear_defend_cfg();
    spm_qos_defend_init();
}

#endif

