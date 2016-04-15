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

#ifdef ARP_CFG

extern unsigned char arp_cfg_print_setting;

NBB_INT spm_arp_basic_data_proc(ATG_DCI_SET_ARP *pst_set_arp,NBB_ULONG ul_oper_basic,
    SPM_ARP_CB *pst_arp)
{

	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];  
    NBB_INT ret = SUCCESS;
    ATG_DCI_ARP_BASIC_DATA *pst_basic_data = NULL;
    NBB_BYTE *puc_basic_data_start = NULL; 
	ARP_T stArp;

    NBB_TRC_ENTRY("spm_vc_basic_data_proc");

    if (NULL == pst_set_arp)
    {
       	return ADD_ARP_BASIC_ARP_IPS_IS_NULL;
        
    }
    
    if (NULL == pst_arp)
    {
        return ADD_ARP_BASIC_ARP_CFG_IS_NULL;
    }
	OS_MEMSET(&stArp,0,sizeof(ARP_T));

	if(ATG_DCI_OPER_ADD == ul_oper_basic)
	{
		/* 计算第一个entry的地址。*/
	    puc_basic_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_arp, &pst_set_arp->basic_data);

	    /* 如果指针为NULL，无数据 */
	    if (puc_basic_data_start == NULL)
	    {
	        return ADD_ARP_BASIC_DATA_START_IS_NULL;
	    }
   		else
	    {
	        pst_basic_data = (ATG_DCI_ARP_BASIC_DATA *)puc_basic_data_start;

	        if (arp_cfg_print_setting == SPM_PRINT_CFG)
	        {
	            printf("  1）基本配置\n");
	            spm_dbg_print_arp_basic_cfg(pst_basic_data);
	        }

	        OS_SPRINTF(uc_message, "  1）基本配置\n");
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
	        
	        spm_dbg_record_arp_basic_cfg(pst_basic_data);

			//先保存配置
			
	        if (pst_arp->basic_cfg_cb == NULL)
	        {
	            pst_arp->basic_cfg_cb = (ATG_DCI_ARP_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_ARP_BASIC_DATA),
	                NBB_NORETRY_ACT,
	                MEM_SPM_ARP_BASIC_CB);

				if(NULL == pst_arp->basic_cfg_cb)
				{
					return ADD_ARP_BASIC_MEM_BASIC_NULL;
				}
	        }

	        OS_MEMCPY(pst_arp->basic_cfg_cb, pst_basic_data, sizeof(ATG_DCI_ARP_BASIC_DATA));
			ret = spm_arp_drv_add_arp(pst_arp);
	    }
	}

	return ret;
  

}

/*****************************************************************************
   函 数 名  : spm_rcv_dci_set_arp
   功能描述  : 处理ARP表的IPS消息
   输入参数  : ATG_DCI_SET_ARP *pst_set_arp NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年11月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_arp(ATG_DCI_SET_ARP *pst_set_arp)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_BYTE uc_messageLen = 0;

    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_ARP_CB *pst_arp = NULL;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;

    SPM_PORT_INFO_CB stPortInfo;

    ATG_DCI_ARP_KEY stArpKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_arp");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pst_set_arp != NULL);

    if (pst_set_arp == NULL)
    {
        ret = set_ARP_IPS_NULL;
		goto EXIT_LABEL;
    }

	pst_set_arp->return_code = ATG_DCI_RC_OK;
	pst_set_arp->basic_return_code = ATG_DCI_RC_OK;

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    stArpKey.index = pst_set_arp->key.index;
    stArpKey.ip_addr = pst_set_arp->key.ip_addr;

    pst_arp = AVLL_FIND(SHARED.arp_tree, &stArpKey);

    if (pst_arp == NULL)      
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &stArpKey.index);

    if ((NULL == pstLogicalPort) || (NULL == pstLogicalPort->basic_cfg_cb))
    {
        ret = set_ARP_LOGICAL_PORT_NULL;
        goto EXIT_LABEL;
    }
   
    if (ATG_DCI_L2 == pstLogicalPort->basic_cfg_cb->port_route_type)
    {
        ret = set_ARP_ROUTE_L2;
        goto EXIT_LABEL;
    }
    

    /* 获取子配置操作 */
    ulOperBasic = pst_set_arp->oper_basic;

    //根据端口逻辑配置中的端口子类型，获取端口的port_id
    spm_get_portid_from_logical_port_index(pst_set_arp->key.index, &stPortInfo NBB_CCXT);

    /* 删除整个条目 */
    if (pst_set_arp->delete_struct == TRUE)
    {
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            goto EXIT_LABEL;
        }

        /* 存在，删除 */
        else
        {
            spm_dbg_print_arp_head(&stArpKey, SPM_OPER_DEL);
            spm_dbg_record_arp_head(&stArpKey, SPM_OPER_DEL);

#ifdef SPU
            ret = spm_arp_drv_del_arp(pst_arp);
			if(SUCCESS != ret)
			{
				pst_arp->total_ret_code = ret;
				goto EXIT_LABEL;
			}
#endif

			if (pst_arp != NULL)
			{
	            AVLL_DELETE(SHARED.arp_tree, pst_arp->spm_arp_node);

	            //释放ARP表节点的内存空间
	            spm_free_arp_cb(pst_arp NBB_CCXT);
			}
        }
    }

    /* 增加或更新条目 */
    else
    {

        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            spm_dbg_print_arp_head(&stArpKey, SPM_OPER_ADD);
            spm_dbg_record_arp_head(&stArpKey, SPM_OPER_ADD);

            /* 申请一个新条目的内存空间 */
            pst_arp = spm_alloc_arp_cb(NBB_CXT);

			if (pst_arp != NULL)
			{
	            //插入到tree中
	            OS_MEMCPY(&pst_arp->arp_key, &stArpKey, sizeof(ATG_DCI_ARP_KEY));
	            AVLL_INSERT(SHARED.arp_tree, pst_arp->spm_arp_node);
			}
        }
        else
        {
            spm_dbg_print_arp_head(&stArpKey, SPM_OPER_UPD);
            spm_dbg_record_arp_head(&stArpKey, SPM_OPER_UPD);
        }

        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/
       /******************************** 基本配置 *********************************/        
        if (ATG_DCI_OPER_NULL != ulOperBasic)
        {
            ret = spm_arp_basic_data_proc(pst_set_arp,ulOperBasic,pst_arp);
			if (SUCCESS != ret)    
	        {
	        	SPM_L2_ERROR_PRINT_LOG(uc_message,"arp basic proc r1=%d\n",ret);
				pst_arp->basic_ret_code = ret;
	        }   
        }

    }

EXIT_LABEL: NBB_TRC_EXIT();


    return;

}

/*****************************************************************************
   函 数 名  : spm_init_arp_tree
   功能描述  : 初始化存储ARP配置的树
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年11月24日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_arp_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_arp_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_ARP_CB, arp_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.arp_tree, spm_arp_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_ARP_CB, spm_arp_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   函 数 名  : spm_alloc_arp_cb
   功能描述  : 申请ARP节点的内存空间
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_ARP_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年11月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_ARP_CB *spm_alloc_arp_cb(NBB_CXT_T NBB_CXT)
{
    SPM_ARP_CB *pst_arp = NULL;

    NBB_TRC_ENTRY("spm_alloc_arp_cb");

    /* 分配一个新的VC表配置条目。*/
    pst_arp = (SPM_ARP_CB *)NBB_MM_ALLOC(sizeof(SPM_ARP_CB), NBB_NORETRY_ACT, MEM_SPM_ARP_CB);
    if (pst_arp == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VC表配置条目 */
    OS_MEMSET(pst_arp, 0, sizeof(SPM_ARP_CB));
    pst_arp->basic_cfg_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pst_arp->spm_arp_handle = NBB_CREATE_HANDLE(pst_arp, HDL_SPM_ARP_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_ARP_CB allocated at %p with handle %#lx",
            pst_arp, pst_arp->spm_arp_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_arp->spm_arp_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_arp);
}

/*****************************************************************************
   函 数 名  : spm_free_arp_cb
   功能描述  : 释放ARP表节点泊内存空间
   输入参数  : SPM_ARP_CB *pst_arp NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年11月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_arp_cb(SPM_ARP_CB *pst_arp NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_arp_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_arp, sizeof(SPM_ARP_CB), MEM_SPM_ARP_CB);

    if (pst_arp == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_ARP_CB at %p", pst_arp));
    NBB_TRC_FLOW((NBB_FORMAT "Free INDEX %d  IP_ADDR %s", pst_arp->arp_key.index,
            spm_set_ulong_to_ipv4(pst_arp->arp_key.ip_addr NBB_CCXT)));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pst_arp->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_arp->basic_cfg_cb, MEM_SPM_ARP_BASIC_CB);
        pst_arp->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_arp->spm_arp_handle, HDL_SPM_ARP_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_arp, MEM_SPM_ARP_CB);
    pst_arp = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_arp_cb
   功能描述  : 释放arp配置所有的内存空间
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

NBB_VOID spm_free_all_arp_cb(NBB_CXT_T NBB_CXT)
{
	SPM_ARP_CB *pst_arp = NULL;
	
    for (pst_arp = (SPM_ARP_CB*) AVLL_FIRST(SHARED.arp_tree);
         pst_arp != NULL;
         pst_arp = (SPM_ARP_CB*) AVLL_FIRST(SHARED.arp_tree))
    {
    	
		AVLL_DELETE(SHARED.arp_tree, pst_arp->spm_arp_node);     	
    	spm_free_arp_cb(pst_arp NBB_CCXT);
    }
}


#endif

