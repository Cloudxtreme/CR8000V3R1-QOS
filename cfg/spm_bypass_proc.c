/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_bypass_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年7月16日
  最近修改   :
  功能描述   : Bypass双归保护配置处理
  函数列表   :
  修改历史   :
  1.日    期   : 2013年7月16日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef BYPASS_CFG

extern unsigned char bypass_cfg_print_setting;

/*****************************************************************************
   函 数 名  : spm_rcv_dci_set_arp
   功能描述  : 处理ARP表的IPS消息
   输入参数  : ATG_DCI_SET_ARP *pstSetArp NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2012年11月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_bypass(ATG_DCI_SET_BYPASS *pstSetBypass)
{
    NBB_CHAR print_message[SPM_MSG_INFO_LEN] = {0};
    NBB_BYTE ucMessageLen = 0;

    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_BYPASS_CB *pstBypass = NULL;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    ATG_DCI_BYPASS_BASIC_DATA *pstBasicData = NULL;
    
    NBB_BOOL avll_ret_code = FALSE;

    NBB_BYTE *pucBasicDataStart = NULL;

    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;

    NBB_USHORT usBypassDhIdKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_bypass");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetBypass != NULL);

    if (pstSetBypass == NULL)
    {
        SPM_L2_ERROR_PRINT_LOG(print_message,"r1=%d\n",PROC_BYPASS_IPS_DATA_NULL);
        goto EXIT_LABEL;
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetBypass->return_code = ATG_DCI_RC_OK;

    usBypassDhIdKey = pstSetBypass->key;

    pstBypass = AVLL_FIND(SHARED.bypass_tree, &usBypassDhIdKey);

    if (pstBypass == NULL)       /* 如果条目不存在*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }

    /* 获取子配置操作 */
    ulOperBasic = pstSetBypass->oper_basic;

    /* 删除整个条目 */
    if (pstSetBypass->delete_struct == TRUE)
    {
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            SPM_L2_ERROR_PRINT_LOG(print_message,"r1=%d key=%d\n",DEL_BYPASS_CFG_NULL,usBypassDhIdKey);
            goto EXIT_LABEL;
        }

        /* 存在，删除 */
        else
        {
		spm_dbg_print_bypass_head(usBypassDhIdKey, SPM_OPER_DEL);
		spm_dbg_record_bypass_head(usBypassDhIdKey, SPM_OPER_DEL);

		ret = spm_del_bypass_dh(usBypassDhIdKey);

		if(SUCCESS != ret)
		{
		    SPM_L2_ERROR_PRINT_LOG(print_message,"r1=%d key=%d\n",ret,usBypassDhIdKey);
		    goto EXIT_LABEL;        
		}

		if (pstBypass != NULL)
		{
			AVLL_DELETE(SHARED.bypass_tree, pstBypass->spm_bypass_node);

			//释放ARP表节点的内存空间
			spm_free_bypass_cb(pstBypass NBB_CCXT);
		}
        }
    }

    /* 增加或更新条目 */
    else
    {

        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            spm_dbg_print_bypass_head(usBypassDhIdKey, SPM_OPER_ADD);
            spm_dbg_record_bypass_head(usBypassDhIdKey, SPM_OPER_ADD);

            /* 申请一个新条目的内存空间 */
            pstBypass = spm_alloc_bypass_cb(NBB_CXT);

		if (pstBypass != NULL)
		{
			//插入到tree中
			pstBypass->bypass_key = usBypassDhIdKey;

			avll_ret_code = AVLL_INSERT(SHARED.bypass_tree, pstBypass->spm_bypass_node);
			NBB_ASSERT(avll_ret_code);
		}
        }
        else
        {
            spm_dbg_print_bypass_head(usBypassDhIdKey, SPM_OPER_UPD);
            spm_dbg_record_bypass_head(usBypassDhIdKey, SPM_OPER_UPD);
        }

        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/
        /* 基本配置 */
		/******************************** 全局属性 *********************************/
		if(ATG_DCI_OPER_NULL != ulOperBasic)
		{
			ret = spm_bypass_basic_proc(pstSetBypass, pstBypass, ulOperBasic);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(print_message,"process bypass basic faild ret=%d,key=%d\n",
									ret,pstSetBypass->key);
	            pstBypass->basic_ret_code = ret;     
	        }
		}
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return;

}

/*****************************************************************************
 函 数 名  : spm_init_bypass_tree
 功能描述  : 初始化存储Bypass双归保护配置置的树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bypass_basic_proc(ATG_DCI_SET_BYPASS *pstSetBypass, 
                               SPM_BYPASS_CB *pstBypass, NBB_ULONG ulOperBasic)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucBasicDataStart = NULL;    
    ATG_DCI_BYPASS_BASIC_DATA *basic_data = NULL;
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
	NBB_USHORT bypass_key = 0;

    if((NULL == pstSetBypass) || (NULL == pstBypass))
    {
        ret = PROC_BYPASS_BASIC_INPUT_POINTER_NULL;
        goto EXIT_LABEL;
    }

	bypass_key = pstSetBypass->key;
	
    switch(ulOperBasic)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
			break;

		case ATG_DCI_OPER_ADD:

            //基本配置的首地址
            pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetBypass, &pstSetBypass->basic_data);

            /* 如果指针为NULL，无数据 */
            if (pucBasicDataStart == NULL)
            {
                ret = PROC_BYPASS_BASIC_DATA_NULL;
                goto EXIT_LABEL;
            }

            basic_data = (ATG_DCI_BYPASS_BASIC_DATA *)pucBasicDataStart;

			if (bypass_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  1）基本配置\n");
                spm_dbg_print_bypass_basic_cfg(basic_data);
            }

            OS_SPRINTF(ucMessage, "  1）基本配置\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
            spm_dbg_record_bypass_basic_cfg(basic_data);

            if(NULL == pstBypass->basic_cfg_cb)
            {
                pstBypass->basic_cfg_cb = (ATG_DCI_BYPASS_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_BYPASS_BASIC_DATA),
	                            NBB_NORETRY_ACT,MEM_SPM_BYPASS_BASIC_CB);  
            }

            if(NULL != pstBypass->basic_cfg_cb)
            {
                NBB_MEMCPY(pstBypass->basic_cfg_cb, basic_data, sizeof(ATG_DCI_BYPASS_BASIC_DATA));
            }
            else
            {
                ret = BYPASS_BASIC_MALLOC_MEMORY_ERROR;
                goto EXIT_LABEL;    
            }

			//调用保护接口添加bypass保护
			ret = spm_set_aps_bypass_dh(bypass_key,pstBypass->basic_cfg_cb);			
            break;

         default:
            break;
            
    }
	    
EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
 函 数 名  : spm_init_bypass_tree
 功能描述  : 初始化存储Bypass双归保护配置置的树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_bypass_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_bypass_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_BYPASS_CB, bypass_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.bypass_tree, compare_ushort,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_BYPASS_CB, spm_bypass_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
 函 数 名  : spm_alloc_bypass_cb
 功能描述  : 申请Bypass双归保护配置的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : SPM_BYPASS_CB
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_BYPASS_CB *spm_alloc_bypass_cb(NBB_CXT_T NBB_CXT)
{
    SPM_BYPASS_CB *pstBypass = NULL;

    NBB_TRC_ENTRY("spm_alloc_bypass_cb");

    /* 分配一个新的VC表配置条目。*/
    pstBypass = (SPM_BYPASS_CB *)NBB_MM_ALLOC(sizeof(SPM_BYPASS_CB), NBB_NORETRY_ACT, MEM_SPM_BYPASS_CB);
    if (pstBypass == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VC表配置条目 */
    OS_MEMSET(pstBypass, 0, sizeof(SPM_BYPASS_CB));
    pstBypass->basic_cfg_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pstBypass->spm_bypass_handle = NBB_CREATE_HANDLE(pstBypass, HDL_SPM_BYPASS_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_BYPASS_CB allocated at %p with handle %#lx",
            pstBypass, pstBypass->spm_bypass_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstBypass->spm_bypass_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstBypass);
}

/*****************************************************************************
 函 数 名  : spm_free_bypass_cb
 功能描述  : 释放Bypass双归保护配置节点泊内存空间
 输入参数  : SPM_BYPASS_CB *pstBypass  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年7月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_bypass_cb(SPM_BYPASS_CB *pstBypass NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_bypass_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstBypass, sizeof(SPM_BYPASS_CB), MEM_SPM_BYPASS_CB);

    if (pstBypass == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_BYPASS_CB at %p", pstBypass));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstBypass->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstBypass->basic_cfg_cb, MEM_SPM_BYPASS_BASIC_CB);
        pstBypass->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstBypass->spm_bypass_handle, HDL_SPM_BYPASS_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstBypass, MEM_SPM_BYPASS_CB);
    pstBypass = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_bypass_cb
   功能描述  : 释放bypass配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年11月11日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/

NBB_VOID spm_free_all_bypass_cb(NBB_CXT_T NBB_CXT)
{
	SPM_BYPASS_CB *pstBypass = NULL;
	
    for (pstBypass = (SPM_BYPASS_CB*) AVLL_FIRST(SHARED.bypass_tree);
         pstBypass != NULL;
         pstBypass = (SPM_BYPASS_CB*) AVLL_FIRST(SHARED.bypass_tree))
    {
    	
		AVLL_DELETE(SHARED.bypass_tree, pstBypass->spm_bypass_node);    	
    	spm_free_bypass_cb(pstBypass NBB_CCXT);
    }
}


#endif


