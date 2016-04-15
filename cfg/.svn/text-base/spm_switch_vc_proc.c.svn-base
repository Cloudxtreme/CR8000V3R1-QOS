/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_switch_vc_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年3月26日
  最近修改   :
  功能描述   : MS-PW处理配置块
  函数列表   :
  修改历史   :
  1.日    期   : 2013年3月26日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char switch_vc_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_switch_vc
 功能描述  : 分配SWITCH VC表结构体的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月26日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_switch_vc(ATG_DCI_SET_SWITCH_VC *pstSetSwitchVc NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;

    SPM_SWITCH_VC_CB *pstSwitchVc = NULL;
    ATG_DCI_SWITCH_VC_VC_DATA *pstVc1Data = NULL;
    ATG_DCI_SWITCH_VC_VC_DATA *pstVc2Data = NULL;

    SPM_VPN_PORT_INFO_CB stVc1PortInfo;
    SPM_VPN_PORT_INFO_CB stVc2PortInfo;
    
    NBB_BYTE *pucVc1DataStart = NULL;
    NBB_BYTE *pucVc2DataStart = NULL;
    
    NBB_ULONG ulOperVc1       = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperVc2       = ATG_DCI_OPER_NULL;
    
    NBB_ULONG ulSwitchVcKey = 0;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    SPM_VC_CB *pstVcTbl1     = NULL;

    SPM_VC_CB *pstVcTbl2     = NULL;

    ATG_DCI_VC_KEY stVcKey1;

    ATG_DCI_VC_KEY stVcKey2;

    SPM_PORT_INFO_CB stportinfo1;
    SPM_PORT_INFO_CB stportinfo2;
    LSPPROT_KEY          st_lspprot_key1;  //LSPPROT  key值
    LSPPROT_KEY          st_lspprot_key2;  //LSPPROT  key值

    NBB_TRC_ENTRY("spm_rcv_dci_set_switch_vc");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetSwitchVc != NULL);

	if (pstSetSwitchVc == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_switch_vc(pstSetSwitchVc==NULL)"));
        
        OS_PRINTF("***ERROR***:spm_rcv_dci_set_switch_vc(pstSetSwitchVc==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_switch_vc(pstSetSwitchVc==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", 0, 
        		FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage));
  
        goto EXIT_LABEL;
    }
    
    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetSwitchVc->return_code = ATG_DCI_RC_OK;

    OS_MEMSET(&stVc1PortInfo, 0, sizeof(SPM_VPN_PORT_INFO_CB));
    OS_MEMSET(&stVc2PortInfo, 0, sizeof(SPM_VPN_PORT_INFO_CB));
    OS_MEMSET(&stVcKey1, 0, sizeof(ATG_DCI_VC_KEY));
    OS_MEMSET(&stVcKey2, 0, sizeof(ATG_DCI_VC_KEY));
    OS_MEMSET(&stportinfo1, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stportinfo2, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&st_lspprot_key1, 0, sizeof(LSPPROT_KEY));	
    OS_MEMSET(&st_lspprot_key2, 0, sizeof(LSPPROT_KEY));
	
    ulSwitchVcKey = pstSetSwitchVc->key;

    pstSwitchVc = AVLL_FIND(SHARED.switch_vc_tree, &ulSwitchVcKey);
    
    if (pstSwitchVc == NULL)       /* 如果条目不存在*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    

    /* 获取子配置操作 */
    ulOperVc1 = pstSetSwitchVc->oper_vc1;
    ulOperVc2 = pstSetSwitchVc->oper_vc2;

    
    /* 删除整个条目 */
    if (pstSetSwitchVc->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除ID=%d的 SWITCH VC表配置", ulSwitchVcKey));
        
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在"));
            
            OS_PRINTF("***ERROR***:要删除的 INDEX=%d的 SWITCH VC表配置不存在!\n", ulSwitchVcKey);

            OS_SPRINTF(ucMessage, "***ERROR***:要删除的 INDEX=%d的 SWITCH VC表配置不存在!\n", ulSwitchVcKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, DEL_UNEXIST_CONFIG_ERROR, "DEL_UNEXIST_CONFIG_ERROR", ucMessage));
            
            pstSetSwitchVc->return_code = ATG_DCI_RC_OK;
            
            goto EXIT_LABEL;
        }
        /* 存在，删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  查询到此配置，从tree中删除"));
            spm_dbg_print_switch_vc_head(ulSwitchVcKey, SPM_OPER_DEL);
            spm_dbg_record_switch_vc_head(ulSwitchVcKey, SPM_OPER_DEL);
            
#ifdef SPU 
            ret = SUCCESS;
            
            /* 删除VPWS */
            for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
            {
                ret += ApiC3DelMplsVpn(ucC3Unit, L2VPN_MSPW, ulSwitchVcKey);
            }

            if (ret == SUCCESS)
            {
    	        //调用dk接口，传vpid与tunnelid的对应关系
    	        if (pstSwitchVc->vc1_cfg_cb != NULL)
    	        {
			st_lspprot_key1.ingress  = pstSwitchVc->vc1_cfg_cb->ingress;
			st_lspprot_key1.egress   = pstSwitchVc->vc1_cfg_cb->egress;
			st_lspprot_key1.tunnelid = pstSwitchVc->vc1_cfg_cb->tnnl_id;                   
			
			//调用dk的接口获取ftnIndex
			if (ATG_DCI_FTN == pstSwitchVc->vc1_cfg_cb->lsp_select)
			{
				spm_l3_delvpldp(pstSwitchVc->vc1_cfg_cb->peer_ip, 
								pstSwitchVc->vc1_info_cb.vp_idx, 
								pstSwitchVc->vc1_info_cb.next_hop_id
								NBB_CCXT);		            
			}
			else if (ATG_DCI_CR_LSP == pstSwitchVc->vc1_cfg_cb->lsp_select)
			{
				spm_l3_delvprsvp(&st_lspprot_key1, 
								pstSwitchVc->vc1_info_cb.vp_idx, 
								pstSwitchVc->vc1_info_cb.next_hop_id
								NBB_CCXT);	
			}

    	        }

				//回收next_hop_id
    	        if (pstSwitchVc->vc1_info_cb.next_hop_id != 0)
    	        {
    	        	spm_free_vc_nhi_id(pstSwitchVc->vc1_info_cb.next_hop_id NBB_CCXT);
    	        }

    	        //调用dk接口，传vpid与tunnelid的对应关系
    	        if (pstSwitchVc->vc2_cfg_cb != NULL)
    	        {
			st_lspprot_key2.ingress  = pstSwitchVc->vc2_cfg_cb->ingress;
			st_lspprot_key2.egress   = pstSwitchVc->vc2_cfg_cb->egress;
			st_lspprot_key2.tunnelid = pstSwitchVc->vc2_cfg_cb->tnnl_id;                   
			
			//调用dk的接口获取ftnIndex
			if (ATG_DCI_FTN == pstSwitchVc->vc2_cfg_cb->lsp_select)
			{
				spm_l3_delvpldp(pstSwitchVc->vc2_cfg_cb->peer_ip, 
								pstSwitchVc->vc2_info_cb.vp_idx, 
								pstSwitchVc->vc2_info_cb.next_hop_id
								NBB_CCXT);		            
			}
			else if (ATG_DCI_CR_LSP == pstSwitchVc->vc2_cfg_cb->lsp_select)
			{
				spm_l3_delvprsvp(&st_lspprot_key2, 
								pstSwitchVc->vc2_info_cb.vp_idx, 
								pstSwitchVc->vc2_info_cb.next_hop_id
								NBB_CCXT);	
			}

    	        }

				//回收next_hop_id
    	        if (pstSwitchVc->vc2_info_cb.next_hop_id != 0)
    	        {
    	        	spm_free_vc_nhi_id(pstSwitchVc->vc2_info_cb.next_hop_id NBB_CCXT);
    	        }
    	        
            	if (pstSwitchVc != NULL)
            	{
                	OS_MEMSET(&pstSwitchVc->vc1_info_cb, 0, sizeof(SPM_VPN_PORT_INFO_CB));
                	OS_MEMSET(&pstSwitchVc->vc2_info_cb, 0, sizeof(SPM_VPN_PORT_INFO_CB));
				}
                
                if (pstSwitchVc->vc1_cfg_cb != NULL)
                {
                    stVcKey1.vc_id      = pstSwitchVc->vc1_cfg_cb->vc_id;
                    stVcKey1.vc_type    = pstSwitchVc->vc1_cfg_cb->vc_type;
                    stVcKey1.peer_ip    = pstSwitchVc->vc1_cfg_cb->peer_ip;

                    pstVcTbl1 = AVLL_FIND(SHARED.vc_tree, &stVcKey1);

                    if ( NULL != pstVcTbl1)       /* 如果条目存在*/
                    {
                        OS_MEMSET(&pstVcTbl1->vc_info_cb, 0, sizeof(SPM_VC_INFO_CB));
                    }
                }
               
                if (pstSwitchVc->vc2_cfg_cb != NULL)
                {
                    stVcKey2.vc_id      = pstSwitchVc->vc2_cfg_cb->vc_id;
                    stVcKey2.vc_type    = pstSwitchVc->vc2_cfg_cb->vc_type;
                    stVcKey2.peer_ip    = pstSwitchVc->vc2_cfg_cb->peer_ip;

                    pstVcTbl2 = AVLL_FIND(SHARED.vc_tree, &stVcKey2);

                    if ( NULL != pstVcTbl2)       /* 如果条目存在*/
                    {
                        OS_MEMSET(&pstVcTbl2->vc_info_cb, 0, sizeof(SPM_VC_INFO_CB));
                    }  
                }
                               
            }
            else
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除MS_PW(ID=%d)失败! >>> ApiC3DelMplsVpn() ret=%d", ulSwitchVcKey, ret));

                OS_PRINTF("***ERROR***:删除MS_PW(ID=%d)失败! >>> ApiC3DelMplsVpn() ret=%d\n", ulSwitchVcKey, ret);

                OS_SPRINTF(ucMessage, "***ERROR***:删除MS_PW(ID=%d)失败! >>> ApiC3DelMplsVpn() ret=%d\n", ulSwitchVcKey, ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, 
                                                             ret, 
                                                             "CALL_C3_FUNC_ERROR", 
                                                             ucMessage));
                                                             
                pstSetSwitchVc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                
                goto EXIT_LABEL;    
            }
#endif
			if (pstSwitchVc != NULL)
			{
	            AVLL_DELETE(SHARED.switch_vc_tree, pstSwitchVc->spm_switch_vc_node);  

	            //释放ARP表节点的内存空间
	            spm_free_switch_vc_cb(pstSwitchVc NBB_CCXT);
			}
        }
    }
    /* 增加或更新条目 */
    else
    {        
                
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  增加INDEX=%d的 SWITCH VC表配置", ulSwitchVcKey));
            spm_dbg_print_switch_vc_head(ulSwitchVcKey, SPM_OPER_ADD);
            spm_dbg_record_switch_vc_head(ulSwitchVcKey, SPM_OPER_ADD);

            /* 申请一个新条目的内存空间 */
            pstSwitchVc = spm_alloc_switch_vc_cb(NBB_CXT);

			if (pstSwitchVc != NULL)
			{
	            //插入到tree中
	            pstSwitchVc->switch_vc_key = ulSwitchVcKey;

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.switch_vc_tree, pstSwitchVc->spm_switch_vc_node);
			}

#ifdef SPU
            ret = SUCCESS;
            
            /* 创建VPWS */
            for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
            {
                ret += ApiC3CreateMplsVpn(ucC3Unit, L2VPN_MSPW, ulSwitchVcKey);
            }
            
            if (ret != SUCCESS)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:创建MS_PW(ID=%d)失败! >>> ApiC3CreateMplsVpn() ret=%d", ulSwitchVcKey, ret));

                OS_PRINTF("***ERROR***:创建MS_PW(ID=%d)失败! >>> ApiC3CreateMplsVpn() ret=%d\n", ulSwitchVcKey, ret);

                OS_SPRINTF(ucMessage, "***ERROR***:创建MS_PW(ID=%d)失败! >>> ApiC3CreateMplsVpn() ret=%d\n", ulSwitchVcKey, ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, 
                                                             ret, 
                                                             "CALL_C3_FUNC_ERROR", 
                                                             ucMessage));
                                                             
                pstSetSwitchVc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstSetSwitchVc->vc1_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstSetSwitchVc->vc2_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                
                goto EXIT_LABEL;                
            }
#endif 
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  更新INDEX=%d的 SWITCH VC表配置", ulSwitchVcKey));
            spm_dbg_print_switch_vc_head(ulSwitchVcKey, SPM_OPER_UPD);
            spm_dbg_record_switch_vc_head(ulSwitchVcKey, SPM_OPER_UPD);

        }

		//增加此判断，取消PC-Lint告警
        if (pstSwitchVc == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要增加的 INDEX=%d的 SWITCH VC表配置未能成功申请内存!", ulSwitchVcKey));
            
            OS_PRINTF("***ERROR***:要增加的 INDEX=%d的 SWITCH VC表配置未能成功申请内存!\n", ulSwitchVcKey);

            OS_SPRINTF(ucMessage, "***ERROR***:要增加的 INDEX=%d的 SWITCH VC表配置未能成功申请内存!\n", ulSwitchVcKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
            
            pstSetSwitchVc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/
        /******************************** 第一段VC配置 *********************************/
        if (ulOperVc1 == ATG_DCI_OPER_ADD)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  SWITCH VC 1 ADD")); 

            /* 计算第一个entry的地址。*/
            pucVc1DataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetSwitchVc, &pstSetSwitchVc->vc1_data);    

            /* 如果指针为NULL，无数据 */
            if (pucVc1DataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  SWITCH VC vc1_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstVc1Data = (ATG_DCI_SWITCH_VC_VC_DATA*)pucVc1DataStart;

                if (switch_vc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）第一段VC配置 ADD\n");
                    spm_dbg_print_switch_vc_basic_cfg(pstVc1Data);
                }

                OS_SPRINTF(ucMessage,"  1）第一段VC配置 ADD\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_switch_vc_basic_cfg(pstVc1Data);

                //打印配置
                NBB_TRC_DETAIL((NBB_FORMAT "第1段VC配置"));
                NBB_TRC_DETAIL((NBB_FORMAT "  VC ID=%d", pstVc1Data->vc_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP=%s", spm_set_ulong_to_ipv4(pstVc1Data->peer_ip NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE=%d", pstVc1Data->vc_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  LSP选择=%d", pstVc1Data->lsp_select));
                NBB_TRC_DETAIL((NBB_FORMAT "  下一跳IP地址=%x", pstVc1Data->nxhop_ip));
                NBB_TRC_DETAIL((NBB_FORMAT "  下一跳出端口Index=%d", pstVc1Data->nxhop_out_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  INGRESS=%s", spm_set_ulong_to_ipv4(pstVc1Data->ingress NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  EGRESS=%s", spm_set_ulong_to_ipv4(pstVc1Data->egress NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  TUNNEL_ID=%d", pstVc1Data->tnnl_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  出PW标签=%d", pstVc1Data->out_pw_label));
                NBB_TRC_DETAIL((NBB_FORMAT "  入PW标签=%d", pstVc1Data->in_pw_label));
                NBB_TRC_DETAIL((NBB_FORMAT "  NNI_NO=%d", pstVc1Data->nni_no));

#ifdef SPU
				stVc1PortInfo.next_hop_id = pstSwitchVc->vc1_info_cb.next_hop_id;
				
        		ret = spm_vpn_add_mspw(ulSwitchVcKey, pstVc1Data, &stVc1PortInfo NBB_CCXT);

        		if (ret == SUCCESS)
        		{
                    /* 保存数据 */
                    if (pstSwitchVc->vc1_cfg_cb == NULL)
                    {
                        pstSwitchVc->vc1_cfg_cb = (ATG_DCI_SWITCH_VC_VC_DATA*)NBB_MM_ALLOC(sizeof(ATG_DCI_SWITCH_VC_VC_DATA),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_SWITCH_VC_VC1_CB);
                    }  

                    pstSwitchVc->vc1_info_cb.vp_idx = stVc1PortInfo.vp_idx;
                    pstSwitchVc->vc1_info_cb.port_index = stVc1PortInfo.port_index;
                    pstSwitchVc->vc1_info_cb.next_hop_id = stVc1PortInfo.next_hop_id;
                    OS_MEMCPY(pstSwitchVc->vc1_cfg_cb, pstVc1Data, sizeof(ATG_DCI_SWITCH_VC_VC_DATA));
                    
                    stVcKey1.vc_id      = pstVc1Data->vc_id;
                    stVcKey1.vc_type    = pstVc1Data->vc_type;
                    stVcKey1.peer_ip    = pstVc1Data->peer_ip;

                    pstVcTbl1 = AVLL_FIND(SHARED.vc_tree, &stVcKey1);

                    if ( NULL != pstVcTbl1)       /* 如果条目存在*/
                    {
                        pstVcTbl1->vc_info_cb.vpn_type = L2VPN_MSPW;
                        pstVcTbl1->vc_info_cb.vp_idx = stVc1PortInfo.vp_idx;
                        pstVcTbl1->vc_info_cb.port_index = stVc1PortInfo.port_index;
                        pstVcTbl1->vc_info_cb.next_hop_id = stVc1PortInfo.next_hop_id;
                        pstVcTbl1->vc_info_cb.if_main = VC_MAIN;
                        
                        ret = spm_get_portid_from_logical_port_index(stVc1PortInfo.port_index, &stportinfo1 NBB_CCXT);
                        {
                            if(SUCCESS == ret)
                            {
                                pstVcTbl1->vc_info_cb.slot_id = stportinfo1.slot_id;
                                pstVcTbl1->vc_info_cb.port_id = stportinfo1.port_id;
                            }
                        }
                    }   
                    
        		}        		
        		else
        		{
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpn_add_mspw ret=%d", ret));

        		    OS_PRINTF("***ERROR***:spm_vpn_add_mspw() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:spm_vpn_add_mspw() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    pstSetSwitchVc->vc1_return_code = ATG_DCI_RC_UNSUCCESSFUL;

        		    //goto EXIT_LABEL;
        		}
#endif
                
            }                 
        }
        else if (ulOperVc1 == ATG_DCI_OPER_DEL)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  SWITCH VC 1 DEL")); 
            
            //如果此Key值的VPWS配置不存在，则无法删除VPWS内的端口
            if (ucIfExist == ATG_DCI_UNEXIST)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除SWITCH_VC内VC1端口时，SWITCH_VC=%ld 的业务不存在，无法处理!", ulSwitchVcKey));

                OS_PRINTF("***ERROR***:删除SWITCH_VC内VC1端口时，SWITCH_VC=%ld 的业务不存在，无法处理!", ulSwitchVcKey);

                OS_SPRINTF(ucMessage, "***ERROR***:删除SWITCH_VC内VC1端口时，SWITCH_VC=%ld 的业务不存在，无法处理!", ulSwitchVcKey);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, 
                                                             BASIC_CONFIG_NOT_EXIST, 
                                                             "BASIC_CONFIG_NOT_EXIST", 
                                                             ucMessage));
                                                             
                goto EXIT_LABEL;
            }

            /* 计算第一个entry的地址。*/
            pucVc1DataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetSwitchVc, &pstSetSwitchVc->vc1_data);    

            /* 如果指针为NULL，无数据 */
            if (pucVc1DataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  SWITCH VC vc1_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else
            {
                pstVc1Data = (ATG_DCI_SWITCH_VC_VC_DATA*)pucVc1DataStart;

                if (switch_vc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）第一段VC配置 DEL\n");
                    spm_dbg_print_switch_vc_basic_cfg(pstVc1Data);
                }

                OS_SPRINTF(ucMessage,"  1）第一段VC配置 DEL\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_switch_vc_basic_cfg(pstVc1Data);

                //打印配置
                NBB_TRC_DETAIL((NBB_FORMAT "第1段VC配置"));
                NBB_TRC_DETAIL((NBB_FORMAT "  VC ID=%d", pstVc1Data->vc_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP=%s", spm_set_ulong_to_ipv4(pstVc1Data->peer_ip NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE=%d", pstVc1Data->vc_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  LSP选择=%d", pstVc1Data->lsp_select));
                NBB_TRC_DETAIL((NBB_FORMAT "  下一跳IP地址=%x", pstVc1Data->nxhop_ip));
                NBB_TRC_DETAIL((NBB_FORMAT "  下一跳出端口Index=%d", pstVc1Data->nxhop_out_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  INGRESS=%s", spm_set_ulong_to_ipv4(pstVc1Data->ingress NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  EGRESS=%s", spm_set_ulong_to_ipv4(pstVc1Data->egress NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  TUNNEL_ID=%d", pstVc1Data->tnnl_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  出PW标签=%d", pstVc1Data->out_pw_label));
                NBB_TRC_DETAIL((NBB_FORMAT "  入PW标签=%d", pstVc1Data->in_pw_label));
                NBB_TRC_DETAIL((NBB_FORMAT "  NNI_NO=%d", pstVc1Data->nni_no));

                //如果第一段VC配置存在，可以删除第一段VC配置
                if (pstSwitchVc->vc1_info_cb.vp_idx != 0)
                {
#ifdef SPU
                    ret = SUCCESS;
                    
                    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
                    {
                        ret += ApiC3DelMplsPortFromVpn(ucC3Unit, L2VPN_MSPW, ulSwitchVcKey, pstSwitchVc->vc1_info_cb.vp_idx);
                    }

            		if (ret == SUCCESS)
            		{
				st_lspprot_key1.ingress  = pstVc1Data->ingress;
				st_lspprot_key1.egress   = pstVc1Data->egress;
				st_lspprot_key1.tunnelid = pstVc1Data->tnnl_id;                   
				
				//调用dk的接口获取ftnIndex
				if (ATG_DCI_FTN == pstVc1Data->lsp_select)
				{
					spm_l3_delvpldp(pstVc1Data->peer_ip, 
									pstSwitchVc->vc1_info_cb.vp_idx, 
									pstSwitchVc->vc1_info_cb.next_hop_id
									NBB_CCXT);		            
				}
				else if (ATG_DCI_CR_LSP == pstVc1Data->lsp_select)
				{
					spm_l3_delvprsvp(&st_lspprot_key1, 
									pstSwitchVc->vc1_info_cb.vp_idx, 
									pstSwitchVc->vc1_info_cb.next_hop_id
									NBB_CCXT);	
				}

						if (pstSwitchVc->vc1_info_cb.next_hop_id != 0)
						{
            		    	spm_free_vc_nhi_id(pstSwitchVc->vc1_info_cb.next_hop_id NBB_CCXT);
            		    }

            		    //free
                        NBB_MM_FREE(pstSwitchVc->vc1_cfg_cb, MEM_SPM_SWITCH_VC_VC1_CB);

                        pstSwitchVc->vc1_cfg_cb = NULL;
                        OS_MEMSET(&pstSwitchVc->vc1_info_cb, 0, sizeof(SPM_VPN_PORT_INFO_CB));
            		}        		
            		else
            		{
                        pstSetSwitchVc->vc1_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: <SWITCH_VC=%d> ApiC3DelMplsPortFromVpn() MSPW error! ret=%d", ulSwitchVcKey, ret));

                        OS_PRINTF("***ERROR***: <SWITCH_VC=%d> ApiC3DelMplsPortFromVpn() ret=%d", ulSwitchVcKey, ret);

                        OS_SPRINTF(ucMessage, "***ERROR***: <SWITCH_VC=%d> ApiC3DelMplsPortFromVpn() ret=%d", ulSwitchVcKey, ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, 
                                                             ret, 
                                                             "CALL_C3_FUNC_ERROR", 
                                                             ucMessage));
            		}
#endif 
                }
            }                 
        }
        else if (ulOperVc1 == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 第二段VC配置 *********************************/
        if (ulOperVc2 == ATG_DCI_OPER_ADD)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  SWITCH VC 2 ADD")); 

            /* 计算第一个entry的地址。*/
            pucVc2DataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetSwitchVc, &pstSetSwitchVc->vc2_data);    

            /* 如果指针为NULL，无数据 */
            if (pucVc2DataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  SWITCH VC vc2_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstVc2Data = (ATG_DCI_SWITCH_VC_VC_DATA*)pucVc2DataStart;

                if (switch_vc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2）第二段VC配置 ADD\n");
                    spm_dbg_print_switch_vc_basic_cfg(pstVc2Data);
                }

                OS_SPRINTF(ucMessage,"  2）第二段VC配置 ADD\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_switch_vc_basic_cfg(pstVc2Data);

                //打印配置
                NBB_TRC_DETAIL((NBB_FORMAT "第2段VC配置"));
                NBB_TRC_DETAIL((NBB_FORMAT "  VC ID=%d", pstVc2Data->vc_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP=%s", spm_set_ulong_to_ipv4(pstVc2Data->peer_ip NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE=%d", pstVc2Data->vc_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  LSP选择=%d", pstVc2Data->lsp_select));
                NBB_TRC_DETAIL((NBB_FORMAT "  下一跳IP地址=%x", pstVc2Data->nxhop_ip));
                NBB_TRC_DETAIL((NBB_FORMAT "  下一跳出端口Index=%d", pstVc2Data->nxhop_out_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  INGRESS=%s", spm_set_ulong_to_ipv4(pstVc2Data->ingress NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  EGRESS=%s", spm_set_ulong_to_ipv4(pstVc2Data->egress NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  TUNNEL_ID=%d", pstVc2Data->tnnl_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  出PW标签=%d", pstVc2Data->out_pw_label));
                NBB_TRC_DETAIL((NBB_FORMAT "  入PW标签=%d", pstVc2Data->in_pw_label));
                NBB_TRC_DETAIL((NBB_FORMAT "  NNI_NO=%d", pstVc2Data->nni_no));

#ifdef SPU
				stVc2PortInfo.next_hop_id = pstSwitchVc->vc2_info_cb.next_hop_id;
				
        		ret = spm_vpn_add_mspw(ulSwitchVcKey, pstVc2Data, &stVc2PortInfo NBB_CCXT);

        		if (ret == SUCCESS)
        		{                
                    /* 保存数据 */
                    if (pstSwitchVc->vc2_cfg_cb == NULL)
                    {
                        pstSwitchVc->vc2_cfg_cb = (ATG_DCI_SWITCH_VC_VC_DATA*)NBB_MM_ALLOC(sizeof(ATG_DCI_SWITCH_VC_VC_DATA),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_SWITCH_VC_VC2_CB);
                    }  

                    pstSwitchVc->vc2_info_cb.vp_idx = stVc2PortInfo.vp_idx;
                    pstSwitchVc->vc2_info_cb.port_index = stVc2PortInfo.port_index;
                    pstSwitchVc->vc2_info_cb.next_hop_id = stVc2PortInfo.next_hop_id;
                    OS_MEMCPY(pstSwitchVc->vc2_cfg_cb, pstVc2Data, sizeof(ATG_DCI_SWITCH_VC_VC_DATA));
                    
                    stVcKey2.vc_id      = pstVc2Data->vc_id;
                    stVcKey2.vc_type    = pstVc2Data->vc_type;
                    stVcKey2.peer_ip    = pstVc2Data->peer_ip;

                    pstVcTbl2 = AVLL_FIND(SHARED.vc_tree, &stVcKey2);

                    if ( NULL != pstVcTbl2)       /* 如果条目存在*/
                    {
                        pstVcTbl2->vc_info_cb.vpn_type = L2VPN_MSPW;
                        pstVcTbl2->vc_info_cb.vp_idx = stVc2PortInfo.vp_idx;
                        pstVcTbl2->vc_info_cb.port_index = stVc2PortInfo.port_index;
                        pstVcTbl2->vc_info_cb.next_hop_id = stVc2PortInfo.next_hop_id;
                        pstVcTbl2->vc_info_cb.if_main = VC_MAIN;
                        
                        ret = spm_get_portid_from_logical_port_index(stVc2PortInfo.port_index, &stportinfo2 NBB_CCXT);
                        {
                            if(SUCCESS == ret)
                            {
                                pstVcTbl2->vc_info_cb.slot_id = stportinfo2.slot_id;
                                pstVcTbl2->vc_info_cb.port_id = stportinfo2.port_id;
                            }
                        }
                    }  
                    
        		}
        		else
        		{
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpn_add_mspw ret=%d", ret));

        		    OS_PRINTF("***ERROR***:spm_vpn_add_mspw() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:spm_vpn_add_mspw() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    pstSetSwitchVc->vc2_return_code = ATG_DCI_RC_UNSUCCESSFUL;

        		    //goto EXIT_LABEL;
        		}
#endif

            }                 
        }
        else if (ulOperVc2 == ATG_DCI_OPER_DEL)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  SWITCH VC 2 DEL")); 
            
            //如果此Key值的VPWS配置不存在，则无法删除VPWS内的端口
            if (ucIfExist == ATG_DCI_UNEXIST)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除SWITCH_VC内VC2端口时，SWITCH_VC=%ld 的业务不存在，无法处理!", ulSwitchVcKey));

                OS_PRINTF("***ERROR***:删除SWITCH_VC内VC2端口时，SWITCH_VC=%ld 的业务不存在，无法处理!", ulSwitchVcKey);
                                                             
                OS_SPRINTF(ucMessage, "***ERROR***:删除SWITCH_VC内VC2端口时，SWITCH_VC=%ld 的业务不存在，无法处理!", ulSwitchVcKey);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, 
                                                             BASIC_CONFIG_NOT_EXIST, 
                                                             "BASIC_CONFIG_NOT_EXIST", 
                                                             ucMessage));
                                                             
                goto EXIT_LABEL;
            }

            /* 计算第一个entry的地址。*/
            pucVc2DataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetSwitchVc, &pstSetSwitchVc->vc2_data);    

            /* 如果指针为NULL，无数据 */
            if (pucVc2DataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  SWITCH VC vc2_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }
            else
            {
                pstVc2Data = (ATG_DCI_SWITCH_VC_VC_DATA*)pucVc2DataStart;

                if (switch_vc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2）第二段VC配置 DEL\n");
                    spm_dbg_print_switch_vc_basic_cfg(pstVc2Data);
                }

                OS_SPRINTF(ucMessage,"  2）第二段VC配置 DEL\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_switch_vc_basic_cfg(pstVc2Data);

                //打印配置
                NBB_TRC_DETAIL((NBB_FORMAT "第2段VC配置"));
                NBB_TRC_DETAIL((NBB_FORMAT "  VC ID=%d", pstVc2Data->vc_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP=%s", spm_set_ulong_to_ipv4(pstVc2Data->peer_ip NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE=%d", pstVc2Data->vc_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  LSP选择=%d", pstVc2Data->lsp_select));
                NBB_TRC_DETAIL((NBB_FORMAT "  下一跳IP地址=%x", pstVc2Data->nxhop_ip));
                NBB_TRC_DETAIL((NBB_FORMAT "  下一跳出端口Index=%d", pstVc2Data->nxhop_out_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  INGRESS=%s", spm_set_ulong_to_ipv4(pstVc2Data->ingress NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  EGRESS=%s", spm_set_ulong_to_ipv4(pstVc2Data->egress NBB_CCXT)));
                NBB_TRC_DETAIL((NBB_FORMAT "  TUNNEL_ID=%d", pstVc2Data->tnnl_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  出PW标签=%d", pstVc2Data->out_pw_label));
                NBB_TRC_DETAIL((NBB_FORMAT "  入PW标签=%d", pstVc2Data->in_pw_label));
                NBB_TRC_DETAIL((NBB_FORMAT "  NNI_NO=%d", pstVc2Data->nni_no));

                //如果第二段VC配置存在，可以删除第二段VC配置
                if (pstSwitchVc->vc2_info_cb.vp_idx != 0)
                {
#ifdef SPU
                    ret = SUCCESS;
                    
                    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
                    {
                        ret += ApiC3DelMplsPortFromVpn(ucC3Unit, L2VPN_MSPW, ulSwitchVcKey, pstSwitchVc->vc2_info_cb.vp_idx);
                    }

            		if (ret == SUCCESS)
            		{
				st_lspprot_key2.ingress  = pstVc2Data->ingress;
				st_lspprot_key2.egress   = pstVc2Data->egress;
				st_lspprot_key2.tunnelid = pstVc2Data->tnnl_id;                   
				
				//调用dk的接口获取ftnIndex
				if (ATG_DCI_FTN == pstVc2Data->lsp_select)
				{
					spm_l3_delvpldp(pstVc2Data->peer_ip, 
									pstSwitchVc->vc2_info_cb.vp_idx, 
									pstSwitchVc->vc2_info_cb.next_hop_id
									NBB_CCXT);		            
				}
				else if (ATG_DCI_CR_LSP == pstVc2Data->lsp_select)
				{
					spm_l3_delvprsvp(&st_lspprot_key2, 
									pstSwitchVc->vc2_info_cb.vp_idx, 
									pstSwitchVc->vc2_info_cb.next_hop_id
									NBB_CCXT);	
				}

						if (pstSwitchVc->vc2_info_cb.next_hop_id != 0)
						{
            		    	spm_free_vc_nhi_id(pstSwitchVc->vc2_info_cb.next_hop_id NBB_CCXT);
            		    }
            		    				
            		    //free
                        NBB_MM_FREE(pstSwitchVc->vc2_cfg_cb, MEM_SPM_SWITCH_VC_VC2_CB);

                        pstSwitchVc->vc2_cfg_cb = NULL;
                        OS_MEMSET(&pstSwitchVc->vc2_info_cb, 0, sizeof(SPM_VPN_PORT_INFO_CB));
                        
            		}        		
            		else
            		{
                        pstSetSwitchVc->vc2_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: <SWITCH_VC=%d> ApiC3DelMplsPortFromVpn() MSPW error! ret=%d", ulSwitchVcKey, ret));

                        OS_PRINTF("***ERROR***: <SWITCH_VC=%d> ApiC3DelMplsPortFromVpn() ret=%d", ulSwitchVcKey, ret);

                        OS_SPRINTF(ucMessage, "***ERROR***: <SWITCH_VC=%d> ApiC3DelMplsPortFromVpn() ret=%d", ulSwitchVcKey, ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", ulSwitchVcKey, 
                                                             ret, 
                                                             "CALL_C3_FUNC_ERROR", 
                                                             ucMessage));
            		}
#endif 
                }
            }                 
        }
        else if (ulOperVc2 == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }
    
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_vpn_add_mspw
 功能描述  : 添加mspw业务
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年3月26日
    作    者   : xiaoxiang
    修改内容   : 新生成函数
  修改历史      :
  1.日    期   : 2015年1月26日
    作    者   : fansongbo
    修改内容   : 与VC配置进行关联，以便在mspw业务上绑定QOS
   修改历史      :
  1.日    期   : 2015年2月4日
    作    者   : fansongbo
    修改内容   : 根据关联的VC配置在MSPW业务上配置控制字使能  

*****************************************************************************/
NBB_INT spm_vpn_add_mspw(NBB_USHORT usVpnId,
                                  ATG_DCI_SWITCH_VC_VC_DATA *pstVcData,
                                  SPM_VPN_PORT_INFO_CB *pstVcPortInfo
                                  NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    VPORT_T              stNniVport;
    NBB_UINT             iNhiId =0;
    NBB_INT              ret = SUCCESS;
    NBB_BYTE             ucC3Unit = 0;
    SPM_VC_CB            *pstVcMain = NULL;
    SPM_VC_CB            *pstVcBack = NULL;
    ATG_DCI_VC_KEY       stVcKeyMain;
    ATG_DCI_VC_KEY       stVcKeyBack;
    CRTXLSP_KEY          stCrLspKey;     //最新的用这个接口
    LSPPROT_KEY          st_lspprot_key;  //LSPPROT  key值
    FTN_KEY              stFtnKey;
    SPM_VP_INFO_REFRESH_CB stVpInfoRefresh;

    NBB_ULONG            ulTunnelIndexMaster = 0;
    NBB_ULONG            ulTunnelIndexSlave = 0;
    NBB_ULONG            ulPortIndexMaster = 0;
    NBB_ULONG            ulPortIndexSlave = 0;
    NBB_UINT 			 iVpId = 0;
    NBB_BYTE             ucProtectFlag = 0;
    NBB_BYTE             ucProtectType = 0;
    NBB_USHORT           usFrrGroupId = 0;

    ATG_DCI_VPWS_NNI_DATA *pstVpwsNni = NULL;
    ATG_DCI_VPLS_NNI_DATA *pstVplsNni = NULL;

    SPM_LINEAR_PROTECT_SWITCH stLps;
    SPM_PORT_INFO_CB stPortInfoMaster;
    SPM_VC_CB *pstVcTbl     = NULL;
    ATG_DCI_VC_KEY stVcKey;

    NBB_TRC_ENTRY("spm_vpn_add_mspw");

    if (pstVcData == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpn_add_mspw(pstVcData==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpn_add_mspw(pstVcData==NULL)\n");
                                             
        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpn_add_mspw(pstVcData==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", usVpnId, 
                                             FUNC_IN_PARAM_IS_NULL, 
                                             "FUNC_IN_PARAM_IS_NULL", 
                                             ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    iVpId = pstVcData->nni_no;

    if (iVpId == 0)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: VPN_ID=%d ucVpnType=%d iVpId=%d ADD MSPW", usVpnId, L2VPN_MSPW, iVpId));

        OS_PRINTF("***ERROR***: VPN_ID=%d ucVpnType=%d iVpId=%d ADD MSPW\n", usVpnId, L2VPN_MSPW, iVpId);

        OS_SPRINTF(ucMessage, "***ERROR***: VPN_ID=%d ucVpnType=%d iVpId=%d ADD MSPW\n", usVpnId, L2VPN_MSPW, iVpId);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", usVpnId, 
                                             BASIC_CONFIG_NOT_EXIST, 
                                             "BASIC_CONFIG_NOT_EXIST", 
                                             ucMessage));
                
        ret = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stNniVport, 0, sizeof(VPORT_T));
    OS_MEMSET(&stVcKeyMain, 0, sizeof(ATG_DCI_VC_KEY));
    OS_MEMSET(&stVcKeyBack, 0, sizeof(ATG_DCI_VC_KEY));
    OS_MEMSET(&stCrLspKey, 0, sizeof(CRTXLSP_KEY));
    OS_MEMSET(&st_lspprot_key, 0, sizeof(LSPPROT_KEY));	
    OS_MEMSET(&stFtnKey, 0, sizeof(FTN_KEY));
    OS_MEMSET(&stLps, 0, sizeof(SPM_LINEAR_PROTECT_SWITCH));
    OS_MEMSET(&stPortInfoMaster, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stVpInfoRefresh, 0, sizeof(SPM_VP_INFO_REFRESH_CB));
    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));

    stVpInfoRefresh.if_main = VC_MAIN;
    stVpInfoRefresh.vpn_type = L2VPN_MSPW;
    stVpInfoRefresh.vpn_id = usVpnId;
    stVpInfoRefresh.vc_key.vc_id = pstVcData->vc_id;
    stVpInfoRefresh.vc_key.vc_type = pstVcData->vc_type;
    stVpInfoRefresh.vc_key.peer_ip = pstVcData->peer_ip;

    stNniVport.vpIdx = pstVcData->nni_no;
    stNniVport.eVpType = VP_NNI;

    //主用配置
    stNniVport.matchVcLabel = pstVcData->in_pw_label;
    stNniVport.tNextHop.flags |= L2_NEXT_HOP_ENCAP_TUNNEL;
    stNniVport.tNextHop.tPwLabel.label_un.tlabel.label  = pstVcData->out_pw_label;
    stNniVport.tNextHop.tPwLabel.label_un.tlabel.bottom = 1;
    stNniVport.tNextHop.tPwLabel.label_un.tlabel.ttl    = 255; //协议里面需要定义 todo

    stVcKey.vc_id      = pstVcData->vc_id;
    stVcKey.vc_type    = pstVcData->vc_type;
    stVcKey.peer_ip    = pstVcData->peer_ip;

    pstVcTbl = AVLL_FIND(SHARED.vc_tree, &stVcKey);

    if (( NULL != pstVcTbl) && (NULL != pstVcTbl->basic_cfg_cb))      /* 如果条目存在*/
    {
        if (pstVcTbl->basic_cfg_cb->control_word_enable == ENABLE)
        {
            stNniVport.flags |= VPORT_CW_SN;
            stNniVport.tNextHop.eCwMode = EGR_CW_SEQ;
        }
    } 

    if (pstVcPortInfo->next_hop_id != 0)
    {
    	stNniVport.tNextHop.nhIdx = pstVcPortInfo->next_hop_id;
    }
    else
    {
    	spm_alloc_vc_nhi_id(&iNhiId NBB_CCXT);
    	stNniVport.tNextHop.nhIdx = iNhiId;
    }

    //调用dk的接口获取ftnIndex
    if (pstVcData->lsp_select == ATG_DCI_FTN)
    {
        stFtnKey.fec = pstVcData->nxhop_ip;
        stFtnKey.mask = 32;
        stFtnKey.vrfid = 0;
        
        ret = spm_l3_ldpforoam(&stFtnKey, &ulPortIndexMaster, &ulTunnelIndexMaster NBB_CCXT);
        
        if (ret != SUCCESS)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_l3_ldpforoam() error! ret=%d", ret));

            OS_PRINTF("***ERROR***: spm_l3_ldpforoam() ret=%d\n", ret);
                                                     
            OS_SPRINTF(ucMessage, "***ERROR***: spm_l3_ldpforoam() ret=%d\n", ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", usVpnId, 
                                                     ret, 
                                                     "CALL_FUNC_ERROR", 
                                                     ucMessage));
            goto EXIT_LABEL;
        }
    }
    else if (pstVcData->lsp_select == ATG_DCI_CR_LSP)
    {
        stCrLspKey.ingress  = pstVcData->ingress;
        stCrLspKey.egress   = pstVcData->egress;
        stCrLspKey.tunnelid = pstVcData->tnnl_id;
        stCrLspKey.lspid    = 0;
        st_lspprot_key.ingress  = pstVcData->ingress;
        st_lspprot_key.egress   = pstVcData->egress;
        st_lspprot_key.tunnelid = pstVcData->tnnl_id;
			

        ret = spm_l3_rsvpforoam(&stCrLspKey, &ulPortIndexMaster, &ulTunnelIndexMaster NBB_CCXT);

        if (ret != SUCCESS)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_l3_rsvpforoam() error! ret=%d", ret));

            OS_PRINTF("***ERROR***: spm_l3_rsvpforoam() ret=%d\n", ret);
                                                     
            OS_SPRINTF(ucMessage, "***ERROR***: spm_l3_rsvpforoam() ret=%d\n", ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", usVpnId, 
                                                     ret, 
                                                     "CALL_FUNC_ERROR", 
                                                     ucMessage));
            goto EXIT_LABEL;
        }
    }
    else
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: VC LSP选择 未配置"));

        OS_PRINTF("***ERROR***: VC LSP选择 未配置\n");
                                                     
        OS_SPRINTF(ucMessage, "***ERROR***: VC LSP选择 未配置\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", usVpnId, 
                                                     BASIC_CONFIG_NOT_EXIST, 
                                                     "BASIC_CONFIG_NOT_EXIST", 
                                                     ucMessage));
        goto EXIT_LABEL;
    }

    NBB_TRC_DETAIL((NBB_FORMAT "  ulFtnIndexMaster=%d ulPortIndexMaster=%d", ulTunnelIndexMaster, ulPortIndexMaster));
    
    stNniVport.tNextHop.tunnelIdx = ulTunnelIndexMaster;    
    
    //设置包计数等标志
    //stNniVport.flags            |= VPORT_COUNTER_ON;
//    stNniVport.flags            |= VPORT_PROT_COUNTER_ON;

#ifdef SPU

    ret = SUCCESS;

    //调用API函数进行配置，将VP加入到VPN中
    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3AddMplsPortToVpn(ucC3Unit, L2VPN_MSPW, usVpnId, &stNniVport);
        NBB_TRC_DETAIL((NBB_FORMAT"  ApiAddMplsPortToVpn[MSPW]:  ret=%d", ret));
    }

    if (ret == SUCCESS)
    {
        pstVcPortInfo->vp_idx = stNniVport.vpIdx;
        pstVcPortInfo->next_hop_id = stNniVport.tNextHop.nhIdx;
        pstVcPortInfo->port_index = ulPortIndexMaster;

        if (pstVcData->lsp_select == ATG_DCI_FTN)
        {
            //调用dk接口，传vpid与tunnelid的对应关系
            spm_l3_addvpldp(&stVpInfoRefresh, stNniVport.vpIdx, 
                stNniVport.tNextHop.nhIdx, stNniVport.tNextHop.tunnelIdx NBB_CCXT); 
        }		
        else if (ATG_DCI_CR_LSP == pstVcData->lsp_select)
        {
            //20160104 调用dk接口，传vpid与tunnelid的对应关系
            spm_l3_addvprsvp(&stVpInfoRefresh, stNniVport.vpIdx, 
                stNniVport.tNextHop.nhIdx,&st_lspprot_key, 
                stNniVport.tNextHop.tunnelIdx NBB_CCXT); 
        }		
    }
    else
    {
        NBB_TRC_DETAIL((NBB_FORMAT"  ***ERROR***:ApiAddMplsPortToVpn[MSPW]:  ret=%d", ret));

        OS_PRINTF("***ERROR***:ApiAddMplsPortToVpn[MSPW]:  ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***:ApiAddMplsPortToVpn[MSPW]:  ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | SWITCH_VC_PD, 0, "ld d s s", usVpnId, 
                                                     ret, 
                                                     "CALL_C3_FUNC_ERROR", 
                                                     ucMessage));
		if (iNhiId != 0)
		{
        	spm_free_vc_nhi_id(iNhiId NBB_CCXT);
        }
        
        goto EXIT_LABEL;
    }
#endif


    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
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
  1.日    期   : 2013年3月26日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_SWITCH_VC_CB* spm_alloc_switch_vc_cb(NBB_CXT_T NBB_CXT)
{
    SPM_SWITCH_VC_CB *pstSwitchVc = NULL;

    NBB_TRC_ENTRY("spm_alloc_switch_vc_cb");

    /* 分配一个新的VC表配置条目。*/
    pstSwitchVc = (SPM_SWITCH_VC_CB *)NBB_MM_ALLOC(sizeof(SPM_SWITCH_VC_CB), NBB_NORETRY_ACT, MEM_SPM_SWITCH_VC_CB);
    if (pstSwitchVc == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VC表配置条目 */
    OS_MEMSET(pstSwitchVc, 0, sizeof(SPM_SWITCH_VC_CB));
    pstSwitchVc->vc1_cfg_cb = NULL;
    pstSwitchVc->vc2_cfg_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pstSwitchVc->spm_switch_vc_handle = NBB_CREATE_HANDLE(pstSwitchVc, HDL_SPM_SWITCH_VC_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_SWITCH_VC_CB allocated at %p with handle %#lx",
                   pstSwitchVc, pstSwitchVc->spm_switch_vc_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstSwitchVc->spm_switch_vc_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstSwitchVc);
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
  1.日    期   : 2013年3月26日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_switch_vc_cb(SPM_SWITCH_VC_CB *pstSwitchVc NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    //NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_switch_vc_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstSwitchVc, sizeof(SPM_SWITCH_VC_CB), MEM_SPM_SWITCH_VC_CB); 

    if (pstSwitchVc == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_SWITCH_VC_CB at %p", pstSwitchVc));  
    NBB_TRC_FLOW((NBB_FORMAT "Free ID %d", pstSwitchVc->switch_vc_key));

    /***************************************************************************/
    /* 释放第一段VC配置数据块。                                                */
    /***************************************************************************/
    if (pstSwitchVc->vc1_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstSwitchVc->vc1_cfg_cb, MEM_SPM_SWITCH_VC_VC1_CB);  
        pstSwitchVc->vc1_cfg_cb = NULL;
    }    

    /***************************************************************************/
    /* 释放第二段VC配置数据块。                                                */
    /***************************************************************************/
    if (pstSwitchVc->vc2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstSwitchVc->vc2_cfg_cb, MEM_SPM_SWITCH_VC_VC2_CB);  
        pstSwitchVc->vc2_cfg_cb = NULL;
    }    
    
    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstSwitchVc->spm_switch_vc_handle, HDL_SPM_SWITCH_VC_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstSwitchVc, MEM_SPM_SWITCH_VC_CB);  
    pstSwitchVc = NULL;

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
  1.日    期   : 2013年3月26日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_switch_vc_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_switch_vc_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_SWITCH_VC_CB, switch_vc_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.switch_vc_tree, compare_ulong,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_SWITCH_VC_CB, spm_switch_vc_node));

    NBB_TRC_EXIT();    
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


