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

/* QoS模块相关信息 */
typedef struct spm_qos_action_cb
{
    /***************************************************************************/
    /* The AVLL node.                                                          */
    /***************************************************************************/
    AVLL_NODE spm_action_node;

    /* key值: index */
    NBB_ULONG action_key;

    /* 基本配置 */
    ATG_DCI_QOS_BEHAVIOR_BASIC_DATA *basic_cfg_cb;

    /* 流量监管 */
    ATG_DCI_QOS_BEHAVIOR_SUPERVISE *supervise_cfg_cb;

    /* 流浪整形 */
    ATG_DCI_QOS_BEHAVIOR_SHAPPING *shaping_cfg_cb;

    /* 调度策略 */
    ATG_DCI_QOS_BEHAVIOR_SCHEDULE *schedule_cfg_cb;

    /* 复杂流分类动作指示 */
    ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *flow_act_cfg_cb;

    /* 复杂流分类采样配置 */
    ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *flow_samping_cfg_cb;

    /* 复杂流分类重定向配置 */
    ATG_DCI_QOS_BEHAVIOR_REDIRECT *redirect_cfg_cb;

    /* 复杂流分类策略配置 */
    ATG_DCI_QOS_BEHAVIOR_POLICY_RT *policy_nhi_cfg_cb;

    /* 复杂流分类PHB配置 */
    ATG_DCI_QOS_BEHAVIOR_PHB *phb_cfg_cb;

    /* 复杂流分类域修改配置 */
    ATG_DCI_QOS_BEHAVIOR_DOMAIN *domain_cfg_cb;

    /* urpf */
    ATG_DCI_QOS_BEHAVIOR_URPF *urpf_cfg_cb;

    /* 优先级队列整形 */
    ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE que_shaping_cfg_cb[MAX_PHB_NUM];

    /* 优先级队列调度 */
    ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL que_schedul_cfg_cb[MAX_PHB_NUM];

    /* 优先级队列拥塞 */
    ATG_DCI_QOS_BEHAVIOR_QUE_CONGST que_congst_cfg_cb[MAX_PHB_NUM];

    NBB_BYTE   que_shape_flag;

    NBB_USHORT que_shape_num;

    NBB_USHORT que_schedul_num;

    NBB_USHORT que_congest_num;

    NBB_ULONG  meter_id;
} SPM_QOS_ACTION_CB;

#ifdef PTN690

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

NBB_VOID spm_cfg_action_cb_verify(void);

NBB_LONG spm_qos_behavior_init(void);

NBB_LONG spm_qos_apply_meter_id(NBB_ULONG *pid);

NBB_LONG spm_qos_free_meter_id(NBB_ULONG *pid);

NBB_LONG spm_del_action_driver(SPM_QOS_ACTION_CB *pstTbl);

#endif
