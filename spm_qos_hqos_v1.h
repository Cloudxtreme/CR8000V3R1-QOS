/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos.h
   版 本 号   : 初稿
   作    者   : zenglu
   生成日期   : 2012年9月18日
   最近修改   :
   功能描述   : QOS处理
   函数列表   :
   修改历史   :
   1.日    期   : 2012年9月18日
    作    者   : zenglu
    修改内容   : 创建文件
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

    NBB_ULONG pw_id;/*默认的PW ID*/

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
    /* key值: VC ID+ VC_TYPE +PEER_IP.                                         */
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

    /* key值: index */
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