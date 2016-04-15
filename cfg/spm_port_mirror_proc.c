/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_port_mirror_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年3月15日
  最近修改   :
  功能描述   : 处理端口镜像功能
  函数列表   :
              spm_alloc_port_mirror_cb
              spm_free_port_mirror_cb
              spm_init_port_mirror_tree
              spm_rcv_dci_set_port_mirror
  修改历史   :
  1.日    期   : 2013年3月15日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char port_mirror_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_port_mirror
 功能描述  : 处理端口镜像IPS消息
 输入参数  : ATG_DCI_SET_PORT_MIRROR *pstSetPortMirror  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_port_mirror(ATG_DCI_SET_PORT_MIRROR *pstSetPortMirror NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucMirrorMode = ATG_DCI_PORT_MIRROR_DISABLE;

    SPM_PORT_MIRROR_CB *pstPortMirror = NULL;
    ATG_DCI_PORT_MIRROR_BASIC_DATA *pstBasicData = NULL;
    
    NBB_BYTE *pucBasicDataStart = NULL;
    
    NBB_ULONG ulOperBasic       = ATG_DCI_OPER_NULL;
    
    SPM_PORT_INFO_CB stPortInfo;
    SPM_PORT_INFO_CB stMirroredPortInfo;
    
    NBB_ULONG ulPortMirrorKey = 0;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_port_mirror");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetPortMirror != NULL);

	if (pstSetPortMirror == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_port_mirror(pstSetPortMirror==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_port_mirror(pstSetPortMirror==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_port_mirror(pstSetPortMirror==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", 0, 
        		FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage));
        
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stMirroredPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    
    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetPortMirror->return_code = ATG_DCI_RC_OK;

    ulPortMirrorKey = pstSetPortMirror->key;

    pstPortMirror = AVLL_FIND(SHARED.port_mirror_tree, &ulPortMirrorKey);
    
    if (pstPortMirror == NULL)       /* 如果条目不存在*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    

    /* 获取子配置操作 */
    ulOperBasic = pstSetPortMirror->oper_basic;

    
    /* 删除整个条目 */
    if (pstSetPortMirror->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除ID=%ld的 端口镜像配置", ulPortMirrorKey));
        
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在"));

            OS_PRINTF("***ERROR***:要删除的ID=%ld 的端口镜像配置不存在!\n", ulPortMirrorKey);

            OS_SPRINTF(ucMessage, "***ERROR***:要删除的ID=%ld 的端口镜像配置不存在!\n", ulPortMirrorKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", ulPortMirrorKey, DEL_UNEXIST_CONFIG_ERROR, "DEL_UNEXIST_CONFIG_ERROR", ucMessage));
            
            pstSetPortMirror->return_code = ATG_DCI_RC_OK;
            
            goto EXIT_LABEL;
        }
        /* 存在，删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  查询到此配置，从tree中删除"));
            spm_dbg_print_port_mirror_head(ulPortMirrorKey, SPM_OPER_DEL);
            spm_dbg_record_port_mirror_head(ulPortMirrorKey, SPM_OPER_DEL);
            
#ifdef SPU 

			if ((pstPortMirror != NULL) && (pstPortMirror->basic_cfg_cb != NULL))
			{
            	ret = spm_get_portid_from_logical_port_index(pstPortMirror->basic_cfg_cb->mirrored_port_index, 
                                                         &stMirroredPortInfo NBB_CCXT);
			}
			
    		if (ret != SUCCESS)
    		{
    		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

    		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

    		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

    		    NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", ulPortMirrorKey, ret, "CALL_FUNC_ERROR", ucMessage));
    		    
    		    pstSetPortMirror->return_code = ATG_DCI_RC_UNSUCCESSFUL;

    		    goto EXIT_LABEL;
    		}

			if ((pstPortMirror != NULL) && (pstPortMirror->basic_cfg_cb != NULL))
			{
            	ret = spm_get_portid_from_logical_port_index(pstPortMirror->basic_cfg_cb->port_index, 
                                                         &stPortInfo NBB_CCXT);
			}
			
    		if (ret != SUCCESS)
    		{
    		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

    		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

    		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

    		    NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", ulPortMirrorKey, ret, "CALL_FUNC_ERROR", ucMessage));
    		    
    		    pstSetPortMirror->return_code = ATG_DCI_RC_UNSUCCESSFUL;

    		    goto EXIT_LABEL;
    		}

            //只有被镜像端口在本槽位时才建立
            if (stMirroredPortInfo.slot_id == SHARED.local_slot_id)
            {
        		ret = ApiC3SetMirrorPort(stMirroredPortInfo.unit_id, 
        		                         stMirroredPortInfo.port_id, 
        		                         stPortInfo.slot_id,
        		                         stPortInfo.port_id, 
        		                         C3_PORT_MIRROR_DISABLE); 
                
                if (ret != SUCCESS)
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetMirrorPort() ret = %d", ret));

                    OS_PRINTF("***ERROR***:ApiC3SetMirrorPort() ret=%d\n", ret);

                    OS_SPRINTF(ucMessage, "***ERROR***:ApiC3SetMirrorPort() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", ulPortMirrorKey, ret, "CALL_C3_FUNC_ERROR", ucMessage));
                    
                    pstSetPortMirror->return_code = ATG_DCI_RC_DEL_FAILED;
                    
                    goto EXIT_LABEL;
                }
            }

#endif
			if (pstPortMirror != NULL)
			{
	            AVLL_DELETE(SHARED.port_mirror_tree, pstPortMirror->spm_port_mirror_node);  

	            //释放ARP表节点的内存空间
	            spm_free_port_mirror_cb(pstPortMirror NBB_CCXT);
			}
        }
    }
    /* 增加或更新条目 */
    else
    {        
                
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  增加ID=%d的 端口镜像配置", ulPortMirrorKey));
            spm_dbg_print_port_mirror_head(ulPortMirrorKey, SPM_OPER_ADD);
            spm_dbg_record_port_mirror_head(ulPortMirrorKey, SPM_OPER_ADD);

            /* 申请一个新条目的内存空间 */
            pstPortMirror = spm_alloc_port_mirror_cb(NBB_CXT);

			if (pstPortMirror != NULL)
			{
	            //插入到tree中
	            pstPortMirror->port_mirror_key = ulPortMirrorKey;

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.port_mirror_tree, pstPortMirror->spm_port_mirror_node);
			}
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  更新ID=%d的 端口镜像配置", ulPortMirrorKey));
            spm_dbg_print_port_mirror_head(ulPortMirrorKey, SPM_OPER_UPD);
            spm_dbg_record_port_mirror_head(ulPortMirrorKey, SPM_OPER_UPD);

        }

		//增加此判断，取消PC-Lint告警
        if (pstPortMirror == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要增加的ID=%ld 的端口镜像配置未能成功申请内存!", ulPortMirrorKey));

            OS_PRINTF("***ERROR***:要增加的ID=%ld 的端口镜像配置未能成功申请内存!\n", ulPortMirrorKey);

            OS_SPRINTF(ucMessage, "***ERROR***:要增加的ID=%ld 的端口镜像配置未能成功申请内存!\n", ulPortMirrorKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", ulPortMirrorKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
            
            pstSetPortMirror->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/
        /* 基本配置 */
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPortMirror, &pstSetPortMirror->basic_data);    

            /* 如果指针为NULL，无数据 */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  PORT MIRROR basic_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstBasicData = (ATG_DCI_PORT_MIRROR_BASIC_DATA*)pucBasicDataStart;

                if (port_mirror_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）基本配置\n");
                    spm_dbg_print_port_mirror_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1）基本配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_port_mirror_basic_cfg(pstBasicData);

                //打印配置
                NBB_TRC_DETAIL((NBB_FORMAT "  被镜像端口Index=%d", pstBasicData->mirrored_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  镜像端口Index=%d", pstBasicData->port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  镜像模式=%d", pstBasicData->mirror_mode));

#ifdef SPU
                ret = spm_get_portid_from_logical_port_index(pstBasicData->mirrored_port_index, 
                                                             &stMirroredPortInfo NBB_CCXT);

        		if (ret != SUCCESS)
        		{
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

        		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", ulPortMirrorKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    pstSetPortMirror->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		    pstSetPortMirror->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

        		    goto EXIT_LABEL;
        		}

                ret = spm_get_portid_from_logical_port_index(pstBasicData->port_index, 
                                                             &stPortInfo NBB_CCXT);

        		if (ret != SUCCESS)
        		{
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

        		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", ulPortMirrorKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    pstSetPortMirror->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		    pstSetPortMirror->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

        		    goto EXIT_LABEL;
        		}

        		switch (pstBasicData->mirror_mode)
        		{
        		    case ATG_DCI_PORT_MIRROR_DISABLE:
        		        ucMirrorMode = C3_PORT_MIRROR_DISABLE;
        		        break;
        		    case ATG_DCI_PORT_MIRROR_INGRESS:
        		        ucMirrorMode = C3_PORT_MIRROR_INGRESS;
        		        break;
        		    case ATG_DCI_PORT_MIRROR_EGRESS:
        		        ucMirrorMode = C3_PORT_MIRROR_EGRESS;
        		        break;
        		    case ATG_DCI_PORT_MIRROR_BOTH:
        		        ucMirrorMode = C3_PORT_MIRROR_INGRESS | C3_PORT_MIRROR_EGRESS;
        		        break;
        		    default:
        		        ucMirrorMode = C3_PORT_MIRROR_DISABLE;
        		        break;
        		}

                //只有被镜像端口在本槽位时才建立
                if (stMirroredPortInfo.slot_id == SHARED.local_slot_id)
                {
            		ret = ApiC3SetMirrorPort(stMirroredPortInfo.unit_id, 
            		                         stMirroredPortInfo.port_id, 
            		                         stPortInfo.slot_id,
            		                         stPortInfo.port_id, 
            		                         ucMirrorMode);
            		
            		if (ret != SUCCESS)
            		{
            		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetMirrorPort ret=%d", ret));

            		    OS_PRINTF("***ERROR***:ApiC3SetMirrorPort() ret=%d\n", ret);

            		    OS_SPRINTF(ucMessage, "***ERROR***:ApiC3SetMirrorPort() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            		    NBB_EXCEPTION((PCT_SPM | PORT_MIRROR_PD, 0, "ld d s s", ulPortMirrorKey, ret, "CALL_C3_FUNC_ERROR", ucMessage));
            		    
            		    pstSetPortMirror->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

            		    goto EXIT_LABEL;
            		}
                }
#endif
                
                /* 保存数据 */
                if (pstPortMirror->basic_cfg_cb == NULL)
                {
                    pstPortMirror->basic_cfg_cb = (ATG_DCI_PORT_MIRROR_BASIC_DATA*)NBB_MM_ALLOC(sizeof(ATG_DCI_PORT_MIRROR_BASIC_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PORT_MIRROR_BASIC_CB);
                }  
                
                OS_MEMCPY(pstPortMirror->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_PORT_MIRROR_BASIC_DATA));
                
            }                 
        }
        else if (ulOperBasic == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperBasic == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }
    
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}

/*****************************************************************************
 函 数 名  : spm_init_port_mirror_tree
 功能描述  : 初始化端口镜像树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_port_mirror_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_port_mirror_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_PORT_MIRROR_CB, port_mirror_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.port_mirror_tree, compare_ulong,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_PORT_MIRROR_CB, spm_port_mirror_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_alloc_port_mirror_cb
 功能描述  : 分配端口镜像结构体的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_PORT_MIRROR_CB* spm_alloc_port_mirror_cb(NBB_CXT_T NBB_CXT)
{
    SPM_PORT_MIRROR_CB *pstPortMirror = NULL;

    NBB_TRC_ENTRY("spm_alloc_port_mirror_cb");

    /* 分配一个新的VC表配置条目。*/
    pstPortMirror = (SPM_PORT_MIRROR_CB *)NBB_MM_ALLOC(sizeof(SPM_PORT_MIRROR_CB), NBB_NORETRY_ACT, MEM_SPM_PORT_MIRROR_CB);
    if (pstPortMirror == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VC表配置条目 */
    OS_MEMSET(pstPortMirror, 0, sizeof(SPM_PORT_MIRROR_CB));
    pstPortMirror->basic_cfg_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pstPortMirror->spm_port_mirror_handle = NBB_CREATE_HANDLE(pstPortMirror, HDL_SPM_PORT_MIRROR_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_PORT_MIRROR_CB allocated at %p with handle %#lx",
                   pstPortMirror, pstPortMirror->spm_port_mirror_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstPortMirror->spm_port_mirror_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstPortMirror);
}

/*****************************************************************************
 函 数 名  : spm_free_port_mirror_cb
 功能描述  : 释放端口镜像结构体的内存空间
 输入参数  : SPM_PORT_MIRROR_CB *pstPortMirror  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_port_mirror_cb(SPM_PORT_MIRROR_CB *pstPortMirror NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_port_mirror_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstPortMirror, sizeof(SPM_PORT_MIRROR_CB), MEM_SPM_PORT_MIRROR_CB); 

    if (pstPortMirror == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_PORT_MIRROR_CB at %p", pstPortMirror));  
    NBB_TRC_FLOW((NBB_FORMAT "Free ID %d", pstPortMirror->port_mirror_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstPortMirror->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPortMirror->basic_cfg_cb, MEM_SPM_PORT_MIRROR_BASIC_CB);  
        pstPortMirror->basic_cfg_cb = NULL;
    }    

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstPortMirror->spm_port_mirror_handle, HDL_SPM_PORT_MIRROR_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstPortMirror, MEM_SPM_PORT_MIRROR_CB);  
    pstPortMirror = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

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

NBB_VOID spm_free_all_port_mirror_cb(NBB_CXT_T NBB_CXT)
{
	SPM_PORT_MIRROR_CB *pstPortMirror = NULL;
	
    for (pstPortMirror = (SPM_PORT_MIRROR_CB*) AVLL_FIRST(SHARED.port_mirror_tree);
         pstPortMirror != NULL;
         pstPortMirror = (SPM_PORT_MIRROR_CB*) AVLL_FIRST(SHARED.port_mirror_tree))
    {
    	
		AVLL_DELETE(SHARED.port_mirror_tree, pstPortMirror->spm_port_mirror_node);     	
    	spm_free_port_mirror_cb(pstPortMirror NBB_CCXT);
    }
}


