/**MOD+***********************************************************************/
/* Module:    spml3.c                                                      */
/*                                                                           */
/* Purpose:   R8000     L3    cfgprocess                        */
/*                                                                           */
/* FiberHome        2003-2012                                                */
/*                                                                           */
/* $Revision: 1.0       $ $Date: 2012/9/2 07:09:09 $             */
/**MOD-***********************************************************************/



#define SHARED_DATA_TYPE SPM_SHARED_LOCAL
#include <nbase.h>
#include <spmincl.h>

/********************
FTN配置块
********************/
#ifdef FTN_T

/*ftn模块中的打印设置*/
extern unsigned char l3_ftn_cfg_print_setting;

/******************************************************************************
 * FunctionName 	: 	spm_l3_ftncomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_ftncomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    FTN_KEY *key1 = (FTN_KEY *)keyarg1;
    FTN_KEY *key2 = (FTN_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_ftncomp");

    rv = compare_ushort(&key1->vrfid, &key2->vrfid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    rv = compare_ulong(&key1->fec, &key2->fec NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    rv = compare_byte(&key1->mask, &key2->mask NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_ftncomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_peipcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    PEIP_KEY *key1 = (PEIP_KEY *)keyarg1;
    PEIP_KEY *key2 = (PEIP_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_uniportcomp");

    rv = compare_ulong(&key1->peer, &key2->peer NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}


NBB_VOID spm_l3_ftntreeinit( NBB_CXT_T NBB_CXT)
{
    AVLL_INIT_TREE(SHARED.ftn_tree,
                    spm_l3_ftncomp,
                    NBB_OFFSETOF(FTN_TAB, key),/*lint !e413 */
                    NBB_OFFSETOF(FTN_TAB, ftn_node));/*lint !e413 */
    
    AVLL_INIT_TREE(SHARED.peftn_tree,
                    spm_l3_peipcomp,
                    NBB_OFFSETOF(PEERFTN, key),/*lint !e413 */
                    NBB_OFFSETOF(PEERFTN, pe_node));/*lint !e413 */

     BmuRmLog(SPM_L3_LOG_DIR);
     BMU_SLOG(BMU_CRIT, SPM_L3_LOG_DIR, "****************L3******************\n");
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_ftncomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_ftnupdatefib(NBB_BYTE flag,NBB_ULONG nhid, NBB_BYTE mask, NBB_ULONG dip NBB_CCXT_T NBB_CXT)
{
    NBB_INT             rv = 100;
    NBB_INT             rv2 = 100;
    NBB_ULONG           i ;
    VRFUROUTE_KEY       vrfukey;
    VRFUROUTE           *uroute = NULL;
    L3_ROUTE_T          l3route;

    NBB_TRC_ENTRY("spm_l3_ftnupdatefib");
    
    if (mask == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftnupdatefib mask = 0 \n", __LINE__);
        rv = DEL_FTN_UPDATE_FIB_PARA_ERR;
        goto  EXIT_LABEL;
    }
    
    if (dip == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftnupdatefib dip = 0 \n", __LINE__);
        rv = DEL_FTN_UPDATE_FIB_PARA_ERR;
        goto  EXIT_LABEL;
    }
    
    OS_MEMSET(&vrfukey, 0, sizeof(VRFUROUTE_KEY));
    vrfukey.vrfid = 0;
    vrfukey.addrtype = 0;
    vrfukey.mask = mask;
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &(vrfukey.dip[3]), &dip, 4 );

    uroute = (VRFUROUTE *)AVLL_FIND(  SHARED.vrfuroute_tree ,&vrfukey ); 
    if (uroute == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "ftnupdatefib Error: route null"));
        rv = 0;
        goto  EXIT_LABEL;
    }
    for ( i = 0; i < uroute->ipv4num; i++)
    {
        if ( uroute->nexthop[i].label != 0xffffffff )
        {
            NBB_TRC_FLOW((NBB_FORMAT "ftnupdatefib Error: route nni"));
            rv = DEL_FTN_UPDATE_FIB_ROUTE_LABAL_ERR;
            goto  EXIT_LABEL;
        }
    }
    #ifdef SPU
    ApiC3InitL3RouteStruct( &l3route );
    
    l3route.vrfId = uroute->key.vrfid;
    l3route.mIpPktType = IPV4_PKT_UNICAST;
    NBB_MEMCPY( &(l3route.mDstIpv4), &(uroute->key.dip[3]), 4 );
    l3route.mDstIpv4Mask = uroute->key.mask;
    
    if ( uroute->base.routetype == 1 )
    {
        l3route.eRouteType = ROUTE_TYPE_INDIRECT;
    }
    else    if ( uroute->base.routetype == 0 )
    {
        l3route.eRouteType = ROUTE_TYPE_DIRECT;
    }
    else    if ( uroute->base.routetype == 2 )
    {
        l3route.eRouteType = ROUTE_TYPE_LOCAL;
    }
    
    if ( uroute->base.routetype != 2 )
    {
        if ( flag == 1 )
        {
            l3route.eVpnProt = VPN_PROTECT_NONE;
            l3route.nhIdx = nhid;
        }
        else if ( flag == 2 )
        {
            if ( uroute->base.prmode == 0 )
            {
                l3route.eVpnProt = VPN_PROTECT_NONE;
                l3route.nhIdx = uroute->nextindex[0];
                NBB_TRC_DETAIL((NBB_FORMAT "ftnupdatefib uni nhi = %x ", l3route.nhIdx));
            }
            else if ( uroute->base.prmode == 3 )
            {
                l3route.eVpnProt = VPN_PROTECT_FRR;
                l3route.frrGroupId = uroute->frrecmpid;

                for ( i = 0; i <  uroute->ipv4num; i++)
                {
                    if( uroute->nexthop[i].nexttypeb == 1 )
                    {
                        l3route.nhIdx = uroute->nextindex[i];
                    }
                    else if( uroute->nexthop[i].nexttypeb == 2 )
                    {
                        l3route.nhIdx_p = uroute->nextindex[i];
                    }
                }
                
                //主用不存在
                if( l3route.nhIdx == 0)
                {
                    l3route.nhIdx = l3route.nhIdx_p;
                    l3route.nhIdx_p = 0;
                }
                NBB_TRC_DETAIL((NBB_FORMAT "ftnupdatefib uni m nhi = %x ", l3route.nhIdx));
                NBB_TRC_DETAIL((NBB_FORMAT "ftnupdatefib uni b nhi = %x ", l3route.nhIdx_p));
                
            }
            else if ( uroute->base.prmode == 2 )
            {
                l3route.eVpnProt = VPN_PROTECT_ECMP;
                l3route.tL3Ecmp.ecmpNum =  uroute->base.ecmpnum;
                if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 0 )
                {
                    l3route.tL3Ecmp.alg = ECMP_HASH_DSIP;
                }
                else if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 1 )
                {
                    l3route.tL3Ecmp.alg = ECMP_PORT_ROUND_ROBIN;
                }
                l3route.tL3Ecmp.ecmpIdx = uroute->frrecmpid;
                for ( i = 0; i < uroute->base.ecmpnum; i++)
                {
                    l3route.tL3Ecmp.nhIdxGroup[i] = uroute->nextindex[i];
                }
            }
        }
        rv = ApiC3SetL3UCRoute(BCMC31, &l3route);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3SetL3UCRoute(BCMC32, &l3route);
            rv = (rv|rv2);
        }
        if( rv == 0)   
        {
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o ftn-fib fec.flag =%x.%d \n", __LINE__, dip,flag);
        }
        else
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn-fib fec.flag.rv =%x.%d.%d \n", __LINE__, dip,flag,rv);
            rv = DEL_FTN_UPDATE_FIB_ROUTE_DRV_ERR;
            goto  EXIT_LABEL;
        }
    }
    
#endif

    rv = 0;

    EXIT_LABEL:
        
    NBB_TRC_EXIT();
    return rv;
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_ftnprocess
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_ftnprocess( ATG_DCI_SET_FTN  *ftnips NBB_CCXT_T NBB_CXT)
{

    NBB_INT             rv = 100;

    NBB_INT             rv2 = 100;

    NBB_BYTE            *ucdata = NULL;

    NBB_BYTE            routetype = 0;

    NBB_BYTE            flag = 0;

    NBB_ULONG           nxhopnum = 0,posid[8],index[8],port = 0,
                        moutport = 0,boutport = 0,mnextip = 0,bnextip = 0;
    NBB_ULONG           oldhopnum = 0;

    FTN_KEY             ftnkey;

    FTN_TAB             *uftn = NULL;

    FTN_NHOP            newhop[8];
    FTN_NHOP            oldhop[8];
    ATG_DCI_FTN_NXHOP_DATA      *nexthp = NULL;
    
    LSPPROT_KEY         tunnelkey;

    DC_LSP              dclsp;

    NBB_ULONG           i ;
    
    NBB_ULONG           j;

    TX_LSP_T            lsptun;

    TUNNEL_T            tunnel;

    L3_TX_PW_T          txpw;
    L3_TX_PW_T          temp_txpw;

    //L3_ROUTE_T            l3route;

    SPM_PORT_INFO_CB    portfo;

    SPM_LINEAR_PROTECT_SWITCH       stLps;

    ATG_DCI_BFD_LDP_LSP_DATA        bfdldp;

    APS_TUNNEL_ECMP_ST              apsecmp;

    NBB_ULONG           logport = 0;
    NBB_ULONG           mlsposid = 0;
    NBB_ULONG           baklsposid = 0;
    NBB_CHAR            *errorlog = NULL;
    NBB_ULONG           str_len = 0;

    NBB_TRC_ENTRY("spm_l3_ftnprocess");


    NBB_ASSERT(ftnips != NULL);

    ftnips->return_code = ATG_DCI_RC_OK;

    OS_MEMSET( &ftnkey, 0, sizeof( FTN_KEY ) );
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &ftnkey, &(ftnips->key), sizeof( FTN_KEY ) );

    if( ftnkey.fec == 0 )
    {
        NBB_PRINTF("FTN data error  fec = 0 \n");
        ftnips->return_code = ATG_DCI_RC_UNSUCCESSFUL;  
        goto  EXIT_LABEL;
    }
    NBB_TRC_DETAIL((NBB_FORMAT "FTN vrf =%d mask=%d fec =%x",ftnkey.vrfid,ftnkey.mask, ftnkey.fec));

    /*操作之前，查看本地数据是否存在*/
    uftn =  (FTN_TAB *)AVLL_FIND( SHARED.ftn_tree ,  &ftnkey ); 

    if( ftnips->delete_struct == 1) /*  删除*/
     {
         if (l3_ftn_cfg_print_setting == SPM_PRINT_CFG)
         {
             BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "del ftn v.m.p =%d.%d.%x \n", ftnkey.vrfid,ftnkey.mask,ftnkey.fec);
         }
         if( uftn == NULL )
         {
             NBB_TRC_FLOW((NBB_FORMAT "e Del trn: data null"));
             ftnips->return_code = ATG_DCI_RC_OK;  
             rv = 0;
             goto  EXIT_LABEL;
         }
 
         if((uftn->l2num == 0) && (uftn->l3num == 0))
         {
             if ( ftnkey.vrfid == 0 )
             {
                 if( uftn->pwposid != 0)
                 {             
                     rv = spm_l3_ftnupdatefib( 2, 0, uftn->key.mask, uftn->key.fec NBB_CCXT);
                     if( rv != 0)
                     {
                         BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del ftn:spm_l3_ftnupdatefib,rv=%d\n",
                                                                                              __LINE__,rv);
                         uftn->return_code = rv;
                         goto  EXIT_LABEL;
                     }
                     rv = spm_l3_ftn_del_drv_txpw(uftn);
                     if( rv != 0)
                     {
                         rv = DEL_FTN_DRV_L3TXPW_ERR;
                         uftn->return_code = rv;
                         goto  EXIT_LABEL;
                     }  
                 }
                 if( uftn->base.nexttype == 1)
                 {
                     rv = spm_l3_ftn_del_aps_entity(uftn);
                 }
                 if( uftn->base.nexttype == 2)
                 {
                     rv = spm_l3_ftn_del_aps_tunnel_ecmp(uftn);
                 }
                 if( uftn->tunnelid != 0)
                 {
                     rv = spm_l3_ftn_del_drv_tunnel(uftn);
                     if(rv != 0)
                     {
                         rv = DEL_FTN_DRV_TUNNEL_ERR;
                         uftn->return_code = rv;
                     }  
                 }
                 if( uftn->posid[0] != 0)
                 {
                     rv = spm_l3_ftn_del_drv_txlsp(uftn->posid[0]);
                     if( rv != 0)
                     {
                         rv = DEL_FTN_DRV_LSP_ERR;
                         uftn->return_code = rv;
                         goto  EXIT_LABEL;
                     }
                     else
                     {
                         uftn->posid[0] = 0;
                     }
                 }
                 if( uftn->posid[1] != 0)
                 {
                     rv = spm_l3_ftn_del_drv_txlsp(uftn->posid[1]);
                     if( rv != 0)
                     {
                         rv = DEL_FTN_DRV_LSP_ERR;
                         uftn->return_code = rv;
                         goto  EXIT_LABEL;
                     }
                     else
                     {
                         uftn->posid[1] = 0;
                     }
                 }
                 if( uftn->posid[2] != 0)
                 {
                     rv = spm_l3_ftn_del_drv_txlsp(uftn->posid[2]);
                     if( rv != 0)
                     {
                         rv = DEL_FTN_DRV_LSP_ERR;
                         uftn->return_code = rv;
                         goto  EXIT_LABEL;
                     }
                     else
                     {
                         uftn->posid[2] = 0;
                     }
                 }
                 if( uftn->posid[3] != 0)
                 {
                     rv = spm_l3_ftn_del_drv_txlsp(uftn->posid[3]);
                     if( rv != 0)
                     {
                         rv = DEL_FTN_DRV_LSP_ERR;
                         uftn->return_code = rv;
                         goto  EXIT_LABEL;
                     }
                     else
                     {
                         uftn->posid[3] = 0;
                     }
                 }
             }
             if( rv == 0)      /* 删除成功，删除对应的配置*/
             {
                 spm_l3_updatevrf( uftn->key.fec, 0 NBB_CCXT);
                 
                 spm_l3_updatevp( uftn->key.fec, 0 ,0 NBB_CCXT);
                 OS_MEMSET( &bfdldp, 0, sizeof( ATG_DCI_BFD_LDP_LSP_DATA ) );
                 bfdldp.vrf_id = ftnkey.vrfid ;
                 bfdldp.fec_prefix_len = ftnkey.mask;
                 bfdldp.fec_prefix = ftnkey.fec;
                 spm_set_ldp_lsp_state( &bfdldp, 1);
                 
                 //spm_l3_updatepeerecmp( uftn->key.fec, 0 NBB_CCXT);
                 //spm_l3_mpeerfrr( 1, uftn->key.fec NBB_CCXT);
                 AVLL_DELETE( SHARED.ftn_tree , uftn->ftn_node);
                 NBB_MM_FREE(uftn, MEM_SPM_FTN_TAB_NODE_CB);
             }
         }
         else
         {
             /**待删除标志,删lsp时，如果lsp被依赖就置位**/
             uftn->delflag = 2;
         }
     }
    else if( ftnips->delete_struct == 0) /* 增加*/
    {
        if( ftnips->oper_basic == ATG_DCI_OPER_ADD) /* 增加*/
        {
            ftnips->basic_return_code = ATG_DCI_RC_OK;
            if( uftn == NULL )
            {
                if (l3_ftn_cfg_print_setting == SPM_PRINT_CFG)
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "add ftn v.m.p =%d.%d.%x \n", ftnkey.vrfid,ftnkey.mask,ftnkey.fec);
                }
                uftn = (FTN_TAB *)NBB_MM_ALLOC(sizeof(FTN_TAB), NBB_NORETRY_ACT, MEM_SPM_FTN_TAB_NODE_CB);
                if ( uftn == NULL )
                {
                    rv = ADD_FTN_MALLOC_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e add ftn malloc null,rv=%d\n", __LINE__,rv);
                 
                    goto  EXIT_LABEL;
                } 
                NBB_MEMSET(uftn, 0, sizeof(FTN_TAB));
                AVLL_INIT_NODE(uftn->ftn_node);  
                flag = 1;
                NBB_TRC_FLOW((NBB_FORMAT "Add FTN new node  "));
            }
            else
            {
                if (l3_ftn_cfg_print_setting == SPM_PRINT_CFG)
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "upd ftn v.m.p =%d.%d.%x \n", ftnkey.vrfid,ftnkey.mask,ftnkey.fec);
                }
                flag = 2;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(ftnips, &ftnips->basic_data); 
            if (ucdata == NULL)
            {
                rv = ADD_FTN_BASIC_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error: basic data null,rv=%d\n",
                                                                             __LINE__,rv);
                uftn->return_code = rv;
                goto  EXIT_LABEL;
            }
            else      /*成功*/
            {
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(uftn->key), &ftnkey, sizeof( FTN_KEY) );
                
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(uftn->base), ucdata, sizeof( FTN_BASE ) );
                if (flag == 1)
                {
                    //coverity[no_effect_test]
                    AVLL_INSERT(SHARED.ftn_tree, uftn->ftn_node);
                }
                if (flag == 2)
                {
                    if (uftn->base.nexttype != uftn->bntype)
                    {
                        if( uftn->bntype == 1)
                        {
                            rv = spm_l3_ftn_del_aps_entity(uftn);
                        }
                        if( uftn->bntype == 2)
                        {
                            rv = spm_l3_ftn_del_aps_tunnel_ecmp(uftn);
                            uftn->ecmpid = 0;
                        }
                        uftn->bntype = uftn->base.nexttype;
                    }
                }
            }
            rv = 0;
        }   
        else if( ftnips->oper_basic == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
                NBB_TRC_FLOW((NBB_FORMAT "e Del trn: basic data "));
                ftnips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
                goto  EXIT_LABEL;
        }
        if( ftnips->oper_nxhop == ATG_DCI_OPER_UPDATE) /* 增加*/
        {
            if ( uftn == NULL )                                 
            {
                rv = ADD_FTN_NEXTHOP_BASIC_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN nexthop error: ftn null,rv=%d\n",
                                                                                       __LINE__,rv);
                goto   EXIT_LABEL;
            }   
            ftnips->nxhop_return_code = ATG_DCI_RC_OK;
            nxhopnum = ftnips->nxhop_num;
            NBB_TRC_DETAIL((NBB_FORMAT "Add FTN nxhop_num =%d ", nxhopnum));
            if (nxhopnum > 4)
            {
                rv = ADD_FTN_NEXTHOP_NUM_EXCEED;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error: nxhop_num exc 4,rv=%d,nxhop=%d\n",
                                                                             __LINE__,rv,nxhopnum);
                uftn->return_code = rv;
                goto  EXIT_LABEL;
            }   
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(ftnips, &ftnips->nxhop_data);
            if (ucdata == NULL)
            {
                if (nxhopnum == 0)
                {
                    if (flag == 2)
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ftn nexthop null fec=%x \n", 
                                        __LINE__, ftnkey.fec);
                    }
                    rv = 0;
                    goto  EXIT_LABEL;
                }
                rv = ADD_FTN_NEXTHOP_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error: nexthop null,rv=%d\n",
                                                                                       __LINE__,rv);
                uftn->return_code = rv;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &newhop[0], 0, 8 * sizeof( FTN_NHOP ) );
            OS_MEMSET( &oldhop[0], 0, 8 * sizeof( FTN_NHOP ) );

            /*保存原有下一跳配置*/
            NBB_MEMCPY(&oldhop[0], &(uftn->nexthop[0]), 8 * sizeof( FTN_NHOP));
            OS_MEMSET( &(uftn->nexthop[0]), 0, 8 * sizeof( FTN_NHOP ) );
            oldhopnum = uftn->hopnum;
            uftn->hopnum = nxhopnum;
            OS_MEMSET( &(uftn->dclsp[0]), 0, 8 * sizeof( DC_LSP ) );
            for ( i = 0; i < nxhopnum; i++)
            {
                nexthp = (ATG_DCI_FTN_NXHOP_DATA *)(ucdata + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_FTN_NXHOP_DATA))) * i);
                OS_MEMSET( &(newhop[i]), 0, sizeof(ATG_DCI_FTN_NXHOP_DATA));
                
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(newhop[i]), nexthp, sizeof(ATG_DCI_FTN_NXHOP_DATA) );

                /*先保存下一跳配置*/
                NBB_MEMCPY( &(uftn->nexthop[i]), &(newhop[i]), sizeof( FTN_NHOP));
                if ( uftn->key.vrfid == 0 )
                {
                    //rv = spm_l3_checkipdirectconnect( 0 , &(newhop[i].nextip));
                    //rv = spm_l3_checkip( 0, 0, &(newhop[i].nextip) NBB_CCXT);

                    rv = 0;

                    if ( rv == 0 )
                    {
                        uftn->dclsp[i].label2 = 0xffffffff;
                        uftn->dclsp[i].label1 = newhop[i].outlabel;
                        uftn->dclsp[i].nextip =  newhop[i].nextip;
                        uftn->dclsp[i].nextport =  newhop[i].nextport;
                        NBB_TRC_DETAIL((NBB_FORMAT "Add FTN dclabel1 =%d ", uftn->dclsp[i].label1));
                    }
                    else if ( rv == 1 )
                    {
                        rv =    spm_l3_checkporttunnl( newhop[i].nextport, &tunnelkey NBB_CCXT);
                        if ( rv == 0 )
                        {
                            OS_MEMSET( &dclsp, 0, sizeof(DC_LSP));
                            rv = spm_l3_iteration_tunnel( newhop[i].nextport ,&dclsp NBB_CCXT);
                            if ( rv == 0 )
                            {
                                uftn->dclsp[i].label2 = dclsp.label1;
                                uftn->dclsp[i].label1 = newhop[i].outlabel;
                                uftn->dclsp[i].nextip =  dclsp.nextip;
                                uftn->dclsp[i].nextport =  dclsp.nextport;
                            }
                            else
                            {
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error: iter tunnel ,rv=%d\n",
                                                                                           __LINE__,rv);
                                uftn->return_code = rv;
                                goto  EXIT_LABEL;
                            }
                        }
                        else    /*迭代，FTN */
                        {
                            OS_MEMSET( &dclsp, 0, sizeof(DC_LSP));
                            rv = spm_l3_iteration_ftnlsp( newhop[i].nextip ,&dclsp  NBB_CCXT);
                            if ( rv == 0 )
                            {
                                uftn->dclsp[i].label2 = dclsp.label1;
                                uftn->dclsp[i].label1 = newhop[i].outlabel;
                                uftn->dclsp[i].nextip =  dclsp.nextip;
                                uftn->dclsp[i].nextport =  dclsp.nextport;
                            }
                            else
                            {
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error: iter ftnlsp ,rv=%d\n",
                                                                                           __LINE__,rv);
                                uftn->return_code = rv;
                                goto  EXIT_LABEL;
                            }
                        }
                    }
                }
                else
                {
                    rv = ADD_FTN_VRFID_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error: vrfid !=0 ,rv=%d,vrfid=%d\n",
                                                                                 __LINE__,rv,uftn->key.vrfid);
                    uftn->return_code = rv;
                    goto  EXIT_LABEL;
                }   
            }
            
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d upftn peer.ip.ip.ip.ip =%x.%x.%x.%x.%x \n", __LINE__, 
            //                                                                      uftn->key.fec,
            //                                                                      newhop[0].nextip,
            //                                                                      newhop[1].nextip,
            //                                                                      newhop[2].nextip,
            //                                                                      newhop[3].nextip);
            
            if( oldhopnum != 0)  
            {
                NBB_TRC_DETAIL((NBB_FORMAT "UPdate FTN nxhop_num =%d ", uftn->hopnum));
                OS_MEMCPY( &posid[0], &(uftn->posid[0]), 32 );
                OS_MEMCPY( &index[0], &(uftn->index[0]), 32 );
                NBB_MEMSET( &uftn->posid[0], 0, 32 );
                NBB_MEMSET( &uftn->index[0], 0, 32 );
                for ( i = 0; i < 4; i++)
                {
                    flag = 0;
                    for ( j = 0; j < nxhopnum; j++)
                    {
                        if( newhop[j].nextip == oldhop[i].nextip )
                        {
                            flag = 5;
                            break;
                        }
                    }
                    if( flag == 5)  
                    {
                        uftn->posid[j] = posid[i];
                        uftn->index[j] = index[i];
                        
                        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o savlsp peer.nip.pid =%x.%x.%x \n", __LINE__, 
                        //                                                                      uftn->key.fec,
                        //                                                                      uftn->nexthop[i].nextip,
                        //                                                                      posid[i]);
                    }
                    else
                    {
                        if( posid[i] != 0)
                        {
                            rv = spm_l3_ftn_del_drv_txlsp(posid[i]);
                            if( rv != 0)
                            {
                                rv = ADD_FTN_DRV_LSP_ERR;
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delsp peer.nip.pid =%x.%x.%x \n", 
                                                                                             __LINE__, 
                                                                                             uftn->key.fec,
                                                                                             oldhop[i].nextip,
                                                                                             posid[i]);
                                uftn->return_code = rv;
                                
                            }
                            else
                            {
                                //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o delsp peer.nip.pid =%x.%x.%x \n",
                                //                                                          __LINE__, 
                                //                                                          uftn->key.fec,
                                //                                                          uftn->nexthop[i].nextip,
                                //                                                          posid[i]);
                                posid[i] = 0;
                            }
                        }
                    }
                }
            }
            
            if( rv == 0)      /*成功*/
            {
                for ( i = 0; i < nxhopnum; i++)
                {
                    OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                    rv = spm_get_portid_from_logical_port_index(uftn->dclsp[i].nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    {
                        uftn->portinfo[i].slot = portfo.slot_id;
                        uftn->portinfo[i].port_id = portfo.port_id;
                        uftn->portinfo[i].svlan = portfo.svlan;
                    }
                    else
                    {
                        rv = ADD_FTN_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error: get L2 info ,rv=%d,portidx=%d\n",
                                                                    __LINE__,rv,uftn->dclsp[i].nextport);
                        uftn->return_code = rv;
                        goto  EXIT_LABEL;
                    } 
                }


                /*创建txlsp驱动*/
                rv = spm_l3_ftn_add_drv_txlsp(uftn);
                if( rv != 0)
                {
                    rv = ADD_FTN_DRV_LSP_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error:lsp dev ,rv=%d,\n",
                                                                                      __LINE__,rv);
                    uftn->return_code = rv;
                    goto  EXIT_LABEL;
                }

                 /*tunnel 无保护*/
                 if ( uftn->base.nexttype == 0)
                 {
                     rv = spm_l3_ftn_add_drv_tunnel(uftn);
                     if(rv != 0)
                     {
                         rv = ADD_FTN_DRV_TUNNEL_ERR;
                         uftn->return_code = rv;
                         goto  EXIT_LABEL;
                     }
                 }
                else if ( uftn->base.nexttype == 1)
                {
                    for ( i = 0; i < nxhopnum; i++)
                    {
                        if( newhop[i].nexttypeb < 2)
                        {
                            mlsposid = uftn->posid[i];
                            moutport = uftn->dclsp[i].nextport;
                            mnextip = uftn->dclsp[i].nextip;
                        }
                        else if( newhop[i].nexttypeb == 2)
                        {
                            baklsposid = uftn->posid[i];
                            boutport = uftn->dclsp[i].nextport;
                            bnextip = uftn->dclsp[i].nextip;
                        }
                    }   
                    if( mlsposid == 0)
                    {
                        moutport = boutport;
                        mnextip = bnextip;
                    }
                    if( baklsposid == 0)
                    {
                        boutport = moutport;
                        bnextip = mnextip;
                    }
                    rv = spm_l3_ftn_add_drv_tunnel_frr(uftn);
                    if(rv != 0)
                    {
                        rv = ADD_FTN_DRV_TUNNEL_FRR_ERR;
                        uftn->return_code = rv;
                        goto  EXIT_LABEL;                        
                    }
                        
                }
                else if ( uftn->base.nexttype == 2)
                {
                    rv = spm_l3_ftn_add_drv_tunnel_ecmp(uftn);
                    if(rv != 0)
                    {
                        rv = ADD_FTN_DRV_TUNNEL_ECMP_ERR;
                        uftn->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                
                if( rv == 0)     
                {
                    //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o ftn fec.tunid =%x.%d \n", 
                    //                                                          __LINE__, uftn->key.fec,uftn->tunnelid);
                    OS_MEMSET( &bfdldp, 0, sizeof( ATG_DCI_BFD_LDP_LSP_DATA ) );
                    bfdldp.vrf_id = ftnkey.vrfid ;
                    bfdldp.fec_prefix_len = ftnkey.mask;
                    bfdldp.fec_prefix = ftnkey.fec;
                    spm_set_ldp_lsp_state( &bfdldp, 0);
                    
                    spm_l3_updatevrf( uftn->key.fec, uftn->tunnelid NBB_CCXT);
                    
                    //spm_l3_mpeerfrr( 0, uftn->key.fec NBB_CCXT);
                    //spm_l3_updatepeerecmp( uftn->key.fec, 1 NBB_CCXT);
                    if ( uftn->base.nexttype == 1)
                    {
                        port = moutport;
                    }
                    else
                    {
                        port = uftn->dclsp[0].nextport;
                    }
                    spm_l3_updatevp( uftn->key.fec, uftn->tunnelid, port NBB_CCXT);
                    spm_l3_updatemcid( uftn->key.fec, port NBB_CCXT);
                    
                }
                else
                {
                    rv = ADD_FTN_DRV_TUNNEL_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error:tunnel dev ,rv=%d,\n",
                                                                                       __LINE__,rv);
                    uftn->return_code = rv;
                    goto  EXIT_LABEL;
                }

                rv = spm_l3_ftn_add_drv_txpw(uftn);
                if(rv != 0)
                {
                    rv = ADD_FTN_DRV_L3TXPW_ERR;
                    uftn->return_code = rv;
                    goto  EXIT_LABEL;
                }
               
                rv = spm_l3_ftnupdatefib( 1, uftn->pwnhi, uftn->key.mask, uftn->key.fec NBB_CCXT);
                if( rv == 0)   
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "Add FTN update fib nhi =%x ", txpw.nhIdx));
                }
                else
                {
                    //OS_PRINTF("Line=%d  ftnupdatefib err  fec=%x rv=%d\n ",__LINE__, uftn->key.fec ,rv);
                    NBB_TRC_DETAIL((NBB_FORMAT "Line=%d ftnupdatefib rv=%d\n", __LINE__,rv));
                }

            
            }
            else
            {
                rv = ADD_FTN_DRV_DEL_LSP_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add FTN error:del lsp dev ,rv=%d,\n",
                                                                                      __LINE__,rv);
                uftn->return_code = rv;
                goto  EXIT_LABEL;
            }
            
            //coverity[dead_error_condition]
            if( rv == 0)      /*成功*/
            {
                if( uftn->base.nexttype == 1 )
                {              
                    rv = spm_l3_ftn_add_frr_toaps(uftn,moutport,mnextip,boutport,bnextip);
                }
                if( uftn->base.nexttype == 2)
                {
                   
                    rv = spm_l3_ftn_add_ecmp_toaps(uftn);
                }
            }
            else    /*调用驱动失败*/
            {
                //coverity[dead_error_begin]
                NBB_TRC_DETAIL((NBB_FORMAT "Add FTN Error: Route DEV rv=%d ", rv));
                goto  EXIT_LABEL;
            }  
        }
        else if( ftnips->oper_nxhop == ATG_DCI_OPER_DEL) 
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del trn: Nexthop "));
            ftnips->nxhop_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }   
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    /*程序异常时，记录错误条目的key值*/
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d ftn: err vrfid=%d.mask=%d.fec=%x\n", 
                                           __LINE__,ftnkey.vrfid,ftnkey.mask,ftnkey.fec);
    }
    
}

#endif
