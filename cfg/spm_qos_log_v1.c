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



