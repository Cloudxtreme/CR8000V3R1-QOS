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


#ifdef PTN690
#define MAX_SLOT_NUM        42
#define MAX_MSP_ID          64
#define MAX_METER_NUM     (32 * 1024)
#else
#define MAX_SLOT_NUM        13
#define MAX_METER_NUM     (8 * 1024)
#endif

#define MAX_METER_BUKECT_NUM                        (256)
#define MAX_PHYSIC_PORT                             128
#define MIN_PHYSIC_PORT                             0
#define MAX_PHYSIC_PORT_NUM                             128
#define VOQ_OFFSET          40
#define NUM_COS             8
#define HQOS_OFFSET          (MAX_SLOT_NUM * NUM_COS * MAX_PHYSIC_PORT_NUM + VOQ_OFFSET)
#define MAX_NODE_ID          2048
#define MAX_HQOS_SLOT_NUM    4


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

    NBB_ULONG pw_id;/*Ĭ�ϵ�PW ID*/

    NBB_BYTE slot;

    NBB_USHORT port;

    /***************************************************************************/
    /* SDK resource    time sorted                                             */
    /***************************************************************************/
    NBB_ULONG pw_id;//ARAD
    NBB_LONG voq_id;//ARAD
    NBB_LONG tm_vc;//ARAD
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM];//ARAD
    NBB_LONG np_flow_id;//c3
    NBB_LONG posid;//c3


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