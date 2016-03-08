/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos.h
   �� �� ��   : ����
   ��    ��   : zenglu
   ��������   : 2012��9��18��
   ����޸�   :
   ��������   : QOS����
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��9��18��
    ��    ��   : zenglu
    �޸�����   : �����ļ�
******************************************************************************/

#ifndef SPM_QOS_HQOS_H
#define SPM_QOS_HQOS_H



#define MEM_SPM_USER_GROUP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002700))
#define MEM_SPM_BASIC_USER_GROUP_CB                ((NBB_LONG)(PCT_SPM | 0x00002701))
#define MEM_SPM_UP_USER_GROUP_CB                ((NBB_LONG)(PCT_SPM | 0x00002702))
#define MEM_SPM_DOWN_USER_GROUP_CB                ((NBB_LONG)(PCT_SPM | 0x00002703))


#define MEM_SPM_HQOS_LSP_TX_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111D))
#define MEM_SPM_HQOS_VRF_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111E))
#define MEM_SPM_HQOS_VC_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111F))
#define MEM_SPM_HQOS_LOG_USR_CB                     ((NBB_LONG)(PCT_SPM | 0x00001120))

/*HQOS���LSP��Ŀ��*/
#define HQOS_MIN_LSP_ID 85

/*HQOS���LSP��Ŀ��*/
#define HQOS_MAX_LSP_ID 4096

/*HQOS���pw��Ŀ��*/
#define HQOS_MAX_PW_ID   4096

/**STRUCT*********************************************************************/
/* Structure: SPM_QOS_PORT_CB                                           */
/*                                                                           */
/* Description: qos����˿�        */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_qos_port_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_port_node;

    /* keyֵ: ����˿�*/
    NBB_USHORT port_key;

    /* LSP policy����*/
    AVLL_TREE lsp_tree;


    //AVLL_TREE pw_vrf_tree;
    //AVLL_TREE pw_vc_tree;
    //AVLL_TREE group_tree;

} SPM_QOS_PORT_CB;


/* QoSģ�������Ϣ */
typedef struct spm_hqos_usr_cb
{

    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_usr_node;

    /***************************************************************************/
    /* �߼��˿�keyֵ                                        */
    /***************************************************************************/
    NBB_ULONG index;

    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS data;

    /***************************************************************************/
    /* SDK resource    time sorted                                             */
    /***************************************************************************/
    NBB_ULONG pw_id;//ARAD
    NBB_LONG voq_id;//ARAD
    NBB_LONG tm_vc;//ARAD
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM];//ARAD
    NBB_LONG np_flow_id;//c3
    TX_PORT_T l3uni;
} SPM_HQOS_LOG_USR_CB;



/* QoSģ�������Ϣ */
typedef struct spm_hqos_user_group_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_user_group_node;

    /* keyֵ: index */
    NBB_ULONG user_group_key;

    AVLL_TREE usr_tree;

    //ATG_DCI_USER_GROUP_BASIC_DATA *basic_cfg_cb;

    //ATG_DCI_USER_GROUP_UP_QOS *up_cfg_cb;

    ATG_DCI_TX_LSP_QOS data;//ȫFF����ɾ��������

    NBB_ULONG lsp_id;

    NBB_BYTE slot;

    NBB_USHORT port;

} SPM_HQOS_USER_GROUP_CB;




typedef struct spm_hqos_lsp_cb
{

    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_lsp_tx_node;

    SPM_QOS_TUNNEL_KEY lsp_tx_key;

    AVLL_TREE vc_tree;

    AVLL_TREE vrf_tree;

    NBB_ULONG lsp_id;

    NBB_BYTE slot;

    NBB_USHORT port;

    ATG_DCI_TX_LSP_QOS data;

    /***************************************************************************/
    /* SDK resource    time sorted                                             */
    /***************************************************************************/
    NBB_ULONG pw_id;//ARAD ����λ��0
    NBB_LONG voq_id;//ARAD 
    NBB_LONG tm_vc;//ARAD ����λ��0
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM];//ARAD ����λ��0
    NBB_LONG np_flow_id;//c3 ���е��̶���
    NBB_LONG posid;//c3 ���е��̶���


}SPM_HQOS_LSP_CB;

typedef struct spm_hqos_vc_cb
{

    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_vc_node;

    /***************************************************************************/
    /* keyֵ: VC ID+ VC_TYPE +PEER_IP.                                         */
    /***************************************************************************/
    ATG_DCI_VC_KEY vc_key;

    ATG_DCI_TX_LSP_QOS vc_data;

    /***************************************************************************/
    /* SDK resource    time sorted                                             */
    /***************************************************************************/
    NBB_ULONG pw_id;//ARAD
    NBB_LONG voq_id;//ARAD
    NBB_LONG tm_vc;//ARAD
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM];//ARAD
    NBB_LONG np_flow_id;//c3
    NBB_LONG posid;//c3
    NBB_BYTE proflag;//c3


} SPM_HQOS_VC_CB;

typedef struct spm_hqos_vrf_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_vrf_node;

    /* keyֵ: index */
    SPM_QOS_VRF_INSTANSE_KEY vrf_key;

    ATG_DCI_TX_LSP_QOS vrf_data;

    /***************************************************************************/
    /* SDK resource    time sorted                                             */
    /***************************************************************************/
    NBB_ULONG pw_id;//ARAD
    NBB_LONG voq_id;//ARAD
    NBB_LONG tm_vc;//ARAD
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM];//ARAD
    NBB_LONG np_flow_id;//c3
    NBB_LONG posid;//c3
   
} SPM_HQOS_VRF_CB;


NBB_LONG spm_hqos_add_lsp_node(NBB_USHORT slot,
    NBB_LONG port,
    SPM_QOS_TUNNEL_KEY *lsp_key,
    ATG_DCI_TX_LSP_QOS *data,
    NBB_ULONG posid);

NBB_LONG spm_hqos_del_lsp_node(SPM_HQOS_LSP_CB **plsp);

NBB_LONG spm_hqos_add_vc_node(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid, NBB_BYTE proflag,
    SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VC_KEY *vc_key,
    ATG_DCI_VC_UP_VPN_QOS_POLICY *data);

NBB_LONG spm_hqos_del_vc_node(SPM_HQOS_LSP_CB **plsp,SPM_HQOS_VC_CB **pcb);

NBB_LONG spm_hqos_add_vrf_node(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
    SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VRF_INSTANCE_KEY *vrf_key,
    ATG_DCI_VRF_INSTANCE_UP_VPN_QOS *data);

NBB_LONG spm_hqos_del_vrf_node(SPM_HQOS_LSP_CB **plsp,SPM_HQOS_VRF_CB **pcb);

NBB_LONG spm_hqos_add_usr_node(SUB_PORT *sub_port,NBB_ULONG logic_key,
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *data);

NBB_LONG spm_hqos_del_usr_node(SPM_HQOS_USER_GROUP_CB *group,SPM_HQOS_LOG_USR_CB **pcb);

NBB_VOID spm_rcv_dci_set_user_group(ATG_DCI_SET_USER_GROUP *pst_set_ips);

NBB_VOID spm_rcv_dci_set_hqos(ATG_DCI_SET_HQOS *pstSetIps);

NBB_LONG spm_qos_hqos_init(void);

#endif
