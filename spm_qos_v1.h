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

/***************************���QOS���ú���***********************************/
void spm_qos_clear_all_cfg();

void spm_qos_init();

NBB_INT spm_qos_logic_key_compare(NBB_VOID *keyarg1, NBB_VOID *keyarg2);

/*���ݽ���û�ж������ͷ�ļ�*/
NBB_VOID spm_rcv_dci_set_rcu_pir_control(ATG_DCI_SET_SIGNALING_CTRL *pstSetIps);

#endif

