/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_vpls_uc_drv.c
  版 本 号   : 初稿
  作    者   : zhangzhm
  生成日期   : 
  最近修改   :
  功能描述   : VPLS静态单播驱动处理文件
  函数列表   :

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*****************************************************************************
 函 数 名  : spm_vpls_uc_drv_del_static_mac
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
NBB_INT spm_vpls_uc_drv_del_static_mac(SPM_VPLS_UC_CB *pst_vpls_uc)
{
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0;

	if(NULL == pst_vpls_uc)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",DEL_VPLS_UC_STATIC_MAC_CFG_NULL);
		return DEL_VPLS_UC_STATIC_MAC_CFG_NULL;
	}

	#ifdef SPU
	for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
    {
        ret = ApiC3DelStaticMac(c3_unit, 
                                pst_vpls_uc->vpls_uc_key.mac_address, 
                                pst_vpls_uc->vpls_uc_key.vpls_id);

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",DEL_VPLS_UC_STATIC_MAC_ERR,ret);
			return DEL_VPLS_UC_STATIC_MAC_ERR;
		}
    }
	#endif

	return SUCCESS;
}


/*****************************************************************************
 函 数 名  : spm_set_uc_static_mac
 功能描述  : 设置静态组播UNI/NNI的MAC地址
 输入参数  : ATG_DCI_VPLS_UC_KEY *pstVplsUcKey         
             ATG_DCI_VPLS_UC_BASIC_DATA *pstBasicData  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月19日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_uc_drv_set_static_mac(ATG_DCI_VPLS_UC_KEY *pstVplsUcKey,
                                                    ATG_DCI_VPLS_UC_BASIC_DATA *pstBasicData)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_ULONG vp_idx = 0;
    NBB_ULONG ulPortIndex = 0;
    NBB_BYTE ucC3Unit = 0;
    ATG_DCI_VC_KEY stVcKey;

    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_VC_CB *pstVc = NULL;

    if (NULL == pstBasicData)
    {
        ret = SET_VPLS_UC_STATIC_MAC_BASIC_DATA_NULL;
        goto EXIT_LABEL;
    }
	
	if(NULL == pstVplsUcKey)
    {
        ret = SET_VPLS_UC_STATIC_MAC_KEY_NULL;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));

    if (ATG_DCI_UNI == pstBasicData->out_attrib)
    {
        ulPortIndex = pstBasicData->port_index;

        pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

        if (pstLogicalPort == NULL)
        {
            ret = SET_VPLS_UC_STATIC_LOG_PORT_NULL;
            goto EXIT_LABEL;
        }

        vp_idx = pstLogicalPort->logic_port_info_cb.vp_idx;
    }
    else if (ATG_DCI_NNI == pstBasicData->out_attrib)
    {
        stVcKey.vc_id = pstBasicData->vc_id;
        stVcKey.vc_type = pstBasicData->vc_type;
        stVcKey.peer_ip = pstBasicData->peer_ip;

        pstVc = AVLL_FIND(SHARED.vc_tree, &stVcKey);

        if (pstVc == NULL)
        {
            ret = SET_VPLS_UC_STATIC_VC_CFG_NULL;
            goto EXIT_LABEL;
        }

        vp_idx = pstVc->vc_info_cb.vp_idx;
    }

#ifdef SPU
    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret = ApiC3SetStaticMac(ucC3Unit, pstVplsUcKey->mac_address, pstVplsUcKey->vpls_id, 0, vp_idx);
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",SET_VPLS_UC_STATIC_DRV_ERR,ret);
		goto EXIT_LABEL;
        
    }

#endif

    EXIT_LABEL : 

    return ret;
}



















