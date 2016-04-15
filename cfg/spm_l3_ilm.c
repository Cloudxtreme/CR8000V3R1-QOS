
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
ILM配置块
********************/
#ifdef ILM_T

/*ilm模块中的打印设置*/
extern unsigned char l3_ilm_cfg_print_setting;

/******************************************************************************
 * FunctionName 	: 	spm_l3_ilmhashkey
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_ULONG spm_l3_ilmhashkey(NBB_VOID *keyarg NBB_CCXT_T NBB_CXT)
{
    if( keyarg == NULL)
    {
        return 0;
    }
    else
    {
        return spm_hash_algorithm(keyarg, ILMKEYLGTH, 0 NBB_CCXT);
    }
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_ilmhashcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_ULONG spm_l3_ilmhashcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    ILM_KEY *lsp_key1;
    ILM_KEY *lsp_key2;

    lsp_key1 = (ILM_KEY *)(keyarg1);
    lsp_key2 = (ILM_KEY *)(keyarg2);

    if( lsp_key1->inlabel == lsp_key2->inlabel)
    {
        return 1;
    }

    return 0;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_ilmhashtableinit
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_ilmhashtableinit( NBB_ULONG num NBB_CCXT_T NBB_CXT)
{

    SHARED.ilmhash = (SHASH_TABLE *)
                                spm_hashtab_create( num , spm_l3_ilmhashkey, spm_l3_ilmhashcomp NBB_CCXT);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_ILMprocess
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_ILMprocess( ATG_DCI_SET_ILM* ilmips  NBB_CCXT_T NBB_CXT)
{

    NBB_INT         rv = 100;
    NBB_INT         rv2 = 100;

    SHASH_NODE      *node = NULL, *dnode = NULL;

    NBB_BYTE        *ucdata = NULL;

    NBB_BYTE        i;

    NBB_BYTE        flag = 0;

    ILM_KEY         ilmkey;

    ILM_NHOP        newhop[8];

    ILM_TAB         *ilms = NULL;

    ATG_DCI_ILM_NXHOP_DATA      *nexthp = NULL;
    
    ILM_BASE        base;

    L3_RX_PW_T      rxpw;

     NBB_ULONG      posid = 0;

    RX_LSP_T        lspt;

    SPM_PORT_INFO_CB    portfo;

    ILM_DIFSER      difser;
    SPM_QOS_ILM_KEY qosilmkey;

    NBB_USHORT      frrid = 0;
    NBB_USHORT      ecmpid = 0;

    NBB_CHAR          *errorlog = NULL;
    NBB_ULONG        str_len = 0;
    
    NBB_TRC_ENTRY("spm_l3_ILMprocess");

  
    NBB_ASSERT(ilmips != NULL);

    if(NULL == ilmips)
    {
        goto EXIT_LABEL;
    }

    ilmips->return_code = ATG_DCI_RC_OK;

    OS_MEMSET( &ilmkey, 0, sizeof( ILM_KEY ) );
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &ilmkey, &(ilmips->key), sizeof( ILM_KEY ) );

    NBB_TRC_DETAIL((NBB_FORMAT "ILM  key inlabel =%x\n",ilmkey.inlabel));

    /*操作之前，查看本地数据是否存在*/
    node =  (SHASH_NODE *)spm_hashtab_search( SHARED.ilmhash, &ilmkey NBB_CCXT);    
    
    if( ilmips->delete_struct == 1) /*  删除*/
    {
        if (l3_ilm_cfg_print_setting == SPM_PRINT_CFG)
        {
            /*  删除整个条目*/
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "del ilm lable=%d \n", ilmkey.inlabel);
        }
        if( node == NULL )
        {
            rv = DEL_ILM_NODE_NULL;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del  ilm:node null ,rv=%d.\n",
                                                                               __LINE__,rv);
            ilmips->return_code = ATG_DCI_RC_OK;
            rv = 0;
            goto  EXIT_LABEL;
        }   
        if( node->data == NULL )
        {
            rv = DEL_ILM_NODE_DATA_NULL;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del  ilm:node data null ,rv=%d.\n",
                                                                               __LINE__,rv);
            ilmips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto  EXIT_LABEL;
        } 
            ilms = (ILM_TAB *)(node->data);
            if( ilms->frrid != 0 )
            {
                rv = spm_l3_ilm_del_drv_frrgroup(ilms);
                if( rv != 0)   
                {
                    rv = DEL_ILM_DRV_FRR_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del  ilm:drv frr err ,rv=%d.\n",
                                                                               __LINE__,rv);
                    ilms->return_code = rv;
                }
            }
            if( ilms->ecmpid != 0 )
            {
                rv = spm_l3_ilm_del_drv_ecmpid(ilms);
                if( rv != 0)   
                {
                    rv = DEL_ILM_DRV_ECMP_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del  ilm:drv ECMP err ,rv=%d.\n",
                                                                               __LINE__,rv);
                    ilms->return_code = rv;
                }
            }
            if( ilms->base.labelaction == 3 )
            {
                rv = spm_l3_ilm_del_drv_rxpw(ilms);
                if( rv != 0) 
                {
                    rv = DEL_ILM_DRV_L3RXPW_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del  ilm:drv rxpw err ,rv=%d.\n",
                                                                           __LINE__,rv);
                    ilms->return_code = rv;
                
                    goto  EXIT_LABEL;
                }
            }
            else
            {
                rv = spm_l3_ilm_del_drv_rxlsp(ilms); 
                if( rv != 0) 
                {
                    rv = DEL_ILM_DRV_RXLSP_ERR;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del  ilm:drv RXLSP err ,rv=%d.\n",
                                                                           __LINE__,rv);
                    ilms->return_code = rv;
                    goto  EXIT_LABEL;
                }
            }

            if( rv == 0)      /* 删除成功，删除hash表对应的配置*/
            {
                ilms = (ILM_TAB *)(node->data);
                spm_hashtab_del( SHARED.ilmhash, &ilmkey NBB_CCXT);
                NBB_MM_FREE( ilms, MEM_SPM_ILM_TAB_CB);
            }
    }
    else if( ilmips->delete_struct == 0) /* 增加*/
    {
        if( ilmips->oper_basic == ATG_DCI_OPER_ADD) /* 增加*/
        {
            ilmips->basic_return_code = ATG_DCI_RC_OK;
            
             /*操作之前，查看本地数据是否存在*/
            if( node == NULL )
            {
                if (l3_ilm_cfg_print_setting == SPM_PRINT_CFG)
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "add ilm lable=%d \n", ilmkey.inlabel);
                }
                ilms = (ILM_TAB *)NBB_MM_ALLOC(sizeof(ILM_TAB), NBB_NORETRY_ACT, MEM_SPM_ILM_TAB_CB);

                if ( ilms == NULL )
                {
                    rv = ADD_ILM_MALLOC_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:ilm data null ,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto  EXIT_LABEL;
                } 
                NBB_MEMSET(ilms, 0, sizeof(ILM_TAB));
                flag = 1;
                NBB_TRC_FLOW((NBB_FORMAT "Add ILM new node"));
            }
            else
            {
                if (l3_ilm_cfg_print_setting == SPM_PRINT_CFG)
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "upd ilm lable=%d \n", ilmkey.inlabel);
                }
                if( node->data == NULL )
                {
                    rv = ADD_ILM_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:node data null ,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto  EXIT_LABEL;
                }
                flag = 2;
                ilms = (ILM_TAB *)(node->data);
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(ilmips, &ilmips->basic_data); 
            if (ucdata == NULL)
            {
                rv = ADD_ILM_IPS_BASIC_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:ips basic data null ,rv=%d.\n",
                                                                               __LINE__,rv);
                
                ilms->return_code = rv;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &base, 0, sizeof( ILM_BASE ) );
            NBB_MEMCPY( &base, ucdata, sizeof( ILM_BASE ) );
            NBB_MEMCPY( &(ilms->key), &(ilmips->key), sizeof( ILM_KEY ) );
                
            if( flag == 1)
            {
                NBB_MEMCPY( &(ilms->base), &base, sizeof( ILM_BASE ) );
                spm_hashtab_insert ( SHARED.ilmhash, ilms->key, ilms NBB_CCXT);
            }
            if( flag == 2) 
            {
                if( ilms->base.labelaction == base.labelaction ) 
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "Add ILM update action =%d ", base.labelaction));
                }
                else
                {
                    if( ilms->base.labelaction == 3 )
                    {
                        /*删除驱动rxpw资源*/
                        rv = spm_l3_ilm_del_drv_rxpw(ilms);
                        if(rv != 0)
                        {
                            rv = ADD_ILM_DEL_DRV_L3RXPW_ERR;
                            ilms->return_code = rv;
                        }
                        
                    }
                    else
                    {
                        rv = spm_l3_ilm_del_drv_frrgroup(ilms);
                        if(rv != 0)
                        {
                            rv = ADD_ILM_DEL_DRV_FRR_ERR;
                            ilms->return_code = rv;
                        }
                        
                        rv = spm_l3_ilm_del_drv_ecmpid(ilms);
                        if(rv != 0)
                        {
                            rv = ADD_ILM_DEL_DRV_ECMP_ERR;
                            ilms->return_code = rv;
                        }
                        rv = spm_l3_ilm_del_drv_rxlsp(ilms);
                        if(rv != 0)
                        {
                            rv = ADD_ILM_DEL_DRV_LSP_ERR;
                            ilms->return_code = rv;
                        }
                    }
                }
            }
            if( base.labelaction == 2)    
            {
                rv = spm_l3_ilm_create_drv_rxlsp_pop(ilms);
                if(rv != 0)
                {
                    rv = ADD_ILM_DRV_LSP_IPOP_ERR;
                    ilms->return_code = rv;
                    goto  EXIT_LABEL;
                }
            }
            else  if( base.labelaction == 3)  
            {
               rv = spm_l3_ilm_create_drv_rxpw(ilms);
               if(rv != 0)
               {
                   rv = ADD_ILM_DRV_L3RXPW_ERR;
                   ilms->return_code = rv;
                   goto  EXIT_LABEL;
               }
            }
            else if( base.labelaction == 1)  
            {
                if( base.nextype == 1)
                {
                   rv = spm_l3_ilm_create_drv_frrgroup(ilms);
                   if(rv != 0)
                   {
                       rv = ADD_ILM_DRV_FRR_ERR;
                       ilms->return_code = rv;
                       goto  EXIT_LABEL;
                   }
                }
                else if( base.nextype == 2)
                {
                    rv = spm_l3_ilm_create_drv_ecmp(ilms);
                    if(rv != 0)
                    {
                        rv = ADD_ILM_DRV_ECMP_ERR;
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                    
                    if( ilms->frrid != 0 )
                    {
                        rv = spm_l3_ilm_del_drv_frrgroup(ilms);
                        if( rv != 0)   
                        {
                            rv = ADD_ILM_DEL_DRV_FRR_ERR;
                            ilms->return_code = rv;
                            goto  EXIT_LABEL;
                        }     
                    }
                }
                else
                { 
                    rv = spm_l3_ilm_del_drv_frrgroup(ilms);
                    if( rv != 0)   
                    {
                        rv = ADD_ILM_DEL_DRV_FRR_ERR;
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                
                                      
                }
                posid = ilms->posid;
            }
            
            if( rv == 0) /* 成功*/   
            {  
                ilms->ntype = ilms->base.nextype; 
                NBB_MEMCPY( &(ilms->base), &base, sizeof( ILM_BASE ) );
            }
        }
        else     if( ilmips->oper_basic == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del  ilm: basic data "));
            ilmips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
        if( ilmips->oper_nxhop == ATG_DCI_OPER_UPDATE) /* 增加*/   
        {
            ilmips->nxhop_return_code = ATG_DCI_RC_OK;
            if(( ilmips->oper_basic != ATG_DCI_OPER_ADD))
            {
                if ( node == NULL )                             
                {
                    rv = ADD_ILM_NEXTHOP_NODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop node null,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto   EXIT_LABEL;
                }
                if( node->data == NULL )
                {
                    rv = ADD_ILM_NEXTHOP_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop node data null,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto  EXIT_LABEL;
                } 
                ilms = (ILM_TAB *)(node->data);
            }
            if( ilms == NULL )  
            {
                rv = ADD_ILM_NEXTHOP_ILM_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop ilm null,rv=%d.\n",
                                                                               __LINE__,rv);
                goto  EXIT_LABEL;
            }
            if( ilms->base.labelaction != 1)  
            {
                rv = ADD_ILM_NEXTHOP_LABEL_ACTION_ERR;                
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop lable action,rv=%d.\n",
                                                                               __LINE__,rv);
                ilms->return_code = rv;
                goto  EXIT_LABEL;
            
            }
            ilms->nhopnum = ilmips->nxhop_num;
            NBB_TRC_DETAIL((NBB_FORMAT "Add ILM nxhop_num =%d ", ilms->nhopnum));
            if (ilms->nhopnum > 4)
            {
                rv = ADD_ILM_NEXTHOP_NUM_EXC;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop num>4,rv=%d.\n",
                                                                               __LINE__,rv);
                ilms->return_code = rv;
                goto  EXIT_LABEL;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(ilmips, &ilmips->nxhop_data); 
            if (ucdata == NULL)
            {
                if ( ilms->nhopnum == 0)
                {
                    if (flag == 2)
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ilm nexthop null label=%x \n", 
                                                    __LINE__,ilmkey.inlabel);
                    }
                    rv = 0;
                    goto  EXIT_LABEL;
                }

                /****/
                rv = ADD_ILM_NEXTHOP_IPS_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop ips null,rv=%d.\n",
                                                                               __LINE__,rv);
                ilms->return_code = rv;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &newhop[0], 0, 8 * sizeof( ILM_NHOP ) );
            for ( i = 0; i < ilms->nhopnum; i++)
            {
                nexthp = (ATG_DCI_ILM_NXHOP_DATA *)(ucdata + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_ILM_NXHOP_DATA))) * i);
                OS_MEMSET( &(newhop[i]), 0, sizeof(ATG_DCI_ILM_NXHOP_DATA) );
                
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(newhop[i]), nexthp, sizeof(ATG_DCI_ILM_NXHOP_DATA) );
            }
            if( ilms->base.nextype == 1 )
            {
                if ( ilms->nhopnum == 2)
                {
                    if ( newhop[0].nextypeb == 2)
                    {
                        //coverity[bad_sizeof]
                        NBB_MEMCPY( &(newhop[5]), &(newhop[0]), sizeof(ATG_DCI_ILM_NXHOP_DATA) );
                        
                        //coverity[bad_sizeof]
                        NBB_MEMCPY( &(newhop[0]), &(newhop[1]), sizeof(ATG_DCI_ILM_NXHOP_DATA) );
                        
                        //coverity[bad_sizeof]
                        NBB_MEMCPY( &(newhop[1]), &(newhop[5]), sizeof(ATG_DCI_ILM_NXHOP_DATA) );
                    }
                }
                else if ( ilms->nhopnum == 1)
                {
                    ilms->base.nextype = 0;             
                }
            }

            /*先保存下一跳配置*/
            NBB_MEMCPY( &(ilms->nexthop[0]),  &newhop[0],  8 * sizeof( ILM_NHOP ) );
            if(( ilms->ntype == ilms->base.nextype ) || ( ilms->posid == 0 ))
            {
                /*LDP ECMP*/
                if( ilms->base.nextype == 2 )
                {
                    for ( i = 0; i < ilms->nhopnum; i++)
                    {
                        OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                        rv = spm_get_portid_from_logical_port_index(newhop[i].nextport, &portfo NBB_CCXT);
                        if( rv == 0)     
                        {
                            ilms->portinfo[i].slot = portfo.slot_id;
                            ilms->portinfo[i].port_id = portfo.port_id;
                            ilms->portinfo[i].svlan = portfo.svlan ;
                        }
                        else
                        {
                            rv = ADD_ILM_NEXTHOP_GET_L2PORT_ERR;
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop get l2port,rv=%d.\n",
                                                                               __LINE__,rv); 
                            ilms->return_code = rv;
                            goto  EXIT_LABEL;
                        }
                    }
                    rv = spm_l3_ilm_mod_drv_rxlspecmp(ilms);
                    if(rv != 0)
                    {
                        rv = ADD_ILM_NEXTHOP_DRV_LSP_ECMP_ERR;
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }

                }
                else
                {
                    OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                    rv = spm_get_portid_from_logical_port_index(newhop[0].nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    { 
                        ilms->portinfo[0].slot = portfo.slot_id;
                        ilms->portinfo[0].port_id = portfo.port_id;
                        ilms->portinfo[0].svlan = portfo.svlan;
                    }
                    else
                    {
                        rv = ADD_ILM_NEXTHOP_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop get l2port,rv=%d.\n",
                                                                               __LINE__,rv);
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                    
                    /*rxlsp下一跳无保护*/
                    if( ilms->base.nextype == 0 )
                    {
                        rv = spm_l3_ilm_create_drv_rxlsp(ilms);
                        if(rv != 0)
                        {
                            rv = ADD_ILM_NEXTHOP_DRV_LSP_ERR;
                            ilms->return_code = rv;
                            goto  EXIT_LABEL;
                            
                        }
                    }
                    
                    /*下一跳类型 LDP FRR*/
                    else if( ilms->base.nextype == 1 )
                    {
                        OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                        rv = spm_get_portid_from_logical_port_index(newhop[1].nextport, &portfo NBB_CCXT);
                        if( rv == 0)     
                        {
                            ilms->portinfo[1].slot = portfo.slot_id;
                            ilms->portinfo[1].port_id = portfo.port_id;
                            ilms->portinfo[1].svlan = portfo.svlan;
                            
                        }
                        else
                        {
                            rv = ADD_ILM_NEXTHOP_GET_L2PORT_ERR;
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop get l2port,rv=%d.\n",
                                                                               __LINE__,rv);
                            ilms->return_code = rv;
                            goto  EXIT_LABEL;
                       }

                       rv = spm_l3_ilm_create_drv_ldpfrr(ilms);
                       if(rv != 0)
                       {
                            rv = ADD_ILM_RXLSP_LDP_FRR_ERR;
                            ilms->return_code = rv;
                            goto  EXIT_LABEL;
                       }
                        
                       
                        
                    }
                }
            }
            else
            {
                if( ilms->ntype == 1 )
                {
                    rv = spm_l3_ilm_del_drv_rxlspprot(ilms);
                    if(rv != 0)
                    {
                        rv = ADD_ILM_NEXTHOP_DRV_DEL_LSPP_ERR;
                        ilms->return_code = rv;
                    }
                }
                else if( ilms->ntype == 2 )
                {
                    rv = spm_l3_ilm_del_drv_rxlspecmp(ilms);
                    if(rv != 0)
                    {
                        rv = ADD_ILM_NEXTHOP_DRV_DEL_ECMP_ERR;
                        ilms->return_code = rv;
                    }
                }
                if( ilms->base.nextype == 0 )
                {
                    OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                    rv = spm_get_portid_from_logical_port_index( newhop[0].nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    {
                        ilms->portinfo[0].slot = portfo.slot_id;
                        ilms->portinfo[0].port_id = portfo.port_id;
                        ilms->portinfo[0].svlan = portfo.svlan;
                    }
                    else
                    {
                        rv = ADD_ILM_NEXTHOP_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop get l2port,rv=%d.\n",
                                                                               __LINE__,rv);
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                    rv = spm_l3_ilm_create_drv_rxlsp(ilms);
                    if(rv != 0)
                    {
                        rv = ADD_ILM_NEXTHOP_DRV_LSP_ERR;
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }     
                    
                }
                else if( ilms->base.nextype == 1 )
                {
                    OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                    rv = spm_get_portid_from_logical_port_index( newhop[0].nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    {
                        ilms->portinfo[0].slot = portfo.slot_id;
                        ilms->portinfo[0].port_id = portfo.port_id;
                        ilms->portinfo[0].svlan = portfo.svlan;
                    }
                    else
                    {
                        rv = ADD_ILM_NEXTHOP_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop get l2port,rv=%d.\n",
                                                                               __LINE__,rv);
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }

                    OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                    rv = spm_get_portid_from_logical_port_index( newhop[1].nextport, &portfo NBB_CCXT);
                    if( rv == 0)     
                    {
                        ilms->portinfo[1].slot = portfo.slot_id;
                        ilms->portinfo[1].port_id = portfo.port_id;
                        ilms->portinfo[1].svlan = portfo.svlan;
                    }
                    else
                    {
                        rv = ADD_ILM_NEXTHOP_GET_L2PORT_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop get l2port,rv=%d.\n",
                                                                               __LINE__,rv);
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                    rv = spm_l3_ilm_mod_drv_ldpfrr(ilms);
                    if(rv != 0)
                    {
                        rv = ADD_ILM_NEXTHOP_DRV_MODLSP_ERR;
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else if( ilms->base.nextype == 2 )
                {
                    for ( i = 0; i < ilms->nhopnum; i++)
                    {
                        OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
                        rv = spm_get_portid_from_logical_port_index(newhop[i].nextport, &portfo NBB_CCXT);
                        if( rv == 0)     
                        {
                            ilms->portinfo[i].slot = portfo.slot_id;
                            ilms->portinfo[i].port_id = portfo.port_id;
                            ilms->portinfo[i].svlan = portfo.svlan ;
                        }
                        else
                        {
                            rv = ADD_ILM_NEXTHOP_GET_L2PORT_ERR;
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:nexthop get l2port,rv=%d.\n",
                                                                               __LINE__,rv);  
                            ilms->return_code = rv;
                            goto  EXIT_LABEL;
                        }
                    }

                    /*LDP ECMP*/
                    rv = spm_l3_ilm_add_drv_rxlspecmp(ilms);
                    if(rv != 0)
                    {
                        rv = ADD_ILM_NEXTHOP_DRV_LSP_ECMP_ERR;
                        ilms->return_code = rv;
                        goto  EXIT_LABEL;
                    } 
                }

                /*保存上次的配置的下一跳保护类型*/
                ilms->ntype = ilms->base.nextype;
            }
                      
        }
        else  if( ilmips->oper_nxhop == ATG_DCI_OPER_DEL )   /*  删除子TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del  ilm:nexthop data "));
            ilmips->nxhop_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
        if( ilmips->oper_diff_serv == ATG_DCI_OPER_ADD) 
        {
            ilmips->diff_serv_return_code[0] = ATG_DCI_RC_OK;
            if(( ilmips->oper_basic != ATG_DCI_OPER_ADD))
            {
                if ( node == NULL )                             
                {
                    rv = ADD_ILM_DS_NODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:ds node null,rv=%d.\n",
                                                                               __LINE__,rv);
                    ilmips->diff_serv_return_code[0] = ATG_DCI_RC_UNSUCCESSFUL;
                    goto   EXIT_LABEL;
                }
                if( node->data == NULL )
                {
                    rv = ADD_ILM_DS_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:ds node data null,rv=%d.\n",
                                                                               __LINE__,rv);
                    ilmips->diff_serv_return_code[0] = ATG_DCI_RC_UNSUCCESSFUL;
                    goto  EXIT_LABEL;
                } 
                ilms = (ILM_TAB *)(node->data);
            }
            if( ilms == NULL )  
            {
                rv = ADD_ILM_DS_ILM_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:ilm null,rv=%d.\n",
                                                                               __LINE__,rv);
                ilmips->diff_serv_return_code[0] = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(ilmips, &ilmips->diff_serv_data);
            if (ucdata == NULL)
            {
                rv = ADD_ILM_DS_IPS_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:ds ips null,rv=%d.\n",
                                                                               __LINE__,rv);
                ilmips->diff_serv_return_code[0] = ATG_DCI_RC_UNSUCCESSFUL;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &difser, 0, sizeof(ILM_DIFSER));
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &difser, ucdata, sizeof(ILM_DIFSER));
            qosilmkey.inlabel =  ilms->key.inlabel;
            qosilmkey.flag = ilms->base.labelaction;
           // rv = spm_ds_add_ilm_node(&difser, &qosilmkey, ilms->posid, ilms->nexthop[0].action, 0, NULL NBB_CCXT);
            if ( rv == 0 )      
            {
                ilms->dserid = difser.dserid;
            }
            else
            {
                rv = ADD_ILM_DS_DATA_ERR;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lilm:ds err,rv=%d.\n",
                                                                               __LINE__,rv);  
                goto  EXIT_LABEL;
            }
        }
        else if( ilmips->oper_diff_serv == ATG_DCI_OPER_DEL) 
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del  ilm:difs data"));
            ilmips->diff_serv_return_code[0] = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
    }
    EXIT_LABEL:
    
    NBB_TRC_EXIT();

    /*程序异常时，记录错误条目的key值*/
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d ilm:err inlabel=%d\n", __LINE__,ilmkey.inlabel);
    }
}




#endif
