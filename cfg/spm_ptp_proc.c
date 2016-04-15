/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_ptp_proc.c
   版 本 号   : 初稿
   作    者   : zengliang
   生成日期   : 2013年6月28日
   最近修改   : 无
   功能描述   : 时间同步基本配置处理
   函数列表   : 略
   修改历史   :
   1.日    期   : 2013年6月28日
     作    者   : zengliang
     修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char clk_sync_base_print_setting;

#ifdef SRC
/*1588 APPL函数接口外部声明*/
extern NBB_LONG spm_ptp_set_global_config(IN ATG_DCI_CLK_SYNC_PARA *pstSyncParaData, \
                                                IN ATG_DCI_LOCAL_CLK *pstLocalClkData, \
                                                IN ATG_DCI_PPS_TOD_IF_DATA *pstPpstodIfData);
extern NBB_LONG spm_ptp_set_port_config(IN NBB_BYTE ucSlot, IN NBB_BYTE ucSubBoard, IN NBB_BYTE ucPhyPortNum, IN NBB_BYTE ucLogicPortNum, IN ATG_DCI_PHY_PORT_TIME_SYNC_DATA *pstPortCfg);
extern NBB_LONG spm_ptp_delete_port_config(IN NBB_BYTE ucSlot, IN NBB_BYTE ucSubBoard, IN NBB_BYTE ucPhyPortNum, IN NBB_BYTE ucLogicPortNum);
extern NBB_INT spm_ptp_main_init();
#endif

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_clk_sync_base
 功能描述  : 处理PTP IPS消息
 输入参数  : ATG_DCI_SET_CLK_SYNC_BASE *pstSetClkSyncBase  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月18日
    作    者   : zengliang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_clk_sync_base(ATG_DCI_SET_CLK_SYNC_BASE *pstSetClkSyncBase)
{
    //NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    NBB_ULONG ulPtpKey = 0;
    
    /* 获取的子配置 */
    ATG_DCI_CLK_SYNC_PARA *pstSyncParaData = NULL;
    ATG_DCI_LOCAL_CLK *pstLocalClkData = NULL;
    ATG_DCI_PPS_TOD_IF_DATA *pstPpstodIfData = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucSyncParaDataStart = NULL;
    NBB_BYTE *pucLocalClkDataStart = NULL;
    NBB_BYTE *pucPpstodIfDataStart = NULL;

    /* 子配置的操作模式 */
    NBB_ULONG ulOperSyncPara = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLocalClk = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPpstodIf = ATG_DCI_OPER_NULL;

    /* 获取的子配置返回值*/
    NBB_LONG lSyncParaRc = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_LONG lLocalClkRc = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_LONG lPpstodIfRc = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_rcv_dci_set_clk_sync_base");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetClkSyncBase != NULL);

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetClkSyncBase->return_code = ATG_DCI_RC_OK;

    ulPtpKey = pstSetClkSyncBase->key;
    NBB_TRC_DETAIL((NBB_FORMAT "ulPtpKey = %ld", ulPtpKey));

#ifdef SRC

    /* 获取子配置操作 */
    ulOperSyncPara = pstSetClkSyncBase->oper_clk_sync_para;
    ulOperLocalClk = pstSetClkSyncBase->oper_local_clk;
    ulOperPpstodIf = pstSetClkSyncBase->oper_pps_tod_if;

    /* 删除整个条目 */
    if (pstSetClkSyncBase->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "删除所有的时间同步基本配置"));
        spm_dbg_print_ptp_cfg_head(ulPtpKey, SPM_OPER_DEL);
        spm_dbg_record_ptp_cfg_head(ulPtpKey, SPM_OPER_DEL);
        
        /*清空原有配置*/
	 OS_MEMSET(&(SHARED.ptp_cb), 0, sizeof(SPM_PTP_CB));

        /* 如果条目不存在，不删除 */
        //if (ucIfExist == ATG_DCI_UNEXIST)
        //{
            /* 待修改：加打印PD */
            //NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:要删除的时间同步基本配置并不存在"));
            //pstSetClkSyncBase->return_code = ATG_DCI_RC_OK;

            //goto EXIT_LABEL;
        //}
        /* 存在删除 */
        //else
        //{
            /* todo list */
        //}
        
    }
    /* 增加或更新条目 */
    else
    {
		/*清空原有配置*/
		//OS_MEMSET(&(SHARED.ptp_cb), 0, sizeof(SPM_PTP_CB));
	 if(ATG_DCI_UNEXIST == SHARED.ptp_cb.ptp_id_key)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "增加时间同步基本配置"));
            spm_dbg_print_ptp_cfg_head(ulPtpKey, SPM_OPER_ADD);
            spm_dbg_record_ptp_cfg_head(ulPtpKey, SPM_OPER_ADD);	            
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "更新时间同步基本配置"));
            spm_dbg_print_ptp_cfg_head(ulPtpKey, SPM_OPER_UPD);
            spm_dbg_record_ptp_cfg_head(ulPtpKey, SPM_OPER_UPD);	            
        }
	
        SHARED.ptp_cb.ptp_id_key = ulPtpKey;
            
        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/
        /******************************** 同步参数配置 *********************************/
        if (ulOperSyncPara == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucSyncParaDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetClkSyncBase,
                &pstSetClkSyncBase->clk_sync_para);

            /* 首地址为NULL，异常 */
            if (pucSyncParaDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "pucSyncParaDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstSyncParaData = (ATG_DCI_CLK_SYNC_PARA *)pucSyncParaDataStart;
                if (clk_sync_base_print_setting == SPM_PRINT_CFG)
                {
                    spm_dbg_print_sync_para_cfg(pstSyncParaData);
                }
                
                memcpy(&(SHARED.ptp_cb.sync_para_ptp_cb), pstSyncParaData, sizeof(ATG_DCI_CLK_SYNC_PARA));

				lSyncParaRc = ATG_DCI_RC_OK;
            }
        }
        else if (ulOperSyncPara == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperSyncPara == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 本地时钟配置 ********************************/
        if (ulOperLocalClk == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucLocalClkDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetClkSyncBase,
                &pstSetClkSyncBase->local_clk);

            /* 首地址为NULL，异常 */
            if (pucLocalClkDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "pucLocalClkDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstLocalClkData = (ATG_DCI_LOCAL_CLK *)pucLocalClkDataStart;
                if (clk_sync_base_print_setting == SPM_PRINT_CFG)
                {
                    spm_dbg_print_local_clk_cfg(pstLocalClkData);
                }
                
                memcpy(&(SHARED.ptp_cb.local_clk_ptp_cb), pstLocalClkData, sizeof(ATG_DCI_LOCAL_CLK));

				lLocalClkRc = ATG_DCI_RC_OK;
            }
        }
        else if (ulOperLocalClk == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperLocalClk == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** PPS+TOD接口配置 ********************************/
        if (ulOperPpstodIf == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucPpstodIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetClkSyncBase,
                &pstSetClkSyncBase->pps_tod_if);

            /* 首地址为NULL，异常 */
            if (pucPpstodIfDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "pucPpstodIfDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstPpstodIfData = (ATG_DCI_PPS_TOD_IF_DATA*)pucPpstodIfDataStart;
                if (clk_sync_base_print_setting == SPM_PRINT_CFG)
                {
                    spm_dbg_print_pps_tod_if_cfg(pstPpstodIfData);
                }
                
                memcpy(&(SHARED.ptp_cb.pps_tod_if_ptp_cb), pstPpstodIfData, sizeof(ATG_DCI_PPS_TOD_IF_DATA));
                
				lPpstodIfRc = ATG_DCI_RC_OK;
            }
        }
        else if (ulOperPpstodIf == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperPpstodIf == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /*配置全部获取成功*/
		if ((lSyncParaRc == ATG_DCI_RC_OK) || (lLocalClkRc == ATG_DCI_RC_OK) || (lPpstodIfRc == ATG_DCI_RC_OK))
		{
    		pstSetClkSyncBase->return_code = spm_ptp_set_global_config(pstSyncParaData, pstLocalClkData, pstPpstodIfData);
		}
		else
		{
    		pstSetClkSyncBase->return_code = ATG_DCI_RC_UNSUCCESSFUL;
		}
    }
#endif
    
	EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/**************************************************************************
    函数名: spm_ptp_set_port
    功能描述: 更新端口配置
    输入参数: ucSlot-->槽位号
    		  ucSubBoard-->子卡号
    		  ucPhyPortNum-->面板端口号
    		  ucLogicPortNum-->逻辑端口号
    		  pstPortCfg-->端口配置
    输出参数: 无
    返回值: ATG_DCI_RC_OK/ATG_DCI_RC_UNSUCCESSFUL
    调用函数: 无
    被调函数: 无
    
    修改历史: 
    1. 日期:2013年6月8日
       作者:zengliang
       修改内容:生成新函数
    
**************************************************************************/
NBB_LONG spm_ptp_set_port(NBB_BYTE ucSlot, NBB_BYTE ucSubBoard, NBB_BYTE ucPhyPortNum, NBB_BYTE ucLogicPortNum, ATG_DCI_PHY_PORT_TIME_SYNC_DATA *pstPortCfg)
{
    NBB_LONG lRc = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();    
    NBB_TRC_ENTRY("spm_ptp_set_port");

#ifdef SRC
	lRc = spm_ptp_set_port_config(ucSlot, ucSubBoard, ucPhyPortNum, ucLogicPortNum, pstPortCfg);
#endif
    
	NBB_TRC_EXIT();

    return lRc;
}

/**************************************************************************
    函数名: spm_ptp_delete_port
    功能描述: 删除端口配置
    输入参数: ucSlot-->槽位号
    		  ucSubBoard-->子卡号
    		  ucPhyPortNum-->面板端口号
    		  ucLogicPortNum-->逻辑端口号
    输出参数: 无
    返回值: ATG_DCI_RC_OK/ATG_DCI_RC_UNSUCCESSFUL
    调用函数: 无
    被调函数: 无
    
    修改历史: 
    1. 日期:2013年6月8日
       作者:zengliang
       修改内容:生成新函数
    
**************************************************************************/
NBB_LONG spm_ptp_delete_port(NBB_BYTE ucSlot, NBB_BYTE ucSubBoard, NBB_BYTE ucPhyPortNum, NBB_BYTE ucLogicPortNum)
{
    NBB_LONG lRc = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();    
    NBB_TRC_ENTRY("spm_ptp_delete_port");

#ifdef SRC
	lRc = spm_ptp_delete_port_config(ucSlot, ucSubBoard, ucPhyPortNum, ucLogicPortNum);
#endif
    
	NBB_TRC_EXIT();

    return lRc;
}

/*****************************************************************************
 函 数 名  : spm_init_ptp
 功能描述  : 初始化1588配置结构体
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
    1. 日期:2013年6月8日
       作者:zengliang
       修改内容:生成新函数

*****************************************************************************/
NBB_VOID spm_init_ptp()
{
    //NBB_TRC_ENTRY("spm_init_ptp");
    
#ifdef SRC
    spm_ptp_main_init();
#endif

	//NBB_TRC_EXIT();
}

