/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_acl.h
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
#ifndef SPM_QOS_ACL_H
#define SPM_QOS_ACL_H

#include <spm.h>
#include <c3api.h>
#include <a0dciif.h>

#define MEM_SPM_QOS_ACL_LOGIC_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111A))
#define MEM_SPM_ACL_CB                           ((NBB_LONG)(PCT_SPM | 0x00002300))
#define MEM_SPM_ACL_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002301))
#define MEM_SPM_ACL_PORT_CB                     ((NBB_LONG)(PCT_SPM | 0x00002302))
#define MEM_SPM_ACL_ETH_CB                     ((NBB_LONG)(PCT_SPM | 0x00002303))
#define MEM_SPM_ACL_IPV4_CB                     ((NBB_LONG)(PCT_SPM | 0x00002304))
#define MEM_SPM_ACL_IPTCP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002305))
#define MEM_SPM_ACL_IPV6_CB                     ((NBB_LONG)(PCT_SPM | 0x00002306))
#define MEM_SPM_ACL_NOIP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002307))
#define MEM_SPM_ACL_IPUDP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002308))
#define ACTION_DROP         0
#define ACTION_PERMIT       1 
#define ACTION_SAMPLE       2
#define ACTION_REDIRECT     3
#define ACTION_POLICY_ROUTE 4

/* QoSģ�������Ϣ */
typedef struct spm_qos_acl_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_acl_node;

    NBB_LQE spm_acl_group_node;

    /* keyֵ: index */
    ATG_DCI_ACL_KEY acl_key;

    ATG_DCI_ACL_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_ACL_PORT_DATA *Port_cfg_cb;

    ATG_DCI_ACL_ETH_HEADER *Eth_cfg_cb;

    ATG_DCI_ACL_IPV4_FRAME *ipv4_cfg_cb;

    ATG_DCI_ACL_IPV4_TCP *ipTcp_cfg_cb;

    ATG_DCI_ACL_IPV6_FRAME *ipv6_cfg_cb;

    ATG_DCI_ACL_NONE_IP *noIp_cfg_cb;

    ATG_DCI_ACL_IPV4_UDP *ipUdp_cfg_cb;

    NBB_BYTE match_tpye;/*aclʵ��������*/

    /**********��������ֵ*****************/
    NBB_ULONG posid;
} SPM_QOS_ACL_CB;

/**STRUCT*********************************************************************/
/* Structure: SPM_QOS_ACL_ID_CB                                              */
/*                                                                           */
/* Description: SPM_QOS_ACL_ID ��������                                      */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_qos_acl_id_cb
{
    NBB_LQE acl_group;

    /* ʵ��������Щ�˿ںͲ�������*/
    AVLL_TREE instance_tree;

    NBB_BYTE match_type;

    /* ������policy�����ø�aclid�Ĵ���,Ϊ0��ʾδ������ */
    NBB_ULONG defend_aclid_cnt;

} SPM_QOS_ACL_ID_CB;


/* QoSģ�������Ϣ */
typedef struct spm_logic_acl_key
{
    NBB_ULONG  index;
    NBB_BYTE   id;
    NBB_USHORT svlan;
    NBB_USHORT cvlan;
}SPM_QOS_LOGIC_ACL_KEY;

/* QoSģ�������Ϣ */
typedef struct spm_qos_logic_acl_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_acl_node;

    AVLL_NODE spm_acl_id_node;

    /* keyֵ: logic key */
    SPM_QOS_LOGIC_ACL_KEY key;

    ATG_DCI_LOG_PORT_TRAFFIC_FILTER filter;

    SUB_PORT sub_port;
} SPM_QOS_LOGIC_ACL_CB;



/* QoSģ�������Ϣ */
typedef struct spm_qos_logic_univp
{
    NBB_ULONG      vp_type;
    NBB_ULONG      vpn_id;
    NBB_ULONG      vp_posid;
}SPM_QOS_LOGIC_UNIVP;

NBB_LONG spm_qos_acl_init(void);
NBB_VOID spm_cfg_logic_acl_cb_verify(void);
NBB_VOID spm_cfg_acl_cb_verify(void);
NBB_VOID spm_qos_clear_all_logic_acl(void);
NBB_LONG spm_add_logic_acl_node(NBB_ULONG key,ATG_DCI_LOG_PORT_TRAFFIC_FILTER *cfg, SUB_PORT *sub_port);

#endif
