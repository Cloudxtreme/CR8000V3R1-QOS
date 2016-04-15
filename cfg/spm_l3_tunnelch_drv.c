/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_l3_tunnelch.c
  �� �� ��   : v0
  ��    ��   : wjhe
  ��������   : 2016��2��29��
  ����޸�   :
  ��������   : ���ѡ�����ô����������
  �����б�   :
  �޸���ʷ   :

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*************************************************************************

* ������   �� spm_l3_txlsp_create_drv_txlsp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ����txlsp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_tunnelch_mod_drv_txpw(PWTXNODE *pwtxnodes,TUNNELCH *tunnelch NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    L3_TX_PW_T          txpw;
    L3_TX_PW_T          temp_txpw;

    if (pwtxnodes == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e tunnelch_mod_drv_txpw pwtxnode null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    if (tunnelch == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e tunnelch_mod_drv_txpw tunnelch null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&txpw , 0, sizeof(L3_TX_PW_T));
    OS_MEMSET(&temp_txpw , 0, sizeof(L3_TX_PW_T));
    txpw.mVrf = tunnelch->key.vrfid;
    txpw.mPeerIp = tunnelch->key.peerip;
    txpw.mLabel = pwtxnodes->key.label;
    
    txpw.posId = pwtxnodes->posid;
    txpw.tunnelIdx = tunnelch->tunindex; 
    txpw.pushPwNum = 1;

    rv = fhdrv_psn_l3_get_txpw(BCMC31, &txpw, &temp_txpw);
    if (rv == 0)
    {
        txpw.eNniMode = temp_txpw.eNniMode;
    }
    else
    {
        rv = ADD_TUNNELSEL_DRV_GET_TXPW_ERR;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, 
                "Line=%d e tunnelch fhdrv_psn_l3_get_txpw posid.eNniMode.rv =%d.%d.%d\n", 
                __LINE__, txpw.posId, txpw.eNniMode,rv);
    }
    
    //txpw.flags |= L3_TX_PW_COUNTER_ON;
    rv = ApiC3ModL3TxPw( BCMC31, &txpw);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3ModL3TxPw( BCMC32, &txpw);
        rv = (rv|rv2);
    }
    if( rv != 0) 
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add tunnsel: drv mod txpw,rv=%d.\n",
                                                       __LINE__,rv);
        goto  EXIT_LABEL;
    }
    rv = 0;

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}


