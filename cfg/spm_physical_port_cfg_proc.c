/******************************************************************************

              版权所有 (C), 烽火通信科技股份有限公司

******************************************************************************

文 件 名   : spm_physical_port_cfg.c
版 本 号   : 初稿
作    者   : zhangzhm
生成日期   : 2015年8月10日
最近修改   :
功能描述   : 物理接口配置相关处理函数
函数列表   :
修改历史   :
1.日    期   : 2015年8月10日
作    者   : zhangzhm
修改内容   : 创建文件

******************************************************************************/
    
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL
    
#include <nbase.h>
#include <spmincl.h>
    
#ifdef PHYSICAL_PORT_CFG

extern unsigned char g_physical_port_cfg_print_setting;//开关

/*****************************************************************************
 函 数 名  : spm_del_physical_port_cfg
 功能描述  : 删除物理接口配置
 输入参数  : SPM_PHYSICAL_PORT* pstPhysicalPort  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_physical_port_del_cfg(SPM_PHYSICAL_PORT_CB* pst_physical_port NBB_CCXT_T NBB_CXT)
{
    return 0;
    
}

/*****************************************************************************
函 数 名  : spm_physical_port_get_phy_info_from_index
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
NBB_INT spm_physical_port_get_phy_info_from_index(NBB_ULONG port_index,
    SPM_PHYSICAL_PORT_INFO_CB *pst_phy_port_info)
{
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
     
    if(NULL == pst_phy_port_info)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
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
NBB_INT spm_physical_port_basic_proc(
    ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_BASIC_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_basic_proc"); 
    
    if(NULL == pst_set_physical_port)
    {
        ret = PHY_PORT_BASIC_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = PHY_PORT_BASIC_PROC_LOCAL_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
               &pst_set_physical_port->basic_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = PHY_PORT_BASIC_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_BASIC_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  1）basic Config\n");
                spm_dbg_print_physical_port_basic_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  1）basic Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_basic_cfg(pst_physical_port,pst_tlv_cfg);

            /* 保存数据 */
            if (NULL == pst_physical_port->basic_cfg_cb)
            {
                pst_physical_port->basic_cfg_cb = (ATG_DCI_PHY_PORT_BASIC_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_BASIC_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_BASIC_CB);
                
                if (NULL == pst_physical_port->basic_cfg_cb)
                {
                    ret = PHY_PORT_BASIC_PROC_ALLOC_CB_FAIL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->basic_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_BASIC_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = PHY_PORT_BASIC_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = PHY_PORT_BASIC_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = PHY_PORT_BASIC_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}

/*****************************************************************************
函 数 名  : spm_physical_port_eth_phy_proc
功能描述  : 物理接口子tlv--  "2）以太网接口物理配置"  
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
NBB_VOID spm_physical_port_eth_phy_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_PHY_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_eth_phy_proc"); 
    
    if(NULL == pst_set_physical_port)
    {
        ret = PHY_PORT_ETH_PHY_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = PHY_PORT_ETH_PHY_PROC_LOCAL_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
               &pst_set_physical_port->physical_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = PHY_PORT_ETH_PHY_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_PHY_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  2）eth physical Config\n");
                spm_dbg_print_physical_port_phy_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  2）eth physical Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_phy_cfg(pst_physical_port,pst_tlv_cfg);

            //if(SUCCESS != func_ret)
            //{
                //ret = PHY_PORT_ETH_PHY_PROC_CALL_DRV_FAIL;
                //spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            //}

            /* 保存数据 */
            if (NULL == pst_physical_port->phy_cfg_cb)
            {
                pst_physical_port->phy_cfg_cb = (ATG_DCI_PHY_PORT_PHY_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_PHY_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_PHY_CB);
                
                if (NULL == pst_physical_port->phy_cfg_cb)
                {
                    ret = PHY_PORT_ETH_PHY_PROC_ALLOC_CB_FAIL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->phy_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_PHY_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = PHY_PORT_ETH_PHY_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = PHY_PORT_ETH_PHY_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = PHY_PORT_ETH_PHY_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;      
}

/*****************************************************************************
函 数 名  : spm_physical_port_pos_proc
功能描述  : 物理接口子tlv-- "3)POS接口链路层配置"
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
NBB_VOID spm_physical_port_pos_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_POS_LINK_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_pos_proc"); 
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
               &pst_set_physical_port->pos_link_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_POS_LINK_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  3）pos link Config\n");
                spm_dbg_print_physical_port_pos_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  3）pos link Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* 保存数据 */
            if (NULL == pst_physical_port->pos_link_cfg_cb)
            {
                pst_physical_port->pos_link_cfg_cb = (ATG_DCI_PHY_PORT_POS_LINK_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_POS_LINK_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_POS_CB);
                
                if (NULL == pst_physical_port->pos_link_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->pos_link_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_POS_LINK_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

/*****************************************************************************
函 数 名  : spm_physical_port_sdh_sovhd_proc
功能描述  : 物理接口子tlv--4)SDH段开销配置
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
NBB_VOID spm_physical_port_sdh_sovhd_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_sdh_sovhd_proc"); 
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->sdh_soverhead_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  4）sdh sovhd Config\n");
                spm_dbg_print_physical_port_stm1_sovhd_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  4）sdh sovhd Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* 保存数据 */
            if (NULL == pst_physical_port->stm1_sovhd_cfg_cb)
            {
                pst_physical_port->stm1_sovhd_cfg_cb = (ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_SDH_SOVHD_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_SDH_SOVHD_CB);
                
                if (NULL == pst_physical_port->stm1_sovhd_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->stm1_sovhd_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_SDH_SOVHD_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
               
/*****************************************************************************
函 数 名  : spm_physical_port_ces_proc
功能描述  : 物理接口子tlv--5)CES接口配置
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
NBB_VOID spm_physical_port_ces_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_CES_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_ces_proc");  
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->ces_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_CES_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  5）ces Config\n");
                spm_dbg_print_physical_port_ces_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  5）ces Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* 保存数据 */
            if (NULL == pst_physical_port->ces_cfg_cb)
            {
                pst_physical_port->ces_cfg_cb = (ATG_DCI_PHY_PORT_CES_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_CES_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_CES_CB);
                
                if (NULL == pst_physical_port->ces_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->ces_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_CES_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;      
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_cep_proc
功能描述  : 物理接口子tlv--6)CEP接口开销配置
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
NBB_VOID spm_physical_port_cep_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_CEP_OVHD_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_cep_proc");  
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->cep_overhead_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_CEP_OVHD_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  6）cep Config\n");
                spm_dbg_print_physical_port_cep_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  6）cep Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* 保存数据 */
            if (NULL == pst_physical_port->cep_ovhd_cfg_cb)
            {
                pst_physical_port->cep_ovhd_cfg_cb = (ATG_DCI_PHY_PORT_CEP_OVHD_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_CEP_OVHD_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_CEP_CB);
                
                if (NULL == pst_physical_port->cep_ovhd_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->cep_ovhd_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_CEP_OVHD_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_efm_oam_proc
功能描述  : 物理接口子tlv--7）以太网EFM OAM
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
NBB_VOID spm_physical_port_efm_oam_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_EFM_OAM_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_efm_oam_proc");  
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->efm_oam_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_EFM_OAM_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  7）eth efm oam Config\n");
                spm_dbg_print_physical_port_efm_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  7）eth efm oam Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* 保存数据 */
            if (NULL == pst_physical_port->efm_oam_cfg_cb)
            {
                pst_physical_port->efm_oam_cfg_cb = (ATG_DCI_PHY_PORT_EFM_OAM_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_EFM_OAM_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_EFM_OAM_CB);
                
                if (NULL == pst_physical_port->efm_oam_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->efm_oam_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_EFM_OAM_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_time_sync_proc
功能描述  : 物理接口子tlv--8)时间同步端口配置
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
NBB_VOID spm_physical_port_time_sync_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_TIME_SYNC_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_time_sync_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->time_sync_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_TIME_SYNC_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  8）time sync Config\n");
                spm_dbg_print_physical_port_time_sync_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  8）time sync Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_time_sync(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->time_sync_cfg_cb)
            {
                pst_physical_port->time_sync_cfg_cb = (ATG_DCI_PHY_PORT_TIME_SYNC_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_TIME_SYNC_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_TIME_SYNC_CB);
                
                if (NULL == pst_physical_port->time_sync_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->time_sync_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_TIME_SYNC_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_msg_suppress_proc
功能描述  : 物理接口子tlv--9）以太网接口报文抑制
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
NBB_VOID spm_physical_port_msg_suppress_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_msg_suppress_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->msg_suppr_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  9）eth msg suppr Config\n");
                spm_dbg_print_physical_port_msg_suppr_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  9）eth msg suppr Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_msg_suppr(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->msg_suppr_cfg_cb)
            {
                pst_physical_port->msg_suppr_cfg_cb = (ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_MSG_SUPPR_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_MSG_SUPPR_CB);
                
                if (NULL == pst_physical_port->msg_suppr_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->msg_suppr_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_MSG_SUPPR_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_flow_shape_proc
功能描述  : 物理接口子tlv--10）以太网接口流量整形
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
NBB_VOID spm_physical_port_flow_shape_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_FLOW_SHAPE *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_flow_shape_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->flow_shape_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_FLOW_SHAPE *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  10）eth flow shape Config\n");
                spm_dbg_print_physical_port_eth_flow_shape_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  10）eth flow shape Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_flow_shape(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->flow_shape_cfg_cb)
            {
                pst_physical_port->flow_shape_cfg_cb = (ATG_DCI_PHY_PORT_FLOW_SHAPE *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_FLOW_SHAPE),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_FLOW_SHAPE_CB);
                
                if (NULL == pst_physical_port->flow_shape_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->flow_shape_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_FLOW_SHAPE));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_schedule_policy_proc
功能描述  : 物理接口子tlv--11）以太网出口队列调度策略
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
NBB_VOID spm_physical_port_schedule_policy_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_SCHEDUL_POLICY *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_schedule_policy_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->schedul_policy_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_SCHEDUL_POLICY *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  11）eth schedul policy Config\n");
                spm_dbg_print_physical_port_eth_schedul_policy_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  11）eth schedul policy Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_schedul_policy(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->schedule_policy_cfg_cb)
            {
                pst_physical_port->schedule_policy_cfg_cb = (ATG_DCI_PHY_PORT_SCHEDUL_POLICY *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_SCHEDUL_POLICY),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_SCHEDUL_POLICY_CB);
                
                if (NULL == pst_physical_port->schedule_policy_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->schedule_policy_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_SCHEDUL_POLICY));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_congest_policy_proc
功能描述  : 物理接口子tlv--12）以太网出口队列拥塞策略
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
NBB_VOID spm_physical_port_congest_policy_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_CONGEST_POLICY *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_congest_policy_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->congest_policy_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_CONGEST_POLICY *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  12）eth congest policy Config\n");
                spm_dbg_print_physical_port_eth_congest_policy_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  12）eth congest policy Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_congest_policy(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->congest_policy_cfg_cb)
            {
                pst_physical_port->congest_policy_cfg_cb = (ATG_DCI_PHY_PORT_CONGEST_POLICY *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_CONGEST_POLICY),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_CONGEST_POLICY_CB);
                
                if (NULL == pst_physical_port->congest_policy_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->congest_policy_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_CONGEST_POLICY));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_ipv4_netflow_proc
功能描述  : 物理接口子tlv--13）IPV4流采样使能配置IPV4 NetFlow
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
NBB_VOID spm_physical_port_ipv4_netflow_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_IPV4NF_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_ipv4_netflow_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->ipv4nf_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_IPV4NF_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  13）ipv4 nf enable Config\n");
                spm_dbg_print_physical_port_ipv4_nf_enable(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  13）ipv4 nf enable Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_ipv4_nf_enable(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->ipv4_nf_cfg_cb)
            {
                pst_physical_port->ipv4_nf_cfg_cb = (ATG_DCI_PHY_PORT_IPV4NF_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_IPV4_NF_CB);
                
                if (NULL == pst_physical_port->ipv4_nf_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->ipv4_nf_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_ip_nf_sample_proc
功能描述  : 物理接口子tlv--14）IP流采样镜像
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
NBB_VOID spm_physical_port_ip_nf_sample_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_IPNF_SAMPLER *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_ip_nf_sample_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->ipnf_sampler_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_IPNF_SAMPLER *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  14）ip nf sampler Config\n");
                spm_dbg_print_physical_port_ip_nf_smapler(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  14）ip nf sampler Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_ip_nf_smapler(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->ip_sample_cfg_cb)
            {
                pst_physical_port->ip_sample_cfg_cb = (ATG_DCI_PHY_PORT_IPNF_SAMPLER *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_IPNF_SAMPLER),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_IPV4_SAMPLE_CB);
                
                if (NULL == pst_physical_port->ip_sample_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->ip_sample_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_IPNF_SAMPLER));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_ipv6_netflow_proc
功能描述  : 物理接口子tlv--15）IPV6流采样使能配置
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
NBB_VOID spm_physical_port_ipv6_netflow_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_IPV6NF_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_ipv6_netflow_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->ipv6nf_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_IPV6NF_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  15）ipv6 nf enable Config\n");
                spm_dbg_print_physical_port_ipv6_nf_enable(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  15）ipv6 nf enable Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_ipv6_nf_enable(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->ipv6_nf_cfg_cb)
            {
                pst_physical_port->ipv6_nf_cfg_cb = (ATG_DCI_PHY_PORT_IPV6NF_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_IPV6_NF_CB);
                
                if (NULL == pst_physical_port->ipv6_nf_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->ipv6_nf_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

       
/*****************************************************************************
函 数 名  : spm_physical_port_pve_proc
功能描述  : 物理接口子tlv--17）POVE
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
NBB_VOID spm_physical_port_pve_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_POVE *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_physical_port_pve_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->pove_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_POVE *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  17）pove Config\n");
                
                //spm_dbg_print_physical_port_pove_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  17）pove Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_pove(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->pove_cfg_cb)
            {
                pst_physical_port->pove_cfg_cb = (ATG_DCI_PHY_PORT_POVE *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_POVE),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_POVE_CB);
                
                if (NULL == pst_physical_port->pove_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->pove_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_POVE));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}


/*****************************************************************************
函 数 名  : spm_physical_port_common_proc
功能描述  : 物理接口子tlv--18）接口通用配置
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
NBB_VOID spm_physical_port_common_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_COMMON_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_common_proc");   
     
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->common_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_COMMON_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  18）common Config\n");
                spm_dbg_print_physical_port_common_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  18）common Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_common_proc(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->common_cfg_cb)
            {
                pst_physical_port->common_cfg_cb = (ATG_DCI_PHY_PORT_COMMON_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_COMMON_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_COMMON_CB);
                
                if (NULL == pst_physical_port->common_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->common_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_COMMON_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_sdh_tovhd_proc
功能描述  : 物理接口子tlv--19) SDH通道开销配置
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
NBB_VOID spm_physical_port_sdh_tovhd_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_sdh_tovhd_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->sdh_toverhead_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  19）sdh tovhd Config\n");
                spm_dbg_print_physical_port_sdh_tovhd_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  19）sdh tovhd Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

            /* 保存数据 */
            if (NULL == pst_physical_port->stm1_tovhd_cfg_cb)
            {
                pst_physical_port->stm1_tovhd_cfg_cb = (ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_SDH_TOVHD_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_SDH_TOVHD_CB);
                
                if (NULL == pst_physical_port->stm1_tovhd_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->stm1_tovhd_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_SDH_TOVHD_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
函 数 名  : spm_physical_port_als_proc
功能描述  : 物理接口子tlv--20)ALS配置
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
NBB_VOID spm_physical_port_als_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_ALS_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_als_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->als_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_ALS_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  20）als Config\n");
                spm_dbg_print_physical_port_als_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  20）als Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_als_cfg(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->als_cfg_cb)
            {
                pst_physical_port->als_cfg_cb = (ATG_DCI_PHY_PORT_ALS_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_ALS_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_ALS_CB);
                
                if (NULL == pst_physical_port->als_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->als_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_ALS_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

        
/*****************************************************************************
函 数 名  : spm_physical_port_thres_proc
功能描述  : 物理接口子tlv--21)端口发送接收光功率门限值配置
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
NBB_VOID spm_physical_port_thres_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_THRES *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_thres_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->thres_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_THRES *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  21）thres Config\n");
                
                //spm_dbg_print_physical_port_thres_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  21）thres Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

            /* 保存数据 */
            if (NULL == pst_physical_port->thres_cfg_cb)
            {
                pst_physical_port->thres_cfg_cb = (ATG_DCI_PHY_PORT_THRES *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_THRES),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_THRES_CB);
                
                if (NULL == pst_physical_port->thres_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->thres_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_THRES));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

         
/*****************************************************************************
函 数 名  : spm_physical_port_mcc_type_proc
功能描述  : 物理接口子tlv--22）MCC通道类型
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
NBB_VOID spm_physical_port_mcc_type_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_MCC *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_mcc_type_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //基本配置首地址
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->mcc_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_MCC *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  22）mcc Config\n");
                
                //spm_dbg_print_physical_port_mcc_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  22）mcc Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_mcc_cfg(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* 保存数据 */
            if (NULL == pst_physical_port->mcc_cfg_cb)
            {
                pst_physical_port->mcc_cfg_cb = (ATG_DCI_PHY_PORT_MCC *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_MCC),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_MCC_CB);
                
                if (NULL == pst_physical_port->mcc_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->mcc_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_MCC));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*更新，不处理*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*删除*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

/*****************************************************************************
函 数 名  : spm_physical_port_alloc_cb
功能描述  : 为接口物理配置结构分配内存空间
输入参数  :   
输出参数  : 无
返 回 值  : SPM_PHYSICAL_PORT_CB
调用函数  : 
被调函数  : 

修改历史      :
1.日    期   : 2015年8月8日
作    者   : zhangzhm
修改内容   : 新生成函数

*****************************************************************************/
SPM_PHYSICAL_PORT_CB * spm_alloc_physical_port_cb(NBB_CXT_T NBB_CXT)
{
    SPM_PHYSICAL_PORT_CB *pst_physical_port = NULL;

    NBB_TRC_ENTRY("spm_alloc_physical_port_cb");

    /* 分配一个新的接口物理配置条目。*/
    pst_physical_port = (SPM_PHYSICAL_PORT_CB *)NBB_MM_ALLOC(sizeof(SPM_PHYSICAL_PORT_CB),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHYSICAL_PORT_CB);
    if (pst_physical_port == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化接口物理配置条目 */
    OS_MEMSET(pst_physical_port, 0, sizeof(SPM_PHYSICAL_PORT_CB));

    pst_physical_port->basic_cfg_cb       = NULL;
    pst_physical_port->phy_cfg_cb         = NULL;
    pst_physical_port->pos_link_cfg_cb    = NULL;
    pst_physical_port->stm1_sovhd_cfg_cb   = NULL;
    pst_physical_port->ces_cfg_cb         = NULL;
    pst_physical_port->cep_ovhd_cfg_cb    = NULL;
    pst_physical_port->efm_oam_cfg_cb     = NULL;
    pst_physical_port->time_sync_cfg_cb   = NULL;
    pst_physical_port->msg_suppr_cfg_cb   = NULL;
    pst_physical_port->flow_shape_cfg_cb  = NULL;
    pst_physical_port->schedule_policy_cfg_cb = NULL;
    pst_physical_port->congest_policy_cfg_cb  = NULL;
    pst_physical_port->ipv4_nf_cfg_cb     = NULL;
    pst_physical_port->ip_sample_cfg_cb = NULL;
    pst_physical_port->ipv6_nf_cfg_cb     = NULL;
    pst_physical_port->pove_cfg_cb = NULL;    
    pst_physical_port->common_cfg_cb      = NULL;
    pst_physical_port->stm1_tovhd_cfg_cb = NULL;    
    pst_physical_port->als_cfg_cb      = NULL;    
    pst_physical_port->thres_cfg_cb = NULL;
    pst_physical_port->mcc_cfg_cb = NULL;

    /* 建立用于该接口物理配置条目的句柄，作为异步消息交换的相关器。*/
    pst_physical_port->spm_physical_port_handle = NBB_CREATE_HANDLE(pst_physical_port,
                                                                   HDL_SPM_PHYSICAL_PORT_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_PHYSICAL_PORT_CB allocated at %p with handle %#lx",
                   pst_physical_port, pst_physical_port->spm_physical_port_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_physical_port->spm_physical_port_node);    

    EXIT_LABEL : NBB_TRC_EXIT();

    return(pst_physical_port);
}


/*****************************************************************************
函 数 名  : spm_physical_port_free_cb
功能描述  : 释放接口物理配置节点的内存空间
输入参数  : SPM_PHYSICAL_PORT_CB* pstPhysicalPort   
输出参数  : 无
返 回 值  : 
调用函数  : 
被调函数  : 

修改历史      :
1.日    期   : 2015年8月8日
作    者   : zhangzhm
修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_physical_port_cb(SPM_PHYSICAL_PORT_CB* pst_physical_port)
{
    NBB_TRC_ENTRY("spm_free_physical_port_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_physical_port, sizeof(SPM_PHYSICAL_PORT_CB), MEM_SPM_PHYSICAL_PORT_CB); 

    if (pst_physical_port == NULL)
    {
    	goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_PHYSICAL_PORT_CB at %p", pstPhysicalPort));  
    NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pst_physical_port->port_index_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pst_physical_port->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->basic_cfg_cb, MEM_SPM_PHY_PORT_BASIC_CB);  
        pst_physical_port->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网接口物理配置数据块。                                          */
    /***************************************************************************/
    if (pst_physical_port->phy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->phy_cfg_cb, MEM_SPM_PHY_PORT_PHY_CB);  
        pst_physical_port->phy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放POS接口链路层配置数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->pos_link_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->pos_link_cfg_cb, MEM_SPM_PHY_PORT_POS_CB);  
        pst_physical_port->pos_link_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放STM-1接口开销配置数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->stm1_sovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->stm1_sovhd_cfg_cb, MEM_SPM_PHY_PORT_SDH_SOVHD_CB);  
        pst_physical_port->stm1_sovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放CES接口配置数据块。                                                 */
    /***************************************************************************/
    if (pst_physical_port->ces_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->ces_cfg_cb, MEM_SPM_PHY_PORT_CES_CB);  
        pst_physical_port->ces_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放CEP接口配置数据块。                                                 */
    /***************************************************************************/
    if (pst_physical_port->cep_ovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->cep_ovhd_cfg_cb, MEM_SPM_PHY_PORT_CEP_CB);  
        pst_physical_port->cep_ovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网EFM OAM配置数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->efm_oam_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->efm_oam_cfg_cb, MEM_SPM_PHY_PORT_EFM_OAM_CB);  
        pst_physical_port->efm_oam_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放时间同步端口配置数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->time_sync_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->time_sync_cfg_cb, MEM_SPM_PHY_PORT_TIME_SYNC_CB);  
        pst_physical_port->time_sync_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网接口报文抑制数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->msg_suppr_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->msg_suppr_cfg_cb, MEM_SPM_PHY_PORT_MSG_SUPPR_CB);  
        pst_physical_port->msg_suppr_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网接口流量整形数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->flow_shape_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->flow_shape_cfg_cb, MEM_SPM_PHY_PORT_FLOW_SHAPE_CB);  
        pst_physical_port->flow_shape_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网出口队列调度策略数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->schedule_policy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->schedule_policy_cfg_cb, MEM_SPM_PHY_PORT_SCHEDUL_POLICY_CB);  
        pst_physical_port->schedule_policy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网出口队列拥塞策略数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->congest_policy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->congest_policy_cfg_cb, MEM_SPM_PHY_PORT_CONGEST_POLICY_CB);  
        pst_physical_port->congest_policy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放IPV4流采样使能配置IPV4 NetFlow数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->ipv4_nf_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->ipv4_nf_cfg_cb, MEM_SPM_PHY_PORT_IPV4_NF_CB);  
        pst_physical_port->ipv4_nf_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放IPV4流采样镜像 IPV4 NetFlow sampler数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->ip_sample_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->ip_sample_cfg_cb, MEM_SPM_PHY_PORT_IPV4_SAMPLE_CB);  
        pst_physical_port->ip_sample_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放IPV6流采样使能配置IPV6 NetFlow数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->ipv6_nf_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->ipv6_nf_cfg_cb, MEM_SPM_PHY_PORT_IPV6_NF_CB);  
        pst_physical_port->ipv6_nf_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放pove数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->pove_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->pove_cfg_cb, MEM_SPM_PHY_PORT_POVE_CB);  
        pst_physical_port->pove_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放接口通用配置数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->common_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->common_cfg_cb, MEM_SPM_PHY_PORT_COMMON_CB);  
        pst_physical_port->common_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放SDH通道开销配置。                                           */
    /***************************************************************************/
    if (pst_physical_port->stm1_tovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->stm1_tovhd_cfg_cb, MEM_SPM_PHY_PORT_SDH_TOVHD_CB);  
        pst_physical_port->stm1_tovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放ALS 配置数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->als_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->als_cfg_cb, MEM_SPM_PHY_PORT_ALS_CB);  
        pst_physical_port->als_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放端口门限数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->thres_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->thres_cfg_cb, MEM_SPM_PHY_PORT_THRES_CB);  
        pst_physical_port->thres_cfg_cb = NULL;
    }
    
     /***************************************************************************/
    /* 释放MCC 配置数据块。                                           */
    /***************************************************************************/
    if (pst_physical_port->mcc_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->mcc_cfg_cb, MEM_SPM_PHY_PORT_MCC_CB);  
        pst_physical_port->mcc_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_physical_port->spm_physical_port_handle, HDL_SPM_PHYSICAL_PORT_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_physical_port, MEM_SPM_PHYSICAL_PORT_CB);  
    pst_physical_port = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_physical_port
   功能描述  : 释放端口物理配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年11月6日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/

NBB_VOID spm_free_all_physical_port(NBB_CXT_T NBB_CXT)
{
	SPM_PHYSICAL_PORT_CB *pstPhysicalPort = NULL;
	
    for (pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree);
         pstPhysicalPort != NULL;
         pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree))
    {
    	AVLL_DELETE(SHARED.physical_port_tree, pstPhysicalPort->spm_physical_port_node);
    	spm_free_physical_port_cb(pstPhysicalPort NBB_CCXT);
    }
}

/*****************************************************************************
函 数 名  : spm_physical_port_init_tree
功能描述  : 初始化存储接口物理配置的树
输入参数  : 无
输出参数  : 无
返 回 值  : 
调用函数  : 
被调函数  : 

修改历史      :
1.日    期   : 2015年8月8日
作    者   : zhangzhm
修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_physical_port_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_physical_port_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_PHYSICAL_PORT_CB, port_index_key);

    AVLL_INIT_TREE(SHARED.physical_port_tree, compare_ulong,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_PHYSICAL_PORT_CB, spm_physical_port_node)); 

    NBB_TRC_EXIT();
}



    
/*****************************************************************************
函 数 名  : spm_physical_port_proc
功能描述  : 物理接口总处理入口:接收ips消息，处理每一个子配置块
输入参数  : 无
输出参数  : 无
返 回 值  : 
调用函数  : 
被调函数  : 

修改历史      :
1.日    期   : 2015年8月8日
作    者   : zhangzhm
修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_physical_port_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port)
{
    //to do :
    //1.ces需要的配置，增加和删除怎么传递
    //2.端口适配告警的代码没有加进来
    //3.log怎么记录
    //端口告警反转没有加
    
    //NBB_CHAR print_message[SPM_MSG_INFO_LEN];
    
    /*记录下配置打印时间*/
    //struct timeval phy_tv_begin = {0};
    //struct timeval phy_tv_end = {0};
    //gettimeofday(&phy_tv_begin, NULL);

    /*记录开始处理配置时间*/
    //OS_SPRINTF(print_message, "time_test phy_port_proc begin = %ld\n", phy_tv_begin.tv_usec);
    //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);    
    NBB_INT ret = SUCCESS; 
    NBB_BYTE if_exist = ATG_DCI_EXIST;
    NBB_ULONG port_index = 0;
    SPM_PHYSICAL_PORT_CB *pst_physical_port = NULL;
    NBB_BOOL avll_ret_code = FALSE;

     /* 子配置的操作模式 */
    NBB_ULONG oper_basic           = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_physical        = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_pos             = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_sdh_tovhd       = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_ces             = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_cep             = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_efm_oam         = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_time_sync       = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_msg_suppress    = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_flow_shape      = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_schedule_policy = ATG_DCI_OPER_NULL;  
    NBB_ULONG oper_congest_policy  = ATG_DCI_OPER_NULL;  
    NBB_ULONG oper_ipv4_nf         = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_ip_sample       = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_ipv6_nf         = ATG_DCI_OPER_NULL; 
    NBB_ULONG oper_pove            = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_thres           = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_common          = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_als             = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_sdh_sovhd       = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_mcc             = ATG_DCI_OPER_NULL;
    
    NBB_TRC_ENTRY("spm_physical_port_proc");   
    
    if (NULL == pst_set_physical_port)
    {
        ret = PHY_PORT_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pst_set_physical_port->return_code = ATG_DCI_RC_OK;

    port_index = pst_set_physical_port->key;
    pst_physical_port = AVLL_FIND(SHARED.physical_port_tree, &port_index);

    /* 标记该条目是否已经存在 */
    if (NULL == pst_physical_port)
    {
        if_exist = ATG_DCI_UNEXIST;
    } 

    oper_basic           = pst_set_physical_port->oper_basic;
    oper_physical        = pst_set_physical_port->oper_physical;
    oper_pos             = pst_set_physical_port->oper_pos_link;    
    oper_sdh_sovhd       = pst_set_physical_port->oper_sdh_soverhead;
    oper_ces             = pst_set_physical_port->oper_ces;
    oper_cep             = pst_set_physical_port->oper_cep_overhead;
    oper_efm_oam         = pst_set_physical_port->oper_efm_oam;
    oper_time_sync       = pst_set_physical_port->oper_time_sync;
    oper_msg_suppress    = pst_set_physical_port->oper_msg_suppr;
    oper_flow_shape      = pst_set_physical_port->oper_flow_shape;
    oper_schedule_policy = pst_set_physical_port->oper_schedul_policy;
    oper_congest_policy  = pst_set_physical_port->oper_congest_policy;
    oper_ipv4_nf         = pst_set_physical_port->oper_ipv4nf;
    oper_ip_sample       = pst_set_physical_port->oper_ipnf_sampler;
    oper_ipv6_nf         = pst_set_physical_port->oper_ipv6nf;    
    oper_pove            = pst_set_physical_port->oper_pove; 
    oper_common          = pst_set_physical_port->oper_common;
    oper_sdh_tovhd       = pst_set_physical_port->oper_sdh_toverhead;   
    oper_als             = pst_set_physical_port->oper_als;    
    oper_thres           = pst_set_physical_port->oper_thres;
    oper_mcc             = pst_set_physical_port->oper_mcc;

    /* 删除整个条目 */
    if (pst_set_physical_port->delete_struct == TRUE)
    {
        /* 如果条目不存在，不删除 */
        if (ATG_DCI_UNEXIST == if_exist)
        {
            pst_set_physical_port->return_code = ATG_DCI_RC_OK;
            ret = PHY_PORT_PROC_DEL_NULL_LOCAL_CFG;    
            spm_l2_record_err_code(ret);       
            goto EXIT_LABEL;
        }              
        else/* 存在删除 */
        {
            spm_dbg_print_physical_port_head(port_index, SPM_OPER_DEL);
            spm_dbg_record_physical_port_head(port_index, SPM_OPER_DEL);
            
            //调用相关驱动，删除物理接口配置
            spm_physical_port_del_cfg(pst_physical_port NBB_CCXT);
            
            //to do 通知仿真盘删除配置
            /**********后续讨论具体怎么处理************************/
            
            if (NULL != pst_physical_port)
            {
                AVLL_DELETE(SHARED.physical_port_tree, pst_physical_port->spm_physical_port_node); 

                //释放接口物理配置节点的内存空间
                spm_free_physical_port_cb(pst_physical_port);
            }
        }

        /*记录删除配置结束时间*/
        //gettimeofday(&phy_tv_end, NULL);

        /*记录开始处理配置时间*/
        //OS_SPRINTF(print_message, "time_test phy_port_proc delete end = %ld\n", phy_tv_end.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message); 
        //OS_SPRINTF(print_message, "time_test phy_interval = %ld\n", phy_tv_end.tv_usec - phy_tv_begin.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);
        
    }
    else  //更新或者增加
    {   
        if (ATG_DCI_UNEXIST == if_exist)
        {
            spm_dbg_print_physical_port_head(port_index, SPM_OPER_ADD);
            spm_dbg_record_physical_port_head(port_index, SPM_OPER_ADD);
            
            pst_physical_port = spm_alloc_physical_port_cb(NBB_CXT);

            if(NULL == pst_physical_port)
            {
                ret = PHY_PORT_PROC_ALLOC_CB_FAIL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            if (NULL != pst_physical_port)
            {
                //如果是新增，直接赋key值，然后插入到tree中
                pst_physical_port->port_index_key = port_index;
                avll_ret_code = AVLL_INSERT(SHARED.physical_port_tree, pst_physical_port->spm_physical_port_node);
                NBB_ASSERT(avll_ret_code);
            }
        }
        else
        {
            //更新 
            spm_dbg_print_physical_port_head(port_index, SPM_OPER_UPD);
            spm_dbg_record_physical_port_head(port_index, SPM_OPER_UPD);
            
        }
        
        /********************************1)基本配置 *********************************/
        if(ATG_DCI_OPER_NULL != oper_basic)
        {
            spm_physical_port_basic_proc(pst_set_physical_port,pst_physical_port,oper_basic);
        }
        
        /******************************2)以太网接口物理配置 ***************************/
        if(ATG_DCI_OPER_NULL != oper_physical)
        {
            spm_physical_port_eth_phy_proc(pst_set_physical_port,pst_physical_port,oper_physical);
        }
        
        /********************************3)POS接口链路层配置 *************************/
        if(ATG_DCI_OPER_NULL != oper_pos)
        {
            spm_physical_port_pos_proc(pst_set_physical_port,pst_physical_port,oper_pos);
        }
        
        /******************************** 19)SDH通道开销配置 **************************/        
        if(ATG_DCI_OPER_NULL != oper_sdh_tovhd)
        {
            spm_physical_port_sdh_tovhd_proc(pst_set_physical_port,pst_physical_port,oper_sdh_tovhd);
        }
        
        /********************************5)CES接口配置 ******************************/
        if(ATG_DCI_OPER_NULL != oper_ces)
        {
            spm_physical_port_ces_proc(pst_set_physical_port,pst_physical_port,oper_ces);
        }
        
        /********************************6)CEP接口开销配置 ***************************/
        if(ATG_DCI_OPER_NULL != oper_cep)
        {
            spm_physical_port_cep_proc(pst_set_physical_port,pst_physical_port,oper_cep);
        }

        /********************************7)以太网EFM OAM ****************************/
        if(ATG_DCI_OPER_NULL != oper_efm_oam)
        {
            spm_physical_port_efm_oam_proc(pst_set_physical_port,pst_physical_port,oper_efm_oam);
        }
        
        /********************************8)时间同步端口配置 ****************************/
        if(ATG_DCI_OPER_NULL != oper_time_sync)
        {
            spm_physical_port_time_sync_proc(pst_set_physical_port,pst_physical_port,oper_time_sync);
        }
        
        /********************************9)以太网接口报文抑制 ***************************/
        if(ATG_DCI_OPER_NULL != oper_msg_suppress)
        {
            spm_physical_port_msg_suppress_proc(pst_set_physical_port,pst_physical_port,oper_msg_suppress);
        }
        
        /********************************10)以太网接口流量整形 **************************/
        if(ATG_DCI_OPER_NULL != oper_flow_shape)
        {
            spm_physical_port_flow_shape_proc(pst_set_physical_port,pst_physical_port,oper_flow_shape);
        }
        
        /********************************11)以太网出口队列调度策略 ***********************/
        if(ATG_DCI_OPER_NULL != oper_schedule_policy)
        {
            spm_physical_port_schedule_policy_proc(pst_set_physical_port,pst_physical_port,
                oper_schedule_policy);
        }
        
        /********************************12)以太网出口队列拥塞策略 ************************/
        if(ATG_DCI_OPER_NULL != oper_congest_policy)
        {
            spm_physical_port_congest_policy_proc(pst_set_physical_port,pst_physical_port,oper_congest_policy);
        }
        
        /********************************13)IPV4流采样使能配置IPV4 NetFlow ****************/
        if(ATG_DCI_OPER_NULL != oper_ipv4_nf)
        {
            spm_physical_port_ipv4_netflow_proc(pst_set_physical_port,pst_physical_port,oper_ipv4_nf);
        }
        
        /********************************14)IPV4流采样镜像 IPV4 NetFlow sampler ************/
        if(ATG_DCI_OPER_NULL != oper_ip_sample)
        {
            spm_physical_port_ip_nf_sample_proc(pst_set_physical_port,pst_physical_port,oper_ip_sample);
        }
        
        /********************************15)IPV6流采样使能配置IPV6 NetFlow *****************/
        if(ATG_DCI_OPER_NULL != oper_ipv6_nf)
        {
            spm_physical_port_ipv6_netflow_proc(pst_set_physical_port,pst_physical_port,oper_ipv6_nf);
        }
        
        /********************************17)POVE配置 ***********************************/
        if(ATG_DCI_OPER_NULL != oper_pove)
        {
            spm_physical_port_pve_proc(pst_set_physical_port,pst_physical_port,oper_pove);
        }
        
        /********************************21)端口发送接收光功率门限值配置 *********************/
        if(ATG_DCI_OPER_NULL != oper_thres)
        {
            spm_physical_port_thres_proc(pst_set_physical_port,pst_physical_port,oper_thres);
        }
        
        /********************************18)接口通用配置 *********************************/
        if(ATG_DCI_OPER_NULL != oper_common)
        {
            spm_physical_port_common_proc(pst_set_physical_port,pst_physical_port,oper_common);
        }
        
        /********************************20)ALS配置 ************************************/
        if(ATG_DCI_OPER_NULL != oper_als)
        {
            spm_physical_port_als_proc(pst_set_physical_port,pst_physical_port,oper_als);
        }
        
        /********************************4)SDH段开销配置 ********************************/
        if(ATG_DCI_OPER_NULL != oper_sdh_sovhd)
        {
            spm_physical_port_sdh_sovhd_proc(pst_set_physical_port,pst_physical_port,oper_sdh_sovhd);
        }
        
        /********************************22)MCC通道类型配置 *****************************/
        if(ATG_DCI_OPER_NULL != oper_mcc)
        {
            spm_physical_port_mcc_type_proc(pst_set_physical_port,pst_physical_port,oper_mcc);
        }

        /*记录删除配置结束时间*/
        //gettimeofday(&phy_tv_end, NULL);

        /*记录开始处理配置时间*/
        //OS_SPRINTF(print_message, "time_test phy_port_proc add end = %ld\n", phy_tv_end.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);
        //OS_SPRINTF(print_message, "time_test phy_interval = %ld\n", phy_tv_end.tv_usec - phy_tv_begin.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);
        
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}

#endif

