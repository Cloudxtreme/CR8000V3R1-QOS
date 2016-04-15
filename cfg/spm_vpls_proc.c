/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_vpls_proc.c
  �� �� ��   : ����
  ��    ��   : xiaoxiang
  ��������   : 2012��9��18��
  ����޸�   :
  ��������   : VPLS���ô���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��18��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef VPLS_CFG

/* ��ӡVPLS���ÿ��� */
extern unsigned char vpls_cfg_print_setting; 
extern unsigned char vpws_cfg_print_setting; /*vpws���ÿ�ӡ����*/
extern unsigned char g_vpls_cfg_print_setting;//print switch

/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_vpls
 ��������  : VPLS���ô���
 �������  : ATG_DCI_SET_VPLS *pstSetVpls        
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_vpls(ATG_DCI_SET_VPLS *pst_set_vpls)
{
    SPM_VPLS_CB *pst_vpls_cb = NULL;
    ATG_DCI_VPLS_BASIC_DATA *basic_data = NULL;
    NBB_BYTE *basic_data_start = NULL;        // IPS��Ϣƫ�Ƶ��׵�ַ
    NBB_ULONG oper_basic = ATG_DCI_OPER_NULL; //�����õĲ���ģʽ 
    NBB_BYTE if_exist = ATG_DCI_UNEXIST;
    NBB_BOOL avll_ret_code = FALSE;
    NBB_INT unit_id = 0;
    NBB_INT ret = SUCCESS;
    NBB_USHORT vpls_key = 0;                  //��Ŀ�ļ�ֵ 
    NBB_BYTE ucC3Unit = 0;
    NBB_USHORT usVsiMcId = 0;                 //�鲥ID
    NBB_ULONG ulMacLearnLimt = 0;
    NBB_BYTE ucC3Num = 0;
    NBB_CHAR print_message[SPM_MSG_INFO_LEN] = {0};

    NBB_TRC_ENTRY("spm_rcv_dci_set_vpls");
    

    if (NULL == pst_set_vpls)
	{      
	    SPM_L2_ERROR_PRINT_LOG(print_message,"vpls r1=%d\n",VPLS_CFG_NULL);
        goto EXIT_LABEL;
    }

    pst_set_vpls->return_code = ATG_DCI_RC_OK;
    pst_set_vpls->basic_return_code = ATG_DCI_RC_OK;
    
    vpls_key = pst_set_vpls->key;    
    pst_vpls_cb = AVLL_FIND(SHARED.vpls_tree, &vpls_key);

    //�жϸ���vpls�����Ƿ����
    if(NULL != pst_vpls_cb)
    {
        if_exist = ATG_DCI_EXIST; 
    }

    oper_basic = pst_set_vpls->oper_basic;

    /*ɾ��������Ŀ*/
    if(TRUE == pst_set_vpls->delete_struct)
    {
        if(if_exist == ATG_DCI_UNEXIST)
        {
            SPM_L2_ERROR_PRINT_LOG(print_message,"vpls r1=%d, vpls_key=%d\n",DEL_VPLS_CFG_NULL,
										vpls_key);
            goto EXIT_LABEL;
        }
        else
        {
            spm_dbg_print_vpls_head(vpls_key, SPM_OPER_DEL);
            spm_dbg_record_vpls_head(vpls_key, SPM_OPER_DEL);
            
            //�ص�IGMP   
            ret = spm_set_vsi_igmp_enable(pst_vpls_cb->vpls_id_key, ucC3Num, 0); 
            if(SUCCESS != ret)
            {
                //��¼������Ϣ
                SPM_L2_ERROR_PRINT_LOG(print_message,"vpls r1=%d,key=%d\n",ret,
                									pst_vpls_cb->vpls_id_key);
				pst_vpls_cb->total_ret_code = DEL_VPLS_IGMP_ERR;
                goto EXIT_LABEL;
            }    

              
           /*ֻɾ��vpls�Ļ������á�ԭ��:���ؿ������·�vpls��ɾ�������·�vc��ɾ����
           ��ʱ����ͷ�vpls�������ݣ��ᵼ��vc��ɾ����ʱ��������������ҵ��Ҳ���ɹ�*/

		    if(NULL != pst_vpls_cb->basic_cfg_cb)
	 	   	{
	 	   		NBB_MM_FREE(pst_vpls_cb->basic_cfg_cb, MEM_SPM_VPLS_BASIC_CB);  
	     		pst_vpls_cb->basic_cfg_cb = NULL;
	 	   	}
            
        }
    }
	
    /*���»򴴽�����Ŀ*/
    else    
    {   
        if(ATG_DCI_UNEXIST == if_exist)
        {
            spm_dbg_print_vpls_head(vpls_key, SPM_OPER_ADD);
            spm_dbg_record_vpls_head(vpls_key, SPM_OPER_ADD);
            pst_vpls_cb = spm_alloc_vpls_cb();
            if(NULL == pst_vpls_cb)
            {
                //��¼������Ϣ
                SPM_L2_ERROR_PRINT_LOG(print_message,"vpls r1=%d\n",ALLOC_VPLS_MEM_ERR);
				pst_vpls_cb->total_ret_code = ALLOC_VPLS_MEM_ERR;
                goto EXIT_LABEL;
            }

            if(NULL != pst_vpls_cb)
           {
                pst_vpls_cb->vpls_id_key = vpls_key;

                //�����뵽���ڴ���뵽����
                avll_ret_code = AVLL_INSERT(SHARED.vpls_tree, pst_vpls_cb->spm_vpls_node);
	            NBB_ASSERT(avll_ret_code);   
            }                             
        }
        else
        {
            spm_dbg_print_vpls_head(vpls_key, SPM_OPER_UPD);
            spm_dbg_record_vpls_head(vpls_key, SPM_OPER_UPD);
        }

        //�Ի������ÿ���д���
        if(ATG_DCI_OPER_NULL != oper_basic)
        {
            ret = spm_vpls_basic_proc(pst_set_vpls,pst_vpls_cb,oper_basic);
            if(SUCCESS != ret)
            {
                //��¼������Ϣ
                SPM_L2_ERROR_PRINT_LOG(print_message,"vpls r1=%d,key=%d\n",ret,pst_set_vpls->key);
				pst_vpls_cb->basic_success_flag = ret;
                goto EXIT_LABEL;
            }
        }
    }
   
    EXIT_LABEL : NBB_TRC_EXIT();
    return;
}

/*****************************************************************************
 �� �� ��  : spm_vpls_basic_proc
 ��������  : VPLS�Ļ������ô���
 �������  : ATG_DCI_SET_VPLS *pstSetVpls  SPM_VPLS_CB *pst_vpls_cb  NBB_ULONG oper_basic     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_basic_proc(ATG_DCI_SET_VPLS *pst_set_vpls, 
	SPM_VPLS_CB *pst_vpls_cb, 
	NBB_ULONG oper_basic)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *basic_data_start = NULL;
    ATG_DCI_VPLS_BASIC_DATA *basic_data = NULL;
    NBB_CHAR print_message[SPM_MSG_INFO_LEN] = {0};
	NBB_ULONG ulMacLearnLimt = 0;
	NBB_BYTE ucC3Num = 0;

    NBB_TRC_ENTRY("spm_vpls_basic_proc");
    
    if((NULL == pst_set_vpls) || (NULL == pst_vpls_cb))
    {
        //��¼������Ϣ
        ret = JUDGE_VPLS_BASIC_PROC_POINT;
        goto EXIT_LABEL;
    }

    switch(oper_basic)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
			break;

		case ATG_DCI_OPER_ADD:
            
            //���������׵�ַ
            basic_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vpls, 
                                &pst_set_vpls->basic_data);  
            if(NULL == basic_data_start)
            {
                //��¼������Ϣ
                ret = VPLS_BASIC_CFG_NULL;
                goto EXIT_LABEL;    
            }
            
            basic_data = (ATG_DCI_VPLS_BASIC_DATA *)basic_data_start;
            if (g_vpls_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  1����������\n");
                spm_dbg_print_vpls_basic_cfg(basic_data);
            }

            OS_SPRINTF(print_message,"  1����������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, print_message);
            spm_dbg_record_vpls_basic_cfg(basic_data);
            
            if(NULL == pst_vpls_cb->basic_cfg_cb)
            {
                pst_vpls_cb->basic_cfg_cb = (ATG_DCI_VPLS_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_BASIC_DATA),
                                                NBB_NORETRY_ACT,MEM_SPM_VPLS_BASIC_CB);

                if(NULL == pst_vpls_cb->basic_cfg_cb)
                {
                    //��¼������Ϣ
                    ret = ALLOC_VPLS_BASIC_MEM_ERR;
                    goto EXIT_LABEL;
                }
            }

            if(NULL != pst_vpls_cb->basic_cfg_cb)
            {
                NBB_MEMCPY(pst_vpls_cb->basic_cfg_cb, basic_data, sizeof(ATG_DCI_VPLS_BASIC_DATA));
            }

            
            ulMacLearnLimt = pst_vpls_cb->basic_cfg_cb->mac_learning_capacity;

            /*MAC��ַ����ѧϰ��ֻ��ҵ����*/
            ret = spm_vpls_mac_learning(pst_vpls_cb->vpls_id_key,ulMacLearnLimt);           
            if(SUCCESS != ret)
            {
                ret = SPU_CLEARN_MAC_ERR;
                goto EXIT_LABEL;    
            }

            /*ʹ��IGMP*/
            ucC3Num = SHARED.c3_num;
            ret = spm_set_vsi_igmp_enable(pst_vpls_cb->vpls_id_key, ucC3Num, 1);           
            if(SUCCESS != ret)
            {
                ret = BASIC_FLAG_ENABLE_IGMP_FAILED;//�˴��Ĵ���ֵ�������������־����������
                goto EXIT_LABEL;    
            }
            
            break;

        default:
            break;
    }

    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;

}



/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_vpls
 ��������  : VPLS���ô���
 �������  : ATG_DCI_SET_VPLS *pstSetVpls        
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_get_vpls_slot_num(SPM_VPLS_CB *pstVpls, NBB_BYTE ucSlot NBB_CCXT_T NBB_CXT)
{
    NBB_INT i = 0;
    NBB_INT iNum = 0;
    NBB_ULONG ulPortIndex = 0;
    NBB_INT ret = SUCCESS;
    SPM_PORT_INFO_CB stPortInfo;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_INT j = 0;   
    SPM_VPLS_NNI_CB *pst_vpls_nni_cb = NULL;
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;
    ATG_DCI_VC_KEY vc_key;
    SPM_VC_CB *pst_vc_cb = NULL;
    SPM_LOGICAL_PORT_CB *logical_port_cb = NULL;
    
    NBB_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    NBB_MEMSET(&vc_key, 0, sizeof(ATG_DCI_VC_KEY));

    if(pstVpls == NULL)
    {
        ret = ERROR;
        goto EXIT_LABEL;
    }
    
    for(pst_vpls_nni_cb = AVLL_FIRST(pstVpls->nni_cfg_tree);
                  NULL != pst_vpls_nni_cb;
        pst_vpls_nni_cb = AVLL_NEXT(pstVpls->nni_cfg_tree, pst_vpls_nni_cb->spm_vpls_nni_node))
    {
        NBB_MEMCPY(&vc_key, &(pst_vpls_nni_cb->vckey_main),sizeof(ATG_DCI_VC_KEY));
        pst_vc_cb = AVLL_FIND(SHARED.vc_tree, &vc_key);
        
        if(NULL == pst_vc_cb)
        {
            continue;       
        }

        if(ucSlot == pst_vc_cb->vc_info_cb.slot_id)
        {
            iNum++;    
        }
        
    }


   for(pst_vpls_uni_cb = AVLL_FIRST(pstVpls->uni_cfg_tree);
                  NULL != pst_vpls_uni_cb;
        pst_vpls_uni_cb = AVLL_NEXT(pstVpls->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node))
    {
        logical_port_cb = (SPM_LOGICAL_PORT_CB *)AVLL_FIND(SHARED.logical_port_tree, &(pst_vpls_uni_cb->port_index));
        if(NULL == logical_port_cb)
        {
             continue;   
        }
 
        if(ucSlot == logical_port_cb->slot_id)
        {
            iNum++;    
        }
        
    }

/*        
    
    //NNI���ڴ˲�λ�Ķ˿ڸ���
    for (i = 0; i < ATG_DCI_VPLS_NNI_NUM; i++)
    {
    	if ((pstVpls->nni_cfg_cb[i] != NULL) && (pstVpls->nni_info_cb[i].port_index != 0))
		{
			ulPortIndex = pstVpls->nni_info_cb[i].port_index;

			//��ȡ���ӿ�
			OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
		    ret = spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);
		    if (ret != SUCCESS)
		    {
		        //��¼������Ϣ
		        spm_l2_record_err_code_and_two_value(VPLS_NNI_GET_LOG_PORT_ERR,ret,ulPortIndex);
				continue;
			}

			if (stPortInfo.port_type == ATG_DCI_LAG)
			{
				if (stPortInfo.lag_slot_id == ucSlot)
					iNum++;
			}
			else
			{
				if (stPortInfo.slot_id == ucSlot)
					iNum++;
			}
		}
    	
    }

    //NNI���ڴ˲�λ�Ķ˿ڸ���ecmp
    for (i = 0; i < ATG_DCI_VPLS_NNI_NUM; i++)
    {
        if (pstVpls->nni_cfg_cb[i] != NULL) 
		{			
            for(j = 0;j < pstVpls->nni_info_cb[i].port_num;j++)
            {
                if (pstVpls->nni_info_cb[i].slot_id_mc[j] == ucSlot)
                {        				            
                	iNum++;                                   
                }
            }
		}    	
    }

    //UNI���ڴ˲�λ�Ķ˿ڸ���
    for (i = 0; i < ATG_DCI_VPLS_UNI_NUM; i++)
    {
    	if ((pstVpls->uni_cfg_cb[i] != NULL) && (pstVpls->uni_cfg_cb[i]->port_index != 0))
		{
			ulPortIndex = pstVpls->uni_cfg_cb[i]->port_index;

			//��ȡ���ӿ�
			OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
		    ret = spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);

		    if (ret != SUCCESS)
		    {
                //��¼������Ϣ
                spm_l2_record_err_code_and_two_value(VPLS_UNI_GET_LOG_PORT_ERR,ret,ulPortIndex);
				continue;
			}

			if (stPortInfo.port_type == ATG_DCI_LAG)
			{
				if (stPortInfo.lag_slot_id == ucSlot)
					iNum++;
			}
			else
			{
				if (stPortInfo.slot_id == ucSlot)
					iNum++;
			}
		}

    }*/
EXIT_LABEL :
    return iNum;
}


/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_vpls
 ��������  : VPLS���ô���
 �������  : ATG_DCI_SET_VPLS *pstSetVpls        
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_refresh_arad_vp(NBB_USHORT usVplsId, 
										  NBB_USHORT usMcId, 
										  NBB_ULONG ulPortIndexOld, 
										  NBB_ULONG ulPortIndexNew
										  NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR print_message[SPM_MSG_INFO_LEN] = {0};
	NBB_INT ret = SUCCESS;
	NBB_INT i = 0;
	NBB_INT iMcMemberNum = 0;
	NBB_ULONG ulPortIndex = 0;
	NBB_INT iDevSlotNum = 0;
	NBB_INT iSlotNum = 0;
	NBB_INT iSlotGroup[SPM_SLOT_MAX_NUM] = {0};
	SPM_VPLS_CB *pstVpls = NULL;
	SPM_PORT_INFO_CB stPortInfo;
	ARAD_MC_INFO stMcInfo;
    NBB_INT j = 0;
    SPM_VPLS_NNI_CB *pst_vpls_nni_cb = NULL;
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;
    ATG_DCI_VC_KEY vc_key;
    SPM_VC_CB *pst_vc_cb = NULL;
       
       //SPM_PORT_INFO_CB stPortInfoall[ATG_DCI_ECMP_PORT_NUM];

	OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	OS_MEMSET(&stMcInfo, 0, sizeof(ARAD_MC_INFO));
    OS_MEMSET(&vc_key, 0, sizeof(ATG_DCI_VC_KEY));

	//usMcIdΪ0���˳�����
	if ((usMcId == 0) || (usVplsId == 0))
	{
        ret = VPLS_REFRESH_ARAD_VP_INPUT_ERR;
        goto EXIT_LABEL;
	}

	//��VPLS����
	pstVpls = AVLL_FIND(SHARED.vpls_tree, &usVplsId);

	if (pstVpls == NULL)
	{ 
        ret = VPLS_REFRESH_ARAD_VPLS_CB_NULL;            
        goto EXIT_LABEL;
    }

	//�ҵ�NNI�����ڱ���λ��VP
    for(pst_vpls_nni_cb = AVLL_FIRST(pstVpls->nni_cfg_tree);
                  NULL != pst_vpls_nni_cb;
        pst_vpls_nni_cb = AVLL_NEXT(pstVpls->nni_cfg_tree, pst_vpls_nni_cb->spm_vpls_nni_node))
    {
        NBB_MEMCPY(&vc_key, &(pst_vpls_nni_cb->vckey_main),sizeof(ATG_DCI_VC_KEY));
        pst_vc_cb = AVLL_FIND(SHARED.vc_tree, &vc_key);
        if(NULL == pst_vc_cb)
        {
            continue;   
        }
        ulPortIndex = pst_vc_cb->vc_info_cb.port_index;
        OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
		ret = spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);
        if (ret != SUCCESS)
	    {
            
			continue;
		}

        if(stPortInfo.port_type == ATG_DCI_LAG)
        {
            stMcInfo.mc_member_array[iMcMemberNum].slot = pst_vc_cb->vc_info_cb.slot_id;
    		stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
    		stMcInfo.mc_member_array[iMcMemberNum].cud = pst_vc_cb->vc_info_cb.vp_idx;
            iMcMemberNum++;  
        }
        else
        {
            stMcInfo.mc_member_array[iMcMemberNum].slot = stPortInfo.slot_id;
		    stMcInfo.mc_member_array[iMcMemberNum].cud = pst_vc_cb->vc_info_cb.vp_idx;

			if (stPortInfo.unit_id == UNIT_0)
			{
				stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
			}
			else
			{
				stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_1;
			}
			
			iMcMemberNum++;    
        }
        
    }
    
    /*for (i = 0; i < ATG_DCI_VPLS_NNI_NUM; i++)
    {
    	if ((pstVpls->nni_cfg_cb[i] != NULL) && (pstVpls->nni_info_cb[i].port_index != 0))
    	{
    		ulPortIndex = pstVpls->nni_info_cb[i].port_index;
    		
			//��ȡ���ӿ�
			OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
		    ret = spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);

		    if (ret != SUCCESS)
		    {
                spm_l2_record_err_code_and_two_value(VPLS_REFRESH_ARAD_NNI_GET_LOG_PORT_ERR, ret, ulPortIndex);
				continue;
			}

			if (stPortInfo.port_type == ATG_DCI_LAG)
			{
		        stMcInfo.mc_member_array[iMcMemberNum].slot = stPortInfo.lag_slot_id;
				stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
				stMcInfo.mc_member_array[iMcMemberNum].cud = pstVpls->nni_info_cb[i].vp_idx;
				iMcMemberNum++;
			}
			else
			{
				stMcInfo.mc_member_array[iMcMemberNum].slot = stPortInfo.slot_id;
				stMcInfo.mc_member_array[iMcMemberNum].cud = pstVpls->nni_info_cb[i].vp_idx;

				if (stPortInfo.unit_id == UNIT_0)
				{
					stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
				}
				else
				{
					stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_1;
				}
				
				iMcMemberNum++;
			}
    	}
    }*/

    //�ҵ�NNI�����ڱ���λ��VP
    /*for (i = 0; i < ATG_DCI_VPLS_NNI_NUM; i++)
    {
    	if (pstVpls->nni_cfg_cb[i] != NULL) 
        {
            for(j = 0;j < pstVpls->nni_info_cb[i].port_num;j++)
            {
                if (SHARED.local_slot_id == pstVpls->nni_info_cb[i].slot_id_mc[j])
                {
                    if (pstVpls->nni_info_cb[i].port_type_mc[j] == ATG_DCI_LAG)
                    {
                    
                		stMcInfo.mc_member_array[iMcMemberNum].slot = pstVpls->nni_info_cb[i].slot_id_mc[j];
                		stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
                		stMcInfo.mc_member_array[iMcMemberNum].cud = pstVpls->nni_info_cb[i].vp_idx;
                		iMcMemberNum++;
                    }
                    else
                    {
                        stMcInfo.mc_member_array[iMcMemberNum].slot = pstVpls->nni_info_cb[i].slot_id_mc[j];
                        stMcInfo.mc_member_array[iMcMemberNum].cud = pstVpls->nni_info_cb[i].vp_idx;

                        if (stPortInfo.unit_id == UNIT_0)
                        {
                        	stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
                        }
                        else
                        {
                        	stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_1;
                        }
                        iMcMemberNum++;
                    }
                }                
            }
        }
    }*/

    //�ҵ�UNI�����ڱ���λ��VP
    for(pst_vpls_uni_cb = AVLL_FIRST(pstVpls->uni_cfg_tree);
                  NULL != pst_vpls_uni_cb;
        pst_vpls_uni_cb = AVLL_NEXT(pstVpls->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node))
    {
        ulPortIndex = pst_vpls_uni_cb->port_index;
    		
		//��ȡ���ӿ�
		OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
	    ret = spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);
        if(ret != SUCCESS)
        {
            continue;
        }

        if (stPortInfo.port_type == ATG_DCI_LAG)
		{
			stMcInfo.mc_member_array[iMcMemberNum].slot = stPortInfo.lag_slot_id;
			stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
			
			//stMcInfo.mc_member_array[iMcMemberNum].cud = pstVpls->uni_info_cb[i].vp_idx;//�����ɲ�д�ú��ȡ
			iMcMemberNum++;
		}
		else
		{
			stMcInfo.mc_member_array[iMcMemberNum].slot = stPortInfo.slot_id;	
			
			//stMcInfo.mc_member_array[iMcMemberNum].cud = pstVpls->uni_info_cb[i].vp_idx;

			if (stPortInfo.unit_id == UNIT_0)
			{
				stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
			}
			else
			{
				stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_1;
			}
			
			iMcMemberNum++;
		}
    }

	//�ҵ�UNI�����ڱ���λ��VP
    /*for (i = 0; i < ATG_DCI_VPLS_UNI_NUM; i++)
    {
    	if ((pstVpls->uni_cfg_cb[i] != NULL) && (pstVpls->uni_cfg_cb[i]->port_index != 0))
    	{
    		ulPortIndex = pstVpls->uni_cfg_cb[i]->port_index;
    		
			//��ȡ���ӿ�
			OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
		    ret = spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);

		    if (ret != SUCCESS)
		    {
				//spm_l2_record_err_code_and_two_value(VPLS_REFRESH_ARAD_UNI_GET_LOG_PORT_ERR, ret, ulPortIndex);
				continue;
			}

			if (stPortInfo.port_type == ATG_DCI_LAG)
			{
				stMcInfo.mc_member_array[iMcMemberNum].slot = stPortInfo.lag_slot_id;
				stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
				stMcInfo.mc_member_array[iMcMemberNum].cud = pstVpls->uni_info_cb[i].vp_idx;
				iMcMemberNum++;
			}
			else
			{
				stMcInfo.mc_member_array[iMcMemberNum].slot = stPortInfo.slot_id;					
				stMcInfo.mc_member_array[iMcMemberNum].cud = pstVpls->uni_info_cb[i].vp_idx;

				if (stPortInfo.unit_id == UNIT_0)
				{
					stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_0;
				}
				else
				{
					stMcInfo.mc_member_array[iMcMemberNum].port = SPM_MULTICAST_PORT_1;
				}
				
				iMcMemberNum++;
			}
    	}
    }*/

    ret = spm_vpls_arad_update_egress_mc(&stMcInfo, iMcMemberNum, usMcId);

    if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(print_message,"vpls update egress mc error r1=%d, iMcMemberNum=%d,usMcId=%d\n",
                                ret,iMcMemberNum, usMcId);
        ret = VPLS_REFRESH_ARAD_UPDATE_MC_ERR;
        goto EXIT_LABEL;
    }
        

    ret = spm_vpls_dfe_mc_set(pstVpls, usMcId);
    if(SUCCESS != ret)
    {         
        SPM_L2_ERROR_PRINT_LOG(print_message,"vpls dfe_mc_set error r1=%d, iMcMemberNum=%d,vplskey=%d\n",
                                ret, iMcMemberNum, pstVpls->vpls_id_key);
        ret = VPLS_REFRESH_ARAD_SET_MC_ERR;
        goto EXIT_LABEL;
    }
    

EXIT_LABEL :

	return ret;
}


/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_vpls
 ��������  : VPLS���ô���
 �������  : ATG_DCI_SET_VPLS *pstSetVpls        
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : 
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_get_vpls_portindex_by_vp(SPM_VPLS_CB *pst_vpls_cb, NBB_ULONG vp_id, 
    NBB_ULONG *port_index)
{
    NBB_INT ret = SUCCESS;
    SPM_VPLS_NNI_CB *pst_vpls_nni_cb = NULL;
    ATG_DCI_VC_KEY vc_key;
    SPM_VC_CB *pst_vc_cb = NULL;
    NBB_CHAR print_message[SPM_MSG_INFO_LEN] = {0};
    
    NBB_TRC_ENTRY("spm_get_vpls_portindex_by_vp");

    NBB_MEMSET(&vc_key, 0, sizeof(ATG_DCI_VC_KEY));
        
    if((NULL == pst_vpls_cb) || (NULL == port_index))
    {
        ret = GET_VPLS_PORTINDEX_INPUT_ERR;
        SPM_L2_ERROR_PRINT_LOG(print_message,"r1=%d, vp_id=%d\n",GET_VPLS_PORTINDEX_INPUT_ERR,vp_id);
        goto EXIT_LABEL;
    }

    pst_vpls_nni_cb =  (SPM_VPLS_NNI_CB *)AVLL_FIND(pst_vpls_cb->nni_cfg_tree,&vp_id);
    if(NULL == pst_vpls_nni_cb)
    {
        ret = GET_VPLS_PORTINDEX_NNI_CB_NULL;
        SPM_L2_ERROR_PRINT_LOG(print_message,"find nni cb NULL r1=%d, vp_id=%d\n",
                                GET_VPLS_PORTINDEX_NNI_CB_NULL,vp_id);
        goto EXIT_LABEL;
    }
    
    NBB_MEMCPY(&vc_key, &(pst_vpls_nni_cb->vckey_main),sizeof(ATG_DCI_VC_KEY));

    pst_vc_cb = AVLL_FIND(SHARED.vc_tree, &vc_key);
    if(NULL == pst_vc_cb)
    {
        ret = GET_VPLS_PORTINDEX_VC_CB_NULL;
        SPM_L2_ERROR_PRINT_LOG(print_message,"find VC cb NULL r1=%d,vc_id=%d,vc_type=%d,perr_ip=%d\n",
                              GET_VPLS_PORTINDEX_VC_CB_NULL,vc_key.vc_id, vc_key.vc_type, vc_key.peer_ip);
        goto EXIT_LABEL;
    }

    *port_index = pst_vc_cb->vc_info_cb.port_index;
    
    EXIT_LABEL : NBB_TRC_EXIT();
    return ret;

}



/*****************************************************************************
 �� �� ��  : spm_init_vpls_tree
 ��������  : ��ʼ���洢VPLS���õ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��24��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_vpls_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vpls_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_VPLS_CB, vpls_id_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.vpls_tree, compare_ushort,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_VPLS_CB, spm_vpls_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 �� �� ��  : spm_alloc_vpls_nni_cb
 ��������  : ����VPLS��ĿNNI���õ��ڴ�ռ�
 �������  : NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : SPM_VPLS_NNI_CB
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��23��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_VPLS_NNI_CB * spm_alloc_vpls_nni_cb(NBB_CXT_T NBB_CXT)
{
    SPM_VPLS_NNI_CB *pst_vpls_nni_cb = NULL;

    NBB_TRC_ENTRY("spm_alloc_vpls_nni_cb");

    /* ����һ���µ�VPLS_NNI������Ŀ��*/
    pst_vpls_nni_cb = (SPM_VPLS_NNI_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_NNI_CB), 
                    NBB_NORETRY_ACT, MEM_SPM_VPLS_NNI_CB);
    if (pst_vpls_nni_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPLS_NNI������Ŀ */
    OS_MEMSET(pst_vpls_nni_cb, 0, sizeof(SPM_VPLS_NNI_CB));

    /* �������ڸ�VPLS_NNI������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pst_vpls_nni_cb->spm_vpls_nni_handle = NBB_CREATE_HANDLE(pst_vpls_nni_cb, HDL_SPM_VPLS_NNI_CB);

    /* �ɹ�����һ���µ�VPLS_NNI������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_NNI_CB allocated at %p with handle %#lx",
                   pst_vpls_nni_cb, pst_vpls_nni_cb->spm_vpls_nni_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_vpls_nni_cb->spm_vpls_nni_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pst_vpls_nni_cb);
}

/*****************************************************************************
 �� �� ��  : spm_alloc_vpls_uni_cb
 ��������  : ����VPLS��ĿUNI���õ��ڴ�ռ�
 �������  : NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : SPM_VPLS_UNI_CB
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��23��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_VPLS_UNI_CB * spm_alloc_vpls_uni_cb(NBB_CXT_T NBB_CXT)
{
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;

    NBB_TRC_ENTRY("__FUNCTION__");

    /* ����һ���µ�VPLS_NNI������Ŀ��*/
    pst_vpls_uni_cb = (SPM_VPLS_UNI_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_UNI_CB), 
                    NBB_NORETRY_ACT, MEM_SPM_VPLS_UNI_CB);
    if (pst_vpls_uni_cb == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPLS_UNI������Ŀ */
    OS_MEMSET(pst_vpls_uni_cb, 0, sizeof(SPM_VPLS_UNI_CB));

    /* �������ڸ�VPLS_UNI������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pst_vpls_uni_cb->spm_vpls_uni_handle = NBB_CREATE_HANDLE(pst_vpls_uni_cb, HDL_SPM_VPLS_UNI_CB);

    /* �ɹ�����һ���µ�VPLS_UNI������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_UNI_CB allocated at %p with handle %#lx",
                   pst_vpls_uni_cb, pst_vpls_uni_cb->spm_vpls_uni_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_vpls_uni_cb->spm_vpls_uni_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pst_vpls_uni_cb);
}

/*****************************************************************************
 �� �� ��  : spm_alloc_vpls_cb
 ��������  : ����VPLS��Ŀ���ڴ�ռ�
 �������  : NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : SPM_VPLS_CB
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��10��30��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_VPLS_CB * spm_alloc_vpls_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_VPLS_CB *pstVpls = NULL;

    NBB_TRC_ENTRY("spm_alloc_vpls_cb");

    /* ����һ���µ�VPWS������Ŀ��*/
    pstVpls = (SPM_VPLS_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_CB), NBB_NORETRY_ACT, MEM_SPM_VPLS_CB);
    if (pstVpls == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPWS������Ŀ */
    OS_MEMSET(pstVpls, 0, sizeof(SPM_VPLS_CB));
    pstVpls->basic_cfg_cb = NULL;

    //��ʼ��NNI������
    AVLL_INIT_TREE(pstVpls->nni_cfg_tree, compare_ushort, 
        (NBB_USHORT)NBB_OFFSETOF(SPM_VPLS_NNI_CB, nni_no),
        (NBB_USHORT)NBB_OFFSETOF(SPM_VPLS_NNI_CB, spm_vpls_nni_node));

    //��ʼ��UNI������
    AVLL_INIT_TREE(pstVpls->uni_cfg_tree, compare_ushort, 
        (NBB_USHORT)NBB_OFFSETOF(SPM_VPLS_UNI_CB, uni_no),
        (NBB_USHORT)NBB_OFFSETOF(SPM_VPLS_UNI_CB, spm_vpls_uni_node));

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstVpls->spm_vpls_handle = NBB_CREATE_HANDLE(pstVpls, HDL_SPM_VPLS_CB);

    /* �ɹ�����һ���µ�VPLS������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_CB allocated at %p with handle %#lx",
                   pstVpls, pstVpls->spm_vpls_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstVpls->spm_vpls_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstVpls);
}


/*****************************************************************************
 �� �� ��  : spm_free_vpls_nni_cb
 ��������  : �ͷ�VPLS��ĿNNI���õ��ڴ�ռ�
 �������  : NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��23��
    ��    ��   : huxiaolu
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_vpls_nni_cb(SPM_VPLS_NNI_CB * pst_vpls_nni_cb NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("__FUNCTION__");

    if (NULL == pst_vpls_nni_cb)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_vpls_nni_cb, sizeof(SPM_VPLS_NNI_CB), MEM_SPM_VPLS_NNI_CB); 

    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_vpls_nni_cb->spm_vpls_nni_handle, HDL_SPM_VPLS_NNI_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_vpls_nni_cb, MEM_SPM_VPLS_NNI_CB);  
    pst_vpls_nni_cb = NULL;   

    EXIT_LABEL : 
    NBB_TRC_EXIT();
    
    return;
}


/*****************************************************************************
 �� �� ��  : spm_free_vpls_Uni_cb
 ��������  : �ͷ�VPLS��ĿUNI���õ��ڴ�ռ�
 �������  : NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : ��
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��23��
    ��    ��   : zhangsen
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_vpls_uni_cb(SPM_VPLS_UNI_CB *pst_vpls_uni_cb)
{
    NBB_TRC_ENTRY("__FUNCTION__");

    if (NULL == pst_vpls_uni_cb)
    {
        goto EXIT_LABEL;
    }

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_vpls_uni_cb, sizeof(SPM_VPLS_UNI_CB), MEM_SPM_VPLS_UNI_CB); 
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_vpls_uni_cb->spm_vpls_uni_handle, HDL_SPM_VPLS_UNI_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_vpls_uni_cb, MEM_SPM_VPLS_UNI_CB);  
    pst_vpls_uni_cb = NULL;   

    EXIT_LABEL : 
    NBB_TRC_EXIT();
    
    return;
}

/*****************************************************************************
 �� �� ��  : spm_free_vpls_cb
 ��������  : �ͷ�VPLS��Ŀ���ڴ�ռ�
 �������  : NBB_CCXT_T NBB_CXT  
 �������  : ��
 �� �� ֵ  : SPM_VPLS_CB
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��23��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_vpls_cb(SPM_VPLS_CB *pst_vpls_cb NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    SPM_VPLS_NNI_CB *pst_vpls_nni_cb = NULL;
    SPM_VPLS_UNI_CB *pst_vpls_uni_cb = NULL;
    
    NBB_TRC_ENTRY("spm_free_vpls_cb");
    
    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pst_vpls_cb, sizeof(SPM_VPLS_CB), MEM_SPM_VPLS_CB); 

    if (pst_vpls_cb == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_VPLS_CB at %p", pstVpls));  
    NBB_TRC_FLOW((NBB_FORMAT "Free VPLS_ID %d", pst_vpls_cb->vpls_id_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (pst_vpls_cb->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_vpls_cb->basic_cfg_cb, MEM_SPM_VPLS_BASIC_CB);  
        pst_vpls_cb->basic_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* �ͷ�NNI��������                                                */
    /***************************************************************************/
    for (pst_vpls_nni_cb = (SPM_VPLS_NNI_CB *)AVLL_FIRST(pst_vpls_cb->nni_cfg_tree);
         NULL != pst_vpls_nni_cb;
         pst_vpls_nni_cb = (SPM_VPLS_NNI_CB *)AVLL_FIRST(pst_vpls_cb->nni_cfg_tree))
    {
        AVLL_DELETE(pst_vpls_cb->nni_cfg_tree, pst_vpls_nni_cb->spm_vpls_nni_node);
        spm_free_vpls_nni_cb(pst_vpls_nni_cb NBB_CCXT);
    }

    /***************************************************************************/
    /* �ͷ�UNI��������                                                */
    /***************************************************************************/
     for (pst_vpls_uni_cb = (SPM_VPLS_UNI_CB *)AVLL_FIRST(pst_vpls_cb->uni_cfg_tree);
         NULL != pst_vpls_uni_cb;
         pst_vpls_uni_cb = (SPM_VPLS_UNI_CB *)AVLL_FIRST(pst_vpls_cb->uni_cfg_tree))
    {
        AVLL_DELETE(pst_vpls_cb->uni_cfg_tree, pst_vpls_uni_cb->spm_vpls_uni_node);
        spm_free_vpls_uni_cb(pst_vpls_uni_cb NBB_CCXT);
    }


    /*************************************************************************/
    /* ��SHARED��������Ӧ��tree���Ƴ�VPLS�����ÿ��ƿ顣                        */
    /*************************************************************************/
    if (AVLL_IN_TREE(pst_vpls_cb->spm_vpls_node))
    {
        AVLL_DELETE(SHARED.vpls_tree, pst_vpls_cb->spm_vpls_node);  
    }
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_vpls_cb->spm_vpls_handle, HDL_SPM_VPLS_CB);


    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_vpls_cb, MEM_SPM_VPLS_CB);  
    pst_vpls_cb = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_free_all_vpls_cb
   ��������  : �ͷ�vpls�������е��ڴ�ռ�
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
NBB_VOID spm_free_all_vpls_cb(NBB_CXT_T NBB_CXT)
{
	SPM_VPLS_CB *pstVpls = NULL;
	
    for (pstVpls = (SPM_VPLS_CB*) AVLL_FIRST(SHARED.vpls_tree);
         pstVpls != NULL;
         pstVpls = (SPM_VPLS_CB*) AVLL_FIRST(SHARED.vpls_tree))
    {	
    	spm_free_vpls_cb(pstVpls NBB_CCXT);
    }
}



#endif

