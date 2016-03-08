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
#ifndef SPM_QOS_POLICY_H
#define SPM_QOS_POLICY_H

#include <spm.h>
#include <c3api.h>
#include <a0dciif.h>
#include <spm_qos_action_v1.h>
#include <spm_qos_classify_v1.h>

#define MEM_SPM_QOS_FLOW_CLASSIFY_CB                     ((NBB_LONG)(PCT_SPM | 0x0000111B))
#define MEM_SPM_POLICY_CB                     ((NBB_LONG)(PCT_SPM | 0x00002400))
#define MEM_SPM_POLICY_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002401))
#define MEM_SPM_POLICY_C2B_CB                     ((NBB_LONG)(PCT_SPM | 0x00002402))

#define POLICY_C2B_ACL_RULE_MAX  850

/* QoS模块相关信息 */
typedef struct spm_qos_policy_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_policy_node;

    /* 该policy被哪些intf绑定 */
    AVLL_TREE instance_tree;

    /* 该policy所对应的ruleid */
    NBB_BYTE  ruleid[POLICY_C2B_ACL_RULE_MAX];

    NBB_BYTE match_type;

    /* 该policy所对应的aclid */
    NBB_ULONG acl_id;

    AVLL_TREE c2b_tree;

    /* key值: index */
    NBB_ULONG policy_key;

    ATG_DCI_QOS_POLICY_BASIC_DATA *basic_cfg_cb;
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
} SPM_QOS_POLICY_C2B_CB;


/* QoS模块相关信息 */
typedef struct spm_qos_logic_intf_key
{
    NBB_ULONG   index;
    NBB_USHORT  svlan;
    NBB_USHORT  cvlan;
}SPM_QOS_LOGIC_INTF_KEY;


typedef struct spm_qos_flow_classify_cb
{
    /* 插入intf上话复杂流分类的引用树中 */
    AVLL_NODE spm_qos_flow_classify_node;

    /* 插入policy策略模板的intf上话复杂流分类引用树中 */
    AVLL_NODE spm_classify_id_node;

    /* key值: index */
    SPM_QOS_LOGIC_INTF_KEY key;

    /*policy 索引*/
    NBB_ULONG policy_index;

    SUB_PORT  sub_port;

    NBB_ULONG acl_id;
} SPM_QOS_FLOW_CLASSIFY_CB;

/* QoS模块相关信息 */
typedef struct spm_qos_logic_flow_key
{
    NBB_ULONG       index;
    NBB_USHORT      svlan;
    NBB_USHORT      cvlan;
}SPM_QOS_LOGIC_FLOW_KEY;

/* QoS模块相关信息 */
typedef struct tx_uni_port
{
    NBB_CHAR        sysPort;    //传递给C3的SYS PORT  
    NBB_USHORT      port;       //出端口
    NBB_USHORT      vid;        //内层VID
}TX_UNI_PORT;

NBB_LONG spm_add_flow_classify(SPM_QOS_POLICY_CB *policyCb, SUB_PORT *port);

NBB_LONG spm_qos_policy_init(void);

NBB_VOID spm_cfg_policy_cb_verify(void);

NBB_VOID spm_cfg_logic_classify_cb_verify(void);

NBB_LONG spm_add_logic_flow_classify_node(NBB_ULONG key,
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *classify_qos, SUB_PORT *sub_port);

NBB_VOID spm_qos_clear_all_logic_classify(void);

#endif
