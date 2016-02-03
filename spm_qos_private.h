/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_private.h
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
#ifndef SPM_QOS_PRIVATE_INCLUDE
#define SPM_QOS_PRIVATE_INCLUDE

#include "spm_qos_ver_control.h"
#include "spm_qos_ds_v1.h"
#include <spm_qos.h>


#define QOS_PD              25

#define QOS_CFG_STRING       "QOS CFG"
#define HQOS_LSP_CFG         "HQOS LSP CFG"
#define HQOS_LSP_ERROR       "***HQOS LSP ERROR***"
#define HQOS_LSP_WARNING     "***HQOS LSP WARNING***"
#define HQOS_VRF_CFG         "HQOS VRF CFG"
#define HQOS_VRF_ERROR       "***HQOS VRF ERROR***"
#define HQOS_VC_CFG         "HQOS VC CFG"
#define HQOS_VC_ERROR       "***HQOS VC ERROR***"
#define HQOS_USR_CFG         "HQOS USR CFG"
#define HQOS_USR_ERROR       "***HQOS USR ERROR***"
#define HQOS_GROUP_CFG         "HQOS GROUP CFG"
#define HQOS_GROUP_ERROR       "***HQOS GROUP ERROR***"
#define HQOS_ARAD_ERROR       "***HQOS ARAD ERROR***"
#define HQOS_C3_ERROR       "***HQOS C3 ERROR***"
#define HQOS_BMU_ERROR       "***HQOS BMU ERROR***"
#define QOS_ERROR_STRING     "**Qos ERROR**"
#define QOS_MSG_INFO_LEN    300 

#define VOQ_OFFSET          40


#define NUM_COS             8

#define HQOS_OFFSET          (MAX_SLOT_NUM * NUM_COS * MAX_PHYSIC_PORT_NUM + VOQ_OFFSET)

#define MAX_NODE_ID          2048
#define MAX_HQOS_SLOT_NUM    4

#define MAX_DS_NUM          4097


#define MAX_PHB_NUM         8
#define PHB_BE              0
#define PHB_AF1             1
#define PHB_AF2             2
#define PHB_AF3             3
#define PHB_AF4             4
#define PHB_EF              5
#define PHB_CS6             6
#define PHB_CS7             7

#define MAX_COLOR_NUM       3
#define QOS_COLOR_GREEN     0
#define QOS_COLOR_YELLOW        1
#define QOS_COLOR_RED       2

#define MAX_VLAN_PRI_NUM    8

#define MAX_DSCP_PRI_NUM    64

#define MAX_EXP_PRI_NUM     8

#define PRI2PHB_ASSIGN_OFFSET      16
#define PHB2PRI_ASSIGN_OFFSET      16

#define WRED_MODE_QUE_DROP        0
#define WRED_MODE_COL_BIND        1
#define WRED_MODE_COLOR           2



#define QOS_RCU_PORT    (104 + PTN_690_PORT_OFFSET)


#define CLASSIFY_HIGH_ACL_ID  5000
#define CLASSIFY_ETH_ACL_ID   1801
#define CLASSIFY_PORT_ACL_ID  901


#define PRI2PHB_ASSIGN              0
#define PRI2PHB_MAP_DSCP                  1
#define PRI2PHB_MAP_OVLAN                2
#define PRI2PHB_MAP_EXP                     8

#define PHB2PRI_NOT                              0
#define PHB2PRI_DSCP                            1
#define PHB2PRI_OVLAN                          2
#define PHB2PRI_DSCP_AND_VLAN      3
#define PHB2PRI_EXP                                4
#define PHB2PRI_INHERIT_EXP               5
#define PHB2PRI_MAP                                16

#define LABEL_ACTION_SWAP           0
#define LABEL_ACTION_SWAP_PUSH      1
#define LABEL_ACTION_POP_GO         2   /*LSP 标签的pop*/
#define LABEL_ACTION_PW_POP_GO    3


#define QOS_MAIN     1
#define QOS_BACK     2
#define QOS_ECMP     3


#define FTN_TYPE   1
#define LSP_TX_TYPE 2
#define IP_LSP_TYPE 3

#define QOS_EXIST         1
#define QOS_UNEXIST       0


#define MATCH_ERROR         0
#define MATCH_PORT          1
#define MATCH_ETH_HEADER    2   
#define MATCH_IPV4          3
#define MATCH_IP_TCP        9   /*高级ACL*/
#define MATCH_IP_UDP        10
#define MATCH_IPV6          19
#define MATCH_PORT_VLAN     39
#define MATCH_NOIP          40
#define MATCH_ACL_ID        50

#define ACTION_DROP         0
#define ACTION_PERMIT       1 
#define ACTION_SAMPLE       2
#define ACTION_REDIRECT     3
#define ACTION_POLICY_ROUTE 4

#define POLICY_C2B_INSTANCE_NUM 128
#define POLICY_C2B_INSTANCE_MAX 2




/**************防攻击宏***********************/
#define DEFEND_ACL_ID_OFFSET 4899
#define DEFEND_ACL_ID_APPRE_OFFSET 4920
#define DEFEND_ACL_ID_USR_OFFSET 4940

#define DEFEND_ABNORMAL_RULE_OFFSET 10000
#define DEFEND_UDP_RULE_OFFSET 11100

#define DEFEND_ABNORMAL_RULE_NUM 4
#define DEFEND_UDP_RULE_NUM 6


#define DEFEND_OSPF              1
#define DEFEND_ISIS             (DEFEND_OSPF + 1)
#define DEFEND_BGP             (DEFEND_ISIS + 1)
#define DEFEND_LDP            (DEFEND_BGP + 1)
#define DEFEND_LDPUDP             (DEFEND_LDP + 1)
#define DEFEND_RSVP             (DEFEND_LDPUDP + 1)
#define DEFEND_RIP              (DEFEND_RSVP + 1)
#define DEFEND_MSDP             (DEFEND_RIP + 1)
#define DEFEND_PIM             (DEFEND_MSDP + 1)
#define DEFEND_SNMP             (DEFEND_PIM + 1)
#define DEFEND_RADIUS             (DEFEND_SNMP + 1)
#define DEFEND_LSP_PING             (DEFEND_RADIUS + 1)
#define DEFEND_IGMP             (DEFEND_LSP_PING + 1)
#define DEFEND_ICMP             (DEFEND_IGMP + 1)
#define DEFEND_VRRP             (DEFEND_ICMP + 1)
#define DEFEND_DHCP             (DEFEND_VRRP + 1)
#define DEFEND_LACP             (DEFEND_DHCP + 1)
#define DEFEND_BFD             (DEFEND_LACP + 1)
#define DEFEND_TACACS             (DEFEND_BFD + 1)
#define DEFEND_NTP             (DEFEND_TACACS + 1)
#define DEFEND_FTP_S             (DEFEND_NTP + 1)
#define DEFEND_FTP_C             (DEFEND_FTP_S + 1)
#define DEFEND_TELNET_S             (DEFEND_FTP_C + 1)
#define DEFEND_TELNET_C             (DEFEND_TELNET_S + 1)
#define DEFEND_SSH_S            (DEFEND_TELNET_C + 1)
#define DEFEND_SSH_C             (DEFEND_SSH_S + 1)
#define DEFEND_OSPFV3             (DEFEND_SSH_C + 1)
#define DEFEND_MLD              (DEFEND_OSPFV3 + 1)
#define DEFEND_BGPV6            (DEFEND_MLD + 1)
#define DEFEND_RIPNG            (DEFEND_BGPV6 + 1)
#define DEFEND_ICMPV6             (DEFEND_RIPNG + 1)
#define DEFEND_VRRPV6             (DEFEND_ICMPV6 + 1)
#define DEFEND_DHCPV6             (DEFEND_VRRPV6 + 1)
#define DEFEND_PIMV6             (DEFEND_DHCPV6 + 1)
#define DEFEND_UNKNOWN_ARP             (DEFEND_PIMV6 + 1)
#define DEFEND_UNKNOWN_MULICAST             (DEFEND_UNKNOWN_ARP + 1)
#define DEFEND_TCPSYN             (DEFEND_UNKNOWN_MULICAST + 1)
#define DEFEND_DEFAULT             0xff
   
#define DEFEND_DEFAULT_RULE_ID             10000

/* QoS模块相关信息 */
typedef struct spm_qos_pri2phb_map
{
    NBB_BYTE phb;

    NBB_BYTE color;

}SPM_QOS_PRI2PHB_MAP;

/* 内部数据tree */
typedef struct spm_qos_ds_logic_intf_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_logic_node;

    /* key值: index */
    SPM_QOS_LOGIC_INTF_KEY logic_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

    SUB_PORT sub_port;
    NBB_LONG posid;

    /*驱动底层信息*/
    NBB_BYTE in_mode;/*0/1:pri/dscp*/
    NBB_BYTE out_mode;/*(0-不动作，1-pri，2-pri+dscp)*/
    NBB_LONG in_dsptr;
    NBB_LONG out_dsptr;
} SPM_QOS_DS_LOGIC_INTF_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_logic_flow_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_logic_node;


    /* key值: index */
    SPM_QOS_LOGIC_FLOW_KEY flow_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

    SUB_PORT sub_port;
    NBB_LONG posid;

    NBB_BYTE in_mode;
    NBB_LONG in_dsptr;

} SPM_QOS_DS_LOGIC_FLOW_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_logic_univp_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_logic_univp_node;


    /* key值: index */
    NBB_ULONG logic_index;

    SPM_QOS_LOGIC_UNIVP vpuni;

    /*ds 索引*/
    NBB_ULONG ds_index;
    NBB_BYTE out_mode;
    NBB_LONG out_dsptr;

} SPM_QOS_DS_LOGIC_UNIVP_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_vc_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_vc_node;

    /***************************************************************************/
    /* key值: VC ID+ VC_TYPE +PEER_IP.                                         */
    /***************************************************************************/
    ATG_DCI_VC_KEY vc_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

    NBB_ULONG posid;
    NBB_USHORT flag;

    /*底层信息*/
    NBB_BYTE in_mode;
    NBB_LONG in_dsptr;
    NBB_BYTE out_mode;
    NBB_LONG out_dsptr;
} SPM_QOS_DS_VC_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_vrf_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_vrf_node;

    /***************************************************************************/
    /* key值: VC ID+ VC_TYPE +PEER_IP.                                         */
    /***************************************************************************/
    SPM_QOS_VRF_INSTANSE_KEY vrf_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

    NBB_LONG nhi;
    NBB_LONG dsptr;
    NBB_BYTE out_mode;
} SPM_QOS_DS_VRF_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_ilm_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_ilm_node;


    /***************************************************************************/
    /* key值                                     */
    /***************************************************************************/
    SPM_QOS_ILM_KEY ilm_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

    NBB_ULONG posid; 
    NBB_ULONG ecmpid; 
    NBB_BYTE node_action;
    NBB_ULONG ds_index2;
    SPM_QOS_TUNNEL_KEY key_tun2;

    NBB_BYTE in_mode;
    NBB_LONG in_dsptr;
    NBB_BYTE out_mode;
    NBB_LONG out_dsptr;
} SPM_QOS_DS_ILM_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_ftn_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_ftn_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_qos_ftn_handle;

    /***************************************************************************/
    /* key值                                     */
    /***************************************************************************/
    FTN_KEY ftn_key;

    /*ds 索引*/
    NBB_ULONG ds_index;
} SPM_QOS_DS_FTN_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_rxlsp_key
{
    CRRXLSP_KEY rxlspkey;
    NBB_USHORT flag;/*主备标识*/
}SPM_QOS_DS_RXLSP_KEY;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_rx_lsp_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_rx_lsp_node;

    /***************************************************************************/
    /* key值                                     */
    /***************************************************************************/
    SPM_QOS_DS_RXLSP_KEY rx_lsp_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

    NBB_ULONG posid; 
    NBB_BYTE node_action;
    NBB_ULONG ds_index2;
    SPM_QOS_TUNNEL_KEY key_tun2;

    NBB_BYTE in_mode;
    NBB_LONG in_dsptr;
    NBB_BYTE out_mode;
    NBB_LONG out_dsptr;
} SPM_QOS_DS_RX_LSP_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_tx_lsp_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_tx_lsp_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_qos_tx_lsp_handle;

    /***************************************************************************/
    /* key值                                     */
    /***************************************************************************/
    CRTXLSP_KEY tx_lsp_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

} SPM_QOS_DS_TX_LSP_CB;

#ifdef PTN690

/* QoS模块相关信息 */
typedef struct spm_qos_txlsp_car_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_txlsp_car_node;

    /* key值: txlsp */
    CRTXLSP_KEY txlsp_key;

    ATG_DCI_CR_LSP_TX_CAR_DATA txlsp_car;

    /*meter_id*/
    NBB_ULONG meter_id;

    NBB_ULONG color_action_id;

    NBB_ULONG txlsp_posid;

} SPM_QOS_TXLSP_CAR_CB;


#endif


#if defined (CR8000_SMART) || defined (PTN690)
/* QoS模块相关信息 */
typedef struct spm_qos_pw_car_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_pw_car_node;

    /* key值: log index */
    NBB_ULONG log_key;

    ATG_DCI_LOG_PORT_CAR_DATA pw_car;

    ATG_DCI_VC_KEY vc_key;

    /*meter_id*/
    NBB_ULONG meter_id;

    NBB_ULONG color_action_id;

} SPM_QOS_PW_CAR_CB;
#endif

/* QoS模块相关信息 */
typedef struct spm_qos_ds_tunnel_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_tunnel_node;

    /***************************************************************************/
    /* key值                                     */
    /***************************************************************************/
    SPM_QOS_TUNNEL_KEY tunnel_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

    NBB_ULONG posid; 
    NBB_BYTE out_mode;
    NBB_LONG out_dsptr;  
} SPM_QOS_DS_TUNNEL_CB;

/*********************************************** 
 * 1.intf上话复杂流分类节点,其中存放的信息有: *
 *   该intf上绑定的policy策略模板、policy策略 *
 *   模板所对应的acl_id、intf上绑定的meter桶  *
 *   模板的intf_offset和sz_offset等.          *
 * 2.spm_qos_flow_classify_node:              *
 *   intf上话复杂流分类的引用树节点,该树用于  *
 *   指明各个intf上分别绑定的是哪个policy策略 *
 *   模板.                                    *
 * 3.spm_classify_id_node:                    *
 *   policy模板中intf上话复杂流分类的引用树节 *
 *   点,该树用于指明该policy策略模板已经被哪  *
 *   些intf所绑定.                            *
 **********************************************/
typedef struct spm_qos_flow_classify_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/

    /* 插入intf上话复杂流分类的引用树中 */
    AVLL_NODE spm_qos_flow_classify_node;

    /* 插入policy策略模板的intf上话复杂流分类引用树中 */
    AVLL_NODE spm_classify_id_node;

    /* key值: index */
    SPM_QOS_LOGIC_INTF_KEY key;
    
#ifdef PTN690
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS qos_cfg;
    NBB_USHORT intf_offset;
    NBB_USHORT sz_offset;
#else
    
#endif
    /*policy 索引*/
    NBB_ULONG policy_index;
    SUB_PORT sub_port;
    NBB_ULONG acl_id;
} SPM_QOS_FLOW_CLASSIFY_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_up_usr_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_up_usr_node;

    /* key值: index */
    SPM_QOS_LOGIC_INTF_KEY key;

    /*policy 索引*/
    NBB_ULONG policy_index;
    SUB_PORT sub_port;

} SPM_QOS_UP_USR_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_flow_up_usr_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_flow_up_usr_node;

    /* key值: index */
    SPM_QOS_LOGIC_FLOW_KEY key;

    /*policy 索引*/
    NBB_ULONG policy_index;
    SUB_PORT sub_port;

} SPM_QOS_FLOW_UP_USR_CB;

/* QoS模块相关信息 */
typedef struct spm_hqos_usr_cb
{

    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_usr_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    //NBB_HANDLE spm_qos_usr_handle;

    /***************************************************************************/
    /* 逻辑端口key值                                        */
    /***************************************************************************/
    NBB_ULONG index;

    /*PW id*/
    NBB_ULONG pw_id;

    NBB_LONG flow_id;

    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS usr_data;
} SPM_HQOS_LOG_USR_CB;

/* QoS模块相关信息 */
typedef struct spm_hqos_log_group_cb
{

    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_group_node;


    /***************************************************************************/
    /*group index                                        */
    /***************************************************************************/
    NBB_ULONG index;

    NBB_ULONG slot;

    NBB_ULONG port;

    AVLL_TREE usr_tree;

    NBB_ULONG lsp_id;

    /*组索引*/
    NBB_ULONG group_index;

    NBB_ULONG policy_index;

    NBB_USHORT pw_cnt;

    NBB_ULONG cnt;

} SPM_HQOS_LOG_GROUP_CB;



/* QoS模块相关信息 */



#define HQOS_LSP_NODE   3
#define HQOS_PW_NODE    2

/* QoS模块相关信息 */
typedef struct spm_qos_lsp_tx_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_lsp_tx_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_qos_lsp_tx_handle;

    CRTXLSP_KEY lsp_tx_key;

    /*ds 索引*/
    NBB_ULONG ds_index;

} SPM_QOS_LSP_TX_CB;

#define DEFAULT_B_NODE         1

/* QoS模块相关信息 */
typedef struct spm_hqos_lsp_tx_cb
{

    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_lsp_tx_node;


    SPM_QOS_TUNNEL_KEY lsp_tx_key;

    AVLL_TREE vc_tree;

    AVLL_TREE vrf_tree;

    NBB_ULONG lsp_id;

    NBB_ULONG pw_id;

    /*policy索引*/
    NBB_ULONG policy_index;

    NBB_BYTE mode;

    SPM_TE_SHAPING shaping;

    NBB_ULONG node_index;

    NBB_USHORT pw_cnt;/*IP LSP业务模型引用计数*/

    //NBB_USHORT default_flag;
} SPM_HQOS_LSP_TX_CB;


/* cfg tree */
typedef struct spm_ds_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_ds_node;

    /* key值: index */
    NBB_ULONG ds_key;

    /* 用户到phb映射子配置*/
    ATG_DCI_DIFF_SERV_PRI2PHBMAP *pri2phb_cfg_cb;

    /* phb到用户映射子配置*/
    ATG_DCI_DIFF_SERV_PHB2PRIMAP *phb2pri_cfg_cb;

    NBB_ULONG cnt;

} SPM_DS_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_ds_domain_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_ds_domain_node;

    /* key值: index */
    NBB_ULONG ds_domain_key;

    /* dscp 到phb映射子配置*/
    ATG_DCI_DS_DOMAIN_DSCP2PHBMAP *d2p_cfg_cb;

    /* vlan 到phb映射子配置*/
    ATG_DCI_DS_DOMAIN_V8021P2PHBMAP *vlan2p_cfg_cb;

    /* EXP到phb映射子配置*/
    ATG_DCI_DS_DOMAIN_EXP2PHBMAP *exp2p_cfg_cb;

    /* phb到EXP映射子配置*/
    ATG_DCI_DS_DOMAIN_PHB2EXPMAP *p2exp_cfg_cb;

    /* PHB到 VLAN映射子配置*/
    ATG_DCI_DS_DOMAIN_PHB2V8021PMAP *p2vlan_cfg_cb;

    /* phb到dscp映射子配置*/
    ATG_DCI_DS_DOMAIN_PHB2DSCPMAP *p2d_cfg_cb;

    NBB_ULONG cnt;
} SPM_DS_DOMAIN_CB;

/* QoS模块相关信息 */
typedef struct spm_logic_acl_key
{
    NBB_ULONG index;
    NBB_BYTE id;
    NBB_USHORT svlan;
    NBB_USHORT cvlan;
    
}SPM_QOS_LOGIC_ACL_KEY;

/* QoS模块相关信息 */
typedef struct spm_qos_logic_acl_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_acl_node;

    AVLL_NODE spm_acl_id_node;/*ACL_id 树*/

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/

    //NBB_HANDLE spm_acl_handle;

    /* key值: logic key */
    SPM_QOS_LOGIC_ACL_KEY key;

    ATG_DCI_LOG_PORT_TRAFFIC_FILTER filter;

    SUB_PORT sub_port;

} SPM_QOS_LOGIC_ACL_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_acl_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_acl_node;

    NBB_LQE spm_acl_group_node;


    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/

    //NBB_HANDLE spm_acl_handle;

    /* key值: index */
    ATG_DCI_ACL_KEY acl_key;

    ATG_DCI_ACL_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_ACL_PORT_DATA *Port_cfg_cb;

    ATG_DCI_ACL_ETH_HEADER *Eth_cfg_cb;

    ATG_DCI_ACL_IPV4_FRAME *ipv4_cfg_cb;

    ATG_DCI_ACL_IPV4_TCP *ipTcp_cfg_cb;

    ATG_DCI_ACL_IPV6_FRAME *ipv6_cfg_cb;

    ATG_DCI_ACL_NONE_IP *noIp_cfg_cb;

    ATG_DCI_ACL_IPV4_UDP *ipUdp_cfg_cb;
#ifdef R8000_V3R2
    ATG_DCI_ACL_PORT_VLAN *portVlan_cfg_cb;
#endif
    NBB_ULONG meter_id;
    NBB_ULONG samp_id;


    NBB_ULONG cnt;

    NBB_USHORT pri_id;/*实例化ACL id*/

    NBB_BYTE flag;/*acl端口实例化标识*/
    NBB_BYTE match_tpye;/*acl实例化区间*/

    /**********驱动返回值*****************/
    NBB_ULONG posid;
} SPM_QOS_ACL_CB;

#ifdef PTN690

/* QoS模块相关信息 */
typedef struct spm_policy_classify_key
{
    NBB_ULONG policy_id;
    NBB_ULONG acl_id;
}SPM_QOS_POLICY_CLASSIFY_KEY;

/* QoS模块相关信息 */
typedef struct spm_qos_policy_classify_cb
{
    AVLL_NODE tree_node;
    SPM_QOS_POLICY_CLASSIFY_KEY policy_classify_key;
    NBB_ULONG rule_id;
    NBB_USHORT unit;
    NBB_ULONG posid;
}SPM_QOS_POLICY_CLASSIFY_CB;

#endif

/* QoS模块相关信息 */
typedef struct spm_qos_classify_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_classify_node;

    NBB_LQE spm_classify_group_node;

#ifdef PTN690

    /*没有被实例化树应该为空*/
    AVLL_TREE spm_classify_policy_tree;
#else
   
#endif

    ATG_DCI_QOS_CLASSIFY_KEY classify_key;

    ATG_DCI_QOS_CLASSIFY_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_PORT_DATA *Port_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_ETH_HEADER *Eth_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *ipv4_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_TCP *ipTcp_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *ipv6_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_NONE_IP *noIp_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_UDP *ipUdp_cfg_cb;

#ifdef R8000_V3R2
    ATG_DCI_QOS_CLASSIFY_PORT_VLAN *portVlan_cfg_cb;
#endif

    /*全局的rule*/
    NBB_ULONG rule_id;

    NBB_BYTE match_type;

    NBB_ULONG cnt;

} SPM_QOS_CLASSIFY_CB;

#ifdef PTN690

/* QoS模块相关信息 */
typedef struct spm_policy_supervise_key
{
    NBB_ULONG policy_id;
    NBB_ULONG acl_id;
    NBB_ULONG classify_id;
}SPM_QOS_POLICY_SUPERVISE_KEY;

/* QoS模块相关信息 */
typedef struct spm_qos_policy_supervise_cb
{
    AVLL_NODE policy_supervise_node;
    SPM_QOS_POLICY_SUPERVISE_KEY policy_supervise_key;
    NBB_ULONG meter_bucket_id;
}SPM_QOS_POLICY_SUPERVISE_CB;

#endif

/* QoS模块相关信息 */
typedef struct spm_qos_action_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_action_node;

    /* key值: index */
    NBB_ULONG action_key;

    /* 基本配置*/
    ATG_DCI_QOS_BEHAVIOR_BASIC_DATA *basic_cfg_cb;

#ifdef PTN690

    /*没有被实例化树应该为空*/
    AVLL_TREE spm_qos_supervise_tree;
#endif

    /*流量监管*/
    ATG_DCI_QOS_BEHAVIOR_SUPERVISE *supervise_cfg_cb;
    NBB_ULONG color_action_id;

    /*流浪整形*/
    ATG_DCI_QOS_BEHAVIOR_SHAPPING *shaping_cfg_cb;

    /*调度策略*/
    ATG_DCI_QOS_BEHAVIOR_SCHEDULE *schedule_cfg_cb;

    /*复杂流分类动作指示*/
    ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *flow_act_cfg_cb;

    /*复杂流分类采样配置*/
    ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *flow_samping_cfg_cb;

    /*复杂流分类重定向配置*/
    ATG_DCI_QOS_BEHAVIOR_REDIRECT *redirect_cfg_cb;

    /*复杂流分类策略配置*/
    ATG_DCI_QOS_BEHAVIOR_POLICY_RT *policy_nhi_cfg_cb;

    /*复杂流分类PHB配置*/
    ATG_DCI_QOS_BEHAVIOR_PHB *phb_cfg_cb;

    /*复杂流分类域修改配置*/
    ATG_DCI_QOS_BEHAVIOR_DOMAIN *domain_cfg_cb;

    /*urpf*/
    ATG_DCI_QOS_BEHAVIOR_URPF *urpf_cfg_cb;

    /*优先级队列整形*/
    ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE que_shaping_cfg_cb[MAX_PHB_NUM];

    /*优先级队列调度*/
    ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL que_schedul_cfg_cb[MAX_PHB_NUM];

    /*优先级队列拥塞*/
    ATG_DCI_QOS_BEHAVIOR_QUE_CONGST que_congst_cfg_cb[MAX_PHB_NUM];

    NBB_BYTE   que_shape_flag;

    NBB_USHORT que_shape_num;
    

    NBB_USHORT que_schedul_num;

    NBB_USHORT que_congest_num;

    NBB_ULONG meter_id;

    NBB_ULONG cnt;
} SPM_QOS_ACTION_CB;

#ifdef PTN690

/* QoS模块相关信息 */
typedef struct spm_qos_c2b_tree_cb
{
    AVLL_TREE instance_tree;
    NBB_ULONG acl_id;
    NBB_BYTE offset[POLICY_C2B_INSTANCE_NUM + 1];
    NBB_ULONG cnt;/*已经实例化的次数*/
}SPM_QOS_C2B_TREE_CB;

#endif

/* QoS模块相关信息 */
typedef struct spm_qos_policy_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_policy_node;

#ifdef PTN690
    SPM_QOS_C2B_TREE_CB c2b_instance_tree[POLICY_C2B_INSTANCE_MAX];

    /* 一个policy策略cb对中包含的所有if-match规则数 */
    NBB_ULONG rule_resouce;
#else  
    
#endif

    AVLL_TREE instance_tree;/*共享模式 */
    NBB_ULONG acl_id;/*共享模式 */
    AVLL_TREE c2b_tree;

    /* key值: index */
    NBB_ULONG policy_key;

    NBB_USHORT c2b_num;
   
    ATG_DCI_QOS_POLICY_BASIC_DATA *basic_cfg_cb;

    NBB_ULONG meter_id;/*调度节点---流量监管模式下的*/

    /*引用计数*/
    NBB_ULONG cnt;
} SPM_QOS_POLICY_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_policy_c2b_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_policy_c2b_node;

    /* key值: index */
    ATG_DCI_QOS_POLICY_CB2QOS_DATA c2b_key;

    /*对应的驱动的*/
    NBB_ULONG acl_id;

    /*对应的引用ACL的模板的index*/
    NBB_ULONG acl_index;

    NBB_ULONG samp_id;

} SPM_QOS_POLICY_C2B_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_defend_apperc_cb
{
    AVLL_NODE spm_defend_apperc_node;
    ATG_DCI_DEFEND_POLICY_APPERC_P apperc_key;
    NBB_ULONG meter_id;
}SPM_QOS_DEFEND_APPERC_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_defend_user_def_cb
{
    AVLL_NODE spm_defend_user_def_node;
    ATG_DCI_DEFEND_POLICY_USER_DF user_def_key;
    NBB_ULONG meter_id;
}SPM_QOS_DEFEND_USER_DEF_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_defend_policy_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_defend_policy_node;

    AVLL_TREE apperc_tree;
    
    AVLL_TREE user_def_tree;

    NBB_ULONG policy_key;

    ATG_DCI_DEFEND_POLICY_APPERC_S apperc_policy_switch;

    ATG_DCI_DEFEND_POLICY_WHITE     white_policy;
    
    ATG_DCI_DEFEND_POLICY_BLACK     black_policy;

    ATG_DCI_DEFEND_POLICY_TOTAL_PKT total_pkt;

    ATG_DCI_DEFEND_POLICY_ABNORMAL  abnormal_switch;

    ATG_DCI_DEFEND_POLICY_FRAGMENT  fragment_switch;

    ATG_DCI_DEFEND_POLICY_UDP_PKT   udp_swtich;

    ATG_DCI_DEFEND_POLICY_TCPSYS    tcp_switch;


    /*********应用层维护数据********************/
    NBB_ULONG white_meter;
    NBB_ULONG black_meter;
    NBB_USHORT usr_pri;
    NBB_USHORT usr_que_pri;
    NBB_USHORT apper_pri;
    NBB_USHORT apper_que_pri;
    NBB_BYTE defend_slot[MAX_SLOT_NUM + 1];
    NBB_ULONG cnt;/*引用计数*/
} SPM_QOS_DEFEND_POLICY_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_defend_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_defend_node;

    NBB_ULONG key;

    ATG_DCI_DEFEND_SWITCH_BASIC basic;

} SPM_QOS_DEFEND_CB;

/* QoS模块相关信息 */
typedef struct spm_port_wred_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_port_wred_node;

    SPM_PORT_WRED_KEY port_wred_key;

    ATG_DCI_PHY_PORT_CONGEST_QUEUE cfg;

} SPM_PORT_WRED_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_wred_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_wred_node;

    /* key值: index */
    NBB_ULONG wred_key;

    ATG_DCI_WRED_BASIC_DATA *basic_cfg_cb;

    /*引用计数*/
    NBB_ULONG cnt;
} SPM_QOS_WRED_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_user_group_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_user_group_node;

    /* key值: index */
    NBB_ULONG user_group_key;

    ATG_DCI_USER_GROUP_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_USER_GROUP_UP_QOS *up_cfg_cb;

    ATG_DCI_USER_GROUP_DOWN_QOS *down_cfg_cb;

    /*绑定的LSP ID*/
    NBB_ULONG lsp_id;

    /*HQOS实例化端口*/
    NBB_USHORT port;
    NBB_USHORT slot;

    /*引用计数*/
    NBB_BYTE cnt;

} SPM_QOS_USER_GROUP_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_vrf_vpn_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_vrf_node;

    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_vrf_node;

    /***************************************************************************/
    /* Handle for this control block, to be used as a correlator for           */
    /* asynchronous message exchanges.                                         */
    /***************************************************************************/
    NBB_HANDLE spm_qos_vrf_handle;

    /* key值: index */
    SPM_QOS_VRF_INSTANSE_KEY vrf_key;

    /*PW ID*/
    NBB_ULONG pw_id;

    /*ds 索引*/
    NBB_ULONG ds_index;

    /*policy索引*/
    NBB_ULONG policy_index;

    /*引用计数*/
    NBB_BYTE cnt;

} SPM_QOS_VRF_VPN_CB;

/* QoS模块相关信息 */
typedef struct spm_hqos_slot_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_hqos_slot_node;

    /*固定为1 */
    NBB_ULONG index;

    ATG_DCI_HQOS_SLOT_DATA *data;

    NBB_ULONG cnt;

} SPM_HQOS_SLOT_CB;

/* QoS模块相关信息 */
typedef struct spm_twamp_ipv4_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_twamp_ipv4_node;

    ATG_DCI_TWAMP_IPV4_KEY key;

    ATG_DCI_TWAMP_IPV4_BASIC data;
    
    /***************************************************************************/
    /* Driver return code.                                                     */
    /***************************************************************************/
    NBB_ULONG posId;         /*posId*/
    
    NBB_ULONG groupId;       /*groupId*/
    
    NBB_ULONG entryId;       /*entryId*/   
} SPM_TWAMP_IPV4_CB;

/* QoS模块相关信息 */
typedef struct spm_twamp_ipv6_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_twamp_ipv6_node;

    ATG_DCI_TWAMP_IPV6_KEY key;

    ATG_DCI_TWAMP_IPV6_BASIC *data;

} SPM_TWAMP_IPV6_CB;




/***************************************************************************/
/* qos policy function                                                     */
/***************************************************************************/
#if 0
NBB_LONG spm_disconnect_policy_cnt(NBB_ULONG index,NBB_BYTE wred_flag NBB_CCXT_T NBB_CXT);
NBB_LONG spm_connect_policy_cnt(NBB_ULONG index NBB_CCXT_T NBB_CXT);
NBB_VOID spm_qos_clear_all_policy(NBB_CXT_T NBB_CXT);
NBB_LONG spm_qos_clear_logic_classify(NBB_LONG logic_key NBB_CCXT_T NBB_CXT);
NBB_VOID spm_qos_clear_all_logic_classify(NBB_CXT_T NBB_CXT);
#endif



/***************************************************************************/
/* qos action function                                                     */
/***************************************************************************/
#if 0
SPM_QOS_ACTION_CB *spm_qos_find_action_cb(NBB_ULONG index NBB_CCXT_T NBB_CXT);
NBB_LONG spm_qos_apply_meterId(NBB_ULONG *pid NBB_CCXT_T NBB_CXT);
NBB_LONG spm_qos_free_meterId(NBB_ULONG *pid NBB_CCXT_T NBB_CXT);
#endif


/***************************************************************************/
/* compare function                                                        */
/***************************************************************************/
#if 8
#ifdef PTN690
NBB_INT spm_qos_policy_supervise_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_policy_classify_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
SPM_QOS_POLICY_CLASSIFY_CB* spm_alloc_policy_classify_cb(SPM_QOS_POLICY_CLASSIFY_KEY* pkey NBB_CCXT_T);
SPM_QOS_POLICY_SUPERVISE_CB* spm_alloc_policy_supervise_cb(SPM_QOS_POLICY_SUPERVISE_KEY* pkey NBB_CCXT_T);
NBB_VOID spm_set_policer_action_template_error_log( NBB_ULONG unit, 
    POLICER_ACTION_TEMPLATE_T *cfg, const NBB_CHAR *FUNCTION,
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);
NBB_LONG spm_free_policy_supervise_cb(SPM_QOS_POLICY_SUPERVISE_CB *pstTbl NBB_CCXT_T);

NBB_LONG spm_free_policy_classify_cb(SPM_QOS_POLICY_CLASSIFY_CB *pstTbl NBB_CCXT_T);
#endif
NBB_INT spm_qos_vrf_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_c2b_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_logic_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_logic_flow_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_port_wred_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_logic_flow_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_lsp_tx_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_c2b_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_acl_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_logic_acl_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_classify_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_defend_apperc_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_defend_usr_def_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_defend_nusr_def_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_port_wred_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_ilm_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_ftn_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T); 
NBB_INT spm_qos_rx_lsp_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_tx_lsp_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
NBB_INT spm_qos_tunnel_comp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T);
SPM_QOS_POLICY_CB *spm_qos_find_policy_cb(NBB_ULONG index NBB_CCXT_T);
SPM_QOS_CLASSIFY_CB *spm_qos_find_classify_cb(NBB_ULONG index NBB_CCXT_T);
SPM_QOS_ACTION_CB *spm_qos_find_action_cb(NBB_ULONG index NBB_CCXT_T);
SPM_QOS_ACL_CB *spm_qos_find_acl_cb(ATG_DCI_ACL_KEY *key NBB_CCXT_T);
SPM_QOS_WRED_CB *spm_qos_find_wred_cb(NBB_ULONG index NBB_CCXT_T);
#endif

#if 9
NBB_VOID spm_get_portid_from_logical_port_index_error_log( NBB_ULONG index,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_api_c3_set_acl_action_log(ACL_T *acl,SPM_QOS_ACL_CB *pcb,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE  NBB_CCXT_T);

NBB_VOID spm_api_c3_del_acl_error_log(NBB_ULONG unit, ACL_T *acl,
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_get_vlan_from_logical_port_index_error_log( NBB_ULONG index,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_api_c3_set_acl_error_log(NBB_ULONG unit, ACL_T *acl,
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_qos_param_error_log(const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE NBB_CCXT_T);

NBB_VOID spm_qos_apply_meter_error_log( NBB_ULONG *pid,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_LONG spm_qos_set_bucket(NBB_ULONG id, 
    ATG_DCI_QOS_BEHAVIOR_SUPERVISE* cfg NBB_CCXT_T);

NBB_VOID spm_qos_free_meter_error_log( NBB_ULONG *pid,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_LONG spm_qos_del_color_action(NBB_ULONG *id NBB_CCXT_T);

NBB_LONG spm_disconnect_policy_cnt(NBB_ULONG index, 
    NBB_BYTE wred_flag NBB_CCXT_T);

NBB_LONG spm_connect_policy_cnt(NBB_ULONG index NBB_CCXT_T);

NBB_ULONG spm_get_ipv6_mask(NBB_BYTE maskLen, NBB_ULONG *maskIpv6 NBB_CCXT_T);

NBB_VOID spm_dbg_record_qos_classify_head(ATG_DCI_QOS_CLASSIFY_KEY *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T);

NBB_VOID spm_clear_policer_bucket_template_error_log(NBB_ULONG unit,
    NBB_ULONG meter_id, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, 
    NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_api_c3_set_meter_error_log(NBB_ULONG unit, METER_CFG_T *meter,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);



NBB_VOID spm_api_c3_del_meter_error_log(NBB_ULONG unit, NBB_ULONG meterId,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_LONG spm_hqos_checkslot(NBB_LONG slot, NBB_LONG *slot_index NBB_CCXT_T);

NBB_LONG spm_qos_free_meter_id(NBB_ULONG *pid NBB_CCXT_T);

NBB_LONG spm_qos_apply_meter_id(NBB_ULONG *pid NBB_CCXT_T);

NBB_VOID spm_acl_update_rule_error_log(NBB_ULONG unit,
    NBB_ULONG posid,NBB_ULONG acl_id,NBB_ULONG rule_id, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_LONG spm_hqos_add_log_group(NBB_USHORT slot, NBB_USHORT port,
    SPM_HQOS_LOG_GROUP_CB *pcb, NBB_ULONG upflag, NBB_ULONG index NBB_CCXT_T);

NBB_VOID spm_hqos_arad_traffic_set_error_log(NBB_ULONG voq,
    NBB_USHORT slot,NBB_ULONG port,NBB_ULONG baseVCId,NBB_ULONG pw_id,
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_traffic_del_error_log(NBB_ULONG voq,
    NBB_USHORT slot,NBB_ULONG port,NBB_ULONG baseVCId,NBB_ULONG pw_id,
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T);

NBB_LONG spm_connect_wred_cnt(NBB_ULONG index NBB_CCXT_T);

NBB_LONG spm_disconnect_wred_cnt(NBB_ULONG index NBB_CCXT_T);

NBB_VOID spm_qos_vrf_key_pd(SPM_QOS_VRF_INSTANSE_KEY *pkey, 
    NBB_CHAR *string, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_qos_txlsp_key_pd(SPM_QOS_TUNNEL_KEY *pkey, NBB_CHAR *string,
    NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_create_lsp_error_log(NBB_ULONG port, NBB_ULONG lsp_id,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_set_lsp_bandwidth_error_log(
    NBB_ULONG lsp_id, NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_set_lsp_weight_error_log(NBB_ULONG lsp_id, 
    const NBB_ULONG weight, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_qos_vc_key_pd(ATG_DCI_VC_KEY *pkey, NBB_CHAR *string,
    NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_create_pw_error_log(NBB_ULONG port, NBB_ULONG lsp_id,
    NBB_ULONG pw_id, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_set_pw_bandwidth_error_log(NBB_ULONG pw_id, 
    NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_set_pw_weight_error_log(NBB_ULONG pw_id, NBB_ULONG weight,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_set_traffic_bandwidth_error_log(NBB_ULONG pw_id, 
    NBB_ULONG queue_id, NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs, 
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_set_trafficsche_error_log(NBB_ULONG port, 
    NBB_ULONG pw_id, NBB_ULONG que4_1, NBB_ULONG que8_5, const NBB_CHAR *FUNCTION,
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_voq_del_error_log(NBB_ULONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_ULONG baseVCId,const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_dbg_record_qos_logic_subport_head(SUB_PORT *sub_port, 
    NBB_ULONG logic_key, NBB_BYTE ucOper NBB_CCXT_T);

NBB_VOID spm_qos_subport_pd(SUB_PORT *sub_port, NBB_ULONG logic_key,
    NBB_CHAR *string, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_voq_error_log(NBB_ULONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_ULONG baseVCId,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID  spm_hqos_l3uni_error_log(
    NBB_ULONG voq, NBB_ULONG hqosEn, TX_PORT_T *l3uni,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID  spm_hqos_vp_error_log(
    NBB_ULONG voq, NBB_ULONG hqosEn, NBB_ULONG posid, NBB_LONG proflag,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_add_intf_pmline(NBB_ULONG logic_key, NBB_ULONG voq NBB_CCXT_T);

NBB_VOID spm_hqos_del_intf_pmline(NBB_ULONG logic_key NBB_CCXT_T);

NBB_VOID spm_dbg_record_qos_logic_head(NBB_ULONG logic_key,
    NBB_BYTE ucOper NBB_CCXT_T);

NBB_LONG spm_disconnect_usr_group_cnt(NBB_ULONG index NBB_CCXT_T);

NBB_VOID spm_check_hqos_lsp_node_cfg_log(SPM_QOS_TUNNEL_KEY *pkey,
    NBB_ULONG old_node, NBB_ULONG node, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE  NBB_CCXT_T);

NBB_VOID spm_check_hqos_lsp_mode_cfg_log(SPM_QOS_TUNNEL_KEY *pkey,
    NBB_ULONG old_mode, NBB_ULONG mode, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE  NBB_CCXT_T);

NBB_VOID spm_dbg_record_qos_lsp_head(SPM_QOS_TUNNEL_KEY *pkey,
    NBB_BYTE ucOper NBB_CCXT_T);

NBB_VOID spm_check_hqos_lsp_lspid_err_log(NBB_ULONG lspid, 
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE  NBB_CCXT_T);

NBB_VOID spm_check_hqos_lsp_del_cfg_log(SPM_QOS_TUNNEL_KEY *pkey, 
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE NBB_CCXT_T);

NBB_VOID spm_check_hqos_lsp_cfg_err_log(SPM_QOS_TUNNEL_KEY *pkey, 
    NBB_LONG flag, const NBB_CHAR *FUNCTION, NBB_ULONG LINE NBB_CCXT_T);

NBB_VOID spm_check_hqos_lsp_cfg_same_log(SPM_QOS_TUNNEL_KEY *pkey, 
    NBB_ULONG policy_index, const NBB_CHAR *FUNCTION, NBB_ULONG LINE NBB_CCXT_T);

NBB_VOID spm_dbg_record_qos_vc_head(ATG_DCI_VC_KEY *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T);

NBB_VOID spm_hqos_add_vc_pmline(ATG_DCI_VC_KEY *pkey, NBB_ULONG voq NBB_CCXT_T);

NBB_VOID spm_check_hqos_vc_node_index_cfg_log(ATG_DCI_VC_KEY *pkey,
    NBB_ULONG old_node_index, NBB_ULONG node_index, const NBB_CHAR *FUNCTION,
    NBB_ULONG LINE  NBB_CCXT_T);

NBB_VOID spm_hqos_del_vc_pmline(ATG_DCI_VC_KEY *pkey NBB_CCXT_T);

NBB_VOID spm_dbg_record_qos_vrf_head(SPM_QOS_VRF_INSTANSE_KEY *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T);

NBB_VOID  spm_hqos_l3txpw_error_log(NBB_ULONG voq, 
    NBB_ULONG hqosEn, NBB_ULONG posid, const NBB_CHAR *FUNCTION,
    NBB_ULONG LINE, NBB_LONG ret  NBB_CCXT_T);

NBB_VOID spm_hqos_add_vrf_pmline(
    SPM_QOS_VRF_INSTANSE_KEY *pkey, NBB_ULONG voq NBB_CCXT_T);

NBB_VOID spm_check_hqos_vrf_node_index_cfg_log(
    SPM_QOS_VRF_INSTANSE_KEY *pkey, NBB_ULONG old_node_index, NBB_ULONG node_index, 
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE  NBB_CCXT_T);

NBB_VOID spm_hqos_del_vrf_pmline(SPM_QOS_VRF_INSTANSE_KEY *pkey NBB_CCXT_T);

NBB_VOID spm_qos_log_key_pd( NBB_ULONG logic_key,
    NBB_CHAR *string, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_arad_del_lsp_error_log(NBB_ULONG port,NBB_ULONG lsp_id,
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_almpm_addline_vc_log(
    ATG_DCI_VC_KEY *pkey, NBB_ULONG voq, const NBB_CHAR *FUNCTION, 
    NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_almpm_delline_vc_log(ATG_DCI_VC_KEY *pkey, 
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_almpm_addline_vrf_log(
    SPM_QOS_VRF_INSTANSE_KEY *pkey, NBB_ULONG voq, 
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_almpm_delline_vrf_log(SPM_QOS_VRF_INSTANSE_KEY *pkey, 
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_almpm_addline_intf_log(NBB_ULONG logic_key, 
    NBB_ULONG voq, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, 
    NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_hqos_almpm_delline_intf_log(NBB_ULONG logic_key, 
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret NBB_CCXT_T);

NBB_LONG spm_disconnect_action_cnt(NBB_ULONG index,NBB_BYTE wred_flag NBB_CCXT_T);

NBB_LONG spm_disconnect_classify_cnt(NBB_ULONG index NBB_CCXT_T);

NBB_LONG spm_connect_classify_cnt(NBB_ULONG index NBB_CCXT_T);

NBB_VOID spm_dbg_record_qos_policy_c2b_head(ATG_DCI_QOS_POLICY_CB2QOS_DATA *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T);

NBB_VOID spm_dbg_record_qos_policy_head(NBB_ULONG *pkey, 
    NBB_BYTE ucOper NBB_CCXT_T);

NBB_LONG spm_qos_apply_color_action_id(NBB_ULONG *pid NBB_CCXT_T);

NBB_LONG spm_qos_free_color_action_id(NBB_ULONG *pid NBB_CCXT_T);

NBB_VOID spm_clear_policer_color_action_template_error_log(NBB_ULONG unit,
    NBB_ULONG action_id, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_set_policer_bucket_template_error_log(NBB_ULONG unit,
    METER_CFG_T *meter, const NBB_CHAR *FUNCTION, NBB_ULONG LINE, 
    NBB_LONG ret NBB_CCXT_T);



NBB_VOID spm_l3_lookupuni_error_log(NBB_ULONG AclId, NBB_ULONG RuleId, NBB_ULONG ActionId,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_ULONG nxhop_index, NBB_LONG ret NBB_CCXT_T);

NBB_VOID spm_intf_set_policer_bucket_offset_error_log(NBB_ULONG unit,
    NBB_ULONG posid,NBB_ULONG intf_offset,NBB_ULONG acl_id,
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret NBB_CCXT_T);

NBB_LONG spm_connect_action_cnt(NBB_ULONG index NBB_CCXT_T);

NBB_LONG spm_qos_apply_meter_bucket_id(NBB_ULONG *pid NBB_CCXT_T);

NBB_LONG spm_qos_free_meter_bucket_id(NBB_ULONG *pid NBB_CCXT_T);

NBB_LONG spm_ds_pri2phb_map_init(void);

NBB_LONG spm_ds_phb2pri_map_init(void);

NBB_VOID spm_qos_clear_all_logic_acl(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_logic_classify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_logic_up_usr(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_flow_up_usr(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds_logic_flow(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds_logic_intf(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds_vc(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_univp_cb(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds_vrf(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds_ilm(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds_rxlsp(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds_txlsp(NBB_CXT_T NBB_CXT);

NBB_VOID spm_hqos_clear_all(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_acl(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_policy(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_classify(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_action(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_ds_domain(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_usr_group(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_hqos_slot(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_wred(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_port_wred(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_twamp_ipv4_cb(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_twamp_ipv6_cb(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_init(NBB_CXT_T NBB_CXT);

NBB_LONG spm_qos_voq_init(void);

NBB_LONG spm_init_rcu_cir_pir(void);

NBB_LONG spm_init_arad_cpu_cir_pir(void);

NBB_LONG spm_ds_add_logic_univp_node(
    NBB_ULONG logic_index, SPM_QOS_LOGIC_UNIVP *univp, 
    ATG_DCI_LOG_PORT_DS_L2_DATA *diff_serv NBB_CCXT_T);

#ifdef PTN690
NBB_LONG spm_add_pw_car_qos_node(NBB_ULONG log_id, 
    ATG_DCI_VC_KEY *pw_key, SUB_PORT *sub_port, 
    ATG_DCI_LOG_PORT_CAR_DATA *pw_car NBB_CCXT_T);

NBB_LONG spm_add_txlsp_meter_qos_node(
     CRTXLSP_KEY *key, NBB_ULONG txlsp_posid, 
     ATG_DCI_CR_LSP_TX_CAR_DATA *txlsp_car NBB_CCXT_T);

NBB_LONG spm_qos_clear_all_txlsp_car(NBB_CXT_T NBB_CXT);

NBB_VOID spm_qos_clear_all_pw_car(NBB_CXT_T NBB_CXT);
#endif

#endif

#endif

