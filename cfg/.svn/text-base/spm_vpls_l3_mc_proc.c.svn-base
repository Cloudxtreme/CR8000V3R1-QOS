/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_vpls_mc_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年1月12日
  最近修改   :
  功能描述   : VPLS组播配置处理
  函数列表   :
  修改历史   :
  1.日    期   : 2013年1月12日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef VPLS_L3_MC_CFG

/*vpls组播配置块打印开关*/
extern unsigned char vpls_l3_mc_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_init_vpls_l3_mc_tree
 功能描述  : 初始化VPLS组播树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月12日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_vpls_l3_mc_tree(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_init_vpls_l3_mc_tree");

    AVLL_INIT_TREE(SHARED.vpls_l3_mc_tree, spm_vpls_l3_mc_key_compare,
        (NBB_USHORT) NBB_OFFSETOF(SPM_VPLS_L3_MC_CB, vpls_l3_mc_key),
        (NBB_USHORT) NBB_OFFSETOF(SPM_VPLS_L3_MC_CB, spm_vpls_l3_mc_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_alloc_vpls_l3_mc_cb
 功能描述  : 分配VPLS_L3组播内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : SPM_VPLS_L3_MC_CB
 调用函数  : 
 被调函数  : 
 
 修改历史  :
  1.
  日    期 : 2013年1月12日
  作    者 : xiaoxiang
  修改内容 : 新生成函数

*****************************************************************************/
SPM_VPLS_L3_MC_CB * spm_alloc_vpls_l3_mc_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_VPLS_L3_MC_CB *vpls_l3_mc = NULL;

    NBB_TRC_ENTRY("spm_alloc_vpls_l3_mc_cb");

    /* 分配一个新的VPLS_L3组播配置条目。*/
    vpls_l3_mc = (SPM_VPLS_L3_MC_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_L3_MC_CB), 
        NBB_NORETRY_ACT, MEM_SPM_VPLS_L3_MC_CFG);
    if (NULL == vpls_l3_mc)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:申请VPLS_L3组播条目空间为NULL!"));
        OS_PRINTF("***ERROR***:申请VPLS_L3组播条目空间为NULL!\n");
        
        goto EXIT_LABEL;
    }

    /* 初始化VPLS_L3组播配置条目 */
    OS_MEMSET(vpls_l3_mc, 0, sizeof(SPM_VPLS_L3_MC_CB));
    vpls_l3_mc->basic_cfg_cb = NULL;

    for (i = 0; i < ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        vpls_l3_mc->nni_cfg_cb[i] = NULL;        
    }

    for (i = 0; i < ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        vpls_l3_mc->uni_cfg_cb[i] = NULL;
    }

    /* 建立用于该VPLS_L3组播配置条目的句柄，作为异步消息交换的相关器。*/
    vpls_l3_mc->spm_vpls_l3_mc_handle = NBB_CREATE_HANDLE(vpls_l3_mc, 
        HDL_SPM_VPLS_L3_MC_CB);

    /* 成功分配一个新的VPLS_L3组播配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_L3_MC_CB allocated at %p with handle %#lx",
                   vpls_l3_mc, vpls_l3_mc->spm_vpls_l3_mc_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(vpls_l3_mc->spm_vpls_l3_mc_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(vpls_l3_mc);
}

/*****************************************************************************
 函 数 名  : spm_free_vpls_l3_mc_cb
 功能描述  : 释放VPLS组播结构体内存
 输入参数  : SPM_VPLS_MC_CB *pstVplsMc  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月12日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_vpls_l3_mc_cb(SPM_VPLS_L3_MC_CB *vpls_l3_mc NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_vpls_l3_mc_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(vpls_l3_mc, sizeof(SPM_VPLS_L3_MC_CB), 
        MEM_SPM_VPLS_L3_MC_CFG); 

    if (NULL == vpls_l3_mc)
    {
    	goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (vpls_l3_mc->basic_cfg_cb)
    {
        NBB_MM_FREE(vpls_l3_mc->basic_cfg_cb, MEM_SPM_VPLS_L3_MC_BASIC_CFG);  
        vpls_l3_mc->basic_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放NNI和UNI数据块。                                                */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (vpls_l3_mc->nni_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(vpls_l3_mc->nni_cfg_cb[i], MEM_SPM_VPLS_L3_MC_NNI_CFG);  
            vpls_l3_mc->nni_cfg_cb[i] = NULL;
        }
    }

    for (i = 0; i < ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        if (vpls_l3_mc->uni_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(vpls_l3_mc->uni_cfg_cb[i], MEM_SPM_VPLS_L3_MC_UNI_CFG);  
            vpls_l3_mc->uni_cfg_cb[i] = NULL;
        }
    } 

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(vpls_l3_mc->spm_vpls_l3_mc_handle, HDL_SPM_VPLS_L3_MC_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(vpls_l3_mc, MEM_SPM_VPLS_L3_MC_CFG);  
    vpls_l3_mc = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_vpls_l3_mc_del_vp_nni
 功能描述  : 删除VPLS_L3组播端口NNI
 输入参数  : NBB_USHORT mc_id                       
             SPM_VPLS_L3_MC_CB *vpls_l3_mc               
             ATG_DCI_VPLS_L3_MC_NNI_DATA* vpls_l3_mcNni  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史  :
  1.
  日    期 : 2013年1月28日
  作    者 : xiaoxiang
  修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_l3_mc_del_vp_nni(
    NBB_USHORT mc_id, 
	SPM_VPLS_L3_MC_CB *vpls_l3_mc, 
	ATG_DCI_VPLS_L3_MC_NNI_DATA* vpls_l3_mcNni NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

	NBB_INT i = 0;
	NBB_INT ret = SUCCESS;
    
	//NBB_BYTE c3_unit_id = 0;
	SPM_PORT_INFO_CB port_info;
	
	ATG_DCI_VC_KEY vc_key;
	SPM_VC_CB *pst_vc_tbl;
	
	NBB_TRC_ENTRY("spm_vpls_l3_mc_del_vp_nni");

	if (NULL == vpls_l3_mc)
	{
		ret = ERROR;
		goto EXIT_LABEL;
	}
	
	if (NULL == vpls_l3_mcNni)
	{
		NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:"
			"spm_vpls_l3_mc_del_vp_nni(vpls_l3_mcNni==NULL)"));

		OS_PRINTF("***ERROR***:spm_vpls_l3_mc_del_vp_nni"
			"(vpls_l3_mcNni==NULL)\n");

		OS_SPRINTF(uc_message, "***ERROR***:spm_vpls_l3_mc_del_vp_nni"
			"(vpls_l3_mcNni==NULL)\n");
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
		ret = ERROR;
		goto EXIT_LABEL;
	}

	OS_MEMSET(&port_info, 0, sizeof(SPM_PORT_INFO_CB));

	vc_key.vc_id	= vpls_l3_mcNni->vc_id;
	vc_key.vc_type = vpls_l3_mcNni->vc_type;
	vc_key.peer_ip = vpls_l3_mcNni->peer_ip;

	pst_vc_tbl = AVLL_FIND(SHARED.vc_tree, &vc_key);
	
	if (pst_vc_tbl == NULL)		/* 如果条目不存在*/
	{
		NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
			"vc_id=%ld, vc_type=%d, peer_ip=%s 的VC表配置不存在，"
			"无法删除此组播端口!", vc_key.vc_id,
			vc_key.vc_type,
			spm_set_ulong_to_ipv4(vc_key.peer_ip NBB_CCXT)));

		OS_PRINTF("***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s "
			"的VC表配置不存在，无法删除此组播端口!\n", vc_key.vc_id,
			vc_key.vc_type,
			spm_set_ulong_to_ipv4(vc_key.peer_ip NBB_CCXT));

		OS_SPRINTF(uc_message, "***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s "
			"的VC表配置不存在，无法删除此组播端口!\n", vc_key.vc_id,
			vc_key.vc_type,
			spm_set_ulong_to_ipv4(vc_key.peer_ip NBB_CCXT));
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
		ret = ERROR;
		goto EXIT_LABEL;
	}

	//根据VC表配置中的port_index获取端口的port_id和slot_id，配给Arad和FE1600
	spm_get_portid_from_logical_port_index(pst_vc_tbl->vc_info_cb.port_index, 
		&port_info NBB_CCXT);

	//1、如果组播端口接口或子接口，不在本槽位，或
	//2、如果组播端口为LAG，LAG成员端口的第一个端口所在的槽不在本槽位；或
	//3、如果组播端口为VE，不在本槽位
	//则不调用驱动，直接返回成功。
	if (((SHARED.local_slot_id != port_info.slot_id) && (ATG_DCI_INTFACE == port_info.port_type))
	 || ((SHARED.local_slot_id != port_info.slot_id) && (ATG_DCI_SUB_INTFACE == port_info.port_type))
	 || ((SHARED.local_slot_id != port_info.slot_id) && (ATG_DCI_VE_PORT == port_info.port_type))
	 || ((SHARED.local_slot_id != port_info.lag_slot_id) && (ATG_DCI_LAG == port_info.port_type)))
	{
		ret = SUCCESS;
		goto EXIT_LABEL;
	}

	for (i = 0; i < ATG_DCI_VPLS_MC_NNI_NUM; i++)
	{
		//找到要删除的VP
		if ((vpls_l3_mc->nni_cfg_cb[i] != NULL)
		 && (vpls_l3_mc->nni_cfg_cb[i]->vc_id == vpls_l3_mcNni->vc_id)
		 && (vpls_l3_mc->nni_cfg_cb[i]->vc_type == vpls_l3_mcNni->vc_type)
		 && (vpls_l3_mc->nni_cfg_cb[i]->peer_ip == vpls_l3_mcNni->peer_ip))
		{
			break;
		}
	}

	//未找到要删除的VP
	if (i == ATG_DCI_VPLS_MC_NNI_NUM)
	{
		ret = SUCCESS;
		goto EXIT_LABEL;
	}

	NBB_TRC_DETAIL((NBB_FORMAT "  mc_id=%d vp_id=%d usPortId=%d slot_id=%d", 
		mc_id, 
		vpls_l3_mc->nni_info_cb[i].vp_pos_id, 
		port_info.port_id, 
		port_info.slot_id));
	
#ifdef SPU 

	//如果是lag，删除109
	if (ATG_DCI_LAG == port_info.port_type)
	{
		ret = ApiAradDeleteEngressMcPort(UNIT_0, 
			mc_id, 
			SPM_MULTICAST_PORT_0, 
			vpls_l3_mc->nni_info_cb[i].vp_pos_id);	
	}
	else
	{
		//如果VP属于第1片c3，Arad配置到109
		if (port_info.unit_id == UNIT_0)
		{
			ret = ApiAradDeleteEngressMcPort(UNIT_0, 
				 mc_id, 
				 SPM_MULTICAST_PORT_0, 
				 vpls_l3_mc->nni_info_cb[i].vp_pos_id);	
		}
		else	//如果VP属于第2片c3，Arad配置到110
		{
			ret = ApiAradDeleteEngressMcPort(UNIT_0, 
				mc_id, 
				SPM_MULTICAST_PORT_1, 
				vpls_l3_mc->nni_info_cb[i].vp_pos_id);
		}
	}

	if (ret != SUCCESS)
	{
		NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradDeleteEngressMcPort() "
            "ret=%d MC_ID=%ld VpId=%d", 
            ret, mc_id, vpls_l3_mc->nni_info_cb[i].vp_pos_id));
		
		OS_PRINTF("***ERROR***: ApiAradDeleteEngressMcPort() "
            "ret=%d MC_ID=%d VpId=%ld\n", 
            ret, mc_id, vpls_l3_mc->nni_info_cb[i].vp_pos_id);

		OS_SPRINTF(uc_message, "***ERROR***: ApiAradDeleteEngressMcPort() "
            "ret=%d MC_ID=%d VpId=%ld\n", 
            ret, mc_id, vpls_l3_mc->nni_info_cb[i].vp_pos_id);
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
	}
#endif

	EXIT_LABEL : NBB_TRC_EXIT();

	return ret;
}

/*****************************************************************************
 函 数 名  : spm_vpls_l3_mc_del_vp_uni
 功能描述  : 删除VPLS_L3组播端口UNI
 输入参数  : NBB_USHORT mc_id                      
             SPM_VPLS_L3_MC_CB *vpls_l3_mc              
             ATG_DCI_VPLS_MC_UNI_DATA* uni_index  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史  :
  1.
  日    期 : 2013年1月28日
  作    者 : xiaoxiang
  修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_l3_mc_del_vp_uni(
    NBB_USHORT mc_id, 
    SPM_VPLS_L3_MC_CB *vpls_l3_mc, 
    ATG_DCI_VPLS_MC_UNI_DATA* uni_index NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    
    NBB_BYTE i = 0;
    NBB_INT ret = SUCCESS;
    
    //NBB_BYTE c3_unit_id = 0;
    SPM_PORT_INFO_CB port_info;

    NBB_ULONG port_index;
    SPM_LOGICAL_PORT_CB *pst_ogical_port;
    
    NBB_TRC_ENTRY("spm_vpls_l3_mc_del_vp_uni");

    if (vpls_l3_mc == NULL)
    {
        ret = ERROR;
        goto EXIT_LABEL;
    }

    if (uni_index == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_l3_mc_del_vp_uni"
            "(uni_index==NULL)\n"));

        OS_PRINTF("***ERROR***:spm_vpls_l3_mc_del_vp_uni(uni_index==NULL)\n");

        OS_SPRINTF(uc_message, "***ERROR***:spm_vpls_l3_mc_del_vp_uni(uni_index==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        ret = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&port_info, 0, sizeof(SPM_PORT_INFO_CB));

    port_index = uni_index->port_index;

    pst_ogical_port = AVLL_FIND(SHARED.logical_port_tree, &port_index);
    
    if (pst_ogical_port == NULL)       /* 如果条目不存在*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: PORT_INDEX=%ld "
            "的逻辑端口配置不存在，无法删除此组播端口!", uni_index->port_index));

        OS_PRINTF("***ERROR***: PORT_INDEX=%ld 的逻辑端口配置不存在，"
            "无法删除此组播端口!\n", uni_index->port_index);

        OS_SPRINTF(uc_message, "***ERROR***: PORT_INDEX=%ld 的逻辑端口配置不存在，"
            "无法删除此组播端口!\n", uni_index->port_index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //根据逻辑端口配置中的port_index获取端口的port_id和slot_id，配给Arad和FE1600
    spm_get_portid_from_logical_port_index(port_index, &port_info NBB_CCXT);

    //1、如果组播端口接口或子接口，不在本槽位，或
    //2、如果组播端口为LAG，LAG成员端口的第一个端口所在的槽不在本槽位；或
    //3、如果组播端口为VE，不在本槽位
    //则不调用驱动，直接返回成功。
    if (((SHARED.local_slot_id != port_info.slot_id) && (ATG_DCI_INTFACE == port_info.port_type))
     || ((SHARED.local_slot_id != port_info.slot_id) && (ATG_DCI_SUB_INTFACE == port_info.port_type))
     || ((SHARED.local_slot_id != port_info.slot_id) && (ATG_DCI_VE_PORT == port_info.port_type))
     || ((SHARED.local_slot_id != port_info.lag_slot_id) && (ATG_DCI_LAG == port_info.port_type)))
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }

    for (i = 0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        //找到要删除的VP
        if ((vpls_l3_mc->uni_cfg_cb[i] != NULL)
         && (vpls_l3_mc->uni_cfg_cb[i]->port_index == port_index))
        {
            break;
        }
    }

    //未找到要删除的VP
    if (i == ATG_DCI_VPLS_MC_UNI_NUM)
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }

    NBB_TRC_DETAIL((NBB_FORMAT "  mc_id=%d vp_id=%d usPortId=%d slot_id=%d", 
        mc_id, 
        vpls_l3_mc->uni_info_cb[i].vp_pos_id, 
        port_info.port_id, 
        port_info.slot_id));
    
#ifdef SPU 
    
    //如果是lag，删除109
    if (ATG_DCI_LAG == port_info.port_type)
    {
        ret = ApiAradDeleteEngressMcPort(UNIT_0, 
            mc_id, 
            SPM_MULTICAST_PORT_0, 
            vpls_l3_mc->uni_info_cb[i].vp_pos_id);
    }
    else
    {
        //如果VP属于第1片c3，Arad配置到109
        if (port_info.unit_id == UNIT_0)
        {
            ret = ApiAradDeleteEngressMcPort(UNIT_0, 
                mc_id, 
                SPM_MULTICAST_PORT_0, 
                vpls_l3_mc->uni_info_cb[i].vp_pos_id);
        }
        else    //如果VP属于第2片c3，Arad配置到110
        {
            ret = ApiAradDeleteEngressMcPort(UNIT_0, 
                mc_id, 
                SPM_MULTICAST_PORT_1, 
                vpls_l3_mc->uni_info_cb[i].vp_pos_id);
        }
    }

    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradDeleteEngressMcPort() "
            "ret=%d MC_ID=%ld VpId=%d", 
            ret, mc_id, vpls_l3_mc->nni_info_cb[i].vp_pos_id));
        
        OS_PRINTF("***ERROR***: ApiAradDeleteEngressMcPort() "
            "ret=%d MC_ID=%d VpId=%ld\n", 
            ret, mc_id, vpls_l3_mc->nni_info_cb[i].vp_pos_id);

        OS_SPRINTF(uc_message, "***ERROR***: ApiAradDeleteEngressMcPort() "
            "ret=%d MC_ID=%d VpId=%ld\n", 
            ret, mc_id, vpls_l3_mc->nni_info_cb[i].vp_pos_id);

        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
    }

#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_get_vpls_l3_mc_slot_num
 功能描述  : 获取在slot_id槽位端口的个数
 输入参数  : SPM_VPLS_MC_CB *pstVplsMc  
             NBB_BYTE slot_id            
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年2月4日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_vpls_l3_mc_slot_num(
    SPM_VPLS_L3_MC_CB *vpls_l3_mc, 
    NBB_BYTE slot_id NBB_CCXT_T NBB_CXT)
{
    NBB_INT i = 0;
    NBB_INT num = 0;
    
    NBB_TRC_ENTRY("spm_get_vpls_l3_mc_slot_num");

    if (NULL == vpls_l3_mc)
    {
    	goto EXIT_LABEL;
    }

    //NNI中在此槽位的端口个数
    for (i = 0; i < ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (NULL == vpls_l3_mc->nni_cfg_cb[i])
        {
            continue;
        }
        if (slot_id == vpls_l3_mc->nni_info_cb[i].slot_id)
        {
            num++;
        }
    }

    //UNI中在此槽位的端口个数
    for (i = 0; i < ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        if (NULL == vpls_l3_mc->uni_cfg_cb[i])
        {
            continue;
        }
        if (slot_id == vpls_l3_mc->uni_info_cb[i].slot_id)
        {
            num++;
        }

    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return num;
}

/*****************************************************************************
   函 数 名  : spm_if_vpls_l3_mc_nni_exist
   功能描述  : 判断NNI组播成员是否存在,返回存储位置
   输入参数  : SPM_VPLS_L3_MC_CB *vpls_l3_mc
			   ATG_DCI_VPLS_L3_MC_NNI_DATA stNniData
   输出参数  : 无
   返 回 值  : -1 ：不存在
               n  ：存在的位置
   调用函数  :
   被调函数  :
   修改历史  :
   1.
   日    期  : 2014年4月11日
   作    者  : zhangsen
   修改内容  : 新生成函数
*****************************************************************************/
NBB_INT spm_if_vpls_l3_mc_nni_exist(
    const SPM_VPLS_L3_MC_CB *vpls_l3_mc, 
    const ATG_DCI_VPLS_L3_MC_NNI_DATA *nni_data NBB_CCXT_T NBB_CXT)
{
    NBB_INT i = 0;
    NBB_INT nni_num = 1;
    NBB_INT exist_pos = -1;

    //VPLS_L3组播配置不存在
    if (NULL == vpls_l3_mc)
    {
        return -1;
    }

    //VPLS_L3组播不存在NNI配置
    if (0 == vpls_l3_mc->nni_num)
    {
        return -1;
    }

    for (i = 0; i < ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (vpls_l3_mc->nni_cfg_cb[i])
        {
            /*找到，返回存储位置*/
            if ((nni_data->vc_id == vpls_l3_mc->nni_cfg_cb[i]->vc_id)
    		    && (nni_data->peer_ip == vpls_l3_mc->nni_cfg_cb[i]->peer_ip)
    		    && (nni_data->vc_type == vpls_l3_mc->nni_cfg_cb[i]->vc_type))
            {
                exist_pos = i;
                break;
            }

            /*记录NNI数组中不为空的成员数量*/
            if (nni_num < vpls_l3_mc->nni_num)
            {
                nni_num++;
            }
            else//NNI数组中所有存储了数据的成员都已遍历，退出
            {
                break;
            }
        }
    }

    return exist_pos;
}

/*****************************************************************************
   函 数 名  : spm_if_vpls_l3_mc_uni_exist
   功能描述  : 判断UNI组播成员是否存在,返回存储位置
   输入参数  : SPM_VPLS_L3_MC_CB *vpls_l3_mc
			   ATG_DCI_VPLS_L3_MC_UNI_DATA uni_data
   输出参数  : 无
   返 回 值  : -1 --不存在
               n  --存在的位置
   调用函数  :
   被调函数  :
   修改历史  :
   1.
   日    期  : 2014年4月11日
   作    者  : zhangsen
   修改内容  : 新生成函数
*****************************************************************************/
NBB_INT spm_if_vpls_l3_mc_uni_exist(
    const SPM_VPLS_L3_MC_CB *vpls_l3_mc, 
    const ATG_DCI_VPLS_L3_MC_UNI_DATA *uni_data NBB_CCXT_T NBB_CXT)
{
    NBB_INT i = 0;
    NBB_INT uni_num = 1;
    NBB_INT exist_pos = -1;

    //VPLS_L3组播配置不存在
    if (NULL == vpls_l3_mc)
    {
        return -1;
    }

    //VPLS_L3组播不存在UNI配置
    if (0 == vpls_l3_mc->uni_num)
    {
        return -1;
    }

    for (i = 0; i < ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        if (vpls_l3_mc->uni_cfg_cb[i])
        {
            /*找到，返回存储位置*/
            if (uni_data->port_index == vpls_l3_mc->uni_cfg_cb[i]->port_index)
            {
                exist_pos = i;
                break;
            }

            /*记录UNI数组中不为空的成员数量*/
            if (uni_num < vpls_l3_mc->uni_num)
            {
                uni_num++;
            }
            else//UNI数组中所有存储了数据的成员都已遍历，退出
            {
                break;
            }
        }
    }

    return exist_pos;
}

/*****************************************************************************
 函 数 名  : spm_delete_vpls_l3_mc_cb
 功能描述  : 删除VPLS_L3_MC配置
 输入参数  : 该VPLS_L3_MC结构体的指针vpls_l3_mc,
             该VPLS_L3_MC的key值vpls_l3_mc_key,
             该VPLS_L3_MC存在与否的标志if_exist.
 输出参数  : 删除成功与否标志ret.
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史  :
  1.
  日    期 : 2014年4月8日
  作    者 : zhangsen
  修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_delete_vpls_l3_mc_cb(
    SPM_VPLS_L3_MC_CB *vpls_l3_mc, 
    ATG_DCI_VPLS_L3_MC_KEY vpls_l3_mc_key, 
    NBB_BYTE if_exist NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_INT ret = SUCCESS;//返回值

#ifdef SNU
    NBB_BYTE unit_id = 0;
#endif

#ifdef SPU
    NBB_BYTE c3_unit_id = 0;
#endif
                
    NBB_TRC_ENTRY("spm_delete_vpls_l3_mc_cb");      

    /* 如果条目不存在，不删除，记录并返回错误 */
    if ((NULL == vpls_l3_mc) || (ATG_DCI_UNEXIST == if_exist))
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在，退出"));

        OS_PRINTF("***ERROR***:要删除的VplsId=%d SrcIp=0x%lx-0x%lx-0x%lx-0x%lx "
            "GrpIp=0x%lx-0x%lx-0x%lx-0x%lx的VPLS_L3组播配置不存在!\n", 
            vpls_l3_mc_key.vpls_id,
            vpls_l3_mc_key.src_ip[0],
            vpls_l3_mc_key.src_ip[1],
            vpls_l3_mc_key.src_ip[2],
            vpls_l3_mc_key.src_ip[3],
            vpls_l3_mc_key.grp_ip[0],
            vpls_l3_mc_key.grp_ip[1],
            vpls_l3_mc_key.grp_ip[2],
            vpls_l3_mc_key.grp_ip[3]);

        OS_SPRINTF(uc_message, 
            "***ERROR***:要删除的VplsId=%d SrcIp=0x%lx-0x%lx-0x%lx-0x%lx "
            "GrpIp=0x%lx-0x%lx-0x%lx-0x%lx的VPLS_L3组播配置不存在!\n", 
            vpls_l3_mc_key.vpls_id,
            vpls_l3_mc_key.src_ip[0],
            vpls_l3_mc_key.src_ip[1],
            vpls_l3_mc_key.src_ip[2],
            vpls_l3_mc_key.src_ip[3],
            vpls_l3_mc_key.grp_ip[0],
            vpls_l3_mc_key.grp_ip[1],
            vpls_l3_mc_key.grp_ip[2],
            vpls_l3_mc_key.grp_ip[3]);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

        ret = ERROR;
        goto EXIT_LABEL;
    }
    else//条目存在，删除
    {
        NBB_TRC_FLOW((NBB_FORMAT "  查询到此配置，删除此VPLS_L3组播，"
            "并从tree中删除存储数据"));

        //打印，todo
        spm_dbg_print_vpls_l3_mc_head(&vpls_l3_mc_key, SPM_OPER_DEL);
        spm_dbg_record_vpls_l3_mc_head(&vpls_l3_mc_key, SPM_OPER_DEL);

        //删除VPLS_L3组播组，要求删除后下挂的组播端口由驱动自动删除
        //Arad删除组播组后自动删除下挂端口
        if (vpls_l3_mc->basic_cfg_cb)
        {
            #ifdef SPU			
            ret = ApiAradDeleteMc(UNIT_0, vpls_l3_mc->basic_cfg_cb->mc_id);
            if (SUCCESS != ret)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除VPLS_L3_MC"
                    "(VPLS_ID=%d MC_ID=%d)的组播组失败! >>>"
                    " ApiAradDeleteMc() ret=%d", vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret));
                OS_PRINTF("***ERROR***:删除VPLS_L3_MC(VPLS_ID=%d MC_ID=%d)"
                    "的组播组失败! >>> ApiAradDeleteMc() ret=%d\n", 
                    vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret);

                OS_SPRINTF(uc_message, "***ERROR***:删除VPLS_L3_MC"
                    "(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> "
                    "ApiAradDeleteMc() ret=%d\n", 
                    vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                ret = ERROR;
                goto EXIT_LABEL;
            }

            for (c3_unit_id = 0; c3_unit_id < SHARED.c3_num; c3_unit_id++)
            {
                ret = fhdrv_psn_arp_del_igmpsnooping(c3_unit_id, vpls_l3_mc->ulIgmpSnoopPosId);
            }

            NBB_TRC_DETAIL((NBB_FORMAT "  fhdrv_psn_arp_del_igmpsnooping() ret=%d", ret));
            if (SUCCESS != ret)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:fhdrv_psn_arp_del_igmpsnooping()"
                    " ret=%d pos_id=%ld", ret, vpls_l3_mc->ulIgmpSnoopPosId));
                OS_PRINTF("  ***ERROR***:fhdrv_psn_arp_del_igmpsnooping()"
                    " ret=%d pos_id=%ld", ret, vpls_l3_mc->ulIgmpSnoopPosId);

                OS_SPRINTF(uc_message, "  ***ERROR***:fhdrv_psn_arp_del_igmpsnooping()"
                    " ret=%d pos_id=%ld", ret, vpls_l3_mc->ulIgmpSnoopPosId);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                ret = ERROR;
                goto EXIT_LABEL;
            }

            #endif

            #ifdef SRC 

            /* 删除VPLS组播组，要求删除后下挂的组播端口由驱动自动删除*/ 
            ret = ApiDfeMulticastDestroy(UNIT_0, vpls_l3_mc->basic_cfg_cb->mc_id);

            if (ret != SUCCESS)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除VPLS_MC"
                    "(VPLS_ID=%d MC_ID=%d)的组播组失败! "
                    ">>> ApiDfeMulticastDestroy() ret=%d", 
                    vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret));

                OS_PRINTF("***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)"
                    "的组播组失败! >>> ApiDfeMulticastDestroy() ret=%d\n", 
                    vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret);

                OS_SPRINTF(uc_message, "***ERROR***:删除VPLS_MC"
                    "(VPLS_ID=%d MC_ID=%d)的组播组失败! >>>"
                    " ApiDfeMulticastDestroy() ret=%d\n", 
                    vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                ret = ERROR;
                goto EXIT_LABEL;
            }
            #endif

            #ifdef SNU

            /* 删除VPLS组播组，要求删除后下挂的组播端口由驱动自动删除*/ 
            ret += ApiDfeMulticastDestroy(UNIT_0, 
                vpls_l3_mc->basic_cfg_cb->mc_id);
            

            if (ret != SUCCESS)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除VPLS_MC"
                    "(VPLS_ID=%d MC_ID=%d)的组播组失败! "
                    ">>> ApiDfeMulticastDestroy() ret=%d", 
                    vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret));

                OS_PRINTF("***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)"
                    "的组播组失败! >>> ApiDfeMulticastDestroy() ret=%d\n", 
                    vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret);

                OS_SPRINTF(uc_message, "***ERROR***:删除VPLS_MC"
                    "(VPLS_ID=%d MC_ID=%d)的组播组失败! >>>"
                    " ApiDfeMulticastDestroy() ret=%d\n", 
                    vpls_l3_mc_key.vpls_id, 
                    vpls_l3_mc->basic_cfg_cb->mc_id,
                    ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                ret = ERROR;
                goto EXIT_LABEL;
            }
            #endif
        }

        //从树中删除节点
        AVLL_DELETE(SHARED.vpls_l3_mc_tree, vpls_l3_mc->spm_vpls_l3_mc_node); 

        //释放节点内存空间
        spm_free_vpls_l3_mc_cb(vpls_l3_mc NBB_CCXT);
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_vpls_l3_mc_basic_cb_proc
 功能描述  : VPLS_L3_MC基本配置处理函数
 输入参数  : VPLS_L3_MC的IPS消息指针pst_set_vpls_l3_mc,
             该VPLS_L3_MC结构体的指针vpls_l3_mc,
             IPS消息中基本配置的操作模式oper_basic,
             该VPLS_L3_MC存在与否的标志if_exist.
 输出参数  : 
 返 回 值  : 函数返回值ret
 调用函数  : 
 被调函数  : 
 
 修改历史  :
  1.
  日    期 : 2014年4月8日
  作    者 : zhangsen
  修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_l3_mc_basic_cb_proc(
    ATG_DCI_SET_VPLS_L3_MC *pst_set_vpls_l3_mc, 
    SPM_VPLS_L3_MC_CB *vpls_l3_mc, 
    NBB_ULONG oper_basic, 
    NBB_BYTE if_exist NBB_CCXT_T NBB_CXT)
{
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_VPLS_L3_MC_BASIC_DATA *basic_data = NULL;
    IGMPSNOOPING_T igmp_snoop;

    NBB_BYTE *basic_data_start  = NULL;                /* IPS消息偏移的首地址 */
    NBB_INT ret = SUCCESS;                             /*返回值*/   

    /*存放错误信息的字符数组*/
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

#ifdef SNU
    NBB_BYTE unit_id = 0;
#endif

#ifdef SPU
    NBB_BYTE c3_unit_id = 0;
#endif

    NBB_TRC_ENTRY("spm_vpls_l3_mc_basic_cb_proc");

    //入参合法性检验
    if (NULL == pst_set_vpls_l3_mc)
    {
        NBB_TRC_DETAIL((NBB_FORMAT"  ***ERROR***: pst_set_vpls_l3_mc为NULL!"));
        OS_SPRINTF(uc_message, "***ERROR***: pst_set_vpls_l3_mc为NULL!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        ret = ERROR;
        goto EXIT_LABEL;
    }
    if (NULL == vpls_l3_mc)
    {
        NBB_TRC_DETAIL((NBB_FORMAT"  ***ERROR***: vpls_l3_mc为NULL!"));
        OS_SPRINTF(uc_message, "***ERROR***: vpls_l3_mc为NULL!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //初始化结构体
    OS_MEMSET(&igmp_snoop, 0, sizeof(IGMPSNOOPING_T));
    
    switch(oper_basic)
    {
        /*基本配置操作为删除，打印PD，不处理*/
        case ATG_DCI_OPER_DEL:                          
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            break;

        /*基本配置操作为更新，打印PD，不处理*/
        case ATG_DCI_OPER_UPDATE:                       
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            break;

        /*基本配置操作为增加，进入处理流程*/
        case ATG_DCI_OPER_ADD:

            /* 计算第一个entry的地址。*/
            basic_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
                pst_set_vpls_l3_mc, &pst_set_vpls_l3_mc->basic_data);    

            /* 如果指针为NULL，无数据 */
            if (NULL == basic_data_start)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  basic_data_start is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));

                //如果为新增，且基本配置未下发，那么直接退出，不处理
                if (ATG_DCI_UNEXIST == if_exist)
                {
                    OS_PRINTF("***ERROR***:新增组播组<VPLS_ID=%d,"
                        "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>，"
                        "但基本配置未下发，直接退出，不处理!\n", 
                        vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                        vpls_l3_mc->vpls_l3_mc_key.src_ip[0],
                        vpls_l3_mc->vpls_l3_mc_key.src_ip[1],
                        vpls_l3_mc->vpls_l3_mc_key.src_ip[2],
                        vpls_l3_mc->vpls_l3_mc_key.src_ip[3],
                        vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],
                        vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],
                        vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],
                        vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]);

                    OS_SPRINTF(uc_message, "***ERROR***:新增组播组<VPLS_ID=%d,"
                        "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>，"
                        "但基本配置未下发，直接退出，不处理!\n", 
                        vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                        vpls_l3_mc->vpls_l3_mc_key.src_ip[0],
                        vpls_l3_mc->vpls_l3_mc_key.src_ip[1],
                        vpls_l3_mc->vpls_l3_mc_key.src_ip[2],
                        vpls_l3_mc->vpls_l3_mc_key.src_ip[3],
                        vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],
                        vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],
                        vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],
                        vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                    ret = ERROR;
                    goto EXIT_LABEL;
                }
            }
            else
            {
                basic_data = (ATG_DCI_VPLS_L3_MC_BASIC_DATA *)basic_data_start;

                if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）基本配置\n");
                    spm_dbg_print_vpls_l3_mc_basic_cfg(basic_data);
                }
                OS_SPRINTF(uc_message,"  1）基本配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                spm_dbg_record_vpls_l3_mc_basic_cfg(basic_data);

                /* 配置处理，todo */
                NBB_TRC_DETAIL((NBB_FORMAT "  MC_ID = %d", basic_data->mc_id));   
                NBB_TRC_DETAIL((NBB_FORMAT "  if_exist = %d", if_exist));

#ifdef SPU
                if (ATG_DCI_UNEXIST == if_exist)
                {
                    //在arad上创建组播组
                    ret = ApiAradSetupEngressMc(UNIT_0, basic_data->mc_id);
                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiAradSetupEngressMc() ret=%d", 
                        ret));
                    if (SUCCESS != ret)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:"
                            "ApiAradSetupEngressMc() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiAradSetupEngressMc() ret=%d\n", 
                            ret);

                        OS_SPRINTF(uc_message, "***ERROR***:"
                            "ApiAradSetupEngressMc() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                        ret = ERROR;
                        goto EXIT_LABEL;
                    }

                    //设置源ip和匹配组播ip
                    igmp_snoop.mVsiId = vpls_l3_mc->vpls_l3_mc_key.vpls_id;
                    igmp_snoop.mcId = basic_data->mc_id;
                    igmp_snoop.mSip = vpls_l3_mc->vpls_l3_mc_key.src_ip[3];
                    igmp_snoop.mDip = vpls_l3_mc->vpls_l3_mc_key.grp_ip[3];
                    for (c3_unit_id = 0; c3_unit_id < SHARED.c3_num; c3_unit_id++)
                    {
                        ret += fhdrv_psn_arp_add_igmpsnooping(c3_unit_id, &igmp_snoop);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  fhdrv_psn_arp_add_igmpsnooping() ret=%d", ret));
                    if (SUCCESS != ret)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: fhdrv_psn_arp_add_igmpsnooping() "
                            "error! ret=%d", ret));

                        OS_PRINTF("***ERROR***: fhdrv_psn_arp_add_igmpsnooping() ret=%d\n", ret);

                        OS_SPRINTF(uc_message, "***ERROR***: fhdrv_psn_arp_add_igmpsnooping() "
                            "ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                        ret = ERROR;
                        goto EXIT_LABEL;
                    }

                    //保存驱动返回值
                    vpls_l3_mc->ulIgmpSnoopPosId = igmp_snoop.posId;
                }
#endif

#ifdef SRC
                if (ATG_DCI_UNEXIST == if_exist)
                {
                    ret = ApiDfeMulticastCreate(UNIT_0, basic_data->mc_id);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastCreate() ret=%d", ret));

                    if (SUCCESS != ret)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:"
                            "ApiDfeMulticastCreate() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiDfeMulticastCreate() ret=%d\n", 
                            ret);

                        OS_SPRINTF(uc_message, "***ERROR***:"
                            "ApiDfeMulticastCreate() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                        ret = ERROR;
                        goto EXIT_LABEL;
                    }
                }                
#endif

#ifdef SNU

                /*获取FE1600芯片数*/
                if (ATG_DCI_UNEXIST == if_exist)
                {
                    ret += ApiDfeMulticastCreate(UNIT_0, basic_data->mc_id);
                    

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastCreate() ret=%d", 
                        ret));

                    if (SUCCESS != ret)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:"
                            "ApiDfeMulticastCreate() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiDfeMulticastCreate() ret=%d\n", 
                            ret);

                        OS_SPRINTF(uc_message, "***ERROR***:"
                            "ApiDfeMulticastCreate() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                        ret = ERROR;
                        goto EXIT_LABEL;
                    }
                }   
#endif
                if (vpls_l3_mc->basic_cfg_cb == NULL)
                {
                    vpls_l3_mc->basic_cfg_cb = (ATG_DCI_VPLS_L3_MC_BASIC_DATA *)
                        NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_L3_MC_BASIC_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_VPLS_L3_MC_BASIC_CFG);
                }  

                //保存数据
                OS_MEMCPY(vpls_l3_mc->basic_cfg_cb, basic_data, 
                    sizeof(ATG_DCI_VPLS_L3_MC_BASIC_DATA));
            }
            break;

        default:
            OS_PRINTF("***ERROR***:Unknown VPLS_L3 Basic Operation!\n");
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_vpls_l3_mc_nni_cb_proc
 功能描述  : VPLS_L3_MC的NNI成员配置处理函数
 输入参数  : VPLS_L3_MC的IPS消息指针pst_set_vpls_l3_mc,
             该VPLS_L3_MC结构体的指针vpls_l3_mc,
             IPS消息中NNI成员配置的操作模式oper_nni,
             该VPLS_L3_MC存在与否的标志if_exist.
 输出参数  : 
 返 回 值  : 函数返回值ret
 调用函数  : 
 被调函数  : 
 
 修改历史  :
  1.
  日    期 : 2014年1月24日
  作    者 : zhangsen
  修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_l3_mc_nni_cb_proc(
    ATG_DCI_SET_VPLS_L3_MC *pst_set_vpls_l3_mc, 
    SPM_VPLS_L3_MC_CB *vpls_l3_mc, 
    NBB_ULONG oper_nni, 
    NBB_BYTE if_exist NBB_CCXT_T NBB_CXT)
{
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_VPLS_L3_MC_NNI_DATA *nni_data[ATG_DCI_VPLS_MC_NNI_NUM];
    ATG_DCI_VPLS_MC_NNI_DATA vpls_mc_nni;

    NBB_BYTE *nni_data_start  = NULL;                /* IPS消息偏移的首地址 */
    NBB_INT ret = SUCCESS;                           /*返回值*/   
    NBB_UINT l2_mc_pos_id = 0;
    NBB_UINT vp_id = 0;
    NBB_INT  slot_num = 0;
    NBB_INT exist_pos = -1;
    NBB_BYTE slot_id = 0;
    NBB_ULONG i = 0;
    NBB_ULONG j = 0;

    /*存放错误信息的字符数组*/
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

#ifdef SNU
    NBB_BYTE unit_id = 0;
#endif

    NBB_TRC_ENTRY("spm_vpls_l3_mc_nni_cb_proc");

    OS_MEMSET(&vpls_mc_nni, 0, sizeof(ATG_DCI_VPLS_MC_NNI_DATA));

    //入参合法性检验
    if (NULL == pst_set_vpls_l3_mc)
    {
        NBB_TRC_DETAIL((NBB_FORMAT"  ***ERROR***: pst_set_vpls_l3_mc为NULL!"));
        OS_SPRINTF(uc_message, "***ERROR***: pst_set_vpls_l3_mc为NULL!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        ret = ERROR;
        goto EXIT_LABEL;
    }
    if (NULL == vpls_l3_mc)
    {
        NBB_TRC_DETAIL((NBB_FORMAT"  ***ERROR***: vpls_l3_mc为NULL!"));
        OS_SPRINTF(uc_message, "***ERROR***: vpls_l3_mc为NULL!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //初始化NNI指针数组 
    for (i = 0; i < ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        nni_data[i] = NULL;
    }

    switch (oper_nni)
    {
        /*NNI成员配置操作为更新，打印PD，不处理*/
        case ATG_DCI_OPER_UPDATE:
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            break;

        /*NNI成员配置操作为增加，进入处理流程*/
        case ATG_DCI_OPER_ADD:
             NBB_TRC_DETAIL((NBB_FORMAT "  NNI ADD")); 
                   
            /* 计算第一个entry的地址。*/
            nni_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vpls_l3_mc, 
                &pst_set_vpls_l3_mc->nni_member_data);    

            /* 如果指针为NULL，无数据 */
            if (NULL == nni_data_start)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  nni_data_start is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2）NNI接口配置[1-512] (nni_num=%d) ADD\n", 
                        pst_set_vpls_l3_mc->nni_member_num);
                }

                OS_SPRINTF(uc_message,"  2）NNI接口配置[1-512] (nni_num=%d) ADD\n", 
                    pst_set_vpls_l3_mc->nni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                for (i = 0; i < pst_set_vpls_l3_mc->nni_member_num; i++)
                {
                    nni_data[i] = (ATG_DCI_VPLS_L3_MC_NNI_DATA *)(nni_data_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_L3_MC_NNI_DATA))) * i); 

                    if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_l3_mc_nni_cfg(nni_data[i], i);
                    }

                    spm_dbg_record_vpls_l3_mc_nni_cfg(nni_data[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC ID = %ld", 
                        nni_data[i]->vc_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP = %s", 
                        spm_set_ulong_to_ipv4(nni_data[i]->peer_ip NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE = %d", 
                        nni_data[i]->vc_type));

                    //判断该NNI是否存在
                    exist_pos = spm_if_vpls_l3_mc_nni_exist(
                        vpls_l3_mc, nni_data[i] NBB_CCXT);
                    if (exist_pos < 0)//不存在，为新增
                    {
                        //转换结构体，适配原vpls_mc的接口
                        vpls_mc_nni.vc_id = nni_data[i]->vc_id;
                        vpls_mc_nni.vc_type = nni_data[i]->vc_type;
                        vpls_mc_nni.peer_ip = nni_data[i]->peer_ip;
                        
                        ret = spm_vpls_mc_add_vp_nni(vpls_l3_mc->basic_cfg_cb->mc_id, 
                            &vpls_mc_nni, &l2_mc_pos_id, &vp_id, &slot_id NBB_CCXT);

                        //成功，保存数据
                        if (SUCCESS == ret)
                        {
                            //在NNI的数组中找一个存储位置
                            for (j = 0; j < ATG_DCI_VPLS_MC_NNI_NUM; j++)
                            {
                                //为NULL，说明未占用，可以存储
                                if (NULL == vpls_l3_mc->nni_cfg_cb[j])
                                {
                                    vpls_l3_mc->nni_cfg_cb[j] = 
                                        (ATG_DCI_VPLS_L3_MC_NNI_DATA *)
                                        NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_L3_MC_NNI_DATA),
                                        NBB_NORETRY_ACT,
                                        MEM_SPM_VPLS_L3_MC_NNI_CFG);

                                    vpls_l3_mc->nni_info_cb[j].vp_pos_id = vp_id;
                                    vpls_l3_mc->nni_info_cb[j].slot_id = slot_id;
                                    vpls_l3_mc->nni_num++;
                                    
                                    OS_MEMCPY(vpls_l3_mc->nni_cfg_cb[j], 
                                        nni_data[i], sizeof(ATG_DCI_VPLS_L3_MC_NNI_DATA));

                                    break;
                                }
                            }
                        }
                        else    //如果增加NNI口失败记录错误状态
                        {
                            pst_set_vpls_l3_mc->nni_member_return_code[i] = 
                                ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
                                "spm_vpls_mc_add_vp_nni() error! ret=%d", ret));

                            OS_PRINTF("***ERROR***:spm_vpls_mc_add_vp_nni() "
                                "ret=%d\n", ret);

                            OS_SPRINTF(uc_message, "***ERROR***:"
                                "spm_vpls_mc_add_vp_nni() ret=%d\n", ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                        }
                    }
                    else//存在，只保存配置
                    {
                        OS_MEMCPY(vpls_l3_mc->nni_cfg_cb[exist_pos], 
                            nni_data[i], sizeof(ATG_DCI_VPLS_L3_MC_NNI_DATA));
                    }
                }
            }
            break;

        /*NNI成员配置操作为删除，进入处理流程*/
        case ATG_DCI_OPER_DEL:
            NBB_TRC_DETAIL((NBB_FORMAT "  NNI DEL"));

            //如果此Key值的VPLS组播配置不存在，则无法删除组播组内的端口
            if (ATG_DCI_UNEXIST == if_exist)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:"
                    "单独删除组播组内端口时，<VPLS_ID=%d,"
                    "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>"
                    "的组播组不存在，无法处理!\n",                             
                    vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[0],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[1],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[2],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[3],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]));

                OS_PRINTF("  ***ERROR***:"
                    "单独删除组播组内端口时，<VPLS_ID=%d,"
                    "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>"
                    "的组播组不存在，无法处理!\n",                             
                    vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[0],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[1],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[2],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[3],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]);
                                                                            
                OS_SPRINTF(uc_message, "  ***ERROR***:"
                    "单独删除组播组内端口时，<VPLS_ID=%d,"
                    "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>"
                    "的组播组不存在，无法处理!\n",                             
                    vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[0],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[1],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[2],
                    vpls_l3_mc->vpls_l3_mc_key.src_ip[3],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],
                    vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
				
                goto EXIT_LABEL;
            }
                   
            /* 计算第一个entry的地址。*/
            nni_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vpls_l3_mc, 
                &pst_set_vpls_l3_mc->nni_member_data);    

            /* 如果指针为NULL，无数据 */
            if (nni_data_start == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  nni_data_start is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2）NNI接口配置[1-512] (nni_num=%d) DEL\n", 
                        pst_set_vpls_l3_mc->nni_member_num);
                }

                OS_SPRINTF(uc_message,"  2）NNI接口配置[1-512] (nni_num=%d) DEL\n", 
                    pst_set_vpls_l3_mc->nni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                for (i = 0; i < pst_set_vpls_l3_mc->nni_member_num; i++)
                {
                    nni_data[i] = (ATG_DCI_VPLS_L3_MC_NNI_DATA *)(nni_data_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_L3_MC_NNI_DATA))) * i); 

                    if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_l3_mc_nni_cfg(nni_data[i], i);
                    }

                    spm_dbg_record_vpls_l3_mc_nni_cfg(nni_data[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC ID = %ld", 
                        nni_data[i]->vc_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP = %s", 
                        spm_set_ulong_to_ipv4(nni_data[i]->peer_ip NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE = %d", 
                        nni_data[i]->vc_type));

                    //判断该NNI是否存在
                    exist_pos = spm_if_vpls_l3_mc_nni_exist(
                        vpls_l3_mc, nni_data[i] NBB_CCXT);
                    if (exist_pos < 0)//不存在，不删除,记录错误
                    {
                        OS_SPRINTF(uc_message, "  ***ERROR***:<VPLS_ID=%d,McId=%d>"
                            "的组播组内，不存在VC_ID=%ld, VC_TYPE=%d, PEER_IP=%s"
                            "的NNI成员，无法删除!\n",                             
                            vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                            vpls_l3_mc->basic_cfg_cb->mc_id,
                            nni_data[i]->vc_id,
                            nni_data[i]->vc_type,
                            spm_set_ulong_to_ipv4(nni_data[i]->peer_ip NBB_CCXT));
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                    }
                    else//存在，删除
                    {
                        ret = spm_vpls_l3_mc_del_vp_nni(
                            vpls_l3_mc->basic_cfg_cb->mc_id, 
                            vpls_l3_mc, 
                            nni_data[i] NBB_CCXT);

                        //成功，删除数据
                        if (SUCCESS == ret)
                        {
                            slot_id = vpls_l3_mc->nni_info_cb[exist_pos].slot_id;
                            
                            //free
                            NBB_MM_FREE(vpls_l3_mc->nni_cfg_cb[exist_pos], 
                                MEM_SPM_VPLS_L3_MC_NNI_CFG);                                
                            
                            vpls_l3_mc->nni_cfg_cb[exist_pos] = NULL;
                            vpls_l3_mc->nni_num--;

                            slot_num = spm_get_vpls_l3_mc_slot_num(vpls_l3_mc, 
                                slot_id NBB_CCXT);
                            
#ifdef SRC
                            if (slot_num == 0)
                            {
                                ret = ApiDfeMulticastDelete(UNIT_0, 
                                    vpls_l3_mc->basic_cfg_cb->mc_id, 
                                    slot_id);

                                NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() "
                                    "ret=%d", ret));
                                
                                if (SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
                                        "ApiDfeMulticastDelete() error! ret=%d", 
                                        ret));

                                    OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() "
                                        "ret=%d", ret);

                                    OS_SPRINTF(uc_message, "***ERROR***: "
                                        "ApiDfeMulticastDelete() ret=%d", ret);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                                    ret = ERROR;
                                    
                                    //goto EXIT_LABEL;
                                }
                            }
#endif

#ifdef SNU
                            if (slot_num == 0)
                            {
                                ret += ApiDfeMulticastDelete(UNIT_0, 
                                    vpls_l3_mc->basic_cfg_cb->mc_id, 
                                    slot_id);
                                

                                NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() "
                                    "ret=%d", ret));
                                
                                if (SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
                                        "ApiDfeMulticastDelete() error! ret=%d", 
                                        ret));

                                    OS_PRINTF("***ERROR***: ApiDfeMulticastDelete()"
                                        " ret=%d", ret);

                                    OS_SPRINTF(uc_message, "***ERROR***: "
                                        "ApiDfeMulticastDelete() ret=%d", 
                                        ret);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                                    ret = ERROR;     
                                    
                                    //goto EXIT_LABEL;
                                }
                            }
#endif
                        }
                        else    //如果增加NNI口失败记录错误状态
                        {
                            pst_set_vpls_l3_mc->nni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
                                "spm_vpls_l3_mc_del_vp_nni() error! ret=%d", ret));

                            OS_PRINTF("***ERROR***: spm_vpls_l3_mc_del_vp_nni() "
                                "ret=%d", ret);

                            OS_SPRINTF(uc_message, "***ERROR***: "
                                "spm_vpls_l3_mc_del_vp_nni() ret=%d", ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                        }
                    }
                }
            }
            break;

        default:
            OS_PRINTF("***ERROR***:Unknown VPLS_L3 Nni Operation!\n");
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_vpls_l3_mc_uni_cb_proc
 功能描述  : VPLS_L3_MC的UNI成员配置处理函数
 输入参数  : VPLS_L3_MC的IPS消息指针pst_set_vpls_l3_mc,
             该VPLS_L3_MC结构体的指针vpls_l3_mc,
             IPS消息中UNI成员配置的操作模式ulOperUni,
             该VPLS_L3_MC存在与否的标志if_exist.
 输出参数  : 
 返 回 值  : 函数返回值ret
 调用函数  : 
 被调函数  : 
 
 修改历史  :
  1.
  日    期 : 2014年1月24日
  作    者 : zhangsen
  修改内容 : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_l3_mc_uni_cb_proc(
    ATG_DCI_SET_VPLS_L3_MC *pst_set_vpls_l3_mc, 
    SPM_VPLS_L3_MC_CB *pst_vpls_l3_mc, 
    NBB_ULONG oper_uni, 
    NBB_BYTE if_exist NBB_CCXT_T NBB_CXT)
{
    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_VPLS_L3_MC_UNI_DATA *uni_data[ATG_DCI_VPLS_MC_UNI_NUM];

    NBB_BYTE *uni_data_start  = NULL;                /* IPS消息偏移的首地址 */
    NBB_INT ret = SUCCESS;                           /*返回值*/   
    NBB_UINT l2_mc_pos_id = 0;
    NBB_UINT vp_id = 0;
    NBB_INT  slot_num = 0;
    NBB_INT exist_pos = -1;
    NBB_BYTE slot_id = 0;
    NBB_ULONG i = 0;
    NBB_ULONG j = 0;

    /*存放错误信息的字符数组*/
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

#ifdef SNU
    NBB_BYTE unit_id = 0;
#endif

    NBB_TRC_ENTRY("spm_vpls_l3_mc_uni_cb_proc");

    //入参合法性检验
    if (NULL == pst_set_vpls_l3_mc)
    {
        NBB_TRC_DETAIL((NBB_FORMAT"  ***ERROR***: pst_set_vpls_l3_mc为NULL!"));
        OS_SPRINTF(uc_message, "***ERROR***: pst_set_vpls_l3_mc为NULL!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        ret = ERROR;
        goto EXIT_LABEL;
    }
    if (NULL == pst_vpls_l3_mc)
    {
        NBB_TRC_DETAIL((NBB_FORMAT"  ***ERROR***: vpls_l3_mc为NULL!"));
        OS_SPRINTF(uc_message, "***ERROR***: vpls_l3_mc为NULL!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //初始化UNI指针数组 
    for (i = 0; i < ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        uni_data[i] = NULL;     
    }

    switch (oper_uni)
    {
        /*UNI成员配置操作为更新，打印PD，不处理*/
        case ATG_DCI_OPER_UPDATE:
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            break;

        /*UNI成员配置操作为增加，进入处理流程*/
        case ATG_DCI_OPER_ADD:
            NBB_TRC_DETAIL((NBB_FORMAT "  UNI ADD"));

            /* 计算第一个entry的地址。*/
            uni_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(
                pst_set_vpls_l3_mc, &pst_set_vpls_l3_mc->uni_member_data);    

            /* 如果指针为NULL，无数据 */
            if (NULL == uni_data_start)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  uni_data_start is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  3）UNI接口配置[1-64] (uni_num=%d) ADD\n", 
                        pst_set_vpls_l3_mc->uni_member_num);
                }

                OS_SPRINTF(uc_message,"  3）UNI接口配置[1-64] (uni_num=%d) ADD\n", 
                    pst_set_vpls_l3_mc->uni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                for (i = 0; i < pst_set_vpls_l3_mc->uni_member_num; i++)
                {
                    uni_data[i] = (ATG_DCI_VPLS_L3_MC_UNI_DATA *)(uni_data_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_L3_MC_UNI_DATA))) * i); 

                    if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_l3_mc_uni_cfg(uni_data[i], i);
                    }

                    spm_dbg_record_vpls_l3_mc_uni_cfg(uni_data[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  端口Index = %ld", 
                        uni_data[i]->port_index));

                    exist_pos = spm_if_vpls_l3_mc_uni_exist(
                        pst_vpls_l3_mc, uni_data[i] NBB_CCXT);
                    if (exist_pos < 0)//不存在，为新增
                    {
                        l2_mc_pos_id = 0;
                        vp_id = 0;
                        slot_id = 0;
                        
                        ret = spm_vpls_mc_add_vp_uni(pst_vpls_l3_mc->basic_cfg_cb->mc_id, 
                            (ATG_DCI_VPLS_MC_UNI_DATA *)uni_data[i], 
                            &l2_mc_pos_id, &vp_id, &slot_id NBB_CCXT);
                    
                        //如果成功，保存数据
                        if (SUCCESS == ret)
                        {
                            //在UNI的数组中找一个存储位置
                            for (j = 0; j < ATG_DCI_VPLS_MC_UNI_NUM; j++)
                            {
                                //为NULL，说明未占用，可以存储
                                if (NULL == pst_vpls_l3_mc->uni_cfg_cb[j])
                                {
                                    pst_vpls_l3_mc->uni_cfg_cb[j] = 
                                        (ATG_DCI_VPLS_L3_MC_UNI_DATA *)
                                        NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_L3_MC_UNI_DATA),
                                        NBB_NORETRY_ACT,
                                        MEM_SPM_VPLS_L3_MC_UNI_CFG);

                                    pst_vpls_l3_mc->uni_info_cb[j].vp_pos_id = vp_id;
                                    pst_vpls_l3_mc->uni_info_cb[j].slot_id = slot_id;
                                    pst_vpls_l3_mc->uni_num++;
                                    
                                    OS_MEMCPY(pst_vpls_l3_mc->uni_cfg_cb[j], 
                                        uni_data[i], 
                                        sizeof(ATG_DCI_VPLS_L3_MC_UNI_DATA));

                                    break;
                                }
                            }
                        }
                        else    //如果增加UNI口失败记录错误状态
                        {
                            pst_set_vpls_l3_mc->uni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                            
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
                                "spm_vpls_mc_add_vp_uni() error! ret=%d", ret));

                            OS_PRINTF("***ERROR***: spm_vpls_mc_add_vp_uni() ret=%d\n", 
                                ret);

                            OS_SPRINTF(uc_message, "***ERROR***: "
                                "spm_vpls_mc_add_vp_uni() ret=%d\n", ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                        }
                    }
                    else//存在，只保存配置
                    {
                        OS_MEMCPY(pst_vpls_l3_mc->uni_cfg_cb[exist_pos], 
                            uni_data[exist_pos], 
                            sizeof(ATG_DCI_VPLS_L3_MC_UNI_DATA));
                    }


                }
            } 
            break;

        /*UNI成员配置操作为删除，进入处理流程*/
        case ATG_DCI_OPER_DEL:
            NBB_TRC_DETAIL((NBB_FORMAT "  UNI DEL"));

            //如果此Key值的VPLS组播配置不存在，则无法删除组播组内的端口
            if (if_exist == ATG_DCI_UNEXIST)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:"
                    "单独删除组播组内端口时，<VPLS_ID=%d "
                    "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>"
                    "的组播组不存在，无法处理!\n",                             
                    pst_vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[0],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[1],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[2],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[3],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]));

                OS_PRINTF(NBB_FORMAT "  ***ERROR***:"
                    "单独删除组播组内端口时，<VPLS_ID=%d "
                    "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>"
                    "的组播组不存在，无法处理!\n",                            
                    pst_vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[0],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[1],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[2],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[3],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]);

                OS_SPRINTF(uc_message, "  ***ERROR***:"
                    "单独删除组播组内端口时，<VPLS_ID=%d "
                    "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>"
                    "的组播组不存在，无法处理!\n",                             
                    pst_vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[0],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[1],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[2],
                    pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[3],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],
                    pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                goto EXIT_LABEL;
            }
                   
            /* 计算第一个entry的地址。*/
            uni_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vpls_l3_mc, 
                &pst_set_vpls_l3_mc->uni_member_data);    

            /* 如果指针为NULL，无数据 */
            if (uni_data_start == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  uni_data_start is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  3）UNI接口配置[1-64] (uni_num=%d) DEL\n", 
                        pst_set_vpls_l3_mc->uni_member_num);
                }

                OS_SPRINTF(uc_message,"  3）UNI接口配置[1-64] (uni_num=%d) DEL\n", 
                    pst_set_vpls_l3_mc->uni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                
                for (i = 0; i < pst_set_vpls_l3_mc->uni_member_num; i++)
                {
                    uni_data[i] = (ATG_DCI_VPLS_L3_MC_UNI_DATA *)(uni_data_start + 
                        (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_L3_MC_UNI_DATA))) * i); 

                    if (vpls_l3_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_l3_mc_uni_cfg(uni_data[i], i);
                    }

                    spm_dbg_record_vpls_l3_mc_uni_cfg(uni_data[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  UNI PORT_INDEX = %ld", 
                        uni_data[i]->port_index));

                    exist_pos = spm_if_vpls_l3_mc_uni_exist(
                        pst_vpls_l3_mc, uni_data[i] NBB_CCXT);
                    if (exist_pos < 0)//不存在，不删除，记录错误
                    {
                        OS_SPRINTF(uc_message, "  ***ERROR***:<VPLS_ID=%d,McId=%d>"
                            "的组播组内，不存在PORTINDEX=%ld的UNI成员，无法删除!\n",                             
                            pst_vpls_l3_mc->vpls_l3_mc_key.vpls_id,
                            pst_vpls_l3_mc->basic_cfg_cb->mc_id,
                            uni_data[i]->port_index);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                    }
                    else//存在，删除
                    {
                        ret = spm_vpls_l3_mc_del_vp_uni(
                            pst_vpls_l3_mc->basic_cfg_cb->mc_id, 
                            pst_vpls_l3_mc, 
                            (ATG_DCI_VPLS_MC_UNI_DATA *)uni_data[i] NBB_CCXT);

                        //成功，删除数据
                        if (SUCCESS == ret)
                        {
                            slot_id = pst_vpls_l3_mc->uni_info_cb[exist_pos].slot_id;
                            
                            //free
                            NBB_MM_FREE(pst_vpls_l3_mc->uni_cfg_cb[exist_pos], 
                                MEM_SPM_VPLS_L3_MC_UNI_CFG);                                
                            
                            pst_vpls_l3_mc->uni_cfg_cb[exist_pos] = NULL;
                            pst_vpls_l3_mc->uni_num--;

                            slot_num = spm_get_vpls_l3_mc_slot_num(
                                pst_vpls_l3_mc, slot_id NBB_CCXT);
                            
#ifdef SRC
                            if (slot_num == 0)
                            {
                                ret = ApiDfeMulticastDelete(UNIT_0, 
                                    pst_vpls_l3_mc->basic_cfg_cb->mc_id, 
                                    slot_id);

                                NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() "
                                    "ret=%d", ret));
                                
                                if (ret != SUCCESS)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
                                        "ApiDfeMulticastDelete() error! ret=%d", 
                                        ret));

                                    OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() "
                                        "ret=%d", ret);

                                    OS_SPRINTF(uc_message, "***ERROR***: "
                                        "ApiDfeMulticastDelete() ret=%d", ret);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                                    ret = ERROR;   
                                    
                                    //goto EXIT_LABEL;
                                }
                            }
#endif

#ifdef SNU
                            if (slot_num == 0)
                            {
                                ret = ApiDfeMulticastDelete(UNIT_0, 
                                    pst_vpls_l3_mc->basic_cfg_cb->mc_id, 
                                    slot_id);
                                

                                NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() "
                                    "ret=%d", ret));
                                
                                if (ret != SUCCESS)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
                                        "ApiDfeMulticastDelete() error! ret=%d", 
                                        ret));

                                    OS_PRINTF("***ERROR***: ApiDfeMulticastDelete()"
                                        " ret=%d", ret);

                                    OS_SPRINTF(uc_message, "***ERROR***: "
                                        "ApiDfeMulticastDelete() ret=%d", 
                                        ret);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                                    ret = ERROR;   
                                    
                                    //goto EXIT_LABEL;
                                }
                            }
#endif
                        }
                        else    //删除NNI口失败记录错误状态
                        {
                            pst_set_vpls_l3_mc->uni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: "
                                "spm_vpls_l3_mc_del_vp_uni() error! ret=%d", ret));

                            OS_PRINTF("***ERROR***: spm_vpls_l3_mc_del_vp_uni() "
                                "ret=%d", ret);

                            OS_SPRINTF(uc_message, "***ERROR***: "
                                "spm_vpls_l3_mc_del_vp_uni() ret=%d", ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
                        }
                    }
                }
            } 
            break;

        default:
            OS_PRINTF("***ERROR***:Unknown VPLS_L3 Uni Operation!\n");
    }
             

    EXIT_LABEL:

    NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_vpls_l3_mc
 功能描述  : VPLS L3静态组播配置处理
 输入参数  : ATG_DCI_SET_VPLS_MC *pstSetVplsMc  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
 1.
  日    期 : 2014年4月4日
  作    者 : zhangsen
  修改内容 : 新生成函数
 2.
  日    期 : 2014年6月5日
  作    者 : zhangsen
  修改内容 : 在处理nni和uni子TLV之前，增加基本配置是否存在的判断，防止未下发
             基本配置造成单盘挂死。
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_vpls_l3_mc(
    ATG_DCI_SET_VPLS_L3_MC *pst_set_vpls_l3_mc NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_INT ret = SUCCESS;
    NBB_INT i = 0;

    /* 子配置的操作模式 */
    NBB_ULONG oper_basic = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_nni   = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_uni   = ATG_DCI_OPER_NULL;

    /* VPLS组播的键值 */
    ATG_DCI_VPLS_L3_MC_KEY vpls_l3_mc_key;
    SPM_VPLS_L3_MC_CB *pst_vpls_l3_mc = NULL;

    NBB_BYTE if_exist = ATG_DCI_EXIST;

    NBB_BOOL avll_ret_code;

    NBB_TRC_ENTRY("spm_rcv_dci_set_vpls_l3_mc");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pst_set_vpls_l3_mc);

	if (NULL == pst_set_vpls_l3_mc)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_vpls_l3_mc"
            "(pst_set_vpls_l3_mc==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_vpls_l3_mc(pst_set_vpls_l3_mc==NULL)\n");
                                                                        
        OS_SPRINTF(uc_message, "***ERROR***:spm_rcv_dci_set_vpls_l3_mc"
            "(pst_set_vpls_l3_mc==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

        NBB_EXCEPTION((PCT_SPM | VPLS_L3_MC_PD, 0, "d s s d s s", 0, "", "",
            FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", uc_message));

        goto EXIT_LABEL;
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pst_set_vpls_l3_mc->return_code = ATG_DCI_RC_OK;

    /*获取子配置的操作类型*/
    oper_basic = pst_set_vpls_l3_mc->oper_basic;
    oper_nni = pst_set_vpls_l3_mc->oper_nni_member;
    oper_uni = pst_set_vpls_l3_mc->oper_uni_member;

    OS_MEMCPY(&vpls_l3_mc_key, &pst_set_vpls_l3_mc->key, sizeof(ATG_DCI_VPLS_L3_MC_KEY));  
    pst_vpls_l3_mc = AVLL_FIND(SHARED.vpls_l3_mc_tree, &vpls_l3_mc_key);
    
    if (NULL == pst_vpls_l3_mc)    //条目不存在
    {
        if_exist = ATG_DCI_UNEXIST;
    }

    /* 删除整个条目 */
    if (TRUE == pst_set_vpls_l3_mc->delete_struct)
    {
        //条目不存在，无法删除
        if (NULL == pst_vpls_l3_mc)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在，退出"));

            OS_PRINTF("***ERROR***:要删除的VplsId=%d SrcIp=0x%lx-0x%lx-0x%lx-0x%lx "
                "GrpIp=0x%lx-0x%lx-0x%lx-0x%lx的VPLS_L3组播配置不存在!\n", 
                vpls_l3_mc_key.vpls_id,
                vpls_l3_mc_key.src_ip[0],
                vpls_l3_mc_key.src_ip[1],
                vpls_l3_mc_key.src_ip[2],
                vpls_l3_mc_key.src_ip[3],
                vpls_l3_mc_key.grp_ip[0],
                vpls_l3_mc_key.grp_ip[1],
                vpls_l3_mc_key.grp_ip[2],
                vpls_l3_mc_key.grp_ip[3]);

            OS_SPRINTF(uc_message, 
                "***ERROR***:要删除的VplsId=%d SrcIp=0x%lx-0x%lx-0x%lx-0x%lx "
                "GrpIp=0x%lx-0x%lx-0x%lx-0x%lx的VPLS_L3组播配置不存在!\n", 
                vpls_l3_mc_key.vpls_id,
                vpls_l3_mc_key.src_ip[0],
                vpls_l3_mc_key.src_ip[1],
                vpls_l3_mc_key.src_ip[2],
                vpls_l3_mc_key.src_ip[3],
                vpls_l3_mc_key.grp_ip[0],
                vpls_l3_mc_key.grp_ip[1],
                vpls_l3_mc_key.grp_ip[2],
                vpls_l3_mc_key.grp_ip[3]);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            ret = ERROR;
            goto EXIT_LABEL;
        }

        ret = spm_delete_vpls_l3_mc_cb(pst_vpls_l3_mc, 
            vpls_l3_mc_key, if_exist NBB_CCXT);
        if (SUCCESS != ret)
        {
            pst_set_vpls_l3_mc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
    }
    
    /* 增加或更新条目 */
    else
    {        
        
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ATG_DCI_UNEXIST == if_exist)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  增加VPLS_ID=%d  SrcIp=0x%lx-0x%lx-0x%lx-0x%lx "
                "GrpIp=0x%lx-0x%lx-0x%lx-0x%lx 的 VPLS_L3组播配置", 
                vpls_l3_mc_key.vpls_id,
                vpls_l3_mc_key.src_ip[0],
                vpls_l3_mc_key.src_ip[1],
                vpls_l3_mc_key.src_ip[2],
                vpls_l3_mc_key.src_ip[3],
                vpls_l3_mc_key.grp_ip[0],
                vpls_l3_mc_key.grp_ip[1],
                vpls_l3_mc_key.grp_ip[2],
                vpls_l3_mc_key.grp_ip[3]));

            //打印，todo
            spm_dbg_print_vpls_l3_mc_head(&vpls_l3_mc_key, SPM_OPER_ADD);
            spm_dbg_record_vpls_l3_mc_head(&vpls_l3_mc_key, SPM_OPER_ADD);
            
            /* 申请一个新条目的内存空间 */
            pst_vpls_l3_mc = spm_alloc_vpls_l3_mc_cb(NBB_CXT);

			if (pst_vpls_l3_mc != NULL)
			{
	            //插入到节点中
	            OS_MEMCPY(&(pst_vpls_l3_mc->vpls_l3_mc_key), &vpls_l3_mc_key, 
	                sizeof(ATG_DCI_VPLS_L3_MC_KEY));
	            avll_ret_code = AVLL_INSERT(SHARED.vpls_l3_mc_tree, pst_vpls_l3_mc->spm_vpls_l3_mc_node);
		     NBB_ASSERT(avll_ret_code);
			}
            else
            {
                OS_SPRINTF(uc_message, "***ERROR***:要增加VPLS_ID=%d "
                    " SrcIp=0x%lx-0x%lx-0x%lx-0x%lx  GrpIp=0x%lx-0x%lx-0x%lx-0x%lx 的 "
                    "VPLS_L3组播配置未能成功申请内存!\n", 
                    vpls_l3_mc_key.vpls_id,
                    vpls_l3_mc_key.src_ip[0],
                    vpls_l3_mc_key.src_ip[1],
                    vpls_l3_mc_key.src_ip[2],
                    vpls_l3_mc_key.src_ip[3],
                    vpls_l3_mc_key.grp_ip[0],
                    vpls_l3_mc_key.grp_ip[1],
                    vpls_l3_mc_key.grp_ip[2],
                    vpls_l3_mc_key.grp_ip[3]);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

                pst_set_vpls_l3_mc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
        }
        else//条目已存在，更新
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  更新VPLS_ID=%d  SrcIp=0x%lx-0x%lx-0x%lx-0x%lx "
                "GrpIp=0x%lx-0x%lx-0x%lx-0x%lx 的 VPLS_L3组播配置",
                    vpls_l3_mc_key.vpls_id,
                    vpls_l3_mc_key.src_ip[0],
                    vpls_l3_mc_key.src_ip[1],
                    vpls_l3_mc_key.src_ip[2],
                    vpls_l3_mc_key.src_ip[3],
                    vpls_l3_mc_key.grp_ip[0],
                    vpls_l3_mc_key.grp_ip[1],
                    vpls_l3_mc_key.grp_ip[2],
                    vpls_l3_mc_key.grp_ip[3]));    

            //打印，todo
            spm_dbg_print_vpls_l3_mc_head(&vpls_l3_mc_key, SPM_OPER_UPD);
            spm_dbg_record_vpls_l3_mc_head(&vpls_l3_mc_key, SPM_OPER_UPD);
        }

        /***************************************************************************/
        /*                              获取配置                                   */
        /***************************************************************************/
        /******************************** 基本配置 *********************************/
        if (oper_basic)
        {
            ret = spm_vpls_l3_mc_basic_cb_proc(pst_set_vpls_l3_mc, 
                pst_vpls_l3_mc, oper_basic, if_exist NBB_CCXT);

            //基本配置处理失败，不处理后面的组播成员，直接退出
            if (SUCCESS != ret)
            {
                pst_set_vpls_l3_mc->return_code = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                pst_set_vpls_l3_mc->basic_return_code = 
                    ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;

                for (i = 0; i < pst_set_vpls_l3_mc->nni_member_num; i++)
                {
                    pst_set_vpls_l3_mc->nni_member_return_code[i] = 
                        ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                }

                for (i = 0; i < pst_set_vpls_l3_mc->uni_member_num; i++)
                {
                    pst_set_vpls_l3_mc->uni_member_return_code[i] = 
                        ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                }

                goto EXIT_LABEL;
            }
        }

        /*基本配置没有下发，直接退出，不做任何处理*/
        if (NULL == pst_vpls_l3_mc->basic_cfg_cb)/*lint !e613 */
        {
            OS_SPRINTF(uc_message, "***ERROR***:新增组播组<VPLS_ID=%d,"
                "SrcIp=0x%lx-0x%lx-0x%lx-0x%lx GrpIp=0x%lx-0x%lx-0x%lx-0x%lx>，"
                "但基本配置未下发，直接退出，不处理!\n", 
                pst_vpls_l3_mc->vpls_l3_mc_key.vpls_id,/*lint !e613 */
                pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[0],/*lint !e613 */
                pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[1],/*lint !e613 */
                pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[2],/*lint !e613 */
                pst_vpls_l3_mc->vpls_l3_mc_key.src_ip[3],/*lint !e613 */
                pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[0],/*lint !e613 */
                pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[1],/*lint !e613 */
                pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[2],/*lint !e613 */
                pst_vpls_l3_mc->vpls_l3_mc_key.grp_ip[3]);/*lint !e613 */
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

            pst_set_vpls_l3_mc->return_code = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
            for (i = 0; i < pst_set_vpls_l3_mc->nni_member_num; i++)
            {
                pst_set_vpls_l3_mc->nni_member_return_code[i] = 
                    ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
            }
            for (i = 0; i < pst_set_vpls_l3_mc->uni_member_num; i++)
            {
                pst_set_vpls_l3_mc->uni_member_return_code[i] = 
                    ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
            }

            goto EXIT_LABEL;
        }

        /******************************** NNI配置 *********************************/
        if (oper_nni)
        {
            spm_vpls_l3_mc_nni_cb_proc(pst_set_vpls_l3_mc, 
                pst_vpls_l3_mc, oper_nni, if_exist NBB_CCXT);
        }

        /******************************** UNI配置 *********************************/        
        if (oper_uni)
        {
            spm_vpls_l3_mc_uni_cb_proc(pst_set_vpls_l3_mc, 
                pst_vpls_l3_mc, oper_uni, if_exist NBB_CCXT);
        }
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}


#endif
