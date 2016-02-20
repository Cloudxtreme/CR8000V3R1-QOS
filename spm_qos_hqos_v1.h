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







/* QoS模块相关信息 */
typedef struct spm_hqos_usr_cb
{

    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_usr_node;

    /***************************************************************************/
    /* 逻辑端口key值                                        */
    /***************************************************************************/
    NBB_ULONG index;

    /*PW id*/
    NBB_ULONG pw_id;

    NBB_LONG flow_id;

    ATG_DCI_VPN_HQOS_POLICY data;
} SPM_HQOS_LOG_USR_CB;



/* QoS模块相关信息 */
typedef struct spm_qos_user_group_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_user_group_node;

    /* key值: index */
    NBB_ULONG user_group_key;

    AVLL_TREE usr_tree;

    //ATG_DCI_USER_GROUP_BASIC_DATA *basic_cfg_cb;

    //ATG_DCI_USER_GROUP_UP_QOS *up_cfg_cb;

    ATG_DCI_VPN_HQOS_POLICY data;//全FF代表删除不限速

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
    NBB_ULONG pw_id;//ARAD 出槽位非0
    NBB_LONG voq_id;//ARAD 
    NBB_LONG tm_vc;//ARAD 出槽位非0
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM];//ARAD 出槽位非0
    NBB_LONG np_flow_id;//c3 所有单盘都有
    NBB_LONG posid;//c3 所有单盘都有


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
