/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_crlsp_drv.c
  �� �� ��   : v0
  ��    ��   : wjhe
  ��������   : 2016��2��29��
  ����޸�   :
  ��������   : CRLSPTX���ô����������
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
NBB_INT spm_l3_txlsp_create_drv_txlsp(CRTXLSP *txlsp NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    TX_LSP_T        lsptun;
    if (txlsp == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e create_drv_txlsp txlsp null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    OS_MEMSET(&lsptun , 0, sizeof(TX_LSP_T));
    #ifdef SPU
    if( SHARED.local_slot_id != txlsp->slot)
    {
        lsptun.flags = TX_LSP_MY_SLOT;
    }
    lsptun.tTxPort.slot = txlsp->slot;
    lsptun.tTxPort.port = txlsp->port;
    lsptun.tTxPort.vid = txlsp->svlan ;
    lsptun.mIngress = txlsp->key.ingress;
    lsptun.mEgress =  txlsp->key.egress;
    lsptun.mTunnelId = txlsp->key.tunnelid;
    lsptun.mLspId = txlsp->key.lspid;

    lsptun.flags = 0;
    lsptun.tunNum = 1;
    if( txlsp->dclsp.label2 != 0xffffffff)  
    {
        lsptun.tunNum = 2;
    }
    if( txlsp->dclsp.label2 == 3)  
    {
        lsptun.tunNum = 1;
    }
    if( txlsp->dclsp.label1 == 3)  
    {
        lsptun.tunNum = 0;
    }
    lsptun.tSwapTunLabel.label_un.tlabel.label = txlsp->dclsp.label1;
    lsptun.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lsptun.tSwapTunLabel.label_un.tlabel.exp = 0;

    lsptun.tPushTunLabel.label_un.tlabel.label = txlsp->dclsp.label2;
    lsptun.tPushTunLabel.label_un.tlabel.ttl = 255;
    lsptun.tPushTunLabel.label_un.tlabel.exp = 0;

    lsptun.nhIp = txlsp->dclsp.nextip;
    if( txlsp->dclsp.ftnidx == 0)
    {
        rv = ApiC3AddTxLsp( BCMC31 ,&lsptun);  /* ��������*/
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3AddTxLsp( BCMC32 ,&lsptun);
            rv = (rv|rv2);
        }
    }
    else
    {
        lsptun.posId =  txlsp->dclsp.index;
        lsptun.lspIdx =  txlsp->dclsp.ftnidx;
        rv = ApiC3ModTxLsp( BCMC31 ,&lsptun);
        if ( SHARED.c3_num == 2 )
        {
            lsptun.posId =  txlsp->dclsp.index;
            lsptun.lspIdx =  txlsp->dclsp.ftnidx;
            rv2 = ApiC3ModTxLsp( BCMC32 ,&lsptun);
            rv = (rv|rv2);
        }
    }
    if(rv == 0)
    {
        txlsp->dclsp.index = lsptun.posId;
        txlsp->dclsp.ftnidx = lsptun.lspIdx;
    }
    else
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d Add LSP TX Error:lsp drv ,rv=%d.\n",
                                                                             __LINE__,rv);
    }
    #endif
    
#ifdef SRC
    lsptun.posId = 0xffff;
    lsptun.lspIdx = 0xffffffff;
    rv = 0;
#endif
#ifdef SNU
    lsptun.posId = 0xffff;
    lsptun.lspIdx = 0xffffffff;
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_txlsp_del_drv_txlsp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ɾ��txlsp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_txlsp_del_drv_txlsp(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    if (index == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e create_drv_txlsp index = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }  
#ifdef SPU  
   rv = ApiC3DelTxLsp( BCMC31, index);
   if ( SHARED.c3_num == 2 )
   {
       rv2 = ApiC3DelTxLsp( BCMC32, index);
       rv = (rv|rv2);
   }
   if( rv != 0)  
   {
       BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e del lsp drv err ,rv=%d.\n",
                                                                       __LINE__,rv);
       goto  EXIT_LABEL;
   }
   else
   {
       
   }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}




