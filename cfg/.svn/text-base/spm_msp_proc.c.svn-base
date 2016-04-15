/******************************************************************************

                  版权所有 (C), 1999-2014, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_msp_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2014年4月10日
  最近修改   :
  功能描述   : 处理端口镜像功能
  函数列表   :
              spm_alloc_msp_cb
              spm_free_msp_cb
              spm_init_msp_tree
              spm_rcv_dci_set_msp
  修改历史   :
  1.日    期   : 2014年4月10日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char msp_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_msp
 功能描述  : 处理端口镜像IPS消息
 输入参数  : ATG_DCI_SET_MSP *pstSetMsp  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_msp(ATG_DCI_SET_MSP *pstSetMsp NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
	NBB_BYTE ucC3Unit = 0;
    SPM_MSP_CB *pstMsp = NULL;
    ATG_DCI_MSP_BASIC *pstBasicData = NULL;
    
    NBB_BYTE *pucBasicDataStart = NULL;
    
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    
    NBB_ULONG ulMspKey = 0;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    //NBB_USHORT i = 0;

    APS_LMSP_PRO_GROUP pstlmsp;

    SPM_PORT_INFO_CB stPortInfo ;
    SPM_PORT_INFO_CB stBakPortInfo ;


    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));  
    OS_MEMSET(&stBakPortInfo, 0, sizeof(SPM_PORT_INFO_CB));  
    OS_MEMSET(&pstlmsp, 0, sizeof(APS_LMSP_PRO_GROUP));

    NBB_TRC_ENTRY("spm_rcv_dci_set_msp");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetMsp != NULL);

	if (pstSetMsp == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_msp(pstSetMsp==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_msp(pstSetMsp==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_msp(pstSetMsp==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", 0, 
        		FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage));
        
        goto EXIT_LABEL;
    }
    
    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetMsp->return_code = ATG_DCI_RC_OK;

    ulMspKey = pstSetMsp->key;

    pstMsp = AVLL_FIND(SHARED.msp_tree, &ulMspKey);
    
    if (pstMsp == NULL)       /* 如果条目不存在*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    

    /* 获取子配置操作 */
    ulOperBasic = pstSetMsp->oper_basic;

    
    /* 删除整个条目 */
    if (pstSetMsp->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除ID=%ld的 MSP保护协议", ulMspKey));
        
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在"));

            OS_PRINTF("***ERROR***:要删除的ID=%ld 的MSP保护协议配置不存在!\n", ulMspKey);

            OS_SPRINTF(ucMessage, "***ERROR***:要删除的ID=%ld 的MSP保护协议配置不存在!\n", ulMspKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, DEL_UNEXIST_CONFIG_ERROR, "DEL_UNEXIST_CONFIG_ERROR", ucMessage));
            
            pstSetMsp->return_code = ATG_DCI_RC_OK;
            
            goto EXIT_LABEL;
        }
        /* 存在，删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  查询到此配置，从tree中删除"));
            spm_dbg_print_msp_head(ulMspKey, SPM_OPER_DEL);
            spm_dbg_record_msp_head(ulMspKey, SPM_OPER_DEL);
            
#ifdef SPU
           delApsLmspProGroup(ulMspKey);
#endif    

#if 0

			if ((pstMsp != NULL) && (pstMsp->basic_cfg_cb != NULL))
			{
            	ret = spm_get_portid_from_logical_port_index(pstMsp->basic_cfg_cb->mirrored_port_index, 
                                                         &stMirroredPortInfo NBB_CCXT);
			}
			
    		if (ret != SUCCESS)
    		{
    		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

    		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

    		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

    		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_FUNC_ERROR", ucMessage));
    		    
    		    pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;

    		    goto EXIT_LABEL;
    		}

			if ((pstMsp != NULL) && (pstMsp->basic_cfg_cb != NULL))
			{
            	ret = spm_get_portid_from_logical_port_index(pstMsp->basic_cfg_cb->port_index, 
                                                         &stPortInfo NBB_CCXT);
			}
			
    		if (ret != SUCCESS)
    		{
    		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

    		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

    		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

    		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_FUNC_ERROR", ucMessage));
    		    
    		    pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;

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

                    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_C3_FUNC_ERROR", ucMessage));
                    
                    pstSetMsp->return_code = ATG_DCI_RC_DEL_FAILED;
                    
                    goto EXIT_LABEL;
                }
            }

#endif
			if (pstMsp != NULL)
			{
	            AVLL_DELETE(SHARED.msp_tree, pstMsp->spm_msp_node);  

	            //释放MSP保护协议节点的内存空间
	            spm_free_msp_cb(pstMsp NBB_CCXT);
			}
        }
    }
    /* 增加或更新条目 */
    else
    {        
                
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  增加ID=%d的 MSP保护协议配置", ulMspKey));
            spm_dbg_print_msp_head(ulMspKey, SPM_OPER_ADD);
            spm_dbg_record_msp_head(ulMspKey, SPM_OPER_ADD);

            /* 申请一个新条目的内存空间 */
            pstMsp = spm_alloc_msp_cb(NBB_CXT);

			if (pstMsp != NULL)
			{
	            //插入到tree中
	            pstMsp->msp_key = ulMspKey;

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.msp_tree, pstMsp->spm_msp_node);
			}
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  更新ID=%d的 MSP保护协议配置", ulMspKey));
            spm_dbg_print_msp_head(ulMspKey, SPM_OPER_UPD);
            spm_dbg_record_msp_head(ulMspKey, SPM_OPER_UPD);

        }

		//增加此判断，取消PC-Lint告警
        if (pstMsp == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要增加的ID=%ld 的MSP保护协议配置未能成功申请内存!", ulMspKey));

            OS_PRINTF("***ERROR***:要增加的ID=%ld 的MSP保护协议配置未能成功申请内存!\n", ulMspKey);

            OS_SPRINTF(ucMessage, "***ERROR***:要增加的ID=%ld 的MSP保护协议配置未能成功申请内存!\n", ulMspKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
            
            pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/
        /* 基本配置 */
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetMsp, &pstSetMsp->basic_data);    

            /* 如果指针为NULL，无数据 */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  MSP basic_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstBasicData = (ATG_DCI_MSP_BASIC*)pucBasicDataStart;

                if (msp_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）基本配置\n");
                    spm_dbg_print_msp_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1）基本配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_msp_basic_cfg(pstBasicData);

                //打印配置
                NBB_TRC_DETAIL((NBB_FORMAT "  保护类型=%d", pstBasicData->prot_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  主用物理端口Index=%ld", pstBasicData->master_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  备用物理端口Index=%ld", pstBasicData->slave_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  拖延时间=%d", pstBasicData->delay_time));
                NBB_TRC_DETAIL((NBB_FORMAT "  等待恢复时间=%ld", pstBasicData->restore_time));
                NBB_TRC_DETAIL((NBB_FORMAT "  返回类型=%ld", pstBasicData->return_type));
                
                //传递MSP保护配置给保护模块
                //spm_set_aps_msp_prot_group(ulMspKey);
                
                pstlmsp.apsProType = pstBasicData->prot_type;

                pstlmsp.wPortIndex = pstBasicData->master_port_index;      
                spm_get_portid_from_physical_port_index(pstBasicData->master_port_index,&stPortInfo NBB_CCXT);
                pstlmsp.wSlot = stPortInfo.slot_id;
                pstlmsp.wCardId = stPortInfo.card_id;
                pstlmsp.wPortId = stPortInfo.port_id;

                pstlmsp.pPortIndex = pstBasicData->slave_port_index;
                spm_get_portid_from_physical_port_index(pstBasicData->slave_port_index,&stBakPortInfo NBB_CCXT);
                pstlmsp.pSlot = stBakPortInfo.slot_id;
                pstlmsp.pCardId = stBakPortInfo.card_id;
                pstlmsp.pPortId = stBakPortInfo.port_id;        

                pstlmsp.holdoffTimeExpire = pstBasicData->delay_time;
                pstlmsp.wtrTimeExpire = pstBasicData->restore_time;
                pstlmsp.revertMode = pstBasicData->return_type;
                
                ret = SUCCESS;
                ret = addApsLmspProGroup(ulMspKey,&pstlmsp);

                if (ERROR == ret )
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***addApsLmspProGroup ret=ERROR"));
                    OS_SPRINTF(ucMessage, "***ERROR***addApsLmspProGroup ret=ERROR \n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    goto EXIT_LABEL;    
                }

#ifdef SPU
	        	//msp 1+1
	        	if ((pstBasicData->prot_type == ATG_DCI_MSP_UNI_LINK_1J1_PROT)
	        	 || (pstBasicData->prot_type == ATG_DCI_MSP_BI_LINK_1J1_PROT))
				{
					for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
					{
	                	ApiC3SetFrrGroupProtType(ucC3Unit, SHARED.msp_frr_group_id[ulMspKey - 1], VP_NHI_1PLUS1);
	                }
				}
	        	//msp 1:1
	        	else if ((pstBasicData->prot_type == ATG_DCI_MSP_UNI_LINK_1B1_PROT)
	        	 	  || (pstBasicData->prot_type == ATG_DCI_MSP_BI_LINK_1B1_PROT))
	        	{
					for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
					{
	                	ApiC3SetFrrGroupProtType(ucC3Unit, SHARED.msp_frr_group_id[ulMspKey - 1], VP_NHI_FRR);
	                }
	        	}
	        	else
	        	{
					for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
					{
	                	ApiC3SetFrrGroupProtType(ucC3Unit, SHARED.msp_frr_group_id[ulMspKey - 1], VP_NHI_GROUP);
	                }
	        	}
#endif

#if 0
                ret = spm_get_portid_from_logical_port_index(pstBasicData->mirrored_port_index, 
                                                             &stMirroredPortInfo NBB_CCXT);

        		if (ret != SUCCESS)
        		{
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

        		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		    pstSetMsp->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

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

        		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		    pstSetMsp->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

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

            		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_C3_FUNC_ERROR", ucMessage));
            		    
            		    pstSetMsp->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

            		    goto EXIT_LABEL;
            		}
                }
#endif
                
                /* 保存数据 */
                if (pstMsp->basic_cfg_cb == NULL)
                {
                    pstMsp->basic_cfg_cb = (ATG_DCI_MSP_BASIC*)NBB_MM_ALLOC(sizeof(ATG_DCI_MSP_BASIC),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_MSP_BASIC_CB);
                }  
                
                OS_MEMCPY(pstMsp->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_MSP_BASIC));
                
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
 函 数 名  : spm_init_msp_tree
 功能描述  : 初始化端口镜像树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年4月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_msp_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_msp_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_MSP_CB, msp_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.msp_tree, compare_ulong,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_MSP_CB, spm_msp_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_alloc_msp_cb
 功能描述  : 分配MSP保护结构体的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年4月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_MSP_CB* spm_alloc_msp_cb(NBB_CXT_T NBB_CXT)
{
    SPM_MSP_CB *pstMsp = NULL;

    NBB_TRC_ENTRY("spm_alloc_msp_cb");

    /* 分配一个新的MSP保护配置条目。*/
    pstMsp = (SPM_MSP_CB *)NBB_MM_ALLOC(sizeof(SPM_MSP_CB), NBB_NORETRY_ACT, MEM_SPM_MSP_CB);
    if (pstMsp == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化MSP保护配置条目 */
    OS_MEMSET(pstMsp, 0, sizeof(SPM_MSP_CB));
    pstMsp->basic_cfg_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pstMsp->spm_msp_handle = NBB_CREATE_HANDLE(pstMsp, HDL_SPM_MSP_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_MSP_CB allocated at %p with handle %#lx",
                   pstMsp, pstMsp->spm_msp_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstMsp->spm_msp_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstMsp);
}

/*****************************************************************************
 函 数 名  : spm_free_msp_cb
 功能描述  : 释放MSP保护结构体的内存空间
 输入参数  : SPM_MSP_CB *pstMsp  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2014年4月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_msp_cb(SPM_MSP_CB *pstMsp NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_msp_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstMsp, sizeof(SPM_MSP_CB), MEM_SPM_MSP_CB); 

    if (pstMsp == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_MSP_CB at %p", pstMsp));  
    NBB_TRC_FLOW((NBB_FORMAT "Free ID %d", pstMsp->msp_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstMsp->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstMsp->basic_cfg_cb, MEM_SPM_MSP_BASIC_CB);  
        pstMsp->basic_cfg_cb = NULL;
    }    

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstMsp->spm_msp_handle, HDL_SPM_MSP_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstMsp, MEM_SPM_MSP_CB);  
    pstMsp = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_msp_cb
   功能描述  : 释放MSP保护配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年04月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_all_msp_cb(NBB_CXT_T NBB_CXT)
{
	SPM_MSP_CB *pstMsp = NULL;
	
    for (pstMsp = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree);
         pstMsp != NULL;
         pstMsp = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree))
    {    	
		AVLL_DELETE(SHARED.msp_tree, pstMsp->spm_msp_node);     	
    	spm_free_msp_cb(pstMsp NBB_CCXT);
    }
}

