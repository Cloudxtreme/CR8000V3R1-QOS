/******************************************************************************

                  °æÈ¨ËùÓÐ (C), 1999-2013, ·é»ðÍ¨ÐÅ¿Æ¼¼¹É·ÝÓÐÏÞ¹«Ë¾

******************************************************************************
   ÎÄ ¼þ Ãû   : spm_logical_port_proc.c
   °æ ±¾ ºÅ   : ³õ¸å
   ×÷    Õß   : xiaoxiang
   Éú³ÉÈÕÆÚ   : 2012Äê9ÔÂ18ÈÕ
   ×î½üÐÞ¸Ä   :
   ¹¦ÄÜÃèÊö   : ¶Ë¿ÚÂß¼­ÅäÖÃ´¦Àí
   º¯ÊýÁÐ±í   :
   ÐÞ¸ÄÀúÊ·   :
   1.ÈÕ    ÆÚ   : 2012Äê9ÔÂ18ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ´´½¨ÎÄ¼þ

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef LOGICAL_PORT_CFG

extern unsigned char logical_port_cfg_print_setting;

/*****************************************************************************
   º¯ Êý Ãû  : spm_check_if_vip_vmac_exist
   ¹¦ÄÜÃèÊö  : ¼ì²éÒ»¸öÂß¼­¶Ë¿ÚÖÐÐéÄâMACºÍIPµØÖ·ÅäÖÃÊÇ·ñÓÐÖØ¸´
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pstVipVmac
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : 0 £ºÎÞÖØ¸´
             n : ÓÐÖØ¸´£¬n=Êý×éÎ»ÖÃ+1
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê5ÔÂ14ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_check_if_vipv6_vmac_exist(NBB_ULONG ulPortIndex, 
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *pst_vipv6_vmac NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;/*·µ»ØÖµ*/
    SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;

    NBB_TRC_ENTRY("spm_check_if_vipv6_vmac_exist");

    if (pst_vipv6_vmac == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_vipv6_vmac_exist(pstVipVmac==NULL)"));

        OS_PRINTF("***ERROR***:spm_check_if_vipv6_vmac_exist(pstVipVmac==NULL)\n");

        OS_SPRINTF(uc_message, "***ERROR***:spm_check_if_vipv6_vmac_exist(pstVipVmac==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", uc_message));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    pst_logical_port = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pst_logical_port == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_vipv6_vmac_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_vipv6_vmac_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);

        OS_SPRINTF(uc_message,
            "***ERROR***:spm_check_if_vipv6_vmac_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n",
            ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, LOGICAL_PORT_NOT_EXIST,
                "LOGICAL_PORT_NOT_EXIST", uc_message));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        if (pst_logical_port->vipv6_vmac_cfg_cb[i] == NULL)
        {
            continue;
        }
        else
        {
            //Èç¹ûÐéÄâMACºÍIPµØÖ·ÅäÖÃÏàµÈ£¬·µ»Ø´æ´¢Î»ÖÃ
            if ((spm_ipv6_mac_key_compare(pst_vipv6_vmac, pst_logical_port->vipv6_vmac_cfg_cb[i] NBB_CCXT)) == 0)
            {
                ret = i + 1;
                goto EXIT_LABEL;
            }
        }
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_rcv_dci_set_logical_port
   ¹¦ÄÜÃèÊö  : ¶Ë¿ÚÂß¼­ÅäÖÃ´¦Àí
   ÊäÈë²ÎÊý  : ATG_DCI_SET_LOGICAL_PORT *pstSetLogicalPort
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê10ÔÂ12ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_logical_port(ATG_DCI_SET_LOGICAL_PORT *pstSetLogicalPort NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;

    NBB_ULONG i = 0;
    NBB_ULONG j = 0;
    NBB_BYTE ucC3Unit = 0;
    NBB_INT ret = SUCCESS;
    NBB_INT iCfgPos = 0;
    NBB_INT v6cfgpos = 0;/* vrrp µÄ´æ´¢Î»ÖÃ*/
    NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    NBB_BYTE ucLocalSlot = 0;
    NBB_USHORT usFlowId = 0;
    NBB_ULONG ulVlanPosId = 0;
    NBB_ULONG ul_vlanposid2 = 0;  /* ÌõÄ¿µÄ¼üÖµ */
    NBB_BYTE ulAssociateIfKey = 0;

    INTF_T stIntfL3;
    INTF_T stIntfFlow; /* vrrpµÄ½á¹¹Ìå*/
    VRRP_ACL_T stvrrp; /* vrrpµÄ½á¹¹Ìå*/
    VRRP_ACL_T stvrrp_v6; /* vrrp ipv6µÄ½á¹¹Ìå*/
    NBB_BYTE ve_mac[6] = {0x00, 0x01, 0x002, 0x03, 0x04, 0x05};
    SPM_PORT_INFO_CB stPortInfo;
    SPM_VPN_PORT_INFO_CB stVpPortInfo;

    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;
    SPM_TERMINAL_IF_CB *tempterminalifcb = NULL;/* VLAN½Ó¿ÚÊ×µØÖ·*/
    SPM_TERMINAL_IF_CB *tempterminalifcb_v6 = NULL;/* VLAN½Ó¿ÚÊ×µØÖ·*/
    NBB_ULONG terminalifnum = 0;/* if num*/
    NBB_ULONG terminalifnum_v6 = 0;/* if num*/
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA stLogicL2Data;
    ATG_DCI_VPWS_UNI_DATA stVpwsUni;
    ATG_DCI_VPLS_UNI_DATA stVplsUni;
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV stFlowDiffServDel;
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA stDiffServDel;
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS stInclassifyQosDel;
    ATG_DCI_LOG_UP_USER_QOS_POLICY stUpUserQosDel;
    ATG_DCI_LOG_FLOW_UP_USER_QOS stFlowUpUserQosDel;
    SPM_ASSOCIATE_IF_CB *pstAssociateIfCb = NULL;
    SPM_DIFF_SERV_CB *pstdiffservcb = NULL;    
    SPM_FLOW_DIFF_SERV_CB *pstflowdiffservcb = NULL;  
    SPM_INCLASSIFY_QOS_CB *pstinclassifyqoscb = NULL;     
    SPM_TRAFFIC_FILTER_CB *psttrafficfiltercb = NULL; 
    
    /* »ñÈ¡µÄ×ÓÅäÖÃ */
    ATG_DCI_LOG_PORT_BASIC_DATA *pstBasicData = NULL;
    ATG_DCI_LOG_PORT_PHY_DATA *pstPhyData = NULL;
    ATG_DCI_LOG_PORT_LOGIC_L3_DATA *pstLogicL3Data = NULL;
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *pstLogicL2Data = NULL;
    ATG_DCI_LOG_PORT_FLOW_DATA *pstFlowData = NULL;
    ATG_DCI_LOG_PORT_CES_DATA *pstCesData = NULL;
    ATG_DCI_LOG_PORT_VE_MAC_DATA *pstVeMacData = NULL;
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *pstDiffServData = NULL;
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *pstInclassifyQosData = NULL;
    ATG_DCI_LOG_UP_USER_QOS_POLICY *pstUpUserQosData = NULL;
    ATG_DCI_LOG_UP_USER_GROUP_QOS *pstUpGroupQosData = NULL;
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *pstDownUserQosData = NULL;
    ATG_DCI_LOG_DOWN_USER_GROUP_QOS *pstDownGroupQosData = NULL;
    ATG_DCI_LOG_FLOW_UP_USER_QOS *pstFlowUpUserQosData = NULL;
    ATG_DCI_LOG_PORT_VLAN *pstTerminalIf = NULL;
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *pstFlowDiffServData = NULL;
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *pstTrafficFilter = NULL;
    ATG_DCI_LOG_PORT_DS_L2_DATA *pstDsL2Data = NULL;

    //ATG_DCI_LOG_PORT_FLOW_DATA *pstFlowData[ATG_DCI_LOG_PORT_FLOW_NUM];
    //ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *pstFlowDiffServData[ATG_DCI_LOG_FLOW_DIFF_SERV_NUM];    
    ATG_DCI_LOG_PORT_IPV4_DATA *pstIpv4Data[ATG_DCI_LOG_PORT_IPV4_NUM];
    ATG_DCI_LOG_PORT_IPV6_DATA *pstIpv6Data[ATG_DCI_LOG_PORT_IPV6_NUM];
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *pstMcIpv4Data[ATG_DCI_LOG_PORT_MC_IPV4_NUM];
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pstVipVmacData[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];

    /* »ñÈ¡µÄ×ÓÅäÖÃ */
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *pst_vipv6_vmac_data[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];
    ATG_DCI_LOG_PORT_MC_IPV6_DATA * pstMcIpv6Data[ATG_DCI_LOG_PORT_MC_IPV6_NUM];
    //ATG_DCI_LOG_PORT_TRAFFIC_FILTER *pstTrafficFilter[ATG_DCI_LOG_TRAFFIC_FILTER_NUM];
    //ATG_DCI_LOG_PORT_TERMINAL_IF *pstTerminalIf[ATG_DCI_LOG_TERMINAL_IF_NUM];

    /* IPSÏûÏ¢Æ«ÒÆµÄÊ×µØÖ· */
    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucPhyDataStart = NULL;
    NBB_BYTE *pucLogicL3DataStart = NULL;
    NBB_BYTE *pucLogicL2DataStart = NULL;
    NBB_BYTE *pucFlowDataStart = NULL;
    NBB_BYTE *pucCesDataStart = NULL;
    NBB_BYTE *pucIpv4DataStart = NULL;
    NBB_BYTE *pucIpv6DataStart = NULL;
    NBB_BYTE *pucMcMacDataStart = NULL;
    NBB_BYTE *pucVipVmacDataStart = NULL;
    NBB_BYTE *puc_vipv6_vmac_data_start = NULL;/* IPSÏûÏ¢Æ«ÒÆµÄÊ×µØÖ· */
    NBB_BYTE *pucVeMacDataStart = NULL;
    NBB_BYTE *pucDiffServDataStart = NULL;
    NBB_BYTE *pucFlowDiffServDataStart = NULL;
    NBB_BYTE *pucInclassifyQosDataStart = NULL;
    NBB_BYTE *pucUpUserQosDataStart = NULL;
    NBB_BYTE *pucUpGroupQosDataStart = NULL;
    NBB_BYTE *pucDownUserQosDataStart = NULL;
    NBB_BYTE *pucDownGroupQosDataStart = NULL;
    NBB_BYTE *pucFlowUpUserQosDataStart = NULL;
    NBB_BYTE *pucTrafficFilterDataStart = NULL;
    NBB_BYTE *pucTerminalIfDataStart = NULL;
    NBB_BYTE *pucIpv6McMacDataStart = NULL;
    NBB_BYTE *pucDsL2DataStart = NULL;

    /* ×ÓÅäÖÃµÄ²Ù×÷Ä£Ê½ */
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPhy = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLogicL3 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLogicL2 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperFlow = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperCes = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperMcIpv4 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperVipVmac = ATG_DCI_OPER_NULL;
    NBB_ULONG ul_oper_vipv6_vmac = ATG_DCI_OPER_NULL;/* ×ÓÅäÖÃµÄ²Ù×÷Ä£Ê½ */
    NBB_ULONG ulOperVeMac = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperDiffServ = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperFlowDiffServ = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperInclassifyQos = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperUpUserQos = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperUpGroupQos = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperDownUserQos = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperDownGroupQos = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperFlowUpUserQos = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperTrafficFilter = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperTerminalIf = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperMcIpv6 = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperDsL2 = ATG_DCI_OPER_NULL; 

    //QoS½á¹¹Ìå
    SPM_QOS_LOGIC_FLOW_KEY stQosFlowKey;
    SPM_QOS_LOGIC_INTF_KEY stQosIntfKey;
    SPM_QOS_LOGIC_UNIVP stUniVp;
    TX_UNI_PORT stTxUniPort;
    SUB_PORT stSubPort;

    NBB_BYTE trigger_if_flag = 0;
    NBB_BYTE act_if_flag = 0;
    NBB_BYTE ucExistPos_trigger = 0;  
    NBB_BYTE ucExistPos_act = 0; 
    NBB_USHORT usPortId = 0;
    NBB_USHORT usMcVePortId = 0;
    NBB_BYTE usSlotId = 0;
    PORT_GROUP_INFO IfGoupInfo;

    NBB_ULONG ulPortIndexKey = 0;
    NBB_ULONG intf_pos_id_temp = 0; /* ÌõÄ¿µÄ¼üÖµ */
    NBB_ULONG vrrp4_pos_id_temp = 0; /* ÌõÄ¿µÄ¼üÖµ */
    NBB_ULONG vrrp6_pos_id_temp = 0; /* ÌõÄ¿µÄ¼üÖµ */
    
    /* ÌõÄ¿µÄ¼üÖµ */
    NBB_USHORT usVplsIdKey = 0;
    NBB_USHORT usVsiMcId = 0;
    SPM_VPLS_CB *pstVpls = NULL;
    ATG_DCI_LOG_PORT_VLAN flowdiffservcfgkey ;
    ATG_DCI_LOG_PORT_VLAN inclassifyqoscfgkey ;
    ATG_DCI_LOG_PORT_VLAN trafficfiltercfgkey ;
    ATG_DCI_LOG_PORT_VLAN diffservcfgkey ;/* ÌõÄ¿µÄ¼üÖµ */

    NBB_TRC_ENTRY("spm_rcv_dci_set_logical_port");

    /* ÊäÈë²ÎÊýÖ¸Õë±ØÐëÓÐÐ§ */
    NBB_ASSERT(pstSetLogicalPort != NULL);

	if (pstSetLogicalPort == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_logical_port(pstSetLogicalPort==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_logical_port(pstSetLogicalPort==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_logical_port(pstSetLogicalPort==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", 0, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", ucMessage));

        goto EXIT_LABEL;
    }

	OS_MEMSET(&stIntfL3, 0, sizeof(INTF_T));
	OS_MEMSET(&stIntfFlow, 0, sizeof(INTF_T));
    OS_MEMSET(&stvrrp, 0, sizeof(VRRP_ACL_T));
    OS_MEMSET(&stvrrp_v6, 0, sizeof(VRRP_ACL_T));
    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stLogicL2Data, 0, sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA));
    OS_MEMSET(&stVpwsUni, 0, sizeof(ATG_DCI_VPWS_UNI_DATA));
    OS_MEMSET(&stVplsUni, 0, sizeof(ATG_DCI_VPLS_UNI_DATA));
    OS_MEMSET(&stVpPortInfo, 0, sizeof(SPM_VPN_PORT_INFO_CB));
    OS_MEMSET(&stSubPort, 0, sizeof(SUB_PORT));
    OS_MEMSET(&stTxUniPort, 0, sizeof(TX_UNI_PORT));
    OS_MEMSET(&stFlowDiffServDel, 0, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
    OS_MEMSET(&stDiffServDel, 0, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
    OS_MEMSET(&stInclassifyQosDel, 0, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
    OS_MEMSET(&stUpUserQosDel, 0, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
    OS_MEMSET(&stFlowUpUserQosDel, 0, sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS));
    OS_MEMSET(&stUniVp, 0, sizeof(SPM_QOS_LOGIC_UNIVP));
    OS_MEMSET(&flowdiffservcfgkey, 0, sizeof(ATG_DCI_LOG_PORT_VLAN));
    OS_MEMSET(&inclassifyqoscfgkey, 0, sizeof(ATG_DCI_LOG_PORT_VLAN));
    OS_MEMSET(&trafficfiltercfgkey, 0, sizeof(ATG_DCI_LOG_PORT_VLAN));
    OS_MEMSET(&diffservcfgkey, 0, sizeof(ATG_DCI_LOG_PORT_VLAN));

    //Ê×ÏÈ½«IPSÏûÏ¢µÄ·µ»ØÖµÉèÖÃÎªOK£¬Èç¹ûÓÐÒ»¸ö×ÓÅäÖÃÊ§°Ü£¬ÔòÖÃÎªFAIL
    pstSetLogicalPort->return_code = ATG_DCI_RC_OK;

    //printf("***ERROR***:PORT_INDEX=%d, ApiC3AddIntf() ret=%d Ôö¼Óindex=%dµÄINTFÊ§°Ü!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //²»¹Ò
    //OS_PRINTF("***ERROR***:PORT_INDEX=%ld, ApiC3AddIntf() ret=%d Ôö¼Óindex=%ldµÄINTFÊ§°Ü!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //¹Ò
    //OS_PRINTF("***ERROR***:PORT_INDEX=%d, ApiC3AddIntf() ret=%d Ôö¼Óindex=%dµÄINTFÊ§°Ü!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //²»¹Ò
    //OS_PRINTF("***ERROR***:PORT_INDEX=%d, ApiC3AddIntf() ret=%d Ôö¼Óindex=%ldµÄINTFÊ§°Ü!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //¹Ò
    //OS_PRINTF("***ERROR***:PORT_INDEX=%ld, ApiC3AddIntf() ret=%d Ôö¼Óindex=%dµÄINTFÊ§°Ü!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //²»¹Ò
    //OS_PRINTF("***ERROR***:PORT_INDEX=%d, ApiC3AddIntf() ret=%d Ôö¼Óindex=%d µÄINTFÊ§°Ü!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //³õÊ¼»¯Á÷ÅäÖÃ
    //for (i = 0; i < ATG_DCI_LOG_PORT_FLOW_NUM; i++)
    //{
    //    pstFlowData = NULL;
    //}

    //³õÊ¼»¯Á÷Ïà¹ØDiff-ServÅäÖÃL2
    //for (i = 0; i < ATG_DCI_LOG_FLOW_DIFF_SERV_NUM; i++)
    //{
    //    pstFlowDiffServData[i] = NULL;
    //}

    //³õÊ¼»¯Ipv4µØÖ·ÅäÖÃ
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        pstIpv4Data[i] = NULL;
    }

    //³õÊ¼»¯Ipv6µØÖ·ÅäÖÃ
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        pstIpv6Data[i] = NULL;
    }

    //³õÊ¼»¯×é²¥×éµØÖ·
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        pstMcIpv4Data[i] = NULL;
    }

    //³õÊ¼»¯VRRPÐéÄâMACºÍIPµØÖ·ÅäÖÃ
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pstVipVmacData[i] = NULL;
    }
    
    //³õÊ¼»¯VRRP ipv6ÐéÄâMACºÍIPµØÖ·ÅäÖÃ
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pst_vipv6_vmac_data[i] = NULL;
    }
    

    //³õÊ¼»¯ipv6×é²¥µØÖ·ÅäÖÃ
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        pstMcIpv6Data[i] = NULL;
    }

    //³õÊ¼»¯°ü¹ýÂËÆ÷traffic_filterÅäÖÃ
    //for (i = 0; i < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; i++)
    //{
    //    pstTrafficFilter[i] = NULL;
    //}

    
#ifdef SPU

    /* ³õÊ¼»¯intf */
    ApiC3InitIntfStruct(&stIntfL3);
    ApiC3InitIntfStruct(&stIntfFlow);
#endif

    ulPortIndexKey = pstSetLogicalPort->key;

    NBB_TRC_DETAIL((NBB_FORMAT "ulPortIndexKey = %ld", ulPortIndexKey));

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndexKey);

    if (pstLogicalPort == NULL) /* Èç¹ûÌõÄ¿²»´æÔÚ*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }

    /* »ñÈ¡×ÓÅäÖÃ²Ù×÷ */
    ulOperBasic = pstSetLogicalPort->oper_basic;
    ulOperPhy = pstSetLogicalPort->oper_physical;
    ulOperLogicL3 = pstSetLogicalPort->oper_logical_l3;
    ulOperLogicL2 = pstSetLogicalPort->oper_logical_l2;
    ulOperFlow = pstSetLogicalPort->oper_flow;
    ulOperCes = pstSetLogicalPort->oper_ces;
    ulOperIpv4 = pstSetLogicalPort->oper_ipv4;
    ulOperIpv6 = pstSetLogicalPort->oper_ipv6;
    ulOperMcIpv4 = pstSetLogicalPort->oper_mc_ipv4;
    ulOperVipVmac = pstSetLogicalPort->oper_vip_vmac;
    ul_oper_vipv6_vmac = pstSetLogicalPort->oper_vrrp_ipv6_mac_l3;
    ulOperVeMac = pstSetLogicalPort->oper_ve_mac;
    ulOperDiffServ = pstSetLogicalPort->oper_diff_serv;
    ulOperFlowDiffServ = pstSetLogicalPort->oper_flow_diff_serv;
    ulOperInclassifyQos = pstSetLogicalPort->oper_inclassify_qos;
    ulOperUpUserQos = pstSetLogicalPort->oper_up_user_qos;
    ulOperUpGroupQos = pstSetLogicalPort->oper_upuser_group_qos;
    ulOperDownUserQos = pstSetLogicalPort->oper_down_user_queue_qos;
    ulOperDownGroupQos = pstSetLogicalPort->oper_down_user_group_qos;
    ulOperFlowUpUserQos = pstSetLogicalPort->oper_flow_upuser_qos;
    ulOperTrafficFilter = pstSetLogicalPort->oper_traffic_filter;
    ulOperTerminalIf = pstSetLogicalPort->oper_vlan;
    ulOperMcIpv6 = pstSetLogicalPort->oper_mc_ipv6;
    ulOperDsL2 = pstSetLogicalPort->oper_ds_l2;

    /* É¾³ýÕû¸öÌõÄ¿ */
    if (pstSetLogicalPort->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  É¾³ýPORT_INDEX=%ld µÄ¶Ë¿ÚÂß¼­ÅäÖÃ", ulPortIndexKey));

        /* Èç¹ûÌõÄ¿²»´æÔÚ£¬²»É¾³ý */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:ÒªÉ¾³ýµÄÅäÖÃ²¢²»´æÔÚ"));

            OS_PRINTF("***ERROR***:ÒªÉ¾³ýµÄPORT_INDEX=%ld µÄ¶Ë¿ÚÂß¼­ÅäÖÃ²¢²»´æÔÚ!\n", ulPortIndexKey);

            OS_SPRINTF(ucMessage, "***ERROR***:ÒªÉ¾³ýµÄPORT_INDEX=%ld µÄ¶Ë¿ÚÂß¼­ÅäÖÃ²¢²»´æÔÚ!\n", ulPortIndexKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, DEL_UNEXIST_CONFIG_ERROR,
                    "DEL_UNEXIST_CONFIG_ERROR", ucMessage));
                    
            pstSetLogicalPort->return_code = ATG_DCI_RC_OK;

            goto EXIT_LABEL;
        }

        /* ´æÔÚÉ¾³ý */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ²éÑ¯µ½´ËÅäÖÃ£¬´ÓtreeÖÐÉ¾³ý"));
            spm_dbg_print_logical_port_head(ulPortIndexKey, SPM_OPER_DEL);
            spm_dbg_record_logical_port_head(ulPortIndexKey, SPM_OPER_DEL);

            //É¾³ý¶Ë¿ÚÂß¼­ÅäÖÃÇý¶¯ÅäÖÃ
            spm_del_logical_port_cfg(pstLogicalPort NBB_CCXT);

			if (pstLogicalPort != NULL)
			{
	            AVLL_DELETE(SHARED.logical_port_tree, pstLogicalPort->spm_logical_port_node);

	            //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
	            spm_free_logical_port_cb(pstLogicalPort NBB_CCXT);
			}
        }
    }

    /* Ôö¼Ó»ò¸üÐÂÌõÄ¿ */
    else
    {

        /* Èç¹ûÌõÄ¿²»´æÔÚ£¬Ê÷ÖÐÒªÔö¼ÓÌõÄ¿ */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  Ôö¼ÓPORT_INDEX=%ldµÄ ¶Ë¿ÚÂß¼­ÅäÖÃ", ulPortIndexKey));
            spm_dbg_print_logical_port_head(ulPortIndexKey, SPM_OPER_ADD);
            spm_dbg_record_logical_port_head(ulPortIndexKey, SPM_OPER_ADD);

            pstLogicalPort = spm_alloc_logical_port_cb(NBB_CXT);

			if (pstLogicalPort != NULL)
			{
	            pstLogicalPort->port_index_key = ulPortIndexKey;

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.logical_port_tree, pstLogicalPort->spm_logical_port_node);
			}
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ¸üÐÂPORT_INDEX=%ldµÄ ¶Ë¿ÚÂß¼­ÅäÖÃ", ulPortIndexKey));
            spm_dbg_print_logical_port_head(ulPortIndexKey, SPM_OPER_UPD);
            spm_dbg_record_logical_port_head(ulPortIndexKey, SPM_OPER_UPD);
        }

		//Ôö¼Ó´ËÅÐ¶Ï£¬È¡ÏûPC-Lint¸æ¾¯
        if (pstLogicalPort == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:ÒªÔö¼ÓµÄPORT_INDEX=%ld µÄ¶Ë¿ÚÂß¼­ÅäÖÃÎ´ÄÜ³É¹¦ÉêÇëÄÚ´æ!", ulPortIndexKey));

            OS_PRINTF("***ERROR***:ÒªÔö¼ÓµÄPORT_INDEX=%ld µÄ¶Ë¿ÚÂß¼­ÅäÖÃÎ´ÄÜ³É¹¦ÉêÇëÄÚ´æ!\n", ulPortIndexKey);

            OS_SPRINTF(ucMessage, "***ERROR***:ÒªÔö¼ÓµÄPORT_INDEX=%ld µÄ¶Ë¿ÚÂß¼­ÅäÖÃÎ´ÄÜ³É¹¦ÉêÇëÄÚ´æ!\n", ulPortIndexKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ALLOC_MEMORY_ERROR,
                    "ALLOC_MEMORY_ERROR", ucMessage));
                    
            pstSetLogicalPort->return_code = ATG_DCI_RC_UNSUCCESSFUL;

            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /* »ñÈ¡ÅäÖÃ                                                                */
        /***************************************************************************/
        /******************************** »ù±¾ÅäÖÃ *********************************/
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
            pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                &pstSetLogicalPort->basic_data);

            /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  LOGICAL PORT pucBasicDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstBasicData = (ATG_DCI_LOG_PORT_BASIC_DATA *)pucBasicDataStart;

                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1£©»ù±¾ÅäÖÃ\n");
                    spm_dbg_print_logical_port_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage, "  1£©»ù±¾ÅäÖÃ\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_logical_port_basic_cfg(pstBasicData);

                NBB_TRC_DETAIL((NBB_FORMAT "  ¶Ë¿Ú±êÊ¶ = %d", pstBasicData->port_flag));
                NBB_TRC_DETAIL((NBB_FORMAT "  ¶Ë¿Ú×ÓÀàÐÍ = %d", pstBasicData->port_sub_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  ½Ó¿ÚÂ·ÓÉÄÜÁ¦ = %d", pstBasicData->port_route_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  ×Ó½Ó¿ÚÀàÐÍ = %d", pstBasicData->sub_if_type));

                //»ñÈ¡±¾²ÛÎ»ºÅ
                //spm_hardw_getslot(&ucLocalSlot);

                /* ±£´æÊý¾Ý */
                if (pstLogicalPort->basic_cfg_cb == NULL)
                {
                    pstLogicalPort->basic_cfg_cb =
                        (ATG_DCI_LOG_PORT_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_BASIC_DATA),
                        NBB_NORETRY_ACT,
                        MEM_SPM_LOG_PORT_BASIC_CB);
                }

                OS_MEMCPY(pstLogicalPort->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_LOG_PORT_BASIC_DATA));

                if (pstBasicData->port_flag == ATG_DCI_LOOPBACK)
                {
                    goto EXIT_LABEL;
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

        /******************************** ÎïÀíÅäÖÃ ********************************/
        if (ulOperPhy == ATG_DCI_OPER_ADD)
        {

            /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
            pucPhyDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                &pstSetLogicalPort->physical_data);

            /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
            if (pucPhyDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  LOGICAL PORT pucPhyDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstPhyData = (ATG_DCI_LOG_PORT_PHY_DATA *)pucPhyDataStart;

                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2£©ÎïÀíÅäÖÃ\n");
                    spm_dbg_print_logical_port_phy_cfg(pstPhyData);
                }

                OS_SPRINTF(ucMessage, "  2£©ÎïÀíÅäÖÃ\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_logical_port_phy_cfg(pstPhyData);

                NBB_TRC_DETAIL((NBB_FORMAT "  ÎïÀí½Ó¿Úindex  = %ld", pstPhyData->phy_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  Á´Â·¾ÛºÏLAG_ID = %ld", pstPhyData->lag_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  ÐéÄâÒÔÌ«Íø×éID = %ld", pstPhyData->ve_group_id));

				
                //Èç¹ûÌõÄ¿Îª¸üÐÂ£¬ÇÒ´Ë´ÎÏÂ·¢Ã»ÓÐ¸üÐÂ»ù±¾ÅäÖÃ£¬ÇÒÖ®Ç°µÄÌõÄ¿ÅäÖÃÁË»ù±¾ÅäÖÃ
                if (pstLogicalPort->basic_cfg_cb != NULL)
                {
                	//Èç¹û¶Ë¿Ú²»Îªtunnel¿Ú»òloopback¿Ú£¬ÐèÒª»ñÈ¡Êµ¼Êportid
                	if ((pstLogicalPort->basic_cfg_cb->port_flag != ATG_DCI_TUNNEL)
				     && (pstLogicalPort->basic_cfg_cb->port_flag != ATG_DCI_LOOPBACK))
					{
	                    if ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_SUB_INTFACE)
	                        || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE))
	                    {
	                    	//coverity[check_return]
	                        spm_get_portid_from_physical_port_index(pstPhyData->phy_port_index,
	                            &stPortInfo NBB_CCXT);
	                    }
	                    else if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)
	                    {
	                        //unit?
	                        stPortInfo.port_id = pstPhyData->ve_group_id;
                            stPortInfo.slot_id = pstPhyData->ve_slot;
#ifdef SPU                  

	                        //ApiC3SetPortType(UNIT_0, stPortInfo.port_id, VE);
#endif

	                        //NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%d, ¶Ë¿Ú%dÎªVE, ApiC3SetPortType() ret=%d",
	                        // ulPortIndexKey, stPortInfo.port_id, ret));
	                    }
	                    else if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG)
	                    {
	                        stPortInfo.port_id = pstPhyData->lag_id + ATG_DCI_LAG_OFFSET;
	                    }

	                    pstLogicalPort->port_id = stPortInfo.port_id;
	                    pstLogicalPort->slot_id = stPortInfo.slot_id;
	                    pstLogicalPort->unit_id = stPortInfo.unit_id;

		                //Èç¹û»ñÈ¡¶Ë¿ÚÕýÈ·²Å±£´æ
		                if (pstLogicalPort->port_id != 0)
		                {
		                    /* ±£´æÊý¾Ý */
		                    if (pstLogicalPort->phy_cfg_cb == NULL)
		                    {
		                        pstLogicalPort->phy_cfg_cb =
		                            (ATG_DCI_LOG_PORT_PHY_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_PHY_DATA),
		                            NBB_NORETRY_ACT,
		                            MEM_SPM_LOG_PORT_PHY_CB);
		                    }

		                    OS_MEMCPY(pstLogicalPort->phy_cfg_cb, pstPhyData, sizeof(ATG_DCI_LOG_PORT_PHY_DATA)); 
                        }
		                else
		                {
		                    pstSetLogicalPort->phy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
		                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:Âß¼­¶Ë¿Ú PORT_INDEX=%ld, »ñÈ¡ÎïÀí¶Ë¿ÚÊ§°Ü",
		                            ulPortIndexKey));

		                    OS_PRINTF("***ERROR***:Âß¼­¶Ë¿Ú PORT_INDEX=%ld, »ñÈ¡ÎïÀí¶Ë¿ÚÊ§°Ü\n", ulPortIndexKey);

		                    OS_SPRINTF(ucMessage, "***ERROR***:Âß¼­¶Ë¿Ú PORT_INDEX=%ld, »ñÈ¡ÎïÀí¶Ë¿ÚÊ§°Ü\n", ulPortIndexKey);
		                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

		                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, PHYSICAL_PORT_NOT_EXIST,
		                            "PHYSICAL_PORT_NOT_EXIST", ucMessage));
		                }
					}	
					else	//Èç¹û¶Ë¿ÚÎªtunnel¿Ú»òloopback¿Ú£¬Ö÷¿Ø»áÏÂÈ«0Êý¾Ý¸øµ¥ÅÌ£¬Ö»±£´æ¼´¿É
					{
	                    /* ±£´æÊý¾Ý */
	                    if (pstLogicalPort->phy_cfg_cb == NULL)
	                    {
	                        pstLogicalPort->phy_cfg_cb =
	                            (ATG_DCI_LOG_PORT_PHY_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_PHY_DATA),
	                            NBB_NORETRY_ACT,
	                            MEM_SPM_LOG_PORT_PHY_CB);
	                    }

	                    OS_MEMCPY(pstLogicalPort->phy_cfg_cb, pstPhyData, sizeof(ATG_DCI_LOG_PORT_PHY_DATA));
					}

                }
                else
                {
                    pstSetLogicalPort->phy_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    OS_PRINTF("PORT_INDEX=%ld, ÎïÀíÅäÖÃÊ§Åä£¬»ù±¾ÅäÖÃÎÞ·¨»ñÈ¡!\n", ulPortIndexKey);

                    OS_SPRINTF(ucMessage, "PORT_INDEX=%ld, ÎïÀíÅäÖÃÊ§Åä£¬»ù±¾ÅäÖÃÎÞ·¨»ñÈ¡!\n", ulPortIndexKey);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, BASIC_CONFIG_NOT_EXIST,
                            "BASIC_CONFIG_NOT_EXIST", ucMessage));
                }

            }
        }
        else if (ulOperPhy == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperPhy == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }


        /*****************************************************************************/
        /* Èç¹ûÏÂ·¢ÁËÐÂµÄ»ù±¾ÅäÖÃ¡£                                                  */
        /* »òÌõÄ¿ÓÐ¸üÐÂ¡£                                                            */
        /*                                                                           */
        /* L2¶Ë¿Ú                                                                    */
        /*****************************************************************************/
        if ((pstLogicalPort->basic_cfg_cb != NULL) && (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L2))
        {

            /****************************** Âß¼­ÅäÖÃL2 ******************************/
            if (ulOperLogicL2 == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucLogicL2DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->logical_l2_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucLogicL2DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  LOGICAL PORT pucLogicL2DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstLogicL2Data = (ATG_DCI_LOG_PORT_LOGIC_L2_DATA *)pucLogicL2DataStart;

                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  4£©Âß¼­ÅäÖÃL2\n");
                        spm_dbg_print_logical_port_logic_l2_cfg(pstLogicL2Data);
                    }

                    OS_SPRINTF(ucMessage, "  4£©Âß¼­ÅäÖÃL2\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    //spm_dbg_record_logical_port_logic_l2_cfg(pstLogicL2Data);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ½ÓÊÕTAGÐÐÎª = %s", pstLogicL2Data->recv_action ? "TAGGED_VLAN" : "RAW_ETH"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ·¢ËÍTAGÐÐÎª = %d", pstLogicL2Data->send_tag_action));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ·¢ËÍÔö¼Ó/Ìæ»»VLAN ID = %d", pstLogicL2Data->send_vlan_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ·¢ËÍÔö¼Ó/Ìæ»»VLAN PRI = %d", pstLogicL2Data->send_vlan_pri));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IGMP  SNOOPINGÊ¹ÄÜ = %s", pstLogicL2Data->igmp_snooping ? "Ê¹ÄÜ" : "²»Ê¹ÄÜ"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  TagType = %x", pstLogicL2Data->tagtype));
                    NBB_TRC_DETAIL((NBB_FORMAT "  MACÑ§Ï°¿ª¹Ø = %s", pstLogicL2Data->mac_learning_enable ? "¿ª" : "¹Ø"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  Ë®Æ½·Ö¸î¿ª¹Ø = %s", pstLogicL2Data->split_horizon_enable ? "¿ª" : "¹Ø"));
                    //NBB_TRC_DETAIL((NBB_FORMAT "  Á÷ÀàÐÍ = %d", pstLogicL2Data->flow_type));

                    //Èç¹ûposid==0£¬ËµÃ÷vp²¢Ã»ÓÐ¼ÓÈëµ½VPWS/VPLSÖÐ£¬Ö»ÐèÒª±£´æ
                    if (pstLogicalPort->logic_port_info_cb.vp_idx == 0)
                    {
                        /* ±£´æÊý¾Ý */
                        if (pstLogicalPort->logic_l2_cfg_cb == NULL)/*lint !e613 */
                        {
                            pstLogicalPort->logic_l2_cfg_cb =
                                (ATG_DCI_LOG_PORT_LOGIC_L2_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LOG_PORT_L2_CB);
                            
                            if (pstLogicalPort->logic_l2_cfg_cb == NULL)/*lint !e613 */
                            {
                                OS_PRINTF("	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
            	
                    			OS_SPRINTF(ucMessage, "	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                    			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                goto EXIT_LABEL;
                    			
                            }
                        }

                        OS_MEMCPY(pstLogicalPort->logic_l2_cfg_cb, pstLogicL2Data,
                            sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA));/*lint !e613 */
                    }
                    else    //Èç¹ûposid!=0£¬ËµÃ÷vpÒÑ¾­¼ÓÈëµ½VPWS/VPLSÖÐ£¬ÐèÒª¸üÐÂ
                    {
                        //ÁÙÊ±±£´æÒ»ÏÂ£¬Èç¹û¸üÐÂ²»³É¹¦£¬»¹Ô­ÅäÖÃ
                        OS_MEMCPY(&stLogicL2Data, pstLogicalPort->logic_l2_cfg_cb,
                            sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA));
                        OS_MEMCPY(pstLogicalPort->logic_l2_cfg_cb, pstLogicL2Data,
                            sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA));
                        
/*                       
                        ret = SUCCESS;

                        for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
                        {
                            ret += ApiC3DelMplsPortFromVpn(ucC3Unit,
                                pstLogicalPort->logic_port_info_cb.vpn_type,
                                pstLogicalPort->logic_port_info_cb.vpn_id,
                                pstLogicalPort->logic_port_info_cb.vp_idx);
                        }

#endif

                        //¸üÐÂVP
                        if (ret == SUCCESS)
                        {
                            stVpwsUni.port_index = ulPortIndexKey;
                            stVpwsUni.uni_no = pstLogicalPort->logic_port_info_cb.vp_idx;

                            OS_MEMSET(&stVpPortInfo, 0, sizeof(SPM_VPN_PORT_INFO_CB));

                            //stVpPortInfo.vp_idx = pstLogicalPort->logic_port_info_cb.vp_idx;

*/
                        ret = SUCCESS;

                        //¸üÐÂintfµÄÈë¿ÚÊ¶±ð¶¯×
                        for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
                	         pstTerminalIfCb != NULL;
                	         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
                	                       pstTerminalIfCb->spm_terminal_if_node))
                        {
                            if(0 != pstTerminalIfCb->intf_pos_id)
                            {
                                #ifdef SPU
                                ret = fhdrv_psn_l3_set_intf_ptag(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,
                                    pstLogicalPort->logic_l2_cfg_cb->recv_action);                              

                                if((2 == SHARED.c3_num) && 
                                    ((stPortInfo.port_type == ATG_DCI_LAG) 
                                    || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                                {
                                    ret += fhdrv_psn_l3_set_intf_ptag(1, pstTerminalIfCb->intf2_pos_id,
                                        pstLogicalPort->logic_l2_cfg_cb->recv_action);
                                } 

                                if(SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfPtag() ret=%d, "
                                            "PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL2Ë¢ÐÂÊ§°Ü",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3SetIntfPtag() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d "
                                            "CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL2Ë¢ÐÂÊ§°Ü\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            	"***ERROR***:ApiC3SetIntfPtag() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d "
			                            	"CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL2Ë¢ÐÂÊ§°Ü\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
                                }
                                
                                #endif
                            }
                        }   

                        //¸üÐÂvp
                        if (ret == SUCCESS)
                        {
                            stVpwsUni.port_index = ulPortIndexKey;
                            stVpwsUni.uni_no = pstLogicalPort->logic_port_info_cb.vp_idx;/*lint !e613 */

                            OS_MEMSET(&stVpPortInfo, 0, sizeof(SPM_VPN_PORT_INFO_CB));

                            stVpPortInfo.vp_idx = pstLogicalPort->logic_port_info_cb.vp_idx;/*lint !e613 */

                            usVsiMcId = 0;
                            
                            if (L2VPN_VPLS == pstLogicalPort->logic_port_info_cb.vpn_type)    //ÌõÄ¿²»´æÔÚ
                            {
                                usVplsIdKey = pstLogicalPort->logic_port_info_cb.vpn_id;    
                                pstVpls = AVLL_FIND(SHARED.vpls_tree, &usVplsIdKey);
                                
                                if ((NULL != pstVpls) && (NULL != pstVpls->basic_cfg_cb))   //ÌõÄ¿²»´æÔÚ
                                {
                                    usVsiMcId = pstVpls->basic_cfg_cb->mc_id;
                                }    
                            }                           
   
                            ret = spm_vpn_add_vp_uni(pstLogicalPort->logic_port_info_cb.vpn_type,
                                pstLogicalPort->logic_port_info_cb.vpn_id,
                                usVsiMcId,
                                &stVpwsUni,
                                &stVpPortInfo NBB_CCXT);/*lint !e613 */

                            if (ret != SUCCESS)
                            {
                                OS_MEMCPY(pstLogicalPort->logic_l2_cfg_cb, &stLogicL2Data,
                                    sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA));

                                NBB_TRC_DETAIL((NBB_FORMAT
                                        "  ***ERROR***:spm_vpn_add_vp_uni() ret=%d ¸üÐÂ VP_UNIÊ§°Ü vpnid=%d vpntype=%d",
                                        ret,
                                        pstLogicalPort->logic_port_info_cb.vpn_id,
                                        pstLogicalPort->logic_port_info_cb.vpn_type));

                                OS_PRINTF("***ERROR***:spm_vpn_add_vp_uni() ret=%d ¸üÐÂ VP_UNIÊ§°Ü vpnid=%d vpntype=%d\n",
                                    ret,
                                    pstLogicalPort->logic_port_info_cb.vpn_id,
                                    pstLogicalPort->logic_port_info_cb.vpn_type);

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:spm_vpn_add_vp_uni() ret=%d ¸üÐÂ VP_UNIÊ§°Ü vpnid=%d vpntype=%d\n",
                                    ret,
                                    pstLogicalPort->logic_port_info_cb.vpn_id,
                                    pstLogicalPort->logic_port_info_cb.vpn_type);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));
                            }
                        }
                        else
                        {
                            OS_MEMCPY(pstLogicalPort->logic_l2_cfg_cb, &stLogicL2Data,
                                sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA));

                            NBB_TRC_DETAIL((NBB_FORMAT
                                    "  ***ERROR***:PORT_INDEX=%ld, ApiC3DelMplsPortFromVpn() ret=%d! É¾³ý VP_UNIÊ§°Ü",
                                    ulPortIndexKey, ret));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld, ApiC3DelMplsPortFromVpn() ret=%d! É¾³ý VP_UNIÊ§°Ü",
                                ulPortIndexKey,
                                ret);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, ApiC3DelMplsPortFromVpn() ret=%d! É¾³ý VP_UNIÊ§°Ü",
                                ulPortIndexKey,
                                ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_C3_FUNC_ERROR", ucMessage));
                        }
                    }
                }
            }
            else if (ulOperLogicL2 == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperLogicL2 == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** Âß¼­ÅäÖÃCES/CEP ******************************/
            if (ulOperCes == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucCesDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ces_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucCesDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucCesDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstCesData = (ATG_DCI_LOG_PORT_CES_DATA *)pucCesDataStart;

                    //NBB_TRC_DETAIL((NBB_FORMAT "CESÁ´Â·ID   = %d", pstCesData->ces_id));
                    //NBB_TRC_DETAIL((NBB_FORMAT "QOS²ßÂÔË÷Òý = %d", pstCesData->qos_id));
                    //NBB_TRC_DETAIL((NBB_FORMAT "ACL²ßÂÔË÷Òý = %d", pstCesData->acl_id));

                    /* ÅäÖÃ´¦Àí£¬todo */

                    /* ±£´æÊý¾Ý */
                    if (pstLogicalPort->ces_cfg_cb == NULL)
                    {
                        pstLogicalPort->ces_cfg_cb =
                            (ATG_DCI_LOG_PORT_CES_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_CES_DATA),
                            NBB_NORETRY_ACT,
                            MEM_SPM_LOG_PORT_CES_CB);
                    }

                    OS_MEMCPY(pstLogicalPort->ces_cfg_cb, pstCesData, sizeof(ATG_DCI_LOG_PORT_CES_DATA));
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

	        /****************************** VLANÊôÐÔ L2´¦Àí******************************/
	        if (ulOperTerminalIf == ATG_DCI_OPER_ADD)
			{

	            /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
	            pucTerminalIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->vlan_data);

	            /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
	            if (pucTerminalIfDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  pucTerminalIfDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {
	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  21£©VLANÊôÐÔ L2 (num = %d) ADD\n", pstSetLogicalPort->vlan_num);
	                }

	                OS_SPRINTF(ucMessage, "  21£©VLANÊôÐÔ L2 (num = %d) ADD\n", 
                           pstSetLogicalPort->vlan_num);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                            
                    //Èç¹û´ËÂß¼­¶Ë¿Ú²»´æÔÚ£¬Ö»±£´æflow vlanÅäÖÃ£¬½¨VPWS/VPLSÊ±½¨Á÷¡£
                    if (ucIfExist == ATG_DCI_UNEXIST)
                    {

	                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
		                {
		                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
		                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

		                    /* ÅäÖÃ´¦Àí£¬todo */
		                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
		                    {
		                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
		                    }

		                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

		                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
		                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

			                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

							//Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó1
			                if (NULL == pstTerminalIfCb)
			                {
			                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
			                	
				                //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
				                //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
				                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
				                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

				                pstLogicalPort->terminal_if_num++;
			                }
							else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
							{
								pstTerminalIfCb->intf_pos_id = stIntfFlow.posId;
								NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
							}
		                }
                    }
                    //Èç¹ûÂß¼­¶Ë¿Ú´æÔÚ£¬Á÷ÓÐÔö¼Ó»ò¸üÐÂ²Ù×÷£¬ÐèÒªÑ­»·±éÀúÒÑ´æÔÚµÄÁ÷
                    else
                    {
                        OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                        spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                        //1¡¢Èç¹û¶Ë¿Ú²»ÊôÓÚÈÎºÎVPWS/VPLS
                        //2¡¢Èç¹ûÒÑ¾­ÊôÓÚVPWS/VPLS£¬µ«¶Ë¿Ú²»ÊôÓÚ±¾²ÛÎ»£¬Ö»±£´æÅäÖÃ
                        if ((pstLogicalPort->logic_port_info_cb.vp_idx == 0)
                            || ((pstLogicalPort->logic_port_info_cb.vp_idx != 0)
                                && (stPortInfo.slot_id != SHARED.local_slot_id)
                                && (stPortInfo.port_type != ATG_DCI_VE_PORT)
                                && (stPortInfo.port_type != ATG_DCI_LAG)))
                        {

		                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
			                {
			                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
			                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

			                    /* ÅäÖÃ´¦Àí£¬todo */
			                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
			                    {
			                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
			                    }

			                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

			                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
			                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

				                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó1
				                if (NULL == pstTerminalIfCb)
				                {
				                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
				                	
					                //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
					                //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
					                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
					                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

					                pstLogicalPort->terminal_if_num++;
				                }
								else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
								{
									pstTerminalIfCb->intf_pos_id = stIntfFlow.posId;
									NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
								}
			                }
                        }
                        //Èç¹û¶Ë¿ÚÒÑ¾­ÔÚVPWS/VPLSÖÐ
                        else
                        {

		                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
			                {
			                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
			                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

			                    /* ÅäÖÃ´¦Àí£¬todo */
			                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
			                    {
			                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
			                    }

			                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

			                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
			                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

                                //stIntfFlow.flags |= INTF_COUNTER_ON;

								//Èë¿ÚÊ¶±ð
                                if ((pstLogicalPort->logic_l2_cfg_cb != NULL) 
                                 && (pstLogicalPort->logic_l2_cfg_cb->recv_action == ATG_DCI_TAGGED_VLAN))
                                {
                                	stIntfFlow.flags |= INTF_PTAG_IDENTIFY_ENABLE;
                                }

                                stIntfFlow.mCardPort = pstLogicalPort->port_id;      //ÕâÀïÒªÓÃ»ñÈ¡¶Ë¿Úº¯ÊýÌæ»»;//todo
                                stIntfFlow.eIntfType = INTF_TYPE_L2;
                                stIntfFlow.bindId = pstLogicalPort->logic_port_info_cb.vp_idx;

                                NBB_TRC_DETAIL((NBB_FORMAT "  stIntfFlow.bindId = %d", stIntfFlow.bindId));

			                    //Á÷ÀàÐÍ :»ùÓÚ¶Ë¿Ú
			                    if ((pstTerminalIf->svlan_id == 0) && (pstTerminalIf->cvlan_id == 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
				                    {
				                        stIntfFlow.mIntfMatchType = INTF_MATCH_L2_VE_PORT;
				                    }
				                    else
				                    {
				                    	stIntfFlow.mIntfMatchType = INTF_MATCH_PORT;
				                    }
			                        
			                        stIntfFlow.mOvid = 0;
			                        stIntfFlow.mIvid = 0;
			                    }

			                    //»ùÓÚ¶Ë¿Ú+VLAN£¨Trunk£©
			                    else if ((pstTerminalIf->svlan_id != 0) && (pstTerminalIf->cvlan_id == 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
				                    {
				                        stIntfFlow.mIntfMatchType = INTF_MATCH_L2_VE_PORT_OVID;
				                    }
				                    else
				                    {
				                    	stIntfFlow.mIntfMatchType = INTF_MATCH_PORT_OVID;
				                    }
			                        
			                        stIntfFlow.mOvid = pstTerminalIf->svlan_id;
			                        stIntfFlow.mIvid = 0;
			                    }

			                    //»ùÓÚ¶Ë¿Ú+QINQ
			                    else if ((pstTerminalIf->svlan_id != 0) && (pstTerminalIf->cvlan_id != 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
				                    {
				                        stIntfFlow.mIntfMatchType = INTF_MATCH_L2_VE_PORT_OVID_IVID;
				                    }
				                    else
				                    {
				                    	stIntfFlow.mIntfMatchType = INTF_MATCH_PORT_OVID_IVID;
				                    }
			                        
			                        stIntfFlow.mOvid = pstTerminalIf->svlan_id;
			                        stIntfFlow.mIvid = pstTerminalIf->cvlan_id;
			                    }
			                    else
			                    {
			                        OS_PRINTF("***WARNNING***: PORT_INDEX=%ld Ö»ÅäÖÃÁËCVlan£¬Î´ÅäÖÃSVlan\n", ulPortIndexKey);
			                    }

	                        	ret = SUCCESS;
#ifdef SPU
	                            ret = ApiC3AddIntf(pstLogicalPort->unit_id, &stIntfFlow);
                                   intf_pos_id_temp = stIntfFlow.posId;
	                            NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%ld, ApiC3AddIntf() ret = %d",
	                                    ulPortIndexKey, ret));
                                   stIntfFlow.posId = 0;
                                
                                   if((2 == SHARED.c3_num) && 
                                        ((stPortInfo.port_type == ATG_DCI_LAG) 
                                        || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                                   {
                                        ret += ApiC3AddIntf(1, &stIntfFlow);
                                        NBB_TRC_DETAIL((NBB_FORMAT "  1: PORT_INDEX=%ld, ApiC3AddIntf() ret = %d",
	                                    ulPortIndexKey, ret));
                                   }                             
#endif
			                    if (ret == SUCCESS)
			                    {

					                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

									//Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó
					                if (NULL == pstTerminalIfCb)
					                {
					                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
					                	
						                //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
						                pstTerminalIfCb->intf_pos_id = intf_pos_id_temp;
                                                          pstTerminalIfCb->intf2_pos_id = stIntfFlow.posId;
						                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
						                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

						                pstLogicalPort->terminal_if_num++;
					                }
									else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
									{
                                                                    pstTerminalIfCb->intf_pos_id = intf_pos_id_temp;
                                                                    pstTerminalIfCb->intf2_pos_id = stIntfFlow.posId;
										NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
									}
#ifdef SPU
									almpm_addcfg_intf_onePos(ulPortIndexKey, pstTerminalIfCb->intf_pos_id);
#endif					                
			                    }
			                    else
			                    {
			                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 Ôö¼ÓÊ§°Ü",
			                        		ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 Ôö¼ÓÊ§°Ü\n",
			                        		ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 Ôö¼ÓÊ§°Ü\n",
			                            	ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
			                    }
			                }
                        }
                    }	                
	            }
	        }
	        else if (ulOperTerminalIf == ATG_DCI_OPER_DEL)
	        {

	            /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
	            pucTerminalIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->vlan_data);

	            /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
	            if (pucTerminalIfDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  pucTerminalIfDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {
	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  21£©VLANÊôÐÔ L2 (num = %d) DEL\n", pstSetLogicalPort->vlan_num);
	                }

	                OS_SPRINTF(ucMessage, "  21£©VLANÊôÐÔ L2 (num = %d) DEL\n", 
                           pstSetLogicalPort->vlan_num);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                    //Èç¹ûÂß¼­¶Ë¿Ú´æÔÚ£¬Á÷ÓÐÉ¾³ý²Ù×÷£¬ÐèÒªÑ­»·±éÀúÒÑ´æÔÚµÄÁ÷
                    if (ucIfExist == ATG_DCI_EXIST)
                    {
                        OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                        spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);
                        
                        //1¡¢Èç¹û¶Ë¿Ú²»ÊôÓÚÈÎºÎVPWS/VPLS
                        //2¡¢Èç¹ûÒÑ¾­ÊôÓÚVPWS/VPLS£¬µ«¶Ë¿Ú²»ÊôÓÚ±¾²ÛÎ»£¬Ö»É¾³ýÅäÖÃ
                        if ((pstLogicalPort->logic_port_info_cb.vp_idx == 0)
                            || ((pstLogicalPort->logic_port_info_cb.vp_idx != 0)
                                && (stPortInfo.slot_id != SHARED.local_slot_id)
                                && (stPortInfo.port_type != ATG_DCI_VE_PORT)
                                && (stPortInfo.port_type != ATG_DCI_LAG)))
                        {

                            //»ñÈ¡ÅäÖÃ
		                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
			                {
			                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
			                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

			                    /* ÅäÖÃ´¦Àí£¬todo */
			                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
			                    {
			                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
			                    }

			                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

			                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
			                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

				                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//Èç¹û²»´æÔÚ£¬ÎÞ·¨É¾³ý
				                if (NULL == pstTerminalIfCb)
				                {
			                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 ²»´æÔÚ£¬ÎÞ·¨É¾³ý",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
				                }
								else
								{
						            AVLL_DELETE(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

						            //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
						            spm_free_terminal_if_cb(pstTerminalIfCb NBB_CCXT);
						            
									pstLogicalPort->terminal_if_num--;									
								}
			                    
                            }
                        }
                        //Èç¹û¶Ë¿ÚÒÑ¾­ÔÚVPWS/VPLSÖÐ£¬ËµÃ÷ÒÑ¾­½¨Á÷£¬É¾³ýÏàÓ¦µÄÁ÷
                        else
                        {

                            //»ñÈ¡ÅäÖÃ
		                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
			                {
			                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
			                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

			                    /* ÅäÖÃ´¦Àí£¬todo */
			                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
			                    {
			                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
			                    }

			                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

			                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
			                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

								pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//Èç¹û²»´æÔÚ£¬ÎÞ·¨É¾³ý
				                if (NULL == pstTerminalIfCb)
				                {
			                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 ²»´æÔÚ£¬ÎÞ·¨É¾³ý",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
				                }
				                else	//´æÔÚÉ¾³ý
                                            {
                                                ret = SUCCESS;
#ifdef SPU
                                                ret = ApiC3DelIntf(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id);

                                                NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%ld, ApiC3DelIntf() ret = %d",
                                                ulPortIndexKey, ret));

                                                if((2 == SHARED.c3_num) && 
                                                    ((stPortInfo.port_type == ATG_DCI_LAG) 
                                                    || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                                                {
                                                    ret += ApiC3DelIntf(1, pstTerminalIfCb->intf2_pos_id);
                                                    NBB_TRC_DETAIL((
                                                    NBB_FORMAT "  1: PORT_INDEX=%ld, ApiC3DelIntf() ret = %d",
                                                    ulPortIndexKey, ret));
                                                }                                     
#endif
									if (ret == SUCCESS)
									{
#ifdef SPU
										almpm_delline_intf(ulPortIndexKey, pstTerminalIfCb->intf_pos_id, 1);
#endif
							            AVLL_DELETE(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

							            //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
							            spm_free_terminal_if_cb(pstTerminalIfCb NBB_CCXT);
							            
										pstLogicalPort->terminal_if_num--;
									}
									else
									{
				                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

				                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3DelIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 Ôö¼ÓÊ§°Ü",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id));

				                        OS_PRINTF("***ERROR***:ApiC3DelIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 É¾³ýÊ§°Ü\n",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id);

				                        OS_SPRINTF(ucMessage,
				                            "***ERROR***:ApiC3DelIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L2 É¾³ýÊ§°Ü\n",
				                            	ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id);
				                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

				                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
				                                "CALL_FUNC_ERROR", ucMessage));
				                    }
								}		
                            }			                
                        }
                    }	                
	            }
	        }
	        else if (ulOperTerminalIf == ATG_DCI_OPER_UPDATE)
	        {
	            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
	        }

            /****************************** Á÷Ïà¹ØDiff-ServÅäÖÃL2 ******************************/
            if (ulOperFlowDiffServ == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucFlowDiffServDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->flow_diff_serv_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucFlowDiffServDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucFlowDiffServDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                
                    OS_SPRINTF(ucMessage, "  13£©Á÷Ïà¹ØDiff-ServÅäÖÃL2 ADD\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->flow_diff_serv_num; i++)
                    {
                        pstFlowDiffServData = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *)
                            (pucFlowDiffServDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV)))
                            * i);

                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            printf("  13£©Á÷Ïà¹ØDiff-ServÅäÖÃL2 ADD\n");
                            spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                        }

                        //OS_SPRINTF(ucMessage, "  13£©Á÷Ïà¹ØDiff-ServÅäÖÃL2 ADD\n");
                        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
                        //spm_dbg_record_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);

                        NBB_TRC_DETAIL((NBB_FORMAT "  Diff-ServÄ£°åË÷Òý   = %d", pstFlowDiffServData->diff_serv_id));


                        //Èç¹ûÂß¼­¶Ë¿Ú´æÔÚ£¬ÇÒÁ÷ÒÑ¼ÓÈëµ½VPN£¬¿ÉÒÔ½¨QoS
                        if (ucIfExist == ATG_DCI_EXIST)
                        {
                            OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                            spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);
                            stSubPort.unit = stPortInfo.unit_id;
                            stSubPort.port = stPortInfo.port_id;
                            stSubPort.slot = stPortInfo.slot_id;
                            stSubPort.ivlan = stPortInfo.cvlan;
                            stSubPort.ovlan = stPortInfo.svlan;

                            /* ÅäÖÃ´¦Àí£¬todo */
                            stQosFlowKey.index = ulPortIndexKey;
                            stQosFlowKey.svlan = pstFlowDiffServData->svlan;
                            stQosFlowKey.cvlan = pstFlowDiffServData->cvlan;

							ulVlanPosId = 0;
		                    spm_get_poscnt_from_intf(ulPortIndexKey, 
		                    	pstFlowDiffServData->svlan, 
		                    	pstFlowDiffServData->cvlan, 
		                    	&ulVlanPosId NBB_CCXT);
		                    	
		                    stSubPort.posid = ulVlanPosId;
                            if(0 == ulVlanPosId)
                            {
                                NBB_TRC_DETAIL((NBB_FORMAT
                                        "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü,posId=0!\n",
                                        ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan));

                                OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü,posId=0!\n",
                                        ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan);
                                    
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                OS_SPRINTF(ucMessage,
                                    "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü,posId=0!\n",
                                        ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                pstSetLogicalPort->flow_diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            }

                            else
                            {
                                ret = spm_ds_add_logic_flow_node(&stSubPort,
                                &stQosFlowKey,
                                pstFlowDiffServData
                                NBB_CCXT);
                                
                                if((2 == SHARED.c3_num) && 
                                     ((stPortInfo.port_type == ATG_DCI_LAG) 
                                     || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                                {
                                    ul_vlanposid2 = 0;
                                    spm_get_poscnt_from_intf2(ulPortIndexKey, 
                                    pstFlowDiffServData->svlan, 
                                    pstFlowDiffServData->cvlan, 
                                    &ul_vlanposid2 NBB_CCXT);

                                    if(0 != ul_vlanposid2)
                                    {
                                        stSubPort.unit = 1;
                                        stSubPort.posid = ul_vlanposid2;
                                        ret += spm_ds_add_logic_flow_node(&stSubPort,
                                        &stQosFlowKey,
                                        pstFlowDiffServData
                                        NBB_CCXT);                                     
                                    }
                                }                               

                                if (ret != SUCCESS)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT
                                            "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü",
                                            ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan));

                                    OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü!\n",
                                        ulPortIndexKey,
                                        ret,
                                        pstFlowDiffServData->svlan, 
                                        pstFlowDiffServData->cvlan);
                                        
                                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                            "CALL_FUNC_ERROR", ucMessage));

                                    OS_SPRINTF(ucMessage,
                                        "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü!\n",
                                        ulPortIndexKey,
                                        ret,
                                        pstFlowDiffServData->svlan, 
                                        pstFlowDiffServData->cvlan);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                    pstSetLogicalPort->flow_diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                }
                                else
                                {
                                    //flowdiffservcfgkey.svlan_id = pstFlowDiffServData->svlan;
                                    //flowdiffservcfgkey.cvlan_id = pstFlowDiffServData->cvlan;
                                    //pstflowdiffservcb = AVLL_FIND(pstLogicalPort->flow_diff_serv_tree, 
                                        //&flowdiffservcfgkey);
                                    pstflowdiffservcb = AVLL_FIND(pstLogicalPort->flow_diff_serv_tree, 
                                        pstFlowDiffServData);

                                    //Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó1
                                    if (NULL == pstflowdiffservcb)
                                    {
                                        pstflowdiffservcb = spm_alloc_flow_diff_serv_cb(NBB_CXT);
                                        
                                        if (NULL == pstflowdiffservcb)/*lint !e613 */
                                        {
                                            OS_PRINTF("	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                        	
                                			OS_SPRINTF(ucMessage, "	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                                			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                            goto EXIT_LABEL;
                                			
                                        }
                                        
                                        //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
                                        //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
                                        //NBB_MEMCPY(&pstflowdiffservcb->key,&flowdiffservcfgkey,
                                            //sizeof(ATG_DCI_LOG_PORT_VLAN));
                                        NBB_MEMCPY(&(pstflowdiffservcb->flow_diff_serv_cfg),
                                            pstFlowDiffServData, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                                        AVLL_INSERT(pstLogicalPort->flow_diff_serv_tree, 
                                            pstflowdiffservcb->spm_flow_diff_serv_node);

                                        pstLogicalPort->flow_diff_serv_num++;
                                    }
                                    else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
                                    {
                                        //pstdiffservcb->intf_pos_id = stIntfFlow.posId;
                                        NBB_MEMCPY(&(pstflowdiffservcb->flow_diff_serv_cfg), 
                                            pstFlowDiffServData, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                                    }
                                }                                
                            }
#if 0
                            }
		                    
                            
                            if (ret == SUCCESS)
                            {
                                pstLogicalPort->logic_port_info_cb.flow_info_cb.if_qos = ATG_DCI_USED;

                                /* ±£´æÊý¾Ý */
                                if (pstLogicalPort->flow_diff_serv_cfg_cb[usFlowId - 1] == NULL)
                                {
                                    pstLogicalPort->flow_diff_serv_cfg_cb[usFlowId - 1] =
                                        (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *)NBB_MM_ALLOC(sizeof(
                                            ATG_DCI_LOG_PORT_FLOW_DIFF_SERV),
                                        NBB_NORETRY_ACT,
                                        MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);

                                    OS_MEMCPY(pstLogicalPort->flow_diff_serv_cfg_cb[usFlowId - 1],
                                        pstFlowDiffServData,
                                        sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));

                                    pstLogicalPort->flow_diff_serv_num++;
                                }
                                else    //Èç¹û²»ÎªNULL£¬ËµÃ÷Êý¾ÝÐèÒª¸üÐÂ
                                {
                                    OS_MEMCPY(pstLogicalPort->flow_diff_serv_cfg_cb[usFlowId - 1],
                                        pstFlowDiffServData,
                                        sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                                }
                            }
                            else
                            {
                                //NBB_TRC_DETAIL((NBB_FORMAT
                                        //"  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! ÐÞ¸Ä flow_id=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü",
                                        //ulPortIndexKey, usFlowId));

                                //OS_PRINTF(
                                   // "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d ÐÞ¸Äflow_id=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü!\n",
                                   // ulPortIndexKey,
                                   // ret,
                                   // usFlowId);
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                //OS_SPRINTF(
                                    //ucMessage,
                                    //"***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d ÐÞ¸Äflow_id=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü!\n",
                                    //ulPortIndexKey,
                                    //ret,
                                    //usFlowId);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                pstSetLogicalPort->flow_diff_serv_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                            }
#endif 
                        }
                        else
                        {
                            pstSetLogicalPort->flow_diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢Flow-Diff-ServÊ§°Ü",
                                ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢Flow-Diff-ServÊ§°Ü\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢Flow-Diff-ServÊ§°Ü\n",
                                ulPortIndexKey);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
                                BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
                        }                    
                    }
                }
            }
            else if (ulOperFlowDiffServ == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucFlowDiffServDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->flow_diff_serv_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucFlowDiffServDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucFlowDiffServDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                { 
                
                    OS_SPRINTF(ucMessage, "  13£©Á÷Ïà¹ØDiff-ServÅäÖÃL2 DEL\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->flow_diff_serv_num; i++)
                    {
                        pstFlowDiffServData = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *)
                            (pucFlowDiffServDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV)))
                            * i);

                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            printf("  13£©Á÷Ïà¹ØDiff-ServÅäÖÃL2 DEL\n");
                            spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                        }

                        //OS_SPRINTF(ucMessage, "  13£©Á÷Ïà¹ØDiff-ServÅäÖÃL2 DEL\n");
                        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
                        //spm_dbg_record_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);

                        //NBB_TRC_DETAIL((NBB_FORMAT "  Á÷ID                = %d", pstFlowDiffServData[i]->flow_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  Diff-ServÄ£°åË÷Òý   = %d", pstFlowDiffServData->diff_serv_id));

                        //usFlowId = pstFlowDiffServData[i]->flow_id;

                        //Èç¹ûÂß¼­¶Ë¿Ú´æÔÚ£¬ÇÒÁ÷ÒÑ¼ÓÈëµ½VPN£¬¿ÉÒÔÔö¼ÓQoS
                        if (ucIfExist == ATG_DCI_EXIST)
                        {
                            OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                            spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);
                            stSubPort.unit = stPortInfo.unit_id;
                            stSubPort.port = stPortInfo.port_id;
                            stSubPort.slot = stPortInfo.slot_id;
                            stSubPort.ivlan = stPortInfo.cvlan;
                            stSubPort.ovlan = stPortInfo.svlan;

                            /* ÅäÖÃ´¦Àí£¬todo */
                            stQosFlowKey.index = ulPortIndexKey;
                            stQosFlowKey.svlan = pstFlowDiffServData->svlan;
                            stQosFlowKey.cvlan = pstFlowDiffServData->cvlan;

							ulVlanPosId = 0;
		                    spm_get_poscnt_from_intf(ulPortIndexKey, 
		                    	pstFlowDiffServData->svlan, 
		                    	pstFlowDiffServData->cvlan, 
		                    	&ulVlanPosId NBB_CCXT);
		                    	
		                    stSubPort.posid = ulVlanPosId;
		                    
                            pstFlowDiffServData->diff_serv_id = 0;

                            ret = spm_ds_add_logic_flow_node(&stSubPort,
                                &stQosFlowKey,
                                pstFlowDiffServData
                                NBB_CCXT);
                            
                            if((2 == SHARED.c3_num) && 
                                 ((stPortInfo.port_type == ATG_DCI_LAG) || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                            {
                                ul_vlanposid2 = 0;
                                spm_get_poscnt_from_intf2(ulPortIndexKey, 
                                pstFlowDiffServData->svlan, 
                                pstFlowDiffServData->cvlan, 
                                &ul_vlanposid2 NBB_CCXT);

                                if(0 != ul_vlanposid2)
                                {
                                    stSubPort.unit = 1;
                                    stSubPort.posid = ul_vlanposid2;
                                    ret += spm_ds_add_logic_flow_node(&stSubPort,
                                    &stQosFlowKey,
                                    pstFlowDiffServData
                                    NBB_CCXT);                                     
                                }
                            } 
                            

                            if (ret != SUCCESS)
                            {
                                NBB_TRC_DETAIL((NBB_FORMAT
                                        "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! É¾³ý SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü",
                                        ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstFlowDiffServData->svlan, 
                                    pstFlowDiffServData->cvlan);
                                    
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstFlowDiffServData->svlan, 
                                    pstFlowDiffServData->cvlan);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                pstSetLogicalPort->flow_diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            }
                            else
                            {
                                    //flowdiffservcfgkey.svlan_id = pstFlowDiffServData->svlan;
                                    //flowdiffservcfgkey.cvlan_id = pstFlowDiffServData->cvlan;
                                    //pstflowdiffservcb = AVLL_FIND(pstLogicalPort->flow_diff_serv_tree, 
                                        //&flowdiffservcfgkey);
                                    pstflowdiffservcb = AVLL_FIND(pstLogicalPort->flow_diff_serv_tree, 
                                        pstFlowDiffServData);

                                //Èç¹û²»´æÔÚ£¬ÎÞ·¨É¾³ý
                                if (NULL == pstflowdiffservcb)
                                {
                                    pstSetLogicalPort->flow_diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                    NBB_TRC_DETAIL((NBB_FORMAT 
                                    "  ***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄFlow-Diff-ServÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý",
                                    ulPortIndexKey,
                                    pstFlowDiffServData->svlan,
                                    pstFlowDiffServData->cvlan));

                                    OS_PRINTF(
                                    "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄFlow-Diff-ServÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
                                    ulPortIndexKey,
                                    pstFlowDiffServData->svlan,
                                    pstFlowDiffServData->cvlan);

                                    OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄFlow-Diff-ServÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
                                    ulPortIndexKey,
                                    pstFlowDiffServData->svlan,
                                    pstFlowDiffServData->cvlan);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", 
                                    ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                                }
                                else
                                {
                                    AVLL_DELETE(pstLogicalPort->flow_diff_serv_tree, 
                                    pstflowdiffservcb->spm_flow_diff_serv_node);

                                    //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
                                    spm_free_flow_diff_serv_cb(pstflowdiffservcb NBB_CCXT);

                                    pstLogicalPort->flow_diff_serv_num--;									
                                }
                            }    
                            
#if 0
                            if (ret == SUCCESS)
                            {
                                pstLogicalPort->logic_port_info_cb.flow_info_cb.if_qos = ATG_DCI_UNUSED;

                                NBB_MM_FREE(pstLogicalPort->flow_diff_serv_cfg_cb[usFlowId - 1],
                                    MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
                                pstLogicalPort->flow_diff_serv_cfg_cb[usFlowId - 1] = NULL;

                                pstLogicalPort->flow_diff_serv_num--;
                            }
                            else
                            {
                                //NBB_TRC_DETAIL((NBB_FORMAT
                                //        "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! É¾³ý flow_id=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü",
                                //        ulPortIndexKey, usFlowId));

                                //OS_PRINTF(
                                //    "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d É¾³ý flow_id=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü!\n",
                                //    ulPortIndexKey,
                                //    ret,
                                //    usFlowId);
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                //OS_SPRINTF(
                                //    ucMessage,
                                //    "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d É¾³ý flow_id=%d µÄÁ÷Ïà¹ØDiff-ServÅäÖÃÊ§°Ü!\n",
                                //    ulPortIndexKey,
                                //    ret,
                                //    usFlowId);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                pstSetLogicalPort->flow_diff_serv_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                            }
#endif
                        }
                        else
                        {
                            pstSetLogicalPort->flow_diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýFlow-Diff-ServÊ§°Ü",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýFlow-Diff-ServÊ§°Ü\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýFlow-Diff-ServÊ§°Ü\n",
                                ulPortIndexKey);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
                                    BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
                        }                        
                    }
                }
            }
            else if (ulOperFlowDiffServ == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** Á÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL2 ******************************/
            if (ulOperFlowUpUserQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucFlowUpUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->flow_upuser_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucFlowUpUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucFlowUpUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    OS_SPRINTF(ucMessage, "  19£©Á÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL2\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->flow_upuser_qos_num; i++)
	                {
                        pstFlowUpUserQosData = (ATG_DCI_LOG_FLOW_UP_USER_QOS *)
                            (pucFlowUpUserQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  19£©Á÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL2\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    //NBB_TRC_DETAIL((NBB_FORMAT "  Á÷ID                = %d", pstFlowUpUserQosData->flow_id));
	                    NBB_TRC_DETAIL((NBB_FORMAT "  QOS²ßÂÔÄ£°åË÷Òý     = %d", pstFlowUpUserQosData->qos_policy_index));

	                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

	                    stSubPort.unit = stPortInfo.unit_id;
	                    stSubPort.port = stPortInfo.port_id;
	                    stSubPort.slot = stPortInfo.slot_id;
	                    stSubPort.ivlan = stPortInfo.cvlan;
	                    stSubPort.ovlan = stPortInfo.svlan;

						ulVlanPosId = 0;
	                    spm_get_poscnt_from_intf(ulPortIndexKey, 
	                    	pstFlowUpUserQosData->svlan, 
	                    	pstFlowUpUserQosData->cvlan, 
	                    	&ulVlanPosId NBB_CCXT);
	                    	
	                    stSubPort.posid = ulVlanPosId;

                        if(0 == ulVlanPosId)
                        {
                            NBB_TRC_DETAIL((NBB_FORMAT
                                    "  ***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() error! Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü£¬posId=0!\n",
                                    ulPortIndexKey, pstFlowUpUserQosData->svlan, pstFlowUpUserQosData->cvlan));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü£¬posId=0!\n",
                                ulPortIndexKey,
                                pstFlowUpUserQosData->svlan, 
                                pstFlowUpUserQosData->cvlan);
                                
                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node()Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü£¬posId=0!\n",
                                ulPortIndexKey,
                                pstFlowUpUserQosData->svlan, 
                                pstFlowUpUserQosData->cvlan);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            pstSetLogicalPort->flow_upuser_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        }

                        else
                        {
                             ret = spm_add_flow_up_usr_qos_node(ulPortIndexKey,
	                        							&stSubPort,
	                        							pstFlowUpUserQosData);
                             if (ret != SUCCESS)
                            {
                                NBB_TRC_DETAIL((NBB_FORMAT
                                        "  ***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() error! Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü",
                                        ulPortIndexKey, pstFlowUpUserQosData->svlan, pstFlowUpUserQosData->cvlan));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstFlowUpUserQosData->svlan, 
                                    pstFlowUpUserQosData->cvlan);
                                    
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÁ÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstFlowUpUserQosData->svlan, 
                                    pstFlowUpUserQosData->cvlan);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                pstSetLogicalPort->flow_upuser_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            }
                        }
	                    
	                   
	                }   
                }
            }
            else if (ulOperFlowUpUserQos == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperFlowUpUserQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL2 ******************************/
            if (ulOperInclassifyQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucInclassifyQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->inclassify_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucInclassifyQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucInclassifyQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  14)ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL2 ADD\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);    
                    
                    for (i = 0; i < pstSetLogicalPort->inclassify_qos_num; i++)
	                {
                        pstInclassifyQosData = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)
                            (pucInclassifyQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  14)ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL2 ADD\n");
                            
                               //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                        spm_dbg_print_logical_port_inclassify_qos_cfg(pstInclassifyQosData);
	                    }
                        
                           //OS_SPRINTF(ucMessage, "  14)ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL2 ADD\n");
                           //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                            
	                    NBB_TRC_DETAIL((NBB_FORMAT "  ¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÄ£°åË÷Òý = %d",
	                            pstInclassifyQosData->qos_policy_index));
                        
                            if (ucIfExist == ATG_DCI_EXIST)
                            {
                                OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                                spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                                stSubPort.unit = stPortInfo.unit_id;
                                stSubPort.port = stPortInfo.port_id;
                                stSubPort.slot = stPortInfo.slot_id;
                                stSubPort.ivlan = stPortInfo.cvlan;
                                stSubPort.ovlan = stPortInfo.svlan;

                                ulVlanPosId = 0;
                                spm_get_poscnt_from_intf(ulPortIndexKey, 
                                pstInclassifyQosData->svlan, 
                                pstInclassifyQosData->cvlan, 
                                &ulVlanPosId NBB_CCXT);

                                stSubPort.posid = ulVlanPosId;

                                if(0 == ulVlanPosId)
                                {
                                    pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                    NBB_TRC_DETAIL((NBB_FORMAT
                                    "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü,posId=0!\n",
                                    ulPortIndexKey, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

                                    OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü,posId=0!\n",
                                    ulPortIndexKey, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

                                    OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü,posId=0!\n",
                                    ulPortIndexKey, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                                }
                                else
                                {
                                    ret = spm_add_logic_flow_classify_node(ulPortIndexKey,
                                    pstInclassifyQosData,
                                    &stSubPort
                                    NBB_CCXT);
                                    
                                    if((2 == SHARED.c3_num) && 
                                         ((stPortInfo.port_type == ATG_DCI_LAG) 
                                         || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                                    {
                                        ul_vlanposid2 = 0;
                                        spm_get_poscnt_from_intf2(ulPortIndexKey, 
                                        pstInclassifyQosData->svlan, 
                                        pstInclassifyQosData->cvlan, 
                                        &ul_vlanposid2 NBB_CCXT);

                                        if(0 != ul_vlanposid2)
                                        {
                                            stSubPort.unit = 1;
                                            stSubPort.posid = ul_vlanposid2;
                                            ret += spm_add_logic_flow_classify_node(ulPortIndexKey,
                                            pstInclassifyQosData,
                                            &stSubPort
                                            NBB_CCXT);                                    
                                        }
                                    } 
                                
                                    if (ret != SUCCESS)
                                    {
                                        pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                        NBB_TRC_DETAIL((NBB_FORMAT
                                        "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü!\n",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

                                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü!\n",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

                                        OS_SPRINTF(ucMessage,
                                        "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü!\n",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));
                                    }
                                    else
                                    {
                                        //inclassifyqoscfgkey.svlan_id = pstInclassifyQosData->svlan;
                                        //inclassifyqoscfgkey.cvlan_id = pstInclassifyQosData->cvlan;
                                        //pstinclassifyqoscb = 
                                            //AVLL_FIND(pstLogicalPort->inclassify_qos_tree, &inclassifyqoscfgkey);
                                        pstinclassifyqoscb = 
                                            AVLL_FIND(pstLogicalPort->inclassify_qos_tree, pstInclassifyQosData);
                                        
                                        //Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó1
                                        if (NULL == pstinclassifyqoscb)
                                        {
                                            pstinclassifyqoscb = spm_alloc_inclassify_qos_cb(NBB_CXT);

                                            if (NULL == pstinclassifyqoscb)/*lint !e613 */
                                            {
                                                OS_PRINTF("	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);

                                                OS_SPRINTF(ucMessage, 
                                                    "   ***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                                goto EXIT_LABEL;

                                            }

                                            //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
                                            //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
                                            //NBB_MEMCPY(&pstinclassifyqoscb->key,&inclassifyqoscfgkey,
                                                //sizeof(ATG_DCI_LOG_PORT_VLAN));
                                            NBB_MEMCPY(&(pstinclassifyqoscb->inclassify_qos_cfg),
                                            pstInclassifyQosData, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
                                            AVLL_INSERT(pstLogicalPort->inclassify_qos_tree, 
                                                pstinclassifyqoscb->spm_inclassify_qos_node);

                                            pstLogicalPort->inclassify_qos_num++;
                                        }
                                        else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
                                        {
                                            //pstdiffservcb->intf_pos_id = stIntfFlow.posId;
                                            NBB_MEMCPY(&(pstinclassifyqoscb->inclassify_qos_cfg), 
                                            pstInclassifyQosData, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
                                        }
                                    }                                
                                }


                            }
                            else
                            {
                                pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢inclassify_qosÊ§°Ü",
                                    ulPortIndexKey));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢inclassify_qosÊ§°Ü\n", ulPortIndexKey);

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢inclassify_qosÊ§°Ü\n",
                                    ulPortIndexKey);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
                                    BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
                            }                               
                    }
                }
            }
            else if (ulOperInclassifyQos == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucInclassifyQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->inclassify_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucInclassifyQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucInclassifyQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    OS_SPRINTF(ucMessage, "  14) ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL2 DEL\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->inclassify_qos_num; i++)
	                {
                        pstInclassifyQosData = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)
                            (pucInclassifyQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf(" 14) ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL2 DEL\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                        spm_dbg_print_logical_port_inclassify_qos_cfg(pstInclassifyQosData);
	                    }
                        
                           //OS_SPRINTF(ucMessage, "  14) ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL2 DEL\n");
                           //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                           
	                    NBB_TRC_DETAIL((NBB_FORMAT "  ¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÄ£°åË÷Òý = %d",
	                            pstInclassifyQosData->qos_policy_index));

                            if (ucIfExist == ATG_DCI_EXIST)
                            {
                                OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                                spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                                stSubPort.unit = stPortInfo.unit_id;
                                stSubPort.port = stPortInfo.port_id;
                                stSubPort.slot = stPortInfo.slot_id;
                                stSubPort.ivlan = stPortInfo.cvlan;
                                stSubPort.ovlan = stPortInfo.svlan;

                                ulVlanPosId = 0;
                                spm_get_poscnt_from_intf(ulPortIndexKey, 
                                pstInclassifyQosData->svlan, 
                                pstInclassifyQosData->cvlan, 
                                &ulVlanPosId NBB_CCXT);

                                stSubPort.posid = ulVlanPosId;
                                if(0 == ulVlanPosId)
                                {
                                    pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                    NBB_TRC_DETAIL((NBB_FORMAT
                                    "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü,posId=0!\n",
                                    ulPortIndexKey,pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

                                    OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü,posId=0!\n",
                                    ulPortIndexKey,pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

                                    OS_SPRINTF(ucMessage,
                                    "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü,posId=0!\n",
                                    ulPortIndexKey,pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                                }

                                else
                                {
                                    pstInclassifyQosData->qos_policy_index = 0;

                                    ret = spm_add_logic_flow_classify_node(ulPortIndexKey,
                                    pstInclassifyQosData,
                                    &stSubPort
                                    NBB_CCXT);
                                    
                                    if((2 == SHARED.c3_num) && 
                                         ((stPortInfo.port_type == ATG_DCI_LAG) 
                                         || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                                    {
                                        ul_vlanposid2 = 0;
                                        spm_get_poscnt_from_intf2(ulPortIndexKey, 
                                        pstInclassifyQosData->svlan, 
                                        pstInclassifyQosData->cvlan, 
                                        &ul_vlanposid2 NBB_CCXT);

                                        if(0 != ul_vlanposid2)
                                        {
                                            stSubPort.unit = 1;
                                            stSubPort.posid = ul_vlanposid2;
                                            ret += spm_add_logic_flow_classify_node(ulPortIndexKey,
                                            pstInclassifyQosData,
                                            &stSubPort
                                            NBB_CCXT);                                    
                                        }
                                    }                                    

                                    if (ret != SUCCESS)
                                    {
                                        pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                        NBB_TRC_DETAIL((NBB_FORMAT
                                        "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

                                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü\n",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

                                        OS_SPRINTF(ucMessage,
                                        "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü\n",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));
                                    }
                                    else
                                    {
                                        //inclassifyqoscfgkey.svlan_id = pstInclassifyQosData->svlan;
                                        //inclassifyqoscfgkey.cvlan_id = pstInclassifyQosData->cvlan;
                                        //pstinclassifyqoscb = 
                                            //AVLL_FIND(pstLogicalPort->inclassify_qos_tree, &inclassifyqoscfgkey);
                                        pstinclassifyqoscb = 
                                            AVLL_FIND(pstLogicalPort->inclassify_qos_tree, pstInclassifyQosData);
                                        
                                        //Èç¹û²»´æÔÚ£¬ÎÞ·¨É¾³ý
                                        if (NULL == pstinclassifyqoscb)
                                        {
                                            pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                            NBB_TRC_DETAIL((NBB_FORMAT 
                                            "  ***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄinclassify_qosÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý",
                                            ulPortIndexKey,
                                            pstInclassifyQosData->svlan,
                                            pstInclassifyQosData->cvlan));

                                            OS_PRINTF(
                                            "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄinclassify_qosÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
                                            ulPortIndexKey,
                                            pstInclassifyQosData->svlan,
                                            pstInclassifyQosData->cvlan);

                                            OS_SPRINTF(ucMessage,
                                            "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄinclassify_qosÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
                                            ulPortIndexKey,
                                            pstInclassifyQosData->svlan,
                                            pstInclassifyQosData->cvlan);
                                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", 
                                            ulPortIndexKey, ret,
                                            "CALL_FUNC_ERROR", ucMessage));
                                        }
                                        else
                                        {
                                            AVLL_DELETE(pstLogicalPort->inclassify_qos_tree, 
                                            pstinclassifyqoscb->spm_inclassify_qos_node);

                                            //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
                                            spm_free_inclassify_qos_cb(pstinclassifyqoscb NBB_CCXT);

                                            pstLogicalPort->inclassify_qos_num--;									
                                        }
                                    }                                      
                                }

                            } 
                            else
                            {
                                pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýinclassify_qosÊ§°Ü",
                                        ulPortIndexKey));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýinclassify_qosÊ§°Ü\n", ulPortIndexKey);

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýinclassify_qosÊ§°Ü\n",
                                    ulPortIndexKey);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
                                        BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
                            }                                  
	                }
                }
            }
            else if (ulOperInclassifyQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ÏÂ»°ÓÃ»§×éQOSÅäÖÃL2 ******************************/
            if (ulOperDownGroupQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucDownGroupQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->down_user_group_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucDownGroupQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDownGroupQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDownGroupQosData = (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)pucDownGroupQosDataStart;
                    
                    OS_SPRINTF(ucMessage, "  18£©ÏÂ»°ÓÃ»§×éQOSÅäÖÃL2\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  18£©ÏÂ»°ÓÃ»§×éQOSÅäÖÃL2\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  ÓÃ»§×éË÷Òý  = %d", pstDownGroupQosData->user_group_index));

                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                    ret = spm_hqos_add_group_node(stPortInfo.slot_id,
                        stPortInfo.port_id,
                        pstDownGroupQosData->user_group_index,
                        ulPortIndexKey
                        NBB_CCXT);

                    if (ret == SUCCESS)
                    {
                        /* ±£´æÊý¾Ý */
                        if (pstLogicalPort->down_user_group_qos_cfg_cb == NULL)
                        {
                            pstLogicalPort->down_user_group_qos_cfg_cb =
                                (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);
                        }

                        OS_MEMCPY(pstLogicalPort->down_user_group_qos_cfg_cb, pstDownGroupQosData,
                            sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS));
                    }
                    else
                    {
                        pstSetLogicalPort->down_user_queue_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() error",
                                ulPortIndexKey));

                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() ret=%d\n", ulPortIndexKey, ret);

                        OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() ret=%d\n",
                            ulPortIndexKey,
                            ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                "CALL_FUNC_ERROR", ucMessage));
                    }
                }
            }
            else if (ulOperDownGroupQos == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperDownGroupQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÅäÖÃL2 ******************************/
            if (ulOperDownUserQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucDownUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->down_user_queue_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucDownUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDownUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDownUserQosData = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)pucDownUserQosDataStart;
                    OS_SPRINTF(ucMessage, "  17£©ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÅäÖÃL2\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  17£©ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÅäÖÃL2\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  NodeIndex  = %d", pstDownUserQosData->node_index));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ÓÃ»§QOS²ßÂÔÄ£°åË÷Òý  = %d",
                            pstDownUserQosData->user_qos_policy_index));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ÓÅÏÈ¼¶¶ÓÁÐQOS²ßÂÔÄ£°åË÷Òý  = %d",
                            pstDownUserQosData->prio_queue_qos_policy_index));

                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                    stSubPort.unit = stPortInfo.unit_id;
                    stSubPort.port = stPortInfo.port_id;
                    stSubPort.slot = stPortInfo.slot_id;
                    stSubPort.ivlan = stPortInfo.cvlan;
                    stSubPort.ovlan = stPortInfo.svlan;
                    stSubPort.posid = stPortInfo.vp_pos_id;

                    if (pstLogicalPort->down_user_group_qos_cfg_cb != NULL)
                    {
                        ret = spm_hqos_add_usr_node(&stSubPort,
                            ulPortIndexKey,
                            pstLogicalPort->down_user_group_qos_cfg_cb->user_group_index,
                            pstDownUserQosData
                            NBB_CCXT);
                            
                        if (ret == SUCCESS)
                        {
                            /* ±£´æÊý¾Ý */
                            if (pstLogicalPort->down_user_qos_cfg_cb == NULL)
                            {
                                pstLogicalPort->down_user_qos_cfg_cb = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)NBB_MM_ALLOC(
                                    sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);
                            }

                            OS_MEMCPY(pstLogicalPort->down_user_qos_cfg_cb, pstDownUserQosData,
                                sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));
                        }
                        else
                        {
                            pstSetLogicalPort->down_user_queue_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() error",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() ret=%d\n",
                                ulPortIndexKey,
                                ret);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() ret=%d\n",
                                ulPortIndexKey,
                                ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                        }
                    }
                    else
                    {
                        pstSetLogicalPort->down_user_queue_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        NBB_TRC_DETAIL((NBB_FORMAT
                                "  ***ERROR***:PORT_INDEX=%ld µÄÏÂ»°ÓÃ»§×éQOSÅäÖÃÎ´ÏÂ£¬½¨Á¢ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÊ§°Ü",
                                ulPortIndexKey));

                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄÏÂ»°ÓÃ»§×éQOSÅäÖÃÎ´ÏÂ£¬½¨Á¢ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÊ§°Ü\n",
                            ulPortIndexKey);

                        OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld µÄÏÂ»°ÓÃ»§×éQOSÅäÖÃÎ´ÏÂ£¬½¨Á¢ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÊ§°Ü\n",
                            ulPortIndexKey);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
                                BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
                    }
                }
            }
            else if (ulOperDownUserQos == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperDownUserQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** °ü¹ýÂËÆ÷traffic_filter L2 ******************************/
            if (ulOperTrafficFilter == ATG_DCI_OPER_ADD)
			{

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucTrafficFilterDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->traffic_filter_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucTrafficFilterDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucTrafficFilterDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    //if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    //{
                        //printf("  20£©°ü¹ýÂËÆ÷traffic_filter L2 ADD\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    //}
                    OS_SPRINTF(ucMessage, "  20£©°ü¹ýÂËÆ÷traffic_filter L2 ADD \n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->traffic_filter_num; i++)
                    {
                        pstTrafficFilter = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)
                            (pucTrafficFilterDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER)))
                            * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            //spm_dbg_print_logical_port_ipv4_cfg(pstIpv4Data[i]);
                            printf("  20£©°ü¹ýÂËÆ÷traffic_filter L2 ADD \n");

                            spm_dbg_print_logical_port_traffic_filter_cfg(pstTrafficFilter);                            
                        }
                        
                        //OS_SPRINTF(ucMessage, "  20£©°ü¹ýÂËÆ÷traffic_filter L2 ADD\\n");
                        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
                        NBB_TRC_DETAIL((NBB_FORMAT "  acl_id  = %d", pstTrafficFilter->acl_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ·½Ïòdirection  = %d", pstTrafficFilter->direction));

                        if (ucIfExist == ATG_DCI_EXIST)
                        {
                            OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                            spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                            stSubPort.unit = stPortInfo.unit_id;
                            stSubPort.port = stPortInfo.port_id;
                            stSubPort.slot = stPortInfo.slot_id;
                            stSubPort.ivlan = stPortInfo.cvlan;
                            stSubPort.ovlan = stPortInfo.svlan;

                            ulVlanPosId = 0;
                            spm_get_poscnt_from_intf(ulPortIndexKey, 
                            pstTrafficFilter->svlan, 
                            pstTrafficFilter->cvlan, 
                            &ulVlanPosId NBB_CCXT);

                            stSubPort.posid = ulVlanPosId;

                            ret = spm_add_logic_acl_node(ulPortIndexKey,
                            pstTrafficFilter,
                            &stSubPort
                            NBB_CCXT);

                            if((2 == SHARED.c3_num) && 
                                 ((stPortInfo.port_type == ATG_DCI_LAG) || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                            {
                                ul_vlanposid2 = 0;
                                spm_get_poscnt_from_intf2(ulPortIndexKey, 
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan, 
                                &ul_vlanposid2 NBB_CCXT);

                                if(0 != ul_vlanposid2)
                                {
                                    stSubPort.unit = 1;
                                    stSubPort.posid = ul_vlanposid2;
                                    ret += spm_add_logic_acl_node(ulPortIndexKey,
                                    pstTrafficFilter,
                                    &stSubPort
                                    NBB_CCXT);                                    
                                }
                            } 
                            

                            if (ret != SUCCESS)
                            {
                                pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filter L2ÅäÖÃÊ§°Ü!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filter L2ÅäÖÃÊ§°Ü!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan);

                                OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filter L2ÅäÖÃÊ§°Ü!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                "CALL_FUNC_ERROR", ucMessage));
                            }                            
                            else
                            {
                                //psttrafficfiltercb = AVLL_FIND(pstLogicalPort->traffic_filter_tree, pstTrafficFilter);
                                //trafficfiltercfgkey.svlan_id = pstTrafficFilter->svlan;
                                //trafficfiltercfgkey.cvlan_id = pstTrafficFilter->cvlan;
                                //psttrafficfiltercb = 
                                    //AVLL_FIND(pstLogicalPort->traffic_filter_tree, &trafficfiltercfgkey);
                                psttrafficfiltercb = 
                                    AVLL_FIND(pstLogicalPort->traffic_filter_tree, pstTrafficFilter);
                                
                                //Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó1
                                if (NULL == psttrafficfiltercb)
                                {
                                    psttrafficfiltercb = spm_alloc_traffic_filter_cb(NBB_CXT);

                                    if (NULL == psttrafficfiltercb)/*lint !e613 */
                                    {
                                        OS_PRINTF("	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);

                                        OS_SPRINTF(ucMessage, "	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                        goto EXIT_LABEL;

                                    }

                                    //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
                                    //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
                                    //NBB_MEMCPY(&psttrafficfiltercb->key,
                                        //&trafficfiltercfgkey,sizeof(ATG_DCI_LOG_PORT_VLAN));
                                    NBB_MEMCPY(&(psttrafficfiltercb->traffic_filter_cfg),
                                    pstTrafficFilter, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                                    AVLL_INSERT(pstLogicalPort->traffic_filter_tree, 
                                        psttrafficfiltercb->spm_traffic_filter_node);

                                    pstLogicalPort->traffic_filter_num++;
                                }
                                else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
                                {
                                    //pstdiffservcb->intf_pos_id = stIntfFlow.posId;
                                    NBB_MEMCPY(&(psttrafficfiltercb->traffic_filter_cfg), 
                                    pstTrafficFilter, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                                }
                            }                                
                        }
                        else
                        {
                            pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢traffic_filterÊ§°Ü",
                            ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢traffic_filterÊ§°Ü\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢traffic_filterÊ§°Ü\n",
                            ulPortIndexKey);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
                            BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
                        }  
#if 0
                        if (ret == SUCCESS)
                        {
                            iCfgPos = spm_check_if_acl_exist(ulPortIndexKey, pstTrafficFilter);

                            if (iCfgPos <= 0)
                            {
                                for (j = 0; j < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; j++)
                                {
                                    /* ±£´æÊý¾Ý */
                                    if (pstLogicalPort->traffic_filter_cfg_cb[j] == NULL)
                                    {
                                        pstLogicalPort->traffic_filter_cfg_cb[j] =
                                            (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)NBB_MM_ALLOC(sizeof(
                                                ATG_DCI_LOG_PORT_TRAFFIC_FILTER),
                                            NBB_NORETRY_ACT,
                                            MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);

	                                    OS_MEMCPY(pstLogicalPort->traffic_filter_cfg_cb[j], pstTrafficFilter,
	                                        sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));

	                                    pstLogicalPort->traffic_filter_num++;

	                                    break;
                                    }
                                }
                            }
                            else
                            {
                                OS_MEMCPY(pstLogicalPort->traffic_filter_cfg_cb[iCfgPos - 1],
                                    pstTrafficFilter,
                                    sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                            }
                        }
                        else
                        {
                            pstSetLogicalPort->traffic_filter_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü\n",
                                ulPortIndexKey);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                        }

#endif
                    }
                }
            }
			else if (ulOperTrafficFilter == ATG_DCI_OPER_DEL)
			{

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucTrafficFilterDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->traffic_filter_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucTrafficFilterDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucTrafficFilterDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    //if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    //{
                        //printf("  20£©°ü¹ýÂËÆ÷traffic_filter L2 DEL\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    //}
                    
                    OS_SPRINTF(ucMessage, "  20£©°ü¹ýÂËÆ÷traffic_filter L2 DEL\\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->traffic_filter_num; i++)
                    {
                        pstTrafficFilter = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)
                            (pucTrafficFilterDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER)))
                            * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            //spm_dbg_print_logical_port_ipv4_cfg(pstIpv4Data[i]);
                            printf("  20£©°ü¹ýÂËÆ÷traffic_filter L2 DEL\n");
                            spm_dbg_print_logical_port_traffic_filter_cfg(pstTrafficFilter);
                        }
                        
                        //OS_SPRINTF(ucMessage, "  20£©°ü¹ýÂËÆ÷traffic_filter L2 DEL\\n");
                        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
                        NBB_TRC_DETAIL((NBB_FORMAT "  acl_id  = %d", pstTrafficFilter->acl_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ·½Ïòdirection  = %d", pstTrafficFilter->direction));
                        
                        if (ucIfExist == ATG_DCI_EXIST)
                        {
                            OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                            spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                            stSubPort.unit = stPortInfo.unit_id;
                            stSubPort.port = stPortInfo.port_id;
                            stSubPort.slot = stPortInfo.slot_id;
                            stSubPort.ivlan = stPortInfo.cvlan;
                            stSubPort.ovlan = stPortInfo.svlan;

                            ulVlanPosId = 0;
                            spm_get_poscnt_from_intf(ulPortIndexKey, 
                            pstTrafficFilter->svlan, 
                            pstTrafficFilter->cvlan, 
                            &ulVlanPosId NBB_CCXT);

                            stSubPort.posid = ulVlanPosId;

                            pstTrafficFilter->acl_id = 0;

                            ret = spm_add_logic_acl_node(ulPortIndexKey,
                            pstTrafficFilter,
                            &stSubPort
                            NBB_CCXT);
                            
                            if((2 == SHARED.c3_num) && 
                                 ((stPortInfo.port_type == ATG_DCI_LAG) || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                            {
                                ul_vlanposid2 = 0;
                                spm_get_poscnt_from_intf2(ulPortIndexKey, 
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan, 
                                &ul_vlanposid2 NBB_CCXT);

                                if(0 != ul_vlanposid2)
                                {
                                    stSubPort.unit = 1;
                                    stSubPort.posid = ul_vlanposid2;
                                    ret += spm_add_logic_acl_node(ulPortIndexKey,
                                    pstTrafficFilter,
                                    &stSubPort
                                    NBB_CCXT);                                    
                                }
                            }                            

                            if (ret != SUCCESS)
                            {
                                pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                NBB_TRC_DETAIL((NBB_FORMAT 
                                "  ***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d"
                                "É¾³ýSVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filter L2ÅäÖÃÊ§°Ü!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d"
                                "É¾³ýSVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filter L2ÅäÖÃÊ§°Ü!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan);

                                OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d"
                                "É¾³ýSVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filter L2ÅäÖÃÊ§°Ü!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan);
                                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                "CALL_FUNC_ERROR", ucMessage));
                            }

                            else
                            {
                                //psttrafficfiltercb = AVLL_FIND(pstLogicalPort->traffic_filter_tree, pstTrafficFilter);
                                //psttrafficfiltercb = AVLL_FIND(pstLogicalPort->traffic_filter_tree, pstTrafficFilter);
                                //trafficfiltercfgkey.svlan_id = pstTrafficFilter->svlan;
                                //trafficfiltercfgkey.cvlan_id = pstTrafficFilter->cvlan;
                                //psttrafficfiltercb = 
                                    //AVLL_FIND(pstLogicalPort->traffic_filter_tree, &trafficfiltercfgkey);
                                psttrafficfiltercb = 
                                    AVLL_FIND(pstLogicalPort->traffic_filter_tree, pstTrafficFilter);

                                //Èç¹û²»´æÔÚ£¬ÎÞ·¨É¾³ý
                                if (NULL == psttrafficfiltercb)
                                {
                                    pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                    NBB_TRC_DETAIL((NBB_FORMAT 
                                    "  ***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄtraffic_filterÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý",
                                    ulPortIndexKey,
                                    pstTrafficFilter->svlan,
                                    pstTrafficFilter->cvlan));

                                    OS_PRINTF(
                                    "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄtraffic_filterÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
                                    ulPortIndexKey,
                                    pstTrafficFilter->svlan,
                                    pstTrafficFilter->cvlan);

                                    OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d µÄtraffic_filterÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
                                    ulPortIndexKey,
                                    pstTrafficFilter->svlan,
                                    pstTrafficFilter->cvlan);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", 
                                    ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                                }
                                else
                                {
                                    AVLL_DELETE(pstLogicalPort->traffic_filter_tree, 
                                    psttrafficfiltercb->spm_traffic_filter_node);

                                    //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
                                    spm_free_traffic_filter_cb(psttrafficfiltercb NBB_CCXT);

                                    pstLogicalPort->traffic_filter_num--;									
                                }
                            }                        
                        }
                        else
                        {
                            pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýtraffic_filterÊ§°Ü",
                            ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýtraffic_filterÊ§°Ü\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld µÄÂß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýtraffic_filterÊ§°Ü\n",
                            ulPortIndexKey);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
                            BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
                        }      
                        
#if 0
                        if (ret == SUCCESS)
                        {
                            iCfgPos = spm_check_if_acl_exist(ulPortIndexKey, pstTrafficFilter);

                            if (iCfgPos <= 0)
                            {
                                for (j = 0; j < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; j++)
                                {
                                    /* ±£´æÊý¾Ý */
                                    if (pstLogicalPort->traffic_filter_cfg_cb[j] == NULL)
                                    {
                                        pstLogicalPort->traffic_filter_cfg_cb[j] =
                                            (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)NBB_MM_ALLOC(sizeof(
                                                ATG_DCI_LOG_PORT_TRAFFIC_FILTER),
                                            NBB_NORETRY_ACT,
                                            MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);

	                                    OS_MEMCPY(pstLogicalPort->traffic_filter_cfg_cb[j], pstTrafficFilter,
	                                        sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));

	                                    pstLogicalPort->traffic_filter_num++;

	                                    break;
                                    }
                                }
                            }
                            else
                            {
                                OS_MEMCPY(pstLogicalPort->traffic_filter_cfg_cb[iCfgPos - 1],
                                    pstTrafficFilter,
                                    sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                            }
                        }
                        else
                        {
                            pstSetLogicalPort->traffic_filter_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü\n",
                                ulPortIndexKey);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                        }

#endif
                    }
                }
            }
            else if (ulOperTrafficFilter == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }
            
            /****************************** Diff-ServÅäÖÃL2£¨VPÏÂ»°UNI²à£© ******************************/
            if (ulOperDsL2 == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucDsL2DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ds_l2_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucDsL2DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDsL2DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDsL2Data = (ATG_DCI_LOG_PORT_DS_L2_DATA *)pucDsL2DataStart;
                    OS_SPRINTF(ucMessage, "  23£©Diff-ServÅäÖÃL2£¨VPÏÂ»°UNI²à£©\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  23£©Diff-ServÅäÖÃL2£¨VPÏÂ»°UNI²à£©\n");

                        spm_dbg_print_logical_port_ds_l2_cfg(pstDsL2Data);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  Diff-ServÄ£°åË÷Òý = %d", pstDsL2Data->ds_id));

                    //OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                    //spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

					stUniVp.vpn_id = pstLogicalPort->logic_port_info_cb.vpn_id;
					stUniVp.vp_type = pstLogicalPort->logic_port_info_cb.vpn_type;
					stUniVp.vp_posid = pstLogicalPort->logic_port_info_cb.vp_idx;

                    ret = spm_ds_add_logic_univp_node(ulPortIndexKey,
													&stUniVp,
							                        pstDsL2Data
							                        NBB_CCXT);
                        
                    if (ret == SUCCESS)
                    {
                        /* ±£´æÊý¾Ý */
                        if (pstLogicalPort->ds_l2_cfg_cb == NULL)
                        {
                            pstLogicalPort->ds_l2_cfg_cb = (ATG_DCI_LOG_PORT_DS_L2_DATA *)NBB_MM_ALLOC(
                                sizeof(ATG_DCI_LOG_PORT_DS_L2_DATA),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LOG_PORT_DS_L2_CB);
                        }

                        OS_MEMCPY(pstLogicalPort->ds_l2_cfg_cb, pstDsL2Data,
                            sizeof(ATG_DCI_LOG_PORT_DS_L2_DATA));
                    }
                    else
                    {
                        pstSetLogicalPort->ds_l2_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_univp_node() error",
                                ulPortIndexKey));

                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_univp_node() ret=%d\n",
                            ulPortIndexKey,
                            ret);

                        OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_univp_node() ret=%d\n",
                            ulPortIndexKey,
                            ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                "CALL_FUNC_ERROR", ucMessage));
                    }
                }
            }
            else if (ulOperDsL2 == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperDsL2 == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }
        }

        /****************************************************************************/
        /* L3¶Ë¿Ú                                                                   */
        /****************************************************************************/
        else if ((pstLogicalPort->basic_cfg_cb != NULL)
            && (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L3))
        {
            /****************************** Âß¼­ÅäÖÃL3 ******************************/
            if (ulOperLogicL3 == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucLogicL3DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->logical_l3_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucLogicL3DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  LOGICAL PORT pucLogicL3DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstLogicL3Data = (ATG_DCI_LOG_PORT_LOGIC_L3_DATA *)pucLogicL3DataStart;

                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  3£©Âß¼­ÅäÖÃL3\n");
                        spm_dbg_print_logical_port_logic_l3_cfg(pstLogicL3Data);
                    }

                    OS_SPRINTF(ucMessage, "  3£©Âß¼­ÅäÖÃL3\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    //spm_dbg_record_logical_port_logic_l3_cfg(pstLogicL3Data);

                    //NBB_TRC_DETAIL((NBB_FORMAT "  CONTROL VID = %d", pstLogicL3Data->control_vid));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IPV4 URPFÊ¹ÄÜ = %d", pstLogicL3Data->ipv4_urpf_mode));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IPV4 È±Ê¡Â·ÓÉ¼ì²é = %s",
                            pstLogicL3Data->ipv4_default_check ? "Ê¹ÄÜ" : "²»Ê¹ÄÜ"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IPV6 URPFÊ¹ÄÜ = %d", pstLogicL3Data->ipv6_urpf_mode));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IPV6 È±Ê¡Â·ÓÉ¼ì²é = %s",
                            pstLogicL3Data->ipv6_default_check ? "Ê¹ÄÜ" : "²»Ê¹ÄÜ"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  VRF ID = %d", pstLogicL3Data->vrf_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  INGRESS = %s", spm_set_ulong_to_ipv4(pstLogicL3Data->ingress  NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  EGRESS = %s", spm_set_ulong_to_ipv4(pstLogicL3Data->egress NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  TUNNEL_ID = %ld", pstLogicL3Data->tunnel_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  MPLS×ª·¢Ê¹ÄÜ = %s", pstLogicL3Data->mpls_forward_enable ? "Ê¹ÄÜ" : "²»Ê¹ÄÜ"));  

                    stIntfL3.mCardPort = pstLogicalPort->port_id;
                    stIntfL3.bindId = pstLogicL3Data->vrf_id;
                    stIntfL3.eIntfType = INTF_TYPE_L3;

                    #if 0
                    //IPV4 URPFÊ¹ÄÜ
                    if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFÈ±Ê¡¼ì²é
                    {
                        stIntfL3.eIpv4UrpfMode = URPF_OFF;
                    }
                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPFÑÏ¸ñ¼ì²é
                    {
                        stIntfL3.eIpv4UrpfMode = URPF_STRICT;
                    }
                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPFËÉÉ¢¼ì²é
                    {
                        stIntfL3.eIpv4UrpfMode = URPF_LOOSE;
                    }

					//IPV6 URPFÊ¹ÄÜ
                    if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFÈ±Ê¡¼ì²é
                    {
                        stIntfL3.eIpv6UrpfMode = URPF_OFF;
                    }
                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPFÑÏ¸ñ¼ì²é
                    {
                        stIntfL3.eIpv6UrpfMode = URPF_STRICT;
                    }
                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPFËÉÉ¢¼ì²é
                    {
                        stIntfL3.eIpv6UrpfMode = URPF_LOOSE;
                    }

                    //È±Ê¡Â·ÓÉ¼ì²éÊ¹ÄÜ
                    if (pstLogicL3Data->ipv4_default_check == ENABLE)
                    {
                        stIntfL3.flags |= INTF_V4_ALLOW_DEFAULT_ROUTE;
                    }

                    if (pstLogicL3Data->ipv6_default_check == ENABLE)
                    {
                        stIntfL3.flags |= INTF_V6_ALLOW_DEFAULT_ROUTE;
                    }
                    
                    //MPLS×ª·¢²»Ê¹ÄÜ
                    if (pstLogicL3Data->mpls_forward_enable == DISABLE)
                    {
                        stIntfL3.flags |= INTF_MPLS_DISABLE;
                    }
                    
                    //stIntfL3.flags |= INTF_COUNTER_ON;
                    #endif

					//Èç¹ûVlanÊôÐÔ¸öÊý²»µÈÓÚ0£¬ËµÃ÷ÅäÖÃÁËintf£¬ÐèÒªÉ¾³ý£¬ÖØ½¨
					if (pstLogicalPort->terminal_if_num != 0)
					{
					    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
					         pstTerminalIfCb != NULL;
					         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
					                       pstTerminalIfCb->spm_terminal_if_node))
					    {
					    	if (pstTerminalIfCb->intf_pos_id != 0)
					    	{

								//×¢ÊÍµô£¬½â¾ö¸üÐÂL3ÅäÖÃÊ± É¾³ý ÔÙ Ôö¼Ó INTF ºóQOSÅäÖÃÃ»ÓÐÁË£¬¸Ä³ÉMod INTF
					    		//ApiC3DelIntf(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id);

					    		//almpm_delline_intf(ulPortIndexKey, pstTerminalIfCb->intf_pos_id, 1);

                                //ÓÉÓÚµ÷ÓÃModIntf½Ó¿Ú»á³åµôqosÅäÖÃ£¬¸ÄÎªµ÷ÓÃÉèÖÃ¸÷¸ö×Ö¶ÎµÄ×Ó½Ó¿Ú¡
                                
			                    //ApiC3SetIntfVrf(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,pstLogicL3Data->vrf_id); //to do

                                stIntfL3.posId = pstTerminalIfCb->intf_pos_id;

                                #ifdef SPU

                                //Ë¢ÐÂvrfid
                                ret = ApiC3SetIntfBindId(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, INTF_TYPE_L3, pstLogicL3Data->vrf_id);
                                if(SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfBindId() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3SetIntfBindId() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            	"***ERROR***:ApiC3SetIntfBindId() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
                                }

                                //IPV4 URPFÊ¹ÄÜ
			                    if (pstLogicL3Data->ipv4_default_check == ENABLE)
                                {
                                    if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFÈ±Ê¡¼ì²é
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_OFF,ENABLE);
                                        
                                    }
                                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPFÑÏ¸ñ¼ì²é
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_STRICT,ENABLE);
                                       
                                    }
                                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPFËÉÉ¢¼ì²é
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_LOOSE,ENABLE);
                                    } 
                                }    

                                else if(pstLogicL3Data->ipv4_default_check == DISABLE)
                                {
                                    if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFÈ±Ê¡¼ì²é
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_OFF,DISABLE);
                                        
                                    }
                                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPFÑÏ¸ñ¼ì²é
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_STRICT,DISABLE);
                                       
                                    }
                                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPFËÉÉ¢¼ì²é
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_LOOSE,DISABLE);
                                    } 
                                }    

                                if(SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfV4UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3SetIntfV4UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            	"***ERROR***:ApiC3SetIntfV4UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
                                }

                                if (pstLogicL3Data->ipv6_default_check == ENABLE)
                                {
                                    if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFÈ±Ê¡¼ì²é
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_OFF,ENABLE);
                                    }
                                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPFÑÏ¸ñ¼ì²é
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_STRICT,ENABLE);
                                    }
                                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPFËÉÉ¢¼ì²é
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_LOOSE,ENABLE);
                                    }
                                }

                                else if (pstLogicL3Data->ipv6_default_check == DISABLE)
                                {
                                    if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFÈ±Ê¡¼ì²é
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_OFF,DISABLE);
                                    }
                                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPFÑÏ¸ñ¼ì²é
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_STRICT,DISABLE);
                                    }
                                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPFËÉÉ¢¼ì²é
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_LOOSE,DISABLE);
                                    }
                                }

                                if(SUCCESS != ret)
                                {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfV6UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü",
                                                ret,
                                                ulPortIndexKey,
                                                pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                                pstTerminalIfCb->terminal_if_cfg.cvlan_id));

                                        OS_PRINTF("***ERROR***:ApiC3SetIntfV6UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü\n",
                                                ret,
                                                ulPortIndexKey,
                                                pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                                pstTerminalIfCb->terminal_if_cfg.cvlan_id);

                                        OS_SPRINTF(ucMessage,
                                                "***ERROR***:ApiC3SetIntfV6UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü\n",
                                                ret,
                                                ulPortIndexKey,
                                                pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                                pstTerminalIfCb->terminal_if_cfg.cvlan_id);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                                "CALL_FUNC_ERROR", ucMessage));
                                    
                                }
                                
                                //MPLS×ª·¢²»Ê¹ÄÜ
                                if (pstLogicL3Data->mpls_forward_enable == DISABLE)
                                {
                                    ApiC3SetIntfMplsDisable(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,ENABLE);//Ê¹ÄÜ:0/1-enable /disable
                                }

                                else if(pstLogicL3Data->mpls_forward_enable == ENABLE)
                                {
                                    ApiC3SetIntfMplsDisable(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,DISABLE);//Ê¹ÄÜ:0/1-enable /disable
                                }

                                if(SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfMplsDisable() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü",
                                            ret,
                                            ulPortIndexKey,
                                            pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                            pstTerminalIfCb->terminal_if_cfg.cvlan_id));

                                    OS_PRINTF("***ERROR***:ApiC3SetIntfMplsDisable() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü\n",
                                            ret,
                                            ulPortIndexKey,
                                            pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                            pstTerminalIfCb->terminal_if_cfg.cvlan_id);

                                    OS_SPRINTF(ucMessage,
                                            "***ERROR***:ApiC3SetIntfMplsDisable() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ Âß¼­ÅäÖÃL3 Ë¢ÐÂÊ§°Ü\n",
                                            ret,
                                            ulPortIndexKey,
                                            pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                            pstTerminalIfCb->terminal_if_cfg.cvlan_id);
                                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                            "CALL_FUNC_ERROR", ucMessage));
                                    
                                }

                                #endif
                                
#if 0


			                    if ((pstTerminalIfCb->terminal_if_cfg.svlan_id == 0) 
			                     && (pstTerminalIfCb->terminal_if_cfg.cvlan_id == 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
				                    {
				                        stIntfL3.mIntfMatchType = INTF_MATCH_L3_VE_PORT;
				                    }
				                    else
				                    {
				                    	stIntfL3.mIntfMatchType = INTF_MATCH_PORT;
				                    }
			                        
			                        stIntfL3.mOvid = 0;
			                        stIntfL3.mIvid = 0;
			                    }

			                    //Dot1qÖÕ½á×Ó½Ó¿ÚÓ¦ÓÃÓÚL3VPN£¬SVLANÓÐÐ§¡¢CVLANÎÞÐ§
			                    else if ((pstTerminalIfCb->terminal_if_cfg.svlan_id != 0) 
			                          && (pstTerminalIfCb->terminal_if_cfg.cvlan_id == 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
				                    {
				                        stIntfL3.mIntfMatchType = INTF_MATCH_L3_VE_PORT_OVID;
				                    }
				                    else
				                    {
				                    	stIntfL3.mIntfMatchType = INTF_MATCH_PORT_OVID;
				                    }
			                        
			                        stIntfL3.mOvid = pstTerminalIfCb->terminal_if_cfg.svlan_id;
			                        stIntfL3.mIvid = 0;
			                    }

			                    //QinQÖÕ½á×Ó½Ó¿ÚÓ¦ÓÃÓÚL3VPN£¬SVLANºÍCVLANÍ¬Ê±ÓÐÐ§
			                    else if ((pstTerminalIfCb->terminal_if_cfg.svlan_id != 0) 
			                          && (pstTerminalIfCb->terminal_if_cfg.cvlan_id != 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
				                    {
				                        stIntfL3.mIntfMatchType = INTF_MATCH_L3_VE_PORT_OVID_IVID;
				                    }
				                    else
				                    {
				                    	stIntfL3.mIntfMatchType = INTF_MATCH_PORT_OVID_IVID;
				                    }
			                        
			                        stIntfL3.mOvid = pstTerminalIfCb->terminal_if_cfg.svlan_id;
			                        stIntfL3.mIvid = pstTerminalIfCb->terminal_if_cfg.cvlan_id;
			                    }
			                    else
			                    {
			                        OS_PRINTF("***WARNNING***: PORT_INDEX=%ld Ö»ÅäÖÃÁËCVlan£¬Î´ÅäÖÃSVlan\n", ulPortIndexKey);
			                    }

			                    stIntfL3.posId = pstTerminalIfCb->intf_pos_id;


	                            ret = ApiC3ModIntf(pstLogicalPort->unit_id, &stIntfL3);
#endif

	                            if (ret == SUCCESS)
	                            {
#ifdef SPU

	                            	pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
	                            	almpm_addcfg_intf_onePos(ulPortIndexKey, stIntfL3.posId);
#endif
/*
									//Èç¹ûÊÇÎïÀí½Ó¿Ú(Ö÷½Ó¿Ú)£¬ÏÈÅÐ¶ÏÊÇ·ñÅäÖÃÁËIP£¬Ö»ÓÐÅäÖÃÁËIPºó²ÅÊ¹ÄÜ
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
									{
										if ((pstLogicalPort->ipv4_num == 0) && (pstLogicalPort->ipv6_num == 0))
										{
											ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, stIntfL3.posId, DISABLE);
											OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d DISABLE\n",
				                        		ret,
				                        		stIntfL3.posId);
										}
										else
										{
											ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, stIntfL3.posId, ENABLE);
											OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d ENABLE\n",
				                        		ret,
				                        		stIntfL3.posId);
										}
									}
*/
	                            }
	                            

					    	}
					    }
					}

                    /* ±£´æÊý¾Ý */
                    if (pstLogicalPort->logic_l3_cfg_cb == NULL)
                    {
                        pstLogicalPort->logic_l3_cfg_cb =
                            (ATG_DCI_LOG_PORT_LOGIC_L3_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_LOGIC_L3_DATA),
                            NBB_NORETRY_ACT,
                            MEM_SPM_LOG_PORT_L3_CB);
                    }

                    OS_MEMCPY(pstLogicalPort->logic_l3_cfg_cb, pstLogicL3Data,
                        sizeof(ATG_DCI_LOG_PORT_LOGIC_L3_DATA));

                }
            }
            else if (ulOperLogicL3 == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperLogicL3 == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** Ipv4µØÖ·ÅäÖÃL3 ******************************/
            if (ulOperIpv4 == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucIpv4DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ipv4_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucIpv4DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv4DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  7) Ipv4µØÖ·ÅäÖÃL3 (ipv4_num = %d) ADD\n", pstSetLogicalPort->ipv4_num);
                    }

                    OS_SPRINTF(ucMessage, "  7) Ipv4µØÖ·ÅäÖÃL3 (ipv4_num = %d) ADD\n", 
                        pstSetLogicalPort->ipv4_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->ipv4_num; i++)
                    {
                        pstIpv4Data[i] = (ATG_DCI_LOG_PORT_IPV4_DATA *)
                            (pucIpv4DataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_ipv4_cfg(pstIpv4Data[i]);
                        }

                        //spm_dbg_record_logical_port_ipv4_cfg(pstIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPµØÖ·Ç°×º = %s", spm_set_ulong_to_ipv4(pstIpv4Data[i]->ip_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", pstIpv4Data[i]->mask_len));

                        //¼ì²éipv4ÊÇ·ñÅäÖÃ¹ý
                        iCfgPos = spm_check_if_ipv4_exist(ulPortIndexKey, pstIpv4Data[i] NBB_CCXT);

                        //ÎÞÖØ¸´
                        if (iCfgPos <= 0)
                        {
                            for (j = 0; j < ATG_DCI_LOG_PORT_IPV4_NUM; j++)
                            {
                                //Èç¹ûÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÎªÎÞÊý¾Ý£¬¿ÉÒÔ´æ´¢Êý¾Ý
                                if (pstLogicalPort->ipv4_cfg_cb[j] == NULL)
                                {
                                    pstLogicalPort->ipv4_cfg_cb[j] =
                                        (ATG_DCI_LOG_PORT_IPV4_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA),
                                        NBB_NORETRY_ACT,
                                        MEM_SPM_LOG_PORT_IPV4_CB);
                                    OS_MEMCPY(pstLogicalPort->ipv4_cfg_cb[j], pstIpv4Data[i],
                                        sizeof(ATG_DCI_LOG_PORT_IPV4_DATA));

                                    pstLogicalPort->ipv4_num++;

                                    break;
                                }
                            }
                        }
                        else    //ÓÐÖØ¸´£¬¸üÐÂÅäÖÃ
                        {
                            OS_MEMCPY(pstLogicalPort->ipv4_cfg_cb[iCfgPos - 1], pstIpv4Data[i],
                                sizeof(ATG_DCI_LOG_PORT_IPV4_DATA));
                        }
                    }

#ifdef SPU
/*
				    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
				         pstTerminalIfCb != NULL;
				         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
				                       pstTerminalIfCb->spm_terminal_if_node))
				    {
				    	if (pstTerminalIfCb->intf_pos_id != 0)
				    	{
							//Èç¹ûÊÇÎïÀí½Ó¿Ú(Ö÷½Ó¿Ú)£¬ÏÈÅÐ¶ÏÊÇ·ñÅäÖÃÁËIP£¬Ö»ÓÐÅäÖÃÁËIPºó²ÅÊ¹ÄÜ
							if ((pstLogicalPort->basic_cfg_cb != NULL)
							 && (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
							 && (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L3))
							{
								if ((pstLogicalPort->ipv4_num == 0) && (pstLogicalPort->ipv6_num == 0))
								{
									ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, DISABLE);
									OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d DISABLE\n",
		                        		ret,
		                        		pstTerminalIfCb->intf_pos_id);
								}
								else
								{
									ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, ENABLE);
									OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d ENABLE\n",
		                        		ret,
		                        		pstTerminalIfCb->intf_pos_id);
								}
							}
				    	}
				    }
*/
#endif
                }
            }
            else if (ulOperIpv4 == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucIpv4DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ipv4_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucIpv4DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv4DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  7) Ipv4µØÖ·ÅäÖÃL3 (ipv4_num = %d) DEL\n", pstSetLogicalPort->ipv4_num);
                    }

                    OS_SPRINTF(ucMessage, "  7) Ipv4µØÖ·ÅäÖÃL3 (ipv4_num = %d) DEL\n", 
                        pstSetLogicalPort->ipv4_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->ipv4_num; i++)
                    {
                        pstIpv4Data[i] = (ATG_DCI_LOG_PORT_IPV4_DATA *)
                            (pucIpv4DataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_ipv4_cfg(pstIpv4Data[i]);
                        }

                        //spm_dbg_record_logical_port_ipv4_cfg(pstIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPµØÖ·Ç°×º = %s", spm_set_ulong_to_ipv4(pstIpv4Data[i]->ip_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", pstIpv4Data[i]->mask_len));

                        for (j = 0; j < ATG_DCI_LOG_PORT_IPV4_NUM; j++)
                        {
                            //Èç¹û²»ÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÓÐÊý¾Ý£¬¿ÉÒÔÉ¾³ýÊý¾Ý
                            if ((pstLogicalPort->ipv4_cfg_cb[j] != NULL)
                                && (!(spm_ipv4_key_compare(pstIpv4Data[i], pstLogicalPort->ipv4_cfg_cb[j] NBB_CCXT))))
                            {
                                NBB_MM_FREE(pstLogicalPort->ipv4_cfg_cb[j], MEM_SPM_LOG_PORT_IPV4_CB);
                                pstLogicalPort->ipv4_cfg_cb[j] = NULL;

                                pstLogicalPort->ipv4_num--;

                                break;
                            }
                        }
                    }
#ifdef SPU
/*
				    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
				         pstTerminalIfCb != NULL;
				         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
				                       pstTerminalIfCb->spm_terminal_if_node))
				    {
				    	if (pstTerminalIfCb->intf_pos_id != 0)
				    	{
							//Èç¹ûÊÇÎïÀí½Ó¿Ú(Ö÷½Ó¿Ú)£¬ÏÈÅÐ¶ÏÊÇ·ñÅäÖÃÁËIP£¬Ö»ÓÐÅäÖÃÁËIPºó²ÅÊ¹ÄÜ
							if ((pstLogicalPort->basic_cfg_cb != NULL)
							 && (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
							 && (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L3))
							{
								if ((pstLogicalPort->ipv4_num == 0) && (pstLogicalPort->ipv6_num == 0))
								{
									ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, DISABLE);
									OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d DISABLE\n",
		                        		ret,
		                        		pstTerminalIfCb->intf_pos_id);
								}
								else
								{
									ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, ENABLE);
									OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d ENABLE\n",
		                        		ret,
		                        		pstTerminalIfCb->intf_pos_id);
								}
							}
				    	}
				    }
*/
#endif
                }
            }
            else if (ulOperIpv4 == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** IPv6µØÖ·ÅäÖÃL3 ******************************/
            if (ulOperIpv6 == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucIpv6DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ipv6_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucIpv6DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  8) Ipv6µØÖ·ÅäÖÃL3 (ipv6_num = %d) ADD\n", pstSetLogicalPort->ipv6_num);
                    }

                    OS_SPRINTF(ucMessage, "  8) Ipv6µØÖ·ÅäÖÃL3 (ipv6_num = %d) ADD\n", 
                        pstSetLogicalPort->ipv6_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->ipv6_num; i++)
                    {
                        pstIpv6Data[i] = (ATG_DCI_LOG_PORT_IPV6_DATA *)
                            (pucIpv6DataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_ipv6_cfg(pstIpv6Data[i]);
                        }

                        //spm_dbg_record_logical_port_ipv6_cfg(pstIpv6Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV6µØÖ·Ç°×º = %x-%x-%x-%x", pstIpv6Data[i]->ip_addr[0],
                                pstIpv6Data[i]->ip_addr[1],
                                pstIpv6Data[i]->ip_addr[2],
                                pstIpv6Data[i]->ip_addr[3]));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", pstIpv6Data[i]->mask_len));

                        //¼ì²éipv6ÊÇ·ñÅäÖÃ¹ý
                        iCfgPos = spm_check_if_ipv6_exist(ulPortIndexKey, pstIpv6Data[i] NBB_CCXT);

                        //ÎÞÖØ¸´
                        if (iCfgPos <= 0)
                        {
                            for (j = 0; j < ATG_DCI_LOG_PORT_IPV6_NUM; j++)
                            {
                                //Èç¹ûÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÎªÎÞÊý¾Ý£¬¿ÉÒÔ´æ´¢Êý¾Ý
                                if (pstLogicalPort->ipv6_cfg_cb[j] == NULL)
                                {
                                    pstLogicalPort->ipv6_cfg_cb[j] =
                                        (ATG_DCI_LOG_PORT_IPV6_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA),
                                        NBB_NORETRY_ACT,
                                        MEM_SPM_LOG_PORT_IPV6_CB);
                                    OS_MEMCPY(pstLogicalPort->ipv6_cfg_cb[j], pstIpv6Data[i],
                                        sizeof(ATG_DCI_LOG_PORT_IPV6_DATA));

                                    pstLogicalPort->ipv6_num++;

                                    break;
                                }
                            }
                        }
                        else
                        {
                            OS_MEMCPY(pstLogicalPort->ipv6_cfg_cb[iCfgPos - 1], pstIpv6Data[i],
                                sizeof(ATG_DCI_LOG_PORT_IPV6_DATA));
                        }
                    }
#ifdef SPU
/*
				    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
				         pstTerminalIfCb != NULL;
				         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
				                       pstTerminalIfCb->spm_terminal_if_node))
				    {
				    	if (pstTerminalIfCb->intf_pos_id != 0)
				    	{
							//Èç¹ûÊÇÎïÀí½Ó¿Ú(Ö÷½Ó¿Ú)£¬ÏÈÅÐ¶ÏÊÇ·ñÅäÖÃÁËIP£¬Ö»ÓÐÅäÖÃÁËIPºó²ÅÊ¹ÄÜ
							if ((pstLogicalPort->basic_cfg_cb != NULL)
							 && (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
							 && (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L3))
							{
								if ((pstLogicalPort->ipv4_num == 0) && (pstLogicalPort->ipv6_num == 0))
								{
									ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, DISABLE);
									OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d DISABLE\n",
		                        		ret,
		                        		pstTerminalIfCb->intf_pos_id);
								}
								else
								{
									ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, ENABLE);
									OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d ENABLE\n",
		                        		ret,
		                        		pstTerminalIfCb->intf_pos_id);
								}
							}
				    	}
				    }
*/
#endif
                }
            }
            else if (ulOperIpv6 == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucIpv6DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ipv6_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucIpv6DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  8) Ipv6µØÖ·ÅäÖÃL3 (ipv6_num = %d) DEL\n", pstSetLogicalPort->ipv6_num);
                    }

                    OS_SPRINTF(ucMessage, "  8) Ipv6µØÖ·ÅäÖÃL3 (ipv6_num = %d) DEL\n", 
                        pstSetLogicalPort->ipv6_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->ipv6_num; i++)
                    {
                        pstIpv6Data[i] = (ATG_DCI_LOG_PORT_IPV6_DATA *)
                            (pucIpv6DataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_ipv6_cfg(pstIpv6Data[i]);
                        }

                        //spm_dbg_record_logical_port_ipv6_cfg(pstIpv6Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV6µØÖ·Ç°×º = %x-%x-%x-%x", pstIpv6Data[i]->ip_addr[0],
                                pstIpv6Data[i]->ip_addr[1],
                                pstIpv6Data[i]->ip_addr[2],
                                pstIpv6Data[i]->ip_addr[3]));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", pstIpv6Data[i]->mask_len));

                        for (j = 0; j < ATG_DCI_LOG_PORT_IPV6_NUM; j++)
                        {
                            //Èç¹û²»ÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÓÐÊý¾Ý£¬¿ÉÒÔÉ¾³ýÊý¾Ý
                            if ((pstLogicalPort->ipv6_cfg_cb[j] != NULL)
                                && (!(spm_ipv6_key_compare(pstIpv6Data[i], pstLogicalPort->ipv6_cfg_cb[j] NBB_CCXT))))
                            {
                                NBB_MM_FREE(pstLogicalPort->ipv6_cfg_cb[j], MEM_SPM_LOG_PORT_IPV6_CB);
                                pstLogicalPort->ipv6_cfg_cb[j] = NULL;

                                pstLogicalPort->ipv6_num--;

                                break;
                            }
                        }
                    }
#ifdef SPU
/*
				    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
				         pstTerminalIfCb != NULL;
				         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
				                       pstTerminalIfCb->spm_terminal_if_node))
				    {
				    	if (pstTerminalIfCb->intf_pos_id != 0)
				    	{
							//Èç¹ûÊÇÎïÀí½Ó¿Ú(Ö÷½Ó¿Ú)£¬ÏÈÅÐ¶ÏÊÇ·ñÅäÖÃÁËIP£¬Ö»ÓÐÅäÖÃÁËIPºó²ÅÊ¹ÄÜ
							if ((pstLogicalPort->basic_cfg_cb != NULL)
							 && (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
							 && (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L3))
							{
								if ((pstLogicalPort->ipv4_num == 0) && (pstLogicalPort->ipv6_num == 0))
								{
									ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, DISABLE);
									OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d DISABLE\n",
		                        		ret,
		                        		pstTerminalIfCb->intf_pos_id);
								}
								else
								{
									ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, ENABLE);
									OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d ENABLE\n",
		                        		ret,
		                        		pstTerminalIfCb->intf_pos_id);
								}
							}
				    	}
				    }
*/
#endif
                }
            }
            else if (ulOperIpv6 == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ×é²¥×éµØÖ·L3 ******************************/
            if (ulOperMcIpv4 == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucMcMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->mc_ipv4_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucMcMacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucMcMacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  9) ×é²¥×éµØÖ·L3 (mc_ipv4_num = %d) ADD\n", pstSetLogicalPort->mc_ipv4_num);
                    }

                    OS_SPRINTF(ucMessage, "  9) ×é²¥×éµØÖ·L3 (mc_ipv4_num = %d) ADD\n", 
                        pstSetLogicalPort->mc_ipv4_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->mc_ipv4_num; i++)
                    {
                        pstMcIpv4Data[i] = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)
                            (pucMcMacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);
                        }

                        //spm_dbg_record_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV4×é²¥µØÖ· = %s",
                                spm_set_ulong_to_ipv4(pstMcIpv4Data[i]->mc_ipv4_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", pstMcIpv4Data[i]->mask_len));

						//µ÷ÓÃdk½Ó¿Ú
						if ((pstLogicalPort->logic_l3_cfg_cb != NULL) && (pstLogicalPort->basic_cfg_cb != NULL))
						{
							//Èç¹ûÊÇVE¿Ú£¬¸ù¾ÝÇý¶¯Ìá¹©µÄËã·¨: x*2 + 1 + 0x800£¬¼ÆËã³öport_id·¢¸øÇý¶¯
							if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)
							{
								usMcVePortId = (pstLogicalPort->port_id * 2 + 1) + SPM_MC_VE_PORT_OFFSET;
								spm_l3_addonemcip(usMcVePortId, pstLogicalPort->logic_l3_cfg_cb->vrf_id, 0, &pstMcIpv4Data[i]->mc_ipv4_addr);
							}
							else
							{
								spm_l3_addonemcip(pstLogicalPort->port_id, pstLogicalPort->logic_l3_cfg_cb->vrf_id, 0, &pstMcIpv4Data[i]->mc_ipv4_addr);							
							}
						}
						else
						{
	                        pstSetLogicalPort->mc_ipv4_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬×é²¥×éµØÖ·L3Ê§°Ü",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬×é²¥×éµØÖ·L3Ê§°Ü\n", ulPortIndexKey);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬×é²¥×éµØÖ·L3Ê§°Ü\n",
	                            ulPortIndexKey);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
	                                BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
						}

                        iCfgPos = spm_check_if_mc_ipv4_exist(ulPortIndexKey, pstMcIpv4Data[i] NBB_CCXT);

                        if (iCfgPos <= 0)
                        {
                            for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV4_NUM; j++)
                            {
                                //Èç¹ûÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÎªÎÞÊý¾Ý£¬¿ÉÒÔ´æ´¢Êý¾Ý
                                if (pstLogicalPort->mc_ipv4_cfg_cb[j] == NULL)
                                {
                                    pstLogicalPort->mc_ipv4_cfg_cb[j] = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)NBB_MM_ALLOC(
                                        sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA),
                                        NBB_NORETRY_ACT,
                                        MEM_SPM_LOG_PORT_MC_IPV4_CB);
                                    OS_MEMCPY(pstLogicalPort->mc_ipv4_cfg_cb[j], pstMcIpv4Data[i],
                                        sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA));

                                    pstLogicalPort->mc_ipv4_num++;

                                    break;
                                }
                            }
                        }
                        else
                        {
                            OS_MEMCPY(pstLogicalPort->mc_ipv4_cfg_cb[iCfgPos - 1], pstMcIpv4Data[i],
                                sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA));
                        }
                    }
                }
            }
            else if (ulOperMcIpv4 == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucMcMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->mc_ipv4_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucMcMacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucMcMacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  9) ×é²¥×éµØÖ·L3 (mc_ipv4_num = %d) DEL\n", pstSetLogicalPort->mc_ipv4_num);
                    }

                    OS_SPRINTF(ucMessage, "  9) ×é²¥×éµØÖ·L3 (mc_ipv4_num = %d) DEL\n", 
                        pstSetLogicalPort->mc_ipv4_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->mc_ipv4_num; i++)
                    {
                        pstMcIpv4Data[i] = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)
                            (pucMcMacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);
                        }

                        //spm_dbg_record_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV4×é²¥µØÖ· = %s",
                                spm_set_ulong_to_ipv4(pstMcIpv4Data[i]->mc_ipv4_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", pstMcIpv4Data[i]->mask_len));

                        for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV4_NUM; j++)
                        {
                            //Èç¹û²»ÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÓÐÊý¾Ý£¬¿ÉÒÔÉ¾³ýÊý¾Ý
                            if ((pstLogicalPort->mc_ipv4_cfg_cb[j] != NULL)
                                && (!(spm_ipv4_key_compare(pstMcIpv4Data[i], pstLogicalPort->mc_ipv4_cfg_cb[j] NBB_CCXT))))
                            {
                                NBB_MM_FREE(pstLogicalPort->mc_ipv4_cfg_cb[j], MEM_SPM_LOG_PORT_MC_IPV4_CB);
                                pstLogicalPort->mc_ipv4_cfg_cb[j] = NULL;

                                pstLogicalPort->mc_ipv4_num--;

                                break;
                            }
                        }
                    }
                }
            }
            else if (ulOperMcIpv4 == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

	    	/****************************** IPV6×é²¥×éµØÖ·L3 ******************************/
            if (ulOperMcIpv6 == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucIpv6McMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->mc_ipv6_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucIpv6McMacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6McMacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  22) IPV6×é²¥×éµØÖ·L3 (mc_ipv6_num = %d) ADD\n", pstSetLogicalPort->mc_ipv6_num);
                    }

                    OS_SPRINTF(ucMessage, "  22) IPV6×é²¥×éµØÖ·L3 (mc_ipv6_num = %d) ADD\n", 
                        pstSetLogicalPort->mc_ipv6_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->mc_ipv6_num; i++)
                    {
                        pstMcIpv6Data[i] = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)
                            (pucIpv6McMacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */

						if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_mc_ipv6_cfg(pstMcIpv6Data[i]);
                        }

                        //spm_dbg_record_logical_port_mc_ipv6_cfg(pstMcIpv6Data[i]);
						
                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV6×é²¥×éµØÖ· = %x-%x-%x-%x", pstMcIpv6Data[i]->mc_ipv6_addr[0],
                                pstMcIpv6Data[i]->mc_ipv6_addr[1],
                                pstMcIpv6Data[i]->mc_ipv6_addr[2],
                                pstMcIpv6Data[i]->mc_ipv6_addr[3]));
                        NBB_TRC_DETAIL((NBB_FORMAT "  Ç°×º³¤¶È = %d", pstMcIpv6Data[i]->mask_len));

						//µ÷ÓÃdk½Ó¿Ú
						if ((pstLogicalPort->logic_l3_cfg_cb != NULL) && (pstLogicalPort->basic_cfg_cb != NULL))
						{
							//Èç¹ûÊÇVE¿Ú£¬¸ù¾ÝÇý¶¯Ìá¹©µÄËã·¨: x*2 + 1 + 0x800£¬¼ÆËã³öport_id·¢¸øÇý¶¯
							if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)
							{
								usMcVePortId = (pstLogicalPort->port_id * 2 + 1) + SPM_MC_VE_PORT_OFFSET;
								spm_l3_addonemcip(usMcVePortId, pstLogicalPort->logic_l3_cfg_cb->vrf_id, 1, pstMcIpv6Data[i]->mc_ipv6_addr);
							}
							else
							{
								spm_l3_addonemcip(pstLogicalPort->port_id, pstLogicalPort->logic_l3_cfg_cb->vrf_id, 1, pstMcIpv6Data[i]->mc_ipv6_addr);
							}
						}
						else
						{
	                        pstSetLogicalPort->mc_ipv6_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬IPV6×é²¥×éµØÖ·L3Ê§°Ü",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬IPV6×é²¥×éµØÖ·L3Ê§°Ü\n", ulPortIndexKey);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬IPV6×é²¥×éµØÖ·L3Ê§°Ü\n",
	                            ulPortIndexKey);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
	                                BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
						}

                        iCfgPos = spm_check_if_mc_ipv6_exist(ulPortIndexKey, pstMcIpv6Data[i] NBB_CCXT);

                        if (iCfgPos <= 0)
                        {
                            for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV6_NUM; j++)
                            {
                                //Èç¹ûÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÎªÎÞÊý¾Ý£¬¿ÉÒÔ´æ´¢Êý¾Ý
                                if (pstLogicalPort->mc_ipv6_cfg_cb[j] == NULL)
                                {
                                    pstLogicalPort->mc_ipv6_cfg_cb[j] = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)NBB_MM_ALLOC(
                                        sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA),
                                        NBB_NORETRY_ACT,
                                        MEM_SPM_LOG_PORT_MC_IPV6_CB);
                                    OS_MEMCPY(pstLogicalPort->mc_ipv6_cfg_cb[j], pstMcIpv6Data[i],
                                        sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA));

                                    pstLogicalPort->mc_ipv6_num++;

                                    break;
                                }
                            }
                        }
                        else
                        {
                            OS_MEMCPY(pstLogicalPort->mc_ipv6_cfg_cb[iCfgPos - 1], pstMcIpv6Data[i],
                                sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA));
                        }
                    }
                }
            }
            else if (ulOperMcIpv6 == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucIpv6McMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->mc_ipv6_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucIpv6McMacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6McMacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  22) IPV6×é²¥×éµØÖ·L3 (mc_ipv6_num = %d) DEL\n", pstSetLogicalPort->mc_ipv6_num);
                    }

                    OS_SPRINTF(ucMessage, "  22) IPV6×é²¥×éµØÖ·L3 (mc_ipv6_num = %d) DEL\n", 
                        pstSetLogicalPort->mc_ipv6_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->mc_ipv6_num; i++)
                    {
                        pstMcIpv6Data[i] = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)
                            (pucIpv6McMacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
						#if 0
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);
                        }

                        spm_dbg_record_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV4×é²¥µØÖ· = %s",
                                spm_set_ulong_to_ipv4(pstMcIpv4Data[i]->mc_ipv4_addr)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", pstMcIpv4Data[i]->mask_len));
						#endif

                        for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV6_NUM; j++)
                        {
                            //Èç¹û²»ÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÓÐÊý¾Ý£¬¿ÉÒÔÉ¾³ýÊý¾Ý
                            if ((pstLogicalPort->mc_ipv6_cfg_cb[j] != NULL)
                                && (!(spm_ipv6_key_compare(pstMcIpv6Data[i], pstLogicalPort->mc_ipv6_cfg_cb[j] NBB_CCXT))))
                            {
                                NBB_MM_FREE(pstLogicalPort->mc_ipv6_cfg_cb[j], MEM_SPM_LOG_PORT_MC_IPV6_CB);
                                pstLogicalPort->mc_ipv6_cfg_cb[j] = NULL;

                                pstLogicalPort->mc_ipv6_num--;

                                break;
                            }
                        }
                    }
                }
            }
            else if (ulOperMcIpv6 == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** VRRPÐéÄâMACºÍIPµØÖ·ÅäÖÃL3 ******************************/
            if (ulOperVipVmac == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucVipVmacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->vip_vmac_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucVipVmacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucVipVmacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  10) ÐéÄâMACºÍIPµØÖ·ÅäÖÃL3 (vip_vmac_num = %d) ADD\n",
                            pstSetLogicalPort->vip_vmac_num);
                    }

                    OS_SPRINTF(ucMessage,
                        "  10) ÐéÄâMACºÍIPµØÖ·ÅäÖÃL3 (vip_vmac_num = %d) ADD\n",
                        pstSetLogicalPort->vip_vmac_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    //ÕÒµ½vlanÊôÐÔµÚÒ»¸öÅäÖÃ
                    pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);

                    //Ã»ÓÐvlanÊôÐÔ£¬ÎÞ·¨°ó¶¨£¬´òÓ¡´íÎó£¬Ö±½ÓÍË³ö
                    if(NULL == pstTerminalIfCb)
                    {
                        //´Ë´ÎÏÂ·¢µÄËùÓÐVRRPÅäÖÃ¾ù·µ»ØÊ§°Ü
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄ Âß¼­¶Ë¿ÚVLANÊôÐÔ²»´æÔÚ£¬ÎÞ·¨ÉèÖÃVRRP!\n",
			                        		ulPortIndexKey));

                        OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld µÄ Âß¼­¶Ë¿ÚVLANÊôÐÔ²»´æÔÚ£¬ÎÞ·¨ÉèÖÃVRRP!\n",
			                        		ulPortIndexKey);

                        OS_SPRINTF(ucMessage,	"  ***ERROR***:PORT_INDEX=%ld µÄ Âß¼­¶Ë¿ÚVLANÊôÐÔ²»´æÔÚ£¬ÎÞ·¨ÉèÖÃVRRP!\n",
			                            	ulPortIndexKey);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, BASIC_CONFIG_NOT_EXIST,
			                                "BASIC_CONFIG_NOT_EXIST", ucMessage));		
                    }
                    else
                    {
                        //¼ÇÂ¼Ê×¸öVLANÊôÐÔÅäÖÃµÄµØÖ·
                        tempterminalifcb = pstTerminalIfCb;
						
                        //³õÊ¼»¯VrrpµÄ¹«¹²³ÉÔ±
                        stvrrp.l3_port_id = pstLogicalPort->port_id;/*lint !e613 */
			   stvrrp.slot = pstLogicalPort->slot_id;/*lint !e613 */

                        //VE¿ÚÖ»ÐèÖÃVE±êÖ¾£¬·ÇVE¿ÚÐèÒªÌîvlanÐÅÏ¢
                        if(ATG_DCI_VE_PORT == pstLogicalPort->basic_cfg_cb->port_sub_type)/*lint !e613 */
                        {
                            stvrrp.ve_flag = 1;
                        }

                        //¸ÃÂß¼­½Ó¿ÚÏÂµÄËùÓÐINTF¶¼ÒªÊ¹ÄÜVRRP
#ifdef SPU                        
                        while (pstTerminalIfCb)
                        {

                            ret = fhdrv_psn_l3_set_intf_vrrp(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, 1);
                            
                            if((2 == SHARED.c3_num) && 
                                ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                           {                              
                                ret += fhdrv_psn_l3_set_intf_vrrp(1, pstTerminalIfCb->intf2_pos_id, 1);
                           }

                            if (SUCCESS != ret)
                            {
                                   for (i = 0; i < pstSetLogicalPort->vip_vmac_num; i++)
                                   {
                                   	pstSetLogicalPort->vip_vmac_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                                   }                          	

                            	NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:fhdrv_psn_l3_set_intf_vrrp()"
                                    " ret=%d PORT_INDEX=%ld µÄintf_pos_id=%dÊ¹ÄÜÊ§°Ü\n",
	                        		ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id));

                            	OS_PRINTF("  ***ERROR***:fhdrv_psn_l3_set_intf_vrrp() ret=%d "
                                    "PORT_INDEX=%ld µÄintf_pos_id=%dÊ¹ÄÜÊ§°Ü\n",
	                        		ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id);

                            	OS_SPRINTF(ucMessage,
    	                            "  ***ERROR***:fhdrv_psn_l3_set_intf_vrrp() ret=%d "
    	                            "PORT_INDEX=%ld µÄintf_pos_id=%dÊ¹ÄÜÊ§°Ü\n",
	                            	ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id);
                            	BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            	NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", 
                                    ulPortIndexKey, CALL_C3_FUNC_ERROR,
	                                "CALL_C3_FUNC_ERROR", ucMessage));

                                   break;
                            }
							
                            pstTerminalIfCb = AVLL_NEXT(pstLogicalPort->terminal_if_tree, 
                                pstTerminalIfCb->spm_terminal_if_node);
                        }
#endif

                        for (i = 0; i < pstSetLogicalPort->vip_vmac_num; i++)
                        {
                            pstVipVmacData[i] = (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)
                                (pucVipVmacDataStart + 
                                (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))) * i);

                            /* ÅäÖÃ´¦Àí£¬todo */
                            if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                            {
                                spm_dbg_print_logical_port_vip_vmac_cfg(pstVipVmacData[i]);
                            }

                            //spm_dbg_record_logical_port_vip_vmac_cfg(pstVipVmacData[i]);

                            NBB_TRC_DETAIL((NBB_FORMAT "  MACµØÖ· = %0x-%0x-%0x-%0x-%0x-%0x", 
                                pstVipVmacData[i]->mac[0],
                                pstVipVmacData[i]->mac[1],
                                pstVipVmacData[i]->mac[2],
                                pstVipVmacData[i]->mac[3],
                                pstVipVmacData[i]->mac[4],
                                pstVipVmacData[i]->mac[5]));
                            NBB_TRC_DETAIL((NBB_FORMAT "  IPv4µØÖ· = %s", 
                                spm_set_ulong_to_ipv4(pstVipVmacData[i]->ip_addr NBB_CCXT)));
                            NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", 
                                pstVipVmacData[i]->mask_len));

                            iCfgPos = spm_check_if_vip_vmac_exist(ulPortIndexKey, 
                                pstVipVmacData[i] NBB_CCXT);

                            //¸ù¾Ý±¾vrrpÊôÐÔË¢ÐÂÇý¶¯µÄvrrpÅäÖÃ
                            stvrrp.dmac[0] = pstVipVmacData[i]->mac[0];
                            stvrrp.dmac[1] = pstVipVmacData[i]->mac[1];
                            stvrrp.dmac[2] = pstVipVmacData[i]->mac[2];
                            stvrrp.dmac[3] = pstVipVmacData[i]->mac[3];
                            stvrrp.dmac[4] = pstVipVmacData[i]->mac[4];
                            stvrrp.dmac[5] = pstVipVmacData[i]->mac[5];


			       //Ê×¸öVLANÊôÐÔÅäÖÃµÄµØÖ·
	                     pstTerminalIfCb =  tempterminalifcb ;
#ifdef SRC				   
			       terminalifnum = pstLogicalPort->terminal_if_num;
#endif	

#ifdef SPU	      
                            terminalifnum = 0;	

                        //¸ÃÂß¼­½Ó¿ÚÏÂµÄËùÓÐINTF¶¼ÒªÊ¹ÄÜVRRP
                        while (pstTerminalIfCb)
                        {

				stvrrp.ovlan_id = pstTerminalIfCb->terminal_if_cfg.svlan_id;
				stvrrp.ivlan_id = pstTerminalIfCb->terminal_if_cfg.cvlan_id;

				//Íâ²ãVLAN
				if(0  !=  stvrrp.ovlan_id)/*lint !e613 */
				{
					stvrrp.ovlan_id_mask = 0xfff;
				}
				else
				{
					stvrrp.ovlan_id_mask = 0;
				}

				//ÄÚÍâ²ãVLAN
				if(0  !=  stvrrp.ivlan_id)/*lint !e613 */
				{
					stvrrp.ivlan_id_mask = 0xfff;
				}
				else
				{
				 	stvrrp.ivlan_id_mask = 0;
				}						

		              ret = SUCCESS;
			       ret = fhdrv_psn_acl_set_vrrp(pstLogicalPort->unit_id, &stvrrp);
                            vrrp4_pos_id_temp = stvrrp.posid;
                            stvrrp.posid = 0;
                   
                            if((2 == SHARED.c3_num) && 
                                ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                           {
                                
                                ret += fhdrv_psn_acl_set_vrrp(1, &stvrrp);
                           }                   
                   
				if(SUCCESS != ret)
                            {
                            	pstSetLogicalPort->vip_vmac_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            	NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:fhdrv_psn_acl_set_vrrp()"
                                            " ret=%d PORT_INDEX=%ld°ó¶¨VRRPÊ§°Ü\n",
    		                        		ret, ulPortIndexKey));

                            	OS_PRINTF("  ***ERROR***:fhdrv_psn_acl_set_vrrp() ret=%d PORT_INDEX=%ld°ó¶¨VRRPÊ§°Ü\n",
    		                        		ret, ulPortIndexKey);

                            	OS_SPRINTF(ucMessage,
    		                            "  ***ERROR***:fhdrv_psn_acl_set_vrrp() ret=%d PORT_INDEX=%ld °ó¶¨VRRPÊ§°Ü\n",
    		                            	ret, ulPortIndexKey);
                            	BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            	NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_C3_FUNC_ERROR,
    		                                "CALL_C3_FUNC_ERROR", ucMessage));

                                break;
                            }
				
				terminalifnum++;
							
                            pstTerminalIfCb = AVLL_NEXT(pstLogicalPort->terminal_if_tree, 
                                pstTerminalIfCb->spm_terminal_if_node);
                        }
 #endif                           


                            if(terminalifnum == pstLogicalPort->terminal_if_num)
                            {
                                if (iCfgPos <= 0)
                                {
                                    for (j = 0; j < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; j++)
                                    {
                                        //Èç¹ûÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÎªÎÞÊý¾Ý£¬¿ÉÒÔ´æ´¢Êý¾Ý
                                        if (pstLogicalPort->vip_vmac_cfg_cb[j] == NULL)/*lint !e613 */
                                        {
                                            pstLogicalPort->vip_vmac_cfg_cb[j] =
                                                (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)NBB_MM_ALLOC(sizeof(
                                                    ATG_DCI_LOG_PORT_VIP_VMAC_DATA),
                                                NBB_NORETRY_ACT,
                                                MEM_SPM_LOG_PORT_VIP_VMAC_CB);/*lint !e613 */

                                            if(pstLogicalPort->vip_vmac_cfg_cb[j] == NULL)/*lint !e613 */
                                            {
                                                OS_PRINTF("	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                            	
                                    			OS_SPRINTF(ucMessage, "	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                                    			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                                goto EXIT_LABEL;
                                    			
                                            }
                                            OS_MEMCPY(pstLogicalPort->vip_vmac_cfg_cb[j], pstVipVmacData[i],
                                                sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA));/*lint !e613 */

                                            pstLogicalPort->logic_port_info_cb.ulVrrpMacPosId[j] =    /*lint !e613 */
                                                vrrp4_pos_id_temp;
                                            pstLogicalPort->logic_port_info_cb.vrrp_ipv4_posid2[j] =    /*lint !e613 */
                                                stvrrp.posid;
                                            pstLogicalPort->vip_vmac_num++;/*lint !e613 */
        
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    OS_MEMCPY(pstLogicalPort->vip_vmac_cfg_cb[iCfgPos - 1], pstVipVmacData[i],
                                        sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA));/*lint !e613 */
                                }
                            }
                        }
                    }


                }
            }
            else if (ulOperVipVmac == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucVipVmacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->vip_vmac_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucVipVmacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucVipVmacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  10) ÐéÄâMACºÍIPµØÖ·ÅäÖÃL3 (vip_vmac_num = %d) DEL\n",
                            pstSetLogicalPort->vip_vmac_num);
                    }

                    OS_SPRINTF(ucMessage,
                        "  10) ÐéÄâMACºÍIPµØÖ·ÅäÖÃL3 (vip_vmac_num = %d) DEL\n",
                        pstSetLogicalPort->vip_vmac_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->vip_vmac_num; i++)
                    {
                        pstVipVmacData[i] = (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)
                            (pucVipVmacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_vip_vmac_cfg(pstVipVmacData[i]);
                        }

                        //spm_dbg_record_logical_port_vip_vmac_cfg(pstVipVmacData[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  MACµØÖ· = %0x-%0x-%0x-%0x-%0x-%0x", pstVipVmacData[i]->mac[0],
                                pstVipVmacData[i]->mac[1],
                                pstVipVmacData[i]->mac[2],
                                pstVipVmacData[i]->mac[3],
                                pstVipVmacData[i]->mac[4],
                                pstVipVmacData[i]->mac[5]));
                        NBB_TRC_DETAIL((NBB_FORMAT "  IPv4µØÖ· = %s", 
                            spm_set_ulong_to_ipv4(pstVipVmacData[i]->ip_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", pstVipVmacData[i]->mask_len));

                        iCfgPos = spm_check_if_vip_vmac_exist(ulPortIndexKey, pstVipVmacData[i] NBB_CCXT);
                        if(iCfgPos < 0)//²»´æÔÚ£¬ÎÞ·¨É¾³ý
                        {
                            pstSetLogicalPort->vip_vmac_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­¶Ë¿Ú²»´æÔÚVRRP-MACµØÖ· "
                                            "=%2x-%2x-%2x-%2x-%2x-%2x,ÎÞ·¨É¾³ý! \n",
			                        		ulPortIndexKey, pstVipVmacData[i]->mac[0],
			                        		pstVipVmacData[i]->mac[1],
			                        		pstVipVmacData[i]->mac[2],
			                        		pstVipVmacData[i]->mac[3],
			                        		pstVipVmacData[i]->mac[4],
			                        		pstVipVmacData[i]->mac[5]);

                            OS_SPRINTF(ucMessage,
			                            "  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­¶Ë¿Ú²»´æÔÚVRRP-MACµØÖ· "
    			                            "=%2x-%2x-%2x-%2x-%2x-%2x,ÎÞ·¨É¾³ý! \n",
			                            	ulPortIndexKey, pstVipVmacData[i]->mac[0],
			                        		pstVipVmacData[i]->mac[1],
			                        		pstVipVmacData[i]->mac[2],
			                        		pstVipVmacData[i]->mac[3],
			                        		pstVipVmacData[i]->mac[4],
			                        		pstVipVmacData[i]->mac[5]);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        }
                        else
                        {
#ifdef SPU                        
                            stvrrp.posid = 
                                pstLogicalPort->logic_port_info_cb.ulVrrpMacPosId[iCfgPos - 1];/*lint !e613 */
                            ret = SUCCESS;

                            ret = fhdrv_psn_acl_del_vrrp(pstLogicalPort->unit_id, &stvrrp);
                   
                            if((2 == SHARED.c3_num) && 
                                ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                           {
                                stvrrp.posid = 
                                    pstLogicalPort->logic_port_info_cb.vrrp_ipv4_posid2[iCfgPos - 1];
                                if(0 != stvrrp.posid)
                                {
                                    ret += fhdrv_psn_acl_del_vrrp(1, &stvrrp);
                                }                                
                           } 

                            if(SUCCESS != ret)
                            {
                                pstSetLogicalPort->vip_vmac_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            	NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld"
                                            " µÄvrrp_pos_id=%dÉ¾³ýÊ§°Ü\n",
			                        		ret, ulPortIndexKey, 
			                        		pstLogicalPort->logic_port_info_cb.ulVrrpMacPosId[iCfgPos - 1]));

                            	OS_PRINTF(
						"  ***ERROR***:fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld µÄvrrp_pos_id=%dÉ¾³ýÊ§°Ü\n",
			                        		ret, ulPortIndexKey, 
			                        		pstLogicalPort->logic_port_info_cb.ulVrrpMacPosId[iCfgPos - 1]);

                            	OS_SPRINTF(ucMessage,
			                            "  ***ERROR***:fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld µÄvrrp_pos_id=%dÉ¾³ýÊ§°Ü\n",
			                            	ret, ulPortIndexKey, 
			                            	pstLogicalPort->logic_port_info_cb.ulVrrpMacPosId[iCfgPos - 1]);
                            	BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            	NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_C3_FUNC_ERROR,
			                                "CALL_C3_FUNC_ERROR", ucMessage));
                                continue;
                            }
#endif

                            for (j = 0; j < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; j++)
                            {
                                //Èç¹û²»ÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÓÐÊý¾Ý£¬¿ÉÒÔÉ¾³ýÊý¾Ý
                                if ((pstLogicalPort->vip_vmac_cfg_cb[j] != NULL)
                                    && (!(spm_ip_mac_key_compare(pstVipVmacData[i], 
                                    pstLogicalPort->vip_vmac_cfg_cb[j] NBB_CCXT))))/*lint !e613 */
                                {
                                    NBB_MM_FREE(pstLogicalPort->vip_vmac_cfg_cb[j], /*lint !e613 */
                                        MEM_SPM_LOG_PORT_VIP_VMAC_CB);
                                    pstLogicalPort->vip_vmac_cfg_cb[j] = NULL;/*lint !e613 */
    
                                    pstLogicalPort->logic_port_info_cb.ulVrrpMacPosId[j] = 0;/*lint !e613 */
                                    
                                    if((2 == SHARED.c3_num) && 
                                        ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                        || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                                   {
                                        pstLogicalPort->logic_port_info_cb.vrrp_ipv4_posid2[j] = 0;/*lint !e613 */
                                   } 
                                    
                                    pstLogicalPort->vip_vmac_num--;/*lint !e613 */
    
                                    break;
                                }
                            }
                        }

                    }
                }
            }
            else if (ulOperVipVmac == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

	        /******************************** VE MACµØÖ·ÅäÖÃ *********************************/
	        if (ulOperVeMac == ATG_DCI_OPER_ADD)
	        {

	            /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
	            pucVeMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->ve_mac_data);

	            /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
	            if (pucVeMacDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  LOGICAL PORT pucVeMacDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {
	                pstVeMacData = (ATG_DCI_LOG_PORT_VE_MAC_DATA *)pucVeMacDataStart;

	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  11£©VE MACµØÖ·ÅäÖÃ\n");
	                    spm_dbg_print_logical_port_ve_mac_cfg(pstVeMacData);
	                }

	                OS_SPRINTF(ucMessage, "  11£©VE MACµØÖ·ÅäÖÃ\n");
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
	                //spm_dbg_record_logical_port_ve_mac_cfg(pstVeMacData);

	                NBB_TRC_DETAIL((NBB_FORMAT "  MACµØÖ· = %0x-%0x-%0x-%0x-%0x-%0x", pstVeMacData->mac[0],
	                        pstVeMacData->mac[1],
	                        pstVeMacData->mac[2],
	                        pstVeMacData->mac[3],
	                        pstVeMacData->mac[4],
	                        pstVeMacData->mac[5]));
	                        
	                ve_mac[0] = pstVeMacData->mac[0];
	                ve_mac[1] = pstVeMacData->mac[1];
	                ve_mac[2] = pstVeMacData->mac[2];
	                ve_mac[3] = pstVeMacData->mac[3];
	                ve_mac[4] = pstVeMacData->mac[4];
	                ve_mac[5] = pstVeMacData->mac[5];

#ifdef SPU
					if ((pstLogicalPort->basic_cfg_cb->port_flag != ATG_DCI_TUNNEL)
					 && (pstLogicalPort->basic_cfg_cb->port_flag != ATG_DCI_LOOPBACK)
					 && (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT))
					{
		                //Èç¹ûÌõÄ¿ÎªÐÂÔö£¬ÇÒÖ®Ç°ÅäÖÃ¹ýÒÔÌ«ÍøÎïÀíÅäÖÃ
		                if (pstLogicalPort->phy_cfg_cb != NULL)
		                {
		                    for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
		                    {
		                        ret = ApiC3SetPortMac(ucC3Unit, ATG_DCI_IS_VE, pstLogicalPort->phy_cfg_cb->ve_group_id, ve_mac);
		                        NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%d, ApiC3SetPortMac() ret=%d", ulPortIndexKey, ret));
		                    }
		                }
		                else
		                {
		                    pstSetLogicalPort->ve_mac_return_code = ATG_DCI_RC_UNSUCCESSFUL;
		                    OS_PRINTF("PORT_INDEX=%ld, VE MACµØÖ·ÅäÖÃÊ§Åä£¬group_idÎÞ·¨»ñÈ¡!\n", ulPortIndexKey);
		                }
					}

#endif

	                /* ±£´æÊý¾Ý */
	                if (pstLogicalPort->ve_mac_cfg_cb == NULL)
	                {
	                    pstLogicalPort->ve_mac_cfg_cb =
	                        (ATG_DCI_LOG_PORT_VE_MAC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_PORT_VE_MAC_DATA),
	                        NBB_NORETRY_ACT,
	                        MEM_SPM_LOG_PORT_VE_MAC_CB);
	                }

	                OS_MEMCPY(pstLogicalPort->ve_mac_cfg_cb, pstVeMacData, sizeof(ATG_DCI_LOG_PORT_VE_MAC_DATA));
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
        
	        /****************************** VLANÊôÐÔ L3´¦Àí******************************/
	        if (ulOperTerminalIf == ATG_DCI_OPER_ADD)
			{

	            /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
	            pucTerminalIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->vlan_data);

	            /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
	            if (pucTerminalIfDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  pucTerminalIfDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {

	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  21£©VLANÊôÐÔ L3 (num = %d) ADD\n", pstSetLogicalPort->vlan_num);
	                }

	                OS_SPRINTF(ucMessage, "  21£©VLANÊôÐÔ L3 (num = %d) ADD\n", pstSetLogicalPort->vlan_num);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                //Èç¹ûÎªBROADCAST¿Ú²Å½¨INTF£¬ÎªTUNNEL¿Ú²»½¨INTF£¬·µ»Ø´í
	                //&& Èç¹û²»Îª VLAN×Ó½Ó¿Ú²Å½¨INTF£¬²»È»±£´æ
	                if ((pstLogicalPort->basic_cfg_cb != NULL)
	                 && (pstLogicalPort->logic_l3_cfg_cb != NULL)
	                 && (pstLogicalPort->basic_cfg_cb->port_flag == ATG_DCI_BROADCAST)
	                 && (pstLogicalPort->port_id != 0))
	                {

	                    stIntfL3.mCardPort = pstLogicalPort->port_id;
	                    stIntfL3.bindId = pstLogicalPort->logic_l3_cfg_cb->vrf_id;
	                    stIntfL3.eIntfType = INTF_TYPE_L3;

	                    //IPV4 URPFÊ¹ÄÜ
	                    if (pstLogicalPort->logic_l3_cfg_cb->ipv4_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFÈ±Ê¡¼ì²é
	                    {
	                        stIntfL3.eIpv4UrpfMode = URPF_OFF;
	                    }
	                    else if (pstLogicalPort->logic_l3_cfg_cb->ipv4_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPFÑÏ¸ñ¼ì²é
	                    {
	                        stIntfL3.eIpv4UrpfMode = URPF_STRICT;
	                    }
	                    else if (pstLogicalPort->logic_l3_cfg_cb->ipv4_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPFËÉÉ¢¼ì²é
	                    {
	                        stIntfL3.eIpv4UrpfMode = URPF_LOOSE;
	                    }

	                    //IPV6 URPFÊ¹ÄÜ
	                    if (pstLogicalPort->logic_l3_cfg_cb->ipv6_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFÈ±Ê¡¼ì²é
	                    {
	                        stIntfL3.eIpv6UrpfMode = URPF_OFF;
	                    }
	                    else if (pstLogicalPort->logic_l3_cfg_cb->ipv6_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPFÑÏ¸ñ¼ì²é
	                    {
	                        stIntfL3.eIpv6UrpfMode = URPF_STRICT;
	                    }
	                    else if (pstLogicalPort->logic_l3_cfg_cb->ipv6_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPFËÉÉ¢¼ì²é
	                    {
	                        stIntfL3.eIpv6UrpfMode = URPF_LOOSE;
	                    }
	                    
	                    //È±Ê¡Â·ÓÉ¼ì²éÊ¹ÄÜ
	                    if (pstLogicalPort->logic_l3_cfg_cb->ipv4_default_check == ENABLE)
	                    {
	                        stIntfL3.flags |= INTF_V4_ALLOW_DEFAULT_ROUTE;
	                    }

	                    if (pstLogicalPort->logic_l3_cfg_cb->ipv6_default_check == ENABLE)
	                    {
	                        stIntfL3.flags |= INTF_V6_ALLOW_DEFAULT_ROUTE;
	                    }

	                    //MPLS×ª·¢²»Ê¹ÄÜ
	                    if (pstLogicalPort->logic_l3_cfg_cb->mpls_forward_enable == DISABLE)
	                    {
	                        stIntfL3.flags |= INTF_MPLS_DISABLE;
	                    }
	                    
	                    //stIntfL3.flags |= INTF_COUNTER_ON;  //todo

	                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
		                {
		                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
		                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN)))
		                        * i);

		                    /* ÅäÖÃ´¦Àí£¬todo */
		                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
		                    {
		                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
		                    }

		                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

		                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
		                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

		                    //»ùÓÚ¶Ë¿Ú
		                    if ((pstTerminalIf->svlan_id == 0) && (pstTerminalIf->cvlan_id == 0))
		                    {
								if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
			                    {
			                        stIntfL3.mIntfMatchType = INTF_MATCH_L3_VE_PORT;
			                    }
			                    else
			                    {
			                    	stIntfL3.mIntfMatchType = INTF_MATCH_PORT;
			                    }
		                        
		                        stIntfL3.mOvid = 0;
		                        stIntfL3.mIvid = 0;
		                    }

		                    //Dot1qÖÕ½á×Ó½Ó¿ÚÓ¦ÓÃÓÚL3VPN£¬SVLANÓÐÐ§¡¢CVLANÎÞÐ§
		                    else if ((pstTerminalIf->svlan_id != 0) && (pstTerminalIf->cvlan_id == 0))
		                    {
								if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
			                    {
			                        stIntfL3.mIntfMatchType = INTF_MATCH_L3_VE_PORT_OVID;
			                    }
			                    else
			                    {
			                    	stIntfL3.mIntfMatchType = INTF_MATCH_PORT_OVID;
			                    }
		                        
		                        stIntfL3.mOvid = pstTerminalIf->svlan_id;
		                        stIntfL3.mIvid = 0;
		                    }

		                    //QinQÖÕ½á×Ó½Ó¿ÚÓ¦ÓÃÓÚL3VPN£¬SVLANºÍCVLANÍ¬Ê±ÓÐÐ§
		                    else if ((pstTerminalIf->svlan_id != 0) && (pstTerminalIf->cvlan_id != 0))
		                    {
								if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //ÐéÄâÒÔÌ«Íø¿Ú
			                    {
			                        stIntfL3.mIntfMatchType = INTF_MATCH_L3_VE_PORT_OVID_IVID;
			                    }
			                    else
			                    {
			                    	stIntfL3.mIntfMatchType = INTF_MATCH_PORT_OVID_IVID;
			                    }
		                        
		                        stIntfL3.mOvid = pstTerminalIf->svlan_id;
		                        stIntfL3.mIvid = pstTerminalIf->cvlan_id;
		                    }
		                    else
		                    {
		                        OS_PRINTF("***WARNNING***: PORT_INDEX=%ld Ö»ÅäÖÃÁËCVlan£¬Î´ÅäÖÃSVlan\n", ulPortIndexKey);
		                    }


	                        //Èç¹ûÎª×Ó½Ó¿Ú£¬Ö»ÔÚ±¾²ÛÎ»½¨intf£»
	                        //Èç¹ûÎªLAG»òVE¿Ú£¬±ØÐë½¨intf£»
	                        if ((((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_SUB_INTFACE))
	                                && (SHARED.local_slot_id == pstLogicalPort->slot_id))
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG)
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT))
	                        {
	                        	ret = SUCCESS;
#ifdef SPU
	                            ret = ApiC3AddIntf(pstLogicalPort->unit_id, &stIntfL3);
                                   intf_pos_id_temp = stIntfL3.posId;
	                            NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%ld, ApiC3AddIntf() ret = %d",
	                                    ulPortIndexKey, ret));
                                   stIntfL3.posId = 0;
                                   
                                    if((2 == SHARED.c3_num) && 
                                        ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                        || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                                   {
                                        
                                        ret += ApiC3AddIntf(1, &stIntfL3);
                                        NBB_TRC_DETAIL((NBB_FORMAT " L3: 1: PORT_INDEX=%ld, ApiC3AddIntf() ret = %d",
	                                    ulPortIndexKey, ret));
                                   }                                 
#endif
			                    if (ret == SUCCESS)
			                    {

					                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

									//Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó1
					                if (NULL == pstTerminalIfCb)
					                {
					                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
					                	
						                //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
						                pstTerminalIfCb->intf_pos_id = intf_pos_id_temp;
                                                          pstTerminalIfCb->intf2_pos_id = stIntfL3.posId;
						                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
						                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

						                pstLogicalPort->terminal_if_num++;
					                }
									else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
									{
                                                                    pstTerminalIfCb->intf_pos_id = intf_pos_id_temp;
                                                                    pstTerminalIfCb->intf2_pos_id = stIntfL3.posId;
										NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
									}
#ifdef SPU
									almpm_addcfg_intf_onePos(ulPortIndexKey, pstTerminalIfCb->intf_pos_id);
/*
									//Èç¹ûÊÇÎïÀí½Ó¿Ú(Ö÷½Ó¿Ú)£¬ÏÈÅÐ¶ÏÊÇ·ñÅäÖÃÁËIP£¬Ö»ÓÐÅäÖÃÁËIPºó²ÅÊ¹ÄÜ
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
									{
										if ((pstLogicalPort->ipv4_num == 0) && (pstLogicalPort->ipv6_num == 0))
										{
											ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, stIntfL3.posId, DISABLE);
											OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d DISABLE\n",
				                        		ret,
				                        		stIntfL3.posId);
										}
										else
										{
											ret = ApiC3SetIntfStat(pstLogicalPort->unit_id, stIntfL3.posId, ENABLE);
											OS_PRINTF("***INFO***:ApiC3SetIntfStat() ret=%d posId=%d ENABLE\n",
				                        		ret,
				                        		stIntfL3.posId);
										}
									}
*/
#endif					                
			                    }
			                    else
			                    {
			                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 Ôö¼ÓÊ§°Ü",
			                        		ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 Ôö¼ÓÊ§°Ü\n",
			                        		ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 Ôö¼ÓÊ§°Ü\n",
			                            	ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
			                    }
	                        }
	                        else
	                        {
				                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó1
				                if (NULL == pstTerminalIfCb)
				                {
				                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
				                	
					                //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
					                pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
					                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
					                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

					                pstLogicalPort->terminal_if_num++;
				                }
								else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
								{
									pstTerminalIfCb->intf_pos_id = stIntfL3.posId;

									//coverity[bad_sizeof]
									NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
								}
	                        }
		                }
	                }
	                else
	                {
	                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
		                {
	                    	pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld <»ù±¾ÅäÖÃÎª¿Õ><Âß¼­ÅäÖÃL3Îª¿Õ><¶Ë¿Ú±êÊ¶²»ÎªBroadCast><port_id=0>",
	                    		ulPortIndexKey));

	                    OS_PRINTF("***ERROR***:PORT_INDEX=%ld <»ù±¾ÅäÖÃÎª¿Õ><Âß¼­ÅäÖÃL3Îª¿Õ><¶Ë¿Ú±êÊ¶²»ÎªBroadCast><port_id=0>\n",
	                    		ulPortIndexKey);

	                    OS_SPRINTF(ucMessage,
	                        "***ERROR***:PORT_INDEX=%ld <»ù±¾ÅäÖÃÎª¿Õ><Âß¼­ÅäÖÃL3Îª¿Õ><¶Ë¿Ú±êÊ¶²»ÎªBroadCast><port_id=0>\n",
	                    		ulPortIndexKey);
	                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                            "CALL_FUNC_ERROR", ucMessage));
	                }
	            }
	        }
	        else if (ulOperTerminalIf == ATG_DCI_OPER_DEL)
	        {

	            /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
	            pucTerminalIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->vlan_data);

	            /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
	            if (pucTerminalIfDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  pucTerminalIfDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {

	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  21£©VLANÊôÐÔ L3 (num = %d) DEL\n", pstSetLogicalPort->vlan_num);
	                }

	                OS_SPRINTF(ucMessage, "  21£©VLANÊôÐÔ L3 (num = %d) DEL\n", pstSetLogicalPort->vlan_num);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
	                
	                //Èç¹ûÎªBROADCAST¿Ú²Å½¨INTF£¬ÎªTUNNEL¿Ú²»½¨INTF£¬·µ»Ø´í
	                //&& Èç¹û²»Îª VLAN×Ó½Ó¿Ú²Å½¨INTF£¬²»È»±£´æ
	                if ((pstLogicalPort->basic_cfg_cb != NULL)
	                 && (pstLogicalPort->logic_l3_cfg_cb != NULL)
	                 && (pstLogicalPort->basic_cfg_cb->port_flag == ATG_DCI_BROADCAST)
	                 && (pstLogicalPort->port_id != 0))
	                {

	                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
		                {
		                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
		                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

		                    /* ÅäÖÃ´¦Àí£¬todo */
		                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
		                    {
		                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
		                    }

		                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

		                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
		                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));


	                        //Èç¹ûÎª×Ó½Ó¿Ú£¬Ö»ÔÚ±¾²ÛÎ»½¨intf£»
	                        //Èç¹ûÎªLAG»òVE¿Ú£¬±ØÐë½¨intf£»
	                        if ((((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_SUB_INTFACE))
	                                && (SHARED.local_slot_id == pstLogicalPort->slot_id))
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG)
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT))
	                        {

				                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//Èç¹û²»´æÔÚ£¬ÎÞ·¨É¾³ý
				                if (NULL == pstTerminalIfCb)
				                {
			                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 ²»´æÔÚ£¬ÎÞ·¨É¾³ý",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
				                }
                                            else	//´æÔÚÉ¾³ý
                                            {
                                                ret = SUCCESS;
#ifdef SPU
                                                ret = ApiC3DelIntf(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id);

                                                NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%ld, ApiC3DelIntf() ret = %d",
                                                ulPortIndexKey, ret));

                                                if((2 == SHARED.c3_num) && 
                                                    ((stPortInfo.port_type == ATG_DCI_LAG) 
                                                    || (stPortInfo.port_type == ATG_DCI_VE_PORT)))
                                                {
                                                    ret += ApiC3DelIntf(1, pstTerminalIfCb->intf2_pos_id);
                                                    NBB_TRC_DETAIL((NBB_FORMAT 
                                                    " L3 1: PORT_INDEX=%ld, ApiC3DelIntf() ret = %d",
                                                    ulPortIndexKey, ret));
                                                }                                      
#endif
									if (ret == SUCCESS)
									{
#ifdef SPU
										almpm_delline_intf(ulPortIndexKey, pstTerminalIfCb->intf_pos_id, 1);
#endif
							            AVLL_DELETE(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

							            //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
							            spm_free_terminal_if_cb(pstTerminalIfCb NBB_CCXT);
							            
										pstLogicalPort->terminal_if_num--;
									}
									else
									{
				                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

				                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3DelIntf() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 Ôö¼ÓÊ§°Ü",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id));

				                        OS_PRINTF("***ERROR***:ApiC3DelIntf() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 Ôö¼ÓÊ§°Ü\n",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id);

				                        OS_SPRINTF(ucMessage,
				                            	"***ERROR***:ApiC3DelIntf() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄ VLANÊôÐÔ L3 Ôö¼ÓÊ§°Ü\n",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id);
				                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

				                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
				                                "CALL_FUNC_ERROR", ucMessage));
				                    }
								}			
	                        }    
	                        else
	                        {
	                        	if (pstTerminalIfCb != NULL)
	                        	{
						            AVLL_DELETE(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

						            //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
						            spm_free_terminal_if_cb(pstTerminalIfCb NBB_CCXT);
						            
									pstLogicalPort->terminal_if_num--;
	                        	}
	                        }
		                }
	                }
	            }
	        }
	        else if (ulOperTerminalIf == ATG_DCI_OPER_UPDATE)
	        {
	            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
	        }
	        
            /****************************** Diff-ServÅäÖÃ ******************************/
            if (ulOperDiffServ == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucDiffServDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->diff_serv_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucDiffServDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDiffServDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                
                    OS_SPRINTF(ucMessage, "  12£©Diff-ServÅäÖÃ ADD\n");
	             BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                 
                    for (i = 0; i < pstSetLogicalPort->diff_serv_num; i++)
                    {
                        pstDiffServData = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *)
                            (pucDiffServDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  12£©Diff-ServÅäÖÃ ADD\n");
	                        spm_dbg_print_logical_port_diff_serv_cfg(pstDiffServData);
	                    }

	                    //OS_SPRINTF(ucMessage, "  12£©Diff-ServÅäÖÃ ADD\n");
	                    //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
	                    //spm_dbg_record_logical_port_diff_serv_cfg(pstDiffServData);

	                    NBB_TRC_DETAIL((NBB_FORMAT "  Diff-ServID   = %d", pstDiffServData->diff_serv_id));

	                    /* ÅäÖÃ´¦Àí£¬todo */
	                    stQosIntfKey.index = ulPortIndexKey;
	                    stQosIntfKey.svlan = pstDiffServData->svlan;
	                    stQosIntfKey.cvlan = pstDiffServData->cvlan;

	                    //Èç¹ûÂß¼­½Ó¿Ú´æÔÚ
	                    if (ucIfExist == ATG_DCI_EXIST)
	                    {
	                        OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

	                        spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);
	                        stSubPort.unit = stPortInfo.unit_id;
	                        stSubPort.port = stPortInfo.port_id;
	                        stSubPort.slot = stPortInfo.slot_id;
	                        stSubPort.ivlan = stPortInfo.cvlan;
	                        stSubPort.ovlan = stPortInfo.svlan;

							ulVlanPosId = 0;
		                    spm_get_poscnt_from_intf(ulPortIndexKey, 
		                    	pstDiffServData->svlan, 
		                    	pstDiffServData->cvlan, 
		                    	&ulVlanPosId NBB_CCXT);
		                    	
		                    stSubPort.posid = ulVlanPosId;

                            /*
                            if(0 == ulVlanPosId)
                            {
                                pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                            NBB_TRC_DETAIL((NBB_FORMAT
	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()Ôö¼Ó SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan));

	                            OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()Ôö¼Ó SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);

	                            OS_SPRINTF(ucMessage,
	                                "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()Ôö¼Ó SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);
	                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                    "CALL_FUNC_ERROR", ucMessage));
                            }*/

                            //else
                            //{
                               ret = spm_ds_add_logic_intf_node(&stSubPort,
	                            &stQosIntfKey,
	                            pstDiffServData
	                            NBB_CCXT);

    	                        if (ret != SUCCESS)
    	                        {
    	                            pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    	                            NBB_TRC_DETAIL((NBB_FORMAT
    	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan));

    	                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü\n", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);

    	                            OS_SPRINTF(ucMessage,
    	                                "***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü\n", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);
    	                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

    	                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
    	                                    "CALL_FUNC_ERROR", ucMessage));
    	                        } 
                                else
                                {
                                    //pstdiffservcb = AVLL_FIND(pstLogicalPort->diff_serv_tree, pstDiffServData);
                                    //diffservcfgkey.svlan_id = pstDiffServData->svlan;
                                    //diffservcfgkey.cvlan_id = pstDiffServData->cvlan;
                                    //pstdiffservcb = 
                                        //AVLL_FIND(pstLogicalPort->diff_serv_tree, &diffservcfgkey);
                                    pstdiffservcb = 
                                        AVLL_FIND(pstLogicalPort->diff_serv_tree, pstDiffServData);  
                                    
                                    //Èç¹û²»´æÔÚ£¬ÉêÇëÊ÷½Úµã£¬²åÈëÊ÷ÖÐ£¬¼ÆÊý¼Ó1
                                    if (NULL == pstdiffservcb)
                                    {
                                        pstdiffservcb = spm_alloc_diff_serv_cb(NBB_CXT);

                                        //±£´æÊý¾Ý²¢²åÈëÊ÷ÖÐ
                                        //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
                                        //NBB_MEMCPY(&pstdiffservcb->key,&diffservcfgkey,
                                            //sizeof(ATG_DCI_LOG_PORT_VLAN));                                        
                                        NBB_MEMCPY(&(pstdiffservcb->diff_serv_cfg),
                                            pstDiffServData, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
                                        AVLL_INSERT(pstLogicalPort->diff_serv_tree, pstdiffservcb->spm_diff_serv_node);

                                        pstLogicalPort->diff_serv_num++;
                                    }
                                    else	//Ó¦¸Ã²»»á³öÏÖ´ËÇé¿ö£¬ÒòÎªÏÂ·¢ÏÂÀ´µÄ£¬±ØÐëÊÇÅäÖÃ²»Í¬µÄ
                                    {
                                        //pstdiffservcb->intf_pos_id = stIntfFlow.posId;
                                        NBB_MEMCPY(&(pstdiffservcb->diff_serv_cfg), 
                                            pstDiffServData, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
                                    }
                                }      
								
                            //}

	                        
#if 0
	                        if (ret == SUCCESS)
	                        {
	                            /* ±£´æÊý¾Ý */
	                            if (pstLogicalPort->diff_serv_cfg_cb == NULL)
	                            {
	                                pstLogicalPort->diff_serv_cfg_cb = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *)NBB_MM_ALLOC(
	                                    sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA),
	                                    NBB_NORETRY_ACT,
	                                    MEM_SPM_LOG_PORT_DIFF_SERV_CB);
	                            }

	                            OS_MEMCPY(pstLogicalPort->diff_serv_cfg_cb, pstDiffServData,
	                                sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
	                        }
	                        else
	                        {
	                            pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                            NBB_TRC_DETAIL((NBB_FORMAT
	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() error", ulPortIndexKey));

	                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d\n",
	                                ulPortIndexKey,
	                                ret);

	                            OS_SPRINTF(ucMessage,
	                                "***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d\n",
	                                ulPortIndexKey,
	                                ret);
	                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                    "CALL_FUNC_ERROR", ucMessage));
	                        }
#endif
	                    }
	                    else
	                    {
	                        pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢Diff-ServÊ§°Ü",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢Diff-ServÊ§°Ü\n", ulPortIndexKey);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬½¨Á¢Diff-ServÊ§°Ü\n",
	                            ulPortIndexKey);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
	                                BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
	                    }
                    }
                }
            }
            else if (ulOperDiffServ == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucDiffServDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->diff_serv_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucDiffServDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDiffServDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  12£©Diff-ServÅäÖÃ DEL\n");
	             BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                 
                    for (i = 0; i < pstSetLogicalPort->diff_serv_num; i++)
                    {
                        pstDiffServData = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *)
                            (pucDiffServDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  12£©Diff-ServÅäÖÃ DEL\n");
	                        spm_dbg_print_logical_port_diff_serv_cfg(pstDiffServData);
	                    }

	                    //OS_SPRINTF(ucMessage, "  12£©Diff-ServÅäÖÃ DEL\n");
	                    //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
	                    //spm_dbg_record_logical_port_diff_serv_cfg(pstDiffServData);

	                    NBB_TRC_DETAIL((NBB_FORMAT "  Diff-ServID   = %d", pstDiffServData->diff_serv_id));

	                    /* ÅäÖÃ´¦Àí£¬todo */
	                    stQosIntfKey.index = ulPortIndexKey;
	                    stQosIntfKey.svlan = pstDiffServData->svlan;
	                    stQosIntfKey.cvlan = pstDiffServData->cvlan;

	                    //Èç¹ûÂß¼­½Ó¿Ú´æÔÚ
	                    if (ucIfExist == ATG_DCI_EXIST)
	                    {
	                        OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

	                        spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);
	                        stSubPort.unit = stPortInfo.unit_id;
	                        stSubPort.port = stPortInfo.port_id;
	                        stSubPort.slot = stPortInfo.slot_id;
	                        stSubPort.ivlan = stPortInfo.cvlan;
	                        stSubPort.ovlan = stPortInfo.svlan;

							ulVlanPosId = 0;
		                    spm_get_poscnt_from_intf(ulPortIndexKey, 
		                    	pstDiffServData->svlan, 
		                    	pstDiffServData->cvlan, 
		                    	&ulVlanPosId NBB_CCXT);
		                    	
		                    stSubPort.posid = ulVlanPosId;
                            if(0 == ulVlanPosId)
                            {
                                pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                            NBB_TRC_DETAIL((NBB_FORMAT
	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()É¾³ý SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan));

	                            OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()É¾³ý SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);

	                            OS_SPRINTF(ucMessage,
	                                "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()É¾³ý SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);
	                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                    "CALL_FUNC_ERROR", ucMessage));
                            }

                            else
                            {
                                pstDiffServData->diff_serv_id = 0;
		                    
    	                        ret = spm_ds_add_logic_intf_node(&stSubPort,
    	                            &stQosIntfKey,
    	                            pstDiffServData
    	                            NBB_CCXT);

    	                        if (ret != SUCCESS)
    	                        {
    	                            pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
    	                            NBB_TRC_DETAIL((NBB_FORMAT
    	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan));

    	                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü\n", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);

    	                            OS_SPRINTF(ucMessage,
    	                                "***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄDiff-ServÅäÖÃÊ§°Ü\n", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);
    	                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

    	                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
    	                                    "CALL_FUNC_ERROR", ucMessage));
    	                        }
                                 else
                                    {
                                        //pstdiffservcb = AVLL_FIND(pstLogicalPort->diff_serv_tree, pstDiffServData);
                                        //diffservcfgkey.svlan_id = pstDiffServData->svlan;
                                        //diffservcfgkey.cvlan_id = pstDiffServData->cvlan;
                                        //pstdiffservcb = 
                                            //AVLL_FIND(pstLogicalPort->diff_serv_tree, &diffservcfgkey);
                                        pstdiffservcb = 
                                            AVLL_FIND(pstLogicalPort->diff_serv_tree, pstDiffServData);  
                                    
                                        //Èç¹û²»´æÔÚ£¬ÎÞ·¨É¾³ý
                                        if (NULL == pstdiffservcb)
                                        {
                                            pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                            NBB_TRC_DETAIL((NBB_FORMAT 
                                            "  ***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄDiff-ServÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý",
                                            ulPortIndexKey,
                                            pstDiffServData->svlan,
                                            pstDiffServData->cvlan));

                                            OS_PRINTF(
                                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄDiff-ServÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
                                            ulPortIndexKey,
                                            pstDiffServData->svlan,
                                            pstDiffServData->cvlan);

                                            OS_SPRINTF(ucMessage,
                                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d µÄDiff-ServÅäÖÃ²»´æÔÚ£¬ÎÞ·¨É¾³ý\n",
                                            ulPortIndexKey,
                                            pstDiffServData->svlan,
                                            pstDiffServData->cvlan);
                                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", 
                                            ulPortIndexKey, ret,
                                            "CALL_FUNC_ERROR", ucMessage));
                                        }
                                        else
                                        {
                                            AVLL_DELETE(pstLogicalPort->diff_serv_tree, 
                                                pstdiffservcb->spm_diff_serv_node);

                                            //ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
                                            spm_free_diff_serv_cb(pstdiffservcb NBB_CCXT);

                                            pstLogicalPort->diff_serv_num--;									
                                        }
                                    }                               
                            }

		                    
#if 0
	                        if (ret == SUCCESS)
	                        {
	                            /* ±£´æÊý¾Ý */
	                            if (pstLogicalPort->diff_serv_cfg_cb == NULL)
	                            {
	                                pstLogicalPort->diff_serv_cfg_cb = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *)NBB_MM_ALLOC(
	                                    sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA),
	                                    NBB_NORETRY_ACT,
	                                    MEM_SPM_LOG_PORT_DIFF_SERV_CB);
	                            }

	                            OS_MEMCPY(pstLogicalPort->diff_serv_cfg_cb, pstDiffServData,
	                                sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
	                        }
	                        else
	                        {
	                            pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                            NBB_TRC_DETAIL((NBB_FORMAT
	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() error", ulPortIndexKey));

	                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d\n",
	                                ulPortIndexKey,
	                                ret);

	                            OS_SPRINTF(ucMessage,
	                                "***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d\n",
	                                ulPortIndexKey,
	                                ret);
	                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                    "CALL_FUNC_ERROR", ucMessage));
	                        }
#endif
	                    }
	                    else
	                    {
	                        pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýDiff-ServÊ§°Ü",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýDiff-ServÊ§°Ü\n", ulPortIndexKey);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld µÄL3Âß¼­ÅäÖÃÎ´ÏÂ£¬É¾³ýDiff-ServÊ§°Ü\n",
	                            ulPortIndexKey);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
	                                BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
	                    }
                    }
                }
            }
            else if (ulOperDiffServ == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL3 ******************************/
            if (ulOperInclassifyQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucInclassifyQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->inclassify_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucInclassifyQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucInclassifyQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL3 ADD\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->inclassify_qos_num; i++)
	                {
                        pstInclassifyQosData = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)
                            (pucInclassifyQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL3 ADD\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÄ£°åË÷Òý = %d",
	                            pstInclassifyQosData->qos_policy_index));

	                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

	                    stSubPort.unit = stPortInfo.unit_id;
	                    stSubPort.port = stPortInfo.port_id;
	                    stSubPort.slot = stPortInfo.slot_id;
	                    stSubPort.ivlan = stPortInfo.cvlan;
	                    stSubPort.ovlan = stPortInfo.svlan;

						ulVlanPosId = 0;
	                    spm_get_poscnt_from_intf(ulPortIndexKey, 
	                    	pstInclassifyQosData->svlan, 
	                    	pstInclassifyQosData->cvlan, 
	                    	&ulVlanPosId NBB_CCXT);
	                    	
	                    stSubPort.posid = ulVlanPosId;

	                    ret = spm_add_logic_flow_classify_node(ulPortIndexKey,
	                        pstInclassifyQosData,
	                        &stSubPort
	                        NBB_CCXT);

						if (ret != SUCCESS)
						{
	                        pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü\n",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü\n",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#if 0
	                    if (ret == SUCCESS)
	                    {
	                        /* ±£´æÊý¾Ý */
	                        if (pstLogicalPort->inclassify_qos_cfg_cb == NULL)
	                        {
	                            pstLogicalPort->inclassify_qos_cfg_cb = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)NBB_MM_ALLOC(
	                                sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS),
	                                NBB_NORETRY_ACT,
	                                MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
	                        }

	                        OS_MEMCPY(pstLogicalPort->inclassify_qos_cfg_cb, pstInclassifyQosData,
	                            sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
	                    }
	                    else
	                    {
	                        pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() error",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d\n",
	                            ulPortIndexKey,
	                            ret);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d\n",
	                            ulPortIndexKey,
	                            ret);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#endif
	                }
                }
            }
            else if (ulOperInclassifyQos == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucInclassifyQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->inclassify_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucInclassifyQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucInclassifyQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL3 DEL\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->inclassify_qos_num; i++)
	                {
                        pstInclassifyQosData = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)
                            (pucInclassifyQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL3 DEL\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÄ£°åË÷Òý = %d",
	                            pstInclassifyQosData->qos_policy_index));

	                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

	                    stSubPort.unit = stPortInfo.unit_id;
	                    stSubPort.port = stPortInfo.port_id;
	                    stSubPort.slot = stPortInfo.slot_id;
	                    stSubPort.ivlan = stPortInfo.cvlan;
	                    stSubPort.ovlan = stPortInfo.svlan;

						ulVlanPosId = 0;
	                    spm_get_poscnt_from_intf(ulPortIndexKey, 
	                    	pstInclassifyQosData->svlan, 
	                    	pstInclassifyQosData->cvlan, 
	                    	&ulVlanPosId NBB_CCXT);
	                    	
	                    stSubPort.posid = ulVlanPosId;

	                    pstInclassifyQosData->qos_policy_index = 0;
	                    
	                    ret = spm_add_logic_flow_classify_node(ulPortIndexKey,
	                        pstInclassifyQosData,
	                        &stSubPort
	                        NBB_CCXT);

						if (ret != SUCCESS)
						{
	                        pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü\n",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃÊ§°Ü\n",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#if 0
	                    if (ret == SUCCESS)
	                    {
	                        /* ±£´æÊý¾Ý */
	                        if (pstLogicalPort->inclassify_qos_cfg_cb == NULL)
	                        {
	                            pstLogicalPort->inclassify_qos_cfg_cb = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)NBB_MM_ALLOC(
	                                sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS),
	                                NBB_NORETRY_ACT,
	                                MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
	                        }

	                        OS_MEMCPY(pstLogicalPort->inclassify_qos_cfg_cb, pstInclassifyQosData,
	                            sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
	                    }
	                    else
	                    {
	                        pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() error",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d\n",
	                            ulPortIndexKey,
	                            ret);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d\n",
	                            ulPortIndexKey,
	                            ret);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#endif
	                }
                }
            }
            else if (ulOperInclassifyQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL3 ******************************/
            if (ulOperUpUserQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucUpUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->up_user_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucUpUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucUpUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  15£©ÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL3 ADD\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                    for (i = 0; i < pstSetLogicalPort->up_user_qos_num; i++)
	                {
                        pstUpUserQosData = (ATG_DCI_LOG_UP_USER_QOS_POLICY *)
                            (pucUpUserQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  15£©ÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL3 ADD\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ÓÃ»§QOS²ßÂÔÄ£°åË÷Òý = %d", pstUpUserQosData->qos_policy_index));

	                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

	                    stSubPort.unit = stPortInfo.unit_id;
	                    stSubPort.port = stPortInfo.port_id;
	                    stSubPort.slot = stPortInfo.slot_id;
	                    stSubPort.ivlan = stPortInfo.cvlan;
	                    stSubPort.ovlan = stPortInfo.svlan;

						ulVlanPosId = 0;
	                    spm_get_poscnt_from_intf(ulPortIndexKey, 
	                    	pstUpUserQosData->svlan, 
	                    	pstUpUserQosData->cvlan, 
	                    	&ulVlanPosId NBB_CCXT);
	                    	
	                    stSubPort.posid = ulVlanPosId;
		                    
	                    ret = spm_add_logic_up_usr_qos_node(ulPortIndexKey,
	                        &stSubPort,
	                        pstUpUserQosData
	                        NBB_CCXT);

	                    if (ret != SUCCESS)
	                    {
	                        pstSetLogicalPort->up_user_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü\n", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü\n", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#if 0
	                    if (ret == SUCCESS)
	                    {
	                        /* ±£´æÊý¾Ý */
	                        if (pstLogicalPort->up_user_qos_cfg_cb == NULL)
	                        {
	                            pstLogicalPort->up_user_qos_cfg_cb =
	                                (ATG_DCI_LOG_UP_USER_QOS_POLICY *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY),
	                                NBB_NORETRY_ACT,
	                                MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
	                        }

	                        OS_MEMCPY(pstLogicalPort->up_user_qos_cfg_cb, pstUpUserQosData,
	                            sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
	                    }
	                    else
	                    {
	                        pstSetLogicalPort->up_user_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() error", ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d\n",
	                            ulPortIndexKey,
	                            ret);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d\n",
	                            ulPortIndexKey,
	                            ret);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#endif
	                }

                }
            }
            else if (ulOperUpUserQos == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucUpUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->up_user_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucUpUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucUpUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  15£©ÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL3 DEL\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                    for (i = 0; i < pstSetLogicalPort->up_user_qos_num; i++)
	                {
                        pstUpUserQosData = (ATG_DCI_LOG_UP_USER_QOS_POLICY *)
                            (pucUpUserQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  15£©ÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL3 DEL\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ÓÃ»§QOS²ßÂÔÄ£°åË÷Òý = %d", pstUpUserQosData->qos_policy_index));

	                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

	                    stSubPort.unit = stPortInfo.unit_id;
	                    stSubPort.port = stPortInfo.port_id;
	                    stSubPort.slot = stPortInfo.slot_id;
	                    stSubPort.ivlan = stPortInfo.cvlan;
	                    stSubPort.ovlan = stPortInfo.svlan;

						ulVlanPosId = 0;
	                    spm_get_poscnt_from_intf(ulPortIndexKey, 
	                    	pstUpUserQosData->svlan, 
	                    	pstUpUserQosData->cvlan, 
	                    	&ulVlanPosId NBB_CCXT);
	                    	
	                    stSubPort.posid = ulVlanPosId;

	                    pstUpUserQosData->qos_policy_index = 0;
	                    
	                    ret = spm_add_logic_up_usr_qos_node(ulPortIndexKey,
	                        &stSubPort,
	                        pstUpUserQosData
	                        NBB_CCXT);

	                    if (ret != SUCCESS)
	                    {
	                        pstSetLogicalPort->up_user_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü\n", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d É¾³ý SVlan=%d CVlan=%d µÄÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃÊ§°Ü\n", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#if 0
	                    if (ret == SUCCESS)
	                    {
	                        /* ±£´æÊý¾Ý */
	                        if (pstLogicalPort->up_user_qos_cfg_cb == NULL)
	                        {
	                            pstLogicalPort->up_user_qos_cfg_cb =
	                                (ATG_DCI_LOG_UP_USER_QOS_POLICY *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY),
	                                NBB_NORETRY_ACT,
	                                MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
	                        }

	                        OS_MEMCPY(pstLogicalPort->up_user_qos_cfg_cb, pstUpUserQosData,
	                            sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
	                    }
	                    else
	                    {
	                        pstSetLogicalPort->up_user_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                        NBB_TRC_DETAIL((NBB_FORMAT
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() error", ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d\n",
	                            ulPortIndexKey,
	                            ret);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d\n",
	                            ulPortIndexKey,
	                            ret);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#endif
	                }

                }
            }
            else if (ulOperUpUserQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ÉÏ»°ÓÃ»§×éQOS²ßÂÔÅäÖÃ(ÎÞÐ§) ******************************/
            if (ulOperUpGroupQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucUpGroupQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->upuser_group_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucUpGroupQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucUpGroupQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstUpGroupQosData = (ATG_DCI_LOG_UP_USER_GROUP_QOS *)pucUpGroupQosDataStart;

                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  16£©ÉÏ»°ÓÃ»§×éQOS²ßÂÔÅäÖÃ(ÎÞÐ§)\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    //NBB_TRC_DETAIL((NBB_FORMAT "ÓÃ»§QOS²ßÂÔÄ£°åË÷Òý = %d", pstUpUserQosData->qos_policy_index));

                    /* ±£´æÊý¾Ý */
                    if (pstLogicalPort->up_user_group_qos_cfg_cb == NULL)
                    {
                        pstLogicalPort->up_user_group_qos_cfg_cb = (ATG_DCI_LOG_UP_USER_GROUP_QOS *)NBB_MM_ALLOC(
                            sizeof(ATG_DCI_LOG_UP_USER_GROUP_QOS),
                            NBB_NORETRY_ACT,
                            MEM_SPM_LOG_PORT_UP_GROUP_QOS_CB);
                    }

                    OS_MEMCPY(pstLogicalPort->up_user_group_qos_cfg_cb, pstUpGroupQosData,
                        sizeof(ATG_DCI_LOG_UP_USER_GROUP_QOS));
                }
            }
            else if (ulOperUpGroupQos == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperUpGroupQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ÏÂ»°ÓÃ»§×éQOSÅäÖÃL3 ******************************/
            if (ulOperDownGroupQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucDownGroupQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->down_user_group_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucDownGroupQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDownGroupQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDownGroupQosData = (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)pucDownGroupQosDataStart;
                    OS_SPRINTF(ucMessage, "  18£©ÏÂ»°ÓÃ»§×éQOSÅäÖÃL3\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  18£©ÏÂ»°ÓÃ»§×éQOSÅäÖÃL3\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  ÓÃ»§×éË÷Òý  = %d", pstDownGroupQosData->user_group_index));

                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                    ret = spm_hqos_add_group_node(stPortInfo.slot_id,
                        stPortInfo.port_id,
                        pstDownGroupQosData->user_group_index,
                        ulPortIndexKey
                        NBB_CCXT);

                    if (ret == SUCCESS)
                    {
                        /* ±£´æÊý¾Ý */
                        if (pstLogicalPort->down_user_group_qos_cfg_cb == NULL)
                        {
                            pstLogicalPort->down_user_group_qos_cfg_cb =
                                (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)NBB_MM_ALLOC(sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);
                        }

                        OS_MEMCPY(pstLogicalPort->down_user_group_qos_cfg_cb, pstDownGroupQosData,
                            sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS));
                    }
                    else
                    {
                        pstSetLogicalPort->down_user_queue_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() error",
                                ulPortIndexKey));

                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() ret=%d\n", ulPortIndexKey, ret);

                        OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() ret=%d\n",
                            ulPortIndexKey,
                            ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                "CALL_FUNC_ERROR", ucMessage));
                    }
                }
            }
            else if (ulOperDownGroupQos == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperDownGroupQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÅäÖÃL3 ******************************/
            if (ulOperDownUserQos == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucDownUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->down_user_queue_qos_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucDownUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDownUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDownUserQosData = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)pucDownUserQosDataStart;
                    OS_SPRINTF(ucMessage, "  17£©ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÅäÖÃL3\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  17£©ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÅäÖÃL3\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  NodeIndex  = %d", pstDownUserQosData->node_index));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ÓÃ»§QOS²ßÂÔÄ£°åË÷Òý  = %d",
                            pstDownUserQosData->user_qos_policy_index));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ÓÅÏÈ¼¶¶ÓÁÐQOS²ßÂÔÄ£°åË÷Òý  = %d",
                            pstDownUserQosData->prio_queue_qos_policy_index));

                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                    stSubPort.unit = stPortInfo.unit_id;
                    stSubPort.port = stPortInfo.port_id;
                    stSubPort.slot = stPortInfo.slot_id;
                    stSubPort.ivlan = stPortInfo.cvlan;
                    stSubPort.ovlan = stPortInfo.svlan;
                    stSubPort.posid = 0;

                    if (pstLogicalPort->down_user_group_qos_cfg_cb != NULL)
                    {
                        ret = spm_hqos_add_usr_node(&stSubPort,
                            ulPortIndexKey,
                            pstLogicalPort->down_user_group_qos_cfg_cb->user_group_index,
                            pstDownUserQosData
                            NBB_CCXT);
                            
                        if (ret == SUCCESS)
                        {
                            /* ±£´æÊý¾Ý */
                            if (pstLogicalPort->down_user_qos_cfg_cb == NULL)
                            {
                                pstLogicalPort->down_user_qos_cfg_cb = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)NBB_MM_ALLOC(
                                    sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS),
                                    NBB_NORETRY_ACT,
                                    MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);
                            }

                            OS_MEMCPY(pstLogicalPort->down_user_qos_cfg_cb, pstDownUserQosData,
                                sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));
                        }
                        else
                        {
                            pstSetLogicalPort->down_user_queue_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() error",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() ret=%d\n",
                                ulPortIndexKey,
                                ret);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld spm_hqos_add_group_node() ret=%d\n",
                                ulPortIndexKey,
                                ret);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                        }
                    }
                    else
                    {
                        pstSetLogicalPort->down_user_queue_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        NBB_TRC_DETAIL((NBB_FORMAT
                                "  ***ERROR***:PORT_INDEX=%ld µÄÏÂ»°ÓÃ»§×éQOSÅäÖÃÎ´ÏÂ£¬½¨Á¢ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÊ§°Ü",
                                ulPortIndexKey));

                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄÏÂ»°ÓÃ»§×éQOSÅäÖÃÎ´ÏÂ£¬½¨Á¢ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÊ§°Ü\n",
                            ulPortIndexKey);

                        OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld µÄÏÂ»°ÓÃ»§×éQOSÅäÖÃÎ´ÏÂ£¬½¨Á¢ÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÊ§°Ü\n",
                            ulPortIndexKey);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey,
                                BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
                    }
                }
            }
            else if (ulOperDownUserQos == ATG_DCI_OPER_DEL)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
            }
            else if (ulOperDownUserQos == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** °ü¹ýÂËÆ÷traffic_filterL3 ******************************/
            if (ulOperTrafficFilter == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucTrafficFilterDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->traffic_filter_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucTrafficFilterDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucTrafficFilterDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  20£©°ü¹ýÂËÆ÷traffic_filter L3 ADD\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }
                    
                    OS_SPRINTF(ucMessage, "  20£©°ü¹ýÂËÆ÷traffic_filter L3 ADD\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->traffic_filter_num; i++)
                    {
                        pstTrafficFilter = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)
                            (pucTrafficFilterDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER)))
                            * i);

                        NBB_TRC_DETAIL((NBB_FORMAT "  acl_id  = %d", pstTrafficFilter->acl_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ·½Ïòdirection  = %d", pstTrafficFilter->direction));

	                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

	                    stSubPort.unit = stPortInfo.unit_id;
	                    stSubPort.port = stPortInfo.port_id;
	                    stSubPort.slot = stPortInfo.slot_id;
	                    stSubPort.ivlan = stPortInfo.cvlan;
	                    stSubPort.ovlan = stPortInfo.svlan;
                    
						ulVlanPosId = 0;
	                    spm_get_poscnt_from_intf(ulPortIndexKey, 
	                    	pstTrafficFilter->svlan, 
	                    	pstTrafficFilter->cvlan, 
	                    	&ulVlanPosId NBB_CCXT);
	                    	
	                    stSubPort.posid = ulVlanPosId;
	                    
                        ret = spm_add_logic_acl_node(ulPortIndexKey,
                            pstTrafficFilter,
                            &stSubPort
                            NBB_CCXT);

                        if (ret != SUCCESS)
                        {
                            pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filterÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filterÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filterÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                        }
#if 0
                        if (ret == SUCCESS)
                        {
                            iCfgPos = spm_check_if_acl_exist(ulPortIndexKey, pstTrafficFilter);

                            if (iCfgPos <= 0)
                            {
                                for (j = 0; j < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; j++)
                                {
                                    /* ±£´æÊý¾Ý */
                                    if (pstLogicalPort->traffic_filter_cfg_cb[j] == NULL)
                                    {
                                        pstLogicalPort->traffic_filter_cfg_cb[j] =
                                            (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)NBB_MM_ALLOC(sizeof(
                                                ATG_DCI_LOG_PORT_TRAFFIC_FILTER),
                                            NBB_NORETRY_ACT,
                                            MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);

	                                    OS_MEMCPY(pstLogicalPort->traffic_filter_cfg_cb[j], pstTrafficFilter,
	                                        sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));

	                                    pstLogicalPort->traffic_filter_num++;

	                                    break;
                                    }
                                }
                            }
                            else
                            {
                                OS_MEMCPY(pstLogicalPort->traffic_filter_cfg_cb[iCfgPos - 1],
                                    pstTrafficFilter,
                                    sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                            }
                        }
                        else
                        {
                            pstSetLogicalPort->traffic_filter_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü\n",
                                ulPortIndexKey);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                        }

#endif
                    }
                }
            }
            else if (ulOperTrafficFilter == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                pucTrafficFilterDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->traffic_filter_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (pucTrafficFilterDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucTrafficFilterDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  20£©°ü¹ýÂËÆ÷traffic_filter L3 DEL\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }
                    
                    OS_SPRINTF(ucMessage, "  20£©°ü¹ýÂËÆ÷traffic_filter L3 DEL\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->traffic_filter_num; i++)
                    {
                        pstTrafficFilter = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)
                            (pucTrafficFilterDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER)))
                            * i);

                        NBB_TRC_DETAIL((NBB_FORMAT "  acl_id  = %d", pstTrafficFilter->acl_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ·½Ïòdirection  = %d", pstTrafficFilter->direction));

	                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

	                    stSubPort.unit = stPortInfo.unit_id;
	                    stSubPort.port = stPortInfo.port_id;
	                    stSubPort.slot = stPortInfo.slot_id;
	                    stSubPort.ivlan = stPortInfo.cvlan;
	                    stSubPort.ovlan = stPortInfo.svlan;
                    
						ulVlanPosId = 0;
	                    spm_get_poscnt_from_intf(ulPortIndexKey, 
	                    	pstTrafficFilter->svlan, 
	                    	pstTrafficFilter->cvlan, 
	                    	&ulVlanPosId NBB_CCXT);
	                    	
	                    stSubPort.posid = ulVlanPosId;

	                    pstTrafficFilter->acl_id = 0;
	                    
                        ret = spm_add_logic_acl_node(ulPortIndexKey,
                            pstTrafficFilter,
                            &stSubPort
                            NBB_CCXT);

						if (ret != SUCCESS)
						{
                            pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filterÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filterÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d Ôö¼Ó SVlan=%d CVlan=%d µÄ°ü¹ýÂËÆ÷traffic_filterÅäÖÃÊ§°Ü!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                        }
#if 0
                        if (ret == SUCCESS)
                        {
                            iCfgPos = spm_check_if_acl_exist(ulPortIndexKey, pstTrafficFilter);

                            if (iCfgPos <= 0)
                            {
                                for (j = 0; j < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; j++)
                                {
                                    /* ±£´æÊý¾Ý */
                                    if (pstLogicalPort->traffic_filter_cfg_cb[j] == NULL)
                                    {
                                        pstLogicalPort->traffic_filter_cfg_cb[j] =
                                            (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)NBB_MM_ALLOC(sizeof(
                                                ATG_DCI_LOG_PORT_TRAFFIC_FILTER),
                                            NBB_NORETRY_ACT,
                                            MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);

	                                    OS_MEMCPY(pstLogicalPort->traffic_filter_cfg_cb[j], pstTrafficFilter,
	                                        sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));

	                                    pstLogicalPort->traffic_filter_num++;

	                                    break;
                                    }
                                }
                            }
                            else
                            {
                                OS_MEMCPY(pstLogicalPort->traffic_filter_cfg_cb[iCfgPos - 1],
                                    pstTrafficFilter,
                                    sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                            }
                        }
                        else
                        {
                            pstSetLogicalPort->traffic_filter_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld µÄ°ü¹ýÂËÆ÷traffic_filterÔö¼ÓÊ§°Ü\n",
                                ulPortIndexKey);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));
                        }

#endif
                    }
                }
            }
            else if (ulOperTrafficFilter == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }

            /****************************** VRRPÐéÄâMACºÍIPv6µØÖ·ÅäÖÃL3 ******************************/
            if (ul_oper_vipv6_vmac == ATG_DCI_OPER_ADD)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                puc_vipv6_vmac_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->vrrp_ipv6_mac_l3_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (puc_vipv6_vmac_data_start == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  puc_vipv6_vmac_data_start is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  25) ÐéÄâMACºÍIPv6µØÖ·ÅäÖÃL3 (vrrp_ipv6_mac_l3_num = %d) ADD\n",
                            pstSetLogicalPort->vrrp_ipv6_mac_l3_num);
                    }

                    OS_SPRINTF(ucMessage,
                        "  25) ÐéÄâMACºÍIPv6µØÖ·ÅäÖÃL3 (vrrp_ipv6_mac_l3_num = %d) ADD\n",
                        pstSetLogicalPort->vrrp_ipv6_mac_l3_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    //ÕÒµ½vlanÊôÐÔµÚÒ»¸öÅäÖÃ
                    pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);

                    //Ã»ÓÐvlanÊôÐÔ£¬ÎÞ·¨°ó¶¨£¬´òÓ¡´íÎó£¬Ö±½ÓÍË³ö
                    if(NULL == pstTerminalIfCb)
                    {
                        //´Ë´ÎÏÂ·¢µÄËùÓÐVRRPÅäÖÃ¾ù·µ»ØÊ§°Ü
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld µÄ Âß¼­¶Ë¿ÚVLANÊôÐÔ²»´æÔÚ£¬ÎÞ·¨ÉèÖÃVRRP ipv6!\n",
			                        		ulPortIndexKey));

                        OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld µÄ Âß¼­¶Ë¿ÚVLANÊôÐÔ²»´æÔÚ£¬ÎÞ·¨ÉèÖÃVRRP ipv6!\n",
			                        		ulPortIndexKey);

                        OS_SPRINTF(ucMessage,	"  ***ERROR***:PORT_INDEX=%ld µÄ Âß¼­¶Ë¿ÚVLANÊôÐÔ²»´æÔÚ£¬ÎÞ·¨ÉèÖÃVRRP ipv6!\n",
			                            	ulPortIndexKey);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, BASIC_CONFIG_NOT_EXIST,
			                                "BASIC_CONFIG_NOT_EXIST", ucMessage));		
                    }
                    else
                    {
                        //¼ÇÂ¼Ê×¸öVLANÊôÐÔÅäÖÃµÄµØÖ·
                        tempterminalifcb_v6 = pstTerminalIfCb;
						
                        //³õÊ¼»¯VrrpµÄ¹«¹²³ÉÔ±
                        stvrrp_v6.l3_port_id = pstLogicalPort->port_id;/*lint !e613 */
			   stvrrp_v6.slot = pstLogicalPort->slot_id;/*lint !e613 */

                        //VE¿ÚÖ»ÐèÖÃVE±êÖ¾£¬·ÇVE¿ÚÐèÒªÌîvlanÐÅÏ¢
                        if(ATG_DCI_VE_PORT == pstLogicalPort->basic_cfg_cb->port_sub_type)/*lint !e613 */
                        {
                            stvrrp_v6.ve_flag = 1;
                        }

                        //¸ÃÂß¼­½Ó¿ÚÏÂµÄËùÓÐINTF¶¼ÒªÊ¹ÄÜVRRP
#ifdef SPU                        
                        while (pstTerminalIfCb)
                        {

                            ret = fhdrv_psn_l3_set_intf_vrrp(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, 1);

                            if((2 == SHARED.c3_num) && 
                                ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                           {                              
                                ret += fhdrv_psn_l3_set_intf_vrrp(1, pstTerminalIfCb->intf2_pos_id, 1);
                           }

                            if (SUCCESS != ret)
                            {
                                   for (i = 0; i < pstSetLogicalPort->vrrp_ipv6_mac_l3_num; i++)
                                   {
                                   	pstSetLogicalPort->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                   }                          	

                            	NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:vrrp fhdrv_psn_l3_set_intf_vrrp()"
                                    " ret=%d PORT_INDEX=%ld µÄintf_pos_id=%dÊ¹ÄÜÊ§°Ü\n",
	                        		ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id));

                            	OS_PRINTF("  ***ERROR***:vrrp fhdrv_psn_l3_set_intf_vrrp() ret=%d "
                                    "PORT_INDEX=%ld µÄintf_pos_id=%dÊ¹ÄÜÊ§°Ü\n",
	                        		ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id);

                            	OS_SPRINTF(ucMessage,
    	                            "  ***ERROR***:vrrp fhdrv_psn_l3_set_intf_vrrp() ret=%d "
    	                            "PORT_INDEX=%ld µÄintf_pos_id=%dÊ¹ÄÜÊ§°Ü\n",
	                            	ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id);
                            	BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            	NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", 
                                    ulPortIndexKey, CALL_C3_FUNC_ERROR,
	                                "CALL_C3_FUNC_ERROR", ucMessage));

                                   break;
                            }
							
                            pstTerminalIfCb = AVLL_NEXT(pstLogicalPort->terminal_if_tree, 
                                pstTerminalIfCb->spm_terminal_if_node);
                        }
#endif

                        for (i = 0; i < pstSetLogicalPort->vrrp_ipv6_mac_l3_num; i++)
                        {
                            pst_vipv6_vmac_data[i] = (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)
                                (puc_vipv6_vmac_data_start + 
                                (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))) * i);

                            /* ÅäÖÃ´¦Àí£¬todo */
                            if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                            {
                                spm_dbg_print_logical_port_vipv6_vmac_cfg(pst_vipv6_vmac_data[i]);
                            }

                            //spm_dbg_record_logical_port_vip_vmac_cfg(pst_vipv6_vmac_data[i]);

                            NBB_TRC_DETAIL((NBB_FORMAT "  MACµØÖ· = %0x-%0x-%0x-%0x-%0x-%0x", 
                                pst_vipv6_vmac_data[i]->mac_address[0],
                                pst_vipv6_vmac_data[i]->mac_address[1],
                                pst_vipv6_vmac_data[i]->mac_address[2],
                                pst_vipv6_vmac_data[i]->mac_address[3],
                                pst_vipv6_vmac_data[i]->mac_address[4],
                                pst_vipv6_vmac_data[i]->mac_address[5]));
                            
                            //NBB_TRC_DETAIL((NBB_FORMAT "  IPv4µØÖ· = %s", 
                                //spm_set_ulong_to_ipv4(pst_vipv6_vmac_data[i]->ip_addr NBB_CCXT)));
                            NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", 
                                pst_vipv6_vmac_data[i]->sub_net_mask_len));

                            v6cfgpos = spm_check_if_vipv6_vmac_exist(ulPortIndexKey, 
                                pst_vipv6_vmac_data[i] NBB_CCXT);

                            //¸ù¾Ý±¾vrrpÊôÐÔË¢ÐÂÇý¶¯µÄvrrpÅäÖÃ
                            stvrrp_v6.dmac[0] = pst_vipv6_vmac_data[i]->mac_address[0];
                            stvrrp_v6.dmac[1] = pst_vipv6_vmac_data[i]->mac_address[1];
                            stvrrp_v6.dmac[2] = pst_vipv6_vmac_data[i]->mac_address[2];
                            stvrrp_v6.dmac[3] = pst_vipv6_vmac_data[i]->mac_address[3];
                            stvrrp_v6.dmac[4] = pst_vipv6_vmac_data[i]->mac_address[4];
                            stvrrp_v6.dmac[5] = pst_vipv6_vmac_data[i]->mac_address[5];


			       //Ê×¸öVLANÊôÐÔÅäÖÃµÄµØÖ·
	                     pstTerminalIfCb =  tempterminalifcb_v6 ;
#ifdef SRC				   
			       terminalifnum_v6 = pstLogicalPort->terminal_if_num;
#endif	

#ifdef SPU	      
                            terminalifnum_v6 = 0;	

                        //¸ÃÂß¼­½Ó¿ÚÏÂµÄËùÓÐINTF¶¼ÒªÊ¹ÄÜVRRP
                        while (pstTerminalIfCb)
                        {

				stvrrp_v6.ovlan_id = pstTerminalIfCb->terminal_if_cfg.svlan_id;
				stvrrp_v6.ivlan_id = pstTerminalIfCb->terminal_if_cfg.cvlan_id;

				//Íâ²ãVLAN
				if(0  !=  stvrrp_v6.ovlan_id)/*lint !e613 */
				{
					stvrrp_v6.ovlan_id_mask = 0xfff;
				}
				else
				{
					stvrrp_v6.ovlan_id_mask = 0;
				}

				//ÄÚÍâ²ãVLAN
				if(0  !=  stvrrp_v6.ivlan_id)/*lint !e613 */
				{
					stvrrp_v6.ivlan_id_mask = 0xfff;
				}
				else
				{
				 	stvrrp_v6.ivlan_id_mask = 0;
				}						

		              ret = SUCCESS;
			       ret = fhdrv_psn_acl_set_vrrp(pstLogicalPort->unit_id, &stvrrp_v6);

                            vrrp6_pos_id_temp = stvrrp_v6.posid;
                            stvrrp_v6.posid = 0;
                   
                            if((2 == SHARED.c3_num) && 
                                ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                            {
                                
                                ret += fhdrv_psn_acl_set_vrrp(1, &stvrrp_v6);
                            }   
                            
				if(SUCCESS != ret)
                            {
                            	pstSetLogicalPort->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                            	NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:vrrp fhdrv_psn_acl_set_vrrp()"
                                            " ret=%d PORT_INDEX=%ld°ó¶¨VRRPÊ§°Ü\n",
    		                        		ret, ulPortIndexKey));

                            	OS_PRINTF("  ***ERROR***:vrrp fhdrv_psn_acl_set_vrrp() ret=%d PORT_INDEX=%ld°ó¶¨VRRPÊ§°Ü\n",
    		                        		ret, ulPortIndexKey);

                            	OS_SPRINTF(ucMessage,
    		                            "  ***ERROR***:vrrp fhdrv_psn_acl_set_vrrp() ret=%d PORT_INDEX=%ld °ó¶¨VRRPÊ§°Ü\n",
    		                            	ret, ulPortIndexKey);
                            	BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            	NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_C3_FUNC_ERROR,
    		                                "CALL_C3_FUNC_ERROR", ucMessage));

                                break;
                            }
				
				terminalifnum_v6++;
							
                            pstTerminalIfCb = AVLL_NEXT(pstLogicalPort->terminal_if_tree, 
                                pstTerminalIfCb->spm_terminal_if_node);
                        }
 #endif                           


                            if(terminalifnum_v6 == pstLogicalPort->terminal_if_num)
                            {
                                if (v6cfgpos <= 0)
                                {
                                    for (j = 0; j < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; j++)
                                    {
                                        //Èç¹ûÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÎªÎÞÊý¾Ý£¬¿ÉÒÔ´æ´¢Êý¾Ý
                                        if (pstLogicalPort->vipv6_vmac_cfg_cb[j] == NULL)/*lint !e613 */
                                        {
                                            pstLogicalPort->vipv6_vmac_cfg_cb[j] =
                                                (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)NBB_MM_ALLOC(sizeof(
                                                    ATG_DCI_LOG_VRRP_IPV6_MAC_L3),
                                                NBB_NORETRY_ACT,
                                                MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);/*lint !e613 */

                                            if(pstLogicalPort->vipv6_vmac_cfg_cb[j] == NULL)/*lint !e613 */
                                            {
                                                OS_PRINTF("	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                            	
                                    			OS_SPRINTF(ucMessage, "	***ERROR***:(%s:%d)malloc failed!\n", __FILE__,__LINE__);
                                    			BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                                                goto EXIT_LABEL;
                                    			
                                            }
                                            OS_MEMCPY(pstLogicalPort->vipv6_vmac_cfg_cb[j], pst_vipv6_vmac_data[i],
                                                sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3));/*lint !e613 */
                                            pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid[j] =    /*lint !e613 */
                                                vrrp6_pos_id_temp;
                                            pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid2[j] =    /*lint !e613 */
                                                stvrrp_v6.posid;                                                
                                            pstLogicalPort->vipv6_vmac_num++;/*lint !e613 */
        
                                            break;
                                        }
                                    }
                                }
                                else
                                {
                                    OS_MEMCPY(pstLogicalPort->vipv6_vmac_cfg_cb[v6cfgpos - 1], pst_vipv6_vmac_data[i],
                                        sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3));/*lint !e613 */
                                }
                            }
                        }
                    }


                }
            }
            else if (ul_oper_vipv6_vmac == ATG_DCI_OPER_DEL)
            {

                /* ¼ÆËãµÚÒ»¸öentryµÄµØÖ·¡£*/
                puc_vipv6_vmac_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->vrrp_ipv6_mac_l3_data);

                /* Ê×µØÖ·ÎªNULL£¬Òì³£ */
                if (puc_vipv6_vmac_data_start == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  puc_vipv6_vmac_data_start is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  25) ÐéÄâMACºÍIPv6µØÖ·ÅäÖÃL3 (vrrp_ipv6_mac_l3_num = %d) DEL\n",
                            pstSetLogicalPort->vrrp_ipv6_mac_l3_num);
                    }

                    OS_SPRINTF(ucMessage,
                        "  25) ÐéÄâMACºÍIPv6µØÖ·ÅäÖÃL3 (vrrp_ipv6_mac_l3_num = %d) DEL\n",
                        pstSetLogicalPort->vrrp_ipv6_mac_l3_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->vrrp_ipv6_mac_l3_num; i++)
                    {
                        pst_vipv6_vmac_data[i] = (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)
                            (puc_vipv6_vmac_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))) * i);

                        /* ÅäÖÃ´¦Àí£¬todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_vipv6_vmac_cfg(pst_vipv6_vmac_data[i]);
                        }

                        //spm_dbg_record_logical_port_vip_vmac_cfg(pst_vipv6_vmac_data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  MACµØÖ· = %0x-%0x-%0x-%0x-%0x-%0x", 
                            pst_vipv6_vmac_data[i]->mac_address[0],
                                pst_vipv6_vmac_data[i]->mac_address[1],
                                pst_vipv6_vmac_data[i]->mac_address[2],
                                pst_vipv6_vmac_data[i]->mac_address[3],
                                pst_vipv6_vmac_data[i]->mac_address[4],
                                pst_vipv6_vmac_data[i]->mac_address[5]));
                        
                        //NBB_TRC_DETAIL((NBB_FORMAT "  IPv4µØÖ· = %s", 
                            //spm_set_ulong_to_ipv4(pst_vipv6_vmac_data[i]->ip_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ×ÓÍøÑÚÂë³¤¶È = %d", 
                            pst_vipv6_vmac_data[i]->sub_net_mask_len));

                        v6cfgpos = spm_check_if_vipv6_vmac_exist(ulPortIndexKey, pst_vipv6_vmac_data[i] NBB_CCXT);
                        if(v6cfgpos < 0)//²»´æÔÚ£¬ÎÞ·¨É¾³ý
                        {
                            pstSetLogicalPort->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                            OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­¶Ë¿Ú²»´æÔÚVRRP v6-MACµØÖ· "
                                            "=%2x-%2x-%2x-%2x-%2x-%2x,ÎÞ·¨É¾³ý! \n",
			                        		ulPortIndexKey, pst_vipv6_vmac_data[i]->mac_address[0],
			                        		pst_vipv6_vmac_data[i]->mac_address[1],
			                        		pst_vipv6_vmac_data[i]->mac_address[2],
			                        		pst_vipv6_vmac_data[i]->mac_address[3],
			                        		pst_vipv6_vmac_data[i]->mac_address[4],
			                        		pst_vipv6_vmac_data[i]->mac_address[5]);

                            OS_SPRINTF(ucMessage,
			                            "  ***ERROR***:PORT_INDEX=%ld µÄÂß¼­¶Ë¿Ú²»´æÔÚVRRP v6-MACµØÖ· "
    			                            "=%2x-%2x-%2x-%2x-%2x-%2x,ÎÞ·¨É¾³ý! \n",
			                            	ulPortIndexKey, pst_vipv6_vmac_data[i]->mac_address[0],
			                        		pst_vipv6_vmac_data[i]->mac_address[1],
			                        		pst_vipv6_vmac_data[i]->mac_address[2],
			                        		pst_vipv6_vmac_data[i]->mac_address[3],
			                        		pst_vipv6_vmac_data[i]->mac_address[4],
			                        		pst_vipv6_vmac_data[i]->mac_address[5]);
                            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        }
                        else
                        {
#ifdef SPU                        
                            stvrrp_v6.posid = 
                            pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid[v6cfgpos - 1];/*lint !e613 */
                            ret = SUCCESS;

                            ret = fhdrv_psn_acl_del_vrrp(pstLogicalPort->unit_id, &stvrrp_v6);

                            if((2 == SHARED.c3_num) && 
                                ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                           {
                                stvrrp_v6.posid = 
                                    pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid2[v6cfgpos - 1];
                                if(0 != stvrrp_v6.posid)
                                {
                                    ret += fhdrv_psn_acl_del_vrrp(1, &stvrrp_v6);
                                }                                
                           } 

                            if(SUCCESS != ret)
                            {
                                pstSetLogicalPort->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                            	NBB_TRC_DETAIL((NBB_FORMAT 
                                    "  ***ERROR***:vrrp fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld"
                                            " µÄvrrp_pos_id=%dÉ¾³ýÊ§°Ü\n",
			                        		ret, ulPortIndexKey, 
			                        		pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid[v6cfgpos - 1]));

                            	OS_PRINTF(
						"  ***ERROR***:vrrp fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld µÄvrrp_pos_id=%dÉ¾³ýÊ§°Ü\n",
			                        		ret, ulPortIndexKey, 
			                        		pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid[v6cfgpos - 1]);

                            	OS_SPRINTF(ucMessage,
			                            "  ***ERROR***:vrrp fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld µÄvrrp_pos_id=%dÉ¾³ýÊ§°Ü\n",
			                            	ret, ulPortIndexKey, 
			                            	pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid[v6cfgpos - 1]);
                            	BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                            	NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, CALL_C3_FUNC_ERROR,
			                                "CALL_C3_FUNC_ERROR", ucMessage));
                                continue;
                            }
#endif

                            for (j = 0; j < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; j++)
                            {
                                //Èç¹û²»ÎªNULL£¬ËµÃ÷Õâ¸öÎ»ÖÃÓÐÊý¾Ý£¬¿ÉÒÔÉ¾³ýÊý¾Ý
                                if ((pstLogicalPort->vipv6_vmac_cfg_cb[j] != NULL)
                                    && (!(spm_ipv6_mac_key_compare(pst_vipv6_vmac_data[i], 
                                    pstLogicalPort->vipv6_vmac_cfg_cb[j] NBB_CCXT))))/*lint !e613 */
                                {
                                    NBB_MM_FREE(pstLogicalPort->vipv6_vmac_cfg_cb[j], /*lint !e613 */
                                        MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
                                    pstLogicalPort->vipv6_vmac_cfg_cb[j] = NULL;/*lint !e613 */
    
                                    pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid[j] = 0;/*lint !e613 */
                                    
                                    if((2 == SHARED.c3_num) && 
                                        ((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
                                        || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)))
                                   {
                                        pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid2[j] = 0;/*lint !e613 */
                                   } 
                                    
                                    pstLogicalPort->vipv6_vmac_num--;/*lint !e613 */
    
                                    break;
                                }
                            }
                        }

                    }
                }
            }
            else if (ul_oper_vipv6_vmac == ATG_DCI_OPER_UPDATE)
            {
                NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
            }
            
        }

        //¶Ë¿ÚÊôÐÔ¼È²»ÊÇL2Ò²²»ÊÇL3
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***WARNNING***:¶Ë¿Ú(%ld)ÊôÐÔ¼È²»ÊÇL2Ò²²»ÊÇL3(»ò»ù±¾ÅäÖÃ²»´æÔÚ)", ulPortIndexKey));
            OS_PRINTF("***WARNNING***:¶Ë¿Ú(%ld)ÊôÐÔ¼È²»ÊÇL2Ò²²»ÊÇL3(»ò»ù±¾ÅäÖÃ²»´æÔÚ)\n", ulPortIndexKey);

            OS_SPRINTF(ucMessage,
                "***WARNNING***:¶Ë¿Ú(%ld)ÊôÐÔ¼È²»ÊÇL2Ò²²»ÊÇL3(»ò»ù±¾ÅäÖÃ²»´æÔÚ)\n", 
                ulPortIndexKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, BASIC_CONFIG_NOT_EXIST,
                    "BASIC_CONFIG_NOT_EXIST", ucMessage));
                                    
            pstSetLogicalPort->log_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetLogicalPort->log_l2_return_code = ATG_DCI_RC_UNSUCCESSFUL;

            for (i = 0; i < ATG_DCI_LOG_PORT_FLOW_NUM; i++)
            {
                pstSetLogicalPort->flow_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            }

            for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
            {
                pstSetLogicalPort->ipv4_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
            }

            for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
            {
                pstSetLogicalPort->ipv6_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
            }

            for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
            {
                pstSetLogicalPort->mc_ipv4_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
            }

            pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;

            //for (i = 0; i < ATG_DCI_LOG_FLOW_DIFF_SERV_NUM; i++)
            //{
            //    pstSetLogicalPort->flow_diff_serv_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
            //}
            pstSetLogicalPort->flow_diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;

            pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetLogicalPort->up_user_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetLogicalPort->upuser_group_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetLogicalPort->down_user_queue_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetLogicalPort->down_user_group_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetLogicalPort->log_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            pstSetLogicalPort->flow_upuser_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            

            //for (i = 0; i < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; i++)
            //{
            //    pstSetLogicalPort->traffic_filter_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
            //}
            pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;

            for (i = 0; i < ATG_DCI_LOG_VLAN_SINGLE_NUM; i++)
            {
                pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
            }

			for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
            {
                pstSetLogicalPort->mc_ipv6_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
            }

        }

        #if 1

        /******************************** ¹ØÁª½Ó¿Ú×é *********************************/

        /*Ó¦ÓÃ³¡¾°:ÏÂ¹ØÁª½Ó¿Ú×éÅäÖÃÊ±£¬Âß¼­½Ó¿ÚÎ´ÏÂ·¢£¬µ±Ê±Ö»±£´æ£¬ÔÚÏÂÂß¼­½Ó¿ÚÅäÖÃÊ±·´Ë¢*/
         
            for (pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB*) AVLL_FIRST(SHARED.associate_if_tree);
                  pstAssociateIfCb != NULL;
                  pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB*) AVLL_NEXT(SHARED.associate_if_tree,
                  pstAssociateIfCb->spm_associate_if_node))
           {
                for(i = 0;i < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM;i++)
                {
                   if((pstAssociateIfCb->trigger_if_cfg_cb[i] != NULL)&&(pstAssociateIfCb->trigger_if_cfg_cb[i]->interface_index == ulPortIndexKey) 
                      && (pstAssociateIfCb->trigger_if_exist[i] == UNEXIST))
                   {
                        trigger_if_flag = 1;
                        ucExistPos_trigger = i;
                        ulAssociateIfKey = pstAssociateIfCb->key;
                        break;
                   }
                    
                }
             
                if(trigger_if_flag != 1)
                {
                   for(i = 0;i < ATG_DCI_ASSOCIATE_IF_ACT_NUM;i++)
                    {
                       if((pstAssociateIfCb->act_if_cfg_cb[i] != NULL)&& (pstAssociateIfCb->act_if_cfg_cb[i]->interface_index == ulPortIndexKey) 
                         && (pstAssociateIfCb->act_if_exist[i] == UNEXIST))
                       {
                            act_if_flag = 1;
                            ucExistPos_act = i;
                            ulAssociateIfKey = pstAssociateIfCb->key;
                            break;
                       }
                        
                    } 
                }

                if((trigger_if_flag == 1) || (act_if_flag == 1))
                {
                    break;
                }
                                
                             }
            /*¸ÃÂß¼­¿ÚµÄindexÒÑ¾­¼ÓÈëµ½¹ØÁª½Ó¿Ú×éÖÐ£¬Ö®Ç°Ö»±£´æ£¬ÏÖÔÚÕýÊ½¼ÓÈë*/

             if(trigger_if_flag == 1)
             {
                ret = spm_get_portid_from_logical_port_index(ulPortIndexKey,&stPortInfo NBB_CCXT);
                
                if(ret != SUCCESS)
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: Âß¼­¶Ë¿Úindex=%ld»ñÈ¡ÎïÀí¿ÚÊ§°Ü£¬ ret=%d",ulPortIndexKey,ret));
    		
					OS_PRINTF("***ERROR***:Âß¼­¶Ë¿Úindex=%ld»ñÈ¡ÎïÀí¿ÚÊ§°Ü£¬ ret=%d",ulPortIndexKey,ret);

					OS_SPRINTF(ucMessage, "***ERROR***:Âß¼­¶Ë¿Úindex=%ld»ñÈ¡ÎïÀí¿ÚÊ§°Ü£¬ ret=%d",ulPortIndexKey,ret);
					BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

					NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ulAssociateIfKey, ret, "spm_get_portid_from_logical_port_index", ucMessage));
    				
                }

                /*lag¿ÚÔÚlagÅäÖÃ¿éÀï¼Ó*/
                
			    if((stPortInfo.port_type == ATG_DCI_SUB_INTFACE)||(stPortInfo.port_type == ATG_DCI_INTFACE))
    			{
    				usPortId = stPortInfo.port_id;
    				usSlotId = stPortInfo.slot_id;
    						
    				OS_MEMSET(&IfGoupInfo,0,sizeof(PORT_GROUP_INFO));

    				IfGoupInfo.usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    				IfGoupInfo.cardId = stPortInfo.card_id;
    				IfGoupInfo.lacpState = 1;
    				IfGoupInfo.actionType = 0;
    						
    				ret = SUCCESS;

    				ret = aps_add_monitor_group_port(ulAssociateIfKey,&IfGoupInfo);

                	if(ret == SUCCESS)
                	{	
                	    if(pstAssociateIfCb != NULL)
                        {
                            pstAssociateIfCb->trigger_if_exist[ucExistPos_trigger] = EXIST;
                            pstAssociateIfCb->trigger_if_num++;  
                        }   
                         
                	}
                    
                    if(ret != SUCCESS)
    				{
    				
               			NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: aps_add_monitor_group_port() ret=%d", ret));

    					OS_PRINTF("***ERROR***:aps_add_monitor_group_port() ret=%d\n", ret);

    					OS_SPRINTF(ucMessage, "***ERROR***:aps_add_monitor_group_port() ret=%d\n", ret);
    					BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    	
    					NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ulAssociateIfKey, ret, "aps_add_monitor_group_port", ucMessage));
    			
    				}
							
            }
        }

        
        else if(act_if_flag == 1)
         {
            ret = spm_get_portid_from_logical_port_index(ulPortIndexKey,&stPortInfo NBB_CCXT);

            /*lag¿ÚÔÚlagÅäÖÃ¿éÀï¼Ó*/
            
            if((stPortInfo.port_type == ATG_DCI_SUB_INTFACE)||(stPortInfo.port_type == ATG_DCI_INTFACE))
            {
                usPortId = stPortInfo.port_id;
                usSlotId = stPortInfo.slot_id;
                        
                OS_MEMSET(&IfGoupInfo,0,sizeof(PORT_GROUP_INFO));

                IfGoupInfo.usSlotPort = (usSlotId << 8)+(usPortId&0xff);
                IfGoupInfo.cardId = stPortInfo.card_id;
                IfGoupInfo.lacpState = 1;
                IfGoupInfo.actionType = 1;
                        
                ret = SUCCESS;

                ret = aps_add_monitor_group_port(ulAssociateIfKey,&IfGoupInfo);

                if(ret == SUCCESS)
                {
                    if(pstAssociateIfCb != NULL)
                    {
                        pstAssociateIfCb->act_if_exist[ucExistPos_act] = EXIST;
                        pstAssociateIfCb->act_if_num++;  
                    }
                     
                            
                }
                if(ret != SUCCESS)
                {
                
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: aps_add_monitor_group_port() ret=%d", ret));

                    OS_PRINTF("***ERROR***:aps_add_monitor_group_port() ret=%d\n", ret);

                    OS_SPRINTF(ucMessage, "***ERROR***:aps_add_monitor_group_port() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    
                    NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ulAssociateIfKey, ret, "aps_add_monitor_group_port", ucMessage));
            
                }
                        
             }
        }
    #endif

    }    
                
        

EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_del_logical_port_cfg
   ¹¦ÄÜÃèÊö  : É¾³ý¶Ë¿ÚÂß¼­ÅäÖÃ
   ÊäÈë²ÎÊý  : SPM_LOGICAL_PORT_CB* pstLogicalPort
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê5ÔÂ14ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_del_logical_port_cfg(SPM_LOGICAL_PORT_CB *pstLogicalPort NBB_CCXT_T NBB_CXT)
{
    NBB_INT ret = SUCCESS;
    NBB_INT i = 0;
    NBB_BYTE ucPort = 0;
    NBB_BYTE ucSlot = 0;
    NBB_BYTE ucSubBoardNo = 0;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_ULONG ulPortIndexKey = 0;
    NBB_USHORT usFlowId = 0;
    SPM_PORT_STRUCT stPort;
    SUB_PORT stSubPort;
    SPM_PORT_INFO_CB stPortInfo;
    SPM_QOS_LOGIC_FLOW_KEY stQosFlowKey;
    SPM_QOS_LOGIC_INTF_KEY stQosIntfKey;
    TX_UNI_PORT stTxUniPort;
    SPM_QOS_LOGIC_UNIVP stUniVp;
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS stDownUserQueueQos;
    ATG_DCI_LOG_UP_USER_QOS_POLICY stUpUserQosPolicy;
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV stFlowDiffServ;
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA stDiffServ;
    ATG_DCI_LOG_PORT_DS_L2_DATA stDsL2;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;

    NBB_TRC_ENTRY("spm_del_logical_port_cfg");

    if (pstLogicalPort == NULL)
    {
    	goto EXIT_LABEL;
    }

    OS_MEMSET(&stPort, 0, sizeof(SPM_PORT_STRUCT));
    OS_MEMSET(&stSubPort, 0, sizeof(SUB_PORT));
    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stTxUniPort, 0, sizeof(TX_UNI_PORT));
    OS_MEMSET(&stQosFlowKey, 0, sizeof(SPM_QOS_LOGIC_FLOW_KEY));
    OS_MEMSET(&stQosIntfKey, 0, sizeof(SPM_QOS_LOGIC_INTF_KEY));
    OS_MEMSET(&stDownUserQueueQos, 0, sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));
    OS_MEMSET(&stUpUserQosPolicy, 0, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY));
    OS_MEMSET(&stFlowDiffServ, 0, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
    OS_MEMSET(&stDiffServ, 0, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
    OS_MEMSET(&stDsL2, 0, sizeof(ATG_DCI_LOG_PORT_DS_L2_DATA));
    OS_MEMSET(&stUniVp, 0, sizeof(SPM_QOS_LOGIC_UNIVP));

    ulPortIndexKey = pstLogicalPort->port_index_key;

    //Èç¹û»ù±¾ÅäÖÃ²»´æÔÚ£¬Ôò²»ÐèÒª´¦Àí
    if ((pstLogicalPort->basic_cfg_cb == NULL) || (pstLogicalPort->phy_cfg_cb == NULL))
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:¶Ë¿ÚÂß¼­ÅäÖÃµÄ»ù±¾ÅäÖÃ²»´æÔÚ"));

        OS_PRINTF("***ERROR***:ÒªÉ¾³ýµÄPORT_INDEX=%ld µÄ¶Ë¿ÚÂß¼­ÅäÖÃµÄ»ù±¾ÅäÖÃ»òÎïÀíÅäÖÃ²¢²»´æÔÚ!\n", ulPortIndexKey);

        OS_SPRINTF(ucMessage,
            "***ERROR***:ÒªÉ¾³ýµÄPORT_INDEX=%ld µÄ¶Ë¿ÚÂß¼­ÅäÖÃµÄ»ù±¾ÅäÖÃ»òÎïÀíÅäÖÃ²¢²»´æÔÚ!\n",
            ulPortIndexKey);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | PHYSICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, BASIC_CONFIG_NOT_EXIST,
                "BASIC_CONFIG_NOT_EXIST", ucMessage));
                
        goto EXIT_LABEL;
    }

    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

    stQosIntfKey.index = ulPortIndexKey;
    stQosFlowKey.index = ulPortIndexKey;
    stTxUniPort.port = pstLogicalPort->port_id;
    stTxUniPort.sysPort = pstLogicalPort->slot_id;

    stSubPort.unit = stPortInfo.unit_id;
    stSubPort.port = stPortInfo.port_id;
    stSubPort.slot = stPortInfo.slot_id;
    stSubPort.ivlan = stPortInfo.cvlan;
    stSubPort.ovlan = stPortInfo.svlan;
#if 0
    //É¾³ýDiff-ServÅäÖÃ
    if ((pstLogicalPort->diff_serv_cfg_cb != NULL) && (pstLogicalPort->logic_l3_cfg_cb != NULL))
    {
        spm_ds_add_logic_intf_node(&stSubPort,
            &stQosIntfKey,
            &stDiffServ);
    }

    //É¾³ýÁ÷Ïà¹ØDiff-ServÅäÖÃL2
    for (i = 0; i < ATG_DCI_LOG_FLOW_DIFF_SERV_NUM; i++)
    {
        if (pstLogicalPort->flow_diff_serv_cfg_cb[i] != NULL)
        {
            //usFlowId = pstLogicalPort->flow_diff_serv_cfg_cb[i]->flow_id;

            //stQosFlowKey.flow_id = usFlowId;

            spm_ds_add_logic_flow_node(&stSubPort,
                0,
                &stQosFlowKey,
                pstLogicalPort->logic_port_info_cb.flow_info_cb.pos_id);
        }
    }


    //É¾³ýÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃ
    if (pstLogicalPort->inclassify_qos_cfg_cb != NULL)
    {
        spm_add_logic_flow_classify_node(ulPortIndexKey, 0, &stSubPort);
    }

    //É¾³ýÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃ
    if (pstLogicalPort->up_user_qos_cfg_cb != NULL)
    {
        spm_add_logic_up_usr_qos_node(ulPortIndexKey,
            &stSubPort,
            &stUpUserQosPolicy);
    }
#endif
    //É¾³ýÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÅäÖÃ ºÍ ÏÂ»°ÓÃ»§×éQOSÅäÖÃ
    if ((pstLogicalPort->down_user_qos_cfg_cb != NULL) && (pstLogicalPort->down_user_group_qos_cfg_cb != NULL))
    {
        stDownUserQueueQos.node_index = pstLogicalPort->down_user_qos_cfg_cb->node_index;
        stDownUserQueueQos.user_qos_policy_index = 0;
        stDownUserQueueQos.prio_queue_qos_policy_index = 0;

        spm_hqos_add_usr_node(&stSubPort,
            ulPortIndexKey,
            pstLogicalPort->down_user_group_qos_cfg_cb->user_group_index,
            &stDownUserQueueQos
            NBB_CCXT);

        spm_hqos_add_group_node(stPortInfo.slot_id,
            stPortInfo.port_id,
            0,
            ulPortIndexKey
            NBB_CCXT); //nhiÔÝÊ±»ñÈ¡²»µ½£¬Ìî0
    }

    //É¾³ýDiff-ServÅäÖÃL2£¨VPÏÂ»°UNI²à£©
    if (pstLogicalPort->ds_l2_cfg_cb != NULL)
    {
		stUniVp.vpn_id = pstLogicalPort->logic_port_info_cb.vpn_id;
		stUniVp.vp_type = pstLogicalPort->logic_port_info_cb.vpn_type;
		stUniVp.vp_posid = pstLogicalPort->logic_port_info_cb.vp_idx;

		stDsL2.ds_id = 0;

		spm_ds_add_logic_univp_node(ulPortIndexKey,	&stUniVp, &stDsL2 NBB_CCXT);
    }

	//°ü¹ýÂËÆ÷traffic_filter
	spm_qos_clear_logic_acl(ulPortIndexKey NBB_CCXT);

	//ÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃ
	spm_qos_clear_logic_classify(ulPortIndexKey NBB_CCXT);

	//Diff-ServÅäÖÃ
	spm_qos_clear_ds_logic_intf(ulPortIndexKey NBB_CCXT);

	//Á÷Ïà¹ØDiff-ServÅäÖÃL2
	spm_qos_clear_logic_ds_flow(ulPortIndexKey NBB_CCXT);

	//É¾³ý´ËÂß¼­½Ó¿ÚÉÏµÄÐÔÄÜ°ó¶¨
    //almpm_delline_intf(ulPortIndexKey, 1);
	
    //Èç¹ûÊÇÈý²ã½Ó¿Ú£¬É¾³ýintf
    if (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L3)
    {
	    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
	         pstTerminalIfCb != NULL;
	         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
	                       pstTerminalIfCb->spm_terminal_if_node))
	    {
	    
#ifdef SPU
			ret = ApiC3DelIntf(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id);
			almpm_delline_intf(ulPortIndexKey, pstTerminalIfCb->intf_pos_id, 1);
#endif

	    }
    }
    else if (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L2)   //Èç¹ûÊÇ¶þ²ã½Ó¿Ú
    {
        //L2ÖÐÃ»ÓÐVP¾Í²»¿ÉÄÜ´æÔÚINTFÁ÷£¬ËùÒÔÒªÉ¾³ýL2µÄINTF£¬±ØÐëÏàÓ¦µÄVP´æÔÚ
        if (pstLogicalPort->logic_port_info_cb.vp_idx != 0)
        {
		    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
		         pstTerminalIfCb != NULL;
		         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
		                       pstTerminalIfCb->spm_terminal_if_node))
		    {
                //flow²»´æÔÚ
                if (pstTerminalIfCb->intf_pos_id== 0)
                {
                    continue;
                }

#ifdef SPU
				ret = ApiC3DelIntf(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id);
				almpm_delline_intf(ulPortIndexKey, pstTerminalIfCb->intf_pos_id, 1);
#endif

                //É¾³ýÊ§°Ü
                if (ret != SUCCESS)
                {
                	//coverity[dead_error_begin]
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3DelIntf() L2 ret=%d PORT_INDEX=%ld É¾³ýINTFÊ§°Ü!\n", ret, ulPortIndexKey));
                    OS_PRINTF("***ERROR***:ApiC3DelIntf() L2 ret=%d PORT_INDEX=%ld É¾³ýINTFÊ§°Ü!\n", ret, ulPortIndexKey);

			        OS_SPRINTF(ucMessage, "***ERROR***:ApiC3DelIntf() L2 ret=%d PORT_INDEX=%ld É¾³ýINTFÊ§°Ü!\n", ret, ulPortIndexKey);
			        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        
                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", pstLogicalPort->port_index_key, ret,
                            "CALL_C3_FUNC_ERROR", "ApiC3DelIntf() L3"));

                    //pstSetLogicalPort->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    //goto EXIT_LABEL;
                }
            }

        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:VP²»´æÔÚ! flow²»ÊôÓÚÈÎºÎVP£¬INTF flowÎÞ·¨É¾³ý!"));

            //OS_PRINTF("***ERROR***:VP²»´æÔÚ! flow²»ÊôÓÚÈÎºÎVP£¬INTF flowÎÞ·¨É¾³ý!\n");

            //goto EXIT_LABEL;
        }

    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_check_flow_vlan_conflict
   ¹¦ÄÜÃèÊö  : ¼ì²éÒ»¸öÂß¼­¶Ë¿ÚÖÐ10ÌõÁ÷µÄvlanÊÇ·ñÓÐÖØ¸´
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_FLOW_DATA *pstFlow
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :  0 £ºÎÞÖØ¸´
             -1 : ÓÐÖØ¸´
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê5ÔÂ7ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_check_flow_vlan_conflict(NBB_ULONG ulPortIndex, ATG_DCI_LOG_PORT_FLOW_DATA *pstFlow NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;

    NBB_TRC_ENTRY("spm_check_flow_vlan_conflict");

    if (pstFlow == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_flow_vlan_conflict(pstFlow==NULL)"));

        OS_PRINTF("***ERROR***:spm_check_flow_vlan_conflict(pstFlow==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_flow_vlan_conflict(pstFlow==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_flow_vlan_conflict() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_flow_vlan_conflict() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);

        OS_SPRINTF(ucMessage,
            "***ERROR***:spm_check_flow_vlan_conflict() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n",
            ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, LOGICAL_PORT_NOT_EXIST,
                "LOGICAL_PORT_NOT_EXIST", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    for (i = 0; i < ATG_DCI_LOG_PORT_FLOW_NUM; i++)
    {
        //if (pstLogicalPort->flow_cfg_cb == NULL)
        //{
        //    continue;
        //}
        //else
        //{
            //Èç¹ûflow_idÏàÍ¬£¬ÄÇÃ´ovlan¿ÉÒÔÏàÍ¬
            //if ((pstLogicalPort->flow_cfg_cb->otci == pstFlow->otci)
            //    && (pstLogicalPort->flow_cfg_cb->flow_id != pstFlow->flow_id))
            //{
            //    ret = ERROR;

            //    goto EXIT_LABEL;
            //}
        //}
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_check_if_ipv4_exist
   ¹¦ÄÜÃèÊö  : ¼ì²éÒ»¸öÂß¼­¶Ë¿ÚÖÐipv4ÊÇ·ñÓÐÖØ¸´
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_IPV4_DATA *pstIpv4
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :  0 £ºÎÞÖØ¸´
             n : ÓÐÖØ¸´£¬n=Êý×éÎ»ÖÃ+1
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê5ÔÂ14ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_check_if_ipv4_exist(NBB_ULONG ulPortIndex, ATG_DCI_LOG_PORT_IPV4_DATA *pstIpv4 NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;

    NBB_TRC_ENTRY("spm_check_if_ipv4_exist");

    if (pstIpv4 == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_ipv4_exist(pstIpv4==NULL)"));

        OS_PRINTF("***ERROR***:spm_check_if_ipv4_exist(pstIpv4==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_ipv4_exist(pstIpv4==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_ipv4_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_ipv4_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_ipv4_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, LOGICAL_PORT_NOT_EXIST,
                "LOGICAL_PORT_NOT_EXIST", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        if (pstLogicalPort->ipv4_cfg_cb[i] == NULL)
        {
            continue;
        }
        else
        {
            //Èç¹ûIPV4ÏàµÈ£¬·µ»Ø´æ´¢Î»ÖÃ
            if ((spm_ipv4_key_compare(pstIpv4, pstLogicalPort->ipv4_cfg_cb[i] NBB_CCXT)) == 0)
            {
                ret = i + 1;
                goto EXIT_LABEL;
            }
        }
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_check_if_ipv6_exist
   ¹¦ÄÜÃèÊö  : ¼ì²éÒ»¸öÂß¼­¶Ë¿ÚÖÐipv6ÊÇ·ñÓÐÖØ¸´
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_IPV6_DATA *pstIpv6
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :  0 £ºÎÞÖØ¸´
             n : ÓÐÖØ¸´£¬n=Êý×éÎ»ÖÃ+1
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê5ÔÂ14ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_check_if_ipv6_exist(NBB_ULONG ulPortIndex, ATG_DCI_LOG_PORT_IPV6_DATA *pstIpv6 NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;

    NBB_TRC_ENTRY("spm_check_if_ipv6_exist");

    if (pstIpv6 == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_ipv6_exist(pstIpv6==NULL)"));

        OS_PRINTF("***ERROR***:spm_check_if_ipv6_exist(pstIpv6==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_ipv6_exist(pstIpv6==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_ipv6_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_ipv6_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_ipv6_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, LOGICAL_PORT_NOT_EXIST,
                "LOGICAL_PORT_NOT_EXIST", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        if (pstLogicalPort->ipv6_cfg_cb[i] == NULL)
        {
            continue;
        }
        else
        {
            //Èç¹ûIPV4ÏàµÈ£¬·µ»Ø´æ´¢Î»ÖÃ
            if ((spm_ipv6_key_compare(pstIpv6, pstLogicalPort->ipv6_cfg_cb[i] NBB_CCXT)) == 0)
            {
                ret = i + 1;
                goto EXIT_LABEL;
            }
        }
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_check_if_mc_mac_exist
   ¹¦ÄÜÃèÊö  : ¼ì²éÒ»¸öÂß¼­¶Ë¿ÚÖÐ×é²¥MACµØÖ·ÊÇ·ñÓÐÖØ¸´
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_MC_MAC_DATA *pstMcMac
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : 0 £ºÎÞÖØ¸´
             n : ÓÐÖØ¸´£¬n=Êý×éÎ»ÖÃ+1
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê5ÔÂ14ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_check_if_mc_ipv4_exist(NBB_ULONG ulPortIndex, ATG_DCI_LOG_PORT_MC_IPV4_DATA *pstMcIpv4 NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;

    NBB_TRC_ENTRY("spm_check_if_mc_ipv4_exist");

    if (pstMcIpv4 == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_mc_ipv4_exist(pstMcIpv4==NULL)"));

        OS_PRINTF("***ERROR***:spm_check_if_mc_ipv4_exist(pstMcIpv4==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_mc_ipv4_exist(pstMcIpv4==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_mc_ipv4_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_mc_ipv4_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_mc_ipv4_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, LOGICAL_PORT_NOT_EXIST,
                "LOGICAL_PORT_NOT_EXIST", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        if (pstLogicalPort->mc_ipv4_cfg_cb[i] == NULL)
        {
            continue;
        }
        else
        {
            //Èç¹ûIPV4ÏàµÈ£¬·µ»Ø´æ´¢Î»ÖÃ
            if ((spm_ipv4_key_compare(pstMcIpv4, pstLogicalPort->mc_ipv4_cfg_cb[i] NBB_CCXT)) == 0)
            {
                ret = i + 1;
                goto EXIT_LABEL;
            }
        }
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_check_if_mc_ipv6_exist
   ¹¦ÄÜÃèÊö  : ¼ì²éÒ»¸öÂß¼­¶Ë¿ÚÖÐIPV6×é²¥µØÖ·ÊÇ·ñÓÐÖØ¸´
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_MC_IPV6_DATA *pstMcIpv6
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : 0 £ºÎÞÖØ¸´
             n : ÓÐÖØ¸´£¬n=Êý×éÎ»ÖÃ+1
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê10ÔÂ31ÈÕ
    ×÷    Õß   : zhangzhm
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/

NBB_INT spm_check_if_mc_ipv6_exist(NBB_ULONG ulPortIndex, ATG_DCI_LOG_PORT_MC_IPV6_DATA *pstMcIpv6 NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;

    NBB_TRC_ENTRY("spm_check_if_mc_ipv6_exist");

    if (pstMcIpv6 == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_mc_ipv6_exist(pstMcIpv6==NULL)"));

        OS_PRINTF("***ERROR***:spm_check_if_mc_ipv6_exist(pstMcIpv6==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_mc_ipv6_exist(pstMcIpv6==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_mc_ipv6_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_mc_ipv6_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_mc_ipv6_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, LOGICAL_PORT_NOT_EXIST,
                "LOGICAL_PORT_NOT_EXIST", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        if (pstLogicalPort->mc_ipv6_cfg_cb[i] == NULL)
        {
            continue;
        }
        else
        {
            //Èç¹ûIPV6ÏàµÈ£¬·µ»Ø´æ´¢Î»ÖÃ
            if ((spm_ipv6_key_compare(pstMcIpv6, pstLogicalPort->mc_ipv6_cfg_cb[i] NBB_CCXT)) == 0)
            {
                ret = i + 1;
                goto EXIT_LABEL;
            }
        }
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
   º¯ Êý Ãû  : spm_check_if_vip_vmac_exist
   ¹¦ÄÜÃèÊö  : ¼ì²éÒ»¸öÂß¼­¶Ë¿ÚÖÐÐéÄâMACºÍIPµØÖ·ÅäÖÃÊÇ·ñÓÐÖØ¸´
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pstVipVmac
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : 0 £ºÎÞÖØ¸´
             n : ÓÐÖØ¸´£¬n=Êý×éÎ»ÖÃ+1
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê5ÔÂ14ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_check_if_vip_vmac_exist(NBB_ULONG ulPortIndex, ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pstVipVmac NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;

    NBB_TRC_ENTRY("spm_check_if_vip_vmac_exist");

    if (pstVipVmac == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_vip_vmac_exist(pstVipVmac==NULL)"));

        OS_PRINTF("***ERROR***:spm_check_if_vip_vmac_exist(pstVipVmac==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_vip_vmac_exist(pstVipVmac==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_vip_vmac_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_vip_vmac_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);

        OS_SPRINTF(ucMessage,
            "***ERROR***:spm_check_if_vip_vmac_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n",
            ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, LOGICAL_PORT_NOT_EXIST,
                "LOGICAL_PORT_NOT_EXIST", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        if (pstLogicalPort->vip_vmac_cfg_cb[i] == NULL)
        {
            continue;
        }
        else
        {
            //Èç¹ûÐéÄâMACºÍIPµØÖ·ÅäÖÃÏàµÈ£¬·µ»Ø´æ´¢Î»ÖÃ
            if ((spm_ip_mac_key_compare(pstVipVmac, pstLogicalPort->vip_vmac_cfg_cb[i] NBB_CCXT)) == 0)
            {
                ret = i + 1;
                goto EXIT_LABEL;
            }
        }
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

NBB_INT spm_check_if_acl_exist(NBB_ULONG ulPortIndex, ATG_DCI_LOG_PORT_TRAFFIC_FILTER *pstTrafficFilter NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;

    NBB_TRC_ENTRY("spm_check_if_vip_vmac_exist");

    if (pstTrafficFilter == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_acl_exist(pstTrafficFilter==NULL)"));

        OS_PRINTF("***ERROR***:spm_check_if_acl_exist(pstTrafficFilter==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_acl_exist(pstTrafficFilter==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, FUNC_IN_PARAM_IS_NULL,
                "FUNC_IN_PARAM_IS_NULL", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_acl_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ", ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_acl_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_acl_exist() PORT_INDEX=%ld Âß¼­¶Ë¿Ú²»´æÔÚ \n", ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndex, LOGICAL_PORT_NOT_EXIST,
                "LOGICAL_PORT_NOT_EXIST", ucMessage));
                
        ret = ERROR;

        goto EXIT_LABEL;
    }
#if 0
    for (i = 0; i < ATG_DCI_LOG_VLAN_SINGLE_NUM; i++)
    {
        if (pstLogicalPort->traffic_filter_cfg_cb[i] == NULL)
        {
            continue;
        }
        else
        {
            //Èç¹ûÐéÄâMACºÍIPµØÖ·ÅäÖÃÏàµÈ£¬·µ»Ø´æ´¢Î»ÖÃ
            if ((spm_acl_key_compare(pstTrafficFilter, pstLogicalPort->traffic_filter_cfg_cb[i])) == 0)
            {
                ret = i + 1;
                goto EXIT_LABEL;
            }
        }
    }
#endif
EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_logical_port_drvinfo_clear
   ¹¦ÄÜÃèÊö  : Çå¿ÕÇý¶¯·µ»ØµÄÖµ
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê5ÔÂ3ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_logical_port_drvinfo_clear(NBB_ULONG ulPortIndex NBB_CCXT_T NBB_CXT)
{
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort != NULL)
    {
        OS_MEMSET(&pstLogicalPort->logic_port_info_cb, 0, sizeof(SPM_LOGICAL_PORT_INFO_CB));

	    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
	         pstTerminalIfCb != NULL;
	         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
	                       pstTerminalIfCb->spm_terminal_if_node))
	    {
	    	pstTerminalIfCb->intf_pos_id = 0;
	    }
    }

    return SUCCESS;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_uni_logical_port_drvinfo_clear
   ¹¦ÄÜÃèÊö  : Çå¿ÕÇý¶¯·µ»ØµÄÖµ
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2014Äê1ÔÂ17ÈÕ
    ×÷    Õß   : zhangzm
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_uni_logical_port_drvinfo_clear(NBB_ULONG ulPortIndex NBB_CCXT_T NBB_CXT)
{
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort != NULL)
    {
        OS_MEMSET(&pstLogicalPort->logic_port_info_cb, 0, sizeof(SPM_LOGICAL_PORT_INFO_CB));
    }

    return SUCCESS;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_uni_logical_port_drvinfo_nhi_clear
   ¹¦ÄÜÃèÊö  : Çå¿ÕÇý¶¯·µ»ØµÄÖµ£¬±£Áônhi
   ÊäÈë²ÎÊý  : NBB_ULONG ulPortIndex
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2014Äê8ÔÂ21ÈÕ
    ×÷    Õß   : zhangsen
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_INT spm_uni_logical_port_drvinfo_nhi_clear(NBB_ULONG ulPortIndex NBB_CCXT_T NBB_CXT)
{
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    NBB_UINT iNhiId = 0;

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    if (pstLogicalPort != NULL)
    {
    	iNhiId = pstLogicalPort->logic_port_info_cb.next_hop_id;
        OS_MEMSET(&pstLogicalPort->logic_port_info_cb, 0, sizeof(SPM_LOGICAL_PORT_INFO_CB));
        pstLogicalPort->logic_port_info_cb.next_hop_id = iNhiId;
    }

    return SUCCESS;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_init_logical_port_tree
   ¹¦ÄÜÃèÊö  : ³õÊ¼»¯´æ´¢¶Ë¿ÚÂß¼­ÅäÖÃµÄÊ÷
   ÊäÈë²ÎÊý  : ÎÞ
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê11ÔÂ24ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_init_logical_port_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_logical_port_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_LOGICAL_PORT_CB, port_index_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.logical_port_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_LOGICAL_PORT_CB, spm_logical_port_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_logical_port_cb
   ¹¦ÄÜÃèÊö  : ¶Ë¿ÚÂß¼­ÅäÖÃ½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_LOGICAL_PORT_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê10ÔÂ12ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_LOGICAL_PORT_CB *spm_alloc_logical_port_cb(NBB_CXT_T NBB_CXT)
{
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    NBB_BUF_SIZE avll_key_offset;
    NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_alloc_physical_port_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    pstLogicalPort = (SPM_LOGICAL_PORT_CB *)NBB_MM_ALLOC(sizeof(SPM_LOGICAL_PORT_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_LOGICAL_PORT_CB);
    if (pstLogicalPort == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstLogicalPort, 0, sizeof(SPM_LOGICAL_PORT_CB));

    /* ³õÊ¼»¯Ö¸Õë */
    pstLogicalPort->basic_cfg_cb = NULL;
    pstLogicalPort->phy_cfg_cb = NULL;
    pstLogicalPort->logic_l3_cfg_cb = NULL;
    pstLogicalPort->logic_l2_cfg_cb = NULL;
    pstLogicalPort->ces_cfg_cb = NULL;
    pstLogicalPort->ve_mac_cfg_cb = NULL;
//    pstLogicalPort->diff_serv_cfg_cb = NULL;
//    pstLogicalPort->inclassify_qos_cfg_cb = NULL;
//    pstLogicalPort->up_user_qos_cfg_cb = NULL;
    pstLogicalPort->up_user_group_qos_cfg_cb = NULL;
    pstLogicalPort->down_user_qos_cfg_cb = NULL;
    pstLogicalPort->down_user_group_qos_cfg_cb = NULL;
    pstLogicalPort->ds_l2_cfg_cb = NULL;

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_FLOW_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
/*    for (i = 0; i < ATG_DCI_LOG_PORT_FLOW_NUM; i++)
    {
        pstLogicalPort->flow_cfg_cb = NULL;
    }
*/
    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_FLOW_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2013/4/26*/
    /***************************************************************************/
    //for (i = 0; i < ATG_DCI_LOG_FLOW_DIFF_SERV_NUM; i++)
    //{
    //    pstLogicalPort->flow_diff_serv_cfg_cb[i] = NULL;
    //}

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_IPV4_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        pstLogicalPort->ipv4_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_IPV6_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        pstLogicalPort->ipv6_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_MC_MAC_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        pstLogicalPort->mc_ipv4_cfg_cb[i] = NULL;
    }

	/***************************************************************************/
    /* ³õÊ¼»¯IPV6×é²¥ÅäÖÃ Added by zhangzhm, 2013/11/1*/
    /***************************************************************************/
	for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        pstLogicalPort->mc_ipv6_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_VIP_VMAC_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pstLogicalPort->vip_vmac_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_VIP_VMAC_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pstLogicalPort->vipv6_vmac_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_VIP_VMAC_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2013/4/26*/
    /***************************************************************************/
    //for (i = 0; i < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; i++)
    //{
    //    pstLogicalPort->traffic_filter_cfg_cb[i] = NULL;
    //}

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_TERMINAL_IF_NODE_CBµÄAVLLÊ÷ Added by xiaoxiang, 2013/9/3*/
    /***************************************************************************/
    //for (i = 0; i < ATG_DCI_LOG_TERMINAL_IF_NUM; i++)
    //{
    //    pstLogicalPort->terminal_if_cfg_cb[i] = NULL;
    //}

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_TERMINAL_IF_CBµÄAVLLÊ÷.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->terminal_if_tree, spm_terminal_if_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TERMINAL_IF_CB, terminal_if_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TERMINAL_IF_CB, spm_terminal_if_node));/*lint !e413 */

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_DIFF_SERV_CBµÄAVLLÊ÷.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->diff_serv_tree, spm_diff_serv_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_DIFF_SERV_CB, diff_serv_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_DIFF_SERV_CB, spm_diff_serv_node));/*lint !e413 */

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_FLOW_DIFF_SERV_CBµÄAVLLÊ÷.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->flow_diff_serv_tree, spm_flow_diff_serv_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_DIFF_SERV_CB, flow_diff_serv_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_DIFF_SERV_CB, spm_flow_diff_serv_node));/*lint !e413 */

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_INCLASSIFY_QOS_CBµÄAVLLÊ÷.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->inclassify_qos_tree, spm_inclassify_qos_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_INCLASSIFY_QOS_CB, inclassify_qos_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_INCLASSIFY_QOS_CB, spm_inclassify_qos_node));/*lint !e413 */

    /***************************************************************************/
    /* ³õÊ¼»¯ÓÃÓÚ´æ´¢SPM_TRAFFIC_FILTER_CBµÄAVLLÊ÷.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->traffic_filter_tree, spm_traffic_filter_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TRAFFIC_FILTER_CB, traffic_filter_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TRAFFIC_FILTER_CB, spm_traffic_filter_node));/*lint !e413 */    
    
    /* ½¨Á¢ÓÃÓÚ¸Ã½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    pstLogicalPort->spm_logical_port_handle = NBB_CREATE_HANDLE(pstLogicalPort,
        HDL_SPM_LOGICAL_PORT_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_LOGICAL_PORT_CB allocated at %p with handle %#lx",
            pstLogicalPort, pstLogicalPort->spm_logical_port_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstLogicalPort->spm_logical_port_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstLogicalPort);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_free_logical_port_cb
   ¹¦ÄÜÃèÊö  : ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ½ÚµãµÄÄÚ´æ¿Õ¼ä
   ÊäÈë²ÎÊý  : SPM_LOGICAL_PORT_CB *pstLogicalPort
             NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê11ÔÂ8ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_free_logical_port_cb(SPM_LOGICAL_PORT_CB *pstLogicalPort NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT i = 0;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;
    SPM_DIFF_SERV_CB *pstdiffservcb = NULL;   
    SPM_FLOW_DIFF_SERV_CB *pstflowdiffservcb = NULL; 
    SPM_INCLASSIFY_QOS_CB *pstinclassifyqoscb = NULL; 
    SPM_TRAFFIC_FILTER_CB *psttrafficfiltercb = NULL;     
    AVLL_NODE node = {0};

    NBB_TRC_ENTRY("spm_free_logical_port_cb");
    
    /***************************************************************************/
    /* ¼ì²é¿ØÖÆ¿éµÄÕýÈ·ÐÔ¡£                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstLogicalPort, sizeof(SPM_LOGICAL_PORT_CB), MEM_SPM_LOGICAL_PORT_CB);

    if (pstLogicalPort == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* É¾³ýµ¥ÅÌÐÅÏ¢¿ØÖÆ¿é¡£                                                    */
    /***************************************************************************/

    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_LOGICAL_PORT_CB at %p", pstLogicalPort));
    NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* ÊÍ·Å»ù±¾ÅäÖÃÊý¾Ý¿é¡£                                                    */
    /***************************************************************************/
    if (pstLogicalPort->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->basic_cfg_cb, MEM_SPM_LOG_PORT_BASIC_CB);
        pstLogicalPort->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅÎïÀíÅäÖÃÊý¾Ý¿é¡£                                                    */
    /***************************************************************************/
    if (pstLogicalPort->phy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->phy_cfg_cb, MEM_SPM_LOG_PORT_PHY_CB);
        pstLogicalPort->phy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅÂß¼­ÅäÖÃL3Êý¾Ý¿é¡£                                                  */
    /***************************************************************************/
    if (pstLogicalPort->logic_l3_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->logic_l3_cfg_cb, MEM_SPM_LOG_PORT_L3_CB);
        pstLogicalPort->logic_l3_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅÂß¼­ÅäÖÃL2Êý¾Ý¿é¡£                                                  */
    /***************************************************************************/
    if (pstLogicalPort->logic_l2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->logic_l2_cfg_cb, MEM_SPM_LOG_PORT_L2_CB);
        pstLogicalPort->logic_l2_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅÁ÷Ïà¹ØÅäÖÃL2Êý¾Ý¿é¡£                                                */
    /***************************************************************************/
/*    for (i = 0; i < ATG_DCI_LOG_PORT_FLOW_NUM; i++)
    {
        if (pstLogicalPort->flow_cfg_cb != NULL)
        {
            NBB_MM_FREE(pstLogicalPort->flow_cfg_cb, MEM_SPM_LOG_PORT_FLOW_CB);
            pstLogicalPort->flow_cfg_cb = NULL;
        }
    }
*/
    /***************************************************************************/
    /* ÊÍ·ÅÂß¼­ÅäÖÃCES/CEPÊý¾Ý¿é¡£                                             */
    /***************************************************************************/
    if (pstLogicalPort->ces_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->ces_cfg_cb, MEM_SPM_LOG_PORT_CES_CB);
        pstLogicalPort->ces_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅIpv4µØÖ·ÅäÖÃL3Êý¾Ý¿é¡£                                             */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        if (pstLogicalPort->ipv4_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstLogicalPort->ipv4_cfg_cb[i], MEM_SPM_LOG_PORT_IPV4_CB);
            pstLogicalPort->ipv4_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* ÊÍ·ÅIpv6µØÖ·ÅäÖÃL3Êý¾Ý¿é¡£                                             */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        if (pstLogicalPort->ipv6_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstLogicalPort->ipv6_cfg_cb[i], MEM_SPM_LOG_PORT_IPV6_CB);
            pstLogicalPort->ipv6_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* ÊÍ·Å×é²¥×éµØÖ·L3Êý¾Ý¿é¡£                                                */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        if (pstLogicalPort->mc_ipv4_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstLogicalPort->mc_ipv4_cfg_cb[i], MEM_SPM_LOG_PORT_MC_IPV4_CB);
            pstLogicalPort->mc_ipv4_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* ÊÍ·ÅÐéÄâMACºÍIPµØÖ·ÅäÖÃL3Êý¾Ý¿é¡£                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        if (pstLogicalPort->vip_vmac_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstLogicalPort->vip_vmac_cfg_cb[i], MEM_SPM_LOG_PORT_VIP_VMAC_CB);
            pstLogicalPort->vip_vmac_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* ÊÍ·ÅÐéÄâMACºÍIPµØÖ·ÅäÖÃL3Êý¾Ý¿é¡£                                       */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        if (pstLogicalPort->vipv6_vmac_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstLogicalPort->vipv6_vmac_cfg_cb[i], MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
            pstLogicalPort->vipv6_vmac_cfg_cb[i] = NULL;
        }
    }   

    /***************************************************************************/
    /* ÊÍ·ÅVE MACµØÖ·Êý¾Ý¿é¡£                                             */
    /***************************************************************************/
    if (pstLogicalPort->ve_mac_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->ve_mac_cfg_cb, MEM_SPM_LOG_PORT_VE_MAC_CB);
        pstLogicalPort->ve_mac_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅDiff-ServÅäÖÃÊý¾Ý¿é¡£                                             */
    /***************************************************************************/
    //if (pstLogicalPort->diff_serv_cfg_cb != NULL)
    //{
    //    NBB_MM_FREE(pstLogicalPort->diff_serv_cfg_cb, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    //    pstLogicalPort->diff_serv_cfg_cb = NULL;
    //}

    /***************************************************************************/
    /* ÊÍ·ÅÁ÷Ïà¹ØDiff-ServÅäÖÃL2Êý¾Ý¿é¡£                                                */
    /***************************************************************************/
    //for (i = 0; i < ATG_DCI_LOG_FLOW_DIFF_SERV_NUM; i++)
    //{
    //    if (pstLogicalPort->flow_diff_serv_cfg_cb[i] != NULL)
    //    {
    //        NBB_MM_FREE(pstLogicalPort->flow_diff_serv_cfg_cb[i], MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    //        pstLogicalPort->flow_diff_serv_cfg_cb[i] = NULL;
    //    }
    //}
    
    /***************************************************************************/
    /* ÊÍ·ÅÁ÷Ïà¹ØDiff-ServÅäÖÃL2Êý¾Ý¿é¡£                                                */
    /***************************************************************************/
    for (pstflowdiffservcb = (SPM_FLOW_DIFF_SERV_CB*) AVLL_FIRST(pstLogicalPort->flow_diff_serv_tree);
         pstflowdiffservcb != NULL;
         pstflowdiffservcb = (SPM_FLOW_DIFF_SERV_CB*) AVLL_FIRST(pstLogicalPort->flow_diff_serv_tree))
    {
    	//NBB_MEMCPY(&node, &(pstTerminalIfCb->spm_terminal_if_node), sizeof(AVLL_NODE));
    	AVLL_DELETE(pstLogicalPort->flow_diff_serv_tree, pstflowdiffservcb->spm_flow_diff_serv_node);
    	spm_free_flow_diff_serv_cb(pstflowdiffservcb NBB_CCXT);
    }
    
    /***************************************************************************/
    /* ÊÍ·ÅÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL3ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    //if (pstLogicalPort->inclassify_qos_cfg_cb != NULL)
    //{
    //    NBB_MM_FREE(pstLogicalPort->inclassify_qos_cfg_cb, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    //    pstLogicalPort->inclassify_qos_cfg_cb = NULL;
    //}
    
    /***************************************************************************/
    /* ÊÍ·ÅÉÏ»°¸´ÔÓÁ÷·ÖÀàQOS²ßÂÔÅäÖÃL3ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    for (pstinclassifyqoscb = (SPM_INCLASSIFY_QOS_CB*) AVLL_FIRST(pstLogicalPort->inclassify_qos_tree);
         pstinclassifyqoscb != NULL;
         pstinclassifyqoscb = (SPM_INCLASSIFY_QOS_CB*) AVLL_FIRST(pstLogicalPort->inclassify_qos_tree))
    {
    	//NBB_MEMCPY(&node, &(pstTerminalIfCb->spm_terminal_if_node), sizeof(AVLL_NODE));
    	AVLL_DELETE(pstLogicalPort->inclassify_qos_tree, pstinclassifyqoscb->spm_inclassify_qos_node);
    	spm_free_inclassify_qos_cb(pstinclassifyqoscb NBB_CCXT);
    }
    
    /***************************************************************************/
    /* ÊÍ·ÅÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL3ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    //if (pstLogicalPort->up_user_qos_cfg_cb != NULL)
    //{
    //    NBB_MM_FREE(pstLogicalPort->up_user_qos_cfg_cb, MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
    //    pstLogicalPort->up_user_qos_cfg_cb = NULL;
    //}

    /***************************************************************************/
    /* ÊÍ·ÅÉÏ»°ÓÃ»§×éQOS²ßÂÔÅäÖÃ(ÎÞÐ§)ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    if (pstLogicalPort->up_user_group_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->up_user_group_qos_cfg_cb, MEM_SPM_LOG_PORT_UP_GROUP_QOS_CB);
        pstLogicalPort->up_user_group_qos_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅÏÂ»°ÓÃ»§¶ÓÁÐQOS²ßÂÔÅäÖÃL3ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    if (pstLogicalPort->down_user_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->down_user_qos_cfg_cb, MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);
        pstLogicalPort->down_user_qos_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅÏÂ»°ÓÃ»§×éQOSÅäÖÃL3ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    if (pstLogicalPort->down_user_group_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->down_user_group_qos_cfg_cb, MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);
        pstLogicalPort->down_user_group_qos_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ÊÍ·ÅÁ÷Ïà¹ØÉÏ»°ÓÃ»§QOS²ßÂÔÅäÖÃL2ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    //if (pstLogicalPort->flow_up_user_qos_cfg_cb != NULL)
    //{
    //    NBB_MM_FREE(pstLogicalPort->flow_up_user_qos_cfg_cb, MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB);
    //    pstLogicalPort->flow_up_user_qos_cfg_cb = NULL;
    //}

    /***************************************************************************/
    /* ÊÍ·ÅÁ÷°ü¹ýÂËÆ÷traffic_filterL3ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    for (psttrafficfiltercb = (SPM_TRAFFIC_FILTER_CB*) AVLL_FIRST(pstLogicalPort->traffic_filter_tree);
         psttrafficfiltercb != NULL;
         psttrafficfiltercb = (SPM_TRAFFIC_FILTER_CB*) AVLL_FIRST(pstLogicalPort->traffic_filter_tree))
    {
    	//NBB_MEMCPY(&node, &(pstTerminalIfCb->spm_terminal_if_node), sizeof(AVLL_NODE));
    	AVLL_DELETE(pstLogicalPort->traffic_filter_tree, psttrafficfiltercb->spm_traffic_filter_node);
    	spm_free_traffic_filter_cb(psttrafficfiltercb NBB_CCXT);
    }
    
    /***************************************************************************/
    /* ÊÍ·ÅÁ÷°ü¹ýÂËÆ÷traffic_filterL3ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    //for (i = 0; i < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; i++)
    //{
    //    if (pstLogicalPort->traffic_filter_cfg_cb[i] != NULL)
    //    {
    //        NBB_MM_FREE(pstLogicalPort->traffic_filter_cfg_cb[i], MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    //        pstLogicalPort->traffic_filter_cfg_cb[i] = NULL;
    //    }
    //}

    /***************************************************************************/
    /* ÊÍ·ÅÖÕ½á×Ó½Ó¿ÚÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
         pstTerminalIfCb != NULL;
         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree))
    {
    	//NBB_MEMCPY(&node, &(pstTerminalIfCb->spm_terminal_if_node), sizeof(AVLL_NODE));
    	AVLL_DELETE(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);
    	spm_free_terminal_if_cb(pstTerminalIfCb NBB_CCXT);
    }

    /***************************************************************************/
    /* ÊÍ·Ådiff_servÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    for (pstdiffservcb = (SPM_DIFF_SERV_CB*) AVLL_FIRST(pstLogicalPort->diff_serv_tree);
         pstdiffservcb != NULL;
         pstdiffservcb = (SPM_DIFF_SERV_CB*) AVLL_FIRST(pstLogicalPort->diff_serv_tree))
    {
    	//NBB_MEMCPY(&node, &(pstTerminalIfCb->spm_terminal_if_node), sizeof(AVLL_NODE));
    	AVLL_DELETE(pstLogicalPort->diff_serv_tree, pstdiffservcb->spm_diff_serv_node);
    	spm_free_diff_serv_cb(pstdiffservcb NBB_CCXT);
    }

    /***************************************************************************/
    /* ÊÍ·ÅIPV6×é²¥×éµØÖ·L3Êý¾Ý¿é¡£                                                */
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        if (pstLogicalPort->mc_ipv6_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstLogicalPort->mc_ipv6_cfg_cb[i], MEM_SPM_LOG_PORT_MC_IPV6_CB);
            pstLogicalPort->mc_ipv6_cfg_cb[i] = NULL;
        }
    }

    /***************************************************************************/
    /* ÊÍ·ÅDiff-ServÅäÖÃL2£¨VPÏÂ»°UNI²à£©ÅäÖÃÊý¾Ý¿é¡£                                         */
    /***************************************************************************/
    if (pstLogicalPort->ds_l2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->ds_l2_cfg_cb, MEM_SPM_LOG_PORT_DS_L2_CB);
        pstLogicalPort->ds_l2_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* É¾³ý¿ØÖÆ¿éµÄ¾ä±ú¡£                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstLogicalPort->spm_logical_port_handle, HDL_SPM_LOGICAL_PORT_CB);

    /***************************************************************************/
    /* ÏÖÔÚÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ¿ØÖÆ¿é¡£                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstLogicalPort, MEM_SPM_LOGICAL_PORT_CB);
    pstLogicalPort = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_terminal_if_cb
   ¹¦ÄÜÃèÊö  : ¶Ë¿ÚÂß¼­ÅäÖÃÖÕ½á×Ó½Ó¿Ú½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_TERMINAL_IF_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê9ÔÂ5ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_TERMINAL_IF_CB *spm_alloc_terminal_if_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;

    NBB_TRC_ENTRY("spm_alloc_terminal_if_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄVPWSÅäÖÃÌõÄ¿¡£*/
    pstTerminalIfCb = (SPM_TERMINAL_IF_CB *)NBB_MM_ALLOC(sizeof(SPM_TERMINAL_IF_CB), NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
    if (pstTerminalIfCb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:ÉêÇëpstTerminalIfÌõÄ¿¿Õ¼äÎªNULL!"));
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯VPWSÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstTerminalIfCb, 0, sizeof(SPM_TERMINAL_IF_CB));

    /* ½¨Á¢ÓÃÓÚ¸ÃVC±íÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    pstTerminalIfCb->spm_terminal_if_handle = NBB_CREATE_HANDLE(pstTerminalIfCb, HDL_SPM_TERMINAL_IF_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_TERMINAL_IF_CB allocated at %p with handle %#lx",
            pstTerminalIfCb, pstTerminalIfCb->spm_terminal_if_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTerminalIfCb->spm_terminal_if_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstTerminalIfCb);
}


/*****************************************************************************
   º¯ Êý Ãû  : spm_free_terminal_if_cb
   ¹¦ÄÜÃèÊö  : ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃÖÕ½á×Ó½Ó¿Ú½ÚµãµÄÄÚ´æ¿Õ¼ä
   ÊäÈë²ÎÊý  : (SPM_TERMINAL_IF_CB *pstTerminalIf
             NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê9ÔÂ5ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_free_terminal_if_cb(SPM_TERMINAL_IF_CB *pstTerminalIfCb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_terminal_if_cb");

    /***************************************************************************/
    /* ¼ì²é¿ØÖÆ¿éµÄÕýÈ·ÐÔ¡£                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTerminalIfCb, sizeof(SPM_TERMINAL_IF_CB), MEM_SPM_LOG_PORT_TERMINAL_IF_CB);

    if (pstTerminalIfCb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* É¾³ýµ¥ÅÌÐÅÏ¢¿ØÖÆ¿é¡£                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_TERMINAL_IF_CB at %p", pstTerminalIfCb));
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* ÊÍ·Å»ù±¾ÅäÖÃÊý¾Ý¿é¡£                                                    */
    /***************************************************************************/
    OS_MEMSET(&pstTerminalIfCb->terminal_if_cfg, 0, sizeof(ATG_DCI_LOG_PORT_VLAN));
    
    /***************************************************************************/
    /* É¾³ý¿ØÖÆ¿éµÄ¾ä±ú¡£                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstTerminalIfCb->spm_terminal_if_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* ÏÖÔÚÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ¿ØÖÆ¿é¡£                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstTerminalIfCb, MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
    pstTerminalIfCb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_diff_serv_cb
   ¹¦ÄÜÃèÊö  : ¶Ë¿ÚÂß¼­ÅäÖÃdiff_serv½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_DIFF_SERV_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2015Äê1ÔÂ22ÈÕ
    ×÷    Õß   : fansongbo
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_DIFF_SERV_CB *spm_alloc_diff_serv_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_DIFF_SERV_CB *pstdiffservcb = NULL;

    NBB_TRC_ENTRY("spm_alloc_diff_serv_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄVPWSÅäÖÃÌõÄ¿¡£*/
    pstdiffservcb = (SPM_DIFF_SERV_CB *)NBB_MM_ALLOC(sizeof(SPM_DIFF_SERV_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    
    if (pstdiffservcb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:ÉêÇëpstdiffservcbÌõÄ¿¿Õ¼äÎªNULL!"));
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯VPWSÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstdiffservcb, 0, sizeof(SPM_DIFF_SERV_CB));

    /* ½¨Á¢ÓÃÓÚ¸ÃVC±íÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    //pstdiffservcb->spm_diff_serv_handle = NBB_CREATE_HANDLE(pstdiffservcb, HDL_SPM_TERMINAL_IF_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_DIFF_SERV_CB allocated at %p with handle %#lx",
            //pstdiffservcb, pstdiffservcb->spm_diff_serv_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstdiffservcb->spm_diff_serv_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstdiffservcb);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_free_diff_serv_cb
  ¹¦ÄÜÃèÊö  : ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃdiff_serv½ÚµãµÄÄÚ´æ¿Õ¼ä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_DIFF_SERV_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2015Äê1ÔÂ22ÈÕ
    ×÷    Õß   : fansongbo
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_free_diff_serv_cb(SPM_DIFF_SERV_CB *pstdiffservcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_diff_serv_cb");

    /***************************************************************************/
    /* ¼ì²é¿ØÖÆ¿éµÄÕýÈ·ÐÔ¡£                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstdiffservcb, sizeof(SPM_DIFF_SERV_CB), MEM_SPM_LOG_PORT_DIFF_SERV_CB);

    if (pstdiffservcb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* É¾³ýµ¥ÅÌÐÅÏ¢¿ØÖÆ¿é¡£                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_DIFF_SERV_CB at %p", pstdiffservcb));
    
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* ÊÍ·Å»ù±¾ÅäÖÃÊý¾Ý¿é¡£                                                    */
    /***************************************************************************/
    OS_MEMSET(&pstdiffservcb->diff_serv_cfg, 0, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
    
    /***************************************************************************/
    /* É¾³ý¿ØÖÆ¿éµÄ¾ä±ú¡£                                                      */
    /***************************************************************************/
    //NBB_DESTROY_HANDLE(pstdiffservcb->spm_diff_serv_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* ÏÖÔÚÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ¿ØÖÆ¿é¡£                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstdiffservcb, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    pstdiffservcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_flow_diff_serv_cb
   ¹¦ÄÜÃèÊö  : ¶Ë¿ÚÂß¼­ÅäÖÃdiff_serv½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_DIFF_SERV_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2015Äê3ÔÂ9ÈÕ
    ×÷    Õß   : fansongbo
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_FLOW_DIFF_SERV_CB *spm_alloc_flow_diff_serv_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_FLOW_DIFF_SERV_CB *pstflowdiffservcb = NULL;

    NBB_TRC_ENTRY("spm_alloc_flow_diff_serv_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄVPWSÅäÖÃÌõÄ¿¡£*/
    pstflowdiffservcb = (SPM_FLOW_DIFF_SERV_CB *)NBB_MM_ALLOC(sizeof(SPM_FLOW_DIFF_SERV_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    
    if (pstflowdiffservcb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:ÉêÇëpstflowdiffservcbÌõÄ¿¿Õ¼äÎªNULL!"));
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯VPWSÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstflowdiffservcb, 0, sizeof(SPM_FLOW_DIFF_SERV_CB));

    /* ½¨Á¢ÓÃÓÚ¸ÃVC±íÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    //pstdiffservcb->spm_diff_serv_handle = NBB_CREATE_HANDLE(pstdiffservcb, HDL_SPM_TERMINAL_IF_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_DIFF_SERV_CB allocated at %p with handle %#lx",
            //pstdiffservcb, pstdiffservcb->spm_diff_serv_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstflowdiffservcb->spm_flow_diff_serv_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstflowdiffservcb);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_free_flow_diff_serv_cb
  ¹¦ÄÜÃèÊö  : ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃdiff_serv½ÚµãµÄÄÚ´æ¿Õ¼ä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_DIFF_SERV_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2015Äê3ÔÂ9ÈÕ
    ×÷    Õß   : fansongbo
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_free_flow_diff_serv_cb(SPM_FLOW_DIFF_SERV_CB *pstflowdiffservcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_flow_diff_serv_cb");

    /***************************************************************************/
    /* ¼ì²é¿ØÖÆ¿éµÄÕýÈ·ÐÔ¡£                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstflowdiffservcb, sizeof(SPM_FLOW_DIFF_SERV_CB), MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);

    if (pstflowdiffservcb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* É¾³ýµ¥ÅÌÐÅÏ¢¿ØÖÆ¿é¡£                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_FLOW_DIFF_SERV_CB at %p", pstflowdiffservcb));
    
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* ÊÍ·Å»ù±¾ÅäÖÃÊý¾Ý¿é¡£                                                    */
    /***************************************************************************/
    OS_MEMSET(&pstflowdiffservcb->flow_diff_serv_cfg, 0, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
    
    /***************************************************************************/
    /* É¾³ý¿ØÖÆ¿éµÄ¾ä±ú¡£                                                      */
    /***************************************************************************/
    //NBB_DESTROY_HANDLE(pstdiffservcb->spm_diff_serv_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* ÏÖÔÚÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ¿ØÖÆ¿é¡£                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstflowdiffservcb, MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    pstflowdiffservcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_inclassify_qos_cb
   ¹¦ÄÜÃèÊö  : ¶Ë¿ÚÂß¼­ÅäÖÃdiff_serv½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_DIFF_SERV_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2015Äê3ÔÂ9ÈÕ
    ×÷    Õß   : fansongbo
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý(SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(pstLogicalPort->inclassify_qos_tree,

*****************************************************************************/
SPM_INCLASSIFY_QOS_CB *spm_alloc_inclassify_qos_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_INCLASSIFY_QOS_CB *pstinclassifyqoscb = NULL;

    NBB_TRC_ENTRY("spm_alloc_inclassify_qos_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄVPWSÅäÖÃÌõÄ¿¡£*/
    pstinclassifyqoscb = (SPM_INCLASSIFY_QOS_CB *)NBB_MM_ALLOC(sizeof(SPM_INCLASSIFY_QOS_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    
    if (pstinclassifyqoscb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:ÉêÇëpstinclassifyqoscbÌõÄ¿¿Õ¼äÎªNULL!"));
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯VPWSÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstinclassifyqoscb, 0, sizeof(SPM_INCLASSIFY_QOS_CB));

    /* ½¨Á¢ÓÃÓÚ¸ÃVC±íÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    //pstdiffservcb->spm_diff_serv_handle = NBB_CREATE_HANDLE(pstdiffservcb, HDL_SPM_TERMINAL_IF_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_DIFF_SERV_CB allocated at %p with handle %#lx",
            //pstdiffservcb, pstdiffservcb->spm_diff_serv_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstinclassifyqoscb->spm_inclassify_qos_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstinclassifyqoscb);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_free_inclassify_qos_cb
  ¹¦ÄÜÃèÊö  : ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃdiff_serv½ÚµãµÄÄÚ´æ¿Õ¼ä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_DIFF_SERV_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2015Äê3ÔÂ9ÈÕ
    ×÷    Õß   : fansongbo
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_free_inclassify_qos_cb(SPM_INCLASSIFY_QOS_CB *pstinclassifyqoscb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_inclassify_qos_cb");

    /***************************************************************************/
    /* ¼ì²é¿ØÖÆ¿éµÄÕýÈ·ÐÔ¡£                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstinclassifyqoscb, sizeof(SPM_INCLASSIFY_QOS_CB), MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);

    if (pstinclassifyqoscb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* É¾³ýµ¥ÅÌÐÅÏ¢¿ØÖÆ¿é¡£                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_INCLASSIFY_QOS_CB at %p", pstinclassifyqoscb));
    
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* ÊÍ·Å»ù±¾ÅäÖÃÊý¾Ý¿é¡£                                                    */
    /***************************************************************************/
    OS_MEMSET(&pstinclassifyqoscb->inclassify_qos_cfg, 0, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
    
    /***************************************************************************/
    /* É¾³ý¿ØÖÆ¿éµÄ¾ä±ú¡£                                                      */
    /***************************************************************************/
    //NBB_DESTROY_HANDLE(pstdiffservcb->spm_diff_serv_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* ÏÖÔÚÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ¿ØÖÆ¿é¡£                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstinclassifyqoscb, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    pstinclassifyqoscb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_traffic_filter_cb
   ¹¦ÄÜÃèÊö  : ¶Ë¿ÚÂß¼­ÅäÖÃdiff_serv½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_DIFF_SERV_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :(SPM_TRAFFIC_FILTER_CB*) AVLL_NEXT(pstLogicalPort->traffic_filter_tree,

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2015Äê3ÔÂ9ÈÕ
    ×÷    Õß   : fansongbo
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_TRAFFIC_FILTER_CB *spm_alloc_traffic_filter_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_TRAFFIC_FILTER_CB *psttrafficfiltercb = NULL;

    NBB_TRC_ENTRY("spm_alloc_traffic_filter_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄVPWSÅäÖÃÌõÄ¿¡£*/
    psttrafficfiltercb = (SPM_TRAFFIC_FILTER_CB *)NBB_MM_ALLOC(sizeof(SPM_TRAFFIC_FILTER_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    
    if (psttrafficfiltercb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:ÉêÇëpsttrafficfiltercbÌõÄ¿¿Õ¼äÎªNULL!"));
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯VPWSÅäÖÃÌõÄ¿ */
    OS_MEMSET(psttrafficfiltercb, 0, sizeof(SPM_TRAFFIC_FILTER_CB));

    /* ½¨Á¢ÓÃÓÚ¸ÃVC±íÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    //pstdiffservcb->spm_diff_serv_handle = NBB_CREATE_HANDLE(pstdiffservcb, HDL_SPM_TERMINAL_IF_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_DIFF_SERV_CB allocated at %p with handle %#lx",
            //pstdiffservcb, pstdiffservcb->spm_diff_serv_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(psttrafficfiltercb->spm_traffic_filter_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(psttrafficfiltercb);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_free_traffic_filter_cb
  ¹¦ÄÜÃèÊö  : ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃdiff_serv½ÚµãµÄÄÚ´æ¿Õ¼ä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_DIFF_SERV_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2015Äê3ÔÂ9ÈÕ
    ×÷    Õß   : fansongbo
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_free_traffic_filter_cb(SPM_TRAFFIC_FILTER_CB *psttrafficfiltercb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_traffic_filter_cb");

    /***************************************************************************/
    /* ¼ì²é¿ØÖÆ¿éµÄÕýÈ·ÐÔ¡£                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(psttrafficfiltercb, sizeof(SPM_TRAFFIC_FILTER_CB), MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);

    if (psttrafficfiltercb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* É¾³ýµ¥ÅÌÐÅÏ¢¿ØÖÆ¿é¡£                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_TRAFFIC_FILTER_CB at %p", psttrafficfiltercb));
    
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* ÊÍ·Å»ù±¾ÅäÖÃÊý¾Ý¿é¡£                                                    */
    /***************************************************************************/
    OS_MEMSET(&psttrafficfiltercb->traffic_filter_cfg, 0, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
    
    /***************************************************************************/
    /* É¾³ý¿ØÖÆ¿éµÄ¾ä±ú¡£                                                      */
    /***************************************************************************/
    //NBB_DESTROY_HANDLE(pstdiffservcb->spm_diff_serv_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* ÏÖÔÚÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃ¿ØÖÆ¿é¡£                                            */
    /***************************************************************************/
    NBB_MM_FREE(psttrafficfiltercb, MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    psttrafficfiltercb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_free_all_logical_port
   ¹¦ÄÜÃèÊö  : ÊÍ·Å¶Ë¿ÚÂß¼­ÅäÖÃËùÓÐµÄÄÚ´æ¿Õ¼ä
   ÊäÈë²ÎÊý  : ÎÞ
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  :
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2013Äê11ÔÂ6ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
NBB_VOID spm_free_all_logical_port(NBB_CXT_T NBB_CXT)
{
	SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
	
    for (pstLogicalPort = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree);
         pstLogicalPort != NULL;
         pstLogicalPort = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree))
    {
    	AVLL_DELETE(SHARED.logical_port_tree, pstLogicalPort->spm_logical_port_node);
    	spm_free_logical_port_cb(pstLogicalPort NBB_CCXT);
    }
}


#if 0

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_flow_node_cb
   ¹¦ÄÜÃèÊö  : Á÷½Úµã½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_FLOW_NODE_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê10ÔÂ16ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_FLOW_NODE_CB *spm_alloc_flow_node_cb()
{
    SPM_FLOW_NODE_CB *pstFlowNode = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_flow_node_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    pstFlowNode = (SPM_FLOW_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_FLOW_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_FLOW_NODE_CB);
    if (pstFlowNode == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstFlowNode, 0, sizeof(SPM_FLOW_NODE_CB));

    /* ½¨Á¢ÓÃÓÚ¸Ã½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    pstFlowNode->flow_tree_handle = NBB_CREATE_HANDLE(pstFlowNode, HDL_SPM_FLOW_NODE_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_FLOW_NODE_CB allocated at %p with handle %#lx",
            pstFlowNode, pstFlowNode->flow_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstFlowNode->flow_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstFlowNode);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_ipv4_node_cb
   ¹¦ÄÜÃèÊö  : ipv4½áµã½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_IPV4_NODE_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê10ÔÂ16ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_IPV4_NODE_CB *spm_alloc_ipv4_node_cb()
{
    SPM_IPV4_NODE_CB *pstIpv4Node = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_ipv4_node_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    pstIpv4Node = (SPM_IPV4_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_IPV4_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_IPV4_NODE_CB);
    if (pstIpv4Node == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstIpv4Node, 0, sizeof(SPM_IPV4_NODE_CB));

    /* ½¨Á¢ÓÃÓÚ¸Ã½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    pstIpv4Node->ipv4_tree_handle = NBB_CREATE_HANDLE(pstIpv4Node, HDL_SPM_IPV4_NODE_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_IPV4_NODE_CB allocated at %p with handle %#lx",
            pstIpv4Node, pstIpv4Node->ipv4_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstIpv4Node->ipv4_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstIpv4Node);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_ipv6_node_cb
   ¹¦ÄÜÃèÊö  : ipv6½Úµã½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_IPV6_NODE_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê10ÔÂ16ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_IPV6_NODE_CB *spm_alloc_ipv6_node_cb()
{
    SPM_IPV6_NODE_CB *pstIpv6Node = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_ipv6_node_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    pstIpv6Node = (SPM_IPV6_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_IPV6_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_IPV6_NODE_CB);
    if (pstIpv6Node == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstIpv6Node, 0, sizeof(SPM_IPV6_NODE_CB));

    /* ½¨Á¢ÓÃÓÚ¸Ã½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    pstIpv6Node->ipv6_tree_handle = NBB_CREATE_HANDLE(pstIpv6Node, HDL_SPM_IPV6_NODE_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_IPV6_NODE_CB allocated at %p with handle %#lx",
            pstIpv6Node, pstIpv6Node->ipv6_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstIpv6Node->ipv6_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstIpv6Node);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_mc_mac_node_cb
   ¹¦ÄÜÃèÊö  : ×é²¥µØÖ·½Úµã½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_MC_MAC_NODE_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê10ÔÂ16ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_MC_MAC_NODE_CB *spm_alloc_mc_mac_node_cb()
{
    SPM_MC_MAC_NODE_CB *pstMcMacNode = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_mc_mac_node_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    pstMcMacNode = (SPM_MC_MAC_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_MC_MAC_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_MC_MAC_NODE_CB);
    if (pstMcMacNode == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstMcMacNode, 0, sizeof(SPM_MC_MAC_NODE_CB));

    /* ½¨Á¢ÓÃÓÚ¸Ã½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    pstMcMacNode->mc_mac_tree_handle = NBB_CREATE_HANDLE(pstMcMacNode, HDL_SPM_MC_IPV4_NODE_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_MC_MAC_NODE_CB allocated at %p with handle %#lx",
            pstMcMacNode, pstMcMacNode->mc_mac_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstMcMacNode->mc_mac_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstMcMacNode);
}

/*****************************************************************************
   º¯ Êý Ãû  : spm_alloc_vip_vmac_node_cb
   ¹¦ÄÜÃèÊö  : ÐéÄâMACºÍIPµØÖ·½Úµã½á¹¹ÄÚ´æ¿Õ¼ä·ÖÅä
   ÊäÈë²ÎÊý  : NBB_CCXT_T NBB_CXT
   Êä³ö²ÎÊý  : ÎÞ
   ·µ »Ø Öµ  : SPM_VIP_VMAC_NODE_CB
   µ÷ÓÃº¯Êý  :
   ±»µ÷º¯Êý  :

   ÐÞ¸ÄÀúÊ·      :
   1.ÈÕ    ÆÚ   : 2012Äê10ÔÂ16ÈÕ
    ×÷    Õß   : xiaoxiang
    ÐÞ¸ÄÄÚÈÝ   : ÐÂÉú³Éº¯Êý

*****************************************************************************/
SPM_VIP_VMAC_NODE_CB *spm_alloc_vip_vmac_node_cb()
{
    SPM_VIP_VMAC_NODE_CB *pstVipVmacNode = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_vip_vmac_node_cb");

    /* ·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    pstVipVmacNode = (SPM_VIP_VMAC_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_VIP_VMAC_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_VIP_VMAC_NODE_CB);
    if (pstVipVmacNode == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ³õÊ¼»¯½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿ */
    OS_MEMSET(pstVipVmacNode, 0, sizeof(SPM_VIP_VMAC_NODE_CB));

    /* ½¨Á¢ÓÃÓÚ¸Ã½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿µÄ¾ä±ú£¬×÷ÎªÒì²½ÏûÏ¢½»»»µÄÏà¹ØÆ÷¡£*/
    pstVipVmacNode->vip_vmac_tree_handle = NBB_CREATE_HANDLE(pstVipVmacNode, HDL_SPM_VIP_VMAC_NODE_CB);

    /* ³É¹¦·ÖÅäÒ»¸öÐÂµÄ½Ó¿ÚÎïÀíÅäÖÃÌõÄ¿¡£*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VIP_VMAC_NODE_CB allocated at %p with handle %#lx",
            pstVipVmacNode, pstVipVmacNode->vip_vmac_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstVipVmacNode->vip_vmac_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstVipVmacNode);
}

#endif

#endif

