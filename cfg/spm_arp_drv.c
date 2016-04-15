/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_arp_proc.c
   版 本 号   : 初稿
   作    者   : xiaoxiang
   生成日期   : 2012年11月10日
   最近修改   :
   功能描述   : ARP表配置处理
   函数列表   :
   修改历史   :
   1.日    期   : 2012年11月10日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*****************************************************************************
 函 数 名  : spm_arp_drv_add_arp
 功能描述  : 
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : zhangzhm
    修改内容   : 新生成函数
    

*****************************************************************************/
NBB_INT spm_arp_drv_add_arp(SPM_ARP_CB *pst_arp)
{
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    ARP_T st_arp;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0;
	SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
	SPM_PORT_INFO_CB stPortInfo;

	if ((NULL == pst_arp) || (NULL == pst_arp->basic_cfg_cb))
    {
        return ADD_ARP_DRV_ARP_CFG_IS_NULL;
        
    }

	OS_MEMSET(&st_arp,0,sizeof(ARP_T));
	OS_MEMSET(&stPortInfo,0,sizeof(SPM_PORT_INFO_CB));

	pst_logical_port = AVLL_FIND(v_spm_shared->logical_port_tree,&pst_arp->arp_key.index);
	if((NULL == pst_logical_port)
		|| (NULL == pst_logical_port->basic_cfg_cb))
	{
		return ADD_ARP_DRV_LOGICAL_PORT_IS_NULL;
	}

	ret= spm_get_portid_from_logical_port_index(pst_arp->arp_key.index,&stPortInfo);

	#ifdef SPU
    ret = ApiC3InitArp(&st_arp);

    if (SUCCESS != ret)
    {
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",ADD_ARP_DRV_INIT_ARP_ERR,ret);
		return ADD_ARP_DRV_INIT_ARP_ERR;
    }
    #endif

    //如果是虚拟以太网口
    if (ATG_DCI_VE_PORT == pst_logical_port->basic_cfg_cb->port_sub_type)
    {
        st_arp.mVeFlag = 1;
    }

    st_arp.mIpv4 = pst_arp->arp_key.ip_addr;
    st_arp.mCardPort = stPortInfo.port_id;
    st_arp.mSlot = stPortInfo.slot_id;
	
    st_arp.mVid = pst_logical_port->basic_cfg_cb->control_vid;	
	

    st_arp.sVid = pst_arp->basic_cfg_cb->svlan_id;
    st_arp.cVid = pst_arp->basic_cfg_cb->cvlan_id;

    OS_MEMCPY(st_arp.dmac, pst_arp->basic_cfg_cb->des_mac, ATG_DCI_MAC_LEN);

#ifdef SPU
    ret = SUCCESS;
    
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
    	if(0 == pst_arp->arp_pos_id)
    	{
    		ret += ApiC3AddArp(c3_unit, &st_arp);

			if(SUCCESS == ret)
			{
				pst_arp->arp_pos_id = st_arp.posId;
			}

			else
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",ADD_ARP_DRV_ADD_C3_ARP_ERR,ret);
				return ADD_ARP_DRV_ADD_C3_ARP_ERR;
			}
			
    	}

		else
		{
			st_arp.posId = pst_arp->arp_pos_id;
            ret += ApiC3ModArp(c3_unit, &st_arp);

			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",ADD_ARP_DRV_MOD_C3_ARP_ERR,ret);
				return ADD_ARP_DRV_MOD_C3_ARP_ERR;
			}
			
		}
        
    }

#endif

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_arp_drv_del_arp
 功能描述  : 
 输入参数  : 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : zhangzhm
    修改内容   : 新生成函数
    

*****************************************************************************/
NBB_INT spm_arp_drv_del_arp(SPM_ARP_CB *pst_arp)
{
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    ARP_T st_arp;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0;

	if ((NULL == pst_arp) || (NULL == pst_arp->basic_cfg_cb))
    {
        return DEL_ARP_DRV_ARP_CFG_IS_NULL;
        
    }

	#ifdef SPU
    for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
    	if (0 != pst_arp->arp_pos_id)
    	{
        	ret += ApiC3DelArp(c3_unit, pst_arp->arp_pos_id);
		}
		
        if (SUCCESS != ret)
        {
            SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",DEL_ARP_DRV_DEL_C3_ARP_ERR,ret);
			return DEL_ARP_DRV_DEL_C3_ARP_ERR;
        }
    }

	#endif

	return ret;
}













