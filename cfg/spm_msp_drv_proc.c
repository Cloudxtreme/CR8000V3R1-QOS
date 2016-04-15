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

/*****************************************************************************
 函 数 名  : spm_msp_aps_del_prot_group
 功能描述  : MSP保护 基本配置 1）
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_DOWN_USER_GROUP_QOS *pst_down_group_qos
             NBB_ULONG intf_pos_id       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月23日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_msp_aps_del_prot_group(NBB_ULONG msp_key)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_msp_aps_del_prot_group");

#ifdef SPU   
    func_ret = delApsLmspProGroup(msp_key);

    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
    }      

#endif    

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}

/*****************************************************************************
 函 数 名  : spm_msp_aps_add_prot_group
 功能描述  : MSP保护 基本配置 1）
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_DOWN_USER_GROUP_QOS *pst_down_group_qos
             NBB_ULONG intf_pos_id       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月23日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_msp_aps_add_prot_group(
    SPM_MSP_CB *pst_msp,
    ATG_DCI_MSP_BASIC *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    APS_LMSP_PRO_GROUP pst_lmsp;
    SPM_PORT_INFO_CB st_port_info ;
    SPM_PORT_INFO_CB st_bak_port_info ;

    NBB_TRC_ENTRY("spm_msp_aps_add_prot_group");

    if (NULL == pst_msp)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_tlv_cfg)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&pst_lmsp, 0, sizeof(APS_LMSP_PRO_GROUP));
    OS_MEMSET(&st_port_info, 0, sizeof(SPM_PORT_INFO_CB));  
    OS_MEMSET(&st_bak_port_info, 0, sizeof(SPM_PORT_INFO_CB));
    
#ifdef SPU   
    pst_lmsp.apsProType = pst_tlv_cfg->prot_type;
    pst_lmsp.holdoffTimeExpire = pst_tlv_cfg->delay_time;
    pst_lmsp.wtrTimeExpire = pst_tlv_cfg->restore_time;
    pst_lmsp.revertMode = pst_tlv_cfg->return_type;

    pst_lmsp.wPortIndex = pst_tlv_cfg->master_port_index;      
    spm_get_portid_from_physical_port_index(pst_tlv_cfg->master_port_index,&st_port_info);
    pst_lmsp.wSlot = st_port_info.slot_id;
    pst_lmsp.wCardId = st_port_info.card_id;
    pst_lmsp.wPortId = st_port_info.port_id;

    pst_lmsp.pPortIndex = pst_tlv_cfg->slave_port_index;
    spm_get_portid_from_physical_port_index(pst_tlv_cfg->slave_port_index,&st_bak_port_info);
    pst_lmsp.pSlot = st_bak_port_info.slot_id;
    pst_lmsp.pCardId = st_bak_port_info.card_id;
    pst_lmsp.pPortId = st_bak_port_info.port_id;        

    func_ret = addApsLmspProGroup(pst_msp->msp_key,&pst_lmsp);

    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
    }       

#endif    

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}

/*****************************************************************************
 函 数 名  : spm_msp_drv_add_prot_group
 功能描述  : MSP保护 基本配置 1）
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_DOWN_USER_GROUP_QOS *pst_down_group_qos
             NBB_ULONG intf_pos_id       
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月23日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_msp_drv_add_prot_group(
    SPM_MSP_CB *pst_msp,
    ATG_DCI_MSP_BASIC *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    NBB_CHAR c3_unit = 0;
    NBB_ULONG msp_key = 0;
    
    NBB_TRC_ENTRY("spm_msp_drv_add_prot_group");

    if (NULL == pst_msp)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (NULL == pst_tlv_cfg)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    } 
            
#ifdef SPU
    msp_key = pst_msp->msp_key;

    //msp 1+1
    if ((pst_tlv_cfg->prot_type == ATG_DCI_MSP_UNI_LINK_1J1_PROT)
        || (pst_tlv_cfg->prot_type == ATG_DCI_MSP_BI_LINK_1J1_PROT))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret += ApiC3SetFrrGroupProtType(c3_unit, SHARED.msp_frr_group_id[msp_key - 1], VP_NHI_1PLUS1);
        }
    }
    //msp 1:1
    else if ((pst_tlv_cfg->prot_type == ATG_DCI_MSP_UNI_LINK_1B1_PROT)
        || (pst_tlv_cfg->prot_type == ATG_DCI_MSP_BI_LINK_1B1_PROT))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret += ApiC3SetFrrGroupProtType(c3_unit, SHARED.msp_frr_group_id[msp_key - 1], VP_NHI_FRR);
        }
    }
    //无保护
    else
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret += ApiC3SetFrrGroupProtType(c3_unit, SHARED.msp_frr_group_id[msp_key - 1], VP_NHI_GROUP);
        }
    }       

    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code_and_two_value(ret,c3_unit - 1,func_ret);
    }       

#endif    

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}


