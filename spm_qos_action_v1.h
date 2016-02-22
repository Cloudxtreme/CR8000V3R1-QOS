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

#ifndef SPM_QOS_ACTION_H
#define SPM_QOS_ACTION_H

#include <spm.h>
#include <c3api.h>
#include <a0dciif.h>


#define MAX_COLOR_ACTION_NUM                    4000
#define MEM_SPM_ACTION_CB                     ((NBB_LONG)(PCT_SPM | 0x00002600))
#define MEM_SPM_ACTION_BASIC_CB                     ((NBB_LONG)(PCT_SPM | 0x00002601))
#define MEM_SPM_ACTION_SHAPING_CB                     ((NBB_LONG)(PCT_SPM | 0x00002602))
#define MEM_SPM_ACTION_SUPERVISE_CB                     ((NBB_LONG)(PCT_SPM | 0x00002603))
#define MEM_SPM_ACTION_SCHEDULE_CB                     ((NBB_LONG)(PCT_SPM | 0x00002604))
#define MEM_SPM_ACTION_FLOW_ACT_CB                     ((NBB_LONG)(PCT_SPM | 0x00002606))
#define MEM_SPM_ACTION_FLOW_SAMPL_CB                     ((NBB_LONG)(PCT_SPM | 0x00002607))
#define MEM_SPM_ACTION_REDIRECT_CB                     ((NBB_LONG)(PCT_SPM | 0x000026008))
#define MEM_SPM_ACTION_POLCY_RT_CB                     ((NBB_LONG)(PCT_SPM | 0x00002609))
#define MEM_SPM_ACTION_PHB_CB                     ((NBB_LONG)(PCT_SPM | 0x0000260a))
#define MEM_SPM_ACTION_DOMAIN_CB                     ((NBB_LONG)(PCT_SPM | 0x0000260b))
#define MEM_SPM_ACTION_URPF_CB                     ((NBB_LONG)(PCT_SPM | 0x0000260c))
#define MEM_SPM_ACTION_POLICY_SUPERVISE_CB         ((NBB_LONG)(PCT_SPM | 0x0000260e))

/* QoSģ�������Ϣ */
typedef struct spm_qos_action_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_action_node;

    /* keyֵ: index */
    NBB_ULONG action_key;

    /* �������� */
    ATG_DCI_QOS_BEHAVIOR_BASIC_DATA *basic_cfg_cb;

    /* ������� */
    ATG_DCI_QOS_BEHAVIOR_SUPERVISE *supervise_cfg_cb;

    /* �������� */
    ATG_DCI_QOS_BEHAVIOR_SHAPPING *shaping_cfg_cb;

    /* ���Ȳ��� */
    ATG_DCI_QOS_BEHAVIOR_SCHEDULE *schedule_cfg_cb;

    /* ���������ද��ָʾ */
    ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *flow_act_cfg_cb;

    /* ����������������� */
    ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *flow_samping_cfg_cb;

    /* �����������ض������� */
    ATG_DCI_QOS_BEHAVIOR_REDIRECT *redirect_cfg_cb;

    /* ����������������� */
    ATG_DCI_QOS_BEHAVIOR_POLICY_RT *policy_nhi_cfg_cb;

    /* ����������PHB���� */
    ATG_DCI_QOS_BEHAVIOR_PHB *phb_cfg_cb;

    /* �������������޸����� */
    ATG_DCI_QOS_BEHAVIOR_DOMAIN *domain_cfg_cb;

    /* urpf */
    ATG_DCI_QOS_BEHAVIOR_URPF *urpf_cfg_cb;

    /* ���ȼ��������� */
    ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE que_shaping_cfg_cb[MAX_PHB_NUM];

    /* ���ȼ����е��� */
    ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL que_schedul_cfg_cb[MAX_PHB_NUM];

    /* ���ȼ�����ӵ�� */
    ATG_DCI_QOS_BEHAVIOR_QUE_CONGST que_congst_cfg_cb[MAX_PHB_NUM];

    NBB_BYTE   que_shape_flag;

    NBB_USHORT que_shape_num;

    NBB_USHORT que_schedul_num;

    NBB_USHORT que_congest_num;

    NBB_ULONG  meter_id;
} SPM_QOS_ACTION_CB;

#ifdef PTN690

/* QoSģ�������Ϣ */
typedef struct spm_qos_pw_car_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_qos_pw_car_node;

    /* keyֵ: log index */
    NBB_ULONG log_key;

    ATG_DCI_LOG_PORT_CAR_DATA pw_car;

    ATG_DCI_VC_KEY vc_key;

    /*meter_id*/
    NBB_ULONG meter_id;

    NBB_ULONG color_action_id;
} SPM_QOS_PW_CAR_CB;

#endif

NBB_VOID spm_cfg_action_cb_verify(void);

NBB_LONG spm_qos_behavior_init(void);

NBB_LONG spm_qos_apply_meter_id(NBB_ULONG *pid);

NBB_LONG spm_qos_free_meter_id(NBB_ULONG *pid);

NBB_LONG spm_del_action_driver(SPM_QOS_ACTION_CB *pstTbl);

#endif
