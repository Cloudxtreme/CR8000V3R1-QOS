
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
LSP保护配置块
********************/
#ifdef LSP_PROT

/*RSVP永久1:1保护交叉比对lsp保存*/
CRTXLSP_KEY     oldhwtxlspkey[2];

/******************************************************************************
 * FunctionName 	: 	spm_l3_lspprothashcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_ULONG spm_l3_lspprothashkey(NBB_VOID *keyarg NBB_CCXT_T NBB_CXT)
{
    if( keyarg == NULL)
    {
        return 0;
    }
    else
    {
        return spm_hash_algorithm(keyarg, LSPPROTKEYLGTH, 0 NBB_CCXT);
    }
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_lspprothashcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_ULONG spm_l3_lspprothashcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
    LSPPROT_KEY *key1;
    LSPPROT_KEY *key2;

    key1 = (LSPPROT_KEY *)(keyarg1);
    key2 = (LSPPROT_KEY *)(keyarg2);

    if( key1->ingress == key2->ingress 
    && key1->egress == key2->egress 
    && key1->tunnelid == key2->tunnelid )
    {
        return 1;
    }
    return 0;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_lspprotkeycomp
 * Author		: 	    wjhe
 * CreateDate		:	2015-12-31
 * Description		:   
 * InputParam	:	     
 * OutputParam	:	    no
 * ReturnValue	:	    success/error
 * Relation		:	      
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_lspprotkeycomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{
	LSPPROT_KEY *key1;
	LSPPROT_KEY *key2;
	NBB_INT     rv = 0;

	key1 = (LSPPROT_KEY *)(keyarg1);
	key2 = (LSPPROT_KEY *)(keyarg2);

	rv = compare_ulong(&key1->ingress, &key2->ingress NBB_CCXT);
	if (rv != 0)
	{
		goto EXIT_LABEL;
	}

	rv = compare_ulong(&key1->egress, &key2->egress NBB_CCXT);
	if (rv != 0)
	{
		goto EXIT_LABEL;
	}

	rv = compare_ulong(&key1->tunnelid, &key2->tunnelid NBB_CCXT);
	if (rv != 0)
	{
		goto EXIT_LABEL;
	}
	

	EXIT_LABEL:

	return (rv);
		
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_lspprothashtableinit
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_lspprothashtableinit( NBB_ULONG num NBB_CCXT_T NBB_CXT)
{
    SHARED.lspprothash = (SHASH_TABLE *)
                        spm_hashtab_create( num , spm_l3_lspprothashkey,spm_l3_lspprothashcomp NBB_CCXT);
	
	/*初始化 RSVP FOR LSP 树*/
	AVLL_INIT_TREE(SHARED.rsvplsp_tree,
					spm_l3_lspprotkeycomp,
					NBB_OFFSETOF(RSVPCRLSP, key),/*lint !e413 */
					NBB_OFFSETOF(RSVPCRLSP, rsvp_node));/*lint !e413 */
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_rsvpcitecomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_rsvpcitecomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    LSPPROT_KEY *key1 = (LSPPROT_KEY *)keyarg1;
    LSPPROT_KEY *key2 = (LSPPROT_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_rsvpcitecomp");

    rv = compare_ulong(&key1->ingress, &key2->ingress NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    rv = compare_ulong(&key1->egress, &key2->egress NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }   

    rv = compare_ulong(&key1->tunnelid, &key2->tunnelid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }   
    
    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_rsvpcitetreeinit
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_rsvpcitetreeinit( NBB_CXT_T NBB_CXT)
{
    AVLL_INIT_TREE(SHARED.rsvpcite,
                    spm_l3_rsvpcitecomp,
                    NBB_OFFSETOF(LSPRSVP, key),/*lint !e413 */
                    NBB_OFFSETOF(LSPRSVP, lsp_node));/*lint !e413 */

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_pwcitecomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_pwcitecomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    TUNNELCH_KEY *key1 = (TUNNELCH_KEY *)keyarg1;
    TUNNELCH_KEY *key2 = (TUNNELCH_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_pwcitecomp");

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

/******************************************************************************
 * FunctionName 	: 	spm_l3_alloc_rsvpcite_node
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_alloc_rsvpcite_node(LSPPROT_KEY *lspprotkey NBB_CCXT_T NBB_CXT)
{
    LSPRSVP *lsprs;
    NBB_BOOL rv;

    NBB_TRC_ENTRY("spm_l3_alloc_rsvpcite_node");

    lsprs = (LSPRSVP *)AVLL_FIND(SHARED.rsvpcite, lspprotkey);
    if (lsprs != NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "LSPRSVP Exist"));
        goto EXIT_LABEL;
    }
    lsprs = (LSPRSVP *)NBB_MM_ALLOC(sizeof(LSPRSVP),NBB_NORETRY_ACT,MEM_SPM_LSPPROT_RSVPCITE_CB);
    if (lsprs == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to allocate LSPRSVP"));
        goto EXIT_LABEL;
    }
    NBB_TRC_DETAIL((NBB_FORMAT "Newalloc  for lsprsvpcite ingress=%x   egress=%x   tunnelid=%x",
                                    lspprotkey->ingress, lspprotkey->egress, lspprotkey->tunnelid));

    NBB_MEMSET(lsprs, 0, sizeof(LSPRSVP));
    AVLL_INIT_NODE(lsprs->lsp_node);
    AVLL_INIT_TREE(lsprs->pw_tree,
                        spm_l3_pwcitecomp,
                        NBB_OFFSETOF(PWNODE, key),/*lint !e413 */
                        NBB_OFFSETOF(PWNODE, pw_node));/*lint !e413 */

    //coverity[bad_sizeof]
    NBB_MEMCPY( &(lsprs->key), lspprotkey, sizeof( LSPPROT_KEY) );
    rv = AVLL_INSERT(SHARED.rsvpcite, lsprs->lsp_node);
    NBB_TRC_DETAIL((NBB_FORMAT "lsprsvpcite AVLL_INSERT  rv= %B", rv));
    
    
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return;

} 

/******************************************************************************
 * FunctionName 	: 	spm_l3_free_rsvpcite_node
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_free_rsvpcite_node( LSPPROT_KEY *lspprotkey NBB_CCXT_T NBB_CXT)
{
    LSPRSVP *lsprs;
    PWNODE *pwnode;

    NBB_TRC_ENTRY("spm_l3_free_rsvpcite_node");


    lsprs = (LSPRSVP *)AVLL_FIND(SHARED.rsvpcite, lspprotkey);
    if (lsprs == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find LSPRSVP"));
        goto EXIT_LABEL;
    }
    NBB_TRC_DETAIL((NBB_FORMAT "Free  for lsprsvpcite ingress=%x   egress=%x   tunnelid=%x",
                                lsprs->key.ingress, lsprs->key.egress, lsprs->key.tunnelid));
                                
    /******************************************************/
    /* First, free off all of the PW nodes.              */
    /******************************************************/
    pwnode = (PWNODE *)AVLL_FIRST(lsprs->pw_tree);
    while (pwnode != NULL)
    {
        AVLL_DELETE(lsprs->pw_tree, pwnode->pw_node);
        pwnode->lspcite = NULL;
        NBB_MM_FREE(pwnode, MEM_SPM_TUNNELCH_PWNODE_CB);

        pwnode = (PWNODE *)AVLL_FIRST(lsprs->pw_tree);
    }
    AVLL_DELETE(SHARED.rsvpcite, lsprs->lsp_node);

    NBB_MM_FREE(lsprs, MEM_SPM_LSPPROT_RSVPCITE_CB);

    EXIT_LABEL:
    NBB_TRC_EXIT();
    return;

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_lspprotprocess
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY COMP
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_lspprotprocess( ATG_DCI_SET_LSP_PROT * lspprotips NBB_CCXT_T NBB_CXT)
{

    NBB_INT         rv = 100;
    NBB_INT         rv2 = 100;

    SHASH_NODE      *node = NULL, *dnode = NULL,*txnode = NULL;

    NBB_BYTE        *ucdata = NULL;

    NBB_BYTE        flag = 0;
    NBB_BYTE        reverseflag = 0;
    NBB_BYTE        workstate = 0;

    LSPPROT_KEY     lspprotkey;
    LSPPROT         *lspprot = NULL;

    CRTXLSP_KEY     txlspkey;
    CRTXLSP_KEY     worktxlspkey;
    CRTXLSP_KEY     newtxlspkey[2]; /*注意这两个变量在哪个地方清除为0*/
    CRTXLSP_KEY     hwtxlspkey[2];                   /*下给硬件的表项*/
    CRTXLSP_KEY     mtxlspkey;
    CRTXLSP_KEY     btxlspkey;

    CRTXLSP         *txlsp = NULL;

    LSPPROT_1B1     lsp[2], *lsp1b1 = NULL;

    NBB_ULONG       i = 0;
    NBB_ULONG       moutport = 0;
    NBB_ULONG       boutport = 0;
    NBB_ULONG       mlsp = 0;
    NBB_ULONG       blsp = 0;
    NBB_ULONG       txposid[2] = {0};

    NBB_USHORT      lspnum = 0;

    TUNNEL_T        tunnel;

    SPM_LINEAR_PROTECT_SWITCH       stLps;

    NBB_CHAR            *errorlog = NULL;
    NBB_ULONG           str_len = 0;

    NBB_TRC_ENTRY("spm_l3_lspprotprocess");
    
        NBB_ASSERT(lspprotips != NULL);

    if(NULL == lspprotips)
    {
        goto EXIT_LABEL;
    }

    lspprotips->return_code = ATG_DCI_RC_OK;

    OS_MEMSET( &lsp[0], 0, 2 * sizeof( LSPPROT_1B1 ) );

    OS_MEMSET(&tunnel , 0, sizeof(TUNNEL_T));

    OS_MEMSET( &lspprotkey, 0, sizeof( LSPPROT_KEY ) );
    OS_MEMSET(&hwtxlspkey, 0, 2 * sizeof(CRTXLSP_KEY));    
    OS_MEMSET(&worktxlspkey, 0, sizeof(CRTXLSP_KEY));
    OS_MEMSET(&newtxlspkey, 0, 2 * sizeof(CRTXLSP_KEY));
    OS_MEMSET(&mtxlspkey, 0, sizeof(CRTXLSP_KEY));
    OS_MEMSET(&btxlspkey, 0, sizeof(CRTXLSP_KEY));
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &lspprotkey, &(lspprotips->key), sizeof( LSPPROT_KEY ) );

    if(( lspprotkey.ingress == 0 ) && ( lspprotkey.egress == 0 ) && ( lspprotkey.tunnelid == 0 ))
    {
        NBB_PRINTF(" LSPPROT error key = 0 \n");
        lspprotips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        goto  EXIT_LABEL;
    }

    /*操作之前，查看本地数据是否存在*/
    node =  (SHASH_NODE *)spm_hashtab_search( SHARED.lspprothash, &lspprotkey NBB_CCXT);    
    
    if( lspprotips->delete_struct == 1) /*  删除*/
    {
        /*  删除整个条目*/
        if( node == NULL )
        {
            rv = DEL_LSPPROT_NODE_NULL ;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del lspprot: node null ,rv=%d,\n",
                                                                                    __LINE__,rv);
            lspprotips->return_code = ATG_DCI_RC_OK;
            goto  EXIT_LABEL;
        }
        if( node->data == NULL )
        {
            rv = DEL_LSPPROT_NODE_DATA_NULL;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Del lspprot: node data null ,rv=%d,\n",
                                                                                    __LINE__,rv);
            lspprotips->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            goto  EXIT_LABEL;
        }
        
        lspprot = (LSPPROT *)(node->data);

        /*lspprot引用计数为0时，删除对应配置*/
        if((lspprot->l2num == 0) && (lspprot->l3num == 0))
        {
            if ( lspprot->index != 0)
            {
                spm_l3_lspprot_del_tunnel_aps(lspprot);
                rv = spm_l3_lspprot_del_drv_tunnel(lspprot);
                if(rv != 0)
                {
                    rv = DEL_LSPPROT_DRV_TUNNEL_ERR;
                    lspprot->return_code = rv;
                }
            }
            if( rv == 0)      /* 删除成功，删除hash表对应的配置*/
            {
                lspprot = (LSPPROT *)(node->data);
                spm_hashtab_del( SHARED.lspprothash , &lspprotkey NBB_CCXT);
                NBB_MM_FREE(lspprot, MEM_SPM_LSPPROT_CB);
            }
        }
        else
        {
            /*置待删除标志位*/
            lspprot->delflag = 2;
        }
    }
    else if( lspprotips->delete_struct == 0) /* 增加*/
    {
        if( lspprotips->oper_basic == ATG_DCI_OPER_ADD) /* 增加*/
        {
             lspprotips->basic_return_code = ATG_DCI_RC_OK;
             
             /*操作之前，查看本地数据是否存在*/
            if( node == NULL )
            {
                lspprot = (LSPPROT *)NBB_MM_ALLOC(sizeof(LSPPROT), NBB_NORETRY_ACT, MEM_SPM_LSPPROT_CB);
                if ( lspprot == NULL )
                {
                    rv = ADD_LSPPROT_MALLOC_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot: malloc null ,rv=%d,\n",
                                                                                          __LINE__,rv);
                    goto  EXIT_LABEL;
                } 
                NBB_MEMSET(lspprot, 0, sizeof(LSPPROT));
                flag = 1;
                NBB_TRC_FLOW((NBB_FORMAT "Add LSPPROT new node"));
            }
            else
            {
                if( node->data == NULL )
                {
                    rv = ADD_LSPPROT_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:node data null ,rv=%d,\n",
                                                                                          __LINE__,rv);
                    goto  EXIT_LABEL;
                }
                flag = 2;
                lspprot = (LSPPROT *)(node->data);
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(lspprotips, &lspprotips->basic_data); 
            if (ucdata == NULL)
            {
                rv = ADD_LSPPROT_BASIC_IPS_DATA_NULL;
                lspprot->return_code = rv;
                goto  EXIT_LABEL;
            }
                    
            //lspprot->base= (LSPPROT_BASE *)NBB_MM_ALLOC(sizeof(LSPPROT_BASE), 
            //                                              NBB_NORETRY_ACT, MEM_SPM_LSPPROT_BASE_CB);
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(lspprot->key), &(lspprotips->key), sizeof( LSPPROT_KEY ) );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(lspprot->base), ucdata, sizeof( LSPPROT_BASE ) ); 

			/*刷新保护参数*/
			if(lspprot->frrid != 0)
			{
                spm_l3_mod_lspp_para(lspprot);
			}
            
            rv = 0;  /* 调用驱动成功*/  
            if( rv == 0)      /*成功*/
            {
                if( flag == 1 )
                {
                    spm_hashtab_insert ( SHARED.lspprothash ,  lspprot->key , lspprot NBB_CCXT);
                }
        
            }
        }
        else     if( lspprotips->oper_basic == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
            NBB_TRC_FLOW((NBB_FORMAT "e Del lspprot:  basic data "));
            lspprotips->basic_return_code = ATG_DCI_RC_DEL_FAILED;  
            goto  EXIT_LABEL;
        }
        if( lspprotips->oper_lsp_1to1 == ATG_DCI_OPER_UPDATE) /* 增加*/   
        {            

             lspprotips->lsp1to1_return_code = ATG_DCI_RC_OK;
            if(( lspprotips->oper_basic != ATG_DCI_OPER_ADD))
            {
                if ( node == NULL )                                 
                {
                    rv =  ADD_LSPPROT_1TO1_DATA_NODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 node null ,rv=%d,\n",
                                                                                          __LINE__,rv);
                    goto   EXIT_LABEL;
                }
                if( node->data == NULL )
                {
                    rv = ADD_LSPPROT_1TO1_DATA_NODE_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 node data null ,rv=%d,\n",
                                                                                          __LINE__,rv);
                    goto  EXIT_LABEL;
                }
                lspprot = (LSPPROT *)(node->data);
            }
            if( lspprot == NULL )   
            {
                rv = ADD_LSPPROT_1TO1_DATA_LSPPROT_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 lspprot null ,rv=%d,\n",
                                                                                          __LINE__,rv);
                goto  EXIT_LABEL;
            }
            lspnum = lspprotips->lsp_1to1_num;
            if (lspnum > 2)
            {
                rv = ADD_LSPPROT_1TO1_DATA_LSPNUM_EXC;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 lspnum>2 ,rv=%d.lspnum=%d,\n",
                                                                               __LINE__,rv,lspnum);
                lspprot->return_code = rv;
                goto  EXIT_LABEL;
            }
            
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(lspprotips, &lspprotips->lsp_1to1);
            if (ucdata == NULL)
            {
                rv = ADD_LSPPROT_1TO1_IPS_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 data ips null ,rv=%d.\n",
                                                                               __LINE__,rv);
                lspprot->return_code = rv;
                goto  EXIT_LABEL;
            }
            for ( i = 0; i < lspnum; i++)
            {
                lsp1b1 = (LSPPROT_1B1 *)(ucdata + (NBB_ALIGN_OFFSET(sizeof(LSPPROT_1B1))) * i);
                OS_MEMSET( &(lsp[i]), 0, sizeof( LSPPROT_1B1 ) );

                /*保存lsp保护配置*/
                NBB_MEMCPY( &(lspprot->lspb.lsp[i]),  lsp1b1,  sizeof( LSPPROT_1B1 ) );
                
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(lsp[i]), lsp1b1, sizeof( LSPPROT_1B1 ) );
                txlspkey.ingress = lspprotkey.ingress;
                txlspkey.egress = lspprotkey.egress;
                txlspkey.tunnelid = lspprotkey.tunnelid;
                txlspkey.lspid = lsp[i].lspid;                

                NBB_MEMCPY(&(newtxlspkey[i]), &txlspkey, sizeof (CRTXLSP_KEY));               
                
                txnode =  (SHASH_NODE *)spm_hashtab_search( SHARED.crlsptxhash, &txlspkey NBB_CCXT);  
                if ((txnode == NULL) || (txnode->data == NULL))
                {
                    rv = ADD_LSPPROT_1TO1_TXLSP_DATA_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 txlsp null ,rv=%d.\n",
                                                                               __LINE__,rv);  
                    lspprot->return_code = rv;
                    goto  EXIT_LABEL;
                }
                else
                {
                    rv = 0;
                    txlsp = (CRTXLSP *)(txnode->data);
                    
                    //coverity[bad_sizeof]
                    NBB_MEMCPY( &(lspprot->dclsp[i]), &(txlsp->dclsp), sizeof( DC_LSP ) );
                    NBB_TRC_DETAIL((NBB_FORMAT "Add LSPPROT lspindex =%d ", lspprot->dclsp[i].index));
                }

                if (lsp[i].state == 0)
                {
                    NBB_MEMCPY(&mtxlspkey, &txlspkey, sizeof (CRTXLSP_KEY));
                    moutport = lspprot->dclsp[i].nextport;
                    mlsp = lsp[i].lspid; 
                }
                if (lsp[i].state == 1)
                {
                    NBB_MEMCPY(&btxlspkey, &txlspkey, sizeof (CRTXLSP_KEY));
                    boutport = lspprot->dclsp[i].nextport;
                    blsp = lsp[i].lspid; 
                }
            }
            
            if (lspnum == 1)
            {                
                if ((mtxlspkey.egress == 0) && (mtxlspkey.ingress == 0)
                    && (mtxlspkey.lspid == 0) && (mtxlspkey.tunnelid == 0))
                {
                    NBB_MEMCPY(&mtxlspkey, &btxlspkey, sizeof(CRTXLSP_KEY));                    
                    moutport = boutport;
                    mlsp = blsp;                    
                }                
                if ((btxlspkey.egress == 0) && (btxlspkey.ingress == 0)
                    && (btxlspkey.lspid == 0) && (btxlspkey.tunnelid == 0))
                {
                    NBB_MEMCPY(&btxlspkey, &mtxlspkey, sizeof(CRTXLSP_KEY));                    
                    boutport = moutport;
                    blsp = mlsp;
                }                
                NBB_MEMCPY(&newtxlspkey[1], &newtxlspkey[0], sizeof(CRTXLSP_KEY));
            }

            if (lspprot->index == 0)
            {  
                OS_MEMSET(&oldhwtxlspkey, 0, 2 * sizeof(CRTXLSP_KEY));                
            }           
#if 0
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, new[0] = {%x, %x, %x, %x}\n", 
                __LINE__, newtxlspkey[0].ingress, newtxlspkey[0].egress, newtxlspkey[0].tunnelid, 
                newtxlspkey[0].lspid);            
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, new[1] = {%x, %x, %x, %x}\n", 
                __LINE__, newtxlspkey[1].ingress, newtxlspkey[1].egress, newtxlspkey[1].tunnelid, 
                newtxlspkey[1].lspid);
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, mtxlspkey = {%x, %x, %x, %x}\n", 
                __LINE__, mtxlspkey.ingress, mtxlspkey.egress, mtxlspkey.tunnelid, mtxlspkey.lspid);
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, btxlspkey = {%x, %x, %x, %x}\n", 
                __LINE__, btxlspkey.ingress, btxlspkey.egress, btxlspkey.tunnelid, btxlspkey.lspid);
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, oldhwtxlspkey[0] = {%x, %x, %x, %x}\n", 
                __LINE__, oldhwtxlspkey[0].ingress, oldhwtxlspkey[0].egress, 
                oldhwtxlspkey[0].tunnelid, oldhwtxlspkey[0].lspid);        
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, oldhwtxlspkey[1] = {%x, %x, %x, %x}\n", 
                __LINE__, oldhwtxlspkey[1].ingress, oldhwtxlspkey[1].egress, 
                oldhwtxlspkey[1].tunnelid, oldhwtxlspkey[1].lspid);            
#endif

            /*在这里将hwtxlspkey值先赋值出来*/
            if (lspprot->index == 0)
            {               
                NBB_MEMCPY(&hwtxlspkey[0], &mtxlspkey, sizeof(CRTXLSP_KEY));
                NBB_MEMCPY(&hwtxlspkey[1], &btxlspkey, sizeof(CRTXLSP_KEY));                            
            }
            else
            {   
                rv = spm_l3_lspprot_get_drv_frrstatus(lspprot->frrid,&workstate);
                if(rv != 0)
                {
                    rv = ADD_LSPPROT_GET_DRV_FRR_STATUS_ERR;
                    lspprot->return_code = rv;
                }

                /*下面获取工作lsp路径信息*/
                worktxlspkey.egress = lspprotkey.egress;
                worktxlspkey.ingress = lspprotkey.ingress;
                worktxlspkey.tunnelid = lspprotkey.tunnelid;                    

                if (workstate == 0)
                {                
                    worktxlspkey.lspid = oldhwtxlspkey[0].lspid;
                }
                if (workstate == 1)
                {                    
                    worktxlspkey.lspid = oldhwtxlspkey[1].lspid;
                }

                /*交叉对比*/
                if ((NBB_MEMCMP(&oldhwtxlspkey[0], &newtxlspkey[0], sizeof(CRTXLSP_KEY)) == 0) 
                    || (NBB_MEMCMP(&oldhwtxlspkey[0], &newtxlspkey[1], sizeof(CRTXLSP_KEY)) == 0))
                {                    
                    NBB_MEMCPY(&hwtxlspkey[0], &oldhwtxlspkey[0], sizeof (CRTXLSP_KEY));
                    
                    if (NBB_MEMCMP(&oldhwtxlspkey[0], &newtxlspkey[0], sizeof (CRTXLSP_KEY)) != 0)
                    {                       
                        NBB_MEMCPY(&hwtxlspkey[1], &newtxlspkey[0], sizeof (CRTXLSP_KEY));
                    }
                    else
                    {                        
                        NBB_MEMCPY(&hwtxlspkey[1], &newtxlspkey[1], sizeof (CRTXLSP_KEY));
                    } 

                    /*如果工作lsp被更新, 则先切换硬件工作状态再刷新硬件表项*/
                    if ((NBB_MEMCMP(&worktxlspkey, &newtxlspkey[0], sizeof(CRTXLSP_KEY)) != 0) 
                        && (NBB_MEMCMP(&worktxlspkey, &newtxlspkey[1], sizeof(CRTXLSP_KEY)) != 0))
                    {
                        /*处理: 先切换硬件状态,再刷新*/
                        workstate = (workstate ^ 1);

                        /*是否需要切换工作路径?*/

                        /*刷新硬件表项*/
                    }
                }
                
                else if ((NBB_MEMCMP(&oldhwtxlspkey[1], &newtxlspkey[0], sizeof (CRTXLSP_KEY)) == 0) 
                    || (NBB_MEMCMP(&oldhwtxlspkey[1], &newtxlspkey[1], sizeof (CRTXLSP_KEY)) == 0))
                {                    
                    NBB_MEMCPY(&hwtxlspkey[1], &oldhwtxlspkey[1], sizeof (CRTXLSP_KEY));
                    if (NBB_MEMCMP(&oldhwtxlspkey[1], &newtxlspkey[0], sizeof (CRTXLSP_KEY)) != 0)
                    {                        
                        NBB_MEMCPY(&hwtxlspkey[0], &newtxlspkey[0], sizeof (CRTXLSP_KEY));
                    }
                    else
                    {                        
                        NBB_MEMCPY(&hwtxlspkey[0], &newtxlspkey[1], sizeof (CRTXLSP_KEY));
                    }

                    /*如果工作lsp被更新, 则先切换硬件工作状态再刷新硬件表项*/
                    if ((NBB_MEMCMP(&worktxlspkey, &newtxlspkey[0], sizeof(CRTXLSP_KEY)) != 0) 
                        && (NBB_MEMCMP(&worktxlspkey, &newtxlspkey[1], sizeof(CRTXLSP_KEY)) != 0))
                    {
                        /*处理: 先切换硬件状态,再刷新*/
                        workstate = (workstate ^ 1);

                        /*是否需要切换工作路径?*/

                        /*刷新硬件表项*/
                    }
                    
                }
                else
                { 
                    NBB_MEMCPY(&hwtxlspkey[0], &mtxlspkey, sizeof(CRTXLSP_KEY));
                    NBB_MEMCPY(&hwtxlspkey[1], &btxlspkey, sizeof(CRTXLSP_KEY));
                    workstate = 0;
                } 

                /*主备颠倒标志赋值*/
                if ((NBB_MEMCMP(&mtxlspkey, &hwtxlspkey[0], sizeof(CRTXLSP_KEY)) == 0)
                    && (NBB_MEMCMP(&btxlspkey, &hwtxlspkey[1], sizeof(CRTXLSP_KEY)) == 0))
                {
                    reverseflag = 0;
                }
                else if ((NBB_MEMCMP(&mtxlspkey, &hwtxlspkey[1], sizeof(CRTXLSP_KEY)) == 0)
                    && (NBB_MEMCMP(&btxlspkey, &hwtxlspkey[0], sizeof(CRTXLSP_KEY)) == 0))
                {
                    reverseflag = 1;
                }
                     
            }

#if 0
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, hwtxlspkey[0] = {%x, %x, %x, %x}\n", 
                    __LINE__, hwtxlspkey[0].ingress, hwtxlspkey[0].egress, 
                    hwtxlspkey[0].tunnelid, hwtxlspkey[0].lspid);
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, hwtxlspkey[1] = {%x, %x, %x, %x}\n", 
                    __LINE__, hwtxlspkey[1].ingress, hwtxlspkey[1].egress, hwtxlspkey[1].tunnelid, 
                    hwtxlspkey[1].lspid);            
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, worktxlspkey = {%x, %x, %x, %x}\n", 
                    __LINE__, worktxlspkey.ingress, worktxlspkey.egress,
                    worktxlspkey.tunnelid, worktxlspkey.lspid);                    
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "rsvp Line=%d, reverseflag = %d, workstate = %d\n", 
                    __LINE__, reverseflag, workstate); 
#endif

            /**********获取对应hwtxlspkey的posid***********************/
			txnode =  (SHASH_NODE *)spm_hashtab_search( SHARED.crlsptxhash, &hwtxlspkey[0] NBB_CCXT);  
            if ((txnode == NULL) || (txnode->data == NULL))
            {
                rv = ADD_LSPPROT_1TO1_TXLSP_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 txlsp null ,rv=%d.\n",
                                                                               __LINE__,rv);
                lspprot->return_code = rv;
                goto  EXIT_LABEL;
            }
            else
            {
                rv = 0;
                txlsp = (CRTXLSP *)(txnode->data); 
                txposid[0] = txlsp->dclsp.index;                                                             
            }

            txnode =  (SHASH_NODE *)spm_hashtab_search( SHARED.crlsptxhash, &hwtxlspkey[1] NBB_CCXT);  
            if ((txnode == NULL) || (txnode->data == NULL))
            {
                rv = ADD_LSPPROT_1TO1_TXLSP_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 txlsp null ,rv=%d.\n",
                                                                               __LINE__,rv);
                lspprot->return_code = rv;
                goto  EXIT_LABEL;
            }
            else
            {
                rv = 0;
                txlsp = (CRTXLSP *)(txnode->data); 
                txposid[1] = txlsp->dclsp.index;                                                           
            }   

            /***********************/
            //rsvp 调试配置保存
            lspprot->reverseflag = reverseflag;
            lspprot->hwstate = workstate;
            lspprot->hwposid[0] = txposid[0];
            lspprot->hwposid[1] = txposid[1];
           
            rv = spm_l3_lspprot_create_drv_tunnel(lspprot);
            if(rv != 0)
            {
                rv = ADD_LSPPROT_1TO1_DRV_TUNNEL_ERR;
                lspprot->return_code = rv;
                goto  EXIT_LABEL;
            }
            //coverity[dead_error_condition]
            if( rv == 0)      /* 成功*/
            {
                lspprot->lspnum = lspnum;           
				spm_l3_updatevrf_rsvp(&(lspprot->key),lspprot->index NBB_CCXT);
				spm_l3_updatevp_rsvp(&(lspprot->key),moutport,lspprot->index NBB_CCXT);
                spm_l3_updatemcid_rsvp(&(lspprot->key),moutport NBB_CCXT);
                
                NBB_MEMCPY(&lspprot->worklspkey, &worktxlspkey, sizeof(CRTXLSP_KEY));
                NBB_MEMCPY(&lspprot->hwlspkey[0], &hwtxlspkey[0], 2 * sizeof(CRTXLSP_KEY));
                NBB_MEMCPY(&lspprot->oldhwlspkey[0], &oldhwtxlspkey[0], 2 * sizeof(CRTXLSP_KEY));

                /*这个地方需要讨论只有一个旧的txlsp的情况，如何赋值?*/               
                NBB_MEMCPY(&oldhwtxlspkey[0], &hwtxlspkey[0], 2 * sizeof(CRTXLSP_KEY));                
                
                //coverity[bad_sizeof]
                    

                spm_l3_lspprot_create_tunnelfrr_aps(lspprot);/*保护信息传递给aps模块*/
                
            }
            else    /*调用驱动失败，记录错误*/
            {
                rv = ADD_LSPPROT_1TO1_DRV_TUNNEL_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add lspprot:1to1 tunnel drv ,rv=%d.\n",
                                                                               __LINE__,rv);
                lspprot->return_code = rv;
                //coverity[dead_error_begin]
                  
                goto  EXIT_LABEL;
            }
        }
        else  if( lspprotips->oper_lsp_1to1 == ATG_DCI_OPER_DEL )   /*  删除子TLV*/
        {
            lspprotips->lsp1to1_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
            goto  EXIT_LABEL;           
        }
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    /*程序异常时，记录错误条目的key值*/
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d lspp:err in=%x.egr=%x.tunl=%d\n",
                                           __LINE__,lspprotkey.ingress,lspprotkey.egress,lspprotkey.tunnelid);
    }

}

#endif
