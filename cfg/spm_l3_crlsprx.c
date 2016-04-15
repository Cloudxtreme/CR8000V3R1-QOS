
/**MOD+***********************************************************************/
/* Module:    spml3.c                                                      */
/*                                                                           */
/* Purpose:   R8000     L3    cfgprocess                        */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* FiberHome        2003-2012                                                */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/*                                                                           */
/* $Revision: 1.0       $ $Date: 2012/9/2 07:09:09 $             */
/**MOD-***********************************************************************/



#define SHARED_DATA_TYPE SPM_SHARED_LOCAL
#include <nbase.h>
#include <spmincl.h>

/********************
CR_LSP_RX配置块
********************/
#ifdef CR_LSP_RX
NBB_ULONG spm_l3_crlsprxhashkey(NBB_VOID *keyarg NBB_CCXT_T NBB_CXT)
{
    if( keyarg == NULL)
    {
        return 0;
    }
    else
    {
        return spm_hash_algorithm(keyarg, CRRXLSPKEYLGTH, 0 NBB_CCXT);
    }
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_crlsprxhashcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_ULONG spm_l3_crlsprxhashcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    CRRXLSP_KEY *lsp_key1;
    CRRXLSP_KEY *lsp_key2;

    lsp_key1 = (CRRXLSP_KEY *)(keyarg1);
    lsp_key2 = (CRRXLSP_KEY *)(keyarg2);

    if( lsp_key1->ingress == lsp_key2->ingress 
    && lsp_key1->egress == lsp_key2->egress 
    && lsp_key1->tunnelid == lsp_key2->tunnelid 
    && lsp_key1->lspid == lsp_key2->lspid)
    {
        return 1;
    }

    return 0;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_crlsprxhashtableinit
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_crlsprxhashtableinit( NBB_ULONG num NBB_CCXT_T NBB_CXT)
{

    SHARED.crlsprxhash = (SHASH_TABLE *)
                            spm_hashtab_create( num , spm_l3_crlsprxhashkey,spm_l3_crlsprxhashcomp NBB_CCXT);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_crlsprxprocess
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_crlsprxprocess( ATG_DCI_SET_CR_LSP_RX * crlsprxips NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 100;

    NBB_INT         rv2 = 100;

    SHASH_NODE      *node = NULL, *dnode = NULL;

    NBB_BYTE        *ucdata = NULL;

    NBB_BYTE        flag = 0;

    CRRXLSP_KEY     rxlspkey;

    CRRXLSP_BASE    base;
    CRRXLSP_BASE    old_base;

    CRRXLSP_TEFRR   tefrr;

    CRRXLSP         *rxlsp = NULL;
    
    RX_LSP_T        lspt;

    SPM_PORT_INFO_CB    portfo;

    CRRXLSP_DIFSER  difser;

    DC_LSP          dclsp;

    NBB_USHORT      frrid = 0;

    NBB_CHAR        *errorlog = NULL;
    NBB_ULONG       str_len = 0;

    NBB_TRC_ENTRY("spm_l3_crlsprxprocess");

    NBB_ASSERT(crlsprxips != NULL);

    if(NULL == crlsprxips)
    {
        goto EXIT_LABEL;
    }

    crlsprxips->return_code = ATG_DCI_RC_OK;

    OS_MEMSET(&lspt, 0, sizeof(RX_LSP_T));

    OS_MEMSET( &rxlspkey, 0, sizeof( CRRXLSP_KEY ) );
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &rxlspkey, &(crlsprxips->key), sizeof( CRRXLSP_KEY ) );

    if(( rxlspkey.ingress == 0 ) && ( rxlspkey.egress == 0 ) && ( rxlspkey.tunnelid == 0 ) && ( rxlspkey.lspid == 0 ))
    {
        NBB_PRINTF("LSP RX data error rxlspkey = 0 \n");
        crlsprxips->return_code = ATG_DCI_RC_OK;
        goto  EXIT_LABEL;
    }
   
    /*查看本地数据是否存在*/
    node =  (SHASH_NODE *)spm_hashtab_search( SHARED.crlsprxhash, &rxlspkey NBB_CCXT);   
    
    if( crlsprxips->delete_struct == 1) /*  删除*/
    {
        if( node == NULL )  
        {
            crlsprxips->return_code = ATG_DCI_RC_OK;
            rv = 0;
            goto  EXIT_LABEL;
        }
        if( node->data == NULL )    
        {
            rv = DEL_CRLSP_RX_NODE_DATA_NULL;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del rxlsp data null ,rv=%d.\n",
                                                                                __LINE__,rv);
            crlsprxips->return_code = ATG_DCI_RC_OK;
            goto  EXIT_LABEL;
        }
        rxlsp = (CRRXLSP *)(node->data);

        if( rxlsp->frrid != 0 )
        {

            rv = spm_l3_rxlsp_del_drv_frrgroup(rxlsp->frrid);
            if(rv != 0)
            {
                rv = DEL_CRLSP_RX_DRV_FRR_ERR; 
                rxlsp->return_code = rv;
                
            }
            else
            {
                rxlsp->frrid = 0;
            }
        }
        if( rxlsp->posid != 0 )
        {
            rv = spm_l3_rxlsp_del_drv_rxlsp(rxlsp->posid);
            if(rv != 0)
            {
                rv = DEL_CRLSP_RX_DRV_LSP_ERR;
                rxlsp->return_code = rv;
            }
            else
            {
                rxlsp->posid = 0;
            }
        }
        
        if( rv == 0)      
        {
            rxlsp = (CRRXLSP *)(node->data);
            spm_hashtab_del( SHARED.crlsprxhash, &rxlspkey NBB_CCXT);
            NBB_MM_FREE(rxlsp, MEM_SPM_CR_LSP_RX_CB);
        }   
    }
    else if( crlsprxips->delete_struct == 0) /* 增加*/
    {
        if( crlsprxips->oper_basic == ATG_DCI_OPER_ADD) /* 增加*/
        {
            crlsprxips->basic_return_code = ATG_DCI_RC_OK;
            if( node == NULL )
            {
                rxlsp = (CRRXLSP *)NBB_MM_ALLOC(sizeof(CRRXLSP), NBB_NORETRY_ACT, MEM_SPM_CR_LSP_RX_CB);
                if ( rxlsp == NULL )
                {
                    rv = ADD_CRLSP_RX_MALLOC_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp,moc null ,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto  EXIT_LABEL;
                } 
                NBB_MEMSET(rxlsp, 0, sizeof(CRRXLSP));
                flag = 1;
            }
            else
            {
                if( node->data == NULL )    
                {
                    rv = ADD_CRLSP_RX_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp node null ,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto  EXIT_LABEL;
                }
                flag = 2;
                rxlsp = (CRRXLSP *)(node->data);
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(crlsprxips, &crlsprxips->basic_data); 
            if (ucdata == NULL)
            {
                rv = ADD_CRLSP_RX_IPS_BASIC_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp:ips basic data null ,rv=%d.\n",
                                                                               __LINE__,rv);
                rxlsp->return_code = rv;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &base, 0, sizeof( CRRXLSP_BASE ) );
            OS_MEMSET( &old_base, 0, sizeof( CRRXLSP_BASE ) );
            NBB_MEMCPY( &base, ucdata, sizeof( CRRXLSP_BASE ) ); 

            /*配置下发完，存储配置*/
            NBB_MEMCPY( &(rxlsp->key), &(crlsprxips->key), sizeof( CRRXLSP_KEY ) );
            NBB_MEMCPY( &(old_base), &rxlsp->base, sizeof( CRRXLSP_BASE ) );  
            NBB_MEMCPY( &(rxlsp->base), &base, sizeof( CRRXLSP_BASE ) );   
            //coverity[bad_sizeof]
           
            if( flag == 1)
            {
                spm_hashtab_insert ( SHARED.crlsprxhash, rxlsp->key, rxlsp NBB_CCXT); 
            }
            if( old_base.tefrrnode != 1 )
            {
                if( rxlsp->frrid != 0 )
                {
                    rv =  spm_l3_rxlsp_del_drv_frrgroup(rxlsp->frrid);
                    if( rv != 0)   
                    {
                        rv = DEL_CRLSP_RX_DRV_FRR_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e crlsprx frrid.rv = %d.%d \n", 
                                        __LINE__, rxlsp->frrid, rv);
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                    else
                    {
                        rxlsp->frrid = 0;
                    }
                }
                
                //ApiC3InitLspInstStruct( &lspt );;
                OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                if( base.nextport != 0 )
                {
                    rv = spm_get_portid_from_logical_port_index(base.nextport, &portfo NBB_CCXT);
                    if( rv == 0)    
                    {   
                        rxlsp->mslot_id = portfo.slot_id;
                        rxlsp->mport_id = portfo.port_id;
                        rxlsp->msvlan = portfo.svlan;
                        
                    }
                    else
                    {
                        rv = ADD_CRLSP_RX_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: get l2port,rv=%d.port=%d\n",
                                                                           __LINE__,rv,base.nextport);
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                   }
                }
                rv = spm_l3_rxlsp_create_drv_rxlsp (rxlsp);
                if(rv != 0)
                {
                    rv = ADD_CRLSP_RX_DRV_LSP_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: drv lsp,rv=%d\n",
                                                                                 __LINE__,rv);
                    rxlsp->return_code = rv;
                    goto  EXIT_LABEL;
                }
            }
            else
            {
                rv = 0 ;
            }
            
        }
        else   if( crlsprxips->oper_basic == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
            crlsprxips->basic_return_code = ATG_DCI_RC_OK;  
            goto  EXIT_LABEL;
        }
        if( crlsprxips->oper_prot == ATG_DCI_OPER_ADD) /* 增加*/   
        {
            crlsprxips->prot_return_code = ATG_DCI_RC_OK;
            if(( crlsprxips->oper_basic != ATG_DCI_OPER_ADD))
            {
                if ( node == NULL )                                 
                {
                    rv = ADD_CRLSP_RX_PROT_NODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: tefrr node null,rv=%d\n",
                                                                                     __LINE__,rv);                    
                    goto   EXIT_LABEL;
                }
                if( node->data == NULL )    
                {
                    rv = ADD_CRLSP_RX_PROT_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp:tefrr node data null,rv=%d\n",
                                                                                     __LINE__,rv);
                    goto  EXIT_LABEL;
                }
                rxlsp = (CRRXLSP *)(node->data);
            }
            if( rxlsp == NULL ) 
            {
                rv = ADD_CRLSP_RX_PROT_RXLSP_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: tefrr rxlsp null,rv=%d\n",
                                                                                     __LINE__,rv);
                goto  EXIT_LABEL;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(crlsprxips, &crlsprxips->prot_data); 
            if (ucdata == NULL)
            {
                rv = ADD_CRLSP_RX_PROT_IPS_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: tefrr ips null,rv=%d\n",
                                                                                     __LINE__,rv);
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &tefrr, 0, sizeof( CRRXLSP_TEFRR) ); 
            NBB_MEMCPY( &tefrr, ucdata, sizeof( CRRXLSP_TEFRR) ); 

            /*先保存TE FRR配置*/
            NBB_MEMCPY( &(rxlsp->tefrr),  &tefrr,  sizeof( CRRXLSP_TEFRR ) );
            if( rxlsp->frrid == 0 )
            {
                rv = spm_l3_rxlsp_create_drv_frrgroup(&frrid);
                if(rv != 0)
                {
                    rv = ADD_CRLSP_RX_PROT_DRV_FRR_ERR;
                    rxlsp->return_code = rv;
                    goto  EXIT_LABEL;
                }
                else
                {
                    rxlsp->frrid = frrid;
                }
                
                if ( rxlsp->base.tefrrtype == 1)//bypass
                {
                    OS_MEMSET(&dclsp, 0, sizeof(DC_LSP));
                    rv = spm_l3_iteration_tunnel( tefrr.nextport, &dclsp NBB_CCXT);
                    if ( rv == 0 )
                    {
                        rxlsp->dclsp.label1 = tefrr.outlabel;
                        rxlsp->dclsp.label2 = dclsp.label1;
                        rxlsp->dclsp.nextip =  dclsp.nextip;
                        rxlsp->dclsp.nextport =  dclsp.nextport;
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: iter tunl,rv=%d\n",
                                                                                 __LINE__,rv);
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else
                {
                    rxlsp->dclsp.label1 = tefrr.outlabel;
                    rxlsp->dclsp.nextip =  tefrr.nextip;
                    rxlsp->dclsp.nextport =  tefrr.nextport;
                }
                if( rxlsp->dclsp.nextport != 0 )
                {
                    rv = spm_get_portid_from_logical_port_index(rxlsp->dclsp.nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    {
                         rxlsp->prot_slot_id = portfo.slot_id;
                         rxlsp->prot_port_id = portfo.port_id;
                         rxlsp->prot_svlan = portfo.svlan ;
                    }
                    else
                    {
                        rv = ADD_CRLSP_RX_PROT_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: tefrr get l2port,rv=%d\n",
                                                                                 __LINE__,rv);  
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                if( rxlsp->base.nextport != 0 )
                {
                    rv = spm_get_portid_from_logical_port_index(rxlsp->base.nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    {
                        rxlsp->mslot_id = portfo.slot_id;
                        rxlsp->mport_id = portfo.port_id;
                        rxlsp->msvlan = portfo.svlan;
                    }
                    else
                    {
                        rv = ADD_CRLSP_RX_PROT_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: tefrr getl2port,rv=%d\n",
                                                                             __LINE__,rv); 
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                rv = spm_l3_rxlsp_create_drv_rxlspprot(rxlsp);
                if(rv != 0)
                {
                    rv = ADD_CRLSP_RX_PROT_DRV_LSPP_ERR;
                    rxlsp->return_code = rv;
                    goto  EXIT_LABEL;
                }
            }
            else
            {
                OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                if( rxlsp->base.nextport != 0 )
                {
                    rv = spm_get_portid_from_logical_port_index(rxlsp->base.nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    {
                        rxlsp->mslot_id = portfo.slot_id;
                        rxlsp->mport_id = portfo.port_id;
                        rxlsp->msvlan = portfo.svlan;
                    }
                    else
                    {
                        rv = ADD_CRLSP_RX_PROT_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp:tefrr getl2port,rv=%d\n",
                                                                                 __LINE__,rv);  
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                
                if ( rxlsp->base.tefrrtype == 1)//bypass
                {   
                    OS_MEMSET(&dclsp, 0, sizeof(DC_LSP));
                    rv = spm_l3_iteration_tunnel( tefrr.nextport , &dclsp NBB_CCXT);
                    if ( rv == 0 )
                    {
                        rxlsp->dclsp.label1 = tefrr.outlabel;
                        rxlsp->dclsp.label2 = dclsp.label1;
                        rxlsp->dclsp.nextip =  dclsp.nextip;
                        rxlsp->dclsp.nextport =  dclsp.nextport;
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp: tefrr iter tunl,rv=%d\n",
                                                                                 __LINE__,rv); 
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else
                {
                    rxlsp->dclsp.label1 = tefrr.outlabel;
                    rxlsp->dclsp.nextip =  tefrr.nextip;
                    rxlsp->dclsp.nextport =  tefrr.nextport;
                }
                if( rxlsp->dclsp.nextport != 0 )
                {
                    rv = spm_get_portid_from_logical_port_index(rxlsp->dclsp.nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    {
                        rxlsp->prot_slot_id = portfo.slot_id;
                        rxlsp->prot_port_id = portfo.port_id;
                        rxlsp->prot_svlan = portfo.svlan ;
                    }
                    else
                    {
                        rv = ADD_CRLSP_RX_PROT_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d Add rxlsp: tefrr get l2port,rv=%d\n",
                                                                                 __LINE__,rv); 
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                    }

                    rv = spm_l3_rxlsp_mod_drv_rxlspprot(rxlsp);
                    if(rv != 0)
                    {
                        rv = ADD_CRLSP_RX_PROT_DRV_MODLSP_ERR;
                        rxlsp->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                    
                } 
            }
                               
        }
        else  if( crlsprxips->oper_prot == ATG_DCI_OPER_DEL )   /*  删除子TLV*/
        {
            if( node == NULL )  
            {
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e crlsprx node = NULL  \n", __LINE__);
                rv = 0;
                goto  EXIT_LABEL;
            }
            if( node->data == NULL )    
            {
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e crlsprx node->data = NULL \n", __LINE__);
                rv = 0;
                goto  EXIT_LABEL;
            }
            rxlsp = (CRRXLSP *)(node->data);
            if( rxlsp->posid != 0 )
            {
                 rv = spm_l3_rxlsp_del_drv_rxlspprot(rxlsp->posid);
                 if(rv != 0)
                 {
                     rv = DEL_CRLSP_RX_PROT_DRV_LSPP_ERR;
                     rxlsp->return_code = rv;
                     goto  EXIT_LABEL;
                 }
            }
        }
        if( crlsprxips->oper_diff_serv == ATG_DCI_OPER_ADD) 
        {
            if(( crlsprxips->oper_basic != ATG_DCI_OPER_ADD))
            {
                if ( node == NULL )                             
                {
                    rv = ADD_CRLSP_RX_DS_NODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp:ds node null,rv=%d\n",
                                                                                     __LINE__,rv);
                    crlsprxips->diff_serv_return_code = ATG_DCI_RC_OK;
                    goto   EXIT_LABEL;
                }
                if( node->data == NULL )    
                {
                    rv = ADD_CRLSP_RX_DS_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp:ds node data null,rv=%d\n",
                                                                                     __LINE__,rv);
                    crlsprxips->diff_serv_return_code = ATG_DCI_RC_OK;
                    goto  EXIT_LABEL;
                }
                rxlsp = (CRRXLSP *)(node->data);
            }
            if( rxlsp == NULL ) 
            {
                rv = ADD_CRLSP_RX_PROT_RXLSP_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp:rxlsp null,rv=%d\n",
                                                                                     __LINE__,rv);
                crlsprxips->diff_serv_return_code = ATG_DCI_RC_OK;
                goto  EXIT_LABEL;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(crlsprxips, &crlsprxips->diff_serv_data);
            if (ucdata == NULL)
            {
                rv = ADD_CRLSP_RX_DS_IPS_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp:ds ips null,rv=%d\n",
                                                                                     __LINE__,rv);
                crlsprxips->diff_serv_return_code = ATG_DCI_RC_OK;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &difser, 0, sizeof(CRRXLSP_DIFSER));
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &difser, ucdata, sizeof(CRRXLSP_DIFSER)); 
            //rv = spm_ds_add_rx_lsp_node(difser.dserid, &(rxlsp->key), 
             //                                            rxlsp->posid, rxlsp->base.action, 1, 0, NULL NBB_CCXT);
            
            if ( rv == 0 )      
            {
                rxlsp->mdserid = difser.dserid;
            }
            else
            {
                rv = ADD_CRLSP_RX_DS_DATA_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add rxlsp:ds err,rv=%d\n",
                                                                                     __LINE__,rv);
                crlsprxips->diff_serv_return_code = ATG_DCI_RC_OK; 
                goto  EXIT_LABEL;
            }
        }
        else if( crlsprxips->oper_diff_serv == ATG_DCI_OPER_DEL) 
        {
            NBB_TRC_FLOW((NBB_FORMAT "Del LSP RX Error: qos data "));
            crlsprxips->diff_serv_return_code = ATG_DCI_RC_OK;  
            goto  EXIT_LABEL;
        }
        if( crlsprxips->oper_prot_diff_serv == ATG_DCI_OPER_ADD) 
        {
        
        }
        else if( crlsprxips->oper_prot_diff_serv == ATG_DCI_OPER_DEL) 
        {

        }
        if( crlsprxips->oper_wrapping == ATG_DCI_OPER_ADD) 
        {
        
        }
        else if( crlsprxips->oper_wrapping == ATG_DCI_OPER_DEL) 
        {

        }
    }
    EXIT_LABEL:

    NBB_TRC_EXIT();

    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d rx:err in=%x.egr=%x.tunl=%d.lspid=%x\n",
                                 __LINE__,rxlspkey.ingress,rxlspkey.egress,rxlspkey.tunnelid,rxlspkey.lspid);
    }

}

#endif
