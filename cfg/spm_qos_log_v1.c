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
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pid))
    {
        OS_PRINTF("%s line = %ld spm_qos_apply_meter_id err: ret = %ld, "
                  "meter_id = %ld\n\n",FUNCTION,LINE,ret,*pid);
        OS_SPRINTF(ucMessage,"%s line = %ld spm_qos_apply_meter_id err: ret = %ld, "
                   "meter_id = %ld\n\n",FUNCTION,LINE,ret,*pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0)); 
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
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret )
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pid))
    {
        OS_PRINTF("%s line = %ld  spm_qos_free_meter_id err: free meterId failed. "
                  "ret = %ld, meter_id = %ld\n\n",FUNCTION,LINE,ret,*pid);
        OS_SPRINTF(ucMessage,"%s line = %ld  spm_qos_free_meter_id err: free meterId failed. "
                   "ret = %ld, meter_id = %ld\n\n",FUNCTION,LINE,ret,*pid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0)); 
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
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret )
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != meter))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetMeter err : ret = %ld (unit = %ld, MeterId = %ld, "
                  "MeterType = %d, cir = %ld, pir = %ld, cbs = %ld, pbs = %ld, dropred = %d)\n\n",
                  FUNCTION,LINE,ret,unit,meter->meterId,meter->eMeterType,meter->cir,meter->pireir,
                  meter->cbs,meter->pbsebs,meter->dropRed);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3SetMeter err : ret = %ld (unit = %ld, MeterId = %ld, "
                   "MeterType = %d, cir = %ld, pir = %ld, cbs = %ld, pbs = %ld, dropred = %d)\n\n",
                   FUNCTION,LINE,ret,unit,meter->meterId,meter->eMeterType,meter->cir,meter->pireir,
                   meter->cbs,meter->pbsebs,meter->dropRed);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
    const NBB_CHAR *FUNCTION, NBB_ULONG LINE, NBB_LONG ret )
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld  ApiC3DelMeter err : ret = %ld (unit = %ld, "
                  "MeterId = %ld)\n\n", FUNCTION, LINE, ret, unit, meterId);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3DelMeter err : ret = %ld (unit = %ld, "
                   "MeterId = %ld)\n\n", FUNCTION, LINE, ret, unit, meterId);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pcb) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetAcl Print: AclId = %d, RuleId = %d, "
                  "acl.eAclAction = %d, basic_cfg_cb->action = %d, match_type = %d)\n\n",FUNCTION,
                  LINE,acl->mAclId,acl->mRuleId,acl->eAclAction,pcb->basic_cfg_cb->action,
                  pcb->basic_cfg_cb->match_type);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3SetAcl Print: AclId = %d, RuleId = %d, "
                   "acl.eAclAction = %d, basic_cfg_cb->action = %d, match_type = %d)\n\n",FUNCTION,
                   LINE,acl->mAclId,acl->mRuleId,acl->eAclAction,pcb->basic_cfg_cb->action,
                   pcb->basic_cfg_cb->match_type);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
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
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret )
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3SetAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                  "meter_id = %ld)\n\n",FUNCTION,LINE,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3SetAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                   "meter_id = %ld)\n\n",FUNCTION,LINE,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
                                    NBB_ULONG pRecordSwitch,
                                    NBB_ULONG pSampleRatio,
                                    const NBB_CHAR *FUNCTION,
                                    NBB_ULONG LINE,
                                    NBB_LONG ret)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        OS_PRINTF("%s line = %ld fhdrv_psn_set_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d, "
                  "SampleRatio = %d, unit = %ld)\n\n",FUNCTION,LINE,ret,pRecordSwitch,pSampleRatio,unit);
        OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_psn_set_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d"
                  ", SampleRatio = %d, unit = %ld)\n\n",FUNCTION,LINE,ret,pRecordSwitch,pSampleRatio,unit);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
                                    NBB_ULONG *pRecordSwitch,
                                    NBB_ULONG *pSampleRatio,
                                    const NBB_CHAR *FUNCTION,
                                    NBB_ULONG LINE,
                                    NBB_LONG ret)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != pRecordSwitch) && (NULL != pSampleRatio))
    {
        OS_PRINTF("%s line = %ld fhdrv_psn_get_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d, "
                  "SampleRatio = %d, unit = %ld)\n\n",FUNCTION,LINE,ret,*pRecordSwitch,*pSampleRatio,unit);
        OS_SPRINTF(ucMessage,"%s line = %ld  fhdrv_psn_get_slot_attack_sampler err: ret = %ld (AttackRecordSwitch = %d"
                  ", SampleRatio = %d, unit = %ld)\n\n",FUNCTION,LINE,ret,*pRecordSwitch,*pSampleRatio,unit);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
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
    const NBB_CHAR *FUNCTION,NBB_ULONG LINE,NBB_LONG ret)
{
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    if((NULL != FUNCTION) && (NULL != acl))
    {
        OS_PRINTF("%s line = %ld  ApiC3DelAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                  "meter_id = %ld)\n\n",FUNCTION,LINE,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        OS_SPRINTF(ucMessage,"%s line = %ld  ApiC3DelAcl err: ret = %ld (AclId = %d, RuleId = %d, unit = %ld, "
                   "meter_id = %ld)\n\n",FUNCTION,LINE,ret,acl->mAclId,acl->mRuleId,unit,acl->meterIdx);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			       QOS_ERROR_STRING,ret,
    			       ucMessage,FUNCTION,"LINE","",
    			       0,0,LINE,0));  
    }  
}




