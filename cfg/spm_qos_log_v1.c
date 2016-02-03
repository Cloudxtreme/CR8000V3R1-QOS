/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_log.c
   版 本 号   : 初稿
   作    者   : zenglu
   生成日期   : 2012年9月18日
   最近修改   :
   功能描述   : QOS 模块外公共报错log文件
   函数列表   :
   修改历史   :
   1.日    期   : 2012年9月18日
    作    者   : zenglu
    修改内容   : 创建文件
******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>


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
NBB_VOID spm_qos_parameter_error_log(const NBB_CHAR *function,NBB_ULONG line)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s %s,line=%ld param_error\n",
                QOS_CFG_STRING,function,line);
        OS_SPRINTF(message,"%s %s,line=%ld param_error\n",
                QOS_CFG_STRING,function,line);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			QOS_CFG_STRING,ATG_DCI_RC_UNSUCCESSFUL,
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
NBB_VOID spm_qos_exception_log(const NBB_CHAR *function,NBB_ULONG line)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s %s,line=%ld exception\n",
                QOS_CFG_STRING,function,line);
        OS_SPRINTF(message,"%s %s,line=%ld exception\n",
                QOS_CFG_STRING,function,line);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			QOS_CFG_STRING,ATG_DCI_RC_UNSUCCESSFUL,
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
NBB_VOID spm_qos_get_port_from_logical_port_index_err_log( NBB_ULONG index,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line = %ld  spm_get_portid_from_logical_port_index  err : "
                  "ret = %ld, logic_key = %ld\n\n", function, line, ret, index);
        OS_SPRINTF(message,"%s line = %ld  spm_get_portid_from_logical_port_index  err : "
                   "ret = %ld, logic_key = %ld\n\n", function, line, ret, index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        	           QOS_ERROR_STRING,ret,
        	           message,function,"line","",
        	           0,0,line,0)); 
    }  
}



