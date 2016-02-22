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
#ifndef SPM_QOS_CLASSIFY_H
#define SPM_QOS_CLASSIFY_H

#include <spm.h>
#include <c3api.h>
#include <a0dciif.h>
#include <spm_qos_action_v1.h>
#include <spm_qos_policy_v1.h>

#define CLASSIFY_HIGH_ACL_ID  5000
#define CLASSIFY_ETH_ACL_ID   1801
#define CLASSIFY_PORT_ACL_ID  901
#define MEM_SPM_CLASSIFY_CB                     ((NBB_LONG)(PCT_SPM | 0x00002500))
#define MEM_SPM_CLASSIFY_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002501))
#define MEM_SPM_CLASSIFY_PORT_CB                     ((NBB_LONG)(PCT_SPM | 0x00002502))
#define MEM_SPM_CLASSIFY_ETH_CB                     ((NBB_LONG)(PCT_SPM | 0x00002503))
#define MEM_SPM_CLASSIFY_IPV4_CB                     ((NBB_LONG)(PCT_SPM | 0x00002504))
#define MEM_SPM_CLASSIFY_IPTCP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002505))
#define MEM_SPM_CLASSIFY_IPV6_CB                     ((NBB_LONG)(PCT_SPM | 0x00002506))
#define MEM_SPM_CLASSIFY_NOIP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002507))
#define MEM_SPM_CLASSIFY_IPUDP_CB                     ((NBB_LONG)(PCT_SPM | 0x00002508))
#define MEM_SPM_CLASSIFY_POLICY_CB                     ((NBB_LONG)(PCT_SPM | 0x00002509))

/* QoS模块相关信息 */
typedef struct spm_qos_policy_classify_cb
{
    AVLL_NODE  tree_node;

    /* key值:policyid */
    NBB_ULONG  supervise_key;
    NBB_ULONG  rule_id;
    NBB_ULONG  posid;
    NBB_ULONG  behavior_id;
    NBB_ULONG  acl_id;
}SPM_QOS_POLICY_CLASSIFY_CB;


typedef struct spm_qos_classify_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_classify_node;

    NBB_LQE spm_classify_group_node;

    AVLL_TREE spm_classify_policy_tree;  

    ATG_DCI_QOS_CLASSIFY_KEY classify_key;

    ATG_DCI_QOS_CLASSIFY_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_PORT_DATA *Port_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_ETH_HEADER *Eth_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *ipv4_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_TCP *ipTcp_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *ipv6_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_NONE_IP *noIp_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_UDP *ipUdp_cfg_cb;

    NBB_BYTE match_type;
} SPM_QOS_CLASSIFY_CB;

/**STRUCT*********************************************************************/
/* Structure: SPM_QOS_CLASSIFY_ID_CB                                         */
/*                                                                           */
/* Description: SPM_QOS_CLASSIFY_ID 引用属性                                 */
/*                                                                           */
/*****************************************************************************/
typedef struct spm_qos_classify_id_cb
{

    NBB_LQE classify_group;

    /* 实例化被哪些端口和策略引用*/
    AVLL_TREE instance_tree;

    NBB_BYTE match_type;

} SPM_QOS_CLASSIFY_ID_CB;


NBB_VOID spm_cfg_classify_cb_verify(void);

NBB_LONG spm_qos_classify_init(void);


NBB_LONG spm_apply_classify_high_acl_id(NBB_ULONG *pid);

NBB_LONG spm_apply_classify_port_acl_id(NBB_ULONG *pid);

NBB_LONG spm_apply_classify_eth_acl_id(NBB_ULONG *pid);

NBB_LONG spm_release_classify_high_acl_id(NBB_ULONG pid);

NBB_LONG spm_release_classify_port_acl_id(NBB_ULONG pid);

NBB_LONG spm_release_classify_eth_acl_id(NBB_ULONG pid);

SPM_QOS_POLICY_CLASSIFY_CB* spm_alloc_policy_classify_cb(NBB_ULONG key);

NBB_LONG spm_free_policy_classify_cb(SPM_QOS_POLICY_CLASSIFY_CB *pstTbl);

NBB_LONG spm_check_classify_match_type(ATG_DCI_QOS_CLASSIFY_KEY classifykey, NBB_BYTE match_type);

NBB_LONG spm_del_classify_rule_cfg(SPM_QOS_CLASSIFY_CB *pstClassify);

#endif
