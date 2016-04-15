/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_vpls_uc_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年4月18日
  最近修改   :
  功能描述   : VPLS静态单播配置处理
  函数列表   :
              spm_rcv_dci_set_vpls_mc
  修改历史   :
  1.日    期   : 2013年4月18日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef VPLS_UC_CFG

extern unsigned char vpls_uc_cfg_print_setting;


/*****************************************************************************
 函 数 名  : spm_vpls_uc_basic_data_proc
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
NBB_INT spm_vpls_uc_basic_data_proc(ATG_DCI_SET_VPLS_UC *pst_set_vpls_uc,
	NBB_ULONG ul_oper_basic,
    SPM_VPLS_UC_CB *pst_vpls_uc)
{

    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];  
    NBB_INT ret = SUCCESS;
    ATG_DCI_VPLS_UC_BASIC_DATA *pst_basic_data = NULL;
    NBB_BYTE *puc_basic_data_start = NULL;   

    NBB_TRC_ENTRY("spm_vpls_uc_basic_data_proc");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(NULL != pst_set_vpls_uc);

    if (NULL == pst_set_vpls_uc)
    {
       	ret = ADD_VPLS_UC_BASIC_IPS_IS_NULL;
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_vpls_uc)
    {
        ret = ADD_VPLS_UC_BASIC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }
	
    if (ATG_DCI_OPER_ADD == ul_oper_basic)
    {

        /* 计算第一个entry的地址。*/
        puc_basic_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vpls_uc, 
        	&pst_set_vpls_uc->basic_data);    

        /* 如果指针为NULL，无数据 */
        if (NULL == puc_basic_data_start)
        {
            ret = ADD_VPLS_UV_BASIC_DATA_START_IS_NULL;
            
            goto EXIT_LABEL;
        }
        
        /* 基本数据存在处理 (zhangzhm) */
        pst_basic_data = (ATG_DCI_VPLS_UC_BASIC_DATA *)puc_basic_data_start;

        if (SPM_PRINT_CFG == vpls_uc_cfg_print_setting)
        {
            printf("  1）basic cfg\n");
            spm_dbg_print_vpls_uc_basic_cfg(pst_basic_data);
        }

		OS_SPRINTF(uc_message,"  1）basic cfg\n");
		spm_dbg_record_vpls_uc_basic_cfg(pst_basic_data);
		

		SPM_L2_INFO_PRINT_LOG(uc_message,"  1）vpls uc basic cfg add\n");

		
		

        /* 保存数据处理，为空则申请新空间 */
        if (NULL == pst_vpls_uc->basic_cfg_cb)
        {
            pst_vpls_uc->basic_cfg_cb = 
                (ATG_DCI_VPLS_UC_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_UC_BASIC_DATA),
                                                    NBB_NORETRY_ACT,
                                                    MEM_SPM_VPLS_UC_BASIC_CB);

            if(NULL == pst_vpls_uc->basic_cfg_cb)
            {
            	ret = ADD_VPLS_UC_MALLOC_BASIC_MEM_FAIL;
                goto EXIT_LABEL;
            }
        }

        /*数据为新申请或已存在，存储数据 */
        if(NULL != pst_vpls_uc->basic_cfg_cb)   
        {
            OS_MEMCPY(pst_vpls_uc->basic_cfg_cb, pst_basic_data, sizeof(ATG_DCI_VPLS_UC_BASIC_DATA)); 
        } 

		//调用驱动
		#ifdef SPU
		ret = spm_vpls_uc_drv_set_static_mac(&pst_vpls_uc->vpls_uc_key,
				pst_vpls_uc->basic_cfg_cb);

		if(SUCCESS != ret)
		{
			goto EXIT_LABEL;
		}
		#endif
		
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
 函 数 名  : spm_rcv_dci_set_vpls_mc
 功能描述  : VPLS组播配置处理函数
 输入参数  : ATG_DCI_SET_VPLS_MC *pstSetVplsMc  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月12日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_vpls_uc(ATG_DCI_SET_VPLS_UC *pstSetVplsUc)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    
    SPM_VPLS_UC_CB *pstVplsUc = NULL;

    /* 子配置的操作模式 */
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;

    /* VPLS组播的键值 */
    ATG_DCI_VPLS_UC_KEY stVplsUcKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_vpls_uc");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetVplsUc != NULL);

	if (pstSetVplsUc == NULL)
	{
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"*ERR*,r1=%d\n",VPLS_UC_IPS_NULL);
        goto EXIT_LABEL;
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetVplsUc->return_code = ATG_DCI_RC_OK;

    OS_MEMCPY(&stVplsUcKey, &pstSetVplsUc->key, sizeof(ATG_DCI_VPLS_UC_KEY)); 
    pstVplsUc = AVLL_FIND(SHARED.vpls_uc_tree, &stVplsUcKey);

	//条目不存在
    if (NULL == pstVplsUc)    
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    
    
    /* 获取子配置的操作类型 */
    ulOperBasic = pstSetVplsUc->oper_basic;


    /* 删除整个条目 */
    if (pstSetVplsUc->delete_struct == TRUE)
    {
       
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
        	SPM_L2_ERROR_PRINT_LOG(ucMessage,"*ERR*,r1=%d\n",DEL_VPLS_UC_CFG_NULL);
            goto EXIT_LABEL;
        }
		
        /* 存在，删除 */
        else
        {
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_DEL);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_DEL);


			if (pstVplsUc == NULL)
			{
				goto EXIT_LABEL;
			}

			ret = spm_vpls_uc_drv_del_static_mac(pstVplsUc);

			if(SUCCESS != ret)
			{
				pstVplsUc->total_ret_code = ret;
				goto EXIT_LABEL;
			}


			if (pstVplsUc != NULL)
			{
	            //从树中删除节点
	            AVLL_DELETE(SHARED.vpls_uc_tree, pstVplsUc->spm_vpls_uc_node); 

	            //释放节点内存空间
	            spm_free_vpls_uc_cb(pstVplsUc NBB_CCXT);
			}
        }
    }
    /* 增加或更新条目 */
    else
    {        
        
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {                                                                          
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_ADD);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_ADD);
            
            /* 申请一个新条目的内存空间 */
            pstVplsUc = spm_alloc_vpls_uc_cb(NBB_CXT);

			if (NULL != pstVplsUc)
			{
	            //插入到节点中
	            OS_MEMCPY(&pstVplsUc->vpls_uc_key, &stVplsUcKey, sizeof(ATG_DCI_VPLS_UC_KEY));
	            AVLL_INSERT(SHARED.vpls_uc_tree, pstVplsUc->spm_vpls_uc_node);
			}

			else
			{
				SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d\n",ADD_VPLS_UC_MALLOC_NULL);
				pstVplsUc->total_ret_code = ADD_VPLS_UC_MALLOC_NULL;
				goto EXIT_LABEL;
			}
         
        }
        else
        {                                                                                               
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_UPD);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_UPD);
        }

        /***************************************************************************/
        /*                              获取配置                                   */
        /***************************************************************************/
        /******************************** 基本配置 *********************************/
        if (ATG_DCI_OPER_NULL != ulOperBasic)
        {
        	ret = spm_vpls_uc_basic_data_proc(pstSetVplsUc,ulOperBasic,pstVplsUc);
			if (SUCCESS != ret)    
	        {
	        	SPM_L2_ERROR_PRINT_LOG(ucMessage,"ERR,vpls_uc_basic_proc r1=%d\n",ret);
				pstVplsUc->basic_ret_code = ret;
	        } 
        }
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}


/*****************************************************************************
 函 数 名  : spm_init_vpls_uc_tree
 功能描述  : 初始化VPLS静态单播树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月18日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_vpls_uc_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vpls_uc_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_VPLS_UC_CB, vpls_uc_key);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.vpls_uc_tree, spm_vpls_uc_key_compare,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_VPLS_UC_CB, spm_vpls_uc_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_free_vpls_uc_cb
 功能描述  : 释放VPLS静态单播结构体内存
 输入参数  : SPM_VPLS_UC_CB *pstVplsUc  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月18日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_vpls_uc_cb(SPM_VPLS_UC_CB *pstVplsUc NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    //NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_vpls_uc_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstVplsUc, sizeof(SPM_VPLS_UC_CB), MEM_SPM_VPLS_UC_CB); 

    if (pstVplsUc == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pstVplsUc));  
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws_id %d", pstVplsUc->vpws_id_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstVplsUc->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstVplsUc->basic_cfg_cb, MEM_SPM_VPLS_UC_BASIC_CB);  
        pstVplsUc->basic_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstVplsUc->spm_vpls_uc_handle, HDL_SPM_VPLS_UC_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstVplsUc, MEM_SPM_VPLS_UC_CB);  
    pstVplsUc = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_alloc_vpls_uc_cb
 功能描述  : 分配VPLS静态单播内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : SPM_VPLS_UC_CB
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月18日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_VPLS_UC_CB * spm_alloc_vpls_uc_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_VPLS_UC_CB *pstVplsUc = NULL;

    NBB_TRC_ENTRY("spm_alloc_vpls_uc_cb");

    /* 分配一个新的VPLS组播配置条目。*/
    pstVplsUc = (SPM_VPLS_UC_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_UC_CB), NBB_NORETRY_ACT, MEM_SPM_VPLS_UC_CB);
    if (pstVplsUc == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:申请VPLS组播条目空间为NULL!"));
        OS_PRINTF("***ERROR***:申请VPLS组播条目空间为NULL!\n");
        
        goto EXIT_LABEL;
    }

    /* 初始化VPLS组播配置条目 */
    OS_MEMSET(pstVplsUc, 0, sizeof(SPM_VPLS_UC_CB));
    pstVplsUc->basic_cfg_cb = NULL;

    /* 建立用于该VPLS组播配置条目的句柄，作为异步消息交换的相关器。*/
    pstVplsUc->spm_vpls_uc_handle = NBB_CREATE_HANDLE(pstVplsUc, HDL_SPM_VPLS_UC_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_UC_CB allocated at %p with handle %#lx",
                   pstVplsUc, pstVplsUc->spm_vpls_uc_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstVplsUc->spm_vpls_uc_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstVplsUc);
}

/*****************************************************************************
   函 数 名  : spm_free_all_vpls_uc_cb
   功能描述  : 释放vpls单播配置所有的内存空间
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

NBB_VOID spm_free_all_vpls_uc_cb(NBB_CXT_T NBB_CXT)
{
	SPM_VPLS_UC_CB *pstVplsUc = NULL;
	
    for (pstVplsUc = (SPM_VPLS_UC_CB*) AVLL_FIRST(SHARED.vpls_uc_tree);
         pstVplsUc != NULL;
         pstVplsUc = (SPM_VPLS_UC_CB*) AVLL_FIRST(SHARED.vpls_uc_tree))
    {
    	
		AVLL_DELETE(SHARED.vpls_uc_tree, pstVplsUc->spm_vpls_uc_node);     	
    	spm_free_vpls_uc_cb(pstVplsUc NBB_CCXT);
    }
}


#endif
