/******************************************************************************

              ��Ȩ���� (C), ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************

�� �� ��   : spm_vpls_dev.c
�� �� ��   : ����
��    ��   : huxl
��������   : 2016��1��22��
����޸�   :
��������   :��װVPLS���ÿ���ص������ӿ�
�����б�   :
�޸���ʷ   :
1.��    ��   : 2016��1��22��
��    ��   : zhangzhm
�޸�����   : �����ļ�

******************************************************************************/


#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*****************************************************************************
�� �� ��  : spm_lag_drv_set_port_cfg
��������  : ����lag�������ã�ɾ��C3�е�lag��Ϣ
             
�������  : NBB_ULONG ulLagIdKey
�������  : APS_PORT_LAG_INFO stApsPortLagInfo
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��2��25��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_lag_drv_set_port_cfg(SPM_LAG_CB *pstLag)
{
    NBB_INT ret = SUCCESS;
    SPM_PORT_INFO_CB stPortInfo;
    APS_PORT_LAG_INFO stApsPortLagInfo;
    NBB_ULONG port_num = 0;
    NBB_ULONG lag_key = 0;
    NBB_USHORT usSlotPort = 0;
    NBB_BYTE i = 0;
    NBB_ULONG lag_consult_port_num = 0;
    LAG_T stTrunk;
    NBB_BYTE ucC3Unit = 0;
    NBB_ULONG ulLagPortId = 0;
    NBB_BYTE ucLagSlotCur = 0;
    NBB_USHORT usLagPortCur = 0;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};

    if((NULL == pstLag) || (NULL == pstLag->lag_cfg_cb))
    {
        ret = UPDATE_APS_PORT_INPUT_POINTER_NULL;
        return ret;
    }

    //��ʼ�������ṹ��
    OS_MEMSET(&stTrunk, 0, sizeof(LAG_T));
    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stApsPortLagInfo, 0, sizeof(APS_PORT_LAG_INFO));
    
    lag_key = pstLag->lag_id_key;
    port_num = pstLag->port_num;
    ulLagPortId = lag_key + 0x80;

    if(0 == port_num)
    {
        stApsPortLagInfo.portNum = 0;
	    aps_update_lag_port_info(lag_key, &stApsPortLagInfo);
    }
    else
    {
         //��bfdʹ�ܱ�־����apsģ��
        stApsPortLagInfo.bfdEnable = pstLag->lag_cfg_cb->bfd_flag;
         
        if ((pstLag->lag_cfg_cb != NULL) && (pstLag->lag_cfg_cb->mode == ATG_DCI_MANUAL_LOADBALANCE))
        {
            for(i = 0; i < port_num; i++)
            {
                //��ȡ��λ�Ͷ˿���Ϣ
                if(NULL != pstLag->port_cfg_cb[i])
                {
	                ret = spm_get_portid_from_logical_port_index(pstLag->port_cfg_cb[i]->port_index, &stPortInfo NBB_CCXT);
                    if(SUCCESS != ret)
                    {
                        SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,ret=%d\n",
                                            GET_PORTID_FROM_LOG_PORT_ERROR_1,ret);
                        ret = GET_PORTID_FROM_LOG_PORT_ERROR_1;
                        return ret;
                    }
                    usSlotPort = ((stPortInfo.slot_id & 0xffff) << 8) + stPortInfo.port_id;
                    stTrunk.cfgSlotPort[i] = usSlotPort;
	                        
					//ˢ�»ص������˿ڱ�����
					stApsPortLagInfo.lagPort[i].slot = stPortInfo.slot_id;
					stApsPortLagInfo.lagPort[i].portId = stPortInfo.port_id;
                    stApsPortLagInfo.lagPort[i].cardId = stPortInfo.card_id;
                    stApsPortLagInfo.lagPort[i].portIndex = pstLag->port_cfg_cb[i]->port_index;
                }
            }
            stTrunk.lagId = lag_key;
            stTrunk.cfgMemberPortNum = port_num;

			//ˢ�»ص������˿ڱ���Ŀ
            stApsPortLagInfo.portNum = port_num;
            stApsPortLagInfo.lagType = pstLag->lag_cfg_cb->mode;
        }
        else if((pstLag->lag_cfg_cb != NULL) 
			&& (pstLag->lag_cfg_cb->mode == ATG_DCI_LACP_STATIC_LOADBALANCE))
        {
            lag_consult_port_num = 0;    

            for(i = 0; i < port_num; i++)
            {
                //��ȡ��λ�Ͷ˿���Ϣ
                if(NULL != pstLag->port_cfg_cb[i])
                {
                    ret = spm_get_portid_from_logical_port_index(pstLag->port_cfg_cb[i]->port_index, &stPortInfo NBB_CCXT);
                    if(SUCCESS != ret)
                    {
                        SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,ret=%d\n",GET_PORTID_FROM_LOG_PORT_ERROR_2,ret);
                        ret = GET_PORTID_FROM_LOG_PORT_ERROR_2;
                        return ret;
                    }
                    usSlotPort = ((stPortInfo.slot_id & 0xffff) << 8) + stPortInfo.port_id;
                    stTrunk.cfgSlotPort[i] = usSlotPort;
                            
                	//Э�̳ɹ�
                    if (pstLag->port_cfg_cb[i]->port_consult == 1)
                    {
                		//ˢ�»ص������˿ڱ�����
                		stApsPortLagInfo.lagPort[lag_consult_port_num].slot = stPortInfo.slot_id;
                		stApsPortLagInfo.lagPort[lag_consult_port_num].portId = stPortInfo.port_id;
                        stApsPortLagInfo.lagPort[lag_consult_port_num].cardId = stPortInfo.card_id;
                        stApsPortLagInfo.lagPort[lag_consult_port_num].portIndex = pstLag->port_cfg_cb[i]->port_index;
                        lag_consult_port_num++;
                    }
                }
            }

            //ִ������
            stTrunk.lagId = lag_key;
            stTrunk.cfgMemberPortNum = port_num;

			//ˢ�»ص������˿ڱ���Ŀ
            stApsPortLagInfo.portNum = lag_consult_port_num;
            stApsPortLagInfo.lagType = pstLag->lag_cfg_cb->mode;
            
        }

        //�ֵ��㷨
    	switch (pstLag->lag_cfg_cb->load_sharing_algorithm)
    	{
    		case ATG_DCI_BASE_BOTH_MAC:
                stTrunk.alg = LAG_HASH_DSMAC;
    			break;
    		case ATG_DCI_BASE_BOTH_IP:
                stTrunk.alg = LAG_HASH_DSIP;
    			break;
    		case ATG_DCI_BASE_PACKET:
                stTrunk.alg = LAG_PORT_ROUND_ROBIN;
    			break;
    		default:
    			break;
    	}

        //�����Ч��Ա�˿���Ϊ0��ֱ�Ӹ���APSģ�����
        if ((pstLag->lag_cfg_cb->mode == ATG_DCI_LACP_STATIC_LOADBALANCE) 
         && (lag_consult_port_num == 0))
        {
			stApsPortLagInfo.portNum = 0;
			aps_update_lag_port_info(lag_key, &stApsPortLagInfo);
            
            pstLag->lag_port_id = 0;
            pstLag->lag_slot_id = 0;
        }
        else
        {
#ifdef SPU
            for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
            {
                ret += ApiC3SetLag(ucC3Unit, &stTrunk);
				
            }

			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,ret=%d\n",C3_SET_LAG_ERR,ret);
                return C3_SET_LAG_ERR;
                
			}

            for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
            {
                ret += ApiC3SetPortMac(ucC3Unit, ATG_DCI_IS_NOT_VE, 
						ulLagPortId, pstLag->lag_cfg_cb->lag_mac);
            }

			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,ret=%d\n",
                                            C3_DEL_LAG_PORT_INFO_ERROR,ret);
                ret = SET_C3_LAG_PORT_INFO_FAILD;
                return ret;
			}
#endif                    

            spm_get_lag_min_slot_port(lag_key, &ucLagSlotCur, &usLagPortCur NBB_CCXT);
            pstLag->lag_port_id = usLagPortCur;
            pstLag->lag_slot_id = ucLagSlotCur;

        	ret = aps_update_lag_port_info(lag_key, &stApsPortLagInfo);
			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",LAG_UPDATE_APS_INFO_ERR,ret);
				return LAG_UPDATE_APS_INFO_ERR;
			}
        }
        
    }

    return ret;
}

/*****************************************************************************
�� �� ��  : spm_lag_drv_set_nlb_member_cfg
��������  : ����lag�Ǹ��س�Ա�������ã�ɾ��C3�е�lag��Ϣ
             
�������  : NBB_ULONG ulLagIdKey
�������  : APS_PORT_LAG_INFO stApsPortLagInfo
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��2��25��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_lag_drv_set_nlb_member_cfg(SPM_LAG_CB *pstLag)
{
    NBB_INT ret = SUCCESS;
    APS_PORT_LAG_INFO stApsPortLagInfo;
    SPM_PORT_INFO_CB stPortInfo;
    LAG_T stTrunk;
    NBB_ULONG lag_key = 0;
    NBB_USHORT usSlotPort = 0;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    NBB_BYTE ucC3Unit = 0;
    NBB_ULONG ulLagPortId = 0;
    NBB_BYTE ucLagSlotCur = 0;
    NBB_USHORT usLagPortCur = 0;

    if((NULL == pstLag) || (NULL == pstLag->lag_cfg_cb))
    {
        ret = UPDATE_NLB_PORT_INPUT_POINTER_NULL;
        return ret;
    }

    //��ʼ���ṹ��
    OS_MEMSET(&stTrunk, 0, sizeof(LAG_T));
    OS_MEMSET(&stApsPortLagInfo, 0, sizeof(APS_PORT_LAG_INFO));
    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    lag_key = pstLag->lag_id_key;
    ulLagPortId = lag_key + 0x80;

    if(NULL == pstLag->nlb_port_cfg_cb)
    {
        ret = UPDATE_NLB_PORT_NLB_CFG_NULL;
        return ret;    
    }
    
    //���Ϊ�ֹ��Ǹ��طֵ���������lacpЭ��״̬
    if ((pstLag->lag_cfg_cb != NULL) && (pstLag->lag_cfg_cb->mode == ATG_DCI_MANUAL_NONE_LOADBALANCE))
    {       
        stApsPortLagInfo.lagType = pstLag->lag_cfg_cb->mode;
        stApsPortLagInfo.retType = pstLag->nlb_port_cfg_cb->ret_mode;
        stApsPortLagInfo.wtrTime = pstLag->nlb_port_cfg_cb->restoration_time;

        //����Ա�˿�, ��ȡ��λ�Ͷ˿���Ϣ
    	if (pstLag->nlb_port_cfg_cb->master_port_index != 0)
    	{ 
            ret = spm_get_portid_from_logical_port_index(pstLag->nlb_port_cfg_cb->master_port_index, 
                                                    &stPortInfo NBB_CCXT);
            if(SUCCESS != ret)
            {
                SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,ret=%d\n",
                                    GET_PORTID_FROM_LOG_PORT_ERROR_3,ret);
                ret = GET_PORTID_FROM_LOG_PORT_ERROR_3;
                return ret;
            }
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].slot = stPortInfo.slot_id;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].portId = stPortInfo.port_id;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].portIndex 
                = pstLag->nlb_port_cfg_cb->master_port_index;
            
            stApsPortLagInfo.portNum++;
            
            usSlotPort = ((stPortInfo.slot_id & 0xffff) << 8) + stPortInfo.port_id;

            stTrunk.curSlotPort[stTrunk.curMemberPortNum] = usSlotPort;
            stTrunk.cfgSlotPort[stTrunk.cfgMemberPortNum] = usSlotPort;
            stTrunk.cfgMemberPortNum++;
            stTrunk.curMemberPortNum++;
            
    	}

        //�ӳ�Ա�˿�, ��ȡ��λ�Ͷ˿���Ϣ
        if (pstLag->nlb_port_cfg_cb->slave_port_index != 0)	//�ӳ�Ա�˿�, ��ȡ��λ�Ͷ˿���Ϣ
    	{
    	    OS_MEMSET(&stPortInfo,0,sizeof(SPM_PORT_INFO_CB));
            ret = spm_get_portid_from_logical_port_index(pstLag->nlb_port_cfg_cb->slave_port_index, &stPortInfo NBB_CCXT);
            if(SUCCESS != ret)
            {
                SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,ret=%d\n",GET_PORTID_FROM_LOG_PORT_ERROR_4,ret);
                ret = GET_PORTID_FROM_LOG_PORT_ERROR_4;
                return ret;
            }
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].slot = stPortInfo.slot_id;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].portId = stPortInfo.port_id;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].portIndex = pstLag->nlb_port_cfg_cb->slave_port_index;
            stApsPortLagInfo.portNum++;
            
            usSlotPort = ((stPortInfo.slot_id & 0xffff) << 8) + stPortInfo.port_id;
            
            stTrunk.cfgSlotPort[stTrunk.cfgMemberPortNum] = usSlotPort;

            if(0 == stTrunk.curMemberPortNum)
            {
                stTrunk.curSlotPort[stTrunk.curMemberPortNum] = usSlotPort;
                stTrunk.curMemberPortNum++;
            }
            stTrunk.cfgMemberPortNum++;            
    	}
        stTrunk.lagId = lag_key;

        if (0 == stTrunk.curMemberPortNum)
        {
            //ˢ�»ص�������
            stApsPortLagInfo.portNum = 0;
        }
         else
         {        
             //ִ������
             stTrunk.lagId = lag_key;
#ifdef SPU	
             for (ucC3Unit = 0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
             { 
                 ret += ApiC3SetLag(ucC3Unit, &stTrunk);
             }

             if(SUCCESS != ret)
             {
                SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiC3SetLag error r1=%d,ret=%d\n",
                                            SET_C3_NLB_LAG_PORT_INFO_FAILD,ret);
                ret = SET_C3_NLB_LAG_PORT_INFO_FAILD;
                return ret;
             }
#endif             
            }

           
    }

    //���ΪLACP��̬�Ǹ��طֵ���Ҫ����lacpЭ��״̬
    else if ((pstLag->lag_cfg_cb != NULL) && (pstLag->lag_cfg_cb->mode == ATG_DCI_LACP_STATIC_NONE_LOADBALANCE))
    {
        //��������Ҫ�󣬴�Э��״̬
        stApsPortLagInfo.lagType = pstLag->lag_cfg_cb->mode;
        stApsPortLagInfo.retType = pstLag->nlb_port_cfg_cb->ret_mode;
        stApsPortLagInfo.wtrTime = pstLag->nlb_port_cfg_cb->restoration_time;

        //����Ա�˿�, ��ȡ��λ�Ͷ˿���Ϣ
        if(pstLag->nlb_port_cfg_cb->master_port_index != 0)
        {
            ret = spm_get_portid_from_logical_port_index(pstLag->nlb_port_cfg_cb->master_port_index, &stPortInfo NBB_CCXT);
            if(SUCCESS != ret)
            {
                SPM_L2_ERROR_PRINT_LOG(uc_message,"spm_get_portid_from_logical_port_index error r1=%d,ret=%d\n",
                                    GET_PORTID_FROM_LOG_PORT_ERROR_5,ret);
                ret = GET_PORTID_FROM_LOG_PORT_ERROR_5;
                return ret;
            }

            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].slot = stPortInfo.slot_id;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].portId = stPortInfo.port_id;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].cardId = 0;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].lacpState 
              = pstLag->nlb_port_cfg_cb->master_port_consult;

            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].portIndex 
              = pstLag->nlb_port_cfg_cb->master_port_index;

            stApsPortLagInfo.portNum++;

            usSlotPort = ((stPortInfo.slot_id & 0xffff) << 8) + stPortInfo.port_id;

            stTrunk.cfgSlotPort[stTrunk.cfgMemberPortNum] = usSlotPort;
            stTrunk.cfgMemberPortNum++;

            stTrunk.curSlotPort[stTrunk.curMemberPortNum] = usSlotPort;
            stTrunk.curMemberPortNum++;
        }	            	
        if (pstLag->nlb_port_cfg_cb->slave_port_index != 0)	//�ӳ�Ա�˿�, ��ȡ��λ�Ͷ˿���Ϣ
        {
            OS_MEMSET(&stPortInfo,0,sizeof(SPM_PORT_INFO_CB));
            ret = spm_get_portid_from_logical_port_index(pstLag->nlb_port_cfg_cb->slave_port_index,
            &stPortInfo NBB_CCXT);
            if(SUCCESS != ret)
            {
                SPM_L2_ERROR_PRINT_LOG(uc_message,"spm_get_portid_from_logical_port_index error r1=%d,ret=%d\n",
                                    GET_PORTID_FROM_LOG_PORT_ERROR_6,ret);
                ret = GET_PORTID_FROM_LOG_PORT_ERROR_5;
                return ret;
            }

            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].slot = stPortInfo.slot_id;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].portId = stPortInfo.port_id;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].cardId = 0;
            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].lacpState 
            	= pstLag->nlb_port_cfg_cb->slave_port_consult;

            stApsPortLagInfo.lagPort[stApsPortLagInfo.portNum].portIndex 
            	= pstLag->nlb_port_cfg_cb->slave_port_index;

            stApsPortLagInfo.portNum++;

            usSlotPort = ((stPortInfo.slot_id & 0xffff) << 8) + stPortInfo.port_id;

            stTrunk.cfgSlotPort[stTrunk.cfgMemberPortNum] = usSlotPort;
            stTrunk.cfgMemberPortNum++;

             if(0 == stTrunk.curMemberPortNum)
             {
                 stTrunk.curSlotPort[stTrunk.curMemberPortNum] = usSlotPort;
                 stTrunk.curMemberPortNum++;
             }         
        }
                 
        //����˿ںͲ�λ��Ϊ0��˵��û�ж˿���Ҫ�ۺ�
        if (0 == stTrunk.curMemberPortNum)
        {
	        //ˢ�»ص�������
	        stApsPortLagInfo.portNum = 0;
         
        }
        else
	    {

	         //ִ������
	         stTrunk.lagId = lag_key;
			 
#ifdef SPU
             for (ucC3Unit = 0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
             {
                 ret += ApiC3SetLag(ucC3Unit, &stTrunk);
             }

             if(SUCCESS != ret)
             {
                SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiC3SetLag error r1=%d,ret=%d\n",
                                            SET_C3_NLB_LAG_PORT_INFO_FAILD_2,ret);
                ret = SET_C3_NLB_LAG_PORT_INFO_FAILD_2;
                return ret;
             }
			 
#endif
	    }
    }


#ifdef SPU
    
    ret = aps_update_lag_port_info(lag_key, &stApsPortLagInfo);
    if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"aps_update_lag_port_info error r1=%d,ret=%d\n",
                                    UPDATE_APS_NLB_LAG_PORT_INFO_FAILD,ret);
        ret = UPDATE_APS_NLB_LAG_PORT_INFO_FAILD;
        return ret;
    }
    
#endif

    //д��������ֵ
    pstLag->port_num = stTrunk.cfgMemberPortNum;
    spm_get_lag_min_slot_port(lag_key, &ucLagSlotCur, &usLagPortCur NBB_CCXT);
    pstLag->lag_port_id = usLagPortCur;
    pstLag->lag_slot_id = ucLagSlotCur;

    //���Ӹ澯ע��
    if((pstLag->lag_slot_id == SHARED.local_slot_id)
        && ((ATG_DCI_MANUAL_NONE_LOADBALANCE == pstLag->lag_cfg_cb->mode)
        || (ATG_DCI_LACP_STATIC_NONE_LOADBALANCE == pstLag->lag_cfg_cb->mode)))
    {
#ifdef SPU
        //���������
        //spm_add_lag_alarm(lag_key);
#endif 
    }
   
    return ret;
}


/*****************************************************************************
�� �� ��  : spm_lag_drv_set_ipv4_nf_cfg
��������  : ����IPV4����������
             
�������  : SPM_LAG_CB *pstLag
�������  : 
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��2��25��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_lag_drv_set_ipv4_nf_cfg(SPM_LAG_CB *pstLag)
{
    NBB_INT ret = SUCCESS;
    SUB_PORT stSubPort;
    NBB_ULONG lag_key = 0;

    if(NULL == pstLag)
    {
        ret = SET_IPV4_NF_INPUT_POINTER_NULL;
        return ret;
    }

    OS_MEMSET(&stSubPort,0,sizeof(SUB_PORT));

    lag_key = pstLag->lag_id_key;
#ifdef SPU
    stSubPort.port = lag_key + ATG_DCI_LAG_OFFSET;
    stSubPort.slot = 0;
    stSubPort.unit = 0;

    if(NULL == pstLag->ipv4_nf_data)
    {
        ret = SET_IPV4_NF_DATA_NULL;
        return ret;
    }
    
    ret = spm_set_lag_ipv4_nf_enable(&stSubPort, pstLag->ipv4_nf_data);
    if(ATG_DCI_RC_OK != ret)
    {
        ret = SET_IPV4_NF_ENABLE_FAILD;
        return ret;
    } 
#endif    
    return ret;
}


/*****************************************************************************
�� �� ��  : spm_lag_drv_set_ipv6_nf_cfg
��������  : ����IPV6����������
             
�������  : SPM_LAG_CB *pstLag
�������  : 
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��2��25��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_lag_drv_set_ipv6_nf_cfg(SPM_LAG_CB *pstLag)
{
    NBB_INT ret = SUCCESS;
    SUB_PORT stSubPort;
    NBB_ULONG lag_key = 0;

    if(NULL == pstLag)
    {
        ret = SET_IPV6_NF_INPUT_POINTER_NULL;
        return ret;
    }

    OS_MEMSET(&stSubPort,0,sizeof(SUB_PORT));

    lag_key = pstLag->lag_id_key;
#ifdef SPU
    stSubPort.port = lag_key + ATG_DCI_LAG_OFFSET;
    stSubPort.slot = 0;
    stSubPort.unit = 0;

    if(NULL == pstLag->ipv6_nf_data)
    {
        ret = SET_IPV6_NF_DATA_NULL;
        return ret;
    }
    
    ret = spm_set_lag_ipv6_nf_enable(&stSubPort, pstLag->ipv6_nf_data);
    if(ATG_DCI_RC_OK != ret)
    {
        ret = SET_IPV6_NF_ENABLE_FAILD;
        return ret;
    } 
#endif    
    return ret;
}


/*****************************************************************************
�� �� ��  : spm_lag_drv_set_ip_sample_cfg
��������  : ����IP��������������
             
�������  : SPM_LAG_CB *pstLag
�������  : 
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��2��25��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_lag_drv_set_ip_sample_cfg(SPM_LAG_CB *pstLag)
{
    NBB_INT ret = SUCCESS;
    SUB_PORT stSubPort;
    NBB_ULONG lag_key = 0;

    if(NULL == pstLag)
    {
        ret = SET_IP_SAMPLE_INPUT_POINTER_NULL;
        return ret;
    }

    OS_MEMSET(&stSubPort,0,sizeof(SUB_PORT));

    lag_key = pstLag->lag_id_key;
#ifdef SPU
    stSubPort.port = lag_key + ATG_DCI_LAG_OFFSET;
    stSubPort.slot = 0;
    stSubPort.unit = 0;
    if(NULL == pstLag->ip_sample_data)
    {
        ret = SET_IP_SAMPLE_DATA_NULL;
        return ret;
    }
    
    ret = spm_set_lag_sampler(&stSubPort, pstLag->ip_sample_data);
    if(SUCCESS != ret)
    {
        ret = SET_IP_SAMPLE_ENABLE_FAILD;
        return ret;    
    }
#endif                    

    return ret; 
}

/*****************************************************************************
�� �� ��  : spm_lag_drv_del_lag_all
��������  : ����lag�������ã�ɾ��C3�е�lag��Ϣ
             
�������  : NBB_ULONG ulLagIdKey
�������  : APS_PORT_LAG_INFO stApsPortLagInfo
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��2��25��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_lag_drv_del_lag_all(NBB_ULONG ulLagIdKey)
{
    NBB_INT ret = SUCCESS;
#ifdef SPU    
    NBB_BYTE ucC3Unit = 0;
    APS_PORT_LAG_INFO stApsPortLagInfo;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    OS_MEMSET(&stApsPortLagInfo, 0, sizeof(APS_PORT_LAG_INFO));
    
    stApsPortLagInfo.portNum = 0;
    ret = aps_update_lag_port_info(ulLagIdKey, &stApsPortLagInfo);
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",DEL_LAG_UPDATE_APS_ERR,ret);
		return DEL_LAG_UPDATE_APS_ERR;
	}

    for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3DelLag(ucC3Unit, ulLagIdKey);
    }

    if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,ret=%d\n",
                                            C3_DEL_LAG_PORT_INFO_ERROR,ret);
        ret = C3_DEL_LAG_PORT_INFO_ERROR;
        return ret;
    }
#endif
    return ret;
    
}


/*****************************************************************************
�� �� ��  : spm_lag_drv_set_lag_attri
��������  : ����C3�е�mac��ַ
             
�������  : NBB_ULONG ulLagIdKey
�������  : APS_PORT_LAG_INFO stApsPortLagInfo
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��2��25��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_lag_drv_set_lag_attri(NBB_ULONG ulLagKey, ATG_DCI_LINK_AGGR_LAG_CONF *pstLagData)
{
    NBB_INT ret = SUCCESS;
#ifdef SPU    
    NBB_BYTE ucC3Unit = 0;
    NBB_ULONG ulLagPortId = 0;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};

    if(NULL == pstLagData)
    {
        ret = LAG_SET_C3_MAC_INPUT_POINTER_NULL;
        return ret;
    }

    ulLagPortId = ulLagKey + 0x80;
    for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
    {
        //�ֵ��㷨
    	switch (pstLagData->load_sharing_algorithm)
    	{
    		case ATG_DCI_BASE_BOTH_MAC:
                ret = ApiC3SetLagHashAlg(ucC3Unit, ulLagKey, LAG_HASH_DSMAC);
    			break;
    		case ATG_DCI_BASE_BOTH_IP:
                ret = ApiC3SetLagHashAlg(ucC3Unit, ulLagKey, LAG_HASH_DSIP);
    			break;
    		case ATG_DCI_BASE_PACKET:
                ret = ApiC3SetLagHashAlg(ucC3Unit, ulLagKey, LAG_PORT_ROUND_ROBIN);
    			break;
    		default:
    			break;
    	}

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",SET_LAG_HASH_ALG_ERR,ret);
			return SET_LAG_HASH_ALG_ERR;
		}

    	ret = ApiC3SetPortMac(ucC3Unit, ATG_DCI_IS_NOT_VE, ulLagPortId, pstLagData->lag_mac);
		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",SET_LAG_MAC_ERR,ret);
			return SET_LAG_MAC_ERR;
		}
    }
#endif
    return ret;
    
}

/*****************************************************************************
   �� �� ��  : spm_add_trigger_lag_port_info
   ��������  : ��lag�ĳ�Ա�˿ڶ�Ӧ������ڼ��������Ĵ����ӿ���
   �������  : 
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ    :
   1.��    ��  : 2014��5��20��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_add_trigger_lag_port_info(NBB_BYTE pucPortNum, NBB_ULONG ulAssociateIfKey, 
                                        SPM_LAG_PORT_INFO_CB stTriggerLagPortInfo[])
{
    NBB_INT ret = SUCCESS;
    NBB_USHORT usPortId = 0;
    NBB_BYTE usSlotId = 0;
    PORT_GROUP_INFO IfGoupInfo;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    NBB_BYTE i = 0;

    for(i = 0;i < pucPortNum;i++)
	{
		usPortId = stTriggerLagPortInfo[i].port_id;
		usSlotId = stTriggerLagPortInfo[i].slot_id;

		OS_MEMSET(&IfGoupInfo,0,sizeof(PORT_GROUP_INFO));

		IfGoupInfo.usSlotPort = (usSlotId << 8)+(usPortId&0xff);
		IfGoupInfo.lacpState = stTriggerLagPortInfo[i].port_consult;
		IfGoupInfo.actionType = 0;
        IfGoupInfo.cardId = stTriggerLagPortInfo[i].card_id;
		
		ret += aps_add_monitor_group_port(ulAssociateIfKey,&IfGoupInfo);	
	}

    if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"aps add monitor group port error r1=%d,ret=%d\n",
                                            APS_ADD_MINITOR_GROUP_PORT_ERROR,ret);
        ret = APS_ADD_MINITOR_GROUP_PORT_ERROR;
        return ret;        
    }

    return ret;

}


/*****************************************************************************
   �� �� ��  : spm_add_trigger_lag_port_info
   ��������  : ��lag�ĳ�Ա�˿ڶ�Ӧ������ڼ�������鶯���ӿ���
   �������  : 
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ    :
   1.��    ��  : 2014��5��20��
    ��    ��   : huxl
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_add_ifgoup_lag_port_info(NBB_BYTE pucPortNum, NBB_ULONG ulAssociateIfKey, 
                                        SPM_LAG_PORT_INFO_CB stActLagPortInfo[])
{
    NBB_INT ret = SUCCESS;
    NBB_USHORT usPortId = 0;
    NBB_BYTE usSlotId = 0;
    PORT_GROUP_INFO IfGoupInfo;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    NBB_BYTE i = 0;
    
    for(i = 0;i < pucPortNum;i++)
    {
    	usPortId = stActLagPortInfo[i].port_id;
    	usSlotId = stActLagPortInfo[i].slot_id;

    	OS_MEMSET(&IfGoupInfo,0,sizeof(PORT_GROUP_INFO));

    	IfGoupInfo.usSlotPort = (usSlotId << 8)+(usPortId&0xff);
    	IfGoupInfo.lacpState = stActLagPortInfo[i].port_consult;
    	IfGoupInfo.actionType = 1;
        IfGoupInfo.cardId = stActLagPortInfo[i].card_id;
    	
    	ret += aps_add_monitor_group_port(ulAssociateIfKey,&IfGoupInfo);	
    }

    if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"aps add monitor group port error r1=%d,ret=%d\n",
                                            APS_ADD_MINITOR_GROUP_PORT_ERROR_2,ret);
        ret = APS_ADD_MINITOR_GROUP_PORT_ERROR_2;
        return ret;        
    }
    
    return ret;

}

/*****************************************************************************
   �� �� ��  : spm_set_lag_min_member
   ��������  :
   �������  : 
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ    :
   1.��    ��  : 
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_set_lag_min_member(SPM_LAG_CB *pst_lag)
{
	NBB_INT ret = SUCCESS;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};

	if((NULL == pst_lag) || (NULL == pst_lag->mc_data))
	{
		return SET_MIN_LAG_MEMBER_LAG_CFG_NULL;
	}

	//ret = aps_set_lag_min_port_info(pst_lag->lag_id_key, pst_lag->mc_data->mem_num);
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",SET_APS_LAG_MIN_MEMBER_NUM_ERR,ret);
		return SET_APS_LAG_MIN_MEMBER_NUM_ERR;
	}

	return ret;

}


/*****************************************************************************
   �� �� ��  : spm_set_lag_enable
   ��������  : ����LAG��ʹ�ܣ������� APSģ��
   �������  : 
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ    :
   1.��    ��  : 
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_set_lag_enable(NBB_ULONG lag_id,NBB_BYTE enable)
{
	SPM_TERMINAL_IF_CB *pst_terminal_if_cb = NULL;
	SPM_LOGICAL_PORT_CB *pst_tmp_logical_port = NULL;
	SPM_LOGICAL_PORT_CB *pst_logical_port = NULL;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
	NBB_INT ret = 0;
	NBB_INT c3_unit = 0;

	#ifdef SPU

	//����lag������
	for(c3_unit = UNIT_0;c3_unit < v_spm_shared->c3_num;++c3_unit)
	{
		//ret = fhdrv_psn_l3_set_lag_block(c3_unit,lag_id,enable);
		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",SET_LAG_BLOCK_ERR,ret);
			return SET_LAG_BLOCK_ERR;
		}
		
	}
	

	for(pst_tmp_logical_port = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(v_spm_shared->logical_port_tree);
        pst_tmp_logical_port != NULL;
        pst_tmp_logical_port = (SPM_LOGICAL_PORT_CB*) AVLL_NEXT(v_spm_shared->logical_port_tree,
        pst_tmp_logical_port->spm_logical_port_node))
    {	
    	if((NULL != pst_tmp_logical_port->basic_cfg_cb)
			&& (NULL != pst_tmp_logical_port->phy_cfg_cb)
			&& (ATG_DCI_LAG == pst_tmp_logical_port->basic_cfg_cb->port_sub_type)
			&& (lag_id == pst_tmp_logical_port->phy_cfg_cb->lag_id))
    		{
    			pst_logical_port = pst_tmp_logical_port;
				break;
    		}
    	
    }

	if(NULL == pst_logical_port)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",SET_LAG_ENABLE_LOGICAL_NULL);
		return SET_LAG_ENABLE_LOGICAL_NULL;
	}

	
	
	for (pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(pst_logical_port->terminal_if_tree);
        pst_terminal_if_cb != NULL;
        pst_terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(pst_logical_port->terminal_if_tree,
        pst_terminal_if_cb->spm_terminal_if_node))
    {
    	if(0 != pst_terminal_if_cb->intf_pos_id)
    	{
    		//ret = fhdrv_psn_l3_set_intf_lag_block(pst_logical_port->logic_port_info_cb.unit_id, 
			//	pst_terminal_if_cb->intf_pos_id,
			//	enable);

			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",SET_INTF_LAG_BLOCK_ERR,ret);
				return SET_INTF_LAG_BLOCK_ERR;
			}

    	}
    }

	#endif

	return ret;
}


//�澯�ϱ��������
#if 0
/*****************************************************************************
   �� �� ��  : spm_reg_lag_alm
   ��������  :lag�澯ע�ắ��
   �������  : 
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��5��20��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_reg_lag_alm()
{
    #ifdef SPU
    NBB_BYTE ucAlarmNum = 0;
    NBB_UINT ucLagAlarm[] = {
        ALM_SWR,
        ALM_BRIDGE,
        ALM_SWTR,
    };

    //ע�����ܸ澯
    
    ucAlarmNum = sizeof(ucLagAlarm) / sizeof(NBB_UINT);
    almpm_reg_alminfo(LINEKEY_TYPE_LAG, ucAlarmNum, ucLagAlarm, spm_get_lag_alarm);
    #endif
}

/*****************************************************************************
   �� �� ��  : spm_add_lag_alarm
   ��������  :lag�����·�ʱ��ע��һ���澯��Ŀ
   �������  : 
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��5��20��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_add_lag_alarm(NBB_ULONG lag_id)
{
    #ifdef SPU
    NBB_INT ret = SUCCESS;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    
    ALMPM_LINE_STRUCT stLagAlarmLine;
    OS_MEMSET(&stLagAlarmLine,0,sizeof(ALMPM_LINE_STRUCT));

    //�����ܸ澯ģ�鴫����
    stLagAlarmLine.usLineLayer = LINELARYER_LAG;
   
    stLagAlarmLine.usLineType = LINETYPE_LAG;
   
    stLagAlarmLine.ucKeyLen = sizeof(lag_id);
    OS_MEMCPY(&stLagAlarmLine.ucKeyValue,&lag_id,stLagAlarmLine.ucKeyLen);
    
    
    ret = almpm_cfg(stLagAlarmLine,0,0,1);

    //coverity[dead_error_condition]
    if(SUCCESS != ret)
    {
        
    }
    #endif

}

/*****************************************************************************
   �� �� ��  : spm_get_lag_alarm
   ��������  :lag�澯��ȡ����
   �������  : 
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��5��4��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
int spm_get_lag_alarm (IN unsigned char *ucKey, 
    IN unsigned char ucKeyLen,  
    IN unsigned int ucAlmNum,
    OUT unsigned char *ucAlmValue)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_ULONG  lag_id = 0;
    SPM_LAG_CB *pstLag = NULL;
    NBB_INT ret = SUCCESS;
    LPS_STATE_ALM_ST stLagAlarm;
    
    if((NULL == ucKey) || (NULL == ucAlmValue))
    {
        OS_PRINTF("  ***ERROR***:spm_get_msrp_alarm(pointer==NULL)\n");

        OS_SPRINTF(ucMessage, "  ***ERROR***:spm_get_msrp_alarm(pointer==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", 0,
                FUNC_IN_PARAM_IS_NULL,"FUNC_IN_PARAM_IS_NULL",ucMessage));
        return ERROR;
    }

    lag_id =  *(NBB_ULONG *)ucKey;

    pstLag = AVLL_FIND(v_spm_shared->lag_tree,&lag_id);

    if(NULL == pstLag)
    {
        OS_PRINTF("  ***ERROR***(%s:%d):ID=%ld��lag����Ϊ��\n",__FILE__,__LINE__,lag_id);

        OS_SPRINTF(ucMessage, "  ***ERROR***(%s:%d):ID=%ld��lag����Ϊ��\n",__FILE__,__LINE__,lag_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LAG_PD, 0, "ld d s s", 0,
                FUNC_IN_PARAM_IS_NULL,"FUNC_IN_PARAM_IS_NULL",ucMessage));
        
        return ERROR;
    }

    if((NULL != pstLag->lag_cfg_cb)
        && (NULL != pstLag->nlb_port_cfg_cb)
        && ((ATG_DCI_MANUAL_NONE_LOADBALANCE == pstLag->lag_cfg_cb->mode)
        || (ATG_DCI_LACP_STATIC_NONE_LOADBALANCE == pstLag->lag_cfg_cb->mode)))
    {
        
#if 1
        OS_MEMSET(&stLagAlarm,0,sizeof(LPS_STATE_ALM_ST));

        if(v_spm_shared->local_slot_id == pstLag->lag_slot_id)
        {
            ret = aps_get_lag_state_alm(lag_id,&stLagAlarm);
        }
        
        if(SUCCESS != ret)
        {
            OS_PRINTF("  ***ERROR***(%s:%d):aps_get_msrp_state_alm(%ld)ʧ�ܣ�ret=%d\n",
                __FILE__,__LINE__,lag_id,ret);
    
            OS_SPRINTF(ucMessage, "  ***ERROR***(%s:%d):aps_get_msrp_state_alm(%ld)ʧ�ܣ�ret=%d\n",
                __FILE__,__LINE__,lag_id,ret);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
    
            NBB_EXCEPTION((PCT_SPM | MSP_PD, 0, "ld d s s", 0,
                    FUNC_IN_PARAM_IS_NULL,"FUNC_IN_PARAM_IS_NULL",ucMessage));
            
            return ERROR;
           
        }
        
        *(ucAlmValue++) = stLagAlarm.almSwr;//����

        *(ucAlmValue++) = stLagAlarm.almBridge;//�Ž�

        *(ucAlmValue++) = stLagAlarm.almSwtr;//�ȴ��ָ�
        
#endif
    }

    return SUCCESS;
}

/*****************************************************************************
   �� �� ��  : spm_del_lag_alarm
   ��������  :lag����ɾ��ʱ��ע��һ���澯��Ŀ
   �������  : 
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2014��5��20��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_del_lag_alarm(NBB_ULONG lag_id)
{
#ifdef SPU
    NBB_INT ret = SUCCESS;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    
    ALMPM_LINE_STRUCT stLagAlarmLine;
    OS_MEMSET(&stLagAlarmLine,0,sizeof(ALMPM_LINE_STRUCT));

    //�����ܸ澯ģ�鴫����
    stLagAlarmLine.usLineLayer = LINELARYER_LAG;
   
    stLagAlarmLine.usLineType = LINETYPE_LAG;
   
    stLagAlarmLine.ucKeyLen = sizeof(lag_id);
    OS_MEMCPY(&stLagAlarmLine.ucKeyValue,&lag_id,stLagAlarmLine.ucKeyLen);
    
    
    ret = almpm_cfg(stLagAlarmLine,0,0,0);
    
    if(SUCCESS != ret)
    {
		OS_PRINTF("	***ERROR***:almpm_cfgʧ�ܣ�ret = %d\n!", ret);

		OS_SPRINTF(ucMessage, "	***ERROR***:almpm_cfgʧ�ܣ�ret = %d\n!", ret);
		BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

		NBB_EXCEPTION((PCT_SPM | LAG_PD, 0, "ld d s s", lag_id, 
            ALLOC_MEMORY_ERROR, "ALLOC_MEMORY_ERROR", ucMessage));

    }
#endif
}

#endif

