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

/* QoSģ�������Ϣ */
typedef struct spm_qos_policy_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_policy_node;

    /* ��policy����Щintf�� */
    AVLL_TREE instance_tree;

    /* ��policy����Ӧ��ruleid */
    NBB_BYTE  ruleid[POLICY_C2B_ACL_RULE_MAX];

    NBB_BYTE match_type;

    /* ��policy����Ӧ��aclid */
    NBB_ULONG acl_id;

    AVLL_TREE c2b_tree;

    /* keyֵ: index */
    NBB_ULONG policy_key;

    ATG_DCI_QOS_POLICY_BASIC_DATA *basic_cfg_cb;
} SPM_QOS_POLICY_CB;

/* QoSģ�������Ϣ */
typedef struct spm_qos_policy_c2b_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_policy_c2b_node;

    /* keyֵ: index */
    ATG_DCI_QOS_POLICY_CB2QOS_DATA c2b_key;
} SPM_QOS_POLICY_C2B_CB;


/* QoSģ�������Ϣ */
typedef struct spm_qos_logic_intf_key
{
    NBB_ULONG   index;
    NBB_USHORT  svlan;
    NBB_USHORT  cvlan;
}SPM_QOS_LOGIC_INTF_KEY;


typedef struct spm_qos_flow_classify_cb
{
    /* ����intf�ϻ�������������������� */
    AVLL_NODE spm_qos_flow_classify_node;

    /* ����policy����ģ���intf�ϻ������������������� */
    AVLL_NODE spm_classify_id_node;

    /* keyֵ: index */
    SPM_QOS_LOGIC_INTF_KEY key;

    /*policy ����*/
    NBB_ULONG policy_index;

    SUB_PORT  sub_port;

    NBB_ULONG acl_id;
} SPM_QOS_FLOW_CLASSIFY_CB;

/* QoSģ�������Ϣ */
typedef struct spm_qos_logic_flow_key
{
    NBB_ULONG       index;
    NBB_USHORT      svlan;
    NBB_USHORT      cvlan;
}SPM_QOS_LOGIC_FLOW_KEY;

/* QoSģ�������Ϣ */
typedef struct tx_uni_port
{
    NBB_CHAR        sysPort;    //���ݸ�C3��SYS PORT  
    NBB_USHORT      port;       //���˿�
    NBB_USHORT      vid;        //�ڲ�VID
}TX_UNI_PORT;

NBB_LONG spm_add_flow_classify(SPM_QOS_POLICY_CB *policyCb, SUB_PORT *port);

NBB_LONG spm_qos_policy_init(void);

NBB_VOID spm_cfg_policy_cb_verify(void);

NBB_VOID spm_cfg_logic_classify_cb_verify(void);

NBB_LONG spm_add_logic_flow_classify_node(NBB_ULONG key,
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *classify_qos, SUB_PORT *sub_port);

NBB_VOID spm_qos_clear_all_logic_classify(void);

#endif
