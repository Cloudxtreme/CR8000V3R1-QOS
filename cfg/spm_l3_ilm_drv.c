/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_ilm_drv.c
  �� �� ��   : v0
  ��    ��   : wjhe
  ��������   : 2016��2��29��
  ����޸�   :
  ��������   : ilm���ô����������
  �����б�   :
  �޸���ʷ   :

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
/*************************************************************************

* ������   �� spm_l3_ilm_del_drv_rxpw
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ɾ��rxpw����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_del_drv_rxpw(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_del_drv_rxpw ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    if( ilms->posid != 0 )
    {
        rv = ApiC3DelL3RxPw( BCMC31, ilms->posid);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3DelL3RxPw( BCMC32, ilms->posid);
            rv = (rv|rv2);
        }
        if( rv != 0) 
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Del ilm:del drv l3rxpw  ,rv=%d.\n",
                                                           __LINE__,rv);
            goto  EXIT_LABEL;
        }
        else
        {
            ilms->posid = 0;
        }
    }
    else
    {
        rv = 0;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_del_drv_frrgroup
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ɾ��frrgroup����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_del_drv_frrgroup(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_del_drv_frrgroup ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    if( ilms->frrid != 0 )
    {
        rv = ApiC3DeleteFrrGroup( BCMC31, ilms->frrid );
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3DeleteFrrGroup( BCMC32, ilms->frrid);
            rv = (rv|rv2);
        }
        if( rv != 0)   
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del ilm:del drv frr  ,rv=%d.\n",
                                                           __LINE__,rv);
            goto  EXIT_LABEL;
        }
        else
        {
            ilms->frrid = 0;
        }
    }
    else
    {
        rv = 0;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_del_drv_ecmpid
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ɾ��ecmp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_del_drv_ecmpid(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_del_drv_ecmp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    if( ilms->ecmpid != 0 )
    {
        rv = ApiC3FreeEcmpGroupId( BCMC31,  ilms->ecmpid );
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3FreeEcmpGroupId( BCMC32, ilms->ecmpid );
            rv = (rv|rv2);
        }
        if( rv != 0)   
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d ILM Error:del drv ecmp  ,rv=%d.\n",
                                                           __LINE__,rv);
        }
        else
        {
            ilms->ecmpid = 0;
        }
    }
    else
    {
        rv = 0;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_del_drv_rxlsp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ɾ��rxlsp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_del_drv_rxlsp(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_del_drv_rxlsp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    if( ilms->posid != 0 )
    {
        rv = ApiC3DelRxLsp( BCMC31, ilms->posid);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3DelRxLsp( BCMC32, ilms->posid);
            rv = (rv|rv2);
        }
        if( rv != 0) 
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:del drv lsp  ,rv=%d.\n",
                                                           __LINE__,rv);
            goto  EXIT_LABEL;
        }
        else
        {
            ilms->posid = 0;
        }
    }
    else
    {
        rv = 0;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_create_drv_rxlsp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ����rxlsp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_create_drv_rxlsp_pop(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    RX_LSP_T lspt;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_rxlsp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.mLabel =  ilms->key.inlabel;                       
    lspt.eRxLspAction = MPLS_ACTION_IPOP;
    
    if( ilms->posid == 0 )
    {
        rv = ApiC3AddRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3AddRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
    }
    else
    {
        lspt.posId = ilms->posid;
        rv = ApiC3ModRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
    }
    if( rv != 0) 
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm: drv lsp ipop  ,rv=%d.\n",
                                                                   __LINE__,rv);       
        goto  EXIT_LABEL;
    }
    else
    {
        ilms->posid = lspt.posId;
    }

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_create_drv_rxpw
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ����rxpw����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_create_drv_rxpw(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 1;
    NBB_INT         rv2 = 1;
    L3_RX_PW_T      rxpw;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_rxpw ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&rxpw , 0, sizeof(L3_RX_PW_T));
    rxpw.mLabel = ilms->key.inlabel;
    rxpw.vrf = ilms->base.vrfid;
    rxpw.flags = L3_RX_PW_COUNTER_ON;
    rxpw.eLableAction = MPLS_ACTION_L3VPN_VRF_ROUTE;
    if( ilms->posid == 0 )
    {
        rv = ApiC3AddL3RxPw( BCMC31, &rxpw);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3AddL3RxPw( BCMC32, &rxpw);
            rv = (rv|rv2);
        }
    }
    else
    {
        rxpw.posId = ilms->posid;
        rv = ApiC3ModL3RxPw( BCMC31, &rxpw);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModL3RxPw( BCMC32, &rxpw);
            rv = (rv|rv2);
        }
    }
    if( rv != 0) 
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm: drv rxpw  ,rv=%d.\n",
                                                                   __LINE__,rv);
        goto  EXIT_LABEL;
    }
    else
    {
        ilms->posid = rxpw.posId;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_create_drv_frrgroup
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ����frrgroup����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_create_drv_frrgroup(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 1;
    NBB_INT         rv2 = 1;
    NBB_USHORT      frrid = 0;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_frrgroup ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

     if( ilms->frrid == 0 )
    {
        rv = ApiC3CreateFrrGroup( BCMC31, &frrid, 0, 0);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3CreateFrrGroup( BCMC32, &frrid, 0, 0);
            rv = (rv|rv2);
        }
        if( rv != 0)   
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm: drv frr ,rv=%d.\n",
                                                               __LINE__,rv);
            goto  EXIT_LABEL;
        }
        else
        {
             ilms->frrid = frrid;
        }
    }
    else
    {
        rv = 0;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_create_drv_ecmp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ����ecmp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_create_drv_ecmp(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 1;
    NBB_INT         rv2 = 1;
    NBB_USHORT      ecmpid = 0;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_ecmp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    if( ilms->ecmpid == 0 )
    {
        rv = ApiC3ReqEcmpGroupId( BCMC31, &ecmpid);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ReqEcmpGroupId( BCMC32, &ecmpid);
            rv = (rv|rv2);
        }
        if( rv != 0)   
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm: drv ecmp ,rv=%d.\n",
                                                               __LINE__,rv);
            goto  EXIT_LABEL;
        }
        else
        {
             ilms->ecmpid = ecmpid;
        }
    }
    else
    {
        rv = 0;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_mod_drv_rxlspecmp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� �޸�rxlsp ecmp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_mod_drv_rxlspecmp(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 1;
    NBB_INT         rv2 = 1;
    NBB_INT         i = 0;
    RX_LSP_T        lspt;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_ecmp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.mLabel =  ilms->key.inlabel;                        
    lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
    lspt.eNhiProtType = C3_PROT_ECMP;
    lspt.tEcmpNh.ecmpNum = ilms->nhopnum;
    
    //lspt.tEcmpNh.alg = ECMP_PORT_ROUND_ROBIN;
    if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 0 )
    {
        lspt.tEcmpNh.alg = ECMP_HASH_BOTTOM_LABEL;
    }
    else if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 1 )
    {
        lspt.tEcmpNh.alg = ECMP_PORT_ROUND_ROBIN;
    }
    for ( i = 0; i < ilms->nhopnum; i++)
    {
        lspt.tEcmpNh.nhGroup[i].tTxPort.slot =ilms->portinfo[i].slot;
        lspt.tEcmpNh.nhGroup[i].tTxPort.port = ilms->portinfo[i].port_id;
        lspt.tEcmpNh.nhGroup[i].tTxPort.vid = ilms->portinfo[i].svlan ;
        
        lspt.tEcmpNh.nhGroup[i].tunNum = 1;
        lspt.tEcmpNh.nhGroup[i].cntEnable = 1;
        lspt.tEcmpNh.nhGroup[i].nhIp = ilms->nexthop[i].nextip;

        if(( ilms->base.labelaction == 1 ) && ( ilms->nexthop[i].outlabel == 3 ))  
        {
            lspt.tEcmpNh.nhGroup[i].tunNum = 0;
        }
        
        lspt.tEcmpNh.nhGroup[i].tSwapTunLabel.label_un.tlabel.label = ilms->nexthop[i].outlabel;
        lspt.tEcmpNh.nhGroup[i].tSwapTunLabel.label_un.tlabel.ttl = 255;
        lspt.tEcmpNh.nhGroup[i].tSwapTunLabel.label_un.tlabel.exp = 7;

        //��ѹ���ǩ
        lspt.tEcmpNh.nhGroup[i].tPushTunLabel.label_un.tlabel.label = 0;
        lspt.tEcmpNh.nhGroup[i].tPushTunLabel.label_un.tlabel.ttl = 0;
        lspt.tEcmpNh.nhGroup[i].tPushTunLabel.label_un.tlabel.exp = 0;
    }
    if( ilms->posid == 0 )
    {
        lspt.tEcmpNh.ecmpIdx = ilms->ecmpid;
        rv = ApiC3AddRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            lspt.tEcmpNh.ecmpIdx = ilms->ecmpid;
            rv2 = ApiC3AddRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv == 0)
        {
            ilms->posid = lspt.posId;
        }
        else
        {
            rv = ADD_ILM_NEXTHOP_DRV_LSP_ECMP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop dev lsp ecmp,rv=%d.\n",
                                                               __LINE__,rv);
            goto  EXIT_LABEL;
        }
    }
    else
    {
        lspt.posId = ilms->posid ;
        lspt.tEcmpNh.ecmpIdx = ilms->ecmpid;
        rv = ApiC3ModRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            lspt.tEcmpNh.ecmpIdx = ilms->ecmpid;
            rv2 = ApiC3ModRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv == 0)
        {
            ilms->posid = lspt.posId;
            
            //ilms->ecmpid = lspt.tEcmpNh.ecmpIdx;
        }
        else
        {
            rv = ADD_ILM_NEXTHOP_DRV_MODLSP_ECMP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop dev modlsp ecmp,rv=%d.\n",
                                                               __LINE__,rv);
            goto  EXIT_LABEL;
        }
    }

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_add_drv_rxlspecmp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ����rxlsp ecmp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_add_drv_rxlspecmp(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 1;
    NBB_INT         rv2 = 1;
    NBB_INT         i = 0;
    RX_LSP_T        lspt;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_ecmp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.mLabel =  ilms->key.inlabel;                        
    lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
    lspt.eNhiProtType = C3_PROT_ECMP;
    lspt.tEcmpNh.ecmpNum = ilms->nhopnum;
    
    //lspt.tEcmpNh.alg = ECMP_PORT_ROUND_ROBIN;
    if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 0 )
    {
        lspt.tEcmpNh.alg = ECMP_HASH_BOTTOM_LABEL;
    }
    else if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 1 )
    {
        lspt.tEcmpNh.alg = ECMP_PORT_ROUND_ROBIN;
    }
    for ( i = 0; i < ilms->nhopnum; i++)
    {
        lspt.tEcmpNh.nhGroup[i].tTxPort.slot =ilms->portinfo[i].slot;
        lspt.tEcmpNh.nhGroup[i].tTxPort.port = ilms->portinfo[i].port_id;
        lspt.tEcmpNh.nhGroup[i].tTxPort.vid = ilms->portinfo[i].svlan ;
        
        lspt.tEcmpNh.nhGroup[i].tunNum = 1;
        lspt.tEcmpNh.nhGroup[i].cntEnable = 1;
        lspt.tEcmpNh.nhGroup[i].nhIp = ilms->nexthop[i].nextip;

        if(( ilms->base.labelaction == 1 ) && ( ilms->nexthop[i].outlabel == 3 ))  
        {
            lspt.tEcmpNh.nhGroup[i].tunNum = 0;
        }
        
        lspt.tEcmpNh.nhGroup[i].tSwapTunLabel.label_un.tlabel.label = ilms->nexthop[i].outlabel;
        lspt.tEcmpNh.nhGroup[i].tSwapTunLabel.label_un.tlabel.ttl = 255;
        lspt.tEcmpNh.nhGroup[i].tSwapTunLabel.label_un.tlabel.exp = 7;

        //��ѹ���ǩ
        lspt.tEcmpNh.nhGroup[i].tPushTunLabel.label_un.tlabel.label = 0;
        lspt.tEcmpNh.nhGroup[i].tPushTunLabel.label_un.tlabel.ttl = 0;
        lspt.tEcmpNh.nhGroup[i].tPushTunLabel.label_un.tlabel.exp = 0;
    }
   
    lspt.tEcmpNh.ecmpIdx = ilms->ecmpid;
    lspt.posId = ilms->posid ;
    rv = ApiC3AddRxLspEcmp( BCMC31,  &lspt);
    if ( SHARED.c3_num == 2 )
    {
        lspt.tEcmpNh.ecmpIdx = ilms->ecmpid;
        rv2 = ApiC3AddRxLspEcmp( BCMC32,  &lspt);
        rv = (rv|rv2);
    }
    if( rv == 0)
    {
        ilms->posid = lspt.posId;
    }
    else
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop dev lsp ecmp,rv=%d.\n",
                                                           __LINE__,rv);
        goto  EXIT_LABEL;
    }
    
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_create_drv_rxlsp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ����rxlsp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_create_drv_rxlsp(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 1;
    NBB_INT         rv2 = 1;
    RX_LSP_T        lspt;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_ecmp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.mLabel =  ilms->key.inlabel;                        
    lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;

    lspt.tRxLspNh.tTxPort.slot = ilms->portinfo[0].slot;
    lspt.tRxLspNh.tTxPort.port = ilms->portinfo[0].port_id;
    lspt.tRxLspNh.tTxPort.vid = ilms->portinfo[0].svlan ;
     
    lspt.eNhiProtType = C3_PROT_NONE;
    lspt.tRxLspNh.tunNum = 1;

    if(( ilms->base.labelaction == 1 ) && ( ilms->nexthop[0].outlabel == 3 ))  
    {
        lspt.tRxLspNh.tunNum = 0;
    }
    
    lspt.tRxLspNh.cntEnable = 1;
    lspt.tRxLspNh.nhIp = ilms->nexthop[0].nextip;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.label = ilms->nexthop[0].outlabel;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.exp = 7;

    //��ѹ���ǩ
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.label = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.ttl = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.exp = 0;

    if( ilms->posid == 0 )
    {
        rv = ApiC3AddRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3AddRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv == 0)
        {
            ilms->posid = lspt.posId;
        }
        else
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop dev lsp ,rv=%d.\n",
                                                           __LINE__,rv);
            goto  EXIT_LABEL;
        }
    }
    else
    {
        lspt.posId = ilms->posid ;
        rv = ApiC3ModRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv == 0)
        {
            ilms->posid = lspt.posId;
        }
        else
        {
            rv = ADD_ILM_NEXTHOP_DRV_MODLSP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop dev modlsp,rv=%d.\n",
                                                           __LINE__,rv);
            goto  EXIT_LABEL;
        }
    }  
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_create_drv_ldpfrr
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ����rxlsp ldp frr����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_create_drv_ldpfrr(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 1;
    NBB_INT         rv2 = 1;
    RX_LSP_T        lspt;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_ecmp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.mLabel =  ilms->key.inlabel;                        
    lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;

    lspt.tRxLspNh.tTxPort.slot = ilms->portinfo[0].slot;
    lspt.tRxLspNh.tTxPort.port = ilms->portinfo[0].port_id;
    lspt.tRxLspNh.tTxPort.vid = ilms->portinfo[0].svlan ;
    lspt.tRxLspNh_p.tTxPort.slot = ilms->portinfo[1].slot;
    lspt.tRxLspNh_p.tTxPort.port = ilms->portinfo[1].port_id;
    lspt.tRxLspNh_p.tTxPort.vid = ilms->portinfo[1].svlan ;
     
    lspt.tRxLspNh.tunNum = 1;

    if(( ilms->base.labelaction == 1 ) && ( ilms->nexthop[0].outlabel == 3 ))  
    {
        lspt.tRxLspNh.tunNum = 0;
    }
    
    lspt.tRxLspNh.cntEnable = 1;
    lspt.tRxLspNh.nhIp = ilms->nexthop[0].nextip;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.label = ilms->nexthop[0].outlabel;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.exp = 7;

    //��ѹ���ǩ
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.label = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.ttl = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.exp = 0;

    lspt.eNhiProtType = C3_PROT_FRR;
    lspt.frrGroupId = ilms->frrid;
    lspt.tRxLspNh_p.tunNum = 1;
    lspt.tRxLspNh_p.cntEnable = 1;
    lspt.tRxLspNh_p.nhIp = ilms->nexthop[1].nextip;
    if(( ilms->base.labelaction == 1 ) && (  ilms->nexthop[1].outlabel == 3 ))  
    {
        lspt.tRxLspNh_p.tunNum = 0;
    }
    
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.label = ilms->nexthop[1].outlabel;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.exp = 7;

    //��ѹ���ǩ��ϵ
    lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.label = 0;
    lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.ttl = 0;
    lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.exp = 0;

    if( ilms->posid == 0 )
    {
        rv = ApiC3AddRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3AddRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv == 0)
        {
            ilms->posid = lspt.posId;
        }
        else
        {
            rv = ADD_ILM_NEXTHOP_DRV_LSP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop dev lsp ,rv=%d.\n",
                                                           __LINE__,rv);
            goto  EXIT_LABEL;
        }
    }
    else
    {
        lspt.posId = ilms->posid ;
        rv = ApiC3ModRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv == 0)
        {
            ilms->posid = lspt.posId;
        }
        else
        {
            rv = ADD_ILM_NEXTHOP_DRV_MODLSP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop dev modlsp,rv=%d.\n",
                                                           __LINE__,rv);
            goto  EXIT_LABEL;
        }
    }  
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_del_drv_rxlspprot
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ɾ��rxlsp��������
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_del_drv_rxlspprot(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_del_drv_rxpw ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    rv = ApiC3DelRxLspProt( BCMC31, ilms->posid);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3DelRxLspProt( BCMC32, ilms->posid);
        rv = (rv|rv2);
    }
    if( rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delrxlspprot label.p =%x.%d \n", 
                                    __LINE__, ilms->key.inlabel,ilms->posid);
    }
    else
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o delrxlspprot label.p =%x.%d \n", 
                                    __LINE__, ilms->key.inlabel,ilms->posid);
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_del_drv_rxlspecmp
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� ɾ��rxlsp ecmp����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_del_drv_rxlspecmp(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    RX_LSP_T        lspt;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_del_drv_rxpw ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  
    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.posId = ilms->posid ;
   
    lspt.tRxLspNh.tTxPort.slot = ilms->portinfo[0].slot;
    lspt.tRxLspNh.tTxPort.port = ilms->portinfo[0].port_id;
    lspt.tRxLspNh.tTxPort.vid = ilms->portinfo[0].svlan ;
    
    lspt.eNhiProtType = C3_PROT_NONE;
    lspt.tRxLspNh.tunNum = 1;
    lspt.tRxLspNh.cntEnable = 1;
    lspt.tRxLspNh.nhIp = ilms->nexthop[0].nextip;

    if(( ilms->base.labelaction == 1 ) && ( ilms->nexthop[0].outlabel == 3 ))  
    {
        lspt.tRxLspNh.tunNum = 0;
    }
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.label = ilms->nexthop[0].outlabel;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.exp = 7;

    //��ѹ���ǩ
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.label = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.ttl = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.exp = 0;
    lspt.tEcmpNh.ecmpIdx = ilms->ecmpid;
    rv = ApiC3DelRxLspEcmp( BCMC31,  &lspt);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3DelRxLspEcmp( BCMC32,  &lspt);
        rv = (rv|rv2);
    }
    if( rv != 0)
    {
        rv = ADD_ILM_NEXTHOP_DRV_DEL_ECMP_ERR;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delrxlspecmp label.p.e =%x.%d.%d \n", 
                                    __LINE__, ilms->key.inlabel,ilms->posid,ilms->ecmpid);
    }
    else
    {
        rv = ApiC3FreeEcmpGroupId( BCMC31,  ilms->ecmpid );
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3FreeEcmpGroupId( BCMC32, ilms->ecmpid );
            rv = (rv|rv2);
        }
        if( rv != 0)   
        {
            rv = ADD_ILM_NEXTHOP_DRV_FREE_ECMP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e freerxlspecmp label.p.e =%x.%d.%d \n", 
                                        __LINE__, ilms->key.inlabel,ilms->posid,ilms->ecmpid);
        }
        else
        {
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o freerxlspecmp label.p.e =%x.%d.%d \n", 
            //                          __LINE__, ilms->key.inlabel,ilms->posid,ilms->ecmpid);
            
            ilms->ecmpid = 0;
        }
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_ilm_mod_drv_ldpfrr
* ������   �� hwenjie
* �������� �� 2016��2��29��
* �������� �� �޸�rxlsp ldp frr����
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_ilm_mod_drv_ldpfrr(ILM_TAB *ilms NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 1;
    NBB_INT         rv2 = 1;
    RX_LSP_T        lspt;

    if (ilms == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm_create_drv_ecmp ilms NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.mLabel =  ilms->key.inlabel;  
    lspt.tRxLspNh.tTxPort.slot = ilms->portinfo[0].slot;
    lspt.tRxLspNh.tTxPort.port = ilms->portinfo[0].port_id;
    lspt.tRxLspNh.tTxPort.vid = ilms->portinfo[0].svlan;
    
    lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
    lspt.eNhiProtType = C3_PROT_NONE;
    lspt.tRxLspNh.tunNum = 1;

    if(( ilms->base.labelaction == 1 ) && ( ilms->nexthop[0].outlabel == 3 ))  
    {
        lspt.tRxLspNh.tunNum = 0;
    }
    
    lspt.tRxLspNh.cntEnable = 1;
    lspt.tRxLspNh.nhIp = ilms->nexthop[0].nextip;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.label = ilms->nexthop[0].outlabel;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.exp = 7;

    //��ѹ���ǩ
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.label = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.ttl = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.exp = 0;

    lspt.posId = ilms->posid;
    rv = ApiC3ModRxLsp( BCMC31,  &lspt);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3ModRxLsp( BCMC32,  &lspt);
        rv = (rv|rv2);
    }
    if( rv == 0)
    {
        ilms->posid = lspt.posId;
        
        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o ilmmodlsp label.p =%x.%d \n", 
        //                          __LINE__, ilms->key.inlabel,ilms->posid);
    }
    else
    {
        rv = ADD_ILM_NEXTHOP_DRV_MODLSP_ERR;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop drv mod rxlsp,rv=%d.\n",
                                                               __LINE__,rv);
        goto  EXIT_LABEL;
    }

    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
   
    lspt.tRxLspNh_p.tTxPort.slot = ilms->portinfo[1].slot;
    lspt.tRxLspNh_p.tTxPort.port = ilms->portinfo[1].port_id;
    lspt.tRxLspNh_p.tTxPort.vid = ilms->portinfo[1].svlan ;
   
    lspt.eNhiProtType = C3_PROT_FRR;
    
    //lspt.frrGroupId = ilms->frrid;
    lspt.tRxLspNh_p.tunNum = 1;
    lspt.tRxLspNh_p.cntEnable = 1;
    lspt.tRxLspNh_p.nhIp = ilms->nexthop[1].nextip;
    if(( ilms->base.labelaction == 1 ) && ( ilms->nexthop[1].outlabel == 3 ))  
    {
        lspt.tRxLspNh_p.tunNum = 0;
    }
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.label = ilms->nexthop[1].outlabel;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.exp = 7;

    //��ѹ���ǩ��ϵ
    lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.label = 0;
    lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.ttl = 0;
    lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.exp = 0;

    lspt.posId = ilms->posid;
    lspt.frrGroupId = ilms->frrid;
    rv = ApiC3AddRxLspProt( BCMC31,  &lspt);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3AddRxLspProt( BCMC32,  &lspt);
        rv = (rv|rv2);
    }
    if( rv != 0)
    {
        rv = ADD_ILM_NEXTHOP_RXLSP_PROT_ERR;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ilm:nexthop rxlsp prot,rv=%d.\n",
                                                               __LINE__,rv);
        goto  EXIT_LABEL;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

