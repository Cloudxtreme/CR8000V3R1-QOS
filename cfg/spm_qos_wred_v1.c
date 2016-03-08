/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_hqos_slot.c
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

/*WRED模块全局打印开关*/
NBB_BYTE g_qos_wred_print = ATG_DCI_RC_OK;

/*WRED模块全局打印开关*/
NBB_BYTE g_qos_wred_log = ATG_DCI_RC_OK;

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
static NBB_VOID  spm_api_set_wred_profile_err(NBB_LONG unit, NBB_LONG dp, NBB_LONG profile_id, 
        NBB_LONG max_th, NBB_LONG min_th, NBB_LONG max_prob,const NBB_CHAR *function,
        NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  fhdrv_fap_qos_set_wred_profile ret=%d:\n"
                  "unit = %d, dp = %d,profile_id = %d, min_th = %d,max_th=%d,max_prob=%d\n\n",
                  function,line,ret,unit,dp,profile_id,min_th,max_th,max_prob);
        OS_SPRINTF(uc_message,"%s line = %ld  fhdrv_fap_qos_set_wred_profile ret=%d:\n"
                  "unit = %d, dp = %d,profile_id = %d, min_th = %d,max_th=%d,max_prob=%d\n\n",
                  function,line,ret,unit,dp,profile_id,min_th,max_th,max_prob);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }  
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
static NBB_VOID  spm_api_del_wred_profile_err(NBB_LONG unit,  NBB_LONG profile_id, 
        const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  fhdrv_fap_qos_delete_wred_profile ret=%d:\n"
                  "unit = %d,profile_id = %d\n\n",
                  function,line,ret,unit,profile_id);
        OS_SPRINTF(uc_message,"%s line = %ld  fhdrv_fap_qos_delete_wred_profile ret=%d:\n"
                  "unit = %d,profile_id = %d\n\n",
                  function,line,ret,unit,profile_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }  
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
static NBB_VOID  spm_api_set_que_wred_err(NBB_LONG unit,NBB_LONG queue_id,
        NBB_LONG cos, NBB_LONG profile_id, 
        const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  fhdrv_fap_qos_set_queue_wred ret=%d:\n"
                  "unit = %d,queue_id = %d,cos=%d,profile_id = %d\n\n",
                  function,line,ret,unit,queue_id,cos,profile_id);
        OS_SPRINTF(uc_message,"%s line = %ld  fhdrv_fap_qos_set_queue_wred ret=%d:\n"
                  "unit = %d,queue_id = %d,cos=%d,profile_id = %d\n\n",
                  function,line,ret,unit,queue_id,cos,profile_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }  
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
static NBB_VOID  spm_api_set_que_tail_drop_err(NBB_LONG unit,NBB_LONG queue_id,
        NBB_LONG cos,NBB_LONG drop_precedence,NBB_LONG max_queue_size, 
        const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiAradSetQueueTailDrop ret=%d:\n"
                  "unit = %d,queue_id = %d,cos=%d,drop_precedence = %d"
                  "max_queue_size=%d\n\n",
                  function,line,ret,unit,queue_id,cos,drop_precedence,max_queue_size);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiAradSetQueueTailDrop ret=%d:\n"
                  "unit = %d,queue_id = %d,cos=%d,drop_precedence = %d"
                  "max_queue_size=%d\n\n",
                  function,line,ret,unit,queue_id,cos,drop_precedence,max_queue_size);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }  
}


#if 2

/*****************************************************************************
   函 数 名  : spm_qos_find_wred_cb
   功能描述  : 查找wred模板相关配置
   输入参数  : wred模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG spm_set_wred_basic_cfg(NBB_ULONG key,NBB_ULONG oper,NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    ATG_DCI_WRED_BASIC_DATA *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips消息类型 */
    switch (oper)
    {
        /* 更新 */
        case  ATG_DCI_OPER_UPDATE:
        break;

        /* 删除 */
        case  ATG_DCI_OPER_DEL:
#if defined (SPU) || defined (SRC)
        ret = fhdrv_fap_qos_delete_wred_profile(unit,key);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_del_wred_profile_err(unit,key,__FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }

#endif
        break;

        /* 增加 */
        case  ATG_DCI_OPER_ADD:

        /* 消息内容为空 */
        if (NULL == buf)
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /* 消息内容不为空 */
        else
        {
            ptemp = (ATG_DCI_WRED_BASIC_DATA *)buf;

            /***************************************************************************/
            /*                          存配置                                         */
            /***************************************************************************/
#if defined (SPU) || defined (SRC)
            ret = fhdrv_fap_qos_set_wred_profile(unit,0,key,
                ptemp->green_end_threshold,ptemp->green_start_threshold,
                ptemp->green_discard_percent);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_set_wred_profile_err(unit,0,key,
                    ptemp->green_end_threshold,ptemp->green_start_threshold,
                    ptemp->green_discard_percent,__FUNCTION__, __LINE__, ret);
                goto EXIT_LABEL;
            }
               
#endif

#if defined (SPU) || defined (SRC)
            ret = fhdrv_fap_qos_set_wred_profile(unit,1,key,
                ptemp->yellow_end_threshold,ptemp->yellow_start_threshold,
                ptemp->yellow_discard_percent);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_set_wred_profile_err(unit,1,key,
                    ptemp->yellow_end_threshold,ptemp->yellow_start_threshold,
                    ptemp->yellow_discard_percent,__FUNCTION__, __LINE__, ret);
                goto EXIT_LABEL;
            }
               
#endif

#if defined (SPU) || defined (SRC)
            ret = fhdrv_fap_qos_set_wred_profile(unit,2,key,
                ptemp->red_end_threshold,ptemp->red_start_threshold,
                ptemp->red_discard_percent);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_set_wred_profile_err(unit,2,key,
                    ptemp->red_end_threshold,ptemp->red_start_threshold,
                    ptemp->red_discard_percent,__FUNCTION__, __LINE__, ret);
                goto EXIT_LABEL;
            }
               
#endif
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
   函 数 名  : spm_qos_find_wred_cb
   功能描述  : 查找wred模板相关配置
   输入参数  : wred模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_wred(ATG_DCI_SET_WRED *pst_set_ips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG key = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE *basic_date_start = NULL;
    NBB_ULONG oper_basic = ATG_DCI_OPER_NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pst_set_ips)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    pst_set_ips->return_code = ATG_DCI_RC_OK;
    key = pst_set_ips->key;


    oper_basic = pst_set_ips->oper_basic;
    basic_date_start = (NBB_BYTE*)NTL_OFFLEN_GET_POINTER(pst_set_ips, &pst_set_ips->basic_data);

    /* ips消息为删除 */
    if (TRUE == pst_set_ips->delete_struct)
    {
#if defined (SPU) || defined (SRC)
        ret = fhdrv_fap_qos_delete_wred_profile(0,key);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_del_wred_profile_err(0,key,__FUNCTION__, __LINE__, ret);
            pst_set_ips->return_code = ATG_DCI_RC_DEL_FAILED;
            goto EXIT_LABEL;
        }
#endif
    }

    /* 增加或者更新 */
    else
    {
        ret = spm_set_wred_basic_cfg(key,oper_basic,basic_date_start);
        pst_set_ips->basic_return_code = ret;
        pst_set_ips->return_code = ret;
    }


    /* 异常跳出 */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}

#endif

#if 3

/*****************************************************************************
   函 数 名  : spm_ds_add_logic_intf_node
   功能描述  : 逻辑端口优先级映射的接口函数
   输入参数  : ds模板的index  ,逻辑端口的key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static void qos_print_port_wred_data(ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg)
{
    if(NULL != cfg)
    {
       if(ATG_DCI_RC_OK != g_qos_wred_print)
        {
            printf("mode=%d,wred id=%d\n\n",cfg->mode,cfg->wred_id);
        } 
    }
    
}


/*****************************************************************************
   函 数 名  : spm_ds_add_logic_intf_node
   功能描述  : 逻辑端口优先级映射的接口函数
   输入参数  : ds模板的index  ,逻辑端口的key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static void qos_log_port_wred_data(NBB_LONG slot,NBB_LONG port,NBB_ULONG voq,NBB_BYTE cos,
     ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_port_wred_data(cfg);
    
    if(NULL != cfg)
    {
        if(ATG_DCI_RC_OK != g_qos_wred_log)
        {
            
            OS_SPRINTF(uc_message,"slot=%d,port=%d,voq=%ld,cos=%d\n"
                "mode=%d,wred id=%d\n\n",slot,port,voq,cos,cfg->mode,cfg->wred_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
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
NBB_LONG spm_set_physical_port_wred(SUB_PORT *sub_port,NBB_BYTE cos,ATG_DCI_PHY_PORT_CONGEST_QUEUE *cfg)
{
    NBB_INT ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_ULONG voq = 0;
    NBB_ULONG offset = MAX_SLOT_NUM * NUM_COS;

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == sub_port) || (NULL == cfg))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* 全量配置:在所有入口盘上都会配置到出口盘出端口上8个队列的拥塞避免策略 */
    voq = offset * (sub_port->port) + VOQ_OFFSET + 8 * (sub_port->slot) - 8;
    
    qos_log_port_wred_data(sub_port->slot,sub_port->port,voq,cos,cfg);
    
    if (0 == cfg->mode)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradSetQueueTailDrop(unit, voq, cos, -1, 10 * 1024 * 1024);

        //ret = ApiSetQueueWred(unit,voq,cos,0,-1,100,100,0);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_api_set_que_tail_drop_err(unit, voq, cos, -1, 10 * 1024 * 1024,
                __FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }
    else
    {
#if defined (SPU) || defined (SRC)
        ret = fhdrv_fap_qos_set_queue_wred(unit, voq, cos,cfg->wred_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_api_set_que_wred_err(unit, voq, cos,cfg->wred_id,__FUNCTION__, __LINE__, ret);
            goto EXIT_LABEL;
        }
#endif
    }

    


    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}
#endif

