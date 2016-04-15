/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_logical_port_proc.c
   �� �� ��   : ����
   ��    ��   : xiaoxiang
   ��������   : 2012��9��18��
   ����޸�   :
   ��������   : �˿��߼����ô���
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��9��18��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef LOGICAL_PORT_CFG

extern unsigned char logical_port_cfg_print_setting;

/*****************************************************************************
   �� �� ��  : spm_check_if_vip_vmac_exist
   ��������  : ���һ���߼��˿�������MAC��IP��ַ�����Ƿ����ظ�
   �������  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pstVipVmac
   �������  : ��
   �� �� ֵ  : 0 �����ظ�
             n : ���ظ���n=����λ��+1
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��5��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_check_if_vipv6_vmac_exist(NBB_ULONG ulPortIndex, 
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *pst_vipv6_vmac NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;/*����ֵ*/
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
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_vipv6_vmac_exist() PORT_INDEX=%ld �߼��˿ڲ�����",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_vipv6_vmac_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);

        OS_SPRINTF(uc_message,
            "***ERROR***:spm_check_if_vipv6_vmac_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n",
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
            //�������MAC��IP��ַ������ȣ����ش洢λ��
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
   �� �� ��  : spm_rcv_dci_set_logical_port
   ��������  : �˿��߼����ô���
   �������  : ATG_DCI_SET_LOGICAL_PORT *pstSetLogicalPort
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��10��12��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
    NBB_INT v6cfgpos = 0;/* vrrp �Ĵ洢λ��*/
    NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    NBB_BYTE ucLocalSlot = 0;
    NBB_USHORT usFlowId = 0;
    NBB_ULONG ulVlanPosId = 0;
    NBB_ULONG ul_vlanposid2 = 0;  /* ��Ŀ�ļ�ֵ */
    NBB_BYTE ulAssociateIfKey = 0;

    INTF_T stIntfL3;
    INTF_T stIntfFlow; /* vrrp�Ľṹ��*/
    VRRP_ACL_T stvrrp; /* vrrp�Ľṹ��*/
    VRRP_ACL_T stvrrp_v6; /* vrrp ipv6�Ľṹ��*/
    NBB_BYTE ve_mac[6] = {0x00, 0x01, 0x002, 0x03, 0x04, 0x05};
    SPM_PORT_INFO_CB stPortInfo;
    SPM_VPN_PORT_INFO_CB stVpPortInfo;

    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;
    SPM_TERMINAL_IF_CB *tempterminalifcb = NULL;/* VLAN�ӿ��׵�ַ*/
    SPM_TERMINAL_IF_CB *tempterminalifcb_v6 = NULL;/* VLAN�ӿ��׵�ַ*/
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
    
    /* ��ȡ�������� */
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

    /* ��ȡ�������� */
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *pst_vipv6_vmac_data[ATG_DCI_LOG_PORT_VIP_VMAC_NUM];
    ATG_DCI_LOG_PORT_MC_IPV6_DATA * pstMcIpv6Data[ATG_DCI_LOG_PORT_MC_IPV6_NUM];
    //ATG_DCI_LOG_PORT_TRAFFIC_FILTER *pstTrafficFilter[ATG_DCI_LOG_TRAFFIC_FILTER_NUM];
    //ATG_DCI_LOG_PORT_TERMINAL_IF *pstTerminalIf[ATG_DCI_LOG_TERMINAL_IF_NUM];

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
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
    NBB_BYTE *puc_vipv6_vmac_data_start = NULL;/* IPS��Ϣƫ�Ƶ��׵�ַ */
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

    /* �����õĲ���ģʽ */
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
    NBB_ULONG ul_oper_vipv6_vmac = ATG_DCI_OPER_NULL;/* �����õĲ���ģʽ */
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

    //QoS�ṹ��
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
    NBB_ULONG intf_pos_id_temp = 0; /* ��Ŀ�ļ�ֵ */
    NBB_ULONG vrrp4_pos_id_temp = 0; /* ��Ŀ�ļ�ֵ */
    NBB_ULONG vrrp6_pos_id_temp = 0; /* ��Ŀ�ļ�ֵ */
    
    /* ��Ŀ�ļ�ֵ */
    NBB_USHORT usVplsIdKey = 0;
    NBB_USHORT usVsiMcId = 0;
    SPM_VPLS_CB *pstVpls = NULL;
    ATG_DCI_LOG_PORT_VLAN flowdiffservcfgkey ;
    ATG_DCI_LOG_PORT_VLAN inclassifyqoscfgkey ;
    ATG_DCI_LOG_PORT_VLAN trafficfiltercfgkey ;
    ATG_DCI_LOG_PORT_VLAN diffservcfgkey ;/* ��Ŀ�ļ�ֵ */

    NBB_TRC_ENTRY("spm_rcv_dci_set_logical_port");

    /* �������ָ�������Ч */
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

    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pstSetLogicalPort->return_code = ATG_DCI_RC_OK;

    //printf("***ERROR***:PORT_INDEX=%d, ApiC3AddIntf() ret=%d ����index=%d��INTFʧ��!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //����
    //OS_PRINTF("***ERROR***:PORT_INDEX=%ld, ApiC3AddIntf() ret=%d ����index=%ld��INTFʧ��!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //��
    //OS_PRINTF("***ERROR***:PORT_INDEX=%d, ApiC3AddIntf() ret=%d ����index=%d��INTFʧ��!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //����
    //OS_PRINTF("***ERROR***:PORT_INDEX=%d, ApiC3AddIntf() ret=%d ����index=%ld��INTFʧ��!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //��
    //OS_PRINTF("***ERROR***:PORT_INDEX=%ld, ApiC3AddIntf() ret=%d ����index=%d��INTFʧ��!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //����
    //OS_PRINTF("***ERROR***:PORT_INDEX=%d, ApiC3AddIntf() ret=%d ����index=%d ��INTFʧ��!\n", ulPortIndexKey, ret,
    // ulPortIndexKey);

    //��ʼ��������
    //for (i = 0; i < ATG_DCI_LOG_PORT_FLOW_NUM; i++)
    //{
    //    pstFlowData = NULL;
    //}

    //��ʼ�������Diff-Serv����L2
    //for (i = 0; i < ATG_DCI_LOG_FLOW_DIFF_SERV_NUM; i++)
    //{
    //    pstFlowDiffServData[i] = NULL;
    //}

    //��ʼ��Ipv4��ַ����
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        pstIpv4Data[i] = NULL;
    }

    //��ʼ��Ipv6��ַ����
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        pstIpv6Data[i] = NULL;
    }

    //��ʼ���鲥���ַ
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        pstMcIpv4Data[i] = NULL;
    }

    //��ʼ��VRRP����MAC��IP��ַ����
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pstVipVmacData[i] = NULL;
    }
    
    //��ʼ��VRRP ipv6����MAC��IP��ַ����
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pst_vipv6_vmac_data[i] = NULL;
    }
    

    //��ʼ��ipv6�鲥��ַ����
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        pstMcIpv6Data[i] = NULL;
    }

    //��ʼ����������traffic_filter����
    //for (i = 0; i < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; i++)
    //{
    //    pstTrafficFilter[i] = NULL;
    //}

    
#ifdef SPU

    /* ��ʼ��intf */
    ApiC3InitIntfStruct(&stIntfL3);
    ApiC3InitIntfStruct(&stIntfFlow);
#endif

    ulPortIndexKey = pstSetLogicalPort->key;

    NBB_TRC_DETAIL((NBB_FORMAT "ulPortIndexKey = %ld", ulPortIndexKey));

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndexKey);

    if (pstLogicalPort == NULL) /* �����Ŀ������*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }

    /* ��ȡ�����ò��� */
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

    /* ɾ��������Ŀ */
    if (pstSetLogicalPort->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ɾ��PORT_INDEX=%ld �Ķ˿��߼�����", ulPortIndexKey));

        /* �����Ŀ�����ڣ���ɾ�� */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:Ҫɾ�������ò�������"));

            OS_PRINTF("***ERROR***:Ҫɾ����PORT_INDEX=%ld �Ķ˿��߼����ò�������!\n", ulPortIndexKey);

            OS_SPRINTF(ucMessage, "***ERROR***:Ҫɾ����PORT_INDEX=%ld �Ķ˿��߼����ò�������!\n", ulPortIndexKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, DEL_UNEXIST_CONFIG_ERROR,
                    "DEL_UNEXIST_CONFIG_ERROR", ucMessage));
                    
            pstSetLogicalPort->return_code = ATG_DCI_RC_OK;

            goto EXIT_LABEL;
        }

        /* ����ɾ�� */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ��ѯ�������ã���tree��ɾ��"));
            spm_dbg_print_logical_port_head(ulPortIndexKey, SPM_OPER_DEL);
            spm_dbg_record_logical_port_head(ulPortIndexKey, SPM_OPER_DEL);

            //ɾ���˿��߼�������������
            spm_del_logical_port_cfg(pstLogicalPort NBB_CCXT);

			if (pstLogicalPort != NULL)
			{
	            AVLL_DELETE(SHARED.logical_port_tree, pstLogicalPort->spm_logical_port_node);

	            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
	            spm_free_logical_port_cb(pstLogicalPort NBB_CCXT);
			}
        }
    }

    /* ���ӻ������Ŀ */
    else
    {

        /* �����Ŀ�����ڣ�����Ҫ������Ŀ */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ����PORT_INDEX=%ld�� �˿��߼�����", ulPortIndexKey));
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
            NBB_TRC_DETAIL((NBB_FORMAT "  ����PORT_INDEX=%ld�� �˿��߼�����", ulPortIndexKey));
            spm_dbg_print_logical_port_head(ulPortIndexKey, SPM_OPER_UPD);
            spm_dbg_record_logical_port_head(ulPortIndexKey, SPM_OPER_UPD);
        }

		//���Ӵ��жϣ�ȡ��PC-Lint�澯
        if (pstLogicalPort == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:Ҫ���ӵ�PORT_INDEX=%ld �Ķ˿��߼�����δ�ܳɹ������ڴ�!", ulPortIndexKey));

            OS_PRINTF("***ERROR***:Ҫ���ӵ�PORT_INDEX=%ld �Ķ˿��߼�����δ�ܳɹ������ڴ�!\n", ulPortIndexKey);

            OS_SPRINTF(ucMessage, "***ERROR***:Ҫ���ӵ�PORT_INDEX=%ld �Ķ˿��߼�����δ�ܳɹ������ڴ�!\n", ulPortIndexKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ALLOC_MEMORY_ERROR,
                    "ALLOC_MEMORY_ERROR", ucMessage));
                    
            pstSetLogicalPort->return_code = ATG_DCI_RC_UNSUCCESSFUL;

            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /* ��ȡ����                                                                */
        /***************************************************************************/
        /******************************** �������� *********************************/
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* �����һ��entry�ĵ�ַ��*/
            pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                &pstSetLogicalPort->basic_data);

            /* �׵�ַΪNULL���쳣 */
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
                    printf("  1����������\n");
                    spm_dbg_print_logical_port_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage, "  1����������\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_logical_port_basic_cfg(pstBasicData);

                NBB_TRC_DETAIL((NBB_FORMAT "  �˿ڱ�ʶ = %d", pstBasicData->port_flag));
                NBB_TRC_DETAIL((NBB_FORMAT "  �˿������� = %d", pstBasicData->port_sub_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  �ӿ�·������ = %d", pstBasicData->port_route_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  �ӽӿ����� = %d", pstBasicData->sub_if_type));

                //��ȡ����λ��
                //spm_hardw_getslot(&ucLocalSlot);

                /* �������� */
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

        /******************************** �������� ********************************/
        if (ulOperPhy == ATG_DCI_OPER_ADD)
        {

            /* �����һ��entry�ĵ�ַ��*/
            pucPhyDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                &pstSetLogicalPort->physical_data);

            /* �׵�ַΪNULL���쳣 */
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
                    printf("  2����������\n");
                    spm_dbg_print_logical_port_phy_cfg(pstPhyData);
                }

                OS_SPRINTF(ucMessage, "  2����������\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_logical_port_phy_cfg(pstPhyData);

                NBB_TRC_DETAIL((NBB_FORMAT "  ����ӿ�index  = %ld", pstPhyData->phy_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  ��·�ۺ�LAG_ID = %ld", pstPhyData->lag_id));
                NBB_TRC_DETAIL((NBB_FORMAT "  ������̫����ID = %ld", pstPhyData->ve_group_id));

				
                //�����ĿΪ���£��Ҵ˴��·�û�и��»������ã���֮ǰ����Ŀ�����˻�������
                if (pstLogicalPort->basic_cfg_cb != NULL)
                {
                	//����˿ڲ�Ϊtunnel�ڻ�loopback�ڣ���Ҫ��ȡʵ��portid
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

	                        //NBB_TRC_DETAIL((NBB_FORMAT "  PORT_INDEX=%d, �˿�%dΪVE, ApiC3SetPortType() ret=%d",
	                        // ulPortIndexKey, stPortInfo.port_id, ret));
	                    }
	                    else if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG)
	                    {
	                        stPortInfo.port_id = pstPhyData->lag_id + ATG_DCI_LAG_OFFSET;
	                    }

	                    pstLogicalPort->port_id = stPortInfo.port_id;
	                    pstLogicalPort->slot_id = stPortInfo.slot_id;
	                    pstLogicalPort->unit_id = stPortInfo.unit_id;

		                //�����ȡ�˿���ȷ�ű���
		                if (pstLogicalPort->port_id != 0)
		                {
		                    /* �������� */
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
		                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:�߼��˿� PORT_INDEX=%ld, ��ȡ����˿�ʧ��",
		                            ulPortIndexKey));

		                    OS_PRINTF("***ERROR***:�߼��˿� PORT_INDEX=%ld, ��ȡ����˿�ʧ��\n", ulPortIndexKey);

		                    OS_SPRINTF(ucMessage, "***ERROR***:�߼��˿� PORT_INDEX=%ld, ��ȡ����˿�ʧ��\n", ulPortIndexKey);
		                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

		                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, PHYSICAL_PORT_NOT_EXIST,
		                            "PHYSICAL_PORT_NOT_EXIST", ucMessage));
		                }
					}	
					else	//����˿�Ϊtunnel�ڻ�loopback�ڣ����ػ���ȫ0���ݸ����̣�ֻ���漴��
					{
	                    /* �������� */
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
                    OS_PRINTF("PORT_INDEX=%ld, ��������ʧ�䣬���������޷���ȡ!\n", ulPortIndexKey);

                    OS_SPRINTF(ucMessage, "PORT_INDEX=%ld, ��������ʧ�䣬���������޷���ȡ!\n", ulPortIndexKey);
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
        /* ����·����µĻ������á�                                                  */
        /* ����Ŀ�и��¡�                                                            */
        /*                                                                           */
        /* L2�˿�                                                                    */
        /*****************************************************************************/
        if ((pstLogicalPort->basic_cfg_cb != NULL) && (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L2))
        {

            /****************************** �߼�����L2 ******************************/
            if (ulOperLogicL2 == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucLogicL2DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->logical_l2_data);

                /* �׵�ַΪNULL���쳣 */
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
                        printf("  4���߼�����L2\n");
                        spm_dbg_print_logical_port_logic_l2_cfg(pstLogicL2Data);
                    }

                    OS_SPRINTF(ucMessage, "  4���߼�����L2\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    //spm_dbg_record_logical_port_logic_l2_cfg(pstLogicL2Data);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ����TAG��Ϊ = %s", pstLogicL2Data->recv_action ? "TAGGED_VLAN" : "RAW_ETH"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ����TAG��Ϊ = %d", pstLogicL2Data->send_tag_action));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ��������/�滻VLAN ID = %d", pstLogicL2Data->send_vlan_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ��������/�滻VLAN PRI = %d", pstLogicL2Data->send_vlan_pri));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IGMP  SNOOPINGʹ�� = %s", pstLogicL2Data->igmp_snooping ? "ʹ��" : "��ʹ��"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  TagType = %x", pstLogicL2Data->tagtype));
                    NBB_TRC_DETAIL((NBB_FORMAT "  MACѧϰ���� = %s", pstLogicL2Data->mac_learning_enable ? "��" : "��"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ˮƽ�ָ�� = %s", pstLogicL2Data->split_horizon_enable ? "��" : "��"));
                    //NBB_TRC_DETAIL((NBB_FORMAT "  ������ = %d", pstLogicL2Data->flow_type));

                    //���posid==0��˵��vp��û�м��뵽VPWS/VPLS�У�ֻ��Ҫ����
                    if (pstLogicalPort->logic_port_info_cb.vp_idx == 0)
                    {
                        /* �������� */
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
                    else    //���posid!=0��˵��vp�Ѿ����뵽VPWS/VPLS�У���Ҫ����
                    {
                        //��ʱ����һ�£�������²��ɹ�����ԭ����
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

                        //����VP
                        if (ret == SUCCESS)
                        {
                            stVpwsUni.port_index = ulPortIndexKey;
                            stVpwsUni.uni_no = pstLogicalPort->logic_port_info_cb.vp_idx;

                            OS_MEMSET(&stVpPortInfo, 0, sizeof(SPM_VPN_PORT_INFO_CB));

                            //stVpPortInfo.vp_idx = pstLogicalPort->logic_port_info_cb.vp_idx;

*/
                        ret = SUCCESS;

                        //����intf�����ʶ���
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
                                            "PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L2ˢ��ʧ��",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3SetIntfPtag() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d "
                                            "CVLAN_ID=%d �� �߼�����L2ˢ��ʧ��\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            	"***ERROR***:ApiC3SetIntfPtag() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d "
			                            	"CVLAN_ID=%d �� �߼�����L2ˢ��ʧ��\n",
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

                        //����vp
                        if (ret == SUCCESS)
                        {
                            stVpwsUni.port_index = ulPortIndexKey;
                            stVpwsUni.uni_no = pstLogicalPort->logic_port_info_cb.vp_idx;/*lint !e613 */

                            OS_MEMSET(&stVpPortInfo, 0, sizeof(SPM_VPN_PORT_INFO_CB));

                            stVpPortInfo.vp_idx = pstLogicalPort->logic_port_info_cb.vp_idx;/*lint !e613 */

                            usVsiMcId = 0;
                            
                            if (L2VPN_VPLS == pstLogicalPort->logic_port_info_cb.vpn_type)    //��Ŀ������
                            {
                                usVplsIdKey = pstLogicalPort->logic_port_info_cb.vpn_id;    
                                pstVpls = AVLL_FIND(SHARED.vpls_tree, &usVplsIdKey);
                                
                                if ((NULL != pstVpls) && (NULL != pstVpls->basic_cfg_cb))   //��Ŀ������
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
                                        "  ***ERROR***:spm_vpn_add_vp_uni() ret=%d ���� VP_UNIʧ�� vpnid=%d vpntype=%d",
                                        ret,
                                        pstLogicalPort->logic_port_info_cb.vpn_id,
                                        pstLogicalPort->logic_port_info_cb.vpn_type));

                                OS_PRINTF("***ERROR***:spm_vpn_add_vp_uni() ret=%d ���� VP_UNIʧ�� vpnid=%d vpntype=%d\n",
                                    ret,
                                    pstLogicalPort->logic_port_info_cb.vpn_id,
                                    pstLogicalPort->logic_port_info_cb.vpn_type);

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:spm_vpn_add_vp_uni() ret=%d ���� VP_UNIʧ�� vpnid=%d vpntype=%d\n",
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
                                    "  ***ERROR***:PORT_INDEX=%ld, ApiC3DelMplsPortFromVpn() ret=%d! ɾ�� VP_UNIʧ��",
                                    ulPortIndexKey, ret));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld, ApiC3DelMplsPortFromVpn() ret=%d! ɾ�� VP_UNIʧ��",
                                ulPortIndexKey,
                                ret);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, ApiC3DelMplsPortFromVpn() ret=%d! ɾ�� VP_UNIʧ��",
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

            /****************************** �߼�����CES/CEP ******************************/
            if (ulOperCes == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucCesDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ces_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucCesDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucCesDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstCesData = (ATG_DCI_LOG_PORT_CES_DATA *)pucCesDataStart;

                    //NBB_TRC_DETAIL((NBB_FORMAT "CES��·ID   = %d", pstCesData->ces_id));
                    //NBB_TRC_DETAIL((NBB_FORMAT "QOS�������� = %d", pstCesData->qos_id));
                    //NBB_TRC_DETAIL((NBB_FORMAT "ACL�������� = %d", pstCesData->acl_id));

                    /* ���ô���todo */

                    /* �������� */
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

	        /****************************** VLAN���� L2����******************************/
	        if (ulOperTerminalIf == ATG_DCI_OPER_ADD)
			{

	            /* �����һ��entry�ĵ�ַ��*/
	            pucTerminalIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->vlan_data);

	            /* �׵�ַΪNULL���쳣 */
	            if (pucTerminalIfDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  pucTerminalIfDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {
	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  21��VLAN���� L2 (num = %d) ADD\n", pstSetLogicalPort->vlan_num);
	                }

	                OS_SPRINTF(ucMessage, "  21��VLAN���� L2 (num = %d) ADD\n", 
                           pstSetLogicalPort->vlan_num);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                            
                    //������߼��˿ڲ����ڣ�ֻ����flow vlan���ã���VPWS/VPLSʱ������
                    if (ucIfExist == ATG_DCI_UNEXIST)
                    {

	                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
		                {
		                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
		                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

		                    /* ���ô���todo */
		                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
		                    {
		                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
		                    }

		                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

		                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
		                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

			                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

							//��������ڣ��������ڵ㣬�������У�������1
			                if (NULL == pstTerminalIfCb)
			                {
			                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
			                	
				                //�������ݲ���������
				                //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
				                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
				                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

				                pstLogicalPort->terminal_if_num++;
			                }
							else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
							{
								pstTerminalIfCb->intf_pos_id = stIntfFlow.posId;
								NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
							}
		                }
                    }
                    //����߼��˿ڴ��ڣ��������ӻ���²�������Ҫѭ�������Ѵ��ڵ���
                    else
                    {
                        OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                        spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                        //1������˿ڲ������κ�VPWS/VPLS
                        //2������Ѿ�����VPWS/VPLS�����˿ڲ����ڱ���λ��ֻ��������
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

			                    /* ���ô���todo */
			                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
			                    {
			                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
			                    }

			                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

			                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
			                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

				                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//��������ڣ��������ڵ㣬�������У�������1
				                if (NULL == pstTerminalIfCb)
				                {
				                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
				                	
					                //�������ݲ���������
					                //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
					                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
					                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

					                pstLogicalPort->terminal_if_num++;
				                }
								else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
								{
									pstTerminalIfCb->intf_pos_id = stIntfFlow.posId;
									NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
								}
			                }
                        }
                        //����˿��Ѿ���VPWS/VPLS��
                        else
                        {

		                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
			                {
			                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
			                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

			                    /* ���ô���todo */
			                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
			                    {
			                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
			                    }

			                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

			                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
			                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

                                //stIntfFlow.flags |= INTF_COUNTER_ON;

								//���ʶ��
                                if ((pstLogicalPort->logic_l2_cfg_cb != NULL) 
                                 && (pstLogicalPort->logic_l2_cfg_cb->recv_action == ATG_DCI_TAGGED_VLAN))
                                {
                                	stIntfFlow.flags |= INTF_PTAG_IDENTIFY_ENABLE;
                                }

                                stIntfFlow.mCardPort = pstLogicalPort->port_id;      //����Ҫ�û�ȡ�˿ں����滻;//todo
                                stIntfFlow.eIntfType = INTF_TYPE_L2;
                                stIntfFlow.bindId = pstLogicalPort->logic_port_info_cb.vp_idx;

                                NBB_TRC_DETAIL((NBB_FORMAT "  stIntfFlow.bindId = %d", stIntfFlow.bindId));

			                    //������ :���ڶ˿�
			                    if ((pstTerminalIf->svlan_id == 0) && (pstTerminalIf->cvlan_id == 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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

			                    //���ڶ˿�+VLAN��Trunk��
			                    else if ((pstTerminalIf->svlan_id != 0) && (pstTerminalIf->cvlan_id == 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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

			                    //���ڶ˿�+QINQ
			                    else if ((pstTerminalIf->svlan_id != 0) && (pstTerminalIf->cvlan_id != 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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
			                        OS_PRINTF("***WARNNING***: PORT_INDEX=%ld ֻ������CVlan��δ����SVlan\n", ulPortIndexKey);
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

									//��������ڣ��������ڵ㣬�������У�������
					                if (NULL == pstTerminalIfCb)
					                {
					                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
					                	
						                //�������ݲ���������
						                pstTerminalIfCb->intf_pos_id = intf_pos_id_temp;
                                                          pstTerminalIfCb->intf2_pos_id = stIntfFlow.posId;
						                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
						                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

						                pstLogicalPort->terminal_if_num++;
					                }
									else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
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

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 ����ʧ��",
			                        		ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 ����ʧ��\n",
			                        		ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 ����ʧ��\n",
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

	            /* �����һ��entry�ĵ�ַ��*/
	            pucTerminalIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->vlan_data);

	            /* �׵�ַΪNULL���쳣 */
	            if (pucTerminalIfDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  pucTerminalIfDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {
	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  21��VLAN���� L2 (num = %d) DEL\n", pstSetLogicalPort->vlan_num);
	                }

	                OS_SPRINTF(ucMessage, "  21��VLAN���� L2 (num = %d) DEL\n", 
                           pstSetLogicalPort->vlan_num);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                    //����߼��˿ڴ��ڣ�����ɾ����������Ҫѭ�������Ѵ��ڵ���
                    if (ucIfExist == ATG_DCI_EXIST)
                    {
                        OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                        spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);
                        
                        //1������˿ڲ������κ�VPWS/VPLS
                        //2������Ѿ�����VPWS/VPLS�����˿ڲ����ڱ���λ��ֻɾ������
                        if ((pstLogicalPort->logic_port_info_cb.vp_idx == 0)
                            || ((pstLogicalPort->logic_port_info_cb.vp_idx != 0)
                                && (stPortInfo.slot_id != SHARED.local_slot_id)
                                && (stPortInfo.port_type != ATG_DCI_VE_PORT)
                                && (stPortInfo.port_type != ATG_DCI_LAG)))
                        {

                            //��ȡ����
		                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
			                {
			                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
			                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

			                    /* ���ô���todo */
			                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
			                    {
			                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
			                    }

			                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

			                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
			                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

				                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//��������ڣ��޷�ɾ��
				                if (NULL == pstTerminalIfCb)
				                {
			                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 �����ڣ��޷�ɾ��",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 �����ڣ��޷�ɾ��\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 �����ڣ��޷�ɾ��\n",
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

						            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
						            spm_free_terminal_if_cb(pstTerminalIfCb NBB_CCXT);
						            
									pstLogicalPort->terminal_if_num--;									
								}
			                    
                            }
                        }
                        //����˿��Ѿ���VPWS/VPLS�У�˵���Ѿ�������ɾ����Ӧ����
                        else
                        {

                            //��ȡ����
		                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
			                {
			                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
			                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

			                    /* ���ô���todo */
			                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
			                    {
			                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
			                    }

			                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

			                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
			                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

								pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//��������ڣ��޷�ɾ��
				                if (NULL == pstTerminalIfCb)
				                {
			                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 �����ڣ��޷�ɾ��",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 �����ڣ��޷�ɾ��\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 �����ڣ��޷�ɾ��\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
				                }
				                else	//����ɾ��
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

							            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
							            spm_free_terminal_if_cb(pstTerminalIfCb NBB_CCXT);
							            
										pstLogicalPort->terminal_if_num--;
									}
									else
									{
				                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

				                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3DelIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 ����ʧ��",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id));

				                        OS_PRINTF("***ERROR***:ApiC3DelIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 ɾ��ʧ��\n",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id);

				                        OS_SPRINTF(ucMessage,
				                            "***ERROR***:ApiC3DelIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L2 ɾ��ʧ��\n",
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

            /****************************** �����Diff-Serv����L2 ******************************/
            if (ulOperFlowDiffServ == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucFlowDiffServDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->flow_diff_serv_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucFlowDiffServDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucFlowDiffServDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                
                    OS_SPRINTF(ucMessage, "  13�������Diff-Serv����L2 ADD\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->flow_diff_serv_num; i++)
                    {
                        pstFlowDiffServData = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *)
                            (pucFlowDiffServDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV)))
                            * i);

                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            printf("  13�������Diff-Serv����L2 ADD\n");
                            spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                        }

                        //OS_SPRINTF(ucMessage, "  13�������Diff-Serv����L2 ADD\n");
                        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
                        //spm_dbg_record_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);

                        NBB_TRC_DETAIL((NBB_FORMAT "  Diff-Servģ������   = %d", pstFlowDiffServData->diff_serv_id));


                        //����߼��˿ڴ��ڣ������Ѽ��뵽VPN�����Խ�QoS
                        if (ucIfExist == ATG_DCI_EXIST)
                        {
                            OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                            spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);
                            stSubPort.unit = stPortInfo.unit_id;
                            stSubPort.port = stPortInfo.port_id;
                            stSubPort.slot = stPortInfo.slot_id;
                            stSubPort.ivlan = stPortInfo.cvlan;
                            stSubPort.ovlan = stPortInfo.svlan;

                            /* ���ô���todo */
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
                                        "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! ���� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��,posId=0!\n",
                                        ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan));

                                OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! ���� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��,posId=0!\n",
                                        ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan);
                                    
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                OS_SPRINTF(ucMessage,
                                    "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! ���� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��,posId=0!\n",
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
                                            "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! ���� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��",
                                            ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan));

                                    OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d ���� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��!\n",
                                        ulPortIndexKey,
                                        ret,
                                        pstFlowDiffServData->svlan, 
                                        pstFlowDiffServData->cvlan);
                                        
                                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                            "CALL_FUNC_ERROR", ucMessage));

                                    OS_SPRINTF(ucMessage,
                                        "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d ���� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��!\n",
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

                                    //��������ڣ��������ڵ㣬�������У�������1
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
                                        
                                        //�������ݲ���������
                                        //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
                                        //NBB_MEMCPY(&pstflowdiffservcb->key,&flowdiffservcfgkey,
                                            //sizeof(ATG_DCI_LOG_PORT_VLAN));
                                        NBB_MEMCPY(&(pstflowdiffservcb->flow_diff_serv_cfg),
                                            pstFlowDiffServData, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                                        AVLL_INSERT(pstLogicalPort->flow_diff_serv_tree, 
                                            pstflowdiffservcb->spm_flow_diff_serv_node);

                                        pstLogicalPort->flow_diff_serv_num++;
                                    }
                                    else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
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

                                /* �������� */
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
                                else    //�����ΪNULL��˵��������Ҫ����
                                {
                                    OS_MEMCPY(pstLogicalPort->flow_diff_serv_cfg_cb[usFlowId - 1],
                                        pstFlowDiffServData,
                                        sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
                                }
                            }
                            else
                            {
                                //NBB_TRC_DETAIL((NBB_FORMAT
                                        //"  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! �޸� flow_id=%d �������Diff-Serv����ʧ��",
                                        //ulPortIndexKey, usFlowId));

                                //OS_PRINTF(
                                   // "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d �޸�flow_id=%d �������Diff-Serv����ʧ��!\n",
                                   // ulPortIndexKey,
                                   // ret,
                                   // usFlowId);
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                //OS_SPRINTF(
                                    //ucMessage,
                                    //"***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d �޸�flow_id=%d �������Diff-Serv����ʧ��!\n",
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
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����Flow-Diff-Servʧ��",
                                ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����Flow-Diff-Servʧ��\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����Flow-Diff-Servʧ��\n",
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

                /* �����һ��entry�ĵ�ַ��*/
                pucFlowDiffServDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->flow_diff_serv_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucFlowDiffServDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucFlowDiffServDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                { 
                
                    OS_SPRINTF(ucMessage, "  13�������Diff-Serv����L2 DEL\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->flow_diff_serv_num; i++)
                    {
                        pstFlowDiffServData = (ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *)
                            (pucFlowDiffServDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV)))
                            * i);

                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            printf("  13�������Diff-Serv����L2 DEL\n");
                            spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                        }

                        //OS_SPRINTF(ucMessage, "  13�������Diff-Serv����L2 DEL\n");
                        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
                        //spm_dbg_record_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);

                        //NBB_TRC_DETAIL((NBB_FORMAT "  ��ID                = %d", pstFlowDiffServData[i]->flow_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  Diff-Servģ������   = %d", pstFlowDiffServData->diff_serv_id));

                        //usFlowId = pstFlowDiffServData[i]->flow_id;

                        //����߼��˿ڴ��ڣ������Ѽ��뵽VPN����������QoS
                        if (ucIfExist == ATG_DCI_EXIST)
                        {
                            OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                            spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);
                            stSubPort.unit = stPortInfo.unit_id;
                            stSubPort.port = stPortInfo.port_id;
                            stSubPort.slot = stPortInfo.slot_id;
                            stSubPort.ivlan = stPortInfo.cvlan;
                            stSubPort.ovlan = stPortInfo.svlan;

                            /* ���ô���todo */
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
                                        "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! ɾ�� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��",
                                        ulPortIndexKey, pstFlowDiffServData->svlan, pstFlowDiffServData->cvlan));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d ɾ�� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstFlowDiffServData->svlan, 
                                    pstFlowDiffServData->cvlan);
                                    
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d ɾ�� SVlan=%d CVlan=%d �������Diff-Serv����ʧ��!\n",
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

                                //��������ڣ��޷�ɾ��
                                if (NULL == pstflowdiffservcb)
                                {
                                    pstSetLogicalPort->flow_diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                    NBB_TRC_DETAIL((NBB_FORMAT 
                                    "  ***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��Flow-Diff-Serv���ò����ڣ��޷�ɾ��",
                                    ulPortIndexKey,
                                    pstFlowDiffServData->svlan,
                                    pstFlowDiffServData->cvlan));

                                    OS_PRINTF(
                                    "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��Flow-Diff-Serv���ò����ڣ��޷�ɾ��\n",
                                    ulPortIndexKey,
                                    pstFlowDiffServData->svlan,
                                    pstFlowDiffServData->cvlan);

                                    OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��Flow-Diff-Serv���ò����ڣ��޷�ɾ��\n",
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

                                    //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
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
                                //        "  ***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() error! ɾ�� flow_id=%d �������Diff-Serv����ʧ��",
                                //        ulPortIndexKey, usFlowId));

                                //OS_PRINTF(
                                //    "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d ɾ�� flow_id=%d �������Diff-Serv����ʧ��!\n",
                                //    ulPortIndexKey,
                                //    ret,
                                //    usFlowId);
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                //OS_SPRINTF(
                                //    ucMessage,
                                //    "***ERROR***:PORT_INDEX=%ld, spm_ds_add_logic_flow_node() ret=%d ɾ�� flow_id=%d �������Diff-Serv����ʧ��!\n",
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
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��Flow-Diff-Servʧ��",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��Flow-Diff-Servʧ��\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��Flow-Diff-Servʧ��\n",
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

            /****************************** ������ϻ��û�QOS��������L2 ******************************/
            if (ulOperFlowUpUserQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucFlowUpUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->flow_upuser_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucFlowUpUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucFlowUpUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    OS_SPRINTF(ucMessage, "  19��������ϻ��û�QOS��������L2\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->flow_upuser_qos_num; i++)
	                {
                        pstFlowUpUserQosData = (ATG_DCI_LOG_FLOW_UP_USER_QOS *)
                            (pucFlowUpUserQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_FLOW_UP_USER_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  19��������ϻ��û�QOS��������L2\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    //NBB_TRC_DETAIL((NBB_FORMAT "  ��ID                = %d", pstFlowUpUserQosData->flow_id));
	                    NBB_TRC_DETAIL((NBB_FORMAT "  QOS����ģ������     = %d", pstFlowUpUserQosData->qos_policy_index));

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
                                    "  ***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() error! ���� SVlan=%d CVlan=%d ��������ϻ��û�QOS��������ʧ�ܣ�posId=0!\n",
                                    ulPortIndexKey, pstFlowUpUserQosData->svlan, pstFlowUpUserQosData->cvlan));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() ���� SVlan=%d CVlan=%d ��������ϻ��û�QOS��������ʧ�ܣ�posId=0!\n",
                                ulPortIndexKey,
                                pstFlowUpUserQosData->svlan, 
                                pstFlowUpUserQosData->cvlan);
                                
                            NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                    "CALL_FUNC_ERROR", ucMessage));

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node()���� SVlan=%d CVlan=%d ��������ϻ��û�QOS��������ʧ�ܣ�posId=0!\n",
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
                                        "  ***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() error! ���� SVlan=%d CVlan=%d ��������ϻ��û�QOS��������ʧ��",
                                        ulPortIndexKey, pstFlowUpUserQosData->svlan, pstFlowUpUserQosData->cvlan));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() ret=%d ���� SVlan=%d CVlan=%d ��������ϻ��û�QOS��������ʧ��!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstFlowUpUserQosData->svlan, 
                                    pstFlowUpUserQosData->cvlan);
                                    
                                NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                        "CALL_FUNC_ERROR", ucMessage));

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld, spm_add_flow_up_usr_qos_node() ret=%d ���� SVlan=%d CVlan=%d ��������ϻ��û�QOS��������ʧ��!\n",
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

            /****************************** �ϻ�����������QOS��������L2 ******************************/
            if (ulOperInclassifyQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucInclassifyQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->inclassify_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucInclassifyQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucInclassifyQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  14)�ϻ�����������QOS��������L2 ADD\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);    
                    
                    for (i = 0; i < pstSetLogicalPort->inclassify_qos_num; i++)
	                {
                        pstInclassifyQosData = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)
                            (pucInclassifyQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  14)�ϻ�����������QOS��������L2 ADD\n");
                            
                               //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                        spm_dbg_print_logical_port_inclassify_qos_cfg(pstInclassifyQosData);
	                    }
                        
                           //OS_SPRINTF(ucMessage, "  14)�ϻ�����������QOS��������L2 ADD\n");
                           //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                            
	                    NBB_TRC_DETAIL((NBB_FORMAT "  ����������QOS����ģ������ = %d",
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
                                    "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��,posId=0!\n",
                                    ulPortIndexKey, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

                                    OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��,posId=0!\n",
                                    ulPortIndexKey, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

                                    OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��,posId=0!\n",
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
                                        "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��!\n",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

                                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��!\n",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

                                        OS_SPRINTF(ucMessage,
                                        "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��!\n",
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
                                        
                                        //��������ڣ��������ڵ㣬�������У�������1
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

                                            //�������ݲ���������
                                            //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
                                            //NBB_MEMCPY(&pstinclassifyqoscb->key,&inclassifyqoscfgkey,
                                                //sizeof(ATG_DCI_LOG_PORT_VLAN));
                                            NBB_MEMCPY(&(pstinclassifyqoscb->inclassify_qos_cfg),
                                            pstInclassifyQosData, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
                                            AVLL_INSERT(pstLogicalPort->inclassify_qos_tree, 
                                                pstinclassifyqoscb->spm_inclassify_qos_node);

                                            pstLogicalPort->inclassify_qos_num++;
                                        }
                                        else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
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
                                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����inclassify_qosʧ��",
                                    ulPortIndexKey));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����inclassify_qosʧ��\n", ulPortIndexKey);

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����inclassify_qosʧ��\n",
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

                /* �����һ��entry�ĵ�ַ��*/
                pucInclassifyQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->inclassify_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucInclassifyQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucInclassifyQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    OS_SPRINTF(ucMessage, "  14) �ϻ�����������QOS��������L2 DEL\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->inclassify_qos_num; i++)
	                {
                        pstInclassifyQosData = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)
                            (pucInclassifyQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf(" 14) �ϻ�����������QOS��������L2 DEL\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                        spm_dbg_print_logical_port_inclassify_qos_cfg(pstInclassifyQosData);
	                    }
                        
                           //OS_SPRINTF(ucMessage, "  14) �ϻ�����������QOS��������L2 DEL\n");
                           //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                           
	                    NBB_TRC_DETAIL((NBB_FORMAT "  ����������QOS����ģ������ = %d",
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
                                    "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��,posId=0!\n",
                                    ulPortIndexKey,pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

                                    OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��,posId=0!\n",
                                    ulPortIndexKey,pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

                                    OS_SPRINTF(ucMessage,
                                    "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node()ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��,posId=0!\n",
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
                                        "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

                                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��\n",
                                        ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

                                        OS_SPRINTF(ucMessage,
                                        "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��\n",
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
                                        
                                        //��������ڣ��޷�ɾ��
                                        if (NULL == pstinclassifyqoscb)
                                        {
                                            pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                            NBB_TRC_DETAIL((NBB_FORMAT 
                                            "  ***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��inclassify_qos���ò����ڣ��޷�ɾ��",
                                            ulPortIndexKey,
                                            pstInclassifyQosData->svlan,
                                            pstInclassifyQosData->cvlan));

                                            OS_PRINTF(
                                            "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��inclassify_qos���ò����ڣ��޷�ɾ��\n",
                                            ulPortIndexKey,
                                            pstInclassifyQosData->svlan,
                                            pstInclassifyQosData->cvlan);

                                            OS_SPRINTF(ucMessage,
                                            "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��inclassify_qos���ò����ڣ��޷�ɾ��\n",
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

                                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                                            spm_free_inclassify_qos_cb(pstinclassifyqoscb NBB_CCXT);

                                            pstLogicalPort->inclassify_qos_num--;									
                                        }
                                    }                                      
                                }

                            } 
                            else
                            {
                                pstSetLogicalPort->inclassify_qos_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��inclassify_qosʧ��",
                                        ulPortIndexKey));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��inclassify_qosʧ��\n", ulPortIndexKey);

                                OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��inclassify_qosʧ��\n",
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

            /****************************** �»��û���QOS����L2 ******************************/
            if (ulOperDownGroupQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucDownGroupQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->down_user_group_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucDownGroupQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDownGroupQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDownGroupQosData = (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)pucDownGroupQosDataStart;
                    
                    OS_SPRINTF(ucMessage, "  18���»��û���QOS����L2\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  18���»��û���QOS����L2\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  �û�������  = %d", pstDownGroupQosData->user_group_index));

                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                    ret = spm_hqos_add_group_node(stPortInfo.slot_id,
                        stPortInfo.port_id,
                        pstDownGroupQosData->user_group_index,
                        ulPortIndexKey
                        NBB_CCXT);

                    if (ret == SUCCESS)
                    {
                        /* �������� */
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

            /****************************** �»��û�����QOS��������L2 ******************************/
            if (ulOperDownUserQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucDownUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->down_user_queue_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucDownUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDownUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDownUserQosData = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)pucDownUserQosDataStart;
                    OS_SPRINTF(ucMessage, "  17���»��û�����QOS��������L2\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  17���»��û�����QOS��������L2\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  NodeIndex  = %d", pstDownUserQosData->node_index));
                    NBB_TRC_DETAIL((NBB_FORMAT "  �û�QOS����ģ������  = %d",
                            pstDownUserQosData->user_qos_policy_index));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ���ȼ�����QOS����ģ������  = %d",
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
                            /* �������� */
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
                                "  ***ERROR***:PORT_INDEX=%ld ���»��û���QOS����δ�£������»��û�����QOS����ʧ��",
                                ulPortIndexKey));

                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld ���»��û���QOS����δ�£������»��û�����QOS����ʧ��\n",
                            ulPortIndexKey);

                        OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld ���»��û���QOS����δ�£������»��û�����QOS����ʧ��\n",
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

            /****************************** ��������traffic_filter L2 ******************************/
            if (ulOperTrafficFilter == ATG_DCI_OPER_ADD)
			{

                /* �����һ��entry�ĵ�ַ��*/
                pucTrafficFilterDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->traffic_filter_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucTrafficFilterDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucTrafficFilterDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    //if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    //{
                        //printf("  20����������traffic_filter L2 ADD\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    //}
                    OS_SPRINTF(ucMessage, "  20����������traffic_filter L2 ADD \n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->traffic_filter_num; i++)
                    {
                        pstTrafficFilter = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)
                            (pucTrafficFilterDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER)))
                            * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            //spm_dbg_print_logical_port_ipv4_cfg(pstIpv4Data[i]);
                            printf("  20����������traffic_filter L2 ADD \n");

                            spm_dbg_print_logical_port_traffic_filter_cfg(pstTrafficFilter);                            
                        }
                        
                        //OS_SPRINTF(ucMessage, "  20����������traffic_filter L2 ADD\\n");
                        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
                        NBB_TRC_DETAIL((NBB_FORMAT "  acl_id  = %d", pstTrafficFilter->acl_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ����direction  = %d", pstTrafficFilter->direction));

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

                                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter L2����ʧ��!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter L2����ʧ��!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan);

                                OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter L2����ʧ��!\n",
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
                                
                                //��������ڣ��������ڵ㣬�������У�������1
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

                                    //�������ݲ���������
                                    //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
                                    //NBB_MEMCPY(&psttrafficfiltercb->key,
                                        //&trafficfiltercfgkey,sizeof(ATG_DCI_LOG_PORT_VLAN));
                                    NBB_MEMCPY(&(psttrafficfiltercb->traffic_filter_cfg),
                                    pstTrafficFilter, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
                                    AVLL_INSERT(pstLogicalPort->traffic_filter_tree, 
                                        psttrafficfiltercb->spm_traffic_filter_node);

                                    pstLogicalPort->traffic_filter_num++;
                                }
                                else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
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
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����traffic_filterʧ��",
                            ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����traffic_filterʧ��\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�����traffic_filterʧ��\n",
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
                                    /* �������� */
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

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��\n",
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

                /* �����һ��entry�ĵ�ַ��*/
                pucTrafficFilterDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->traffic_filter_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucTrafficFilterDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucTrafficFilterDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    //if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    //{
                        //printf("  20����������traffic_filter L2 DEL\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    //}
                    
                    OS_SPRINTF(ucMessage, "  20����������traffic_filter L2 DEL\\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->traffic_filter_num; i++)
                    {
                        pstTrafficFilter = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)
                            (pucTrafficFilterDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER)))
                            * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            //spm_dbg_print_logical_port_ipv4_cfg(pstIpv4Data[i]);
                            printf("  20����������traffic_filter L2 DEL\n");
                            spm_dbg_print_logical_port_traffic_filter_cfg(pstTrafficFilter);
                        }
                        
                        //OS_SPRINTF(ucMessage, "  20����������traffic_filter L2 DEL\\n");
                        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
                        NBB_TRC_DETAIL((NBB_FORMAT "  acl_id  = %d", pstTrafficFilter->acl_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ����direction  = %d", pstTrafficFilter->direction));
                        
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
                                "ɾ��SVlan=%d CVlan=%d �İ�������traffic_filter L2����ʧ��!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan));

                                OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d"
                                "ɾ��SVlan=%d CVlan=%d �İ�������traffic_filter L2����ʧ��!\n",
                                ulPortIndexKey,
                                ret,
                                pstTrafficFilter->svlan, 
                                pstTrafficFilter->cvlan);

                                OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d"
                                "ɾ��SVlan=%d CVlan=%d �İ�������traffic_filter L2����ʧ��!\n",
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

                                //��������ڣ��޷�ɾ��
                                if (NULL == psttrafficfiltercb)
                                {
                                    pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                    NBB_TRC_DETAIL((NBB_FORMAT 
                                    "  ***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��traffic_filter���ò����ڣ��޷�ɾ��",
                                    ulPortIndexKey,
                                    pstTrafficFilter->svlan,
                                    pstTrafficFilter->cvlan));

                                    OS_PRINTF(
                                    "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��traffic_filter���ò����ڣ��޷�ɾ��\n",
                                    ulPortIndexKey,
                                    pstTrafficFilter->svlan,
                                    pstTrafficFilter->cvlan);

                                    OS_SPRINTF(ucMessage,
                                    "***ERROR***:PORT_INDEX=%ld SVLAN=%d CVLAN=%d ��traffic_filter���ò����ڣ��޷�ɾ��\n",
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

                                    //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                                    spm_free_traffic_filter_cb(psttrafficfiltercb NBB_CCXT);

                                    pstLogicalPort->traffic_filter_num--;									
                                }
                            }                        
                        }
                        else
                        {
                            pstSetLogicalPort->traffic_filter_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��traffic_filterʧ��",
                            ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��traffic_filterʧ��\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld ���߼�����δ�£�ɾ��traffic_filterʧ��\n",
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
                                    /* �������� */
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

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��\n",
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
            
            /****************************** Diff-Serv����L2��VP�»�UNI�ࣩ ******************************/
            if (ulOperDsL2 == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucDsL2DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ds_l2_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucDsL2DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDsL2DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDsL2Data = (ATG_DCI_LOG_PORT_DS_L2_DATA *)pucDsL2DataStart;
                    OS_SPRINTF(ucMessage, "  23��Diff-Serv����L2��VP�»�UNI�ࣩ\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  23��Diff-Serv����L2��VP�»�UNI�ࣩ\n");

                        spm_dbg_print_logical_port_ds_l2_cfg(pstDsL2Data);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  Diff-Servģ������ = %d", pstDsL2Data->ds_id));

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
                        /* �������� */
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
        /* L3�˿�                                                                   */
        /****************************************************************************/
        else if ((pstLogicalPort->basic_cfg_cb != NULL)
            && (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L3))
        {
            /****************************** �߼�����L3 ******************************/
            if (ulOperLogicL3 == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucLogicL3DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->logical_l3_data);

                /* �׵�ַΪNULL���쳣 */
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
                        printf("  3���߼�����L3\n");
                        spm_dbg_print_logical_port_logic_l3_cfg(pstLogicL3Data);
                    }

                    OS_SPRINTF(ucMessage, "  3���߼�����L3\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    //spm_dbg_record_logical_port_logic_l3_cfg(pstLogicL3Data);

                    //NBB_TRC_DETAIL((NBB_FORMAT "  CONTROL VID = %d", pstLogicL3Data->control_vid));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IPV4 URPFʹ�� = %d", pstLogicL3Data->ipv4_urpf_mode));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IPV4 ȱʡ·�ɼ�� = %s",
                            pstLogicL3Data->ipv4_default_check ? "ʹ��" : "��ʹ��"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IPV6 URPFʹ�� = %d", pstLogicL3Data->ipv6_urpf_mode));
                    NBB_TRC_DETAIL((NBB_FORMAT "  IPV6 ȱʡ·�ɼ�� = %s",
                            pstLogicL3Data->ipv6_default_check ? "ʹ��" : "��ʹ��"));
                    NBB_TRC_DETAIL((NBB_FORMAT "  VRF ID = %d", pstLogicL3Data->vrf_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  INGRESS = %s", spm_set_ulong_to_ipv4(pstLogicL3Data->ingress  NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  EGRESS = %s", spm_set_ulong_to_ipv4(pstLogicL3Data->egress NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  TUNNEL_ID = %ld", pstLogicL3Data->tunnel_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  MPLSת��ʹ�� = %s", pstLogicL3Data->mpls_forward_enable ? "ʹ��" : "��ʹ��"));  

                    stIntfL3.mCardPort = pstLogicalPort->port_id;
                    stIntfL3.bindId = pstLogicL3Data->vrf_id;
                    stIntfL3.eIntfType = INTF_TYPE_L3;

                    #if 0
                    //IPV4 URPFʹ��
                    if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFȱʡ���
                    {
                        stIntfL3.eIpv4UrpfMode = URPF_OFF;
                    }
                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPF�ϸ���
                    {
                        stIntfL3.eIpv4UrpfMode = URPF_STRICT;
                    }
                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPF��ɢ���
                    {
                        stIntfL3.eIpv4UrpfMode = URPF_LOOSE;
                    }

					//IPV6 URPFʹ��
                    if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFȱʡ���
                    {
                        stIntfL3.eIpv6UrpfMode = URPF_OFF;
                    }
                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPF�ϸ���
                    {
                        stIntfL3.eIpv6UrpfMode = URPF_STRICT;
                    }
                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPF��ɢ���
                    {
                        stIntfL3.eIpv6UrpfMode = URPF_LOOSE;
                    }

                    //ȱʡ·�ɼ��ʹ��
                    if (pstLogicL3Data->ipv4_default_check == ENABLE)
                    {
                        stIntfL3.flags |= INTF_V4_ALLOW_DEFAULT_ROUTE;
                    }

                    if (pstLogicL3Data->ipv6_default_check == ENABLE)
                    {
                        stIntfL3.flags |= INTF_V6_ALLOW_DEFAULT_ROUTE;
                    }
                    
                    //MPLSת����ʹ��
                    if (pstLogicL3Data->mpls_forward_enable == DISABLE)
                    {
                        stIntfL3.flags |= INTF_MPLS_DISABLE;
                    }
                    
                    //stIntfL3.flags |= INTF_COUNTER_ON;
                    #endif

					//���Vlan���Ը���������0��˵��������intf����Ҫɾ�����ؽ�
					if (pstLogicalPort->terminal_if_num != 0)
					{
					    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
					         pstTerminalIfCb != NULL;
					         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
					                       pstTerminalIfCb->spm_terminal_if_node))
					    {
					    	if (pstTerminalIfCb->intf_pos_id != 0)
					    	{

								//ע�͵����������L3����ʱ ɾ�� �� ���� INTF ��QOS����û���ˣ��ĳ�Mod INTF
					    		//ApiC3DelIntf(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id);

					    		//almpm_delline_intf(ulPortIndexKey, pstTerminalIfCb->intf_pos_id, 1);

                                //���ڵ���ModIntf�ӿڻ���qos���ã���Ϊ�������ø����ֶε��ӽӿڡ
                                
			                    //ApiC3SetIntfVrf(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,pstLogicL3Data->vrf_id); //to do

                                stIntfL3.posId = pstTerminalIfCb->intf_pos_id;

                                #ifdef SPU

                                //ˢ��vrfid
                                ret = ApiC3SetIntfBindId(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id, INTF_TYPE_L3, pstLogicL3Data->vrf_id);
                                if(SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfBindId() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3SetIntfBindId() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            	"***ERROR***:ApiC3SetIntfBindId() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
                                }

                                //IPV4 URPFʹ��
			                    if (pstLogicL3Data->ipv4_default_check == ENABLE)
                                {
                                    if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFȱʡ���
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_OFF,ENABLE);
                                        
                                    }
                                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPF�ϸ���
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_STRICT,ENABLE);
                                       
                                    }
                                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPF��ɢ���
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_LOOSE,ENABLE);
                                    } 
                                }    

                                else if(pstLogicL3Data->ipv4_default_check == DISABLE)
                                {
                                    if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFȱʡ���
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_OFF,DISABLE);
                                        
                                    }
                                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPF�ϸ���
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_STRICT,DISABLE);
                                       
                                    }
                                    else if (pstLogicL3Data->ipv4_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPF��ɢ���
                                    {
                                        ret = ApiC3SetIntfV4UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_LOOSE,DISABLE);
                                    } 
                                }    

                                if(SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfV4UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3SetIntfV4UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��\n",
											ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIfCb->terminal_if_cfg.svlan_id,
			                        		pstTerminalIfCb->terminal_if_cfg.cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            	"***ERROR***:ApiC3SetIntfV4UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��\n",
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
                                    if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFȱʡ���
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_OFF,ENABLE);
                                    }
                                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPF�ϸ���
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_STRICT,ENABLE);
                                    }
                                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPF��ɢ���
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_LOOSE,ENABLE);
                                    }
                                }

                                else if (pstLogicL3Data->ipv6_default_check == DISABLE)
                                {
                                    if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFȱʡ���
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_OFF,DISABLE);
                                    }
                                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPF�ϸ���
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_STRICT,DISABLE);
                                    }
                                    else if (pstLogicL3Data->ipv6_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPF��ɢ���
                                    {
                                        ret = ApiC3SetIntfV6UrpfMode(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,URPF_LOOSE,DISABLE);
                                    }
                                }

                                if(SUCCESS != ret)
                                {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfV6UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��",
                                                ret,
                                                ulPortIndexKey,
                                                pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                                pstTerminalIfCb->terminal_if_cfg.cvlan_id));

                                        OS_PRINTF("***ERROR***:ApiC3SetIntfV6UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��\n",
                                                ret,
                                                ulPortIndexKey,
                                                pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                                pstTerminalIfCb->terminal_if_cfg.cvlan_id);

                                        OS_SPRINTF(ucMessage,
                                                "***ERROR***:ApiC3SetIntfV6UrpfMode() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��\n",
                                                ret,
                                                ulPortIndexKey,
                                                pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                                pstTerminalIfCb->terminal_if_cfg.cvlan_id);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
                                                "CALL_FUNC_ERROR", ucMessage));
                                    
                                }
                                
                                //MPLSת����ʹ��
                                if (pstLogicL3Data->mpls_forward_enable == DISABLE)
                                {
                                    ApiC3SetIntfMplsDisable(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,ENABLE);//ʹ��:0/1-enable /disable
                                }

                                else if(pstLogicL3Data->mpls_forward_enable == ENABLE)
                                {
                                    ApiC3SetIntfMplsDisable(pstLogicalPort->unit_id,pstTerminalIfCb->intf_pos_id,DISABLE);//ʹ��:0/1-enable /disable
                                }

                                if(SUCCESS != ret)
                                {
                                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetIntfMplsDisable() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��",
                                            ret,
                                            ulPortIndexKey,
                                            pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                            pstTerminalIfCb->terminal_if_cfg.cvlan_id));

                                    OS_PRINTF("***ERROR***:ApiC3SetIntfMplsDisable() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��\n",
                                            ret,
                                            ulPortIndexKey,
                                            pstTerminalIfCb->terminal_if_cfg.svlan_id,
                                            pstTerminalIfCb->terminal_if_cfg.cvlan_id);

                                    OS_SPRINTF(ucMessage,
                                            "***ERROR***:ApiC3SetIntfMplsDisable() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� �߼�����L3 ˢ��ʧ��\n",
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
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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

			                    //Dot1q�ս��ӽӿ�Ӧ����L3VPN��SVLAN��Ч��CVLAN��Ч
			                    else if ((pstTerminalIfCb->terminal_if_cfg.svlan_id != 0) 
			                          && (pstTerminalIfCb->terminal_if_cfg.cvlan_id == 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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

			                    //QinQ�ս��ӽӿ�Ӧ����L3VPN��SVLAN��CVLANͬʱ��Ч
			                    else if ((pstTerminalIfCb->terminal_if_cfg.svlan_id != 0) 
			                          && (pstTerminalIfCb->terminal_if_cfg.cvlan_id != 0))
			                    {
									if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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
			                        OS_PRINTF("***WARNNING***: PORT_INDEX=%ld ֻ������CVlan��δ����SVlan\n", ulPortIndexKey);
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
									//���������ӿ�(���ӿ�)�����ж��Ƿ�������IP��ֻ��������IP���ʹ��
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

                    /* �������� */
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

            /****************************** Ipv4��ַ����L3 ******************************/
            if (ulOperIpv4 == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucIpv4DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ipv4_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucIpv4DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv4DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  7) Ipv4��ַ����L3 (ipv4_num = %d) ADD\n", pstSetLogicalPort->ipv4_num);
                    }

                    OS_SPRINTF(ucMessage, "  7) Ipv4��ַ����L3 (ipv4_num = %d) ADD\n", 
                        pstSetLogicalPort->ipv4_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->ipv4_num; i++)
                    {
                        pstIpv4Data[i] = (ATG_DCI_LOG_PORT_IPV4_DATA *)
                            (pucIpv4DataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))) * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_ipv4_cfg(pstIpv4Data[i]);
                        }

                        //spm_dbg_record_logical_port_ipv4_cfg(pstIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IP��ַǰ׺ = %s", spm_set_ulong_to_ipv4(pstIpv4Data[i]->ip_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", pstIpv4Data[i]->mask_len));

                        //���ipv4�Ƿ����ù�
                        iCfgPos = spm_check_if_ipv4_exist(ulPortIndexKey, pstIpv4Data[i] NBB_CCXT);

                        //���ظ�
                        if (iCfgPos <= 0)
                        {
                            for (j = 0; j < ATG_DCI_LOG_PORT_IPV4_NUM; j++)
                            {
                                //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
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
                        else    //���ظ�����������
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
							//���������ӿ�(���ӿ�)�����ж��Ƿ�������IP��ֻ��������IP���ʹ��
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

                /* �����һ��entry�ĵ�ַ��*/
                pucIpv4DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ipv4_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucIpv4DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv4DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  7) Ipv4��ַ����L3 (ipv4_num = %d) DEL\n", pstSetLogicalPort->ipv4_num);
                    }

                    OS_SPRINTF(ucMessage, "  7) Ipv4��ַ����L3 (ipv4_num = %d) DEL\n", 
                        pstSetLogicalPort->ipv4_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->ipv4_num; i++)
                    {
                        pstIpv4Data[i] = (ATG_DCI_LOG_PORT_IPV4_DATA *)
                            (pucIpv4DataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV4_DATA))) * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_ipv4_cfg(pstIpv4Data[i]);
                        }

                        //spm_dbg_record_logical_port_ipv4_cfg(pstIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IP��ַǰ׺ = %s", spm_set_ulong_to_ipv4(pstIpv4Data[i]->ip_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", pstIpv4Data[i]->mask_len));

                        for (j = 0; j < ATG_DCI_LOG_PORT_IPV4_NUM; j++)
                        {
                            //�����ΪNULL��˵�����λ�������ݣ�����ɾ������
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
							//���������ӿ�(���ӿ�)�����ж��Ƿ�������IP��ֻ��������IP���ʹ��
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

            /****************************** IPv6��ַ����L3 ******************************/
            if (ulOperIpv6 == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucIpv6DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ipv6_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucIpv6DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  8) Ipv6��ַ����L3 (ipv6_num = %d) ADD\n", pstSetLogicalPort->ipv6_num);
                    }

                    OS_SPRINTF(ucMessage, "  8) Ipv6��ַ����L3 (ipv6_num = %d) ADD\n", 
                        pstSetLogicalPort->ipv6_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->ipv6_num; i++)
                    {
                        pstIpv6Data[i] = (ATG_DCI_LOG_PORT_IPV6_DATA *)
                            (pucIpv6DataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))) * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_ipv6_cfg(pstIpv6Data[i]);
                        }

                        //spm_dbg_record_logical_port_ipv6_cfg(pstIpv6Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV6��ַǰ׺ = %x-%x-%x-%x", pstIpv6Data[i]->ip_addr[0],
                                pstIpv6Data[i]->ip_addr[1],
                                pstIpv6Data[i]->ip_addr[2],
                                pstIpv6Data[i]->ip_addr[3]));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", pstIpv6Data[i]->mask_len));

                        //���ipv6�Ƿ����ù�
                        iCfgPos = spm_check_if_ipv6_exist(ulPortIndexKey, pstIpv6Data[i] NBB_CCXT);

                        //���ظ�
                        if (iCfgPos <= 0)
                        {
                            for (j = 0; j < ATG_DCI_LOG_PORT_IPV6_NUM; j++)
                            {
                                //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
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
							//���������ӿ�(���ӿ�)�����ж��Ƿ�������IP��ֻ��������IP���ʹ��
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

                /* �����һ��entry�ĵ�ַ��*/
                pucIpv6DataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->ipv6_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucIpv6DataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6DataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  8) Ipv6��ַ����L3 (ipv6_num = %d) DEL\n", pstSetLogicalPort->ipv6_num);
                    }

                    OS_SPRINTF(ucMessage, "  8) Ipv6��ַ����L3 (ipv6_num = %d) DEL\n", 
                        pstSetLogicalPort->ipv6_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->ipv6_num; i++)
                    {
                        pstIpv6Data[i] = (ATG_DCI_LOG_PORT_IPV6_DATA *)
                            (pucIpv6DataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_IPV6_DATA))) * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_ipv6_cfg(pstIpv6Data[i]);
                        }

                        //spm_dbg_record_logical_port_ipv6_cfg(pstIpv6Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV6��ַǰ׺ = %x-%x-%x-%x", pstIpv6Data[i]->ip_addr[0],
                                pstIpv6Data[i]->ip_addr[1],
                                pstIpv6Data[i]->ip_addr[2],
                                pstIpv6Data[i]->ip_addr[3]));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", pstIpv6Data[i]->mask_len));

                        for (j = 0; j < ATG_DCI_LOG_PORT_IPV6_NUM; j++)
                        {
                            //�����ΪNULL��˵�����λ�������ݣ�����ɾ������
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
							//���������ӿ�(���ӿ�)�����ж��Ƿ�������IP��ֻ��������IP���ʹ��
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

            /****************************** �鲥���ַL3 ******************************/
            if (ulOperMcIpv4 == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucMcMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->mc_ipv4_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucMcMacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucMcMacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  9) �鲥���ַL3 (mc_ipv4_num = %d) ADD\n", pstSetLogicalPort->mc_ipv4_num);
                    }

                    OS_SPRINTF(ucMessage, "  9) �鲥���ַL3 (mc_ipv4_num = %d) ADD\n", 
                        pstSetLogicalPort->mc_ipv4_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->mc_ipv4_num; i++)
                    {
                        pstMcIpv4Data[i] = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)
                            (pucMcMacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))) * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);
                        }

                        //spm_dbg_record_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV4�鲥��ַ = %s",
                                spm_set_ulong_to_ipv4(pstMcIpv4Data[i]->mc_ipv4_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", pstMcIpv4Data[i]->mask_len));

						//����dk�ӿ�
						if ((pstLogicalPort->logic_l3_cfg_cb != NULL) && (pstLogicalPort->basic_cfg_cb != NULL))
						{
							//�����VE�ڣ����������ṩ���㷨: x*2 + 1 + 0x800�������port_id��������
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
	                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£��鲥���ַL3ʧ��",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£��鲥���ַL3ʧ��\n", ulPortIndexKey);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£��鲥���ַL3ʧ��\n",
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
                                //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
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

                /* �����һ��entry�ĵ�ַ��*/
                pucMcMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->mc_ipv4_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucMcMacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucMcMacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  9) �鲥���ַL3 (mc_ipv4_num = %d) DEL\n", pstSetLogicalPort->mc_ipv4_num);
                    }

                    OS_SPRINTF(ucMessage, "  9) �鲥���ַL3 (mc_ipv4_num = %d) DEL\n", 
                        pstSetLogicalPort->mc_ipv4_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->mc_ipv4_num; i++)
                    {
                        pstMcIpv4Data[i] = (ATG_DCI_LOG_PORT_MC_IPV4_DATA *)
                            (pucMcMacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA))) * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);
                        }

                        //spm_dbg_record_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV4�鲥��ַ = %s",
                                spm_set_ulong_to_ipv4(pstMcIpv4Data[i]->mc_ipv4_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", pstMcIpv4Data[i]->mask_len));

                        for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV4_NUM; j++)
                        {
                            //�����ΪNULL��˵�����λ�������ݣ�����ɾ������
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

	    	/****************************** IPV6�鲥���ַL3 ******************************/
            if (ulOperMcIpv6 == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucIpv6McMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->mc_ipv6_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucIpv6McMacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6McMacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  22) IPV6�鲥���ַL3 (mc_ipv6_num = %d) ADD\n", pstSetLogicalPort->mc_ipv6_num);
                    }

                    OS_SPRINTF(ucMessage, "  22) IPV6�鲥���ַL3 (mc_ipv6_num = %d) ADD\n", 
                        pstSetLogicalPort->mc_ipv6_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->mc_ipv6_num; i++)
                    {
                        pstMcIpv6Data[i] = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)
                            (pucIpv6McMacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))) * i);

                        /* ���ô���todo */

						if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_mc_ipv6_cfg(pstMcIpv6Data[i]);
                        }

                        //spm_dbg_record_logical_port_mc_ipv6_cfg(pstMcIpv6Data[i]);
						
                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV6�鲥���ַ = %x-%x-%x-%x", pstMcIpv6Data[i]->mc_ipv6_addr[0],
                                pstMcIpv6Data[i]->mc_ipv6_addr[1],
                                pstMcIpv6Data[i]->mc_ipv6_addr[2],
                                pstMcIpv6Data[i]->mc_ipv6_addr[3]));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ǰ׺���� = %d", pstMcIpv6Data[i]->mask_len));

						//����dk�ӿ�
						if ((pstLogicalPort->logic_l3_cfg_cb != NULL) && (pstLogicalPort->basic_cfg_cb != NULL))
						{
							//�����VE�ڣ����������ṩ���㷨: x*2 + 1 + 0x800�������port_id��������
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
	                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�IPV6�鲥���ַL3ʧ��",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�IPV6�鲥���ַL3ʧ��\n", ulPortIndexKey);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�IPV6�鲥���ַL3ʧ��\n",
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
                                //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
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

                /* �����һ��entry�ĵ�ַ��*/
                pucIpv6McMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->mc_ipv6_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucIpv6McMacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucIpv6McMacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  22) IPV6�鲥���ַL3 (mc_ipv6_num = %d) DEL\n", pstSetLogicalPort->mc_ipv6_num);
                    }

                    OS_SPRINTF(ucMessage, "  22) IPV6�鲥���ַL3 (mc_ipv6_num = %d) DEL\n", 
                        pstSetLogicalPort->mc_ipv6_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->mc_ipv6_num; i++)
                    {
                        pstMcIpv6Data[i] = (ATG_DCI_LOG_PORT_MC_IPV6_DATA *)
                            (pucIpv6McMacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA))) * i);

                        /* ���ô���todo */
						#if 0
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);
                        }

                        spm_dbg_record_logical_port_mc_ipv4_cfg(pstMcIpv4Data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  IPV4�鲥��ַ = %s",
                                spm_set_ulong_to_ipv4(pstMcIpv4Data[i]->mc_ipv4_addr)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", pstMcIpv4Data[i]->mask_len));
						#endif

                        for (j = 0; j < ATG_DCI_LOG_PORT_MC_IPV6_NUM; j++)
                        {
                            //�����ΪNULL��˵�����λ�������ݣ�����ɾ������
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

            /****************************** VRRP����MAC��IP��ַ����L3 ******************************/
            if (ulOperVipVmac == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucVipVmacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->vip_vmac_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucVipVmacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucVipVmacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  10) ����MAC��IP��ַ����L3 (vip_vmac_num = %d) ADD\n",
                            pstSetLogicalPort->vip_vmac_num);
                    }

                    OS_SPRINTF(ucMessage,
                        "  10) ����MAC��IP��ַ����L3 (vip_vmac_num = %d) ADD\n",
                        pstSetLogicalPort->vip_vmac_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    //�ҵ�vlan���Ե�һ������
                    pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);

                    //û��vlan���ԣ��޷��󶨣���ӡ����ֱ���˳�
                    if(NULL == pstTerminalIfCb)
                    {
                        //�˴��·�������VRRP���þ�����ʧ��
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld �� �߼��˿�VLAN���Բ����ڣ��޷�����VRRP!\n",
			                        		ulPortIndexKey));

                        OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld �� �߼��˿�VLAN���Բ����ڣ��޷�����VRRP!\n",
			                        		ulPortIndexKey);

                        OS_SPRINTF(ucMessage,	"  ***ERROR***:PORT_INDEX=%ld �� �߼��˿�VLAN���Բ����ڣ��޷�����VRRP!\n",
			                            	ulPortIndexKey);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, BASIC_CONFIG_NOT_EXIST,
			                                "BASIC_CONFIG_NOT_EXIST", ucMessage));		
                    }
                    else
                    {
                        //��¼�׸�VLAN�������õĵ�ַ
                        tempterminalifcb = pstTerminalIfCb;
						
                        //��ʼ��Vrrp�Ĺ�����Ա
                        stvrrp.l3_port_id = pstLogicalPort->port_id;/*lint !e613 */
			   stvrrp.slot = pstLogicalPort->slot_id;/*lint !e613 */

                        //VE��ֻ����VE��־����VE����Ҫ��vlan��Ϣ
                        if(ATG_DCI_VE_PORT == pstLogicalPort->basic_cfg_cb->port_sub_type)/*lint !e613 */
                        {
                            stvrrp.ve_flag = 1;
                        }

                        //���߼��ӿ��µ�����INTF��Ҫʹ��VRRP
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
                                    " ret=%d PORT_INDEX=%ld ��intf_pos_id=%dʹ��ʧ��\n",
	                        		ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id));

                            	OS_PRINTF("  ***ERROR***:fhdrv_psn_l3_set_intf_vrrp() ret=%d "
                                    "PORT_INDEX=%ld ��intf_pos_id=%dʹ��ʧ��\n",
	                        		ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id);

                            	OS_SPRINTF(ucMessage,
    	                            "  ***ERROR***:fhdrv_psn_l3_set_intf_vrrp() ret=%d "
    	                            "PORT_INDEX=%ld ��intf_pos_id=%dʹ��ʧ��\n",
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

                            /* ���ô���todo */
                            if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                            {
                                spm_dbg_print_logical_port_vip_vmac_cfg(pstVipVmacData[i]);
                            }

                            //spm_dbg_record_logical_port_vip_vmac_cfg(pstVipVmacData[i]);

                            NBB_TRC_DETAIL((NBB_FORMAT "  MAC��ַ = %0x-%0x-%0x-%0x-%0x-%0x", 
                                pstVipVmacData[i]->mac[0],
                                pstVipVmacData[i]->mac[1],
                                pstVipVmacData[i]->mac[2],
                                pstVipVmacData[i]->mac[3],
                                pstVipVmacData[i]->mac[4],
                                pstVipVmacData[i]->mac[5]));
                            NBB_TRC_DETAIL((NBB_FORMAT "  IPv4��ַ = %s", 
                                spm_set_ulong_to_ipv4(pstVipVmacData[i]->ip_addr NBB_CCXT)));
                            NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", 
                                pstVipVmacData[i]->mask_len));

                            iCfgPos = spm_check_if_vip_vmac_exist(ulPortIndexKey, 
                                pstVipVmacData[i] NBB_CCXT);

                            //���ݱ�vrrp����ˢ��������vrrp����
                            stvrrp.dmac[0] = pstVipVmacData[i]->mac[0];
                            stvrrp.dmac[1] = pstVipVmacData[i]->mac[1];
                            stvrrp.dmac[2] = pstVipVmacData[i]->mac[2];
                            stvrrp.dmac[3] = pstVipVmacData[i]->mac[3];
                            stvrrp.dmac[4] = pstVipVmacData[i]->mac[4];
                            stvrrp.dmac[5] = pstVipVmacData[i]->mac[5];


			       //�׸�VLAN�������õĵ�ַ
	                     pstTerminalIfCb =  tempterminalifcb ;
#ifdef SRC				   
			       terminalifnum = pstLogicalPort->terminal_if_num;
#endif	

#ifdef SPU	      
                            terminalifnum = 0;	

                        //���߼��ӿ��µ�����INTF��Ҫʹ��VRRP
                        while (pstTerminalIfCb)
                        {

				stvrrp.ovlan_id = pstTerminalIfCb->terminal_if_cfg.svlan_id;
				stvrrp.ivlan_id = pstTerminalIfCb->terminal_if_cfg.cvlan_id;

				//���VLAN
				if(0  !=  stvrrp.ovlan_id)/*lint !e613 */
				{
					stvrrp.ovlan_id_mask = 0xfff;
				}
				else
				{
					stvrrp.ovlan_id_mask = 0;
				}

				//�����VLAN
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
                                            " ret=%d PORT_INDEX=%ld��VRRPʧ��\n",
    		                        		ret, ulPortIndexKey));

                            	OS_PRINTF("  ***ERROR***:fhdrv_psn_acl_set_vrrp() ret=%d PORT_INDEX=%ld��VRRPʧ��\n",
    		                        		ret, ulPortIndexKey);

                            	OS_SPRINTF(ucMessage,
    		                            "  ***ERROR***:fhdrv_psn_acl_set_vrrp() ret=%d PORT_INDEX=%ld ��VRRPʧ��\n",
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
                                        //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
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

                /* �����һ��entry�ĵ�ַ��*/
                pucVipVmacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->vip_vmac_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucVipVmacDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucVipVmacDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  10) ����MAC��IP��ַ����L3 (vip_vmac_num = %d) DEL\n",
                            pstSetLogicalPort->vip_vmac_num);
                    }

                    OS_SPRINTF(ucMessage,
                        "  10) ����MAC��IP��ַ����L3 (vip_vmac_num = %d) DEL\n",
                        pstSetLogicalPort->vip_vmac_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->vip_vmac_num; i++)
                    {
                        pstVipVmacData[i] = (ATG_DCI_LOG_PORT_VIP_VMAC_DATA *)
                            (pucVipVmacDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA))) * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_vip_vmac_cfg(pstVipVmacData[i]);
                        }

                        //spm_dbg_record_logical_port_vip_vmac_cfg(pstVipVmacData[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  MAC��ַ = %0x-%0x-%0x-%0x-%0x-%0x", pstVipVmacData[i]->mac[0],
                                pstVipVmacData[i]->mac[1],
                                pstVipVmacData[i]->mac[2],
                                pstVipVmacData[i]->mac[3],
                                pstVipVmacData[i]->mac[4],
                                pstVipVmacData[i]->mac[5]));
                        NBB_TRC_DETAIL((NBB_FORMAT "  IPv4��ַ = %s", 
                            spm_set_ulong_to_ipv4(pstVipVmacData[i]->ip_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", pstVipVmacData[i]->mask_len));

                        iCfgPos = spm_check_if_vip_vmac_exist(ulPortIndexKey, pstVipVmacData[i] NBB_CCXT);
                        if(iCfgPos < 0)//�����ڣ��޷�ɾ��
                        {
                            pstSetLogicalPort->vip_vmac_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                            OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld ���߼��˿ڲ�����VRRP-MAC��ַ "
                                            "=%2x-%2x-%2x-%2x-%2x-%2x,�޷�ɾ��! \n",
			                        		ulPortIndexKey, pstVipVmacData[i]->mac[0],
			                        		pstVipVmacData[i]->mac[1],
			                        		pstVipVmacData[i]->mac[2],
			                        		pstVipVmacData[i]->mac[3],
			                        		pstVipVmacData[i]->mac[4],
			                        		pstVipVmacData[i]->mac[5]);

                            OS_SPRINTF(ucMessage,
			                            "  ***ERROR***:PORT_INDEX=%ld ���߼��˿ڲ�����VRRP-MAC��ַ "
    			                            "=%2x-%2x-%2x-%2x-%2x-%2x,�޷�ɾ��! \n",
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
                                            " ��vrrp_pos_id=%dɾ��ʧ��\n",
			                        		ret, ulPortIndexKey, 
			                        		pstLogicalPort->logic_port_info_cb.ulVrrpMacPosId[iCfgPos - 1]));

                            	OS_PRINTF(
						"  ***ERROR***:fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld ��vrrp_pos_id=%dɾ��ʧ��\n",
			                        		ret, ulPortIndexKey, 
			                        		pstLogicalPort->logic_port_info_cb.ulVrrpMacPosId[iCfgPos - 1]);

                            	OS_SPRINTF(ucMessage,
			                            "  ***ERROR***:fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld ��vrrp_pos_id=%dɾ��ʧ��\n",
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
                                //�����ΪNULL��˵�����λ�������ݣ�����ɾ������
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

	        /******************************** VE MAC��ַ���� *********************************/
	        if (ulOperVeMac == ATG_DCI_OPER_ADD)
	        {

	            /* �����һ��entry�ĵ�ַ��*/
	            pucVeMacDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->ve_mac_data);

	            /* �׵�ַΪNULL���쳣 */
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
	                    printf("  11��VE MAC��ַ����\n");
	                    spm_dbg_print_logical_port_ve_mac_cfg(pstVeMacData);
	                }

	                OS_SPRINTF(ucMessage, "  11��VE MAC��ַ����\n");
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
	                //spm_dbg_record_logical_port_ve_mac_cfg(pstVeMacData);

	                NBB_TRC_DETAIL((NBB_FORMAT "  MAC��ַ = %0x-%0x-%0x-%0x-%0x-%0x", pstVeMacData->mac[0],
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
		                //�����ĿΪ��������֮ǰ���ù���̫����������
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
		                    OS_PRINTF("PORT_INDEX=%ld, VE MAC��ַ����ʧ�䣬group_id�޷���ȡ!\n", ulPortIndexKey);
		                }
					}

#endif

	                /* �������� */
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
        
	        /****************************** VLAN���� L3����******************************/
	        if (ulOperTerminalIf == ATG_DCI_OPER_ADD)
			{

	            /* �����һ��entry�ĵ�ַ��*/
	            pucTerminalIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->vlan_data);

	            /* �׵�ַΪNULL���쳣 */
	            if (pucTerminalIfDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  pucTerminalIfDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {

	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  21��VLAN���� L3 (num = %d) ADD\n", pstSetLogicalPort->vlan_num);
	                }

	                OS_SPRINTF(ucMessage, "  21��VLAN���� L3 (num = %d) ADD\n", pstSetLogicalPort->vlan_num);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                //���ΪBROADCAST�ڲŽ�INTF��ΪTUNNEL�ڲ���INTF�����ش�
	                //&& �����Ϊ VLAN�ӽӿڲŽ�INTF����Ȼ����
	                if ((pstLogicalPort->basic_cfg_cb != NULL)
	                 && (pstLogicalPort->logic_l3_cfg_cb != NULL)
	                 && (pstLogicalPort->basic_cfg_cb->port_flag == ATG_DCI_BROADCAST)
	                 && (pstLogicalPort->port_id != 0))
	                {

	                    stIntfL3.mCardPort = pstLogicalPort->port_id;
	                    stIntfL3.bindId = pstLogicalPort->logic_l3_cfg_cb->vrf_id;
	                    stIntfL3.eIntfType = INTF_TYPE_L3;

	                    //IPV4 URPFʹ��
	                    if (pstLogicalPort->logic_l3_cfg_cb->ipv4_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFȱʡ���
	                    {
	                        stIntfL3.eIpv4UrpfMode = URPF_OFF;
	                    }
	                    else if (pstLogicalPort->logic_l3_cfg_cb->ipv4_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPF�ϸ���
	                    {
	                        stIntfL3.eIpv4UrpfMode = URPF_STRICT;
	                    }
	                    else if (pstLogicalPort->logic_l3_cfg_cb->ipv4_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPF��ɢ���
	                    {
	                        stIntfL3.eIpv4UrpfMode = URPF_LOOSE;
	                    }

	                    //IPV6 URPFʹ��
	                    if (pstLogicalPort->logic_l3_cfg_cb->ipv6_urpf_mode == ATG_DCI_INTF_URPF_DEFAULT)     //URPFȱʡ���
	                    {
	                        stIntfL3.eIpv6UrpfMode = URPF_OFF;
	                    }
	                    else if (pstLogicalPort->logic_l3_cfg_cb->ipv6_urpf_mode == ATG_DCI_INTF_URPF_STRICT)  //URPF�ϸ���
	                    {
	                        stIntfL3.eIpv6UrpfMode = URPF_STRICT;
	                    }
	                    else if (pstLogicalPort->logic_l3_cfg_cb->ipv6_urpf_mode == ATG_DCI_INTF_URPF_LOOSE)  //URPF��ɢ���
	                    {
	                        stIntfL3.eIpv6UrpfMode = URPF_LOOSE;
	                    }
	                    
	                    //ȱʡ·�ɼ��ʹ��
	                    if (pstLogicalPort->logic_l3_cfg_cb->ipv4_default_check == ENABLE)
	                    {
	                        stIntfL3.flags |= INTF_V4_ALLOW_DEFAULT_ROUTE;
	                    }

	                    if (pstLogicalPort->logic_l3_cfg_cb->ipv6_default_check == ENABLE)
	                    {
	                        stIntfL3.flags |= INTF_V6_ALLOW_DEFAULT_ROUTE;
	                    }

	                    //MPLSת����ʹ��
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

		                    /* ���ô���todo */
		                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
		                    {
		                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
		                    }

		                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

		                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
		                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));

		                    //���ڶ˿�
		                    if ((pstTerminalIf->svlan_id == 0) && (pstTerminalIf->cvlan_id == 0))
		                    {
								if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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

		                    //Dot1q�ս��ӽӿ�Ӧ����L3VPN��SVLAN��Ч��CVLAN��Ч
		                    else if ((pstTerminalIf->svlan_id != 0) && (pstTerminalIf->cvlan_id == 0))
		                    {
								if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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

		                    //QinQ�ս��ӽӿ�Ӧ����L3VPN��SVLAN��CVLANͬʱ��Ч
		                    else if ((pstTerminalIf->svlan_id != 0) && (pstTerminalIf->cvlan_id != 0))
		                    {
								if (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT)    //������̫����
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
		                        OS_PRINTF("***WARNNING***: PORT_INDEX=%ld ֻ������CVlan��δ����SVlan\n", ulPortIndexKey);
		                    }


	                        //���Ϊ�ӽӿڣ�ֻ�ڱ���λ��intf��
	                        //���ΪLAG��VE�ڣ����뽨intf��
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

									//��������ڣ��������ڵ㣬�������У�������1
					                if (NULL == pstTerminalIfCb)
					                {
					                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
					                	
						                //�������ݲ���������
						                pstTerminalIfCb->intf_pos_id = intf_pos_id_temp;
                                                          pstTerminalIfCb->intf2_pos_id = stIntfL3.posId;
						                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
						                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

						                pstLogicalPort->terminal_if_num++;
					                }
									else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
									{
                                                                    pstTerminalIfCb->intf_pos_id = intf_pos_id_temp;
                                                                    pstTerminalIfCb->intf2_pos_id = stIntfL3.posId;
										NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
									}
#ifdef SPU
									almpm_addcfg_intf_onePos(ulPortIndexKey, pstTerminalIfCb->intf_pos_id);
/*
									//���������ӿ�(���ӿ�)�����ж��Ƿ�������IP��ֻ��������IP���ʹ��
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

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 ����ʧ��",
			                        		ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 ����ʧ��\n",
			                        		ret,
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:ApiC3AddIntf() ret=%d PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 ����ʧ��\n",
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

								//��������ڣ��������ڵ㣬�������У�������1
				                if (NULL == pstTerminalIfCb)
				                {
				                	pstTerminalIfCb = spm_alloc_terminal_if_cb(NBB_CXT);
				                	
					                //�������ݲ���������
					                pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
					                NBB_MEMCPY(&(pstTerminalIfCb->terminal_if_cfg), pstTerminalIf, sizeof(ATG_DCI_LOG_PORT_VLAN));
					                AVLL_INSERT(pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node);

					                pstLogicalPort->terminal_if_num++;
				                }
								else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
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

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld <��������Ϊ��><�߼�����L3Ϊ��><�˿ڱ�ʶ��ΪBroadCast><port_id=0>",
	                    		ulPortIndexKey));

	                    OS_PRINTF("***ERROR***:PORT_INDEX=%ld <��������Ϊ��><�߼�����L3Ϊ��><�˿ڱ�ʶ��ΪBroadCast><port_id=0>\n",
	                    		ulPortIndexKey);

	                    OS_SPRINTF(ucMessage,
	                        "***ERROR***:PORT_INDEX=%ld <��������Ϊ��><�߼�����L3Ϊ��><�˿ڱ�ʶ��ΪBroadCast><port_id=0>\n",
	                    		ulPortIndexKey);
	                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                    NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                            "CALL_FUNC_ERROR", ucMessage));
	                }
	            }
	        }
	        else if (ulOperTerminalIf == ATG_DCI_OPER_DEL)
	        {

	            /* �����һ��entry�ĵ�ַ��*/
	            pucTerminalIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
	                &pstSetLogicalPort->vlan_data);

	            /* �׵�ַΪNULL���쳣 */
	            if (pucTerminalIfDataStart == NULL)
	            {
	                NBB_TRC_FLOW((NBB_FORMAT "  pucTerminalIfDataStart is NULL."));
	                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
	            }
	            else
	            {

	                if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                {
	                    printf("  21��VLAN���� L3 (num = %d) DEL\n", pstSetLogicalPort->vlan_num);
	                }

	                OS_SPRINTF(ucMessage, "  21��VLAN���� L3 (num = %d) DEL\n", pstSetLogicalPort->vlan_num);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
	                
	                //���ΪBROADCAST�ڲŽ�INTF��ΪTUNNEL�ڲ���INTF�����ش�
	                //&& �����Ϊ VLAN�ӽӿڲŽ�INTF����Ȼ����
	                if ((pstLogicalPort->basic_cfg_cb != NULL)
	                 && (pstLogicalPort->logic_l3_cfg_cb != NULL)
	                 && (pstLogicalPort->basic_cfg_cb->port_flag == ATG_DCI_BROADCAST)
	                 && (pstLogicalPort->port_id != 0))
	                {

	                	for (i = 0; i < pstSetLogicalPort->vlan_num; i++)
		                {
		                    pstTerminalIf = (ATG_DCI_LOG_PORT_VLAN*)
		                        (pucTerminalIfDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_VLAN))) * i);

		                    /* ���ô���todo */
		                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
		                    {
		                        spm_dbg_print_logical_port_terminal_if_cfg(pstTerminalIf);
		                    }

		                    //spm_dbg_record_logical_port_terminal_if_cfg(pstTerminalIf);

		                    NBB_TRC_DETAIL((NBB_FORMAT "  SVLAN_ID  = %d", pstTerminalIf->svlan_id));
		                    NBB_TRC_DETAIL((NBB_FORMAT "  CVLAN_ID  = %d", pstTerminalIf->cvlan_id));


	                        //���Ϊ�ӽӿڣ�ֻ�ڱ���λ��intf��
	                        //���ΪLAG��VE�ڣ����뽨intf��
	                        if ((((pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_INTFACE)
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_SUB_INTFACE))
	                                && (SHARED.local_slot_id == pstLogicalPort->slot_id))
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG)
	                            || (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT))
	                        {

				                pstTerminalIfCb = AVLL_FIND(pstLogicalPort->terminal_if_tree, pstTerminalIf);

								//��������ڣ��޷�ɾ��
				                if (NULL == pstTerminalIfCb)
				                {
			                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

			                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 �����ڣ��޷�ɾ��",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id));

			                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 �����ڣ��޷�ɾ��\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);

			                        OS_SPRINTF(ucMessage,
			                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 �����ڣ��޷�ɾ��\n",
			                        		ulPortIndexKey,
			                        		pstTerminalIf->svlan_id,
			                        		pstTerminalIf->cvlan_id);
			                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

			                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
			                                "CALL_FUNC_ERROR", ucMessage));
				                }
                                            else	//����ɾ��
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

							            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
							            spm_free_terminal_if_cb(pstTerminalIfCb NBB_CCXT);
							            
										pstLogicalPort->terminal_if_num--;
									}
									else
									{
				                        pstSetLogicalPort->vlan_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

				                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3DelIntf() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 ����ʧ��",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id));

				                        OS_PRINTF("***ERROR***:ApiC3DelIntf() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 ����ʧ��\n",
				                        		ret,
				                        		ulPortIndexKey,
				                        		pstTerminalIf->svlan_id,
				                        		pstTerminalIf->cvlan_id);

				                        OS_SPRINTF(ucMessage,
				                            	"***ERROR***:ApiC3DelIntf() ret=%d, PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d �� VLAN���� L3 ����ʧ��\n",
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

						            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
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
	        
            /****************************** Diff-Serv���� ******************************/
            if (ulOperDiffServ == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucDiffServDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->diff_serv_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucDiffServDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDiffServDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                
                    OS_SPRINTF(ucMessage, "  12��Diff-Serv���� ADD\n");
	             BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                 
                    for (i = 0; i < pstSetLogicalPort->diff_serv_num; i++)
                    {
                        pstDiffServData = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *)
                            (pucDiffServDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  12��Diff-Serv���� ADD\n");
	                        spm_dbg_print_logical_port_diff_serv_cfg(pstDiffServData);
	                    }

	                    //OS_SPRINTF(ucMessage, "  12��Diff-Serv���� ADD\n");
	                    //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
	                    //spm_dbg_record_logical_port_diff_serv_cfg(pstDiffServData);

	                    NBB_TRC_DETAIL((NBB_FORMAT "  Diff-ServID   = %d", pstDiffServData->diff_serv_id));

	                    /* ���ô���todo */
	                    stQosIntfKey.index = ulPortIndexKey;
	                    stQosIntfKey.svlan = pstDiffServData->svlan;
	                    stQosIntfKey.cvlan = pstDiffServData->cvlan;

	                    //����߼��ӿڴ���
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
	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()���� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan));

	                            OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()���� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);

	                            OS_SPRINTF(ucMessage,
	                                "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()���� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��,posId=0!\n", 
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
    	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d ���� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan));

    	                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d ���� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��\n", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);

    	                            OS_SPRINTF(ucMessage,
    	                                "***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d ���� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��\n", 
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
                                    
                                    //��������ڣ��������ڵ㣬�������У�������1
                                    if (NULL == pstdiffservcb)
                                    {
                                        pstdiffservcb = spm_alloc_diff_serv_cb(NBB_CXT);

                                        //�������ݲ���������
                                        //pstTerminalIfCb->intf_pos_id = stIntfL3.posId;
                                        //NBB_MEMCPY(&pstdiffservcb->key,&diffservcfgkey,
                                            //sizeof(ATG_DCI_LOG_PORT_VLAN));                                        
                                        NBB_MEMCPY(&(pstdiffservcb->diff_serv_cfg),
                                            pstDiffServData, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
                                        AVLL_INSERT(pstLogicalPort->diff_serv_tree, pstdiffservcb->spm_diff_serv_node);

                                        pstLogicalPort->diff_serv_num++;
                                    }
                                    else	//Ӧ�ò�����ִ��������Ϊ�·������ģ����������ò�ͬ��
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
	                            /* �������� */
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
	                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�����Diff-Servʧ��",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�����Diff-Servʧ��\n", ulPortIndexKey);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�����Diff-Servʧ��\n",
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

                /* �����һ��entry�ĵ�ַ��*/
                pucDiffServDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->diff_serv_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucDiffServDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDiffServDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  12��Diff-Serv���� DEL\n");
	             BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                 
                    for (i = 0; i < pstSetLogicalPort->diff_serv_num; i++)
                    {
                        pstDiffServData = (ATG_DCI_LOG_PORT_DIFF_SERV_DATA *)
                            (pucDiffServDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  12��Diff-Serv���� DEL\n");
	                        spm_dbg_print_logical_port_diff_serv_cfg(pstDiffServData);
	                    }

	                    //OS_SPRINTF(ucMessage, "  12��Diff-Serv���� DEL\n");
	                    //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                        
	                    //spm_dbg_record_logical_port_diff_serv_cfg(pstDiffServData);

	                    NBB_TRC_DETAIL((NBB_FORMAT "  Diff-ServID   = %d", pstDiffServData->diff_serv_id));

	                    /* ���ô���todo */
	                    stQosIntfKey.index = ulPortIndexKey;
	                    stQosIntfKey.svlan = pstDiffServData->svlan;
	                    stQosIntfKey.cvlan = pstDiffServData->cvlan;

	                    //����߼��ӿڴ���
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
	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()ɾ�� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan));

	                            OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()ɾ�� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��,posId=0!\n", 
	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);

	                            OS_SPRINTF(ucMessage,
	                                "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node()ɾ�� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��,posId=0!\n", 
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
    	                                    "  ***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d ɾ�� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan));

    	                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d ɾ�� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��\n", 
    	                                    	ulPortIndexKey, ret, pstDiffServData->svlan, pstDiffServData->cvlan);

    	                            OS_SPRINTF(ucMessage,
    	                                "***ERROR***:PORT_INDEX=%ld spm_ds_add_logic_intf_node() ret=%d ɾ�� SVlan=%d CVlan=%d ��Diff-Serv����ʧ��\n", 
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
                                    
                                        //��������ڣ��޷�ɾ��
                                        if (NULL == pstdiffservcb)
                                        {
                                            pstSetLogicalPort->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                                            NBB_TRC_DETAIL((NBB_FORMAT 
                                            "  ***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d ��Diff-Serv���ò����ڣ��޷�ɾ��",
                                            ulPortIndexKey,
                                            pstDiffServData->svlan,
                                            pstDiffServData->cvlan));

                                            OS_PRINTF(
                                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d ��Diff-Serv���ò����ڣ��޷�ɾ��\n",
                                            ulPortIndexKey,
                                            pstDiffServData->svlan,
                                            pstDiffServData->cvlan);

                                            OS_SPRINTF(ucMessage,
                                            "***ERROR***:PORT_INDEX=%ld SVLAN_ID=%d CVLAN_ID=%d ��Diff-Serv���ò����ڣ��޷�ɾ��\n",
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

                                            //�ͷŶ˿��߼����ýڵ���ڴ�ռ�
                                            spm_free_diff_serv_cb(pstdiffservcb NBB_CCXT);

                                            pstLogicalPort->diff_serv_num--;									
                                        }
                                    }                               
                            }

		                    
#if 0
	                        if (ret == SUCCESS)
	                        {
	                            /* �������� */
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
	                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�ɾ��Diff-Servʧ��",
	                                ulPortIndexKey));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�ɾ��Diff-Servʧ��\n", ulPortIndexKey);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld ��L3�߼�����δ�£�ɾ��Diff-Servʧ��\n",
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

            /****************************** �ϻ�����������QOS��������L3 ******************************/
            if (ulOperInclassifyQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucInclassifyQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->inclassify_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucInclassifyQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucInclassifyQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  �ϻ�����������QOS��������L3 ADD\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->inclassify_qos_num; i++)
	                {
                        pstInclassifyQosData = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)
                            (pucInclassifyQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  �ϻ�����������QOS��������L3 ADD\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ����������QOS����ģ������ = %d",
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
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��\n",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��\n",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#if 0
	                    if (ret == SUCCESS)
	                    {
	                        /* �������� */
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

                /* �����һ��entry�ĵ�ַ��*/
                pucInclassifyQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->inclassify_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucInclassifyQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucInclassifyQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  �ϻ�����������QOS��������L3 DEL\n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->inclassify_qos_num; i++)
	                {
                        pstInclassifyQosData = (ATG_DCI_LOG_PORT_INCLASSIFY_QOS *)
                            (pucInclassifyQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  �ϻ�����������QOS��������L3 DEL\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ����������QOS����ģ������ = %d",
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
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��\n",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_flow_classify_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ�����������QOS��������ʧ��\n",
	                                ulPortIndexKey, ret, pstInclassifyQosData->svlan, pstInclassifyQosData->cvlan);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#if 0
	                    if (ret == SUCCESS)
	                    {
	                        /* �������� */
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

            /****************************** �ϻ��û�QOS��������L3 ******************************/
            if (ulOperUpUserQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucUpUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->up_user_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucUpUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucUpUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  15���ϻ��û�QOS��������L3 ADD\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                    for (i = 0; i < pstSetLogicalPort->up_user_qos_num; i++)
	                {
                        pstUpUserQosData = (ATG_DCI_LOG_UP_USER_QOS_POLICY *)
                            (pucUpUserQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  15���ϻ��û�QOS��������L3 ADD\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  �û�QOS����ģ������ = %d", pstUpUserQosData->qos_policy_index));

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
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ��û�QOS��������ʧ��", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ��û�QOS��������ʧ��\n", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d ���� SVlan=%d CVlan=%d ���ϻ��û�QOS��������ʧ��\n", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#if 0
	                    if (ret == SUCCESS)
	                    {
	                        /* �������� */
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

                /* �����һ��entry�ĵ�ַ��*/
                pucUpUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->up_user_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucUpUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucUpUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    OS_SPRINTF(ucMessage, "  15���ϻ��û�QOS��������L3 DEL\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                    for (i = 0; i < pstSetLogicalPort->up_user_qos_num; i++)
	                {
                        pstUpUserQosData = (ATG_DCI_LOG_UP_USER_QOS_POLICY *)
                            (pucUpUserQosDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY)))
                            * i);

	                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
	                    {
	                        printf("  15���ϻ��û�QOS��������L3 DEL\n");

	                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
	                    }

	                    NBB_TRC_DETAIL((NBB_FORMAT "  �û�QOS����ģ������ = %d", pstUpUserQosData->qos_policy_index));

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
	                                "  ***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ��û�QOS��������ʧ��", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan));

	                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ��û�QOS��������ʧ��\n", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan);

	                        OS_SPRINTF(ucMessage,
	                            "***ERROR***:PORT_INDEX=%ld spm_add_logic_up_usr_qos_node() ret=%d ɾ�� SVlan=%d CVlan=%d ���ϻ��û�QOS��������ʧ��\n", 
	                                	ulPortIndexKey, ret, pstUpUserQosData->svlan, pstUpUserQosData->cvlan);
	                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, ret,
	                                "CALL_FUNC_ERROR", ucMessage));
	                    }
#if 0
	                    if (ret == SUCCESS)
	                    {
	                        /* �������� */
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

            /****************************** �ϻ��û���QOS��������(��Ч) ******************************/
            if (ulOperUpGroupQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucUpGroupQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->upuser_group_qos_data);

                /* �׵�ַΪNULL���쳣 */
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
                        printf("  16���ϻ��û���QOS��������(��Ч)\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    //NBB_TRC_DETAIL((NBB_FORMAT "�û�QOS����ģ������ = %d", pstUpUserQosData->qos_policy_index));

                    /* �������� */
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

            /****************************** �»��û���QOS����L3 ******************************/
            if (ulOperDownGroupQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucDownGroupQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->down_user_group_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucDownGroupQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDownGroupQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDownGroupQosData = (ATG_DCI_LOG_DOWN_USER_GROUP_QOS *)pucDownGroupQosDataStart;
                    OS_SPRINTF(ucMessage, "  18���»��û���QOS����L3\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  18���»��û���QOS����L3\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  �û�������  = %d", pstDownGroupQosData->user_group_index));

                    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
                    spm_get_portid_from_logical_port_index(ulPortIndexKey, &stPortInfo NBB_CCXT);

                    ret = spm_hqos_add_group_node(stPortInfo.slot_id,
                        stPortInfo.port_id,
                        pstDownGroupQosData->user_group_index,
                        ulPortIndexKey
                        NBB_CCXT);

                    if (ret == SUCCESS)
                    {
                        /* �������� */
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

            /****************************** �»��û�����QOS��������L3 ******************************/
            if (ulOperDownUserQos == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucDownUserQosDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->down_user_queue_qos_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucDownUserQosDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucDownUserQosDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    pstDownUserQosData = (ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *)pucDownUserQosDataStart;
                    OS_SPRINTF(ucMessage, "  17���»��û�����QOS��������L3\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  17���»��û�����QOS��������L3\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }

                    NBB_TRC_DETAIL((NBB_FORMAT "  NodeIndex  = %d", pstDownUserQosData->node_index));
                    NBB_TRC_DETAIL((NBB_FORMAT "  �û�QOS����ģ������  = %d",
                            pstDownUserQosData->user_qos_policy_index));
                    NBB_TRC_DETAIL((NBB_FORMAT "  ���ȼ�����QOS����ģ������  = %d",
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
                            /* �������� */
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
                                "  ***ERROR***:PORT_INDEX=%ld ���»��û���QOS����δ�£������»��û�����QOS����ʧ��",
                                ulPortIndexKey));

                        OS_PRINTF("***ERROR***:PORT_INDEX=%ld ���»��û���QOS����δ�£������»��û�����QOS����ʧ��\n",
                            ulPortIndexKey);

                        OS_SPRINTF(ucMessage,
                            "***ERROR***:PORT_INDEX=%ld ���»��û���QOS����δ�£������»��û�����QOS����ʧ��\n",
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

            /****************************** ��������traffic_filterL3 ******************************/
            if (ulOperTrafficFilter == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucTrafficFilterDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->traffic_filter_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucTrafficFilterDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucTrafficFilterDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  20����������traffic_filter L3 ADD\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }
                    
                    OS_SPRINTF(ucMessage, "  20����������traffic_filter L3 ADD\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->traffic_filter_num; i++)
                    {
                        pstTrafficFilter = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)
                            (pucTrafficFilterDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER)))
                            * i);

                        NBB_TRC_DETAIL((NBB_FORMAT "  acl_id  = %d", pstTrafficFilter->acl_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ����direction  = %d", pstTrafficFilter->direction));

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

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter����ʧ��!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter����ʧ��!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter����ʧ��!\n",
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
                                    /* �������� */
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

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��\n",
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

                /* �����һ��entry�ĵ�ַ��*/
                pucTrafficFilterDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->traffic_filter_data);

                /* �׵�ַΪNULL���쳣 */
                if (pucTrafficFilterDataStart == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  pucTrafficFilterDataStart is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {

                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  20����������traffic_filter L3 DEL\n");

                        //spm_dbg_print_logical_port_flow_diff_serv_cfg(pstFlowDiffServData);
                    }
                    
                    OS_SPRINTF(ucMessage, "  20����������traffic_filter L3 DEL\n"); 
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                    
                    for (i = 0; i < pstSetLogicalPort->traffic_filter_num; i++)
                    {
                        pstTrafficFilter = (ATG_DCI_LOG_PORT_TRAFFIC_FILTER *)
                            (pucTrafficFilterDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER)))
                            * i);

                        NBB_TRC_DETAIL((NBB_FORMAT "  acl_id  = %d", pstTrafficFilter->acl_id));
                        NBB_TRC_DETAIL((NBB_FORMAT "  ����direction  = %d", pstTrafficFilter->direction));

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

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter����ʧ��!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter����ʧ��!\n",
                                    ulPortIndexKey,
                                    ret,
                                    pstTrafficFilter->svlan, 
                                    pstTrafficFilter->cvlan);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld, spm_add_logic_acl_node() ret=%d ���� SVlan=%d CVlan=%d �İ�������traffic_filter����ʧ��!\n",
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
                                    /* �������� */
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

                            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��",
                                    ulPortIndexKey));

                            OS_PRINTF("***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��\n", ulPortIndexKey);

                            OS_SPRINTF(ucMessage,
                                "***ERROR***:PORT_INDEX=%ld �İ�������traffic_filter����ʧ��\n",
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

            /****************************** VRRP����MAC��IPv6��ַ����L3 ******************************/
            if (ul_oper_vipv6_vmac == ATG_DCI_OPER_ADD)
            {

                /* �����һ��entry�ĵ�ַ��*/
                puc_vipv6_vmac_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->vrrp_ipv6_mac_l3_data);

                /* �׵�ַΪNULL���쳣 */
                if (puc_vipv6_vmac_data_start == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  puc_vipv6_vmac_data_start is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  25) ����MAC��IPv6��ַ����L3 (vrrp_ipv6_mac_l3_num = %d) ADD\n",
                            pstSetLogicalPort->vrrp_ipv6_mac_l3_num);
                    }

                    OS_SPRINTF(ucMessage,
                        "  25) ����MAC��IPv6��ַ����L3 (vrrp_ipv6_mac_l3_num = %d) ADD\n",
                        pstSetLogicalPort->vrrp_ipv6_mac_l3_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    //�ҵ�vlan���Ե�һ������
                    pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);

                    //û��vlan���ԣ��޷��󶨣���ӡ����ֱ���˳�
                    if(NULL == pstTerminalIfCb)
                    {
                        //�˴��·�������VRRP���þ�����ʧ��
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld �� �߼��˿�VLAN���Բ����ڣ��޷�����VRRP ipv6!\n",
			                        		ulPortIndexKey));

                        OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld �� �߼��˿�VLAN���Բ����ڣ��޷�����VRRP ipv6!\n",
			                        		ulPortIndexKey);

                        OS_SPRINTF(ucMessage,	"  ***ERROR***:PORT_INDEX=%ld �� �߼��˿�VLAN���Բ����ڣ��޷�����VRRP ipv6!\n",
			                            	ulPortIndexKey);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | LOGICAL_PORT_PD, 0, "ld d s s", ulPortIndexKey, BASIC_CONFIG_NOT_EXIST,
			                                "BASIC_CONFIG_NOT_EXIST", ucMessage));		
                    }
                    else
                    {
                        //��¼�׸�VLAN�������õĵ�ַ
                        tempterminalifcb_v6 = pstTerminalIfCb;
						
                        //��ʼ��Vrrp�Ĺ�����Ա
                        stvrrp_v6.l3_port_id = pstLogicalPort->port_id;/*lint !e613 */
			   stvrrp_v6.slot = pstLogicalPort->slot_id;/*lint !e613 */

                        //VE��ֻ����VE��־����VE����Ҫ��vlan��Ϣ
                        if(ATG_DCI_VE_PORT == pstLogicalPort->basic_cfg_cb->port_sub_type)/*lint !e613 */
                        {
                            stvrrp_v6.ve_flag = 1;
                        }

                        //���߼��ӿ��µ�����INTF��Ҫʹ��VRRP
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
                                    " ret=%d PORT_INDEX=%ld ��intf_pos_id=%dʹ��ʧ��\n",
	                        		ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id));

                            	OS_PRINTF("  ***ERROR***:vrrp fhdrv_psn_l3_set_intf_vrrp() ret=%d "
                                    "PORT_INDEX=%ld ��intf_pos_id=%dʹ��ʧ��\n",
	                        		ret, ulPortIndexKey, pstTerminalIfCb->intf_pos_id);

                            	OS_SPRINTF(ucMessage,
    	                            "  ***ERROR***:vrrp fhdrv_psn_l3_set_intf_vrrp() ret=%d "
    	                            "PORT_INDEX=%ld ��intf_pos_id=%dʹ��ʧ��\n",
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

                            /* ���ô���todo */
                            if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                            {
                                spm_dbg_print_logical_port_vipv6_vmac_cfg(pst_vipv6_vmac_data[i]);
                            }

                            //spm_dbg_record_logical_port_vip_vmac_cfg(pst_vipv6_vmac_data[i]);

                            NBB_TRC_DETAIL((NBB_FORMAT "  MAC��ַ = %0x-%0x-%0x-%0x-%0x-%0x", 
                                pst_vipv6_vmac_data[i]->mac_address[0],
                                pst_vipv6_vmac_data[i]->mac_address[1],
                                pst_vipv6_vmac_data[i]->mac_address[2],
                                pst_vipv6_vmac_data[i]->mac_address[3],
                                pst_vipv6_vmac_data[i]->mac_address[4],
                                pst_vipv6_vmac_data[i]->mac_address[5]));
                            
                            //NBB_TRC_DETAIL((NBB_FORMAT "  IPv4��ַ = %s", 
                                //spm_set_ulong_to_ipv4(pst_vipv6_vmac_data[i]->ip_addr NBB_CCXT)));
                            NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", 
                                pst_vipv6_vmac_data[i]->sub_net_mask_len));

                            v6cfgpos = spm_check_if_vipv6_vmac_exist(ulPortIndexKey, 
                                pst_vipv6_vmac_data[i] NBB_CCXT);

                            //���ݱ�vrrp����ˢ��������vrrp����
                            stvrrp_v6.dmac[0] = pst_vipv6_vmac_data[i]->mac_address[0];
                            stvrrp_v6.dmac[1] = pst_vipv6_vmac_data[i]->mac_address[1];
                            stvrrp_v6.dmac[2] = pst_vipv6_vmac_data[i]->mac_address[2];
                            stvrrp_v6.dmac[3] = pst_vipv6_vmac_data[i]->mac_address[3];
                            stvrrp_v6.dmac[4] = pst_vipv6_vmac_data[i]->mac_address[4];
                            stvrrp_v6.dmac[5] = pst_vipv6_vmac_data[i]->mac_address[5];


			       //�׸�VLAN�������õĵ�ַ
	                     pstTerminalIfCb =  tempterminalifcb_v6 ;
#ifdef SRC				   
			       terminalifnum_v6 = pstLogicalPort->terminal_if_num;
#endif	

#ifdef SPU	      
                            terminalifnum_v6 = 0;	

                        //���߼��ӿ��µ�����INTF��Ҫʹ��VRRP
                        while (pstTerminalIfCb)
                        {

				stvrrp_v6.ovlan_id = pstTerminalIfCb->terminal_if_cfg.svlan_id;
				stvrrp_v6.ivlan_id = pstTerminalIfCb->terminal_if_cfg.cvlan_id;

				//���VLAN
				if(0  !=  stvrrp_v6.ovlan_id)/*lint !e613 */
				{
					stvrrp_v6.ovlan_id_mask = 0xfff;
				}
				else
				{
					stvrrp_v6.ovlan_id_mask = 0;
				}

				//�����VLAN
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
                                            " ret=%d PORT_INDEX=%ld��VRRPʧ��\n",
    		                        		ret, ulPortIndexKey));

                            	OS_PRINTF("  ***ERROR***:vrrp fhdrv_psn_acl_set_vrrp() ret=%d PORT_INDEX=%ld��VRRPʧ��\n",
    		                        		ret, ulPortIndexKey);

                            	OS_SPRINTF(ucMessage,
    		                            "  ***ERROR***:vrrp fhdrv_psn_acl_set_vrrp() ret=%d PORT_INDEX=%ld ��VRRPʧ��\n",
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
                                        //���ΪNULL��˵�����λ��Ϊ�����ݣ����Դ洢����
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

                /* �����һ��entry�ĵ�ַ��*/
                puc_vipv6_vmac_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetLogicalPort,
                    &pstSetLogicalPort->vrrp_ipv6_mac_l3_data);

                /* �׵�ַΪNULL���쳣 */
                if (puc_vipv6_vmac_data_start == NULL)
                {
                    NBB_TRC_FLOW((NBB_FORMAT "  puc_vipv6_vmac_data_start is NULL."));
                    NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
                }
                else
                {
                    if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        printf("  25) ����MAC��IPv6��ַ����L3 (vrrp_ipv6_mac_l3_num = %d) DEL\n",
                            pstSetLogicalPort->vrrp_ipv6_mac_l3_num);
                    }

                    OS_SPRINTF(ucMessage,
                        "  25) ����MAC��IPv6��ַ����L3 (vrrp_ipv6_mac_l3_num = %d) DEL\n",
                        pstSetLogicalPort->vrrp_ipv6_mac_l3_num);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    for (i = 0; i < pstSetLogicalPort->vrrp_ipv6_mac_l3_num; i++)
                    {
                        pst_vipv6_vmac_data[i] = (ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *)
                            (puc_vipv6_vmac_data_start + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3))) * i);

                        /* ���ô���todo */
                        if (logical_port_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_logical_port_vipv6_vmac_cfg(pst_vipv6_vmac_data[i]);
                        }

                        //spm_dbg_record_logical_port_vip_vmac_cfg(pst_vipv6_vmac_data[i]);

                        NBB_TRC_DETAIL((NBB_FORMAT "  MAC��ַ = %0x-%0x-%0x-%0x-%0x-%0x", 
                            pst_vipv6_vmac_data[i]->mac_address[0],
                                pst_vipv6_vmac_data[i]->mac_address[1],
                                pst_vipv6_vmac_data[i]->mac_address[2],
                                pst_vipv6_vmac_data[i]->mac_address[3],
                                pst_vipv6_vmac_data[i]->mac_address[4],
                                pst_vipv6_vmac_data[i]->mac_address[5]));
                        
                        //NBB_TRC_DETAIL((NBB_FORMAT "  IPv4��ַ = %s", 
                            //spm_set_ulong_to_ipv4(pst_vipv6_vmac_data[i]->ip_addr NBB_CCXT)));
                        NBB_TRC_DETAIL((NBB_FORMAT "  �������볤�� = %d", 
                            pst_vipv6_vmac_data[i]->sub_net_mask_len));

                        v6cfgpos = spm_check_if_vipv6_vmac_exist(ulPortIndexKey, pst_vipv6_vmac_data[i] NBB_CCXT);
                        if(v6cfgpos < 0)//�����ڣ��޷�ɾ��
                        {
                            pstSetLogicalPort->vrrp_ipv6_mac_l3_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                            OS_PRINTF("  ***ERROR***:PORT_INDEX=%ld ���߼��˿ڲ�����VRRP v6-MAC��ַ "
                                            "=%2x-%2x-%2x-%2x-%2x-%2x,�޷�ɾ��! \n",
			                        		ulPortIndexKey, pst_vipv6_vmac_data[i]->mac_address[0],
			                        		pst_vipv6_vmac_data[i]->mac_address[1],
			                        		pst_vipv6_vmac_data[i]->mac_address[2],
			                        		pst_vipv6_vmac_data[i]->mac_address[3],
			                        		pst_vipv6_vmac_data[i]->mac_address[4],
			                        		pst_vipv6_vmac_data[i]->mac_address[5]);

                            OS_SPRINTF(ucMessage,
			                            "  ***ERROR***:PORT_INDEX=%ld ���߼��˿ڲ�����VRRP v6-MAC��ַ "
    			                            "=%2x-%2x-%2x-%2x-%2x-%2x,�޷�ɾ��! \n",
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
                                            " ��vrrp_pos_id=%dɾ��ʧ��\n",
			                        		ret, ulPortIndexKey, 
			                        		pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid[v6cfgpos - 1]));

                            	OS_PRINTF(
						"  ***ERROR***:vrrp fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld ��vrrp_pos_id=%dɾ��ʧ��\n",
			                        		ret, ulPortIndexKey, 
			                        		pstLogicalPort->logic_port_info_cb.vrrp_ipv6_posid[v6cfgpos - 1]);

                            	OS_SPRINTF(ucMessage,
			                            "  ***ERROR***:vrrp fhdrv_psn_acl_del_vrrp() ret=%d PORT_INDEX=%ld ��vrrp_pos_id=%dɾ��ʧ��\n",
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
                                //�����ΪNULL��˵�����λ�������ݣ�����ɾ������
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

        //�˿����ԼȲ���L2Ҳ����L3
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***WARNNING***:�˿�(%ld)���ԼȲ���L2Ҳ����L3(��������ò�����)", ulPortIndexKey));
            OS_PRINTF("***WARNNING***:�˿�(%ld)���ԼȲ���L2Ҳ����L3(��������ò�����)\n", ulPortIndexKey);

            OS_SPRINTF(ucMessage,
                "***WARNNING***:�˿�(%ld)���ԼȲ���L2Ҳ����L3(��������ò�����)\n", 
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

        /******************************** �����ӿ��� *********************************/

        /*Ӧ�ó���:�¹����ӿ�������ʱ���߼��ӿ�δ�·�����ʱֻ���棬�����߼��ӿ�����ʱ��ˢ*/
         
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
            /*���߼��ڵ�index�Ѿ����뵽�����ӿ����У�֮ǰֻ���棬������ʽ����*/

             if(trigger_if_flag == 1)
             {
                ret = spm_get_portid_from_logical_port_index(ulPortIndexKey,&stPortInfo NBB_CCXT);
                
                if(ret != SUCCESS)
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: �߼��˿�index=%ld��ȡ�����ʧ�ܣ� ret=%d",ulPortIndexKey,ret));
    		
					OS_PRINTF("***ERROR***:�߼��˿�index=%ld��ȡ�����ʧ�ܣ� ret=%d",ulPortIndexKey,ret);

					OS_SPRINTF(ucMessage, "***ERROR***:�߼��˿�index=%ld��ȡ�����ʧ�ܣ� ret=%d",ulPortIndexKey,ret);
					BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

					NBB_EXCEPTION((PCT_SPM | ASSOCIATE_IF_PD, 0, "ld d s s", ulAssociateIfKey, ret, "spm_get_portid_from_logical_port_index", ucMessage));
    				
                }

                /*lag����lag���ÿ����*/
                
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

            /*lag����lag���ÿ����*/
            
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
   �� �� ��  : spm_del_logical_port_cfg
   ��������  : ɾ���˿��߼�����
   �������  : SPM_LOGICAL_PORT_CB* pstLogicalPort
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��5��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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

    //����������ò����ڣ�����Ҫ����
    if ((pstLogicalPort->basic_cfg_cb == NULL) || (pstLogicalPort->phy_cfg_cb == NULL))
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:�˿��߼����õĻ������ò�����"));

        OS_PRINTF("***ERROR***:Ҫɾ����PORT_INDEX=%ld �Ķ˿��߼����õĻ������û��������ò�������!\n", ulPortIndexKey);

        OS_SPRINTF(ucMessage,
            "***ERROR***:Ҫɾ����PORT_INDEX=%ld �Ķ˿��߼����õĻ������û��������ò�������!\n",
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
    //ɾ��Diff-Serv����
    if ((pstLogicalPort->diff_serv_cfg_cb != NULL) && (pstLogicalPort->logic_l3_cfg_cb != NULL))
    {
        spm_ds_add_logic_intf_node(&stSubPort,
            &stQosIntfKey,
            &stDiffServ);
    }

    //ɾ�������Diff-Serv����L2
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


    //ɾ���ϻ�����������QOS��������
    if (pstLogicalPort->inclassify_qos_cfg_cb != NULL)
    {
        spm_add_logic_flow_classify_node(ulPortIndexKey, 0, &stSubPort);
    }

    //ɾ���ϻ��û�QOS��������
    if (pstLogicalPort->up_user_qos_cfg_cb != NULL)
    {
        spm_add_logic_up_usr_qos_node(ulPortIndexKey,
            &stSubPort,
            &stUpUserQosPolicy);
    }
#endif
    //ɾ���»��û�����QOS�������� �� �»��û���QOS����
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
            NBB_CCXT); //nhi��ʱ��ȡ��������0
    }

    //ɾ��Diff-Serv����L2��VP�»�UNI�ࣩ
    if (pstLogicalPort->ds_l2_cfg_cb != NULL)
    {
		stUniVp.vpn_id = pstLogicalPort->logic_port_info_cb.vpn_id;
		stUniVp.vp_type = pstLogicalPort->logic_port_info_cb.vpn_type;
		stUniVp.vp_posid = pstLogicalPort->logic_port_info_cb.vp_idx;

		stDsL2.ds_id = 0;

		spm_ds_add_logic_univp_node(ulPortIndexKey,	&stUniVp, &stDsL2 NBB_CCXT);
    }

	//��������traffic_filter
	spm_qos_clear_logic_acl(ulPortIndexKey NBB_CCXT);

	//�ϻ�����������QOS��������
	spm_qos_clear_logic_classify(ulPortIndexKey NBB_CCXT);

	//Diff-Serv����
	spm_qos_clear_ds_logic_intf(ulPortIndexKey NBB_CCXT);

	//�����Diff-Serv����L2
	spm_qos_clear_logic_ds_flow(ulPortIndexKey NBB_CCXT);

	//ɾ�����߼��ӿ��ϵ����ܰ�
    //almpm_delline_intf(ulPortIndexKey, 1);
	
    //���������ӿڣ�ɾ��intf
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
    else if (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L2)   //����Ƕ���ӿ�
    {
        //L2��û��VP�Ͳ����ܴ���INTF��������Ҫɾ��L2��INTF��������Ӧ��VP����
        if (pstLogicalPort->logic_port_info_cb.vp_idx != 0)
        {
		    for (pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pstLogicalPort->terminal_if_tree);
		         pstTerminalIfCb != NULL;
		         pstTerminalIfCb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pstLogicalPort->terminal_if_tree,
		                       pstTerminalIfCb->spm_terminal_if_node))
		    {
                //flow������
                if (pstTerminalIfCb->intf_pos_id== 0)
                {
                    continue;
                }

#ifdef SPU
				ret = ApiC3DelIntf(pstLogicalPort->unit_id, pstTerminalIfCb->intf_pos_id);
				almpm_delline_intf(ulPortIndexKey, pstTerminalIfCb->intf_pos_id, 1);
#endif

                //ɾ��ʧ��
                if (ret != SUCCESS)
                {
                	//coverity[dead_error_begin]
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3DelIntf() L2 ret=%d PORT_INDEX=%ld ɾ��INTFʧ��!\n", ret, ulPortIndexKey));
                    OS_PRINTF("***ERROR***:ApiC3DelIntf() L2 ret=%d PORT_INDEX=%ld ɾ��INTFʧ��!\n", ret, ulPortIndexKey);

			        OS_SPRINTF(ucMessage, "***ERROR***:ApiC3DelIntf() L2 ret=%d PORT_INDEX=%ld ɾ��INTFʧ��!\n", ret, ulPortIndexKey);
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
            NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:VP������! flow�������κ�VP��INTF flow�޷�ɾ��!"));

            //OS_PRINTF("***ERROR***:VP������! flow�������κ�VP��INTF flow�޷�ɾ��!\n");

            //goto EXIT_LABEL;
        }

    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_check_flow_vlan_conflict
   ��������  : ���һ���߼��˿���10������vlan�Ƿ����ظ�
   �������  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_FLOW_DATA *pstFlow
   �������  : ��
   �� �� ֵ  :  0 �����ظ�
             -1 : ���ظ�
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��5��7��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_flow_vlan_conflict() PORT_INDEX=%ld �߼��˿ڲ�����",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_flow_vlan_conflict() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);

        OS_SPRINTF(ucMessage,
            "***ERROR***:spm_check_flow_vlan_conflict() PORT_INDEX=%ld �߼��˿ڲ����� \n",
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
            //���flow_id��ͬ����ôovlan������ͬ
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
   �� �� ��  : spm_check_if_ipv4_exist
   ��������  : ���һ���߼��˿���ipv4�Ƿ����ظ�
   �������  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_IPV4_DATA *pstIpv4
   �������  : ��
   �� �� ֵ  :  0 �����ظ�
             n : ���ظ���n=����λ��+1
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��5��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_ipv4_exist() PORT_INDEX=%ld �߼��˿ڲ�����",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_ipv4_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_ipv4_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);
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
            //���IPV4��ȣ����ش洢λ��
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
   �� �� ��  : spm_check_if_ipv6_exist
   ��������  : ���һ���߼��˿���ipv6�Ƿ����ظ�
   �������  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_IPV6_DATA *pstIpv6
   �������  : ��
   �� �� ֵ  :  0 �����ظ�
             n : ���ظ���n=����λ��+1
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��5��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_ipv6_exist() PORT_INDEX=%ld �߼��˿ڲ�����",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_ipv6_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_ipv6_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);
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
            //���IPV4��ȣ����ش洢λ��
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
   �� �� ��  : spm_check_if_mc_mac_exist
   ��������  : ���һ���߼��˿����鲥MAC��ַ�Ƿ����ظ�
   �������  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_MC_MAC_DATA *pstMcMac
   �������  : ��
   �� �� ֵ  : 0 �����ظ�
             n : ���ظ���n=����λ��+1
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��5��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_mc_ipv4_exist() PORT_INDEX=%ld �߼��˿ڲ�����",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_mc_ipv4_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_mc_ipv4_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);
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
            //���IPV4��ȣ����ش洢λ��
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
   �� �� ��  : spm_check_if_mc_ipv6_exist
   ��������  : ���һ���߼��˿���IPV6�鲥��ַ�Ƿ����ظ�
   �������  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_MC_IPV6_DATA *pstMcIpv6
   �������  : ��
   �� �� ֵ  : 0 �����ظ�
             n : ���ظ���n=����λ��+1
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��10��31��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

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
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_mc_ipv6_exist() PORT_INDEX=%ld �߼��˿ڲ�����",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_mc_ipv6_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_mc_ipv6_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);
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
            //���IPV6��ȣ����ش洢λ��
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
   �� �� ��  : spm_check_if_vip_vmac_exist
   ��������  : ���һ���߼��˿�������MAC��IP��ַ�����Ƿ����ظ�
   �������  : NBB_ULONG ulPortIndex
             ATG_DCI_LOG_PORT_VIP_VMAC_DATA *pstVipVmac
   �������  : ��
   �� �� ֵ  : 0 �����ظ�
             n : ���ظ���n=����λ��+1
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��5��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_vip_vmac_exist() PORT_INDEX=%ld �߼��˿ڲ�����",
                ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_vip_vmac_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);

        OS_SPRINTF(ucMessage,
            "***ERROR***:spm_check_if_vip_vmac_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n",
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
            //�������MAC��IP��ַ������ȣ����ش洢λ��
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
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_check_if_acl_exist() PORT_INDEX=%ld �߼��˿ڲ�����", ulPortIndex));

        OS_PRINTF("***ERROR***:spm_check_if_acl_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***:spm_check_if_acl_exist() PORT_INDEX=%ld �߼��˿ڲ����� \n", ulPortIndex);
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
            //�������MAC��IP��ַ������ȣ����ش洢λ��
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
   �� �� ��  : spm_logical_port_drvinfo_clear
   ��������  : ����������ص�ֵ
   �������  : NBB_ULONG ulPortIndex
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��5��3��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
   �� �� ��  : spm_uni_logical_port_drvinfo_clear
   ��������  : ����������ص�ֵ
   �������  : NBB_ULONG ulPortIndex
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��1��17��
    ��    ��   : zhangzm
    �޸�����   : �����ɺ���

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
   �� �� ��  : spm_uni_logical_port_drvinfo_nhi_clear
   ��������  : ����������ص�ֵ������nhi
   �������  : NBB_ULONG ulPortIndex
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��8��21��
    ��    ��   : zhangsen
    �޸�����   : �����ɺ���

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
   �� �� ��  : spm_init_logical_port_tree
   ��������  : ��ʼ���洢�˿��߼����õ���
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��11��24��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
   �� �� ��  : spm_alloc_logical_port_cb
   ��������  : �˿��߼����ýṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_LOGICAL_PORT_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��10��12��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_LOGICAL_PORT_CB *spm_alloc_logical_port_cb(NBB_CXT_T NBB_CXT)
{
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    NBB_BUF_SIZE avll_key_offset;
    NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_alloc_physical_port_cb");

    /* ����һ���µĽӿ�����������Ŀ��*/
    pstLogicalPort = (SPM_LOGICAL_PORT_CB *)NBB_MM_ALLOC(sizeof(SPM_LOGICAL_PORT_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_LOGICAL_PORT_CB);
    if (pstLogicalPort == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ���ӿ�����������Ŀ */
    OS_MEMSET(pstLogicalPort, 0, sizeof(SPM_LOGICAL_PORT_CB));

    /* ��ʼ��ָ�� */
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
    /* ��ʼ�����ڴ洢SPM_FLOW_NODE_CB��AVLL�� Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
/*    for (i = 0; i < ATG_DCI_LOG_PORT_FLOW_NUM; i++)
    {
        pstLogicalPort->flow_cfg_cb = NULL;
    }
*/
    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_FLOW_NODE_CB��AVLL�� Added by xiaoxiang, 2013/4/26*/
    /***************************************************************************/
    //for (i = 0; i < ATG_DCI_LOG_FLOW_DIFF_SERV_NUM; i++)
    //{
    //    pstLogicalPort->flow_diff_serv_cfg_cb[i] = NULL;
    //}

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_IPV4_NODE_CB��AVLL�� Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
    {
        pstLogicalPort->ipv4_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_IPV6_NODE_CB��AVLL�� Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
    {
        pstLogicalPort->ipv6_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_MC_MAC_NODE_CB��AVLL�� Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
    {
        pstLogicalPort->mc_ipv4_cfg_cb[i] = NULL;
    }

	/***************************************************************************/
    /* ��ʼ��IPV6�鲥���� Added by zhangzhm, 2013/11/1*/
    /***************************************************************************/
	for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
    {
        pstLogicalPort->mc_ipv6_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_VIP_VMAC_NODE_CB��AVLL�� Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pstLogicalPort->vip_vmac_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_VIP_VMAC_NODE_CB��AVLL�� Added by xiaoxiang, 2012/10/16*/
    /***************************************************************************/
    for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
    {
        pstLogicalPort->vipv6_vmac_cfg_cb[i] = NULL;
    }

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_VIP_VMAC_NODE_CB��AVLL�� Added by xiaoxiang, 2013/4/26*/
    /***************************************************************************/
    //for (i = 0; i < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; i++)
    //{
    //    pstLogicalPort->traffic_filter_cfg_cb[i] = NULL;
    //}

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_TERMINAL_IF_NODE_CB��AVLL�� Added by xiaoxiang, 2013/9/3*/
    /***************************************************************************/
    //for (i = 0; i < ATG_DCI_LOG_TERMINAL_IF_NUM; i++)
    //{
    //    pstLogicalPort->terminal_if_cfg_cb[i] = NULL;
    //}

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_TERMINAL_IF_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->terminal_if_tree, spm_terminal_if_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TERMINAL_IF_CB, terminal_if_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TERMINAL_IF_CB, spm_terminal_if_node));/*lint !e413 */

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_DIFF_SERV_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->diff_serv_tree, spm_diff_serv_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_DIFF_SERV_CB, diff_serv_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_DIFF_SERV_CB, spm_diff_serv_node));/*lint !e413 */

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_FLOW_DIFF_SERV_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->flow_diff_serv_tree, spm_flow_diff_serv_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_DIFF_SERV_CB, flow_diff_serv_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_FLOW_DIFF_SERV_CB, spm_flow_diff_serv_node));/*lint !e413 */

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_INCLASSIFY_QOS_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->inclassify_qos_tree, spm_inclassify_qos_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_INCLASSIFY_QOS_CB, inclassify_qos_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_INCLASSIFY_QOS_CB, spm_inclassify_qos_node));/*lint !e413 */

    /***************************************************************************/
    /* ��ʼ�����ڴ洢SPM_TRAFFIC_FILTER_CB��AVLL��.                              */
    /***************************************************************************/
    AVLL_INIT_TREE(pstLogicalPort->traffic_filter_tree, spm_traffic_filter_key_compare,
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TRAFFIC_FILTER_CB, traffic_filter_cfg),/*lint !e413 */
                   (NBB_USHORT) NBB_OFFSETOF(SPM_TRAFFIC_FILTER_CB, spm_traffic_filter_node));/*lint !e413 */    
    
    /* �������ڸýӿ�����������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstLogicalPort->spm_logical_port_handle = NBB_CREATE_HANDLE(pstLogicalPort,
        HDL_SPM_LOGICAL_PORT_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_LOGICAL_PORT_CB allocated at %p with handle %#lx",
            pstLogicalPort, pstLogicalPort->spm_logical_port_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstLogicalPort->spm_logical_port_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstLogicalPort);
}

/*****************************************************************************
   �� �� ��  : spm_free_logical_port_cb
   ��������  : �ͷŶ˿��߼����ýڵ���ڴ�ռ�
   �������  : SPM_LOGICAL_PORT_CB *pstLogicalPort
             NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��11��8��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstLogicalPort, sizeof(SPM_LOGICAL_PORT_CB), MEM_SPM_LOGICAL_PORT_CB);

    if (pstLogicalPort == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_LOGICAL_PORT_CB at %p", pstLogicalPort));
    NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    if (pstLogicalPort->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->basic_cfg_cb, MEM_SPM_LOG_PORT_BASIC_CB);
        pstLogicalPort->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ������������ݿ顣                                                    */
    /***************************************************************************/
    if (pstLogicalPort->phy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->phy_cfg_cb, MEM_SPM_LOG_PORT_PHY_CB);
        pstLogicalPort->phy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ��߼�����L3���ݿ顣                                                  */
    /***************************************************************************/
    if (pstLogicalPort->logic_l3_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->logic_l3_cfg_cb, MEM_SPM_LOG_PORT_L3_CB);
        pstLogicalPort->logic_l3_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ��߼�����L2���ݿ顣                                                  */
    /***************************************************************************/
    if (pstLogicalPort->logic_l2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->logic_l2_cfg_cb, MEM_SPM_LOG_PORT_L2_CB);
        pstLogicalPort->logic_l2_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ����������L2���ݿ顣                                                */
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
    /* �ͷ��߼�����CES/CEP���ݿ顣                                             */
    /***************************************************************************/
    if (pstLogicalPort->ces_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->ces_cfg_cb, MEM_SPM_LOG_PORT_CES_CB);
        pstLogicalPort->ces_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�Ipv4��ַ����L3���ݿ顣                                             */
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
    /* �ͷ�Ipv6��ַ����L3���ݿ顣                                             */
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
    /* �ͷ��鲥���ַL3���ݿ顣                                                */
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
    /* �ͷ�����MAC��IP��ַ����L3���ݿ顣                                       */
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
    /* �ͷ�����MAC��IP��ַ����L3���ݿ顣                                       */
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
    /* �ͷ�VE MAC��ַ���ݿ顣                                             */
    /***************************************************************************/
    if (pstLogicalPort->ve_mac_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->ve_mac_cfg_cb, MEM_SPM_LOG_PORT_VE_MAC_CB);
        pstLogicalPort->ve_mac_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�Diff-Serv�������ݿ顣                                             */
    /***************************************************************************/
    //if (pstLogicalPort->diff_serv_cfg_cb != NULL)
    //{
    //    NBB_MM_FREE(pstLogicalPort->diff_serv_cfg_cb, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    //    pstLogicalPort->diff_serv_cfg_cb = NULL;
    //}

    /***************************************************************************/
    /* �ͷ������Diff-Serv����L2���ݿ顣                                                */
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
    /* �ͷ������Diff-Serv����L2���ݿ顣                                                */
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
    /* �ͷ��ϻ�����������QOS��������L3�������ݿ顣                                         */
    /***************************************************************************/
    //if (pstLogicalPort->inclassify_qos_cfg_cb != NULL)
    //{
    //    NBB_MM_FREE(pstLogicalPort->inclassify_qos_cfg_cb, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    //    pstLogicalPort->inclassify_qos_cfg_cb = NULL;
    //}
    
    /***************************************************************************/
    /* �ͷ��ϻ�����������QOS��������L3�������ݿ顣                                         */
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
    /* �ͷ��ϻ��û�QOS��������L3�������ݿ顣                                         */
    /***************************************************************************/
    //if (pstLogicalPort->up_user_qos_cfg_cb != NULL)
    //{
    //    NBB_MM_FREE(pstLogicalPort->up_user_qos_cfg_cb, MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
    //    pstLogicalPort->up_user_qos_cfg_cb = NULL;
    //}

    /***************************************************************************/
    /* �ͷ��ϻ��û���QOS��������(��Ч)�������ݿ顣                                         */
    /***************************************************************************/
    if (pstLogicalPort->up_user_group_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->up_user_group_qos_cfg_cb, MEM_SPM_LOG_PORT_UP_GROUP_QOS_CB);
        pstLogicalPort->up_user_group_qos_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ��»��û�����QOS��������L3�������ݿ顣                                         */
    /***************************************************************************/
    if (pstLogicalPort->down_user_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->down_user_qos_cfg_cb, MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);
        pstLogicalPort->down_user_qos_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ��»��û���QOS����L3�������ݿ顣                                         */
    /***************************************************************************/
    if (pstLogicalPort->down_user_group_qos_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->down_user_group_qos_cfg_cb, MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);
        pstLogicalPort->down_user_group_qos_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�������ϻ��û�QOS��������L2�������ݿ顣                                         */
    /***************************************************************************/
    //if (pstLogicalPort->flow_up_user_qos_cfg_cb != NULL)
    //{
    //    NBB_MM_FREE(pstLogicalPort->flow_up_user_qos_cfg_cb, MEM_SPM_LOG_PORT_FLOW_UP_USER_QOS_CB);
    //    pstLogicalPort->flow_up_user_qos_cfg_cb = NULL;
    //}

    /***************************************************************************/
    /* �ͷ�����������traffic_filterL3�������ݿ顣                                         */
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
    /* �ͷ�����������traffic_filterL3�������ݿ顣                                         */
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
    /* �ͷ��ս��ӽӿ��������ݿ顣                                         */
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
    /* �ͷ�diff_serv�������ݿ顣                                         */
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
    /* �ͷ�IPV6�鲥���ַL3���ݿ顣                                                */
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
    /* �ͷ�Diff-Serv����L2��VP�»�UNI�ࣩ�������ݿ顣                                         */
    /***************************************************************************/
    if (pstLogicalPort->ds_l2_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLogicalPort->ds_l2_cfg_cb, MEM_SPM_LOG_PORT_DS_L2_CB);
        pstLogicalPort->ds_l2_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstLogicalPort->spm_logical_port_handle, HDL_SPM_LOGICAL_PORT_CB);

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstLogicalPort, MEM_SPM_LOGICAL_PORT_CB);
    pstLogicalPort = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_alloc_terminal_if_cb
   ��������  : �˿��߼������ս��ӽӿڽṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_TERMINAL_IF_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��9��5��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_TERMINAL_IF_CB *spm_alloc_terminal_if_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;

    NBB_TRC_ENTRY("spm_alloc_terminal_if_cb");

    /* ����һ���µ�VPWS������Ŀ��*/
    pstTerminalIfCb = (SPM_TERMINAL_IF_CB *)NBB_MM_ALLOC(sizeof(SPM_TERMINAL_IF_CB), NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
    if (pstTerminalIfCb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:����pstTerminalIf��Ŀ�ռ�ΪNULL!"));
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPWS������Ŀ */
    OS_MEMSET(pstTerminalIfCb, 0, sizeof(SPM_TERMINAL_IF_CB));

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstTerminalIfCb->spm_terminal_if_handle = NBB_CREATE_HANDLE(pstTerminalIfCb, HDL_SPM_TERMINAL_IF_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_TERMINAL_IF_CB allocated at %p with handle %#lx",
            pstTerminalIfCb, pstTerminalIfCb->spm_terminal_if_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTerminalIfCb->spm_terminal_if_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstTerminalIfCb);
}


/*****************************************************************************
   �� �� ��  : spm_free_terminal_if_cb
   ��������  : �ͷŶ˿��߼������ս��ӽӿڽڵ���ڴ�ռ�
   �������  : (SPM_TERMINAL_IF_CB *pstTerminalIf
             NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��9��5��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_terminal_if_cb(SPM_TERMINAL_IF_CB *pstTerminalIfCb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_terminal_if_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTerminalIfCb, sizeof(SPM_TERMINAL_IF_CB), MEM_SPM_LOG_PORT_TERMINAL_IF_CB);

    if (pstTerminalIfCb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_TERMINAL_IF_CB at %p", pstTerminalIfCb));
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pstTerminalIfCb->terminal_if_cfg, 0, sizeof(ATG_DCI_LOG_PORT_VLAN));
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstTerminalIfCb->spm_terminal_if_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstTerminalIfCb, MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
    pstTerminalIfCb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_alloc_diff_serv_cb
   ��������  : �˿��߼�����diff_serv�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��1��22��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_DIFF_SERV_CB *spm_alloc_diff_serv_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_DIFF_SERV_CB *pstdiffservcb = NULL;

    NBB_TRC_ENTRY("spm_alloc_diff_serv_cb");

    /* ����һ���µ�VPWS������Ŀ��*/
    pstdiffservcb = (SPM_DIFF_SERV_CB *)NBB_MM_ALLOC(sizeof(SPM_DIFF_SERV_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    
    if (pstdiffservcb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:����pstdiffservcb��Ŀ�ռ�ΪNULL!"));
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPWS������Ŀ */
    OS_MEMSET(pstdiffservcb, 0, sizeof(SPM_DIFF_SERV_CB));

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    //pstdiffservcb->spm_diff_serv_handle = NBB_CREATE_HANDLE(pstdiffservcb, HDL_SPM_TERMINAL_IF_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_DIFF_SERV_CB allocated at %p with handle %#lx",
            //pstdiffservcb, pstdiffservcb->spm_diff_serv_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstdiffservcb->spm_diff_serv_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstdiffservcb);
}

/*****************************************************************************
   �� �� ��  : spm_free_diff_serv_cb
  ��������  : �ͷŶ˿��߼�����diff_serv�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��1��22��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_diff_serv_cb(SPM_DIFF_SERV_CB *pstdiffservcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_diff_serv_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstdiffservcb, sizeof(SPM_DIFF_SERV_CB), MEM_SPM_LOG_PORT_DIFF_SERV_CB);

    if (pstdiffservcb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_DIFF_SERV_CB at %p", pstdiffservcb));
    
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pstdiffservcb->diff_serv_cfg, 0, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    //NBB_DESTROY_HANDLE(pstdiffservcb->spm_diff_serv_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstdiffservcb, MEM_SPM_LOG_PORT_DIFF_SERV_CB);
    pstdiffservcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_alloc_flow_diff_serv_cb
   ��������  : �˿��߼�����diff_serv�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_FLOW_DIFF_SERV_CB *spm_alloc_flow_diff_serv_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_FLOW_DIFF_SERV_CB *pstflowdiffservcb = NULL;

    NBB_TRC_ENTRY("spm_alloc_flow_diff_serv_cb");

    /* ����һ���µ�VPWS������Ŀ��*/
    pstflowdiffservcb = (SPM_FLOW_DIFF_SERV_CB *)NBB_MM_ALLOC(sizeof(SPM_FLOW_DIFF_SERV_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    
    if (pstflowdiffservcb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:����pstflowdiffservcb��Ŀ�ռ�ΪNULL!"));
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPWS������Ŀ */
    OS_MEMSET(pstflowdiffservcb, 0, sizeof(SPM_FLOW_DIFF_SERV_CB));

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    //pstdiffservcb->spm_diff_serv_handle = NBB_CREATE_HANDLE(pstdiffservcb, HDL_SPM_TERMINAL_IF_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_DIFF_SERV_CB allocated at %p with handle %#lx",
            //pstdiffservcb, pstdiffservcb->spm_diff_serv_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstflowdiffservcb->spm_flow_diff_serv_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstflowdiffservcb);
}

/*****************************************************************************
   �� �� ��  : spm_free_flow_diff_serv_cb
  ��������  : �ͷŶ˿��߼�����diff_serv�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_flow_diff_serv_cb(SPM_FLOW_DIFF_SERV_CB *pstflowdiffservcb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_flow_diff_serv_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstflowdiffservcb, sizeof(SPM_FLOW_DIFF_SERV_CB), MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);

    if (pstflowdiffservcb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_FLOW_DIFF_SERV_CB at %p", pstflowdiffservcb));
    
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pstflowdiffservcb->flow_diff_serv_cfg, 0, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV));
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    //NBB_DESTROY_HANDLE(pstdiffservcb->spm_diff_serv_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstflowdiffservcb, MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
    pstflowdiffservcb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_alloc_inclassify_qos_cb
   ��������  : �˿��߼�����diff_serv�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���(SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(pstLogicalPort->inclassify_qos_tree,

*****************************************************************************/
SPM_INCLASSIFY_QOS_CB *spm_alloc_inclassify_qos_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_INCLASSIFY_QOS_CB *pstinclassifyqoscb = NULL;

    NBB_TRC_ENTRY("spm_alloc_inclassify_qos_cb");

    /* ����һ���µ�VPWS������Ŀ��*/
    pstinclassifyqoscb = (SPM_INCLASSIFY_QOS_CB *)NBB_MM_ALLOC(sizeof(SPM_INCLASSIFY_QOS_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    
    if (pstinclassifyqoscb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:����pstinclassifyqoscb��Ŀ�ռ�ΪNULL!"));
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPWS������Ŀ */
    OS_MEMSET(pstinclassifyqoscb, 0, sizeof(SPM_INCLASSIFY_QOS_CB));

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    //pstdiffservcb->spm_diff_serv_handle = NBB_CREATE_HANDLE(pstdiffservcb, HDL_SPM_TERMINAL_IF_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_DIFF_SERV_CB allocated at %p with handle %#lx",
            //pstdiffservcb, pstdiffservcb->spm_diff_serv_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstinclassifyqoscb->spm_inclassify_qos_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstinclassifyqoscb);
}

/*****************************************************************************
   �� �� ��  : spm_free_inclassify_qos_cb
  ��������  : �ͷŶ˿��߼�����diff_serv�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_inclassify_qos_cb(SPM_INCLASSIFY_QOS_CB *pstinclassifyqoscb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_inclassify_qos_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstinclassifyqoscb, sizeof(SPM_INCLASSIFY_QOS_CB), MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);

    if (pstinclassifyqoscb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_INCLASSIFY_QOS_CB at %p", pstinclassifyqoscb));
    
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&pstinclassifyqoscb->inclassify_qos_cfg, 0, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS));
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    //NBB_DESTROY_HANDLE(pstdiffservcb->spm_diff_serv_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(pstinclassifyqoscb, MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
    pstinclassifyqoscb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_alloc_traffic_filter_cb
   ��������  : �˿��߼�����diff_serv�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :(SPM_TRAFFIC_FILTER_CB*) AVLL_NEXT(pstLogicalPort->traffic_filter_tree,

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_TRAFFIC_FILTER_CB *spm_alloc_traffic_filter_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_TRAFFIC_FILTER_CB *psttrafficfiltercb = NULL;

    NBB_TRC_ENTRY("spm_alloc_traffic_filter_cb");

    /* ����һ���µ�VPWS������Ŀ��*/
    psttrafficfiltercb = (SPM_TRAFFIC_FILTER_CB *)NBB_MM_ALLOC(sizeof(SPM_TRAFFIC_FILTER_CB), 
        NBB_NORETRY_ACT, MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    
    if (psttrafficfiltercb == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:����psttrafficfiltercb��Ŀ�ռ�ΪNULL!"));
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPWS������Ŀ */
    OS_MEMSET(psttrafficfiltercb, 0, sizeof(SPM_TRAFFIC_FILTER_CB));

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    //pstdiffservcb->spm_diff_serv_handle = NBB_CREATE_HANDLE(pstdiffservcb, HDL_SPM_TERMINAL_IF_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    //NBB_TRC_DETAIL((NBB_FORMAT "SPM_DIFF_SERV_CB allocated at %p with handle %#lx",
            //pstdiffservcb, pstdiffservcb->spm_diff_serv_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(psttrafficfiltercb->spm_traffic_filter_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(psttrafficfiltercb);
}

/*****************************************************************************
   �� �� ��  : spm_free_traffic_filter_cb
  ��������  : �ͷŶ˿��߼�����diff_serv�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_DIFF_SERV_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2015��3��9��
    ��    ��   : fansongbo
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_traffic_filter_cb(SPM_TRAFFIC_FILTER_CB *psttrafficfiltercb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_traffic_filter_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(psttrafficfiltercb, sizeof(SPM_TRAFFIC_FILTER_CB), MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);

    if (psttrafficfiltercb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_TRAFFIC_FILTER_CB at %p", psttrafficfiltercb));
    
    //NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pstLogicalPort->port_index_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                    */
    /***************************************************************************/
    OS_MEMSET(&psttrafficfiltercb->traffic_filter_cfg, 0, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER));
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    //NBB_DESTROY_HANDLE(pstdiffservcb->spm_diff_serv_handle, HDL_SPM_TERMINAL_IF_CB);

    /***************************************************************************/
    /* �����ͷŶ˿��߼����ÿ��ƿ顣                                            */
    /***************************************************************************/
    NBB_MM_FREE(psttrafficfiltercb, MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
    psttrafficfiltercb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_free_all_logical_port
   ��������  : �ͷŶ˿��߼��������е��ڴ�ռ�
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��11��6��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

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
   �� �� ��  : spm_alloc_flow_node_cb
   ��������  : ���ڵ�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_FLOW_NODE_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��10��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_FLOW_NODE_CB *spm_alloc_flow_node_cb()
{
    SPM_FLOW_NODE_CB *pstFlowNode = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_flow_node_cb");

    /* ����һ���µĽӿ�����������Ŀ��*/
    pstFlowNode = (SPM_FLOW_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_FLOW_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_FLOW_NODE_CB);
    if (pstFlowNode == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ���ӿ�����������Ŀ */
    OS_MEMSET(pstFlowNode, 0, sizeof(SPM_FLOW_NODE_CB));

    /* �������ڸýӿ�����������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstFlowNode->flow_tree_handle = NBB_CREATE_HANDLE(pstFlowNode, HDL_SPM_FLOW_NODE_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_FLOW_NODE_CB allocated at %p with handle %#lx",
            pstFlowNode, pstFlowNode->flow_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstFlowNode->flow_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstFlowNode);
}

/*****************************************************************************
   �� �� ��  : spm_alloc_ipv4_node_cb
   ��������  : ipv4���ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_IPV4_NODE_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��10��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_IPV4_NODE_CB *spm_alloc_ipv4_node_cb()
{
    SPM_IPV4_NODE_CB *pstIpv4Node = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_ipv4_node_cb");

    /* ����һ���µĽӿ�����������Ŀ��*/
    pstIpv4Node = (SPM_IPV4_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_IPV4_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_IPV4_NODE_CB);
    if (pstIpv4Node == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ���ӿ�����������Ŀ */
    OS_MEMSET(pstIpv4Node, 0, sizeof(SPM_IPV4_NODE_CB));

    /* �������ڸýӿ�����������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstIpv4Node->ipv4_tree_handle = NBB_CREATE_HANDLE(pstIpv4Node, HDL_SPM_IPV4_NODE_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_IPV4_NODE_CB allocated at %p with handle %#lx",
            pstIpv4Node, pstIpv4Node->ipv4_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstIpv4Node->ipv4_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstIpv4Node);
}

/*****************************************************************************
   �� �� ��  : spm_alloc_ipv6_node_cb
   ��������  : ipv6�ڵ�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_IPV6_NODE_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��10��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_IPV6_NODE_CB *spm_alloc_ipv6_node_cb()
{
    SPM_IPV6_NODE_CB *pstIpv6Node = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_ipv6_node_cb");

    /* ����һ���µĽӿ�����������Ŀ��*/
    pstIpv6Node = (SPM_IPV6_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_IPV6_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_IPV6_NODE_CB);
    if (pstIpv6Node == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ���ӿ�����������Ŀ */
    OS_MEMSET(pstIpv6Node, 0, sizeof(SPM_IPV6_NODE_CB));

    /* �������ڸýӿ�����������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstIpv6Node->ipv6_tree_handle = NBB_CREATE_HANDLE(pstIpv6Node, HDL_SPM_IPV6_NODE_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_IPV6_NODE_CB allocated at %p with handle %#lx",
            pstIpv6Node, pstIpv6Node->ipv6_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstIpv6Node->ipv6_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstIpv6Node);
}

/*****************************************************************************
   �� �� ��  : spm_alloc_mc_mac_node_cb
   ��������  : �鲥��ַ�ڵ�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_MC_MAC_NODE_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��10��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_MC_MAC_NODE_CB *spm_alloc_mc_mac_node_cb()
{
    SPM_MC_MAC_NODE_CB *pstMcMacNode = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_mc_mac_node_cb");

    /* ����һ���µĽӿ�����������Ŀ��*/
    pstMcMacNode = (SPM_MC_MAC_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_MC_MAC_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_MC_MAC_NODE_CB);
    if (pstMcMacNode == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ���ӿ�����������Ŀ */
    OS_MEMSET(pstMcMacNode, 0, sizeof(SPM_MC_MAC_NODE_CB));

    /* �������ڸýӿ�����������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstMcMacNode->mc_mac_tree_handle = NBB_CREATE_HANDLE(pstMcMacNode, HDL_SPM_MC_IPV4_NODE_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_MC_MAC_NODE_CB allocated at %p with handle %#lx",
            pstMcMacNode, pstMcMacNode->mc_mac_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstMcMacNode->mc_mac_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstMcMacNode);
}

/*****************************************************************************
   �� �� ��  : spm_alloc_vip_vmac_node_cb
   ��������  : ����MAC��IP��ַ�ڵ�ṹ�ڴ�ռ����
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_VIP_VMAC_NODE_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��10��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_VIP_VMAC_NODE_CB *spm_alloc_vip_vmac_node_cb()
{
    SPM_VIP_VMAC_NODE_CB *pstVipVmacNode = NULL;

    //NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_alloc_vip_vmac_node_cb");

    /* ����һ���µĽӿ�����������Ŀ��*/
    pstVipVmacNode = (SPM_VIP_VMAC_NODE_CB *)NBB_MM_ALLOC(sizeof(SPM_VIP_VMAC_NODE_CB),
        NBB_NORETRY_ACT,
        MEM_SPM_VIP_VMAC_NODE_CB);
    if (pstVipVmacNode == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ���ӿ�����������Ŀ */
    OS_MEMSET(pstVipVmacNode, 0, sizeof(SPM_VIP_VMAC_NODE_CB));

    /* �������ڸýӿ�����������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstVipVmacNode->vip_vmac_tree_handle = NBB_CREATE_HANDLE(pstVipVmacNode, HDL_SPM_VIP_VMAC_NODE_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VIP_VMAC_NODE_CB allocated at %p with handle %#lx",
            pstVipVmacNode, pstVipVmacNode->vip_vmac_tree_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstVipVmacNode->vip_vmac_tree_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstVipVmacNode);
}

#endif

#endif

