/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_vc_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2012年9月18日
  最近修改   :
  功能描述   : VC表配置处理
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月18日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>


#ifdef VC_CFG


/**STRUCT-********************************************************************/

/**STRUCT+********************************************************************/
/* Structure: SPM_VPN_PORT_INFO_CB                                           */
/*                                                                           */
/* Description: VPN保存的一些组播配置和Bypass需要的配置                         */
/*                                                                           */
/* Added by xiaoxiang, 2012/4/14                                            */
/*****************************************************************************/



/**STRUCT-********************************************************************/


/**STRUCT+********************************************************************/
/* Structure: SPM_VC_CB                                                    */
/*                                                                           */
/* Description: VC表。                                                       */
/*                                                                           */
/* Added by xiaoxiang, 2012/9/18                                             */
/*****************************************************************************/

/*vc配置块打印开关*/
extern unsigned char g_vc_cfg_print_setting;
SPM_VPLS_CB * spm_alloc_vpls_cb(NBB_CXT_T);


/*****************************************************************************
 函 数 名  : spm_refresh_vc_nni_cfg
 功能描述  : 根据新的port_index反刷VC的相关信息
 输入参数  : pst_vc                     本地数据结构首地址
                 ul_portindex_new      新的port_index
 输出参数  : 无
 返 回 值  :  成功或者失败
 调用函数  : spm_vpn_refresh_vc_nni_cfg
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年10月21日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_refresh_vc_nni_cfg(SPM_VC_CB *pst_vc,NBB_ULONG ul_portindex_new)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;

    SPM_PORT_INFO_CB st_port_info;
    SPM_QOS_TUNNEL_KEY st_qos_tunkey;
    ATG_DCI_VC_UP_VPN_QOS_POLICY st_vc_upvpn_qos_policy;

    OS_MEMSET(&st_port_info, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&st_qos_tunkey, 0, sizeof(SPM_QOS_TUNNEL_KEY));
    OS_MEMSET(&st_vc_upvpn_qos_policy, 0, sizeof(ATG_DCI_VC_UP_VPN_QOS_POLICY));

    if (NULL == pst_vc)
    {
        OS_PRINTF("***ERROR***:Line=%d func=%s \n",__LINE__ ,__FUNCTION__);

        OS_SPRINTF(uc_message, "***ERROR***:Line=%d func=%s \n",__LINE__ ,__FUNCTION__);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        ret = ERROR;
        goto EXIT_LABEL;
    }

    st_qos_tunkey.flag   			= 1;
    st_qos_tunkey.type   		= pst_vc->basic_cfg_cb->lsp_option;
    st_qos_tunkey.tx_lsp.egress    = pst_vc->basic_cfg_cb->egress;
    st_qos_tunkey.tx_lsp.ingress   = pst_vc->basic_cfg_cb->ingress;
    st_qos_tunkey.tx_lsp.tunnelid  = pst_vc->basic_cfg_cb->tunnel_id;
    st_qos_tunkey.ftn.fec			= pst_vc->basic_cfg_cb->next_hop_ip;
    st_qos_tunkey.ftn.mask           = 32;
    st_qos_tunkey.ftn.vrfid            = 0;

    //如果VC存在，且已经属于VP中，删除VPN QOS
    if ((pst_vc->vc_info_cb.vp_idx != 0) && (pst_vc->up_vpn_cfg_cb != NULL))
    {
        st_vc_upvpn_qos_policy.node_index = pst_vc->up_vpn_cfg_cb->node_index;

        /*ret = spm_vc_qos_add_hqos_vc_node(pst_vc->vc_info_cb.slot_id, 
        pst_vc->vc_info_cb.port_id, 
        pst_vc->vc_info_cb.vp_idx, 
        pst_vc->vc_info_cb.if_main,
        &st_qos_tunkey,
        &pst_vc->vc_key,
        &st_vc_upvpn_qos_policy
        NBB_CCXT);*/

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",REFRESH_VC_NNI_DEL_HQOS_ERR,ret);
			ret = REFRESH_VC_NNI_DEL_HQOS_ERR;
			goto EXIT_LABEL;
		}
    }

    ret = spm_get_portid_from_logical_port_index(ul_portindex_new, &st_port_info NBB_CCXT);

    if (ret == SUCCESS)
    {
        pst_vc->vc_info_cb.port_index = ul_portindex_new;
        pst_vc->vc_info_cb.port_id = st_port_info.port_id;

        if (st_port_info.port_type == ATG_DCI_LAG)
        {
            pst_vc->vc_info_cb.slot_id = st_port_info.lag_slot_id;
        }
        else
        {
            pst_vc->vc_info_cb.slot_id = st_port_info.slot_id;

        }               
    }

    //如果VC存在，且已经属于VP中，更新VPN QOS
    if ((pst_vc->vc_info_cb.vp_idx != 0) && (pst_vc->up_vpn_cfg_cb != NULL))
    {	                    
        /*ret = spm_vc_qos_add_hqos_vc_node(pst_vc->vc_info_cb.slot_id, 
        pst_vc->vc_info_cb.port_id, 
        pst_vc->vc_info_cb.vp_idx, 
        pst_vc->vc_info_cb.if_main,
        &st_qos_tunkey,
        &pst_vc->vc_key,
        pst_vc->up_vpn_cfg_cb
        NBB_CCXT);*/

        if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",
				REFRESH_VC_NNI_ADD_HQOS_ERR,ret);
			
			ret = REFRESH_VC_NNI_ADD_HQOS_ERR;
			goto EXIT_LABEL;
		}
    }

EXIT_LABEL :

    return ret;
}


/*****************************************************************************
 函 数 名  : spm_vpn_refresh_vc_nni_cfg
 功能描述  : 根据新的port_index反刷VC的相关信息
 输入参数  : pst_vpinfo_refresh    反刷VC数据结构首地址
                 ul_portindex_new      新的port_index
 输出参数  : 无
 返 回 值  :  成功或者失败
 调用函数  : spm_l3_lspprot_update_vprsvp
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年10月21日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vpn_refresh_vc_nni_cfg(SPM_VP_INFO_REFRESH_CB *pst_vpinfo_refresh,
										  NBB_ULONG ul_portindex_new
										  NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_CB *pst_vpls = NULL;
    SPM_SWITCH_VC_CB *pst_switchvc = NULL;
    SPM_VC_CB *pst_vc = NULL;
    SPM_VPLS_NNI_CB *pst_vpls_nni = NULL;

    if (NULL == pst_vpinfo_refresh)
    {
    	SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",REFRESH_VC_NNI_VPINFO_NULL);
        ret = REFRESH_VC_NNI_VPINFO_NULL;
        goto EXIT_LABEL;
    }

    if (0 == ul_portindex_new)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",REFRESH_VC_NNI_PORTINFEX_NULL);
        ret = REFRESH_VC_NNI_PORTINFEX_NULL;
        goto EXIT_LABEL;
    }

    //反刷的VC不存在，退出处理
    pst_vc = AVLL_FIND(SHARED.vc_tree, &pst_vpinfo_refresh->vc_key);

    if ((NULL == pst_vc) || (NULL == pst_vc->basic_cfg_cb))
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",REFRESH_VC_NNI_VC_NULL);
        ret = REFRESH_VC_NNI_VC_NULL;
        goto EXIT_LABEL;
    }    

    //如果反刷的为VPWS
    if (L2VPN_VPWS == pst_vpinfo_refresh->vpn_type)
    {
        pst_vpws = AVLL_FIND(SHARED.vpws_tree, &pst_vpinfo_refresh->vpn_id);

        if (NULL == pst_vpws)
        {
            SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",REFRESH_VC_NNI_VC_VPWS_NULL);
	        ret = REFRESH_VC_NNI_VC_VPWS_NULL;
	        goto EXIT_LABEL;
        }

        if (VC_MAIN == pst_vpinfo_refresh->if_main)
        {
            if ((pst_vpws->vckey_main.vc_id == pst_vpinfo_refresh->vc_key.vc_id)
            && (pst_vpws->vckey_main.vc_type == pst_vpinfo_refresh->vc_key.vc_type)
            && (pst_vpws->vckey_main.peer_ip == pst_vpinfo_refresh->vc_key.peer_ip))
            {
                ret = spm_refresh_vc_nni_cfg(pst_vc,ul_portindex_new NBB_CCXT);
                if(SUCCESS != ret)
                {
                    goto EXIT_LABEL;
                }
            }

        }
        else if (VC_BACK == pst_vpinfo_refresh->if_main)
        {
            if ((pst_vpws->vckey_back.vc_id == pst_vpinfo_refresh->vc_key.vc_id)
            && (pst_vpws->vckey_back.vc_type == pst_vpinfo_refresh->vc_key.vc_type)
            && (pst_vpws->vckey_back.peer_ip == pst_vpinfo_refresh->vc_key.peer_ip))
            {
                ret = spm_refresh_vc_nni_cfg(pst_vc,ul_portindex_new NBB_CCXT);
                if(SUCCESS != ret)
                {
                    goto EXIT_LABEL;
                }
            }

        }
        

    }
    else if (L2VPN_VPLS == pst_vpinfo_refresh->vpn_type)
    {
        pst_vpls = AVLL_FIND(SHARED.vpls_tree, &pst_vpinfo_refresh->vpn_id);

        if ((NULL == pst_vpls) || (NULL == pst_vpls->basic_cfg_cb))
        {
        	SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",REFRESH_VC_NNI_VC_VPLS_NULL);
	        ret = REFRESH_VC_NNI_VC_VPLS_NULL;
	        goto EXIT_LABEL;
            
        }
        
        pst_vpls_nni = AVLL_FIND(pst_vpls->nni_cfg_tree, &pst_vc->vpn_property_cfg_cb->nni_no);
        
        if (NULL == pst_vpls_nni)   /* vpls中未找到该nni (zhangzhm) */
        {
            SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",REFRESH_VC_NNI_VC_VPLS_NNI_NULL);
	        ret = REFRESH_VC_NNI_VC_VPLS_NNI_NULL;
	        goto EXIT_LABEL;
        }         

        if (VC_MAIN == pst_vpinfo_refresh->if_main)
        {
            if ((pst_vpls_nni->vckey_main.vc_id == pst_vpinfo_refresh->vc_key.vc_id)
            && (pst_vpls_nni->vckey_main.vc_type == pst_vpinfo_refresh->vc_key.vc_type)
            && (pst_vpls_nni->vckey_main.peer_ip == pst_vpinfo_refresh->vc_key.peer_ip))
            {
                ret = spm_refresh_vc_nni_cfg(pst_vc,ul_portindex_new NBB_CCXT);
                if(SUCCESS != ret)
                {
                    goto EXIT_LABEL;
                }
            }
        }
        else if (VC_BACK == pst_vpinfo_refresh->if_main )
        {
            if ((pst_vpls_nni->vckey_back.vc_id == pst_vpinfo_refresh->vc_key.vc_id)
            && (pst_vpls_nni->vckey_back.vc_type == pst_vpinfo_refresh->vc_key.vc_type)
            && (pst_vpls_nni->vckey_back.peer_ip == pst_vpinfo_refresh->vc_key.peer_ip))
            {
                ret = spm_refresh_vc_nni_cfg(pst_vc,ul_portindex_new NBB_CCXT);
                if(SUCCESS != ret)
                {
                    goto EXIT_LABEL;
                }            
            }
        }
       
        
    }
    else if (L2VPN_MSPW == pst_vpinfo_refresh->vpn_type)
    {
        pst_switchvc = AVLL_FIND(SHARED.switch_vc_tree, &pst_vpinfo_refresh->vpn_id);

        if (NULL == pst_switchvc)
        {
            SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",REFRESH_VC_NNI_VC_MSPW_NULL);
	        ret = REFRESH_VC_NNI_VC_MSPW_NULL;
	        goto EXIT_LABEL;
        }

        if (VC_MAIN == pst_vpinfo_refresh->if_main)
        {
            if ((pst_switchvc->vckey_1.vc_id == pst_vpinfo_refresh->vc_key.vc_id)
            && (pst_switchvc->vckey_1.vc_type == pst_vpinfo_refresh->vc_key.vc_type)
            && (pst_switchvc->vckey_1.peer_ip == pst_vpinfo_refresh->vc_key.peer_ip))
            {
                ret = spm_refresh_vc_nni_cfg(pst_vc,ul_portindex_new NBB_CCXT);
                if(SUCCESS != ret)
                {
                    goto EXIT_LABEL;
                }
            }

            else if ((pst_switchvc->vckey_2.vc_id == pst_vpinfo_refresh->vc_key.vc_id)
            && (pst_switchvc->vckey_2.vc_type == pst_vpinfo_refresh->vc_key.vc_type)
            && (pst_switchvc->vckey_2.peer_ip == pst_vpinfo_refresh->vc_key.peer_ip))
            {
                ret = spm_refresh_vc_nni_cfg(pst_vc,ul_portindex_new NBB_CCXT);
                if(SUCCESS != ret)
                {
                    goto EXIT_LABEL;
                }
            }
        }
        
    }
    EXIT_LABEL :

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_del_vpws_vc_from_alm
   功能描述  : 从告警模块删除一条vc的条目
   输入参数  : stVcKey--要删除的vc的key
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史  :
   1.
   日    期  : 2014年5月22日
   作    者  : zhangsen
   修改内容  : 新生成函数

*****************************************************************************/
NBB_VOID spm_del_vpws_vc_from_alm(ATG_DCI_VC_KEY stVcKey 
    NBB_CCXT_T NBB_CXT)
{
return;

}


/*****************************************************************************
   函 数 名  : spm_alloc_l2_nni_cb
   功能描述  : 端口逻辑配置diff_serv结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_VPLS_NNI_CB
   调用函数  :
   被调函数  :(SPM_TRAFFIC_FILTER_CB*) AVLL_NEXT(pstLogicalPort->traffic_filter_tree,

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_VPLS_NNI_CB *spm_vpls_nni_alloc_cb(NBB_CXT_T NBB_CXT)
{
    SPM_VPLS_NNI_CB *pst_vpls_nni = NULL;

    NBB_TRC_ENTRY("spm_vpls_nni_alloc_cb");

    /* 分配一个新的vpls_nni配置条目。*/
    pst_vpls_nni = (SPM_VPLS_NNI_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_NNI_CB), NBB_NORETRY_ACT, MEM_SPM_VPLS_NNI_CB);
    
    if (NULL == pst_vpls_nni)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VPWS配置条目 */
    OS_MEMSET(pst_vpls_nni, 0, sizeof(SPM_VPLS_NNI_CB));

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pst_vpls_nni->spm_vpls_nni_handle = NBB_CREATE_HANDLE(pst_vpls_nni, HDL_SPM_VPLS_NNI_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_NNI_CB allocated at %p with handle %#lx",
        pst_vpls_nni, pst_vpls_nni->spm_vpls_nni_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_vpls_nni->spm_vpls_nni_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_vpls_nni);
}

/*****************************************************************************
   函 数 名  : spm_vpls_nni_free_cb
  功能描述  : 释放端口逻辑配置diff_serv节点的内存空间
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_DIFF_SERV_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年3月9日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_vpls_nni_free_cb(SPM_VPLS_NNI_CB *pst_vpls_nni NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_vpls_nni_free_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_vpls_nni, sizeof(SPM_VPLS_NNI_CB), MEM_SPM_VPLS_NNI_CB);

    if (NULL == pst_vpls_nni)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pst_vpws));
    NBB_TRC_FLOW((NBB_FORMAT "Free vpls_nni_id %d", pst_vpls_nni->nni_no));

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_vpls_nni->spm_vpls_nni_handle, HDL_SPM_VPLS_NNI_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_vpls_nni, MEM_SPM_VPLS_NNI_CB);
    pst_vpls_nni = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_init_vpws_tree
   功能描述  : 初始化存储VPWS配置的树
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_vpws_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vpws_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_VPWS_CB, vpws_id_key);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.vpws_tree, compare_ushort,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_VPWS_CB, spm_vpws_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   函 数 名  : spm_alloc_vpws_cb
   功能描述  : VPWS结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_VPWS_CB
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_VPWS_CB *spm_vpws_alloc_cb(NBB_CXT_T NBB_CXT)
{
    SPM_VPWS_CB *pst_vpws = NULL;

    NBB_TRC_ENTRY("spm_vpws_alloc_cb");

    /* 分配一个新的VPWS配置条目。*/
    pst_vpws = (SPM_VPWS_CB *)NBB_MM_ALLOC(sizeof(SPM_VPWS_CB), NBB_NORETRY_ACT, MEM_SPM_VPWS_CB);
    
    if (NULL == pst_vpws)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VPWS配置条目 */
    OS_MEMSET(pst_vpws, 0, sizeof(SPM_VPWS_CB));

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pst_vpws->spm_vpws_handle = NBB_CREATE_HANDLE(pst_vpws, HDL_SPM_VPWS_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPWS_CB allocated at %p with handle %#lx",
        pst_vpws, pst_vpws->spm_vpws_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_vpws->spm_vpws_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_vpws);
}

/*****************************************************************************
   函 数 名  : spm_free_vpws_cb
   功能描述  : 释放VPWS结构体的内存
   输入参数  : SPM_VPWS_CB *pstVpws NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_vpws_free_cb(SPM_VPWS_CB *pst_vpws NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_vpws_free_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_vpws, sizeof(SPM_VPWS_CB), MEM_SPM_VPWS_CB);

    if (NULL == pst_vpws)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pstVpws));
    NBB_TRC_FLOW((NBB_FORMAT "Free vpws_id %d", pst_vpws->vpws_id_key));

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_vpws->spm_vpws_handle, HDL_SPM_VPWS_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_vpws, MEM_SPM_VPWS_CB);
    pst_vpws = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_init_switch_vc_tree
 功能描述  : 初始化SWITCH VC表树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_switch_vc_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_switch_vc_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_SWITCH_VC_CB, switch_vc_key);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.switch_vc_tree, compare_ushort,
        (NBB_USHORT) avll_key_offset,
        (NBB_USHORT) NBB_OFFSETOF(SPM_SWITCH_VC_CB, spm_switch_vc_node));

    NBB_TRC_EXIT();    
}

/*****************************************************************************
 函 数 名  : spm_alloc_switch_vc_cb
 功能描述  : 分配SWITCH VC表结构体的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_SWITCH_VC_CB* spm_switch_vc_alloc_cb(NBB_CXT_T NBB_CXT)
{
    SPM_SWITCH_VC_CB *pst_switch_vc = NULL;

    NBB_TRC_ENTRY("spm_switch_vc_alloc_cb");

    /* 分配一个新的VC表配置条目。*/
    pst_switch_vc = (SPM_SWITCH_VC_CB *)NBB_MM_ALLOC(sizeof(SPM_SWITCH_VC_CB), 
        NBB_NORETRY_ACT, MEM_SPM_SWITCH_VC_CB);
    
    if (NULL == pst_switch_vc)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VC表配置条目 */
    OS_MEMSET(pst_switch_vc, 0, sizeof(SPM_SWITCH_VC_CB));

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pst_switch_vc->spm_switch_vc_handle = NBB_CREATE_HANDLE(pst_switch_vc, HDL_SPM_SWITCH_VC_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_SWITCH_VC_CB allocated at %p with handle %#lx",
        pst_switch_vc, pst_switch_vc->spm_switch_vc_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_switch_vc->spm_switch_vc_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pst_switch_vc);
}

/*****************************************************************************
 函 数 名  : spm_free_switch_vc_cb
 功能描述  : 释放SWITCH VC表结构体的内存空间
 输入参数  : SPM_SWITCH_VC_CB *pstSwitchVc  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_switch_vc_cb(SPM_SWITCH_VC_CB *pst_switch_vc NBB_CCXT_T NBB_CXT)
{

    NBB_TRC_ENTRY("spm_free_switch_vc_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_switch_vc, sizeof(SPM_SWITCH_VC_CB), MEM_SPM_SWITCH_VC_CB); 

    if (NULL == pst_switch_vc)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_SWITCH_VC_CB at %p", pstSwitchVc));  
    NBB_TRC_FLOW((NBB_FORMAT "Free ID %d", pst_switch_vc->switch_vc_key));
    
    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_switch_vc->spm_switch_vc_handle, HDL_SPM_SWITCH_VC_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_switch_vc, MEM_SPM_SWITCH_VC_CB);  
    pst_switch_vc = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}


/*****************************************************************************
 函 数 名  : spm_init_vc_tree
 功能描述  : 初始化存储VC表配置的树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_vc_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vc_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_VC_CB, vc_key);/*lint !e413 */
    AVLL_INIT_TREE(SHARED.vc_tree, spm_vc_key_compare,
        (NBB_USHORT) avll_key_offset,
        (NBB_USHORT) NBB_OFFSETOF(SPM_VC_CB, spm_vc_node));

    NBB_TRC_EXIT();    
}

/*****************************************************************************
 函 数 名  : spm_init_vc_tree
 功能描述  : 分配VC表结构体的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_VC_CB * spm_vc_alloc_cb(NBB_CXT_T NBB_CXT)
{
    SPM_VC_CB *pst_vc_tbl = NULL;

    NBB_TRC_ENTRY("spm_vc_alloc_cb");

    /* 分配一个新的VC表配置条目。*/
    pst_vc_tbl = (SPM_VC_CB *)NBB_MM_ALLOC(sizeof(SPM_VC_CB), NBB_NORETRY_ACT, MEM_SPM_VC_CB);
    
    if (NULL == pst_vc_tbl)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VC表配置条目 */
    OS_MEMSET(pst_vc_tbl, 0, sizeof(SPM_VC_CB));
    pst_vc_tbl->basic_cfg_cb = NULL;
    pst_vc_tbl->diff_serv_cfg_cb = NULL;
    pst_vc_tbl->up_vpn_cfg_cb = NULL;
    pst_vc_tbl->vpn_property_cfg_cb = NULL;
	pst_vc_tbl->connect_detect_cfg_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pst_vc_tbl->spm_vc_handle = NBB_CREATE_HANDLE(pst_vc_tbl, HDL_SPM_VC_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VC_CB allocated at %p with handle %#lx",
        pst_vc_tbl, pst_vc_tbl->spm_vc_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_vc_tbl->spm_vc_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pst_vc_tbl);
}


/*****************************************************************************
 函 数 名  : spm_vc_free_cb
 功能描述  : 释放VC表节点的内存空间
 输入参数  : SPM_VC_CB *pstVcTbl  
             NBB_CCXT_T NBB_CXT   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_vc_free_cb(SPM_VC_CB *pst_vc_tbl NBB_CCXT_T NBB_CXT)
{
    
    NBB_TRC_ENTRY("spm_vc_free_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_vc_tbl, sizeof(SPM_VC_CB), MEM_SPM_VC_CB); 

    if (NULL == pst_vc_tbl)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "Free VC_ID %d  PEER_IP %s  VC_TYPE %d", pst_vc_tbl->vc_key.vc_id,
        spm_set_ulong_to_ipv4(pst_vc_tbl->vc_key.peer_ip NBB_CCXT),
        pst_vc_tbl->vc_key.vc_type));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (NULL != pst_vc_tbl->basic_cfg_cb )
    {
        NBB_MM_FREE(pst_vc_tbl->basic_cfg_cb, MEM_SPM_VC_BASIC_CB);  
        pst_vc_tbl->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放Diff-Serv配置数据块。                                                */
    /***************************************************************************/
    if (NULL != pst_vc_tbl->diff_serv_cfg_cb)
    {
        NBB_MM_FREE(pst_vc_tbl->diff_serv_cfg_cb, MEM_SPM_VC_DIFF_SERV_CB);  
        pst_vc_tbl->diff_serv_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放上话VPN QOS策略配置数据块。                                         */
    /***************************************************************************/
    if (NULL != pst_vc_tbl->up_vpn_cfg_cb)
    {
        NBB_MM_FREE(pst_vc_tbl->up_vpn_cfg_cb, MEM_SPM_VC_UP_VPN_CB);  
        pst_vc_tbl->up_vpn_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放vpn 属性配置数据块。                                         */
    /***************************************************************************/
    if (NULL != pst_vc_tbl->vpn_property_cfg_cb)
    {
        NBB_MM_FREE(pst_vc_tbl->vpn_property_cfg_cb, MEM_SPM_VC_VPN_PROPER_CB);  
        pst_vc_tbl->vpn_property_cfg_cb = NULL;
    }  

	/***************************************************************************/
    /* 释放连通性检测配置数据块。                                         */
    /***************************************************************************/
    if (NULL != pst_vc_tbl->connect_detect_cfg_cb)
    {
        NBB_MM_FREE(pst_vc_tbl->connect_detect_cfg_cb, MEM_SPM_VC_CONNECT_DETECT_CB);  
        pst_vc_tbl->connect_detect_cfg_cb = NULL;
    }  

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_vc_tbl->spm_vc_handle, HDL_SPM_VC_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_vc_tbl, MEM_SPM_VC_CB);  
    pst_vc_tbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_alloc_vc_nhi_id
   功能描述  : 在64K的VC资源池中分配一个vc_nhi_id
   输入参数  : NBB_UINT *iVcNhiId
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :
    修改历史      :
  1.日    期   : 2015年9月21日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_alloc_nhi_id(NBB_UINT *vc_nhiid NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_UINT i = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_alloc_vc_nhi_id");

    for (i = 0; i < SPM_VC_NHI_ID_NUM; i++)
    {
        if (SHARED.vc_nhi_id[i] == ATG_DCI_USED)
        {
            continue;
        }

        *vc_nhiid = i + 1;
        SHARED.vc_nhi_id[i] = ATG_DCI_USED;
        break;
    }

    //分配满了，返回错误
    if (i == SPM_VC_NHI_ID_NUM)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:vc_nhi_id已经分配满了"));

        OS_PRINTF("***ERROR***:vc_nhi_id已经分配满了!\n");

        OS_SPRINTF(uc_message, "***ERROR***:vc_nhi_id已经分配满了!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

        ret = ERROR;
    }

    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_free_vc_nhi_id
   功能描述  : 释放分配的vc_nhi_id资源
   输入参数  : NBB_UINT iVcNhiId
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :
    修改历史      :
  1.日    期   : 2015年9月21日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_free_nhi_id(NBB_UINT iVcNhiId NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_free_vc_nhi_id");

    if ((iVcNhiId < 1) || (iVcNhiId > SPM_VC_NHI_ID_NUM))
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:输入的vc_nhi_id值超出范围！"));
        OS_PRINTF("***ERROR***:输入的vc_nhi_id值超出范围!\n");

        ret = ERROR;
        goto EXIT_LABEL;
    }

    SHARED.vc_nhi_id[iVcNhiId - 1] = ATG_DCI_UNUSED;

    //OS_PRINTF("iMcCud=%d mc_cud=%d\n", iMcCud, SHARED.mc_cud[iMcCud-1]);

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}




/*****************************************************************************
 函 数 名  : spm_vc_get_add_type
 功能描述  : 获取输入的vc条目，要添加的处理类型
 输入参数  : NBB_BYTE uc_vpn_type   
             SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 返回为直接添加或mod操作
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月28日
    作    者   : zhangzhm
    修改内容   : 新生成函数
  封装为函数作为获取判断类型，以免后期处理方式变化，只修改函数体内部即可

*****************************************************************************/
SPM_VC_ADD_TYPE_E spm_vc_get_add_type(NBB_BYTE vpn_type,NBB_USHORT vpn_id,NBB_USHORT vp_idx,
	SPM_VC_CB *pst_vc_cfg)
{
    NBB_INT ret = 0;    
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  
    NBB_INT vc_exist = 0;  
    SPM_VC_ADD_TYPE_E add_type = 0;

	SPM_VPWS_CB *pst_vpws_cb = NULL;
	SPM_VPLS_CB *pst_vpls_cb = NULL;
	SPM_VPLS_NNI_CB *pst_vpls_nni_cb = NULL;
	SPM_SWITCH_VC_CB *pst_mspw_cb = NULL;
    
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_TRC_ENTRY("spm_vc_get_add_type");
	
	if(NULL == pst_vc_cfg)
	{
		return VC_GET_ADD_TYPE_VC_NULL;
	}
	
	if(NULL == pst_vc_cfg->basic_cfg_cb)
	{
        return VC_GET_ADD_TYPE_VC_BASIC_NULL;
	}

	if(NULL == pst_vc_cfg->vpn_property_cfg_cb)
	{
		return VC_GET_ADD_TYPE_VC_VPN_NULL;
	}

    if(L2VPN_VPWS == vpn_type)
    {
    	pst_vpws_cb = AVLL_FIND(v_spm_shared->vpws_tree,&vpn_id);

		if(NULL == pst_vpws_cb)
		{
			return SPM_VC_ADD_TYPE_ADD;
		}


		//如果是NNI_VP
		if(0 == pst_vc_cfg->vpn_property_cfg_cb->bypass_dh_id)
		{
			//新数据和老数据不一致，返错,举例:试图向VPWS中添加两个NNI
			if((VC_MAIN == pst_vc_cfg->vpn_property_cfg_cb->vc_state)
				&& (0 != pst_vpws_cb->vckey_main.vc_id))
			{
				ret = spm_vc_key_compare(&pst_vc_cfg->vc_key,&pst_vpws_cb->vckey_main);
				if(0 != ret)
				{
					return VC_GET_ADD_TYPE_VPWS_VC_MAIN_NOT_EQUAL;
				}
			}

			if((VC_BACK == pst_vc_cfg->vpn_property_cfg_cb->vc_state)
				&& (0 != pst_vpws_cb->vckey_back.vc_id))
			{
				ret = spm_vc_key_compare(&pst_vc_cfg->vc_key,&pst_vpws_cb->vckey_back);
				if(0 != ret)
				{
					return VC_GET_ADD_TYPE_VPWS_VC_BACK_NOT_EQUAL;
				}
			}
		
			//其他情况，统一只看if_drv_nni_vp_exist
			if(DRV_NNI_VP_EXIST == pst_vpws_cb->if_drv_nni_vp_exist)
			{
				return SPM_VC_ADD_TYPE_MOD;
			}

			else
			{
				return SPM_VC_ADD_TYPE_ADD;
			}
		}

		//如果是BYPASS_VP
		else
		{
			if((0 != pst_vpws_cb->bypass_no)
				&& (0 != spm_vc_key_compare(&pst_vc_cfg->vc_key,&pst_vpws_cb->vckey_bypass)))
			{
				return VC_GET_ADD_TYPE_VPWS_BYPASS_NOT_EQUAL;
			}

			//其他情况，统一只看if_drv_nni_vp_exist
			if(DRV_NNI_VP_EXIST == pst_vpws_cb->if_drv_bypass_vp_exist)
			{
				return SPM_VC_ADD_TYPE_MOD;
			}

			else
			{
				return SPM_VC_ADD_TYPE_ADD;
			}
		}
		
    }

	else if(L2VPN_VPLS == vpn_type)
	{
		pst_vpls_cb = AVLL_FIND(v_spm_shared->vpls_tree,&vpn_id);
		
		if(NULL == pst_vpls_cb)
		{
			return SPM_VC_ADD_TYPE_ADD;
		}
		
		pst_vpls_nni_cb = AVLL_FIND(pst_vpls_cb->nni_cfg_tree,&vp_idx);
		if(NULL == pst_vpls_nni_cb)
		{
			return SPM_VC_ADD_TYPE_ADD;
		}

		//新数据和老数据不一致，返错
		if((VC_MAIN == pst_vc_cfg->vpn_property_cfg_cb->vc_state)
			&& (0 != pst_vpls_nni_cb->vckey_main.vc_id))
		{
			ret = spm_vc_key_compare(&pst_vc_cfg->vc_key,&pst_vpls_nni_cb->vckey_main);
			if(0 != ret)
			{
				return VC_GET_ADD_TYPE_VPLS_VC_MAIN_NOT_EQUAL;;
			}
		}

		if((VC_BACK == pst_vc_cfg->vpn_property_cfg_cb->vc_state)
			&& (0 != pst_vpls_nni_cb->vckey_back.vc_id))
		{
			ret = spm_vc_key_compare(&pst_vc_cfg->vc_key,&pst_vpls_nni_cb->vckey_back);
			if(0 != ret)
			{
				return VC_GET_ADD_TYPE_VPLS_VC_BACK_NOT_EQUAL;;
			}
		}	
		
		//其他情况，统一只看if_drv_nni_vp_exist
		if(DRV_NNI_VP_EXIST == pst_vpls_nni_cb->if_drv_nni_vp_exist)
		{
			return SPM_VC_ADD_TYPE_MOD;
		}

		else
		{
			return SPM_VC_ADD_TYPE_ADD;
		}
		
	}

	
	else if(L2VPN_MSPW == vpn_type)
	{
    	pst_mspw_cb = AVLL_FIND(v_spm_shared->switch_vc_tree,&vpn_id);
        if(NULL == pst_mspw_cb)
        {
           return L2VPN_MSPW_NNI_1_ADD;    
        }

        if((0 != pst_mspw_cb->nni_no_1) && (0 != pst_mspw_cb->nni_no_2))
        {
            if(pst_mspw_cb->nni_no_1 == vp_idx)
            {
                if(0 == pst_mspw_cb->if_drv_nni_vp_exist_1)
                {
                    return L2VPN_MSPW_NNI_1_ADD;    
                }
                else if(1 == pst_mspw_cb->if_drv_nni_vp_exist_1)
                {
                    return L2VPN_MSPW_NNI_1_MOD;   
                }
            }
            else if(pst_mspw_cb->nni_no_2 == vp_idx)
            {
                if(0 == pst_mspw_cb->if_drv_nni_vp_exist_2)
                {
                    return L2VPN_MSPW_NNI_2_ADD;    
                }
                else if(1 == pst_mspw_cb->if_drv_nni_vp_exist_2)
                {
                    return L2VPN_MSPW_NNI_2_MOD;   
                }    
            }
            else
            {
                return VC_GET_ADD_TYPE_VC_MSPW_ERR1;//出错
            }
        }
        else if((0 == pst_mspw_cb->nni_no_1) && (0 != pst_mspw_cb->nni_no_2))
        {
            if(pst_mspw_cb->nni_no_2 == vp_idx)
            {
                if(0 == pst_mspw_cb->if_drv_nni_vp_exist_2)
                {
                    return L2VPN_MSPW_NNI_2_ADD;    
                }
                else if(1 == pst_mspw_cb->if_drv_nni_vp_exist_2)
                {
                    return L2VPN_MSPW_NNI_2_MOD;   
                }        
            }
            else
            {
                return L2VPN_MSPW_NNI_1_ADD;    
            }
        }
        else if((0 == pst_mspw_cb->nni_no_2) && (0 != pst_mspw_cb->nni_no_1))
        {
            if(pst_mspw_cb->nni_no_1 == vp_idx)
            {
                if(0 == pst_mspw_cb->if_drv_nni_vp_exist_1)
                {
                    return L2VPN_MSPW_NNI_1_ADD;    
                }
                else if(1 == pst_mspw_cb->if_drv_nni_vp_exist_1)
                {
                    return L2VPN_MSPW_NNI_1_MOD;   
                }        
            }
            else
            {
                return L2VPN_MSPW_NNI_2_ADD;    
            }
        }
        else if((0 == pst_mspw_cb->nni_no_1) && (0 == pst_mspw_cb->nni_no_2))
        {
            return L2VPN_MSPW_NNI_1_ADD;      
        }
        else
        {
            return VC_GET_ADD_TYPE_VC_MSPW_ERR2;//出错
        }
        
        
    }

    
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return add_type;    

}

/*****************************************************************************
 函 数 名  : spm_vc_add_nni_vp
 功能描述  : 该函数主要调用同其它模块的逻辑调用，包括:
             1、先判断该vc的添加类型；
             2、调用驱动处理接口，包括add mod模式；
             3、其它待处理模块；
             
 输入参数  : NBB_BYTE    vpn_type     
             SPM_VC_CB   *pst_vc_tbl  
             SPM_VPWS_CB *pst_vpws    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月30日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_add_nni_vp(NBB_BYTE vpn_type,NBB_USHORT vp_idx,NBB_VOID *pst_l2vpn_cfg,
	SPM_VC_ADD_TYPE_E vc_add_type,NBB_USHORT mc_id)
{
    NBB_INT ret = 0;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  
	SPM_VPN_PORT_INFO_CB st_vc_info;
	SPM_VPWS_CB *pst_vpws = NULL;
	SPM_VPLS_CB *pst_vpls = NULL;
	SPM_SWITCH_VC_CB *pst_mspw =NULL;
	SPM_VPLS_NNI_CB *pst_vpls_nni = NULL;
	
    
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    NBB_TRC_ENTRY("spm_vc_add_nni_vp");

	OS_MEMSET(&st_vc_info,0,sizeof(SPM_VPN_PORT_INFO_CB));

	if(NULL == pst_l2vpn_cfg)
	{
		ret = ADD_NNI_VP_FUNC_L2VPN_NULL;
		goto EXIT_LABEL;
	}

	//区分ADD还是MOD,MOD的情况下，MOD取出原来的frr等信息
	if(L2VPN_VPWS == vpn_type)
	{
		pst_vpws = (SPM_VPWS_CB *)pst_l2vpn_cfg;
		
		if(SPM_VC_ADD_TYPE_MOD == vc_add_type)
		{
		    st_vc_info.next_hop_id = pst_vpws->nni_info.next_hop_id;
			st_vc_info.next_hop_id_p = pst_vpws->nni_info.next_hop_id_p;
		    st_vc_info.frr_group_id = pst_vpws->nni_info.frr_group_id;
		}

		//添加VP_NNI
		ret = spm_vc_drv_add_or_mod_nni_vp(vpn_type,vp_idx,pst_vpws,vc_add_type,0,&st_vc_info);

		//写驱动返回值
		if(SUCCESS == ret)
		{
			pst_vpws->nni_info.next_hop_id = st_vc_info.next_hop_id;
			pst_vpws->nni_info.next_hop_id_p = st_vc_info.next_hop_id_p;
			pst_vpws->nni_info.frr_group_id = st_vc_info.frr_group_id;
			pst_vpws->if_drv_nni_vp_exist = DRV_NNI_VP_EXIST;
		}
		
	}

	else if(L2VPN_VPLS == vpn_type)
	{
		pst_vpls = (SPM_VPLS_CB *)pst_l2vpn_cfg;
		pst_vpls_nni = AVLL_FIND(pst_vpls->nni_cfg_tree,&vp_idx);

		if(NULL == pst_vpls_nni)
		{
			ret = ADD_NNI_VP_FUNC_VPLS_NNI_NULL;
			goto EXIT_LABEL;
		}

		if(SPM_VC_ADD_TYPE_MOD == vc_add_type)
		{
		    st_vc_info.next_hop_id = pst_vpls_nni->nni_info.next_hop_id;
			st_vc_info.next_hop_id_p = pst_vpls_nni->nni_info.next_hop_id_p;
		    st_vc_info.frr_group_id = pst_vpls_nni->nni_info.frr_group_id;
		}
		
		//添加VP_NNI,pst_vpls->basic_cfg_cb为空，mc_id=0，vpls的基本配置下发的时候进行
		//vp中mc_id和交叉盘、业务盘组播的更新。
		ret = spm_vc_drv_add_or_mod_nni_vp(vpn_type,vp_idx,pst_vpls,
			   vc_add_type,mc_id,&st_vc_info);

		if(SUCCESS != ret)
		{
	        goto EXIT_LABEL;
		}

		//存储驱动返回值
		pst_vpls_nni->nni_info.next_hop_id = st_vc_info.next_hop_id;
		pst_vpls_nni->nni_info.next_hop_id_p = st_vc_info.next_hop_id_p;
		pst_vpls_nni->nni_info.frr_group_id = st_vc_info.frr_group_id;
		pst_vpls_nni->if_drv_nni_vp_exist = DRV_NNI_VP_EXIST;

		//ADD时添加组播
		if((SPM_VC_ADD_TYPE_ADD == vc_add_type) && (0 != mc_id))
		{
			ret = spm_vc_drv_add_mc_nni_vp(pst_vpls->vpls_id_key,mc_id,pst_vpls_nni);

			if(SUCCESS != ret)
			{
		        goto EXIT_LABEL;
			}
		}
		
	}
	
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;  

}



/*****************************************************************************
 函 数 名  : spm_vc_add_vpws_vp
 功能描述  : 添加vpws类型vc
 输入参数  : SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月27日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_add_vpws_vp(SPM_VC_CB *pst_vc_tbl)
{
    NBB_INT ret = 0;
    
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  
    
    ATG_DCI_VC_BASIC_DATA   *pst_basic_data = NULL;     
    ATG_DCI_VC_VPN_DATA     *pst_vpn_proper = NULL;     
    SPM_VPWS_CB *pst_vpws = NULL;
    NBB_INT vp_num = 0;
    NBB_BOOL avll_ret_code = 0;
	SPM_VC_ADD_TYPE_E vc_add_type = SPM_VC_ADD_TYPE_ADD;
	NBB_BYTE add_flag = 0;
    
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    NBB_TRC_ENTRY("spm_vc_add_vpws_vp");

    /*  pst_vc_tbl check (zhangzhm) */
    if (NULL == pst_vc_tbl)
    {
        ret = ADD_VC_VPWS_VP_VC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }

    /* basic vpn_propety check (zhangzhm) */
    if (NULL == pst_vc_tbl->basic_cfg_cb)
    {
        ret = ADD_VC_VPWS_VP_VC_BASIC_IS_NULL;
        goto EXIT_LABEL;
    }

	if(NULL == pst_vc_tbl->vpn_property_cfg_cb)
	{
		ret = ADD_VC_VPWS_VP_VC_VPN_IS_NULL;
		goto EXIT_LABEL;
	}

    pst_basic_data = pst_vc_tbl->basic_cfg_cb;
    pst_vpn_proper = pst_vc_tbl->vpn_property_cfg_cb;

	//在存配置之前，获取操作类型；
	//之后VC的配置直接存储在VPWS中，后面建立vp的时候用VPWS中完整的主备VC信息,
	//简化代码流程
	vc_add_type = spm_vc_get_add_type(L2VPN_VPWS,pst_vpn_proper->vpn_id,
					pst_vpn_proper->nni_no,pst_vc_tbl);
	
	if((SPM_VC_ADD_TYPE_ADD != vc_add_type) && (SPM_VC_ADD_TYPE_MOD != vc_add_type))
	{
        goto EXIT_LABEL;
	}

	//存储配置
	pst_vpws = AVLL_FIND(SHARED.vpws_tree, &pst_vpn_proper->vpn_id);

    if (NULL == pst_vpws)
    {
        /* 申请一个VPWS新条目的内存空间 */
        pst_vpws = spm_vpws_alloc_cb(NBB_CXT);

        if(NULL == pst_vpws)
        {
            ret = ADD_VC_VPWS_VP_MALLOC_VPWS_NULL;
            goto EXIT_LABEL;
        }
        
        else
        {
            //将新节点插入到tree中
            pst_vpws->vpws_id_key = pst_vpn_proper->vpn_id;
            avll_ret_code = AVLL_INSERT(SHARED.vpws_tree, pst_vpws->spm_vpws_node);
            NBB_ASSERT(avll_ret_code);
        }
    } 
	
    if (0 == pst_vpn_proper->bypass_dh_id) 
    {
        pst_vpws->nni_no = pst_vpn_proper->nni_no;
        pst_vpws->nni_num = 1;
        
        if(VC_MAIN == pst_vpn_proper->vc_state)
        {
            pst_vpws->vckey_main.vc_id = pst_vc_tbl->vc_key.vc_id;
            pst_vpws->vckey_main.vc_type =  pst_vc_tbl->vc_key.vc_type;
            pst_vpws->vckey_main.peer_ip =  pst_vc_tbl->vc_key.peer_ip;
        }

        if(VC_BACK == pst_vpn_proper->vc_state)
        {
            pst_vpws->vckey_back.vc_id = pst_vc_tbl->vc_key.vc_id;
            pst_vpws->vckey_back.vc_type = pst_vc_tbl->vc_key.vc_type;
            pst_vpws->vckey_back.peer_ip = pst_vc_tbl->vc_key.peer_ip;

			//如果是备用VC，写VPWS保护的标志位。删除备用VC时该标志为要置为0
			pst_vpws->use_protect = 1;
        }

		//建立vp
		ret = spm_vc_add_nni_vp(L2VPN_VPWS,
			pst_vpn_proper->nni_no,
			pst_vpws,
			vc_add_type,
			0);
        spm_logical_port_drv_refresh_bypass_vp(pst_vpws,2);
    }
        
    /* 为bypass侧VP*/
    else
    {
        pst_vpws->bypass_no = pst_vpn_proper->nni_no;                      
        pst_vpws->vckey_bypass.vc_id = pst_vc_tbl->vc_key.vc_id;
        pst_vpws->vckey_bypass.vc_type = pst_vc_tbl->vc_key.vc_type;
        pst_vpws->vckey_bypass.peer_ip = pst_vc_tbl->vc_key.peer_ip;


		//同时存在一对UNI_VP和NNI_VP,才建立BYPASS_VP
		//if((DRV_NNI_VP_NOT_EXIST == pst_vpws->if_drv_nni_vp_exist)
			//|| (0 == pst_vpws->uni_num))
		//{
			//ret  = VC_ADD_BYPASS_UNI_NNI_NO_EXIST;
			//goto EXIT_LABEL;
		//}

		ret = spm_vc_add_bypass_vp(pst_vpws,vc_add_type);
        
           spm_logical_port_drv_refresh_bypass_vp(pst_vpws,3);     
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;  
}





/*****************************************************************************
 函 数 名  : spm_vc_add_vpls_vp
 功能描述  : 添加vpls类型vc
 输入参数  : SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月30日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_add_vpls_vp(SPM_VC_CB *pst_vc_tbl )
{    
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  
    NBB_INT ret = SUCCESS;
    SPM_VPLS_CB *pst_vpls = NULL;   
    SPM_VPLS_NNI_CB *pst_vpls_nni = NULL;  
    NBB_BOOL avll_ret_code = 0;
    SPM_VC_ADD_TYPE_E vc_add_type = SPM_VC_ADD_TYPE_ADD;
	NBB_BYTE add_flag = 0;


    ATG_DCI_VC_BASIC_DATA   *pst_basic_data = NULL;     /* 指向配置数据基本信息 (zhangzhm) */
    ATG_DCI_VC_VPN_DATA     *pst_vpn_proper = NULL;     /* 指向配置数据vpn属性 (zhangzhm) */

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    NBB_TRC_ENTRY("spm_vc_add_vpls_vp");

    /*  pst_vc_tbl check (zhangzhm) */
    if (NULL == pst_vc_tbl)
    {
      	ret = ADD_VC_VPLS_VP_VC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }
    
    /* basic vpn_propety check (zhangzhm) */
    if (NULL == pst_vc_tbl->basic_cfg_cb)
    {
       ret = ADD_VC_VPLS_VP_VC_BASIC_IS_NULL;
       goto EXIT_LABEL;
    }

	if(NULL == pst_vc_tbl->vpn_property_cfg_cb)
	{
		ret = ADD_VC_VPLS_VP_VC_VPN_IS_NULL;
        goto EXIT_LABEL;
	}
    
    pst_basic_data = pst_vc_tbl->basic_cfg_cb;
    pst_vpn_proper =  pst_vc_tbl->vpn_property_cfg_cb;
    
	//在存配置之前，获取操作类型；
	//之后VC的配置直接存储在VPLS中，后面建立vp的时候用VPLS中完整的主备VC信息,
	//简化代码流程
	 
	vc_add_type = spm_vc_get_add_type(L2VPN_VPLS,pst_vpn_proper->vpn_id,pst_vpn_proper->nni_no,
		pst_vc_tbl);
	if((SPM_VC_ADD_TYPE_ADD != vc_add_type) && (SPM_VC_ADD_TYPE_MOD != vc_add_type))
	{
        goto EXIT_LABEL;
	}

	//存储配置

	pst_vpls = AVLL_FIND(SHARED.vpls_tree, &pst_vpn_proper->vpn_id);
    
    if (NULL == pst_vpls)
    {    
        /* 申请一个新条目的内存空间 */
        pst_vpls = spm_alloc_vpls_cb(NBB_CXT);

        if(NULL == pst_vpls)
        {
            ret = ADD_VC_VPLS_VP_MALLOC_VPLS_NULL;
        	goto EXIT_LABEL;
        }
        else    /* 申请成功 (zhangzhm) */
        {
            //将新节点插入到tree中
            pst_vpls->vpls_id_key = pst_vpn_proper->vpn_id;
            avll_ret_code = AVLL_INSERT(SHARED.vpls_tree, pst_vpls->spm_vpls_node);
            NBB_ASSERT(avll_ret_code);
        }
    } 

	/*vpls中vp为0个，建立大组播组*/
	if(0 == pst_vpls->uni_num + pst_vpls->nni_num)
	{
		ret = spm_vpls_create_mc(pst_vc_tbl->vpn_property_cfg_cb->mc_id);

		if(SUCCESS != ret)
		{
			goto EXIT_LABEL;
		}
	}
    
   
    pst_vpls_nni = AVLL_FIND(pst_vpls->nni_cfg_tree, &pst_vpn_proper->nni_no);
    
    if (NULL == pst_vpls_nni)   /* vpls中未找到该nni (zhangzhm) */
    {        
        /* 申请一个新条目的内存空间 */
        pst_vpls_nni = spm_vpls_nni_alloc_cb(NBB_CXT);

        if(NULL == pst_vpls_nni)
        {
            ret = ADD_VC_VPLS_VP_MALLOC_VPLS_NNI_NULL;
            goto EXIT_LABEL;
        }
        else
        {
            //将新节点插入到tree中
            pst_vpls_nni->nni_no = pst_vpn_proper->nni_no;
            avll_ret_code = AVLL_INSERT(pst_vpls->nni_cfg_tree, pst_vpls_nni->spm_vpls_nni_node);
            NBB_ASSERT(avll_ret_code);
			pst_vpls->nni_num++;
        }
    }

	if(VC_MAIN == pst_vpn_proper->vc_state)
    {
        pst_vpls_nni->vckey_main.vc_id = pst_vc_tbl->vc_key.vc_id;
        pst_vpls_nni->vckey_main.vc_type = pst_vc_tbl->vc_key.vc_type;
        pst_vpls_nni->vckey_main.peer_ip = pst_vc_tbl->vc_key.peer_ip;
    }

    if(VC_BACK == pst_vpn_proper->vc_state)
    {
        pst_vpls_nni->vckey_back.vc_id = pst_vc_tbl->vc_key.vc_id;
        pst_vpls_nni->vckey_back.vc_type = pst_vc_tbl->vc_key.vc_type;
        pst_vpls_nni->vckey_back.peer_ip = pst_vc_tbl->vc_key.peer_ip;

		//如果是备用VC，写VPWS保护的标志位。删除备用VC时该标志为要置为0
		pst_vpls_nni->use_protect = 1;
    }

	//添加vp
	ret = spm_vc_add_nni_vp(L2VPN_VPLS,
		pst_vpn_proper->nni_no,
		pst_vpls,
		vc_add_type,
		pst_vc_tbl->vpn_property_cfg_cb->mc_id);
	
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;    
}



/*****************************************************************************
 函 数 名  : spm_vc_add_mspw_vp
 功能描述  : mspw nni vp添加
 输入参数  : SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月30日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_add_mspw_vp(SPM_VC_CB *pst_vc_tbl)
{    
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN]={0};  
    NBB_INT ret = SUCCESS;
    SPM_SWITCH_VC_CB *pst_mspw = NULL; 
    NBB_BOOL avll_ret_code = 0;
    SPM_VC_ADD_TYPE_E vc_add_type = SPM_VC_ADD_TYPE_ADD;
    ATG_DCI_VC_BASIC_DATA   *pst_basic_data = NULL;    
    ATG_DCI_VC_VPN_DATA     *pst_vpn_proper = NULL;     
    ATG_DCI_VC_KEY vc_key;
    NBB_BYTE addflag = 0 ; 
	SPM_VPN_PORT_INFO_CB st_vc_info;

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    
    NBB_TRC_ENTRY("spm_vc_add_mspw_vp");

    NBB_MEMSET(&vc_key, 0, sizeof(ATG_DCI_VC_KEY));
	OS_MEMSET(&st_vc_info,0,sizeof(SPM_VPN_PORT_INFO_CB));
    
    /*  pst_vc_tbl check (zhangzhm) */
    if (NULL == pst_vc_tbl)
    {
       ret = ADD_MSPW_VP_VC_CFG_NULL;
       goto EXIT_LABEL;
    }
    
    
    if ((NULL == pst_vc_tbl->basic_cfg_cb) || (NULL == pst_vc_tbl->vpn_property_cfg_cb))
    {     
       ret = ADD_MSPW_VP_VC_BASIC_VPN_CFG_NULL;
       goto EXIT_LABEL;
    }
    
    pst_basic_data = pst_vc_tbl->basic_cfg_cb;
    pst_vpn_proper =  pst_vc_tbl->vpn_property_cfg_cb;
	
    pst_mspw = AVLL_FIND(v_spm_shared->switch_vc_tree, &pst_vpn_proper->vpn_id);
    
    if (NULL == pst_mspw)
    {
        /* 申请一个新条目的内存空间 */
        pst_mspw = spm_switch_vc_alloc_cb(NBB_CXT);

        if(NULL == pst_mspw)    /* 申请失败 (zhangzhm) */
        {
            ret = ADD_MSPW_VP_VC_ALLOC_MSPW_CB_NULL;
            goto EXIT_LABEL;
        }
        else    
        {
            //将新节点插入到tree中
            pst_mspw->switch_vc_key = pst_vpn_proper->vpn_id;
            avll_ret_code = AVLL_INSERT(SHARED.switch_vc_tree, pst_mspw->spm_switch_vc_node);
            NBB_ASSERT(avll_ret_code);
        }
    } 

    vc_add_type = spm_vc_get_add_type(L2VPN_MSPW,
		pst_vpn_proper->vpn_id,
		pst_vpn_proper->nni_no,
		pst_vc_tbl); 

	if((L2VPN_MSPW_NNI_1_ADD != vc_add_type)
		&& (L2VPN_MSPW_NNI_1_MOD != vc_add_type)
		&& (L2VPN_MSPW_NNI_2_ADD != vc_add_type)
		&& (L2VPN_MSPW_NNI_2_MOD != vc_add_type))
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",ADD_MSPW_VP_VC_ADD_TYPE_ERR,vc_add_type);
		ret = ADD_MSPW_VP_VC_ADD_TYPE_ERR;
		goto EXIT_LABEL;
	}
    
    if((L2VPN_MSPW_NNI_1_ADD == vc_add_type) || (L2VPN_MSPW_NNI_1_MOD == vc_add_type))
    {
        pst_mspw->nni_no_1 = pst_vpn_proper->nni_no;
        pst_mspw->vckey_1.vc_id = pst_vc_tbl->vc_key.vc_id;
        pst_mspw->vckey_1.peer_ip = pst_vc_tbl->vc_key.peer_ip;
        pst_mspw->vckey_1.vc_type = pst_vc_tbl->vc_key.vc_type; 

        //MOD的时候传入next_hop_id
        if(L2VPN_MSPW_NNI_1_MOD == vc_add_type)
        {
        	st_vc_info.next_hop_id = pst_mspw->next_hop_id_1;
        }
        
        ret = spm_vc_drv_create_nni_vp_for_mspw(pst_mspw->switch_vc_key,
			pst_mspw->nni_no_1,
			pst_vc_tbl->vc_key,
            vc_add_type, 
            &st_vc_info);

        if(SUCCESS == ret)
        {
        	pst_mspw->next_hop_id_1 = st_vc_info.next_hop_id;
			pst_mspw->if_drv_nni_vp_exist_1 = DRV_NNI_VP_EXIST;
        }
    }

    if((L2VPN_MSPW_NNI_2_ADD == vc_add_type) || (L2VPN_MSPW_NNI_2_MOD == vc_add_type))
    {
        pst_mspw->nni_no_2 = pst_vpn_proper->nni_no;
        pst_mspw->vckey_2.vc_id = pst_vc_tbl->vc_key.vc_id;
        pst_mspw->vckey_2.peer_ip = pst_vc_tbl->vc_key.peer_ip;
        pst_mspw->vckey_2.vc_type = pst_vc_tbl->vc_key.vc_type;
        
        if(L2VPN_MSPW_NNI_2_MOD == vc_add_type)
        {
        	st_vc_info.next_hop_id = pst_mspw->next_hop_id_2;
        }
        
        ret = spm_vc_drv_create_nni_vp_for_mspw(pst_mspw->switch_vc_key,
			pst_mspw->nni_no_2, 
			pst_vc_tbl->vc_key,
            vc_add_type,
            &st_vc_info);
		
		if(SUCCESS == ret)
		{
			pst_mspw->next_hop_id_2 = st_vc_info.next_hop_id;
			pst_mspw->if_drv_nni_vp_exist_2 = DRV_NNI_VP_EXIST;
		}

             
    }

    if(SUCCESS != ret)
    {
        goto EXIT_LABEL;
    }


    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;        
}


/*****************************************************************************
 函 数 名  : spm_vc_del_or_mod_vpws_nni_vp
 功能描述  : 收到VC配置删除的时候，删除或者更新VP-NNI
 输入参数  : SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月27日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_del_or_mod_vpws_nni_vp(SPM_VC_CB *pst_vc_tbl)
{
    NBB_INT ret = 0;
    
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  
    
    ATG_DCI_VC_BASIC_DATA   *pst_basic_data = NULL;     
    ATG_DCI_VC_VPN_DATA     *pst_vpn_proper = NULL;     
    SPM_VPWS_CB *pst_vpws = NULL;
    NBB_INT vp_num = 0;
    NBB_BOOL avll_ret_code = 0;
	SPM_VPN_PORT_INFO_CB pst_vc_info;

	//1.如果原来是主备共存，删除一条VC的时候，驱动NNI的操作是mod,
	//此时先删除VPWS中对应该VC的本地数据,用新的数据调用spm_vc_drv_add_od_mod_nni_vp
	
	//2.如果原来只有主 或者 备，删除对应VC的时候，驱动NNI的操作是DEL
	//此时，先不更改VPWS的本地数据，用老的数据调用接口spm_vc_drv_add_mc_vp和
	//spm_vc_drv_nni_vp,删除驱动之后再删除本地数据
	
	
    
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

	OS_MEMSET(&pst_vc_info,0,sizeof(SPM_VPN_PORT_INFO_CB));


    NBB_TRC_ENTRY("spm_vc_add_vpws_vp");

    /*  pst_vc_tbl check (zhangzhm) */
    if (NULL == pst_vc_tbl)
    {
    	ret = DEL_VPWS_VP_VC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }

    /* basic vpn_propety check (zhangzhm) */
    if (NULL == pst_vc_tbl->basic_cfg_cb)
    {
       ret = DEL_VPWS_VP_VC_BASIC_CFG_IS_NULL;
	   goto EXIT_LABEL;
    }

	if(NULL == pst_vc_tbl->vpn_property_cfg_cb)
	{
		ret = DEL_VPWS_VP_VC_VPN_CFG_IS_NULL;
	    goto EXIT_LABEL;
	}

    pst_basic_data = pst_vc_tbl->basic_cfg_cb;
    pst_vpn_proper =  pst_vc_tbl->vpn_property_cfg_cb;

    pst_vpws = AVLL_FIND(SHARED.vpws_tree, &pst_vpn_proper->vpn_id);

    if (NULL == pst_vpws)
    {
    	ret = DEL_VPWS_VP_VPWS_CFG_IS_NULL;
	    goto EXIT_LABEL;
    }

	if(0 == pst_vpws->nni_num)
	{
    	ret = DEL_VPWS_VP_VPWS_NNI_NUM_IS_ZERO;
	    goto EXIT_LABEL;
    }	
	

	//删除VPWS本地数据中关于该VC的信息,并根据主备情况，分别调用MOD或者DEL
	if(VC_MAIN == pst_vc_tbl->vpn_property_cfg_cb->vc_state)
	{
		//如果VPWS已经有一个NNI，后来主控又下发一条VC加入该VPWS，添加的时候不成功，
		//会出现这种情况
		if(0 != spm_vc_key_compare(&pst_vc_tbl->vc_key,&pst_vpws->vckey_main))
		{
			ret = DEL_VPWS_VP_MAIN_NOT_EQUAL;
	        goto EXIT_LABEL;
		}
		
		//备用VC不存在，如果驱动已经建立成功，需要删除驱动和保护，否则，值删除本地配置
		if(0 == pst_vpws->vckey_back.vc_id)
		{
			if(DRV_NNI_VP_EXIST == pst_vpws->if_drv_nni_vp_exist)
			{
				ret = spm_vc_drv_del_nni_vp(L2VPN_VPWS,pst_vpn_proper->vpn_id,pst_vpws);
				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
			}

			OS_MEMSET(&pst_vpws->vckey_main,0,sizeof(ATG_DCI_VC_KEY));
			pst_vpws->nni_no = 0;
			pst_vpws->nni_num = 0;
			pst_vpws->if_drv_nni_vp_exist = 0;

			OS_MEMSET(&pst_vpws->nni_info,0,sizeof(SPM_VPN_PORT_INFO_CB));
		}

		//备用VC存在，如果之前驱动已经建立成功，此处MOD VP。否则，只需要修改本地数据
		else
		{
			OS_MEMSET(&pst_vpws->vckey_main,0,sizeof(ATG_DCI_VC_KEY));
			
			if(DRV_NNI_VP_EXIST == pst_vpws->if_drv_nni_vp_exist)
			{
			    pst_vc_info.next_hop_id = pst_vpws->nni_info.next_hop_id;
				pst_vc_info.next_hop_id_p = pst_vpws->nni_info.next_hop_id_p;
			    pst_vc_info.frr_group_id = pst_vpws->nni_info.frr_group_id;
				
				ret = spm_vc_drv_add_or_mod_nni_vp(L2VPN_VPWS,pst_vpws->nni_no,
					pst_vpws,SPM_VC_ADD_TYPE_MOD,0,&pst_vc_info);

				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
				pst_vpws->nni_info.next_hop_id = pst_vc_info.next_hop_id;
				pst_vpws->nni_info.next_hop_id_p = pst_vc_info.next_hop_id_p;
				pst_vpws->nni_info.frr_group_id = pst_vc_info.frr_group_id;
				
			}
			
		}
			
	
	}

	else if(VC_BACK == pst_vc_tbl->vpn_property_cfg_cb->vc_state)
	{
		if(0 != spm_vc_key_compare(&pst_vc_tbl->vc_key,&pst_vpws->vckey_back))
		{
			ret = DEL_VPWS_VP_BAK_NOT_EQUAL;
	        goto EXIT_LABEL;
		}
		
		//主用VC不存在，如果之前已经建立驱动成功，删除驱动；否则值需要删除本地VPWS中的数据
		if(0 == pst_vpws->vckey_main.vc_id)
		{
			if(DRV_NNI_VP_EXIST == pst_vpws->if_drv_nni_vp_exist)
			{
				ret = spm_vc_drv_del_nni_vp(L2VPN_VPWS,pst_vpn_proper->vpn_id,pst_vpws);
				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
			}

			OS_MEMSET(&pst_vpws->vckey_back,0,sizeof(ATG_DCI_VC_KEY));

			//删除备用VC的时候，把保护标志删除掉  重要!
			pst_vpws->use_protect = 0;
			
			pst_vpws->nni_no = 0;
			pst_vpws->nni_num = 0;
			pst_vpws->if_drv_nni_vp_exist = 0;
			OS_MEMSET(&pst_vpws->nni_info,0,sizeof(SPM_VPN_PORT_INFO_CB));
		}

		//主用vc存在，之前已经建立驱动成功，MOD VP，否则，只需要删除VPWS中的数据
		else
		{
			OS_MEMSET(&pst_vpws->vckey_back,0,sizeof(ATG_DCI_VC_KEY));

			//删除备用VC的时候，把保护标志删除掉  重要!
			pst_vpws->use_protect = 0;
			
			if(DRV_NNI_VP_EXIST == pst_vpws->if_drv_nni_vp_exist)
			{
			    pst_vc_info.next_hop_id = pst_vpws->nni_info.next_hop_id;
				pst_vc_info.next_hop_id_p = pst_vpws->nni_info.next_hop_id_p;
			    pst_vc_info.frr_group_id = pst_vpws->nni_info.frr_group_id;
				
				ret = spm_vc_drv_add_or_mod_nni_vp(L2VPN_VPWS,pst_vpws->nni_no,
					pst_vpws,SPM_VC_ADD_TYPE_MOD,0,&pst_vc_info);

				if(SUCCESS != ret)
				{
					
			        goto EXIT_LABEL;
				}
				else
				{
					pst_vpws->nni_info.next_hop_id = pst_vc_info.next_hop_id;
					pst_vpws->nni_info.next_hop_id_p = pst_vc_info.next_hop_id_p;
					pst_vpws->nni_info.frr_group_id = pst_vc_info.frr_group_id;
				}
			}
		}
	}
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;  
}


/*****************************************************************************
 函 数 名  : spm_vc_del_or_mod_vpls_vp
 功能描述  : 收到VC配置删除的时候，删除或者更新VP-NNI
 输入参数  : SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月24日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_del_mspw_vp(SPM_VC_CB *pst_vc_tbl)
{
    NBB_INT ret = SUCCESS;
    NBB_USHORT usVpnId = 0;
    SPM_SWITCH_VC_CB *pst_mspw = NULL;
    LSPPROT_KEY st_lspprot_key;
    NBB_BYTE lsp_option = 0;
    NBB_USHORT vp_idx = 0;
    NBB_ULONG next_hop_id = 0;
    NBB_ULONG peer_ip;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  

    NBB_TRC_ENTRY("spm_vc_del_mspw_vp");

    if(NULL == pst_vc_tbl)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"*ERR pst_vc_tbl is null *,r1=%d\n",DEL_MSPW_VP_VC_CB_NULL);
        ret = DEL_MSPW_VP_VC_CB_NULL;
        goto EXIT_LABEL;
    }

    if((NULL == pst_vc_tbl->vpn_property_cfg_cb) || (NULL == pst_vc_tbl->basic_cfg_cb))
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"*ERR pst_vc_tbl is null *,r1=%d\n",DEL_MSPW_VP_VC_BASIC_VPN_CB_NULL);
        ret = DEL_MSPW_VP_VC_BASIC_VPN_CB_NULL;
        goto EXIT_LABEL;    
    }
    
    usVpnId = pst_vc_tbl->vpn_property_cfg_cb->vpn_id;
    
    pst_mspw = AVLL_FIND(SHARED.switch_vc_tree, &usVpnId);

    if(pst_mspw == NULL)
    {
        ret = DEL_MSPW_VP_MSPW_CB_NULL;
        goto EXIT_LABEL;      
    }

    if(0 == spm_vc_key_compare(&pst_mspw->vckey_1,&pst_vc_tbl->vc_key))
    {
    	if(DRV_NNI_VP_EXIST == pst_mspw->if_drv_nni_vp_exist_1)
    	{
    		ret = spm_drv_del_mspw_nni_vp(pst_vc_tbl->vc_key,usVpnId);
			if(SUCCESS != ret)
			{
				goto EXIT_LABEL;
			}
    	}
        
        pst_mspw->nni_no_1 = 0;
        pst_mspw->next_hop_id_1 = 0;
		pst_mspw->if_drv_nni_vp_exist_1 = DRV_NNI_VP_NOT_EXIST;
        NBB_MEMSET(&pst_mspw->vckey_1, 0, sizeof(ATG_DCI_VC_KEY));
        
    }
    else if(0 ==spm_vc_key_compare(&pst_mspw->vckey_2,&pst_vc_tbl->vc_key))
    {
    	if(DRV_NNI_VP_EXIST == pst_mspw->if_drv_nni_vp_exist_2)
    	{
    		ret = spm_drv_del_mspw_nni_vp(pst_vc_tbl->vc_key,usVpnId);
			if(SUCCESS != ret)
			{
				goto EXIT_LABEL;
			}
    	}
        
        pst_mspw->nni_no_2 = 0;
        pst_mspw->next_hop_id_2 = 0;
		pst_mspw->if_drv_nni_vp_exist_2 = DRV_NNI_VP_NOT_EXIST;
        NBB_MEMSET(&pst_mspw->vckey_2, 0, sizeof(ATG_DCI_VC_KEY));
       
    }
	
    if((0 == pst_mspw->nni_no_1) && (0 == pst_mspw->nni_no_2))
    {
    	//从树中删除节点
        AVLL_DELETE(v_spm_shared->switch_vc_tree, pst_mspw->spm_switch_vc_node);
        spm_free_switch_vc_cb(pst_mspw);
    }

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return ret;      
}


/*****************************************************************************
 函 数 名  : spm_vc_del_or_mod_vpls_vp
 功能描述  : 收到VC配置删除的时候，删除或者更新VP-NNI
 输入参数  : SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月24日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_del_or_mod_vpls_vp(SPM_VC_CB *pst_vc_tbl)
{
    NBB_INT ret = 0;
    
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  
    
    ATG_DCI_VC_BASIC_DATA   *pst_basic_data = NULL;    
    ATG_DCI_VC_VPN_DATA     *pst_vpn_proper = NULL;    
    SPM_VPLS_CB *pst_vpls = NULL;
	SPM_VPLS_NNI_CB *pst_vpls_nni = NULL;
    NBB_INT vp_num = 0;
	SPM_VC_ADD_TYPE_E vc_add_type = SPM_VC_ADD_TYPE_ADD;
	SPM_VPN_PORT_INFO_CB pst_vc_info;
	NBB_USHORT mc_id = 0;  //mod vp时候使用的mc_id
	
	//1.如果原来是主备共存，删除一条VC的时候，驱动NNI的操作是mod,
	//此时先删除VPLS中对应该VC的本地数据,用新的数据调用spm_vc_drv_add_od_mod_nni_vp
	
	//2.如果原来只有主 或者 备，删除对应VC的时候，驱动NNI的操作是DEL
	//此时，先不更改VPLS的本地数据，用老的数据调用接口spm_vc_drv_add_mc_vp和
	//spm_vc_drv_del_nni_vp,删除驱动之后再删除本地数据
    
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

	OS_MEMSET(&pst_vc_info,0,sizeof(SPM_VPN_PORT_INFO_CB));


    NBB_TRC_ENTRY("spm_vc_add_vpws_vp");

    /*  pst_vc_tbl check (zhangzhm) */
    if (NULL == pst_vc_tbl)
    {
        ret = DEL_VPLS_VP_VC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }

    /* basic vpn_propety check (zhangzhm) */
    if (NULL == pst_vc_tbl->basic_cfg_cb) 
    {
        ret = DEL_VPLS_VP_VC_BASIC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }

	if(NULL == pst_vc_tbl->vpn_property_cfg_cb)
	{
		ret = DEL_VPLS_VP_VC_VPN_CFG_IS_NULL;
		goto EXIT_LABEL;
	}

    pst_basic_data = pst_vc_tbl->basic_cfg_cb;
    pst_vpn_proper =  pst_vc_tbl->vpn_property_cfg_cb;
	mc_id = pst_vpn_proper->mc_id;

    pst_vpls = AVLL_FIND(SHARED.vpls_tree, &pst_vpn_proper->vpn_id);
    if(NULL == pst_vpls)
    {
    	ret = DEL_VPLS_VP_VPLS_CFG_IS_NULL;
        goto EXIT_LABEL;
    }

	pst_vpls_nni = AVLL_FIND(pst_vpls->nni_cfg_tree,&pst_vpn_proper->nni_no);
	if (NULL == pst_vpls_nni)
    {
    	ret = DEL_VPLS_VP_VPLS_NNI_CFG_IS_NULL;
        goto EXIT_LABEL;
    }
	

	//删除VPWS本地数据中关于该VC的信息,并根据主备情况，分别调用MOD或者DEL
	if(VC_MAIN == pst_vc_tbl->vpn_property_cfg_cb->vc_state)
	{
		if(0 != spm_vc_key_compare(&pst_vc_tbl->vc_key,&pst_vpls_nni->vckey_main))
		{
			ret = DEL_VPLS_VP_MAIN_NOT_EQUAL;
	        goto EXIT_LABEL;
		}
		
		//备用VC不存在，如果驱动已经建立成功，需要删除驱动和保护，否则，值删除本地配置
		if(0 == pst_vpls_nni->vckey_back.vc_id)
		{
			if(DRV_NNI_VP_EXIST == pst_vpls_nni->if_drv_nni_vp_exist)
			{
				//删除SPU的组播成员
				ret = spm_vc_drv_del_arad_mc_nni_vp(pst_vc_tbl);
				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
				
				ret = spm_vc_drv_del_nni_vp(L2VPN_VPLS,
						pst_vpls->vpls_id_key,
						pst_vpls_nni);
				
				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
			}

			//删除成功，释放节点
			
            AVLL_DELETE(pst_vpls->nni_cfg_tree, pst_vpls_nni->spm_vpls_nni_node); 
            spm_vpls_nni_free_cb(pst_vpls_nni);  
			pst_vpls_nni = NULL;
			pst_vpls->nni_num--;

			//删除配置之后,
			//1.删除交叉盘上的槽位组播成员
			
			ret = spm_vc_drv_del_src_mc_vp(pst_vpls,
				pst_vc_tbl->vc_info_cb.slot_id,
				mc_id);
			
			if(SUCCESS != ret)
			{
		        goto EXIT_LABEL;
			}

			/*如果vpls中已经没有vp了，就删除组播成员*/
			if(0 == pst_vpls->uni_num + pst_vpls->nni_num)
			{
				ret = spm_vpls_del_mc(mc_id);
				if(SUCCESS != ret)
				{
					goto EXIT_LABEL;
				}
				
			}
			
		}

		//备用VC存在，如果之前驱动已经建立成功，此处MOD VP。否则，只需要修改本地数据
		else
		{
			OS_MEMSET(&pst_vpls_nni->vckey_main,0,sizeof(ATG_DCI_VC_KEY));
			if(DRV_NNI_VP_EXIST == pst_vpls_nni->if_drv_nni_vp_exist)
			{
			    pst_vc_info.next_hop_id = pst_vpls_nni->nni_info.next_hop_id;
				pst_vc_info.next_hop_id_p = pst_vpls_nni->nni_info.next_hop_id_p;
			    pst_vc_info.frr_group_id = pst_vpls_nni->nni_info.frr_group_id;
				
				ret = spm_vc_drv_add_or_mod_nni_vp(L2VPN_VPLS,
					pst_vpls_nni->nni_no,
					pst_vpls,
					SPM_VC_ADD_TYPE_MOD,
					mc_id,
					&pst_vc_info);

				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
				
				pst_vpls_nni->nni_info.next_hop_id = pst_vc_info.next_hop_id;
				pst_vpls_nni->nni_info.next_hop_id_p = pst_vc_info.next_hop_id_p;
				pst_vpls_nni->nni_info.frr_group_id = pst_vc_info.frr_group_id;
			}
		}
			
	
	}

	else if(VC_BACK == pst_vc_tbl->vpn_property_cfg_cb->vc_state)
	{
		if(0 != spm_vc_key_compare(&pst_vc_tbl->vc_key,&pst_vpls_nni->vckey_back))
		{
			ret = DEL_VPLS_VP_MAIN_NOT_EQUAL;
	        goto EXIT_LABEL;
		}
		
		//主用VC不存在，如果之前已经建立驱动成功，删除驱动；否则值需要删除本地VPWS中的数据
		if(0 == pst_vpls_nni->vckey_main.vc_id)
		{
			if(DRV_NNI_VP_EXIST == pst_vpls_nni->if_drv_nni_vp_exist)
			{
				
				ret = spm_vc_drv_del_arad_mc_nni_vp(pst_vc_tbl);
				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
				
				ret = spm_vc_drv_del_nni_vp(L2VPN_VPLS,
					pst_vpls->vpls_id_key,
					pst_vpls_nni);
				
				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
			}
			
			//删除成功，释放节点
            AVLL_DELETE(pst_vpls->nni_cfg_tree, pst_vpls_nni->spm_vpls_nni_node); 
            spm_vpls_nni_free_cb(pst_vpls_nni);  
			pst_vpls_nni = NULL;
			pst_vpls->nni_num--;

			//删除配置之后,
			//1.删除交叉盘上的槽位组播成员
			ret = spm_vc_drv_del_src_mc_vp(pst_vpls,
				pst_vc_tbl->vc_info_cb.slot_id,
				mc_id);
			if(SUCCESS != ret)
			{
		        goto EXIT_LABEL;
			}

			/*如果vpls中已经没有vp了，就删除组播成员*/
			if(0 == pst_vpls->uni_num + pst_vpls->nni_num)
			{
				ret = spm_vpls_del_mc(mc_id);
				if(SUCCESS != ret)
				{
					goto EXIT_LABEL;
				}
				
			}
			
		}

		//主用vc存在，之前已经建立驱动成功，MOD VP，否则，只需要删除VPWS中的数据
		else
		{
			OS_MEMSET(&pst_vpls_nni->vckey_back,0,sizeof(ATG_DCI_VC_KEY));

			//删除备用VC的时候，把保护标志删除掉  重要!
			pst_vpls_nni->use_protect = 0;
			if(DRV_NNI_VP_EXIST == pst_vpls_nni->if_drv_nni_vp_exist)
			{
			    pst_vc_info.next_hop_id = pst_vpls_nni->nni_info.next_hop_id;
				pst_vc_info.next_hop_id_p = pst_vpls_nni->nni_info.next_hop_id_p;
			    pst_vc_info.frr_group_id = pst_vpls_nni->nni_info.frr_group_id;
				
				ret = spm_vc_drv_add_or_mod_nni_vp(L2VPN_VPLS,
					pst_vpls_nni->nni_no,
					pst_vpls,
					SPM_VC_ADD_TYPE_MOD,
					mc_id,
					&pst_vc_info);

				if(SUCCESS != ret)
				{
			        goto EXIT_LABEL;
				}
				else
				{
					pst_vpls_nni->nni_info.next_hop_id = pst_vc_info.next_hop_id;
					pst_vpls_nni->nni_info.next_hop_id_p = pst_vc_info.next_hop_id_p;
					pst_vpls_nni->nni_info.frr_group_id = pst_vc_info.frr_group_id;
				}
			}
		}
	}
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;  
}



/*****************************************************************************
 函 数 名  : spm_vc_add_vp_and_prot
 功能描述  : 由于vc基本配置和vpn属性分离，所以要求二者同时存在时，对vc和保护
             进行添加或mod处理
 输入参数  : SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月27日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_add_vp_and_prot(SPM_VC_CB *pst_vc_tbl)
{
    NBB_INT ret = 0;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  
    
    ATG_DCI_VC_BASIC_DATA   *pst_basic_data = NULL;          
    ATG_DCI_VC_VPN_DATA     *pst_vpn_proper = NULL;   
    
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    NBB_TRC_ENTRY("spm_vc_add_vp_and_prot");

	if(NULL == pst_vc_tbl)
	{
		ret = ADD_VC_VP_AND_PROT_VC_CFG_BULL;
		goto EXIT_LABEL;
	}

    /* 确保如下两内容同时存在，才去添加或mod vc (zhangzhm) */
    if (NULL == pst_vc_tbl->basic_cfg_cb)
    {
        ret = ADD_VC_VP_AND_PROT_BASIC_BULL;
        goto EXIT_LABEL;
    }

	if(NULL == pst_vc_tbl->vpn_property_cfg_cb)
	{
		ret = ADD_VC_VP_AND_PROT_VPN_NULL;
		goto EXIT_LABEL;
	}

    pst_basic_data = pst_vc_tbl->basic_cfg_cb;
    pst_vpn_proper = pst_vc_tbl->vpn_property_cfg_cb;

	//to do
	//每一次都打印??
	SPM_L2_INFO_PRINT_LOG(uc_message,"INFO:add_or_mod vp,vpnid=%d,vpntype=%d,vpid=%d,dhid=%d\n\n",
		pst_vpn_proper->vpn_id,pst_vpn_proper->vpn_type,pst_vpn_proper->nni_no,
		pst_vpn_proper->bypass_dh_id);
	
    /* vpn类型为VPWS*/
    if(L2VPN_VPWS == pst_vpn_proper->vpn_type)
    {
        ret = spm_vc_add_vpws_vp(pst_vc_tbl);
    }  

    /* vpn类型为VPLS*/
    else if(L2VPN_VPLS == pst_vpn_proper->vpn_type)
    {
        ret = spm_vc_add_vpls_vp(pst_vc_tbl);
    }      
    
    /* vpn类型为MSPW*/
    else if(L2VPN_MSPW == pst_vpn_proper->vpn_type)
    {
        ret = spm_vc_add_mspw_vp(pst_vc_tbl);
    }
    
    /* vpn类型错误*/
    else
    {
        ret = ADD_VC_VP_AND_PROT_VPN_TYPE_ERR;
        goto EXIT_LABEL;
    }        

    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;    

}







/*****************************************************************************
 函 数 名  : spm_vc_basic_data_proc
 功能描述  : VC表基本配置处理函数
 输入参数  : ATG_DCI_SET_VC *pstSetVc
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年8月29日
    作    者   : fansongbo
    修改内容   : 新生成函数
    
  2.日    期   : 2015年11月27日
    作    者   : zhangzhm
    修改内容   : 1)根据协议修改，融合配置块1、4处理vc添加
                 2)拆分不同业务类型vc处理，提高代码可读性
                 3)修改数据存储模式为只要数据下发及存储或更新；驱动成功后更新vcinfo

*****************************************************************************/
NBB_INT spm_vc_basic_data_proc(ATG_DCI_SET_VC *pst_set_vc,NBB_ULONG ul_oper_basic,
    SPM_VC_CB *pst_vc_tbl)
{

    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];  
    NBB_INT ret = SUCCESS;
    ATG_DCI_VC_BASIC_DATA *pst_basic_data = NULL;
    NBB_BYTE *puc_basic_data_start = NULL;   

    NBB_TRC_ENTRY("spm_vc_basic_data_proc");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pst_set_vc);

    if (NULL == pst_set_vc)
    {
       	ret = ADD_VC_BASIC_VC_IPS_IS_NULL;
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_vc_tbl)
    {
        ret = ADD_VC_BASIC_VC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }
	
   
    /***************************************************************************/
    /*                        基本配置                               */
    /***************************************************************************/
    if (ATG_DCI_OPER_ADD == ul_oper_basic)
    {

        /* 计算第一个entry的地址。*/
        puc_basic_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vc, &pst_set_vc->basic_data);    

        /* 如果指针为NULL，无数据 */
        if (NULL == puc_basic_data_start)
        {
            ret = ADD_VC_BASIC_DATA_START_IS_NULL;
            
            goto EXIT_LABEL;
        }
        
        /* 基本数据存在处理 (zhangzhm) */
        pst_basic_data = (ATG_DCI_VC_BASIC_DATA *)puc_basic_data_start;

        if (SPM_PRINT_CFG == g_vc_cfg_print_setting)
        {
            printf("  1）basic cfg\n");
            spm_dbg_print_vc_basic_cfg(pst_basic_data);
        }

		OS_SPRINTF(uc_message,"  1）basic cfg\n");
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
		
		spm_dbg_record_vc_basic_cfg(pst_basic_data);

		//如果基本配置中的tunnel配置有变化，原来建立的vc和tunnle的关系需要解除。zhangzhm
		if(NULL != pst_vc_tbl->basic_cfg_cb)
		{
			ret = spm_vc_if_tunnel_change(pst_vc_tbl->basic_cfg_cb,pst_basic_data);
			if(VC_TUNNEL_CHANGED == ret)
			{
				ret = spm_del_vc_lsp_rsvp_relation(pst_vc_tbl->vc_key);

				//打印不退出
				if(SUCCESS != ret)
				{
					SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",
						ADD_VC_BASIC_DEL_LSP_RELATION_FAIL,ret);
					ret = SUCCESS;
				}
				
			}
		}

        /* 保存数据处理，为空则申请新空间 */
        if (NULL == pst_vc_tbl->basic_cfg_cb)
        {
            pst_vc_tbl->basic_cfg_cb = 
                (ATG_DCI_VC_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VC_BASIC_DATA),
                                                    NBB_NORETRY_ACT,
                                                    MEM_SPM_VC_BASIC_CB);

            if(NULL == pst_vc_tbl->basic_cfg_cb)
            {
            	ret = ADD_VC_MALLOC_BASIC_MEM_FAIL;
                goto EXIT_LABEL;
            }
        }

        /*数据为新申请或已存在，存储数据 */
        if(NULL != pst_vc_tbl->basic_cfg_cb)   
        {
            OS_MEMCPY(pst_vc_tbl->basic_cfg_cb, pst_basic_data, sizeof(ATG_DCI_VC_BASIC_DATA)); 
        } 

		//使用新的数据建立vc和lsp的关系
		//to do

        //判断配置4)vpn属性是否存在， 不存在则退出(zhangzhm)
        if (NULL == pst_vc_tbl->vpn_property_cfg_cb)
        {
            goto EXIT_LABEL;
        }

		// 配置4 vpn属性存在，执行vc添加操作 (zhangzhm)
        else    
        {
            ret = spm_vc_add_vp_and_prot(pst_vc_tbl);
        }
        
    }

    else if (ul_oper_basic == ATG_DCI_OPER_DEL)
    { 
        NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
    }
    
    else if (ul_oper_basic == ATG_DCI_OPER_UPDATE)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;

}




/*****************************************************************************
 函 数 名  : spm_vc_del_all
 功能描述  : 删除一个vc条目
 输入参数  : pst_vc_tbl    
 输出参数  : 无
 返 回 值  : 
 调用函数  : spm_vc_process
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年10月19日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_del_all(SPM_VC_CB *pst_vc_tbl)    
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};	
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_CB *pst_vpls = NULL;
    SPM_SWITCH_VC_CB *pst_mspw = NULL;
    SPM_VPLS_NNI_CB *pst_vpls_nni = NULL; 
    NBB_USHORT vpn_id = 0;
    NBB_UINT vp_id = 0;                         
    NBB_BYTE vpn_type = 0; 
    NBB_INT ret = SUCCESS;
    NBB_INT vp_num = 0;
    
    NBB_TRC_ENTRY("spm_vc_del_nni_vp");
    
    if (NULL == pst_vc_tbl)
    {	
    	ret = DEL_VC_CFG_NULL;
        goto EXIT_LABEL;
    }

	//基本配置为空或者VPN为空，之前没有建立业务，不需要往下执行
	if((NULL == pst_vc_tbl->basic_cfg_cb)
		||  (NULL == pst_vc_tbl->vpn_property_cfg_cb))
	{
        goto EXIT_LABEL;
	}
    
    vp_id = pst_vc_tbl->vpn_property_cfg_cb->nni_no;
    vpn_id = pst_vc_tbl->vpn_property_cfg_cb->vpn_id;  
    vpn_type = pst_vc_tbl->vpn_property_cfg_cb->vpn_type;

	//记录重要信息
	SPM_L2_INFO_PRINT_LOG(uc_message,"INFO:del_or_mod nni,vpnid=%d,vpntype=%d,vpid=%d,dh_id=%d\n",
		vpn_id,vpn_type,vp_id,pst_vc_tbl->vpn_property_cfg_cb->bypass_dh_id);
	
   
    
    /* vpn类型为VPWS*/
    if(L2VPN_VPWS == vpn_type)
    {     
         /* 为NNI侧VP*/
        if (0 == pst_vc_tbl->vpn_property_cfg_cb->bypass_dh_id) 
        {  
            ret = spm_vc_del_or_mod_vpws_nni_vp(pst_vc_tbl);
			if(SUCCESS != ret)
			{
	            goto EXIT_LABEL;
			}
        }
		
		else
		{
			ret = spm_vc_drv_del_vpws_bypass_vp(pst_vc_tbl);
			if(SUCCESS != ret)
	        {
	            goto EXIT_LABEL;

	        }
		}
       
    }  

	else if(L2VPN_VPLS == vpn_type)
	{
        ret = spm_vc_del_or_mod_vpls_vp(pst_vc_tbl);
		
        if(SUCCESS != ret)
        {
            goto EXIT_LABEL;

        }
    }
    else if(L2VPN_MSPW == vpn_type)
    {
        ret = spm_vc_del_mspw_vp(pst_vc_tbl);
        if(SUCCESS != ret)
        {
            goto EXIT_LABEL;

        }
    }
	
    /* vpn类型错误*/
    else
    {
        ret = DEL_VC_VPN_TYPE_ERR;
        goto EXIT_LABEL;

    } 

	//删除业务成功之后，删除下一跳资源和LSP关系表
	if(0 != pst_vc_tbl->vc_info_cb.next_hop_id)
	{
		spm_free_vc_nhi_id(pst_vc_tbl->vc_info_cb.next_hop_id);
	}

	if(0 != pst_vc_tbl->vc_info_cb.vp_idx)
	{
		ret = spm_del_vc_lsp_rsvp_relation(pst_vc_tbl->vc_key);
		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",
				DEL_VC_LSP_RELATION_FAIL,ret);

			ret = SUCCESS;
		}
	}

	

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;    
}

/*****************************************************************************
   函 数 名  : spm_vc_add_bypass_vp
   功能描述  : 为VPWS添加一个bypass VP(NNI)
   输入参数  : pst_vpws
                   vc_key
                   pst_vc_basic_data
                   pst_vc_info
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :
  修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_add_bypass_vp(SPM_VPWS_CB *pst_vpws,
	SPM_VC_ADD_TYPE_E vc_add_type)
{

    NBB_INT ret = SUCCESS;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
	SPM_VPN_PORT_INFO_CB vpn_info;

	OS_MEMSET(&vpn_info,0,sizeof(vpn_info));

	if(NULL == pst_vpws)
	{
		return VC_ADD_BYPASS_VP_VPWS_NULL;
	}

	if(SPM_VC_ADD_TYPE_MOD == vc_add_type)
	{
		vpn_info.next_hop_id = pst_vpws->bypass_info.next_hop_id;
	}

	ret = spm_vc_drv_add_or_mod_bypass_vp(pst_vpws,vc_add_type,&vpn_info);

	if(SUCCESS == ret)
	{
		pst_vpws->bypass_info.next_hop_id = vpn_info.next_hop_id;
		pst_vpws->if_drv_bypass_vp_exist = DRV_NNI_VP_EXIST;
	}

	return ret;

    
}

NBB_INT spm_vc_del_vpws_bypass_vp(SPM_VC_CB *pst_vc)
{
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
	SPM_VPWS_CB *pst_vpws = NULL;
	

	if((NULL == pst_vc) || (NULL == pst_vc->vpn_property_cfg_cb))
	{
		return VC_DEL_BYPASS_VC_NULL;
	}
	
	pst_vpws = AVLL_FIND(v_spm_shared->vpws_tree,
		&pst_vc->vpn_property_cfg_cb->vpn_id);
	
	if(NULL == pst_vpws)
	{
		return VC_DRV_DEL_BYPASS_VPWS_NULL;
	}

	ret = spm_vc_drv_del_vpws_bypass_vp(pst_vc);
	if(SUCCESS != ret)
	{
		return ret;
	}

	OS_MEMSET(&pst_vpws->vckey_bypass,0,sizeof(ATG_DCI_VC_KEY));
	OS_MEMSET(&pst_vpws->bypass_info,0,sizeof(SPM_VPN_PORT_INFO_CB));
	pst_vpws->bypass_no = 0;
	pst_vpws->if_drv_bypass_vp_exist = 0;

	return ret;

	
}



/*****************************************************************************
 函 数 名  : spm_vc_diff_serv_proc
 功能描述  : VC表IPS消息Diff-Serv子配置处理函数
 输入参数  : ATG_DCI_SET_VC *pstSetVc
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月9日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_diff_serv_proc(ATG_DCI_SET_VC *pst_set_vc, NBB_ULONG uloperdiffserv,
    SPM_VC_CB *pst_vc_tbl)
{

    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];  
    NBB_INT ret = SUCCESS;
    ATG_DCI_VC_DIFF_SERV_DATA *pst_diff_serv_data = NULL;
    NBB_BYTE *puc_data_start = NULL;   

    NBB_TRC_ENTRY("spm_vc_diff_serv_proc");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pst_set_vc);

    if (NULL == pst_set_vc)
    {
       	ret = ADD_VC_DIFF_SERV_VC_IPS_IS_NULL;
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_vc_tbl)
    {
        ret = ADD_VC_DIFF_SERV_VC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }
   
    if (ATG_DCI_OPER_ADD == uloperdiffserv)
    {

        /* 计算第一个entry的地址。*/
        puc_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vc, &pst_set_vc->diff_serv_data);    

        /* 如果指针为NULL，无数据 */
        if (NULL == puc_data_start)
        {
            ret = ADD_VC_DIFF_SERV_DATA_START_IS_NULL;
            
            goto EXIT_LABEL;
        }
        
        /* 基本数据存在处理 (zhangzhm) */
        pst_diff_serv_data = (ATG_DCI_VC_DIFF_SERV_DATA *)puc_data_start;

        if (SPM_PRINT_CFG == g_vc_cfg_print_setting)
        {
            printf("  2）vc diff_serv cfg\n");
            spm_dbg_print_vc_diff_serv_cfg(pst_diff_serv_data);
        }

		SPM_L2_INFO_PRINT_LOG(uc_message,"  2）vc diff_serv cfg add\n");

        /* 保存数据处理，为空则申请新空间 */
        if (NULL == pst_vc_tbl->diff_serv_cfg_cb)
        {
            pst_vc_tbl->diff_serv_cfg_cb = 
                (ATG_DCI_VC_DIFF_SERV_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VC_DIFF_SERV_DATA),
                                                    NBB_NORETRY_ACT,
                                                    MEM_SPM_VC_DIFF_SERV_CB);

            if(NULL == pst_vc_tbl->diff_serv_cfg_cb)
            {
            	ret = ADD_VC_MALLOC_SERV_DATA_MEM_FAIL;
                goto EXIT_LABEL;
            }
        }

        /*数据为新申请或已存在，存储数据 */
        if(NULL != pst_vc_tbl->diff_serv_cfg_cb)   
        {
            OS_MEMCPY(pst_vc_tbl->diff_serv_cfg_cb, pst_diff_serv_data, 
				sizeof(ATG_DCI_VC_DIFF_SERV_DATA)); 
        } 

		//业务已经建立，调用QOS的接口

		if(0 != pst_vc_tbl->vc_info_cb.vp_idx)
		{
			ret = spm_drv_add_vc_diff_serv(pst_vc_tbl);
		}
		
    }

    else if (uloperdiffserv == ATG_DCI_OPER_DEL)
    { 
        NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
    }
    
    else if (uloperdiffserv == ATG_DCI_OPER_UPDATE)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;

}

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_vc
 功能描述  : VC表IPS消息处理函数
 输入参数  : ATG_DCI_SET_VC *pstSetVc
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月9日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_up_vpn_qos_policy_proc(ATG_DCI_SET_VC *pst_set_vc, NBB_ULONG oper_vc_hqos,
    SPM_VC_CB *pst_vc)
{

    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];  
    NBB_INT ret = SUCCESS;
    ATG_DCI_VC_UP_VPN_QOS_POLICY *pst_vc_hqos = NULL;
    NBB_BYTE *puc_data_start = NULL;   

    NBB_TRC_ENTRY("spm_vc_up_vpn_qos_policy_proc");
	
    if (NULL == pst_set_vc)
    {
       	ret = ADD_VC_HQOS_VC_IPS_IS_NULL;
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_vc)
    {
        ret = ADD_VC_HQOS_VC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }
   
    if (ATG_DCI_OPER_ADD == oper_vc_hqos)
    {

        /* 计算第一个entry的地址。*/
        puc_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vc, &pst_set_vc->up_vpn_qos_data);    

        /* 如果指针为NULL，无数据 */
        if (NULL == puc_data_start)
        {
            ret = ADD_VC_HQOS_START_IS_NULL;
            
            goto EXIT_LABEL;
        }
        
       
        pst_vc_hqos = (ATG_DCI_VC_UP_VPN_QOS_POLICY *)puc_data_start;

        if (SPM_PRINT_CFG == g_vc_cfg_print_setting)
        {
            printf("  3）add vc hqos cfg\n");
            spm_dbg_print_vc_hqos_cfg(pst_vc_hqos);
        }

		OS_SPRINTF(uc_message,"  3）add vc hqos cfg\n");
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message); 
		spm_dbg_record_vc_hqos_cfg(pst_vc_hqos);

        /* 保存数据处理，为空则申请新空间 */
        if (NULL == pst_vc->up_vpn_cfg_cb)
        {
            pst_vc->up_vpn_cfg_cb = 
                (ATG_DCI_VC_UP_VPN_QOS_POLICY *)NBB_MM_ALLOC(sizeof(ATG_DCI_VC_UP_VPN_QOS_POLICY),
                                                    NBB_NORETRY_ACT,
                                                    MEM_SPM_VC_UP_VPN_CB);

            if(NULL == pst_vc->up_vpn_cfg_cb)
            {
            	ret = ADD_VC_MALLOC_UP_VPN_QOS_MEM_FAIL;
                goto EXIT_LABEL;
            }
        }

        /*数据为新申请或已存在，存储数据 */
        if(NULL != pst_vc->up_vpn_cfg_cb)   
        {
            OS_MEMCPY(pst_vc->up_vpn_cfg_cb, pst_vc_hqos, 
				sizeof(ATG_DCI_VC_UP_VPN_QOS_POLICY)); 
        } 

		//业务已经建立，调用QOS的接口

		if(0 != pst_vc->vc_info_cb.vp_idx)
		{
			ret = spm_drv_add_vc_up_vpn_qos(pst_vc);
		}
		
    }

    else if (oper_vc_hqos == ATG_DCI_OPER_DEL)
    {

        /* 计算第一个entry的地址。*/
        puc_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vc, &pst_set_vc->up_vpn_qos_data);    

        /* 如果指针为NULL，无数据 */
        if (NULL == puc_data_start)
        {
            ret = ADD_VC_HQOS_START_IS_NULL;
            
            goto EXIT_LABEL;
        }

        if (SPM_PRINT_CFG == g_vc_cfg_print_setting)
        {
            printf("  3）del vc hqos cfg\n");
            spm_dbg_print_vc_hqos_cfg(pst_vc_hqos);
        }

		OS_SPRINTF(uc_message,"  3）del vc hqos cfg\n");
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message); 
		spm_dbg_record_vc_hqos_cfg(pst_vc_hqos);
		
		ret = spm_drv_del_vc_up_vpn_qos(pst_vc);
		if(SUCCESS == ret)
		{
			if (NULL != pst_vc->up_vpn_cfg_cb)
		    {
		        NBB_MM_FREE(pst_vc->up_vpn_cfg_cb, MEM_SPM_VC_UP_VPN_CB);  
		        pst_vc->up_vpn_cfg_cb = NULL;
		    }
		}
		
		
    }
    
    else if (oper_vc_hqos == ATG_DCI_OPER_UPDATE)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;

}




/*****************************************************************************
 函 数 名  : spm_vc_vpn_property_proc
 功能描述  : vc配置块vpn属性处理
 输入参数  : ATG_DCI_SET_VC *pst_set_vc                
             NBB_ULONG ul_oper_vpn                     
             SPM_VC_CB *pst_vc_tbl NBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月30日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_vpn_property_proc(
    ATG_DCI_SET_VC *pst_set_vc,
    NBB_ULONG ul_oper_vpn,
    SPM_VC_CB *pst_vc_tbl)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];  
    NBB_INT ret = SUCCESS;
    ATG_DCI_VC_VPN_DATA *pst_vpn_proper_data = NULL;
    NBB_BYTE *puc_vpn_proper_data_start = NULL; 
	
    if (NULL == pst_set_vc)
    {
        return ADD_VC_VPN_PROPERTY_VC_IPS_IS_NULL;
    }
    
    if (NULL == pst_vc_tbl)
    {
        return ADD_VC_VPN_PROPERTY_VC_CFG_IS_NULL;
    }
	
   
    /***************************************************************************/
    /*                        vpn属性包括保护配置                               */
    /***************************************************************************/
    if (ATG_DCI_OPER_ADD == ul_oper_vpn)
    {
        /* 计算第一个entry的地址。*/
        puc_vpn_proper_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vc, &pst_set_vc->vpn_data);    

        /* 如果指针为NULL，无数据 */
        if (NULL == puc_vpn_proper_data_start)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  vc vpn property is null."));  
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
        }
        else    /* 数据存在 (zhangzhm) */
        {
            pst_vpn_proper_data = (ATG_DCI_VC_VPN_DATA *)puc_vpn_proper_data_start;

            if (SPM_PRINT_CFG == g_vc_cfg_print_setting)
            {
                printf("  4）vpn property cfg add\n");
                spm_dbg_print_vc_vpn_cfg(pst_vpn_proper_data);
            }

			OS_SPRINTF(uc_message,"  4）vpn property cfg add\n");
			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message); 
			
			spm_dbg_record_vc_vpn_cfg(pst_vpn_proper_data);


			//判断数据是否合法,前后的vpn_id或者vpn_type不一样，是错误配置
			if(NULL != pst_vc_tbl->vpn_property_cfg_cb)
			{
				if(pst_vc_tbl->vpn_property_cfg_cb->vpn_type != pst_vpn_proper_data->vpn_type)
				{
					SPM_L2_ERROR_PRINT_LOG(uc_message,"vc vpn_type change!old=%d\n,new=%d\n",
						pst_vc_tbl->vpn_property_cfg_cb->vpn_type,pst_vpn_proper_data->vpn_type);
					
					return ADD_VC_VPN_PROPERTY_VPN_TYPE_CHANGE;
				}

				if(pst_vc_tbl->vpn_property_cfg_cb->vpn_id != pst_vpn_proper_data->vpn_id)
				{
					SPM_L2_ERROR_PRINT_LOG(uc_message,"vc vpn_id change!old=%d\n,new=%d\n",
						pst_vc_tbl->vpn_property_cfg_cb->vpn_id,pst_vpn_proper_data->vpn_id);
					
					return ADD_VC_VPN_PROPERTY_VPN_ID_CHANGE;
				}

				if(pst_vc_tbl->vpn_property_cfg_cb->nni_no != pst_vpn_proper_data->nni_no)
				{
					SPM_L2_ERROR_PRINT_LOG(uc_message,"vc nni_no change!old=%d\n,new=%d\n",
						pst_vc_tbl->vpn_property_cfg_cb->nni_no,pst_vpn_proper_data->nni_no);
					
					return ADD_VC_VPN_PROPERTY_NNI_NO_CHANGE;
				}

				if(pst_vc_tbl->vpn_property_cfg_cb->mc_id != pst_vpn_proper_data->mc_id)
				{
					SPM_L2_ERROR_PRINT_LOG(uc_message,"vc mc_id change!old=%d\n,new=%d\n",
						pst_vc_tbl->vpn_property_cfg_cb->mc_id,pst_vpn_proper_data->mc_id);
					
					return ADD_VC_VPN_PROPERTY_MC_ID_CHANGE;
				}


				
					
			}
            
            /* 保存数据处理，为空则申请新空间 */
            if (NULL == pst_vc_tbl->vpn_property_cfg_cb)   /*lint !e613 */  
            {
                pst_vc_tbl->vpn_property_cfg_cb = 
                    (ATG_DCI_VC_VPN_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VC_VPN_DATA),
                                                        NBB_NORETRY_ACT,
                                                        MEM_SPM_VC_VPN_PROPER_CB);/*lint !e613 */  

                if(NULL == pst_vc_tbl->vpn_property_cfg_cb) /*lint !e613 */
                {
                    return ADD_VC_VPN_PROPERTY_MALLOC_VPN_CFG_FAIL;
                }
            }

            /*数据为新申请或已存在，存储数据 */
            if(NULL != pst_vc_tbl->vpn_property_cfg_cb)   
            {
                OS_MEMCPY(pst_vc_tbl->vpn_property_cfg_cb, pst_vpn_proper_data, sizeof(ATG_DCI_VC_VPN_DATA)); 

				//由于主控暂时不能下发vc表中的mc id，暂时把vpn id当mc id使用
				//t_vc_tbl->vpn_property_cfg_cb->mc_id = pst_vc_tbl->vpn_property_cfg_cb->vpn_id;
            }  

            /* 判断配置1)基本配置是否存在， 不存在则退出(zhangzhm) */
            if (NULL == pst_vc_tbl->basic_cfg_cb)
            {
                return SUCCESS;
            }

			/*  配置1 存在，执行vc添加操作 (zhangzhm)   */  
            else    
            {
                ret = spm_vc_add_vp_and_prot(pst_vc_tbl);
            }
        }
    }

    else if (ul_oper_vpn == ATG_DCI_OPER_DEL)
    { 
        NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
    }
    
    else if (ul_oper_vpn == ATG_DCI_OPER_UPDATE)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
    }

    return ret;
}




/*****************************************************************************
   函 数 名  : spm_add_vc_lsp_rsvp_relation
   功能描述  : L2VPN NNI 业务建立的时候，建立VC和LSP、RSVP的关系
                             
   输入参数  : 无
   输出参数  : 无
   返 回 值  :     
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年9月26日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_vc_lsp_rsvp_relation(SPM_VP_INFO_REFRESH_CB *vp_info_refresh,
    NBB_ULONG vp_idx,NBB_ULONG nh_idx,NBB_ULONG tunnel_id)
{
    NBB_INT ret = SUCCESS;
    SPM_VC_CB *pstVc = NULL;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    LSPPROT_KEY lsp_prot_key;
	FTN_KEY ftn_key;
	CRTXLSP_KEY crlsp_key;
	

    OS_MEMSET(&lsp_prot_key,0,sizeof(LSPPROT_KEY));
	OS_MEMSET(&ftn_key,0,sizeof(FTN_KEY));
	OS_MEMSET(&crlsp_key,0,sizeof(CRTXLSP_KEY));

    if(NULL == vp_info_refresh)
    {
        return ADD_VC_LSP_RELATION_POINTER_NULL;
    }

    pstVc = AVLL_FIND(v_spm_shared->vc_tree,&vp_info_refresh->vc_key);

    if((NULL == pstVc) || (NULL == pstVc->basic_cfg_cb))
    {
        return ADD_VC_LSP_RELATION_VC_NULL;
    }

    if(ATG_DCI_FTN == pstVc->basic_cfg_cb->lsp_option)
    {
    	ftn_key.fec = pstVc->basic_cfg_cb->next_hop_ip;
        ftn_key.mask = 32;
        ftn_key.vrfid = 0;
		
        ret = spm_l3_addvpldp(vp_info_refresh,vp_idx,nh_idx,tunnel_id NBB_CCXT);
		if(SUCCESS !=ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_ADD_VP_FTN_ERR,ret);
			return ret;
		}
		
		if (L2VPN_VPLS == vp_info_refresh->vpn_type)
		{
			ret = spm_l3_add_mcid_ldp(vp_info_refresh->vpn_id,
				vp_info_refresh->mc_id,
				ATG_DCI_FTN,
				ftn_key,
				crlsp_key,
				vp_info_refresh->port_index);
			if(SUCCESS !=ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_ADD_VP_MC_FTN_ERR,ret);
				return ret;
			}
		}
		
    }

    else if(ATG_DCI_CR_LSP == pstVc->basic_cfg_cb->lsp_option)
    {
        lsp_prot_key.ingress = pstVc->basic_cfg_cb->ingress;
        lsp_prot_key.egress = pstVc->basic_cfg_cb->egress;
        lsp_prot_key.tunnelid = pstVc->basic_cfg_cb->tunnel_id;
        
        ret = spm_l3_addvprsvp(vp_info_refresh,vp_idx,nh_idx,&lsp_prot_key,tunnel_id);
		if(SUCCESS !=ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_ADD_VP_RSVP_ERR,ret);
			return ret;
		}

		if (L2VPN_VPLS == vp_info_refresh->vpn_type)
		{
			ret = spm_l3_add_mcid_rsvp(vp_info_refresh->vpn_id,
				vp_info_refresh->mc_id,
				&lsp_prot_key,
				vp_info_refresh->port_index);
			
			if(SUCCESS !=ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_ADD_VP_MC_RSVP_ERR,ret);
				return ret;
			}
		}
    }

	else
	{
		return ADD_VC_LSP_RELATION_LSP_OPTION_ERR;
	}
	
    return ret;
}




/*****************************************************************************
   函 数 名  : spm_del_vc_lsp_rsvp_relation
   功能描述  : L2VPN NNI 业务删除的时候，删除VC和LSP、RSVP的关系
                             
   输入参数  : 无
   输出参数  : 无
   返 回 值  :     
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年9月26日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_del_vc_lsp_rsvp_relation(ATG_DCI_VC_KEY vc_key)
{
    NBB_INT ret = SUCCESS;
    SPM_VC_CB *pstVc = NULL;
	SPM_VPLS_CB *pst_vpls = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    SPM_VC_INFO_CB stVcDrvInfo;
    LSPPROT_KEY lsp_prot_key;
	FTN_KEY ftn_key;
	CRTXLSP_KEY txlsp_key;

    OS_MEMSET(&stVcDrvInfo,0,sizeof(SPM_VC_INFO_CB));
    OS_MEMSET(&lsp_prot_key,0,sizeof(LSPPROT_KEY));
	OS_MEMSET(&ftn_key,0,sizeof(ftn_key));
	OS_MEMSET(&txlsp_key,0,sizeof(CRTXLSP_KEY));

    pstVc = AVLL_FIND(SHARED.vc_tree,&vc_key);
    if((NULL == pstVc) || (NULL == pstVc->basic_cfg_cb)
		|| (NULL == pstVc->vpn_property_cfg_cb))
    { 
        return DEL_VC_LSP_RELATION_VC_NULL;
    }

    spm_get_vc_drvinfo(vc_key,&stVcDrvInfo NBB_CCXT);

    if(ATG_DCI_FTN == pstVc->basic_cfg_cb->lsp_option)
    {
        ret = spm_l3_delvpldp(vc_key.peer_ip,stVcDrvInfo.vp_idx,
			stVcDrvInfo.next_hop_id NBB_CCXT);

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",VC_DEL_VP_FTN_ERR,ret);
			return ret;
			
		}

		if(L2VPN_VPLS == pstVc->vpn_property_cfg_cb->vpn_type)
		{
			pst_vpls = AVLL_FIND(v_spm_shared->vpls_tree,&pstVc->vpn_property_cfg_cb->vpn_id);
			if((NULL != pst_vpls) && (NULL != pst_vpls->basic_cfg_cb))
			{
				ftn_key.fec = pstVc->basic_cfg_cb->next_hop_ip;
		        ftn_key.mask = 32;
		        ftn_key.vrfid = 0;
				
				ret = spm_l3_del_mcid_ldp(pstVc->vpn_property_cfg_cb->vpn_id,
					pstVc->vpn_property_cfg_cb->mc_id,
					ATG_DCI_FTN,
					ftn_key,
					txlsp_key);
				if(SUCCESS != ret)
				{
					SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",VC_DEL_VP_MC_FTN_ERR,ret);
					return ret;
				}
			}
			
		}
    }

    else if(ATG_DCI_CR_LSP == pstVc->basic_cfg_cb->lsp_option)
    {
        lsp_prot_key.ingress = pstVc->basic_cfg_cb->ingress;
        lsp_prot_key.egress = pstVc->basic_cfg_cb->egress;
        lsp_prot_key.tunnelid = pstVc->basic_cfg_cb->tunnel_id;
        
        ret = spm_l3_delvprsvp(&lsp_prot_key,stVcDrvInfo.vp_idx, 
                    stVcDrvInfo.next_hop_id NBB_CCXT);

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",VC_DEL_VP_RSVP_ERR,ret);
			return ret;
			
		}

		if(L2VPN_VPLS == pstVc->vpn_property_cfg_cb->vpn_type)
		{
			pst_vpls = AVLL_FIND(v_spm_shared->vpls_tree,&pstVc->vpn_property_cfg_cb->vpn_id);
			if((NULL != pst_vpls) && (NULL != pst_vpls->basic_cfg_cb))
			{
				
				ret = spm_l3_del_mcid_rsvp(pstVc->vpn_property_cfg_cb->mc_id,&lsp_prot_key);
				if(SUCCESS != ret)
				{
					SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",VC_DEL_VP_MC_RSVP_ERR,ret);
					return ret;
				}
			}
			
		}
    }

	else
	{
		return DEL_VC_LSP_RELATION_LSP_OPTION_ERR;
	}

    return ret;

    
}



/*****************************************************************************
 函 数 名  : spm_vc_get_lspinfo
 功能描述  : 1、将vc挂到lsp上供反刷成树；2、从lsp中获取信息
            
 输入参数  : SPM_VC_CB *pst_vc_tbl  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月28日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_get_lspinfo(SPM_VC_CB *pst_vc_tbl,
	NBB_ULONG  *port_index, NBB_ULONG  *tunnel_id)
{
    NBB_INT ret = 0;
    
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};  
    
    ATG_DCI_VC_BASIC_DATA *pst_basic_data = NULL;    
    ATG_DCI_VC_VPN_DATA *pst_vpn_proper = NULL;     
    SPM_VC_INFO_CB *pst_vc_info = NULL;        

    NBB_UINT nhi_id = 0;                 
    NBB_ULONG ul_tunnel_index = 0;        
    NBB_ULONG  ul_port_index = 0;          

    CRTXLSP_KEY st_crlsp_key;              
    FTN_KEY ftn_key;                
    SPM_VP_INFO_REFRESH_CB st_vp_info_refresh;        

    
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    NBB_TRC_ENTRY("spm_vc_bind_lsp_and_get_lspinfo");
	
    if ((NULL == pst_vc_tbl) || (NULL == port_index)
		|| (NULL == tunnel_id))
    {
        ret = VC_GET_LSP_INFO_POINTER_NULL;
        goto EXIT_LABEL;
    }
   
    if ((NULL == pst_vc_tbl->basic_cfg_cb) || (NULL == pst_vc_tbl->vpn_property_cfg_cb))
    {
    	ret = VC_GET_LSP_INFO_VC_CFG_NULL;
        goto EXIT_LABEL;
    }

    NBB_MEMSET(&st_crlsp_key, 0, sizeof(CRTXLSP_KEY));
    NBB_MEMSET(&ftn_key, 0, sizeof(FTN_KEY));    
    NBB_MEMSET(&st_vp_info_refresh, 0, sizeof(SPM_VP_INFO_REFRESH_CB));

    pst_basic_data = pst_vc_tbl->basic_cfg_cb;    
    pst_vpn_proper = pst_vc_tbl->vpn_property_cfg_cb;
    pst_vc_info = &(pst_vc_tbl->vc_info_cb);

	if((ATG_DCI_FTN != pst_basic_data->lsp_option)
		&& (ATG_DCI_CR_LSP != pst_basic_data->lsp_option))
	{
		ret = VC_GET_LSP_INFO_LSP_OPTION_ERR;
		goto EXIT_LABEL;
	}

	ftn_key.fec = pst_basic_data->next_hop_ip;
    ftn_key.mask = 32;
    ftn_key.vrfid = 0;

	st_crlsp_key.ingress = pst_basic_data->ingress;
    st_crlsp_key.egress = pst_basic_data->egress;
    st_crlsp_key.tunnelid = pst_basic_data->tunnel_id;
    st_crlsp_key.lspid = 0;

	ret = spm_l3_get_tunl_info(&ftn_key,&st_crlsp_key,pst_basic_data->lsp_option, 
		port_index,tunnel_id NBB_CCXT);
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;    

}

/*****************************************************************************
   函 数 名  : spm_vc_pw_prot_proc
   功能描述  : 判断vc引用的tunnel是否有变化
                             
   输入参数  : 无
   输出参数  : 无
   返 回 值  :  0/-1 = 无变化/有变化   
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2016年1月22日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_vc_if_tunnel_change(ATG_DCI_VC_BASIC_DATA *old_cfg,
	ATG_DCI_VC_BASIC_DATA *new_cfg)
{
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];   
    NBB_INT ret = SUCCESS;
	
	NBB_TRC_ENTRY("spm_vc_if_tunnel_change");
	
	if((NULL == old_cfg) || (NULL == new_cfg))
	{
		ret = ERROR;
        goto EXIT_LABEL;
	}

	if(old_cfg->lsp_option != new_cfg->lsp_option)
	{
		ret = VC_TUNNEL_CHANGED;
	}

	else
	{
		if((ATG_DCI_FTN == old_cfg->lsp_option)
			&& (old_cfg->next_hop_ip != new_cfg->next_hop_ip))
		{
			ret = VC_TUNNEL_CHANGED;
		}

		else if((ATG_DCI_CR_LSP == old_cfg->lsp_option)
			&& ((old_cfg->egress != new_cfg->egress)
			 || (old_cfg->ingress != new_cfg->ingress)
			 || (old_cfg->tunnel_id != new_cfg->tunnel_id)))
		{
			ret = VC_TUNNEL_CHANGED;
		}
	}

	EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

NBB_INT spm_vc_mod_del_old_mc(SPM_VC_CB *pst_vc,NBB_ULONG port_index,NBB_USHORT mc_id)
{
	SPM_VPLS_CB *pst_vpls = NULL;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];   
    NBB_INT ret = SUCCESS;


	//配置不全，或者业务未建立成功，此处不需要再继续执行
	if((NULL == pst_vc) || (NULL == pst_vc->basic_cfg_cb)
		|| (NULL == pst_vc->vpn_property_cfg_cb)
		|| (0 == pst_vc->vc_info_cb.vp_idx))
	{
		return SUCCESS;
	}
	
	//业务类型不是VPLS，不需要继续执行
	if(L2VPN_VPLS != pst_vc->vpn_property_cfg_cb->vpn_type)
	{
		return SUCCESS;
	}

	pst_vpls = AVLL_FIND(v_spm_shared->vpls_tree,&pst_vc->vpn_property_cfg_cb->vpn_id);

	if((NULL == pst_vpls) || (NULL == pst_vpls->basic_cfg_cb))
	{
		return SUCCESS;
	}

	
}


/*****************************************************************************
   函 数 名  : spm_get_aps_pw_prot_info
   功能描述  : 组织APS模块需要的pw保护的信息
                             
   输入参数  : 无
   输出参数  : 无
   返 回 值  :     
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2016年1月26日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_get_aps_pw_prot_info(SPM_VC_CB *pst_vc_main,
 SPM_VC_CB *pst_vc_bak,NBB_BYTE prot_type,NBB_USHORT frr_id,
 SPM_LINEAR_PROTECT_SWITCH *aps_pw_prot)
{
	NBB_INT ret = SUCCESS;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN]; 

	if(NULL == aps_pw_prot)
	{
        return;
	}

	if((NULL != pst_vc_main) && (NULL != pst_vc_main->basic_cfg_cb)
		&& (NULL != pst_vc_main->vpn_property_cfg_cb))
	{
		aps_pw_prot->workEntity.portIndex = 0;
	    aps_pw_prot->workEntity.tpId = 0;
	    aps_pw_prot->workEntity.type = 1;
	    aps_pw_prot->workEntity.vckey.vc_id = pst_vc_main->vc_key.vc_id;
	    aps_pw_prot->workEntity.vckey.vc_type = pst_vc_main->vc_key.vc_type;
	    aps_pw_prot->workEntity.vckey.peer_ip = pst_vc_main->vc_key.peer_ip;
	}

	if((NULL != pst_vc_bak) && (NULL != pst_vc_bak->basic_cfg_cb)
		&& (NULL != pst_vc_bak->vpn_property_cfg_cb))
	{
		aps_pw_prot->revertiveType = (SPM_LPS_REVERTIVE_TYPE_E)pst_vc_bak->vpn_property_cfg_cb->if_return;
	    aps_pw_prot->holdOffTime = pst_vc_bak->vpn_property_cfg_cb->holdoff_time;
	    aps_pw_prot->wtrTime = pst_vc_bak->vpn_property_cfg_cb->restore_time;
		aps_pw_prot->protEntity.portIndex = 0;
		aps_pw_prot->protEntity.tpId = 0;
        aps_pw_prot->protEntity.type = 1;
        aps_pw_prot->protEntity.vckey.vc_id = pst_vc_bak->vc_key.vc_id;
        aps_pw_prot->protEntity.vckey.vc_type = pst_vc_bak->vc_key.vc_type;
        aps_pw_prot->protEntity.vckey.peer_ip = pst_vc_bak->vc_key.peer_ip;
	}

	aps_pw_prot->protType = FRR_TYPE;
    aps_pw_prot->protGroupId = frr_id;
    aps_pw_prot->lpsType = FRR_1B1;
    if (prot_type == 4)
    {
        aps_pw_prot->lpsType = FRR_UNIDIR;
        aps_pw_prot->vpId = pst_vc_bak->vpn_property_cfg_cb->nni_no;
    }

	return;
}

/*****************************************************************************
 函 数 名  : spm_vc_mod_vp_mc_id_in_vpls
 功能描述  : 如果建立VP的时候，VPLS的基本配置不存在，当时就没有建立组播；
 			  后面VPLS的基本配置下发之后，调用该接口进行VP的更新和组播成员的添加。
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
NBB_INT spm_vc_mod_vp_mc_id_in_vpls(NBB_USHORT vpls_id,NBB_USHORT mc_id)
{
	SPM_VPLS_CB *pst_vpls_cb = NULL;
	SPM_VPLS_NNI_CB *pst_vpls_nni_cb = NULL;
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;

	 for (pst_vpls_nni_cb = (SPM_VPLS_NNI_CB *)AVLL_FIRST(pst_vpls_cb->nni_cfg_tree);
         NULL != pst_vpls_nni_cb;
         pst_vpls_nni_cb = (SPM_VPLS_NNI_CB *)AVLL_NEXT(pst_vpls_cb->nni_cfg_tree, 
            pst_vpls_nni_cb->spm_vpls_nni_node))
     {
     	if(DRV_NNI_VP_EXIST == pst_vpls_nni_cb->if_drv_nni_vp_exist)
     	{
     		//to do
     		//***调用驱动的接口，进行VP和MC_id的绑定**
     		/*c3 app*/
     		
     		ret = spm_vc_drv_add_mc_nni_vp(vpls_id,mc_id,pst_vpls_nni_cb);
			if(SUCCESS != ret)
			{
				//error;
			}
			
     	}
     }

	return SUCCESS;
	
}





/*****************************************************************************
 函 数 名  : spm_vc_process
 功能描述  : VC表IPS消息处理函数
 输入参数  : ATG_DCI_SET_VC *pstSetVc
 输出参数  : 无
 返 回 值  : 
 调用函数  :                
 被调函数  : spm_vc_basic_data_proc
                 spm_vc_diff_serv_proc
                 spm_vc_up_vpn_qos_policy_proc
                 spm_vc_pw_prot_proc  
 修改历史      :
  1.日    期   : 2015年9月21日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_vc_process(ATG_DCI_SET_VC *pst_set_vc)
{

    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];   
    NBB_INT ret = SUCCESS;
    SPM_VC_CB *pst_vc_tbl     = NULL;
    NBB_ULONG ul_oper_basic    = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_diffserv = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_upvpn = ATG_DCI_OPER_NULL;

    /* vpn 属性操作 (zhangzhm) */
    NBB_ULONG ul_oper_vpnproperty = ATG_DCI_OPER_NULL;   

    ATG_DCI_VC_KEY st_vc_key;
    NBB_BYTE uc_if_exist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_vc_process");
    

    if (NULL == pst_set_vc)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"*ERR*,r1=%d\n",VC_IPS_NULL);
		goto EXIT_LABEL;
        
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pst_set_vc->return_code = ATG_DCI_RC_OK;
	pst_set_vc->basic_return_code = ATG_DCI_RC_OK;
	pst_set_vc->diff_serv_return_code = ATG_DCI_RC_OK;
	pst_set_vc->up_vpn_qos_return_code = ATG_DCI_RC_OK;
	pst_set_vc->vpn_return_code = ATG_DCI_RC_OK;
	pst_set_vc->pw_associate_return_code = ATG_DCI_RC_OK;
	pst_set_vc->connect_detect_return_code = ATG_DCI_RC_OK;

    OS_MEMSET(&st_vc_key, 0, sizeof(ATG_DCI_VC_KEY));

    st_vc_key.vc_id      = pst_set_vc->key.vc_id;
    st_vc_key.vc_type    = pst_set_vc->key.vc_type;
    st_vc_key.peer_ip    = pst_set_vc->key.peer_ip;
    
    /* 获取子配置操作 */
    ul_oper_basic    = pst_set_vc->oper_basic;
    ul_oper_diffserv = pst_set_vc->oper_diff_serv;
    ul_oper_upvpn	   = pst_set_vc->oper_up_vpn_qos; 
    ul_oper_vpnproperty = pst_set_vc->oper_vpn; 
    
    pst_vc_tbl = AVLL_FIND(SHARED.vc_tree, &st_vc_key);
    
    if (NULL == pst_vc_tbl)       /* 如果条目不存在*/
    {
        uc_if_exist = ATG_DCI_UNEXIST;
    }    
    
    /* 删除整个条目 */
    if (TRUE == pst_set_vc->delete_struct)
    {
        /* 如果条目不存在，不删除 */
        if (ATG_DCI_UNEXIST == uc_if_exist )
        {
        	SPM_L2_ERROR_PRINT_LOG(uc_message,"*ERR*,r1=%d\n",DEL_VC_CFG_NULL);
            goto EXIT_LABEL;
        }
        
         /* 存在，删除 */
        else
        {
            spm_dbg_print_vc_head(&st_vc_key, SPM_OPER_DEL);
            spm_dbg_record_vc_head(&st_vc_key, SPM_OPER_DEL);
            
            ret = spm_vc_del_all(pst_vc_tbl NBB_CCXT);

            if (SUCCESS != ret)
            {
                SPM_L2_ERROR_PRINT_LOG(uc_message,"*ERR*,spm_vc_del_all,ret=%d\n",ret);
				pst_vc_tbl->total_ret_code = ret;
				pst_vc_tbl->if_spm_del_failed = SPM_CFG_DEL_FAILED;

                goto EXIT_LABEL;
            }

			//从树中删除节点
            AVLL_DELETE(SHARED.vc_tree, pst_vc_tbl->spm_vc_node);  

            //释放VC表分配的内存空间
            spm_vc_free_cb(pst_vc_tbl NBB_CCXT);  
			pst_vc_tbl = NULL;
        }
    }
    
    /* 增加或更新条目 */    
    else
    {
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ATG_DCI_UNEXIST == uc_if_exist )
        {
            spm_dbg_print_vc_head(&st_vc_key, SPM_OPER_ADD);
            spm_dbg_record_vc_head(&st_vc_key, SPM_OPER_ADD);
            
            /* 申请一个新条目的内存空间 */
            pst_vc_tbl = spm_vc_alloc_cb(NBB_CXT);
            
            if(NULL == pst_vc_tbl)
            {
                SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d",ADD_VC_MALLOC_VC_MEM_NULL);

				pst_vc_tbl->total_ret_code = ADD_VC_MALLOC_VC_MEM_NULL;

                goto EXIT_LABEL;
            }
            
            else
            {
                //把条目插入到tree中
                OS_MEMCPY(&pst_vc_tbl->vc_key, &st_vc_key, sizeof(ATG_DCI_VC_KEY));

                //coverity[no_effect_test]
                AVLL_INSERT(SHARED.vc_tree, pst_vc_tbl->spm_vc_node);
            }
        }
        else
        {
            spm_dbg_print_vc_head(&st_vc_key, SPM_OPER_UPD);
            spm_dbg_record_vc_head(&st_vc_key, SPM_OPER_UPD);
        }

    
        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/

        /******************************** 基本配置 *********************************/        
        if (ATG_DCI_OPER_NULL != ul_oper_basic)
        {
            ret = spm_vc_basic_data_proc(pst_set_vc,ul_oper_basic,pst_vc_tbl);
			if (SUCCESS != ret)    
	        {
	        	SPM_L2_ERROR_PRINT_LOG(uc_message,"vc basic proc r1=%d\n",ret);
				pst_vc_tbl->basic_ret_code = ret;
	        }   
        }

        /******************************** Diff-Serv配置 *********************************/
        if (ATG_DCI_OPER_NULL != ul_oper_diffserv)
        {            
            ret = spm_vc_diff_serv_proc(pst_set_vc,ul_oper_diffserv,pst_vc_tbl);
			if (SUCCESS != ret)    
	        {
	        	SPM_L2_ERROR_PRINT_LOG(uc_message,"vc diff_serv proc r1=%d\n",ret);
				pst_vc_tbl->diff_serv_ret_code = ret;
	        }  
        }
		
        /******************************** 上话VPN QOS策略配置 *********************************/
        if (ATG_DCI_OPER_NULL != ul_oper_upvpn)
        {            
            ret = spm_vc_up_vpn_qos_policy_proc(pst_set_vc,ul_oper_upvpn,pst_vc_tbl);
			if (SUCCESS != ret)    
	        {
	        	SPM_L2_ERROR_PRINT_LOG(uc_message,"vc hqos proc r1=%d\n",ret);
				pst_vc_tbl->up_vpn_ret_code = ret;
	        }  
        }
        
        /******************************** vpn属性 PW保护配置 *********************************/
        if (ATG_DCI_OPER_NULL != ul_oper_vpnproperty)
        {            
            ret = spm_vc_vpn_property_proc(pst_set_vc,ul_oper_vpnproperty,pst_vc_tbl);

			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"vpn property proc r1=%d\n",ret);
				pst_vc_tbl->vpn_property_ret_code = ret;
			}
			
        }
        

        
        
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}


/*****************************************************************************
   函 数 名  : spm_free_all_port_mirror_cb
   功能描述  : 释放port_mirror配置所有的内存空间
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
NBB_VOID spm_free_all_switch_vc_cb(NBB_CXT_T NBB_CXT)
{
	SPM_SWITCH_VC_CB *pstSwitchVc = NULL;
	
    for (pstSwitchVc = (SPM_SWITCH_VC_CB*) AVLL_FIRST(SHARED.switch_vc_tree);
         pstSwitchVc != NULL;
         pstSwitchVc = (SPM_SWITCH_VC_CB*) AVLL_FIRST(SHARED.switch_vc_tree))
    {
    	
        AVLL_DELETE(SHARED.switch_vc_tree, pstSwitchVc->spm_switch_vc_node);      	
        spm_free_switch_vc_cb(pstSwitchVc NBB_CCXT);
    }
}


/*****************************************************************************
   函 数 名  : spm_free_all_vc_cb
   功能描述  : 释放vc配置所有的内存空间
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
NBB_VOID spm_free_all_vc_cb(NBB_CXT_T NBB_CXT)
{
	SPM_VC_CB *pstVcTbl = NULL;
	
    for (pstVcTbl = (SPM_VC_CB*) AVLL_FIRST(SHARED.vc_tree);
         pstVcTbl != NULL;
         pstVcTbl = (SPM_VC_CB*) AVLL_FIRST(SHARED.vc_tree))
    {
    	
		AVLL_DELETE(SHARED.vc_tree, pstVcTbl->spm_vc_node);     	
    	spm_vc_free_cb(pstVcTbl NBB_CCXT);
    }
}


/*****************************************************************************
   函 数 名  : spm_free_all_vpws_cb
   功能描述  : 释放vpws配置所有的内存空间
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
NBB_VOID spm_free_all_vpws_cb(NBB_CXT_T NBB_CXT)
{
	SPM_VPWS_CB *pstVpws = NULL;
	
    for (pstVpws = (SPM_VPWS_CB*) AVLL_FIRST(SHARED.vpws_tree);
         pstVpws != NULL;
         pstVpws = (SPM_VPWS_CB*) AVLL_FIRST(SHARED.vpws_tree))
    {
    	
		AVLL_DELETE(SHARED.vpws_tree, pstVpws->spm_vpws_node);     	
    	spm_vpws_free_cb(pstVpws NBB_CCXT);
    }
}

/*****************************************************************************
   函 数 名  : spm_free_bypass_vp_cb
   功能描述  : 释放VPWS结构体的内存
   输入参数  : SPM_VPWS_CB *pstVpws NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_bypass_vp_cb(SPM_BYPASS_VP_CB *pst_bypass_vp)
{
    NBB_TRC_ENTRY("spm_free_bypass_vp_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_bypass_vp, sizeof(SPM_BYPASS_VP_CB), MEM_SPM_BYPASS_VP_CB);

    if (NULL == pst_bypass_vp)
    {
    	goto EXIT_LABEL;
    }
	
    pst_bypass_vp->pst_vpws = NULL;
	
    //pst_bypass_vpws->bypass_cfg_cb = NULL;
	
    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_bypass_vp, MEM_SPM_BYPASS_VP_CB);
    pst_bypass_vp = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}


/*****************************************************************************
   函 数 名  : spm_alloc_bypass_vp_cb
   功能描述  : VPWS结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_VPWS_CB
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_BYPASS_VP_CB *spm_alloc_bypass_vp_cb()
{
    SPM_BYPASS_VP_CB *pst_bypass_vp = NULL;

    NBB_TRC_ENTRY("spm_alloc_bypass_vp_cb");

    /* 分配一个新的VPWS配置条目。*/
    pst_bypass_vp = (SPM_BYPASS_VP_CB *)NBB_MM_ALLOC(sizeof(SPM_BYPASS_VP_CB), 
    	NBB_NORETRY_ACT, MEM_SPM_BYPASS_VP_CB);
    
    if (NULL == pst_bypass_vp)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VPWS配置条目 */
    OS_MEMSET(pst_bypass_vp, 0, sizeof(SPM_BYPASS_VP_CB));
    pst_bypass_vp->pst_vpws = NULL;
	
    //pst_bypass_vpws->bypass_cfg_cb = NULL;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_bypass_vp->spm_bypass_vp_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_bypass_vp);
}

/*****************************************************************************
   函 数 名  : spm_init_bypass_dh_tree
   功能描述  : 初始化存储VPWS配置的树
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_bypass_dh_tree()
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_bypass_dh_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_BYPASS_DH_CB, bypass_dh_key);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.bypass_dh_tree, compare_ushort,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_BYPASS_DH_CB, spm_bypass_dh_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   函 数 名  : spm_free_bypass_dh_cb
   功能描述  : 释放VPWS结构体的内存
   输入参数  : SPM_VPWS_CB *pstVpws NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_bypass_dh_cb(SPM_BYPASS_DH_CB *pst_bypass_dh)
{
    SPM_BYPASS_VP_CB *pst_bypass_vp = NULL;
	
    NBB_TRC_ENTRY("spm_free_bypass_dh_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_bypass_dh, sizeof(SPM_BYPASS_DH_CB), MEM_SPM_BYPASS_DH_CB);

    if (NULL == pst_bypass_dh)
    {
    	goto EXIT_LABEL;
    }
	
    /***************************************************************************/
    /* 释放终结子接口配置数据块。                                              */
    /***************************************************************************/
    for (pst_bypass_vp = (SPM_BYPASS_VP_CB*) AVLL_FIRST(pst_bypass_dh->bypass_vp_tree);
        pst_bypass_vp != NULL;
        pst_bypass_vp = (SPM_BYPASS_VP_CB*) AVLL_FIRST(pst_bypass_dh->bypass_vp_tree))
    {
        AVLL_DELETE(pst_bypass_dh->bypass_vp_tree, pst_bypass_vp->spm_bypass_vp_node);
        spm_free_bypass_vp_cb(pst_bypass_vp);
    }
		
    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_bypass_dh, MEM_SPM_BYPASS_DH_CB);
    pst_bypass_dh = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}


/*****************************************************************************
   函 数 名  : spm_alloc_bypass_dh_cb
   功能描述  : VPWS结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_VPWS_CB
   调用函数  :
   被调函数  :

 修改历史      :
  1.日    期   : 2015年8月31日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_BYPASS_DH_CB *spm_alloc_bypass_dh_cb()
{
    SPM_BYPASS_DH_CB *pst_bypass_dh = NULL;

    NBB_TRC_ENTRY("spm_alloc_bypass_dh_cb");

    /* 分配一个新的VPWS配置条目。*/
    pst_bypass_dh = (SPM_BYPASS_DH_CB *)NBB_MM_ALLOC(sizeof(SPM_BYPASS_DH_CB), 
    	NBB_NORETRY_ACT, MEM_SPM_BYPASS_DH_CB);
    
    if (NULL == pst_bypass_dh)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VPWS配置条目 */
    OS_MEMSET(pst_bypass_dh, 0, sizeof(SPM_BYPASS_DH_CB));
	
    /***************************************************************************/
    /* 初始化用于存储SPM_FLOW_DIFF_SERV_CB的AVLL树.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pst_bypass_dh->bypass_vp_tree, compare_ushort,
        (NBB_USHORT) NBB_OFFSETOF(SPM_BYPASS_VP_CB, vp_idx),
        (NBB_USHORT) NBB_OFFSETOF(SPM_BYPASS_VP_CB, spm_bypass_vp_node));
	
    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_bypass_dh->spm_bypass_dh_node);

    EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_bypass_dh);
}

/*****************************************************************************
   函 数 名  : spm_free_all_bypass_dh_cb
   功能描述  : 释放vpws配置所有的内存空间
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
NBB_VOID spm_free_all_bypass_dh_cb(NBB_CXT_T NBB_CXT)
{
    SPM_BYPASS_DH_CB *pst_bypass_dh = NULL;
	
    for (pst_bypass_dh = (SPM_BYPASS_DH_CB*) AVLL_FIRST(SHARED.bypass_dh_tree);
         pst_bypass_dh != NULL;
         pst_bypass_dh = (SPM_BYPASS_DH_CB*) AVLL_FIRST(SHARED.bypass_dh_tree))
    {    	
        AVLL_DELETE(SHARED.bypass_dh_tree, pst_bypass_dh->spm_bypass_dh_node);     	
        spm_free_bypass_dh_cb(pst_bypass_dh);
    }
}

/*****************************************************************************
 函 数 名  : spm_vc_add_bypass_refresh
 功能描述  : 双归保护配置传递
 输入参数  : SPM_VPWS_CB *pstVpws       
             NBB_USHORT usAssociatedId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月28日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_add_bypass_refresh(
    SPM_VPWS_CB *pstVpws, 
    NBB_USHORT bypass_dh_id,
    NBB_USHORT associate_if_id,
    NBB_USHORT vp_idx)
{

    NBB_INT ret = SUCCESS;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    SPM_BYPASS_DH_CB *pst_bypass_dh = NULL;
    SPM_BYPASS_VP_CB *pst_bypass_vp = NULL;	
    NBB_BOOL avll_ret_code;
	
    NBB_TRC_ENTRY("spm_vc_add_bypass_refresh");

    if (NULL == pstVpws)
    {
    	ret = ERROR;
    	goto EXIT_LABEL;
    }
	
		pst_bypass_dh = AVLL_FIND(SHARED.bypass_dh_tree, &bypass_dh_id);
		
        if (NULL == pst_bypass_dh)
        {
            /* 申请一个新条目的内存空间 */
            pst_bypass_dh = spm_alloc_bypass_dh_cb();

            if(NULL == pst_bypass_dh)
            {
	        OS_SPRINTF(uc_message, "***ERROR***:spm_alloc_bypass_dh_cb alloc men fail\n");
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);      
                goto EXIT_LABEL;
            }

            //将新节点插入到tree中
            pst_bypass_dh->bypass_dh_key = bypass_dh_id;
            avll_ret_code = AVLL_INSERT(SHARED.bypass_dh_tree, pst_bypass_dh->spm_bypass_dh_node);
            NBB_ASSERT(avll_ret_code);

        }	

		pst_bypass_vp = AVLL_FIND(pst_bypass_dh->bypass_vp_tree, &vp_idx);
		
        if (NULL == pst_bypass_vp)
        {
            /* 申请一个新条目的内存空间 */
            pst_bypass_vp = spm_alloc_bypass_vp_cb();

            if(NULL == pst_bypass_vp)
            {
	        OS_SPRINTF(uc_message, "***ERROR***:spm_alloc_bypass_vp_cb alloc men fail\n");
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);          
                goto EXIT_LABEL;
            }

            //将新节点插入到tree中
            pst_bypass_vp->vp_idx = vp_idx;
		pst_bypass_dh->bypass_vp_num++;
            avll_ret_code = AVLL_INSERT(pst_bypass_dh->bypass_vp_tree, pst_bypass_vp->spm_bypass_vp_node);
            NBB_ASSERT(avll_ret_code);

        }
		
    pst_bypass_vp->pst_vpws = pstVpws;
    pst_bypass_vp->associate_if_id = associate_if_id;	

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}



/*****************************************************************************
 函 数 名  : spm_vc_del_bypass_refresh
 功能描述  : 双归保护配置传递
 输入参数  : SPM_VPWS_CB *pstVpws       
             NBB_USHORT usAssociatedId  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月28日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vc_del_bypass_refresh(
    SPM_VPWS_CB *pstVpws, 
    NBB_USHORT bypass_dh_id,
    NBB_USHORT vp_idx)
{

    NBB_INT ret = SUCCESS;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    SPM_BYPASS_DH_CB *pst_bypass_dh = NULL;
    SPM_BYPASS_VP_CB *pst_bypass_vp = NULL;	
    NBB_BOOL avll_ret_code;
	
    NBB_TRC_ENTRY("spm_vc_del_bypass_refresh");

    if(NULL == pstVpws)
    {
    	ret = ERROR;
    	goto EXIT_LABEL;
    }
	
		pst_bypass_dh = AVLL_FIND(SHARED.bypass_dh_tree, &bypass_dh_id);
		
        if (NULL != pst_bypass_dh)
        {
		pst_bypass_vp = AVLL_FIND(pst_bypass_dh->bypass_vp_tree, &vp_idx);
		
	        if (NULL != pst_bypass_vp)
	        {
                        //从树中删除节点
                        AVLL_DELETE(pst_bypass_dh->bypass_vp_tree, pst_bypass_vp->spm_bypass_vp_node);

                        //释放节点内存空间
                        spm_free_bypass_vp_cb(pst_bypass_vp);  
                        pst_bypass_dh->bypass_vp_num--;	        
	        }

        }	

        if (0 == pst_bypass_dh->bypass_vp_num)
        {
                        //从树中删除节点
                        AVLL_DELETE(SHARED.bypass_dh_tree, pst_bypass_dh->spm_bypass_dh_node);

                        //释放节点内存空间
                        spm_free_bypass_dh_cb(pst_bypass_dh);  

        }	

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}



#endif

