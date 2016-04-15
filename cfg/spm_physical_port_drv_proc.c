/******************************************************************************

              版权所有 (C), 烽火通信科技股份有限公司

******************************************************************************

文 件 名   : spm_physical_port_dev.c
版 本 号   : 初稿
作    者   : zhangzhm
生成日期   : 2015年8月10日
最近修改   :
功能描述   :封装物理接口配置块相关的驱动接口
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

/*物理接口配置打印开关*/
extern unsigned char g_physical_port_cfg_print_setting;//开关

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_basic_cfg
 功能描述  : 适配层处理"1)基本配置 
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
NBB_INT spm_physical_port_drv_add_basic_cfg(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_BASIC_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SPM_PORT_STRUCT st_port;
    
    /*    
    SPM_PHYSICAL_PORT_INFO_CB pst_phy_port_info;

    NBB_ULONG port_index_key = 0;
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_basic_cfg");

    if (NULL == pst_physical_port)
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

    OS_MEMSET(&pst_phy_port_info,0,sizeof(SPM_PHYSICAL_PORT_INFO_CB));
    OS_MEMSET(&st_port,0,sizeof(SPM_PORT_STRUCT));
    pst_physical_port->basic_success_flag = SUCCESS;      
    port_index_key = pst_physical_port->port_index_key;
    func_ret = spm_physical_port_get_phy_info_from_index(port_index_key,&pst_phy_port_info);
    
    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
        goto EXIT_LABEL;
    }

    pst_physical_port->slot_id = pst_phy_port_info.slot_id;  
    func_ret = spm_hardw_getc3lineport(pst_phy_port_info.card_id,pst_phy_port_info.phy_port_id,
        pst_tlv_cfg->circuit_emu_mode,&st_port);

    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
        goto EXIT_LABEL;
    }
    
    pst_physical_port->unit_id = st_port.ucUnit;
    pst_physical_port->port_id = st_port.ucPort; 
    pst_phy_port_info.unit_id = st_port.ucUnit;
    pst_phy_port_info.port_id = st_port.ucPort;     
    NBB_MEMCPY(&pst_physical_port->phy_port_info_cb,&pst_phy_port_info,sizeof(SPM_PHYSICAL_PORT_INFO_CB));    

#ifdef SPU

    if (SHARED.local_slot_id == pst_phy_port_info.slot_id)
    {        
        //如果物理接口为仿真口，则不配置端口属性，在子卡初始化完成后liujuan配置
        if ((ATG_DCI_ETH == pst_phy_port_info.phy_port_type) 
            && (ATG_DCI_DERECT == pst_tlv_cfg->port_attrib))
        {
            func_ret = ApiC3SetPortType(st_port.ucUnit, st_port.ucPort, ETH);
            
            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                pst_physical_port->basic_success_flag = SPM_CALL_SPM_FUNTION_ERROR;
                spm_l2_record_err_code_and_one_value(ret,func_ret);
                goto EXIT_LABEL;
            }            
        } 
    }
#endif 
    */
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_basic_cfg");

    if (NULL == pst_physical_port)
    {
        ret = PHY_PORT_BASIC_DRV_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = PHY_PORT_BASIC_DRV_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    OS_MEMSET(&st_port,0,sizeof(SPM_PORT_STRUCT));
    pst_physical_port->basic_success_flag = SUCCESS;       
    func_ret = spm_hardw_getc3lineport(pst_tlv_cfg->sub_board,pst_tlv_cfg->port,
        pst_tlv_cfg->circuit_emu_mode,&st_port);

    if(SUCCESS != func_ret)
    {
        ret = PHY_PORT_BASIC_DRV_PROC_GET_DRV_LINE_PORT_FAIL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
        goto EXIT_LABEL;
    }
    
    pst_physical_port->slot_id = pst_tlv_cfg->slot;     
    pst_physical_port->unit_id = st_port.ucUnit;
    pst_physical_port->port_id = st_port.ucPort; 
    pst_physical_port->phy_port_info_cb.slot_id = pst_tlv_cfg->slot; 
    pst_physical_port->phy_port_info_cb.card_id = pst_tlv_cfg->sub_board;     
    pst_physical_port->phy_port_info_cb.phy_port_id = pst_tlv_cfg->port; 
    pst_physical_port->phy_port_info_cb.phy_port_type = pst_tlv_cfg->port_type;     
    pst_physical_port->phy_port_info_cb.unit_id = st_port.ucUnit;
    pst_physical_port->phy_port_info_cb.port_id = st_port.ucPort;      
    
#ifdef SPU

    if (SHARED.local_slot_id == pst_tlv_cfg->slot)
    {        
        //如果物理接口为仿真口，则不配置端口属性，在子卡初始化完成后liujuan配置
        if ((ATG_DCI_ETH == pst_tlv_cfg->port_type) 
            && (ATG_DCI_DERECT == pst_tlv_cfg->port_attrib))
        {
            func_ret = ApiC3SetPortType(st_port.ucUnit, st_port.ucPort, ETH);
            
            if(SUCCESS != func_ret)
            {
                ret = PHY_PORT_BASIC_DRV_PROC_DRV_SET_PORT_TYPE_FAIL;
                pst_physical_port->basic_success_flag = 1;//调驱动失败
                spm_l2_record_err_code_and_one_value(ret,func_ret);
                goto EXIT_LABEL;
            }            
        } 
    }
#endif 
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_eth_phy_cfg
 功能描述  : 适配层处理"2)物理接口子tlv--  "2）以太网接口物理配置"  
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
NBB_INT spm_physical_port_drv_add_eth_phy_cfg(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_PHY_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    NBB_BYTE unit_id = 0;
    NBB_BYTE port_id = 0;  
    NBB_BYTE mac_check = 0;
    NBB_BYTE smac[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    NBB_BYTE smac_temp[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_eth_phy_cfg");

    if (NULL == pst_physical_port)
    {
        ret = PHY_PORT_ETH_PHY_DRV_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = PHY_PORT_ETH_PHY_DRV_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = PHY_PORT_ETH_PHY_DRV_PROC_CFG_BASIC_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = PHY_PORT_ETH_PHY_DRV_PROC_CFG_BASIC_EXE_FAIL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    unit_id = pst_physical_port->unit_id;        
    port_id = pst_physical_port->port_id;
    pst_physical_port->phy_port_info_cb.phy_port_mac[0] = pst_tlv_cfg->phy_addr[8];
    pst_physical_port->phy_port_info_cb.phy_port_mac[1] = pst_tlv_cfg->phy_addr[9];
    pst_physical_port->phy_port_info_cb.phy_port_mac[2] = pst_tlv_cfg->phy_addr[10];
    pst_physical_port->phy_port_info_cb.phy_port_mac[3] = pst_tlv_cfg->phy_addr[11];
    pst_physical_port->phy_port_info_cb.phy_port_mac[4] = pst_tlv_cfg->phy_addr[12];
    pst_physical_port->phy_port_info_cb.phy_port_mac[5] = pst_tlv_cfg->phy_addr[13];
    
#ifdef SPU

    //只有是Eth网类型的口才进入端口配置
    if ((SHARED.local_slot_id == pst_physical_port->slot_id)
        && (ATG_DCI_ETH == pst_physical_port->phy_port_info_cb.phy_port_type))
    {        
        /* 端口开关：0/1=关/开 */
        func_ret = ApiC3SetPortState(unit_id, port_id,pst_tlv_cfg->port_switch);
                  
        if(SUCCESS != func_ret)
        {
            ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV_SET_PORT_STATE_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        }    
       
        /* 带宽：0-1000-100000 单位：1Mbps */
        func_ret = SUCCESS;
        func_ret = ApiAradSetOFPBandWidth(UNIT_0,port_id,1000 * pst_tlv_cfg->bandwidth);
                    
        if(SUCCESS != func_ret)
        {
            ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV2_SET_BAND_WIDTH_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        }
        
        /* 端口流控开关: 0/1=关/开*/
        func_ret = SUCCESS;
        func_ret = ApiC3SetPortPause(unit_id, port_id, pst_tlv_cfg->flow_control, pst_tlv_cfg->flow_control);
        
        if(SUCCESS != func_ret)
        {
            ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV_SET_PORT_PAUSE_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        }
        
        /* 端口TP模式：0/1=否/是 */
        func_ret = SUCCESS;
        mac_check = (!pst_tlv_cfg->port_tp_mode);
        func_ret = ApiC3SetPortMacCheck(unit_id, port_id,mac_check);

        if(SUCCESS != func_ret)
        {
            ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV_SET_PORT_MAC_CHECK_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        } 
        
        /* STPID */
        //如果tpid配置下来的值不合法，强行配置成0x8100
        func_ret = SUCCESS;

        if (pst_tlv_cfg->stpid > 0x600)
        {
            func_ret = ApiC3SetPortTpid(unit_id,port_id,pst_tlv_cfg->stpid);
        }
        else
        {
            func_ret = ApiC3SetPortTpid(unit_id,port_id,0x8100);
        } 
        
        if(SUCCESS != func_ret)
        {
            ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV_SET_PORT_TPID_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        } 
        
        /* MTU：1500-10240 */
        //如果主控下发的MTU值为0，进行容错
        func_ret = SUCCESS;

        if(0 == pst_tlv_cfg->mtu)
        {
            func_ret = ApiC3SetPortRxMaxSize(unit_id,port_id,2000, 1);
        }
        else
        {
            func_ret = ApiC3SetPortRxMaxSize(unit_id,port_id,pst_tlv_cfg->mtu, 1);
        } 
        
        if(SUCCESS != func_ret)
        {
            ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV_SET_RX_MAX_SIZE_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        }  
        
        /* 设置端口mac */
        smac[0] = pst_tlv_cfg->phy_addr[8];
        smac[1] = pst_tlv_cfg->phy_addr[9];
        smac[2] = pst_tlv_cfg->phy_addr[10];
        smac[3] = pst_tlv_cfg->phy_addr[11];
        smac[4] = pst_tlv_cfg->phy_addr[12];
        smac[5] = pst_tlv_cfg->phy_addr[13];
        ret = SUCCESS;
        ret = NBB_MEMCMP(smac,smac_temp,6);

        if((0 != ret) && (1 != (smac[0] & 0x1)))
        {
            func_ret = SUCCESS;
            func_ret = ApiC3SetPortMac(unit_id, ATG_DCI_IS_NOT_VE, port_id, smac);

            if(SUCCESS != func_ret)
            {
                ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV_SET_PORT_MAC_FAIL;
                spm_l2_record_err_code_and_one_value(ret,func_ret);
            }                                                
        } 
                    
        //****工作模式：0/1=非自协商/自协商****
        func_ret = SUCCESS;
        
        if ((ATG_DCI_PORT_RATE_GE == pst_tlv_cfg->port_type)
            || (ATG_DCI_PORT_RATE_FE == pst_tlv_cfg->port_type))
        {
            //只有GE FE 口才会设置协商模式,其他类型的端口直接设置为全双工
            func_ret = ApiC3SetPortAutoNego(unit_id, port_id,pst_tlv_cfg->work_mode);
            
            if(SUCCESS != func_ret)
            {
                ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV_SET_PORT_AUTO_NEGO_FAIL;
                spm_l2_record_err_code_and_one_value(ret,func_ret);
            }
        }
        else
        {
            //func_ret = ApiC3SetPortDuplex(unit_id,port_id,ATG_DCI_FULL_DUPLEX);
            
            if(SUCCESS != func_ret)
            {
                ret = PHY_PORT_ETH_PHY_DRV_PROC_DRV_SET_PORT_DUPLEX_FAIL;
                spm_l2_record_err_code_and_one_value(ret,func_ret);
            }            
        }   
        
        /* 10GE端口模式：0/1=10GE LAN/10GE WAN .  */
        func_ret = SUCCESS;
        func_ret = spm_subcard_setxtcbportproc(pst_physical_port->phy_port_info_cb.card_id,
            pst_physical_port->phy_port_info_cb.phy_port_id,pst_tlv_cfg->xge_port_mode);

        if(SUCCESS != func_ret)
        {
            ret = PHY_PORT_ETH_PHY_DRV_PROC_HW_SET_XTCB_PORT_MODE_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        }            
    } 
#endif
  
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_time_sync
功能描述  : 物理接口子tlv--8)时间同步端口配置 
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
NBB_INT spm_physical_port_drv_add_time_sync(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_TIME_SYNC_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_time_sync");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
#ifdef SRC
    func_ret = spm_ptp_set_port(pst_physical_port->slot_id, 
        pst_physical_port->phy_port_info_cb.card_id,
        pst_physical_port->phy_port_info_cb.phy_port_id,
        pst_physical_port->port_id,
        pst_tlv_cfg);

    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
    } 
    
#endif                        
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_eth_msg_suppr
 功能描述  : 物理接口子tlv--9）以太网接口报文抑制
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
NBB_INT spm_physical_port_drv_add_eth_msg_suppr(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SUB_PORT sub_port;  
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_eth_msg_suppr");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&sub_port, 0, sizeof(SUB_PORT));

#ifdef SPU   
    sub_port.port = pst_physical_port->port_id;
    sub_port.slot = pst_physical_port->slot_id;
    sub_port.unit = pst_physical_port->unit_id;
    
    if (SHARED.local_slot_id == pst_physical_port->slot_id)
    {
        func_ret = spm_set_port_flood_control(&sub_port, 
           pst_tlv_cfg->bc_suppr_switch,
           pst_tlv_cfg->mc_suppr_switch,
           pst_tlv_cfg->fd_suppr_switch, 
           pst_tlv_cfg->bc_suppr_flow,
           pst_tlv_cfg->mc_suppr_flow,
           pst_tlv_cfg->fd_suppr_flow);

        if(SUCCESS != func_ret)
        {
            ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        }     
    }

#endif     
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_eth_flow_shape
 功能描述  : 物理接口子tlv--10）以太网接口流量整形
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
NBB_INT spm_physical_port_drv_add_eth_flow_shape(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_FLOW_SHAPE *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SUB_PORT sub_port;  
    NBB_BYTE loop = 0;
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_eth_flow_shape");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&sub_port, 0, sizeof(SUB_PORT));

#ifdef SPU   
    sub_port.port = pst_physical_port->port_id;
    sub_port.slot = pst_physical_port->slot_id;
    sub_port.unit = pst_physical_port->unit_id;
    
    if (SHARED.local_slot_id == pst_physical_port->slot_id)
    {
        for (loop = 0; loop < ATG_DCI_QOS_BEHAVIOR_SHAPE_NUM; loop++)
        {
            spm_set_port_cir_pir(&sub_port, loop, &pst_tlv_cfg->flow_shape_queue[loop]);
        }       
    }

#endif     
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_eth_schedul_policy
 功能描述  : 物理接口子tlv--11）以太网出口队列调度策略
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
NBB_INT spm_physical_port_drv_add_eth_schedul_policy(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_SCHEDUL_POLICY *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SUB_PORT sub_port;  
    NBB_BYTE loop = 0;
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_eth_schedul_policy");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&sub_port, 0, sizeof(SUB_PORT));

#ifdef SPU   
    sub_port.port = pst_physical_port->port_id;
    sub_port.slot = pst_physical_port->slot_id;
    sub_port.unit = pst_physical_port->unit_id;
    
    if (SHARED.local_slot_id == pst_physical_port->slot_id)
    {
        for (loop = 0; loop < ATG_DCI_QOS_BEHAVIOR_SCHEDL_NUM; loop++)
        {
            spm_set_port_sp_wfq(&sub_port,loop,&pst_tlv_cfg->schedule_queue[loop]);
        }      
    }

#endif     
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_eth_congest_policy
 功能描述  : 物理接口子tlv--12）以太网出口队列拥塞策略
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
NBB_INT spm_physical_port_drv_add_eth_congest_policy(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_CONGEST_POLICY *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SUB_PORT sub_port;  
    SPM_PORT_WRED_KEY wred_key;   
    NBB_BYTE loop = 0;
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_eth_congest_policy");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&sub_port, 0, sizeof(SUB_PORT));
    OS_MEMSET(&wred_key, 0, sizeof(SPM_PORT_WRED_KEY));
    
#ifdef SPU   
    sub_port.port = pst_physical_port->port_id;
    sub_port.slot = pst_physical_port->slot_id;
    sub_port.unit = pst_physical_port->unit_id;
    wred_key.index = pst_physical_port->port_index_key;
    
    /* 配置处理，每个槽位都建 */    
    for (loop = 0; loop < ATG_DCI_QOS_BEHAVIOR_CONGST_NUM; loop++)
    {
        wred_key.cos = loop;
        
        //spm_set_port_wred(&wred_key,&sub_port,&pst_tlv_cfg->congest_queue[loop]);
    }      

#endif     
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_ipv4_nf_enable
 功能描述  : 物理接口子tlv--13）IPV4流采样使能配置IPV4 NetFlow
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
NBB_INT spm_physical_port_drv_add_ipv4_nf_enable(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_IPV4NF_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SUB_PORT sub_port;  
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_ipv4_nf_enable");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&sub_port, 0, sizeof(SUB_PORT));
    
#ifdef SPU   
    sub_port.port = pst_physical_port->port_id;
    sub_port.slot = pst_physical_port->slot_id;
    sub_port.unit = pst_physical_port->unit_id;
    func_ret = spm_set_ipv4_nf_enable(&sub_port, pst_tlv_cfg);
    
    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
    }     

#endif   
                      
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_ip_nf_smapler
 功能描述  : 物理接口子tlv--14）IP流采样镜像
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
NBB_INT spm_physical_port_drv_add_ip_nf_smapler(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_IPNF_SAMPLER *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SUB_PORT sub_port;  
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_ip_nf_smapler");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&sub_port, 0, sizeof(SUB_PORT));
    
#ifdef SPU   
    sub_port.port = pst_physical_port->port_id;
    sub_port.slot = pst_physical_port->slot_id;
    sub_port.unit = pst_physical_port->unit_id;
    func_ret = spm_set_ipv4_sampler(&sub_port, pst_tlv_cfg);
    
    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
    }     

#endif   
                      
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}

/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_ipv6_nf_enable
 功能描述  : 物理接口子tlv--15）IPV6流采样使能配置IPV6 NetFlow 
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
NBB_INT spm_physical_port_drv_add_ipv6_nf_enable(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_IPV6NF_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SUB_PORT sub_port;  
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_ipv6_nf_enable");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&sub_port, 0, sizeof(SUB_PORT));
    
#ifdef SPU   
    sub_port.port = pst_physical_port->port_id;
    sub_port.slot = pst_physical_port->slot_id;
    sub_port.unit = pst_physical_port->unit_id;
    func_ret = spm_set_ipv6_nf_enable(&sub_port, pst_tlv_cfg);
    
    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
    }     

#endif   
                      
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}


/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_pove
 功能描述  : 物理接口子tlv--17）POVE
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
NBB_INT spm_physical_port_drv_add_pove(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_POVE *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_pove");

    if (NULL == pst_physical_port)
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

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_common_proc
 功能描述  : 物理接口子tlv--18）接口通用配置
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
NBB_INT spm_physical_port_drv_add_common_proc(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_COMMON_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_common_proc");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
#ifdef SPU

    //只有是Eth网类型的口才进入端口配置
    if ((SHARED.local_slot_id == pst_physical_port->slot_id)
        && (ATG_DCI_ETH == pst_physical_port->phy_port_info_cb.phy_port_type))
    {
        func_ret = spm_hardw_setupholdtime(pst_physical_port->phy_port_info_cb.card_id,
            pst_physical_port->phy_port_info_cb.phy_port_id,  
            pst_tlv_cfg->up_time);

        if(SUCCESS != func_ret)
        {
            ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
            spm_l2_record_err_code_and_three_value(ret,
                pst_physical_port->phy_port_info_cb.card_id,
                pst_physical_port->phy_port_info_cb.phy_port_id,
                func_ret);
        }
        
        func_ret = SUCCESS;
        func_ret = spm_hardw_setdownholdtime(pst_physical_port->phy_port_info_cb.card_id,
            pst_physical_port->phy_port_info_cb.phy_port_id, 
            pst_tlv_cfg->down_time); 
        
        if(SUCCESS != func_ret)
        {
            ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
            spm_l2_record_err_code_and_three_value(ret,
                pst_physical_port->phy_port_info_cb.card_id,
                pst_physical_port->phy_port_info_cb.phy_port_id,
                func_ret);
        }
    }
   
#endif  

    //只有是Eth网类型的口才进入端口配置
    if (ATG_DCI_ETH == pst_physical_port->phy_port_info_cb.phy_port_type)
    {        
        func_ret = SUCCESS;
        func_ret = spm_bfd_update_common_from_physical_port_index(pst_physical_port->port_index_key);
        
        if(SUCCESS != func_ret)
        {
            ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
            spm_l2_record_err_code_and_three_value(ret,
                pst_physical_port->phy_port_info_cb.card_id,
                pst_physical_port->phy_port_info_cb.phy_port_id,
                func_ret);
        }
    }
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_als_cfg
 功能描述  : 物理接口子tlv--20)ALS配置 
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
NBB_INT spm_physical_port_drv_add_als_cfg(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_ALS_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    ALS_CONFIG_INFO als_conf; 
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_als_cfg");

    if (NULL == pst_physical_port)
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
    
    if (NULL == pst_physical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (SUCCESS != pst_physical_port->basic_success_flag)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&als_conf, 0, sizeof(ALS_CONFIG_INFO));
    
#ifdef SPU

    //只有本槽且为母卡
    if ((SHARED.local_slot_id == pst_physical_port->slot_id)
        && (0 == pst_physical_port->phy_port_info_cb.card_id))
    {
        als_conf.als_en = pst_tlv_cfg->als_enable;
        als_conf.revsiglos_time= pst_tlv_cfg->rev_lost_time;
        als_conf.waiting_time= pst_tlv_cfg->laser_delay_time;
        als_conf.sendsig_time= pst_tlv_cfg->send_time;
        func_ret = Als_Config(pst_physical_port->port_id, als_conf);

        if(SUCCESS != func_ret)
        {
            ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
            spm_l2_record_err_code_and_three_value(ret,
                pst_physical_port->phy_port_info_cb.card_id,
                pst_physical_port->phy_port_info_cb.phy_port_id,
                func_ret);
        }
    }
   
#endif  

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
 函 数 名  : spm_physical_port_drv_add_mcc_cfg
 功能描述  : 物理接口子tlv--22）MCC通道类型  
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
NBB_INT spm_physical_port_drv_add_mcc_cfg(
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    ATG_DCI_PHY_PORT_MCC *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_physical_port_drv_add_mcc_cfg");

    if (NULL == pst_physical_port)
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

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

#endif