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
NBB_VOID spm_rcv_dci_set_vpls_uc(ATG_DCI_SET_VPLS_UC *pstSetVplsUc NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    
    SPM_VPLS_UC_CB *pstVplsUc = NULL;

    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_VPLS_UC_BASIC_DATA *pstBasicData = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucBasicDataStart = NULL;

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
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_vpls_uc(pstSetVplsUc==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_vpls_uc(pstSetVplsUc==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_vpls_uc(pstSetVplsUc==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_UC_PD, 0, "ld x x x x x x d s s", 0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        goto EXIT_LABEL;
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetVplsUc->return_code = ATG_DCI_RC_OK;

    OS_MEMCPY(&stVplsUcKey, &pstSetVplsUc->key, sizeof(ATG_DCI_VPLS_UC_KEY)); 
    pstVplsUc = AVLL_FIND(SHARED.vpls_uc_tree, &stVplsUcKey);
    
    if (pstVplsUc == NULL)    //条目不存在
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    
    
    /* 获取子配置的操作类型 */
    ulOperBasic = pstSetVplsUc->oper_basic;


    /* 删除整个条目 */
    if (pstSetVplsUc->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除VPLS_ID=%d  MAC_ADDRESS=%x-%x-%x-%x-%x-%x的 VPLS静态单播配置", stVplsUcKey.vpls_id,
                                                                                                      stVplsUcKey.mac_address[0],
                                                                                                      stVplsUcKey.mac_address[1],
                                                                                                      stVplsUcKey.mac_address[2],
                                                                                                      stVplsUcKey.mac_address[3],
                                                                                                      stVplsUcKey.mac_address[4],
                                                                                                      stVplsUcKey.mac_address[5]));        

        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            /* 待修改：加打印PD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在，退出"));

            OS_PRINTF("***ERROR***:要删除的VPLS_ID=%d 的 VPLS静态单播配置不存在!\n", stVplsUcKey.vpls_id);

            OS_SPRINTF(ucMessage, "***ERROR***:要删除的VPLS_ID=%d 的 VPLS静态单播配置不存在!\n", stVplsUcKey.vpls_id);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPLS_UC_PD, 0, "ld x x x x x x d s s", stVplsUcKey.vpls_id,
                                                                            stVplsUcKey.mac_address[0],
                                                                            stVplsUcKey.mac_address[1],
                                                                            stVplsUcKey.mac_address[2],
                                                                            stVplsUcKey.mac_address[3],
                                                                            stVplsUcKey.mac_address[4],
                                                                            stVplsUcKey.mac_address[5],
                                                                            DEL_UNEXIST_CONFIG_ERROR, 
                                                                            "DEL_UNEXIST_CONFIG_ERROR", 
                                                                            ucMessage));
            goto EXIT_LABEL;
        }
        /* 存在，删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  查询到此配置，删除此VPLS静态单播，并从tree中删除存储数据"));
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_DEL);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_DEL);

#ifdef SPU
			if (pstVplsUc == NULL)
			{
				goto EXIT_LABEL;
			}
			
            for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
            {
                ret = ApiC3DelStaticMac(ucC3Unit, 
                                        pstVplsUc->vpls_uc_key.mac_address, 
                                        pstVplsUc->vpls_uc_key.vpls_id);
                                
                NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelStaticMac() ret=%d", ret));
            }
#endif      
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
            NBB_TRC_DETAIL((NBB_FORMAT "  增加VPLS_ID=%d  MAC_ADDRESS=%x-%x-%x-%x-%x-%x 的 VPLS静态单播配置", stVplsUcKey.vpls_id,
                                                                                                      stVplsUcKey.mac_address[0],
                                                                                                      stVplsUcKey.mac_address[1],
                                                                                                      stVplsUcKey.mac_address[2],
                                                                                                      stVplsUcKey.mac_address[3],
                                                                                                      stVplsUcKey.mac_address[4],
                                                                                                      stVplsUcKey.mac_address[5]));
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_ADD);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_ADD);
            
            /* 申请一个新条目的内存空间 */
            pstVplsUc = spm_alloc_vpls_uc_cb(NBB_CXT);

			if (pstVplsUc != NULL)
			{
	            //插入到节点中
	            OS_MEMCPY(&pstVplsUc->vpls_uc_key, &stVplsUcKey, sizeof(ATG_DCI_VPLS_UC_KEY));

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.vpls_uc_tree, pstVplsUc->spm_vpls_uc_node);
			}
         
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  更新VPLS_ID=%d  MAC=%x-%x-%x-%x-%x-%x 的 VPLS静态单播配置", stVplsUcKey.vpls_id,
                                                                                                      stVplsUcKey.mac_address[0],
                                                                                                      stVplsUcKey.mac_address[1],
                                                                                                      stVplsUcKey.mac_address[2],
                                                                                                      stVplsUcKey.mac_address[3],
                                                                                                      stVplsUcKey.mac_address[4],
                                                                                                      stVplsUcKey.mac_address[5]));          
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_UPD);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_UPD);
        }

		//增加此判断，取消PC-Lint告警
        if (pstVplsUc == NULL)
        {
            /* 待修改：加打印PD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要增加的VPLS_ID=%d 的 VPLS静态单播配置未能成功申请内存!", stVplsUcKey.vpls_id));

            OS_PRINTF("***ERROR***:要增加的VPLS_ID=%d 的 VPLS静态单播配置未能成功申请内存!\n", stVplsUcKey.vpls_id);

            OS_SPRINTF(ucMessage, "***ERROR***:要增加的VPLS_ID=%d 的 VPLS静态单播配置未能成功申请内存!\n", stVplsUcKey.vpls_id);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPLS_UC_PD, 0, "ld x x x x x x d s s", stVplsUcKey.vpls_id,
                                                                            stVplsUcKey.mac_address[0],
                                                                            stVplsUcKey.mac_address[1],
                                                                            stVplsUcKey.mac_address[2],
                                                                            stVplsUcKey.mac_address[3],
                                                                            stVplsUcKey.mac_address[4],
                                                                            stVplsUcKey.mac_address[5],
                                                                            ALLOC_MEMORY_ERROR, 
                                                                            "ALLOC_MEMORY_ERROR", 
                                                                            ucMessage));
			pstSetVplsUc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
			
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                              获取配置                                   */
        /***************************************************************************/
        /******************************** 基本配置 *********************************/
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsUc, &pstSetVplsUc->basic_data);    

            /* 如果指针为NULL，无数据 */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucBasicDataStart is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));

                //如果为新增，且基本配置未下发，那么直接退出，不处理
                if (ucIfExist == ATG_DCI_UNEXIST)
                {
                    OS_PRINTF("***ERROR***:新增静态单播<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>，但基本配置未下发，直接退出，不处理!\n", stVplsUcKey.vpls_id,
                                                                                                      stVplsUcKey.mac_address[0],
                                                                                                      stVplsUcKey.mac_address[1],
                                                                                                      stVplsUcKey.mac_address[2],
                                                                                                      stVplsUcKey.mac_address[3],
                                                                                                      stVplsUcKey.mac_address[4],
                                                                                                      stVplsUcKey.mac_address[5]);

                    OS_SPRINTF(ucMessage, "***ERROR***:新增静态单播<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>，但基本配置未下发，直接退出，不处理!\n", stVplsUcKey.vpls_id,
                                                                                                      stVplsUcKey.mac_address[0],
                                                                                                      stVplsUcKey.mac_address[1],
                                                                                                      stVplsUcKey.mac_address[2],
                                                                                                      stVplsUcKey.mac_address[3],
                                                                                                      stVplsUcKey.mac_address[4],
                                                                                                      stVplsUcKey.mac_address[5]);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | VPLS_UC_PD, 0, "ld x x x x x x d s s", stVplsUcKey.vpls_id,
                                                                            stVplsUcKey.mac_address[0],
                                                                            stVplsUcKey.mac_address[1],
                                                                            stVplsUcKey.mac_address[2],
                                                                            stVplsUcKey.mac_address[3],
                                                                            stVplsUcKey.mac_address[4],
                                                                            stVplsUcKey.mac_address[5],
                                                                            BASIC_CONFIG_NOT_EXIST, 
                                                                            "BASIC_CONFIG_NOT_EXIST", 
                                                                            ucMessage));
                    pstSetVplsUc->return_code = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                    pstSetVplsUc->basic_return_code = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                    goto EXIT_LABEL;
                }
            }
            else
            {
                pstBasicData = (ATG_DCI_VPLS_UC_BASIC_DATA *)pucBasicDataStart;

                if (vpls_uc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）基本配置\n");
                    spm_dbg_print_vpls_uc_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1）基本配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_vpls_uc_basic_cfg(pstBasicData);

                /* 配置处理，todo */
                NBB_TRC_DETAIL((NBB_FORMAT "  出口属性 = %d", pstBasicData->out_attrib));   
                NBB_TRC_DETAIL((NBB_FORMAT "  VC ID = %ld", pstBasicData->vc_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP = %s", spm_set_ulong_to_ipv4(pstBasicData->peer_ip NBB_CCXT)));   
                NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE = %d", pstBasicData->vc_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  端口Index = %ld", pstBasicData->port_index));                

                ret = spm_set_uc_static_mac(&stVplsUcKey, pstBasicData NBB_CCXT);

                if (ret == SUCCESS)
                {
                    if (pstVplsUc->basic_cfg_cb == NULL)
                    {
                        pstVplsUc->basic_cfg_cb = (ATG_DCI_VPLS_UC_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_UC_BASIC_DATA),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_VPLS_UC_BASIC_CB);
                    }  

                    //保存数据
                    OS_MEMCPY(pstVplsUc->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_VPLS_UC_BASIC_DATA));
                }
                else
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:设置静态单播<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>失败", stVplsUcKey.vpls_id,
                                                                                                      stVplsUcKey.mac_address[0],
                                                                                                      stVplsUcKey.mac_address[1],
                                                                                                      stVplsUcKey.mac_address[2],
                                                                                                      stVplsUcKey.mac_address[3],
                                                                                                      stVplsUcKey.mac_address[4],
                                                                                                      stVplsUcKey.mac_address[5]));

                    OS_PRINTF("***ERROR***:设置静态单播<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>失败\n", stVplsUcKey.vpls_id,
                                                                                                  stVplsUcKey.mac_address[0],
                                                                                                  stVplsUcKey.mac_address[1],
                                                                                                  stVplsUcKey.mac_address[2],
                                                                                                  stVplsUcKey.mac_address[3],
                                                                                                  stVplsUcKey.mac_address[4],
                                                                                                  stVplsUcKey.mac_address[5]);

                    OS_SPRINTF(ucMessage, "***ERROR***:设置静态单播<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>失败\n", stVplsUcKey.vpls_id,
                                                                                                  stVplsUcKey.mac_address[0],
                                                                                                  stVplsUcKey.mac_address[1],
                                                                                                  stVplsUcKey.mac_address[2],
                                                                                                  stVplsUcKey.mac_address[3],
                                                                                                  stVplsUcKey.mac_address[4],
                                                                                                  stVplsUcKey.mac_address[5]);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | VPLS_UC_PD, 0, "ld x x x x x x d s s", stVplsUcKey.vpls_id,
                                                                            stVplsUcKey.mac_address[0],
                                                                            stVplsUcKey.mac_address[1],
                                                                            stVplsUcKey.mac_address[2],
                                                                            stVplsUcKey.mac_address[3],
                                                                            stVplsUcKey.mac_address[4],
                                                                            stVplsUcKey.mac_address[5],
                                                                            CALL_FUNC_ERROR, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    pstSetVplsUc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
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
        
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

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
NBB_INT spm_set_uc_static_mac(ATG_DCI_VPLS_UC_KEY *pstVplsUcKey,
                                                           ATG_DCI_VPLS_UC_BASIC_DATA *pstBasicData
                                                           NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_ULONG vp_idx = 0;
    NBB_ULONG ulPortIndex = 0;
    NBB_BYTE ucC3Unit = 0;
    ATG_DCI_VC_KEY stVcKey;

    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_VC_CB *pstVc = NULL;

    NBB_TRC_ENTRY("spm_set_vp_static_mac");

    if (pstBasicData == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_set_uc_static_mac(pstBasicData==NULL)"));

        OS_PRINTF("***ERROR***:spm_set_uc_static_mac(pstBasicData==NULL)\n");
        
        OS_SPRINTF(ucMessage, "***ERROR***:spm_set_uc_static_mac(pstBasicData==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", pstVplsUcKey->vpls_id,
                                                                pstVplsUcKey->mac_address[0],
                                                                pstVplsUcKey->mac_address[1],
                                                                pstVplsUcKey->mac_address[2],
                                                                pstVplsUcKey->mac_address[3],
                                                                pstVplsUcKey->mac_address[4],
                                                                pstVplsUcKey->mac_address[5],
                                                                FUNC_IN_PARAM_IS_NULL, 
                                                                "FUNC_IN_PARAM_IS_NULL", 
                                                                ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));

    if (pstBasicData->out_attrib == ATG_DCI_UNI)
    {
        ulPortIndex = pstBasicData->port_index;

        pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

        if (pstLogicalPort == NULL)
        {
            OS_PRINTF("***ERROR***:PORT_INDEX=%ld 的静态单播UNI不存在\n", ulPortIndex);

            OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld 的静态单播UNI不存在\n", ulPortIndex);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", pstVplsUcKey->vpls_id,
                                                                pstVplsUcKey->mac_address[0],
                                                                pstVplsUcKey->mac_address[1],
                                                                pstVplsUcKey->mac_address[2],
                                                                pstVplsUcKey->mac_address[3],
                                                                pstVplsUcKey->mac_address[4],
                                                                pstVplsUcKey->mac_address[5],
                                                                BASIC_CONFIG_NOT_EXIST, 
                                                                "BASIC_CONFIG_NOT_EXIST", 
                                                                ucMessage));
            ret = ERROR;
            goto EXIT_LABEL;
        }

        vp_idx = pstLogicalPort->logic_port_info_cb.vp_idx;
    }
    else if (pstBasicData->out_attrib == ATG_DCI_NNI)
    {
        stVcKey.vc_id = pstBasicData->vc_id;
        stVcKey.vc_type = pstBasicData->vc_type;
        stVcKey.peer_ip = pstBasicData->peer_ip;

        pstVc = AVLL_FIND(SHARED.vc_tree, &stVcKey);

        if (pstVc == NULL)
        {
            OS_PRINTF("***ERROR***:VC_ID=%ld VC_TYPE=%d PERR_IP=%s 的静态单播NNI不存在\n", stVcKey.vc_id, stVcKey.vc_type, spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));

            OS_SPRINTF(ucMessage, "***ERROR***:VC_ID=%ld VC_TYPE=%d PERR_IP=%s 的静态单播NNI不存在\n", stVcKey.vc_id, stVcKey.vc_type, spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", pstVplsUcKey->vpls_id,
                                                                pstVplsUcKey->mac_address[0],
                                                                pstVplsUcKey->mac_address[1],
                                                                pstVplsUcKey->mac_address[2],
                                                                pstVplsUcKey->mac_address[3],
                                                                pstVplsUcKey->mac_address[4],
                                                                pstVplsUcKey->mac_address[5],
                                                                BASIC_CONFIG_NOT_EXIST, 
                                                                "BASIC_CONFIG_NOT_EXIST", 
                                                                ucMessage));
            ret = ERROR;
            goto EXIT_LABEL;
        }

        vp_idx = pstVc->vc_info_cb.vp_idx;
    }

#ifdef SPU
    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret = ApiC3SetStaticMac(ucC3Unit, pstVplsUcKey->mac_address, pstVplsUcKey->vpls_id, 0, vp_idx);
        
        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3SetStaticMac() UC ret=%d", ret));
    }

#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
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
