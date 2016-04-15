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
int fhdrv_psn_common_set_txlsp_hqos(int unit, uint32 txlsp_posid, int hqos_enable, uint32 hqos_flow_id) 
{
    return 0;
}


/*************************************************************************/
/**************************************************************************/
/*************************************************************************/
#define HQOS_TAG       "QOS HQOS CFG"

/*LOG�����С��ض���*/
#define HQOS_MSG_INFO_LEN    300 

/*DSģ��ȫ�ִ�ӡ����*/
NBB_BYTE g_qos_hqos_print = ATG_DCI_RC_UNSUCCESSFUL;

/*DSģ��ȫ��LOG����*/
NBB_BYTE g_qos_hqos_log = ATG_DCI_RC_UNSUCCESSFUL;

NBB_BYTE g_hqos_port_index[MAX_HQOS_SLOT_NUM] = {1,2,3,4};

/***************************************************************************/
/* ����û�������*/
/***************************************************************************/
AVLL_TREE g_qos_user_group_tree;

SPM_QOS_PORT_CB g_qos_port_cb[MAX_SLOT_NUM + 1][MAX_PHYSIC_PORT_NUM];

/***************************************************************************/
/* ���Hqos LSP ��Դ*/
/***************************************************************************/
NBB_BYTE g_hqos_lsp_pool[HQOS_MAX_LSP_ID];

/***************************************************************************/
/* ���Hqos PW ��Դ*/
/***************************************************************************/
NBB_BYTE g_hqos_pw_pool[HQOS_MAX_PW_ID];

static char* qos_cfg_oper_type_string[] = {"ADD", "DEL","UPD", "INIT_ADD", "FRESH", NULL,NULL};

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
NBB_VOID spm_dbg_record_hqos_vc_head(ATG_DCI_VC_KEY *vc_key, NBB_BYTE uc_oper)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];
    if(NULL != vc_key)
    {
        if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            printf("################### VC (vc_id=%ld,peer_ip=0x%lx,"
            "vc_type=%d, %s) ###################\n",
                    vc_key->vc_id,vc_key->peer_ip,vc_key->vc_type,
                    *(qos_cfg_oper_type_string + uc_oper));
        }
        if(ATG_DCI_RC_OK != g_qos_hqos_log)
        {
            OS_SPRINTF(message,"################### VC (vc_id=%ld,peer_ip=0x%lx,"
            "vc_type=%d, %s) ###################\n",
                    vc_key->vc_id,vc_key->peer_ip,vc_key->vc_type,
                    *(qos_cfg_oper_type_string + uc_oper));
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, message);  
        }
        
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
NBB_VOID spm_dbg_record_hqos_vrf_head(ATG_DCI_VRF_INSTANCE_KEY *pkey, 
    NBB_BYTE uc_oper)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];
    if(NULL != pkey)
    {
        if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            printf("################### VRF (vrf_id=%d,peer_ip=0x%lx,"
            " %s) ###################\n",
                    pkey->vrf_id,pkey->peer_ip,
                    *(qos_cfg_oper_type_string + uc_oper));
        }
        if(ATG_DCI_RC_OK != g_qos_hqos_log)
        {
            OS_SPRINTF(uc_message,"################### VRF (vrf_id=%d,peer_ip=0x%lx,"
            "%s) ###################\n",
                    pkey->vrf_id,pkey->peer_ip,
                    *(qos_cfg_oper_type_string + uc_oper));
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);  
        } 
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
NBB_VOID spm_dbg_record_hqos_lsp_head(SPM_QOS_TUNNEL_KEY *lsp_key,
    NBB_BYTE uc_oper)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if(0 == lsp_key->type)/*FTN*/
    {
        if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            printf("###################FTN(vrfid=%d,fec=%ld,"
                "mask=%d %s) ###################\n",
              lsp_key->ftn.vrfid,lsp_key->ftn.fec,lsp_key->ftn.mask,
              *(qos_cfg_oper_type_string + uc_oper));
        }
        if(ATG_DCI_RC_OK != g_qos_hqos_log)
        {
            OS_SPRINTF(uc_message,"###################FTN(vrfid=%d,fec=%ld,"
                "mask=%d %s) ###################\n",
              lsp_key->ftn.vrfid,lsp_key->ftn.fec,lsp_key->ftn.mask,
              *(qos_cfg_oper_type_string + uc_oper));
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
        
    }
    else
    {
        if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            printf("###################TXLSP(tunnelid=%ld,lspid=%ld,ingress=%ld,"
                "egress=%d %s) ###################\n",
              lsp_key->tx_lsp.tunnelid,lsp_key->tx_lsp.lspid,
              lsp_key->tx_lsp.ingress,lsp_key->tx_lsp.egress,
              *(qos_cfg_oper_type_string + uc_oper));
        }
        if(ATG_DCI_RC_OK != g_qos_hqos_log)
        {
            OS_SPRINTF(uc_message,"###################TXLSP(tunnelid=%ld,lspid=%ld,ingress=%ld,"
                "egress=%d %s) ###################\n",
              lsp_key->tx_lsp.tunnelid,lsp_key->tx_lsp.lspid,
              lsp_key->tx_lsp.ingress,lsp_key->tx_lsp.egress,
              *(qos_cfg_oper_type_string + uc_oper));
            BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        }
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
NBB_VOID spm_dbg_record_hqos_logic_head(NBB_ULONG logic_key,
    NBB_BYTE uc_oper)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if(ATG_DCI_RC_OK != g_qos_hqos_print)
    {
        printf("################### LOGIC KEY (logic_key=%ld, %s) ###################\n",
                logic_key,*(qos_cfg_oper_type_string + uc_oper));
    }
    if(ATG_DCI_RC_OK != g_qos_hqos_log)
    {
        OS_SPRINTF(uc_message,"#################LOGIC KEY (logic_key=%ld, %s)#################\n",
                logic_key,*(qos_cfg_oper_type_string + uc_oper));
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);     
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
NBB_LONG spm_hqos_checkslot(NBB_LONG slot, NBB_LONG *slot_index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;

    for (i = 0; i < MAX_HQOS_SLOT_NUM; i++)
    {
        /*hqos_port_index[i]:֧��hqos�Ĳ�λ��*/
        if (g_hqos_port_index[i] == slot)
        {
            if (NULL != slot_index)
            {
                *slot_index = i;
            }

            /*�쳣����*/
            goto EXIT_LABEL;
        }
    }
    ret = ATG_DCI_RC_UNSUCCESSFUL;

    /*�쳣����*/
    EXIT_LABEL:
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
    NBB_LONG offset = (MAX_HQOS_SLOT_NUM * NUM_COS);

    if(NULL == voq || NULL == vc || NULL == vc_id)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    NBB_TRC_ENTRY(__FUNCTION__);

    for (i = 0; i < MAX_HQOS_SLOT_NUM; i++)
    {
        /*g_hqos_port_index[i]:֧��hqos�Ĳ�λ��*/
        if (g_hqos_port_index[i] == slot)
        {
           slot_index = i;
        }
        if(g_hqos_port_index[i] == fapid)
        {
            fapid_index = i;
        }
        vc_id[i] = (offset * node_index) + (i * NUM_COS) + HQOS_OFFSET;
    }

    if(128 == slot_index || 128 == fapid_index)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    (*voq) = (slot_index * NUM_COS) + (offset * node_index) + HQOS_OFFSET;
    (*vc) = (offset * node_index) + (fapid_index * NUM_COS) + HQOS_OFFSET;
    

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
NBB_LONG spm_hqos_apply_lsp_id(NBB_ULONG *pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pid || 0 != *pid)/*������*/
    {
        goto EXIT_LABEL;
    }



    /*����LSP ID*/
    for (id = HQOS_MIN_LSP_ID; id < HQOS_MAX_LSP_ID; id++)
    {
        if (ATG_DCI_RC_OK == g_hqos_lsp_pool[id]) /*�п���ID*/
        {
            *pid = id + 1;
            g_hqos_lsp_pool[id] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;

            /*�쳣����*/
            goto EXIT_LABEL;
        }
    }
    ret = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"HQOS apply lspid",ATG_DCI_RC_UNSUCCESSFUL,
						"id","","","",
						id,0,0,0));

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
NBB_LONG spm_hqos_apply_pw_id(NBB_ULONG *pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG id = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pid || 0 != *pid)
    {
        goto EXIT_LABEL;
    }



    /*����PW ID*/
    for (id = 0; id < HQOS_MAX_PW_ID; id++)
    {
        if (ATG_DCI_RC_OK == g_hqos_pw_pool[id]) /*�п���ID*/
        {
            *pid = id + 1;
            g_hqos_pw_pool[id] = ATG_DCI_RC_UNSUCCESSFUL;
            ret = ATG_DCI_RC_OK;

            /*�쳣����*/
            goto EXIT_LABEL;
        }
    }
    ret = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"HQOS apply pwid",ATG_DCI_RC_UNSUCCESSFUL,
						"id","","","",
						id,0,0,0));

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
NBB_LONG spm_hqos_release_lsp_id(NBB_ULONG pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(0 == pid)
    {
        goto EXIT_LABEL;
    }

    /*��Դ��ռ�ÿ����ͷ�*/
    if (MAX_PHYSIC_PORT_NUM > pid)
    {

        printf("**QOS ERROR** %s %d lsp id is not in use\n",
                    __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"HQOS release lspid",ATG_DCI_RC_UNSUCCESSFUL,
						"id","","","",
						pid,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }
    if (ATG_DCI_RC_OK != g_hqos_lsp_pool[pid - 1])
    {
        g_hqos_lsp_pool[pid - 1] = ATG_DCI_RC_OK;

        /*�쳣����*/
        goto EXIT_LABEL;
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
NBB_LONG spm_hqos_release_pw_id(NBB_LONG pid)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(0 == pid)
    {
        goto EXIT_LABEL;
    }

    /*��Դ��ռ�ÿ����ͷ�*/
    if (pid >  HQOS_MAX_PW_ID)
    {
        
        printf("**QOS ERROR** %s %d pw id is not in use\n",
                    __FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
						"HQOS release pwid",ATG_DCI_RC_UNSUCCESSFUL,
						"id","","","",
						pid,0,0,0));
        ret = ATG_DCI_RC_UNSUCCESSFUL;

        /*�쳣����*/
        goto EXIT_LABEL;
    }
    if (ATG_DCI_RC_OK != g_hqos_pw_pool[pid - 1])
    {
        g_hqos_pw_pool[pid - 1] = ATG_DCI_RC_OK;

        /*�쳣����*/
        goto EXIT_LABEL;
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
NBB_LONG spm_hqos_apply_lsp_pw_id(NBB_LONG *lsp_id,NBB_LONG *pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*����LSP ID*/
    ret = spm_hqos_apply_lsp_id(lsp_id);
    if(ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }    


    ret = spm_hqos_apply_pw_id(pw_id);
    if(ATG_DCI_RC_OK != ret && NULL != lsp_id)
    {
        spm_hqos_release_lsp_id(*lsp_id);
        goto EXIT_LABEL;
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
NBB_LONG spm_hqos_release_lsp_pw_id(NBB_LONG *lsp_id,NBB_LONG *pw_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;

    NBB_TRC_ENTRY(__FUNCTION__);

    /*��Դ��ռ�ÿ����ͷ�*/
    if(NULL != lsp_id)
    {
        ret = spm_hqos_release_lsp_id(*lsp_id);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
        *lsp_id = 0;
    }
    
    if(NULL != pw_id)
    {
        ret = spm_hqos_release_pw_id(*pw_id);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
        *pw_id = 0;
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
    NBB_CHAR message[HQOS_MSG_INFO_LEN];
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
    NBB_CHAR message[HQOS_MSG_INFO_LEN];
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
        printf("traffic_schedule:mod=%d,phy_queue_id=%d\n",
                  data->mod,data->phy_queue_id);
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
    NBB_CHAR message[HQOS_MSG_INFO_LEN];
    if(NULL != data)
    {
        OS_SPRINTF(message,"traffic_schedule:mod=%d,phy_queue_id=%d\n",
                  data->mod,data->phy_queue_id);
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
NBB_VOID spm_hqos_arad_voq_error_log(NBB_LONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_LONG base_vcid,
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
static NBB_VOID spm_hqos_arad_traffic_set_error_log(NBB_LONG voq,
    NBB_USHORT slot,NBB_ULONG port,NBB_LONG base_vcid,NBB_LONG pw_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosTrafficSet err: %ld (base_queue=%ld,slot=%d,"
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
NBB_VOID spm_hqos_arad_set_pw_bandwidth_error_log(NBB_LONG pw_id, 
    NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs, const NBB_CHAR *function, 
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosPwBandwidthSet err: %ld "
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
NBB_VOID spm_hqos_arad_create_pw_error_log(NBB_ULONG port,NBB_LONG lsp_id,
    NBB_LONG pw_id,const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
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
NBB_VOID spm_hqos_arad_voq_del_error_log(NBB_LONG voq,
    NBB_USHORT slot, NBB_ULONG port, NBB_LONG base_vcid,const NBB_CHAR *function, 
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosVoqDelete err: %ld (base_queue=%ld,slot=%d,port=%ld,vc=%ld)\n\n",
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
NBB_VOID spm_hqos_arad_traffic_del_error_log(NBB_LONG voq,
    NBB_USHORT slot,NBB_ULONG port,NBB_LONG base_vcid,NBB_LONG pw_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosTrafficDelete err: %ld (base_queue=%ld,slot=%d,port=%ld,"
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
NBB_VOID spm_hqos_arad_del_pw_error_log(NBB_LONG lsp_id,NBB_LONG pw_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosPwDelete err: %ld (lsp_id=%ld,pw_id=%ld)\n\n",
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
NBB_VOID spm_hqos_arad_set_pw_weight_error_log(NBB_LONG pw_id, NBB_ULONG weight,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosPwWfqWeightSet err: %ld "
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
NBB_VOID spm_hqos_arad_set_traffic_bandwidth_error_log(NBB_LONG pw_id, 
    NBB_LONG queue_id, NBB_ULONG cir, NBB_ULONG pir, NBB_ULONG cbs, NBB_ULONG pbs, 
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosTrafficBandwidthSet err: %ld "
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
    NBB_LONG pw_id, NBB_ULONG que4_1, NBB_ULONG que8_5, const NBB_CHAR *function,
    NBB_ULONG line, NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosTrafficScheModeSet err: %ld "
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
        printf("%s %s,line=%ld ret=%ld fhdrv_fap_qos_refresh_pw_to_lsp\n"
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
static NBB_VOID spm_hqos_arad_create_lsp_error_log(NBB_ULONG port,NBB_LONG lsp_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s %s,line=%ld ApiAradHqosLspCreate port=%ld,lsp id=%ld,ret=%ld\n",
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
NBB_VOID spm_hqos_arad_del_lsp_error_log(NBB_ULONG port,NBB_LONG lsp_id,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s %s,line=%ld ApiAradHqosLspDelete port=%ld,lsp id=%ld,ret=%ld\n",
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
    NBB_LONG lsp_id,NBB_ULONG cir,NBB_ULONG pir,NBB_ULONG cbs,NBB_ULONG pbs,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s %s,line=%ld ApiAradHqosLspBandwidthSet lsp id=%ld,cir=%ld,"
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
NBB_VOID spm_hqos_arad_set_lsp_weight_error_log(NBB_LONG lsp_id,NBB_ULONG weight,
    const NBB_CHAR *function,NBB_ULONG line,NBB_LONG ret)
{

    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ApiAradHqosLspWfqWeightSet err: %ld "
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
NBB_VOID  spm_hqos_l3txpw_error_log(NBB_ULONG voq, 
    NBB_ULONG hqos_en, NBB_ULONG posid, const NBB_CHAR *FUNCTION,
    NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR uc_message[QOS_MSG_INFO_LEN];

    if(NULL != FUNCTION)
    {
        printf("%s line=%ld  ret=%ld ApiC3SetL3TxPwHqos voq=%ld posid=%ld,hqos_en=%s\n\n",
                FUNCTION,line,ret,voq, posid,hqos_en?"ʹ��":"��ʹ��");
        OS_SPRINTF(uc_message,"%s line=%ld  ret=%ld ApiC3SetL3TxPwHqos voq=%ld posid=%ld,hqos_en=%s\n\n",
                FUNCTION,line,ret,voq, posid,hqos_en?"ʹ��":"��ʹ��");
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);   
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
    			HQOS_C3_ERROR,ret,
    			uc_message,FUNCTION,"line","",
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
NBB_VOID  spm_hqos_txlsp_error_log(NBB_LONG unit,NBB_ULONG posid,NBB_ULONG hqos_en,
    NBB_LONG voq,  const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ret=%ld fhdrv_psn_common_set_txlsp_hqos\n"
                  "unit=%ld,posid=%ld,hqos_en=%s,voq=%ld\n\n",
                function,line,ret,unit,posid,
                hqos_en?"ʹ��":"��ʹ��",voq);
        OS_SPRINTF(message,"%s line=%ld  ret=%ld fhdrv_psn_common_set_txlsp_hqos\n"
                  "unit=%ld,posid=%ld,hqos_en=%s,voq=%ld\n\n",
                function,line,ret,unit,posid,
                hqos_en?"ʹ��":"��ʹ��",voq);
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
    NBB_LONG voq, NBB_ULONG hqos_en, NBB_ULONG posid, NBB_LONG proflag,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ret=%ld ApiC3SetVpHqos voq=%ld "
                  "posid=%ld,hqos_en=%s,proflag=%s\n\n",
                function,line,ret,voq, posid,
                hqos_en?"ʹ��":"��ʹ��",(VC_BACK == proflag)?"����":"����");
        OS_SPRINTF(message,"%s line=%ld  ret=%ld ApiC3SetVpHqos voq=%ld"
                   " posid=%ld,hqos_en=%s proflag=%s\n\n",
                function,line,ret,voq, posid,
                hqos_en?"ʹ��":"��ʹ��",(VC_BACK == proflag)?"����":"����");
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
NBB_VOID  spm_hqos_l3uni_error_log(NBB_LONG unit, NBB_ULONG l3_txuni_posId, TX_PORT_T *pt_txport,
    NBB_LONG hqos_en, NBB_ULONG hqos_flowid,
    const NBB_CHAR *function, NBB_ULONG line, NBB_LONG ret)
{
    NBB_CHAR message[QOS_MSG_INFO_LEN];

    if(NULL != function)
    {
        printf("%s line=%ld  ret=%ld ApiC3SetL3UniHqos unit=%ld \n"
                "l3_txuni_posId=%ld,hqos_en=%s,hqos_flowid=%ld\n\n",
                function,line,ret,unit,l3_txuni_posId,hqos_en?"ʹ��":"��ʹ��",hqos_flowid);
        OS_SPRINTF(message,"%s line=%ld  ret=%ld ApiC3SetL3UniHqos unit=%ld \n"
                "l3_txuni_posId=%ld,hqos_en=%s,hqos_flowid=%ld\n\n",
                function,line,ret,unit,l3_txuni_posId,hqos_en?"ʹ��":"��ʹ��",hqos_flowid);
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

    /*��Ĭ�ϵ�LSP�ڵ�Ŵ���LSP*/
    if(0 != lsp_id && lsp_id > MAX_PHYSIC_PORT)
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

    /*ɾ����Ĭ�Ͻڵ�LSP*/
    if(0 != lsp_id && lsp_id > MAX_PHYSIC_PORT)
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
            spm_hqos_arad_del_pw_error_log(lsp_id,pw_id,__FUNCTION__,__LINE__,ret);
            rv = ret;
        }
#endif        
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
static NBB_LONG spm_hqos_create_voq_vc(NBB_BYTE slot, NBB_LONG port,
    NBB_LONG pw_id,
    NBB_LONG voq_id,NBB_LONG tm_vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],
    NBB_LONG *ulp_voq,NBB_LONG *p_vc,NBB_LONG p_vc_id[MAX_HQOS_SLOT_NUM])
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
    if(NULL != ulp_voq && NULL != p_vc)
    {
        *ulp_voq = voq_id;
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
     NBB_LONG base_queue,ATG_DCI_TX_LSP_QOS *data)
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
              data->normal_shaping.pir, data->normal_shaping.cbs, data->normal_shaping.pbs);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_pw_bandwidth_error_log(pw_id, data->normal_shaping.cir,
              data->normal_shaping.pir, data->normal_shaping.cbs, data->normal_shaping.pbs,
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
                  data->proi_queue_shaping[i].pir, data->proi_queue_shaping[i].cbs,
                  data->proi_queue_shaping[i].pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_set_traffic_bandwidth_error_log(pw_id,i,
                    data->proi_queue_shaping[i].cir,data->proi_queue_shaping[i].pir,
                    data->proi_queue_shaping[i].cbs,data->proi_queue_shaping[i].pbs,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif  

#if defined (SPU) || defined (SRC)

            /*ret = ApiAradHqosTrafficScheModeSet(unit, pw_id, data->proi_queue_strategy[i].cir,
                  data->proi_queue_strategy[i].pir, data->proi_queue_strategy[i].cbs,
                  data->proi_queue_strategy[i].pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                
                goto EXIT_LABEL;
            }*/
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
     NBB_LONG base_queue,ATG_DCI_TX_LSP_QOS *data)
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
              data->normal_shaping.pir, data->normal_shaping.cbs, data->normal_shaping.pbs);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_arad_set_lsp_bandwidth_error_log(lsp_id, data->normal_shaping.cir,
              data->normal_shaping.pir, data->normal_shaping.cbs, data->normal_shaping.pbs,
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
                  data->proi_queue_shaping[i].pir, data->proi_queue_shaping[i].cbs,
                  data->proi_queue_shaping[i].pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_set_traffic_bandwidth_error_log(pw_id,i,
                    data->proi_queue_shaping[i].cir, data->proi_queue_shaping[i].pir, data->proi_queue_shaping[i].cbs,
                  data->proi_queue_shaping[i].pbs,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif  

#if defined (SPU) || defined (SRC)

            /*ret = ApiAradHqosTrafficScheModeSet(unit,port, pw_id, data->proi_queue_strategy[i].cir,
                  data->proi_queue_strategy[i].pir, data->proi_queue_strategy[i].cbs,
                  data->proi_queue_strategy[i].pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                
                goto EXIT_LABEL;
            }*/
#endif 
        }
     
    }

    /*����̺ͳ����̶���ִ��*/
    /*�������C3��flow id hqos��ҵ��ͨ����*/
    for(unit = 0; unit < c3_num;unit++)
    {
        if(0 != posid)
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
    }

    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}


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
    NBB_LONG ret = ATG_DCI_RC_OK;
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
                spm_hqos_release_lsp_pw_id(&lsp_id,&pw_id);
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
    AVLL_INIT_TREE(pst_tbl->vrf_tree, spm_qos_vrf_hqos_comp,
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
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_LSP_CB), MEM_SPM_HQOS_LSP_TX_CB);

    /*˵���Ѿ��ǳ����̲��Ҳ���Ĭ�ϵ�LSP�ڵ�*/
    if((pst_tbl->lsp_id != pst_tbl->port) && (0 != pst_tbl->lsp_id))
    {
        ret = spm_hqos_del_lsp_pw(pst_tbl->lsp_id, pst_tbl->port,pst_tbl->pw_id);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }

        ret = spm_hqos_release_lsp_pw_id(&(pst_tbl->lsp_id),&(pst_tbl->pw_id));
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

#if 5




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
void qos_print_vc_hqos_cfg(ATG_DCI_TX_LSP_QOS *data)
{
    NBB_BYTE i = 0;
    if(NULL != data)
    {
       if(ATG_DCI_RC_OK != g_qos_hqos_print)
        {
            printf("node_index = %ld:\n",data->node_index);
            qos_print_traffic_shaping_cfg(&(data->normal_shaping));
            qos_print_traffic_schedule_cfg(&(data->normal_strategy));
            for(i = 0;i < 8;i++)
            {
               printf("queue id = %d:\n",i);
               qos_print_traffic_shaping_cfg(&(data->proi_queue_shaping[i])); 
               qos_print_traffic_schedule_cfg(&(data->proi_queue_strategy[i]));
               qos_print_traffic_congestion_cfg(&(data->proi_queue_congestion_strategy[i]));
            }
            printf("\n\n\n");
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
void qos_log_vc_hqos_cfg(ATG_DCI_TX_LSP_QOS *data)
{
    NBB_BYTE i = 0;
    if(NULL != data)
    {
       if(ATG_DCI_RC_OK != g_qos_hqos_log)
        {
            qos_log_traffic_shaping_cfg(&(data->normal_shaping));
            qos_log_traffic_schedule_cfg(&(data->normal_strategy));
            for(i = 0;i < 8;i++)
            {
               qos_log_traffic_shaping_cfg(&(data->proi_queue_shaping[i])); 
               qos_log_traffic_schedule_cfg(&(data->proi_queue_strategy[i]));
               qos_log_traffic_congestion_cfg(&(data->proi_queue_congestion_strategy[i]));
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
    SPM_QOS_TUNNEL_KEY *lsp_key,ATG_DCI_VC_KEY *vc_key,ATG_DCI_TX_LSP_QOS *data)
{
    NBB_CHAR uc_message[HQOS_MSG_INFO_LEN];
    
    qos_print_vc_hqos_cfg(data);
    
    if(ATG_DCI_RC_OK != g_qos_hqos_log && NULL != data)
    {
        OS_SPRINTF(uc_message,"slot=%d,port=%d,proflag=%d,node_index = %ld\n",
                  slot,port,proflag,data->node_index);
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
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG id = 0;
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
        pst_tbl->pw_id = 0;
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
                if(VC_MAIN == vc->proflag)
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
            ret = ApiAradHqosVoqDelete(0, vc->voq_id, slot, port, vc->tm_vc);
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
NBB_LONG spm_hqos_del_vc_resource(SPM_HQOS_LSP_CB **plsp,SPM_HQOS_VC_CB **pcb)
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
    if(NULL == plsp || NULL == *plsp)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    if(NULL != pcb)
    {
        vc = *pcb;
    }
    lsp = *plsp;
    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;

    

    /***************************************************************************/
    /* �������VC�ϵ�����                                                    */
    /***************************************************************************/
    if(NULL != vc)
    {
        //spm_dbg_record_hqos_vc_head(&(vc->vc_key),SPM_OPER_DEL);�ڲ�ɾ���ӿ�
        
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
        if(AVLL_IN_TREE(vc->spm_hqos_vc_node))
        {
            AVLL_DELETE(lsp->vc_tree, vc->spm_hqos_vc_node);
        }

        /*�ͷ��ڴ���Դ*/
        ret = spm_free_hqos_vc_cb(vc,lsp_id);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }
        
        //*pcb = NULL;/*�ⲿ��������ָ�벻����Ϊ��*/ 
         
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
    ATG_DCI_TX_LSP_QOS *data,SPM_HQOS_VC_CB *pcb)
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
        OS_MEMCPY(&(pcb->vc_data), data, sizeof(ATG_DCI_TX_LSP_QOS)); 
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
        ATG_DCI_TX_LSP_QOS *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_VC_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data) || (NULL == vc_id))
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
    ret = spm_hqos_set_vc_pw_property(slot,pw_id,posid,voq,proflag,data,pcb);
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
        ATG_DCI_VC_KEY *vc_key,ATG_DCI_TX_LSP_QOS *data,
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
        spm_dbg_record_hqos_vc_head(vc_key,SPM_OPER_INIT_ADD);

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
            spm_hqos_del_vc_resource(&plsp,NULL);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ret = spm_hqos_set_vc_drive(slot,port,pcb->pw_id,posid,proflag,data,
            voq,vc,vc_id,pcb);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_del_vc_resource(&plsp,&pcb);
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
            spm_dbg_record_hqos_vc_head(vc_key,SPM_OPER_ADD);
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
                spm_hqos_del_vc_resource(&plsp,&pcb);
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
            spm_dbg_record_hqos_vc_head(vc_key,SPM_OPER_UPD);
            
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
    ATG_DCI_VC_UP_VPN_QOS_POLICY *data)
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
    NBB_LONG vc_id[MAX_HQOS_SLOT_NUM] = {0,0,0,0};
    NBB_CHAR message[HQOS_MSG_INFO_LEN];


    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (port > 0x80))/*��֧��LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }
    
    qos_log_vc_hqos_data(slot,port,posid,proflag,lsp_key,vc_key,(ATG_DCI_TX_LSP_QOS*)data);
    
        
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
    
    ret = spm_hqos_add_vc( slot,  port,  posid,proflag, lsp_key, vc_key, (ATG_DCI_TX_LSP_QOS*)data,
                    voq, vc, vc_id);

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
NBB_LONG spm_hqos_del_vc_node(NBB_LONG slot, NBB_USHORT port,
        SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VC_KEY *vc_key)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VC_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    CRTXLSP_KEY lsptx_key = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_ULONG fapid = v_spm_shared->local_slot;
    NBB_LONG slot_index = 0;
    NBB_LONG fapid_index = 0;
    NBB_CHAR message[HQOS_MSG_INFO_LEN];



    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vc_key) || (NULL == lsp_key) || (port > 0x80))/*��֧��LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    spm_dbg_record_hqos_vc_head(vc_key,SPM_OPER_DEL);
    
        
    /* �ж�slot�Ƿ���hqos�Ŀ��� */
    ret = spm_hqos_checkslot(slot,&slot_index);
    if (ATG_DCI_RC_OK != ret)/*û�п���HQOS��λʹ��ֱ������������*/
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    ret = spm_hqos_checkslot(fapid,&fapid_index);
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

    /*�����Ƿ����B�ڵ�*/
    plsp = AVLL_FIND(g_qos_port_cb[slot][port].lsp_tree, lsp_key);
    if(NULL == plsp)
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(plsp->vc_tree, vc_key);
    if(NULL == pcb)
    {
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    ret = spm_hqos_del_vc_resource(&plsp, &pcb);


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
    SPM_QOS_TUNNEL_KEY *lsp_key,ATG_DCI_VRF_INSTANCE_KEY *vrf_key,ATG_DCI_TX_LSP_QOS *data)
{
    NBB_CHAR uc_message[HQOS_MSG_INFO_LEN];
    
    qos_print_vc_hqos_cfg(data);
    
    if(ATG_DCI_RC_OK != g_qos_hqos_log)
    {
        OS_SPRINTF(uc_message,"slot=%d,port=%d,node_index = %ld \n",
                  slot,port,data->node_index);
        BMU_SLOG(BMU_INFO, SPM_QOS_LOG_DIR, uc_message);
        qos_log_lsp_key(lsp_key);
        qos_log_vrf_key(vrf_key);
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
static SPM_HQOS_VRF_CB *spm_alloc_hqos_vrf_cb(ATG_DCI_VRF_INSTANCE_KEY *vrf_key,
    NBB_LONG lsp_id,NBB_BYTE slot,NBB_USHORT port)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VRF_CB *pst_tbl = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG id = 0;
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
NBB_LONG spm_free_hqos_vrf_cb(SPM_HQOS_VRF_CB *pst_tbl,NBB_LONG lsp_id)
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
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_VRF_CB), MEM_SPM_HQOS_VRF_CB);
    
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
        pst_tbl->pw_id = 0;
    }
    

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_HQOS_VRF_CB);
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
            ret = ApiAradHqosVoqDelete(0, vrf->voq_id, slot, port, vrf->tm_vc);
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
NBB_LONG spm_hqos_del_vrf_resource(SPM_HQOS_LSP_CB **plsp,SPM_HQOS_VRF_CB **pcb)
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
    if(NULL == plsp || NULL == *plsp)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    if(NULL != pcb)
    {
        vrf = *pcb;
    }
    lsp = *plsp;
    slot = lsp->slot;
    port = lsp->port;
    lsp_id = lsp->lsp_id;

    

    /***************************************************************************/
    /* �������VC�ϵ�����                                                    */
    /***************************************************************************/
    if(NULL != vrf)
    {
        //spm_dbg_record_hqos_vrf_head(&(vrf->vrf_key),SPM_OPER_DEL);
        
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
        if(AVLL_IN_TREE(vrf->spm_hqos_vrf_node))
        {
            AVLL_DELETE(lsp->vrf_tree, vrf->spm_hqos_vrf_node);
        }

        /*�ͷ��ڴ���Դ*/
        ret = spm_free_hqos_vrf_cb(vrf,lsp_id);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }
        
        //*pcb = NULL;/*ָ�����ΪNULL*/ 
         
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
    NBB_ULONG posid,ATG_DCI_TX_LSP_QOS *data,SPM_HQOS_VRF_CB *pcb)
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
        OS_MEMCPY(&(pcb->vrf_data), data, sizeof(ATG_DCI_TX_LSP_QOS)); 
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
        ATG_DCI_TX_LSP_QOS *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_VRF_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data) || (NULL == vc_id))
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
    ret = spm_hqos_set_vrf_pw_property(slot,voq,pw_id,posid,data,pcb);
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
        ATG_DCI_VRF_INSTANCE_KEY *vrf_key,ATG_DCI_TX_LSP_QOS *data,
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
        spm_dbg_record_hqos_vrf_head(vrf_key,SPM_OPER_INIT_ADD);
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
            spm_hqos_del_vrf_resource(&plsp,&pcb);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ret = spm_hqos_set_vrf_drive(slot,port,pcb->pw_id,posid,data,voq,vc,vc_id,pcb);
        if(ATG_DCI_RC_OK != ret)
        {
            spm_hqos_del_vrf_resource(&plsp,&pcb);
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
            spm_dbg_record_hqos_vrf_head(vrf_key,SPM_OPER_ADD);
            pcb = spm_alloc_hqos_vrf_cb(vrf_key,plsp->lsp_id,slot,port);
            if (NULL == pcb)
            {
                ret = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

            ret = spm_hqos_set_vrf_drive(slot,port,pcb->pw_id,posid,data,voq,vc,vc_id,pcb);
            if(ATG_DCI_RC_OK != ret)
            {
                spm_hqos_del_vrf_resource(&plsp,&pcb);
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
            spm_dbg_record_hqos_vrf_head(vrf_key,SPM_OPER_UPD);
            
            ret = spm_hqos_set_vrf_pw_property(slot,voq,pcb->pw_id,posid,data,pcb);

            if(ATG_DCI_RC_OK != ret)
            {
                //spm_hqos_del_vrf_resource(&plsp,&pcb);
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
NBB_LONG spm_hqos_add_vrf_node(NBB_BYTE slot, NBB_USHORT port, NBB_ULONG posid,
    SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VRF_INSTANCE_KEY *vrf_key,
    ATG_DCI_VRF_INSTANCE_UP_VPN_QOS *data)
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
    NBB_CHAR message[QOS_MSG_INFO_LEN] = {0};
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vrf_key) || (NULL == lsp_key) || (NULL == data) 
        || (0 == posid) || (port > 0x80))/*��֧��LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }
    
    qos_log_vrf_hqos_data(slot,port,posid,lsp_key,vrf_key,data);
    
        
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
    
    ret = spm_hqos_add_vrf( slot,  port,  posid, lsp_key, vrf_key, (ATG_DCI_TX_LSP_QOS*)data,
                    voq, vc, vc_id);

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
NBB_LONG spm_hqos_del_vrf_node(NBB_LONG slot, NBB_USHORT port, NBB_ULONG posid,
        SPM_QOS_TUNNEL_KEY *lsp_key, ATG_DCI_VRF_INSTANCE_KEY *vrf_key)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_VRF_CB *pcb = NULL;
    SPM_HQOS_LSP_CB *plsp = NULL;
    CRTXLSP_KEY lsptx_key = {0};
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_ULONG fapid = v_spm_shared->local_slot;
    NBB_LONG slot_index = 0;
    NBB_LONG fapid_index = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    NBB_LONG unit = 0;
    NBB_CHAR message[HQOS_MSG_INFO_LEN];



    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == vrf_key) || (NULL == lsp_key) || (port > 0x80))/*��֧��LAG*/
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    spm_dbg_record_hqos_vrf_head(vrf_key,SPM_OPER_DEL);
    
        
    /* �ж�slot�Ƿ���hqos�Ŀ��� */
    ret = spm_hqos_checkslot(slot,&slot_index);
    if (ATG_DCI_RC_OK != ret)/*û�п���HQOS��λʹ��ֱ������������*/
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }
    ret = spm_hqos_checkslot(fapid,&fapid_index);
    if (ATG_DCI_RC_OK != ret)/*û�п���HQOS��λʹ��ֱ������������*/
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }

    if(0 != posid)/*hqos�Ѿ�ɾ����ȥʹ��flow id*/
    {
        for(unit = 0; unit < c3_num;unit++)
        {
#if defined (SPU) || defined (PTN690_CES)
            ret = ApiC3SetL3TxPwHqos(unit, posid, 0, 0);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_l3txpw_error_log(0,0,posid,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
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

    /*�����Ƿ����B�ڵ�*/
    plsp = AVLL_FIND(g_qos_port_cb[slot][port].lsp_tree, lsp_key);
    if(NULL == plsp)
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(plsp->vrf_tree, vrf_key);
    if(NULL == pcb)
    {
        ret = ATG_DCI_RC_OK;
        goto EXIT_LABEL;
    }

    ret = spm_hqos_del_vrf_resource(&plsp, &pcb);


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
}

#endif


#if 4




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
NBB_LONG spm_hqos_refresh_lsp_port(NBB_LONG port,NBB_LONG lsp_id,SPM_HQOS_LSP_CB *lsp_cb)
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
NBB_LONG spm_hqos_set_lsp_drive(NBB_BYTE slot,NBB_USHORT port,
        NBB_LONG lsp_id,NBB_LONG pw_id,NBB_ULONG posid,
        ATG_DCI_TX_LSP_QOS *data,NBB_LONG voq,
        NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_LSP_CB *plsp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data) || (NULL == vc_id))
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
        OS_MEMCPY(&(plsp->data), data, sizeof(ATG_DCI_TX_LSP_QOS));
    }


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
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
NBB_LONG spm_hqos_del_lsp_node(SPM_HQOS_LSP_CB **plsp)
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

    spm_dbg_record_hqos_lsp_head(&(lsp->lsp_tx_key),SPM_OPER_DEL);

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
        ret = ApiAradHqosVoqDelete(0, lsp->voq_id, slot, port, lsp->tm_vc);
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


    
    if(AVLL_IN_TREE(lsp->spm_hqos_lsp_tx_node))/*ɾ�����ڵ�����*/
    {
        /*ɾ����Ĭ�ϵ�LSP�ڵ�:����ʧ�ܻ�����Դ���߸��µ�Ĭ�ϵ�LSP ID*/
        if(0 != lsp_id && lsp_id != port)
        {
            /*����LSP ID ��Ĭ�ϵĽڵ���*/
            ret = spm_hqos_refresh_lsp_port(port,port,lsp);
            if (ATG_DCI_RC_OK != ret)
            {
                rv = ret;
            }
        }
        
        
        if ((NULL == AVLL_FIRST(lsp->vc_tree)) && (NULL == AVLL_FIRST(lsp->vrf_tree)))
        {
            AVLL_DELETE(g_qos_port_cb[slot][port].lsp_tree,lsp->spm_hqos_lsp_tx_node);    
            ret = spm_free_hqos_lsp_cb(lsp);
            if (ATG_DCI_RC_OK != ret) 
            {
                rv = ret;
            } 
            
            //*plsp = NULL;�ⲿָ�벻����Ϊ��
        }
        
    }
    else/*ɾ����Ϊ�쳣������Դ����Ĭ�Ϻͷ�Ĭ��LSP�ڵ�*/
    {
        ret = spm_free_hqos_lsp_cb(lsp);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        } 
        
        //*plsp = NULL;�ⲿָ�벻����Ϊ��
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
        ret = ApiAradHqosVoqDelete(0, lsp->voq_id, slot, port, lsp->tm_vc);
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
    ret = spm_hqos_release_lsp_pw_id(&(lsp->lsp_id),&(lsp->pw_id));
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
            lsp_cb->pw_id,lsp_cb->posid,&(lsp_cb->data),lsp_cb->voq_id,
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
                vc_cb->posid,vc_cb->proflag,&(vc_cb->vc_data),vc_cb->voq_id,
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
         ret = spm_hqos_del_vrf_drive(lsp_cb->slot, lsp_cb->port, vrf_cb);
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
NBB_LONG spm_hqos_add_lsp_node(NBB_USHORT slot,
    NBB_LONG port,
    SPM_QOS_TUNNEL_KEY *lsp_key,
    ATG_DCI_TX_LSP_QOS *data,
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
        spm_dbg_record_hqos_lsp_head(lsp_key,SPM_OPER_ADD);
        plsp = spm_alloc_hqos_lsp_cb(lsp_key,ATG_DCI_RC_UNSUCCESSFUL,slot,port);/*����LSP ID*/

        /*�����ڴ�ʧ��*/
        if (NULL == plsp)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        ret = spm_hqos_set_lsp_drive(slot,port,lsp_id,pw_id,posid,data,voq,
            vc,vc_id, plsp);
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
            spm_dbg_record_hqos_lsp_head(lsp_key,SPM_OPER_INIT_ADD);
            
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
            spm_dbg_record_hqos_lsp_head(lsp_key,SPM_OPER_INIT_ADD);
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
                spm_dbg_record_hqos_lsp_head(lsp_key,SPM_OPER_FRESH);
                ret = spm_hqos_apply_lsp_pw_id(&lsp_id,&pw_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }

                ret = spm_hqos_create_lsp_pw(lsp_id,port,pw_id);
                if(ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_release_lsp_pw_id(&lsp_id,&pw_id);
                    goto EXIT_LABEL;
                }
                plsp->lsp_id = lsp_id;
                plsp->pw_id = pw_id;
                
                ret = spm_hqos_set_lsp_drive(slot,port,lsp_id,pw_id,posid,data,voq,
                    vc,vc_id, plsp);
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
                spm_dbg_record_hqos_lsp_head(lsp_key,SPM_OPER_UPD);
                ret = spm_hqos_set_lsp_pw_property(slot,port,plsp->lsp_id,plsp->pw_id,posid,voq,data);
                if (ATG_DCI_RC_OK != ret)
                {
                    goto EXIT_LABEL;
                }
                plsp->posid = posid;
                plsp->np_flow_id = voq;
                OS_MEMCPY(&(plsp->data), data, sizeof(ATG_DCI_TX_LSP_QOS));
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
SPM_HQOS_USER_GROUP_CB *spm_alloc_hqos_usr_group_cb(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_USER_GROUP_CB *pst_tbl = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY(__FUNCTION__);


    /* ����һ���µ�log group���ÿ�qos��������Ŀ��*/
    pst_tbl = (SPM_HQOS_USER_GROUP_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_USER_GROUP_CB),
        NBB_NORETRY_ACT, MEM_SPM_USER_GROUP_CB);

    /*�����ڴ�ʧ��*/
    if (pst_tbl == NULL)
    {
        /*�쳣����*/
        goto EXIT_LABEL;
    }

    /* ��ʼlog group���ÿ�������Ŀ */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_HQOS_USER_GROUP_CB));
    pst_tbl->user_group_key = index;



    /*QOS USR ����ʼ��*/
    avll_key_offset = NBB_OFFSETOF(SPM_HQOS_LOG_USR_CB, index);
    AVLL_INIT_TREE(pst_tbl->usr_tree, compare_ulong,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_LOG_USR_CB, spm_hqos_usr_node));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_tbl->spm_qos_user_group_node);


    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return(pst_tbl);
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
NBB_LONG spm_free_hqos_user_group_cb(SPM_HQOS_USER_GROUP_CB *pst_tbl)
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
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_USER_GROUP_CB), MEM_SPM_USER_GROUP_CB);

    if(0 != pst_tbl->lsp_id)
    {
        ret = spm_hqos_del_lsp_pw(pst_tbl->lsp_id, pst_tbl->port,0);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }

        ret = spm_hqos_release_lsp_pw_id(&(pst_tbl->lsp_id),NULL);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }


    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_USER_GROUP_CB);
    pst_tbl = NULL;

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
NBB_LONG spm_release_hqos_group_drive(SPM_HQOS_USER_GROUP_CB *pst_tbl)
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


    if(0 != pst_tbl->lsp_id)
    {
        ret = spm_hqos_del_lsp_pw(pst_tbl->lsp_id, pst_tbl->port,0);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }

        ret = spm_hqos_release_lsp_pw_id(&(pst_tbl->lsp_id),NULL);
        if(ATG_DCI_RC_OK != ret)
        {
            rv = ret;
        }
    }
    pst_tbl->slot = 0;
    pst_tbl->port = 0;


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
NBB_LONG spm_set_down_user_group_cfg(NBB_ULONG oper, SPM_HQOS_USER_GROUP_CB *pcb,
    NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_TX_LSP_QOS *data = NULL;
   

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
        OS_MEMSET(&(pcb->data), 0XFF, sizeof(ATG_DCI_TX_LSP_QOS));
        break;

        /* ���� */
        case  ATG_DCI_OPER_ADD:

        /* ips��Ϣ����Ϊ�� */
        if (NULL != buf)
        {
            data = (ATG_DCI_TX_LSP_QOS *)buf;

            if(0 != pcb->lsp_id)/*�Ѿ���ʵ����*/
            {
#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosLspBandwidthSet(0, pcb->lsp_id, data->normal_shaping.cir,
                      data->normal_shaping.pir, data->normal_shaping.cbs, 
                      data->normal_shaping.pbs);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_arad_set_lsp_bandwidth_error_log(pcb->lsp_id, data->normal_shaping.cir,
                      data->normal_shaping.pir, data->normal_shaping.cbs, 
                      data->normal_shaping.pbs,
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
            OS_MEMCPY(&(pcb->data), data, sizeof(ATG_DCI_TX_LSP_QOS));
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
    SPM_HQOS_USER_GROUP_CB *pst_cb = NULL;
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

    pst_cb = AVLL_FIND(g_qos_user_group_tree, &ulkey);
    if (TRUE == pst_set_ips->delete_struct)
    {
        if(NULL != pst_cb)/*���ô��ڲ�ɾ��*/
        {
            if(NULL != AVLL_FIRST(pst_cb->usr_tree))   
            {
                pst_set_ips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
                
            }
            else/*��Ϊ�������ɾ��group�ڵ�*/
            {
               AVLL_DELETE(g_qos_user_group_tree, pst_cb->spm_qos_user_group_node);
               spm_free_hqos_user_group_cb(pst_cb);
            }
        }   
    }
    else
    {
        if(NULL == pst_cb)/*����*/
        {
            pst_cb = spm_alloc_hqos_usr_group_cb(ulkey);
            if (NULL == pst_cb)
            {
                pst_set_ips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto EXIT_LABEL;
            }

            ret = spm_set_down_user_group_cfg(oper_down, pst_cb, down_data_start);
            if(ATG_DCI_RC_OK != ret)
            {
                pst_set_ips->return_code = pst_set_ips->down_qos_return_code = ret;
                spm_free_hqos_user_group_cb(pst_cb);
            }   
            rv = AVLL_INSERT(g_qos_user_group_tree, pst_cb->spm_qos_user_group_node);
        }
        else/*����*/
        {
            ret = spm_set_down_user_group_cfg(oper_down, pst_cb, down_data_start);
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
    SPM_HQOS_LOG_USR_CB *pst_tbl = NULL;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_ULONG pw_id = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == lsp_id)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }

    /*����������ID��Դ����A,B�ڵ�*/
    if(slot == v_spm_shared->local_slot_id)
    {
        ret = spm_hqos_apply_lsp_pw_id(lsp_id,&pw_id);
        if(ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        } 
   
         ret = spm_hqos_create_lsp_pw(*lsp_id,port,pw_id);
         if(ATG_DCI_RC_OK != ret)
         {
             spm_hqos_release_lsp_pw_id(lsp_id,&pw_id);
             goto EXIT_LABEL;
         }       
    }


    /* ����һ���µ��߼����ÿ�qos��������Ŀ��*/
    pst_tbl = (SPM_HQOS_LOG_USR_CB *)NBB_MM_ALLOC(sizeof(SPM_HQOS_LOG_USR_CB),
        NBB_NORETRY_ACT, MEM_SPM_HQOS_LOG_USR_CB);

    /*�����ڴ�ʧ��*/
    if (pst_tbl == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ�߼����ÿ�������Ŀ */
    OS_MEMSET(pst_tbl, 0, sizeof(SPM_HQOS_LOG_USR_CB));
    pst_tbl->index = index;
    pst_tbl->pw_id = pw_id;
    AVLL_INIT_NODE(pst_tbl->spm_hqos_usr_node);

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
NBB_LONG spm_free_hqos_log_usr_cb(SPM_HQOS_LOG_USR_CB *pst_tbl,NBB_LONG lsp_id)
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
    NBB_ASSERT_MEMORY(pst_tbl, sizeof(SPM_HQOS_LOG_USR_CB), MEM_SPM_HQOS_LOG_USR_CB);
    
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
        pst_tbl->pw_id = 0;
    }
    

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_tbl, MEM_SPM_HQOS_LOG_USR_CB);
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
static NBB_LONG spm_hqos_del_usr_drive(NBB_BYTE slot,NBB_USHORT port,SPM_HQOS_LOG_USR_CB *usr)
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

    if(NULL == usr)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }


    /***************************************************************************/
    /* �������VC�ϵ�����                                                    */
    /***************************************************************************/
    if(NULL != usr)
    {
        /*NP hqos ȥʹ��*/
        if(0 != usr->np_flow_id)
        {
            for(unit = 0; unit < c3_num;unit++)
            {
#if defined (SPU) || defined (PTN690_CES)
                ret = ApiC3SetL3UniHqos(unit, 0,&(usr->l3uni),0,usr->np_flow_id);
                if (ATG_DCI_RC_OK != ret)
                {
                    spm_hqos_l3uni_error_log(unit, 0,&(usr->l3uni),0,usr->np_flow_id,
                        __FUNCTION__,__LINE__,ret);
                    goto EXIT_LABEL;
                }
#endif         
              
            }
        }


        /*ɾ��voq*/
        if( 0 != usr->voq_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosVoqDelete(0, usr->voq_id, slot, port, usr->tm_vc);
            if (ATG_DCI_RC_OK != ret)       
            {
                spm_hqos_arad_voq_error_log(usr->voq_id,slot,port,
                    usr->tm_vc, __FUNCTION__,__LINE__,ret);
                rv = ret;
            } 
#endif    
         }

        /*ɾ��vc*/
        for(i = 0; i < MAX_HQOS_SLOT_NUM;i++)
        {
            if(0 != usr->vc_id[i])
            {
#if defined (SPU) || defined (SRC)
                ret = ApiAradHqosTrafficDelete(0, usr->voq_id, NUM_COS, g_hqos_port_index[i],
                        port, usr->vc_id[i],usr->pw_id);
                if (ATG_DCI_RC_OK != ret)       
                {
                    spm_hqos_arad_traffic_del_error_log(usr->voq_id,g_hqos_port_index[i],
                        port, usr->vc_id[i],usr->pw_id, __FUNCTION__,__LINE__,ret);
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
static NBB_LONG spm_hqos_set_gourp_usr_property(NBB_BYTE slot,NBB_USHORT port,
     NBB_LONG lsp_id,NBB_LONG pw_id,TX_PORT_T *l3uni,NBB_LONG base_queue,
     ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *data,ATG_DCI_TX_LSP_QOS *group_data)
{
    NBB_LONG unit = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_BYTE i = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == data || NULL == l3uni)
    {
        spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }



    /*�����̲��Ҳ���Ĭ��LSP�ڵ����������LSP ��Ĭ��PW��������*/
    if(slot == v_spm_shared->local_slot_id && lsp_id != port)
    {
        if(NULL != group_data && 0 != lsp_id)
        {
#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosLspBandwidthSet(unit, lsp_id, group_data->normal_shaping.cir,
                  group_data->normal_shaping.pir, group_data->normal_shaping.cbs, 
                  group_data->normal_shaping.pbs);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_set_lsp_bandwidth_error_log(lsp_id, group_data->normal_shaping.cir,
                  group_data->normal_shaping.pir, group_data->normal_shaping.cbs, 
                  group_data->normal_shaping.pbs,
                    __FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif

#if defined (SPU) || defined (SRC)
            ret = ApiAradHqosLspWfqWeightSet(unit, lsp_id, group_data->normal_strategy.weight);
            if (ATG_DCI_RC_OK != ret)
            {
                spm_hqos_arad_set_lsp_weight_error_log(lsp_id,
                    group_data->normal_strategy.weight,__FUNCTION__,__LINE__,ret);
                goto EXIT_LABEL;
            }
#endif
        }
     
    }

    ret = spm_hqos_set_pw_property(slot,pw_id,base_queue,(ATG_DCI_TX_LSP_QOS*)data);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }

    /*����̺ͳ����̶���ִ��*/
    /*�������C3��flow id hqos��ҵ��ͨ����*/
    for(unit = 0; unit < c3_num;unit++)
    {
#if defined (SPU) || defined (PTN690_CES)
        ret = ApiC3SetL3UniHqos(unit, 0,l3uni,1,base_queue);
        if (ATG_DCI_RC_OK != ret)
        {
            spm_hqos_l3uni_error_log(unit, 0,l3uni,1,base_queue,
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
static NBB_LONG spm_hqos_set_usr_drive(NBB_BYTE slot,NBB_USHORT port,
        NBB_LONG lsp_id,NBB_LONG pw_id,TX_PORT_T* l3uni,
        ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *data,ATG_DCI_TX_LSP_QOS *group_data,
        NBB_LONG voq, NBB_LONG vc,NBB_LONG vc_id[MAX_HQOS_SLOT_NUM],SPM_HQOS_LOG_USR_CB *pcb
        ,SPM_HQOS_USER_GROUP_CB *group)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    

    NBB_TRC_ENTRY(__FUNCTION__);

    if ((NULL == data) || (NULL == vc_id) || (NULL == l3uni))
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


    /*����LSP�ڵ��HQOS������Ч*/
    ret = spm_hqos_set_gourp_usr_property(slot,port,lsp_id,pw_id,l3uni,
            voq,data,group_data);
    if (ATG_DCI_RC_OK != ret)
    {
        goto EXIT_LABEL;
    }
    if(NULL != pcb)
    {
        pcb->np_flow_id = voq;
        
        /*�������ݣ����ݲ�����Ӧ������*/
        OS_MEMCPY(&(pcb->data), data, sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));
    }
    if(NULL != group)
    {
        group->slot = slot;
        group->port = port;
        OS_MEMCPY(&(group->data), group_data, sizeof(ATG_DCI_TX_LSP_QOS));
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
NBB_LONG spm_hqos_add_usr_node(SUB_PORT *sub_port,NBB_ULONG logic_key,
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *data)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_LOG_USR_CB *pcb = NULL;
    SPM_HQOS_USER_GROUP_CB *pst_bnode = NULL;
    TX_PORT_T l3uni = {0};
    ATG_DCI_TX_LSP_QOS *group_data = NULL;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    NBB_ULONG group_key = 0;
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
    slot = sub_port->slot;
    port = sub_port->port;
    group_key = data->group_index;
    

    /*�����Ƿ����B�ڵ�*/
    pst_bnode = AVLL_FIND(g_qos_user_group_tree, &group_key);

    /* �����Ŀ�����������Ĭ��B�ڵ��LSP ID*/
    if (pst_bnode == NULL)
    {
        spm_qos_exception_log(__FUNCTION__,__LINE__);
        ret = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;

    }
    group_data = &(pst_bnode->data);

    pcb = AVLL_FIND(pst_bnode->usr_tree, &logic_key);
    
    if(NULL == pcb)/*�״ΰ󶨵��û���*/
    { 
        spm_dbg_record_hqos_logic_head(logic_key,SPM_OPER_ADD);
        pcb = spm_alloc_hqos_log_usr_cb(logic_key,&(pst_bnode->lsp_id),slot,port);
        if (NULL == pcb)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }     
        ret = spm_hqos_set_usr_drive(slot,port,pst_bnode->lsp_id,pcb->pw_id,&l3uni,data,
            &(pst_bnode->data),voq,vc,vc_id,pcb,pst_bnode);
        if (ATG_DCI_RC_OK != ret) /*���������Դ*/
        {
            spm_hqos_del_usr_node(pst_bnode,&pcb);
            spm_release_hqos_group_drive(pst_bnode);
        }

        /*ע�����ܸ澯*/
#ifdef PTN690
        spm_hqos_add_intf_pmline(logic_key,pcb->voq_id);
#endif
        rv = AVLL_INSERT(pst_bnode->usr_tree, pcb->spm_hqos_usr_node);
        
    }
    else
    {
        if(pst_bnode->slot != slot || pst_bnode->port != port)
        {
            spm_qos_exception_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }
        
        spm_dbg_record_hqos_logic_head(logic_key,SPM_OPER_UPD);

        /*���µ��µ��û�����ȥ����ʱ��֧�ֱ���*/
        if(group_key != pst_bnode->user_group_key)
        {
            spm_qos_exception_log(__FUNCTION__,__LINE__);
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        ret = spm_hqos_set_gourp_usr_property(slot,port,0,pcb->pw_id,&l3uni,
            voq,data,NULL);
        if (ATG_DCI_RC_OK != ret)
        {
            goto EXIT_LABEL;
        }
    }   

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;
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
NBB_LONG spm_hqos_del_usr_node(SPM_HQOS_USER_GROUP_CB *group,SPM_HQOS_LOG_USR_CB **pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv = ATG_DCI_RC_OK;
    SPM_HQOS_LOG_USR_CB *usr = NULL;
    NBB_LONG unit = 0;
    NBB_BYTE c3_num = v_spm_shared->c3_num;
    NBB_BYTE i = 0;
    NBB_LONG lsp_id = 0;
    NBB_BYTE slot = 0;
    NBB_USHORT port = 0;
    
    NBB_TRC_ENTRY(__FUNCTION__);

    /*LSP�ڵ㲻����ΪNULL*/
    if(NULL == pcb || NULL == group)
    {
       spm_qos_parameter_error_log(__FUNCTION__,__LINE__);
       ret = ATG_DCI_RC_UNSUCCESSFUL;
       goto EXIT_LABEL;
    }

    usr = *pcb;
    slot = group->slot;
    port = group->port;
    lsp_id = group->lsp_id;


    /***************************************************************************/
    /* �������VC�ϵ�����                                                    */
    /***************************************************************************/
    if(NULL != usr)
    {
        spm_dbg_record_hqos_logic_head(usr->index,SPM_OPER_DEL);
        
        ret = spm_hqos_del_usr_drive(slot,port,usr);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }

         /*ע�����ܸ澯*/
#ifdef PTN690
         spm_hqos_del_intf_pmline(usr->index);
#endif


        /*����ɾ���Ѵ��ڵ���ȷ����*/
        if(AVLL_IN_TREE(usr->spm_hqos_usr_node))
        {
            AVLL_DELETE(group->usr_tree, usr->spm_hqos_usr_node);
        }

        /*�ͷ��ڴ���Դ*/
        ret = spm_free_hqos_log_usr_cb(usr,lsp_id);
        if (ATG_DCI_RC_OK != ret) 
        {
            rv = ret;
        }
        *pcb = NULL;/*ָ�����ΪNULL*/ 
         
    }
  

   
    /*�쳣����*/
    EXIT_LABEL: NBB_TRC_EXIT();
    return rv;
}


#endif



#if 8




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
NBB_LONG spm_set_hqos_slot_cfg(NBB_ULONG oper, NBB_BYTE *buf)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_LONG ret = ATG_DCI_RC_OK;
    ATG_DCI_HQOS_SLOT_DATA *ptemp = NULL;
    NBB_BYTE i = 0;
    NBB_BYTE hqos_slot = 0;

    NBB_TRC_ENTRY(__FUNCTION__);

    /* ips��Ϣ���� */
    switch (oper)
    {
        /* ���� */
        case  ATG_DCI_OPER_UPDATE:
        break;

        /* ɾ�� */
        case  ATG_DCI_OPER_DEL:
        break;

        /* ���� */
        case  ATG_DCI_OPER_ADD:

        /* ��Ϣ����Ϊ�� */
        if (NULL == buf)
        {
            ret = ATG_DCI_RC_UNSUCCESSFUL;
            goto EXIT_LABEL;
        }

        /* ��Ϣ���ݲ�Ϊ�� */
        else
        {
            ptemp = (ATG_DCI_HQOS_SLOT_DATA *)buf;

            for (i = 0; i < ATG_DCI_HQOS_SLOT_NUM; i++)
            {
                if ((0 != ptemp->enable_switch[i].enable) && (hqos_slot < MAX_HQOS_SLOT_NUM))
                {
                    g_hqos_port_index[hqos_slot++] = i + 1;
                }
            }
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
NBB_VOID spm_rcv_dci_set_hqos(ATG_DCI_SET_HQOS *pst_setips)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE uc_exist = QOS_EXIST;
    NBB_ULONG ulkey = 0;
    NBB_LONG ret = ATG_DCI_RC_OK;
    NBB_LONG rv  = ATG_DCI_RC_OK;
    NBB_BYTE *puc_datastart = NULL;
    NBB_ULONG ul_oper = ATG_DCI_OPER_NULL;

    NBB_TRC_ENTRY(__FUNCTION__);

    if(NULL == pst_setips)
    {
        printf("**QOS ERROR**%s,%d IPS == NULL\n",__FUNCTION__,__LINE__);
        NBB_EXCEPTION((PCT_SPM| QOS_PD, 1,  "s d s s s s d d d d", 
                	   "QOS IPS = NULL",ATG_DCI_RC_UNSUCCESSFUL,
                	   "","","","",0,0,0,0));
        goto EXIT_LABEL;
    }

    pst_setips->return_code = ATG_DCI_RC_OK;
    ulkey = pst_setips->key;
    if (1 != ulkey)
    {
        pst_setips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        goto EXIT_LABEL;
    }


    ul_oper = pst_setips->oper_hqos_slot;
    puc_datastart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_setips, &pst_setips->hqos_slot_data);

    /* ips��ϢΪɾ�� */
    if (TRUE == pst_setips->delete_struct)
    {
        
        OS_MEMSET(g_hqos_port_index,0,sizeof(g_hqos_port_index));
      
    }

    /* ���ӻ��߸��� */
    else
    {

        ret = spm_set_hqos_slot_cfg(ul_oper, puc_datastart);
        pst_setips->hqos_slot_return_code = ret;

    }

    /* �쳣���� */
    EXIT_LABEL: NBB_TRC_EXIT();
    return;
}



#endif



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
NBB_VOID show_hqos_lsp(NBB_LONG id)
{
#if defined (SPU) || defined (SRC)
  DbgHQosLspInfoPrint(0, id);
#endif
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
NBB_VOID show_hqos_pw(NBB_LONG id)
{
#if defined (SPU) || defined (SRC)
  DbgHQosInfoPrint(0, id);
#endif
}

/*****************************************************************************
   �� �� ��  : spm_show_hqos_cfg
   ��������  : ��ʾ����
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��1��15�� ���ڶ�
    ��    ��   : zenglu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID qos_show_hqos_slot()
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_ULONG index = 1;
    NBB_USHORT i = 0;


    for (i = 0; i < MAX_HQOS_SLOT_NUM; i++)
    {
        if (0 != g_hqos_port_index[i])
        {
            printf("slot = %d start hqos\n",  g_hqos_port_index[i]);
        }
    }


    EXIT_LABEL:

    return;

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
NBB_VOID qos_show_hqos_summary()
{
    NBB_ULONG lsp_num = 0;
    NBB_ULONG pw_num = 0;
    NBB_ULONG id = 0;

    
    for (id = 0; id < HQOS_MAX_LSP_ID; id++)
    {
        if (ATG_DCI_RC_OK != g_hqos_lsp_pool[id]) 
        {
            lsp_num++;
        }
    }
    for (id = 0; id < HQOS_MAX_PW_ID; id++)
    {
        if (ATG_DCI_RC_OK != g_hqos_pw_pool[id]) 
        {
            pw_num++;
        }
    }

    printf("lsp used num = %ld,pw used num=%ld\n\n", lsp_num,pw_num);

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
NBB_VOID qos_show_hqos_print_usr(SPM_HQOS_LOG_USR_CB *usr)
{

    if(NULL != usr)
    {
        printf("port index = %ld group_index = % ld\n",usr->index,(usr->data).group_index);
        qos_print_vc_hqos_cfg((ATG_DCI_TX_LSP_QOS*)&(usr->data));        
        printf("\n\n");
    }

    return;
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
NBB_VOID qos_show_usrgroup(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_HQOS_USER_GROUP_CB *pcb = NULL;
    SPM_HQOS_LOG_USR_CB *usr = NULL;
        
    NBB_BYTE pt = ATG_DCI_RC_OK;

    pt = g_qos_hqos_print;
    g_qos_hqos_print = ATG_DCI_RC_UNSUCCESSFUL;

    for (pcb = (SPM_HQOS_USER_GROUP_CB *)AVLL_FIRST(g_qos_user_group_tree);pcb != NULL;
         pcb = (SPM_HQOS_USER_GROUP_CB *)AVLL_NEXT(g_qos_user_group_tree, pcb->spm_qos_user_group_node))
    {
        if(0 == index || index == pcb->user_group_key)
        {
            printf("user_group_key = %ld,slot = %d,port = %d,lsp_id= %ld\n\n", 
                pcb->user_group_key,pcb->slot,pcb->port,pcb->lsp_id);  
            qos_print_vc_hqos_cfg(&(pcb->data));

            for (usr = (SPM_HQOS_LOG_USR_CB *)AVLL_FIRST(pcb->usr_tree); usr != NULL;
                usr = (SPM_HQOS_LOG_USR_CB *)AVLL_NEXT(pcb->usr_tree, usr->spm_hqos_usr_node))
            {
                qos_show_hqos_print_usr(usr);   
            }
        }
    }

    g_qos_hqos_print = pt;


    return;

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
NBB_VOID qos_show_hqos_up(NBB_USHORT slot,NBB_USHORT port)
{
    SPM_HQOS_LSP_CB *pcb = NULL;
    SPM_HQOS_VC_CB *vc_cb = NULL;
    SPM_HQOS_VRF_CB *vrf_cb = NULL;

    NBB_BYTE pt = ATG_DCI_RC_OK;

    pt = g_qos_hqos_print;
    g_qos_hqos_print = ATG_DCI_RC_UNSUCCESSFUL;


    if((slot == 0) || (slot > MAX_SLOT_NUM))
    {
        printf("slot = %d is out of range\n", slot);
        goto EXIT_LABEL;
    }
    
    if(port >= MAX_PHYSIC_PORT_NUM)
    {
        printf("port = %d is out of range\n", port);
        goto EXIT_LABEL;
    }
    
    for (pcb = (SPM_HQOS_LSP_CB *)AVLL_FIRST(g_qos_port_cb[slot][port].lsp_tree);
        pcb != NULL;
        pcb = (SPM_HQOS_LSP_CB *)AVLL_NEXT(g_qos_port_cb[slot][port].lsp_tree,pcb->spm_hqos_lsp_tx_node))
    {
        qos_print_lsp_key(&(pcb->lsp_tx_key));
        printf("\n\t\t slot=%d,port=%d,lsp_id=%ld,pw_id=%ld,posid=%ld,voq_id=%ld,tm_vc=%ld\n"
            "vc_id[0]=%ld,vc_id[1]=%ld,vc_id[2]=%ld,vc_id[3]=%ld\n",
            pcb->slot,pcb->port,pcb->lsp_id,pcb->pw_id,pcb->posid,pcb->voq_id,
            pcb->tm_vc,pcb->vc_id[0],pcb->vc_id[1],pcb->vc_id[2],pcb->vc_id[3]);
        if(0 != pcb->data.node_index)/*����ӡĬ�Ͻڵ�*/
        {
            qos_print_vc_hqos_cfg(&(pcb->data));
        }
        
        

        for (vrf_cb = (SPM_HQOS_VRF_CB *)AVLL_FIRST(pcb->vrf_tree);
            vrf_cb != NULL;
            vrf_cb = (SPM_HQOS_VRF_CB *)AVLL_NEXT(pcb->vrf_tree, vrf_cb->spm_hqos_vrf_node))
        {
            printf("\n");
            qos_print_vrf_key(&(vrf_cb->vrf_key));
            printf("\n\t\t pw_id=%ld,posid=%ld,voq_id=%ld,tm_vc=%ld\n"
                "vc_id[0]=%ld,vc_id[1]=%ld,vc_id[2]=%ld,vc_id[3]=%ld\n",
                vrf_cb->pw_id,vrf_cb->posid,vrf_cb->voq_id,vrf_cb->tm_vc,
                vrf_cb->vc_id[0],vrf_cb->vc_id[1],vrf_cb->vc_id[2],vrf_cb->vc_id[3]);
            qos_print_vc_hqos_cfg(&(vrf_cb->vrf_data));
        }
        for (vc_cb = (SPM_HQOS_VC_CB *)AVLL_FIRST(pcb->vc_tree);
            vc_cb != NULL;
            vc_cb = (SPM_HQOS_VC_CB *)AVLL_NEXT(pcb->vc_tree, vc_cb->spm_hqos_vc_node))
        {
            printf("\n");
            qos_print_vc_key(&(vc_cb->vc_key));
            printf("\n\t\t pw_id=%ld,posid=%ld,voq_id=%ld,tm_vc=%ld\n"
                "vc_id[0]=%ld,vc_id[1]=%ld,vc_id[2]=%ld,vc_id[3]=%ld\n",
                vc_cb->pw_id,vc_cb->posid,vc_cb->voq_id,vc_cb->tm_vc,
                vc_cb->vc_id[0],vc_cb->vc_id[1],vc_cb->vc_id[2],vc_cb->vc_id[3]);
            qos_print_vc_hqos_cfg(&(vc_cb->vc_data));
        }
    }

    g_qos_hqos_print = pt;

    EXIT_LABEL:
    return;
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
NBB_VOID qos_show_hqos_cfg()
{
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;

    for(j = 1; j < MAX_SLOT_NUM + 1;j++)
    {
        for (i = 0; i < MAX_PHYSIC_PORT_NUM; i++)
        {
            qos_show_hqos_up(j,i);  
        } 
    }

    qos_show_usrgroup(0);
    

    return;
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
NBB_VOID qos_show_hqos_voq(NBB_LONG slot, NBB_ULONG node_index)
{
    NBB_LONG offset = MAX_HQOS_SLOT_NUM * NUM_COS;
    NBB_LONG base_queue = 0;
    NBB_LONG ret = 0;
    NBB_LONG fapid_index = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    ret = spm_hqos_checkslot(slot, &fapid_index);

    if (ATG_DCI_RC_OK != ret)
    {
        printf("slot=%ld is not hqos slot\n", fapid_index);
    }

    base_queue = fapid_index * NUM_COS + offset * node_index + HQOS_OFFSET;

    printf("flow id=%ld\n", base_queue);
}


/**PROC+**********************************************************************/
/* Name:                                                                     */
/*                                                                           */
/* Purpose:                                .                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID qos_hqos_help()
{
    NBB_CHAR **ppc_msg;

    static NBB_CHAR *s_help_msg[] = {     
        "qos_show_hqos_slot()",                     "show slot start hqos func",
        "qos_show_hqos_cfg()",                      "show all hqos tree",
        "qos_show_hqos_down(slot,port)",            "show down hqos tree",
        "qos_show_hqos_up(slot,port)",              "show up hqos tree",
        "qos_show_hqos_voq(dst_slot,node_index)",   "show hqos voq",
        "qos_show_voq(dst_slot,dst_port)",          "show port voq", 
        "qos_show_hqos_summary()",                  "show hqos resource",
        "show_hqos_lsp(id)",                "show hqos lsp info",
        "show_hqos_pw(id)",                 "show hqos pw info",
        NULL
    };

    printf("\n");

    for (ppc_msg = s_help_msg; *ppc_msg != NULL; ppc_msg += 2)
    {
        if (strlen(*(ppc_msg)) > 45)
        {
            printf(" %s %s\n", *ppc_msg, *(ppc_msg + 1));
        }
        else
        {
            printf(" %-40s %s\n", *ppc_msg, *(ppc_msg + 1));
        }
    }

    printf("\n");
}


        

/**PROC+**********************************************************************/
/* Name:                                                                     */
/*                                                                           */
/* Purpose:                                .                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_hqos_cb_verify()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_HQOS_LSP_CB *lsp_cb = NULL;
    SPM_HQOS_VRF_CB *vrf_cb = NULL;
    SPM_HQOS_VC_CB *vc_cb = NULL;
    SPM_HQOS_USER_GROUP_CB *group_cb = NULL;
    SPM_HQOS_LOG_USR_CB *usr_cb = NULL;
    NBB_USHORT i = 0;
    NBB_USHORT j = 0;

    NBB_TRC_ENTRY("spm_cfg_hqos_cb_verify");

    for(j = 1;j < MAX_SLOT_NUM + 1;j++)
    {
        for (i = 0; i < MAX_PHYSIC_PORT_NUM; i++)
        {
            for (lsp_cb = (SPM_HQOS_LSP_CB*) AVLL_FIRST(g_qos_port_cb[j][i].lsp_tree);
                 lsp_cb != NULL;
                 lsp_cb = (SPM_HQOS_LSP_CB*) AVLL_NEXT(g_qos_port_cb[j][i].lsp_tree,
                               lsp_cb->spm_hqos_lsp_tx_node))
            {

                NBB_VERIFY_MEMORY(lsp_cb, sizeof(SPM_HQOS_LSP_CB), MEM_SPM_HQOS_LSP_TX_CB);


                 for(vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_FIRST(lsp_cb->vrf_tree);
                     vrf_cb != NULL;
                     vrf_cb = (SPM_HQOS_VRF_CB*) AVLL_NEXT(lsp_cb->vrf_tree,
                                   vrf_cb->spm_hqos_vrf_node))
                 {
                     NBB_TRC_FLOW((NBB_FORMAT "Verify vrf_cb cb %p", vrf_cb));
                     NBB_VERIFY_MEMORY(vrf_cb, sizeof(SPM_HQOS_VRF_CB),
                                      MEM_SPM_HQOS_VRF_CB);
                 }

                 for(vc_cb = (SPM_HQOS_VC_CB*) AVLL_FIRST(lsp_cb->vc_tree);
                     vc_cb != NULL;
                     vc_cb = (SPM_HQOS_VC_CB*) AVLL_NEXT(lsp_cb->vc_tree,
                                   vc_cb->spm_hqos_vc_node))
                 {
                     NBB_TRC_FLOW((NBB_FORMAT "Verify vc_cb cb %p", vc_cb));
                     NBB_VERIFY_MEMORY(vc_cb, sizeof(SPM_HQOS_VC_CB),
                                      MEM_SPM_HQOS_VC_CB);
                 }
            }

        }
    }
    
    for (group_cb = (SPM_HQOS_USER_GROUP_CB*) AVLL_FIRST(g_qos_user_group_tree);
                 group_cb != NULL;
                 group_cb = (SPM_HQOS_USER_GROUP_CB*) AVLL_NEXT(g_qos_user_group_tree,
                               group_cb->spm_qos_user_group_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify group_cb %p", group_cb));
        NBB_VERIFY_MEMORY(group_cb, sizeof(SPM_HQOS_USER_GROUP_CB), MEM_SPM_USER_GROUP_CB);


         for(usr_cb = (SPM_HQOS_LOG_USR_CB*) AVLL_FIRST(group_cb->usr_tree);
             usr_cb != NULL;
             usr_cb = (SPM_HQOS_LOG_USR_CB*) AVLL_NEXT(group_cb->usr_tree,
                           usr_cb->spm_hqos_usr_node))
         {
             NBB_TRC_FLOW((NBB_FORMAT "Verify vrf_cb cb %p", usr_cb));
             NBB_VERIFY_MEMORY(usr_cb, sizeof(SPM_HQOS_LOG_USR_CB),
                              MEM_SPM_HQOS_LOG_USR_CB);
         }  
    }
    NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_qos_acl_init
   ��������  : Ĭ�ϵ�DSӳ���ϵ��ʼ��
   �������  :
   �������  :
   �� �� ֵ  :
   ���ú���  :
   ��������  :
   �޸���ʷ  :
   ��    ��  : 2013��1��15�� ���ڶ�
   ��    ��  : zenglu
   �޸�����  : �����ɺ���

*****************************************************************************/
NBB_LONG spm_qos_hqos_init(void)
{
    NBB_ULONG i = 0;
    NBB_ULONG j = 0;
    NBB_BUF_SIZE avll_key_offset;

    /* ���ж˿�HQOS���ĳ�ʼ�� */
    for(j = 0;j < MAX_SLOT_NUM + 1;j++)
    {
       for (i = 0; i < MAX_PHYSIC_PORT_NUM; i++)
       {
            g_qos_port_cb[j][i].port_key = i;

            avll_key_offset = NBB_OFFSETOF(SPM_HQOS_LSP_CB, lsp_tx_key);
            AVLL_INIT_TREE(g_qos_port_cb[j][i].lsp_tree, spm_qos_tunnel_comp,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_LSP_CB, spm_hqos_lsp_tx_node));

            /*avll_key_offset = NBB_OFFSETOF(SPM_HQOS_VRF_CB, vrf_key);
            AVLL_INIT_TREE(g_qos_port_cb[j][i].pw_vrf_tree, spm_qos_vrf_comp,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_VRF_CB, spm_hqos_vrf_node));

            avll_key_offset = NBB_OFFSETOF(SPM_HQOS_VC_CB, vc_key);
            AVLL_INIT_TREE(g_qos_port_cb[j][i].pw_vc_tree, spm_vc_key_compare,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_VC_CB, spm_hqos_vc_node));

            avll_key_offset = NBB_OFFSETOF(SPM_HQOS_LOG_GROUP_CB, index);
            AVLL_INIT_TREE(SHARED.qos_port_cb[j][i].group_tree, compare_ulong,
                (NBB_USHORT)avll_key_offset,
                (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_LOG_GROUP_CB, spm_hqos_group_node));*/
        } 
    }


    /* �û�������ʼ�� */
    avll_key_offset = NBB_OFFSETOF(SPM_HQOS_USER_GROUP_CB,user_group_key);
    AVLL_INIT_TREE(g_qos_user_group_tree, compare_ulong,
            (NBB_USHORT)avll_key_offset,
            (NBB_USHORT)NBB_OFFSETOF(SPM_HQOS_USER_GROUP_CB,spm_qos_user_group_node));



    OS_MEMSET(&g_hqos_lsp_pool, 0, sizeof(g_hqos_lsp_pool));

    OS_MEMSET(&g_hqos_pw_pool, 0, sizeof(g_hqos_pw_pool));

    //OS_MEMSET(&g_hqos_port_index, 0, sizeof(g_hqos_port_index));
    
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
NBB_VOID spm_hqos_clear_all()
{
    
}

