/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_log.c
   �� �� ��   : ����
   ��    ��   : zenglu
   ��������   : 2012��9��18��
   ����޸�   :
   ��������   : QOS ģ���⹫������log�ļ�
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��9��18��
    ��    ��   : zenglu
    �޸�����   : �����ļ�
******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_parameter_error_log(const NBB_CHAR *function,NBB_ULONG line)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s %s,line=%ld param_error\n",
                QOS_CFG_STRING,function,line);
        OS_SPRINTF(message,"%s %s,line=%ld param_error\n",
                QOS_CFG_STRING,function,line);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			QOS_CFG_STRING,ATG_DCI_RC_UNSUCCESSFUL,
    			message,function,"line","",
    			0,0,line,0));      
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_exception_log(const NBB_CHAR *function,NBB_ULONG line)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s %s,line=%ld exception\n",
                QOS_CFG_STRING,function,line);
        OS_SPRINTF(message,"%s %s,line=%ld exception\n",
                QOS_CFG_STRING,function,line);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			QOS_CFG_STRING,ATG_DCI_RC_UNSUCCESSFUL,
    			message,function,"line","",
    			0,0,line,0));      
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_get_port_from_logical_port_index_err_log( NBB_ULONG index,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line = %ld  spm_get_portid_from_logical_port_index  err : "
                  "ret = %ld, logic_key = %ld\n\n", function, line, ret, index);
        OS_SPRINTF(message,"%s line = %ld  spm_get_portid_from_logical_port_index  err : "
                   "ret = %ld, logic_key = %ld\n\n", function, line, ret, index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        	           QOS_ERROR_STRING,ret,
        	           message,function,"line","",
        	           0,0,line,0)); 
    }  
}




/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_apply_meter_error_log( NBB_ULONG *pid,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != pid))
    {
        OS_PRINTF("%s line = %ld spm_qos_apply_meter_id err: ret = %ld, "
                  "meter_id = %ld\n\n",function,line,ret,*pid);
        OS_SPRINTF(uc_message,"%s line = %ld spm_qos_apply_meter_id err: ret = %ld, "
                   "meter_id = %ld\n\n",function,line,ret,*pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       uc_message,function,"line","",
    			       0,0,line,0)); 
    }  
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_qos_free_meter_error_log( NBB_ULONG *pid,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret )
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != pid))
    {
        OS_PRINTF("%s line = %ld  spm_qos_free_meter_id err: free meterId failed. "
                  "ret = %ld, meter_id = %ld\n\n",function,line,ret,*pid);
        OS_SPRINTF(uc_message,"%s line = %ld  spm_qos_free_meter_id err: free meterId failed. "
                   "ret = %ld, meter_id = %ld\n\n",function,line,ret,*pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       uc_message,function,"line","",
    			       0,0,line,0)); 
    }  
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_api_c3_set_meter_error_log(NBB_ULONG unit, METER_CFG_T *meter,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret )
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != meter))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetMeter err : ret = %ld (unit = %ld, MeterId = %ld, "
                  "MeterType = %d, cir = %ld, pir = %ld, cbs = %ld, pbs = %ld, dropred = %d)\n\n",
                  function,line,ret,unit,meter->meterId,meter->eMeterType,meter->cir,meter->pireir,
                  meter->cbs,meter->pbsebs,meter->dropRed);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetMeter err : ret = %ld (unit = %ld, MeterId = %ld, "
                   "MeterType = %d, cir = %ld, pir = %ld, cbs = %ld, pbs = %ld, dropred = %d)\n\n",
                   function,line,ret,unit,meter->meterId,meter->eMeterType,meter->cir,meter->pireir,
                   meter->cbs,meter->pbsebs,meter->dropRed);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       uc_message,function,"line","",
    			       0,0,line,0));  
    }  
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_api_c3_del_meter_error_log(NBB_ULONG unit, NBB_ULONG meterId,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret )
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line = %ld  ApiC3DelMeter err : ret = %ld (unit = %ld, "
                  "MeterId = %ld)\n\n", function, line, ret, unit, meterId);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3DelMeter err : ret = %ld (unit = %ld, "
                   "MeterId = %ld)\n\n", function, line, ret, unit, meterId);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       uc_message,function,"line","",
    			       0,0,line,0));  
    }  
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_api_c3_set_acl_action_log(ACL_T *acl,SPM_QOS_ACL_CB *pcb,
    const NBB_CHAR *function,NBB_ULONG line)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != pcb) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetAcl Print: AclId = %d, RuleId = %d, "
                  "acl.eAclAction = %d, basic_cfg_cb->action = %d, match_type = %d)\n\n",function,
                  line,acl->mAclId,acl->mRuleId,acl->eAclAction,pcb->basic_cfg_cb->action,
                  pcb->basic_cfg_cb->match_type);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetAcl Print: AclId = %d, RuleId = %d, "
                   "acl.eAclAction = %d, basic_cfg_cb->action = %d, match_type = %d)\n\n",function,
                   line,acl->mAclId,acl->mRuleId,acl->eAclAction,pcb->basic_cfg_cb->action,
                   pcb->basic_cfg_cb->match_type);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
    }  
}



/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_api_c3_set_acl_error_log(NBB_ULONG unit, ACL_T *acl,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret )
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                  "meter_id = %ld)\n\n",function,line,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3SetAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                   "meter_id = %ld)\n\n",function,line,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       uc_message,function,"line","",
    			       0,0,line,0));  
    }  
}

/*****************************************************************************
   �� �� ��  : spm_api_c3_set_slot_attact_error_log
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2015��8��17�� 
   ��    ��  : tianf
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_api_c3_set_slot_attact_error_log(NBB_ULONG unit, 
                                    NBB_ULONG recordswitch,
                                    NBB_ULONG sampleratio,
                                    const NBB_CHAR *function,
                                    NBB_ULONG line,
                                    NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line = %ld fhdrv_psn_set_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d, "
                  "SampleRatio = %d, unit = %ld)\n\n",function,line,ret,recordswitch,sampleratio,unit);
        OS_SPRINTF(uc_message,"%s line = %ld  fhdrv_psn_set_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d"
                  ", SampleRatio = %d, unit = %ld)\n\n",function,line,ret,recordswitch,sampleratio,unit);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       uc_message,function,"line","",
    			       0,0,line,0));  
    }  
}

/*****************************************************************************
   �� �� ��  : spm_api_c3_set_slot_attact_error_log
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2015��8��17�� 
   ��    ��  : tianf
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_api_c3_get_slot_attact_error_log(NBB_ULONG unit, 
                                    NBB_ULONG *recordswitch,
                                    NBB_ULONG *sampleratio,
                                    const NBB_CHAR *function,
                                    NBB_ULONG line,
                                    NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != recordswitch) && (NULL != sampleratio))
    {
        OS_PRINTF("%s line = %ld fhdrv_psn_get_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d, "
                  "SampleRatio = %d, unit = %ld)\n\n",function,line,ret,*recordswitch,*sampleratio,unit);
        OS_SPRINTF(uc_message,"%s line = %ld  fhdrv_psn_get_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d"
                  ", SampleRatio = %d, unit = %ld)\n\n",function,line,ret,*recordswitch,*sampleratio,unit);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       uc_message,function,"line","",
    			       0,0,line,0));  
    }  
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_api_c3_del_acl_error_log(NBB_ULONG unit, ACL_T *acl,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3DelAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                  "meter_id = %ld)\n\n",function,line,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        OS_SPRINTF(uc_message,"%s line = %ld  ApiC3DelAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                   "meter_id = %ld)\n\n",function,line,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       uc_message,function,"line","",
    			       0,0,line,0));  
    }  
}

#if 2

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_hqos_almpm_addline_vc_log(
    ATG_DCI_VC_KEY *pkey, NBB_ULONG voq, const NBB_CHAR *function, 
    NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != pkey))
    {
        OS_PRINTF("%s line=%ld almpm_addline_hqos_vc ret =%ld,VC (vc_id=%ld,"
				   "peer_ip=0x%lx,vc_type=%d) voq =%ld\n",function,line,ret,
				   pkey->vc_id,pkey->peer_ip,pkey->vc_type,voq);
        OS_SPRINTF(uc_message,"%s line=%ld almpm_addline_hqos_vc ret =%ld, VC "
				   "(vc_id=%ld,peer_ip=0x%lx,vc_type=%d) voq =%ld\n",function,
				   line,ret,pkey->vc_id,pkey->peer_ip,pkey->vc_type,voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			HQOS_BMU_ERROR,ret,
        			uc_message,function,"line","",
        			0,0,line,0)); 
    }
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_hqos_almpm_delline_vc_log(ATG_DCI_VC_KEY *pkey, 
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != pkey))
    {
        OS_PRINTF("%s line=%ld almpm_delline_hqos_vc ret =%ld,VC (vc_id=%ld,"
				   "peer_ip=0x%lx,vc_type=%d)\n",function,line,ret,pkey->vc_id,
				   pkey->peer_ip,pkey->vc_type);
        OS_SPRINTF(uc_message,"%s line=%ld almpm_delline_hqos_vc ret =%ld,VC (vc_id=%ld,"
				   "peer_ip=0x%lx,vc_type=%d)\n",function,line,ret,pkey->vc_id,
				   pkey->peer_ip,pkey->vc_type);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,uc_message,function,"line","",
        				0,0,line,0)); 
    }
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_hqos_almpm_addline_vrf_log(
    SPM_QOS_VRF_INSTANSE_KEY *pkey, NBB_ULONG voq, 
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != pkey))
    {
        OS_PRINTF("%s line=%ld almpm_addline_hqos_vrf ret = %ld,VRF (vrf_id=%d,"
				   "peer_ip=0x%lx,label=%ld), voq = %ld\n",function,line,ret,
				   pkey->vrf_id,pkey->peer_ip,pkey->label,voq);
        OS_SPRINTF(uc_message,"%s line=%ld almpm_addline_hqos_vrf ret = %ld,"
				   "VRF (vrf_id=%d,peer_ip=0x%lx,label=%ld), voq = %ld\n",
				   function,line,ret,pkey->vrf_id,pkey->peer_ip,pkey->label,voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,uc_message,function,"line","",
        				0,0,line,0)); 
    }
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_hqos_almpm_delline_vrf_log(SPM_QOS_VRF_INSTANSE_KEY *pkey, 
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret )
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (NULL != pkey))
    {
        OS_PRINTF("%s line=%ld almpm_delline_hqos_vrf ret = %ld,VRF (vrf_id=%d,"
				   "peer_ip=0x%lx,label=%ld)\n",function,line,ret,pkey->vrf_id,
				   pkey->peer_ip,pkey->label);
        OS_SPRINTF(uc_message,"%s line=%ld almpm_delline_hqos_vrf ret = %ld,"
				   "VRF (vrf_id=%d,peer_ip=0x%lx,label=%ld)\n",function,line,
				   ret,pkey->vrf_id,pkey->peer_ip,pkey->label);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,uc_message,function,"line","",
        				0,0,line,0)); 
    }
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_hqos_almpm_addline_intf_log(NBB_ULONG logic_key, 
    NBB_ULONG voq, const NBB_CHAR *function, NBB_ULONG line, 
    NBB_LONG ret )
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (0 != logic_key))
    {
        OS_PRINTF("%s line=%ld spm_hqos_almpm_addline_intf ret = %ld,INTF "
				   "(logic_key=%ld), voq = %ld\n",function,line,ret,logic_key,voq);
        OS_SPRINTF(uc_message,"%s line=%ld spm_hqos_almpm_addline_intf ret = %ld,INTF "
				   "(logic_key=%ld), voq = %ld\n",function,line,ret,logic_key,voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,uc_message,function,"line","",
        				0,0,line,0)); 
    }
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : usr group���ü�����1
   �������  : usr group��index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID spm_hqos_almpm_delline_intf_log(NBB_ULONG logic_key, 
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if((NULL != function) && (0 != logic_key))
    {
        OS_PRINTF("%s line=%ld almpm_delline_hqos_intf ret = %ld,INTF "
				   "(logic_key=%ld)\n",function,line,ret,logic_key);
        OS_SPRINTF(uc_message,"%s line=%ld almpm_delline_hqos_intf ret = %ld,INTF "
				   "(logic_key=%ld)\n",function,line,ret,logic_key);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        				HQOS_BMU_ERROR,ret,uc_message,function,"line","",
        				0,0,line,0)); 
    }
}

#endif



