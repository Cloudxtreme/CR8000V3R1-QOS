/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

******************************************************************************
   �� �� ��   : spm_ptp_proc.c
   �� �� ��   : ����
   ��    ��   : zengliang
   ��������   : 2013��6��28��
   ����޸�   : ��
   ��������   : ʱ��ͬ���������ô���
   �����б�   : ��
   �޸���ʷ   :
   1.��    ��   : 2013��6��28��
     ��    ��   : zengliang
     �޸�����   : �����ļ�

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char clk_sync_base_print_setting;

#ifdef SRC
/*1588 APPL�����ӿ��ⲿ����*/
extern NBB_LONG spm_ptp_set_global_config(IN ATG_DCI_CLK_SYNC_PARA *pstSyncParaData, \
                                                IN ATG_DCI_LOCAL_CLK *pstLocalClkData, \
                                                IN ATG_DCI_PPS_TOD_IF_DATA *pstPpstodIfData);
extern NBB_LONG spm_ptp_set_port_config(IN NBB_BYTE ucSlot, IN NBB_BYTE ucSubBoard, IN NBB_BYTE ucPhyPortNum, IN NBB_BYTE ucLogicPortNum, IN ATG_DCI_PHY_PORT_TIME_SYNC_DATA *pstPortCfg);
extern NBB_LONG spm_ptp_delete_port_config(IN NBB_BYTE ucSlot, IN NBB_BYTE ucSubBoard, IN NBB_BYTE ucPhyPortNum, IN NBB_BYTE ucLogicPortNum);
extern NBB_INT spm_ptp_main_init();
#endif

/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_clk_sync_base
 ��������  : ����PTP IPS��Ϣ
 �������  : ATG_DCI_SET_CLK_SYNC_BASE *pstSetClkSyncBase  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��6��18��
    ��    ��   : zengliang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_clk_sync_base(ATG_DCI_SET_CLK_SYNC_BASE *pstSetClkSyncBase)
{
    //NBB_BYTE ucIfExist = ATG_DCI_EXIST;
    NBB_ULONG ulPtpKey = 0;
    
    /* ��ȡ�������� */
    ATG_DCI_CLK_SYNC_PARA *pstSyncParaData = NULL;
    ATG_DCI_LOCAL_CLK *pstLocalClkData = NULL;
    ATG_DCI_PPS_TOD_IF_DATA *pstPpstodIfData = NULL;

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *pucSyncParaDataStart = NULL;
    NBB_BYTE *pucLocalClkDataStart = NULL;
    NBB_BYTE *pucPpstodIfDataStart = NULL;

    /* �����õĲ���ģʽ */
    NBB_ULONG ulOperSyncPara = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLocalClk = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPpstodIf = ATG_DCI_OPER_NULL;

    /* ��ȡ�������÷���ֵ*/
    NBB_LONG lSyncParaRc = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_LONG lLocalClkRc = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_LONG lPpstodIfRc = ATG_DCI_RC_UNSUCCESSFUL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    NBB_TRC_ENTRY("spm_rcv_dci_set_clk_sync_base");

    /* �������ָ�������Ч */
    NBB_ASSERT(pstSetClkSyncBase != NULL);

    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pstSetClkSyncBase->return_code = ATG_DCI_RC_OK;

    ulPtpKey = pstSetClkSyncBase->key;
    NBB_TRC_DETAIL((NBB_FORMAT "ulPtpKey = %ld", ulPtpKey));

#ifdef SRC

    /* ��ȡ�����ò��� */
    ulOperSyncPara = pstSetClkSyncBase->oper_clk_sync_para;
    ulOperLocalClk = pstSetClkSyncBase->oper_local_clk;
    ulOperPpstodIf = pstSetClkSyncBase->oper_pps_tod_if;

    /* ɾ��������Ŀ */
    if (pstSetClkSyncBase->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "ɾ�����е�ʱ��ͬ����������"));
        spm_dbg_print_ptp_cfg_head(ulPtpKey, SPM_OPER_DEL);
        spm_dbg_record_ptp_cfg_head(ulPtpKey, SPM_OPER_DEL);
        
        /*���ԭ������*/
	 OS_MEMSET(&(SHARED.ptp_cb), 0, sizeof(SPM_PTP_CB));

        /* �����Ŀ�����ڣ���ɾ�� */
        //if (ucIfExist == ATG_DCI_UNEXIST)
        //{
            /* ���޸ģ��Ӵ�ӡPD */
            //NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:Ҫɾ����ʱ��ͬ���������ò�������"));
            //pstSetClkSyncBase->return_code = ATG_DCI_RC_OK;

            //goto EXIT_LABEL;
        //}
        /* ����ɾ�� */
        //else
        //{
            /* todo list */
        //}
        
    }
    /* ���ӻ������Ŀ */
    else
    {
		/*���ԭ������*/
		//OS_MEMSET(&(SHARED.ptp_cb), 0, sizeof(SPM_PTP_CB));
	 if(ATG_DCI_UNEXIST == SHARED.ptp_cb.ptp_id_key)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "����ʱ��ͬ����������"));
            spm_dbg_print_ptp_cfg_head(ulPtpKey, SPM_OPER_ADD);
            spm_dbg_record_ptp_cfg_head(ulPtpKey, SPM_OPER_ADD);	            
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "����ʱ��ͬ����������"));
            spm_dbg_print_ptp_cfg_head(ulPtpKey, SPM_OPER_UPD);
            spm_dbg_record_ptp_cfg_head(ulPtpKey, SPM_OPER_UPD);	            
        }
	
        SHARED.ptp_cb.ptp_id_key = ulPtpKey;
            
        /***************************************************************************/
        /*                        ��ȡ���ã���������                               */
        /***************************************************************************/
        /******************************** ͬ���������� *********************************/
        if (ulOperSyncPara == ATG_DCI_OPER_ADD)
        {

            /* �����һ��entry�ĵ�ַ��*/
            pucSyncParaDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetClkSyncBase,
                &pstSetClkSyncBase->clk_sync_para);

            /* �׵�ַΪNULL���쳣 */
            if (pucSyncParaDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "pucSyncParaDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstSyncParaData = (ATG_DCI_CLK_SYNC_PARA *)pucSyncParaDataStart;
                if (clk_sync_base_print_setting == SPM_PRINT_CFG)
                {
                    spm_dbg_print_sync_para_cfg(pstSyncParaData);
                }
                
                memcpy(&(SHARED.ptp_cb.sync_para_ptp_cb), pstSyncParaData, sizeof(ATG_DCI_CLK_SYNC_PARA));

				lSyncParaRc = ATG_DCI_RC_OK;
            }
        }
        else if (ulOperSyncPara == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperSyncPara == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** ����ʱ������ ********************************/
        if (ulOperLocalClk == ATG_DCI_OPER_ADD)
        {

            /* �����һ��entry�ĵ�ַ��*/
            pucLocalClkDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetClkSyncBase,
                &pstSetClkSyncBase->local_clk);

            /* �׵�ַΪNULL���쳣 */
            if (pucLocalClkDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "pucLocalClkDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstLocalClkData = (ATG_DCI_LOCAL_CLK *)pucLocalClkDataStart;
                if (clk_sync_base_print_setting == SPM_PRINT_CFG)
                {
                    spm_dbg_print_local_clk_cfg(pstLocalClkData);
                }
                
                memcpy(&(SHARED.ptp_cb.local_clk_ptp_cb), pstLocalClkData, sizeof(ATG_DCI_LOCAL_CLK));

				lLocalClkRc = ATG_DCI_RC_OK;
            }
        }
        else if (ulOperLocalClk == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperLocalClk == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /******************************** PPS+TOD�ӿ����� ********************************/
        if (ulOperPpstodIf == ATG_DCI_OPER_ADD)
        {

            /* �����һ��entry�ĵ�ַ��*/
            pucPpstodIfDataStart = (NBB_BYTE *)NTL_OFFLEN_GET_POINTER(pstSetClkSyncBase,
                &pstSetClkSyncBase->pps_tod_if);

            /* �׵�ַΪNULL���쳣 */
            if (pucPpstodIfDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "pucPpstodIfDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstPpstodIfData = (ATG_DCI_PPS_TOD_IF_DATA*)pucPpstodIfDataStart;
                if (clk_sync_base_print_setting == SPM_PRINT_CFG)
                {
                    spm_dbg_print_pps_tod_if_cfg(pstPpstodIfData);
                }
                
                memcpy(&(SHARED.ptp_cb.pps_tod_if_ptp_cb), pstPpstodIfData, sizeof(ATG_DCI_PPS_TOD_IF_DATA));
                
				lPpstodIfRc = ATG_DCI_RC_OK;
            }
        }
        else if (ulOperPpstodIf == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperPpstodIf == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

        /*����ȫ����ȡ�ɹ�*/
		if ((lSyncParaRc == ATG_DCI_RC_OK) || (lLocalClkRc == ATG_DCI_RC_OK) || (lPpstodIfRc == ATG_DCI_RC_OK))
		{
    		pstSetClkSyncBase->return_code = spm_ptp_set_global_config(pstSyncParaData, pstLocalClkData, pstPpstodIfData);
		}
		else
		{
    		pstSetClkSyncBase->return_code = ATG_DCI_RC_UNSUCCESSFUL;
		}
    }
#endif
    
	EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/**************************************************************************
    ������: spm_ptp_set_port
    ��������: ���¶˿�����
    �������: ucSlot-->��λ��
    		  ucSubBoard-->�ӿ���
    		  ucPhyPortNum-->���˿ں�
    		  ucLogicPortNum-->�߼��˿ں�
    		  pstPortCfg-->�˿�����
    �������: ��
    ����ֵ: ATG_DCI_RC_OK/ATG_DCI_RC_UNSUCCESSFUL
    ���ú���: ��
    ��������: ��
    
    �޸���ʷ: 
    1. ����:2013��6��8��
       ����:zengliang
       �޸�����:�����º���
    
**************************************************************************/
NBB_LONG spm_ptp_set_port(NBB_BYTE ucSlot, NBB_BYTE ucSubBoard, NBB_BYTE ucPhyPortNum, NBB_BYTE ucLogicPortNum, ATG_DCI_PHY_PORT_TIME_SYNC_DATA *pstPortCfg)
{
    NBB_LONG lRc = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();    
    NBB_TRC_ENTRY("spm_ptp_set_port");

#ifdef SRC
	lRc = spm_ptp_set_port_config(ucSlot, ucSubBoard, ucPhyPortNum, ucLogicPortNum, pstPortCfg);
#endif
    
	NBB_TRC_EXIT();

    return lRc;
}

/**************************************************************************
    ������: spm_ptp_delete_port
    ��������: ɾ���˿�����
    �������: ucSlot-->��λ��
    		  ucSubBoard-->�ӿ���
    		  ucPhyPortNum-->���˿ں�
    		  ucLogicPortNum-->�߼��˿ں�
    �������: ��
    ����ֵ: ATG_DCI_RC_OK/ATG_DCI_RC_UNSUCCESSFUL
    ���ú���: ��
    ��������: ��
    
    �޸���ʷ: 
    1. ����:2013��6��8��
       ����:zengliang
       �޸�����:�����º���
    
**************************************************************************/
NBB_LONG spm_ptp_delete_port(NBB_BYTE ucSlot, NBB_BYTE ucSubBoard, NBB_BYTE ucPhyPortNum, NBB_BYTE ucLogicPortNum)
{
    NBB_LONG lRc = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();    
    NBB_TRC_ENTRY("spm_ptp_delete_port");

#ifdef SRC
	lRc = spm_ptp_delete_port_config(ucSlot, ucSubBoard, ucPhyPortNum, ucLogicPortNum);
#endif
    
	NBB_TRC_EXIT();

    return lRc;
}

/*****************************************************************************
 �� �� ��  : spm_init_ptp
 ��������  : ��ʼ��1588���ýṹ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
    1. ����:2013��6��8��
       ����:zengliang
       �޸�����:�����º���

*****************************************************************************/
NBB_VOID spm_init_ptp()
{
    //NBB_TRC_ENTRY("spm_init_ptp");
    
#ifdef SRC
    spm_ptp_main_init();
#endif

	//NBB_TRC_EXIT();
}

