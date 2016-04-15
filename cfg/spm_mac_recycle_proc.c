/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_mac_recycle_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年4月18日
  最近修改   :
  功能描述   : 处理MAC地址撤销配置
  函数列表   :
              spm_init_mac_recycle_cb
              spm_rcv_dci_cmd_mac_withdraw
  修改历史   :
  1.日    期   : 2013年4月12日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char mac_recycle_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_cmd_mac_withdraw
 功能描述  : 处理MAC地址撤销命令的IPS消息
 输入参数  : ATG_DCI_CMD_MAC_WITHDRAW *pstCmdMacWithdraw  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月18日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_cmd_mac_withdraw(ATG_DCI_CMD_MAC_WITHDRAW *pstCmdMacWithdraw NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;

    ATG_DCI_MAC_WITHDRAW_DATA *pstBasicData = NULL;
    
    NBB_BYTE *pucBasicDataStart = NULL;
    
    NBB_ULONG ulOperBasic       = ATG_DCI_OPER_NULL;
    
    NBB_ULONG ulMacRecycleIdKey = 0;
    NBB_BYTE ucC3Unit = 0;
    
    SPM_VPLS_CB *pstVpls = NULL;
    NBB_USHORT vplsid = 0; 
    
    //NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_cmd_mac_withdraw");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstCmdMacWithdraw != NULL);

    if (pstCmdMacWithdraw == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_cmd_mac_withdraw(pstCmdMacWithdraw==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_cmd_mac_withdraw(pstCmdMacWithdraw==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_cmd_mac_withdraw(pstCmdMacWithdraw==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        goto EXIT_LABEL;
    }

    ulMacRecycleIdKey = pstCmdMacWithdraw->key;

    /* 获取子配置操作 */
    ulOperBasic = pstCmdMacWithdraw->oper_mac_withdraw;
    
    /**********************************************************************/
    /*                              获取配置                              */
    /**********************************************************************/
    if (SPM_PRINT_CFG == mac_recycle_cfg_print_setting)
    {
        printf("\n#############MAC地址撤销命令(%s) #############\n", "ADD");
    }

    //记录处理过程
    OS_SPRINTF(ucMessage,"#############MAC地址撤销命令(%s) #############\n",
        "ADD");
    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage); 
    
    /***************************************************************************/
    /*                       逐条处理                               */
    /***************************************************************************/
    /* 基本配置 */
    if (ulOperBasic == ATG_DCI_OPER_ADD)
    {

        /* 计算第一个entry的地址。*/
        pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstCmdMacWithdraw, &pstCmdMacWithdraw->mac_withdraw_data);    

        /* 如果指针为NULL，无数据 */
        if (pucBasicDataStart == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  MAC RECYCLE basic_data is NULL."));   
            NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
        }
        else
        {
            pstBasicData = (ATG_DCI_MAC_WITHDRAW_DATA*)pucBasicDataStart;

            if (mac_recycle_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  1）基本配置\n");
                spm_dbg_print_mac_recycle_basic_cfg(pstBasicData);
            }

            OS_SPRINTF(ucMessage,"  1）基本配置\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_mac_recycle_basic_cfg(pstBasicData);

            //打印配置
            NBB_TRC_DETAIL((NBB_FORMAT "  VPLS ID=%d", pstBasicData->vpls_id));

#ifdef SPU

            for (pstVpls = (SPM_VPLS_CB *)AVLL_FIRST(v_spm_shared->vpls_tree);
                pstVpls != NULL;
                pstVpls = (SPM_VPLS_CB *)AVLL_NEXT(v_spm_shared->vpls_tree,
                    pstVpls->spm_vpls_node))                                        
           {

                    if((NULL != pstVpls->basic_cfg_cb) && (pstBasicData->vpls_id == pstVpls->basic_cfg_cb->vsi_id))
                    {
                        vplsid = pstVpls->vpls_id_key;
                        break;
                    }              
            }
                
            if(0 < vplsid) 
            {
                for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
                {
                    ret += ApiDeleteMacEntryByVlan(ucC3Unit, vplsid, 1);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDeleteMacEntryByVlan() ret=%d", ret));                     
                }
                
                if (SUCCESS != ret)
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDeleteMacEntryByVlan ret=%d", ret));
                    OS_PRINTF("***ERROR***:ApiDeleteMacEntryByVlan() ret=%d\n", ret);

                    //pstCmdMacWithdraw->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    OS_SPRINTF(ucMessage, "***ERROR***:ApiDeleteMacEntryByVlan() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    goto EXIT_LABEL;
                }                
            }
                      
#endif

            if (ret == SUCCESS)
            {
                /* 保存数据 */                    
                NBB_MEMCPY(&(SHARED.mac_recycle_cb.basic_cfg_cb), pstBasicData, sizeof(ATG_DCI_MAC_WITHDRAW_DATA));
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
NBB_VOID spm_init_mac_recycle_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_mac_recycle_cb");
    
    //avll_key_offset = NBB_OFFSETOF(SPM_SWITCH_VC_CB, switch_vc_key);

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    OS_MEMSET(&(SHARED.mac_recycle_cb), 0, sizeof(SPM_MAC_RECYCLE_CB));

    NBB_TRC_EXIT();    
}



