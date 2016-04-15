/******************************************************************************

              ��Ȩ���� (C), ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************

�� �� ��   : spm_physical_port_cfg.c
�� �� ��   : ����
��    ��   : zhangzhm
��������   : 2015��8��10��
����޸�   :
��������   : ����ӿ�������ش�����
�����б�   :
�޸���ʷ   :
1.��    ��   : 2015��8��10��
��    ��   : zhangzhm
�޸�����   : �����ļ�

******************************************************************************/
    
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL
    
#include <nbase.h>
#include <spmincl.h>
    
#ifdef PHYSICAL_PORT_CFG

extern unsigned char g_physical_port_cfg_print_setting;//����

/*****************************************************************************
 �� �� ��  : spm_del_physical_port_cfg
 ��������  : ɾ������ӿ�����
 �������  : SPM_PHYSICAL_PORT* pstPhysicalPort  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_physical_port_del_cfg(SPM_PHYSICAL_PORT_CB* pst_physical_port NBB_CCXT_T NBB_CXT)
{
    return 0;
    
}

/*****************************************************************************
�� �� ��  : spm_physical_port_get_phy_info_from_index
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
NBB_INT spm_physical_port_get_phy_info_from_index(NBB_ULONG port_index,
    SPM_PHYSICAL_PORT_INFO_CB *pst_phy_port_info)
{
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
     
    if(NULL == pst_phy_port_info)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
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
NBB_INT spm_physical_port_basic_proc(
    ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_BASIC_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_basic_proc"); 
    
    if(NULL == pst_set_physical_port)
    {
        ret = PHY_PORT_BASIC_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = PHY_PORT_BASIC_PROC_LOCAL_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
               &pst_set_physical_port->basic_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = PHY_PORT_BASIC_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_BASIC_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  1��basic Config\n");
                spm_dbg_print_physical_port_basic_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  1��basic Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_basic_cfg(pst_physical_port,pst_tlv_cfg);

            /* �������� */
            if (NULL == pst_physical_port->basic_cfg_cb)
            {
                pst_physical_port->basic_cfg_cb = (ATG_DCI_PHY_PORT_BASIC_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_BASIC_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_BASIC_CB);
                
                if (NULL == pst_physical_port->basic_cfg_cb)
                {
                    ret = PHY_PORT_BASIC_PROC_ALLOC_CB_FAIL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->basic_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_BASIC_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = PHY_PORT_BASIC_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = PHY_PORT_BASIC_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = PHY_PORT_BASIC_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;    
}

/*****************************************************************************
�� �� ��  : spm_physical_port_eth_phy_proc
��������  : ����ӿ���tlv--  "2����̫���ӿ���������"  
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
NBB_VOID spm_physical_port_eth_phy_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_PHY_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_eth_phy_proc"); 
    
    if(NULL == pst_set_physical_port)
    {
        ret = PHY_PORT_ETH_PHY_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = PHY_PORT_ETH_PHY_PROC_LOCAL_CFG_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
               &pst_set_physical_port->physical_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = PHY_PORT_ETH_PHY_PROC_GET_DATA_START_FAIL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_PHY_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  2��eth physical Config\n");
                spm_dbg_print_physical_port_phy_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  2��eth physical Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_phy_cfg(pst_physical_port,pst_tlv_cfg);

            //if(SUCCESS != func_ret)
            //{
                //ret = PHY_PORT_ETH_PHY_PROC_CALL_DRV_FAIL;
                //spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            //}

            /* �������� */
            if (NULL == pst_physical_port->phy_cfg_cb)
            {
                pst_physical_port->phy_cfg_cb = (ATG_DCI_PHY_PORT_PHY_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_PHY_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_PHY_CB);
                
                if (NULL == pst_physical_port->phy_cfg_cb)
                {
                    ret = PHY_PORT_ETH_PHY_PROC_ALLOC_CB_FAIL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->phy_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_PHY_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = PHY_PORT_ETH_PHY_PROC_OPER_CODE_UPDATE;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = PHY_PORT_ETH_PHY_PROC_OPER_CODE_DEL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = PHY_PORT_ETH_PHY_PROC_OPER_CODE_DEFAULT;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;      
}

/*****************************************************************************
�� �� ��  : spm_physical_port_pos_proc
��������  : ����ӿ���tlv-- "3)POS�ӿ���·������"
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
NBB_VOID spm_physical_port_pos_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_POS_LINK_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_pos_proc"); 
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
               &pst_set_physical_port->pos_link_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_POS_LINK_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  3��pos link Config\n");
                spm_dbg_print_physical_port_pos_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  3��pos link Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* �������� */
            if (NULL == pst_physical_port->pos_link_cfg_cb)
            {
                pst_physical_port->pos_link_cfg_cb = (ATG_DCI_PHY_PORT_POS_LINK_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_POS_LINK_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_POS_CB);
                
                if (NULL == pst_physical_port->pos_link_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->pos_link_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_POS_LINK_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

/*****************************************************************************
�� �� ��  : spm_physical_port_sdh_sovhd_proc
��������  : ����ӿ���tlv--4)SDH�ο�������
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
NBB_VOID spm_physical_port_sdh_sovhd_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_sdh_sovhd_proc"); 
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->sdh_soverhead_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  4��sdh sovhd Config\n");
                spm_dbg_print_physical_port_stm1_sovhd_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  4��sdh sovhd Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* �������� */
            if (NULL == pst_physical_port->stm1_sovhd_cfg_cb)
            {
                pst_physical_port->stm1_sovhd_cfg_cb = (ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_SDH_SOVHD_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_SDH_SOVHD_CB);
                
                if (NULL == pst_physical_port->stm1_sovhd_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->stm1_sovhd_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_SDH_SOVHD_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
               
/*****************************************************************************
�� �� ��  : spm_physical_port_ces_proc
��������  : ����ӿ���tlv--5)CES�ӿ�����
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
NBB_VOID spm_physical_port_ces_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_CES_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_ces_proc");  
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->ces_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_CES_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  5��ces Config\n");
                spm_dbg_print_physical_port_ces_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  5��ces Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* �������� */
            if (NULL == pst_physical_port->ces_cfg_cb)
            {
                pst_physical_port->ces_cfg_cb = (ATG_DCI_PHY_PORT_CES_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_CES_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_CES_CB);
                
                if (NULL == pst_physical_port->ces_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->ces_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_CES_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;      
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_cep_proc
��������  : ����ӿ���tlv--6)CEP�ӿڿ�������
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
NBB_VOID spm_physical_port_cep_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_CEP_OVHD_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_cep_proc");  
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->cep_overhead_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_CEP_OVHD_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  6��cep Config\n");
                spm_dbg_print_physical_port_cep_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  6��cep Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* �������� */
            if (NULL == pst_physical_port->cep_ovhd_cfg_cb)
            {
                pst_physical_port->cep_ovhd_cfg_cb = (ATG_DCI_PHY_PORT_CEP_OVHD_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_CEP_OVHD_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_CEP_CB);
                
                if (NULL == pst_physical_port->cep_ovhd_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->cep_ovhd_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_CEP_OVHD_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_efm_oam_proc
��������  : ����ӿ���tlv--7����̫��EFM OAM
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
NBB_VOID spm_physical_port_efm_oam_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_EFM_OAM_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_efm_oam_proc");  
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->efm_oam_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_EFM_OAM_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  7��eth efm oam Config\n");
                spm_dbg_print_physical_port_efm_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  7��eth efm oam Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            //spm_dbg_record_physical_port_phy_cfg(pst_tlv_cfg);

            /* �������� */
            if (NULL == pst_physical_port->efm_oam_cfg_cb)
            {
                pst_physical_port->efm_oam_cfg_cb = (ATG_DCI_PHY_PORT_EFM_OAM_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_EFM_OAM_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_EFM_OAM_CB);
                
                if (NULL == pst_physical_port->efm_oam_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->efm_oam_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_EFM_OAM_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_time_sync_proc
��������  : ����ӿ���tlv--8)ʱ��ͬ���˿�����
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
NBB_VOID spm_physical_port_time_sync_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_TIME_SYNC_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_time_sync_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->time_sync_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_TIME_SYNC_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  8��time sync Config\n");
                spm_dbg_print_physical_port_time_sync_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  8��time sync Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_time_sync(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->time_sync_cfg_cb)
            {
                pst_physical_port->time_sync_cfg_cb = (ATG_DCI_PHY_PORT_TIME_SYNC_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_TIME_SYNC_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_TIME_SYNC_CB);
                
                if (NULL == pst_physical_port->time_sync_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->time_sync_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_TIME_SYNC_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_msg_suppress_proc
��������  : ����ӿ���tlv--9����̫���ӿڱ�������
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
NBB_VOID spm_physical_port_msg_suppress_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_msg_suppress_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->msg_suppr_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  9��eth msg suppr Config\n");
                spm_dbg_print_physical_port_msg_suppr_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  9��eth msg suppr Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_msg_suppr(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->msg_suppr_cfg_cb)
            {
                pst_physical_port->msg_suppr_cfg_cb = (ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_MSG_SUPPR_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_MSG_SUPPR_CB);
                
                if (NULL == pst_physical_port->msg_suppr_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->msg_suppr_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_MSG_SUPPR_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_flow_shape_proc
��������  : ����ӿ���tlv--10����̫���ӿ���������
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
NBB_VOID spm_physical_port_flow_shape_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_FLOW_SHAPE *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_flow_shape_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->flow_shape_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_FLOW_SHAPE *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  10��eth flow shape Config\n");
                spm_dbg_print_physical_port_eth_flow_shape_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  10��eth flow shape Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_flow_shape(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->flow_shape_cfg_cb)
            {
                pst_physical_port->flow_shape_cfg_cb = (ATG_DCI_PHY_PORT_FLOW_SHAPE *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_FLOW_SHAPE),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_FLOW_SHAPE_CB);
                
                if (NULL == pst_physical_port->flow_shape_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->flow_shape_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_FLOW_SHAPE));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_schedule_policy_proc
��������  : ����ӿ���tlv--11����̫�����ڶ��е��Ȳ���
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
NBB_VOID spm_physical_port_schedule_policy_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_SCHEDUL_POLICY *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_schedule_policy_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->schedul_policy_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_SCHEDUL_POLICY *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  11��eth schedul policy Config\n");
                spm_dbg_print_physical_port_eth_schedul_policy_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  11��eth schedul policy Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_schedul_policy(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->schedule_policy_cfg_cb)
            {
                pst_physical_port->schedule_policy_cfg_cb = (ATG_DCI_PHY_PORT_SCHEDUL_POLICY *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_SCHEDUL_POLICY),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_SCHEDUL_POLICY_CB);
                
                if (NULL == pst_physical_port->schedule_policy_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->schedule_policy_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_SCHEDUL_POLICY));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_congest_policy_proc
��������  : ����ӿ���tlv--12����̫�����ڶ���ӵ������
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
NBB_VOID spm_physical_port_congest_policy_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_CONGEST_POLICY *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_congest_policy_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->congest_policy_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_CONGEST_POLICY *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  12��eth congest policy Config\n");
                spm_dbg_print_physical_port_eth_congest_policy_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  12��eth congest policy Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_eth_congest_policy(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->congest_policy_cfg_cb)
            {
                pst_physical_port->congest_policy_cfg_cb = (ATG_DCI_PHY_PORT_CONGEST_POLICY *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_CONGEST_POLICY),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_CONGEST_POLICY_CB);
                
                if (NULL == pst_physical_port->congest_policy_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->congest_policy_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_CONGEST_POLICY));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_ipv4_netflow_proc
��������  : ����ӿ���tlv--13��IPV4������ʹ������IPV4 NetFlow
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
NBB_VOID spm_physical_port_ipv4_netflow_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_IPV4NF_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_ipv4_netflow_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->ipv4nf_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_IPV4NF_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  13��ipv4 nf enable Config\n");
                spm_dbg_print_physical_port_ipv4_nf_enable(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  13��ipv4 nf enable Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_ipv4_nf_enable(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->ipv4_nf_cfg_cb)
            {
                pst_physical_port->ipv4_nf_cfg_cb = (ATG_DCI_PHY_PORT_IPV4NF_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_IPV4_NF_CB);
                
                if (NULL == pst_physical_port->ipv4_nf_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->ipv4_nf_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_ip_nf_sample_proc
��������  : ����ӿ���tlv--14��IP����������
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
NBB_VOID spm_physical_port_ip_nf_sample_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_IPNF_SAMPLER *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_ip_nf_sample_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->ipnf_sampler_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_IPNF_SAMPLER *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  14��ip nf sampler Config\n");
                spm_dbg_print_physical_port_ip_nf_smapler(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  14��ip nf sampler Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_ip_nf_smapler(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->ip_sample_cfg_cb)
            {
                pst_physical_port->ip_sample_cfg_cb = (ATG_DCI_PHY_PORT_IPNF_SAMPLER *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_IPNF_SAMPLER),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_IPV4_SAMPLE_CB);
                
                if (NULL == pst_physical_port->ip_sample_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->ip_sample_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_IPNF_SAMPLER));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_ipv6_netflow_proc
��������  : ����ӿ���tlv--15��IPV6������ʹ������
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
NBB_VOID spm_physical_port_ipv6_netflow_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_IPV6NF_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_ipv6_netflow_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->ipv6nf_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_IPV6NF_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  15��ipv6 nf enable Config\n");
                spm_dbg_print_physical_port_ipv6_nf_enable(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  15��ipv6 nf enable Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_ipv6_nf_enable(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->ipv6_nf_cfg_cb)
            {
                pst_physical_port->ipv6_nf_cfg_cb = (ATG_DCI_PHY_PORT_IPV6NF_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_IPV6_NF_CB);
                
                if (NULL == pst_physical_port->ipv6_nf_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->ipv6_nf_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

       
/*****************************************************************************
�� �� ��  : spm_physical_port_pve_proc
��������  : ����ӿ���tlv--17��POVE
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
NBB_VOID spm_physical_port_pve_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_POVE *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS;
    
    NBB_TRC_ENTRY("spm_physical_port_pve_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->pove_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_POVE *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  17��pove Config\n");
                
                //spm_dbg_print_physical_port_pove_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  17��pove Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_pove(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->pove_cfg_cb)
            {
                pst_physical_port->pove_cfg_cb = (ATG_DCI_PHY_PORT_POVE *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_POVE),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_POVE_CB);
                
                if (NULL == pst_physical_port->pove_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->pove_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_POVE));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}


/*****************************************************************************
�� �� ��  : spm_physical_port_common_proc
��������  : ����ӿ���tlv--18���ӿ�ͨ������
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
NBB_VOID spm_physical_port_common_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_COMMON_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_common_proc");   
     
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->common_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_COMMON_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  18��common Config\n");
                spm_dbg_print_physical_port_common_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  18��common Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_common_proc(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->common_cfg_cb)
            {
                pst_physical_port->common_cfg_cb = (ATG_DCI_PHY_PORT_COMMON_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_COMMON_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_COMMON_CB);
                
                if (NULL == pst_physical_port->common_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->common_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_COMMON_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_sdh_tovhd_proc
��������  : ����ӿ���tlv--19) SDHͨ����������
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
NBB_VOID spm_physical_port_sdh_tovhd_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_sdh_tovhd_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->sdh_toverhead_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  19��sdh tovhd Config\n");
                spm_dbg_print_physical_port_sdh_tovhd_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  19��sdh tovhd Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

            /* �������� */
            if (NULL == pst_physical_port->stm1_tovhd_cfg_cb)
            {
                pst_physical_port->stm1_tovhd_cfg_cb = (ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_SDH_TOVHD_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_SDH_TOVHD_CB);
                
                if (NULL == pst_physical_port->stm1_tovhd_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->stm1_tovhd_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_SDH_TOVHD_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }
    
    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}
        
/*****************************************************************************
�� �� ��  : spm_physical_port_als_proc
��������  : ����ӿ���tlv--20)ALS����
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
NBB_VOID spm_physical_port_als_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_ALS_DATA *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_als_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->als_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_ALS_DATA *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  20��als Config\n");
                spm_dbg_print_physical_port_als_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  20��als Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_als_cfg(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->als_cfg_cb)
            {
                pst_physical_port->als_cfg_cb = (ATG_DCI_PHY_PORT_ALS_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_ALS_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_ALS_CB);
                
                if (NULL == pst_physical_port->als_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->als_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_ALS_DATA));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

        
/*****************************************************************************
�� �� ��  : spm_physical_port_thres_proc
��������  : ����ӿ���tlv--21)�˿ڷ��ͽ��չ⹦������ֵ����
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
NBB_VOID spm_physical_port_thres_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_THRES *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_thres_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->thres_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_THRES *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  21��thres Config\n");
                
                //spm_dbg_print_physical_port_thres_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  21��thres Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);

            /* �������� */
            if (NULL == pst_physical_port->thres_cfg_cb)
            {
                pst_physical_port->thres_cfg_cb = (ATG_DCI_PHY_PORT_THRES *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_THRES),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_THRES_CB);
                
                if (NULL == pst_physical_port->thres_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->thres_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_THRES));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

         
/*****************************************************************************
�� �� ��  : spm_physical_port_mcc_type_proc
��������  : ����ӿ���tlv--22��MCCͨ������
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
NBB_VOID spm_physical_port_mcc_type_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port,
    SPM_PHYSICAL_PORT_CB *pst_physical_port,
    NBB_ULONG ul_oper)
{ 
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    ATG_DCI_PHY_PORT_MCC *pst_tlv_cfg = NULL;
    NBB_BYTE *puc_tlv_cfg_data_start = NULL;
    NBB_INT ret = SUCCESS; 
    NBB_INT func_ret = SUCCESS; 
    
    NBB_TRC_ENTRY("spm_physical_port_mcc_type_proc");   
    
    if(NULL == pst_set_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    if(NULL == pst_physical_port)
    {
        ret = SPM_LOGICAL_E_PARAM_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }
    
    switch(ul_oper)
    {
        case ATG_DCI_OPER_ADD:  

            //���������׵�ַ
            puc_tlv_cfg_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_physical_port, 
                &pst_set_physical_port->mcc_data); 

            if(NULL == puc_tlv_cfg_data_start)
            {
                ret = SPM_LOGICAL_E_PARAM_NULL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            pst_tlv_cfg = (ATG_DCI_PHY_PORT_MCC *)puc_tlv_cfg_data_start;

            if (SPM_PRINT_CFG == g_physical_port_cfg_print_setting)
            {
                printf("  22��mcc Config\n");
                
                //spm_dbg_print_physical_port_mcc_cfg(pst_tlv_cfg);
            }

            OS_SPRINTF(uc_message,"  22��mcc Config\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
            
            func_ret = spm_physical_port_drv_add_mcc_cfg(pst_physical_port,pst_tlv_cfg);

            if(SUCCESS != func_ret)
            {
                ret = SPM_LOGICAL_E_CALL_QOS;
                spm_l2_record_err_code(ret); 
                
                //goto EXIT_LABEL;
            }

            /* �������� */
            if (NULL == pst_physical_port->mcc_cfg_cb)
            {
                pst_physical_port->mcc_cfg_cb = (ATG_DCI_PHY_PORT_MCC *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_PHY_PORT_MCC),
                    NBB_NORETRY_ACT,
                    MEM_SPM_PHY_PORT_MCC_CB);
                
                if (NULL == pst_physical_port->mcc_cfg_cb)
                {
                    ret = SPM_LOGICAL_E_PARAM_NULL;    
                    spm_l2_record_err_code(ret);       
                    goto EXIT_LABEL;
                }
            }    
            
            OS_MEMCPY(pst_physical_port->mcc_cfg_cb, pst_tlv_cfg, sizeof(ATG_DCI_PHY_PORT_MCC));
            break;     
        case ATG_DCI_OPER_UPDATE:                       /*���£�������*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;        
        case ATG_DCI_OPER_DEL:                            /*ɾ��*/
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret);
            break;
        default:
            ret = SPM_LOGICAL_E_PARAM_UNAVAIL;
            spm_l2_record_err_code(ret); 
            break;
    }

    EXIT_LABEL: NBB_TRC_EXIT();
    return ret;       
}

/*****************************************************************************
�� �� ��  : spm_physical_port_alloc_cb
��������  : Ϊ�ӿ��������ýṹ�����ڴ�ռ�
�������  :   
�������  : ��
�� �� ֵ  : SPM_PHYSICAL_PORT_CB
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2015��8��8��
��    ��   : zhangzhm
�޸�����   : �����ɺ���

*****************************************************************************/
SPM_PHYSICAL_PORT_CB * spm_alloc_physical_port_cb(NBB_CXT_T NBB_CXT)
{
    SPM_PHYSICAL_PORT_CB *pst_physical_port = NULL;

    NBB_TRC_ENTRY("spm_alloc_physical_port_cb");

    /* ����һ���µĽӿ�����������Ŀ��*/
    pst_physical_port = (SPM_PHYSICAL_PORT_CB *)NBB_MM_ALLOC(sizeof(SPM_PHYSICAL_PORT_CB),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_PHYSICAL_PORT_CB);
    if (pst_physical_port == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ���ӿ�����������Ŀ */
    OS_MEMSET(pst_physical_port, 0, sizeof(SPM_PHYSICAL_PORT_CB));

    pst_physical_port->basic_cfg_cb       = NULL;
    pst_physical_port->phy_cfg_cb         = NULL;
    pst_physical_port->pos_link_cfg_cb    = NULL;
    pst_physical_port->stm1_sovhd_cfg_cb   = NULL;
    pst_physical_port->ces_cfg_cb         = NULL;
    pst_physical_port->cep_ovhd_cfg_cb    = NULL;
    pst_physical_port->efm_oam_cfg_cb     = NULL;
    pst_physical_port->time_sync_cfg_cb   = NULL;
    pst_physical_port->msg_suppr_cfg_cb   = NULL;
    pst_physical_port->flow_shape_cfg_cb  = NULL;
    pst_physical_port->schedule_policy_cfg_cb = NULL;
    pst_physical_port->congest_policy_cfg_cb  = NULL;
    pst_physical_port->ipv4_nf_cfg_cb     = NULL;
    pst_physical_port->ip_sample_cfg_cb = NULL;
    pst_physical_port->ipv6_nf_cfg_cb     = NULL;
    pst_physical_port->pove_cfg_cb = NULL;    
    pst_physical_port->common_cfg_cb      = NULL;
    pst_physical_port->stm1_tovhd_cfg_cb = NULL;    
    pst_physical_port->als_cfg_cb      = NULL;    
    pst_physical_port->thres_cfg_cb = NULL;
    pst_physical_port->mcc_cfg_cb = NULL;

    /* �������ڸýӿ�����������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pst_physical_port->spm_physical_port_handle = NBB_CREATE_HANDLE(pst_physical_port,
                                                                   HDL_SPM_PHYSICAL_PORT_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_PHYSICAL_PORT_CB allocated at %p with handle %#lx",
                   pst_physical_port, pst_physical_port->spm_physical_port_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_physical_port->spm_physical_port_node);    

    EXIT_LABEL : NBB_TRC_EXIT();

    return(pst_physical_port);
}


/*****************************************************************************
�� �� ��  : spm_physical_port_free_cb
��������  : �ͷŽӿ��������ýڵ���ڴ�ռ�
�������  : SPM_PHYSICAL_PORT_CB* pstPhysicalPort   
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2015��8��8��
��    ��   : zhangzhm
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_physical_port_cb(SPM_PHYSICAL_PORT_CB* pst_physical_port)
{
    NBB_TRC_ENTRY("spm_free_physical_port_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_physical_port, sizeof(SPM_PHYSICAL_PORT_CB), MEM_SPM_PHYSICAL_PORT_CB); 

    if (pst_physical_port == NULL)
    {
    	goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_PHYSICAL_PORT_CB at %p", pstPhysicalPort));  
    NBB_TRC_FLOW((NBB_FORMAT "Free PORT_INDEX %d", pst_physical_port->port_index_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (pst_physical_port->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->basic_cfg_cb, MEM_SPM_PHY_PORT_BASIC_CB);  
        pst_physical_port->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ���̫���ӿ������������ݿ顣                                          */
    /***************************************************************************/
    if (pst_physical_port->phy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->phy_cfg_cb, MEM_SPM_PHY_PORT_PHY_CB);  
        pst_physical_port->phy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�POS�ӿ���·���������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->pos_link_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->pos_link_cfg_cb, MEM_SPM_PHY_PORT_POS_CB);  
        pst_physical_port->pos_link_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�STM-1�ӿڿ����������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->stm1_sovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->stm1_sovhd_cfg_cb, MEM_SPM_PHY_PORT_SDH_SOVHD_CB);  
        pst_physical_port->stm1_sovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�CES�ӿ��������ݿ顣                                                 */
    /***************************************************************************/
    if (pst_physical_port->ces_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->ces_cfg_cb, MEM_SPM_PHY_PORT_CES_CB);  
        pst_physical_port->ces_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�CEP�ӿ��������ݿ顣                                                 */
    /***************************************************************************/
    if (pst_physical_port->cep_ovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->cep_ovhd_cfg_cb, MEM_SPM_PHY_PORT_CEP_CB);  
        pst_physical_port->cep_ovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ���̫��EFM OAM�������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->efm_oam_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->efm_oam_cfg_cb, MEM_SPM_PHY_PORT_EFM_OAM_CB);  
        pst_physical_port->efm_oam_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�ʱ��ͬ���˿��������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->time_sync_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->time_sync_cfg_cb, MEM_SPM_PHY_PORT_TIME_SYNC_CB);  
        pst_physical_port->time_sync_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ���̫���ӿڱ����������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->msg_suppr_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->msg_suppr_cfg_cb, MEM_SPM_PHY_PORT_MSG_SUPPR_CB);  
        pst_physical_port->msg_suppr_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ���̫���ӿ������������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->flow_shape_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->flow_shape_cfg_cb, MEM_SPM_PHY_PORT_FLOW_SHAPE_CB);  
        pst_physical_port->flow_shape_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ���̫�����ڶ��е��Ȳ������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->schedule_policy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->schedule_policy_cfg_cb, MEM_SPM_PHY_PORT_SCHEDUL_POLICY_CB);  
        pst_physical_port->schedule_policy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ���̫�����ڶ���ӵ���������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->congest_policy_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->congest_policy_cfg_cb, MEM_SPM_PHY_PORT_CONGEST_POLICY_CB);  
        pst_physical_port->congest_policy_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�IPV4������ʹ������IPV4 NetFlow���ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->ipv4_nf_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->ipv4_nf_cfg_cb, MEM_SPM_PHY_PORT_IPV4_NF_CB);  
        pst_physical_port->ipv4_nf_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�IPV4���������� IPV4 NetFlow sampler���ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->ip_sample_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->ip_sample_cfg_cb, MEM_SPM_PHY_PORT_IPV4_SAMPLE_CB);  
        pst_physical_port->ip_sample_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�IPV6������ʹ������IPV6 NetFlow���ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->ipv6_nf_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->ipv6_nf_cfg_cb, MEM_SPM_PHY_PORT_IPV6_NF_CB);  
        pst_physical_port->ipv6_nf_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�pove���ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->pove_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->pove_cfg_cb, MEM_SPM_PHY_PORT_POVE_CB);  
        pst_physical_port->pove_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷŽӿ�ͨ���������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->common_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->common_cfg_cb, MEM_SPM_PHY_PORT_COMMON_CB);  
        pst_physical_port->common_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�SDHͨ���������á�                                           */
    /***************************************************************************/
    if (pst_physical_port->stm1_tovhd_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->stm1_tovhd_cfg_cb, MEM_SPM_PHY_PORT_SDH_TOVHD_CB);  
        pst_physical_port->stm1_tovhd_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�ALS �������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->als_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->als_cfg_cb, MEM_SPM_PHY_PORT_ALS_CB);  
        pst_physical_port->als_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷŶ˿��������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->thres_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->thres_cfg_cb, MEM_SPM_PHY_PORT_THRES_CB);  
        pst_physical_port->thres_cfg_cb = NULL;
    }
    
     /***************************************************************************/
    /* �ͷ�MCC �������ݿ顣                                           */
    /***************************************************************************/
    if (pst_physical_port->mcc_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_physical_port->mcc_cfg_cb, MEM_SPM_PHY_PORT_MCC_CB);  
        pst_physical_port->mcc_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_physical_port->spm_physical_port_handle, HDL_SPM_PHYSICAL_PORT_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_physical_port, MEM_SPM_PHYSICAL_PORT_CB);  
    pst_physical_port = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_free_all_physical_port
   ��������  : �ͷŶ˿������������е��ڴ�ռ�
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��11��6��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/

NBB_VOID spm_free_all_physical_port(NBB_CXT_T NBB_CXT)
{
	SPM_PHYSICAL_PORT_CB *pstPhysicalPort = NULL;
	
    for (pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree);
         pstPhysicalPort != NULL;
         pstPhysicalPort = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree))
    {
    	AVLL_DELETE(SHARED.physical_port_tree, pstPhysicalPort->spm_physical_port_node);
    	spm_free_physical_port_cb(pstPhysicalPort NBB_CCXT);
    }
}

/*****************************************************************************
�� �� ��  : spm_physical_port_init_tree
��������  : ��ʼ���洢�ӿ��������õ���
�������  : ��
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2015��8��8��
��    ��   : zhangzhm
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_physical_port_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_physical_port_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_PHYSICAL_PORT_CB, port_index_key);

    AVLL_INIT_TREE(SHARED.physical_port_tree, compare_ulong,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_PHYSICAL_PORT_CB, spm_physical_port_node)); 

    NBB_TRC_EXIT();
}



    
/*****************************************************************************
�� �� ��  : spm_physical_port_proc
��������  : ����ӿ��ܴ������:����ips��Ϣ������ÿһ�������ÿ�
�������  : ��
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2015��8��8��
��    ��   : zhangzhm
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_physical_port_proc(ATG_DCI_SET_PHYSICAL_PORT *pst_set_physical_port)
{
    //to do :
    //1.ces��Ҫ�����ã����Ӻ�ɾ����ô����
    //2.�˿�����澯�Ĵ���û�мӽ���
    //3.log��ô��¼
    //�˿ڸ澯��תû�м�
    
    //NBB_CHAR print_message[SPM_MSG_INFO_LEN];
    
    /*��¼�����ô�ӡʱ��*/
    //struct timeval phy_tv_begin = {0};
    //struct timeval phy_tv_end = {0};
    //gettimeofday(&phy_tv_begin, NULL);

    /*��¼��ʼ��������ʱ��*/
    //OS_SPRINTF(print_message, "time_test phy_port_proc begin = %ld\n", phy_tv_begin.tv_usec);
    //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);    
    NBB_INT ret = SUCCESS; 
    NBB_BYTE if_exist = ATG_DCI_EXIST;
    NBB_ULONG port_index = 0;
    SPM_PHYSICAL_PORT_CB *pst_physical_port = NULL;
    NBB_BOOL avll_ret_code = FALSE;

     /* �����õĲ���ģʽ */
    NBB_ULONG oper_basic           = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_physical        = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_pos             = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_sdh_tovhd       = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_ces             = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_cep             = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_efm_oam         = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_time_sync       = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_msg_suppress    = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_flow_shape      = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_schedule_policy = ATG_DCI_OPER_NULL;  
    NBB_ULONG oper_congest_policy  = ATG_DCI_OPER_NULL;  
    NBB_ULONG oper_ipv4_nf         = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_ip_sample       = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_ipv6_nf         = ATG_DCI_OPER_NULL; 
    NBB_ULONG oper_pove            = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_thres           = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_common          = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_als             = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_sdh_sovhd       = ATG_DCI_OPER_NULL;
    NBB_ULONG oper_mcc             = ATG_DCI_OPER_NULL;
    
    NBB_TRC_ENTRY("spm_physical_port_proc");   
    
    if (NULL == pst_set_physical_port)
    {
        ret = PHY_PORT_PROC_IPS_NULL;    
        spm_l2_record_err_code(ret);       
        goto EXIT_LABEL;
    }

    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pst_set_physical_port->return_code = ATG_DCI_RC_OK;

    port_index = pst_set_physical_port->key;
    pst_physical_port = AVLL_FIND(SHARED.physical_port_tree, &port_index);

    /* ��Ǹ���Ŀ�Ƿ��Ѿ����� */
    if (NULL == pst_physical_port)
    {
        if_exist = ATG_DCI_UNEXIST;
    } 

    oper_basic           = pst_set_physical_port->oper_basic;
    oper_physical        = pst_set_physical_port->oper_physical;
    oper_pos             = pst_set_physical_port->oper_pos_link;    
    oper_sdh_sovhd       = pst_set_physical_port->oper_sdh_soverhead;
    oper_ces             = pst_set_physical_port->oper_ces;
    oper_cep             = pst_set_physical_port->oper_cep_overhead;
    oper_efm_oam         = pst_set_physical_port->oper_efm_oam;
    oper_time_sync       = pst_set_physical_port->oper_time_sync;
    oper_msg_suppress    = pst_set_physical_port->oper_msg_suppr;
    oper_flow_shape      = pst_set_physical_port->oper_flow_shape;
    oper_schedule_policy = pst_set_physical_port->oper_schedul_policy;
    oper_congest_policy  = pst_set_physical_port->oper_congest_policy;
    oper_ipv4_nf         = pst_set_physical_port->oper_ipv4nf;
    oper_ip_sample       = pst_set_physical_port->oper_ipnf_sampler;
    oper_ipv6_nf         = pst_set_physical_port->oper_ipv6nf;    
    oper_pove            = pst_set_physical_port->oper_pove; 
    oper_common          = pst_set_physical_port->oper_common;
    oper_sdh_tovhd       = pst_set_physical_port->oper_sdh_toverhead;   
    oper_als             = pst_set_physical_port->oper_als;    
    oper_thres           = pst_set_physical_port->oper_thres;
    oper_mcc             = pst_set_physical_port->oper_mcc;

    /* ɾ��������Ŀ */
    if (pst_set_physical_port->delete_struct == TRUE)
    {
        /* �����Ŀ�����ڣ���ɾ�� */
        if (ATG_DCI_UNEXIST == if_exist)
        {
            pst_set_physical_port->return_code = ATG_DCI_RC_OK;
            ret = PHY_PORT_PROC_DEL_NULL_LOCAL_CFG;    
            spm_l2_record_err_code(ret);       
            goto EXIT_LABEL;
        }              
        else/* ����ɾ�� */
        {
            spm_dbg_print_physical_port_head(port_index, SPM_OPER_DEL);
            spm_dbg_record_physical_port_head(port_index, SPM_OPER_DEL);
            
            //�������������ɾ������ӿ�����
            spm_physical_port_del_cfg(pst_physical_port NBB_CCXT);
            
            //to do ֪ͨ������ɾ������
            /**********�������۾�����ô����************************/
            
            if (NULL != pst_physical_port)
            {
                AVLL_DELETE(SHARED.physical_port_tree, pst_physical_port->spm_physical_port_node); 

                //�ͷŽӿ��������ýڵ���ڴ�ռ�
                spm_free_physical_port_cb(pst_physical_port);
            }
        }

        /*��¼ɾ�����ý���ʱ��*/
        //gettimeofday(&phy_tv_end, NULL);

        /*��¼��ʼ��������ʱ��*/
        //OS_SPRINTF(print_message, "time_test phy_port_proc delete end = %ld\n", phy_tv_end.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message); 
        //OS_SPRINTF(print_message, "time_test phy_interval = %ld\n", phy_tv_end.tv_usec - phy_tv_begin.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);
        
    }
    else  //���»�������
    {   
        if (ATG_DCI_UNEXIST == if_exist)
        {
            spm_dbg_print_physical_port_head(port_index, SPM_OPER_ADD);
            spm_dbg_record_physical_port_head(port_index, SPM_OPER_ADD);
            
            pst_physical_port = spm_alloc_physical_port_cb(NBB_CXT);

            if(NULL == pst_physical_port)
            {
                ret = PHY_PORT_PROC_ALLOC_CB_FAIL;    
                spm_l2_record_err_code(ret);       
                goto EXIT_LABEL;
            }

            if (NULL != pst_physical_port)
            {
                //�����������ֱ�Ӹ�keyֵ��Ȼ����뵽tree��
                pst_physical_port->port_index_key = port_index;
                avll_ret_code = AVLL_INSERT(SHARED.physical_port_tree, pst_physical_port->spm_physical_port_node);
                NBB_ASSERT(avll_ret_code);
            }
        }
        else
        {
            //���� 
            spm_dbg_print_physical_port_head(port_index, SPM_OPER_UPD);
            spm_dbg_record_physical_port_head(port_index, SPM_OPER_UPD);
            
        }
        
        /********************************1)�������� *********************************/
        if(ATG_DCI_OPER_NULL != oper_basic)
        {
            spm_physical_port_basic_proc(pst_set_physical_port,pst_physical_port,oper_basic);
        }
        
        /******************************2)��̫���ӿ��������� ***************************/
        if(ATG_DCI_OPER_NULL != oper_physical)
        {
            spm_physical_port_eth_phy_proc(pst_set_physical_port,pst_physical_port,oper_physical);
        }
        
        /********************************3)POS�ӿ���·������ *************************/
        if(ATG_DCI_OPER_NULL != oper_pos)
        {
            spm_physical_port_pos_proc(pst_set_physical_port,pst_physical_port,oper_pos);
        }
        
        /******************************** 19)SDHͨ���������� **************************/        
        if(ATG_DCI_OPER_NULL != oper_sdh_tovhd)
        {
            spm_physical_port_sdh_tovhd_proc(pst_set_physical_port,pst_physical_port,oper_sdh_tovhd);
        }
        
        /********************************5)CES�ӿ����� ******************************/
        if(ATG_DCI_OPER_NULL != oper_ces)
        {
            spm_physical_port_ces_proc(pst_set_physical_port,pst_physical_port,oper_ces);
        }
        
        /********************************6)CEP�ӿڿ������� ***************************/
        if(ATG_DCI_OPER_NULL != oper_cep)
        {
            spm_physical_port_cep_proc(pst_set_physical_port,pst_physical_port,oper_cep);
        }

        /********************************7)��̫��EFM OAM ****************************/
        if(ATG_DCI_OPER_NULL != oper_efm_oam)
        {
            spm_physical_port_efm_oam_proc(pst_set_physical_port,pst_physical_port,oper_efm_oam);
        }
        
        /********************************8)ʱ��ͬ���˿����� ****************************/
        if(ATG_DCI_OPER_NULL != oper_time_sync)
        {
            spm_physical_port_time_sync_proc(pst_set_physical_port,pst_physical_port,oper_time_sync);
        }
        
        /********************************9)��̫���ӿڱ������� ***************************/
        if(ATG_DCI_OPER_NULL != oper_msg_suppress)
        {
            spm_physical_port_msg_suppress_proc(pst_set_physical_port,pst_physical_port,oper_msg_suppress);
        }
        
        /********************************10)��̫���ӿ��������� **************************/
        if(ATG_DCI_OPER_NULL != oper_flow_shape)
        {
            spm_physical_port_flow_shape_proc(pst_set_physical_port,pst_physical_port,oper_flow_shape);
        }
        
        /********************************11)��̫�����ڶ��е��Ȳ��� ***********************/
        if(ATG_DCI_OPER_NULL != oper_schedule_policy)
        {
            spm_physical_port_schedule_policy_proc(pst_set_physical_port,pst_physical_port,
                oper_schedule_policy);
        }
        
        /********************************12)��̫�����ڶ���ӵ������ ************************/
        if(ATG_DCI_OPER_NULL != oper_congest_policy)
        {
            spm_physical_port_congest_policy_proc(pst_set_physical_port,pst_physical_port,oper_congest_policy);
        }
        
        /********************************13)IPV4������ʹ������IPV4 NetFlow ****************/
        if(ATG_DCI_OPER_NULL != oper_ipv4_nf)
        {
            spm_physical_port_ipv4_netflow_proc(pst_set_physical_port,pst_physical_port,oper_ipv4_nf);
        }
        
        /********************************14)IPV4���������� IPV4 NetFlow sampler ************/
        if(ATG_DCI_OPER_NULL != oper_ip_sample)
        {
            spm_physical_port_ip_nf_sample_proc(pst_set_physical_port,pst_physical_port,oper_ip_sample);
        }
        
        /********************************15)IPV6������ʹ������IPV6 NetFlow *****************/
        if(ATG_DCI_OPER_NULL != oper_ipv6_nf)
        {
            spm_physical_port_ipv6_netflow_proc(pst_set_physical_port,pst_physical_port,oper_ipv6_nf);
        }
        
        /********************************17)POVE���� ***********************************/
        if(ATG_DCI_OPER_NULL != oper_pove)
        {
            spm_physical_port_pve_proc(pst_set_physical_port,pst_physical_port,oper_pove);
        }
        
        /********************************21)�˿ڷ��ͽ��չ⹦������ֵ���� *********************/
        if(ATG_DCI_OPER_NULL != oper_thres)
        {
            spm_physical_port_thres_proc(pst_set_physical_port,pst_physical_port,oper_thres);
        }
        
        /********************************18)�ӿ�ͨ������ *********************************/
        if(ATG_DCI_OPER_NULL != oper_common)
        {
            spm_physical_port_common_proc(pst_set_physical_port,pst_physical_port,oper_common);
        }
        
        /********************************20)ALS���� ************************************/
        if(ATG_DCI_OPER_NULL != oper_als)
        {
            spm_physical_port_als_proc(pst_set_physical_port,pst_physical_port,oper_als);
        }
        
        /********************************4)SDH�ο������� ********************************/
        if(ATG_DCI_OPER_NULL != oper_sdh_sovhd)
        {
            spm_physical_port_sdh_sovhd_proc(pst_set_physical_port,pst_physical_port,oper_sdh_sovhd);
        }
        
        /********************************22)MCCͨ���������� *****************************/
        if(ATG_DCI_OPER_NULL != oper_mcc)
        {
            spm_physical_port_mcc_type_proc(pst_set_physical_port,pst_physical_port,oper_mcc);
        }

        /*��¼ɾ�����ý���ʱ��*/
        //gettimeofday(&phy_tv_end, NULL);

        /*��¼��ʼ��������ʱ��*/
        //OS_SPRINTF(print_message, "time_test phy_port_proc add end = %ld\n", phy_tv_end.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);
        //OS_SPRINTF(print_message, "time_test phy_interval = %ld\n", phy_tv_end.tv_usec - phy_tv_begin.tv_usec);
        //BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);
        
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}

#endif

