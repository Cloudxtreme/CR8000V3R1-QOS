/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_oam_proc.c
  版 本 号   : 初稿
  作    者   : huxi
  生成日期   : 2013年2月25日
  最近修改   :
  功能描述   : OAM配置处理
  函数列表   :
  修改历史   :
  1.日    期   : 2013年2月25日
    作    者   : huxi
    修改内容   : 创建文件

******************************************************************************/
#include <nbase.h>
#include <spmincl.h>
#ifdef PTN690_CES

//NBB_INT spm_oam_add_cfg(IN NBB_USHORT usLoop, VC_OAM_T stOAM);
#endif

/*****************************************************************************
 函 数 名  : spm_oam_init
 功能描述  : oam模块初始化
 输入参数  : NBB_VOID
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月8日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_init(NBB_CCXT_T NBB_CCXT)
{
    NBB_INT iRet = ERROR;
    NBB_USHORT usLoop;
    NBB_BYTE ucLoop;

    //SPM_BFD_SLOT_TRAN_INFO info;
    NBB_INT fd = 0;
    g_usoam_treelevel  = 11;//1K搜索树只有1023个节点，2K只有2047个节点
    g_usoam_treenodemaxnum = (1 << g_usoam_treelevel);
    g_usoam_nomax   = g_usoam_treenodemaxnum - 1;
    g_usoam_num = 0;
    g_odoam_cnt1 = 0;
    g_odoam_cnt2 = 0;
    g_sd_threshold = 0;
    g_sd_windows = 5;//测试用，sd窗口固定为5s
    g_sd_windowticks = g_sd_windows * 200;
    g_channel_type = CHANNEL_TYPE_7FFA;
    g_tp_cfmindex[2047].isIndexEnable = TRUE;/* 2047和4095为无效索引 */
    g_tp_cfmindex[4095].isIndexEnable = TRUE;
    g_puloam_treebase = (NBB_ULONG *)NBB_MM_ALLOC(sizeof(NBB_ULONG) * g_usoam_treelevel,
                       NBB_NORETRY_ACT, MEM_SPM_OAM_SEARCH_TREE);

    if (NULL == g_puloam_treebase)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nMalloc error\n"
                   "init oam search tree failed\n"
                   "oam init failed\n", ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }


    g_uloam_synidinfolth = OAM_MAX_NUM_ACTIVE_SPU * (sizeof(SPM_OAM_ID_INFO) +
                          sizeof(NBB_BYTE));
    g_pucoam_synidinfo = (NBB_BYTE *)NBB_MM_ALLOC(g_uloam_synidinfolth,
                        NBB_NORETRY_ACT, MEM_SPM_OAM_PUC_SYN_ID_INFO_CB);

    if (NULL == g_puloam_treebase)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nMalloc error\n"
                   "init oam search tree failed\n"
                   "oam init failed\n", ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

#ifdef SRC
    spm_oam_init_oam_id_conf_db();
#endif

    spm_oam_init_oam_id_conf();

    /* 创建盘间同步OAM ID的fd */
    //fd = spm_bfd_create_slot_transfer(tran_syn_oam);
    //g_oam_fd.syn_id_fd = fd;

    /* 注册OAM相关的同步信息 */
    //NBB_MEMSET(&info, 0, sizeof(SPM_BFD_SLOT_TRAN_INFO));
    //info.getDate = spm_oam_get_syn_oam_id_data;
    //info.setDate = spm_oam_set_syn_oam_id_data;
    //info.release = NULL;                            /* get 使用了静态全局变量，不需要释放资源 */

    //spm_bfd_bind_slot_transfer(fd, &info);

    for (usLoop = 0 ; usLoop < OAM_MAX_NUM_ACTIVE_SPU; usLoop++)
    {
        OAM_LOCAL_INIT(usLoop);
        g_sd_oamloop[usLoop] = USELESS;
        g_sdstate_now[usLoop] = TPOAM_SD_INITE;
        g_sdstate_last[usLoop] = TPOAM_SD_INITE;
        g_tx_cnt[usLoop] = 0;
        g_rx_cnt[usLoop] = 0;
        g_lag_state_last[usLoop] = 0x0;//up,默认
        g_lag_state_now[usLoop] = g_lag_state_last[usLoop];
    }

    for (ucLoop = 0 ; ucLoop < OAM_MAX_NUM_OD; ucLoop++)
    {
        OD_OAM_LOCAL_INIT(ucLoop);
    }

    /* 二叉树初始化 */
    iRet = spm_oamtree_init(g_usoam_treenodemaxnum, spm_oam_tree_get_fpga_data, &g_sttree_oamnodeset0);

    /* 判断执行结果 */
    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nInit oamtree failed\n"
                   "oam tree 0 init failed\n", ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 二叉树初始化 */
    iRet = spm_oamtree_init(g_usoam_treenodemaxnum, spm_oam_tree_get_fpga_data, &g_sttree_oamnodeset1);

    /* 判断执行结果 */
    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nInit oamtree failed\n"
                   "oam tree 1 init failed\n", ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /*初始化配置树(vp/vc/vs/all/wrapping)*/
    spm_init_rsvp_tx_cb_tree();
    spm_init_rsvp_rx_cb_tree();
    spm_init_pw_cb_tree();
    spm_init_vs_cb_tree();
    spm_init_oam_cb_tree();
    spm_init_wrapping_cb_tree();
    
#if 0
    iRet = init_sd_list(&g_oam_sdlist);

    if (SUCCESS != iRet)
    {
        g_sd_listlength = 0;
        OS_SPRINTF(g_uc_message, "%s %s,%d\nInit sd list failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    g_sd_listlength = 1;
#endif
    
    g_oam_sdlist =  NULL;
    g_sd_listlength = 0;

    
EXIT_LABEL:
    return;
}


/*****************************************************************************
 函 数 名  : spm_oam_init_fpga
 功能描述  : oam模块初始化
 输入参数  : NBB_VOID
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月8日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_init_fpga(NBB_CCXT_T NBB_CCXT)
{
    /* FPGA复位 */
    spm_oam_16write(OAM_REG_BASE1_ADDRESS, OAM_FPGA_RESET, 0x1000);
    spm_init_channeltype_fpga(g_channel_type);

    /* 设置超时门限,1-100,1表示50ms */
    spm_oam_16write(OAM_REG_BASE1_ADDRESS, OAM_TIME_THRESHOLD_OFFSET, 1 NBB_CCXT_T NBB_CCXT);
#if SPU
    spm_oam_16write(OAM_REG_BASE2_ADDRESS, OAM_FPGA_RESET, 0x1000);
    spm_oam_16write(OAM_REG_BASE2_ADDRESS, OAM_TIME_THRESHOLD_OFFSET, 1 NBB_CCXT_T NBB_CCXT);
#endif
    return;
}


/*****************************************************************************
 函 数 名  : spm_oam_tx_tms_mac_fpga
 功能描述  : VS层OAM帧发向MAC配置
 输入参数  : NBB_USHORT usOamLoop
             BASIC_CFG_VALUE_TYPE *pstBasiCfgValueType,
             VS_CFG_TYPE *pstVsCfgType;

 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月26日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_tx_tms_mac_fpga(IN NBB_USHORT usLoop,
                                 IN NBB_BYTE ActOrOdType)
{
    CV_CFH_SLOTID_STRU CvSlotId;
    CV_CFH_PORTID_STRU CvPortId;
    NBB_USHORT usPortIdData;
    NBB_USHORT usSlotIdData;
    NBB_USHORT usVlanId;
    NBB_INT iRet = ERROR;
    NBB_BYTE ucSlot;
    NBB_ULONG baseAddr;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_ULONG ip = 0;
    NBB_USHORT usLocalIndex = 0; //模块本地索引
    NBB_USHORT frameHead = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\nframe type=%d\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, ActOrOdType);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (ActOrOdType !=  ACTIVE_OAM)
    {
        iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

        if (SUCCESS != iRet)
        {
            goto EXIT_LABEL;
        }

        ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    }

    usLocalIndex = g_stoam_local[usLoop].module_index;
    usVlanId = 0;

    OS_MEMSET((&CvSlotId), 0, sizeof(CV_CFH_SLOTID_STRU));
    OS_MEMSET((&CvPortId), 0, sizeof(CV_CFH_PORTID_STRU));

    /* 帧头各部分赋值 */
    CvSlotId.Unit = 0;
    CvSlotId.SlotId = g_stoam_local[usLoop].vs_Slotid;
    CvPortId.DType = 3; /* 代表是vs层oam */
    CvPortId.IsLagId = 0;
    CvPortId.chip_id = g_stoam_local[usLoop].chip_id;
    CvPortId.PortId = g_stoam_local[usLoop].vs_Portid;
    usSlotIdData = *(NBB_USHORT *)(&CvSlotId);
    usPortIdData = *(NBB_USHORT *)(&CvPortId);

    ip = g_stoam_local[usLoop].ip;

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usLoop, &baseAddr);

    switch (ActOrOdType)
    {
        case ACTIVE_OAM :

            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, CV_APS_CSF_TX_MAC_CFG_OFFSET + usLocalIndex * 8 + 0x0,
                            0x0000);
            spm_oam_16write(baseAddr, CV_APS_CSF_TX_MAC_CFG_OFFSET + usLocalIndex * 8 + 0x1,
                            usVlanId);
            spm_oam_16write(baseAddr, CV_APS_CSF_TX_MAC_CFG_OFFSET + usLocalIndex * 8 + 0x2,
                            usSlotIdData);
            spm_oam_16write(baseAddr, CV_APS_CSF_TX_MAC_CFG_OFFSET + usLocalIndex * 8 + 0x3,
                            usPortIdData);  
            spm_oam_16write(baseAddr, CV_APS_CSF_TX_MAC_CFG_OFFSET + usLocalIndex * 8 + 0x4,
                            (ip >> 16) & 0xFFFF);
            spm_oam_16write(baseAddr, CV_APS_CSF_TX_MAC_CFG_OFFSET + usLocalIndex * 8 + 0x5,
                            ip & 0xFFFF); 
            spm_oam_tx_megfpga(usLoop);

            spm_oam_tx_enablefpga(usLoop, ACTIVE_OAM);
            break;
#if 0
        case LB_OAM :
            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                            usSlotIdData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                            usPortIdData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                            (ip >> 16) & 0xFFFF);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                            ip & 0xFFFF);  /* IPADDRESS 需修改 */
            break;

        case LCK_OAM :
            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                            usSlotIdData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                            usPortIdData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                            (ip >> 16) & 0xFFFF);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                            ip & 0xFFFF);  /* IPADDRESS 需修改 */
            break;

        case DMM_OAM:
            /* 填写帧头MAC内容 */
            /* DMM发向 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            usSlotIdData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x6,
                            usPortIdData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x7,
                            (ip >> 16) & 0xFFFF);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x8,
                            ip & 0xFFFF);  /* IPADDRESS 需修改 */
            break;

        case DMR_OAM:
            /* DMR发向 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x0,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x1,
                            usVlanId);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x2,
                            usSlotIdData);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            usPortIdData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            (ip >> 16) & 0xFFFF);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            ip & 0xFFFF);  /* IPADDRESS 需修改 */
            break;

        case LMM_OAM:
            /* 填写帧头MAC内容 */
            /* LMM发向 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            usSlotIdData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x6,
                            usPortIdData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x7,
                            (ip >> 16) & 0xFFFF);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x8,
                            ip & 0xFFFF);  /* IPADDRESS 需修改 */
            break;

        case LMR_OAM:
            /* LMR发向 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x0,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x1,
                            usVlanId);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x2,
                            usSlotIdData);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            usPortIdData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            (ip >> 16) & 0xFFFF);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            ip & 0xFFFF);  /* IPADDRESS 需修改 */
            break;

        case TST_OAM:
            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                            usSlotIdData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                            usPortIdData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                            (ip >> 16) & 0xFFFF);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                            ip & 0xFFFF);  /* IPADDRESS 需修改 */
            break;
#endif
        default:
            OS_SPRINTF(g_uc_message, "%s %s,%d\n vp ActOrOdType = %d\n is wrong\n"
                       "frame type for writing mac field error\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ActOrOdType);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
    }

    /* 对等MEP ID写入寄存器 */
    spm_oam_16write(baseAddr,  CV_EXPECT_MEP_OFFSET + usLocalIndex, g_stoam_local[usLoop].dst_mep_id);

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_tx_tmp_mac_fpga
 功能描述  : 发送vp层OAM帧MAC配置
 输入参数  : NBB_USHORT OamIndex,
             OAM_ACTIVE_CFG_TYPE *pstOamActiveCfgType
 输出参数  : 无
 返 回 值  : NBB_BYTE
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月5日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_tx_tmp_mac_fpga(IN NBB_USHORT usOamLoop, IN NBB_BYTE ActOrOdType)
{
    CV_CFH_PORTID_STRU CvDtype;
    NBB_USHORT usDTypeData;
    NBB_USHORT usLspId;
    NBB_USHORT usVlanId = 0;
    NBB_INT iRet = ERROR;
    NBB_ULONG baseAddr;
    NBB_ULONG ulLocalTunneLable;
    NBB_ULONG ulLocalPwLable;
    NBB_USHORT usLocalIndex;
    NBB_USHORT frameHead = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    OS_MEMSET((&CvDtype), 0, sizeof(CV_CFH_PORTID_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\nframe type=%d\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, ActOrOdType);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    usLspId = g_stoam_local[usOamLoop].vp_Lspid & 0xFFFF;

    /* Dtype赋值 */
    CvDtype.chip_id = g_stoam_local[usOamLoop].chip_id;
    CvDtype.DType = 1; /* 代表是vp层oam */
    usDTypeData = *(NBB_USHORT *)(&CvDtype);

    /*判断该条目存在哪个模块*/
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    usLocalIndex = g_stoam_local[usOamLoop].module_index;

    /* 发帧头类型配置 */
    frameHead = spm_oam_16read(baseAddr, CV_FRAME_HEAD + (usLocalIndex >> 2));
    frameHead &= ~(0xf << (3 - usLocalIndex % 4) * 4);
    spm_oam_16write(baseAddr, CV_FRAME_HEAD + (usLocalIndex >> 2), frameHead);
    spm_oam_16write(baseAddr, FDI_FRAME_HEAD + (usLocalIndex >> 2), frameHead);

    if ((ActOrOdType !=  ACTIVE_OAM) && (ActOrOdType != FDI_OAM))
    {
        iRet = spm_odindex_by_activeindex(usOamLoop, &ucodLoop);

        if (SUCCESS != iRet)
        {
            goto EXIT_LABEL;
        }

        ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    }

    /* 条目N的发送标签使能,填入tunnel和pw标签 */
    ulLocalTunneLable = g_stoam_local[usOamLoop].ulTunnelLabel_tx;
    ulLocalPwLable = g_stoam_local[usOamLoop].ulPwLable_tx;

    switch (ActOrOdType)
    {
        case ACTIVE_OAM :

            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET1 + usLocalIndex,
                            0x0000);
            spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET2 + usLocalIndex,
                            usVlanId);
            spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET3 + usLocalIndex,
                            usLspId);
            spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET1 + usLocalIndex,
                            usDTypeData);
            spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET2 + usLocalIndex,
                            0x0000);
            spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET3 + usLocalIndex,
                            0x0000);

            /* tms/tmp/tmc层cv&asps/csf成帧MEG配置 */
            spm_oam_tx_megfpga(usOamLoop);

            spm_oam_tx_enablefpga(usOamLoop, ACTIVE_OAM);
            break;

        case LB_OAM:

            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                            usLspId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                            usDTypeData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                            0x0000);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                            0x0000);  /* IPADDRESS 需修改 */
            break;

        case LCK_OAM:

            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                            usLspId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                            usDTypeData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                            0x0c01);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                            0x0102);  /* IPADDRESS 需修改 */
            break;

        case DMM_OAM:

            /* 填写帧头MAC内容 */
            /* DMM发向 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            usLspId);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x6,
                            usDTypeData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x7,
                            0x0c01);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x8,
                            0x0102);  /* IPADDRESS 需修改 */
            break;

        case DMR_OAM:

            /* DMR发向 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x0,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x1,
                            usVlanId);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x2,
                            usLspId);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            usDTypeData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            0x0c01);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            0x0102);  /* IPADDRESS 需修改 */
            break;

        case LMM_OAM:

            /* 填写帧头MAC内容 */
            /* LMM发向 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            usLspId);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x6,
                            usDTypeData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x7,
                            0x0c01);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x8,
                            0x0102);  /* IPADDRESS 需修改 */
            break;

        case LMR_OAM:

            /* LMR发向 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x0,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x1,
                            usVlanId);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x2,
                            usLspId);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            usDTypeData);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            0x0c01);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            0x0102);  /* IPADDRESS 需修改 */
            break;

        case TST_OAM:

            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                            0x0000);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            usVlanId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                            usLspId);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                            usDTypeData);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                            0x0c01);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                            0x0102);  /* IPADDRESS 需修改 */
            break;

        case FDI_OAM:

            /* 监视rxlsp入端口的los告警，有告警才发FDI报文 */
            /* 填写帧头MAC内容 */
            spm_oam_16write(baseAddr, FDI_TX_DMAC_OFFSET1 + usLocalIndex,
                            0x0000);
            spm_oam_16write(baseAddr, FDI_TX_DMAC_OFFSET2 + usLocalIndex,
                            usVlanId);
            spm_oam_16write(baseAddr, FDI_TX_DMAC_OFFSET3 + usLocalIndex,
                            g_stoam_local[usOamLoop].lspid1);
            spm_oam_16write(baseAddr, FDI_TX_SMAC_OFFSET1 + usLocalIndex,
                            usDTypeData);
            spm_oam_16write(baseAddr, FDI_TX_SMAC_OFFSET2 + usLocalIndex,
                            0x0c01);  /* IPADDRESS 需修改 */
            spm_oam_16write(baseAddr, FDI_TX_SMAC_OFFSET3 + usLocalIndex,
                            0x0102);  /* IPADDRESS 需修改 */
            spm_oam_tx_enablefpga(usOamLoop, FDI_OAM);
            break;

        default:
            OS_SPRINTF(g_uc_message, "%s %s,%d\n vp ActOrOdType = %d is wrong\n"
                       "frame type for writing mac field error\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ActOrOdType);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            goto EXIT_LABEL;
    }

    /* 对等MEP ID写入寄存器 */
    spm_oam_16write(baseAddr, CV_EXPECT_MEP_OFFSET + usLocalIndex,
                    g_stoam_local[usOamLoop].dst_mep_id);

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_tree_get_fpga_data
 功能描述  : a
 输入参数  : IN SPM_OAMTREE_NODE *oamTreeNode     
             OUT SPM_FPGATREE_DATA *fpgaTreeDate  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_tree_get_fpga_data(IN SPM_OAMTREE_NODE *oamTreeNode,
                                   OUT SPM_FPGATREE_DATA *fpgaTreeDate)
{
    NBB_INT iRet = ERROR;

    if ((NULL == oamTreeNode) || (NULL == fpgaTreeDate))
    {
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(fpgaTreeDate, 0, sizeof(SPM_FPGATREE_DATA));

    fpgaTreeDate->num = 4;  /* 有效字节为4字节 */
    fpgaTreeDate->usData.oamTable.lspNo = oamTreeNode->usLspNo;
    fpgaTreeDate->usData.oamTable.tunnel_19to8b = (oamTreeNode->ulTunnelId >> 8) & 0xfff;
    fpgaTreeDate->usData.oamTable.tunnel_7to0b = oamTreeNode->ulTunnelId & 0xff;
    fpgaTreeDate->usData.oamTable.pwLabel = oamTreeNode->ulPwId;

    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}


#ifdef PTN690_CES

/*****************************************************************************
 函 数 名  : spm_oam_add_to_array
 功能描述  : 将配置存入本地数组
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月12日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_to_array(SPM_OAM_CB *poamcfg)
{
    NBB_INT iRet = ERROR; /* 表示添加成功或失败的变量 */
    SPM_PW_KEY stpw_key;
    NBB_USHORT iloop = 0;
    NBB_ULONG ulOamId = 0;
    OAM_ALMPM_LINE_STRUCT st_oam_alarm_line;
    NBB_ULONG ulTempVcid = 0;
    OS_MEMSET(&stpw_key, 0, sizeof(SPM_PW_KEY));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    ulOamId = poamcfg->oam_id;

    /* 将树的配置放入本地数组，方便发送报文 */
    spm_put_cfg_into_array(poamcfg);
    iloop = poamcfg->oamindex;

    if (NULL != poamcfg->pAlarm)
    {
        g_stoam_local[iloop].ais_alarm_flag = poamcfg->pAlarm->ais_alarm_flag;
        g_stoam_local[iloop].rdi_alarm_flag = poamcfg->pAlarm->rdi_alarm_flag;
    }

    if (VC_OAM == poamcfg->oamtype)
    {
        stpw_key.peer_ip = poamcfg->pPwCfg->peer_ip;
        stpw_key.vc_type = poamcfg->pPwCfg->vc_type;
        stpw_key.vc_id = poamcfg->pPwCfg->vc_id;
        g_stoam_local[iloop].vp_id = poamcfg->vp_id;
        g_stoam_local[iloop].vc_no_section1 = poamcfg->vc_no_section1;
#if 0
        iRet = spm_set_tp_oam_cfg_change_if(PW_TPOAM_CHANGE);

        if (SUCCESS != iRet)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                       "call spm_set_tp_oam_cfg_change_if failed ret=%d\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId, iRet);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        }
#endif
        st_oam_alarm_line.usLineType = VC_LINE_TYPE;
        st_oam_alarm_line.usLineLayer = VC_LINE_LAYER;
        st_oam_alarm_line.ucKeyLen = VC_KEY_LEN;
        ulTempVcid = stpw_key.vc_id;
        OS_MEMCPY(st_oam_alarm_line.ucKeyValue, (char *)&ulTempVcid, 4);
        OS_MEMCPY(st_oam_alarm_line.ucKeyValue + 4, (char *)&stpw_key.vc_type, 2);
        OS_MEMCPY(st_oam_alarm_line.ucKeyValue + 6, (char *)&stpw_key.peer_ip, 4);
        iRet = almpm_cfg(st_oam_alarm_line, 0, 0, 1);

    }

    spm_oam_send_packedge(poamcfg);
    OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
               "add oam success!\n",
               ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_add
 功能描述  : oam添加配置
 输入参数  : NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : 0 添加成功
             1 添加失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月1日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_tree(SPM_OAM_CB *poamcfg)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_oam_get_data
 功能描述  : 给CCM_T结构体赋值
 输入参数  : NBB_USHORT usLoop
            CCM_T *stCCM
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年2月11日
    作    者   : huxiaolu
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_send_packedge(IN SPM_OAM_CB *poamcfg)
{
#if 0
    NBB_INT ret = SUCCESS;
    NBB_USHORT usLoop;
    NBB_USHORT usmegData1; 
    NBB_USHORT usmegData2; 
    NBB_USHORT usmegData3;
    NBB_USHORT usmegData4;
    NBB_USHORT usmegData5;
    NBB_USHORT usmegData6;
    NBB_BYTE ucmegData7;
    VC_OAM_T stOAM;
    NBB_BYTE ucPort;
    NBB_ULONG ulCeslabel;
    NBB_ULONG ulVcLabel;

    OS_MEMSET(&stOAM, 0, sizeof(VC_OAM_T));

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    usLoop = poamcfg->oamindex;
    ulVcLabel = g_stoam_local[usLoop].ulPwCesLable_tx;

    ret = spm_get_ces_label_from_vc_label(ulVcLabel, &ulCeslabel);

    if (ret != SUCCESS)
    {
        printf("***ERROR***: spm_get_ces_label_from_vc_label() ret=%d\n", ret);
        goto EXIT_LABEL;
    }

    ret = spm_hardw_getIndexPort(ulCeslabel, &ucPort);

    if (ret != SUCCESS)
    {
        printf("***ERROR***: spm_hardw_getIndexPort() ret=%d\n", ret);
        goto EXIT_LABEL;
    }


    if ((ucPort < 1) || (ucPort > 8))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n ucPort=%d\n"
                   "get ucport faild",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message, ucPort);
        goto EXIT_LABEL;
    }

    g_group_id = ucPort;

    usmegData1 = g_stoam_local[usLoop].meg_icc[0];
    usmegData1 = ((usmegData1 << 8) & 0xff00) + g_stoam_local[usLoop].meg_icc[1];

    usmegData2 = g_stoam_local[usLoop].meg_icc[2];
    usmegData2 = ((usmegData2 << 8) & 0xff00) + g_stoam_local[usLoop].meg_icc[3];

    usmegData3 = g_stoam_local[usLoop].meg_icc[4];
    usmegData3 = ((usmegData3 << 8) & 0xff00) + g_stoam_local[usLoop].meg_icc[5];

    usmegData4 = g_stoam_local[usLoop].meg_umc[0];
    usmegData4 = ((usmegData4 << 8) & 0xff00) + g_stoam_local[usLoop].meg_umc[1];

    usmegData5 = g_stoam_local[usLoop].meg_umc[2];
    usmegData5 = ((usmegData5 << 8) & 0xff00) + g_stoam_local[usLoop].meg_umc[3];

    usmegData6 = g_stoam_local[usLoop].meg_umc[4];
    usmegData6 = ((usmegData6 << 8) & 0xff00) + g_stoam_local[usLoop].meg_umc[5];

    ucmegData7 = 0;

    stOAM.rxMepId = g_stoam_local[usLoop].dst_mep_id;
    stOAM.txMepId = g_stoam_local[usLoop].src_mep_id;
    stOAM.mel = g_stoam_local[usLoop].mel;

    if (1 == g_stoam_local[usLoop].cv_enable)
    {
        stOAM.ccmEna = g_stoam_local[usLoop].cv_enable;
        stOAM.ccmPeriod = g_stoam_local[usLoop].cv_cycle;
    }

    g_csf_ena = g_stoam_local[usLoop].csf_enable;
    g_cv_ena = g_stoam_local[usLoop].cv_enable;
    stOAM.megData1 = usmegData1;
    stOAM.megData2 = usmegData2;
    stOAM.megData3 = usmegData3;
    stOAM.megData4 = usmegData4;
    stOAM.megData5 = usmegData5;
    stOAM.megData6 = usmegData6;
    stOAM.megData7 = ucmegData7;

    if (1 == g_stoam_local[usLoop].csf_enable)
    {
        stOAM.csfGroupId = g_group_id;
        stOAM.csfPeriod = 1;//表示3.3ms一帧
    }

    if (1 == g_stoam_local[usLoop].aps_enable)
    {
        stOAM.apsEna = g_stoam_local[usLoop].aps_enable;
        stOAM.apsPeriod = 1;
    }

    if (1 == g_stoam_local[usLoop].csf_enable)
    {
        //spm_get_oam_alarm();
        stOAM.csfEna = g_stoam_local[usLoop].csf_enable;
    }

    //printf("ulmegData1 %d ulmegData2 %d ulmegData3 %d ulmegData4 %d\n",ulmegData1,ulmegData2,ulmegData3,ucmegData4);
    spm_oam_add_cfg(usLoop, stOAM);

EXIT_LABEL:
    return;
#endif
}


#if 0
/*****************************************************************************
 函 数 名  : spm_oam_add_cfg
 功能描述  : 添加配置
 输入参数  : CCM_T stCCM
             NBB_USHORT usLoop
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月4日
    作    者   : huxiaolu
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_cfg(IN NBB_USHORT usLoop, VC_OAM_T stOAM NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT oamIdx = 0;
    NBB_USHORT ulVc_no;
    NBB_ULONG vclabel;
    NBB_ULONG ulPwtxlabel = 0;
    NBB_ULONG ulCeslabel;
    NBB_INT iRet = ERROR;

//    NBB_TRC_ENTRY((NBB_FORMAT "spm_oam_add_cfg"));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    vclabel = g_stoam_local[usLoop].ulPwLable_rx;
    ulPwtxlabel = g_stoam_local[usLoop].ulPwCesLable_tx;

    iRet = spm_get_ces_label_from_vc_label(ulPwtxlabel, &ulCeslabel);

    if (iRet != SUCCESS)
    {

        printf("***ERROR***: spm_get_ces_label_from_vc_label() ret=%d\n", iRet);
        goto EXIT_LABEL;
    }

    ulVc_no = g_stoam_local[usLoop].vc_no_section1;

    if (VC_OAM == g_stoam_local[usLoop].oam_type)
    {
        if ((TRUE == g_stoam_local[usLoop].cv_enable) 
            || (TRUE == g_stoam_local[usLoop].csf_enable) || (TRUE == g_stoam_local[usLoop].aps_enable))
        {
            iRet = ApiAddOam(0, &stOAM);
            printf("ApiAddOam ret %d\n", iRet);
            g_stoam_local[usLoop].vc_ces_oamIdx = stOAM.oamIdx;
            oamIdx = stOAM.oamIdx;
            iRet = ApiSetCcmCounter(0, oamIdx, 1, 0); //使能CV计数

            if (iRet != SUCCESS)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamIdx=%d\n"
                           "ApiSetCcmCounter is faild",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message, oamIdx);
                goto EXIT_LABEL;
            }

            printf("ulCeslabel = %d ulPwtxlabel = %d\n", ulCeslabel, ulPwtxlabel);
            ApiSetBindOamToVc(0, oamIdx, ulCeslabel, ulPwtxlabel, ulVc_no);
        }
    }

    iRet = SUCCESS;
EXIT_LABEL:
    return  iRet;
}
#endif

/*****************************************************************************
 函 数 名  : spm_oam_delete
 功能描述  : 删除配置
 输入参数  : NBB_USHORT usLoop
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月4日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_delete(IN NBB_USHORT usLoop)
{
    NBB_INT iRet = ERROR;/* 表示添加成功或失败的变量 */
    SPM_OAMTREE_NODE stOamTreeNode;
    SPM_OAM_CB *p_oamcfg = NULL;
    NBB_USHORT oamIdx;
    OS_MEMSET(&stOamTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (usLoop >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        goto EXIT_LABEL;
    }

//    spm_get_oamindex_by_oamid(ulOamId, &usLoop);
    stOamTreeNode.ulTunnelId = g_stoam_local[usLoop].ulTunnelLabel_rx; /* 待添加 */
    stOamTreeNode.ulPwId = g_stoam_local[usLoop].ulPwLable_rx; /* 待添加 */
    stOamTreeNode.usLspNo = g_stoam_local[usLoop].module_index;

    oamIdx = g_stoam_local[usLoop].vc_ces_oamIdx;
    
    /* 清除相关芯片内容 */
    ApiDelOam(0, oamIdx);

    /* 将本地存储配置的oamIndex置为0 */
    OS_MEMSET(&g_stoam_local[usLoop], 0, sizeof(OAM_ACTIVE_LOCAL_STRU));

    /* 将本地存储配置的ActiveOamkey置为0x0 */
    g_stoam_local[usLoop].ulActiveOamkey = USELESS;

    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

#endif


#ifdef SPU
NBB_INT spm_oam_set_syn_oam_id_data(IN NBB_VOID *pBuf,
                                    IN NBB_ULONG ulMsgLenth,
                                    IN NBB_USHORT usSlotid)
{
    return SUCCESS;
}


/*****************************************************************************
 函 数 名  : spm_oam_add_to_array
 功能描述  : 将配置存入本地数组
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月12日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_to_array(SPM_OAM_CB *poamcfg)
{
    NBB_INT iRet = ERROR; /* 表示添加成功或失败的变量 */
    OAM_RSVP_LSP_KEY strsvp_key_rx;
    NBB_USHORT vs_index;
    SPM_PW_KEY stpw_key;
    NBB_ULONG ulTempVcid = 0;
    NBB_USHORT iloop = 0;
    NBB_ULONG ulOamId = 0;
    NBB_ULONG labelMplsFlag = 0;
    OAM_ALMPM_LINE_STRUCT st_oam_alarm_line;

    OS_MEMSET(&strsvp_key_rx, 0, sizeof(OAM_RSVP_LSP_KEY));
    OS_MEMSET(&stpw_key, 0, sizeof(SPM_PW_KEY));

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    ulOamId = poamcfg->oam_id;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    iRet = spm_put_cfg_into_array(poamcfg);
    iloop = poamcfg->oamindex;

    switch (poamcfg->oamtype)
    {
        case WRAPPING_OAM:
            if (NULL == poamcfg->pWrapping)
            {
                goto EXIT_LABEL;
            }

            g_stoam_local[iloop].vs_Portid = poamcfg->out_portid;
            g_stoam_local[iloop].wrapping_port_logical_index =
                poamcfg->logical_port_index;//mantis 58637
            g_stoam_local[iloop].vs_Slotid = poamcfg->slot_id;
            g_stoam_local[iloop].ip = poamcfg->nhop_ip;

//            /* 将oam与环网保护绑定 */
            //spm_wrap_add_tp_id(poamcfg->pWrapping->ring_id, ulOamId, poamcfg->pWrapping->direction_select);
            spm_record_wrapping_oamloop(iloop);
            break;

        case VS_OAM:
            if (NULL == poamcfg->pVsCfg)
            {
                goto EXIT_LABEL;
            }

            g_stoam_local[iloop].vs_Portid = poamcfg->vs_portid;
            g_stoam_local[iloop].vs_Slotid = poamcfg->vs_slotid;
            g_stoam_local[iloop].ip = poamcfg->pVsCfg->nxhop_ip;

            labelMplsFlag = g_stoam_local[iloop].vs_Portid;

            //通过vs层lm下插sd
            spm_record_sd_oamloop(iloop);
            break;

        case VP_OAM:
            if (NULL == poamcfg->pRsvplspCfg)
            {
                goto EXIT_LABEL;
            }

            strsvp_key_rx.Egress = poamcfg->pRsvplspCfg->rx_egress;
            strsvp_key_rx.Ingress = poamcfg->pRsvplspCfg->rx_ingress;
            strsvp_key_rx.TunnelId = poamcfg->pRsvplspCfg->rx_tunnel_id;
            strsvp_key_rx.LspId = poamcfg->pRsvplspCfg->rx_lsp_id;

            /* MEP */
            if (0x1 == g_stoam_local[iloop].vp_attrib)
            {
                g_stoam_local[iloop].vp_Lspid = poamcfg->lsp_id_tx;
                g_stoam_local[iloop].vp_Slot = poamcfg->slotMcid;
                g_stoam_local[iloop].vp_PortId = poamcfg->vp_PortId_tx;
            }

            /* MIP */
            else if (0x0 == g_stoam_local[iloop].vp_attrib)
            {
                g_stoam_local[iloop].inport1 = poamcfg->vp_PortId_tx;
                g_stoam_local[iloop].lspid1 = poamcfg->lsp_id_tx;
                g_stoam_local[iloop].inport_index1 = poamcfg->inport_index_tx;
                g_stoam_local[iloop].inport2 = poamcfg->vp_PortId_rx;
                g_stoam_local[iloop].lspid2 = poamcfg->lsp_id_rx;
                g_stoam_local[iloop].inport_index2 = poamcfg->inport_index_rx;
                g_stoam_local[iloop].mip_second_tunnel_rx =
                    poamcfg->mip_second_ulTunnelLabel_rx;
            }

            st_oam_alarm_line.usLineType = VP_LINE_TYPE;
            st_oam_alarm_line.usLineLayer = VP_LINE_LAYER;
            st_oam_alarm_line.ucKeyLen = VP_KEY_LEN;
            OS_MEMCPY(st_oam_alarm_line.ucKeyValue, &strsvp_key_rx, VP_KEY_LEN);
            iRet = almpm_cfg(st_oam_alarm_line, 0, 0, 1);

            if (SUCCESS != iRet)
            {
                goto EXIT_LABEL;
            }

            labelMplsFlag = g_stoam_local[iloop].ulTunnelLabel_rx;
            break;

        case VC_OAM:
            if (NULL == poamcfg->pPwCfg)
            {
                goto EXIT_LABEL;
            }

            stpw_key.peer_ip = poamcfg->pPwCfg->peer_ip;
            stpw_key.vc_type = poamcfg->pPwCfg->vc_type;
            stpw_key.vc_id = poamcfg->pPwCfg->vc_id;
            g_stoam_local[iloop].vc_no_section1 = poamcfg->vc_no_section1;
            g_stoam_local[iloop].vc_no_section2 = poamcfg->vc_no_section2;
            g_stoam_local[iloop].vc_PortId_section1 = poamcfg->vc_PortId_section1;
            g_stoam_local[iloop].vc_PortId_section2 = poamcfg->vc_PortId_section2;
            g_stoam_local[iloop].uni_slot = poamcfg->uni_slot;
            g_stoam_local[iloop].nni_slot = poamcfg->nni_slot;
            g_stoam_local[iloop].uni_port_type = poamcfg->uni_port_type;
            g_stoam_local[iloop].vpn_type = poamcfg->vpn_type;
            g_stoam_local[iloop].vp_id = poamcfg->vp_id;

            if (0x0 == g_stoam_local[iloop].vp_attrib)//mip
            {
                g_stoam_local[iloop].nni_mip_slot1 = poamcfg->mip_nni_slot1;
                g_stoam_local[iloop].nni_mip_slot2 = poamcfg->mip_nni_slot2;
                g_stoam_local[iloop].vc_logical_port_index1
                    = poamcfg->vc_logical_port_index1;
                g_stoam_local[iloop].vc_logical_port_index2
                    = poamcfg->vc_logical_port_index2;
                g_stoam_local[iloop].swtich_pw_label_rx1
                    = poamcfg->swtich_pw_label_rx1;
                g_stoam_local[iloop].swtich_pw_label_rx2
                    = poamcfg->swtich_pw_label_rx2;
            }

            st_oam_alarm_line.usLineType = VC_LINE_TYPE;
            st_oam_alarm_line.usLineLayer = VC_LINE_LAYER;
            st_oam_alarm_line.ucKeyLen = VC_KEY_LEN;
            ulTempVcid = stpw_key.vc_id;
            OS_MEMCPY(st_oam_alarm_line.ucKeyValue, (char *)&ulTempVcid, 4);
            OS_MEMCPY(st_oam_alarm_line.ucKeyValue + 4, (char *)&stpw_key.vc_type, 2);
            OS_MEMCPY(st_oam_alarm_line.ucKeyValue + 6, (char *)&stpw_key.peer_ip, 4);

            //OS_MEMCPY(st_oam_alarm_line.ucKeyValue, &stpw_key, VC_KEY_LEN);
            iRet = almpm_cfg(st_oam_alarm_line, 0, 0, 1);

            if (SUCCESS != iRet)
            {
                goto EXIT_LABEL;
            }

            labelMplsFlag = g_stoam_local[iloop].ulPwLable_rx;
            break;

        default:
            goto EXIT_LABEL;
    }

#if 0
    iRet = spm_set_mpls_flag(poamcfg);

    if (SUCCESS != iRet)
    {
        goto EXIT_LABEL;
    }
#endif

    spm_write_table_for_active_lm(g_stoam_local[iloop].chip_id, labelMplsFlag,
                                  g_stoam_local[iloop].global_index, iloop);
    spm_oam_send_packedge(poamcfg);

    //spm_bfd_slot_tran_syn_need_send(g_oam_fd.syn_id_fd);

    OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
               "add oam success!\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__, ulOamId);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_add
 功能描述  : oam添加配置
 输入参数  : NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : 0 添加成功
             1 添加失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月1日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_tree(SPM_OAM_CB *poamcfg)
{
    NBB_INT iRet = ERROR; /* 表示添加成功或失败的变量 */
    SPM_OAMTREE_NODE stOamTreeNode;
    NBB_BYTE module_switch = 0;
    NBB_ULONG ulOamId = 0;
    OS_MEMSET(&stOamTreeNode, 0, sizeof(SPM_OAMTREE_NODE));

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    ulOamId = poamcfg->oam_id;
    stOamTreeNode.usLspNo = poamcfg->module_index;
    module_switch = poamcfg->module;
    stOamTreeNode.ulTunnelId = poamcfg->ulTunnelLabel_rx;
    stOamTreeNode.ulPwId = poamcfg->ulPwLable_rx;

    switch (module_switch)
    {
        case 0 :
            iRet = spm_oam_tree_build(&stOamTreeNode, OAM_REG_BASE1_ADDRESS, module_switch, 0);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "build tree in fpga failed ret=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId, iRet);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            break;

        case 1 :
            iRet = spm_oam_tree_build(&stOamTreeNode, OAM_REG_BASE2_ADDRESS, module_switch, 0);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "build tree in fpga failed ret=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId, iRet);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            break;

        default:


            break;
    }

    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_send_packedge
 功能描述  : cv帧发送配置
 输入参数  : NBB_USHORT usOamLoop
            OAM_ACTIVE_CFG_TYPE *pstOamActiveCfgType
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月6日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_send_packedge(IN SPM_OAM_CB *poamcfg)
{
    NBB_ULONG baseAddr;
    NBB_USHORT usLoop = 0;
    NBB_USHORT module_index = 0;

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);


    usLoop = poamcfg->oamindex;
    module_index = g_stoam_local[usLoop].module_index;

    switch (g_stoam_local[usLoop].oam_type)
    {
        case WRAPPING_OAM :
            spm_oam_wrapping(poamcfg);
            break;

        case VS_OAM :
            if (NULL != poamcfg->pCvcfg)
            {
                spm_oam_tx_tms_mac_fpga(usLoop, ACTIVE_OAM);
            }

            /* 判断cv/APS/CSF帧是否使能 */
            if (NULL != poamcfg->pApscfg)
            {
                spm_oam_tx_tms_mac_fpga(usLoop, ACTIVE_OAM);
                
                /* APS成帧高16位和低16位 */
                /* 判断所在模块 */
                spm_get_moudle_base_addr(usLoop, &baseAddr);
            }

            break;

        case VP_OAM :
            spm_lsp_oam_send_packedge(poamcfg);
            break;

        case VC_OAM:
            spm_pw_oam_send_packedge(poamcfg);
            break;

        default:
            OS_SPRINTF(g_uc_message, "%s %s,%d\n oam type is not supported oam type=%d"
                       "oam_index=%d,send packedge failed\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__,
                       g_stoam_local[usLoop].oam_type, usLoop);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            break;
    }

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_tx_vpws_mac_fpga
 功能描述  : VPWS OAM帧发向MAC配置
 输入参数  : NBB_USHORT usOamLoop,
            OAM_ACTIVE_CFG_TYPE *pstOamActiveCfgType,
            PW_CFG_VALUE_TYPE *pstPwCfgType
            NBB_BYTE ActOrOdType
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月5日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_tx_vpws_mac_fpga(IN NBB_USHORT usOamLoop,
                                  IN NBB_BYTE ActOrOdType NBB_CCXT_T NBB_CXT)
{
    CV_CFG_VPWS_DTYPE_STRU stVpwsDtype;
    NBB_USHORT usVpwsDtype;
    NBB_INT iRet = ERROR;
    NBB_USHORT usVlanId = 0;
    NBB_ULONG baseAddr;
    NBB_USHORT usLocalIndex = 0; //模块本地索引
    NBB_BYTE ucLocalSlot;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_USHORT frameHead = 0;
    OS_MEMSET((&stVpwsDtype), 0, sizeof(CV_CFG_VPWS_DTYPE_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\nframe type=%d\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, ActOrOdType);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    //判断该条目存在哪个模块
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    usLocalIndex = g_stoam_local[usOamLoop].module_index;

    /* 发帧头类型配置 */
    frameHead = spm_oam_16read(baseAddr, CV_FRAME_HEAD + (usLocalIndex >> 2));
    frameHead &= ~(0xf << (3 - usLocalIndex % 4) * 4);
    spm_oam_16write(baseAddr, CV_FRAME_HEAD + (usLocalIndex >> 2), frameHead);
    spm_oam_16write(baseAddr, FDI_FRAME_HEAD + (usLocalIndex >> 2), frameHead);

    if ((ActOrOdType !=  ACTIVE_OAM) && (ActOrOdType !=  FDI_OAM))
    {
        iRet = spm_odindex_by_activeindex(usOamLoop, &ucodLoop);

        if (SUCCESS != iRet)
        {
            goto EXIT_LABEL;
        }

        ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    }

    /* 判断vpn的类型是VPWS/VPLS */
    /* 若是VPWS */
    if (VPWS_TYPE == g_stoam_local[usOamLoop].vpn_type)
    {
        /* Dtype字段赋值 */
        stVpwsDtype.PktT = 0; /*  代表vc层vpws的oam帧 */
        stVpwsDtype.Dtype = 4; /* 代表vpws vc层oam帧 */
        stVpwsDtype.Reserved = 0;
        stVpwsDtype.chip_id = g_stoam_local[usOamLoop].chip_id;
        usVpwsDtype = *(NBB_USHORT *)(&stVpwsDtype);

        /* 迭代出pwlable值 */
        /* 条目N的发送标签使能,填入tunnel和pw标签 */
        switch (ActOrOdType)
        {
            case ACTIVE_OAM :
                spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET1 + usLocalIndex,
                                0x0000);
                spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET2 + usLocalIndex,
                                usVlanId);
                spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET3 + usLocalIndex,
                                g_stoam_local[usOamLoop].vc_no_section1);
                spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET1 + usLocalIndex,
                                usVpwsDtype);
                spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET2 + usLocalIndex,
                                0x0000);
                spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET3 + usLocalIndex,
                                0x0000);
                
                /* TMS/TMP/TMC层CV&APS/CSF成帧MEG配置 */
                spm_oam_tx_megfpga(usOamLoop);

                spm_oam_tx_enablefpga(usOamLoop, ACTIVE_OAM);
                break;

            case LB_OAM:
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                                0x0000);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                                usVlanId);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                                g_stoam_local[usOamLoop].vc_no_section1);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                                usVpwsDtype);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                                0x0000);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                                0x0000);
                break;

            case LCK_OAM:
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                                0x0000);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                                usVlanId);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                                g_stoam_local[usOamLoop].vc_no_section1);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                                usVpwsDtype);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                                0x0c01);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                                0x0102);  /* IPADDRESS 需修改 */
                break;

            case DMM_OAM:

                /* DMM发向 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                                0x0000);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                                usVlanId);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                                g_stoam_local[usOamLoop].vc_no_section1);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x6,
                                usVpwsDtype);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x7,
                                0x0c01);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x8,
                                0x0102);  /* IPADDRESS 需修改 */
                break;

            case DMR_OAM:

                /* DMR发向 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x0,
                                0x0000);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x1,
                                usVlanId);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x2,
                                g_stoam_local[usOamLoop].vc_no_section1);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                                usVpwsDtype);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                                0x0c01);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                                0x0102);  /* IPADDRESS 需修改 */
                break;

            case LMM_OAM:

                /* LMM发向 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                                0x0000);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                                usVlanId);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                                g_stoam_local[usOamLoop].vc_no_section1);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x6,
                                usVpwsDtype);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x7,
                                0x0c01);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x8,
                                0x0102);  /* IPADDRESS 需修改 */
                break;

            case LMR_OAM:

                /* DMR发向 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x0,
                                0x0000);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x1,
                                usVlanId);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x2,
                                g_stoam_local[usOamLoop].vc_no_section1);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                                usVpwsDtype);
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                                0x0c01);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                                0x0102);  /* IPADDRESS 需修改 */
                break;

            case TST_OAM:
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                                0x0000);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                                usVlanId);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                                g_stoam_local[usOamLoop].vc_no_section1);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                                usVpwsDtype);
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                                0x0c01);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                                0x0102);  /* IPADDRESS 需修改 */
                break;

            case FDI_OAM:
                spm_oam_16write(baseAddr,  FDI_TX_DMAC_OFFSET1 + usLocalIndex,
                                0x0000);
                spm_oam_16write(baseAddr,  FDI_TX_DMAC_OFFSET2 + usLocalIndex,
                                usVlanId);
                spm_oam_16write(baseAddr,  FDI_TX_DMAC_OFFSET3 + usLocalIndex,
                                g_stoam_local[usOamLoop].vc_no_section2);
                spm_oam_16write(baseAddr,  FDI_TX_SMAC_OFFSET1 + usLocalIndex,
                                usVpwsDtype);
                spm_oam_16write(baseAddr,  FDI_TX_SMAC_OFFSET2 + usLocalIndex,
                                0x0c01);  /* IPADDRESS 需修改 */
                spm_oam_16write(baseAddr,  FDI_TX_SMAC_OFFSET3 + usLocalIndex,
                                0x0102);  /* IPADDRESS 需修改 */
                spm_oam_tx_enablefpga(usOamLoop, FDI_OAM);
                break;

            default:
                OS_SPRINTF(g_uc_message, "%s %s,%d\n frame type error,EXIT\n"
                           "frame type for writing mac field error\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
        }
    }
    else
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n vpn type is vpls,EXIT! vpn_type=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, g_stoam_local[usOamLoop].vpn_type);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 对等MEP ID写入寄存器 */
    spm_oam_16write(baseAddr, CV_EXPECT_MEP_OFFSET + usLocalIndex, g_stoam_local[usOamLoop].dst_mep_id);
EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_delete
 功能描述  : 删除配置
 输入参数  : NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月4日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_delete(IN NBB_USHORT usLoop)
{
    NBB_INT iRet = ERROR; /* 表示添加成功或失败的变量 */
    SPM_OAMTREE_NODE stOamTreeNode;
    NBB_BYTE ucPort = 0;
    NBB_ULONG ulOamId = 0;
    OS_MEMSET(&stOamTreeNode, 0, sizeof(SPM_OAMTREE_NODE));

    if (usLoop >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        goto EXIT_LABEL;
    }

    ulOamId = g_stoam_local[usLoop].ulActiveOamkey;
    OS_SPRINTF(g_uc_message, "%s %s,%d\noamid=%d\n", ACT_OAM_ENTER_STRING,
               __FUNCTION__, __LINE__, ulOamId);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    stOamTreeNode.ulTunnelId = g_stoam_local[usLoop].ulTunnelLabel_rx; /* 待添加 */
    stOamTreeNode.ulPwId = g_stoam_local[usLoop].ulPwLable_rx; /* 待添加 */
    stOamTreeNode.usLspNo = g_stoam_local[usLoop].module_index;

    //判断是在模块1还是在模块2内删除配置
    iRet = spm_oam_tree_delete(&stOamTreeNode,
                               OAM_REG_BASE1_ADDRESS + REG_DIFF_VALUE * g_stoam_local[usLoop].bModuleSwitch,
                               g_stoam_local[usLoop].bModuleSwitch, 0);

    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "delete oam tree in the first module failed ret=%d\n"
                   "delete oam failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId, iRet);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 清除sd轮询 */
    if (VS_OAM == g_stoam_local[usLoop].oam_type)
    {
        spm_delete_record_sd_oamloop(usLoop);
    }
    else if (WRAPPING_OAM == g_stoam_local[usLoop].oam_type)
    {
        spm_delete_record_wrapping_oamloop(usLoop);
    }
    
    /* 清除相关寄存器内容 */
    spm_oam_delete_fpga(usLoop);

    /* 判断本条oam是否关联了保护 */
    if (TRUE == g_stoam_local[usLoop].bIsRelateProt)
    {
        //spm_del_tp_oam_cfg_if(1, ulOamId);
    }

    //若取消FDI帧定时器
    BmuWdCancel(g_stoam_local[usLoop].fdi_monitor);

    /* 将本地存储配置置为0 */
    OS_MEMSET(&g_stoam_local[usLoop], 0, sizeof(OAM_ACTIVE_LOCAL_STRU));

    OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d, delete oam success!\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__, ulOamId);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_od_delete
 功能描述  : 删除按需配置寄存器
 输入参数  : IN NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月27日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_od_delete(IN NBB_BYTE ucOdLoop)
{
    NBB_BYTE ucLoop = 0;
    NBB_INT iRet = ERROR;
    NBB_USHORT usOamLoopi;
    NBB_ULONG baseAddr;
    NBB_BYTE ucOdDmloop = 0;

    if (ucOdLoop >= OAM_MAX_NUM_OD)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n ucOdLoop=%d\n"
                   "ucOdLoop is illegal\n"
                   "delete od oam failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ucOdLoop);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    usOamLoopi = g_stoam_odlocal[ucOdLoop].usOamLoop;
    ucLoop = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;
    
    /* 判断所在模块基地址 */
    spm_get_moudle_base_addr(usOamLoopi, &baseAddr);

    if (g_stoam_odlocal[ucOdLoop].ucOdLoop > 0)
    {
        /* dm */
        spm_dm_delete_fpga(baseAddr, ucLoop);
        
        /* lb */
        spm_lb_delete_fpga(baseAddr, ucLoop);
        
        /* lm */
        spm_lm_delete_fpga(baseAddr, ucLoop);
        
        /* lck */
        spm_lck_delete_fpga(baseAddr, ucLoop);
        
        /* tst */
        spm_tst_delete_fpga(baseAddr, ucLoop);

        //按需的删除是删除整个按需配置块，不会单个子配置块
        spm_mep_meg_delete_fpga(baseAddr, ucLoop);

        spm_rx_label_delete_fpga(baseAddr, ucLoop);
    }

    //若删除的是离线test，需要关掉lck锁定业务功能
    if (NULL != g_stoam_odlocal[ucOdLoop].pTestFrame)
    {
        if ((TRUE == g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.send_enable)
            && (OAM_TEST_OFFLINE ==
                g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.send_method))
        {
            spm_tst_offline_lock_flow(usOamLoopi, ucOdLoop, 0x0);
        }
    }

    iRet = spm_delete_od_num(usOamLoopi, ucOdLoop);

    if (SUCCESS != iRet)
    {
        goto EXIT_LABEL;
    }

    spm_memfree_od_oam_cfg(&g_stoam_odlocal[ucOdLoop]);

    OS_MEMSET(&g_stoam_odlocal[ucOdLoop], 0, sizeof(OAM_ON_DEMAND_LOCAL_STRU));

    OS_SPRINTF(g_uc_message, "%s %s,%d\ndelete od oam success!\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_od_lb
 功能描述  : 按需LB帧头信息字节寄存器配置
 输入参数  : NBB_BYTE usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月1日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_mep_lb(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    OD_OAM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdLbFrame;
    NBB_ULONG baseAddr;
    NBB_BYTE odLoop = 0;
    NBB_INT iRet = ERROR;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    if ((NULL == g_stoam_odlocal[ucOdLoop].pLoopFrame)
        || (NULL == g_stoam_odlocal[ucOdLoop].pBasicData))
    {
        goto EXIT_LABEL;
    }

    stOdFrame.Port = 0;//690设备只有一个端口，此处可以不填，默认为0.
    stOdFrame.Mel = g_stoam_odlocal[ucOdLoop].pBasicData->mel;

    //连接性监测,必须带request mep id tlv;诊断模式，不带
    if ((0x0 == g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.lb_mode)
        || (0x1 == g_stoam_odlocal[ucOdLoop].pLoopFrame->mep_id_enable))
    {
        stOdFrame.LbMode = 0x0;
    }
    else
    {
        stOdFrame.LbMode = 0x1;
    }

    stOdFrame.VpAttribute = g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_mode;
    stOdFrame.SendCycle = g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.send_cycle;

    //若是离线方式，则发送测试tlv；若是在线方式，则发送数据tlv
    if (0x0 == g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.test_method)//在线
    {
        stOdFrame.SendDataType = 0x0;
    }
    else
    {
        stOdFrame.SendDataType = g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.test_tlv_type;
    }

    /* 当vp属性为mep时才发lbm，发送使能 */
    stOdFrame.SendEnable = g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.send_enable;

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    odLoop = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;
    usOdLbFrame = *(NBB_USHORT *)(&stOdFrame);
    spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET
                    + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0,
                    usOdLbFrame NBB_CCXT_T NBB_CCXT);
    
    /* LB发送数据字节数最小值25,最大值1396 */
    if ((25 > g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length) ||
        (1396 < g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length))
    {
        g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length = 25;
    }

    spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET +
                    LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x7,
                    (g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length) NBB_CCXT_T NBB_CCXT);
    spm_od_tmpls_lable_fpga(ucOdLoop, LB_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LB_OAM);
    spm_od_send_data_fpga(ucOdLoop, LB_OAM);
    spm_od_mep_meg_fpga(usOamLoop, ucOdLoop);

    /* 当配置为离线测试时，主动发lck帧 */
    if (TRUE == g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.test_method)
    {
        spm_od_lck(usOamLoop, ucOdLoop);
    }

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_mip_lb
 功能描述  : mip节点发lbr报文，建acl
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月18日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_mip_lb(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\ninput parameter illegal\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        return;
    }

    NBB_INT iRet = ERROR;

    if (NULL == g_stoam_odlocal[ucOdLoop].pBasicData)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nod basic cfg is null\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        return;
    }

    /* 节点属性为MIP时，自动配置ACL */
    switch (g_stoam_local[usOamLoop].oam_type)
    {
        case VS_OAM:

            break;

        case VP_OAM:
            spm_set_acl_for_lb_lsp(usOamLoop, ucOdLoop);
            break;

        case VC_OAM:
            spm_set_acl_for_lb_pw(usOamLoop, ucOdLoop);
            break;

        default:
            break;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lb
 功能描述  : lb报文发送
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月18日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lb(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        return;
    }

    if (0x1 == g_stoam_local[usOamLoop].vp_attrib)
    {
        spm_od_mep_lb(usOamLoop, ucOdLoop);
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lck
 功能描述  : lck帧发送
 输入参数  : IN NBB_USHORT usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月2日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lck(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop NBB_CCXT_T NBB_CXT)
{
    OD_OAM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdLckFrame;
    NBB_ULONG ulLspLabel;
    NBB_ULONG ulPwLabel;
    NBB_ULONG baseAddr;
    NBB_BYTE odLoop = 0;
    NBB_BYTE lckEnable = 0;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    odLoop = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;

    /* 当lck使能或者测试方式是离线时 */
    stOdFrame.SendCycle = 4;/* 默认值为“4”（即发送周期为1秒），还可设置成
                               “6”（即发送周期为1分钟） */
                                
    /* 如果tst是离线测试，则发lck */
    spm_decide_lck_or_tst(ucOdLoop, &lckEnable);
    stOdFrame.SendEnable = lckEnable;
    stOdFrame.Mel = g_stoam_local[usOamLoop].mel;
    usOdLckFrame = *(NBB_USHORT *)(&stOdFrame);
    spm_oam_16write(baseAddr,
                    DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0,
                    usOdLckFrame NBB_CCXT_T NBB_CCXT);
    switch (g_stoam_local[usOamLoop].oam_type)
    {
        case VS_OAM :
            spm_oam_tx_tms_mac_fpga(usOamLoop, LCK_OAM);
            break;

        case VP_OAM :
            spm_oam_tx_tmp_mac_fpga(usOamLoop, LCK_OAM);
            break;

        case VC_OAM :
            spm_oam_tx_vpws_mac_fpga(usOamLoop, LCK_OAM);
            break;

        default:
            break;
    }

    spm_od_tmpls_lable_fpga(ucOdLoop, LCK_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LCK_OAM);

EXIT_LABEL:
    return;
}



/*****************************************************************************
 函 数 名  : spm_od_lm
 功能描述  : 按需LM帧收发
 输入参数  : IN NBB_USHORT usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年10月22日
    作    者   : jwwang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lm(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    NBB_BYTE odLoop = 0;
    OD_OAM_DM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdDmFrame;
    NBB_INT ulRtn = ERROR;
    NBB_ULONG baseAddr = 0;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_DM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        return;
    }

    if ((NULL == g_stoam_odlocal[ucOdLoop].pBasicData)
        || (NULL == g_stoam_odlocal[ucOdLoop].pLmFrame))
    {
        goto EXIT_LABEL;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);

    odLoop = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;


    /* 发向 */
    stOdFrame.Mel = g_stoam_odlocal[ucOdLoop].pBasicData->mel;
    stOdFrame.vlan = 0;
    stOdFrame.pad = 0;
    stOdFrame.pad2 = 0;
    stOdFrame.SendCycle = g_stoam_odlocal[ucOdLoop].pLmFrame->byte1.send_cycle;

    /* 周期 */
    if (FALSE == g_stoam_odlocal[ucOdLoop].pLmFrame->byte1.send_cycle)
    {
        stOdFrame.SendCycle = one_s;
    }
    else
    {
        stOdFrame.SendCycle = hundred_ms;
    }

    stOdFrame.SendEnable = g_stoam_odlocal[ucOdLoop].pLmFrame->byte1.send_enable;
    usOdDmFrame = *(NBB_USHORT *)(&stOdFrame);

    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET +
                    LM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x0, usOdDmFrame);

    //保留
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET +
                    LM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x1, 0x0000);

    //长度
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET +
                    LM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x2, 64);

    switch (g_stoam_local[usOamLoop].oam_type)
    {
        case VS_OAM :
            spm_oam_tx_tms_mac_fpga(usOamLoop, LMM_OAM);
            break;

        case VP_OAM :
            spm_oam_tx_tmp_mac_fpga(usOamLoop, LMM_OAM);
            break;

        case VC_OAM :
            spm_oam_tx_vpws_mac_fpga(usOamLoop, LMM_OAM);
            break;

        default:
            break;
    }

    spm_od_tmpls_lable_fpga(ucOdLoop, LMM_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LMM_OAM);
    spm_od_send_data_fpga(ucOdLoop, LMM_OAM);

    //LMM保留
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET +
                    LM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0xd, 0x0000);
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET +
                    LM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0xf, 0x0000);

    g_updata_flag = 0;
EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_dm
 功能描述  : 按需DM帧收发
 输入参数  : IN NBB_USHORT usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_dm(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
#if 0
    OD_OAM_DM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdDmFrame;
    NBB_ULONG ulLspLabel;
    NBB_ULONG ulPwLabel;
    NBB_ULONG baseAddr;
    NBB_BYTE odLoop = 0;
    NBB_INT ulRtn = ERROR;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_DM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    if ((NULL == g_stoam_odlocal[ucOdLoop].pBasicData)
        || (NULL == g_stoam_odlocal[ucOdLoop].pDmFrame))
    {
        goto EXIT_LABEL;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);

    odLoop = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;

    /* 发向 */
    stOdFrame.Mel = g_stoam_odlocal[ucOdLoop].pBasicData->mel;
    stOdFrame.vlan = 0;
    stOdFrame.pad = 0;
    stOdFrame.pad2 = 0;
    stOdFrame.SendCycle = g_stoam_odlocal[ucOdLoop].pDmFrame->byte1.send_cycle;

    /* 周期 */
    if (FALSE == g_stoam_odlocal[ucOdLoop].pDmFrame->byte1.send_cycle)
    {
        stOdFrame.SendCycle = one_s;
    }
    else
    {
        stOdFrame.SendCycle = hundred_ms;
    }

    stOdFrame.SendEnable = g_stoam_odlocal[ucOdLoop].pDmFrame->byte1.send_enable;
    usOdDmFrame = *(NBB_USHORT *)(&stOdFrame);

    /* dm发送数据字节数最小值为4，最大值为1452 */
    if ((4 > g_stoam_odlocal[ucOdLoop].pDmFrame->frame_length) ||
        1452 < g_stoam_odlocal[ucOdLoop].pDmFrame->frame_length)
    {
        g_stoam_odlocal[ucOdLoop].pDmFrame->frame_length = 4;
    }

    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x0,
                    usOdDmFrame NBB_CCXT_T NBB_CCXT);

    //保留
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x1,
                    0x0000);

    //长度
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x2,
                    (g_stoam_odlocal[ucOdLoop].pDmFrame->frame_length));

    switch (g_stoam_local[usOamLoop].oam_type)
    {
        case VS_OAM :
            spm_oam_tx_tms_mac_fpga(usOamLoop, DMM_OAM);
            break;

        case VP_OAM :
            spm_oam_tx_tmp_mac_fpga(usOamLoop, DMM_OAM);
            break;

        case VC_OAM :
            spm_oam_tx_vpws_mac_fpga(usOamLoop, DMM_OAM);
            break;

        default:
            break;
    }

    spm_od_tmpls_lable_fpga(ucOdLoop, DMM_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, DMM_OAM);
    spm_od_send_data_fpga(ucOdLoop, DMM_OAM);

    //DMM保留
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET +
                    DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0xd,
                    0x0000);
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET +
                    DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0xf,
                    0x0000);

EXIT_LABEL:
#endif
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_tst
 功能描述  : TST帧收发
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_tst(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    OD_OAM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdTstFrame;
    NBB_ULONG ulLspLabel;
    NBB_ULONG ulPwLabel;
    NBB_ULONG baseAddr;
    NBB_BYTE odLoop = 0;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    if (NULL == g_stoam_odlocal[ucOdLoop].pTestFrame)
    {
        goto EXIT_LABEL;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    odLoop = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;

    /* 发向 */
    stOdFrame.SendCycle = g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.send_cycle;
    stOdFrame.SendDataType = g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.test_tlv_type;
    stOdFrame.SendEnable = g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.send_enable;
    stOdFrame.Mel = g_stoam_local[usOamLoop].mel;
    usOdTstFrame = *(NBB_USHORT *)(&stOdFrame);
    spm_oam_16write(baseAddr,
                    DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0,
                    usOdTstFrame NBB_CCXT_T NBB_CCXT);

    switch (g_stoam_local[usOamLoop].oam_type)
    {
        case VS_OAM :
            spm_oam_tx_tms_mac_fpga(usOamLoop, TST_OAM);
            break;

        case VP_OAM :
            spm_oam_tx_tmp_mac_fpga(usOamLoop, TST_OAM);
            break;

        case VC_OAM :
            spm_oam_tx_vpws_mac_fpga(usOamLoop, TST_OAM);
            break;

        default:
            break;
    }

    /* tst发送数据字节数最小值为25，最大值为1480 */
    if ((25 > g_stoam_odlocal[ucOdLoop].pTestFrame->tlv_length) ||
        1480 < g_stoam_odlocal[ucOdLoop].pTestFrame->tlv_length)
    {
        g_stoam_odlocal[ucOdLoop].pTestFrame->tlv_length = 25;
    }

    spm_oam_16write(baseAddr,
                    DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x7,
                    (g_stoam_odlocal[ucOdLoop].pTestFrame->tlv_length) NBB_CCXT_T NBB_CCXT);
    spm_od_tmpls_lable_fpga(ucOdLoop, TST_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, TST_OAM);

    /* 当TST离线测试时，主动发lck帧并关断业务 */
    if (TRUE == g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.send_enable)
    {
        if (OAM_TEST_OFFLINE == g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.send_method)
        {
            spm_od_lck(usOamLoop, ucOdLoop);
            spm_tst_offline_lock_flow(usOamLoop, ucOdLoop, 0x1);
        }
        else
        {
            spm_od_lck(usOamLoop, ucOdLoop);

            //如果TST更新为不使能或者在线测试，业务关断功能应该去掉
            spm_tst_offline_lock_flow(usOamLoop, ucOdLoop, 0x0);
        }
    }
    else
    {
        spm_od_lck(usOamLoop, ucOdLoop);

        //如果TST更新为不使能或者在线测试，业务关断功能应该去掉
        spm_tst_offline_lock_flow(usOamLoop, ucOdLoop, 0x0);
    }

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_wrapping
 功能描述  : wrapping V3oam
 输入参数  : IN NBB_USHORT usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月25日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_wrapping(IN SPM_OAM_CB *poamcfg)
{
    NBB_USHORT usLoop = 0;
    NBB_INT iRet = ERROR;

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    usLoop = poamcfg->oamindex;

    if (NULL != poamcfg->pWrapping)
    {
        spm_wrapping_mac(usLoop);
        spm_wrapping_label(usLoop);
        spm_oam_tx_megfpga(usLoop);
    }

    iRet = spm_oam_wrapping_build_acl(usLoop);

    if (SUCCESS != iRet)
    {
        goto EXIT_LABEL;
    }

EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_od_lbr_dmr
 功能描述  : lbr和dmr写入寄存器
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月18日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lbr_dmr(IN NBB_USHORT usOamLoop, NBB_BYTE ucOdLoop)
{
    NBB_ULONG baseAddr = 0;
    NBB_BYTE odLoop = 0;
    NBB_USHORT ucOdLoopi = 0;
    NBB_INT iRet = ERROR;
    NBB_ULONG NodeId = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        return;
    }

    //回lbr要将本端的TARGET MEP ID与报文的REQUEST MEP ID匹配
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    odLoop = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;
    spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * odLoop + 0x0,
                    g_stoam_local[usOamLoop].dst_mep_id NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * odLoop + 0x9,
                    g_stoam_local[usOamLoop].src_mep_id NBB_CCXT_T NBB_CCXT);

    if (0x0 == g_stoam_local[usOamLoop].vp_attrib)
    {
        spm_od_mip_lb(usOamLoop, ucOdLoop);

        //对mip节点环回的时候，mip节点要匹配此字段，约定固定为1.
        NodeId = 1;
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE
                        + SOURCE_NODE_ID_OFFSET + odLoop * 0x2 + 0x0,
                        ((NodeId >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE
                        + SOURCE_NODE_ID_OFFSET + odLoop * 0x2 + 0x1,
                        (NodeId & 0xFFFF) NBB_CCXT_T NBB_CCXT);
    }

    /* lb/dm不使能，FPGA收到lbm/dmm后也会回lbr/dmr */
    if (VS_OAM == g_stoam_local[usOamLoop].oam_type)
    {
        spm_oam_tx_tms_mac_fpga(usOamLoop, LB_OAM);

        spm_oam_tx_tms_mac_fpga(usOamLoop, DMR_OAM);

        spm_oam_tx_tms_mac_fpga(usOamLoop, LMR_OAM);
    }
    else if (VP_OAM == g_stoam_local[usOamLoop].oam_type)
    {
        if (0x0 == g_stoam_local[usOamLoop].vp_attrib)
        {
            spm_mip_lb_tmp_mac_fpga(usOamLoop);
        }
        else
        {
            spm_oam_tx_tmp_mac_fpga(usOamLoop, LB_OAM);
        }

        spm_oam_tx_tmp_mac_fpga(usOamLoop, DMR_OAM);
        spm_oam_tx_tmp_mac_fpga(usOamLoop, LMR_OAM);
    }
    else if (VC_OAM == g_stoam_local[usOamLoop].oam_type)
    {
        if (0x0 == g_stoam_local[usOamLoop].vp_attrib)
        {
            spm_mip_lb_tmc_mac_fpga(usOamLoop);
        }
        else
        {
            spm_oam_tx_vpws_mac_fpga(usOamLoop, LB_OAM);
        }

        spm_oam_tx_vpws_mac_fpga(usOamLoop, DMR_OAM);
        spm_oam_tx_vpws_mac_fpga(usOamLoop, LMR_OAM);
    }

    spm_od_tmpls_lable_fpga(ucOdLoop, DMR_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, DMR_OAM);

    spm_od_tmpls_lable_fpga(ucOdLoop, LMR_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LMR_OAM);
    spm_od_tmpls_lable_fpga(ucOdLoop, LB_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LB_OAM);
    return;
}

/*****************************************************************************
 函 数 名  : spm_set_mpls_flag
 功能描述  : a
 输入参数  : SPM_OAM_CB *pOamCfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_mpls_flag(SPM_OAM_CB *pOamCfg)
{
    NBB_INT iRet = ERROR;
    NBB_BYTE labelAction = 0;
    NBB_USHORT usLoop = 0;
    NBB_ULONG ulOamId = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((NULL == pOamCfg) || (NULL == pOamCfg->pBasiCfg))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\ninput pointer is null\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (0x0 == pOamCfg->pBasiCfg->vp_attrib)
    {
        iRet = SUCCESS;//mip结点，标签动作为swap，不能写mpls表
        goto EXIT_LABEL;
    }

    usLoop = pOamCfg->oamindex;
    ulOamId = pOamCfg->oam_id;

    switch (pOamCfg->oamtype)
    {
#if 0
        case VS_OAM:
            /* 写oamid与标签的绑定关系表格 */
            iRet = ApiC3SetIngVsOamFlag(pOamCfg->chip_id,
                                        pOamCfg->oamindex,
                                        pOamCfg->ulPwLable_rx);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "build oamid & label table failed ret=%d\n"
                           "chip_id=%d,global_index=%d,ulPwLable_rx=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->oamindex,
                           pOamCfg->ulPwLable_rx);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            iRet = ApiC3SetIngVsOamId(pOamCfg->chip_id,
                                      g_stoam_local[usLoop].vs_Portid,
                                      pOamCfg->oamindex);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "call active lm rx count failed ret=%d\n"
                           "chip_id=%d,Portid=%d,oamindex=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, g_stoam_local[usLoop].vs_Portid,
                           pOamCfg->oamindex);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            iRet = ApiC3SetEgrVsOamId(pOamCfg->chip_id,
                                      g_stoam_local[usLoop].vs_Portid,
                                      pOamCfg->oamindex);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "call active lm tx count failed ret=%d\n"
                           "chip_id=%d,Portid=%d,oamindex=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, g_stoam_local[usLoop].vs_Portid,
                           pOamCfg->oamindex);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            break;

        case VP_OAM:
            iRet = spm_get_label_action(pOamCfg, &labelAction);

            if (SUCCESS != iRet)
            {
                goto EXIT_LABEL;
            }

            if (0x0 == labelAction)//swap
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n lable action is swap!\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            /* 写oamid与标签的绑定关系表格 */
            iRet = ApiC3SetMplsOamFlag(pOamCfg->chip_id, pOamCfg->oamindex,
                                       pOamCfg->ulTunnelLabel_rx, 0x1);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "build oamid & label table failed ret=%d\n"
                           "chip_id=%d,global_index=%d,ulTunnelLabel_rx=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->oamindex,
                           pOamCfg->ulTunnelLabel_rx);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            //上送槽位
            iRet = ApiC3SetMplsMcFlag(pOamCfg->chip_id, pOamCfg->mcFlag,
                                      pOamCfg->ulTunnelLabel_rx, pOamCfg->slotMcid);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "call ApiC3SetMplsMcFlag() failed ret=%d\n"
                           "chip_id=%d,mcFlag=%d,ulTunnelLabel_rx=%d,slotMcid=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->mcFlag,
                           pOamCfg->ulTunnelLabel_rx, pOamCfg->slotMcid);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            iRet = ApiC3SetMplsVpvcOamId(pOamCfg->chip_id,
                                         pOamCfg->ulTunnelLabel_rx,
                                         pOamCfg->oamindex);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "call active lm rx count failed ret=%d\n"
                           "chip_id=%d,ulTunnelLabel_rx=%d,oamindex=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->ulTunnelLabel_rx,
                           pOamCfg->oamindex);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            iRet = ApiC3SetLspVpOamId(pOamCfg->chip_id,
                                      pOamCfg->lsp_id_tx,
                                      pOamCfg->oamindex);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "call active lm tx count failed ret=%d\n"
                           "chip_id=%d,Lspid=%d,oamindex=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->lsp_id_tx,
                           pOamCfg->oamindex);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            break;

        case VC_OAM:
            /* 写oamid与标签的绑定关系表格 */
            iRet = ApiC3SetMplsOamFlag(pOamCfg->chip_id, pOamCfg->oamindex,
                                       pOamCfg->ulPwLable_rx, 0x0);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "build oamid & label table failed ret=%d\n"
                           "chip_id=%d,global_index=%d,ulPwLable_rx=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->oamindex, pOamCfg->ulPwLable_rx);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            //上送槽位
            iRet = ApiC3SetMplsMcFlag(pOamCfg->chip_id, pOamCfg->mcFlag,
                                      pOamCfg->ulPwLable_rx, pOamCfg->slotMcid);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "call ApiC3SetMplsMcFlag() failed ret=%d\n"
                           "chip_id=%d,mcFlag=%d,ulPwLable_rx=%d,slotMcid=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->mcFlag,
                           pOamCfg->ulPwLable_rx, pOamCfg->slotMcid);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

            iRet = ApiC3SetMplsVpvcOamId(pOamCfg->chip_id,
                                         pOamCfg->ulPwLable_rx,
                                         pOamCfg->oamindex);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "call active lm rx count failed ret=%d\n"
                           "chip_id=%d,ulPwLable_rx=%d,oamindex=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->ulPwLable_rx,
                           pOamCfg->oamindex);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            iRet = ApiC3SetVpVcOamId(pOamCfg->chip_id,
                                     pOamCfg->vc_no,
                                     pOamCfg->oamindex);

            if (SUCCESS != iRet)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "call active lm tx count failed ret=%d\n"
                           "chip_id=%d,vp_id=%d,oamindex=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId,
                           iRet, pOamCfg->chip_id, pOamCfg->vc_no,
                           pOamCfg->oamindex);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            }

            break;
#endif 
        default:
            iRet = SUCCESS;
            break;
    }

EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_set_tpoam_change
 功能描述  : a
 输入参数  : NBB_BYTE changeType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_tpoam_change(NBB_BYTE changeType)
{
    NBB_INT iRet = ERROR;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
#if 0
    iRet = spm_set_tp_oam_cfg_change_if(changeType);

    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "call spm_set_tp_oam_cfg_change_if failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iRet);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    }
#endif
    iRet = SUCCESS;
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_wrapping_build_acl
 功能描述  : P节点和PE节点，wrapping报文都需要建acl提取
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月20日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_wrapping_build_acl(IN NBB_USHORT usOamLoop)
{
    NBB_INT iRet = ERROR;
    NBB_ULONG inport_index = 0;
    NBB_ULONG ulLspLabel1;
    NBB_ULONG ulLspLabel2;
    NBB_ULONG ulPwLabel;
    NBB_ULONG ach_32bit = 0;
    NBB_ULONG customdata[3] = {0};
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        return iRet;
    }

    ach_32bit = ACH_32BIT;
    ach_32bit = ((ach_32bit << 16) + g_channel_type);
    inport_index = g_stoam_local[usOamLoop].wrapping_port_logical_index;
    spm_set_mpls_and_gal_label_for_acl(usOamLoop, &ulLspLabel1,
                                       &ulLspLabel2, &ulPwLabel);
    customdata[0] = ulLspLabel2;
    customdata[1] = ulPwLabel;
    customdata[2] = ach_32bit;
    g_stoam_local[usOamLoop].aclRuleId = g_usoam_num;
    iRet = spm_build_acl_for_wrapping(inport_index, customdata);

    if (SUCCESS != iRet)
    {
        return iRet;
    }

    iRet = SUCCESS;
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_csf
 功能描述  : csf帧寄存器配置
 输入参数  : IN NBB_USHORT usOamIndex
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月20日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_csf(IN NBB_USHORT usOamLoop NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT us_uni_port_id = 0;
    NBB_ULONG baseAddr;
    NBB_USHORT usLocalIndex;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1))
    {
        goto EXIT_LABEL;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    usLocalIndex = g_stoam_local[usOamLoop].module_index;
    
    /* 只vc层有csf */
    spm_oam_tx_vpws_mac_fpga(usOamLoop, ACTIVE_OAM);

    us_uni_port_id = g_stoam_local[usOamLoop].vc_PortId_section1 - 1;
    spm_oam_16write(baseAddr, CV_TXVLAN_LABEL_OFFSET + usLocalIndex * 8 + 7,
                    us_uni_port_id NBB_CCXT_T NBB_CCXT);
EXIT_LABEL:
    return;
}

#endif /* SPU */

#ifdef SRC

/****************************************************************************
 *                                                                          *
 * Function    : spm_oam_set_syn_oam_id_data                                *
 *                                                                          *
 * Description : This function handles OAM ID sync messages in CCUT1.       *
 *                                                                          *
 * Input       : pBuf       - The pointer to the message.                   *
 *               ulMsgLenth - The length of message.                        *
 *               usSlotid   - The slot from which the message is sent.      *
 * Output      :                                                            *
 *                                                                          *
 * Return      :                                                            *
 *                                                                          *
 * Others      : 64k_alarm_inband                                           *
 *                                                                          *
 ****************************************************************************/
NBB_INT spm_oam_set_syn_oam_id_data(IN NBB_VOID *pBuf,
                                    IN NBB_ULONG ulMsgLenth, IN NBB_USHORT usSlotid)
{
    NBB_INT     iRet        = BFD_ERROR_BASE;
#if 0
    NBB_USHORT  usLoopi     = 0;
    NBB_BYTE    *pusTmp     = NULL;
    NBB_INT     bfdMaxNum   = 0;
    NBB_USHORT  oam_index   = 0;
    NBB_ULONG   oam_id      = 0;
    NBB_BYTE    en          = 0;
    NBB_ULONG   fpga_value  = 0;

    if (NULL == pBuf)
    {
        iRet = BFD_ERROR_PAMT_NULL;
        goto EXIT_LABEL;
    }

    /* 报文长度不够存储本地的结构体,由于是单盘把单盘本地的结构往外发送的，

    有可能出现单盘升级而主控没有升级的情况，此时单盘传的条目可能比主控多，也可能 */
    /* 单盘传的条目比主控少，只需要处理单盘传过来的条目 */

//   if(ulMsgLenth < sizeof(SPM_OAM_SYN_OAM_ID))

    if (ulMsgLenth <= OAM_MAX_NUM_ACTIVE_SPU * (2 + 4 + 1))
    {
        //bfdMaxNum = (ulMsgLenth - NBB_OFFSETOF(SPM_OAM_SYN_OAM_ID, arrucOamId))/ sizeof(NBB_USHORT);
        bfdMaxNum = ulMsgLenth / (2 + 4 + 1);
    }
    else    /* 单盘传的条目和主控相等或比主控多，只能处理主控支持的条目 */
    {
        bfdMaxNum = OAM_MAX_NUM_ACTIVE_SPU;
    }

    //g_stOamIdStoreCnt = bfdMaxNum;

    pusTmp = (NBB_BYTE *)pBuf;

    /* 轮询配置 */
    for (usLoopi = 0; usLoopi < bfdMaxNum; usLoopi++)
    {
        //SPM_L3_RD_U16(oam_index, pusTmp);
        //SPM_L3_RD_U32(oam_id, pusTmp);
        //SPM_L3_RD_U8(en, pusTmp);

        if (1 == en)
        {
            if (1 != g_stoam_idstore[usSlotid - 1].stOamIdInfo[oam_index].m_enable_flag)
            {
                fpga_value = (usSlotid - 1) * 4096 + oam_index;
                spm_oam_fpga_write_32bits(SRC_DDR3_BASEADDR, oam_id - 1, fpga_value);
                g_stoam_idstore[usSlotid - 1].stOamIdInfo[oam_index].m_enable_flag = 1;
                g_stoam_idstore[usSlotid - 1].stOamIdInfo[oam_index].m_oam_id = oam_id;
            }
            else
            {
                if (oam_id != g_stoam_idstore[usSlotid - 1].stOamIdInfo[oam_index].m_oam_id)
                {
                    fpga_value = (usSlotid - 1) * 4096 + oam_index;
                    spm_oam_fpga_write_32bits(SRC_DDR3_BASEADDR, oam_id - 1, fpga_value);
                    g_stoam_idstore[usSlotid - 1].stOamIdInfo[oam_index].m_oam_id = oam_id;
                }
            }
        }
        else if (0 == en)
        {
            if (1 == g_stoam_idstore[usSlotid - 1].stOamIdInfo[oam_index].m_enable_flag)
            {
                fpga_value = 0x7ffff;
                spm_oam_fpga_write_32bits(SRC_DDR3_BASEADDR, oam_id - 1,
                                          fpga_value);

                g_stoam_idstore[usSlotid - 1].stOamIdInfo[oam_index].m_enable_flag = 0;
                g_stoam_idstore[usSlotid - 1].stOamIdInfo[oam_index].m_oam_id = 0;
            }
        }

//        g_stoam_idstore[usSlotid - 1].stOamIdInfo[usLoopi].ulOamid =
//            pusTmp->arrucOamId[usLoopi];  /* 一次发送1k个数据 */
    }

    /* 将ID内容写入FPGA */
    //spm_oam_id_fpgareg(usSlotid, TRUE);
#endif

    iRet = SUCCESS;

EXIT_LABEL:
    return iRet;
}

/****************************************************************************
 *                                                                          *
 * Function    : spm_oam_init_fpga_for_oamid                                *
 *                                                                          *
 * Description : This function initializes FPGA registers in CCUT1.         *
 *                                                                          *
 * Input       :                                                            *
 *                                                                          *
 * Output      :                                                            *
 *                                                                          *
 * Return      :                                                            *
 *                                                                          *
 * Called By   : hardwinit() in                                             *
 *               /dev/linux/dev_cr8000/drv/linecard/ccut1/srcUsrFunction.c  *
 *                                                                          *
 * Others      : 64k_alarm_inband                                           *
 *               DDR3(base:0x1000000): All 0x0007ffff                       *
 *               ETH FRAME(base:0xa00000, offset:0x00002): enable           *
 *                                                                          *
 ****************************************************************************/
NBB_VOID spm_oam_init_fpga_for_oamid()
{
#if 0
    NBB_ULONG i = 0;
    NBB_ULONG base_addr = 0;

    printf("\nstart to initialize FPGA for OAMID\n");

    base_addr = SRC_DDR3_BASEADDR;

    for (i = 0; i < 68; i++)
    {
        bfdwritefpga32b(base_addr, 1, 0x0007ffff, 0, 1024);
        printf(".");

        base_addr += 1024;
    }

    printf("\nfinish initializing FPGA for OAMID\n");

    /* 64k_alarm_inband: enable sending packets */
    hw_write_fpga32b(0xa00002, 1);
#endif
}

/****************************************************************************
 *                                                                          *
 * Function    : spm_oam_init_oam_id_conf_db                                *
 *                                                                          *
 * Description : This function initializes data structure of OAM ID info.   *
 *                                                                          *
 * Input       :                                                            *
 *                                                                          *
 * Output      :                                                            *
 *                                                                          *
 * Return      :                                                            *
 *                                                                          *
 * Called By   : spm_oam_init()                                             *
 *                                                                          *
 * Others      : 64k_alarm_inband                                           *
 *                                                                          *
 ****************************************************************************/
NBB_VOID spm_oam_init_oam_id_conf_db()
{
    OS_MEMSET(g_stoam_idstore, 0, sizeof(g_stoam_idstore));
}

/****************************************************************************
 *                                                                          *
 * Function    : spm_oam_print_oam_id_conf_db                               *
 *                                                                          *
 * Description : This function prints the content of data structure of      *
 *               OAM ID info.                                               *
 *                                                                          *
 * Input       :                                                            *
 *                                                                          *
 * Output      :                                                            *
 *                                                                          *
 * Return      :                                                            *
 *                                                                          *
 * Others      : 64k_alarm_inband                                           *
 *                                                                          *
 ****************************************************************************/
NBB_VOID spm_oam_print_oam_id_conf_db()
{
    NBB_BYTE    slot_i  = 0;
    NBB_USHORT  index_i = 0;
    NBB_BYTE    slot_oam_flag = 0;

    for (slot_i = 0; slot_i < SUPPORT_SLOT_NUM; slot_i++)
    {
        slot_oam_flag = 0;

        for (index_i = 0; index_i < OAM_MAX_NUM_ACTIVE_SPU; index_i++)
        {
            if (1 == g_stoam_idstore[slot_i].stOamIdInfo[index_i].m_enable_flag)
            {
                if (0 == slot_oam_flag)
                {
                    printf("\n============================ SLOT %d ============================\n",
                           slot_i + 1);
                    slot_oam_flag = 1;
                }

                printf(" INDEX:%-4u OAM_ID:%-6u FPGA_ADDR:0x%08x VALUE:0x%08x\n",
                       index_i, g_stoam_idstore[slot_i].stOamIdInfo[index_i].m_oam_id,
                       0x1000000 + (g_stoam_idstore[slot_i].stOamIdInfo[index_i].m_oam_id - 1),
                       slot_i * 4096 + index_i);
            }
        }
    }
}

/*****************************************************************************
 函 数 名  : spm_oam_add_to_array
 功能描述  : 将配置存入本地数组
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月12日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_to_array(SPM_OAM_CB *poamcfg)
{
    NBB_INT iRet = ERROR; /* 表示添加成功或失败的变量 */
    NBB_ULONG ulOamId = 0;
    NBB_USHORT iloop = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    ulOamId = poamcfg->oam_id;

    /* 将树的配置放入本地数组，方便发送报文 */
    iloop = poamcfg->oamindex;
    g_stoam_local[iloop].global_index = poamcfg->oamindex;

    g_stoam_local[iloop].module_index = poamcfg->module_index;
    g_stoam_local[iloop].aps_enable = poamcfg->aps_enable;
    g_stoam_local[iloop].csf_enable = poamcfg->csf_enable;
    g_stoam_local[iloop].cv_cycle = poamcfg->cv_cycle;
    g_stoam_local[iloop].cv_enable = poamcfg->cv_enable;
    g_stoam_local[iloop].fdi_enable = poamcfg->fdi_enable;

    g_stoam_local[iloop].oam_type = poamcfg->oamtype;
    g_stoam_local[iloop].chip_id = poamcfg->chip_id;
    g_stoam_local[iloop].vp_attrib = poamcfg->pBasiCfg->vp_attrib;
    NBB_MEMCPY(g_stoam_local[iloop].meg_icc, poamcfg->pBasiCfg->meg_icc, 6 * sizeof(NBB_BYTE));
    NBB_MEMCPY(g_stoam_local[iloop].meg_umc, poamcfg->pBasiCfg->meg_umc, 6 * sizeof(NBB_BYTE));
    g_stoam_local[iloop].dst_mep_id = poamcfg->pBasiCfg->dst_mep_id;
    g_stoam_local[iloop].src_mep_id = poamcfg->pBasiCfg->src_mep_id;
    g_stoam_local[iloop].mel = poamcfg->pBasiCfg->mel;
    g_stoam_local[iloop].lsp_exp = poamcfg->pBasiCfg->lsp_exp;
    g_stoam_local[iloop].pw_exp = poamcfg->pBasiCfg->pw_exp;

    g_stoam_local[iloop].ulActiveOamkey = poamcfg->oam_id;
    g_stoam_local[iloop].ulTunnelLabel_tx = poamcfg->ulTunnelLabel_tx;
    g_stoam_local[iloop].ulPwLable_tx = poamcfg->ulPwLable_tx;
    g_stoam_local[iloop].ulTunnelLabel_rx = poamcfg->ulTunnelLabel_rx;
    g_stoam_local[iloop].ulPwLable_rx = poamcfg->ulPwLable_rx;

    g_stoam_local[iloop].vc_no_section1 = poamcfg->vc_no_section1;
    g_stoam_local[iloop].vc_no_section2 = poamcfg->vc_no_section2;
    g_stoam_local[iloop].vc_PortId_section1 = poamcfg->vc_PortId_section1;
    g_stoam_local[iloop].vc_PortId_section2 = poamcfg->vc_PortId_section2;
    g_stoam_local[iloop].uni_slot = poamcfg->uni_slot;
    g_stoam_local[iloop].nni_slot = poamcfg->nni_slot;
    g_stoam_local[iloop].uni_port_type = poamcfg->uni_port_type;
    g_stoam_local[iloop].vpn_type = poamcfg->vpn_type;
    g_stoam_local[iloop].vp_id = poamcfg->vp_id;

    OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
               "add vpwc oam success!\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__, ulOamId);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    spm_oam_send_packedge(poamcfg);
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_add
 功能描述  : oam添加配置
 输入参数  : NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : 0 添加成功
             1 添加失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月1日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_tree(SPM_OAM_CB *poamcfg)
{
    NBB_INT iRet = ERROR; /* 表示添加成功或失败的变量 */
    SPM_OAMTREE_NODE stOamTreeNode;
    NBB_ULONG ulOamId = 0;
    SPM_PW_KEY stpw_key;
    SPM_OAM_PW_CB *p_pw = NULL;
    NBB_USHORT iloop = 0;
    OS_MEMSET(&stpw_key, 0, sizeof(SPM_PW_KEY));
    OS_MEMSET(&stOamTreeNode, 0, sizeof(SPM_OAMTREE_NODE));

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    ulOamId = poamcfg->oam_id;
    iloop = poamcfg->oamindex;
    
    /* 节点属性为MEP时，发主动OAM；为MIP时发AIS和LB */
    /* 建树，先将第一个模块的2K条目建满，才能建第二个模块,主控上只有一个2k模块 */

    stOamTreeNode.usLspNo = poamcfg->oamindex;
    stOamTreeNode.ulTunnelId = poamcfg->ulTunnelLabel_rx;
    stOamTreeNode.ulPwId = poamcfg->ulPwLable_rx;
    iRet = spm_oam_tree_build(&stOamTreeNode, OAM_REG_BASE_ADDRESS_CCU, 0x0, 0);

    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n build tree in fpga failed oamid=%d ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId, iRet);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
               "build tree in fpga succeed\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__, ulOamId);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_send_packedge
 功能描述  : cv帧发送配置
 输入参数  : NBB_USHORT usOamLoop
            OAM_ACTIVE_CFG_TYPE *pstOamActiveCfgType
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月6日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_send_packedge(IN SPM_OAM_CB *poamcfg)
{
    NBB_ULONG baseAddr = 0;
    NBB_USHORT usLoop = 0;

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    usLoop = poamcfg->oamindex;

    if (NULL != poamcfg->pCvcfg)
    {
        /* 判断cv帧是否使能 */
        spm_oam_tx_vpls_mac_fpga(usLoop, ACTIVE_OAM);

        /* TMS/TMP/TMC层CV&APS/CSF成帧MEG配置 */
        spm_oam_tx_megfpga(usLoop);

        /* 条目N的发送标签使能,填入tunnel和pw标签 */
        spm_oam_tx_enablefpga(usLoop, ACTIVE_OAM);
    }

    if (NULL != poamcfg->pCsfcfg)
    {
        spm_oam_csf(usLoop);
    }

    if (NULL != poamcfg->pApscfg)
    {
        spm_oam_tx_vpls_mac_fpga(usLoop, ACTIVE_OAM);

        /* TMS/TMP/TMC层CV&APS/CSF成帧MEG配置 */
        spm_oam_tx_megfpga(usLoop);

        /* 条目N的发送标签使能,填入tunnel和pw标签 */
        spm_oam_tx_enablefpga(usLoop, ACTIVE_OAM);
        spm_get_moudle_base_addr(usLoop, &baseAddr);
    }

    if (NULL != poamcfg->pFdicfg)
    {
        if (0 == g_stoam_local[usLoop].vp_attrib)
        {
            /* 判断fdi是否是MIP结点 */
            spm_oam_tx_vpls_mac_fpga(usLoop, FDI_OAM);
        }
    }

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_tx_vpls_mac_fpga
 功能描述  : vpls OAM SRC盘MAC配置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月28日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_tx_vpls_mac_fpga(IN NBB_USHORT usLoop,
                                  IN NBB_BYTE ActOrOdType NBB_CCXT_T NBB_CXT)
{
    NBB_BYTE ucLocalSlot;/* 本盘槽位号 */
    CV_CFG_VPLS_HEAD_STRU stVplsHead;
    CV_CFG_VPLS_SLOT_HEAD_STRU stVplsSlotHead;
    ATG_DCI_VC_KEY stVcKey;
    NBB_USHORT usVplsSlotDa;
    NBB_INT iRet = ERROR;
    NBB_ULONG ulVplsHeadDa;
    NBB_BYTE ucSlot;/* 迭代得出的slot */
    NBB_ULONG ulPortIndex; /* 迭代得出的portindex */
    NBB_ULONG ulLspid;
    NBB_USHORT usLocalIndex = 0;
    NBB_USHORT frameHead = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;

    OS_MEMSET(&stVplsHead, 0, sizeof(CV_CFG_VPLS_HEAD_STRU));
    OS_MEMSET(&stVplsSlotHead, 0, sizeof(CV_CFG_VPLS_SLOT_HEAD_STRU));
    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));
    OS_SPRINTF(g_uc_message, "%s %s,%d\nframe type=%d\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, ActOrOdType);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (usLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1))
    {
        goto EXIT_LABEL;
    }

    usLocalIndex = g_stoam_local[usLoop].module_index;

    /* 发帧头类型配置 */
    frameHead = spm_oam_16read(OAM_REG_BASE_ADDRESS_CCU, CV_FRAME_HEAD + (usLocalIndex >> 2));
    frameHead &= ~(0xf << (3 - usLocalIndex % 4) * 4);
    spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, CV_FRAME_HEAD + (usLocalIndex >> 2), frameHead);
    spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, FDI_FRAME_HEAD + (usLocalIndex >> 2), frameHead);

    if (ActOrOdType !=  ACTIVE_OAM)
    {
        iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

        if (SUCCESS != iRet)
        {
            goto EXIT_LABEL;
        }

        ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
    }

    stVplsHead.FwDp = 0;
    stVplsHead.PphType = 0;
    stVplsHead.InMirrFlag = 0;
    stVplsHead.SnoopCmd = 0;
    stVplsHead.FwdTraffiClass = 0; /* 优先级，默认为0 */
    stVplsHead.FWDestInfo = 0x18; /* 最后一位表示是否聚合，此处填0 */

    ucSlot = g_stoam_local[usLoop].uni_slot;
    stVplsHead.FWDestInfo = ucSlot * 256 + 101;

    /* 槽位字段赋值 */
    stVplsSlotHead.Backup = 0;
    stVplsSlotHead.Dtype = 4; /* 代表vpls vc层oam帧 */
    stVplsSlotHead.PktT = 0;
    ulVplsHeadDa = *(NBB_ULONG *)(&stVplsHead);
    usVplsSlotDa = *(NBB_USHORT *)(&stVplsSlotHead);

    switch (ActOrOdType)
    {
        case ACTIVE_OAM:
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, CV_TX_DMAC_OFFSET1 + usLocalIndex,
                            (ulVplsHeadDa >> 16) & 0xffff);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, CV_TX_DMAC_OFFSET1 + usLocalIndex,
                            ulVplsHeadDa & 0xffff);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, CV_TX_DMAC_OFFSET1 + usLocalIndex,
                            g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, CV_TX_DMAC_OFFSET1 + usLocalIndex,
                            usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, CV_TX_DMAC_OFFSET1 + usLocalIndex,
                            0x0000);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, CV_TX_DMAC_OFFSET1 + usLocalIndex,
                            0x0000);
            break;

        case LB_OAM:
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1, ulVplsHeadDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            (ulVplsHeadDa >> 16) & 0xffff);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3, 
                g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4, usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5, 0x0000);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6, 0x0000);
            break;

        case LCK_OAM:
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                            ulVplsHeadDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            (ulVplsHeadDa >> 16) & 0xffff);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                            g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                            usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                            0x0000);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                            0x0000);
            break;

        case DMM_OAM:

            /* DMM发向 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
            DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            ulVplsHeadDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            (ulVplsHeadDa >> 16) & 0xffff);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x6,
                            usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x7,
                            0x0000);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x8,
                            0x0000);

            /* DMR发向 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
            DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x0,
                            ulVplsHeadDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x1,
                            (ulVplsHeadDa >> 16) & 0xffff);  /* IPADDRESS 需修改 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x2,
                            g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            0x0000);  /* IPADDRESS 需修改 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + DMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            0x0000);  /* IPADDRESS 需修改 */
            break;

        case LMM_OAM:

            /* LMM发向 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
            DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            ulVplsHeadDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            (ulVplsHeadDa >> 16) & 0xffff);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x6,
                            usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x7,
                            0x0000);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LM_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x8,
                            0x0000);

            /* LMR发向 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
            DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x0,
                            ulVplsHeadDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x1,
                            (ulVplsHeadDa >> 16) & 0xffff);  /* IPADDRESS 需修改 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x2,
                            g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x3,
                            usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x4,
                            0x0000);  /* IPADDRESS 需修改 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_MODULE_OFFSET + LMR_FRAME_HEAD_OFFSET + ucOdindex * 0x10 + 0x5,
                            0x0000);  /* IPADDRESS 需修改 */
            break;

        case TST_OAM:

            /* 填写帧头MAC内容 */
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
            DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x1,
                            ulVplsHeadDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x2,
                            (ulVplsHeadDa >> 16) & 0xffff);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x3,
                            g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x4,
                            usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x5,
                            0x0000);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, 
                DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + ucOdindex * 8 + 0x6,
                            0x0000);
            break;

        case FDI_OAM:
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU,  FDI_TX_DMAC_OFFSET1 + usLocalIndex,
                            ulVplsHeadDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU,  FDI_TX_DMAC_OFFSET2 + usLocalIndex,
                            (ulVplsHeadDa >> 16) & 0xffff);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU,  FDI_TX_DMAC_OFFSET3 + usLocalIndex,
                            g_stoam_local[usLoop].vc_no_section1);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU,  FDI_TX_SMAC_OFFSET1 + usLocalIndex,
                            usVplsSlotDa);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU,  FDI_TX_SMAC_OFFSET2 + usLocalIndex,
                            0x0000);
            spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU,  FDI_TX_SMAC_OFFSET3 + usLocalIndex,
                            0x0000);
            spm_oam_tx_enablefpga(usLoop, ACTIVE_OAM);
            break;

        default:
            goto EXIT_LABEL;
    }

    /* 对等MEP ID写入寄存器 */
    spm_oam_16write(OAM_REG_BASE_ADDRESS_CCU, CV_EXPECT_MEP_OFFSET + usLocalIndex, 
                    g_stoam_local[usLoop].dst_mep_id);
EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_delete
 功能描述  : 删除配置
 输入参数  : NBB_USHORT usLoop
 输出参数  : 无
 返 回 值  : NBB_INT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月4日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_delete(IN NBB_USHORT usLoop)
{
    NBB_INT iRet = ERROR; /* 表示添加成功或失败的变量 */
    SPM_OAMTREE_NODE stOamTreeNode;
    SPM_OAM_CB *p_oamcfg = NULL;
    NBB_ULONG ulOamId = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((usLoop == 0) || (usLoop >= OAM_MAX_NUM_ACTIVE_SPU))
    {
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stOamTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    ulOamId = g_stoam_local[usLoop].ulActiveOamkey;

    //为了统一 ，ccu盘的全局数组也建了4096个。
    stOamTreeNode.ulTunnelId = g_stoam_local[usLoop].ulTunnelLabel_rx; /* 待添加 */
    stOamTreeNode.ulPwId = g_stoam_local[usLoop].ulPwLable_rx; /* 待添加 */
    stOamTreeNode.usLspNo = g_stoam_local[usLoop].module_index;

    iRet = spm_oam_tree_delete(&stOamTreeNode, OAM_REG_BASE_ADDRESS_CCU,
                               g_stoam_local[usLoop].bModuleSwitch, 0);

    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "delete oam tree in the first module failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId, iRet);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* 清除相关寄存器内容 */
    spm_oam_delete_fpga(usLoop);

    /* 将本地存储配置的oamIndex置为0 */
    OS_MEMSET(&g_stoam_local[usLoop], 0, sizeof(OAM_ACTIVE_LOCAL_STRU));

    /* 将本地存储配置的ActiveOamkey置为0x0 */
    g_stoam_local[usLoop].ulActiveOamkey = USELESS;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
               "delete oam success!\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__, ulOamId);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_od_delete
 功能描述  : 删除按需配置寄存器
 输入参数  : IN NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月27日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_od_delete(IN NBB_BYTE ucOdLoop)
{
    NBB_INT iRet = ERROR;
    NBB_USHORT usOamLoopi;
    NBB_BYTE ucOdLoopi = 0;
    NBB_ULONG baseAddr;
    NBB_BYTE ucOdDmloop = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (ucOdLoop >= OAM_MAX_NUM_OD)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n ucOdLoop=%d\n"
                   "ucOdLoop is illegal\n"
                   "delete od oam failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ucOdLoop);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    usOamLoopi = g_stoam_odlocal[ucOdLoop].usOamLoop;
    ucOdLoopi = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;
    ucOdDmloop = g_stoam_odlocal[ucOdLoop].ucOdLoop;

    iRet = spm_delete_od_num(usOamLoopi, ucOdLoop);

    if (SUCCESS != iRet)
    {
        goto EXIT_LABEL;
    }

    /* 判断所在模块基地址 */
    spm_get_moudle_base_addr(usOamLoopi, &baseAddr);

    /* 删除寄存器内容 */

    /*lb*/
    spm_lb_delete_fpga(baseAddr, ucOdLoopi);

    /* lck */
    spm_lck_delete_fpga(baseAddr, ucOdLoopi);

    /* lm */
    spm_lm_delete_fpga(baseAddr, ucOdLoopi);


    /* dm */
    spm_dm_delete_fpga(baseAddr, ucOdLoopi);

    /* tst */
    spm_tst_delete_fpga(baseAddr, ucOdLoopi);

    spm_mep_meg_delete_fpga(baseAddr, ucOdLoop);

    spm_rx_label_delete_fpga(baseAddr, ucOdLoop);

    spm_memfree_od_oam_cfg(&g_stoam_odlocal[ucOdLoop]);

    OS_MEMSET(&g_stoam_odlocal[ucOdLoop], 0, sizeof(OAM_ON_DEMAND_LOCAL_STRU));

    OS_SPRINTF(g_uc_message, "%s %s,%d\n"
               "delete od oam success!\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_od_lb
 功能描述  : 按需LB帧头信息字节寄存器配置
 输入参数  : NBB_BYTE usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月1日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lb(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usPortId = 0;/* 迭代出的端口号 */
    OD_OAM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdLbFrame;
    NBB_ULONG ulLspLabel;
    NBB_ULONG ulPwLabel;
    NBB_ULONG baseAddr;
    NBB_BYTE odLoop = 0;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1))
        || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    if ((NULL == g_stoam_odlocal[ucOdLoop].pBasicData)
        || (NULL == g_stoam_odlocal[ucOdLoop].pLoopFrame))
    {
        goto EXIT_LABEL;
    }

    stOdFrame.Port = usPortId;
    stOdFrame.Mel = g_stoam_odlocal[ucOdLoop].pBasicData->mel;
    stOdFrame.LbMode = g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.lb_mode;
    stOdFrame.VpAttribute = ~(g_stoam_local[usOamLoop].vp_attrib);
    stOdFrame.SendCycle = g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.send_cycle;
    stOdFrame.SendDataType = g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.test_tlv_type;

    stOdFrame.SendEnable = g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.send_enable;

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    usOdLbFrame = *(NBB_USHORT *)(&stOdFrame);
    spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET
                    + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0,
                    usOdLbFrame NBB_CCXT_T NBB_CCXT);

    spm_oam_tx_vpls_mac_fpga(usOamLoop, LB_OAM);

    /* LB发送数据字节数最小值25,最大值1396 */
    if ((25 > g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length) ||
        (1396 < g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length))
    {
        g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length = 25;
    }

    spm_oam_16write(baseAddr,
                    DEMAND_OAM_MODULE_OFFSET + LB_FRAME_HEAD_OFFSET + odLoop * 8 + 0x7,
                    (g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length) NBB_CCXT_T NBB_CCXT);
    spm_od_tmpls_lable_fpga(ucOdLoop, LB_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LB_OAM);
    spm_od_send_data_fpga(usOamLoop, LB_OAM);
    spm_od_mep_meg_fpga(usOamLoop, ucOdLoop);

    /* 当配置为离线测试时，主动发lck帧 */
    if (TRUE == g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.test_method)
    {
        spm_od_lck(usOamLoop, ucOdLoop);
    }

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lck
 功能描述  : lck帧发送
 输入参数  : IN NBB_USHORT usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月2日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lck(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop NBB_CCXT_T NBB_CXT)
{
    OD_OAM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdLckFrame;
    NBB_ULONG ulLspLabel;
    NBB_ULONG ulPwLabel;
    NBB_ULONG baseAddr;
    NBB_BYTE odLoop = 0;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1))
         || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    if (NULL == g_stoam_odlocal[ucOdLoop].pBasicData)
    {
        goto EXIT_LABEL;
    }

    baseAddr = OAM_REG_BASE_ADDRESS_CCU;

    /* LCK发向 */
    stOdFrame.Mel = g_stoam_odlocal[ucOdLoop].pBasicData->mel;
    stOdFrame.SendCycle = 4;/* 默认值为“4”（即发送周期为1秒），还可设置成
                               “6”（即发送周期为1分钟） */

    if (NULL == g_stoam_odlocal[ucOdLoop].pLckFrame)
    {
        stOdFrame.SendEnable = 1;
    }
    else
    {
        stOdFrame.SendEnable = g_stoam_odlocal[ucOdLoop].pLckFrame->lck_frame_send_enable;
    }

    usOdLckFrame = *(NBB_USHORT *)(&stOdFrame);
    spm_oam_16write(baseAddr,
                    DEMAND_OAM_MODULE_OFFSET + LCK_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0,
                    usOdLckFrame NBB_CCXT_T NBB_CCXT);

    if (VC_OAM == g_stoam_local[usOamLoop].oam_type)
    {
        spm_oam_tx_vpls_mac_fpga(usOamLoop, LCK_OAM);
    }

    spm_od_tmpls_lable_fpga(ucOdLoop, LCK_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LCK_OAM);

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_dm
 功能描述  : 按需DM帧收发
 输入参数  : IN NBB_USHORT usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_dm(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop NBB_CCXT_T NBB_CXT)
{
    //OD_OAM_BYTE1_STRU stOdFrame;
    OD_OAM_DM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdDmFrame;
    NBB_ULONG ulLspLabel;
    NBB_ULONG ulPwLabel;
    NBB_ULONG baseAddr;
    NBB_BYTE odLoop = 0;
    NBB_BYTE ulRtn = ERROR;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1))
        || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    if ((NULL == g_stoam_odlocal[ucOdLoop].pBasicData)
        || (NULL == g_stoam_odlocal[ucOdLoop].pDmFrame))
    {
        goto EXIT_LABEL;
    }

    baseAddr = OAM_REG_BASE_ADDRESS_CCU;


    odLoop = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;

    #if 0
    if (ulRtn != SUCCESS)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n get dm index error\n"
                   "rcv od oam failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }
    #endif

    /* 发向 */
    stOdFrame.Mel = g_stoam_odlocal[ucOdLoop].pBasicData->mel;
    stOdFrame.vlan = 0;
    stOdFrame.pad = 0;
    stOdFrame.SendCycle = g_stoam_odlocal[ucOdLoop].pDmFrame->byte1.send_cycle;

    /* 周期 */
    if (FALSE == g_stoam_odlocal[ucOdLoop].pDmFrame->byte1.send_cycle)
    {
        stOdFrame.SendCycle = one_s;
    }
    else
    {
        stOdFrame.SendCycle = hundred_ms;
    }

    stOdFrame.SendEnable = g_stoam_odlocal[ucOdLoop].pDmFrame->byte1.send_enable;
    usOdDmFrame = *(NBB_USHORT *)(&stOdFrame);

    /* dm发送数据字节数最小值为4，最大值为1452 */
    if ((4 > g_stoam_odlocal[ucOdLoop].pDmFrame->frame_length) ||
        1452 < g_stoam_odlocal[ucOdLoop].pDmFrame->frame_length)
    {
        g_stoam_odlocal[ucOdLoop].pDmFrame->frame_length = 4;
    }

    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x0,
                    usOdDmFrame NBB_CCXT_T NBB_CCXT);

    //保留
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x1,
                    0x0000);

    //长度
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0x2,
                    (g_stoam_odlocal[ucOdLoop].pDmFrame->frame_length));

    if (VC_OAM == g_stoam_local[usOamLoop].oam_type)
    {
        spm_oam_tx_vpls_mac_fpga(usOamLoop, DMM_OAM);
    }

    spm_od_tmpls_lable_fpga(ucOdLoop, DMM_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, DMM_OAM);
    spm_od_send_data_fpga(ucOdLoop, DMM_OAM);

    //DMM保留
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0xd,
                    0x0000);
    spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_FRAME_HEAD_OFFSET + odLoop * 0x10 + 0xf,
                    0x0000);

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_tst
 功能描述  : TST帧收发
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月3日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_tst(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop NBB_CCXT_T NBB_CXT)
{
    OD_OAM_BYTE1_STRU stOdFrame;
    NBB_USHORT usOdTstFrame;
    NBB_ULONG ulLspLabel;
    NBB_ULONG ulPwLabel;
    NBB_ULONG baseAddr;
    NBB_BYTE odLoop = 0;
    OS_MEMSET(&stOdFrame, 0, sizeof(OD_OAM_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1))
        || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        goto EXIT_LABEL;
    }

    if (NULL == g_stoam_odlocal[ucOdLoop].pTestFrame)
    {
        goto EXIT_LABEL;
    }

    baseAddr = OAM_REG_BASE_ADDRESS_CCU;

    /* 发向 */
    stOdFrame.SendCycle = g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.send_cycle;
    stOdFrame.SendDataType = g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.test_tlv_type;
    stOdFrame.SendEnable = g_stoam_odlocal[ucOdLoop].pTestFrame->byte1.send_enable;
    stOdFrame.Mel = g_stoam_local[usOamLoop].mel;
    usOdTstFrame = *(NBB_USHORT *)(&stOdFrame);
    spm_oam_16write(baseAddr,
                    DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x0,
                    usOdTstFrame NBB_CCXT_T NBB_CCXT);

    if (VC_OAM == g_stoam_local[usOamLoop].oam_type)
    {
        spm_oam_tx_vpls_mac_fpga(usOamLoop, TST_OAM);
    }

    /* tst发送数据字节数最小值为25，最大值为1480 */
    if ((25 > g_stoam_odlocal[ucOdLoop].pTestFrame->tlv_length) ||
        1480 < g_stoam_odlocal[ucOdLoop].pTestFrame->tlv_length)
    {
        g_stoam_odlocal[ucOdLoop].pTestFrame->tlv_length = 25;
    }

    spm_oam_16write(baseAddr,
                    DEMAND_OAM_MODULE_OFFSET + TST_FRAME_HEAD_OFFSET + odLoop * 8 + 0x7,
                    (g_stoam_odlocal[ucOdLoop].pTestFrame->tlv_length) NBB_CCXT_T NBB_CCXT);
    spm_od_tmpls_lable_fpga(ucOdLoop, TST_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, TST_OAM);

    /* 当TST使能时，主动发lck帧 */
    spm_od_lck(usOamLoop, ucOdLoop);

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lbr_dmr
 功能描述  : lbr和dmr写入寄存器
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月18日
    作    者   : hx
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lbr_dmr(IN NBB_USHORT usOamLoop, NBB_BYTE ucOdLoop)
{
    NBB_ULONG baseAddr = 0;
    NBB_BYTE odLoop = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1))
        || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        return;
    }

    /* lb/dm不使能，FPGA收到lbm/dmm后也会回lbr/dmr */
    spm_oam_tx_vpls_mac_fpga(usOamLoop, LB_OAM);

    spm_oam_tx_vpls_mac_fpga(usOamLoop, DMR_OAM);

    spm_od_tmpls_lable_fpga(ucOdLoop, DMR_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, DMR_OAM);

    spm_oam_tx_vpls_mac_fpga(usOamLoop, LMR_OAM);

    spm_od_tmpls_lable_fpga(ucOdLoop, LMR_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LMR_OAM);

    spm_od_tmpls_lable_fpga(ucOdLoop, LB_OAM);
    spm_od_gal_lable_fpga(ucOdLoop, LB_OAM);
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_csf
 功能描述  : csf帧寄存器配置
 输入参数  : IN NBB_USHORT usOamIndex
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月20日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_csf(IN NBB_USHORT usOamLoop)
{
    NBB_USHORT us_uni_port_id = 0;
    NBB_USHORT us_uni_sub_id = 0;
    NBB_ULONG baseAddr;
    NBB_USHORT usLocalIndex = 0; //模块本地索引
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1))
    {
        goto EXIT_LABEL;
    }

    baseAddr = OAM_REG_BASE_ADDRESS_CCU;
    usLocalIndex = g_stoam_local[usOamLoop].module_index;

    /* 只vc层有csf */
    spm_oam_tx_vpls_mac_fpga(usOamLoop, ACTIVE_OAM);

    /* 条目N的发送标签使能,填入tunnel和pw标签 */
    spm_oam_tx_enablefpga(usOamLoop, ACTIVE_OAM);

    /* TMS/TMP/TMC层CV&APS/CSF成帧MEG配置 */
    spm_oam_tx_megfpga(usOamLoop);

    us_uni_port_id = g_stoam_local[usOamLoop].vc_PortId_section1 - 1;
    spm_oam_16write(baseAddr, CV_TXENABLE_OFFSET + usLocalIndex * 8 + 7,
                    us_uni_port_id NBB_CCXT_T NBB_CCXT);

    /* 对等MEP ID写入寄存器 */
    spm_oam_16write(baseAddr, CV_EXPECT_MEP_OFFSET + usLocalIndex, g_stoam_local[usOamLoop].dst_mep_id);
EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_ccu_32read
 功能描述  : OAM读FPGA
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_USHORT
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年11月29日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_USHORT spm_ccu_32read(NBB_ULONG base_addr,
                          NBB_ULONG offset_addr
                          NBB_CCXT_T NBB_CCXT)
{
    NBB_INT fd;
    NBB_UINT mmap_baseaddr;
    NBB_ULONG ul_data;
    NBB_USHORT us_data = 0;
#if 0
    fd = OpenPcieFpga();
    mmap_baseaddr = PcieFpgaMmap(fd);
    ReadPcieFpga_base(fd, base_addr + offset_addr, &ul_data, mmap_baseaddr);
    PcieFpgaMunmap(fd, mmap_baseaddr);
    ClosePcieFpga(fd);
    us_data = ((ul_data >> 16) & 0xFFFF);
#endif
    return us_data;
}

/*****************************************************************************
 函 数 名  : spm_ccu_32write
 功能描述  : OAM写FPGA
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年11月29日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_ccu_32write(NBB_ULONG base_addr,
                         NBB_ULONG offset_addr,
                         NBB_USHORT data
                         NBB_CCXT_T NBB_CCXT)
{
#if 0
    NBB_INT fd;
    NBB_UINT mmap_baseaddr;
    NBB_ULONG uldata;
    uldata = data;
    fd = OpenPcieFpga();
    mmap_baseaddr = PcieFpgaMmap(fd);
    WritePcieFpga_base(fd, base_addr + offset_addr, ((uldata << 16) & 0xFFFF0000), mmap_baseaddr);
    PcieFpgaMunmap(fd, mmap_baseaddr);
    ClosePcieFpga(fd);
#endif
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_fpga_write_32bits
 功能描述  : a
 输入参数  : NBB_ULONG base_addr    
             NBB_ULONG offset_addr  
             NBB_ULONG data         
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_fpga_write_32bits(NBB_ULONG base_addr, NBB_ULONG offset_addr,
                                   NBB_ULONG data)
{
#if 0
    NBB_INT fd;
    NBB_UINT mmap_baseaddr;

    fd = OpenPcieFpga();
    mmap_baseaddr = PcieFpgaMmap(fd);
    WritePcieFpga_base(fd, base_addr + offset_addr, data, mmap_baseaddr);
    PcieFpgaMunmap(fd, mmap_baseaddr);
    ClosePcieFpga(fd);
#endif
    return;
}

/*****************************************************************************
 函 数 名  : spm_set_mpls_flag
 功能描述  : a
 输入参数  : SPM_OAM_CB *pOamCfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_mpls_flag(SPM_OAM_CB *pOamCfg)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_set_tpoam_change
 功能描述  : a
 输入参数  : NBB_BYTE changeType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_tpoam_change(NBB_BYTE changeType)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_oam_wrapping_build_acl
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_wrapping_build_acl(IN NBB_USHORT usOamLoop)
{
    return 0;
}

#endif /* SRC */

/*****************************************************************************
 函 数 名  : spm_oam_get_local_oamindex
 功能描述  : 根据全局OAMID得出在全局数组的索引
 输入参数  : NBB_USHORT usOamCfgId

 输出参数  : NBB_USHORT *usLocalOamId
 返 回 值  : SUCCESS/ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月2日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_get_local_oamindex(IN NBB_ULONG ulOamID, OUT NBB_USHORT *usLocalOamLoop
                                   NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    SPM_OAM_CB *p_oamcfg = NULL;
    NBB_USHORT usLoop = 0;

//    printf("%s, oamid=%d\n", __FUNCTION__, ulOamID);

    /* 判断入参 */
    if ((ulOamID == 0) || (ulOamID > OAM_ID_MAX))
    {
        if (1 == g_alarm_debug)
        {
            printf("CES in spm_oam_get_local_oamindex  OAMID is error\n");
        }

        goto EXIT_LABEL;
    }

    p_oamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &ulOamID);

    if (NULL == p_oamcfg)
    {
        if (1 == g_alarm_debug)
        {
            printf("CES in spm_oam_get_local_oamindex  p_oamcfg is NULL\n");
        }

        goto EXIT_LABEL;
    }

    usLoop = p_oamcfg->oamindex;

    if (ulOamID == g_stoam_local[usLoop].ulActiveOamkey)
    {
        *usLocalOamLoop = usLoop;
        iRet = SUCCESS;
    }

//    printf("usLocalOamLoop=%d\n", *usLocalOamLoop);

EXIT_LABEL:
    return iRet;
}

/****************************************************************************
 *                                                                          *
 * Function    : spm_oam_print_oam_id_conf_db                               *
 *                                                                          *
 * Description : This function handles OAM ID sync messages before TX.      *
 *                                                                          *
 * Input       :                                                            *
 *                                                                          *
 * Output      : ulMslength - The length of message.                        *
 *                                                                          *
 * Return      : The pointer to message.                                    *
 *                                                                          *
 * Others      : 64k_alarm_inband                                           *
 *                                                                          *
 ****************************************************************************/
NBB_BYTE* spm_oam_get_syn_oam_id_data(OUT NBB_ULONG *ulMslength)
{
    //SPM_OAM_SYN_OAM_ID *pTmp  = NULL;
    NBB_BYTE *p_data = NULL;
    NBB_ULONG data_len = 0;
    NBB_USHORT usLoopi = 0;

    /* 组包的内容 */
    //pTmp = (SPM_OAM_SYN_OAM_ID *)g_pucoam_synidinfo; //TODO:改成支持OAM
    p_data = (NBB_BYTE *)g_pucoam_synidinfo;

    /* 组帧 */
    for (usLoopi = 0; usLoopi < OAM_MAX_NUM_ACTIVE_SPU; usLoopi++)
    {
        //pTmp->arrucOamId[usLoopi] = (NBB_USHORT)g_stoam_local[usLoopi].ulActiveOamkey;
        if (g_oam_id_conf[usLoopi].m_enable_flag != 0xff)
        {
            //SPM_L3_WR_U16(usLoopi, p_data);
            //SPM_L3_WR_U32(g_oam_id_conf[usLoopi].m_oam_id, p_data);
            //SPM_L3_WR_U8(g_oam_id_conf[usLoopi].m_enable_flag, p_data);
        }
    }

    //*ulMslength = g_uloam_synidinfolth;
    data_len = p_data - (NBB_BYTE *)g_pucoam_synidinfo;
    *ulMslength = data_len;

    return (NBB_BYTE *)g_pucoam_synidinfo;
}

/****************************************************************************
 *                                                                          *
 * Function    : spm_oam_init_oam_id_conf                                   *
 *                                                                          *
 * Description : This function initializes data structure of OAM ID info.   *
 *                                                                          *
 * Input       :                                                            *
 *                                                                          *
 * Output      :                                                            *
 *                                                                          *
 * Return      :                                                            *
 *                                                                          *
 * Others      : 64k_alarm_inband                                           *
 *                                                                          *
 ****************************************************************************/
NBB_VOID spm_oam_init_oam_id_conf()
{
    NBB_USHORT usLoopi = 0;

    for (usLoopi = 0; usLoopi < OAM_MAX_NUM_ACTIVE_SPU; usLoopi++)
    {
        g_oam_id_conf[usLoopi].m_enable_flag = 0xff;
        g_oam_id_conf[usLoopi].m_oam_id = 0;
    }
}

/****************************************************************************
 *                                                                          *
 * Function    : spm_oam_set_oam_id_conf                                    *
 *                                                                          *
 * Description : This function sets the mapping table of OAM ID in linecard *
 *               and notify the CCUT1.                                      *
 *                                                                          *
 * Input       : oam_id     - The global OAM ID.                            *
 *               oam_index  - The local OAM index.                          *
 *               en         - Enable or disable the mapping.                *
 *                                                                          *
 * Output      :                                                            *
 *                                                                          *
 * Return      :                                                            *
 *                                                                          *
 * Called By   : spm_rcv_dci_set_active_oam() : set or delete the mapping   *
 *               of global OAM ID and local OAM index.                      *
 *                                                                          *
 * Others      : 64k_alarm_inband                                           *
 *                                                                          *
 ****************************************************************************/
NBB_INT spm_oam_set_oam_id_conf(NBB_ULONG oam_id, NBB_USHORT oam_index,
                                NBB_BYTE en)
{
    NBB_BYTE *p_buf = NULL;

#if 0
    if (oam_id < 1 || oam_id > OAM_MAX_ID)
    {
        printf("OAM ID(%d) is out of range from 1 to 65536.\n", oam_id);
        return -1;
    }
#endif

    if (oam_index >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        printf("OAM index(%d) is larger than %d.\n", oam_index,
               OAM_MAX_NUM_ACTIVE_SPU);
        return -1;
    }

    g_oam_id_conf[oam_index].m_oam_id = oam_id;
    g_oam_id_conf[oam_index].m_enable_flag = en;

    //spm_bfd_slot_tran_syn_need_send(g_oam_fd.syn_id_fd);

    return SUCCESS;
}

/****************************************************************************
 *                                                                          *
 * Function    : spm_oam_print_oam_id_conf                                  *
 *                                                                          *
 * Description : This function prints the content of data structure of      *
 *               OAM ID info.                                               *
 *                                                                          *
 * Input       :                                                            *
 *                                                                          *
 * Output      :                                                            *
 *                                                                          *
 * Return      :                                                            *
 *                                                                          *
 * Others      : 64k_alarm_inband                                           *
 *                                                                          *
 ****************************************************************************/
NBB_VOID spm_oam_print_oam_id_conf()
{
    NBB_USHORT index_i = 0;

    for (index_i = 0; index_i < OAM_MAX_NUM_ACTIVE_SPU; index_i++)
    {
        if (g_oam_id_conf[index_i].m_enable_flag != 0xff)
        {
            printf("INDEX:%-4u OAM_ID:%-6u ENABLE:%d\n",
                   index_i, g_oam_id_conf[index_i].m_oam_id,
                   g_oam_id_conf[index_i].m_enable_flag);
        }
    }
}

/*****************************************************************************
 函 数 名  : spm_od_mep_meg_fpga
 功能描述  : MEP/MEG数据寄存器定义
 输入参数  : IN NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_mep_meg_fpga(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT DesMepId;
    NBB_BYTE CarrierCode[6];
    NBB_BYTE MegIcc[6];
    NBB_BYTE MegUmc[6];
    NBB_BYTE ucLoop;
    NBB_USHORT tempCcode;
    NBB_USHORT tempMeg;
    NBB_ULONG NodeId;
    NBB_ULONG IfNum;
    NBB_USHORT CountryCode;
    NBB_ULONG baseAddr;
    NBB_BYTE ucLoopi = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    if ((ucOdLoop > (OAM_MAX_NUM_OD - 1)) || (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1)))
    {
        return;
    }

    if (NULL == g_stoam_odlocal[ucOdLoop].pLoopFrame)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oam_index=%d,od_index=%d\n"
                   "Mip cfg is empty\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, usOamLoop, ucOdLoop);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        return;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    ucLoopi = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;

    //MEP TLV/MIP TLV的可选TLV均带MEG ID
    for (ucLoop = 0 ; ucLoop < 6 ; ucLoop++)
    {
        MegIcc[ucLoop] = g_stoam_local[usOamLoop].meg_icc[ucLoop];
        MegUmc[ucLoop] = g_stoam_local[usOamLoop].meg_umc[ucLoop];
    }

    for (ucLoop = 0 ; ucLoop < 5 ; ucLoop = ucLoop + 2)
    {
        tempMeg = MegIcc[ucLoop];
        tempMeg = ((tempMeg << 8) & 0XFF00) + MegIcc[ucLoop + 1];
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0xa + (ucLoop / 2),
                        tempMeg NBB_CCXT_T NBB_CCXT);
    }
    for (ucLoop = 0 ; ucLoop < 5 ; ucLoop = ucLoop + 2)
    {
        tempMeg = MegUmc[ucLoop];
        tempMeg = ((tempMeg << 8) & 0XFF00) + MegUmc[ucLoop + 1];
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0xd + (ucLoop / 2),
                        tempMeg NBB_CCXT_T NBB_CCXT);
        
    }

    /* MIP TLV */
    if (TRUE == g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_mode)
    {
        if (NULL == g_stoam_odlocal[ucOdLoop].pMip)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\nod mip cfg is null\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            return;
        }

        DesMepId = 0;//对MIP环回才配MIP基本配置，MIP ID无用，配为0.目的MEP ID
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x0,
                        DesMepId NBB_CCXT_T NBB_CCXT);

        for (ucLoop = 0 ; ucLoop < 6 ; ucLoop++)
        {
            CarrierCode[ucLoop] = g_stoam_odlocal[ucOdLoop].pMip->mip_icc[ucLoop];
        }

        for (ucLoop = 0 ; ucLoop < 5; ucLoop = ucLoop + 2)
        {
            tempCcode = CarrierCode[ucLoop];
            tempCcode = ((tempCcode << 8) & 0xFF00) + CarrierCode[ucLoop + 1];
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x1 + (ucLoop / 2),
                            tempCcode NBB_CCXT_T NBB_CCXT);
        }

//        NodeId = g_stoam_odlocal[ucOdLoop].pMip->node_id;
        NodeId = 1;//对mip节点环回的时候，mip节点要匹配此字段，约定固定为1.
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x4,
                        ((NodeId >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x5,
                        (NodeId & 0xFFFF) NBB_CCXT_T NBB_CCXT);
        IfNum = g_stoam_odlocal[ucOdLoop].pMip->if_num;
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x6,
                        ((IfNum >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x7,
                        (IfNum & 0xFFFF) NBB_CCXT_T NBB_CCXT);
        CountryCode = g_stoam_odlocal[ucOdLoop].pMip->cc;
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x8,
                        CountryCode NBB_CCXT_T NBB_CCXT);
        
        //源MEP ID
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x9,
                        g_stoam_local[usOamLoop].src_mep_id NBB_CCXT_T NBB_CCXT);
        NodeId = g_stoam_odlocal[ucOdLoop].pMip->node_id;
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE
                        + SOURCE_NODE_ID_OFFSET + 0x2 * ucLoopi + 0x0,
                        ((NodeId >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE
                        + SOURCE_NODE_ID_OFFSET + 0x2 * ucLoopi + 0x1,
                        (NodeId & 0xFFFF) NBB_CCXT_T NBB_CCXT);
    }
    
    /* MEP TLV */
    else
    {
        if (TRUE == g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.send_enable)
        {
            DesMepId = g_stoam_odlocal[ucOdLoop].pLoopFrame->dest_mep_id;
        }
        else
        {
            DesMepId = g_stoam_local[usOamLoop].dst_mep_id;
        }

        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x0,
                        DesMepId NBB_CCXT_T NBB_CCXT);
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE + 16 * ucLoopi + 0x9,
                        g_stoam_local[usOamLoop].src_mep_id NBB_CCXT_T NBB_CCXT);
        
//        NodeId = g_stoam_odlocal[ucOdLoop].pMip->node_id;
        NodeId = 1;//无意义，固定为1
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE
            + SOURCE_NODE_ID_OFFSET + 0x2 * ucLoopi + 0x0,
            ((NodeId >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + MEP_MEG_BASE
            + SOURCE_NODE_ID_OFFSET + 0x2 * ucLoopi + 0x1,
            (NodeId & 0xFFFF) NBB_CCXT_T NBB_CCXT);
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_send_data_fpga
 功能描述  : 发送数据字节寄存器定义
 输入参数  : NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月2日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_send_data_fpga(IN NBB_BYTE ucOdLoop, IN NBB_BYTE ActOrOdType NBB_CCXT_T NBB_CXT)
{
    NBB_BYTE ucType;
    NBB_USHORT usLength;
    NBB_USHORT ustempdata;
    NBB_BYTE content = 0;
    NBB_USHORT ucLoop;
    NBB_USHORT usOamLoopi = 0;
    NBB_ULONG baseAddr;
    OS_SPRINTF(g_uc_message, "%s %s,%d\nframe type=%d\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, ActOrOdType);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (ucOdLoop > (OAM_MAX_NUM_OD - 1))
    {
        return;
    }

    usOamLoopi = g_stoam_odlocal[ucOdLoop].usOamLoop;
    
    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoopi, &baseAddr);

    if (NULL == g_stoam_odlocal[ucOdLoop].pLoopFrame)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\nod lb cfg is null\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        return;
    }

    /* 若是数据tlv */
    switch (ActOrOdType)
    {
        case LB_OAM:
            if (NULL != g_stoam_odlocal[ucOdLoop].pLoopFrame)
            {
                if (TRUE == g_stoam_odlocal[ucOdLoop].pLoopFrame->byte1.lb_mode)
                {
                    usLength = g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_length;
                    content = g_stoam_odlocal[ucOdLoop].pLoopFrame->tlv_test_content;

                    for (ucLoop = 0 ; ucLoop < ((usLength / 2) - 1) ; ucLoop++)
                    {
                        ustempdata = content;
                        ustempdata = ((ustempdata << 8) & 0xFF00) + ((content) & 0xFF);
                        spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_SEND_DATA_OFFSET + ucLoop,
                                        0x0000 NBB_CCXT_T NBB_CCXT);
                    }
                }
            }

            break;

        default:
            break;
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_od_gal_lable_fpga
 功能描述  : GAL字节寄存器定义
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月2日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_gal_lable_fpga(IN NBB_BYTE ucOdLoop, IN NBB_BYTE ActOrOdType NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG ulGalLable;
    NBB_ULONG baseAddr;
    NBB_USHORT usOamLoopi;
    NBB_BYTE ucOdLoopi = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\nframe type=%d\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, ActOrOdType);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (ucOdLoop > (OAM_MAX_NUM_OD - 1))
    {
        return;
    }

    usOamLoopi = g_stoam_odlocal[ucOdLoop].usOamLoop;
    ucOdLoopi = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;
    spm_get_od_gal(ucOdLoop);
    ulGalLable = g_stoam_odlocal[ucOdLoop].gal_label_fpga;

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoopi, &baseAddr);

    switch (ActOrOdType)
    {
        case LB_OAM:
            
            /* LB */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_GAL_OFFSET + ucOdLoopi * 2 + 0x0,
                            ((ulGalLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_GAL_OFFSET + ucOdLoopi * 2 + 0x1,
                            (ulGalLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case LCK_OAM:
            
            /* LCK */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_GAL_OFFSET + ucOdLoopi * 2 + 0x0,
                            ((ulGalLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_GAL_OFFSET + ucOdLoopi * 2 + 0x1,
                            (ulGalLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);

            break;

        case DMM_OAM:

            /* DM */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_GAL_OFFSET + ucOdLoopi * 0x10 + 0x0,
                            ((ulGalLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_GAL_OFFSET + ucOdLoopi * 0x10 + 0x1,
                            (ulGalLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case DMR_OAM:

            /*DMR*/
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_GAL_OFFSET + ucOdLoopi * 0x10 + 0x0,
                            ((ulGalLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_GAL_OFFSET + ucOdLoopi * 0x10 + 0x1,
                            (ulGalLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case LMM_OAM:

            /* LM */
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_GAL_OFFSET + ucOdLoopi * 0x10 + 0x0,
                            ((ulGalLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_GAL_OFFSET + ucOdLoopi * 0x10 + 0x1,
                            (ulGalLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case LMR_OAM:
            
            /*LMR*/
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_GAL_OFFSET + ucOdLoopi * 0x10 + 0x0,
                            ((ulGalLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_GAL_OFFSET + ucOdLoopi * 0x10 + 0x1,
                            (ulGalLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case TST_OAM:
            
            /* TST */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_GAL_OFFSET + ucOdLoopi * 2 + 0x0,
                            ((ulGalLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_GAL_OFFSET + ucOdLoopi * 2 + 0x1,
                            (ulGalLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        default:
            return;
    }
}

/*****************************************************************************
 函 数 名  : spm_od_tmpls_lable_fpga
 功能描述  : TMPLS标签查找表寄存器定义
 输入参数  : NBB_USHORT OamIndex
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月2日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_tmpls_lable_fpga(IN NBB_BYTE ucOdLoop, IN NBB_BYTE ActOrOdType NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG ulTmplsLable;
    NBB_USHORT usOamLoopi = 0;
    NBB_ULONG baseAddr;
    NBB_BYTE ucOdLoopi = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\nframe type=%d\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, ActOrOdType);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (ucOdLoop > (OAM_MAX_NUM_OD - 1))
    {
        return;
    }

    spm_get_od_tmpls(ucOdLoop);
    ulTmplsLable = g_stoam_odlocal[ucOdLoop].tmpls_label_fpga;
    usOamLoopi = g_stoam_odlocal[ucOdLoop].usOamLoop;
    ucOdLoopi = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;
    
    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoopi, &baseAddr);

    switch (ActOrOdType)
    {
        case LB_OAM:

            /* LB */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_LABLE_OFFSET + ucOdLoopi * 2 + 0x0,
                            ((ulTmplsLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LB_LABLE_OFFSET + ucOdLoopi * 2 + 0x1,
                            (ulTmplsLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);

            break;

        case LCK_OAM:

            /* LCK */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_LABLE_OFFSET + ucOdLoopi * 2 + 0x0,
                            ((ulTmplsLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + LCK_LABLE_OFFSET + ucOdLoopi * 2 + 0x1,
                            (ulTmplsLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);

            break;

        case DMM_OAM:

            /*DMM*/
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_LABLE_OFFSET + ucOdLoopi * 0x10 + 0x0,
                            ((ulTmplsLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DM_LABLE_OFFSET + ucOdLoopi * 0x10 + 0x1,
                            (ulTmplsLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case DMR_OAM:
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_LABLE_OFFSET + ucOdLoopi * 0x10 + 0x0,
                            ((ulTmplsLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + DMR_LABLE_OFFSET + ucOdLoopi * 0x10 + 0x1,
                            (ulTmplsLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case LMM_OAM:

            /*LMM*/
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_LABLE_OFFSET + ucOdLoopi * 0x10 + 0x0,
                            ((ulTmplsLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LM_LABLE_OFFSET + ucOdLoopi * 0x10 + 0x1,
                            (ulTmplsLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case LMR_OAM:
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_LABLE_OFFSET + ucOdLoopi * 0x10 + 0x0,
                            ((ulTmplsLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_MODULE_OFFSET + LMR_LABLE_OFFSET + ucOdLoopi * 0x10 + 0x1,
                            (ulTmplsLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            break;

        case TST_OAM:
            
            /* TST */
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_LABLE_OFFSET + ucOdLoopi * 2 + 0x0,
                            ((ulTmplsLable >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_OAM_MODULE_OFFSET + TST_LABLE_OFFSET + ucOdLoopi * 2 + 0x1,
                            (ulTmplsLable & 0xFFFF) NBB_CCXT_T NBB_CCXT);

            break;

        default:
            return;
    }
}

/*****************************************************************************
 函 数 名  : spm_od_lm_tx_fpga
 功能描述  : 按需lm帧成帧配置
 输入参数  : NBB_BYTE ucOamIndex
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月28日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lm_tx_fpga(IN NBB_BYTE ucOdLoop NBB_CCXT_T NBB_CXT)
{
    OD_LM_TX_BYTE1_STRU stLmTxByte1;
    NBB_USHORT usLmTxByte1;
    NBB_BYTE DMac[6];
    NBB_BYTE SMac[6];
    NBB_USHORT *pustempDMac;
    NBB_USHORT *pustempSMac;
    NBB_USHORT usOamLoopi = 0;
    NBB_ULONG baseAddr;
    NBB_BYTE ucLocalIndex;
    NBB_BYTE ucLoop = 0;
    OS_MEMSET(DMac, 0, 6 * sizeof(NBB_BYTE));
    OS_MEMSET(SMac, 0, 6 * sizeof(NBB_BYTE));
    OS_MEMSET(&stLmTxByte1, 0, sizeof(OD_LM_TX_BYTE1_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (ucOdLoop > (OAM_MAX_NUM_OD - 1))
    {
        return;
    }

    usOamLoopi = g_stoam_odlocal[ucOdLoop].usOamLoop;
    
    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoopi, &baseAddr);
    ucLocalIndex = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;
    stLmTxByte1.LmmSendcycle = g_stoam_odlocal[ucOdLoop].pLmFrame->byte1.send_cycle;
    stLmTxByte1.LmmSendEnable = g_stoam_odlocal[ucOdLoop].pLmFrame->byte1.send_enable;
    stLmTxByte1.Mel = 7;
    stLmTxByte1.PwLable = ((g_stoam_odlocal[ucOdLoop].ulPwLabel_tx >> 8) & 0xFF);
    usLmTxByte1 = *(NBB_USHORT *)(&stLmTxByte1);
    
    /* 迭代出目的mac和源mac */
    /****************************/
    pustempDMac = (NBB_USHORT *)(&DMac[0]);
    pustempSMac = (NBB_USHORT *)(&SMac[0]);

    for (ucLoop = 1 ; ucLoop <= 3 ; ucLoop++)
    {
        if ((NULL != pustempDMac) && (NULL != pustempSMac))
        {
            spm_oam_16write(baseAddr, DEMAND_LM_TX_CFG_OFFSET + ucLocalIndex * 10 + ucLoop, 
                (*pustempDMac) NBB_CCXT_T NBB_CCXT);
            spm_oam_16write(baseAddr, DEMAND_LM_TX_CFG_OFFSET + ucLocalIndex * 10 + ucLoop + 3, 
                (*pustempSMac) NBB_CCXT_T NBB_CCXT);
            pustempDMac++;
            pustempSMac++;
        }
    }

    spm_oam_16write(baseAddr, DEMAND_LM_TX_CFG_OFFSET + ucLocalIndex * 10,
                    usLmTxByte1 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_LM_TX_CFG_OFFSET + ucLocalIndex * 10 + 7,
                    ((g_stoam_odlocal[usOamLoopi].ulTunnelLabel_tx >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_LM_TX_CFG_OFFSET + ucLocalIndex * 10 + 8,
                    (g_stoam_odlocal[usOamLoopi].ulTunnelLabel_tx & 0xFFFF) NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_LM_TX_CFG_OFFSET + ucLocalIndex * 10 + 9,
                    ((g_stoam_odlocal[usOamLoopi].ulPwLabel_tx >> 16) & 0xFFFF) NBB_CCXT_T NBB_CCXT);
}

/*****************************************************************************
 函 数 名  : spm_od_lm_rx_lsp_fpga
 功能描述  : tmp/tmc层按需lm帧对cv收向的期望标签值配置/lm帧收向期望标签
 输入参数  : NBB_BYTE
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月22日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lm_rx_lsp_fpga(IN NBB_BYTE ucOdLoop NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG usLspLabel;/*迭代出的LSP标签*/
    NBB_ULONG usPwLabel;/*迭代出的pw标签*/
    OD_OAM_LSP_BYTE1_STRU tempLabel0;
    OD_OAM_LSP_BYTE2_STRU tempLabel1;
    OD_OAM_LSP_BYTE3_STRU tempLabel2;
    NBB_USHORT usTempLabel0 = 0;
    NBB_USHORT usTempLabel1 = 0;
    NBB_USHORT usTempLabel2 = 0;
    NBB_USHORT usActOamIndex;
    NBB_ULONG baseAddr;
    NBB_BYTE ucLocalIndex;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (ucOdLoop > (OAM_MAX_NUM_OD - 1))
    {
        return;
    }

    OS_MEMSET(&tempLabel0, 0, sizeof(OD_OAM_LSP_BYTE1_STRU));
    OS_MEMSET(&tempLabel1, 0, sizeof(OD_OAM_LSP_BYTE2_STRU));
    OS_MEMSET(&tempLabel2, 0, sizeof(OD_OAM_LSP_BYTE3_STRU));
    usActOamIndex = g_stoam_odlocal[ucOdLoop].usOamLoop;
    ucLocalIndex = g_stoam_odlocal[ucOdLoop].ucOdLoop - 1;
    usLspLabel = (NBB_USHORT)g_stoam_odlocal[ucOdLoop].ulTunnelLabel_rx;
    usPwLabel = (NBB_USHORT)g_stoam_odlocal[ucOdLoop].ulPwLabel_rx;
    
    /* 判断所在模块 */
    spm_get_moudle_base_addr(usActOamIndex, &baseAddr);
    
    /* 写寄存器 */
    tempLabel0.default0 = 0;
    tempLabel0.Lsp19to12 = ((usLspLabel >> 12) & 0XFF);
    tempLabel1.Lsp11to0 = (usLspLabel & 0XFFF);
    tempLabel1.Pw19to16 = ((usPwLabel >> 16) & 0XF);
    tempLabel2.Pw15to0 = (usPwLabel & 0XFFFF);
    usTempLabel0 = *(NBB_USHORT *)(&tempLabel0);
    usTempLabel1 = *(NBB_USHORT *)(&tempLabel1);
    usTempLabel2 = *(NBB_USHORT *)(&tempLabel2);
    spm_oam_16write(baseAddr, DEMAND_LM_RX_LSP_OFFSET + ucLocalIndex * 3 + 0x0, usTempLabel0 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_LM_RX_LSP_OFFSET + ucLocalIndex * 3 + 0x1, usTempLabel1 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_LM_RX_LSP_OFFSET + ucLocalIndex * 3 + 0x2, usTempLabel2 NBB_CCXT_T NBB_CCXT);
}

/*****************************************************************************
 函 数 名  : spm_oam_delete_fpga
 功能描述  : 删除配置，清除相关寄存器内容
 输入参数  : NBB_USHORT usOamLoop
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月19日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_delete_fpga(IN NBB_USHORT usOamLoop NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG baseAddr;
    NBB_USHORT usLocalIndex;
    NBB_INT i = 0;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1))
    {
        return;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    usLocalIndex = g_stoam_local[usOamLoop].module_index;
    NBB_BYTE fdi_enabel = 0;

    fdi_enabel = g_stoam_local[usOamLoop].fdi_enable;


    if (1 == fdi_enabel)
    {
        /* 清除帧头MAC内容 */
        spm_oam_16write(baseAddr,  FDI_TX_DMAC_OFFSET1 + usLocalIndex,
                        USELESS);
        spm_oam_16write(baseAddr,  FDI_TX_DMAC_OFFSET2 + usLocalIndex,
                        USELESS);
        spm_oam_16write(baseAddr,  FDI_TX_DMAC_OFFSET3 + usLocalIndex,
                        USELESS);
        spm_oam_16write(baseAddr,  FDI_TX_SMAC_OFFSET1 + usLocalIndex,
                        USELESS);
        spm_oam_16write(baseAddr,  FDI_TX_SMAC_OFFSET2 + usLocalIndex,
                        USELESS);
        spm_oam_16write(baseAddr,  FDI_TX_SMAC_OFFSET3 + usLocalIndex,
                        USELESS);
        for (i = 0 ; i < 8 ; i++)
        {
            /* 清除条目N的发送标签使能写寄存器 */
            spm_oam_16write(baseAddr, FDI_TX_CFG_OFFSET + usLocalIndex * 8 + i,
                USELESS);
        }
        
        /* 清除ACH寄存器 */
        //spm_oam_16write(baseAddr, OAM_GLOBAL_ACH_OFFSET, USELESS);
        spm_oam_16write(baseAddr, CV_EXPECT_MEP_OFFSET + usLocalIndex, USELESS);
    }
    else
    {
        /* 填写帧头内容 */
        spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET1 + usLocalIndex, USELESS);
        spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET2 + usLocalIndex, USELESS);
        spm_oam_16write(baseAddr, CV_TX_DMAC_OFFSET3 + usLocalIndex, USELESS);
        spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET1 + usLocalIndex, USELESS);
        spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET2 + usLocalIndex, USELESS);
        spm_oam_16write(baseAddr, CV_TX_SMAC_OFFSET3 + usLocalIndex, USELESS);
        
        /* 条目N的发送标签使能 */
        for (i = 0 ; i < 4 ; i++)
        {
            spm_oam_16write(baseAddr, CV_TXVLAN_LABEL_OFFSET + 
                usLocalIndex * 4 + i, USELESS);
        }
        spm_oam_16write(baseAddr, CV_TXENABLE_OFFSET + usLocalIndex, USELESS);

        /* 发送的mepid写入寄存器 */
        spm_oam_16write(baseAddr, CV_TXMEP_OFFSET + usLocalIndex, USELESS);
        
        /* tms/tmp/tmc层cv&asps/csf成帧MEG配置 */
        for (i = 0 ; i < 8; i++)
        {
            spm_oam_16write(baseAddr, CV_TXMEG_OFFSET + 
                usLocalIndex * 8 + i, USELESS);
        }

        /* 对等MEP ID写入寄存器 */
        spm_oam_16write(baseAddr, CV_EXPECT_MEP_OFFSET + usLocalIndex, USELESS);
    }
}

/*****************************************************************************
 函 数 名  : spm_od_rx_label_fpga
 功能描述  : 按需lb/test/dm/lck帧收向tmp/tmc/tms标签-lsp配置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月22日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_rx_label_fpga(IN NBB_BYTE ucLoop)
{
    OD_OAM_LSP_BYTE1_STRU tempLabel0;
    OD_OAM_LSP_BYTE2_STRU tempLabel1;
    OD_OAM_LSP_BYTE3_STRU tempLabel2;
    NBB_INT iRet = ERROR;
    NBB_USHORT usTempLabel0 = 0;
    NBB_USHORT usTempLabel1 = 0;
    NBB_USHORT usTempLabel2 = 0;
    NBB_ULONG ulLspLabel;
    NBB_ULONG ulLspLabel2;
    NBB_ULONG ulPwLabel;
    NBB_ULONG ulPwLabel2;
    NBB_ULONG baseAddr;
    NBB_BYTE ucLocalIndex = 0;
    NBB_USHORT usOamLoop = 0;
    OS_MEMSET(&tempLabel0, 0, sizeof(OD_OAM_LSP_BYTE1_STRU));
    OS_MEMSET(&tempLabel1, 0, sizeof(OD_OAM_LSP_BYTE2_STRU));
    OS_MEMSET(&tempLabel2, 0, sizeof(OD_OAM_LSP_BYTE3_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (ucLoop > (OAM_MAX_NUM_OD - 1))
    {
        return;
    }

    usOamLoop = g_stoam_odlocal[ucLoop].usOamLoop;

    if (0x0 == g_stoam_local[usOamLoop].vp_attrib) //mip
    {
        if (VC_OAM == g_stoam_local[usOamLoop].oam_type)
        {
            ulPwLabel = g_stoam_local[usOamLoop].swtich_pw_label_rx1;
            ulPwLabel2 = g_stoam_local[usOamLoop].swtich_pw_label_rx2;
            ulLspLabel2 = g_stoam_local[usOamLoop].ulTunnelLabel_rx;
        }
        else
        {
            ulPwLabel = g_stoam_odlocal[ucLoop].ulPwLabel_rx;
            ulPwLabel2 = g_stoam_odlocal[ucLoop].ulPwLabel_rx;
            ulLspLabel2 = g_stoam_local[usOamLoop].mip_second_tunnel_rx;
        }

        ulLspLabel = g_stoam_odlocal[ucLoop].ulTunnelLabel_rx;
    }
    else //mep
    {
        ulPwLabel = g_stoam_odlocal[ucLoop].ulPwLabel_rx;
        ulLspLabel = g_stoam_odlocal[ucLoop].ulTunnelLabel_rx;
    }

    ucLocalIndex = g_stoam_odlocal[ucLoop].ucOdLoop - 1;
    
    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &baseAddr);
    
    /* 写寄存器 */
    tempLabel0.default0 = 0;
    tempLabel0.Lsp19to12 = ((ulLspLabel >> 12) & 0XFF);
    tempLabel1.Lsp11to0 = (ulLspLabel & 0XFFF);
    tempLabel1.Pw19to16 = ((ulPwLabel >> 16) & 0XF);
    tempLabel2.Pw15to0 = (ulPwLabel & 0XFFFF);
    usTempLabel0 = *(NBB_USHORT *)(&tempLabel0);
    usTempLabel1 = *(NBB_USHORT *)(&tempLabel1);
    usTempLabel2 = *(NBB_USHORT *)(&tempLabel2);
    spm_oam_16write(baseAddr, DEMAND_RX_LSP_CFG_OFFSET + ucLocalIndex * 3 + 0x0,
                    usTempLabel0 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_RX_LSP_CFG_OFFSET + ucLocalIndex * 3 + 0x1,
                    usTempLabel1 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_RX_LSP_CFG_OFFSET + ucLocalIndex * 3 + 0x2,
                    usTempLabel2 NBB_CCXT_T NBB_CCXT);

    spm_oam_16write(baseAddr, DEMAND_LM_RX_LSP_OFFSET + ucLocalIndex * 3 + 0x0,
                    usTempLabel0 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_LM_RX_LSP_OFFSET + ucLocalIndex * 3 + 0x1,
                    usTempLabel1 NBB_CCXT_T NBB_CCXT);
    spm_oam_16write(baseAddr, DEMAND_LM_RX_LSP_OFFSET + ucLocalIndex * 3 + 0x2,
                    usTempLabel2 NBB_CCXT_T NBB_CCXT);
}

/*****************************************************************************
 函 数 名  : spm_oam_tx_enablefpga
 功能描述  : 条目N的帧发送标签使能
 输入参数  : NBB_USHORT usOamLoop,
            ATG_DCI_TPOAM_ACTIVE_BASIC_DATA *pstBasic,
            NBB_ULONG usTunneLable,
            NBB_ULONG usPwLable
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月6日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_tx_enablefpga(IN NBB_USHORT usOamLoop,
                               IN NBB_BYTE ActOrOdType NBB_CCXT_T NBB_CXT)
{
    FRAM_TX_LABEL_ENABLE_STRU FrameTxLableEnable;
    CV_CFH_HEAD_LABLE1_STRU CvHeadLable1;
    CV_CFH_HEAD_LABLE2_STRU CvHeadLable2;
    NBB_ULONG ulLableDa1;
    NBB_ULONG ulLableDa2;
    NBB_USHORT usFramTxEnableData;
    NBB_ULONG ulOamRegBaseAddress;
    NBB_USHORT usLocalIndex = 0; //模块本地索引
    OS_SPRINTF(g_uc_message, "%s %s,%d\nframe type=%d\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, ActOrOdType);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    OS_MEMSET((&FrameTxLableEnable), 0, sizeof(FRAM_TX_LABEL_ENABLE_STRU));
    OS_MEMSET((&CvHeadLable1), 0, sizeof(CV_CFH_HEAD_LABLE1_STRU));
    OS_MEMSET((&CvHeadLable2), 0, sizeof(CV_CFH_HEAD_LABLE2_STRU));

    if (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1))
    {
        return;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usOamLoop, &ulOamRegBaseAddress);
    usLocalIndex = g_stoam_local[usOamLoop].module_index;
    
    /* 两层标签字段赋值 */
    CvHeadLable1.TunnelLable = g_stoam_local[usOamLoop].ulTunnelLabel_tx;
    CvHeadLable1.Zero = 0;

    switch (g_stoam_local[usOamLoop].oam_type)
    {
        case VS_OAM :
            CvHeadLable1.Tc = g_stoam_local[usOamLoop].gal_exp;/* Mantis 0055856 */
            CvHeadLable1.Ttl = g_stoam_local[usOamLoop].gal_ttl;//TTL若为1，驱动会将报文识别为scc报文，上传到cpu

            CvHeadLable2.Tc = g_stoam_local[usOamLoop].gal_exp;/* Mantis 0055856 */
            CvHeadLable2.Ttl = g_stoam_local[usOamLoop].gal_ttl;
            break;

        case VP_OAM:
            CvHeadLable1.Tc = g_stoam_local[usOamLoop].lsp_exp;/* Mantis 0055856 */
            CvHeadLable1.Ttl = g_stoam_local[usOamLoop].lsp_ttl;

            CvHeadLable2.Tc = g_stoam_local[usOamLoop].gal_exp;/* Mantis 0055856 */
            CvHeadLable2.Ttl = g_stoam_local[usOamLoop].gal_ttl;
            break;

        case VC_OAM:
            CvHeadLable1.Tc = g_stoam_local[usOamLoop].lsp_exp;/* Mantis 0055856 */
            CvHeadLable1.Ttl = g_stoam_local[usOamLoop].lsp_ttl;

            CvHeadLable2.Tc = g_stoam_local[usOamLoop].pw_exp;/* Mantis 0055856 */
            CvHeadLable2.Ttl = g_stoam_local[usOamLoop].pw_ttl;/* Mantis 0055859 */
            break;

        default:
            OS_SPRINTF(g_uc_message, "%s %s,%d\noam type illegal\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            return;
    }

    CvHeadLable2.Gal = g_stoam_local[usOamLoop].ulPwLable_tx;
    CvHeadLable2.Zero = 1;
    ulLableDa1 = *(NBB_ULONG *)(&CvHeadLable1);
    ulLableDa2 = *(NBB_ULONG *)(&CvHeadLable2);

    if (FDI_OAM != ActOrOdType)
    {
        FrameTxLableEnable.CvEnable = g_stoam_local[usOamLoop].cv_enable;

        /* 若为APS帧，在number_lsp为2k条时，TxSelectSwitch发APS写00/01，其
           中发opcode 0x27写00，0x28写01，发csf写10 ；
           2k以下，就不管这个开关，aps和csf可以并发，但还是可以用这个开关控制opcode*/
        if (TRUE ==  g_stoam_local[usOamLoop].csf_enable)
        {
            FrameTxLableEnable.PwCsfEnable = g_stoam_local[usOamLoop].csf_enable;
            FrameTxLableEnable.TxSelectSwitch = CSF_TX;
        }
        else if (TRUE ==  g_stoam_local[usOamLoop].aps_enable)
        {
            FrameTxLableEnable.TxSelectSwitch = APS_TX;
            FrameTxLableEnable.LspApsEnable = g_stoam_local[usOamLoop].aps_enable;
        }

        FrameTxLableEnable.TxMel = g_stoam_local[usOamLoop].mel;
        usFramTxEnableData = *(NBB_USHORT *)(&FrameTxLableEnable);
        
        /* 条目N的发送标签使能写寄存器 */
        spm_oam_16write(ulOamRegBaseAddress, TX_FRAME_CFG_OFFSET + usLocalIndex * 8 + 0x1,
                        (ulLableDa1 >> 16) & 0xFFFF);
        spm_oam_16write(ulOamRegBaseAddress, TX_FRAME_CFG_OFFSET + usLocalIndex * 8 + 0x2,
                        ulLableDa1 & 0xFFFF);
        spm_oam_16write(ulOamRegBaseAddress, TX_FRAME_CFG_OFFSET + usLocalIndex * 8 + 0x3,
                        (ulLableDa2 >> 16) & 0xFFFF);
        spm_oam_16write(ulOamRegBaseAddress, TX_FRAME_CFG_OFFSET + usLocalIndex * 8 + 0x4,
                        ulLableDa2 & 0xFFFF);
        
        /* 发送的mepid写入寄存器 */
        spm_oam_16write(ulOamRegBaseAddress, TX_FRAME_CFG_OFFSET + usLocalIndex * 8 + 0x5,
                        g_stoam_local[usOamLoop].src_mep_id);
        spm_oam_16write(ulOamRegBaseAddress, TX_FRAME_CFG_OFFSET + usLocalIndex * 8 + 0x0,
                        usFramTxEnableData);
    }
    else if (FDI_OAM == ActOrOdType)
    {
        /* 条目N的帧发送标签赋值 */
        // FrameTxLableEnable.FdiEnable = g_stoam_local[usOamLoop].fdi_enable;
        /* 条目N的发送标签使能写寄存器 */

        spm_oam_16write(ulOamRegBaseAddress, FDI_TX_CFG_OFFSET + usLocalIndex * 8 + 0x1,
                        (ulLableDa1 >> 16) & 0xFFFF);
        spm_oam_16write(ulOamRegBaseAddress, FDI_TX_CFG_OFFSET + usLocalIndex * 8 + 0x2,
                        ulLableDa1 & 0xFFFF);
        spm_oam_16write(ulOamRegBaseAddress, FDI_TX_CFG_OFFSET + usLocalIndex * 8 + 0x3,
                        (ulLableDa2 >> 16) & 0xFFFF);
        spm_oam_16write(ulOamRegBaseAddress, FDI_TX_CFG_OFFSET + usLocalIndex * 8 + 0x4,
                        ulLableDa2 & 0xFFFF);
    }
}

/*****************************************************************************
 函 数 名  : spm_oam_tx_megfpga
 功能描述  : 发向MEG配置
 输入参数  : NBB_USHORT OamIndex,
            ATG_DCI_TPOAM_ACTIVE_BASIC_DATA *pstBasic
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月6日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_tx_megfpga(IN NBB_USHORT usLoop)
{
    MEG_TX_CONFIG_STRU MegTxConfig;
    NBB_USHORT usMegTxData;
    NBB_USHORT usLocalIndex;
    NBB_USHORT MegIcc1to2;
    NBB_USHORT MegIcc3to4;
    NBB_USHORT MegIcc5;
    NBB_USHORT MegUmc1to2;
    NBB_USHORT MegUmc3to4;
    NBB_USHORT MegUmc5;
    NBB_ULONG ulOamRegBaseAddress;
    OS_MEMSET((&MegTxConfig), 0, sizeof(MEG_TX_CONFIG_STRU));
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (usLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1))
    {
        return;
    }

    /* 判断所在模块 */
    spm_get_moudle_base_addr(usLoop, &ulOamRegBaseAddress);
    usLocalIndex = g_stoam_local[usLoop].module_index;
    MegTxConfig.Default = 0;
    MegTxConfig.CvTxPreriod = g_stoam_local[usLoop].cv_cycle;
    MegTxConfig.MegId = g_stoam_local[usLoop].meg_icc[0];
    usMegTxData = *(NBB_USHORT *)(&MegTxConfig);
    MegIcc1to2 = g_stoam_local[usLoop].meg_icc[1];
    MegIcc1to2 = ((MegIcc1to2 << 8) & 0xff00) + g_stoam_local[usLoop].meg_icc[2];
    MegIcc3to4 = g_stoam_local[usLoop].meg_icc[3];
    MegIcc3to4 = ((MegIcc3to4 << 8) & 0xff00) + g_stoam_local[usLoop].meg_icc[4];
    MegIcc5 = g_stoam_local[usLoop].meg_icc[5];
    MegIcc5 = ((MegIcc5 << 8) & 0xff00) + g_stoam_local[usLoop].meg_umc[0];
    MegUmc1to2 = g_stoam_local[usLoop].meg_umc[1];
    MegUmc1to2 = ((MegUmc1to2 << 8) & 0xff00) + g_stoam_local[usLoop].meg_umc[2];
    MegUmc3to4 = g_stoam_local[usLoop].meg_umc[3];
    MegUmc3to4 = ((MegUmc3to4 << 8) & 0xff00) + g_stoam_local[usLoop].meg_umc[4];
    MegUmc5 = g_stoam_local[usLoop].meg_umc[5];
    MegUmc5 = (MegUmc5 << 8) & 0xff00;/* Mantis 0055862 */
    
    /* tms/tmp/tmc层cv&asps/csf成帧MEG配置 */
    spm_oam_16write(ulOamRegBaseAddress, CV_TXMEG_OFFSET + usLocalIndex * 8,
                    usMegTxData);
    spm_oam_16write(ulOamRegBaseAddress, CV_TXMEG_OFFSET + usLocalIndex * 8 + 0x1,
                    MegIcc1to2);
    spm_oam_16write(ulOamRegBaseAddress, CV_TXMEG_OFFSET + usLocalIndex * 8 + 0x2,
                    MegIcc3to4);
    spm_oam_16write(ulOamRegBaseAddress, CV_TXMEG_OFFSET + usLocalIndex * 8 + 0x3,
                    MegIcc5);
    spm_oam_16write(ulOamRegBaseAddress, CV_TXMEG_OFFSET + usLocalIndex * 8 + 0x4,
                    MegUmc1to2);
    spm_oam_16write(ulOamRegBaseAddress, CV_TXMEG_OFFSET + usLocalIndex * 8 + 0x5,
                    MegUmc3to4);
    spm_oam_16write(ulOamRegBaseAddress, CV_TXMEG_OFFSET + usLocalIndex * 8 + 0x6,
                    MegUmc5);
    spm_oam_16write(ulOamRegBaseAddress, CV_TXMEG_OFFSET + usLocalIndex * 8 + 0x7,
                    0x0000);
}

/*****************************************************************************
 函 数 名  : spm_get_moudle_base_addr
 功能描述  : 获取模块基地址和在模块里的序号
 输入参数  : NBB_ULONG
 输出参数  : 无
 返 回 值  : NBB_ULONG
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年12月23日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_get_moudle_base_addr(IN NBB_USHORT usOamLoop,
                                  OUT NBB_ULONG *pulOamRegBaseAddress NBB_CCXT_T NBB_CXT)
{
    if (usOamLoop > (OAM_MAX_NUM_ACTIVE_SPU - 1))
    {
        return;
    }

#if SPU

    //判断该条目存在哪个模块
    if (FALSE == g_stoam_local[usOamLoop].bModuleSwitch)
    {
        *pulOamRegBaseAddress = OAM_REG_BASE1_ADDRESS;
    }
    else
    {
        *pulOamRegBaseAddress = OAM_REG_BASE2_ADDRESS;
    }

#endif
#if SRC
    *pulOamRegBaseAddress = OAM_REG_BASE_ADDRESS_CCU;
#endif
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_update
 功能描述  : oam修改配置
 输入参数  : OAM_ACTIVE_LOCAL_STRU *pstOamActive
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年3月1日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_update(SPM_OAM_CB *poamcfg)
{
    NBB_INT iRet = ERROR; /* 表示添加成功或失败的变量 */
    NBB_USHORT vs_index;
    NBB_USHORT iloop = 0;
    NBB_ULONG ulOamId = 0;
    NBB_USHORT usmegData1;
    NBB_USHORT usmegData2;
    NBB_USHORT usmegData3;
    NBB_USHORT usmegData4;
    NBB_USHORT usmegData5;
    NBB_USHORT usmegData6;
    NBB_BYTE ucmegData7;

    if (NULL == poamcfg)
    {
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    
    /* 将树的配置放入本地数组，方便发送报文 */
    iloop = poamcfg->oamindex;
    ulOamId = poamcfg->oam_id;
    g_stoam_local[iloop].global_index = poamcfg->oamindex;

    if (iloop < OAMTREE_MAX_NODE)
    {
        g_stoam_local[iloop].bModuleSwitch = 0;
        g_stoam_local[iloop].module_index = iloop;
    }
    else
    {
        g_stoam_local[iloop].bModuleSwitch = 1;
        g_stoam_local[iloop].module_index = iloop - OAMTREE_MAX_NODE;
    }

    g_stoam_local[iloop].aps_enable = poamcfg->aps_enable;
    g_stoam_local[iloop].csf_enable = poamcfg->csf_enable;
    g_stoam_local[iloop].cv_cycle = poamcfg->cv_cycle;
    g_stoam_local[iloop].cv_enable = poamcfg->cv_enable;
    g_stoam_local[iloop].fdi_enable = poamcfg->fdi_enable;

    g_stoam_local[iloop].oam_type = poamcfg->oamtype;
    g_stoam_local[iloop].chip_id = poamcfg->chip_id;
    g_stoam_local[iloop].vp_attrib = poamcfg->pBasiCfg->vp_attrib;
    NBB_MEMCPY(g_stoam_local[iloop].meg_icc, poamcfg->pBasiCfg->meg_icc, 6 * sizeof(NBB_BYTE));
    NBB_MEMCPY(g_stoam_local[iloop].meg_umc, poamcfg->pBasiCfg->meg_umc, 6 * sizeof(NBB_BYTE));
    g_stoam_local[iloop].dst_mep_id = poamcfg->pBasiCfg->dst_mep_id;
    g_stoam_local[iloop].src_mep_id = poamcfg->pBasiCfg->src_mep_id;
    g_stoam_local[iloop].mel = poamcfg->pBasiCfg->mel;
    g_stoam_local[iloop].lsp_exp = poamcfg->pBasiCfg->lsp_exp;
    g_stoam_local[iloop].pw_exp = poamcfg->pBasiCfg->pw_exp;
    g_stoam_local[iloop].gal_exp = poamcfg->pBasiCfg->gal_exp;
    g_stoam_local[iloop].gal_ttl = poamcfg->pBasiCfg->gal_ttl;
    g_stoam_local[iloop].lsp_ttl = poamcfg->pBasiCfg->lsp_ttl;
    g_stoam_local[iloop].pw_ttl = poamcfg->pBasiCfg->pw_ttl;

    g_stoam_local[iloop].ulActiveOamkey = poamcfg->oam_id;
    g_stoam_local[iloop].ulTunnelLabel_tx = poamcfg->ulTunnelLabel_tx;
    g_stoam_local[iloop].ulPwLable_tx = poamcfg->ulPwLable_tx;
    g_stoam_local[iloop].ulTunnelLabel_rx = poamcfg->ulTunnelLabel_rx;
    g_stoam_local[iloop].ulPwLable_rx = poamcfg->ulPwLable_rx;

    if (NULL != poamcfg->pAdjust)
    {
        g_stoam_local[iloop].nearpklos = poamcfg->pAdjust->near_pklos;
        g_stoam_local[iloop].farpklos = poamcfg->pAdjust->far_pklos;
        g_stoam_local[iloop].delay = poamcfg->pAdjust->delay;
        g_stoam_local[iloop].vardelay = poamcfg->pAdjust->vardelay;
    }

    if (NULL != poamcfg->pAlarm)
    {
        g_stoam_local[iloop].ais_alarm_flag = poamcfg->pAlarm->ais_alarm_flag;
        g_stoam_local[iloop].rdi_alarm_flag = poamcfg->pAlarm->rdi_alarm_flag;
    }

    switch (poamcfg->oamtype)
    {
        case WRAPPING_OAM:
            if (NULL == poamcfg->pWrapping)
            {
                goto EXIT_LABEL;
            }

            g_stoam_local[iloop].vs_Portid = poamcfg->out_portid;
            g_stoam_local[iloop].wrapping_port_logical_index =
                poamcfg->logical_port_index;//mantis 58637
            g_stoam_local[iloop].vs_Slotid = poamcfg->slot_id;
            g_stoam_local[iloop].ip = poamcfg->nhop_ip;
            break;

        case VS_OAM :
            if (NULL == poamcfg->pVsCfg)
            {
                goto EXIT_LABEL;
            }

            g_stoam_local[iloop].vs_Portid = poamcfg->vs_portid;
            g_stoam_local[iloop].vs_Slotid = poamcfg->vs_slotid;
            g_stoam_local[iloop].ip = poamcfg->nhop_ip;
            
            //通过vs层主动lm下插sd
            spm_record_sd_oamloop(iloop);
            break;

        case VP_OAM:
            if (NULL == poamcfg->pRsvplspCfg)
            {
                goto EXIT_LABEL;
            }

            /* MEP */
            if (0x1 == g_stoam_local[iloop].vp_attrib)
            {
                g_stoam_local[iloop].vp_Lspid = poamcfg->lsp_id_tx;
                g_stoam_local[iloop].vp_Slot = poamcfg->slotMcid;
                g_stoam_local[iloop].vp_PortId = poamcfg->vp_PortId_tx;
            }

            /* MIP */
            else if (0x0 == g_stoam_local[iloop].vp_attrib)
            {
                g_stoam_local[iloop].inport1 = poamcfg->vp_PortId_tx;
                g_stoam_local[iloop].lspid1 = poamcfg->lsp_id_tx;
                g_stoam_local[iloop].inport_index1 = poamcfg->inport_index_tx;
                g_stoam_local[iloop].inport2 = poamcfg->vp_PortId_rx;
                g_stoam_local[iloop].lspid2 = poamcfg->lsp_id_rx;
                g_stoam_local[iloop].inport_index2 = poamcfg->inport_index_rx;
            }

            break;

        case VC_OAM:
            if (NULL == poamcfg->pPwCfg)
            {
                goto EXIT_LABEL;
            }

            g_stoam_local[iloop].vc_no_section1 = poamcfg->vc_no_section1;
            g_stoam_local[iloop].vc_no_section2 = poamcfg->vc_no_section2;
            g_stoam_local[iloop].vc_PortId_section1 = poamcfg->vc_PortId_section1;
            g_stoam_local[iloop].vc_PortId_section2 = poamcfg->vc_PortId_section2;
            g_stoam_local[iloop].uni_slot = poamcfg->uni_slot;
            g_stoam_local[iloop].nni_slot = poamcfg->nni_slot;
            g_stoam_local[iloop].uni_port_type = poamcfg->uni_port_type;
            g_stoam_local[iloop].vpn_type = poamcfg->vpn_type;
            g_stoam_local[iloop].vp_id = poamcfg->vp_id;

            break;

        default:
            goto EXIT_LABEL;
    }

#if 0
    iRet = spm_set_mpls_flag(poamcfg);

    if (SUCCESS != iRet)
    {
        goto EXIT_LABEL;
    }
#endif

#if 0
#ifdef PTN690_CES
    VC_OAM_T stOAM;
    usmegData1 = g_stoam_local[iloop].meg_icc[0];
    usmegData1 = ((usmegData1 << 8) & 0xff00) + g_stoam_local[iloop].meg_icc[1];

    usmegData2 = g_stoam_local[iloop].meg_icc[2];
    usmegData2 = ((usmegData2 << 8) & 0xff00) + g_stoam_local[iloop].meg_icc[3];

    usmegData3 = g_stoam_local[iloop].meg_icc[4];
    usmegData3 = ((usmegData3 << 8) & 0xff00) + g_stoam_local[iloop].meg_icc[5];

    usmegData4 = g_stoam_local[iloop].meg_umc[0];
    usmegData4 = ((usmegData4 << 8) & 0xff00) + g_stoam_local[iloop].meg_umc[1];

    usmegData5 = g_stoam_local[iloop].meg_umc[2];
    usmegData5 = ((usmegData5 << 8) & 0xff00) + g_stoam_local[iloop].meg_umc[3];

    usmegData6 = g_stoam_local[iloop].meg_umc[4];
    usmegData6 = ((usmegData6 << 8) & 0xff00) + g_stoam_local[iloop].meg_umc[5];

    ucmegData7 = 0;

    stOAM.rxMepId = g_stoam_local[iloop].dst_mep_id;
    stOAM.txMepId = g_stoam_local[iloop].src_mep_id;
    stOAM.mel = g_stoam_local[iloop].mel;
    stOAM.ccmEna = g_stoam_local[iloop].cv_enable;
    stOAM.ccmPeriod = g_stoam_local[iloop].cv_cycle;
    g_csf_ena = g_stoam_local[iloop].csf_enable;
    g_cv_ena = g_stoam_local[iloop].cv_enable;
    stOAM.megData1 = usmegData1;
    stOAM.megData2 = usmegData2;
    stOAM.megData3 = usmegData3;
    stOAM.megData4 = usmegData4;
    stOAM.megData5 = usmegData5;
    stOAM.megData6 = usmegData6;
    stOAM.megData7 = ucmegData7;

    stOAM.csfPeriod = 1;//表示3.3ms一帧

    stOAM.csfGroupId = g_group_id;
    stOAM.apsEna = g_stoam_local[iloop].aps_enable;

    stOAM.apsPeriod = 1;
    stOAM.oamIdx = g_stoam_local[iloop].vc_ces_oamIdx;
    stOAM.nhiId = g_stoam_local[iloop].vc_no_section1;
    stOAM.csfEna = g_stoam_local[iloop].csf_enable;
    printf("ccmPeriod = %d mel = %d oamIdx = %d\n", stOAM.ccmPeriod, stOAM.mel, stOAM.oamIdx);
    printf("csfPeriod = %d csfGroupId = %d\n", stOAM.csfPeriod, stOAM.csfGroupId);
    iRet = ApiModOam(0, &stOAM);
    printf("ApiModOam iRet = %d\n", iRet);
#else

    /* 判断oam类型，不同类型的二叉树的内容不同 */
    spm_oam_send_packedge(poamcfg);
#endif
#endif

    if ((poamcfg->IsaddSuccess == OAM_FAILED)
        || (poamcfg->IsaddSuccess == OAM_INIT_STATE))
    {
        poamcfg->IsaddSuccess = OAM_UP_CFG;
        g_usoam_num++;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
               "update oam success!\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__, ulOamId);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_od_update
 功能描述  : 按需oam更新配置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月23日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_od_update(IN OAM_ON_DEMAND_LOCAL_STRU *pstOdOam,
                          NBB_BYTE odloop)
{
    NBB_INT iRet = ERROR;

    if (NULL == pstOdOam)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n Input od oam cfg pointer is NUll\n"
                   "update od oam failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if (odloop > (OAM_MAX_NUM_OD - 1))
    {
        goto EXIT_LABEL;
    }

    spm_od_oam_update_cfg(odloop, pstOdOam);
    spm_oam_od_send_packedge(odloop);
    spm_od_rx_label_fpga(odloop);
    iRet = SUCCESS;
    OS_SPRINTF(g_uc_message, "%s %s,%d\n update od oam succeed oam id=%d\n",
               ACT_OAM_END_STRING, __FUNCTION__, __LINE__, pstOdOam->ulOamActiveKey);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_oam_od_send_packedge
 功能描述  : 按需OAM发包
 输入参数  : NBB_BYTE ucOdOamIndex
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年5月7日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_od_send_packedge(IN NBB_BYTE ucOdLoop)
{
    NBB_USHORT usOamActLoop;

    if (ucOdLoop > (OAM_MAX_NUM_OD - 1))
    {
        return;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING,
               __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    
    /* 寻找相关主动OAM配置 */
    usOamActLoop = g_stoam_odlocal[ucOdLoop].usOamLoop;
    spm_od_lbr_dmr(usOamActLoop, ucOdLoop);

    if ((NULL != g_stoam_odlocal[ucOdLoop].pLoopFrame))
    {
        spm_od_lb(usOamActLoop, ucOdLoop);
    }

    if (NULL != g_stoam_odlocal[ucOdLoop].pLckFrame)
    {
        spm_od_lck(usOamActLoop, ucOdLoop);
    }

    if (NULL != g_stoam_odlocal[ucOdLoop].pDmFrame)
    {
        spm_od_dm(usOamActLoop, ucOdLoop);
    }

    if (NULL != g_stoam_odlocal[ucOdLoop].pTestFrame)
    {
        spm_od_tst(usOamActLoop, ucOdLoop);
    }

    if (NULL != g_stoam_odlocal[ucOdLoop].pLmFrame)
    {
#ifdef SPU
        spm_od_lm(usOamActLoop, ucOdLoop);
#endif
    }

    return;
}


/*****************************************************************************
 函 数 名  : spm_oam_od_add
 功能描述  : 按需oam添加配置
 输入参数  : 无
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月23日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_od_add(IN OAM_ON_DEMAND_LOCAL_STRU *pstOdOam)
{
    NBB_BYTE ucLoop;
    NBB_INT iRet = ERROR;
    NBB_USHORT usOamActLoop;
    NBB_BYTE ucUniSlot = 0;
    OAM_VC_KEY OamVcKey;
    OAM_ALMPM_LINE_STRUCT st_oam_alarm_line;

    if (NULL == pstOdOam)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n Input od oam cfg pointer is NUll\n"
                   "add od oam to array failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    for (ucLoop = 0 ; ucLoop < OAM_MAX_NUM_OD; ucLoop++)
    {
        if (USELESS == g_stoam_odlocal[ucLoop].ulOamActiveKey)
        {
            g_stoam_odlocal[ucLoop].ulOamActiveKey = pstOdOam->ulOamActiveKey;
            g_stoam_odlocal[ucLoop].usOamLoop = pstOdOam->usOamLoop;
            usOamActLoop = g_stoam_odlocal[ucLoop].usOamLoop;
            spm_od_oam_add_cfg(ucLoop, pstOdOam);

            if (NULL == g_stoam_odlocal[ucLoop].pBasicData)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\nod oam basic cfg pointer is NUll\n"
                           "add od oam to array failed\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                goto EXIT_LABEL;
            }

//            printf("usOamActLoop=%d,ucLoop=%d\n", usOamActLoop, ucLoop);
            iRet = spm_add_od_num(usOamActLoop, ucLoop);

            if (SUCCESS != iRet)
            {
                goto EXIT_LABEL;
            }

            /* basic */
            NBB_MEMCPY(g_stoam_odlocal[ucLoop].pBasicData,
                       pstOdOam->pBasicData, sizeof(ATG_DCI_TPOAM_OD_BASIC_DATA));

            g_stoam_odlocal[ucLoop].bModuleSwitch =
                g_stoam_local[usOamActLoop].bModuleSwitch;
            g_stoam_odlocal[ucLoop].bvpAttribute =
                g_stoam_local[usOamActLoop].vp_attrib;
            g_stoam_odlocal[ucLoop].ulPwLabel_tx =
                g_stoam_local[usOamActLoop].ulPwLable_tx;
            g_stoam_odlocal[ucLoop].ulTunnelLabel_tx =
                g_stoam_local[usOamActLoop].ulTunnelLabel_tx;
            g_stoam_odlocal[ucLoop].ulPwLabel_rx =
                g_stoam_local[usOamActLoop].ulPwLable_rx;
            g_stoam_odlocal[ucLoop].ulTunnelLabel_rx =
                g_stoam_local[usOamActLoop].ulTunnelLabel_rx;

            spm_oam_od_send_packedge(ucLoop);

            spm_od_rx_label_fpga(ucLoop);


#if 0

            if (0x0 == g_stoam_local[usOamActLoop].vp_attrib)
            {
                spm_mip_lb_sencond_rx_fpga(usOamActLoop, ucLoop);
//                spm_od_mip_lck_second_rx_fpga(usOamActLoop,ucLoop);
            }

#endif

            if (g_stoam_local[usOamActLoop].oam_type == 2)
            {
                iRet = spm_get_vc_key(&OamVcKey);

                if (iRet != SUCCESS)
                {
                    printf("from spm_get_vc_key Get vc key fail \n");
                    goto EXIT_LABEL;
                }

                printf("add almpm_cfg vc_id %d vc_type %d peer_ip %x\n", 
                    OamVcKey.VcId, OamVcKey.VcType, OamVcKey.PeerIp);
                g_stoam_vckey.VcId = OamVcKey.VcId;
                g_stoam_vckey.VcType = OamVcKey.VcType;
                g_stoam_vckey.PeerIp = OamVcKey.PeerIp;
                st_oam_alarm_line.usLineType = VC_LINE_TYPE;
                st_oam_alarm_line.usLineLayer = VC_LINE_LAYER;
                st_oam_alarm_line.ucKeyLen = VC_KEY_LEN;
                OS_MEMCPY(st_oam_alarm_line.ucKeyValue, &OamVcKey, VC_KEY_LEN);
#if defined (SPU) || defined (PTN690_CES)
                iRet = almpm_cfg(st_oam_alarm_line, 0, 0, 1);
#endif
                g_register_vcoam_flag = 1;
            }

            iRet = SUCCESS;
            break;
        }
    }

    if (ucLoop == OAM_MAX_NUM_OD)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "Local od oam cfg is full\n"
                   "add od oam to array failed\n",
                   ACT_OAM_END_STRING, __FUNCTION__, __LINE__, pstOdOam->ulOamActiveKey);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iRet = ERROR;
    }
    else
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "add od oam to array succeed\n",
                   ACT_OAM_END_STRING, __FUNCTION__, __LINE__, pstOdOam->ulOamActiveKey);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    }

    spm_memfree_od_oam_cfg(pstOdOam);
EXIT_LABEL:
    return iRet;
}



/*****************************************************************************
 函 数 名  : spm_oam_get_local_od_oamindex
 功能描述  : 根据全局odOAMID得出在全局数组的索引
 输入参数  : NBB_USHORT usOamCfgId

 输出参数  : NBB_USHORT *usLocalOamId
 返 回 值  : SUCCESS/ERROR
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月2日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_get_local_od_oamindex(IN NBB_ULONG ulOamId,
                                      OUT NBB_BYTE *ucLocalOdLoop
                                      NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    NBB_BYTE ucLoop = 0;

    /* 判断入参 */
    if (ulOamId > OAM_ID_MAX)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "ucOdOamId is illegal\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    for (ucLoop = 0; ucLoop < OAM_MAX_NUM_OD; ucLoop++)
    {
        if (ulOamId == g_stoam_odlocal[ucLoop].ulOamActiveKey)
        {
            *ucLocalOdLoop = ucLoop;
            iRet = SUCCESS;
            break;
        }
    }

    if (ucLoop == OAM_MAX_NUM_OD)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                   "OdOamId is not exist\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iRet = ERROR;
        goto EXIT_LABEL;
    }

EXIT_LABEL:
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_active_oam
 功能描述  : OAM配置处理模块
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年2月28日
    作    者   : huxi
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_active_oam(ATG_DCI_SET_TPOAM_ACTIVE *pstActiveOam_set)
{
    NBB_ULONG ulOamId = 0;
    NBB_INT iRet = ERROR;
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMainBoradSlot = 0; //处理盘槽位号
    SPM_OAM_CB *poamcfg = NULL;
    NBB_ULONG uloamid = 0;
    NBB_USHORT usOamindex = 0;
    SPM_OAM_VS_CB *p_vscfg = NULL;
    SPM_OAM_RSVP_CB_TX    *p_rsvpcfg_tx = NULL;
    SPM_OAM_RSVP_CB_RX    *p_rsvpcfg_rx = NULL;
    SPM_OAM_PW_CB *p_pwcfg = NULL;
    SPM_OAM_WRAPPING_CB *p_wrappingcfg = NULL;
    OAM_RSVP_LSP_KEY stRsvpKey_tx;
    OAM_RSVP_LSP_KEY stRsvpKey_rx;
    SPM_PW_KEY stPwKey;
    OAM_ALMPM_LINE_STRUCT st_oam_alarm_line;
    NBB_ULONG ulTempVcid = 0;

    OS_MEMSET(&stRsvpKey_tx, 0, sizeof(OAM_RSVP_LSP_KEY));
    OS_MEMSET(&stRsvpKey_rx, 0, sizeof(OAM_RSVP_LSP_KEY));
    OS_MEMSET(&stPwKey, 0, sizeof(SPM_PW_KEY));
    OS_MEMSET(&st_oam_alarm_line, 0, sizeof(OAM_ALMPM_LINE_STRUCT));

    /* 入参判断 */
    if (NULL == pstActiveOam_set)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n Input active oam cfg pointer is NUll\n"
                   "rev oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d,oamid=%d,action = %s\n", ACT_OAM_ENTER_STRING,
               __FUNCTION__, __LINE__, pstActiveOam_set->key,
               (pstActiveOam_set->delete_struct ? "delete" : "add"));
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    ulOamId = pstActiveOam_set->key;

    if ((ulOamId == 0) || (ulOamId > OAM_ID_MAX))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n illegal oamid, oamid = %ld\n"
                   "rev oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
        goto EXIT_LABEL;
    }

    /* 获取本盘槽位 */
    iRet = spm_hardw_getslot(&ucLocalSlot);

    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n get local slot error\n"
                   "rev oam cfg failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
        goto EXIT_LABEL;
    }

    /* 删除配置 */
    if (TRUE == pstActiveOam_set->delete_struct)
    {
        poamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &ulOamId);

        /* 若找不到配置 */
        if (NULL == poamcfg)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\nDelete failed, oamid = %ld is not exit\n"
                       "delete oam failed\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstActiveOam_set->key);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
            goto EXIT_LABEL;
        }
        else
        {
            /* 判断OAM类型 */
            switch (poamcfg->oamtype)
            {
                case VS_OAM:
                    spm_delete_avll_tree_by_index(poamcfg);
                    break;
                default:
                    break;
            }

            uloamid = poamcfg->oam_id;
            usOamindex = poamcfg->oamindex;
            
            //删除搜索树中的内容
            AVLL_DELETE(v_spm_shared->g_oamavltreebyoamid, poamcfg->spm_oam_node);
            spm_memfree_oam_cfg(poamcfg);
            NBB_MM_FREE(poamcfg, MEM_SPM_ACTIVE_OAM_CB);
            poamcfg = NULL;

            if (g_usoam_num <= 0)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oam num is less than zero,oamid = %ld\n"
                           "oam number = %d,delete oam failed\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId, g_usoam_num);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                goto EXIT_LABEL;
            }

            /* 删除全局数组里面的内容 */
            iRet = spm_oam_delete(usOamindex);

            if (SUCCESS != iRet)   /* 判断执行结果 */
            {
                /* 函数里面已添加打印信息 */
                goto EXIT_LABEL;
            }
            else     /* 成功 */
            {
                g_usoam_num--;
                spm_recycle_oam_index(usOamindex);

                //删除主控上的OAMID映射表的条目
                //64k_alarm_inband
                spm_oam_set_oam_id_conf(uloamid, usOamindex, 0);

            }

            goto EXIT_LABEL;
        }
    }

    /* 添加配置 */
    else
    {
        /* 初始化基本配置结点 */
        poamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &ulOamId);

        if (NULL == poamcfg) /* 添加 */
        {
            poamcfg = (SPM_OAM_CB *)NBB_MM_ALLOC(sizeof(SPM_OAM_CB), NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_CB);

            if (NULL == poamcfg)
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n memory is not enough for SPM_OAM_CB,oamid = %ld\n"
                           "rcv oam cfg failed\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstActiveOam_set->key);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                goto EXIT_LABEL;
            }

            OS_MEMSET(poamcfg, 0, sizeof(SPM_OAM_CB));
            AVLL_INIT_NODE(poamcfg->spm_oam_node);
            
            //从ips消息获取配置
            iRet = spm_set_from_bmu_active(pstActiveOam_set, poamcfg);

            if (SUCCESS != iRet)
            {
                spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                goto EXIT_LABEL;
            }

            //获取OAM帧的使能信息
            iRet = spm_get_enable(poamcfg);

            if (SUCCESS != iRet)
            {
                poamcfg->IsaddSuccess = OAM_FAILED;
                spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                goto EXIT_LABEL;
            }

            poamcfg->oam_id = ulOamId;
            poamcfg->oamindex = SD_UNUSED;
            poamcfg->oamtype = poamcfg->pBasiCfg->oam_type;
            poamcfg->IsaddSuccess = OAM_INIT_STATE;

            //插入搜索树节点
            //iRet = AVLL_INSERT(v_spm_shared->g_oamavltreebyoamid, poamcfg->spm_oam_node);
            if (!AVLL_INSERT(v_spm_shared->g_oamavltreebyoamid, poamcfg->spm_oam_node))
            {
                spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                goto EXIT_LABEL;
            }

            //CFM和TPOAM共用FPGA，调用条目分配函数，获取索引
            iRet = spm_assign_oam_index(&(poamcfg->oamindex),
                                        &(poamcfg->module_index), &(poamcfg->module));

            if (SUCCESS != iRet)
            {
                spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                goto EXIT_LABEL;
            }

        ADD_OAM:

            //根据OAM层次来判断处理盘槽位号
            switch (poamcfg->oamtype)
            {
                case VS_OAM :
                    if(NULL == poamcfg->pVsCfg)
                    {
                        poamcfg->IsaddSuccess = OAM_FAILED;
                        goto EXIT_LABEL;
                    }
                    p_vscfg = (SPM_OAM_VS_CB *)NBB_MM_ALLOC(sizeof(SPM_OAM_VS_CB),
                                                            NBB_NORETRY_ACT, MEM_SPM_ACTIVE_OAM_VS_CB);

                    if (NULL == p_vscfg)
                    {
                        OS_SPRINTF(g_uc_message, "%s %s,%d\n memory is not enough for SPM_OAM_VS_CB oamid = %ld\n"
                                   "rcv oam cfg failed\n",
                                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
                        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                        goto EXIT_LABEL;
                    }

                    OS_MEMSET(p_vscfg, 0, sizeof(SPM_OAM_VS_CB));
                    AVLL_INIT_NODE(p_vscfg->spm_vs_node);
                    p_vscfg->oamid = poamcfg->oam_id;
                    p_vscfg->vs_index = poamcfg->pVsCfg->index;
                    p_vscfg->oamindex = poamcfg->oamindex;
                    
                    //iRet = AVLL_INSERT(v_spm_shared->g_oamavltreebyindex, p_vscfg->spm_vs_node);
                    if ( !AVLL_INSERT(v_spm_shared->g_oamavltreebyindex, p_vscfg->spm_vs_node))
                    {
                        poamcfg->IsaddSuccess = OAM_FAILED;
                        spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                        goto EXIT_LABEL;
                    }

                    //分析VS的业务配置，判断OAM是否建在本盘
                    iRet = spm_add_vs_all(p_vscfg, poamcfg, ucLocalSlot);

                    if (SUCCESS != iRet)
                    {
                        poamcfg->IsaddSuccess = OAM_FAILED;
                        spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                        goto EXIT_LABEL;
                    }

                    break;

                default:
                    OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                               "oamtype is illegal,add failed\n",
                               ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
                    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                    goto EXIT_LABEL;
            }

            /* 全局配置树插入结点 */
            iRet = spm_add_tree(ulOamId, poamcfg);

            //不是节点已存在或建树成功，则退出
            if (!((-4 == iRet) || (SUCCESS == iRet)))  
            {
                goto EXIT_LABEL;
            }

            //添加到全局数组，准备写FPGA发包
            iRet = spm_oam_add_to_array(poamcfg);

            if (SUCCESS != iRet)
            {
                poamcfg->IsaddSuccess = OAM_FAILED;
                spm_to_bmu_return_code_unsuccessful(pstActiveOam_set);
                goto EXIT_LABEL;
            }

            spm_to_bmu_return_code_ok(pstActiveOam_set);
            goto EXIT_LABEL;
        }
        else /* 修改 */
        {
            //从IPS消息获取配置更新
            iRet = spm_set_from_bmu_active_update(pstActiveOam_set, poamcfg);

            if (SUCCESS != iRet)
            {
                poamcfg->IsaddSuccess = OAM_FAILED;
                goto EXIT_LABEL;
            }

            //获取OAM帧使能
            iRet = spm_get_enable(poamcfg);

            if (SUCCESS != iRet)
            {
                poamcfg->IsaddSuccess = OAM_FAILED;
                goto EXIT_LABEL;
            }

            //根据帧类型，更新配置树
            switch (poamcfg->oamtype)
            {
                case VS_OAM:
                    if (NULL == poamcfg->pVsCfg)
                    {
                        poamcfg->IsaddSuccess = OAM_FAILED;
                        goto EXIT_LABEL;
                    }

                    p_vscfg = AVLL_FIND(v_spm_shared->g_oamavltreebyindex, &(poamcfg->pVsCfg->index));
                    
                    if (NULL != p_vscfg)//上次添加成功
                    {
                        //分析VS业务配置，判断OAM是否建在本盘
                        iRet = spm_add_vs_all(p_vscfg, poamcfg, ucLocalSlot);

                        if (SUCCESS != iRet)
                        {
                            poamcfg->IsaddSuccess = OAM_FAILED;
                            goto EXIT_LABEL;
                        }
                    }
                    else//若上次未添加成功，则改走添加流程
                    {
                        goto ADD_OAM;
                    }

                    break;
                default:
                    break;
            }

            //添加FPGA搜索树
            iRet = spm_add_tree(ulOamId, poamcfg);
            
            //不是节点已存在或建树成功，则退出
            if (!((-4 == iRet) || (SUCCESS == iRet)))  
            {
                goto EXIT_LABEL;
            }

            //更新全局数组里的OAM配置，准备写FPGA，发包
            iRet = spm_oam_update(poamcfg);

            if (SUCCESS != iRet)
            {
                /* 函数内已有打印 */
                poamcfg->IsaddSuccess = OAM_FAILED;
                goto EXIT_LABEL;
            }
            else
            {
                poamcfg->IsaddSuccess = OAM_UP_CFG;
                spm_to_bmu_return_code_ok(pstActiveOam_set);
                goto EXIT_LABEL;
            }
        }
    }

EXIT_LABEL:

    if (NULL != poamcfg)
    {
        //若OAM关联的业务配置错误，则将此OAMID在全局数组的内容删除
        if (poamcfg->IsaddSuccess == OAM_FAILED)//添加失败
        {
            spm_delete_wrong_cfg(&poamcfg);
        }
    }

    return;
}

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_od_oam
 功能描述  : 接收按需OAM IPS消息
 输入参数  : 无
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年4月25日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_od_oam(IN ATG_DCI_SET_TPOAM_ON_DEMAND *pstOdOam_set)
{
    NBB_INT iRet;
    NBB_ULONG ulOamId;
    NBB_BYTE ucMainBoradSlot = 0;
    NBB_BYTE ucLocalSlot;
    NBB_BYTE ucOdLoop = 0;
    NBB_BYTE odloop = 0;
    NBB_USHORT usOamLoop = 0;
    OAM_ON_DEMAND_LOCAL_STRU stOdOam;
    OS_MEMSET(&stOdOam, 0, sizeof(OAM_ON_DEMAND_LOCAL_STRU));

    /* 入参判断 */
    if (NULL == pstOdOam_set)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n Input od oam cfg pointer is NUll\n"
                   "rcv od oam failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d,oamid=%d,action=%s\n",
               ACT_OAM_ENTER_STRING, __FUNCTION__, __LINE__, pstOdOam_set->key,
               pstOdOam_set->delete_struct ? "delete" : "add");
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

    if ((pstOdOam_set->key == 0) || (pstOdOam_set->key > OAM_ID_MAX))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n illegal oamid, oamid = %ld\n"
                   "rcv od oam failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstOdOam_set->key);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
        goto EXIT_LABEL;
    }

    ulOamId = pstOdOam_set->key;
    iRet = spm_hardw_getslot(&ucLocalSlot);

    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n get localslot error\n"
                   "rcv od oam failed\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
        goto EXIT_LABEL;
    }


    /* 删除配置 */
    if (TRUE == pstOdOam_set->delete_struct)
    {
        iRet = spm_oam_get_local_od_oamindex(ulOamId, &ucOdLoop);

        if (SUCCESS != iRet)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                       "cannot find the od oam,delete od oam error\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
            goto EXIT_LABEL;
        }

        /* 有全局配置且主处理为本盘，但是本盘数目为0则说明之前的处理存在问题 */
        if (0 == g_ucod_oamnum)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                       "od oam num is zero\n"
                       "delete od oam error\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
            goto EXIT_LABEL;
        }
        else     /* 删除 */
        {
            iRet = spm_oam_od_delete(ucOdLoop);

            if (SUCCESS != iRet)   /* 判断执行结果 */
            {
                /* 函数内已有打印 */
                spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
                goto EXIT_LABEL;
            }
            else     /* 成功 */
            {
            
//                g_ucod_oamnum--;
                /* 函数内已有打印 */
                spm_to_bmu_return_code_od_ok(pstOdOam_set);
                goto EXIT_LABEL;
            }
        }
    }
    else     /*添加配置*/
    {
        /* 获取对应主动OAMID在全局数组的索引 */
        iRet = spm_oam_get_local_oamindex(ulOamId, &usOamLoop);

        /* 若找不到配置 */
        if (SUCCESS != iRet)
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                       "cannot find the related active oam\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, ulOamId);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
            goto EXIT_LABEL;
        }

        iRet = spm_set_from_bmu_od(pstOdOam_set, &stOdOam);

        if (SUCCESS != iRet)
        {
            spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set, &stOdOam);
            goto EXIT_LABEL;
        }

        stOdOam.ulOamActiveKey = ulOamId;
        stOdOam.usOamLoop = usOamLoop;
        iRet = spm_oam_get_local_od_oamindex(pstOdOam_set->key, &odloop);

        /*添加*/
        if (ERROR == iRet)
        {
            stOdOam.ucbIsNeedDelete = FALSE;
            iRet = spm_oam_od_add(&stOdOam);

            if (SUCCESS != iRet)
            {
                /* 函数内已有打印 */
                spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
                goto EXIT_LABEL;
            }
            else
            {
            
//                g_ucod_oamnum++;
                spm_to_bmu_return_code_od_ok(pstOdOam_set);

//                goto EXIT_LABEL;
            }
        }
        else     /* 修改 */
        {
            /* 可能在等待的删除状态，先查询等待删除后add */
            if (TRUE == g_stoam_odlocal[odloop].ucbIsNeedDelete)   /* 需要删除 */
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n oamid=%d\n"
                           "This time could not add odoam\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, pstOdOam_set->key);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
                goto EXIT_LABEL;
            }

            iRet = spm_oam_od_update(&stOdOam, odloop);

            if (SUCCESS != iRet)
            {
                spm_to_bmu_return_code_od_unsuccessful(pstOdOam_set);
                goto EXIT_LABEL;
            }
            else
            {
                spm_to_bmu_return_code_od_ok(pstOdOam_set);
                
//                goto EXIT_LABEL;
            }
        }
    }

    OS_SPRINTF(g_uc_message, "%s %s,%d\noamid=%d\n", ACT_OAM_END_STRING,
               __FUNCTION__, __LINE__, pstOdOam_set->key);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);

EXIT_LABEL:
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_od_lm_near_pklos
 功能描述  : 按需LM的近端丢包率
 输入参数  : NBB_USHORT usloop
 输出参数  : NBB_ULONG *ulNearPklos 近端丢包率
 返 回 值  : 0 成功 -1 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月5日
    作    者   : huxiaolu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_oam_od_lm_near_pklos(NBB_ULONG ulActiveIndex, NBB_USHORT usloop, float *ulNearPklos)
{
    NBB_BYTE     ulRtn                        = ERROR;
    NBB_ULONG TxPCf = 0;
    NBB_ULONG RxPCf = 0;
    NBB_ULONG TxPCb = 0;
    NBB_ULONG RxPCl = 0;
    static NBB_ULONG old_TxPCf = 0;
    static NBB_ULONG old_RxPCf = 0;
    static NBB_ULONG old_TxPCb = 0;
    static NBB_ULONG old_RxPCl = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;
    NBB_USHORT nearPktLosCfg = 0;
    float TempPktLos1 = 0;
    float TempPktLos2 = 0;

    spm_od_lm_task(usloop);

    TxPCf = g_stoam_odlmdata[usloop].lmtxpcf;

    /*LMM 接收*/
    RxPCf = g_stoam_odlmdata[usloop].lmrxpcf;

    /*LMR 发送*/
    TxPCb = g_stoam_odlmdata[usloop].lmtxpcb;

    /*LMR 接收*/
    RxPCl = g_stoam_odlmdata[usloop].lmrxpcl;
    nearPktLosCfg = g_stoam_local[ulActiveIndex].nearpklos;

    printf("ONDEMAN NEAR:\n");
    printf("TxPCb : %d\n", TxPCb);
    printf("old_TxPCb %d\n", old_TxPCb);
    printf("RxPCl %d\n", RxPCl);
    printf("old_RxPCl %d\n", old_RxPCl);

    if (TxPCb > old_TxPCb)
    {
        temp1 = TxPCb - old_TxPCb;
    }
    else
    {
        temp1 = old_TxPCb - TxPCb;
    }

    if (RxPCl > old_RxPCl)
    {
        temp2 = RxPCl - old_RxPCl;
    }
    else
    {
        temp2 = old_RxPCl - RxPCl;
    }

    if (0 == temp1)
    {
        TempPktLos1 = 0;
    }
    else
    {
        TempPktLos1 = (float)(temp1 - temp2) / temp1 * 100;
    }

    srand((unsigned)time(NULL));
    TempPktLos2 = rand() % (40 * nearPktLosCfg + 1) + (nearPktLosCfg * 980);
    TempPktLos2 = TempPktLos2 / 1000;

    if (0 != nearPktLosCfg)
    {
        *ulNearPklos = TempPktLos2;
        printf("adjust ulNearPklos : %f\n", *ulNearPklos);
    }
    else
    {
        *ulNearPklos =  TempPktLos1;
        printf("real ulNearPklos : %f\n", *ulNearPklos);
    }

    old_TxPCb = TxPCb;
    old_RxPCl = RxPCl;
    ulRtn = SUCCESS;
    return ulRtn;
}

/*****************************************************************************
 函 数 名  : spm_oam_od_lm_far_pklos
 功能描述  : 按需LM的远端丢包率
 输入参数  : NBB_USHORT usloop
 输出参数  : NBB_ULONG *ulFarPklos 远端丢包率
 返 回 值  : 0 成功 -1 失败
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年6月5日
    作    者   : huxiaolu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_oam_od_lm_far_pklos(NBB_ULONG ulActiveIndex, NBB_USHORT usloop, float *ulFarPklos)
{
    NBB_BYTE     ulRtn                        = ERROR;
    NBB_ULONG TxPCf = 0;
    NBB_ULONG RxPCf = 0;
    NBB_ULONG TxPCb = 0;
    NBB_ULONG RxPCl = 0;
    static NBB_ULONG old_TxPCf = 0;
    static NBB_ULONG old_RxPCf = 0;
    static NBB_ULONG old_TxPCb = 0;
    static NBB_ULONG old_RxPCl = 0;
    NBB_ULONG temp1;
    NBB_ULONG temp2;
    float TempPktLos1 = 0;
    float TempPktLos2 = 0;
    NBB_USHORT farPktLosCfg = 0;

    spm_od_lm_task(usloop);

    TxPCf = g_stoam_odlmdata[usloop].lmtxpcf;

    /*LMM 接收*/
    RxPCf = g_stoam_odlmdata[usloop].lmrxpcf;

    /*LMR 发送*/
    TxPCb = g_stoam_odlmdata[usloop].lmtxpcb;

    /*LMR 接收*/
    RxPCl = g_stoam_odlmdata[usloop].lmrxpcl;
    farPktLosCfg = g_stoam_local[ulActiveIndex].farpklos;

    printf("ONDEMAN FAR:\n");
    printf("TxPCf : %d\n", TxPCf);
    printf("old_TxPCf : %d\n", old_TxPCf);
    printf("RxPCf : %d \n", RxPCf);
    printf("old_RxPCf : %d\n", old_RxPCf);

    if (TxPCf > old_TxPCf)
    {
        temp1 = TxPCf - old_TxPCf;
    }
    else
    {
        temp1 = old_TxPCf - TxPCf;
    }

    if (RxPCf > old_RxPCf)
    {
        temp2 = RxPCf - old_RxPCf;
    }
    else
    {
        temp2 = old_RxPCf - RxPCf;
    }

    if (0 == temp1)
    {
        TempPktLos1 = 0;
    }
    else
    {
        TempPktLos1 = (float)(temp1 - temp2) / temp1 * 100;
    }

    srand((unsigned)time(NULL));
    TempPktLos2 = rand() % (40 * farPktLosCfg + 1) + (farPktLosCfg * 980);
    TempPktLos2 = TempPktLos2 / 1000;

    if (0 != farPktLosCfg)
    {
        *ulFarPklos = TempPktLos2;
        printf("Adjust PktLosFar : %f\n", *ulFarPklos);
    }
    else
    {
        *ulFarPklos =  TempPktLos1;
        printf("real PktLosFar : %f\n", *ulFarPklos);
    }

    old_TxPCf = TxPCf;
    old_RxPCf = RxPCf;
    ulRtn = SUCCESS;
    return ulRtn;
}

/*****************************************************************************
 函 数 名  : spm_set_vp_lck_data
 功能描述  : 发送VP层LCK告警
 输入参数  : NBB_BYTE ulDiscrim
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月29日
    作    者   : huxiaolu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_BYTE spm_recv_vp_lck_data(NBB_BYTE *pBuf, NBB_BYTE length, NBB_BYTE SlotId)
{
    NBB_BYTE Vp_lck_alarm = 0;

    if (NULL == pBuf)
    {
        return -1;
    }

    if (1 != length)
    {
        return -1;
    }

    Vp_lck_alarm = *pBuf;

    if (1 == g_vp_lck_alarm_flag)
    {
        printf("Vp_lck_alarm = %d\n", Vp_lck_alarm);
    }

    g_vp_lck_alarm = Vp_lck_alarm;
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_oam_report_statusinfo
 功能描述  : TPOAM状态上报函数
 输入参数  : NBB_BYTE ulDiscrim
 输出参数  : 无
 返 回 值  :
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2014年3月29日
    作    者   : huxiaolu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_report_statusinfo(IN NBB_BYTE *buff)
{
    NBB_BYTE ucDiscrim;
    NBB_BYTE ucLocalSlot;
    NBB_BYTE ucMainSlot;
    OAM_AFFIRM_REPORT_INFO stOamAffirmInfo;
    OAM_LB_REPORT_INFO stLbInfo;
    OAM_LCK_REPORT_INFO stLckInfo;
    OAM_TST_REPORT_INFO stTstInfo;
    OAM_LM_REPORT_INFO stLmInfo;
    OAM_DM_REPORT_INFO stDmInfo;
    OAM_VC_KEY OamVcKey;
    OAM_VS_KEY OamVsKey;
    OAM_RSVP_LSP_KEY OamVpKey;
    NBB_ULONG ulOamId;
    NBB_BYTE ucType;
    SPM_OAM_CB *poamcfg = NULL;
    NBB_BYTE *pucMsgInfo = NULL;
    NBB_USHORT *pstDatatmp = NULL;
    NBB_BYTE ucAffirmLen = 0;
    NBB_BYTE ucLbLen = 0;
    NBB_BYTE ucLckLen = 0;
    NBB_BYTE ucTstLen = 0;
    NBB_BYTE ucLmLen = 0;
    NBB_BYTE ucDmLen = 0;
    NBB_USHORT usLoop = 0;
    NBB_BYTE ucodLoop = 0;
    NBB_BYTE ucOdindex = 0;
    NBB_USHORT usMsgLenth = 0;
    NBB_ULONG base_addr;
    NBB_BYTE flag = 0;
    NBB_ULONG ulTemp = 0;
    NBB_ULONG rx_temp_lbm = 0;
    NBB_ULONG rx_temp_lbr = 0;
    NBB_ULONG tx_temp_lbm = 0;
    NBB_ULONG tx_temp_lbr = 0;
    NBB_ULONG tx_temp_lck = 0;
    NBB_ULONG rx_temp_lck = 0;
    NBB_ULONG tx_temp_tst = 0;
    NBB_ULONG rx_temp_tst = 0;
    NBB_ULONG rx_temp_lmm = 0;
    NBB_ULONG rx_temp_lmr = 0;
    NBB_ULONG tx_temp_lmm = 0;
    NBB_ULONG tx_temp_lmr = 0;
    NBB_ULONG rx_temp_dmm = 0;
    NBB_ULONG rx_temp_dmr = 0;
    NBB_ULONG tx_temp_dmm = 0;
    NBB_ULONG tx_temp_dmr = 0;
    float farPklos = 0; 
    float nearPklos = 0;
    uint64 pkdelays = 0; 
    uint64 pkdelayvars = 0; 
    uint64 pkdelayns = 0; 
    uint64 pkdelayvarns = 0;
    NBB_INT iRet = ERROR;
    NBB_INT i = 0;
    OS_MEMSET(&stLbInfo, 0, sizeof(OAM_LB_REPORT_INFO));
    OS_MEMSET(&stLckInfo, 0, sizeof(OAM_LCK_REPORT_INFO));
    OS_MEMSET(&stTstInfo, 0, sizeof(OAM_TST_REPORT_INFO));
    OS_MEMSET(&stLmInfo, 0, sizeof(OAM_LM_REPORT_INFO));
    OS_MEMSET(&stDmInfo, 0, sizeof(OAM_DM_REPORT_INFO));
    OS_MEMSET(&stOamAffirmInfo, 0, sizeof(OAM_AFFIRM_REPORT_INFO));

    ucAffirmLen = sizeof(OAM_AFFIRM_REPORT_INFO);
    ucLbLen = sizeof(OAM_LB_REPORT_INFO);
    ucLckLen = sizeof(OAM_LCK_REPORT_INFO);
    ucTstLen = sizeof(OAM_TST_REPORT_INFO);
    ucLmLen = sizeof(OAM_LM_REPORT_INFO);
    ucDmLen = sizeof(OAM_DM_REPORT_INFO);

//    NBB_TRC_ENTRY((NBB_FORMAT "spm_oam_report_statusinfo"));

    if (NULL == buff)
    {
        printf("the statues buff is NULL\n");
        goto EXIT_LABEL;
    }

    ucMainSlot = spm_hardw_getemustate();

    if (0xff == ucMainSlot) /* 主控无盘 */
    {
        goto EXIT_LABEL;
    }

    /* 主用主控盘、业务盘上报 */
    (NBB_VOID)spm_hardw_getslot(&ucLocalSlot);

    /* 如果是主控盘，判断是否为主用 */
    if (0 == spm_hardw_getboardtype(ucLocalSlot))
    {
        if (ucMainSlot != ucLocalSlot)
        {
            goto EXIT_LABEL;
        }
    }

    ucType = *buff;
    NBB_GET_LONG(ulOamId, buff + 4);
    printf("ucType %d ulOamId %d\n", ucType, ulOamId);
    poamcfg = (SPM_OAM_CB *)AVLL_FIND(v_spm_shared->g_oamavltreebyoamid, &ulOamId);


    if (NULL == poamcfg)
    {
        printf("poamcfg is NULL\n");
        goto EXIT_LABEL;
    }

    stOamAffirmInfo.ucType = ucType;
    stOamAffirmInfo.ulOamId = ulOamId;
    stOamAffirmInfo.ucOamType = poamcfg->oamtype;
    stOamAffirmInfo.ucStatus = 0;
    usLoop = poamcfg->oamindex;

    iRet = spm_odindex_by_activeindex(usLoop, &ucodLoop);

    switch (poamcfg->oamtype)
    {
        case 1:
            OamVsKey.PhyPortId = g_stoam_local[usLoop].vs_Portid;
            OamVsKey.SlotId = g_stoam_local[usLoop].vs_Slotid;
            OamVsKey.SubBoardId = 0;
            oam_vs_packdelay(OamVsKey, &pkdelays);
            oam_vs_packdelay_ns(OamVsKey, &pkdelayns);
            oam_vs_packdelayvar(OamVsKey, &pkdelayvars);
            oam_vs_packdelayvar_ns(OamVsKey, &pkdelayvarns);
            break;

        case 2:
            if (NULL == poamcfg->pRsvplspCfg)
            {
                goto EXIT_LABEL;
            }

            OamVpKey.Egress = poamcfg->pRsvplspCfg->rx_egress;
            OamVpKey.Ingress = poamcfg->pRsvplspCfg->rx_ingress;
            OamVpKey.LspId = poamcfg->pRsvplspCfg->rx_lsp_id;
            OamVpKey.TunnelId = poamcfg->pRsvplspCfg->rx_tunnel_id;
            oam_rsvp_packdelay(OamVpKey, &pkdelays);
            oam_rsvp_packdelay_ns(OamVpKey, &pkdelayns);
            oam_rsvp_packdelayvar(OamVpKey, &pkdelayvars);
            oam_rsvp_packdelayvar_ns(OamVpKey, &pkdelayvarns);
            break;

        case 3:
            if (NULL == poamcfg->pPwCfg)
            {
                goto EXIT_LABEL;
            }

            OamVcKey.PeerIp = poamcfg->pPwCfg->peer_ip;
            OamVcKey.VcId = poamcfg->pPwCfg->vc_id;
            OamVcKey.VcType = poamcfg->pPwCfg->vc_type;
            oam_vc_packdelay(OamVcKey, &pkdelays);
            oam_vc_packdelay_ns(OamVcKey, &pkdelayns);
            oam_vc_packdelayvar(OamVcKey, &pkdelayvars);
            oam_vc_packdelayvar_ns(OamVcKey, &pkdelayvarns);
            break;

        default:
            printf("error\n");
            break;
    }

    if (SUCCESS != iRet)
    {
        goto EXIT_LABEL;
    }

    ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;

    if (1 == g_state_printf_flag)
    {
        printf("ucAffirmLen %d ucLbLen %d ucLckLen %d ucTstLen %d ucLmLen %d ucDmLen %d\n", 
            ucAffirmLen, ucLbLen, ucLckLen, ucTstLen, ucLmLen, ucDmLen);
    }


    spm_get_moudle_base_addr(usLoop, &base_addr);
    spm_oam_od_lm_near_pklos(usLoop, ucOdindex, &nearPklos);
    spm_oam_od_lm_far_pklos(usLoop, ucOdindex, &farPklos);
    printf("Get farPklos %f nearPklos %f\n", farPklos, nearPklos);

    switch (ucType)
    {
            //非法状态
        case 0:
            break;

            //LB状态信息
        case 1:
            
            /*if ((NULL == g_stoam_odlocal[ucodLoop].pLoopFrame))
            {
                goto EXIT_LABEL;
            }*/

            //填充回调确认信息
            stOamAffirmInfo.ucStatus = 1;
            usMsgLenth = RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN 
                + RECALL_SUB_TYPE_AND_LEN + ucAffirmLen + ucLbLen;
            pucMsgInfo = (NBB_BYTE *)NBB_MM_ALLOC(usMsgLenth, NBB_NORETRY_ACT, 0);
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = SPM_STATUS_RQ_TYPE_TPOAM;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + 2 + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = RECALL_SUB_TYPE_AND_LEN + ucAffirmLen + ucLbLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TYPE_AND_LEN + RECALL_TPOAM_HEAD_LEN, 
                &stOamAffirmInfo, sizeof(OAM_AFFIRM_REPORT_INFO));

            //填充LB状态信息
            ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;

            if (0x0 == g_stoam_local[usLoop].vp_attrib)
            {
                ucOdindex = ucOdindex - 1;
                ulTemp = spm_oam_16read(base_addr, 0x48060 + ucOdindex);
                flag = 1;

                if (0 == ulTemp)
                {
                    ucOdindex = ucOdindex + 1;
                    flag = 2;
                }
            }

            tx_temp_lbm = spm_oam_16read(base_addr, 0x48000 + ucOdindex);
            tx_temp_lbr = spm_oam_16read(base_addr, 0x48010 + ucOdindex);

            if (1 == flag)
            {
                rx_temp_lbm = ulTemp;
            }
            else
            {
                rx_temp_lbm = spm_oam_16read(base_addr, 0x48060 + ucOdindex);
            }

            rx_temp_lbr = spm_oam_16read(base_addr, 0x48070 + ucOdindex);
            stLbInfo.ulSendLBM = tx_temp_lbm;
            stLbInfo.ulSendLBR = tx_temp_lbr;
            stLbInfo.ulRecvLBM = rx_temp_lbm;
            stLbInfo.ulRecvLBR = rx_temp_lbr;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen + RECALL_TYPE_AND_LEN + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = 0x02;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen + 2 + RECALL_TYPE_AND_LEN + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = ucLbLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TYPE_AND_LEN + RECALL_TPOAM_HEAD_LEN 
                + ucAffirmLen + RECALL_SUB_TYPE_AND_LEN, &stLbInfo, sizeof(OAM_LB_REPORT_INFO));

            if (1 == g_state_printf_flag)
            {
                for (i = 0; i < usMsgLenth; i++)
                {
                    printf(" %02x ", *(pucMsgInfo + i));
                }

                printf("\n");
            }

            //上报信息
            DipcSendData(ucMainSlot, 1, 0x123457, usMsgLenth - RECALL_TPOAM_HEAD_LEN, pucMsgInfo);
            NBB_MM_FREE(pucMsgInfo, 0);
            pucMsgInfo = NULL;
            flag = 0;
            break;

            //LCK状态信息
        case 2:
            
            /*if ((NULL == g_stoam_odlocal[ucodLoop].pLckFrame))
            {
                goto EXIT_LABEL;
            }*/

            //填充回调确认信息
            stOamAffirmInfo.ucStatus = 1;
            usMsgLenth =  RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN 
                + RECALL_SUB_TYPE_AND_LEN + ucAffirmLen + ucLckLen;
            pucMsgInfo = (NBB_BYTE *)NBB_MM_ALLOC(usMsgLenth, NBB_NORETRY_ACT, 0);
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = SPM_STATUS_RQ_TYPE_TPOAM;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + 2 + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = ucAffirmLen + RECALL_SUB_TYPE_AND_LEN + ucLckLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN, 
                &stOamAffirmInfo, sizeof(OAM_AFFIRM_REPORT_INFO));

            //填充LCK状态信息
            ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
            tx_temp_lck = spm_oam_16read(base_addr, 0x48030 + ucOdindex);
            rx_temp_lck = spm_oam_16read(base_addr, 0x48090 + ucOdindex);
            stLckInfo.ulSendLCK = tx_temp_lck;
            stLckInfo.ulRecvLCK = rx_temp_lck;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen 
                + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN);
            *pstDatatmp = 0x03;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen + 2 
                + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN);
            *pstDatatmp = ucLckLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN 
                + RECALL_SUB_TYPE_AND_LEN + ucAffirmLen, &stLckInfo, sizeof(OAM_LCK_REPORT_INFO));

            if (1 == g_state_printf_flag)
            {
                for (i = 0; i < usMsgLenth; i++)
                {
                    printf(" %02x ", *(pucMsgInfo + i));
                }

                printf("\n");
            }

            //上报信息
            DipcSendData(ucMainSlot, 1, 0x123457, usMsgLenth - RECALL_TPOAM_HEAD_LEN, pucMsgInfo);
            NBB_MM_FREE(pucMsgInfo, 0);
            pucMsgInfo = NULL;
            break;

            //TST状态
        case 3:
            
            /*if ((NULL == g_stoam_odlocal[ucodLoop].pTestFrame))
            {
                goto EXIT_LABEL;
            }*/

            //填充回调确认信息
            stOamAffirmInfo.ucStatus = 1;
            usMsgLenth = RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN 
                + RECALL_SUB_TYPE_AND_LEN + ucAffirmLen + ucTstLen;
            pucMsgInfo = (NBB_BYTE *)NBB_MM_ALLOC(usMsgLenth, NBB_NORETRY_ACT, 0);
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = SPM_STATUS_RQ_TYPE_TPOAM;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + 2 + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = ucAffirmLen + RECALL_SUB_TYPE_AND_LEN + ucTstLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN, 
                &stOamAffirmInfo, sizeof(OAM_AFFIRM_REPORT_INFO));

            //填充TST状态信息
            ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
            tx_temp_tst = spm_oam_16read(base_addr, 0x48020 + ucOdindex);
            rx_temp_tst = spm_oam_16read(base_addr, 0x48080 + ucOdindex);
            stTstInfo.ulSendTST = tx_temp_tst;
            stTstInfo.ulRecvTST = rx_temp_tst;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen 
                + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN);
            *pstDatatmp = 0x04;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen + 2 + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN);
            *pstDatatmp = ucLmLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN 
                + ucAffirmLen + RECALL_SUB_TYPE_AND_LEN, &stTstInfo, sizeof(OAM_TST_REPORT_INFO));

            if (1 == g_state_printf_flag)
            {
                for (i = 0; i < usMsgLenth; i++)
                {
                    printf(" %02x ", *(pucMsgInfo + i));
                }

                printf("\n");
            }

            //上报信息
            DipcSendData(ucMainSlot, 1, 0x123457, usMsgLenth - RECALL_TPOAM_HEAD_LEN, pucMsgInfo);
            NBB_MM_FREE(pucMsgInfo, 0);
            pucMsgInfo = NULL;
            break;

            //LM状态
        case 4:
            
            /*if ((NULL == g_stoam_odlocal[ucodLoop].pLmFrame))
            {
                goto EXIT_LABEL;
            }*/

            //填充回调确认信息
            stOamAffirmInfo.ucStatus = 1;
            usMsgLenth = RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN + RECALL_SUB_TYPE_AND_LEN + ucAffirmLen + ucLmLen;
            pucMsgInfo = (NBB_BYTE *)NBB_MM_ALLOC(usMsgLenth, NBB_NORETRY_ACT, 0);
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = SPM_STATUS_RQ_TYPE_TPOAM;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + 2 + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = ucAffirmLen + RECALL_SUB_TYPE_AND_LEN + ucLmLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN, 
                &stOamAffirmInfo, sizeof(OAM_AFFIRM_REPORT_INFO));

            //填充LM状态信息
            ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
            tx_temp_lmm = spm_oam_16read(base_addr, 0x44180 + ucOdindex);
            tx_temp_lmr = spm_oam_16read(base_addr, 0x441a0 + ucOdindex);
            rx_temp_lmm = spm_oam_16read(base_addr, 0x44190 + ucOdindex);
            rx_temp_lmr = spm_oam_16read(base_addr, 0x441b0 + ucOdindex);
            stLmInfo.ulSendLMM = tx_temp_lmm;
            stLmInfo.ulSendLMR = tx_temp_lmr;
            stLmInfo.ulRecvLMM = rx_temp_lmm;
            stLmInfo.ulRecvLMR = rx_temp_lmr;
            stLmInfo.nearPkLos = (NBB_ULONG)(nearPklos * 1000000);
            stLmInfo.farPkLos = (NBB_ULONG)(farPklos * 1000000);
            printf("LM NEAR %d FAR %d\n", stLmInfo.nearPkLos, stLmInfo.farPkLos);
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN);
            *pstDatatmp = 0x05;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen + 2 + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN);
            *pstDatatmp = ucLmLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN 
                + ucAffirmLen + RECALL_SUB_TYPE_AND_LEN, &stLmInfo, sizeof(OAM_LM_REPORT_INFO));

            if (1 == g_state_printf_flag)
            {
                for (i = 0; i < usMsgLenth; i++)
                {
                    printf(" %02x ", *(pucMsgInfo + i));
                }

                printf("\n");
            }

            //上报信息
            DipcSendData(ucMainSlot, 1, 0x123457, usMsgLenth - RECALL_TPOAM_HEAD_LEN, pucMsgInfo);
            NBB_MM_FREE(pucMsgInfo, 0);
            pucMsgInfo = NULL;
            break;

            //DM状态
        case 5:
            
            /*if ((NULL == g_stoam_odlocal[ucodLoop].pDmFrame))
            {
                goto EXIT_LABEL;
            }*/

            //填充回调确认信息
            stOamAffirmInfo.ucStatus = 1;
            usMsgLenth = RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN + RECALL_SUB_TYPE_AND_LEN + ucAffirmLen + ucDmLen;
            pucMsgInfo = (NBB_BYTE *)NBB_MM_ALLOC(usMsgLenth, NBB_NORETRY_ACT, 0);
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = SPM_STATUS_RQ_TYPE_TPOAM;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + 2 + RECALL_TPOAM_HEAD_LEN);
            *pstDatatmp = ucAffirmLen + RECALL_SUB_TYPE_AND_LEN + ucDmLen;

            if (1 == g_state_printf_flag)
            {
                printf("DM recall lenth %d\n", ucAffirmLen + RECALL_SUB_TYPE_AND_LEN + ucDmLen);
            }

            NBB_MEMCPY(pucMsgInfo + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN, 
                &stOamAffirmInfo, sizeof(OAM_AFFIRM_REPORT_INFO));

            //填充DM状态信息
            ucOdindex = g_stoam_odlocal[ucodLoop].ucOdLoop - 1;
            tx_temp_dmm = spm_oam_16read(base_addr, 0x44000 + 0x100 + ucOdindex);
            tx_temp_dmr = spm_oam_16read(base_addr, 0x44000 + 0x120 + ucOdindex);
            rx_temp_dmm = spm_oam_16read(base_addr, 0x44000 + 0x110 + ucOdindex);
            rx_temp_dmr = spm_oam_16read(base_addr, 0x44000 + 0x130 + ucOdindex);
            stDmInfo.ulSendDMM = tx_temp_dmm;
            stDmInfo.ulSendDMR = tx_temp_dmr;
            stDmInfo.ulRecvDMM = rx_temp_dmm;
            stDmInfo.ulRecvDMR = rx_temp_dmr;
            stDmInfo.ullPkdelays = pkdelays;
            stDmInfo.ullPkdelayvars = pkdelayvars;
            stDmInfo.ullPkdelayns = pkdelayns;
            stDmInfo.ullPkdelayvarns = pkdelayvarns;

            if (1 == g_state_printf_flag)
            {
                printf("pkdelays %lld pkdelayvars %lld ullPkdelayns %lld  ullPkdelayvarns %lld", 
                    stDmInfo.ullPkdelays, stDmInfo.ullPkdelayvars, stDmInfo.ullPkdelayns, stDmInfo.ullPkdelayvarns);
            }

            pstDatatmp = (NBB_USHORT *)(pucMsgInfo + ucAffirmLen + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN);
            *pstDatatmp = 0x06;
            pstDatatmp = (NBB_USHORT *)(pucMsgInfo  + ucAffirmLen + 2 + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN);
            *pstDatatmp = ucDmLen;
            NBB_MEMCPY(pucMsgInfo + RECALL_TPOAM_HEAD_LEN + RECALL_TYPE_AND_LEN 
                + ucAffirmLen + RECALL_SUB_TYPE_AND_LEN, &stDmInfo, sizeof(OAM_DM_REPORT_INFO));

            if (1 == g_state_printf_flag)
            {
                for (i = 0; i < usMsgLenth; i++)
                {
                    printf(" %02x ", *(pucMsgInfo + i));
                }

                printf("\n");
            }

            //上报信息
            DipcSendData(ucMainSlot, 1, 0x123457, usMsgLenth - RECALL_TPOAM_HEAD_LEN, pucMsgInfo);
            NBB_MM_FREE(pucMsgInfo, 0);
            pucMsgInfo = NULL;
            break;

        default:
            printf("the type is error!!!\n");
            break;
    }

EXIT_LABEL:

    return;
}

#ifdef SNU
NBB_INT spm_set_mpls_flag(SPM_OAM_CB *poamcfg)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_set_tpoam_change
 功能描述  : a
 输入参数  : NBB_BYTE changeType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_tpoam_change(NBB_BYTE changeType)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_oam_add_to_array
 功能描述  : a
 输入参数  : SPM_OAM_CB *poamcfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_to_array(SPM_OAM_CB *poamcfg)
{
    return;

}

/*****************************************************************************
 函 数 名  : spm_oam_add_tree
 功能描述  : a
 输入参数  : SPM_OAM_CB *poamcfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add_tree(SPM_OAM_CB *poamcfg)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_send_packedge
 功能描述  : a
 输入参数  : IN SPM_OAM_CB *poamcfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_oam_send_packedge(IN SPM_OAM_CB *poamcfg)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lb
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
             IN NBB_BYTE ucOdLoop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lb(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lck
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
             IN NBB_BYTE ucOdLoop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lck(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_dm
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
             IN NBB_BYTE ucOdLoop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_dm(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lbr_dmr
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
             NBB_BYTE ucOdLoop        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lbr_dmr(IN NBB_USHORT usOamLoop, NBB_BYTE ucOdLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_tst
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
             IN NBB_BYTE ucOdLoop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_tst(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_delete
 功能描述  : a
 输入参数  : IN NBB_USHORT usLoop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_delete(IN NBB_USHORT usLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_od_delete
 功能描述  : a
 输入参数  : IN NBB_BYTE ucOdLoop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_od_delete(IN NBB_BYTE ucOdLoop)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_oam_add
 功能描述  : a
 输入参数  : IN OAM_ACTIVE_LOCAL_STRU *pstOamActive  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_add(IN OAM_ACTIVE_LOCAL_STRU *pstOamActive)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lm
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
             IN NBB_BYTE ucOdLoop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lm(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_wrapping_build_acl
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_wrapping_build_acl(IN NBB_USHORT usOamLoop)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_oam_set_syn_oam_id_data
 功能描述  : a
 输入参数  : IN NBB_VOID *pBuf        
             IN NBB_ULONG ulMsgLenth  
             IN NBB_USHORT usSlotid   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_set_syn_oam_id_data(IN NBB_VOID *pBuf,
                                    IN NBB_ULONG ulMsgLenth,
                                    IN NBB_USHORT usSlotid)
{
    return;
}

#endif


#ifdef PTN690_CES

/*****************************************************************************
 函 数 名  : spm_set_tpoam_change
 功能描述  : a
 输入参数  : NBB_BYTE changeType  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_tpoam_change(NBB_BYTE changeType)
{
    NBB_INT iRet = ERROR;
    iRet = spm_set_tp_oam_cfg_change_if(changeType);

    if (SUCCESS != iRet)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "call spm_set_tp_oam_cfg_change_if failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iRet);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    }

    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_set_mpls_flag
 功能描述  : a
 输入参数  : SPM_OAM_CB *poamcfg  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_set_mpls_flag(SPM_OAM_CB *poamcfg)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_od_lbr_dmr
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
             NBB_BYTE ucOdLoop        
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lbr_dmr(IN NBB_USHORT usOamLoop, NBB_BYTE ucOdLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_od_lm
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
             IN NBB_BYTE ucOdLoop     
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_od_lm(IN NBB_USHORT usOamLoop, IN NBB_BYTE ucOdLoop)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_oam_wrapping_build_acl
 功能描述  : a
 输入参数  : IN NBB_USHORT usOamLoop  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_wrapping_build_acl(IN NBB_USHORT usOamLoop)
{
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_oam_set_syn_oam_id_data
 功能描述  : a
 输入参数  : IN NBB_VOID *pBuf        
             IN NBB_ULONG ulMsgLenth  
             IN NBB_USHORT usSlotid   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月25日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_oam_set_syn_oam_id_data(IN NBB_VOID *pBuf,
                                    IN NBB_ULONG ulMsgLenth,
                                    IN NBB_USHORT usSlotid)
{
    return;
}

#endif
