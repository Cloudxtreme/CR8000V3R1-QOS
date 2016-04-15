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
VRF_M_ROUTE配置块
********************/
#ifdef VRF_M_ROUTE

/******************************************************************************
 * FunctionName 	: 	spm_l3_mcroutecomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_mcroutecomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    MCROUTE_KEY *key1 = (MCROUTE_KEY *)keyarg1;
    MCROUTE_KEY *key2 = (MCROUTE_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_mcroutecomp");

    rv = NBB_MEMCMP(&(key1->ips[0]), &(key2->ips[0]),16 );
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    rv = NBB_MEMCMP(&(key1->ipg[0]), &(key2->ipg[0]),16 );
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    rv = compare_byte(&key1->addrtype, &key2->addrtype NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    rv = compare_ushort(&key1->vrfid, &key2->vrfid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}

NBB_VOID spm_l3_mcroutetreeinit( NBB_CXT_T NBB_CXT)
{
    AVLL_INIT_TREE(SHARED.mcroute_tree,
                    spm_l3_mcroutecomp,
                    NBB_OFFSETOF(MCROUTE, key),/*lint !e413 */
                    NBB_OFFSETOF(MCROUTE, route_node));/*lint !e413 */
}



/**PROC+****************************************************************************************/
/* Name:      spm_l3_mcrouteprocess                                   */
/*                                                                              */
/* Purpose:  组播配置处理               */
/*                                                                              */
/* Returns:                                             */
/*                                                              */
/*                                                              */
/* Params:  内存释放还需要考虑          */
/*                                                                              */
/* Operation:                                                   */
/*                                                                              */
/**PROC-****************************************************************************************/
NBB_VOID spm_l3_mcrouteprocess( ATG_DCI_SET_VRF_MC  *mcrouteips  NBB_CCXT_T NBB_CXT)
{

#if 1
    NBB_INT             rv = 100;
    NBB_INT             rv2 = 100;

    NBB_INT             cudid = 0;

    NBB_BYTE            *ucdata = NULL;

    NBB_BYTE             flag = 0;

    MCROUTE_KEY     mroutekey;

    MCROUTE_BASE        base;

    MCROUTE_PMSI        pmsi[16];

    MCROUTE         *mroute = NULL;

    MCROUTE_PMSI        *pmsiport = NULL;

    L3_ROUTE_T          l3route;

    IPMC_INTF_T             mcout;

    NBB_BYTE            i;
    NBB_BYTE            j;

    NBB_BYTE            slot[96];
    NBB_BYTE            outslot[32];
    NBB_BYTE            lagslot = 0;

    NBB_USHORT          dcport[32];
    NBB_USHORT          lagport = 0;
    NBB_USHORT          unit[32];

    NBB_ULONG           posid[32];
    NBB_ULONG           outcud[32];
    NBB_ULONG           nextindex[32];
    NBB_ULONG           portindex = 0;
    NBB_ULONG           index = 0;
    NBB_ULONG           nextip = 0;
    
    SPM_PORT_INFO_CB    portfo;

    NBB_CHAR            *errorlog = NULL;
    NBB_ULONG           str_len = 0;

    NBB_TRC_ENTRY("spm_l3_mcrouteprocess");

    
    NBB_ASSERT(mcrouteips != NULL);

    if(NULL == mcrouteips)
    {
        goto EXIT_LABEL;
    }

    mcrouteips->return_code = ATG_DCI_RC_OK;

    OS_MEMSET( &base, 0, sizeof( MCROUTE_BASE) );

    OS_MEMSET( &mroutekey, 0, sizeof( MCROUTE_KEY ) );
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &mroutekey, &(mcrouteips->key), sizeof( MCROUTE_KEY ) );

    if( mroutekey.addrtype == 0 )
    {
        if(( mroutekey.ips[3] == 0 ) && ( mroutekey.ipg[3] == 0 ))
        {
            NBB_PRINTF(" MCROUTE  error key = 0 \n");
            mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto  EXIT_LABEL;
        }
        NBB_TRC_DETAIL((NBB_FORMAT "MCROUTE vrfid=%d  sip =%x  gip=%x\n ",
                                                        mroutekey.vrfid, mroutekey.ips[3] ,mroutekey.ipg[3]));
    }
    else if( mroutekey.addrtype == 1 )
    {
        if(( mroutekey.ipg[3] == 0 ) && ( mroutekey.ipg[2] == 0 ) && 
                                                    ( mroutekey.ipg[1] == 0 ) && ( mroutekey.ipg[0] == 0 ))
        {
            NBB_PRINTF(" MCROUTE  error key = 0 \n");
            mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto  EXIT_LABEL;
        }
        
    }
    OS_MEMSET(&unit[0] , 0, 64);
    OS_MEMSET(&slot[0] , 0, 96);
    OS_MEMSET(&outslot[0] , 0, 32);
    OS_MEMSET(&posid[0] , 0, 128);
    OS_MEMSET(&outcud[0] , 0, 128);
    OS_MEMSET(&nextindex[0] , 0, 128);
    
    /*操作之前，查看本地数据是否存在*/
    mroute =  (MCROUTE *)AVLL_FIND( SHARED.mcroute_tree ,  &mroutekey ); 
    if( mcrouteips->delete_struct == 1) /*  删除*/
    {
        if( mroute == NULL )
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del mcroute: data null"));
            mcrouteips->return_code = ATG_DCI_RC_OK;
            rv = 0;
            goto  EXIT_LABEL;
        }
        
        #ifdef SPU
            for ( i = 0; i < mroute->base.portnumm; i++)
            {
                if( mroute->posid[i] != 0 ) 
                {
                    if( mroute->posid[i] > 16384 ) //16K
                    {
                        rv = ApiC3DelL3MCOutIntf( BCMC31,  mroute->posid[i]);
                        if ( SHARED.c3_num == 2 )
                        {
                            rv2 = ApiC3DelL3MCOutIntf( BCMC32,  mroute->posid[i]);
                            rv = (rv|rv2);
                        }
                        if( rv != 0)
                        {
                            rv = DEL_MCROUTE_DRV_MCOUTIF_ERR;
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute: drv mcoutif ,rv=%d.\n",
                                                                               __LINE__,rv);
                            mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto  EXIT_LABEL;
                            
                        }
                        else
                        {
                            mroute->posid[i] = 0;
                        }
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute deloutintf posid =%d \n", 
                                                    __LINE__,mroute->posid[i]);
                    }
                }
                if( mroute->outcud[i] != 0 ) 
                {
                    rv = spm_free_vpls_mc_cud( mroute->outcud[i] NBB_CCXT);
                    if( rv != 0)
                    {
                        rv = DEL_MCROUTE_FREE_CUD_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute: free cud ,rv=%d.\n",
                                                                               __LINE__,rv);
                        mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto  EXIT_LABEL;
                    }
                    else
                    {
                        mroute->outcud[i] = 0;
                    }
                }
                if( mroute->nhi[i] != 0 ) 
                {
                    if( i < mroute->base.portnumn ) 
                    {
                        rv = spm_l3_deluni( 0, &nextip,mroute->base.outport[i] NBB_CCXT);
                        if( rv != 0)
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute: del uni ,rv=%d.\n",
                                                                               __LINE__,rv);
                            mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto  EXIT_LABEL;
                        }
                        else
                        {
                            mroute->nhi[i] = 0;
                        }
                    }
                    else
                    {
                        j = i - mroute->base.portnumn;
                        rv = spm_l3_delpwv4( mroute->key.vrfid, 
                                                            mroute->pmsi[j].downip[3], 
                                                            mroute->pmsi[j].outlabel NBB_CCXT);
                        if( rv != 0)
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute: del pw ,rv=%d.\n",
                                                                               __LINE__,rv);
                            mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto  EXIT_LABEL;
                        }
                        else
                        {
                            mroute->nhi[i] = 0;
                        }
                    }
                }
            }
            if( mroute->base.mcid != 0 ) 
            {
                rv = ApiAradDeleteEngressMcAllPort( 0, mroute->base.mcid);
                rv2 = ApiAradDeleteMc( 0, mroute->base.mcid);
                rv = (rv|rv2);
                if( rv != 0)
                {
                    rv = DEL_MCROUTE_DRV_MC_PORT_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute: del mc arad ,rv=%d.\n",
                                                                               __LINE__,rv);
                    mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto  EXIT_LABEL;
                }
                else
                {
                    mroute->base.mcid = 0;
                }
            }
            ApiC3InitL3RouteStruct( &l3route );
            l3route.vrfId = mroutekey.vrfid;//暂时

                OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                rv = spm_get_portid_from_logical_port_index(mroute->base.inport, &portfo NBB_CCXT);
                if( rv == 0) 
                {
                    l3route.mLogicPort = portfo.port_id;
                }
                else
                {
                    rv = DEL_MCROUTE_GET_L2PORT_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute:get l2port ,rv=%d.\n",
                                                                               __LINE__,rv);
                    mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto  EXIT_LABEL;
                }
            
            if( mroutekey.addrtype == 0 )
            {
                l3route.mIpPktType = IPV4_PKT_MULTICAST;
                l3route.mDstIpv4 = mroutekey.ipg[3];
                l3route.mSrcIpv4 = mroutekey.ips[3];
            }
            else
            {
                l3route.mIpPktType = IPV6_PKT_MULTICAST;
                NBB_MEMCPY( &(l3route.mDstIpv6), &(mroutekey.ipg[0]), 16 );
                NBB_MEMCPY( &(l3route.mSrcIpv6), &(mroutekey.ips[0]), 16 );
            }
            rv = ApiC3DelL3MCRoute( BCMC31, &l3route);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3DelL3MCRoute( BCMC32, &l3route);
                rv = (rv|rv2);
            }
            if( rv != 0)
            {
                rv = DEL_MCROUTE_DRV_MCROUTE_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute:del drv mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
        #endif
        
        #ifdef SRC
            for ( i = 0; i < 16; i++)
            {
                if( mroute->outslot[i] != 0) 
                {
                    rv = ApiDfeMulticastDelete( 0, mroute->base.mcid, mroute->outslot[i]);
                    if( rv != 0)
                    {
                        rv = DEL_MCROUTE_DRV_MCROUTE_DFE_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute:del drv mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                        mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto  EXIT_LABEL;
                    }
                    else
                    {
                        mroute->outslot[i] = 0;
                    }
                }
            }
            if( mroute->base.mcid != 0) 
            {
                rv = ApiDfeMulticastDestroy( 0, mroute->base.mcid);
                if( rv != 0)
                {
                    rv = DEL_MCROUTE_DRV_MCROUTE_DFE_DST_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute:destroy mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                    mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto  EXIT_LABEL;
                }
                else
                {
                    mroute->base.mcid = 0;
                }
            }
        #endif
        #ifdef SNU
            for ( i = 0; i < 16; i++)
            {
                if( mroute->outslot[i] != 0) 
                {
                    rv = ApiDfeMulticastDelete( 0, mroute->base.mcid, mroute->outslot[i]);
                    if( rv != 0)
                    {
                        rv = DEL_MCROUTE_DRV_MCROUTE_DFE_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute:del drv mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                        mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto  EXIT_LABEL;
                    }
                    else
                    {
                        mroute->outslot[i] = 0;
                    }
                }
            }
            if( mroute->base.mcid != 0) 
            {
                rv = ApiDfeMulticastDestroy( 0, mroute->base.mcid);
                if( rv != 0)
                {
                    rv = DEL_MCROUTE_DRV_MCROUTE_DFE_DST_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del mcroute:destroy mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                    mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto  EXIT_LABEL;
                }
                else
                {
                    mroute->base.mcid = 0;
                }
            }
        #endif
        
        rv = 0;
        if( rv == 0)      /* 删除成功，删除对应的配置*/
        {
            AVLL_DELETE( SHARED.mcroute_tree , mroute->route_node);
            NBB_MM_FREE(mroute, MEM_SPM_VRFMROUTE_NODE_CB);
        }
        #if 0
        else    /*不成功，不删除对应的数据，同时记录错误*/
        {
            NBB_TRC_DETAIL((NBB_FORMAT "e Del mcroute: DEV  rv=%d ", rv));
            mcrouteips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            NBB_SPRINTF(errorlog + str_len, "Line=%d Del MCROUTE rv=%d\n", __LINE__,rv);
            str_len =  OS_STRLEN(errorlog);
            NBB_SPRINTF(errorlog + str_len, "Del Error: del mc\n");
            str_len =  OS_STRLEN(errorlog);
            goto  EXIT_LABEL;
        }
        #endif
    }
    else if( mcrouteips->delete_struct == 0) /* 增加*/
    {
        if( mroute == NULL )
        {
            flag = 1;
        }
        else
        {
            flag = 2;
        }
        if( mcrouteips->oper_basic == ATG_DCI_OPER_ADD) /* 增加*/
        {
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(mcrouteips, &mcrouteips->basic_data); 
            if (ucdata == NULL)
            {
                rv = ADD_MCROUTE_IPS_BASIC_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:mc basic ips null ,rv=%d.\n",
                                                                               __LINE__,rv);
                mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &base, 0, sizeof( MCROUTE_BASE) );
            NBB_MEMCPY( &base, ucdata, sizeof( MCROUTE_BASE) );
            if(flag == 2)
            {
                rv = NBB_MEMCMP( &base, &(mroute->base), sizeof( MCROUTE_BASE) );/*lint !e613 */
                if (rv == 0)
                {
                    //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o mroute base  same mcid =%d \n", __LINE__,base.mcid);
                    goto EXIT_LABEL;
                }
            }
                for ( i = 0; i < base.portnumn; i++)
                {
                    OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                    rv = spm_get_portid_from_logical_port_index(base.outport[i], &portfo NBB_CCXT);
                    if( rv == 0) 
                    {
                        unit[i] = portfo.unit_id;
                        dcport[i] = portfo.port_id;
                        outslot[i] = portfo.slot_id ;
                        if (portfo.port_type == ATG_DCI_LAG)
                        {
                            rv = spm_get_lag_slot_port(portfo.port_id - ATG_DCI_LAG_OFFSET, 
                                                                                    &lagslot, &lagport NBB_CCXT);
                            if( rv == 0 )
                            {
                                outslot[i] = lagslot;
                            }
                            else
                            {
                                rv = ADD_MCROUTE_L2_LAG_DATA_ERR;
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute: mroute getlag port.rv =%d.%d \n", 
                                                            __LINE__,portfo.port_id, rv);
                                mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto  EXIT_LABEL;
                            }
                        }
                        rv = spm_l3_getuni( 0, &nextip, base.outport[i], &index);
                        if( rv == 0 )
                        {
                            nextindex[i] = index;
                            rv = spm_alloc_vpls_mc_cud( &cudid NBB_CCXT );
                            if( rv == 0)
                            {
                                outcud[i] = cudid;
                                if (portfo.port_type == ATG_DCI_LAG)
                                {
                                    if ( slot[lagslot] == 0) 
                                    {
                                        slot[lagslot] = 1;
                                    }
                                }
                                else
                                {
                                    if ( slot[portfo.slot_id] == 0) 
                                    {
                                        slot[portfo.slot_id] = 1;
                                    }
                                }
                            }
                            else
                            {
                                rv = ADD_MCROUTE_ALLOC_CUD_ERR;
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:alloc cud ,rv=%d.\n",
                                                                               __LINE__,rv);
                                mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto  EXIT_LABEL;
                            }
                        }
                        else
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:get l3uni ,rv=%d.\n",
                                                                               __LINE__,rv);
                            mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto  EXIT_LABEL;
                        }

                    }
                    else
                    {
                        rv = ADD_MCROUTE_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:get l2port ,rv=%d.\n",
                                                                               __LINE__,rv);
                        mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto  EXIT_LABEL;
                    }
                }
            
        }   
        else if( mcrouteips->oper_basic == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del mcroute: basic data "));
            mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
        if( mcrouteips->oper_out_pmsi == ATG_DCI_OPER_ADD) /* 增加*/
        {
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(mcrouteips, &mcrouteips->out_pmsi_data); 
            if (ucdata == NULL)
            {
                rv = ADD_MCROUTE_IPS_PMSI_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:pmsi ips null ,rv=%d.\n",
                                                                               __LINE__,rv);
                mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
            
            if( base.portnumm > base.portnumn) 
            {
                for ( i = base.portnumn; i < base.portnumm; i++)
                {
                    j = i - base.portnumn;
                    pmsiport = (MCROUTE_PMSI *)(ucdata + (NBB_ALIGN_OFFSET(sizeof(MCROUTE_PMSI))) * j);
                    OS_MEMSET( &pmsi[j], 0, sizeof( MCROUTE_PMSI) );
                    
                    //coverity[bad_sizeof]
                    NBB_MEMCPY( &pmsi[j], pmsiport, sizeof(MCROUTE_PMSI));
                    
                    rv = spm_l3_peerforport(mroutekey.vrfid, pmsi[j].downip[3],&portindex NBB_CCXT);
                    if( rv == 0) 
                    {
                        OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                        rv = spm_get_portid_from_logical_port_index(portindex, &portfo NBB_CCXT);
                        if( rv == 0) 
                        {   
                            unit[i] = portfo.unit_id;
                            dcport[i] = portfo.port_id;
                            outslot[i] = portfo.slot_id ;
                            rv = spm_l3_getpwv4( mroutekey.vrfid, pmsi[j].downip[3], pmsi[j].outlabel,&index NBB_CCXT); 
                            if( rv == 0)
                            {
                                nextindex[i] = index;
                                rv = spm_alloc_vpls_mc_cud( &cudid NBB_CCXT);
                                if( rv == 0)
                                {
                                    outcud[i] = cudid;
                                    if ( slot[portfo.slot_id] == 0) 
                                    {
                                        slot[portfo.slot_id] = 1;
                                    }
                                }
                                else
                                {
                                    rv = ADD_MCROUTE_ALLOC_CUD_ERR;
                                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:alloc cud ,rv=%d.\n",
                                                                               __LINE__,rv);
                                    mcrouteips->out_pmsi_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                    goto  EXIT_LABEL;
                                }
                            }
                            else
                            {
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:get pw ,rv=%d.\n",
                                                                               __LINE__,rv);
                                mcrouteips->out_pmsi_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                                goto  EXIT_LABEL;
                            }   
                        }
                        else
                        {
                            rv = ADD_MCROUTE_GET_L2PORT_ERR;
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:get l2port ,rv=%d.\n",
                                                                               __LINE__,rv);
                            mcrouteips->out_pmsi_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                            goto  EXIT_LABEL;
                        }
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:peerport ,rv=%d.\n",
                                                                               __LINE__,rv);
                        mcrouteips->out_pmsi_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                        goto  EXIT_LABEL;
                    }
                }
            }
            else
            {
                rv = ADD_MCROUTE_PORT_NUM_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:portnum err,rv=%d.\n",
                                                                               __LINE__,rv);
                mcrouteips->out_pmsi_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
        }   
        else if( mcrouteips->oper_out_pmsi == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del mcroute: pmsi data "));
            mcrouteips->out_pmsi_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
    #ifdef SPU

        if( flag == 2 )
        {
            if( base.inport != mroute->base.inport)
            {
                ApiC3InitL3RouteStruct( &l3route );
                l3route.vrfId = mroute->key.vrfid;

                OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                rv = spm_get_portid_from_logical_port_index(mroute->base.inport, &portfo NBB_CCXT);
                if( rv == 0) 
                {
                    l3route.mLogicPort = portfo.port_id;
                }
                else
                {
                    rv = ADD_MCROUTE_GET_L2PORT_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:get l2port ,rv=%d.\n",
                                                                               __LINE__,rv);
                }
                if( mroutekey.addrtype == 0 )
                {
                    l3route.mIpPktType = IPV4_PKT_MULTICAST;
                    l3route.mDstIpv4 = mroutekey.ipg[3];
                    l3route.mSrcIpv4 = mroutekey.ips[3];
                }
                else
                {
                    l3route.mIpPktType = IPV6_PKT_MULTICAST;
                    NBB_MEMCPY( &(l3route.mDstIpv6), &(mroutekey.ipg[0]), 16 );
                    NBB_MEMCPY( &(l3route.mSrcIpv6), &(mroutekey.ips[0]), 16 );
                }
                rv = ApiC3DelL3MCRoute( BCMC31, &l3route);
                if ( SHARED.c3_num == 2 )
                {
                    rv2 = ApiC3DelL3MCRoute( BCMC32, &l3route);
                    rv = (rv|rv2);
                }
                if( rv != 0)
                {   
                    rv = ADD_MCROUTE_DEL_DRV_MCROUTE_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:mc drv ,rv=%d.\n",
                                                                               __LINE__,rv);
                    
                }
            }
        }
        ApiC3InitL3RouteStruct( &l3route );
        OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
        rv = spm_get_portid_from_logical_port_index(base.inport, &portfo NBB_CCXT);
        if( rv == 0) 
        {
            l3route.mLogicPort = portfo.port_id;
        }
        else
        {
            rv = ADD_MCROUTE_GET_L2PORT_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:get l2port ,rv=%d.\n",
                                                                               __LINE__,rv);
            mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto  EXIT_LABEL;

        }   
        
        l3route.vrfId = mroutekey.vrfid;
        if( mroutekey.addrtype == 0 )
        {
            l3route.mIpPktType = IPV4_PKT_MULTICAST;
            l3route.mDstIpv4 = mroutekey.ipg[3];
            l3route.mSrcIpv4 = mroutekey.ips[3];
        }
        else
        {
            l3route.mIpPktType = IPV6_PKT_MULTICAST;
            NBB_MEMCPY( &(l3route.mDstIpv6), &(mroutekey.ipg[0]), 16 );
            NBB_MEMCPY( &(l3route.mSrcIpv6), &(mroutekey.ips[0]), 16 );
        }
        l3route.ipmcIdx = base.mcid;

        rv = ApiC3SetL3MCRoute( BCMC31, &l3route);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3SetL3MCRoute( BCMC32, &l3route);
            rv = (rv|rv2);
        }
        if( rv != 0)
        {
            rv = ADD_MCROUTE_SET_DRV_MCROUTE_ERR;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:set drv mc ,rv=%d.\n",
                                                                               __LINE__,rv);
            mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto  EXIT_LABEL;
        }
        
        if( flag == 1 )
        {
            rv = ApiAradSetupEngressMc( 0, base.mcid);
            if( rv != 0)
            {
                rv = ADD_MCROUTE_SET_DRV_ENGRESS_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:set drv arad mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
        }
        else
        {
            for ( i = 0; i < mroute->base.portnumm; i++)
            {
                if( mroute->posid[i] != 0 ) 
                {
                    if( mroute->posid[i] > 16384 ) //16K
                    {
                        rv = ApiC3DelL3MCOutIntf( BCMC31,  mroute->posid[i]);
                        if ( SHARED.c3_num == 2 )
                        {
                            rv2 = ApiC3DelL3MCOutIntf( BCMC32,  mroute->posid[i]);
                            rv = (rv|rv2);
                        }
                        if( rv != 0)
                        {
                            rv = ADD_MCROUTE_DEL_DRV_MCOUTIF_ERR;
                            
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute deloutintf posid.rv =%d.%d \n", 
                                                        __LINE__,mroute->posid[i], rv);
                        }
                        else
                        {
                            mroute->posid[i] = 0;
                        }
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute deloutintf posid =%d \n", 
                                                    __LINE__,mroute->posid[i]);
                    }
                }
                if( mroute->outcud[i] != 0 ) 
                {
                    rv = spm_free_vpls_mc_cud( mroute->outcud[i] NBB_CCXT);
                    if( rv != 0)
                    {
                        rv = ADD_MCROUTE_FREE_CUD_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute delcud cud =%d \n", 
                                                    __LINE__,mroute->outcud[i]);
                    }
                    else
                    {
                        mroute->outcud[i] = 0;
                    }
                }
                if( i < mroute->base.portnumn ) 
                {
                    rv = spm_l3_deluni( 0, &nextip,mroute->base.outport[i] NBB_CCXT);
                    if( rv != 0)
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute deluni rv =%d \n", __LINE__,rv);
                    }
                    else
                    {
                        mroute->nhi[i] = 0;
                    }
                }
                else
                {
                    j = i - mroute->base.portnumn;
                    rv = spm_l3_delpwv4( mroute->key.vrfid, 
                                                        mroute->pmsi[j].downip[3], 
                                                        mroute->pmsi[j].outlabel NBB_CCXT);
                    if( rv != 0)
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute delnni rv =%d \n", __LINE__,rv);
                    }
                    else
                    {
                        mroute->nhi[i] = 0;
                    }
                }
            }
            if( mroute->base.mcid != 0 ) 
            {
                rv = ApiAradDeleteEngressMcAllPort( 0, mroute->base.mcid);
                if( rv != 0)
                {
                    rv = ADD_MCROUTE_DEL_DRV_ENGRESS_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute delmcid rv =%d \n", __LINE__,rv);
                }
                else
                {
                    mroute->base.mcid = 0;
                }
            }
        }
        for ( i = 0; i < base.portnumm; i++)
        {
            if( SHARED.local_slot_id == outslot[i])
            {
                if( unit[i] == 0)
                {
                    rv = ApiAradAddEngressMcPort( 0, base.mcid, SPM_MULTICAST_PORT_0, outcud[i]);
                }
                else
                {
                    rv = ApiAradAddEngressMcPort( 0, base.mcid, SPM_MULTICAST_PORT_1, outcud[i]);
                }
                if( rv != 0)
                {
                    rv = ADD_MCROUTE_DRV_ENGRESS_MCPORT_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute addmcport mcid.cud =%d.%d \n", 
                                                __LINE__,base.mcid, outcud[i]);
                }
            }
            OS_MEMSET(&mcout , 0, sizeof(IPMC_INTF_T));
            mcout.mcId = base.mcid;
            mcout.seq = outcud[i];
            mcout.l3_main_nhi = nextindex[i];
            rv = ApiC3SetL3MCOutIntf( BCMC31,  &mcout);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3SetL3MCOutIntf( BCMC32,  &mcout);
                rv = (rv|rv2);
            }
            if( rv != 0)
            {
                rv = ADD_MCROUTE_SET_DRV_MCOUTIF_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute addoutintf nhi.rv =%d.%d \n", 
                                            __LINE__,nextindex[i], rv);
            }
            else
            {
                posid[i] = mcout.posId;
            }
        }

    #endif

    #ifdef SRC
        if( flag == 1 )
        {
            rv = ApiDfeMulticastCreate( 0, base.mcid);
            if( rv != 0)
            {
                rv = ADD_MCROUTE_CREATE_DRV_DFEMC_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:set drv dfe mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
        }
        else
        {
            for ( i = 0; i < 32; i++)
            {
                if( mroute->outslot[i] != 0) 
                {
                    rv = ApiDfeMulticastDelete( 0, mroute->base.mcid, mroute->outslot[i]);
                    if( rv != 0)
                    {
                        rv = ADD_MCROUTE_DEL_DRV_DFEMC_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:del drv dfe mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                        OS_PRINTF("Line=%d error fe1600del ouslot id=%d rv=%d ",__LINE__, i ,rv);
                    }
                    else
                    {
                        mroute->outslot[i] = 0;
                    }
                }
                if( mroute->outcud[i] != 0 ) 
                {
                    rv = spm_free_vpls_mc_cud( mroute->outcud[i] NBB_CCXT);
                    if( rv != 0)
                    {
                        rv = ADD_MCROUTE_FREE_CUD_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute delcud cud =%d \n", 
                                                    __LINE__,mroute->outcud[i]);
                        OS_PRINTF("Line=%d error free cudid rv=%d ", __LINE__,rv);
                    }
                    else
                    {
                        mroute->outcud[i] = 0;
                    }
                }
            }
        }
        for ( i = 0; i < base.portnumm; i++)
        {
            if ( slot[outslot[i]] == 1) 
            {
                rv = ApiDfeMulticastAdd( 0, base.mcid, outslot[i]);
                if( rv != 0)
                {
                    rv = ADD_MCROUTE_DRV_DFEMC_PORT_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:add drv dfe mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                    OS_PRINTF("Line=%d error fe1600add ouslot id=%d rv=%d ",__LINE__,i ,rv);
                }
                else
                {
                    slot[outslot[i]] = 0;
                }
            }
        }
    #endif

    #ifdef SNU
        if( flag == 1 )
        {
            rv = ApiDfeMulticastCreate( 0, base.mcid);
            if( rv != 0)
            {
                rv = ADD_MCROUTE_CREATE_DRV_DFEMC_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:create drv dfe mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
        }
        else
        {
            for ( i = 0; i < 32; i++)
            {
                if( mroute->outslot[i] != 0) 
                {
                    rv = ApiDfeMulticastDelete( 0, mroute->base.mcid, mroute->outslot[i]);
                    if( rv != 0)
                    {
                        rv = ADD_MCROUTE_DEL_DRV_DFEMC_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:del drv dfe mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                        OS_PRINTF("Line=%d error fe1600del ouslot id=%d rv=%d ",__LINE__, i ,rv);
                    }
                    else
                    {
                        mroute->outslot[i] = 0;
                    }
                }
                if( mroute->outcud[i] != 0 ) 
                {
                    rv = spm_free_vpls_mc_cud( mroute->outcud[i] NBB_CCXT);
                    if( rv != 0)
                    {
                        rv = ADD_MCROUTE_FREE_CUD_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mroute delcud cud =%d \n", 
                                                    __LINE__,mroute->outcud[i]);
                        OS_PRINTF("Line=%d error free cudid rv=%d ", __LINE__,rv);
                    }
                    else
                    {
                        mroute->outcud[i] = 0;
                    }
                }
            }
        }
        for ( i = 0; i < base.portnumm; i++)
        {
            if ( slot[outslot[i]] == 1) 
            {
                rv = ApiDfeMulticastAdd( 0, base.mcid, outslot[i]);
                if( rv != 0)
                {
                    rv = ADD_MCROUTE_DRV_DFEMC_PORT_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:add drv dfe mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                    OS_PRINTF("Line=%d error fe1600add ouslot id=%d rv=%d ",__LINE__,i ,rv);
                }
                else
                {
                    slot[outslot[i]] = 0;
                }
            }
        }
    #endif

        if( rv == 0)      /*成功*/
        {
            if( flag == 1 )
            {
                mroute = (MCROUTE *)NBB_MM_ALLOC(sizeof(MCROUTE), NBB_NORETRY_ACT, MEM_SPM_VRFMROUTE_NODE_CB);
                if ( mroute == NULL )
                {
                    rv = ADD_MCROUTE_MALLOC_MC_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute:malloc mc ,rv=%d.\n",
                                                                               __LINE__,rv);
                    mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                    goto  EXIT_LABEL;
                } 
                NBB_MEMSET(mroute, 0, sizeof(MCROUTE));
                AVLL_INIT_NODE(mroute->route_node);  
                NBB_TRC_FLOW((NBB_FORMAT "Add MCROUTE new node  "));
                
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(mroute->key), &mroutekey, sizeof( MCROUTE_KEY) );
                
                //coverity[no_effect_test]
                AVLL_INSERT(SHARED.mcroute_tree, mroute->route_node);
            }
            if ( mroute == NULL )
            {
                rv = ADD_MCROUTE_MC_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add mcroute: mc null ,rv=%d.\n",
                                                                               __LINE__,rv);
                mcrouteips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            } 
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(mroute->base), &base, sizeof( MCROUTE_BASE ) );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(mroute->pmsi[0]), &pmsi[0], 16 * sizeof( MCROUTE_PMSI ) );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(mroute->nhi[0]), &nextindex[0], 128 );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(mroute->outcud[0]), &outcud[0], 128 );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(mroute->posid[0]), &posid[0], 128 );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(mroute->outslot[0]), &outslot[0], 32 );
        }
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Add mcroute"));
        }

    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    /*程序异常时，记录错误条目的key值*/
    if(rv != 0)
    {
        if(mroutekey.addrtype == 0)
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d mcroute:err vrfid=%d.sip=%x.dip=%x\n",__LINE__, mroutekey.vrfid,
                                            mroutekey.ips[3],mroutekey.ipg[3]);
        }
        else if(mroutekey.addrtype == 1)
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d mcroute:err vrfid=%d.sip=%x.%x.%x.%x.dip=%x.%x.%x.%x\n", __LINE__,
            mroutekey.vrfid, mroutekey.ips[0], mroutekey.ips[1], mroutekey.ips[2], mroutekey.ips[3],mroutekey.ipg[0],
            mroutekey.ipg[1],mroutekey.ipg[2],mroutekey.ipg[3]);
        }
    }

#endif
}

#endif

