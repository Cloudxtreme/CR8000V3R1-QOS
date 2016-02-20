/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_qos_proc.c
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
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>


/*************************************************************************/
/**************************************************************************/
/*************************************************************************/
#define HQOS_TAG       "QOS HQOS CFG"

/*DSģ��ȫ�ִ�ӡ����*/
NBB_BYTE g_qos_hqos_print = ATG_DCI_RC_OK;

/*DSģ��ȫ��LOG����*/
NBB_BYTE g_qos_hqos_log = ATG_DCI_RC_OK;

NBB_BYTE g_hqos_port_index[4] = {0,0,0,0};

/***************************************************************************/
/* ����û�������*/
/***************************************************************************/
AVLL_TREE g_qos_user_group_tree;

SPM_QOS_PORT_CB g_qos_port_cb[MAX_SLOT_NUM + 1][MAX_PHYSIC_PORT_NUM];

/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID qos_set_hqos_print(NBB_BYTE set)
{
    g_qos_hqos_print = set;
}











/*****************************************************************************
   �� �� ��  : spm_qos_find_classify_cb
   ��������  : ����classifyģ���������
   �������  : classifyģ���index
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_VOID qos_set_hqos_log(NBB_BYTE set)
{
    g_qos_hqos_log = set;
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
NBB_VOID spm_dbg_record_hqos_vc_head(ATG_DCI_VC_KEY *vc_key, NBB_BYTE ucOper)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];
    if(NULL != vc_key && ATG_DCI_RC_OK != g_qos_hqos_log)
    {
        OS_SPRINTF(message,"################### VC (vc_id=%ld,peer_ip=0x%lx,"
            "vc_type=%d, %s) ###################\n",
                    vc_key->vc_id,vc_key->peer_ip,vc_key->vc_type,
                    *(qos_cfg_oper_type_string + ucOper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);  
    }
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : ���������̵�VC
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
static NBB_LONG spm_hqos_check_slot(NBB_BYTE slot,NBB_ULONG node_index,
                    NBB_LONG *voq,NBB_LONG *vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_LONG base_queue = 0;
    NBB_LONG base_vcid = 0;
    NBB_BYTE fapid = v_spm_shared->local_slot;
    NBB_BYTE slot_index = 128;
    NBB_BYTE fapid_index = 128;
    NBB_ULONG offset = MAX_HQOS_SLOT_NUM * NUM_COS;

    NBB_TRC_ENTRY(__FUNCTION__);

    for (i = 0; i < MAX_HQOS_SLOT_NUM; i++)
    {
        /*g_hqos_port_index[i]:֧��hqos�Ĳ�λ��*/
        if (g_hqos_port_index[i] == slot)
        {
           *slot_index = i;
        }
        if(g_hqos_port_index[i] == fapid)
        {
            fapid_index = i;
        }
        vc_id[i] = offset * node_index + i * NUM_COS + HQOS_OFFSET;
    }

    if(128 == slot_index || 128 == fapid_index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    *voq = slot_index * NUM_COS + offset * node_index + HQOS_OFFSET;
    *vc = offset * node_index + fapid_index * NUM_COS + HQOS_OFFSET;
    

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}



/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : ɾ�������̵�VC
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
NBB_LONG spm_hqos_apply_lsp_pw_id(NBB_LONG *lsp_id,NBB_LONG *pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*����LSP ID*/

    if(NULL != lsp_id)
    {
        ret = spm_hqos_apply_lsp_id(lsp_id);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }    
    }
    
    if(NULL != pw_id)
    {
        ret = spm_hqos_apply_pw_id(pw_id);
        if(ATG_DCI_RC_OK != ret && NULL != lsp_id)
        {
            spm_hqos_release_lsp_id(*lsp_id);
            goto EXIT_LABEL;
        }    
    }
    

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}




/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : ɾ�������̵�VC
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
NBB_LONG spm_hqos_release_lsp_pw_id(NBB_LONG lsp_id,NBB_LONG pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*��Դ��ռ�ÿ����ͷ�*/
    ret = spm_hqos_release_lsp_id(lsp_id);
    if(ATG_DCI_RC_OK != ret)
    {
        rv = ret;
    }
 
    ret = spm_hqos_release_pw_id(pw_id);
    if(ATG_DCI_RC_OK != ret)
    {
        rv = ret;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_create_lsp_pw(NBB_LONG lsp_id,
    NBB_USHORT port,NBB_LONG pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);


    if(0 != lsp_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspCreate(0,port,lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_create_lsp_error_log(port,lsp_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif         
    }

    if(0 != lsp_id && 0 != pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwCreate(0,port,lsp_id,pw_id,0);
        if (ATG_DCI_RC_OK != ret)
        {
            ApiAradHqosLspDelete(0,port,lsp_id);
            spm_hqos_arad_create_pw_error_log(port,lsp_id,pw_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif 
    }
    


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_del_lsp_pw(NBB_LONG lsp_id,
    NBB_USHORT port,NBB_LONG pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);


    if(0 != lsp_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspDelete(0,port,lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_lsp_error_log(port,lsp_id,
                __FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif        
    }
 

    if(0 != pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwDelete(0,lsp_id,pw_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_pw_error_log(lsp_id,pw_id,
                __FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif        
    }
     

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_print_traffic_shaping_cfg(QOS_TRAFFIC_SHAPING *data)
{
    if(NULL != data)
    {
        printf("traffic_shaping:cir=%ld,cbs=%ld,pir=%ld,pbs=%ld\n",
                  data->cir,data->cbs,data->pir,data->pbs);
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_log_traffic_shaping_cfg(QOS_TRAFFIC_SHAPING *data)
{
    NBB_CHAR message[DS_MSG_INFO_LEN];
    if(NULL != data)
    {
        OS_SPRINTF(message,"traffic_shaping:cir=%ld,cbs=%ld,pir=%ld,pbs=%ld\n",
                  data->cir,data->cbs,data->pir,data->pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_print_traffic_schedule_cfg(QOS_SCHEDULE_STRATEGY *data)
{
    if(NULL != data)
    {
        printf("traffic_schedule:mod=%d,weight=%d\n",
                  data->mod,data->weight);
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_log_traffic_schedule_cfg(QOS_SCHEDULE_STRATEGY *data)
{
    NBB_CHAR message[DS_MSG_INFO_LEN];
    if(NULL != data)
    {
        OS_SPRINTF(message,"traffic_schedule:mod=%d,weight=%d\n",
                  data->mod,data->weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_print_traffic_congestion_cfg(QOS_CONGESTION_STRATEGY *data)
{
    if(NULL != data)
    {
        printf("traffic_schedule:mod=%d,weight=%d\n",
                  data->mod,data->weight);
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_log_traffic_congestion_cfg(QOS_CONGESTION_STRATEGY *data)
{
    NBB_CHAR message[DS_MSG_INFO_LEN];
    if(NULL != data)
    {
        OS_SPRINTF(message,"traffic_schedule:mod=%d,weight=%d\n",
                  data->mod,data->weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
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
NBB_VOID spm_hqos_arad_voq_error_log(NBB_ULONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_ULONG base_vcid,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosVoqSet err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosVoqSet err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);     
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_TAG,ret,
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
static NBB_VOID spm_hqos_arad_traffic_set_error_log(NBB_ULONG voq,
    NBB_USHORT slot,NBB_ULONG port,NBB_ULONG base_vcid,NBB_ULONG pw_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosTrafficSet err: %ld (base_queue=%ld,slot=%d,"
                  "port=%ld,vc=%ld,pw_id=%ld)\n\n",function,line,ret,voq, slot, port, base_vcid,pw_id);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosTrafficSet err: %ld (base_queue=%ld,slot=%d,"
                   "port=%ld,vc=%ld,pw_id=%ld)\n\n",function,line,ret,voq, slot, port, base_vcid,pw_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_set_pw_bandwidth_error_log(NBB_ULONG pw_id, 
    NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs, const NBB_CHAR *function, 
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosPwBandwidthSet err: %ld "
            "(pw_id=%ld,cir=%ld,pir=%ld,cbs=%ld,pbs=%ld)\n\n",
            function,line,ret, pw_id,cir,pir,cbs,pbs);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosPwBandwidthSet err: %ld "
            "(pw_id=%ld,cir=%ld,pir=%ld,cbs=%ld,pbs=%ld)\n\n",
            function,line,ret, pw_id,cir,pir,cbs,pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_create_pw_error_log(NBB_ULONG port,NBB_ULONG lsp_id,
    NBB_ULONG pw_id,const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosPwCreate err: %ld (port=%ld,lsp_id=%ld,pw_id=%ld)\n\n",
                function,line,ret, port,lsp_id,pw_id);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosPwCreate err: %ld (port=%ld,lsp_id=%ld,pw_id=%ld)\n\n",
                function,line,ret, port,lsp_id,pw_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_voq_del_error_log(NBB_ULONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_ULONG base_vcid,const NBB_CHAR *function, 
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosVoqDelete err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosVoqDelete err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);  
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_traffic_del_error_log(NBB_ULONG voq,
    NBB_USHORT slot,NBB_ULONG port,NBB_ULONG base_vcid,NBB_ULONG pw_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosTrafficDelete err: %ld (base_queue=%ld,slot=%d,port=%ld,"
                  "vc=%ld,pw_id=%ld)\n\n",function,line,ret,voq, slot, port, base_vcid,pw_id);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosTrafficDelete err: "
                   "%ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld,pw_id=%ld)\n\n",
                   function,line,ret,voq, slot, port, base_vcid,pw_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);     
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_del_pw_error_log(NBB_ULONG lsp_id,NBB_ULONG pw_id,
    NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosPwDelete err: %ld (lsp_id=%ld,pw_id=%ld)\n\n",
                function,line,ret,lsp_id,pw_id);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosPwDelete err: %ld (lsp_id=%ld,pw_id=%ld)\n\n",
                function,line,ret,lsp_id,pw_id);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_set_pw_weight_error_log(NBB_ULONG pw_id, NBB_ULONG weight,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosPwWfqWeightSet err: %ld "
            "(pw_id=%ld,weight=%ld)\n\n",
            function,line,ret, pw_id,weight);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosPwWfqWeightSet err: %ld "
            "(pw_id=%ld,weight=%ld)\n\n",
            function,line,ret, pw_id,weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_set_traffic_bandwidth_error_log(NBB_ULONG pw_id, 
    NBB_ULONG queue_id, NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs, 
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosTrafficBandwidthSet err: %ld "
            "(pw_id=%ld,queue_id=%ld,cir=%ld,pir=%ld,cbs=%ld,pbs=%ld)\n\n",
            function,line,ret, pw_id,queue_id,cir,pir,cbs,pbs);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosTrafficBandwidthSet err: %ld "
            "(pw_id=%ld,queue_id=%ld,cir=%ld,pir=%ld,cbs=%ld,pbs=%ld)\n\n",
            function,line,ret, pw_id,queue_id,cir,pir,cbs,pbs);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_set_trafficsche_error_log(NBB_ULONG port, 
    NBB_ULONG pw_id, NBB_ULONG que4_1, NBB_ULONG que8_5, const NBB_CHAR *function,
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosTrafficScheModeSet err: %ld "
            "(port=%ld,pw_id=%ld,que4_1=%ld,que8_5=%ld)\n\n",
            function,line,ret, port,pw_id,que4_1,que8_5);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosTrafficScheModeSet err: %ld "
            "(port=%ld,pw_id=%ld,que4_1=%ld,que8_5=%ld)\n\n",
            function,line,ret, port,pw_id,que4_1,que8_5);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
static NBB_VOID spm_hqos_arad_fresh_pw_to_lsp_error_log(NBB_LONG unit,NBB_LONG pw_id,
    NBB_LONG lsp_id,NBB_USHORT port,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s %s,line=%ld ret=%ld fhdrv_fap_qos_refresh_pw_to_lsp\n"
                "unit=%ld,pw_id=%ld,lsp=%ld,port=%d\n\n",
                HQOS_LSP_ERROR,function,line,unit,pw_id,lsp_id,port);
        OS_SPRINTF(message,"%s %s,line=%ld ret=%ld fhdrv_fap_qos_refresh_pw_to_lsp\n"
                "unit=%ld,pw_id=%ld,lsp=%ld,port=%d\n\n",
                HQOS_LSP_ERROR,function,line,unit,pw_id,lsp_id,port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
static NBB_VOID spm_hqos_arad_create_lsp_error_log(NBB_ULONG port,NBB_ULONG lsp_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s %s,line=%ld ApiAradHqosLspCreate port=%ld,lsp id=%ld,ret=%ld\n",
                HQOS_LSP_ERROR,function,line,port,lsp_id,ret);
        OS_SPRINTF(message,"%s %s,line=%ld ApiAradHqosLspCreate port=%ld,lsp id=%ld,ret=%ld\n\n",
                HQOS_LSP_ERROR,function,line,port,lsp_id,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_del_lsp_error_log(NBB_ULONG port,NBB_ULONG lsp_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s %s,line=%ld ApiAradHqosLspDelete port=%ld,lsp id=%ld,ret=%ld\n",
                HQOS_LSP_ERROR,function,line,port,lsp_id,ret);
        OS_SPRINTF(message,"%s %s,line=%ld ApiAradHqosLspDelete port=%ld,lsp id=%ld,ret=%ld\n\n",
                HQOS_LSP_ERROR,function,line,port,lsp_id,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_set_lsp_bandwidth_error_log(
    NBB_ULONG lsp_id,NBB_ULONG cir,NBB_ULONG pir,NBB_ULONG cbs,NBB_ULONG pbs,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s %s,line=%ld ApiAradHqosLspBandwidthSet lsp id=%ld,cir=%ld,"
            "pir=%ld,cbs=%ld,pbs=%ld,ret=%ld\n",
                HQOS_LSP_ERROR,function,line,lsp_id,cir,pir,cbs,pbs,ret);
        OS_SPRINTF(message,"%s %s,line=%ld ApiAradHqosLspBandwidthSet lsp id=%ld,cir=%ld,"
            "pir=%ld,cbs=%ld,pbs=%ld,ret=%ld\n\n",
                HQOS_LSP_ERROR,function,line,lsp_id,cir,pir,cbs,pbs,ret);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_LSP_ERROR,ret,
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
NBB_VOID spm_hqos_arad_set_lsp_weight_error_log(NBB_ULONG lsp_id,NBB_ULONG weight,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ApiAradHqosLspWfqWeightSet err: %ld "
            "(lsp_id=%ld,weight=%ld)\n\n",
            function,line,ret, lsp_id,weight);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosLspWfqWeightSet err: %ld "
            "(lsp_id=%ld,weight=%ld)\n\n",
            function,line,ret, lsp_id,weight);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
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
NBB_VOID spm_hqos_arad_voq_error_log(NBB_ULONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_ULONG base_vcid,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosVoqSet err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        OS_SPRINTF(message,"%s line=%ld  ApiAradHqosVoqSet err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
                function,line,ret,voq, slot, port, base_vcid);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);     
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_ARAD_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
    }
     
}

fhdrv_psn_common_set_txlsp_hqos

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
NBB_VOID  spm_hqos_txlsp_error_log(NBB_LONG unit,NBB_ULONG posid,NBB_ULONG hqosEn,
    NBB_ULONG voq,  const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ret=%ld fhdrv_psn_common_set_txlsp_hqos\n"
                  "unit=%ld,posid=%ld,hqosEn=%s,voq=%ld\n\n",
                function,line,ret,unit,posid,
                hqosEn?"ʹ��":"��ʹ��",voq);
        OS_SPRINTF(message,"%s line=%ld  ret=%ld fhdrv_psn_common_set_txlsp_hqos\n"
                  "unit=%ld,posid=%ld,hqosEn=%s,voq=%ld\n\n",
                function,line,ret,unit,posid,
                hqosEn?"ʹ��":"��ʹ��",voq);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_C3_ERROR,ret,
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
NBB_VOID  spm_hqos_vp_error_log(
    NBB_ULONG voq, NBB_ULONG hqosEn, NBB_ULONG posid, NBB_LONG proflag,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        OS_PRINTF("%s line=%ld  ret=%ld ApiC3SetVpHqos voq=%ld "
                  "posid=%ld,hqosEn=%s,proflag=%s\n\n",
                function,line,ret,voq, posid,
                hqosEn?"ʹ��":"��ʹ��",(QOS_BACK == proflag)?"����":"����");
        OS_SPRINTF(message,"%s line=%ld  ret=%ld ApiC3SetVpHqos voq=%ld"
                   " posid=%ld,hqosEn=%s proflag=%s\n\n",
                function,line,ret,voq, posid,
                hqosEn?"ʹ��":"��ʹ��",(QOS_BACK == proflag)?"����":"����");
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_C3_ERROR,ret,
    			message,function,"line","",
    			0,0,line,0));  
    }
   
}

#endif

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : ���������̵�VC
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
static NBB_LONG spm_hqos_create_voq_vc(NBB_BYTE slot, NBB_LONG port,
    NBB_ULONG pw_id,
    NBB_LONG voq_id,NBB_LONG tm_vc,NBB_ULONG vc_id[MAX_HQOS_SLOT_NUM],
    NBB_LONG *p_voq,NBB_LONG *p_vc,NBB_ULONG p_vc_id[MAX_HQOS_SLOT_NUM])
{
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vc_id)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    

#if defined (SPU) || defined (SRC)
    ret = ApiAradHqosVoqSet(unit, voq_id, NUM_COS, slot, port, tm_vc);
    if (ATG_DCI_RC_OK != ret)       
    {
        spm_hqos_arad_voq_error_log(voq_id,slot,port,
            tm_vc, __FUNCTION__,__LINE__,ret);
        goto EXIT_LABEL;
    }
    if(NULL != p_voq && NULL != p_vc)
    {
        *p_voq = voq_id;
        *p_vc = tm_vc;
    }  
#endif

    /*����λ�ǳ����̽���VC�Ľ���*/
    if(slot == v_spm_shared->local_slot_id)
    {
        for(i = 0; i < MAX_HQOS_SLOT_NUM; i++)
        {
            if(0 == g_hqos_port_index[i])
            {
                continue; /*��HQOS��λ*/
            }
              
#if defined (SPU) || defined (SRC)

            /*�ڳ��ڶ˴���VC���Ϳ�����hqos���صĵ�����ڶ�VOQ��������*/
            ret = ApiAradHqosTrafficSet(unit, voq_id, NUM_COS, g_hqos_port_index[i], 
                        port, vc_id[i], pw_id);
            if (ATG_DCI_RC_OK != ret) 
            {
                spm_hqos_arad_traffic_set_error_log(voq_id,
                    g_hqos_port_index[i], port,vc_id[i],
                    pw_id,__FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
            if(NULL != p_vc_id)
            {
                p_vc_id[i] = vc_id[i];
            }          
#endif
        }
      
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}



/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : ���������̵�VC
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
static NBB_LONG spm_hqos_set_pw_property(NBB_BYTE slot,NBB_LONG pw_id,
     NBB_LONG base_queue,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }



    /*�����������PW��������*/
    if(slot == v_spm_shared->local_slot_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwBandwidthSet(unit, pw_id, data->normal_shaping.cir,
              data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_pw_bandwidth_error_log(pw_id, data->normal_shaping.cir,
              data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif

#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwWfqWeightSet(unit, pw_id, data->normal_strategy.weight);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_pw_weight_error_log( pw_id,
                data->normal_strategy.weight,__FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif


        /***************************************************************************/
        /* 8�������ô���                                                        */
        /***************************************************************************/
        for(i = 0;i < 8;i++)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficBandwidthSet(unit, pw_id,i, data->proi_queue_shaping[i].cir,
                  data->proi_queue_shaping[i]->pir, data->proi_queue_shaping[i]->cbs,
                  data->proi_queue_shaping[i]->pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_set_traffic_bandwidth_error_log(pw_id,i,
                    data->proi_queue_shaping[i].cir,data->proi_queue_shaping[i]->pir, 
                    data->proi_queue_shaping[i]->cbs,data->proi_queue_shaping[i]->pbs,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif  

#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficScheModeSet(unit, pw_id, data->proi_queue_strategy[i].cir,
                  data->proi_queue_strategy[i]->pir, data->proi_queue_strategy[i]->cbs,
                  data->proi_queue_strategy[i]->pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                
                goto EXIT_LABEL;
            }
#endif 
        }
     
    }


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : ���������̵�VC
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
static NBB_LONG spm_hqos_set_lsp_pw_property(NBB_BYTE slot,NBB_USHORT port,
     NBB_LONG lsp_id,NBB_LONG pw_id,NBB_ULONG posid,
     NBB_LONG base_queue,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }



    /*�����̲��Ҳ���Ĭ��LSP�ڵ����������LSP ��Ĭ��PW��������*/
    if(slot == v_spm_shared->local_slot_id && lsp_id != port)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspBandwidthSet(unit, lsp_id, data->normal_shaping.cir,
              data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_lsp_bandwidth_error_log(lsp_id, data->normal_shaping.cir,
              data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif

#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosLspWfqWeightSet(unit, lsp_id, data->normal_strategy.weight);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_lsp_weight_error_log(lsp_id,
                data->normal_strategy.weight,__FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif


        /***************************************************************************/
        /* 8�������ô���                                                        */
        /***************************************************************************/
        for(i = 0;i < 8;i++)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficBandwidthSet(unit, pw_id,i, data->proi_queue_shaping[i].cir,
                  data->proi_queue_shaping[i]->pir, data->proi_queue_shaping[i]->cbs,
                  data->proi_queue_shaping[i]->pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_set_traffic_bandwidth_error_log(pw_id,i,
                    data->proi_queue_shaping[i].cir,data->proi_queue_shaping[i]->pir, 
                    data->proi_queue_shaping[i]->cbs,data->proi_queue_shaping[i]->pbs,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif  

#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficScheModeSet(unit, pw_id, data->proi_queue_strategy[i].cir,
                  data->proi_queue_strategy[i]->pir, data->proi_queue_strategy[i]->cbs,
                  data->proi_queue_strategy[i]->pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                
                goto EXIT_LABEL;
            }
#endif 
        }
     
    }

    /*����̺ͳ����̶���ִ��*/
    /*�������C3��flow id hqos��ҵ��ͨ����*/
    for(unit = 0; unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
   
        ret = fhdrv_psn_common_set_txlsp_hqos(unit, posid,1, base_queue);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_txlsp_error_log(unit, posid,1, base_queue,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#if 4





/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ沢����lsp id ��ʼ��HQOS��
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
SPM_HQOS_LSP_CB *spm_alloc_hqos_lsp_cb(SPM_QOS_TUNNEL_KEY *pkey,
    NBB_LONG defaulte,NBB_BYTE slot,NBB_USHORT port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_LSP_CB *pst_tbl = NULL;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_LONG lsp_id = 0;
    NBB_LONG pw_id = 0;
    NBB_BUF_SIZE avll_key_offset;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pkey)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        goto EXIT_LABEL;
    }
    
    /*����������LSP ID ��Դ*/
    if(slot == v_spm_shared->local_slot_id)
    {
        /*LSP��Դ����ĸ��ʱ��ڴ治��ĸ��ʴ�ܶ�*/
        if(ATG_DCI_RC_OK != defaulte)
        {
            ret = spm_hqos_apply_lsp_pw_id(&lsp_id,&pw_id);
            if(ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

            ret = spm_hqos_create_lsp_pw(lsp_id,port,pw_id);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_hqos_release_lsp_pw_id(lsp_id,pw_id);
                goto EXIT_LABEL;
            }
        }
        else/*����Ĭ�ϵ�LSP ID*/
        {
            lsp_id = port;
        } 
    }

    /* ����һ���µ�lsp_tx���ÿ�qos��������Ŀ��*/
    pst_tbl = (SPM_HQOS_LSP_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_LSP_CB),
        NBB_NORETRY_ACT, MEM_SPM_HQOS_LSP_TX_CB);

    /*�����ڴ�ʧ�ܲ��ͷ�HQOS��Դ���ڴ�û���ˣ������͹���*/
    if (pst_tbl == NULL)
    {
        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /* ��ʼlsp_tx���ÿ�������Ŀ */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_HQOS_LSP_CB));
    OS_MEMCPY(&pst_tbl->lsp_tx_key, pkey, sizeof(SPM_QOS_TUNNEL_KEY));
    pst_tbl->lsp_id = lsp_id;
    pst_tbl->slot = slot;
    pst_tbl->port = port;
    pst_tbl->pw_id = pw_id;

    /*QOS vc ����ʼ��*/
    avll_key_offset = NBB_OFFSETOF(SPM_HQOS_VC_CB, vc_key);
    AVLL_INIT_TREE(pst_tbl->vc_tree, spm_vc_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_VC_CB, spm_hqos_vc_node));

    /*QOS vrf ����ʼ��*/
    avll_key_offset = NBB_OFFSETOF(SPM_HQOS_VRF_CB, vrf_key);
    AVLL_INIT_TREE(pst_tbl->vrf_tree, spm_qos_vrf_comp,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_VRF_CB, spm_hqos_vrf_node));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_hqos_lsp_tx_node);

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_free_hqos_lsp_cb(SPM_HQOS_LSP_CB *pst_tbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pst_tbl)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_LSP_TX_CB), MEM_SPM_HQOS_LSP_TX_CB);

    /*˵���Ѿ��ǳ����̲��Ҳ���Ĭ�ϵ�LSP�ڵ�*/
    if((pst_tbl->lsp_id != pst_tbl->port) && (0 != pst_tbl->lsp_id))
    {
        ret = spm_hqos_del_lsp_pw(pst_tbl->lsp_id, pst_tbl->port,pst_tbl->pw_id);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }

        ret = spm_hqos_release_lsp_pw_id(pst_tbl->lsp_id,pst_tbl->pw_id);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_HQOS_LSP_TX_CB);
    pst_tbl = NULL;

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}




/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_del_lsp_node(SPM_HQOS_LSP_CB **plsp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_LSP_CB *lsp = NULL;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_LONG lsp_id = 0;
    NBB_BYTE i = 0;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP�ڵ㲻����ΪNULL*/
    if(NULL == plsp || NULL == *plsp)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    lsp = *plsp;
    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;

     /*NP hqos ȥʹ��*/
    if(0 != lsp->np_flow_id || 0 != lsp->posid)
    {
        for(unit = 0; unit < c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
       
            ret = fhdrv_psn_common_set_txlsp_hqos(unit, lsp->posid,0, lsp->np_flow_id);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_txlsp_error_log(unit, lsp->posid,0, lsp->np_flow_id,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
    }


    /*ɾ��voq*/
    if( 0 != lsp->voq_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosVoqDelete(0, lsp->voq_id, NUM_COS, slot, port, lsp->tm_vc);
        if (ATG_DCI_RC_OK != ret)       
        {
            spm_hqos_arad_voq_error_log(lsp->voq_id,slot,port,
                lsp->tm_vc, __FUNCTION__,__LINE__,ret);
            rv = ret;
        } 
#endif    
     }

    /*ɾ��vc*/
    for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
    {
        if(0 != lsp->vc_id[i])
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficDelete(0, lsp->voq_id, NUM_COS, g_hqos_port_index[i],
                    port, lsp->vc_id[i],lsp->pw_id);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_traffic_del_error_log(lsp->voq_id,g_hqos_port_index[i],
                    port, lsp->vc_id[i],lsp->pw_id, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif     
        }
    }


    
    if(AVLL_IN_TREE(lsp->spm_hqos_lsp_tx_node)/*ɾ�����ڵ�����*/
    {
        /*ɾ����Ĭ�ϵ�LSP�ڵ�:����ʧ�ܻ�����Դ���߸��µ�Ĭ�ϵ�LSP ID*/
        if(0 != lsp_id && lsp_id != port)
        {
            /*����LSP ID ��Ĭ�ϵĽڵ���*/
            ret = spm_hqos_refresh_lsp_port(slot,port,port,lsp);
            if (ATG_DCI_RC_OK != ret)
            {
                rv = ret;
            }
        }
        
        /***************************************************************************/
        /* VC VRF�ڵ���Ϣȫ��Ϊ���˲�����󶨹�ϵ�ͷ���Դ***********************/                                                   */
        /***********************************************************************/
        if ((NULL == AVLL_FIRST(lsp->vc_tree) && NULL == AVLL_FIRST(lsp->vrf_tree))
        {
            AVLL_DELETE((g_qos_port_cb[slot][port]).lsp_tree,lsp->spm_hqos_lsp_tx_node);    
            ret = spm_free_hqos_lsp_cb(lsp);
            if (ATG_DCI_RC_OK != ret) 
            {
                rv = ret;
            } 
            *plsp = NULL;
        }
        
    }
    else/*ɾ����Ϊ�쳣������Դ����Ĭ�Ϻͷ�Ĭ��LSP�ڵ�*/
    {
        ret = spm_free_hqos_lsp_cb(lsp);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        } 
        *plsp = NULL;
    }


   
    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_set_lsp_to_default(SPM_HQOS_LSP_CB *lsp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_LSP_CB *lsp = NULL;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_LONG lsp_id = 0;
    NBB_LONG pw_id = 0;
    NBB_BYTE i = 0;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP�ڵ㲻����ΪNULL*/
    if(NULL == lsp)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       rv = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;
    pw_id = lsp->pw_id;

     /*NP hqos ȥʹ��*/
    if(0 != lsp->np_flow_id || 0 != lsp->posid)
    {
        for(unit = 0; unit < c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
       
            ret = fhdrv_psn_common_set_txlsp_hqos(unit, lsp->posid,0, lsp->np_flow_id);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_txlsp_error_log(unit, lsp->posid,0, lsp->np_flow_id,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
    }


    /*ɾ��voq*/
    if( 0 != lsp->voq_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosVoqDelete(0, lsp->voq_id, NUM_COS, slot, port, lsp->tm_vc);
        if (ATG_DCI_RC_OK != ret)       
        {
            spm_hqos_arad_voq_error_log(lsp->voq_id,slot,port,
                lsp->tm_vc, __FUNCTION__,__LINE__,ret);
            rv = ret;
        } 
#endif    
     }

    /*ɾ��vc*/
    for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
    {
        if(0 != lsp->vc_id[i])
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosTrafficDelete(0, lsp->voq_id, NUM_COS, g_hqos_port_index[i],
                    port, lsp->vc_id[i],lsp->pw_id);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_traffic_del_error_log(lsp->voq_id,g_hqos_port_index[i],
                    port, lsp->vc_id[i],lsp->pw_id, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif     
        }
    }

    ret = spm_hqos_del_lsp_pw(lsp_id,pw_id,port);
    if (ATG_DCI_RC_OK != ret)       
    {
        rv = ret;
    } 
    ret = spm_hqos_release_lsp_pw_id(lsp->lsp_id,lsp->pw_id);
    if (ATG_DCI_RC_OK != ret)       
    {
        rv = ret;
    } 
    lsp->lsp_id = port;
    lsp->pw_id = 0;

   
    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_add_lsp_all_drive(NBB_USHORT slot,
    NBB_LONG port,SPM_HQOS_LSP_CB *lsp_cb)
{
    SPM_HQOS_VRF_CB *vrf_cb = NULL;
    SPM_HQOS_VC_CB *vc_cb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == lsp_cb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    /*��Ĭ��lsp�ڵ�*/
    if(0 != lsp_cb->lsp_id && lsp_cb->port != lsp_cb->lsp_id)
    {
#if defined (SPU) || defined (SRC) 
        ret = ApiAradHqosLspCreate(0,port,lsp_cb->lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_create_lsp_error_log(port,lsp_cb->lsp_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }


    if(0 != lsp_cb->pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwCreate(0,port,lsp_cb->lsp_id,lsp_cb->pw_id,0);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_create_pw_error_log(port,lsp_cb->lsp_id,lsp_cb->pw_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif 
        ret = spm_hqos_set_lsp_drive(slot,port,lsp_cb->lsp_id,
            lsp_cb->pw_id,lsp_cb->posid,lsp_cb->data,lsp_cb->voq_id,
             lsp_cb->tm_vc,lsp_cb->vc_id,NULL);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }

    }

    
    

    for(vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_FIRST(lsp_cb->vrf_tree);
         vrf_cb != NULL;
         vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_NEXT(lsp_cb->vrf_tree,
                       vrf_cb->spm_hqos_vrf_node))
    {
        
    }

    for(vc_cb = (SPM_HQOS_VC_CB*) AVLL_FIRST(lsp_cb->vc_tree);
         vc_cb != NULL;
         vc_cb = (SPM_HQOS_VC_CB*) AVLL_NEXT(lsp_cb->vc_tree,
                       vc_cb->spm_hqos_vc_node))
    {
        if(0 != vc_cb->pw_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosPwCreate(0,port,lsp_cb->lsp_id,vc_cb->pw_id,0);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_create_pw_error_log(port,lsp_cb->lsp_id,
                    vc_cb->pw_id,__FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif 
            ret = spm_hqos_set_vc_drive(slot, port, vc_cb->pw_id,
                vc_cb->posid,vc_cb->proflag,vc_cb->vc_data,vc_cb->voq_id,
                vc_cb->tm_vc,vc_cb->vc_id,NULL);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }

        }
        
        
    }

    /*���³��µĳ���λ�ͳ��˿�*/
    lsp_cb->slot = slot;
    lsp_cb->port = port;

    

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_del_lsp_all_drive(SPM_HQOS_LSP_CB *lsp_cb)
{
    SPM_HQOS_VRF_CB *vrf_cb = NULL;
    SPM_HQOS_VC_CB *vc_cb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == lsp_cb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    for(vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_FIRST(lsp_cb->vrf_tree);
         vrf_cb != NULL;
         vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_NEXT(lsp_cb->vrf_tree,
                       vrf_cb->spm_hqos_vrf_node))
    {
         ret = spm_hqos_del_vrf_drive(lsp_cb->slot, lsp_cb->port, vc_cb);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        if(0 != vc_cb->pw_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosPwDelete(0,vc_cb->pw_id,lsp_cb->lsp_id);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_del_pw_error_log(lsp_cb->lsp_id,vc_cb->pw_id,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif 

        }
    }

    for(vc_cb = (SPM_HQOS_VC_CB*) AVLL_FIRST(lsp_cb->vc_tree);
         vc_cb != NULL;
         vc_cb = (SPM_HQOS_VC_CB*) AVLL_NEXT(lsp_cb->vc_tree,
                       vc_cb->spm_hqos_vc_node))
    {
        ret = spm_hqos_del_vc_drive(lsp_cb->slot, lsp_cb->port, vc_cb);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
        if(0 != vc_cb->pw_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosPwDelete(0,vc_cb->pw_id,lsp_cb->lsp_id);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_del_pw_error_log(lsp_cb->lsp_id,vc_cb->pw_id,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif 

        }
        
    }

    if(0 != lsp_cb->pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwDelete(0,lsp_cb->pw_id,lsp_cb->lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_pw_error_log(lsp_cb->lsp_id,lsp_cb->pw_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif 

    }

    /*��Ĭ��lsp�ڵ�*/
    if(0 != lsp_cb->lsp_id && lsp_cb->port != lsp_cb->lsp_id)
    {
#if defined (SPU) || defined (SRC) 
        ret = ApiAradHqosLspDelete(0,lsp_cb->port,lsp_cb->lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_lsp_error_log(lsp_cb->port,lsp_cb->lsp_id,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }


    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_refresh_lsp_port(
    NBB_LONG port,NBB_LONG lsp_id,
    SPM_HQOS_LSP_CB *lsp_cb)
{
    SPM_HQOS_VRF_CB *vrf_cb = NULL;
    SPM_HQOS_VC_CB *vc_cb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == lsp_cb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }

    
    if(lsp_cb->slot == v_spm_shared->local_slot_id)
    {
        for(vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_FIRST(lsp_cb->vrf_tree);
         vrf_cb != NULL;
         vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_NEXT(lsp_cb->vrf_tree,
                       vrf_cb->spm_hqos_vrf_node))
        {
#if defined (SPU) || defined (SRC) 
            ret = fhdrv_fap_qos_refresh_pw_to_lsp(unit,vrf_cb->pw_id,lsp_id,port);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_fresh_pw_to_lsp_error_log(unit,vrf_cb->pw_id,
                    lsp_id,port,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }

        for(vc_cb = (SPM_HQOS_VC_CB*) AVLL_FIRST(lsp_cb->vc_tree);
             vc_cb != NULL;
             vc_cb = (SPM_HQOS_VC_CB*) AVLL_NEXT(lsp_cb->vc_tree,
                           vc_cb->spm_hqos_vc_node))
        {
#if defined (SPU) || defined (SRC) 
            ret = fhdrv_fap_qos_refresh_pw_to_lsp(unit,vc_cb->pw_id,lsp_id,port);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_fresh_pw_to_lsp_error_log(unit,vc_cb->pw_id,
                    lsp_id,port,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
    }

    

    /*��Ĭ��lsp�ڵ�*/
    if(0 != lsp_cb->pw_id)
    {
#if defined (SPU) || defined (SRC) 
        ret = fhdrv_fap_qos_refresh_pw_to_lsp(unit,lsp_cb->pw_id,lsp_id,port);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_fresh_pw_to_lsp_error_log(unit,lsp_cb->pw_id,
                lsp_id,port,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    lsp_cb->port = port;

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG* spm_hqos_set_lsp_drive(NBB_BYTE slot,NBB_USHORT port,
        NBB_LONG lsp_id,NBB_LONG pw_id,
        NBB_ULONG posid,
        ATG_DCI_VPN_HQOS_POLICY *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_LSP_CB *plsp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data)|| (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    /*��������̳��ڵ�voq��vc�����ӹ�ϵHQOSҵ�����ͨ*/
    if(NULL == plsp)/*���ݱ�������ý���ˢ������*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,NULL,NULL,NULL);  
    }
    else/*�����յ������ô�����������*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,&(plsp->voq_id),&(plsp->tm_vc),plsp->vc_id);  
    }
    
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }


    /*����LSP�ڵ��HQOS������Ч*/
    ret = spm_hqos_set_lsp_pw_property(slot,port,lsp_id,pw_id,posid,voq,data);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    if(NULL != plsp)
    {
        plsp->posid = posid;
        plsp->np_flow_id = voq;
        
        /*�������ݣ����ݲ�����Ӧ������*/
        OS_MEMCPY(&(plsp->data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY));
    }


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_add_lsp_node(NBB_USHORT slot,
    NBB_LONG port,
    SPM_QOS_TUNNEL_KEY *lsp_key,
    ATG_DCI_LSP_TX_UP_TNNL_QOS *data,
    NBB_ULONG posid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_LSP_CB *plsp = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_ULONG node_index = 0;
    NBB_LONG voq = 0;
    NBB_LONG vc = 0;
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM] = {0};
    NBB_LONG lsp_id = 0;
    NBB_LONG pw_id = 0;
    

    NBB_TRC_ENTRY(__FUNCTION__);

        
    if ((NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (port > 0x80))/*��֧��LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }
    
    //qos_log_vc_hqos_data(slot,port,posid,proflag,lsp_key,vc_key,data);
    

    /* �����Ƿ����,�ؼ���pkeyΪ��lsp��Type��Flag��Tx_Lsp��keyֵ(ingress��egress��tunnelid��lspid) */
    plsp = AVLL_FIND(g_qos_port_cb[slot][port].lsp_tree, lsp_key);
    
    /* �ж�slot�Ƿ���hqos�Ŀ��� */
    ret = spm_hqos_check_slot(slot,data->node_index,&voq,&vc,vc_id);
    if (ATG_DCI_RC_OK != ret)
    {   
        if(NULL != plsp)/*�µ������ڷ�HQOS��λ��ҪʹHQOS��Ч*/
        {

        }
        else/*û�п���HQOS��λʹ��ֱ������������*/
        {
            ret = ATG_DCI_RC_OK;
        }
        goto EXIT_LABEL;
    }

    
    if(NULL == plsp)/*�ȴ���LSP�ڵ�*/
    {
        plsp = spm_alloc_hqos_lsp_cb(lsp_key,ATG_DCI_RC_UNSUCCESSFUL,slot,port);/*����LSP ID*/

        /*�����ڴ�ʧ��*/
        if (NULL == plsp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_hqos_set_lsp_drive(slot,port,lsp_id,pw_id,posid,data,voq,
            vc,vc_id [ MAX_HQOS_SLOT_NUM ], plsp);
        if(ATG_DCI_RC_OK != ret)
        {
            //�ͷ���LSP��Դ
            spm_hqos_del_lsp_node(&plsp);
            goto EXIT_LABEL;
        }
        
        rv = AVLL_INSERT(g_qos_port_cb[slot][port].lsp_tree, plsp->spm_hqos_lsp_tx_node);
    }
    else/*lsp�ڵ�ΪĬ�Ͻڵ���߸��½ڵ�*/
    {
        if(plsp->slot != slot)/*����λ�����仯��Ҫ����ȫ���ڵ����������*/
        {
            /*���²�λ*/
            ret = spm_hqos_del_lsp_all_drive(plsp);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
            ret = spm_hqos_add_lsp_all_drive(slot,port,plsp);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
        else if(plsp->port != port)/*���˿ڷ����仯��������*/
        {
            ret = spm_hqos_refresh_lsp_port(port,plsp->lsp_id,plsp);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
        else/*��������*/
        {
            /*Ĭ�Ͻڵ��Ϊ��Ĭ�Ͻڵ�Ҫˢ��LSP ID*/
            if(plsp->lsp_id == plsp->port)
            {
                ret = spm_hqos_apply_lsp_pw_id(lsp_id,pw_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }

                ret = spm_hqos_create_lsp_pw(lsp_id,port,pw_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_release_lsp_pw_id(lsp_id,pw_id);
                    goto EXIT_LABEL;
                }
                plsp->lsp_id = lsp_id;
                plsp->pw_id = pw_id;
                
                ret = spm_hqos_set_lsp_drive(slot,port,lsp_id,pw_id,posid,data,voq,
                    vc,vc_id [ MAX_HQOS_SLOT_NUM ], plsp);
                if(ATG_DCI_RC_OK != ret)
                {
                    //�ͷ���LSP��Դ
                    spm_hqos_set_lsp_to_default(plsp);
                    spm_hqos_del_lsp_node(&plsp);
                    goto EXIT_LABEL;
                }
            }
            else if((plsp->lsp_id != plsp->port) && (0 != plsp->lsp_id))/*���·�Ĭ�Ͻڵ�*/
            {
                ret = spm_hqos_set_lsp_pw_property(slot,port,plsp->lsp_id,plsp->pw_id,posid,voq,data);
                if (ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }
                plsp->posid = posid;
                plsp->np_flow_id = voq;
                OS_MEMCPY(&(plsp->data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY));
            }  
            else/*������߼�*/
            {
                spm_qos_exception_log(__FUNCTION__,__LINE__);
            }
        }

        
        
    }



    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif

#if 5

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
NBB_VOID spm_qos_vc_key_pd(ATG_DCI_VC_KEY *vc_key,NBB_CHAR *string,
    NBB_LONG ret)
{
    if((NULL != vc_key) && (NULL != string))
    {
       NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			string,ret,
    			"vc_id","peer_ip","vc_type","",
    			vc_key->vc_id,vc_key->peer_ip,vc_key->vc_type,0)); 
    }
}


/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_print_vc_hqos_cfg(ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_BYTE i = 0;
    if(NULL != data)
    {
       if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            qos_print_traffic_shaping_cfg(data->normal_shaping);
            qos_print_traffic_schedule_cfg(data->normal_strategy);
            for(i = 0;i < 8;i++)
            {
               qos_print_traffic_shaping_cfg(data->proi_queue_shaping[i]); 
               qos_print_traffic_schedule_cfg(data->proi_queue_strategy[i]);
               qos_print_traffic_congestion_cfg(data->proi_queue_congestion_strategy[i]);
            }
            
        } 
    }
    
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
static void qos_log_vc_hqos_cfg(ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_BYTE i = 0;
    if(NULL != data)
    {
       if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            qos_log_traffic_shaping_cfg(data->normal_shaping);
            qos_log_traffic_schedule_cfg(data->normal_strategy);
            for(i = 0;i < 8;i++)
            {
               qos_log_traffic_shaping_cfg(data->proi_queue_shaping[i]); 
               qos_log_traffic_schedule_cfg(data->proi_queue_strategy[i]);
               qos_log_traffic_congestion_cfg(data->proi_queue_congestion_strategy[i]);
            }
            
        } 
    }
}

/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
void qos_log_vc_hqos_data(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid, NBB_BYTE proflag,
    SPM_QOS_TUNNEL_KEY *lsp_key,,ATG_DCI_VC_KEY *vc_key,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_vc_hqos_cfg(data);
    
    if(ATG_DCI_RC_OK != g_qos_hqos_log)
    {
        OS_SPRINTF(uc_message,"slot=%d,port=%d,proflag=%d\n",
                  slot,port,proflag);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        qos_log_lsp_key(lsp_key);
        qos_log_vc_key(vc_key);
        qos_log_vc_hqos_cfg(data);
    }

   
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static SPM_HQOS_VC_CB *spm_alloc_hqos_vc_cb(ATG_DCI_VC_KEY *vc_key,
    NBB_LONG lsp_id,NBB_BYTE slot,NBB_USHORT port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VC_CB *pst_tbl = NULL;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_ULONG id = 0;
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vc_key)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        
        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /*����������pw��Դ*/
    if(slot == v_spm_shared->local_slot_id)
    {
        /*id��Դ������ڴ治��ĸ��ʴ�ܶ�*/
        ret = spm_hqos_apply_pw_id(&id);

        /*����pwidʧ��*/
        if (ATG_DCI_RC_OK != ret)
        {
            /*�쳣����*/
            goto EXIT_LABEL;
        }

#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwCreate(unit, port,lsp_id,id,0);

        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_release_pw_id(id);
            spm_hqos_arad_create_pw_error_log(port ,lsp_id,
                id,__FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif    
    }



    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pst_tbl = (SPM_HQOS_VC_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_VC_CB),
        NBB_NORETRY_ACT, MEM_SPM_HQOS_VC_CB);

    /*�����ڴ�ʧ�ܲ��ͷ�HQOS��Դ���ڴ�û���ˣ������͹���*/
    if (pst_tbl == NULL)
    {
        /*�쳣����*/
        goto EXIT_LABEL;
    }
    
    
    

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_HQOS_VC_CB));
    OS_MEMCPY(&(pst_tbl->vc_key), vc_key, sizeof(ATG_DCI_VC_KEY));
    pst_tbl->pw_id = id;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_hqos_vc_node);

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_free_hqos_vc_cb(SPM_HQOS_VC_CB *pst_tbl,NBB_LONG lsp_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_TRC_ENTRY(__FUNCTION__);

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pst_tbl)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_VC_CB), MEM_SPM_HQOS_VC_CB);
    
    if(0 != pst_tbl->pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwDelete(0, pst_tbl->pw_id,lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_pw_error_log(lsp_id,pst_tbl->pw_id,
                __FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif 

        ret = spm_hqos_release_pw_id(pst_tbl->pw_id);
        if (ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }
    

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_HQOS_VC_CB);
    pst_tbl = NULL;

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}



/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_del_vc_drive(NBB_BYTE slot,NBB_USHORT port,SPM_HQOS_VC_CB *vc)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num; 
    NBB_BYTE i = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vc)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }


    /***************************************************************************/
    /* �������VC�ϵ�����                                                    */
    /***************************************************************************/
    if(NULL != vc)
    {
        /*NP hqos ȥʹ��*/
        if(0 != vc->np_flow_id || 0 != vc->posid)
        {
            for(unit = 0; unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                if(VC_MAIN == proflag)
                {
                    ret = ApiC3SetVpHqos(unit, vc->posid, 0, 0, vc->np_flow_id);
                }
                else
                {
                    ret = ApiC3SetVpHqos(unit, vc->posid, 1, 0, vc->np_flow_id);
                }
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_vp_error_log(vc->np_flow_id,0,vc->posid,vc->proflag,
                        __FUNCTION__,__LINE__,ret);
                    rv = ret;
                }
#endif
            }
        }


        /*ɾ��voq*/
        if( 0 != vc->voq_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosVoqDelete(0, vc->voq_id, NUM_COS, slot, port, vc->tm_vc);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_voq_error_log(vc->voq_id,slot,port,
                    vc->tm_vc, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif    
         }

        /*ɾ��vc*/
        for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
        {
            if(0 != vc->vc_id[i])
            {
#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosTrafficDelete(0, vc->voq_id, NUM_COS, g_hqos_port_index[i],
                        port, vc->vc_id[i],vc->pw_id);
                if (ATG_DCI_RC_OK != ret)       
                {
                    spm_hqos_arad_traffic_del_error_log(vc->voq_id,g_hqos_port_index[i],
                        port, vc->vc_id[i],vc->pw_id, __FUNCTION__,__LINE__,ret);
                    rv = ret;
                } 
#endif     
            }
        }
         
    }
  
   
    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_del_vc_node(SPM_HQOS_LSP_CB **plsp,SPM_HQOS_VC_CB **pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_VC_CB *vc = NULL;
    SPM_HQOS_LSP_CB *lsp = NULL;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_BYTE i = 0;
    NBB_LONG lsp_id = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP�ڵ㲻����ΪNULL*/
    if(NULL == plsp || NULL == *plsp || NULL == pcb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    vc = *pcb;
    lsp = *plsp;
    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;

    /***************************************************************************/
    /* �������VC�ϵ�����                                                    */
    /***************************************************************************/
    if(NULL != vc)
    {
        ret = spm_hqos_del_vc_drive(slot,port,vc);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }

         /*ע�����ܸ澯*/
#ifdef PTN690
         spm_hqos_del_vc_pmline(vc->vc_key);
#endif


        /*����ɾ���Ѵ��ڵ���ȷ����*/
        if(AVLL_IN_TREE(vc->spm_hqos_vc_node)
        {
            AVLL_DELETE(lsp->vc_tree, vc->spm_hqos_vc_node);
        }

        /*�ͷ��ڴ���Դ*/
        ret = spm_free_hqos_vc_cb(vc,lsp_id);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }
        *pcb = NULL;/*ָ�����ΪNULL*/ 
         
    }
  
    ret = spm_hqos_del_lsp_node(plsp);
    if (ATG_DCI_RC_OK != ret) 
    {
        rv = ret;
    }

   
    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : ���������̵�VC
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
static NBB_LONG spm_hqos_set_vc_pw_property(NBB_BYTE slot,NBB_LONG pw_id,
    NBB_ULONG posid,NBB_LONG base_queue,NBB_BYTE proflag,
    ATG_DCI_VPN_HQOS_POLICY *data,SPM_HQOS_VC_CB *pcb)
{
    NBB_LONG base_queue = 0;
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }


    ret = spm_hqos_set_pw_property(slot,pw_id,base_queue,data);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    

     
    /*����̺ͳ����̶���ִ��*/
    /*�������C3��flow id hqos��ҵ��ͨ����*/
    for(unit = 0; unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        if(VC_MAIN == proflag)
        {
            ret = ApiC3SetVpHqos(unit, posid, 0, 1,base_queue);
        }
        else
        {
            ret = ApiC3SetVpHqos(unit, posid, 1, 1, base_queue);
        }
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_vp_error_log(base_queue,1,posid,proflag,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    if(NULL != pcb)
    {
        pcb->np_flow_id = base_queue;
        pcb->posid = posid;
        pcb->proflag = proflag;

        /*�������ݣ����ݲ�����Ӧ������*/
        OS_MEMCPY(&(pcb->vc_data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY)); 
    }
    

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}




/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_set_vc_drive(NBB_BYTE slot,NBB_USHORT port, NBB_LONG pw_id,
        NBB_ULONG posid,NBB_BYTE proflag, 
        ATG_DCI_VPN_HQOS_POLICY *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_VC_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data)|| (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    /*��������̳��ڵ�voq��vc�����ӹ�ϵHQOSҵ�����ͨ*/
    if(NULL == pcb)/*���ݱ�������ý���ˢ������*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,NULL,NULL,NULL);  
    }
    else/*�����յ������ô�����������*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,&(pcb->voq_id),&(pcb->tm_vc),pcb->vc_id);  
    }
    
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }


    /*����VC�ڵ��HQOS������Ч*/
    ret = spm_hqos_set_vc_pw_property(slot,posid,voq,proflag,data,pcb);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}




/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_add_vc(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
        NBB_BYTE proflag,SPM_QOS_TUNNEL_KEY *lsp_key, 
        ATG_DCI_VC_KEY *vc_key,ATG_DCI_VPN_HQOS_POLICY *data,
        NBB_LONG voq,NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_LONG unit = 0;
    SPM_HQOS_VC_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    /*�����Ƿ����B�ڵ�*/
    plsp = AVLL_FIND(g_qos_port_cb[slot][port].lsp_tree, lsp_key);

    /*LSP�ڵ�HQOS�����ڣ�����Ĭ�ϵ�LSP�ڵ�*/
    if (plsp == NULL)
    {     
        spm_dbg_record_hqos_vc_head(vc_key,SPM_OPER_ADD);

        plsp = spm_alloc_hqos_lsp_cb(lsp_key,ATG_DCI_RC_OK,slot,port);/*������LSP ID*/

        /*�����ڴ�ʧ��*/
        if (NULL == plsp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        pcb = AVLL_FIND(plsp->vc_tree, vc_key);
        if(NULL != pcb)/*�쳣����Ӧ��ΪNULL*/
        {
            spm_qos_exception_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        pcb = spm_alloc_hqos_vc_cb(vc_key,plsp->lsp_id,slot,port);
        if (NULL == pcb)
        {
            /*�ͷ���Դ*/
            spm_hqos_del_vc_node(&plsp,NULL);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ret = spm_hqos_set_vc_drive(slot,port,pcb->pw_id,posid,proflag,data,
            voq,vc,vc_id,pcb);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_del_vc_node(&plsp,&pcb);
            goto EXIT_LABEL;
        }

        /*ע�����ܸ澯*/
#ifdef PTN690
        spm_hqos_add_vc_pmline(vc_key,pcb->voq_id);
#endif

        rv = AVLL_INSERT(g_qos_port_cb[slot][port].lsp_tree, plsp->spm_hqos_lsp_tx_node);
        rv = AVLL_INSERT(plsp->vc_tree, pcb->spm_hqos_vc_node);
        
    }
    else/*LSP HQOS���ô��ڷ�Ĭ��LSP�ڵ�ͷ�Ĭ�Ͻڵ�֮��*/
    {
        pcb = AVLL_FIND(plsp->vc_tree, vc_key);
        if(NULL == pcb)/*�½��ڵ�*/
        {
            pcb = spm_alloc_hqos_vc_cb(vc_key,plsp->lsp_id,slot,port);
            if (NULL == pcb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

            ret = spm_hqos_set_vc_drive(slot,port,pcb->pw_id,posid,proflag,data,
                voq,vc,vc_id,pcb);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_hqos_del_vc_node(&plsp,&pcb);
                goto EXIT_LABEL;
            } 
            
            /*ע�����ܸ澯*/
#ifdef PTN690
            spm_hqos_add_vc_pmline(vc_key,pcb->voq_id);
#endif
            rv = AVLL_INSERT(plsp->vc_tree, pcb->spm_hqos_vc_node);
        }
        else/*����*/
        {
            /*����VC�ڵ��HQOS����*/
            ret = spm_hqos_set_vc_pw_property(slot,pcb->pw_id,posid,pcb->voq_id,
                proflag,data,pcb);
            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
        
    }
    
  

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_add_vc_node(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid, NBB_BYTE proflag,
    SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VC_KEY *vc_key,
    ATG_DCI_VPN_HQOS_POLICY *data)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VC_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    CRTXLSP_KEY lsptx_key = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG voq = 0;
    NBB_LONG vc = 0;
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM] = {0};


    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (port > 0x80))/*��֧��LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }
    
    qos_log_vc_hqos_data(slot,port,posid,proflag,lsp_key,vc_key,data);
    
        
    /* �ж�slot�Ƿ���hqos�Ŀ��� */
    ret = spm_hqos_check_slot(slot,data->node_index,&voq,&vc,vc_id);
    if (ATG_DCI_RC_OK != ret)/*û�п���HQOS��λʹ��ֱ������������*/
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }


    /*������L3ģ��ֱ�Ӹ��ң������Լ�����*/
    if((0 != lsp_key->type) && (0 == lsp_key->tx_lsp.lspid)) /*LSP����ΪCRLSP_TX*/
    {
        OS_MEMCPY(&lsptx_key,&(lsp_key->tx_lsp),sizeof(CRTXLSP_KEY));
        ret = spm_l3_gettxlspid(&lsptx_key, &(lsp_key->tx_lsp.lspid) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(message,"%s,%d spm_l3_rsvpforoam err=%ld\n\n",
                       __FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			HQOS_VC_ERROR,ret,
        			message,__FUNCTION__,"line","spm_l3_rsvpforoam",
        			0,0,__LINE__,0));

            /*�쳣����*/
            goto EXIT_LABEL;
        }
    }
    
    ret = spm_hqos_add_vc( slot,  port,  posid,proflag, lsp_key, vc_key, *data,
                    voq, vc, vc_id[MAX_HQOS_SLOT_NUM]);

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif



#if 6







/*****************************************************************************
   �� �� ��  : spm_ds_add_logic_intf_node
   ��������  : �߼��˿����ȼ�ӳ��Ľӿں���
   �������  : dsģ���index  ,�߼��˿ڵ�key,�����ṹ��ָ��
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
void qos_log_vrf_hqos_data(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
    SPM_QOS_TUNNEL_KEY *lsp_key,,ATG_DCI_VC_KEY *vc_key,ATG_DCI_VPN_HQOS_POLICY *data)
{
    NBB_CHAR uc_message[DS_MSG_INFO_LEN];
    
    qos_print_vc_hqos_cfg(data);
    
    if(ATG_DCI_RC_OK != g_qos_hqos_log)
    {
        OS_SPRINTF(uc_message,"slot=%d,port=%d\n",
                  slot,port);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        qos_log_lsp_key(lsp_key);
        qos_log_vc_key(vc_key);
        qos_log_vc_hqos_cfg(data);
    }

   
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static SPM_HQOS_VC_CB *spm_alloc_hqos_vrf_cb(ATG_DCI_VRF_INSTANCE_KEY *vrf_key,
    NBB_LONG lsp_id,NBB_BYTE slot,NBB_USHORT port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VRF_CB *pst_tbl = NULL;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_ULONG id = 0;
    NBB_LONG unit = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vrf_key)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        
        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /*����������pw��Դ*/
    if(slot == v_spm_shared->local_slot_id)
    {
        /*id��Դ������ڴ治��ĸ��ʴ�ܶ�*/
        ret = spm_hqos_apply_pw_id(&id);

        /*����pwidʧ��*/
        if (ATG_DCI_RC_OK != ret)
        {
            /*�쳣����*/
            goto EXIT_LABEL;
        }

#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwCreate(unit, port,lsp_id,id,0);

        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_release_pw_id(id);
            spm_hqos_arad_create_pw_error_log(port ,lsp_id,
                id,__FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif    
    }



    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pst_tbl = (SPM_HQOS_VRF_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_VRF_CB),
        NBB_NORETRY_ACT, MEM_SPM_HQOS_VRF_CB);

    /*�����ڴ�ʧ�ܲ��ͷ�HQOS��Դ���ڴ�û���ˣ������͹���*/
    if (pst_tbl == NULL)
    {
        /*�쳣����*/
        goto EXIT_LABEL;
    }
    
    

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_HQOS_VRF_CB));
    OS_MEMCPY(&(pst_tbl->vrf_key), vrf_key, sizeof(ATG_DCI_VRF_INSTANCE_KEY));
    pst_tbl->pw_id = id;

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_hqos_vrf_node);

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_free_hqos_vrf_cb(SPM_HQOS_VC_CB *pst_tbl,NBB_LONG lsp_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_TRC_ENTRY(__FUNCTION__);

    /*�ͷ��ڴ�ʧ��*/
    if (NULL == pst_tbl)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        rv = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_VC_CB), MEM_SPM_HQOS_VC_CB);
    
    if(0 != pst_tbl->pw_id)
    {
#if defined (SPU) || defined (SRC)
        ret = ApiAradHqosPwDelete(0, pst_tbl->pw_id,lsp_id);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_del_pw_error_log(lsp_id,pst_tbl->pw_id,
                __FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif 

        ret = spm_hqos_release_pw_id(pst_tbl->pw_id);
        if (ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }
    

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_HQOS_VC_CB);
    pst_tbl = NULL;

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_del_vrf_drive(NBB_BYTE slot,NBB_USHORT port,SPM_HQOS_VRF_CB *vrf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num; 
    NBB_BYTE i = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == vrf)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }


    /***************************************************************************/
    /* �������VC�ϵ�����                                                    */
    /***************************************************************************/
    if(NULL != vrf)
    {
        /*NP hqos ȥʹ��*/
        if(0 != vrf->np_flow_id || 0 != vrf->posid)
        {
            for(unit = 0; unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetL3TxPwHqos(unit, vrf->posid, 0, vrf->np_flow_id);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_l3txpw_error_log(vrf->np_flow_id,0,vrf->posid,
                        __FUNCTION__,__LINE__,ret);
                    rv = ret;
                }
#endif
            }
        }


        /*ɾ��voq*/
        if( 0 != vrf->voq_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosVoqDelete(0, vrf->voq_id, NUM_COS, slot, port, vrf->tm_vc);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_voq_error_log(vrf->voq_id,slot,port,
                    vrf->tm_vc, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif    
         }

        /*ɾ��vc*/
        for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
        {
            if(0 != vrf->vc_id[i])
            {
#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosTrafficDelete(0, vrf->voq_id, NUM_COS, g_hqos_port_index[i],
                        port, vrf->vc_id[i],vrf->pw_id);
                if (ATG_DCI_RC_OK != ret)       
                {
                    spm_hqos_arad_traffic_del_error_log(vrf->voq_id,g_hqos_port_index[i],
                        port, vrf->vc_id[i],vrf->pw_id, __FUNCTION__,__LINE__,ret);
                    rv = ret;
                } 
#endif     
            }
        }
         
    }
  
   
    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_del_vrf_node(SPM_HQOS_LSP_CB **plsp,SPM_HQOS_VRF_CB **pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_VRF_CB *vrf = NULL;
    SPM_HQOS_LSP_CB *lsp = NULL;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_BYTE i = 0;
    NBB_LONG lsp_id = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP�ڵ㲻����ΪNULL*/
    if(NULL == plsp || NULL == *plsp || NULL == pcb)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    vrf = *pcb;
    lsp = *plsp;
    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;

    /***************************************************************************/
    /* �������VC�ϵ�����                                                    */
    /***************************************************************************/
    if(NULL != vrf)
    {
         ret = spm_hqos_del_vrf_drive(slot,port,vrf);
         if (ATG_DCI_RC_OK != ret) 
         {
             rv = ret;
         }
         
         /*ע�����ܸ澯*/
#ifdef PTN690
         spm_hqos_del_vrf_pmline(vrf->vrf_key);
#endif


        /*����ɾ���Ѵ��ڵ���ȷ����*/
        if(AVLL_IN_TREE(vrf->spm_hqos_vrf_node)
        {
            AVLL_DELETE(lsp->vrf_tree, vrf->spm_hqos_vrf_node);
        }

        /*�ͷ��ڴ���Դ*/
        ret = spm_free_hqos_vrf_cb(vrf,lsp_id);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }
        *pcb = NULL;/*ָ�����ΪNULL*/ 
         
    }
  
    ret = spm_hqos_del_lsp_node(plsp);
    if (ATG_DCI_RC_OK != ret) 
    {
        rv = ret;
    }

   
    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : ���������̵�VC
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
static NBB_LONG spm_hqos_set_vrf_pw_property(NBB_BYTE slot,NBB_LONG base_queue,NBB_LONG pw_id,
    NBB_ULONG posid,ATG_DCI_VPN_HQOS_POLICY *data,SPM_HQOS_VRF_CB *pcb)
{
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }


    ret = spm_hqos_set_pw_property(slot,pw_id,base_queue,data);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

   
     
    /*����̺ͳ����̶���ִ��*/
    /*�������C3��flow id hqos��ҵ��ͨ����*/
    for(unit = 0; unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL3TxPwHqos(unit, posid, 1, base_queue);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_l3txpw_error_log(base_queue,1,posid,
                __FUNCTION__,__LINE__,ret);
            goto EXIT_LABEL;
        }
#endif
    }

    if(NULL != pcb)
    {
        pcb->np_flow_id = base_queue;
        pcb->posid = posid;
        /*�������ݣ����ݲ�����Ӧ������*/
        OS_MEMCPY(&(pcb->vrf_data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY)); 
    }
    

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}





/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_set_vrf_drive(NBB_BYTE slot,NBB_USHORT port, NBB_LONG pw_id,
        NBB_ULONG posid, 
        ATG_DCI_VPN_HQOS_POLICY *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_VRF_CB*pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data)|| (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       goto EXIT_LABEL;
    }


    /*��������̳��ڵ�voq��vc�����ӹ�ϵHQOSҵ�����ͨ*/
    if(NULL == pcb)/*���ݱ�������ý���ˢ������*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,NULL,NULL,NULL);  
    }
    else/*�����յ������ô�����������*/
    {
        ret = spm_hqos_create_voq_vc(slot,port,pw_id,
            voq,vc,vc_id,&(pcb->voq_id),&(pcb->tm_vc),pcb->vc_id);  
    }
    
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }


    /*����VC�ڵ��HQOS������Ч*/
    ret = spm_hqos_set_vrf_pw_property(slot,posid,voq,data,pcb);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
static NBB_LONG spm_hqos_add_vrf(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
        SPM_QOS_TUNNEL_KEY *lsp_key, 
        ATG_DCI_VRF_INSTANCE_KEY *vrf_key,ATG_DCI_VPN_HQOS_POLICY *data,
        NBB_LONG voq,NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM])
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_LONG unit = 0;
    SPM_HQOS_VRF_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vrf_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (NULL == vc_id))
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    /*�����Ƿ����B�ڵ�*/
    plsp = AVLL_FIND(g_qos_port_cb[slot][port].lsp_tree, lsp_key);

    /*LSP�ڵ�HQOS�����ڣ�����Ĭ�ϵ�LSP�ڵ�*/
    if (plsp == NULL)
    {     
        //spm_dbg_record_(vrf_key,SPM_OPER_ADD);
        plsp = spm_alloc_hqos_lsp_cb(lsp_key,ATG_DCI_RC_OK,slot,port);/*������LSP ID*/

        /*�����ڴ�ʧ��*/
        if (NULL == plsp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        pcb = AVLL_FIND(plsp->vrf_tree, vrf_key);
        if(NULL != pcb)/*�쳣����Ӧ��ΪNULL*/
        {
            spm_qos_exception_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        pcb = spm_alloc_hqos_vrf_cb(vrf_key,plsp->lsp_id,slot,port);
        if (NULL == pcb)
        {
            spm_hqos_del_vrf_node(&plsp,&pcb);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ret = spm_hqos_set_vrf_drive(slot,port,pcb->pw_id,posid,data,voq,vc,vc_id,pcb);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_del_vrf_node(&plsp,&pcb);
            goto EXIT_LABEL;
        }

        /*ע�����ܸ澯*/
#ifdef PTN690
        spm_hqos_add_vrf_pmline(vrf_key,pcb->voq_id);
#endif


        rv = AVLL_INSERT(g_qos_port_cb[slot][port].lsp_tree, plsp->spm_hqos_lsp_tx_node);
        rv = AVLL_INSERT(plsp->vrf_tree, pcb->spm_hqos_vrf_node);
        
    }
    else/*LSP HQOS���ô��ڷ�Ĭ��LSP�ڵ�ͷ�Ĭ�Ͻڵ�֮��*/
    {
        pcb = AVLL_FIND(plsp->vrf_tree, vrf_key);
        if(NULL == pcb)/*�½��ڵ�*/
        {
             pcb = spm_alloc_hqos_vrf_cb(vrf_key,plsp->lsp_id,slot,port);
            if (NULL == pcb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

            ret = spm_hqos_set_vrf_drive(slot,port,pcb->pw_id,posid,data,voq,vc,vc_id,pcb);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_hqos_del_vrf_node(&plsp,&pcb);
                goto EXIT_LABEL;
            }

            /*ע�����ܸ澯*/
#ifdef PTN690
            spm_hqos_add_vrf_pmline(vrf_key,pcb->voq_id);
#endif
            rv = AVLL_INSERT(plsp->vrf_tree, pcb->spm_hqos_vrf_node);

        }
        else/*����*/
        {
            /*ֻ����TXPW ��hqos��flow id*/
            for(unit = 0; unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetL3TxPwHqos(unit, posid, 1, pcb->voq_id);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_l3txpw_error_log(pcb->voq_id,1,posid,
                        __FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }
#endif
            }
        }
        
    }
    

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_add_vrf_node(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
    SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VRF_INSTANCE_KEY *vrf_key,
    ATG_DCI_VPN_HQOS_POLICY *data)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VC_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    CRTXLSP_KEY lsptx_key = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG voq = 0;
    NBB_LONG vc = 0;
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM] = {0};
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (port > 0x80))/*��֧��LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }
    
    qos_log_vc_hqos_data(slot,port,posid,lsp_key,vrf_key,data);
    
        
    /* �ж�slot�Ƿ���hqos�Ŀ��� */
    ret = spm_hqos_check_slot(slot,data->node_index,&voq,&vc,vc_id);
    if (ATG_DCI_RC_OK != ret)/*û�п���HQOS��λʹ��ֱ������������*/
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }


    /*������L3ģ��ֱ�Ӹ��ң������Լ�����*/
    if((0 != lsp_key->type) && (0 == lsp_key->tx_lsp.lspid)) /*LSP����ΪCRLSP_TX*/
    {
        OS_MEMCPY(&lsptx_key,&(lsp_key->tx_lsp),sizeof(CRTXLSP_KEY));
        ret = spm_l3_gettxlspid(&lsptx_key, &(lsp_key->tx_lsp.lspid) NBB_CCXT);
        if(ATG_DCI_RC_OK != ret)
        {
            OS_SPRINTF(message,"%s,%d spm_l3_rsvpforoam err=%ld\n\n",
                       __FUNCTION__,__LINE__,ret);
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
        			HQOS_VC_ERROR,ret,
        			message,__FUNCTION__,"line","spm_l3_rsvpforoam",
        			0,0,__LINE__,0));

            /*�쳣����*/
            goto EXIT_LABEL;
        }
    }
    
    ret = spm_hqos_add_vc( slot,  port,  posid,proflag, lsp_key, vc_key, *data,
                    voq, vc, vc_id[MAX_HQOS_SLOT_NUM]);

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 7



/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ沢����lsp id ��ʼ��HQOS��
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
SPM_QOS_USER_GROUP_CB *spm_alloc_hqos_usr_group_cb(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_USER_GROUP_CB *pstTbl = NULL;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY(__FUNCTION__);


    /* ����һ���µ�log group���ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_QOS_USER_GROUP_CB *)NBB_MM_ALLOC(sizeof(SPM_QOS_USER_GROUP_CB),
        NBB_NORETRY_ACT, MEM_SPM_USER_GROUP_CB);

    /*�����ڴ�ʧ��*/
    if (pstTbl == NULL)
    {
        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /* ��ʼlog group���ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_QOS_USER_GROUP_CB));
    pstTbl->index = index;



    /*QOS USR ����ʼ��*/
    avll_key_offset = NBB_OFFSETOF(SPM_HQOS_LOG_USR_CB, index);
    AVLL_INIT_TREE(pstTbl->usr_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_LOG_USR_CB, spm_hqos_usr_node));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstTbl->spm_hqos_group_node);


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
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
NBB_LONG spm_free_hqos_user_group_cb(SPM_QOS_USER_GROUP_CB *pst_tbl)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if (NULL == pst_tbl)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstTbl, sizeof(SPM_QOS_USER_GROUP_CB), MEM_SPM_USER_GROUP_CB);

    if(0 != pst_tbl->lsp_id)
    {
        ret = spm_hqos_del_lsp_pw(pst_tbl->lsp_id, pst_tbl->port,0);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }

        ret = spm_hqos_release_lsp_pw_id(pst_tbl->lsp_id,0);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }


    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_USER_GROUP_CB);
    pstTbl = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
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
NBB_LONG spm_set_down_user_group_cfg(NBB_ULONG oper, SPM_QOS_USER_GROUP_CB *pcb,
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_VPN_HQOS_POLICY *data = NULL;
   

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pcb)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* ��Ϣ���� */
    switch (oper)
    {
        /* ���� */
        case  ATG_DCI_OPER_UPDATE:
        NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        break;

        /* ɾ�� */
        case  ATG_DCI_OPER_DEL:
        if(0 != pcb->lsp_id)/*ɾ����������*/
        {
#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosLspBandwidthSet(0, pcb->lsp_id, 0x8fffffff,0x8fffffff, 
                    0x8fffffff, 0x8fffffff);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_arad_set_lsp_bandwidth_error_log(pcb->lsp_id, 0x8fffffff,0x8fffffff,
                        0x8fffffff, 0x8fffffff,
                        __FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }
#endif
        }
        OS_MEMSET(&(pcb->data), 0XFF, sizeof(ATG_DCI_VPN_HQOS_POLICY));
        break;

        /* ���� */
        case  ATG_DCI_OPER_ADD:

        /* ips��Ϣ����Ϊ�� */
        if (NULL != buf)
        {
            data = (ATG_DCI_VPN_HQOS_POLICY *)buf;

            if(0 != pcb->lsp_id)/*�Ѿ���ʵ����*/
            {
#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosLspBandwidthSet(0, pcb->lsp_id, data->normal_shaping.cir,
                      data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_arad_set_lsp_bandwidth_error_log(pcb->lsp_id, data->normal_shaping.cir,
                      data->normal_shaping->pir, data->normal_shaping->cbs, data->normal_shaping->pbs,
                        __FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }
#endif

#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosLspWfqWeightSet(0, pcb->lsp_id, data->normal_strategy.weight);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_arad_set_lsp_weight_error_log(pcb->lsp_id,
                        data->normal_strategy.weight,__FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }
#endif
            } 
            OS_MEMCPY(&(pcb->data), data, sizeof(ATG_DCI_VPN_HQOS_POLICY));
        }
        break;

        default:
        break;
    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
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
NBB_VOID spm_rcv_dci_set_user_group(ATG_DCI_SET_USER_GROUP *pst_set_ips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG ulkey = 0;
    SPM_QOS_USER_GROUP_CB *pstCb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_LONG ips_ret = ATG_DCI_RC_OK;

    NBB_BYTE *down_data_start = NULL;

    NBB_ULONG oper_down = ATG_DCI_OPER_NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pst_set_ips)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        goto EXIT_LABEL;
    }

    pst_set_ips->return_code = ATG_DCI_RC_OK;
    ulkey = pst_set_ips->key;
    

    oper_down = pst_set_ips->oper_down_qos;
    down_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_ips, &pst_set_ips->down_qos_data);

    pstCb = AVLL_FIND(g_qos_user_group_tree, &ulkey);
    if (TRUE == pst_set_ips->delete_struct)
    {
        if(NULL != pstCb)/*���ô��ڲ�ɾ��*/
        {
            if(NULL != AVLL_FIRST(pstCb->usr_tree))/*�а󶨹�ϵ������ɾ��*/
            {
                pst_set_ips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
            else/*��Ϊ�������ɾ��group�ڵ�*/
            {
               rv = AVLL_DELETE(g_qos_user_group_tree, pstCb->spm_qos_user_group_node);
               spm_free_hqos_user_group_cb(pstCb); 
            }
        }   
    }
    else/* ���»������� */
    {
        if(NULL == pstCb)/*����*/
        {
            pstCb = spm_alloc_hqos_usr_group_cb(ulkey);
            if (NULL == pstCb)
            {
                pst_set_ips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

            ret = spm_set_down_user_group_cfg(oper_down, pstCb, down_data_start);
            if(ATG_DCI_RC_OK != ret)
            {
                pst_set_ips->return_code = pst_set_ips->down_qos_return_code = ret;
                spm_free_hqos_user_group_cb(pstCb);
            }   
            rv = AVLL_INSERT(g_qos_user_group_tree, pstCb->spm_qos_user_group_node);
        }
        else/*����*/
        {
            ret = spm_set_down_user_group_cfg(oper_down, pstCb, down_data_start);
            if(ATG_DCI_RC_OK != ret)
            {
                pst_set_ips->return_code = pst_set_ips->down_qos_return_code = ret;
            }  
        }

    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}



/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �����ڴ��ͬʱ����PW ID
   �������  : defaulte = ATG_DCI_RC_OK:����LSP ID
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
SPM_HQOS_LOG_USR_CB *spm_alloc_hqos_log_usr_cb(NBB_ULONG index,NBB_LONG *lsp_id,
     NBB_BYTE slot,NBB_USHORT port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_LOG_USR_CB *pstTbl = NULL;
    NBB_ULONG ret = ATG_DCI_RC_OK;
    NBB_ULONG pw_id = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*����������ID��Դ����A,B�ڵ�*/
    if(slot == v_spm_shared->local_slot_id)
    {
        ret = spm_hqos_apply_lsp_pw_id(lsp_id,&pw_id);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        } 

        if(NULL != lsp_id)
        {
             ret = spm_hqos_create_lsp_pw(*lsp_id,port,pw_id);
             if(ATG_DCI_RC_OK != ret)
             {
                 spm_hqos_release_lsp_pw_id(*lsp_id,pw_id);
                 goto EXIT_LABEL;
             }   
        }
        else
        {
             ret = spm_hqos_create_pw(0,port,pw_id);
             if(ATG_DCI_RC_OK != ret)
             {
                 spm_hqos_release_lsp_pw_id(0,pw_id);
                 goto EXIT_LABEL;
             } 
        }
     
        
    }

    ret = spm_hqos_apply_pw_id(&pw_id);

    /*����pwidʧ��*/
    if (ATG_DCI_RC_UNSUCCESSFUL == ret)
    {
        printf("**QOS ERROR** %s %d spm_hqos_apply_pw_id failed\n",
                    __FUNCTION__,__LINE__);

        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pstTbl = (SPM_HQOS_LOG_USR_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_LOG_USR_CB),
        NBB_NORETRY_ACT, MEM_SPM_HQOS_LOG_USR_CB);

    /*�����ڴ�ʧ��*/
    if (pstTbl == NULL)
    {

        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pstTbl, 0, sizeof(SPM_HQOS_LOG_USR_CB));
    pstTbl->index = index;
    pstTbl->pw_id = pw_id;
    AVLL_INIT_NODE(pstTbl->spm_hqos_usr_node);

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pstTbl);
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_add_usr_node(SUB_PORT *sub_port,
    NBB_ULONG logic_key,
    NBB_ULONG group_key,
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *data)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_LOG_USR_CB *pcb = NULL;
    SPM_QOS_USER_GROUP_CB *pB = NULL;
    TX_PORT_T l3uni = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    NBB_LONG voq = 0;
    NBB_LONG vc = 0;
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM] = {0};

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data) || (NULL == sub_port) || (sub_port->port > 0x80))
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /* �ж�slot�Ƿ���hqos�Ŀ��� */
    ret = spm_hqos_check_slot(sub_port->slot,data->node_index,&voq,&vc,vc_id);
    if (ATG_DCI_RC_OK != ret)/*û�п���HQOS��λʹ��ֱ������������*/
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }


    l3uni.slot = sub_port->slot;
    l3uni.port = sub_port->port;
    l3uni.vid = sub_port->ovlan;

    /*�����Ƿ����B�ڵ�*/
    pB = AVLL_FIND(g_qos_user_group_tree, &group_key);

    /* �����Ŀ�����������Ĭ��B�ڵ��LSP ID*/
    if (pB == NULL)
    {
        printf("**QOS ERROR**%s,%d can't find group node\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"HQOS vrf pw config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
						"slot","port","posid","ovlan",
				        sub_port->slot,sub_port->port,
						sub_port->posid,sub_port->ovlan));
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;

    }

    pcb = AVLL_FIND(pB->usr_tree, &logic_key);
    
    if(0 == pB->lsp_id)/*�״ΰ󶨵��û���*/
    {
        if (pcb == NULL)/*һ��Ϊ�ղ�����*/
        {
            pcb = spm_alloc_hqos_log_usr_cb(logic_key);
            if (NULL == pcb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }
           
        }
    }
    else
    {
        if((sub_port->slot != pB->slot) || (sub_port->port != pB->port))
        {
            printf("**QOS ERROR**%s,%d slot port cfg miss macth,old=%d,new=%ld\n",
                __FUNCTION__,__LINE__,sub_port->port,pB->port);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    						"HQOS node_index cfg miss macth",ATG_DCI_RC_UNSUCCESSFUL,
    						"slot","port","old node","new node",
    				        sub_port->slot,sub_port->port,
    						pB->slot,pB->port));
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /*�쳣����*/
            goto EXIT_LABEL;
        }
    }
    
    
    if ((0 != pcb->usr_data.node_index) && (pcb->usr_data.node_index != data->node_index))
    {
        printf("**QOS ERROR**%s,%d node_index cfg miss macth,old=%ld,new=%ld\n",
            __FUNCTION__,__LINE__,pcb->usr_data.node_index,data->node_index);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"HQOS node_index cfg miss macth",ATG_DCI_RC_UNSUCCESSFUL,
						"slot","port","old node","new node",
				        sub_port->slot,sub_port->port,
						pcb->usr_data.node_index,data->node_index));
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    old_index = (pcb->usr_data.user_qos_policy_index) | (pcb->usr_data.prio_queue_qos_policy_index);
    index = (data->user_qos_policy_index) | (data->prio_queue_qos_policy_index);
    flag = ((pcb->usr_data.user_qos_policy_index != data->user_qos_policy_index)
           || (pcb->usr_data.prio_queue_qos_policy_index != data->prio_queue_qos_policy_index));
    if ((0 == old_index) && (0 != index)) /*����*/
    {
        spm_dbg_record_qos_logic_subport_head(sub_port,logic_key,SPM_OPER_ADD NBB_CCXT);
#if defined (SPU) || defined (SRC)

        /* ��������̺ͳ�����֮���ȫ���� */
        ret = ApiAradHqosVoqSet(UNIT_0, voq, NUM_COS, sub_port->slot, sub_port->port + PTN_690_PORT_OFFSET, baseVCId);
#endif
        if (ATG_DCI_RC_OK != ret)       
        {
            spm_qos_subport_pd(sub_port,logic_key,HQOS_VC_ERROR,ret NBB_CCXT);
            spm_hqos_arad_voq_error_log(voq,sub_port->slot,sub_port->port + PTN_690_PORT_OFFSET,
                baseVCId, __FUNCTION__,__LINE__,ret NBB_CCXT);
            spm_free_hqos_log_usr_cb(pcb NBB_CCXT);

            /*�쳣����*/
            goto EXIT_LABEL;
        }
        ret = spm_hqos_add_log_usr(sub_port->slot, sub_port->port, pcb, pB,
            data, ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
        if (ATG_DCI_RC_OK != ret) /*�����������ش���*/
        {
#if defined (SPU) || defined (SRC)

            /* ɾ������̺ͳ�����֮���voqȫ���� */
            ApiAradHqosVoqDelete(UNIT_0, voq, sub_port->slot, sub_port->port + PTN_690_PORT_OFFSET, baseVCId);
#endif

            /*�쳣����*/
            goto EXIT_LABEL;
        }
#if defined (SPU) || defined (PTN690_CES)
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
            if(0 != sub_port->posid)
            {

                ret += ApiC3SetVpHqos(unit,sub_port->posid,0, 1, voq);
            }
            else
            {
                /*�����»�l3uni��hqos:��hqosʹ�ܿ��غ�flowid���ݸ�c3����*/
                ret += ApiC3SetL3UniHqos(unit, 0,&l3uni, 1, voq);
            }
        }
        if (ATG_DCI_RC_OK != ret)
        {
            spm_qos_subport_pd(sub_port,logic_key,HQOS_USR_ERROR,ret NBB_CCXT);
            if(0 == sub_port->posid)
            {
                spm_hqos_l3uni_error_log(voq,1,&l3uni,__FUNCTION__,__LINE__,ret NBB_CCXT);
            }
            else
            {
                spm_hqos_vp_error_log(voq,1,sub_port->posid,0,__FUNCTION__,__LINE__,ret NBB_CCXT);
            }
#if defined (SPU) || defined (SRC)

            /* ɾ������̺ͳ�����֮���voqȫ���� */
            ApiAradHqosVoqDelete(UNIT_0, voq, sub_port->slot, sub_port->port, baseVCId);
#endif
            spm_hqos_del_log_usr(sub_port->slot, sub_port->port, pB,
                pcb,data->node_index NBB_CCXT);

            /*�쳣����*/
            goto EXIT_LABEL;
        }
#endif 
#ifdef PTN690
        spm_hqos_add_intf_pmline(logic_key,voq NBB_CCXT);
#else
#if defined (SPU) || defined (PTN690_CES)
        rv = almpm_addline_hqos(data->node_index,sub_port->slot,voq);
        if (ATG_DCI_RC_OK != rv)
        {
            spm_hqos_almpm_addline_error_log(voq,data->node_index,sub_port->slot,
                __FUNCTION__,__LINE__,rv NBB_CCXT);
        }
#endif 
#endif
        OS_MEMCPY(&(pcb->usr_data), data, sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));
        pcb->flow_id = voq;
        pB->slot = sub_port->slot;
        pB->port = sub_port->port;
        rv = AVLL_INSERT(pB->usr_tree, pcb->spm_hqos_usr_node);

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else if ((0 != old_index) && (0 != index) && (0 != flag)) /*����A�ڵ�policy index*/
    {
        spm_dbg_record_qos_logic_subport_head(sub_port,logic_key,SPM_OPER_UPD NBB_CCXT);
        ret = spm_hqos_add_log_usr(sub_port->slot, sub_port->port, pcb,
            pB, data, ATG_DCI_RC_OK NBB_CCXT);
        if (ATG_DCI_RC_OK != ret) /*�����������ش���*/
        {
        
            /*�쳣����*/
            goto EXIT_LABEL;
        }
        if (sub_port->slot == fapid) /*���ü���ֻ�ڳ�����*/
        {
            ret = spm_disconnect_policy_cnt(pcb->usr_data.user_qos_policy_index,
                ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
            if (ATG_DCI_RC_OK != ret) /*���ü�����1ʧ��*/
            {
            
                /*�쳣����*/
                goto EXIT_LABEL;
            }
            ret = spm_disconnect_policy_cnt(pcb->usr_data.prio_queue_qos_policy_index,
                ATG_DCI_RC_OK NBB_CCXT);
            if (ATG_DCI_RC_OK != ret) /*���ü�����1ʧ��*/
            {

                /*�쳣����*/
                goto EXIT_LABEL;
            }
        }
        OS_MEMCPY(&(pcb->usr_data), data, sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));
        goto EXIT_LABEL;
    }
    else if ((0 != old_index) && (0 != index) && (0 == flag)) /*ͬ��������*/
    {
        printf("**QOS ERROR**%s,%d the same index=%ld\n",
                __FUNCTION__,__LINE__,index);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					"HQOS the same index",ret,
					"policy index","","","",
					index,0,0,0));

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else if ((0 != old_index) && (0 == index)) /*ɾ��A�ڵ�*/
    {
        spm_dbg_record_qos_logic_subport_head(sub_port,logic_key,SPM_OPER_DEL NBB_CCXT);
        if (sub_port->slot == fapid) /*���ü���ֻ�ڳ�����*/
        {
            spm_disconnect_policy_cnt(pcb->usr_data.user_qos_policy_index,
                ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
            spm_disconnect_policy_cnt(pcb->usr_data.prio_queue_qos_policy_index,
                ATG_DCI_RC_OK NBB_CCXT);

        }
#if defined (SPU) || defined (PTN690_CES)
        for(unit = 0; unit < SHARED.c3_num;unit++)
        {
            if(0 != sub_port->posid)
            {
                rv += ApiC3SetVpHqos(unit,sub_port->posid,0, 0, voq);
            }
            else
            {
                rv += ApiC3SetL3UniHqos(unit, 0,&l3uni, 0, 0);
            }
        }
        if (ATG_DCI_RC_OK != rv)
        {
            if(0 == sub_port->posid)
            {
                spm_hqos_l3uni_error_log(voq,0,&l3uni,__FUNCTION__,__LINE__,ret NBB_CCXT);
            }
            else
            {
                spm_hqos_vp_error_log(voq,0,sub_port->posid,0,__FUNCTION__,__LINE__,ret NBB_CCXT);
            }
            ret = rv;
        }
#endif

#if defined (SPU) || defined (SRC)

        /* ɾ������̺ͳ�����֮���voqȫ���� */
        rv = ApiAradHqosVoqDelete(UNIT_0, voq, sub_port->slot, sub_port->port + PTN_690_PORT_OFFSET, baseVCId);
#endif
        if (ATG_DCI_RC_OK != rv)
        {
            printf("**QOS ERROR**%s,%d ApiAradHqosVoqDelete err: %ld (baseQueue=%ld,slot=%d,port=%d,vc=%ld)\n",
                __FUNCTION__,__LINE__,rv, voq, 
                sub_port->slot, sub_port->port, baseVCId);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"HQOS del voq",rv,
						"baseQueue","slot","port","baseVCId",
						voq,sub_port->slot,sub_port->port,baseVCId));
            ret = rv;
        }
#ifdef PTN690
		spm_hqos_del_intf_pmline(logic_key NBB_CCXT);
#else
#if defined (SPU) || defined (PTN690_CES)
        rv = almpm_delline_hqos(data->node_index,sub_port->slot);
        if (ATG_DCI_RC_OK != rv)
        {
            printf("**QOS ERROR**%s,%d almpm_delline_hqos,node_index=%ld",
                __FUNCTION__,__LINE__,data->node_index);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"HQOS almpm_delline_hqos",rv,
						"baseQueue","slot","port","baseVCId",
						voq,sub_port->slot,sub_port->port,baseVCId));
            ret = rv;
        }
#endif
#endif
        AVLL_DELETE(pB->usr_tree, pcb->spm_hqos_usr_node);
        rv = spm_hqos_del_log_usr(sub_port->slot, sub_port->port, 
            pB, pcb,data->node_index NBB_CCXT);
        if (ATG_DCI_RC_OK != rv) /*���ش���*/
        {
            ret = rv;
        }

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else if ((0 == old_index) && (0 == index))
    {
        printf("**QOS ERROR**%s,%d usr hqos config err\n",
                __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					"HQOS usr hqos config missmatch",ret,
					"node_index","user_qos_policy_index",
					"prio_queue_qos_policy_index","group_key",
					data->node_index,data->user_qos_policy_index,
					data->prio_queue_qos_policy_index,group_key));
        spm_free_hqos_log_usr_cb(pcb NBB_CCXT);
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else /*�Ƿ�����*/
    {
        printf("**QOS ERROR**%s,%d usr hqos config missmatch\n",
                __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					"HQOS usr hqos config missmatch",ret,
					"node_index","user_qos_policy_index",
					"prio_queue_qos_policy_index","group_key",
					data->node_index,data->user_qos_policy_index,
					data->prio_queue_qos_policy_index,group_key));
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

/*****************************************************************************
   �� �� ��  : spm_disconnect_usr_group_cnt
   ��������  : �ͷ�HQOS���Ľڵ㲢�ͷ��ڴ�
   �������  : upflag=0�������
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���
*****************************************************************************/
NBB_LONG spm_hqos_add_group_node(NBB_USHORT slot, NBB_LONG port, 
    NBB_ULONG index, NBB_ULONG log_key)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_LOG_GROUP_CB *pcb = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_BYTE fapid = 0;
    SPM_QOS_USER_GROUP_CB *p_group = NULL;
    SPM_HQOS_LOG_GROUP_CB *group_cb = NULL;
    NBB_ULONG policy_index = 0;
    NBB_CHAR ucMessage[QOS_MSG_INFO_LEN];

    NBB_TRC_ENTRY(__FUNCTION__);

    if(0 == slot)
    {
        printf("**QOS ERROR**%s,%d,param=NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"QOS param ERROR",ATG_DCI_RC_UNSUCCESSFUL,
						"","","","",
						0,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;
    }
        
    /* �ж�slot�Ƿ���hqos�Ŀ��� */
    ret = spm_hqos_checkslot(slot, NULL NBB_CCXT);
    if (ATG_DCI_RC_OK != ret)
    {
        ret = ATG_DCI_RC_OK;

        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /* ��ȡ����λ�Ĳ�λ�� */
    spm_hardw_getslot(&fapid);
        
    /* �ж�slot�Ƿ���hqos�Ŀ��� */
    ret = spm_hqos_checkslot(fapid, NULL NBB_CCXT);
    if (ATG_DCI_RC_OK != ret)
    {
        ret = ATG_DCI_RC_OK; 

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    if(ATG_DCI_RC_OK != qos_log_cfg_print)
    {
        printf("%s %s,LINE=%d slot=%d,port=%ld,usr gourp index=%ld,log_key=%ld \n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,
            slot,port,index,log_key);
        OS_SPRINTF(ucMessage,"%s %s,%d slot=%d,port=%ld,usr gourp index=%ld,log_key=%ld \n",
            QOS_CFG_STRING,__FUNCTION__,__LINE__,
            slot,port,index,log_key);
       BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, ucMessage);
    }
    
    if(port >= 0x80)
    {
        ret = ATG_DCI_RC_OK;

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    
    if (0 == index)
    {
        policy_index = index;
        for (group_cb = (SPM_HQOS_LOG_GROUP_CB *)AVLL_FIRST(v_spm_shared->qos_port_cb[slot - 1][port].group_tree);
            group_cb != NULL;
            group_cb = (SPM_HQOS_LOG_GROUP_CB *)AVLL_NEXT(v_spm_shared->qos_port_cb[slot - 1][port].group_tree, 
                        group_cb->spm_hqos_group_node))
        {
            if((group_cb->slot == slot) && (group_cb->port == port))
            {
                pcb = group_cb;
                break;
            }
        }
    }
    else
    {
        /* ����û���ڵ�������Ƿ�Ϊ�� */
        p_group = spm_check_group_down_cb(index,slot, port, &policy_index NBB_CCXT);
        if ((NULL == p_group) || (0 == policy_index))
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /*�쳣����*/
            goto EXIT_LABEL;
        }

        /* group_tree:���˿ڳ���λ�����󶨵��»��û���QoS���õ���; */
        /*            ����,������keyֵΪ�û���ڵ������ֵindex.   */
        pcb = AVLL_FIND(SHARED.qos_port_cb[slot - 1][port].group_tree, &index);
    }

    /******************************����B(LSP)�ڵ�************************************/
    if ((pcb == NULL) && (0 != index))
    {
        spm_dbg_record_qos_logic_head(log_key,SPM_OPER_ADD NBB_CCXT);
        pcb = spm_alloc_hqos_log_group_cb(index NBB_CCXT);
        if (NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /*�쳣����*/
            goto EXIT_LABEL;
        }
        ret = spm_hqos_add_log_group(slot, port, pcb, ATG_DCI_RC_UNSUCCESSFUL, policy_index NBB_CCXT);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_free_hqos_log_group_cb(pcb NBB_CCXT);

            /*�쳣����*/
            goto EXIT_LABEL;
        }
        if (NULL == p_group)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            spm_free_hqos_log_group_cb(pcb NBB_CCXT);

            /*�쳣����*/
            goto EXIT_LABEL;
        }
        if (slot == fapid)
        {      
            (p_group->cnt)++;    
        }
        pcb->policy_index = policy_index;
        pcb->group_index = index;
        pcb->slot = slot;
        pcb->port = port;
        p_group->port = port;
        p_group->slot = slot;
        p_group->lsp_id = pcb->lsp_id;
        
        rv = AVLL_INSERT(SHARED.qos_port_cb[slot - 1][port].group_tree, pcb->spm_hqos_group_node);

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else if ((NULL != pcb) && (0 != pcb->group_index) && (0 != index) && (pcb->group_index != index)) /*ǿ�Ƹ���*/
    {
        spm_dbg_record_qos_logic_head(log_key,SPM_OPER_UPD NBB_CCXT);
#if 0
        ret = spm_hqos_add_log_group(slot, port, pcb, ATG_DCI_RC_OK, index, nhi);

        if (ATG_DCI_RC_OK != ret) /*����ʧ��*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "**ERROR** update lsp policy failed"));
            goto EXIT_LABEL;
        }

        if (slot == fapid)
        {
            (p_group->cnt)++;
        }
        pcb->policy_index = policy_index;
        pcb->group_index = index;
        pcb->slot = slot;
        p_group->port = port;

        p_group = spm_find_group_cb(pcb->group_index);
        p_group->port = 0;

        if (slot == fapid) /*���ü���ֻ�ڳ�����*/
        {
            ret = spm_disconnect_policy_cnt(pcb->policy_index);

            if (ATG_DCI_RC_OK != ret) /*���ü�����1ʧ��*/
            {
                goto EXIT_LABEL;
            }

            ret = spm_disconnect_usr_group_cnt(pcb->group_index);

            if (ATG_DCI_RC_OK != ret)
            {
                goto EXIT_LABEL;
            }
        }
#endif
        printf("**QOS ERROR**%s,LINE=%d different group old=%ld,new=%ld\n",
                __FUNCTION__,__LINE__,pcb->group_index,index);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					"HQOS group hqos config missmatch",ATG_DCI_RC_UNSUCCESSFUL,
					"old group","new group","slot","port",
					pcb->group_index,index,slot,port));
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else if ((NULL != pcb) && (0 != pcb->group_index) && (0 != index) && (pcb->group_index == index)) /*һ��������*/
    {
        printf("**QOS ERROR**%s,LINE=%d the same group cfg old=%ld,new=%ld\n",
                __FUNCTION__,__LINE__,pcb->group_index,index);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
					"HQOS the same group cfg",ret,
					"old group","new group","slot","port",
					pcb->group_index,index,slot,port));

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else if ((NULL != pcb) && (0 != pcb->group_index) && (0 == index)) /*ɾ��B�ڵ�����*/
    {
        spm_dbg_record_qos_logic_head(log_key,SPM_OPER_DEL NBB_CCXT);
        
        /*���A�ڵ㻹��������������ɾ��B�ڵ�*/
        if (NULL != AVLL_FIRST(pcb->usr_tree))
        {
            printf("**QOS ERROR**%s,LINE=%d usr tree is in used can't del group\n",
                __FUNCTION__,__LINE__);
            NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					"HQOS usr tree is in used can't del group",ret,
    					"old group","new group","slot","port",
    					pcb->group_index,index,slot,port));
            ret = ATG_DCI_RC_UNSUCCESSFUL;

            /*�쳣����*/
            goto EXIT_LABEL;
        }
        if (slot == fapid) /*���ü���ֻ�ڳ�����*/
        {
            spm_disconnect_policy_cnt(pcb->policy_index,ATG_DCI_RC_UNSUCCESSFUL NBB_CCXT);
            spm_disconnect_usr_group_cnt(pcb->group_index NBB_CCXT);
        }
        p_group = spm_find_group_cb(pcb->group_index NBB_CCXT);
        if((NULL != p_group) && (0 == p_group->cnt))
        {
            p_group->port = 0;
            p_group->slot = 0;
        }

        /*����ɾ��B�ڵ㺯��*/
        ret = spm_hqos_del_log_group(slot, port, pcb NBB_CCXT); 

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else if ((pcb == NULL) && (0 == index))
    {
        printf("**QOS ERROR**%s,LINE=%d group has been deleted\n",
                __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					"HQOS group has been deleted",ret,
    					"pcb==null","new group","slot","port",
    					0,index,slot,port));

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    else
    {
        printf("**QOS ERROR**%s,LINE=%d policy index config missmatch\n",
                __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    					"HQOS policy index config missmatch",ret,
    					"","new group","slot","port",
    					0,index,slot,port));
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}
#endif

