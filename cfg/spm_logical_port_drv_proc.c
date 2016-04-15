/******************************************************************************

                  版权所有 (C), 烽火通信科技股份有限公司

******************************************************************************

  文 件 名   : spm_logical_port_drv_proc.c
  版 本 号   : 初稿
  作    者   : lish
  生成日期   : 2015年8月15日
  最近修改   :
  功能描述   : 端口逻辑配置的驱动等相关处理函数
  函数列表   :
  修改历史   :
  1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 创建文件

******************************************************************************/

#include <nbase.h>
#include <spmincl.h>
extern unsigned char g_logical_port_cfg_print_setting;

/* 逻辑口生成全局唯一索引，资源规格依驱动决定 (hdjia) */
//NBB_ULONG g_logical_port_cvt_id[SPM_LOGICAL_PORT_DRV_NUM] = {0};
/*****************************************************************************
函 数 名  : spm_logical_port_get_port_info_from_index
功能描述  : 逻辑接口子tlv--基本配置，处理函数 1)
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
//暂时从物理接口信息里获取，等主控最新的port_index解析方案确定后，直接从逻辑接口的port_index里获取
//fansongbo 2016.2.25
//需要同时把spm_logical_port_apply_set_basic_data里注释反一下
/*
NBB_INT spm_logical_port_get_port_info_from_index(NBB_ULONG port_index,
    SPM_LOGICAL_PORT_INFO_CB *pst_logic_port_info)
{
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
    SPM_PORT_INFO_CB st_port_info;
    NBB_BYTE  ucLagSlotId = 0;
    NBB_USHORT usLagPortId = 0;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_logical_port_get_port_info_from_index");

    if(NULL == pst_logic_port_info)
    {
        ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_port_info, 0, sizeof(SPM_PORT_INFO_CB));

    pst_logical_port = AVLL_FIND(v_spm_shared->logical_port_tree, &port_index);

    //逻辑端口不存在
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_BASIC_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    //只有端口属性为子接口且接口index不为0的逻辑端口才能获取到PortId
    if (((ATG_DCI_SUB_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type) 
        || (ATG_DCI_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type))
        && (0 != pst_logical_port->phy_cfg_cb->phy_port_index))
    {
        func_ret = spm_get_portid_from_physical_port_index(pst_logical_port->phy_cfg_cb->phy_port_index,                                                                                                                 
            &st_port_info);

        if (SUCCESS == func_ret)
        {
            pst_logic_port_info->port_id = st_port_info.port_id;
            pst_logic_port_info->slot_id = st_port_info.slot_id; 
            pst_logic_port_info->card_id = st_port_info.card_id; 
            pst_logic_port_info->unit_id = st_port_info.unit_id;
            pst_logic_port_info->phy_port_type = st_port_info.phy_port_type;
            pst_logic_port_info->phy_port_id = st_port_info.phy_port;
        }
        else
        {
            ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_CALL_FUNC_FAIL;    
            spm_l2_record_err_code(ret);       
            goto EXIT_LABEL;
        }
    }
    else if ((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
        && (0 != pst_logical_port->phy_cfg_cb->lag_id))
    {
        spm_get_lag_min_slot_port(pst_logical_port->phy_cfg_cb->lag_id, &ucLagSlotId, &usLagPortId NBB_CCXT);       
        pst_logic_port_info->port_id = pst_logical_port->phy_cfg_cb->lag_id + ATG_DCI_LAG_OFFSET;
        pst_logic_port_info->slot_id = 0;
        pst_logic_port_info->unit_id = UNIT_0;  //待处理//unit?
        pst_logic_port_info->port_type = ATG_DCI_LAG;
        pst_logic_port_info->lag_slot_id = ucLagSlotId;
        pst_logic_port_info->lag_port_id = usLagPortId;
    }
    else if ((ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type) 
        && (0 != pst_logical_port->phy_cfg_cb->ve_group_id))
    {
        pst_logic_port_info->port_id = pst_logical_port->phy_cfg_cb->ve_group_id;
        pst_logic_port_info->slot_id = pst_logical_port->phy_cfg_cb->ve_slot;
        pst_logic_port_info->unit_id = UNIT_0;  //待处理//unit?
        pst_logic_port_info->port_type = ATG_DCI_VE_PORT;
        pst_logic_port_info->ve_slot_id = pst_logical_port->phy_cfg_cb->ve_slot;

    }

    //pst_logic_port_info->svlan = pst_logical_port->basic_cfg_cb->control_vid;
    pst_logic_port_info->port_type = pst_logical_port->basic_cfg_cb->port_sub_type;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}
*/

/*****************************************************************************
函 数 名  : spm_logical_port_get_port_info_from_index
功能描述  : 逻辑接口子tlv--基本配置，处理函数 1)
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
//暂时从物理接口信息里获取，等主控最新的port_index解析方案确定后，直接从逻辑接口的port_index里获取
//fansongbo 2016.2.25
//需要同时把spm_logical_port_apply_set_basic_data里注释反一下
NBB_INT spm_logical_port_get_port_info_from_index(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_PHY_DATA *pst_tlv_cfg,
    SPM_LOGICAL_PORT_INFO_CB *pst_logic_port_info)
{
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    SPM_PORT_INFO_CB st_port_info;
    NBB_BYTE  ucLagSlotId = 0;
    NBB_USHORT usLagPortId = 0;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_logical_port_get_port_info_from_index");

    if(NULL == pst_logical_port)
    {
        ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_tlv_cfg)
    {
        ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_logic_port_info)
    {
        ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_BASIC_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_port_info, 0, sizeof(SPM_PORT_INFO_CB));
    
    //只有端口属性为子接口且接口index不为0的逻辑端口才能获取到PortId
    if (((ATG_DCI_SUB_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type) 
        || (ATG_DCI_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type))
        && (0 != pst_tlv_cfg->phy_port_index))
    {
        func_ret = spm_get_portid_from_physical_port_index(pst_tlv_cfg->phy_port_index,                                                                                                                 
            &st_port_info);

        if (SUCCESS == func_ret)
        {
            pst_logic_port_info->port_id = st_port_info.port_id;
            pst_logic_port_info->slot_id = st_port_info.slot_id; 
            pst_logic_port_info->card_id = st_port_info.card_id; 
            pst_logic_port_info->unit_id = st_port_info.unit_id;
            pst_logic_port_info->phy_port_type = st_port_info.phy_port_type;
            pst_logic_port_info->phy_port_id = st_port_info.phy_port;
        }
        else
        {
            ret = LOG_PORT_GET_PORT_INFO_FROM_INDEX_CALL_FUNC_FAIL;    
            spm_l2_record_err_code(ret);       
            goto EXIT_LABEL;
        }
    }
    else if ((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
        && (0 != pst_tlv_cfg->lag_id))
    {
        spm_get_lag_min_slot_port(pst_tlv_cfg->lag_id, &ucLagSlotId, &usLagPortId NBB_CCXT);       
        pst_logic_port_info->port_id = pst_tlv_cfg->lag_id + ATG_DCI_LAG_OFFSET;
        pst_logic_port_info->slot_id = 0;
        pst_logic_port_info->unit_id = UNIT_0;  //待处理//unit?
        pst_logic_port_info->lag_slot_id = ucLagSlotId;
        pst_logic_port_info->lag_port_id = usLagPortId;
    }
    else if ((ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type) 
        && (0 != pst_tlv_cfg->ve_group_id))
    {
        pst_logic_port_info->port_id = pst_tlv_cfg->ve_group_id;
        pst_logic_port_info->slot_id = pst_tlv_cfg->ve_slot;
        pst_logic_port_info->unit_id = UNIT_0;  //待处理//unit?
        pst_logic_port_info->ve_slot_id = pst_tlv_cfg->ve_slot;

    }

    //pst_logic_port_info->svlan = pst_logical_port->basic_cfg_cb->control_vid;
    pst_logic_port_info->port_type = pst_logical_port->basic_cfg_cb->port_sub_type;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}


#if 0
/*****************************************************************************
   函 数 名  : spm_logical_port_drv_set_intf_ptag
   功能描述  : 设置intf的入口识别动作
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_set_intf_counter(NBB_ULONG intf_pos_id, NBB_INT enable, NBB_INT intf_counter_id)
{
    return 0;
}


/*****************************************************************************
   函 数 名  : spm_logical_port_drv_add_intf
   功能描述  : 增加intf或mod  intf
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数
   2.日    期   : 2015年12月15日
    作    者   : hdjia
    修改内容   : 适配代码开发，该函数只是intf的增加操作，无mod操作

*****************************************************************************/
NBB_INT spm_logical_port_drv_add_l2_intf(NBB_BYTE unit,SPM_INTF_CB *intf_cb)
{
    INTF_T       st_flow; /* INTF的结构体*/
    NBB_INT         ret = SUCCESS;
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_l2_intf");
    
    if (NULL == intf_cb)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_two_value(ret,intf_cb->svlan_id,intf_cb->cvlan_id);     
        goto EXIT_LABEL;
    }

    OS_MEMSET(&st_flow, 0, sizeof(INTF_T));
    
#ifdef SPU  

    /* 配置解耦，8000的驱动接口保持不变*/
    if ((0 == intf_cb->svlan_id) && (0 == intf_cb->cvlan_id))
    {        
        if (ATG_DCI_VE_PORT == intf_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L2_VE_PORT;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT;
        }
    }
    else if ((0 != intf_cb->svlan_id) && (0 == intf_cb->cvlan_id))
    {   
        if (ATG_DCI_VE_PORT == intf_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L2_VE_PORT_OVID;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT_OVID;
        }
    }
    else if ((0 != intf_cb->svlan_id) && (0 != intf_cb->cvlan_id))
    {  
        if (ATG_DCI_VE_PORT == intf_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L2_VE_PORT_OVID_IVID;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT_OVID_IVID;
        }
    }
    else
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_two_value(ret,intf_cb->svlan_id,intf_cb->cvlan_id);     
        goto EXIT_LABEL;      
    }
    
    if (ATG_DCI_TAGGED_VLAN == intf_cb->recv_action)
    {        
        st_flow.flags |= INTF_PTAG_IDENTIFY_ENABLE;
    } 
    
    st_flow.eIntfType = INTF_TYPE_L2;    
    st_flow.mCardPort = intf_cb->port_id;      
    st_flow.bindId = intf_cb->bind_id;
    st_flow.mOvid= intf_cb->svlan_id;
    st_flow.mIvid = intf_cb->cvlan_id;
    
    func_ret = ApiC3AddIntf(unit, &st_flow);

    if (SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code_and_three_value(ret,intf_cb->svlan_id,
            intf_cb->cvlan_id,func_ret);
        goto EXIT_LABEL;
    }
       
    intf_cb->intf_pos_id[unit] = st_flow.posId;
#endif  

    EXIT_LABEL:    NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_drv_add_intf
   功能描述  : 增加intf或mod  intf
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数
   2.日    期   : 2015年12月15日
    作    者   : hdjia
    修改内容   : 适配代码开发，该函数只是intf的增加操作，无mod操作

*****************************************************************************/
NBB_INT spm_logical_port_drv_add_l3_intf(NBB_BYTE unit,SPM_INTF_CB *intf_cb)
{
    INTF_T       st_flow; /* INTF的结构体*/
    NBB_INT         ret = SUCCESS;
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_l3_intf");
    
    if (NULL == intf_cb)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_two_value(ret,intf_cb->svlan_id,intf_cb->cvlan_id);     
        goto EXIT_LABEL;
    }

    OS_MEMSET(&st_flow, 0, sizeof(INTF_T));
    
#ifdef SPU   

    //IPV4 URPF使能
    if (ATG_DCI_INTF_URPF_DEFAULT == intf_cb->ipv4_urpf_mode)     //URPF缺省检查
    {
        st_flow.eIpv4UrpfMode = URPF_OFF;
    }
    else if (ATG_DCI_INTF_URPF_STRICT == intf_cb->ipv4_urpf_mode)  //URPF严格检查
    {
        st_flow.eIpv4UrpfMode = URPF_STRICT;
    }
    else if (ATG_DCI_INTF_URPF_LOOSE == intf_cb->ipv4_urpf_mode)  //URPF松散检查
    {
        st_flow.eIpv4UrpfMode = URPF_LOOSE;
    }

    //IPV6 URPF使能
    if (ATG_DCI_INTF_URPF_DEFAULT == intf_cb->ipv6_urpf_mode)     //URPF缺省检查
    {
        st_flow.eIpv6UrpfMode = URPF_OFF;
    }
    else if (ATG_DCI_INTF_URPF_STRICT == intf_cb->ipv6_urpf_mode)  //URPF严格检查
    {
        st_flow.eIpv6UrpfMode = URPF_STRICT;
    }
    else if (ATG_DCI_INTF_URPF_LOOSE == intf_cb->ipv6_urpf_mode)  //URPF松散检查
    {
        st_flow.eIpv6UrpfMode = URPF_LOOSE;
    }
    
    //缺省路由检查使能
    if (ENABLE == intf_cb->ipv4_default_check == ENABLE)
    {
        st_flow.flags |= INTF_V4_ALLOW_DEFAULT_ROUTE;
    }

    if (ENABLE == intf_cb->ipv6_default_check == ENABLE)
    {
        st_flow.flags |= INTF_V6_ALLOW_DEFAULT_ROUTE;
    }

    //MPLS转发不使能
    if (DISABLE == intf_cb->mpls_forward_enable == DISABLE)
    {
        st_flow.flags |= INTF_MPLS_DISABLE;
    }
    
    /* 配置解耦，8000的驱动接口保持不变*/
    if ((0 == intf_cb->svlan_id) && (0 == intf_cb->cvlan_id))
    {        
        if (ATG_DCI_VE_PORT == intf_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L3_VE_PORT;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT;
        }
    }
    else if ((0 != intf_cb->svlan_id) && (0 == intf_cb->cvlan_id))
    {   
        if (ATG_DCI_VE_PORT == intf_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L3_VE_PORT_OVID;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT_OVID;
        }
    }
    else if ((0 != intf_cb->svlan_id) && (0 != intf_cb->cvlan_id))
    {  
        if (ATG_DCI_VE_PORT == intf_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L3_VE_PORT_OVID_IVID;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT_OVID_IVID;
        }
    }
    else
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_two_value(ret,intf_cb->svlan_id,intf_cb->cvlan_id);     
        goto EXIT_LABEL;      
    }

    st_flow.eIntfType = INTF_TYPE_L3;    
    st_flow.mCardPort = intf_cb->port_id;      
    st_flow.bindId = intf_cb->bind_id;
    st_flow.mOvid= intf_cb->svlan_id;
    st_flow.mIvid = intf_cb->cvlan_id;
    
    func_ret = ApiC3AddIntf(unit, &st_flow);

    if (SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code_and_three_value(ret,intf_cb->svlan_id,
            intf_cb->cvlan_id,func_ret);
        goto EXIT_LABEL;
    }
       
    intf_cb->intf_pos_id[unit] = st_flow.posId;
#endif  

    EXIT_LABEL:    NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_logical_port_drv_del_intf
   功能描述  : 删除intf
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_del_intf(NBB_BYTE unit_id,NBB_ULONG intf_pos_id)
{
    NBB_INT     ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_INT     unit = 0;

    NBB_TRC_ENTRY("spm_logical_port_drv_del_intf");

    /* id valid judge (hdjia) */
    if (0 == intf_pos_id)
    {
        ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
        goto EXIT_LABEL;
    }
    
#ifdef SPU      
    unit = unit_id;
    func_ret = ApiC3DelIntf(unit, intf_pos_id);
    
    if (SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
        spm_l2_record_err_code_and_two_value(ret,intf_pos_id,func_ret);
        goto EXIT_LABEL;
    }
#endif  

    EXIT_LABEL: NBB_TRC_EXIT();   
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_ve_alarm_cfg
   功能描述  : 注册ve主接口的告警
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_ve_alarm_cfg(NBB_ULONG port_index)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_main_logical_port_pm_cfg
   功能描述  : 注册以太网主接口的性能
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_main_logical_port_pm_cfg(NBB_ULONG port_index)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_del_main_logical_port_pm_cfg
   功能描述  : 删除以太网主接口的性能
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_del_main_logical_port_pm_cfg(NBB_ULONG port_index)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_logic_flow_classify_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_logic_flow_classify_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_acl_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_acl_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_ds_add_logic_flow_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_ds_add_logic_flow_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_flow_up_usr_qos_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_flow_up_usr_qos_node (NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_hqos_add_group_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_hqos_add_group_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_hqos_add_usr_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_hqos_add_usr_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_pw_car_qos_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_pw_car_qos_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_ds_add_logic_univp_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_ds_add_logic_univp_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

#endif

/*****************************************************************************
   函 数 名  : spm_logical_port_del_almpm_line_intf
   功能描述  : 删除时，调用almpm接口
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_del_almpm_line_intf(NBB_ULONG port_index,NBB_ULONG intf_pos_id,NBB_BYTE uc_flag)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_almpm_addcfg_intf_onePos
   功能描述  : intf类业务配置增加条目时，查询是否需要采集性
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_almpm_addcfg_intf_onepos(NBB_ULONG port_index, NBB_UINT pos_id)
{
  return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_drv_set_acl_vrrp
   功能描述  : 删除端口逻辑配置
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数
   修改历史      :
   1.日    期   : 2016年1月25日
    作    者   : fansongbo
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT spm_logical_port_drv_set_acl_vrrp(SPM_LOGICAL_PORT_CB *pst_logical_port,
    SPM_VRRP_CB *st_vrrp_cb,NBB_ULONG *terminal_if_num)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;

    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    NBB_BYTE c3_unit =0; 
    NBB_ULONG terminal_intf_num = 0;
    VRRP_ACL_T st_vrrp; /* vrrp的结构体*/    
    
    NBB_TRC_ENTRY("spm_logical_port_drv_set_acl_vrrp");
    
    if (NULL == pst_logical_port)
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        goto EXIT_LABEL;
    }
    
    if (NULL == st_vrrp_cb)
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        goto EXIT_LABEL;
    }

    if (NULL == terminal_if_num)
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_vrrp, 0, sizeof(VRRP_ACL_T)); 

 #ifdef SPU     
    st_vrrp.slot = st_vrrp_cb->slot_id;
    st_vrrp.l3_port_id = st_vrrp_cb->port_id;
    st_vrrp.ve_flag = st_vrrp_cb->ve_flag;
    st_vrrp.dmac[0] = st_vrrp_cb->dmac[0];
    st_vrrp.dmac[1] = st_vrrp_cb->dmac[1];
    st_vrrp.dmac[2] = st_vrrp_cb->dmac[2];
    st_vrrp.dmac[3] = st_vrrp_cb->dmac[3];
    st_vrrp.dmac[4] = st_vrrp_cb->dmac[4];
    st_vrrp.dmac[5] = st_vrrp_cb->dmac[5];   
    
    //设置intf VRRP
    for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
        pst_terminal_if_cb->spm_terminal_if_node))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {        
            if(0 != pst_terminal_if_cb->intf_pos_id[c3_unit])
            {
                st_vrrp.ovlan_id = pst_terminal_if_cb->terminal_if_cfg.svlan_id;
                st_vrrp.ivlan_id = pst_terminal_if_cb->terminal_if_cfg.cvlan_id;
                
                //外层VLAN
                if(0  !=  st_vrrp.ovlan_id)/*lint !e613 */
                {
                    st_vrrp.ovlan_id_mask = 0xfff;
                }
                else
                {
                    st_vrrp.ovlan_id_mask = 0;
                }

                //内外层VLAN
                if(0  !=  st_vrrp.ivlan_id)/*lint !e613 */
                {
                    st_vrrp.ivlan_id_mask = 0xfff;
                }
                else
                {
                    st_vrrp.ivlan_id_mask = 0;
                }
                
                func_ret = SUCCESS;
                func_ret = fhdrv_psn_acl_set_vrrp(c3_unit,&st_vrrp);

                if(SUCCESS == func_ret)
                {
                    st_vrrp_cb->posid[c3_unit] = st_vrrp.posid;
                }
                else
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                    spm_l2_record_err_code_and_four_value(ret,c3_unit,
                        pst_terminal_if_cb->terminal_if_cfg.svlan_id,
                        pst_terminal_if_cb->terminal_if_cfg.cvlan_id,
                        func_ret);
                    break;                
                }
            }           
        }  
        
        terminal_intf_num++;
    }  
        
    *terminal_if_num = terminal_intf_num;
 #endif
 
    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_drv_set_intf_vrrp
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_set_intf_vrrp(NBB_BYTE unit_id,NBB_ULONG intf_pos_id)
{
    NBB_INT     ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_drv_set_intf_vrrp");
    
#ifdef SPU  
    func_ret = fhdrv_psn_l3_set_intf_vrrp(unit_id, intf_pos_id,1);
    
    if (SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
        spm_l2_record_err_code_and_two_value(ret,intf_pos_id,func_ret);
        goto EXIT_LABEL;
    }
#endif

    EXIT_LABEL: NBB_TRC_EXIT();   
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_apply_set_basic_data
   功能描述  : 1)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_set_basic_data(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_BASIC_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_logical_port_apply_set_basic_data"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_BASIC_DRV_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = LOG_PORT_BASIC_DRV_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    //更新逻辑接口，如果前后下发的L2、L3属性不一致
    //非法，直接返错退出
     if((NULL != pst_logical_port->basic_cfg_cb)
        && (pst_logical_port->basic_cfg_cb->port_route_type != pst_tlv_cfg->port_route_type))
    {
        ret = LOG_PORT_BASIC_DRV_PROC_L2L3_TYPE_ERR;
        spm_l2_record_err_code_and_two_value(ret,
            pst_logical_port->basic_cfg_cb->port_route_type,
            pst_tlv_cfg->port_route_type);  
        
        //goto EXIT_LABEL;
    }
    
    if (ATG_DCI_LOOPBACK == pst_tlv_cfg->port_flag)
    {
        ret = LOG_PORT_BASIC_DRV_PROC_PORT_IS_LOOP_BACK;
        spm_l2_record_err_code(ret); 
        
        //goto EXIT_LABEL;
    }
   
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
   函 数 名  : spm_logical_port_apply_set_phy_data
   功能描述  : 2)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_set_phy_data(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_PHY_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_INFO_CB pst_logical_port_info;
    SPM_PORT_STRUCT st_port;
    
    /* key值: index */
    NBB_ULONG port_index_key = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_apply_set_phy_data");

    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_PHY_DRV_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = LOG_PORT_PHY_DRV_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&pst_logical_port_info,0,sizeof(SPM_LOGICAL_PORT_INFO_CB));
    OS_MEMSET(&st_port,0,sizeof(SPM_PORT_STRUCT));
    port_index_key = pst_logical_port->port_index_key;
    
    //func_ret = spm_logical_port_get_port_info_from_index(port_index_key,&pst_logical_port_info);
    func_ret = spm_logical_port_get_port_info_from_index(
        pst_logical_port,
        pst_tlv_cfg,
        &pst_logical_port_info);
    
    if(SUCCESS != func_ret)
    {
        ret = LOG_PORT_PHY_DRV_PROC_GET_PORT_INFO_FAIL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
        goto EXIT_LABEL;
    }
    
#if 0    
    func_ret = spm_hardw_getc3lineport(pst_logical_port_info.card_id,pst_logical_port_info.phy_port_id,
        pst_tlv_cfg->circuit_emu_mode,&st_port);

    if(SUCCESS != func_ret)
    {
        ret = LOG_PORT_PHY_DRV_PROC_GET_DRV_LINE_PORT_FAIL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
        //goto EXIT_LABEL;
    }
    
    pst_logical_port->slot_id = pst_logical_port_info.slot_id; 
    pst_logical_port->unit_id = st_port.ucUnit;
    pst_logical_port->port_id = st_port.ucPort; 
    pst_logical_port_info.unit_id = st_port.ucUnit;
    pst_logical_port_info.port_id = st_port.ucPort; 
#endif 
#if 1    
    pst_logical_port->slot_id = pst_logical_port_info.slot_id; 
    pst_logical_port->unit_id = pst_logical_port_info.unit_id;
    pst_logical_port->port_id = pst_logical_port_info.port_id;
#endif 
    
    //NBB_MEMCPY(&pst_logical_port->logic_port_info_cb,&pst_logical_port_info,sizeof(SPM_LOGICAL_PORT_INFO_CB));    
    pst_logical_port->logic_port_info_cb.slot_id = pst_logical_port_info.slot_id;
    pst_logical_port->logic_port_info_cb.card_id = pst_logical_port_info.card_id;    
    pst_logical_port->logic_port_info_cb.unit_id = pst_logical_port_info.unit_id;
    pst_logical_port->logic_port_info_cb.port_id = pst_logical_port_info.port_id; 
    pst_logical_port->logic_port_info_cb.port_type = pst_logical_port_info.port_type;
    pst_logical_port->logic_port_info_cb.phy_port_type = pst_logical_port_info.phy_port_type;
    pst_logical_port->logic_port_info_cb.phy_port_id = pst_logical_port_info.phy_port_id;    
    pst_logical_port->logic_port_info_cb.lag_slot_id = pst_logical_port_info.lag_slot_id;
    pst_logical_port->logic_port_info_cb.lag_port_id = pst_logical_port_info.lag_port_id;
    pst_logical_port->logic_port_info_cb.ve_slot_id = pst_logical_port_info.ve_slot_id;     
        
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_drv_set_logic_l3
   功能描述  : 3)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_set_logic_l3(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_LOGIC_L3_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0; 
    SPM_PORT_INFO_CB st_port_info;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    NBB_ULONG intf_pos_id;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_set_logic_l3");

    if (NULL == pst_logical_port)
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
    
    OS_MEMSET(&st_port_info,0,sizeof(SPM_PORT_INFO_CB));
    
#ifdef SPU   

    if (0 != pst_logical_port->terminal_if_num)
    {
        for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
            pst_terminal_if_cb != NULL;
            pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
            pst_terminal_if_cb->spm_terminal_if_node))
        {
            for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
            {
                if (0 != pst_terminal_if_cb->intf_pos_id[c3_unit])
                {
                    intf_pos_id = pst_terminal_if_cb->intf_pos_id[c3_unit];

                    //刷新vrfid
                    func_ret = SUCCESS;
                    func_ret = ApiC3SetIntfBindId(c3_unit,intf_pos_id,1,pst_tlv_cfg->vrf_id);                         
                    
                    if(SUCCESS != func_ret)
                    {
                        ret = SPM_LOGICAL_E_CALL_LOGICAL;
                        spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->vrf_id,
                            pst_terminal_if_cb->terminal_if_cfg.svlan_id,
                            pst_terminal_if_cb->terminal_if_cfg.cvlan_id);
                    }

                    //IPV4 URPF使能
                    func_ret = SUCCESS;
 
                    if (ATG_DCI_INTF_URPF_DEFAULT == pst_tlv_cfg->ipv4_urpf_mode)     //URPF缺省检查
                    {
                        func_ret = ApiC3SetIntfV4UrpfMode(c3_unit,intf_pos_id,URPF_OFF,pst_tlv_cfg->ipv4_default_check);
                        
                    }
                    else if (ATG_DCI_INTF_URPF_STRICT == pst_tlv_cfg->ipv4_urpf_mode)  //URPF严格检查
                    {
                        func_ret = ApiC3SetIntfV4UrpfMode(c3_unit,intf_pos_id,URPF_STRICT,pst_tlv_cfg->ipv4_default_check);
                       
                    }
                    else if (ATG_DCI_INTF_URPF_LOOSE == pst_tlv_cfg->ipv4_urpf_mode)  //URPF松散检查
                    {
                        func_ret = ApiC3SetIntfV4UrpfMode(c3_unit,intf_pos_id,URPF_LOOSE,pst_tlv_cfg->ipv4_default_check);
                    } 
                    
                    if(SUCCESS != func_ret)
                    {
                        ret = SPM_LOGICAL_E_CALL_LOGICAL;
                        spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->vrf_id,
                            pst_terminal_if_cb->terminal_if_cfg.svlan_id,
                            pst_terminal_if_cb->terminal_if_cfg.cvlan_id);
                    }
                    
                    //IPV6 URPF使能
                    func_ret = SUCCESS;
                    
                    if (ATG_DCI_INTF_URPF_DEFAULT == pst_tlv_cfg->ipv6_urpf_mode)     //URPF缺省检查
                    {
                        func_ret = ApiC3SetIntfV6UrpfMode(c3_unit,intf_pos_id,URPF_OFF,pst_tlv_cfg->ipv6_default_check);
                        
                    }
                    else if (ATG_DCI_INTF_URPF_STRICT == pst_tlv_cfg->ipv6_urpf_mode)  //URPF严格检查
                    {
                        func_ret = ApiC3SetIntfV6UrpfMode(c3_unit,intf_pos_id,URPF_STRICT,pst_tlv_cfg->ipv6_default_check);
                       
                    }
                    else if (ATG_DCI_INTF_URPF_LOOSE == pst_tlv_cfg->ipv6_urpf_mode)  //URPF松散检查
                    {
                        func_ret = ApiC3SetIntfV6UrpfMode(c3_unit,intf_pos_id,URPF_LOOSE,pst_tlv_cfg->ipv6_default_check);
                    } 
                    
                    if(SUCCESS != func_ret)
                    { 
                        ret = SPM_LOGICAL_E_CALL_LOGICAL;
                        spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->vrf_id,
                            pst_terminal_if_cb->terminal_if_cfg.svlan_id,
                            pst_terminal_if_cb->terminal_if_cfg.cvlan_id);
                    }

                    //MPLS转发不使能
                    //使能:0/1-enable /disable
                    func_ret = SUCCESS;
                    
                    if (pst_tlv_cfg->mpls_forward_enable == DISABLE)
                    {
                        func_ret = ApiC3SetIntfMplsDisable(c3_unit,intf_pos_id,ENABLE);
                    }

                    else if(pst_tlv_cfg->mpls_forward_enable == ENABLE)
                    {
                        func_ret = ApiC3SetIntfMplsDisable(c3_unit,intf_pos_id,DISABLE);
                    }

                    if(SUCCESS != func_ret)
                    {
                        ret = SPM_LOGICAL_E_CALL_LOGICAL;
                        spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->mpls_forward_enable,
                            pst_terminal_if_cb->terminal_if_cfg.svlan_id,
                            pst_terminal_if_cb->terminal_if_cfg.cvlan_id);
                    }

                    spm_logical_port_almpm_addcfg_intf_onepos(pst_logical_port->port_index_key, intf_pos_id);

                }                        
            }                   
        }
    }
#endif

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_del_intf
   功能描述  : 删除端口逻辑配置的intf
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2016年1月15日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_set_intf_ptag(SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_tlv_cfg)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit = 0;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_set_intf_ptag");
    
    if (NULL == pst_logical_port)
    { 
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret);         
        goto EXIT_LABEL;
    }
    
     if (NULL == pst_tlv_cfg)
    { 
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret);         
        goto EXIT_LABEL;
    }
    
#ifdef SPU

    //刷新ntf
    for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
        pst_terminal_if_cb->spm_terminal_if_node))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret = SUCCESS;

            if(0 != pst_terminal_if_cb->intf_pos_id[c3_unit])
            {
                func_ret = fhdrv_psn_l3_set_intf_ptag(c3_unit,
                    pst_terminal_if_cb->intf_pos_id[c3_unit],                    
                    pst_tlv_cfg->recv_action);

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;          
                    spm_l2_record_err_code_and_three_value(ret,c3_unit,pst_terminal_if_cb->intf_pos_id,func_ret);                      
                }
            } 
        }        
    }
#endif

    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_get_vpws_vp_num
   功能描述  : 获取VPWS中UNI和NNI的个数和
   输入参数  : NBB_USHORT usVpnId
             NBB_INT *piVpNum
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

  修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_get_vpws_vp_num(SPM_VPWS_CB *pst_vpws, NBB_INT *vp_num)
{
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_get_vpws_vp_num");
    
    if (NULL == pst_vpws)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == vp_num)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    *vp_num = pst_vpws->nni_num + pst_vpws->uni_num;
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   函 数 名   : spm_if_vpn_uni_exist
   功能描述   : 判断vpn中uni是否存在
   输入参数   : ATG_DCI_SET_LOGICAL_PORT *pst_set_logical_port
   输出参数   : 无
   返 回 值   :
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_if_vpn_uni_exist(NBB_BYTE uc_vpn_type,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data,
    NBB_VOID *pst_vpn)
{
    NBB_INT ret = SUCCESS;

    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_CB *pst_vpls = NULL;
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_if_vpn_uni_exist");
    
    if (NULL == pst_vpn)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if ((NULL == pst_l2vpn_data)) 
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (L2VPN_VPWS == uc_vpn_type)
    {
        pst_vpws = (SPM_VPWS_CB *)pst_vpn;

        if (pst_vpws->uni_no == pst_l2vpn_data->uni_no)
        {
            ret = 1;
        }
        else if (pst_vpws->uni_no_2 == pst_l2vpn_data->uni_no)
        {
            ret = 2;
        }       
    }
    else if (L2VPN_VPLS == uc_vpn_type)
    {
    
    }
    else
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名   : spm_vpn_add_vp_uni
   功能描述   : 向vpn中添加UNI VP
   输入参数   : 
   输出参数   : 无
   返 回 值   :
   调用函数   :
   被调函数   :

   修改历史   :
   1.日    期 : 2015年8月29日
     作    者 : lish
     修改内容 : 新生成函数

   2.日    期 : 2015年12月15日
     作    者 : hdjia
     修改内容 : 修改，将配置整体数据传入适配层，避免后期接口数据不全函数接口
        改动大；避免配置二次赋值

*****************************************************************************/
NBB_INT spm_logical_port_drv_add_uni_to_vpn(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    SPM_PHYSICAL_PORT_CB *pst_physical_port = NULL;
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_CB *pst_vpls = NULL;
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
	

    /* 逻辑接口的键值 */
    VPORT_T st_uni_vport;
    NBB_ULONG msp_id = 0;    
    NBB_UINT nhi_id = 0;   
    NBB_BYTE c3_unit = 0;
    NBB_BYTE  vpn_type = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_uni_to_vpn");
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_LOGL2_DRV_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logic_l2_data)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    if (NULL == pst_l2vpn_data)
    {
        ret = LOG_PORT_LOGL2_DRV_PROC_TLV_L2VPN_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = LOG_PORT_LOGL2_DRV_PROC_BASIC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->phy_cfg_cb)
    {
        ret = LOG_PORT_LOGL2_DRV_PROC_PHY_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_uni_vport, 0, sizeof(VPORT_T));

  #ifdef SPU
  
    st_uni_vport.vpIdx = pst_l2vpn_data->uni_no;    
    st_uni_vport.tNextHop.tTxPort.port = pst_logical_port->port_id;
    st_uni_vport.tNextHop.tTxPort.slot = pst_logical_port->slot_id;  //todo
    
    if ((ATG_DCI_SUB_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type)
        || (ATG_DCI_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type))
    {


    }
    
    //VLAN 发送TAG行为
    switch (pst_logic_l2_data->send_tag_action)
    {
        case ATG_DCI_ADD_VID_PRI:
            st_uni_vport.tNextHop.eEgrPtagAction = EGR_ENCAP;
            break;
        case ATG_DCI_DEL_VID_PRI:
            st_uni_vport.tNextHop.eEgrPtagAction = EGR_NO_ENCAP;
            break;
        default:
        	break;
    }
    
    st_uni_vport.tNextHop.egrPtagVlan = pst_logic_l2_data->send_vlan_id;
    st_uni_vport.tNextHop.egrPtagPri = pst_logic_l2_data->send_vlan_pri;    

    if (0x8100 == pst_logic_l2_data->tagtype)
    {
        st_uni_vport.tNextHop.eEgrPtagTpidType = TPID_0X8100;
    }
    else if (0x88a8 == pst_logic_l2_data->tagtype)
    {
        st_uni_vport.tNextHop.eEgrPtagTpidType = TPID_0X88A8;
    }   

    //如果UNI口为CES类型
    if (ATG_DCI_CES == pst_logical_port->basic_cfg_cb->port_flag)
    {
        st_uni_vport.eVpType = VP_CES;
        st_uni_vport.matchVcLabel = pst_logical_port->phy_cfg_cb->phy_port_index;
        st_uni_vport.tNextHop.tPwLabel.label_un.tlabel.label = pst_logical_port->phy_cfg_cb->phy_port_index;
        st_uni_vport.tNextHop.tPwLabel.label_un.tlabel.bottom = 1;
        st_uni_vport.tNextHop.tPwLabel.label_un.tlabel.ttl = 255; //协议里面需要定义 todo
        st_uni_vport.tNextHop.flags |= L2_NEXT_HOP_CES;
        st_uni_vport.tNextHop.eCwMode = EGR_CW_ORIGINAL;

        //如果备用端口不为0，且msp_id不为0，配置msp保护
        if (0 != msp_id) 
        {

        }
    }
    else
    {
        st_uni_vport.eVpType = VP_UNI;

        //如果是VE口，要置VE标致位
        //虚拟以太网接口
        if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
        {
            st_uni_vport.tNextHop.flags |= L2_NEXT_HOP_VEPORT;
        }
        else
        {
            st_uni_vport.tNextHop.flags |= L2_NEXT_HOP_INTF;
        }
    }
    
    //如果是VPLS业务，需要配置MCA学习和水平分割
    if (L2VPN_VPLS == pst_l2vpn_data->vpn_type)
    {
        //MAC学习开关
        if (DISABLE == pst_logic_l2_data->mac_learning_enable)
        {
            st_uni_vport.flags |= VPORT_LEARN_DISABLE;
        }

        //水平分割开关
        if (ENABLE == pst_logic_l2_data->split_horizon_enable)
        {
            st_uni_vport.flags |= VPORT_NETWORK;
        }

		pst_vpls = AVLL_FIND(v_spm_shared->vpls_tree,&pst_l2vpn_data->vpn_id);

		if((NULL != pst_vpls) && (NULL != pst_vpls->basic_cfg_cb))
		{
			st_uni_vport.mcId = pst_vpls->basic_cfg_cb->mc_id;
		}
    }
    
    if (0 != pst_logical_port->logic_port_info_cb.next_hop_id)
    {
    	st_uni_vport.tNextHop.nhIdx = pst_logical_port->logic_port_info_cb.next_hop_id;
    }
    else
    {
    	spm_vc_alloc_nhi_id(&nhi_id NBB_CCXT);
    	st_uni_vport.tNextHop.nhIdx = nhi_id;
    }

	//新建UNI
    if (0 == pst_logical_port->logic_port_info_cb.vp_idx)
    {
        //调用API函数进行配置
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret += ApiC3AddMplsPortToVpn(c3_unit, 
                pst_l2vpn_data->vpn_type, pst_l2vpn_data->vpn_id, &st_uni_vport);
        }

        if (SUCCESS == func_ret)
        {
            pst_logical_port->logic_port_info_cb.vp_idx = st_uni_vport.vpIdx;
            pst_logical_port->logic_port_info_cb.next_hop_counter_id = st_uni_vport.tNextHop.counterIdx;
            pst_logical_port->logic_port_info_cb.vpn_type = pst_l2vpn_data->vpn_type;
            pst_logical_port->logic_port_info_cb.vpn_id = pst_l2vpn_data->vpn_id;
            pst_logical_port->logic_port_info_cb.next_hop_id = st_uni_vport.tNextHop.nhIdx;
            
            //将vp与intf绑定
            for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
                pst_terminal_if_cb != NULL;
                pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
                pst_terminal_if_cb->spm_terminal_if_node))
            {
                    func_ret = SUCCESS;
                    
                    //调用API函数进行配置
                    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
                    {
                        if (0 != pst_terminal_if_cb->intf_pos_id[c3_unit])
                        {
                            func_ret += ApiC3SetIntfBindId(c3_unit,pst_terminal_if_cb->intf_pos_id[c3_unit],
                                INTF_TYPE_L2,pst_logical_port->logic_port_info_cb.vp_idx);
                        }
                    }           

                    if (SUCCESS != func_ret)
                    {
                        ret = LOG_PORT_LOGL2_DRV_PROC_CALL_DRV_BIND_INTF_FAIL;
                        spm_l2_record_err_code_and_three_value(ret,
                            pst_terminal_if_cb->terminal_if_cfg.svlan_id,
                            pst_terminal_if_cb->terminal_if_cfg.cvlan_id,
                            pst_logical_port->logic_port_info_cb.vp_idx);  
                    }
            }            
        }
        else
        {
            if (0 != st_uni_vport.tNextHop.nhIdx)
            {
                spm_vc_free_nhi_id(st_uni_vport.tNextHop.nhIdx);
            }

            if (0 != st_uni_vport.tNextHop_p.nhIdx)
            {
                spm_vc_free_nhi_id(st_uni_vport.tNextHop_p.nhIdx);
            }

            ret = LOG_PORT_LOGL2_DRV_PROC_CALL_DRV_ADD_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);       
            goto EXIT_LABEL;
        }
    }
    else
    {      
        //调用API函数进行配置，更新VP
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret += ApiC3ModVp(c3_unit, &st_uni_vport);
        }

        if (func_ret == SUCCESS)
        {
            pst_logical_port->logic_port_info_cb.vp_idx = st_uni_vport.vpIdx;
            pst_logical_port->logic_port_info_cb.next_hop_counter_id = st_uni_vport.tNextHop.counterIdx;
            pst_logical_port->logic_port_info_cb.vpn_type = pst_l2vpn_data->vpn_type;
            pst_logical_port->logic_port_info_cb.vpn_id = pst_l2vpn_data->vpn_id;
            pst_logical_port->logic_port_info_cb.next_hop_id = st_uni_vport.tNextHop.nhIdx;
        }
        else
        {
            ret = LOG_PORT_LOGL2_DRV_PROC_CALL_DRV_MOD_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);       
            goto EXIT_LABEL;
        }
    }
#endif 

   EXIT_LABEL :NBB_TRC_EXIT();
    return ret; 
}




/*****************************************************************************
 函 数 名  : spm_vc_drv_add_mc_nni_vp
 功能描述  : VPLS时建立大组播, 添加组播VP
 输入参数  : NBB_BYTE ucVpType                       
             NBB_USHORT usVpnId                      
             NBB_VOID* pstVpData                     
             SPM_VPN_PORT_INFO_CB *pstVplsPortInfo  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月24日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_add_mc_uni_vp(NBB_USHORT vpls_id,NBB_USHORT mc_id,
	SPM_LOGICAL_PORT_CB *logical_port)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_INT i = 0;
	NBB_INT uni_vp_id = 0;
    NBB_BYTE ucC3Unit = 0;
    SPM_PORT_INFO_CB stPortInfo;
   
    NBB_BYTE  ucPortNum = 0;
    NBB_ULONG port_index = 0;
	NBB_INT ecmpVpflag = 0;

    NBB_TRC_ENTRY("spm_vpls_add_mc_vp_nni");

    if (NULL == logical_port)
    {
		ret = LOG_PORT_LOGL2_DRV_ADD_MC_LOGICAL_PORT_NULL;
		goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	uni_vp_id = logical_port->logic_port_info_cb.vp_idx;
	port_index = logical_port->port_index_key;
	
    ret = spm_get_portid_from_logical_port_index(port_index,&stPortInfo);

	//打印关键信息
	SPM_L2_INFO_PRINT_LOG(ucMessage,"INFO:add_drv_mc,mc_id=%d,index=%ld,slot=%d,vp=%d\n",
		mc_id,port_index,stPortInfo.slot_id,uni_vp_id);

	
#if defined SRC || defined SNU

	//如果是LAG，需要找到LAG下第一个端口的槽位，下发给FE1600
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = ApiDfeMulticastAdd(UNIT_0, mc_id, stPortInfo.lag_slot_id);
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, mc_id, stPortInfo.slot_id);
	}
    
    if (ret != SUCCESS)
    {
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",
			LOG_PORT_ADD_DRV_UNI_MC_FE_ADD_MEMBER_FAIL,ret);
		
        ret = LOG_PORT_ADD_DRV_UNI_MC_FE_ADD_MEMBER_FAIL;
        
        goto EXIT_LABEL;
    }
    
#endif




#ifdef SPU 
    
     
    //1、如果组播端口接口或子接口，不在本槽位，或
    //2、如果组播端口为LAG，LAG成员端口的第一个端口所在的槽不在本槽位；或
    //3、如果组播端口为VE，不在本槽位
    //则不调用驱动，直接返回成功。
    if (((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_SUB_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_VE_PORT == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.lag_slot_id) && (ATG_DCI_LAG == stPortInfo.port_type)))
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }


	//如果是LAG，需要找到LAG下第一个端口，下发给Arad
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = ApiAradAddEngressMcPort(UNIT_0, mc_id, SPM_MULTICAST_PORT_0, uni_vp_id);
	}
	else
	{
		//防止同一个VP在前面ECMP VP模式调用过驱动之后再次调用驱动
		if (0 == ecmpVpflag)
		{
			ret = ApiAradAddEngressMcPort(UNIT_0, mc_id, SPM_MULTICAST_PORT_0, uni_vp_id);
		}
	}

	if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",
			LOG_PORT_ADD_DRV_UNI_MC_ARAD_ADD_MEMBER_FAIL,ret);
		
        ret = LOG_PORT_ADD_DRV_UNI_MC_ARAD_ADD_MEMBER_FAIL;
        goto EXIT_LABEL;
    }

	//把mc_id写入vc表中，后面删除vc的时候直接用该字段删除组播
	logical_port->logic_port_info_cb.mc_id = mc_id;
    
#endif

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return ret;

}




/*****************************************************************************
 函 数 名  : spm_vc_drv_del_arad_mc_nni_vp
 功能描述  : 删除MC_VP
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016.1.25
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_del_arad_mc_uni_vp(SPM_LOGICAL_PORT_CB *pst_logical_port)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_INT uni_vp_id = 0;
    NBB_INT i = 0;
    NBB_BYTE ucC3Unit = 0;
    NBB_BYTE ucSlot = 0;
	NBB_INT vp_num_in_slot = 0;//同一个槽位上VP的个数
    SPM_PORT_INFO_CB stPortInfo;

    NBB_TRC_ENTRY("spm_logical_port_drv_del_arad_mc_uni_vp");

    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_DEL_DRV_UNI_MC_LOGICAL_PORT_NULL;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

	//打印关键信息
	SPM_L2_INFO_PRINT_LOG(ucMessage,"INFO:del_drv_mc,mc_id=%d,index=%ld\n",
		pst_logical_port->logic_port_info_cb.mc_id,pst_logical_port->port_index_key);

    
    //根据VC表配置中的port_index获取端口的port_id和slot_id，配给Arad和FE1600
    //这里先只考虑主用VC
    spm_get_portid_from_logical_port_index(pst_logical_port->port_index_key, &stPortInfo);
	
    uni_vp_id = pst_logical_port->logic_port_info_cb.vp_idx;
    ucSlot = stPortInfo.slot_id;

	SPM_L2_ERROR_PRINT_LOG(ucMessage,"DEL ARAD MC,slot=%d,mc_id=%d,vp=%d\n",
		stPortInfo.slot_id,pst_logical_port->logic_port_info_cb.mc_id,uni_vp_id);
    
#ifdef SPU 

//1、如果组播端口接口或子接口，不在本槽位，或
	//2、如果组播端口为LAG，LAG成员端口的第一个端口所在的槽不在本槽位；或
	//3、如果组播端口为VE，不在本槽位
	//则不调用驱动，直接返回成功。
	if (((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_INTFACE == stPortInfo.port_type))
	 || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_SUB_INTFACE == stPortInfo.port_type))
	 || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_VE_PORT == stPortInfo.port_type))
	 || ((SHARED.local_slot_id != stPortInfo.lag_slot_id) && (ATG_DCI_LAG == stPortInfo.port_type)))
	{
        
        ret = SUCCESS;
        goto EXIT_LABEL;
    }
	
	ret = ApiAradDeleteEngressMcPort(UNIT_0,pst_logical_port->logic_port_info_cb.mc_id,
			SPM_MULTICAST_PORT_0,uni_vp_id);

    if (ret != SUCCESS)
    {
    	SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d",
			LOG_PORT_DEL_DRV_NNI_MC_VP_ARAD_PORT_FAIL,ret);
		
    	ret = LOG_PORT_DEL_DRV_NNI_MC_VP_ARAD_PORT_FAIL;
		goto EXIT_LABEL;
    }
	
#endif

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return ret;

}


NBB_INT spm_logical_port_drv_del_src_mc_uni_vp(SPM_VPLS_CB *pst_vpls,NBB_USHORT slot_id)
{
	NBB_INT slot_num = 0;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;

	if((NULL == pst_vpls) || (NULL == pst_vpls->basic_cfg_cb))
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",DRV_DEL_NNI_SRC_MC_VC_NULL);
		return DRV_DEL_NNI_SRC_MC_VC_NULL;
	}

	slot_num = spm_get_vpls_slot_num(pst_vpls,slot_id);

	SPM_L2_ERROR_PRINT_LOG(uc_message,"DEL SRC MC,mc_id=%d,slot_id=%d,slot_num=%d\n",
		pst_vpls->basic_cfg_cb->mc_id,slot_id,slot_num);
	
	if(0 == slot_num)
	{
		#if defined SRC || defined SNU
		ret = ApiDfeMulticastDelete(UNIT_0,pst_vpls->basic_cfg_cb->mc_id,slot_id);

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",DRV_DEL_NNI_SRC_MC_ERR,ret);
			return DRV_DEL_NNI_SRC_MC_ERR;
		}
		#endif
		
	}

	return SUCCESS;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_uni_to_vpn
   功能描述  : 为vpn添加一个VP(UNI)
   输入参数  : NBB_BYTE uc_vpn_type
               NBB_USHORT us_vpn_id
               NBB_VOID *pst_uni_data
               SPM_VPN_PORT_INFO_CB *pst_vpn_port_info
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_add_uni_to_vpn(
    SPM_LOGICAL_PORT_CB *pst_logical_port, 
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data, 
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_CB *pst_vpls = NULL;
    SPM_VP_CB *pst_vp = NULL;    
    NBB_BOOL avll_ret_code;
    NBB_INT vp_num = 0;
    NBB_INT iuni_exist_pos = 0;
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;
    
    /* 逻辑接口的键值 */
    NBB_ULONG ul_port_key = 0;
    
     NBB_TRC_ENTRY("spm_logical_port_apply_add_uni_to_vpn"); 

     if (NULL == pst_logical_port)
     {
        ret = LOG_PORT_LOGL2_APP2_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);         
        goto EXIT_LABEL;
     }

     if (NULL == LOG_PORT_LOGL2_APP2_PROC_TLV_NULL)
     {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret);         
        goto EXIT_LABEL;
     }

     if (NULL == pst_l2vpn_data)
     {
        ret = LOG_PORT_LOGL2_APP2_PROC_TLV_L2VPN_NULL;
        spm_l2_record_err_code(ret);         
        goto EXIT_LABEL;
     }
     
    if (0 == pst_l2vpn_data->uni_no)
    {
        ret = LOG_PORT_LOGL2_APP2_PROC_UNI_NO_IS_ZERO;
        spm_l2_record_err_code(ret);         
        goto EXIT_LABEL;
    }  
      
    ul_port_key = pst_logical_port->port_index_key;     

    /* vpn类型为VPWS*/
    if(L2VPN_VPWS == pst_l2vpn_data->vpn_type)
    {
        pst_vpws = AVLL_FIND(SHARED.vpws_tree, &pst_l2vpn_data->vpn_id);
        
        if (NULL == pst_vpws)
        {
            /* 申请一个新条目的内存空间 */
            pst_vpws = spm_vpws_alloc_cb(NBB_CXT);

            if(NULL == pst_vpws)
            {
                ret = LOG_PORT_LOGL2_APP2_PROC_ALLOC_VPWS_CB_FAIL;
                spm_l2_record_err_code(ret);         
                goto EXIT_LABEL;
            }

            //将新节点插入到tree中
            pst_vpws->vpws_id_key = pst_l2vpn_data->vpn_id;
            avll_ret_code = AVLL_INSERT(SHARED.vpws_tree, pst_vpws->spm_vpws_node);
            NBB_ASSERT(avll_ret_code);

        }

        vp_num = 0;            
        spm_logical_port_get_vpws_vp_num(pst_vpws, &vp_num);            
        pst_vp = AVLL_FIND(SHARED.vp_tree, &pst_l2vpn_data->uni_no);
        
        //新增的VP
        if (NULL == pst_vp) 
        {
            if (vp_num < 2)
            {
                func_ret = spm_logical_port_drv_add_uni_to_vpn(pst_logical_port,pst_logic_l2_data,pst_l2vpn_data);						                    
            }
            else
            {
                ret = LOG_PORT_LOGL2_APP2_PROC_VP_NUM_EXC;
                spm_l2_record_err_code(ret);         
                goto EXIT_LABEL;
            }
        }
        else
        {
            //VP已经存在，且在这个VPN里面
            if (pst_l2vpn_data->vpn_id == pst_vp->vpn_id)
            {
                //是已经存在的某一个UNI vp，可进行更新
                if ((SPM_VP_UNI == pst_vp->vp_type) && (L2VPN_VPWS == pst_vp->vpn_type))                        
                {
                    func_ret = spm_logical_port_drv_add_uni_to_vpn(pst_logical_port,pst_logic_l2_data,pst_l2vpn_data);						                    
                }
                else
                {
                    ret = LOG_PORT_LOGL2_APP2_PROC_VPWS_VP_OR_VPN_TYPE_ERR;
                    spm_l2_record_err_code_and_two_value(ret,pst_vp->vp_type,pst_vp->vpn_type);         
                    goto EXIT_LABEL;
                }					                    
            }
            
            //VP已存在于其它VPN里面，说明主控下发的数据有错
            else
            {
                ret = LOG_PORT_LOGL2_APP2_PROC_VPWS_VP_IN_OTHER_VPN;
                spm_l2_record_err_code(ret);         
                goto EXIT_LABEL;
            } 
        }

        //如果成功 
        if (SUCCESS == func_ret)
        {
            if (NULL == pst_vp)    /* 数据之前未绑定vpn   */
            {
                /* 申请一个新条目的内存空间 */
                pst_vp = spm_alloc_vp_cb();

                if(NULL == pst_vp)
                {
                    ret = LOG_PORT_LOGL2_APP2_PROC_VPWS_ALLOC_VP_CB_FAIL;
                    spm_l2_record_err_code(ret);         
                    goto EXIT_LABEL;
                }

                //将新节点插入到tree中
                pst_vp->vp_idx = pst_l2vpn_data->uni_no;
                avll_ret_code = AVLL_INSERT(SHARED.vp_tree, pst_vp->spm_vp_node);
                NBB_ASSERT(avll_ret_code);
                pst_vp->vpn_type = L2VPN_VPWS;
                pst_vp->vpn_id = pst_l2vpn_data->vpn_id;
                pst_vp->vp_type = SPM_VP_UNI;
               
                if(STA_MAIN == pst_l2vpn_data->master_backup)
                {
                    if(0 == pst_vpws->uni_no)
                    {
                        pst_vpws->uni_no = pst_l2vpn_data->uni_no;
                        pst_vpws->msp_id = pst_l2vpn_data->msp_id;
                        pst_vpws->port_index = ul_port_key;                          
                    }
                    else if (0 == pst_vpws->uni_no_2)
                    {
                        pst_vpws->uni_no_2 = pst_l2vpn_data->uni_no;
                        pst_vpws->msp_id_2 = pst_l2vpn_data->msp_id;
                        pst_vpws->port_index_2 = ul_port_key;                         
                    }
                    
                    pst_vp->port_index = ul_port_key;
                }
                else
                {
                    if(0 == pst_vpws->uni_no)
                    {
                        pst_vpws->uni_no = pst_l2vpn_data->uni_no;
                        pst_vpws->msp_id = pst_l2vpn_data->msp_id;
                        pst_vpws->port_index_bak = ul_port_key;                        
                    }
                    else if (0 == pst_vpws->uni_no_2)
                    {
                        pst_vpws->uni_no_2 = pst_l2vpn_data->uni_no;
                        pst_vpws->msp_id_2 = pst_l2vpn_data->msp_id;
                        pst_vpws->port_index_bak_2 = ul_port_key;                                                    
                    }
                                     
                    pst_vp->port_index_bak = ul_port_key; 
                } 
                
                pst_vpws->uni_num++;                
            } 
            else    /* 数据之前已绑定vpn ，本次更新或者新增主备中的一个  */
            {
                iuni_exist_pos = spm_logical_port_if_vpn_uni_exist(L2VPN_VPWS,pst_l2vpn_data, pst_vpws); 
                
                if(STA_MAIN == pst_l2vpn_data->master_backup) 
                {
                    if(1 == iuni_exist_pos) 
                    {
                        pst_vpws->port_index = ul_port_key;
                    }
                    else if(2 == iuni_exist_pos)
                    {
                        pst_vpws->port_index_2 = ul_port_key;  
                    }
                    
                    pst_vp->port_index = ul_port_key;
                }
                else
                {
                    if(1 == iuni_exist_pos) 
                    {
                        pst_vpws->port_index_bak = ul_port_key;
                    }
                    else if(2 == iuni_exist_pos)
                    {
                        pst_vpws->port_index_bak_2 = ul_port_key;  
                    }

                    pst_vp->port_index_bak = ul_port_key; 
                }                    
            }                               
        }
        else //如果增加UNI口失败记录错误状态
        {
            ret = LOG_PORT_LOGL2_APP2_PROC_VPWS_CALL_DRV_FAIL;
            spm_l2_record_err_code_and_two_value(ret,pst_l2vpn_data->vpn_id,pst_l2vpn_data->uni_no);          
            goto EXIT_LABEL;
        }
    }

    /* vpn类型为VPLS*/
    else if(L2VPN_VPLS == pst_l2vpn_data->vpn_type)
    {
        pst_vpls = AVLL_FIND(SHARED.vpls_tree, &pst_l2vpn_data->vpn_id);
        
        if (NULL == pst_vpls)
        {
            /* 申请一个新条目的内存空间 */
            pst_vpls = spm_alloc_vpls_cb(NBB_CXT);

            if(NULL == pst_vpls)
            {
                ret = LOG_PORT_LOGL2_APP2_PROC_ALLOC_VPLS_CB_FAIL;
                spm_l2_record_err_code(ret);         
                goto EXIT_LABEL;
            }

            //将新节点插入到tree中
            pst_vpls->vpls_id_key = pst_l2vpn_data->vpn_id;
            avll_ret_code = AVLL_INSERT(SHARED.vpls_tree, pst_vpls->spm_vpls_node);
            NBB_ASSERT(avll_ret_code);

#if 0   /* vpls创建放在初始化，业务条目处理取消 (hdjia) */
            func_ret = spm_creat_mpls_vpn(L2VPN_VPLS,pst_l2vpn_data->vpn_id);
            if (SUCCESS != func_ret)
            {
                OS_PRINTF("***ERROR***(%s:%d)creat VPLS_ID=%d vpn error,ret=%d!\n",
                    __FUNCTION__,__LINE__,pst_l2vpn_data->vpn_id,func_ret);
                OS_SPRINTF(uc_message,
                    "***ERROR***(%s:%d)creat VPLS_ID=%d vpn error,ret=%d!\n",
                    __FUNCTION__,__LINE__,pst_l2vpn_data->vpn_id,func_ret);
                BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
                ret = SPM_LOGICAL_E_CALL_LOGICAL;
            }
#endif
            
        }
        
        pst_vp = AVLL_FIND(SHARED.vp_tree, &pst_l2vpn_data->uni_no);
        
        //新增的VP
        if (NULL == pst_vp) 
        {
            func_ret = spm_logical_port_drv_add_uni_to_vpn(pst_logical_port,pst_logic_l2_data,pst_l2vpn_data);
        }
        else
        {
            //VP已经存在，且在这个VPN里面
            if (pst_l2vpn_data->vpn_id == pst_vp->vpn_id)
            {
                //是已经存在的某一个UNI vp，可进行更新
                if ((SPM_VP_UNI == pst_vp->vp_type) && (L2VPN_VPLS == pst_vp->vpn_type))                        
                {
                    func_ret = spm_logical_port_drv_add_uni_to_vpn(pst_logical_port,pst_logic_l2_data,pst_l2vpn_data);						                    
                }
                else
                {
                    ret = LOG_PORT_LOGL2_APP2_PROC_VPLS_VP_OR_VPN_TYPE_ERR;
                    spm_l2_record_err_code_and_two_value(ret,pst_vp->vp_type,pst_vp->vpn_type);         
                    goto EXIT_LABEL;
                }					                    
            }
            
            //VP已存在于其它VPN里面，说明主控下发的数据有错
            else
            {
                ret = LOG_PORT_LOGL2_APP2_PROC_VPLS_VP_IN_OTHER_VPN;
                spm_l2_record_err_code(ret);         
                goto EXIT_LABEL;
            } 
        }    
        
        //如果成功，保存数据
        if (SUCCESS == func_ret)
        {
            if (NULL == pst_vp)    /* 数据之前未绑定vpn   */
            {
                /* 申请一个新条目的内存空间 */
                pst_vp = spm_alloc_vp_cb();

                if(NULL == pst_vp)
                {
                    ret = LOG_PORT_LOGL2_APP2_PROC_VPLS_ALLOC_VP_CB_FAIL;
                    spm_l2_record_err_code(ret);         
                    goto EXIT_LABEL;
                }

                //将新节点插入到tree中
                pst_vp->vp_idx = pst_l2vpn_data->uni_no;
                avll_ret_code = AVLL_INSERT(SHARED.vp_tree, pst_vp->spm_vp_node);
                NBB_ASSERT(avll_ret_code);
                pst_vp->vpn_type = L2VPN_VPLS;
                pst_vp->vpn_id = pst_l2vpn_data->vpn_id;
                pst_vp->vp_type = SPM_VP_UNI;
                
                pst_vpls_uni_cb = spm_alloc_vpls_uni_cb(NBB_CXT);

                if (NULL == pst_vpls_uni_cb)
                {
                    ret = LOG_PORT_LOGL2_APP2_PROC_ALLOC_VPLS_UNI_CB_FAIL;
                    spm_l2_record_err_code(ret);         
                    goto EXIT_LABEL;
                }
                
                pst_vpls_uni_cb->uni_no = pst_l2vpn_data->uni_no;
                avll_ret_code = AVLL_INSERT(pst_vpls->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node);
                NBB_ASSERT(avll_ret_code);                   
                pst_vpls_uni_cb->msp_id = pst_l2vpn_data->msp_id;
                
                if(STA_MAIN == pst_l2vpn_data->master_backup)
                {                    
                    pst_vpls_uni_cb->port_index = ul_port_key;
                    pst_vp->port_index = ul_port_key;
                }
                else
                {
                    pst_vpls_uni_cb->port_index_bak = ul_port_key;
                    pst_vp->port_index_bak = ul_port_key;
                    
                } 
                pst_vpls->uni_num++;
                
            } 
            else    /* 数据之前已绑定vpn ，本次更新或者新增主备中的一个  */
            {
                pst_vpls_uni_cb = AVLL_FIND(pst_vpls->uni_cfg_tree,&pst_l2vpn_data->uni_no);
                
    	         //此时应该可以找到，找不到就返错
                if (NULL != pst_vpls_uni_cb)
                {
                    if(STA_MAIN == pst_l2vpn_data->master_backup)
                    {                    
                        pst_vpls_uni_cb->port_index = ul_port_key;
                        pst_vp->port_index = ul_port_key;
                    }
                    else
                    {
                        pst_vpls_uni_cb->port_index_bak = ul_port_key;
                        pst_vp->port_index_bak = ul_port_key;                        
                    } 
                }
                else
                {
                    ret = LOG_PORT_LOGL2_APP2_PROC_FIND_VPLS_UNI_CB_FAIL;
                    spm_l2_record_err_code(ret);         
                    goto EXIT_LABEL;
                }                    
            }                                          
        }
        else //如果增加UNI口失败记录错误状态
        {
            ret = LOG_PORT_LOGL2_APP2_PROC_VPLS_CALL_DRV_FAIL;
            spm_l2_record_err_code_and_two_value(ret,pst_l2vpn_data->vpn_id,pst_l2vpn_data->uni_no);          
            goto EXIT_LABEL;
        }          
    }
    else
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_two_value(ret,pst_l2vpn_data->vpn_type,pst_l2vpn_data->vpn_id);          
        goto EXIT_LABEL;   
    }
    
   EXIT_LABEL :NBB_TRC_EXIT();
   return ret; 
}

/*****************************************************************************
   函 数 名  : spm_logical_port_apply_set_logic_l2
   功能描述  : 4)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_set_logic_l2(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
	SPM_VPLS_CB *pst_vpls = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_apply_set_logic_l2"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_LOGL2_APP_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = LOG_PORT_LOGL2_APP_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    //更新intf的入口识别动作
    spm_logical_port_drv_set_intf_ptag(pst_logical_port,pst_tlv_cfg);
                
    if (NULL != pst_logical_port->l2vpn_cfg_cb) /* vpn属性已存在 */
    {
        //新增业务或者更新业务
        func_ret = spm_logical_port_apply_add_uni_to_vpn(pst_logical_port,pst_tlv_cfg,
            pst_logical_port->l2vpn_cfg_cb);

		//新增VPLS的VP时候要添加组播成员 zhangzhm
		if((0 == pst_logical_port->logic_port_info_cb.vp_idx)
			&& (L2VPN_VPLS == pst_logical_port->l2vpn_cfg_cb->vpn_type))
		{
			pst_vpls = AVLL_FIND(v_spm_shared->vpls_tree,&pst_logical_port->l2vpn_cfg_cb);

			if((NULL != pst_vpls) && (NULL != pst_vpls->basic_cfg_cb))
			{
				spm_logical_port_drv_add_mc_uni_vp(pst_logical_port->l2vpn_cfg_cb->vpn_id,
					pst_vpls->basic_cfg_cb->mc_id,
					pst_logical_port);
			}
		}
        
        if (SUCCESS != func_ret)
        {
            ret = LOG_PORT_LOGL2_APP_PROC_CALL_APP2_FAIL;
            spm_l2_record_err_code_and_one_value(ret,func_ret);
        }
    }    
                
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_apply_add_l2vpn
   功能描述  : 5)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_add_l2vpn(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
	SPM_VPLS_CB *pst_vpls = NULL;
    
    NBB_TRC_ENTRY("spm_logical_port_apply_add_l2vpn"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_L2VPN_ADD_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = LOG_PORT_L2VPN_ADD_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if ((NULL != pst_logical_port->l2vpn_cfg_cb) 
        && (0 != pst_logical_port->logic_port_info_cb.vp_idx))
    {    				
        if (0 != (NBB_MEMCMP(pst_tlv_cfg, pst_logical_port->l2vpn_cfg_cb,
            sizeof(ATG_DCI_LOG_PORT_L2VPN_DATA))))
        {
            ret = LOG_PORT_L2VPN_ADD_PROC_TLV_DATA_IS_NOT_SAME;
            spm_l2_record_err_code(ret); 
            goto EXIT_LABEL;
        }
    }
    else
    {
        if (NULL != pst_logical_port->logic_l2_cfg_cb)
        {
            //新增业务
            func_ret = spm_logical_port_apply_add_uni_to_vpn(pst_logical_port,
                pst_logical_port->logic_l2_cfg_cb,pst_tlv_cfg);

			//建立组播 zhangzhm
			if(L2VPN_VPLS == pst_tlv_cfg->vpn_type)
			{
				pst_vpls = AVLL_FIND(v_spm_shared->vpls_tree,&pst_tlv_cfg->vpn_id);

				if((NULL != pst_vpls) && (NULL != pst_vpls->basic_cfg_cb))
				{
					spm_logical_port_drv_add_mc_uni_vp(pst_tlv_cfg->vpn_id,
						pst_vpls->basic_cfg_cb->mc_id,
						pst_logical_port);
				}
			}
            
            if (SUCCESS != func_ret)
            {
                ret = LOG_PORT_L2VPN_ADD_PROC_CALL_LOGL2_APP2_FAIL;
                spm_l2_record_err_code_and_one_value(ret,func_ret);
            }
        }
    }
                
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_drv_del_l2vpn
   功能描述  : 5)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_del_l2vpn(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data,
    NBB_VOID *pst_vpn)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL; 
    NBB_BYTE c3_unit = 0;
    
    //可能存在的主备中的另一个   
    SPM_LOGICAL_PORT_CB *pst_logical_port2 = NULL;
    NBB_ULONG port_index = 0;     
    
    NBB_TRC_ENTRY("spm_logical_port_drv_del_l2vpn"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_L2VPN_DRV_DEL_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_l2vpn_data)
    {
        ret = LOG_PORT_L2VPN_DRV_DEL_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_vpn)
    {
        ret = LOG_PORT_L2VPN_DRV_DEL_PROC_VPN_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
#ifdef SPU

    /* vpn类型为VPWS*/
    if(L2VPN_VPWS == pst_l2vpn_data->vpn_type)
    {
        pst_vpws = (SPM_VPWS_CB *)pst_vpn;
        
        if (STA_MAIN == pst_l2vpn_data->master_backup) 
        {  
            port_index = pst_vpws->port_index_bak;
        }  
        else
        {  
            port_index = pst_vpws->port_index;
        }      
    }
    
    /* vpn类型为VPLS*/
    else if(L2VPN_VPLS == pst_l2vpn_data->vpn_type)
    {
        pst_vpls_uni_cb = (SPM_VPLS_UNI_CB *)pst_vpn;  
        
        if (STA_MAIN == pst_l2vpn_data->master_backup) 
        {  
            port_index = pst_vpls_uni_cb->port_index_bak;
        }  
        else
        {  
            port_index = pst_vpls_uni_cb->port_index;
        }        
    }
    
    pst_logical_port2 = AVLL_FIND(SHARED.logical_port_tree, &port_index);   
        
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret += ApiC3DelMplsPortFromVpn(
            c3_unit, 
            pst_l2vpn_data->vpn_type, 
            pst_l2vpn_data->vpn_id, 
            pst_l2vpn_data->uni_no);
    }
    
    if (SUCCESS == func_ret) 
    {  
        pst_logical_port->logic_port_info_cb.vpn_type = 0;
        pst_logical_port->logic_port_info_cb.vpn_id = 0;
        pst_logical_port->logic_port_info_cb.vp_idx = 0;
        pst_logical_port->logic_port_info_cb.next_hop_counter_id = 0;
                
        if (0 != pst_logical_port->logic_port_info_cb.next_hop_id)
        {
            spm_free_vc_nhi_id(pst_logical_port->logic_port_info_cb.next_hop_id);
        }
        
        pst_logical_port->logic_port_info_cb.next_hop_id = 0;
        
        if (NULL != pst_logical_port2) 
        {  
            pst_logical_port2->logic_port_info_cb.vpn_type = 0;
            pst_logical_port2->logic_port_info_cb.vpn_id = 0;
            pst_logical_port2->logic_port_info_cb.vp_idx = 0;
            pst_logical_port2->logic_port_info_cb.next_hop_counter_id = 0;
            
            if (0 != pst_logical_port2->logic_port_info_cb.next_hop_id)
            {
                spm_free_vc_nhi_id(pst_logical_port2->logic_port_info_cb.next_hop_id);
            }
            
            pst_logical_port2->logic_port_info_cb.next_hop_id = 0;
        }
    }
    else 
    {  
        ret = LOG_PORT_L2VPN_DRV_DEL_PROC_CALL_DRV_FAIL;
        spm_l2_record_err_code_and_three_value(
            ret,
            pst_l2vpn_data->vpn_type, 
            pst_l2vpn_data->vpn_id, 
            pst_l2vpn_data->uni_no);      
        goto EXIT_LABEL;
    } 
#endif

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
   函 数 名  : spm_logical_port_apply_del_l2vpn
   功能描述  : 5)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_del_l2vpn(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data)
{
    NBB_INT func_ret = SUCCESS;   
    NBB_INT ret = SUCCESS;
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_CB *pst_vpls = NULL;
    SPM_VP_CB *pst_vp = NULL;  
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;
    NBB_INT vp_num = 0;
    
    //可能存在的主备中的另一个   
    SPM_LOGICAL_PORT_CB *pst_logical_port2 = NULL;
    NBB_ULONG port_index = 0;   
    
    NBB_TRC_ENTRY("spm_logical_port_apply_del_l2vpn"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_L2VPN_APP_DEL_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_l2vpn_data)
    {
        ret = LOG_PORT_L2VPN_APP_DEL_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->logic_l2_cfg_cb)
    {
        ret = LOG_PORT_L2VPN_APP_DEL_PROC_TLV_LOGL2_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    } 
    
    if (0 == pst_logical_port->logic_port_info_cb.vp_idx)
    {
        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VP_IS_ZERO;
        spm_l2_record_err_code(ret); 
        goto EXIT_LABEL;
    }
    
    pst_vp = AVLL_FIND(SHARED.vp_tree, &pst_l2vpn_data->uni_no);
    
    //VP不存在
    if (NULL == pst_vp) 
    {
        ret = LOG_PORT_L2VPN_APP_DEL_PROC_FIND_VP_CB_FAIL;
        spm_l2_record_err_code(ret);         
        goto EXIT_LABEL;
    }
    
    /* vpn类型为VPWS*/
    if(L2VPN_VPWS == pst_l2vpn_data->vpn_type)
    {
        pst_vpws = AVLL_FIND(SHARED.vpws_tree, &pst_l2vpn_data->vpn_id);
        
        if (NULL == pst_vpws)
        {
            ret = LOG_PORT_L2VPN_APP_DEL_PROC_FIND_VPWS_CB_FAIL;
            spm_l2_record_err_code(ret); 
            goto EXIT_LABEL;
        } 
              
         /* 为UNI侧VP*/
        if (pst_vpws->uni_no == pst_l2vpn_data->uni_no) 
        {             
            //删除主用
            if (STA_MAIN == pst_l2vpn_data->master_backup) 
            {  
                //只有主用
                if (0 == pst_vpws->port_index_bak) 
                {  
                    func_ret = spm_logical_port_drv_del_l2vpn(pst_logical_port,pst_l2vpn_data, pst_vpws);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpws->port_index = 0;
                        pst_vpws->uni_no = 0;
                        pst_vpws->uni_num--;
                        
                        //从树中删除节点
                        AVLL_DELETE(SHARED.vp_tree, pst_vp->spm_vp_node);

                        //释放节点内存空间
                        spm_free_vp_cb(pst_vp);                       
                    }
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPWS_CALL_DRV_DEL_MAIN_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }
                }
                else
                {
                    func_ret = spm_logical_port_drv_add_uni_to_vpn(
                        pst_logical_port,
                        pst_l2vpn_data,
                        pst_logical_port->logic_l2_cfg_cb);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpws->port_index = 0;
                        pst_vp->port_index = 0;
                    }
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPWS_CALL_DRV_MOD_VP_MAIN_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                   
                }            
            }
            else 
            {  
                //只有备用
                if (0 == pst_vpws->port_index) 
                {  
                    func_ret = spm_logical_port_drv_del_l2vpn(pst_logical_port,pst_l2vpn_data, pst_vpws);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpws->port_index_bak = 0;
                        pst_vpws->uni_no = 0;
                        pst_vpws->msp_id = 0;
                        pst_vpws->uni_num--;
                        
                         //从树中删除节点
                        AVLL_DELETE(SHARED.vp_tree, pst_vp->spm_vp_node);

                        //释放节点内存空间
                        spm_free_vp_cb(pst_vp);                         
                    }
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPWS_CALL_DRV_DEL_BAK_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                   
                }
                else
                {
                    func_ret = spm_logical_port_drv_add_uni_to_vpn(
                        pst_logical_port,
                        pst_l2vpn_data,
                        pst_logical_port->logic_l2_cfg_cb);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpws->port_index_bak = 0;
                        pst_vpws->msp_id = 0;
                        pst_vp->port_index_bak = 0;
                    }  
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPWS_CALL_DRV_MOD_VP_BAK_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                    
                }                  
            }            
        }
        
         /* 为UNI侧VP*/
        else if (pst_vpws->uni_no_2 == pst_l2vpn_data->uni_no) 
        {  
            //删除主用
            if (STA_MAIN == pst_l2vpn_data->master_backup) 
            {  
                //只有主用
                if (0 == pst_vpws->port_index_bak_2) 
                {  
                    func_ret = spm_logical_port_drv_del_l2vpn(pst_logical_port,pst_l2vpn_data, pst_vpws);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpws->port_index_2 = 0;
                        pst_vpws->uni_no_2 = 0;
                        pst_vpws->uni_num--;
                        
                        //从树中删除节点
                        AVLL_DELETE(SHARED.vp_tree, pst_vp->spm_vp_node);

                        //释放节点内存空间
                        spm_free_vp_cb(pst_vp);                       
                    } 
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPWS_CALL_DRV_DEL_MAIN2_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                    
                }
                else
                {
                    func_ret = spm_logical_port_drv_add_uni_to_vpn(
                        pst_logical_port,
                        pst_l2vpn_data,
                        pst_logical_port->logic_l2_cfg_cb);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpws->port_index_2 = 0;
                        pst_vp->port_index = 0;
                    }
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPWS_CALL_DRV_MOD_VP_MAIN2_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                     
                }                  
            }
            else 
            {  
                //只有备用
                if (0 == pst_vpws->port_index_2) 
                {  
                    func_ret = spm_logical_port_drv_del_l2vpn(pst_logical_port,pst_l2vpn_data, pst_vpws);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpws->port_index_bak_2 = 0;
                        pst_vpws->uni_no_2 = 0;
                        pst_vpws->msp_id_2 = 0;
                        pst_vpws->uni_num--;
                        
                        //从树中删除节点
                        AVLL_DELETE(SHARED.vp_tree, pst_vp->spm_vp_node);

                        //释放节点内存空间
                        spm_free_vp_cb(pst_vp);                        
                    }
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPWS_CALL_DRV_DEL_BAK2_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                      
                }
                else
                {
                    func_ret = spm_logical_port_drv_add_uni_to_vpn(
                        pst_logical_port,
                        pst_l2vpn_data,
                        pst_logical_port->logic_l2_cfg_cb);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpws->port_index_bak_2 = 0;
                        pst_vpws->msp_id_2 = 0;
                        pst_vp->port_index_bak = 0;
                    }
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPWS_CALL_DRV_MOD_VP_BAK2_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                      
                }                  
            }            

            spm_logical_port_get_vpws_vp_num(pst_vpws, &vp_num);

            //VPWS中已经无VP，此时需要把整条业务删除
            if(0 == vp_num)
            {
                //从树中删除节点
                AVLL_DELETE(SHARED.vpws_tree, pst_vpws->spm_vpws_node);

                //释放节点内存空间
                spm_vpws_free_cb(pst_vpws NBB_CCXT);                  
            }           
        }
    }  
    
    /* vpn类型为VPLS*/
    else if(L2VPN_VPLS == pst_l2vpn_data->vpn_type)
    {
        pst_vpls = AVLL_FIND(SHARED.vpls_tree, &pst_l2vpn_data->vpn_id);
        
        if (NULL == pst_vpls)
        {
            ret = LOG_PORT_L2VPN_APP_DEL_PROC_FIND_VPLS_CB_FAIL;
            spm_l2_record_err_code(ret); 
            goto EXIT_LABEL;
        } 
        
        if (NULL != pst_vpls)
        {
            pst_vpls_uni_cb = AVLL_FIND(pst_vpls->uni_cfg_tree, &pst_l2vpn_data->vpn_id);
            
            if (NULL == pst_vpls_uni_cb)
            {
                ret = LOG_PORT_L2VPN_APP_DEL_PROC_FIND_VPLS_UNI_CB_FAIL;
                spm_l2_record_err_code(ret); 
                goto EXIT_LABEL;
            }
            
            //删除主用
            if (STA_MAIN == pst_l2vpn_data->master_backup) 
            {  
                //只有主用,删除vp
                if (0 == pst_vpls_uni_cb->port_index_bak) 
                {
                	func_ret = spm_logical_Port_apply_del_vpls_uni(pst_logical_port,
                        pst_l2vpn_data);
                }
                
                else
                {
                    func_ret = spm_logical_port_drv_add_uni_to_vpn(
                        pst_logical_port,
                        pst_l2vpn_data,
                        pst_logical_port->logic_l2_cfg_cb);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpls_uni_cb->port_index = 0;
                        pst_vp->port_index = 0;
                    }
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPLS_CALL_DRV_MOD_VP_MAIN_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                      
                }            
            }
            else 
            {  
                //只有备用
                if (0 == pst_vpls_uni_cb->port_index)
                {
                	func_ret = spm_logical_Port_apply_del_vpls_uni(pst_logical_port,
                        pst_l2vpn_data);
                }
                
                else
                {
                    func_ret = spm_logical_port_drv_add_uni_to_vpn(
                        pst_logical_port,
                        pst_l2vpn_data,
                        pst_logical_port->logic_l2_cfg_cb);
                    
                    if(SUCCESS == func_ret)
                    {
                        pst_vpls_uni_cb->port_index_bak = 0;
                        pst_vp->port_index_bak = 0;
                    }
                    else
                    {
                        ret = LOG_PORT_L2VPN_APP_DEL_PROC_VPLS_CALL_DRV_MOD_VP_BAK_FAIL;
                        spm_l2_record_err_code(ret); 
                        goto EXIT_LABEL;                   
                    }                     
                }                  
            } 

            vp_num = pst_vpls->uni_num + pst_vpls->nni_num;

            if(0 == vp_num)
            {
                //从树中删除节点
                AVLL_DELETE(SHARED.vpls_tree, pst_vpls->spm_vpls_node);

                //释放节点内存空间
                spm_free_vpls_cb(pst_vpls NBB_CCXT);               
            }
        }                  
    }   

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_Port_apply_del_vpls_uni
   功能描述  : 
   输入参数  : 
   输出参数  : 
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_Port_apply_del_vpls_uni(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data)
{
	SPM_VPLS_CB *pst_vpls = NULL;
	SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;
	SPM_VP_CB *pst_vp = NULL; 
	NBB_INT ret = SUCCESS;
	NBB_INT func_ret = SUCCESS;

	if(NULL == pst_logical_port)
	{
		ret = LOG_PORT_DEL_VPLS_UNI_LOGICAL_PORT_NULL;
        spm_l2_record_err_code(ret);       
        return ret; 
	}

	if(NULL == pst_l2vpn_data)
	{
		ret = LOG_PORT_DEL_VPLS_UNI_LOGICAL_PORT_L2VPN_NULL;
        spm_l2_record_err_code(ret);       
        return ret;
	}
	
	pst_vpls = AVLL_FIND(SHARED.vpls_tree, &pst_l2vpn_data->vpn_id);
        
    if (NULL == pst_vpls)
    {
        ret = LOG_PORT_DEL_VPLS_UNI_VPLS_NULL;
        spm_l2_record_err_code(ret); 
        return ret;
    } 

	pst_vp = AVLL_FIND(SHARED.vp_tree, &pst_l2vpn_data->uni_no);
	if(NULL == pst_vp)
	{
		ret = LOG_PORT_DEL_VPLS_UNI_VP_NODE_NULL;
        spm_l2_record_err_code(ret); 
        return ret;
	}
    
    
    pst_vpls_uni_cb = AVLL_FIND(pst_vpls->uni_cfg_tree, &pst_l2vpn_data->uni_no);
    
    if (NULL == pst_vpls_uni_cb)
    {
        ret = LOG_PORT_DEL_VPLS_UNI_VPLS_UNI_NULL;
        spm_l2_record_err_code(ret); 
        return ret;
    }
    

	func_ret = spm_logical_port_drv_del_l2vpn(pst_logical_port,pst_l2vpn_data, pst_vpls_uni_cb);

	/*如果主控先下发vpls的删除，则vpls的基本配置已经为null，且组播组已经被删除
	不需要再调用删除组播成员的接口,所以要先判断pst_vpls->basic_cfg_cb是否为null*/
	if((NULL != pst_vpls->basic_cfg_cb)
		&& (0 != pst_logical_port->logic_port_info_cb.mc_id))
	{
		ret = spm_logical_port_drv_del_arad_mc_uni_vp(pst_logical_port);
	}
		
    if(SUCCESS == func_ret)
    {
        //从树中删除节点
        AVLL_DELETE(pst_vpls->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node);

        //释放节点内存空间
        spm_free_vpls_uni_cb(pst_vpls_uni_cb);  
        pst_vpls->uni_num--;
        
         //从树中删除节点
        AVLL_DELETE(SHARED.vp_tree, pst_vp->spm_vp_node);

        //释放节点内存空间
        spm_free_vp_cb(pst_vp);  

		//释放该VP配置之后，删除交叉盘组播
		if((NULL != pst_vpls->basic_cfg_cb)
			&& (0 != pst_logical_port->logic_port_info_cb.mc_id))
		{
			ret = spm_vc_drv_del_src_mc_vp(pst_vpls,pst_logical_port->logic_port_info_cb.slot_id);
		}
    }

	return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_apply_set_mc_ipv4
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_set_mc_ipv4(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *pst_tlv_cfg)
{
    NBB_INT ret = SUCCESS;
    
    /* 逻辑接口的键值 */
    NBB_USHORT mc_ve_portid = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_apply_set_mc_ipv4");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->logic_l3_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    //调用dk接口
    //如果是VE口，根据驱动提供的算法: x*2 + 1 + 0x800，计算出port_id发给驱动
    if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
    {
        mc_ve_portid = (pst_logical_port->port_id * 2 + 1) + SPM_MC_VE_PORT_OFFSET;
        spm_l3_addonemcip(mc_ve_portid, pst_logical_port->logic_l3_cfg_cb->vrf_id, 0, &pst_tlv_cfg->mc_ipv4_addr);
    }
    else
    {
        spm_l3_addonemcip(pst_logical_port->port_id, pst_logical_port->logic_l3_cfg_cb->vrf_id, 0, &pst_tlv_cfg->mc_ipv4_addr);							
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
   函 数 名  : spm_logical_port_apply_set_mc_ipv6
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_set_mc_ipv6(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_MC_IPV6_DATA *pst_tlv_cfg)
{
    NBB_INT ret = SUCCESS;
    
    /* 逻辑接口的键值 */
    NBB_USHORT mc_ve_portid = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_apply_set_mc_ipv6");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->logic_l3_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    //调用dk接口
    //如果是VE口，根据驱动提供的算法: x*2 + 1 + 0x800，计算出port_id发给驱动
    if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
    {
        mc_ve_portid = (pst_logical_port->port_id * 2 + 1) + SPM_MC_VE_PORT_OFFSET;
        spm_l3_addonemcip(mc_ve_portid, pst_logical_port->logic_l3_cfg_cb->vrf_id, 0, &pst_tlv_cfg->mc_ipv6_addr);
    }
    else
    {
        spm_l3_addonemcip(pst_logical_port->port_id, pst_logical_port->logic_l3_cfg_cb->vrf_id, 0, &pst_tlv_cfg->mc_ipv6_addr);							
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
   函 数 名  : spm_logical_port_drv_set_ve_port_mac
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_set_ve_port_mac(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_VE_MAC_DATA *pst_tlv_cfg)
{
    NBB_INT  ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0;    
    NBB_BYTE ve_mac[6] = {0x00, 0x01, 0x002, 0x03, 0x04, 0x05};  
    NBB_ULONG long_mac[2] = {0};  
     
    NBB_TRC_ENTRY("spm_logical_port_drv_set_ve_port_mac"); 
    
    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }    
    
    if (NULL == pst_logical_port->phy_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    } 
        
#ifdef SPU      
                
    ve_mac[0] = pst_tlv_cfg->mac[0];
    ve_mac[1] = pst_tlv_cfg->mac[1];
    ve_mac[2] = pst_tlv_cfg->mac[2];
    ve_mac[3] = pst_tlv_cfg->mac[3];
    ve_mac[4] = pst_tlv_cfg->mac[4];
    ve_mac[5] = pst_tlv_cfg->mac[5]; 
    spm_l2_convert_mac_to_ulong(ve_mac,long_mac);    
   
    if ((ATG_DCI_TUNNEL != pst_logical_port->basic_cfg_cb->port_flag)
        && (ATG_DCI_LOOPBACK != pst_logical_port->basic_cfg_cb->port_flag)
        && (ATG_DCI_VE_PORT != pst_logical_port->basic_cfg_cb->port_sub_type))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret = ApiC3SetPortMac(c3_unit, ATG_DCI_IS_VE, 
                pst_logical_port->phy_cfg_cb->ve_group_id, ve_mac);
            
            if (SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                spm_l2_record_err_code_and_five_value(ret,c3_unit,
                    pst_logical_port->phy_cfg_cb->ve_group_id,
                    long_mac[0],long_mac[1],func_ret);
            }
        }
    }
#endif  

    EXIT_LABEL: NBB_TRC_EXIT();   
    return ret;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_drv_add_l3_intf
   功能描述  : 21)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_add_l3_intf(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_VLAN *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0;
    INTF_T  st_flow; /* INTF的结构体*/   
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_l3_intf"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_VLAN_DRV_ADD_L3_INTF_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = LOG_PORT_VLAN_DRV_ADD_L3_INTF_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = LOG_PORT_VLAN_DRV_ADD_L3_INTF_PROC_BASIC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_flow, 0, sizeof(INTF_T));
    
#ifdef SPU
    //1,ETH只在本槽位建流；
    //2，端口类型为LAG或者端口类型为VE；所有槽位都建流 
    //建INTF与建VP没有先后顺序关系 
    /* 配置解耦，8000的驱动接口保持不变*/
    if ((0 == pst_tlv_cfg->svlan_id) && (0 == pst_tlv_cfg->cvlan_id))
    {        
        if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L3_VE_PORT;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT;
        }
    }
    else if ((0 != pst_tlv_cfg->svlan_id) && (0 == pst_tlv_cfg->cvlan_id))
    {   
        if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L3_VE_PORT_OVID;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT_OVID;
        }
    }
    else if ((0 != pst_tlv_cfg->svlan_id) && (0 != pst_tlv_cfg->cvlan_id))
    {  
        if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L3_VE_PORT_OVID_IVID;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT_OVID_IVID;
        }
    }
    else
    {
        ret = LOG_PORT_VLAN_DRV_ADD_L3_INTF_PROC_TLV_DATA_ERR;
        spm_l2_record_err_code_and_two_value(ret,pst_tlv_cfg->svlan_id,pst_tlv_cfg->cvlan_id);     
        goto EXIT_LABEL;      
    }
    
    if(NULL != pst_logical_port->logic_l3_cfg_cb)
    {
        st_flow.bindId = pst_logical_port->logic_l3_cfg_cb->vrf_id;

        //IPV4 URPF使能
        if (ATG_DCI_INTF_URPF_DEFAULT == pst_logical_port->logic_l3_cfg_cb->ipv4_urpf_mode)     //URPF缺省检查
        {
            st_flow.eIpv4UrpfMode = URPF_OFF;
        }
        else if (ATG_DCI_INTF_URPF_STRICT == pst_logical_port->logic_l3_cfg_cb->ipv4_urpf_mode)  //URPF严格检查
        {
            st_flow.eIpv4UrpfMode = URPF_STRICT;
        }
        else if (ATG_DCI_INTF_URPF_LOOSE == pst_logical_port->logic_l3_cfg_cb->ipv4_urpf_mode)  //URPF松散检查
        {
            st_flow.eIpv4UrpfMode = URPF_LOOSE;
        }

        //IPV6 URPF使能
        if (ATG_DCI_INTF_URPF_DEFAULT == pst_logical_port->logic_l3_cfg_cb->ipv6_urpf_mode)     //URPF缺省检查
        {
            st_flow.eIpv6UrpfMode = URPF_OFF;
        }
        else if (ATG_DCI_INTF_URPF_STRICT == pst_logical_port->logic_l3_cfg_cb->ipv6_urpf_mode)  //URPF严格检查
        {
            st_flow.eIpv6UrpfMode = URPF_STRICT;
        }
        else if (ATG_DCI_INTF_URPF_LOOSE == pst_logical_port->logic_l3_cfg_cb->ipv6_urpf_mode)  //URPF松散检查
        {
            st_flow.eIpv6UrpfMode = URPF_LOOSE;
        }

        //缺省路由检查使能
        if (ENABLE == pst_logical_port->logic_l3_cfg_cb->ipv4_default_check == ENABLE)
        {
            st_flow.flags |= INTF_V4_ALLOW_DEFAULT_ROUTE;
        }

        if (ENABLE == pst_logical_port->logic_l3_cfg_cb->ipv6_default_check == ENABLE)
        {
            st_flow.flags |= INTF_V6_ALLOW_DEFAULT_ROUTE;
        }

        //MPLS转发不使能
        if (DISABLE == pst_logical_port->logic_l3_cfg_cb->mpls_forward_enable)
        {
            st_flow.flags |= INTF_MPLS_DISABLE;
        }    
    }

    st_flow.eIntfType = INTF_TYPE_L3;    
    st_flow.mCardPort = pst_logical_port->port_id;      
    st_flow.mOvid= pst_tlv_cfg->svlan_id;
    st_flow.mIvid = pst_tlv_cfg->cvlan_id;
    
    if ((pst_logical_port->slot_id == SHARED.local_slot_id) 
        && ((ATG_DCI_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type) 
        || (ATG_DCI_SUB_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type)))
    {
        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
            func_ret = ApiC3AddIntf(pst_logical_port->unit_id, &st_flow);

            if (SUCCESS != func_ret)
            {
                ret = LOG_PORT_VLAN_DRV_ADD_L3_INTF_PROC_CALL_DRV_FAIL;    
                spm_l2_record_err_code_and_four_value(ret,
                    pst_logical_port->unit_id,
                    pst_tlv_cfg->svlan_id,
                    pst_tlv_cfg->cvlan_id,
                    func_ret);
                goto EXIT_LABEL;
            }
               
            pst_logical_port->temp_intf_pos_id[pst_logical_port->unit_id] = st_flow.posId;			               
    } 
    
    else if ((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type)
        || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret = SUCCESS;

            //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
            //第二片C3上，则intf2_pos_id有效
            //ve和LAG，两片C3上都建
            func_ret = ApiC3AddIntf(c3_unit, &st_flow);

            if (SUCCESS != func_ret)
            {
                ret = LOG_PORT_VLAN_DRV_ADD_L3_INTF_PROC_CALL_DRV2_FAIL;    
                spm_l2_record_err_code_and_four_value(ret,
                    c3_unit,
                    pst_tlv_cfg->svlan_id,
                    pst_tlv_cfg->cvlan_id,
                    func_ret);
                goto EXIT_LABEL;
            }
               
            pst_logical_port->temp_intf_pos_id[c3_unit] = st_flow.posId;
        }				               
    } 
    
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}


/*****************************************************************************
   函 数 名  : spm_logical_port_drv_add_l2_intf
   功能描述  : 21)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_add_l2_intf(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_VLAN *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0;
    INTF_T  st_flow; /* INTF的结构体*/   
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_l2_intf"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_VLAN_DRV_ADD_L2_INTF_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = LOG_PORT_VLAN_DRV_ADD_L2_INTF_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = LOG_PORT_VLAN_DRV_ADD_L2_INTF_PROC_BASIC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_flow, 0, sizeof(INTF_T));
    
#ifdef SPU
    //1,ETH只在本槽位建流；
    //2，端口类型为LAG或者端口类型为VE；所有槽位都建流 
    //建INTF与建VP没有先后顺序关系 
    /* 配置解耦，8000的驱动接口保持不变*/
    if ((0 == pst_tlv_cfg->svlan_id) && (0 == pst_tlv_cfg->cvlan_id))
    {        
        if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L2_VE_PORT;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT;
        }
    }
    else if((0 != pst_tlv_cfg->svlan_id) && (0 == pst_tlv_cfg->cvlan_id))
    {   
        if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L2_VE_PORT_OVID;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT_OVID;
        }
    }
    else if ((0 != pst_tlv_cfg->svlan_id) && (0 != pst_tlv_cfg->cvlan_id))
    {  
        if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
        {        
            st_flow.mIntfMatchType = INTF_MATCH_L2_VE_PORT_OVID_IVID;
        }
        else
        {
            st_flow.mIntfMatchType = INTF_MATCH_PORT_OVID_IVID;
        }
    }
    else
    {
        ret = LOG_PORT_VLAN_DRV_ADD_L2_INTF_PROC_TLV_DATA_ERR;
        spm_l2_record_err_code_and_two_value(ret,pst_tlv_cfg->svlan_id,pst_tlv_cfg->cvlan_id);     
        goto EXIT_LABEL;      
    }

    if(NULL != pst_logical_port->logic_l2_cfg_cb)
    {
        if (ATG_DCI_TAGGED_VLAN == pst_logical_port->logic_l2_cfg_cb->recv_action)
        {        
            st_flow.flags |= INTF_PTAG_IDENTIFY_ENABLE;
        }             
    }             

    st_flow.eIntfType = INTF_TYPE_L2;    
    st_flow.mCardPort = pst_logical_port->port_id;      
    st_flow.bindId = pst_logical_port->logic_port_info_cb.vp_idx;
    st_flow.mOvid= pst_tlv_cfg->svlan_id;
    st_flow.mIvid = pst_tlv_cfg->cvlan_id;
    
    if ((pst_logical_port->slot_id == SHARED.local_slot_id) 
        && ((ATG_DCI_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type) 
        || (ATG_DCI_SUB_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type)))
    {
        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
            func_ret = ApiC3AddIntf(pst_logical_port->unit_id, &st_flow);

            if (SUCCESS != func_ret)
            {
                ret = LOG_PORT_VLAN_DRV_ADD_L2_INTF_PROC_CALL_DRV_FAIL;    
                spm_l2_record_err_code_and_four_value(ret,
                    pst_logical_port->unit_id,
                    pst_tlv_cfg->svlan_id,
                    pst_tlv_cfg->cvlan_id,
                    func_ret);
                goto EXIT_LABEL;
            }
               
            pst_logical_port->temp_intf_pos_id[pst_logical_port->unit_id] = st_flow.posId;			               
    } 
    
    else if ((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type)
        || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type))
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret = SUCCESS;

            //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
            //第二片C3上，则intf2_pos_id有效
            //ve和LAG，两片C3上都建
            func_ret = ApiC3AddIntf(c3_unit, &st_flow);

            if (SUCCESS != func_ret)
            {
                ret = LOG_PORT_VLAN_DRV_ADD_L2_INTF_PROC_CALL_DRV2_FAIL;    
                spm_l2_record_err_code_and_four_value(ret,
                    c3_unit,
                    pst_tlv_cfg->svlan_id,
                    pst_tlv_cfg->cvlan_id,
                    func_ret);
                goto EXIT_LABEL;
            }
               
            pst_logical_port->temp_intf_pos_id[c3_unit] = st_flow.posId;
        }				               
    } 
    
#endif

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_almpm_add_intf
   功能描述  : 21)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_almpm_add_intf(
    SPM_LOGICAL_PORT_CB *pst_logical_port)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_almpm_add_intf"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_BASIC_DRV_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

#ifdef SPU

    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;

        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if(0 != pst_logical_port->temp_intf_pos_id[c3_unit])
        {
            func_ret = almpm_addcfg_intf_onePos(pst_logical_port->port_index_key, 
                pst_logical_port->temp_intf_pos_id[c3_unit]);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_two_value(ret,c3_unit,func_ret); 
                goto EXIT_LABEL;
            }            
        }     
    }               
#endif	
          
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_apply_add_vlan
   功能描述  : 21)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_add_vlan(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_VLAN *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_logical_port_apply_add_vlan"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_VLAN_APP_ADD_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg)
    {
        ret = LOG_PORT_VLAN_APP_ADD_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = LOG_PORT_VLAN_APP_ADD_PROC_BASIC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    if (ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type)
    {
        spm_logical_port_drv_add_l2_intf(pst_logical_port,pst_tlv_cfg);
        spm_logical_port_almpm_add_intf(pst_logical_port);
    }
    else if(ATG_DCI_L3 == pst_logical_port->basic_cfg_cb->port_route_type)
    {
        spm_logical_port_drv_add_l3_intf(pst_logical_port,pst_tlv_cfg);
        spm_logical_port_almpm_add_intf(pst_logical_port);
    }
    else
    {
        ret = LOG_PORT_VLAN_APP_ADD_PROC_L2L3_TYPE_ERR;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
          
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_apply_del_vlan
   功能描述  : 21)
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_apply_del_vlan(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    SPM_TERMINAL_IF_CB *pst_tlv_cfg_cb)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0;
    
    NBB_TRC_ENTRY("spm_logical_port_apply_del_vlan"); 
    
    if (NULL == pst_logical_port)
    {
        ret = LOG_PORT_VLAN_APP_DEL_PROC_CFG_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_tlv_cfg_cb)
    {
        ret = LOG_PORT_VLAN_APP_DEL_PROC_TLV_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    } 
    
#ifdef SPU

    //删除intf
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {        
        if(0 != pst_tlv_cfg_cb->intf_pos_id[c3_unit])
        {
            func_ret = SUCCESS;
            func_ret = ApiC3DelIntf(c3_unit,pst_tlv_cfg_cb->intf_pos_id[c3_unit]);

            if(SUCCESS != func_ret)
            {
                ret = LOG_PORT_VLAN_APP_DEL_PROC_CALL_DRV_FAIL;          
                spm_l2_record_err_code_and_four_value(ret,c3_unit,
                    pst_tlv_cfg_cb->terminal_if_cfg.svlan_id,
                    pst_tlv_cfg_cb->terminal_if_cfg.cvlan_id,
                    func_ret);
            }
            
            func_ret = SUCCESS;
            func_ret = spm_logical_port_del_almpm_line_intf(pst_logical_port->port_index_key,
                pst_tlv_cfg_cb->intf_pos_id[c3_unit],1);

            if(SUCCESS != func_ret)
            {
                ret = LOG_PORT_VLAN_APP_DEL_PROC_CALL_ALMPM_FAIL;          
                spm_l2_record_err_code_and_four_value(ret,c3_unit,
                    pst_tlv_cfg_cb->terminal_if_cfg.svlan_id,
                    pst_tlv_cfg_cb->terminal_if_cfg.cvlan_id,
                    func_ret);
            }           
        }
    }    
#endif 

    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;

}

/*****************************************************************************
   函 数 名  : spm_logical_port_drv_del_acl_vrrp
   功能描述  : 删除端口逻辑配置
   输入参数  : SPM_LOGICAL_PORT_CB* pst_logical_port
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数
   修改历史      :
   1.日    期   : 2016年1月25日
    作    者   : fansongbo
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT spm_logical_port_drv_del_acl_vrrp(SPM_LOGICAL_PORT_CB *pst_logical_port,
    NBB_ULONG icfg_pos)
{
    NBB_INT ret = SUCCESS;
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0; 
    VRRP_ACL_T st_vrrp; /* vrrp的结构体*/    
    NBB_ULONG long_mac[2] = {0};  
    
    NBB_TRC_ENTRY("spm_logical_port_drv_del_acl_vrrp");
    
    if (NULL == pst_logical_port)
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret);         
        goto EXIT_LABEL;
    }
    if (NULL == pst_logical_port->vip_vmac_cfg_cb[icfg_pos])
    { 
        ret = SPM_LOGICAL_PORT_DEL_CFG_ERR_PARAM_NULL;
        spm_l2_record_err_code(ret); 
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_vrrp, 0, sizeof(VRRP_ACL_T)); 
    
#ifdef SPU      
    spm_l2_convert_mac_to_ulong(&(pst_logical_port->vip_vmac_cfg_cb[icfg_pos]->mac[0]),long_mac);
 
    //删除acl VRRP
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {        
        if(0 != pst_logical_port->logic_port_info_cb.vip_vmac_posid[icfg_pos][c3_unit])
        {
            st_vrrp.posid = pst_logical_port->logic_port_info_cb.vip_vmac_posid[icfg_pos][c3_unit];
            func_ret = SUCCESS;
            func_ret = fhdrv_psn_acl_del_vrrp(c3_unit, &st_vrrp);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_DRV_DEL_INTF_FAIL;          
                spm_l2_record_err_code_and_four_value(ret,c3_unit,long_mac[0],long_mac[1],
                    pst_logical_port->logic_port_info_cb.vip_vmac_posid[icfg_pos][c3_unit],
                    func_ret);
            }
            else
            {
                pst_logical_port->logic_port_info_cb.vip_vmac_posid[icfg_pos][c3_unit] = 0;
            }
        }           
    }      
#endif        

    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   函 数 名  : spm_logical_port_add_logic_up_usr_qos_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_logic_up_usr_qos_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_add_logic_acl_node
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_add_logic_acl_node(NBB_ULONG port_index, SPM_LOGICAL_PORT_CB *pst_logical_port)
{
 return 0;
}

/*****************************************************************************
   函 数 名  : spm_logical_port_del_port_to_vpn
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_del_port_to_vpn(
    NBB_BYTE uc_vpn_type,
    NBB_USHORT us_vpn_id,
    NBB_ULONG vp_idx)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_INT ret = SUCCESS;
    
    NBB_BYTE unit = 0;

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_TRC_ENTRY("spm_logical_port_drv_del_port_to_vpn");

    
    if (0 == vp_idx)
    {
        OS_PRINTF("***ERROR***(%s:%d)vp_idx=%d\n",
            __FUNCTION__, __LINE__,vp_idx);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)vp_idx=%d\n",
            __FUNCTION__, __LINE__,vp_idx);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
    
        goto EXIT_LABEL;
    }

    /* delete vp (hdjia) */
    //ret = fhdrv_psn_l2_del_vport(unit, vp_idx);

    if (SUCCESS != ret)
    {
        OS_PRINTF("***ERROR***(%s:%d)fhdrv_psn_l2_del_vport,ret=%d,vpn_type=%d,"
            "vpn_id=%d,vp_idx=%d\n",
            __FUNCTION__, __LINE__,ret,uc_vpn_type,us_vpn_id,vp_idx);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)fhdrv_psn_l2_del_vport,ret=%d,vpn_type=%d,"
            "vpn_id=%d,vp_idx=%d\n",
            __FUNCTION__, __LINE__,ret,uc_vpn_type,us_vpn_id,vp_idx);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_DRV;
    
        goto EXIT_LABEL;
    }
    else
    {

        OS_SPRINTF(uc_message, "***DEBUG***(%s:%d)fhdrv_psn_l2_del_vport,ret=%d,vpn_type=%d,"
            "vpn_id=%d,vp_idx=%d\n",
            __FUNCTION__, __LINE__,ret,uc_vpn_type,us_vpn_id,vp_idx);
        
        BMU_SLOG(BMU_DEBUG, SPM_L2_LOG_DIR, uc_message);
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
   函 数 名  : spm_logical_port_mod_vp
   功能描述  : 
   输入参数  : NBB_ULONG port_index
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月15日
    作    者   : lish
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_mod_vp(SPM_VPN_VP_CB *pst_vp_data)
{
    return 0;
}




/*****************************************************************************
 函 数 名  : spm_logical_port_drv_get_vp_basic_info
 功能描述  : 根据逻辑口基本配置、物理接口配置、l2cfg、l2vpn子配置获取创建二
             层vp最基本的信息，供vp add 、mod操作公用
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data  
             ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月17日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_get_vp_basic_info(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data,
    VPORT_T *pst_vport      /*  返回创建vp所需要的基本信息(hdjia) */
    )
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_INT ret = SUCCESS;
    

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_TRC_ENTRY("spm_logical_port_drv_get_vp_basic_info");

    if ((NULL == pst_logical_port) 
        || (NULL == pst_logic_l2_data)
        || (NULL == pst_l2vpn_data)
        || (NULL == pst_vport)
        )
    {
        OS_PRINTF("***ERROR***(%s:%d)pst_logical_port addr=%#x, "
            "pst_logic_l2_data addr=%#x, pst_l2vpn_data=%#x, pst_vport=%#x\n",
            __FUNCTION__, __LINE__, pst_logical_port, pst_logic_l2_data,
            pst_l2vpn_data, pst_vport);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)pst_logical_port addr=%#x, "
            "pst_logic_l2_data addr=%#x, pst_l2vpn_data=%#x, pst_vport=%#x\n",
            __FUNCTION__, __LINE__, pst_logical_port, pst_logic_l2_data,
            pst_l2vpn_data, pst_vport);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
		        
        goto EXIT_LABEL;
    }

    

    //pst_vport->fwd_id = pst_l2vpn_data->uni_no;


    
    //pst_vport->vpn_id = pst_l2vpn_data->vpn_id;

    /* VPORT 入口flag设置 (hdjia) */
    pst_vport->flags |= VPORT_COUNTER_ON;

    /* vport ptag设置 (hdjia) */
    switch (pst_logic_l2_data->recv_action)
    {
        case ATG_DCI_RAW_ETH:
            
            //pst_vport->ptag_action.type = PTAG_TYPE_RAW;
            break;
            
        case ATG_DCI_TAGGED_VLAN: 
            
            //pst_vport->ptag_action.type = PTAG_TYPE_TAGGED;             
            break;
        default:
            break;        
    }


    /* vlan 发送tag行为及赋值 驱动确认,转发忽略----------- (hdjia) */
    switch (pst_logical_port->logic_l2_cfg_cb->send_tag_action)
    {
        case ATG_DCI_ADD_VID_PRI:
            
            break;

        /* flag ----- (hdjia) */
        case ATG_DCI_DEL_VID_PRI:

            //pst_vport->ptag_action.vlan = 0;
            break;
        default:
            break;       
    }

    /* vlan tag发送方向赋值 (hdjia) */

    
    
   
    /* 子接口类型设置 (hdjia) */

    
    //else if (SUB_IF_QINQ_STACK == pst_logical_port->basic_cfg_cb->sub_if_type)
    {
        //pst_vport->term_type = INTF_TERM_QINQ_STACKING;

        /* stack模式协议不支持，vlan swap 设备不支持-------------- (hdjia) */
        //pst_vport->stacking_vid = 0;
    }

    /* 保护类型参数传递，后期需考虑 ，是否在函数入口传入-------(hdjia) */
    //pst_vport->prot_type = VP_PROT_TYPE_NONE;

    /* 保护frrid参数传递，后期需考虑 ，是否在函数入口传入，向驱动申请-------(hdjia) */
    //pst_vport->frr_id = 0;
    
    //pst_vport->logic_cvt_id = pst_logical_port->logic_cvt_id;


    /* 下一跳出口属性设置 (hdjia) */    
    //物理接口 或 子接口
    if ((ATG_DCI_SUB_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type)
        || (ATG_DCI_INTFACE == pst_logical_port->basic_cfg_cb->port_sub_type))
    {        
        //pst_vport->next_hop.nh_intf.tx_port.type = PORT_TYPE_ETH;
    }
    else if (ATG_DCI_PVE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
    {        
        //pst_vport->next_hop.nh_intf.tx_port.type = PORT_TYPE_PVE;
    }
    
    //虚拟以太网接口
    else if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
    {        
        //pst_vport->next_hop.nh_intf.tx_port.type = PORT_TYPE_L2VE;
    }
    else if (ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type)
    {        
        //pst_vport->next_hop.nh_intf.tx_port.type = PORT_TYPE_LAG;
    }

    //CES UNI
    if(ATG_DCI_CES == pst_logical_port->basic_cfg_cb->port_flag)
    {        
        /* 需要添加ces标签 -----------------(hdjia) */
        //pst_vport->match_label = 0;      
        
        /* 出方向相关设置 (hdjia) */
        //pst_vport->next_hop.flags |= NEXT_HOP_CES;   
        
        //pst_vport->next_hop.nh_intf.tx_port.type = PORT_TYPE_ETH;

        
        /* 需要添加ces标签 -----------------(hdjia) */
        //pst_vport->next_hop.tx_lable[0].label_un.tlabel.label = 0;   

        
        /* 优先级决定 -----------------(hdjia) */
        //pst_vport->next_hop.tx_lable[0].label_un.tlabel.exp = 7;  
        
        //pst_vport->next_hop.tx_lable[0].label_un.tlabel.bottom = 1;  
        
        //pst_vport->next_hop.tx_lable[0].label_un.tlabel.ttl = 255;  

        
        /*  ces收发控制字设置 -----------------(hdjia) */
    }  

    
    //pst_vport->next_hop.nh_intf.tx_port.slot = pst_logical_port->slot_id;
    
    //pst_vport->next_hop.nh_intf.tx_port.port = pst_logical_port->port_id;

    /* next_hop_id 驱动返回还是应用层传递----------- (hdjia) */

/* nni 侧 vp 使用-------------------- (hdjia) */
#if 0
    st_uni_vport.next_hop.nh_intf.control_vid = 
        pst_logical_port->basic_cfg_cb->control_vid;
    
    /* --------- (hdjia) */
    st_uni_vport.next_hop.nh_intf.nhi_ip_addr = 0;
    
    /* 各业务类型， 控制字 flag 设置------------- (hdjia) */    
    st_uni_vport.next_hop.flags |= 0;
#endif
   
    //pst_vport->next_hop.flags |= NEXT_HOP_COUNTER_ON;

    //如果是VPLS业务，需要配置MCA学习和水平分割
    if (L2VPN_VPLS == pst_l2vpn_data->vpn_type)
    {
        //MAC学习开关
        if (pst_logical_port->logic_l2_cfg_cb->mac_learning_enable == ENABLE)
        {
            /* 驱动标志位----------------- (hdjia) */
            //pst_vport->flags |= VPORT_MAC_LEARN_PRI_SET;
        }

        //水平分割开关
        if (pst_logical_port->logic_l2_cfg_cb->split_horizon_enable == ENABLE)
        {
            //pst_vport->flags |= VPORT_SPLIT_HORIZON;
        }
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_port_to_vpn_wait_driver
 功能描述  : 适配层添加驱动创建uni vp；vp创建后，需将驱动返回的posid、count-
             erid、nexthopid存储
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data  
             ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月15日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_add_port_to_vpn(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_INT ret = SUCCESS;
    VPORT_T st_vport;
    
    NBB_BYTE unit = 0;

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_port_to_vpn");

    if ((NULL == pst_logical_port) 
        || (NULL == pst_logic_l2_data)
        || (NULL == pst_l2vpn_data))
    {
        OS_PRINTF("***ERROR***(%s:%d)pst_logical_port addr=%#x, "
            "pst_logic_l2_data addr=%#x, pst_l2vpn_data=%#x\n",
            __FUNCTION__, __LINE__, pst_logical_port, pst_logic_l2_data,
            pst_l2vpn_data);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)pst_logical_port addr=%#x, "
            "pst_logic_l2_data addr=%#x, pst_l2vpn_data=%#x\n",
            __FUNCTION__, __LINE__, pst_logical_port, pst_logic_l2_data,
            pst_l2vpn_data);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
		        
        goto EXIT_LABEL;
    }

    

    //初始化结构t_vport_uni todo
    OS_MEMSET(&st_vport, 0, sizeof(VPORT_T));

    //ret = fhdrv_psn_l2_init_vport(&st_vport);

    /* 调用通用接口获取vp信息 (hdjia) */
    ret = spm_logical_port_drv_get_vp_basic_info(
        pst_logical_port, pst_logic_l2_data, pst_l2vpn_data, &st_vport);

    if (SUCCESS != ret)
    {
        OS_PRINTF("***ERROR***(%s:%d)spm_logical_port_drv_get_vp_basic_info ret=%d\n",
            __FUNCTION__, __LINE__, ret);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)spm_logical_port_drv_get_vp_basic_info ret=%d\n",
            __FUNCTION__, __LINE__, ret);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_ADAPT;
		        
        goto EXIT_LABEL;
    }
   

    /* 调用驱动接口 (hdjia) */
    //ret += fhdrv_psn_l2_add_vport(unit, &st_vport);       

    if (ret != SUCCESS)
    {
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_DRV;
		        
        goto EXIT_LABEL;
    }
    else
    {
       
        BMU_SLOG(BMU_DEBUG, SPM_L2_LOG_DIR, uc_message);

        /* 驱动返回值存储，只存vp整体的，不存储vp next_hop中信息----(hdjia) */
        //pst_logical_port->logic_port_info_cb.vp_idx = st_vport.pos_id;
        
        //pst_logical_port->logic_port_info_cb.next_hop_counter_id = st_vport.hw_counter_idx;

    }


EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}




/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_port_to_vpn_wait_driver
 功能描述  : 适配层添加驱动创建uni vp；vp创建后，需将驱动返回的posid、count-
             erid、nexthopid存储
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data  
             ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月15日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_mod_port_to_vpn(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pst_logic_l2_data,
    ATG_DCI_LOG_PORT_L2VPN_DATA *pst_l2vpn_data)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_INT ret = SUCCESS;
    VPORT_T st_vport;
    
    NBB_BYTE unit = 0;

    NBB_ULONG vp_idx = 0;           /* 对应于驱动，为posid (hdjia) */

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_port_to_vpn");

    if ((NULL == pst_logical_port) 
        || (NULL == pst_logic_l2_data)
        || (NULL == pst_l2vpn_data))
    {
        OS_PRINTF("***ERROR***(%s:%d)pst_logical_port addr=%#x, "
            "pst_logic_l2_data addr=%#x, pst_l2vpn_data=%#x\n",
            __FUNCTION__, __LINE__, pst_logical_port, pst_logic_l2_data,
            pst_l2vpn_data);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)pst_logical_port addr=%#x, "
            "pst_logic_l2_data addr=%#x, pst_l2vpn_data=%#x\n",
            __FUNCTION__, __LINE__, pst_logical_port, pst_logic_l2_data,
            pst_l2vpn_data);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
		        
        goto EXIT_LABEL;
    }

    

    //初始化结构t_vport_uni todo
    OS_MEMSET(&st_vport, 0, sizeof(VPORT_T));
    
    vp_idx = pst_logical_port->logic_port_info_cb.vp_idx;

    /* 适配层判断是的确可以mod，可根据适配设备进行修改 (hdjia) */
    if (0 == vp_idx)
    {
        OS_PRINTF("***ERROR***(%s:%d)can not mod, vp_idx=0\n",
            __FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)can not mod, vp_idx=0\n",
            __FUNCTION__, __LINE__);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_ADAPT;
		        
        goto EXIT_LABEL;
    }

    /* 根据pos id获取驱动已有的结构信息 (hdjia) */
    //ret = fhdrv_psn_l2_get_vport(unit, vp_idx, &st_vport);
    if (SUCCESS != ret)
    {
        OS_PRINTF("***ERROR***(%s:%d)fhdrv_psn_l2_get_vport ret=%d\n",
            __FUNCTION__, __LINE__, ret);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)fhdrv_psn_l2_get_vport ret=%d\n",
            __FUNCTION__, __LINE__, ret);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_DRV;
		        
        goto EXIT_LABEL;
    }
   

    /* 调用通用接口获取vp基本信息，获取到的新配置信息局部覆盖对应驱动信息 (hdjia) */
    ret = spm_logical_port_drv_get_vp_basic_info(
        pst_logical_port, pst_logic_l2_data, pst_l2vpn_data, &st_vport);

    if (SUCCESS != ret)
    {
        OS_PRINTF("***ERROR***(%s:%d)spm_logical_port_drv_get_vp_basic_info ret=%d\n",
            __FUNCTION__, __LINE__, ret);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d)spm_logical_port_drv_get_vp_basic_info ret=%d\n",
            __FUNCTION__, __LINE__, ret);
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_ADAPT;
		        
        goto EXIT_LABEL;
    }
   

    /* 调用驱动接口 (hdjia) */
    for (unit = 0; unit < SHARED.c3_num; unit++)
    {
        //ret += fhdrv_psn_l2_mod_vport(unit, vp_idx, &st_vport);       
    }

    if (ret != SUCCESS)
    {
        
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_CALL_DRV;
		        
        goto EXIT_LABEL;
    }
    else
    {

        
        BMU_SLOG(BMU_DEBUG, SPM_L2_LOG_DIR, uc_message);

        
        /* 驱动返回值存储，只存vp整体的，不存储vp next_hop中信息----(hdjia) */
        //pst_logical_port->logic_port_info_cb.vp_idx = st_vport.pos_id;
        
        //pst_logical_port->logic_port_info_cb.next_hop_counter_id = st_vport.hw_counter_idx;

    }


EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
 函 数 名  : spm_drv_create_mpls_vpn
 功能描述  : 根据vpn类型创建vpn
 输入参数  : NBB_BYTE uc_vpn_type  
             NBB_USHORT vpn_id     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月21日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_drv_create_mpls_vpn(
    NBB_BYTE uc_vpn_type,
    NBB_USHORT vpn_id)
{
    NBB_INT         ret = 0;    /* 依不同设备对unit再做区分处理 (hdjia) */
    NBB_CHAR        uc_message[SPM_MSG_INFO_LEN] = {0};

    NBB_INT         unit = 0;

    //L2_VPN_TYPE_E   drv_vpn_type = L2_VPN_TYPE_VPWS;

    

    NBB_TRC_ENTRY("spm_drv_creat_mpls_vpn");

    if (L2VPN_VPWS == uc_vpn_type)
    {
        //drv_vpn_type = L2_VPN_TYPE_VPWS;
    }
    else if (L2VPN_VPLS == uc_vpn_type)
    {
        //drv_vpn_type = L2_VPN_TYPE_VPLS;
    }
    else
    {
        OS_PRINTF("***ERROR***(%s:%d),uc_vpn_type=%d\n",
            __FUNCTION__,__LINE__,uc_vpn_type);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d),uc_vpn_type=%d\n",
            __FUNCTION__,__LINE__,uc_vpn_type);

        BMU_LOG(BMU_ERR,SPM_L2_LOG_DIR, uc_message);

        goto EXIT_LABEL;
    }

    //ret = fhdrv_psn_l2_create_vpn(unit, drv_vpn_type, vpn_id);

    if (SUCCESS != ret)
    {
        OS_PRINTF("***ERROR***(%s:%d),fhdrv_psn_l2_create_vpn,ret=%d,"
            "uc_vpn_type=%d,vpn_id=%d\n",
            __FUNCTION__,__LINE__,ret,uc_vpn_type,vpn_id);
        OS_SPRINTF(uc_message, "***ERROR***(%s:%d),fhdrv_psn_l2_create_vpn,ret=%d,"
            "uc_vpn_type=%d,vpn_id=%d\n",
            __FUNCTION__,__LINE__,ret,uc_vpn_type,vpn_id);
        BMU_LOG(BMU_ERR,SPM_L2_LOG_DIR, uc_message);

        goto EXIT_LABEL;
    }

EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_diff_serv
 功能描述  : 适配层处理"12）Diff-Serv配置"
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_PORT_DIFF_SERV_DATA *pst_diff_serv
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
NBB_INT spm_logical_port_drv_add_diff_serv(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *pst_tlv_cfg,
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb)
{
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0; 
    NBB_INT ret = SUCCESS;
    SUB_PORT st_sub_port;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_diff_serv");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_terminal_if_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_sub_port,0,sizeof(SUB_PORT));
    st_sub_port.slot = pst_logical_port->slot_id;
    st_sub_port.port = pst_logical_port->port_id;
    
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;

        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
            || ((ATG_DCI_LAG != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (c3_unit == pst_logical_port->unit_id)))
        {
            st_sub_port.unit = c3_unit;
            st_sub_port.posid = pst_terminal_if_cb->intf_pos_id[c3_unit];
            
            //func_ret = spm_ds_add_logic_intf_node(pst_logical_port->port_index_key,
                //&st_sub_port,pst_tlv_cfg);             

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->svlan,
                    pst_tlv_cfg->cvlan,func_ret); 
                goto EXIT_LABEL;
            }    
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_flow_diff_serv
 功能描述  : 适配层处理"13）流相关Diff-Serv配置L2"
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_PORT_DIFF_SERV_DATA *pst_diff_serv
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
NBB_INT spm_logical_port_drv_add_flow_diff_serv(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *pst_tlv_cfg,
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb)
{
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0; 
    NBB_INT ret = SUCCESS;
    SUB_PORT st_sub_port;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_flow_diff_serv");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_terminal_if_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_sub_port,0,sizeof(SUB_PORT));
    st_sub_port.slot = pst_logical_port->slot_id;
    st_sub_port.port = pst_logical_port->port_id;
    
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;

        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
            || ((ATG_DCI_LAG != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (c3_unit == pst_logical_port->unit_id)))
        {
            st_sub_port.unit = c3_unit;
            st_sub_port.posid = pst_terminal_if_cb->intf_pos_id[c3_unit];
            
            if(0 == st_sub_port.posid)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_three_value(ret,c3_unit,pst_tlv_cfg->svlan,
                    pst_tlv_cfg->cvlan); 
                goto EXIT_LABEL;
            }
            else
            {
                //func_ret = spm_ds_add_logic_flow_node(pst_logical_port->port_index_key,
                    //&st_sub_port,pst_tlv_cfg);             

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                    spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->svlan,
                        pst_tlv_cfg->cvlan,func_ret); 
                    goto EXIT_LABEL;
                }            
            }   
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_classify_qos
 功能描述  : 适配层处理"14）上话复杂流分类QOS策略配置"
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *pst_flow_diff_serv
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
NBB_INT spm_logical_port_drv_add_classify_qos(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *pst_tlv_cfg,
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb)
{

    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0; 
    NBB_INT ret = SUCCESS;
    SUB_PORT st_sub_port;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_classify_qos");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_terminal_if_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_sub_port,0,sizeof(SUB_PORT));
    st_sub_port.slot = pst_logical_port->slot_id;
    st_sub_port.port = pst_logical_port->port_id;
    
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;

        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
            || ((ATG_DCI_LAG != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (c3_unit == pst_logical_port->unit_id)))
        {
            st_sub_port.unit = c3_unit;
            st_sub_port.posid = pst_terminal_if_cb->intf_pos_id[c3_unit];
            
            if(ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                if(0 == st_sub_port.posid)
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                    spm_l2_record_err_code_and_three_value(ret,c3_unit,pst_tlv_cfg->svlan,
                        pst_tlv_cfg->cvlan); 
                    goto EXIT_LABEL;
                }
                else
                {
                    func_ret = spm_add_logic_flow_classify_node(pst_logical_port->port_index_key,
                        pst_tlv_cfg,&st_sub_port);             

                    if(SUCCESS != func_ret)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                        spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->svlan,
                            pst_tlv_cfg->cvlan,func_ret); 
                        goto EXIT_LABEL;
                    }            
                }  
            } 
            else if(ATG_DCI_L3 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                func_ret = spm_add_logic_flow_classify_node(pst_logical_port->port_index_key,
                    pst_tlv_cfg,&st_sub_port);             

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                    spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->svlan,
                        pst_tlv_cfg->cvlan,func_ret); 
                    goto EXIT_LABEL;
                } 
            }      
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_up_user_qos
 功能描述  : 适配层处理"15）上话用户QOS策略配置"
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *pst_flow_diff_serv
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
NBB_INT spm_logical_port_drv_add_up_user_qos(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_UP_USER_QOS_POLICY *pst_tlv_cfg,
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb)
{
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0; 
    NBB_INT ret = SUCCESS;
    SUB_PORT st_sub_port;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_up_user_qos");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_terminal_if_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_sub_port,0,sizeof(SUB_PORT));
    st_sub_port.slot = pst_logical_port->slot_id;
    st_sub_port.port = pst_logical_port->port_id;
    
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;

        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
            || ((ATG_DCI_LAG != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (c3_unit == pst_logical_port->unit_id)))
        {
            st_sub_port.unit = c3_unit;
            st_sub_port.posid = pst_terminal_if_cb->intf_pos_id[c3_unit];
            
            //func_ret = spm_add_logic_up_usr_qos_node(pst_logical_port->port_index_key,
                //&st_sub_port,pst_tlv_cfg);             

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->svlan,
                    pst_tlv_cfg->cvlan,func_ret); 
                goto EXIT_LABEL;
            }    
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_down_user_queue_qos
 功能描述  : 适配层处理"17）下话用户队列QOS策略配置",条目数1，依赖子tlv18
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_UP_USER_QOS_POLICY *pstUpUserQosData
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
NBB_INT spm_logical_port_drv_add_down_user_queue_qos(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0; 
    NBB_INT ret = SUCCESS;
    SUB_PORT st_sub_port;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_down_user_group_qos");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_logical_port->down_user_group_qos_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_sub_port,0,sizeof(SUB_PORT));
    st_sub_port.slot = pst_logical_port->slot_id;
    st_sub_port.port = pst_logical_port->port_id;
    st_sub_port.ovlan = pst_logical_port->basic_cfg_cb->control_vid;
    
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;

        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
            || ((ATG_DCI_LAG != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (c3_unit == pst_logical_port->unit_id)))
        {
            st_sub_port.unit = c3_unit;
            
            if(ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                st_sub_port.posid = pst_logical_port->logic_port_info_cb.vp_idx;
            } 
            else if(ATG_DCI_L3 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                st_sub_port.posid = 0;
            } 
            
            //func_ret = spm_hqos_add_usr_node(&st_sub_port,pst_logical_port->port_index_key,
                //pst_logical_port->down_user_group_qos_cfg_cb->user_group_index,pst_tlv_cfg);   

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_four_value(ret,c3_unit,st_sub_port.ovlan,func_ret); 
                goto EXIT_LABEL;
            }            
        }     
    }
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}



/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_down_user_qos
 功能描述  : 适配层处理"18)下话用户组QOS配置 ",条目数1
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
NBB_INT spm_logical_port_drv_add_down_user_group_qos(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_DOWN_USER_GROUP_QOS *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_down_user_group_qos");

    if (NULL == pst_logical_port)
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

    //func_ret = spm_hqos_add_group_node(pst_logical_port->slot_id,pst_logical_port->port_id,
        //pst_tlv_cfg->user_group_index,pst_logical_port->port_index_key);      

    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
        goto EXIT_LABEL;
    }   
            
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_flow_up_user_qos
 功能描述  : 适配层处理"19）流相关上话用户QOS策略配置L2 "
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_flow_up_user_qos
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
NBB_INT spm_logical_port_drv_add_flow_up_user_qos(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_tlv_cfg,
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb)
{
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0; 
    NBB_INT ret = SUCCESS;
    SUB_PORT st_sub_port;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_flow_up_user_qos");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_terminal_if_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_sub_port,0,sizeof(SUB_PORT));
    st_sub_port.slot = pst_logical_port->slot_id;
    st_sub_port.port = pst_logical_port->port_id;
    
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;

        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
            || ((ATG_DCI_LAG != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (c3_unit == pst_logical_port->unit_id)))
        {
            st_sub_port.unit = c3_unit;
            st_sub_port.posid = pst_terminal_if_cb->intf_pos_id[c3_unit];
            
            if(0 == st_sub_port.posid)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_three_value(ret,c3_unit,pst_tlv_cfg->svlan,
                    pst_tlv_cfg->cvlan); 
                goto EXIT_LABEL;
            }
            else
            {
                //func_ret = spm_add_flow_up_usr_qos_node(pst_logical_port->port_index_key,
                    //&st_sub_port,pst_tlv_cfg);             

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                    spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->svlan,
                        pst_tlv_cfg->cvlan,func_ret); 
                    goto EXIT_LABEL;
                }            
            }    
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}



/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_traffic_filter
 功能描述  : 适配层处理"20）包过滤器traffic_filter" L2 L3
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port              
             ATG_DCI_LOG_FLOW_UP_USER_QOS *pst_flow_up_user_qos
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
NBB_INT spm_logical_port_drv_add_traffic_filter(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *pst_tlv_cfg,
    SPM_TERMINAL_IF_CB *pst_terminal_if_cb)
{
    NBB_INT func_ret = SUCCESS;
    NBB_BYTE c3_unit =0; 
    NBB_INT ret = SUCCESS;
    SUB_PORT st_sub_port;
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_traffic_filter");

    if (NULL == pst_logical_port)
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
    
    if (NULL == pst_terminal_if_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }  
    
    if (NULL == pst_logical_port->basic_cfg_cb)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&st_sub_port,0,sizeof(SUB_PORT));
    st_sub_port.slot = pst_logical_port->slot_id;
    st_sub_port.port = pst_logical_port->port_id;
    
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        func_ret = SUCCESS;

        //对于ETH业务，业务在哪一片C3上，就把posid放在哪里，即如果业务在
        //第二片C3上，则intf2_pos_id有效
        //ve和LAG，两片C3上都建
        if((ATG_DCI_LAG == pst_logical_port->basic_cfg_cb->port_sub_type) 
            || (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
            || ((ATG_DCI_LAG != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (ATG_DCI_VE_PORT != pst_logical_port->basic_cfg_cb->port_sub_type) 
            && (c3_unit == pst_logical_port->unit_id)))
        {
            st_sub_port.unit = c3_unit;
            st_sub_port.posid = pst_terminal_if_cb->intf_pos_id[c3_unit];
            
            if(ATG_DCI_L2 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                if(0 == st_sub_port.posid)
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                    spm_l2_record_err_code_and_three_value(ret,c3_unit,pst_tlv_cfg->svlan,
                        pst_tlv_cfg->cvlan); 
                    goto EXIT_LABEL;
                }
                else
                {
                    func_ret = spm_add_logic_acl_node(pst_logical_port->port_index_key,
                        pst_tlv_cfg,&st_sub_port);             

                    if(SUCCESS != func_ret)
                    {
                        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                        spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->svlan,
                            pst_tlv_cfg->cvlan,func_ret); 
                        goto EXIT_LABEL;
                    }            
                }  
            } 
            else if(ATG_DCI_L3 == pst_logical_port->basic_cfg_cb->port_route_type)
            {
                func_ret = spm_add_logic_acl_node(pst_logical_port->port_index_key,
                    pst_tlv_cfg,&st_sub_port);             

                if(SUCCESS != func_ret)
                {
                    ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                    spm_l2_record_err_code_and_four_value(ret,c3_unit,pst_tlv_cfg->svlan,
                        pst_tlv_cfg->cvlan,func_ret); 
                    goto EXIT_LABEL;
                } 
            }     
        }
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_ds_l2
 功能描述  : 适配层处理"23)Diff-Serv配置L2（VP下话UNI侧）,条目数1
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
NBB_INT spm_logical_port_drv_add_ds_l2(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_DS_L2_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    SPM_QOS_LOGIC_UNIVP st_uni_vp;
     
    NBB_TRC_ENTRY("spm_logical_port_drv_add_down_user_group_qos");

    if (NULL == pst_logical_port)
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
    
    OS_MEMSET(&st_uni_vp,0,sizeof(SPM_QOS_LOGIC_UNIVP));
    st_uni_vp.vpn_id = pst_logical_port->logic_port_info_cb.vpn_id;
    st_uni_vp.vp_type = pst_logical_port->logic_port_info_cb.vpn_type;
    st_uni_vp.vp_posid = pst_logical_port->logic_port_info_cb.vp_idx;  
    
    //func_ret = spm_ds_add_logic_univp_node(pst_logical_port->port_index_key,
        //&st_uni_vp,pst_tlv_cfg);
        
    if(SUCCESS != func_ret)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);
        goto EXIT_LABEL;
    }   
            
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_logical_port_drv_add_car
 功能描述  : 适配层处理"24)car配置L2条目数1
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
NBB_INT spm_logical_port_drv_add_car(
    SPM_LOGICAL_PORT_CB *pst_logical_port,
    ATG_DCI_LOG_PORT_CAR_DATA *pst_tlv_cfg)
{
    NBB_INT func_ret = SUCCESS;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit =0;    
    SUB_PORT st_sub_port;
    ATG_DCI_VC_KEY vc_key; /* vc_key的结构体*/  
    
    NBB_TRC_ENTRY("spm_logical_port_drv_add_car");

    if (NULL == pst_logical_port)
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
    
    OS_MEMSET(&st_sub_port,0,sizeof(SUB_PORT));
    OS_MEMSET(&vc_key,0,sizeof(ATG_DCI_VC_KEY));
    st_sub_port.slot = pst_logical_port->slot_id;
    st_sub_port.port = pst_logical_port->port_id;

    func_ret = spm_get_vpws_vc_key_from_uni_index(
        pst_logical_port->port_index_key,&vc_key);
    
    if(SUCCESS != ret)
    {
        func_ret = SPM_LOGICAL_E_PARAM_NULL;
        spm_l2_record_err_code_and_one_value(ret,func_ret);       
        goto EXIT_LABEL;
    }
    else
    {
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            func_ret = SUCCESS;
            st_sub_port.unit = c3_unit;
            
            //func_ret = spm_add_pw_car_qos_node(pst_logical_port->port_index_key,
            //&vc_key,&st_sub_port,pst_tlv_cfg);             

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
                spm_l2_record_err_code_and_two_value(ret,c3_unit,func_ret);
                goto EXIT_LABEL;
            }
        }
    }
           
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
 函 数 名  : spm_logical_port_alloc_cvt_id
 功能描述  : 逻辑口获取全局唯一索引，规格由驱动确定
 输入参数  : NBB_ULONG *convert_id  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月4日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_alloc_cvt_id(NBB_ULONG *convert_id)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_UINT index = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_alloc_vc_nhi_id");
    
    if (NULL == convert_id)
    { 
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;       
        goto EXIT_LABEL;
    }
    
    //for (index = 0; index < SPM_LOGICAL_PORT_DRV_NUM; index++)
    {
        //if (g_logical_port_cvt_id[index] == ATG_DCI_USED)
        //{
            //continue;
       // }

       // *convert_id = index + 1;
        //g_logical_port_cvt_id[index] = ATG_DCI_USED;
       // break;
    }

    //分配满了，返回错误
   // if (SPM_LOGICAL_PORT_DRV_NUM == index)
    {
        ret = SPM_LOGICAL_PORT_DEL_INTF_ERR_PARAM_NULL;
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
 函 数 名  : spm_logical_port_free_cvt_id
 功能描述  : 删除逻辑口时，释放逻辑口转换给驱动的convert_id
 输入参数  : NBB_ULONG convert_id  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月4日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_free_cvt_id(NBB_ULONG convert_id)
{
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_logical_port_free_cvt_id");

   // if ((convert_id < 1) || (convert_id > SPM_LOGICAL_PORT_DRV_NUM))
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:输入的convert_id值超出范围！"));
        OS_PRINTF("***ERROR***(%s:%d):输入的convert_id=%d值超出范围!\n",
            __FUNCTION__, __LINE__, convert_id);

        ret = ERROR;
        goto EXIT_LABEL;
    }

    //g_logical_port_cvt_id[convert_id - 1] = ATG_DCI_UNUSED;

    //OS_PRINTF("iMcCud=%d mc_cud=%d\n", iMcCud, SHARED.mc_cud[iMcCud-1]);

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}







/*****************************************************************************
 函 数 名  : spm_logical_port_drv_set_vp_invalid
 功能描述  : 设置驱动vp为无效模式，确保业务不通
 输入参数  : SPM_LOGICAL_PORT_CB *pst_logical_port  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月6日
    作    者   : hdjia
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_logical_port_drv_set_vp_invalid(
    SPM_LOGICAL_PORT_CB *pst_logical_port
    )
{
    NBB_INT         ret = SUCCESS;
    
    NBB_INT         unit = 0;
    
    NBB_CHAR        uc_message[SPM_MSG_INFO_LEN];

    NBB_ULONG       vp_idx = 0;

    VPORT_T         st_vport;

    NBB_TRC_ENTRY("spm_logical_port_drv_set_vp_invalid");

    if ((NULL == pst_logical_port) || (NULL == pst_logical_port->l2vpn_cfg_cb))
    {
        OS_PRINTF(" ***ERROR***(%s:%d)pst_logical_port or l2vpn NULL!\n",
            __FUNCTION__, __LINE__);
        OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)pst_logical_port or l2vpn NULL!\n",
            __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
        ret = SPM_LOGICAL_E_PARAM_NULL;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&st_vport, 0, sizeof(VPORT_T));

    vp_idx = pst_logical_port->logic_port_info_cb.vp_idx;

    if (0 != vp_idx)    /* vp存在，设置vp为invalid (hdjia) */
    {
        //ret = fhdrv_psn_l2_get_vport(unit, vp_idx, &st_vport);

        if (SUCCESS != ret)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)fhdrv_psn_l2_get_vport ret=%d, vp_idx=%d\n",
                __FUNCTION__, __LINE__, ret, vp_idx);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)fhdrv_psn_l2_get_vport ret=%d, vp_idx=%d\n",
                __FUNCTION__, __LINE__, ret, vp_idx);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_CALL_DRV;
            goto EXIT_LABEL;
        }

        /* 置驱动vp为无效处理 (hdjia) */
        //st_vport.vpn_type = 0;

        //ret = fhdrv_psn_l2_mod_vport(unit, vp_idx, &st_vport);

        if (SUCCESS != ret)
        {
            OS_PRINTF(" ***ERROR***(%s:%d)fhdrv_psn_l2_mod_vport ret=%d, vp_idx=%d\n",
                __FUNCTION__, __LINE__, ret, vp_idx);
            OS_SPRINTF(uc_message, " ***ERROR***(%s:%d)fhdrv_psn_l2_mod_vport ret=%d, vp_idx=%d\n",
                __FUNCTION__, __LINE__, ret, vp_idx);
            BMU_SLOG(BMU_ERR, SPM_L2_LOG_DIR, uc_message);
            ret = SPM_LOGICAL_E_CALL_DRV;
            goto EXIT_LABEL;
        }
    }


    EXIT_LABEL:
        NBB_TRC_EXIT();
        return ret;
}









 
    
