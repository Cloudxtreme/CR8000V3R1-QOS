/******************************************************************************

                  版权所有 (C), 1999-2014, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_msp_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2014年4月10日
  最近修改   :
  功能描述   : 处理端口镜像功能
  函数列表   :
              spm_alloc_msp_cb
              spm_free_msp_cb
              spm_init_msp_tree
              spm_rcv_dci_set_msp
  修改历史   :
  1.日    期   : 2014年4月10日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char g_msp_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_init_msp_tree
 功能描述  : 初始化端口镜像树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年4月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_msp_tree()
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_msp_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_MSP_CB, msp_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.msp_tree, compare_ulong,
        (NBB_USHORT) avll_key_offset,
        (NBB_USHORT) NBB_OFFSETOF(SPM_MSP_CB, spm_msp_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_alloc_msp_cb
 功能描述  : 分配MSP保护结构体的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年4月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_MSP_CB* spm_alloc_msp_cb()
{
    SPM_MSP_CB *pst_msp = NULL;

    NBB_TRC_ENTRY("spm_alloc_msp_cb");

    /* 分配一个新的MSP保护配置条目。*/
    pst_msp = (SPM_MSP_CB *)NBB_MM_ALLOC(sizeof(SPM_MSP_CB),NBB_NORETRY_ACT, MEM_SPM_MSP_CB);
    
    if (NULL == pst_msp)
    {
        goto EXIT_LABEL;
    }

    /* 初始化MSP保护配置条目 */
    OS_MEMSET(pst_msp, 0, sizeof(SPM_MSP_CB));
    pst_msp->basic_cfg_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pst_msp->spm_msp_handle = NBB_CREATE_HANDLE(pst_msp, HDL_SPM_MSP_CB);
  
    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_msp->spm_msp_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pst_msp);
}

/*****************************************************************************
 函 数 名  : spm_free_msp_cb
 功能描述  : 释放MSP保护结构体的内存空间
 输入参数  : SPM_MSP_CB *pst_msp  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年4月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_msp_cb(SPM_MSP_CB *pst_msp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
  
    NBB_TRC_ENTRY("spm_free_msp_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_msp, sizeof(SPM_MSP_CB), MEM_SPM_MSP_CB); 

    if (NULL == pst_msp)
    {
    	goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (NULL != pst_msp->basic_cfg_cb)
    {
        NBB_MM_FREE(pst_msp->basic_cfg_cb, MEM_SPM_MSP_BASIC_CB);  
        pst_msp->basic_cfg_cb = NULL;
    }    

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_msp->spm_msp_handle, HDL_SPM_MSP_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_msp, MEM_SPM_MSP_CB);  
    pst_msp = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_msp_cb
   功能描述  : 释放MSP保护配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年04月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_all_msp_cb()
{
    SPM_MSP_CB *pst_msp = NULL;

    for (pst_msp = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree);
        pst_msp != NULL;
        pst_msp = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree))
    {    	
        AVLL_DELETE(SHARED.msp_tree, pst_msp->spm_msp_node);
        spm_free_msp_cb(pst_msp);
    }
}

/*****************************************************************************
函 数 名  : spm_physical_port_basic_proc
功能描述  : 物理接口子tlv--基本配置，处理函数 1)
输入参数  : 
输出参数  : 无
返 回 值  : 
调用函数  : 
被调函数  : 

修改历史      :
1.日    期   : 2015年8月10日
作    者   : zhangzhm
修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_msp_basic_proc(ATG_DCI_SET_MSP *pst_set_msp,
    SPM_MSP_CB *pst_msp,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_MSP_BASIC *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
     
    if(NULL == pst_set_msp)
    {
        ret = MSP_BASIC_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_msp)
    {
        ret = MSP_BASIC_PROC_LOCAL_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_msp, 
               &pst_set_msp->basic_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = MSP_BASIC_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_MSP_BASIC *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_msp_cfg_print_setting)
            {
                printf("  1）basic Config\n");
                spm_dbg_print_msp_basic_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  1）basic Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            spm_msp_aps_add_prot_group(pst_msp,pst_tlv_cfg);
            spm_msp_drv_add_prot_group(pst_msp,pst_tlv_cfg);

            /* 保存数据 */
            if (NULL == pst_msp->basic_cfg_cb)
            {
                pst_msp->basic_cfg_cb = (ATG_DCI_MSP_BASIC *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_MSP_BASIC),
                    NBB_NORETRY_ACT,
                    MEM_SPM_MSP_BASIC_CB);
                
                if (NULL == pst_msp->basic_cfg_cb)
                {
                    ret = MSP_BASIC_PROC_ALLOC_CB_FAIL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_msp->basic_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_MSP_BASIC));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = MSP_BASIC_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = MSP_BASIC_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = MSP_BASIC_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_msp
 功能描述  : 处理端口镜像IPS消息
 输入参数  : ATG_DCI_SET_MSP *pst_set_msp  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_msp_proc(ATG_DCI_SET_MSP *pst_set_msp)
{
    NBB_INT ret = SUCCESS;
    SPM_MSP_CB *pst_msp = NULL;   
    NBB_ULONG oper_basic = ATG_DCI_OPER_NULL;   
    NBB_ULONG msp_key = 0;
    NBB_BYTE uc_if_exist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_msp_proc");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pst_set_msp != NULL);

    if (NULL == pst_set_msp)
    {
        ret = MSP_PROC_IPS_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pst_set_msp->return_code = ATG_DCI_RC_OK;
    msp_key = pst_set_msp->key;
    pst_msp = AVLL_FIND(SHARED.msp_tree, &msp_key);
    
    if (NULL == pst_msp)       /* 如果条目不存在*/
    {
        uc_if_exist = ATG_DCI_UNEXIST;
    }    

    /* 获取子配置操作 */
    oper_basic = pst_set_msp->oper_basic;
   
    /* 删除整个条目 */
    if (TRUE == pst_set_msp->delete_struct)
    {        
        /* 如果条目不存在，不删除 */
        if (ATG_DCI_UNEXIST == uc_if_exist)
        {
            ret = MSP_PROC_DEL_NULL_LOCAL_CFG;
            spm_l2_record_err_code(ret);       
            goto EXIT_LABEL;
        }      
        else/* 存在，删除 */
        {
            spm_dbg_print_msp_head(msp_key, SPM_OPER_DEL);
            spm_dbg_record_msp_head(msp_key, SPM_OPER_DEL);
            spm_msp_aps_del_prot_group(msp_key);
            
            if (NULL != pst_msp)
            {
                AVLL_DELETE(SHARED.msp_tree, pst_msp->spm_msp_node);  

                //释放MSP保护协议节点的内存空间
                spm_free_msp_cb(pst_msp);
            }
        }
    }
    /* 增加或更新条目 */
    else
    {        
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ATG_DCI_UNEXIST == uc_if_exist)
        {
            spm_dbg_print_msp_head(msp_key, SPM_OPER_ADD);
            spm_dbg_record_msp_head(msp_key, SPM_OPER_ADD);

            /* 申请一个新条目的内存空间 */
            pst_msp = spm_alloc_msp_cb();

            if(NULL == pst_msp)
            {
                ret = MSP_PROC_ALLOC_CB_FAIL;
                spm_l2_record_err_code(ret);   
                goto EXIT_LABEL;
            }
            if (NULL != pst_msp)
            {
                //插入到tree中
                pst_msp->msp_key = msp_key;

                //coverity[no_effect_test]
                AVLL_INSERT(SHARED.msp_tree, pst_msp->spm_msp_node);
            }
        }
        else
        {
            spm_dbg_print_msp_head(msp_key, SPM_OPER_UPD);
            spm_dbg_record_msp_head(msp_key, SPM_OPER_UPD);
        }
        
        /************************************ 1) 基本配置 **************************************/
        if(ATG_DCI_OPER_NULL != oper_basic)
        {
            spm_msp_basic_proc(pst_set_msp, pst_msp,oper_basic); 
        }
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}


