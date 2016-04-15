
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
CR_LSP_TX配置块
********************/
#ifdef CR_LSP_TX

/**PROC+****************************************************************************************/
/* Name:      spm_l3_crlsptxhashkey                                        */
/*                                                                              */
/* Purpose:   计算 CR_LSP_TX配置块hash值      */
/*                                                                              */
/* Returns:                                             */
/*                                                              */
/*                                                              */
/* Params:                                                              */
/*                                                                              */
/* Operation:                                                   */
/*                                                                              */
/**PROC-****************************************************************************************/
NBB_ULONG  spm_l3_crlsptxhashkey(NBB_VOID *keyarg NBB_CCXT_T NBB_CXT)
{
    if( keyarg == NULL)
    {
        return 0;
    }
    else
    {
        return spm_hash_algorithm(keyarg, CRTXLSPKEYLGTH, 0 NBB_CCXT);
    }
}

/**PROC+****************************************************************************************/
/* Name:      spm_l3_crlsptxhashcomp                                      */
/*                                                                              */
/* Purpose:  CR_LSP_TX配置块hash比较            */
/*                                                                              */
/* Returns:                                             */
/*                                                              */
/*                                                              */
/* Params:                                                              */
/*                                                                              */
/* Operation:                                                   */
/*                                                                              */
/**PROC-****************************************************************************************/
NBB_ULONG  spm_l3_crlsptxhashcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    CRTXLSP_KEY *lsp_key1;
    CRTXLSP_KEY *lsp_key2;

    lsp_key1 = (CRTXLSP_KEY *)(keyarg1);
    lsp_key2 = (CRTXLSP_KEY *)(keyarg2);

    if( lsp_key1->ingress == lsp_key2->ingress 
    && lsp_key1->egress == lsp_key2->egress 
    && lsp_key1->tunnelid == lsp_key2->tunnelid 
    && lsp_key1->lspid == lsp_key2->lspid)
    {
        return 1;
    }

    return 0;
}

/**PROC+****************************************************************************************/
/* Name:      CrRcLspHashtableInit                                  */
/*                                                                              */
/* Purpose:  CR_LSP_TX   hash初始化             */
/*                                                                              */
/* Returns:                                             */
/*                                                              */
/*                                                              */
/* Params:                                                              */
/*                                                                              */
/* Operation:                                                   */
/*                                                                              */
/**PROC-****************************************************************************************/
NBB_VOID  spm_l3_crlsptxhashtableinit( NBB_ULONG num NBB_CCXT_T NBB_CXT)
{

    SHARED.crlsptxhash = (SHASH_TABLE *)
                            spm_hashtab_create( num , spm_l3_crlsptxhashkey, spm_l3_crlsptxhashcomp NBB_CCXT);

}

/**PROC+****************************************************************************************/
/* Name:      SpmCrLspTxProcess                                   */
/*                                                                              */
/* Purpose:  CR_LSP_TX   配置处理               */
/*                                                                              */
/* Returns:                                             */
/*                                                              */
/*                                                              */
/* Params:  内存释放还需要考虑          */
/*                                                                              */
/* Operation:                                                   */
/*                                                                              */
/**PROC-****************************************************************************************/
NBB_VOID spm_l3_crlsptxprocess(ATG_DCI_SET_CR_LSP_TX * crlsptxips NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 100;

    NBB_INT         rv2 = 100;

    SHASH_NODE      *node = NULL, *dnode = NULL;

    NBB_BYTE        *ucdata = NULL;

    NBB_BYTE        flag = 0;

    NBB_ULONG       nhid = 0;

    CRTXLSP_KEY     txlspkey;

    CRTXLSP         *txlsp = NULL;

    TX_LSP_T        lsptun;

    SPM_PORT_INFO_CB    portfo;

    NBB_ULONG       portindex;

    DC_LSP          dclsp;

    CRTXLSP_DIFSER  difser;

    CRTXLSP_UPQOS   upqos;

    ATG_DCI_LSP_TX_UP_TNNL_QOS txupqos;

    SPM_QOS_TUNNEL_KEY      qoskey;
    
    NBB_CHAR            *errorlog = NULL;
    NBB_ULONG           str_len = 0;

    LSPPROT_KEY     tunlkey;

    LSPPROT         *lspprot = NULL;
        
    NBB_TRC_ENTRY("spm_l3_crlsptxprocess");

    NBB_ASSERT(crlsptxips != NULL);

    if(NULL == crlsptxips)
    {
        goto EXIT_LABEL;
    }

    crlsptxips->return_code = ATG_DCI_RC_OK;

    OS_MEMSET(&lsptun , 0, sizeof(TX_LSP_T));
    
    OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));

    OS_MEMSET( &txlspkey, 0, sizeof( CRTXLSP_KEY ) );
    
    NBB_MEMCPY( &txlspkey, &(crlsptxips->key), sizeof( CRTXLSP_KEY ) );

    if(( txlspkey.ingress == 0 ) && ( txlspkey.egress == 0 ) && ( txlspkey.tunnelid == 0 ) && ( txlspkey.lspid == 0 ))
    {
        NBB_PRINTF("LSP TX data error txlspkey = 0 \n");
        crlsptxips->return_code = ATG_DCI_RC_UNSUCCESSFUL;  
        goto  EXIT_LABEL;
    }
    
    /**************************************************************/

    /**************************************************************/

    /*操作之前，查看本地数据是否存在*/
    node =  (SHASH_NODE *)spm_hashtab_search( SHARED.crlsptxhash ,&txlspkey  NBB_CCXT);    
    
    if( crlsptxips->delete_struct == 1) /*  删除*/
    {
        /*  删除整个条目*/
        if( node == NULL )
        {
            rv = DEL_CRLSP_TX_NODE_NULL;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Deltxlsp:del node null ,rv=%d.\n",
                                                                               __LINE__,rv);
            crlsptxips->return_code = ATG_DCI_RC_OK;  
            rv = 0;
            goto  EXIT_LABEL;
        }
        if( node->data == NULL )
        {
            rv = DEL_CRLSP_TX_NODE_DATA_NULL;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del txlsp:del node null ,rv=%d.\n",
                                                                               __LINE__,rv);
            crlsptxips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto  EXIT_LABEL;
        }
        txlsp = (CRTXLSP *)(node->data);
        if( txlsp->mdserid != 0 )
        {
            OS_MEMSET( &qoskey, 0, sizeof(SPM_QOS_TUNNEL_KEY));
            qoskey.type = 1;
            NBB_MEMCPY( &(qoskey.tx_lsp), &(txlsp->key), sizeof(CRTXLSP_KEY) ); 
           // rv = spm_ds_add_tx_lsp_node( 0, &qoskey, txlsp->dclsp.index, 0, NULL NBB_CCXT);
            if( rv != 0)  
            {
                rv = DEL_CRLSP_TX_DS_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del txlsp:del ds ,rv=%d.\n",
                                                                               __LINE__,rv);
                txlsp->return_code = rv;
                crlsptxips->return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            }
            else
            {
                txlsp->mdserid = 0;
            }
        }
        if( txlsp->upqos.nodeindex != 0 )
        {
            OS_MEMSET( &qoskey, 0, sizeof(SPM_QOS_TUNNEL_KEY));
            qoskey.type = 1;
            NBB_MEMCPY( &(qoskey.tx_lsp), &(txlsp->key), sizeof(CRTXLSP_KEY) ); 
            OS_MEMSET( &upqos, 0, sizeof(CRTXLSP_UPQOS));
            upqos.mode = txlsp->upqos.mode;
            upqos.nodeindex = txlsp->upqos.nodeindex;
            OS_MEMCPY( &txupqos, &upqos, sizeof( ATG_DCI_LSP_TX_UP_TNNL_QOS ) );
           // rv = spm_hqos_add_lsp_node( txlsp->slot, txlsp->port, &qoskey, &txupqos, nhid NBB_CCXT);
            if( rv != 0)  
            {
                rv = DEL_CRLSP_TX_TUNNEL_QOS_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del txlsp:del tunl qos,rv=%d.\n",
                                                                               __LINE__,rv);
                txlsp->return_code = rv;
                crlsptxips->return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            }
            else
            {
                txlsp->upqos.nodeindex = 0;
            }
        }
        if( txlsp->dclsp.index != 0 )
        {
            rv = spm_l3_txlsp_del_drv_txlsp(txlsp->dclsp.index);
            if(rv != 0)
            {
                rv = DEL_CRLSP_TX_DRV_LSP_ERR;
                txlsp->return_code = rv;
            }
            else
            {
                txlsp->dclsp.index = 0;
            }
        }
        if( rv == 0)      /* 删除成功，删除hash表对应的配置*/
        {
            txlsp = (CRTXLSP *)(node->data);
            spm_hashtab_del( SHARED.crlsptxhash ,&txlspkey NBB_CCXT);
            NBB_MM_FREE(txlsp, MEM_SPM_CR_LSP_TX_CB);
        }
    }
    else if( crlsptxips->delete_struct == 0) /* 增加*/
    {
        if( crlsptxips->oper_basic == ATG_DCI_OPER_ADD) /* 增加*/
        {
            crlsptxips->basic_return_code = ATG_DCI_RC_OK;
             /*操作之前，查看本地数据是否存在*/
            if( node == NULL )
            {
                txlsp = (CRTXLSP *)NBB_MM_ALLOC(sizeof(CRTXLSP), NBB_NORETRY_ACT, MEM_SPM_CR_LSP_TX_CB);
                
                if ( txlsp == NULL )
                {
                    rv = ADD_CRLSP_TX_MALLOC_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:moc txlsp null ,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto  EXIT_LABEL;
                } 
                NBB_MEMSET(txlsp, 0, sizeof(CRTXLSP));
                flag = 1;
            }
            else
            {
                if( node->data == NULL )
                {
                    rv = ADD_CRLSP_TX_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:node data null ,rv=%d.\n",
                                                                               __LINE__,rv);  
                    goto  EXIT_LABEL;
                }
                flag = 2;
                txlsp = (CRTXLSP *)(node->data);
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(crlsptxips, &crlsptxips->basic_data); 
            if (ucdata == NULL)
            {
                rv = ADD_CRLSP_TX_IPS_BASIC_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:basic data null ,rv=%d.\n",
                                                                               __LINE__,rv);
                txlsp->return_code = rv;
                goto  EXIT_LABEL;
            }
             
            //coverity[bad_sizeof]       
            NBB_MEMCPY( &(txlsp->key), &(crlsptxips->key), sizeof( CRTXLSP_KEY ) );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(txlsp->base), ucdata, sizeof( CRTXLSP_BASE ) );

            /*配置下发，先存储txlsp配置*/
            if( flag == 1)
            {
                spm_hashtab_insert ( SHARED.crlsptxhash ,txlsp->key, txlsp);
            }
          
            //rv = spm_l3_checkipdirectconnect( 0 ,&(txlsp->base.nextip));
            //rv = spm_l3_checkip( 0, 0, &(txlsp->base.nextip) NBB_CCXT);

            rv = 0;
            if ( rv == 0 )
            {
                txlsp->dclsp.label2 = 0xffffffff;
                txlsp->dclsp.label1 = txlsp->base.outlabel;
                txlsp->dclsp.nextip =  txlsp->base.nextip;
                txlsp->dclsp.nextport =  txlsp->base.nextport;
            }
            else if ( rv == 1 )
            {
                //因为系统暂时只支持3层标签。
                portindex = txlsp->base.nextport;
                rv = spm_l3_iteration_tunnel( portindex , &dclsp NBB_CCXT);
                if ( rv == 0 )
                {
                    txlsp->dclsp.label1 = txlsp->base.outlabel;
                    txlsp->dclsp.label2 = dclsp.label1;
                    txlsp->dclsp.nextip =  dclsp.nextip;
                    txlsp->dclsp.nextport =  dclsp.nextport;
                }
                else
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:iter tunnel ,rv=%d.port=%d.\n",
                                                                      __LINE__,rv,portindex);
                    txlsp->return_code = rv;
                    goto  EXIT_LABEL;
                }
            }
            if( rv == 0)
            {
                OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                rv = spm_get_portid_from_logical_port_index(txlsp->dclsp.nextport, &portfo NBB_CCXT);
                if( rv == 0)     
                {
                    txlsp->slot = portfo.slot_id;
                    txlsp->port = portfo.port_id;
                    txlsp->svlan = portfo.svlan;
                    txlsp->porttype = portfo.port_type;
                }
                else
                {
                    rv = ADD_CRLSP_TX_GET_L2PORT_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:get l2port ,rv=%d.port=%d\n",
                                                                    __LINE__,rv,txlsp->dclsp.nextport);
                    txlsp->return_code = rv;                  
                    goto  EXIT_LABEL;
                }
            }

            /*调用驱动适配接口，创建驱动lsp*/
            rv = spm_l3_txlsp_create_drv_txlsp(txlsp);
            if( rv == 0)      /*成功*/
            {
                tunlkey.ingress = txlsp->key.ingress;
                tunlkey.egress =  txlsp->key.egress;
                tunlkey.tunnelid = txlsp->key.tunnelid;
                dnode =  (SHASH_NODE *)spm_hashtab_search( SHARED.lspprothash, &tunlkey NBB_CCXT);
                if(( dnode == NULL) || ( dnode->data == NULL))
                {
                
                }
                else
                {
                    lspprot = (LSPPROT *)(dnode->data);
                    if( lspprot->lspb.lsp[0].lspid == txlsp->key.lspid )
                    {
                        //coverity[bad_sizeof]
                        NBB_MEMCPY( &(lspprot->dclsp[0]), &(txlsp->dclsp), sizeof( DC_LSP ) );
                    }
                    else if ( lspprot->lspb.lsp[1].lspid == txlsp->key.lspid )
                    {
                        //coverity[bad_sizeof]
                        NBB_MEMCPY( &(lspprot->dclsp[1]), &(txlsp->dclsp), sizeof( DC_LSP ) );
                    }   
                }

                
            }
            else    /*调用驱动失败，记录错误*/
            {
                rv = ADD_CRLSP_TX_DRV_LSP_ERR;  
                txlsp->return_code = rv;
                goto  EXIT_LABEL;
            }
        }
        else if( crlsptxips->oper_basic == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "Del LSP TX Error: basic data "));
            crlsptxips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
        if( crlsptxips->oper_tefrr == ATG_DCI_OPER_ADD) /* 增加*/   
        {
            NBB_TRC_FLOW((NBB_FORMAT "Add LSP TX Error: tefrr data "));
            crlsptxips->tefrr_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
        else if( crlsptxips->oper_tefrr == ATG_DCI_OPER_DEL )   /*  删除子TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "Del LSP TX Error: tefrr data "));
            crlsptxips->tefrr_return_code = ATG_DCI_RC_UNSUCCESSFUL; 
            goto  EXIT_LABEL;
        }
        if( crlsptxips->oper_diff_serv == ATG_DCI_OPER_ADD) 
        {
            crlsptxips->diff_serv_return_code = ATG_DCI_RC_OK;
            if(( crlsptxips->oper_basic != ATG_DCI_OPER_ADD))
            {
                if ( node == NULL )                             
                {
                    rv = ADD_CRLSP_TX_DS_NODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:ds txlsp node null  ,rv=%d.\n",
                                                                             __LINE__,rv);
                    goto   EXIT_LABEL;
                }
                if( node->data == NULL )
                {
                    rv = ADD_CRLSP_TX_DS_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:ds  node data null  ,rv=%d.\n",
                                                                             __LINE__,rv);  
                    goto  EXIT_LABEL;
                }
                txlsp = (CRTXLSP *)(node->data);
            }
            if( txlsp == NULL ) 
            {
                rv = ADD_CRLSP_TX_DS_TXLSP_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:ds txlsp null  ,rv=%d.\n",
                                                                             __LINE__,rv);
                goto  EXIT_LABEL;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(crlsptxips, &crlsptxips->diff_serv_data);
            if (ucdata == NULL)
            {
                rv = ADD_CRLSP_TX_DS_IPS_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:ds ips data null  ,rv=%d.\n",
                                                                             __LINE__,rv);
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &difser, 0, sizeof(CRTXLSP_DIFSER));
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &difser, ucdata, sizeof(CRTXLSP_DIFSER));   
            OS_MEMSET( &qoskey, 0, sizeof(SPM_QOS_TUNNEL_KEY));
            qoskey.type = 1;
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(qoskey.tx_lsp), &(txlsp->key), sizeof(CRTXLSP_KEY) ); 
           // rv = spm_ds_add_tx_lsp_node( difser.dserid, &qoskey, txlsp->dclsp.index, 0, NULL NBB_CCXT);
            if ( rv == 0 )      
            {
                txlsp->mdserid = difser.dserid;
            }
            else
            {
                rv = ADD_CRLSP_TX_DS_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:add ds err  ,rv=%d.\n",
                                                                             __LINE__,rv);
                goto  EXIT_LABEL;
            }
        }
        else if( crlsptxips->oper_diff_serv == ATG_DCI_OPER_DEL) 
        {
            NBB_TRC_FLOW((NBB_FORMAT "Del LSP TX Error: difs data "));
            crlsptxips->diff_serv_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
        if( crlsptxips->oper_prot_diff_serv == ATG_DCI_OPER_ADD) 
        {
        
        }
        else if( crlsptxips->oper_prot_diff_serv == ATG_DCI_OPER_DEL) 
        {

        }
        if( crlsptxips->oper_up_tnnl_qos == ATG_DCI_OPER_ADD) 
        {
            crlsptxips->up_tnnl_qos_return_code = ATG_DCI_RC_OK;
            if(( crlsptxips->oper_basic != ATG_DCI_OPER_ADD))
            {
                if ( node == NULL )                             
                {
                    rv = ADD_CRLSP_TX_TUNL_QOS_NODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:tunl qos txlsp node null  ,rv=%d.\n",
                                                                             __LINE__,rv);
                    goto   EXIT_LABEL;
                }
                if( node->data == NULL )
                {
                    rv = ADD_CRLSP_TX_TUNL_QOS_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:tunl qos node data null  ,rv=%d.\n",
                                                                             __LINE__,rv);  
                    goto  EXIT_LABEL;
                }
                txlsp = (CRTXLSP *)(node->data);
            }
            if( txlsp == NULL ) 
            {
                rv = ADD_CRLSP_TX_TUNL_QOS_TXLSP_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:tunl qos txlsp null  ,rv=%d.\n",
                                                                             __LINE__,rv);
                goto  EXIT_LABEL;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(crlsptxips, &crlsptxips->up_tnnl_qos_data);
            if (ucdata == NULL)
            {
                rv = ADD_CRLSP_TX_TUNL_QOS_IPS_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:tunl qos ips data null  ,rv=%d.\n",
                                                                             __LINE__,rv);
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &qoskey, 0, sizeof(SPM_QOS_TUNNEL_KEY));
            qoskey.flag = 1;
            qoskey.type = 1;
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(qoskey.tx_lsp), &txlspkey, sizeof(CRTXLSP_KEY) ); 
            
            OS_MEMSET( &upqos, 0, sizeof(CRTXLSP_UPQOS));
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &upqos, ucdata, sizeof(CRTXLSP_UPQOS)); 
            OS_MEMCPY( &txupqos, &upqos, sizeof( ATG_DCI_LSP_TX_UP_TNNL_QOS ) );
           // rv = spm_hqos_add_lsp_node( txlsp->slot, txlsp->port, &qoskey, &txupqos, nhid NBB_CCXT);
            if ( rv == 0 )      
            {
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(txlsp->upqos), &upqos, sizeof(CRTXLSP_UPQOS));    
            }
            else
            {
                rv = ADD_CRLSP_TX_TUNL_QOS_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add txlsp:add tunl qos err  ,rv=%d.\n",
                                                                             __LINE__,rv);
                goto  EXIT_LABEL;
            }
        }
        else if( crlsptxips->oper_up_tnnl_qos == ATG_DCI_OPER_DEL) 
        {

        }
        if( crlsptxips->oper_wrapping == ATG_DCI_OPER_ADD) 
        {
        
        }
        else if( crlsptxips->oper_wrapping == ATG_DCI_OPER_DEL) 
        {

        }
    }
    EXIT_LABEL:
        
    NBB_TRC_EXIT();

    /*程序异常，记录错误条目的key值*/
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d tx:err in=%x.egr=%x.tunl=%d.lspid=%x\n",
                                           __LINE__,txlspkey.ingress,txlspkey.egress,txlspkey.tunnelid,txlspkey.lspid);
    }
}

#endif






