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

extern unsigned char msp_cfg_print_setting;

/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_msp
 ��������  : ����˿ھ���IPS��Ϣ
 �������  : ATG_DCI_SET_MSP *pstSetMsp  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��3��15��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_msp(ATG_DCI_SET_MSP *pstSetMsp NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
	NBB_BYTE ucC3Unit = 0;
    SPM_MSP_CB *pstMsp = NULL;
    ATG_DCI_MSP_BASIC *pstBasicData = NULL;
    
    NBB_BYTE *pucBasicDataStart = NULL;
    
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    
    NBB_ULONG ulMspKey = 0;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    //NBB_USHORT i = 0;

    APS_LMSP_PRO_GROUP pstlmsp;

    SPM_PORT_INFO_CB stPortInfo ;
    SPM_PORT_INFO_CB stBakPortInfo ;


    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));  
    OS_MEMSET(&stBakPortInfo, 0, sizeof(SPM_PORT_INFO_CB));  
    OS_MEMSET(&pstlmsp, 0, sizeof(APS_LMSP_PRO_GROUP));

    NBB_TRC_ENTRY("spm_rcv_dci_set_msp");

    /* �������ָ�������Ч */
    NBB_ASSERT(pstSetMsp != NULL);

	if (pstSetMsp == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_msp(pstSetMsp==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_msp(pstSetMsp==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_msp(pstSetMsp==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", 0, 
        		FUNC_IN_PARAM_IS_NULL, "FUNC_IN_PARAM_IS_NULL", ucMessage));
        
        goto EXIT_LABEL;
    }
    
    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pstSetMsp->return_code = ATG_DCI_RC_OK;

    ulMspKey = pstSetMsp->key;

    pstMsp = AVLL_FIND(SHARED.msp_tree, &ulMspKey);
    
    if (pstMsp == NULL)       /* �����Ŀ������*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    

    /* ��ȡ�����ò��� */
    ulOperBasic = pstSetMsp->oper_basic;

    
    /* ɾ��������Ŀ */
    if (pstSetMsp->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ɾ��ID=%ld�� MSP����Э��", ulMspKey));
        
        /* �����Ŀ�����ڣ���ɾ�� */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:Ҫɾ�������ò�������"));

            OS_PRINTF("***ERROR***:Ҫɾ����ID=%ld ��MSP����Э�����ò�����!\n", ulMspKey);

            OS_SPRINTF(ucMessage, "***ERROR***:Ҫɾ����ID=%ld ��MSP����Э�����ò�����!\n", ulMspKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, DEL_UNEXIST_CONFIG_ERROR, "DEL_UNEXIST_CONFIG_ERROR", ucMessage));
            
            pstSetMsp->return_code = ATG_DCI_RC_OK;
            
            goto EXIT_LABEL;
        }
        /* ���ڣ�ɾ�� */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ��ѯ�������ã���tree��ɾ��"));
            spm_dbg_print_msp_head(ulMspKey, SPM_OPER_DEL);
            spm_dbg_record_msp_head(ulMspKey, SPM_OPER_DEL);
            
#ifdef SPU
           delApsLmspProGroup(ulMspKey);
#endif    

#if 0

			if ((pstMsp != NULL) && (pstMsp->basic_cfg_cb != NULL))
			{
            	ret = spm_get_portid_from_logical_port_index(pstMsp->basic_cfg_cb->mirrored_port_index, 
                                                         &stMirroredPortInfo NBB_CCXT);
			}
			
    		if (ret != SUCCESS)
    		{
    		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

    		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

    		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

    		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_FUNC_ERROR", ucMessage));
    		    
    		    pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;

    		    goto EXIT_LABEL;
    		}

			if ((pstMsp != NULL) && (pstMsp->basic_cfg_cb != NULL))
			{
            	ret = spm_get_portid_from_logical_port_index(pstMsp->basic_cfg_cb->port_index, 
                                                         &stPortInfo NBB_CCXT);
			}
			
    		if (ret != SUCCESS)
    		{
    		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

    		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

    		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

    		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_FUNC_ERROR", ucMessage));
    		    
    		    pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;

    		    goto EXIT_LABEL;
    		}

            //ֻ�б�����˿��ڱ���λʱ�Ž���
            if (stMirroredPortInfo.slot_id == SHARED.local_slot_id)
            {
        		ret = ApiC3SetMirrorPort(stMirroredPortInfo.unit_id, 
        		                         stMirroredPortInfo.port_id, 
        		                         stPortInfo.slot_id,
        		                         stPortInfo.port_id, 
        		                         C3_PORT_MIRROR_DISABLE); 
                
                if (ret != SUCCESS)
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3SetMirrorPort() ret = %d", ret));

                    OS_PRINTF("***ERROR***:ApiC3SetMirrorPort() ret=%d\n", ret);

                    OS_SPRINTF(ucMessage, "***ERROR***:ApiC3SetMirrorPort() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_C3_FUNC_ERROR", ucMessage));
                    
                    pstSetMsp->return_code = ATG_DCI_RC_DEL_FAILED;
                    
                    goto EXIT_LABEL;
                }
            }

#endif
			if (pstMsp != NULL)
			{
	            AVLL_DELETE(SHARED.msp_tree, pstMsp->spm_msp_node);  

	            //�ͷ�MSP����Э��ڵ���ڴ�ռ�
	            spm_free_msp_cb(pstMsp NBB_CCXT);
			}
        }
    }
    /* ���ӻ������Ŀ */
    else
    {        
                
        /* �����Ŀ�����ڣ��������ڴ�ռ䱣������ */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ����ID=%d�� MSP����Э������", ulMspKey));
            spm_dbg_print_msp_head(ulMspKey, SPM_OPER_ADD);
            spm_dbg_record_msp_head(ulMspKey, SPM_OPER_ADD);

            /* ����һ������Ŀ���ڴ�ռ� */
            pstMsp = spm_alloc_msp_cb(NBB_CXT);

			if (pstMsp != NULL)
			{
	            //���뵽tree��
	            pstMsp->msp_key = ulMspKey;

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.msp_tree, pstMsp->spm_msp_node);
			}
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ����ID=%d�� MSP����Э������", ulMspKey));
            spm_dbg_print_msp_head(ulMspKey, SPM_OPER_UPD);
            spm_dbg_record_msp_head(ulMspKey, SPM_OPER_UPD);

        }

		//���Ӵ��жϣ�ȡ��PC-Lint�澯
        if (pstMsp == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:Ҫ���ӵ�ID=%ld ��MSP����Э������δ�ܳɹ������ڴ�!", ulMspKey));

            OS_PRINTF("***ERROR***:Ҫ���ӵ�ID=%ld ��MSP����Э������δ�ܳɹ������ڴ�!\n", ulMspKey);

            OS_SPRINTF(ucMessage, "***ERROR***:Ҫ���ӵ�ID=%ld ��MSP����Э������δ�ܳɹ������ڴ�!\n", ulMspKey);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));
            
            pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                        ��ȡ���ã���������                               */
        /***************************************************************************/
        /* �������� */
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* �����һ��entry�ĵ�ַ��*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetMsp, &pstSetMsp->basic_data);    

            /* ���ָ��ΪNULL�������� */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  MSP basic_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstBasicData = (ATG_DCI_MSP_BASIC*)pucBasicDataStart;

                if (msp_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1����������\n");
                    spm_dbg_print_msp_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1����������\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_msp_basic_cfg(pstBasicData);

                //��ӡ����
                NBB_TRC_DETAIL((NBB_FORMAT "  ��������=%d", pstBasicData->prot_type));
                NBB_TRC_DETAIL((NBB_FORMAT "  ��������˿�Index=%ld", pstBasicData->master_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  ��������˿�Index=%ld", pstBasicData->slave_port_index));
                NBB_TRC_DETAIL((NBB_FORMAT "  ����ʱ��=%d", pstBasicData->delay_time));
                NBB_TRC_DETAIL((NBB_FORMAT "  �ȴ��ָ�ʱ��=%ld", pstBasicData->restore_time));
                NBB_TRC_DETAIL((NBB_FORMAT "  ��������=%ld", pstBasicData->return_type));
                
                //����MSP�������ø�����ģ��
                //spm_set_aps_msp_prot_group(ulMspKey);
                
                pstlmsp.apsProType = pstBasicData->prot_type;

                pstlmsp.wPortIndex = pstBasicData->master_port_index;      
                spm_get_portid_from_physical_port_index(pstBasicData->master_port_index,&stPortInfo NBB_CCXT);
                pstlmsp.wSlot = stPortInfo.slot_id;
                pstlmsp.wCardId = stPortInfo.card_id;
                pstlmsp.wPortId = stPortInfo.port_id;

                pstlmsp.pPortIndex = pstBasicData->slave_port_index;
                spm_get_portid_from_physical_port_index(pstBasicData->slave_port_index,&stBakPortInfo NBB_CCXT);
                pstlmsp.pSlot = stBakPortInfo.slot_id;
                pstlmsp.pCardId = stBakPortInfo.card_id;
                pstlmsp.pPortId = stBakPortInfo.port_id;        

                pstlmsp.holdoffTimeExpire = pstBasicData->delay_time;
                pstlmsp.wtrTimeExpire = pstBasicData->restore_time;
                pstlmsp.revertMode = pstBasicData->return_type;
                
                ret = SUCCESS;
                ret = addApsLmspProGroup(ulMspKey,&pstlmsp);

                if (ERROR == ret )
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***addApsLmspProGroup ret=ERROR"));
                    OS_SPRINTF(ucMessage, "***ERROR***addApsLmspProGroup ret=ERROR \n");
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    goto EXIT_LABEL;    
                }

#ifdef SPU
	        	//msp 1+1
	        	if ((pstBasicData->prot_type == ATG_DCI_MSP_UNI_LINK_1J1_PROT)
	        	 || (pstBasicData->prot_type == ATG_DCI_MSP_BI_LINK_1J1_PROT))
				{
					for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
					{
	                	ApiC3SetFrrGroupProtType(ucC3Unit, SHARED.msp_frr_group_id[ulMspKey - 1], VP_NHI_1PLUS1);
	                }
				}
	        	//msp 1:1
	        	else if ((pstBasicData->prot_type == ATG_DCI_MSP_UNI_LINK_1B1_PROT)
	        	 	  || (pstBasicData->prot_type == ATG_DCI_MSP_BI_LINK_1B1_PROT))
	        	{
					for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
					{
	                	ApiC3SetFrrGroupProtType(ucC3Unit, SHARED.msp_frr_group_id[ulMspKey - 1], VP_NHI_FRR);
	                }
	        	}
	        	else
	        	{
					for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
					{
	                	ApiC3SetFrrGroupProtType(ucC3Unit, SHARED.msp_frr_group_id[ulMspKey - 1], VP_NHI_GROUP);
	                }
	        	}
#endif

#if 0
                ret = spm_get_portid_from_logical_port_index(pstBasicData->mirrored_port_index, 
                                                             &stMirroredPortInfo NBB_CCXT);

        		if (ret != SUCCESS)
        		{
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

        		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		    pstSetMsp->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

        		    goto EXIT_LABEL;
        		}

                ret = spm_get_portid_from_logical_port_index(pstBasicData->port_index, 
                                                             &stPortInfo NBB_CCXT);

        		if (ret != SUCCESS)
        		{
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_portid_from_logical_port_index ret=%d", ret));

        		    OS_PRINTF("***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:spm_get_portid_from_logical_port_index() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_FUNC_ERROR", ucMessage));
        		    
        		    pstSetMsp->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		    pstSetMsp->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

        		    goto EXIT_LABEL;
        		}

        		switch (pstBasicData->mirror_mode)
        		{
        		    case ATG_DCI_PORT_MIRROR_DISABLE:
        		        ucMirrorMode = C3_PORT_MIRROR_DISABLE;
        		        break;
        		    case ATG_DCI_PORT_MIRROR_INGRESS:
        		        ucMirrorMode = C3_PORT_MIRROR_INGRESS;
        		        break;
        		    case ATG_DCI_PORT_MIRROR_EGRESS:
        		        ucMirrorMode = C3_PORT_MIRROR_EGRESS;
        		        break;
        		    case ATG_DCI_PORT_MIRROR_BOTH:
        		        ucMirrorMode = C3_PORT_MIRROR_INGRESS | C3_PORT_MIRROR_EGRESS;
        		        break;
        		    default:
        		        ucMirrorMode = C3_PORT_MIRROR_DISABLE;
        		        break;
        		}

                //ֻ�б�����˿��ڱ���λʱ�Ž���
                if (stMirroredPortInfo.slot_id == SHARED.local_slot_id)
                {
            		ret = ApiC3SetMirrorPort(stMirroredPortInfo.unit_id, 
            		                         stMirroredPortInfo.port_id, 
            		                         stPortInfo.slot_id,
            		                         stPortInfo.port_id, 
            		                         ucMirrorMode);
            		
            		if (ret != SUCCESS)
            		{
            		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetMirrorPort ret=%d", ret));

            		    OS_PRINTF("***ERROR***:ApiC3SetMirrorPort() ret=%d\n", ret);

            		    OS_SPRINTF(ucMessage, "***ERROR***:ApiC3SetMirrorPort() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            		    NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", ulMspKey, ret, "CALL_C3_FUNC_ERROR", ucMessage));
            		    
            		    pstSetMsp->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

            		    goto EXIT_LABEL;
            		}
                }
#endif
                
                /* �������� */
                if (pstMsp->basic_cfg_cb == NULL)
                {
                    pstMsp->basic_cfg_cb = (ATG_DCI_MSP_BASIC*)NBB_MM_ALLOC(sizeof(ATG_DCI_MSP_BASIC),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_MSP_BASIC_CB);
                }  
                
                OS_MEMCPY(pstMsp->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_MSP_BASIC));
                
            }                 
        }
        else if (ulOperBasic == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperBasic == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }
    
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}

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
NBB_VOID spm_init_msp_tree(NBB_CXT_T NBB_CXT)
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
SPM_MSP_CB* spm_alloc_msp_cb(NBB_CXT_T NBB_CXT)
{
    SPM_MSP_CB *pstMsp = NULL;

    NBB_TRC_ENTRY("spm_alloc_msp_cb");

    /* ����һ���µ�MSP����������Ŀ��*/
    pstMsp = (SPM_MSP_CB *)NBB_MM_ALLOC(sizeof(SPM_MSP_CB), NBB_NORETRY_ACT, MEM_SPM_MSP_CB);
    if (pstMsp == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��MSP����������Ŀ */
    OS_MEMSET(pstMsp, 0, sizeof(SPM_MSP_CB));
    pstMsp->basic_cfg_cb = NULL;

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstMsp->spm_msp_handle = NBB_CREATE_HANDLE(pstMsp, HDL_SPM_MSP_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_MSP_CB allocated at %p with handle %#lx",
                   pstMsp, pstMsp->spm_msp_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstMsp->spm_msp_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstMsp);
}

/*****************************************************************************
 �� �� ��  : spm_free_msp_cb
 ��������  : �ͷ�MSP�����ṹ����ڴ�ռ�
 �������  : SPM_MSP_CB *pstMsp  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2014��4��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_msp_cb(SPM_MSP_CB *pstMsp NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_msp_cb");
    
    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstMsp, sizeof(SPM_MSP_CB), MEM_SPM_MSP_CB); 

    if (pstMsp == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_MSP_CB at %p", pstMsp));  
    NBB_TRC_FLOW((NBB_FORMAT "Free ID %d", pstMsp->msp_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (pstMsp->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstMsp->basic_cfg_cb, MEM_SPM_MSP_BASIC_CB);  
        pstMsp->basic_cfg_cb = NULL;
    }    

    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstMsp->spm_msp_handle, HDL_SPM_MSP_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstMsp, MEM_SPM_MSP_CB);  
    pstMsp = NULL;

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
NBB_VOID spm_free_all_msp_cb(NBB_CXT_T NBB_CXT)
{
	SPM_MSP_CB *pstMsp = NULL;
	
    for (pstMsp = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree);
         pstMsp != NULL;
         pstMsp = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree))
    {    	
		AVLL_DELETE(SHARED.msp_tree, pstMsp->spm_msp_node);     	
    	spm_free_msp_cb(pstMsp NBB_CCXT);
    }
}

