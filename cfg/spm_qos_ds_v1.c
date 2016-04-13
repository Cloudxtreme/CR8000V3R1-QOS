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
#define DS_TAG       "QOS DS CFG"

/*DS模块全局打印开关*/
NBB_BYTE g_qos_ds_print = ATG_DCI_RC_OK;

/*DS模块全局LOG开关*/
NBB_BYTE g_qos_ds_log = ATG_DCI_RC_OK;

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
NBB_VOID qos_set_ds_print(NBB_BYTE set)
{
    g_qos_ds_print = set;
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
NBB_VOID qos_set_ds_log(NBB_BYTE set)
{
    g_qos_ds_log = set;
}



#if 1

/*****************************************************************/
/****************************驱动LOG报错函数*********************/
/*****************************************************************/



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
static NBB_VOID  spm_api_c3_set_exp2phb_err(NBB_LONG unit,NBB_USHORT dsptr,
       NBB_BYTE exp,NBB_BYTE phb,NBB_BYTE color,
       const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetExpToPhb ret=%d:\n"
                  "unit = %d,dsptr = %d,exp=%d,phb=%d,color=%d\n\n",
                  function,line,ret,unit,dsptr,exp,phb,color);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetExpToPhb ret=%d:\n"
                  "unit = %d,dsptr = %d,exp=%d,phb=%d,color=%d\n\n",
                  function,line,ret,unit,dsptr,exp,phb,color);
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
static NBB_VOID  spm_api_c3_set_dscp2phb_err(NBB_LONG unit,NBB_USHORT dsptr,
       NBB_BYTE dscp,NBB_BYTE phb,NBB_BYTE color,
       const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetDscpToPhb ret=%d:\n"
                  "unit = %d,dsptr = %d,dscp=%d,phb=%d,color=%d\n\n",
                  function,line,ret,unit,dsptr,dscp,phb,color);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetDscpToPhb ret=%d:\n"
                  "unit = %d,dsptr = %d,dscp=%d,phb=%d,color=%d\n\n",
                  function,line,ret,unit,dsptr,dscp,phb,color);
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
static NBB_VOID  spm_api_c3_set_pri2phb_err(NBB_LONG unit,NBB_USHORT dsptr,
       NBB_BYTE pri,NBB_BYTE phb,NBB_BYTE color,
       const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetPriToPhb ret=%d:\n"
                  "unit = %d,dsptr = %d,pri=%d,phb=%d,color=%d\n\n",
                  function,line,ret,unit,dsptr,pri,phb,color);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetPriToPhb ret=%d:\n"
                  "unit = %d,dsptr = %d,pri=%d,phb=%d,color=%d\n\n",
                  function,line,ret,unit,dsptr,pri,phb,color);
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
static NBB_VOID  spm_api_c3_set_phb2dscp_err(NBB_LONG unit,NBB_USHORT dsptr,
       NBB_BYTE phb,NBB_BYTE color,NBB_BYTE dscp, const NBB_CHAR *function,
       NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetPhbToDscp ret=%d:\n"
                  "unit = %d,dsptr = %d,phb=%d,color=%d,dscp=%d\n\n",
                  function,line,ret,unit,dsptr,phb,color,dscp);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetPhbToDscp ret=%d:\n"
                  "unit = %d,dsptr = %d,phb=%d,color=%d,dscp=%d\n\n",
                  function,line,ret,unit,dsptr,phb,color,dscp);
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
static NBB_VOID  spm_api_c3_set_phb2exp_err(NBB_LONG unit,NBB_USHORT dsptr,
       NBB_BYTE phb,NBB_BYTE color,NBB_BYTE exp, const NBB_CHAR *function,
       NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetPhbToExp ret=%d:\n"
                  "unit = %d,dsptr = %d,phb=%d,color=%d,exp=%d\n\n",
                  function,line,ret,unit,dsptr,phb,color,exp);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetPhbToExp ret=%d:\n"
                  "unit = %d,dsptr = %d,phb=%d,color=%d,exp=%d\n\n",
                  function,line,ret,unit,dsptr,phb,color,exp);
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
static NBB_VOID  spm_api_c3_set_phb2pri_err(NBB_LONG unit,NBB_USHORT dsptr,
       NBB_BYTE phb,NBB_BYTE color,NBB_BYTE pri, const NBB_CHAR *function,
       NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetPhbToPri ret=%d:\n"
                  "unit = %d,dsptr = %d,phb=%d,color=%d,pri=%d\n\n",
                  function,line,ret,unit,dsptr,phb,color,pri);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetPhbToPri ret=%d:\n"
                  "unit = %d,dsptr = %d,phb=%d,color=%d,pri=%d\n\n",
                  function,line,ret,unit,dsptr,phb,color,pri);
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
static NBB_VOID  spm_api_c3_set_l2unipri_err(NBB_LONG unit,NBB_ULONG vpid,
    NBB_USHORT mode,NBB_USHORT dsptr,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("(%s) line = %ld  ApiC3SetL2UniNhiPri ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,vpid,mode,dsptr);
        OS_SPRINTF(uc_message,"(%s) line = %ld  ApiC3SetL2UniNhiPri ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,vpid,mode,dsptr);
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
static NBB_VOID  spm_api_c3_set_intf_phb_err(NBB_LONG unit,NBB_ULONG posid,
    NBB_USHORT mode,NBB_USHORT dsptr,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetIntfPhb ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetIntfPhb ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
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
static NBB_VOID  spm_api_c3_set_l3uni_pri_err(NBB_LONG unit,NBB_ULONG posid,
    TX_PORT_T *txport,NBB_USHORT mode,NBB_USHORT dsptr,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if((NULL != function))
    {
        if(NULL != txport)
        {
            printf("%s line = %ld  ApiC3SetL3UniPri ret=%d: \n"
                  "unit = %ld, posid = %ld,slot=%d,port=%d,vid=%d,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,txport->slot,txport->port,
                  txport->vid,mode,dsptr);
            OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetL3UniPri ret=%d: \n"
                      "unit = %ld, posid = %ld,slot=%d,port=%d,vid=%d,mode = %d, dsptr = %d\n\n",
                      function,line,ret,unit,posid,txport->slot,txport->port,
                      txport->vid,mode,dsptr);   
        }
        else
        {
            printf("%s line = %ld  ApiC3SetL3UniPri ret=%d: \n"
                  "unit = %ld, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
            OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetL3UniPri ret=%d: \n"
                  "unit = %ld, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        }
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
static NBB_VOID  spm_api_c3_set_vpin_phb_err(NBB_LONG unit,NBB_ULONG posid,
    NBB_USHORT mode,NBB_USHORT dsptr,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetVpIngPhb ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetVpIngPhb ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
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
static NBB_VOID  spm_api_c3_set_vpin_backup_phb_err(NBB_LONG unit,NBB_ULONG posid,
    NBB_USHORT mode,NBB_USHORT dsptr,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetVpIngBackupPhb ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetVpIngBackupPhb ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
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
static NBB_VOID  spm_api_c3_set_vpegr_phb_err(NBB_LONG unit,NBB_ULONG posid,
    NBB_USHORT dsptr,NBB_USHORT mode,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetVpEgrExp ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetVpEgrExp ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
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
static NBB_VOID  spm_api_c3_set_vpegr_backup_phb_err(NBB_LONG unit,NBB_ULONG posid,
    NBB_USHORT dsptr,NBB_USHORT mode,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetVpEgrBackupExp ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetVpEgrBackupExp ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
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
static NBB_VOID  spm_api_c3_set_txpw_exp_err(NBB_LONG unit,NBB_ULONG posid,
    NBB_USHORT dsptr,NBB_USHORT mode,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetTxpwExp ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetTxpwExp ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d, dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
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
static NBB_VOID  spm_api_c3_set_txlsp_exp_err(NBB_LONG unit,NBB_ULONG posid,
    NBB_USHORT mode,NBB_BYTE exp,NBB_USHORT dsptr,const NBB_CHAR *function,
    NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    if(NULL != function)
    {
        printf("%s line = %ld  ApiC3SetTxLspInExp ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d,exp=%d,dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetTxLspInExp ret=%d:\n"
                  "unit = %d, posid = %ld,mode = %d,exp=%d,dsptr = %d\n\n",
                  function,line,ret,unit,posid,mode,dsptr);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }  
}

 

#endif



#if 2

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
static NBB_LONG spm_set_phb2vlanpri_map(NBB_USHORT dsptr, 
 NBB_BYTE map[ATG_DCI_MAX_PHB_NUM][ATG_DCI_MAX_COLOR_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT k = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* 指针为空 */
    if (NULL == map)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for (i = 0; i < MAX_PHB_NUM; i++)
    {
        for (j = QOS_COLOR_GREEN, k = 0; j < MAX_COLOR_NUM; j++, k++)
        {
            for(unit = 0;unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToPri(unit, dsptr, i, j, map[i][k]);               
                if (ATG_DCI_RC_OK != ret)
                {
            	    spm_api_c3_set_phb2pri_err(unit, dsptr, i, j, map[i][k],
                        __FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }  
#endif   
            } 
        }
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
static NBB_LONG spm_set_phb2exppri_map(NBB_USHORT dsptr,
    NBB_BYTE map[ATG_DCI_MAX_PHB_NUM][ATG_DCI_MAX_COLOR_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT k = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*指针为空*/
    if (NULL == map)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for (i = 0; i < MAX_PHB_NUM; i++)
    {
        for (j = QOS_COLOR_GREEN, k = 0; j < MAX_COLOR_NUM; j++, k++)
        {
            for(unit = 0;unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToExp(unit, dsptr, i, j, map[i][k]);          
                if (ATG_DCI_RC_OK != ret)
                {
            	    spm_api_c3_set_phb2exp_err(unit, dsptr, i, j, map[i][k],
                        __FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }
#endif  
            } 
        }
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
static NBB_LONG spm_set_phb2dscppri_map(NBB_USHORT dsptr,
    NBB_BYTE map[ATG_DCI_MAX_PHB_NUM][ATG_DCI_MAX_COLOR_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT k = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*指针为空*/
    if (NULL == map)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for (i = 0; i < MAX_PHB_NUM; i++)
    {
        for (j = QOS_COLOR_GREEN, k = 0; j < MAX_COLOR_NUM; j++, k++)
        {
            for(unit = 0;unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToDscp(unit, dsptr, i, j, map[i][k]);          
                if (ATG_DCI_RC_OK != ret)
                {
            	    spm_api_c3_set_phb2dscp_err(unit, dsptr, i, j, map[i][k],
                        __FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }
#endif 
            }
        }
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
static NBB_LONG spm_set_vlanpri2phb_map(NBB_USHORT dsptr, 
    ATG_DCI_DS_DOMAIN_PHB_COLOR *map)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == map)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for (i = 0; i < MAX_VLAN_PRI_NUM; i++, map++)
    {
        for(unit = 0;unit < c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPriToPhb(unit, dsptr, i, map->phb, map->color);       
            if (ATG_DCI_RC_OK != ret)
            {
        	    spm_api_c3_set_pri2phb_err(unit, dsptr, i, map->phb, map->color,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
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
static NBB_LONG spm_set_exppri2phb_map(NBB_USHORT dsptr,
    ATG_DCI_DS_DOMAIN_PHB_COLOR *map)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == map)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for (i = 0; i < MAX_EXP_PRI_NUM; i++, map++)
    {
        for(unit = 0;unit < c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetExpToPhb(unit, dsptr, i, map->phb, map->color);        
            if (ATG_DCI_RC_OK != ret)
            {
        	    spm_api_c3_set_exp2phb_err(unit, dsptr, i, map->phb, map->color,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif 
        }
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
static NBB_LONG spm_set_dscppri2phb_map(NBB_USHORT dsptr, 
    ATG_DCI_DS_DOMAIN_PHB_COLOR *map)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == map)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    for (i = 0; i < ATG_DCI_DSCP_MAP_NUM; i++, map++)
    {
        for(unit = 0;unit < c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetDscpToPhb(unit, dsptr, i, map->phb, map->color);      
            if (ATG_DCI_RC_OK != ret)
            {
        	    spm_api_c3_set_dscp2phb_err(unit, dsptr, i, map->phb, map->color,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif 
        }
        
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
static NBB_LONG spm_set_default_exp2phb_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE c3num = 0;

    spm_hardw_getc3chipnum(&c3num);
    for(unit = 0;unit < c3num;unit++)
    {
        /* 默认的vlanpri到PHB映射 */
        for (i = 0; i < MAX_EXP_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetExpToPhb(unit, dsptr, i, i, QOS_COLOR_GREEN);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_exp2phb_err(unit, dsptr, i, i, QOS_COLOR_GREEN,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
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
static NBB_LONG spm_set_default_dscp2phb_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE c3num = 0;
#if defined (SPU) || defined (PTN690_CES)
    SPM_QOS_PRI2PHB_MAP ds_pri2phb_map[MAX_DSCP_PRI_NUM] = {
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_EF, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_EF, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_CS6, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_CS7, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN}
    };
#endif

    spm_hardw_getc3chipnum(&c3num);
    for(unit = 0;unit < c3num;unit++)
    {
        for (i = 0; i < MAX_DSCP_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetDscpToPhb(unit,dsptr,i,ds_pri2phb_map[i].phb,ds_pri2phb_map[i].color);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_dscp2phb_err(unit,dsptr,i,ds_pri2phb_map[i].phb,
                    ds_pri2phb_map[i].color,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
    } 

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
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
static NBB_LONG spm_set_default_vlan2phb_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE c3num = 0;

    spm_hardw_getc3chipnum(&c3num);
    for(unit = 0;unit < c3num;unit++)
    {
        /* 默认的vlanpri到PHB映射 */
        for (i = 0; i < MAX_VLAN_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPriToPhb(unit, dsptr, i, i, QOS_COLOR_GREEN);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_pri2phb_err(unit, dsptr, i, i, QOS_COLOR_GREEN,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }    
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
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
static NBB_LONG spm_set_default_pri2phb_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_BYTE c3num = 0;

#if defined (SPU) || defined (PTN690_CES)
    SPM_QOS_PRI2PHB_MAP ds_pri2phb_map[MAX_DSCP_PRI_NUM] = {
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF1, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF2, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF3, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_YELLOW},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_AF4, QOS_COLOR_RED},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_EF, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_EF, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_CS6, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_CS7, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN},
        {PHB_BE, QOS_COLOR_GREEN}
    };
#endif

    spm_hardw_getc3chipnum(&c3num);
    for(unit = 0;unit < c3num;unit++)
    {
        /* 默认的vlanpri到PHB映射 */
        for (i = 0; i < MAX_VLAN_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPriToPhb(unit, dsptr, i, i, QOS_COLOR_GREEN);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_pri2phb_err(unit, dsptr, i, i, QOS_COLOR_GREEN,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }

        /* 默认的exp到PHB映射 */
        for (i = 0; i < MAX_EXP_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetExpToPhb(unit, dsptr, i, i, QOS_COLOR_GREEN);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_exp2phb_err(unit, dsptr, i, i, QOS_COLOR_GREEN,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }

        /* 默认的dscp到PHB映射 */
        for (i = 0; i < MAX_DSCP_PRI_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetDscpToPhb(unit,dsptr,i,ds_pri2phb_map[i].phb,ds_pri2phb_map[i].color);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_dscp2phb_err(unit,dsptr,i,ds_pri2phb_map[i].phb,
                    ds_pri2phb_map[i].color,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }       
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
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
static NBB_LONG spm_set_default_phb2exp_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    
    //NBB_USHORT offset = 0;
    NBB_BYTE c3num = 0;

    spm_hardw_getc3chipnum(&c3num);
    
    for(unit = 0;unit < c3num;unit++)
    {
        /* phb到exp映射 */
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_GREEN, i);
            if (ATG_DCI_RC_OK != ret)
            {
              spm_api_c3_set_phb2exp_err(unit, dsptr, i, QOS_COLOR_GREEN, i,
                        __FUNCTION__,__LINE__,ret);
              goto EXIT_LABEL;
            }
#endif
        }

        /* phb到exp映射 */
        for (i = PHB_AF1; i < PHB_EF; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_YELLOW, i);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_phb2exp_err(unit, dsptr, i, QOS_COLOR_YELLOW, i,
                        __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_RED, i);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_phb2exp_err(unit, dsptr, i, QOS_COLOR_RED, i,
                        __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        } 
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
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
static NBB_LONG spm_set_default_phb2dscp_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT offset = 0;
    NBB_BYTE c3num = 0;

    spm_hardw_getc3chipnum(&c3num);
    for(unit = 0;unit < c3num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_BE, QOS_COLOR_GREEN, 0);
        if (ATG_DCI_RC_OK != ret)
        {
           spm_api_c3_set_phb2dscp_err(unit, dsptr, PHB_BE, QOS_COLOR_GREEN, 0,
                        __FUNCTION__,__LINE__,ret);
           goto EXIT_LABEL;
        }
#endif
        for (i = PHB_AF1, offset = 10; i < PHB_EF; i++, offset += 2)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++, offset += 2)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToDscp(unit, dsptr, i, j, offset);
                if (ATG_DCI_RC_OK != ret)
                {
                   spm_api_c3_set_phb2dscp_err(unit, dsptr, i, j, offset,
                        __FUNCTION__,__LINE__,ret);
                   goto EXIT_LABEL;
                }
#endif
            }
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_EF, QOS_COLOR_GREEN, 46);
        if (ATG_DCI_RC_OK != ret)
        {
           spm_api_c3_set_phb2dscp_err(unit, dsptr, PHB_EF, QOS_COLOR_GREEN, 46,
                        __FUNCTION__,__LINE__,ret);
           goto EXIT_LABEL;
        }
#endif
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_CS6, QOS_COLOR_GREEN, 48);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_phb2dscp_err(unit, dsptr, PHB_CS6, QOS_COLOR_GREEN, 48,
                        __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_CS7, QOS_COLOR_GREEN, 56);
        if (ATG_DCI_RC_OK != ret)
        {
           spm_api_c3_set_phb2dscp_err(unit, dsptr, PHB_CS7, QOS_COLOR_GREEN, 56,
                        __FUNCTION__,__LINE__,ret);
           goto EXIT_LABEL;
        }  
#endif   
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
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
static NBB_LONG spm_set_default_phb2vlan_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    
    //NBB_USHORT offset = 0;
    NBB_BYTE c3num = 0;

    spm_hardw_getc3chipnum(&c3num);
    for(unit = 0;unit < c3num;unit++)
    {
        /* phb到vlan映射 */
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_GREEN, i);
            if (ATG_DCI_RC_OK != ret)
            {
               spm_api_c3_set_phb2pri_err(unit, dsptr, i, QOS_COLOR_GREEN, i,
                        __FUNCTION__,__LINE__,ret);
               goto EXIT_LABEL;
            }
#endif
        }

        /* phb到vlan映射 */
        for (i = PHB_AF1; i < PHB_EF; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_YELLOW, i);
            if (ATG_DCI_RC_OK != ret)
            {
              spm_api_c3_set_phb2pri_err(unit, dsptr, i, QOS_COLOR_YELLOW, i,
                        __FUNCTION__,__LINE__,ret);
              goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_RED, i);
            if (ATG_DCI_RC_OK != ret)
            {
               spm_api_c3_set_phb2pri_err(unit, dsptr, i, QOS_COLOR_RED, i,
                        __FUNCTION__,__LINE__,ret);
               goto EXIT_LABEL;
            }
#endif
        }
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
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
static NBB_LONG spm_set_default_phb2pri_map(NBB_USHORT dsptr)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;
    NBB_USHORT offset = 0;
    NBB_BYTE c3num = 0;

    spm_hardw_getc3chipnum(&c3num);
    for(unit = 0;unit < c3num;unit++)
    {
        /* phb到vlan映射 */
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_GREEN, i);
            if (ATG_DCI_RC_OK != ret)
            {
               spm_api_c3_set_phb2pri_err(unit, dsptr, i, QOS_COLOR_GREEN, i,
                        __FUNCTION__,__LINE__,ret);
               goto EXIT_LABEL;
            }
#endif
        }

        /* phb到vlan映射 */
        for (i = PHB_AF1; i < PHB_EF; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_YELLOW, i);
            if (ATG_DCI_RC_OK != ret)
            {
              spm_api_c3_set_phb2pri_err(unit, dsptr, i, QOS_COLOR_YELLOW, i,
                        __FUNCTION__,__LINE__,ret);
              goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToPri(unit, dsptr, i, QOS_COLOR_RED, i);
            if (ATG_DCI_RC_OK != ret)
            {
               spm_api_c3_set_phb2pri_err(unit, dsptr, i, QOS_COLOR_RED, i,
                        __FUNCTION__,__LINE__,ret);
               goto EXIT_LABEL;
            }
#endif
        }

        /* phb到exp映射:默认一一映射,用于解决向绍虎mantis0072497 */
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
            for(j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToExp(unit, dsptr, i, j, i);
                if (ATG_DCI_RC_OK != ret)
                {
                  spm_api_c3_set_phb2exp_err(unit, dsptr, i, j, i,
                        __FUNCTION__,__LINE__,ret);
                  goto EXIT_LABEL;
                }
#endif
            }
        }

#if 0
        /* phb到exp映射 */
        for (i = PHB_AF1; i < PHB_EF; i++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_YELLOW, i);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetPhbToExp(unit, dsptr, i, QOS_COLOR_RED, i);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif
        }
#endif

        /* phb be到DSCP映射 */
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_BE, QOS_COLOR_GREEN, 0);
        if (ATG_DCI_RC_OK != ret)
        {
           spm_api_c3_set_phb2dscp_err(unit, dsptr, PHB_BE, QOS_COLOR_GREEN, 0,
                        __FUNCTION__,__LINE__,ret);
           goto EXIT_LABEL;
        }
#endif
        for (i = PHB_AF1, offset = 10; i < PHB_EF; i++, offset += 2)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++, offset += 2)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetPhbToDscp(unit, dsptr, i, j, offset);
                if (ATG_DCI_RC_OK != ret)
                {
                   spm_api_c3_set_phb2dscp_err(unit, dsptr, i, j, offset,
                        __FUNCTION__,__LINE__,ret);
                   goto EXIT_LABEL;
                }
#endif
            }
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_EF, QOS_COLOR_GREEN, 46);
        if (ATG_DCI_RC_OK != ret)
        {
           spm_api_c3_set_phb2dscp_err(unit, dsptr, PHB_EF, QOS_COLOR_GREEN, 46,
                        __FUNCTION__,__LINE__,ret);
           goto EXIT_LABEL;
        }
#endif
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_CS6, QOS_COLOR_GREEN, 48);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_phb2dscp_err(unit, dsptr, PHB_CS6, QOS_COLOR_GREEN, 48,
                        __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetPhbToDscp(unit, dsptr, PHB_CS7, QOS_COLOR_GREEN, 56);
        if (ATG_DCI_RC_OK != ret)
        {
           spm_api_c3_set_phb2dscp_err(unit, dsptr, PHB_CS7, QOS_COLOR_GREEN, 56,
                        __FUNCTION__,__LINE__,ret);
           goto EXIT_LABEL;
        }  
#endif
    }

#if defined (SPU) || defined (PTN690_CES)
    EXIT_LABEL:
#endif
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
static NBB_LONG spm_ds_pri2phb_map_init(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT dsptr = 0;

    for (dsptr = 0; dsptr < 16; dsptr++)
    {
        ret = spm_set_default_pri2phb_map(dsptr);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL:
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
static NBB_LONG spm_ds_phb2pri_map_init(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_USHORT dsptr = 0;

    for (dsptr = 0; dsptr < PHB2PRI_ASSIGN_OFFSET; dsptr++)
    {
        ret = spm_set_default_phb2pri_map(dsptr); 
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL:
    return ret;
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
NBB_LONG spm_ds_init(void)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_BYTE i = 0;
    NBB_BYTE j = 0;
    NBB_BYTE k = 0;
    NBB_USHORT dsptr = 0;
    NBB_BYTE c3num = 0;

    spm_hardw_getc3chipnum(&c3num);

    ret = spm_ds_pri2phb_map_init();
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    dsptr = PRI2PHB_ASSIGN_OFFSET;

    /* 初始化PRI到PHB的指配dsptr范围16-39 */
    for (i = 0; i < MAX_PHB_NUM; i++)
    {
        for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
        {
            for (k = 0; k < MAX_VLAN_PRI_NUM; k++)
            {
                for(unit = 0;unit < c3num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetPriToPhb(unit, dsptr, k, i, j);
#endif 
                    if (ATG_DCI_RC_OK != ret)
                    {
                        spm_api_c3_set_pri2phb_err(unit, dsptr, k, i, j,
                            __FUNCTION__,__LINE__,ret);
                        goto EXIT_LABEL;
                    }
                }
            }
            for (k = 0; k < MAX_DSCP_PRI_NUM; k++)
            {
                for(unit = 0;unit < c3num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetDscpToPhb(unit, dsptr, k, i, j);                 
#endif 
                    if (ATG_DCI_RC_OK != ret)
                    {
                        spm_api_c3_set_dscp2phb_err(unit, dsptr, k, i, j,
                            __FUNCTION__,__LINE__,ret);
                        goto EXIT_LABEL;
                    }
                }
            }
            for (k = 0; k < MAX_EXP_PRI_NUM; k++)
            {
                for(unit = 0;unit < c3num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetExpToPhb(unit, dsptr, k, i, j);            
#endif
                    if (ATG_DCI_RC_OK != ret)
                    {
                        spm_api_c3_set_exp2phb_err(unit, dsptr, k, i, j,
                            __FUNCTION__,__LINE__,ret);
                        goto EXIT_LABEL;
                    }
                }
            }
            dsptr++;
        }
    }

    ret = spm_ds_phb2pri_map_init();
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    /* 初始化PHB到PRI的指配dsptr范围16-79 */
    dsptr = PHB2PRI_ASSIGN_OFFSET;
    for (k = 0; k < MAX_VLAN_PRI_NUM; k++,dsptr++)
    {
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
            {           
                for(unit = 0;unit < c3num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetPhbToPri(unit, dsptr, i, j, k);               
#endif
                    if (ATG_DCI_RC_OK != ret)
                    {
                       spm_api_c3_set_phb2pri_err(unit, dsptr, i, j, k,
                        __FUNCTION__,__LINE__,ret);
                       goto EXIT_LABEL;
                    } 
                }
            }
        }
    }

    dsptr = PHB2PRI_ASSIGN_OFFSET;
    for (k = 0; k < MAX_DSCP_PRI_NUM; k++,dsptr++)
    {
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
            {           
                for(unit = 0;unit < c3num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetPhbToDscp(unit, dsptr, i, j, k);            
#endif
                    if (ATG_DCI_RC_OK != ret)
                    {
                       spm_api_c3_set_phb2dscp_err(unit, dsptr, i, j, k,
                        __FUNCTION__,__LINE__,ret);
                       goto EXIT_LABEL;
                    }  
                } 
            }
        }
    }

    dsptr = PHB2PRI_ASSIGN_OFFSET;
    for (k = 0; k < MAX_EXP_PRI_NUM; k++,dsptr++)
    {
        for (i = 0; i < MAX_PHB_NUM; i++)
        {
            for (j = QOS_COLOR_GREEN; j < MAX_COLOR_NUM; j++)
            {           
                for(unit = 0;unit < c3num;unit++)
                {
#if defined (SPU) || defined (PTN690_CES)
                    ret = ApiC3SetPhbToExp(unit, dsptr, i, j, k);             
#endif
                    if (ATG_DCI_RC_OK != ret)
                    {
                       spm_api_c3_set_phb2exp_err(unit, dsptr, i, j, k,
                        __FUNCTION__,__LINE__,ret);
                       goto EXIT_LABEL;
                    }  
                }  
            }
        }
    }

    EXIT_LABEL:
    return ret;
}


#endif


#if 3






/*****************************************************************************
   函 数 名  : spm_set_d2p_ds_domain_cfg
   功能描述  : ds域DSCP到PHB映射配置接收函数
   输入参数  : 子TLV的操作码，子配置指针的指针,ips消息的缓存
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_set_d2p_ds_domain_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_DSCP2PHBMAP *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*ips消息的动作*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = spm_set_default_dscp2phb_map(dsptr);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*如果没有配置数据*/
        if (NULL == buf)
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*有数据*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_DSCP2PHBMAP *)buf;


            /***************************************************************************/
            /*调用驱动配置函数                                            */
            /***************************************************************************/
            ret = spm_set_dscppri2phb_map(dsptr, ptemp->dscp_map);
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
   函 数 名  : spm_set_vlan2p_ds_domain_cfg
   功能描述  : ds域vlan到PHB映射配置接收函数
   输入参数  : 子TLV的操作码，子配置指针的指针,ips消息的缓存
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_set_vlan2p_ds_domain_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_V8021P2PHBMAP *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*ips消息的动作*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = spm_set_default_vlan2phb_map(dsptr);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*如果没有配置数据*/
        if (NULL == buf)
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*有数据*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_V8021P2PHBMAP *)buf;


            /***************************************************************************/
            /*调用驱动配置函数                                            */
            /***************************************************************************/
            ret = spm_set_vlanpri2phb_map(dsptr, ptemp->v8021p_map);
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
   函 数 名  : spm_set_exp2p_ds_domain_cfg
   功能描述  : ds域exp到PHB映射配置接收函数
   输入参数  : 子TLV的操作码，子配置指针的指针,ips消息的缓存
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_set_exp2p_ds_domain_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_EXP2PHBMAP *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*ips消息的动作*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = spm_set_default_exp2phb_map(dsptr);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*如果没有配置数据*/
        if (NULL == buf)
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*有数据*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_EXP2PHBMAP *)buf;


            /***************************************************************************/
            /*调用驱动配置函数                                            */
            /***************************************************************************/
            ret = spm_set_exppri2phb_map(dsptr, ptemp->exp_map);
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
   函 数 名  : spm_set_p2d_ds_domain_cfg
   功能描述  : ds域PHB到dscp映射配置接收函数
   输入参数  : ds域键值，子TLV的操作码，子配置指针的指针,ips消息的缓存
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_set_p2d_ds_domain_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_PHB2DSCPMAP *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*ips消息的动作*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:

        ret = spm_set_default_phb2dscp_map(dsptr);
        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*如果没有配置数据*/
        if (NULL == buf)
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*有数据*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_PHB2DSCPMAP *)buf;


            /***************************************************************************/
            /*调用驱动配置函数                                            */
            /***************************************************************************/
            ret = spm_set_phb2dscppri_map(dsptr, ptemp->phb_map_dscp);
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
   函 数 名  : spm_set_p2vlan_ds_domain_cfg
   功能描述  : ds域PHB到vlan映射配置接收函数
   输入参数  : ds域键值，子TLV的操作码，子配置指针的指针,ips消息的缓存
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_set_p2vlan_ds_domain_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_PHB2V8021PMAP *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*ips消息的动作*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        ret = spm_set_default_phb2dscp_map(dsptr);

        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*如果没有配置数据*/
        if (NULL == buf)
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*有数据*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_PHB2V8021PMAP *)buf;


            /***************************************************************************/
            /*调用驱动配置函数                                            */
            /***************************************************************************/
            ret = spm_set_phb2vlanpri_map(dsptr, ptemp->phb_map_8021p);
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
   函 数 名  : spm_set_p2exp_ds_domain_cfg
   功能描述  : ds域PHB到EXP映射配置接收函数
   输入参数  : ds域键值，子TLV的操作码，子配置指针的指针,ips消息的缓存
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_set_p2exp_ds_domain_cfg(NBB_USHORT dsptr,
    NBB_ULONG oper,NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_DS_DOMAIN_PHB2EXPMAP *ptemp = NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*ips消息的动作*/
    switch (oper)
    {
        /*更新*/
        case  ATG_DCI_OPER_UPDATE:
        break;

        /*删除*/
        case  ATG_DCI_OPER_DEL:
        
        ret = spm_set_default_phb2exp_map(dsptr);

        break;

        /*增加*/
        case  ATG_DCI_OPER_ADD:

        /*如果没有配置数据*/
        if (NULL == buf)
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /*有数据*/
        else
        {
            ptemp = (ATG_DCI_DS_DOMAIN_PHB2EXPMAP *)buf;


            /***************************************************************************/
            /*调用驱动配置函数                                            */
            /***************************************************************************/
            ret = spm_set_phb2exppri_map(dsptr, ptemp->phb_map_exp);
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
   函 数 名  : spm_rcv_dci_set_ds_domain
   功能描述  : DS域接收函数
   输入参数  : DS域IPS消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_ds_domain_v1(ATG_DCI_SET_DS_DOMAIN *pst_set_ips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG domain_key = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;


    /* IPS消息子配置偏移地址 */
    NBB_BYTE *d2p_data_start = NULL;
    NBB_BYTE *vlan2p_data_start = NULL;
    NBB_BYTE *exp2p_data_start = NULL;
    NBB_BYTE *p2dscp_data_start = NULL;
    NBB_BYTE *p2vlan_data_start = NULL;
    NBB_BYTE *p2exp_data_start = NULL;

    /*IPS消息子配置操作*/
    NBB_ULONG oper_dscp2phb = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_vlan2phb = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_exp2phb = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_phb2dscp = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_phb2vlan = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_phb2exp = ATG_DCI_OPER_NULL;

    /*逻辑处理流程*/
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pst_set_ips)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if ((0 == pst_set_ips->key) || (pst_set_ips->key > 15))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL*/
    pst_set_ips->return_code = ATG_DCI_RC_OK;
    domain_key = pst_set_ips->key;

    /* 获取子配置操作 */
    oper_dscp2phb = pst_set_ips->oper_dscp2PhbMap;
    oper_vlan2phb = pst_set_ips->oper_v8021p2PhbMap;
    oper_exp2phb = pst_set_ips->oper_exp2PhbMap;
    oper_phb2dscp = pst_set_ips->oper_phb2DscpMap;
    oper_phb2vlan = pst_set_ips->oper_phb2V8021pMap;
    oper_phb2exp = pst_set_ips->oper_phb2ExpMap;

    /* ips消息子配置数据偏移*/
    d2p_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_ips, &(pst_set_ips->dscp2PhbMap_data));
    vlan2p_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_ips, &(pst_set_ips->v8021p2PhbMap_data));
    exp2p_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_ips, &(pst_set_ips->exp2PhbMap_data));
    p2dscp_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_ips, &(pst_set_ips->phb2DscpMap_data));
    p2vlan_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_ips, &(pst_set_ips->phb2V8021pMap_data));
    p2exp_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_ips, &(pst_set_ips->phb2ExpMap_data));

    /*删除操作*/
    if (TRUE == pst_set_ips->delete_struct)
    {
       
    }  
    else/*增加/更新操作*/
    {
        
        /*子配置处理流程*/
        ret = spm_set_d2p_ds_domain_cfg(domain_key,oper_dscp2phb,d2p_data_start);

        /*子配置IPS消息返回值*/
        pst_set_ips->dscp2PhbMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                         VLAN to PHB映射                        */
        /***************************************************************************/
        ret = spm_set_vlan2p_ds_domain_cfg(domain_key,oper_vlan2phb,vlan2p_data_start);

        /*子配置IPS消息返回值*/
        pst_set_ips->v8021p2PhbMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                         EXP to PHB映射                        */
        /***************************************************************************/
        ret = spm_set_exp2p_ds_domain_cfg(domain_key,oper_exp2phb,exp2p_data_start);

        /*子配置IPS消息返回值*/
        pst_set_ips->exp2PhbMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                        phb到dscp映射                        */
        /***************************************************************************/
        ret = spm_set_p2d_ds_domain_cfg(domain_key,oper_phb2dscp,p2dscp_data_start);

        /*子配置IPS消息返回值*/
        pst_set_ips->phb2DscpMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                        phb到vlan映射                        */
        /***************************************************************************/
        ret = spm_set_p2vlan_ds_domain_cfg(domain_key,oper_phb2vlan,p2vlan_data_start);

        /*子配置IPS消息返回值*/
        pst_set_ips->phb2V8021pMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }

        /***************************************************************************/
        /*                        phb到exp映射                        */
        /***************************************************************************/
        ret = spm_set_p2exp_ds_domain_cfg(domain_key,oper_phb2exp,p2exp_data_start);

        /*子配置IPS消息返回值*/
        pst_set_ips->phb2ExpMap_return_code = ret;
        if (ATG_DCI_RC_UNSUCCESSFUL == ret)
        {
            ips_ret = ret;
        }
    }
    pst_set_ips->return_code = ips_ret;


    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}




#endif






#if 4

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
static void qos_print_l3_intf_diff_serv_cfg(ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff)
{
    if(NULL != diff)
    {
       if(ATG_DCI_RC_OK != g_qos_ds_print)
        {
            printf("svlan=%d,cvlan=%d\n"
                      "l3_in_ds_mod=%d,phb=%d,colour=%d,l3_in_diff_serv_id=%ld\n"
                      "l3_out_ds_mod=%d,dscp=%d,pri=%d,l3_out_diff_serv_id=%ld\n\n",
                      diff->svlan,diff->cvlan,diff->l3_in_ds_mod,diff->phb,
                      diff->colour,diff->l3_in_diff_serv_id,
                      diff->l3_out_ds_mod,diff->dscp,diff->pri,
                      diff->l3_out_diff_serv_id);
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
static void qos_log_l3_intf_diff_serv_data(NBB_ULONG logic_key,ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_l3_intf_diff_serv_cfg(diff);
    
    if(NULL != diff)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            
            OS_SPRINTF(uc_message,"(%s)logic_key =%ld,svlan=%d,cvlan=%d\n"
                      "l3_in_ds_mod=%d,phb=%d,colour=%d,l3_in_diff_serv_id=%ld\n"
                      "l3_out_ds_mod=%d,dscp=%d,pri=%d,l3_out_diff_serv_id=%ld\n\n",
                      DS_TAG,logic_key,
                      diff->svlan,diff->cvlan,diff->l3_in_ds_mod,diff->phb,
                      diff->colour,diff->l3_in_diff_serv_id,
                      diff->l3_out_ds_mod,diff->dscp,diff->pri,
                      diff->l3_out_diff_serv_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
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
NBB_LONG spm_ds_set_l3_intf_node(
    SUB_PORT               *sub_port,
    NBB_ULONG  log_key,
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serve) //INTF 配置块条目键值
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    TX_PORT_T node = {0};
    TX_PORT_T *pL3Uni = NULL;
    NBB_ULONG posid = 0;
    ING_UNI_MODE_E in_mod = DSCP_TO_PHB;
    EGR_UNI_MODE_E out_mod = PHB_TO_DSCP_PRI;
    NBB_USHORT in_dsptr = 0;
    NBB_USHORT out_dsptr = 0;
    NBB_BYTE unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);


    /*指针为空*/
    if ((NULL == sub_port) || (NULL == diff_serve))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    qos_log_l3_intf_diff_serv_data(log_key,diff_serve);

    posid = sub_port->posid;
    node.slot = sub_port->slot;
    node.port = sub_port->port;
    node.vid = sub_port->ovlan;
    pL3Uni = &node;

    if(v_spm_shared->local_slot_id == sub_port->slot)/*本槽位处理*/
    {
        if(0 == diff_serve->l3_in_ds_mod)/*指配*/
        {
            in_mod = DSCP_TO_PHB;
            in_dsptr = ((diff_serve->phb) * MAX_COLOR_NUM) + diff_serve->colour + PRI2PHB_ASSIGN_OFFSET;
        }
        else if(1 == diff_serve->l3_in_ds_mod)/*基于PRI*/
        {
            in_mod = PRI_TO_PHB;
            in_dsptr = diff_serve->l3_in_diff_serv_id;
        }
        else if(2 == diff_serve->l3_in_ds_mod)/*基于DSCP*/
        {
            in_mod = DSCP_TO_PHB;
            in_dsptr = diff_serve->l3_in_diff_serv_id;
        }
        else
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetIntfPhb(sub_port->unit, posid, in_mod, in_dsptr);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_intf_phb_err(sub_port->unit, posid,in_mod,in_dsptr,
                __FUNCTION__, __LINE__, ret); 
            goto EXIT_LABEL;
        }
#endif
    }
        

    if (0 == diff_serve->l3_out_ds_mod) /*指配DSCP*/
    {
        out_mod = PHB_TO_DSCP_PRI;
        out_dsptr = diff_serve->dscp + PHB2PRI_ASSIGN_OFFSET;
        
    }
    else if (1 == diff_serve->l3_out_ds_mod) /* 指配PRI */
    {
        out_mod = PHB_TO_PRI;
        out_dsptr = (diff_serve->pri) + PHB2PRI_ASSIGN_OFFSET;
    }
    else if (4 == diff_serve->l3_out_ds_mod) /*映射到PRI*/
    {
        out_mod = PHB_TO_PRI;
        out_dsptr = diff_serve->l3_out_diff_serv_id;
    }
    else if (5 == diff_serve->l3_out_ds_mod) /*映射到PRI*/
    {
        out_mod = PHB_TO_DSCP_PRI;
        out_dsptr = diff_serve->l3_out_diff_serv_id;
    }
    else
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for(unit = 0;unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL3UniPri(unit, 0, pL3Uni, out_mod, out_dsptr);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_l3uni_pri_err(unit, 0,pL3Uni,out_mod,out_dsptr,
                __FUNCTION__, __LINE__, ret); 
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
NBB_LONG spm_ds_set_l3out_uni(NBB_ULONG logic_key,NBB_ULONG posid,
            ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serve)
{
    NBB_INT ret = ATG_DCI_RC_OK;
    SPM_PORT_INFO_CB port_info = {0};
    EGR_UNI_MODE_E out_mod = PHB_TO_DSCP_PRI;
    NBB_USHORT in_dsptr = 0;
    NBB_USHORT out_dsptr = 0;
    NBB_BYTE unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    
    NBB_TRC_ENTRY(__FUNCTION__);


    if((NULL == diff_serve) || (0 == posid))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*异常跳出*/
        goto EXIT_LABEL;
    }

    qos_log_l3_intf_diff_serv_data(logic_key,diff_serve);
    
    ret = spm_get_portid_from_logical_port_index(logic_key, &port_info NBB_CCXT);
    if(0 != ret)
    {
        spm_qos_get_port_from_logical_port_index_err_log(logic_key,
            __FUNCTION__,__LINE__,ret);
        
        /*异常跳出*/
        goto EXIT_LABEL;
    }
    
    

    if (0 == diff_serve->l3_out_ds_mod) /*指配DSCP*/
    {
        out_mod = PHB_TO_DSCP_PRI;
        out_dsptr = diff_serve->dscp + PHB2PRI_ASSIGN_OFFSET;
        
    }
    else if (1 == diff_serve->l3_out_ds_mod) /* 指配PRI */
    {
        out_mod = PHB_TO_PRI;
        out_dsptr = (diff_serve->pri) + PHB2PRI_ASSIGN_OFFSET;
    }
    else if (4 == diff_serve->l3_out_ds_mod) /*映射到PRI*/
    {
        out_mod = PHB_TO_PRI;
        out_dsptr = diff_serve->l3_out_diff_serv_id;
    }
    else if (5 == diff_serve->l3_out_ds_mod) /*映射到PRI*/
    {
        out_mod = PHB_TO_DSCP_PRI;
        out_dsptr = diff_serve->l3_out_diff_serv_id;
    }
    else
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for(unit = 0;unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL3UniPri(unit, posid, NULL, out_mod, out_dsptr);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_l3uni_pri_err(unit, posid,NULL,out_mod,out_dsptr,
                __FUNCTION__, __LINE__, ret); 
            goto EXIT_LABEL;
        }
#endif
    }

    /*异常跳出*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}





#endif


#if 5

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
static void qos_print_l2_intf_diff_serv_cfg(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *diff)
{
    if(NULL != diff)
    {
       if(ATG_DCI_RC_OK != g_qos_ds_print)
        {
            printf("svlan=%d,cvlan=%d\n"
                      "l3_in_ds_mod=%d,phb=%d,colour=%d,l3_in_diff_serv_id=%ld\n\n",
                      diff->svlan,diff->cvlan,diff->l2_in_ds_mod,diff->phb,
                      diff->colour,diff->l2_in_diff_serv_id);
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
static void qos_log_l2_intf_diff_serv_data(NBB_ULONG logic_key,ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *diff)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_l2_intf_diff_serv_cfg(diff);
    
    if(NULL != diff)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            
            OS_SPRINTF(uc_message,"(%s)logic_key=%ld, svlan=%d,cvlan=%d\n"
                      "l3_in_ds_mod=%d,phb=%d,colour=%d,l3_in_diff_serv_id=%ld\n\n",
                      DS_TAG,logic_key,
                      diff->svlan,diff->cvlan,diff->l2_in_ds_mod,diff->phb,
                      diff->colour,diff->l2_in_diff_serv_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
    }
   
}

/*****************************************************************************
   函 数 名  : spm_ds_add_logic_flow_node
   功能描述  : 逻辑端口flow优先级映射的接口函数
   输入参数  : ds模板的index  ,逻辑端口flow的key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_l2_intf_node(SUB_PORT *sub_port, 
            NBB_ULONG log_key,ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *diff_serve)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_ULONG posid = 0;
    ING_UNI_MODE_E in_mod = PRI_TO_PHB;
    NBB_USHORT in_dsptr = 0;
    NBB_BYTE unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);



    /*指针为空*/
    if ((NULL == sub_port) || (NULL == diff_serve))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    qos_log_l2_intf_diff_serv_data(log_key,diff_serve);

    posid = sub_port->posid;

    if(v_spm_shared->local_slot_id == sub_port->slot)/*本槽位处理*/
    {
        if(0 == diff_serve->l2_in_ds_mod)/*指配*/
        {
            in_mod = PRI_TO_PHB;
            in_dsptr = ((diff_serve->phb) * MAX_COLOR_NUM) + diff_serve->colour + PRI2PHB_ASSIGN_OFFSET;
        }
        else if(1 == diff_serve->l2_in_ds_mod)/*基于PRI*/
        {
            in_mod = PRI_TO_PHB;
            in_dsptr = diff_serve->l2_in_diff_serv_id;
        }
        else if(2 == diff_serve->l2_in_ds_mod)/*基于DSCP*/
        {
            in_mod = DSCP_TO_PHB;
            in_dsptr = diff_serve->l2_in_diff_serv_id;
        }
        else
        {
            spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetIntfPhb(sub_port->unit, posid, in_mod, in_dsptr);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_intf_phb_err(sub_port->unit, posid,in_mod,in_dsptr,
                __FUNCTION__, __LINE__, ret); 
            goto EXIT_LABEL;
        }
#endif
    }
        


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
static void qos_print_l2_unipri_diff_serv_cfg(ATG_DCI_LOG_PORT_DS_L2_DATA *diff)
{
    if(NULL != diff)
    {
       if(ATG_DCI_RC_OK != g_qos_ds_print)
        {
            printf("l2_out_ds_mod=%d,dscp=%d,pri=%d,l2_out_diff_serv_id=%ld\n\n",
                      diff->l2_out_ds_mod,diff->dscp,diff->pri,
                      diff->l2_out_diff_serv_id);
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
static void qos_log_l2_unipri_diff_serv_data(NBB_ULONG logic_key,ATG_DCI_LOG_PORT_DS_L2_DATA *diff)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_l2_unipri_diff_serv_cfg(diff);
    
    if(NULL != diff)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            
            OS_SPRINTF(uc_message,"(%s)logic_key=%ld\n"
                   "l2_out_ds_mod=%d,dscp=%d,pri=%d,l2_out_diff_serv_id=%ld\n\n",
                      DS_TAG,logic_key,
                      diff->l2_out_ds_mod,diff->dscp,diff->pri,
                      diff->l2_out_diff_serv_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
    }
   
}


/*****************************************************************************
   函 数 名  : spm_ds_add_logic_flow_vpuni_node
   功能描述  : 逻辑端口flow优先级映射的接口函数
   输入参数  : ds模板的index  ,逻辑端口flow的key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_univp_node(NBB_ULONG logic_index, 
     NBB_ULONG vpid, ATG_DCI_LOG_PORT_DS_L2_DATA *diff_serve)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_ULONG posid = 0;
    EGR_UNI_MODE_E out_mod = PHB_TO_PRI;
    NBB_USHORT out_dsptr = 0;
    NBB_BYTE unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);


    /* 指针为空 */
    if (NULL == diff_serve)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    qos_log_l2_unipri_diff_serv_data(logic_index,diff_serve);
    
    if (0 == diff_serve->l2_out_ds_mod) /* 指配PRI */
    {
        out_mod = PHB_TO_DSCP_PRI;
        out_dsptr = diff_serve->pri + PHB2PRI_ASSIGN_OFFSET;
        
    }
    else if (1 == diff_serve->l2_out_ds_mod) /*指配DSCP*/
    {
        out_mod = PHB_TO_PRI;
        out_dsptr = (diff_serve->dscp) + PHB2PRI_ASSIGN_OFFSET;
    }
    else if (4 == diff_serve->l2_out_ds_mod) /*映射到PRI*/
    {
        out_mod = PHB_TO_PRI;
        out_dsptr = diff_serve->l2_out_diff_serv_id;
    }
    else if (5 == diff_serve->l2_out_ds_mod) /*映射到DSCP*/
    {
        out_mod = PHB_TO_DSCP_PRI;
        out_dsptr = diff_serve->l2_out_diff_serv_id;
    }
    else
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for(unit = 0;unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL2UniNhiPri(unit,vpid,out_mod,out_dsptr);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_l2unipri_err(unit,vpid,out_mod,out_dsptr,
                __FUNCTION__, __LINE__, ret); 
            goto EXIT_LABEL;
        }
#endif
    }


    

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif

#if 7

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
static void qos_print_vc_diff_serv_cfg(ATG_DCI_VC_DIFF_SERV_DATA *diff)
{
    if(NULL != diff)
    {
       if(ATG_DCI_RC_OK != g_qos_ds_print)
        {
            printf("in_ds_mod=%d,phb=%d,corlour=%d,in_diff_serv_id=%ld\n",
                    "out_ds_mod=%d,exp=%d,out_diff_serv_id=%ld\n\n",
                      diff->in_ds_mod,diff->phb,diff->corlour,diff->in_diff_serv_id,
                      diff->out_ds_mod,diff->exp,diff->out_diff_serv_id);
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
void qos_print_vc_key(ATG_DCI_VC_KEY *vc_key)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
        
    if(NULL != vc_key)
    {
        printf("vc_id=%ld,vc_type=%d,peer_ip=%ld\n",
                  vc_key->vc_id,vc_key->vc_type,vc_key->peer_ip);

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
void qos_log_vc_key(ATG_DCI_VC_KEY *vc_key)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
        
    if(NULL != vc_key)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            OS_SPRINTF(uc_message,"vc_id=%ld,vc_type=%d,peer_ip=%ld\n",
                  vc_key->vc_id,vc_key->vc_type,vc_key->peer_ip);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
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
void qos_log_vc_diff_serv_data(ATG_DCI_VC_KEY *vc_key,ATG_DCI_VC_DIFF_SERV_DATA *diff)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_vc_diff_serv_cfg(diff);
    
    if(NULL != diff && NULL != vc_key)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            qos_log_vc_key(vc_key);     
            OS_SPRINTF(uc_message,"(%s)in_ds_mod=%d,phb=%d,corlour=%d,in_diff_serv_id=%ld\n",
                      "out_ds_mod=%d,exp=%d,out_diff_serv_id=%ld\n\n",
                      DS_TAG,diff->in_ds_mod,diff->phb,diff->corlour,diff->in_diff_serv_id,
                      diff->out_ds_mod,diff->exp,diff->out_diff_serv_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
    }
   
}


/*****************************************************************************
   函 数 名  : spm_ds_add_vc_node
   功能描述  : vc优先级映射的接口函数
   输入参数  : ds模板的index  ,vc key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_vc_node(
    ATG_DCI_VC_KEY *vc_key, //VC表 配置块条目键值
    ATG_DCI_VC_DIFF_SERV_DATA *diff_serve, //VC表 DS配置
    NBB_ULONG posid, //驱动返回值
    NBB_USHORT flag)    //VC主备标识
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ING_NNI_MODE_E in_mod = UNIFORM;
    EGR_PW_MODE_E out_mod = PHB_TO_EXP;
    NBB_USHORT in_dsptr = 0;
    NBB_USHORT out_dsptr = 0;
    NBB_BYTE unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == diff_serve))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    qos_log_vc_diff_serv_data(vc_key,diff_serve);

    if(0 == diff_serve->in_ds_mod)/*指配*/
    {
        in_mod = UNIFORM;
        in_dsptr = ((diff_serve->phb) * MAX_COLOR_NUM) + diff_serve->corlour + PRI2PHB_ASSIGN_OFFSET;
    }
    else if(1 == diff_serve->in_ds_mod)/*基于外层EXP*/
    {
        in_mod = UNIFORM;
        in_dsptr = diff_serve->in_diff_serv_id;
    }
    else if(2 == diff_serve->in_ds_mod)/*基于内层EXP*/
    {
        in_mod = UNIFORM;
        in_dsptr = diff_serve->in_diff_serv_id;
    }
    else if(3 == diff_serve->in_ds_mod)/*基于dscp*/
    {
        in_mod = SHORT_PIPE;
        in_dsptr = diff_serve->in_diff_serv_id;
    }
    else
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    
    for(unit = 0;unit < c3_num;unit++)
    {
        if(VC_MAIN == flag)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetVpIngPhb(unit,posid,in_mod,in_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_vpin_phb_err(unit, posid,in_mod,in_dsptr,
                    __FUNCTION__, __LINE__, ret); 
                goto EXIT_LABEL;
            }
#endif       
        }
        else if(VC_BACK == flag)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetVpIngBackupPhb(unit,posid,in_mod,in_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_vpin_backup_phb_err(unit, posid,in_mod,in_dsptr,
                    __FUNCTION__, __LINE__, ret); 
                goto EXIT_LABEL;
            }
#endif     
        }

    }




    if (0 == diff_serve->out_ds_mod) /*指配exp*/
    {
        out_mod = PHB_TO_EXP;
        out_dsptr = diff_serve->exp + PHB2PRI_ASSIGN_OFFSET;
        
    }
    else if (1 == diff_serve->out_ds_mod) /* 映射 */
    {
        out_mod = PHB_TO_EXP;
        out_dsptr = diff_serve->out_diff_serv_id;
    }
    else if (2 == diff_serve->out_ds_mod) /*拷贝ip优先级*/
    {
        out_mod = NNI_NO_ACTION;
    }
    else
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for(unit = 0;unit < c3_num;unit++)
    {
        if(VC_MAIN == flag)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetVpEgrExp(unit,posid,out_dsptr,out_mod);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_vpegr_phb_err(unit,posid,out_dsptr,out_mod,
                    __FUNCTION__, __LINE__, ret); 
                goto EXIT_LABEL;
            }
#endif       
        }
        else if(VC_BACK == flag)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetVpEgrBackupExp(unit,posid,out_dsptr,out_mod);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_vpegr_backup_phb_err(unit,posid,out_dsptr,out_mod,
                    __FUNCTION__, __LINE__, ret); 
                goto EXIT_LABEL;
            }
#endif   
        }

    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 8

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
static void qos_print_vrf_diff_serv_cfg(ATG_DCI_VRF_INSTANCE_DIFF_SERV *diff)
{
    if(NULL != diff)
    {
       if(ATG_DCI_RC_OK != g_qos_ds_print)
        {
            printf("out_ds_mod=%d,exp=%d,out_diff_serv_id=%ld\n\n",
                      diff->out_ds_mod,diff->exp,diff->out_diff_serv_id);
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
void qos_print_vrf_key(SPM_QOS_VRF_INSTANSE_KEY *vrf_key)
{        
    if(NULL != vrf_key)
    {        
        printf("vrf_id=%d,label=%d,peer_ip=%ld\n",
                  vrf_key->vrf_id,vrf_key->label,vrf_key->peer_ip);
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
void qos_log_vrf_key(SPM_QOS_VRF_INSTANSE_KEY *vrf_key)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
        
    if(NULL != vrf_key)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            
            OS_SPRINTF(uc_message,"vrf_id=%d,label=%d,peer_ip=%ld\n",
                      vrf_key->vrf_id,vrf_key->label,vrf_key->peer_ip);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
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
void qos_log_vrf_diff_serv_data(SPM_QOS_VRF_INSTANSE_KEY *vrf_key,ATG_DCI_VRF_INSTANCE_DIFF_SERV *diff)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_vrf_diff_serv_cfg(diff);
    
    if(NULL != diff && NULL != vrf_key)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            qos_log_vrf_key(vrf_key);
            OS_SPRINTF(uc_message,"(%s)out_ds_mod=%d,exp=%d,out_diff_serv_id=%ld\n\n",
                      DS_TAG,diff->out_ds_mod,diff->exp,diff->out_diff_serv_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
    }
   
}


/*****************************************************************************
   函 数 名  : spm_ds_add_vc_node
   功能描述  : vc优先级映射的接口函数
   输入参数  : ds模板的index  ,vc key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_LONG spm_ds_set_vrf_node(
    SPM_QOS_VRF_INSTANSE_KEY *vrf_key, //VRF表 配置块条目键值
    ATG_DCI_VRF_INSTANCE_DIFF_SERV *diff_serve, //VRF表 DS配置
    NBB_ULONG posid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    EGR_PW_MODE_E out_mod = PHB_TO_EXP;
    NBB_USHORT out_dsptr = 0;
    NBB_BYTE unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vrf_key) || (NULL == diff_serve))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if (0 == diff_serve->out_ds_mod) /*指配exp*/
    {
        out_mod = PHB_TO_EXP;
        out_dsptr = diff_serve->exp + PHB2PRI_ASSIGN_OFFSET;
        
    }
    else if (1 == diff_serve->out_ds_mod) /* 映射 */
    {
        out_mod = PHB_TO_EXP;
        out_dsptr = diff_serve->out_diff_serv_id;
    }
    else if (2 == diff_serve->out_ds_mod) /*拷贝ip优先级*/
    {
        out_mod = NNI_NO_ACTION;
    }
    else
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for(unit = 0;unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetTxpwExp(unit,posid,out_dsptr,out_mod);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_api_c3_set_txpw_exp_err(unit,posid,out_dsptr,out_mod,
                __FUNCTION__, __LINE__, ret); 
            goto EXIT_LABEL;
        }
#endif       
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif

#if 9

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
static void qos_print_lsp_diff_serv_cfg(ATG_DCI_LSP_TX_PROT_DIFF_SERV *diff)
{
    if(NULL != diff)
    {
       if(ATG_DCI_RC_OK != g_qos_ds_print)
        {
            printf("out_ds_mod=%d,exp=%d,out_diff_serv_id=%ld\n\n",
                      diff->out_ds_mod,diff->exp,diff->out_diff_serv_id);
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
void qos_print_lsp_key(SPM_QOS_TUNNEL_KEY *lsp_key)
{   
    if(NULL != lsp_key)
    {     
        if(0 == lsp_key->type)/*FTN*/
        {
            printf("LSP KEY:ftn.vrfid=%d,ftn.fec=%ld,ftn.mask=%d\n",
                  lsp_key->ftn.vrfid,lsp_key->ftn.fec,lsp_key->ftn.mask);
        }
        else
        {
            printf("LSP KEY:tunnelid=%ld,lspid=%ld,ingress=%ld,egress=%d\n",
                  lsp_key->tx_lsp.tunnelid,lsp_key->tx_lsp.lspid,
                  lsp_key->tx_lsp.ingress,lsp_key->tx_lsp.egress);
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
void qos_log_lsp_key(SPM_QOS_TUNNEL_KEY *lsp_key)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
        
    if(NULL != lsp_key)
    {     
        if(0 == lsp_key->type)/*FTN*/
        {
            OS_SPRINTF(uc_message,"ftn.vrfid=%d,ftn.fec=%ld,ftn.mask=%d\n",
                  lsp_key->ftn.vrfid,lsp_key->ftn.fec,lsp_key->ftn.mask);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
        else
        {
            OS_SPRINTF(uc_message,"tunnelid=%ld,lspid=%ld,ingress=%ld,egress=%d\n",
                  lsp_key->tx_lsp.tunnelid,lsp_key->tx_lsp.lspid,
                  lsp_key->tx_lsp.ingress,lsp_key->tx_lsp.egress);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
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
void qos_log_lsp_diff_serv_data(SPM_QOS_TUNNEL_KEY *lsp_key,ATG_DCI_LSP_TX_PROT_DIFF_SERV *diff)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_lsp_diff_serv_cfg(diff);
    
    if(NULL != diff && NULL != lsp_key)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            qos_log_lsp_key(lsp_key);
            OS_SPRINTF(uc_message,"(%s)out_ds_mod=%d,exp=%d,out_diff_serv_id=%ld\n\n",
                  DS_TAG,diff->out_ds_mod,diff->exp,diff->out_diff_serv_id);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
   
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
NBB_LONG spm_ds_add_tx_lsp_node (SPM_QOS_TUNNEL_KEY *lspkey,
        ATG_DCI_LSP_TX_PROT_DIFF_SERV *diff_serve,
        NBB_ULONG posid,NBB_BYTE type)//type:(0:inlsp;1:outlsp)  
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    EGR_LSP_MODE_E out_mod = MAP_EXP;
    NBB_USHORT out_dsptr = 0;
    NBB_BYTE unit = 0;
    NBB_BYTE exp = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == lspkey) || (NULL == diff_serve))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    if (0 == diff_serve->out_ds_mod) /*指配exp*/
    {
        exp = diff_serve->exp;
        out_mod = LSP_NO_ACTION;
        
        //out_dsptr = diff_serve->exp+ PHB2PRI_ASSIGN_OFFSET;
        
    }
    else if (1 == diff_serve->out_ds_mod) /* 映射 */
    {
        out_mod = MAP_EXP;
        out_dsptr = diff_serve->out_diff_serv_id;
    }
    else if (2 == diff_serve->out_ds_mod) /*拷贝内层PW的EXP优先级*/
    {
        //out_mod = LSP_NO_ACTION;
    }
    else
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    for(unit = 0;unit < c3_num;unit++)
    {
        if(0 == type)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetTxLspInExp(unit,posid,out_mod,exp,out_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_txlsp_exp_err(unit,posid,out_mod,exp,out_dsptr,
                    __FUNCTION__, __LINE__, ret); 
                goto EXIT_LABEL;
            }
#endif      
        }
        else if(1 == type)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetTxLspOutExp(unit,posid,out_mod,exp);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_api_c3_set_txlsp_exp_err(unit,posid,out_mod,exp,out_dsptr,
                    __FUNCTION__, __LINE__, ret); 
                goto EXIT_LABEL;
            }
#endif       
        }
      
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


#endif


#if 10

/*****************************************************************************
   函 数 名  : spm_ds_add_ilm_node
   功能描述  : ILM优先级映射的接口函数(它可能是内层标签)
   输入参数  : ds模板的index  ,ilm key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_ds_set_out_ilm(NBB_ULONG posid, NBB_ULONG eposid,NBB_BYTE type,
                EGR_LSP_MODE_E out_mod,NBB_USHORT out_dsptr,NBB_BYTE exp) 
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*出方向*/
    if(1 == type)/*RxLspEgrInExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrInExp(unit, posid, out_mod,exp, out_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(2 == type)/*RxLspEgrBackupInExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrBackupInExp(unit, posid, out_mod,exp, out_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(3 == type)/*RxLspEgrOutExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrOutExp(unit, posid, out_mod,exp/*, out_dsptr*/);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(4 == type)/*RxLspEgrBackupOutExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrBackupOutExp(unit, posid, out_mod,exp/*, out_dsptr*/);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(5 == type)/*RxLspEcmpInExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrEcmpInExp(unit, posid,eposid, out_mod,exp, out_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else if(6 == type)/*RxLspEcmpOutExp*/
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspEgrEcmpOutExp(unit, posid,eposid, out_mod,exp/*, out_dsptr*/);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else
    {
        //spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }


    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_ds_add_ilm_node
   功能描述  : ILM优先级映射的接口函数(它可能是内层标签)
   输入参数  : ds模板的index  ,ilm key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_ds_set_in_ilm(NBB_ULONG posid,NBB_BYTE type,
                                    ING_NNI_MODE_E in_mod,NBB_USHORT in_dsptr) 
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*入方向*/
    if(0 == type)
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxpwPhb(unit, posid, in_mod, in_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }
    }
    else
    {
        for(unit = 0;unit < v_spm_shared->c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetRxLspIngPhb(unit, posid, in_mod, in_dsptr);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
#endif 
        }   
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_ds_add_ilm_node
   功能描述  : ILM优先级映射的接口函数(它可能是内层标签)
   输入参数  : ds模板的index  ,ilm key,驱动结构体指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
static NBB_LONG spm_ds_set_ilm(NBB_ULONG posid, NBB_ULONG eposid,NBB_BYTE type,
                ING_NNI_MODE_E in_mod,NBB_USHORT in_dsptr,EGR_LSP_MODE_E out_mod,
                NBB_USHORT out_dsptr,NBB_BYTE exp) 
{
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE unit = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    ret = spm_ds_set_in_ilm(posid, type,in_mod, in_dsptr);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    
    ret = spm_ds_set_out_ilm(posid,eposid,type,out_mod,out_dsptr, exp);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
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
static void qos_print_ilm_diff_serv_data(NBB_ULONG label,ATG_DCI_ILM_DIFF_SERV_DATA *diff,
                            NBB_ULONG posid,NBB_ULONG eposid,NBB_BYTE type)
{
    if(NULL != diff)
    {
       if(ATG_DCI_RC_OK != g_qos_ds_print)
        {
            printf("label =%ld,nxhp_index=%ld,nextip=%ld\n"
                      "in_ds_mod=%d,phb=%d,corlour=%d,in_diff_serv_id=%ld\n,"
                      "out_ds_mod=%d,exp=%d,out_diff_serv_id=%d\n"
                      "posid=%ld,eposid=%ld,type=%d\n\n",
                      label,diff->nxhp_index,diff->nxhp_ip,diff->in_ds_mod,
                      diff->phb,diff->corlour,diff->in_diff_serv_id,
                      diff->out_ds_mod,diff->exp,diff->out_diff_serv_id,
                      posid,eposid,type);
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
static void qos_log_ilm_diff_serv_data(NBB_ULONG label,ATG_DCI_ILM_DIFF_SERV_DATA *diff,NBB_ULONG posid,
                                        NBB_ULONG eposid,NBB_BYTE type)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_ilm_diff_serv_data(label,diff, posid,eposid, type);
    
    if(NULL != diff)
    {
        if(ATG_DCI_RC_OK != g_qos_ds_log)
        {
            
            OS_SPRINTF(uc_message,"(%s)label =%ld,nextport=%ld,nextip=%ld\n"
                      "in_ds_mod=%d,phb=%d,corlour=%d,in_diff_serv_id=%ld\n,"
                      "out_ds_mod=%d,exp=%d,out_diff_serv_id=%d\n"
                      "posid=%ld,eposid=%ld,type=%d\n\n",
                      DS_TAG,
                      label,diff->nxhp_index,diff->nxhp_ip,diff->in_ds_mod,
                      diff->phb,diff->corlour,diff->in_diff_serv_id,
                      diff->out_ds_mod,diff->exp,diff->out_diff_serv_id,
                      posid,eposid,type);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
    }
   
}

/*****************************************************************************
   函 数 名  : spm_ds_add_ilm_node
   功能描述  : ILM优先级映射的接口函数(它可能是内层标签)
   输入参数  : eposid:ECMP ID
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/

//type:
//0:RXPW;1: RxLspEgrInExp;2:RxLspEgrBackupInExp;
//3:RxLspEgrOutExp;4:RxLspEgrBackupOutExp
//5:RxLspEcmpInExp;6:RxLspEcmpOutExp
NBB_LONG spm_add_ilm_ds_node(NBB_ULONG label,ATG_DCI_ILM_DIFF_SERV_DATA *diff,NBB_ULONG posid,
                                NBB_ULONG eposid,NBB_BYTE type) 
{

    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ING_NNI_MODE_E in_mod = UNIFORM;
    EGR_LSP_MODE_E out_mod = LSP_NO_ACTION;
    NBB_BYTE unit = 0;
    NBB_USHORT in_dsptr = 0;
    NBB_USHORT out_dsptr = 0;
    NBB_BYTE exp = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == diff)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    qos_log_ilm_diff_serv_data(label,diff, posid,eposid,type);

    
    if(0 == diff->in_ds_mod)/*指配*/
    {
        in_mod = UNIFORM;
        in_dsptr = (diff->phb) * MAX_COLOR_NUM + diff->corlour + PRI2PHB_ASSIGN_OFFSET;
    }
    else if(1 == diff->in_ds_mod)/*基于外层映射*/
    {
        in_mod = UNIFORM;
        in_dsptr =  diff->in_diff_serv_id;
    }
    else if(2 == diff->in_ds_mod)/*短管道*/
    {
        in_mod = SHORT_PIPE;
        in_dsptr =   diff->in_diff_serv_id;
    }
    else
    {
        //spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    ret = spm_ds_set_in_ilm(posid, type,in_mod, in_dsptr);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    
    if(0 == diff->out_ds_mod)/*指配EXP*/
    {
        out_mod = MAP_EXP;
        exp = diff->exp;
        out_dsptr = diff->out_diff_serv_id;
    }
    else if(1 == diff->out_ds_mod)/*映射*/
    {
        out_mod = MAP_EXP;
        out_dsptr = diff->exp + PHB2PRI_ASSIGN_OFFSET;
    }
    else if(2 == diff->out_ds_mod)/*拷贝ip优先级*/
    {
        out_mod = LSP_NO_ACTION;
        out_dsptr = diff->out_diff_serv_id;
    }
    else
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }


    ret = spm_ds_set_out_ilm(posid,eposid,type,out_mod,out_dsptr, exp);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    
   
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


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
NBB_VOID qos_ds_help()
{
    NBB_CHAR **ppc_msg;

    static NBB_CHAR *s_help_msg[] = {       
        "---------------------------DS-----------------------------------", "",
        "qos_set_ds_print(set)",                   "set all ds cfg print",
        "qos_set_ds_log(set)",                     "set all ds log print",
        "qos_show_phb2pri_ds_offset(dsptr,phb,color)",  "show ds offset",
        "qos_show_pri2phb_ds_offset(dsptr,pri)",        "show ds offset",   
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

