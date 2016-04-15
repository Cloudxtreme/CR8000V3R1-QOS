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

#ifdef VPLS_MC_CFG

extern unsigned char vpls_mc_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_vpls_mc
 功能描述  : VPLS静态组播配置处理
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
NBB_VOID spm_rcv_dci_set_vpls_mc(ATG_DCI_SET_VPLS_MC *pstSetVplsMc NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_ULONG i = 0;
    NBB_ULONG j = 0;
    NBB_INT ret = SUCCESS;
    NBB_USHORT usMcId = 0;
    NBB_UINT iL2mcPosId = 0;
    NBB_UINT iVpId = 0;
    NBB_INT  iSlotNum = 0;
    NBB_BYTE ucSlot = 0;
    NBB_BYTE ucC3Unit = 0;
    
    NBB_USHORT usNniPosId[ATG_DCI_VPLS_MC_NNI_NUM];
    NBB_USHORT usUniPosId[ATG_DCI_VPLS_MC_UNI_NUM];
    
    SPM_VPLS_MC_CB *pstVplsMc = NULL;

    /* 用来保存从IPS中获取的子配置 */
    ATG_DCI_VPLS_MC_BASIC_DATA *pstBasicData = NULL;
    ATG_DCI_VPLS_MC_NNI_DATA *pstNniData[ATG_DCI_VPLS_MC_NNI_NUM];
    ATG_DCI_VPLS_MC_UNI_DATA *pstUniData[ATG_DCI_VPLS_MC_UNI_NUM];  

    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucNniDataStart   = NULL;
    NBB_BYTE *pucUniDataStart   = NULL;

    /* 子配置的操作模式 */
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperNni   = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperUni   = ATG_DCI_OPER_NULL;

    /* VPLS组播的键值 */
    ATG_DCI_VPLS_MC_KEY stVplsMcKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_vpls_mc");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetVplsMc != NULL);

	if (pstSetVplsMc == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_vpls_mc(pstSetVplsMc==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_vpls_mc(pstSetVplsMc==NULL)\n");
                                                                        
        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_vpls_mc(pstSetVplsMc==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", 0,
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
    pstSetVplsMc->return_code = ATG_DCI_RC_OK;

    //初始化NNI指针数组 
    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        pstNniData[i] = NULL;
        usNniPosId[i] = 0;
    }

    //初始化UNI指针数组 
    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        pstUniData[i] = NULL;
        usUniPosId[i] = 0;        
    }

    OS_MEMCPY(&stVplsMcKey, &pstSetVplsMc->key, sizeof(ATG_DCI_VPLS_MC_KEY));
    //stVplsMcKey = pstSetVplsMc->key;    
    pstVplsMc = AVLL_FIND(SHARED.vpls_mc_tree, &stVplsMcKey);
    
    if (pstVplsMc == NULL)    //条目不存在
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    
    
    /* 获取子配置的操作类型 */
    ulOperBasic = pstSetVplsMc->oper_basic;
    ulOperNni   = pstSetVplsMc->oper_nni_member;
    ulOperUni   = pstSetVplsMc->oper_uni_member;


    /* 删除整个条目 */
    if (pstSetVplsMc->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除VPLS_ID=%d  MAC_ADDRESS=%x-%x-%x-%x-%x-%x的 VPLS组播配置", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]));        

        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            /* 待修改：加打印PD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在，退出"));

            OS_PRINTF("***ERROR***:要删除的VPLS_ID=%ld 的 VPLS组播配置不存在!\n", stVplsMcKey.vpls_id);
                                                                            
            OS_SPRINTF(ucMessage, "***ERROR***:要删除的VPLS_ID=%ld 的 VPLS组播配置不存在!\n", stVplsMcKey.vpls_id);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            DEL_UNEXIST_CONFIG_ERROR, 
                                                                            "DEL_UNEXIST_CONFIG_ERROR", 
                                                                            ucMessage));
            goto EXIT_LABEL;
        }
        /* 存在，删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  查询到此配置，删除此VPLS组播，并从tree中删除存储数据"));
            spm_dbg_print_vpls_mc_head(&stVplsMcKey, SPM_OPER_DEL);
            spm_dbg_record_vpls_mc_head(&stVplsMcKey, SPM_OPER_DEL);

            //删除VPLS组播组，要求删除后下挂的组播端口由驱动自动删除
            //Arad删除组播组后自动删除下挂端口
            if ((pstVplsMc != NULL) && (pstVplsMc->basic_cfg_cb != NULL))
            {
#ifdef SPU			
	            ret = ApiAradDeleteMc(UNIT_0, pstVplsMc->basic_cfg_cb->mc_id);

	            if (ret != SUCCESS)
	            {
	                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiAradDeleteMc() ret=%d", stVplsMcKey.vpls_id, 
	                                                                                                                                      pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                                      ret));

	                OS_PRINTF("***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiAradDeleteMc() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                      pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                      ret);

	                OS_SPRINTF(ucMessage, "***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiAradDeleteMc() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                      pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                      ret);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
	                                                                            stVplsMcKey.mac_address[0],
	                                                                            stVplsMcKey.mac_address[1],
	                                                                            stVplsMcKey.mac_address[2],
	                                                                            stVplsMcKey.mac_address[3],
	                                                                            stVplsMcKey.mac_address[4],
	                                                                            stVplsMcKey.mac_address[5],
	                                                                            ret, 
	                                                                            "CALL_ARAD_FUNC_ERROR", 
	                                                                            ucMessage));
	                pstSetVplsMc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                goto EXIT_LABEL;
	            }

#if 0	//由于组播方案修改，现在可以不建组播出口intf
	            //C3里面的组播端口要手动删除
	            //释放给NNI端口分配的CUD
	            for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
	            {
	                //如果不等于0，证明分配了CUD
	                if (pstVplsMc->nni_info_cb[i].cud == 0)
	                    continue;

	                spm_free_vpls_mc_cud(pstVplsMc->nni_info_cb[i].cud);

	                for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
	                {
	                    ret = ApiC3DelL2MCOutIntf(ucC3Unit, pstVplsMc->nni_info_cb[i].vp_pos_id);

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelL2MCOutIntf() UNI ret=%d", ret));
	                }
	            }

	            //释放给UNI端口分配的CUD
	            for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
	            {
	                //如果不等于0，证明分配了CUD
	                if (pstVplsMc->uni_info_cb[i].cud == 0)
	                    continue;

	                spm_free_vpls_mc_cud(pstVplsMc->uni_info_cb[i].cud);

	                for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
	                {
	                    ret = ApiC3DelL2MCOutIntf(ucC3Unit, pstVplsMc->uni_info_cb[i].vp_pos_id);

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelL2MCOutIntf() NNI ret=%d", ret));
	                }
	            }
#endif
	            for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
	            {
	                ret = ApiC3DelStaticMac(ucC3Unit, 
	                                        pstVplsMc->vpls_mc_key.mac_address, 
	                                        pstVplsMc->vpls_mc_key.vpls_id);
	                                
	                NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelStaticMac() ret=%d", ret));
	            }

#endif

#ifdef SRC 
	            /* 删除VPLS组播组，要求删除后下挂的组播端口由驱动自动删除*/ 
	            ret = ApiDfeMulticastDestroy(UNIT_0, pstVplsMc->basic_cfg_cb->mc_id);

	            if (ret != SUCCESS)
	            {
	                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiDfeMulticastDestroy() ret=%d", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret));

	                OS_PRINTF("***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiDfeMulticastDestroy() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret);

	                OS_SPRINTF(ucMessage, "***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiDfeMulticastDestroy() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
	                                                                            stVplsMcKey.mac_address[0],
	                                                                            stVplsMcKey.mac_address[1],
	                                                                            stVplsMcKey.mac_address[2],
	                                                                            stVplsMcKey.mac_address[3],
	                                                                            stVplsMcKey.mac_address[4],
	                                                                            stVplsMcKey.mac_address[5],
	                                                                            ret, 
	                                                                            "CALL_ARAD_FUNC_ERROR", 
	                                                                            ucMessage));
	                pstSetVplsMc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                goto EXIT_LABEL;
	            }
#endif

#ifdef SNU
	            /* 删除VPLS组播组，要求删除后下挂的组播端口由驱动自动删除*/ 
	            ret = ApiDfeMulticastDestroy(UNIT_0, pstVplsMc->basic_cfg_cb->mc_id);

	            if (ret != SUCCESS)
	            {
	                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiDfeMulticastDestroy() ret=%d", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret));

	                OS_PRINTF("***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiDfeMulticastDestroy() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret);

	                OS_SPRINTF(ucMessage, "***ERROR***:删除VPLS_MC(VPLS_ID=%d MC_ID=%d)的组播组失败! >>> ApiDfeMulticastDestroy() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
	                                                                            stVplsMcKey.mac_address[0],
	                                                                            stVplsMcKey.mac_address[1],
	                                                                            stVplsMcKey.mac_address[2],
	                                                                            stVplsMcKey.mac_address[3],
	                                                                            stVplsMcKey.mac_address[4],
	                                                                            stVplsMcKey.mac_address[5],
	                                                                            ret, 
	                                                                            "CALL_ARAD_FUNC_ERROR", 
	                                                                            ucMessage));
	                pstSetVplsMc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                goto EXIT_LABEL;
	            }
#endif
            }

			if (pstVplsMc != NULL)
			{
	            //从树中删除节点
	            AVLL_DELETE(SHARED.vpls_mc_tree, pstVplsMc->spm_vpls_mc_node); 

	            //释放节点内存空间
	            spm_free_vpls_mc_cb(pstVplsMc NBB_CCXT);
			}
        }
    }
    /* 增加或更新条目 */
    else
    {        
        
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  增加VPLS_ID=%d  MAC_ADDRESS=%x-%x-%x-%x-%x-%x 的 VPLS组播配置", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]));
            spm_dbg_print_vpls_mc_head(&stVplsMcKey, SPM_OPER_ADD);
            spm_dbg_record_vpls_mc_head(&stVplsMcKey, SPM_OPER_ADD);
            
            /* 申请一个新条目的内存空间 */
            pstVplsMc = spm_alloc_vpls_mc_cb(NBB_CXT);

			if (pstVplsMc != NULL)
			{
	            //插入到节点中
	            OS_MEMCPY(&pstVplsMc->vpls_mc_key, &stVplsMcKey, sizeof(ATG_DCI_VPLS_MC_KEY));

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.vpls_mc_tree, pstVplsMc->spm_vpls_mc_node);
			}         
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  更新VPLS_ID=%d  MAC=%x-%x-%x-%x-%x-%x 的 VPLS组播配置", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]));          
            spm_dbg_print_vpls_mc_head(&stVplsMcKey, SPM_OPER_UPD);
            spm_dbg_record_vpls_mc_head(&stVplsMcKey, SPM_OPER_UPD);
        }

		//增加此判断，取消PC-Lint告警
        if (pstVplsMc == NULL)
        {
            /* 待修改：加打印PD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要增加的VPLS_ID=%ld 的 VPLS组播配置未能成功申请内存!", stVplsMcKey.vpls_id));

            OS_PRINTF("***ERROR***:要增加的VPLS_ID=%ld 的 VPLS组播配置未能成功申请内存!\n", stVplsMcKey.vpls_id);
                                                                            
            OS_SPRINTF(ucMessage, "***ERROR***:要增加的VPLS_ID=%ld 的 VPLS组播配置未能成功申请内存!\n", stVplsMcKey.vpls_id);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ALLOC_MEMORY_ERROR, 
                                                                            "ALLOC_MEMORY_ERROR", 
                                                                            ucMessage));
			pstSetVplsMc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
			
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                              获取配置                                   */
        /***************************************************************************/
        /******************************** 基本配置 *********************************/
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->basic_data);    

            /* 如果指针为NULL，无数据 */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucBasicDataStart is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));

                //如果为新增，且基本配置未下发，那么直接退出，不处理
                if (ucIfExist == ATG_DCI_UNEXIST)
                {
                    OS_PRINTF("***ERROR***:新增组播组<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>，但基本配置未下发，直接退出，不处理!\n", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]);

                    OS_SPRINTF(ucMessage, "***ERROR***:新增组播组<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>，但基本配置未下发，直接退出，不处理!\n", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            BASIC_CONFIG_NOT_EXIST, 
                                                                            "BASIC_CONFIG_NOT_EXIST", 
                                                                            ucMessage));
                                                                            
                    pstSetVplsMc->return_code = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                    pstSetVplsMc->basic_return_code = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;

                    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
                    {
                        pstSetVplsMc->nni_member_return_code[i] = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                    }

                    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
                    {
                        pstSetVplsMc->uni_member_return_code[i] = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                    }
                    
                    goto EXIT_LABEL;
                }
            }
            else
            {
                pstBasicData = (ATG_DCI_VPLS_MC_BASIC_DATA *)pucBasicDataStart;

                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）基本配置\n");
                    spm_dbg_print_vpls_mc_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1）基本配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_vpls_mc_basic_cfg(pstBasicData);

                /* 配置处理，todo */
                NBB_TRC_DETAIL((NBB_FORMAT "  MC_ID = %d", pstBasicData->mc_id));   
                NBB_TRC_DETAIL((NBB_FORMAT "  ucIfExist = %d", ucIfExist));

#ifdef SPU
                if (ucIfExist == ATG_DCI_UNEXIST)
                {

                    ret = ApiAradSetupEngressMc(UNIT_0, pstBasicData->mc_id);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiAradSetupEngressMc() ret=%d", ret));

                    if (ret != SUCCESS)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiAradSetupEngressMc() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiAradSetupEngressMc() ret=%d\n", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***:ApiAradSetupEngressMc() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                                                            
                        pstSetVplsMc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                        goto EXIT_LABEL;
                    }
                }

                ret = SUCCESS;
                
                for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
                {
                    //组播时，填mcid，vpIdx=0
                    ret += ApiC3SetStaticMac(ucC3Unit, 
                                            stVplsMcKey.mac_address, 
                                            stVplsMcKey.vpls_id, 
                                            pstBasicData->mc_id, 
                                            0);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3SetStaticMac() ret=%d", ret));


                }
                
                if (ret != SUCCESS)
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetStaticMac() error! ret=%d", ret));

                    OS_PRINTF("***ERROR***: ApiC3SetStaticMac() ret=%d\n", ret);

                    OS_SPRINTF(ucMessage, "***ERROR***: ApiC3SetStaticMac() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_C3_FUNC_ERROR", 
                                                                            ucMessage));
                                                                            
                    pstSetVplsMc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                }
#endif

#ifdef SRC
                if (ucIfExist == ATG_DCI_UNEXIST)
                {
                    ret = ApiDfeMulticastCreate(UNIT_0, pstBasicData->mc_id);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastCreate() ret=%d", ret));

                    if (ret != SUCCESS)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiDfeMulticastCreate() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiDfeMulticastCreate() ret=%d\n", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***:ApiDfeMulticastCreate() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                                                            
                        pstSetVplsMc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                        goto EXIT_LABEL;
                    }
                }                
#endif

#ifdef SNU
                if (ucIfExist == ATG_DCI_UNEXIST)
                {
                    ret = ApiDfeMulticastCreate(UNIT_0, pstBasicData->mc_id);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastCreate() ret=%d", ret));

                    if (ret != SUCCESS)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiDfeMulticastCreate() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiDfeMulticastCreate() ret=%d\n", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***:ApiDfeMulticastCreate() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                                                            
                        pstSetVplsMc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                        goto EXIT_LABEL;
                    }
                }   
#endif
                if (pstVplsMc->basic_cfg_cb == NULL)
                {
                    pstVplsMc->basic_cfg_cb = (ATG_DCI_VPLS_MC_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_MC_BASIC_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_VPLS_MC_BASIC_CB);
                }  

                //保存数据
                OS_MEMCPY(pstVplsMc->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_VPLS_MC_BASIC_DATA));

                usMcId = pstBasicData->mc_id;

                //pstBasicData->return_code = ATG_DCI_RC_OK;
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
        
        /******************************** NNI配置 *********************************/
        if (ulOperNni == ATG_DCI_OPER_ADD)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  NNI ADD")); 
                   
            /* 计算第一个entry的地址。*/
            pucNniDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->nni_member_data);    

            /* 如果指针为NULL，无数据 */
            if (pucNniDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucNniDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2）NNI接口配置[1-512] (nni_num=%d) ADD\n", pstSetVplsMc->nni_member_num);
                }

                OS_SPRINTF(ucMessage,"  2）NNI接口配置[1-512] (nni_num=%d) ADD\n", pstSetVplsMc->nni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                for (i=0; i<pstSetVplsMc->nni_member_num; i++)
                {
                    pstNniData[i] = (ATG_DCI_VPLS_MC_NNI_DATA *)
                                  (pucNniDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_MC_NNI_DATA))) * i); 

                    if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_mc_nni_cfg(pstNniData[i], i);
                    }

                    //spm_dbg_record_vpls_mc_nni_cfg(pstNniData[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC ID = %ld", pstNniData[i]->vc_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP = %s", spm_set_ulong_to_ipv4(pstNniData[i]->peer_ip NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE = %d", pstNniData[i]->vc_type));

                    iL2mcPosId = 0;
                    iVpId = 0;
                    ucSlot = 0;
                    
                    ret = spm_vpls_mc_add_vp_nni(pstVplsMc->basic_cfg_cb->mc_id, 
                                                 pstNniData[i], 
                                                 &iL2mcPosId, 
                                                 &iVpId, 
                                                 &ucSlot
                                                 NBB_CCXT);

                    //成功，保存数据
                    if (ret == SUCCESS)
                    {
                        //在NNI的数组中找一个存储位置
                        for (j=0; j<ATG_DCI_VPLS_MC_NNI_NUM; j++)
                        {
                            //为NULL，说明未占用，可以存储
                            if (pstVplsMc->nni_cfg_cb[j] == NULL)
                            {
                                pstVplsMc->nni_cfg_cb[j] = (ATG_DCI_VPLS_MC_NNI_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_MC_NNI_DATA),
                                                                        NBB_NORETRY_ACT,
                                                                        MEM_SPM_VPLS_MC_NNI_CB);

                                pstVplsMc->nni_info_cb[j].vp_pos_id = iVpId;
                                //pstVplsMc->nni_info_cb[j].cud = iMcCud;
                                pstVplsMc->nni_info_cb[j].slot_id = ucSlot;
                                pstVplsMc->nni_num++;
                                
                                OS_MEMCPY(pstVplsMc->nni_cfg_cb[j], pstNniData[i], sizeof(ATG_DCI_VPLS_MC_NNI_DATA));

                                break;
                            }
                        }
                    }
                    else    //如果增加NNI口失败记录错误状态
                    {
                        pstSetVplsMc->nni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_add_vp_nni() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:spm_vpls_mc_add_vp_nni() ret=%d\n", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_add_vp_nni() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    }
                }
            }                
        }
        else if (ulOperNni == ATG_DCI_OPER_DEL)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  NNI DEL"));

            //如果此Key值的VPLS组播配置不存在，则无法删除组播组内的端口
            if (ucIfExist == ATG_DCI_UNEXIST)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:单独删除组播组内端口时，组播Key VPLS_ID=%d 的组播组不存在，无法处理!", stVplsMcKey.vpls_id));

                OS_PRINTF("***ERROR***:单独删除组播组内端口时，组播Key VPLS_ID=%d 的组播组不存在，无法处理!\n", stVplsMcKey.vpls_id);
                                                                            
                OS_SPRINTF(ucMessage, "***ERROR***:单独删除组播组内端口时，组播Key VPLS_ID=%d 的组播组不存在，无法处理!\n", stVplsMcKey.vpls_id);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            BASIC_CONFIG_NOT_EXIST, 
                                                                            "BASIC_CONFIG_NOT_EXIST", 
                                                                            ucMessage));
                                                                            
                goto EXIT_LABEL;
            }
                   
            /* 计算第一个entry的地址。*/
            pucNniDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->nni_member_data);    

            /* 如果指针为NULL，无数据 */
            if (pucNniDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucNniDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2）NNI接口配置[1-512] (nni_num=%d) DEL\n", pstSetVplsMc->nni_member_num);
                }

                OS_SPRINTF(ucMessage,"  2）NNI接口配置[1-512] (nni_num=%d) DEL\n", pstSetVplsMc->nni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                for (i=0; i<pstSetVplsMc->nni_member_num; i++)
                {
                    pstNniData[i] = (ATG_DCI_VPLS_MC_NNI_DATA *)
                                  (pucNniDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_MC_NNI_DATA))) * i); 

                    if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_mc_nni_cfg(pstNniData[i], i);
                    }

                    //spm_dbg_record_vpls_mc_nni_cfg(pstNniData[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC ID = %ld", pstNniData[i]->vc_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP = %s", spm_set_ulong_to_ipv4(pstNniData[i]->peer_ip NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE = %d", pstNniData[i]->vc_type));


                    ret = spm_vpls_mc_del_vp_nni(pstVplsMc->basic_cfg_cb->mc_id, 
                                                 pstVplsMc, 
                                                 pstNniData[i]
                                                 NBB_CCXT);

                    //成功，删除数据
                    if (ret == SUCCESS)
                    {
                        //找到要删除的NNI端口
                        for (j=0; j<ATG_DCI_VPLS_MC_NNI_NUM; j++)
                        {
                            //key值相等，说明找到
                            if ((pstVplsMc->nni_cfg_cb[j]->vc_id   == pstNniData[i]->vc_id) 
                             && (pstVplsMc->nni_cfg_cb[j]->vc_type == pstNniData[i]->vc_type)
                             && (pstVplsMc->nni_cfg_cb[j]->peer_ip == pstNniData[i]->peer_ip))
                            {
                                ucSlot = pstVplsMc->nni_info_cb[j].slot_id;
                                iSlotNum = spm_get_vpls_mc_slot_num(pstVplsMc, ucSlot NBB_CCXT);
                                
                                //free
                                NBB_MM_FREE(pstVplsMc->nni_cfg_cb[j], MEM_SPM_VPLS_MC_NNI_CB);                                
                                
                                pstVplsMc->nni_cfg_cb[j] = NULL;
                                pstVplsMc->nni_num--;
                                
#ifdef SRC
                                if (iSlotNum == 0)
                                {
                                    //unit?
                                    ret = ApiDfeMulticastDelete(UNIT_0, 
                                                                pstVplsMc->basic_cfg_cb->mc_id, 
                                                                ucSlot);

                                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() ret=%d", ret));
                                    
                                    if (ret != SUCCESS)
                                    {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastDelete() error! ret=%d", ret));

                                        OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);

                                        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                        ret = ERROR;
                                        
                                        //goto EXIT_LABEL;
                                    }
                                }
#endif

#ifdef SNU
                                if (iSlotNum == 0)
                                {
                                    //unit?
                                    ret = ApiDfeMulticastDelete(UNIT_0, 
                                                                pstVplsMc->basic_cfg_cb->mc_id, 
                                                                ucSlot);

                                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() ret=%d", ret));
                                    
                                    if (ret != SUCCESS)
                                    {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastDelete() error! ret=%d", ret));

                                        OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);

                                        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                        ret = ERROR;
                                        
                                        //goto EXIT_LABEL;
                                    }
                                }
#endif
                                break;
                            }
                        }
                    }
                    else    //如果增加NNI口失败记录错误状态
                    {
                        pstSetVplsMc->nni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_del_vp_nni() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***: spm_vpls_mc_del_vp_nni() ret=%d", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_del_vp_nni() ret=%d", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    }
                }
            }                
        }
        else if (ulOperNni == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }  


        /******************************** UNI配置 *********************************/        
        if (ulOperUni == ATG_DCI_OPER_ADD)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  UNI ADD")); 

            /* 计算第一个entry的地址。*/
            pucUniDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->uni_member_data);    

            /* 如果指针为NULL，无数据 */
            if (pucUniDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucUniDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  3）UNI接口配置[1-64] (uni_num=%d) ADD\n", pstSetVplsMc->uni_member_num);
                }

                OS_SPRINTF(ucMessage,"  3）UNI接口配置[1-64] (uni_num=%d) ADD\n", pstSetVplsMc->uni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                for (i=0; i<pstSetVplsMc->uni_member_num; i++)
                {
                    pstUniData[i] = (ATG_DCI_VPLS_MC_UNI_DATA *)
                                  (pucUniDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_MC_UNI_DATA))) * i); 

                    if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_mc_uni_cfg(pstUniData[i], i);
                    }

                    //spm_dbg_record_vpls_mc_uni_cfg(pstUniData[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  端口Index = %ld", pstUniData[i]->port_index));

                    iL2mcPosId = 0;
                    iVpId = 0;
                    ucSlot = 0;
                    
                    ret = spm_vpls_mc_add_vp_uni(pstVplsMc->basic_cfg_cb->mc_id, 
                                                 pstUniData[i], 
                                                 &iL2mcPosId, 
                                                 &iVpId, 
                                                 &ucSlot
                                                 NBB_CCXT);
                
                    //如果成功，保存数据
                    if (ret == SUCCESS)
                    {
                        //在UNI的数组中找一个存储位置
                        for (j=0; j<ATG_DCI_VPLS_MC_UNI_NUM; j++)
                        {
                            //为NULL，说明未占用，可以存储
                            if (pstVplsMc->uni_cfg_cb[j] == NULL)
                            {
                                pstVplsMc->uni_cfg_cb[j] = (ATG_DCI_VPLS_MC_UNI_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_MC_UNI_DATA),
                                                                        NBB_NORETRY_ACT,
                                                                        MEM_SPM_VPLS_MC_UNI_CB);

                                pstVplsMc->uni_info_cb[j].vp_pos_id = iVpId;
                                //pstVplsMc->uni_info_cb[j].cud = iMcCud;
                                pstVplsMc->uni_info_cb[j].slot_id = ucSlot;
                                pstVplsMc->uni_num++;
                                
                                OS_MEMCPY(pstVplsMc->uni_cfg_cb[j], pstUniData[i], sizeof(ATG_DCI_VPLS_MC_UNI_DATA));

                                break;
                            }
                        }
                    }
                    else    //如果增加UNI口失败记录错误状态
                    {
                        pstSetVplsMc->uni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                        
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_add_vp_uni() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***: spm_vpls_mc_add_vp_uni() ret=%d", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_add_vp_uni() ret=%d", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    }
                }
            }             
        }
        else if (ulOperUni == ATG_DCI_OPER_DEL)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  UNI DEL"));

            //如果此Key值的VPLS组播配置不存在，则无法删除组播组内的端口
            if (ucIfExist == ATG_DCI_UNEXIST)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:单独删除组播组内端口时，组播Key VPLS_ID=%d 的组播组不存在，无法处理!", stVplsMcKey.vpls_id));

                OS_PRINTF("***ERROR***:单独删除组播组内端口时，组播Key VPLS_ID=%d 的组播组不存在，无法处理!\n", stVplsMcKey.vpls_id);

                OS_SPRINTF(ucMessage, "***ERROR***:单独删除组播组内端口时，组播Key VPLS_ID=%d 的组播组不存在，无法处理!\n", stVplsMcKey.vpls_id);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            BASIC_CONFIG_NOT_EXIST, 
                                                                            "BASIC_CONFIG_NOT_EXIST", 
                                                                            ucMessage));
                goto EXIT_LABEL;
            }
                   
            /* 计算第一个entry的地址。*/
            pucUniDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->uni_member_data);    

            /* 如果指针为NULL，无数据 */
            if (pucUniDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucUniDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  3）UNI接口配置[1-64] (uni_num=%d) DEL\n", pstSetVplsMc->uni_member_num);
                }

                OS_SPRINTF(ucMessage,"  3）UNI接口配置[1-64] (uni_num=%d) DEL\n", pstSetVplsMc->uni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                for (i=0; i<pstSetVplsMc->uni_member_num; i++)
                {
                    pstUniData[i] = (ATG_DCI_VPLS_MC_UNI_DATA *)
                                  (pucUniDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_MC_UNI_DATA))) * i); 

                    if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_mc_uni_cfg(pstUniData[i], i);
                    }

                    //spm_dbg_record_vpls_mc_uni_cfg(pstUniData[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  UNI PORT_INDEX = %ld", pstUniData[i]->port_index));

                    ret = spm_vpls_mc_del_vp_uni(pstVplsMc->basic_cfg_cb->mc_id, 
                                                 pstVplsMc, 
                                                 pstUniData[i]
                                                 NBB_CCXT);

                    //成功，删除数据
                    if (ret == SUCCESS)
                    {
                        //找到要删除的UNI接口
                        for (j=0; j<ATG_DCI_VPLS_MC_UNI_NUM; j++)
                        {
                            //找到
                            if (pstVplsMc->uni_cfg_cb[j]->port_index == pstUniData[i]->port_index)
                            {
                                ucSlot = pstVplsMc->uni_info_cb[j].slot_id;
                                iSlotNum = spm_get_vpls_mc_slot_num(pstVplsMc, ucSlot NBB_CCXT);
                                
                                //free
                                NBB_MM_FREE(pstVplsMc->uni_cfg_cb[j], MEM_SPM_VPLS_MC_UNI_CB);                                
                                
                                pstVplsMc->uni_cfg_cb[j] = NULL;
                                pstVplsMc->uni_num--;
                                
#ifdef SRC
                                if (iSlotNum == 0)
                                {
                                    //unit?
                                    ret = ApiDfeMulticastDelete(UNIT_0, 
                                                                pstVplsMc->basic_cfg_cb->mc_id, 
                                                                ucSlot);

                                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() ret=%d", ret));
                                    
                                    if (ret != SUCCESS)
                                    {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastDelete() error! ret=%d", ret));

                                        OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);

                                        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_C3_FUNC_ERROR", 
                                                                            ucMessage));
                                        ret = ERROR;
                                        
                                        //goto EXIT_LABEL;
                                    }
                                }
#endif

#ifdef SNU
                                if (iSlotNum == 0)
                                {
                                    //unit?
                                    ret = ApiDfeMulticastDelete(UNIT_0, 
                                                                pstVplsMc->basic_cfg_cb->mc_id, 
                                                                ucSlot);

                                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() ret=%d", ret));
                                    
                                    if (ret != SUCCESS)
                                    {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastDelete() error! ret=%d", ret));

                                        OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);

                                        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_C3_FUNC_ERROR", 
                                                                            ucMessage));
                                        ret = ERROR;
                                        
                                        //goto EXIT_LABEL;
                                    }
                                }
#endif
                                break;
                            }
                        }
                    }
                    else    //如果增加NNI口失败记录错误状态
                    {
                        pstSetVplsMc->uni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_del_vp_uni() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***: spm_vpls_mc_del_vp_uni() ret=%d", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_del_vp_uni() ret=%d", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    }
                }
            }                
        }
        else if (ulOperUni == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}

/*****************************************************************************
 函 数 名  : spm_vpls_mc_add_vp_nni
 功能描述  : 为VPLS组播组添加NNI端口
 输入参数  : ATG_DCI_VPLS_MC_KEY *pstVplsMcKey   
             ATG_DCI_VPLS_MC_NNI_DATA* pstVcKey  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月14日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_mc_add_vp_nni(NBB_USHORT usMcId, 
                               ATG_DCI_VPLS_MC_NNI_DATA* pstVplsMcNni,
                               NBB_UINT *piPosId,
                               NBB_UINT *piVpId,
                               NBB_BYTE *pucSlot
                               NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_INT iVpId = 0;
    NBB_BYTE ucC3Unit = 0;
    NBB_BYTE ucLagSlotId = 0;
    NBB_USHORT usLagPortId = 0;

    SPM_PORT_INFO_CB stPortInfo;
    
    L2MC_INTF_T stL2mcIntf;
    ATG_DCI_VC_KEY stVcKey;
    SPM_VC_CB *pstVcTbl;
    
    NBB_TRC_ENTRY("spm_vpls_mc_add_vp_nni");

    if (pstVplsMcNni == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_mc_add_vp_nni(pstVplsMcNni==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpls_mc_add_vp_nni(pstVplsMcNni==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_add_vp_nni(pstVplsMcNni==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stL2mcIntf, 0, sizeof(L2MC_INTF_T));
    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));

    stVcKey.vc_id   = pstVplsMcNni->vc_id;
    stVcKey.vc_type = pstVplsMcNni->vc_type;
    stVcKey.peer_ip = pstVplsMcNni->peer_ip;

    pstVcTbl = AVLL_FIND(SHARED.vc_tree, &stVcKey);
    
    if (pstVcTbl == NULL)       /* 如果条目不存在*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s 的VC表配置不存在，无法添加此组播端口!", stVcKey.vc_id,
                                                                                                                         stVcKey.vc_type,
                                                                                                                         spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT)));

        OS_PRINTF("***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s 的VC表配置不存在，无法添加此组播端口!\n", stVcKey.vc_id,
                                                                                                         stVcKey.vc_type,
                                                                                                         spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));

        OS_SPRINTF(ucMessage, "***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s 的VC表配置不存在，无法添加此组播端口!\n", stVcKey.vc_id,
                                                                                                         stVcKey.vc_type,
                                                                                                         spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        BASIC_CONFIG_NOT_EXIST, 
                                                                        "BASIC_CONFIG_NOT_EXIST", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //根据VC表配置中的port_index获取端口的port_id和slot_id，配给Arad和FE1600
    spm_get_portid_from_logical_port_index(pstVcTbl->vc_info_cb.port_index, &stPortInfo NBB_CCXT);

#ifdef SRC

	//如果是LAG，需要找到LAG下第一个端口的槽位，下发给FE1600
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucLagSlotId);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usMcId, stPortInfo.slot_id);
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastAdd() ret=%d", ret));
    
    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

#ifdef SNU
	//如果是LAG，需要找到LAG下第一个端口的槽位，下发给FE1600
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucLagSlotId);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usMcId, stPortInfo.slot_id);
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastAdd() ret=%d", ret));
    
    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

#ifdef SPU 
    //1、如果组播端口接口或子接口，不在本槽位，或
    //2、如果组播端口为LAG，LAG成员端口的第一个端口所在的槽不在本槽位；或
    //3、如果组播端口为VE，不在本槽位
    //则不调用驱动，直接返回成功。
    if (((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_SUB_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_VE_PORT == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.lag_slot_id) && (ATG_DCI_LAG == stPortInfo.port_type)))
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }

    //获取一个CUD资源
    //spm_alloc_vpls_mc_cud(&iMcCud);
#endif

    NBB_TRC_DETAIL((NBB_FORMAT "  usMcId=%d iVpId=%d usPortId=%d ucSlot=%d", usMcId,
                                                                            pstVcTbl->vc_info_cb.vp_idx, 
                                                                            stPortInfo.port_id, 
                                                                            stPortInfo.slot_id));
    
    //获取组播数据，给C3调用
    stL2mcIntf.mcId = usMcId;
    stL2mcIntf.seq = iVpId;
    stL2mcIntf.dvpIdx = pstVcTbl->vc_info_cb.vp_idx;
	*piVpId = pstVcTbl->vc_info_cb.vp_idx;
	*piPosId = pstVcTbl->vc_info_cb.next_hop_id;
    
#ifdef SPU 

	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_0, pstVcTbl->vc_info_cb.vp_idx);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
		//如果是第1片c3，发送到109
		//if (stPortInfo.unit_id == UNIT_0)
		{
			ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_0, pstVcTbl->vc_info_cb.vp_idx);
		}
    	//else	//如果是第2片c3，发送到110
    	//{
    	//	ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_1, pstVcTbl->vc_info_cb.vp_idx);
    	//}
	}    

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiAradAddEngressMcPort() ret=%d", ret));
#endif

    if (ret != SUCCESS)
    {    
#ifdef SPU 
        //如果端口没有添加成功，必须删除已分配的CUD
        //spm_free_vpls_mc_cud(iMcCud);
#endif
		//coverity[dead_error_begin]
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradAddEngressMcPort() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiAradAddEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVcTbl->vc_info_cb.vp_idx);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiAradAddEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVcTbl->vc_info_cb.vp_idx);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }

	//如果端口为LAG，则记录LAG第一个成员端口所在的槽位
    if (stPortInfo.port_type == ATG_DCI_LAG)
    {
    	*pucSlot = stPortInfo.lag_slot_id;
    }
    else
    {
        *pucSlot = stPortInfo.slot_id;
    }

    //pstVplsPortInfo->port_type = stPortInfo.port_type;

#if 0	//由于组播方案修改，现在可以不建组播出口intf
    ret = SUCCESS;

#ifdef SPU 
    //for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3SetL2MCOutIntf(stPortInfo.unit_id, &stL2mcIntf);

        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3SetL2MCOutIntf() ret=%d", ret));
    }
#endif 

    if (ret == SUCCESS)
    {
        *piPosId = stL2mcIntf.posId;
        *piMcCud = iMcCud;
        *pucSlot = stPortInfo.slot_id;
    }
    else
    {
#ifdef SPU 
        //如果端口没有添加成功，必须删除已分配的CUD
        spm_free_vpls_mc_cud(iMcCud);
#endif    

        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetL2MCOutIntf() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiC3SetL2MCOutIntf() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiC3SetL2MCOutIntf() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_C3_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }
#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_vpls_mc_add_vp_uni
 功能描述  : 为VPLS组播组添加UNI端口
 输入参数  : ATG_DCI_VPLS_MC_KEY *pstVplsMcKey      
             ATG_DCI_VPLS_MC_UNI_DATA* pstUniIndex  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月14日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_mc_add_vp_uni(NBB_USHORT usMcId, 
                                        ATG_DCI_VPLS_MC_UNI_DATA* pstUniIndex, 
                                        NBB_UINT *piPosId,
                                        NBB_UINT *piVpId,
                                        NBB_BYTE *pucSlot
                                        NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_INT iVpId = 0;
    NBB_BYTE ucC3Unit = 0;
    NBB_BYTE ucLagSlotId = 0;
    NBB_USHORT usLagPortId = 0;

    SPM_PORT_INFO_CB stPortInfo;
    
    L2MC_INTF_T stL2mcIntf;
    NBB_ULONG ulPortIndex;
    SPM_LOGICAL_PORT_CB *pstLogicalPort;
    
    NBB_TRC_ENTRY("spm_vpls_mc_add_vp_uni");

    if (pstUniIndex == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_mc_add_vp_uni(pstUniIndex==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpls_mc_add_vp_uni(pstUniIndex==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_add_vp_uni(pstUniIndex==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stL2mcIntf, 0, sizeof(L2MC_INTF_T));
    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    ulPortIndex = pstUniIndex->port_index;

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);
    
    if (pstLogicalPort == NULL)       /* 如果条目不存在*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: PORT_INDEX=%ld 的逻辑端口配置不存在，无法添加此组播端口!", pstUniIndex->port_index));

        OS_PRINTF("***ERROR***: PORT_INDEX=%ld 的逻辑端口配置不存在，无法添加此组播端口!\n", pstUniIndex->port_index);

        OS_SPRINTF(ucMessage, "***ERROR***: PORT_INDEX=%ld 的逻辑端口配置不存在，无法添加此组播端口!\n", pstUniIndex->port_index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        BASIC_CONFIG_NOT_EXIST, 
                                                                        "BASIC_CONFIG_NOT_EXIST", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //根据逻辑端口配置中的port_index获取端口的port_id和slot_id，配给Arad和FE1600
    spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);

#ifdef SRC

	//如果是LAG，需要找到LAG下第一个端口的槽位，下发给Arad
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucLagSlotId);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usMcId, stPortInfo.slot_id);
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastAdd() ret=%d", ret));

    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }
#endif

#ifdef SNU
	//如果是LAG，需要找到LAG下第一个端口的槽位，下发给Arad
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucLagSlotId);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usMcId, stPortInfo.slot_id);
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastAdd() ret=%d", ret));

    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }
#endif

#ifdef SPU	
    //1、如果组播端口接口或子接口，不在本槽位，或
    //2、如果组播端口为LAG，LAG成员端口的第一个端口所在的槽不在本槽位；或
    //3、如果组播端口为VE，不在本槽位
    //则不调用驱动，直接返回成功。
    if (((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_SUB_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_VE_PORT == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.lag_slot_id) && (ATG_DCI_LAG == stPortInfo.port_type)))
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }


    //获取一个CUD资源
    //spm_alloc_vpls_mc_cud(&iMcCud);
#endif

    NBB_TRC_DETAIL((NBB_FORMAT "  usMcId=%d iVpId=%d usPortId=%d ucSlot=%d", usMcId,
                                                                            pstLogicalPort->logic_port_info_cb.vp_idx, 
                                                                            stPortInfo.port_id, 
                                                                            stPortInfo.slot_id));
    
    //获取组播数据，给C3调用
    stL2mcIntf.mcId = usMcId;
    stL2mcIntf.seq = iVpId;
    stL2mcIntf.dvpIdx = pstLogicalPort->logic_port_info_cb.vp_idx;
	*piVpId = pstLogicalPort->logic_port_info_cb.vp_idx;
	*piPosId= pstLogicalPort->logic_port_info_cb.vp_idx;

#ifdef SPU 

	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			//ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, usLagPortId, pstLogicalPort->logic_port_info_cb.vp_idx);
			ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_0, pstLogicalPort->logic_port_info_cb.vp_idx);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
		//如果是第1片c3，发送到109
		//if (stPortInfo.unit_id == UNIT_0)
		{
			ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_0, pstLogicalPort->logic_port_info_cb.vp_idx);
		}
    	//else	//如果是第2片c3，发送到110
    	//{
    	//	ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_1, pstLogicalPort->logic_port_info_cb.vp_idx);
    	//}
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiAradAddEngressMcPort() ret=%d", ret));
#endif

    if (ret != SUCCESS)
    {
#ifdef SPU 
        //如果端口没有添加成功，必须删除已分配的CUD
        //spm_free_vpls_mc_cud(iMcCud);
#endif
		//coverity[dead_error_begin]
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradAddEngressMcPort() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiAradAddEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstLogicalPort->logic_port_info_cb.vp_idx);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiAradAddEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstLogicalPort->logic_port_info_cb.vp_idx);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }

	//如果端口为LAG，则记录LAG第一个成员端口所在的槽位
    if (stPortInfo.port_type == ATG_DCI_LAG)
    {
    	*pucSlot = stPortInfo.lag_slot_id;
    }
    else
    {
        *pucSlot = stPortInfo.slot_id;
    }

#if 0	//由于组播方案修改，现在可以不建组播出口intf
    ret = SUCCESS;
    
#ifdef SPU 
    //for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3SetL2MCOutIntf(stPortInfo.unit_id, &stL2mcIntf);

        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3SetL2MCOutIntf() ret=%d", ret));
    }
#endif 

    if (ret == SUCCESS)
    {
        *piPosId = stL2mcIntf.posId;
        *piMcCud = iMcCud;
        *pucSlot = stPortInfo.slot_id;
    }
    else
    {
#ifdef SPU 
        //如果端口没有添加成功，必须删除已分配的CUD
        spm_free_vpls_mc_cud(iMcCud);
#endif 
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetL2MCOutIntf() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiC3SetL2MCOutIntf() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiC3SetL2MCOutIntf() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_C3_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }
#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_vpls_mc_del_vp_nni
 功能描述  : 删除VPLS组播端口NNI
 输入参数  : NBB_USHORT usMcId                       
             SPM_VPLS_MC_CB *pstVplsMc               
             ATG_DCI_VPLS_MC_NNI_DATA* pstVplsMcNni  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月28日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_mc_del_vp_nni(NBB_USHORT usMcId, 
                               SPM_VPLS_MC_CB *pstVplsMc, 
                               ATG_DCI_VPLS_MC_NNI_DATA* pstVplsMcNni
                               NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_PORT_INFO_CB stPortInfo;
    
    ATG_DCI_VC_KEY stVcKey;
    SPM_VC_CB *pstVcTbl;
    
    NBB_TRC_ENTRY("spm_vpls_mc_del_vp_nni");

    if (pstVplsMc == NULL)
    {
    	ret = ERROR;
    	goto EXIT_LABEL;
    }
    
    if (pstVplsMcNni == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_mc_del_vp_nni(pstVplsMcNni==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpls_mc_del_vp_nni(pstVplsMcNni==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_del_vp_nni(pstVplsMcNni==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    stVcKey.vc_id   = pstVplsMcNni->vc_id;
    stVcKey.vc_type = pstVplsMcNni->vc_type;
    stVcKey.peer_ip = pstVplsMcNni->peer_ip;

    pstVcTbl = AVLL_FIND(SHARED.vc_tree, &stVcKey);
    
    if (pstVcTbl == NULL)       /* 如果条目不存在*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s 的VC表配置不存在，无法删除此组播端口!", stVcKey.vc_id,
                                                                                                                         stVcKey.vc_type,
                                                                                                                         spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT)));

        OS_PRINTF("***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s 的VC表配置不存在，无法删除此组播端口!\n", stVcKey.vc_id,
                                                                                                           stVcKey.vc_type,
                                                                                                           spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));

        OS_SPRINTF(ucMessage, "***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s 的VC表配置不存在，无法删除此组播端口!\n", stVcKey.vc_id,
                                                                                                           stVcKey.vc_type,
                                                                                                           spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        BASIC_CONFIG_NOT_EXIST, 
                                                                        "BASIC_CONFIG_NOT_EXIST", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //根据VC表配置中的port_index获取端口的port_id和slot_id，配给Arad和FE1600
    spm_get_portid_from_logical_port_index(pstVcTbl->vc_info_cb.port_index, &stPortInfo NBB_CCXT);


#if 0

    //如果这个槽位没有组播端口(包括UNI和NNI)了，SRC盘要删除此槽位，以免组播包继续向此槽位发送
    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (ucSlot == pstVplsMc->uni_slot[i])
    }
    
    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
    
    }
    
    if (pstVplsMc->nni_num == 1)
    {
        ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucSlot);

        NBB_TRC_DETAIL((NBB_FORMAT "ApiDfeMulticastAdd() ret=%d", ret));
        
        if (ret != SUCCESS)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));
            ret = ERROR;
            
            goto EXIT_LABEL;
        }
    }

#endif

    //1、如果组播端口接口或子接口，不在本槽位，或
    //2、如果组播端口为LAG，LAG成员端口的第一个端口所在的槽不在本槽位；或
    //3、如果组播端口为VE，不在本槽位
    //则不调用驱动，直接返回成功。
    if (((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_SUB_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_VE_PORT == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.lag_slot_id) && (ATG_DCI_LAG == stPortInfo.port_type)))
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }

    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        //找到要删除的VP
        if ((pstVplsMc->nni_cfg_cb[i] != NULL)
         && (pstVplsMc->nni_cfg_cb[i]->vc_id == pstVplsMcNni->vc_id)
         && (pstVplsMc->nni_cfg_cb[i]->vc_type == pstVplsMcNni->vc_type)
         && (pstVplsMc->nni_cfg_cb[i]->peer_ip == pstVplsMcNni->peer_ip))
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

    NBB_TRC_DETAIL((NBB_FORMAT "  usMcId=%d iVpId=%d usPortId=%d ucSlot=%d", usMcId, 
                                                                            pstVplsMc->nni_info_cb[i].vp_pos_id, 
                                                                            stPortInfo.port_id, 
                                                                            stPortInfo.slot_id));
    
#ifdef SPU 

	//如果是lag，删除109
    if (ATG_DCI_LAG == stPortInfo.port_type)
    {
	    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
	                                     usMcId, 
	                                     SPM_MULTICAST_PORT_0, 
	                                     pstVplsMc->nni_info_cb[i].vp_pos_id);	
    }
    else
    {
	    //如果VP属于第1片c3，Arad配置到109
		//if (stPortInfo.unit_id == UNIT_0)
		{
		    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
		                                     usMcId, 
		                                     SPM_MULTICAST_PORT_0, 
		                                     pstVplsMc->nni_info_cb[i].vp_pos_id);	
		}
		//else	//如果VP属于第2片c3，Arad配置到110
		//{
		//    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
		//                                     usMcId, 
		//                                     SPM_MULTICAST_PORT_1, 
		//                                     pstVplsMc->nni_info_cb[i].vp_pos_id);
		//}
    }

	if (ret != SUCCESS)
	{
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id));
        
        OS_PRINTF("***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
                                                         ret, 
                                                         "CALL_ARAD_FUNC_ERROR", 
                                                         ucMessage));
    }

#if 0	//由于组播方案修改，现在可以不建组播出口intf
    ret = SUCCESS;
    
    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3DelL2MCOutIntf(ucC3Unit, 
                                  pstVplsMc->nni_info_cb[i].vp_pos_id);

        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelL2MCOutIntf() ret=%d", ret));
    }

    if (ret == SUCCESS)
    {
        //释放VP所占据的CUD资源
        spm_free_vpls_mc_cud(pstVplsMc->nni_info_cb[i].cud);
        OS_MEMSET(&pstVplsMc->nni_info_cb[i], 0, sizeof(SPM_VPLS_MC_PORT_INFO_CB));
    }
    else
    {        
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3DelL2MCOutIntf() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiC3DelL2MCOutIntf() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiC3DelL2MCOutIntf() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_C3_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

#endif


    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_vpls_mc_del_vp_uni
 功能描述  : 删除VPLS组播端口UNI
 输入参数  : NBB_USHORT usMcId                      
             SPM_VPLS_MC_CB *pstVplsMc              
             ATG_DCI_VPLS_MC_UNI_DATA* pstUniIndex  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月28日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_mc_del_vp_uni(NBB_USHORT usMcId, 
                                       SPM_VPLS_MC_CB *pstVplsMc, 
                                       ATG_DCI_VPLS_MC_UNI_DATA* pstUniIndex
                                       NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_BYTE i = 0;
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_PORT_INFO_CB stPortInfo;

    NBB_ULONG ulPortIndex;
    SPM_LOGICAL_PORT_CB *pstLogicalPort;
    
    NBB_TRC_ENTRY("spm_vpls_mc_del_vp_uni");

    if (pstVplsMc == NULL)
    {
    	ret = ERROR;
    	goto EXIT_LABEL;
    }

    if (pstUniIndex == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_mc_del_vp_uni(pstUniIndex==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpls_mc_del_vp_uni(pstUniIndex==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_del_vp_uni(pstUniIndex==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    ulPortIndex = pstUniIndex->port_index;

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);
    
    if (pstLogicalPort == NULL)       /* 如果条目不存在*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: PORT_INDEX=%ld 的逻辑端口配置不存在，无法删除此组播端口!", pstUniIndex->port_index));

        OS_PRINTF("***ERROR***: PORT_INDEX=%ld 的逻辑端口配置不存在，无法删除此组播端口!\n", pstUniIndex->port_index);

        OS_SPRINTF(ucMessage, "***ERROR***: PORT_INDEX=%ld 的逻辑端口配置不存在，无法删除此组播端口!\n", pstUniIndex->port_index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        BASIC_CONFIG_NOT_EXIST, 
                                                                        "BASIC_CONFIG_NOT_EXIST", 
                                                                        ucMessage));
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //根据逻辑端口配置中的port_index获取端口的port_id和slot_id，配给Arad和FE1600
    spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);


#if 0
    ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucSlot);

    NBB_TRC_DETAIL((NBB_FORMAT "ApiDfeMulticastAdd() ret=%d", ret));
    
    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

    //1、如果组播端口接口或子接口，不在本槽位，或
    //2、如果组播端口为LAG，LAG成员端口的第一个端口所在的槽不在本槽位；或
    //3、如果组播端口为VE，不在本槽位
    //则不调用驱动，直接返回成功。
    if (((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_SUB_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_VE_PORT == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.lag_slot_id) && (ATG_DCI_LAG == stPortInfo.port_type)))
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }

    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        //找到要删除的VP
        if ((pstVplsMc->uni_cfg_cb[i]!= NULL)
         && (pstVplsMc->uni_cfg_cb[i]->port_index == ulPortIndex))
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

    NBB_TRC_DETAIL((NBB_FORMAT "  usMcId=%d iVpId=%d usPortId=%d ucSlot=%d", usMcId, 
                                                                            pstVplsMc->uni_info_cb[i].vp_pos_id, 
                                                                            stPortInfo.port_id, 
                                                                            stPortInfo.slot_id));
    
#ifdef SPU 
    
	//如果是lag，删除109
    if (ATG_DCI_LAG == stPortInfo.port_type)
    {
	    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
	                                     usMcId, 
	                                     SPM_MULTICAST_PORT_0, 
	                                     pstVplsMc->uni_info_cb[i].vp_pos_id);
    }
    else
    {
	    //如果VP属于第1片c3，Arad配置到109
		//if (stPortInfo.unit_id == UNIT_0)
		{
		    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
		                                     usMcId, 
		                                     SPM_MULTICAST_PORT_0, 
		                                     pstVplsMc->uni_info_cb[i].vp_pos_id);
		}
		//else	//如果VP属于第2片c3，Arad配置到110
		//{
		//    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
		//                                     usMcId, 
		//                                     SPM_MULTICAST_PORT_1, 
		//                                     pstVplsMc->uni_info_cb[i].vp_pos_id);
		//}
    }

	if (ret != SUCCESS)
	{
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id));
        
        OS_PRINTF("***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
                                                         ret, 
                                                         "CALL_ARAD_FUNC_ERROR", 
                                                         ucMessage));
    }

#if 0  
    ret = SUCCESS;
    
    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3DelL2MCOutIntf(ucC3Unit, pstVplsMc->uni_info_cb[i].vp_pos_id);

        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelL2MCOutIntf() ret=%d", ret));
    }


    if (ret == SUCCESS)
    {
        //释放VP所占据的CUD资源
        spm_free_vpls_mc_cud(pstVplsMc->uni_info_cb[i].cud);
        OS_MEMSET(&pstVplsMc->uni_info_cb[i], 0, sizeof(SPM_VPLS_MC_PORT_INFO_CB));
    }
    else
    {        
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3DelL2MCOutIntf() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiC3DelL2MCOutIntf() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiC3DelL2MCOutIntf() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_C3_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_init_vpls_mc_tree
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
NBB_VOID spm_init_vpls_mc_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vpls_mc_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_VPLS_MC_CB, vpls_mc_key);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.vpls_mc_tree, spm_vpls_mc_key_compare,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_VPLS_MC_CB, spm_vpls_mc_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_free_vpls_mc_cb
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
NBB_VOID spm_free_vpls_mc_cb(SPM_VPLS_MC_CB *pstVplsMc NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_vpls_mc_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstVplsMc, sizeof(SPM_VPLS_MC_CB), MEM_SPM_VPLS_MC_CB); 

    if (pstVplsMc == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pstVplsMc));  
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws_id %d", pstVplsMc->vpws_id_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstVplsMc->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstVplsMc->basic_cfg_cb, MEM_SPM_VPLS_MC_BASIC_CB);  
        pstVplsMc->basic_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放NNI和UNI数据块。                                                */
    /***************************************************************************/
    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (pstVplsMc->nni_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstVplsMc->nni_cfg_cb[i], MEM_SPM_VPLS_MC_NNI_CB);  
            pstVplsMc->nni_cfg_cb[i] = NULL;
        }
    }

    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        if (pstVplsMc->uni_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstVplsMc->uni_cfg_cb[i], MEM_SPM_VPLS_MC_UNI_CB);  
            pstVplsMc->uni_cfg_cb[i] = NULL;
        }
    } 

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstVplsMc->spm_vpls_mc_handle, HDL_SPM_VPLS_MC_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstVplsMc, MEM_SPM_VPLS_MC_CB);  
    pstVplsMc = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_vpls_mc_get_nni_num
 功能描述  : VPLS组播中NNI的个数
 输入参数  : ATG_DCI_VPLS_MC_KEY stVplsMcKey  
             NBB_INT *piVpNum                 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_mc_get_nni_num(ATG_DCI_VPLS_MC_KEY stVplsMcKey,
                                                                  NBB_INT *piVpNum
                                                                  NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    SPM_VPLS_MC_CB *pstVplsMc = NULL;

    NBB_TRC_ENTRY("spm_vpls_mc_get_nni_num");

    pstVplsMc = AVLL_FIND(v_spm_shared->vpls_mc_tree, &stVplsMcKey);

    if (pstVplsMc != NULL)
    {
        *piVpNum = pstVplsMc->nni_num;
        NBB_TRC_DETAIL((NBB_FORMAT "  VPLS_ID=%d<NNI=%d>", stVplsMcKey.vpls_id, pstVplsMc->nni_num));
    }
    else
    {
        *piVpNum = 0;
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_get_nni_num() VPLS_ID=%d 的 VPLS 不存在", stVplsMcKey.vpls_id));

        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_get_nni_num() VPLS_ID=%d 的 VPLS 不存在\n", stVplsMcKey.vpls_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_vpls_mc_get_uni_num
 功能描述  : VPLS组播中UNI的个数
 输入参数  : ATG_DCI_VPLS_MC_KEY stVplsMcKey  
             NBB_INT *piVpNum                 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_vpls_mc_get_uni_num(ATG_DCI_VPLS_MC_KEY stVplsMcKey,
                                                                  NBB_INT *piVpNum
                                                                  NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    SPM_VPLS_MC_CB *pstVplsMc = NULL;

    NBB_TRC_ENTRY("spm_vpls_mc_get_uni_num");

    pstVplsMc = AVLL_FIND(v_spm_shared->vpls_mc_tree, &stVplsMcKey);

    if (pstVplsMc != NULL)
    {
        *piVpNum = pstVplsMc->uni_num;
        NBB_TRC_DETAIL((NBB_FORMAT "  VPLS_ID=%d<UNI=%d>", stVplsMcKey.vpls_id, pstVplsMc->uni_num));
    }
    else
    {
        *piVpNum = 0;
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_get_uni_num() VPLS_ID=%d 的 VPLS 不存在", stVplsMcKey.vpls_id));


        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_get_uni_num() VPLS_ID=%d 的 VPLS 不存在\n", stVplsMcKey.vpls_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_alloc_vpLs_mc_cb
 功能描述  : 分配VPLS组播内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : SPM_VPLS_MC_CB
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月12日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_VPLS_MC_CB * spm_alloc_vpls_mc_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_VPLS_MC_CB *pstVplsMc = NULL;

    NBB_TRC_ENTRY("spm_alloc_vpls_mc_cb");

    /* 分配一个新的VPLS组播配置条目。*/
    pstVplsMc = (SPM_VPLS_MC_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_MC_CB), NBB_NORETRY_ACT, MEM_SPM_VPLS_MC_CB);
    if (pstVplsMc == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:申请VPLS组播条目空间为NULL!"));
        OS_PRINTF("***ERROR***:申请VPLS组播条目空间为NULL!\n");
        
        goto EXIT_LABEL;
    }

    /* 初始化VPLS组播配置条目 */
    OS_MEMSET(pstVplsMc, 0, sizeof(SPM_VPLS_MC_CB));
    pstVplsMc->basic_cfg_cb = NULL;

    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        pstVplsMc->nni_cfg_cb[i] = NULL;        
    }

    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        pstVplsMc->uni_cfg_cb[i] = NULL;
    }

    /* 建立用于该VPLS组播配置条目的句柄，作为异步消息交换的相关器。*/
    pstVplsMc->spm_vpls_mc_handle = NBB_CREATE_HANDLE(pstVplsMc, HDL_SPM_VPLS_MC_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_MC_CB allocated at %p with handle %#lx",
                   pstVplsMc, pstVplsMc->spm_vpls_mc_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstVplsMc->spm_vpls_mc_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstVplsMc);
}

/*****************************************************************************
 函 数 名  : spm_alloc_vpls_mc_cud
 功能描述  :  在64K的CUD资源池中分配一个CUD号
 输入参数  : NBB_INT *iMcCud  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月14日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_alloc_vpls_mc_cud(NBB_INT *iMcCud NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_UINT i = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_alloc_vpls_mc_cud");

    for (i=0; i<ATG_DCI_CUD_NUM; i++)
    {
        if (SHARED.mc_cud[i] == ATG_DCI_USED)
        {
            continue;
        }

        *iMcCud = i+1;
        SHARED.mc_cud[i] = ATG_DCI_USED;
        break;
    }

    //分配满了，返回错误
    if (i == ATG_DCI_CUD_NUM)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:CUD已经分配满了"));

        OS_PRINTF("***ERROR***:CUD已经分配满了!\n");

        OS_SPRINTF(ucMessage, "***ERROR***:CUD已经分配满了!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        
        ret = ERROR;
    }    

    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_free_vpls_mc_cud
 功能描述  : 释放分配的cud资源
 输入参数  : NBB_INT iMcCud  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月23日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_free_vpls_mc_cud(NBB_INT iMcCud NBB_CCXT_T NBB_CXT)
{
    NBB_UINT i = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_free_vpls_mc_cud");

    if ((iMcCud < 1) || (iMcCud > ATG_DCI_CUD_NUM))
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:输入的CUD值超出范围！"));
        OS_PRINTF("***ERROR***:输入的CUD值超出范围!\n");
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //OS_PRINTF("iMcCud=%d mc_cud=%d\n", iMcCud, SHARED.mc_cud[iMcCud-1]);
    
    SHARED.mc_cud[iMcCud-1] = ATG_DCI_UNUSED;
    
    //OS_PRINTF("iMcCud=%d mc_cud=%d\n", iMcCud, SHARED.mc_cud[iMcCud-1]);

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_init_vpls_mc_cud
 功能描述  : 初始化cud,系统起来时运行一次
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_vpls_mc_cud(NBB_CXT_T NBB_CXT)
{
    NBB_UINT i = 0;

    NBB_TRC_ENTRY("spm_init_vpls_mc_cud");

    for (i=0; i<ATG_DCI_CUD_NUM; i++)
    {
        SHARED.mc_cud[i] = ATG_DCI_UNUSED;
    }
    
    NBB_TRC_EXIT();
}

/*****************************************************************************
 函 数 名  : spm_set_vpls_mc_cud_state
 功能描述  : 设置CUD状态,0=未使用,1=使用
 输入参数  : NBB_UINT iPos   
             NBB_INT iState  0=未使用,1=使用
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月15日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_set_vpls_mc_cud_state(NBB_UINT iMcCud, NBB_INT iState NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_set_vpls_mc_cud_state");

    SHARED.mc_cud[iMcCud-1] = iState;

    NBB_TRC_EXIT();
}

/*****************************************************************************
 函 数 名  : spm_get_vpls_mc_slot_num
 功能描述  : 获取在ucSlot槽位端口的个数
 输入参数  : SPM_VPLS_MC_CB *pstVplsMc  
             NBB_BYTE ucSlot            
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年2月4日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_get_vpls_mc_slot_num(SPM_VPLS_MC_CB *pstVplsMc, NBB_BYTE ucSlot NBB_CCXT_T NBB_CXT)
{
    NBB_INT i = 0;
    NBB_INT iNum = 0;
    
    NBB_TRC_ENTRY("spm_get_vpls_mc_slot_num");

    if (pstVplsMc == NULL)
    {
    	goto EXIT_LABEL;
    }

    //NNI中在此槽位的端口个数
    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (ucSlot == pstVplsMc->nni_info_cb[i].slot_id)
            iNum++;
    }

    //UNI中在此槽位的端口个数
    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        if (ucSlot == pstVplsMc->uni_info_cb[i].slot_id)
            iNum++;
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return iNum;
}

/*****************************************************************************
   函 数 名  : spm_free_all_vpls_mc_cb
   功能描述  : 释放vpls多播配置所有的内存空间
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

NBB_VOID spm_free_all_vpls_mc_cb(NBB_CXT_T NBB_CXT)
{
	SPM_VPLS_MC_CB *pstVplsMc = NULL;
	
    for (pstVplsMc = (SPM_VPLS_MC_CB*) AVLL_FIRST(SHARED.vpls_mc_tree);
         pstVplsMc != NULL;
         pstVplsMc = (SPM_VPLS_MC_CB*) AVLL_FIRST(SHARED.vpls_mc_tree))
    {
    	
		AVLL_DELETE(SHARED.vpls_mc_tree, pstVplsMc->spm_vpls_mc_node);     	
    	spm_free_vpls_mc_cb(pstVplsMc NBB_CCXT);
    }
}


#endif

