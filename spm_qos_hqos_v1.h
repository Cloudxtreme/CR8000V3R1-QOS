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

#ifndef SPM_QOS_WRED_H
#define SPM_QOS_WRED_H







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

    /*PW id*/
    NBB_ULONG pw_id;

    NBB_LONG flow_id;

    ATG_DCI_VPN_HQOS_POLICY data;
} SPM_HQOS_LOG_USR_CB;



/* QoSģ�������Ϣ */
typedef struct spm_qos_user_group_cb
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

    ATG_DCI_VPN_HQOS_POLICY data;//ȫFF����ɾ��������

    NBB_ULONG lsp_id;

    NBB_BYTE slot;

    NBB_USHORT port;

} SPM_QOS_USER_GROUP_CB;




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

    ATG_DCI_VPN_HQOS_POLICY data;

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

    ATG_DCI_VPN_HQOS_POLICY vc_data;

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

    ATG_DCI_VPN_HQOS_POLICY vrf_data;

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

#endif
