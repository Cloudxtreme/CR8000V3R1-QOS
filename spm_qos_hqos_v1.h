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

/*HQOS最大LSP条目数*/
#define HQOS_MIN_LSP_ID 85

/*HQOS最大LSP条目数*/
#define HQOS_MAX_LSP_ID 4096

/*HQOS最大pw条目数*/
#define HQOS_MAX_PW_ID   4096

/**STRUCT*********************************************************************/
/* Structure: SPM_QOS_PORT_CB                                           */
/*                                                                           */
/* Description: qos物理端口        */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_qos_port_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_port_node;

    /* key值: 物理端口*/
    NBB_USHORT port_key;

    /* LSP policy索引*/
    AVLL_TREE lsp_tree;


    //AVLL_TREE pw_vrf_tree;
    //AVLL_TREE pw_vc_tree;
    //AVLL_TREE group_tree;

} SPM_QOS_PORT_CB;


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



/* QoS模块相关信息 */
typedef struct spm_hqos_user_group_cb
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

    ATG_DCI_TX_LSP_QOS data;//全FF代表删除不限速

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

    /* key值: index */
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
