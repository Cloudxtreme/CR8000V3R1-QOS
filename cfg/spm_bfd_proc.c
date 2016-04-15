/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_bfd_proc.c
  �� �� ��   : ����
  ��    ��   : limingming
  ��������   : 2012��9��24��
  ����޸�   :
  ��������   : BFD���ô���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��24��
    ��    ��   : limingming
    �޸�����   : �����ļ�

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
#include <c3api.h> 
#include "bmuInclude.h"
BMU_WDOG_ID g_stBfdWdog; /* BFD��ʱ�� */ 
BMU_MSG_Q_ID g_stBfdMsgQID;
extern SPM_BOARD_INF    g_stHardwBoardInf;

/* ������Ϣ��ӡ���� */
extern BFD_DEBUGSHOW g_enBfdDebug;

/* �����ñ�� */
extern NBB_BYTE g_ucCleanCfgCheck;

/* fwdtable ��ҳд����Ĵ��� */
extern NBB_UINT g_uiFwdTableCount;

/* ����fpgaת����������д�� */
extern BMU_RWLOCK_ID g_build_fpga_fwd_lock;

/*****************************************************************************
 �� �� ��  : spm_bfd_init
 ��������  : BFDģ���ʼ������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��27��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��9��9��
    ��    ��   : gaos
    �޸�����   : ���������־�ļ��Ĵ�С
*****************************************************************************/
NBB_INT spm_bfd_init(NBB_CXT_T NBB_CXT)
{
    NBB_INT   iRet = ERROR;
    NBB_ULONG ulLoopi = 0;
    NBB_ULONG ulLoopj = 0;
    
    NBB_TRC_ENTRY(("spm_bfd_init"));
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "call spm_bfd_init\r\n");
    bmu_set_log_size(SPM_BFD_LOG_DIR, 1024 * 1024 * 20);//20M
    
    /* ��ʼ�� */
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

    /* �ж��ڴ� */
    if (NULL == g_pulBfdTreebase)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Malloc error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Malloc error\r\n");
        goto EXIT_LABEL;
    }
    NBB_MEMSET(g_pulBfdTreebase, 0, sizeof(NBB_ULONG)*g_usBfdTreeLevel);

    g_ulSynIdInfoLth = sizeof(SPM_BFD_SYN_BFD_ID) + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD);
    g_pucSynIdInfo = (NBB_BYTE *)NBB_MM_ALLOC(g_ulSynIdInfoLth, NBB_NORETRY_ACT, MEM_SPM_BFD_PUC_SYN_ID_INFO_CB);

    if (NULL == g_pucSynIdInfo)/* ����ʧ�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Syn Id, malloc failed!"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Syn Id, malloc failed!\r\n");
        goto EXIT_LABEL;
    }
    
    /* ��������ʼ�� */    
    iRet = spm_oamtree_init(g_usBfdTreeNodeMaxnum, spm_bfd_tree_get_fpga_data, &g_stTreeBfdNodeSet);

    /* �ж�ִ�н�� */
    if (SUCCESS != iRet)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Bfd node set error, nodenum = %d", g_usBfdTreeNodeMaxnum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Bfd node set error, nodenum = %d\r\n", g_usBfdTreeNodeMaxnum);
        goto EXIT_LABEL;
    }

    
    /* ��ʼ��BFD���ô洢���� */
    for (ulLoopi = 0; ulLoopi < BFD_MAX_SESNUM; ulLoopi++)/* ��ʼ�� */
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
    
    /* ��ʼ��ȫ�����ñ��� �����ṹ��ʼ�� */
    AVLL_INIT_TREE(g_stBfdGlbTreeByDis, 
        compare_ulong,
        NBB_OFFSETOF(SPM_BFD_GLOBAL, ulDiscrimer),
        NBB_OFFSETOF(SPM_BFD_GLOBAL, spm_bfd_node));

    AVLL_INIT_TREE(g_stBfdGlbTreeByIp,
        spm_bfd_key_compare,
        NBB_OFFSETOF(SPM_BFD_IP_GLOBAL, stBfdGlbKey),
        NBB_OFFSETOF(SPM_BFD_IP_GLOBAL, spm_bfdip_node));
    /* add begin by limingming, 2013/5/3, Mantis��:HSDIPV6BFD, ԭ��:ʵ��IPV6BFD */
    AVLL_INIT_TREE(g_stBfdGlbTreeByIpv6,
        spm_bfd_ipv6key_compare,
        NBB_OFFSETOF(SPM_BFD_IPV6_GLOBAL, stBfdGlbKey),
        NBB_OFFSETOF(SPM_BFD_IPV6_GLOBAL, spm_bfdipv6_node));
    /* add end by limingming, 2013/5/3 */

    AVLL_INIT_TREE(g_stFwdGlbTreeByDis, 
        compare_ulong,
        NBB_OFFSETOF(SPM_FWD_GLOBAL, ulDiscrimer),
        NBB_OFFSETOF(SPM_FWD_GLOBAL, spm_fwd_node));

    /* ���Ի�ʧ�ܸ澯�� */
    AVLL_INIT_TREE(g_stAlarmFailTree,
        compare_ulong,
        NBB_OFFSETOF(BFD_ALARM_FAIL_INFO, ulBfdId),
        NBB_OFFSETOF(BFD_ALARM_FAIL_INFO, alarm_fail_node));
    
    /* ��ʼ��������Ϣģ�� */
    spm_bfd_init_dbg(NBB_CXT);

    /* ��Ϊҵ���̣����ʼ��ת���� */
    iRet = spm_bfd_init_fwdtable(NBB_CXT);

    /* �ж�ִ�н�� */
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
    if (NULL == g_pucAlarmInfo)/* ����ʧ�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Alarm info, malloc failed!"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Alarm info, malloc failed!\r\n");
        goto EXIT_LABEL;
    }

    g_ucFirstNpBoard = 0;
    g_ucMainEmuId = 0xff;
    NBB_MEMSET(g_astBoardState, 0, 10 * sizeof(BFD_BOARD_INFO));

    /* ��ʼ����д�� */
    g_build_fpga_fwd_lock = BmuRWLockCreate(BMU_SEM_Q_FIFO);
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_clean_cfg_fwdtree
 ��������  : ɾ��ת�����ã�������ʱ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��9��15��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

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
            /* ɾ��ȫ��ת������ */
            AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
            OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by ����,jiangbo, 20131113, Mantis��:��,
                                                                ԭ��:��Ϊ����nbase�������Ҳ����䡢�ͷ������
                                                                �棬����ʹ��NBB_MM_FREE����ͬʱ���ܱ�verify */
            pstFwdInfo = NULL;
      
        }

    /* ���ض�����ȫ����ΪĬ��ֵ */
    iRet = spm_oamtree_init(g_usFwdTreeNodeMaxnum, spm_fwd_tree_get_fpga_data, &g_stTreeFwdNodeSet);
    if (SUCCESS != iRet)/* �ж�ִ�н�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Fwd node set error, nodenum = %d", g_usFwdTreeNodeMaxnum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd node set error, nodenum = %d\r\n", g_usFwdTreeNodeMaxnum);
        goto EXIT_LABEL;
    }

    /* ���fpga���ͼĴ���������ת��������Ϊ��Чֵ */
    for (usLoopi = 0; usLoopi < BFD_MAX_FWDNUM; usLoopi++)
    {
        /* ���Ự��Ϣ��Ϊ��Чֵ */
        BFD_FWD_INIT(usLoopi);
        
        /* ���FPGA��Ӧ�ķ��ͼĴ������� */
        spm_bfd_clear_fwdfpgareg(usLoopi);
    }
    
    /* �ؽ�fpgaת���Ự������ */   
    iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
        g_pulFwdTreebase, OAMFWDFPAGBASE);
    
    /* ��ҳд���� */
    spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);
    
    
    iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
        g_pulFwdTreebase, OAMFWDFPAGBASE);
    
    /* ��ҳ */
    spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);     
    
    /* ����ת����Ŀ��Ϊ0 */
    g_usFwdNum = 0;
  
    iRet = SUCCESS;
EXIT_LABEL :
    return iRet;
}
    
/*****************************************************************************
 �� �� ��  : spm_bfd_clean_cfg_tree
 ��������  : ɾ�����ã�������ʱ����
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��9��15��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_clean_cfg_tree()
{
    NBB_INT iRet = 0;
    
    iRet = spm_bfd_clean_cfg_fwdtree();
    
    return OK;
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_clean_all_cfg
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��5��28��
 * ��������  : ɾ��BFD��������
 * �������  : NBB_CXT_T NBB_CXT  NBASE���̲߳���
 * �������  : ��
 * �� �� ֵ  : NBB_INT  ����ֵ
 * ���ù�ϵ  : 
 * ��    ��  : 

  1.��    ��   : 2015��9��15��
    ��    ��   : gaos
    �޸�����   : �޸�
*****************************************************************************/
NBB_INT spm_bfd_clean_all_cfg(NBB_CXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    SPM_BFD_GLOBAL          *pstBfdCfg = NULL;
    
    NBB_TRC_ENTRY(("spm_bfd_clean_all_cfg"));
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "##spm_bfd_clean_all_cfg begin!\r\n");
    
    /*  �����ñ��mantis:CR8000 V1R1-ER�汾���Բ��� 0083509*/
    g_ucCleanCfgCheck = 1;

    for (pstBfdCfg = (SPM_BFD_GLOBAL*) AVLL_FIRST(g_stBfdGlbTreeByDis);
        pstBfdCfg != NULL;
        pstBfdCfg = (SPM_BFD_GLOBAL*) AVLL_FIRST(g_stBfdGlbTreeByDis))
    {

        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s: clean cfg,del session %lu begin, mainboard:%d!\r\n",
            __FUNCTION__, pstBfdCfg->ulDiscrimer, pstBfdCfg->usSlotid);
        (NBB_VOID)spm_bfd_del_cfg(pstBfdCfg NBB_CCXT); 
             
    }
        
    /*  ��fwd table ɾ����ʽ���mantis:CR8000 V1R1-ER�汾���Բ��� 0083509*/
    g_ucCleanCfgCheck = 0;   
    
//    spm_bfd_clean_cfg_tree();    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "##spm_bfd_clean_all_cfg end!\r\n");

    iRet = SUCCESS;
EXIT_LABEL: NBB_TRC_EXIT();
    return iRet;
}

/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_bfd
 ��������  : BFD���ô���ģ��
 �������  : atg_dci_set_bfd_session *set_bfd  : IPS��Ϣ
             NBB_CCXT_T NBB_CCXT               
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��24��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���
 ��ע          : ���ڻỰ������洢������������һ��������������Ϊkeyֵ��һ����SIP+DIP+vrfid��Ϊkeyֵ��
                 ���У���������Ϊkey�ģ��������bfd for ldp lsp��passive�˲�ѯ����ת����ģ�
                 SIP+DIP+VRFIDֻ��¼bfd for ip��remotediscrim ��Ϊ0�����
                 ��add��������ִ�в��ɹ���������Դ��������
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
    NBB_INT                 iRet = ERROR;   /* ִ�з���ֵ */
    NBB_USHORT              usLoopi = 0;
    NBB_ULONG   ulDiscrimer = 0;
    NBB_USHORT  usMainBoardId = 0;          /* �Ự�������� */
    NBB_BYTE    ucLocalSlot   = 0;          /* ���̲�λ�� */
    NBB_BYTE    ucSlotId      = 0;          /* ����portindex��ѯ��slotid */
    NBB_BYTE    ucPortType    = 0;          /* ����portindex��ѯ�Ľӿ����� */
    NBB_ULONG   ulPortIndex   = 0;          /* ��������ĳ������� */
    NBB_USHORT  usPortId      = 0;          /* ��������������LAG��ѡ��Ա�˿� */
    NBB_ULONG   aulLagPortIndex[ATG_DCI_LAG_MEMBER_PORT_NUM] = {0};     /* LAG��Ա�˿� */
    NBB_BYTE    ucLagNum = 0;               /* LAG��Ա�˿ڸ��� */
    NBB_ULONG   ulDriverIndex = 0;          /* ���������tunnelid������lspid */
    CRTXLSP_KEY stCrlspKey;
    FTN_KEY     stLdplspKey;
    NBB_USHORT  usloopi = 0;

    NBB_BYTE *pucIpsMsgStar = NULL;         /* IPS��Ϣ��ַ */
    SPM_VP_INFO_CB stVcInfo;
    ATG_DCI_VC_KEY stVcKey;    
    SPM_PORT_INFO_CB stPortInfo;
    
    #ifdef SPU
    NBB_BYTE c3_unit = 0;
    #endif

    NBB_TRC_ENTRY(("spm_rcv_dci_set_bfd"));

    /* ����ж� */
    if (NULL == pstset_bfd)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stBfdGlbKey, 0, sizeof(SPM_BFDGLB_IP_KEY));
    OS_MEMSET(&stBfdIpv6GlbKey, 0, sizeof(SPM_BFDGLB_IPV6_KEY));

    /*  ������ɾʱ������ת��������*/
    g_ucCleanCfgCheck = 1;

    /* ɾ���Ự */
    if (TRUE == pstset_bfd->delete_struct)
    {
        spm_bfd_dci_set_bfd_del(pstset_bfd);
    }
    else /* ������ */
    {
        /* ��ȡ���̲�λ */
        (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
        
        ulDiscrimer = pstset_bfd->key;
        pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulDiscrimer);
        if (NULL == pstBfdCfg)/* ˵�������add */
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

            /* �����ûỰ���������� */
            if (BFD_CFG_IP == pstBasicBfdCB->bfd_frame_type)/* for ip */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is for ip\r\n");

                if (1 == pstBasicBfdCB->detect_method) /* �����������������Ϊ���� */
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
                    /* add begin by limingming, 2013/2/5, Mantis��:HSD111111, ԭ��:BFD�Ựѡ�� */
                    /* BFD FOR BGP��������ػ�ȡ�������ӿڣ���Ҫ���̸���IP���ҳ��ӿڣ������������BFD�϶�����·��֮���·�
                       �ģ���Ϊֻ�н���TCP�ŻὨ��BFD */
                    if ((0x7fffffff == pstBasicBfdCB->phy_if_index) || (0 == pstBasicBfdCB->phy_if_index))
                    {
                        if (BFD_IPV4 == pstBasicBfdCB->ip_type) 
                        {
                            /* ����vrfid+dip��ѯFTN�������˿� */
                            iRet = spm_l3_routeforoam(pstBasicBfdCB->vrf_id, 0, &(pstBasicBfdCB->remote_ip[3]), 
                                &ulPortIndex);
                            if (SUCCESS != iRet)/* ��ִ��ʧ�� */
                            {
                                NBB_TRC_FLOW((NBB_FORMAT"get ftn error, vrf_id = %ld remote_ip = %ld",
                                        pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[3]));
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                        "Get ftn error %d, vrf_id = %ld remote_ip = %lx\r\n", 
                                        iRet, pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[3]);

                                /* �ϱ�����ʧ����Ϣ��ֻ��BFD FOR IPû��·�ɵ�ʱ���ϱ� */
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
                            /* ����vrfid+dip��ѯFTN�������˿� */
                            iRet = spm_l3_routeforoam(pstBasicBfdCB->vrf_id, 1, &(pstBasicBfdCB->remote_ip[0]), &ulPortIndex);
                            if (SUCCESS != iRet)/* ��ִ��ʧ�� */
                            {
                                NBB_TRC_FLOW((NBB_FORMAT"get ipv6 ftn error, vrf_id = %ld remote_ip = %lx.%lx.%lx.%lx",
                                        pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[0], pstBasicBfdCB->remote_ip[1],
                                        pstBasicBfdCB->remote_ip[2], pstBasicBfdCB->remote_ip[3]));
                                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                        "Get ipv6 ftn error %d, vrf_id = %ld remote_ip = %lx.%lx.%lx.%lx\r\n", 
                                        iRet, pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[0],
                                        pstBasicBfdCB->remote_ip[1],
                                        pstBasicBfdCB->remote_ip[2], pstBasicBfdCB->remote_ip[3]);
                                
                                /* �ϱ�����ʧ����Ϣ��ֻ��BFD FOR IPû��·�ɵ�ʱ���ϱ� */
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
                    else    /* BFD for OSPS/ISIS BFD������·��֮���·� */
                    {
                        ulPortIndex = pstBasicBfdCB->phy_if_index;
                    }
                    
                    iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                    if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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

                    /* �жϳ��ӿ����� */
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
            else if (BFD_CFG_Tunnel == pstBasicBfdCB->bfd_frame_type) /* for rsvp tunnel��lsp */
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
                
                /* ����ingress+egress+tunnelid��ѯ���ӿ������� */
                OS_MEMSET(&stCrlspKey, 0, sizeof(CRTXLSP_KEY));
                stCrlspKey.egress = pstBasicBfdTunnel->egress;
                stCrlspKey.ingress = pstBasicBfdTunnel->ingress;
                stCrlspKey.tunnelid = pstBasicBfdTunnel->tunnel_id;
                iRet = spm_l3_rsvpforoam(&stCrlspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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

                /* add begin by limingming, 2013/2/17, Mantis��:HSD111111, ԭ��:BFD�Ựѡ�� */
                iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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

                /* �жϳ��ӿ����� */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* ��ΪLAG�ڣ����ص�portidΪlagid */
                    iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
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

                    /* ȡLAG��Ա�˿����ڵĵ�һ������Ϊ�������� */
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
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
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
                    
                    /* ����fec+vrfid��ѯ���ӿ������� */
                    OS_MEMSET(&stLdplspKey, 0, sizeof(FTN_KEY));
                    stLdplspKey.fec = pstBasicBfdLsp->fec_prefix;
                    stLdplspKey.mask = pstBasicBfdLsp->fec_prefix_len;
                    stLdplspKey.vrfid = pstBasicBfdLsp->vrf_id;
                    iRet = spm_l3_ldpforoam(&stLdplspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                    if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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
                    
                    /* ����ingress+egress+tunnelid��ѯ���ӿ������� */
                    OS_MEMSET(&stCrlspKey, 0, sizeof(CRTXLSP_KEY));
                    stCrlspKey.egress = pstBasicBfdTunnel->egress;
                    stCrlspKey.ingress = pstBasicBfdTunnel->ingress;
                    stCrlspKey.tunnelid = pstBasicBfdTunnel->tunnel_id;
                    stCrlspKey.lspid = pstBasicBfdTunnel->lsp_id;
                    iRet = spm_l3_rsvpforoam(&stCrlspKey, &ulPortIndex, &ulDriverIndex NBB_CCXT);
                    if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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
                
                /* add begin by limingming, 2013/2/18, Mantis��:HSD111111, ԭ��:BFD�Ựѡ�� */
                iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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

                /* �жϳ��ӿ����� */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* ��ΪLAG�ڣ����ص�portidΪlagid */
                    iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
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

                    /* ȡLAG��Ա�˿����ڵĵ�һ������Ϊ�������� */
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
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
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

                /* ����vcid+vctype+peerid��ѯ��ӿ������� */
                iRet = spm_get_vpinfo_from_vckey(&stVcKey, &stVcInfo NBB_CCXT);
                if (SUCCESS != iRet)/* ���ɹ� */
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
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
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

                if (L2VPN_VPWS == stVcInfo.vpn_type)    /* VPWS���ֶ˿���� */
                {
                    if (0 != stVcInfo.uni_slot)     /* ���Ի�ȡ��UNI��λ��˵������ͨ��PW�����õ�UNI�� */  
                    {
                        ucSlotId = stVcInfo.uni_slot;
                    }
                    else    /* ��ȡ����UNI��λ��ʾUNI��LAG��(�䵽��һ��LAG��Ա) */
                    {
                        iRet = spm_get_portid_for_bfd(stVcInfo.uni_port_index, &usPortId, &ucSlotId,
                            &ucPortType NBB_CCXT);
                        if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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

                        /* �жϳ��ӿ����� */
                        if (ATG_DCI_LAG == ucPortType)  /* LAG��(�䵽��һ��LAG��Ա) */
                        {
                            /* ��ΪLAG�ڣ����ص�portidΪlagid */
                            iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET,
                                aulLagPortIndex, &ucLagNum NBB_CCXT);
                            if (SUCCESS != iRet)/* �ж�ִ�н�� */
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

                            /* ȡLAG��Ա�˿����ڵĵ�һ������Ϊ�������� */
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
                            if (SUCCESS != iRet)/* �ж�ִ�н�� */
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
                        else if (ATG_DCI_VE_PORT == ucPortType) /* VE(�䵽ָ����λ) */  /* �˴���������VE */
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
                            if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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
                else    /* VPLS (���õ�NNI��) */
                {
                    
                    /* ����vcid+vctype+peerid��ѯ���ӿ������� */
                    iRet = spm_get_port_index_from_vc_key(&stVcKey, &ulPortIndex NBB_CCXT);
                    if (SUCCESS != iRet)/* ���ɹ� */
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

                    /* add begin by limingming, 2013/2/18, Mantis��:HSD111111, ԭ��:BFD�Ựѡ�� */
                    iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                    if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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

                    /* �жϳ��ӿ����� */
                    if (ATG_DCI_LAG == ucPortType)
                    {
                        /* ��ΪLAG�ڣ����ص�portidΪlagid */
                        iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                        if (SUCCESS != iRet)/* �ж�ִ�н�� */
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

                        /* ȡLAG��Ա�˿����ڵĵ�һ������Ϊ�������� */
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
                        if (SUCCESS != iRet)/* �ж�ִ�н�� */
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
                        if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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
                if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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
                
                /* �жϳ��ӿ����� */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* ��ΪLAG�ڣ����ص�portidΪlagid */
                    iRet = spm_get_lag_ports(usPortId - ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
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

                    /* ȡLAG��Ա�˿����ڵĵ�һ������Ϊ�������� */
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
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
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

            /* �����ý��д洢 */
            pstBfdCfg = (SPM_BFD_GLOBAL *)NBB_MM_ALLOC(sizeof(SPM_BFD_GLOBAL), NBB_NORETRY_ACT, MEM_SPM_BFD_GLOBAL_CB);
            if (NULL == pstBfdCfg)/* Ϊ�� */
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
            
            /* �ж��Ƿ���Ҫ��¼��g_stBfdGlbTreeByIp�� */
            if (((BFD_CFG_IP == pstBasicBfdCB->bfd_frame_type) || (BFD_CFG_INTERFACE == pstBasicBfdCB->bfd_frame_type)) 
                && (0 == pstBasicBfdCB->peer_sid))
            {
                if ((BFD_IPV4 == pstBasicBfdCB->ip_type) && (0 == pstBasicBfdCB->peer_sid))/* ipv4 */
                {
                    pstBfdIpCfg = (SPM_BFD_IP_GLOBAL *)NBB_MM_ALLOC(sizeof(SPM_BFD_IP_GLOBAL), 
                        NBB_NORETRY_ACT, MEM_SPM_BFD_IP_GLOBAL_CB);
                    if (NULL == pstBfdIpCfg)/* ��Ϊ�� */
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

                    /* �����Ƿ��Ѿ����� */
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
                    if (NULL == pstBfdIpv6Cfg)/* ��Ϊ�� */
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

                    /* �����Ƿ��Ѿ����� */
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
            
            /* ���ӿ�Ϊ������ */
            if (0 == spm_hardw_getboardtype((NBB_BYTE)usMainBoardId))
            {
                if (0 == spm_hardw_getboardtype(ucLocalSlot)) /* ����Ϊ�����̣��������ñ���һ��(˫����) */
                {
                    if (g_usBfdNum >= BFD_MAX_SESNUM)/* ��������� */
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
                        if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }

                    /* ���ܻỰ�ڵȴ���ɾ��״̬���Ȳ�ѯ�ȴ�ɾ����add */
                    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
                    {
                        if (ulDiscrimer == g_stBfdlocal[usLoopi].ulDiscrimer)/* ������� */
                        {
                            if (TRUE == g_stBfdlocal[usLoopi].ucbIsNeedDelete)/* ��Ҫɾ�� */
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
                                if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                                {
                                    AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                                    NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                                }
                                if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                                {
                                    AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                                    NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                                }
                                goto EXIT_LABEL;
                            }
                        }
                    }

                    /* ��ӻỰ */
                    iRet = spm_bfd_addsession(pstset_bfd->key, pstBasicBfdCB, pstBasicBfdTunnel, 
                        pstBasicBfdLsp, pstBasicBfdPw NBB_CCXT);
                    if (SUCCESS != iRet)/* ���ɹ� */
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
                        if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                    g_usBfdNum++;
                }
                else /* ���̲�Ϊ�����̣����ת������ */ 
                {
                    if (g_usFwdNum >= BFD_MAX_FWDNUM)/* ���������Ŀ */
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
                        if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                    
                    /* Ϊ���Ͻ������һ���Ƿ���ڸ�ת����Ϣ�������ڣ���˵���д��� */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &ulDiscrimer);
                    if (NULL != pstFwdInfo)/* ��Ϊ�� */
                    {
                        NBB_ASSERT(FALSE);
                        NBB_TRC_FLOW((NBB_FORMAT"Fwd error, Dri = %ld", ulDiscrimer));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd error, Dri = %ld\r\n", ulDiscrimer);
                
                        /* Ϊ��ȷ���´�ִ�гɹ���ɾ�� */
                        AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                        OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by ����,jiangbo, 20131113, Mantis��:
                                                                           ��, ԭ��:��Ϊ����nbase�������Ҳ����䡢�ͷ�
                                                                           ����ڴ棬����ʹ��NBB_MM_FREE����ͬʱ����
                                                                           ��verify */
                        pstFwdInfo = NULL;
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                        
                    iRet = spm_bfd_addfwdtable(ulDiscrimer, usMainBoardId);
                    if (SUCCESS != iRet)/* ���ɹ� */
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
                        if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                    
                    /* ��¼ת����Ϣ */
                    /* modify begin by ����,jiangbo, 20131113, Mantis��:��, ԭ��:��Ϊ����nbase�������Ҳ����䡢�ͷ����
                       �ڴ棬����ʹ��NBB_MM_ALLOC����ͬʱ���ܱ�verify */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)OS_ALLOC(sizeof(SPM_FWD_GLOBAL),MEM_SPM_BFD_FWD_GLOBAL_CB);
                    /* modify end by ����,jiangbo, 20131113 */
                    if (NULL == pstFwdInfo)/* Ϊ�� */
                    {
                        NBB_TRC_FLOW((NBB_FORMAT"Memory is not enough"));
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Memory is not enough\r\n");
                        pstset_bfd->return_code = ATG_DCI_RC_ALLOC_ERROR;
                        pstset_bfd->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->rsvp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->ldp_lsp_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        pstset_bfd->pw_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        /* ����fwdtable */
                        (NBB_VOID)spm_bfd_delfwdtable(ulDiscrimer NBB_CCXT);
                        
                        AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
                        NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
                        if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
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

                        /* �����ӹ����ͷ��ڴ棬��ֹ�ڴ���� */                        
                        OS_FREE(pstFwdInfo, MEM_SPM_BFD_FWD_GLOBAL_CB);
                        pstFwdInfo = NULL;

                        /* �����в�Ӱ��������ܣ����Լ���ִ�� */
                    }
                    else
                    {
                        g_usFwdNum++;
                    }
                }
            }
            else
            {
                if (usMainBoardId == (NBB_USHORT)ucLocalSlot)   /* �������Ǳ��� */
                {
                    if (g_usBfdNum >= BFD_MAX_SESNUM)/* ��������� */
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
                        if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }

                    /* ���ܻỰ�ڵȴ���ɾ��״̬���Ȳ�ѯ�ȴ�ɾ����add */
                    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
                    {
                        if (ulDiscrimer == g_stBfdlocal[usLoopi].ulDiscrimer)/* ������� */
                        {
                            if (TRUE == g_stBfdlocal[usLoopi].ucbIsNeedDelete)/* ��Ҫɾ�� */
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
                                if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                                {
                                    AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                                    NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                                }
                                
                                if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                                {
                                    AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                                    NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                                }
                                goto EXIT_LABEL;
                            }
                        }
                    }

                    /* ��ӻỰ */
                    iRet = spm_bfd_addsession(pstset_bfd->key, pstBasicBfdCB, pstBasicBfdTunnel, 
                        pstBasicBfdLsp, pstBasicBfdPw NBB_CCXT);
                    if (SUCCESS != iRet)/* ���ɹ� */
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
                        if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIp, pstBfdIpCfg->spm_bfdip_node);
                            NBB_MM_FREE(pstBfdIpCfg, MEM_SPM_BFD_IP_GLOBAL_CB);
                        }
                        
                        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
                        {
                            AVLL_DELETE(g_stBfdGlbTreeByIpv6, pstBfdIpv6Cfg->spm_bfdipv6_node);
                            NBB_MM_FREE(pstBfdIpv6Cfg, MEM_SPM_BFD_IPV6_GLOBAL_CB);
                        }
                        goto EXIT_LABEL;
                    }
                    g_usBfdNum++;
                }
            }

            /* ������������ */
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
                (NBB_VOID)spm_set_bfd_cfg_change(LDP_LSP_BFD_CHANGE);/* ����ģ����Ҫ��ѯldp��rsvp�ĻỰ */
            }
            if (BFD_CFG_Tunnel == pstBfdCfg->ucFramtype)
            {
                (NBB_VOID)spm_set_bfd_cfg_change(RSVP_LSP_BFD_CHANGE);
            }
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add session %d end\r\n", pstset_bfd->key);
        }
        else /* modify��� */
        {      
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Modify session %d, session mainboard:%d\r\n",
                pstset_bfd->key, pstBfdCfg->usSlotid);
            
            /* �鿴�Ƿ��Ǳ��̴��� */
            if (0 == spm_hardw_getboardtype((NBB_BYTE)pstBfdCfg->usSlotid))/* ��������Ϊ������ */
            {
                if (0 == spm_hardw_getboardtype(ucLocalSlot))/* ����Ϊ������ */
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
                    if (SUCCESS != iRet)/* ���ɹ� */
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
                    if (SUCCESS != iRet)/* ���ɹ� */
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

    /*  ������ɾʱ������ת��������*/
    g_ucCleanCfgCheck = 0;
    return;
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_dci_set_bfd_del
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��5��28��
 * ��������  : ����IPS��Ϣɾ��BFDָ������
 * �������  : ATG_DCI_SET_BFD_SESSION *pstset_bfd  IPS��Ϣ
               NBB_CCXT_T NBB_CXT                   NBASE���̲߳���
 * �������  : ��
 * �� �� ֵ  : NBB_VOID  ��
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_VOID spm_bfd_dci_set_bfd_del(ATG_DCI_SET_BFD_SESSION *pstset_bfd NBB_CCXT_T NBB_CXT)
{
    SPM_BFD_GLOBAL          *pstBfdCfg = NULL;
    NBB_ULONG   ulDiscrimer = 0;

    NBB_TRC_ENTRY(("spm_bfd_dci_set_bfd_del"));

    
    /* ����ж� */
    if (NULL == pstset_bfd)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        goto EXIT_LABEL;
    }
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Begin delete,Dri = %ld\r\n", pstset_bfd->key);
    ulDiscrimer = pstset_bfd->key;
    pstBfdCfg = (SPM_BFD_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByDis, &ulDiscrimer);
    if (NULL == pstBfdCfg)/* ��Ϊ�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Session is not exist, Dri = %ld", pstset_bfd->key));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete session %ld, but session is not exist\r\n", pstset_bfd->key);
    }
    else/* ����Ϊ�� */
    {
        spm_bfd_del_cfg(pstBfdCfg NBB_CCXT);
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s:Delete session %d end \r\n", __FUNCTION__, pstset_bfd->key);
    }
    
EXIT_LABEL : NBB_TRC_EXIT();
    return;
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_del_cfg
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��5��28��
 * ��������  : �������е��̶��д洢��BFDȫ������ɾ��ָ��BFD����
 * �������  : SPM_BFD_GLOBAL *pstBfdCfg  ���е��̶��д洢��BFDȫ������
               NBB_CCXT_T NBB_CXT         NBASE���̲߳���
 * �������  : ��
 * �� �� ֵ  : NBB_INT  ������
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_INT spm_bfd_del_cfg(SPM_BFD_GLOBAL *pstBfdCfg NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    NBB_ULONG   ulDiscrimer = 0;
    NBB_USHORT  usMainBoardId = 0;          /* �Ự�������� */
    NBB_BYTE    ucLocalSlot   = 0;          /* ���̲�λ�� */
    
    NBB_TRC_ENTRY(("spm_bfd_del_cfg"));
    
    /* ����ж� */
    if (NULL == pstBfdCfg)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        iRet = ERROR;
        goto EXIT_LABEL;
    }
    
    /* ����,���߳�ѧϰfpga fifo ����ת������ϣ�mantis 0083412 */
//    BmuWrLock(g_build_fpga_fwd_lock, BMU_WAIT_FOREVER);
    
    ulDiscrimer = pstBfdCfg->ulDiscrimer;
    
    /* ������������ */
    (NBB_VOID)spm_bfd_notify_aps_del(ulDiscrimer, pstBfdCfg->ucFramtype NBB_CCXT);

    /* ɾ����IPΪKEYֵ���� */
    (NBB_VOID)spm_bfd_del_ip_tree(pstBfdCfg NBB_CCXT);

    /* ���ͷ��ڴ�ǰ�ȰѲ�λ�Ŵ洢 */
    usMainBoardId = pstBfdCfg->usSlotid;
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    
    /* ɾ��ȫ������ */
    AVLL_DELETE(g_stBfdGlbTreeByDis, pstBfdCfg->spm_bfd_node);
    NBB_MM_FREE(pstBfdCfg, MEM_SPM_BFD_GLOBAL_CB);
    
    /* �ж��Ƿ�Ϊ���̴��� */
    if (0 == spm_hardw_getboardtype((NBB_BYTE)usMainBoardId)) /* ������������������� */
    {
        if (0 == spm_hardw_getboardtype(ucLocalSlot))/* ����Ҳ�������̣������̴���һ�£�ͬʱ��ӣ�ͬʱɾ�� */
        {
            /* ��ȫ��������������Ϊ���̣����Ǳ�����ĿΪ0��˵��֮ǰ�Ĵ���������� */
            if (0 == g_usBfdNum)
            {
                NBB_ASSERT(FALSE);
                NBB_TRC_FLOW((NBB_FORMAT"Session is empty, Dri = %ld", ulDiscrimer));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is empty, Dri = %ld\r\n", ulDiscrimer);
            }
            else/* ɾ�� */
            {
                iRet = spm_bfd_delsession(ulDiscrimer NBB_CCXT);
                if (SUCCESS != iRet)/* �ж�ִ�н�� */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"Del session failed, Dri = %ld", ulDiscrimer));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del session failed, Dri = %ld\r\n", ulDiscrimer);
                }
                else/* �ɹ� */
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
            /* ��ȫ��������������Ϊ���̣����Ǳ�����ĿΪ0��˵��֮ǰ�Ĵ���������� */
            if (0 == g_usBfdNum)
            {
                NBB_ASSERT(FALSE);
                NBB_TRC_FLOW((NBB_FORMAT"Session is empty, Dri = %ld", ulDiscrimer));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Session is empty, Dri = %ld\r\n", ulDiscrimer);
            }
            else/* ɾ�� */
            {
                iRet = spm_bfd_delsession(ulDiscrimer NBB_CCXT);
                if (SUCCESS != iRet)/* �ж�ִ�н�� */
                {
                    NBB_TRC_FLOW((NBB_FORMAT"Del session failed, Dri = %ld", ulDiscrimer));
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del session failed, Dri = %ld\r\n", ulDiscrimer);
                }
                else/* �ɹ� */
                {
                    g_usBfdNum--;
                }
            }
        }
    }
    
    /* ��������������ɾ����mantis:CR8000 V1R1-ER�汾���Բ��� 0083509*/
//    if(!g_ucCleanCfgCheck)
//    {
        /* �鿴��ҵ�����Ƿ���ڸ�ת������ */
        spm_bfd_del_fwd_tree(ulDiscrimer NBB_CCXT);
        
//    }

    /* ɾ��NNI�� */
    spm_bfd_del_nniId_table(ulDiscrimer NBB_CCXT);

    /* ���� */
//    BmuUnlock(g_build_fpga_fwd_lock);
    iRet = SUCCESS;

EXIT_LABEL : NBB_TRC_EXIT();
    return iRet;
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_del_ip_tree
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��5��28��
 * ��������  : ɾ����IPΪkeyֵ��������
 * �������  : SPM_BFD_GLOBAL *pstBfdCfg            ���е��̶��д洢��BFDȫ������
               NBB_CCXT_T NBB_CXT                   NBASE���̲߳���
 * �������  : ��
 * �� �� ֵ  : NBB_INT  ������
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_INT spm_bfd_del_ip_tree(SPM_BFD_GLOBAL *pstBfdCfg NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    SPM_BFDGLB_IP_KEY       stBfdGlbKey;
    SPM_BFDGLB_IPV6_KEY     stBfdIpv6GlbKey;
    SPM_BFD_IP_GLOBAL       *pstBfdIpCfg = NULL;
    SPM_BFD_IPV6_GLOBAL     *pstBfdIpv6Cfg = NULL;
    
    NBB_TRC_ENTRY(("spm_bfd_del_ip_tree"));
    
    /* ����ж� */
    if (NULL == pstBfdCfg)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        iRet = ERROR;
        goto EXIT_LABEL;
    }

    /* ��ʼ������ */
    OS_MEMSET(&stBfdGlbKey, 0, sizeof(SPM_BFDGLB_IP_KEY));
    OS_MEMSET(&stBfdIpv6GlbKey, 0, sizeof(SPM_BFDGLB_IPV6_KEY));

    if (BFD_IPV4 == pstBfdCfg->ucIptype)/* IPV4 */
    {
        stBfdGlbKey.ulSip = pstBfdCfg->ulSip[3];
        stBfdGlbKey.ulDip = pstBfdCfg->ulDip[3];
        stBfdGlbKey.usVrfid = pstBfdCfg->usVrfid;            
    
        pstBfdIpCfg = (SPM_BFD_IP_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIp, &stBfdGlbKey);
        if (NULL != pstBfdIpCfg)/* ���洢����ɾ�� */
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
        if (NULL != pstBfdIpv6Cfg)/* ���洢����ɾ�� */
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
 * �� �� ��  : spm_bfd_del_fwd_tree
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��5��28��
 * ��������  : ɾ��ת����
 * �������  : NBB_ULONG ulDiscrimer                ����������
               NBB_CCXT_T NBB_CXT                   NBASE���̲߳���
 * �������  : ��
 * �� �� ֵ  : NBB_INT  ������
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_INT spm_bfd_del_fwd_tree(NBB_ULONG ulDiscrimer NBB_CCXT_T NBB_CXT)
{
    NBB_INT iRet = ERROR;
    SPM_FWD_GLOBAL          *pstFwdInfo  = NULL;
    
    NBB_TRC_ENTRY(("spm_bfd_del_ip_tree"));
    
    pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &ulDiscrimer);
    if (NULL !=  pstFwdInfo)/* ��Ϊ�� */
    {
        /* ��Ϊҵ���̣������ת�����ɾ������ */
        if (0 == g_usFwdNum)
        {
            NBB_ASSERT(FALSE);
            NBB_TRC_FLOW((NBB_FORMAT"Fwd is empty, Dri = %ld", ulDiscrimer));
            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd is empty, Dri = %ld\r\n", ulDiscrimer);
        }
        else/* ɾ�� */
        {
            /* ɾ��ת������ */
            AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
            OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by ����,jiangbo, 20131113, Mantis��:��,
                                                                ԭ��:��Ϊ����nbase�������Ҳ����䡢�ͷ������
                                                                �棬����ʹ��NBB_MM_FREE����ͬʱ���ܱ�verify */
            pstFwdInfo = NULL;
            iRet = spm_bfd_delfwdtable(ulDiscrimer NBB_CCXT);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Del fwd failed = %d, Dri = %ld", iRet, ulDiscrimer));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del fwd failed = %d, Dri = %ld\r\n", iRet, ulDiscrimer);
            }
            else/* �ɹ� */
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
 �� �� ��  : spm_bfd_addsession
 ��������  : ��ӻỰ
 �������  : IN NBB_ULONG ulDrim:������  
             IN ATG_DCI_BFD_BASIC_DATA  *pstBasicBfdCB, �Ự��������
             IN ATG_DCI_BFD_RSVP_LSP_DATA *pstBasicBfdTunnel,���Ựfor tunnel����Ϊ��
             IN ATG_DCI_BFD_LDP_LSP_DATA    *pstBasicBfdLsp,���Ựfor lsp����Ϊ��
             IN ATG_DCI_BFD_PW_DATA     *pstBasicBfdPw,���Ựfor pw����Ϊ��
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��15��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_addsession(
    IN NBB_ULONG ulDrim, 
    IN ATG_DCI_BFD_BASIC_DATA  *pstBasicBfdCB, 
    IN ATG_DCI_BFD_RSVP_LSP_DATA *pstBasicBfdTunnel,
    IN ATG_DCI_BFD_LDP_LSP_DATA    *pstBasicBfdLsp,
    IN ATG_DCI_BFD_PW_DATA     *pstBasicBfdPw
    NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0; /* ѭ������ */
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;
    CRTXLSP_KEY stCrlspKey;
    FTN_KEY     stLdplspKey;
    NBB_ULONG   ulPortIndex   = 0;          /* ��������ĳ������� */
    NBB_ULONG   ulDriverIndex = 0;          /* ���������tunnelid������lspid */
    ATG_DCI_VC_KEY stVcKey;
    SPM_VP_INFO_CB stVcInfo;
    SPM_PORT_INFO_CB stPortInfo;
    NBB_BYTE ucLocalSlot;
    NBB_BYTE    ucSlotId      = 0;          /* ����portindex��ѯ��slotid */
    NBB_BYTE    ucPortType    = 0;          /* ����portindex��ѯ�Ľӿ����� */
    NBB_USHORT  usPortId      = 0;          /* ��������������LAG��ѡ��Ա�˿� */
    NBB_ULONG   aulLagPortIndex[ATG_DCI_LAG_MEMBER_PORT_NUM] = {0};     /* LAG��Ա�˿� */
    NBB_BYTE    ucLagNum = 0;               /* LAG��Ա�˿ڸ��� */
    NBB_USHORT  usLoopj = 0;
    ATG_DCI_PHY_PORT_COMMON_DATA stCommonCfg;
    NBB_ULONG ulPhyPortIndex;

    
    NBB_TRC_ENTRY(("spm_bfd_addsession"));
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));

    /* �ж���� */
    if (NULL == pstBasicBfdCB)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Input message is NULL"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        goto EXIT_LABEL;
    }

    /* ��ȡ���̲�λ */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    /* ������� */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if (DRV_ULONG_INVALID == g_stBfdlocal[usLoopi].ulDiscrimer)/* �ж����� */
        {
            /* ��¼���� */
            NBB_MEMCPY(&(g_stBfdlocal[usLoopi].pstBasicBfdCB), pstBasicBfdCB, sizeof(ATG_DCI_BFD_BASIC_DATA));
            g_stBfdlocal[usLoopi].bBasicBfdCB = ENABLE;

            /* ������ã��������ò��ҵ������ */
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
            
            /* add begin by limingming, 2013/4/11, Mantis��:HSDxxxxxx, ԭ��:IDͬ�� */
            if (0 != pstBasicBfdCB->peer_sid)
            {
                g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
            }
            /* add end by limingming, 2013/4/11 */
            
            if (TRUE != pstBasicBfdCB->bfd_enable)/* ���Ͳ�ʹ�� */
            {
                g_stBfdlocal[usLoopi].ucSessionState = BFD_ADMINDOWN;
            }
            
            if (BFD_CFG_IP == pstBasicBfdCB->bfd_frame_type) /* bfd for ip */
            {
                g_stBfdlocal[usLoopi].ucBfdtype = BFD_IP;
                
                /* BFD FOR BGP��������ػ�ȡ�������ӿڣ���Ҫ���̸���IP���ҳ��ӿڣ������������BFD�϶�����·��֮���·�
                    �ģ���Ϊֻ�н���TCP�ŻὨ��BFD */
                if ((0x7fffffff == pstBasicBfdCB->phy_if_index) || (0 == pstBasicBfdCB->phy_if_index))
                {
                    if (BFD_IPV4 == pstBasicBfdCB->ip_type) 
                    {
                        /* ����vrfid+dip��ѯFTN�������˿� */
                        iRet = spm_l3_routeforoam(pstBasicBfdCB->vrf_id, 0, &(pstBasicBfdCB->remote_ip[3]), &ulPortIndex);
                        if (SUCCESS != iRet)/* ��ִ��ʧ�� */
                        {
                            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get ftn error %d, vrf_id = %ld remote_ip = %lx\r", 
                                    iRet, pstBasicBfdCB->vrf_id, pstBasicBfdCB->remote_ip[3]);
                            BFD_LOCAL_INIT(usLoopi); 
                            goto EXIT_LABEL;
                            
                        }
                    }
                    else if (BFD_IPV6 == pstBasicBfdCB->ip_type)
                    {
                        /* ����vrfid+dip��ѯFTN�������˿� */
                        iRet = spm_l3_routeforoam(pstBasicBfdCB->vrf_id, 1, &(pstBasicBfdCB->remote_ip[0]), &ulPortIndex);
                        if (SUCCESS != iRet)/* ��ִ��ʧ�� */
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
                else    /* BFD for OSPS/ISIS BFD������·��֮���·� */
                {
                    ulPortIndex = pstBasicBfdCB->phy_if_index;
                }
                
                g_stBfdlocal[usLoopi].ulPortIndex = ulPortIndex;
                
                if ((BFD_IPV6 == pstBasicBfdCB->ip_type) && (0XFE80 == (pstBasicBfdCB->remote_ip[0] >> 16)))
                {
                    iRet = spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
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
                if (SUCCESS != iRet)/* ��ִ��ʧ�� */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, 
                        "Get port info error=%d, PortIndex = %ld\r\n", iRet, ulPortIndex);
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                
                /* �жϳ��ӿ����� */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* ��ΪLAG�ڣ����ص�portidΪlagid */
                    iRet = spm_get_lag_ports(usPortId-ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n", iRet, usPortId);
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    
                    /* ȡLAG��Ա�˿����ڵĵ�һ������Ϊ�������� */
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
                
                /* ��ȡ�˿���ʱ */
                iRet = spm_get_common_from_logical_port_index(ulPortIndex, &ulPhyPortIndex, &stCommonCfg NBB_CCXT);
                if (SUCCESS != iRet)/* ���������û��ͨ��TLV���ã�ʹ��Ĭ��ֵ */
                {
                    stCommonCfg.up_time = 40;
                    stCommonCfg.down_time = 0;
                }
                g_stBfdlocal[usLoopi].usTimerPortDownToUp = stCommonCfg.up_time;
                g_stBfdlocal[usLoopi].usTimerPortUPToDown = stCommonCfg.down_time;
                g_stBfdlocal[usLoopi].usTimerLiveUp = g_stBfdlocal[usLoopi].usTimerPortDownToUp;
                g_stBfdlocal[usLoopi].usTimerLiveDown = g_stBfdlocal[usLoopi].usTimerPortUPToDown;
            }
            else if (BFD_CFG_Tunnel == pstBasicBfdCB->bfd_frame_type)   /* bfd for rsvp,����tunnel��lsp */
            {
                g_stBfdlocal[usLoopi].pstBasicBfdTunnel = (ATG_DCI_BFD_RSVP_LSP_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_BFD_RSVP_LSP_DATA), NBB_NORETRY_ACT, MEM_SPM_BFD_RSVP_LSP_DATA_CB);
                if (NULL == g_stBfdlocal[usLoopi].pstBasicBfdTunnel)
                {
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                if(NULL == pstBasicBfdTunnel)   /* bfd for tunnel������£�tunnel���ݲ���Ϊ�� */
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
                g_stBfdlocal[usLoopi].ulLspLabel = 1;/* ��ת������ */
                
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
                    /* LDP LSP��ʱ��spm_l3_ldpforoam���ص���tunnel ID */
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
                    if(NULL == pstBasicBfdTunnel)   /* bfd for rsvp lsp������£�rsvp lsp���ݲ���Ϊ�� */
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
                    /* RSVP LSP��ʱ��spm_l3_rsvpforoam���ص���lsp ID */
                    g_stBfdlocal[usLoopi].ucBfdFwdType = BFD_FWD_LSPID;
                    g_stBfdlocal[usLoopi].usLspID = (NBB_USHORT)ulDriverIndex;
                }
                g_stBfdlocal[usLoopi].ulLspLabel = 1;/* ��ת������ */
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
                if(NULL == pstBasicBfdPw)   /* bfd for pw������£�pw���ݲ���Ϊ�� */
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
                if (SUCCESS != iRet)/* �ж�ִ�н�� */
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
                if (SUCCESS != iRet)/* �ж�ִ�н�� */
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
                if (SUCCESS != iRet)/* ��ִ��ʧ�� */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error=%d, PortIndex = %ld\r\n",
                        iRet, ulPortIndex);
                    BFD_LOCAL_INIT(usLoopi); 
                    goto EXIT_LABEL;
                }
                
                /* �жϳ��ӿ����� */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* ��ΪLAG�ڣ����ص�portidΪlagid */
                    iRet = spm_get_lag_ports(usPortId-ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n", iRet, usPortId);
                        BFD_LOCAL_INIT(usLoopi); 
                        goto EXIT_LABEL;
                    }
                    
                    /* ȡLAG��Ա�˿����ڵĵ�һ������Ϊ�������� */
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
                
                /* ��ȡ�˿���ʱ */
                iRet = spm_get_common_from_logical_port_index(ulPortIndex, &ulPhyPortIndex, &stCommonCfg NBB_CCXT);
                if (SUCCESS != iRet)/* ���������û��ͨ��TLV���ã�ʹ��Ĭ��ֵ */
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
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                BFD_LOCAL_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Set node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set node failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* �ӽڵ� */
            iRet = spm_oamtree_nodeadd(g_usBfdLspNoMax, &stBfdTreeNode, &g_stTreeBfdNodeSet);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                BFD_LOCAL_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Add node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            
            //spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  

            /* �����Ự������ */
            iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
                g_pulBfdTreebase, BFDFGPABASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                BFD_LOCAL_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Build main Tree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build main Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* ��ҳд���� */
            spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
                g_pulBfdTreebase, BFDFGPABASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                BFD_LOCAL_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Build back Tree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build back Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }
            
            spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET, 1);

            /* �洢�Ự���� */    
            g_stBfdlocal[usLoopi].bIsSendtoMaster = FALSE;
            g_stBfdlocal[usLoopi].ucStateRecord = BFD_RECORD_INVALID;

            g_bIsModifySession = TRUE;

            /* ����Ŀ��Ϊ����������֯�õ�BFD���ͱ�������дFPGA */
            spm_bfd_sendpacket_fpgareg(usLoopi, FALSE NBB_CCXT);

            break;
        }
    }

    /* û���ҵ�������Ŀ��˵���쳣��������ɾ���Ựʱδ��ԭ��ʼ�� */
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
 �� �� ��  : spm_bfd_delsession
 ��������  : ɾ���Ự
 �������  : IN NBB_ULONG ulDrim : ������ 
             NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��15��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_delsession(IN NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;

    NBB_TRC_ENTRY(("spm_bfd_delsession"));
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    
    /* ��ѯ���ػỰ���� */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if (g_stBfdlocal[usLoopi].ulDiscrimer == ulDrim)/* �ж����� */
        {
            iRet = spm_bfd_tree_set_data(usLoopi, ulDrim, &stBfdTreeNode);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Set node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            
            /* ɾ���ڵ� */
            iRet = spm_oamtree_nodedel(g_usBfdLspNoMax, &stBfdTreeNode, &g_stTreeBfdNodeSet);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            
            //spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
            
            /* �ؽ��Ự������ */
            iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
                g_pulBfdTreebase, BFDFGPABASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del node failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* ��ҳд���� */
            spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(BFDSESSIONSTATE, OAMTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usBfdTreeLevel, &g_stTreeBfdNodeSet, 
                g_pulBfdTreebase, BFDFGPABASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Build back Tree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build back Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }
            
            spm_oamtree_s16write(BFDSESSIONSTATE, OAMTREESWITCHOFFSET, 1);

            /* admindown�������ɾ��,���������Ϊ��������״̬��������Ҫ��״̬���߳�����ɾ�� */
            if (BFD_ADMINDOWN == g_stBfdlocal[usLoopi].ucSessionState)
            {
                if (TRUE == g_stBfdlocal[usLoopi].bIsChangeToDown)
                {
                    if (g_ulStateToDownNum > 0)
                    {
                        g_ulStateToDownNum--;
                    }
                }
                
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BFD_LOCAL_INIT(usLoopi);
                g_bIsModifySession = TRUE;
                
                /* ���FPGA��Ӧ�ķ��ͼĴ������� */
                spm_bfd_clear_sendfpgareg(usLoopi);

                /*���FPGA BFD�����Ĵ�������*/
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

    if (BFD_MAX_SESNUM == usLoopi)/* �ж����� */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Do not find, Disrim = %ld", ulDrim));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Do not find, Disrim = %ld\r\n", ulDrim);
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_updatesession
 ��������  : �Ự�޸Ĳ�����״̬
 �������  : IN NBB_ULONG ulDrim : ������                                          
             IN ATG_DCI_BFD_BASIC_DATA  *pstBasicBfdCB : �Ự��������
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��5��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

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
    
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* ѭ�� */
    {
        if (ulDrim == g_stBfdlocal[usLoopi].ulDiscrimer)/* ������ƥ�� */
        {
            if (TRUE != pstBasicBfdCB->bfd_enable)/* ���Ͳ�ʹ�� */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Change session to admin, Drim = %ld\r\n", ulDrim);
                if (BFD_ADMINDOWN != g_stBfdlocal[usLoopi].ucSessionState)/* ��Ϊadmin */
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

            if (g_stBfdlocal[usLoopi].ucRMRI != pstBasicBfdCB->rmri)/* �ж�rmri */
            {
                g_stBfdlocal[usLoopi].ucRMRI = pstBasicBfdCB->rmri;
                g_stBfdlocal[usLoopi].ucParaChangeFlag |= BFD_PARA_INTERVAL_CHANGE;
            }

            if (g_stBfdlocal[usLoopi].ucDMTI != pstBasicBfdCB->dmti)/* �ж�dmti */
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

    /* ��û���ҵ��Ự����˵�������� */
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
 * �� �� ��  : spm_bfd_update_common_from_physical_port_index
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��3��17��
 * ��������  : ��������ӿ�index����BFD����ʱ
 * �������  : NBB_ULONG ulPhyPortIndex     ����ӿ�index
               NBB_CCXT_T NBB_CXT           nbase���̲߳���
 * �������  : ��
 * �� �� ֵ  : NBB_INT  NBB���̲߳���
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_INT spm_bfd_update_common_from_physical_port_index(NBB_ULONG ulPhyPortIndex
                                                         NBB_CCXT_T NBB_CXT)
{
    NBB_INT  iRet = SUCCESS;
    NBB_USHORT usLoopi = 0; /* ѭ������ */
    NBB_USHORT usLoopj = 0; /* ѭ������ */
    NBB_BYTE    ucSlotId      = 0;          /* ����portindex��ѯ��slotid */
    NBB_BYTE    ucPortType    = 0;          /* ����portindex��ѯ�Ľӿ����� */
    NBB_USHORT  usPortId      = 0;          /* ��������������LAG��ѡ��Ա�˿� */
    NBB_ULONG   aulLagPortIndex[ATG_DCI_LAG_MEMBER_PORT_NUM] = {0};     /* LAG��Ա�˿� */
    NBB_BYTE    ucLagNum = 0;               /* LAG��Ա�˿ڸ��� */
    NBB_ULONG   ulPortIndex   = 0;          /* ��������ĳ������� */
    NBB_ULONG   ulLocalPhyPortIndex;
    ATG_DCI_PHY_PORT_COMMON_DATA stCommonCfg;

    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++) /* �Ա���usLoopi����ѭ�� */
    {
        if (DRV_ULONG_INVALID != g_stBfdlocal[usLoopi].ulDiscrimer)/* ������BFD */
        {
            if ((BFD_FWD_PORT == g_stBfdlocal[usLoopi].ucBfdFwdType) /* bfd for interface��bfd for linklocal ipv6 */
                || (BFD_FWD_IP == g_stBfdlocal[usLoopi].ucBfdFwdType))  /* ��bfd for ip */
            {
                ulPortIndex = g_stBfdlocal[usLoopi].ulPortIndex;

                iRet = spm_get_portid_for_bfd(ulPortIndex, &usPortId, &ucSlotId, &ucPortType NBB_CCXT);
                if (SUCCESS != iRet)/* ��ִ��ʧ�� */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                        "Get port info error=%d, PortIndex = %ld\r\n", iRet, ulPortIndex);
                    goto EXIT_LABEL;
                }

                /* �жϳ��ӿ����� */
                if (ATG_DCI_LAG == ucPortType)
                {
                    /* ��ΪLAG�ڣ����ص�portidΪlagid */
                    iRet = spm_get_lag_ports(usPortId-ATG_DCI_LAG_OFFSET, aulLagPortIndex, &ucLagNum NBB_CCXT);
                    if (SUCCESS != iRet)/* �ж�ִ�н�� */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Get port info error = %d, lagid = %d\r\n", iRet, usPortId);
                        goto EXIT_LABEL;
                    }
                    
                    /* ȡLAG��Ա�˿����ڵĵ�һ����Ա�˿� */
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

                /* ��ȡ�˿���ʱ */
                iRet = spm_get_common_from_logical_port_index(ulPortIndex, &ulLocalPhyPortIndex, &stCommonCfg NBB_CCXT);
                if (SUCCESS != iRet)/* ���������û��ͨ��TLV���ã�����һ�� */
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
 �� �� ��  : spm_bfd_addfwdtable
 ��������  : ���ת����
 �������  : IN NBB_ULONG ulDrim    :������                      
             IN NBB_USHORT usSlotid :��������
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��29��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_addfwdtable(IN NBB_ULONG ulDrim, IN NBB_USHORT usSlotid)
{
    NBB_USHORT usLoopi = 0; /* ѭ������ */
    
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;
    
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add fwd %lu begin, slot %d\r\n", ulDrim, usSlotid);
    /* ������� */
    for (usLoopi = 0; usLoopi < BFD_MAX_FWDNUM; usLoopi++)
    {
        if (DRV_ULONG_INVALID == g_stBfdfwd[usLoopi].ulDiscrimer)/* ����� */
        {            
            g_stBfdfwd[usLoopi].usSlotid = usSlotid;
            g_stBfdfwd[usLoopi].ulDiscrimer = ulDrim;
            
            iRet = spm_fwd_tree_set_data(usLoopi, ulDrim, &stBfdTreeNode);
            if (SUCCESS != iRet)/* ���ɹ� */
            {
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BFD_FWD_INIT(usLoopi);
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Set fwd node failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* �ӽڵ� */
            iRet = spm_oamtree_nodeadd(g_usFwdLspNoMax, &stBfdTreeNode, &g_stTreeFwdNodeSet);
            if (SUCCESS != iRet)/* ���ɹ� */
            {
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BFD_FWD_INIT(usLoopi);
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Add fwd node failed, Discri = %ld, iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  

            iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
                g_pulFwdTreebase, OAMFWDFPAGBASE);
            
            if (SUCCESS != iRet) /* �ж�ִ�н�� */
            {
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BFD_FWD_INIT(usLoopi);
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Build fwd main Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            /* ��ҳд���� */
            spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
                g_pulFwdTreebase, OAMFWDFPAGBASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                BFD_FWD_INIT(usLoopi);
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Build fwd back Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }

            spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);

            /* ����Ŀ��Ϊ����������֯�õ�ת������дFPGA */
            spm_bfd_fwdpacket_fpgareg(usLoopi);
            g_uiFwdTableCount++;
            break;
            
        }
    }

    /* û���ҵ�������Ŀ��˵���쳣��������ɾ���Ựʱδ��ԭ��ʼ�� */
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
 �� �� ��  : spm_bfd_delfwdtable
 ��������  : ɾ��ת������
 �������  : IN NBB_ULONG ulDrim  :������
             NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��15��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_delfwdtable(IN NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;

    NBB_TRC_ENTRY(("spm_bfd_delfwdtable"));
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete fwd %lu begin\r\n", ulDrim);
    /* ��ѯ���ػỰ���� */
    for (usLoopi = 0; usLoopi < BFD_MAX_FWDNUM; usLoopi++)
    {
        if (g_stBfdfwd[usLoopi].ulDiscrimer == ulDrim)/* �ж����� */
        {
            iRet = spm_fwd_tree_set_data(usLoopi, ulDrim, &stBfdTreeNode);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BFD_FWD_INIT(usLoopi);
                
                /* ���FPGA��Ӧ�ķ��ͼĴ������� */
                spm_bfd_clear_fwdfpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Set node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }

            /* ɾ���ڵ� */
            iRet = spm_oamtree_nodedel(g_usFwdLspNoMax, &stBfdTreeNode, &g_stTreeFwdNodeSet);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BFD_FWD_INIT(usLoopi);
                
                /* ���FPGA��Ӧ�ķ��ͼĴ������� */
                spm_bfd_clear_fwdfpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Del node failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del node failed, Discri = %ld,iRet=%d\r\n", ulDrim, iRet);
                goto EXIT_LABEL;
            }
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  

            /* �ؽ��Ự������ */
            iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
                g_pulFwdTreebase, OAMFWDFPAGBASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BFD_FWD_INIT(usLoopi);
                
                /* ���FPGA��Ӧ�ķ��ͼĴ������� */
                spm_bfd_clear_fwdfpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"rebuild oamtree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "rebuild oamtree  failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }
            
            /* ��ҳд���� */
            spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usFwdTreeLevel, &g_stTreeFwdNodeSet, 
                g_pulFwdTreebase, OAMFWDFPAGBASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                BFD_FWD_INIT(usLoopi);
                NBB_TRC_FLOW((NBB_FORMAT"Build fwd back Tree failed, Discri = %ld", ulDrim));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build fwd back Tree failed, Discri = %ld\r\n", ulDrim);
                goto EXIT_LABEL;
            }
            spm_oamtree_s16write(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET, 1);
            
            /* ���Ự��Ϣ��Ϊ��Чֵ */
            BFD_FWD_INIT(usLoopi);

            /* ���FPGA��Ӧ�ķ��ͼĴ������� */
            spm_bfd_clear_fwdfpgareg(usLoopi NBB_CCXT);
            
            break;
        }
    }

    if (BFD_MAX_FWDNUM == usLoopi)/* �ж����� */
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
 �� �� ��  : spm_bfd_clear_recivefpgareg
 ��������  : �Ựɾ��ʱ���FPGA BFD_RX�лỰ�ռĴ�������
 �������  : IN NBB_USHORT usLocalNum :������Ŀ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��28��
    ��    ��   : tianf
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_clear_recv_fpgareg(IN NBB_USHORT usLocalNum)
{
    NBB_USHORT usData = 0;

    spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x1, usData);
    
    /*����YID���հ�MYIDƥ����λȥʹ��*/
    spm_oamtree_s16write(BFDSESSIONBASE, 0x5000 + usLocalNum, 0);
    return 0;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_set_anti_attack
 ��������  : ����FPGA BFD_RX�лỰ�ռĴ���BFD����������
 �������  : IN NBB_USHORT usLocalNum :������Ŀ��
                IN NBB_USHORT usDate :ʹ��ֵ 0/1=��ʹ��/ʹ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��28��
    ��    ��   : tianf
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
        /* BFD��ģ��Ĵ���ƫ��0x4000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
        spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x0, 
        (g_stBfdlocal[usLocalNum].ulRemoteDiscr)&DRV_USHORT_INVALID);
        
         /* BFD��ģ��Ĵ���ƫ��0x4000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
        spm_oamtree_s16write(BFDSESSIONBASE, 0x4000 + 2 * usLocalNum + 0x1, 
            (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
            
        /*����YID���հ�MYIDƥ����λʹ��*/
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
 �� �� ��  : spm_bfd_clear_sendfpgareg
 ��������  : �Ựɾ��ʱ���FPGA�лỰ���ͼĴ�������
 �������  : IN NBB_USHORT usLocalNum :������Ŀ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��17��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
    
    /* 0xe000 ֻ�� */
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x10000 + 2 * usLocalNum + 0x1, usData);
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x11000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x11000 + 2 * usLocalNum + 0x1, usData);
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x26000 + 2 * usLocalNum + 0x0, usData);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x26000 + 2 * usLocalNum + 0x1, usData);

    /* 0xf000 ֻ�� */

    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLocalNum + 0x0,usData);
    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLocalNum + 0x1,usData);
    
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_clear_fwdfpgareg
 ��������  : �Ựɾ��ʱ���FPGA��ת���Ĵ�������
 �������  : IN NBB_USHORT usLocalNum :������Ŀ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��17��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
 �� �� ��  : spm_bfd_fwdpacket_fpgareg
 ��������  : ����BFDת�����ĵļĴ���
 �������  : IN NBB_USHORT usLocalNum : ������Ŀ�� 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��30��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_fwdpacket_fpgareg(IN NBB_USHORT usLocalNum)
{
    SPM_BFD_FWD_PACKET1 stBfdFwd1;
    SPM_BFD_FWD_PACKET2 stBfdFwd2;
    SPM_BFD_FWD_PACKET3 stBfdFwd3;
    NBB_USHORT           usData1     = 0;
    NBB_USHORT           usData2     = 0;
    NBB_USHORT           usData3     = 0;
    NBB_BYTE    ucLocalSlot   = 0;          /* ���̲�λ�� */
    

    /* ����ж� */
    if (usLocalNum >= BFD_MAX_FWDNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fwd fpga, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }
    OS_MEMSET(&stBfdFwd1, 0, sizeof(SPM_BFD_FWD_PACKET1));
    OS_MEMSET(&stBfdFwd2, 0, sizeof(SPM_BFD_FWD_PACKET2));
    OS_MEMSET(&stBfdFwd3, 0, sizeof(SPM_BFD_FWD_PACKET3));

    /* �ж��Ƿ���ת���������̣����ǣ������鲥 */
    if (0 == spm_hardw_getboardtype(g_stBfdfwd[usLocalNum].usSlotid))
    {
        stBfdFwd1.usFwdTrafficClass = 0; /* ������arad���ȼ� */
        stBfdFwd1.usFwdDesInfo1 = (NBB_USHORT)((BFD_MULTI_ID >> 16)&0x7);
        usData1 = *(NBB_USHORT *)(&stBfdFwd1);
        spm_oamtree_s16write(OAMFWDPAKFPGABASE, 8 * usLocalNum + 0x0, usData1);
        
        usData2 = (NBB_USHORT)(BFD_MULTI_ID&DRV_USHORT_INVALID);
        spm_oamtree_s16write(OAMFWDPAKFPGABASE, 8 * usLocalNum + 0x1, usData2);
        stBfdFwd3.usDslotid = 31; /* �鲥���ͣ�Dslot��31 */
    }
    else/* ���� */
    {
        stBfdFwd1.usFwdTrafficClass = 7; /* ������arad���ȼ� */
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
 �� �� ��  : spm_bfd_sendpacket_fpgareg
 ��������  : ����BFD���ͱ��ĵļĴ���
 �������  : IN NBB_USHORT usLocalNum  :������Ŀ��
             IN NBB_BOOL bIsModify     :�Ƿ����޸Ĳ�����������Ϊ�޸Ĳ�������Ҫ��λĳ��bitλ��ֵ��
             ����ֵΪFALSE/TRUE
                
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��12��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
    
    /* �ж���� */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Set send packet fpga, input num %d is wrong!", usLocalNum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set send packet fpga, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stPacketInfo1, 0, sizeof(SPM_BFD_REG_PACKET1));
    OS_MEMSET(&stMplsLabel1, 0, sizeof(SPM_BFD_MPLSLABEL));
    OS_MEMSET(&stMplsLabel2, 0, sizeof(SPM_BFD_MPLSLABEL));

    /* DMAC+SMAC��12�ֽڣ���װ��ת��ƽ�棬��ʽΪreserved(1byte)+SVLAN(2bytes)+LSPID(2bytes)+ reserved(5bits)+FWDTYPE(3bits)
        + vrfid(2byte)+ipv4address(4bytes), �������ϵ�BFD��Ҫ�ȷ���һ��ҵ�����㣬Ȼ����ת��ƽ�棬������صĸ�ʽ
        Ϊ ITMH(4bytes)+PKT_TYPE(3bits)+Dslotid(5bits)+Sslotid(5bits)+FWDTYPE(3bits)+vrfid(2byte)+
        ipv4address(4bytes) */
    
    spm_bfd_sendpacket_macreg(usLocalNum);

    spm_bfd_sendpacket_intervalreg(usLocalNum, bIsModify);

    /* �������� */
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
    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x0, 
        (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
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

    /* UDP�˿ں� */
    stPacketInfo1.usUDPtype = g_stBfdlocal[usLocalNum].ucDetectiontype;

    /* �жϷ�װ���� */
    stPacketInfo1.usSendType = g_stBfdlocal[usLocalNum].ucBfdtype;
    if((BFD_FWD_PORT == g_stBfdlocal[usLocalNum].ucBfdFwdType) && (g_stBfdlocal[usLocalNum].usSVlanID != 0))
    {
       stPacketInfo1.usVlanAble = FALSE; //����VLANʹ��
    }
    else
    {
        stPacketInfo1.usVlanAble = TRUE; //����VLAN��ʹ��
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
 �� �� ��  : spm_bfd_sendpacket_intervalreg
 ��������  : �޸�Ƶ��/״̬�Ĵ�������
 �������  : IN NBB_USHORT usLocalNum  :������Ŀ��
             IN NBB_BOOL bIsModify  :�Ƿ����޸Ĳ�����������Ϊ�޸Ĳ�������Ҫ��λĳ��bitλ��ֵ��
             ����ֵΪFALSE/TRUE   
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��13��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_intervalreg(IN NBB_USHORT usLocalNum, IN NBB_BOOL bIsModify)
{
    NBB_USHORT   usData1 = 0;
    NBB_USHORT   usData2 = 0;
    SPM_BFD_REG_PACKET2 stPacketInfo2;
    SPM_BFD_REG_PACKET3 stPacketInfo3;

    //NBB_TRC_ENTRY(("spm_bfd_sendpacket_intervalreg"));
    
    /* �ж���� */
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
    /* BFD����ģ��Ĵ���ƫ��0xd000+2*N+0x0��
       Bit15-11��BFD��������루Diag�����������ͷ����һ�λỰdown��ԭ��
       Bit10-9��BFD��ǰ�Ự״̬��Sta����0-AdminDown��1-Down��2-Init��3-Up��
       Bit7-0�����ʱ�䱶����Detect Mult����Ĭ��ֵΪ3 */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xd000 + 2 * usLocalNum + 0x0, usData1);

    OS_MEMSET(&stPacketInfo3, 0, sizeof(SPM_BFD_REG_PACKET3));
    stPacketInfo3.usRxInterval = g_stBfdlocal[usLocalNum].ucRequiredMinRxInterval;
    stPacketInfo3.usTxInterval = g_stBfdlocal[usLocalNum].ucDesiredMinTxInterval;
    stPacketInfo3.usModify     = bIsModify;
    usData2 = *(NBB_USHORT *)(&stPacketInfo3);
    /* BFD����ģ��Ĵ���ƫ��0xd000+2*N+0x1��
       Bit0������2��������κ�һ����Ҫ���޸ģ������д'1'������ͶƱ���̡���FPGA�ڲ����㡣 */
    spm_oamtree_s16write(BFDPAKFGPABASE, 0xd000 + 2 * usLocalNum + 0x1, usData2);

    usLocalNum = 0x8000 + usLocalNum;
    /* BFD����ģ��Ĵ���ƫ��0x0002��
       Bit15: �޸�BFD������5�ֶ�̬������Diag/ Sta/ Detect Mult/DMTI/RMRI����ʶ���������ÿ������5�ֶ�̬������ɺ�д'1'��
       Bit10-0��������BFD����5�ֶ�̬��������Ŀ�š�����������ò���ǰд��Ҫ���ò�������Ŀ�š�*/
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x0002, usLocalNum);
    
    EXIT_LABEL : /*NBB_TRC_EXIT();*/
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_state_fsm
 ��������  : ����FPGA�ж�Ӧ��Ŀ�Ự�Ľ��յ��ı���״̬���޸ı���Ŀ�ĻỰ״̬
 �������  : IN NBB_USHORT usLocalNum :��Ŀ�ţ����ô˺���ʱ��֤��Ŀ�Ŷ�Ӧ�ĻỰ�м�¼ 
             IN NBB_BYTE ucSessionState : �յ��ı��ĵ�״̬
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��18��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm(IN NBB_USHORT usLocalNum, IN NBB_BYTE ucSessionState)
{
    //NBB_TRC_ENTRY(("spm_bfd_state_fsm"));
    
    /* �ж���� */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fsm, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }

    /* ��ǰΪadmindown���κ�״̬������ */
    if (BFD_ADMINDOWN == g_stBfdlocal[usLocalNum].ucSessionState)
    {
        goto EXIT_LABEL;
    }
    
    switch(ucSessionState)
    {
        case BFD_ADMINDOWN:
        if (BFD_DOWN != g_stBfdlocal[usLocalNum].ucSessionState)/* ��Ϊdown */
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
        if (BFD_DOWN == g_stBfdlocal[usLocalNum].ucSessionState)/* Ϊdown */
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
 �� �� ��  : spm_bfd_state_fsm_down
 ��������  : �Ự����down״̬����
 �������  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��18��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm_down(IN NBB_USHORT usLocalNum,IN NBB_BYTE ucPacktState)
{
    NBB_BYTE ucLastState = 0;
    SPM_BFD_EVENT_INFO stBfdEventInfo;

    //NBB_TRC_ENTRY(("spm_bfd_state_fsm_down"));
    
    /* �ж���� */
    if (usLocalNum >= BFD_MAX_SESNUM)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set fsm down state, input num %d is wrong!\r\n", usLocalNum);
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stBfdEventInfo, 0, sizeof(SPM_BFD_EVENT_INFO));
    if (BFD_DOWN != g_stBfdlocal[usLocalNum].ucSessionState)/* ��Ϊdown */
    {
        /* add begin by limingming, 2013/4/11, Mantis��:HSDxxxxxx, ԭ��:IDͬ�� */
        if (BFD_UP == g_stBfdlocal[usLocalNum].ucSessionState) /* ԭ����up״̬�������Ǵ�up���down */
        {
            if (0 != g_stBfdlocal[usLocalNum].usTimerLiveUp)    /* ���ֹ���down��up�ֻص�down�������֮��û���ϱ�up����� */
            {
                g_stBfdlocal[usLocalNum].usTimerLiveDown = 0;   /* ��ռ����������Ͳ����ϱ� */
            }            
            else
            {
                if(0 == g_stBfdlocal[usLocalNum].usTimerPortUPToDown)   /* �����ϱ�������������ⲿ�жϲ������ȥ�������޷��ϱ� */
                {
                    g_stBfdlocal[usLocalNum].usTimerLiveDown = 1;   /* ǿ����������Ϊ1����spm_bfd_fsm_task״̬�������-1�����ϱ� */
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
        g_stBfdlocal[usLocalNum].usTimerLiveUp = 0;   /* ״̬���down��up��ʱ��ʱ��ʧȥ���� */

        spm_bfd_set_anti_attack(usLocalNum, 0);
        
        stBfdEventInfo.usLocalIndex = usLocalNum;
        stBfdEventInfo.ucLastSessionState = ucLastState;
        stBfdEventInfo.ucRxPacketState = ucPacktState;
        spm_bfd_event(&stBfdEventInfo, TRUE);
        
        spm_bfd_sendpacket_intervalreg(usLocalNum, TRUE);

        /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x0, 
            (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
        /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x1, 
            (g_stBfdlocal[usLocalNum].ulRemoteDiscr)&DRV_USHORT_INVALID);
    }
    else    /* ԭ��Ϊdown״̬ */
    {
        if (0 != g_stBfdlocal[usLocalNum].ulRemoteDiscr)    /* ѧϰ���������� */
        {
            /* ��ѧϰ��BFD��Ŀ */
            if (ENABLE == g_stBfdlocal[usLocalNum].bBasicBfdCB)
            {
                if (TRUE == g_stBfdlocal[usLocalNum].pstBasicBfdCB.peer_sid_selflearn_enable)
                {
                    
                    g_stBfdlocal[usLocalNum].usTimerStayDown++;
                    if(g_stBfdlocal[usLocalNum].usTimerStayDown > BFD_STAYDOWN) /* ��ʱ������ֵ�� */
                    {
                        /* ��ʱ��Ҫ���FPGA���ϴ�ѧϰ���������� */
                        spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLocalNum + 0x0,0);
                        spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLocalNum + 0x1,0);

                        g_stBfdlocal[usLocalNum].ulRemoteDiscr = 0;
                        g_stBfdlocal[usLocalNum].usBfdflag = BFD_NO_CREATE;

                        /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x0, 
                            (g_stBfdlocal[usLocalNum].ulRemoteDiscr >> 16)&DRV_USHORT_INVALID);
                        /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLocalNum + 0x1, 
                            (g_stBfdlocal[usLocalNum].ulRemoteDiscr)&DRV_USHORT_INVALID);

                        g_stBfdlocal[usLocalNum].usTimerStayDown = 0;   /* ��ն�ʱ�� */

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
 �� �� ��  : spm_bfd_state_fsm_init
 ��������  : �Ự����init״̬����
 �������  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��18��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm_init(IN NBB_USHORT usLocalNum,IN NBB_BYTE ucPacktState)
{
    NBB_USHORT   usData = 0;
    SPM_BFD_REG_PACKET2 stPacketInfo2;
    NBB_BYTE ucLastState = 0;
    SPM_BFD_EVENT_INFO stBfdEventInfo;
    
    //NBB_TRC_ENTRY(("spm_bfd_state_fsm_init"));
    
    /* �ж���� */
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
 �� �� ��  : spm_bfd_state_fsm_up
 ��������  : �Ự����up״̬����
 �������  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��18��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm_up(IN NBB_USHORT usLocalNum,IN NBB_BYTE ucPacktState)
{
    NBB_BOOL bIsModify = FALSE;
    NBB_BYTE ucLastState = 0;
    SPM_BFD_EVENT_INFO stBfdEventInfo;
    
    //NBB_TRC_ENTRY(("spm_bfd_state_fsm_up"));
    
    /* �ж���� */
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
        g_stBfdlocal[usLocalNum].usTimerLiveUp = BFD_LIVEUP; /* �ȶ�up5s */
    }
    g_stBfdlocal[usLocalNum].usTimerLiveDown = 0;   /* ״̬���up��down��ʱ��ʱ��ʧȥ���� */
    g_stBfdlocal[usLocalNum].bIsAdminFlag = FALSE;
    g_stBfdlocal[usLocalNum].bIsUpOnce = TRUE;
    ucLastState = g_stBfdlocal[usLocalNum].ucSessionState;
    g_stBfdlocal[usLocalNum].ucSessionState = BFD_UP;
    g_stBfdlocal[usLocalNum].ucLocalDiag = bfd_diag_no_diag;
    g_stBfdlocal[usLocalNum].ucDetectMult = g_stBfdlocal[usLocalNum].ucDM;

    /* �ж�dmti */
    if (g_stBfdlocal[usLocalNum].ucDesiredMinTxInterval != g_stBfdlocal[usLocalNum].ucDMTI)
    {
        g_stBfdlocal[usLocalNum].ucDesiredMinTxInterval = g_stBfdlocal[usLocalNum].ucDMTI;
        bIsModify = TRUE;
    }

    /* �ж�rmri */
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
 �� �� ��  : spm_bfd_state_fsm_admindown
 ��������  : �Ự����admindown״̬����
 �������  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��18��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_state_fsm_admindown(IN NBB_USHORT usLocalNum)
{
    //NBB_TRC_ENTRY(("spm_bfd_state_fsm_admindown"));
    
    /* �ж���� */
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
 �� �� ��  : spm_bfd_packet_forward
 ��������  : ͨ��linxת��������������
 �������  : IN NBB_BYTE *pBuf        
             IN NBB_ULONG ulMsgLenth  
             IN NBB_USHORT usDesSlotid
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��8��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : �޸�ΪMTL��ʽ������linx
*****************************************************************************/
NBB_VOID spm_bfd_packet_forward(IN NBB_BYTE *pBuf, IN NBB_ULONG ulMsgLenth, IN NBB_USHORT usDesSlotid)
{
    NBB_CHAR *pcLinxData = NULL;
    NBB_ULONG ulSendLenth = 0;
    NBB_INT    iRet = ERROR;
    NBB_CHAR *pcLinxTmp = NULL;
    MTL_NODE mtl_head = {0};

    g_stDesNode.module_id = TYPE_MTL_TNE;
    g_stDesNode.slot = spm_hardw_getemustate(); /* �����������أ�����������ת�� */
    g_stDesNode.cpu = 0;                /* ���͵����ص�CPU,��������ת�������CPU */
    
    ulSendLenth = ulMsgLenth + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD);    /* ��ԭ���Ļ����ϼ���ͷ */
    pcLinxData = malloc(ulSendLenth);
    if (NULL == pcLinxData)
    {
        return;
    }

    pcLinxTmp = pcLinxData;
    
    /* �鷢������CPU��ͷ */
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->des_head.module_id = TYPE_MTL_BFD;
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->des_head.frame = g_stDesNode.frame;
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->des_head.slot = usDesSlotid;
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->des_head.cpu = 0;
    
    /* ��ͬ�����ݵ�ͷ */
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->syn_head.ucType = BFD_FWD_MSG_PCK;
    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pcLinxTmp)->syn_head.ulSrcSlot = g_stSrcNode.slot;

    /* �������� */
    pcLinxTmp = pcLinxTmp + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD);
    memcpy(pcLinxTmp, pBuf, ulMsgLenth);
    
    /* 2015.5.21����mtl��ʽ���ͺ󣬱���λֱ�ӷ�������CPU,����Ҫ�ȷ���������ͨ�����ط�������CPU, 
    �����޸Ľ�����֮ǰlinx��������ϣ��˴�������Ϣ��ȥ��MTL_NODE,����mtl��˵��Ϊ����*/
    iRet = mtl_send(mtl_identity_to_fd(1 << 24 | usDesSlotid << 16 | 0), TYPE_MTL_BFD, TYPE_MTL_BFD,
        pcLinxData + sizeof(MTL_NODE), ulSendLenth - sizeof(MTL_NODE));

    /* free memory */
    free(pcLinxData);
    pcLinxData = NULL;

    /* ����ʧ�� */
    if(iRet < 0)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s:mtl_send error = %d! \r\n",__FUNCTION__, iRet);
    }
    return;

}

/*****************************************************************************
 �� �� ��  : BFD_Packet_Receive
 ��������  : BFD���մ�C3����YOURIDΪ0�ı���
 �������  : IN void *pBuf  
             IN NBB_ULONG ulMsgLenth
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��6��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���
 ��ע          : C3����BFD���ģ���YOURIDΪ0�ı���ֱ�����ͱ���CPU��
                 ����CPUͨ������ƥ�䣬��ѯȫ��BFD���ñ���   
                 ƥ�������Vrfid+dip+sip,C3���͵�BFD��������port��Ϣ��
                 ����vlan+port��ѯ�ӿڵ�Vrfid,
                 BFD for pw�������Ѿ�Э��, BFD FOR LSP activeһ���������ֱ��ĵ���������Ϊ127/8
                 ���Σ��޷�ѧϰ��������BFD FOR INTERFACE����˿�ƥ�䣬��ͨ����ѯDIP���Ҳ����ڿ�������

  2.��    ��   : 2015��6��18��
    ��    ��   : gaos
    �޸�����   : ��ӵ�����Ϣ
*****************************************************************************/
NBB_INT spm_bfd_packet_receive(IN NBB_BYTE *pBuf, IN NBB_ULONG ulMsgLenth)
{
    NBB_USHORT     usLoopi = 0;         /* ѭ������ */
    NBB_USHORT     usLoopj = 0;         /* ѭ������ */
    NBB_USHORT     usPortId = 0;
    NBB_USHORT     usVrfid = 0;
    NBB_LONG       lLenthTmp = 0;
    NBB_BYTE   ucSessionState = 0;     /* ���ֱ��ĵ�״̬ */
    NBB_INT    iRet = ERROR;
    SPM_BFD_IP_GLOBAL       *pstBfdIpCfg = NULL;
    SPM_BFD_IPV6_GLOBAL       *pstBfdIpv6Cfg = NULL;
    SPM_FWD_GLOBAL          *pstFwdInfo  = NULL;
    SPM_BFDGLB_IP_KEY       stBfdGlbKey;
    SPM_BFDGLB_IPV6_KEY      stBfdGlbipv6Key;
    NBB_BYTE    ucLocalSlot   = 0;          /* ���̲�λ�� */
    minm_header_t *pstMacInMac = NULL;
    SPM_BFD_PACKET_ABSTRACT stPacketInfo;
    SPM_XCU_STATE stXcuState = {0};        /* ���ص������ò�λ */
    NBB_LONG ulCounter = 0;/* ������ */
    
    /* ��nbase�߳�ʹ��nbase�ڲ�ʹ��NBB_CXT�����ĵ�APIʱ��ƭ�������� */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();

    if (NULL == pBuf)/* ����ж� */
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Input message is NULL\r\n");
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPacketInfo, 0, sizeof(SPM_BFD_PACKET_ABSTRACT));
    
    lLenthTmp = ulMsgLenth - 28; /* ƫ�Ƶ�ǰ���mac in mac */
    BFD_PACKETLEN_JUDGE(lLenthTmp);

    /* ��ȡ���̲�λ */
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);

    OS_MEMSET(&stBfdGlbKey, 0, sizeof(SPM_BFDGLB_IP_KEY)); 
    OS_MEMSET(&stBfdGlbipv6Key, 0, sizeof(SPM_BFDGLB_IPV6_KEY)); 
    
    /* ����DIP/SIP��mac in mac�л�ȡVPNID����ȡ���̲�λ�ţ���ȡ�����е�MYID */
    pstMacInMac = (minm_header_t *)pBuf;

    /* �ж϶˿����� */
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
        if (0x7F000000 == (stPacketInfo.ulDip[0] & 0x7F000000))/* 127���� */    /* 127����bfd for lsp�ģ�FPGAѧϰ */
        {
            iRet = SUCCESS;
            goto EXIT_LABEL;
        }
        else if (0xE0000000 == (stPacketInfo.ulDip[0] & 0xE0000000))/* 224���� *//* bfd for interface�Ļ�Ϊ���鲥��ַ */
        {
            /* ֻ�ò�ѯ�������̵ı��������з��ͷ�ʽΪport������ */
            for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
            {
                /* ������Ŀ�޻Ự������ */
                if (DRV_ULONG_INVALID == g_stBfdlocal[usLoopi].ulDiscrimer)
                {
                    continue;
                }
                
                if (BFD_FWD_PORT == g_stBfdlocal[usLoopi].ucBfdFwdType)/* port */
                {
                    if (usPortId == g_stBfdlocal[usLoopi].usPortID)/* �ж�portid */
                    {
                        g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;
                        
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                            "The %u bfd(Segment 224) learn discr  from c3, my_id %ld, yourid %ld, local ip %#x, remote ip %#x, vrf %d\r\n", 
                            usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, stPacketInfo.ulMyDrim, 
                            stPacketInfo.ulDip[0], stPacketInfo.ulSip[0], usVrfid);

                        /* ��ѧϰ����������д��Ĵ��� */
                        /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                            (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                        /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                        spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                            (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                        g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;

                        /* ��״̬�� */
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
            /* ����VRFID+DIP+SIP��ѯȫ�ֻỰ���� */
            stBfdGlbKey.ulSip = stPacketInfo.ulDip[0];
            stBfdGlbKey.ulDip = stPacketInfo.ulSip[0];
            stBfdGlbKey.usVrfid = usVrfid;
            pstBfdIpCfg = (SPM_BFD_IP_GLOBAL *)AVLL_FIND(g_stBfdGlbTreeByIp, &stBfdGlbKey);
            if (NULL != pstBfdIpCfg)/* ��Ϊ�� */
            {
                if(pstBfdIpCfg->usSlotid == (NBB_USHORT)ucLocalSlot)/* ���ָûỰΪ���̴���Ự */
                {
                    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* ѭ�� */
                    {
                        /* ������Ŀ�޻Ự������ */
                        if (DRV_ULONG_INVALID == g_stBfdlocal[usLoopi].ulDiscrimer)
                        {
                            continue;
                        }
                        if (pstBfdIpCfg->ulDiscrimer == g_stBfdlocal[usLoopi].ulDiscrimer)/* ������ƥ�� */
                        {
                            if ((BFD_DOWN == g_stBfdlocal[usLoopi].ucSessionState) 
                                    || (BFD_INIT == g_stBfdlocal[usLoopi].ucSessionState))
                            {
                                if ((0 == g_stBfdlocal[usLoopi].ulRemoteDiscr) 
                                    || (stPacketInfo.ulMyDrim != g_stBfdlocal[usLoopi].ulRemoteDiscr))
                                {
                                    /* ��ʱ��Ҫ���FPGA���ϴ�ѧϰ���������� */
                                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x0,0);
                                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x1,0);
                                    
                                    g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;

                                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                                        "The %u bfd(ip) learn discr from c3, my_id %ld, yourid %ld, local ip %#x, remote ip %#x, vrf %d\r\n", 
                                        usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, stPacketInfo.ulMyDrim, 
                                        stPacketInfo.ulDip[0], stPacketInfo.ulSip[0], usVrfid);
                                    
                                    /* ��ѧϰ����������д��Ĵ��� */
                                    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                                        (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                                    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
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
                else if (1 == spm_hardw_getboardtype(pstBfdIpCfg->usSlotid))/* �����Ǳ��̣��ֲ��������̴���Ự */
                {
                    if (g_usFwdNum >= BFD_MAX_FWDNUM)/* ���������Ŀ */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd is full, Dri = %ld\r\n", pstBfdIpCfg->ulDiscrimer);
                        goto EXIT_LABEL;
                    }

                    #if 0
                    /* ����ת������ */
                    /* ��ת�������Ƿ���� */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &pstBfdIpCfg->ulDiscrimer);
                    if (NULL != pstFwdInfo)/* ˵����ת�������Ѿ����� */
                    {
                        /* ������ת������������ */
                        spm_bfd_packet_forward(pBuf, ulMsgLenth, pstFwdInfo->usSlotid);

                        /* ��¼ת����Ϣ */
                        spm_bfd_fwd_txrecord(pstFwdInfo->usSlotid, pstFwdInfo->ulDiscrimer);
                        iRet = SUCCESS;
                        goto EXIT_LABEL;
                    }

                    /* �������ڣ����¼ת����Ϣ������ת������ */
                    /* modify begin by ����,jiangbo, 20131113, Mantis��:��, ԭ��:��Ϊ����nbase�������Ҳ����䡢�ͷ����
                       �ڴ棬����ʹ��NBB_MM_ALLOC����ͬʱ���ܱ�verify */
                    pstFwdInfo = (SPM_FWD_GLOBAL *)OS_ALLOC(sizeof(SPM_FWD_GLOBAL), MEM_SPM_BFD_FWD_GLOBAL_CB);
                    /* modify end by ����,jiangbo, 20131113 */
                    if (NULL == pstFwdInfo)/* Ϊ�� */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Memory is not enough\r\n");
                        goto EXIT_LABEL;
                    }
                    pstFwdInfo->ulDiscrimer = pstBfdIpCfg->ulDiscrimer;
                    pstFwdInfo->usSlotid = pstBfdIpCfg->usSlotid;
                    AVLL_INIT_NODE(pstFwdInfo->spm_fwd_node);
                    AVLL_INSERT(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                    
                    iRet = spm_bfd_addfwdtable(pstBfdIpCfg->ulDiscrimer, pstBfdIpCfg->usSlotid);
                    if (SUCCESS != iRet)/* ���ɹ� */
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "add fwd table failed, discrim = %ld, Dslotid = %d\r\n", 
                            pstBfdIpCfg->ulDiscrimer, pstBfdIpCfg->usSlotid);
                        AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                        OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by ����,jiangbo, 20131113, Mantis��:
                                                                           ��, ԭ��:��Ϊ����nbase�������Ҳ����䡢�ͷ�
                                                                           ����ڴ棬����ʹ��NBB_MM_FREE����ͬʱ����
                                                                           ��verify */
                        goto EXIT_LABEL;
                    }
                    
                    g_usFwdNum++;
                    #endif

                    /* ������ͨ���̼�ͨ��ת�����Ự�������� */
                    spm_bfd_packet_forward(pBuf, ulMsgLenth, pstBfdIpCfg->usSlotid);
                    
                    /* ��¼ת����Ϣ */
                    spm_bfd_fwd_txrecord(pstBfdIpCfg->usSlotid, pstBfdIpCfg->ulDiscrimer);
                    if(BFD_DEBUG_SHOWPACKETRECV == g_enBfdDebug)
                    {
                       printf("%s,%d:spm_bfd_packet_forward call!\n",__FUNCTION__,__LINE__); 
                       BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                        "%s,%d:spm_bfd_packet_forward call!\n",__FUNCTION__,__LINE__);
                    }                     
                }
                else/* �ỰΪ�����̴��� */
                {
                    #ifdef SPU
                    /* ת�������Ѿ����»Ự��ʱ���������ֻ�轫����˫���������� */
                    
                    iRet = spm_hardw_getxcustate(&stXcuState);
                    if(SUCCESS != iRet)
                    {
                        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "getxcustate error %d\r\n", iRet);
                        iRet = ERROR;
                        goto EXIT_LABEL;
                    }
                    /* ���������ز۷� */
                    if(0xff != stXcuState.ucMainXcuSlot)
                    {
                        spm_bfd_packet_forward(pBuf, ulMsgLenth, stXcuState.ucMainXcuSlot);
                    }
                    /* �����������ز۷� */
                    if(0xff != stXcuState.ucBakXcuSlot)
                    {
                        spm_bfd_packet_forward(pBuf, ulMsgLenth, stXcuState.ucBakXcuSlot);
                    }
                    
                    /* ��¼ת����Ϣ */
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
        if (NULL != pstBfdIpv6Cfg)/* ��Ϊ�� */
        {
            if(pstBfdIpv6Cfg->usSlotid == (NBB_USHORT)ucLocalSlot)/* ���ָûỰΪ���̴���Ự */
            {
                for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* ѭ�� */
                {
                    /* ������Ŀ�޻Ự������ */
                    if (DRV_ULONG_INVALID == g_stBfdlocal[usLoopi].ulDiscrimer)
                    {
                        continue;
                    }
                    if (pstBfdIpv6Cfg->ulDiscrimer == g_stBfdlocal[usLoopi].ulDiscrimer)/* ������ƥ�� */
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
                            
                            /* ��ѧϰ����������д��Ĵ��� */
                            /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                            spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                                (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                            /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                            spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                                (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                            g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
                            spm_bfd_state_fsm(usLoopi, ucSessionState);
                            break;
                        }
                    }
                }
            }
            else if (1 == spm_hardw_getboardtype(pstBfdIpv6Cfg->usSlotid))/* ��Ϊ�����̴���Ự */
            {
                if (g_usFwdNum >= BFD_MAX_FWDNUM)/* ���������Ŀ */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd is full, Dri = %ld\r\n", pstBfdIpv6Cfg->ulDiscrimer);
                    goto EXIT_LABEL;
                }
                
                /* ����ת������ */
                /* ��ת�������Ƿ���� */
                pstFwdInfo = (SPM_FWD_GLOBAL *)AVLL_FIND(g_stFwdGlbTreeByDis, &pstBfdIpv6Cfg->ulDiscrimer);
                if (NULL != pstFwdInfo)/* ˵����ת�������Ѿ����� */
                {
                    /* ������ת������������ */
                    spm_bfd_packet_forward(pBuf, ulMsgLenth, pstFwdInfo->usSlotid);

                    /* ��¼ת����Ϣ */
                    spm_bfd_fwd_txrecord(pstFwdInfo->usSlotid, pstFwdInfo->ulDiscrimer);
                    iRet = SUCCESS;
                    goto EXIT_LABEL;
                }

                /* �������ڣ����¼ת����Ϣ������ת������ */                
                /* modify begin by ����,jiangbo, 20131113, Mantis��:��, ԭ��:��Ϊ����nbase�������Ҳ����䡢�ͷ�����ڴ�
                   ������ʹ��NBB_MM_ALLOC����ͬʱ���ܱ�verify */
                pstFwdInfo = (SPM_FWD_GLOBAL *)OS_ALLOC(sizeof(SPM_FWD_GLOBAL), MEM_SPM_BFD_FWD_GLOBAL_CB);
                /* modify end by ����,jiangbo, 20131113 */
                if (NULL == pstFwdInfo)/* Ϊ�� */
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
                if (SUCCESS != iRet)/* ���ɹ� */
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "add fwd table failed, discrim = %ld, Dslotid = %d\r\n", 
                        pstBfdIpv6Cfg->ulDiscrimer, pstBfdIpv6Cfg->usSlotid);
                    AVLL_DELETE(g_stFwdGlbTreeByDis, pstFwdInfo->spm_fwd_node);
                    OS_FREE(pstFwdInfo, sizeof(SPM_FWD_GLOBAL));    /* modify by ����,jiangbo, 20131113, Mantis��:
                                                                       ��, ԭ��:��Ϊ����nbase�������Ҳ����䡢�ͷ�
                                                                       ����ڴ棬����ʹ��NBB_MM_FREE����ͬʱ����
                                                                       ��verify */
                    pstFwdInfo = NULL;                                                   
                    goto EXIT_LABEL;
                }
                
                g_usFwdNum++;

                /* ������ͨ���̼�ͨ��ת�����Ự�������� */
                spm_bfd_packet_forward(pBuf, ulMsgLenth, pstFwdInfo->usSlotid);
                
                /* ��¼ת����Ϣ */
                spm_bfd_fwd_txrecord(pstFwdInfo->usSlotid, pstFwdInfo->ulDiscrimer);
            }
            else/* �ỰΪ�����̴��� */
            {
                #ifdef SPU
                /* ת�������Ѿ����»Ự��ʱ���������ֻ�轫����˫���������� */
                
                iRet = spm_hardw_getxcustate(&stXcuState);
                if(SUCCESS != iRet)
                {
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "getxcustate error %d\r\n", iRet);
                    iRet = ERROR;
                    goto EXIT_LABEL;
                }
                /* ���������ز۷� */
                if(0xff != stXcuState.ucMainXcuSlot)
                {
                    spm_bfd_packet_forward(pBuf, ulMsgLenth, stXcuState.ucMainXcuSlot);
                }
                /* �����������ز۷� */
                if(0xff != stXcuState.ucBakXcuSlot)
                {
                    spm_bfd_packet_forward(pBuf, ulMsgLenth, stXcuState.ucBakXcuSlot);
                }
                
                /* ��¼ת����Ϣ */
                spm_bfd_fwd_txrecord(stXcuState.ucMainXcuSlot, pstBfdIpv6Cfg->ulDiscrimer);
                #endif
            }
        }
        
    }

    /* ��ӡbuffer�������� */
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
 �� �� ��  : bfd_c3_to_cp
 ��������  : C3��yourid=0�ı��ķ���CPU
 �������  : int unit              
             c3_rxtx_pkt_t *rxpkt  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��7��2��
    ��    ��   : gaos
    �޸�����   : ��Ӻ���ͷ����

*****************************************************************************/
c3_rx_ret_t bfd_c3_to_cp(int unit, c3_rxtx_pkt_t *rxpkt)
{
    int ret = 0;
    NBB_UINT uiMsgLenth = 0;
    NBB_UINT mhead_len = 0;
    NBB_CHAR acBuf[BFD_MSG_BUF_LENTH];

    /* ������� */
    if((0 != unit ) && (1 != unit ))
    {
        printf("RXPKT is not to cp, return!\n");
        return C3_RX_HANDLED_OWNED;
    }
    
    /* ������� */
    if(NULL == rxpkt)
    {
        printf("Rxpkt is NULL!\n");
        return C3_RX_HANDLED_OWNED;
    }

    mhead_len = sizeof(minm_header_t);
    
    /* ������� */
    if((NULL == rxpkt->head) || (NULL == rxpkt->payload) || (rxpkt->payload != ((uint8 *)rxpkt->head + mhead_len)))
    {
        printf("The rxpkt.head and rxpkt.payload is not Connected!\n");
        return C3_RX_HANDLED_OWNED;
    }

    uiMsgLenth = rxpkt->len + mhead_len;

    /* ����������� */
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
 �� �� ��  : spm_bfd_sendpacket_ipv4fpga
 ��������  : IPV4 BFD���ķ�װ����IPV4�ֶε�UDP�ֶ�
 �������  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��17��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_ipv4fpga(IN NBB_USHORT usLocalNum)
{
    NBB_USHORT usData1 = 0;
    NBB_USHORT usData2 = 0;

    usData1 = 0x45;
    usData1 = (usData1 << 8);
    usData2 = 0x7; /* ���ȼ� */
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
 �� �� ��  : spm_bfd_sendpacket_ipv6fpga
 ��������  : IPV6 BFD���ķ�װ����IPV6�ֶε�UDP�ֶ�
 �������  : IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��17��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_sendpacket_ipv6fpga(IN NBB_USHORT usLocalNum)
{
    
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x4000 + usLocalNum, 0x6000);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x5000 + usLocalNum, 0);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x6000 + usLocalNum, 32);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x7000 + usLocalNum, 0x11ff);

    /*���Ų��� 2015.11.09 ��дVLAN����PRI = 7*/
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
 �� �� ��  : spm_bfd_init_bus_map_table
 ��������  : ��ʼ�� SRC FPGA 0x930000 ~ 0x932fff Ϊ0xffff���˺�����ҵ������Ϊ׮������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��15��
    ��    ��   : tianf
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_bfd_state_map_init(NBB_CXT_T NBB_CXT)
{

    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_tx_rx_counter_init
 ��������  : ��ʼ��BFD ���ļ���������ʹ��
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��12��1��
    ��    ��   : tianf
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_tx_rx_counter_init(NBB_CXT_T NBB_CXT)
{
    
    NBB_USHORT usDate = TRUE;

    NBB_TRC_ENTRY(__FUNCTION__);

    spm_oamtree_s16write(BFDSESSIONBASE, 0x3, usDate); /*�ռ���Ĭ�϶�����ʹ��*/
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x3, usDate); /*������Ĭ�϶�����ʹ��*/
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return SUCCESS;
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_init_fpga
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��11��14��
 * ��������  : ��ʼ��BFD ��ص�FPGA�Ĵ���
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : NBB_VOID  ��
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_VOID spm_bfd_init_fpga(NBB_VOID)
{
    /* ��ʼ��BFD�澯ͬ��ӳ��� */
    spm_proc_bfd_state_map_init();

    /*�շ�����Ĭ�϶�����ʹ��*/
    spm_bfd_tx_rx_counter_init();
    return;
}


/*****************************************************************************
 �� �� ��  : spm_bfd_init_fwdtable
 ��������  : ת�������ʼ�����˺�������������Ϊ׮������ҵ���̲���ת�����֧��2047��
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��31��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_init_fwdtable(NBB_CXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0; /* ѭ������ */
    NBB_INT iRet = ERROR;

    NBB_TRC_ENTRY(("spm_bfd_init_fwdtable"));

    /* ת����������ʼ�� */
    for (usLoopi = 0; usLoopi < BFD_MAX_FWDNUM; usLoopi++)
    {
        BFD_FWD_INIT(usLoopi);
    }

    /* BFDת�����������ʼ����ÿҵ����֧��2kת���� */    
    g_usFwdTreeLevel  = 11;
    g_usFwdTreeNodeMaxnum = (1 << g_usFwdTreeLevel);
    g_usFwdLspNoMax   = g_usFwdTreeNodeMaxnum - 1;
    g_usFwdNum = 0;
    g_pulFwdTreebase = (NBB_ULONG *)NBB_MM_ALLOC(sizeof(NBB_ULONG)*g_usFwdTreeLevel,
        NBB_NORETRY_ACT, MEM_SPM_BFD_PUL_FWD_TREE_BASE_CB);
    if (NULL == g_pulFwdTreebase)/* �ڴ�����ʧ�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Malloc error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Malloc error\r\n");
        goto EXIT_LABEL;
    }
    
    NBB_MEMSET(g_pulFwdTreebase, 0 ,sizeof(NBB_ULONG)*g_usFwdTreeLevel);
    iRet = spm_oamtree_init(g_usFwdTreeNodeMaxnum, spm_fwd_tree_get_fpga_data, &g_stTreeFwdNodeSet);
    if (SUCCESS != iRet)/* �ж�ִ�н�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Fwd node set error, nodenum = %d", g_usFwdTreeNodeMaxnum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd node set error, nodenum = %d\r\n", g_usFwdTreeNodeMaxnum);
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_syn_sessionid
 ��������  : ҵ���̶�ʱ��Ȿ�̵�ID�Ƿ����˱仯���������˱仯��ͬ����������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��4��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���
 ��ע          :
 ÿ��ҵ���̰���1k������ͬ��ID��������Ŀ�ŵ�˳������ÿ��ͬ����Ϣ������Ϊ
 ��������(1���ֽ�)+g_ulSequenceID(���к�)+������(2bytes * 1023)

  2.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : ����mtlҪ���޸�
*****************************************************************************/
NBB_INT  spm_bfd_syn_sessionid(NBB_VOID)
{
    NBB_USHORT usLoopi = 0;         /* ѭ������ */
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
    if ( SUCCESS != iRet ) /* ��SUCCESS != iRet����ʱִ�� */
    {
        goto EXIT_LABEL;
    }
    ucMustateSlot[0] = stXcuState.ucMainXcuSlot;    /*�������صĲ�λ�ţ�����λ��0xff */
    ucMustateSlot[1] = stXcuState.ucBakXcuSlot;     /* �������صĲ�λ��,����λ��0xff */

    g_stDesNode.module_id = TYPE_MTL_TNE;
    g_stDesNode.slot = stXcuState.ucMainXcuSlot; /*�������صĲ�λ�ţ�����λ��0xff */
    g_stDesNode.cpu = 1;                        /* ���͵����ص�CPU,��������ת�������CPU */
    g_ucMainEmuId = g_stDesNode.slot;
    
    if (0xff == g_stDesNode.slot)
    {
        goto EXIT_LABEL;
    }
        
    /* �߱�ͬ��ID������ */
    if ((TRUE == g_bIsModifySession) && (FALSE == g_bIsReadySendtoMaster))
    {        
        pucTmpInfo = g_pucSynIdInfo;

        /* �鷢������CPU��ͷ */
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.module_id = TYPE_MTL_BFD;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.frame = g_stDesNode.frame;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.slot = g_stDesNode.slot;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.cpu = 0;
        
        /* ��ͬ�����ݵ�ͷ */
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->syn_head.ucType = BFD_FWD_MSG_ID;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->syn_head.ulSrcSlot = g_stSrcNode.slot;
        
        /* ��������� */
        pusTmp = (SPM_BFD_SYN_BFD_ID*)(pucTmpInfo + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD));
        pusTmp->ulSequenceID= g_ulSequenceID;    /* ��װ���к� */
        
        /* ��֡ */
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            pusTmp->arrucBfdId[usLoopi] = (NBB_USHORT)g_stBfdlocal[usLoopi].ulDiscrimer;/* ulDiscrimer��BFD ID��2���ֽ� */
        }

        /* ������������ */
        for (iLoop = 0; iLoop < 2; iLoop++) /* �������������� */
        {
            if (0xff == ucMustateSlot[iLoop])   /* ����۲���λ�������� */
            {
                continue;
            }
          
            ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.slot = ucMustateSlot[iLoop]; /* �޸���Ҫ�����Ĳ�λ�� */
            pcLinxData = malloc(g_ulSynIdInfoLth);
            if (NULL == pcLinxData)
            {
                /* �ڴ治���Ͳ����ط��ˣ���ֹ200msˢһ���ڴ���� */
                BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR,
                    "alloc linx sig error, mod id %lu, frame %lu, des slot %lu, cpu %lu, G_SeqId %lu\r\n",
                    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.module_id,
                    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.frame,
                    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.slot,
                    ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.cpu, g_ulSequenceID);
                goto EXIT_LABEL;
            }

            memcpy(pcLinxData, g_pucSynIdInfo, g_ulSynIdInfoLth);
            
            /* 2015.5.21����mtl��ʽ���ͺ󣬱���λֱ�ӷ�������CPU,����Ҫ�ȷ���������ͨ�����ط�������CPU, 
                �����޸Ľ�����֮ǰlinx��������ϣ��˴�������Ϣ��ȥ��MTL_NODE,����mtl��˵��Ϊ����*/
            iRet = mtl_send(mtl_identity_to_fd(1 << 24 | ucMustateSlot[iLoop] << 16 | 0), TYPE_MTL_BFD, TYPE_MTL_BFD, 
                pcLinxData + sizeof(MTL_NODE), g_ulSynIdInfoLth - sizeof(MTL_NODE));
            free(pcLinxData);
            pcLinxData = NULL;
            
            if (iRet < 0)
            {
                if(ucMustateSlot[iLoop] == stXcuState.ucMainXcuSlot)    /* �������ز���Ҫ�ط���ͬʱֻ�����زŻ��ȷ�ϰ� */
                {
                    g_bIsModifySession = TRUE;  /* ʧ�ܺ����·��� */
                    BMU_SLOG(BMU_WARNING, SPM_BFD_LOG_DIR, "%s:mtl send error = %d,  G_SeqId %lu,XcuSlot:%d\r\n",
                    __FUNCTION__,iRet, g_ulSequenceID, ucMustateSlot[iLoop]);                    
                }

                goto EXIT_LABEL;
            }

//            BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "%s:mtl send success, iRet=%ld G_SeqId %lu,XcuSlot:%d\r\n",
//                __FUNCTION__,iRet, g_ulSequenceID, ucMustateSlot[iLoop]);

        }

        /* ��֡��ɺ󣬷���pucSynIdInfo����λg_bIsReadySendtoMaster */
        g_bIsReadySendtoMaster = TRUE;
        g_bIsModifySession = FALSE;
        ucSynIdTimer = 0;       /* ��ռ����� */

        /* ������ʱ����5s���ж��Ƿ��л�Ӧ��Ϣ */
        (NBB_VOID)BmuWdStart(g_stBfdWdog, BmuWd5MSTickBase(1000), spm_bfd_syncheck_task, 0);
        
    }
    
    iRet = SUCCESS;
    EXIT_LABEL : 
    return iRet;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_synrecv_sessionid
 ��������  : ҵ���̽���������ID��Ӧ����
 �������  : IN void *pBuf  :���յ��ı���
             IN NBB_ULONG ulBufLength :���յ��ı��ĳ���                    
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��5��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_synrecv_sessionid(
    IN NBB_VOID *pBuf, 
    IN NBB_ULONG ulBufLength, 
    IN NBB_USHORT usSlotid)
{
    NBB_ULONG   pulSeqID  = 0;
    NBB_USHORT     usLoopi = 0;         /* ѭ������ */
    SPM_BFD_SYN_BFD_ID   *pucTmpInfo = NULL;
    NBB_INT     iRet = ERROR;
    NBB_BYTE   ucNewEmuId = 0;
    
    (NBB_VOID)usSlotid;     /* ���Ը���Σ�Ҳ����ʹ��NBB_UNREFERENCED_PARAMETER */
    
    /* ����ж� */
    if ((NULL == pBuf) || (0 == ulBufLength))
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Syn rev Id, Input is NULL\r\n");
        goto EXIT_LABEL;
    }

    pucTmpInfo = (SPM_BFD_SYN_BFD_ID *)pBuf;
    pulSeqID = pucTmpInfo->ulSequenceID;

    /* �жϻ�Ӧ�����к��Ƿ�һ�£���һ�£���˵������Ϣ��Ч */
    if (g_ulSequenceID == pulSeqID)
    {
        /* ��ѯ�������� */
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            /* �ж����� */
            if ((DRV_USHORT_INVALID != pucTmpInfo->arrucBfdId[usLoopi])
                && (pucTmpInfo->arrucBfdId[usLoopi] == (NBB_USHORT)g_stBfdlocal[usLoopi].ulDiscrimer))
            {
                g_stBfdlocal[usLoopi].bIsSendtoMaster = TRUE;
            }
        }
       
        BMU_SLOG(BMU_DEBUG, SPM_BFD_LOG_DIR, "mani emu successful recv bfd id. G_SeqId = %lu\r\n", g_ulSequenceID);
    }
    else /* ����� */
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Msg is not on time coming��SeqID = %ld, G_SeqID = %ld\r\n",
            pulSeqID, g_ulSequenceID);
        
        g_bIsModifySession = TRUE;
    }

    /* �������������浹�� */
    ucNewEmuId = spm_hardw_getemustate();
    if ((ucNewEmuId != g_ucMainEmuId) && (ucNewEmuId != 0xff))
    {
        g_bIsModifySession = TRUE;
      
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "main emu chaned, need resend bfd id, G_SeqId = %lu\r\n", g_ulSequenceID);
    }
    
    g_ulSequenceID++;               /* ���кż�1��Ϊ�´�ͬ����׼�� */
    g_bIsReadySendtoMaster = FALSE; /* �����ٴ�ͬ�� */

    /* �ص�5s�ӵĶ�ʱ�� */
    (NBB_VOID)BmuWdCancel(g_stBfdWdog);   
    iRet = SUCCESS;
    EXIT_LABEL : 
    return iRet;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_sendpacket_macreg
 ��������  : ҵ�������ö�Ӧmac�ļĴ���
 �������  : IN NBB_USHORT usLocalNum : :������Ŀ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��14��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
    
    /* �ж���� */
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

    /* LSP����� */
    if (BFD_FWD_LSPID == g_stBfdlocal[usLocalNum].ucBfdFwdType) 
    {
        usData1 = g_stBfdlocal[usLocalNum].usLspID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }

    else if (BFD_FWD_TUNNELID == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* tunnel��� */
    {
        usData1 = g_stBfdlocal[usLocalNum].usTunnelID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }

    else if (BFD_FWD_PW == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* pw��� */
    {
        usData1 = g_stBfdlocal[usLocalNum].usNHIID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }

    else if (BFD_FWD_IP == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* ip��� */
    {
        usData1 = g_stBfdlocal[usLocalNum].usVrfid;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);

        usData2 = (g_stBfdlocal[usLocalNum].ulDip[3] >> 16)&DRV_USHORT_INVALID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData2);

        usData3 = g_stBfdlocal[usLocalNum].ulDip[3]&DRV_USHORT_INVALID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData3);
    }
    else if (BFD_FWD_PORT == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* interface��� */
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

    /* ת������ */
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
 �� �� ��  : spm_bfd_state_fpgareg
 ��������  : �����̵ĻỰ״̬д�뱳������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��29��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_state_fpgareg(NBB_VOID)
{
    NBB_USHORT usLoopi = 0;
    NBB_USHORT usLoopj = 0;
    NBB_UINT uiData = 0;
    SPM_BFD_REG_STATE stState;

    for (usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM; usLoopi = usLoopi + 16) /* ѭ�� */
    {
        /* TODO:�Ż�Ч�� */
        OS_MEMSET(&stState, 0, sizeof(SPM_BFD_REG_STATE));
        stState.uiState1 = g_stBfdlocal[usLoopi].ucStateRecord;
        if ((usLoopi + 1) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState2 = g_stBfdlocal[usLoopi + 1].ucStateRecord;
        }
        if ((usLoopi + 2) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState3 = g_stBfdlocal[usLoopi + 2].ucStateRecord;
        }
        if ((usLoopi + 3) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState4 = g_stBfdlocal[usLoopi + 3].ucStateRecord;
        }
        if ((usLoopi + 4) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState5 = g_stBfdlocal[usLoopi + 4].ucStateRecord;
        }
        if ((usLoopi + 5) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState6 = g_stBfdlocal[usLoopi + 5].ucStateRecord;
        }
        if ((usLoopi + 6) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState7 = g_stBfdlocal[usLoopi + 6].ucStateRecord;
        }
        if ((usLoopi + 7) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState8 = g_stBfdlocal[usLoopi + 7].ucStateRecord;
        }
        if ((usLoopi + 8) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState9 = g_stBfdlocal[usLoopi + 8].ucStateRecord;
        }
        if ((usLoopi + 9) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState10 = g_stBfdlocal[usLoopi + 9].ucStateRecord;
        }
        if ((usLoopi + 10) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState11 = g_stBfdlocal[usLoopi + 10].ucStateRecord;
        }
        if ((usLoopi + 11) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState12 = g_stBfdlocal[usLoopi + 11].ucStateRecord;
        }
        if ((usLoopi + 12) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState13 = g_stBfdlocal[usLoopi + 12].ucStateRecord;
        }
        if ((usLoopi + 13) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState14 = g_stBfdlocal[usLoopi + 13].ucStateRecord;
        }
        if ((usLoopi + 14) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState15 = g_stBfdlocal[usLoopi + 14].ucStateRecord;
        }
        if ((usLoopi + 15) < BFD_MAX_SESNUM) /* �ж����� */
        {
            stState.uiState16 = g_stBfdlocal[usLoopi + 15].ucStateRecord;
        }
        uiData = *(NBB_UINT *)(&stState);
        spm_oamtree_s32write(BFDSTATEBASE, 0x10000 + usLoopj, uiData);
        usLoopj++;
    }
}


/*****************************************************************************
 �� �� ��  : spm_bfd_init_rxpacket
 ��������  : ע��C3��������CPU��BFD����
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��26��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

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
    if (BMU_INVALID_MSG_ID == g_stBfdMsgQID) /* ������Ϣ����ʧ�� */
    {
        NBB_PRINTF("Creat Que Err!\n");
        return ERROR;
    }
    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_Bfd_msgque_read
 ��������  : ��MSG���к���
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��26��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

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
 �� �� ��  : spm_bfd_msgque_task
 ��������  : ����������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��27��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

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
 �� �� ��  : spm_bfd_init_Nni_table
 ��������  : NNI�����ʼ�����˺�������������Ϊ׮������ҵ���̲���NNI���֧��1024��
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��31��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_init_Nni_table(NBB_CXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0; /* ѭ������ */
    NBB_INT iRet = ERROR;

    NBB_TRC_ENTRY(("spm_bfd_init_fwdtable"));

    /* ת����������ʼ�� */
    for (usLoopi = 0; usLoopi < BFD_MAX_NNINUM; usLoopi++)
    {
        g_stBfdNni[usLoopi].ulDrim = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulIpv4 = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulPwLabel = DRV_ULONG_INVALID;
        g_stBfdNni[usLoopi].ulNniId = DRV_ULONG_INVALID;
    }

    /* BFD NNI���������ʼ����ÿҵ����֧��1kת���� */    
    g_usNniTreeLevel  = 10;
    g_usNniTreeNodeMaxnum = (1 << g_usNniTreeLevel);
    g_usNniNoMax   = g_usNniTreeNodeMaxnum - 1;
    g_usNniNum = 0;
    g_pulNniTreebase = (NBB_ULONG *)NBB_MM_ALLOC(sizeof(NBB_ULONG)*g_usNniTreeLevel,
        NBB_NORETRY_ACT, MEM_SPM_BFD_PUL_NNI_TREE_BASE_CB);
    if (NULL == g_pulNniTreebase)/* �ڴ�����ʧ�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Malloc error"));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Malloc error\r\n");
        goto EXIT_LABEL;
    }
    
    NBB_MEMSET(g_pulNniTreebase, 0 ,sizeof(NBB_ULONG)*g_usNniTreeLevel);
    iRet = spm_oamtree_init(g_usNniTreeNodeMaxnum, spm_nni_tree_get_fpga_data, &g_stTreeNniNodeSet);
    if (SUCCESS != iRet)/* �ж�ִ�н�� */
    {
        NBB_TRC_FLOW((NBB_FORMAT "Fwd node set error, nodenum = %d", g_usNniTreeNodeMaxnum));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Fwd node set error, nodenum = %d\r\n", g_usNniTreeNodeMaxnum);
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return iRet;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_add_nniId_table
 ��������  : ##
 �������  : NBB_ULONG ulDrim                     
             NBB_ULONG ulPeerIp                   
             NBB_ULONG ulPwLabel                  
             NBB_ULONG ulNniIdNBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

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
    
    /* ������� */
    for (usLoopi = 0; usLoopi < BFD_MAX_NNINUM; usLoopi++)
    {
        if (DRV_ULONG_INVALID == g_stBfdNni[usLoopi].ulDrim)/* ����� */
        {
            g_stBfdNni[usLoopi].ulDrim = ulDrim;
            g_stBfdNni[usLoopi].ulIpv4 = ulPeerIp;
            g_stBfdNni[usLoopi].ulPwLabel = ulPwLabel;
            g_stBfdNni[usLoopi].ulNniId = ulNniId;
            
            /* �ӽڵ� */
            iRet = spm_nni_tree_set_data(usLoopi, g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel, &stBfdTreeNode);
            if (SUCCESS != iRet)/* ���ɹ� */
            {
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                    "Set NNI node failed, ulPeerIp = %#x, ulPwLabel = %lu, ulNniId = %lu,iRet=%d\r\n",
                    ulPeerIp, ulPwLabel, ulNniId, iRet);
                goto ERROR_LABEL;
            }

            iRet = spm_oamtree_nodeadd(g_usNniNoMax, &stBfdTreeNode, &g_stTreeNniNodeSet);
            if (SUCCESS != iRet)/* ���ɹ� */
            {
                /* ���Ự��Ϣ��Ϊ��Чֵ */
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                    "Add NNI node failed, ulPeerIp = %#x, ulPwLabel = %lu, ulNniId = %lu,iRet=%d\r\n",
                    ulPeerIp, ulPwLabel, ulNniId, iRet);
                goto ERROR_LABEL;
            }
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  

            iRet = spm_oamtree_build(g_usNniTreeLevel, &g_stTreeNniNodeSet, 
                g_pulNniTreebase, OAMNNIFPAGBASE);
            
            if (SUCCESS != iRet) /* �ж�ִ�н�� */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                    "Build NNI main Tree failed, ulPeerIp = %#x, ulPwLabel = %lu, ulNniId = %lu\r\n",
                    ulPeerIp, ulPwLabel, ulNniId);
                goto ERROR_LABEL;
            }

            /* ��ҳд���� */
            spm_oamtree_s16write(OAMNNIDATAFPAGBASE, NNITREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usNniTreeLevel, &g_stTreeNniNodeSet, 
                g_pulNniTreebase, OAMNNIFPAGBASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                    "Build NNI back Tree failed, ulPeerIp = %#x, ulPwLabel = %lu, ulNniId = %lu\r\n",
                    ulPeerIp, ulPwLabel, ulNniId);
                goto ERROR_LABEL;
            }

            spm_oamtree_s16write(OAMNNIDATAFPAGBASE, NNITREESWITCHOFFSET, 1);

            /* ����Ŀ��Ϊ����������֯�õ�ת������дFPGA */
            spm_bfd_nniId_fpgareg(usLoopi);
            break;
            
        }
    }

    /* û���ҵ�������Ŀ��˵���쳣��������ɾ���Ựʱδ��ԭ��ʼ�� */
    if (BFD_MAX_NNINUM == usLoopi)
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,"Error, please check !!!!\n");
        goto EXIT_LABEL;
    }
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Add nni end\r\n");
    goto EXIT_LABEL;

ERROR_LABEL:
    /* ���Ự��Ϣ��Ϊ��Чֵ */
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
 �� �� ��  : spm_bfd_delfwdtable
 ��������  : ɾ��ת������
 �������  : IN NBB_ULONG ulDrim  :������
             NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��15��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_del_nniId_table(NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT usLoopi = 0;
    NBB_INT  iRet = ERROR;
    SPM_OAMTREE_NODE stBfdTreeNode;

    NBB_TRC_ENTRY(("spm_bfd_del_nniId_table"));
    OS_MEMSET(&stBfdTreeNode, 0, sizeof(SPM_OAMTREE_NODE));
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete nni %lu begin\r\n", ulDrim);
    /* ��ѯ���ػỰ���� */
    for (usLoopi = 0; usLoopi < BFD_MAX_NNINUM; usLoopi++)
    {
        if (ulDrim == g_stBfdNni[usLoopi].ulDrim)/* �ж����� */
        {
            iRet = spm_nni_tree_set_data(usLoopi, g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel, &stBfdTreeNode);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {   
                /* ���FPGA��Ӧ�ķ��ͼĴ������� */
                spm_bfd_clear_nniId_fpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Set node failed, peerIp = %#x, pwLabel = %lu",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Set node failed, peerIp = %#x, pwLabel = %lu\r\n",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
                goto ERROR_LABEL;
            }

            /* ɾ���ڵ� */
            iRet = spm_oamtree_nodedel(g_usNniNoMax, &stBfdTreeNode, &g_stTreeNniNodeSet);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {   
                /* ���FPGA��Ӧ�ķ��ͼĴ������� */
                spm_bfd_clear_nniId_fpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Del node failed, peerIp = %#x, pwLabel = %lu",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Del node failed, peerIp = %#x, pwLabel = %lu\r\n",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
                goto ERROR_LABEL;
            }
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  

            /* �ؽ��Ự������ */
            iRet = spm_oamtree_build(g_usNniTreeLevel, &g_stTreeNniNodeSet, 
                g_pulNniTreebase, OAMNNIFPAGBASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {                
                /* ���FPGA��Ӧ�ķ��ͼĴ������� */
                spm_bfd_clear_nniId_fpgareg(usLoopi NBB_CCXT);
                NBB_TRC_FLOW((NBB_FORMAT"Build nni main Tree failed, peerIp = %#x, pwLabel = %lu",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build nni main Tree failed, peerIp = %#x, pwLabel = %lu\r\n",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
                goto ERROR_LABEL;
            }
            
            /* ��ҳд���� */
            spm_oamtree_s16write(OAMNNIDATAFPAGBASE, NNITREESWITCHOFFSET, 1);
            
            //spm_oamtree_switch(OAMFWDSRCFPAGBASE, FWDTREESWITCHOFFSET);  
            iRet = spm_oamtree_build(g_usNniTreeLevel, &g_stTreeNniNodeSet, 
                g_pulNniTreebase, OAMNNIFPAGBASE);
            if (SUCCESS != iRet)/* �ж�ִ�н�� */
            {
                NBB_TRC_FLOW((NBB_FORMAT"Build nni back Tree failed, peerIp = %#x, pwLabel = %lu",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
                BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Build nni back Tree failed, peerIp = %#x, pwLabel = %lu\r\n",
                    g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
                goto ERROR_LABEL;
            }
            spm_oamtree_s16write(OAMNNIDATAFPAGBASE, NNITREESWITCHOFFSET, 1);
            
            /* ���Ự��Ϣ��Ϊ��Чֵ */
            if (usLoopi < BFD_MAX_NNINUM)
            {
                g_stBfdNni[usLoopi].ulDrim = DRV_ULONG_INVALID;
                g_stBfdNni[usLoopi].ulIpv4 = DRV_ULONG_INVALID;
                g_stBfdNni[usLoopi].ulPwLabel = DRV_ULONG_INVALID;
                g_stBfdNni[usLoopi].ulNniId = DRV_ULONG_INVALID;
            }

            /* ���FPGA��Ӧ�ķ��ͼĴ������� */
            spm_bfd_clear_nniId_fpgareg(usLoopi NBB_CCXT);
            
            break;
        }
    }

    if (BFD_MAX_NNINUM == usLoopi)/* �ж����� */
    {
        NBB_TRC_FLOW((NBB_FORMAT"Not exit, peerIp = %#x, pwLabel = %lu",
            g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel));
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "ulDrim%lu Not exit, peerIp = %#x, pwLabel = %lu\r\n",
            ulDrim, g_stBfdNni[usLoopi].ulIpv4, g_stBfdNni[usLoopi].ulPwLabel);
        goto EXIT_LABEL;
    }
    
    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Delete nni end\r\n");
    
ERROR_LABEL:
    /* ���Ự��Ϣ��Ϊ��Чֵ */
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
 �� �� ��  : spm_bfd_nniId_fpgareg
 ��������  : ##
 �������  : IN NBB_USHORT usLocalNum  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_nniId_fpgareg(IN NBB_USHORT usLocalNum)
{
    NBB_ULONG           usData1     = 0;
    NBB_INT     iRet = ERROR;

    /* ����ж� */
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
 �� �� ��  : spm_bfd_clear_nniId_fpgareg
 ��������  : �Ựɾ��ʱ���FPGA��ת���Ĵ�������
 �������  : IN NBB_USHORT usLocalNum :������Ŀ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��17��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
 �� �� ��  : spm_bfd_init_bus_map_table
 ��������  : ��ʼ�� SRC FPGA 0x930000 ~ 0x9353ff Ϊ0xffff���˺�����ҵ������Ϊ׮������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��15��
    ��    ��   : tianf
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_bfd_state_map_init(NBB_CXT_T NBB_CXT)
{
    NBB_USHORT usSlotid = 0;
    NBB_USHORT usLoopi = 0;
    NBB_ULONG  ulBaseAddress = 0;
    NBB_USHORT usDate = 0x5400;

    NBB_TRC_ENTRY(__FUNCTION__);
    
    /* 1��21��λ����*/
    for(usSlotid = 1; usSlotid <= SYS_MAX_SLOTNUM + 8; usSlotid++)
    {
        ulBaseAddress = (usSlotid - 1)*0x400 + 0x30000;
        for(usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM + 1; usLoopi++)
        {
            spm_oamtree_s16write(BFDSTATEBASE, (ulBaseAddress + usLoopi), DRV_USHORT_INVALID);        
        }
    }

    /*22��λ  �̶�д 0x5400~ 0x57ff*/
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
 * �� �� ��  : spm_bfd_init_fpga
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��11��14��
 * ��������  : ��ʼ��BFD ��ص�FPGA�Ĵ���
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : NBB_VOID  ��
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_VOID spm_bfd_init_fpga(NBB_VOID)
{
    /* ��ʼ��BFD�澯ͬ��ӳ��� */
    spm_proc_bfd_state_map_init();
    return;
}


/*****************************************************************************
 �� �� ��  : spm_bfd_init_fwdtable
 ��������  : ת�������ʼ�����˺�������������Ϊ׮������ҵ���̲���ת�����֧��2047��
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��31��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
 �� �� ��  : spm_bfd_syn_sessionid
 ��������  : �����̺������ж�ID�Ƿ����仯�����仯��д��FPGA�Ĵ���
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��4��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���
*****************************************************************************/
NBB_INT  spm_bfd_syn_sessionid(NBB_VOID)
{
    NBB_BYTE    ucLocalSlot = 0;
    NBB_BYTE    ucMainBoardId = 0;
    NBB_USHORT  usLoopi = 0;
    
    /* ��ǰ��Ϊ���������� */
    ucMainBoardId = spm_hardw_getemustate();
    (NBB_VOID)spm_bfd_hardw_getslot(&ucLocalSlot);
    if (ucMainBoardId == ucLocalSlot)
    {
        /* �����������ҵ���̲�ͬ��������ֱ�ӽ����̴���ĻỰID���м�¼��д��FPGA */
        if (TRUE == g_bIsModifySession)
        {
            for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)/* ѭ�� */
            {
                g_stBfdIdStore[ucLocalSlot - 1].stBfdIdInfo[usLoopi].ulDiscrimer = 
                    g_stBfdlocal[usLoopi].ulDiscrimer;          /* ʹ��ȫ�ֱ�������Ϊ�˴�ӡ���Է��� */
                g_stBfdlocal[usLoopi].bIsSendtoMaster = TRUE;
            }

            /* ������IDд��FPGA�Ĵ��� */
            spm_bfd_id_fpgareg(ucLocalSlot, FALSE);

            /* ��ɲ�������Ҫ��գ���ֹ�´α�����д */
            g_bIsModifySession = FALSE;
            
            BMU_SLOG(BMU_DEBUG, SPM_BFD_LOG_DIR, "write local bfd id to fpga success\r\n");
        }
    }
    
    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_synrecv_sessionid
 ��������  : �����̽���ҵ����ͬ����ID��Ϣ
 �������  : IN NBB_VOID *pBuf  :���յ��ı���
             IN NBB_ULONG ulBufLength :���յ��ı��ĳ���  
             IN USHORT usSlotid :ͬ����Ϣ�Ĳ�λ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��5��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : ����MTLҪ���޸�
*****************************************************************************/
NBB_INT spm_bfd_synrecv_sessionid(
    IN NBB_VOID *pBuf, 
    IN NBB_ULONG ulBufLength, 
    IN NBB_USHORT usSlotid)
{
    NBB_USHORT     usLoopi = 0;         /* ѭ������ */
    NBB_BYTE   *pucTmpInfo = NULL;
    SPM_BFD_SYN_BFD_ID     *pusTmp     = NULL;
    NBB_INT     iRet = ERROR;
    NBB_CHAR *pcLinxData = NULL;
    MTL_NODE mtl_head = {0};
    NBB_BYTE ucLocalSlot = 0;
    NBB_BYTE ucMustate = 0;
    
    /* ����ж� */
    if ((NULL == pBuf) || (0 == ulBufLength))
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Master Board Syn rev Id, Input is NULL\r\n");
        goto EXIT_LABEL;
    }

    if ((usSlotid > SYS_MAX_SLOTNUM) || (0 == usSlotid))/* �ж����� */  /* TODO��SYS_MAX_SLOTNUM��Ҫ���ݲ�ͬ�ĵ��̸ı��С */
    {
        BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR, "Master Board Syn rev Id, Slotid = %d is error\r\n", usSlotid);
        goto EXIT_LABEL;
    }

    pucTmpInfo = (NBB_BYTE *)pBuf;
    
    pusTmp = (SPM_BFD_SYN_BFD_ID *)pucTmpInfo;
    
    /* ��ѯ���� */
    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        /* һ�η���1k������ */
        g_stBfdIdStore[usSlotid - 1].stBfdIdInfo[usLoopi].ulDiscrimer = pusTmp->arrucBfdId[usLoopi];  
    }

    /* ��ID����д��FPGA */
    spm_bfd_id_fpgareg(usSlotid, TRUE);
    
    BMU_SLOG(BMU_DEBUG, SPM_BFD_LOG_DIR, 
             "write slot %u bfd id to fpga success, G_SeqId = %lu\r\n", usSlotid, g_ulSequenceID);

    iRet = spm_bfd_hardw_getslot(&ucLocalSlot);    /* �õ����̲�λ�� */
    if ( SUCCESS != iRet ) /* ��ȡ���̲�λʧ�� */
    {
        BMU_SLOG(BMU_ERR, SPM_BFD_LOG_DIR, "get local slot error:%d\r\n", iRet);
        goto EXIT_LABEL;
    }

    ucMustate = spm_hardw_getemustate();        /* ��ȡ�������ز�λ */
    if ( 0xff == ucMustate ) /* ��0xff == ucMustate����ʱִ�� */
    {
        BMU_SLOG(BMU_ERR, SPM_BFD_LOG_DIR, "get mustate error:%d\r\n", ucMustate);
        goto EXIT_LABEL;
    }

    if (ucLocalSlot == ucMustate)   /* ֻ���������ز���Ҫ�ذ� */
    {
        if (0 == spm_hardw_getboardtype(usSlotid))
        {
            BMU_SLOG(BMU_ERR, SPM_BFD_LOG_DIR, "error master send to itself, src slot:%d\r\n", usSlotid);
            goto EXIT_LABEL;
        }
        /* ����Buf��ҵ���̣���ӦID��Ϣ */
        pcLinxData = malloc(g_ulSynIdInfoLth);
        if (NULL == pcLinxData)//�ڴ治��
        {
            goto EXIT_LABEL;
        }
        pucTmpInfo = (NBB_BYTE *)pcLinxData;
#if 0
        /* �鷢������CPU��ͷ */
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.module_id = TYPE_MTL_BFD;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.frame = g_stDesNode.frame;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.slot = usSlotid;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->des_head.cpu = 0;
#endif

        /* ��ͬ�����ݵ�ͷ */
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->syn_head.ucType = BFD_FWD_MSG_ID;
        ((SPM_BFD_SYN_SEND_PKT_HEAD*)pucTmpInfo)->syn_head.ulSrcSlot = g_stSrcNode.slot;

        /* ��������� */
        pucTmpInfo = pucTmpInfo + sizeof(SPM_BFD_SYN_SEND_PKT_HEAD);
        memcpy(pucTmpInfo, pBuf, ulBufLength);
        
        /* 2015.5.21����mtl��ʽ���ͺ󣬱���λֱ�ӷ�������CPU,����Ҫ�ȷ���������ͨ�����ط�������CPU, 
          �����޸Ľ�����֮ǰlinx��������ϣ��˴�������Ϣ��ȥ��MTL_NODE,����mtl��˵��Ϊ����*/
        mtl_send(mtl_identity_to_fd(1 << 24 | usSlotid << 16 | 0),
            TYPE_MTL_BFD, TYPE_MTL_BFD, 
        pcLinxData + sizeof(MTL_NODE), 
        g_ulSynIdInfoLth - sizeof(MTL_NODE));
        free(pcLinxData);
        pcLinxData = NULL;
        
        #if 0
        /* Ŀ�ĵ�ַ */
        g_stDesNode.module_id = TYPE_MTL_TNE;
        g_stDesNode.slot = spm_hardw_getemustate(); /* �����������أ�����������ת�� */
        g_stDesNode.cpu = 1;                        /* ���͵����ص�CPU,��������ת�������CPU */
        
        iRet = pal_mtl_send(g_stSrcNode, (void **)(&pcLinxData), g_stDesNode);
        if (SUCCESS != iRet)
        {
            BMU_LOG(BMU_INFO, SPM_BFD_LOG_DIR, "mtl send to slot %u error = %d, G_SeqId = %lu\r\n", usSlotid, iRet, g_ulSequenceID);
            goto EXIT_LABEL;
        }
       #endif

        /* ����ʧ�� */
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
 �� �� ��  : spm_bfd_sendpacket_macreg
 ��������  : ���������ö�Ӧmac�ļĴ���
 �������  : IN NBB_USHORT usLocalNum : :������Ŀ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��14��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
    
    /* �ж���� */
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
    
    /* LSP����� */
    if (BFD_FWD_LSPID == g_stBfdlocal[usLocalNum].ucBfdFwdType) 
    {
        usData1 = g_stBfdlocal[usLocalNum].usLspID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }
    else if (BFD_FWD_TUNNELID == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* tunnel��� */
    {
        usData1 = g_stBfdlocal[usLocalNum].usTunnelID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);
    }
    else if (BFD_FWD_IP == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* ip��� */
    {
        usData1 = g_stBfdlocal[usLocalNum].usVrfid;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x3, usData1);

        usData2 = (g_stBfdlocal[usLocalNum].ulDip[3] >> 16)&DRV_USHORT_INVALID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData2);

        usData3 = g_stBfdlocal[usLocalNum].ulDip[3]&DRV_USHORT_INVALID;
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData3);
    }
    else if (BFD_FWD_PORT == g_stBfdlocal[usLocalNum].ucBfdFwdType)/* interface��� */
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
    else if (BFD_FWD_PW == g_stBfdlocal[usLocalNum].ucBfdFwdType)       /* PW��� */
    {
        stBfdPwSmac2.usPeerIpH = (g_stBfdlocal[usLocalNum].ulPeerId >> 16) & DRV_USHORT_INVALID;
        usData2 = *(NBB_USHORT *)(&stBfdPwSmac2);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x2, usData2);

        stBfdPwSmac3.usPeerIpL = g_stBfdlocal[usLocalNum].ulPeerId & DRV_USHORT_INVALID;
        usData3 = *(NBB_USHORT *)(&stBfdPwSmac3);
        spm_oamtree_s16write(BFDPAKFGPABASE, 0x12000 + 4 * usLocalNum + 0x3, usData3);
    }

    /* ��װITMHͷ */
    stBfdFwd1.usFwdTrafficClass = 7; /* ������arad���ȼ� */
    stBfdFwd1.usFwdDesInfo1 = 0xc;
    usData4 = *(NBB_USHORT *)(&stBfdFwd1);
    spm_oamtree_s16write(BFDPAKFGPABASE, 0x2000 + 4 * usLocalNum + 0x1, usData4);

    /* ��ȡ��ǰ��λ�ĵ�һ��SPU�̵Ĳ�λ�� */
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
 �� �� ��  : spm_bfd_id_fpgareg
 ��������  : �����̽�ID���ղ�λ��д��FPGA
 �������  : NBB_USHORT usSlotid
             NBB_BOOL bIsSpu
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��26��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_id_fpgareg(IN NBB_USHORT usSlotid, IN NBB_BOOL bIsSpu)
{
    NBB_USHORT usLoopi = 0;
    NBB_ULONG  ulBaseAddress = 0;
    
    //NBB_TRC_ENTRY(("spm_bfd_id_fpgareg"));

    /* ����ж� */
    if ((usSlotid > SYS_MAX_SLOTNUM) || (0 == usSlotid))
    {
        goto EXIT_LABEL;
    }
    if (TRUE ==  bIsSpu)/* ҵ���� */
    {
        ulBaseAddress = (usSlotid - 1)*0x400 + 0x30000;
    }
    else 
    {
        /* �̶�д���21��λ��Ӧ�ĵ�ַ */
        ulBaseAddress = (21 - 1)*0x400 + 0x30000;
    }
    
    for (usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM + 1; usLoopi++)/* ѭ�� */
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
 �� �� ��  : spm_bfd_state_fpgareg
 ��������  : �����������̵ĻỰ״̬д�뱳������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��29��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_state_fpgareg(NBB_VOID)
{
    NBB_USHORT usLoopi = 0;
            
    /* �̶�д���Ӧ�ĵ�ַ */
    for (usLoopi = 0; usLoopi < BFD_MAX_SYN_SESNUM; usLoopi++)/* ѭ�� */
    {
        spm_oamtree_s16write(BFDSTATEBASE, 0x60000 + usLoopi, g_stBfdlocal[usLoopi].ucStateRecord);
    }
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_msgque_task
 ��������  : ����������SRC��׮
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��27��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_msgque_task(NBB_VOID)
{
    return;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_init_Nni_table
 ��������  : NNI�����ʼ�����˺�������������Ϊ׮������ҵ���̲���NNI���֧��1024��
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��31��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

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
 �� �� ��  : spm_bfd_delfwdtable
 ��������  : ɾ��ת������
 �������  : IN NBB_ULONG ulDrim  :������
             NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��15��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_del_nniId_table(NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY(("spm_bfd_del_nniId_table"));
    
EXIT_LABEL: NBB_TRC_EXIT();

    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_nniId_fpgareg
 ��������  : ##
 �������  : IN NBB_USHORT usLocalNum  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_nniId_fpgareg(IN NBB_USHORT usLocalNum)
{
    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_clear_nniId_fpgareg
 ��������  : �Ựɾ��ʱ���FPGA��ת���Ĵ�������
 �������  : IN NBB_USHORT usLocalNum :������Ŀ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��17��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_clear_nniId_fpgareg(IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CXT)
{
    return;
}

#endif

#ifdef SNU

/*****************************************************************************
 �� �� ��  : spm_bfd_init_fpga_map_table
 ��������  : ��ʼ�� SRC FPGA 0x930000 ~ 0x932fff Ϊ0xffff���˺�����ҵ������Ϊ׮������
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��15��
    ��    ��   : tianf
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_bfd_state_map_init(NBB_CXT_T NBB_CXT)
{
    return SUCCESS;
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_init_fpga
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��11��14��
 * ��������  : ��ʼ��BFD ��ص�FPGA�Ĵ���
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : NBB_VOID  ��
 * ���ù�ϵ  : 
 * ��    ��  : 

*****************************************************************************/
NBB_VOID spm_bfd_init_fpga(NBB_VOID)
{
    /* ��ʼ��BFD�澯ͬ��ӳ��� */
    spm_proc_bfd_state_map_init();
    return;
}

/*****************************************************************************
 * �� �� ��  : spm_bfd_init_fwdtable
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��11��14��
 * ��������  : ��ʼ��BFD ��fwdtable
 * �������  : ��
 * �������  : ��
 * �� �� ֵ  : NBB_VOID  ��
 * ���ù�ϵ  : 
 * ��    ��  : 

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
 �� �� ��  : spm_bfd_syn_sessionid
 ��������  : �����̺������ж�ID�Ƿ����仯�����仯��д��FPGA�Ĵ���
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��4��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���
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
 �� �� ��  : spm_bfd_init_Nni_table
 ��������  : NNI�����ʼ�����˺�������������Ϊ׮������ҵ���̲���NNI���֧��1024��
 �������  : NBB_VOID  
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��8��31��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_init_Nni_table(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY(("spm_bfd_init_fwdtable"));
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_add_nniId_table
 ��������  : ..
 �������  : NBB_ULONG ulDrim                     
             NBB_ULONG ulPeerIp                   
             NBB_ULONG ulPwLabel                  
             NBB_ULONG ulNniIdNBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

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
 �� �� ��  : spm_bfd_delfwdtable
 ��������  : ɾ��ת������
 �������  : IN NBB_ULONG ulDrim  :������
             NBB_CCXT_T NBB_CCXT  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��15��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_del_nniId_table(NBB_ULONG ulDrim NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY(("spm_bfd_del_nniId_table"));
    
EXIT_LABEL: NBB_TRC_EXIT();

    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_nniId_fpgareg
 ��������  : ..
 �������  : IN NBB_USHORT usLocalNum  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��5��21��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bfd_nniId_fpgareg(IN NBB_USHORT usLocalNum)
{
    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_bfd_clear_nniId_fpgareg
 ��������  : �Ựɾ��ʱ���FPGA��ת���Ĵ�������
 �������  : IN NBB_USHORT usLocalNum :������Ŀ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��17��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_clear_nniId_fpgareg(IN NBB_USHORT usLocalNum NBB_CCXT_T NBB_CXT)
{
    return;
}
#endif

/*****************************************************************************
 �� �� ��  : spm_bfd_fwdpacket_receive
 ��������  : ���̽��մ�������ת������YOURIDΪ0�����ְ�
 �������  : IN NBB_BYTE *pBuf  
             IN NBB_ULONG ulMsgLenth
             IN NBB_USHORT usSlotid
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��7��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���
 ��ע          : �յ������̵�����down���ģ�˵���������Ѿ���ѯ��ȫ�������ˣ��ܹ�ƥ�䣬
                 ��ô������ֻ��Ҫƥ�䱾�����þͿ����ˣ���ƥ�䲻�ɹ������������δ�£�
                 ��������

*****************************************************************************/
NBB_INT spm_bfd_fwdpacket_receive(IN NBB_BYTE *pBuf, IN NBB_ULONG ulMsgLenth, IN NBB_USHORT usSlotid)
{
    NBB_USHORT     usLoopi = 0;         /* ѭ������ */
    NBB_USHORT     usVrfid = 0;
    NBB_BYTE   ucSessionState = 0;     /* ���ֱ��ĵ�״̬ */
    NBB_INT    iRet = ERROR;
    minm_header_t *pstMacInMac = NULL;
    NBB_LONG       lLenthTmp = 0;
    SPM_BFD_PACKET_ABSTRACT stPacketInfo;
    
    //NBB_TRC_ENTRY(("spm_bfd_fwdpacket_receive"));
    OS_ASSERT(NULL != pBuf);

    
    OS_MEMSET(&stPacketInfo, 0, sizeof(SPM_BFD_PACKET_ABSTRACT));

    lLenthTmp = ulMsgLenth - 28; /* ƫ�Ƶ�ǰ���mac in mac */
    BFD_PACKETLEN_JUDGE(lLenthTmp);
    
    /* ����DIP��SIP����ȡVPNID����ȡ�����е�MYID */
    pstMacInMac = (minm_header_t *)pBuf;
    
    /* �ж϶˿����� */
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
        
    /* ֱ���ڱ��̵�������ƥ�䣬����Ҫ��ȫ��������ƥ�� */
    if (BFD_IPV4 == stPacketInfo.ucIpType)                  /* ��Ҫ��CPUѧϰ��������ת���������̵�ֻ��BFD FOR IP */
    {
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            /* �ж����� */
            if ((usVrfid == g_stBfdlocal[usLoopi].usVrfid) 
                && (stPacketInfo.ulDip[0] == g_stBfdlocal[usLoopi].ulSip[3])
                && (stPacketInfo.ulSip[0] == g_stBfdlocal[usLoopi].ulDip[3]))
            {
                if ((0 == g_stBfdlocal[usLoopi].ulRemoteDiscr) 
                    || (stPacketInfo.ulMyDrim != g_stBfdlocal[usLoopi].ulRemoteDiscr))
                {
                    /* ��ʱ��Ҫ���FPGA���ϴ�ѧϰ���������� */
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x0,0);
                    spm_oamtree_s16write(BFDSTUDYBASE, 2 * usLoopi + 0x1,0);
                    g_stBfdlocal[usLoopi].ulRemoteDiscr = stPacketInfo.ulMyDrim;
                    
                    BMU_SLOG(BMU_INFO, SPM_BFD_LOG_DIR,
                        "The %u bfd learn discr from c3, my_id %ld, yourid %ld, "
                        "local ip %#x remote ip %#x, vrf %d\r\n", 
                        usLoopi, g_stBfdlocal[usLoopi].ulDiscrimer, stPacketInfo.ulMyDrim, 
                        stPacketInfo.ulDip[0], stPacketInfo.ulSip[0], usVrfid);
                    
                    /* ��ѧϰ����������д��Ĵ��� */
                    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                    
                    g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;

                    /* ��״̬�� */
                    spm_bfd_state_fsm(usLoopi, ucSessionState);
                }

                /* ��¼ÿ���̽��յ�ת�����ļ�¼ */
                spm_bfd_fwd_rxrecord(usSlotid ,g_stBfdlocal[usLoopi].ulLocalDiscr);
                
                break;  /* ���˾Ͳ������ˣ���Լʱ�䣬ͬʱΪ�����ж��Ƿ��ҵ��ṩ���� */
            }
        }
    }

    if (BFD_IPV6 == stPacketInfo.ucIpType)
    {
        for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
        {
            /* �ж����� */
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
                    /* ��ʱ��Ҫ���FPGA���ϴ�ѧϰ���������� */
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
                    
                    /* ��ѧϰ����������д��Ĵ��� */
                    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x0��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x0, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr >> 16) & DRV_USHORT_INVALID);
                    /* BFD����ģ��Ĵ���ƫ��0xc000+2*N+0x1��Bit15-0��BFD����Զ�˻Ự��ʶ����Your Discriminator������16λ�� */
                    spm_oamtree_s16write(BFDPAKFGPABASE, 0xc000 + 2 * usLoopi + 0x1, 
                        (g_stBfdlocal[usLoopi].ulRemoteDiscr) & DRV_USHORT_INVALID);
                    
                    g_stBfdlocal[usLoopi].usBfdflag = BFD_NORMAL;
                    spm_bfd_state_fsm(usLoopi, ucSessionState);
                }

                spm_bfd_fwd_rxrecord(usSlotid ,g_stBfdlocal[usLoopi].ulLocalDiscr);
                
                break;  /* ���˾Ͳ������ˣ���Լʱ�䣬ͬʱΪ�����ж��Ƿ��ҵ��ṩ���� */
            }
        }
    }

    if (BFD_MAX_SESNUM == usLoopi)/* �ж����� */
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
 �� �� ��  : spm_bfd_fwd_txrecord
 ��������  : ÿ����ת���ı��ļ�¼
 �������  : IN NBB_USHORT usDesSlotid:Ŀ�Ĳ�λ��
             IN NBB_ULONG ulDrim  :ת���ı��ĵ���������ת������down 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��10��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_fwd_txrecord(IN NBB_USHORT usDesSlotid, IN NBB_ULONG ulDrim)
{
    NBB_USHORT usRecordNum = 0; 

    /* ����ж� */
    if ((usDesSlotid > SYS_TOTAL_SLOTNUM) || (0 == usDesSlotid))
    {
        return;
    }
    
    usRecordNum = (g_stFwdTxRec[usDesSlotid - 1].ulNum) % BFD_MAX_FWDRECORDNUM;/* ����¼128�� */
    g_stFwdTxRec[usDesSlotid - 1].ulNum++;
    g_stFwdTxRec[usDesSlotid - 1].aulDiscrimer[usRecordNum] = ulDrim;
    return;    
}

/*****************************************************************************
 �� �� ��  : spm_bfd_fwd_rxrecord
 ��������  : ÿ���̽��յı��ļ�¼
 �������  : IN NBB_USHORT usSourceSlotid :Դ��λ�� 
             IN NBB_ULONG ulDrim      :������
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��9��10��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_bfd_fwd_rxrecord(IN NBB_USHORT usSourceSlotid, IN NBB_ULONG ulDrim)
{
    NBB_USHORT usRecordNum = 0;
    
    /* ����ж� */
    if ((usSourceSlotid > SYS_TOTAL_SLOTNUM) || (0 == usSourceSlotid))
    {
        return;
    }
    
    usRecordNum = (g_stFwdRxRec[usSourceSlotid - 1].ulNum) % BFD_MAX_FWDRECORDNUM;/* ����¼128�� */
    g_stFwdRxRec[usSourceSlotid - 1].ulNum++;
    g_stFwdRxRec[usSourceSlotid - 1].aulDiscrimer[usRecordNum] = ulDrim;
    return;    
}


/*****************************************************************************
 �� �� ��  : spm_proc_bfd_packet
 ��������  : CPU����C3������BFD���ģ����Ļ�ת��������ͷ��ڴ�(������)
 �������  : IN SPM_THREAD_SOCKET_CB *thread_cb  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��2��1��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_proc_bfd_packet (IN NBB_BYTE *buf,
                              IN NBB_ULONG len
                              NBB_CCXT_T NBB_CXT)
{
    NBB_BYTE *pBuf = NULL;  /* ָ���ĵ�ָ�� */  
    NBB_ULONG ulMsgLength = 0;
    
    /* ��μ�� */
    if (NULL == buf)
    {
        return;
    }

    ulMsgLength = len - SPM_TCP_RP_HEAD_DATA_SIZE;
    if (ulMsgLength < 94)   /* ���յ���BFD���ģ���������Ϊ28�ֽ�macinmac+ipv4bfd66�ֽ�(����vlantag��) */
    {
        NBB_EXCEPTION((PCT_SPM | 26, 1, "lu", len));
        return;
    }

    pBuf = buf + SPM_TCP_RP_HEAD_DATA_SIZE; /* ƫ�Ƶ����ľ��ɴ� */
    spm_bfd_packet_receive(pBuf, ulMsgLength);
    return;
    
}

/*****************************************************************************
 �� �� ��  : spm_bfd_packet_analysis
 ��������  : ����BFD���ĺ���
 �������  : IN NBB_BYTE *pBuf : ��̫������(��cpu��bfd���ģ�ȥ��ǰ28�ֽڵ�mac in mac)                                             
             IN NBB_LONG lMsgLenth : �����ܳ���                                       
 �������  : OUT SPM_BFD_PACKET_ABSTRACT *pstPacketInfo 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��2��28��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

  2.��    ��   : 2015��6��18��
    ��    ��   : gaos
    �޸�����   : ��ӵ�����Ϣ
*****************************************************************************/
NBB_INT spm_bfd_packet_analysis(
    IN NBB_BYTE *pBuf, 
    IN NBB_LONG lMsgLenth, 
    OUT SPM_BFD_PACKET_ABSTRACT *pstPacketInfo)
{
    NBB_LONG       lLenthTmp = 0;
    NBB_ULONG      ulOffset  = 0;       /* �ӱ���ͷ��IP��ƫ�Ƴ��� */
    NBB_USHORT     usEthtype = 0;       /* �ж��Ƿ��vlantag */
    SPM_BFD_MPLSLABEL *pstMplsLabel = NULL;
    NBB_BYTE   ucIpType;
        
    OS_ASSERT(NULL != pBuf);
    OS_ASSERT(NULL != pstPacketInfo);

    /* ƫ�Ƶ�12�ֽڵ�mac */
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
    else if (0x8100 == usEthtype)/* vlan��� */
    {
        lLenthTmp = lLenthTmp - 4;
        BFD_PACKETLEN_JUDGE(lLenthTmp);
        ulOffset = ulOffset + 4;
        usEthtype = *(NBB_USHORT *)(pBuf + ulOffset);
        if (0x8100 == usEthtype)/* QinQ��� */
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
 �� �� ��  : spm_search_bfdId_by_vckey
 ��������  : ����VCKEY����ѯ��Ӧ��BFDID
 �������  : IN ATG_DCI_VC_KEY *pstVCKey : vc key 
 �������  : OUT NBB_USHORT *pulBfdId : BFD ID
 �� �� ֵ  : SUCCESS��ѯ�ɹ�����ʹ�÷��ص�idֵ
             ERROR��ѯ���ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��20��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_search_bfdId_by_vckey(IN ATG_DCI_VC_KEY *pstVCKey, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;

    /* ��nbase�߳�ʹ��nbase�ڲ�ʹ��NBB_CXT�����ĵ�APIʱ��ƭ�������� */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
    
    /* ��μ�� */
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
 �� �� ��  : spm_search_bfdId_by_ldpkey
 ��������  : ����VCKEY����ѯ��Ӧ��BFDID
 �������  : IN ATG_DCI_BFD_LDP_LSP_DATA *pstldpKey : ldp key 
 �������  : OUT NBB_USHORT *pulBfdId : BFD ID
 �� �� ֵ  : SUCCESS��ѯ�ɹ�����ʹ�÷��ص�idֵ
             ERROR��ѯ���ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��20��
    ��    ��   : limingming
    �� �� ��   : #
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_search_bfdId_by_ldpkey(IN ATG_DCI_BFD_LDP_LSP_DATA *pstldpKey, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;

    /* ��nbase�߳�ʹ��nbase�ڲ�ʹ��NBB_CXT�����ĵ�APIʱ��ƭ�������� */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
    
    /* ��μ�� */
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
 �� �� ��  : spm_search_bfdId_by_rsvpkey
 ��������  : ����VCKEY����ѯ��Ӧ��BFDID
 �������  : IN ATG_DCI_BFD_RSVP_LSP_DATA *pstrsvpKey : rsvp key 
 �������  : OUT NBB_USHORT *pulBfdId : BFD ID
 �� �� ֵ  : SUCCESS��ѯ�ɹ�����ʹ�÷��ص�idֵ
           ERROR��ѯ���ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
 1.��    ��   : 2013��6��20��
  ��    ��   : limingming
  �� �� ��   : #
  �޸�����   : �����ɺ���
 
*****************************************************************************/
NBB_INT spm_search_bfdId_by_rsvpkey(IN ATG_DCI_BFD_RSVP_LSP_DATA *pstrsvpKey, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;

    /* ��nbase�߳�ʹ��nbase�ڲ�ʹ��NBB_CXT�����ĵ�APIʱ��ƭ�������� */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
    
    /* ��μ�� */
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
 �� �� ��  : spm_search_bfdId_by_rsvpkey
 ��������  : ����IPKEY����ѯ��Ӧ��BFDID
 �������  : IN VRFUROUTE_KEY *pstipKey : ip key 
 �������  : OUT NBB_USHORT *pulBfdId : BFD ID
 �� �� ֵ  : SUCCESS��ѯ�ɹ�����ʹ�÷��ص�idֵ
           ERROR��ѯ���ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
 1.��    ��   : 2013��6��20��
  ��    ��   : limingming
  �� �� ��   : #
  �޸�����   : �����ɺ���
 
*****************************************************************************/
NBB_INT spm_search_bfdid_by_ipkey(IN VRFUROUTE_KEY * pstipKey, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;
    NBB_ULONG ulDipTmp1 = 0;
    NBB_ULONG ulDipTmp2 = 0;
    NBB_BYTE  ucMaskLen = 0;

    NBB_INT iRet = ERROR;

    /* ��nbase�߳�ʹ��nbase�ڲ�ʹ��NBB_CXT�����ĵ�APIʱ��ƭ�������� */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
        
    /* ��μ�� */
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

    if (0 == pstipKey->addrtype) /* IPV4���� */
    {
        if (ucMaskLen > 32)
        {
            iRet =  ERROR;
            goto EXIT_LABEL;
        }

        ulDipTmp2 = (pstipKey->dip[3] >> (32 - ucMaskLen));
        /* IP��ַΪ0,��ʾ����������� */
        if ( 0 == ulDipTmp2 ) /* IP��ַΪ0 */
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
                /* ulDipTmp2��ֵ��ȡ��forѭ���⣬���Ч�� */
                if (ulDipTmp1 == ulDipTmp2)
                {
                    *pulBfdId = pstBfdCfg->ulDiscrimer;
                    iRet =  SUCCESS;
                    goto EXIT_LABEL;
                }
            }
        }
    }
    else if (1 == pstipKey->addrtype) /* IPV6���� */
    {
        if (ucMaskLen > 128)
        {
            iRet =  ERROR;
            goto EXIT_LABEL;
        }

        /* IP��ַΪ0,��ʾ����������� */
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

        /* ��������λ���õ����4���ֽ����� */
        
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
                        /* ulDipTmp2��ֵ��ȡ��forѭ���⣬���Ч�� */
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
                        /* ulDipTmp2��ֵ��ȡ��forѭ���⣬���Ч�� */
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
                        /* ulDipTmp2��ֵ��ȡ��forѭ���⣬���Ч�� */
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
                    /* ulDipTmp2��ֵ��ȡ��forѭ���⣬�ṩЧ�� */
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
 �� �� ��  : spm_search_bfdid_by_portkey
 ��������  : ����PORTKEY����ѯ��Ӧ��BFDID
 �������  : IN VRFUROUTE_KEY *pstrsvpKey : port key 
 �������  : OUT NBB_USHORT *pulBfdId : BFD ID
 �� �� ֵ  : SUCCESS��ѯ�ɹ�����ʹ�÷��ص�idֵ
           ERROR��ѯ���ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
 1.��    ��   : 2013��6��20��
  ��    ��   : limingming
  �� �� ��   : #
  �޸�����   : �����ɺ���
 
*****************************************************************************/
NBB_INT spm_search_bfdid_by_portkey(IN NBB_ULONG *pulPortIndex, OUT NBB_ULONG *pulBfdId)
{
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;

    /* ��nbase�߳�ʹ��nbase�ڲ�ʹ��NBB_CXT�����ĵ�APIʱ��ƭ�������� */
    NBB_NULL_THREAD_CONTEXT;
    NBB_GET_THREAD_CONTEXT();
    
    /* ��μ�� */
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
 * �� �� ��  : spm_bfd_notify_aps_del
 * �� �� ��  : ����,jiangbo
 * ��������  : 2014��5��28��
 * ��������  : ֪ͨ����ģ��ɾ������
 * �������  : NBB_ULONG ulDiscrimer      ����������
               NBB_BYTE ucFramtype        BFD����
               NBB_CCXT_T NBB_CXT         NBASE���̲߳���
 * �������  : ��
 * �� �� ֵ  : NBB_INT  ������
 * ���ù�ϵ  : 
 * ��    ��  : 

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
        iRet = spm_del_bfd_cfg(LDP_LSP_BFD_CHANGE, ulDiscrimer);/* ����ģ����Ҫ��ѯldp��rsvp�ĻỰ */
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
 �� �� ��  : spm_bfd_tree_get_fpga_data
 ��������  : BFD �Ự���������ұ�Ĵ���bitλ��ֵ
 �������  : IN SPM_OAMTREE_NODE* oamTreeNode     
             OUT SPM_FPGATREE_DATA *fpgaTreeDate  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��5��28��
    ��    ��   : gaos
    �޸�����   : �����ɺ���

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

    fpgaTreeDate->num = 4;  /* ��Ч�ֽ�Ϊ4�ֽ� */

    /* 8.28.1	�Ự���������ұ�Ĵ������� ����ַ 0xea060000+0x4000*2�� N= 0~7feH */
    /*
    ����[63��48]����Ŀ�����ţ�
    ����[35:24]��MYID��12λ���أ�
    ����[19:0]��MYID��20λ���ء�
    */
    fpgaTreeDate->usData.bfdTable.index = oamTreeNode->usLspNo;
    fpgaTreeDate->usData.bfdTable.myId_31to28b = (oamTreeNode->ulTunnelId >> 28) & 0xf; //��12bit�еĸ�4bit
    fpgaTreeDate->usData.bfdTable.myId_27to20b = (oamTreeNode->ulTunnelId >> 20) & 0xff;//��12bit�еĵ�8bit
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

    fpgaTreeDate->num = 4;  /* ��Ч�ֽ�Ϊ4�ֽ� */
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

    fpgaTreeDate->num = 4;  /* ��Ч�ֽ�Ϊ4�ֽ� */
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

