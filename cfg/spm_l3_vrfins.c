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
#ifdef VRF_VRF_INSTANCE

/******************************************************************************
 * FunctionName 	: 	spm_l3_vrfinscomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_vrfinscomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    VRFINS_KEY *key1 = (VRFINS_KEY *)keyarg1;
    VRFINS_KEY *key2 = (VRFINS_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_mcroutecomp");


    rv = compare_ushort(&key1->vrfid, &key2->vrfid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    rv = compare_ulong(&key1->peerip, &key2->peerip NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    
    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}

NBB_VOID spm_l3_vrfinstreeinit( NBB_CXT_T NBB_CXT)
{
    AVLL_INIT_TREE(SHARED.vrfins_tree,
                    spm_l3_vrfinscomp,
                    NBB_OFFSETOF(VRFINS, key),/*lint !e413 */
                    NBB_OFFSETOF(VRFINS, ins_node));/*lint !e413 */
}



/**PROC+****************************************************************************************/
/* Name:      spm_l3_vrfinsprocess                                   */
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
NBB_VOID spm_l3_vrfinsprocess( ATG_DCI_SET_VRF_INSTANCE  *vrfinsips  NBB_CCXT_T NBB_CXT)
{
    NBB_INT             rv = 100;
    NBB_INT             rv2 = 100;

    NBB_INT             cudid = 0;

    NBB_ULONG           nhid = 0;
    
    NBB_ULONG           portindex = 0;
            
    NBB_BYTE            *ucdata = NULL;
    NBB_BYTE            flag = 0;
    
    NBB_BYTE            pwflag = 0 ;
        
    VRFINS_KEY          vrfinskey;

    VRFINS_DIFSER       difs;

    VRFINS_UPQOS        upqos;

    ATG_DCI_VRF_INSTANCE_UP_VPN_QOS vrfupqos;

    VRFINS              *vrfins = NULL;

    SHASH_NODE          *node = NULL;
    
    TUNNELCH            *tunnels = NULL;

    PWTXNODE            *pwnode = NULL;

    TUNNELCH_KEY        tunskey;

    CRTXLSP_KEY     txlspkey;

    FTN_KEY             ftnkey;

    FTN_TAB             *uftn = NULL;

    CRTXLSP         *txlsp = NULL;
    
    LSPPROT_KEY     tunlkey;
        
    LSPPROT         *lspprot = NULL;

    SPM_PORT_INFO_CB    portfo;
    SPM_QOS_VRF_INSTANSE_KEY    qosvrfinskey;
    QOS_TUNNEL_KEY          qostunl;

    NBB_CHAR            *errorlog = NULL;
    NBB_ULONG           str_len = 0;

    NBB_TRC_ENTRY("spm_l3_vrfinsprocess");

   
    NBB_ASSERT(vrfinsips != NULL);

    vrfinsips->return_code = ATG_DCI_RC_OK;

    vrfinsips->diff_serv_return_code = ATG_DCI_RC_OK;

    vrfinsips->up_vpn_qos_return_code = ATG_DCI_RC_OK;
    
    OS_MEMSET( &txlspkey, 0, sizeof( CRTXLSP_KEY ) );

    OS_MEMSET( &qostunl, 0, sizeof( QOS_TUNNEL_KEY ) );
    
    OS_MEMSET( &vrfinskey, 0, sizeof( VRFINS_KEY ) );
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &vrfinskey, &(vrfinsips->key), sizeof( VRFINS_KEY ) );

    vrfinskey.res = 0;
    if(( vrfinskey.vrfid == 0 ) && ( vrfinskey.peerip == 0 ))
    {
        NBB_PRINTF(" VRFINS  error key = 0 \n");
        vrfinsips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        goto  EXIT_LABEL;
    }
    NBB_TRC_DETAIL((NBB_FORMAT "VRFINS vrfid=%d  peer =%x ",vrfinskey.vrfid, vrfinskey.peerip));
      
    /*操作之前，查看本地数据是否存在*/
    vrfins =  (VRFINS *)AVLL_FIND( SHARED.vrfins_tree ,  &vrfinskey ); 
    if( vrfinsips->delete_struct == 1) /*  删除*/
    {
        if( vrfins == NULL )
        {
            vrfinsips->return_code = ATG_DCI_RC_OK;
            rv = 0;
            goto  EXIT_LABEL;
        }
        
        #ifdef SPU
        
        OS_MEMSET( &qosvrfinskey, 0, sizeof( SPM_QOS_VRF_INSTANSE_KEY) );
        qosvrfinskey.vrf_id = vrfinskey.vrfid;
        qosvrfinskey.peer_ip = vrfinskey.peerip;
        if( vrfins->difs.dserid != 0 )
        {
           // spm_ds_add_vrf_node( 0, &qosvrfinskey, 0 NBB_CCXT);
            vrfins->difs.dserid = 0;
        }
        if( vrfins->upqos.nodeindex != 0 )
        {
            rv = spm_l3_vrfins_del_hqos_drv(vrfins,0);
            if(rv != 0)
            {
                rv = DEL_VRFINS_VPN_HQOS_ERR;
                vrfins->return_code = rv;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del vrfins: hqos vrf ,rv=%d.\n",
                                                                               __LINE__,rv);
            }
            else
            {
                vrfins->upqos.nodeindex = 0;
            }
        }
        #else
           rv = 0;
        #endif

        /*成功时，删除配置*/
        if(rv == 0)
        {
            AVLL_DELETE( SHARED.vrfins_tree , vrfins->ins_node);
            NBB_MM_FREE(vrfins, MEM_SPM_VRFINS_CB);
        }
    }
    else if( vrfinsips->delete_struct == 0) /* 增加*/
    {
        if( vrfins == NULL )
        {
            flag = 1;
            vrfins = (VRFINS *)NBB_MM_ALLOC(sizeof(VRFINS), NBB_NORETRY_ACT, MEM_SPM_VRFINS_CB);
            if ( vrfins == NULL )
            {
                rv = ADD_VRFINS_MALLOC_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: malloc vrfins null ,rv=%d.\n",
                                                                       __LINE__,rv);
                goto  EXIT_LABEL;
            } 
            NBB_MEMSET(vrfins, 0, sizeof(VRFINS));
            AVLL_INIT_NODE(vrfins->ins_node);  
            NBB_TRC_FLOW((NBB_FORMAT "Add VRFINS new node  "));
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(vrfins->key), &vrfinskey, sizeof( VRFINS_KEY) );
            AVLL_INSERT(SHARED.vrfins_tree, vrfins->ins_node);
        }
        else
        {
            flag = 2;
        }
        
        if( vrfinsips->oper_diff_serv == ATG_DCI_OPER_ADD) /* 增加*/
        {
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(vrfinsips, &vrfinsips->diff_serv_data); 
            if (ucdata == NULL)
            {
                rv = ADD_VRFINS_DS_IPS_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: diff_serv_data null ,rv=%d.\n",
                                                                               __LINE__,rv);
               
                goto  EXIT_LABEL;
            }

            vrfinsips->return_code = ATG_DCI_RC_OK;
            vrfinsips->diff_serv_return_code = ATG_DCI_RC_OK;
            OS_MEMSET( &difs, 0, sizeof( VRFINS_DIFSER) );
            NBB_MEMCPY( &difs, ucdata, sizeof( VRFINS_DIFSER) );
            OS_MEMSET( &qosvrfinskey, 0, sizeof( SPM_QOS_VRF_INSTANSE_KEY) );
            qosvrfinskey.vrf_id = vrfinskey.vrfid;
            qosvrfinskey.peer_ip = vrfinskey.peerip;

            //删除qos
            //if (difs.dserid == 0)
            //{
            //  spm_ds_add_vrf_node( 0 , &qosvrfinskey, 0 NBB_CCXT);
            //  rv = 0;
            //}
            //else
            //{
                tunskey.res = 0;
                tunskey.vrfid = vrfinskey.vrfid;
                tunskey.peerip = vrfinskey.peerip;
                node =  (SHASH_NODE *)spm_hashtab_search( SHARED.tunnelchhash, &tunskey NBB_CCXT);
                if(( node == NULL) || ( node->data == NULL))
                {
                    rv = ADD_VRFINS_TUNNELS_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins:  tunnesel data null ,rv=%d.\n",
                                                                               __LINE__,rv);
                }
                else
                {
                    rv = 0;
                    pwflag = 0 ;
                    tunnels = (TUNNELCH *)(node->data);
                    pwnode =  (PWTXNODE *)AVLL_FIRST(tunnels->pwtree);
                    while (pwnode != NULL)
                    {
                        qosvrfinskey.label = pwnode->key.label;
                        nhid = pwnode->posid;
						
                       // rv2 = spm_ds_add_vrf_node( difs.dserid , &qosvrfinskey, nhid NBB_CCXT);
                        rv = (rv|rv2);
                        pwflag = 1 ;
                        pwnode = (PWTXNODE *)AVLL_NEXT( tunnels->pwtree, pwnode->pw_node);
                    }
                    if(pwflag == 0)
                    {
                        rv = ADD_VRFINS_PWNODE_DATA_NULL;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: pwnode null ,rv=%d.\n",
                                                                               __LINE__,rv);
						
                       // spm_ds_add_vrf_node( 0 , &qosvrfinskey, 0 NBB_CCXT); 
                        goto  EXIT_LABEL;
                    }
                }
                
            //}
            if( rv == 0) 
            {
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(vrfins->difs), &difs, sizeof( VRFINS_DIFSER) );

            }
            else
            {
                rv = ADD_VRFINS_VRF_DATA_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins:ds vrf err ,rv=%d.\n",
                                                                               __LINE__,rv);
                goto  EXIT_LABEL;
            }
        }   
        else if( vrfinsips->oper_diff_serv == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "Del VRFINS Error: diff_serv_data data "));
            vrfinsips->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
        if( vrfinsips->oper_up_vpn_qos == ATG_DCI_OPER_ADD) /* 增加*/
        {
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(vrfinsips, &vrfinsips->up_vpn_qos_data); 
            if (ucdata == NULL)
            {
                rv = ADD_VRFINS_VPN_QOS_IPS_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins:vpn qos ips null ,rv=%d.\n",
                                                                               __LINE__,rv);
                goto  EXIT_LABEL;
            }

            vrfinsips->return_code = ATG_DCI_RC_OK;
            vrfinsips->up_vpn_qos_return_code = ATG_DCI_RC_OK;
            OS_MEMSET( &upqos, 0, sizeof(VRFINS_UPQOS));
            NBB_MEMCPY( &upqos, ucdata, sizeof(VRFINS_UPQOS));
            NBB_MEMCPY( &(vrfins->upqos), &upqos, sizeof(VRFINS_UPQOS));
            OS_MEMSET( &qosvrfinskey, 0, sizeof( SPM_QOS_VRF_INSTANSE_KEY) );
            qosvrfinskey.vrf_id = vrfinskey.vrfid;
            qosvrfinskey.peer_ip = vrfinskey.peerip;

            tunskey.res = 0;
            tunskey.vrfid = vrfinskey.vrfid;
            tunskey.peerip = vrfinskey.peerip;
            node =  (SHASH_NODE *)spm_hashtab_search( SHARED.tunnelchhash, &tunskey NBB_CCXT);
            if(( node == NULL) || ( node->data == NULL))
            {	
                rv = spm_l3_vrfins_del_hqos_drv(vrfins,0);
                if(rv != 0)
                {
                    rv = DEL_VRFINS_VPN_HQOS_ERR;
                    vrfins->return_code = rv;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: hqos vrf ,rv=%d.\n",
                                                                               __LINE__,rv);
                }
                goto  EXIT_LABEL;
            }
            tunnels = (TUNNELCH *)(node->data);
            if( tunnels->base.tunnelid == 0)  /*FTN  TYPE*/
            {
                ftnkey.fec = tunskey.peerip;
                ftnkey.vrfid = 0;
                ftnkey.mask = 32;
                ftnkey.res = 0;
                
                uftn =  (FTN_TAB *)AVLL_FIND( SHARED.ftn_tree ,  &ftnkey ); 
                if( uftn == NULL)
                {
                    rv = spm_l3_vrfins_del_hqos_drv(vrfins,0);
                    if(rv != 0)
                    {
                        rv = DEL_VRFINS_VPN_HQOS_ERR;
                        vrfins->return_code = rv;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: hqos vrf ,rv=%d.\n",
                                                                               __LINE__,rv);
                    }
                    goto  EXIT_LABEL;
            
                }
                portindex = uftn->dclsp[0].nextport  ;
                qostunl.type = 0;
            }
            else  /*CRTX_LSP  TYPE */
            {
                tunlkey.ingress = tunnels->base.ingress;
                tunlkey.egress = tunnels->base.egress;
                tunlkey.tunnelid = tunnels->base.tunnelid;
                node =  (SHASH_NODE *)spm_hashtab_search( SHARED.lspprothash, &tunlkey NBB_CCXT);
                if(( node == NULL) || ( node->data == NULL))
                {
                   rv = spm_l3_vrfins_del_hqos_drv(vrfins,0);
                   if(rv != 0)
                   {
                       rv = DEL_VRFINS_VPN_HQOS_ERR;
                       vrfins->return_code = rv;
                       BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: hqos vrf ,rv=%d.\n",
                                                                               __LINE__,rv);
                   }
                   goto  EXIT_LABEL;
                }
                lspprot = (LSPPROT *)(node->data);
                txlspkey.ingress = tunnels->base.ingress;
                txlspkey.egress = tunnels->base.egress;
                txlspkey.tunnelid = tunnels->base.tunnelid;
                txlspkey.lspid = lspprot->lspb.lsp[0].lspid;
                node =  (SHASH_NODE *)spm_hashtab_search( SHARED.crlsptxhash, &txlspkey NBB_CCXT);
                if(( node == NULL) || ( node->data == NULL))
                {
                   rv = spm_l3_vrfins_del_hqos_drv(vrfins,0);
                   if(rv != 0)
                   {
                       rv = DEL_VRFINS_VPN_HQOS_ERR;
                       vrfins->return_code = rv;
                       BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: hqos vrf ,rv=%d.\n",
                                                                               __LINE__,rv);
                   }
                   goto  EXIT_LABEL;
                }
                txlsp = (CRTXLSP *)(node->data);
                portindex = txlsp->dclsp.nextport  ;
                qostunl.type = 1;
            }
            OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
            rv = spm_get_portid_from_logical_port_index(portindex, &portfo NBB_CCXT);
            if( rv != 0)     
            {
                rv = spm_l3_vrfins_del_hqos_drv(vrfins,0);
                if(rv != 0)
                {
                   rv = DEL_VRFINS_VPN_HQOS_ERR;
                   vrfins->return_code = rv;
                   BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: hqos vrf ,rv=%d.\n",
                                                                               __LINE__,rv);
                }
                goto  EXIT_LABEL;
            }
            else
            {
                vrfins->slot = portfo.slot_id;
                vrfins->port = portfo.port_id;
                vrfins->porttype = portfo.port_type;
                
                rv = 0;
                if(qostunl.type == 0)
                {
                qostunl.flag = 1;
                OS_MEMCPY( &(qostunl.ftn), &ftnkey, sizeof( FTN_KEY ) );
                }
                if(qostunl.type == 1)
                {
                    qostunl.flag = 1;
                    OS_MEMCPY( &(qostunl.tx_lsp), &txlspkey, sizeof( CRTXLSP_KEY ) );
                }
                pwflag = 0 ;
                OS_MEMSET( &qosvrfinskey, 0, sizeof( SPM_QOS_VRF_INSTANSE_KEY) );
                qosvrfinskey.vrf_id = vrfinskey.vrfid;
                qosvrfinskey.peer_ip = vrfinskey.peerip;
                OS_MEMCPY( &vrfupqos, &upqos, sizeof( ATG_DCI_VRF_INSTANCE_UP_VPN_QOS ) );
                OS_MEMCPY( &(vrfins->qos_tunkey), &qostunl, sizeof( SPM_QOS_TUNNEL_KEY ) );
                pwnode =  (PWTXNODE *)AVLL_FIRST(tunnels->pwtree);
                while (pwnode != NULL)
                {
                    qosvrfinskey.label = pwnode->key.label;
                    nhid = pwnode->posid;					
                    rv2 =  spm_l3_vrfins_add_hqos_drv(vrfins,nhid);                                              
                    rv = (rv|rv2);
                    pwflag = 1 ;
                    pwnode = (PWTXNODE *)AVLL_NEXT( tunnels->pwtree, pwnode->pw_node);
                }
                if(pwflag == 0)
                {
                    rv = ADD_VRFINS_VPN_QOS_PWNODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins:pwnode null ,rv=%d.\n",
                                                                               __LINE__,rv);

                    goto  EXIT_LABEL;
                }
            }
            if ( rv == 0 )      
            {
                
            }
            else
            {
                rv = ADD_VRFINS_VPN_HQOS_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: hqos vrf ,rv=%d.\n",
                                                                               __LINE__,rv);
                goto  EXIT_LABEL;
            }
        }   
        else if( vrfinsips->oper_up_vpn_qos == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
            vrfinsips->up_vpn_qos_return_code = ATG_DCI_RC_OK;
            if( vrfins == NULL )
            {
                vrfinsips->return_code = ATG_DCI_RC_OK;
                rv = 0;
                goto  EXIT_LABEL;
            }
            tunskey.res = 0;
            tunskey.vrfid = vrfins->key.vrfid;
            tunskey.peerip = vrfins->key.peerip;
            node =  (SHASH_NODE *)spm_hashtab_search( SHARED.tunnelchhash, &tunskey NBB_CCXT);
            if(( node == NULL) || ( node->data == NULL))
            {	
                rv = spm_l3_vrfins_del_hqos_drv(vrfins,0);
                if(rv != 0)
                {
                    rv = DEL_VRFINS_VPN_HQOS_ERR;
                    vrfins->return_code = rv;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add vrfins: hqos vrf ,rv=%d.\n",
                                                                               __LINE__,rv);
                }
                goto  EXIT_LABEL;
            }
            tunnels = (TUNNELCH *)(node->data);
            pwnode =  (PWTXNODE *)AVLL_FIRST(tunnels->pwtree);
            while (pwnode != NULL)
            {
                qosvrfinskey.label = pwnode->key.label;
                nhid = pwnode->posid;					
                rv = spm_l3_vrfins_del_hqos_drv(vrfins,nhid);                                     
                rv = (rv|rv2);
                pwflag = 1 ;
                pwnode = (PWTXNODE *)AVLL_NEXT( tunnels->pwtree, pwnode->pw_node);
            }
            if(pwflag == 0)
            {
                rv = DEl_VRFINS_VPN_QOS_PWNODE_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e del vrfins:pwnode null ,rv=%d.\n",
                                                                           __LINE__,rv);

                goto  EXIT_LABEL;
            }
            if(rv != 0)
            {
                rv = DEL_VRFINS_VPN_HQOS_ERR;
                vrfins->return_code = rv;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e del vrfins: hqos vrf ,rv=%d.\n",
                                                                           __LINE__,rv);
            }
            else
            {
              vrfins->upqos.nodeindex = 0;
            }
          
        }
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();
    
    /*程序异常时，记录错误条目的key值*/
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d vrfins:err vrfid=%d.peerip=%x\n",__LINE__,
			                                vrfinskey.vrfid,vrfinskey.peerip);
    }

}

#endif

