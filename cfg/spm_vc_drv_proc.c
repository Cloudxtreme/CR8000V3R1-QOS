/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_vc_proc.c
  �� �� ��   : ����
  ��    ��   : xiaoxiang
  ��������   : 2012��9��18��
  ����޸�   :
  ��������   : VC�����ô���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2012��9��18��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef VC_DRV

/*vc���ÿ��ӡ����*/
extern unsigned char g_vc_cfg_print_setting;


/*****************************************************************************
 �� �� ��  : spm_vc_drv_add_or_mod_nni_vp
 ��������  : ���vc�Ĵ���������add��modģʽ���ú�����ע��:
            1����ȡӦ�����ã�����������
            2��������ӳɹ��󣬸���vc_info_cb�е�vpidx���Ա�ʾ�����ɹ���
 �������  : NBB_BYTE            vpn_type     
             SPM_VC_CB           *pst_vc_tbl  
             SPM_VC_ADD_TYPE_E   vc_add_type  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��30��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���,���ڵ����������

*****************************************************************************/
NBB_INT spm_vc_drv_add_or_mod_nni_vp(NBB_BYTE vpn_type,NBB_USHORT vp_idx,
    NBB_VOID *pst_l2vpn_cfg,SPM_VC_ADD_TYPE_E vc_add_type,NBB_USHORT mc_id,
    SPM_VPN_PORT_INFO_CB *pst_vc_info)
{
	
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    VPORT_T st_nni_vport;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_unit = 0;
    
    SPM_VP_INFO_REFRESH_CB st_vp_info_refresh_main;
	SPM_VP_INFO_REFRESH_CB st_vp_info_refresh_bak;
	
    NBB_ULONG tunnel_id_main = 0;
	NBB_ULONG tunnel_id_bak = 0;
    NBB_ULONG port_index_main = 0;
	NBB_ULONG port_index_bak = 0;
    NBB_USHORT us_frr_group_id = 0;
    NBB_UINT nhi_id = 0;
	NBB_UINT nhi_id_p = 0;
    NBB_USHORT vpn_id = 0;
    SPM_PORT_INFO_CB st_port_info_main;
	SPM_PORT_INFO_CB st_port_info_bak;
	NBB_BYTE prot_flag = 0;//  0/1=�б���/�ޱ���
	NBB_BYTE prot_type = 0;

	//VPWS��VPLS��Ϣ
	SPM_VPWS_CB *pst_vpws = NULL;
	SPM_VPLS_CB *pst_vpls = NULL;
	SPM_VPLS_NNI_CB *pst_vpls_nni =NULL;

	//����VC��Ϣ
	SPM_VC_CB *pst_vc_main = NULL;
	SPM_VC_CB *pst_vc_bak = NULL;
	CRTXLSP_KEY st_crlsp_key_main; 
	CRTXLSP_KEY st_crlsp_key_bak;
    FTN_KEY st_ftn_key_main;
	FTN_KEY st_ftn_key_bak;
	LSPPROT_KEY lspkey_main;
	LSPPROT_KEY lspkey_bak;
	SPM_LINEAR_PROTECT_SWITCH aps_pw_prot;

	
	
    NBB_TRC_ENTRY("spm_vc_add_nni_vp");

	OS_MEMSET(&st_nni_vport, 0, sizeof(VPORT_T));
    OS_MEMSET(&st_crlsp_key_main, 0, sizeof(CRTXLSP_KEY));
	OS_MEMSET(&st_crlsp_key_bak, 0, sizeof(CRTXLSP_KEY));
    OS_MEMSET(&st_ftn_key_main, 0, sizeof(FTN_KEY));
	OS_MEMSET(&st_ftn_key_bak, 0, sizeof(FTN_KEY));
	OS_MEMSET(&lspkey_main, 0, sizeof(LSPPROT_KEY));
	OS_MEMSET(&lspkey_bak, 0, sizeof(LSPPROT_KEY));
    OS_MEMSET(&st_port_info_main, 0, sizeof(SPM_PORT_INFO_CB));
	OS_MEMSET(&st_port_info_bak, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&st_vp_info_refresh_main, 0, sizeof(SPM_VP_INFO_REFRESH_CB));
	OS_MEMSET(&st_vp_info_refresh_bak, 0, sizeof(SPM_VP_INFO_REFRESH_CB));
	OS_MEMSET(&aps_pw_prot,0,sizeof(SPM_LINEAR_PROTECT_SWITCH));

	st_nni_vport.eVpType = VP_NNI;
	st_nni_vport.vpIdx = vp_idx;
	
    if (NULL == pst_l2vpn_cfg)
    {
        ret = ADD_DRV_NNI_L2VPN_IS_NULL;
        goto EXIT_LABEL;

    }

	if(NULL == pst_vc_info)
	{
		ret = ADD_DRV_NNI_VC_INFO_IS_NULL;
        goto EXIT_LABEL;
	}
	
	if(L2VPN_VPWS == vpn_type)
	{
		pst_vpws = (SPM_VPWS_CB *)pst_l2vpn_cfg;
		vpn_id = pst_vpws->vpws_id_key;
		
		pst_vc_main = AVLL_FIND(v_spm_shared->vc_tree,&pst_vpws->vckey_main);
		pst_vc_bak = AVLL_FIND(v_spm_shared->vc_tree,&pst_vpws->vckey_back);
        prot_flag = pst_vpws->use_protect;
		
	}

	else if(L2VPN_VPLS == vpn_type)
	{
		pst_vpls = (SPM_VPLS_CB *)pst_l2vpn_cfg;
		vpn_id = pst_vpls->vpls_id_key;
		
		pst_vpls_nni = AVLL_FIND(pst_vpls->nni_cfg_tree,&vp_idx);

		if(NULL == pst_vpls_nni)
		{
	        ret = ADD_DRV_NNI_VPLS_NNI_CFG_NULL;
	        goto EXIT_LABEL;
		}

		else
		{
			pst_vc_main = AVLL_FIND(v_spm_shared->vc_tree,&pst_vpls_nni->vckey_main);
			pst_vc_bak = AVLL_FIND(v_spm_shared->vc_tree,&pst_vpls_nni->vckey_back);

	        prot_flag = pst_vpls_nni->use_protect;
		}
	}

	//�ӹؼ���Ϣ�Ĵ�ӡ
	SPM_L2_INFO_PRINT_LOG(uc_message,"INFO:add_mod_drv_nni,vpnid=%d.vpntype=%d,vpid=%d,addtype=%d,mc_id=%d\n",
		vpn_id,vpn_type,vp_idx,vc_add_type,mc_id);

	
    //��������
    if ((NULL != pst_vc_main) && (NULL != pst_vc_main->basic_cfg_cb)
		&& (NULL != pst_vc_main->vpn_property_cfg_cb))
    {
        //VLAN ����TAGʶ��
        if (ATG_DCI_TAGGED_VLAN == pst_vc_main->basic_cfg_cb->recv_action)
        {
            st_nni_vport.flags |= VPORT_PTAG_IDENTIFY_ON;
        }

        //VLAN ����TAG��Ϊ ����
        switch (pst_vc_main->basic_cfg_cb->send_tag_action)
        {
            case ATG_DCI_ADD_VID_PRI:
                st_nni_vport.tNextHop.eEgrPtagAction = EGR_ENCAP;
                break;
            case ATG_DCI_DEL_VID_PRI:
                st_nni_vport.tNextHop.eEgrPtagAction = EGR_NO_ENCAP;
                break;
            default:
                break;
        }
	    
        st_nni_vport.tNextHop.egrPtagVlan = pst_vc_main->basic_cfg_cb->send_vlan_id;
        st_nni_vport.tNextHop.egrPtagPri = pst_vc_main->basic_cfg_cb->send_vlan_pri;

        if (pst_vc_main->basic_cfg_cb->tag_type == 0x8100)
        {
            st_nni_vport.tNextHop.eEgrPtagTpidType = TPID_0X8100;
        }
        else if (pst_vc_main->basic_cfg_cb->tag_type == 0x88a8)
        {
            st_nni_vport.tNextHop.eEgrPtagTpidType = TPID_0X88A8;
        }

        if((0 == pst_vc_main->basic_cfg_cb->pw_in_label) || (3 == pst_vc_main->basic_cfg_cb->pw_in_label))
        {
            ret = ADD_DRV_NNI_MAIN_PW_IN_LABEL_ERR;
            goto EXIT_LABEL;                 
        }

        st_nni_vport.matchVcLabel = pst_vc_main->basic_cfg_cb->pw_in_label;

        st_nni_vport.tNextHop.flags |= L2_NEXT_HOP_ENCAP_TUNNEL;

        if (pst_vc_info->next_hop_id != 0)
        {
        	st_nni_vport.tNextHop.nhIdx = pst_vc_info->next_hop_id;
        }
        else
        {
        	ret = spm_vc_alloc_nhi_id(&nhi_id NBB_CCXT);
			{
				if(ret != SUCCESS)
				{
					ret = ADD_DRV_NNI_ALLOC_MAIN_NHI_ID_FAIL;
					goto EXIT_LABEL;
				}
			}
        	st_nni_vport.tNextHop.nhIdx = nhi_id;
        }

        st_nni_vport.tNextHop.tPwLabel.label_un.tlabel.label = pst_vc_main->basic_cfg_cb->pw_out_label;
        st_nni_vport.tNextHop.tPwLabel.label_un.tlabel.bottom = 1;
        st_nni_vport.tNextHop.tPwLabel.label_un.tlabel.ttl = 255; 

		//��tunnel��Ϣ
		ret = spm_vc_get_lspinfo(pst_vc_main,&port_index_main,&tunnel_id_main);

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_MAIN_GET_LSP_INFO_ERR,ret);
			ret = SUCCESS;
		}

		else
		{
			st_nni_vport.tNextHop.tunnelIdx = tunnel_id_main;
			spm_get_portid_from_logical_port_index(port_index_main, &st_port_info_main NBB_CCXT);
		}
		
        //������ʹ�ܡ�
        if (pst_vc_main->basic_cfg_cb->control_word_enable == ENABLE)
        {
            st_nni_vport.flags |= VPORT_CW_SN;
            st_nni_vport.tNextHop.eCwMode = EGR_CW_SEQ;
        }

        //�����VPLSҵ����Ҫ����MCAѧϰ��ˮƽ�ָ�
        if (L2VPN_VPLS == vpn_type)
        {
            //MACѧϰ����
            if (pst_vc_main->basic_cfg_cb->mac_learning_enable == DISABLE)
            {
                st_nni_vport.flags |= VPORT_LEARN_DISABLE;
            }

            //ˮƽ�ָ��
            if (pst_vc_main->basic_cfg_cb->split_horizon_enable == ENABLE)
            {
                st_nni_vport.svp_split_horizon = 1;
            }

            //IGMP
            st_nni_vport.igmpId = pst_vc_main->vpn_property_cfg_cb->virtual_index;
			st_nni_vport.mcId = mc_id;

        }
    }

    //��������
    if ((NULL != pst_vc_bak) && (NULL != pst_vc_bak->basic_cfg_cb)
		&& (NULL != pst_vc_bak->vpn_property_cfg_cb))
    {
    	//to do ֱ�Ӵӱ�����ȡ������?
    	prot_type = pst_vc_bak->vpn_property_cfg_cb->prot_type;
		
        //VLAN ����TAGʶ��
        if (ATG_DCI_TAGGED_VLAN == pst_vc_bak->basic_cfg_cb->recv_action)
        {
            st_nni_vport.flags |= VPORT_BACKUP_PTAG_IDENTIFY_ON;
        }

        //VLAN ����TAG��Ϊ 
        switch (pst_vc_bak->basic_cfg_cb->send_tag_action)
        {
            case ATG_DCI_ADD_VID_PRI:
                st_nni_vport.tNextHop_p.eEgrPtagAction = EGR_ENCAP;
                break;
            case ATG_DCI_DEL_VID_PRI:
                st_nni_vport.tNextHop_p.eEgrPtagAction = EGR_NO_ENCAP;
                break;
            default:
            break;
        }
	    
        st_nni_vport.tNextHop_p.egrPtagVlan = pst_vc_bak->basic_cfg_cb->send_vlan_id;
        st_nni_vport.tNextHop_p.egrPtagPri = pst_vc_bak->basic_cfg_cb->send_vlan_pri;

        if (pst_vc_bak->basic_cfg_cb->tag_type == 0x8100)
        {
            st_nni_vport.tNextHop_p.eEgrPtagTpidType = TPID_0X8100;
        }
        else if (pst_vc_bak->basic_cfg_cb->tag_type == 0x88a8)
        {
            st_nni_vport.tNextHop_p.eEgrPtagTpidType = TPID_0X88A8;
        }

        if((0 == pst_vc_bak->basic_cfg_cb->pw_in_label) || (3 == pst_vc_bak->basic_cfg_cb->pw_in_label))
        {
            ret = ADD_DRV_NNI_BAK_PW_IN_LABEL_ERR;
            goto EXIT_LABEL;                 
        }

        st_nni_vport.matchVcLabel_p = pst_vc_bak->basic_cfg_cb->pw_in_label;
        st_nni_vport.tNextHop_p.flags |= L2_NEXT_HOP_ENCAP_TUNNEL;

        if (pst_vc_info->next_hop_id_p != 0)
        {
        	st_nni_vport.tNextHop_p.nhIdx = pst_vc_info->next_hop_id_p;
        }
        else
        {
        	ret = spm_vc_alloc_nhi_id(&nhi_id_p NBB_CCXT);
			if(ret != SUCCESS)
			{
				ret = ADD_DRV_NNI_ALLOC_BAK_NHI_ID_FAIL;
				goto EXIT_LABEL;
			}
        	st_nni_vport.tNextHop_p.nhIdx = nhi_id_p;
        }

        st_nni_vport.tNextHop_p.tPwLabel.label_un.tlabel.label = pst_vc_bak->basic_cfg_cb->pw_out_label;
        st_nni_vport.tNextHop_p.tPwLabel.label_un.tlabel.bottom = 1;
        st_nni_vport.tNextHop_p.tPwLabel.label_un.tlabel.ttl = 255;

		//��tunnel��Ϣ
		ret = spm_vc_get_lspinfo(pst_vc_bak,&port_index_bak,&tunnel_id_bak);

		if(ret == SUCCESS)
		{
			st_nni_vport.tNextHop_p.tunnelIdx = tunnel_id_bak;
			spm_get_portid_from_logical_port_index(port_index_bak, &st_port_info_bak NBB_CCXT);
		}

		else
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_BAK_GET_LSP_INFO_ERR,ret);
			ret = SUCCESS;
		}

        //������ʹ�ܡ�
        if (pst_vc_bak->basic_cfg_cb->control_word_enable == ENABLE)
        {
            st_nni_vport.flags |= VPORT_CW_SN;
            st_nni_vport.tNextHop_p.eCwMode = EGR_CW_SEQ;
        }

        //�����VPLSҵ����Ҫ����MCAѧϰ��ˮƽ�ָ�
        if (L2VPN_VPLS == vpn_type)
        {
            //MACѧϰ����
            if (pst_vc_bak->basic_cfg_cb->mac_learning_enable == DISABLE)
            {
                st_nni_vport.flags |= VPORT_LEARN_DISABLE;
            }

            //ˮƽ�ָ��
            if (pst_vc_bak->basic_cfg_cb->split_horizon_enable == ENABLE)
            {
                st_nni_vport.svp_split_horizon = 1;
            }

            //IGMP
            st_nni_vport.igmpId = pst_vc_bak->vpn_property_cfg_cb->virtual_index;

        }
		
		//�������PW���ౣ��
        st_nni_vport.eProtType = VP_NHI_FRR;
        
        ret = SUCCESS;

	#if defined SPU || defined PTN690_CES

		//1���б�������������Ҫ����group_id
		//2����: ����ǰ��group_idҲΪ0����Ҫ����group_id
        if ((SPM_VC_ADD_TYPE_ADD == vc_add_type) 
         || ((SPM_VC_ADD_TYPE_MOD == vc_add_type) && (pst_vc_info->frr_group_id == 0)))
        {
            for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
            {
                ret += ApiC3CreateFrrGroup(c3_unit, &us_frr_group_id, 0, 0);
            }

            if (ret == SUCCESS)
            {
                st_nni_vport.frrGroupId = us_frr_group_id;	
				SPM_L2_INFO_PRINT_LOG(uc_message,"OK:create frr=%d\n",us_frr_group_id);
            }
            else
            {	
            	SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",ADD_DRV_NNI_CREATE_FRR_FAIL,ret);
            	ret = ADD_DRV_NNI_CREATE_FRR_FAIL;
                goto EXIT_LABEL;
            }
        }
        else
        {
            st_nni_vport.frrGroupId = pst_vc_info->frr_group_id;
        }    

		#endif
    }

	//�ޱ��������֮ǰ�б�����ɾ����������Ϣ


    //�ϴ��б��������ޱ���
    if ((prot_flag == 0) && (SPM_VC_ADD_TYPE_MOD == vc_add_type) && (pst_vc_info->frr_group_id != 0))
    {

		#if defined SPU || defined PTN690_CES
        for(c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            ret += ApiC3DeleteFrrGroup(c3_unit, pst_vc_info->frr_group_id);

			if(SUCCESS == ret)
			{
				SPM_L2_INFO_PRINT_LOG(uc_message,"OK:del frr=%d\n",us_frr_group_id);
			}

			else
	        {
	        	SPM_L2_ERROR_PRINT_LOG(uc_message,"frr=%d,r1=%d,r2=%d\n",
					pst_vc_info->frr_group_id,ADD_DRV_NNI_DEL_FRR_FAIL,ret);
				ret = ADD_DRV_NNI_DEL_FRR_FAIL;
	            goto EXIT_LABEL;
	        }
			
        }

		#endif

       
    }


	spm_get_aps_pw_prot_info(pst_vc_main,pst_vc_bak,prot_type,
		st_nni_vport.frrGroupId,&aps_pw_prot);

    //�������vc�����ڣ�Ҫ�ù���ʵ���vcɾ����־λ
    if ((NULL == pst_vc_main) || (NULL == pst_vc_main->basic_cfg_cb)
		|| (NULL == pst_vc_main->vpn_property_cfg_cb))
    {
        aps_pw_prot.workEntity.vcDelFlag = 1;
    }

    //�������vc�����ڣ�Ҫ�ñ���ʵ���vcɾ����־λ
    if((NULL == pst_vc_bak) || (NULL == pst_vc_bak->basic_cfg_cb)
		|| (NULL == pst_vc_bak->vpn_property_cfg_cb))
    {
        aps_pw_prot.protEntity.vcDelFlag = 1;
    }
	
    //���ð������ȱ�־
    //st_nni_vport.flags |= VPORT_COUNTER_ON;

    //st_nni_vport.tNextHop.flags |= L2_NEXT_HOP_COUNTER_ON;
    //st_nni_vport.tNextHop_p.flags |= L2_NEXT_HOP_COUNTER_ON;

    ret = SUCCESS;   
    
#if defined SPU || defined PTN690_CES

    if (SPM_VC_ADD_TYPE_ADD == vc_add_type)
    {
        //����API�����������ã���VP���뵽VPN��
        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            ret += ApiC3AddMplsPortToVpn(c3_unit, vpn_type, vpn_id, &st_nni_vport);
        }
    }
    else
    {
        //����API�����������ã�����VP
        st_nni_vport.vpIdx = vp_idx;

        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
        {
            ret += ApiC3ModVp(c3_unit, &st_nni_vport);
        }
    }
	
#endif

    if (ret == SUCCESS) /* vc����������ӳɹ� (zhangzhm) */
    {
        //��������˱�������group_id��Ϊ0
    	if ((1 == prot_flag) && (0 != st_nni_vport.frrGroupId))
    	{
    		spm_lps_add_entity(&aps_pw_prot);
    	}

    	//����ϴ������˱���������û�����ñ�����Ҫ���ñ���ɾ���Ľӿ�
    	if ((pst_vc_info->frr_group_id != 0) && (prot_flag == 0))
    	{
    		spm_lps_del_entity(FRR_TYPE, pst_vc_info->frr_group_id);
    	}

		//����ʱ��Ϊ0�����º�Ϊ0,���磬����֮ǰ������������֮��ֻ������������һ��IDҪ�ͷ�
        //����next_hop_id
        if ((0 != pst_vc_info->next_hop_id) && (0 == st_nni_vport.tNextHop.nhIdx))
        {
        	spm_free_vc_nhi_id(pst_vc_info->next_hop_id NBB_CCXT);
        }
                
        if ((0 != pst_vc_info->next_hop_id_p) && (0 == st_nni_vport.tNextHop_p.nhIdx))
        {
        	spm_free_vc_nhi_id(pst_vc_info->next_hop_id_p NBB_CCXT);
        }

		////����FRR ID����һ��ID,����������ִ���꣬Ҫ����Щ��Ϣд��VPWS��VPLS��
    	pst_vc_info->frr_group_id = st_nni_vport.frrGroupId;
        pst_vc_info->next_hop_id = st_nni_vport.tNextHop.nhIdx;
        pst_vc_info->next_hop_id_p = st_nni_vport.tNextHop_p.nhIdx;
		pst_vc_info->next_hop_counter_id = st_nni_vport.tNextHop.counterIdx;
		pst_vc_info->next_hop_counter_id_p = st_nni_vport.tNextHop_p.counterIdx;
        
    	if ((NULL != pst_vc_main) && (NULL != pst_vc_main->basic_cfg_cb)
			&& (NULL != pst_vc_main->vpn_property_cfg_cb))
    	{	    	
	        pst_vc_main->vc_info_cb.vp_idx = st_nni_vport.vpIdx;
		    pst_vc_main->vc_info_cb.vpn_id = vpn_id;
		    pst_vc_main->vc_info_cb.vpn_type = vpn_type;
		    pst_vc_main->vc_info_cb.if_main = VC_MAIN;
            pst_vc_main->vc_info_cb.drv_tunnel_id = st_nni_vport.tNextHop.tunnelIdx;
			pst_vc_main->vc_info_cb.port_index = port_index_main;
			pst_vc_main->vc_info_cb.next_hop_id = st_nni_vport.tNextHop.nhIdx;

			
	        pst_vc_info->port_index = port_index_main;	        
	        pst_vc_info->port_type = st_port_info_main.port_type;

	        if (st_port_info_main.port_type == ATG_DCI_LAG)
	        {
	        	pst_vc_main->vc_info_cb.port_id = st_port_info_main.lag_port_id;
	        	pst_vc_info->slot_id = st_port_info_main.lag_slot_id;
	        	pst_vc_main->vc_info_cb.slot_id = st_port_info_main.lag_slot_id;        	
	        }
	        else
	        {
	        	pst_vc_main->vc_info_cb.port_id = st_port_info_main.port_id;
	        	pst_vc_info->slot_id = st_port_info_main.slot_id;
	        	pst_vc_main->vc_info_cb.slot_id = st_port_info_main.slot_id;
	        }

			st_vp_info_refresh_main.vpn_type = vpn_type;
			st_vp_info_refresh_main.if_main = VC_MAIN;
			st_vp_info_refresh_main.vpn_id = vpn_id;
			st_vp_info_refresh_main.vc_key.vc_id = pst_vc_main->vc_key.vc_id;
			st_vp_info_refresh_main.vc_key.vc_type = pst_vc_main->vc_key.vc_type;
			st_vp_info_refresh_main.vc_key.peer_ip = pst_vc_main->vc_key.peer_ip;
			st_vp_info_refresh_main.port_index = port_index_main;
			st_vp_info_refresh_main.mc_id = mc_id;

	        //����vc��LSP��RSVP�Ĺ�ϵ
			ret = spm_add_vc_lsp_rsvp_relation(&st_vp_info_refresh_main,vp_idx,
				st_nni_vport.tNextHop.nhIdx, st_nni_vport.tNextHop.tunnelIdx);
			
			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",
					ADD_DRV_NNI_MAIN_LSP_RELATION_FAIL,ret);

				ret = SUCCESS;
			}
    	}
		
        if ((NULL != pst_vc_bak) && (NULL != pst_vc_bak->basic_cfg_cb)
			&& (NULL != pst_vc_bak->vpn_property_cfg_cb))
        {
            pst_vc_bak->vc_info_cb.vp_idx = st_nni_vport.vpIdx;
	        pst_vc_bak->vc_info_cb.vpn_id = vpn_id;
	        pst_vc_bak->vc_info_cb.vpn_type = vpn_type;
	        pst_vc_bak->vc_info_cb.if_main = VC_BACK;
			pst_vc_bak->vc_info_cb.port_index = port_index_bak;
            pst_vc_bak->vc_info_cb.drv_tunnel_id = st_nni_vport.tNextHop_p.tunnelIdx;
			pst_vc_bak->vc_info_cb.next_hop_id = st_nni_vport.tNextHop_p.nhIdx;
              
        	if (st_port_info_bak.port_type == ATG_DCI_LAG)
        	{
        		pst_vc_bak->vc_info_cb.port_id = st_port_info_bak.lag_port_id;
        		pst_vc_bak->vc_info_cb.slot_id = st_port_info_bak.lag_slot_id;
        	}
        	else
        	{
        		pst_vc_bak->vc_info_cb.port_id = st_port_info_bak.port_id;
        		pst_vc_bak->vc_info_cb.slot_id = st_port_info_bak.slot_id;
        	}

			st_vp_info_refresh_bak.vpn_type = vpn_type;
			st_vp_info_refresh_bak.if_main = VC_BACK;
			st_vp_info_refresh_bak.vpn_id = vpn_id;
			st_vp_info_refresh_bak.vc_key.vc_id = pst_vc_bak->vc_key.vc_id;
			st_vp_info_refresh_bak.vc_key.vc_type = pst_vc_bak->vc_key.vc_type;
			st_vp_info_refresh_bak.vc_key.peer_ip = pst_vc_bak->vc_key.peer_ip;
			st_vp_info_refresh_bak.port_index = port_index_bak;
			st_vp_info_refresh_bak.mc_id = mc_id;

			//����vc��LSP��RSVP�Ĺ�ϵ
			ret = spm_add_vc_lsp_rsvp_relation(&st_vp_info_refresh_bak,vp_idx,
				st_nni_vport.tNextHop_p.nhIdx, st_nni_vport.tNextHop_p.tunnelIdx);
			
			//ֻ��ӡ�����˳�
			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",
					ADD_DRV_NNI_BAK_LSP_RELATION_FAIL,ret);
				ret = SUCCESS;
			}
        }

        //�����VC�����ڣ���VC���ڣ�VP�ĳ��ھ�Ҫˢ�ɱ��ó���
        if ((pst_vc_main == NULL) && (pst_vc_bak != NULL))
        {
	        pst_vc_info->port_index = port_index_bak;	        
	        pst_vc_info->port_type = st_port_info_bak.port_type;
               
	        if (st_port_info_bak.port_type == ATG_DCI_LAG)
	        {
	        	pst_vc_info->slot_id = st_port_info_bak.lag_slot_id;       	
	        }
	        else
	        {
	        	pst_vc_info->slot_id = st_port_info_bak.slot_id;
	        }
			
#if defined SPU || defined PTN690_CES        
			//ǿ�е���������
	        for (c3_unit = 0; c3_unit < SHARED.c3_num; c3_unit++)
	        {
	        	ret = ApiC3SetFrrGroupStatus(c3_unit, st_nni_vport.frrGroupId, 1);

				if(SUCCESS != ret)
				{
					SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",
						ADD_DRV_NNI_SET_FRR_STATUS_FAIL,ret);
					ret = ADD_DRV_NNI_SET_FRR_STATUS_FAIL;
					goto EXIT_LABEL;
				}
	        }
#endif
        }

		
    }
    else    /* vc����������ӻ����ʧ�� (zhangzhm) */
    {

        //�½�VP���ɹ�Ҫ����next_hop_id���޸Ĳ��ɹ����ܻ���
        if (SPM_VC_ADD_TYPE_ADD == vc_add_type)
        {
        	if (0 != nhi_id)
        	{
        		spm_free_vc_nhi_id(nhi_id NBB_CCXT);
        	}

        	if (0 != nhi_id_p)
        	{
        		spm_free_vc_nhi_id(nhi_id_p NBB_CCXT);
        	}
        }

		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",ADD_DRV_NNI_VP_C3_FAIL,ret);
		ret = ADD_DRV_NNI_VP_C3_FAIL;
		
        goto EXIT_LABEL;
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}



/*****************************************************************************
 �� �� ��  : spm_vc_drv_del_nni_vp
 ��������  : ��VPWS ����VPLS��ɾ��һ��VP(NNI)
 �������  : NBB_BYTE ucNniId                   
             NBB_ULONG ulVplsId                 
             ATG_DCI_VPLS_NNI_DATA* pstNniData  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��25��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vc_drv_del_nni_vp(NBB_BYTE ucVpnType,NBB_USHORT usVpnId, 
	NBB_VOID* pst_l2vpn_nni_cfg)
{


	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    SPM_VC_CB *pstVcTbl = NULL;
    SPM_VC_CB *pstVcTblBack = NULL;
    ATG_DCI_VC_KEY stVcKey;
    ATG_DCI_VC_KEY stVcKeyBack;
    NBB_INT ret = SUCCESS;

    NBB_BYTE ucC3Unit = 0;
    NBB_UINT iVpId = 0;
    NBB_USHORT usFrrGroupId = 0;
    CRTXLSP_KEY stCrLspKey;
    FTN_KEY stFtnKey;
    SPM_QOS_TUNNEL_KEY stQosTunKey;
    ATG_DCI_VC_UP_VPN_QOS_POLICY stUpVpnData;
    LSPPROT_KEY st_lspprot_key;  
    LSPPROT_KEY st_lspprot_key_back;
    SPM_VPWS_CB *pst_vpws = NULL;
    SPM_VPLS_NNI_CB *pst_vpls_nni = NULL;
    NBB_BYTE ucprotectflag = 0;  
	NBB_BYTE if_vp_nni_exist = 0;
	NBB_INT next_hop_id = 0;
	NBB_INT next_hop_id_p = 0;

    NBB_TRC_ENTRY("spm_vpn_del_vp_nni");

    if (NULL == pst_l2vpn_nni_cfg)
    { 
        ret = DEL_DRV_NNI_VPN_IS_NULL;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));
    OS_MEMSET(&stVcKeyBack, 0, sizeof(ATG_DCI_VC_KEY));
    OS_MEMSET(&stCrLspKey, 0, sizeof(CRTXLSP_KEY));
    OS_MEMSET(&stFtnKey, 0, sizeof(FTN_KEY));
    OS_MEMSET(&st_lspprot_key, 0, sizeof(LSPPROT_KEY));
    OS_MEMSET(&st_lspprot_key_back, 0, sizeof(LSPPROT_KEY));    
    OS_MEMSET(&stQosTunKey, 0, sizeof(SPM_QOS_TUNNEL_KEY));
    OS_MEMSET(&stUpVpnData, 0, sizeof(ATG_DCI_VC_UP_VPN_QOS_POLICY));
    
    if (L2VPN_VPWS == ucVpnType)
    {
        pst_vpws = (SPM_VPWS_CB *)pst_l2vpn_nni_cfg;

        iVpId = pst_vpws->nni_no;
        
        stVcKey.vc_id   = pst_vpws->vckey_main.vc_id;
        stVcKey.vc_type = pst_vpws->vckey_main.vc_type;
        stVcKey.peer_ip = pst_vpws->vckey_main.peer_ip;

        stVcKeyBack.vc_id   = pst_vpws->vckey_back.vc_id;
        stVcKeyBack.vc_type = pst_vpws->vckey_back.vc_type;
        stVcKeyBack.peer_ip = pst_vpws->vckey_back.peer_ip;
		
		usFrrGroupId = pst_vpws->nni_info.frr_group_id;
		ucprotectflag = pst_vpws->use_protect;  
		if_vp_nni_exist = pst_vpws->if_drv_nni_vp_exist;
		next_hop_id = pst_vpws->nni_info.next_hop_id;
		next_hop_id_p = pst_vpws->nni_info.next_hop_id_p;
    }
    else if (L2VPN_VPLS == ucVpnType)
    {
        pst_vpls_nni = (SPM_VPLS_NNI_CB *)pst_l2vpn_nni_cfg;

        iVpId = pst_vpls_nni->nni_no;  
        
        stVcKey.vc_id   = pst_vpls_nni->vckey_main.vc_id;
        stVcKey.vc_type = pst_vpls_nni->vckey_main.vc_type;
        stVcKey.peer_ip = pst_vpls_nni->vckey_main.peer_ip;

        stVcKeyBack.vc_id   = pst_vpls_nni->vckey_back.vc_id;
        stVcKeyBack.vc_type = pst_vpls_nni->vckey_back.vc_type;
        stVcKeyBack.peer_ip = pst_vpls_nni->vckey_back.peer_ip;
		
		usFrrGroupId = pst_vpls_nni->nni_info.frr_group_id;
        ucprotectflag = pst_vpls_nni->use_protect;   
		if_vp_nni_exist = pst_vpls_nni->if_drv_nni_vp_exist;
		next_hop_id = pst_vpls_nni->nni_info.next_hop_id;
		next_hop_id_p = pst_vpls_nni->nni_info.next_hop_id_p;
    }

	SPM_L2_INFO_PRINT_LOG(ucMessage,"INFO:del_drv_nni,vpnid=%d.vpntype=%d,vpid=%d\n",
		usVpnId,ucVpnType,iVpId);

	//���NNI_NOΪ0��ֱ�ӱ�����
    if (iVpId == 0)
    {
        ret = DEL_DRV_NNI_VP_IDX_ZERO;
        goto EXIT_LABEL;
    }
    
    pstVcTbl = AVLL_FIND(SHARED.vc_tree, &stVcKey);
    pstVcTblBack = AVLL_FIND(SHARED.vc_tree, &stVcKeyBack);

#ifdef SPU
        ret = SUCCESS;

        if((usFrrGroupId != 0) && (ucprotectflag == 1))
        {
            for(ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
            {
                ret += ApiC3DeleteFrrGroup(ucC3Unit, usFrrGroupId);
            }

            if(ret != SUCCESS)
            {
				SPM_L2_ERROR_PRINT_LOG(ucMessage,"del vp nni frr,ret=%d\n",ret);
				ret = DEL_DRV_NNI_DEL_FRR_FAIL;
				goto EXIT_LABEL;
            }
        }
  #endif  

  #if 0

	//�������VC���ڣ�Ҫɾ������VC���������ص���Ϣ
	if ((pstVcTbl != NULL) && (pstVcTbl->basic_cfg_cb != NULL))
	{
        //ɾ��QOS����
       // spm_ds_add_vc_node(0,&stVcKey,pstVcTbl->vc_info_cb.vp_idx, 
        	//pstVcTbl->vc_info_cb.if_main);
		                         
		if (pstVcTbl->up_vpn_cfg_cb != NULL)
		{
        	stQosTunKey.flag = 1;
        	stQosTunKey.type = pstVcTbl->basic_cfg_cb->lsp_option;                	
            stQosTunKey.tx_lsp.egress = pstVcTbl->basic_cfg_cb->egress;
            stQosTunKey.tx_lsp.ingress = pstVcTbl->basic_cfg_cb->ingress;
            stQosTunKey.tx_lsp.tunnelid = pstVcTbl->basic_cfg_cb->tunnel_id;
            stQosTunKey.ftn.fec	= pstVcTbl->basic_cfg_cb->next_hop_ip;
            stQosTunKey.ftn.mask = 32;
            stQosTunKey.ftn.vrfid = 0;

        	stUpVpnData.node_index = pstVcTbl->up_vpn_cfg_cb->node_index;
        	stUpVpnData.qos_policy_index = 0;
        	stUpVpnData.flow_queue_qos_policy_index = 0;

            ret = spm_hqos_add_vc_node(pstVcTbl->vc_info_cb.slot_id,pstVcTbl->vc_info_cb.port_id,
				pstVcTbl->vc_info_cb.vp_idx,pstVcTbl->vc_info_cb.if_main,
				&stQosTunKey,&stVcKey,&stUpVpnData);
			
			//ֻ��¼���˳�
			SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",DEL_DRV_NNI_MAIN_UP_VPN_FAIL,ret);
		}
	}

    //�������VC���ڣ�Ҫɾ������VC���������ص���Ϣ
    if ((pstVcTblBack != NULL) && (pstVcTblBack->basic_cfg_cb != NULL))
    {
        //ɾ��QOS����
       // spm_ds_add_vc_node(0,&stVcKeyBack,pstVcTblBack->vc_info_cb.vp_idx, 
        	//pstVcTblBack->vc_info_cb.if_main);
		                         
		if (pstVcTblBack->up_vpn_cfg_cb != NULL)
		{
        	stQosTunKey.flag = 1;
        	stQosTunKey.type = pstVcTblBack->basic_cfg_cb->lsp_option;                	
            stQosTunKey.tx_lsp.egress = pstVcTblBack->basic_cfg_cb->egress;
            stQosTunKey.tx_lsp.ingress = pstVcTblBack->basic_cfg_cb->ingress;
            stQosTunKey.tx_lsp.tunnelid = pstVcTblBack->basic_cfg_cb->tunnel_id;
            stQosTunKey.ftn.fec	= pstVcTblBack->basic_cfg_cb->next_hop_ip;
            stQosTunKey.ftn.mask = 32;
            stQosTunKey.ftn.vrfid = 0;

        	stUpVpnData.node_index= pstVcTblBack->up_vpn_cfg_cb->node_index;
        	stUpVpnData.qos_policy_index = 0;
        	stUpVpnData.flow_queue_qos_policy_index = 0;

            spm_hqos_add_vc_node(pstVcTblBack->vc_info_cb.slot_id,pstVcTblBack->vc_info_cb.port_id, 
            	pstVcTblBack->vc_info_cb.vp_idx,pstVcTblBack->vc_info_cb.if_main,&stQosTunKey,
                &stVcKeyBack,&stUpVpnData);

			//ֻ��¼���˳�
			SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",DEL_DRV_NNI_BAK_UP_VPN_FAIL,ret);
		}
    }

	#endif
        
#ifdef SPU
    ret = SUCCESS;

	//���NNI���ڲ�ɾ���������ڱ���:�ظ�ɾ��
    if (DRV_NNI_VP_EXIST == if_vp_nni_exist)
    {
	    for (ucC3Unit = 0; ucC3Unit < SHARED.c3_num; ucC3Unit++)
	    {
	        ret += ApiC3DelMplsPortFromVpn(ucC3Unit, ucVpnType, usVpnId, iVpId);

			if(SUCCESS != ret)
			{
				SPM_L2_ERROR_PRINT_LOG(ucMessage,"del_c3_vp,r1=%d,r2=%d\n",
					DEL_DRV_NNI_VP_C3_FAIL,ret);
				ret = DEL_DRV_NNI_VP_C3_FAIL;
				
				goto EXIT_LABEL;
			}
	    }
    }
    
    
#endif

	if(0 != next_hop_id)
	{
		spm_free_vc_nhi_id(next_hop_id);
	}
	if(0 != next_hop_id_p)
	{
		spm_free_vc_nhi_id(next_hop_id_p);
	}

	if (usFrrGroupId != 0)
	{
		spm_lps_del_entity(FRR_TYPE, usFrrGroupId);
	}
	
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_vc_drv_del_nni_vp
 ��������  : ��VPWS ����VPLS��ɾ��һ��VP(NNI)
 �������  : NBB_BYTE ucNniId                   
             NBB_ULONG ulVplsId                 
             ATG_DCI_VPLS_NNI_DATA* pstNniData  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��25��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vc_drv_add_or_mod_bypass_vp(SPM_VPWS_CB *pst_vpws, 
	SPM_VC_ADD_TYPE_E vc_add_type,
	SPM_VPN_PORT_INFO_CB *vpn_info)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    VPORT_T bypass_vp;
    NBB_INT ret = SUCCESS;
    NBB_BYTE c3_uni_id = 0;
    SPM_VC_CB *pst_vc = NULL;
    ATG_DCI_VC_KEY vc_key;
    SPM_VP_INFO_REFRESH_CB vp_info_refresh;

    NBB_ULONG tunnel_id = 0;
    NBB_ULONG port_index = 0;
	NBB_INT nhi_id = 0;
	NBB_USHORT uni_vp_idx = 0;
	NBB_USHORT nni_vp_idx = 0;
	NBB_USHORT bypass_vp_idx = 0;
    
    SPM_PORT_INFO_CB port_info;

    NBB_TRC_ENTRY("spm_vpn_add_vp_bypass");

	if((NULL == pst_vpws) || (NULL == vpn_info))
	{
		ret = VC_DRV_ADD_BYPASS_VP_INPUT_NULL;
		goto EXIT_LABEL;
	}
    

    OS_MEMSET(&bypass_vp, 0, sizeof(VPORT_T));
    OS_MEMSET(&vc_key, 0, sizeof(ATG_DCI_VC_KEY));
    OS_MEMSET(&port_info, 0, sizeof(SPM_PORT_INFO_CB));
	OS_MEMSET(&vp_info_refresh, 0, sizeof(SPM_VP_INFO_REFRESH_CB));

    vp_info_refresh.if_main = VC_MAIN;
    vp_info_refresh.vpn_type = L2VPN_VPWS;
    vp_info_refresh.vpn_id = pst_vpws->vpws_id_key;
    vp_info_refresh.vc_key.vc_id = pst_vpws->vckey_bypass.vc_id;
    vp_info_refresh.vc_key.vc_type = pst_vpws->vckey_bypass.vc_type;
    vp_info_refresh.vc_key.peer_ip = pst_vpws->vckey_bypass.peer_ip;
    
    vc_key.vc_id = pst_vpws->vckey_bypass.vc_id;
    vc_key.peer_ip = pst_vpws->vckey_bypass.peer_ip;
    vc_key.vc_type = pst_vpws->vckey_bypass.vc_type;

	bypass_vp.vpIdx = pst_vpws->bypass_no;
    bypass_vp.eVpType = VP_NNI;
    

    /* ��ȡ��VC������� */
    pst_vc = AVLL_FIND(SHARED.vc_tree, &vc_key);
    if ((pst_vc == NULL) || (pst_vc->basic_cfg_cb == NULL)
		|| (pst_vc->vpn_property_cfg_cb == NULL))
    {
    	ret = VC_DRV_ADD_BYPASS_VP_VC_NULL;
     
        goto EXIT_LABEL;
    }

    //VLAN ����TAGʶ��
    if (pst_vc->basic_cfg_cb->recv_action == ATG_DCI_TAGGED_VLAN)
    {
    	bypass_vp.flags |= VPORT_PTAG_IDENTIFY_ON;
    }

    //VLAN ����TAG��Ϊ ����
    switch (pst_vc->basic_cfg_cb->send_tag_action)
    {
        case ATG_DCI_ADD_VID_PRI:
            bypass_vp.tNextHop.eEgrPtagAction = EGR_ENCAP;
            break;
        case ATG_DCI_DEL_VID_PRI:
            bypass_vp.tNextHop.eEgrPtagAction = EGR_NO_ENCAP;
            break;
        default:
            break;
    }

    bypass_vp.tNextHop.egrPtagVlan = pst_vc->basic_cfg_cb->send_vlan_id;
    bypass_vp.tNextHop.egrPtagPri = pst_vc->basic_cfg_cb->send_vlan_pri;

    if (pst_vc->basic_cfg_cb->tag_type == 0x8100)
    {
        bypass_vp.tNextHop.eEgrPtagTpidType = TPID_0X8100;
    }
    else if (pst_vc->basic_cfg_cb->tag_type == 0x88a8)
    {
        bypass_vp.tNextHop.eEgrPtagTpidType = TPID_0X88A8;
    }

    //��������
    bypass_vp.matchVcLabel = pst_vc->basic_cfg_cb->pw_in_label;

    bypass_vp.tNextHop.flags |= L2_NEXT_HOP_ENCAP_TUNNEL;
    bypass_vp.tNextHop.tPwLabel.label_un.tlabel.label = pst_vc->basic_cfg_cb->pw_out_label;
    bypass_vp.tNextHop.tPwLabel.label_un.tlabel.bottom = 1;
    bypass_vp.tNextHop.tPwLabel.label_un.tlabel.ttl = 255; 

	if (pst_vc->basic_cfg_cb->control_word_enable == ENABLE)
    {
        bypass_vp.flags |= VPORT_CW_SN;
        bypass_vp.tNextHop.eCwMode = EGR_CW_SEQ;
        bypass_vp.tNextHop_p.eCwMode = EGR_CW_SEQ;
    }

    if (vpn_info->next_hop_id != 0)
    {
    	bypass_vp.tNextHop.nhIdx = vpn_info->next_hop_id;
    }
    else
    {
    	spm_alloc_vc_nhi_id(&nhi_id);
    	bypass_vp.tNextHop.nhIdx = nhi_id;
    }

    //��ȡtunnel_id��Ϣ
    ret = spm_vc_get_lspinfo(pst_vc,&port_index,&tunnel_id);
	
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_GET_BYPASS_LSP_INFO_ERR,ret);
		ret = SUCCESS;
	}

	else
	{
		bypass_vp.tNextHop.tunnelIdx = tunnel_id;
		spm_get_portid_from_logical_port_index(port_index, &port_info);
	}

    ret = SUCCESS;

	#ifdef SPU

    //����API�����������ã���Bypass VP���뵽VPN��
    if(SPM_VC_ADD_TYPE_ADD == vc_add_type)
    {
    	for (c3_uni_id = 0; c3_uni_id < SHARED.c3_num; c3_uni_id++)
	    {
	        ret += ApiC3AddMplsPortToVpn(c3_uni_id, L2VPN_VPWS, pst_vpws->vpws_id_key, &bypass_vp);
	    }

		if(SUCCESS != ret)
		{
			if (nhi_id != 0)
			{
	        	spm_free_vc_nhi_id(nhi_id NBB_CCXT);
	        }
			
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_DRV_ADD_BYPASS_VP_ERR,ret);
			ret = VC_DRV_ADD_BYPASS_VP_ERR;
			goto EXIT_LABEL;
		}
    }

	else
	{
		for (c3_uni_id = 0; c3_uni_id < SHARED.c3_num; c3_uni_id++)
	    {
	        ret += ApiC3ModVp(c3_uni_id,&bypass_vp);
	    }

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_DRV_MOD_BYPASS_VP_ERR,ret);
			ret = VC_DRV_MOD_BYPASS_VP_ERR;
			goto EXIT_LABEL;
		}
	}

	#endif
    
    pst_vc->vc_info_cb.vp_idx = bypass_vp.vpIdx;
    pst_vc->vc_info_cb.next_hop_counter_id = bypass_vp.tNextHop.counterIdx;
    pst_vc->vc_info_cb.next_hop_id = bypass_vp.tNextHop.nhIdx;
    pst_vc->vc_info_cb.vpn_id = pst_vpws->vpws_id_key;
    pst_vc->vc_info_cb.vpn_type = L2VPN_VPWS;
    pst_vc->vc_info_cb.if_main = VC_MAIN;
	pst_vc->vc_info_cb.port_index = port_index;
    pst_vc->vc_info_cb.drv_tunnel_id = bypass_vp.tNextHop.tunnelIdx;
    pst_vc->vc_info_cb.lsp_option = pst_vc->basic_cfg_cb->lsp_option;  
	
    vpn_info->vp_idx = bypass_vp.vpIdx;
    vpn_info->next_hop_id = bypass_vp.tNextHop.nhIdx;

	

	if (port_info.port_type == ATG_DCI_LAG)
    {
    	pst_vc->vc_info_cb.port_id = port_info.lag_port_id;
    	pst_vc->vc_info_cb.slot_id = port_info.lag_slot_id;        	
    }
    else
    {
    	pst_vc->vc_info_cb.port_id = port_info.port_id;
    	pst_vc->vc_info_cb.slot_id = port_info.slot_id;
    }

	//����VC��LSP�Ĺ�ϵ
	ret = spm_add_vc_lsp_rsvp_relation(&vp_info_refresh,
		bypass_vp.vpIdx,
		bypass_vp.tNextHop.nhIdx,
		tunnel_id);
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_ADD_BYPASS_VC_LSP_RELATION_ERR,ret);
		ret = SUCCESS;
	}

	//����VP�İ�
	uni_vp_idx = pst_vpws->uni_no;
	nni_vp_idx = pst_vpws->nni_no;
	
#ifdef SPU
	for (c3_uni_id = 0; c3_uni_id < SHARED.c3_num; c3_uni_id++)
    {
        //ret += fhdrv_psn_l2_set_bypass(c3_uni_id, uni_vp_idx, nni_vp_idx, 
			//nni_vp_idx, uni_vp_idx, bypass_vp_idx, nni_vp_idx);

    }

	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_DRV_BIND_VP_ERR,ret);
		ret = VC_DRV_BIND_VP_ERR;
		goto EXIT_LABEL;
	}

#endif

	//��APS������
	//ret = spm_vc_drv_set_bypass_prot(pst_vpws, 
		//pst_vc->vpn_property_cfg_cb->associate_if_id, 
		//pst_vc->vpn_property_cfg_cb->bypass_dh_id);

    spm_vc_add_bypass_refresh(
        pst_vpws,
        pst_vc->vpn_property_cfg_cb->bypass_dh_id,
        pst_vc->vpn_property_cfg_cb->associate_if_id,
        bypass_vp.vpIdx);
    
    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_vc_drv_del_vpws_bypass_vp
 ��������  :
 �������  :                    
                             
              
 �������  : 
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��25��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vc_drv_del_vpws_bypass_vp(SPM_VC_CB *pst_vc)
{
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
	NBB_INT c3_unit_id = 0;
	NBB_USHORT uni_vp_idx = 0;
	NBB_USHORT nni_vp_idx = 0;
	SPM_VPWS_CB *pst_vpws = NULL;
	NBB_USHORT vpws_id_key = 0;

	if((NULL == pst_vc) || (NULL == pst_vc->vpn_property_cfg_cb))
	{
		return VC_DRV_DEL_BYPASS_VC_NULL;
	}

	vpws_id_key = pst_vc->vpn_property_cfg_cb->vpn_id;
	pst_vpws = AVLL_FIND(v_spm_shared->vpws_tree,&vpws_id_key);
	
	if(NULL == pst_vpws)
	{
		return VC_DRV_DEL_BYPASS_VPWS_NULL;
	}

	#ifdef SPU

	if(DRV_NNI_VP_EXIST == pst_vpws->if_drv_bypass_vp_exist)
	{
		for (c3_unit_id = 0; c3_unit_id < SHARED.c3_num; c3_unit_id++)
	    {
	        ret += ApiC3DelMplsPortFromVpn(c3_unit_id,L2VPN_VPWS,
	            vpws_id_key,pst_vpws->bypass_no);
	    }

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_C3_DEL_BYPASS_VP_ERR,ret);
			return VC_C3_DEL_BYPASS_VP_ERR;
		}
	}
	
	#endif
	
	//��uni_vp��nni_vp
	if(0 != pst_vpws->uni_no)
	{
		uni_vp_idx = pst_vpws->uni_no;
	}

	else if(0 != pst_vpws->uni_no_2)
	{
		uni_vp_idx = pst_vpws->uni_no_2;
	}

	if((0 != pst_vpws->nni_no)
		&& (DRV_NNI_VP_EXIST == pst_vpws->if_drv_nni_vp_exist))
	{
		nni_vp_idx = pst_vpws->nni_no;
	}

	#ifdef SPU

	if((0 != uni_vp_idx) && (0 != nni_vp_idx))
	{
		for (c3_unit_id = 0; c3_unit_id < SHARED.c3_num; c3_unit_id++)
	    {
	        ret += ApiC3BindVp(c3_unit_id, uni_vp_idx, nni_vp_idx);
	    }

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_DRV_DEL_BYPASS_BIND_VP_ERR,ret);
			return VC_DRV_DEL_BYPASS_BIND_VP_ERR;
		}
	}

	ret = aps_del_bypass_vpws(vpws_id_key);

       //fansongbo 20160408 ���ﲻ�ܷ�����ΪAPSģ��Ҳ��������δ��ӵ��������BYPASS
	//if(SUCCESS != ret)
	//{
		//SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_DEL_APS_BYPASS_VPWS_ERR,ret);
		//return VC_DEL_APS_BYPASS_VPWS_ERR;
	//}
	
    spm_vc_del_bypass_refresh(pst_vpws,pst_vc->vpn_property_cfg_cb->bypass_dh_id,pst_vpws->bypass_no);

	
	#endif
	return ret;

	
}


/*****************************************************************************
 �� �� ��  : spm_vc_drv_add_mc_nni_vp
 ��������  : VPLSʱ�������鲥, ����鲥VP
 �������  : NBB_BYTE ucVpType                       
             NBB_USHORT usVpnId                      
             NBB_VOID* pstVpData                     
             SPM_VPN_PORT_INFO_CB *pstVplsPortInfo  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��24��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vc_drv_add_mc_nni_vp(NBB_USHORT usVpnId,NBB_USHORT usVsiMcId,
	SPM_VPLS_NNI_CB *pst_vpls_nni)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_INT i = 0;
	NBB_INT iVpId = 0;
    NBB_BYTE ucC3Unit = 0;
    SPM_PORT_INFO_CB stPortInfo;
    SPM_PORT_INFO_CB stLagPortInfo;
   
    NBB_BYTE  ucPortNum = 0;
    NBB_ULONG ulPortIndex = 0;
    SPM_VC_CB *pst_vc = NULL;
    ATG_DCI_VC_KEY vc_key;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    
   
    NBB_INT ecmpVpflag = 0;

    NBB_TRC_ENTRY("spm_vpls_add_mc_vp_nni");

    if (NULL == pst_vpls_nni)
    {
		ret = ADD_DRV_NNI_MC_L2VPN_NNI_IS_NULL;
		goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stLagPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&vc_key, 0, sizeof(ATG_DCI_VC_KEY));

    //������ʱѡ������
    vc_key.vc_id = pst_vpls_nni->vckey_main.vc_id;
    vc_key.vc_type = pst_vpls_nni->vckey_main.vc_type;
    vc_key.peer_ip = pst_vpls_nni->vckey_main.peer_ip;

    pst_vc = AVLL_FIND(SHARED.vc_tree, &vc_key);

    if (pst_vc == NULL)
    {
        ret = ADD_DRV_NNI_MC_VC_MAIN_NULL;
        goto EXIT_LABEL;
    }

    //����VC�������е�port_index��ȡ�˿ڵ�port_id��slot_id�����Arad��FE1600
    //������ֻ��������VC
    ret = spm_get_portid_from_logical_port_index(pst_vc->vc_info_cb.port_index, 
    	&stPortInfo NBB_CCXT);

	iVpId = pst_vc->vc_info_cb.vp_idx;
	ulPortIndex = pst_vc->vc_info_cb.port_index;

	//��ӡ�ؼ���Ϣ
	SPM_L2_INFO_PRINT_LOG(ucMessage,"INFO:add_drv_mc,mc_id=%d,index=%ld,slot=%d,vp=%d\n",
		usVsiMcId,ulPortIndex,stPortInfo.slot_id,iVpId);

	
#if defined SRC || defined SNU

	//�����LAG����Ҫ�ҵ�LAG�µ�һ���˿ڵĲ�λ���·���FE1600
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = ApiDfeMulticastAdd(UNIT_0, usVsiMcId, stPortInfo.lag_slot_id);
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usVsiMcId, stPortInfo.slot_id);
	}
    
    if (ret != SUCCESS)
    {
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",ADD_DRV_NNI_MC_FE_ADD_MEMBER_FAIL,ret);
        ret = ADD_DRV_NNI_MC_FE_ADD_MEMBER_FAIL;
        
        goto EXIT_LABEL;
    }
    
#endif




#ifdef SPU 
    
     
    //1������鲥�˿ڽӿڻ��ӽӿڣ����ڱ���λ����
    //2������鲥�˿�ΪLAG��LAG��Ա�˿ڵĵ�һ���˿����ڵĲ۲��ڱ���λ����
    //3������鲥�˿�ΪVE�����ڱ���λ
    //�򲻵���������ֱ�ӷ��سɹ���
    if (((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_SUB_INTFACE == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_VE_PORT == stPortInfo.port_type))
     || ((SHARED.local_slot_id != stPortInfo.lag_slot_id) && (ATG_DCI_LAG == stPortInfo.port_type)))
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }


	//�����LAG����Ҫ�ҵ�LAG�µ�һ���˿ڣ��·���Arad
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = ApiAradAddEngressMcPort(UNIT_0, usVsiMcId, SPM_MULTICAST_PORT_0, iVpId);
	}
	else
	{
		//��ֹͬһ��VP��ǰ��ECMP VPģʽ���ù�����֮���ٴε�������
		if (0 == ecmpVpflag)
		{
			ret = ApiAradAddEngressMcPort(UNIT_0, usVsiMcId, SPM_MULTICAST_PORT_0, iVpId);
		}
	}

	if(SUCCESS != ret)
    {
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d\n",ADD_DRV_NNI_MC_ARAD_ADD_MEMBER_FAIL,ret);
        ret = ADD_DRV_NNI_MC_ARAD_ADD_MEMBER_FAIL;
        goto EXIT_LABEL;
    }
    
#endif

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return ret;

}



/*****************************************************************************
 �� �� ��  : spm_vc_drv_del_arad_mc_nni_vp
 ��������  : ɾ��MC_VP
 �������  : 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016.1.25
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vc_drv_del_arad_mc_nni_vp(SPM_VC_CB *pst_vc)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    NBB_INT iVpId = 0;
    NBB_INT i = 0;
    NBB_BYTE ucC3Unit = 0;
    NBB_BYTE ucSlot = 0;
	NBB_USHORT usVpnId = 0;
	NBB_INT vp_num_in_slot = 0;//ͬһ����λ��VP�ĸ���
    SPM_PORT_INFO_CB stPortInfo;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
	

    NBB_TRC_ENTRY("spm_vpls_del_mc_vp");

    if (NULL == pst_vc)
    {
        ret = DEL_DRV_MC_NNI_VC_NULL;
        goto EXIT_LABEL;
    }

	if(NULL == pst_vc->basic_cfg_cb)
	{
		ret = DEL_DRV_MC_NNI_VC_BASIC_NULL;
		goto EXIT_LABEL;
	}

	if(NULL == pst_vc->vpn_property_cfg_cb)
	{
		ret = DEL_DRV_MC_NNI_VC_VPN_NULL;
		goto EXIT_LABEL;
	}

	usVpnId = pst_vc->vc_info_cb.vpn_id;

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

	//��ӡ�ؼ���Ϣ
	SPM_L2_INFO_PRINT_LOG(ucMessage,"INFO:del_drv_mc,mc_id=%d,index=%ld\n",
		pst_vc->vpn_property_cfg_cb->mc_id,pst_vc->vc_info_cb.port_index);

    
    //����VC�������е�port_index��ȡ�˿ڵ�port_id��slot_id�����Arad��FE1600
    //������ֻ��������VC
    spm_get_portid_from_logical_port_index(pst_vc->vc_info_cb.port_index, &stPortInfo);
	
    iVpId = pst_vc->vc_info_cb.vp_idx;
    ucSlot = pst_vc->vc_info_cb.slot_id;

	SPM_L2_ERROR_PRINT_LOG(ucMessage,"DEL ARAD MC,slot=%d,mc_id=%d,vp=%d\n",
		stPortInfo.slot_id,pst_vc->vpn_property_cfg_cb->mc_id,iVpId);
    
#ifdef SPU 

//1������鲥�˿ڽӿڻ��ӽӿڣ����ڱ���λ����
	//2������鲥�˿�ΪLAG��LAG��Ա�˿ڵĵ�һ���˿����ڵĲ۲��ڱ���λ����
	//3������鲥�˿�ΪVE�����ڱ���λ
	//�򲻵���������ֱ�ӷ��سɹ���
	if (((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_INTFACE == stPortInfo.port_type))
	 || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_SUB_INTFACE == stPortInfo.port_type))
	 || ((SHARED.local_slot_id != stPortInfo.slot_id) && (ATG_DCI_VE_PORT == stPortInfo.port_type))
	 || ((SHARED.local_slot_id != stPortInfo.lag_slot_id) && (ATG_DCI_LAG == stPortInfo.port_type)))
	{
        
        ret = SUCCESS;
        goto EXIT_LABEL;
    }
	
	ret = ApiAradDeleteEngressMcPort(UNIT_0,pst_vc->vpn_property_cfg_cb->mc_id,
			SPM_MULTICAST_PORT_0,iVpId);

    if (ret != SUCCESS)
    {
    	SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d,r2=%d",
			DEL_DRV_NNI_MC_VP_ARAD_PORT_FAIL,ret);
		
    	ret = DEL_DRV_NNI_MC_VP_ARAD_PORT_FAIL;
		goto EXIT_LABEL;
    }
	
#endif

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return ret;

}

NBB_INT spm_vc_drv_del_src_mc_vp(SPM_VPLS_CB *pst_vpls,NBB_USHORT slot_id,NBB_USHORT mc_id)
{
	NBB_INT slot_num = 0;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;

	if(NULL == pst_vpls)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",DRV_DEL_NNI_SRC_MC_VC_NULL);
		return DRV_DEL_NNI_SRC_MC_VC_NULL;
	}

	slot_num = spm_get_vpls_slot_num(pst_vpls,slot_id);

	SPM_L2_ERROR_PRINT_LOG(uc_message,"DEL SRC MC,mc_id=%d,slot_id=%d,slot_num=%d\n",
		mc_id,slot_id,slot_num);
	
	if(0 == slot_num)
	{
		#if defined SRC || defined SNU
		ret = ApiDfeMulticastDelete(UNIT_0,mc_id,slot_id);

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",DRV_DEL_NNI_SRC_MC_ERR,ret);
			return DRV_DEL_NNI_SRC_MC_ERR;
		}
		#endif
		
	}

	return SUCCESS;
}

/*****************************************************************************
   �� �� ��  : spm_vc_drv_del_vpls_mc_id
   ��������  : ˢ��VPLS�鲥��Ϣ
                             
   �������  : ��
   �������  : ��
   �� �� ֵ  :     
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2016��1��26��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vc_drv_del_vpls_mc_id(NBB_USHORT mc_id)
{
	NBB_INT ret = SUCCESS;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN]={0};

	
	#ifdef SPU
	ret = ApiAradDeleteMc(UNIT_0, mc_id);
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",DEL_VC_VPLS_MC_ARAD_ERR,ret);
		return DEL_VC_VPLS_MC_ARAD_ERR;
	}
	#endif

	#if defined SRC || defined SNU
	ret = ApiDfeMulticastDestroy(UNIT_0, mc_id);
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",DEL_VC_VPLS_MC_FE_ERR,ret);
		return DEL_VC_VPLS_MC_FE_ERR;
	}
	#endif

	return ret;
}



/*****************************************************************************
 �� �� ��  : spm_vc_drv_create_nni_vp_for_mspw
 ��������  : ���vc�Ĵ���������add��modģʽ���ú�����ע��:
            1����ȡӦ�����ã�����������
            2��������ӳɹ��󣬸���vc_info_cb�е�vpidx���Ա�ʾ�����ɹ���
 �������  : NBB_BYTE            vpn_type     
             SPM_VC_CB           *pst_vc_tbl  
             SPM_VC_ADD_TYPE_E   vc_add_type  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2015��11��30��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���,���ڵ����������

*****************************************************************************/
NBB_INT spm_vc_drv_create_nni_vp_for_mspw(NBB_USHORT vpn_id,
	NBB_USHORT vp_idx,
    ATG_DCI_VC_KEY vc_key, 
    SPM_VC_ADD_TYPE_E vc_add_type,
    SPM_VPN_PORT_INFO_CB *pst_vc_info)
{
    NBB_INT ret = SUCCESS;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN]={0};
    VPORT_T              stNniVport;
    NBB_UINT             iNhiId =0;
    NBB_BYTE             ucC3Unit = 0;
    SPM_VC_CB            *pstVcMain = NULL;
    SPM_VC_CB            *pstVcBack = NULL;
    ATG_DCI_VC_KEY       stVcKeyMain;
    ATG_DCI_VC_KEY       stVcKeyBack;
    CRTXLSP_KEY          stCrLspKey;     //���µ�������ӿ�
    LSPPROT_KEY          st_lspprot_key;  //LSPPROT  keyֵ
    FTN_KEY              stFtnKey;
    SPM_VP_INFO_REFRESH_CB stVpInfoRefresh;
    SPM_SWITCH_VC_CB        *pst_mspw =NULL;
    SPM_VC_CB *pst_vc_tbl     = NULL;
    NBB_ULONG  port_index = 0;
    SPM_PORT_INFO_CB st_port_info;
    

    NBB_ULONG            ulTunnelIndexMaster = 0;
    NBB_ULONG            ulTunnelIndexSlave = 0;
    NBB_ULONG            ulPortIndexMaster = 0;
    NBB_ULONG            ulPortIndexSlave = 0;
    NBB_UINT 			 iVpId = 0;
    NBB_BYTE             ucProtectFlag = 0;
    NBB_BYTE             ucProtectType = 0;
    NBB_USHORT           usFrrGroupId = 0;

    NBB_TRC_ENTRY("spm_vc_drv_create_nni_vp_for_mspw");

    if((0 == vp_idx) || (0 == vpn_id))
    {
        ret = MSPW_CREATE_NNI_VP_INPUT_ERROR;
        goto EXIT_LABEL;     
    }

	if(NULL == pst_vc_info)
	{
		ret = MSPW_CREATE_NNI_VP_VC_INFO_NULL;
		goto EXIT_LABEL;
	}


	//��¼��Ҫ��Ϣ
	SPM_L2_ERROR_PRINT_LOG(uc_message,"DRV ADD MPWS VP,vpn_id=%d,vp_idx=%d,add_type=%d\n",
		vpn_id,vp_idx,vc_add_type);

    OS_MEMSET(&stNniVport, 0, sizeof(VPORT_T));
    OS_MEMSET(&stCrLspKey, 0, sizeof(CRTXLSP_KEY));
    OS_MEMSET(&st_lspprot_key, 0, sizeof(LSPPROT_KEY));	
    OS_MEMSET(&stFtnKey, 0, sizeof(FTN_KEY));
    OS_MEMSET(&stVpInfoRefresh, 0, sizeof(SPM_VP_INFO_REFRESH_CB));
    OS_MEMSET(&st_port_info, 0, sizeof(SPM_PORT_INFO_CB));

    pst_vc_tbl = AVLL_FIND(SHARED.vc_tree, &vc_key);

    if((NULL == pst_vc_tbl) || (NULL == pst_vc_tbl->basic_cfg_cb))
    {
        ret = MSPW_CREATE_NNI_VP_VC_OR_VCBASIC_NULL;
        goto EXIT_LABEL;    
    }
    
    stVpInfoRefresh.if_main = VC_MAIN;
    stVpInfoRefresh.vpn_type = L2VPN_MSPW;
    stVpInfoRefresh.vpn_id = vpn_id;
    stVpInfoRefresh.vc_key.vc_id = vc_key.vc_id;
    stVpInfoRefresh.vc_key.vc_type = vc_key.vc_type;
    stVpInfoRefresh.vc_key.peer_ip = vc_key.peer_ip;

    stNniVport.vpIdx = vp_idx;
    stNniVport.eVpType = VP_NNI;

    //��������
    stNniVport.matchVcLabel = pst_vc_tbl->basic_cfg_cb->pw_in_label;
    stNniVport.tNextHop.flags |= L2_NEXT_HOP_ENCAP_TUNNEL;
    stNniVport.tNextHop.tPwLabel.label_un.tlabel.label  = pst_vc_tbl->basic_cfg_cb->pw_out_label;
    stNniVport.tNextHop.tPwLabel.label_un.tlabel.bottom = 1;
    stNniVport.tNextHop.tPwLabel.label_un.tlabel.ttl    = 255; //Э��������Ҫ���� todo
    
    if (pst_vc_tbl->basic_cfg_cb->control_word_enable == ENABLE)
    {
        stNniVport.flags |= VPORT_CW_SN;
        stNniVport.tNextHop.eCwMode = EGR_CW_SEQ;
    }

    if (0 != pst_vc_info->next_hop_id)
    {
    	stNniVport.tNextHop.nhIdx = pst_vc_info->next_hop_id;
    }
    else
    {
    	spm_alloc_vc_nhi_id(&iNhiId NBB_CCXT);
    	stNniVport.tNextHop.nhIdx = iNhiId;
    }

    ret = spm_vc_get_lspinfo(pst_vc_tbl,&port_index, &ulTunnelIndexMaster);
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",MSPW_CREATE_NNI_VP_GET_LSP_INFO_ERR,ret);
		
		ret = SUCCESS;
	}
	
    stNniVport.tNextHop.tunnelIdx = ulTunnelIndexMaster;
    spm_get_portid_from_logical_port_index(port_index, &st_port_info NBB_CCXT);

    
#ifdef SPU

    ret = SUCCESS;

    //�����µ�VPN
    if((vc_add_type == L2VPN_MSPW_NNI_1_ADD)
		|| (vc_add_type == L2VPN_MSPW_NNI_2_ADD))
    {
        //����API�����������ã���VP���뵽VPN��
        for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
        {
            ret += ApiC3AddMplsPortToVpn(ucC3Unit, L2VPN_MSPW, vpn_id, &stNniVport);          
        }

        if(SUCCESS != ret)
        {
            spm_free_vc_nhi_id(iNhiId);
           
            //��¼������Ϣ
            SPM_L2_ERROR_PRINT_LOG(uc_message,"*r1=%d,ret=%d\n",
            MSPW_CREATE_NNI_VP_C3_VPN_ERR,ret);
            ret = MSPW_CREATE_NNI_VP_C3_VPN_ERR;
            goto EXIT_LABEL;     
        }
        else
        {
            pst_vc_info->next_hop_id = stNniVport.tNextHop.nhIdx;
        }
    }
    else if((vc_add_type == L2VPN_MSPW_NNI_1_MOD)
		|| (vc_add_type == L2VPN_MSPW_NNI_2_MOD))
    {
        for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
        {
            ret += ApiC3ModVp(ucC3Unit,&stNniVport);          
        }
        if(SUCCESS != ret)
        {
            //��¼������Ϣ
            SPM_L2_ERROR_PRINT_LOG(uc_message,"*ERR pst_vc_tbl is null *,r1=%d,ret=%d\n",
            MSPW_MOD_NNI_VP_C3_VPN_ERR,ret);
            ret = MSPW_MOD_NNI_VP_C3_VPN_ERR;
            goto EXIT_LABEL;     
        }
    }

    //ADD��MOD�ɹ������
    if(ret == SUCCESS)
    {
        ret = spm_add_vc_lsp_rsvp_relation(&stVpInfoRefresh,vp_idx,
					stNniVport.tNextHop.nhIdx,ulTunnelIndexMaster);

		//ֻ��ӡ�����˳�
		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",
				ADD_DRV_NNI_MSPW_LSP_RELATION_FAIL,ret);
			ret = SUCCESS;
		}
		
		
        pst_vc_tbl->vc_info_cb.port_index = port_index;
        pst_vc_tbl->vc_info_cb.vpn_id = vpn_id;
        pst_vc_tbl->vc_info_cb.vpn_type = L2VPN_MSPW;
        pst_vc_tbl->vc_info_cb.port_id = st_port_info.port_id;
        pst_vc_tbl->vc_info_cb.drv_tunnel_id = ulTunnelIndexMaster;
        pst_vc_tbl->vc_info_cb.vp_idx = vp_idx;
		pst_vc_tbl->vc_info_cb.next_hop_id = stNniVport.tNextHop.nhIdx;

        if(st_port_info.port_type == ATG_DCI_LAG)
        {
            pst_vc_tbl->vc_info_cb.slot_id = st_port_info.lag_slot_id;
        }
        else
        {
            pst_vc_tbl->vc_info_cb.slot_id = st_port_info.slot_id;
        }
        
    }
	
#endif

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;    
}


/*****************************************************************************
 �� �� ��  : spm_drv_del_mspw_nni_vp
 ��������  : ɾ��һ��VC��ʱ�򣬴�L2VPN��ɾ��һ��VP(NNI),ע��:��ʱVPWS,VPLS��
 				����VC�������Ѿ����
 �������  : NBB_BYTE ucNniId                   
             NBB_ULONG ulVplsId                 
             ATG_DCI_VPLS_NNI_DATA* pstNniData  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��24��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_drv_del_mspw_nni_vp(ATG_DCI_VC_KEY vc_key, NBB_USHORT usVpnId)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_VC_CB *pstVc = NULL;
    SPM_VC_INFO_CB stVcDrvInfo;
    NBB_ULONG next_hop_id = 0;
	NBB_USHORT vp_idx = 0;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN]={0};

    NBB_TRC_ENTRY("spm_drv_del_mspw_nni_vp");

    pstVc = AVLL_FIND(SHARED.vc_tree,&vc_key);

    if((NULL == pstVc)
		|| (NULL == pstVc->basic_cfg_cb)
		|| (NULL == pstVc->vpn_property_cfg_cb))
    {
        ret = DRV_DEL_MSPW_NNI_INPUT_ERR;
        goto EXIT_LABEL;
    }

    spm_get_vc_drvinfo(vc_key,&stVcDrvInfo NBB_CCXT);

    next_hop_id = stVcDrvInfo.next_hop_id;
	vp_idx = stVcDrvInfo.vp_idx;

	SPM_L2_ERROR_PRINT_LOG(uc_message,"DRV DEL MSPW VP,vpn_id=%d,vp_idx=%d\n",usVpnId,vp_idx);
    
#ifdef SPU 
   
    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3DelMplsPortFromVpn(ucC3Unit,L2VPN_MSPW,usVpnId,vp_idx);

		if(SUCCESS != ret)
		{
			SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",
				DRV_DEL_MSPW_NNI_C3_DEL_VPN_ERR,ret);
			ret = DRV_DEL_MSPW_NNI_C3_DEL_VPN_ERR;
			goto EXIT_LABEL;
		}
    }
	
    if (next_hop_id != 0)
    {
    	spm_free_vc_nhi_id(next_hop_id );
    }
  
#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
 �� �� ��  : spm_drv_add_vc_diff_serv
 ��������  : ����QOS�Ľӿڣ�����VC��DIFF_SERV�Ĵ���
 				
 �������  :                  
                          
              
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��24��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_drv_add_vc_diff_serv(SPM_VC_CB *pst_vc)
{
	NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN]={0};

	if(NULL == pst_vc)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",DRV_ADD_VC_DIFF_SERV_VC_IS_NULL);
        return DRV_ADD_VC_DIFF_SERV_VC_IS_NULL;
        
	}

	if(NULL == pst_vc->vpn_property_cfg_cb)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",DRV_ADD_VC_DIFF_SERV_VPN_IS_NULL);
        return DRV_ADD_VC_DIFF_SERV_VPN_IS_NULL;
	}

	if(NULL == pst_vc->diff_serv_cfg_cb)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",DRV_ADD_VC_DIFF_SERV_DIFF_SERV_IS_NULL);
        return DRV_ADD_VC_DIFF_SERV_DIFF_SERV_IS_NULL;
	}

	ret = spm_ds_set_vc_node(&pst_vc->vc_key,
		pst_vc->diff_serv_cfg_cb,
		pst_vc->vc_info_cb.vp_idx,
		pst_vc->vpn_property_cfg_cb->vc_state);

	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_DRV_SET_DS_VC_NODE_ERR,ret);
		return VC_DRV_SET_DS_VC_NODE_ERR;
	}

	return ret;
	
}

/*****************************************************************************
 �� �� ��  : spm_drv_add_vc_up_vpn_qos
 ��������  : 
 				
 �������  :                  
                          
              
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��24��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_drv_add_vc_up_vpn_qos(SPM_VC_CB *pst_vc)
{
	NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN]={0};
	SPM_QOS_TUNNEL_KEY qos_tunnel_key;

	if(NULL == pst_vc)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",DRV_ADD_VC_HQOS_VC_IS_NULL);
        return DRV_ADD_VC_HQOS_VC_IS_NULL;
	}

	if((NULL == pst_vc->basic_cfg_cb)
		|| (NULL == pst_vc->vpn_property_cfg_cb))
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d\n",DRV_ADD_VC_HQOS_VC_TLV_NULL);
		return DRV_ADD_VC_HQOS_VC_TLV_NULL;
	}

	OS_MEMSET(&qos_tunnel_key,0,sizeof(qos_tunnel_key));
	
	qos_tunnel_key.type   		   = pst_vc->basic_cfg_cb->lsp_option;               	
    qos_tunnel_key.tx_lsp.egress   = pst_vc->basic_cfg_cb->egress;  
    qos_tunnel_key.tx_lsp.ingress  = pst_vc->basic_cfg_cb->ingress; 
    qos_tunnel_key.tx_lsp.tunnelid = pst_vc->basic_cfg_cb->tunnel_id;  
    qos_tunnel_key.ftn.fec		   = pst_vc->basic_cfg_cb->next_hop_ip;  
    qos_tunnel_key.ftn.mask        = 32;
    qos_tunnel_key.ftn.vrfid       = 0;

	ret = spm_hqos_add_vc_node(pst_vc->vc_info_cb.slot_id,
		pst_vc->vc_info_cb.port_id,
		pst_vc->vc_info_cb.vp_idx,
		pst_vc->vpn_property_cfg_cb->vc_state,
		&qos_tunnel_key,
		&pst_vc->vc_key,
		pst_vc->up_vpn_cfg_cb);

	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_DRV_ADD_VC_HQOS_ERR,ret);
		return VC_DRV_ADD_VC_HQOS_ERR;
	}

	return ret;
}


/*****************************************************************************
 �� �� ��  : spm_drv_del_vc_up_vpn_qos
 ��������  : 
 				
 �������  :                  
                          
              
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2016��1��24��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_drv_del_vc_up_vpn_qos(SPM_VC_CB *pst_vc)
{
	NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
	NBB_CHAR uc_message[SPM_MSG_INFO_LEN]={0};
	SPM_QOS_TUNNEL_KEY qos_tunnel_key;

	if(NULL == pst_vc)
	{
        return DRV_DEL_VC_HQOS_VC_IS_NULL;
	}

	if(NULL == pst_vc->basic_cfg_cb)
	{
		return DRV_DEL_VC_HQOS_VC_BASIC_IS_NULL;
	}

	OS_MEMSET(&qos_tunnel_key,0,sizeof(qos_tunnel_key));
	
	qos_tunnel_key.type   		   = pst_vc->basic_cfg_cb->lsp_option;               	
    qos_tunnel_key.tx_lsp.egress   = pst_vc->basic_cfg_cb->egress;  
    qos_tunnel_key.tx_lsp.ingress  = pst_vc->basic_cfg_cb->ingress; 
    qos_tunnel_key.tx_lsp.tunnelid = pst_vc->basic_cfg_cb->tunnel_id;  
    qos_tunnel_key.ftn.fec		   = pst_vc->basic_cfg_cb->next_hop_ip;  
    qos_tunnel_key.ftn.mask        = 32;
    qos_tunnel_key.ftn.vrfid       = 0;
	
	ret =  spm_hqos_del_vc_node(pst_vc->vc_info_cb.slot_id,
	 	pst_vc->vc_info_cb.port_id,
        &qos_tunnel_key,
        &pst_vc->vc_key);
	
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",VC_DRV_DEL_VC_HQOS_ERR,ret);
		return VC_DRV_DEL_VC_HQOS_ERR;
	}

	return ret;
}



/*****************************************************************************
 �� �� ��  : spm_vc_drv_set_bypass_prot
 ��������  : ˫�鱣�����ô���
 �������  : SPM_VPWS_CB *pstVpws       
             NBB_USHORT usAssociatedId  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��28��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vc_drv_set_bypass_prot(SPM_VPWS_CB *pst_vpws, NBB_USHORT associate_if_id, 
	NBB_USHORT bypass_dh_id)
{
    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    ST_BYPASS_VPWS_CFG aps_bypass_cfg;
    SPM_VC_CB *pst_vc = NULL;
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];

    NBB_TRC_ENTRY("spm_vc_drv_set_bypass_prot");

    if (pst_vpws == NULL)
    {
    	ret = VC_DRV_SET_BYPASS_PROT_VPWS_NULL;
    	goto EXIT_LABEL;
    }
    
    OS_MEMSET(&aps_bypass_cfg, 0, sizeof(ST_BYPASS_VPWS_CFG));
   
    
    //�����ڵ�֮���໥������bypass vpws�Ĺ���ID��ͬ
    aps_bypass_cfg.bypassVpwsId = pst_vpws->vpws_id_key;
    aps_bypass_cfg.vpwsAssociatedId = associate_if_id;
    aps_bypass_cfg.bypassDhId = bypass_dh_id;
	
    //NNI����
    if(0 != pst_vpws->nni_no)
    {
    	aps_bypass_cfg.nniVpPosId = pst_vpws->nni_no;
	    aps_bypass_cfg.nniVcKey.vc_id = pst_vpws->vckey_main.vc_id;
	    aps_bypass_cfg.nniVcKey.vc_type = pst_vpws->vckey_main.vc_type;
	    aps_bypass_cfg.nniVcKey.peer_ip = pst_vpws->vckey_main.peer_ip;

	    pst_vc = AVLL_FIND(SHARED.vc_tree, &aps_bypass_cfg.nniVcKey);
	    
	    if (pst_vc != NULL)      
	    {
	        aps_bypass_cfg.nniPortIndex = pst_vc->vc_info_cb.port_index;
	    }

		else
		{
			ret = VC_DRV_SET_BYPASS_PROT_NNI_VC_NULL;
	    	goto EXIT_LABEL;
		}

    }

	else
	{
		ret = VC_DRV_SET_BYPASS_PROT_NNI_NO_EXIST;
		goto EXIT_LABEL;
	}
    

    //�ҵ� UNI
    if(0 != pst_vpws->uni_no)
    {
    	 aps_bypass_cfg.acVpPosId = pst_vpws->uni_no;
         aps_bypass_cfg.acPortIndex = pst_vpws->port_index;
    }

	else if(0 != pst_vpws->uni_no_2)
	{
		aps_bypass_cfg.acVpPosId = pst_vpws->uni_no_2;
        aps_bypass_cfg.acPortIndex = pst_vpws->port_index_2;
	}

	else
	{
		ret = VC_DRV_SET_BYPASS_PROT_UNI_NO_EXIST;
		goto EXIT_LABEL;
	}

	//�ҵ�BYPASS
	if(0 != pst_vpws->bypass_no)
	{
		aps_bypass_cfg.bypassVpPosId = pst_vpws->bypass_no;
    
	    aps_bypass_cfg.bypassVcKey.vc_id = pst_vpws->vckey_bypass.vc_id;
	    aps_bypass_cfg.bypassVcKey.vc_type = pst_vpws->vckey_bypass.vc_type;
	    aps_bypass_cfg.bypassVcKey.peer_ip = pst_vpws->vckey_bypass.peer_ip;

	    pst_vc = AVLL_FIND(SHARED.vc_tree, &aps_bypass_cfg.bypassVcKey);
    
	    if (pst_vc != NULL)
	    {
	        aps_bypass_cfg.bypassPortIndex = pst_vc->vc_info_cb.port_index;
	    }
	    else
	    {
	        ret = VC_DRV_SET_BYPASS_PROT_BYPASS_VC_NULL;
			goto EXIT_LABEL;
	    }
	}

	else
	{	
		ret = VC_DRV_SET_BYPASS_PROT_BYPASS_NO_EXIST;
		goto EXIT_LABEL;
	}
	
    ret = aps_add_bypass_vpws(&aps_bypass_cfg);
	if(SUCCESS != ret)
	{
		SPM_L2_ERROR_PRINT_LOG(uc_message,"r1=%d,r2=%d\n",APS_ADD_BYPASS_VPWS_ERR,ret);
		ret = APS_ADD_BYPASS_VPWS_ERR;
		goto EXIT_LABEL;
	}

    EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}



#endif
