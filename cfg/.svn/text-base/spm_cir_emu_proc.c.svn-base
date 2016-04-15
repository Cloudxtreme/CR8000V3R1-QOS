/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_cir_emu_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年6月24日
  最近修改   :
  功能描述   : 处理电路仿真盘全局配置
  函数列表   :
              spm_init_cir_emu_cb
              spm_rcv_dci_set_cir_emu
  修改历史   :
  1.日    期   : 2013年6月24日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char cir_emu_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_cir_emu
 功能描述  : 电路仿真盘全局配置IPS消息处理函数
 输入参数  : ATG_DCI_SET_CIR_EMU_BOARD *pstSetCirEmu  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月24日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_cir_emu(ATG_DCI_SET_CIR_EMU_BOARD *pstSetCirEmu NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    NBB_BYTE ucTdmSubCardExist = ATG_DCI_UNEXIST;

    ATG_DCI_CIR_EMU_PUB_LIMIT *pstPubLimit = NULL;
    ATG_DCI_CIR_EMU_SYN_CLK *pstSynClk = NULL;
    ATG_DCI_CIR_EMU_ELTRC_BOARD *pstEltrcBoard = NULL;
    ATG_DCI_CIR_EMU_REBTH_LIMIT *pstRebthLimit = NULL;
    ATG_DCI_CIR_EMU_MU_LIMIT *pstMuLimit = NULL;
    ATG_DCI_CIR_EMU_HIGH_ORD_PASS *pstHighOrdPass = NULL;
    ATG_DCI_CIR_EMU_LOW_ORD_PASS *pstLowOrdPass = NULL;
    
    NBB_BYTE *pucPubLimitStart = NULL;
    NBB_BYTE *pucSynClkStart = NULL;
    NBB_BYTE *pucEltrcBoardStart = NULL;
    NBB_BYTE *pucRebthLimitStart = NULL;
    NBB_BYTE *pucMuLimitStart = NULL;
    NBB_BYTE *pucHighOrdPassStart = NULL;
    NBB_BYTE *pucLowOrdPassStart = NULL;
    
    NBB_ULONG ulOperPubLimit    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperSynClk      = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperEltrcBoard  = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperRebthLimit  = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperMuLimit     = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperHighOrdPass = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLowOrdPass  = ATG_DCI_OPER_NULL;
    
    ATG_DCI_CIR_EMU_KEY stCirEmuKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    SPM_CIR_EMU_CB *pstciremu = NULL;

    NBB_TRC_ENTRY("spm_rcv_dci_set_cir_emu");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetCirEmu != NULL);

    if (pstSetCirEmu == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_cir_emu(pstSetCirEmu==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_cir_emu(pstSetCirEmu==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_cir_emu(pstSetCirEmu==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        goto EXIT_LABEL;
    }

    OS_MEMSET(&stCirEmuKey, 0, sizeof(ATG_DCI_CIR_EMU_KEY));
    
    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetCirEmu->return_code = ATG_DCI_RC_OK;
	
    //OS_MEMCPY(&(SHARED.cir_emu_cb.stCirEmuKey),&stCirEmuKey,sizeof(ATG_DCI_CIR_EMU_KEY));

    stCirEmuKey.slot = pstSetCirEmu->key.slot;
    stCirEmuKey.subslot = pstSetCirEmu->key.subslot; 
    
    pstciremu = AVLL_FIND(SHARED.cir_emu_tree, &stCirEmuKey);

    if (pstciremu == NULL)    //条目不存在
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }
    
    /* 获取子配置操作 */
    ulOperPubLimit      = pstSetCirEmu->oper_pub_limit;
    ulOperSynClk        = pstSetCirEmu->oper_syn_clk;
    ulOperEltrcBoard    = pstSetCirEmu->oper_eltrc_board;
    ulOperRebthLimit    = pstSetCirEmu->oper_rebth_limit;
    ulOperMuLimit       = pstSetCirEmu->oper_mu_limit;
    ulOperHighOrdPass   = pstSetCirEmu->oper_high_ord_pass;
    ulOperLowOrdPass    = pstSetCirEmu->oper_low_ord_pass;
    
    /* 删除整个条目 */
    if (pstSetCirEmu->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除SLOT=%d SUB_CARD_NO=%d 的电路仿真盘全局配置", stCirEmuKey.slot, stCirEmuKey.subslot));
        
         if (ucIfExist == ATG_DCI_UNEXIST)
        {
            /* 待修改：加打印PD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的电路仿真全局配置并不存在，退出!"));

            OS_PRINTF("***ERROR***:要删除的电路仿真全局配置并不存在，退出!\n");

            OS_SPRINTF(ucMessage, "***ERROR***:要删除的电路仿真全局配置并不存在，退出!\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPWS_PD, 0, "ld d s s", stCirEmuKey.slot,stCirEmuKey.subslot,
                    DEL_UNEXIST_CONFIG_ERROR,
                    "DEL_UNEXIST_CONFIG_ERROR",
                    ucMessage));
                    
            goto EXIT_LABEL;
        }
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  查询到此电路仿真盘 全局配置，从tree中删除"));
            spm_dbg_print_cir_emu_head(stCirEmuKey.slot, stCirEmuKey.subslot,SPM_OPER_DEL);
            spm_dbg_record_cir_emu_head(stCirEmuKey.slot, stCirEmuKey.subslot,SPM_OPER_DEL);

            if (pstciremu != NULL)
            {

                AVLL_DELETE(SHARED.cir_emu_tree, pstciremu->spm_cir_emu_node);  

                //释放VC表分配的内存空间
                spm_free_cir_emu_cb(pstciremu NBB_CCXT);
            }
            
        }
            
    }
    /* 增加或更新条目 */
    else
    {     
        if(stCirEmuKey.slot == SHARED.local_slot)
        {
        
             /* 如果条目不存在，新申请内存空间保存数据 */
            if (ucIfExist == ATG_DCI_UNEXIST)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  增加slot=%ld subslot=%d 的 电路仿真盘全局配置", 
                    stCirEmuKey.slot, stCirEmuKey.subslot));
                spm_dbg_print_cir_emu_head(stCirEmuKey.slot, stCirEmuKey.subslot,SPM_OPER_ADD);
                spm_dbg_record_cir_emu_head(stCirEmuKey.slot, stCirEmuKey.subslot,SPM_OPER_ADD);
                
                /* 申请一个新条目的内存空间 */
                pstciremu = spm_alloc_cir_emu_cb(NBB_CXT);

    			if (pstciremu != NULL)
    			{
    	            //把条目插入到tree中
    	            OS_MEMCPY(&pstciremu->stCirEmuKey, &stCirEmuKey, sizeof(ATG_DCI_CIR_EMU_KEY));

    	            //coverity[no_effect_test]
    	            AVLL_INSERT(SHARED.cir_emu_tree, pstciremu->spm_cir_emu_node);
    			}
            }
            else
            {
                NBB_TRC_DETAIL((NBB_FORMAT "   更新slot=%ld subslot=%d 的 电路仿真盘全局配置", 
                    stCirEmuKey.slot, stCirEmuKey.subslot));
                spm_dbg_print_cir_emu_head(stCirEmuKey.slot, stCirEmuKey.subslot,SPM_OPER_UPD);
                spm_dbg_record_cir_emu_head(stCirEmuKey.slot, stCirEmuKey.subslot,SPM_OPER_UPD);
            }

    		//增加此判断，取消PC-Lint告警
            if (pstciremu == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT 
                    "  ***ERROR***:要增加的slot=%ld subslot=%d 的 电路仿真盘全局配置未能成功申请内存!", 
                    stCirEmuKey.slot, stCirEmuKey.subslot));

                OS_PRINTF("***ERROR***:要增加的slot=%ld subslot=%d 的 电路仿真盘全局配置未能成功申请内存!\n", 
                    stCirEmuKey.slot, stCirEmuKey.subslot);

                OS_SPRINTF(ucMessage,
                    "***ERROR***:要增加的slot=%ld subslot=%d 的 电路仿真盘全局配置未能成功申请内存!\n", 
                    stCirEmuKey.slot, stCirEmuKey.subslot);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | GLOBAL_PD, 0, "ld d s d s s", stCirEmuKey.slot, stCirEmuKey.subslot,
                                                                 ALLOC_MEMORY_ERROR, 
                                                                 "ALLOC_MEMORY_ERROR", 
                                                                 ucMessage));
                                                                 
                pstSetCirEmu->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                
                goto EXIT_LABEL;
            }
        
            /***************************************************************************/
            /*                        获取配置，逐条处理                               */
            /***************************************************************************/
            /******************************** 公共门限配置 *********************************/
            if (ulOperPubLimit == ATG_DCI_OPER_ADD)
            {

                /* 计算第一个entry的地址。*/
                pucPubLimitStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, &pstSetCirEmu->pub_limit_data);    

                /* 如果指针为NULL，无数据 */
                if (pucPubLimitStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  CIR EMU pucPubLimitStart is NULL."));   
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstPubLimit = (ATG_DCI_CIR_EMU_PUB_LIMIT*)pucPubLimitStart;

                    if (cir_emu_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  1）公共门限配置\n");
                        
                        //spm_dbg_print_global_basic_cfg(pstBasicData);
                    }
    				OS_PRINTF("  1）公共门限配置\n");
                    OS_SPRINTF(ucMessage,"  1）公共门限配置\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    //spm_dbg_record_global_basic_cfg(pstBasicData);

                    //打印配置
                    //NBB_TRC_DETAIL((NBB_FORMAT "  MAC老化时间=%d(100ms)", pstBasicData->mac_ageing_time));                  
                    if (pstciremu->pub_limit_cb == NULL)
                    {
                        pstciremu->pub_limit_cb = 
                            (ATG_DCI_CIR_EMU_PUB_LIMIT *)NBB_MM_ALLOC(sizeof(ATG_DCI_CIR_EMU_PUB_LIMIT),
                        NBB_NORETRY_ACT,
                        MEM_SPM_CIR_EMU_PUB_LIMIT_CB);
                    }

                    if(NULL == pstciremu->pub_limit_cb)
                    {
                        NBB_TRC_FLOW((NBB_FORMAT 
                        "  ***ERROR***:slot=%ld subslot=%d 的pub_limit_cb 子配置申请内存失败!", 
                            stCirEmuKey.slot, stCirEmuKey.subslot));
                        OS_SPRINTF(ucMessage,
                            "***ERROR***:slot=%ld subslot=%d 的pub_limit_cb 子配置申请内存失败!\n", 
                            stCirEmuKey.slot, stCirEmuKey.subslot);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        goto EXIT_LABEL;
                    }
                    
                    //保存数据
                    else
                    {
                        OS_MEMCPY(pstciremu->pub_limit_cb, pstPubLimit, sizeof(ATG_DCI_CIR_EMU_PUB_LIMIT));
                    }                  
                    
                }                 
            }
            else if (ulOperPubLimit == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperPubLimit == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /******************************** 同步时钟源输出配置 *********************************/
            if (ulOperSynClk == ATG_DCI_OPER_ADD)
            {

                /* 计算第一个entry的地址。*/
                pucSynClkStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, &pstSetCirEmu->syn_clk_data);    

                /* 如果指针为NULL，无数据 */
                if (pucSynClkStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  CIR EMU pucSynClkStart is NULL."));   
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstSynClk = (ATG_DCI_CIR_EMU_SYN_CLK*)pucSynClkStart;

                    if (cir_emu_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  2）同步时钟源输出配置\n");
                        //spm_dbg_print_global_basic_cfg(pstBasicData);
                    }

                    OS_SPRINTF(ucMessage,"  2）同步时钟源输出配置\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    //spm_dbg_record_global_basic_cfg(pstBasicData);

                    //打印配置
                    //NBB_TRC_DETAIL((NBB_FORMAT "  MAC老化时间=%d(100ms)", pstBasicData->mac_ageing_time));
                    //OS_MEMCPY(&(SHARED.cir_emu_cb.syn_clk_cb),pstSynClk,sizeof(ATG_DCI_CIR_EMU_SYN_CLK));
                    if (pstciremu->syn_clk_cb == NULL)
                    {
                        pstciremu->syn_clk_cb = (ATG_DCI_CIR_EMU_SYN_CLK *)NBB_MM_ALLOC(sizeof(ATG_DCI_CIR_EMU_SYN_CLK),
                        NBB_NORETRY_ACT,
                        MEM_SPM_CIR_EMU_SYN_CLK_CB);
                    }
                    
                    if(NULL == pstciremu->syn_clk_cb)
                    {
                        NBB_TRC_FLOW((NBB_FORMAT 
                        "  ***ERROR***:slot=%ld subslot=%d 的syn_clk_cb 子配置申请内存失败!", 
                            stCirEmuKey.slot, stCirEmuKey.subslot));
                        OS_SPRINTF(ucMessage,
                            "***ERROR***:slot=%ld subslot=%d 的syn_clk_cb 子配置申请内存失败!\n", 
                            stCirEmuKey.slot, stCirEmuKey.subslot);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        goto EXIT_LABEL;
                    }
                    
                    //保存数据
                    else
                    {
                        OS_MEMCPY(pstciremu->syn_clk_cb, pstSynClk, sizeof(ATG_DCI_CIR_EMU_SYN_CLK));
                    }  
                    
                }                 
            }
            else if (ulOperSynClk == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperSynClk == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /******************************** 电口盘配置 *********************************/
            if (ulOperEltrcBoard == ATG_DCI_OPER_ADD)
            {

                /* 计算第一个entry的地址。*/
                pucEltrcBoardStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, &pstSetCirEmu->eltrc_board_data);    

                /* 如果指针为NULL，无数据 */
                if (pucEltrcBoardStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  CIR EMU pucEltrcBoardStart is NULL."));   
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstEltrcBoard = (ATG_DCI_CIR_EMU_ELTRC_BOARD*)pucEltrcBoardStart;

                    if (cir_emu_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  3）电口盘配置\n");
                        //spm_dbg_print_global_basic_cfg(pstBasicData);
                    }

                    OS_SPRINTF(ucMessage,"  3）电口盘配置\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    //spm_dbg_record_global_basic_cfg(pstBasicData);

                    //打印配置
                    //NBB_TRC_DETAIL((NBB_FORMAT "  MAC老化时间=%d(100ms)", pstBasicData->mac_ageing_time));
                    //OS_MEMCPY(&(SHARED.cir_emu_cb.eltrc_board_cb),pstEltrcBoard,sizeof(ATG_DCI_CIR_EMU_ELTRC_BOARD));
                    if (pstciremu->eltrc_board_cb == NULL)
                    {
                        pstciremu->eltrc_board_cb = 
                            (ATG_DCI_CIR_EMU_ELTRC_BOARD *)NBB_MM_ALLOC(sizeof(ATG_DCI_CIR_EMU_ELTRC_BOARD),
                        NBB_NORETRY_ACT,
                        MEM_SPM_CIR_EMU_ELTRC_BOARD_CB);
                    }
                    
                    if(NULL == pstciremu->eltrc_board_cb)
                    {
                        NBB_TRC_FLOW((NBB_FORMAT 
                        "  ***ERROR***:slot=%ld subslot=%d 的eltrc_board_cb 子配置申请内存失败!", 
                            stCirEmuKey.slot, stCirEmuKey.subslot));
                        OS_SPRINTF(ucMessage,
                            "***ERROR***:slot=%ld subslot=%d 的eltrc_board_cb 子配置申请内存失败!\n", 
                            stCirEmuKey.slot, stCirEmuKey.subslot);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        goto EXIT_LABEL;
                    }
                    
                    //保存数据
                    else
                    {
                        OS_MEMCPY(pstciremu->eltrc_board_cb, pstEltrcBoard, sizeof(ATG_DCI_CIR_EMU_ELTRC_BOARD));
                    }  
                }                 
            }
            else if (ulOperEltrcBoard == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperEltrcBoard == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /******************************** 再生段门限配置 *********************************/
            if (ulOperRebthLimit == ATG_DCI_OPER_ADD)
            {

                /* 计算第一个entry的地址。*/
                pucRebthLimitStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, &pstSetCirEmu->rebth_limit_data);    

                /* 如果指针为NULL，无数据 */
                if (pucRebthLimitStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  CIR EMU pucRebthLimitStart is NULL."));   
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstRebthLimit = (ATG_DCI_CIR_EMU_REBTH_LIMIT*)pucRebthLimitStart;

                    if (cir_emu_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  4）再生段门限配置\n");
                        //spm_dbg_print_global_basic_cfg(pstBasicData);
                    }

                    OS_SPRINTF(ucMessage,"  4）再生段门限配置\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    //spm_dbg_record_global_basic_cfg(pstBasicData);

                    //打印配置
                    //NBB_TRC_DETAIL((NBB_FORMAT "  MAC老化时间=%d(100ms)", pstBasicData->mac_ageing_time));
                    //OS_MEMCPY(&(SHARED.cir_emu_cb.rebth_limit_cb),pstRebthLimit,sizeof(ATG_DCI_CIR_EMU_REBTH_LIMIT));
                    if (pstciremu->rebth_limit_cb == NULL)
                    {
                        pstciremu->rebth_limit_cb = 
                            (ATG_DCI_CIR_EMU_REBTH_LIMIT *)NBB_MM_ALLOC(sizeof(ATG_DCI_CIR_EMU_REBTH_LIMIT),
                        NBB_NORETRY_ACT,
                        MEM_SPM_CIR_EMU_REBTH_LIMIT_CB);
                    }
                    
                    if(NULL == pstciremu->rebth_limit_cb)
                    {
                        NBB_TRC_FLOW((NBB_FORMAT 
                        "  ***ERROR***:slot=%ld subslot=%d 的rebth_limit_cb 子配置申请内存失败!", 
                            stCirEmuKey.slot, stCirEmuKey.subslot));
                        OS_SPRINTF(ucMessage,
                            "***ERROR***:slot=%ld subslot=%d 的rebth_limit_cb 子配置申请内存失败!\n", 
                            stCirEmuKey.slot, stCirEmuKey.subslot);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        goto EXIT_LABEL;
                    }
                    
                    //保存数据
                    else
                    {
                        OS_MEMCPY(pstciremu->rebth_limit_cb, pstRebthLimit, sizeof(ATG_DCI_CIR_EMU_REBTH_LIMIT));
                    }  
                }                 
            }
            else if (ulOperRebthLimit == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperRebthLimit == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /******************************** 复用段门限配置 *********************************/
            if (ulOperMuLimit == ATG_DCI_OPER_ADD)
            {

                /* 计算第一个entry的地址。*/
                pucMuLimitStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, &pstSetCirEmu->mu_limit_data);    

                /* 如果指针为NULL，无数据 */
                if (pucMuLimitStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  CIR EMU pucMuLimitStart is NULL."));   
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstMuLimit = (ATG_DCI_CIR_EMU_MU_LIMIT*)pucMuLimitStart;

                    if (cir_emu_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  5）复用段门限配置\n");
                        //spm_dbg_print_global_basic_cfg(pstBasicData);
                    }

                    OS_SPRINTF(ucMessage,"  5）复用段门限配置\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    //spm_dbg_record_global_basic_cfg(pstBasicData);

                    //打印配置
                    //NBB_TRC_DETAIL((NBB_FORMAT "  MAC老化时间=%d(100ms)", pstBasicData->mac_ageing_time));
                    //OS_MEMCPY(&(SHARED.cir_emu_cb.mu_limit_cb), pstMuLimit,sizeof(ATG_DCI_CIR_EMU_MU_LIMIT));
                    if (pstciremu->mu_limit_cb == NULL)
                    {
                        pstciremu->mu_limit_cb = 
                            (ATG_DCI_CIR_EMU_MU_LIMIT *)NBB_MM_ALLOC(sizeof(ATG_DCI_CIR_EMU_MU_LIMIT),
                        NBB_NORETRY_ACT,
                        MEM_SPM_CIR_EMU_MU_LIMIT_CB);
                    }

                    if(NULL == pstciremu->mu_limit_cb)
                    {
                        NBB_TRC_FLOW((NBB_FORMAT 
                        "  ***ERROR***:slot=%ld subslot=%d 的mu_limit_cb 子配置申请内存失败!", 
                            stCirEmuKey.slot, stCirEmuKey.subslot));
                        OS_SPRINTF(ucMessage,
                            "***ERROR***:slot=%ld subslot=%d 的mu_limit_cb 子配置申请内存失败!\n", 
                            stCirEmuKey.slot, stCirEmuKey.subslot);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        goto EXIT_LABEL;
                    }
                    
                    //保存数据
                    else
                    {
                       OS_MEMCPY(pstciremu->mu_limit_cb, pstMuLimit, sizeof(ATG_DCI_CIR_EMU_MU_LIMIT)); 
                    }  
                  
                }                 
            }
            else if (ulOperMuLimit == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperMuLimit == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /******************************** 高阶通道门限配置 *********************************/
            if (ulOperHighOrdPass == ATG_DCI_OPER_ADD)
            {

                /* 计算第一个entry的地址。*/
                pucHighOrdPassStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, &pstSetCirEmu->high_ord_pass_data);    

                /* 如果指针为NULL，无数据 */
                if (pucHighOrdPassStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  CIR EMU pucHighOrdPassStart is NULL."));   
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstHighOrdPass = (ATG_DCI_CIR_EMU_HIGH_ORD_PASS*)pucHighOrdPassStart;

                    if (cir_emu_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  6）高阶通道门限配置\n");
                        //spm_dbg_print_global_basic_cfg(pstBasicData);
                    }

                    OS_SPRINTF(ucMessage,"  6）高阶通道门限配置\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    //spm_dbg_record_global_basic_cfg(pstBasicData);

                    //打印配置
                    //NBB_TRC_DETAIL((NBB_FORMAT "  MAC老化时间=%d(100ms)", pstBasicData->mac_ageing_time));
                    if (pstciremu->high_ord_pass_cb == NULL)
                    {
                        pstciremu->high_ord_pass_cb = 
                            (ATG_DCI_CIR_EMU_HIGH_ORD_PASS *)NBB_MM_ALLOC(sizeof(ATG_DCI_CIR_EMU_HIGH_ORD_PASS),
                        NBB_NORETRY_ACT,
                        MEM_SPM_CIR_EMU_HIGH_ORD_PASS_CB);
                    }

                    if(NULL == pstciremu->high_ord_pass_cb)
                    {
                        NBB_TRC_FLOW((NBB_FORMAT 
                        "  ***ERROR***:slot=%ld subslot=%d 的high_ord_pass_cb 子配置申请内存失败!", 
                            stCirEmuKey.slot, stCirEmuKey.subslot));
                        OS_SPRINTF(ucMessage,
                            "***ERROR***:slot=%ld subslot=%d 的high_ord_pass_cb 子配置申请内存失败!\n", 
                            stCirEmuKey.slot, stCirEmuKey.subslot);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        goto EXIT_LABEL;
                    }
                    
                    //保存数据
                    else
                    {
                        OS_MEMCPY(pstciremu->high_ord_pass_cb, 
                            pstHighOrdPass, sizeof(ATG_DCI_CIR_EMU_HIGH_ORD_PASS));
                    } 
                         
                }                 
            }
            else if (ulOperHighOrdPass == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperHighOrdPass == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /******************************** 低阶通道门限配置 *********************************/
            if (ulOperLowOrdPass == ATG_DCI_OPER_ADD)
            {

                /* 计算第一个entry的地址。*/
                pucLowOrdPassStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, &pstSetCirEmu->low_ord_pass_data);    

                /* 如果指针为NULL，无数据 */
                if (pucLowOrdPassStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  CIR EMU pucLowOrdPassStart is NULL."));   
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstLowOrdPass = (ATG_DCI_CIR_EMU_LOW_ORD_PASS*)pucLowOrdPassStart;

                    if (cir_emu_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  7）低阶通道门限配置\n");
                        //spm_dbg_print_global_basic_cfg(pstBasicData);
                    }

                    OS_SPRINTF(ucMessage,"  7）低阶通道门限配置\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    //spm_dbg_record_global_basic_cfg(pstBasicData);

                    //打印配置
                    //NBB_TRC_DETAIL((NBB_FORMAT "  MAC老化时间=%d(100ms)", pstBasicData->mac_ageing_time));
                    if (pstciremu->low_ord_pass_cb == NULL)
                    {
                        pstciremu->low_ord_pass_cb = 
                            (ATG_DCI_CIR_EMU_LOW_ORD_PASS *)NBB_MM_ALLOC(sizeof(ATG_DCI_CIR_EMU_LOW_ORD_PASS),
                        NBB_NORETRY_ACT,
                        MEM_SPM_CIR_EMU_LOW_ORD_PASS_CB);
                    }

                    if(NULL == pstciremu->low_ord_pass_cb)
                    {
                        NBB_TRC_FLOW((NBB_FORMAT 
                        "  ***ERROR***:slot=%ld subslot=%d 的low_ord_pass_cb 子配置申请内存失败!", 
                            stCirEmuKey.slot, stCirEmuKey.subslot));
                        OS_SPRINTF(ucMessage,
                            "***ERROR***:slot=%ld subslot=%d 的low_ord_pass_cb 子配置申请内存失败!\n", 
                            stCirEmuKey.slot, stCirEmuKey.subslot);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        goto EXIT_LABEL;
                    }
                    
                    //保存数据
                    else
                    {
                       OS_MEMCPY(pstciremu->low_ord_pass_cb, pstLowOrdPass, sizeof(ATG_DCI_CIR_EMU_LOW_ORD_PASS));  
                    } 
                }                 
            }
            else if (ulOperLowOrdPass == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperLowOrdPass == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }
            
            ucTdmSubCardExist = spm_hardw_getcesability(pstSetCirEmu->key.subslot);
                
            //如果仿真盘存在才发送IPS消息给子卡
            if ((SHARED.local_slot_id == pstSetCirEmu->key.slot) && (ucTdmSubCardExist == ATG_DCI_EXIST))
            {
                spm_send_cir_emu_cfg(pstSetCirEmu, pstSetCirEmu->key.subslot NBB_CCXT);
            }
        }
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_send_cir_emu_cfg
 功能描述  : 
 输入参数  : ATG_DCI_SET_CIR_EMU_BOARD *pstSetCirEmu  
             NBB_BYTE ucSubBoardNo                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月25日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_cir_emu_cfg(ATG_DCI_SET_CIR_EMU_BOARD *pstSetCirEmu, NBB_BYTE ucSubBoardNo NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    
    /* 获取的子配置 */
    ATG_SBI_CIR_EMU_PUB_LIMIT *pstSbmPubLimit       = NULL;
    ATG_SBI_CIR_EMU_SYN_CLK *pstSbmSynClk           = NULL;
    ATG_SBI_CIR_EMU_ELTRC_BOARD *pstSbmEltrcBoard      = NULL;
    ATG_SBI_CIR_EMU_REBTH_LIMIT *pstSbmRebthLimit    = NULL;
    ATG_SBI_CIR_EMU_MU_LIMIT *pstSbmMuLimit           = NULL;
    ATG_SBI_CIR_EMU_HIGH_ORD_PASS *pstSbmHighOrdPass      = NULL;
    ATG_SBI_CIR_EMU_LOW_ORD_PASS *pstSbmLowOrdPass      = NULL;    

    ATG_DCI_CIR_EMU_PUB_LIMIT *pstPubLimit       = NULL;
    ATG_DCI_CIR_EMU_SYN_CLK *pstSynClk           = NULL;
    ATG_DCI_CIR_EMU_ELTRC_BOARD *pstEltrcBoard      = NULL;
    ATG_DCI_CIR_EMU_REBTH_LIMIT *pstRebthLimit    = NULL;
    ATG_DCI_CIR_EMU_MU_LIMIT *pstMuLimit           = NULL;
    ATG_DCI_CIR_EMU_HIGH_ORD_PASS *pstHighOrdPass      = NULL;
    ATG_DCI_CIR_EMU_LOW_ORD_PASS *pstLowOrdPass      = NULL;    
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucSbmPubLimitStart = NULL;
    NBB_BYTE *pucSbmSynClkStart   = NULL;
    NBB_BYTE *pucSbmEltrcBoardStart   = NULL;
    NBB_BYTE *pucSbmRebthLimitStart  = NULL;
    NBB_BYTE *pucSbmMuLimitStart   = NULL;
    NBB_BYTE *pucSbmHighOrdPassStart   = NULL;   
    NBB_BYTE *pucSbmLowOrdPassStart = NULL;

    NBB_BYTE *pucPubLimitStart = NULL;
    NBB_BYTE *pucSynClkStart = NULL;
    NBB_BYTE *pucEltrcBoardStart = NULL;
    NBB_BYTE *pucRebthLimitStart = NULL;
    NBB_BYTE *pucMuLimitStart = NULL;
    NBB_BYTE *pucHighOrdPassStart = NULL;
    NBB_BYTE *pucLowOrdPassStart = NULL;

    NBB_BYTE ucPubLimit = 1;
    NBB_BYTE ucSynClk   = 1;
    NBB_BYTE ucEltrcBoard   = 1;
    NBB_BYTE ucRebthLimit  = 1;
    NBB_BYTE ucMuLimit   = 1;
    NBB_BYTE ucHighOrdPass   = 1;
    NBB_BYTE ucLowOrdPass   = 1;
    
    NBB_BUF_SIZE buf_size = 0;
    NBB_BUF_SIZE size_array[OFL_ATG_SBI_SET_CIR_EMU_BOARD + 1];

    ATG_SBI_SET_CIR_EMU_BOARD *pstSetSbiCirEmu = NULL;
    
    NBB_TRC_ENTRY("spm_send_cir_emu_cfg");

    if (pstSetCirEmu == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_send_cir_emu_cfg(pstSetCirEmu==NULL)"));

        OS_PRINTF("***ERROR***:spm_send_cir_emu_cfg(pstSetCirEmu==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_send_cir_emu_cfg(pstSetCirEmu==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        //NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, ucMessage));
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /*判断basic_data是否有数据。                                               */
    /***************************************************************************/ 
    if (pstSetCirEmu->oper_pub_limit == ATG_DCI_OPER_NULL)
    {
        ucPubLimit = 0;
    }

    /***************************************************************************/
    /*判断physical_data是否有数据。                                            */
    /***************************************************************************/  
    if (pstSetCirEmu->oper_syn_clk == ATG_DCI_OPER_NULL)
    {
        ucSynClk = 0;
    }
    
    /***************************************************************************/
    /*判断pos_link_data是否有数据。                                            */
    /***************************************************************************/ 
    if (pstSetCirEmu->oper_eltrc_board == ATG_DCI_OPER_NULL)
    {
        ucEltrcBoard = 0;
    }

    /***************************************************************************/
    /*判断stm1_overhead_data是否有数据。                                       */
    /***************************************************************************/ 
    if (pstSetCirEmu->oper_rebth_limit == ATG_DCI_OPER_NULL)
    {
        ucRebthLimit = 0;
    }

    /***************************************************************************/
    /*判断ces_data是否有数据。                                                 */
    /***************************************************************************/ 
    if (pstSetCirEmu->oper_mu_limit == ATG_DCI_OPER_NULL)
    {
        ucMuLimit = 0;
    }

    /***************************************************************************/
    /*判断cep_overhead_data是否有数据。                                        */
    /***************************************************************************/ 
    if (pstSetCirEmu->oper_high_ord_pass == ATG_DCI_OPER_NULL)
    {
        ucHighOrdPass = 0;
    }
    
    /***************************************************************************/
    /*判断cep_overhead_data是否有数据。                                        */
    /***************************************************************************/ 
    if (pstSetCirEmu->oper_low_ord_pass == ATG_DCI_OPER_NULL)
    {
        ucLowOrdPass = 0;
    }
    
    buf_size = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_SET_CIR_EMU_BOARD))
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_PUB_LIMIT))) * ucPubLimit
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_SYN_CLK))) * ucSynClk
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_ELTRC_BOARD))) * ucEltrcBoard
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_REBTH_LIMIT))) * ucRebthLimit
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_MU_LIMIT))) * ucMuLimit
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_HIGH_ORD_PASS))) * ucHighOrdPass
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_LOW_ORD_PASS))) * ucLowOrdPass;
        
    pstSetSbiCirEmu = (ATG_SBI_SET_CIR_EMU_BOARD *) NBB_GET_BUFFER(NBB_NULL_HANDLE,
                            buf_size, 0, NBB_NORETRY_ACT | NBB_BUF_INTRN);
    
    /***************************************************************************/
    /*判断set_phy_port的值是否有效。*/
    /***************************************************************************/        
    if (pstSetSbiCirEmu == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to alloc set Cir Emu buffer"));
        goto EXIT_LABEL;
    }
  
    /*************************************************************************/
    /* 初始化消息。                                                          */
    /*************************************************************************/
    NBB_ZERO_IPS(pstSetSbiCirEmu);

    /***************************************************************************/
    /* 初始化offsets。                                                         */
    /***************************************************************************/
    size_array[0] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_SET_CIR_EMU_BOARD));
    size_array[1] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_PUB_LIMIT)) * ucPubLimit;
    size_array[2] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_SYN_CLK)) * ucSynClk;
    size_array[3] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_ELTRC_BOARD)) * ucEltrcBoard;
    size_array[4] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_REBTH_LIMIT)) * ucRebthLimit;
    size_array[5] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_MU_LIMIT)) * ucMuLimit;
    size_array[6] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_HIGH_ORD_PASS)) * ucHighOrdPass;
    size_array[7] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_LOW_ORD_PASS)) * ucLowOrdPass;
    size_array[8] = NTL_OFF_SIZE_ARRAY_TERMINATOR;

    NBB_ASSERT(OFL_ATG_SBI_SET_CIR_EMU_BOARD == 8);

    NTL_OFF_INIT_STRUCT(pstSetSbiCirEmu, size_array, off_atg_sbi_set_cir_emu_board);

    pstSetSbiCirEmu->ips_hdr.ips_type     = IPS_ATG_SBI_SET_CIR_EMU_BOARD;
    pstSetSbiCirEmu->delete_struct        = pstSetCirEmu->delete_struct;
    pstSetSbiCirEmu->key.slot             = pstSetCirEmu->key.slot;
    pstSetSbiCirEmu->key.subslot          = pstSetCirEmu->key.subslot;
    pstSetSbiCirEmu->oper_pub_limit       = pstSetCirEmu->oper_pub_limit;
    pstSetSbiCirEmu->oper_syn_clk         = pstSetCirEmu->oper_syn_clk;
    pstSetSbiCirEmu->oper_eltrc_board     = pstSetCirEmu->oper_eltrc_board;
    pstSetSbiCirEmu->oper_rebth_limit     = pstSetCirEmu->oper_rebth_limit;
    pstSetSbiCirEmu->oper_mu_limit        = pstSetCirEmu->oper_mu_limit;
    pstSetSbiCirEmu->oper_high_ord_pass   = pstSetCirEmu->oper_high_ord_pass;
    pstSetSbiCirEmu->oper_low_ord_pass    = pstSetCirEmu->oper_low_ord_pass;

    /***************************************************************************/
    /* 获得指向基础配置的指针。                                                */
    /***************************************************************************/
    if (ucPubLimit == 1)
    {
        pucSbmPubLimitStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                    &(pstSetSbiCirEmu->pub_limit_data));  
        pstSbmPubLimit = (ATG_SBI_CIR_EMU_PUB_LIMIT*)pucSbmPubLimitStart;
        
        pucPubLimitStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                               &pstSetCirEmu->pub_limit_data);
        pstPubLimit = (ATG_DCI_CIR_EMU_PUB_LIMIT*)pucPubLimitStart;

		if ((pstSbmPubLimit != NULL) && (pstPubLimit != NULL))
		{
        	OS_MEMCPY(pstSbmPubLimit, pstPubLimit, sizeof(ATG_SBI_CIR_EMU_PUB_LIMIT));       
        }
    }    
    
    /***************************************************************************/
    /* 获得指向以太网接口物理配置的指针。                                      */
    /***************************************************************************/
    if (ucSynClk == 1)
    {
        pucSbmSynClkStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->syn_clk_data));  
        pstSbmSynClk = (ATG_SBI_CIR_EMU_SYN_CLK*)pucSbmSynClkStart;

        pucSynClkStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                             &pstSetCirEmu->syn_clk_data);          
        pstSynClk = (ATG_DCI_CIR_EMU_SYN_CLK*)pucSynClkStart;

        if ((pstSbmSynClk != NULL) && (pstSynClk != NULL))
        {
        	OS_MEMCPY(pstSbmSynClk, pstSynClk, sizeof(ATG_SBI_CIR_EMU_SYN_CLK));
        }
    }

    /***************************************************************************/
    /* 获得指向POS接口链路层配置的指针。                                       */
    /***************************************************************************/
    if (ucEltrcBoard == 1)
    {
        pucSbmEltrcBoardStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->eltrc_board_data));  
        pstSbmEltrcBoard = (ATG_SBI_CIR_EMU_ELTRC_BOARD*)pucSbmEltrcBoardStart;

        pucEltrcBoardStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                             &pstSetCirEmu->eltrc_board_data);        
        pstEltrcBoard = (ATG_DCI_CIR_EMU_ELTRC_BOARD*)pucEltrcBoardStart;

        if ((pstSbmEltrcBoard != NULL) && (pstEltrcBoard != NULL))
        {
        	OS_MEMCPY(pstSbmEltrcBoard, pstEltrcBoard, sizeof(ATG_SBI_CIR_EMU_ELTRC_BOARD));
        }
    }
    
    /***************************************************************************/
    /* 获得指向STM-1接口开销配置的指针。                                       */
    /***************************************************************************/
    if (ucRebthLimit == 1)
    {
        pucSbmRebthLimitStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                   &(pstSetSbiCirEmu->rebth_limit_data)); 
        pstSbmRebthLimit = (ATG_SBI_CIR_EMU_REBTH_LIMIT*)pucSbmRebthLimitStart;

        pucRebthLimitStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                              &pstSetCirEmu->rebth_limit_data);
        pstRebthLimit = (ATG_DCI_CIR_EMU_REBTH_LIMIT*)pucRebthLimitStart;

        if ((pstSbmRebthLimit != NULL) && (pstRebthLimit != NULL))
        {
        	OS_MEMCPY(pstSbmRebthLimit, pstRebthLimit, sizeof(ATG_SBI_CIR_EMU_REBTH_LIMIT));
        }
    }

    /***************************************************************************/
    /* 获得指向CES接口配置的指针。                                             */
    /***************************************************************************/
    if (ucMuLimit == 1)
    {
        pucSbmMuLimitStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->mu_limit_data)); 
        pstSbmMuLimit = (ATG_SBI_CIR_EMU_MU_LIMIT*)pucSbmMuLimitStart;

        pucMuLimitStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                             &pstSetCirEmu->mu_limit_data);                                                          
        pstMuLimit = (ATG_DCI_CIR_EMU_MU_LIMIT*)pucMuLimitStart;

        if ((pstSbmMuLimit != NULL) && (pstMuLimit != NULL))
        {
        	OS_MEMCPY(pstSbmMuLimit, pstMuLimit, sizeof(ATG_SBI_CIR_EMU_MU_LIMIT));
        }
    }    

    /***************************************************************************/
    /* 获得指向CEP接口开销配置的指针。                                         */
    /***************************************************************************/
    if (ucHighOrdPass == 1)
    {
        pucSbmHighOrdPassStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->high_ord_pass_data)); 
        pstSbmHighOrdPass = (ATG_SBI_CIR_EMU_HIGH_ORD_PASS*)pucSbmHighOrdPassStart;

        pucHighOrdPassStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                          &pstSetCirEmu->high_ord_pass_data);        
        pstHighOrdPass = (ATG_DCI_CIR_EMU_HIGH_ORD_PASS*)pucHighOrdPassStart;

        if ((pstSbmHighOrdPass != NULL) && (pstHighOrdPass != NULL))
        {
        	OS_MEMCPY(pstSbmHighOrdPass, pstHighOrdPass, sizeof(ATG_SBI_CIR_EMU_HIGH_ORD_PASS));
        }
    }

    /***************************************************************************/
    /* 获得指向CEP接口开销配置的指针。                                         */
    /***************************************************************************/
    if (ucLowOrdPass == 1)
    {
        pucSbmLowOrdPassStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->low_ord_pass_data)); 
        pstSbmLowOrdPass = (ATG_SBI_CIR_EMU_LOW_ORD_PASS*)pucSbmLowOrdPassStart;

        pucLowOrdPassStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                          &pstSetCirEmu->low_ord_pass_data);        
        pstLowOrdPass = (ATG_DCI_CIR_EMU_LOW_ORD_PASS*)pucLowOrdPassStart;

        if ((pstSbmLowOrdPass != NULL) && (pstLowOrdPass != NULL))
        {
        	OS_MEMCPY(pstSbmLowOrdPass, pstLowOrdPass, sizeof(ATG_SBI_CIR_EMU_LOW_ORD_PASS));
        }
    }

    if (SHARED.sub_card_cb[ucSubBoardNo-1] != NULL)
    {
        spm_snd_sbi_ips(SHARED.sub_card_cb[ucSubBoardNo-1], (NBB_VOID*)pstSetSbiCirEmu, &(pstSetSbiCirEmu->ips_hdr) NBB_CCXT);
    }    

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
   函 数 名  : spm_send_cir_emu_cfg_ips_from_local
   功能描述  : 初始化存储电路仿真全局配置的树
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年1月27日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_send_cir_emu_cfg_ips_from_local(NBB_BYTE subslot NBB_CCXT_T NBB_CXT)
{

    SPM_CIR_EMU_CB *pstciremu = NULL;
	
    for (pstciremu = (SPM_CIR_EMU_CB*) AVLL_FIRST(SHARED.cir_emu_tree);
         pstciremu != NULL;
         pstciremu = (SPM_CIR_EMU_CB*) AVLL_NEXT(SHARED.cir_emu_tree,
            pstciremu->spm_cir_emu_node))
    {   	
        if(subslot == pstciremu->stCirEmuKey.subslot)
        {
            spm_send_cir_emu_cfg_from_local(pstciremu,subslot NBB_CCXT);
        }
    }
         
    return;
}


/*****************************************************************************
 函 数 名  : spm_send_cir_emu_cfg_from_local
 功能描述  : 把母卡保存的仿真全局配置发生到子卡
 输入参数  : SPM_CIR_EMU_CB cir_emu_cb  
             NBB_BYTE ucSubBoardNo                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年1月24日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_cir_emu_cfg_from_local(SPM_CIR_EMU_CB *cir_emu_cb, NBB_BYTE ucSubBoardNo NBB_CCXT_T NBB_CXT)
{

    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucifexist[100] = {0};
    
    /* 获取的子配置 */
    ATG_SBI_CIR_EMU_PUB_LIMIT *pstSbmPubLimit       = NULL;
    ATG_SBI_CIR_EMU_SYN_CLK *pstSbmSynClk           = NULL;
    ATG_SBI_CIR_EMU_ELTRC_BOARD *pstSbmEltrcBoard      = NULL;
    ATG_SBI_CIR_EMU_REBTH_LIMIT *pstSbmRebthLimit    = NULL;
    ATG_SBI_CIR_EMU_MU_LIMIT *pstSbmMuLimit           = NULL;
    ATG_SBI_CIR_EMU_HIGH_ORD_PASS *pstSbmHighOrdPass      = NULL;
    ATG_SBI_CIR_EMU_LOW_ORD_PASS *pstSbmLowOrdPass      = NULL;    
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucSbmPubLimitStart = NULL;
    NBB_BYTE *pucSbmSynClkStart   = NULL;
    NBB_BYTE *pucSbmEltrcBoardStart   = NULL;
    NBB_BYTE *pucSbmRebthLimitStart  = NULL;
    NBB_BYTE *pucSbmMuLimitStart   = NULL;
    NBB_BYTE *pucSbmHighOrdPassStart   = NULL;   
    NBB_BYTE *pucSbmLowOrdPassStart = NULL;

    NBB_BYTE ucPubLimit = 1;
    NBB_BYTE ucSynClk   = 1;
    NBB_BYTE ucEltrcBoard   = 1;
    NBB_BYTE ucRebthLimit  = 1;
    NBB_BYTE ucMuLimit   = 1;
    NBB_BYTE ucHighOrdPass   = 1;
    NBB_BYTE ucLowOrdPass   = 1;
    
    NBB_BUF_SIZE buf_size = 0;
    NBB_BUF_SIZE size_array[OFL_ATG_SBI_SET_CIR_EMU_BOARD + 1];

    ATG_SBI_SET_CIR_EMU_BOARD *pstSetSbiCirEmu = NULL;
    
    NBB_TRC_ENTRY("spm_send_cir_emu_cfg_from_local");


    if (cir_emu_cb == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_send_cir_emu_cfg(cir_emu_cb==NULL)"));

        OS_PRINTF("***ERROR***:spm_send_cir_emu_cfg(cir_emu_cb==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_send_cir_emu_cfg(cir_emu_cb==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        //NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, ucMessage));
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /*判断basic_data是否有数据。                                               */
    /***************************************************************************/ 
    if (NULL == cir_emu_cb->pub_limit_cb)
    {
        ucPubLimit = 0;
    }

    /***************************************************************************/
    /*判断physical_data是否有数据。                                            */
    /***************************************************************************/  
     if (NULL == cir_emu_cb->syn_clk_cb)
    {
        ucSynClk = 0;
    }
    
    /***************************************************************************/
    /*判断pos_link_data是否有数据。                                            */
    /***************************************************************************/ 
     if (NULL == cir_emu_cb->eltrc_board_cb)
    {
        ucEltrcBoard = 0;
    }

    /***************************************************************************/
    /*判断stm1_overhead_data是否有数据。                                       */
    /***************************************************************************/ 
     if (NULL == cir_emu_cb->rebth_limit_cb)
    {
        ucRebthLimit = 0;
    }

    /***************************************************************************/
    /*判断ces_data是否有数据。                                                 */
    /***************************************************************************/ 
     if (NULL == cir_emu_cb->mu_limit_cb)
    {
        ucMuLimit = 0;
    }

    /***************************************************************************/
    /*判断cep_overhead_data是否有数据。                                        */
    /***************************************************************************/ 
     if (NULL == cir_emu_cb->high_ord_pass_cb)
    {
        ucHighOrdPass = 0;
    }
    
    /***************************************************************************/
    /*判断cep_overhead_data是否有数据。                                        */
    /***************************************************************************/ 
     if (NULL == cir_emu_cb->low_ord_pass_cb)
    {
        ucLowOrdPass = 0;
    }

    buf_size = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_SET_CIR_EMU_BOARD))
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_PUB_LIMIT))) * ucPubLimit
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_SYN_CLK))) * ucSynClk
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_ELTRC_BOARD))) * ucEltrcBoard
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_REBTH_LIMIT))) * ucRebthLimit
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_MU_LIMIT))) * ucMuLimit
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_HIGH_ORD_PASS))) * ucHighOrdPass
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_LOW_ORD_PASS))) * ucLowOrdPass;
        
    pstSetSbiCirEmu = (ATG_SBI_SET_CIR_EMU_BOARD *) NBB_GET_BUFFER(NBB_NULL_HANDLE,
                            buf_size, 0, NBB_NORETRY_ACT | NBB_BUF_INTRN);
    
    /***************************************************************************/
    /*判断set_phy_port的值是否有效。*/
    /***************************************************************************/        
    if (pstSetSbiCirEmu == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to alloc set Cir Emu buffer from local"));
        goto EXIT_LABEL;
    }
  
    /*************************************************************************/
    /* 初始化消息。                                                          */
    /*************************************************************************/
    NBB_ZERO_IPS(pstSetSbiCirEmu);

    /***************************************************************************/
    /* 初始化offsets。                                                         */
    /***************************************************************************/
    size_array[0] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_SET_CIR_EMU_BOARD));
    size_array[1] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_PUB_LIMIT)) * ucPubLimit;
    size_array[2] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_SYN_CLK)) * ucSynClk;
    size_array[3] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_ELTRC_BOARD)) * ucEltrcBoard;
    size_array[4] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_REBTH_LIMIT)) * ucRebthLimit;
    size_array[5] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_MU_LIMIT)) * ucMuLimit;
    size_array[6] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_HIGH_ORD_PASS)) * ucHighOrdPass;
    size_array[7] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_CIR_EMU_LOW_ORD_PASS)) * ucLowOrdPass;
    size_array[8] = NTL_OFF_SIZE_ARRAY_TERMINATOR;

    NBB_ASSERT(OFL_ATG_SBI_SET_CIR_EMU_BOARD == 8);

    NTL_OFF_INIT_STRUCT(pstSetSbiCirEmu, size_array, off_atg_sbi_set_cir_emu_board);

    pstSetSbiCirEmu->ips_hdr.ips_type     = IPS_ATG_SBI_SET_CIR_EMU_BOARD;
    pstSetSbiCirEmu->delete_struct        = FALSE;
    pstSetSbiCirEmu->key.slot             = cir_emu_cb->stCirEmuKey.slot;
    pstSetSbiCirEmu->key.subslot          = cir_emu_cb->stCirEmuKey.subslot;
    
    if(1 == ucPubLimit)
    {
        pstSetSbiCirEmu->oper_pub_limit       = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiCirEmu->oper_pub_limit       = ATG_DCI_OPER_NULL;
    }

    if(1 == ucSynClk)
    {
        pstSetSbiCirEmu->oper_syn_clk       = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiCirEmu->oper_syn_clk       = ATG_DCI_OPER_NULL;
    }

    if(1 == ucEltrcBoard)
    {
        pstSetSbiCirEmu->oper_eltrc_board       = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiCirEmu->oper_eltrc_board       = ATG_DCI_OPER_NULL;
    }

    if(1 == ucRebthLimit)
    {
        pstSetSbiCirEmu->oper_rebth_limit       = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiCirEmu->oper_rebth_limit       = ATG_DCI_OPER_NULL;
    }    

    if(1 == ucMuLimit)
    {
        pstSetSbiCirEmu->oper_mu_limit       = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiCirEmu->oper_mu_limit       = ATG_DCI_OPER_NULL;
    }

    if(1 == ucHighOrdPass)
    {
        pstSetSbiCirEmu->oper_high_ord_pass       = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiCirEmu->oper_high_ord_pass       = ATG_DCI_OPER_NULL;
    }

    if(1 == ucLowOrdPass)
    {
        pstSetSbiCirEmu->oper_low_ord_pass       = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiCirEmu->oper_low_ord_pass       = ATG_DCI_OPER_NULL;
    }     

    /***************************************************************************/
    /* 获得指向基础配置的指针。                                                */
    /***************************************************************************/
    if (ucPubLimit == 1)
    {
        pucSbmPubLimitStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                    &(pstSetSbiCirEmu->pub_limit_data));  
        pstSbmPubLimit = (ATG_SBI_CIR_EMU_PUB_LIMIT*)pucSbmPubLimitStart;

        if ((pstSbmPubLimit != NULL) && (cir_emu_cb->pub_limit_cb != NULL))
        {
            OS_MEMCPY(pstSbmPubLimit, cir_emu_cb->pub_limit_cb, sizeof(ATG_SBI_CIR_EMU_PUB_LIMIT));       
        }
    }    
    
    /***************************************************************************/
    /* 获得指向以太网接口物理配置的指针。                                      */
    /***************************************************************************/
    if (ucSynClk == 1)
    {
        pucSbmSynClkStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->syn_clk_data));  
        pstSbmSynClk = (ATG_SBI_CIR_EMU_SYN_CLK*)pucSbmSynClkStart;

        //pucSynClkStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                             //&pstSetCirEmu->syn_clk_data);          
        //pstSynClk = (ATG_DCI_CIR_EMU_SYN_CLK*)pucSynClkStart;

        if ((pstSbmSynClk != NULL) && (cir_emu_cb->syn_clk_cb != NULL))
        {
        	OS_MEMCPY(pstSbmSynClk, cir_emu_cb->syn_clk_cb, sizeof(ATG_SBI_CIR_EMU_SYN_CLK));
        }
    }

    /***************************************************************************/
    /* 获得指向POS接口链路层配置的指针。                                       */
    /***************************************************************************/
    if (ucEltrcBoard == 1)
    {
        pucSbmEltrcBoardStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->eltrc_board_data));  
        pstSbmEltrcBoard = (ATG_SBI_CIR_EMU_ELTRC_BOARD*)pucSbmEltrcBoardStart;

        //pucEltrcBoardStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                             //&pstSetCirEmu->eltrc_board_data);        
        //pstEltrcBoard = (ATG_DCI_CIR_EMU_ELTRC_BOARD*)pucEltrcBoardStart;

        if ((pstSbmEltrcBoard != NULL) && (cir_emu_cb->eltrc_board_cb != NULL))
        {
        	OS_MEMCPY(pstSbmEltrcBoard, cir_emu_cb->eltrc_board_cb, sizeof(ATG_SBI_CIR_EMU_ELTRC_BOARD));
        }
    }
    
    /***************************************************************************/
    /* 获得指向STM-1接口开销配置的指针。                                       */
    /***************************************************************************/
    if (ucRebthLimit == 1)
    {
        pucSbmRebthLimitStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                   &(pstSetSbiCirEmu->rebth_limit_data)); 
        pstSbmRebthLimit = (ATG_SBI_CIR_EMU_REBTH_LIMIT*)pucSbmRebthLimitStart;

        //pucRebthLimitStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                              //&pstSetCirEmu->rebth_limit_data);
        //pstRebthLimit = (ATG_DCI_CIR_EMU_REBTH_LIMIT*)pucRebthLimitStart;

        if ((pstSbmRebthLimit != NULL) && (cir_emu_cb->rebth_limit_cb != NULL))

        {
        	OS_MEMCPY(pstSbmRebthLimit, cir_emu_cb->rebth_limit_cb, sizeof(ATG_SBI_CIR_EMU_REBTH_LIMIT));
        }
    }

    /***************************************************************************/
    /* 获得指向CES接口配置的指针。                                             */
    /***************************************************************************/
    if (ucMuLimit == 1)
    {
        pucSbmMuLimitStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->mu_limit_data)); 
        pstSbmMuLimit = (ATG_SBI_CIR_EMU_MU_LIMIT*)pucSbmMuLimitStart;

        //pucMuLimitStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
        //&pstSetCirEmu->mu_limit_data);
        //pstMuLimit = (ATG_DCI_CIR_EMU_MU_LIMIT*)pucMuLimitStart;

        if ((pstSbmMuLimit != NULL) && (cir_emu_cb->mu_limit_cb != NULL))
        {
        	OS_MEMCPY(pstSbmMuLimit, cir_emu_cb->mu_limit_cb, sizeof(ATG_SBI_CIR_EMU_MU_LIMIT));
        }
    }    

    /***************************************************************************/
    /* 获得指向CEP接口开销配置的指针。                                         */
    /***************************************************************************/
    if (ucHighOrdPass == 1)
    {
        pucSbmHighOrdPassStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->high_ord_pass_data)); 
        pstSbmHighOrdPass = (ATG_SBI_CIR_EMU_HIGH_ORD_PASS*)pucSbmHighOrdPassStart;

        //pucHighOrdPassStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                          //&pstSetCirEmu->high_ord_pass_data);        
        //pstHighOrdPass = (ATG_DCI_CIR_EMU_HIGH_ORD_PASS*)pucHighOrdPassStart;

        if ((pstSbmHighOrdPass != NULL) && (cir_emu_cb->high_ord_pass_cb != NULL))
        {
        	OS_MEMCPY(pstSbmHighOrdPass, cir_emu_cb->high_ord_pass_cb, sizeof(ATG_SBI_CIR_EMU_HIGH_ORD_PASS));
        }
    }

    /***************************************************************************/
    /* 获得指向CEP接口开销配置的指针。                                         */
    /***************************************************************************/
    if (ucLowOrdPass == 1)
    {
        pucSbmLowOrdPassStart = NTL_OFFLEN_GET_POINTER(pstSetSbiCirEmu,
                                                  &(pstSetSbiCirEmu->low_ord_pass_data)); 
        pstSbmLowOrdPass = (ATG_SBI_CIR_EMU_LOW_ORD_PASS*)pucSbmLowOrdPassStart;

        //pucLowOrdPassStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetCirEmu, 
                                                          //&pstSetCirEmu->low_ord_pass_data);        
        //pstLowOrdPass = (ATG_DCI_CIR_EMU_LOW_ORD_PASS*)pucLowOrdPassStart;

        if ((pstSbmLowOrdPass != NULL) && (cir_emu_cb->low_ord_pass_cb != NULL))
        {
        	OS_MEMCPY(pstSbmLowOrdPass, cir_emu_cb->low_ord_pass_cb, sizeof(ATG_SBI_CIR_EMU_LOW_ORD_PASS));
        }
    }

    if (SHARED.sub_card_cb[ucSubBoardNo - 1] != NULL)
    {
        spm_snd_sbi_ips(SHARED.sub_card_cb[ucSubBoardNo - 1], 
            (NBB_VOID*)pstSetSbiCirEmu, &(pstSetSbiCirEmu->ips_hdr) NBB_CCXT);
    }    

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
    
}


/*****************************************************************************
 函 数 名  : spm_init_cir_emu_cb
 功能描述  : 初始化电路仿真盘全局配置结构体
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月24日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
/*NBB_VOID spm_init_cir_emu_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_cir_emu_cb");
    
    //avll_key_offset = NBB_OFFSETOF(SPM_SWITCH_VC_CB, switch_vc_key);

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    OS_MEMSET(&(SHARED.cir_emu_cb), 0, sizeof(SPM_CIR_EMU_CB));

    NBB_TRC_EXIT();    
}*/

/*****************************************************************************
   函 数 名  : spm_init_cir_emu_tree
   功能描述  : 初始化存储电路仿真全局配置的树
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年1月27日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_cir_emu_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_cir_emu_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_CIR_EMU_CB, stCirEmuKey);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.cir_emu_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_CIR_EMU_CB, spm_cir_emu_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   函 数 名  : spm_alloc_cir_emu_cb
   功能描述  : VPWS结构内存空间分配
   输入参数  : NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  : SPM_VPWS_CB
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年1月27日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
SPM_CIR_EMU_CB *spm_alloc_cir_emu_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_CIR_EMU_CB *pstciremu = NULL;

    NBB_TRC_ENTRY("spm_alloc_cir_emu_cb");

    /* 分配一个新的VPWS配置条目。*/
    pstciremu = (SPM_CIR_EMU_CB *)NBB_MM_ALLOC(sizeof(SPM_CIR_EMU_CB), NBB_NORETRY_ACT, MEM_SPM_CIR_EMU_CB);
    
    if (pstciremu == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:申请cir_emu条目空间为NULL!"));
        goto EXIT_LABEL;
    }

    /* 初始化VPWS配置条目 */
    OS_MEMSET(pstciremu, 0, sizeof(SPM_CIR_EMU_CB));
    
    pstciremu->pub_limit_cb = NULL;
    pstciremu->syn_clk_cb = NULL;
    pstciremu->eltrc_board_cb = NULL;
    pstciremu->rebth_limit_cb = NULL;
    pstciremu->mu_limit_cb =  NULL;
    pstciremu->high_ord_pass_cb = NULL;
    pstciremu->low_ord_pass_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    //pstciremu->spm_cir_emu_handle = NBB_CREATE_HANDLE(pstciremu, HDL_SPM_VPWS_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPWS_CB allocated at %p with handle %#lx",
            //pstVpws, pstVpws->spm_vpws_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstciremu->spm_cir_emu_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstciremu);
}

/*****************************************************************************
   函 数 名  : spm_free_cir_emu_cb
   功能描述  : 释放VPWS结构体的内存
   输入参数  : SPM_VPWS_CB *pstVpws NBB_CCXT_T NBB_CXT
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年1月27日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_cir_emu_cb(SPM_CIR_EMU_CB *pstciremu NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_cir_emu_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstciremu, sizeof(SPM_CIR_EMU_CB), MEM_SPM_CIR_EMU_CB);

    if (pstciremu == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/

    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pstVpws));
    NBB_TRC_FLOW((NBB_FORMAT "Free cir_emu_id %d", pstciremu->stCirEmuKey));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstciremu->pub_limit_cb  != NULL)
    {
        NBB_MM_FREE(pstciremu->pub_limit_cb, MEM_SPM_CIR_EMU_PUB_LIMIT_CB);
        pstciremu->pub_limit_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstciremu->syn_clk_cb  != NULL)
    {
        NBB_MM_FREE(pstciremu->syn_clk_cb, MEM_SPM_CIR_EMU_SYN_CLK_CB);
        pstciremu->syn_clk_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstciremu->eltrc_board_cb  != NULL)
    {
        NBB_MM_FREE(pstciremu->eltrc_board_cb, MEM_SPM_CIR_EMU_ELTRC_BOARD_CB);
        pstciremu->eltrc_board_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstciremu->rebth_limit_cb  != NULL)
    {
        NBB_MM_FREE(pstciremu->rebth_limit_cb, MEM_SPM_CIR_EMU_REBTH_LIMIT_CB);
        pstciremu->rebth_limit_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstciremu->mu_limit_cb  != NULL)
    {
        NBB_MM_FREE(pstciremu->mu_limit_cb, MEM_SPM_CIR_EMU_MU_LIMIT_CB);
        pstciremu->mu_limit_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstciremu->high_ord_pass_cb  != NULL)
    {
        NBB_MM_FREE(pstciremu->high_ord_pass_cb, MEM_SPM_CIR_EMU_HIGH_ORD_PASS_CB);
        pstciremu->high_ord_pass_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstciremu->low_ord_pass_cb  != NULL)
    {
        NBB_MM_FREE(pstciremu->low_ord_pass_cb, MEM_SPM_CIR_EMU_LOW_ORD_PASS_CB);
        pstciremu->low_ord_pass_cb = NULL;
    }
    
    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstciremu, MEM_SPM_CIR_EMU_CB);
    pstciremu = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_cir_emu_cb
   功能描述  : 释放vpws配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年1月27日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_all_cir_emu_cb(NBB_CXT_T NBB_CXT)
{
	SPM_CIR_EMU_CB *pstciremu = NULL;
	
    for (pstciremu = (SPM_CIR_EMU_CB*) AVLL_FIRST(SHARED.cir_emu_tree);
         pstciremu != NULL;
         pstciremu = (SPM_CIR_EMU_CB*) AVLL_FIRST(SHARED.cir_emu_tree))
    {
    	
        AVLL_DELETE(SHARED.cir_emu_tree, pstciremu->spm_cir_emu_node);     	
        spm_free_cir_emu_cb(pstciremu NBB_CCXT);
    }
}

