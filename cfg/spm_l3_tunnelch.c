
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
���ѡ�����ÿ�
********************/
#ifdef TUNNEL_CHOOSE

/*tunnelchooseģ���еĴ�ӡ����*/
extern unsigned char l3_tunnelch_cfg_print_setting;

/******************************************************************************
 * FunctionName 	: 	spm_l3_tunnelchhashkey
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_ULONG spm_l3_tunnelchhashkey(NBB_VOID *keyarg NBB_CCXT_T NBB_CXT)
{
    if( keyarg == NULL)
    {
        return 0;
    }
    else
    {
        return spm_hash_algorithm(keyarg, TUNNELCHKEYLGTH, 0 NBB_CCXT);
    }
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_tunnelchhashcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_ULONG spm_l3_tunnelchhashcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    TUNNELCH_KEY  *key1;
    TUNNELCH_KEY  *key2;

    key1 = (TUNNELCH_KEY *)(keyarg1);
    key2 = (TUNNELCH_KEY *)(keyarg2);

    if( key1->vrfid == key2->vrfid
    && key1->peerip == key2->peerip)
    {
        return 1;
    }

    return 0;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_tunnelchhashtableinit
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_tunnelchhashtableinit( NBB_ULONG num NBB_CCXT_T NBB_CXT)
{

    SHARED.tunnelchhash = (SHASH_TABLE *)
                    spm_hashtab_create( num , spm_l3_tunnelchhashkey,spm_l3_tunnelchhashcomp NBB_CCXT);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_routecomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_routecomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    VRFUROUTE_KEY *key1 = (VRFUROUTE_KEY *)keyarg1;
    VRFUROUTE_KEY *key2 = (VRFUROUTE_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_vrfuroutecomp");

    rv = NBB_MEMCMP(&(key1->dip[0]), &(key2->dip[0]),16 );
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
 * FunctionName 	: 	spm_l3_alloc_pwcite_node
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_alloc_pwcite_node(TUNNELCH_KEY *pwkey,LSPPROT_KEY *lspprotkey NBB_CCXT_T NBB_CXT)
{
    PWNODE *pwnode;
    LSPRSVP *lsprs;
    NBB_BOOL rv;

    NBB_TRC_ENTRY("spm_l3_alloc_pwcite_node");

    lsprs = (LSPRSVP *)AVLL_FIND(SHARED.rsvpcite, lspprotkey);
    if (lsprs == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find LSPRSVP"));
        goto EXIT_LABEL;
    }
    pwnode = (PWNODE *)AVLL_FIND(lsprs->pw_tree, pwkey);
    if (pwnode != NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT " PWNODE exist"));
        goto EXIT_LABEL;
    }
    pwnode = (PWNODE *)NBB_MM_ALLOC(sizeof(PWNODE),NBB_NORETRY_ACT,MEM_SPM_TUNNELCH_PWNODE_CB);
    if (pwnode == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to allocate PWNODE"));
        goto EXIT_LABEL;
    }
    NBB_TRC_DETAIL((NBB_FORMAT "Newalloc  for pwnode vrfid=%x   peerip=%x",
                                    pwkey->vrfid, pwkey->peerip));

    NBB_MEMSET(pwnode, 0, sizeof(PWNODE));
    AVLL_INIT_NODE(pwnode->pw_node);
    AVLL_INIT_TREE(pwnode->rout_tree,
                        spm_l3_routecomp,
                        NBB_OFFSETOF(ROUTNODE, key),/*lint !e413 */
                        NBB_OFFSETOF(ROUTNODE, rout_node));/*lint !e413 */
    pwnode->lspcite = NULL;
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &(pwnode->key), pwkey, sizeof( TUNNELCH_KEY) );
    
     if (lsprs != NULL)
     {
        rv = AVLL_INSERT(lsprs->pw_tree, lsprs->lsp_node);
        lsprs->pwnum += 1;
        pwnode->lspcite = lsprs;
        NBB_TRC_DETAIL((NBB_FORMAT "lsprsvpcite AVLL_INSERT  rv= %B", rv));
     }

    
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return;

} 

/******************************************************************************
 * FunctionName 	: 	spm_l3_free_pwcite_node
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_free_pwcite_node(TUNNELCH_KEY *pwkey,LSPPROT_KEY *lspprotkey NBB_CCXT_T NBB_CXT)
{
    PWNODE *pwnode;
    LSPRSVP *lsprs;
    ROUTNODE *rou_node;

    NBB_TRC_ENTRY("spm_l3_free_pwcite_node");

    NBB_TRC_DETAIL((NBB_FORMAT "free  for pwnode vrfid=%x   peerip=%x",
                                    pwkey->vrfid, pwkey->peerip));

    lsprs = (LSPRSVP *)AVLL_FIND(SHARED.rsvpcite, lspprotkey);
    if (lsprs == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find LSPRSVP"));
        goto EXIT_LABEL;
    }
    pwnode = (PWNODE *)AVLL_FIND(lsprs->pw_tree, pwkey);
    if (pwnode == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find PWNODE"));
        goto EXIT_LABEL;
    }
    
    /******************************************************/
    /* First, free off all of the ROUTE nodes.              */
    /******************************************************/
    rou_node = (ROUTNODE *)AVLL_FIRST(pwnode->rout_tree);
    while (rou_node != NULL)
    {
        AVLL_DELETE(pwnode->rout_tree, rou_node->rout_node);
        rou_node->pwcite = NULL;
        NBB_MM_FREE(rou_node, MEM_SPM_VRFUROUTE_ROUTNODE_CB);

        rou_node = (ROUTNODE *)AVLL_FIRST(pwnode->rout_tree);
    }
    AVLL_DELETE(lsprs->pw_tree, pwnode->pw_node);
    lsprs->pwnum = lsprs->pwnum - 1;
    NBB_MM_FREE(pwnode, MEM_SPM_TUNNELCH_PWNODE_CB);

    EXIT_LABEL:
    NBB_TRC_EXIT();
    return;

}

extern NBB_INT spm_l3_pwcomp(NBB_VOID *, NBB_VOID *);



/******************************************************************************
 * FunctionName 	: 	spm_l3_tunnelchprocess
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_tunnelchprocess( ATG_DCI_SET_TNNL_SELECT * tunnelchips NBB_CCXT_T NBB_CXT)
{

    NBB_INT             rv = 100;
    NBB_INT             rv2 = 100;

    SHASH_NODE          *node = NULL, *dnode = NULL;

    NBB_BYTE            *ucdata = NULL;

    NBB_BYTE            flag = 0;

    LSPPROT_KEY         tunnelkey;

    TUNNELCH_KEY        tunnelskey;

    TUNNELCH_BASE       base;

    NBB_ULONG           nextindex = 0;

    TUNNELCH            *tunnels = NULL;

    PWTXNODE            *pwnodes = NULL;

    L3_TX_PW_T          txpw;
    L3_TX_PW_T          temp_txpw;

    NBB_CHAR            *errorlog = NULL;
    NBB_ULONG           str_len = 0;

    NBB_TRC_ENTRY("spm_l3_tunnelchprocess");

    NBB_ASSERT(tunnelchips != NULL);
        
    if(NULL == tunnelchips)
    {
        goto EXIT_LABEL;
    }

    tunnelchips->return_code = ATG_DCI_RC_OK;
        
    //NBB_MEMCPY( &tunnelskey, &(tunnelchips->key), sizeof( TUNNELCH_KEY ) );

    tunnelskey.res = 0;
    tunnelskey.vrfid = tunnelchips->key.vrf_id;
    tunnelskey.peerip = tunnelchips->key.peer_id;
    
    if( tunnelskey.peerip == 0 )
    {
        NBB_PRINTF("TUNNELCH data error peerip = 0 \n");
        tunnelchips->return_code = ATG_DCI_RC_UNSUCCESSFUL;  
        goto  EXIT_LABEL;
    }

    if( tunnelchips->delete_struct == 1)
    {
        if (l3_tunnelch_cfg_print_setting == SPM_PRINT_CFG)
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "del tunsel vrf.peer =%d.%x \n", tunnelskey.vrfid, tunnelskey.peerip);
        }
    }
    else if( tunnelchips->delete_struct == 0)
    {
        if (l3_tunnelch_cfg_print_setting == SPM_PRINT_CFG)
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "add tunsel vrf.peer =%d.%x \n", tunnelskey.vrfid, tunnelskey.peerip);
        }
    }
    
    /*����֮ǰ���鿴���������Ƿ����*/
    node =  (SHASH_NODE *)spm_hashtab_search( SHARED.tunnelchhash, &tunnelskey NBB_CCXT);    
    
    if( tunnelchips->delete_struct == 1) /*  ɾ��*/
    {
         tunnelchips->return_code = ATG_DCI_RC_OK;
         
        /*  ɾ��������Ŀ*/
        if( node == NULL )
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del  tunnel: node null"));
            tunnelchips->return_code = ATG_DCI_RC_OK;  
            rv = 0;
            goto  EXIT_LABEL;
        }
        if( node->data == NULL )
        {
            rv = DEL_TUNNELSEL_NODE_DATA_NULL;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del tunnel: node null ,rv=%d.\n",
                                                                               __LINE__,rv);
            tunnelchips->return_code = ATG_DCI_RC_OK;  
            goto  EXIT_LABEL;
        }
        
        /* ɾ���ɹ���ɾ��hash���Ӧ������*/
        tunnels = (TUNNELCH *)(node->data);
        if( tunnels->pwnum == 0) 
        {
            OS_MEMSET( &tunnelkey, 0, sizeof( LSPPROT_KEY) );
			tunnelkey.ingress = tunnels->base.ingress;
			tunnelkey.egress = tunnels->base.egress;
			tunnelkey.tunnelid = tunnels->base.tunnelid;
			spm_hashtab_del( SHARED.tunnelchhash ,&tunnelskey NBB_CCXT);
            NBB_MM_FREE(tunnels, MEM_SPM_TUNNELCH_CB);

			/*ldp*/
			if(tunnelkey.egress == 0)
			{
                spm_l3_delvrfldp( tunnelskey.peerip, tunnelskey.vrfid NBB_CCXT);  
			}
			else
			{
                spm_l3_delvrfrsvp(tunnelskey.peerip, tunnelskey.vrfid,&tunnelkey NBB_CCXT);
			}
			
        }
        else
        {
            tunnels->delflag = 100;
        }
    }
    else if( tunnelchips->delete_struct == 0) 
    {
        if( tunnelchips->oper_basic == ATG_DCI_OPER_ADD) 
        {
            tunnelchips->basic_return_code = ATG_DCI_RC_OK;
            if( node == NULL )
            {
                tunnels = (TUNNELCH *)NBB_MM_ALLOC(sizeof(TUNNELCH), NBB_NORETRY_ACT, MEM_SPM_TUNNELCH_CB);
                if ( tunnels == NULL )
                {
                    rv = ADD_TUNNELSEL_MALLOC_MEM_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d Add TUNNELCH Error: malloc null ,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto  EXIT_LABEL;
                } 
                NBB_MEMSET(tunnels, 0, sizeof(TUNNELCH));
                AVLL_INIT_TREE(tunnels->pwtree,
                                    spm_l3_pwcomp,
                                    NBB_OFFSETOF(PWTXNODE, key),/*lint !e413 */
                                    NBB_OFFSETOF(PWTXNODE, pw_node));/*lint !e413 */
                flag = 1;
                NBB_TRC_FLOW((NBB_FORMAT "Add TUNNELCH new node"));
            }
            else
            {
                if( node->data == NULL )
                {
                    rv = ADD_TUNNELSEL_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d Add TUNNELCH Error: node data ,rv=%d.\n",
                                                                               __LINE__,rv);
                    goto  EXIT_LABEL;
                }
                flag = 2;
                tunnels = (TUNNELCH *)(node->data);
                tunnels->delflag = 0;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(tunnelchips, &tunnelchips->basic_data); 
            if (ucdata == NULL)
            {
                rv = ADD_TUNNELSEL_IPS_BASIC_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d Add TUNNELCH Error: basic ips null ,rv=%d.\n",
                                                                               __LINE__,rv);
                tunnels->return_code = rv;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &base, 0, sizeof( TUNNELCH_BASE) );
            NBB_MEMCPY( &base, ucdata, sizeof( TUNNELCH_BASE) );
            NBB_MEMCPY( &(tunnels->key), &tunnelskey, sizeof( TUNNELCH_KEY ) );
            NBB_MEMCPY( &(tunnels->base), &base, sizeof( TUNNELCH_BASE) );
            if ( flag == 1 )
            {
                spm_hashtab_insert (SHARED.tunnelchhash ,  &(tunnels->key), tunnels NBB_CCXT);
            }
            if( base.egress == 0 )
            {
                rv = spm_l3_iteration_lookupftn( tunnelskey.peerip, &nextindex NBB_CCXT);
                if ( rv == 0 )
                {
                    //tunnels->tunnelindex = nextindex;
                    NBB_TRC_DETAIL((NBB_FORMAT "Add TUNNELCH lookupftn tunnel =%d ", nextindex));
                }
                else
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d Add TUNNELCH Error: iter ftn ,rv=%d.\n",
                                                                               __LINE__,rv);
                    tunnels->return_code = rv;
                    spm_l3_addvrfldp( tunnelskey.peerip, tunnelskey.vrfid, 0 NBB_CCXT); 
                    goto  EXIT_LABEL;
                }
            }
            else
            {
                tunnelkey.ingress = base.ingress;
                tunnelkey.egress = base.egress;
                tunnelkey.tunnelid = base.tunnelid;
                rv = spm_l3_iteration_lookuptunnel(&tunnelkey , &nextindex NBB_CCXT);
                if ( rv == 0 )
                {
                    //tunnels->tunnelindex = nextindex;
                    NBB_TRC_DETAIL((NBB_FORMAT "Add TUNNELCH lookuptunnel tunnel =%x ", nextindex));
                }
                else
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d Add TUNNELCH Error: iter tunnel ,rv=%d.\n",
                                                                               __LINE__,rv);
                    tunnels->return_code = rv;
                    spm_l3_addvrfrsvp(tunnelskey.peerip,tunnelskey.vrfid,&tunnelkey,0 NBB_CCXT);
                    goto  EXIT_LABEL;
                    
                }
            }
            
            //coverity[dead_error_condition]
            if( rv == 0)      /*�ɹ�*/
            {
                tunnels->tunindex = nextindex;
				/*ldp*/
				if(base.egress == 0)
				{
                    spm_l3_addvrfldp( tunnelskey.peerip, tunnelskey.vrfid, nextindex NBB_CCXT); 
				}
				else
				{
				    spm_l3_addvrfrsvp(tunnelskey.peerip,tunnelskey.vrfid,&tunnelkey,nextindex NBB_CCXT);
				}
                
                if ( flag == 2 )
                {
                    pwnodes = (PWTXNODE *)AVLL_FIRST(tunnels->pwtree);
                    while (pwnodes != NULL)
                    {
                        if( pwnodes->posid != 0 ) 
                        {
                            rv = spm_l3_tunnelch_mod_drv_txpw(pwnodes,tunnels);
                            if(rv != 0)
                            {
                                rv = ADD_TUNNELSEL_DRV_MOD_TXPW_ERR; 
                                tunnels->return_code = rv;
                            }
                        }
                        pwnodes = (PWTXNODE *)AVLL_NEXT( tunnels->pwtree, pwnodes->pw_node);
                    }
             
                }
            }
            else    /*��������ʧ�ܣ���¼����*/
            {
                rv = ADD_TUNNELSEL_ITER_ERR;
                tunnels->return_code = rv;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d Add TUNNELCH Error: iter ,rv=%d.\n",
                                                                               __LINE__,rv); 
                goto  EXIT_LABEL;
            }
        }
        else     if( tunnelchips->oper_basic == ATG_DCI_OPER_DEL) /*  ɾ����TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del  tunnel: basic data "));
            tunnelchips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;
        }
    }
    EXIT_LABEL:

    NBB_TRC_EXIT();
    
     /*�����쳣ʱ����¼������Ŀ��keyֵ*/
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d tunsel:err vrfid=%d.peerip=%x\n", 
                                           __LINE__,tunnelskey.vrfid,tunnelskey.peerip);
    }
}

#endif
