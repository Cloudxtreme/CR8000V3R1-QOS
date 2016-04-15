/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_l3_ftn_drv.c
  版 本 号   : v0
  作    者   : wjhe
  生成日期   : 2016年2月29日
  最近修改   :
  功能描述   : ftn配置处理适配程序
  函数列表   :
  修改历史   :

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>
/*************************************************************************

* 函数名   ： spm_l3_ftn_del_drv_txlsp
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除txlsp驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_del_drv_txlsp(NBB_ULONG index  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (index == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_drv_txlsp index = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    rv = ApiC3DelTxLsp( BCMC31, index );
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3DelTxLsp( BCMC32, index );
        rv = (rv|rv2);
    }
    if( rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_drv_txlsp  drv txlsp index = 0 \n", __LINE__);
        
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

/*************************************************************************

* 函数名   ： spm_l3_ftn_del_drv_txpw
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除txpw驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_del_drv_txpw(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_drv_txpw uftn null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    rv = ApiC3DelL3TxPw(BCMC31, uftn->pwposid);
     if ( SHARED.c3_num == 2 )
     {
         rv2 = ApiC3DelL3TxPw(BCMC32, uftn->pwposid);
         rv = (rv|rv2);
     }
     if( rv != 0)
     {
         rv = DEL_FTN_DRV_L3TXPW_ERR;
         BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Del ftn:Del L3TXPW,rv=%d\n",
                                                                   __LINE__,rv);
         goto  EXIT_LABEL;
     }
     else
     {
         uftn->pwposid = 0;
     }

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_del_aps_entity
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除aps保护实体
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_del_aps_entity(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_aps_entity uftn null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    spm_lps_del_entity( TUNNEL_TYPE, uftn->tunnelid);
    rv = 0;

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_del_aps_tunnel_ecmp
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除aps tunnel ecmp实体
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_del_aps_tunnel_ecmp(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_aps_tunnel_ecmp uftn null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    aps_del_tunnel_ecmp(uftn->ecmpid);
    rv = 0;

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_del_drv_tunnel
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除驱动 tunnel 资源
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_del_drv_tunnel(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_drv_tunnel uftn null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

 
     rv = ApiC3DeleteTunnel( BCMC31, uftn->tunnelid );
     if ( SHARED.c3_num == 2 )
     {
         rv2 = ApiC3DeleteTunnel( BCMC32, uftn->tunnelid );
         rv = (rv|rv2);
     }
     if( rv != 0)
     {
         BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Del ftn:Del Tunnel,rv=%d\n",
                                                                    __LINE__,rv); 
         goto  EXIT_LABEL;
     }
     else
     {
         uftn->tunnelid = 0;
     }

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_del_drv_txlsp
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除txlsp驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_add_drv_txlsp(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    TX_LSP_T            lsptun;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_add_drv_txlsp uftn = null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    for ( i = 0; i < uftn->hopnum; i++)
    {
        OS_MEMSET(&lsptun , 0, sizeof(TX_LSP_T));
        
        lsptun.tTxPort.slot = uftn->portinfo[i].slot;
        lsptun.tTxPort.port = uftn->portinfo[i].port_id;
        lsptun.tTxPort.vid = uftn->portinfo[i].svlan ;
        
        lsptun.mVrf = uftn->key.vrfid;
        lsptun.mFecv4 = uftn->key.fec;
        lsptun.mFecv4Mask = uftn->key.mask;
        lsptun.mNhipv4 = uftn->dclsp[i].nextip;

        lsptun.flags = 0;
        lsptun.tunNum = 1;
        if( uftn->dclsp[i].label2 != 0xffffffff)  
        {
            lsptun.tunNum = 2;
        }
        if( uftn->dclsp[i].label2 == 3)  
        {
            lsptun.tunNum = 1;
        }
        if( uftn->dclsp[i].label1 == 3)  
        {
            lsptun.tunNum = 0;
        }
        lsptun.tSwapTunLabel.label_un.tlabel.label = uftn->dclsp[i].label1;
        lsptun.tSwapTunLabel.label_un.tlabel.ttl = 255;
        lsptun.tSwapTunLabel.label_un.tlabel.exp = 0;

        lsptun.tPushTunLabel.label_un.tlabel.label = uftn->dclsp[i].label2;
        lsptun.tPushTunLabel.label_un.tlabel.ttl = 255;
        lsptun.tPushTunLabel.label_un.tlabel.exp = 0;

        lsptun.nhIp = uftn->dclsp[i].nextip;
        
        if( uftn->posid[i] == 0)    
        {
            rv = ApiC3AddTxLsp( BCMC31 ,&lsptun);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3AddTxLsp( BCMC32 ,&lsptun);
                rv = (rv|rv2);
            }
        }
        else
        {
            lsptun.posId = uftn->posid[i];
            lsptun.lspIdx = uftn->index[i];
            rv = ApiC3ModTxLsp( BCMC31 ,&lsptun); 
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3ModTxLsp( BCMC32 ,&lsptun); 
                rv = (rv|rv2);
            }
        }
        if( rv != 0)
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ftn:lsp dev ,rv=%d,\n",
                                                                              __LINE__,rv);
            goto  EXIT_LABEL;
        }
        else
        {
            uftn->posid[i] = lsptun.posId;
            uftn->index[i] = lsptun.lspIdx;
        }
    }


#else
    rv = 0;
    uftn->index[0] = 0xffffffff;
    uftn->posid[0] = 0xffffffff;
    uftn->index[1] = 0xffffffff;
    uftn->posid[1] = 0xffffffff;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_add_drv_tunnel
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建tunnel驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_add_drv_tunnel(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    TUNNEL_T            tunnel;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_add_drv_tunnel uftn = null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

   OS_MEMSET(&tunnel , 0, sizeof(TUNNEL_T));
 
   
   tunnel.eProt = C3_PROT_NONE;
   tunnel.txLspPosId = uftn->posid[0];
   
   if ( uftn->tunnelid == 0)
   {
        rv = ApiC3CreateTunnel( BCMC31 ,&tunnel);

        if ( SHARED.c3_num == 2 )
        {
            tunnel.tTunnelEcmp.ecmpIdx = 0;
            rv2 = ApiC3CreateTunnel( BCMC32 ,&tunnel); 
            rv = (rv|rv2);
        }
   }
   else
   {
        tunnel.tunnelId = uftn->tunnelid;
        rv = ApiC3ModTunnel( BCMC31 ,&tunnel); 
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModTunnel( BCMC32 ,&tunnel); 
            rv = (rv|rv2);
        }
   }
   if(rv != 0)
   {
       BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  FTN error:tunnel dev ,rv=%d,\n",
                                                                                      __LINE__,rv);
   }
   else
   {
       uftn->tunnelid = tunnel.tunnelId;
   }


#else
    rv = 0;
    uftn->tunnelid = 0xffffffff;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_add_drv_tunnel_frr
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建tunnel frr 驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_add_drv_tunnel_frr(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    TUNNEL_T            tunnel;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_add_drv_tunnel_frr uftn = null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

   OS_MEMSET(&tunnel , 0, sizeof(TUNNEL_T));
 
    tunnel.eProt = C3_PROT_FRR;
    for ( i = 0; i < uftn->hopnum; i++)
    {
        if( uftn->nexthop[i].nexttypeb < 2)
        {
            tunnel.txLspPosId = uftn->posid[i];
        }
        else if( uftn->nexthop[i].nexttypeb == 2)
        {
            tunnel.backupTxLspPosId = uftn->posid[i];
         
        }
    }   
    if( tunnel.txLspPosId == 0)
    {
        tunnel.txLspPosId = tunnel.backupTxLspPosId;
    }
    if( tunnel.backupTxLspPosId == 0)
    {
        tunnel.backupTxLspPosId = tunnel.txLspPosId; 
   }
    
   
   if ( uftn->tunnelid == 0)
   {
        rv = ApiC3CreateTunnel( BCMC31 ,&tunnel);

        if ( SHARED.c3_num == 2 )
        {
            tunnel.tTunnelEcmp.ecmpIdx = 0;
            rv2 = ApiC3CreateTunnel( BCMC32 ,&tunnel); 
            rv = (rv|rv2);
        }
   }
   else
   {
        tunnel.tunnelId = uftn->tunnelid;
        rv = ApiC3ModTunnel( BCMC31 ,&tunnel); 
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModTunnel( BCMC32 ,&tunnel); 
            rv = (rv|rv2);
        }
   }
   if(rv != 0)
   {
       BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ftn:tunnel frr dev ,rv=%d,\n",
                                                                                      __LINE__,rv);
   }
   else
   {
       uftn->tunnelid = tunnel.tunnelId;
   }


#else
    rv = 0;
    uftn->tunnelid = 0xffffffff;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_add_drv_tunnel_ecmp
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建tunnel ecmp 驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_add_drv_tunnel_ecmp(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    TUNNEL_T            tunnel;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_drv_txlsp uftn = null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

   OS_MEMSET(&tunnel , 0, sizeof(TUNNEL_T));
 

    tunnel.eProt = C3_PROT_ECMP;
    tunnel.tTunnelEcmp.ecmpNum = uftn->hopnum;
    
    //tunnel.tTunnelEcmp.alg = ECMP_HASH_SIP;
    if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 0 )
    {
        tunnel.tTunnelEcmp.alg = ECMP_HASH_DSIP;
    }
    else if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 1 )
    {
        tunnel.tTunnelEcmp.alg = ECMP_PORT_ROUND_ROBIN;
    }
    tunnel.tTunnelEcmp.txlspPosId[0] = uftn->posid[0];
    tunnel.tTunnelEcmp.txlspPosId[1] = uftn->posid[1];
    tunnel.tTunnelEcmp.txlspPosId[2] = uftn->posid[2];
    tunnel.tTunnelEcmp.txlspPosId[3] = uftn->posid[3];
    if ( uftn->tunnelid == 0)
    {
        rv = ApiC3CreateTunnel( BCMC31 ,&tunnel);
        if ( SHARED.c3_num == 2 )
        {
            tunnel.tTunnelEcmp.ecmpIdx = 0;
            rv2 = ApiC3CreateTunnel( BCMC32 ,&tunnel); 
            rv = (rv|rv2);
        }
    }
    else
    {
        tunnel.tunnelId = uftn->tunnelid;
        if ( uftn->base.nexttype == 2)
        {
            tunnel.tTunnelEcmp.ecmpIdx = uftn->ecmpid;
        }
        rv = ApiC3ModTunnel( BCMC31 ,&tunnel); 
        if ( SHARED.c3_num == 2 )
        {
            if ( uftn->base.nexttype == 2)
            {
                tunnel.tTunnelEcmp.ecmpIdx = uftn->ecmpid;
            }
            rv2 = ApiC3ModTunnel( BCMC32 ,&tunnel); 
            rv = (rv|rv2);
        }
    }

    if(rv != 0)
    {
       BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ftn:tunnel ecmp dev ,rv=%d,\n",
                                                                                      __LINE__,rv);
    }
    else
    {
        uftn->tunnelid = tunnel.tunnelId; 
        uftn->ecmpid = tunnel.tTunnelEcmp.ecmpIdx;
        
    }


#else
    rv = 0;
    uftn->tunnelid = 0xffffffff;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_add_drv_txpw
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建txpw 驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_add_drv_txpw(FTN_TAB *uftn  NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    L3_TX_PW_T          txpw;
    L3_TX_PW_T          temp_txpw;


    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_drv_txlsp uftn = null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

   OS_MEMSET(&txpw , 0, sizeof(L3_TX_PW_T));
   OS_MEMSET(&temp_txpw , 0, sizeof(L3_TX_PW_T));
    
   txpw.mVrf = uftn->key.vrfid;
   txpw.mPeerIp = uftn->key.fec;
   txpw.mLabel = 0;

   txpw.pushPwNum = 0;
   txpw.flags |= L3_TX_PW_COUNTER_ON;
   txpw.tunnelIdx = uftn->tunnelid; 
   txpw.eNniMode = PHB_TO_EXP;
   if( uftn->pwposid == 0)
   {
       rv = ApiC3AddL3TxPw(BCMC31, &txpw);
       if ( SHARED.c3_num == 2 )
       {
            rv2 = ApiC3AddL3TxPw(BCMC32, &txpw); 
            rv = (rv|rv2);
       }
   }
   else
   {
       txpw.posId = uftn->pwposid ;
       txpw.nhIdx = uftn->pwnhi;

       rv = fhdrv_psn_l3_get_txpw(BCMC31, &txpw, &temp_txpw);
       if (rv == 0)
       {
           txpw.eNniMode = temp_txpw.eNniMode;
       }
       else
       {
           rv = ADD_FTN_GET_DRV_TXPW_ERR;
           BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, 
                    "Line=%d e ftn fhdrv_psn_l3_get_txpw posid.eNniMode.rv =%d.%d.%d\n", 
                            __LINE__, txpw.posId, txpw.eNniMode,rv);
       }
        
       rv = ApiC3ModL3TxPw(BCMC31, &txpw);
       if ( SHARED.c3_num == 2 )
       {
           rv2 = ApiC3ModL3TxPw(BCMC32, &txpw);
           rv = (rv|rv2);
       }
    }
    if( rv == 0)   
    {
        uftn->pwposid = txpw.posId;
        uftn->pwnhi = txpw.nhIdx;
    }
    else
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add ftn:L3TxPw dev ,rv=%d,\n",
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

* 函数名   ： spm_l3_ftn_add_frr_toaps
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 传递保护参数aps
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_add_frr_toaps(FTN_TAB *uftn,NBB_ULONG mport,NBB_ULONG mnextip,
                                      NBB_ULONG bakport,NBB_ULONG baknextip NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    SPM_LINEAR_PROTECT_SWITCH       stLps;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_drv_txlsp uftn = null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&stLps, 0, sizeof(SPM_LINEAR_PROTECT_SWITCH));
    stLps.revertiveType = LPS_REVERTIVE;
    stLps.holdOffTime = 0;
    stLps.wtrTime = 0;
    
    stLps.workEntity.portIndex = mport;
    stLps.workEntity.port = 0;
    stLps.workEntity.slot = 0;
    stLps.workEntity.tpId = 0;
    stLps.workEntity.type = LPS_IP_ADDR;
    
    //OS_MEMCPY( &(stLps.workEntity.ldpKey), &(uftn->key), sizeof( FTN_KEY ) );
    stLps.workEntity.routeKey.vrfid = 0;
    stLps.workEntity.routeKey.addrtype = 0;
    stLps.workEntity.routeKey.mask = 32;
    stLps.workEntity.routeKey.dip[0] = 0;
    stLps.workEntity.routeKey.dip[1] = 0;
    stLps.workEntity.routeKey.dip[2] = 0;
    stLps.workEntity.routeKey.dip[3] = mnextip;

    stLps.protEntity.portIndex = bakport;
    stLps.protEntity.port = 0;
    stLps.protEntity.slot = 0;
    stLps.protEntity.tpId = 0;
    stLps.protEntity.type = LPS_IP_ADDR;
    
    //OS_MEMCPY( &(stLps.protEntity.ldpKey), &(uftn->key), sizeof( FTN_KEY ) );
    stLps.protEntity.routeKey.vrfid = 0;
    stLps.protEntity.routeKey.addrtype = 0;
    stLps.protEntity.routeKey.mask = 32;
    stLps.protEntity.routeKey.dip[0] = 0;
    stLps.protEntity.routeKey.dip[1] = 0;
    stLps.protEntity.routeKey.dip[2] = 0;
    stLps.protEntity.routeKey.dip[3] = baknextip;
    
    stLps.protType = TUNNEL_TYPE;
    stLps.protGroupId = uftn->tunnelid;
    stLps.lpsType = FRR_1B1;

    spm_lps_add_entity(&stLps);
    rv = 0;
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ftn_add_ecmp_toaps
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 传递ldp ecmp参数aps
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ftn_add_ecmp_toaps(FTN_TAB *uftn NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    APS_TUNNEL_ECMP_ST    apsecmp;

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn_del_drv_txlsp uftn = null \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU  

    OS_MEMSET(&apsecmp, 0, sizeof(APS_TUNNEL_ECMP_ST));
    apsecmp.ecmpNum = uftn->hopnum;
    apsecmp.eProt = C3_PROT_ECMP;
    apsecmp.tunnelId = uftn->tunnelid;

    if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 0 )
    {
        apsecmp.alg = ECMP_HASH_DSIP;
    }
    else if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 1 )
    {
        apsecmp.alg = ECMP_PORT_ROUND_ROBIN;
    }
    apsecmp.ecmpNextHop[0].txlspPosId = uftn->posid[0];
    apsecmp.ecmpNextHop[0].portIndex = uftn->dclsp[0].nextport;
    apsecmp.ecmpNextHop[0].vrfKey.vrfid = 0;
    apsecmp.ecmpNextHop[0].vrfKey.addrtype = 0;
    apsecmp.ecmpNextHop[0].vrfKey.mask = 32;
    apsecmp.ecmpNextHop[0].vrfKey.dip[3] = uftn->dclsp[0].nextip;

    apsecmp.ecmpNextHop[1].txlspPosId = uftn->posid[1];
    apsecmp.ecmpNextHop[1].portIndex = uftn->dclsp[1].nextport;
    apsecmp.ecmpNextHop[1].vrfKey.vrfid = 0;
    apsecmp.ecmpNextHop[1].vrfKey.addrtype = 0;
    apsecmp.ecmpNextHop[1].vrfKey.mask = 32;
    apsecmp.ecmpNextHop[1].vrfKey.dip[3] = uftn->dclsp[1].nextip;

    apsecmp.ecmpNextHop[2].txlspPosId = uftn->posid[2];
    apsecmp.ecmpNextHop[2].portIndex = uftn->dclsp[2].nextport;
    apsecmp.ecmpNextHop[2].vrfKey.vrfid = 0;
    apsecmp.ecmpNextHop[2].vrfKey.addrtype = 0;
    apsecmp.ecmpNextHop[2].vrfKey.mask = 32;
    apsecmp.ecmpNextHop[2].vrfKey.dip[3] = uftn->dclsp[2].nextip;
    
    apsecmp.ecmpNextHop[3].txlspPosId = uftn->posid[3];
    apsecmp.ecmpNextHop[3].portIndex = uftn->dclsp[3].nextport;
    apsecmp.ecmpNextHop[3].vrfKey.vrfid = 0;
    apsecmp.ecmpNextHop[3].vrfKey.addrtype = 0;
    apsecmp.ecmpNextHop[3].vrfKey.mask = 32;
    apsecmp.ecmpNextHop[3].vrfKey.dip[3] = uftn->dclsp[3].nextip;

    apsecmp.ecmpIdx = uftn->ecmpid;
    aps_add_tunnel_ecmp(&apsecmp);
    rv = 0;


#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}



