/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_associate_if_proc.c
  版 本 号   : 初稿
  作    者   : zhangzhm
  生成日期   : 2013年11月20日
  最近修改   :
  功能描述   : 关联接口配置块
  函数列表   :
            

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char associate_if_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_global
 功能描述  : 关联接口组IPS消息处理函数
 输入参数  : ATG_DCI_SET_ASSOCIATE_IF *pstSetAssociateIf
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月20日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_associate_if(ATG_DCI_SET_ASSOCIATE_IF *pstSetAssociateIf NBB_CCXT_T NBB_CXT)
{
    
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	NBB_BYTE ucMessageLen = 0;
		
	NBB_ULONG i = 0;
	NBB_ULONG j = 0;
	NBB_INT ret = SUCCESS;
	NBB_BYTE ucC3Unit = 0;
	SPM_PORT_INFO_CB stPortInfo;
	NBB_ULONG ulPortIndex = 0;
	NBB_USHORT usPortId = 0;
	NBB_BYTE usSlotId = 0;
	NBB_BYTE pucPortNum = 0;
	SPM_LAG_PORT_INFO_CB  stTriggerLagPortInfo[ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ];
	SPM_LAG_PORT_INFO_CB  stActLagPortInfo[ATG_DCI_ASSOCIATE_IF_ACT_NUM];
	PORT_GROUP_INFO IfGoupInfo;
	NBB_USHORT usSlotPort = 0;
    NBB_BYTE ucFindCfgFlag = 0;
		
	SPM_ASSOCIATE_IF_CB *pstAssociateIfCb = NULL;
	
	/* 用来保存从IPS中获取的子配置 */
	
	ATG_DCI_ASSOCIATE_IF_BASIC *basic_cfg_cb = NULL;
	ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *trigger_if_cfg_cb[ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ];
	ATG_DCI_ASSOCIATE_IF_ACT_IF *act_if_cfg_cb[ATG_DCI_ASSOCIATE_IF_ACT_NUM];
	
	
	/* IPS消息偏移的首地址 */
	NBB_BYTE *pucBasicDataStart  = NULL;
	NBB_BYTE *pucTriggerIfDataStart	  = NULL;
	NBB_BYTE *pucActIfDataStart	  = NULL;
	
	/* 子配置的操作模式 */
	NBB_ULONG ulOperBasic	= ATG_DCI_OPER_NULL;
	NBB_ULONG ulOperTriggerIf 	= ATG_DCI_OPER_NULL;
	NBB_ULONG ulOperActIf	= ATG_DCI_OPER_NULL;

	/* 关联接口组的键值 */
	NBB_BYTE ucAssociateIfKey = 0;
	
	/*初始化结构体*/
	OS_MEMSET(&stPortInfo,0,sizeof(SPM_PORT_INFO_CB));
	OS_MEMSET(stTriggerLagPortInfo,0,sizeof(SPM_LAG_PORT_INFO_CB)*(ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM));
	OS_MEMSET(stActLagPortInfo,0,sizeof(SPM_LAG_PORT_INFO_CB)*(ATG_DCI_ASSOCIATE_IF_ACT_NUM));
		
	NBB_BYTE ucIfExist = ATG_DCI_EXIST;
	NBB_TRC_ENTRY("spm_rcv_dci_set_associate_if");
	
	/* 输入参数指针必须有效 */
	NBB_ASSERT(pstSetAssociateIf != NULL);
	
	if (pstSetAssociateIf == NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:spm_rcv_dci_set_associate_if(pstSetAssociateIf==NULL)"));
	
		OS_PRINTF("***ERROR***:spm_rcv_dci_set_associate_if(pstSetAssociateIf==NULL)\n");
	
		OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_associate_if(pstSetAssociateIf==NULL)\n");
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
	
		NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage));
			
		goto EXIT_LABEL;
	}
	
	
	//首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
	pstSetAssociateIf->return_code = ATG_DCI_RC_OK;
	
	/* 初始化触发接口的指针数组 */
	for (i=0; i<ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM; i++)
	{
		trigger_if_cfg_cb[i] = NULL;
	}

	/* 初始化动作接口的指针数组 */
	for (i=0; i<ATG_DCI_ASSOCIATE_IF_ACT_NUM; i++)
	{
		act_if_cfg_cb[i] = NULL;
	}
	/*初始化触发接口存在标志位*/

    
		
	ucAssociateIfKey = pstSetAssociateIf->key;	 
	pstAssociateIfCb= AVLL_FIND(SHARED.associate_if_tree, &ucAssociateIfKey);
		
	if (pstAssociateIfCb == NULL)    //条目不存在
	{
		ucIfExist = ATG_DCI_UNEXIST;
	}
		
		
	/* 获取子配置的操作类型 */
	ulOperBasic  = pstSetAssociateIf->oper_basic;
	ulOperTriggerIf	  = pstSetAssociateIf->oper_trigger_if;
	ulOperActIf  = pstSetAssociateIf->oper_act_if;
		
	
	
	/* 删除整个条目 */
	if (pstSetAssociateIf->delete_struct == TRUE)
	{
		NBB_TRC_DETAIL((NBB_FORMAT "  删除AssociateIf key = %d 的关联接口组配置", ucAssociateIfKey)); 	   
	
		/* 如果条目不存在，不删除 */
		if (ucIfExist == ATG_DCI_UNEXIST)
		{
			
			pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要删除的配置并不存在，退出"));
	
			OS_PRINTF("***ERROR***:要删除的AssociateIf key=%d 的关联接口组配置不存在!\n", ucAssociateIfKey);
	
			OS_SPRINTF(ucMessage, "***ERROR***:要删除的AssociateIf key=%d 的关联接口组配置不存在!\n", ucAssociateIfKey);
			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
	
			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, DEL_UNEXIST_CONFIG_ERROR, "DEL_UNEXIST_CONFIG_ERROR", ucMessage));
				
			goto EXIT_LABEL;
		}
		/* 存在，删除 */

		else
		{

			ret = SUCCESS;
			NBB_TRC_FLOW((NBB_FORMAT "	查询到此配置，删除此关联接口组配置，并从tree中删除存储数据"));
            
            spm_dbg_print_associate_if_head(ucAssociateIfKey, SPM_OPER_DEL);
            spm_dbg_record_associate_if_head(ucAssociateIfKey, SPM_OPER_DEL);
		
			 ret = aps_del_monitor_group(ucAssociateIfKey);

			if(ret != SUCCESS)

			{
				NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除asociate_if(ID=%d)失败! >>> aps_del_monitor_group() ret=%d",
                       ucAssociateIfKey, ret));

                OS_PRINTF("***ERROR***:删除asociate_if(ID=%d)失败! >>> aps_del_monitor_group() ret=%d", ucAssociateIfKey, ret);

                OS_SPRINTF(ucMessage,
                    "***ERROR***:删除asociate_if(ID=%d)失败! >>> aps_del_monitor_group() ret=%d\n",
                    ucAssociateIfKey,
                    ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey,
                        ret,
                        "aps_del_monitor_group",
                        ucMessage));
                        
                pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;

                goto EXIT_LABEL;
			}

			if (pstAssociateIfCb != NULL)
			{
				//从树中删除节点
				AVLL_DELETE(SHARED.associate_if_tree, pstAssociateIfCb->spm_associate_if_node); 
	
				//释放节点内存空间
				spm_free_associate_if_cb(pstAssociateIfCb NBB_CCXT);
			}
		}
	}
		/* 增加或更新条目 */
	else
	{		 
			
		/* 如果条目不存在，树中要增加条目 */
		if (ucIfExist == ATG_DCI_UNEXIST)
		{
			NBB_TRC_DETAIL((NBB_FORMAT "  增加关联接口配置(id = %d)", ucAssociateIfKey));
			spm_dbg_print_associate_if_head(ucAssociateIfKey, SPM_OPER_ADD);
			spm_dbg_record_associate_if_head(ucAssociateIfKey, SPM_OPER_ADD);
				
			pstAssociateIfCb = spm_alloc_associate_if_cb(NBB_CXT);
	
			if (pstAssociateIfCb != NULL)
			{
				//申请空间后先插入到tree中
				pstAssociateIfCb->key= ucAssociateIfKey;

				//coverity[no_effect_test]
				AVLL_INSERT(SHARED.associate_if_tree, pstAssociateIfCb->spm_associate_if_node);
                
			}
		}
		else
		{
			NBB_TRC_DETAIL((NBB_FORMAT "  更新associate id =%ld的 关联接口配置", ucAssociateIfKey));
			spm_dbg_print_associate_if_head(ucAssociateIfKey, SPM_OPER_UPD);
			spm_dbg_record_associate_if_head(ucAssociateIfKey, SPM_OPER_UPD);
		}
	
			//增加此判断，取消PC-Lint告警
		if (pstAssociateIfCb == NULL)
		{
			pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置未能成功申请内存!", ucAssociateIfKey));
	
			OS_PRINTF("***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置未能成功申请内存!", ucAssociateIfKey);
	
			OS_SPRINTF(ucMessage, "***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置未能成功申请内存!", ucAssociateIfKey);
			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
	
			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
				
			goto EXIT_LABEL;
		}		 
	
			/***************************************************************************/
			/* 获取配置 															   */
			/***************************************************************************/
			/********************************基本配置 *********************************/
		if (ulOperBasic == ATG_DCI_OPER_ADD)
		{
	
			/* 计算第一个entry的地址。*/
			pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetAssociateIf, 
																		&pstSetAssociateIf->basic_data);	  
	
				/* 首地址为NULL，异常 */
			if (pucBasicDataStart == NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "	Associate_if BasicDataStart is NULL.")); 
				NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
			}
			else
			
			{
				basic_cfg_cb = (ATG_DCI_ASSOCIATE_IF_BASIC*)pucBasicDataStart;
	
				/* 保存数据 */
				if (pstAssociateIfCb->basic_cfg_cb == NULL)
				{
					pstAssociateIfCb->basic_cfg_cb = (ATG_DCI_ASSOCIATE_IF_BASIC*)NBB_MM_ALLOC(sizeof(ATG_DCI_ASSOCIATE_IF_BASIC),
																NBB_NORETRY_ACT,
																MEM_SPM_ASSOCIATE_BASIC_CB);
                    if (pstAssociateIfCb->basic_cfg_cb != NULL)
                    {
                        OS_MEMCPY(pstAssociateIfCb->basic_cfg_cb, basic_cfg_cb, sizeof(ATG_DCI_ASSOCIATE_IF_BASIC));
                    }
				}

                if (pstAssociateIfCb->basic_cfg_cb == NULL)
        		{
        			pstSetAssociateIf->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
        			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的基本配置未能成功申请内存!", ucAssociateIfKey));
        	
        			OS_PRINTF("***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的基本配置未能成功申请内存!", ucAssociateIfKey);
        	
        			OS_SPRINTF(ucMessage, "***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的基本配置未能成功申请内存!", ucAssociateIfKey);
        			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        	
        			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
        	
        		}	
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
	
			/******************************** 触发接口配置 *********************************/
		if (ulOperTriggerIf == ATG_DCI_OPER_ADD)
		{
	
			/* 计算第一个entry的地址。*/
			pucTriggerIfDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetAssociateIf, 
																	 &pstSetAssociateIf->trigger_if_data);	

	
			/* 首地址为NULL，异常 */
			if(pucTriggerIfDataStart == NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "	pucTriggerIfDataStart is NULL."));  
				NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
			}
	
	 		else
	 		{
				if (associate_if_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2）trigger_if接口配置[1-8] (trigger_if_num=%d) ADD\n", pstSetAssociateIf->trigger_if_num);
                }

                OS_SPRINTF(ucMessage, "  2）trigger_if接口配置[1-8] (trigger_if_num=%d) ADD\n", pstSetAssociateIf->trigger_if_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
				
				
	 			for (i=0; i < pstSetAssociateIf->trigger_if_num; i++)
				{
					trigger_if_cfg_cb[i] = (ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *)
					(pucTriggerIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF))) * i); 
                    
                     NBB_TRC_DETAIL((NBB_FORMAT "ulPortIndex=%d ", trigger_if_cfg_cb[i]->interface_index));

                    if (associate_if_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_associate_trigger_cfg(trigger_if_cfg_cb[i], i);
                    }

                    //spm_dbg_record_associate_trigger_cfg(trigger_if_cfg_cb[i], i);
						
					ulPortIndex = trigger_if_cfg_cb[i]->interface_index;
					
	 				ret = spm_get_portid_from_logical_port_index(ulPortIndex,&stPortInfo NBB_CCXT);
					
					if(ret !=SUCCESS)
					{
						pstSetAssociateIf->trigger_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
						pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                	
                   		NBB_TRC_DETAIL((NBB_FORMAT " 逻辑端口index=%ld未下发，只保存配置\n ",ulPortIndex));
		
						OS_PRINTF(" 逻辑端口index=%ld未下发，只保存配置\n",ulPortIndex);
		
						OS_SPRINTF(ucMessage, " 逻辑端口index=%ld未下发，只保存配置\n ",ulPortIndex);
						BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
		
						NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "spm_get_portid_from_logical_port_index", ucMessage));

                        /*保存配置，下逻辑口时再加入关联组*/
                        ucFindCfgFlag = 0;

                        for(j = 0;j < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ;j++)
                        {
                            /*重复下发*/
                            if((pstAssociateIfCb->trigger_if_cfg_cb[j] != NULL)&&
                                (pstAssociateIfCb->trigger_if_cfg_cb[j]->interface_index == trigger_if_cfg_cb[i]->interface_index))
                            {
                                /*写逻辑接口是否存在标志位，以便下逻辑接口时触发关联接口组配置*/
                                pstAssociateIfCb->trigger_if_exist[j] = UNEXIST;
                                ucFindCfgFlag = 1;
                                break;
                            }
                        }

                        /*不是重复下发*/
                        if(ucFindCfgFlag != 1)
                        {
                            for(j = 0;j < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ;j++)
        					{
        						if(pstAssociateIfCb->trigger_if_cfg_cb[j] == NULL)
        						{
        							pstAssociateIfCb->trigger_if_cfg_cb[j] = (ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *)NBB_MM_ALLOC(sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF),
        				                                                                  NBB_NORETRY_ACT,
        				                                                                  MEM_SPM_ASSOCIATE_TRIGGER_IF_CB);
                                    if(pstAssociateIfCb->trigger_if_cfg_cb[j] != NULL)
                                    {
                                        OS_MEMCPY(pstAssociateIfCb->trigger_if_cfg_cb[j],trigger_if_cfg_cb[i],sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF));
                                        /*写逻辑接口是否存在标志位，以便下逻辑接口时触发关联接口组配置*/
                                        pstAssociateIfCb->trigger_if_exist[j] = UNEXIST;
        							    break;  
                                    }

                                    if (pstAssociateIfCb->trigger_if_cfg_cb[j] == NULL)
                            		{
                            			pstSetAssociateIf->trigger_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey));
                            	
                            			OS_PRINTF("***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey);
                            	
                            			OS_SPRINTF(ucMessage, "***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey);
                            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                            	
                            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
                                        break;
                            	
                            		}	
                                 
        							
        						}
        					
        					}
                        }
					}

                    else
                    {

					    if((stPortInfo.port_type == ATG_DCI_SUB_INTFACE)||(stPortInfo.port_type == ATG_DCI_INTFACE))
    					{
    						usPortId = stPortInfo.port_id;
    						usSlotId = stPortInfo.slot_id;
    						
    						OS_MEMSET(&IfGoupInfo,0,sizeof(PORT_GROUP_INFO));

    						IfGoupInfo.usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    						IfGoupInfo.cardId = stPortInfo.card_id;
    						IfGoupInfo.lacpState = 1;
    						IfGoupInfo.actionType = 0;
    						
    						ret = SUCCESS;

    						ret = aps_add_monitor_group_port(ucAssociateIfKey,&IfGoupInfo);

    						if(ret == SUCCESS)
    						{
    						    ucFindCfgFlag = 0;

                                for(j = 0;j < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ;j++)
                                {
                                    /*重复下发*/
                                    if((pstAssociateIfCb->trigger_if_cfg_cb[j] != NULL)&&
                                        (pstAssociateIfCb->trigger_if_cfg_cb[j]->interface_index == trigger_if_cfg_cb[i]->interface_index))
                                    {
                                        pstAssociateIfCb->trigger_if_exist[j] = EXIST;
                                        ucFindCfgFlag = 1;
                                        break;
                                    }
                                }

                                /*不是重复下发*/
                                if(ucFindCfgFlag != 1)
                                {
    							    for(j = 0;j < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ;j++)
        							{
        								if(pstAssociateIfCb->trigger_if_cfg_cb[j] == NULL)
        								{
        									pstAssociateIfCb->trigger_if_cfg_cb[j] = (ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *)NBB_MM_ALLOC(sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF),
        				                                                                    NBB_NORETRY_ACT,
        				                                                                    MEM_SPM_ASSOCIATE_TRIGGER_IF_CB);
                                            if(pstAssociateIfCb->trigger_if_cfg_cb[j]!= NULL)
                                            {
                                                OS_MEMCPY(pstAssociateIfCb->trigger_if_cfg_cb[j],trigger_if_cfg_cb[i],sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF));
                                                /*逻辑接口已存在，已真正加入接口组*/
                                                pstAssociateIfCb->trigger_if_exist[j] = EXIST;
                                                pstAssociateIfCb->trigger_if_num++;
        									    break; 
                                            }

                                            if (pstAssociateIfCb->trigger_if_cfg_cb[j] == NULL)
                                    		{
                                    			pstSetAssociateIf->trigger_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                                    			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey));
                                    	
                                    			OS_PRINTF("***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                    	
                                    			OS_SPRINTF(ucMessage, "***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                    			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                    	
                                    			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));

                                                break;
                                    		}
                                            
        									
        								}
        					
        							}
                                }
    						}
    					}

						

					    if(stPortInfo.port_type == ATG_DCI_LAG)
    					{
    						usPortId = stPortInfo.port_id;
                            
                            /*获取lag口所有成员物理端口，加入关联组*/
    						spm_get_lag_all_ports_info(usPortId - ATG_DCI_LAG_OFFSET,stTriggerLagPortInfo,&pucPortNum NBB_CCXT);

    						if(pucPortNum == 0)
    						{
    							pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
								pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            	
               					NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: 逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret));
	
								OS_PRINTF("***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
	
								OS_SPRINTF(ucMessage, "***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
								BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
	
								NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "spm_get_lag_all_ports_info", ucMessage));
			
    						}

    						else
    						{
    						
    							ret = SUCCESS;
    						
    							for(j = 0;j < pucPortNum;j++)
    							{
    								usPortId = stTriggerLagPortInfo[j].port_id;
    								usSlotId = stTriggerLagPortInfo[j].slot_id;
    						
    								OS_MEMSET(&IfGoupInfo,0,sizeof(PORT_GROUP_INFO));

    								IfGoupInfo.usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    								IfGoupInfo.lacpState = stTriggerLagPortInfo[j].port_consult;
    								IfGoupInfo.actionType = 0;
                                    IfGoupInfo.cardId = stTriggerLagPortInfo[j].card_id;
    								
    								ret += aps_add_monitor_group_port(ucAssociateIfKey,&IfGoupInfo);
    							}
    						
    							if(ret == SUCCESS)
                                {                     
        						    ucFindCfgFlag = 0;

                                    for(j = 0;j < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ;j++)
                                    {
                                        /*重复下发*/
                                        if((pstAssociateIfCb->trigger_if_cfg_cb[j] != NULL)&&
                                            (pstAssociateIfCb->trigger_if_cfg_cb[j]->interface_index == trigger_if_cfg_cb[i]->interface_index))
                                        {
                                            pstAssociateIfCb->trigger_if_exist[j] = EXIST;
                                            ucFindCfgFlag = 1;
                                            break;
                                        }
                                    }

                                    /*不是重复下发*/
                                    if(ucFindCfgFlag != 1)
                                    {
        							    for(j = 0;j < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ;j++)
            							{
            								if(pstAssociateIfCb->trigger_if_cfg_cb[j] == NULL)
            								{
            									pstAssociateIfCb->trigger_if_cfg_cb[j] = (ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *)NBB_MM_ALLOC(sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF),
            				                                                                    NBB_NORETRY_ACT,
            				                                                                    MEM_SPM_ASSOCIATE_TRIGGER_IF_CB);
                                                if(pstAssociateIfCb->trigger_if_cfg_cb[j]!= NULL)
                                                {
                                                    OS_MEMCPY(pstAssociateIfCb->trigger_if_cfg_cb[j],trigger_if_cfg_cb[i],sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF));
                                                    /*逻辑接口已存在，已真正加入接口组*/
                                                    pstAssociateIfCb->trigger_if_exist[j] = EXIST;
                                                    pstAssociateIfCb->trigger_if_num++;
            									    break; 
                                                }

                                                if (pstAssociateIfCb->trigger_if_cfg_cb[j] == NULL)
                                        		{
                                        			pstSetAssociateIf->trigger_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                                        			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey));
                                        	
                                        			OS_PRINTF("***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                        	
                                        			OS_SPRINTF(ucMessage, "***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的触发接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                        			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                        	
                                        			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));

                                                    break;
                                        		}
                                                
            									
            								}
            					
            							}
                                    }
        						}

    							else if(ret != SUCCESS)
    							{
    								pstSetAssociateIf->trigger_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
    	                	
    	                   			NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: aps_add_monitor_group_port() ret=%d", ret));
    			
    								OS_PRINTF("***ERROR***:aps_add_monitor_group_port() ret=%d\n", ret);
    			
    								OS_SPRINTF(ucMessage, "***ERROR***:aps_add_monitor_group_port() ret=%d\n", ret);
    								BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    				
    								NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "aps_add_monitor_group_port", ucMessage));
    						
    							}
    						}
    		
    	 				}
                    }

	 			}

			}
		}
		

		else if (ulOperTriggerIf == ATG_DCI_OPER_DEL)
		{
			
			/* 计算第一个entry的地址。*/
			pucTriggerIfDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetAssociateIf, 
												&pstSetAssociateIf->trigger_if_data);	
			
			/* 首地址为NULL，异常 */
			if(pucTriggerIfDataStart == NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "	pucTriggerIfDataStart is NULL."));	
				NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
			}
			
			else
			{
			    if (associate_if_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  2）trigger_if接口配置[1-8] (trigger_if_num=%d) DEL\n", pstSetAssociateIf->trigger_if_num);
                    }

                    OS_SPRINTF(ucMessage, "  2）trigger_if接口配置[1-8] (trigger_if_num=%d) DEL\n", pstSetAssociateIf->trigger_if_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
						
				for (i = 0; i < pstSetAssociateIf->trigger_if_num; i++)
				{
					trigger_if_cfg_cb[i] = (ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *)
					(pucTriggerIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF))) * i); 

                    if (associate_if_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_associate_trigger_cfg(trigger_if_cfg_cb[i], i);
                    }

                    //spm_dbg_record_associate_trigger_cfg(trigger_if_cfg_cb[i], i);
                    
								
					ulPortIndex = trigger_if_cfg_cb[i]->interface_index;
							
					ret = spm_get_portid_from_logical_port_index(ulPortIndex,&stPortInfo NBB_CCXT);

					if(ret !=SUCCESS)
					{
						pstSetAssociateIf->trigger_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
						pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                	
                   		NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: 逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret));
		
						OS_PRINTF("***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
		
						OS_SPRINTF(ucMessage, "***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
						BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
		
						NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "spm_get_portid_from_logical_port_index", ucMessage));

					}

                    else
                    {
		
					    if((stPortInfo.port_type == ATG_DCI_SUB_INTFACE)||(stPortInfo.port_type == ATG_DCI_INTFACE))
    					{
    						usPortId = stPortInfo.port_id;
    						usSlotId = stPortInfo.slot_id;
    								
    								
    						usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    								
    						ret = aps_del_monitor_group_port(ucAssociateIfKey,usSlotPort);

    						if(ret == SUCCESS)
    						{
    							for(j = 0;j < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ;j++)
    							{
    								if((pstAssociateIfCb->trigger_if_cfg_cb[j] != NULL)&&(pstAssociateIfCb->trigger_if_cfg_cb[j]->interface_index == trigger_if_cfg_cb[i]->interface_index))
    								{
    									NBB_MM_FREE(pstAssociateIfCb->trigger_if_cfg_cb[j], MEM_SPM_ASSOCIATE_TRIGGER_IF_CB);

                                   		pstAssociateIfCb->trigger_if_cfg_cb[j] = NULL;
                                        /*删除后还原写逻辑接口是否存在标志位*/
                                        pstAssociateIfCb->trigger_if_exist[j] = UNEXIST;
    									break;

    								}
    								
    							}
    		
    							pstAssociateIfCb->trigger_if_num--;
    										
    						}
    					}
		
								
		
					    if(stPortInfo.port_type == ATG_DCI_LAG)
    					{
    						ret = SUCCESS;
                            
                            usPortId = stPortInfo.port_id;
    						ret = spm_get_lag_all_ports_info(usPortId - ATG_DCI_LAG_OFFSET,stTriggerLagPortInfo,&pucPortNum NBB_CCXT);

    						if(pucPortNum == 0)
    						{
    							pstSetAssociateIf->trigger_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
    							pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    	
                       			NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: 逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret));
    		
    							OS_PRINTF("***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
    		
    							OS_SPRINTF(ucMessage, "***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
    							BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    		
    							NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "spm_get_lag_all_ports_info", ucMessage));
    				
    						}
    						else
    						{
    						
    							for(j = 0;j < pucPortNum;j++)
    							{
    								usPortId = stTriggerLagPortInfo[j].port_id;
    								usSlotId = stTriggerLagPortInfo[j].slot_id;
    								
    			
    								usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    							
    								ret += aps_del_monitor_group_port(ucAssociateIfKey,usSlotPort);
    							}
    							
    						
    							if(ret == SUCCESS)
    							{
    								for(j = 0;j < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM ;j++)
    								{
    									if((pstAssociateIfCb->trigger_if_cfg_cb[j] != NULL)&&(pstAssociateIfCb->trigger_if_cfg_cb[j]->interface_index == trigger_if_cfg_cb[i]->interface_index))
    									{
    										NBB_MM_FREE(pstAssociateIfCb->trigger_if_cfg_cb[j], MEM_SPM_ASSOCIATE_TRIGGER_IF_CB);

    	                               		pstAssociateIfCb->trigger_if_cfg_cb[j] = NULL;
                                            /*删除后还原逻辑接口是否存在标志位*/
                                            pstAssociateIfCb->trigger_if_exist[j] = UNEXIST;
    										break;
    	                             
    									}
    								 	
    							
    								}
    								pstAssociateIfCb->trigger_if_num--;
    			
    									
    							}
    						
    							
    							if(ret != SUCCESS)
    							{
    								pstSetAssociateIf->trigger_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
    									
    								NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: aps_del_monitor_group_port() ret=%d\n", ret));
    						
    								OS_PRINTF("***ERROR***:aps_del_monitor_group_port() ret=%d\n", ret);
    						
    								OS_SPRINTF(ucMessage, "***ERROR***:aps_del_monitor_group_port() ret=%d\n", ret);
    								BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    								NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "aps_del_monitor_group_port", ucMessage));
    						
    							}
    						}
    					
    					}
                    }
				}
			}
		}

        else if (ulOperTriggerIf == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }


		
		/******************************** 动作接口配置 *********************************/
				if (ulOperActIf== ATG_DCI_OPER_ADD)
				{
			
					/* 计算第一个entry的地址。*/
					pucActIfDataStart= (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetAssociateIf, 
																			 &pstSetAssociateIf->act_if_data);	
			
					/* 首地址为NULL，异常 */
					if(pucActIfDataStart == NULL)
					{
						NBB_TRC_FLOW((NBB_FORMAT "	pucActIfDataStart is NULL."));	
						NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
					}
			
					else
					{
					
					    if (associate_if_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            printf("  3）act_if接口配置[1-8] (act_if_num=%d) ADD\n", pstSetAssociateIf->act_if_num);
                        }

                        OS_SPRINTF(ucMessage, "  3）act_if接口配置[1-8] (act_if_num=%d) ADD\n", pstSetAssociateIf->act_if_num);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
						ret = SUCCESS;
						
						for (i=0; i < pstSetAssociateIf->act_if_num; i++)
						{
							act_if_cfg_cb[i] = (ATG_DCI_ASSOCIATE_IF_ACT_IF *)
							(pucActIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF))) * i); 

                            if (associate_if_cfg_print_setting == SPM_PRINT_CFG)
                            {
                                spm_dbg_print_associate_act_cfg(act_if_cfg_cb[i], i);
                            }

                            //spm_dbg_record_associate_act_cfg(act_if_cfg_cb[i], i);
								
							ulPortIndex = act_if_cfg_cb[i]->interface_index;
							
							ret = spm_get_portid_from_logical_port_index(ulPortIndex,&stPortInfo NBB_CCXT);

							if(ret !=SUCCESS)
							{
								pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
								pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                	
                   				NBB_TRC_DETAIL((NBB_FORMAT "   逻辑端口index=%ld不存在，只保存配置 \n",ulPortIndex));
		
								OS_PRINTF("   逻辑端口index=%ld不存在，只保存配置 \n",ulPortIndex);
		
								OS_SPRINTF(ucMessage, "   逻辑端口index=%ld不存在，只保存配置 \n",ulPortIndex);
								BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
		
								NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "spm_get_portid_from_logical_port_index", ucMessage));

                                 /*保存配置，下逻辑口时再加入关联组*/
                                ucFindCfgFlag = 0;
                                for(j = 0;j < ATG_DCI_ASSOCIATE_IF_ACT_NUM ;j++)
                                {
                                    if((pstAssociateIfCb->act_if_cfg_cb[j] != NULL)
                                    &&(pstAssociateIfCb->act_if_cfg_cb[j]->interface_index == act_if_cfg_cb[i]->interface_index))
                                    {
                                        pstAssociateIfCb->act_if_exist[j] = UNEXIST;
                                        ucFindCfgFlag = 1;
                                        break;  
                                    }
                                }

                                if(ucFindCfgFlag != 1)
                                {
                                 
                                    for(j = 0;j < ATG_DCI_ASSOCIATE_IF_ACT_NUM ;j++)
        							{
        								if(pstAssociateIfCb->act_if_cfg_cb[j] == NULL)
        								{
        									pstAssociateIfCb->act_if_cfg_cb[j] = (ATG_DCI_ASSOCIATE_IF_ACT_IF *)NBB_MM_ALLOC(sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF),
        																					NBB_NORETRY_ACT,
        																					MEM_SPM_ASSOCIATE_ACT_IF_CB);
                                            if(pstAssociateIfCb->act_if_cfg_cb[j] != NULL)
                                            {
                                                OS_MEMCPY(pstAssociateIfCb->act_if_cfg_cb[j],act_if_cfg_cb[i],sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF));
                                                
                                                /*写逻辑接口是否存在标志位，以便下逻辑接口时触发关联接口组配置*/
                                                pstAssociateIfCb->act_if_exist[j] = UNEXIST;
                                                break;  
                                            }
                                            if (pstAssociateIfCb->act_if_cfg_cb[j] == NULL)
                                    		{
                                    			pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                                    			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey));
                                    	
                                    			OS_PRINTF("***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                    	
                                    			OS_SPRINTF(ucMessage, "***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                    			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                    	
                                    			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));

                                                break;
                                    		}
        									
        								}
        							
        							}
                                }
        			
							}	
                            else
                            {
		
							    if((stPortInfo.port_type == ATG_DCI_SUB_INTFACE)||(stPortInfo.port_type == ATG_DCI_INTFACE))
    							{
    								usPortId = stPortInfo.port_id;
    								usSlotId = stPortInfo.slot_id;
    								
    								OS_MEMSET(&IfGoupInfo,0,sizeof(PORT_GROUP_INFO));
    		
    								IfGoupInfo.usSlotPort = (usSlotId << 8)+(usPortId&0xff);
                                    IfGoupInfo.cardId = stPortInfo.card_id;
    								IfGoupInfo.lacpState = 1;
    								IfGoupInfo.actionType = 1;
    								
    								ret = aps_add_monitor_group_port(ucAssociateIfKey,&IfGoupInfo);
    								if(ret == SUCCESS)
    								{
    								    ucFindCfgFlag = 0;
    								    /*重复下发*/
                                        for(j = 0;j < ATG_DCI_ASSOCIATE_IF_ACT_NUM ;j++)
                                        {
                                            if((pstAssociateIfCb->act_if_cfg_cb[j] != NULL)
                                                &&(pstAssociateIfCb->act_if_cfg_cb[j]->interface_index == act_if_cfg_cb[i]->interface_index))
                                            {
                                                pstAssociateIfCb->act_if_exist[j] = EXIST;
                                                ucFindCfgFlag = 1;

                                                break;
                                            }
                                        }

                                        /*不是重复下发*/
                                        if(ucFindCfgFlag != 1)
                                        {
                                            
    									    for(j = 0;j < ATG_DCI_ASSOCIATE_IF_ACT_NUM ;j++)
        									{
        										if(pstAssociateIfCb->act_if_cfg_cb[j] == NULL)
        										{
        											pstAssociateIfCb->act_if_cfg_cb[j] = (ATG_DCI_ASSOCIATE_IF_ACT_IF *)NBB_MM_ALLOC(sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF),
        																							NBB_NORETRY_ACT,
        																							MEM_SPM_ASSOCIATE_ACT_IF_CB);
                                                    if(pstAssociateIfCb->act_if_cfg_cb[j] != NULL)
                                                    {
                                                        OS_MEMCPY(pstAssociateIfCb->act_if_cfg_cb[j],act_if_cfg_cb[i],sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF));
                                                        /*逻辑接口已存在，已真正加入接口组*/
                                                        pstAssociateIfCb->act_if_exist[j] = EXIST;
                                                        pstAssociateIfCb->act_if_num++;

                                                        break;  
                                                    }
                                                    if (pstAssociateIfCb->act_if_cfg_cb[j] == NULL)
                                            		{
                                            			pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                                            			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey));
                                            	
                                            			OS_PRINTF("***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                            	
                                            			OS_SPRINTF(ucMessage, "***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                            			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                            	
                                            			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));

                                                        break;
                                            		}
        											
        										}
        									}
                                        }	
    								}
    							}
		
								
		
							    if(stPortInfo.port_type == ATG_DCI_LAG)
    							{
    							    
    								usPortId = stPortInfo.port_id;
    								spm_get_lag_all_ports_info(usPortId - ATG_DCI_LAG_OFFSET,stActLagPortInfo,&pucPortNum NBB_CCXT);

    								if(pucPortNum == 0)
    								{
    									pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
    									pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    	
                       					NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: 逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret));
    		
    									OS_PRINTF("***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
    		
    									OS_SPRINTF(ucMessage, "***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
    									BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    		
    									NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "spm_get_lag_all_ports_info", ucMessage));
    				
    								}
    								else
    								{

    									ret = SUCCESS;
    									
    									for(j = 0;j < pucPortNum;j++)
    									{
    										usPortId = stActLagPortInfo[j].port_id;
    										usSlotId = stActLagPortInfo[j].slot_id;
    									
    										OS_MEMSET(&IfGoupInfo,0,sizeof(PORT_GROUP_INFO));
    			
    										IfGoupInfo.usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    										IfGoupInfo.lacpState = stActLagPortInfo[j].port_consult;
    										IfGoupInfo.actionType = 1;
                                            IfGoupInfo.cardId = stActLagPortInfo[j].card_id;
    											
    										ret += aps_add_monitor_group_port(ucAssociateIfKey,&IfGoupInfo);
    					
    									}
    									
    									if(ret == SUCCESS)
        								{
        								    ucFindCfgFlag = 0;
        								    /*重复下发*/
                                            for(j = 0;j < ATG_DCI_ASSOCIATE_IF_ACT_NUM ;j++)
                                            {
                                                if((pstAssociateIfCb->act_if_cfg_cb[j] != NULL)
                                                    &&(pstAssociateIfCb->act_if_cfg_cb[j]->interface_index == act_if_cfg_cb[i]->interface_index))
                                                {
                                                    pstAssociateIfCb->act_if_exist[j] = EXIST;
                                                    ucFindCfgFlag = 1;

                                                    break;
                                                }
                                            }

                                            /*不是重复下发*/
                                            if(ucFindCfgFlag != 1)
                                            {
                                                
        									    for(j = 0;j < ATG_DCI_ASSOCIATE_IF_ACT_NUM ;j++)
            									{
            										if(pstAssociateIfCb->act_if_cfg_cb[j] == NULL)
            										{
            											pstAssociateIfCb->act_if_cfg_cb[j] = (ATG_DCI_ASSOCIATE_IF_ACT_IF *)NBB_MM_ALLOC(sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF),
            																							NBB_NORETRY_ACT,
            																							MEM_SPM_ASSOCIATE_ACT_IF_CB);
                                                        if(pstAssociateIfCb->act_if_cfg_cb[j] != NULL)
                                                        {
                                                            OS_MEMCPY(pstAssociateIfCb->act_if_cfg_cb[j],act_if_cfg_cb[i],sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF));
                                                            /*逻辑接口已存在，已真正加入接口组*/
                                                            pstAssociateIfCb->act_if_exist[j] = EXIST;
                                                            pstAssociateIfCb->act_if_num++;

                                                            break;  
                                                        }
                                                        if (pstAssociateIfCb->act_if_cfg_cb[j] == NULL)
                                                		{
                                                			pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                                                			NBB_TRC_FLOW((NBB_FORMAT "	***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey));
                                                	
                                                			OS_PRINTF("***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                                	
                                                			OS_SPRINTF(ucMessage, "***ERROR***:要增加的ucAssociateIfKey=%ld 的关联接口配置的动作接口配置未能成功申请内存!\n", ucAssociateIfKey);
                                                			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                                	
                                                			NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));

                                                            break;
                                                		}
            											
            										}
            									}
                                            }	
        								}
    			
    								
    									if(ret != SUCCESS)
    									{
    										pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
    																
    										NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: aps_add_monitor_group_port() ret=%d", ret));
    											
    										OS_PRINTF("***ERROR***:aps_add_monitor_group_port() ret=%d\n", ret);
    											
    										OS_SPRINTF(ucMessage, "***ERROR***:aps_add_monitor_group_port() ret=%d\n", ret);
    										BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    											
    										NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "aps_add_monitor_group_port", ucMessage));
    								
    									}
    								}
    							}
                            }
               
						}
			
					}
		
				}
				
		
			    else if (ulOperActIf == ATG_DCI_OPER_DEL)
				{
					
					/* 计算第一个entry的地址。*/
					pucActIfDataStart= (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetAssociateIf,  &pstSetAssociateIf->act_if_data);	
			
					/* 首地址为NULL，异常 */
					if(pucActIfDataStart == NULL)
					{
						NBB_TRC_FLOW((NBB_FORMAT "	pucActIfDataStart is NULL."));	
						NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
					}
					
					else
					{
					    if (associate_if_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            printf("  3）act_if接口配置[1-8] (act_if_num=%d) DEL\n", pstSetAssociateIf->act_if_num);
                        }

                        OS_SPRINTF(ucMessage, "  3）act_if接口配置[1-8] (act_if_num=%d) DEL\n", pstSetAssociateIf->act_if_num);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        ret = SUCCESS;
								
						for (i=0; i < pstSetAssociateIf->act_if_num; i++)
						{
							act_if_cfg_cb[i] = (ATG_DCI_ASSOCIATE_IF_ACT_IF *)
							(pucActIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF))) * i); 
                            
                            if (associate_if_cfg_print_setting == SPM_PRINT_CFG)
                            {
                                spm_dbg_print_associate_act_cfg(act_if_cfg_cb[i], i);
                            }

                            //spm_dbg_record_associate_act_cfg(act_if_cfg_cb[i], i);
										
							ulPortIndex = act_if_cfg_cb[i]->interface_index;
									
							ret = spm_get_portid_from_logical_port_index(ulPortIndex,&stPortInfo NBB_CCXT);

							if(ret !=SUCCESS)
							{
								pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
								pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                	
                   				NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: 逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret));
		
								OS_PRINTF("***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
		
								OS_SPRINTF(ucMessage, "***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
								BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
		
								NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "spm_get_portid_from_logical_port_index", ucMessage));
				
							}		
				
				            else
                            {            
							    if((stPortInfo.port_type == ATG_DCI_SUB_INTFACE)||(stPortInfo.port_type == ATG_DCI_INTFACE))
    							{
    								usPortId = stPortInfo.port_id;
    								usSlotId = stPortInfo.slot_id;
    										
    										
    								usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    										
    								ret = aps_del_monitor_group_port(ucAssociateIfKey,usSlotPort);

    								if(ret == SUCCESS)
    								{
    									for(j = 0;j < ATG_DCI_ASSOCIATE_IF_ACT_NUM  ;j++)
    									{
    										if((pstAssociateIfCb->act_if_cfg_cb[j] != NULL)&&(pstAssociateIfCb->act_if_cfg_cb[j]->interface_index == act_if_cfg_cb[i]->interface_index))
    										{
    											NBB_MM_FREE(pstAssociateIfCb->act_if_cfg_cb[j], MEM_SPM_ASSOCIATE_ACT_IF_CB);
    		
    											pstAssociateIfCb->act_if_cfg_cb[j] = NULL;
                                                /*删除后还原逻辑接口是否存在标志位*/
                                                pstAssociateIfCb->act_if_exist[j] = UNEXIST;

    											break;
    		
    										}
    									}
    				
    									pstAssociateIfCb->act_if_num--;
    												
    								}
    						}
				
										
				
						    if(stPortInfo.port_type == ATG_DCI_LAG)
    						{
    						    usPortId = stPortInfo.port_id;
    							spm_get_lag_all_ports_info(usPortId - ATG_DCI_LAG_OFFSET,stActLagPortInfo,&pucPortNum NBB_CCXT);
    							

    							if(pucPortNum == 0)
    							{
    								pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
    								pstSetAssociateIf->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    	
                       				NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: 逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret));
    		
    								OS_PRINTF("***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
    		
    								OS_SPRINTF(ucMessage, "***ERROR***:逻辑端口index=%ld获取物理口失败， ret=%d\n",ulPortIndex,ret);
    								BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    		
    								NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "spm_get_lag_all_ports_info", ucMessage));
    			
    							}
    							else
    							{
    							
    								for(j = 0;j < pucPortNum;j++)
    								{
    									usPortId = stActLagPortInfo[j].port_id;
    									usSlotId = stActLagPortInfo[j].slot_id;
    										
    					
    									usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    									
    													
    									ret += aps_del_monitor_group_port(ucAssociateIfKey,usSlotPort);
    													
    					
    								}
    								
    								
    								if(ret == SUCCESS)
    								{
    									for(j = 0;j < ATG_DCI_ASSOCIATE_IF_ACT_NUM;j++)
    									{
    										if((pstAssociateIfCb->act_if_cfg_cb[j] != NULL)&&(pstAssociateIfCb->act_if_cfg_cb[j]->interface_index == act_if_cfg_cb[j]->interface_index))
    										{
    											NBB_MM_FREE(pstAssociateIfCb->act_if_cfg_cb[j], MEM_SPM_ASSOCIATE_ACT_IF_CB);
    			
    											pstAssociateIfCb->act_if_cfg_cb[j] = NULL;
                                                /*删除后还原逻辑接口是否存在标志位*/
                                                pstAssociateIfCb->act_if_exist[j] = UNEXIST;
    			
    											break;
    										 
    										}
    									}
    					
    									pstAssociateIfCb->act_if_num--;
    													
    					
    								}
    							
    									
    								if(ret != SUCCESS)
    								{
    									pstSetAssociateIf->act_if_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
    											
    									NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: aps_del_monitor_group_port() ret=%d\n", ret));
    						
    									OS_PRINTF("***ERROR***:aps_del_monitor_group_port() ret=%d\n", ret);
    						
    									OS_SPRINTF(ucMessage, "***ERROR***:aps_del_monitor_group_port() ret=%d\n", ret);
    									BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    									NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ucAssociateIfKey, ret, "aps_del_monitor_group_port", ucMessage));
    							
    								}
    							}
    						}
                        }
							
					}
		 
		
				
				}
			}
                
            else if (ulOperActIf == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

	}

	EXIT_LABEL: NBB_TRC_EXIT();
	
 	return;
	
}

/*****************************************************************************
 函 数 名  : spm_init_associate_if_tree
 功能描述  : 初始化关联接口组配置存储树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月20日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_associate_if_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_associate_if_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_ASSOCIATE_IF_CB, key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.associate_if_tree, compare_byte,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_ASSOCIATE_IF_CB, spm_associate_if_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_alloc_associate_if_cb
 功能描述  : 申请关联接口配置的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  :SPM_ASSOCIATE_IF_CB
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月20日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
SPM_ASSOCIATE_IF_CB *spm_alloc_associate_if_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_ASSOCIATE_IF_CB *pstAssociateIfCb = NULL;

    NBB_TRC_ENTRY("spm_alloc_associate_if_cb");

    /* 分配一个新的VPWS配置条目。*/
    pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB *)NBB_MM_ALLOC(sizeof(SPM_ASSOCIATE_IF_CB), NBB_NORETRY_ACT, MEM_SPM_ASSOCIATE_IF_CB);
    
    if (pstAssociateIfCb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:申请关联接口条目空间为NULL!"));
        goto EXIT_LABEL;
    }

    /* 初始化VPWS配置条目 */
    OS_MEMSET(pstAssociateIfCb, 0, sizeof(SPM_ASSOCIATE_IF_CB));
    pstAssociateIfCb->basic_cfg_cb = NULL;

    for (i=0; i<ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM; i++)
    {
        pstAssociateIfCb->trigger_if_cfg_cb[i]= NULL;
        pstAssociateIfCb->trigger_if_exist[i] = 0;
    }
	 for (i=0; i<ATG_DCI_ASSOCIATE_IF_ACT_NUM; i++)
    {
        pstAssociateIfCb->act_if_cfg_cb[i]= NULL;
        pstAssociateIfCb->act_if_exist[i] = 0;
    }

	pstAssociateIfCb->act_if_num = 0;
	pstAssociateIfCb->trigger_if_num = 0;



    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pstAssociateIfCb->spm_associate_if_handle = NBB_CREATE_HANDLE(pstAssociateIfCb, HDL_SPM_ASSOCIATE_IF_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_ASSOCIATE_IF_CB allocated at %p with handle %#lx",
                   pstAssociateIfCb,pstAssociateIfCb->spm_associate_if_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstAssociateIfCb->spm_associate_if_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return (pstAssociateIfCb);
}

/*****************************************************************************
 函 数 名  : spm_free_lag_cb
 功能描述  : 释放关联接口内存空间
 输入参数  : SPM_ASSOCIATE_IF_CB *pstAssociateIfCb
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年11月20日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_associate_if_cb(SPM_ASSOCIATE_IF_CB *pstAssociateIfCb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_associate_if_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstAssociateIfCb, sizeof(SPM_ASSOCIATE_IF_CB), MEM_SPM_ASSOCIATE_IF_CB); 

    if (pstAssociateIfCb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "Free associate_if id = %d", pstAssociateIfCb->key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstAssociateIfCb->basic_cfg_cb!= NULL)
    {
        NBB_MM_FREE(pstAssociateIfCb->basic_cfg_cb, MEM_SPM_ASSOCIATE_BASIC_CB);  
       pstAssociateIfCb->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放LAG配置数据块。                                                */
    /***************************************************************************/
	for (i=0; i<ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM; i++)
    {
        if(pstAssociateIfCb->trigger_if_cfg_cb[i] != NULL)
        {
        	
			NBB_MM_FREE(pstAssociateIfCb->trigger_if_cfg_cb[i], MEM_SPM_ASSOCIATE_TRIGGER_IF_CB);
			pstAssociateIfCb->trigger_if_cfg_cb[i]= NULL;
        }
    }
	 for (i=0; i<ATG_DCI_ASSOCIATE_IF_ACT_NUM; i++)
    {
        if(pstAssociateIfCb->act_if_cfg_cb[i] != NULL)
        {
        	
			NBB_MM_FREE(pstAssociateIfCb->act_if_cfg_cb[i], MEM_SPM_ASSOCIATE_ACT_IF_CB);
			pstAssociateIfCb->act_if_cfg_cb[i]= NULL;
        }
    }

	pstAssociateIfCb->act_if_num = 0;
	pstAssociateIfCb->trigger_if_num = 0;

    
    /***************************************************************************/
       /* 删除控制块的句柄。                                                      */
       /***************************************************************************/
       NBB_DESTROY_HANDLE(pstAssociateIfCb->spm_associate_if_handle, HDL_SPM_ASSOCIATE_IF_CB);
    
       /***************************************************************************/
       /* 现在释放单盘信息控制块。                                                */
       /***************************************************************************/
       NBB_MM_FREE(pstAssociateIfCb, MEM_SPM_ASSOCIATE_IF_CB);  
       pstAssociateIfCb = NULL;


    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_associate_if_cb
   功能描述  : 释放链路聚合配置所有的内存空间
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

NBB_VOID spm_free_all_associate_if_cb(NBB_CXT_T NBB_CXT)
{
	SPM_ASSOCIATE_IF_CB *pstAssociateIfCb = NULL;
	
    for (pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB*) AVLL_FIRST(SHARED.associate_if_tree);
         pstAssociateIfCb != NULL;
         pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB*) AVLL_FIRST(SHARED.associate_if_tree))
    {
    	
		AVLL_DELETE(SHARED.associate_if_tree, pstAssociateIfCb->spm_associate_if_node);      	
    	spm_free_associate_if_cb(pstAssociateIfCb NBB_CCXT);
    }
}



		
