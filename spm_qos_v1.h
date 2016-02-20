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
#ifndef SPM_QOS_INCLUDE
#define SPM_QOS_INCLUDE

#include "spm_qos_private_v1.h"
#include "spm_qos_ds_v1.h"
#include "spm_qos_acl_v1.h"
#include "spm_qos_classify_v1.h"
#include "spm_qos_action_v1.h"
#include "spm_qos_policy_v1.h"
#include "spm_qos_defend_v1.h"
#include <spm.h>
#include <c3api.h>
#include <a0dciif.h>

/***************************清除QOS配置函数***********************************/
void spm_qos_clear_all_cfg();

void spm_qos_init();

NBB_INT spm_qos_logic_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2);

/*内容较少没有定义相关头文件*/
NBB_VOID spm_rcv_dci_set_rcu_pir_control(ATG_DCI_SET_SIGNALING_CTRL *pstSetIps);

#endif

