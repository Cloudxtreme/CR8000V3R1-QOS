/******************************************************************************

                  ��Ȩ���� (C), 1999-2014, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_msp_proc.c
  �� �� ��   : ����
  ��    ��   : xiaoxiang
  ��������   : 2014��4��10��
  ����޸�   :
  ��������   : ����˿ھ�����
  �����б�   :
              spm_alloc_msp_cb
              spm_free_msp_cb
              spm_init_msp_tree
              spm_rcv_dci_set_msp
  �޸���ʷ   :
  1.��    ��   : 2014��4��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char g_msp_cfg_print_setting;

/*****************************************************************************
 �� �� ��  : spm_init_msp_tree
 ��������  : ��ʼ���˿ھ�����
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��4��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_msp_tree()
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_msp_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_MSP_CB, msp_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.msp_tree, compare_ulong,
        (NBB_USHORT) avll_key_offset,
        (NBB_USHORT) NBB_OFFSETOF(SPM_MSP_CB, spm_msp_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 �� �� ��  : spm_alloc_msp_cb
 ��������  : ����MSP�����ṹ����ڴ�ռ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��4��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_MSP_CB* spm_alloc_msp_cb()
{
    SPM_MSP_CB *pst_msp = NULL;

    NBB_TRC_ENTRY("spm_alloc_msp_cb");

    /* ����һ���µ�MSP����������Ŀ��*/
    pst_msp = (SPM_MSP_CB *)NBB_MM_ALLOC(sizeof(SPM_MSP_CB),NBB_NORETRY_ACT, MEM_SPM_MSP_CB);
    
    if (NULL == pst_msp)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��MSP����������Ŀ */
    OS_MEMSET(pst_msp, 0, sizeof(SPM_MSP_CB));
    pst_msp->basic_cfg_cb = NULL;

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pst_msp->spm_msp_handle = NBB_CREATE_HANDLE(pst_msp, HDL_SPM_MSP_CB);
  
    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_msp->spm_msp_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pst_msp);
}

/*****************************************************************************
 �� �� ��  : spm_free_msp_cb
 ��������  : �ͷ�MSP�����ṹ����ڴ�ռ�
 �������  : SPM_MSP_CB *pst_msp  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��4��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_msp_cb(SPM_MSP_CB *pst_msp)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
  
    NBB_TRC_ENTRY("spm_free_msp_cb");
    
    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_msp, sizeof(SPM_MSP_CB), MEM_SPM_MSP_CB); 

    if (NULL == pst_msp)
    {
    	goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (NULL != pst_msp->basic_cfg_cb)
    {
        NBB_MM_FREE(pst_msp->basic_cfg_cb, MEM_SPM_MSP_BASIC_CB);  
        pst_msp->basic_cfg_cb = NULL;
    }    

    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_msp->spm_msp_handle, HDL_SPM_MSP_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_msp, MEM_SPM_MSP_CB);  
    pst_msp = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_free_all_msp_cb
   ��������  : �ͷ�MSP�����������е��ڴ�ռ�
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��04��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_all_msp_cb()
{
    SPM_MSP_CB *pst_msp = NULL;

    for (pst_msp = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree);
        pst_msp != NULL;
        pst_msp = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree))
    {    	
        AVLL_DELETE(SHARED.msp_tree, pst_msp->spm_msp_node);
        spm_free_msp_cb(pst_msp);
    }
}

/*****************************************************************************
�� �� ��  : spm_physical_port_basic_proc
��������  : ����ӿ���tlv--�������ã������� 1)
�������  : 
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2015��8��10��
��    ��   : zhangzhm
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_msp_basic_proc(ATG_DCI_SET_MSP *pst_set_msp,
    SPM_MSP_CB *pst_msp,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_MSP_BASIC *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
     
    if(NULL == pst_set_msp)
    {
        ret = MSP_BASIC_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_msp)
    {
        ret = MSP_BASIC_PROC_LOCAL_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_msp, 
               &pst_set_msp->basic_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = MSP_BASIC_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_MSP_BASIC *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_msp_cfg_print_setting)
            {
                printf("  1��basic Config\n");
                spm_dbg_print_msp_basic_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  1��basic Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            spm_msp_aps_add_prot_group(pst_msp,pst_tlv_cfg);
            spm_msp_drv_add_prot_group(pst_msp,pst_tlv_cfg);

            /* �������� */
            if (NULL == pst_msp->basic_cfg_cb)
            {
                pst_msp->basic_cfg_cb = (ATG_DCI_MSP_BASIC *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_MSP_BASIC),
                    NBB_NORETRY_ACT,
                    MEM_SPM_MSP_BASIC_CB);
                
                if (NULL == pst_msp->basic_cfg_cb)
                {
                    ret = MSP_BASIC_PROC_ALLOC_CB_FAIL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_msp->basic_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_MSP_BASIC));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = MSP_BASIC_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = MSP_BASIC_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = MSP_BASIC_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}

/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_msp
 ��������  : ����˿ھ���IPS��Ϣ
 �������  : ATG_DCI_SET_MSP *pst_set_msp  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��15��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_msp_proc(ATG_DCI_SET_MSP *pst_set_msp)
{
    NBB_INT ret = SUCCESS;
    SPM_MSP_CB *pst_msp = NULL;   
    NBB_ULONG oper_basic = ATG_DCI_OPER_NULL;   
    NBB_ULONG msp_key = 0;
    NBB_BYTE uc_if_exist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_msp_proc");

    /* �������ָ�������Ч */
    NBB_ASSERT(pst_set_msp != NULL);

    if (NULL == pst_set_msp)
    {
        ret = MSP_PROC_IPS_NULL;
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pst_set_msp->return_code = ATG_DCI_RC_OK;
    msp_key = pst_set_msp->key;
    pst_msp = AVLL_FIND(SHARED.msp_tree, &msp_key);
    
    if (NULL == pst_msp)       /* �����Ŀ������*/
    {
        uc_if_exist = ATG_DCI_UNEXIST;
    }    

    /* ��ȡ�����ò��� */
    oper_basic = pst_set_msp->oper_basic;
   
    /* ɾ��������Ŀ */
    if (TRUE == pst_set_msp->delete_struct)
    {        
        /* �����Ŀ�����ڣ���ɾ�� */
        if (ATG_DCI_UNEXIST == uc_if_exist)
        {
            ret = MSP_PROC_DEL_NULL_LOCAL_CFG;
            spm_l2_record_err_code(ret);       
            goto EXIT_LABEL;
        }      
        else/* ���ڣ�ɾ�� */
        {
            spm_dbg_print_msp_head(msp_key, SPM_OPER_DEL);
            spm_dbg_record_msp_head(msp_key, SPM_OPER_DEL);
            spm_msp_aps_del_prot_group(msp_key);
            
            if (NULL != pst_msp)
            {
                AVLL_DELETE(SHARED.msp_tree, pst_msp->spm_msp_node);  

                //�ͷ�MSP����Э��ڵ���ڴ�ռ�
                spm_free_msp_cb(pst_msp);
            }
        }
    }
    /* ���ӻ������Ŀ */
    else
    {        
        /* �����Ŀ�����ڣ��������ڴ�ռ䱣������ */
        if (ATG_DCI_UNEXIST == uc_if_exist)
        {
            spm_dbg_print_msp_head(msp_key, SPM_OPER_ADD);
            spm_dbg_record_msp_head(msp_key, SPM_OPER_ADD);

            /* ����һ������Ŀ���ڴ�ռ� */
            pst_msp = spm_alloc_msp_cb();

            if(NULL == pst_msp)
            {
                ret = MSP_PROC_ALLOC_CB_FAIL;
                spm_l2_record_err_code(ret);   
                goto EXIT_LABEL;
            }
            if (NULL != pst_msp)
            {
                //���뵽tree��
                pst_msp->msp_key = msp_key;

                //coverity[no_effect_test]
                AVLL_INSERT(SHARED.msp_tree, pst_msp->spm_msp_node);
            }
        }
        else
        {
            spm_dbg_print_msp_head(msp_key, SPM_OPER_UPD);
            spm_dbg_record_msp_head(msp_key, SPM_OPER_UPD);
        }
        
        /************************************ 1) �������� **************************************/
        if(ATG_DCI_OPER_NULL != oper_basic)
        {
            spm_msp_basic_proc(pst_set_msp, pst_msp,oper_basic); 
        }
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}


