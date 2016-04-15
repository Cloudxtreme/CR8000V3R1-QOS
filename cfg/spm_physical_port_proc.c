/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_cfg_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2012年9月11日
  最近修改   :
  功能描述   : 配置块数据处理
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月11日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef PHYSICAL_PORT_CFG

extern unsigned char physical_port_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_physical_port
 功能描述  : 接口物理配置处理
 输入参数  : ATG_DCI_SET_PHYSICAL_PORT *set_physical_port   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月11日
    作    者   : xiaoxiang
    修改内容   : 新生成函数
  修改历史      :
  1.日    期   : 2014年8月20日
    作    者   : xiaoxiang
    修改内容   : 增加子配置块20 ALS

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_physical_port(ATG_DCI_SET_PHYSICAL_PORT *pstSetPhysicalPort NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_BYTE smac[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    NBB_BYTE smac_temp[6] = {0x00,0x00,0x00,0x00,0x00,0x00};
    NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    NBB_BYTE ucSubBoardNo = 0;
    NBB_INT  ret = SUCCESS;
    SUB_PORT stSubPort;
	EFM_PHY_PORT stEfmPort;
    SPM_PORT_WRED_KEY stWredKey;
    NBB_INT i = 0;
    NBB_BYTE ucTdmSubCardExist = ATG_DCI_UNEXIST;
    
    SPM_PHYSICAL_PORT_CB *pstPhysicalPort = NULL;

    /* 获取的子配置 */
    ATG_DCI_PHY_PORT_BASIC_DATA *pstBasicData        = NULL;
    ATG_DCI_PHY_PORT_PHY_DATA *pstPhyData            = NULL;
    ATG_DCI_PHY_PORT_POS_LINK_DATA *pstPosData       = NULL;
    ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *pstSdhSovhdData = NULL;
    ATG_DCI_PHY_PORT_CES_DATA *pstCesData            = NULL;
    ATG_DCI_PHY_PORT_CEP_OVHD_DATA *pstCepData       = NULL;
    ATG_DCI_PHY_PORT_EFM_OAM_DATA *pstEfmOamData     = NULL;
    ATG_DCI_PHY_PORT_TIME_SYNC_DATA *pstTimeSyncData = NULL;
    ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *pstMsgSupprData = NULL;
    ATG_DCI_PHY_PORT_FLOW_SHAPE *pstFlowShapeData    = NULL;
    ATG_DCI_PHY_PORT_SCHEDUL_POLICY *pstSchedulePolicyData = NULL;
    ATG_DCI_PHY_PORT_CONGEST_POLICY *pstCongestPolicyData  = NULL;
    ATG_DCI_PHY_PORT_IPV4NF_DATA *pstIpv4NfData            = NULL;
    ATG_DCI_PHY_PORT_IPNF_SAMPLER *pstIpSampleData   = NULL;
    ATG_DCI_PHY_PORT_IPV6NF_DATA *pstIpv6NfData      = NULL;
    ATG_DCI_PHY_PORT_COMMON_DATA *pstCommonData      = NULL;
    ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *pstSdhTovhdData = NULL;
    ATG_DCI_PHY_PORT_ALS_DATA *pstAlsData = NULL;

    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucBasicDataStart         = NULL;
    NBB_BYTE *pucPhyDataStart           = NULL;
    NBB_BYTE *pucPosDataStart           = NULL;
    NBB_BYTE *pucSdhSovhdDataStart      = NULL;
    NBB_BYTE *pucCesDataStart           = NULL;
    NBB_BYTE *pucCepDataStart           = NULL;
    NBB_BYTE *pucEfmOamDataStart        = NULL;
    NBB_BYTE *pucTimeSyncDataStart      = NULL;
    NBB_BYTE *pucMsgSupprDataStart      = NULL;
    NBB_BYTE *pucFlowShapeDataStart     = NULL;
    NBB_BYTE *pucSchedulePolicyDataStart = NULL;
    NBB_BYTE *pucCongestPolicyDataStart = NULL;
    NBB_BYTE *pucIpv4NfDataStart        = NULL;
    NBB_BYTE *pucIpSampleDataStart      = NULL;
    NBB_BYTE *pucIpv6NfDataStart        = NULL;
    NBB_BYTE *pucIpv6SampleDataStart    = NULL;
    NBB_BYTE *pucCommonDataStart        = NULL;
    NBB_BYTE *pucSdhTovhdDataStart      = NULL;
    NBB_BYTE *pucAlsDataStart      = NULL;
    
    /* 子配置的操作模式 */
    NBB_ULONG ulOperBasic       = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPhysical    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPos         = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperSdhSovhd    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperCes         = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperCep         = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperEfmOam      = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperTimeSync    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperMsgSuppr    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperFlowShape   = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperSchedulePolicy = ATG_DCI_OPER_NULL;  
    NBB_ULONG ulOperCongestPolicy  = ATG_DCI_OPER_NULL;  
    NBB_ULONG ulOperIpv4Nf      = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpSample    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6Nf      = ATG_DCI_OPER_NULL;  
    NBB_ULONG ulOperIpv6Sample  = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperCommon      = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperSdhTovhd    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperAls    = ATG_DCI_OPER_NULL;
    
    SPM_PORT_STRUCT stPort;
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMplsDisEn = 0;
    NBB_BYTE ucMacCheck = 0;
    NBB_USHORT usOtpid = 0;
    NBB_INT iPortRate = 0;
    NBB_INT iDuplex = 0;
    ALS_CONFIG_INFO stAlsConf;
    
    NBB_ULONG ulPortIndexKey = 0;
    
    NBB_TRC_ENTRY("spm_rcv_dci_set_physical_port");    

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetPhysicalPort != NULL);

	if (pstSetPhysicalPort == NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_physical_port(pstSetPhysicalPort==NULL)"));
        
        OS_PRINTF("***ERROR***:spm_rcv_dci_set_physical_port(pstSetPhysicalPort==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_physical_port(pstSetPhysicalPort==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", 0, 
        		FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage));
        
        goto EXIT_LABEL;
    }

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetPhysicalPort->return_code = ATG_DCI_RC_OK;

    OS_MEMSET(&stSubPort, 0, sizeof(SUB_PORT));
    OS_MEMSET(&stEfmPort, 0, sizeof(EFM_PHY_PORT));
    OS_MEMSET(&stWredKey, 0, sizeof(SPM_PORT_WRED_KEY));
    OS_MEMSET(&stAlsConf, 0, sizeof(ALS_CONFIG_INFO));

    ulPortIndexKey = pstSetPhysicalPort->key;
    NBB_TRC_DETAIL((NBB_FORMAT "  ulPortIndexKey = %ld", ulPortIndexKey));

    pstPhysicalPort = AVLL_FIND(SHARED.physical_port_tree, &ulPortIndexKey);

    /* 如果条目不存在，不删除 */
    if (pstPhysicalPort == NULL)
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    

/*
if ((ulPortIndexKey == 3) && (pstSetPhysicalPort->delete_struct == FALSE))
{
    sleep(5);
    sysinfo(&s_info1);
    printf("\n----------------------------------------------------\n");
    printf("Before:\n");
    printf("RAM: total %dk / free %dk\n",
          s_info1.totalram/1024, s_info1.freeram/1024);
    //spm_get_mem_info();

}
*/
    ulOperBasic          = pstSetPhysicalPort->oper_basic;
    ulOperPhysical       = pstSetPhysicalPort->oper_physical;
    ulOperPos            = pstSetPhysicalPort->oper_pos_link;
    ulOperSdhSovhd       = pstSetPhysicalPort->oper_sdh_soverhead;
    ulOperCes            = pstSetPhysicalPort->oper_ces;
    ulOperCep            = pstSetPhysicalPort->oper_cep_overhead;
    ulOperEfmOam         = pstSetPhysicalPort->oper_efm_oam;
    ulOperTimeSync       = pstSetPhysicalPort->oper_time_sync;
    ulOperMsgSuppr       = pstSetPhysicalPort->oper_msg_suppr;
    ulOperFlowShape      = pstSetPhysicalPort->oper_flow_shape;
    ulOperSchedulePolicy = pstSetPhysicalPort->oper_schedul_policy;
    ulOperCongestPolicy  = pstSetPhysicalPort->oper_congest_policy;
    ulOperIpv4Nf         = pstSetPhysicalPort->oper_ipv4nf;
    ulOperIpSample       = pstSetPhysicalPort->oper_ipnf_sampler;
    ulOperIpv6Nf         = pstSetPhysicalPort->oper_ipv6nf;
    ulOperCommon         = pstSetPhysicalPort->oper_common;
    ulOperSdhTovhd       = pstSetPhysicalPort->oper_sdh_toverhead;
    ulOperAls                 = pstSetPhysicalPort->oper_als;
    
    //获取本槽位号
    spm_hardw_getslot(&ucLocalSlot);

    /* 删除整个条目 */
    if (pstSetPhysicalPort->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除PORT_INDEX=%ld的 接口物理配置", ulPortIndexKey));

        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            /* 待修改：加打印PD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在"));
            
            OS_PRINTF("***ERROR***:要删除的PORT_INDEX=%ld 的接口物理配置并不存在!\n", ulPortIndexKey);

            OS_SPRINTF(ucMessage, "***ERROR***:要删除的PORT_INDEX=%ld 的接口物理配置并不存在!\n", ulPortIndexKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, DEL_UNEXIST_CONFIG_ERROR, "DEL_UNEXIST_CONFIG_ERROR", ucMessage));
            
            pstSetPhysicalPort->return_code = ATG_DCI_RC_OK;
            
            goto EXIT_LABEL;
        }
        /* 存在删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  查询到此配置，从tree中删除"));
            spm_dbg_print_physical_port_head(ulPortIndexKey, SPM_OPER_DEL);
            spm_dbg_record_physical_port_head(ulPortIndexKey, SPM_OPER_DEL);
            

            //调用相关驱动，删除物理接口配置
            spm_del_physical_port_cfg(pstPhysicalPort NBB_CCXT);
            
            //向子卡发送删除的IPS消息，仿真盘才用发送
            if ((pstPhysicalPort != NULL) && (pstPhysicalPort->basic_cfg_cb != NULL) && (pstPhysicalPort->basic_cfg_cb->port_type != ATG_DCI_ETH))
            {
                ucTdmSubCardExist = spm_hardw_getcesability(pstPhysicalPort->basic_cfg_cb->sub_board);
                
                if (ucTdmSubCardExist == ATG_DCI_EXIST)
                {
                    spm_send_physical_port_cfg(pstSetPhysicalPort, pstPhysicalPort->basic_cfg_cb->sub_board NBB_CCXT);
                }

                 //als配置向所有子卡发送,向子卡发送删除的IPS消息
                if ((0 != pstPhysicalPort->basic_cfg_cb->sub_board) && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id))
                {
                    spm_send_physical_port_cfg(pstSetPhysicalPort, pstPhysicalPort->basic_cfg_cb->sub_board NBB_CCXT);
                }
            
            }

            if (pstPhysicalPort != NULL)
            {
	            AVLL_DELETE(SHARED.physical_port_tree, pstPhysicalPort->spm_physical_port_node);   

	            //释放接口物理配置节点的内存空间
	            spm_free_physical_port_cb(pstPhysicalPort NBB_CCXT);
            }
        }
    }
    /* 增加或更新条目 */
    else
    {        
        /* 如果条目不存在，树中要增加条目 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  增加PORT_INDEX=%ld 的接口物理配置", ulPortIndexKey));
            spm_dbg_print_physical_port_head(ulPortIndexKey, SPM_OPER_ADD);
            spm_dbg_record_physical_port_head(ulPortIndexKey, SPM_OPER_ADD);
            
            pstPhysicalPort = spm_alloc_physical_port_cb(NBB_CXT);

			if (pstPhysicalPort != NULL)
			{
	            //如果是新增，直接赋key值，然后插入到tree中
	            pstPhysicalPort->port_index_key = ulPortIndexKey;

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.physical_port_tree, pstPhysicalPort->spm_physical_port_node);
			}
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  更新PORT_INDEX=%ld 的接口物理配置", ulPortIndexKey));
            spm_dbg_print_physical_port_head(ulPortIndexKey, SPM_OPER_UPD);
            spm_dbg_record_physical_port_head(ulPortIndexKey, SPM_OPER_UPD);
        }

		//增加此判断，取消PC-Lint告警
		if (pstPhysicalPort == NULL)
		{
            /* 待修改：加打印PD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要增加的PORT_INDEX=%ld 的接口物理配置未能成功申请内存!", ulPortIndexKey));
            
            OS_PRINTF("***ERROR***:要增加的PORT_INDEX=%ld 的接口物理配置未能成功申请内存!\n", ulPortIndexKey);

            OS_SPRINTF(ucMessage, "***ERROR***:要增加的PORT_INDEX=%ld 的接口物理配置未能成功申请内存!\n", ulPortIndexKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
            
            pstSetPhysicalPort->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /* 获取配置                                                                */
        /***************************************************************************/
        /******************************** 基本配置 *********************************/
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->basic_data);    

            /* 首地址为NULL，异常 */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucBasicDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstBasicData = (ATG_DCI_PHY_PORT_BASIC_DATA *)pucBasicDataStart;      

                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）基本配置\n");
                    spm_dbg_print_physical_port_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1）基本配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_basic_cfg(pstBasicData);
                
                NBB_TRC_DETAIL((NBB_FORMAT "  接口类型    = %d", pstBasicData->port_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  接口属性    = %d", pstBasicData->port_attrib));
                NBB_TRC_DETAIL((NBB_FORMAT "  接口速率    = %d", pstBasicData->port_rate));
                NBB_TRC_DETAIL((NBB_FORMAT "  框号        = %d", pstBasicData->subrack));
                NBB_TRC_DETAIL((NBB_FORMAT "  槽位号      = %d", pstBasicData->slot));
                NBB_TRC_DETAIL((NBB_FORMAT "  子卡号      = %d", pstBasicData->sub_board));
                NBB_TRC_DETAIL((NBB_FORMAT "  端口号      = %d", pstBasicData->port));
                NBB_TRC_DETAIL((NBB_FORMAT "  AU4 NO      = %d", pstBasicData->au4_no));
                NBB_TRC_DETAIL((NBB_FORMAT "  TUG3 NO     = %d", pstBasicData->tug3_no));
                NBB_TRC_DETAIL((NBB_FORMAT "  TUG2 NO     = %d", pstBasicData->tug2_no));
                NBB_TRC_DETAIL((NBB_FORMAT "  TU12 NO     = %d", pstBasicData->tu12_no));
                NBB_TRC_DETAIL((NBB_FORMAT "  TMD业务类型 = %d", pstBasicData->circuit_emu_mode));
                NBB_TRC_DETAIL((NBB_FORMAT "  上级接口    = %4x", pstBasicData->senior_index));

                //保存子卡号
                ucSubBoardNo = pstBasicData->sub_board;

                //存储设备端口号
                spm_add_dev_port_info(pstBasicData NBB_CCXT);
                
#ifdef SPU
                if (SHARED.local_slot_id == pstBasicData->slot)
                {
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstBasicData->port, 
                    		(LOGIC_PORT_ATTRI)pstBasicData->circuit_emu_mode, &stPort);

                    if (ret != SUCCESS)
                    {                        
                        OS_PRINTF("***ERROR***: 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ucSubBoardNo, pstBasicData->port);

                        OS_SPRINTF(ucMessage, "***ERROR***: 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ucSubBoardNo, pstBasicData->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->phy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->pos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->sdh_soverhead_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->ces_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->cep_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->efm_oam_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->time_sync_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->msg_suppr_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->flow_shape_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->schedul_policy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->congest_policy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->ipv4nf_return_code= ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->ipnf_sampler_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->ipv6nf_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //pstSetPhysicalPort->ipv6nf_sampler_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        
                        goto EXIT_LABEL;
                    }

                    //如果物理接口为仿真口，则不配置端口属性，在子卡初始化完成后liujuan配置
                    if ((pstBasicData->port_type == ATG_DCI_ETH) 
                     && (pstBasicData->port_attrib == ATG_DCI_DERECT))
                    {
                        ret = ApiC3SetPortType(stPort.ucUnit, stPort.ucPort, ETH);
                        NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%d, 端口%d为ETH, ApiC3SetPortType() ret=%d", ulPortIndexKey, stPort.ucPort, ret));
                    }
                    /*
                    else if ((pstBasicData->port_type != ATG_DCI_ETH)
                          && (pstBasicData->port_attrib == ATG_DCI_DERECT)
                          && (pstBasicData->circuit_emu_mode != 0))
                    {
                        ApiC3SetPortType(stPort.ucUnit, stPort.ucPort, CES);
                        NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%d, 端口%d为CES, ApiC3SetPortType() ret=%d", ulPortIndexKey, stPort.ucPort, ret));
                    }
                    */
                }
#endif
                if (ret == SUCCESS)
                {
                    /* 保存数据 */
                    if (pstPhysicalPort->basic_cfg_cb == NULL)
                    {
                        pstPhysicalPort->basic_cfg_cb = (ATG_DCI_PHY_PORT_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_BASIC_DATA),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_PHY_PORT_BASIC_CB);
                    }               

                    OS_MEMCPY(pstPhysicalPort->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_PHY_PORT_BASIC_DATA));
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


        /******************************** 以太网接口物理配置 ********************************/
        if (ulOperPhysical == ATG_DCI_OPER_ADD)
        {
        
            /* 计算第一个entry的地址。*/
            pucPhyDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                &pstSetPhysicalPort->physical_data);    

            /* 首地址为NULL，异常 */
            if (pucPhyDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pstPhyData is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else
            {
                pstPhyData = (ATG_DCI_PHY_PORT_PHY_DATA *)pucPhyDataStart;

                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2）以太网接口物理配置\n");
                    spm_dbg_print_physical_port_phy_cfg(pstPhyData);
                }

                OS_SPRINTF(ucMessage,"  2）以太网接口物理配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_phy_cfg(pstPhyData);

                NBB_TRC_DETAIL((NBB_FORMAT "  工作模式 = %s", pstPhyData->work_mode ? "自协商" : "非自协商"));
                NBB_TRC_DETAIL((NBB_FORMAT "  端口开关 = %s", pstPhyData->port_switch ? "开" : "关"));
                NBB_TRC_DETAIL((NBB_FORMAT "  物理地址 = %02x %02x %02x %02x %02x %02x", pstPhyData->phy_addr[8],
                                                                           pstPhyData->phy_addr[9],
                                                                           pstPhyData->phy_addr[10],
                                                                           pstPhyData->phy_addr[11],
                                                                           pstPhyData->phy_addr[12],
                                                                           pstPhyData->phy_addr[13]));
                NBB_TRC_DETAIL((NBB_FORMAT "  MTU   = %d", pstPhyData->mtu));
                NBB_TRC_DETAIL((NBB_FORMAT "  带宽  = %d Mbps", pstPhyData->bandwidth));
                NBB_TRC_DETAIL((NBB_FORMAT "  STPID = %x", pstPhyData->stpid));
                //NBB_TRC_DETAIL((NBB_FORMAT "  CTPID = %x", pstPhyData->ctpid));
                NBB_TRC_DETAIL((NBB_FORMAT "  EFM OAM使能 = %s", pstPhyData->efm_oam ? "使能" : "不使能"));
                NBB_TRC_DETAIL((NBB_FORMAT "  10GE端口模式 = %s", pstPhyData->xge_port_mode ? "10GE WAN" : "10GE LAN"));
                NBB_TRC_DETAIL((NBB_FORMAT "  MCC开关 = %s", pstPhyData->mcc_switch ? "开" : "关"));
                NBB_TRC_DETAIL((NBB_FORMAT "  端口TP模式 = %s", pstPhyData->port_tp_mode ? "是" : "否"));
                NBB_TRC_DETAIL((NBB_FORMAT "  端口光电模式 = %s", pstPhyData->port_optical_mode ? "电口" : "光口"));
                NBB_TRC_DETAIL((NBB_FORMAT "  端口GE  = %d", pstPhyData->port_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  端口优先级  = %d", pstPhyData->port_prio));
                NBB_TRC_DETAIL((NBB_FORMAT "  MCC VLAN_ID  = %d", pstPhyData->mcc_vlan_id));

#ifdef SPU
                //只有是Eth网类型的口才进入端口配置
                if ((pstPhysicalPort->basic_cfg_cb != NULL) 
                 && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id)
                 && (pstPhysicalPort->basic_cfg_cb->port_type == ATG_DCI_ETH))
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret != SUCCESS)
                    {
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->phy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->pos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->sdh_soverhead_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->ces_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->cep_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->efm_oam_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->time_sync_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->msg_suppr_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->flow_shape_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->schedul_policy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->congest_policy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->ipv4nf_return_code= ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->ipnf_sampler_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstSetPhysicalPort->ipv6nf_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //pstSetPhysicalPort->ipv6nf_sampler_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        
                        goto EXIT_LABEL;
                    }

//                    ucMplsDisEn = (!pstPhyData->mpls_forward_oam);
                    ucMacCheck = (!pstPhyData->port_tp_mode);

                    //如果tpid配置下来的值不合法，强行配置成0x8100
                    if (pstPhyData->stpid > 0x600)
                    {
                        usOtpid = pstPhyData->stpid;
                    }
                    else
                    {
                        usOtpid = 0x8100;
                    }                    

                    smac[0] = pstPhyData->phy_addr[8];
                    smac[1] = pstPhyData->phy_addr[9];
                    smac[2] = pstPhyData->phy_addr[10];
                    smac[3] = pstPhyData->phy_addr[11];
                    smac[4] = pstPhyData->phy_addr[12];
                    smac[5] = pstPhyData->phy_addr[13];


                    /* 端口速率 */
                    switch (pstPhyData->port_type)
                    {
                        case ATG_DCI_PORT_RATE_100GE:
                            iPortRate = 100000;
                            break;
                        case ATG_DCI_PORT_RATE_40GE:
                            iPortRate = 40000;
                            break;
                        case ATG_DCI_PORT_RATE_10GE:
                            iPortRate = 10000;
                            break;
                        case ATG_DCI_PORT_RATE_GE:
                            iPortRate = 1000;
                            break;
                        case ATG_DCI_PORT_RATE_FE:
                            iPortRate = 100;
                            break;
                        default:
                            break;
                    }         

                    /* 端口开关：0/1=关/开 */
                    ApiC3SetPortState(stPort.ucUnit, stPort.ucPort, pstPhyData->port_switch);
                    
                    ret = SUCCESS;
                    ret = NBB_MEMCMP(smac,smac_temp,6);
                    
                    if((0 != ret) && (1 != (smac[0] & 0x1)))
                    {
                         ret = SUCCESS;
                        
                         /* 设置端口mac */
    			    ret = ApiC3SetPortMac(stPort.ucUnit, ATG_DCI_IS_NOT_VE, stPort.ucPort, smac);
                    
    			    //printf("ret=%d (unit=%d port=%d smac=%02x-%02x-%02x-%02x-%02x-%02x)\n", ret, 
    			                                                    //stPort.ucUnit, 
    			                                                    //stPort.ucPort,
    			                                                    //smac[0],smac[1],smac[2],smac[3],smac[4],smac[5]);
                    }                    
    			    
                    /* MTU：1600-10240 */
                    ApiC3SetPortRxMaxSize(stPort.ucUnit, stPort.ucPort, pstPhyData->mtu, 1);

                    /* 带宽：0-1000-100000 单位：1Mbps */
                    ApiAradSetOFPBandWidth(UNIT_0, stPort.ucPort, 1000 * pstPhyData->bandwidth);
                    //ApiC3SetPortEgressRc(stPort.ucUnit, stPort.ucPort, pstPhyData->bandwidth, 640);

                    /* STPID */
                    ApiC3SetPortTpid(stPort.ucUnit, stPort.ucPort, usOtpid);

                    /* MPLS转发使能 */
                    //ApiC3SetPortMplsDisable(stPort.ucUnit, stPort.ucPort, ucMplsDisEn);

                    /* MCC开关 和 MCC VLAN_ID */
                    //ApiC3SetMccVlan(stPort.ucUnit, stPort.ucPort, pstPhyData->mcc_vlan_id, pstPhyData->mcc_switch);

                    /* 端口TP模式：0/1=否/是 */
                    ApiC3SetPortMacCheck(stPort.ucUnit, stPort.ucPort, ucMacCheck);

                    /* 端口流控开关: 0/1=关/开*/
                    ApiC3SetPortPause(stPort.ucUnit, stPort.ucPort, pstPhyData->flow_control, pstPhyData->flow_control);

					/* 如果是自协商 且为千兆或百兆，才设置自协商 */
					if ((pstPhyData->work_mode == 1) 
					 && ((pstPhyData->port_type == ATG_DCI_PORT_RATE_GE) || (pstPhyData->port_type == ATG_DCI_PORT_RATE_FE)))
					{
						/* 工作模式：0/1=非自协商/自协商 ，注意：只有GE口才会设置协商模式*/
					    ret = ApiC3SetPortAutoNego(stPort.ucUnit, stPort.ucPort, pstPhyData->work_mode);
                        //printf("ApiC3SetPortAutoNego(%d, %d, %d) ret=%d\n", stPort.ucUnit, stPort.ucPort, pstPhyData->work_mode, ret);
					}
					else
					{
						if ((pstPhyData->port_type == ATG_DCI_PORT_RATE_GE) || (pstPhyData->port_type == ATG_DCI_PORT_RATE_FE))
						{
							ret = ApiC3SetPortAutoNego(stPort.ucUnit, stPort.ucPort, pstPhyData->work_mode);
						}						

						iPortRate = 0;
						iDuplex = 0;
						ApiC3GetPortSpeedAndDuplex(stPort.ucUnit, stPort.ucPort, &iPortRate, &iDuplex);
						
						//如果重复调用 ApiC3SetPortSpeedAndDuplex ，端口会los up，应用层判断相同配置不执行驱动
						if ((ucIfExist == ATG_DCI_EXIST) && (pstPhysicalPort != NULL) && (pstPhysicalPort->phy_cfg_cb != NULL))
						{
							//如果端口类型切换，调用
							if (pstPhysicalPort->phy_cfg_cb->port_type != pstPhyData->port_type)
							{
                    			/* 设置端口速率和工作模式 */
                    			ApiC3SetPortSpeedAndDuplex(stPort.ucUnit, stPort.ucPort, iPortRate, ATG_DCI_FULL_DUPLEX);								
							}
						}
						else
						{
							/* 设置端口速率和工作模式 */
                    		ApiC3SetPortSpeedAndDuplex(stPort.ucUnit, stPort.ucPort, iPortRate, ATG_DCI_FULL_DUPLEX);
						}
					} 
                    
                    ret = SUCCESS;
                    ret = spm_subcard_setxtcbportproc(pstPhysicalPort->basic_cfg_cb->sub_board,
                        pstPhysicalPort->basic_cfg_cb->port,pstPhyData->xge_port_mode);

                    if (ret != SUCCESS)
                    {
                        OS_PRINTF("***ERROR***:spm_subcard_setxtcbportproc()失败 PORT_INDEX=%ld !\n", ulPortIndexKey);

                        OS_SPRINTF(ucMessage, 
                            "***ERROR***:spm_subcard_setxtcbportproc()失败 PORT_INDEX=%ld !\n", ulPortIndexKey);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);                   
                    }                   
                }
#endif

                /* 保存数据 */
                if (pstPhysicalPort->phy_cfg_cb == NULL)
                {
                    pstPhysicalPort->phy_cfg_cb = (ATG_DCI_PHY_PORT_PHY_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_PHY_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_PHY_CB);
                }    
                
                OS_MEMCPY(pstPhysicalPort->phy_cfg_cb, pstPhyData, sizeof(ATG_DCI_PHY_PORT_PHY_DATA));

            }
        }
        else if (ulOperPhysical == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperPhysical == ATG_DCI_OPER_ADD)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }        

        /******************************** POS接口链路层配置 ********************************/
        if (ulOperPos == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucPosDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->pos_link_data);    

            /* 首地址为NULL，异常 */
            if (pucPosDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucPosDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstPosData = (ATG_DCI_PHY_PORT_POS_LINK_DATA *)pucPosDataStart;      

                /* 配置处理，todo */            

                /* 保存数据 */
                if (pstPhysicalPort->pos_link_cfg_cb == NULL)
                {
                    pstPhysicalPort->pos_link_cfg_cb = (ATG_DCI_PHY_PORT_POS_LINK_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_POS_LINK_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_POS_CB);
                }   
                
                OS_MEMCPY(pstPhysicalPort->pos_link_cfg_cb, pstPosData, sizeof(ATG_DCI_PHY_PORT_POS_LINK_DATA));

            }
        }
        else if (ulOperPos == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperPos == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** SDH段开销配置 ********************************/
        if (ulOperSdhSovhd == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucSdhSovhdDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->sdh_soverhead_data);    

            /* 首地址为NULL，异常 */
            if (pucSdhSovhdDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucSdhSovhdDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstSdhSovhdData = (ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *)pucSdhSovhdDataStart;    

                /* 配置处理，todo */
                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  4)SDH段开销配置\n");
                    spm_dbg_print_physical_port_sdh_sovhd_cfg(pstSdhSovhdData);
                }

                OS_SPRINTF(ucMessage,"  4)SDH段开销配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_sdh_sovhd_cfg(pstSdhSovhdData);

                /* 保存数据 */
                if (pstPhysicalPort->sdh_sovhd_cfg_cb == NULL)
                {
                    pstPhysicalPort->sdh_sovhd_cfg_cb = (ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_SDH_SOVHD_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_SDH_SOVHD_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->sdh_sovhd_cfg_cb, pstSdhSovhdData, sizeof(ATG_DCI_PHY_PORT_SDH_SOVHD_DATA));

            }
        }
        else if (ulOperSdhSovhd == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperSdhSovhd == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** SDH通道开销配置 ********************************/
        if (ulOperSdhTovhd == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucSdhTovhdDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->sdh_toverhead_data);    

            /* 首地址为NULL，异常 */
            if (pucSdhTovhdDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucSdhTovhdDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstSdhTovhdData = (ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *)pucSdhTovhdDataStart;    

                /* 配置处理，todo */
                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  19)SDH通道开销配置\n");
                    spm_dbg_print_physical_port_sdh_tovhd_cfg(pstSdhTovhdData);
                }

                OS_SPRINTF(ucMessage,"  19)SDH通道开销配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_sdh_tovhd_cfg(pstSdhSovhdData);

                /* 保存数据 */
                if (pstPhysicalPort->sdh_tovhd_cfg_cb == NULL)
                {
                    pstPhysicalPort->sdh_tovhd_cfg_cb = (ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_SDH_TOVHD_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_SDH_TOVHD_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->sdh_tovhd_cfg_cb, pstSdhTovhdData, sizeof(ATG_DCI_PHY_PORT_SDH_TOVHD_DATA));

            }
        }
        else if (ulOperSdhTovhd == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperSdhTovhd == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }
        
        /******************************** CES接口配置 ********************************/
        if (ulOperCes == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucCesDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->ces_data);    

            /* 首地址为NULL，异常 */
            if (pucCesDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucCesDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstCesData = (ATG_DCI_PHY_PORT_CES_DATA *)pucCesDataStart;   

                /* 配置处理，todo */
                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  5)CES接口配置\n");
                    spm_dbg_print_physical_port_ces_cfg(pstCesData);
                }

                OS_SPRINTF(ucMessage,"  5)CES接口配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_ces_cfg(pstCesData);
            

                /* 保存数据 */
                if (pstPhysicalPort->ces_cfg_cb == NULL)
                {
                    pstPhysicalPort->ces_cfg_cb = (ATG_DCI_PHY_PORT_CES_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_CES_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_CES_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->ces_cfg_cb, pstCesData, sizeof(ATG_DCI_PHY_PORT_CES_DATA));

            }
        }
        else if (ulOperCes == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperCes == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }
        
        /******************************** CEP接口开销配置 ********************************/
        if (ulOperCep == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucCepDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->cep_overhead_data);    

            /* 首地址为NULL，异常 */
            if (pucCepDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucCepDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstCepData = (ATG_DCI_PHY_PORT_CEP_OVHD_DATA *)pucCepDataStart;    

                /* 配置处理，todo */
                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  6)CEP接口开销配置\n");
                    spm_dbg_print_physical_port_cep_cfg(pstCepData);
                }

                OS_SPRINTF(ucMessage,"  6)CEP接口开销配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_cep_cfg(pstCepData);

                /* 保存数据 */
                if (pstPhysicalPort->cep_ovhd_cfg_cb == NULL)
                {
                    pstPhysicalPort->cep_ovhd_cfg_cb = (ATG_DCI_PHY_PORT_CEP_OVHD_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_CEP_OVHD_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_CEP_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->cep_ovhd_cfg_cb, pstCepData, sizeof(ATG_DCI_PHY_PORT_CEP_OVHD_DATA));

            }
        }
        else if (ulOperCep == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperCep == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 以太网EFM OAM ********************************/
        if (ulOperEfmOam == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucEfmOamDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->efm_oam_data);    

            /* 首地址为NULL，异常 */
            if (pucEfmOamDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucEfmOamDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstEfmOamData = (ATG_DCI_PHY_PORT_EFM_OAM_DATA *)pucEfmOamDataStart;    

                /* 配置处理，todo */
                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  7）以太网EFM OAM\n");
                    spm_dbg_print_physical_port_efm_cfg(pstEfmOamData);
                }

                OS_SPRINTF(ucMessage,"  7）以太网EFM OAM\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_efm_cfg(pstEfmOamData);
				
#ifdef SPU
                /* 配置处理，每个槽位都建 */                
                if (pstPhysicalPort->basic_cfg_cb != NULL)
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        stEfmPort.ports = stPort.ucPort;
                        stEfmPort.slots = pstPhysicalPort->basic_cfg_cb->slot;
                        stEfmPort.cards = pstPhysicalPort->basic_cfg_cb->sub_board;
						stEfmPort.cases = pstPhysicalPort->basic_cfg_cb->subrack;

                        //spm_set_efm_oam(&stEfmPort, pstEfmOamData);
                    }
                    else
                    {
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->ipv4nf_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }      
#endif

                /* 保存数据 */
                if (pstPhysicalPort->efm_oam_cfg_cb == NULL)
                {
                    pstPhysicalPort->efm_oam_cfg_cb = (ATG_DCI_PHY_PORT_EFM_OAM_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_EFM_OAM_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_EFM_OAM_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->efm_oam_cfg_cb, pstEfmOamData, sizeof(ATG_DCI_PHY_PORT_EFM_OAM_DATA));

            }
        }
        else if (ulOperEfmOam == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperEfmOam == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 时间同步端口配置 ********************************/
        if (ulOperTimeSync == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucTimeSyncDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->time_sync_data);    

            /* 首地址为NULL，异常 */
            if (pucTimeSyncDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  puTimeSyncDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstTimeSyncData = (ATG_DCI_PHY_PORT_TIME_SYNC_DATA *)pucTimeSyncDataStart;    

                /* 配置处理，todo */
                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  8）时间同步端口配置\n");
                    spm_dbg_print_physical_port_time_sync_cfg(pstTimeSyncData);
                }

                OS_SPRINTF(ucMessage,"  8）时间同步端口配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_time_sync_cfg(pstTimeSyncData);

#ifdef SRC
                /* 配置处理，todo */
                if (pstPhysicalPort->basic_cfg_cb != NULL)
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        OS_MEMSET(&stSubPort, 0, sizeof(SUB_PORT));
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;
                        
                        ret = spm_ptp_set_port(stSubPort.slot, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, stSubPort.port, pstTimeSyncData);

                        if (ret != SUCCESS)
                        {                        
	                        OS_PRINTF("***ERROR***:spm_ptp_set_port() ret=%d\n", ret);

	                        OS_SPRINTF(ucMessage, "***ERROR***:spm_ptp_set_port() ret=%d\n", ret);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
	                        
	                        pstSetPhysicalPort->time_sync_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        //goto EXIT_LABEL;
	                    }
                    }
                    else
                    {                        
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->time_sync_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }       
                }       
#endif

                /* 保存数据 */
                if (pstPhysicalPort->time_sync_cfg_cb == NULL)
                {
                    pstPhysicalPort->time_sync_cfg_cb = (ATG_DCI_PHY_PORT_TIME_SYNC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_TIME_SYNC_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_TIME_SYNC_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->time_sync_cfg_cb, pstTimeSyncData, sizeof(ATG_DCI_PHY_PORT_TIME_SYNC_DATA));

            }
        }
        else if (ulOperTimeSync == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperTimeSync == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 以太网接口报文抑制 ********************************/
        if (ulOperMsgSuppr == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucMsgSupprDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->msg_suppr_data);    

            /* 首地址为NULL，异常 */
            if (pucMsgSupprDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucMsgSupprDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstMsgSupprData = (ATG_DCI_PHY_PORT_MSG_SUPPR_DATA*)pucMsgSupprDataStart;    

                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  9）以太网接口报文抑制\n");
                    spm_dbg_print_physical_port_msg_suppr_cfg(pstMsgSupprData);
                }

                OS_SPRINTF(ucMessage,"  9）以太网接口报文抑制\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_msg_suppr_cfg(pstMsgSupprData);
                
                NBB_TRC_DETAIL((NBB_FORMAT "  BC-Supress广播包抑制开关 = %d", pstMsgSupprData->bc_suppr_switch));
                NBB_TRC_DETAIL((NBB_FORMAT "  MC-Supress组播包抑制开关 = %d", pstMsgSupprData->mc_suppr_switch));
                NBB_TRC_DETAIL((NBB_FORMAT "  FD-Supress泛洪包抑制开关 = %d", pstMsgSupprData->fd_suppr_switch));
                NBB_TRC_DETAIL((NBB_FORMAT "  BC-Supress广播包抑制流量 = %d", pstMsgSupprData->bc_suppr_flow));
                NBB_TRC_DETAIL((NBB_FORMAT "  MC-Supress组播包抑制流量 = %d", pstMsgSupprData->mc_suppr_flow));
                NBB_TRC_DETAIL((NBB_FORMAT "  FD-Supress泛洪包抑制流量 = %d", pstMsgSupprData->fd_suppr_flow));
#ifdef SPU
                /* 配置处理，todo */
                if ((pstPhysicalPort->basic_cfg_cb != NULL) 
                 && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id))
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        OS_MEMSET(&stSubPort, 0, sizeof(SUB_PORT));
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;
                        
                        spm_set_port_flood_control(&stSubPort, 
                                                   pstMsgSupprData->bc_suppr_switch,
                                                   pstMsgSupprData->mc_suppr_switch,
                                                   pstMsgSupprData->fd_suppr_switch, 
                                                   pstMsgSupprData->bc_suppr_flow,
                                                   pstMsgSupprData->mc_suppr_flow,
                                                   pstMsgSupprData->fd_suppr_flow
                                                   NBB_CCXT);                                       
                    }
                    else
                    {                        
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->msg_suppr_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }       
                }       
#endif
                /* 保存数据 */
                if (pstPhysicalPort->msg_suppr_cfg_cb == NULL)
                {
                    pstPhysicalPort->msg_suppr_cfg_cb = (ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_MSG_SUPPR_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_MSG_SUPPR_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->msg_suppr_cfg_cb, pstMsgSupprData, sizeof(ATG_DCI_PHY_PORT_MSG_SUPPR_DATA));

            }
        }
        else if (ulOperMsgSuppr == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperMsgSuppr == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }
        
        /******************************** 以太网接口流量整形 ********************************/
        if (ulOperFlowShape == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucFlowShapeDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->flow_shape_data);    

            /* 首地址为NULL，异常 */
            if (pucFlowShapeDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucFlowShapeDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstFlowShapeData = (ATG_DCI_PHY_PORT_FLOW_SHAPE*)pucFlowShapeDataStart;    
#ifdef SPU
                /* 配置处理，todo */
                if ((pstPhysicalPort->basic_cfg_cb != NULL) 
                 && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id))
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;

                        for (i=0; i<ATG_DCI_QOS_BEHAVIOR_SHAPE_NUM; i++)
                        {
                            spm_set_port_cir_pir(&stSubPort, i, &pstFlowShapeData->flow_shape_queue[i] NBB_CCXT);
                        }     
                    }
                    else
                    {
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->flow_shape_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }               
#endif
                /* 保存数据 */
                if (pstPhysicalPort->flow_shape_cfg_cb== NULL)
                {
                    pstPhysicalPort->flow_shape_cfg_cb= (ATG_DCI_PHY_PORT_FLOW_SHAPE*)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_FLOW_SHAPE),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_FLOW_SHAPE_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->flow_shape_cfg_cb, pstFlowShapeData, sizeof(ATG_DCI_PHY_PORT_FLOW_SHAPE));

            }
        }
        else if (ulOperFlowShape== ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperFlowShape== ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 以太网出口队列调度策略 ********************************/
        if (ulOperSchedulePolicy == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucSchedulePolicyDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->schedul_policy_data);    

            /* 首地址为NULL，异常 */
            if (pucSchedulePolicyDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucSchedulePolicyDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstSchedulePolicyData = (ATG_DCI_PHY_PORT_SCHEDUL_POLICY*)pucSchedulePolicyDataStart;    
#ifdef SPU
                /* 配置处理，todo */                
                if ((pstPhysicalPort->basic_cfg_cb != NULL) 
                 && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id))
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;

                        for (i=0; i<ATG_DCI_QOS_BEHAVIOR_SCHEDL_NUM; i++)
                        {
                            spm_set_port_sp_wfq(&stSubPort, i, &pstSchedulePolicyData->schedule_queue[i] NBB_CCXT);
                        }
                    }
                    else
                    {                        
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->schedul_policy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }       
#endif
                /* 保存数据 */
                if (pstPhysicalPort->schedule_policy_cfg_cb == NULL)
                {
                    pstPhysicalPort->schedule_policy_cfg_cb = (ATG_DCI_PHY_PORT_SCHEDUL_POLICY*)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_SCHEDUL_POLICY),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_SCHEDUL_POLICY_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->schedule_policy_cfg_cb, pstSchedulePolicyData, sizeof(ATG_DCI_PHY_PORT_SCHEDUL_POLICY));

            }
        }
        else if (ulOperSchedulePolicy== ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperSchedulePolicy== ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 以太网出口队列拥塞策略 ********************************/
        if (ulOperCongestPolicy == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucCongestPolicyDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->congest_policy_data);    

            /* 首地址为NULL，异常 */
            if (pucCongestPolicyDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucCongestPolicyDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstCongestPolicyData = (ATG_DCI_PHY_PORT_CONGEST_POLICY*)pucCongestPolicyDataStart;    
#ifdef SPU
                /* 配置处理，每个槽位都建 */                
                if (pstPhysicalPort->basic_cfg_cb != NULL)
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;
                        stWredKey.index = ulPortIndexKey;

                        for (i=0; i<ATG_DCI_QOS_BEHAVIOR_CONGST_NUM; i++)
                        {
                            stWredKey.cos = i;
                            spm_set_port_wred(&stWredKey, &stSubPort, &pstCongestPolicyData->congest_queue[i] NBB_CCXT);
                        }
                    }
                    else
                    {
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->congest_policy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }      
#endif
                /* 保存数据 */
                if (pstPhysicalPort->congest_policy_cfg_cb == NULL)
                {
                    pstPhysicalPort->congest_policy_cfg_cb = (ATG_DCI_PHY_PORT_CONGEST_POLICY*)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_CONGEST_POLICY),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_CONGEST_POLICY_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->congest_policy_cfg_cb, pstCongestPolicyData, sizeof(ATG_DCI_PHY_PORT_CONGEST_POLICY));

            }
        }
        else if (ulOperCongestPolicy== ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperCongestPolicy== ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** IPV4流采样使能配置IPV4 NetFlow ********************************/
        if (ulOperIpv4Nf == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucIpv4NfDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->ipv4nf_data);    

            /* 首地址为NULL，异常 */
            if (pucIpv4NfDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucIpv4NfDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstIpv4NfData = (ATG_DCI_PHY_PORT_IPV4NF_DATA*)pucIpv4NfDataStart; 

                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  13）ipv4流采样使能配置\n");
                    spm_print_ipv4_nf_enable(pstIpv4NfData);
                }

                OS_SPRINTF(ucMessage,"  13）ipv4流采样使能配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_record_ipv4_nf_enable(pstIpv4NfData);

#ifdef SPU
                /* 配置处理，每个槽位都建 */                
                if (pstPhysicalPort->basic_cfg_cb != NULL)
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;

                        ret = spm_set_ipv4_nf_enable(&stSubPort, pstIpv4NfData);
                        
                        if(ret != SUCCESS)
                        {
                            OS_PRINTF("***ERROR***:spm_set_ipv4_nf_enable(),ret = %d!\n", ret);

                            OS_SPRINTF(ucMessage, "***ERROR***:spm_set_ipv4_nf_enable(),ret = %d!\n", ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_FUNC_ERROR, "CALL_FUNC_ERROR", ucMessage));
                            
                            pstSetPhysicalPort->ipv4nf_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        }
                        else
                        {
                            
                            /* 保存数据 */
                            if (pstPhysicalPort->ipv4_nf_cfg_cb == NULL)
                            {
                                pstPhysicalPort->ipv4_nf_cfg_cb = (ATG_DCI_PHY_PORT_IPV4NF_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA),
                                                                        NBB_NORETRY_ACT,
                                                                        MEM_SPM_PHY_PORT_IPV4_NF_CB);
                            }  
                            
                            OS_MEMCPY(pstPhysicalPort->ipv4_nf_cfg_cb, pstIpv4NfData, sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA));
                        }
                    }
                    else
                    {
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->ipv4nf_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }      
#endif
            }
        }
        else if (ulOperIpv4Nf == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperIpv4Nf == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** IPV4流采样镜像 IPV4 NetFlow sampler ********************************/
       
        if (ulOperIpSample == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucIpSampleDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->ipnf_sampler_data);    

            /* 首地址为NULL，异常 */
            if (pucIpSampleDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucIpSampleDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstIpSampleData = (ATG_DCI_PHY_PORT_IPNF_SAMPLER*)pucIpSampleDataStart; 
                
                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  14）ip流采样镜像\n");
                    spm_print_ip_nf_smapler(pstIpSampleData);
                }

                OS_SPRINTF(ucMessage,"  14）ip流采样镜像\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_record_ip_nf_smapler(pstIpSampleData);

#ifdef SPU
                /* 配置处理，每个槽位都建 */                
                if (pstPhysicalPort->basic_cfg_cb != NULL)
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                            (LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;

                        ret = spm_set_ipv4_sampler(&stSubPort, pstIpSampleData NBB_CCXT);
                        
                        if(ret != SUCCESS)
                        {
                            OS_PRINTF("***ERROR***:spm_set_ipv4_sampler(),ret = %d!\n", ret);

                            OS_SPRINTF(ucMessage, "***ERROR***:spm_set_ipv4_sampler(),ret = %d!\n", ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_FUNC_ERROR, "CALL_FUNC_ERROR", ucMessage));
                            
                            pstSetPhysicalPort->ipnf_sampler_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        }
                        else
                        {
                            /* 保存数据 */
                            if (pstPhysicalPort->ip_sample_cfg_cb == NULL)
                            {
                                pstPhysicalPort->ip_sample_cfg_cb = (ATG_DCI_PHY_PORT_IPNF_SAMPLER *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_IPNF_SAMPLER),
                                                                        NBB_NORETRY_ACT,
                                                                        MEM_SPM_PHY_PORT_IPV4_SAMPLE_CB);
                            }  
                            
                            OS_MEMCPY(pstPhysicalPort->ip_sample_cfg_cb, pstIpSampleData, sizeof(ATG_DCI_PHY_PORT_IPNF_SAMPLER));
                        }
                    }
                    else
                    {
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->ipnf_sampler_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }      
#endif
            }
        }
        else if (ulOperIpSample == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperIpSample == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        
        /******************************** IPV6流采样使能配置IPV6 NetFlow ********************************/
        if (ulOperIpv6Nf == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucIpv6NfDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->ipv6nf_data);    

            /* 首地址为NULL，异常 */
            if (pucIpv6NfDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6NfDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstIpv6NfData = (ATG_DCI_PHY_PORT_IPV6NF_DATA*)pucIpv6NfDataStart; 

                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  15）ipv6流采样使能\n");
                    spm_print_ipv6_nf_enable(pstIpv6NfData);
                }

                OS_SPRINTF(ucMessage,"  15）ipv6流采样使能\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_record_ipv6_nf_enable(pstIpv6NfData);
                

#ifdef SPU
                /* 配置处理，每个槽位都建 */                
                if (pstPhysicalPort->basic_cfg_cb != NULL)
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;

                        ret = spm_set_ipv6_nf_enable(&stSubPort, pstIpv6NfData);
                        
                        if(ret != SUCCESS)
                        {
                            OS_PRINTF("***ERROR***:spm_set_ipv6_nf_enable(),ret = %d!\n", ret);

                            OS_SPRINTF(ucMessage, "***ERROR***:spm_set_ipv6_nf_enable(),ret = %d!\n", ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_FUNC_ERROR, "CALL_FUNC_ERROR", ucMessage));
                            
                            pstSetPhysicalPort->ipv6nf_return_code= ATG_DCI_RC_UNSUCCESSFUL;
                        }
                        else
                        {
                            /* 保存数据 */
                            if (pstPhysicalPort->ipv6_nf_cfg_cb == NULL)
                            {
                                pstPhysicalPort->ipv6_nf_cfg_cb = (ATG_DCI_PHY_PORT_IPV6NF_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA),
                                                                        NBB_NORETRY_ACT,
                                                                        MEM_SPM_PHY_PORT_IPV6_NF_CB);
                            }  
                            
                            OS_MEMCPY(pstPhysicalPort->ipv6_nf_cfg_cb, pstIpv6NfData, sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA));
                        }
                    }
                    else
                    {
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->ipv6nf_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }      
#endif
            }
        }
        else if (ulOperIpv6Nf == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperIpv6Nf == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** 接口通用配置 ********************************/
        if (ulOperCommon == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucCommonDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->common_data);    

            /* 首地址为NULL，异常 */
            if (pucCommonDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucCommonDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstCommonData = (ATG_DCI_PHY_PORT_COMMON_DATA*)pucCommonDataStart; 

                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  18）接口通用配置\n");
                    spm_dbg_print_physical_port_common_cfg(pstCommonData);
                }

                OS_SPRINTF(ucMessage,"  18）接口通用配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_common_cfg(pstCommonData);

#ifdef SPU
                //只有是Eth网类型的口才进入端口配置
                if ((pstPhysicalPort->basic_cfg_cb != NULL) 
                 && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id)
                 && (pstPhysicalPort->basic_cfg_cb->port_type == ATG_DCI_ETH))
                {
		            ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;

		            ret = SUCCESS;

		            ret += spm_hardw_setupholdtime(ucSubBoardNo,  \
		                    pstPhysicalPort->basic_cfg_cb->port,  \
		                    pstCommonData->up_time);
		            ret += spm_hardw_setdownholdtime(ucSubBoardNo,  \
		                    pstPhysicalPort->basic_cfg_cb->port,  \
		                    pstCommonData->down_time);

					if (ret != SUCCESS)
					{                        
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 接口通用配置失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 接口通用配置失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_FUNC_ERROR, "CALL_FUNC_ERROR", ucMessage));
                        
                        pstSetPhysicalPort->common_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
		        }
#endif

                //只有是Eth网类型的口才进入端口配置
                if ((pstPhysicalPort->basic_cfg_cb != NULL) 
                 && (pstPhysicalPort->basic_cfg_cb->port_type == ATG_DCI_ETH))
                {
                	ret = spm_bfd_update_common_from_physical_port_index(ulPortIndexKey NBB_CCXT);

					if (ret != SUCCESS)
					{                        
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 接口通用配置更新失败!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 接口通用配置更新失败!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_FUNC_ERROR, "CALL_FUNC_ERROR", ucMessage));
                        
                        pstSetPhysicalPort->common_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }
                
                /* 保存数据，每个槽位都存 */
                if (pstPhysicalPort->common_cfg_cb == NULL)
                {
                    pstPhysicalPort->common_cfg_cb = (ATG_DCI_PHY_PORT_COMMON_DATA*)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_COMMON_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_COMMON_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->common_cfg_cb, pstCommonData, sizeof(ATG_DCI_PHY_PORT_COMMON_DATA));
            }
        }
        else if (ulOperCommon== ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperCommon== ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** ALS配置 *********************************/
        if (ulOperAls == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucAlsDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->als_data);    

            /* 首地址为NULL，异常 */
            if (pucAlsDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucAlsDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstAlsData = (ATG_DCI_PHY_PORT_ALS_DATA *)pucAlsDataStart;      

                if (physical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  20）Als 配置\n");
                    spm_dbg_print_physical_port_als_cfg(pstAlsData);
                }

                OS_SPRINTF(ucMessage,"  20）Als 配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_physical_port_als_cfg(pstAlsData);
                
                NBB_TRC_DETAIL((NBB_FORMAT "  Als使能                    = %d", pstAlsData->als_enable));
                NBB_TRC_DETAIL((NBB_FORMAT "  接收信号丢失时间              = %d", pstAlsData->rev_lost_time));
                NBB_TRC_DETAIL((NBB_FORMAT "  激光器延时开启时间        = %d", pstAlsData->laser_delay_time));
                NBB_TRC_DETAIL((NBB_FORMAT "  发送时间                      = %d", pstAlsData->send_time));
                
#ifdef SPU
                //只有是Eth网类型的口才进入端口配置
                if ((pstPhysicalPort->basic_cfg_cb != NULL) 
                 && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id)
                 && (pstPhysicalPort->basic_cfg_cb->sub_board == 0))
                {
		            ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                          stAlsConf.als_en = pstAlsData->als_enable;
                          stAlsConf.revsiglos_time= pstAlsData->rev_lost_time;
                          stAlsConf.waiting_time= pstAlsData->laser_delay_time;
                          stAlsConf.sendsig_time= pstAlsData->send_time;
              
		            ret = SUCCESS;

		            ret += Als_Config(pstPhysicalPort->basic_cfg_cb->port, stAlsConf);
		            
			     if (ret != SUCCESS)
			     {                        
                              OS_PRINTF("***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 接口Als 配置失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                              OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld,  子卡%d 面板口%d 接口Als 配置失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                              BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                              NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_FUNC_ERROR, "CALL_FUNC_ERROR", ucMessage));
                        
                              pstSetPhysicalPort->als_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        
                              //goto EXIT_LABEL;
                         }
		  }

 #endif               
                 /* 保存数据 */
                if (pstPhysicalPort->als_cfg_cb == NULL)
                {
                        pstPhysicalPort->als_cfg_cb = (ATG_DCI_PHY_PORT_ALS_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_ALS_DATA),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_PHY_PORT_ALS_CB);
                }               

                OS_MEMCPY(pstPhysicalPort->als_cfg_cb, pstAlsData, sizeof(ATG_DCI_PHY_PORT_ALS_DATA));

            }         
        }
        
        else if (ulOperAls== ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperAls == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        
#if 0
        /******************************** IPV6流采样镜像 IPV6 NetFlow sampler ********************************/
        if (ulOperIpv6Sample == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucIpv6SampleDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                                  &pstSetPhysicalPort->ipv6nf_sampler_data);    

            /* 首地址为NULL，异常 */
            if (pucIpv6SampleDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6SampleDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstIpv6SampleData = (ATG_DCI_PHY_PORT_IPV6NF_SAMPLER*)pucIpv6SampleDataStart;    

#ifdef SPU
                /* 配置处理，每个槽位都建 */                
                if (pstPhysicalPort->basic_cfg_cb != NULL)
                {
                    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
                    
                    ret = spm_hardw_getc3lineport(ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, 
                    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);

                    if (ret == SUCCESS)
                    {
                        stSubPort.port = stPort.ucPort;
                        stSubPort.slot = pstPhysicalPort->basic_cfg_cb->slot;
                        stSubPort.unit = stPort.ucUnit;

                        spm_set_ipv6_sampler(&stSubPort, pstIpv6SampleData NBB_CCXT);
                    }
                    else
                    {
                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);

                        OS_SPRINTF(ucMessage, "***ERROR***:PORT_INDEX=%ld, 子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ulPortIndexKey, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, UNABLE_GET_C3_PORT, "UNABLE_GET_C3_PORT", ucMessage));
                        
                        pstSetPhysicalPort->ipv6nf_sampler_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        //goto EXIT_LABEL;
                    }
                }      
#endif

                /* 保存数据 */
                if (pstPhysicalPort->ipv6_sample_cfg_cb == NULL)
                {
                    pstPhysicalPort->ipv6_sample_cfg_cb = (ATG_DCI_PHY_PORT_IPV6NF_SAMPLER *)NBB_MM_ALLOC(sizeof(ATG_DCI_PHY_PORT_IPV6NF_SAMPLER),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHY_PORT_IPV6_SAMPLE_CB);
                }  
                
                OS_MEMCPY(pstPhysicalPort->ipv6_sample_cfg_cb, pstIpv6SampleData, sizeof(ATG_DCI_PHY_PORT_IPV6NF_SAMPLER));

            }
        }
        else if (ulOperIpv6Sample == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperIpv6Sample == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }
#endif

        //如果为仿真盘或wan盘才发送IPS消息给子卡
        if (pstPhysicalPort->basic_cfg_cb != NULL)
        {
            ucTdmSubCardExist = spm_hardw_getcesability(pstPhysicalPort->basic_cfg_cb->sub_board);
            
            if ((ucTdmSubCardExist == ATG_DCI_EXIST) && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id))
            {
                spm_send_physical_port_cfg(pstSetPhysicalPort, pstPhysicalPort->basic_cfg_cb->sub_board NBB_CCXT);
            }

            //als配置向所有子卡发送
            if ((0 != pstPhysicalPort->basic_cfg_cb->sub_board) && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id))
            {
                spm_send_physical_port_cfg(pstSetPhysicalPort, pstPhysicalPort->basic_cfg_cb->sub_board NBB_CCXT);
            }
            
        }
        
    }



/*
if ((ulPortIndexKey == 2) && (pstSetPhysicalPort->delete_struct == TRUE))
{
    sleep(5);
    sysinfo(&s_info2);
    printf("After:\n");
    printf("RAM: total %dk / free %dk\n",
          s_info2.totalram/1024, s_info2.freeram/1024);


    printf("free_After - free_Before = %dk\n", (s_info2.freeram/1024 - s_info1.freeram/1024));

}
*/
    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_del_physical_port_cfg
 功能描述  : 删除物理接口配置
 输入参数  : SPM_PHYSICAL_PORT* pstPhysicalPort  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月13日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_del_physical_port_cfg(SPM_PHYSICAL_PORT_CB* pstPhysicalPort NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_INT i = 0;
    NBB_BYTE ucPort = 0;
    NBB_BYTE ucSlot = 0;
    NBB_BYTE ucSubBoardNo = 0;
    SPM_PORT_STRUCT stPort;
    SUB_PORT stSubPort;
	EFM_PHY_PORT stEfmPort;
    SPM_PORT_WRED_KEY stWredKey;
    ATG_DCI_PHY_PORT_IPV4NF_DATA stIpv4NfData;
    ATG_DCI_PHY_PORT_IPNF_SAMPLER stIpv4SampleData;
    ATG_DCI_PHY_PORT_IPV6NF_DATA stIpv6NfData;
    
    //ATG_DCI_PHY_PORT_IPV6NF_SAMPLER stIpv6SampleData;
    ALS_CONFIG_INFO stAlsConf;
    
    NBB_TRC_ENTRY("spm_del_physical_port_cfg");

    if (pstPhysicalPort == NULL)
    {
    	goto EXIT_LABEL;
    }

    OS_MEMSET(&stPort, 0, sizeof(SPM_PORT_STRUCT));
    OS_MEMSET(&stSubPort, 0, sizeof(SUB_PORT));
	OS_MEMSET(&stEfmPort, 0, sizeof(EFM_PHY_PORT));
    OS_MEMSET(&stWredKey, 0, sizeof(SPM_PORT_WRED_KEY));
    OS_MEMSET(&stIpv4NfData, 0, sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA));
    OS_MEMSET(&stIpv4SampleData, 0, sizeof(ATG_DCI_PHY_PORT_IPNF_SAMPLER));
    OS_MEMSET(&stIpv6NfData, 0, sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA));
    
   // OS_MEMSET(&stIpv6SampleData, 0, sizeof(ATG_DCI_PHY_PORT_IPV6NF_SAMPLER));
   OS_MEMSET(&stAlsConf, 0, sizeof(ALS_CONFIG_INFO));
    

    //如果基本配置不存在，则不需要处理
    if (pstPhysicalPort->basic_cfg_cb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:接口物理配置的基本配置不存在"));

        OS_PRINTF("***ERROR***:要删除的PORT_INDEX=%ld 的接口物理配置的基本配置并不存在!\n", pstPhysicalPort->port_index_key);

        OS_SPRINTF(ucMessage, "***ERROR***:要删除的PORT_INDEX=%ld 的接口物理配置的基本配置并不存在!\n", pstPhysicalPort->port_index_key);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", pstPhysicalPort->port_index_key, BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));

        ret = ERROR;
        
        goto EXIT_LABEL;
    }    

    //删除dev_port_info中相应的物理端口数据
    spm_del_dev_port_info(pstPhysicalPort->basic_cfg_cb NBB_CCXT);

    ucPort = pstPhysicalPort->basic_cfg_cb->port;
    ucSlot = pstPhysicalPort->basic_cfg_cb->slot;
    ucSubBoardNo = pstPhysicalPort->basic_cfg_cb->sub_board;
    
    ret = spm_hardw_getc3lineport(ucSubBoardNo, ucPort, 
    		(LOGIC_PORT_ATTRI)pstPhysicalPort->basic_cfg_cb->circuit_emu_mode, &stPort);
    
    if (ret != SUCCESS)
    {                        
        OS_PRINTF("***ERROR***: 删除子卡%d 面板口%d 获取芯片口失败!退出处理!\n", 
            ucSubBoardNo, ucPort);

        OS_SPRINTF(ucMessage, 
            "***ERROR***:  删除子卡%d 面板口%d 获取芯片口失败!退出处理!\n", ucSubBoardNo, ucPort);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
             
        //goto EXIT_LABEL;
    }
    
    stSubPort.port = stPort.ucPort;
    stSubPort.slot = ucSlot;
    stSubPort.unit = stPort.ucUnit;

	stEfmPort.cases = pstPhysicalPort->basic_cfg_cb->subrack;
	stEfmPort.cards = pstPhysicalPort->basic_cfg_cb->sub_board;
	stEfmPort.slots = ucSlot;
	stEfmPort.ports = stPort.ucPort;
    
    //如果在本槽位
    if (ucSlot == SHARED.local_slot_id)
    {
        //删除以太网接口报文抑制
        if (pstPhysicalPort->msg_suppr_cfg_cb != NULL)
        {
            spm_set_port_flood_control(&stSubPort, 
                                       0,
                                       0,
                                       0, 
                                       pstPhysicalPort->msg_suppr_cfg_cb->bc_suppr_flow,
                                       pstPhysicalPort->msg_suppr_cfg_cb->mc_suppr_flow,
                                       pstPhysicalPort->msg_suppr_cfg_cb->fd_suppr_flow
                                       NBB_CCXT);   
        }
    }

    //删除时间同步端口配置
    if (pstPhysicalPort->time_sync_cfg_cb != NULL)
    {
        spm_ptp_delete_port(stSubPort.slot, ucSubBoardNo, pstPhysicalPort->basic_cfg_cb->port, stSubPort.port);
    }

	//删除以太网EFM OAM，每个槽位都调接口，由接口内判断
    if (pstPhysicalPort->efm_oam_cfg_cb != NULL)
    {        
		//spm_set_efm_oam(&stEfmPort, NULL);
    }

    //删除以太网出口队列拥塞策略，WRED是每个槽位都建了，所以每个槽位都要删除
    if (pstPhysicalPort->congest_policy_cfg_cb != NULL)
    {
        stWredKey.index = pstPhysicalPort->port_index_key;
        
        for (i=0; i<ATG_DCI_QOS_BEHAVIOR_CONGST_NUM; i++)
        {
            stWredKey.cos = i;
            spm_set_port_wred(&stWredKey, &stSubPort, NULL NBB_CCXT);
        } 
    }

    //删除IPV4流采样使能配置IPV4 NetFlow
    if (pstPhysicalPort->ipv4_nf_cfg_cb != NULL)
    {
        spm_set_ipv4_nf_enable(&stSubPort, &stIpv4NfData);
    }
    
    //删除IPV4流采样镜像 IPV4 NetFlow sampler
    if (pstPhysicalPort->ip_sample_cfg_cb != NULL)
    {
        spm_set_ipv4_sampler(&stSubPort, &stIpv4SampleData NBB_CCXT);
    }
    
    //删除IPV6流采样使能配置 IPV6 NetFlow
    if (pstPhysicalPort->ipv6_nf_cfg_cb != NULL)
    {
        spm_set_ipv6_nf_enable(&stSubPort, &stIpv6NfData);
    }
    
    //删除IPV6流采样镜像IPV6 NetFlow sampler
    //if (pstPhysicalPort->ipv6_sample_cfg_cb != NULL)
   // {
    //    spm_set_ipv6_sampler(&stSubPort, &stIpv6SampleData NBB_CCXT);
    //}

    //删除IPV6流采样镜像IPV6 NetFlow sampler
    if (pstPhysicalPort->als_cfg_cb != NULL)
    {
        #ifdef SPU
        ret += Als_Config(ucPort, stAlsConf);
        #endif
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_send_physical_port_cfg
 功能描述  : 向子卡发送物理接口配置中的仿真配置
 输入参数  : ATG_DCI_SET_PHYSICAL_PORT *pstSetPhysicalPort  
             NBB_BYTE ucSubBoardNo                          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年12月5日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_physical_port_cfg(ATG_DCI_SET_PHYSICAL_PORT *pstSetPhysicalPort, NBB_BYTE ucSubBoardNo NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    
    /* 获取的子配置 */
    ATG_SBI_PHY_PORT_BASIC_DATA *pstSbmBasicData       	= NULL;
    ATG_SBI_PHY_PORT_PHY_DATA *pstSbmPhyData           	= NULL;
    ATG_SBI_PHY_PORT_POS_LINK_DATA *pstSbmPosData      	= NULL;
    ATG_SBI_PHY_PORT_STM1_OVHD_DATA *pstSbmSdhSovhdData = NULL;
    ATG_SBI_PHY_PORT_CES_DATA *pstSbmCesData           	= NULL;
    ATG_SBI_PHY_PORT_CEP_OVHD_DATA *pstSbmCepData      	= NULL;
    ATG_SBI_PHY_PORT_SDH_TOVHD_DATA *pstSbmSdhTovhdData = NULL;
    ATG_SBI_PHY_PORT_ALS_DATA *pstSbmAlsData = NULL;

    ATG_DCI_PHY_PORT_BASIC_DATA *pstBasicData       	= NULL;
    ATG_DCI_PHY_PORT_PHY_DATA *pstPhyData           	= NULL;
    ATG_DCI_PHY_PORT_POS_LINK_DATA *pstPosData      	= NULL;
    ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *pstSdhSovhdData    = NULL;
    ATG_DCI_PHY_PORT_CES_DATA *pstCesData           	= NULL;
    ATG_DCI_PHY_PORT_CEP_OVHD_DATA *pstCepData      	= NULL;   
    ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *pstSdhTovhdData    = NULL;
    ATG_DCI_PHY_PORT_ALS_DATA *pstAlsData = NULL;
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucSbmBasicDataStart = NULL;
    NBB_BYTE *pucSbmPhyDataStart   = NULL;
    NBB_BYTE *pucSbmPosDataStart   = NULL;
    NBB_BYTE *pucSbmSdhSovhdDataStart  = NULL;
    NBB_BYTE *pucSbmCesDataStart   = NULL;
    NBB_BYTE *pucSbmCepDataStart   = NULL;   
    NBB_BYTE *pucSbmSdhTovhdDataStart  = NULL;
    NBB_BYTE *pucSbmAlsDataStart  = NULL;

    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucPhyDataStart   = NULL;
    NBB_BYTE *pucPosDataStart   = NULL;
    NBB_BYTE *pucSdhSovhdDataStart  = NULL;
    NBB_BYTE *pucCesDataStart   = NULL;
    NBB_BYTE *pucCepDataStart   = NULL;
    NBB_BYTE *pucSdhTovhdDataStart  = NULL;
    NBB_BYTE *pucAlsDataStart  = NULL;

    NBB_BYTE ucBasic = 1;
    NBB_BYTE ucPhy   = 1;
    NBB_BYTE ucPos   = 1;
    NBB_BYTE ucSdhSovhd  = 1;
    NBB_BYTE ucCes   = 1;
    NBB_BYTE ucCep   = 1;
    NBB_BYTE ucSdhTovhd  = 1;
    NBB_BYTE ucAls  = 1;
    
    NBB_BUF_SIZE buf_size = 0;
    NBB_BUF_SIZE size_array[OFL_ATG_SBI_SET_PHYSICAL_PORT + 1];

    ATG_SBI_SET_PHYSICAL_PORT *pstSetSbiPhysicalPort = NULL;
    
    NBB_TRC_ENTRY("spm_send_physical_port_cfg");

    if (pstSetPhysicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_send_physical_port_cfg(pstSetPhysicalPort==NULL)"));

        OS_PRINTF("***ERROR***:spm_send_physical_port_cfg(pstSetPhysicalPort==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_send_physical_port_cfg(pstSetPhysicalPort==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, ucMessage));
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /*判断basic_data是否有数据。                                               */
    /***************************************************************************/ 
    if (pstSetPhysicalPort->oper_basic == ATG_DCI_OPER_NULL)
    {
        ucBasic = 0;
    }

    /***************************************************************************/
    /*判断physical_data是否有数据。                                            */
    /***************************************************************************/  
    if (pstSetPhysicalPort->oper_physical == ATG_DCI_OPER_NULL)
    {
        ucPhy = 0;
    }
    
    /***************************************************************************/
    /*判断pos_link_data是否有数据。                                            */
    /***************************************************************************/ 
    if (pstSetPhysicalPort->oper_pos_link == ATG_DCI_OPER_NULL)
    {
        ucPos = 0;
    }

    /***************************************************************************/
    /*判断sdh_soverhead_data是否有数据。                                       */
    /***************************************************************************/ 
    if (pstSetPhysicalPort->oper_sdh_soverhead == ATG_DCI_OPER_NULL)
    {
        ucSdhSovhd = 0;
    }

    /***************************************************************************/
    /*判断sdh_toverhead_data是否有数据。                                       */
    /***************************************************************************/ 
    if (pstSetPhysicalPort->oper_sdh_toverhead == ATG_DCI_OPER_NULL)
    {
        ucSdhTovhd = 0;
    }

    /***************************************************************************/
    /*判断ces_data是否有数据。                                                 */
    /***************************************************************************/ 
    if (pstSetPhysicalPort->oper_ces == ATG_DCI_OPER_NULL)
    {
        ucCes = 0;
    }

    /***************************************************************************/
    /*判断cep_overhead_data是否有数据。                                        */
    /***************************************************************************/ 
    if (pstSetPhysicalPort->oper_cep_overhead == ATG_DCI_OPER_NULL)
    {
        ucCep = 0;
    }
    
    /***************************************************************************/
    /*判断ALS_data是否有数据。                                                 */
    /***************************************************************************/ 
    if (pstSetPhysicalPort->oper_als== ATG_DCI_OPER_NULL)
    {
        ucAls = 0;
    }
    buf_size = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_SET_PHYSICAL_PORT))
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_BASIC_DATA))) * ucBasic
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_PHY_DATA))) * ucPhy
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_POS_LINK_DATA))) * ucPos
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_STM1_OVHD_DATA))) * ucSdhSovhd
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_CES_DATA))) * ucCes
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_CEP_OVHD_DATA))) * ucCep
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_SDH_TOVHD_DATA))) * ucSdhTovhd
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_ALS_DATA))) * ucAls;
        
    pstSetSbiPhysicalPort = (ATG_SBI_SET_PHYSICAL_PORT *) NBB_GET_BUFFER(NBB_NULL_HANDLE,
                            buf_size, 0, NBB_NORETRY_ACT | NBB_BUF_INTRN);
    
    /***************************************************************************/
    /*判断set_phy_port的值是否有效。*/
    /***************************************************************************/        
    if (pstSetSbiPhysicalPort == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to alloc set physical port buffer"));
        goto EXIT_LABEL;
    }
  
    /*************************************************************************/
    /* 初始化消息。                                                          */
    /*************************************************************************/
    NBB_ZERO_IPS(pstSetSbiPhysicalPort);

    /***************************************************************************/
    /* 初始化offsets。                                                         */
    /***************************************************************************/
    size_array[0] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_SET_PHYSICAL_PORT));
    size_array[1] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_BASIC_DATA)) * ucBasic;
    size_array[2] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_PHY_DATA)) * ucPhy;
    size_array[3] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_POS_LINK_DATA)) * ucPos;
    size_array[4] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_STM1_OVHD_DATA)) * ucSdhSovhd;
    size_array[5] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_CES_DATA)) * ucCes;
    size_array[6] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_CEP_OVHD_DATA)) * ucCep;
    size_array[7] = 0;
    size_array[8] = 0;
    size_array[9] = 0;
    size_array[10] = 0;
    size_array[11] = 0;
    size_array[12] = 0;
    size_array[13] = 0;
    size_array[14] = 0;
    size_array[15] = 0;
    size_array[16] = 0;
    size_array[17] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_SDH_TOVHD_DATA)) * ucSdhTovhd;
    size_array[18] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_ALS_DATA)) * ucAls;
    size_array[19] = NTL_OFF_SIZE_ARRAY_TERMINATOR;

    NBB_ASSERT(OFL_ATG_SBI_SET_PHYSICAL_PORT == 19);

    NTL_OFF_INIT_STRUCT(pstSetSbiPhysicalPort, size_array, off_atg_sbi_set_physical_port);

    pstSetSbiPhysicalPort->ips_hdr.ips_type     = IPS_ATG_SBI_SET_PHYSICAL_PORT;
    pstSetSbiPhysicalPort->delete_struct        = pstSetPhysicalPort->delete_struct;
    pstSetSbiPhysicalPort->key                  = pstSetPhysicalPort->key;
    pstSetSbiPhysicalPort->oper_basic           = pstSetPhysicalPort->oper_basic;
    pstSetSbiPhysicalPort->oper_physical        = pstSetPhysicalPort->oper_physical;
    pstSetSbiPhysicalPort->oper_pos_link        = pstSetPhysicalPort->oper_pos_link;
    pstSetSbiPhysicalPort->oper_stm1_overhead   = pstSetPhysicalPort->oper_sdh_soverhead;
    pstSetSbiPhysicalPort->oper_ces             = pstSetPhysicalPort->oper_ces;
    pstSetSbiPhysicalPort->oper_cep_overhead    = pstSetPhysicalPort->oper_cep_overhead;
    pstSetSbiPhysicalPort->oper_sdh_toverhead   = pstSetPhysicalPort->oper_sdh_toverhead;
    pstSetSbiPhysicalPort->oper_als                     = pstSetPhysicalPort->oper_als;

    /***************************************************************************/
    /* 获得指向基础配置的指针。                                                */
    /***************************************************************************/
    if (ucBasic == 1)
    {
        pucSbmBasicDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                    &(pstSetSbiPhysicalPort->basic_data));  
        pstSbmBasicData = (ATG_SBI_PHY_PORT_BASIC_DATA*)pucSbmBasicDataStart;
        
        pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                               &pstSetPhysicalPort->basic_data);
        pstBasicData = (ATG_DCI_PHY_PORT_BASIC_DATA*)pucBasicDataStart;

		if ((pstSbmBasicData != NULL) && (pstBasicData != NULL))
		{
        	OS_MEMCPY(pstSbmBasicData, pstBasicData, sizeof(ATG_SBI_PHY_PORT_BASIC_DATA));     
        }
    }    
    
    /***************************************************************************/
    /* 获得指向以太网接口物理配置的指针。                                      */
    /***************************************************************************/
    if (ucPhy == 1)
    {
        pucSbmPhyDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->physical_data));  
        pstSbmPhyData = (ATG_SBI_PHY_PORT_PHY_DATA*)pucSbmPhyDataStart;

        pucPhyDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                             &pstSetPhysicalPort->physical_data);          
        pstPhyData = (ATG_DCI_PHY_PORT_PHY_DATA*)pucPhyDataStart;

        if ((pstSbmPhyData != NULL) && (pstPhyData != NULL))
        {
        	OS_MEMCPY(pstSbmPhyData, pstPhyData, sizeof(ATG_SBI_PHY_PORT_PHY_DATA));
        }
    }

    /***************************************************************************/
    /* 获得指向POS接口链路层配置的指针。                                       */
    /***************************************************************************/
    if (ucPos == 1)
    {
        pucSbmPosDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->pos_link_data));  
        pstSbmPosData = (ATG_SBI_PHY_PORT_POS_LINK_DATA*)pucSbmPosDataStart;

        pucPosDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                             &pstSetPhysicalPort->pos_link_data);        
        pstPosData = (ATG_DCI_PHY_PORT_POS_LINK_DATA*)pucPosDataStart;

        if ((pstSbmPosData != NULL) && (pstPosData != NULL))
        {
        	OS_MEMCPY(pstSbmPosData, pstPosData, sizeof(ATG_SBI_PHY_PORT_POS_LINK_DATA));
        }
    }
    
    /***************************************************************************/
    /* 获得指向SDH段开销配置的指针。                                       */
    /***************************************************************************/
    if (ucSdhSovhd == 1)
    {
        pucSbmSdhSovhdDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                   &(pstSetSbiPhysicalPort->stm1_overhead_data)); 
        pstSbmSdhSovhdData = (ATG_SBI_PHY_PORT_STM1_OVHD_DATA*)pucSbmSdhSovhdDataStart;

        pucSdhSovhdDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                              &pstSetPhysicalPort->sdh_soverhead_data);
        pstSdhSovhdData = (ATG_DCI_PHY_PORT_SDH_SOVHD_DATA*)pucSdhSovhdDataStart;

        if ((pstSbmSdhSovhdData != NULL) && (pstSdhSovhdData != NULL))
        {
        	OS_MEMCPY(pstSbmSdhSovhdData, pstSdhSovhdData, sizeof(ATG_SBI_PHY_PORT_STM1_OVHD_DATA));
        }
    }

    /***************************************************************************/
    /* 获得指向SDH通道开销配置的指针。                                       */
    /***************************************************************************/
    if (ucSdhTovhd == 1)
    {
        pucSbmSdhTovhdDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                   &(pstSetSbiPhysicalPort->sdh_toverhead_data)); 
        pstSbmSdhTovhdData = (ATG_SBI_PHY_PORT_SDH_TOVHD_DATA*)pucSbmSdhTovhdDataStart;

        pucSdhTovhdDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                              &pstSetPhysicalPort->sdh_toverhead_data);
        pstSdhTovhdData = (ATG_DCI_PHY_PORT_SDH_TOVHD_DATA*)pucSdhTovhdDataStart;

        if ((pstSbmSdhTovhdData != NULL) && (pstSdhTovhdData != NULL))
        {
        	OS_MEMCPY(pstSbmSdhTovhdData, pstSdhTovhdData, sizeof(ATG_SBI_PHY_PORT_SDH_TOVHD_DATA));
        }
    }
    
    /***************************************************************************/
    /* 获得指向CES接口配置的指针。                                             */
    /***************************************************************************/
    if (ucCes == 1)
    {
        pucSbmCesDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->ces_data)); 
        pstSbmCesData = (ATG_SBI_PHY_PORT_CES_DATA*)pucSbmCesDataStart;

        pucCesDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                             &pstSetPhysicalPort->ces_data);                                                          
        pstCesData = (ATG_DCI_PHY_PORT_CES_DATA*)pucCesDataStart;

        if ((pstSbmCesData != NULL) && (pstCesData != NULL))
        {
        	OS_MEMCPY(pstSbmCesData, pstCesData, sizeof(ATG_SBI_PHY_PORT_CES_DATA));
        }
    }    

    /***************************************************************************/
    /* 获得指向CEP接口开销配置的指针。                                         */
    /***************************************************************************/
    if (ucCep == 1)
    {
        pucSbmCepDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->cep_overhead_data)); 
        pstSbmCepData = (ATG_SBI_PHY_PORT_CEP_OVHD_DATA*)pucSbmCepDataStart;

        pucCepDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                          &pstSetPhysicalPort->cep_overhead_data);        
        pstCepData = (ATG_DCI_PHY_PORT_CEP_OVHD_DATA*)pucCepDataStart;

        if ((pstSbmCepData != NULL) && (pstCepData != NULL))
        {
        	OS_MEMCPY(pstSbmCepData, pstCepData, sizeof(ATG_SBI_PHY_PORT_CEP_OVHD_DATA));
        }
    }

    /***************************************************************************/
    /* 获得指向als 配置的指针。                                         */
    /***************************************************************************/
    if (ucAls== 1)
    {
        pucSbmAlsDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->als_data)); 
        pstSbmAlsData = (ATG_SBI_PHY_PORT_ALS_DATA*)pucSbmAlsDataStart;

        pucAlsDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                          &pstSetPhysicalPort->als_data);        
        pstAlsData = (ATG_DCI_PHY_PORT_ALS_DATA*)pucAlsDataStart;

        if ((pstSbmAlsData != NULL) && (pstAlsData != NULL))
        {
        	OS_MEMCPY(pstSbmAlsData, pstAlsData, sizeof(ATG_SBI_PHY_PORT_ALS_DATA));
        }
    }
    
    if (SHARED.sub_card_cb[ucSubBoardNo-1] != NULL)
    {
        spm_snd_sbi_ips(SHARED.sub_card_cb[ucSubBoardNo-1], (NBB_VOID*)pstSetSbiPhysicalPort, &(pstSetSbiPhysicalPort->ips_hdr) NBB_CCXT);
    }    
    else
    {
    	NBB_FREE_BUFFER(pstSetSbiPhysicalPort);
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return 0;
}


/*****************************************************************************
 函 数 名  : spm_send_physical_port_cfg_from_local
 功能描述  : 向子卡发送物理接口配置中的仿真配置
 输入参数  : ATG_DCI_SET_PHYSICAL_PORT *pstSetPhysicalPort  
             NBB_BYTE ucSubBoardNo                          
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年1月27日
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_send_physical_port_cfg_from_local(SPM_PHYSICAL_PORT_CB *pstPhysicalPort, 
    NBB_BYTE ucSubBoardNo NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    
    /* 获取的子配置 */
    ATG_SBI_PHY_PORT_BASIC_DATA *pstSbmBasicData       	= NULL;
    ATG_SBI_PHY_PORT_PHY_DATA *pstSbmPhyData           	= NULL;
    ATG_SBI_PHY_PORT_POS_LINK_DATA *pstSbmPosData      	= NULL;
    ATG_SBI_PHY_PORT_STM1_OVHD_DATA *pstSbmSdhSovhdData = NULL;
    ATG_SBI_PHY_PORT_CES_DATA *pstSbmCesData           	= NULL;
    ATG_SBI_PHY_PORT_CEP_OVHD_DATA *pstSbmCepData      	= NULL;
    ATG_SBI_PHY_PORT_SDH_TOVHD_DATA *pstSbmSdhTovhdData = NULL;
    ATG_SBI_PHY_PORT_ALS_DATA *pstSbmAlsData = NULL;

    ATG_DCI_PHY_PORT_BASIC_DATA *pstBasicData       	= NULL;
    ATG_DCI_PHY_PORT_PHY_DATA *pstPhyData           	= NULL;
    ATG_DCI_PHY_PORT_POS_LINK_DATA *pstPosData      	= NULL;
    ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *pstSdhSovhdData    = NULL;
    ATG_DCI_PHY_PORT_CES_DATA *pstCesData           	= NULL;
    ATG_DCI_PHY_PORT_CEP_OVHD_DATA *pstCepData      	= NULL;   
    ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *pstSdhTovhdData    = NULL;
    ATG_DCI_PHY_PORT_ALS_DATA *pstAlsData = NULL;
    
    /* IPS消息偏移的首地址 */
    NBB_BYTE *pucSbmBasicDataStart = NULL;
    NBB_BYTE *pucSbmPhyDataStart   = NULL;
    NBB_BYTE *pucSbmPosDataStart   = NULL;
    NBB_BYTE *pucSbmSdhSovhdDataStart  = NULL;
    NBB_BYTE *pucSbmCesDataStart   = NULL;
    NBB_BYTE *pucSbmCepDataStart   = NULL;   
    NBB_BYTE *pucSbmSdhTovhdDataStart  = NULL;
    NBB_BYTE *pucSbmAlsDataStart  = NULL;

    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucPhyDataStart   = NULL;
    NBB_BYTE *pucPosDataStart   = NULL;
    NBB_BYTE *pucSdhSovhdDataStart  = NULL;
    NBB_BYTE *pucCesDataStart   = NULL;
    NBB_BYTE *pucCepDataStart   = NULL;
    NBB_BYTE *pucSdhTovhdDataStart  = NULL;
    NBB_BYTE *pucAlsDataStart  = NULL;

    NBB_BYTE ucBasic = 1;
    NBB_BYTE ucPhy   = 1;
    NBB_BYTE ucPos   = 1;
    NBB_BYTE ucSdhSovhd  = 1;
    NBB_BYTE ucCes   = 1;
    NBB_BYTE ucCep   = 1;
    NBB_BYTE ucSdhTovhd  = 1;
    NBB_BYTE ucAls  = 1;
    
    NBB_BUF_SIZE buf_size = 0;
    NBB_BUF_SIZE size_array[OFL_ATG_SBI_SET_PHYSICAL_PORT + 1];

    ATG_SBI_SET_PHYSICAL_PORT *pstSetSbiPhysicalPort = NULL;
    
    NBB_TRC_ENTRY("spm_send_physical_port_cfg");

    if (pstPhysicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_send_physical_port_cfg(pstSetPhysicalPort==NULL)"));

        OS_PRINTF("***ERROR***:spm_send_physical_port_cfg(pstSetPhysicalPort==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_send_physical_port_cfg(pstSetPhysicalPort==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL, ucMessage));
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /*判断basic_data是否有数据。                                               */
    /***************************************************************************/ 
    if (pstPhysicalPort->basic_cfg_cb == NULL)
    {
        ucBasic = 0;
    }

    /***************************************************************************/
    /*判断physical_data是否有数据。                                            */
    /***************************************************************************/  
    if (pstPhysicalPort->phy_cfg_cb == NULL)
    {
        ucPhy = 0;
    }
    
    /***************************************************************************/
    /*判断pos_link_data是否有数据。                                            */
    /***************************************************************************/ 
    if (pstPhysicalPort->pos_link_cfg_cb == NULL)
    {
        ucPos = 0;
    }

    /***************************************************************************/
    /*判断sdh_soverhead_data是否有数据。                                       */
    /***************************************************************************/ 
    if (pstPhysicalPort->sdh_sovhd_cfg_cb == NULL)
    {
        ucSdhSovhd = 0;
    }

    /***************************************************************************/
    /*判断sdh_toverhead_data是否有数据。                                       */
    /***************************************************************************/ 
    if (pstPhysicalPort->sdh_tovhd_cfg_cb == NULL)
    {
        ucSdhTovhd = 0;
    }

    /***************************************************************************/
    /*判断ces_data是否有数据。                                                 */
    /***************************************************************************/ 
    if (pstPhysicalPort->ces_cfg_cb == NULL)
    {
        ucCes = 0;
    }

    /***************************************************************************/
    /*判断cep_overhead_data是否有数据。                                        */
    /***************************************************************************/ 
    if (pstPhysicalPort->cep_ovhd_cfg_cb == NULL)
    {
        ucCep = 0;
    }
    
    /***************************************************************************/
    /*判断ALS_data是否有数据。                                                 */
    /***************************************************************************/ 
    if (pstPhysicalPort->als_cfg_cb == NULL)
    {
        ucAls = 0;
    }
    buf_size = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_SET_PHYSICAL_PORT))
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_BASIC_DATA))) * ucBasic
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_PHY_DATA))) * ucPhy
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_POS_LINK_DATA))) * ucPos
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_STM1_OVHD_DATA))) * ucSdhSovhd
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_CES_DATA))) * ucCes
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_CEP_OVHD_DATA))) * ucCep
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_SDH_TOVHD_DATA))) * ucSdhTovhd
            + (NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_ALS_DATA))) * ucAls;
        
    pstSetSbiPhysicalPort = (ATG_SBI_SET_PHYSICAL_PORT *) NBB_GET_BUFFER(NBB_NULL_HANDLE,
                            buf_size, 0, NBB_NORETRY_ACT | NBB_BUF_INTRN);
    
    /***************************************************************************/
    /*判断set_phy_port的值是否有效。*/
    /***************************************************************************/        
    if (pstSetSbiPhysicalPort == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to alloc set physical port buffer"));
        goto EXIT_LABEL;
    }
  
    /*************************************************************************/
    /* 初始化消息。                                                          */
    /*************************************************************************/
    NBB_ZERO_IPS(pstSetSbiPhysicalPort);

    /***************************************************************************/
    /* 初始化offsets。                                                         */
    /***************************************************************************/
    size_array[0] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_SET_PHYSICAL_PORT));
    size_array[1] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_BASIC_DATA)) * ucBasic;
    size_array[2] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_PHY_DATA)) * ucPhy;
    size_array[3] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_POS_LINK_DATA)) * ucPos;
    size_array[4] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_STM1_OVHD_DATA)) * ucSdhSovhd;
    size_array[5] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_CES_DATA)) * ucCes;
    size_array[6] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_CEP_OVHD_DATA)) * ucCep;
    size_array[7] = 0;
    size_array[8] = 0;
    size_array[9] = 0;
    size_array[10] = 0;
    size_array[11] = 0;
    size_array[12] = 0;
    size_array[13] = 0;
    size_array[14] = 0;
    size_array[15] = 0;
    size_array[16] = 0;
    size_array[17] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_SDH_TOVHD_DATA)) * ucSdhTovhd;
    size_array[18] = NBB_ALIGN_OFFSET(sizeof(ATG_SBI_PHY_PORT_ALS_DATA)) * ucAls;
    size_array[19] = NTL_OFF_SIZE_ARRAY_TERMINATOR;

    NBB_ASSERT(OFL_ATG_SBI_SET_PHYSICAL_PORT == 19);

    NTL_OFF_INIT_STRUCT(pstSetSbiPhysicalPort, size_array, off_atg_sbi_set_physical_port);

    pstSetSbiPhysicalPort->ips_hdr.ips_type     = IPS_ATG_SBI_SET_PHYSICAL_PORT;
    pstSetSbiPhysicalPort->delete_struct        = FALSE;
    pstSetSbiPhysicalPort->key                  = pstPhysicalPort->port_index_key;
    
    if(1 == ucBasic)
    {
        pstSetSbiPhysicalPort->oper_basic = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiPhysicalPort->oper_basic       = ATG_DCI_OPER_NULL;
    } 
    
    if(1 == ucPhy)
    {
        pstSetSbiPhysicalPort->oper_physical = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiPhysicalPort->oper_physical       = ATG_DCI_OPER_NULL;
    } 

    if(1 == ucPos)
    {
        pstSetSbiPhysicalPort->oper_pos_link = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiPhysicalPort->oper_pos_link       = ATG_DCI_OPER_NULL;
    } 
    
    if(1 == ucSdhSovhd)
    {
        pstSetSbiPhysicalPort->oper_stm1_overhead = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiPhysicalPort->oper_stm1_overhead       = ATG_DCI_OPER_NULL;
    } 

    if(1 == ucCes)
    {
        pstSetSbiPhysicalPort->oper_ces = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiPhysicalPort->oper_ces       = ATG_DCI_OPER_NULL;
    } 
    
    if(1 == ucCep)
    {
        pstSetSbiPhysicalPort->oper_cep_overhead = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiPhysicalPort->oper_cep_overhead       = ATG_DCI_OPER_NULL;
    } 

    if(1 == ucSdhTovhd)
    {
        pstSetSbiPhysicalPort->oper_sdh_toverhead = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiPhysicalPort->oper_sdh_toverhead       = ATG_DCI_OPER_NULL;
    } 
    
    if(1 == ucAls)
    {
        pstSetSbiPhysicalPort->oper_als = ATG_DCI_OPER_ADD;
    }
    else
        
    {
        pstSetSbiPhysicalPort->oper_als       = ATG_DCI_OPER_NULL;
    } 

    /***************************************************************************/
    /* 获得指向基础配置的指针。                                                */
    /***************************************************************************/
    if (ucBasic == 1)
    {
        pucSbmBasicDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                    &(pstSetSbiPhysicalPort->basic_data));  
        pstSbmBasicData = (ATG_SBI_PHY_PORT_BASIC_DATA*)pucSbmBasicDataStart;
        
        //pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
                                                               //&pstSetPhysicalPort->basic_data);
        //pstBasicData = (ATG_DCI_PHY_PORT_BASIC_DATA*)pucBasicDataStart;

        if ((pstSbmBasicData != NULL) && (pstPhysicalPort->basic_cfg_cb != NULL))
        {
            OS_MEMCPY(pstSbmBasicData, pstPhysicalPort->basic_cfg_cb, sizeof(ATG_SBI_PHY_PORT_BASIC_DATA));     
        }
        
    }    
    
    /***************************************************************************/
    /* 获得指向以太网接口物理配置的指针。                                      */
    /***************************************************************************/
    if (ucPhy == 1)
    {
        pucSbmPhyDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->physical_data));  
        pstSbmPhyData = (ATG_SBI_PHY_PORT_PHY_DATA*)pucSbmPhyDataStart;

        //pucPhyDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
        //                                                     &pstSetPhysicalPort->physical_data);          
        //pstPhyData = (ATG_DCI_PHY_PORT_PHY_DATA*)pucPhyDataStart;

        if ((pstSbmPhyData != NULL) && (pstPhysicalPort->phy_cfg_cb != NULL))
        {
        	OS_MEMCPY(pstSbmPhyData, pstPhysicalPort->phy_cfg_cb, sizeof(ATG_SBI_PHY_PORT_PHY_DATA));
        }
    }

    /***************************************************************************/
    /* 获得指向POS接口链路层配置的指针。                                       */
    /***************************************************************************/
    if (ucPos == 1)
    {
        pucSbmPosDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->pos_link_data));  
        pstSbmPosData = (ATG_SBI_PHY_PORT_POS_LINK_DATA*)pucSbmPosDataStart;

        //pucPosDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
         //                                                    &pstSetPhysicalPort->pos_link_data);        
        //pstPosData = (ATG_DCI_PHY_PORT_POS_LINK_DATA*)pucPosDataStart;

        if ((pstSbmPosData != NULL) && (pstPhysicalPort->pos_link_cfg_cb != NULL))
        {
        	OS_MEMCPY(pstSbmPosData, pstPhysicalPort->pos_link_cfg_cb, sizeof(ATG_SBI_PHY_PORT_POS_LINK_DATA));
        }
    }
    
    /***************************************************************************/
    /* 获得指向SDH段开销配置的指针。                                       */
    /***************************************************************************/
    if (ucSdhSovhd == 1)
    {
        pucSbmSdhSovhdDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                   &(pstSetSbiPhysicalPort->stm1_overhead_data)); 
        pstSbmSdhSovhdData = (ATG_SBI_PHY_PORT_STM1_OVHD_DATA*)pucSbmSdhSovhdDataStart;

        //pucSdhSovhdDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
         //                                                     &pstSetPhysicalPort->sdh_soverhead_data);
        //pstSdhSovhdData = (ATG_DCI_PHY_PORT_SDH_SOVHD_DATA*)pucSdhSovhdDataStart;

        if ((pstSbmSdhSovhdData != NULL) && (pstPhysicalPort->sdh_sovhd_cfg_cb != NULL))
        {
        	OS_MEMCPY(pstSbmSdhSovhdData, pstPhysicalPort->sdh_sovhd_cfg_cb, sizeof(ATG_SBI_PHY_PORT_STM1_OVHD_DATA));
        }
    }

    /***************************************************************************/
    /* 获得指向SDH通道开销配置的指针。                                       */
    /***************************************************************************/
    if (ucSdhTovhd == 1)
    {
        pucSbmSdhTovhdDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                   &(pstSetSbiPhysicalPort->sdh_toverhead_data)); 
        pstSbmSdhTovhdData = (ATG_SBI_PHY_PORT_SDH_TOVHD_DATA*)pucSbmSdhTovhdDataStart;

        //pucSdhTovhdDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
        //                                                      &pstSetPhysicalPort->sdh_toverhead_data);
        //pstSdhTovhdData = (ATG_DCI_PHY_PORT_SDH_TOVHD_DATA*)pucSdhTovhdDataStart;

        if ((pstSbmSdhTovhdData != NULL) && (pstPhysicalPort->sdh_tovhd_cfg_cb != NULL))
        {
        	OS_MEMCPY(pstSbmSdhTovhdData, pstPhysicalPort->sdh_tovhd_cfg_cb, sizeof(ATG_SBI_PHY_PORT_SDH_TOVHD_DATA));
        }
    }
    
    /***************************************************************************/
    /* 获得指向CES接口配置的指针。                                             */
    /***************************************************************************/
    if (ucCes == 1)
    {
        pucSbmCesDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->ces_data)); 
        pstSbmCesData = (ATG_SBI_PHY_PORT_CES_DATA*)pucSbmCesDataStart;

        //pucCesDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort,
        //pstCesData = (ATG_DCI_PHY_PORT_CES_DATA*)pucCesDataStart;

        if ((pstSbmCesData != NULL) && (pstPhysicalPort->ces_cfg_cb != NULL))
        {
        	OS_MEMCPY(pstSbmCesData, pstPhysicalPort->ces_cfg_cb, sizeof(ATG_SBI_PHY_PORT_CES_DATA));
        }
    }    

    /***************************************************************************/
    /* 获得指向CEP接口开销配置的指针。                                         */
    /***************************************************************************/
    if (ucCep == 1)
    {
        pucSbmCepDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->cep_overhead_data)); 
        pstSbmCepData = (ATG_SBI_PHY_PORT_CEP_OVHD_DATA*)pucSbmCepDataStart;

        //pucCepDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
         //                                                 &pstSetPhysicalPort->cep_overhead_data);        
        //pstCepData = (ATG_DCI_PHY_PORT_CEP_OVHD_DATA*)pucCepDataStart;

        if ((pstSbmCepData != NULL) && (pstPhysicalPort->cep_ovhd_cfg_cb != NULL))
        {
        	OS_MEMCPY(pstSbmCepData, pstPhysicalPort->cep_ovhd_cfg_cb, sizeof(ATG_SBI_PHY_PORT_CEP_OVHD_DATA));
        }
    }

    /***************************************************************************/
    /* 获得指向als 配置的指针。                                         */
    /***************************************************************************/
    if (ucAls == 1)
    {
        pucSbmAlsDataStart = NTL_OFFLEN_GET_POINTER(pstSetSbiPhysicalPort,
                                                  &(pstSetSbiPhysicalPort->als_data)); 
        pstSbmAlsData = (ATG_SBI_PHY_PORT_ALS_DATA*)pucSbmAlsDataStart;

        //pucAlsDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetPhysicalPort, 
          //                                                &pstSetPhysicalPort->als_data);        
        //pstAlsData = (ATG_DCI_PHY_PORT_ALS_DATA*)pucAlsDataStart;

        if ((pstSbmAlsData != NULL) && (pstPhysicalPort->als_cfg_cb != NULL))
        {
        	OS_MEMCPY(pstSbmAlsData, pstPhysicalPort->als_cfg_cb, sizeof(ATG_SBI_PHY_PORT_ALS_DATA));
        }
    }
    
    if (SHARED.sub_card_cb[ucSubBoardNo - 1] != NULL)
    {
        spm_snd_sbi_ips(SHARED.sub_card_cb[ucSubBoardNo - 1], 
            (NBB_VOID*)pstSetSbiPhysicalPort, &(pstSetSbiPhysicalPort->ips_hdr) NBB_CCXT);
    }    
    else
    {
    	NBB_FREE_BUFFER(pstSetSbiPhysicalPort);
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return 0;
}

/*****************************************************************************
   函 数 名  : spm_send_physical_port_cfg_ips_from_local
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
NBB_VOID spm_send_physical_port_cfg_ips_from_local(NBB_BYTE subslot NBB_CCXT_T NBB_CXT)
{

    SPM_PHYSICAL_PORT_CB *pstPhysicalPort = NULL;
	
    for (pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree);
         pstPhysicalPort != NULL;
         pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_NEXT(SHARED.physical_port_tree,
            pstPhysicalPort->spm_physical_port_node))
    {   	
        if((NULL != pstPhysicalPort->basic_cfg_cb) 
            && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id) 
            && (subslot == pstPhysicalPort->basic_cfg_cb->sub_board))
        {
            spm_send_physical_port_cfg_from_local(pstPhysicalPort,subslot NBB_CCXT);
        }
    }
}

/*
NBB_VOID spm_send_physical_port_cfg_ips_from_local2(NBB_BYTE subslot )
{

    SPM_PHYSICAL_PORT_CB *pstPhysicalPort = NULL;
	
    for (pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree);
         pstPhysicalPort != NULL;
         pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_NEXT(SHARED.physical_port_tree,
            pstPhysicalPort->spm_physical_port_node))
    {   	
        if((NULL != pstPhysicalPort->basic_cfg_cb) 
            && (pstPhysicalPort->basic_cfg_cb->slot == SHARED.local_slot_id) 
            && (subslot == pstPhysicalPort->basic_cfg_cb->sub_board))
        {
            spm_send_physical_port_cfg_from_local(pstPhysicalPort,subslot);
        }
    }
}*/

/*****************************************************************************
 函 数 名  : spm_alloc_physical_port_cb
 功能描述  : 为接口物理配置结构分配内存空间
 输入参数  :   
 输出参数  : 无
 返 回 值  : SPM_PHYSICAL_PORT_CB
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月14日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_PHYSICAL_PORT_CB * spm_alloc_physical_port_cb(NBB_CXT_T NBB_CXT)
{
    SPM_PHYSICAL_PORT_CB *pstPhysicalPort = NULL;

    NBB_TRC_ENTRY("spm_alloc_physical_port_cb");

    /* 分配一个新的接口物理配置条目。*/
    pstPhysicalPort = (SPM_PHYSICAL_PORT_CB *)NBB_MM_ALLOC(sizeof(SPM_PHYSICAL_PORT_CB),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHYSICAL_PORT_CB);
    if (pstPhysicalPort == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化接口物理配置条目 */
    OS_MEMSET(pstPhysicalPort, 0, sizeof(SPM_PHYSICAL_PORT_CB));
    
    pstPhysicalPort->basic_cfg_cb       = NULL;
    pstPhysicalPort->phy_cfg_cb         = NULL;
    pstPhysicalPort->pos_link_cfg_cb    = NULL;
    pstPhysicalPort->sdh_sovhd_cfg_cb   = NULL;
    pstPhysicalPort->ces_cfg_cb         = NULL;
    pstPhysicalPort->cep_ovhd_cfg_cb    = NULL;
    pstPhysicalPort->efm_oam_cfg_cb     = NULL;
    pstPhysicalPort->time_sync_cfg_cb   = NULL;
    pstPhysicalPort->msg_suppr_cfg_cb   = NULL;
    pstPhysicalPort->flow_shape_cfg_cb  = NULL;
    pstPhysicalPort->schedule_policy_cfg_cb = NULL;
    pstPhysicalPort->congest_policy_cfg_cb  = NULL;
    pstPhysicalPort->ipv4_nf_cfg_cb     = NULL;
    pstPhysicalPort->ip_sample_cfg_cb   = NULL;
    pstPhysicalPort->ipv6_nf_cfg_cb     = NULL;
    pstPhysicalPort->common_cfg_cb      = NULL;
    pstPhysicalPort->sdh_tovhd_cfg_cb   = NULL;
    pstPhysicalPort->als_cfg_cb   = NULL;
    
    /* 建立用于该接口物理配置条目的句柄，作为异步消息交换的相关器。*/
    pstPhysicalPort->spm_physical_port_handle = NBB_CREATE_HANDLE(pstPhysicalPort,
                                                                   HDL_SPM_PHYSICAL_PORT_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_PHYSICAL_PORT_CB allocated at %p with handle %#lx",
                   pstPhysicalPort, pstPhysicalPort->spm_physical_port_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstPhysicalPort->spm_physical_port_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstPhysicalPort);
}

/*****************************************************************************
 函 数 名  : spm_free_physical_port_cb
 功能描述  : 释放接口物理配置节点的内存空间
 输入参数  : SPM_PHYSICAL_PORT_CB* pstPhysicalPort   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月8日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_physical_port_cb(SPM_PHYSICAL_PORT_CB* pstPhysicalPort NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_physical_port_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstPhysicalPort, sizeof(SPM_PHYSICAL_PORT_CB), MEM_SPM_PHYSICAL_PORT_CB); 

    if (pstPhysicalPort == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_PHYSICAL_PORT_CB at %p", pstPhysicalPort));  
    NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstPhysicalPort->port_index_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstPhysicalPort->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->basic_cfg_cb, MEM_SPM_PHY_PORT_BASIC_CB);  
        pstPhysicalPort->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网接口物理配置数据块。                                          */
    /***************************************************************************/
    if (pstPhysicalPort->phy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->phy_cfg_cb, MEM_SPM_PHY_PORT_PHY_CB);  
        pstPhysicalPort->phy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放POS接口链路层配置数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->pos_link_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->pos_link_cfg_cb, MEM_SPM_PHY_PORT_POS_CB);  
        pstPhysicalPort->pos_link_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放STM-1接口开销配置数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->sdh_sovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->sdh_sovhd_cfg_cb, MEM_SPM_PHY_PORT_SDH_SOVHD_CB);  
        pstPhysicalPort->sdh_sovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放CES接口配置数据块。                                                 */
    /***************************************************************************/
    if (pstPhysicalPort->ces_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->ces_cfg_cb, MEM_SPM_PHY_PORT_CES_CB);  
        pstPhysicalPort->ces_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放CEP接口配置数据块。                                                 */
    /***************************************************************************/
    if (pstPhysicalPort->cep_ovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->cep_ovhd_cfg_cb, MEM_SPM_PHY_PORT_CEP_CB);  
        pstPhysicalPort->cep_ovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网EFM OAM配置数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->efm_oam_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->efm_oam_cfg_cb, MEM_SPM_PHY_PORT_EFM_OAM_CB);  
        pstPhysicalPort->efm_oam_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放时间同步端口配置数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->time_sync_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->time_sync_cfg_cb, MEM_SPM_PHY_PORT_TIME_SYNC_CB);  
        pstPhysicalPort->time_sync_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网接口报文抑制数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->msg_suppr_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->msg_suppr_cfg_cb, MEM_SPM_PHY_PORT_MSG_SUPPR_CB);  
        pstPhysicalPort->msg_suppr_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网接口流量整形数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->flow_shape_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->flow_shape_cfg_cb, MEM_SPM_PHY_PORT_FLOW_SHAPE_CB);  
        pstPhysicalPort->flow_shape_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网出口队列调度策略数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->schedule_policy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->schedule_policy_cfg_cb, MEM_SPM_PHY_PORT_SCHEDUL_POLICY_CB);  
        pstPhysicalPort->schedule_policy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放以太网出口队列拥塞策略数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->congest_policy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->congest_policy_cfg_cb, MEM_SPM_PHY_PORT_CONGEST_POLICY_CB);  
        pstPhysicalPort->congest_policy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放IPV4流采样使能配置IPV4 NetFlow数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->ipv4_nf_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->ipv4_nf_cfg_cb, MEM_SPM_PHY_PORT_IPV4_NF_CB);  
        pstPhysicalPort->ipv4_nf_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放IPV4流采样镜像 IPV4 NetFlow sampler数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->ip_sample_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->ip_sample_cfg_cb, MEM_SPM_PHY_PORT_IPV4_SAMPLE_CB);  
        pstPhysicalPort->ip_sample_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放IPV6流采样使能配置IPV6 NetFlow数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->ipv6_nf_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->ipv6_nf_cfg_cb, MEM_SPM_PHY_PORT_IPV6_NF_CB);  
        pstPhysicalPort->ipv6_nf_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放接口通用配置数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->common_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->common_cfg_cb, MEM_SPM_PHY_PORT_COMMON_CB);  
        pstPhysicalPort->common_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放 SDH通道开销配置数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->sdh_tovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->sdh_tovhd_cfg_cb, MEM_SPM_PHY_PORT_SDH_TOVHD_CB);  
        pstPhysicalPort->sdh_tovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放ALS 配置数据块。                                           */
    /***************************************************************************/
    if (pstPhysicalPort->als_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstPhysicalPort->als_cfg_cb, MEM_SPM_PHY_PORT_ALS_CB);  
        pstPhysicalPort->als_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstPhysicalPort->spm_physical_port_handle, HDL_SPM_PHYSICAL_PORT_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstPhysicalPort, MEM_SPM_PHYSICAL_PORT_CB);  
    pstPhysicalPort = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_init_physical_port_tree
 功能描述  : 初始化存储接口物理配置的树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月24日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_physical_port_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_physical_port_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_PHYSICAL_PORT_CB, port_index_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.physical_port_tree, compare_ulong,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_PHYSICAL_PORT_CB, spm_physical_port_node)); 

    NBB_TRC_EXIT();
}

/*****************************************************************************
 函 数 名  : spm_init_dev_port_info
 功能描述  : 初始化整机端口表
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月13日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_dev_port_info(NBB_CXT_T NBB_CXT)
{
    NBB_INT i=0;
    NBB_INT j=0;
    
    NBB_TRC_ENTRY("spm_init_dev_port_info");

    for (i=0; i<SPM_SLOT_MAX_NUM; i++)
    {
        for (j=0; j<SPM_PORT_MAX_NUM; j++)
        {
            SHARED.dev_port_info[j][i] = 0xff;
        }
    }

    NBB_TRC_EXIT();
}

/*****************************************************************************
 函 数 名  : spm_add_dev_port_info
 功能描述  : 向整机端口表中添加一个以太网口
 输入参数  : ATG_DCI_PHY_PORT_BASIC_DATA *pstBasicData  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月13日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_add_dev_port_info(ATG_DCI_PHY_PORT_BASIC_DATA *pstBasicData NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_BYTE slot = 0;
    NBB_BYTE port = 0;
    NBB_BYTE sub_board_no = 0;
    NBB_BYTE port_pos = 0;
    SPM_PORT_STRUCT stPort;

    NBB_TRC_ENTRY("spm_add_dev_port_info");
    
    if (pstBasicData == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_add_dev_port_info(pstBasicData==NULL)"));

        OS_PRINTF("***ERROR***:spm_add_dev_port_info(pstBasicData==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_add_dev_port_info(pstBasicData==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", 0, 
        		FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage));
        		
        ret = ERROR;
        goto EXIT_LABEL;
    }

    slot = pstBasicData->slot;
    port = pstBasicData->port;
    sub_board_no = pstBasicData->sub_board;

    //如果是无子卡或子卡1，那都从子卡1存储
    if ((sub_board_no == 0) || (sub_board_no == 1))
    {
        port_pos = port;
    }
    else if (port < 21)    //如果是子卡2，那都从子卡2存储
    {
        port_pos = port + 20;
    }
    else
    {
        ret = ERROR;
        goto EXIT_LABEL;
    }

    if (pstBasicData->port_type == ATG_DCI_ETH)
    {
        ret = spm_hardw_getc3lineport(sub_board_no, port, LOGIC_ETH, &stPort);

        if (ret == SUCCESS)
        {
            SHARED.dev_port_info[port_pos-1][slot-1] = stPort.ucPort;
        }
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_hardw_getc3lineport() error"));

            OS_PRINTF("***ERROR***:spm_hardw_getc3lineport() ret=%d\n", ret);

            OS_SPRINTF(ucMessage, "***ERROR***:spm_hardw_getc3lineport() ret=%d\n", ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        	NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", 0, 
        		CALL_FUNC_ERROR, "CALL_FUNC_ERROR", ucMessage));
        		
            ret = ERROR;
            goto EXIT_LABEL;
        }
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 函 数 名  : spm_del_dev_port_info
 功能描述  : 向整机端口表中删除一个以太网口
 输入参数  : ATG_DCI_PHY_PORT_BASIC_DATA *pstBasicData  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年5月13日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_del_dev_port_info(ATG_DCI_PHY_PORT_BASIC_DATA *pstBasicData NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_BYTE slot = 0;
    NBB_BYTE port = 0;
    NBB_BYTE sub_board_no = 0;
    NBB_BYTE port_pos = 0;

    NBB_TRC_ENTRY("spm_del_dev_port_info");
    
    if (pstBasicData == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_del_dev_port_info(pstBasicData==NULL)"));

        OS_PRINTF("***ERROR***:spm_del_dev_port_info(pstBasicData==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_del_dev_port_info(pstBasicData==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", 0, 
        		FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage));
        		
        ret = ERROR;
        goto EXIT_LABEL;
    }

    slot = pstBasicData->slot;
    port = pstBasicData->port;
    sub_board_no = pstBasicData->sub_board;

    //如果是无子卡或子卡1，那都从子卡1存储
    if ((sub_board_no == 0) || (sub_board_no == 1))
    {
        port_pos = port;
    }
    else    //如果是子卡2，那都从子卡2存储
    {
        port_pos = port + 20;
    }

    if (pstBasicData->port_type == ATG_DCI_ETH)
    {
        SHARED.dev_port_info[port_pos-1][slot-1] = 0xff;
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   函 数 名  : spm_free_all_physical_port
   功能描述  : 释放端口物理配置所有的内存空间
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

NBB_VOID spm_free_all_physical_port(NBB_CXT_T NBB_CXT)
{
	SPM_PHYSICAL_PORT_CB *pstPhysicalPort = NULL;
	
    for (pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree);
         pstPhysicalPort != NULL;
         pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree))
    {
    	AVLL_DELETE(SHARED.physical_port_tree, pstPhysicalPort->spm_physical_port_node);
    	spm_free_physical_port_cb(pstPhysicalPort NBB_CCXT);
    }
}


#endif


