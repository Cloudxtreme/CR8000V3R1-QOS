/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_log.c
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

static char* qos_cfg_oper_type_string[] = {"ADD", "UPD", "DEL", "", "", NULL};




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
NBB_VOID spm_qos_param_error_log(const NBB_CHAR *FUNCTION,NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        printf("%s %s,line=%ld param_error\n",
                QOS_CFG_STRING,FUNCTION,LINE);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld param_error\n",
                QOS_CFG_STRING,FUNCTION,LINE);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			QOS_CFG_STRING,ATG_DCI_RC_UNSUCCESSFUL,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));      
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
NBB_VOID spm_qos_txlsp_key_pd(SPM_QOS_TUNNEL_KEY *pkey,NBB_CHAR *string,
    NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    if((NULL != pkey) && (NULL != string))
    {
       NBB_EXCEPTION((PCT_SPM | QOS_PD, 1,  "s d s s s s d d d d", 
    			string,ret,
    			"vrfid","fec","mask","type",
    			pkey->ftn.vrfid,pkey->ftn.fec,
    			pkey->ftn.mask,pkey->type));
       
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
NBB_VOID spm_qos_vrf_key_pd(SPM_QOS_VRF_INSTANSE_KEY *pkey,NBB_CHAR *string,
    NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    if((NULL != pkey) && (NULL != string))
    {
       NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			string,ret,
    			"vrf_id","peer_ip","label","",
    			pkey->vrf_id,pkey->peer_ip,pkey->label,0)); 
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
NBB_VOID spm_qos_subport_pd(SUB_PORT *sub_port,NBB_ULONG logic_key,
    NBB_CHAR *string,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    if((NULL != sub_port) && (NULL != string))
    {
       NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			string,ret,
    			"logic_key","port","ovlan","posid",
    			logic_key,sub_port->port,sub_port->ovlan,sub_port->posid)); 
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
NBB_VOID spm_qos_log_key_pd(NBB_ULONG logic_key,
    NBB_CHAR *string,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    if(NULL != string)
    {
       NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			string,ret,
    			"logic_key","","","",
    			logic_key,0,0,0)); 
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
NBB_VOID spm_dbg_record_qos_policy_c2b_head(ATG_DCI_QOS_POLICY_CB2QOS_DATA *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    if(NULL != pkey)
    {
        OS_PRINTF("################### (classify_id=%ld behavior_id =%ld, %s)"
                  " ###################\n",pkey->qos_classify_index,
                  pkey->qos_behavior_index,*(qos_cfg_oper_type_string + ucOper));
        OS_SPRINTF(ucMessage,"################### (classify_id=%ld behavior_id =%ld, %s)"
                   " ###################\n",pkey->qos_classify_index,
                   pkey->qos_behavior_index,*(qos_cfg_oper_type_string + ucOper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
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
NBB_VOID spm_dbg_record_qos_policy_head(NBB_ULONG *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    if(NULL != pkey)
    {
        OS_PRINTF("################### policy (id =%ld, %s) ###################\n",
                  *pkey,*(qos_cfg_oper_type_string + ucOper));
        OS_SPRINTF(ucMessage,"################### policy (id =%ld, %s) ###################\n",
                   *pkey,*(qos_cfg_oper_type_string + ucOper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
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
NBB_VOID spm_dbg_record_qos_classify_head(ATG_DCI_QOS_CLASSIFY_KEY *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    if(NULL != pkey)
    {
        OS_PRINTF("################### classify (classify_id=%ld,if_match_id=%ld,"
                  " %s) ###################\n",pkey->qos_classify_index,pkey->if_match_id,
                  *(qos_cfg_oper_type_string + ucOper));
        OS_SPRINTF(ucMessage,"################### classify (classify_id=%ld,if_match_id=%ld,"
                   " %s) ###################\n",pkey->qos_classify_index,pkey->if_match_id,
                   *(qos_cfg_oper_type_string + ucOper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
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
NBB_VOID spm_dbg_record_qos_lsp_head(SPM_QOS_TUNNEL_KEY *pkey,
    NBB_BYTE ucOper NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    OS_PRINTF("################### LSP (vrfid=%d,fec=%ld,"
        "mask=%d,type=%d %s) ###################\n",
                pkey->ftn.vrfid,pkey->ftn.fec,
    			pkey->ftn.mask,pkey->type,*(qos_cfg_oper_type_string + ucOper));
    OS_SPRINTF(ucMessage,"################### LSP (vrfid=%d,fec=%ld,"
        "mask=%d,type=%d %s) ###################\n",
                pkey->ftn.vrfid,pkey->ftn.fec,
    			pkey->ftn.mask,pkey->type,*(qos_cfg_oper_type_string + ucOper));
    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
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
NBB_VOID spm_dbg_record_qos_vrf_head(SPM_QOS_VRF_INSTANSE_KEY *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    if(NULL != pkey)
    {
        OS_PRINTF("################### VRF (vrf_id=%d,peer_ip=0x%lx,"
            "label=%ld, %s) ###################\n",
                    pkey->vrf_id,pkey->peer_ip,pkey->label,
                    *(qos_cfg_oper_type_string + ucOper));
        OS_SPRINTF(ucMessage,"################### VRF (vrf_id=%d,peer_ip=0x%lx,"
            "label=%ld, %s) ###################\n",
                    pkey->vrf_id,pkey->peer_ip,pkey->label,
                    *(qos_cfg_oper_type_string + ucOper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
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
NBB_VOID spm_dbg_record_qos_vc_head(ATG_DCI_VC_KEY *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    if(NULL != pkey)
    {
        OS_PRINTF("################### VC (vc_id=%ld,peer_ip=0x%lx,"
            "vc_type=%d, %s) ###################\n",
                    pkey->vc_id,pkey->peer_ip,pkey->vc_type,
                    *(qos_cfg_oper_type_string + ucOper));
        OS_SPRINTF(ucMessage,"################### VC (vc_id=%ld,peer_ip=0x%lx,"
            "vc_type=%d, %s) ###################\n",
                    pkey->vc_id,pkey->peer_ip,pkey->vc_type,
                    *(qos_cfg_oper_type_string + ucOper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
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
NBB_VOID spm_dbg_record_qos_logic_subport_head(SUB_PORT *sub_port,NBB_ULONG logic_key,
    NBB_BYTE ucOper NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    if(NULL != sub_port)
    {      
        OS_PRINTF("################### LOGIC KEY (logic_key=%ld,port=%d,"
            "ovlan=%d,posid=%ld %s) ###################\n",
                    logic_key,sub_port->port,sub_port->ovlan,sub_port->posid,
                    *(qos_cfg_oper_type_string + ucOper));
        OS_SPRINTF(ucMessage,"################### LOGIC KEY (logic_key=%ld,port=%d,"
            "ovlan=%d,posid=%ld %s) ###################\n",
                    logic_key,sub_port->port,sub_port->ovlan,sub_port->posid,
                    *(qos_cfg_oper_type_string + ucOper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
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
NBB_VOID spm_dbg_record_qos_logic_head(NBB_ULONG logic_key,
    NBB_BYTE ucOper NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    
    OS_PRINTF("################### LOGIC KEY (logic_key=%ld, %s) ###################\n",
                logic_key,*(qos_cfg_oper_type_string + ucOper));
    OS_SPRINTF(ucMessage,"################### LOGIC KEY (logic_key=%ld, %s) ###################\n",
                logic_key,*(qos_cfg_oper_type_string + ucOper));
    BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);     
}


#if 1/*HQOS LSP*/

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
NBB_VOID spm_check_hqos_lsp_node_cfg_log(SPM_QOS_TUNNEL_KEY *pkey,
    NBB_ULONG old_node, NBB_ULONG node, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != pkey) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s %s,line=%ld LSP node index cfg miss match old node=%ld,new node=%ld\n",
                HQOS_LSP_ERROR,FUNCTION,LINE,old_node,node);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld LSP node index cfg miss match old node=%ld,new node=%ld\n\n",
                HQOS_LSP_ERROR,FUNCTION,LINE,old_node,node);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);

        spm_qos_txlsp_key_pd(pkey,HQOS_LSP_CFG,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_LSP_ERROR,ATG_DCI_RC_UNSUCCESSFUL,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));      
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
NBB_VOID spm_check_hqos_lsp_mode_cfg_log(SPM_QOS_TUNNEL_KEY *pkey,
    NBB_ULONG old_mode, NBB_ULONG mode, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != pkey) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s %s,line=%ld LSP old mode=%ld,mode=%ld miss match\n",
                HQOS_LSP_ERROR,FUNCTION,LINE,old_mode,mode);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld LSP old mode=%ld,mode=%ld miss match\n\n",
                HQOS_LSP_ERROR,FUNCTION,LINE,old_mode,mode);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        
        spm_qos_txlsp_key_pd(pkey,HQOS_LSP_CFG,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_LSP_ERROR,ATG_DCI_RC_UNSUCCESSFUL,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));   
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
NBB_VOID spm_check_hqos_lsp_del_cfg_log(SPM_QOS_TUNNEL_KEY *pkey, 
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != pkey) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s %s,line=%ld pw tree is in used can't del\n",
                HQOS_LSP_ERROR,FUNCTION,LINE);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld pw tree is in used can't del\n\n",
                HQOS_LSP_ERROR,FUNCTION,LINE);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        
        spm_qos_txlsp_key_pd(pkey,HQOS_LSP_CFG,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_LSP_ERROR,ATG_DCI_RC_UNSUCCESSFUL,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));   
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
NBB_VOID spm_check_hqos_lsp_cfg_same_log(SPM_QOS_TUNNEL_KEY *pkey, 
    NBB_ULONG policy_index, const NBB_CHAR *FUNCTION, NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != pkey) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s %s,line=%ld HQOS LSP CFG the same policy_index=%ld\n",
                HQOS_LSP_ERROR,FUNCTION,LINE,policy_index);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld  HQOS LSP CFG the same policy_index=%ld\n\n",
                HQOS_LSP_ERROR,FUNCTION,LINE,policy_index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        
        spm_qos_txlsp_key_pd(pkey,HQOS_LSP_CFG,ATG_DCI_RC_OK NBB_CCXT);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_LSP_ERROR,ATG_DCI_RC_OK,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));   
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
NBB_VOID spm_check_hqos_lsp_lspid_err_log(NBB_ULONG lspid, 
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE  NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s %s,line=%ld,lspid = %ld, HQOS Correct Configuration Sequence : \n"
                  "Configure the HQOS of LSP first, and then Configure the HQOS of VPN.",
                  HQOS_LSP_WARNING,FUNCTION,LINE,lspid);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld,lspid = %ld, HQOS Correct Configuration Sequence : \n"
                  "Configure the HQOS of LSP first, and then Configure the HQOS of VPN.",
                  HQOS_LSP_WARNING,FUNCTION,LINE,lspid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       HQOS_LSP_WARNING,ATG_DCI_RC_OK,
    			       ucMessage,FUNCTION,"LINE","",0,0,LINE,0));   
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
NBB_VOID spm_check_hqos_lsp_cfg_err_log(SPM_QOS_TUNNEL_KEY *pkey, 
    NBB_LONG flag, const NBB_CHAR *FUNCTION, NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != pkey) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s %s,line=%ld HQOS LSP CFG ERROR flag=%s\n",
                HQOS_LSP_ERROR,FUNCTION,LINE,(0 == flag)?"存在":"不存在");
        OS_SPRINTF(ucMessage,"%s %s,line=%ld HQOS LSP CFG ERROR flag=%s\n\n",
                HQOS_LSP_ERROR,FUNCTION,LINE,(0 == flag)?"存在":"不存在");
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        
        spm_qos_txlsp_key_pd(pkey,HQOS_LSP_CFG,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_LSP_ERROR,ATG_DCI_RC_UNSUCCESSFUL,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));   
    }
}

#endif


#if 2/*HQOS VRF*/

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
NBB_VOID spm_check_hqos_vrf_cfg_log(SPM_QOS_VRF_INSTANSE_KEY *pkey,
    NBB_CHAR *FUNCTION,NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];
    
    if((NULL != pkey) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s %s,line=%ld LSP node can't finde\n",
                HQOS_VRF_ERROR,FUNCTION,LINE);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld LSP node can't finde\n\n",
                HQOS_VRF_ERROR,FUNCTION,LINE);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);

        spm_qos_vrf_key_pd(pkey,HQOS_VRF_CFG,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_VRF_ERROR,ATG_DCI_RC_UNSUCCESSFUL,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));      
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
NBB_VOID spm_check_hqos_vrf_node_index_cfg_log(SPM_QOS_VRF_INSTANSE_KEY *pkey, 
    NBB_ULONG old_node_index, NBB_ULONG node_index, const NBB_CHAR *FUNCTION, NBB_ULONG LINE
    NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != pkey) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s %s,line=%ld LSP old_node_index=%ld,node_index=%ld miss match\n",
                HQOS_VRF_ERROR,FUNCTION,LINE,old_node_index,node_index);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld LSP old_node_index=%ld,node_index=%ld miss match\n\n",
                HQOS_VRF_ERROR,FUNCTION,LINE,old_node_index,node_index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        
        spm_qos_vrf_key_pd(pkey,HQOS_VRF_ERROR,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_VRF_ERROR,ATG_DCI_RC_UNSUCCESSFUL,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));  
    }  
}

#endif


#if 3

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
NBB_VOID spm_check_hqos_vc_node_index_cfg_log(ATG_DCI_VC_KEY *pkey,
    NBB_ULONG old_node_index, NBB_ULONG node_index, const NBB_CHAR *FUNCTION,
    NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != pkey) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s %s,line=%ld LSP old_node_index=%ld,node_index=%ld miss match\n",
                HQOS_VC_ERROR,FUNCTION,LINE,old_node_index,node_index);
        OS_SPRINTF(ucMessage,"%s %s,line=%ld LSP old_node_index=%ld,node_index=%ld miss match\n\n",
                HQOS_VC_ERROR,FUNCTION,LINE,old_node_index,node_index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        
        spm_qos_vc_key_pd(pkey,HQOS_VC_ERROR,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_VC_ERROR,ATG_DCI_RC_UNSUCCESSFUL,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));  
    }
}

#endif


#if 4




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
NBB_VOID spm_l3_lookupuni_error_log(NBB_ULONG AclId, NBB_ULONG RuleId, NBB_ULONG ActionId,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_ULONG nxhop_index, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld  spm_l3_lookupuni  err : ret = %ld, AclId = %ld, "
                  "RuleId = %ld, BehaviorId = %ld, NxHop_PortId = %ld\n\n", FUNCTION, LINE, ret,
                  AclId, RuleId, ActionId, nxhop_index);
        OS_SPRINTF(ucMessage,"%s line = %ld  spm_l3_lookupuni  err : ret = %ld, AclId = %ld, "
                   "RuleId = %ld, BehaviorId = %ld, NxHop_PortId = %ld\n\n", FUNCTION, LINE, ret,
                   AclId, RuleId, ActionId, nxhop_index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        	           QOS_ERROR_STRING,ret,
        	           ucMessage,FUNCTION,"LINE","",
        	           0,0,LINE,0)); 
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
NBB_VOID spm_get_portid_from_logical_port_index_error_log( NBB_ULONG index,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld  spm_get_portid_from_logical_port_index  err : "
                  "ret = %ld, logic_key = %ld\n\n", FUNCTION, LINE, ret, index);
        OS_SPRINTF(ucMessage,"%s line = %ld  spm_get_portid_from_logical_port_index  err : "
                   "ret = %ld, logic_key = %ld\n\n", FUNCTION, LINE, ret, index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        	           QOS_ERROR_STRING,ret,
        	           ucMessage,FUNCTION,"LINE","",
        	           0,0,LINE,0)); 
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
NBB_VOID spm_get_vlan_from_logical_port_index_error_log( NBB_ULONG index,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld  spm_get_vlan_from_logical_port_index  err : "
                  "ret = %ld, logic_key = %ld\n\n",FUNCTION,LINE,ret,index);
        OS_SPRINTF(ucMessage,"%s line = %ld  spm_get_vlan_from_logical_port_index  err : "
                   "ret = %ld, logic_key = %ld\n\n",FUNCTION,LINE,ret,index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0)); 
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
NBB_VOID spm_qos_apply_meter_error_log( NBB_ULONG *pid,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pid))
    {
        OS_PRINTF("%s line = %ld spm_qos_apply_meter_id err: ret = %ld, "
                  "meter_id = %ld\n\n",FUNCTION,LINE,ret,*pid);
        OS_SPRINTF(ucMessage,"%s line = %ld spm_qos_apply_meter_id err: ret = %ld, "
                   "meter_id = %ld\n\n",FUNCTION,LINE,ret,*pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0)); 
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
NBB_VOID spm_qos_free_meter_error_log( NBB_ULONG *pid,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pid))
    {
        OS_PRINTF("%s line = %ld  spm_qos_free_meter_id err: free meterId failed. "
                  "ret = %ld, meter_id = %ld\n\n",FUNCTION,LINE,ret,*pid);
        OS_SPRINTF(ucMessage,"%s line = %ld  spm_qos_free_meter_id err: free meterId failed. "
                   "ret = %ld, meter_id = %ld\n\n",FUNCTION,LINE,ret,*pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0)); 
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
NBB_VOID spm_api_c3_set_meter_error_log(NBB_ULONG unit, METER_CFG_T *meter,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != meter))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetMeter err : ret = %ld (unit = %ld, MeterId = %ld, "
                  "MeterType = %d, cir = %ld, pir = %ld, cbs = %ld, pbs = %ld, dropred = %d)\n\n",
                  FUNCTION,LINE,ret,unit,meter->meterId,meter->eMeterType,meter->cir,meter->pireir,
                  meter->cbs,meter->pbsebs,meter->dropRed);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3SetMeter err : ret = %ld (unit = %ld, MeterId = %ld, "
                   "MeterType = %d, cir = %ld, pir = %ld, cbs = %ld, pbs = %ld, dropred = %d)\n\n",
                   FUNCTION,LINE,ret,unit,meter->meterId,meter->eMeterType,meter->cir,meter->pireir,
                   meter->cbs,meter->pbsebs,meter->dropRed);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
NBB_VOID spm_api_c3_del_meter_error_log(NBB_ULONG unit, NBB_ULONG meterId,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld  ApiC3DelMeter err : ret = %ld (unit = %ld, "
                  "MeterId = %ld)\n\n", FUNCTION, LINE, ret, unit, meterId);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3DelMeter err : ret = %ld (unit = %ld, "
                   "MeterId = %ld)\n\n", FUNCTION, LINE, ret, unit, meterId);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
NBB_VOID spm_api_c3_set_acl_action_log(ACL_T *acl,SPM_QOS_ACL_CB *pcb,
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pcb) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetAcl Print: AclId = %d, RuleId = %d, "
                  "acl.eAclAction = %d, basic_cfg_cb->action = %d, match_type = %d)\n\n",FUNCTION,
                  LINE,acl->mAclId,acl->mRuleId,acl->eAclAction,pcb->basic_cfg_cb->action,
                  pcb->basic_cfg_cb->match_type);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3SetAcl Print: AclId = %d, RuleId = %d, "
                   "acl.eAclAction = %d, basic_cfg_cb->action = %d, match_type = %d)\n\n",FUNCTION,
                   LINE,acl->mAclId,acl->mRuleId,acl->eAclAction,pcb->basic_cfg_cb->action,
                   pcb->basic_cfg_cb->match_type);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
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
NBB_VOID spm_api_c3_set_acl_error_log(NBB_ULONG unit, ACL_T *acl,
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                  "meter_id = %ld)\n\n",FUNCTION,LINE,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3SetAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                   "meter_id = %ld)\n\n",FUNCTION,LINE,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
    }  
}

/*****************************************************************************
   函 数 名  : spm_api_c3_set_slot_attact_error_log
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2015年8月17日 
   作    者  : tianf
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_api_c3_set_slot_attact_error_log(NBB_ULONG unit, 
                                    NBB_ULONG pRecordSwitch,
                                    NBB_ULONG pSampleRatio,
                                    const NBB_CHAR *FUNCTION,
                                    NBB_ULONG LINE,
                                    NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld fhdrv_psn_set_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d, "
                  "SampleRatio = %d, unit = %ld)\n\n",FUNCTION,LINE,ret,pRecordSwitch,pSampleRatio,unit);
        OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_psn_set_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d"
                  ", SampleRatio = %d, unit = %ld)\n\n",FUNCTION,LINE,ret,pRecordSwitch,pSampleRatio,unit);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
    }  
}

/*****************************************************************************
   函 数 名  : spm_api_c3_set_slot_attact_error_log
   功能描述  : usr group引用计数减1
   输入参数  : usr group的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2015年8月17日 
   作    者  : tianf
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_api_c3_get_slot_attact_error_log(NBB_ULONG unit, 
                                    NBB_ULONG *pRecordSwitch,
                                    NBB_ULONG *pSampleRatio,
                                    const NBB_CHAR *FUNCTION,
                                    NBB_ULONG LINE,
                                    NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pRecordSwitch) && (NULL != pSampleRatio))
    {
        OS_PRINTF("%s line = %ld fhdrv_psn_get_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d, "
                  "SampleRatio = %d, unit = %ld)\n\n",FUNCTION,LINE,ret,*pRecordSwitch,*pSampleRatio,unit);
        OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_psn_get_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d"
                  ", SampleRatio = %d, unit = %ld)\n\n",FUNCTION,LINE,ret,*pRecordSwitch,*pSampleRatio,unit);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
NBB_VOID spm_api_c3_del_acl_error_log(NBB_ULONG unit, ACL_T *acl,
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3DelAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                  "meter_id = %ld)\n\n",FUNCTION,LINE,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3DelAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                   "meter_id = %ld)\n\n",FUNCTION,LINE,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
    }  
}

#ifdef PTN690


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
NBB_VOID spm_set_policer_bucket_template_error_log(NBB_ULONG unit,
    METER_CFG_T *meter, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, 
    NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != meter) && (NULL != FUNCTION))
    {
        OS_PRINTF("%s line = %ld  fhdrv_qos_set_policer_bucket_template err : ret = %ld (unit = %ld, MeterId = %ld, "
         "MeterType = %d, cir = %ld, pir = %ld, cbs = %ld, pbs = %ld, dropred = %d,colorBlind=%d)\n\n",
                  FUNCTION,LINE,ret,unit,meter->meterId,meter->eMeterType,meter->cir,meter->pireir,
                  meter->cbs,meter->pbsebs,meter->dropRed,meter->colorBlind);
                  OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_qos_set_policer_bucket_template err "
                  ": ret = %ld (unit = %ld, MeterId = %ld, MeterType = %d, cir = %ld, pir = %ld, "
                  "cbs = %ld, pbs = %ld, dropred = %d,colorBlind=%d)\n\n",FUNCTION,LINE,ret,unit,meter->meterId,
                  meter->eMeterType,meter->cir,meter->pireir,meter->cbs,meter->pbsebs,
                  meter->dropRed,meter->colorBlind);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
NBB_VOID spm_clear_policer_bucket_template_error_log(NBB_ULONG unit,
    NBB_ULONG meter_id, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, 
    NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld  fhdrv_qos_clear_policer_bucket_template err : ret = %ld "
            "(unit = %ld, MeterId = %ld)\n\n",
                  FUNCTION,LINE,ret,unit,meter_id);
        OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_qos_clear_policer_bucket_template err : ret = %ld "
            "(unit = %ld, MeterId = %ld)\n\n",
                  FUNCTION,LINE,ret,unit,meter_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
NBB_VOID spm_intf_set_policer_bucket_offset_error_log(NBB_ULONG unit,
    NBB_ULONG posid,NBB_ULONG intf_offset,NBB_ULONG acl_id,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        printf("%s line = %ld  fhdrv_intf_set_policer_bucket_offset err : ret = %ld "
               "(unit = %ld,posid = %ld,intf_offset=%ld,acl_id=%ld\n\n",
               FUNCTION,LINE,ret,unit,posid,intf_offset,acl_id);
        OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_intf_set_policer_bucket_offset err : "
                   "ret = %ld (unit = %ld,posid = %ld,intf_offset=%ld,acl_id=%ld\n\n",
                   FUNCTION,LINE,ret,unit,posid,intf_offset,acl_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
NBB_VOID spm_acl_update_rule_error_log(NBB_ULONG unit,
    NBB_ULONG posid,NBB_ULONG acl_id,NBB_ULONG rule_id, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        printf("%s line = %ld  fhdrv_acl_update_rule err : ret = %ld (unit = %ld,posid = %ld,"
         "acl_id=%ld,rule_id=%ld\n\n",
                  FUNCTION,LINE,ret,unit,posid,acl_id,rule_id);
        OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_acl_update_rule err : ret = %ld (unit = %ld,posid = %ld,"
         "acl_id=%ld,rule_id=%ld\n\n",
                  FUNCTION,LINE,ret,unit,posid,acl_id,rule_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
    }  
}

#endif

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
NBB_VOID spm_set_policer_action_template_error_log( NBB_ULONG unit, 
    POLICER_ACTION_TEMPLATE_T *cfg, const NBB_CHAR *FUNCTION,
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != cfg))
    {
        OS_PRINTF("%s line = %ld  fhdrv_qos_set_policer_action_template err : ret = %ld "
                  "(unit = %ld,chg_gre_pri_ena = %d,gre_pri = %d, gre_drop_ena = %d, "
                  "chg_yel_pri_ena = %d, yel_pri = %d, yel_drop_ena = %d,chg_red_pri_ena = %d,"
                  "red_pri=%d,red_drop_ena=%d,id=%ld)\n\n",
                  FUNCTION,LINE,ret,unit,cfg->chg_gre_pri_ena,cfg->gre_pri,cfg->gre_drop_ena,
                  cfg->chg_yel_pri_ena,cfg->yel_pri,cfg->yel_drop_ena,
                  cfg->chg_red_pri_ena,cfg->red_pri,cfg->red_drop_ena,cfg->policer_action_id);
                  OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_qos_set_policer_action_template err "
                  ": ret = %ld (unit = %ld, chg_gre_pri_ena = %d,"
         "gre_pri = %d, gre_drop_ena = %d, chg_yel_pri_ena = %d, yel_pri = %d, yel_drop_ena = %d,"
         " chg_red_pri_ena = %d,red_pri=%d,red_drop_ena=%d,id=%ld)\n\n",
                  FUNCTION,LINE,ret,unit,cfg->chg_gre_pri_ena,cfg->gre_pri,cfg->gre_drop_ena,
                  cfg->chg_yel_pri_ena,cfg->yel_pri,cfg->yel_drop_ena,
                  cfg->chg_red_pri_ena,cfg->red_pri,cfg->red_drop_ena,cfg->policer_action_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
NBB_VOID spm_clear_policer_color_action_template_error_log(NBB_ULONG unit,
    NBB_ULONG action_id, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld  fhdrv_qos_clear_policer_action_template err : ret = %ld "
            "(unit = %ld, MeterId = %ld)\n\n",
                  FUNCTION,LINE,ret,unit,action_id);
        OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_qos_clear_policer_action_template err : ret = %ld "
            "(unit = %ld, MeterId = %ld)\n\n",
                  FUNCTION,LINE,ret,unit,action_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
    }  
}

#endif


#if 10





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
NBB_VOID  spm_hqos_l3txpw_error_log(NBB_ULONG voq, 
    NBB_ULONG hqosEn, NBB_ULONG posid, const NBB_CHAR *FUNCTION,
    NBB_ULONG LINE, NBB_LONG ret)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line=%ld  ret=%ld ApiC3SetL3TxPwHqos voq=%ld posid=%ld,hqosEn=%s\n\n",
                FUNCTION,LINE,ret,voq, posid,hqosEn?"使能":"不使能");
        OS_SPRINTF(ucMessage,"%s line=%ld  ret=%ld ApiC3SetL3TxPwHqos voq=%ld posid=%ld,hqosEn=%s\n\n",
                FUNCTION,LINE,ret,voq, posid,hqosEn?"使能":"不使能");
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);   
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_C3_ERROR,ret,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));  
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
NBB_VOID  spm_hqos_l3uni_error_log(
    NBB_ULONG voq, NBB_ULONG hqosEn, TX_PORT_T *l3uni,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != l3uni))
    {
        OS_PRINTF("%s line=%ld  ret=%ld ApiC3SetL3UniHqos voq=%ld "
                  "L3UNI slot=%d,port=%d,vid=%d,hqosEn=%s\n\n",
                FUNCTION,LINE,ret,voq, l3uni->slot,l3uni->port,l3uni->vid,
                hqosEn?"使能":"不使能");
        OS_SPRINTF(ucMessage,"%s line=%ld  ret=%ld ApiC3SetL3UniHqos voq=%ld "
                  "L3UNI slot=%d,port=%d,vid=%d,hqosEn=%s\n\n",
                FUNCTION,LINE,ret,voq, l3uni->slot,l3uni->port,l3uni->vid,
                hqosEn?"使能":"不使能");
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_C3_ERROR,ret,
    			ucMessage,FUNCTION,"LINE","",
    			0,0,LINE,0));  
    }
}

#endif


#if 11

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
NBB_VOID spm_hqos_almpm_addline_vc_log(
    ATG_DCI_VC_KEY *pkey, NBB_ULONG voq, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pkey))
    {
        OS_PRINTF("%s line=%ld almpm_addline_hqos_vc ret =%ld,VC (vc_id=%ld,"
				   "peer_ip=0x%lx,vc_type=%d) voq =%ld\n",FUNCTION,LINE,ret,
				   pkey->vc_id,pkey->peer_ip,pkey->vc_type,voq);
        OS_SPRINTF(ucMessage,"%s line=%ld almpm_addline_hqos_vc ret =%ld, VC "
				   "(vc_id=%ld,peer_ip=0x%lx,vc_type=%d) voq =%ld\n",FUNCTION,
				   LINE,ret,pkey->vc_id,pkey->peer_ip,pkey->vc_type,voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			HQOS_BMU_ERROR,ret,
        			ucMessage,FUNCTION,"LINE","",
        			0,0,LINE,0)); 
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
NBB_VOID spm_hqos_almpm_delline_vc_log(ATG_DCI_VC_KEY *pkey, 
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pkey))
    {
        OS_PRINTF("%s line=%ld almpm_delline_hqos_vc ret =%ld,VC (vc_id=%ld,"
				   "peer_ip=0x%lx,vc_type=%d)\n",FUNCTION,LINE,ret,pkey->vc_id,
				   pkey->peer_ip,pkey->vc_type);
        OS_SPRINTF(ucMessage,"%s line=%ld almpm_delline_hqos_vc ret =%ld,VC (vc_id=%ld,"
				   "peer_ip=0x%lx,vc_type=%d)\n",FUNCTION,LINE,ret,pkey->vc_id,
				   pkey->peer_ip,pkey->vc_type);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,ucMessage,FUNCTION,"LINE","",
        				0,0,LINE,0)); 
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
NBB_VOID spm_hqos_almpm_addline_vrf_log(
    SPM_QOS_VRF_INSTANSE_KEY *pkey, NBB_ULONG voq, 
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pkey))
    {
        OS_PRINTF("%s line=%ld almpm_addline_hqos_vrf ret = %ld,VRF (vrf_id=%d,"
				   "peer_ip=0x%lx,label=%ld), voq = %ld\n",FUNCTION,LINE,ret,
				   pkey->vrf_id,pkey->peer_ip,pkey->label,voq);
        OS_SPRINTF(ucMessage,"%s line=%ld almpm_addline_hqos_vrf ret = %ld,"
				   "VRF (vrf_id=%d,peer_ip=0x%lx,label=%ld), voq = %ld\n",
				   FUNCTION,LINE,ret,pkey->vrf_id,pkey->peer_ip,pkey->label,voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,ucMessage,FUNCTION,"LINE","",
        				0,0,LINE,0)); 
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
NBB_VOID spm_hqos_almpm_delline_vrf_log(SPM_QOS_VRF_INSTANSE_KEY *pkey, 
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pkey))
    {
        OS_PRINTF("%s line=%ld almpm_delline_hqos_vrf ret = %ld,VRF (vrf_id=%d,"
				   "peer_ip=0x%lx,label=%ld)\n",FUNCTION,LINE,ret,pkey->vrf_id,
				   pkey->peer_ip,pkey->label);
        OS_SPRINTF(ucMessage,"%s line=%ld almpm_delline_hqos_vrf ret = %ld,"
				   "VRF (vrf_id=%d,peer_ip=0x%lx,label=%ld)\n",FUNCTION,LINE,
				   ret,pkey->vrf_id,pkey->peer_ip,pkey->label);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,ucMessage,FUNCTION,"LINE","",
        				0,0,LINE,0)); 
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
NBB_VOID spm_hqos_almpm_addline_intf_log(NBB_ULONG logic_key, 
    NBB_ULONG voq, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, 
    NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (0 != logic_key))
    {
        OS_PRINTF("%s line=%ld spm_hqos_almpm_addline_intf ret = %ld,INTF "
				   "(logic_key=%ld), voq = %ld\n",FUNCTION,LINE,ret,logic_key,voq);
        OS_SPRINTF(ucMessage,"%s line=%ld spm_hqos_almpm_addline_intf ret = %ld,INTF "
				   "(logic_key=%ld), voq = %ld\n",FUNCTION,LINE,ret,logic_key,voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,ucMessage,FUNCTION,"LINE","",
        				0,0,LINE,0)); 
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
NBB_VOID spm_hqos_almpm_delline_intf_log(NBB_ULONG logic_key, 
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (0 != logic_key))
    {
        OS_PRINTF("%s line=%ld almpm_delline_hqos_intf ret = %ld,INTF "
				   "(logic_key=%ld)\n",FUNCTION,LINE,ret,logic_key);
        OS_SPRINTF(ucMessage,"%s line=%ld almpm_delline_hqos_intf ret = %ld,INTF "
				   "(logic_key=%ld)\n",FUNCTION,LINE,ret,logic_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,ucMessage,FUNCTION,"LINE","",
        				0,0,LINE,0)); 
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
NBB_VOID spm_hqos_almpm_addline_error_log(
    NBB_ULONG voq,NBB_ULONG node_index,NBB_USHORT slot,
    NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line=%ld  ret=%ld,almpm_addline_hqos,slot=%d,node_index=%ld,"
			  	   "voq=%ld\n\n",FUNCTION,LINE,ret,slot,node_index,voq);
        OS_SPRINTF(ucMessage,"%s line=%ld  ret=%ld,almpm_addline_hqos,slot=%d,"
				   "node_index=%ld,voq=%ld\n\n",FUNCTION,LINE,ret,slot,node_index,
				   voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					HQOS_BMU_ERROR,ret,ucMessage,FUNCTION,"LINE","",
    					0,0,LINE,0));  
    }
}

/*****************************************************************************
   函 数 名  : spm_hqos_almpm_attack_error_log
   功能描述  : 攻击性能统计error log
   输入参数  : 
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2015年8月17日 星期二
   作    者  : tianf
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_hqos_almpm_attack_error_log(
    NBB_ULONG pAction,NBB_ULONG pThreshold,NBB_ULONG pTimeInterval,
    NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line=%ld  ret=%ld,Action = %d, "
                  "AlarmThreshold = %ld,CountTimeInterval = %d\n\n",
                  FUNCTION,LINE,ret,pAction,pThreshold,pTimeInterval);
        OS_SPRINTF(ucMessage,"%s line=%ld  ret=%ld,Action = %d, "
                  "AlarmThreshold = %ld,CountTimeInterval = %d\n\n",
                  FUNCTION,LINE,ret,pAction,pThreshold,pTimeInterval);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					HQOS_BMU_ERROR,ret,ucMessage,FUNCTION,"LINE","",
    					0,0,LINE,0));  
    }
}


#endif
