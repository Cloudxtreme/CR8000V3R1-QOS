/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_bypass_proc.c
  �� �� ��   : ����
  ��    ��   : xiaoxiang
  ��������   : 2013��7��16��
  ����޸�   :
  ��������   : Bypass˫�鱣�����ô���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��7��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef BYPASS_CFG

extern unsigned char bypass_cfg_print_setting;

/*****************************************************************************
   �� �� ��  : spm_rcv_dci_set_arp
   ��������  : ����ARP���IPS��Ϣ
   �������  : ATG_DCI_SET_ARP *pstSetArp NBB_CCXT_T NBB_CXT
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��11��10��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_bypass(ATG_DCI_SET_BYPASS *pstSetBypass)
{
    NBB_CHAR print_message[SPM_MSG_INFO_LEN] = {0};
    NBB_BYTE ucMessageLen = 0;

    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_BYPASS_CB *pstBypass = NULL;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    ATG_DCI_BYPASS_BASIC_DATA *pstBasicData = NULL;
    
    NBB_BOOL avll_ret_code = FALSE;

    NBB_BYTE *pucBasicDataStart = NULL;

    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;

    NBB_USHORT usBypassDhIdKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_bypass");

    /* �������ָ�������Ч */
    NBB_ASSERT(pstSetBypass != NULL);

    if (pstSetBypass == NULL)
    {
        SPM_L2_ERROR_PRINT_LOG(print_message,"r1=%d\n",PROC_BYPASS_IPS_DATA_NULL);
        goto EXIT_LABEL;
    }

    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pstSetBypass->return_code = ATG_DCI_RC_OK;

    usBypassDhIdKey = pstSetBypass->key;

    pstBypass = AVLL_FIND(SHARED.bypass_tree, &usBypassDhIdKey);

    if (pstBypass == NULL)       /* �����Ŀ������*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }

    /* ��ȡ�����ò��� */
    ulOperBasic = pstSetBypass->oper_basic;

    /* ɾ��������Ŀ */
    if (pstSetBypass->delete_struct == TRUE)
    {
        /* �����Ŀ�����ڣ���ɾ�� */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            SPM_L2_ERROR_PRINT_LOG(print_message,"r1=%d key=%d\n",DEL_BYPASS_CFG_NULL,usBypassDhIdKey);
            goto EXIT_LABEL;
        }

        /* ���ڣ�ɾ�� */
        else
        {
		spm_dbg_print_bypass_head(usBypassDhIdKey, SPM_OPER_DEL);
		spm_dbg_record_bypass_head(usBypassDhIdKey, SPM_OPER_DEL);

		ret = spm_del_bypass_dh(usBypassDhIdKey);

		if(SUCCESS != ret)
		{
		    SPM_L2_ERROR_PRINT_LOG(print_message,"r1=%d key=%d\n",ret,usBypassDhIdKey);
		    goto EXIT_LABEL;        
		}

		if (pstBypass != NULL)
		{
			AVLL_DELETE(SHARED.bypass_tree, pstBypass->spm_bypass_node);

			//�ͷ�ARP��ڵ���ڴ�ռ�
			spm_free_bypass_cb(pstBypass NBB_CCXT);
		}
        }
    }

    /* ���ӻ������Ŀ */
    else
    {

        /* �����Ŀ�����ڣ��������ڴ�ռ䱣������ */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            spm_dbg_print_bypass_head(usBypassDhIdKey, SPM_OPER_ADD);
            spm_dbg_record_bypass_head(usBypassDhIdKey, SPM_OPER_ADD);

            /* ����һ������Ŀ���ڴ�ռ� */
            pstBypass = spm_alloc_bypass_cb(NBB_CXT);

		if (pstBypass != NULL)
		{
			//���뵽tree��
			pstBypass->bypass_key = usBypassDhIdKey;

			avll_ret_code = AVLL_INSERT(SHARED.bypass_tree, pstBypass->spm_bypass_node);
			NBB_ASSERT(avll_ret_code);
		}
        }
        else
        {
            spm_dbg_print_bypass_head(usBypassDhIdKey, SPM_OPER_UPD);
            spm_dbg_record_bypass_head(usBypassDhIdKey, SPM_OPER_UPD);
        }

        /***************************************************************************/
        /*                        ��ȡ���ã���������                               */
        /***************************************************************************/
        /* �������� */
		/******************************** ȫ������ *********************************/
		if(ATG_DCI_OPER_NULL != ulOperBasic)
		{
			ret = spm_bypass_basic_proc(pstSetBypass, pstBypass, ulOperBasic);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(print_message,"process bypass basic faild ret=%d,key=%d\n",
									ret,pstSetBypass->key);
	            pstBypass->basic_ret_code = ret;     
	        }
		}
    }

EXIT_LABEL: NBB_TRC_EXIT();

    return;

}

/*****************************************************************************
 �� �� ��  : spm_init_bypass_tree
 ��������  : ��ʼ���洢Bypass˫�鱣�������õ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_bypass_basic_proc(ATG_DCI_SET_BYPASS *pstSetBypass, 
                               SPM_BYPASS_CB *pstBypass, NBB_ULONG ulOperBasic)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucBasicDataStart = NULL;    
    ATG_DCI_BYPASS_BASIC_DATA *basic_data = NULL;
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
	NBB_USHORT bypass_key = 0;

    if((NULL == pstSetBypass) || (NULL == pstBypass))
    {
        ret = PROC_BYPASS_BASIC_INPUT_POINTER_NULL;
        goto EXIT_LABEL;
    }

	bypass_key = pstSetBypass->key;
	
    switch(ulOperBasic)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
			break;

		case ATG_DCI_OPER_ADD:

            //�������õ��׵�ַ
            pucBasicDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetBypass, &pstSetBypass->basic_data);

            /* ���ָ��ΪNULL�������� */
            if (pucBasicDataStart == NULL)
            {
                ret = PROC_BYPASS_BASIC_DATA_NULL;
                goto EXIT_LABEL;
            }

            basic_data = (ATG_DCI_BYPASS_BASIC_DATA *)pucBasicDataStart;

			if (bypass_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  1����������\n");
                spm_dbg_print_bypass_basic_cfg(basic_data);
            }

            OS_SPRINTF(ucMessage, "  1����������\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            
            spm_dbg_record_bypass_basic_cfg(basic_data);

            if(NULL == pstBypass->basic_cfg_cb)
            {
                pstBypass->basic_cfg_cb = (ATG_DCI_BYPASS_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_BYPASS_BASIC_DATA),
	                            NBB_NORETRY_ACT,MEM_SPM_BYPASS_BASIC_CB);  
            }

            if(NULL != pstBypass->basic_cfg_cb)
            {
                NBB_MEMCPY(pstBypass->basic_cfg_cb, basic_data, sizeof(ATG_DCI_BYPASS_BASIC_DATA));
            }
            else
            {
                ret = BYPASS_BASIC_MALLOC_MEMORY_ERROR;
                goto EXIT_LABEL;    
            }

			//���ñ����ӿ����bypass����
			ret = spm_set_aps_bypass_dh(bypass_key,pstBypass->basic_cfg_cb);			
            break;

         default:
            break;
            
    }
	    
EXIT_LABEL: NBB_TRC_EXIT();

    return ret;
}


/*****************************************************************************
 �� �� ��  : spm_init_bypass_tree
 ��������  : ��ʼ���洢Bypass˫�鱣�������õ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_bypass_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_bypass_tree");

    avll_key_offset = NBB_OFFSETOF(SPM_BYPASS_CB, bypass_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.bypass_tree, compare_ushort,
        (NBB_USHORT)avll_key_offset,
        (NBB_USHORT)NBB_OFFSETOF(SPM_BYPASS_CB, spm_bypass_node));

    NBB_TRC_EXIT();
}

/*****************************************************************************
 �� �� ��  : spm_alloc_bypass_cb
 ��������  : ����Bypass˫�鱣�����õ��ڴ�ռ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : SPM_BYPASS_CB
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_BYPASS_CB *spm_alloc_bypass_cb(NBB_CXT_T NBB_CXT)
{
    SPM_BYPASS_CB *pstBypass = NULL;

    NBB_TRC_ENTRY("spm_alloc_bypass_cb");

    /* ����һ���µ�VC��������Ŀ��*/
    pstBypass = (SPM_BYPASS_CB *)NBB_MM_ALLOC(sizeof(SPM_BYPASS_CB), NBB_NORETRY_ACT, MEM_SPM_BYPASS_CB);
    if (pstBypass == NULL)
    {
        goto EXIT_LABEL;
    }

    /* ��ʼ��VC��������Ŀ */
    OS_MEMSET(pstBypass, 0, sizeof(SPM_BYPASS_CB));
    pstBypass->basic_cfg_cb = NULL;

    /* �������ڸ�VC��������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstBypass->spm_bypass_handle = NBB_CREATE_HANDLE(pstBypass, HDL_SPM_BYPASS_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_BYPASS_CB allocated at %p with handle %#lx",
            pstBypass, pstBypass->spm_bypass_handle));

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstBypass->spm_bypass_node);

EXIT_LABEL: NBB_TRC_EXIT();

    return(pstBypass);
}

/*****************************************************************************
 �� �� ��  : spm_free_bypass_cb
 ��������  : �ͷ�Bypass˫�鱣�����ýڵ㲴�ڴ�ռ�
 �������  : SPM_BYPASS_CB *pstBypass  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��7��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_bypass_cb(SPM_BYPASS_CB *pstBypass NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    //NBB_INT i = 0;

    NBB_TRC_ENTRY("spm_free_bypass_cb");
    
    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/
    NBB_ASSERT_MEMORY(pstBypass, sizeof(SPM_BYPASS_CB), MEM_SPM_BYPASS_CB);

    if (pstBypass == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/

    NBB_TRC_FLOW((NBB_FORMAT "Free SPM_BYPASS_CB at %p", pstBypass));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (pstBypass->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstBypass->basic_cfg_cb, MEM_SPM_BYPASS_BASIC_CB);
        pstBypass->basic_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstBypass->spm_bypass_handle, HDL_SPM_BYPASS_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstBypass, MEM_SPM_BYPASS_CB);
    pstBypass = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   �� �� ��  : spm_free_all_bypass_cb
   ��������  : �ͷ�bypass�������е��ڴ�ռ�
   �������  : ��
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2013��11��11��
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���

*****************************************************************************/

NBB_VOID spm_free_all_bypass_cb(NBB_CXT_T NBB_CXT)
{
	SPM_BYPASS_CB *pstBypass = NULL;
	
    for (pstBypass = (SPM_BYPASS_CB*) AVLL_FIRST(SHARED.bypass_tree);
         pstBypass != NULL;
         pstBypass = (SPM_BYPASS_CB*) AVLL_FIRST(SHARED.bypass_tree))
    {
    	
		AVLL_DELETE(SHARED.bypass_tree, pstBypass->spm_bypass_node);    	
    	spm_free_bypass_cb(pstBypass NBB_CCXT);
    }
}


#endif


