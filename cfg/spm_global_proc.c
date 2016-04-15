/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_global_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年6月24日
  最近修改   :
  功能描述   : 全局配置处理
  函数列表   :
              spm_init_global_cb
              spm_rcv_dci_set_global
  修改历史   :
  1.日    期   : 2013年6月24日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char global_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_global
 功能描述  : 全局配置IPS消息处理函数
 输入参数  : ATG_DCI_SET_GLOB_CONF *pstSetGlobal  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月24日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

  2.日    期   : 2015年6月12日
    作    者   : gaos
    修改内容   : 修改OAM_SD全局配置接收
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_global(ATG_DCI_SET_GLOB_CONF *pstSetGlobal NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
    NBB_INT i = 0;
    NBB_BYTE ucC3Unit = 0;
    NBB_BYTE maclimit = 3;

    //SPM_GLOBAL_CB *pstGlobal = NULL;
    ATG_DCI_GLOB_CONF_BASIC_DATA *pstBasicData = NULL;
    ATG_DCI_GLOB_CONF_THRES_DATA *pstThresData = NULL;
    ATG_DCI_GLOB_CONF_OAM_DATA *pstOamData = NULL;

    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucThresStart = NULL;
    NBB_BYTE *pucOamStrart = NULL;
    
    NBB_ULONG ulOperBasic       = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperThres = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperOam = ATG_DCI_OPER_NULL;
    
    NBB_ULONG ulGlobalIdKey = 0;

    //NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_global");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetGlobal != NULL);

	if (pstSetGlobal == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_global(pstSetGlobal==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_global(pstSetGlobal==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_global(pstSetGlobal==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        goto EXIT_LABEL;
    }
    
    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetGlobal->return_code = ATG_DCI_RC_OK;

    ulGlobalIdKey = pstSetGlobal->key;

    /* 获取子配置操作 */
    ulOperBasic = pstSetGlobal->oper_basic;
    ulOperThres = pstSetGlobal->oper_thres;
    ulOperOam = pstSetGlobal->oper_oam;    
    
    /* 删除整个条目 */
    if (pstSetGlobal->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除ID=%ld 的全局配置", ulGlobalIdKey));
        spm_dbg_print_global_head(ulGlobalIdKey, SPM_OPER_DEL);
        spm_dbg_record_global_head(ulGlobalIdKey, SPM_OPER_DEL);
    }
    /* 增加或更新条目 */
    else
    {     
        spm_dbg_print_global_head(ulGlobalIdKey, SPM_OPER_ADD);
        spm_dbg_record_global_head(ulGlobalIdKey, SPM_OPER_ADD);
        
        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/
        /* 基本配置 */
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetGlobal, &pstSetGlobal->basic_data);    

            /* 如果指针为NULL，无数据 */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  GLOBAL basic_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstBasicData = (ATG_DCI_GLOB_CONF_BASIC_DATA*)pucBasicDataStart;

                if (global_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）基本配置\n");
                    spm_dbg_print_global_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1）基本配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_global_basic_cfg(pstBasicData);

                //打印配置
                NBB_TRC_DETAIL((NBB_FORMAT "  MAC老化时间=%d(100ms)", pstBasicData->mac_ageing_time));

#ifdef SRC
				//MAC老化时间设置
                ret = SUCCESS;
                
                ret = ApiMacSetMacAgeTime(pstBasicData->mac_ageing_time/10);

                if (ret != SUCCESS)
                {
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiMacSetMacAgeTime() ret=%d", ret));

        		    OS_PRINTF("***ERROR***:ApiMacSetMacAgeTime() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:ApiMacSetMacAgeTime() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | GLOBAL_PD, 0, "ld d s s", ulGlobalIdKey, ret, "CALL_C3_FUNC_ERROR", ucMessage));
        		    
        		    //pstSetGlobal->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		}
        		
                maclimit = pstBasicData->mac_flow_limit;
                
                for(i = 0; i < 8;i++)
                {
                    ret = SUCCESS;
                    ret = ApiAradSetPortQueueCirEir(0,0,i,0,maclimit * 1024,1024,1024);
                    
                    if (ret != SUCCESS)
                    {
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, "Line=%d i=%d ret=%d \n", __LINE__, i,ret);
                    }                
                
                }
        		
#endif

#ifdef SPU

               //在ER版本把MAC地址老化时间设置改在业务盘fansongbo 2014.10.15
                //MAC老化时间设置
                ret = SUCCESS;
                for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
                {
                	ret += ApiC3SetAgeingTime(ucC3Unit,pstBasicData->mac_ageing_time/10);
                }

                if (ret != SUCCESS)
                {
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetAgeingTime() ret=%d", ret));

        		    OS_PRINTF("***ERROR***:ApiC3SetAgeingTime() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:ApiC3SetAgeingTime() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | GLOBAL_PD, 0, "ld d s s", ulGlobalIdKey, ret, "CALL_C3_FUNC_ERROR", ucMessage));
        		    
        		    //pstSetGlobal->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		}
                
                //以太网帧间隔设置
                ret = SUCCESS;

                for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
                {
                	ret += spm_set_Interframegap(pstBasicData->frame_interval);
                }

                if (ret != SUCCESS)
                {
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_set_Interframegap() ret=%d", ret));

        		    OS_PRINTF("***ERROR***:spm_set_Interframegap() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:spm_set_Interframegap() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | GLOBAL_PD, 0, "ld d s s", ulGlobalIdKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    //pstSetGlobal->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		}

#endif 
                /* 保存数据 */                    
                OS_MEMCPY(&(SHARED.global_cb.basic_cfg_cb), pstBasicData, sizeof(ATG_DCI_GLOB_CONF_BASIC_DATA));

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
        
		/*********************** 门限配置 *******************/
        if (ulOperThres == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucThresStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetGlobal, &pstSetGlobal->thres_data);    

            /* 如果指针为NULL，无数据 */
            if (pucThresStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  GLOBAL thres_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstThresData = (ATG_DCI_GLOB_CONF_THRES_DATA *)pucThresStart;

                if (global_cfg_print_setting == SPM_PRINT_CFG)
                {
                    OS_PRINTF("  2）门限配置\n");
                    spm_dbg_print_global_thres_cfg(pstThresData);
                }

                OS_SPRINTF(ucMessage,"  2）门限配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                spm_dbg_record_global_thres_cfg(pstThresData);
                spm_init_sd_cfg(pstThresData->sd_thres,pstThresData->sd_window);

                //保存配置
                OS_MEMCPY(&(SHARED.global_cb.thres_data), pstThresData, sizeof(ATG_DCI_GLOB_CONF_THRES_DATA));
                
            }
        }

        
        /*********************** OAM配置 *******************/
        if (ulOperOam == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucOamStrart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetGlobal, &pstSetGlobal->oam_data);    

            /* 如果指针为NULL，无数据 */
            if (pucOamStrart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  GLOBAL oam_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstOamData = (ATG_DCI_GLOB_CONF_OAM_DATA *)pucOamStrart;

                if (global_cfg_print_setting == SPM_PRINT_CFG)
                {
                    OS_PRINTF("  2）OAM配置\n");
                    spm_dbg_print_global_oam_cfg(pstOamData);
                }

                OS_SPRINTF(ucMessage,"  2）OAM配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                spm_dbg_record_global_oam_cfg(pstOamData);

                spm_init_channeltype_fpga(pstOamData->ChannelType);
                
                //保存配置
                OS_MEMCPY(&(SHARED.global_cb.oam_data), pstOamData, sizeof(ATG_DCI_GLOB_CONF_OAM_DATA));
                
                
            }
        }
    
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_init_global_cb
 功能描述  : 初始化全局配置结构体
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月12日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_global_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_global_cb");
    
    //avll_key_offset = NBB_OFFSETOF(SPM_SWITCH_VC_CB, switch_vc_key);

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    OS_MEMSET(&(SHARED.global_cb), 0, sizeof(SPM_GLOBAL_CB));

    NBB_TRC_EXIT();    
}

/*****************************************************************************
   函 数 名  : spm_free_all_global_cb
   功能描述  : 释放全局配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2014年05月19日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_all_global_cb(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_init_global_cb");

    OS_MEMSET(&(SHARED.global_cb), 0, sizeof(SPM_GLOBAL_CB));

    NBB_TRC_EXIT();   
}


