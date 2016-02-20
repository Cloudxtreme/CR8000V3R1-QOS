/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_private.h
   版 本 号   : 初稿
   作    者   : zenglu
   生成日期   : 2012年9月18日
   最近修改   :
   功能描述   : QOS内部依赖公共部分
   函数列表   :
   修改历史   :
   1.日    期   : 2012年9月18日
    作    者   : zenglu
    修改内容   : 创建文件
******************************************************************************/
#ifndef SPM_QOS_DEFEND_INCLUDE
#define SPM_QOS_DEFEND_INCLUDE


#define MEM_SPM_DEFEND_POLICY_CB               ((NBB_LONG)(PCT_SPM | 0x00003100))
#define MEM_SPM_DEFEND_APPERC_CB               ((NBB_LONG)(PCT_SPM | 0x000031004))
#define MEM_SPM_DEFEND_USR_DEF_CB              ((NBB_LONG)(PCT_SPM | 0x00003105))
#define MEM_SPM_DEFEND_CB                       ((NBB_LONG)(PCT_SPM | 0x00003000))

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

void spm_qos_defend_init();

void spm_qos_clear_defend_cfg();

void spm_qos_defend_verify();

#endif
