/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_crlsp_drv.c
  版 本 号   : v0
  作    者   : wjhe
  生成日期   : 2016年2月29日
  最近修改   :
  功能描述   : CRLSPRX配置处理适配程序
  函数列表   :
  修改历史   :

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*************************************************************************

* 函数名   ： spm_l3_rxlsp_del_drv_frrgroup
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除frrfroup驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_rxlsp_del_drv_frrgroup(NBB_USHORT frrid NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

     if (frrid == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e rxlsp_create_drv_frrgroup frrid = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    
#ifdef SPU  
    rv = ApiC3DeleteFrrGroup( BCMC31, frrid );
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3DeleteFrrGroup( BCMC32, frrid);
        rv = (rv|rv2);
    }
    if( rv != 0)   
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Del rxlsp:drv frr err ,rv=%d.\n",
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

* 函数名   ： spm_l3_rxlsp_del_drv_frrgroup
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建frrfroup驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_rxlsp_create_drv_frrgroup(NBB_USHORT *frrid NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_USHORT  drv_frrid = 0;

    if (frrid == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e rxlsp_create_drv_frrgroup frrid = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    
#ifdef SPU  
    rv = ApiC3CreateFrrGroup( BCMC31, &drv_frrid, 0, 0);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3CreateFrrGroup( BCMC32, &drv_frrid, 0, 0);
        rv = (rv|rv2);
    }
    if( rv != 0)   
    {
        rv = ADD_CRLSP_RX_PROT_DRV_FRR_ERR;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add rxlsp: tefrr drv frr,rv=%d\n",
                                                                     __LINE__,rv);
        goto  EXIT_LABEL;
    }
    else
    {
        *frrid = drv_frrid;    
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}


/*************************************************************************

* 函数名   ： spm_l3_rxlsp_del_drv_rxlsp
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除rxlsp驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_rxlsp_del_drv_rxlsp(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    
#ifdef SPU  
   rv = ApiC3DelRxLsp( BCMC31, index );
   if ( SHARED.c3_num == 2 )
   {
       rv2 = ApiC3DelRxLsp( BCMC32, index);
       rv = (rv|rv2);
   }
   if( rv != 0)   
   {
       BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Del rxlsp:drv lsp err ,rv=%d.\n",
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

* 函数名   ： spm_l3_rxlsp_create_drv_rxlsp
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建rxlsp驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_rxlsp_create_drv_rxlsp(CRRXLSP *rxlsp NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    RX_LSP_T       lspt;

    if (rxlsp == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e rxlsp_create_drv_rxlsp= NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    
#ifdef SPU   
                 
    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.tRxLspNh.tTxPort.slot = rxlsp->mslot_id;
    lspt.tRxLspNh.tTxPort.port = rxlsp->mport_id;
    lspt.tRxLspNh.tTxPort.vid = rxlsp->msvlan ;
    lspt.tRxLspNh.nhIp = rxlsp->base.nextip;
    lspt.mIngress = rxlsp->key.ingress;
    lspt.mEgress = rxlsp->key.egress;
    lspt.mTunnelId = rxlsp->key.tunnelid;
    lspt.mLspId = rxlsp->key.lspid;
    lspt.mLabel = rxlsp->base.inlabel;
    
    switch( rxlsp->base.action )
    {
        case 0:
            lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
            break;
        case 1:
            lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
            break;
        case 2:
            lspt.eRxLspAction = MPLS_ACTION_IPOP;
            break;
        default:
            break;
    }
    
    //lspt.flags = RX_LSP_COUNTER_ON;
    lspt.eNhiProtType = C3_PROT_NONE;
    
    //if( base.tefrrnode == 1 )
    //{
        //lspt.eNhiProtType = PROT_FRR;
    //}
    lspt.tRxLspNh.tunNum = 1;
    if( rxlsp->base.action == 1 )
    {
        lspt.tRxLspNh.tunNum = 2;
    }
    if(( rxlsp->base.action == 0 ) && ( rxlsp->base.action == 3 ))  
    {
        lspt.tRxLspNh.tunNum = 0;
    }
    lspt.tRxLspNh.cntEnable = 1;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.label = rxlsp->base.outlabel;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.exp = 7;

    //无压入标签
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.label = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.exp = 7;

    if( rxlsp->posid == 0 )
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
        lspt.posId = rxlsp->posid;
        rv = ApiC3ModRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
    }

#endif

#ifdef SRC
    lspt.posId = 0xffff;
    rv = 0;
#endif  
#ifdef SNU
    lspt.posId = 0xffff;
    rv = 0;
#endif  
    if(rv == 0)
    {
        /*保存驱动返回的posid*/
        rxlsp->posid = lspt.posId;
    }

   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_rxlsp_create_drv_rxlspprot
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建rxlsp驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_rxlsp_create_drv_rxlspprot(CRRXLSP *rxlsp NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    RX_LSP_T       lspt;

    if (rxlsp == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e txlabelforoam key = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU
    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.tRxLspNh_p.tTxPort.slot = rxlsp->prot_slot_id;
    lspt.tRxLspNh_p.tTxPort.port = rxlsp->prot_port_id;
    lspt.tRxLspNh_p.tTxPort.vid = rxlsp->prot_svlan ;
    lspt.tRxLspNh_p.nhIp = rxlsp->dclsp.nextip;
    lspt.tRxLspNh_p.cntEnable = 1;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.label = rxlsp->dclsp.label1;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.exp = 7;
    
    lspt.tRxLspNh_p.tunNum = 1;
    if( rxlsp->base.tefrrtype == 1)
    {
        lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.label = rxlsp->dclsp.label2;
        lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.ttl = 255;
        lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.exp = 7;
        lspt.tRxLspNh_p.tunNum = 2;
    }
    lspt.frrGroupId = rxlsp->frrid;
    if( rxlsp->posid != 0 )
    {
        lspt.posId = rxlsp->posid;
        rv = ApiC3AddRxLspProt( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3AddRxLspProt( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv != 0)   
        {
            rv = ADD_CRLSP_RX_PROT_DRV_LSPP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add rxlsp: drv lspp err,rv=%d\n",
                                                                     __LINE__,rv);
            goto  EXIT_LABEL;
        }
    }
    else
    {
        lspt.tRxLspNh.tTxPort.slot = rxlsp->mslot_id;
        lspt.tRxLspNh.tTxPort.port = rxlsp->mport_id;
        lspt.tRxLspNh.tTxPort.vid = rxlsp->msvlan ;
        lspt.tRxLspNh.nhIp = rxlsp->base.nextip;
        lspt.mIngress = rxlsp->key.ingress;
        lspt.mEgress = rxlsp->key.egress;
        lspt.mTunnelId = rxlsp->key.tunnelid;
        lspt.mLspId = rxlsp->key.lspid;
        lspt.mLabel = rxlsp->base.inlabel;
        
        switch( rxlsp->base.action )
        {
            case 0:
                lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
                break;
            case 1:
                lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
                break;
            case 2:
                lspt.eRxLspAction = MPLS_ACTION_IPOP;
                break;
            default:
                break;
        }
        
        //lspt.flags = RX_LSP_COUNTER_ON;
        lspt.eNhiProtType = C3_PROT_FRR;
        lspt.tRxLspNh.tunNum = 1;
        if( rxlsp->base.action == 1 )
        {
            lspt.tRxLspNh.tunNum = 2;
        }
        lspt.tRxLspNh.cntEnable = 1;
        lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.label = rxlsp->base.outlabel;
        lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.ttl = 255;
        lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.exp = 7;

        //无压入标签
        lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.label = 0;
        lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.ttl = 255;
        lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.exp = 7;

        rv = ApiC3AddRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3AddRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv != 0)   
        {
            rv = ADD_CRLSP_RX_PROT_DRV_LSP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add rxlsp: tefrr drv lsp err,rv=%d\n",
                                                                     __LINE__,rv);
            goto  EXIT_LABEL;
        }
        else
        {
            rxlsp->posid = lspt.posId ;
        }
    }
#endif
#ifdef SRC
lspt.posId = 0xffff;
rv = 0;
#endif
#ifdef SNU
    lspt.posId = 0xffff;
    rv = 0;
#endif
 EXIT_LABEL:
 return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_rxlsp_mod_drv_rxlspprot
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建rxlsp驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_rxlsp_mod_drv_rxlspprot(CRRXLSP *rxlsp NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    RX_LSP_T       lspt;

    if (rxlsp == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e txlabelforoam key = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
#ifdef SPU
    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));
    lspt.tRxLspNh.tTxPort.slot = rxlsp->mslot_id;
    lspt.tRxLspNh.tTxPort.port = rxlsp->mport_id;
    lspt.tRxLspNh.tTxPort.vid = rxlsp->msvlan;
    lspt.tRxLspNh.nhIp = rxlsp->base.nextip;
    lspt.mIngress = rxlsp->key.ingress;
    lspt.mEgress = rxlsp->key.egress;
    lspt.mTunnelId = rxlsp->key.tunnelid;
    lspt.mLspId = rxlsp->key.lspid;
    lspt.mLabel = rxlsp->base.inlabel;
    
    switch( rxlsp->base.action )
    {
        case 0:
            lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
            break;
        case 1:
            lspt.eRxLspAction = MPLS_ACTION_LSP_SWAP;
            break;
        case 2:
            lspt.eRxLspAction = MPLS_ACTION_IPOP;
            break;
        default:
            break;
    }
    
    //lspt.flags = RX_LSP_COUNTER_ON;
    lspt.eNhiProtType = C3_PROT_FRR;
    lspt.tRxLspNh.tunNum = 1;
    if( rxlsp->base.action == 1 )
    {
        lspt.tRxLspNh.tunNum = 2;
    }
    lspt.tRxLspNh.cntEnable = 1;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.label = rxlsp->base.outlabel;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh.tSwapTunLabel.label_un.tlabel.exp = 7;

    //无压入标签
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.label = 0;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh.tPushTunLabel.label_un.tlabel.exp = 7;

    lspt.tRxLspNh_p.tTxPort.slot = rxlsp->prot_slot_id;
    lspt.tRxLspNh_p.tTxPort.port = rxlsp->prot_port_id;
    lspt.tRxLspNh_p.tTxPort.vid = rxlsp->prot_svlan ;
    lspt.tRxLspNh_p.nhIp = rxlsp->dclsp.nextip;
    lspt.tRxLspNh_p.cntEnable = 1;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.label = rxlsp->dclsp.label1;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.ttl = 255;
    lspt.tRxLspNh_p.tSwapTunLabel.label_un.tlabel.exp = 7;
    
    lspt.tRxLspNh_p.tunNum = 1;
    if( rxlsp->base.tefrrtype == 1)
    {
        lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.label = rxlsp->dclsp.label2;
        lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.ttl = 255;
        lspt.tRxLspNh_p.tPushTunLabel.label_un.tlabel.exp = 7;
        lspt.tRxLspNh_p.tunNum = 2;
    }
    lspt.frrGroupId = rxlsp->frrid;
    if( rxlsp->posid == 0 )
    {
        rv = ApiC3AddRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3AddRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv != 0)   
        { 
            rv = ADD_CRLSP_RX_PROT_DRV_LSP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add rxlsp: tefrr drv lsp err,rv=%d\n",
                                                                     __LINE__,rv);
            
            goto  EXIT_LABEL;
        }
        else
        {
            rxlsp->posid = lspt.posId ;
        }
    }
    else
    {
        lspt.posId = rxlsp->posid;
        rv = ApiC3ModRxLsp( BCMC31,  &lspt);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModRxLsp( BCMC32,  &lspt);
            rv = (rv|rv2);
        }
        if( rv != 0)   
        {
            rv = ADD_CRLSP_RX_PROT_DRV_MODLSP_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add rxlsp: tefrr drv modlsp err,rv=%d\n",
                                                                     __LINE__,rv);
            goto  EXIT_LABEL;
        }
    }
#endif
#ifdef SRC
lspt.posId = 0xffff;
rv = 0;
#endif
#ifdef SNU
lspt.posId = 0xffff;
rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_rxlsp_del_drv_frrgroup
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除rxlsp保护组驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_rxlsp_del_drv_rxlspprot(NBB_ULONG index NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    
#ifdef SPU
    rv = ApiC3DelRxLspProt( BCMC31, index );
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3DelRxLspProt( BCMC32, index);
        rv = (rv|rv2);
    }
    if( rv != 0)   
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  Add rxlsp: tefrr drv del lspp err,rv=%d\n",
                                                                         __LINE__,rv); 
        goto  EXIT_LABEL;
    }
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}




