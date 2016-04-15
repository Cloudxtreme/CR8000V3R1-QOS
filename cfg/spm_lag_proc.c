/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************

  �� �� ��   : spm_lag_proc.c
  �� �� ��   : ����
  ��    ��   : xiaoxiang
  ��������   : 2012��11��30��
  ����޸�   :
  ��������   : ��·�ۺϴ����ļ�
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��11��30��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char lag_cfg_print_setting;

/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_link_aggr
 ��������  : ��·�ۺ�IPS��Ϣ������
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��11��30��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_link_aggr(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    
    NBB_INT ret = SUCCESS;
    
    SPM_LAG_CB *pstLag = NULL;
	
    /* �����õĲ���ģʽ */
    NBB_ULONG ulOperGlobal  = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLag     = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPort    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperNlbPort = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4Nf = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpSample = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6Nf = ATG_DCI_OPER_NULL;
	NBB_ULONG ulOperMc = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLinkDetect = ATG_DCI_OPER_NULL;

    /* LAG�ļ�ֵ */
    NBB_ULONG ulLagIdKey = 0;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_BOOL avll_ret_code;

    NBB_TRC_ENTRY("spm_rcv_dci_set_link_aggr");

    /* �������ָ�������Ч */
    NBB_ASSERT(pstSetLinkAggr != NULL);

	if (pstSetLinkAggr == NULL)
	{
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d\n",LAG_PROC_IPS_NULL);        
        goto EXIT_LABEL;
    }
	

    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pstSetLinkAggr->return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->glob_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->lag_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->nlb_port_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->ipv4nf_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->ipnf_sampler_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->ipv6nf_return_code = ATG_DCI_RC_OK;
	pstSetLinkAggr->mc_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->link_detect_return_code = ATG_DCI_RC_OK;

    
    ulLagIdKey = pstSetLinkAggr->key;    
    pstLag = AVLL_FIND(SHARED.lag_tree, &ulLagIdKey);
    
    if (pstLag == NULL)    //��Ŀ������
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }
    
    /* ��ȡ�����õĲ������� */
    ulOperGlobal    = pstSetLinkAggr->oper_glob_attrib;
    ulOperLag       = pstSetLinkAggr->oper_lag_conf;
    ulOperPort      = pstSetLinkAggr->oper_member_port;
    ulOperNlbPort   = pstSetLinkAggr->oper_nlb_member_port;
    ulOperIpv4Nf    = pstSetLinkAggr->oper_ipv4nf;
    ulOperIpSample  = pstSetLinkAggr->oper_ipnf_sampler;
    ulOperIpv6Nf    = pstSetLinkAggr->oper_ipv6nf;
	ulOperMc        = pstSetLinkAggr->oper_mc;
    ulOperLinkDetect = pstSetLinkAggr->oper_link_detect;

    /* ɾ��������Ŀ */
    if (pstSetLinkAggr->delete_struct == TRUE)
    {   
        /* �����Ŀ�����ڣ���ɾ�� */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {  
            SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d\n",LAG_PROC_DEL_NULL_LOCAL_CFG);
            goto EXIT_LABEL;
        }
        
        /* ���ڣ�ɾ�� */
        else
        {
            spm_dbg_print_lag_head(ulLagIdKey, SPM_OPER_DEL);
            spm_dbg_record_lag_head(ulLagIdKey, SPM_OPER_DEL);

            //ɾ���澯ע��
            if((NULL != pstLag->lag_cfg_cb)
                && (NULL != pstLag->nlb_port_cfg_cb)
                && ((ATG_DCI_MANUAL_NONE_LOADBALANCE == pstLag->lag_cfg_cb->mode)
                || (ATG_DCI_LACP_STATIC_NONE_LOADBALANCE == pstLag->lag_cfg_cb->mode)))
            {
                //��������ܸ澯�ϱ�
                #ifdef SPU
                //spm_del_lag_alarm(ulLagIdKey);
                #endif
            }
            

            //ˢ�»ص�������
            ret = spm_lag_drv_del_lag_all(ulLagIdKey);
			if(ret != SUCCESS)
            {
                pstLag->total_ret_code = ret;
				pstLag->if_spm_del_failed = SPM_CFG_DEL_FAILED;
                goto EXIT_LABEL;
            }         
			
            //������ɾ���ڵ�
            AVLL_DELETE(SHARED.lag_tree, pstLag->spm_lag_node); 

            //�ͷŽڵ��ڴ�ռ�
            spm_free_lag_cb(pstLag);
            pstLag = NULL;
			
        }
    }
    
    /* ���ӻ������Ŀ */
    else
    {        
        
        /* �����Ŀ�����ڣ�����Ҫ������Ŀ */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            spm_dbg_print_lag_head(ulLagIdKey, SPM_OPER_ADD);
            spm_dbg_record_lag_head(ulLagIdKey, SPM_OPER_ADD);
            
            pstLag = spm_alloc_lag_cb(NBB_CXT);

            if(NULL == pstLag)
            {
            	pstLag->total_ret_code = LAG_CFG_ALLOC_MEMORY_FAILD;
                goto EXIT_LABEL;
            }

			if (pstLag != NULL)
			{
	            //����ռ���Ȳ��뵽tree��
	            pstLag->lag_id_key = ulLagIdKey;
	            avll_ret_code = AVLL_INSERT(SHARED.lag_tree, pstLag->spm_lag_node);
		        NBB_ASSERT(avll_ret_code);
			}
        }
        else
        {
            spm_dbg_print_lag_head(ulLagIdKey, SPM_OPER_UPD);
            spm_dbg_record_lag_head(ulLagIdKey, SPM_OPER_UPD);
        }
       
        /******************************** ȫ������ *********************************/
		if(ATG_DCI_OPER_NULL != ulOperGlobal)
		{
			ret = spm_proc_lag_global(pstSetLinkAggr, pstLag, ulOperGlobal);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process lag global faild ret=%d\n",ret);
	            pstLag->global_cfg_return_code = ret;     
	        }
		}

        /******************************** LAG���� *********************************/
		if(ATG_DCI_OPER_NULL != ulOperLag)
		{
			ret = spm_proc_lag_conf(pstSetLinkAggr, pstLag, ulOperLag);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process lag conf faild ret=%d\n",ret);
	            pstLag->lag_cfg_return_code = ret;    
	        }
		}
        

        /******************************** ��Ա�˿����� *********************************/
		if(ATG_DCI_OPER_NULL != ulOperPort)
		{
			ret = spm_proc_lag_member_port(pstSetLinkAggr,pstLag,ulOperPort);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process lag member port faild ret=%d\n",ret);
	            pstLag->port_cfg_return_code = ret;   
	        }
		}
		

        /******************************** �Ǹ��طֵ���Ա�˿����� *********************************/
		if(ATG_DCI_OPER_NULL != ulOperNlbPort)
		{
			ret = spm_proc_nlb_member_port(pstSetLinkAggr,pstLag,ulOperNlbPort);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process nlb member port faild ret=%d\n",ret);
	            pstLag->nlb_port_cfg_return_code = ret;  
	        }
		}
		

        /*********************** IPV4������ʹ������IPV4 NetFlow ********************************/
		if(ATG_DCI_OPER_NULL != ulOperIpv4Nf)
		{
			ret = spm_proc_lag_ipv4_nf(pstSetLinkAggr,pstLag,ulOperIpv4Nf);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process ipv4 nf faild ret=%d\n",ret);
	            pstLag->ipv4_nf_cfg_return_code = ret;  
	        }    
		}
		  

        /*********************** IPV6������ʹ������IPV6 NetFlow ********************************/
		if(ATG_DCI_OPER_NULL != ulOperIpv6Nf)
		{
			ret = spm_proc_lag_ipv6_nf(pstSetLinkAggr,pstLag,ulOperIpv6Nf);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process ipv6 nf faild ret=%d\n",ret);
	            pstLag->ipv6_nf_cfg_return_code = ret;  
	        } 
		}
		

        /******************************** IP��������������IPV4 NetFlow ********************************/
		if(ATG_DCI_OPER_NULL != ulOperIpSample)
		{
			ret = spm_proc_lag_ip_sample(pstSetLinkAggr,pstLag,ulOperIpSample);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process ip sample faild ret=%d\n",ret);
	            pstLag->ip_sample_cfg_return_code = ret;  
	        }
		}
		
        /**************************************��·��� *********************************/
		if(ATG_DCI_OPER_NULL != ulOperLinkDetect)
		{
			ret = spm_proc_lag_link_detect(pstSetLinkAggr,pstLag,ulOperLinkDetect);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process ip sample faild ret=%d\n",ret);
	            pstLag->ip_sample_cfg_return_code = ret;  
	        }
		}
		
        
        #if 1       
	    /**************************************�����ӿ���*********************************/
       
        //Ӧ�ó���:�¹����ӿ���ʱ���߼��ӿ�δ�·���
        //��������·����߼��ӿ�Ϊlag����lag���ÿ����淴ˢ
        ret = spm_refresh_associateif_lag(ulLagIdKey);  
        if(SUCCESS != ret)
        {
            SPM_L2_ERROR_PRINT_LOG(ucMessage,"refresh associateif faild ret=%d\n",ret);
            pstLag->ip_sample_cfg_return_code = ret;   
        }
        #endif
    }

    EXIT_LABEL : NBB_TRC_EXIT();

}

/*****************************************************************************
 �� �� ��  : spm_proc_lag_global
 ��������  : LAG��ȫ�����ô���
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperGlobal     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_lag_global(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperGlobal)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucGlobalDataStart = NULL;
    ATG_DCI_LINK_AGGR_GLOB_ATTRIB *pstGlobalData = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_LAG_GLOBAL_INPUT_POINTER_NULL;
        return ret;
    }

    //�жϲ������ͣ�ֻ�����ӵĲ������д���
    switch(ulOperGlobal)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
			break;

		case ATG_DCI_OPER_ADD:

            //��ȡglobal���õ��׵�ַ
            pucGlobalDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                    &pstSetLinkAggr->glob_attrib);
            if(NULL == pucGlobalDataStart)
            {
                ret = LAG_GLOBAL_CFG_NULL;
                return ret;    
            }

            pstGlobalData = (ATG_DCI_LINK_AGGR_GLOB_ATTRIB *)pucGlobalDataStart;

            //���������Ƿ��ӡ����Ļ��
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  1��ȫ������\n");
                spm_dbg_print_lag_global_cfg(pstGlobalData);
            }

            //���·������ü�¼����־��
            OS_SPRINTF(ucMessage,"  1��ȫ������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_global_cfg(pstGlobalData);

            //�ж��ڴ��Ƿ�Ϊ�գ�Ϊ�յĻ������ڴ�
            if(NULL == pstLag->global_cfg_cb)
            {
                pstLag->global_cfg_cb = (ATG_DCI_LINK_AGGR_GLOB_ATTRIB*)NBB_MM_ALLOC(
                                sizeof(ATG_DCI_LINK_AGGR_GLOB_ATTRIB),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LAG_GLOBAL_CB);    
            }

            if(NULL != pstLag->global_cfg_cb)
            {
                //�������ݵ����ص���������
                OS_MEMCPY(pstLag->global_cfg_cb, pstGlobalData, sizeof(ATG_DCI_LINK_AGGR_GLOB_ATTRIB));
            }
            else
            {
                ret = LAG_GLOBAL_CFG_ALLOC_MEMORY_FAILD;
                return ret;    
            }
            break;
            
        default:
            break;
    }
    
    return ret;    

}


/*****************************************************************************
 �� �� ��  : spm_proc_lag_conf
 ��������  : LAG��ȫ�����ô���
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperLag     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_lag_conf(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperLag)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucLagDataStart = NULL;
    ATG_DCI_LINK_AGGR_LAG_CONF *pstLagData = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_LAG_CONF_INPUT_POINTER_NULL;
        return ret;
    }

    ulLagKey = pstSetLinkAggr->key;
    
    //�жϲ������ͣ�ֻ�����ӵĲ������д���
    switch(ulOperLag)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
			break;

		case ATG_DCI_OPER_ADD:

            //��ȡglobal���õ��׵�ַ
            pucLagDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                    &pstSetLinkAggr->lag_conf);
            if(NULL == pucLagDataStart)
            {
                ret = LAG_CONF_CFG_NULL;
                return ret;    
            }

            pstLagData = (ATG_DCI_LINK_AGGR_LAG_CONF *)pucLagDataStart;

            //���������Ƿ��ӡ����Ļ��
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  2��LAG����\n");
                spm_dbg_print_lag_lagconf_cfg(pstLagData);
            }
            
            //���·������ü�¼����־��
            OS_SPRINTF(ucMessage,"  2��LAG����\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_lagconf_cfg(pstLagData);

            //�ж��ڴ��Ƿ�Ϊ�գ�Ϊ�յĻ������ڴ�
            if(NULL == pstLag->lag_cfg_cb)
            {
                pstLag->lag_cfg_cb = (ATG_DCI_LINK_AGGR_LAG_CONF*)NBB_MM_ALLOC(
                                sizeof(ATG_DCI_LINK_AGGR_LAG_CONF),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LAG_CONF_CB);    
            }

            if(NULL != pstLag->lag_cfg_cb)
            {
                //�������ݵ����ص���������
                OS_MEMCPY(pstLag->lag_cfg_cb, pstLagData, sizeof(ATG_DCI_LINK_AGGR_LAG_CONF));
            }
            else
            {
                ret = LAG_CONF_CFG_ALLOC_MEMORY_FAILD;
                return ret;    
            }
            break;
            
        default:
            break;
    }

    ret = spm_lag_drv_set_lag_attri(ulLagKey, pstLagData);
    
    return ret;    

}

/*****************************************************************************
 �� �� ��  : spm_proc_lag_member_port
 ��������  : LAG��Ա�˿����ô���
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperPort     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_lag_member_port(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperPort)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucPortDataStart = NULL;
    ATG_DCI_LINK_AGGR_MEMBER_PORT *pstPortData[ATG_DCI_LAG_MEMBER_PORT_NUM];
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;
    NBB_BYTE i = 0;

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_LAG_CONF_INPUT_POINTER_NULL;
        return ret;
    }

    for (i = 0; i < ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
    {
        pstPortData[i] = NULL;
    }
    
    ulLagKey = pstSetLinkAggr->key;
            
    //�жϲ������ͣ�ֻ�����ӵĲ������д���
    switch(ulOperPort)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  3����Ա�˿�����(num = %d) UPD\n", pstSetLinkAggr->member_port_num);
            }

            OS_SPRINTF(ucMessage,"  3����Ա�˿�����(num = %d) UPD\n", pstSetLinkAggr->member_port_num);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            if(0 == pstSetLinkAggr->member_port_num)
            {
                //�����ԭ�����ã��ͷŵ��ڴ�ռ�
                for (i = 0; i < ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
                {
                    if (pstLag->port_cfg_cb[i] != NULL)
                    {
                        NBB_MM_FREE(pstLag->port_cfg_cb[i], MEM_SPM_LAG_PORT_CB);
                        pstLag->port_cfg_cb[i] = NULL;
                    }
                }

                pstLag->port_num = 0;
                pstLag->lag_slot_id = 0;
                pstLag->lag_port_id = 0;
            }
            else
            {

                /* �����һ��entry�ĵ�ַ��*/
                pucPortDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, &pstSetLinkAggr->member_port);    

                /* ���ָ��ΪNULL�������� */
                if (pucPortDataStart == NULL)
                {
                    ret = LAG_MEMBER_PORT_CFG_NULL;
                    return ret;
                        
                }
                else
                {
                    //�����ԭ�����ã��ͷŵ��ڴ�ռ�
                    for (i = 0; i<ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
                    {
                        if (pstLag->port_cfg_cb[i] != NULL)
                        {
                            NBB_MM_FREE(pstLag->port_cfg_cb[i], MEM_SPM_LAG_PORT_CB);
                            pstLag->port_cfg_cb[i] = NULL;                            
                        }
                    }
                    
                    for (i = 0; i < pstSetLinkAggr->member_port_num; i++)
                    {
                        pstPortData[i] = (ATG_DCI_LINK_AGGR_MEMBER_PORT *)(pucPortDataStart 
                            + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LINK_AGGR_MEMBER_PORT))) * i); 

                        //�������ô�ӡ����Ļ
                        if (lag_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_lag_port_cfg(pstPortData[i]);
                        }

                        //�ѳ�Ա�˿ڵ����ü�¼����־
                        spm_dbg_record_lag_port_cfg(pstPortData[i]);

                        if (pstLag->port_cfg_cb[i] == NULL)
                        {
                            pstLag->port_cfg_cb[i] = (ATG_DCI_LINK_AGGR_MEMBER_PORT *)NBB_MM_ALLOC(
                                sizeof(ATG_DCI_LINK_AGGR_MEMBER_PORT),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LAG_PORT_CB);

                            if (pstLag->port_cfg_cb[i] == NULL)
                            {
                                ret = LAG_MEMBER_PORT_ALLOC_MEMORY_FAILD;
                                return ret;
                    			
                            }
                        }
 
                        //�������õ�����
                        OS_MEMCPY(pstLag->port_cfg_cb[i], pstPortData[i], sizeof(ATG_DCI_LINK_AGGR_MEMBER_PORT));

                    } 
                    pstLag->member_port_num = pstSetLinkAggr->member_port_num;
                    pstLag->port_num = pstSetLinkAggr->member_port_num;
                }  
            }
			break;

		case ATG_DCI_OPER_ADD:
            break;
            
        default:
            break;
    }
    if((pstLag->lag_cfg_cb->mode == ATG_DCI_MANUAL_LOADBALANCE)
     ||(pstLag->lag_cfg_cb->mode == ATG_DCI_LACP_STATIC_LOADBALANCE))
    {
        ret = spm_lag_drv_set_port_cfg(pstLag);
    }
    else
    {
        ret = MEMBER_PORT_MOD_IS_ERROR;
    }
     
    return ret;    

}

/*****************************************************************************
 �� �� ��  : spm_proc_nlb_member_port
 ��������  : LAG�Ǹ��س�Ա�˿����ô���
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperPort     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_nlb_member_port(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperNlbPort)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucNlbPortDataStart = NULL;
    ATG_DCI_LAG_NLB_MEMBER_PORT *pstNlbPortData;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;    

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = LAG_NLB_PORT_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperNlbPort)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            pucNlbPortDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                     &pstSetLinkAggr->nlb_member_port);    

            /* �׵�ַΪNULL���쳣 */
            if (pucNlbPortDataStart == NULL)
            {
                ret = LAG_NLB_PORT_CFG_NULL;
                return ret;
            }

            pstNlbPortData = (ATG_DCI_LAG_NLB_MEMBER_PORT*)pucNlbPortDataStart;

            //���ƴ�ӡ����Ļ
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  4���Ǹ��طֵ���Ա�˿�����\n");
                spm_dbg_print_lag_nlb_port_cfg(pstNlbPortData);
            }

            //��¼���õ���־�ļ�
            OS_SPRINTF(ucMessage,"  4���Ǹ��طֵ���Ա�˿�����\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_nlb_port_cfg(pstNlbPortData);

            if (pstLag->nlb_port_cfg_cb == NULL)
            {
                //�����ڴ�
                pstLag->nlb_port_cfg_cb = (ATG_DCI_LAG_NLB_MEMBER_PORT*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_NLB_MEMBER_PORT),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_NLB_PORT_CB);
                
                if (pstLag->nlb_port_cfg_cb == NULL)
                {
                    ret = LAG_NLB_PORT_ALLOC_MEMORY_FAILD;
                    return ret;
        			
                }                
            }                

            //�������õ�����
            if(NULL != pstLag->nlb_port_cfg_cb)
            {
                OS_MEMCPY(pstLag->nlb_port_cfg_cb, pstNlbPortData, sizeof(ATG_DCI_LAG_NLB_MEMBER_PORT));
            }                    
            break;

        default:
            break;        
    }
    if((pstLag->lag_cfg_cb->mode == ATG_DCI_MANUAL_NONE_LOADBALANCE)
     ||(pstLag->lag_cfg_cb->mode == ATG_DCI_LACP_STATIC_NONE_LOADBALANCE))
    {
        ret = spm_lag_drv_set_nlb_member_cfg(pstLag);
    }
    else
    {
        ret = NLB_MEMBER_PORT_MOD_IS_ERROR;
    }
    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_proc_lag_ipv4_nf
 ��������  : LAGIPV4���������ô���
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperPort     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_lag_ipv4_nf(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperIpv4Nf)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucIpv4NfDataStart = NULL;
    ATG_DCI_LAG_IPV4NF_DATA *pstIpv4NfData;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0; 
    
    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_IPV4_NF_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperIpv4Nf)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            
            /* �����һ��entry�ĵ�ַ��*/
            pucIpv4NfDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->ipv4nf_data);    

            /* �׵�ַΪNULL���쳣 */
            if (pucIpv4NfDataStart == NULL)
            {
                ret = IPV4_NF_CFG_NULL;
                return ret;    
            }

            pstIpv4NfData = (ATG_DCI_LAG_IPV4NF_DATA*)pucIpv4NfDataStart;                
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  5��ipv4������ʹ������\n");
                spm_dbg_print_lag_ipv4_nf_data(pstIpv4NfData);
            }

            OS_SPRINTF(ucMessage,"  5��ipv4������ʹ������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_ipv4_nf_data(pstIpv4NfData);

            /* �������� */
            if (pstLag->ipv4_nf_data == NULL)
            {
                pstLag->ipv4_nf_data = (ATG_DCI_LAG_IPV4NF_DATA*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_IPV4NF_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_IPV4_NF_CB);

                if (pstLag->ipv4_nf_data == NULL)
                {
                    ret = IPV4_NF_ALLOC_MEMORY_FAILD;
                    return ret;
                	
                }
            }

            if(NULL != pstLag->ipv4_nf_data)
            {
                OS_MEMCPY(pstLag->ipv4_nf_data, pstIpv4NfData, sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA));
            }
            
            break;
            
        default:
            break;
    }

    ret = spm_lag_drv_set_ipv4_nf_cfg(pstLag);
    return ret; 
}

/*****************************************************************************
 �� �� ��  : spm_proc_lag_ipv6_nf
 ��������  : LAGIPV6���������ô���
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperIpv6Nf     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_lag_ipv6_nf(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperIpv6Nf)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucIpv6NfDataStart = NULL;
    ATG_DCI_LAG_IPV6NF_DATA *pstIpv6NfData;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0; 
    
    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_IPV6_NF_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperIpv6Nf)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            
            /* �����һ��entry�ĵ�ַ��*/
            pucIpv6NfDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->ipv6nf_data);    

            /* �׵�ַΪNULL���쳣 */
            if (pucIpv6NfDataStart == NULL)
            {
                ret = IPV6_NF_CFG_NULL;
                return ret;
            }
            pstIpv6NfData = (ATG_DCI_LAG_IPV6NF_DATA*)pucIpv6NfDataStart;

            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  7��ipv6������ʹ������\n");
                spm_dbg_print_lag_ipv6_nf_data(pstIpv6NfData);
            }

            OS_SPRINTF(ucMessage,"  7��ipv6������ʹ������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_ipv6_nf_data(pstIpv6NfData);

            /* �������� */
            if (pstLag->ipv6_nf_data == NULL)
            {
                pstLag->ipv6_nf_data = (ATG_DCI_LAG_IPV6NF_DATA*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_IPV6NF_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_IPV6_NF_CB);

                if (pstLag->ipv6_nf_data == NULL)
                {                   
        		    ret = IPV6_NF_ALLOC_MEMORY_FAILD;
                    return ret;	
                }
            }
            
            if(pstLag->ipv6_nf_data != NULL)
            {              
                OS_MEMCPY(pstLag->ipv6_nf_data, pstIpv6NfData, sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA));
            }
            
            break;
            
        default:
            break;
    }

    ret = spm_lag_drv_set_ipv6_nf_cfg(pstLag);
    return ret; 
    
}


/*****************************************************************************
 �� �� ��  : spm_proc_lag_ipv6_nf
 ��������  : LAGIP�������������ô���
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperIpSample     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_lag_ip_sample(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperIpSample)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucIpSampleDataStart = NULL;
    ATG_DCI_LAG_IPNF_SAMPLER_DATA *pstIpSampleData;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0; 

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_IP_SAMPLE_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperIpSample)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            /* �����һ��entry�ĵ�ַ��*/
            pucIpSampleDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->ipnf_sampler_data);    

            /* �׵�ַΪNULL���쳣 */
            if (pucIpSampleDataStart == NULL)
            {
                ret = IP_SAMPLE_CFG_NULL;
                return ret;   
            }

            pstIpSampleData = (ATG_DCI_LAG_IPNF_SAMPLER_DATA*)pucIpSampleDataStart; 

            //��ӡ���õ���Ļ
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  6��ip��������������\n");
                spm_dbg_print_lag_ip_sampler_data(pstIpSampleData);
            }

            //���������ݼ�¼����־�ļ�
            OS_SPRINTF(ucMessage,"  6��ip��������������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_ip_sampler_data(pstIpSampleData);

            if (pstLag->ip_sample_data == NULL)
            {
                //Ϊ�洢�����������ڴ�
                pstLag->ip_sample_data = (ATG_DCI_LAG_IPNF_SAMPLER_DATA*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_IPNF_SAMPLER_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_IPNF_SAMPLER);
                
                if (pstLag->ip_sample_data == NULL)
        		{
        		    ret = IP_SAMPLE_ALLOC_MEMORY_FAILD;
                    return ret;     
        		}
            }  
            if(pstLag->ip_sample_data != NULL)
            {
                //�������ݵ�����������
                OS_MEMCPY(pstLag->ip_sample_data, pstIpSampleData, sizeof(ATG_DCI_LAG_IPNF_SAMPLER_DATA));
            } 
            
            break;
            
        default:
            break;
    }

    ret = spm_lag_drv_set_ip_sample_cfg(pstLag);
    return ret;    

}



/*****************************************************************************
 �� �� ��  : spm_proc_lag_mc_lag
 ��������  : MC-LAG����
 �������  : 
                
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_lag_mc_lag(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperMc)

{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *puc_mc_start = NULL;
    ATG_DCI_LAG_MC_DATA *pst_mc_data;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;     

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        return PROC_MC_LAG_INPUT_POINTER_NULL;
    }

    switch(ulOperMc)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            
            /* �����һ��entry�ĵ�ַ��*/
            puc_mc_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->mc_data);    

            /* �׵�ַΪNULL���쳣 */
            if (puc_mc_start == NULL)
            {
                return MC_LAG_DATA_START_NULL;
                
            }
            pst_mc_data = (ATG_DCI_LAG_MC_DATA *)puc_mc_start;

            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  8��MC-LAG\n");
                spm_dbg_print_lag_mc_lag_data(pst_mc_data);
            }

            OS_SPRINTF(ucMessage,"  8��MC-LAG\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_mc_lag_data(pst_mc_data);

            /* �������� */
            if (pstLag->mc_data == NULL)
            {
                pstLag->mc_data = (ATG_DCI_LAG_MC_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_MC_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_MC_LAG_CB);

                if (pstLag->mc_data == NULL)
                {                   
        		    return MC_LAG_ALLOC_MEMORY_FAILD;
                }
            }
            
            if(pstLag->mc_data != NULL)
            {              
                OS_MEMCPY(pstLag->mc_data, pst_mc_data, sizeof(ATG_DCI_LAG_MC_DATA));
            }
            
            break;
            
        default:
            break;
    }

    ret = spm_set_lag_min_member(pstLag);
    return ret; 
    
}


/*****************************************************************************
 �� �� ��  : spm_proc_lag_ipv6_nf
 ��������  : LAGIP�������������ô���
 �������  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperLinkDetect     
 �������  : ��
 �� �� ֵ  : SUCCESS/ERROR
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��22��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_proc_lag_link_detect(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperLinkDetect)

{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *puc_link_detect = NULL;
    ATG_DCI_LAG_LINK_DETECT_DATA *pst_link_detect;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;     

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_IPV6_NF_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperLinkDetect)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            
            /* �����һ��entry�ĵ�ַ��*/
            puc_link_detect = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->link_detect_data);    

            /* �׵�ַΪNULL���쳣 */
            if (puc_link_detect == NULL)
            {
                ret = LINK_DETECT_CFG_NULL;
                return ret;
            }
            pst_link_detect = (ATG_DCI_LAG_LINK_DETECT_DATA *)puc_link_detect;

            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  9����·�������\n");
                spm_dbg_print_lag_link_detect_data(pst_link_detect);
            }

            OS_SPRINTF(ucMessage,"  9����·�������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_link_detect_data(pst_link_detect);

            /* �������� */
            if (pstLag->link_detect_cfg == NULL)
            {
                pstLag->link_detect_cfg = (ATG_DCI_LAG_LINK_DETECT_DATA*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_LINK_DETECT_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_LINK_DETECT_CB);

                if (pstLag->link_detect_cfg == NULL)
                {                   
        		    ret = LINK_DETECT_ALLOC_MEMORY_FAILD;
                    return ret;	
                }
            }
            
            if(pstLag->link_detect_cfg != NULL)
            {              
                OS_MEMCPY(pstLag->link_detect_cfg, pst_link_detect, sizeof(ATG_DCI_LAG_LINK_DETECT_DATA));
            }
            
            break;
            
        default:
            break;
    }

   ret = spm_lag_drv_set_ipv6_nf_cfg(pstLag);
    return ret; 
    
}


/*****************************************************************************
 �� �� ��  : spm_refresh_associateif_lag
 ��������  : Ӧ�ó���:�¹����ӿ���ʱ���߼��ӿ�δ�·���
             ��������·����߼��ӿ�Ϊlag����lag���ÿ����淴ˢ
 �������  : NBB_ULONG ulLagId         
 �������  : ��
 �� �� ֵ  : -1/0 = ʧ��/�ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��10��31��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_refresh_associateif_lag(NBB_ULONG lag_key)
{
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_ASSOCIATE_IF_CB *pstAssociateIfCb = NULL;
    NBB_ULONG ulPortIndexKey = 0;
    NBB_ULONG ulAssociateIfKey = 0;
    NBB_BYTE trigger_if_flag = 0;
    NBB_BYTE act_if_flag = 0;
    NBB_BYTE ucExistPos_trigger = 0;
    NBB_BYTE ucExistPos_act = 0;
    NBB_BYTE pucPortNum = 0;
    NBB_USHORT usPortId = 0;
    NBB_BYTE usSlotId = 0;
    NBB_BYTE i = 0;
    SPM_LAG_PORT_INFO_CB  stTriggerLagPortInfo[ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM];
    SPM_LAG_PORT_INFO_CB  stActLagPortInfo[ATG_DCI_ASSOCIATE_IF_ACT_NUM];    
    

    OS_MEMSET(stTriggerLagPortInfo,0,sizeof(SPM_LAG_PORT_INFO_CB)*(ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM));
    OS_MEMSET(stActLagPortInfo,0,sizeof(SPM_LAG_PORT_INFO_CB)*(ATG_DCI_ASSOCIATE_IF_ACT_NUM));

    for (pstLogicalPort = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree);
            pstLogicalPort != NULL;
            pstLogicalPort = (SPM_LOGICAL_PORT_CB*) AVLL_NEXT(SHARED.logical_port_tree,pstLogicalPort->spm_logical_port_node))
    {
        if((pstLogicalPort->basic_cfg_cb != NULL)&&(pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
            &&(pstLogicalPort->phy_cfg_cb != NULL)&&(pstLogicalPort->phy_cfg_cb->lag_id == lag_key))
        {
            ulPortIndexKey = pstLogicalPort->port_index_key;
        }
    }
        
     for (pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB *)AVLL_FIRST(v_spm_shared->associate_if_tree);
                 pstAssociateIfCb != NULL;
                 pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB *)AVLL_NEXT(v_spm_shared->associate_if_tree,
                 pstAssociateIfCb->spm_associate_if_node))
    {
        for(i = 0;i < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM;i++)
        {
           if((pstAssociateIfCb->trigger_if_cfg_cb[i] != NULL)&&(pstAssociateIfCb->trigger_if_cfg_cb[i]->interface_index == ulPortIndexKey) 
            && (pstAssociateIfCb->trigger_if_exist[i] == UNEXIST))
           {
                trigger_if_flag = 1;
                ucExistPos_trigger = i;
                ulAssociateIfKey = pstAssociateIfCb->key;
                break;
           }
            
        }
     
        if(trigger_if_flag != 1)
        {
           for(i = 0;i < ATG_DCI_ASSOCIATE_IF_ACT_NUM;i++)
            {
               if((pstAssociateIfCb->act_if_cfg_cb[i] != NULL)&&(pstAssociateIfCb->act_if_cfg_cb[i]->interface_index == ulPortIndexKey)
                && (pstAssociateIfCb->act_if_exist[i] == UNEXIST))
               {
                    act_if_flag = 1;
                    ucExistPos_act = i;
                    ulAssociateIfKey = pstAssociateIfCb->key;
                    break;
               }
                
            } 
        }

        if((trigger_if_flag == 1) || (act_if_flag == 1))
        {
            break;
        }
        
    }

     /*��lag�ĳ�Ա�˿ڶ�Ӧ������ڼ��������Ĵ����ӿ���*/
    if(trigger_if_flag == 1)
    {
        spm_get_lag_all_ports_info(lag_key,stTriggerLagPortInfo,&pucPortNum NBB_CCXT);

		if(pucPortNum == 0)
		{
   		    ret = REFRESH_ASSOCIATIATE_GET_PORT_INFO_FAILD;
            return ret;
		}

		else
		{

            ret = spm_add_trigger_lag_port_info(pucPortNum, ulAssociateIfKey, stTriggerLagPortInfo);						
			if(ret == SUCCESS)
			{
			    if(pstAssociateIfCb != NULL)
                {
                    pstAssociateIfCb->trigger_if_exist[ucExistPos_trigger] = EXIST;
			        pstAssociateIfCb->trigger_if_num++;  
                }         
                
			} 
            else
            {              
                return ret;    
            }

        }
    }
    
    /*��lag��Ա�˿ڼ��������Ķ����ӿ�*/
    else if(act_if_flag == 1)
    {
       spm_get_lag_all_ports_info(lag_key,stActLagPortInfo,&pucPortNum NBB_CCXT);

    	if(pucPortNum == 0)
    	{   	    
            ret = REFRESH_ASSOCIATIATE_GET_PORT_INFO_FAILD_2;
            return ret;
    	}

    	else
    	{

    	    ret = spm_add_ifgoup_lag_port_info(pucPortNum, ulAssociateIfKey, stActLagPortInfo);   					
    		if(ret == SUCCESS)
    		{
    		    if(pstAssociateIfCb != NULL)
                {
                    pstAssociateIfCb->act_if_exist[ucExistPos_act] = EXIST;
    		        pstAssociateIfCb->act_if_num++;  
                }         
                
    		} 

            else
            {
                return ret;         
            }

        }
    }

    return ret;
}




/*****************************************************************************
 �� �� ��  : spm_refresh_vpls_lag
 ��������  : ˢ��vpls��lag
 �������  : NBB_ULONG ulLagId         
 �������  : ��
 �� �� ֵ  : -1/0 = ʧ��/�ɹ�
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��10��31��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_refresh_vpls_lag(NBB_ULONG ulLagId, NBB_ULONG ulPortIndex, 
									NBB_BYTE ucLagSlotCur, NBB_USHORT usLagPortCur,
									NBB_BYTE ucLagSlotPre, NBB_USHORT usLagPortPre
									NBB_CCXT_T NBB_CXT)
{
	#if 0
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	NBB_BYTE ucFlag = 0;
	NBB_INT i = 0;
	NBB_INT ret = SUCCESS;
	NBB_INT iUniPos = -1;
	NBB_INT iVpId = 0;
	SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
	SPM_VPLS_CB *pstVpls = NULL;

	//coverity[returned_pointer]
    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    /*************************************************************************/
    /* ����VPLS ���õ�����һ������֤��                                        */
    /*************************************************************************/
    for (pstVpls = (SPM_VPLS_CB*) AVLL_FIRST(SHARED.vpls_tree);
         pstVpls != NULL;
         pstVpls = (SPM_VPLS_CB*) AVLL_NEXT(SHARED.vpls_tree,
                   pstVpls->spm_vpls_node))
    {
    	//�����ȿ���LAGΪuni�������nni������Ƚϸ���
    	for (i=0; i<ATG_DCI_VPLS_UNI_NUM; i++)
    	{
    		//����ҵ��߼��˿����ڵ�VPLS
    		if ((pstVpls->uni_cfg_cb[i] != NULL) && (pstVpls->uni_cfg_cb[i]->port_index == ulPortIndex))
    		{
    			iUniPos = i;
    			iVpId = pstVpls->uni_info_cb[i].vp_idx;

#ifdef SRC
				//С©��:��Ա�˿�ɾ���ɾ��������ӳ�Ա��������Ĳ�λ����ô֮ǰ�ĳ�Ա�����ڵĲ��޷�ɾ��
				if (ucLagSlotCur != 0)
				{
					//ɾ��SRC�����ϲ�λ���ư�
					ret = ApiDfeMulticastDelete(UNIT_0, pstVpls->vpls_id_key, ucLagSlotPre);			

					//����SRC�����²�λ���ư�
					ret = ApiDfeMulticastAdd(UNIT_0, pstVpls->vpls_id_key, ucLagSlotCur);
				}				
#endif

#ifdef SPU
				//���ϵĲ�λ��ɾ�����ø�Arad�İ�����
				if (SHARED.local_slot_id == ucLagSlotPre)
				{
					//ret = ApiAradDeleteEngressMcPort(UNIT_0, pstVpls->vpls_id_key, usLagPortPre, iVpId);
					ret = ApiAradDeleteEngressMcPort(UNIT_0, pstVpls->vpls_id_key, SPM_MULTICAST_PORT_0, iVpId);
					pstVpls->uni_info_cb[i].slot_id = 0;
					//pstVpls->uni_info_cb[i].cud = 0;
				}

				//���µĲ�λ���������ø�Arad�İ�����
				if ((SHARED.local_slot_id == ucLagSlotCur) && (usLagPortCur != 0))
				{
					//ret = ApiAradAddEngressMcPort(UNIT_0, pstVpls->vpls_id_key, usLagPortCur, iVpId);
					ret = ApiAradAddEngressMcPort(UNIT_0, pstVpls->vpls_id_key, SPM_MULTICAST_PORT_0, iVpId);
					pstVpls->uni_info_cb[i].slot_id = ucLagSlotCur;
					//pstVpls->uni_info_cb[i].cud = iVpId;
				}
#endif
						

    			break;
    		}
    	}

#if 0
    	//����LAGΪnni�����
    	for (i=0; i<ATG_DCI_VPLS_NNI_NUM; i++)
    	{
    		//����ҵ��߼��˿����ڵ�VPLS
    		if ((pstVpls->nni_cfg_cb[i] != NULL) && (pstVpls->nni_info_cb[i].port_index_mc == ulPortIndex))
    		{
    			iUniPos = i;
    			iVpId = pstVpls->nni_info_cb[i].vp_idx;

#ifdef SRC
				//С©��:��Ա�˿�ɾ���ɾ��������ӳ�Ա��������Ĳ�λ����ô֮ǰ�ĳ�Ա�����ڵĲ��޷�ɾ��
				if (ucLagSlotCur != 0)
				{
					//ɾ��SRC�����ϲ�λ���ư�
					ret = ApiDfeMulticastDelete(UNIT_0, pstVpls->vpls_id_key, ucLagSlotPre);			

					//����SRC�����²�λ���ư�
					ret = ApiDfeMulticastAdd(UNIT_0, pstVpls->vpls_id_key, ucLagSlotCur);
				}				
#endif

#ifdef SPU
				//���ϵĲ�λ��ɾ�����ø�Arad�İ�����
				if (SHARED.local_slot_id == ucLagSlotPre)
				{
					ret = ApiAradDeleteEngressMcPort(UNIT_0, pstVpls->vpls_id_key, SPM_MULTICAST_PORT_0, iVpId);
					pstVpls->nni_info_cb[i].slot_id = 0;
				}

				//���µĲ�λ���������ø�Arad�İ�����
				if ((SHARED.local_slot_id == ucLagSlotCur) && (usLagPortCur != 0))
				{
					ret = ApiAradAddEngressMcPort(UNIT_0, pstVpls->vpls_id_key, SPM_MULTICAST_PORT_0, iVpId);
					pstVpls->nni_info_cb[i].slot_id = ucLagSlotCur;
				}
#endif						

    			break;
    		}
    	}
#endif
    	if (iUniPos != -1)
    	{
    		break;
    	}
    }    

	//���δ�ҵ��߼��˿����ڵ�VPLS��ֱ�ӷ��ش�
    if (iUniPos == -1)
    {
    	OS_PRINTF("***ERROR***: δ�ҵ��߼��˿� PORT_INDEX=%ld ���ڵ�VPLS��ֱ�ӷ��ش�\n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***: δ�ҵ��߼��˿� PORT_INDEX=%ld ���ڵ�VPLS��ֱ�ӷ��ش�\n", ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LAG_PD, 0, "ld d s s", ulLagId, BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
    	return ERROR;    	
    }

	#endif

    return SUCCESS;
}

/*****************************************************************************
 �� �� ��  : spm_init_lag_tree
 ��������  : ��ʼ��LAG���ô洢��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��23��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_lag_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_lag_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_LAG_CB, lag_id_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.lag_tree, compare_ulong,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_LAG_CB, spm_lag_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 �� �� ��  : spm_alloc_lag_cb
 ��������  : ������·�ۺ����õ��ڴ�ռ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : SPM_LAG_CB
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��12��1��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_LAG_CB * spm_alloc_lag_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_LAG_CB *pstLag = NULL;

    NBB_TRC_ENTRY("spm_alloc_lag_cb");

    /* ����һ���µ�VPWS������Ŀ��*/
    pstLag = (SPM_LAG_CB *)NBB_MM_ALLOC(sizeof(SPM_LAG_CB), NBB_NORETRY_ACT, MEM_SPM_LAG_CB);
    if (pstLag == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:����LAG��Ŀ�ռ�ΪNULL!"));
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPWS������Ŀ */
    OS_MEMSET(pstLag, 0, sizeof(SPM_LAG_CB));
    pstLag->global_cfg_cb = NULL;
    pstLag->lag_cfg_cb = NULL;
    pstLag->nlb_port_cfg_cb = NULL;
    pstLag->ipv4_nf_data = NULL;
    pstLag->ipv6_nf_data = NULL;
    pstLag->ip_sample_data = NULL;
	pstLag->mc_data = NULL;
    pstLag->link_detect_cfg = NULL;

    for (i = 0; i<ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
    {
        pstLag->port_cfg_cb[i] = NULL;
    }

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstLag->spm_lag_handle = NBB_CREATE_HANDLE(pstLag, HDL_SPM_LAG_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_LAG_CB allocated at %p with handle %#lx",
                   pstLag, pstLag->spm_lag_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstLag->spm_lag_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstLag);
}

/*****************************************************************************
 �� �� ��  : spm_free_lag_cb
 ��������  : �ͷ�LAG���ڴ�ռ�
 �������  : SPM_LAG_CB *pstLag  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2012��12��1��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_lag_cb(SPM_LAG_CB *pstLag)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_lag_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstLag, sizeof(SPM_LAG_CB), MEM_SPM_LAG_CB); 

    if (pstLag == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    ////NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pstLag));  
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws_id %d", pstLag->lag_id_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (pstLag->global_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLag->global_cfg_cb, MEM_SPM_LAG_GLOBAL_CB);  
        pstLag->global_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷ�LAG�������ݿ顣                                                */
    /***************************************************************************/
    if (pstLag->lag_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLag->lag_cfg_cb, MEM_SPM_LAG_CONF_CB);  
        pstLag->lag_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* �ͷŷǸ��طֵ���Ա�˿��������ݿ顣                                      */
    /***************************************************************************/
    if (pstLag->nlb_port_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLag->nlb_port_cfg_cb, MEM_SPM_LAG_NLB_PORT_CB);  
        pstLag->nlb_port_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* �ͷų�Ա�˿��������ݿ顣                                                */
    /***************************************************************************/
    for (i = 0; i<ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
    {
        if (pstLag->port_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstLag->port_cfg_cb[i], MEM_SPM_LAG_PORT_CB);  
            pstLag->port_cfg_cb[i] = NULL;
        }
    }

	pstLag->port_num = 0;
    
    /***************************************************************************/
    /* �ͷ�ipv4_nf_data���ݿ顣                                                */
    /***************************************************************************/
    if(pstLag->ipv4_nf_data != NULL)
     {
        NBB_MM_FREE(pstLag->ipv4_nf_data, MEM_SPM_LAG_IPV4_NF_CB);  
        pstLag->ipv4_nf_data = NULL;
     }
    
    /***************************************************************************/
    /* �ͷ�ipv6_nf_data���ݿ顣                                                */
    /***************************************************************************/
    if(pstLag->ipv6_nf_data != NULL)
     {
        NBB_MM_FREE(pstLag->ipv6_nf_data, MEM_SPM_LAG_IPV6_NF_CB);  
        pstLag->ipv6_nf_data = NULL;
     }
    
    /***************************************************************************/
    /* �ͷ��������������ݿ顣                                                */
    /***************************************************************************/
    if(pstLag->ip_sample_data!= NULL)
     {
        NBB_MM_FREE(pstLag->ip_sample_data, MEM_SPM_LAG_IPNF_SAMPLER);  
        pstLag->ip_sample_data = NULL;
     }

	/***************************************************************************/
    /* �ͷ�MC-LAG���ݿ顣                                                */
    /***************************************************************************/
    if(pstLag->mc_data != NULL)
     {
        NBB_MM_FREE(pstLag->mc_data, MEM_SPM_LAG_MC_LAG_CB);  
        pstLag->mc_data = NULL;
     }

    /***************************************************************************/
    /* �ͷ���·������ݿ顣                                                */
    /***************************************************************************/
    if(pstLag->link_detect_cfg!= NULL)
     {
        NBB_MM_FREE(pstLag->link_detect_cfg, MEM_SPM_LAG_LINK_DETECT_CB);  
        pstLag->link_detect_cfg = NULL;
     }
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstLag->spm_lag_handle, HDL_SPM_LAG_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstLag, MEM_SPM_LAG_CB);  
    pstLag = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_free_all_lag_cb
   ��������  : �ͷ���·�ۺ��������е��ڴ�ռ�
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

NBB_VOID spm_free_all_lag_cb(NBB_CXT_T NBB_CXT)
{
	SPM_LAG_CB *pstLag = NULL;
	
    for (pstLag = (SPM_LAG_CB*) AVLL_FIRST(SHARED.lag_tree);
         pstLag != NULL;
         pstLag = (SPM_LAG_CB*) AVLL_FIRST(SHARED.lag_tree))
    {
    	
		AVLL_DELETE(SHARED.lag_tree, pstLag->spm_lag_node);     	
    	spm_free_lag_cb(pstLag);
    }
}


