/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_vpls_mc_proc.c
  �� �� ��   : ����
  ��    ��   : xiaoxiang
  ��������   : 2013��1��12��
  ����޸�   :
  ��������   : VPLS�鲥���ô���
  �����б�   :
  �޸���ʷ   :
  1.��    ��   : 2013��1��12��
    ��    ��   : xiaoxiang
    �޸�����   : �����ļ�

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef VPLS_MC_CFG

extern unsigned char vpls_mc_cfg_print_setting;

/*****************************************************************************
 �� �� ��  : spm_rcv_dci_set_vpls_mc
 ��������  : VPLS��̬�鲥���ô���
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
NBB_VOID spm_rcv_dci_set_vpls_mc(ATG_DCI_SET_VPLS_MC *pstSetVplsMc NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_ULONG i = 0;
    NBB_ULONG j = 0;
    NBB_INT ret = SUCCESS;
    NBB_USHORT usMcId = 0;
    NBB_UINT iL2mcPosId = 0;
    NBB_UINT iVpId = 0;
    NBB_INT  iSlotNum = 0;
    NBB_BYTE ucSlot = 0;
    NBB_BYTE ucC3Unit = 0;
    
    NBB_USHORT usNniPosId[ATG_DCI_VPLS_MC_NNI_NUM];
    NBB_USHORT usUniPosId[ATG_DCI_VPLS_MC_UNI_NUM];
    
    SPM_VPLS_MC_CB *pstVplsMc = NULL;

    /* ���������IPS�л�ȡ�������� */
    ATG_DCI_VPLS_MC_BASIC_DATA *pstBasicData = NULL;
    ATG_DCI_VPLS_MC_NNI_DATA *pstNniData[ATG_DCI_VPLS_MC_NNI_NUM];
    ATG_DCI_VPLS_MC_UNI_DATA *pstUniData[ATG_DCI_VPLS_MC_UNI_NUM];  

    /* IPS��Ϣƫ�Ƶ��׵�ַ */
    NBB_BYTE *pucBasicDataStart = NULL;
    NBB_BYTE *pucNniDataStart   = NULL;
    NBB_BYTE *pucUniDataStart   = NULL;

    /* �����õĲ���ģʽ */
    NBB_ULONG ulOperBasic = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperNni   = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperUni   = ATG_DCI_OPER_NULL;

    /* VPLS�鲥�ļ�ֵ */
    ATG_DCI_VPLS_MC_KEY stVplsMcKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_vpls_mc");

    /* �������ָ�������Ч */
    NBB_ASSERT(pstSetVplsMc != NULL);

	if (pstSetVplsMc == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_vpls_mc(pstSetVplsMc==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_vpls_mc(pstSetVplsMc==NULL)\n");
                                                                        
        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_vpls_mc(pstSetVplsMc==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", 0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        goto EXIT_LABEL;
    }

    //���Ƚ�IPS��Ϣ�ķ���ֵ����ΪOK�������һ��������ʧ�ܣ�����ΪFAIL
    pstSetVplsMc->return_code = ATG_DCI_RC_OK;

    //��ʼ��NNIָ������ 
    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        pstNniData[i] = NULL;
        usNniPosId[i] = 0;
    }

    //��ʼ��UNIָ������ 
    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        pstUniData[i] = NULL;
        usUniPosId[i] = 0;        
    }

    OS_MEMCPY(&stVplsMcKey, &pstSetVplsMc->key, sizeof(ATG_DCI_VPLS_MC_KEY));
    //stVplsMcKey = pstSetVplsMc->key;    
    pstVplsMc = AVLL_FIND(SHARED.vpls_mc_tree, &stVplsMcKey);
    
    if (pstVplsMc == NULL)    //��Ŀ������
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    
    
    /* ��ȡ�����õĲ������� */
    ulOperBasic = pstSetVplsMc->oper_basic;
    ulOperNni   = pstSetVplsMc->oper_nni_member;
    ulOperUni   = pstSetVplsMc->oper_uni_member;


    /* ɾ��������Ŀ */
    if (pstSetVplsMc->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ɾ��VPLS_ID=%d  MAC_ADDRESS=%x-%x-%x-%x-%x-%x�� VPLS�鲥����", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]));        

        /* �����Ŀ�����ڣ���ɾ�� */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            /* ���޸ģ��Ӵ�ӡPD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:Ҫɾ�������ò������ڣ��˳�"));

            OS_PRINTF("***ERROR***:Ҫɾ����VPLS_ID=%ld �� VPLS�鲥���ò�����!\n", stVplsMcKey.vpls_id);
                                                                            
            OS_SPRINTF(ucMessage, "***ERROR***:Ҫɾ����VPLS_ID=%ld �� VPLS�鲥���ò�����!\n", stVplsMcKey.vpls_id);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            DEL_UNEXIST_CONFIG_ERROR, 
                                                                            "DEL_UNEXIST_CONFIG_ERROR", 
                                                                            ucMessage));
            goto EXIT_LABEL;
        }
        /* ���ڣ�ɾ�� */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ��ѯ�������ã�ɾ����VPLS�鲥������tree��ɾ���洢����"));
            spm_dbg_print_vpls_mc_head(&stVplsMcKey, SPM_OPER_DEL);
            spm_dbg_record_vpls_mc_head(&stVplsMcKey, SPM_OPER_DEL);

            //ɾ��VPLS�鲥�飬Ҫ��ɾ�����¹ҵ��鲥�˿��������Զ�ɾ��
            //Aradɾ���鲥����Զ�ɾ���¹Ҷ˿�
            if ((pstVplsMc != NULL) && (pstVplsMc->basic_cfg_cb != NULL))
            {
#ifdef SPU			
	            ret = ApiAradDeleteMc(UNIT_0, pstVplsMc->basic_cfg_cb->mc_id);

	            if (ret != SUCCESS)
	            {
	                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiAradDeleteMc() ret=%d", stVplsMcKey.vpls_id, 
	                                                                                                                                      pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                                      ret));

	                OS_PRINTF("***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiAradDeleteMc() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                      pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                      ret);

	                OS_SPRINTF(ucMessage, "***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiAradDeleteMc() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                      pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                      ret);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
	                                                                            stVplsMcKey.mac_address[0],
	                                                                            stVplsMcKey.mac_address[1],
	                                                                            stVplsMcKey.mac_address[2],
	                                                                            stVplsMcKey.mac_address[3],
	                                                                            stVplsMcKey.mac_address[4],
	                                                                            stVplsMcKey.mac_address[5],
	                                                                            ret, 
	                                                                            "CALL_ARAD_FUNC_ERROR", 
	                                                                            ucMessage));
	                pstSetVplsMc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                goto EXIT_LABEL;
	            }

#if 0	//�����鲥�����޸ģ����ڿ��Բ����鲥����intf
	            //C3������鲥�˿�Ҫ�ֶ�ɾ��
	            //�ͷŸ�NNI�˿ڷ����CUD
	            for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
	            {
	                //���������0��֤��������CUD
	                if (pstVplsMc->nni_info_cb[i].cud == 0)
	                    continue;

	                spm_free_vpls_mc_cud(pstVplsMc->nni_info_cb[i].cud);

	                for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
	                {
	                    ret = ApiC3DelL2MCOutIntf(ucC3Unit, pstVplsMc->nni_info_cb[i].vp_pos_id);

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelL2MCOutIntf() UNI ret=%d", ret));
	                }
	            }

	            //�ͷŸ�UNI�˿ڷ����CUD
	            for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
	            {
	                //���������0��֤��������CUD
	                if (pstVplsMc->uni_info_cb[i].cud == 0)
	                    continue;

	                spm_free_vpls_mc_cud(pstVplsMc->uni_info_cb[i].cud);

	                for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
	                {
	                    ret = ApiC3DelL2MCOutIntf(ucC3Unit, pstVplsMc->uni_info_cb[i].vp_pos_id);

	                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelL2MCOutIntf() NNI ret=%d", ret));
	                }
	            }
#endif
	            for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
	            {
	                ret = ApiC3DelStaticMac(ucC3Unit, 
	                                        pstVplsMc->vpls_mc_key.mac_address, 
	                                        pstVplsMc->vpls_mc_key.vpls_id);
	                                
	                NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelStaticMac() ret=%d", ret));
	            }

#endif

#ifdef SRC 
	            /* ɾ��VPLS�鲥�飬Ҫ��ɾ�����¹ҵ��鲥�˿��������Զ�ɾ��*/ 
	            ret = ApiDfeMulticastDestroy(UNIT_0, pstVplsMc->basic_cfg_cb->mc_id);

	            if (ret != SUCCESS)
	            {
	                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiDfeMulticastDestroy() ret=%d", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret));

	                OS_PRINTF("***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiDfeMulticastDestroy() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret);

	                OS_SPRINTF(ucMessage, "***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiDfeMulticastDestroy() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
	                                                                            stVplsMcKey.mac_address[0],
	                                                                            stVplsMcKey.mac_address[1],
	                                                                            stVplsMcKey.mac_address[2],
	                                                                            stVplsMcKey.mac_address[3],
	                                                                            stVplsMcKey.mac_address[4],
	                                                                            stVplsMcKey.mac_address[5],
	                                                                            ret, 
	                                                                            "CALL_ARAD_FUNC_ERROR", 
	                                                                            ucMessage));
	                pstSetVplsMc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                goto EXIT_LABEL;
	            }
#endif

#ifdef SNU
	            /* ɾ��VPLS�鲥�飬Ҫ��ɾ�����¹ҵ��鲥�˿��������Զ�ɾ��*/ 
	            ret = ApiDfeMulticastDestroy(UNIT_0, pstVplsMc->basic_cfg_cb->mc_id);

	            if (ret != SUCCESS)
	            {
	                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiDfeMulticastDestroy() ret=%d", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret));

	                OS_PRINTF("***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiDfeMulticastDestroy() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret);

	                OS_SPRINTF(ucMessage, "***ERROR***:ɾ��VPLS_MC(VPLS_ID=%d MC_ID=%d)���鲥��ʧ��! >>> ApiDfeMulticastDestroy() ret=%d\n", stVplsMcKey.vpls_id, 
	                                                                                                                               pstVplsMc->basic_cfg_cb->mc_id,
	                                                                                                                               ret);
	                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
	                                                                            stVplsMcKey.mac_address[0],
	                                                                            stVplsMcKey.mac_address[1],
	                                                                            stVplsMcKey.mac_address[2],
	                                                                            stVplsMcKey.mac_address[3],
	                                                                            stVplsMcKey.mac_address[4],
	                                                                            stVplsMcKey.mac_address[5],
	                                                                            ret, 
	                                                                            "CALL_ARAD_FUNC_ERROR", 
	                                                                            ucMessage));
	                pstSetVplsMc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
	                goto EXIT_LABEL;
	            }
#endif
            }

			if (pstVplsMc != NULL)
			{
	            //������ɾ���ڵ�
	            AVLL_DELETE(SHARED.vpls_mc_tree, pstVplsMc->spm_vpls_mc_node); 

	            //�ͷŽڵ��ڴ�ռ�
	            spm_free_vpls_mc_cb(pstVplsMc NBB_CCXT);
			}
        }
    }
    /* ���ӻ������Ŀ */
    else
    {        
        
        /* �����Ŀ�����ڣ��������ڴ�ռ䱣������ */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ����VPLS_ID=%d  MAC_ADDRESS=%x-%x-%x-%x-%x-%x �� VPLS�鲥����", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]));
            spm_dbg_print_vpls_mc_head(&stVplsMcKey, SPM_OPER_ADD);
            spm_dbg_record_vpls_mc_head(&stVplsMcKey, SPM_OPER_ADD);
            
            /* ����һ������Ŀ���ڴ�ռ� */
            pstVplsMc = spm_alloc_vpls_mc_cb(NBB_CXT);

			if (pstVplsMc != NULL)
			{
	            //���뵽�ڵ���
	            OS_MEMCPY(&pstVplsMc->vpls_mc_key, &stVplsMcKey, sizeof(ATG_DCI_VPLS_MC_KEY));

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.vpls_mc_tree, pstVplsMc->spm_vpls_mc_node);
			}         
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  ����VPLS_ID=%d  MAC=%x-%x-%x-%x-%x-%x �� VPLS�鲥����", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]));          
            spm_dbg_print_vpls_mc_head(&stVplsMcKey, SPM_OPER_UPD);
            spm_dbg_record_vpls_mc_head(&stVplsMcKey, SPM_OPER_UPD);
        }

		//���Ӵ��жϣ�ȡ��PC-Lint�澯
        if (pstVplsMc == NULL)
        {
            /* ���޸ģ��Ӵ�ӡPD */
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:Ҫ���ӵ�VPLS_ID=%ld �� VPLS�鲥����δ�ܳɹ������ڴ�!", stVplsMcKey.vpls_id));

            OS_PRINTF("***ERROR***:Ҫ���ӵ�VPLS_ID=%ld �� VPLS�鲥����δ�ܳɹ������ڴ�!\n", stVplsMcKey.vpls_id);
                                                                            
            OS_SPRINTF(ucMessage, "***ERROR***:Ҫ���ӵ�VPLS_ID=%ld �� VPLS�鲥����δ�ܳɹ������ڴ�!\n", stVplsMcKey.vpls_id);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ALLOC_MEMORY_ERROR, 
                                                                            "ALLOC_MEMORY_ERROR", 
                                                                            ucMessage));
			pstSetVplsMc->return_code = ATG_DCI_RC_UNSUCCESSFUL;
			
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                              ��ȡ����                                   */
        /***************************************************************************/
        /******************************** �������� *********************************/
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* �����һ��entry�ĵ�ַ��*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->basic_data);    

            /* ���ָ��ΪNULL�������� */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucBasicDataStart is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));

                //���Ϊ�������һ�������δ�·�����ôֱ���˳���������
                if (ucIfExist == ATG_DCI_UNEXIST)
                {
                    OS_PRINTF("***ERROR***:�����鲥��<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>������������δ�·���ֱ���˳���������!\n", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]);

                    OS_SPRINTF(ucMessage, "***ERROR***:�����鲥��<VPLS_ID=%d,MAC=%x-%x-%x-%x-%x-%x>������������δ�·���ֱ���˳���������!\n", stVplsMcKey.vpls_id,
                                                                                                      stVplsMcKey.mac_address[0],
                                                                                                      stVplsMcKey.mac_address[1],
                                                                                                      stVplsMcKey.mac_address[2],
                                                                                                      stVplsMcKey.mac_address[3],
                                                                                                      stVplsMcKey.mac_address[4],
                                                                                                      stVplsMcKey.mac_address[5]);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            BASIC_CONFIG_NOT_EXIST, 
                                                                            "BASIC_CONFIG_NOT_EXIST", 
                                                                            ucMessage));
                                                                            
                    pstSetVplsMc->return_code = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                    pstSetVplsMc->basic_return_code = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;

                    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
                    {
                        pstSetVplsMc->nni_member_return_code[i] = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                    }

                    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
                    {
                        pstSetVplsMc->uni_member_return_code[i] = ATG_DCI_RC_VPLS_MC_NO_BASIC_DATA;
                    }
                    
                    goto EXIT_LABEL;
                }
            }
            else
            {
                pstBasicData = (ATG_DCI_VPLS_MC_BASIC_DATA *)pucBasicDataStart;

                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1����������\n");
                    spm_dbg_print_vpls_mc_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1����������\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_vpls_mc_basic_cfg(pstBasicData);

                /* ���ô���todo */
                NBB_TRC_DETAIL((NBB_FORMAT "  MC_ID = %d", pstBasicData->mc_id));   
                NBB_TRC_DETAIL((NBB_FORMAT "  ucIfExist = %d", ucIfExist));

#ifdef SPU
                if (ucIfExist == ATG_DCI_UNEXIST)
                {

                    ret = ApiAradSetupEngressMc(UNIT_0, pstBasicData->mc_id);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiAradSetupEngressMc() ret=%d", ret));

                    if (ret != SUCCESS)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiAradSetupEngressMc() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiAradSetupEngressMc() ret=%d\n", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***:ApiAradSetupEngressMc() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                                                            
                        pstSetVplsMc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                        goto EXIT_LABEL;
                    }
                }

                ret = SUCCESS;
                
                for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
                {
                    //�鲥ʱ����mcid��vpIdx=0
                    ret += ApiC3SetStaticMac(ucC3Unit, 
                                            stVplsMcKey.mac_address, 
                                            stVplsMcKey.vpls_id, 
                                            pstBasicData->mc_id, 
                                            0);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3SetStaticMac() ret=%d", ret));


                }
                
                if (ret != SUCCESS)
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetStaticMac() error! ret=%d", ret));

                    OS_PRINTF("***ERROR***: ApiC3SetStaticMac() ret=%d\n", ret);

                    OS_SPRINTF(ucMessage, "***ERROR***: ApiC3SetStaticMac() ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_C3_FUNC_ERROR", 
                                                                            ucMessage));
                                                                            
                    pstSetVplsMc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                }
#endif

#ifdef SRC
                if (ucIfExist == ATG_DCI_UNEXIST)
                {
                    ret = ApiDfeMulticastCreate(UNIT_0, pstBasicData->mc_id);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastCreate() ret=%d", ret));

                    if (ret != SUCCESS)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiDfeMulticastCreate() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiDfeMulticastCreate() ret=%d\n", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***:ApiDfeMulticastCreate() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                                                            
                        pstSetVplsMc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                        goto EXIT_LABEL;
                    }
                }                
#endif

#ifdef SNU
                if (ucIfExist == ATG_DCI_UNEXIST)
                {
                    ret = ApiDfeMulticastCreate(UNIT_0, pstBasicData->mc_id);

                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastCreate() ret=%d", ret));

                    if (ret != SUCCESS)
                    {
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiDfeMulticastCreate() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:ApiDfeMulticastCreate() ret=%d\n", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***:ApiDfeMulticastCreate() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                                                            
                        pstSetVplsMc->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

                        goto EXIT_LABEL;
                    }
                }   
#endif
                if (pstVplsMc->basic_cfg_cb == NULL)
                {
                    pstVplsMc->basic_cfg_cb = (ATG_DCI_VPLS_MC_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_MC_BASIC_DATA),
                                                            NBB_NORETRY_ACT,
                                                            MEM_SPM_VPLS_MC_BASIC_CB);
                }  

                //��������
                OS_MEMCPY(pstVplsMc->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_VPLS_MC_BASIC_DATA));

                usMcId = pstBasicData->mc_id;

                //pstBasicData->return_code = ATG_DCI_RC_OK;
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
        
        /******************************** NNI���� *********************************/
        if (ulOperNni == ATG_DCI_OPER_ADD)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  NNI ADD")); 
                   
            /* �����һ��entry�ĵ�ַ��*/
            pucNniDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->nni_member_data);    

            /* ���ָ��ΪNULL�������� */
            if (pucNniDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucNniDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2��NNI�ӿ�����[1-512] (nni_num=%d) ADD\n", pstSetVplsMc->nni_member_num);
                }

                OS_SPRINTF(ucMessage,"  2��NNI�ӿ�����[1-512] (nni_num=%d) ADD\n", pstSetVplsMc->nni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                for (i=0; i<pstSetVplsMc->nni_member_num; i++)
                {
                    pstNniData[i] = (ATG_DCI_VPLS_MC_NNI_DATA *)
                                  (pucNniDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_MC_NNI_DATA))) * i); 

                    if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_mc_nni_cfg(pstNniData[i], i);
                    }

                    //spm_dbg_record_vpls_mc_nni_cfg(pstNniData[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC ID = %ld", pstNniData[i]->vc_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP = %s", spm_set_ulong_to_ipv4(pstNniData[i]->peer_ip NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE = %d", pstNniData[i]->vc_type));

                    iL2mcPosId = 0;
                    iVpId = 0;
                    ucSlot = 0;
                    
                    ret = spm_vpls_mc_add_vp_nni(pstVplsMc->basic_cfg_cb->mc_id, 
                                                 pstNniData[i], 
                                                 &iL2mcPosId, 
                                                 &iVpId, 
                                                 &ucSlot
                                                 NBB_CCXT);

                    //�ɹ�����������
                    if (ret == SUCCESS)
                    {
                        //��NNI����������һ���洢λ��
                        for (j=0; j<ATG_DCI_VPLS_MC_NNI_NUM; j++)
                        {
                            //ΪNULL��˵��δռ�ã����Դ洢
                            if (pstVplsMc->nni_cfg_cb[j] == NULL)
                            {
                                pstVplsMc->nni_cfg_cb[j] = (ATG_DCI_VPLS_MC_NNI_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_MC_NNI_DATA),
                                                                        NBB_NORETRY_ACT,
                                                                        MEM_SPM_VPLS_MC_NNI_CB);

                                pstVplsMc->nni_info_cb[j].vp_pos_id = iVpId;
                                //pstVplsMc->nni_info_cb[j].cud = iMcCud;
                                pstVplsMc->nni_info_cb[j].slot_id = ucSlot;
                                pstVplsMc->nni_num++;
                                
                                OS_MEMCPY(pstVplsMc->nni_cfg_cb[j], pstNniData[i], sizeof(ATG_DCI_VPLS_MC_NNI_DATA));

                                break;
                            }
                        }
                    }
                    else    //�������NNI��ʧ�ܼ�¼����״̬
                    {
                        pstSetVplsMc->nni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_add_vp_nni() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***:spm_vpls_mc_add_vp_nni() ret=%d\n", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_add_vp_nni() ret=%d\n", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    }
                }
            }                
        }
        else if (ulOperNni == ATG_DCI_OPER_DEL)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  NNI DEL"));

            //�����Keyֵ��VPLS�鲥���ò����ڣ����޷�ɾ���鲥���ڵĶ˿�
            if (ucIfExist == ATG_DCI_UNEXIST)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:����ɾ���鲥���ڶ˿�ʱ���鲥Key VPLS_ID=%d ���鲥�鲻���ڣ��޷�����!", stVplsMcKey.vpls_id));

                OS_PRINTF("***ERROR***:����ɾ���鲥���ڶ˿�ʱ���鲥Key VPLS_ID=%d ���鲥�鲻���ڣ��޷�����!\n", stVplsMcKey.vpls_id);
                                                                            
                OS_SPRINTF(ucMessage, "***ERROR***:����ɾ���鲥���ڶ˿�ʱ���鲥Key VPLS_ID=%d ���鲥�鲻���ڣ��޷�����!\n", stVplsMcKey.vpls_id);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            BASIC_CONFIG_NOT_EXIST, 
                                                                            "BASIC_CONFIG_NOT_EXIST", 
                                                                            ucMessage));
                                                                            
                goto EXIT_LABEL;
            }
                   
            /* �����һ��entry�ĵ�ַ��*/
            pucNniDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->nni_member_data);    

            /* ���ָ��ΪNULL�������� */
            if (pucNniDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucNniDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  2��NNI�ӿ�����[1-512] (nni_num=%d) DEL\n", pstSetVplsMc->nni_member_num);
                }

                OS_SPRINTF(ucMessage,"  2��NNI�ӿ�����[1-512] (nni_num=%d) DEL\n", pstSetVplsMc->nni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                for (i=0; i<pstSetVplsMc->nni_member_num; i++)
                {
                    pstNniData[i] = (ATG_DCI_VPLS_MC_NNI_DATA *)
                                  (pucNniDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_MC_NNI_DATA))) * i); 

                    if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_mc_nni_cfg(pstNniData[i], i);
                    }

                    //spm_dbg_record_vpls_mc_nni_cfg(pstNniData[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC ID = %ld", pstNniData[i]->vc_id));
                    NBB_TRC_DETAIL((NBB_FORMAT "  PEER_IP = %s", spm_set_ulong_to_ipv4(pstNniData[i]->peer_ip NBB_CCXT)));
                    NBB_TRC_DETAIL((NBB_FORMAT "  VC_TYPE = %d", pstNniData[i]->vc_type));


                    ret = spm_vpls_mc_del_vp_nni(pstVplsMc->basic_cfg_cb->mc_id, 
                                                 pstVplsMc, 
                                                 pstNniData[i]
                                                 NBB_CCXT);

                    //�ɹ���ɾ������
                    if (ret == SUCCESS)
                    {
                        //�ҵ�Ҫɾ����NNI�˿�
                        for (j=0; j<ATG_DCI_VPLS_MC_NNI_NUM; j++)
                        {
                            //keyֵ��ȣ�˵���ҵ�
                            if ((pstVplsMc->nni_cfg_cb[j]->vc_id   == pstNniData[i]->vc_id) 
                             && (pstVplsMc->nni_cfg_cb[j]->vc_type == pstNniData[i]->vc_type)
                             && (pstVplsMc->nni_cfg_cb[j]->peer_ip == pstNniData[i]->peer_ip))
                            {
                                ucSlot = pstVplsMc->nni_info_cb[j].slot_id;
                                iSlotNum = spm_get_vpls_mc_slot_num(pstVplsMc, ucSlot NBB_CCXT);
                                
                                //free
                                NBB_MM_FREE(pstVplsMc->nni_cfg_cb[j], MEM_SPM_VPLS_MC_NNI_CB);                                
                                
                                pstVplsMc->nni_cfg_cb[j] = NULL;
                                pstVplsMc->nni_num--;
                                
#ifdef SRC
                                if (iSlotNum == 0)
                                {
                                    //unit?
                                    ret = ApiDfeMulticastDelete(UNIT_0, 
                                                                pstVplsMc->basic_cfg_cb->mc_id, 
                                                                ucSlot);

                                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() ret=%d", ret));
                                    
                                    if (ret != SUCCESS)
                                    {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastDelete() error! ret=%d", ret));

                                        OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);

                                        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                        ret = ERROR;
                                        
                                        //goto EXIT_LABEL;
                                    }
                                }
#endif

#ifdef SNU
                                if (iSlotNum == 0)
                                {
                                    //unit?
                                    ret = ApiDfeMulticastDelete(UNIT_0, 
                                                                pstVplsMc->basic_cfg_cb->mc_id, 
                                                                ucSlot);

                                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() ret=%d", ret));
                                    
                                    if (ret != SUCCESS)
                                    {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastDelete() error! ret=%d", ret));

                                        OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);

                                        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_ARAD_FUNC_ERROR", 
                                                                            ucMessage));
                                        ret = ERROR;
                                        
                                        //goto EXIT_LABEL;
                                    }
                                }
#endif
                                break;
                            }
                        }
                    }
                    else    //�������NNI��ʧ�ܼ�¼����״̬
                    {
                        pstSetVplsMc->nni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_del_vp_nni() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***: spm_vpls_mc_del_vp_nni() ret=%d", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_del_vp_nni() ret=%d", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    }
                }
            }                
        }
        else if (ulOperNni == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }  


        /******************************** UNI���� *********************************/        
        if (ulOperUni == ATG_DCI_OPER_ADD)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  UNI ADD")); 

            /* �����һ��entry�ĵ�ַ��*/
            pucUniDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->uni_member_data);    

            /* ���ָ��ΪNULL�������� */
            if (pucUniDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucUniDataStart is NULL."));
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  3��UNI�ӿ�����[1-64] (uni_num=%d) ADD\n", pstSetVplsMc->uni_member_num);
                }

                OS_SPRINTF(ucMessage,"  3��UNI�ӿ�����[1-64] (uni_num=%d) ADD\n", pstSetVplsMc->uni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                for (i=0; i<pstSetVplsMc->uni_member_num; i++)
                {
                    pstUniData[i] = (ATG_DCI_VPLS_MC_UNI_DATA *)
                                  (pucUniDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_MC_UNI_DATA))) * i); 

                    if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_mc_uni_cfg(pstUniData[i], i);
                    }

                    //spm_dbg_record_vpls_mc_uni_cfg(pstUniData[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  �˿�Index = %ld", pstUniData[i]->port_index));

                    iL2mcPosId = 0;
                    iVpId = 0;
                    ucSlot = 0;
                    
                    ret = spm_vpls_mc_add_vp_uni(pstVplsMc->basic_cfg_cb->mc_id, 
                                                 pstUniData[i], 
                                                 &iL2mcPosId, 
                                                 &iVpId, 
                                                 &ucSlot
                                                 NBB_CCXT);
                
                    //����ɹ�����������
                    if (ret == SUCCESS)
                    {
                        //��UNI����������һ���洢λ��
                        for (j=0; j<ATG_DCI_VPLS_MC_UNI_NUM; j++)
                        {
                            //ΪNULL��˵��δռ�ã����Դ洢
                            if (pstVplsMc->uni_cfg_cb[j] == NULL)
                            {
                                pstVplsMc->uni_cfg_cb[j] = (ATG_DCI_VPLS_MC_UNI_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_VPLS_MC_UNI_DATA),
                                                                        NBB_NORETRY_ACT,
                                                                        MEM_SPM_VPLS_MC_UNI_CB);

                                pstVplsMc->uni_info_cb[j].vp_pos_id = iVpId;
                                //pstVplsMc->uni_info_cb[j].cud = iMcCud;
                                pstVplsMc->uni_info_cb[j].slot_id = ucSlot;
                                pstVplsMc->uni_num++;
                                
                                OS_MEMCPY(pstVplsMc->uni_cfg_cb[j], pstUniData[i], sizeof(ATG_DCI_VPLS_MC_UNI_DATA));

                                break;
                            }
                        }
                    }
                    else    //�������UNI��ʧ�ܼ�¼����״̬
                    {
                        pstSetVplsMc->uni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;
                        
                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_add_vp_uni() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***: spm_vpls_mc_add_vp_uni() ret=%d", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_add_vp_uni() ret=%d", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    }
                }
            }             
        }
        else if (ulOperUni == ATG_DCI_OPER_DEL)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  UNI DEL"));

            //�����Keyֵ��VPLS�鲥���ò����ڣ����޷�ɾ���鲥���ڵĶ˿�
            if (ucIfExist == ATG_DCI_UNEXIST)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:����ɾ���鲥���ڶ˿�ʱ���鲥Key VPLS_ID=%d ���鲥�鲻���ڣ��޷�����!", stVplsMcKey.vpls_id));

                OS_PRINTF("***ERROR***:����ɾ���鲥���ڶ˿�ʱ���鲥Key VPLS_ID=%d ���鲥�鲻���ڣ��޷�����!\n", stVplsMcKey.vpls_id);

                OS_SPRINTF(ucMessage, "***ERROR***:����ɾ���鲥���ڶ˿�ʱ���鲥Key VPLS_ID=%d ���鲥�鲻���ڣ��޷�����!\n", stVplsMcKey.vpls_id);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            BASIC_CONFIG_NOT_EXIST, 
                                                                            "BASIC_CONFIG_NOT_EXIST", 
                                                                            ucMessage));
                goto EXIT_LABEL;
            }
                   
            /* �����һ��entry�ĵ�ַ��*/
            pucUniDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetVplsMc, &pstSetVplsMc->uni_member_data);    

            /* ���ָ��ΪNULL�������� */
            if (pucUniDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  pucUniDataStart is NULL."));  
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  3��UNI�ӿ�����[1-64] (uni_num=%d) DEL\n", pstSetVplsMc->uni_member_num);
                }

                OS_SPRINTF(ucMessage,"  3��UNI�ӿ�����[1-64] (uni_num=%d) DEL\n", pstSetVplsMc->uni_member_num);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                for (i=0; i<pstSetVplsMc->uni_member_num; i++)
                {
                    pstUniData[i] = (ATG_DCI_VPLS_MC_UNI_DATA *)
                                  (pucUniDataStart + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VPLS_MC_UNI_DATA))) * i); 

                    if (vpls_mc_cfg_print_setting == SPM_PRINT_CFG)
                    {
                        spm_dbg_print_vpls_mc_uni_cfg(pstUniData[i], i);
                    }

                    //spm_dbg_record_vpls_mc_uni_cfg(pstUniData[i], i);
                    
                    NBB_TRC_DETAIL((NBB_FORMAT "  UNI PORT_INDEX = %ld", pstUniData[i]->port_index));

                    ret = spm_vpls_mc_del_vp_uni(pstVplsMc->basic_cfg_cb->mc_id, 
                                                 pstVplsMc, 
                                                 pstUniData[i]
                                                 NBB_CCXT);

                    //�ɹ���ɾ������
                    if (ret == SUCCESS)
                    {
                        //�ҵ�Ҫɾ����UNI�ӿ�
                        for (j=0; j<ATG_DCI_VPLS_MC_UNI_NUM; j++)
                        {
                            //�ҵ�
                            if (pstVplsMc->uni_cfg_cb[j]->port_index == pstUniData[i]->port_index)
                            {
                                ucSlot = pstVplsMc->uni_info_cb[j].slot_id;
                                iSlotNum = spm_get_vpls_mc_slot_num(pstVplsMc, ucSlot NBB_CCXT);
                                
                                //free
                                NBB_MM_FREE(pstVplsMc->uni_cfg_cb[j], MEM_SPM_VPLS_MC_UNI_CB);                                
                                
                                pstVplsMc->uni_cfg_cb[j] = NULL;
                                pstVplsMc->uni_num--;
                                
#ifdef SRC
                                if (iSlotNum == 0)
                                {
                                    //unit?
                                    ret = ApiDfeMulticastDelete(UNIT_0, 
                                                                pstVplsMc->basic_cfg_cb->mc_id, 
                                                                ucSlot);

                                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() ret=%d", ret));
                                    
                                    if (ret != SUCCESS)
                                    {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastDelete() error! ret=%d", ret));

                                        OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);

                                        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_C3_FUNC_ERROR", 
                                                                            ucMessage));
                                        ret = ERROR;
                                        
                                        //goto EXIT_LABEL;
                                    }
                                }
#endif

#ifdef SNU
                                if (iSlotNum == 0)
                                {
                                    //unit?
                                    ret = ApiDfeMulticastDelete(UNIT_0, 
                                                                pstVplsMc->basic_cfg_cb->mc_id, 
                                                                ucSlot);

                                    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastDelete() ret=%d", ret));
                                    
                                    if (ret != SUCCESS)
                                    {
                                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastDelete() error! ret=%d", ret));

                                        OS_PRINTF("***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);

                                        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastDelete() ret=%d", ret);
                                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_C3_FUNC_ERROR", 
                                                                            ucMessage));
                                        ret = ERROR;
                                        
                                        //goto EXIT_LABEL;
                                    }
                                }
#endif
                                break;
                            }
                        }
                    }
                    else    //�������NNI��ʧ�ܼ�¼����״̬
                    {
                        pstSetVplsMc->uni_member_return_code[i] = ATG_DCI_RC_UNSUCCESSFUL;

                        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_del_vp_uni() error! ret=%d", ret));

                        OS_PRINTF("***ERROR***: spm_vpls_mc_del_vp_uni() ret=%d", ret);

                        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_del_vp_uni() ret=%d", ret);
                        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                            stVplsMcKey.mac_address[0],
                                                                            stVplsMcKey.mac_address[1],
                                                                            stVplsMcKey.mac_address[2],
                                                                            stVplsMcKey.mac_address[3],
                                                                            stVplsMcKey.mac_address[4],
                                                                            stVplsMcKey.mac_address[5],
                                                                            ret, 
                                                                            "CALL_FUNC_ERROR", 
                                                                            ucMessage));
                    }
                }
            }                
        }
        else if (ulOperUni == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }

    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}

/*****************************************************************************
 �� �� ��  : spm_vpls_mc_add_vp_nni
 ��������  : ΪVPLS�鲥�����NNI�˿�
 �������  : ATG_DCI_VPLS_MC_KEY *pstVplsMcKey   
             ATG_DCI_VPLS_MC_NNI_DATA* pstVcKey  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_mc_add_vp_nni(NBB_USHORT usMcId, 
                               ATG_DCI_VPLS_MC_NNI_DATA* pstVplsMcNni,
                               NBB_UINT *piPosId,
                               NBB_UINT *piVpId,
                               NBB_BYTE *pucSlot
                               NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_INT iVpId = 0;
    NBB_BYTE ucC3Unit = 0;
    NBB_BYTE ucLagSlotId = 0;
    NBB_USHORT usLagPortId = 0;

    SPM_PORT_INFO_CB stPortInfo;
    
    L2MC_INTF_T stL2mcIntf;
    ATG_DCI_VC_KEY stVcKey;
    SPM_VC_CB *pstVcTbl;
    
    NBB_TRC_ENTRY("spm_vpls_mc_add_vp_nni");

    if (pstVplsMcNni == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_mc_add_vp_nni(pstVplsMcNni==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpls_mc_add_vp_nni(pstVplsMcNni==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_add_vp_nni(pstVplsMcNni==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stL2mcIntf, 0, sizeof(L2MC_INTF_T));
    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stVcKey, 0, sizeof(ATG_DCI_VC_KEY));

    stVcKey.vc_id   = pstVplsMcNni->vc_id;
    stVcKey.vc_type = pstVplsMcNni->vc_type;
    stVcKey.peer_ip = pstVplsMcNni->peer_ip;

    pstVcTbl = AVLL_FIND(SHARED.vc_tree, &stVcKey);
    
    if (pstVcTbl == NULL)       /* �����Ŀ������*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s ��VC�����ò����ڣ��޷���Ӵ��鲥�˿�!", stVcKey.vc_id,
                                                                                                                         stVcKey.vc_type,
                                                                                                                         spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT)));

        OS_PRINTF("***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s ��VC�����ò����ڣ��޷���Ӵ��鲥�˿�!\n", stVcKey.vc_id,
                                                                                                         stVcKey.vc_type,
                                                                                                         spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));

        OS_SPRINTF(ucMessage, "***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s ��VC�����ò����ڣ��޷���Ӵ��鲥�˿�!\n", stVcKey.vc_id,
                                                                                                         stVcKey.vc_type,
                                                                                                         spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        BASIC_CONFIG_NOT_EXIST, 
                                                                        "BASIC_CONFIG_NOT_EXIST", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //����VC�������е�port_index��ȡ�˿ڵ�port_id��slot_id�����Arad��FE1600
    spm_get_portid_from_logical_port_index(pstVcTbl->vc_info_cb.port_index, &stPortInfo NBB_CCXT);

#ifdef SRC

	//�����LAG����Ҫ�ҵ�LAG�µ�һ���˿ڵĲ�λ���·���FE1600
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucLagSlotId);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usMcId, stPortInfo.slot_id);
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastAdd() ret=%d", ret));
    
    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

#ifdef SNU
	//�����LAG����Ҫ�ҵ�LAG�µ�һ���˿ڵĲ�λ���·���FE1600
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucLagSlotId);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usMcId, stPortInfo.slot_id);
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastAdd() ret=%d", ret));
    
    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;
        
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

    //��ȡһ��CUD��Դ
    //spm_alloc_vpls_mc_cud(&iMcCud);
#endif

    NBB_TRC_DETAIL((NBB_FORMAT "  usMcId=%d iVpId=%d usPortId=%d ucSlot=%d", usMcId,
                                                                            pstVcTbl->vc_info_cb.vp_idx, 
                                                                            stPortInfo.port_id, 
                                                                            stPortInfo.slot_id));
    
    //��ȡ�鲥���ݣ���C3����
    stL2mcIntf.mcId = usMcId;
    stL2mcIntf.seq = iVpId;
    stL2mcIntf.dvpIdx = pstVcTbl->vc_info_cb.vp_idx;
	*piVpId = pstVcTbl->vc_info_cb.vp_idx;
	*piPosId = pstVcTbl->vc_info_cb.next_hop_id;
    
#ifdef SPU 

	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_0, pstVcTbl->vc_info_cb.vp_idx);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
		//����ǵ�1Ƭc3�����͵�109
		//if (stPortInfo.unit_id == UNIT_0)
		{
			ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_0, pstVcTbl->vc_info_cb.vp_idx);
		}
    	//else	//����ǵ�2Ƭc3�����͵�110
    	//{
    	//	ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_1, pstVcTbl->vc_info_cb.vp_idx);
    	//}
	}    

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiAradAddEngressMcPort() ret=%d", ret));
#endif

    if (ret != SUCCESS)
    {    
#ifdef SPU 
        //����˿�û����ӳɹ�������ɾ���ѷ����CUD
        //spm_free_vpls_mc_cud(iMcCud);
#endif
		//coverity[dead_error_begin]
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradAddEngressMcPort() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiAradAddEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVcTbl->vc_info_cb.vp_idx);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiAradAddEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVcTbl->vc_info_cb.vp_idx);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }

	//����˿�ΪLAG�����¼LAG��һ����Ա�˿����ڵĲ�λ
    if (stPortInfo.port_type == ATG_DCI_LAG)
    {
    	*pucSlot = stPortInfo.lag_slot_id;
    }
    else
    {
        *pucSlot = stPortInfo.slot_id;
    }

    //pstVplsPortInfo->port_type = stPortInfo.port_type;

#if 0	//�����鲥�����޸ģ����ڿ��Բ����鲥����intf
    ret = SUCCESS;

#ifdef SPU 
    //for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3SetL2MCOutIntf(stPortInfo.unit_id, &stL2mcIntf);

        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3SetL2MCOutIntf() ret=%d", ret));
    }
#endif 

    if (ret == SUCCESS)
    {
        *piPosId = stL2mcIntf.posId;
        *piMcCud = iMcCud;
        *pucSlot = stPortInfo.slot_id;
    }
    else
    {
#ifdef SPU 
        //����˿�û����ӳɹ�������ɾ���ѷ����CUD
        spm_free_vpls_mc_cud(iMcCud);
#endif    

        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetL2MCOutIntf() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiC3SetL2MCOutIntf() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiC3SetL2MCOutIntf() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_C3_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }
#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_vpls_mc_add_vp_uni
 ��������  : ΪVPLS�鲥�����UNI�˿�
 �������  : ATG_DCI_VPLS_MC_KEY *pstVplsMcKey      
             ATG_DCI_VPLS_MC_UNI_DATA* pstUniIndex  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_mc_add_vp_uni(NBB_USHORT usMcId, 
                                        ATG_DCI_VPLS_MC_UNI_DATA* pstUniIndex, 
                                        NBB_UINT *piPosId,
                                        NBB_UINT *piVpId,
                                        NBB_BYTE *pucSlot
                                        NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT ret = SUCCESS;
    NBB_INT iVpId = 0;
    NBB_BYTE ucC3Unit = 0;
    NBB_BYTE ucLagSlotId = 0;
    NBB_USHORT usLagPortId = 0;

    SPM_PORT_INFO_CB stPortInfo;
    
    L2MC_INTF_T stL2mcIntf;
    NBB_ULONG ulPortIndex;
    SPM_LOGICAL_PORT_CB *pstLogicalPort;
    
    NBB_TRC_ENTRY("spm_vpls_mc_add_vp_uni");

    if (pstUniIndex == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_mc_add_vp_uni(pstUniIndex==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpls_mc_add_vp_uni(pstUniIndex==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_add_vp_uni(pstUniIndex==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }
    
    OS_MEMSET(&stL2mcIntf, 0, sizeof(L2MC_INTF_T));
    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    ulPortIndex = pstUniIndex->port_index;

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);
    
    if (pstLogicalPort == NULL)       /* �����Ŀ������*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: PORT_INDEX=%ld ���߼��˿����ò����ڣ��޷���Ӵ��鲥�˿�!", pstUniIndex->port_index));

        OS_PRINTF("***ERROR***: PORT_INDEX=%ld ���߼��˿����ò����ڣ��޷���Ӵ��鲥�˿�!\n", pstUniIndex->port_index);

        OS_SPRINTF(ucMessage, "***ERROR***: PORT_INDEX=%ld ���߼��˿����ò����ڣ��޷���Ӵ��鲥�˿�!\n", pstUniIndex->port_index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        BASIC_CONFIG_NOT_EXIST, 
                                                                        "BASIC_CONFIG_NOT_EXIST", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //�����߼��˿������е�port_index��ȡ�˿ڵ�port_id��slot_id�����Arad��FE1600
    spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);

#ifdef SRC

	//�����LAG����Ҫ�ҵ�LAG�µ�һ���˿ڵĲ�λ���·���Arad
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucLagSlotId);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usMcId, stPortInfo.slot_id);
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastAdd() ret=%d", ret));

    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }
#endif

#ifdef SNU
	//�����LAG����Ҫ�ҵ�LAG�µ�һ���˿ڵĲ�λ���·���Arad
	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucLagSlotId);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
    	ret = ApiDfeMulticastAdd(UNIT_0, usMcId, stPortInfo.slot_id);
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiDfeMulticastAdd() ret=%d", ret));

    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiDfeMulticastAdd() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
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


    //��ȡһ��CUD��Դ
    //spm_alloc_vpls_mc_cud(&iMcCud);
#endif

    NBB_TRC_DETAIL((NBB_FORMAT "  usMcId=%d iVpId=%d usPortId=%d ucSlot=%d", usMcId,
                                                                            pstLogicalPort->logic_port_info_cb.vp_idx, 
                                                                            stPortInfo.port_id, 
                                                                            stPortInfo.slot_id));
    
    //��ȡ�鲥���ݣ���C3����
    stL2mcIntf.mcId = usMcId;
    stL2mcIntf.seq = iVpId;
    stL2mcIntf.dvpIdx = pstLogicalPort->logic_port_info_cb.vp_idx;
	*piVpId = pstLogicalPort->logic_port_info_cb.vp_idx;
	*piPosId= pstLogicalPort->logic_port_info_cb.vp_idx;

#ifdef SPU 

	if (stPortInfo.port_type == ATG_DCI_LAG)
	{
		ret = spm_get_lag_slot_port(stPortInfo.port_id-ATG_DCI_LAG_OFFSET, &ucLagSlotId, &usLagPortId NBB_CCXT);

		if (ret == SUCCESS)
		{
			//ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, usLagPortId, pstLogicalPort->logic_port_info_cb.vp_idx);
			ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_0, pstLogicalPort->logic_port_info_cb.vp_idx);
		}
		else
		{
	        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_get_lag_slot_port() ret=%d", ret));

	        OS_PRINTF("***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);

	        OS_SPRINTF(ucMessage, "***ERROR***: spm_get_lag_slot_port() ret=%d\n", ret);
	        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

	        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
	                                                             CALL_FUNC_ERROR, 
	                                                             "CALL_FUNC_ERROR", 
	                                                             ucMessage));
	        ret = ERROR;
	        goto EXIT_LABEL;
		}
	}
	else
	{
		//����ǵ�1Ƭc3�����͵�109
		//if (stPortInfo.unit_id == UNIT_0)
		{
			ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_0, pstLogicalPort->logic_port_info_cb.vp_idx);
		}
    	//else	//����ǵ�2Ƭc3�����͵�110
    	//{
    	//	ret = ApiAradAddEngressMcPort(UNIT_0, usMcId, SPM_MULTICAST_PORT_1, pstLogicalPort->logic_port_info_cb.vp_idx);
    	//}
	}

    NBB_TRC_DETAIL((NBB_FORMAT "  ApiAradAddEngressMcPort() ret=%d", ret));
#endif

    if (ret != SUCCESS)
    {
#ifdef SPU 
        //����˿�û����ӳɹ�������ɾ���ѷ����CUD
        //spm_free_vpls_mc_cud(iMcCud);
#endif
		//coverity[dead_error_begin]
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradAddEngressMcPort() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiAradAddEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstLogicalPort->logic_port_info_cb.vp_idx);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiAradAddEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstLogicalPort->logic_port_info_cb.vp_idx);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_ARAD_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }

	//����˿�ΪLAG�����¼LAG��һ����Ա�˿����ڵĲ�λ
    if (stPortInfo.port_type == ATG_DCI_LAG)
    {
    	*pucSlot = stPortInfo.lag_slot_id;
    }
    else
    {
        *pucSlot = stPortInfo.slot_id;
    }

#if 0	//�����鲥�����޸ģ����ڿ��Բ����鲥����intf
    ret = SUCCESS;
    
#ifdef SPU 
    //for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3SetL2MCOutIntf(stPortInfo.unit_id, &stL2mcIntf);

        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3SetL2MCOutIntf() ret=%d", ret));
    }
#endif 

    if (ret == SUCCESS)
    {
        *piPosId = stL2mcIntf.posId;
        *piMcCud = iMcCud;
        *pucSlot = stPortInfo.slot_id;
    }
    else
    {
#ifdef SPU 
        //����˿�û����ӳɹ�������ɾ���ѷ����CUD
        spm_free_vpls_mc_cud(iMcCud);
#endif 
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3SetL2MCOutIntf() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiC3SetL2MCOutIntf() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiC3SetL2MCOutIntf() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_C3_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;        
        goto EXIT_LABEL;
    }
#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_vpls_mc_del_vp_nni
 ��������  : ɾ��VPLS�鲥�˿�NNI
 �������  : NBB_USHORT usMcId                       
             SPM_VPLS_MC_CB *pstVplsMc               
             ATG_DCI_VPLS_MC_NNI_DATA* pstVplsMcNni  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��28��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_mc_del_vp_nni(NBB_USHORT usMcId, 
                               SPM_VPLS_MC_CB *pstVplsMc, 
                               ATG_DCI_VPLS_MC_NNI_DATA* pstVplsMcNni
                               NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_INT i = 0;
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_PORT_INFO_CB stPortInfo;
    
    ATG_DCI_VC_KEY stVcKey;
    SPM_VC_CB *pstVcTbl;
    
    NBB_TRC_ENTRY("spm_vpls_mc_del_vp_nni");

    if (pstVplsMc == NULL)
    {
    	ret = ERROR;
    	goto EXIT_LABEL;
    }
    
    if (pstVplsMcNni == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_mc_del_vp_nni(pstVplsMcNni==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpls_mc_del_vp_nni(pstVplsMcNni==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_del_vp_nni(pstVplsMcNni==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    stVcKey.vc_id   = pstVplsMcNni->vc_id;
    stVcKey.vc_type = pstVplsMcNni->vc_type;
    stVcKey.peer_ip = pstVplsMcNni->peer_ip;

    pstVcTbl = AVLL_FIND(SHARED.vc_tree, &stVcKey);
    
    if (pstVcTbl == NULL)       /* �����Ŀ������*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s ��VC�����ò����ڣ��޷�ɾ�����鲥�˿�!", stVcKey.vc_id,
                                                                                                                         stVcKey.vc_type,
                                                                                                                         spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT)));

        OS_PRINTF("***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s ��VC�����ò����ڣ��޷�ɾ�����鲥�˿�!\n", stVcKey.vc_id,
                                                                                                           stVcKey.vc_type,
                                                                                                           spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));

        OS_SPRINTF(ucMessage, "***ERROR***: vc_id=%ld, vc_type=%d, peer_ip=%s ��VC�����ò����ڣ��޷�ɾ�����鲥�˿�!\n", stVcKey.vc_id,
                                                                                                           stVcKey.vc_type,
                                                                                                           spm_set_ulong_to_ipv4(stVcKey.peer_ip NBB_CCXT));
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        BASIC_CONFIG_NOT_EXIST, 
                                                                        "BASIC_CONFIG_NOT_EXIST", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //����VC�������е�port_index��ȡ�˿ڵ�port_id��slot_id�����Arad��FE1600
    spm_get_portid_from_logical_port_index(pstVcTbl->vc_info_cb.port_index, &stPortInfo NBB_CCXT);


#if 0

    //��������λû���鲥�˿�(����UNI��NNI)�ˣ�SRC��Ҫɾ���˲�λ�������鲥��������˲�λ����
    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (ucSlot == pstVplsMc->uni_slot[i])
    }
    
    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
    
    }
    
    if (pstVplsMc->nni_num == 1)
    {
        ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucSlot);

        NBB_TRC_DETAIL((NBB_FORMAT "ApiDfeMulticastAdd() ret=%d", ret));
        
        if (ret != SUCCESS)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));
            ret = ERROR;
            
            goto EXIT_LABEL;
        }
    }

#endif

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

    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        //�ҵ�Ҫɾ����VP
        if ((pstVplsMc->nni_cfg_cb[i] != NULL)
         && (pstVplsMc->nni_cfg_cb[i]->vc_id == pstVplsMcNni->vc_id)
         && (pstVplsMc->nni_cfg_cb[i]->vc_type == pstVplsMcNni->vc_type)
         && (pstVplsMc->nni_cfg_cb[i]->peer_ip == pstVplsMcNni->peer_ip))
        {
            break;
        }
    }

    //δ�ҵ�Ҫɾ����VP
    if (i == ATG_DCI_VPLS_MC_NNI_NUM)
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }

    NBB_TRC_DETAIL((NBB_FORMAT "  usMcId=%d iVpId=%d usPortId=%d ucSlot=%d", usMcId, 
                                                                            pstVplsMc->nni_info_cb[i].vp_pos_id, 
                                                                            stPortInfo.port_id, 
                                                                            stPortInfo.slot_id));
    
#ifdef SPU 

	//�����lag��ɾ��109
    if (ATG_DCI_LAG == stPortInfo.port_type)
    {
	    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
	                                     usMcId, 
	                                     SPM_MULTICAST_PORT_0, 
	                                     pstVplsMc->nni_info_cb[i].vp_pos_id);	
    }
    else
    {
	    //���VP���ڵ�1Ƭc3��Arad���õ�109
		//if (stPortInfo.unit_id == UNIT_0)
		{
		    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
		                                     usMcId, 
		                                     SPM_MULTICAST_PORT_0, 
		                                     pstVplsMc->nni_info_cb[i].vp_pos_id);	
		}
		//else	//���VP���ڵ�2Ƭc3��Arad���õ�110
		//{
		//    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
		//                                     usMcId, 
		//                                     SPM_MULTICAST_PORT_1, 
		//                                     pstVplsMc->nni_info_cb[i].vp_pos_id);
		//}
    }

	if (ret != SUCCESS)
	{
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id));
        
        OS_PRINTF("***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
                                                         ret, 
                                                         "CALL_ARAD_FUNC_ERROR", 
                                                         ucMessage));
    }

#if 0	//�����鲥�����޸ģ����ڿ��Բ����鲥����intf
    ret = SUCCESS;
    
    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3DelL2MCOutIntf(ucC3Unit, 
                                  pstVplsMc->nni_info_cb[i].vp_pos_id);

        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelL2MCOutIntf() ret=%d", ret));
    }

    if (ret == SUCCESS)
    {
        //�ͷ�VP��ռ�ݵ�CUD��Դ
        spm_free_vpls_mc_cud(pstVplsMc->nni_info_cb[i].cud);
        OS_MEMSET(&pstVplsMc->nni_info_cb[i], 0, sizeof(SPM_VPLS_MC_PORT_INFO_CB));
    }
    else
    {        
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3DelL2MCOutIntf() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiC3DelL2MCOutIntf() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiC3DelL2MCOutIntf() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_C3_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

#endif


    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_vpls_mc_del_vp_uni
 ��������  : ɾ��VPLS�鲥�˿�UNI
 �������  : NBB_USHORT usMcId                      
             SPM_VPLS_MC_CB *pstVplsMc              
             ATG_DCI_VPLS_MC_UNI_DATA* pstUniIndex  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��28��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_mc_del_vp_uni(NBB_USHORT usMcId, 
                                       SPM_VPLS_MC_CB *pstVplsMc, 
                                       ATG_DCI_VPLS_MC_UNI_DATA* pstUniIndex
                                       NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_BYTE i = 0;
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_PORT_INFO_CB stPortInfo;

    NBB_ULONG ulPortIndex;
    SPM_LOGICAL_PORT_CB *pstLogicalPort;
    
    NBB_TRC_ENTRY("spm_vpls_mc_del_vp_uni");

    if (pstVplsMc == NULL)
    {
    	ret = ERROR;
    	goto EXIT_LABEL;
    }

    if (pstUniIndex == NULL)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:spm_vpls_mc_del_vp_uni(pstUniIndex==NULL)"));

        OS_PRINTF("***ERROR***:spm_vpls_mc_del_vp_uni(pstUniIndex==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_vpls_mc_del_vp_uni(pstUniIndex==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
        ret = ERROR;
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));

    ulPortIndex = pstUniIndex->port_index;

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);
    
    if (pstLogicalPort == NULL)       /* �����Ŀ������*/
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: PORT_INDEX=%ld ���߼��˿����ò����ڣ��޷�ɾ�����鲥�˿�!", pstUniIndex->port_index));

        OS_PRINTF("***ERROR***: PORT_INDEX=%ld ���߼��˿����ò����ڣ��޷�ɾ�����鲥�˿�!\n", pstUniIndex->port_index);

        OS_SPRINTF(ucMessage, "***ERROR***: PORT_INDEX=%ld ���߼��˿����ò����ڣ��޷�ɾ�����鲥�˿�!\n", pstUniIndex->port_index);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        BASIC_CONFIG_NOT_EXIST, 
                                                                        "BASIC_CONFIG_NOT_EXIST", 
                                                                        ucMessage));
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //�����߼��˿������е�port_index��ȡ�˿ڵ�port_id��slot_id�����Arad��FE1600
    spm_get_portid_from_logical_port_index(ulPortIndex, &stPortInfo NBB_CCXT);


#if 0
    ret = ApiDfeMulticastAdd(UNIT_0, usMcId, ucSlot);

    NBB_TRC_DETAIL((NBB_FORMAT "ApiDfeMulticastAdd() ret=%d", ret));
    
    if (ret != SUCCESS)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "***ERROR***: ApiDfeMulticastAdd() error! ret=%d", ret));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

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

    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        //�ҵ�Ҫɾ����VP
        if ((pstVplsMc->uni_cfg_cb[i]!= NULL)
         && (pstVplsMc->uni_cfg_cb[i]->port_index == ulPortIndex))
        {
            break;
        }
    }

    //δ�ҵ�Ҫɾ����VP
    if (i == ATG_DCI_VPLS_MC_UNI_NUM)
    {
        ret = SUCCESS;
        goto EXIT_LABEL;
    }

    NBB_TRC_DETAIL((NBB_FORMAT "  usMcId=%d iVpId=%d usPortId=%d ucSlot=%d", usMcId, 
                                                                            pstVplsMc->uni_info_cb[i].vp_pos_id, 
                                                                            stPortInfo.port_id, 
                                                                            stPortInfo.slot_id));
    
#ifdef SPU 
    
	//�����lag��ɾ��109
    if (ATG_DCI_LAG == stPortInfo.port_type)
    {
	    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
	                                     usMcId, 
	                                     SPM_MULTICAST_PORT_0, 
	                                     pstVplsMc->uni_info_cb[i].vp_pos_id);
    }
    else
    {
	    //���VP���ڵ�1Ƭc3��Arad���õ�109
		//if (stPortInfo.unit_id == UNIT_0)
		{
		    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
		                                     usMcId, 
		                                     SPM_MULTICAST_PORT_0, 
		                                     pstVplsMc->uni_info_cb[i].vp_pos_id);
		}
		//else	//���VP���ڵ�2Ƭc3��Arad���õ�110
		//{
		//    ret = ApiAradDeleteEngressMcPort(UNIT_0, 
		//                                     usMcId, 
		//                                     SPM_MULTICAST_PORT_1, 
		//                                     pstVplsMc->uni_info_cb[i].vp_pos_id);
		//}
    }

	if (ret != SUCCESS)
	{
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id));
        
        OS_PRINTF("***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiAradDeleteEngressMcPort() ret=%d MC_ID=%ld VpId=%d\n", ret, usMcId, pstVplsMc->nni_info_cb[i].vp_pos_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_PD, 0, "ld d s s", usMcId, 
                                                         ret, 
                                                         "CALL_ARAD_FUNC_ERROR", 
                                                         ucMessage));
    }

#if 0  
    ret = SUCCESS;
    
    for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    {
        ret += ApiC3DelL2MCOutIntf(ucC3Unit, pstVplsMc->uni_info_cb[i].vp_pos_id);

        NBB_TRC_DETAIL((NBB_FORMAT "  ApiC3DelL2MCOutIntf() ret=%d", ret));
    }


    if (ret == SUCCESS)
    {
        //�ͷ�VP��ռ�ݵ�CUD��Դ
        spm_free_vpls_mc_cud(pstVplsMc->uni_info_cb[i].cud);
        OS_MEMSET(&pstVplsMc->uni_info_cb[i], 0, sizeof(SPM_VPLS_MC_PORT_INFO_CB));
    }
    else
    {        
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3DelL2MCOutIntf() error! ret=%d", ret));

        OS_PRINTF("***ERROR***: ApiC3DelL2MCOutIntf() ret=%d\n", ret);

        OS_SPRINTF(ucMessage, "***ERROR***: ApiC3DelL2MCOutIntf() ret=%d\n", ret);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", usMcId,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        ret, 
                                                                        "CALL_C3_FUNC_ERROR", 
                                                                        ucMessage));
        ret = ERROR;
        
        goto EXIT_LABEL;
    }
#endif

#endif

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_init_vpls_mc_tree
 ��������  : ��ʼ��VPLS�鲥��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��12��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_vpls_mc_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_vpls_mc_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_VPLS_MC_CB, vpls_mc_key);/*lint !e413 */

    AVLL_INIT_TREE(SHARED.vpls_mc_tree, spm_vpls_mc_key_compare,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_VPLS_MC_CB, spm_vpls_mc_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 �� �� ��  : spm_free_vpls_mc_cb
 ��������  : �ͷ�VPLS�鲥�ṹ���ڴ�
 �������  : SPM_VPLS_MC_CB *pstVplsMc  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��12��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_free_vpls_mc_cb(SPM_VPLS_MC_CB *pstVplsMc NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_vpls_mc_cb");
    
    /***************************************************************************/
    /* �����ƿ����ȷ�ԡ�                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstVplsMc, sizeof(SPM_VPLS_MC_CB), MEM_SPM_VPLS_MC_CB); 

    if (pstVplsMc == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* ɾ��������Ϣ���ƿ顣                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pstVplsMc));  
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws_id %d", pstVplsMc->vpws_id_key));

    /***************************************************************************/
    /* �ͷŻ����������ݿ顣                                                */
    /***************************************************************************/
    if (pstVplsMc->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstVplsMc->basic_cfg_cb, MEM_SPM_VPLS_MC_BASIC_CB);  
        pstVplsMc->basic_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* �ͷ�NNI��UNI���ݿ顣                                                */
    /***************************************************************************/
    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (pstVplsMc->nni_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstVplsMc->nni_cfg_cb[i], MEM_SPM_VPLS_MC_NNI_CB);  
            pstVplsMc->nni_cfg_cb[i] = NULL;
        }
    }

    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        if (pstVplsMc->uni_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstVplsMc->uni_cfg_cb[i], MEM_SPM_VPLS_MC_UNI_CB);  
            pstVplsMc->uni_cfg_cb[i] = NULL;
        }
    } 

    /***************************************************************************/
    /* ɾ�����ƿ�ľ����                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstVplsMc->spm_vpls_mc_handle, HDL_SPM_VPLS_MC_CB);

    /***************************************************************************/
    /* �����ͷŵ�����Ϣ���ƿ顣                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstVplsMc, MEM_SPM_VPLS_MC_CB);  
    pstVplsMc = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 �� �� ��  : spm_vpls_mc_get_nni_num
 ��������  : VPLS�鲥��NNI�ĸ���
 �������  : ATG_DCI_VPLS_MC_KEY stVplsMcKey  
             NBB_INT *piVpNum                 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��5��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_mc_get_nni_num(ATG_DCI_VPLS_MC_KEY stVplsMcKey,
                                                                  NBB_INT *piVpNum
                                                                  NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    SPM_VPLS_MC_CB *pstVplsMc = NULL;

    NBB_TRC_ENTRY("spm_vpls_mc_get_nni_num");

    pstVplsMc = AVLL_FIND(v_spm_shared->vpls_mc_tree, &stVplsMcKey);

    if (pstVplsMc != NULL)
    {
        *piVpNum = pstVplsMc->nni_num;
        NBB_TRC_DETAIL((NBB_FORMAT "  VPLS_ID=%d<NNI=%d>", stVplsMcKey.vpls_id, pstVplsMc->nni_num));
    }
    else
    {
        *piVpNum = 0;
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_get_nni_num() VPLS_ID=%d �� VPLS ������", stVplsMcKey.vpls_id));

        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_get_nni_num() VPLS_ID=%d �� VPLS ������\n", stVplsMcKey.vpls_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_vpls_mc_get_uni_num
 ��������  : VPLS�鲥��UNI�ĸ���
 �������  : ATG_DCI_VPLS_MC_KEY stVplsMcKey  
             NBB_INT *piVpNum                 
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��5��16��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_vpls_mc_get_uni_num(ATG_DCI_VPLS_MC_KEY stVplsMcKey,
                                                                  NBB_INT *piVpNum
                                                                  NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_INT ret = SUCCESS;
    SPM_VPLS_MC_CB *pstVplsMc = NULL;

    NBB_TRC_ENTRY("spm_vpls_mc_get_uni_num");

    pstVplsMc = AVLL_FIND(v_spm_shared->vpls_mc_tree, &stVplsMcKey);

    if (pstVplsMc != NULL)
    {
        *piVpNum = pstVplsMc->uni_num;
        NBB_TRC_DETAIL((NBB_FORMAT "  VPLS_ID=%d<UNI=%d>", stVplsMcKey.vpls_id, pstVplsMc->uni_num));
    }
    else
    {
        *piVpNum = 0;
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: spm_vpls_mc_get_uni_num() VPLS_ID=%d �� VPLS ������", stVplsMcKey.vpls_id));


        OS_SPRINTF(ucMessage, "***ERROR***: spm_vpls_mc_get_uni_num() VPLS_ID=%d �� VPLS ������\n", stVplsMcKey.vpls_id);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | VPLS_MC_PD, 0, "ld x x x x x x d s s", stVplsMcKey.vpls_id,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        0,
                                                                        FUNC_IN_PARAM_IS_NULL, 
                                                                        "FUNC_IN_PARAM_IS_NULL", 
                                                                        ucMessage));
    }
    
    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_alloc_vpLs_mc_cb
 ��������  : ����VPLS�鲥�ڴ�ռ�
 �������  : ��
 �������  : ��
 �� �� ֵ  : SPM_VPLS_MC_CB
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��12��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
SPM_VPLS_MC_CB * spm_alloc_vpls_mc_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_VPLS_MC_CB *pstVplsMc = NULL;

    NBB_TRC_ENTRY("spm_alloc_vpls_mc_cb");

    /* ����һ���µ�VPLS�鲥������Ŀ��*/
    pstVplsMc = (SPM_VPLS_MC_CB *)NBB_MM_ALLOC(sizeof(SPM_VPLS_MC_CB), NBB_NORETRY_ACT, MEM_SPM_VPLS_MC_CB);
    if (pstVplsMc == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:����VPLS�鲥��Ŀ�ռ�ΪNULL!"));
        OS_PRINTF("***ERROR***:����VPLS�鲥��Ŀ�ռ�ΪNULL!\n");
        
        goto EXIT_LABEL;
    }

    /* ��ʼ��VPLS�鲥������Ŀ */
    OS_MEMSET(pstVplsMc, 0, sizeof(SPM_VPLS_MC_CB));
    pstVplsMc->basic_cfg_cb = NULL;

    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        pstVplsMc->nni_cfg_cb[i] = NULL;        
    }

    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        pstVplsMc->uni_cfg_cb[i] = NULL;
    }

    /* �������ڸ�VPLS�鲥������Ŀ�ľ������Ϊ�첽��Ϣ�������������*/
    pstVplsMc->spm_vpls_mc_handle = NBB_CREATE_HANDLE(pstVplsMc, HDL_SPM_VPLS_MC_CB);

    /* �ɹ�����һ���µĽӿ�����������Ŀ��*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_VPLS_MC_CB allocated at %p with handle %#lx",
                   pstVplsMc, pstVplsMc->spm_vpls_mc_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstVplsMc->spm_vpls_mc_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstVplsMc);
}

/*****************************************************************************
 �� �� ��  : spm_alloc_vpls_mc_cud
 ��������  :  ��64K��CUD��Դ���з���һ��CUD��
 �������  : NBB_INT *iMcCud  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��14��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_alloc_vpls_mc_cud(NBB_INT *iMcCud NBB_CCXT_T NBB_CXT)
{
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	
    NBB_UINT i = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_alloc_vpls_mc_cud");

    for (i=0; i<ATG_DCI_CUD_NUM; i++)
    {
        if (SHARED.mc_cud[i] == ATG_DCI_USED)
        {
            continue;
        }

        *iMcCud = i+1;
        SHARED.mc_cud[i] = ATG_DCI_USED;
        break;
    }

    //�������ˣ����ش���
    if (i == ATG_DCI_CUD_NUM)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:CUD�Ѿ���������"));

        OS_PRINTF("***ERROR***:CUD�Ѿ���������!\n");

        OS_SPRINTF(ucMessage, "***ERROR***:CUD�Ѿ���������!\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
        
        ret = ERROR;
    }    

    NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_free_vpls_mc_cud
 ��������  : �ͷŷ����cud��Դ
 �������  : NBB_INT iMcCud  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��23��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_free_vpls_mc_cud(NBB_INT iMcCud NBB_CCXT_T NBB_CXT)
{
    NBB_UINT i = 0;
    NBB_INT ret = SUCCESS;

    NBB_TRC_ENTRY("spm_free_vpls_mc_cud");

    if ((iMcCud < 1) || (iMcCud > ATG_DCI_CUD_NUM))
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:�����CUDֵ������Χ��"));
        OS_PRINTF("***ERROR***:�����CUDֵ������Χ!\n");
        
        ret = ERROR;
        goto EXIT_LABEL;
    }

    //OS_PRINTF("iMcCud=%d mc_cud=%d\n", iMcCud, SHARED.mc_cud[iMcCud-1]);
    
    SHARED.mc_cud[iMcCud-1] = ATG_DCI_UNUSED;
    
    //OS_PRINTF("iMcCud=%d mc_cud=%d\n", iMcCud, SHARED.mc_cud[iMcCud-1]);

    EXIT_LABEL : NBB_TRC_EXIT();

    return ret;
}

/*****************************************************************************
 �� �� ��  : spm_init_vpls_mc_cud
 ��������  : ��ʼ��cud,ϵͳ����ʱ����һ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��15��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_init_vpls_mc_cud(NBB_CXT_T NBB_CXT)
{
    NBB_UINT i = 0;

    NBB_TRC_ENTRY("spm_init_vpls_mc_cud");

    for (i=0; i<ATG_DCI_CUD_NUM; i++)
    {
        SHARED.mc_cud[i] = ATG_DCI_UNUSED;
    }
    
    NBB_TRC_EXIT();
}

/*****************************************************************************
 �� �� ��  : spm_set_vpls_mc_cud_state
 ��������  : ����CUD״̬,0=δʹ��,1=ʹ��
 �������  : NBB_UINT iPos   
             NBB_INT iState  0=δʹ��,1=ʹ��
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��1��15��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_set_vpls_mc_cud_state(NBB_UINT iMcCud, NBB_INT iState NBB_CCXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_set_vpls_mc_cud_state");

    SHARED.mc_cud[iMcCud-1] = iState;

    NBB_TRC_EXIT();
}

/*****************************************************************************
 �� �� ��  : spm_get_vpls_mc_slot_num
 ��������  : ��ȡ��ucSlot��λ�˿ڵĸ���
 �������  : SPM_VPLS_MC_CB *pstVplsMc  
             NBB_BYTE ucSlot            
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : 2013��2��4��
    ��    ��   : xiaoxiang
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_get_vpls_mc_slot_num(SPM_VPLS_MC_CB *pstVplsMc, NBB_BYTE ucSlot NBB_CCXT_T NBB_CXT)
{
    NBB_INT i = 0;
    NBB_INT iNum = 0;
    
    NBB_TRC_ENTRY("spm_get_vpls_mc_slot_num");

    if (pstVplsMc == NULL)
    {
    	goto EXIT_LABEL;
    }

    //NNI���ڴ˲�λ�Ķ˿ڸ���
    for (i=0; i<ATG_DCI_VPLS_MC_NNI_NUM; i++)
    {
        if (ucSlot == pstVplsMc->nni_info_cb[i].slot_id)
            iNum++;
    }

    //UNI���ڴ˲�λ�Ķ˿ڸ���
    for (i=0; i<ATG_DCI_VPLS_MC_UNI_NUM; i++)
    {
        if (ucSlot == pstVplsMc->uni_info_cb[i].slot_id)
            iNum++;
    }

    EXIT_LABEL: NBB_TRC_EXIT();

    return iNum;
}

/*****************************************************************************
   �� �� ��  : spm_free_all_vpls_mc_cb
   ��������  : �ͷ�vpls�ಥ�������е��ڴ�ռ�
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

NBB_VOID spm_free_all_vpls_mc_cb(NBB_CXT_T NBB_CXT)
{
	SPM_VPLS_MC_CB *pstVplsMc = NULL;
	
    for (pstVplsMc = (SPM_VPLS_MC_CB*) AVLL_FIRST(SHARED.vpls_mc_tree);
         pstVplsMc != NULL;
         pstVplsMc = (SPM_VPLS_MC_CB*) AVLL_FIRST(SHARED.vpls_mc_tree))
    {
    	
		AVLL_DELETE(SHARED.vpls_mc_tree, pstVplsMc->spm_vpls_mc_node);     	
    	spm_free_vpls_mc_cb(pstVplsMc NBB_CCXT);
    }
}


#endif

