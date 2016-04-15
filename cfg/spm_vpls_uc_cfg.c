/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_vpls_uc_proc.c
  �� �� ��   : ����
  ��    ��   : xiaoxiang
  ��������   : 2013��4��18��
  ����޸�   :
  ��������   : VPLS��̬�������ô���
  �����б�   :
              spm_rcv_dci_set_vpls_mc
  �޸���ʷ   :
  1.��    ��   : 2013��4��18��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef VPLS_UC_CFG

extern unsigned char vpls_uc_cfg_print_setting;


/*****************************************************************************
 �� �� ��  : spm_vpls_uc_basic_data_proc
 ��������  : 
 �������  : 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 
    ��    ��   : zhangzhm
    �޸�����   : �����ɺ���
    

*****************************************************************************/
NBB_INT spm_vpls_uc_basic_data_proc(ATG_DCI_SET_VPLS_UC *pst_set_vpls_uc,
	NBB_ULONG ul_oper_basic,
    SPM_VPLS_UC_CB *pst_vpls_uc)
{

    NBB_CHAR uc_message[SPM_MSG_INFO_LEN];  
    NBB_INT ret = SUCCESS;
    ATG_DCI_VPLS_UC_BASIC_DATA *pst_basic_data = NULL;
    NBB_BYTE *puc_basic_data_start = NULL;   

    NBB_TRC_ENTRY("spm_vpls_uc_basic_data_proc");

    /* �������ָ�������Ч */
    NBB_ASSERT(NULL != pst_set_vpls_uc);

    if (NULL == pst_set_vpls_uc)
    {
       	ret = ADD_VPLS_UC_BASIC_IPS_IS_NULL;
        goto EXIT_LABEL;
    }
    
    if (NULL == pst_vpls_uc)
    {
        ret = ADD_VPLS_UC_BASIC_CFG_IS_NULL;
        goto EXIT_LABEL;
    }
	
    if (ATG_DCI_OPER_ADD == ul_oper_basic)
    {

        /* �����һ��entry�ĵ�ַ��*/
        puc_basic_data_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pst_set_vpls_uc, 
        	&pst_set_vpls_uc->basic_data);    

        /* ���ָ��ΪNULL�������� */
        if (NULL == puc_basic_data_start)
        {
            ret = ADD_VPLS_UV_BASIC_DATA_START_IS_NULL;
            
            goto EXIT_LABEL;
        }
        
        /* �������ݴ��ڴ��� (zhangzhm) */
        pst_basic_data = (ATG_DCI_VPLS_UC_BASIC_DATA *)puc_basic_data_start;

        if (SPM_PRINT_CFG == vpls_uc_cfg_print_setting)
        {
            printf("  1��basic cfg\n");
            spm_dbg_print_vpls_uc_basic_cfg(pst_basic_data);
        }

		OS_SPRINTF(uc_message,"  1��basic cfg\n");
		spm_dbg_record_vpls_uc_basic_cfg(pst_basic_data);
		

		SPM_L2_INFO_PRINT_LOG(uc_message,"  1��vpls uc basic cfg add\n");

		
		

        /* �������ݴ���Ϊ���������¿ռ� */
        if (NULL == pst_vpls_uc->basic_cfg_cb)
        {
            pst_vpls_uc->basic_cfg_cb = 
                (ATG_DCI_VPLS_UC_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_UC_BASIC_DATA),
                                                    NBB_NORETRY_ACT,
                                                    MEM_SPM_VPLS_UC_BASIC_CB);

            if(NULL == pst_vpls_uc->basic_cfg_cb)
            {
            	ret = ADD_VPLS_UC_MALLOC_BASIC_MEM_FAIL;
                goto EXIT_LABEL;
            }
        }

        /*����Ϊ��������Ѵ��ڣ��洢���� */
        if(NULL != pst_vpls_uc->basic_cfg_cb)   
        {
            OS_MEMCPY(pst_vpls_uc->basic_cfg_cb, pst_basic_data, sizeof(ATG_DCI_VPLS_UC_BASIC_DATA)); 
        } 

		//��������
		#ifdef SPU
		ret = spm_vpls_uc_drv_set_static_mac(&pst_vpls_uc->vpls_uc_key,
				pst_vpls_uc->basic_cfg_cb);

		if(SUCCESS != ret)
		{
			goto EXIT_LABEL;
		}
		#endif
		
    }

    else if (ul_oper_basic == ATG_DCI_OPER_DEL)
    { 
        NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
    }
    
    else if (ul_oper_basic == ATG_DCI_OPER_UPDATE)
    {
        NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;

}


/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_vpls_mc
 ��������  : VPLS�鲥���ô�����
 �������  : ATG_DCI_SET_VPLS_MC *pstSetVplsMc  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��12��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_vpls_uc(ATG_DCI_SET_VPLS_UC *pstSetVplsUc)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    
    SPM_VPLS_UC_CB *pstVplsUc = NULL;

    /* �����õĲ���ģʽ */
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;

    /* VPLS�鲥�ļ�ֵ */
    ATG_DCI_VPLS_UC_KEY stVplsUcKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_vpls_uc");

    /* �������ָ�������Ч */
    NBB_ASSERT(pstSetVplsUc != NULL);

	if (pstSetVplsUc == NULL)
	{
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"*ERR*,r1=%d\n",VPLS_UC_IPS_NULL);
        goto EXIT_LABEL;
    }

    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pstSetVplsUc->return_code = ATG_DCI_RC_OK;

    OS_MEMCPY(&stVplsUcKey, &pstSetVplsUc->key, sizeof(ATG_DCI_VPLS_UC_KEY)); 
    pstVplsUc = AVLL_FIND(SHARED.vpls_uc_tree, &stVplsUcKey);

	//��Ŀ������
    if (NULL == pstVplsUc)    
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    
    
    /* ��ȡ�����õĲ������� */
    ulOperBasic = pstSetVplsUc->oper_basic;


    /* ɾ��������Ŀ */
    if (pstSetVplsUc->delete_struct == TRUE)
    {
       
        /* �����Ŀ�����ڣ���ɾ�� */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
        	SPM_L2_ERROR_PRINT_LOG(ucMessage,"*ERR*,r1=%d\n",DEL_VPLS_UC_CFG_NULL);
            goto EXIT_LABEL;
        }
		
        /* ���ڣ�ɾ�� */
        else
        {
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_DEL);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_DEL);


			if (pstVplsUc == NULL)
			{
				goto EXIT_LABEL;
			}

			ret = spm_vpls_uc_drv_del_static_mac(pstVplsUc);

			if(SUCCESS != ret)
			{
				pstVplsUc->total_ret_code = ret;
				goto EXIT_LABEL;
			}


			if (pstVplsUc != NULL)
			{
	            //������ɾ���ڵ�
	            AVLL_DELETE(SHARED.vpls_uc_tree, pstVplsUc->spm_vpls_uc_node); 

	            //�ͷŽڵ��ڴ�ռ�
	            spm_free_vpls_uc_cb(pstVplsUc NBB_CCXT);
			}
        }
    }
    /* ���ӻ������Ŀ */
    else
    {        
        
        /* �����Ŀ�����ڣ��������ڴ�ռ䱣������ */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {                                                                          
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_ADD);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_ADD);
            
            /* ����һ������Ŀ���ڴ�ռ� */
            pstVplsUc = spm_alloc_vpls_uc_cb(NBB_CXT);

			if (NULL != pstVplsUc)
			{
	            //���뵽�ڵ���
	            OS_MEMCPY(&pstVplsUc->vpls_uc_key, &stVplsUcKey, sizeof(ATG_DCI_VPLS_UC_KEY));
	            AVLL_INSERT(SHARED.vpls_uc_tree, pstVplsUc->spm_vpls_uc_node);
			}

			else
			{
				SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d\n",ADD_VPLS_UC_MALLOC_NULL);
				pstVplsUc->total_ret_code = ADD_VPLS_UC_MALLOC_NULL;
				goto EXIT_LABEL;
			}
         
        }
        else
        {                                                                                               
            spm_dbg_print_vpls_uc_head(&stVplsUcKey, SPM_OPER_UPD);
            spm_dbg_record_vpls_uc_head(&stVplsUcKey, SPM_OPER_UPD);
        }

        /***************************************************************************/
        /*                              ��ȡ����                                   */
        /***************************************************************************/
        /******************************** �������� *********************************/
        if (ATG_DCI_OPER_NULL != ulOperBasic)
        {
        	ret = spm_vpls_uc_basic_data_proc(pstSetVplsUc,ulOperBasic,pstVplsUc);
			if (SUCCESS != ret)    
	        {
	        	SPM_L2_ERROR_PRINT_LOG(ucMessage,"ERR,vpls_uc_basic_proc r1=%d\n",ret);
				pstVplsUc->basic_ret_code = ret;
	        } 
        }
    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}


/*****************************************************************************
 �� �� ��  : spm_init_vpls_uc_tree
 ��������  : ��ʼ��VPLS��̬������
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��18��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_vpls_uc_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vpls_uc_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_VPLS_UC_CB, vpls_uc_key);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.vpls_uc_tree, spm_vpls_uc_key_compare,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_VPLS_UC_CB, spm_vpls_uc_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 �� �� ��  : spm_free_vpls_uc_cb
 ��������  : �ͷ�VPLS��̬�����ṹ���ڴ�
 �������  : SPM_VPLS_UC_CB *pstVplsUc  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��18��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_vpls_uc_cb(SPM_VPLS_UC_CB *pstVplsUc NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    //NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_vpls_uc_cb");
    
    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstVplsUc, sizeof(SPM_VPLS_UC_CB), MEM_SPM_VPLS_UC_CB); 

    if (pstVplsUc == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pstVplsUc));  
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws_id %d", pstVplsUc->vpws_id_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (pstVplsUc->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstVplsUc->basic_cfg_cb, MEM_SPM_VPLS_UC_BASIC_CB);  
        pstVplsUc->basic_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstVplsUc->spm_vpls_uc_handle, HDL_SPM_VPLS_UC_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstVplsUc, MEM_SPM_VPLS_UC_CB);  
    pstVplsUc = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_alloc_vpls_uc_cb
 ��������  : ����VPLS��̬�����ڴ�ռ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : SPM_VPLS_UC_CB
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��4��18��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_VPLS_UC_CB * spm_alloc_vpls_uc_cb(NBB_CXT_T NBB_CXT)
{
    //NBB_INT i = 0;
    SPM_VPLS_UC_CB *pstVplsUc = NULL;

    NBB_TRC_ENTRY("spm_alloc_vpls_uc_cb");

    /* ����һ���µ�VPLS�鲥������Ŀ��*/
    pstVplsUc = (SPM_VPLS_UC_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_UC_CB), NBB_NORETRY_ACT, MEM_SPM_VPLS_UC_CB);
    if (pstVplsUc == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:����VPLS�鲥��Ŀ�ռ�ΪNULL!"));
        OS_PRINTF("***ERROR***:����VPLS�鲥��Ŀ�ռ�ΪNULL!\n");
        
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPLS�鲥������Ŀ */
    OS_MEMSET(pstVplsUc, 0, sizeof(SPM_VPLS_UC_CB));
    pstVplsUc->basic_cfg_cb = NULL;

    /* �������ڸ�VPLS�鲥������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstVplsUc->spm_vpls_uc_handle = NBB_CREATE_HANDLE(pstVplsUc, HDL_SPM_VPLS_UC_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_UC_CB allocated at %p with handle %#lx",
                   pstVplsUc, pstVplsUc->spm_vpls_uc_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstVplsUc->spm_vpls_uc_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstVplsUc);
}

/*****************************************************************************
   �� �� ��  : spm_free_all_vpls_uc_cb
   ��������  : �ͷ�vpls�����������е��ڴ�ռ�
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

NBB_VOID spm_free_all_vpls_uc_cb(NBB_CXT_T NBB_CXT)
{
	SPM_VPLS_UC_CB *pstVplsUc = NULL;
	
    for (pstVplsUc = (SPM_VPLS_UC_CB*) AVLL_FIRST(SHARED.vpls_uc_tree);
         pstVplsUc != NULL;
         pstVplsUc = (SPM_VPLS_UC_CB*) AVLL_FIRST(SHARED.vpls_uc_tree))
    {
    	
		AVLL_DELETE(SHARED.vpls_uc_tree, pstVplsUc->spm_vpls_uc_node);     	
    	spm_free_vpls_uc_cb(pstVplsUc NBB_CCXT);
    }
}


#endif
