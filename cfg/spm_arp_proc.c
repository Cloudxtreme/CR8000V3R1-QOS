/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_arp_proc.c
   �� �� ��   : ����
   ��    ��   : xiaoxiang
   ��������   : 2012��11��10��
   ����޸�   :
   ��������   : ARP�����ô���
   �����б�   :
   �޸���ʷ   :
   1.��    ��   : 2012��11��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef ARP_CFG

extern unsigned char arp_cfg_print_setting;

NBB_INT spm_arp_basic_data_proc(ATG_DCI_SET_ARP *pst_set_arp,NBB_ULONG ul_oper_basic,
    SPM_ARP_CB *pst_arp)
{

	NBB_CHAR uc_message[SPM_MSG_INFO_LEN];  
    NBB_INT ret = SUCCESS;
    ATG_DCI_ARP_BASIC_DATA *pst_basic_data = NULL;
    NBB_BYTE *puc_basic_data_start = NULL; 
	ARP_T stArp;

    NBB_TRC_ENTRY("spm_vc_basic_data_proc");

    if (NULL == pst_set_arp)
    {
       	return ADD_ARP_BASIC_ARP_IPS_IS_NULL;
        
    }
    
    if (NULL == pst_arp)
    {
        return ADD_ARP_BASIC_ARP_CFG_IS_NULL;
    }
	OS_MEMSET(&stArp,0,sizeof(ARP_T));

	if(ATG_DCI_OPER_ADD == ul_oper_basic)
	{
		/* �����һ��entry�ĵ�ַ��*/
	    puc_basic_data_start = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pst_set_arp, &pst_set_arp->basic_data);

	    /* ���ָ��ΪNULL�������� */
	    if (puc_basic_data_start == NULL)
	    {
	        return ADD_ARP_BASIC_DATA_START_IS_NULL;
	    }
   		else
	    {
	        pst_basic_data = (ATG_DCI_ARP_BASIC_DATA *)puc_basic_data_start;

	        if (arp_cfg_print_setting == SPM_PRINT_CFG)
	        {
	            printf("  1����������\n");
	            spm_dbg_print_arp_basic_cfg(pst_basic_data);
	        }

	        OS_SPRINTF(uc_message, "  1����������\n");
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, uc_message);
	        
	        spm_dbg_record_arp_basic_cfg(pst_basic_data);

			//�ȱ�������
			
	        if (pst_arp->basic_cfg_cb == NULL)
	        {
	            pst_arp->basic_cfg_cb = (ATG_DCI_ARP_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_ARP_BASIC_DATA),
	                NBB_NORETRY_ACT,
	                MEM_SPM_ARP_BASIC_CB);

				if(NULL == pst_arp->basic_cfg_cb)
				{
					return ADD_ARP_BASIC_MEM_BASIC_NULL;
				}
	        }

	        OS_MEMCPY(pst_arp->basic_cfg_cb, pst_basic_data, sizeof(ATG_DCI_ARP_BASIC_DATA));
			ret = spm_arp_drv_add_arp(pst_arp);
	    }
	}

	return ret;
  

}

/*****************************************************************************
   �� �� ��  : spm_rcv_dci_set_arp
   ��������  : ����ARP���IPS��Ϣ
   �������  : ATG_DCI_SET_ARP *pst_set_arp NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��11��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_arp(ATG_DCI_SET_ARP *pst_set_arp)
{
    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];
    NBB_BYTE uc_messageLen = 0;

    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_ARP_CB *pst_arp = NULL;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;

    SPM_PORT_INFO_CB stPortInfo;

    ATG_DCI_ARP_KEY stArpKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_arp");

    /* �������ָ�������Ч */
    NBB_ASSERT(pst_set_arp != NULL);

    if (pst_set_arp == NULL)
    {
        ret = set_ARP_IPS_NULL;
		goto EXIT_LABEL;
    }

	pst_set_arp->return_code = ATG_DCI_RC_OK;
	pst_set_arp->basic_return_code = ATG_DCI_RC_OK;

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    stArpKey.index = pst_set_arp->key.index;
    stArpKey.ip_addr = pst_set_arp->key.ip_addr;

    pst_arp = AVLL_FIND(SHARED.arp_tree, &stArpKey);

    if (pst_arp == NULL)      
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &stArpKey.index);

    if ((NULL == pstLogicalPort) || (NULL == pstLogicalPort->basic_cfg_cb))
    {
        ret = set_ARP_LOGICAL_PORT_NULL;
        goto EXIT_LABEL;
    }
   
    if (ATG_DCI_L2 == pstLogicalPort->basic_cfg_cb->port_route_type)
    {
        ret = set_ARP_ROUTE_L2;
        goto EXIT_LABEL;
    }
    

    /* ��ȡ�����ò��� */
    ulOperBasic = pst_set_arp->oper_basic;

    //���ݶ˿��߼������еĶ˿������ͣ���ȡ�˿ڵ�port_id
    spm_get_portid_from_logical_port_index(pst_set_arp->key.index, &stPortInfo NBB_CCXT);

    /* ɾ��������Ŀ */
    if (pst_set_arp->delete_struct == TRUE)
    {
        /* �����Ŀ�����ڣ���ɾ�� */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            goto EXIT_LABEL;
        }

        /* ���ڣ�ɾ�� */
        else
        {
            spm_dbg_print_arp_head(&stArpKey, SPM_OPER_DEL);
            spm_dbg_record_arp_head(&stArpKey, SPM_OPER_DEL);

#ifdef SPU
            ret = spm_arp_drv_del_arp(pst_arp);
			if(SUCCESS != ret)
			{
				pst_arp->total_ret_code = ret;
				goto EXIT_LABEL;
			}
#endif

			if (pst_arp != NULL)
			{
	            AVLL_DELETE(SHARED.arp_tree, pst_arp->spm_arp_node);

	            //�ͷ�ARP��ڵ���ڴ�ռ�
	            spm_free_arp_cb(pst_arp NBB_CCXT);
			}
        }
    }

    /* ���ӻ������Ŀ */
    else
    {

        /* �����Ŀ�����ڣ��������ڴ�ռ䱣������ */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            spm_dbg_print_arp_head(&stArpKey, SPM_OPER_ADD);
            spm_dbg_record_arp_head(&stArpKey, SPM_OPER_ADD);

            /* ����һ������Ŀ���ڴ�ռ� */
            pst_arp = spm_alloc_arp_cb(NBB_CXT);

			if (pst_arp != NULL)
			{
	            //���뵽tree��
	            OS_MEMCPY(&pst_arp->arp_key, &stArpKey, sizeof(ATG_DCI_ARP_KEY));
	            AVLL_INSERT(SHARED.arp_tree, pst_arp->spm_arp_node);
			}
        }
        else
        {
            spm_dbg_print_arp_head(&stArpKey, SPM_OPER_UPD);
            spm_dbg_record_arp_head(&stArpKey, SPM_OPER_UPD);
        }

        /***************************************************************************/
        /*                        ��ȡ���ã���������                               */
        /***************************************************************************/
       /******************************** �������� *********************************/        
        if (ATG_DCI_OPER_NULL != ulOperBasic)
        {
            ret = spm_arp_basic_data_proc(pst_set_arp,ulOperBasic,pst_arp);
			if (SUCCESS != ret)    
	        {
	        	SPM_L2_ERROR_PRINT_LOG(uc_message,"arp basic proc r1=%d\n",ret);
				pst_arp->basic_ret_code = ret;
	        }   
        }

    }

EXIT_LABEL: NBB_TRC_EXIT();


    return;

}

/*****************************************************************************
   �� �� ��  : spm_init_arp_tree
   ��������  : ��ʼ���洢ARP���õ���
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
NBB_VOID spm_init_arp_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_arp_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_ARP_CB, arp_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.arp_tree, spm_arp_key_compare,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_ARP_CB, spm_arp_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
   �� �� ��  : spm_alloc_arp_cb
   ��������  : ����ARP�ڵ���ڴ�ռ�
   �������  : NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  : SPM_ARP_CB
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��11��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_ARP_CB *spm_alloc_arp_cb(NBB_CXT_T NBB_CXT)
{
    SPM_ARP_CB *pst_arp = NULL;

    NBB_TRC_ENTRY("spm_alloc_arp_cb");

    /* ����һ���µ�VC��������Ŀ��*/
    pst_arp = (SPM_ARP_CB *)NBB_MM_ALLOC(sizeof(SPM_ARP_CB), NBB_NORETRY_ACT, MEM_SPM_ARP_CB);
    if (pst_arp == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��VC��������Ŀ */
    OS_MEMSET(pst_arp, 0, sizeof(SPM_ARP_CB));
    pst_arp->basic_cfg_cb = NULL;

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pst_arp->spm_arp_handle = NBB_CREATE_HANDLE(pst_arp, HDL_SPM_ARP_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_ARP_CB allocated at %p with handle %#lx",
            pst_arp, pst_arp->spm_arp_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pst_arp->spm_arp_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pst_arp);
}

/*****************************************************************************
   �� �� ��  : spm_free_arp_cb
   ��������  : �ͷ�ARP��ڵ㲴�ڴ�ռ�
   �������  : SPM_ARP_CB *pst_arp NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��11��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_arp_cb(SPM_ARP_CB *pst_arp NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_arp_cb");

    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pst_arp, sizeof(SPM_ARP_CB), MEM_SPM_ARP_CB);

    if (pst_arp == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_ARP_CB at %p", pst_arp));
    NBB_TRC_FLOW((NBB_FORMAT "Free INDEX %d  IP_ADDR %s", pst_arp->arp_key.index,
            spm_set_ulong_to_ipv4(pst_arp->arp_key.ip_addr NBB_CCXT)));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (pst_arp->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pst_arp->basic_cfg_cb, MEM_SPM_ARP_BASIC_CB);
        pst_arp->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pst_arp->spm_arp_handle, HDL_SPM_ARP_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pst_arp, MEM_SPM_ARP_CB);
    pst_arp = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_free_all_arp_cb
   ��������  : �ͷ�arp�������е��ڴ�ռ�
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

NBB_VOID spm_free_all_arp_cb(NBB_CXT_T NBB_CXT)
{
	SPM_ARP_CB *pst_arp = NULL;
	
    for (pst_arp = (SPM_ARP_CB*) AVLL_FIRST(SHARED.arp_tree);
         pst_arp != NULL;
         pst_arp = (SPM_ARP_CB*) AVLL_FIRST(SHARED.arp_tree))
    {
    	
		AVLL_DELETE(SHARED.arp_tree, pst_arp->spm_arp_node);     	
    	spm_free_arp_cb(pst_arp NBB_CCXT);
    }
}


#endif

