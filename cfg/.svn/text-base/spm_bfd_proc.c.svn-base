/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_bfd_proc.c
  版 本 号   : 初稿
  作    者   : limingming
  生成日期   : 2012年9月24日
  最近修改   :
  功能描述   : BFD配置处理
  函数列表   :
  修改历史   :
  1.日    期   : 2012年9月24日
    作    者   : limingming
    修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include <c3api.h> 
#include "bmuInclude.h"
BMU_WDOG_ID g_stBfdWdog; /* BFD定时器 */ 
BMU_MSG_Q_ID g_stBfdMsgQID;
extern SPM_BOARD_INF    g_stHardwBoardInf;

/* 调试信息打印开关 */
extern BFD_DEBUGSHOW g_enBfdDebug;

/* 清配置标记 */
extern NBB_BYTE g_ucCleanCfgCheck;

/* fwdtable 翻页写备表的次数 */
extern NBB_UINT g_uiFwdTableCount;

/* 操作fpga转发二叉树读写锁 */
extern BMU_RWLOCK_ID g_build_fpga_fwd_lock;

/*****************************************************************************
 函 数 名  : spm_bfd_init
 功能描述  : BFD模块初始化函数
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月27日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

  2.日    期   : 2015年9月9日
    作    者   : gaos
    修改内容   : 添加设置日志文件的大小
*****************************************************************************/
NBB_INT spm_bfd_init(NBB_CXT_T NBB_CXT)
{
    NBB_INT   iRet = ERROR;
    NBB_ULONG ulLoopi = 0;
    NBB_ULONG ulLoopj = 0;
    
    NBB_TRC_ENTRY(("spm_bfd_init"));
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "call spm_bfd_init\r\n");
    bmu_set_log_size(SPM_BFD_LOG_DIR, 1024 * 1024 * 20);//20M
    
    /* 初始化 */
    g_bIsReadySendtoMaster = FALSE;
    g_bIsModifySession = FALSE;
    g_ulSequenceID = 0;
    g_ucLocalSlot = 0;

    g_usBfdTreeLevel  = BFD_TREE_LEVEL;

    g_usBfdTreeNodeMaxnum = (1 << g_usBfdTreeLevel);
    g_usBfdLspNoMax   = g_usBfdTreeNodeMaxnum - 1;
    g_usBfdNum = 0;

    g_ulStateToDownNum = 0;
    g_usRecordNum = 0;
    NBB_MEMSET(g_aucCouterRecord, 0, BFD_MAX_SESNUM * sizeof(NBB_BYTE));
    g_pulBfdTreebase = (NBB_ULONG *)NBB_MM_ALLOC(sizeof(NBB_ULONG)*g_usBfdTreeLevel, 
        NBB_NORETRY_ACT, MEM_SPM_BFD_PUL_TREE_BASE_CB);

    /* 判断内存 */
    if (NULL == g_pulBfdTreebase)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Malloc error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Malloc error\r\n");
        goto EXIT_LABEL;
    }
    NBB_MEMSET(g_pulBfdTreebase, 0, sizeof(NBB_ULONG)*g_usBfdTreeLevel);

    g_ulSynIdInfoLth = sizeof(SPM_BFD_SYN_BFD_ID) + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD);
    g_pucSynIdInfo = (NBB_BYTE *)NBB_MM_ALLOC(g_ulSynIdInfoLth, NBB_NORETRY_ACT, MEM_SPM_BFD_PUC_SYN_ID_INFO_CB);

    if (NULL == g_pucSynIdInfo)/* 申请失败 */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Syn Id, malloc failed!"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Syn Id, malloc failed!\r\n");
        goto EXIT_LABEL;
    }
    
    /* 二叉树初始化 */    
    iRet = spm_oamtree_init(g_usBfdTreeNodeMaxnum, spm_bfd_tree_get_fpga_data, &g_stTreeBfdNodeSet);

    /* 判断执行结果 */
    if (SUCCESS != iRet)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Bfd node set error, nodenum = %d", g_usBfdTreeNodeMaxnum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Bfd node set error, nodenum = %d\r\n", g_usBfdTreeNodeMaxnum);
        goto EXIT_LABEL;
    }

    
    /* 初始化BFD配置存储表项 */
    for (ulLoopi = 0; ulLoopi < BFD_MAX_SESNUM; ulLoopi++)/* 初始化 */
    {
        BFD_LOCAL_INIT(ulLoopi);
    }

    for (ulLoopi = 0; ulLoopi < SYS_MAX_SLOTNUM; ulLoopi++)
    {
        for (ulLoopj = 0; ulLoopj < BFD_MAX_SESNUM; ulLoopj++)
        {
            g_stBfdIdStore[ulLoopi].stBfdIdInfo[ulLoopj].ulDiscrimer = DRV_ULONG_INVALID;
        }
    }
    
    /* 初始化全局配置表项 ，树结构初始化 */
    AVLL_INIT_TREE(g_stBfdGlbTreeByDis, 
        compare_ulong,
        NBB_OFFSETOF(SPM_BFD_GLOBAL, ulDiscrimer),
        NBB_OFFSETOF(SPM_BFD_GLOBAL, spm_bfd_node));

    AVLL_INIT_TREE(g_stBfdGlbTreeByIp,
        spm_bfd_key_compare,
        NBB_OFFSETOF(SPM_BFD_IP_GLOBAL, stBfdGlbKey),
        NBB_OFFSETOF(SPM_BFD_IP_GLOBAL, spm_bfdip_node));
    /* add begin by limingming, 2013/5/3, Mantis号:HSDIPV6BFD, 原因:实现IPV6BFD */
    AVLL_INIT_TREE(g_stBfdGlbTreeByIpv6,
        spm_bfd_ipv6key_compare,
        NBB_OFFSETOF(SPM_BFD_IPV6_GLOBAL, stBfdGlbKey),
        NBB_OFFSETOF(SPM_BFD_IPV6_GLOBAL, spm_bfdipv6_node));
    /* add end by limingming, 2013/5/3 */

    AVLL_INIT_TREE(g_stFwdGlbTreeByDis, 
        compare_ulong,
        NBB_OFFSETOF(SPM_FWD_GLOBAL, ulDiscrimer),
        NBB_OFFSETOF(SPM_FWD_GLOBAL, spm_fwd_node));

    /* 初试化失败告警树 */
    AVLL_INIT_TREE(g_stAlarmFailTree,
        compare_ulong,
        NBB_OFFSETOF(BFD_ALARM_FAIL_INFO, ulBfdId),
        NBB_OFFSETOF(BFD_ALARM_FAIL_INFO, alarm_fail_node));
    
    /* 初始化调试信息模块 */
    spm_bfd_init_dbg(NBB_CXT);

    /* 若为业务盘，则初始化转发表 */
    iRet = spm_bfd_init_fwdtable(NBB_CXT);

    /* 判断执行结果 */
    if (SUCCESS != iRet)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Fwd init error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd init error\r\n");
        goto EXIT_LABEL;
    }

    iRet = spm_bfd_init_Nni_table(NBB_CXT);
    if (SUCCESS != iRet)
    {
        NBB_TRC_FLOW((NBB_FORMAT "NNI table init error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "NNI table init error\r\n");
        goto EXIT_LABEL;
    }
    
    g_stBfdWdog = BmuWdCreate();

    g_usAlarmInfoLenth = 24 + 4 + 2 + BFD_MAX_SESNUM * BFD_ALARM_LENTH;
    g_pucAlarmInfo = (NBB_BYTE *)NBB_MM_ALLOC(g_usAlarmInfoLenth, 
        NBB_NORETRY_ACT, MEM_SPM_BFD_PUC_ALARM_INFO_CB);
    if (NULL == g_pucAlarmInfo)/* 申请失败 */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Alarm info, malloc failed!"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Alarm info, malloc failed!\r\n");
        goto EXIT_LABEL;
    }

    g_ucFirstNpBoard = 0;
    g_ucMainEmuId = 0xff;
    NBB_MEMSET(g_astBoardState, 0, 10 * sizeof(BFD_BOARD_INFO));

    /* 初始化读写锁 */
    g_build_fpga_fwd_lock = BmuRWLockCreate(BMU_SEM_Q_FIFO);
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_clean_cfg_fwdtree
 功能描述  : 删除转发配置，清配置时调用
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年9月15日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_clean_cfg_fwdtree( NBB_VOID )
{
    NBB_INT iRet = ERROR;
    SPM_FWD_GLOBAL          *pstFwdInfo  = NULL;
    NBB_USHORT usLoopi = 0;
        
    pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIRST(g_stFwdGlbTreeByDis);
    for (pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIRST(g_stFwdGlbTreeByDis);
        pstFwdInfo != NULL;
        pstFwdInfo = (SPM_FWD_GLOBAL*) AVLL_FIRST(g_stFwdGlbTreeByDis)) 
        {
            /* 删除全局转发配置 */
            AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
            OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by 蒋博,jiangbo, 20131113, Mantis号:无,
                                                                原因:因为不由nbase管理进程也会分配、释放这块内
                                                                存，不能使用NBB_MM_FREE管理，同时不能被verify */
            pstFwdInfo = NULL;
      
        }

    /* 本地二叉树全部置为默认值 */
    iRet = spm_oamtree_init(g_usFwdTreeNodeMaxnum, spm_fwd_tree_get_fpga_data, &g_stTreeFwdNodeSet);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Fwd node set error, nodenum = %d", g_usFwdTreeNodeMaxnum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd node set error, nodenum = %d\r\n", g_usFwdTreeNodeMaxnum);
        goto EXIT_LABEL;
    }

    /* 清空fpga发送寄存器，本地转发数组置为无效值 */
    for (usLoopi = 0; usLoopi < BFD_MAX_FWDNUM; usLoopi++)
    {
        /* 将会话信息至为无效值 */
        BFD_FWD_INIT(usLoopi);
        
        /* 清空FPGA对应的发送寄存器内容 */
        spm_bfd_clear_fwdfpgareg(usLoopi);
    }
    
    /* 重建fpga转发会话二叉树 */   
    iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
        g_pulFwdTreebase, OAMFWDFPAGBASE);
    
    /* 翻页写备表 */
    spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);
    
    
    iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
        g_pulFwdTreebase, OAMFWDFPAGBASE);
    
    /* 翻页 */
    spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);     
    
    /* 本地转发条目置为0 */
    g_usFwdNum = 0;
  
    iRet = SUCCESS;
EXIT_LABEL :
    return iRet;
}
    
/*****************************************************************************
 函 数 名  : spm_bfd_clean_cfg_tree
 功能描述  : 删除配置，清配置时调用
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年9月15日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_clean_cfg_tree()
{
    NBB_INT iRet = 0;
    
    iRet = spm_bfd_clean_cfg_fwdtree();
    
    return OK;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_clean_all_cfg
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年5月28日
 * 函数功能  : 删除BFD所有配置
 * 输入参数  : NBB_CXT_T NBB_CXT  NBASE多线程参数
 * 输出参数  : 无
 * 返 回 值  : NBB_INT  返回值
 * 调用关系  : 
 * 其    它  : 

  1.日    期   : 2015年9月15日
    作    者   : gaos
    修改内容   : 修改
*****************************************************************************/
NBB_INT spm_bfd_clean_all_cfg(NBB_CXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    SPM_BFD_GLOBAL          *pstBfdCfg = NULL;
    
    NBB_TRC_ENTRY(("spm_bfd_clean_all_cfg"));
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "##spm_bfd_clean_all_cfg begin!\r\n");
    
    /*  清配置标记mantis:CR8000 V1R1-ER版本中试测试 0083509*/
    g_ucCleanCfgCheck = 1;

    for (pstBfdCfg = (SPM_BFD_GLOBAL*) AVLL_FIRST(g_stBfdGlbTreeByDis);
        pstBfdCfg != NULL;
        pstBfdCfg = (SPM_BFD_GLOBAL*) AVLL_FIRST(g_stBfdGlbTreeByDis))
    {

        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s: clean cfg,del session %lu begin, mainboard:%d!\r\n",
            __FUNCTION__, pstBfdCfg->ulDiscrimer, pstBfdCfg->usSlotid);
        (NBB_VOID)spm_bfd_del_cfg(pstBfdCfg NBB_CCXT); 
             
    }
        
    /*  对fwd table 删除方式标记mantis:CR8000 V1R1-ER版本中试测试 0083509*/
    g_ucCleanCfgCheck = 0;   
    
//    spm_bfd_clean_cfg_tree();    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "##spm_bfd_clean_all_cfg end!\r\n");

    iRet = SUCCESS;
EXIT_LABEL: NBB_TRC_EXIT();
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_bfd
 功能描述  : BFD配置处理模块
 输入参数  : atg_dci_set_bfd_session *set_bfd  : IPS消息
             NBB_CCXT_T NBB_CCXT               
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月24日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数
 备注          : 对于会话，软件存储用了两棵树，一棵是以描述符作为key值，一棵以SIP+DIP+vrfid做为key值。
                 其中，描述符做为key的，用来针对bfd for ldp lsp，passive端查询建立转发表的；
                 SIP+DIP+VRFID只记录bfd for ip且remotediscrim 不为0的情况
                 在add过程中若执行不成功，所有资源都将回退
*****************************************************************************/
NBB_VOID spm_rcv_dci_set_bfd(ATG_DCI_SET_BFD_SESSION *pstset_bfd NBB_CCXT_T NBB_CXT)
{
#ifdef SNU
    pstset_bfd->return_code = ATG_DCI_RC_OK;
    return;
#endif

    ATG_DCI_BFD_BASIC_DATA  *pstBasicBfdCB = NULL;
    ATG_DCI_BFD_RSVP_LSP_DATA *pstBasicBfdTunnel = NULL;
    ATG_DCI_BFD_LDP_LSP_DATA    *pstBasicBfdLsp = NULL;
    ATG_DCI_BFD_PW_DATA     *pstBasicBfdPw = NULL;
    SPM_BFD_GLOBAL          *pstBfdCfg = NULL;
    SPM_BFD_IP_GLOBAL       *pstBfdIpCfg = NULL;
    SPM_BFD_IPV6_GLOBAL     *pstBfdIpv6Cfg = NULL;
    SPM_BFD_IP_GLOBAL       *pstbfmtlfgtmp = NULL;
    SPM_BFD_IPV6_GLOBAL     *pstbfdipv6cfgtmp = NULL;
    SPM_FWD_GLOBAL          *pstFwdInfo  = NULL;
    SPM_BFDGLB_IP_KEY       stBfdGlbKey;
    SPM_BFDGLB_IPV6_KEY     stBfdIpv6GlbKey;
    NBB_INT                 iRet = ERROR;   /* 执行返回值 */
    NBB_USHORT              usLoopi = 0;
    NBB_ULONG   ulDiscrimer = 0;
    NBB_USHORT  usMainBoardId = 0;          /* 会话主处理盘 */
    NBB_BYTE    ucLocalSlot   = 0;          /* 本盘槽位号 */
    NBB_BYTE    ucSlotId      = 0;          /* 根据portindex查询的slotid */
    NBB_BYTE    ucPortType    = 0;          /* 根据portindex查询的接口类型 */
    NBB_ULONG   ulPortIndex   = 0;          /* 迭代结果的出口索引 */
    NBB_USHORT  usPortId      = 0;          /* 迭代结果，如果是LAG，选成员端口 */
    NBB_ULONG   aulLagPortIndex[ATG_DCI_LAG_MEMBER_PORT_NUM] = {0};     /* LAG成员端口 */
    NBB_BYTE    ucLagNum = 0;               /* LAG成员端口个数 */
    NBB_ULONG   ulDriverIndex = 0;          /* 迭代结果，tunnelid或者是lspid */
    CRTXLSP_KEY stCrlspKey;
    FTN_KEY     stLdplspKey;
    NBB_USHORT  usloopi = 0;

    NBB_BYTE *pucIpsMsgStar = NULL;         /* IPS消息地址 */
    SPM_VP_INFO_CB stVcInfo;
    ATG_DCI_VC_KEY stVcKey;    
    SPM_PORT_INFO_CB stPortInfo;
    
    #ifdef SPU
    NBB_BYTE c3_unit = 0;
    #endif

    NBB_TRC_ENTRY(("spm_rcv_dci_set_bfd"));

    /* 入参判断 */
    if (NULL == pstset_bfd)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stBfdGlbKey, 0, sizeof(SPM_BFDGLB_IP_KEY));
    OS_MEMSET(&stBfdIpv6GlbKey, 0, sizeof(SPM_BFDGLB_IPV6_KEY));

    /*  配置增删时不进行转发表的添加*/
    g_ucCleanCfgCheck = 1;

    /* 删除会话 */
    if (TRUE == pstset_bfd->delete_struct)
    {
        spm_bfd_dci_set_bfd_del(pstset_bfd);
    }
    else /* 添加情况 */
    {
        /* 获取本盘槽位 */
        (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
        
        ulDiscrimer = pstset_bfd->key;
        pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulDiscrimer);
        if (NULL == pstBfdCfg)/* 说明情况是add */
        {
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add session %ld\r\n", pstset_bfd->key);

            if (ATG_DCI_OPER_NULL == pstset_bfd->oper_basic)
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Congfig is null\r\n");
                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

            pucIpsMsgStar = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstset_bfd, &pstset_bfd->basic_data);
            NBB_ASSERT(NULL != pucIpsMsgStar);
            
            pstBasicBfdCB = (ATG_DCI_BFD_BASIC_DATA *)pucIpsMsgStar;

            if(NULL == pstBasicBfdCB)
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd basic CB is null\r\n");
                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

            /* 解析该会话的主处理盘 */
            if (BFD_CFG_IP == pstBasicBfdCB->bfd_frame_type)/* for ip */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is for ip\r\n");

                if (1 == pstBasicBfdCB->detect_method) /* 多跳情况，主处理盘为主控 */
                {
                    usMainBoardId = spm_hardw_getemustate();
                    if (0xff == usMainBoardId)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Mutiple session,but src is absent\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                }
                else
                {
                    /* add begin by limingming, 2013/2/5, Mantis号:HSD111111, 原因:BFD会话选盘 */
                    /* BFD FOR BGP的情况主控获取不到出接口，需要单盘根据IP查找出接口，并且这种情况BFD肯定是在路由之后下发
                       的，因为只有建立TCP才会建立BFD */
                    if ((0x7fffffff == pstBasicBfdCB->phy_if_index) || (0 == pstBasicBfdCB->phy_if_index))
                    {
                        if (BFD_IPV4 == pstBasicBfdCB->ip_type) 
                        {
                            /* 根据vrfid+dip查询FTN迭代出端口 */
                            iRet = spm_l3_routeforoam(pstBasicBfdCB->vrf_id, 0, &(pstBasicBfdCB->remote_ip[3]), 
                                &ulPortIndex);
                            if (SUCCESS != iRet)/* 若执行失败 */
                            {
                                NBB_TRC_FLOW((NBB_FORMAT"get ftn error, vrf_id = %ld remote_ip = %ld",
                                        pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[3]));
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                        "Get ftn error %d, vrf_id = %ld remote_ip = %lx\r\n", 
                                        iRet, pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[3]);

                                /* 上报配置失败消息，只有BFD FOR IP没有路由的时候上报 */
                                spm_bfd_need_alarm_fail(ulDiscrimer);
                                
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                                
                            }
                        }
                        else if (BFD_IPV6 == pstBasicBfdCB->ip_type)
                        {
                            /* 根据vrfid+dip查询FTN迭代出端口 */
                            iRet = spm_l3_routeforoam(pstBasicBfdCB->vrf_id, 1, &(pstBasicBfdCB->remote_ip[0]), &ulPortIndex);
                            if (SUCCESS != iRet)/* 若执行失败 */
                            {
                                NBB_TRC_FLOW((NBB_FORMAT"get ipv6 ftn error, vrf_id = %ld remote_ip = %lx.%lx.%lx.%lx",
                                        pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[0], pstBasicBfdCB->remote_ip[1],
                                        pstBasicBfdCB->remote_ip[2], pstBasicBfdCB->remote_ip[3]));
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                        "Get ipv6 ftn error %d, vrf_id = %ld remote_ip = %lx.%lx.%lx.%lx\r\n", 
                                        iRet, pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[0],
                                        pstBasicBfdCB->remote_ip[1],
                                        pstBasicBfdCB->remote_ip[2], pstBasicBfdCB->remote_ip[3]);
                                
                                /* 上报配置失败消息，只有BFD FOR IP没有路由的时候上报 */
                                spm_bfd_need_alarm_fail(ulDiscrimer);
                                
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                                
                            }
                        }
                    }
                    else    /* BFD for OSPS/ISIS BFD可能在路由之后下发 */
                    {
                        ulPortIndex = pstBasicBfdCB->phy_if_index;
                    }
                    
                    iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                    if (SUCCESS != iRet)/* 若执行失败 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get port info error=%d, PortIndex = %ld ", iRet, ulPortIndex));
                        
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error=%d, PortIndex = %ld\r\n",
                            iRet, ulPortIndex);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    /* 判断出接口类型 */
                    if (ATG_DCI_LAG == ucPortType)
                    {
                        usMainBoardId = spm_hardw_getemustate();
                        if (0xff == usMainBoardId)
                        {
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Port is Lag,but src is absent\r\n");
                            pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }
#if 0
                        if (BFD_IPV6 == pstBasicBfdCB->ip_type)
                        {
                            if (0XFE80 == (pstBasicBfdCB->remote_ip[0] >> 16))
                            {
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Port is Lag,ipv6 fe80 not support\r\n");
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }
                        }   
#endif
                    }
                    else if (ATG_DCI_VE_PORT == ucPortType)
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"Do not support VE type!"));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Do not support VE type\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                    else
                    {
                        usMainBoardId = ucSlotId;
                    }                    
                }
            }
            else if (BFD_CFG_Tunnel == pstBasicBfdCB->bfd_frame_type) /* for rsvp tunnel、lsp */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is for tunnel\r\n");

                if (ATG_DCI_OPER_NULL == pstset_bfd->oper_rsvp_lsp)
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Congfig is null\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }

                pucIpsMsgStar = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstset_bfd, &pstset_bfd->rsvp_lsp_data);
                NBB_ASSERT(NULL != pucIpsMsgStar);
                pstBasicBfdTunnel = (ATG_DCI_BFD_RSVP_LSP_DATA *)pucIpsMsgStar;
                if(NULL == pstBasicBfdTunnel)
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd tunnel CB is null\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                NBB_ASSERT(0 == pstBasicBfdTunnel->lsp_id);
                
                /* 根据ingress+egress+tunnelid查询出接口所在盘 */
                OS_MEMSET(&stCrlspKey, 0, sizeof(CRTXLSP_KEY));
                stCrlspKey.egress = pstBasicBfdTunnel->egress;
                stCrlspKey.ingress = pstBasicBfdTunnel->ingress;
                stCrlspKey.tunnelid = pstBasicBfdTunnel->tunnel_id;
                iRet = spm_l3_rsvpforoam(&stCrlspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                if (SUCCESS != iRet)/* 若执行失败 */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"get crlsp tunnel error = %d, egress = %lx ingress = %lx tunnelid = %ld",
                        iRet, pstBasicBfdTunnel->egress, pstBasicBfdTunnel->ingress, pstBasicBfdTunnel->tunnel_id));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                        "Get crlsp tunnel error = %d, egress = %lx ingress = %lx tunnelid = %ld\r\n",
                        iRet, pstBasicBfdTunnel->egress, pstBasicBfdTunnel->ingress, pstBasicBfdTunnel->tunnel_id);
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }

                /* add begin by limingming, 2013/2/17, Mantis号:HSD111111, 原因:BFD会话选盘 */
                iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* 若执行失败 */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"get port info error = %d, PortIndex = %ld ", iRet, ulPortIndex));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, PortIndex = %ld\r\n",
                        iRet, ulPortIndex);
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }

                /* 判断出接口类型 */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* 若为LAG口，返回的portid为lagid */
                    iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagid = %d ", iRet, usPortId));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get lag info error = %d, lagid = %d\r\n", iRet, usPortId);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    /* 取LAG成员端口所在的第一块盘做为主处理盘 */
                    for (usloopi = 0; usloopi < ATG_DCI_LAG_MEMBER_PORT_NUM; usloopi++)
                    {
                        if (0 != aulLagPortIndex[usloopi])
                        {
                            break;
                        }
                    }
                    
                    if (ATG_DCI_LAG_MEMBER_PORT_NUM == usloopi)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Lag info error, could not find useful portindex\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                    
                    iRet = spm_get_portid_from_physical_port_index(aulLagPortIndex[usloopi], &stPortInfo NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagindex = %ld ",
                            iRet, aulLagPortIndex[usloopi]));                      
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get lag info error = %d, lagindex = %ld\r\n",
                            iRet, aulLagPortIndex[usloopi]);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                    ucSlotId = stPortInfo.slot_id;
                }
                else if (ATG_DCI_VE_PORT == ucPortType)
                {
                    NBB_TRC_FLOW((NBB_FORMAT"Do not support VE type!"));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Do not support VE type\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                usMainBoardId = ucSlotId;
                /* add end by limingming, 2013/2/17 */
            }
            else if (BFD_CFG_LSP == pstBasicBfdCB->bfd_frame_type)/* for ldp lsp */
            {
                if (ATG_DCI_OPER_NULL != pstset_bfd->oper_ldp_lsp)
                {
                    pucIpsMsgStar = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstset_bfd, &pstset_bfd->ldp_lsp_data);
                    NBB_ASSERT(NULL != pucIpsMsgStar);
                    
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is for ldp lsp\r\n");
                    pstBasicBfdLsp = (ATG_DCI_BFD_LDP_LSP_DATA *)pucIpsMsgStar;
                    if(NULL == pstBasicBfdLsp)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd lsp CB is null\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                    
                    /* 根据fec+vrfid查询出接口所在盘 */
                    OS_MEMSET(&stLdplspKey, 0, sizeof(FTN_KEY));
                    stLdplspKey.fec = pstBasicBfdLsp->fec_prefix;
                    stLdplspKey.mask = pstBasicBfdLsp->fec_prefix_len;
                    stLdplspKey.vrfid = pstBasicBfdLsp->vrf_id;
                    iRet = spm_l3_ldpforoam(&stLdplspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                    if (SUCCESS != iRet)/* 若执行失败 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get ldp lsp error = %d, fec = %lx mask = %d vrfid = %ld", 
                            iRet, pstBasicBfdLsp->fec_prefix, pstBasicBfdLsp->fec_prefix_len, pstBasicBfdLsp->vrf_id));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                            "Get ldp lsp error = %d, fec = %lx mask = %d vrfid = %ld\r\n", 
                            iRet, pstBasicBfdLsp->fec_prefix, pstBasicBfdLsp->fec_prefix_len, pstBasicBfdLsp->vrf_id);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                }
                else
                {
                    if (ATG_DCI_OPER_NULL == pstset_bfd->oper_rsvp_lsp)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Congfig is null\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    pucIpsMsgStar = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstset_bfd, &pstset_bfd->rsvp_lsp_data);
                    NBB_ASSERT(NULL != pucIpsMsgStar);
                    pstBasicBfdTunnel = (ATG_DCI_BFD_RSVP_LSP_DATA *)pucIpsMsgStar;
                    if(NULL == pstBasicBfdTunnel)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd tunnel CB is null\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                    
                    NBB_ASSERT(0 != pstBasicBfdTunnel->lsp_id);

                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is for rsvp lsp\r\n");
                    
                    /* 根据ingress+egress+tunnelid查询出接口所在盘 */
                    OS_MEMSET(&stCrlspKey, 0, sizeof(CRTXLSP_KEY));
                    stCrlspKey.egress = pstBasicBfdTunnel->egress;
                    stCrlspKey.ingress = pstBasicBfdTunnel->ingress;
                    stCrlspKey.tunnelid = pstBasicBfdTunnel->tunnel_id;
                    stCrlspKey.lspid = pstBasicBfdTunnel->lsp_id;
                    iRet = spm_l3_rsvpforoam(&stCrlspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                    if (SUCCESS != iRet)/* 若执行失败 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT
                            "get crlsp tunnel error = %d, egress = %lx ingress = %lx tunnelid = %ld",
                            iRet, pstBasicBfdTunnel->egress, pstBasicBfdTunnel->ingress, pstBasicBfdTunnel->tunnel_id));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                            "Get crlsp tunnel error = %d, egress = %x ingress = %x tunnelid = %ld\r\n", 
                            iRet, pstBasicBfdTunnel->egress, pstBasicBfdTunnel->ingress, pstBasicBfdTunnel->tunnel_id);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                }
                
                /* add begin by limingming, 2013/2/18, Mantis号:HSD111111, 原因:BFD会话选盘 */
                iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* 若执行失败 */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"get port info error = %d, PortIndex = %ld ", iRet, ulPortIndex));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, PortIndex = %ld\r\n",
                        iRet, ulPortIndex);
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }

                /* 判断出接口类型 */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* 若为LAG口，返回的portid为lagid */
                    iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagid = %ld ", iRet, usPortId));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get lag info error = %d, lagid = %ld\r\n", iRet, usPortId);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    /* 取LAG成员端口所在的第一块盘做为主处理盘 */
                    for (usloopi = 0; usloopi < ATG_DCI_LAG_MEMBER_PORT_NUM; usloopi++)
                    {
                        if (0 != aulLagPortIndex[usloopi])
                        {
                            break;
                        }
                    }
                    
                    if (ATG_DCI_LAG_MEMBER_PORT_NUM == usloopi)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Lag info error, could not find useful portindex\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                    
                    iRet = spm_get_portid_from_physical_port_index(aulLagPortIndex[usloopi], &stPortInfo NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagindex = %ld ",
                            iRet, aulLagPortIndex[usloopi]));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get lag info error = %d, lagindex = %ld\r\n",
                            iRet, aulLagPortIndex[usloopi]);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                    ucSlotId = stPortInfo.slot_id;
                }
                
                else if (ATG_DCI_VE_PORT == ucPortType)
                {
                    NBB_TRC_FLOW((NBB_FORMAT"Do not support VE type!"));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Do not support VE type!\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                usMainBoardId = ucSlotId;
                /* add end by limingming, 2013/2/18 */
                
            }
            else if (BFD_CFG_PW == pstBasicBfdCB->bfd_frame_type)/* for pw */
            {
                if (ATG_DCI_OPER_NULL == pstset_bfd->oper_pw)
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Congfig is null\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                pucIpsMsgStar = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstset_bfd, &pstset_bfd->pw_data);
                NBB_ASSERT(NULL != pucIpsMsgStar);
                pstBasicBfdPw = (ATG_DCI_BFD_PW_DATA *)pucIpsMsgStar;
                if (NULL == pstBasicBfdPw)
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd PW CB is null\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                stVcKey.peer_ip = pstBasicBfdPw->peer_id;
                stVcKey.vc_id   = pstBasicBfdPw->vc_id;
                stVcKey.vc_type = pstBasicBfdPw->vc_type;
                
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is for pw\r\n");

                /* 根据vcid+vctype+peerid查询入接口所在盘 */
                iRet = spm_get_vpinfo_from_vckey(&stVcKey, &stVcInfo NBB_CCXT);
                if (SUCCESS != iRet)/* 不成功 */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"pw get vp info error = %d, peerid = %lx vcid = %lu vctype = %d",
                        iRet, pstBasicBfdPw->peer_id, pstBasicBfdPw->vc_id, pstBasicBfdPw->vc_type));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Pw info error = %d, peerid = %lx vcid = %lu vctype = %d\r\n",
                        iRet, pstBasicBfdPw->peer_id, pstBasicBfdPw->vc_id, pstBasicBfdPw->vc_type);
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                #ifdef SPU
                for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
                {
                    iRet = fhdrv_psn_mpls_set_pwlabel_yid(c3_unit, stVcInfo.vp_id,
                        stVcInfo.pw_rx_label, pstset_bfd->key);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"write c3 pw_mid table error = %d,unit=%u, pw label=%u, mid=%u\r\n",
                            iRet, c3_unit, stVcInfo.pw_rx_label, pstset_bfd->key));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                            "write c3 pw_mid table error = %d,unit=%u, pw label=%u, mid=%u\r\n",
                            iRet, c3_unit, stVcInfo.pw_rx_label, pstset_bfd->key);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                }
                #endif

                if (L2VPN_VPWS == stVcInfo.vpn_type)    /* VPWS区分端口情况 */
                {
                    if (0 != stVcInfo.uni_slot)     /* 可以获取到UNI槽位号说明是普通的PW，配置到UNI口 */  
                    {
                        ucSlotId = stVcInfo.uni_slot;
                    }
                    else    /* 获取不到UNI槽位表示UNI是LAG口(配到第一个LAG成员) */
                    {
                        iRet = spm_get_portid_for_bfd(stVcInfo.uni_port_index, &usPortId, &ucSlotId,
                            &ucPortType NBB_CCXT);
                        if (SUCCESS != iRet)/* 若执行失败 */
                        {
                            NBB_TRC_FLOW((NBB_FORMAT"get port info error = %d, PortIndex = %ld ", iRet, ulPortIndex));
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "get port info error = %d, PortIndex = %ld\r\n",
                                iRet, ulPortIndex);
                            pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }

                        /* 判断出接口类型 */
                        if (ATG_DCI_LAG == ucPortType)  /* LAG口(配到第一个LAG成员) */
                        {
                            /* 若为LAG口，返回的portid为lagid */
                            iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET,
                                aulLagPortIndex, &ucLagNum NBB_CCXT);
                            if (SUCCESS != iRet)/* 判断执行结果 */
                            {
                                NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagid = %ld ", iRet, usPortId));
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n",
                                    iRet, usPortId);
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }

                            /* 取LAG成员端口所在的第一块盘做为主处理盘 */
                            for (usloopi = 0; usloopi < ATG_DCI_LAG_MEMBER_PORT_NUM; usloopi++)
                            {
                                if (0 != aulLagPortIndex[usloopi])
                                {
                                    break;
                                }
                            }
                            
                            if (ATG_DCI_LAG_MEMBER_PORT_NUM == usloopi)
                            {
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                    "Lag info error, could not find useful portindex\r\n");
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }
                            
                            iRet = spm_get_portid_from_physical_port_index(aulLagPortIndex[usloopi],
                                &stPortInfo NBB_CCXT);
                            if (SUCCESS != iRet)/* 判断执行结果 */
                            {
                                NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagindex = %ld ",
                                    iRet, aulLagPortIndex[usloopi]));
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get lag info error = %d, lagindex = %ld\r\n",
                                    iRet, aulLagPortIndex[usloopi]);
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }    
                            ucSlotId = stPortInfo.slot_id;
                        }
                        else if (ATG_DCI_VE_PORT == ucPortType) /* VE(配到指定槽位) */  /* 此处不可能是VE */
                        {
                            ucSlotId = spm_hardw_getemustate();
                            if (0xff == ucSlotId)
                            {
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Mutiple session,but src is absent\r\n");
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }
                            
                            iRet = spm_bfd_add_nniId_table(ulDiscrimer, pstBasicBfdPw->peer_id,
                                stVcInfo.pw_tx_label, 
                                stVcInfo.next_hop_id);
                            if (SUCCESS != iRet)/* 若执行失败 */
                            {
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                    "spm_bfd_nni_id_fpgareg error = %d, PortIndex = %ld\r\n",
                                    iRet, pstBasicBfdCB->phy_if_index);
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto EXIT_LABEL;
                            }
                        }
                    }
                }
                else    /* VPLS (配置到NNI口) */
                {
                    
                    /* 根据vcid+vctype+peerid查询出接口所在盘 */
                    iRet = spm_get_port_index_from_vc_key(&stVcKey, &ulPortIndex NBB_CCXT);
                    if (SUCCESS != iRet)/* 不成功 */
                    {
                        NBB_TRC_FLOW(
                            (NBB_FORMAT"pw get port index info error = %d,peerid = %lx vcid = %lu vctype = %d",
                            iRet, pstBasicBfdPw->peer_id, pstBasicBfdPw->vc_id, pstBasicBfdPw->vc_type));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, 
                            "Pw info error = %d, peerid = %lx vcid = %lu vctype = %d\r\n",
                            iRet, pstBasicBfdPw->peer_id, pstBasicBfdPw->vc_id, pstBasicBfdPw->vc_type);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    /* add begin by limingming, 2013/2/18, Mantis号:HSD111111, 原因:BFD会话选盘 */
                    iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                    if (SUCCESS != iRet)/* 若执行失败 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get port info error = %d, PortIndex = %ld ", iRet, ulPortIndex));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "get port info error = %d, PortIndex = %ld\r\n",
                            iRet, ulPortIndex);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    /* 判断出接口类型 */
                    if (ATG_DCI_LAG == ucPortType)
                    {
                        /* 若为LAG口，返回的portid为lagid */
                        iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                        if (SUCCESS != iRet)/* 判断执行结果 */
                        {
                            NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagid = %ld ", iRet, usPortId));
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n",
                                iRet, usPortId);
                            pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }

                        /* 取LAG成员端口所在的第一块盘做为主处理盘 */
                        for (usloopi = 0; usloopi < ATG_DCI_LAG_MEMBER_PORT_NUM; usloopi++)
                        {
                            if (0 != aulLagPortIndex[usloopi])
                            {
                                break;
                            }
                        }
                        
                        if (ATG_DCI_LAG_MEMBER_PORT_NUM == usloopi)
                        {
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Lag info error, could not find useful portindex\r\n");
                            pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }
                        
                        iRet = spm_get_portid_from_physical_port_index(aulLagPortIndex[usloopi], &stPortInfo NBB_CCXT);
                        if (SUCCESS != iRet)/* 判断执行结果 */
                        {
                            NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagindex = %ld ",
                                iRet, aulLagPortIndex[usloopi]));
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get lag info error = %d, lagindex = %ld\r\n",
                                iRet, aulLagPortIndex[usloopi]);
                            pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }    
                        ucSlotId = stPortInfo.slot_id;
                    }
                    else if (ATG_DCI_VE_PORT == ucPortType)
                    {
                        ucSlotId = spm_hardw_getemustate();
                        if (0xff == ucSlotId)
                        {
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Mutiple session,but src is absent\r\n");
                            pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }
                        
                        iRet = spm_bfd_add_nniId_table(ulDiscrimer, pstBasicBfdPw->peer_id,
                            stVcInfo.pw_tx_label, 
                            stVcInfo.next_hop_id);
                        if (SUCCESS != iRet)/* 若执行失败 */
                        {
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, 
                                "spm_bfd_nni_id_fpgareg error = %d, PortIndex = %ld\r\n",
                                iRet, pstBasicBfdCB->phy_if_index);
                            pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto EXIT_LABEL;
                        }
                    }
                }
                
                usMainBoardId = ucSlotId;
                /* add end by limingming, 2013/2/18 */
            }
            else if (BFD_CFG_INTERFACE == pstBasicBfdCB->bfd_frame_type)/* interface */
            {
                iRet = spm_get_portid_for_bfd(pstBasicBfdCB->phy_if_index, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* 若执行失败 */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"get port info error = %d, PortIndex = %ld ",
                        iRet, pstBasicBfdCB->phy_if_index));
                    
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, PortIndex = %ld\r\n",
                        iRet, pstBasicBfdCB->phy_if_index);
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                /* 判断出接口类型 */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* 若为LAG口，返回的portid为lagid */
                    iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get lag info error = %d, lagid = %ld ", iRet, usPortId));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n", iRet, usPortId);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    /* 取LAG成员端口所在的第一块盘做为主处理盘 */
                    for (usloopi = 0; usloopi < ATG_DCI_LAG_MEMBER_PORT_NUM; usloopi++)
                    {
                        if (0 != aulLagPortIndex[usloopi])
                        {
                            break;
                        }
                    }
                    
                    if (ATG_DCI_LAG_MEMBER_PORT_NUM == usloopi)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Lag info error, could not find useful portindex\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                    
                    iRet = spm_get_portid_from_physical_port_index(aulLagPortIndex[usloopi], &stPortInfo NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"get lag info error, lagindex = %ld ", aulLagPortIndex[usloopi]));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get lag info error, lagindex = %ld\r\n",
                            aulLagPortIndex[usloopi]);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }    
                    ucSlotId = stPortInfo.slot_id;
                }
                else if (ATG_DCI_VE_PORT == ucPortType)
                {
                    NBB_TRC_FLOW((NBB_FORMAT"Do not support VE type!"));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Do not support VE type\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }                
                usMainBoardId = ucSlotId;
            }

            /* 对配置进行存储 */
            pstBfdCfg = (SPM_BFD_GLOBAL *)NBB_MM_ALLOC(sizeof(SPM_BFD_GLOBAL), NBB_NORETRY_ACT, MEM_SPM_BFD_GLOBAL_CB);
            if (NULL == pstBfdCfg)/* 为空 */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Memory is not enough"));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Memory is not enough!\r\n");
                pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            NBB_MEMSET(pstBfdCfg, 0, sizeof(SPM_BFD_GLOBAL));
            AVLL_INIT_NODE(pstBfdCfg->spm_bfd_node);
            pstBfdCfg->ulDiscrimer = pstset_bfd->key;
            pstBfdCfg->ucIptype = pstBasicBfdCB->ip_type;
            pstBfdCfg->ucFramtype = pstBasicBfdCB->bfd_frame_type;
            pstBfdCfg->ulSip[0] = pstBasicBfdCB->local_ip[0];
            pstBfdCfg->ulSip[1] = pstBasicBfdCB->local_ip[1];
            pstBfdCfg->ulSip[2] = pstBasicBfdCB->local_ip[2];
            pstBfdCfg->ulSip[3] = pstBasicBfdCB->local_ip[3];
            pstBfdCfg->ulDip[0] = pstBasicBfdCB->remote_ip[0];
            pstBfdCfg->ulDip[1] = pstBasicBfdCB->remote_ip[1];
            pstBfdCfg->ulDip[2] = pstBasicBfdCB->remote_ip[2];
            pstBfdCfg->ulDip[3] = pstBasicBfdCB->remote_ip[3];
            pstBfdCfg->usVrfid = pstBasicBfdCB->vrf_id;
            pstBfdCfg->ulPortIndex = pstBasicBfdCB->phy_if_index;
            pstBfdCfg->usSlotid = usMainBoardId;
            if (BFD_CFG_LSP == pstBasicBfdCB->bfd_frame_type)
            {
                if (NULL != pstBasicBfdLsp)
                {
                    pstBfdCfg->stLdpCfg.fec_prefix = pstBasicBfdLsp->fec_prefix;
                    pstBfdCfg->stLdpCfg.fec_prefix_len = pstBasicBfdLsp->fec_prefix_len;
                    pstBfdCfg->stLdpCfg.vrf_id = pstBasicBfdLsp->vrf_id;
                }
                if (NULL != pstBasicBfdTunnel)
                {
                    pstBfdCfg->stRsvpCfg.egress = pstBasicBfdTunnel->egress;
                    pstBfdCfg->stRsvpCfg.ingress = pstBasicBfdTunnel->ingress;
                    pstBfdCfg->stRsvpCfg.lsp_id = pstBasicBfdTunnel->lsp_id;
                    pstBfdCfg->stRsvpCfg.tunnel_id = pstBasicBfdTunnel->tunnel_id;
                }
            }

            if (BFD_CFG_Tunnel == pstBasicBfdCB->bfd_frame_type)
            {
                if(NULL == pstBasicBfdTunnel)
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd tunnel CB is null!\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                pstBfdCfg->stRsvpCfg.egress = pstBasicBfdTunnel->egress;
                pstBfdCfg->stRsvpCfg.ingress = pstBasicBfdTunnel->ingress;
                pstBfdCfg->stRsvpCfg.lsp_id = pstBasicBfdTunnel->lsp_id;
                pstBfdCfg->stRsvpCfg.tunnel_id = pstBasicBfdTunnel->tunnel_id;
            }

            if (BFD_CFG_PW == pstBasicBfdCB->bfd_frame_type)
            {
                if(NULL == pstBasicBfdPw)
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd pw CB is null!\r\n");
                    pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                    pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto EXIT_LABEL;
                }
                
                pstBfdCfg->stVcCfg.peer_ip = pstBasicBfdPw->peer_id;
                pstBfdCfg->stVcCfg.vc_id = pstBasicBfdPw->vc_id;
                pstBfdCfg->stVcCfg.vc_type = pstBasicBfdPw->vc_type;
            }
            
            NBB_TRC_FLOW((NBB_FORMAT"record global cfg, discrim = %ld", pstBfdCfg->ulDiscrimer));
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Record global cfg, discrim = %ld, mainboard = %d\r\n",
                pstBfdCfg->ulDiscrimer, pstBfdCfg->usSlotid);
            if (!AVLL_INSERT(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node))
            {
                NBB_TRC_FLOW((NBB_FORMAT"insert global avll tree error"));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "insert global avll tree error!\r\n");
                pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                goto EXIT_LABEL;
            }
            
            /* 判断是否需要记录在g_stBfdGlbTreeByIp中 */
            if (((BFD_CFG_IP == pstBasicBfdCB->bfd_frame_type) || (BFD_CFG_INTERFACE == pstBasicBfdCB->bfd_frame_type)) 
                && (0 == pstBasicBfdCB->peer_sid))
            {
                if ((BFD_IPV4 == pstBasicBfdCB->ip_type) && (0 == pstBasicBfdCB->peer_sid))/* ipv4 */
                {
                    pstBfdIpCfg = (SPM_BFD_IP_GLOBAL *)NBB_MM_ALLOC(sizeof(SPM_BFD_IP_GLOBAL), 
                        NBB_NORETRY_ACT, MEM_SPM_BFD_IP_GLOBAL_CB);
                    if (NULL == pstBfdIpCfg)/* 若为空 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"Memory is not enough"));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Memory is not enough!\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        goto EXIT_LABEL;
                    }
                    pstBfdIpCfg->stBfdGlbKey.ulSip = pstBasicBfdCB->local_ip[3];
                    pstBfdIpCfg->stBfdGlbKey.ulDip = pstBasicBfdCB->remote_ip[3];
                    pstBfdIpCfg->stBfdGlbKey.usVrfid = pstBasicBfdCB->vrf_id;
                    pstBfdIpCfg->ulDiscrimer = pstset_bfd->key;
                    pstBfdIpCfg->usSlotid    = usMainBoardId;
                    NBB_TRC_FLOW((NBB_FORMAT
                        "record ip cfg, key1 = %lx, key2 = %lx, key3 = %d, discrim = %ld, slot = %d", 
                        pstBfdIpCfg->stBfdGlbKey.ulSip, pstBfdIpCfg->stBfdGlbKey.ulDip,
                        pstBfdIpCfg->stBfdGlbKey.usVrfid, 
                        pstBfdIpCfg->ulDiscrimer, usMainBoardId));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                        "Record ip cfg, key1 = %lx, key2 = %lx, key3 = %d, discrim = %ld, slot = %d\r\n",
                        pstBfdIpCfg->stBfdGlbKey.ulSip, pstBfdIpCfg->stBfdGlbKey.ulDip,
                        pstBfdIpCfg->stBfdGlbKey.usVrfid, 
                        pstBfdIpCfg->ulDiscrimer, usMainBoardId);
                    AVLL_INIT_NODE(pstBfdIpCfg->spm_bfdip_node);

                    /* 查找是否已经存在 */
                    pstbfmtlfgtmp = (SPM_BFD_IP_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIp, &(pstBfdIpCfg->stBfdGlbKey));
                    if (NULL != pstbfmtlfgtmp)
                    {
                        BMU_LOG(BMU_INFO, SPM_BFD_LOG_DIR, "ip tree error,discrim = %ld,slot = %d!\r\n",
                            pstbfmtlfgtmp->ulDiscrimer,
                            pstbfmtlfgtmp->usSlotid);
                        BMU_LOG(BMU_INFO, SPM_BFD_LOG_DIR, "change ip tree,discrim = %ld,slot = %d!\r\n",
                            pstBfdIpCfg->ulDiscrimer,
                            pstBfdIpCfg->usSlotid);
                        AVLL_DELETE(g_stBfdGlbTreeByIp, pstbfmtlfgtmp->spm_bfdip_node);
                        NBB_MM_FREE(pstbfmtlfgtmp, MEM_SPM_BFD_IP_GLOBAL_CB);
                    }
                    
                    if (!AVLL_INSERT(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node))
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"insert ip avll tree error"));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "insert ip avll tree error!\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        goto EXIT_LABEL;
                        
                    }
                }
                
                if ((BFD_IPV6 == pstBasicBfdCB->ip_type) && (0 == pstBasicBfdCB->peer_sid)) /* ipv6 */
                {
                    pstBfdIpv6Cfg = (SPM_BFD_IPV6_GLOBAL *)NBB_MM_ALLOC(sizeof(SPM_BFD_IPV6_GLOBAL), 
                        NBB_NORETRY_ACT, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                    if (NULL == pstBfdIpv6Cfg)/* 若为空 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"Memory is not enough"));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Memory is not enough\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        goto EXIT_LABEL;
                    }
                    pstBfdIpv6Cfg->stBfdGlbKey.usVrfid = pstBasicBfdCB->vrf_id;
                    pstBfdIpv6Cfg->stBfdGlbKey.ulDip[0] = pstBasicBfdCB->remote_ip[0];
                    pstBfdIpv6Cfg->stBfdGlbKey.ulDip[1] = pstBasicBfdCB->remote_ip[1];
                    pstBfdIpv6Cfg->stBfdGlbKey.ulDip[2] = pstBasicBfdCB->remote_ip[2];
                    pstBfdIpv6Cfg->stBfdGlbKey.ulDip[3] = pstBasicBfdCB->remote_ip[3];
                    pstBfdIpv6Cfg->stBfdGlbKey.ulSip[0] = pstBasicBfdCB->local_ip[0];
                    pstBfdIpv6Cfg->stBfdGlbKey.ulSip[1] = pstBasicBfdCB->local_ip[1];
                    pstBfdIpv6Cfg->stBfdGlbKey.ulSip[2] = pstBasicBfdCB->local_ip[2];
                    pstBfdIpv6Cfg->stBfdGlbKey.ulSip[3] = pstBasicBfdCB->local_ip[3];
                    pstBfdIpv6Cfg->ulDiscrimer = pstset_bfd->key;
                    pstBfdIpv6Cfg->usSlotid    = usMainBoardId;
                    NBB_TRC_FLOW((NBB_FORMAT"record ip cfg, key1 = %ld.%ld.%ld.%ld, key2 = %ld.%ld.%ld.%ld, key3 = %d,"
                        " discrim = %ld, slot = %d",
                        pstBfdIpv6Cfg->stBfdGlbKey.ulSip[0], 
                        pstBfdIpv6Cfg->stBfdGlbKey.ulSip[1], pstBfdIpv6Cfg->stBfdGlbKey.ulSip[2],
                        pstBfdIpv6Cfg->stBfdGlbKey.ulSip[3], pstBfdIpv6Cfg->stBfdGlbKey.ulDip[0],
                        pstBfdIpv6Cfg->stBfdGlbKey.ulDip[1], pstBfdIpv6Cfg->stBfdGlbKey.ulDip[2],
                        pstBfdIpv6Cfg->stBfdGlbKey.ulDip[3], pstBfdIpv6Cfg->stBfdGlbKey.usVrfid, 
                        pstBfdIpv6Cfg->ulDiscrimer, usMainBoardId));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "record ip cfg, key1 = %ld.%ld.%ld.%ld,"
                        " key2 = %ld.%ld.%ld.%ld, key3 = %d,"
                        " discrim = %ld, slot = %d\r\n",
                        pstBfdIpv6Cfg->stBfdGlbKey.ulSip[0], 
                        pstBfdIpv6Cfg->stBfdGlbKey.ulSip[1], pstBfdIpv6Cfg->stBfdGlbKey.ulSip[2],
                        pstBfdIpv6Cfg->stBfdGlbKey.ulSip[3], pstBfdIpv6Cfg->stBfdGlbKey.ulDip[0],
                        pstBfdIpv6Cfg->stBfdGlbKey.ulDip[1], pstBfdIpv6Cfg->stBfdGlbKey.ulDip[2],
                        pstBfdIpv6Cfg->stBfdGlbKey.ulDip[3], pstBfdIpv6Cfg->stBfdGlbKey.usVrfid, 
                        pstBfdIpv6Cfg->ulDiscrimer, usMainBoardId);
                    AVLL_INIT_NODE(pstBfdIpv6Cfg->spm_bfdipv6_node);

                    /* 查找是否已经存在 */
                    pstbfdipv6cfgtmp = (SPM_BFD_IPV6_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIpv6,
                                                                     &(pstBfdIpv6Cfg->stBfdGlbKey));
                    if (NULL != pstbfdipv6cfgtmp)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "ip tree error,discrim = %ld,slot = %d!\r\n",
                            pstbfdipv6cfgtmp->ulDiscrimer,
                            pstbfdipv6cfgtmp->usSlotid);
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "change ip tree,discrim = %ld,slot = %d!\r\n",
                            pstBfdIpv6Cfg->ulDiscrimer,
                            pstBfdIpv6Cfg->usSlotid);
                        AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstbfdipv6cfgtmp->spm_bfdipv6_node);
                        NBB_MM_FREE(pstbfdipv6cfgtmp, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                    }
                    if(!AVLL_INSERT(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node))
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"insert ipv6 avll tree error"));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "insert ipv6 avll tree error!\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        goto EXIT_LABEL;
                    }
                }
            }
            
            /* 出接口为主控盘 */
            if (0 == spm_hardw_getboardtype((NBB_BYTE)usMainBoardId))
            {
                if (0 == spm_hardw_getboardtype(ucLocalSlot)) /* 本盘为主控盘，主备配置保持一致(双备份) */
                {
                    if (g_usBfdNum >= BFD_MAX_SESNUM)/* 超过最大数 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"exceed max num 1023, discrim = %ld", pstset_bfd->key));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Exceed max num 1023, discrim = %ld\r\n", pstset_bfd->key);
                        pstset_bfd->return_code = ATG_DCI_RC_NO_RESOURCE;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }

                    /* 可能会话在等待的删除状态，先查询等待删除后add */
                    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
                    {
                        if (ulDiscrimer == g_stBfdlocal[usLoopi].ulDiscrimer)/* 描述相等 */
                        {
                            if (TRUE == g_stBfdlocal[usLoopi].ucbIsNeedDelete)/* 需要删除 */
                            {
                                NBB_TRC_FLOW((NBB_FORMAT"This time could not add discrim = %ld", ulDiscrimer));
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "This time could not add discrim = %ld\r\n",
                                    ulDiscrimer);
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                                NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                                if (NULL != pstBfdIpCfg)/* 不为空 */
                                {
                                    AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                                    NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                                }
                                if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                                {
                                    AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                                    NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                                }
                                goto EXIT_LABEL;
                            }
                        }
                    }

                    /* 添加会话 */
                    iRet = spm_bfd_addsession(pstset_bfd->key, pstBasicBfdCB, pstBasicBfdTunnel, 
                        pstBasicBfdLsp, pstBasicBfdPw NBB_CCXT);
                    if (SUCCESS != iRet)/* 不成功 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"add local session failed, discrim = %ld", pstset_bfd->key));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add local session failed, discrim = %ld\r\n",
                            pstset_bfd->key);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                    g_usBfdNum++;
                }
                else /* 本盘不为主控盘，添加转发表项 */ 
                {
                    if (g_usFwdNum >= BFD_MAX_FWDNUM)/* 超过最大数目 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"Fwd is full, Dri = %ld", pstset_bfd->key));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd is full, Dri = %ld\r\n", pstset_bfd->key);
                        pstset_bfd->return_code = ATG_DCI_RC_NO_RESOURCE;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                    
                    /* 为了严谨，检查一下是否存在该转发信息，若存在，则说明有错误 */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &ulDiscrimer);
                    if (NULL != pstFwdInfo)/* 不为空 */
                    {
                        NBB_ASSERT(FALSE);
                        NBB_TRC_FLOW((NBB_FORMAT"Fwd error, Dri = %ld", ulDiscrimer));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd error, Dri = %ld\r\n", ulDiscrimer);
                
                        /* 为了确保下次执行成功，删除 */
                        AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                        OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by 蒋博,jiangbo, 20131113, Mantis号:
                                                                           无, 原因:因为不由nbase管理进程也会分配、释放
                                                                           这块内存，不能使用NBB_MM_FREE管理，同时不能
                                                                           被verify */
                        pstFwdInfo = NULL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                        
                    iRet = spm_bfd_addfwdtable(ulDiscrimer, usMainBoardId);
                    if (SUCCESS != iRet)/* 不成功 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"add fwd table failed, discrim = %ld, Dslotid = %d", 
                            ulDiscrimer, usMainBoardId));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add fwd table failed, discrim = %ld, Dslotid = %d\r\n", 
                            ulDiscrimer, usMainBoardId);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                    
                    /* 记录转发信息 */
                    /* modify begin by 蒋博,jiangbo, 20131113, Mantis号:无, 原因:因为不由nbase管理进程也会分配、释放这块
                       内存，不能使用NBB_MM_ALLOC管理，同时不能被verify */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)OS_ALLOC(sizeof(SPM_FWD_GLOBAL),MEM_SPM_BFD_FWD_GLOBAL_CB);
                    /* modify end by 蒋博,jiangbo, 20131113 */
                    if (NULL == pstFwdInfo)/* 为空 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"Memory is not enough"));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Memory is not enough\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        /* 回退fwdtable */
                        (NBB_VOID)spm_bfd_delfwdtable(ulDiscrimer NBB_CCXT);
                        
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                        
                    pstFwdInfo->ulDiscrimer = ulDiscrimer;
                    pstFwdInfo->usSlotid = usMainBoardId;
                    AVLL_INIT_NODE(pstFwdInfo->spm_fwd_node);
                    if(!AVLL_INSERT(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node))
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"insert fwd avll tree error"));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "insert fwd avll tree error\r\n");

                        /* 曾经加过，释放内存，防止内存溢出 */                        
                        OS_FREE(pstFwdInfo, MEM_SPM_BFD_FWD_GLOBAL_CB);
                        pstFwdInfo = NULL;

                        /* 曾经有不影响后续功能，可以继续执行 */
                    }
                    else
                    {
                        g_usFwdNum++;
                    }
                }
            }
            else
            {
                if (usMainBoardId == (NBB_USHORT)ucLocalSlot)   /* 处理盘是本盘 */
                {
                    if (g_usBfdNum >= BFD_MAX_SESNUM)/* 超过最大数 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"exceed max num 1023, discrim = %ld", pstset_bfd->key));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Exceed max num 1023, discrim = %ld\r\n", pstset_bfd->key);
                        pstset_bfd->return_code = ATG_DCI_RC_NO_RESOURCE;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }

                    /* 可能会话在等待的删除状态，先查询等待删除后add */
                    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
                    {
                        if (ulDiscrimer == g_stBfdlocal[usLoopi].ulDiscrimer)/* 描述相等 */
                        {
                            if (TRUE == g_stBfdlocal[usLoopi].ucbIsNeedDelete)/* 需要删除 */
                            {
                                NBB_TRC_FLOW((NBB_FORMAT"This time could not add discrim = %ld", ulDiscrimer));
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "This time could not add discrim = %ld\r\n",
                                    ulDiscrimer);
                                pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                                NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                                if (NULL != pstBfdIpCfg)/* 不为空 */
                                {
                                    AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                                    NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                                }
                                
                                if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                                {
                                    AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                                    NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                                }
                                goto EXIT_LABEL;
                            }
                        }
                    }

                    /* 添加会话 */
                    iRet = spm_bfd_addsession(pstset_bfd->key, pstBasicBfdCB, pstBasicBfdTunnel, 
                        pstBasicBfdLsp, pstBasicBfdPw NBB_CCXT);
                    if (SUCCESS != iRet)/* 不成功 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"add local session failed, discrim = %ld", pstset_bfd->key));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add local session failed, discrim = %ld\r\n",
                            pstset_bfd->key);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                    g_usBfdNum++;
                }
            }

            /* 关联保护倒换 */
            if (BFD_CFG_IP == pstBfdCfg->ucFramtype)
            {
                (NBB_VOID)spm_set_bfd_cfg_change(VRF_ROUTE_BFD_CHANGE);
            }
            if (BFD_CFG_PW == pstBfdCfg->ucFramtype)
            {
                (NBB_VOID)spm_set_bfd_cfg_change(PW_BFD_CHANGE);
            }
            if (BFD_CFG_LSP == pstBfdCfg->ucFramtype)
            {
                (NBB_VOID)spm_set_bfd_cfg_change(LDP_LSP_BFD_CHANGE);/* 保护模块需要查询ldp和rsvp的会话 */
            }
            if (BFD_CFG_Tunnel == pstBfdCfg->ucFramtype)
            {
                (NBB_VOID)spm_set_bfd_cfg_change(RSVP_LSP_BFD_CHANGE);
            }
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add session %d end\r\n", pstset_bfd->key);
        }
        else /* modify情况 */
        {      
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Modify session %d, session mainboard:%d\r\n",
                pstset_bfd->key, pstBfdCfg->usSlotid);
            
            /* 查看是否是本盘处理 */
            if (0 == spm_hardw_getboardtype((NBB_BYTE)pstBfdCfg->usSlotid))/* 主处理盘为主控盘 */
            {
                if (0 == spm_hardw_getboardtype(ucLocalSlot))/* 本盘为主控盘 */
                {
                    if (ATG_DCI_OPER_NULL == pstset_bfd->oper_basic)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Congfig is null\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    pucIpsMsgStar = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstset_bfd, &pstset_bfd->basic_data);
                    NBB_ASSERT(NULL != pucIpsMsgStar);
                    pstBasicBfdCB = (ATG_DCI_BFD_BASIC_DATA *)pucIpsMsgStar;
                    
                    iRet = spm_bfd_updatesession(pstset_bfd->key, pstBasicBfdCB NBB_CCXT);
                    if (SUCCESS != iRet)/* 不成功 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"update local session failed = %d, discrim = %ld", iRet,
                            pstset_bfd->key));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Update local session failed = %d, discrim = %ld\r\n", iRet,
                            pstset_bfd->key);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                       
                }
            }
            else
            {
                if (pstBfdCfg->usSlotid == (NBB_USHORT)ucLocalSlot)
                {
                    if (ATG_DCI_OPER_NULL == pstset_bfd->oper_basic)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Congfig is null\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }

                    pucIpsMsgStar = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstset_bfd, &pstset_bfd->basic_data);
                    NBB_ASSERT(NULL != pucIpsMsgStar);
                    pstBasicBfdCB = (ATG_DCI_BFD_BASIC_DATA *)pucIpsMsgStar;
                    
                    iRet = spm_bfd_updatesession(pstset_bfd->key, pstBasicBfdCB NBB_CCXT);
                    if (SUCCESS != iRet)/* 不成功 */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"update local session failed = %d, discrim = %ld", iRet,
                            pstset_bfd->key));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "update local session failed = %d, discrim = %ld\r\n", iRet,
                            pstset_bfd->key);
                        pstset_bfd->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto EXIT_LABEL;
                    }
                }
            }
        }
        
    }
         
    pstset_bfd->return_code = ATG_DCI_RC_OK;
    EXIT_LABEL : NBB_TRC_EXIT();

    /*  配置增删时不进行转发表的添加*/
    g_ucCleanCfgCheck = 0;
    return;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_dci_set_bfd_del
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年5月28日
 * 函数功能  : 根据IPS消息删除BFD指定配置
 * 输入参数  : ATG_DCI_SET_BFD_SESSION *pstset_bfd  IPS消息
               NBB_CCXT_T NBB_CXT                   NBASE多线程参数
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID spm_bfd_dci_set_bfd_del(ATG_DCI_SET_BFD_SESSION *pstset_bfd NBB_CCXT_T NBB_CXT)
{
    SPM_BFD_GLOBAL          *pstBfdCfg = NULL;
    NBB_ULONG   ulDiscrimer = 0;

    NBB_TRC_ENTRY(("spm_bfd_dci_set_bfd_del"));

    
    /* 入参判断 */
    if (NULL == pstset_bfd)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        goto EXIT_LABEL;
    }
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Begin delete,Dri = %ld\r\n", pstset_bfd->key);
    ulDiscrimer = pstset_bfd->key;
    pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulDiscrimer);
    if (NULL == pstBfdCfg)/* 若为空 */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Session is not exist, Dri = %ld", pstset_bfd->key));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete session %ld, but session is not exist\r\n", pstset_bfd->key);
    }
    else/* 若不为空 */
    {
        spm_bfd_del_cfg(pstBfdCfg NBB_CCXT);
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s:Delete session %d end \r\n", __FUNCTION__, pstset_bfd->key);
    }
    
EXIT_LABEL : NBB_TRC_EXIT();
    return;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_del_cfg
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年5月28日
 * 函数功能  : 根据所有单盘都有存储的BFD全局配置删除指定BFD配置
 * 输入参数  : SPM_BFD_GLOBAL *pstBfdCfg  所有单盘都有存储的BFD全局配置
               NBB_CCXT_T NBB_CXT         NBASE多线程参数
 * 输出参数  : 无
 * 返 回 值  : NBB_INT  错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT spm_bfd_del_cfg(SPM_BFD_GLOBAL *pstBfdCfg NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    NBB_ULONG   ulDiscrimer = 0;
    NBB_USHORT  usMainBoardId = 0;          /* 会话主处理盘 */
    NBB_BYTE    ucLocalSlot   = 0;          /* 本盘槽位号 */
    
    NBB_TRC_ENTRY(("spm_bfd_del_cfg"));
    
    /* 入参判断 */
    if (NULL == pstBfdCfg)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        iRet = ERROR;
        goto EXIT_LABEL;
    }
    
    /* 加锁,与线程学习fpga fifo 建立转发表配合，mantis 0083412 */
//    BmuWrLock(g_build_fpga_fwd_lock, BMU_WAIT_FOREVER);
    
    ulDiscrimer = pstBfdCfg->ulDiscrimer;
    
    /* 关联保护倒换 */
    (NBB_VOID)spm_bfd_notify_aps_del(ulDiscrimer, pstBfdCfg->ucFramtype NBB_CCXT);

    /* 删除以IP为KEY值的树 */
    (NBB_VOID)spm_bfd_del_ip_tree(pstBfdCfg NBB_CCXT);

    /* 在释放内存前先把槽位号存储 */
    usMainBoardId = pstBfdCfg->usSlotid;
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    
    /* 删除全局配置 */
    AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
    NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
    
    /* 判断是否为本盘处理 */
    if (0 == spm_hardw_getboardtype((NBB_BYTE)usMainBoardId)) /* 如果主处理盘是主控盘 */
    {
        if (0 == spm_hardw_getboardtype(ucLocalSlot))/* 本盘也是主控盘，主备盘处理一致，同时添加，同时删除 */
        {
            /* 有全局配置且主处理为本盘，但是本盘数目为0则说明之前的处理存在问题 */
            if (0 == g_usBfdNum)
            {
                NBB_ASSERT(FALSE);
                NBB_TRC_FLOW((NBB_FORMAT"Session is empty, Dri = %ld", ulDiscrimer));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is empty, Dri = %ld\r\n", ulDiscrimer);
            }
            else/* 删除 */
            {
                iRet = spm_bfd_delsession(ulDiscrimer NBB_CCXT);
                if (SUCCESS != iRet)/* 判断执行结果 */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"Del session failed, Dri = %ld", ulDiscrimer));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del session failed, Dri = %ld\r\n", ulDiscrimer);
                }
                else/* 成功 */
                {
                    g_usBfdNum--;
                }
            }                    
        }
    }
    else
    {
        if (usMainBoardId == (NBB_USHORT)ucLocalSlot)
        {
            /* 有全局配置且主处理为本盘，但是本盘数目为0则说明之前的处理存在问题 */
            if (0 == g_usBfdNum)
            {
                NBB_ASSERT(FALSE);
                NBB_TRC_FLOW((NBB_FORMAT"Session is empty, Dri = %ld", ulDiscrimer));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is empty, Dri = %ld\r\n", ulDiscrimer);
            }
            else/* 删除 */
            {
                iRet = spm_bfd_delsession(ulDiscrimer NBB_CCXT);
                if (SUCCESS != iRet)/* 判断执行结果 */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"Del session failed, Dri = %ld", ulDiscrimer));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del session failed, Dri = %ld\r\n", ulDiscrimer);
                }
                else/* 成功 */
                {
                    g_usBfdNum--;
                }
            }
        }
    }
    
    /* 非清配置则逐条删除，mantis:CR8000 V1R1-ER版本中试测试 0083509*/
//    if(!g_ucCleanCfgCheck)
//    {
        /* 查看本业务盘是否存在该转发表项 */
        spm_bfd_del_fwd_tree(ulDiscrimer NBB_CCXT);
        
//    }

    /* 删除NNI表 */
    spm_bfd_del_nniId_table(ulDiscrimer NBB_CCXT);

    /* 解锁 */
//    BmuUnlock(g_build_fpga_fwd_lock);
    iRet = SUCCESS;

EXIT_LABEL : NBB_TRC_EXIT();
    return iRet;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_del_ip_tree
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年5月28日
 * 函数功能  : 删除以IP为key值的配置树
 * 输入参数  : SPM_BFD_GLOBAL *pstBfdCfg            所有单盘都有存储的BFD全局配置
               NBB_CCXT_T NBB_CXT                   NBASE多线程参数
 * 输出参数  : 无
 * 返 回 值  : NBB_INT  错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT spm_bfd_del_ip_tree(SPM_BFD_GLOBAL *pstBfdCfg NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    SPM_BFDGLB_IP_KEY       stBfdGlbKey;
    SPM_BFDGLB_IPV6_KEY     stBfdIpv6GlbKey;
    SPM_BFD_IP_GLOBAL       *pstBfdIpCfg = NULL;
    SPM_BFD_IPV6_GLOBAL     *pstBfdIpv6Cfg = NULL;
    
    NBB_TRC_ENTRY(("spm_bfd_del_ip_tree"));
    
    /* 入参判断 */
    if (NULL == pstBfdCfg)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    /* 初始化参数 */
    OS_MEMSET(&stBfdGlbKey, 0, sizeof(SPM_BFDGLB_IP_KEY));
    OS_MEMSET(&stBfdIpv6GlbKey, 0, sizeof(SPM_BFDGLB_IPV6_KEY));

    if (BFD_IPV4 == pstBfdCfg->ucIptype)/* IPV4 */
    {
        stBfdGlbKey.ulSip = pstBfdCfg->ulSip[3];
        stBfdGlbKey.ulDip = pstBfdCfg->ulDip[3];
        stBfdGlbKey.usVrfid = pstBfdCfg->usVrfid;            
    
        pstBfdIpCfg = (SPM_BFD_IP_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIp, &stBfdGlbKey);
        if (NULL != pstBfdIpCfg)/* 若存储过则删除 */
        {
            NBB_TRC_FLOW((NBB_FORMAT"Del ip cfg, sip = %ld, dip = %ld, vrfid = %d", 
                stBfdGlbKey.ulSip, stBfdGlbKey.ulDip, stBfdGlbKey.usVrfid));
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del ip cfg, sip = %ld, dip = %ld, vrfid = %d\r\n", 
                stBfdGlbKey.ulSip, stBfdGlbKey.ulDip, stBfdGlbKey.usVrfid);
            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
        }
    }
    else /* IPV6 */
    {
        stBfdIpv6GlbKey.ulDip[0] = pstBfdCfg->ulDip[0];
        stBfdIpv6GlbKey.ulDip[1] = pstBfdCfg->ulDip[1];
        stBfdIpv6GlbKey.ulDip[2] = pstBfdCfg->ulDip[2];
        stBfdIpv6GlbKey.ulDip[3] = pstBfdCfg->ulDip[3];
        stBfdIpv6GlbKey.ulSip[0] = pstBfdCfg->ulSip[0];
        stBfdIpv6GlbKey.ulSip[1] = pstBfdCfg->ulSip[1];
        stBfdIpv6GlbKey.ulSip[2] = pstBfdCfg->ulSip[2];
        stBfdIpv6GlbKey.ulSip[3] = pstBfdCfg->ulSip[3];
        stBfdGlbKey.usVrfid = pstBfdCfg->usVrfid;  
        
        pstBfdIpv6Cfg = (SPM_BFD_IPV6_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIpv6, &stBfdIpv6GlbKey);
        if (NULL != pstBfdIpv6Cfg)/* 若存储过则删除 */
        {
            NBB_TRC_FLOW((NBB_FORMAT"Del ipv6 cfg, sip = %ld.%ld.%ld.%ld, dip = %ld.%ld.%ld.%ld, vrfid = %d", 
                stBfdIpv6GlbKey.ulSip[0], stBfdIpv6GlbKey.ulSip[1], stBfdIpv6GlbKey.ulSip[2], 
                stBfdIpv6GlbKey.ulSip[3], stBfdIpv6GlbKey.ulDip[0], stBfdIpv6GlbKey.ulDip[1],
                stBfdIpv6GlbKey.ulDip[2], stBfdIpv6GlbKey.ulDip[3], stBfdIpv6GlbKey.usVrfid));
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                "Del ipv6 cfg, sip = %ld.%ld.%ld.%ld, dip = %ld.%ld.%ld.%ld, vrfid = %d\r\n", 
                stBfdIpv6GlbKey.ulSip[0], stBfdIpv6GlbKey.ulSip[1], stBfdIpv6GlbKey.ulSip[2], 
                stBfdIpv6GlbKey.ulSip[3], stBfdIpv6GlbKey.ulDip[0], stBfdIpv6GlbKey.ulDip[1],
                stBfdIpv6GlbKey.ulDip[2], stBfdIpv6GlbKey.ulDip[3], stBfdIpv6GlbKey.usVrfid);
            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
        }
    }
    
    iRet = SUCCESS;
EXIT_LABEL : NBB_TRC_EXIT();
    return iRet;
}


/*****************************************************************************
 * 函 数 名  : spm_bfd_del_fwd_tree
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年5月28日
 * 函数功能  : 删除转发树
 * 输入参数  : NBB_ULONG ulDiscrimer                本地描述符
               NBB_CCXT_T NBB_CXT                   NBASE多线程参数
 * 输出参数  : 无
 * 返 回 值  : NBB_INT  错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT spm_bfd_del_fwd_tree(NBB_ULONG ulDiscrimer NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    SPM_FWD_GLOBAL          *pstFwdInfo  = NULL;
    
    NBB_TRC_ENTRY(("spm_bfd_del_ip_tree"));
    
    pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &ulDiscrimer);
    if (NULL !=  pstFwdInfo)/* 不为空 */
    {
        /* 若为业务盘，则更新转发表项，删除该项 */
        if (0 == g_usFwdNum)
        {
            NBB_ASSERT(FALSE);
            NBB_TRC_FLOW((NBB_FORMAT"Fwd is empty, Dri = %ld", ulDiscrimer));
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd is empty, Dri = %ld\r\n", ulDiscrimer);
        }
        else/* 删除 */
        {
            /* 删除转发配置 */
            AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
            OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by 蒋博,jiangbo, 20131113, Mantis号:无,
                                                                原因:因为不由nbase管理进程也会分配、释放这块内
                                                                存，不能使用NBB_MM_FREE管理，同时不能被verify */
            pstFwdInfo = NULL;
            iRet = spm_bfd_delfwdtable(ulDiscrimer NBB_CCXT);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Del fwd failed = %d, Dri = %ld", iRet, ulDiscrimer));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del fwd failed = %d, Dri = %ld\r\n", iRet, ulDiscrimer);
            }
            else/* 成功 */
            {
                g_usFwdNum--;
            }
        }
    }
    
    iRet = SUCCESS;
EXIT_LABEL : NBB_TRC_EXIT();
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_addsession
 功能描述  : 添加会话
 输入参数  : IN NBB_ULONG ulDrim:描述符  
             IN ATG_DCI_BFD_BASIC_DATA  *pstBasicBfdCB, 会话基础配置
             IN ATG_DCI_BFD_RSVP_LSP_DATA *pstBasicBfdTunnel,若会话for tunnel则此项不为空
             IN ATG_DCI_BFD_LDP_LSP_DATA    *pstBasicBfdLsp,若会话for lsp则此项不为空
             IN ATG_DCI_BFD_PW_DATA     *pstBasicBfdPw,若会话for pw则此项不为空
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月15日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_addsession(
    IN NBB_ULONG ulDrim, 
    IN ATG_DCI_BFD_BASIC_DATA  *pstBasicBfdCB, 
    IN ATG_DCI_BFD_RSVP_LSP_DATA *pstBasicBfdTunnel,
    IN ATG_DCI_BFD_LDP_LSP_DATA    *pstBasicBfdLsp,
    IN ATG_DCI_BFD_PW_DATA     *pstBasicBfdPw
    NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0; /* 循环变量 */
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;
    CRTXLSP_KEY stCrlspKey;
    FTN_KEY     stLdplspKey;
    NBB_ULONG   ulPortIndex   = 0;          /* 迭代结果的出口索引 */
    NBB_ULONG   ulDriverIndex = 0;          /* 迭代结果，tunnelid或者是lspid */
    ATG_DCI_VC_KEY stVcKey;
    SPM_VP_INFO_CB stVcInfo;
    SPM_PORT_INFO_CB stPortInfo;
    NBB_BYTE ucLocalSlot;
    NBB_BYTE    ucSlotId      = 0;          /* 根据portindex查询的slotid */
    NBB_BYTE    ucPortType    = 0;          /* 根据portindex查询的接口类型 */
    NBB_USHORT  usPortId      = 0;          /* 迭代结果，如果是LAG，选成员端口 */
    NBB_ULONG   aulLagPortIndex[ATG_DCI_LAG_MEMBER_PORT_NUM] = {0};     /* LAG成员端口 */
    NBB_BYTE    ucLagNum = 0;               /* LAG成员端口个数 */
    NBB_USHORT  usLoopj = 0;
    ATG_DCI_PHY_PORT_COMMON_DATA stCommonCfg;
    NBB_ULONG ulPhyPortIndex;

    
    NBB_TRC_ENTRY(("spm_bfd_addsession"));
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));

    /* 判断入参 */
    if (NULL == pstBasicBfdCB)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        goto EXIT_LABEL;
    }

    /* 获取本盘槽位 */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* 添加配置 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if (DRV_ULONG_INVALID == g_stBfdlocal[usLoopi].ulDiscrimer)/* 判断条件 */
        {
            /* 记录配置 */
            NBB_MEMCPY(&(g_stBfdlocal[usLoopi].pstBasicBfdCB), pstBasicBfdCB, sizeof(ATG_DCI_BFD_BASIC_DATA));
            g_stBfdlocal[usLoopi].bBasicBfdCB = ENABLE;

            /* 添加配置，根据配置查找迭代结果 */
            g_stBfdlocal[usLoopi].ulDiscrimer = ulDrim;
            g_stBfdlocal[usLoopi].ucTos = pstBasicBfdCB->service_type;
            g_stBfdlocal[usLoopi].ulSip[0] = pstBasicBfdCB->local_ip[0];
            g_stBfdlocal[usLoopi].ulSip[1] = pstBasicBfdCB->local_ip[1];
            g_stBfdlocal[usLoopi].ulSip[2] = pstBasicBfdCB->local_ip[2];
            g_stBfdlocal[usLoopi].ulSip[3] = pstBasicBfdCB->local_ip[3];
            g_stBfdlocal[usLoopi].ulDip[0] = pstBasicBfdCB->remote_ip[0];
            g_stBfdlocal[usLoopi].ulDip[1] = pstBasicBfdCB->remote_ip[1];
            g_stBfdlocal[usLoopi].ulDip[2] = pstBasicBfdCB->remote_ip[2];
            g_stBfdlocal[usLoopi].ulDip[3] = pstBasicBfdCB->remote_ip[3];
            g_stBfdlocal[usLoopi].ucIptype = pstBasicBfdCB->ip_type;
            g_stBfdlocal[usLoopi].usSport = pstBasicBfdCB->udp_src_port;
            g_stBfdlocal[usLoopi].usTotallength = 52;
            g_stBfdlocal[usLoopi].usUdplength = 32;
            g_stBfdlocal[usLoopi].ucSessionState = BFD_DOWN;
            g_stBfdlocal[usLoopi].ucLocalDiag = bfd_diag_no_diag;
            g_stBfdlocal[usLoopi].ucLength = 24;
            g_stBfdlocal[usLoopi].ucDetectMult = 3;
            g_stBfdlocal[usLoopi].ulLocalDiscr = pstBasicBfdCB->bfd_id;
            g_stBfdlocal[usLoopi].ulRemoteDiscr = pstBasicBfdCB->peer_sid;
            g_stBfdlocal[usLoopi].ucDesiredMinTxInterval = BFD_1000MS;
            g_stBfdlocal[usLoopi].ucRequiredMinRxInterval = BFD_1000MS;
            g_stBfdlocal[usLoopi].ucDM = pstBasicBfdCB->dete_mul;
            g_stBfdlocal[usLoopi].ucDMTI = pstBasicBfdCB->dmti;
            g_stBfdlocal[usLoopi].ucRMRI = pstBasicBfdCB->rmri;
            g_stBfdlocal[usLoopi].ucDetectiontype = pstBasicBfdCB->detect_method;
            g_stBfdlocal[usLoopi].ucSendflag = pstBasicBfdCB->bfd_enable;
            g_stBfdlocal[usLoopi].usSlotid = ucLocalSlot;
            
            /* add begin by limingming, 2013/4/11, Mantis号:HSDxxxxxx, 原因:ID同步 */
            if (0 != pstBasicBfdCB->peer_sid)
            {
                g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
            }
            /* add end by limingming, 2013/4/11 */
            
            if (TRUE != pstBasicBfdCB->bfd_enable)/* 发送不使能 */
            {
                g_stBfdlocal[usLoopi].ucSessionState = BFD_ADMINDOWN;
            }
            
            if (BFD_CFG_IP == pstBasicBfdCB->bfd_frame_type) /* bfd for ip */
            {
                g_stBfdlocal[usLoopi].ucBfdtype = BFD_IP;
                
                /* BFD FOR BGP的情况主控获取不到出接口，需要单盘根据IP查找出接口，并且这种情况BFD肯定是在路由之后下发
                    的，因为只有建立TCP才会建立BFD */
                if ((0x7fffffff == pstBasicBfdCB->phy_if_index) || (0 == pstBasicBfdCB->phy_if_index))
                {
                    if (BFD_IPV4 == pstBasicBfdCB->ip_type) 
                    {
                        /* 根据vrfid+dip查询FTN迭代出端口 */
                        iRet = spm_l3_routeforoam(pstBasicBfdCB->vrf_id, 0, &(pstBasicBfdCB->remote_ip[3]), &ulPortIndex);
                        if (SUCCESS != iRet)/* 若执行失败 */
                        {
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get ftn error %d, vrf_id = %ld remote_ip = %lx\r", 
                                    iRet, pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[3]);
                            BFD_LOCAL_INIT(usLoopi); 
                            goto EXIT_LABEL;
                            
                        }
                    }
                    else if (BFD_IPV6 == pstBasicBfdCB->ip_type)
                    {
                        /* 根据vrfid+dip查询FTN迭代出端口 */
                        iRet = spm_l3_routeforoam(pstBasicBfdCB->vrf_id, 1, &(pstBasicBfdCB->remote_ip[0]), &ulPortIndex);
                        if (SUCCESS != iRet)/* 若执行失败 */
                        {
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, 
                                "Get ipv6 ftn error %d, vrf_id = %ld remote_ip = %lx.%lx.%lx.%lx\r", 
                                 iRet, pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[0], pstBasicBfdCB->remote_ip[1],
                                    pstBasicBfdCB->remote_ip[2], pstBasicBfdCB->remote_ip[3]);
                            BFD_LOCAL_INIT(usLoopi); 
                            goto EXIT_LABEL;
                        }
                    }
                }
                else    /* BFD for OSPS/ISIS BFD可能在路由之后下发 */
                {
                    ulPortIndex = pstBasicBfdCB->phy_if_index;
                }
                
                g_stBfdlocal[usLoopi].ulPortIndex = ulPortIndex;
                
                if ((BFD_IPV6 == pstBasicBfdCB->ip_type) && (0XFE80 == (pstBasicBfdCB->remote_ip[0] >> 16)))
                {
                    iRet = spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "get portid error = %d, phy_if_index = %lu\r\n",
                            iRet, pstBasicBfdCB->phy_if_index);
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    if (ATG_DCI_LAG == stPortInfo.port_type)
                    {
                        g_stBfdlocal[usLoopi].bIsLag = TRUE;
                    }

                    g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_PORT;
                    g_stBfdlocal[usLoopi].usSlotid = stPortInfo.slot_id;
                    g_stBfdlocal[usLoopi].usPortID = stPortInfo.port_id;
                    g_stBfdlocal[usLoopi].usSVlanID = stPortInfo.svlan;
                    g_stBfdlocal[usLoopi].usUnit = stPortInfo.unit_id;
                }
                else
                {
                    g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_IP;
                    g_stBfdlocal[usLoopi].usVrfid = pstBasicBfdCB->vrf_id;
                }
                
                iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* 若执行失败 */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, 
                        "Get port info error=%d, PortIndex = %ld\r\n", iRet, ulPortIndex);
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                
                /* 判断出接口类型 */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* 若为LAG口，返回的portid为lagid */
                    iRet = spm_get_lag_ports(usPortId-ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n", iRet, usPortId);
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    
                    /* 取LAG成员端口所在的第一块盘做为主处理盘 */
                    for (usLoopj = 0; usLoopj < ATG_DCI_LAG_MEMBER_PORT_NUM; usLoopj++)
                    {
                        if (0 != aulLagPortIndex[usLoopj])
                        {
                            break;
                        }
                    }
                    
                    if (ATG_DCI_LAG_MEMBER_PORT_NUM == usLoopj)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Lag info error, could not find useful portindex\r\n");
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    
                    ulPortIndex = aulLagPortIndex[usLoopj];
                }
                
                /* 获取端口延时 */
                iRet = spm_get_common_from_logical_port_index(ulPortIndex, &ulPhyPortIndex, &stCommonCfg NBB_CCXT);
                if (SUCCESS != iRet)/* 反错可能是没有通用TLV配置，使用默认值 */
                {
                    stCommonCfg.up_time = 40;
                    stCommonCfg.down_time = 0;
                }
                g_stBfdlocal[usLoopi].usTimerPortDownToUp = stCommonCfg.up_time;
                g_stBfdlocal[usLoopi].usTimerPortUPToDown = stCommonCfg.down_time;
                g_stBfdlocal[usLoopi].usTimerLiveUp = g_stBfdlocal[usLoopi].usTimerPortDownToUp;
                g_stBfdlocal[usLoopi].usTimerLiveDown = g_stBfdlocal[usLoopi].usTimerPortUPToDown;
            }
            else if (BFD_CFG_Tunnel == pstBasicBfdCB->bfd_frame_type)   /* bfd for rsvp,包括tunnel和lsp */
            {
                g_stBfdlocal[usLoopi].pstBasicBfdTunnel = (ATG_DCI_BFD_RSVP_LSP_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_BFD_RSVP_LSP_DATA), NBB_NORETRY_ACT, MEM_SPM_BFD_RSVP_LSP_DATA_CB);
                if (NULL == g_stBfdlocal[usLoopi].pstBasicBfdTunnel)
                {
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                if(NULL == pstBasicBfdTunnel)   /* bfd for tunnel的情况下，tunnel数据不能为空 */
                {
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                NBB_MEMCPY(g_stBfdlocal[usLoopi].pstBasicBfdTunnel, pstBasicBfdTunnel, 
                    sizeof(ATG_DCI_BFD_RSVP_LSP_DATA));
                g_stBfdlocal[usLoopi].ucBfdtype = BFD_LSP;

                stCrlspKey.egress = pstBasicBfdTunnel->egress;
                stCrlspKey.ingress = pstBasicBfdTunnel->ingress;
                stCrlspKey.tunnelid = pstBasicBfdTunnel->tunnel_id;
                stCrlspKey.lspid    = pstBasicBfdTunnel->lsp_id;
                (NBB_VOID)spm_l3_rsvpforoam(&stCrlspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                
                g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_TUNNELID;
                g_stBfdlocal[usLoopi].usTunnelID = (NBB_USHORT)ulDriverIndex;
                g_stBfdlocal[usLoopi].ulLspLabel = 1;/* 无转发意义 */
                
            }
            else if (BFD_CFG_LSP == pstBasicBfdCB->bfd_frame_type)
            {
                g_stBfdlocal[usLoopi].ucBfdtype = BFD_LSP;
                if (NULL != pstBasicBfdLsp) /* ldp lsp */
                {
                    g_stBfdlocal[usLoopi].pstBasicBfdLsp = (ATG_DCI_BFD_LDP_LSP_DATA *)NBB_MM_ALLOC(
                sizeof(ATG_DCI_BFD_LDP_LSP_DATA), NBB_NORETRY_ACT, MEM_SPM_BFD_LDP_LSP_DATA_CB);
                    if (NULL == g_stBfdlocal[usLoopi].pstBasicBfdLsp)
                    {
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    NBB_MEMCPY(g_stBfdlocal[usLoopi].pstBasicBfdLsp, pstBasicBfdLsp, sizeof(ATG_DCI_BFD_LDP_LSP_DATA));
                    
                    OS_MEMSET(&stLdplspKey, 0, sizeof(FTN_KEY));
                    stLdplspKey.fec = pstBasicBfdLsp->fec_prefix;
                    stLdplspKey.mask = pstBasicBfdLsp->fec_prefix_len;
                    stLdplspKey.vrfid = pstBasicBfdLsp->vrf_id;
                    (NBB_VOID)spm_l3_ldpforoam(&stLdplspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                    /* LDP LSP的时候，spm_l3_ldpforoam返回的是tunnel ID */
                    g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_TUNNELID;
                    g_stBfdlocal[usLoopi].usTunnelID = (NBB_USHORT)ulDriverIndex;
                }
                else /* rsvp lsp */
                {
                    g_stBfdlocal[usLoopi].pstBasicBfdTunnel = (ATG_DCI_BFD_RSVP_LSP_DATA *)NBB_MM_ALLOC(
                        sizeof(ATG_DCI_BFD_RSVP_LSP_DATA), NBB_NORETRY_ACT, MEM_SPM_BFD_RSVP_LSP_DATA_CB);
                    if (NULL == g_stBfdlocal[usLoopi].pstBasicBfdTunnel)
                    {
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    if(NULL == pstBasicBfdTunnel)   /* bfd for rsvp lsp的情况下，rsvp lsp数据不能为空 */
                    {
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    NBB_MEMCPY(g_stBfdlocal[usLoopi].pstBasicBfdTunnel, pstBasicBfdTunnel, 
                        sizeof(ATG_DCI_BFD_RSVP_LSP_DATA));
                    stCrlspKey.egress = pstBasicBfdTunnel->egress;
                    stCrlspKey.ingress = pstBasicBfdTunnel->ingress;
                    stCrlspKey.tunnelid = pstBasicBfdTunnel->tunnel_id;
                    stCrlspKey.lspid    = pstBasicBfdTunnel->lsp_id;
                    (NBB_VOID)spm_l3_rsvpforoam(&stCrlspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                    /* RSVP LSP的时候，spm_l3_rsvpforoam返回的是lsp ID */
                    g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_LSPID;
                    g_stBfdlocal[usLoopi].usLspID = (NBB_USHORT)ulDriverIndex;
                }
                g_stBfdlocal[usLoopi].ulLspLabel = 1;/* 无转发意义 */
            }
            else if (BFD_CFG_PW == pstBasicBfdCB->bfd_frame_type)/* bfd for pw */
            {
                g_stBfdlocal[usLoopi].pstBasicBfdPw = (ATG_DCI_BFD_PW_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_BFD_PW_DATA), NBB_NORETRY_ACT, MEM_SPM_BFD_PW_DATA_CB);
                
                if (NULL == g_stBfdlocal[usLoopi].pstBasicBfdPw)
                {
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                if(NULL == pstBasicBfdPw)   /* bfd for pw的情况下，pw数据不能为空 */
                {
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                NBB_MEMCPY(g_stBfdlocal[usLoopi].pstBasicBfdPw, pstBasicBfdPw, sizeof(ATG_DCI_BFD_PW_DATA));
                g_stBfdlocal[usLoopi].ulPeerId = pstBasicBfdPw->peer_id;
                
                stVcKey.peer_ip = pstBasicBfdPw->peer_id;
                stVcKey.vc_id   = pstBasicBfdPw->vc_id;
                stVcKey.vc_type = pstBasicBfdPw->vc_type;
                iRet = spm_get_vpinfo_from_vckey(&stVcKey, &stVcInfo NBB_CCXT);
                if (SUCCESS != iRet)/* 判断执行结果 */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "get vpinfo error = %d\r\n", iRet);
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                
                g_stBfdlocal[usLoopi].ucBfdtype = BFD_PW_type2;
                g_stBfdlocal[usLoopi].usNHIID = (NBB_USHORT)stVcInfo.next_hop_id;
                g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_PW;
                g_stBfdlocal[usLoopi].ulLspLabel = 1;
                g_stBfdlocal[usLoopi].ulPwLabel  = stVcInfo.pw_tx_label;
            }
            else if (BFD_CFG_INTERFACE == pstBasicBfdCB->bfd_frame_type)/* bfd for interface */
            {
                iRet = spm_get_portid_from_logical_port_index(pstBasicBfdCB->phy_if_index, &stPortInfo NBB_CCXT);
                if (SUCCESS != iRet)/* 判断执行结果 */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "get portid error = %d, phy_if_index = %lu\r\n",
                        iRet, pstBasicBfdCB->phy_if_index);
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                if (ATG_DCI_LAG == stPortInfo.port_type)
                {
                    g_stBfdlocal[usLoopi].bIsLag = TRUE;
                }
                
                g_stBfdlocal[usLoopi].ucBfdtype = BFD_IP;
                g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_PORT;
                g_stBfdlocal[usLoopi].usPortID = stPortInfo.port_id;
                g_stBfdlocal[usLoopi].usSVlanID = stPortInfo.svlan;
                g_stBfdlocal[usLoopi].usUnit = stPortInfo.unit_id;

                ulPortIndex = pstBasicBfdCB->phy_if_index;
                
                g_stBfdlocal[usLoopi].ulPortIndex = ulPortIndex;

                iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* 若执行失败 */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error=%d, PortIndex = %ld\r\n",
                        iRet, ulPortIndex);
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                
                /* 判断出接口类型 */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* 若为LAG口，返回的portid为lagid */
                    iRet = spm_get_lag_ports(usPortId-ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n", iRet, usPortId);
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    
                    /* 取LAG成员端口所在的第一块盘做为主处理盘 */
                    for (usLoopj = 0; usLoopj < ATG_DCI_LAG_MEMBER_PORT_NUM; usLoopj++)
                    {
                        if (0 != aulLagPortIndex[usLoopj])
                        {
                            break;
                        }
                    }
                    
                    if (ATG_DCI_LAG_MEMBER_PORT_NUM == usLoopj)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Lag info error, could not find useful portindex\r\n");
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    
                    ulPortIndex = aulLagPortIndex[usLoopj];
                }
                
                /* 获取端口延时 */
                iRet = spm_get_common_from_logical_port_index(ulPortIndex, &ulPhyPortIndex, &stCommonCfg NBB_CCXT);
                if (SUCCESS != iRet)/* 反错可能是没有通用TLV配置，使用默认值 */
                {
                    stCommonCfg.up_time = 40;
                    stCommonCfg.down_time = 0;
                }
                g_stBfdlocal[usLoopi].usTimerPortDownToUp = stCommonCfg.up_time;
                g_stBfdlocal[usLoopi].usTimerPortUPToDown = stCommonCfg.down_time;
                g_stBfdlocal[usLoopi].usTimerLiveUp = g_stBfdlocal[usLoopi].usTimerPortDownToUp;
                g_stBfdlocal[usLoopi].usTimerLiveDown = g_stBfdlocal[usLoopi].usTimerPortUPToDown;
            }
            
            iRet = spm_bfd_tree_set_data(usLoopi, ulDrim, &stBfdTreeNode);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                BFD_LOCAL_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Set node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set node failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* 加节点 */
            iRet = spm_oamtree_nodeadd(g_usBfdLspNoMax, &stBfdTreeNode, &g_stTreeBfdNodeSet);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                BFD_LOCAL_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Add node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            
            //spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  

            /* 建立会话二叉树 */
            iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
                g_pulBfdTreebase, BFDFGPABASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                BFD_LOCAL_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Build main Tree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build main Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* 翻页写备表 */
            spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
                g_pulBfdTreebase, BFDFGPABASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                BFD_LOCAL_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Build back Tree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build back Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }
            
            spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET, 1);

            /* 存储会话配置 */    
            g_stBfdlocal[usLoopi].bIsSendtoMaster = FALSE;
            g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;

            g_bIsModifySession = TRUE;

            /* 以条目号为索引，将组织好的BFD发送报文内容写FPGA */
            spm_bfd_sendpacket_fpgareg(usLoopi, FALSE NBB_CCXT);

            break;
        }
    }

    /* 没有找到空余条目，说明异常，可能在删除会话时未还原初始化 */
    if (BFD_MAX_SESNUM == usLoopi)
    {
        NBB_TRC_FLOW((NBB_FORMAT"Error, please check !!!!"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Error, please check !!!!\r\n");
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();
    return iRet;
}


/*****************************************************************************
 函 数 名  : spm_bfd_delsession
 功能描述  : 删除会话
 输入参数  : IN NBB_ULONG ulDrim : 描述符 
             NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月15日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_delsession(IN NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;

    NBB_TRC_ENTRY(("spm_bfd_delsession"));
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    
    /* 轮询本地会话配置 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if (g_stBfdlocal[usLoopi].ulDiscrimer == ulDrim)/* 判断条件 */
        {
            iRet = spm_bfd_tree_set_data(usLoopi, ulDrim, &stBfdTreeNode);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Set node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            
            /* 删除节点 */
            iRet = spm_oamtree_nodedel(g_usBfdLspNoMax, &stBfdTreeNode, &g_stTreeBfdNodeSet);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            
            //spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
            
            /* 重建会话二叉树 */
            iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
                g_pulBfdTreebase, BFDFGPABASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del node failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* 翻页写备表 */
            spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
                g_pulBfdTreebase, BFDFGPABASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Build back Tree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build back Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }
            
            spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET, 1);

            /* admindown情况立刻删除,其他情况因为可能在跑状态机，所以要在状态机线程里面删除 */
            if (BFD_ADMINDOWN == g_stBfdlocal[usLoopi].ucSessionState)
            {
                if (TRUE == g_stBfdlocal[usLoopi].bIsChangeToDown)
                {
                    if (g_ulStateToDownNum > 0)
                    {
                        g_ulStateToDownNum--;
                    }
                }
                
                /* 将会话信息至为无效值 */
                BFD_LOCAL_INIT(usLoopi);
                g_bIsModifySession = TRUE;
                
                /* 清空FPGA对应的发送寄存器内容 */
                spm_bfd_clear_sendfpgareg(usLoopi);

                /*清空FPGA BFD攻击寄存器内容*/
                spm_bfd_clear_recv_fpgareg(usLoopi);
            }
            else
            {
                if (ENABLE == g_stBfdlocal[usLoopi].bBasicBfdCB)
                {
                    g_stBfdlocal[usLoopi].bBasicBfdCB = DISABLE;
                }
                if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdTunnel)
                {
                    NBB_MM_FREE(g_stBfdlocal[usLoopi].pstBasicBfdTunnel, MEM_SPM_BFD_RSVP_LSP_DATA_CB);
                    g_stBfdlocal[usLoopi].pstBasicBfdTunnel = NULL;
                }
                if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdLsp)
                {
                    NBB_MM_FREE(g_stBfdlocal[usLoopi].pstBasicBfdLsp, MEM_SPM_BFD_LDP_LSP_DATA_CB);
                    g_stBfdlocal[usLoopi].pstBasicBfdLsp = NULL;
                }
                if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdPw)
                {
                    NBB_MM_FREE(g_stBfdlocal[usLoopi].pstBasicBfdPw, MEM_SPM_BFD_PW_DATA_CB);
                    g_stBfdlocal[usLoopi].pstBasicBfdPw = NULL;
                }
                g_stBfdlocal[usLoopi].ucbIsNeedDelete = TRUE;
            }
            break;
        }
    }

    if (BFD_MAX_SESNUM == usLoopi)/* 判断条件 */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Do not find, Disrim = %ld", ulDrim));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Do not find, Disrim = %ld\r\n", ulDrim);
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_updatesession
 功能描述  : 会话修改参数和状态
 输入参数  : IN NBB_ULONG ulDrim : 描述符                                          
             IN ATG_DCI_BFD_BASIC_DATA  *pstBasicBfdCB : 会话基础配置
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月5日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_updatesession(
    IN NBB_ULONG ulDrim, 
    IN ATG_DCI_BFD_BASIC_DATA  *pstBasicBfdCB
    NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT  iRet = ERROR;

    NBB_TRC_ENTRY(("spm_bfd_updatesession"));

    if(NULL == pstBasicBfdCB)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "bfd basic CB is null!\r\n");
        iRet = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* 循环 */
    {
        if (ulDrim == g_stBfdlocal[usLoopi].ulDiscrimer)/* 描述符匹配 */
        {
            if (TRUE != pstBasicBfdCB->bfd_enable)/* 发送不使能 */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Change session to admin, Drim = %ld\r\n", ulDrim);
                if (BFD_ADMINDOWN != g_stBfdlocal[usLoopi].ucSessionState)/* 不为admin */
                {
                    g_stBfdlocal[usLoopi].ucSessionState = BFD_ADMINDOWN;
                    spm_bfd_state_fsm_admindown(usLoopi);
                    g_stBfdlocal[usLoopi].ucSendflag = pstBasicBfdCB->bfd_enable;
                    iRet = SUCCESS;
                    goto EXIT_LABEL;
                }
            }
            else
            {
                if (BFD_ADMINDOWN == g_stBfdlocal[usLoopi].ucSessionState)
                {
                    g_stBfdlocal[usLoopi].ucTos = pstBasicBfdCB->service_type;
                    g_stBfdlocal[usLoopi].usSport = pstBasicBfdCB->udp_src_port;
                    if (TRUE == pstBasicBfdCB->peer_sid_selflearn_enable)
                    {
                        g_stBfdlocal[usLoopi].ulRemoteDiscr = 0;
                        g_stBfdlocal[usLoopi].usBfdflag = BFD_NO_CREATE;
                    }
                    else
                    {
                        g_stBfdlocal[usLoopi].ulRemoteDiscr = pstBasicBfdCB->peer_sid;
                    }
                    g_stBfdlocal[usLoopi].ucSendflag = pstBasicBfdCB->bfd_enable;
                    g_stBfdlocal[usLoopi].ucRMRI = pstBasicBfdCB->rmri;
                    g_stBfdlocal[usLoopi].ucDMTI = pstBasicBfdCB->dmti;
                    g_stBfdlocal[usLoopi].ucDM = pstBasicBfdCB->dete_mul;
                    spm_bfd_state_fsm_down(usLoopi,BFD_STA_BUTT);
                    iRet = SUCCESS;
                    goto EXIT_LABEL;
                }
            }
            
            g_stBfdlocal[usLoopi].ucTos = pstBasicBfdCB->service_type;
            g_stBfdlocal[usLoopi].usSport = pstBasicBfdCB->udp_src_port;
            g_stBfdlocal[usLoopi].ulRemoteDiscr = pstBasicBfdCB->peer_sid;
            g_stBfdlocal[usLoopi].ucSendflag = pstBasicBfdCB->bfd_enable;

            if (g_stBfdlocal[usLoopi].ucRMRI != pstBasicBfdCB->rmri)/* 判断rmri */
            {
                g_stBfdlocal[usLoopi].ucRMRI = pstBasicBfdCB->rmri;
                g_stBfdlocal[usLoopi].ucParaChangeFlag |= BFD_PARA_INTERVAL_CHANGE;
            }

            if (g_stBfdlocal[usLoopi].ucDMTI != pstBasicBfdCB->dmti)/* 判断dmti */
            {
                g_stBfdlocal[usLoopi].ucDMTI = pstBasicBfdCB->dmti;
                g_stBfdlocal[usLoopi].ucParaChangeFlag |= BFD_PARA_INTERVAL_CHANGE;
            }

            if (g_stBfdlocal[usLoopi].ucDM != pstBasicBfdCB->dete_mul)
            {
                g_stBfdlocal[usLoopi].ucDM = pstBasicBfdCB->dete_mul;
                g_stBfdlocal[usLoopi].ucParaChangeFlag |= BFD_PARA_DM_CHANGE;
            }

            spm_bfd_sendpacket_fpgareg(usLoopi, FALSE NBB_CCXT);
            break;
        }
    }

    /* 若没有找到会话，则说明有问题 */
    if (BFD_MAX_SESNUM == usLoopi)
    {
        NBB_ASSERT(FALSE);
        NBB_TRC_FLOW((NBB_FORMAT"Do not find, Disrim = %ld", ulDrim));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Do not find, Disrim = %ld\r\n", ulDrim);
        goto EXIT_LABEL;
    }

    iRet = SUCCESS;

    EXIT_LABEL : NBB_TRC_EXIT();
    return iRet;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_update_common_from_physical_port_index
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年3月17日
 * 函数功能  : 根据物理接口index更新BFD的延时
 * 输入参数  : NBB_ULONG ulPhyPortIndex     物理接口index
               NBB_CCXT_T NBB_CXT           nbase多线程参数
 * 输出参数  : 无
 * 返 回 值  : NBB_INT  NBB多线程参数
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT spm_bfd_update_common_from_physical_port_index(NBB_ULONG ulPhyPortIndex
                                                         NBB_CCXT_T NBB_CXT)
{
    NBB_INT  iRet = SUCCESS;
    NBB_USHORT usLoopi = 0; /* 循环变量 */
    NBB_USHORT usLoopj = 0; /* 循环变量 */
    NBB_BYTE    ucSlotId      = 0;          /* 根据portindex查询的slotid */
    NBB_BYTE    ucPortType    = 0;          /* 根据portindex查询的接口类型 */
    NBB_USHORT  usPortId      = 0;          /* 迭代结果，如果是LAG，选成员端口 */
    NBB_ULONG   aulLagPortIndex[ATG_DCI_LAG_MEMBER_PORT_NUM] = {0};     /* LAG成员端口 */
    NBB_BYTE    ucLagNum = 0;               /* LAG成员端口个数 */
    NBB_ULONG   ulPortIndex   = 0;          /* 迭代结果的出口索引 */
    NBB_ULONG   ulLocalPhyPortIndex;
    ATG_DCI_PHY_PORT_COMMON_DATA stCommonCfg;

    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++) /* 对变量usLoopi进行循环 */
    {
        if (DRV_ULONG_INVALID != g_stBfdlocal[usLoopi].ulDiscrimer)/* 已配置BFD */
        {
            if ((BFD_FWD_PORT == g_stBfdlocal[usLoopi].ucBfdFwdType) /* bfd for interface或bfd for linklocal ipv6 */
                || (BFD_FWD_IP == g_stBfdlocal[usLoopi].ucBfdFwdType))  /* 或bfd for ip */
            {
                ulPortIndex = g_stBfdlocal[usLoopi].ulPortIndex;

                iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* 若执行失败 */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                        "Get port info error=%d, PortIndex = %ld\r\n", iRet, ulPortIndex);
                    goto EXIT_LABEL;
                }

                /* 判断出接口类型 */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* 若为LAG口，返回的portid为lagid */
                    iRet = spm_get_lag_ports(usPortId-ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* 判断执行结果 */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n", iRet, usPortId);
                        goto EXIT_LABEL;
                    }
                    
                    /* 取LAG成员端口所在的第一个成员端口 */
                    for (usLoopj = 0; usLoopj < ATG_DCI_LAG_MEMBER_PORT_NUM; usLoopj++)
                    {
                        if (0 != aulLagPortIndex[usLoopj])
                        {
                            break;
                        }
                    }
                    
                    if (ATG_DCI_LAG_MEMBER_PORT_NUM == usLoopj)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Lag info error, could not find useful portindex\r\n");
                        goto EXIT_LABEL;
                    }
                    
                    ulPortIndex = aulLagPortIndex[usLoopj];
                }

                /* 获取端口延时 */
                iRet = spm_get_common_from_logical_port_index(ulPortIndex, &ulLocalPhyPortIndex, &stCommonCfg NBB_CCXT);
                if (SUCCESS != iRet)/* 反错可能是没有通用TLV配置，找下一条 */
                {
                    iRet = SUCCESS;
                    continue;
                }

                if (ulLocalPhyPortIndex == ulPhyPortIndex)
                {
                    g_stBfdlocal[usLoopi].usTimerPortDownToUp = stCommonCfg.up_time;
                    g_stBfdlocal[usLoopi].usTimerPortUPToDown = stCommonCfg.down_time;
                }
            }
        }
    }
    
    EXIT_LABEL :
    return iRet;
}


/*****************************************************************************
 函 数 名  : spm_bfd_addfwdtable
 功能描述  : 添加转发表
 输入参数  : IN NBB_ULONG ulDrim    :描述符                      
             IN NBB_USHORT usSlotid :主处理盘
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月29日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_addfwdtable(IN NBB_ULONG ulDrim, IN NBB_USHORT usSlotid)
{
    NBB_USHORT usLoopi = 0; /* 循环变量 */
    
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;
    
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add fwd %lu begin, slot %d\r\n", ulDrim, usSlotid);
    /* 添加配置 */
    for (usLoopi = 0; usLoopi < BFD_MAX_FWDNUM; usLoopi++)
    {
        if (DRV_ULONG_INVALID == g_stBfdfwd[usLoopi].ulDiscrimer)/* 可添加 */
        {            
            g_stBfdfwd[usLoopi].usSlotid = usSlotid;
            g_stBfdfwd[usLoopi].ulDiscrimer = ulDrim;
            
            iRet = spm_fwd_tree_set_data(usLoopi, ulDrim, &stBfdTreeNode);
            if (SUCCESS != iRet)/* 不成功 */
            {
                /* 将会话信息至为无效值 */
                BFD_FWD_INIT(usLoopi);
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Set fwd node failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* 加节点 */
            iRet = spm_oamtree_nodeadd(g_usFwdLspNoMax, &stBfdTreeNode, &g_stTreeFwdNodeSet);
            if (SUCCESS != iRet)/* 不成功 */
            {
                /* 将会话信息至为无效值 */
                BFD_FWD_INIT(usLoopi);
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Add fwd node failed, Discri = %ld, iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  

            iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
                g_pulFwdTreebase, OAMFWDFPAGBASE);
            
            if (SUCCESS != iRet) /* 判断执行结果 */
            {
                /* 将会话信息至为无效值 */
                BFD_FWD_INIT(usLoopi);
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Build fwd main Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* 翻页写备表 */
            spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
                g_pulFwdTreebase, OAMFWDFPAGBASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                BFD_FWD_INIT(usLoopi);
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Build fwd back Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);

            /* 以条目号为索引，将组织好的转发内容写FPGA */
            spm_bfd_fwdpacket_fpgareg(usLoopi);
            g_uiFwdTableCount++;
            break;
            
        }
    }

    /* 没有找到空余条目，说明异常，可能在删除会话时未还原初始化 */
    if (BFD_MAX_FWDNUM == usLoopi)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Error, please check !!!!\n");
        goto EXIT_LABEL;
    }
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add fwd %lu end, slot %d\r\n", ulDrim, usSlotid);


    EXIT_LABEL :
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_delfwdtable
 功能描述  : 删除转发表项
 输入参数  : IN NBB_ULONG ulDrim  :描述符
             NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月15日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_delfwdtable(IN NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;

    NBB_TRC_ENTRY(("spm_bfd_delfwdtable"));
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete fwd %lu begin\r\n", ulDrim);
    /* 轮询本地会话配置 */
    for (usLoopi = 0; usLoopi < BFD_MAX_FWDNUM; usLoopi++)
    {
        if (g_stBfdfwd[usLoopi].ulDiscrimer == ulDrim)/* 判断条件 */
        {
            iRet = spm_fwd_tree_set_data(usLoopi, ulDrim, &stBfdTreeNode);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                /* 将会话信息至为无效值 */
                BFD_FWD_INIT(usLoopi);
                
                /* 清空FPGA对应的发送寄存器内容 */
                spm_bfd_clear_fwdfpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Set node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }

            /* 删除节点 */
            iRet = spm_oamtree_nodedel(g_usFwdLspNoMax, &stBfdTreeNode, &g_stTreeFwdNodeSet);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                /* 将会话信息至为无效值 */
                BFD_FWD_INIT(usLoopi);
                
                /* 清空FPGA对应的发送寄存器内容 */
                spm_bfd_clear_fwdfpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  

            /* 重建会话二叉树 */
            iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
                g_pulFwdTreebase, OAMFWDFPAGBASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                /* 将会话信息至为无效值 */
                BFD_FWD_INIT(usLoopi);
                
                /* 清空FPGA对应的发送寄存器内容 */
                spm_bfd_clear_fwdfpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"rebuild oamtree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "rebuild oamtree  failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }
            
            /* 翻页写备表 */
            spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
                g_pulFwdTreebase, OAMFWDFPAGBASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                BFD_FWD_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Build fwd back Tree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build fwd back Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }
            spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);
            
            /* 将会话信息至为无效值 */
            BFD_FWD_INIT(usLoopi);

            /* 清空FPGA对应的发送寄存器内容 */
            spm_bfd_clear_fwdfpgareg(usLoopi NBB_CCXT);
            
            break;
        }
    }

    if (BFD_MAX_FWDNUM == usLoopi)/* 判断条件 */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Not exit , Disrim = %ld", ulDrim));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Not exit , Disrim = %ld\r\n", ulDrim);
        goto EXIT_LABEL;
    }
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete fwd end\r\n");
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_clear_recivefpgareg
 功能描述  : 会话删除时清除FPGA BFD_RX中会话收寄存器内容
 输入参数  : IN NBB_USHORT usLocalNum :本地条目号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月28日
    作    者   : tianf
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_clear_recv_fpgareg(IN NBB_USHORT usLocalNum)
{
    NBB_USHORT usData = 0;

    spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x1, usData);
    
    /*本端YID与收包MYID匹配置位去使能*/
    spm_oamtree_s16write(BFDSESSIONBASE, 0x5000 + usLocalNum, 0);
    return 0;
}

/*****************************************************************************
 函 数 名  : spm_bfd_set_anti_attack
 功能描述  : 设置FPGA BFD_RX中会话收寄存器BFD防攻击参数
 输入参数  : IN NBB_USHORT usLocalNum :本地条目号
                IN NBB_USHORT usDate :使能值 0/1=不使能/使能
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月28日
    作    者   : tianf
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_set_anti_attack(IN NBB_USHORT usLocalNum, IN NBB_USHORT usDate)
{
    NBB_INT iRet = SUCCESS;
    
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set bfd anti attack, input num %d is wrong!\r\n", usLocalNum);
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    if ((usDate != 0) && (usDate != 1))
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set bfd anti attack, input num %d is wrong!\r\n", usDate);
        iRet = ERROR;
        goto EXIT_LABEL;
    }
    
    if(1 == usDate)
    {
        /* BFD收模块寄存器偏移0x4000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
        spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x0, 
        (g_stBfdlocal[usLocalNum].ulRemoteDiscr)&DRV_USHORT_INVALID);
        
         /* BFD收模块寄存器偏移0x4000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
        spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x1, 
            (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
            
        /*本端YID与收包MYID匹配置位使能*/
        spm_oamtree_s16write(BFDSESSIONBASE, 0x5000 + usLocalNum, 1);    
    }
    else
    {
        spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x0, 0);
        spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x1, 0);
        spm_oamtree_s16write(BFDSESSIONBASE, 0x5000 + usLocalNum, 0);      
    }
    
    EXIT_LABEL : 
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_clear_sendfpgareg
 功能描述  : 会话删除时清除FPGA中会话发送寄存器内容
 输入参数  : IN NBB_USHORT usLocalNum :本地条目号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月17日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_clear_sendfpgareg(IN NBB_USHORT usLocalNum)
{
    NBB_USHORT usData = 0;
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1000 + usLocalNum, usData);
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x1, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x2, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData);
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x1, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData);

    spm_oamtree_s16write(BFDPAKFGPABASE, 0x14000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x15000 + usLocalNum, usData);
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x4000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x5000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x6000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x7000 + usLocalNum, usData);

    spm_oamtree_s16write(BFDPAKFGPABASE, 0x16000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x17000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x18000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x19000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1a000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1b000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1c000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1d000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1e000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1f000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x20000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x21000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x22000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x23000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x24000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x25000 + usLocalNum, usData);
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x8000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x9000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xa000 + usLocalNum, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xb000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xb000 + 2 * usLocalNum + 0x1, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x1, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xd000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xd000 + 2 * usLocalNum + 0x1, usData);
    
    /* 0xe000 只读 */
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x1, usData);
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x11000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x11000 + 2 * usLocalNum + 0x1, usData);
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x26000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x26000 + 2 * usLocalNum + 0x1, usData);

    /* 0xf000 只读 */

    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLocalNum + 0x0,usData);
    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLocalNum + 0x1,usData);
    
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_clear_fwdfpgareg
 功能描述  : 会话删除时清除FPGA中转发寄存器内容
 输入参数  : IN NBB_USHORT usLocalNum :本地条目号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月17日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_clear_fwdfpgareg(IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usData = 0;
    
    NBB_TRC_ENTRY(("spm_bfd_clear_fwdfpgareg"));
    spm_oamtree_s16write(OAMFWDPAKFPGABASE, 8 * usLocalNum, usData);
    
    spm_oamtree_s16write(OAMFWDPAKFPGABASE, 0x01 + 8 * usLocalNum, usData);
    spm_oamtree_s16write(OAMFWDPAKFPGABASE, 0x02 + 8 * usLocalNum, usData);
    spm_oamtree_s16write(OAMFWDPAKFPGABASE, 0x03 + 8 * usLocalNum, usData);
    
    spm_oamtree_s16write(OAMFWDPAKFPGABASE, 0x04 + 8 * usLocalNum, usData);
    spm_oamtree_s16write(OAMFWDPAKFPGABASE, 0x05 + 8 * usLocalNum, usData);
    
    NBB_TRC_EXIT();
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_fwdpacket_fpgareg
 功能描述  : 配置BFD转发报文的寄存器
 输入参数  : IN NBB_USHORT usLocalNum : 本地条目号 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月30日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_fwdpacket_fpgareg(IN NBB_USHORT usLocalNum)
{
    SPM_BFD_FWD_PACKET1 stBfdFwd1;
    SPM_BFD_FWD_PACKET2 stBfdFwd2;
    SPM_BFD_FWD_PACKET3 stBfdFwd3;
    NBB_USHORT           usData1     = 0;
    NBB_USHORT           usData2     = 0;
    NBB_USHORT           usData3     = 0;
    NBB_BYTE    ucLocalSlot   = 0;          /* 本盘槽位号 */
    

    /* 入参判断 */
    if (usLocalNum >= BFD_MAX_FWDNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fwd fpga, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }
    OS_MEMSET(&stBfdFwd1, 0, sizeof(SPM_BFD_FWD_PACKET1));
    OS_MEMSET(&stBfdFwd2, 0, sizeof(SPM_BFD_FWD_PACKET2));
    OS_MEMSET(&stBfdFwd3, 0, sizeof(SPM_BFD_FWD_PACKET3));

    /* 判断是否是转发至主控盘，若是，则走组播 */
    if (0 == spm_hardw_getboardtype(g_stBfdfwd[usLocalNum].usSlotid))
    {
        stBfdFwd1.usFwdTrafficClass = 0; /* 报文走arad优先级 */
        stBfdFwd1.usFwdDesInfo1 = (NBB_USHORT)((BFD_MULTI_ID >> 16)&0x7);
        usData1 = *(NBB_USHORT *)(&stBfdFwd1);
        spm_oamtree_s16write(OAMFWDPAKFPGABASE, 8 * usLocalNum + 0x0, usData1);
        
        usData2 = (NBB_USHORT)(BFD_MULTI_ID&DRV_USHORT_INVALID);
        spm_oamtree_s16write(OAMFWDPAKFPGABASE, 8 * usLocalNum + 0x1, usData2);
        stBfdFwd3.usDslotid = 31; /* 组播发送，Dslot填31 */
    }
    else/* 单播 */
    {
        stBfdFwd1.usFwdTrafficClass = 7; /* 报文走arad优先级 */
        stBfdFwd1.usFwdDesInfo1 = 0xc;
        usData1 = *(NBB_USHORT *)(&stBfdFwd1);
        spm_oamtree_s16write(OAMFWDPAKFPGABASE, 8 * usLocalNum + 0x0, usData1);

        stBfdFwd2.usSysPortID = g_stBfdfwd[usLocalNum].usSlotid * 256 + BFD_ARAD_LOGICPORT; 
        usData2 = *(NBB_USHORT *)(&stBfdFwd2);
        spm_oamtree_s16write(OAMFWDPAKFPGABASE, 8 * usLocalNum + 0x1, usData2);
        stBfdFwd3.usDslotid = g_stBfdfwd[usLocalNum].usSlotid;
    }
    
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    stBfdFwd3.usSslotid = ucLocalSlot;
    stBfdFwd3.usPktType = BFD_DUNE_FROMC3;
    usData3 = *(NBB_USHORT *)(&stBfdFwd3);
    spm_oamtree_s16write(OAMFWDPAKFPGABASE, 8 * usLocalNum + 0x3, usData3);
    
    EXIT_LABEL :
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_sendpacket_fpgareg
 功能描述  : 配置BFD发送报文的寄存器
 输入参数  : IN NBB_USHORT usLocalNum  :本地条目号
             IN NBB_BOOL bIsModify     :是否是修改参数操作，若为修改参数则需要置位某个bit位的值；
             输入值为FALSE/TRUE
                
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月12日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_fpgareg(IN NBB_USHORT usLocalNum, IN NBB_BOOL bIsModify NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT   usData = 0;
    NBB_ULONG    ulLsplabel = 0;
    NBB_ULONG    ulPwlabel = 0;
    SPM_BFD_REG_PACKET1 stPacketInfo1;
    SPM_BFD_MPLSLABEL   stMplsLabel1;
    SPM_BFD_MPLSLABEL   stMplsLabel2;
    
    NBB_TRC_ENTRY(("spm_bfd_sendpacket_fpgareg"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Set send packet fpga, input num %d is wrong!", usLocalNum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set send packet fpga, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stPacketInfo1, 0, sizeof(SPM_BFD_REG_PACKET1));
    OS_MEMSET(&stMplsLabel1, 0, sizeof(SPM_BFD_MPLSLABEL));
    OS_MEMSET(&stMplsLabel2, 0, sizeof(SPM_BFD_MPLSLABEL));

    /* DMAC+SMAC共12字节，封装走转发平面，格式为reserved(1byte)+SVLAN(2bytes)+LSPID(2bytes)+ reserved(5bits)+FWDTYPE(3bits)
        + vrfid(2byte)+ipv4address(4bytes), 主控盘上的BFD需要先发到一块业务盘算，然后走转发平面，因此主控的格式
        为 ITMH(4bytes)+PKT_TYPE(3bits)+Dslotid(5bits)+Sslotid(5bits)+FWDTYPE(3bits)+vrfid(2byte)+
        ipv4address(4bytes) */
    
    spm_bfd_sendpacket_macreg(usLocalNum);

    spm_bfd_sendpacket_intervalreg(usLocalNum, bIsModify);

    /* 报文内容 */
    if (BFD_IPV4 == g_stBfdlocal[usLocalNum].ucIptype)
    {
        spm_bfd_sendpacket_ipv4fpga(usLocalNum);
    }
    else /* ipv6 */
    {
        spm_bfd_sendpacket_ipv6fpga(usLocalNum);
    }
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x8000 + usLocalNum, g_stBfdlocal[usLocalNum].usSport);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x9000 + usLocalNum, g_stBfdlocal[usLocalNum].usUdplength);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xa000 + usLocalNum, g_stBfdlocal[usLocalNum].ucLength);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xb000 + 2 * usLocalNum + 0x0, 
        (g_stBfdlocal[usLocalNum].ulLocalDiscr >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xb000 + 2 * usLocalNum + 0x1, 
        (g_stBfdlocal[usLocalNum].ulLocalDiscr)&DRV_USHORT_INVALID);
    /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x0, 
        (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
    /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x1, 
        (g_stBfdlocal[usLocalNum].ulRemoteDiscr)&DRV_USHORT_INVALID);

    if (BFD_LSP == g_stBfdlocal[usLocalNum].ucBfdtype)/* lsp */
    {
        stMplsLabel1.ulMplsLabel = g_stBfdlocal[usLocalNum].ulLspLabel;
        stMplsLabel1.ulStack = 1;
        stMplsLabel1.ulExp = BFD_DEFAULT_MPLS_EXP;
        stMplsLabel1.ulTtl = BFD_DEFAULT_MPLS_TTL;
        ulLsplabel = *(NBB_ULONG *)(&stMplsLabel1);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x0,
            (ulLsplabel >> 16)&DRV_USHORT_INVALID);
        
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x1,
            ulLsplabel&DRV_USHORT_INVALID);
    }

    /* pw */
    else if ((BFD_PW_type1 == g_stBfdlocal[usLocalNum].ucBfdtype)
        || (BFD_PW_type2 == g_stBfdlocal[usLocalNum].ucBfdtype))
    {
        stMplsLabel1.ulMplsLabel = g_stBfdlocal[usLocalNum].ulLspLabel;
        stMplsLabel1.ulStack = 0;
        stMplsLabel1.ulExp = BFD_DEFAULT_MPLS_EXP;
        stMplsLabel1.ulTtl = BFD_DEFAULT_MPLS_TTL;
        ulLsplabel = *(NBB_ULONG *)(&stMplsLabel1);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x0,
            (ulLsplabel >> 16)&DRV_USHORT_INVALID);
        
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x1,
            ulLsplabel&DRV_USHORT_INVALID);

        stMplsLabel2.ulMplsLabel = g_stBfdlocal[usLocalNum].ulPwLabel;
        stMplsLabel2.ulStack = 1;
        stMplsLabel2.ulExp = BFD_DEFAULT_MPLS_EXP;
        stMplsLabel2.ulTtl = BFD_DEFAULT_MPLS_TTL;
        ulPwlabel = *(NBB_ULONG *)(&stMplsLabel2);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x11000 + 2 * usLocalNum + 0x0,
            (ulPwlabel >> 16)&DRV_USHORT_INVALID);
        
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x11000 + 2 * usLocalNum + 0x1,
            ulPwlabel&DRV_USHORT_INVALID);

        if (BFD_PW_type2 == g_stBfdlocal[usLocalNum].ucBfdtype)
        {
            spm_oamtree_s16write(BFDPAKFGPABASE, 0x26000 + 2 * usLocalNum + 0x0, 0x1000);
            spm_oamtree_s16write(BFDPAKFGPABASE, 0x26000 + 2 * usLocalNum + 0x1, 0x0021);
        }
    }
    
    stPacketInfo1.usSendFlag = g_stBfdlocal[usLocalNum].ucSendflag;

    /* UDP端口号 */
    stPacketInfo1.usUDPtype = g_stBfdlocal[usLocalNum].ucDetectiontype;

    /* 判断封装类型 */
    stPacketInfo1.usSendType = g_stBfdlocal[usLocalNum].ucBfdtype;
    if((BFD_FWD_PORT == g_stBfdlocal[usLocalNum].ucBfdFwdType) && (g_stBfdlocal[usLocalNum].usSVlanID != 0))
    {
       stPacketInfo1.usVlanAble = FALSE; //插入VLAN使能
    }
    else
    {
        stPacketInfo1.usVlanAble = TRUE; //插入VLAN不使能
    }
    
    if (BFD_IPV6 == g_stBfdlocal[usLocalNum].ucIptype)
    {
        stPacketInfo1.usIpType = TRUE;
    }
    usData = *(NBB_USHORT *)(&stPacketInfo1);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1000 + usLocalNum, usData);
        
    EXIT_LABEL : NBB_TRC_EXIT();
    return;
    
}

/*****************************************************************************
 函 数 名  : spm_bfd_sendpacket_intervalreg
 功能描述  : 修改频率/状态寄存器函数
 输入参数  : IN NBB_USHORT usLocalNum  :本地条目号
             IN NBB_BOOL bIsModify  :是否是修改参数操作，若为修改参数则需要置位某个bit位的值；
             输入值为FALSE/TRUE   
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月13日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_intervalreg(IN NBB_USHORT usLocalNum, IN NBB_BOOL bIsModify)
{
    NBB_USHORT   usData1 = 0;
    NBB_USHORT   usData2 = 0;
    SPM_BFD_REG_PACKET2 stPacketInfo2;
    SPM_BFD_REG_PACKET3 stPacketInfo3;

    //NBB_TRC_ENTRY(("spm_bfd_sendpacket_intervalreg"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set send packet Interval fpga, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stPacketInfo2, 0, sizeof(SPM_BFD_REG_PACKET2));
    stPacketInfo2.usDiag = g_stBfdlocal[usLocalNum].ucLocalDiag;
    stPacketInfo2.usSta = g_stBfdlocal[usLocalNum].ucSessionState;
    stPacketInfo2.usDectMult = g_stBfdlocal[usLocalNum].ucDetectMult;
    usData1 = *(NBB_USHORT *)(&stPacketInfo2);
    /* BFD发送模块寄存器偏移0xd000+2*N+0x0：
       Bit15-11：BFD报文诊断码（Diag），表明发送方最近一次会话down的原因。
       Bit10-9：BFD当前会话状态（Sta），0-AdminDown，1-Down，2-Init，3-Up。
       Bit7-0：检测时间倍数（Detect Mult）。默认值为3 */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xd000 + 2 * usLocalNum + 0x0, usData1);

    OS_MEMSET(&stPacketInfo3, 0, sizeof(SPM_BFD_REG_PACKET3));
    stPacketInfo3.usRxInterval = g_stBfdlocal[usLocalNum].ucRequiredMinRxInterval;
    stPacketInfo3.usTxInterval = g_stBfdlocal[usLocalNum].ucDesiredMinTxInterval;
    stPacketInfo3.usModify     = bIsModify;
    usData2 = *(NBB_USHORT *)(&stPacketInfo3);
    /* BFD发送模块寄存器偏移0xd000+2*N+0x1：
       Bit0：上述2个间隔中任何一个需要被修改，由软件写'1'，启动投票进程。由FPGA内部清零。 */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xd000 + 2 * usLocalNum + 0x1, usData2);

    usLocalNum = 0x8000 + usLocalNum;
    /* BFD发送模块寄存器偏移0x0002：
       Bit15: 修改BFD报文中5种动态参数（Diag/ Sta/ Detect Mult/DMTI/RMRI）标识。由软件在每条配置5种动态参数完成后写'1'。
       Bit10-0：所配置BFD报文5种动态参数的条目号。由软件在配置参数前写入要配置参数的条目号。*/
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x0002, usLocalNum);
    
    EXIT_LABEL : /*NBB_TRC_EXIT();*/
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_state_fsm
 功能描述  : 根据FPGA中对应条目会话的接收到的报文状态，修改本条目的会话状态
 输入参数  : IN NBB_USHORT usLocalNum :条目号，调用此函数时保证条目号对应的会话有记录 
             IN NBB_BYTE ucSessionState : 收到的报文的状态
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月18日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm(IN NBB_USHORT usLocalNum, IN NBB_BYTE ucSessionState)
{
    //NBB_TRC_ENTRY(("spm_bfd_state_fsm"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fsm, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }

    /* 当前为admindown，任何状态不处理 */
    if (BFD_ADMINDOWN == g_stBfdlocal[usLocalNum].ucSessionState)
    {
        goto EXIT_LABEL;
    }
    
    switch(ucSessionState)
    {
        case BFD_ADMINDOWN:
        if (BFD_DOWN != g_stBfdlocal[usLocalNum].ucSessionState)/* 不为down */
        {
            g_stBfdlocal[usLocalNum].ucLocalDiag = bfd_diag_admin_down;
            if (ENABLE == g_stBfdlocal[usLocalNum].bBasicBfdCB)
            {
                if (TRUE == g_stBfdlocal[usLocalNum].pstBasicBfdCB.peer_sid_selflearn_enable)
                {
                    g_stBfdlocal[usLocalNum].ulRemoteDiscr = 0;
                    g_stBfdlocal[usLocalNum].usBfdflag = BFD_NO_CREATE;
                }
            }
            spm_bfd_state_fsm_down(usLocalNum, BFD_ADMINDOWN);
            g_stBfdlocal[usLocalNum].bIsAdminFlag = TRUE;
            g_stBfdlocal[usLocalNum].bIsUpOnce = FALSE;
        }
        break;
        
        case BFD_DOWN:
        if (BFD_DOWN == g_stBfdlocal[usLocalNum].ucSessionState)/* 为down */
        {
            spm_bfd_state_fsm_init(usLocalNum, BFD_DOWN);
        }
        else if (BFD_UP == g_stBfdlocal[usLocalNum].ucSessionState)/* up */
        {
            g_stBfdlocal[usLocalNum].ucLocalDiag = bfd_diag_nbr_session_down;
            if (ENABLE == g_stBfdlocal[usLocalNum].bBasicBfdCB)
            {
                if (TRUE == g_stBfdlocal[usLocalNum].pstBasicBfdCB.peer_sid_selflearn_enable)
                {
                    g_stBfdlocal[usLocalNum].ulRemoteDiscr = 0;
                    g_stBfdlocal[usLocalNum].usBfdflag = BFD_NO_CREATE;
                }
            }
            spm_bfd_state_fsm_down(usLocalNum, BFD_DOWN);
        }
        break;

        case BFD_INIT:
        if ((BFD_DOWN == g_stBfdlocal[usLocalNum].ucSessionState) || 
        (BFD_INIT == g_stBfdlocal[usLocalNum].ucSessionState))
        {
            spm_bfd_state_fsm_up(usLocalNum, BFD_INIT);
        }
        break;

        case BFD_UP:
        if (BFD_INIT == g_stBfdlocal[usLocalNum].ucSessionState)
        {
            spm_bfd_state_fsm_up(usLocalNum, BFD_UP);
        } 
        break;

        default:
        break;

    }
    
    EXIT_LABEL : /*NBB_TRC_EXIT();*/
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_state_fsm_down
 功能描述  : 会话进入down状态处理
 输入参数  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月18日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm_down(IN NBB_USHORT usLocalNum,IN NBB_BYTE ucPacktState)
{
    NBB_BYTE ucLastState = 0;
    SPM_BFD_EVENT_INFO stBfdEventInfo;

    //NBB_TRC_ENTRY(("spm_bfd_state_fsm_down"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fsm down state, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stBfdEventInfo, 0, sizeof(SPM_BFD_EVENT_INFO));
    if (BFD_DOWN != g_stBfdlocal[usLocalNum].ucSessionState)/* 不为down */
    {
        /* add begin by limingming, 2013/4/11, Mantis号:HSDxxxxxx, 原因:ID同步 */
        if (BFD_UP == g_stBfdlocal[usLocalNum].ucSessionState) /* 原来是up状态，表明是从up变成down */
        {
            if (0 != g_stBfdlocal[usLocalNum].usTimerLiveUp)    /* 出现过从down到up又回到down的情况，之间没有上报up的情况 */
            {
                g_stBfdlocal[usLocalNum].usTimerLiveDown = 0;   /* 清空计数，这样就不会上报 */
            }            
            else
            {
                if(0 == g_stBfdlocal[usLocalNum].usTimerPortUPToDown)   /* 立即上报的情况，由于外部判断不会进不去，导致无法上报 */
                {
                    g_stBfdlocal[usLocalNum].usTimerLiveDown = 1;   /* 强制设置设置为1，在spm_bfd_fsm_task状态机跑完后-1立刻上报 */
                }
                else
                {
                    g_stBfdlocal[usLocalNum].usTimerLiveDown = g_stBfdlocal[usLocalNum].usTimerPortUPToDown * BFD_LIVEUPTIMES;
                }
            }
        }
        
        /* add end by limingming, 2013/4/11 */     
        ucLastState = g_stBfdlocal[usLocalNum].ucSessionState;
        g_stBfdlocal[usLocalNum].ucSessionState = BFD_DOWN;
        g_stBfdlocal[usLocalNum].ucDesiredMinTxInterval = BFD_1000MS;
        g_stBfdlocal[usLocalNum].ucRequiredMinRxInterval = BFD_1000MS;
        g_stBfdlocal[usLocalNum].usTimerLiveUp = 0;   /* 状态变成down后，up延时计时器失去做用 */

        spm_bfd_set_anti_attack(usLocalNum, 0);
        
        stBfdEventInfo.usLocalIndex = usLocalNum;
        stBfdEventInfo.ucLastSessionState = ucLastState;
        stBfdEventInfo.ucRxPacketState = ucPacktState;
        spm_bfd_event(&stBfdEventInfo, TRUE);
        
        spm_bfd_sendpacket_intervalreg(usLocalNum, TRUE);

        /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x0, 
            (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
        /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x1, 
            (g_stBfdlocal[usLocalNum].ulRemoteDiscr)&DRV_USHORT_INVALID);
    }
    else    /* 原来为down状态 */
    {
        if (0 != g_stBfdlocal[usLocalNum].ulRemoteDiscr)    /* 学习到了描述符 */
        {
            /* 自学习的BFD条目 */
            if (ENABLE == g_stBfdlocal[usLocalNum].bBasicBfdCB)
            {
                if (TRUE == g_stBfdlocal[usLocalNum].pstBasicBfdCB.peer_sid_selflearn_enable)
                {
                    
                    g_stBfdlocal[usLocalNum].usTimerStayDown++;
                    if(g_stBfdlocal[usLocalNum].usTimerStayDown > BFD_STAYDOWN) /* 定时器到阀值了 */
                    {
                        /* 此时需要清除FPGA中上次学习到的描述符 */
                        spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLocalNum + 0x0,0);
                        spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLocalNum + 0x1,0);

                        g_stBfdlocal[usLocalNum].ulRemoteDiscr = 0;
                        g_stBfdlocal[usLocalNum].usBfdflag = BFD_NO_CREATE;

                        /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x0, 
                            (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
                        /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x1, 
                            (g_stBfdlocal[usLocalNum].ulRemoteDiscr)&DRV_USHORT_INVALID);

                        g_stBfdlocal[usLocalNum].usTimerStayDown = 0;   /* 清空定时器 */

                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "MID %u forget learned YID %u\r\n", 
                            g_stBfdlocal[usLocalNum].ulDiscrimer, g_stBfdlocal[usLocalNum].ulRemoteDiscr);
                    }
                }            
            }
        }   
    }
    
    EXIT_LABEL : /*NBB_TRC_EXIT();*/
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_state_fsm_init
 功能描述  : 会话进入init状态处理
 输入参数  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月18日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm_init(IN NBB_USHORT usLocalNum,IN NBB_BYTE ucPacktState)
{
    NBB_USHORT   usData = 0;
    SPM_BFD_REG_PACKET2 stPacketInfo2;
    NBB_BYTE ucLastState = 0;
    SPM_BFD_EVENT_INFO stBfdEventInfo;
    
    //NBB_TRC_ENTRY(("spm_bfd_state_fsm_init"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fsm init state, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stPacketInfo2, 0, sizeof(SPM_BFD_REG_PACKET2));
    OS_MEMSET(&stBfdEventInfo, 0, sizeof(SPM_BFD_EVENT_INFO));
    ucLastState = g_stBfdlocal[usLocalNum].ucSessionState;
    g_stBfdlocal[usLocalNum].ucSessionState = BFD_INIT;
    g_stBfdlocal[usLocalNum].bIsAdminFlag = FALSE;
    g_stBfdlocal[usLocalNum].ucLocalDiag = bfd_diag_no_diag; 

    stBfdEventInfo.usLocalIndex = usLocalNum;
    stBfdEventInfo.ucLastSessionState = ucLastState;
    stBfdEventInfo.ucRxPacketState = ucPacktState;
    spm_bfd_event(&stBfdEventInfo, FALSE);
    
    stPacketInfo2.usDectMult = g_stBfdlocal[usLocalNum].ucDetectMult;
    stPacketInfo2.usDiag = g_stBfdlocal[usLocalNum].ucLocalDiag;
    stPacketInfo2.usSta  = g_stBfdlocal[usLocalNum].ucSessionState;
    usData = *(NBB_USHORT *)(&stPacketInfo2);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xd000 + 2 * usLocalNum + 0x0, usData);
     
    usLocalNum = 0x8000 + usLocalNum;
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x0002, usLocalNum);
    
    EXIT_LABEL : /*NBB_TRC_EXIT();*/
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_state_fsm_up
 功能描述  : 会话进入up状态处理
 输入参数  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月18日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm_up(IN NBB_USHORT usLocalNum,IN NBB_BYTE ucPacktState)
{
    NBB_BOOL bIsModify = FALSE;
    NBB_BYTE ucLastState = 0;
    SPM_BFD_EVENT_INFO stBfdEventInfo;
    
    //NBB_TRC_ENTRY(("spm_bfd_state_fsm_up"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fsm up state, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stBfdEventInfo, 0, sizeof(SPM_BFD_EVENT_INFO));
    g_stBfdlocal[usLocalNum].bIsChangeToUp = TRUE;
    if(0 != g_stBfdlocal[usLocalNum].usTimerPortDownToUp)
    {
        g_stBfdlocal[usLocalNum].usTimerLiveUp = g_stBfdlocal[usLocalNum].usTimerPortDownToUp * BFD_LIVEUPTIMES;
    }
    else
    {
        g_stBfdlocal[usLocalNum].usTimerLiveUp = BFD_LIVEUP; /* 稳定up5s */
    }
    g_stBfdlocal[usLocalNum].usTimerLiveDown = 0;   /* 状态变成up后，down延时计时器失去做用 */
    g_stBfdlocal[usLocalNum].bIsAdminFlag = FALSE;
    g_stBfdlocal[usLocalNum].bIsUpOnce = TRUE;
    ucLastState = g_stBfdlocal[usLocalNum].ucSessionState;
    g_stBfdlocal[usLocalNum].ucSessionState = BFD_UP;
    g_stBfdlocal[usLocalNum].ucLocalDiag = bfd_diag_no_diag;
    g_stBfdlocal[usLocalNum].ucDetectMult = g_stBfdlocal[usLocalNum].ucDM;

    /* 判断dmti */
    if (g_stBfdlocal[usLocalNum].ucDesiredMinTxInterval != g_stBfdlocal[usLocalNum].ucDMTI)
    {
        g_stBfdlocal[usLocalNum].ucDesiredMinTxInterval = g_stBfdlocal[usLocalNum].ucDMTI;
        bIsModify = TRUE;
    }

    /* 判断rmri */
    if (g_stBfdlocal[usLocalNum].ucRequiredMinRxInterval != g_stBfdlocal[usLocalNum].ucRMRI)
    {
        g_stBfdlocal[usLocalNum].ucRequiredMinRxInterval = g_stBfdlocal[usLocalNum].ucRMRI;
        bIsModify = TRUE;
    }

    spm_bfd_set_anti_attack(usLocalNum, 1);
    
    stBfdEventInfo.usLocalIndex = usLocalNum;
    stBfdEventInfo.ucLastSessionState = ucLastState;
    stBfdEventInfo.ucRxPacketState = ucPacktState;
    spm_bfd_event(&stBfdEventInfo, FALSE);
    
    spm_bfd_sendpacket_intervalreg(usLocalNum, bIsModify);
    
    EXIT_LABEL : /*NBB_TRC_EXIT();*/
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_state_fsm_admindown
 功能描述  : 会话进入admindown状态处理
 输入参数  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月18日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm_admindown(IN NBB_USHORT usLocalNum)
{
    //NBB_TRC_ENTRY(("spm_bfd_state_fsm_admindown"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fsm up state, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }
    g_stBfdlocal[usLocalNum].ucSessionState = BFD_ADMINDOWN;
    g_stBfdlocal[usLocalNum].ucLocalDiag =  bfd_diag_admin_down;
    g_stBfdlocal[usLocalNum].ucDesiredMinTxInterval = BFD_1000MS;
    
    spm_bfd_sendpacket_intervalreg(usLocalNum, TRUE);
    
    EXIT_LABEL : //NBB_TRC_EXIT();
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_packet_forward
 功能描述  : 通过linx转发报文至其他盘
 输入参数  : IN NBB_BYTE *pBuf        
             IN NBB_ULONG ulMsgLenth  
             IN NBB_USHORT usDesSlotid
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月8日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

  2.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 修改为MTL格式，摈弃linx
*****************************************************************************/
NBB_VOID spm_bfd_packet_forward(IN NBB_BYTE *pBuf, IN NBB_ULONG ulMsgLenth, IN NBB_USHORT usDesSlotid)
{
    NBB_CHAR *pcLinxData = NULL;
    NBB_ULONG ulSendLenth = 0;
    NBB_INT    iRet = ERROR;
    NBB_CHAR *pcLinxTmp = NULL;
    MTL_NODE mtl_head = {0};

    g_stDesNode.module_id = TYPE_MTL_TNE;
    g_stDesNode.slot = spm_hardw_getemustate(); /* 发往主用主控，由主用主控转发 */
    g_stDesNode.cpu = 0;                /* 发送到主控的CPU,再由主控转到交叉的CPU */
    
    ulSendLenth = ulMsgLenth + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD);    /* 在原报文基础上加上头 */
    pcLinxData = malloc(ulSendLenth);
    if (NULL == pcLinxData)
    {
        return;
    }

    pcLinxTmp = pcLinxData;
    
    /* 组发往主控CPU的头 */
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->des_head.module_id = TYPE_MTL_BFD;
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->des_head.frame = g_stDesNode.frame;
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->des_head.slot = usDesSlotid;
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->des_head.cpu = 0;
    
    /* 组同步内容的头 */
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->syn_head.ucType = BFD_FWD_MSG_PCK;
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->syn_head.ulSrcSlot = g_stSrcNode.slot;

    /* 包的内容 */
    pcLinxTmp = pcLinxTmp + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD);
    memcpy(pcLinxTmp, pBuf, ulMsgLenth);
    
    /* 2015.5.21采用mtl方式发送后，本槽位直接发往交换CPU,不需要先发往主控再通过主控发往交换CPU, 
    由于修改建立在之前linx代码基础上，此处发送消息是去掉MTL_NODE,对于mtl来说它为多余*/
    iRet = mtl_send(mtl_identity_to_fd(1 << 24 | usDesSlotid << 16 | 0), TYPE_MTL_BFD, TYPE_MTL_BFD,
        pcLinxData + sizeof(MTL_NODE), ulSendLenth - sizeof(MTL_NODE));

    /* free memory */
    free(pcLinxData);
    pcLinxData = NULL;

    /* 发送失败 */
    if(iRet < 0)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s:mtl_send error = %d! \r\n",__FUNCTION__, iRet);
    }
    return;

}

/*****************************************************************************
 函 数 名  : BFD_Packet_Receive
 功能描述  : BFD接收从C3来的YOURID为0的报文
 输入参数  : IN void *pBuf  
             IN NBB_ULONG ulMsgLenth
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月6日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数
 备注          : C3解析BFD报文，将YOURID为0的报文直接上送本盘CPU，
                 本盘CPU通过规则匹配，查询全局BFD配置表项   
                 匹配规则是Vrfid+dip+sip,C3上送的BFD报文中有port信息，
                 根据vlan+port查询接口的Vrfid,
                 BFD for pw描述符已经协商, BFD FOR LSP active一端由于握手报文的描述符均为127/8
                 网段，无法学习描述符；BFD FOR INTERFACE，与端口匹配，不通过查询DIP，且不存在跨盘现象

  2.日    期   : 2015年6月18日
    作    者   : gaos
    修改内容   : 添加调试信息
*****************************************************************************/
NBB_INT spm_bfd_packet_receive(IN NBB_BYTE *pBuf, IN NBB_ULONG ulMsgLenth)
{
    NBB_USHORT     usLoopi = 0;         /* 循环变量 */
    NBB_USHORT     usLoopj = 0;         /* 循环变量 */
    NBB_USHORT     usPortId = 0;
    NBB_USHORT     usVrfid = 0;
    NBB_LONG       lLenthTmp = 0;
    NBB_BYTE   ucSessionState = 0;     /* 握手报文的状态 */
    NBB_INT    iRet = ERROR;
    SPM_BFD_IP_GLOBAL       *pstBfdIpCfg = NULL;
    SPM_BFD_IPV6_GLOBAL       *pstBfdIpv6Cfg = NULL;
    SPM_FWD_GLOBAL          *pstFwdInfo  = NULL;
    SPM_BFDGLB_IP_KEY       stBfdGlbKey;
    SPM_BFDGLB_IPV6_KEY      stBfdGlbipv6Key;
    NBB_BYTE    ucLocalSlot   = 0;          /* 本盘槽位号 */
    minm_header_t *pstMacInMac = NULL;
    SPM_BFD_PACKET_ABSTRACT stPacketInfo;
    SPM_XCU_STATE stXcuState = {0};        /* 主控的主备用槽位 */
    NBB_LONG ulCounter = 0;/* 调试用 */
    
    /* 非nbase线程使用nbase内不使用NBB_CXT上下文的API时，骗过编译器 */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();

    if (NULL == pBuf)/* 入参判断 */
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPacketInfo, 0, sizeof(SPM_BFD_PACKET_ABSTRACT));
    
    lLenthTmp = ulMsgLenth - 28; /* 偏移掉前面的mac in mac */
    BFD_PACKETLEN_JUDGE(lLenthTmp);

    /* 获取本盘槽位 */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    OS_MEMSET(&stBfdGlbKey, 0, sizeof(SPM_BFDGLB_IP_KEY)); 
    OS_MEMSET(&stBfdGlbipv6Key, 0, sizeof(SPM_BFDGLB_IPV6_KEY)); 
    
    /* 解析DIP/SIP，mac in mac中获取VPNID，获取本盘槽位号，获取报文中的MYID */
    pstMacInMac = (minm_header_t *)pBuf;

    /* 判断端口类型 */
    switch (pstMacInMac->port_type)
    {
        case ETH_PORT_TYPE:
        case POS_PORT_TYPE:
        case LAG_PORT_TYPE:
        {
            usVrfid = ((pstMacInMac->port_instance.eth_port_bits.reserved[0] >> 16) & DRV_USHORT_INVALID);
            usPortId = pstMacInMac->port_instance.eth_port_bits.ports;
            break;
        }

        case L3VE_PORT_TYPE:
        {
            usVrfid = ((pstMacInMac->port_instance.ve_port_bits.reserved[0] >> 16) & DRV_USHORT_INVALID);
            usPortId = pstMacInMac->port_instance.ve_port_bits.port_id;
            break;
        }

        case VRF_TUNNEL_PORT_TYPE:
        {
            usVrfid = pstMacInMac->port_instance.vrf_tunnel_port_bits.vrf_id;
            break;
        }

        case IPV6_IPV4_TUNNEL_PORT_TYPE:
        {
            usVrfid = pstMacInMac->port_instance.ipv6_ipv4_tunnel_port_bits.vrf_id;
            break;
        }

        case GRE_TUNNEL_PORT_TYPE:
        {
            usVrfid = pstMacInMac->port_instance.gre_tunnel_port_bits.vrf_id;
            break;
        }

        case  MPLS_TE_TUNNEL_PORT_TYPE:
        {
            usVrfid = pstMacInMac->port_instance.mpls_te_tunnel_port_bits.vrf_id;
            break;
        }
        default:
            if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
            {
               printf("%s:pstMacInMac->port_type = %d error!\n",__FUNCTION__,pstMacInMac->port_type); 
               BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                "%s:pstMacInMac->port_type = %d error!\n",__FUNCTION__,pstMacInMac->port_type);
            }
            break;
    }
    
    iRet = spm_bfd_packet_analysis((pBuf + 28), lLenthTmp, &stPacketInfo);
    if (SUCCESS != iRet)
    {
        iRet = SUCCESS;
        goto EXIT_LABEL;
    }
    ucSessionState = stPacketInfo.ucState;

    if (BFD_IPV4 == stPacketInfo.ucIpType)
    {
        if (0x7F000000 == (stPacketInfo.ulDip[0] & 0x7F000000))/* 127网段 */    /* 127的是bfd for lsp的，FPGA学习 */
        {
            iRet = SUCCESS;
            goto EXIT_LABEL;
        }
        else if (0xE0000000 == (stPacketInfo.ulDip[0] & 0xE0000000))/* 224网段 *//* bfd for interface的华为用组播地址 */
        {
            /* 只用查询本处理盘的本地配置中发送方式为port的类型 */
            for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
            {
                /* 若该条目无会话则跳过 */
                if (DRV_ULONG_INVALID == g_stBfdlocal[usLoopi].ulDiscrimer)
                {
                    continue;
                }
                
                if (BFD_FWD_PORT == g_stBfdlocal[usLoopi].ucBfdFwdType)/* port */
                {
                    if (usPortId == g_stBfdlocal[usLoopi].usPortID)/* 判断portid */
                    {
                        g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;
                        
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                            "The %u bfd(Segment 224) learn discr  from c3, my_id %ld, yourid %ld, local ip %#x, remote ip %#x, vrf %d\r\n", 
                            usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, stPacketInfo.ulMyDrim, 
                            stPacketInfo.ulDip[0], stPacketInfo.ulSip[0], usVrfid);

                        /* 将学习到的描述符写入寄存器 */
                        /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                            (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                        /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                            (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                        g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;

                        /* 跑状态机 */
                        spm_bfd_state_fsm(usLoopi, ucSessionState);
                        break;
                    }
                }
            }
            if(BFD_MAX_SESNUM == usLoopi)
            {
                if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
                {
                   printf("%s:usPortId = %d not include in g_stBfdlocal!\n",__FUNCTION__,usPortId); 
                   BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                    "%s:usPortId = %d not include in g_stBfdlocal!\n",__FUNCTION__,usPortId);
                }                 
            }
        }
        else/* for ip */
        {
            /* 根据VRFID+DIP+SIP查询全局会话表项 */
            stBfdGlbKey.ulSip = stPacketInfo.ulDip[0];
            stBfdGlbKey.ulDip = stPacketInfo.ulSip[0];
            stBfdGlbKey.usVrfid = usVrfid;
            pstBfdIpCfg = (SPM_BFD_IP_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIp, &stBfdGlbKey);
            if (NULL != pstBfdIpCfg)/* 不为空 */
            {
                if(pstBfdIpCfg->usSlotid == (NBB_USHORT)ucLocalSlot)/* 发现该会话为本盘处理会话 */
                {
                    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* 循环 */
                    {
                        /* 若该条目无会话则跳过 */
                        if (DRV_ULONG_INVALID == g_stBfdlocal[usLoopi].ulDiscrimer)
                        {
                            continue;
                        }
                        if (pstBfdIpCfg->ulDiscrimer == g_stBfdlocal[usLoopi].ulDiscrimer)/* 描述符匹配 */
                        {
                            if ((BFD_DOWN == g_stBfdlocal[usLoopi].ucSessionState) 
                                    || (BFD_INIT == g_stBfdlocal[usLoopi].ucSessionState))
                            {
                                if ((0 == g_stBfdlocal[usLoopi].ulRemoteDiscr) 
                                    || (stPacketInfo.ulMyDrim != g_stBfdlocal[usLoopi].ulRemoteDiscr))
                                {
                                    /* 此时需要清除FPGA中上次学习到的描述符 */
                                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x0,0);
                                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x1,0);
                                    
                                    g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;

                                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                        "The %u bfd(ip) learn discr from c3, my_id %ld, yourid %ld, local ip %#x, remote ip %#x, vrf %d\r\n", 
                                        usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, stPacketInfo.ulMyDrim, 
                                        stPacketInfo.ulDip[0], stPacketInfo.ulSip[0], usVrfid);
                                    
                                    /* 将学习到的描述符写入寄存器 */
                                    /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
                                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                                        (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                                    /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
                                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                                        (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                                    g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
                                    spm_bfd_state_fsm(usLoopi, ucSessionState);
                                    break;
                                }
                            }
                        }
                    }
                }
                else if (1 == spm_hardw_getboardtype(pstBfdIpCfg->usSlotid))/* 即不是本盘，又不是主控盘处理会话 */
                {
                    if (g_usFwdNum >= BFD_MAX_FWDNUM)/* 大于最多数目 */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd is full, Dri = %ld\r\n", pstBfdIpCfg->ulDiscrimer);
                        goto EXIT_LABEL;
                    }

                    #if 0
                    /* 建立转发表项 */
                    /* 看转发表项是否存在 */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &pstBfdIpCfg->ulDiscrimer);
                    if (NULL != pstFwdInfo)/* 说明该转发表项已经存在 */
                    {
                        /* 将报文转发至主处理盘 */
                        spm_bfd_packet_forward(pBuf, ulMsgLenth, pstFwdInfo->usSlotid);

                        /* 纪录转发信息 */
                        spm_bfd_fwd_txrecord(pstFwdInfo->usSlotid, pstFwdInfo->ulDiscrimer);
                        iRet = SUCCESS;
                        goto EXIT_LABEL;
                    }

                    /* 若不存在，则记录转发信息，增加转发表项 */
                    /* modify begin by 蒋博,jiangbo, 20131113, Mantis号:无, 原因:因为不由nbase管理进程也会分配、释放这块
                       内存，不能使用NBB_MM_ALLOC管理，同时不能被verify */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)OS_ALLOC(sizeof(SPM_FWD_GLOBAL), MEM_SPM_BFD_FWD_GLOBAL_CB);
                    /* modify end by 蒋博,jiangbo, 20131113 */
                    if (NULL == pstFwdInfo)/* 为空 */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Memory is not enough\r\n");
                        goto EXIT_LABEL;
                    }
                    pstFwdInfo->ulDiscrimer = pstBfdIpCfg->ulDiscrimer;
                    pstFwdInfo->usSlotid = pstBfdIpCfg->usSlotid;
                    AVLL_INIT_NODE(pstFwdInfo->spm_fwd_node);
                    AVLL_INSERT(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                    
                    iRet = spm_bfd_addfwdtable(pstBfdIpCfg->ulDiscrimer, pstBfdIpCfg->usSlotid);
                    if (SUCCESS != iRet)/* 不成功 */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "add fwd table failed, discrim = %ld, Dslotid = %d\r\n", 
                            pstBfdIpCfg->ulDiscrimer, pstBfdIpCfg->usSlotid);
                        AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                        OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by 蒋博,jiangbo, 20131113, Mantis号:
                                                                           无, 原因:因为不由nbase管理进程也会分配、释放
                                                                           这块内存，不能使用NBB_MM_FREE管理，同时不能
                                                                           被verify */
                        goto EXIT_LABEL;
                    }
                    
                    g_usFwdNum++;
                    #endif

                    /* 将报文通过盘间通信转发到会话主处理盘 */
                    spm_bfd_packet_forward(pBuf, ulMsgLenth, pstBfdIpCfg->usSlotid);
                    
                    /* 纪录转发信息 */
                    spm_bfd_fwd_txrecord(pstBfdIpCfg->usSlotid, pstBfdIpCfg->ulDiscrimer);
                    if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
                    {
                       printf("%s,%d:spm_bfd_packet_forward call!\n",__FUNCTION__,__LINE__); 
                       BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                        "%s,%d:spm_bfd_packet_forward call!\n",__FUNCTION__,__LINE__);
                    }                     
                }
                else/* 会话为主控盘处理 */
                {
                    #ifdef SPU
                    /* 转发表项已经在下会话的时候建立，因此只需将报文双发至主控盘 */
                    
                    iRet = spm_hardw_getxcustate(&stXcuState);
                    if(SUCCESS != iRet)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "getxcustate error %d\r\n", iRet);
                        iRet = ERROR;
                        goto EXIT_LABEL;
                    }
                    /* 往主用主控槽发 */
                    if(0xff != stXcuState.ucMainXcuSlot)
                    {
                        spm_bfd_packet_forward(pBuf, ulMsgLenth, stXcuState.ucMainXcuSlot);
                    }
                    /* 往备用用主控槽发 */
                    if(0xff != stXcuState.ucBakXcuSlot)
                    {
                        spm_bfd_packet_forward(pBuf, ulMsgLenth, stXcuState.ucBakXcuSlot);
                    }
                    
                    /* 纪录转发信息 */
                    spm_bfd_fwd_txrecord(stXcuState.ucMainXcuSlot, pstBfdIpCfg->ulDiscrimer);
                    #endif
                }
            }
            else
            {
                if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
                {
                   printf("%s:pstBfdIpCfg is NULL sip= 0x%8x,dip = 0x%8x,vrfid = %d \n",
                    __FUNCTION__,stBfdGlbKey.ulSip,stBfdGlbKey.ulDip,stBfdGlbKey.usVrfid); 
                   BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                    "%s:pstBfdIpCfg is NULL sip= 0x%8x,dip = 0x%8x,vrfid = %d \n",
                    __FUNCTION__,stBfdGlbKey.ulSip,stBfdGlbKey.ulDip,stBfdGlbKey.usVrfid);
                }                                
            }
        }
    }

    if (BFD_IPV6 == stPacketInfo.ucIpType)
    {
        stBfdGlbipv6Key.ulDip[0] = stPacketInfo.ulSip[0];
        stBfdGlbipv6Key.ulDip[1] = stPacketInfo.ulSip[1];
        stBfdGlbipv6Key.ulDip[2] = stPacketInfo.ulSip[2];
        stBfdGlbipv6Key.ulDip[3] = stPacketInfo.ulSip[3];
        stBfdGlbipv6Key.ulSip[0] = stPacketInfo.ulDip[0];
        stBfdGlbipv6Key.ulSip[1] = stPacketInfo.ulDip[1];
        stBfdGlbipv6Key.ulSip[2] = stPacketInfo.ulDip[2];
        stBfdGlbipv6Key.ulSip[3] = stPacketInfo.ulDip[3];
        stBfdGlbipv6Key.usVrfid = usVrfid;
        pstBfdIpv6Cfg = (SPM_BFD_IPV6_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIpv6, &stBfdGlbipv6Key);
        if (NULL != pstBfdIpv6Cfg)/* 不为空 */
        {
            if(pstBfdIpv6Cfg->usSlotid == (NBB_USHORT)ucLocalSlot)/* 发现该会话为本盘处理会话 */
            {
                for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* 循环 */
                {
                    /* 若该条目无会话则跳过 */
                    if (DRV_ULONG_INVALID == g_stBfdlocal[usLoopi].ulDiscrimer)
                    {
                        continue;
                    }
                    if (pstBfdIpv6Cfg->ulDiscrimer == g_stBfdlocal[usLoopi].ulDiscrimer)/* 描述符匹配 */
                    {
                        if ((0 == g_stBfdlocal[usLoopi].ulRemoteDiscr) 
                            || (stPacketInfo.ulMyDrim != g_stBfdlocal[usLoopi].ulRemoteDiscr))
                        {
                            g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;

                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                "The %u bfd learn discr from c3, my_id %ld, yourid %ld, local ip %lx.%lx.%lx.%lx, "
                                "remote ip %lx.%lx.%lx.%lx, vrf %d\r\n", 
                                usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, stPacketInfo.ulMyDrim, 
                                stPacketInfo.ulDip[0],
                                stPacketInfo.ulDip[1],
                                stPacketInfo.ulDip[2],
                                stPacketInfo.ulDip[3],
                                stPacketInfo.ulSip[0],
                                stPacketInfo.ulSip[1],
                                stPacketInfo.ulSip[2],
                                stPacketInfo.ulSip[3],
                                usVrfid);
                            
                            /* 将学习到的描述符写入寄存器 */
                            /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
                            spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                                (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                            /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
                            spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                                (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                            g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
                            spm_bfd_state_fsm(usLoopi, ucSessionState);
                            break;
                        }
                    }
                }
            }
            else if (1 == spm_hardw_getboardtype(pstBfdIpv6Cfg->usSlotid))/* 不为主控盘处理会话 */
            {
                if (g_usFwdNum >= BFD_MAX_FWDNUM)/* 大于最多数目 */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd is full, Dri = %ld\r\n", pstBfdIpv6Cfg->ulDiscrimer);
                    goto EXIT_LABEL;
                }
                
                /* 建立转发表项 */
                /* 看转发表项是否存在 */
                pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &pstBfdIpv6Cfg->ulDiscrimer);
                if (NULL != pstFwdInfo)/* 说明该转发表项已经存在 */
                {
                    /* 将报文转发至主处理盘 */
                    spm_bfd_packet_forward(pBuf, ulMsgLenth, pstFwdInfo->usSlotid);

                    /* 纪录转发信息 */
                    spm_bfd_fwd_txrecord(pstFwdInfo->usSlotid, pstFwdInfo->ulDiscrimer);
                    iRet = SUCCESS;
                    goto EXIT_LABEL;
                }

                /* 若不存在，则记录转发信息，增加转发表项 */                
                /* modify begin by 蒋博,jiangbo, 20131113, Mantis号:无, 原因:因为不由nbase管理进程也会分配、释放这块内存
                   ，不能使用NBB_MM_ALLOC管理，同时不能被verify */
                pstFwdInfo = (SPM_FWD_GLOBAL *)OS_ALLOC(sizeof(SPM_FWD_GLOBAL), MEM_SPM_BFD_FWD_GLOBAL_CB);
                /* modify end by 蒋博,jiangbo, 20131113 */
                if (NULL == pstFwdInfo)/* 为空 */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Memory is not enough\r\n");
                    goto EXIT_LABEL;
                }
                pstFwdInfo->ulDiscrimer = pstBfdIpv6Cfg->ulDiscrimer;
                pstFwdInfo->usSlotid = pstBfdIpv6Cfg->usSlotid;
                AVLL_INIT_NODE(pstFwdInfo->spm_fwd_node);
                if (!AVLL_INSERT(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node))
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "intert avll tree error\r\n");
                    OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));
                    pstFwdInfo = NULL;
                    goto EXIT_LABEL;
                }
                
                iRet = spm_bfd_addfwdtable(pstBfdIpv6Cfg->ulDiscrimer, pstBfdIpv6Cfg->usSlotid);
                if (SUCCESS != iRet)/* 不成功 */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "add fwd table failed, discrim = %ld, Dslotid = %d\r\n", 
                        pstBfdIpv6Cfg->ulDiscrimer, pstBfdIpv6Cfg->usSlotid);
                    AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                    OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by 蒋博,jiangbo, 20131113, Mantis号:
                                                                       无, 原因:因为不由nbase管理进程也会分配、释放
                                                                       这块内存，不能使用NBB_MM_FREE管理，同时不能
                                                                       被verify */
                    pstFwdInfo = NULL;                                                   
                    goto EXIT_LABEL;
                }
                
                g_usFwdNum++;

                /* 将报文通过盘间通信转发到会话主处理盘 */
                spm_bfd_packet_forward(pBuf, ulMsgLenth, pstFwdInfo->usSlotid);
                
                /* 纪录转发信息 */
                spm_bfd_fwd_txrecord(pstFwdInfo->usSlotid, pstFwdInfo->ulDiscrimer);
            }
            else/* 会话为主控盘处理 */
            {
                #ifdef SPU
                /* 转发表项已经在下会话的时候建立，因此只需将报文双发至主控盘 */
                
                iRet = spm_hardw_getxcustate(&stXcuState);
                if(SUCCESS != iRet)
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "getxcustate error %d\r\n", iRet);
                    iRet = ERROR;
                    goto EXIT_LABEL;
                }
                /* 往主用主控槽发 */
                if(0xff != stXcuState.ucMainXcuSlot)
                {
                    spm_bfd_packet_forward(pBuf, ulMsgLenth, stXcuState.ucMainXcuSlot);
                }
                /* 往备用用主控槽发 */
                if(0xff != stXcuState.ucBakXcuSlot)
                {
                    spm_bfd_packet_forward(pBuf, ulMsgLenth, stXcuState.ucBakXcuSlot);
                }
                
                /* 纪录转发信息 */
                spm_bfd_fwd_txrecord(stXcuState.ucMainXcuSlot, pstBfdIpv6Cfg->ulDiscrimer);
                #endif
            }
        }
        
    }

    /* 打印buffer，调试用 */
    if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
    {
        printf("pBuf:\n");
        for(ulCounter = 0; ulCounter < ulMsgLenth ; ulCounter++)
        {
            if( ulCounter % 16 == 0 )
            {
                printf("0x%02x:\t",ulCounter);
            }
            printf("%02x  ",*(pBuf + ulCounter));

            if(!((ulCounter + 1) % 16))
            {
                printf("\n");
            }
        }        
        printf("\n");
    } 
    
    iRet = SUCCESS;
    EXIT_LABEL :
    
        return iRet;
}

/*****************************************************************************
 函 数 名  : bfd_c3_to_cp
 功能描述  : C3将yourid=0的报文发给CPU
 输入参数  : int unit              
             c3_rxtx_pkt_t *rxpkt  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年7月2日
    作    者   : gaos
    修改内容   : 添加函数头而已

*****************************************************************************/
c3_rx_ret_t bfd_c3_to_cp(int unit, c3_rxtx_pkt_t *rxpkt)
{
    int ret = 0;
    NBB_UINT uiMsgLenth = 0;
    NBB_UINT mhead_len = 0;
    NBB_CHAR acBuf[BFD_MSG_BUF_LENTH];

    /* 参数检查 */
    if((0 != unit ) && (1 != unit ))
    {
        printf("RXPKT is not to cp, return!\n");
        return C3_RX_HANDLED_OWNED;
    }
    
    /* 参数检查 */
    if(NULL == rxpkt)
    {
        printf("Rxpkt is NULL!\n");
        return C3_RX_HANDLED_OWNED;
    }

    mhead_len = sizeof(minm_header_t);
    
    /* 参数检查 */
    if((NULL == rxpkt->head) || (NULL == rxpkt->payload) || (rxpkt->payload != ((uint8 *)rxpkt->head + mhead_len)))
    {
        printf("The rxpkt.head and rxpkt.payload is not Connected!\n");
        return C3_RX_HANDLED_OWNED;
    }

    uiMsgLenth = rxpkt->len + mhead_len;

    /* 将报文入队列 */
    if (uiMsgLenth > BFD_MSG_BUF_LENTH)
    {
        memcpy(acBuf, rxpkt->head, BFD_MSG_BUF_LENTH);
        uiMsgLenth = BFD_MSG_BUF_LENTH;
    }
    else
    {
        memcpy(acBuf, rxpkt->head, uiMsgLenth);
    }
    
    ret = BmuQueueSend(g_stBfdMsgQID, acBuf, uiMsgLenth, BMU_NO_WAIT, BMU_MSG_PRI_NORMAL);
    if (0 != ret)
    {
        printf("BmuQueueSend err %d!\n", ret);
    }
    return C3_RX_HANDLED;

}

/*****************************************************************************
 函 数 名  : spm_bfd_sendpacket_ipv4fpga
 功能描述  : IPV4 BFD报文封装，从IPV4字段到UDP字段
 输入参数  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月17日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_ipv4fpga(IN NBB_USHORT usLocalNum)
{
    NBB_USHORT usData1 = 0;
    NBB_USHORT usData2 = 0;

    usData1 = 0x45;
    usData1 = (usData1 << 8);
    usData2 = 0x7; /* 优先级 */
    usData2 = (usData2 << 5) + g_stBfdlocal[usLocalNum].ucTos;
    usData1 = usData1 + usData2;
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x4000 + usLocalNum, usData1);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x5000 + usLocalNum, g_stBfdlocal[usLocalNum].usTotallength);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x6000 + usLocalNum, 0);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x7000 + usLocalNum, 0);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x16000 + usLocalNum, 0xff11);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x17000 + usLocalNum, 0);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x18000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[3] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x19000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[3])&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1a000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[3] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1b000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[3])&DRV_USHORT_INVALID);
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_sendpacket_ipv6fpga
 功能描述  : IPV6 BFD报文封装，从IPV6字段到UDP字段
 输入参数  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月17日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_ipv6fpga(IN NBB_USHORT usLocalNum)
{
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x4000 + usLocalNum, 0x6000);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x5000 + usLocalNum, 0);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x6000 + usLocalNum, 32);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x7000 + usLocalNum, 0x11ff);

    /*电信测试 2015.11.09 填写VLAN，置PRI = 7*/
    if((BFD_FWD_PORT == g_stBfdlocal[usLocalNum].ucBfdFwdType) && (g_stBfdlocal[usLocalNum].usSVlanID != 0))
    {
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x14000 + usLocalNum, 
            (g_stBfdlocal[usLocalNum].usSVlanID & 0xFFF) | 0xE000);
    }
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x16000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[0] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x17000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[0])&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x18000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[1] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x19000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[1])&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1a000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[2] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1b000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[2])&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1c000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[3] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1d000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulSip[3])&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1e000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[0] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x1f000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[0])&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x20000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[1] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x21000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[1])&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x22000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[2] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x23000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[2])&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x24000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[3] >> 16)&DRV_USHORT_INVALID);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x25000 + usLocalNum, 
        (g_stBfdlocal[usLocalNum].ulDip[3])&DRV_USHORT_INVALID);
    return;
}

#ifdef SPU

/*****************************************************************************
 函 数 名  : spm_bfd_init_bus_map_table
 功能描述  : 初始化 SRC FPGA 0x930000 ~ 0x932fff 为0xffff，此函数在业务盘上为桩函数。
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月15日
    作    者   : tianf
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_bfd_state_map_init(NBB_CXT_T NBB_CXT)
{

    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_tx_rx_counter_init
 功能描述  : 初始化BFD 报文计数读清零使能
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年12月1日
    作    者   : tianf
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_tx_rx_counter_init(NBB_CXT_T NBB_CXT)
{
    
    NBB_USHORT usDate = TRUE;

    NBB_TRC_ENTRY(__FUNCTION__);

    spm_oamtree_s16write(BFDSESSIONBASE, 0x3, usDate); /*收计数默认读清零使能*/
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x3, usDate); /*发计数默认读清零使能*/
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return SUCCESS;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_init_fpga
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月14日
 * 函数功能  : 初始化BFD 相关的FPGA寄存器
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID spm_bfd_init_fpga(NBB_VOID)
{
    /* 初始化BFD告警同步映射表 */
    spm_proc_bfd_state_map_init();

    /*收发计数默认读清零使能*/
    spm_bfd_tx_rx_counter_init();
    return;
}


/*****************************************************************************
 函 数 名  : spm_bfd_init_fwdtable
 功能描述  : 转发表项初始化，此函数在主控盘上为桩函数，业务盘才有转发表项，支持2047条
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月31日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_init_fwdtable(NBB_CXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0; /* 循环变量 */
    NBB_INT iRet = ERROR;

    NBB_TRC_ENTRY(("spm_bfd_init_fwdtable"));

    /* 转发软件表项初始化 */
    for (usLoopi = 0; usLoopi < BFD_MAX_FWDNUM; usLoopi++)
    {
        BFD_FWD_INIT(usLoopi);
    }

    /* BFD转发表二叉树初始化，每业务盘支持2k转发表 */    
    g_usFwdTreeLevel  = 11;
    g_usFwdTreeNodeMaxnum = (1 << g_usFwdTreeLevel);
    g_usFwdLspNoMax   = g_usFwdTreeNodeMaxnum - 1;
    g_usFwdNum = 0;
    g_pulFwdTreebase = (NBB_ULONG *)NBB_MM_ALLOC(sizeof(NBB_ULONG)*g_usFwdTreeLevel,
        NBB_NORETRY_ACT, MEM_SPM_BFD_PUL_FWD_TREE_BASE_CB);
    if (NULL == g_pulFwdTreebase)/* 内存申请失败 */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Malloc error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Malloc error\r\n");
        goto EXIT_LABEL;
    }
    
    NBB_MEMSET(g_pulFwdTreebase, 0 ,sizeof(NBB_ULONG)*g_usFwdTreeLevel);
    iRet = spm_oamtree_init(g_usFwdTreeNodeMaxnum, spm_fwd_tree_get_fpga_data, &g_stTreeFwdNodeSet);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Fwd node set error, nodenum = %d", g_usFwdTreeNodeMaxnum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd node set error, nodenum = %d\r\n", g_usFwdTreeNodeMaxnum);
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_syn_sessionid
 功能描述  : 业务盘定时检测本盘的ID是否发生了变化，若发生了变化则同步到主控盘
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月4日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数
 备注          :
 每个业务盘按照1k的数量同步ID，按照条目号的顺序，所以每个同步信息的内容为
 报文类型(1个字节)+g_ulSequenceID(序列号)+描述符(2bytes * 1023)

  2.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 根据mtl要求修改
*****************************************************************************/
NBB_INT  spm_bfd_syn_sessionid(NBB_VOID)
{
    NBB_USHORT usLoopi = 0;         /* 循环变量 */
    NBB_BYTE *pucTmpInfo       = NULL;
    SPM_BFD_SYN_BFD_ID *pusTmp  = NULL;
    NBB_ULONG *pulSeqID  = NULL;
    NBB_INT    iRet = ERROR;
    NBB_CHAR *pcLinxData = NULL;
    MTL_NODE mtl_head = {0};
    SPM_XCU_STATE stXcuState = {0};
    NBB_BYTE ucMustateSlot[2] = {0};
    NBB_INT iLoop = 0;

    iRet = spm_hardw_getxcustate(&stXcuState);
    if ( SUCCESS != iRet ) /* 当SUCCESS != iRet成立时执行 */
    {
        goto EXIT_LABEL;
    }
    ucMustateSlot[0] = stXcuState.ucMainXcuSlot;    /*主用主控的槽位号，不在位是0xff */
    ucMustateSlot[1] = stXcuState.ucBakXcuSlot;     /* 备用主控的槽位号,不在位是0xff */

    g_stDesNode.module_id = TYPE_MTL_TNE;
    g_stDesNode.slot = stXcuState.ucMainXcuSlot; /*主用主控的槽位号，不在位是0xff */
    g_stDesNode.cpu = 1;                        /* 发送到主控的CPU,再由主控转到交叉的CPU */
    g_ucMainEmuId = g_stDesNode.slot;
    
    if (0xff == g_stDesNode.slot)
    {
        goto EXIT_LABEL;
    }
        
    /* 具备同步ID的条件 */
    if ((TRUE == g_bIsModifySession) && (FALSE == g_bIsReadySendtoMaster))
    {        
        pucTmpInfo = g_pucSynIdInfo;

        /* 组发往主控CPU的头 */
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.module_id = TYPE_MTL_BFD;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.frame = g_stDesNode.frame;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.slot = g_stDesNode.slot;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.cpu = 0;
        
        /* 组同步内容的头 */
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->syn_head.ucType = BFD_FWD_MSG_ID;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->syn_head.ulSrcSlot = g_stSrcNode.slot;
        
        /* 组包的内容 */
        pusTmp = (SPM_BFD_SYN_BFD_ID*)(pucTmpInfo + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD));
        pusTmp->ulSequenceID= g_ulSequenceID;    /* 封装序列号 */
        
        /* 组帧 */
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            pusTmp->arrucBfdId[usLoopi] = (NBB_USHORT)g_stBfdlocal[usLoopi].ulDiscrimer;/* ulDiscrimer即BFD ID，2个字节 */
        }

        /* 发往主用主控 */
        for (iLoop = 0; iLoop < 2; iLoop++) /* 发往两个交叉盘 */
        {
            if (0xff == ucMustateSlot[iLoop])   /* 如果槽不在位，不发送 */
            {
                continue;
            }
          
            ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.slot = ucMustateSlot[iLoop]; /* 修改需要发往的槽位号 */
            pcLinxData = malloc(g_ulSynIdInfoLth);
            if (NULL == pcLinxData)
            {
                /* 内存不够就不再重发了，防止200ms刷一次内存分配 */
                BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                    "alloc linx sig error, mod id %lu, frame %lu, des slot %lu, cpu %lu, G_SeqId %lu\r\n",
                    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.module_id,
                    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.frame,
                    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.slot,
                    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.cpu, g_ulSequenceID);
                goto EXIT_LABEL;
            }

            memcpy(pcLinxData, g_pucSynIdInfo, g_ulSynIdInfoLth);
            
            /* 2015.5.21采用mtl方式发送后，本槽位直接发往交换CPU,不需要先发往主控再通过主控发往交换CPU, 
                由于修改建立在之前linx代码基础上，此处发送消息是去掉MTL_NODE,对于mtl来说它为多余*/
            iRet = mtl_send(mtl_identity_to_fd(1 << 24 | ucMustateSlot[iLoop] << 16 | 0), TYPE_MTL_BFD, TYPE_MTL_BFD, 
                pcLinxData + sizeof(MTL_NODE), g_ulSynIdInfoLth - sizeof(MTL_NODE));
            free(pcLinxData);
            pcLinxData = NULL;
            
            if (iRet < 0)
            {
                if(ucMustateSlot[iLoop] == stXcuState.ucMainXcuSlot)    /* 主用主控才需要重发，同时只有主控才会回确认包 */
                {
                    g_bIsModifySession = TRUE;  /* 失败后重新发送 */
                    BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR, "%s:mtl send error = %d,  G_SeqId %lu,XcuSlot:%d\r\n",
                    __FUNCTION__,iRet, g_ulSequenceID, ucMustateSlot[iLoop]);                    
                }

                goto EXIT_LABEL;
            }

//            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s:mtl send success, iRet=%ld G_SeqId %lu,XcuSlot:%d\r\n",
//                __FUNCTION__,iRet, g_ulSequenceID, ucMustateSlot[iLoop]);

        }

        /* 组帧完成后，发送pucSynIdInfo，置位g_bIsReadySendtoMaster */
        g_bIsReadySendtoMaster = TRUE;
        g_bIsModifySession = FALSE;
        ucSynIdTimer = 0;       /* 清空计数器 */

        /* 开启定时器，5s后判断是否有回应信息 */
        (NBB_VOID)BmuWdStart(g_stBfdWdog, BmuWd5MSTickBase(1000), spm_bfd_syncheck_task, 0);
        
    }
    
    iRet = SUCCESS;
    EXIT_LABEL : 
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_synrecv_sessionid
 功能描述  : 业务盘接收主控盘ID回应函数
 输入参数  : IN void *pBuf  :接收到的报文
             IN NBB_ULONG ulBufLength :接收到的报文长度                    
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月5日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_synrecv_sessionid(
    IN NBB_VOID *pBuf, 
    IN NBB_ULONG ulBufLength, 
    IN NBB_USHORT usSlotid)
{
    NBB_ULONG   pulSeqID  = 0;
    NBB_USHORT     usLoopi = 0;         /* 循环变量 */
    SPM_BFD_SYN_BFD_ID   *pucTmpInfo = NULL;
    NBB_INT     iRet = ERROR;
    NBB_BYTE   ucNewEmuId = 0;
    
    (NBB_VOID)usSlotid;     /* 忽略该入参，也可以使用NBB_UNREFERENCED_PARAMETER */
    
    /* 入参判断 */
    if ((NULL == pBuf) || (0 == ulBufLength))
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Syn rev Id, Input is NULL\r\n");
        goto EXIT_LABEL;
    }

    pucTmpInfo = (SPM_BFD_SYN_BFD_ID *)pBuf;
    pulSeqID = pucTmpInfo->ulSequenceID;

    /* 判断回应的序列号是否一致，若一致，则说明该信息有效 */
    if (g_ulSequenceID == pulSeqID)
    {
        /* 轮询本地配置 */
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            /* 判断条件 */
            if ((DRV_USHORT_INVALID != pucTmpInfo->arrucBfdId[usLoopi])
                && (pucTmpInfo->arrucBfdId[usLoopi] == (NBB_USHORT)g_stBfdlocal[usLoopi].ulDiscrimer))
            {
                g_stBfdlocal[usLoopi].bIsSendtoMaster = TRUE;
            }
        }
       
        BMU_SLOG(BMU_DEBUG, SPM_BFD_LOG_DIR, "mani emu successful recv bfd id. G_SeqId = %lu\r\n", g_ulSequenceID);
    }
    else /* 不相等 */
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Msg is not on time coming，SeqID = %ld, G_SeqID = %ld\r\n",
            pulSeqID, g_ulSequenceID);
        
        g_bIsModifySession = TRUE;
    }

    /* 若发生主备交叉倒换 */
    ucNewEmuId = spm_hardw_getemustate();
    if ((ucNewEmuId != g_ucMainEmuId) && (ucNewEmuId != 0xff))
    {
        g_bIsModifySession = TRUE;
      
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "main emu chaned, need resend bfd id, G_SeqId = %lu\r\n", g_ulSequenceID);
    }
    
    g_ulSequenceID++;               /* 序列号加1，为下次同步做准备 */
    g_bIsReadySendtoMaster = FALSE; /* 可以再次同步 */

    /* 关掉5s钟的定时器 */
    (NBB_VOID)BmuWdCancel(g_stBfdWdog);   
    iRet = SUCCESS;
    EXIT_LABEL : 
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_sendpacket_macreg
 功能描述  : 业务盘设置对应mac的寄存器
 输入参数  : IN NBB_USHORT usLocalNum : :本地条目号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月14日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_macreg(IN NBB_USHORT usLocalNum)
{
    NBB_USHORT usData1 = 0;
    NBB_USHORT usData2 = 0;
    NBB_USHORT usData3 = 0;
    NBB_USHORT usData4 = 0;
    NBB_USHORT usData5 = 0;
    SPM_BFD_INTERFACE_DMAC3 stBFDInterfaceDmac3;
    SPM_BFD_FWD_PACKET3 stBfdFwd3;
    SPM_BFD_INTERFACE_SMAC2 stBFDInterfaceSmac2;
    SPM_BFD_INTERFACE_SMAC3 stBFDInterfaceSmac3;
    
    //NBB_TRC_ENTRY(("spm_bfd_sendpacket_macreg"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set send packet mac fpga, input num %d is wrong!\r\n",
            usLocalNum);
        goto EXIT_LABEL;
    }
    OS_MEMSET(&stBFDInterfaceDmac3, 0, sizeof(SPM_BFD_INTERFACE_DMAC3));
    OS_MEMSET(&stBfdFwd3, 0, sizeof(SPM_BFD_FWD_PACKET3));
    OS_MEMSET(&stBFDInterfaceSmac2, 0, sizeof(SPM_BFD_INTERFACE_SMAC2));
    OS_MEMSET(&stBFDInterfaceSmac3, 0, sizeof(SPM_BFD_INTERFACE_SMAC3));

    /* LSP的情况 */
    if (BFD_FWD_LSPID == g_stBfdlocal[usLocalNum].ucBfdFwdType) 
    {
        usData1 = g_stBfdlocal[usLocalNum].usLspID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }

    else if (BFD_FWD_TUNNELID == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* tunnel情况 */
    {
        usData1 = g_stBfdlocal[usLocalNum].usTunnelID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }

    else if (BFD_FWD_PW == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* pw情况 */
    {
        usData1 = g_stBfdlocal[usLocalNum].usNHIID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }

    else if (BFD_FWD_IP == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* ip情况 */
    {
        usData1 = g_stBfdlocal[usLocalNum].usVrfid;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);

        usData2 = (g_stBfdlocal[usLocalNum].ulDip[3] >> 16)&DRV_USHORT_INVALID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData2);

        usData3 = g_stBfdlocal[usLocalNum].ulDip[3]&DRV_USHORT_INVALID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData3);
    }
    else if (BFD_FWD_PORT == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* interface情况 */
    {
        stBFDInterfaceDmac3.usUnit   = g_stBfdlocal[usLocalNum].usUnit;
        stBFDInterfaceDmac3.usSlotid = g_stBfdlocal[usLocalNum].usSlotid;
        usData1 = *(NBB_USHORT *)(&stBFDInterfaceDmac3);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
        
        stBFDInterfaceSmac2.usBIsLag = g_stBfdlocal[usLocalNum].bIsLag;
        stBFDInterfaceSmac2.usPortId = g_stBfdlocal[usLocalNum].usPortID;
        stBFDInterfaceSmac2.usVlanH = (g_stBfdlocal[usLocalNum].usSVlanID >> 8) & DRV_BYTE_INVALID;
        usData2 = *(NBB_USHORT *)(&stBFDInterfaceSmac2);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData2);

        stBFDInterfaceSmac3.usVlanL = g_stBfdlocal[usLocalNum].usSVlanID & DRV_BYTE_INVALID;
        usData3 = *(NBB_USHORT *)(&stBFDInterfaceSmac3);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData3);
    }

    /* 转发类型 */
    stBfdFwd3.usFwdType = g_stBfdlocal[usLocalNum].ucBfdFwdType;
    usData5 = *(NBB_USHORT *)(&stBfdFwd3);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x1, usData5);

    EXIT_LABEL : //NBB_TRC_EXIT();
    return;
        
}

NBB_VOID spm_bfd_id_fpgareg(IN NBB_USHORT usSlotid, IN NBB_BOOL bIsSpu)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_state_fpgareg
 功能描述  : 将本盘的会话状态写入背板总线
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月29日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_state_fpgareg(NBB_VOID)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usLoopj = 0;
    NBB_UINT uiData = 0;
    SPM_BFD_REG_STATE stState;

    for (usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM; usLoopi = usLoopi + 16) /* 循环 */
    {
        /* TODO:优化效率 */
        OS_MEMSET(&stState, 0, sizeof(SPM_BFD_REG_STATE));
        stState.uiState1 = g_stBfdlocal[usLoopi].ucStateRecord;
        if ((usLoopi + 1) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState2 = g_stBfdlocal[usLoopi + 1].ucStateRecord;
        }
        if ((usLoopi + 2) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState3 = g_stBfdlocal[usLoopi + 2].ucStateRecord;
        }
        if ((usLoopi + 3) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState4 = g_stBfdlocal[usLoopi + 3].ucStateRecord;
        }
        if ((usLoopi + 4) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState5 = g_stBfdlocal[usLoopi + 4].ucStateRecord;
        }
        if ((usLoopi + 5) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState6 = g_stBfdlocal[usLoopi + 5].ucStateRecord;
        }
        if ((usLoopi + 6) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState7 = g_stBfdlocal[usLoopi + 6].ucStateRecord;
        }
        if ((usLoopi + 7) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState8 = g_stBfdlocal[usLoopi + 7].ucStateRecord;
        }
        if ((usLoopi + 8) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState9 = g_stBfdlocal[usLoopi + 8].ucStateRecord;
        }
        if ((usLoopi + 9) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState10 = g_stBfdlocal[usLoopi + 9].ucStateRecord;
        }
        if ((usLoopi + 10) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState11 = g_stBfdlocal[usLoopi + 10].ucStateRecord;
        }
        if ((usLoopi + 11) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState12 = g_stBfdlocal[usLoopi + 11].ucStateRecord;
        }
        if ((usLoopi + 12) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState13 = g_stBfdlocal[usLoopi + 12].ucStateRecord;
        }
        if ((usLoopi + 13) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState14 = g_stBfdlocal[usLoopi + 13].ucStateRecord;
        }
        if ((usLoopi + 14) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState15 = g_stBfdlocal[usLoopi + 14].ucStateRecord;
        }
        if ((usLoopi + 15) < BFD_MAX_SESNUM) /* 判断条件 */
        {
            stState.uiState16 = g_stBfdlocal[usLoopi + 15].ucStateRecord;
        }
        uiData = *(NBB_UINT *)(&stState);
        spm_oamtree_s32write(BFDSTATEBASE, 0x10000 + usLoopj, uiData);
        usLoopj++;
    }
}


/*****************************************************************************
 函 数 名  : spm_bfd_init_rxpacket
 功能描述  : 注册C3，接收上CPU的BFD报文
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月26日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_init_rxpacket(NBB_VOID)
{
    c3_rxreg_info_t  stC3RxInfo;

    stC3RxInfo.type = C3_PKT_TYPE_BFD;
    stC3RxInfo.name = "bfd_c3_to_cp";
    stC3RxInfo.callback = bfd_c3_to_cp;
    stC3RxInfo.priority = 0;
 
    (NBB_VOID)ApiC3RxCbRegister(0, &stC3RxInfo);
    (NBB_VOID)ApiC3RxCbRegister(1, &stC3RxInfo);

    g_stBfdMsgQID = BmuQueueCreate("BFDMSGQUE", BFD_MSG_DEEP_QUEUE, BFD_MSG_BUF_LENTH);
    if (BMU_INVALID_MSG_ID == g_stBfdMsgQID) /* 创建消息队列失败 */
    {
        NBB_PRINTF("Creat Que Err!\n");
        return ERROR;
    }
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_Bfd_msgque_read
 功能描述  : 读MSG队列函数
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月26日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_msgque_read(NBB_VOID)
{
    NBB_INT iMsgLenth = 0;
    NBB_CHAR acBuf[BFD_MSG_BUF_LENTH];
    iMsgLenth = BmuQueueRecv(g_stBfdMsgQID, acBuf, BFD_MSG_BUF_LENTH, BMU_WAIT_FOREVER);
    if (ERROR == iMsgLenth)
    {
        fprintf(stderr, "%s:%d BmuQueueRecv ERROR, %s.\n", __FUNCTION__, __LINE__, strerror(errno));
    }
    else if (iMsgLenth > 0)
    {
        spm_bfd_packet_receive((NBB_BYTE*)acBuf, iMsgLenth);
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_msgque_task
 功能描述  : 读队列任务
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_msgque_task(NBB_VOID)
{
    while(1)
    {
        spm_bfd_msgque_read();
        BmuTaskTestPoint();
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_init_Nni_table
 功能描述  : NNI表项初始化，此函数在主控盘上为桩函数，业务盘才有NNI表项，支持1024条
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月31日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_init_Nni_table(NBB_CXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0; /* 循环变量 */
    NBB_INT iRet = ERROR;

    NBB_TRC_ENTRY(("spm_bfd_init_fwdtable"));

    /* 转发软件表项初始化 */
    for (usLoopi = 0; usLoopi < BFD_MAX_NNINUM; usLoopi++)
    {
        g_stBfdNni[usLoopi].ulDrim = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulIpv4 = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulPwLabel = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulNniId = DRV_ULONG_INVALID;
    }

    /* BFD NNI表二叉树初始化，每业务盘支持1k转发表 */    
    g_usNniTreeLevel  = 10;
    g_usNniTreeNodeMaxnum = (1 << g_usNniTreeLevel);
    g_usNniNoMax   = g_usNniTreeNodeMaxnum - 1;
    g_usNniNum = 0;
    g_pulNniTreebase = (NBB_ULONG *)NBB_MM_ALLOC(sizeof(NBB_ULONG)*g_usNniTreeLevel,
        NBB_NORETRY_ACT, MEM_SPM_BFD_PUL_NNI_TREE_BASE_CB);
    if (NULL == g_pulNniTreebase)/* 内存申请失败 */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Malloc error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Malloc error\r\n");
        goto EXIT_LABEL;
    }
    
    NBB_MEMSET(g_pulNniTreebase, 0 ,sizeof(NBB_ULONG)*g_usNniTreeLevel);
    iRet = spm_oamtree_init(g_usNniTreeNodeMaxnum, spm_nni_tree_get_fpga_data, &g_stTreeNniNodeSet);
    if (SUCCESS != iRet)/* 判断执行结果 */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Fwd node set error, nodenum = %d", g_usNniTreeNodeMaxnum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd node set error, nodenum = %d\r\n", g_usNniTreeNodeMaxnum);
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_add_nniId_table
 功能描述  : ##
 输入参数  : NBB_ULONG ulDrim                     
             NBB_ULONG ulPeerIp                   
             NBB_ULONG ulPwLabel                  
             NBB_ULONG ulNniIdNBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_add_nniId_table(NBB_ULONG ulDrim,
    NBB_ULONG ulPeerIp,
    NBB_ULONG ulPwLabel, 
    NBB_ULONG ulNniId
    NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    NBB_USHORT usLoopi = 0;
    SPM_OAMTREE_NODE stBfdTreeNode;
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "ulDrim:%lu, ulPeerIp:%#x, ulPwLabel:%lu, ulNniId:%lu\r\n",
        ulDrim, ulPeerIp, ulPwLabel, ulNniId);
    
    /* 添加配置 */
    for (usLoopi = 0; usLoopi < BFD_MAX_NNINUM; usLoopi++)
    {
        if (DRV_ULONG_INVALID == g_stBfdNni[usLoopi].ulDrim)/* 可添加 */
        {
            g_stBfdNni[usLoopi].ulDrim = ulDrim;
            g_stBfdNni[usLoopi].ulIpv4 = ulPeerIp;
            g_stBfdNni[usLoopi].ulPwLabel = ulPwLabel;
            g_stBfdNni[usLoopi].ulNniId = ulNniId;
            
            /* 加节点 */
            iRet = spm_nni_tree_set_data(usLoopi, g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel, &stBfdTreeNode);
            if (SUCCESS != iRet)/* 不成功 */
            {
                /* 将会话信息至为无效值 */
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                    "Set NNI node failed, ulPeerIp = %#x, ulPwLabel = %lu, ulNniId = %lu,iRet=%d\r\n",
                    ulPeerIp, ulPwLabel, ulNniId, iRet);
                goto ERROR_LABEL;
            }

            iRet = spm_oamtree_nodeadd(g_usNniNoMax, &stBfdTreeNode, &g_stTreeNniNodeSet);
            if (SUCCESS != iRet)/* 不成功 */
            {
                /* 将会话信息至为无效值 */
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                    "Add NNI node failed, ulPeerIp = %#x, ulPwLabel = %lu, ulNniId = %lu,iRet=%d\r\n",
                    ulPeerIp, ulPwLabel, ulNniId, iRet);
                goto ERROR_LABEL;
            }
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  

            iRet = spm_oamtree_build(g_usNniTreeLevel, &g_stTreeNniNodeSet, 
                g_pulNniTreebase, OAMNNIFPAGBASE);
            
            if (SUCCESS != iRet) /* 判断执行结果 */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                    "Build NNI main Tree failed, ulPeerIp = %#x, ulPwLabel = %lu, ulNniId = %lu\r\n",
                    ulPeerIp, ulPwLabel, ulNniId);
                goto ERROR_LABEL;
            }

            /* 翻页写备表 */
            spm_oamtree_s16write(OAMNNIDATAFPAGBASE, NNITREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usNniTreeLevel, &g_stTreeNniNodeSet, 
                g_pulNniTreebase, OAMNNIFPAGBASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                    "Build NNI back Tree failed, ulPeerIp = %#x, ulPwLabel = %lu, ulNniId = %lu\r\n",
                    ulPeerIp, ulPwLabel, ulNniId);
                goto ERROR_LABEL;
            }

            spm_oamtree_s16write(OAMNNIDATAFPAGBASE, NNITREESWITCHOFFSET, 1);

            /* 以条目号为索引，将组织好的转发内容写FPGA */
            spm_bfd_nniId_fpgareg(usLoopi);
            break;
            
        }
    }

    /* 没有找到空余条目，说明异常，可能在删除会话时未还原初始化 */
    if (BFD_MAX_NNINUM == usLoopi)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Error, please check !!!!\n");
        goto EXIT_LABEL;
    }
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add nni end\r\n");
    goto EXIT_LABEL;

ERROR_LABEL:
    /* 将会话信息至为无效值 */
    if (usLoopi < BFD_MAX_NNINUM)
    {
        g_stBfdNni[usLoopi].ulDrim = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulIpv4 = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulPwLabel = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulNniId = DRV_ULONG_INVALID;
    }

EXIT_LABEL :
    return iRet;    
}


/*****************************************************************************
 函 数 名  : spm_bfd_delfwdtable
 功能描述  : 删除转发表项
 输入参数  : IN NBB_ULONG ulDrim  :描述符
             NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月15日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_del_nniId_table(NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;

    NBB_TRC_ENTRY(("spm_bfd_del_nniId_table"));
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete nni %lu begin\r\n", ulDrim);
    /* 轮询本地会话配置 */
    for (usLoopi = 0; usLoopi < BFD_MAX_NNINUM; usLoopi++)
    {
        if (ulDrim == g_stBfdNni[usLoopi].ulDrim)/* 判断条件 */
        {
            iRet = spm_nni_tree_set_data(usLoopi, g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel, &stBfdTreeNode);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {   
                /* 清空FPGA对应的发送寄存器内容 */
                spm_bfd_clear_nniId_fpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Set node failed, peerIp = %#x, pwLabel = %lu",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set node failed, peerIp = %#x, pwLabel = %lu\r\n",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
                goto ERROR_LABEL;
            }

            /* 删除节点 */
            iRet = spm_oamtree_nodedel(g_usNniNoMax, &stBfdTreeNode, &g_stTreeNniNodeSet);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {   
                /* 清空FPGA对应的发送寄存器内容 */
                spm_bfd_clear_nniId_fpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Del node failed, peerIp = %#x, pwLabel = %lu",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del node failed, peerIp = %#x, pwLabel = %lu\r\n",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
                goto ERROR_LABEL;
            }
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  

            /* 重建会话二叉树 */
            iRet = spm_oamtree_build(g_usNniTreeLevel, &g_stTreeNniNodeSet, 
                g_pulNniTreebase, OAMNNIFPAGBASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {                
                /* 清空FPGA对应的发送寄存器内容 */
                spm_bfd_clear_nniId_fpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Build nni main Tree failed, peerIp = %#x, pwLabel = %lu",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build nni main Tree failed, peerIp = %#x, pwLabel = %lu\r\n",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
                goto ERROR_LABEL;
            }
            
            /* 翻页写备表 */
            spm_oamtree_s16write(OAMNNIDATAFPAGBASE, NNITREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usNniTreeLevel, &g_stTreeNniNodeSet, 
                g_pulNniTreebase, OAMNNIFPAGBASE);
            if (SUCCESS != iRet)/* 判断执行结果 */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Build nni back Tree failed, peerIp = %#x, pwLabel = %lu",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build nni back Tree failed, peerIp = %#x, pwLabel = %lu\r\n",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
                goto ERROR_LABEL;
            }
            spm_oamtree_s16write(OAMNNIDATAFPAGBASE, NNITREESWITCHOFFSET, 1);
            
            /* 将会话信息至为无效值 */
            if (usLoopi < BFD_MAX_NNINUM)
            {
                g_stBfdNni[usLoopi].ulDrim = DRV_ULONG_INVALID;
                g_stBfdNni[usLoopi].ulIpv4 = DRV_ULONG_INVALID;
                g_stBfdNni[usLoopi].ulPwLabel = DRV_ULONG_INVALID;
                g_stBfdNni[usLoopi].ulNniId = DRV_ULONG_INVALID;
            }

            /* 清空FPGA对应的发送寄存器内容 */
            spm_bfd_clear_nniId_fpgareg(usLoopi NBB_CCXT);
            
            break;
        }
    }

    if (BFD_MAX_NNINUM == usLoopi)/* 判断条件 */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Not exit, peerIp = %#x, pwLabel = %lu",
            g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "ulDrim%lu Not exit, peerIp = %#x, pwLabel = %lu\r\n",
            ulDrim, g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
        goto EXIT_LABEL;
    }
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete nni end\r\n");
    
ERROR_LABEL:
    /* 将会话信息至为无效值 */
    if (usLoopi < BFD_MAX_NNINUM)
    {
        g_stBfdNni[usLoopi].ulDrim = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulIpv4 = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulPwLabel = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulNniId = DRV_ULONG_INVALID;
    }
    
EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_nniId_fpgareg
 功能描述  : ##
 输入参数  : IN NBB_USHORT usLocalNum  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_nniId_fpgareg(IN NBB_USHORT usLocalNum)
{
    NBB_ULONG           usData1     = 0;
    NBB_INT     iRet = ERROR;

    /* 入参判断 */
    if (usLocalNum >= BFD_MAX_NNINUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set nni fpga, input num %d is wrong!\r\n", usLocalNum);
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    
    usData1 = g_stBfdNni[usLocalNum].ulNniId;
    spm_oamtree_s32write(OAMNNIPAKFPGABASE, usLocalNum, usData1);
    iRet = SUCCESS;
    
    EXIT_LABEL :
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_bfd_clear_nniId_fpgareg
 功能描述  : 会话删除时清除FPGA中转发寄存器内容
 输入参数  : IN NBB_USHORT usLocalNum :本地条目号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月17日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_clear_nniId_fpgareg(IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usData = 0;
    
    NBB_TRC_ENTRY(("spm_bfd_clear_nniId_fpgareg"));
    spm_oamtree_s16write(OAMNNIPAKFPGABASE, usLocalNum, usData);
        
    NBB_TRC_EXIT();
    return;
}
#endif

#ifdef SRC

/*****************************************************************************
 函 数 名  : spm_bfd_init_bus_map_table
 功能描述  : 初始化 SRC FPGA 0x930000 ~ 0x9353ff 为0xffff，此函数在业务盘上为桩函数。
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月15日
    作    者   : tianf
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_bfd_state_map_init(NBB_CXT_T NBB_CXT)
{
    NBB_USHORT usSlotid = 0;
    NBB_USHORT usLoopi = 0;
    NBB_ULONG  ulBaseAddress = 0;
    NBB_USHORT usDate = 0x5400;

    NBB_TRC_ENTRY(__FUNCTION__);
    
    /* 1到21槽位总线*/
    for(usSlotid = 1; usSlotid <= SYS_MAX_SLOTNUM + 8; usSlotid++)
    {
        ulBaseAddress = (usSlotid - 1)*0x400 + 0x30000;
        for(usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM + 1; usLoopi++)
        {
            spm_oamtree_s16write(BFDSTATEBASE, (ulBaseAddress + usLoopi), DRV_USHORT_INVALID);        
        }
    }

    /*22槽位  固定写 0x5400~ 0x57ff*/
    ulBaseAddress = (22 - 1)*0x400 + 0x30000;
    for(usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM + 1; usLoopi++)
    {
        spm_oamtree_s16write(BFDSTATEBASE, (ulBaseAddress + usLoopi), usDate); 
        usDate++;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return SUCCESS;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_init_fpga
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月14日
 * 函数功能  : 初始化BFD 相关的FPGA寄存器
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID spm_bfd_init_fpga(NBB_VOID)
{
    /* 初始化BFD告警同步映射表 */
    spm_proc_bfd_state_map_init();
    return;
}


/*****************************************************************************
 函 数 名  : spm_bfd_init_fwdtable
 功能描述  : 转发表项初始化，此函数在主控盘上为桩函数，业务盘才有转发表项，支持2047条
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月31日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_init_fwdtable(NBB_CXT_T NBB_CXT)
{
    return SUCCESS;
}

NBB_INT spm_bfd_init_rxpacket(NBB_VOID)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_syn_sessionid
 功能描述  : 主控盘函数，判断ID是否发生变化，若变化则写入FPGA寄存器
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月4日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT  spm_bfd_syn_sessionid(NBB_VOID)
{
    NBB_BYTE    ucLocalSlot = 0;
    NBB_BYTE    ucMainBoardId = 0;
    NBB_USHORT  usLoopi = 0;
    
    /* 当前盘为主用主控盘 */
    ucMainBoardId = spm_hardw_getemustate();
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    if (ucMainBoardId == ucLocalSlot)
    {
        /* 主控盘情况与业务盘不同，主控盘直接将本盘处理的会话ID进行记录并写入FPGA */
        if (TRUE == g_bIsModifySession)
        {
            for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* 循环 */
            {
                g_stBfdIdStore[ucLocalSlot - 1].stBfdIdInfo[usLoopi].ulDiscrimer = 
                    g_stBfdlocal[usLoopi].ulDiscrimer;          /* 使用全局变量，是为了打印调试方便 */
                g_stBfdlocal[usLoopi].bIsSendtoMaster = TRUE;
            }

            /* 将描述ID写入FPGA寄存器 */
            spm_bfd_id_fpgareg(ucLocalSlot, FALSE);

            /* 完成操作了需要清空，防止下次被重新写 */
            g_bIsModifySession = FALSE;
            
            BMU_SLOG(BMU_DEBUG, SPM_BFD_LOG_DIR, "write local bfd id to fpga success\r\n");
        }
    }
    
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_synrecv_sessionid
 功能描述  : 主控盘接收业务盘同步的ID信息
 输入参数  : IN NBB_VOID *pBuf  :接收到的报文
             IN NBB_ULONG ulBufLength :接收到的报文长度  
             IN USHORT usSlotid :同步信息的槽位号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月5日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

  2.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 根据MTL要求修改
*****************************************************************************/
NBB_INT spm_bfd_synrecv_sessionid(
    IN NBB_VOID *pBuf, 
    IN NBB_ULONG ulBufLength, 
    IN NBB_USHORT usSlotid)
{
    NBB_USHORT     usLoopi = 0;         /* 循环变量 */
    NBB_BYTE   *pucTmpInfo = NULL;
    SPM_BFD_SYN_BFD_ID     *pusTmp     = NULL;
    NBB_INT     iRet = ERROR;
    NBB_CHAR *pcLinxData = NULL;
    MTL_NODE mtl_head = {0};
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMustate = 0;
    
    /* 入参判断 */
    if ((NULL == pBuf) || (0 == ulBufLength))
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Master Board Syn rev Id, Input is NULL\r\n");
        goto EXIT_LABEL;
    }

    if ((usSlotid > SYS_MAX_SLOTNUM) || (0 == usSlotid))/* 判断条件 */  /* TODO：SYS_MAX_SLOTNUM需要根据不同的单盘改变大小 */
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Master Board Syn rev Id, Slotid = %d is error\r\n", usSlotid);
        goto EXIT_LABEL;
    }

    pucTmpInfo = (NBB_BYTE *)pBuf;
    
    pusTmp = (SPM_BFD_SYN_BFD_ID *)pucTmpInfo;
    
    /* 轮询配置 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        /* 一次发送1k个数据 */
        g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopi].ulDiscrimer = pusTmp->arrucBfdId[usLoopi];  
    }

    /* 将ID内容写入FPGA */
    spm_bfd_id_fpgareg(usSlotid, TRUE);
    
    BMU_SLOG(BMU_DEBUG, SPM_BFD_LOG_DIR, 
             "write slot %u bfd id to fpga success, G_SeqId = %lu\r\n", usSlotid, g_ulSequenceID);

    iRet = spm_bfd_hardw_getslot(&ucLocalSlot);    /* 得到本盘槽位号 */
    if ( SUCCESS != iRet ) /* 获取本盘槽位失败 */
    {
        BMU_SLOG(BMU_ERR, SPM_BFD_LOG_DIR, "get local slot error:%d\r\n", iRet);
        goto EXIT_LABEL;
    }

    ucMustate = spm_hardw_getemustate();        /* 获取主用主控槽位 */
    if ( 0xff == ucMustate ) /* 当0xff == ucMustate成立时执行 */
    {
        BMU_SLOG(BMU_ERR, SPM_BFD_LOG_DIR, "get mustate error:%d\r\n", ucMustate);
        goto EXIT_LABEL;
    }

    if (ucLocalSlot == ucMustate)   /* 只有主用主控才需要回包 */
    {
        if (0 == spm_hardw_getboardtype(usSlotid))
        {
            BMU_SLOG(BMU_ERR, SPM_BFD_LOG_DIR, "error master send to itself, src slot:%d\r\n", usSlotid);
            goto EXIT_LABEL;
        }
        /* 发送Buf给业务盘，回应ID信息 */
        pcLinxData = malloc(g_ulSynIdInfoLth);
        if (NULL == pcLinxData)//内存不够
        {
            goto EXIT_LABEL;
        }
        pucTmpInfo = (NBB_BYTE *)pcLinxData;
#if 0
        /* 组发往主控CPU的头 */
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.module_id = TYPE_MTL_BFD;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.frame = g_stDesNode.frame;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.slot = usSlotid;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.cpu = 0;
#endif

        /* 组同步内容的头 */
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->syn_head.ucType = BFD_FWD_MSG_ID;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->syn_head.ulSrcSlot = g_stSrcNode.slot;

        /* 组包的内容 */
        pucTmpInfo = pucTmpInfo + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD);
        memcpy(pucTmpInfo, pBuf, ulBufLength);
        
        /* 2015.5.21采用mtl方式发送后，本槽位直接发往交换CPU,不需要先发往主控再通过主控发往交换CPU, 
          由于修改建立在之前linx代码基础上，此处发送消息是去掉MTL_NODE,对于mtl来说它为多余*/
        mtl_send(mtl_identity_to_fd(1 << 24 | usSlotid << 16 | 0),
            TYPE_MTL_BFD, TYPE_MTL_BFD, 
        pcLinxData + sizeof(MTL_NODE), 
        g_ulSynIdInfoLth - sizeof(MTL_NODE));
        free(pcLinxData);
        pcLinxData = NULL;
        
        #if 0
        /* 目的地址 */
        g_stDesNode.module_id = TYPE_MTL_TNE;
        g_stDesNode.slot = spm_hardw_getemustate(); /* 发往主用主控，由主用主控转发 */
        g_stDesNode.cpu = 1;                        /* 发送到主控的CPU,再由主控转到交叉的CPU */
        
        iRet = pal_mtl_send(g_stSrcNode, (void **)(&pcLinxData), g_stDesNode);
        if (SUCCESS != iRet)
        {
            BMU_LOG(BMU_INFO, SPM_BFD_LOG_DIR, "mtl send to slot %u error = %d, G_SeqId = %lu\r\n", usSlotid, iRet, g_ulSequenceID);
            goto EXIT_LABEL;
        }
       #endif

        /* 发送失败 */
        if(iRet < 0)
        {
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s:mtl_send error = %d! dSlotid= %d\r\n",__FUNCTION__, iRet, usSlotid);
        }
           
             
    }

    iRet = SUCCESS;
    EXIT_LABEL : 
    return iRet;
        
}

/*****************************************************************************
 函 数 名  : spm_bfd_sendpacket_macreg
 功能描述  : 主控盘设置对应mac的寄存器
 输入参数  : IN NBB_USHORT usLocalNum : :本地条目号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月14日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_macreg(IN NBB_USHORT usLocalNum)
{
    NBB_USHORT usData1 = 0;
    NBB_USHORT usData2 = 0;
    NBB_USHORT usData3 = 0;
    NBB_USHORT usData4 = 0;
    NBB_USHORT usData5 = 0;
    NBB_USHORT usData6 = 0;
    SPM_BFD_FWD_PACKET1 stBfdFwd1;
    SPM_BFD_FWD_PACKET2 stBfdFwd2;
    SPM_BFD_FWD_PACKET3 stBfdFwd3;
    NBB_BYTE ucLocalSlot;
    SPM_BFD_INTERFACE_DMAC3 stBfdInterfaceDmac3;
    SPM_BFD_INTERFACE_SMAC2 stBfdInterfaceSmac2;
    SPM_BFD_INTERFACE_SMAC3 stBfdInterfaceSmac3;
    SPM_BFD_PW_SMAC2        stBfdPwSmac2;
    SPM_BFD_PW_SMAC3        stBfdPwSmac3;
    //NBB_TRC_ENTRY(("spm_bfd_sendpacket_macreg"));
    
    /* 判断入参 */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set send packet mac fpga, input num %d is wrong!", usLocalNum);
        goto EXIT_LABEL;
    }
    OS_MEMSET(&stBfdFwd1, 0, sizeof(SPM_BFD_FWD_PACKET1));
    OS_MEMSET(&stBfdFwd2, 0, sizeof(SPM_BFD_FWD_PACKET2));
    OS_MEMSET(&stBfdFwd3, 0, sizeof(SPM_BFD_FWD_PACKET3));
    OS_MEMSET(&stBfdInterfaceDmac3, 0, sizeof(SPM_BFD_INTERFACE_DMAC3));
    OS_MEMSET(&stBfdInterfaceSmac2, 0, sizeof(SPM_BFD_INTERFACE_SMAC2));
    OS_MEMSET(&stBfdInterfaceSmac3, 0, sizeof(SPM_BFD_INTERFACE_SMAC3));
    OS_MEMSET(&stBfdPwSmac2, 0, sizeof(SPM_BFD_PW_SMAC2));
    OS_MEMSET(&stBfdPwSmac3, 0, sizeof(SPM_BFD_PW_SMAC3));

    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    
    /* LSP的情况 */
    if (BFD_FWD_LSPID == g_stBfdlocal[usLocalNum].ucBfdFwdType) 
    {
        usData1 = g_stBfdlocal[usLocalNum].usLspID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }
    else if (BFD_FWD_TUNNELID == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* tunnel情况 */
    {
        usData1 = g_stBfdlocal[usLocalNum].usTunnelID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }
    else if (BFD_FWD_IP == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* ip情况 */
    {
        usData1 = g_stBfdlocal[usLocalNum].usVrfid;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);

        usData2 = (g_stBfdlocal[usLocalNum].ulDip[3] >> 16)&DRV_USHORT_INVALID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData2);

        usData3 = g_stBfdlocal[usLocalNum].ulDip[3]&DRV_USHORT_INVALID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData3);
    }
    else if (BFD_FWD_PORT == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* interface情况 */
    {
        stBfdInterfaceDmac3.usUnit   = 0;
        stBfdInterfaceDmac3.usSlotid = g_stBfdlocal[usLocalNum].usSlotid;
        usData1 = *(NBB_USHORT *)(&stBfdInterfaceDmac3);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
        
        stBfdInterfaceSmac2.usBIsLag = g_stBfdlocal[usLocalNum].bIsLag;
        stBfdInterfaceSmac2.usPortId = g_stBfdlocal[usLocalNum].usPortID;
        stBfdInterfaceSmac2.usVlanH = (g_stBfdlocal[usLocalNum].usSVlanID >> 8) & DRV_BYTE_INVALID;
        usData2 = *(NBB_USHORT *)(&stBfdInterfaceSmac2);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData2);

        stBfdInterfaceSmac3.usVlanL = g_stBfdlocal[usLocalNum].usSVlanID & DRV_BYTE_INVALID;
        usData3 = *(NBB_USHORT *)(&stBfdInterfaceSmac3);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData3);
    }
    else if (BFD_FWD_PW == g_stBfdlocal[usLocalNum].ucBfdFwdType)       /* PW情况 */
    {
        stBfdPwSmac2.usPeerIpH = (g_stBfdlocal[usLocalNum].ulPeerId >> 16) & DRV_USHORT_INVALID;
        usData2 = *(NBB_USHORT *)(&stBfdPwSmac2);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData2);

        stBfdPwSmac3.usPeerIpL = g_stBfdlocal[usLocalNum].ulPeerId & DRV_USHORT_INVALID;
        usData3 = *(NBB_USHORT *)(&stBfdPwSmac3);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData3);
    }

    /* 封装ITMH头 */
    stBfdFwd1.usFwdTrafficClass = 7; /* 报文走arad优先级 */
    stBfdFwd1.usFwdDesInfo1 = 0xc;
    usData4 = *(NBB_USHORT *)(&stBfdFwd1);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x1, usData4);

    /* 获取当前在位的第一块SPU盘的槽位号 */
    stBfdFwd2.usSysPortID = g_ucFirstNpBoard * 256 + BFD_ARAD_LOGICPORT; 
    usData5 = *(NBB_USHORT *)(&stBfdFwd2);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x2, usData5);
    
    stBfdFwd3.usDslotid = g_ucFirstNpBoard; 
    stBfdFwd3.usSslotid = ucLocalSlot;
    stBfdFwd3.usPktType = BFD_DUNE_TOC3;
    stBfdFwd3.usFwdType = g_stBfdlocal[usLocalNum].ucBfdFwdType;
    usData6 = *(NBB_USHORT *)(&stBfdFwd3);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x1, usData6);
    
    EXIT_LABEL : //NBB_TRC_EXIT();
    return;
}


/*****************************************************************************
 函 数 名  : spm_bfd_id_fpgareg
 功能描述  : 主控盘将ID按照槽位号写入FPGA
 输入参数  : NBB_USHORT usSlotid
             NBB_BOOL bIsSpu
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月26日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_id_fpgareg(IN NBB_USHORT usSlotid, IN NBB_BOOL bIsSpu)
{
    NBB_USHORT usLoopi = 0;
    NBB_ULONG  ulBaseAddress = 0;
    
    //NBB_TRC_ENTRY(("spm_bfd_id_fpgareg"));

    /* 入参判断 */
    if ((usSlotid > SYS_MAX_SLOTNUM) || (0 == usSlotid))
    {
        goto EXIT_LABEL;
    }
    if (TRUE ==  bIsSpu)/* 业务盘 */
    {
        ulBaseAddress = (usSlotid - 1)*0x400 + 0x30000;
    }
    else 
    {
        /* 固定写入第21槽位对应的地址 */
        ulBaseAddress = (21 - 1)*0x400 + 0x30000;
    }
    
    for (usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM + 1; usLoopi++)/* 循环 */
    {
        if (DRV_USHORT_INVALID != (NBB_USHORT)g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopi].ulDiscrimer)
        {
            spm_oamtree_s16write(BFDSTATEBASE, (ulBaseAddress + usLoopi), 
                (NBB_USHORT)(g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopi].ulDiscrimer - 1));
        }
        else
        {
            spm_oamtree_s16write(BFDSTATEBASE, (ulBaseAddress + usLoopi), DRV_USHORT_INVALID);
        }
    }
    
    EXIT_LABEL : //NBB_TRC_EXIT();
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_state_fpgareg
 功能描述  : 将主用主控盘的会话状态写入背板总线
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月29日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_state_fpgareg(NBB_VOID)
{
    NBB_USHORT usLoopi = 0;
            
    /* 固定写入对应的地址 */
    for (usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM; usLoopi++)/* 循环 */
    {
        spm_oamtree_s16write(BFDSTATEBASE, 0x60000 + usLoopi, g_stBfdlocal[usLoopi].ucStateRecord);
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_msgque_task
 功能描述  : 读队列任务，SRC打桩
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月27日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_msgque_task(NBB_VOID)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_init_Nni_table
 功能描述  : NNI表项初始化，此函数在主控盘上为桩函数，业务盘才有NNI表项，支持1024条
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月31日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_init_Nni_table(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY(("spm_bfd_init_fwdtable"));
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return SUCCESS;
}


NBB_INT spm_bfd_add_nniId_table(NBB_ULONG ulDrim,
    NBB_ULONG ulPeerIp,
    NBB_ULONG ulPwLabel, 
    NBB_ULONG ulNniId
    NBB_CCXT_T NBB_CXT)
{
    return SUCCESS;
}


/*****************************************************************************
 函 数 名  : spm_bfd_delfwdtable
 功能描述  : 删除转发表项
 输入参数  : IN NBB_ULONG ulDrim  :描述符
             NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月15日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_del_nniId_table(NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY(("spm_bfd_del_nniId_table"));
    
EXIT_LABEL: NBB_TRC_EXIT();

    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_nniId_fpgareg
 功能描述  : ##
 输入参数  : IN NBB_USHORT usLocalNum  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_nniId_fpgareg(IN NBB_USHORT usLocalNum)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_clear_nniId_fpgareg
 功能描述  : 会话删除时清除FPGA中转发寄存器内容
 输入参数  : IN NBB_USHORT usLocalNum :本地条目号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月17日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_clear_nniId_fpgareg(IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CXT)
{
    return;
}

#endif

#ifdef SNU

/*****************************************************************************
 函 数 名  : spm_bfd_init_fpga_map_table
 功能描述  : 初始化 SRC FPGA 0x930000 ~ 0x932fff 为0xffff，此函数在业务盘上为桩函数。
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年11月15日
    作    者   : tianf
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_bfd_state_map_init(NBB_CXT_T NBB_CXT)
{
    return SUCCESS;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_init_fpga
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月14日
 * 函数功能  : 初始化BFD 相关的FPGA寄存器
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_VOID spm_bfd_init_fpga(NBB_VOID)
{
    /* 初始化BFD告警同步映射表 */
    spm_proc_bfd_state_map_init();
    return;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_init_fwdtable
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年11月14日
 * 函数功能  : 初始化BFD 的fwdtable
 * 输入参数  : 无
 * 输出参数  : 无
 * 返 回 值  : NBB_VOID  无
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT spm_bfd_init_fwdtable(NBB_CXT_T NBB_CXT)
{
    return SUCCESS;
}

NBB_INT spm_bfd_init_rxpacket(NBB_VOID)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_syn_sessionid
 功能描述  : 主控盘函数，判断ID是否发生变化，若变化则写入FPGA寄存器
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月4日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数
*****************************************************************************/
NBB_INT  spm_bfd_syn_sessionid(NBB_VOID)
{
    return SUCCESS;
}

NBB_INT spm_bfd_synrecv_sessionid(
    IN NBB_VOID *pBuf, 
    IN NBB_ULONG ulBufLength, 
    IN NBB_USHORT usSlotid)
{
    return SUCCESS;
}

NBB_VOID spm_bfd_sendpacket_macreg(IN NBB_USHORT usLocalNum)
{
    return;
}

NBB_VOID spm_bfd_id_fpgareg(IN NBB_USHORT usSlotid, IN NBB_BOOL bIsSpu)
{
    return;
}

NBB_VOID spm_bfd_state_fpgareg(NBB_VOID)
{
    return;
}

NBB_VOID spm_bfd_msgque_task(NBB_VOID)
{
    return;
}

/*****************************************************************************
 函 数 名  : spm_bfd_init_Nni_table
 功能描述  : NNI表项初始化，此函数在主控盘上为桩函数，业务盘才有NNI表项，支持1024条
 输入参数  : NBB_VOID  
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年8月31日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_init_Nni_table(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY(("spm_bfd_init_fwdtable"));
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_add_nniId_table
 功能描述  : ..
 输入参数  : NBB_ULONG ulDrim                     
             NBB_ULONG ulPeerIp                   
             NBB_ULONG ulPwLabel                  
             NBB_ULONG ulNniIdNBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_add_nniId_table(NBB_ULONG ulDrim,
    NBB_ULONG ulPeerIp,
    NBB_ULONG ulPwLabel, 
    NBB_ULONG ulNniId
    NBB_CCXT_T NBB_CXT)
{
    return SUCCESS;
}


/*****************************************************************************
 函 数 名  : spm_bfd_delfwdtable
 功能描述  : 删除转发表项
 输入参数  : IN NBB_ULONG ulDrim  :描述符
             NBB_CCXT_T NBB_CCXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年10月15日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_del_nniId_table(NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY(("spm_bfd_del_nniId_table"));
    
EXIT_LABEL: NBB_TRC_EXIT();

    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_nniId_fpgareg
 功能描述  : ..
 输入参数  : IN NBB_USHORT usLocalNum  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年5月21日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_nniId_fpgareg(IN NBB_USHORT usLocalNum)
{
    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_bfd_clear_nniId_fpgareg
 功能描述  : 会话删除时清除FPGA中转发寄存器内容
 输入参数  : IN NBB_USHORT usLocalNum :本地条目号
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月17日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_clear_nniId_fpgareg(IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CXT)
{
    return;
}
#endif

/*****************************************************************************
 函 数 名  : spm_bfd_fwdpacket_receive
 功能描述  : 各盘接收从其他盘转发来的YOURID为0的握手包
 输入参数  : IN NBB_BYTE *pBuf  
             IN NBB_ULONG ulMsgLenth
             IN NBB_USHORT usSlotid
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月7日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数
 备注          : 收到其他盘的握手down报文，说明其他盘已经查询过全局配置了，能够匹配，
                 那么接收盘只需要匹配本地配置就可以了，若匹配不成功，则可能配置未下，
                 丢弃报文

*****************************************************************************/
NBB_INT spm_bfd_fwdpacket_receive(IN NBB_BYTE *pBuf, IN NBB_ULONG ulMsgLenth, IN NBB_USHORT usSlotid)
{
    NBB_USHORT     usLoopi = 0;         /* 循环变量 */
    NBB_USHORT     usVrfid = 0;
    NBB_BYTE   ucSessionState = 0;     /* 握手报文的状态 */
    NBB_INT    iRet = ERROR;
    minm_header_t *pstMacInMac = NULL;
    NBB_LONG       lLenthTmp = 0;
    SPM_BFD_PACKET_ABSTRACT stPacketInfo;
    
    //NBB_TRC_ENTRY(("spm_bfd_fwdpacket_receive"));
    OS_ASSERT(NULL != pBuf);

    
    OS_MEMSET(&stPacketInfo, 0, sizeof(SPM_BFD_PACKET_ABSTRACT));

    lLenthTmp = ulMsgLenth - 28; /* 偏移掉前面的mac in mac */
    BFD_PACKETLEN_JUDGE(lLenthTmp);
    
    /* 解析DIP、SIP，获取VPNID，获取报文中的MYID */
    pstMacInMac = (minm_header_t *)pBuf;
    
    /* 判断端口类型 */
    switch (pstMacInMac->port_type)
    {
        case ETH_PORT_TYPE:
        case POS_PORT_TYPE:
        case LAG_PORT_TYPE:
        {
            usVrfid = ((pstMacInMac->port_instance.eth_port_bits.reserved[0] >> 16) & DRV_USHORT_INVALID);
            break;
        }

        case L3VE_PORT_TYPE:
        {
            usVrfid = ((pstMacInMac->port_instance.ve_port_bits.reserved[0] >> 16) & DRV_USHORT_INVALID);
            break;
        }

        case VRF_TUNNEL_PORT_TYPE:
        {
            usVrfid = pstMacInMac->port_instance.vrf_tunnel_port_bits.vrf_id;
            break;
        }

        case IPV6_IPV4_TUNNEL_PORT_TYPE:
        {
            usVrfid = pstMacInMac->port_instance.ipv6_ipv4_tunnel_port_bits.vrf_id;
            break;
        }

        case GRE_TUNNEL_PORT_TYPE:
        {
            usVrfid = pstMacInMac->port_instance.gre_tunnel_port_bits.vrf_id;
            break;
        }

        case  MPLS_TE_TUNNEL_PORT_TYPE:
        {
            usVrfid = pstMacInMac->port_instance.mpls_te_tunnel_port_bits.vrf_id;
            break;
        }
        default:
            break;
    }

    iRet = spm_bfd_packet_analysis((pBuf + 28), lLenthTmp, &stPacketInfo);
    if (SUCCESS != iRet)
    {
        iRet = SUCCESS;
        goto EXIT_LABEL;
    }
    
    ucSessionState = stPacketInfo.ucState;
        
    /* 直接在本盘的配置中匹配，不需要在全局配置中匹配 */
    if (BFD_IPV4 == stPacketInfo.ucIpType)                  /* 需要由CPU学习描述符且转发到其他盘的只有BFD FOR IP */
    {
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            /* 判断条件 */
            if ((usVrfid == g_stBfdlocal[usLoopi].usVrfid) 
                && (stPacketInfo.ulDip[0] == g_stBfdlocal[usLoopi].ulSip[3])
                && (stPacketInfo.ulSip[0] == g_stBfdlocal[usLoopi].ulDip[3]))
            {
                if ((0 == g_stBfdlocal[usLoopi].ulRemoteDiscr) 
                    || (stPacketInfo.ulMyDrim != g_stBfdlocal[usLoopi].ulRemoteDiscr))
                {
                    /* 此时需要清除FPGA中上次学习到的描述符 */
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x0,0);
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x1,0);
                    g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;
                    
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                        "The %u bfd learn discr from c3, my_id %ld, yourid %ld, "
                        "local ip %#x remote ip %#x, vrf %d\r\n", 
                        usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, stPacketInfo.ulMyDrim, 
                        stPacketInfo.ulDip[0], stPacketInfo.ulSip[0], usVrfid);
                    
                    /* 将学习到的描述符写入寄存器 */
                    /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                    /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                    
                    g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;

                    /* 跑状态机 */
                    spm_bfd_state_fsm(usLoopi, ucSessionState);
                }

                /* 记录每个盘接收的转发报文纪录 */
                spm_bfd_fwd_rxrecord(usSlotid ,g_stBfdlocal[usLoopi].ulLocalDiscr);
                
                break;  /* 找了就不再找了，节约时间，同时为后续判断是否找到提供依据 */
            }
        }
    }

    if (BFD_IPV6 == stPacketInfo.ucIpType)
    {
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            /* 判断条件 */
            if ((usVrfid == g_stBfdlocal[usLoopi].usVrfid) 
                && (stPacketInfo.ulDip[0] == g_stBfdlocal[usLoopi].ulSip[0])
                && (stPacketInfo.ulDip[1] == g_stBfdlocal[usLoopi].ulSip[1])
                && (stPacketInfo.ulDip[2] == g_stBfdlocal[usLoopi].ulSip[2])
                && (stPacketInfo.ulDip[3] == g_stBfdlocal[usLoopi].ulSip[3])
                && (stPacketInfo.ulSip[0] == g_stBfdlocal[usLoopi].ulDip[0])
                && (stPacketInfo.ulSip[1] == g_stBfdlocal[usLoopi].ulDip[1])
                && (stPacketInfo.ulSip[2] == g_stBfdlocal[usLoopi].ulDip[2])
                && (stPacketInfo.ulSip[3] == g_stBfdlocal[usLoopi].ulDip[3]))
            {
                if ((0 == g_stBfdlocal[usLoopi].ulRemoteDiscr) 
                    || (stPacketInfo.ulMyDrim != g_stBfdlocal[usLoopi].ulRemoteDiscr))
                {
                    /* 此时需要清除FPGA中上次学习到的描述符 */
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x0,0);
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x1,0);
                    g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;
                    
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                        "The %u bfd learn discr from c3, my_id %ld, yourid %ld, "
                        "local ip %lx.%lx.%lx.%lx remote ip %lx.%lx.%lx.%lx, vrf %d\r\n", 
                        usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, stPacketInfo.ulMyDrim, 
                        stPacketInfo.ulDip[0],
                        stPacketInfo.ulDip[1],
                        stPacketInfo.ulDip[2],                     
                        stPacketInfo.ulDip[3],
                        stPacketInfo.ulSip[0],
                        stPacketInfo.ulSip[1],
                        stPacketInfo.ulSip[2],
                        stPacketInfo.ulSip[3],
                        usVrfid);

                    g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;
                    
                    /* 将学习到的描述符写入寄存器 */
                    /* BFD发送模块寄存器偏移0xc000+2*N+0x0：Bit15-0：BFD报文远端会话标识符（Your Discriminator），高16位。 */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                    /* BFD发送模块寄存器偏移0xc000+2*N+0x1：Bit15-0：BFD报文远端会话标识符（Your Discriminator），低16位。 */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                    
                    g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
                    spm_bfd_state_fsm(usLoopi, ucSessionState);
                }

                spm_bfd_fwd_rxrecord(usSlotid ,g_stBfdlocal[usLoopi].ulLocalDiscr);
                
                break;  /* 找了就不再找了，节约时间，同时为后续判断是否找到提供依据 */
            }
        }
    }

    if (BFD_MAX_SESNUM == usLoopi)/* 判断条件 */
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
            "Fwd packet from slot %d, vrf is %d , dip is %lx.%lx.%lx.%lx is mismatch local record\r\n", 
            usSlotid, usVrfid, stPacketInfo.ulDip[0],
            stPacketInfo.ulDip[1], stPacketInfo.ulDip[2], stPacketInfo.ulDip[3]);
    }

    iRet = SUCCESS;
    EXIT_LABEL : //NBB_TRC_EXIT();
    return iRet;
    
}
    
/*****************************************************************************
 函 数 名  : spm_bfd_fwd_txrecord
 功能描述  : 每个盘转发的报文纪录
 输入参数  : IN NBB_USHORT usDesSlotid:目的槽位号
             IN NBB_ULONG ulDrim  :转发的报文的描述符，转发握手down 
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月10日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_fwd_txrecord(IN NBB_USHORT usDesSlotid, IN NBB_ULONG ulDrim)
{
    NBB_USHORT usRecordNum = 0; 

    /* 入参判断 */
    if ((usDesSlotid > SYS_TOTAL_SLOTNUM) || (0 == usDesSlotid))
    {
        return;
    }
    
    usRecordNum = (g_stFwdTxRec[usDesSlotid - 1].ulNum) % BFD_MAX_FWDRECORDNUM;/* 最多纪录128条 */
    g_stFwdTxRec[usDesSlotid - 1].ulNum++;
    g_stFwdTxRec[usDesSlotid - 1].aulDiscrimer[usRecordNum] = ulDrim;
    return;    
}

/*****************************************************************************
 函 数 名  : spm_bfd_fwd_rxrecord
 功能描述  : 每个盘接收的报文纪录
 输入参数  : IN NBB_USHORT usSourceSlotid :源槽位号 
             IN NBB_ULONG ulDrim      :描述符
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年9月10日
    作    者   : limingming
    审 核 人   : aa
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_bfd_fwd_rxrecord(IN NBB_USHORT usSourceSlotid, IN NBB_ULONG ulDrim)
{
    NBB_USHORT usRecordNum = 0;
    
    /* 入参判断 */
    if ((usSourceSlotid > SYS_TOTAL_SLOTNUM) || (0 == usSourceSlotid))
    {
        return;
    }
    
    usRecordNum = (g_stFwdRxRec[usSourceSlotid - 1].ulNum) % BFD_MAX_FWDRECORDNUM;/* 最多纪录128条 */
    g_stFwdRxRec[usSourceSlotid - 1].ulNum++;
    g_stFwdRxRec[usSourceSlotid - 1].aulDiscrimer[usRecordNum] = ulDrim;
    return;    
}


/*****************************************************************************
 函 数 名  : spm_proc_bfd_packet
 功能描述  : CPU处理C3送来的BFD报文，报文或转发或处理后释放内存(已弃用)
 输入参数  : IN SPM_THREAD_SOCKET_CB *thread_cb  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年2月1日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_proc_bfd_packet (IN NBB_BYTE *buf,
                              IN NBB_ULONG len
                              NBB_CCXT_T NBB_CXT)
{
    NBB_BYTE *pBuf = NULL;  /* 指向报文的指针 */  
    NBB_ULONG ulMsgLength = 0;
    
    /* 入参检查 */
    if (NULL == buf)
    {
        return;
    }

    ulMsgLength = len - SPM_TCP_RP_HEAD_DATA_SIZE;
    if (ulMsgLength < 94)   /* 接收到的BFD报文，长度至少为28字节macinmac+ipv4bfd66字节(不带vlantag的) */
    {
        NBB_EXCEPTION((PCT_SPM | 26, 1, "lu", len));
        return;
    }

    pBuf = buf + SPM_TCP_RP_HEAD_DATA_SIZE; /* 偏移到报文净荷处 */
    spm_bfd_packet_receive(pBuf, ulMsgLength);
    return;
    
}

/*****************************************************************************
 函 数 名  : spm_bfd_packet_analysis
 功能描述  : 解析BFD报文函数
 输入参数  : IN NBB_BYTE *pBuf : 以太网报文(上cpu的bfd报文，去掉前28字节的mac in mac)                                             
             IN NBB_LONG lMsgLenth : 报文总长度                                       
 输出参数  : OUT SPM_BFD_PACKET_ABSTRACT *pstPacketInfo 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年2月28日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

  2.日    期   : 2015年6月18日
    作    者   : gaos
    修改内容   : 添加调试信息
*****************************************************************************/
NBB_INT spm_bfd_packet_analysis(
    IN NBB_BYTE *pBuf, 
    IN NBB_LONG lMsgLenth, 
    OUT SPM_BFD_PACKET_ABSTRACT *pstPacketInfo)
{
    NBB_LONG       lLenthTmp = 0;
    NBB_ULONG      ulOffset  = 0;       /* 从报文头到IP的偏移长度 */
    NBB_USHORT     usEthtype = 0;       /* 判断是否带vlantag */
    SPM_BFD_MPLSLABEL *pstMplsLabel = NULL;
    NBB_BYTE   ucIpType;
        
    OS_ASSERT(NULL != pBuf);
    OS_ASSERT(NULL != pstPacketInfo);

    /* 偏移掉12字节的mac */
    lLenthTmp = lMsgLenth - 12;
    BFD_PACKETLEN_JUDGE(lLenthTmp);
    ulOffset = 12;

    usEthtype = *(NBB_USHORT *)(pBuf + ulOffset);
    if (0x0800 == usEthtype)
    {
        ulOffset = ulOffset + 2;
        pstPacketInfo->ucIpType = BFD_IPV4;
    }
    else if (0x86dd == usEthtype)
    {
        ulOffset = ulOffset + 2;
        pstPacketInfo->ucIpType = BFD_IPV6;
    }
    else if (0x8100 == usEthtype)/* vlan情况 */
    {
        lLenthTmp = lLenthTmp - 4;
        BFD_PACKETLEN_JUDGE(lLenthTmp);
        ulOffset = ulOffset + 4;
        usEthtype = *(NBB_USHORT *)(pBuf + ulOffset);
        if (0x8100 == usEthtype)/* QinQ情况 */
        {
            lLenthTmp = lLenthTmp - 4;
            BFD_PACKETLEN_JUDGE(lLenthTmp);
            ulOffset = ulOffset + 4;
            usEthtype = *(NBB_USHORT *)(pBuf + ulOffset);
            if (0x8847 == usEthtype)
            {
                ulOffset = ulOffset + 2;
                pstMplsLabel = (SPM_BFD_MPLSLABEL *)(pBuf + ulOffset);
                if (1 != pstMplsLabel->ulStack)
                {
                    return ERROR;
                }
                ulOffset = ulOffset + 4;
            }
            else if (0x0800 == usEthtype)
            {
                ulOffset = ulOffset + 2;
                pstPacketInfo->ucIpType = BFD_IPV4;
            }
            else if (0x86dd == usEthtype)
            {
                ulOffset = ulOffset + 2;
                pstPacketInfo->ucIpType = BFD_IPV6;
            }
            else
            {
                if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
                {
                   printf("%s:vlan QinQ usEthtype = 0x%4x error!\n",__FUNCTION__,usEthtype); 
                   BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                    "%s:vlan QinQ usEthtype = 0x%4x error!\n",__FUNCTION__,usEthtype);
                }            
            }
        }
        else if (0x8847 == usEthtype)
        {
            ulOffset = ulOffset + 2;
            pstMplsLabel = (SPM_BFD_MPLSLABEL *)(pBuf + ulOffset);
            if (1 != pstMplsLabel->ulStack)
            {
                if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
                {
                   printf("%s:vlan pstMplsLabel->ulStack = %d error!\n",__FUNCTION__,pstMplsLabel->ulStack); 
                   BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                    "%s:vlan pstMplsLabel->ulStack = %d error!\n",__FUNCTION__,pstMplsLabel->ulStack);
                }             
                return ERROR;
            }
            ulOffset = ulOffset + 4;
            ucIpType = *(NBB_BYTE *)(pBuf + ulOffset);
            ucIpType = (ucIpType >> 4);
            if (4 == ucIpType)
            {
                pstPacketInfo->ucIpType = BFD_IPV4;
            }
            else if (6 == ucIpType)
            {
                pstPacketInfo->ucIpType = BFD_IPV6;
            }
            else 
            {
                if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
                {
                   printf("%s:vlan mpls ucIpType = %d error!\n",__FUNCTION__,ucIpType); 
                   BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                    "%s:vlan mpls ucIpType = %d error!\n",__FUNCTION__,ucIpType);
                }             
                return ERROR;
            }
        }
        else if (0x0800 == usEthtype)
        {
            ulOffset = ulOffset + 2;
            pstPacketInfo->ucIpType = BFD_IPV4;
        }
        else if (0x86dd == usEthtype)
        {
            ulOffset = ulOffset + 2;
            pstPacketInfo->ucIpType = BFD_IPV6;
        }
        else
        {
            if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
            {
               printf("%s:vlan usEthtype = 0x%x error!\n",__FUNCTION__,usEthtype); 
               BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                "%s:vlan usEthtype  = 0x%x error!\n",__FUNCTION__,usEthtype);
            }              
        }
    }
    else if (0x8847 == usEthtype)
    {
        ulOffset = ulOffset + 2;
        pstMplsLabel = (SPM_BFD_MPLSLABEL *)(pBuf + ulOffset);
        if (1 != pstMplsLabel->ulStack)
        {
            if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
            {
               printf("%s: mpls pstMplsLabel->ulStack = %d error!\n",__FUNCTION__,pstMplsLabel->ulStack); 
               BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                "%s: mpls pstMplsLabel->ulStack = %d error!\n",__FUNCTION__,pstMplsLabel->ulStack);
            }         
            return ERROR;
        }
        ulOffset = ulOffset + 4;
        
        ucIpType = *(NBB_BYTE *)(pBuf + ulOffset);
        ucIpType = (ucIpType >> 4);
        if (4 == ucIpType)
        {
            pstPacketInfo->ucIpType = BFD_IPV4;
        }
        else if (6 == ucIpType)
        {
            pstPacketInfo->ucIpType = BFD_IPV6;
        }
        else
        {
            if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
            {
               printf("%s: mpls ucIpType = %d error!\n",__FUNCTION__,ucIpType); 
               BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                "%s: mpls ucIpType = %d error!\n",__FUNCTION__,ucIpType);
            }        
            return ERROR;
        }
        
    }
    else
    {
        if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
        {
           printf("%s: usEthtype = 0x%4x error!\n",__FUNCTION__,usEthtype); 
           BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
            "%s: usEthtype = 0x%4x error!\n",__FUNCTION__,usEthtype);
        }     
        return ERROR;
    }

    if (BFD_IPV4 == pstPacketInfo->ucIpType)
    {
        pstPacketInfo->ulDip[0] = *(NBB_ULONG *)(pBuf + ulOffset + 16);
        pstPacketInfo->ulSip[0] = *(NBB_ULONG *)(pBuf + ulOffset + 12);
        pstPacketInfo->ulMyDrim = *(NBB_ULONG *)(pBuf + ulOffset + 32);
        pstPacketInfo->ucState = *(NBB_BYTE *)(pBuf + ulOffset + 29);
        pstPacketInfo->ucState = ((pstPacketInfo->ucState >> 6) & 0x3);
    }

    if (BFD_IPV6 == pstPacketInfo->ucIpType)
    {
        pstPacketInfo->ulSip[0] = *(NBB_ULONG *)(pBuf + ulOffset + 8);
        pstPacketInfo->ulSip[1] = *(NBB_ULONG *)(pBuf + ulOffset + 12);
        pstPacketInfo->ulSip[2] = *(NBB_ULONG *)(pBuf + ulOffset + 16);
        pstPacketInfo->ulSip[3] = *(NBB_ULONG *)(pBuf + ulOffset + 20);
        pstPacketInfo->ulDip[0] = *(NBB_ULONG *)(pBuf + ulOffset + 24);
        pstPacketInfo->ulDip[1] = *(NBB_ULONG *)(pBuf + ulOffset + 28);
        pstPacketInfo->ulDip[2] = *(NBB_ULONG *)(pBuf + ulOffset + 32);
        pstPacketInfo->ulDip[3] = *(NBB_ULONG *)(pBuf + ulOffset + 36);
        pstPacketInfo->ulMyDrim = *(NBB_ULONG *)(pBuf + ulOffset + 52);
        pstPacketInfo->ucState = *(NBB_BYTE *)(pBuf + ulOffset + 49);
        pstPacketInfo->ucState = ((pstPacketInfo->ucState >> 6) & 0x3);
    }
    return SUCCESS;
    
}

/*****************************************************************************
 函 数 名  : spm_search_bfdId_by_vckey
 功能描述  : 根据VCKEY，查询对应的BFDID
 输入参数  : IN ATG_DCI_VC_KEY *pstVCKey : vc key 
 输出参数  : OUT NBB_USHORT *pulBfdId : BFD ID
 返 回 值  : SUCCESS查询成功，可使用返回的id值
             ERROR查询不成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_search_bfdId_by_vckey(IN ATG_DCI_VC_KEY *pstVCKey, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;

    /* 非nbase线程使用nbase内不使用NBB_CXT上下文的API时，骗过编译器 */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
    
    /* 入参检查 */
    OS_ASSERT(NULL != pstVCKey);
    OS_ASSERT(NULL != pulBfdId);
    for (pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIRST(g_stBfdGlbTreeByDis);
            pstBfdCfg != NULL;
            pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_NEXT(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node))
    {
        if ((pstBfdCfg->stVcCfg.peer_ip == pstVCKey->peer_ip)
            && (pstBfdCfg->stVcCfg.vc_id == pstVCKey->vc_id)
            && (pstBfdCfg->stVcCfg.vc_type == pstVCKey->vc_type))
        {
            *pulBfdId = pstBfdCfg->ulDiscrimer;
            return SUCCESS;
        }
    }

    return ERROR;
}

/*****************************************************************************
 函 数 名  : spm_search_bfdId_by_ldpkey
 功能描述  : 根据VCKEY，查询对应的BFDID
 输入参数  : IN ATG_DCI_BFD_LDP_LSP_DATA *pstldpKey : ldp key 
 输出参数  : OUT NBB_USHORT *pulBfdId : BFD ID
 返 回 值  : SUCCESS查询成功，可使用返回的id值
             ERROR查询不成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年6月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_search_bfdId_by_ldpkey(IN ATG_DCI_BFD_LDP_LSP_DATA *pstldpKey, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;

    /* 非nbase线程使用nbase内不使用NBB_CXT上下文的API时，骗过编译器 */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
    
    /* 入参检查 */
    OS_ASSERT(NULL != pstldpKey);
    OS_ASSERT(NULL != pulBfdId);
    for (pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIRST(g_stBfdGlbTreeByDis);
            pstBfdCfg != NULL;
            pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_NEXT(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node))
    {
        if ((pstBfdCfg->stLdpCfg.fec_prefix == pstldpKey->fec_prefix)
            && (pstBfdCfg->stLdpCfg.fec_prefix_len == pstldpKey->fec_prefix_len)
            && (pstBfdCfg->stLdpCfg.vrf_id == pstldpKey->vrf_id))
        {
            *pulBfdId = pstBfdCfg->ulDiscrimer;
            return SUCCESS;
        }
    }

    return ERROR;
}


/*****************************************************************************
 函 数 名  : spm_search_bfdId_by_rsvpkey
 功能描述  : 根据VCKEY，查询对应的BFDID
 输入参数  : IN ATG_DCI_BFD_RSVP_LSP_DATA *pstrsvpKey : rsvp key 
 输出参数  : OUT NBB_USHORT *pulBfdId : BFD ID
 返 回 值  : SUCCESS查询成功，可使用返回的id值
           ERROR查询不成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
 1.日    期   : 2013年6月20日
  作    者   : limingming
  审 核 人   : #
  修改内容   : 新生成函数
 
*****************************************************************************/
NBB_INT spm_search_bfdId_by_rsvpkey(IN ATG_DCI_BFD_RSVP_LSP_DATA *pstrsvpKey, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;

    /* 非nbase线程使用nbase内不使用NBB_CXT上下文的API时，骗过编译器 */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
    
    /* 入参检查 */
    OS_ASSERT(NULL != pstrsvpKey);
    OS_ASSERT(NULL != pulBfdId);
    for (pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIRST(g_stBfdGlbTreeByDis);
            pstBfdCfg != NULL;
            pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_NEXT(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node))
    {
        if ((pstBfdCfg->stRsvpCfg.egress == pstrsvpKey->egress)
            && (pstBfdCfg->stRsvpCfg.ingress == pstrsvpKey->ingress)
            && (pstBfdCfg->stRsvpCfg.tunnel_id == pstrsvpKey->tunnel_id)
            && (pstBfdCfg->stRsvpCfg.lsp_id == pstrsvpKey->lsp_id))
        {
            *pulBfdId = pstBfdCfg->ulDiscrimer;
            return SUCCESS;
        }
    }

    return ERROR;
}


/*****************************************************************************
 函 数 名  : spm_search_bfdId_by_rsvpkey
 功能描述  : 根据IPKEY，查询对应的BFDID
 输入参数  : IN VRFUROUTE_KEY *pstipKey : ip key 
 输出参数  : OUT NBB_USHORT *pulBfdId : BFD ID
 返 回 值  : SUCCESS查询成功，可使用返回的id值
           ERROR查询不成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
 1.日    期   : 2013年6月20日
  作    者   : limingming
  审 核 人   : #
  修改内容   : 新生成函数
 
*****************************************************************************/
NBB_INT spm_search_bfdid_by_ipkey(IN VRFUROUTE_KEY * pstipKey, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;
    NBB_ULONG ulDipTmp1 = 0;
    NBB_ULONG ulDipTmp2 = 0;
    NBB_BYTE  ucMaskLen = 0;

    NBB_INT iRet = ERROR;

    /* 非nbase线程使用nbase内不使用NBB_CXT上下文的API时，骗过编译器 */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
        
    /* 入参检查 */
    #if 0
    OS_ASSERT(NULL != pstipKey);
    OS_ASSERT(NULL != pulBfdId);
    #else
    if(NULL == pstipKey)
    {
        iRet =  ERROR;
        goto EXIT_LABEL;
    }
    
    if(NULL == pulBfdId)
    {
        iRet =  ERROR;
        goto EXIT_LABEL;
    }
    #endif

    ucMaskLen = pstipKey->mask;

    if (0 == pstipKey->addrtype) /* IPV4类型 */
    {
        if (ucMaskLen > 32)
        {
            iRet =  ERROR;
            goto EXIT_LABEL;
        }

        ulDipTmp2 = (pstipKey->dip[3] >> (32 - ucMaskLen));
        /* IP地址为0,表示传入参数出错 */
        if ( 0 == ulDipTmp2 ) /* IP地址为0 */
        {
            iRet =  ERROR;
            goto EXIT_LABEL;
        }
        
        for (pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIRST(g_stBfdGlbTreeByDis);
            pstBfdCfg != NULL;
            pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_NEXT(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node))
        {
            if (pstBfdCfg->usVrfid == pstipKey->vrfid)/* vrfid */
            {
                ulDipTmp1 = (pstBfdCfg->ulDip[3] >> (32 - ucMaskLen));
                /* ulDipTmp2赋值提取到for循环外，提高效率 */
                if (ulDipTmp1 == ulDipTmp2)
                {
                    *pulBfdId = pstBfdCfg->ulDiscrimer;
                    iRet =  SUCCESS;
                    goto EXIT_LABEL;
                }
            }
        }
    }
    else if (1 == pstipKey->addrtype) /* IPV6类型 */
    {
        if (ucMaskLen > 128)
        {
            iRet =  ERROR;
            goto EXIT_LABEL;
        }

        /* IP地址为0,表示传入参数出错 */
        if (ucMaskLen >= 96)
        {
            ulDipTmp2 = (pstipKey->dip[3] >> (128 - ucMaskLen));
            if ((0 == pstipKey->dip[0])
                && (0 == pstipKey->dip[1])
                && (0 == pstipKey->dip[2])
                && (0 == ulDipTmp2))
            {
                iRet =  ERROR;
                goto EXIT_LABEL;
            }
        }
        else if(64 <= ucMaskLen)
        {
            ulDipTmp2 = (pstipKey->dip[2] >> (96 - ucMaskLen));
            if ((0 == pstipKey->dip[0])
                && (0 == pstipKey->dip[1])
                && (0 == ulDipTmp2))
            {
                iRet =  ERROR;
                goto EXIT_LABEL;
            }
        }
        else if (32 <= ucMaskLen)
        {
            ulDipTmp2 = (pstipKey->dip[1] >> (64 - ucMaskLen));
            if ((0 == pstipKey->dip[0])
                && (0 == ulDipTmp2))
            {
                iRet =  ERROR;
                goto EXIT_LABEL;
            }
        }
        else    /* ucMaskLen >= 0 */
        {
            ulDipTmp2 = (pstipKey->dip[0] >> (32 - ucMaskLen));
            if (0 == ulDipTmp2)
            {
                iRet =  ERROR;
                goto EXIT_LABEL;
            }
        }

        /* 根据掩码位数得到最后4个字节内容 */
        
        for (pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIRST(g_stBfdGlbTreeByDis);
            pstBfdCfg != NULL;
            pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_NEXT(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node))
        {
            if (pstBfdCfg->usVrfid == pstipKey->vrfid)
            {
                if (ucMaskLen >= 96)
                {
                    if ((pstBfdCfg->ulDip[0] == pstipKey->dip[0])
                        && (pstBfdCfg->ulDip[1] == pstipKey->dip[1])
                        && (pstBfdCfg->ulDip[2] == pstipKey->dip[2]))
                    {
                        ulDipTmp1 = (pstBfdCfg->ulDip[3] >> (128 - ucMaskLen));
                        /* ulDipTmp2赋值提取到for循环外，提高效率 */
                        if (ulDipTmp1 == ulDipTmp2)
                        {
                            *pulBfdId = pstBfdCfg->ulDiscrimer;
                            iRet =  SUCCESS;
                            goto EXIT_LABEL;
                        }
                    }
                }
                else if(64 <= ucMaskLen)
                {
                    if ((pstBfdCfg->ulDip[0] == pstipKey->dip[0])
                        && (pstBfdCfg->ulDip[1] == pstipKey->dip[1]))
                    {
                        ulDipTmp1 = (pstBfdCfg->ulDip[2] >> (96 - ucMaskLen));
                        /* ulDipTmp2赋值提取到for循环外，提高效率 */
                        if (ulDipTmp1 == ulDipTmp2)
                        {
                            *pulBfdId = pstBfdCfg->ulDiscrimer;
                            iRet =  SUCCESS;
                            goto EXIT_LABEL;
                        }
                    }
                }
                else if (32 <= ucMaskLen)
                {
                    if (pstBfdCfg->ulDip[0] == pstipKey->dip[0])
                    {
                        ulDipTmp1 = (pstBfdCfg->ulDip[1] >> (64 - ucMaskLen));
                        /* ulDipTmp2赋值提取到for循环外，提高效率 */
                        if (ulDipTmp1 == ulDipTmp2)
                        {
                            *pulBfdId = pstBfdCfg->ulDiscrimer;
                            iRet =  SUCCESS;
                            goto EXIT_LABEL;
                        }
                    }
                }
                else    /* ucMaskLen >= 0 */
                {
                    ulDipTmp1 = (pstBfdCfg->ulDip[0] >> (32 - ucMaskLen));
                    /* ulDipTmp2赋值提取到for循环外，提供效率 */
                    if (ulDipTmp1 == ulDipTmp2)
                    {
                        *pulBfdId = pstBfdCfg->ulDiscrimer;
                        iRet =  SUCCESS;
                        goto EXIT_LABEL;
                    }
                }
            }
        }
    }

    iRet = ERROR;
    
    EXIT_LABEL :
    return iRet;
}

/*****************************************************************************
 函 数 名  : spm_search_bfdid_by_portkey
 功能描述  : 根据PORTKEY，查询对应的BFDID
 输入参数  : IN VRFUROUTE_KEY *pstrsvpKey : port key 
 输出参数  : OUT NBB_USHORT *pulBfdId : BFD ID
 返 回 值  : SUCCESS查询成功，可使用返回的id值
           ERROR查询不成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
 1.日    期   : 2013年6月20日
  作    者   : limingming
  审 核 人   : #
  修改内容   : 新生成函数
 
*****************************************************************************/
NBB_INT spm_search_bfdid_by_portkey(IN NBB_ULONG *pulPortIndex, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;

    /* 非nbase线程使用nbase内不使用NBB_CXT上下文的API时，骗过编译器 */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
    
    /* 入参检查 */
    OS_ASSERT(NULL != pulPortIndex);
    OS_ASSERT(NULL != pulBfdId);

    for (pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIRST(g_stBfdGlbTreeByDis);
            pstBfdCfg != NULL;
            pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_NEXT(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node))
    {
        if (*pulPortIndex == pstBfdCfg->ulPortIndex)
        {
            *pulBfdId = pstBfdCfg->ulDiscrimer;
            return SUCCESS;
        }
    }
    return ERROR;
}

/*****************************************************************************
 * 函 数 名  : spm_bfd_notify_aps_del
 * 负 责 人  : 蒋博,jiangbo
 * 创建日期  : 2014年5月28日
 * 函数功能  : 通知保护模块删除关联
 * 输入参数  : NBB_ULONG ulDiscrimer      本地描述符
               NBB_BYTE ucFramtype        BFD类型
               NBB_CCXT_T NBB_CXT         NBASE多线程参数
 * 输出参数  : 无
 * 返 回 值  : NBB_INT  错误码
 * 调用关系  : 
 * 其    它  : 

*****************************************************************************/
NBB_INT spm_bfd_notify_aps_del(NBB_ULONG ulDiscrimer, NBB_BYTE ucFramtype NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    
    NBB_TRC_ENTRY(("spm_bfd_notify_aps_del"));

    if (BFD_CFG_IP == ucFramtype)
    {
        iRet = spm_del_bfd_cfg(VRF_ROUTE_BFD_CHANGE, ulDiscrimer);
    }
    if (BFD_CFG_PW == ucFramtype)
    {
        iRet = spm_del_bfd_cfg(PW_BFD_CHANGE, ulDiscrimer);
    }
    if (BFD_CFG_LSP == ucFramtype)
    {
        iRet = spm_del_bfd_cfg(LDP_LSP_BFD_CHANGE, ulDiscrimer);/* 保护模块需要查询ldp和rsvp的会话 */
    }
    if (BFD_CFG_Tunnel == ucFramtype)
    {
        iRet = spm_del_bfd_cfg(RSVP_LSP_BFD_CHANGE, ulDiscrimer);
    }
    
EXIT_LABEL: NBB_TRC_EXIT();
    return iRet;
}

NBB_INT spm_bfd_tree_set_data(IN NBB_USHORT index,
    IN NBB_ULONG ulDrim,
    OUT SPM_OAMTREE_NODE * pstBfdTreeNode)
{
    NBB_INT iRet = ERROR;
    
    if (NULL == pstBfdTreeNode)
    {
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));

    pstBfdTreeNode->usLspNo = index;
    pstBfdTreeNode->ulTunnelId = ulDrim;
    
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}


/*****************************************************************************
 函 数 名  : spm_bfd_tree_get_fpga_data
 功能描述  : BFD 会话二叉树查找表寄存器bit位赋值
 输入参数  : IN SPM_OAMTREE_NODE* oamTreeNode     
             OUT SPM_FPGATREE_DATA *fpgaTreeDate  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年5月28日
    作    者   : gaos
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_bfd_tree_get_fpga_data(IN SPM_OAMTREE_NODE* oamTreeNode,
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

    /* 8.28.1	会话二叉树查找表寄存器定义 基地址 0xea060000+0x4000*2， N= 0~7feH */
    /*
    比特[63：48]：条目索引号；
    比特[35:24]：MYID高12位比特；
    比特[19:0]：MYID低20位比特。
    */
    fpgaTreeDate->usData.bfdTable.index = oamTreeNode->usLspNo;
    fpgaTreeDate->usData.bfdTable.myId_31to28b = (oamTreeNode->ulTunnelId >> 28) & 0xf; //高12bit中的高4bit
    fpgaTreeDate->usData.bfdTable.myId_27to20b = (oamTreeNode->ulTunnelId >> 20) & 0xff;//高12bit中的低8bit
    fpgaTreeDate->usData.bfdTable.myId_19to0b = oamTreeNode->ulTunnelId & 0xfffff;

    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

NBB_INT spm_fwd_tree_set_data(IN NBB_USHORT index,
    IN NBB_ULONG ulDrim,
    OUT SPM_OAMTREE_NODE * pstFwdTreeNode)
{
    NBB_INT iRet = ERROR;
    
    if (NULL == pstFwdTreeNode)
    {
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstFwdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));

    pstFwdTreeNode->usLspNo = index;
    pstFwdTreeNode->ulTunnelId = ulDrim;
    
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

NBB_INT spm_fwd_tree_get_fpga_data(IN SPM_OAMTREE_NODE* oamTreeNode,
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
    fpgaTreeDate->usData.fwdTable.index = oamTreeNode->usLspNo;
    fpgaTreeDate->usData.fwdTable.myId_31to28b = (oamTreeNode->ulTunnelId >> 28) & 0xf;
    fpgaTreeDate->usData.fwdTable.myId_27to20b = (oamTreeNode->ulTunnelId >> 20) & 0xff;
    fpgaTreeDate->usData.fwdTable.myId_19to0b = oamTreeNode->ulTunnelId & 0xfffff;

    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}

NBB_INT spm_nni_tree_set_data(IN NBB_USHORT index,
    IN NBB_ULONG ulPeerIp,
    IN NBB_ULONG ulPwLabel,
    OUT SPM_OAMTREE_NODE * pstNniTreeNode)
{
    NBB_INT iRet = ERROR;
    
    if (NULL == pstNniTreeNode)
    {
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(pstNniTreeNode, 0, sizeof(SPM_OAMTREE_NODE));

    pstNniTreeNode->usLspNo = index;
    pstNniTreeNode->ulTunnelId = ulPeerIp;
    pstNniTreeNode->ulPwId = ulPwLabel & 0xfffff;
    
    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}


NBB_INT spm_nni_tree_get_fpga_data(IN SPM_OAMTREE_NODE* oamTreeNode,
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
    fpgaTreeDate->usData.nniTable.index = oamTreeNode->usLspNo;
    fpgaTreeDate->usData.nniTable.ipv4_31to12b = (oamTreeNode->ulTunnelId >> 12) & 0xfffff;
    fpgaTreeDate->usData.nniTable.ipv4_11to0b = oamTreeNode->ulTunnelId & 0xfff;
    fpgaTreeDate->usData.nniTable.pwLabel = oamTreeNode->ulPwId;

    iRet = SUCCESS;
EXIT_LABEL:
    return iRet;
}


NBB_INT spm_bfd_hardw_getslot(OUT NBB_BYTE * ucSlot)
{
    NBB_INT iRet = SUCCESS;

    if(NULL == ucSlot)
    {
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    if(0 == g_ucLocalSlot)
    {
        iRet = spm_hardw_getslot(ucSlot);
        if (SUCCESS != iRet)
        {
            goto EXIT_LABEL;
        }

        g_ucLocalSlot = *ucSlot;
    }
    else
    {
        *ucSlot = g_ucLocalSlot;
    }
    
EXIT_LABEL:
    return iRet;
}

