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


/*************************************************************************/
/**************************************************************************/
/*************************************************************************/
#define HQOS_TAG       "QOS HQOS CFG"

/*DS模块全局打印开关*/
NBB_BYTE g_qos_hqos_print = ATG_DCI_RC_OK;

/*DS模块全局LOG开关*/
NBB_BYTE g_qos_hqos_log = ATG_DCI_RC_OK;

NBB_BYTE g_hqos_port_index[4] = {0,0,0,0};

SPM_QOS_PORT_CB g_qos_port_cb[MAX_SLOT_NUM + 1][MAX_PHYSIC_PORT_NUM];

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
NBB_VOID qos_set_hqos_print(NBB_BYTE set)
{
    g_qos_hqos_print = set;
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
NBB_VOID qos_set_hqos_log(NBB_BYTE set)
{
    g_qos_hqos_log = set;
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
NBB_VOID spm_dbg_record_hqos_vc_head(ATG_DCI_VC_KEY *vc_key, NBB_BYTE ucOper)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];
    if(NULL != vc_key && ATG_DCI_RC_OK != g_qos_hqos_log)
    {
        OS_SPRINTF(message,"################### VC (vc_id=%ld,peer_ip=0x%lx,"
            "vc_type=%d, %s) ###################\n",
                    vc_key->vc_id,vc_key->peer_ip,vc_key->vc_type,
                    *(qos_cfg_oper_type_string + ucOper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);  
    }
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 创建出口盘的VC
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
static NBB_LONG spm_hqos_check_slot(NBB_BYTE slot,NBB_ULONG node_index,
                    NBB_LONG *voq,NBB_LONG *vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_LONG base_queue = 0;
    NBB_LONG base_vcid = 0;
    NBB_BYTE fapid = v_spm_shared->local_slot;
    NBB_BYTE slot_index = 128;
    NBB_BYTE fapid_index = 128;
    NBB_ULONG offset = MAX_HQOS_SLOT_NUM * NUM_COS;

    NBB_TRC_ENTRY(__FUNCTION__);

    for (i = 0; i < MAX_HQOS_SLOT_NUM; i++)
    {
        /*g_hqos_port_index[i]:支持hqos的槽位号*/
        if (g_hqos_port_index[i] == slot)
        {
           *slot_index = i;
        }
        if(g_hqos_port_index[i] == fapid)
        {
            fapid_index = i;
        }
        vc_id[i] = offset * node_index + i * NUM_COS + HQOS_OFFSET;
    }

    if(128 == slot_index || 128 == fapid_index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    *voq = slot_index * NUM_COS + offset * node_index + HQOS_OFFSET;
    *vc = offset * node_index + fapid_index * NUM_COS + HQOS_OFFSET;
    

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}



/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 删除出口盘的VC
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
NBB_LONG spm_hqos_apply_lsp_pw_id(NBB_LONG *lsp_id,NBB_LONG *pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*申请LSP ID*/


    ret = spm_hqos_apply_lsp_id(lsp_id);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
  
    ret = spm_hqos_apply_pw_id(pw_id);
    if(ATG_DCI_RC_OK != ret)
    {
        spm_hqos_release_lsp_id(*lsp_id);
        goto EXIT_LABEL;
    }
   

    

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}




/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 删除出口盘的VC
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
NBB_LONG spm_hqos_release_lsp_pw_id(NBB_LONG lsp_id,NBB_LONG pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*资源被占用可以释放*/
    ret = spm_hqos_release_lsp_id(lsp_id);
    if(ATG_DCI_RC_OK != ret)
    {
        rv = ret;
    }
 
    ret = spm_hqos_release_pw_id(pw_id);
    if(ATG_DCI_RC_OK != ret)
    {
        rv = ret;
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_create_lsp_pw(NBB_LONG lsp_id,
    NBB_USHORT port,NBB_LONG pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);



#if defined (SPU) || defined (SRC)
    ret = ApiAradHqosLspCreate(0,port,lsp_id);
    if (ATG_DCI_RC_OK != ret)
    {
        spm_hqos_arad_create_lsp_error_log(port,lsp_id,
            __FUNCTION__,__LINE__,ret);
        goto EXIT_LABEL;
    }
#endif 

#if defined (SPU) || defined (SRC)
    ret = ApiAradHqosPwCreate(0,port,lsp_id,pw_id,0);
    if (ATG_DCI_RC_OK != ret)
    {
        ApiAradHqosLspDelete(0,port,lsp_id);
        spm_hqos_arad_create_pw_error_log(port,lsp_id,pw_id,
            __FUNCTION__,__LINE__,ret);
        goto EXIT_LABEL;
    }
#endif     


    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_del_lsp_pw(NBB_LONG lsp_id,
    NBB_USHORT port,NBB_LONG pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);


    if(0 != lsp_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspDelete(0,port,lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_lsp_error_log(port,lsp_id,
                __FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif        
    }
 

    if(0 != pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwDelete(0,lsp_id,pw_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_pw_error_log(lsp_id,pw_id,
                __FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif        
    }
     

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
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
static void qos_print_traffic_shaping_cfg(QOS_TRAFFIC_SHAPING *data)
{
    if(NULL != data)
    {
        printf("traffic_shaping:cir=%ld,cbs=%ld,pir=%ld,pbs=%ld\n",
                  data->cir,data->cbs,data->pir,data->pbs);
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
static void qos_log_traffic_shaping_cfg(QOS_TRAFFIC_SHAPING *data)
{
    NBB_CHAR message[DS_MSG_INFO_LEN];
    if(NULL != data)
    {
        OS_SPRINTF(message,"traffic_shaping:cir=%ld,cbs=%ld,pir=%ld,pbs=%ld\n",
                  data->cir,data->cbs,data->pir,data->pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
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
static void qos_print_traffic_schedule_cfg(QOS_SCHEDULE_STRATEGY *data)
{
    if(NULL != data)
    {
        printf("traffic_schedule:mod=%d,weight=%d\n",
                  data->mod,data->weight);
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
static void qos_log_traffic_schedule_cfg(QOS_SCHEDULE_STRATEGY *data)
{
    NBB_CHAR message[DS_MSG_INFO_LEN];
    if(NULL != data)
    {
        OS_SPRINTF(message,"traffic_schedule:mod=%d,weight=%d\n",
                  data->mod,data->weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
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
static void qos_print_traffic_congestion_cfg(QOS_CONGESTION_STRATEGY *data)
{
    if(NULL != data)
    {
        printf("traffic_schedule:mod=%d,weight=%d\n",
                  data->mod,data->weight);
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
static void qos_log_traffic_congestion_cfg(QOS_CONGESTION_STRATEGY *data)
{
    NBB_CHAR message[DS_MSG_INFO_LEN];
    if(NULL != data)
    {
        OS_SPRINTF(message,"traffic_schedule:mod=%d,weight=%d\n",
                  data->mod,data->weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
    }
    
}





#if 2

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
NBB_VOID spm_hqos_arad_voq_error_log(NBB_ULONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_ULONG base_vcid,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosVoqSet err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosVoqSet err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);     
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_TAG,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
static NBB_VOID spm_hqos_arad_traffic_set_error_log(NBB_ULONG voq,
    NBB_USHORT slot,NBB_ULONG port,NBB_ULONG base_vcid,NBB_ULONG pw_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosTrafficSet err: %ld (base_queue=%ld,slot=%d,"
                  "port=%ld,vc=%ld,pw_id=%ld)\n\n",function,line,ret,voq, slot, port, base_vcid,pw_id);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosTrafficSet err: %ld (base_queue=%ld,slot=%d,"
                   "port=%ld,vc=%ld,pw_id=%ld)\n\n",function,line,ret,voq, slot, port, base_vcid,pw_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_set_pw_bandwidth_error_log(NBB_ULONG pw_id, 
    NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs, const NBB_CHAR *function, 
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosPwBandwidthSet err: %ld "
            "(pw_id=%ld,cir=%ld,pir=%ld,cbs=%ld,pbs=%ld)\n\n",
            function,line,ret, pw_id,cir,pir,cbs,pbs);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosPwBandwidthSet err: %ld "
            "(pw_id=%ld,cir=%ld,pir=%ld,cbs=%ld,pbs=%ld)\n\n",
            function,line,ret, pw_id,cir,pir,cbs,pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_create_pw_error_log(NBB_ULONG port,NBB_ULONG lsp_id,
    NBB_ULONG pw_id,const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosPwCreate err: %ld (port=%ld,lsp_id=%ld,pw_id=%ld)\n\n",
                function,line,ret, port,lsp_id,pw_id);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosPwCreate err: %ld (port=%ld,lsp_id=%ld,pw_id=%ld)\n\n",
                function,line,ret, port,lsp_id,pw_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_voq_del_error_log(NBB_ULONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_ULONG base_vcid,const NBB_CHAR *function, 
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosVoqDelete err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosVoqDelete err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_traffic_del_error_log(NBB_ULONG voq,
    NBB_USHORT slot,NBB_ULONG port,NBB_ULONG base_vcid,NBB_ULONG pw_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosTrafficDelete err: %ld (base_queue=%ld,slot=%d,port=%ld,"
                  "vc=%ld,pw_id=%ld)\n\n",function,line,ret,voq, slot, port, base_vcid,pw_id);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosTrafficDelete err: "
                   "%ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld,pw_id=%ld)\n\n",
                   function,line,ret,voq, slot, port, base_vcid,pw_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);     
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_del_pw_error_log(NBB_ULONG lsp_id,NBB_ULONG pw_id,
    NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosPwDelete err: %ld (lsp_id=%ld,pw_id=%ld)\n\n",
                function,line,ret,lsp_id,pw_id);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosPwDelete err: %ld (lsp_id=%ld,pw_id=%ld)\n\n",
                function,line,ret,lsp_id,pw_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_set_pw_weight_error_log(NBB_ULONG pw_id, NBB_ULONG weight,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosPwWfqWeightSet err: %ld "
            "(pw_id=%ld,weight=%ld)\n\n",
            function,line,ret, pw_id,weight);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosPwWfqWeightSet err: %ld "
            "(pw_id=%ld,weight=%ld)\n\n",
            function,line,ret, pw_id,weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_set_traffic_bandwidth_error_log(NBB_ULONG pw_id, 
    NBB_ULONG queue_id, NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs, 
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosTrafficBandwidthSet err: %ld "
            "(pw_id=%ld,queue_id=%ld,cir=%ld,pir=%ld,cbs=%ld,pbs=%ld)\n\n",
            function,line,ret, pw_id,queue_id,cir,pir,cbs,pbs);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosTrafficBandwidthSet err: %ld "
            "(pw_id=%ld,queue_id=%ld,cir=%ld,pir=%ld,cbs=%ld,pbs=%ld)\n\n",
            function,line,ret, pw_id,queue_id,cir,pir,cbs,pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_set_trafficsche_error_log(NBB_ULONG port, 
    NBB_ULONG pw_id, NBB_ULONG que4_1, NBB_ULONG que8_5, const NBB_CHAR *function,
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosTrafficScheModeSet err: %ld "
            "(port=%ld,pw_id=%ld,que4_1=%ld,que8_5=%ld)\n\n",
            function,line,ret, port,pw_id,que4_1,que8_5);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosTrafficScheModeSet err: %ld "
            "(port=%ld,pw_id=%ld,que4_1=%ld,que8_5=%ld)\n\n",
            function,line,ret, port,pw_id,que4_1,que8_5);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
static NBB_VOID spm_hqos_arad_fresh_pw_to_lsp_error_log(NBB_LONG unit,NBB_LONG pw_id,
    NBB_LONG lsp_id,NBB_USHORT port,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s %s,line=%ld ret=%ld fhdrv_fap_qos_refresh_pw_to_lsp\n"
                "unit=%ld,pw_id=%ld,lsp=%ld,port=%d\n\n",
                HQOS_LSP_ERROR,function,line,unit,pw_id,lsp_id,port);
        OS_SPRINTF(message,"%s %s,line=%ld ret=%ld fhdrv_fap_qos_refresh_pw_to_lsp\n"
                "unit=%ld,pw_id=%ld,lsp=%ld,port=%d\n\n",
                HQOS_LSP_ERROR,function,line,unit,pw_id,lsp_id,port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));
        
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
static NBB_VOID spm_hqos_arad_create_lsp_error_log(NBB_ULONG port,NBB_ULONG lsp_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s %s,line=%ld ApiAradHqosLspCreate port=%ld,lsp id=%ld,ret=%ld\n",
                HQOS_LSP_ERROR,function,line,port,lsp_id,ret);
        OS_SPRINTF(message,"%s %s,line=%ld ApiAradHqosLspCreate port=%ld,lsp id=%ld,ret=%ld\n\n",
                HQOS_LSP_ERROR,function,line,port,lsp_id,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));
        
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
NBB_VOID spm_hqos_arad_del_lsp_error_log(NBB_ULONG port,NBB_ULONG lsp_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s %s,line=%ld ApiAradHqosLspDelete port=%ld,lsp id=%ld,ret=%ld\n",
                HQOS_LSP_ERROR,function,line,port,lsp_id,ret);
        OS_SPRINTF(message,"%s %s,line=%ld ApiAradHqosLspDelete port=%ld,lsp id=%ld,ret=%ld\n\n",
                HQOS_LSP_ERROR,function,line,port,lsp_id,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));
        
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
NBB_VOID spm_hqos_arad_set_lsp_bandwidth_error_log(
    NBB_ULONG lsp_id,NBB_ULONG cir,NBB_ULONG pir,NBB_ULONG cbs,NBB_ULONG pbs,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s %s,line=%ld ApiAradHqosLspBandwidthSet lsp id=%ld,cir=%ld,"
            "pir=%ld,cbs=%ld,pbs=%ld,ret=%ld\n",
                HQOS_LSP_ERROR,function,line,lsp_id,cir,pir,cbs,pbs,ret);
        OS_SPRINTF(message,"%s %s,line=%ld ApiAradHqosLspBandwidthSet lsp id=%ld,cir=%ld,"
            "pir=%ld,cbs=%ld,pbs=%ld,ret=%ld\n\n",
                HQOS_LSP_ERROR,function,line,lsp_id,cir,pir,cbs,pbs,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_LSP_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_set_lsp_weight_error_log(NBB_ULONG lsp_id,NBB_ULONG weight,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosLspWfqWeightSet err: %ld "
            "(lsp_id=%ld,weight=%ld)\n\n",
            function,line,ret, lsp_id,weight);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosLspWfqWeightSet err: %ld "
            "(lsp_id=%ld,weight=%ld)\n\n",
            function,line,ret, lsp_id,weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID spm_hqos_arad_voq_error_log(NBB_ULONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_ULONG base_vcid,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosVoqSet err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosVoqSet err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);     
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
    }
     
}

fhdrv_psn_common_set_txlsp_hqos

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
NBB_VOID  spm_hqos_txlsp_error_log(NBB_LONG unit,NBB_ULONG posid,NBB_ULONG hqosEn,
    NBB_ULONG voq,  const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ret=%ld fhdrv_psn_common_set_txlsp_hqos\n"
                  "unit=%ld,posid=%ld,hqosEn=%s,voq=%ld\n\n",
                function,line,ret,unit,posid,
                hqosEn?"使能":"不使能",voq);
        OS_SPRINTF(message,"%s line=%ld  ret=%ld fhdrv_psn_common_set_txlsp_hqos\n"
                  "unit=%ld,posid=%ld,hqosEn=%s,voq=%ld\n\n",
                function,line,ret,unit,posid,
                hqosEn?"使能":"不使能",voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_C3_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
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
NBB_VOID  spm_hqos_vp_error_log(
    NBB_ULONG voq, NBB_ULONG hqosEn, NBB_ULONG posid, NBB_LONG proflag,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ret=%ld ApiC3SetVpHqos voq=%ld "
                  "posid=%ld,hqosEn=%s,proflag=%s\n\n",
                function,line,ret,voq, posid,
                hqosEn?"使能":"不使能",(QOS_BACK == proflag)?"备用":"主用");
        OS_SPRINTF(message,"%s line=%ld  ret=%ld ApiC3SetVpHqos voq=%ld"
                   " posid=%ld,hqosEn=%s proflag=%s\n\n",
                function,line,ret,voq, posid,
                hqosEn?"使能":"不使能",(QOS_BACK == proflag)?"备用":"主用");
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_C3_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
    }
   
}

#endif

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 创建出口盘的VC
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
static NBB_LONG spm_hqos_create_voq_vc(NBB_BYTE slot, NBB_LONG port,
    NBB_ULONG pw_id,
    NBB_LONG voq_id,NBB_LONG tm_vc,NBB_ULONG vc_id[MAX_HQOS_SLOT_NUM],
    NBB_LONG *p_voq,NBB_LONG *p_vc,NBB_ULONG p_vc_id[MAX_HQOS_SLOT_NUM])
{
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vc_id)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    

#if defined (SPU) || defined (SRC)
    ret = ApiAradHqosVoqSet(unit, voq_id, NUM_COS, slot, port, tm_vc);
    if (ATG_DCI_RC_OK != ret)       
    {
        spm_hqos_arad_voq_error_log(voq_id,slot,port,
            tm_vc, __FUNCTION__,__LINE__,ret);
        goto EXIT_LABEL;
    }
    if(NULL != p_voq && NULL != p_vc)
    {
        *p_voq = voq_id;
        *p_vc = tm_vc;
    }  
#endif

    /*本槽位是出口盘进行VC的建立*/
    if(slot == v_spm_shared->local_slot_id)
    {
        for(i = 0; i < MAX_HQOS_SLOT_NUM; i++)
        {
            if(0 == g_hqos_port_index[i])
            {
                continue; /*非HQOS槽位*/
            }
              
#if defined (SPU) || defined (SRC)

            /*在出口端创建VC并和开启了hqos开关的单盘入口端VOQ建立关联*/
            ret = ApiAradHqosTrafficSet(unit, voq_id, NUM_COS, g_hqos_port_index[i], 
                        port, vc_id[i], pw_id);
            if (ATG_DCI_RC_OK != ret) 
            {
                spm_hqos_arad_traffic_set_error_log(voq_id,
                    g_hqos_port_index[i], port,vc_id[i],
                    pw_id,__FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
            if(NULL != p_vc_id)
            {
                p_vc_id[i] = vc_id[i];
            }          
#endif
        }
      
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}



/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 创建出口盘的VC
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
static NBB_LONG spm_hqos_set_pw_property(NBB_BYTE slot,NBB_LONG pw_id,
     NBB_LONG base_queue,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }



    /*出口盘则进行PW属性设置*/
    if(slot == v_spm_shared->local_slot_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwBandwidthSet(unit, pw_id, data->normal_shaping.cir,
              data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_pw_bandwidth_error_log(pw_id, data->normal_shaping.cir,
              data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif

#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwWfqWeightSet(unit, pw_id, data->normal_strategy.weight);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_pw_weight_error_log( pw_id,
                data->normal_strategy.weight,__FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif


        /***************************************************************************/
        /* 8队列配置处理                                                        */
        /***************************************************************************/
        for(i = 0;i < 8;i++)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficBandwidthSet(unit, pw_id,i, data->proi_queue_shaping[i].cir,
                  data->proi_queue_shaping[i]->pir, data->proi_queue_shaping[i]->cbs,
                  data->proi_queue_shaping[i]->pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_set_traffic_bandwidth_error_log(pw_id,i,
                    data->proi_queue_shaping[i].cir,data->proi_queue_shaping[i]->pir, 
                    data->proi_queue_shaping[i]->cbs,data->proi_queue_shaping[i]->pbs,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif  

#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficScheModeSet(unit, pw_id, data->proi_queue_strategy[i].cir,
                  data->proi_queue_strategy[i]->pir, data->proi_queue_strategy[i]->cbs,
                  data->proi_queue_strategy[i]->pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                
                goto EXIT_LABEL;
            }
#endif 
        }
     
    }


    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 创建出口盘的VC
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
static NBB_LONG spm_hqos_set_lsp_pw_property(NBB_BYTE slot,NBB_USHORT port,
     NBB_LONG lsp_id,NBB_LONG pw_id,NBB_ULONG posid,
     NBB_LONG base_queue,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }



    /*出口盘并且不是默认LSP节点属性则进行LSP 和默认PW属性设置*/
    if(slot == v_spm_shared->local_slot_id && lsp_id != port)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspBandwidthSet(unit, lsp_id, data->normal_shaping.cir,
              data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_lsp_bandwidth_error_log(lsp_id, data->normal_shaping.cir,
              data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif

#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspWfqWeightSet(unit, lsp_id, data->normal_strategy.weight);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_lsp_weight_error_log(lsp_id,
                data->normal_strategy.weight,__FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif


        /***************************************************************************/
        /* 8队列配置处理                                                        */
        /***************************************************************************/
        for(i = 0;i < 8;i++)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficBandwidthSet(unit, pw_id,i, data->proi_queue_shaping[i].cir,
                  data->proi_queue_shaping[i]->pir, data->proi_queue_shaping[i]->cbs,
                  data->proi_queue_shaping[i]->pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_set_traffic_bandwidth_error_log(pw_id,i,
                    data->proi_queue_shaping[i].cir,data->proi_queue_shaping[i]->pir, 
                    data->proi_queue_shaping[i]->cbs,data->proi_queue_shaping[i]->pbs,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif  

#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficScheModeSet(unit, pw_id, data->proi_queue_strategy[i].cir,
                  data->proi_queue_strategy[i]->pir, data->proi_queue_strategy[i]->cbs,
                  data->proi_queue_strategy[i]->pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                
                goto EXIT_LABEL;
            }
#endif 
        }
     
    }

    /*入口盘和出口盘都会执行*/
    /*最后设置C3的flow id hqos的业务通起来*/
    for(unit = 0; unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
   
        ret = fhdrv_psn_common_set_txlsp_hqos(unit, posid,1, base_queue);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_txlsp_error_log(unit, posid,1, base_queue,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#if 4





/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 申请内存并申请lsp id 初始化HQOS树
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
SPM_HQOS_LSP_CB *spm_alloc_hqos_lsp_cb(SPM_QOS_TUNNEL_KEY *pkey,
    NBB_LONG defaulte,NBB_BYTE slot,NBB_USHORT port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_LSP_CB *pst_tbl = NULL;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_LONG lsp_id = 0;
    NBB_LONG pw_id = 0;
    NBB_BUF_SIZE avll_key_offset;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pkey)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        goto EXIT_LABEL;
    }
    
    /*出口盘申请LSP ID 资源*/
    if(slot == v_spm_shared->local_slot_id)
    {
        /*LSP资源不足的概率比内存不足的概率大很多*/
        if(ATG_DCI_RC_OK != defaulte)
        {
            ret = spm_hqos_apply_lsp_pw_id(lsp_id,pw_id);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            ret = spm_hqos_create_lsp_pw(lsp_id,port,pw_id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_hqos_release_lsp_pw_id(lsp_id,pw_id);
                goto EXIT_LABEL;
            }
        }
        else/*采用默认的LSP ID*/
        {
            lsp_id = port;
        } 
    }

    /* 分配一个新的lsp_tx配置块qos表配置条目。*/
    pst_tbl = (SPM_HQOS_LSP_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_LSP_CB),
        NBB_NORETRY_ACT, MEM_SPM_HQOS_LSP_TX_CB);

    /*分配内存失败不释放HQOS资源，内存没有了，基本就挂了*/
    if (pst_tbl == NULL)
    {
        /*异常跳出*/
        goto EXIT_LABEL;
    }

    /* 初始lsp_tx配置块配置条目 */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_HQOS_LSP_CB));
    OS_MEMCPY(&pst_tbl->lsp_tx_key, pkey, sizeof(SPM_QOS_TUNNEL_KEY));
    pst_tbl->lsp_id = lsp_id;
    pst_tbl->slot = slot;
    pst_tbl->port = port;
    pst_tbl->pw_id = pw_id;

    /*QOS vc 树初始化*/
    avll_key_offset = NBB_OFFSETOF(SPM_HQOS_VC_CB, vc_key);
    AVLL_INIT_TREE(pst_tbl->vc_tree, spm_vc_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_VC_CB, spm_hqos_vc_node));

    /*QOS vrf 树初始化*/
    avll_key_offset = NBB_OFFSETOF(SPM_HQOS_VRF_CB, vrf_key);
    AVLL_INIT_TREE(pst_tbl->vrf_tree, spm_qos_vrf_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_VRF_CB, spm_hqos_vrf_node));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_hqos_lsp_tx_node);

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_hqos_lsp_cb(SPM_HQOS_LSP_CB *pst_tbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*释放内存失败*/
    if (NULL == pst_tbl)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_LSP_TX_CB), MEM_SPM_HQOS_LSP_TX_CB);

    /*说明已经是出口盘并且不是默认的LSP节点*/
    if((pst_tbl->lsp_id != pst_tbl->port) && (0 != pst_tbl->lsp_id))
    {
        ret = spm_hqos_del_lsp_pw(pst_tbl->lsp_id, pst_tbl->port,pst_tbl->pw_id);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }

        ret = spm_hqos_release_lsp_pw_id(pst_tbl->lsp_id,pst_tbl->pw_id);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_HQOS_LSP_TX_CB);
    pst_tbl = NULL;

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}




/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 分配内存的同时分配PW ID
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG spm_hqos_del_lsp_node(SPM_HQOS_LSP_CB **plsp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_LSP_CB *lsp = NULL;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_LONG lsp_id = 0;
    NBB_BYTE i = 0;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP节点不可能为NULL*/
    if(NULL == plsp || NULL == *plsp)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    lsp = *plsp;
    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;

     /*NP hqos 去使能*/
    if(0 != lsp->np_flow_id || 0 != lsp->posid)
    {
        for(unit = 0; unit < c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
       
            ret = fhdrv_psn_common_set_txlsp_hqos(unit, lsp->posid,0, lsp->np_flow_id);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_txlsp_error_log(unit, lsp->posid,0, lsp->np_flow_id,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
    }


    /*删除voq*/
    if( 0 != lsp->voq_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosVoqDelete(0, lsp->voq_id, NUM_COS, slot, port, lsp->tm_vc);
        if (ATG_DCI_RC_OK != ret)       
        {
            spm_hqos_arad_voq_error_log(lsp->voq_id,slot,port,
                lsp->tm_vc, __FUNCTION__,__LINE__,ret);
            rv = ret;
        } 
#endif    
     }

    /*删除vc*/
    for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
    {
        if(0 != lsp->vc_id[i])
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficDelete(0, lsp->voq_id, NUM_COS, g_hqos_port_index[i],
                    port, lsp->vc_id[i],lsp->pw_id);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_traffic_del_error_log(lsp->voq_id,g_hqos_port_index[i],
                    port, lsp->vc_id[i],lsp->pw_id, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif     
        }
    }


    
    if(AVLL_IN_TREE(lsp->spm_hqos_lsp_tx_node)/*删除存在的配置*/
    {
        /*删除非默认的LSP节点:创建失败回收资源或者更新到默认的LSP ID*/
        if(0 != lsp_id && lsp_id != port)
        {
            /*更新LSP ID 到默认的节点上*/
            ret = spm_hqos_refresh_lsp_port(slot,port,port,lsp);
            if (ATG_DCI_RC_OK != ret)
            {
                rv = ret;
            }
        }
        
        /***************************************************************************/
        /* VC VRF节点信息全部为空了才清除绑定关系释放资源***********************/                                                   */
        /***********************************************************************/
        if ((NULL == AVLL_FIRST(lsp->vc_tree) && NULL == AVLL_FIRST(lsp->vrf_tree))
        {
            AVLL_DELETE((g_qos_port_cb[slot][port]).lsp_tree,lsp->spm_hqos_lsp_tx_node);    
            ret = spm_free_hqos_lsp_cb(lsp);
            if (ATG_DCI_RC_OK != ret) 
            {
                rv = ret;
            } 
            *plsp = NULL;
        }
        
    }
    else/*删除因为异常回收资源包括默认和非默认LSP节点*/
    {
        ret = spm_free_hqos_lsp_cb(lsp);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        } 
        *plsp = NULL;
    }


   
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 分配内存的同时分配PW ID
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG spm_hqos_set_lsp_to_default(SPM_HQOS_LSP_CB *lsp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_LSP_CB *lsp = NULL;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_LONG lsp_id = 0;
    NBB_LONG pw_id = 0;
    NBB_BYTE i = 0;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP节点不可能为NULL*/
    if(NULL == lsp)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       rv = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;
    pw_id = lsp->pw_id;

     /*NP hqos 去使能*/
    if(0 != lsp->np_flow_id || 0 != lsp->posid)
    {
        for(unit = 0; unit < c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
       
            ret = fhdrv_psn_common_set_txlsp_hqos(unit, lsp->posid,0, lsp->np_flow_id);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_txlsp_error_log(unit, lsp->posid,0, lsp->np_flow_id,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
    }


    /*删除voq*/
    if( 0 != lsp->voq_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosVoqDelete(0, lsp->voq_id, NUM_COS, slot, port, lsp->tm_vc);
        if (ATG_DCI_RC_OK != ret)       
        {
            spm_hqos_arad_voq_error_log(lsp->voq_id,slot,port,
                lsp->tm_vc, __FUNCTION__,__LINE__,ret);
            rv = ret;
        } 
#endif    
     }

    /*删除vc*/
    for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
    {
        if(0 != lsp->vc_id[i])
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficDelete(0, lsp->voq_id, NUM_COS, g_hqos_port_index[i],
                    port, lsp->vc_id[i],lsp->pw_id);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_traffic_del_error_log(lsp->voq_id,g_hqos_port_index[i],
                    port, lsp->vc_id[i],lsp->pw_id, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif     
        }
    }

    ret = spm_hqos_del_lsp_pw(lsp_id,pw_id,port);
    if (ATG_DCI_RC_OK != ret)       
    {
        rv = ret;
    } 
    ret = spm_hqos_release_lsp_pw_id(lsp->lsp_id,lsp->pw_id);
    if (ATG_DCI_RC_OK != ret)       
    {
        rv = ret;
    } 
    lsp->lsp_id = port;
    lsp->pw_id = 0;

   
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG spm_hqos_add_lsp_all_drive(NBB_USHORT slot,
    NBB_LONG port,SPM_HQOS_LSP_CB *lsp_cb)
{
    SPM_HQOS_VRF_CB *vrf_cb = NULL;
    SPM_HQOS_VC_CB *vc_cb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == lsp_cb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    /*非默认lsp节点*/
    if(0 != lsp_cb->lsp_id && lsp_cb->port != lsp_cb->lsp_id)
    {
#if defined (SPU) || defined (SRC) 
        ret = ApiAradHqosLspCreate(0,port,lsp_cb->lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_create_lsp_error_log(port,lsp_cb->lsp_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }


    if(0 != lsp_cb->pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwCreate(0,port,lsp_cb->lsp_id,lsp_cb->pw_id,0);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_create_pw_error_log(port,lsp_cb->lsp_id,lsp_cb->pw_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif 
        ret = spm_hqos_set_lsp_drive(slot,port,lsp_cb->lsp_id,
            lsp_cb->pw_id,lsp_cb->posid,lsp_cb->data,lsp_cb->voq_id,
             lsp_cb->tm_vc,lsp_cb->vc_id,NULL);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

    }

    
    

    for(vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_FIRST(lsp_cb->vrf_tree);
         vrf_cb != NULL;
         vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_NEXT(lsp_cb->vrf_tree,
                       vrf_cb->spm_hqos_vrf_node))
    {
        
    }

    for(vc_cb = (SPM_HQOS_VC_CB*) AVLL_FIRST(lsp_cb->vc_tree);
         vc_cb != NULL;
         vc_cb = (SPM_HQOS_VC_CB*) AVLL_NEXT(lsp_cb->vc_tree,
                       vc_cb->spm_hqos_vc_node))
    {
        if(0 != vc_cb->pw_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosPwCreate(0,port,lsp_cb->lsp_id,vc_cb->pw_id,0);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_create_pw_error_log(port,lsp_cb->lsp_id,
                    vc_cb->pw_id,__FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif 
            ret = spm_hqos_set_vc_drive(slot, port, vc_cb->pw_id,
                vc_cb->posid,vc_cb->proflag,vc_cb->vc_data,vc_cb->voq_id,
                vc_cb->tm_vc,vc_cb->vc_id,NULL);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

        }
        
        
    }

    /*更新成新的出槽位和出端口*/
    lsp_cb->slot = slot;
    lsp_cb->port = port;

    

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG spm_hqos_del_lsp_all_drive(SPM_HQOS_LSP_CB *lsp_cb)
{
    SPM_HQOS_VRF_CB *vrf_cb = NULL;
    SPM_HQOS_VC_CB *vc_cb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == lsp_cb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    for(vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_FIRST(lsp_cb->vrf_tree);
         vrf_cb != NULL;
         vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_NEXT(lsp_cb->vrf_tree,
                       vrf_cb->spm_hqos_vrf_node))
    {
        
    }

    for(vc_cb = (SPM_HQOS_VC_CB*) AVLL_FIRST(lsp_cb->vc_tree);
         vc_cb != NULL;
         vc_cb = (SPM_HQOS_VC_CB*) AVLL_NEXT(lsp_cb->vc_tree,
                       vc_cb->spm_hqos_vc_node))
    {
        ret = spm_hqos_del_vc_drive(lsp_cb->slot, lsp_cb->port, vc_cb);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        if(0 != vc_cb->pw_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosPwDelete(0,vc_cb->pw_id,lsp_cb->lsp_id);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_del_pw_error_log(lsp_cb->lsp_id,vc_cb->pw_id,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif 

        }
        
    }

    if(0 != lsp_cb->pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwDelete(0,lsp_cb->pw_id,lsp_cb->lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_pw_error_log(lsp_cb->lsp_id,lsp_cb->pw_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif 

    }

    /*非默认lsp节点*/
    if(0 != lsp_cb->lsp_id && lsp_cb->port != lsp_cb->lsp_id)
    {
#if defined (SPU) || defined (SRC) 
        ret = ApiAradHqosLspDelete(0,lsp_cb->port,lsp_cb->lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_lsp_error_log(lsp_cb->port,lsp_cb->lsp_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }


    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_refresh_lsp_port(
    NBB_LONG port,NBB_LONG lsp_id,
    SPM_HQOS_LSP_CB *lsp_cb)
{
    SPM_HQOS_VRF_CB *vrf_cb = NULL;
    SPM_HQOS_VC_CB *vc_cb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == lsp_cb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }

    
    if(lsp_cb->slot == v_spm_shared->local_slot_id)
    {
        for(vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_FIRST(lsp_cb->vrf_tree);
         vrf_cb != NULL;
         vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_NEXT(lsp_cb->vrf_tree,
                       vrf_cb->spm_hqos_vrf_node))
        {
#if defined (SPU) || defined (SRC) 
            ret = fhdrv_fap_qos_refresh_pw_to_lsp(unit,vrf_cb->pw_id,lsp_id,port);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_fresh_pw_to_lsp_error_log(unit,vrf_cb->pw_id,
                    lsp_id,port,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }

        for(vc_cb = (SPM_HQOS_VC_CB*) AVLL_FIRST(lsp_cb->vc_tree);
             vc_cb != NULL;
             vc_cb = (SPM_HQOS_VC_CB*) AVLL_NEXT(lsp_cb->vc_tree,
                           vc_cb->spm_hqos_vc_node))
        {
#if defined (SPU) || defined (SRC) 
            ret = fhdrv_fap_qos_refresh_pw_to_lsp(unit,vc_cb->pw_id,lsp_id,port);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_fresh_pw_to_lsp_error_log(unit,vc_cb->pw_id,
                    lsp_id,port,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
    }

    

    /*非默认lsp节点*/
    if(0 != lsp_cb->pw_id)
    {
#if defined (SPU) || defined (SRC) 
        ret = fhdrv_fap_qos_refresh_pw_to_lsp(unit,lsp_cb->pw_id,lsp_id,port);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_fresh_pw_to_lsp_error_log(unit,lsp_cb->pw_id,
                lsp_id,port,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    lsp_cb->port = port;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG* spm_hqos_set_lsp_drive(NBB_BYTE slot,NBB_USHORT port,
        NBB_LONG lsp_id,NBB_LONG pw_id,
        NBB_ULONG posid,
        ATG_DCI_VPN_HQOS_POLICY *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_LSP_CB *plsp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data)|| (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    /*创建入口盘出口的voq与vc的链接关系HQOS业务才能通*/
    if(NULL == plsp)/*根据保存的配置进行刷新驱动*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,NULL,NULL,NULL);  
    }
    else/*根据收到的配置处理并保存配置*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,&(plsp->voq_id),&(plsp->tm_vc),plsp->vc_id);  
    }
    
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }


    /*设置LSP节点的HQOS策略生效*/
    ret = spm_hqos_set_lsp_pw_property(slot,port,lsp_id,pw_id,posid,voq,data);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    if(NULL != plsp)
    {
        plsp->posid = posid;
        plsp->np_flow_id = voq;
        
        /*保存数据，数据插入相应的树中*/
        OS_MEMCPY(&(plsp->data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY));
    }


    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_add_lsp_node(NBB_USHORT slot,
    NBB_LONG port,
    SPM_QOS_TUNNEL_KEY *lsp_key,
    ATG_DCI_LSP_TX_UP_TNNL_QOS *data,
    NBB_ULONG posid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_LSP_CB *plsp = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_ULONG node_index = 0;
    NBB_LONG voq = 0;
    NBB_LONG vc = 0;
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM] = {0};
    NBB_LONG lsp_id = 0;
    NBB_LONG pw_id = 0;
    

    NBB_TRC_ENTRY(__FUNCTION__);

        
    if ((NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (port > 0x80))/*不支持LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }
    
    //qos_log_vc_hqos_data(slot,port,posid,proflag,lsp_key,vc_key,data);
    

    /* 查找是否存在,关键字pkey为该lsp的Type、Flag及Tx_Lsp的key值(ingress、egress、tunnelid及lspid) */
    plsp = AVLL_FIND(g_qos_port_cb[slot][port].lsp_tree, lsp_key);
    
    /* 判断slot是否开启hqos的开关 */
    ret = spm_hqos_check_slot(slot,data->node_index,&voq,&vc,vc_id);
    if (ATG_DCI_RC_OK != ret)
    {   
        if(NULL != plsp)/*新的配置在非HQOS槽位需要使HQOS无效*/
        {

        }
        else/*没有开启HQOS槽位使能直接跳过不报错*/
        {
            ret = ATG_DCI_RC_OK;
        }
        goto EXIT_LABEL;
    }

    
    if(NULL == plsp)/*先创建LSP节点*/
    {
        plsp = spm_alloc_hqos_lsp_cb(lsp_key,ATG_DCI_RC_UNSUCCESSFUL,slot,port);/*申请LSP ID*/

        /*申请内存失败*/
        if (NULL == plsp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_hqos_set_lsp_drive(slot,port,lsp_id,pw_id,posid,data,voq,
            vc,vc_id [ MAX_HQOS_SLOT_NUM ], plsp);
        if(ATG_DCI_RC_OK != ret)
        {
            //释放了LSP资源
            spm_hqos_del_lsp_node(&plsp);
            goto EXIT_LABEL;
        }
        
        rv = AVLL_INSERT(g_qos_port_cb[slot][port].lsp_tree, plsp->spm_hqos_lsp_tx_node);
    }
    else/*lsp节点为默认节点或者更新节点*/
    {
        if(plsp->slot != slot)/*出槽位发生变化需要更新全部节点的驱动内容*/
        {
            /*更新槽位*/
            ret = spm_hqos_del_lsp_all_drive(plsp);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            ret = spm_hqos_add_lsp_all_drive(slot,port,plsp);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
        else if(plsp->port != port)/*出端口发生变化更新驱动*/
        {
            ret = spm_hqos_refresh_lsp_port(port,plsp->lsp_id,plsp);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
        else/*更新配置*/
        {
            /*默认节点变为非默认节点要刷新LSP ID*/
            if(plsp->lsp_id == plsp->port)
            {
                ret = spm_hqos_apply_lsp_pw_id(lsp_id,pw_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }

                ret = spm_hqos_create_lsp_pw(lsp_id,port,pw_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_release_lsp_pw_id(lsp_id,pw_id);
                    goto EXIT_LABEL;
                }
                plsp->lsp_id = lsp_id;
                plsp->pw_id = pw_id;
                
                ret = spm_hqos_set_lsp_drive(slot,port,lsp_id,pw_id,posid,data,voq,
                    vc,vc_id [ MAX_HQOS_SLOT_NUM ], plsp);
                if(ATG_DCI_RC_OK != ret)
                {
                    //释放了LSP资源
                    spm_hqos_set_lsp_to_default(plsp);
                    spm_hqos_del_lsp_node(&plsp);
                    goto EXIT_LABEL;
                }
            }
            else if((plsp->lsp_id != plsp->port) && (0 != plsp->lsp_id))/*更新非默认节点*/
            {
                ret = spm_hqos_set_lsp_pw_property(slot,port,plsp->lsp_id,plsp->pw_id,posid,voq,data);
                if (ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }
                plsp->posid = posid;
                plsp->np_flow_id = voq;
                OS_MEMCPY(&(plsp->data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY));
            }  
            else/*错误的逻辑*/
            {
                spm_qos_exception_log(__FUNCTION__,__LINE__);
            }
        }

        
        
    }



    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif

#if 5

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
NBB_VOID spm_qos_vc_key_pd(ATG_DCI_VC_KEY *vc_key,NBB_CHAR *string,
    NBB_LONG ret)
{
    if((NULL != vc_key) && (NULL != string))
    {
       NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			string,ret,
    			"vc_id","peer_ip","vc_type","",
    			vc_key->vc_id,vc_key->peer_ip,vc_key->vc_type,0)); 
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
static void qos_print_vc_hqos_cfg(ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_BYTE i = 0;
    if(NULL != data)
    {
       if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            qos_print_traffic_shaping_cfg(data->normal_shaping);
            qos_print_traffic_schedule_cfg(data->normal_strategy);
            for(i = 0;i < 8;i++)
            {
               qos_print_traffic_shaping_cfg(data->proi_queue_shaping[i]); 
               qos_print_traffic_schedule_cfg(data->proi_queue_strategy[i]);
               qos_print_traffic_congestion_cfg(data->proi_queue_congestion_strategy[i]);
            }
            
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
static void qos_log_vc_hqos_cfg(ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_BYTE i = 0;
    if(NULL != data)
    {
       if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            qos_log_traffic_shaping_cfg(data->normal_shaping);
            qos_log_traffic_schedule_cfg(data->normal_strategy);
            for(i = 0;i < 8;i++)
            {
               qos_log_traffic_shaping_cfg(data->proi_queue_shaping[i]); 
               qos_log_traffic_schedule_cfg(data->proi_queue_strategy[i]);
               qos_log_traffic_congestion_cfg(data->proi_queue_congestion_strategy[i]);
            }
            
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
void qos_log_vc_hqos_data(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid, NBB_BYTE proflag,
    SPM_QOS_TUNNEL_KEY *lsp_key,,ATG_DCI_VC_KEY *vc_key,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_vc_hqos_cfg(data);
    
    if(ATG_DCI_RC_OK != g_qos_hqos_log)
    {
        OS_SPRINTF(uc_message,"slot=%d,port=%d,proflag=%d\n",
                  slot,port,proflag);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        qos_log_lsp_key(lsp_key);
        qos_log_vc_key(vc_key);
        qos_log_vc_hqos_cfg(data);
    }

   
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 分配内存的同时分配PW ID
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static SPM_HQOS_VC_CB *spm_alloc_hqos_vc_cb(ATG_DCI_VC_KEY *vc_key,
    NBB_LONG lsp_id,NBB_BYTE slot,NBB_USHORT port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VC_CB *pst_tbl = NULL;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_ULONG id = 0;
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vc_key)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        
        /*异常跳出*/
        goto EXIT_LABEL;
    }

    /*出口盘申请pw资源*/
    if(slot == v_spm_shared->local_slot_id)
    {
        /*id资源不足比内存不足的概率大很多*/
        ret = spm_hqos_apply_pw_id(&id);

        /*申请pwid失败*/
        if (ATG_DCI_RC_OK != ret)
        {
            /*异常跳出*/
            goto EXIT_LABEL;
        }

#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwCreate(unit, port,lsp_id,id,0);

        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_release_pw_id(id);
            spm_hqos_arad_create_pw_error_log(port ,lsp_id,
                id,__FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif    
    }



    /* 分配一个新的逻辑配置块qos表配置条目。*/
    pst_tbl = (SPM_HQOS_VC_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_VC_CB),
        NBB_NORETRY_ACT, MEM_SPM_HQOS_VC_CB);

    /*分配内存失败不释放HQOS资源，内存没有了，基本就挂了*/
    if (pst_tbl == NULL)
    {
        /*异常跳出*/
        goto EXIT_LABEL;
    }
    
    
    

    /* 初始逻辑配置块配置条目 */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_HQOS_VC_CB));
    OS_MEMCPY(&(pst_tbl->vc_key), vc_key, sizeof(ATG_DCI_VC_KEY));
    pst_tbl->pw_id = id;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_hqos_vc_node);

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_hqos_vc_cb(SPM_HQOS_VC_CB *pst_tbl,NBB_LONG lsp_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_TRC_ENTRY(__FUNCTION__);

    /*释放内存失败*/
    if (NULL == pst_tbl)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_VC_CB), MEM_SPM_HQOS_VC_CB);
    
    if(0 != pst_tbl->pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwDelete(0, pst_tbl->pw_id,lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_pw_error_log(lsp_id,pst_tbl->pw_id,
                __FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif 

        ret = spm_hqos_release_pw_id(pst_tbl->pw_id);
        if (ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }
    

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_HQOS_VC_CB);
    pst_tbl = NULL;

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}



/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 分配内存的同时分配PW ID
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_del_vc_drive(NBB_BYTE slot,NBB_USHORT port,SPM_HQOS_VC_CB *vc)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num; 
    NBB_BYTE i = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vc)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }


    /***************************************************************************/
    /* 首先清除VC上的属性                                                    */
    /***************************************************************************/
    if(NULL != vc)
    {
        /*NP hqos 去使能*/
        if(0 != vc->np_flow_id || 0 != vc->posid)
        {
            for(unit = 0; unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                if(VC_MAIN == proflag)
                {
                    ret = ApiC3SetVpHqos(unit, vc->posid, 0, 0, vc->np_flow_id);
                }
                else
                {
                    ret = ApiC3SetVpHqos(unit, vc->posid, 1, 0, vc->np_flow_id);
                }
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_vp_error_log(vc->np_flow_id,0,vc->posid,vc->proflag,
                        __FUNCTION__,__LINE__,ret);
                    rv = ret;
                }
#endif
            }
        }


        /*删除voq*/
        if( 0 != vc->voq_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosVoqDelete(0, vc->voq_id, NUM_COS, slot, port, vc->tm_vc);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_voq_error_log(vc->voq_id,slot,port,
                    vc->tm_vc, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif    
         }

        /*删除vc*/
        for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
        {
            if(0 != vc->vc_id[i])
            {
#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosTrafficDelete(0, vc->voq_id, NUM_COS, g_hqos_port_index[i],
                        port, vc->vc_id[i],vc->pw_id);
                if (ATG_DCI_RC_OK != ret)       
                {
                    spm_hqos_arad_traffic_del_error_log(vc->voq_id,g_hqos_port_index[i],
                        port, vc->vc_id[i],vc->pw_id, __FUNCTION__,__LINE__,ret);
                    rv = ret;
                } 
#endif     
            }
        }
         
    }
  
   
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 分配内存的同时分配PW ID
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_del_vc_node(SPM_HQOS_LSP_CB **plsp,SPM_HQOS_VC_CB **pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_VC_CB *vc = NULL;
    SPM_HQOS_LSP_CB *lsp = NULL;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_BYTE i = 0;
    NBB_LONG lsp_id = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP节点不可能为NULL*/
    if(NULL == plsp || NULL == *plsp || NULL == pcb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    vc = *pcb;
    lsp = *plsp;
    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;

    /***************************************************************************/
    /* 首先清除VC上的属性                                                    */
    /***************************************************************************/
    if(NULL != vc)
    {
        ret = spm_hqos_del_vc_drive(slot,port,vc);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }

         /*注册性能告警*/
#ifdef PTN690
         spm_hqos_del_vc_pmline(vc->vc_key);
#endif


        /*正常删除已存在的正确配置*/
        if(AVLL_IN_TREE(vc->spm_hqos_vc_node)
        {
            AVLL_DELETE(lsp->vc_tree, vc->spm_hqos_vc_node);
        }

        /*释放内存资源*/
        ret = spm_free_hqos_vc_cb(vc,lsp_id);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }
        *pcb = NULL;/*指针清空为NULL*/ 
         
    }
  
    ret = spm_hqos_del_lsp_node(plsp);
    if (ATG_DCI_RC_OK != ret) 
    {
        rv = ret;
    }

   
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 创建出口盘的VC
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
static NBB_LONG spm_hqos_set_vc_pw_property(NBB_BYTE slot,NBB_LONG pw_id,
    NBB_ULONG posid,NBB_LONG base_queue,NBB_BYTE proflag,
    ATG_DCI_VPN_HQOS_POLICY *data,SPM_HQOS_VC_CB *pcb)
{
    NBB_LONG base_queue = 0;
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }


    ret = spm_hqos_set_pw_property(slot,pw_id,base_queue,data);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    

     
    /*入口盘和出口盘都会执行*/
    /*最后设置C3的flow id hqos的业务通起来*/
    for(unit = 0; unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        if(VC_MAIN == proflag)
        {
            ret = ApiC3SetVpHqos(unit, posid, 0, 1,base_queue);
        }
        else
        {
            ret = ApiC3SetVpHqos(unit, posid, 1, 1, base_queue);
        }
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_vp_error_log(base_queue,1,posid,proflag,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    if(NULL != pcb)
    {
        pcb->np_flow_id = base_queue;
        pcb->posid = posid;
        pcb->proflag = proflag;

        /*保存数据，数据插入相应的树中*/
        OS_MEMCPY(&(pcb->vc_data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY)); 
    }
    

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}




/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG* spm_hqos_set_vc_drive(NBB_BYTE slot,NBB_USHORT port, NBB_LONG pw_id,
        NBB_ULONG posid,NBB_BYTE proflag, 
        ATG_DCI_VPN_HQOS_POLICY *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_VC_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data)|| (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    /*创建入口盘出口的voq与vc的链接关系HQOS业务才能通*/
    if(NULL == pcb)/*根据保存的配置进行刷新驱动*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,NULL,NULL,NULL);  
    }
    else/*根据收到的配置处理并保存配置*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,&(pcb->voq_id),&(pcb->tm_vc),pcb->vc_id);  
    }
    
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }


    /*设置VC节点的HQOS策略生效*/
    ret = spm_hqos_set_vc_pw_property(slot,posid,voq,proflag,data,pcb);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }


    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}




/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG spm_hqos_add_vc(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
        NBB_BYTE proflag,SPM_QOS_TUNNEL_KEY *lsp_key, 
        ATG_DCI_VC_KEY *vc_key,ATG_DCI_VPN_HQOS_POLICY *data,
        NBB_LONG voq,NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_LONG unit = 0;
    SPM_HQOS_VC_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    /*查找是否存在B节点*/
    plsp = AVLL_FIND(g_qos_port_cb[slot][port].lsp_tree, lsp_key);

    /*LSP节点HQOS不存在，创建默认的LSP节点*/
    if (plsp == NULL)
    {     
        spm_dbg_record_hqos_vc_head(vc_key,SPM_OPER_ADD);

        plsp = spm_alloc_hqos_lsp_cb(lsp_key,ATG_DCI_RC_OK,slot,port);/*不申请LSP ID*/

        /*申请内存失败*/
        if (NULL == plsp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        pcb = AVLL_FIND(plsp->vc_tree, vc_key);
        if(NULL != pcb)/*异常这里应该为NULL*/
        {
            spm_qos_exception_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        pcb = spm_alloc_hqos_vc_cb(vc_key,plsp->lsp_id,slot,port);
        if (NULL == pcb)
        {
            /*释放资源*/
            spm_hqos_del_vc_node(&plsp,NULL);
            goto EXIT_LABEL;
        }

        ret = spm_hqos_set_vc_drive(slot,port,pcb->pw_id,posid,proflag,data,
            voq,vc,vc_id,pcb);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_del_vc_node(&plsp,&pcb);
            goto EXIT_LABEL;
        }

        /*注册性能告警*/
#ifdef PTN690
        spm_hqos_add_vc_pmline(vc_key,pcb->voq_id);
#endif

        rv = AVLL_INSERT(g_qos_port_cb[slot][port].lsp_tree, plsp->spm_hqos_lsp_tx_node);
        rv = AVLL_INSERT(plsp->vc_tree, pcb->spm_hqos_vc_node);
        
    }
    else/*LSP HQOS配置存在分默认LSP节点和非默认节点之分*/
    {
        pcb = AVLL_FIND(plsp->vc_tree, vc_key);
        if(NULL == pcb)/*新建节点*/
        {
            pcb = spm_alloc_hqos_vc_cb(vc_key,plsp->lsp_id,slot,port);
            if (NULL == pcb)
            {
                /*释放资源*/
                spm_hqos_del_vc_node(&plsp,&pcb);
                goto EXIT_LABEL;
            }

            ret = spm_hqos_set_vc_drive(slot,port,pcb->pw_id,posid,proflag,data,
                voq,vc,vc_id,pcb);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_hqos_del_vc_node(&plsp,&pcb);
                goto EXIT_LABEL;
            } 
            
            /*注册性能告警*/
#ifdef PTN690
            spm_hqos_add_vc_pmline(vc_key,pcb->voq_id);
#endif
            rv = AVLL_INSERT(plsp->vc_tree, pcb->spm_hqos_vc_node);
        }
        else/*更新*/
        {
            /*设置VC节点的HQOS配置*/
            ret = spm_hqos_set_vc_pw_property(slot,pcb->pw_id,posid,pcb->voq_id,
                proflag,data,pcb);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
        
    }
    
  

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_add_vc_node(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid, NBB_BYTE proflag,
    SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VC_KEY *vc_key,
    ATG_DCI_VPN_HQOS_POLICY *data)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VC_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    CRTXLSP_KEY lsptx_key = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG voq = 0;
    NBB_LONG vc = 0;
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM] = {0};


    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (port > 0x80))/*不支持LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }
    
    qos_log_vc_hqos_data(slot,port,posid,proflag,lsp_key,vc_key,data);
    
        
    /* 判断slot是否开启hqos的开关 */
    ret = spm_hqos_check_slot(slot,data->node_index,&voq,&vc,vc_id);
    if (ATG_DCI_RC_OK != ret)/*没有开启HQOS槽位使能直接跳过不报错*/
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }


    /*考虑由L3模块直接给我，不用自己调用*/
    if((0 != lsp_key->type) && (0 == lsp_key->tx_lsp.lspid)) /*LSP类型为CRLSP_TX*/
    {
        OS_MEMCPY(&lsptx_key,&(lsp_key->tx_lsp),sizeof(CRTXLSP_KEY));
        ret = spm_l3_gettxlspid(&lsptx_key, &(lsp_key->tx_lsp.lspid) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(message,"%s,%d spm_l3_rsvpforoam err=%ld\n\n",
                       __FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			HQOS_VC_ERROR,ret,
        			message,__FUNCTION__,"line","spm_l3_rsvpforoam",
        			0,0,__LINE__,0));

            /*异常跳出*/
            goto EXIT_LABEL;
        }
    }
    
    ret = spm_hqos_add_vc( slot,  port,  posid,proflag, lsp_key, vc_key, *data,
                    voq, vc, vc_id[MAX_HQOS_SLOT_NUM]);

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif



#if 6







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
void qos_log_vrf_hqos_data(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
    SPM_QOS_TUNNEL_KEY *lsp_key,,ATG_DCI_VC_KEY *vc_key,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_vc_hqos_cfg(data);
    
    if(ATG_DCI_RC_OK != g_qos_hqos_log)
    {
        OS_SPRINTF(uc_message,"slot=%d,port=%d\n",
                  slot,port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        qos_log_lsp_key(lsp_key);
        qos_log_vc_key(vc_key);
        qos_log_vc_hqos_cfg(data);
    }

   
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 分配内存的同时分配PW ID
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static SPM_HQOS_VC_CB *spm_alloc_hqos_vrf_cb(ATG_DCI_VRF_INSTANCE_KEY *vrf_key,
    NBB_LONG lsp_id,NBB_BYTE slot,NBB_USHORT port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VRF_CB *pst_tbl = NULL;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_ULONG id = 0;
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vrf_key)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        
        /*异常跳出*/
        goto EXIT_LABEL;
    }

    /*出口盘申请pw资源*/
    if(slot == v_spm_shared->local_slot_id)
    {
        /*id资源不足比内存不足的概率大很多*/
        ret = spm_hqos_apply_pw_id(&id);

        /*申请pwid失败*/
        if (ATG_DCI_RC_OK != ret)
        {
            /*异常跳出*/
            goto EXIT_LABEL;
        }

#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwCreate(unit, port,lsp_id,id,0);

        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_release_pw_id(id);
            spm_hqos_arad_create_pw_error_log(port ,lsp_id,
                id,__FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif    
    }



    /* 分配一个新的逻辑配置块qos表配置条目。*/
    pst_tbl = (SPM_HQOS_VRF_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_VRF_CB),
        NBB_NORETRY_ACT, MEM_SPM_HQOS_VRF_CB);

    /*分配内存失败不释放HQOS资源，内存没有了，基本就挂了*/
    if (pst_tbl == NULL)
    {
        /*异常跳出*/
        goto EXIT_LABEL;
    }
    
    

    /* 初始逻辑配置块配置条目 */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_HQOS_VRF_CB));
    OS_MEMCPY(&(pst_tbl->vrf_key), vrf_key, sizeof(ATG_DCI_VRF_INSTANCE_KEY));
    pst_tbl->pw_id = id;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_hqos_vrf_node);

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_free_hqos_vrf_cb(SPM_HQOS_VC_CB *pst_tbl,NBB_LONG lsp_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_TRC_ENTRY(__FUNCTION__);

    /*释放内存失败*/
    if (NULL == pst_tbl)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_VC_CB), MEM_SPM_HQOS_VC_CB);
    
    if(0 != pst_tbl->pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwDelete(0, pst_tbl->pw_id,lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_pw_error_log(lsp_id,pst_tbl->pw_id,
                __FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif 

        ret = spm_hqos_release_pw_id(pst_tbl->pw_id);
        if (ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }
    

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_HQOS_VC_CB);
    pst_tbl = NULL;

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 分配内存的同时分配PW ID
   输入参数  : defaulte = ATG_DCI_RC_OK:申请LSP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_del_vrf_node(SPM_HQOS_LSP_CB **plsp,SPM_HQOS_VRF_CB **pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_VRF_CB *vrf = NULL;
    SPM_HQOS_LSP_CB *lsp = NULL;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_BYTE i = 0;
    NBB_LONG lsp_id = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP节点不可能为NULL*/
    if(NULL == plsp || NULL == *plsp || NULL == pcb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    vrf = *pcb;
    lsp = *plsp;
    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;

    /***************************************************************************/
    /* 首先清除VC上的属性                                                    */
    /***************************************************************************/
    if(NULL != vrf)
    {
        /*NP hqos 去使能*/
        if(0 != vrf->np_flow_id || 0 != vrf->posid)
        {
            for(unit = 0; unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetL3TxPwHqos(unit, vrf->posid, 0, vrf->np_flow_id);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_l3txpw_error_log(vrf->np_flow_id,0,vrf->posid,
                        __FUNCTION__,__LINE__,ret);
                    rv = ret;
                }
#endif
            }
        }


        /*删除voq*/
        if( 0 != vrf->voq_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosVoqDelete(0, vrf->voq_id, NUM_COS, slot, port, vrf->tm_vc);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_voq_error_log(vrf->voq_id,slot,port,
                    vrf->tm_vc, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif    
         }

        /*删除vc*/
        for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
        {
            if(0 != vrf->vc_id[i])
            {
#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosTrafficDelete(0, vrf->voq_id, NUM_COS, g_hqos_port_index[i],
                        port, vrf->vc_id[i],vrf->pw_id);
                if (ATG_DCI_RC_OK != ret)       
                {
                    spm_hqos_arad_traffic_del_error_log(vrf->voq_id,g_hqos_port_index[i],
                        port, vrf->vc_id[i],vrf->pw_id, __FUNCTION__,__LINE__,ret);
                    rv = ret;
                } 
#endif     
            }
        }

         /*注册性能告警*/
#ifdef PTN690
         spm_hqos_del_vrf_pmline(vrf->vrf_key);
#endif


        /*正常删除已存在的正确配置*/
        if(AVLL_IN_TREE(vrf->spm_hqos_vrf_node)
        {
            AVLL_DELETE(lsp->vrf_tree, vrf->spm_hqos_vrf_node);
        }

        /*释放内存资源*/
        ret = spm_free_hqos_vrf_cb(vrf,lsp_id);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }
        *pcb = NULL;/*指针清空为NULL*/ 
         
    }
  
    ret = spm_hqos_del_lsp_node(plsp);
    if (ATG_DCI_RC_OK != ret) 
    {
        rv = ret;
    }

   
    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 创建出口盘的VC
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
static NBB_LONG spm_hqos_set_vrf_pw_property(NBB_BYTE slot,SPM_HQOS_VRF_CB *pcb,
    NBB_ULONG posid,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_LONG base_queue = 0;
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_LONG pw_id = 0;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data || NULL == pcb)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    pw_id = pcb->pw_id;
    base_queue = pcb->voq_id;


    ret = spm_hqos_set_pw_property(slot,pw_id,base_queue,data);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

   
     
    /*入口盘和出口盘都会执行*/
    /*最后设置C3的flow id hqos的业务通起来*/
    for(unit = 0; unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL3TxPwHqos(unit, posid, 1, pcb->voq_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_l3txpw_error_log(pcb->voq_id,1,posid,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }
    pcb->np_flow_id = base_queue;
    pcb->posid = posid;

    /*保存数据，数据插入相应的树中*/
    OS_MEMCPY(&(pcb->vrf_data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY)); 

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}




/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static SPM_HQOS_VRF_CB* spm_hqos_create_vrf_pw(NBB_BYTE slot,NBB_USHORT port, 
        SPM_HQOS_LSP_CB *plsp,
        NBB_ULONG posid,ATG_DCI_VRF_INSTANCE_KEY *vrf_key,
        ATG_DCI_VPN_HQOS_POLICY *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_VRF_CB *pcb = NULL;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vrf_key) || (NULL == plsp) || (NULL == data) 
        || (0 == posid) || (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }

    
    pcb = spm_alloc_hqos_vrf_cb(vrf_key,plsp->lsp_id,slot,port);
    if (NULL == pcb)
    {
        spm_hqos_del_vrf_node(&plsp,&pcb);
        goto EXIT_LABEL;
    }

    /*创建入口盘出口的voq与vc的链接关系HQOS业务才能通*/
    ret = spm_hqos_create_voq_vc(slot,port,pcb->pw_id,
        voq,vc,vc_id,&(pcb->voq_id),&(pcb->tm_vc),pcb->vc_id);
    if (ATG_DCI_RC_OK != ret)
    {
        spm_hqos_del_vrf_node(&plsp,&pcb);
        goto EXIT_LABEL;
    }
     


    /*设置VC节点的HQOS策略生效*/
    ret = spm_hqos_set_vrf_pw_property(slot,pcb,posid,data);
    if (ATG_DCI_RC_OK != ret)
    {
        spm_hqos_del_vrf_node(&plsp,&pcb);
        goto EXIT_LABEL;
    }

    /*注册性能告警*/
#ifdef PTN690
    spm_hqos_add_vrf_pmline(vrf_key,pcb->voq_id);
#endif

    /*保存数据，数据插入相应的树中*/
    OS_MEMCPY(&(pcb->vrf_data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY));

    rv = AVLL_INSERT(g_qos_port_cb[slot][port].lsp_tree, plsp->spm_hqos_lsp_tx_node);
    rv = AVLL_INSERT(plsp->vrf_tree, pcb->spm_hqos_vrf_node);

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return pcb;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
static NBB_LONG spm_hqos_add_vrf(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
        SPM_QOS_TUNNEL_KEY *lsp_key, 
        ATG_DCI_VRF_INSTANCE_KEY *vrf_key,ATG_DCI_VPN_HQOS_POLICY *data,
        NBB_LONG voq,NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_LONG unit = 0;
    SPM_HQOS_VRF_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vrf_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    /*查找是否存在B节点*/
    plsp = AVLL_FIND(g_qos_port_cb[slot][port].lsp_tree, lsp_key);

    /*LSP节点HQOS不存在，创建默认的LSP节点*/
    if (plsp == NULL)
    {     
        //spm_dbg_record_(vrf_key,SPM_OPER_ADD);
        plsp = spm_alloc_hqos_lsp_cb(lsp_key,ATG_DCI_RC_OK,slot,port);/*不申请LSP ID*/

        /*申请内存失败*/
        if (NULL == plsp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        pcb = AVLL_FIND(plsp->vrf_tree, vrf_key);
        if(NULL != pcb)/*异常这里应该为NULL*/
        {
            spm_qos_exception_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        pcb = spm_hqos_create_vrf_pw(slot,port,plsp,posid,vrf_key,data,voq,vc,vc_id);
        if(NULL == pcb)/*资源已经回收*/
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        
    }
    else/*LSP HQOS配置存在分默认LSP节点和非默认节点之分*/
    {
        pcb = AVLL_FIND(plsp->vrf_tree, vrf_key);
        if(NULL == pcb)/*新建节点*/
        {
            pcb = spm_hqos_create_vrf_pw(slot,port,plsp,posid,vrf_key,data,voq,vc,vc_id);
            if(NULL == pcb)/*资源已经回收*/
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

        }
        else/*更新*/
        {
            /*只更新TXPW 的hqos的flow id*/
            for(unit = 0; unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetL3TxPwHqos(unit, posid, 1, pcb->voq_id);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_l3txpw_error_log(pcb->voq_id,1,posid,
                        __FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }
#endif
            }
        }
        
    }
    

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_disconnect_usr_group_cnt
   功能描述  : 释放HQOS树的节点并释放内存
   输入参数  : upflag=0代表更新
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG spm_hqos_add_vrf_node(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
    SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VRF_INSTANCE_KEY *vrf_key,
    ATG_DCI_VPN_HQOS_POLICY *data)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VC_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    CRTXLSP_KEY lsptx_key = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG voq = 0;
    NBB_LONG vc = 0;
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM] = {0};
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (port > 0x80))/*不支持LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }
    
    qos_log_vc_hqos_data(slot,port,posid,lsp_key,vrf_key,data);
    
        
    /* 判断slot是否开启hqos的开关 */
    ret = spm_hqos_check_slot(slot,data->node_index,&voq,&vc,vc_id);
    if (ATG_DCI_RC_OK != ret)/*没有开启HQOS槽位使能直接跳过不报错*/
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }


    /*考虑由L3模块直接给我，不用自己调用*/
    if((0 != lsp_key->type) && (0 == lsp_key->tx_lsp.lspid)) /*LSP类型为CRLSP_TX*/
    {
        OS_MEMCPY(&lsptx_key,&(lsp_key->tx_lsp),sizeof(CRTXLSP_KEY));
        ret = spm_l3_gettxlspid(&lsptx_key, &(lsp_key->tx_lsp.lspid) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(message,"%s,%d spm_l3_rsvpforoam err=%ld\n\n",
                       __FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			HQOS_VC_ERROR,ret,
        			message,__FUNCTION__,"line","spm_l3_rsvpforoam",
        			0,0,__LINE__,0));

            /*异常跳出*/
            goto EXIT_LABEL;
        }
    }
    
    ret = spm_hqos_add_vc( slot,  port,  posid,proflag, lsp_key, vc_key, *data,
                    voq, vc, vc_id[MAX_HQOS_SLOT_NUM]);

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif

