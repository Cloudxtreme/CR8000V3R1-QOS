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
�� �� ��  : spm_set_vsi_igmp_enable
��������  : ɾ��vsiʵ���ϵ�igmp
             unsigned short vsi,unsigned int limit
�������  : 
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��1��22��
��    ��   : zhangzhm
�޸�����   : �����ɺ���

*****************************************************************************/
int spm_set_vsi_igmp_enable(NBB_USHORT vpls_id, NBB_BYTE c3_num, int enable)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE unit = 0;
    
#ifdef SPU
    for(unit = 0; unit < c3_num; unit++)
    {
        //ret = ApiC3SetVsiIgmpEnable(unit,vpls_id,enable);
    }
#endif

    return ret;
}


/*****************************************************************************
�� �� ��  : spm_vpls_del_mc
��������  : vpls����ɾ����ʱ��ɾ�����鲥
             
�������  : 
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��1��22��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
int spm_vpls_del_mc(NBB_USHORT mc_id)
{
    NBB_INT ret = SUCCESS;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};

	SPM_L2_ERROR_PRINT_LOG(uc_message,"del mc,mc_id=%d\n",mc_id);

    //�����鲥��ɾ������
#if ((defined SNU) || (defined SRC))

    ret = ApiDfeMulticastDestroy(UNIT_0, mc_id);
    if(SUCCESS != ret)
    {
        //��¼������Ϣ        
        SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiDfeMulticastDestroy error r1=%d,r2=%d\n",
                                            DEL_VPLS_SNU_SRC_MC_ERR,ret);
        ret = DEL_VPLS_SNU_SRC_MC_ERR;
        return ret;
    }

#endif
    
#ifdef SPU

    ret = ApiAradDeleteMc(UNIT_0, mc_id);
    if(SUCCESS != ret)
    {
        //��¼������Ϣ
        SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiAradDeleteMc error r1=%d,r2=%d\n",
                                            DEL_VPLS_SPU_MC_ERR,ret);
        ret = DEL_VPLS_SNU_SRC_MC_ERR;
        return ret;  
    }

#endif
    
    return ret;
}


/*****************************************************************************
�� �� ��  : spm_vpls_create_mc
��������  : vpls�����鲥
             
�������  : 
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��1��22��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
int spm_vpls_create_mc(NBB_USHORT mc_id)
{
    NBB_INT ret = SUCCESS;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
	NBB_INT i = 0;

	SPM_L2_ERROR_PRINT_LOG(uc_message,"create mc,mc_id=%d\n",mc_id);

#if ((defined SNU) || (defined SRC))

    ret = ApiDfeMulticastCreate(UNIT_0, mc_id);
    if(SUCCESS != ret)
    {
        //��¼������Ϣ����־
        SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiDfeMulticastCreate error r1=%d,r2=%d\n",
                                            SRC_SNU_CREATE_MC_ERR,ret);
        ret = SRC_SNU_CREATE_MC_ERR;
        return ret;
    }
	

#endif

#ifdef SPU
    ret = ApiAradSetupEngressMc(UNIT_0, mc_id);
    if(SUCCESS != ret)
    {
        //��¼������Ϣ����־
        SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiDfeMulticastCreate error r1=%d,r2=%d\n",
                                            SPU_CREATE_MC_ERR,ret);
        ret = SRC_SNU_CREATE_MC_ERR;
        return ret;
    }
#endif
	
    return ret;
}


/*****************************************************************************
�� �� ��  : spm_vpls_mac_learning
��������  : vpls�����鲥
             
�������  : 
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��1��22��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
int spm_vpls_mac_learning(NBB_USHORT vpls_id, NBB_ULONG mac_learn_limit)
{
    NBB_INT ret = SUCCESS;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    NBB_BYTE c3_unit_id = 0;

 #ifdef SPU

    for (c3_unit_id = 0; c3_unit_id < SHARED.c3_num; c3_unit_id++)
    {
        ret += ApiC3SetL2MacLimitByVid(c3_unit_id, vpls_id, mac_learn_limit);
    }
    if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiC3SetL2MacLimitByVid error r1=%d,r2=%d\n",
                                            SPU_CLEARN_MAC_ERR,ret);
        ret = SPU_CLEARN_MAC_ERR;    
    }
 
 #endif 
   
    return ret;
}

/*****************************************************************************
�� �� ��  : spm_vpls_arad_update_egress_mc
��������  : ����arad�е��鲥
             
�������  : ARAD_MC_INFO stMcInfo NBB_INT iMcMemberNum NBB_USHORT usMcId
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��1��22��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
int spm_vpls_arad_update_egress_mc(ARAD_MC_INFO *arad_mc_info, NBB_INT mc_member_num, NBB_USHORT mc_id)
{
    NBB_INT ret = SUCCESS;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};

#ifdef  SPU
    arad_mc_info->member_num = mc_member_num;
	arad_mc_info->multicast_id = mc_id;

    ret = ApiAradUpdateEgressMc(UNIT_0, &arad_mc_info);
    if (ret != SUCCESS)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiAradUpdateEgressMc error r1=%d,r2=%d\n",
                                            VPLS_REFRESH_ARAD_C3_API_UPDATE_MC_ERR,ret);
        ret = VPLS_REFRESH_ARAD_C3_API_UPDATE_MC_ERR;
        return ret;
    }
#endif

    return ret;
}


/*****************************************************************************
�� �� ��  : spm_vpls_dfe_mc_set
��������  : ����arad�е��鲥
             
�������  : ARAD_MC_INFO stMcInfo NBB_INT iMcMemberNum NBB_USHORT usMcId
�������  : ��
�� �� ֵ  : 
���ú���  : 
��������  : 

�޸���ʷ      :
1.��    ��   : 2016��1��22��
��    ��   : huxl
�޸�����   : �����ɺ���

*****************************************************************************/
int spm_vpls_dfe_mc_set(SPM_VPLS_CB *pst_vpls, NBB_USHORT mc_id)
{
    NBB_INT slot_group[SPM_SLOT_MAX_NUM] = {0};
    NBB_INT i = 0;
    NBB_INT slot_num = 0;
    NBB_INT dev_slot_num = 0;
    NBB_INT ret = SUCCESS;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN] = {0};
    
#if ((defined SNU) || (defined SRC))
    for (i = 0; i < SPM_SLOT_MAX_NUM; i++)
	{
		slot_num = spm_get_vpls_slot_num(pst_vpls, i + 1 NBB_CCXT);

		if (slot_num != 0)
		{
			slot_group[dev_slot_num++] = i + 1;
		}
	}
    ret = ApiDfeMulticastSet(UNIT_0, mc_id, dev_slot_num, slot_group);

    if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(uc_message,"ApiDfeMulticastSet error r1=%d,ret=%d,mc_id=%d,dev_slot_num=%d\n",
                             VPLS_REFRESH_ARAD_C3_API_SET_MC_ERR,ret,mc_id,dev_slot_num);
        ret = VPLS_REFRESH_ARAD_C3_API_SET_MC_ERR;
        return ret;
    }
    
#endif

    return ret;
}



