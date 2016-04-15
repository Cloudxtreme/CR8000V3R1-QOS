
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
VRF_U_ROUTE配置块
********************/
#ifdef VRF_U_ROUTE


#if 0
NBB_VOID spm_l3_meminit( NBB_CCXT_T NBB_CXT)
{
    NBB_ULONG   i;

    VRFUROUTE   *route = NULL;

    VRFUROUTE   *preroute = NULL;

    for (i = 0; i < 1300000; i++)  
    {
        if (i == 0)
        {
            SHARED.routemem = (VRFUROUTE *)NBB_MM_ALLOC(sizeof(VRFUROUTE), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_NODE_CB);
            if( SHARED.routemem == NULL )
            {
                NBB_PRINTF(" Init Routemem Error\n");
                
                return ;
            }
            NBB_MEMSET(SHARED.routemem, 0, sizeof(VRFUROUTE));
            
            AVLL_INIT_NODE(SHARED.routemem->route_node);  
            SHARED.routemem->memindex = i +1;
            SHARED.routemem->memflag = 0;
            SHARED.routemem->next = NULL;
            preroute = SHARED.routemem;
        }
        else
        {
            route = (VRFUROUTE *)NBB_MM_ALLOC(sizeof(VRFUROUTE), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_NODE_CB);
            if( route == NULL )
            {
                NBB_PRINTF(" Init Routemem2 Error\n");
                
                return ;
            }
            NBB_MEMSET(route, 0, sizeof(VRFUROUTE));
                
            AVLL_INIT_NODE(route->route_node);  
            route->memindex = i +1;
            route->memflag = 0;
            route->next = NULL;
            preroute->next = route;
            
            preroute = route;
        }
    }

}
#endif


#if 1
NBB_VOID spm_l3_meminit( NBB_CXT_T NBB_CXT)
{
    NBB_ULONG   i;

    VRFUROUTE   *route = NULL;

    VRFUROUTE   *preroute = NULL;


    SHARED.routemem = (VRFUROUTE *)OS_OS_ALLOC(1300000 * sizeof(VRFUROUTE));
    if( SHARED.routemem == NULL )
    {
        NBB_PRINTF(" Init RouteMem Error\n");
        
        return ;
    }
    route = SHARED.routemem;
    preroute = SHARED.routemem;
    for (i = 0; i < 1300000; i++)  
    {
        if( route == NULL )
        {
            NBB_PRINTF(" Init route error\n");
            
            return ;
        }
        OS_MEMSET(route, 0, sizeof(VRFUROUTE));
            
        AVLL_INIT_NODE(route->route_node);  
        route->memindex = i + 1;
        route->memflag = 0;
        route->next = NULL;
        preroute->next = route;
        
        preroute = route;
        
        //coverity[write_write_order]
        route = route++;
        
    }

}

#endif


#if 0

NBB_VOID spm_l3_meminit( NBB_CXT_T NBB_CXT)
{
    NBB_ULONG   i;

    VRFUROUTE   *route = NULL;

    VRFUROUTE   *preroute = NULL;

    SHARED.routemem = &(SHARED.g_uroute[0]);
    preroute = &(SHARED.g_uroute[0]);
    
    OS_MEMSET(preroute, 0, sizeof(VRFUROUTE));          
    AVLL_INIT_NODE(preroute->route_node);  
    preroute->memindex = 1;
    preroute->memflag = 0;
    preroute->next = NULL;
    for (i = 1; i < 200000; i++)  
    {
        route = &(SHARED.g_uroute[i]);
        OS_MEMSET(route, 0, sizeof(VRFUROUTE));         
        AVLL_INIT_NODE(route->route_node);  
        route->memindex = i +1;
        route->memflag = 0;
        route->next = NULL;
        preroute->next = route;
        
        preroute = route;
    }

}

#endif

/******************************************************************************
 * FunctionName 	: 	spm_l3_memmalloc
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID *spm_l3_memmalloc( NBB_CXT_T NBB_CXT)
{

    VRFUROUTE   *route = NULL;

    NBB_TRC_ENTRY("spm_l3_memmalloc");

#if 1   
    if( SHARED.routemem == NULL )
    {
        NBB_PRINTF(" routemem full\n");
        goto EXIT_LABEL;
    }
    if( SHARED.routemem->memflag == 0 )
    {
        route = SHARED.routemem;
        SHARED.routemem = SHARED.routemem->next;
        route->next = NULL;
        AVLL_INIT_NODE(route->route_node);  
        route->memflag = 1;
    }
    else
    {
        NBB_TRC_DETAIL((NBB_FORMAT "malloc error memidex = %d ", SHARED.routemem->memindex));
    }

#endif  
#if 0

    route = (VRFUROUTE *)NBB_MM_ALLOC(sizeof(VRFUROUTE), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_NODE_CB);
    if( route == NULL )
    {
        NBB_PRINTF("   alloc routemem  full\n");
        goto EXIT_LABEL;
    }
    NBB_MEMSET(route, 0, sizeof(VRFUROUTE));    
    AVLL_INIT_NODE(route->route_node);  
    route->memindex = i +1;
    route->memflag = 0;
    route->next = NULL;
    
#endif  

    EXIT_LABEL:
    NBB_TRC_EXIT();

    return(route);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_memfree
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_memfree( VRFUROUTE  *route NBB_CCXT_T NBB_CXT)
{

    NBB_TRC_ENTRY("spm_l3_memfree");

#if 1   
    if( route == NULL )
    {
        NBB_PRINTF(" free routemem  null\n");
        goto EXIT_LABEL;
    }

    if( route->memflag == 1 )
    {
        route->next = SHARED.routemem;
        SHARED.routemem = route;
        route->memflag = 0;
        route->flag = 0;
        route->ipv4num = 0;
        route->ipv6num = 0;

        route->frrtype = 0;
        route->ecmpflag = 0;
        route->frrecmpid = 0;        

        AVLL_INIT_NODE(route->route_node);  
        
        //coverity[bad_sizeof]
        NBB_MEMSET(&(route->key), 0, sizeof(VRFUROUTE_KEY));
        
        //coverity[bad_sizeof]
        NBB_MEMSET(&(route->base), 0, sizeof(VRFUROUTE_BASE));
        
        //coverity[bad_sizeof]
        NBB_MEMSET(&(route->nexthop[0]), 0, 8 * sizeof(VRFUROUTE_NEXT));
        
        //coverity[bad_sizeof]
        NBB_MEMSET(&(route->dchop[0]), 0, 8 * sizeof(DC_UNI));
        
        //coverity[bad_sizeof]
        NBB_MEMSET(&(route->nextindex[0]), 0, 8 * 4);
    }
    else
    {
        NBB_TRC_DETAIL((NBB_FORMAT "free error memidex = %d ", route->memindex));
    }
    NBB_TRC_DETAIL((NBB_FORMAT "free  memidex = %d ", route->memindex));
#endif  

#if 0   
    if( route == NULL )
    {
        NBB_PRINTF("   free routemem  null\n");
        goto EXIT_LABEL;
    }
    NBB_MM_FREE(route, MEM_SPM_VRFUROUTE_NODE_CB);
#endif  
    
    EXIT_LABEL:
    NBB_TRC_EXIT();

    return;

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_vrfuroutecomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_vrfuroutecomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
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
 * FunctionName 	: 	spm_l3_uniportcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_uniportcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    UNI_KEY *key1 = (UNI_KEY *)keyarg1;
    UNI_KEY *key2 = (UNI_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_uniportcomp");

    rv = compare_ulong(&key1->nextport, &key2->nextport NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    rv = NBB_MEMCMP(&(key1->nextip[0]), &(key2->nextip[0]),16 );
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_pwcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_pwcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    PW_KEY *key1 = (PW_KEY *)keyarg1;
    PW_KEY *key2 = (PW_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_pwcitecomp");

    rv = compare_ulong(&key1->label, &key2->label NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }   
    
    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}


/******************************************************************************
 * FunctionName 	: 	spm_l3_peercomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_peercomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    PEER_KEY *key1 = (PEER_KEY *)keyarg1;
    PEER_KEY *key2 = (PEER_KEY *)keyarg2;
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

/******************************************************************************
 * FunctionName 	: 	spm_l3_peerepcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_peerepcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    PEEREP_KEY *key1 = (PEEREP_KEY *)keyarg1;
    PEEREP_KEY *key2 = (PEEREP_KEY *)keyarg2;
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


/******************************************************************************
 * FunctionName 	: 	spm_l3_ecmpnodecomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_ecmpnodecomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    ECMPEN_KEY *key1 = (ECMPEN_KEY *)keyarg1;
    ECMPEN_KEY *key2 = (ECMPEN_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_uniportcomp");

    rv = compare_ushort(&key1->ecmpid, &key2->ecmpid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}


/******************************************************************************
 * FunctionName 	: 	spm_l3_ecmpidcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_ecmpidcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    ECMP_KEY *key1 = (ECMP_KEY *)keyarg1;
    ECMP_KEY *key2 = (ECMP_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_uniportcomp");

    rv = compare_ulong(&key1->nhi1, &key2->nhi1 NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->nhi2, &key2->nhi2 NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->nhi3, &key2->nhi3 NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->nhi4, &key2->nhi4 NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    
    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_bfdfrrcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    BFDFRR_KEY *key1 = (BFDFRR_KEY *)keyarg1;
    BFDFRR_KEY *key2 = (BFDFRR_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_uniportcomp");

    rv = compare_ushort(&key1->bfdid, &key2->bfdid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_ipfrrcomp(NBB_VOID *keyarg1, NBB_VOID *keyarg2 NBB_CCXT_T NBB_CXT)
{

    IPFRR_KEY *key1 = (IPFRR_KEY *)keyarg1;
    IPFRR_KEY *key2 = (IPFRR_KEY *)keyarg2;
    NBB_INT rv = 0;

    //NBB_TRC_ENTRY("spm_l3_uniportcomp");

    rv = compare_ushort(&key1->vrfid, &key2->vrfid NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->mpeer, &key2->mpeer NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }
    rv = compare_ulong(&key1->bpeer, &key2->bpeer NBB_CCXT);
    if (rv != 0)
    {
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    
    //NBB_TRC_EXIT();

    return(rv);

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_vrfuroutetreeinit( NBB_CXT_T NBB_CXT)
{
    
    AVLL_INIT_TREE(SHARED.vrfuroute_tree,
                    spm_l3_vrfuroutecomp,
                    NBB_OFFSETOF(VRFUROUTE, key),/*lint !e413 */
                    NBB_OFFSETOF(VRFUROUTE, route_node));/*lint !e413 */

    AVLL_INIT_TREE(SHARED.dcroute_tree,
                    spm_l3_vrfuroutecomp,
                    NBB_OFFSETOF(VRFUROUTE, key),/*lint !e413 */
                    NBB_OFFSETOF(VRFUROUTE, route_node));/*lint !e413 */

    AVLL_INIT_TREE(SHARED.lbroute_tree,
                    spm_l3_vrfuroutecomp,
                    NBB_OFFSETOF(VRFUROUTE, key),/*lint !e413 */
                    NBB_OFFSETOF(VRFUROUTE, route_node));/*lint !e413 */

    AVLL_INIT_TREE(SHARED.outuni,
                    spm_l3_uniportcomp,
                    NBB_OFFSETOF(UNIPORT, key),/*lint !e413 */
                    NBB_OFFSETOF(UNIPORT, uni_node));/*lint !e413 */

    AVLL_INIT_TREE(SHARED.peer_tree,
                    spm_l3_peercomp,
                    NBB_OFFSETOF(MPEERFRR, key),/*lint !e413 */
                    NBB_OFFSETOF(MPEERFRR, mp_node));/*lint !e413 */

    AVLL_INIT_TREE(SHARED.ecmp_tree,
                    spm_l3_ecmpidcomp,
                    NBB_OFFSETOF(ECMPTAB, key),/*lint !e413 */
                    NBB_OFFSETOF(ECMPTAB, ep_node));/*lint !e413 */

    AVLL_INIT_TREE(SHARED.peecmp_tree,
                    spm_l3_peerepcomp,
                    NBB_OFFSETOF(PEEREPTAB, key),/*lint !e413 */
                    NBB_OFFSETOF(PEEREPTAB, pp_node));/*lint !e413 */

    AVLL_INIT_TREE(SHARED.bfdfrr_tree,
                    spm_l3_bfdfrrcomp,
                    NBB_OFFSETOF(BFDFRR, key),/*lint !e413 */
                    NBB_OFFSETOF(BFDFRR, bf_node));/*lint !e413 */

    AVLL_INIT_TREE(SHARED.ipfrr_tree,
                    spm_l3_ipfrrcomp,
                    NBB_OFFSETOF(IPFRR, key),/*lint !e413 */
                    NBB_OFFSETOF(IPFRR, ipf_node));/*lint !e413 */


    NBB_PRINTF("\n SHARED.outuni: %x\n", &(SHARED.outuni));
    
    //NBB_PRINTF("\n SHARED.g_uroute: %x\n", &(SHARED.g_uroute[0]));
    NBB_PRINTF("\n SHARED.mcroute_tree: %x\n", &(SHARED.mcroute_tree));

    NBB_PRINTF("\n C3 Chip  sum = %d \n", SHARED.c3_num);
    
    spm_l3_meminit(NBB_CXT);
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_alloc_route_node(VRFUROUTE_KEY  *routkey, TUNNELCH_KEY *pwkey,LSPPROT_KEY *lspprotkey NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 100;
    ROUTNODE        *routenode;
    PWNODE          *pwnode;
    LSPRSVP             *lsprs;

    NBB_TRC_ENTRY("spm_l3_alloc_route_node");

    lsprs = (LSPRSVP *)AVLL_FIND(SHARED.rsvpcite, lspprotkey);
    if (lsprs == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find LSPRSVP"));
        rv = 101;
        goto EXIT_LABEL;
    }

    pwnode = (PWNODE *)AVLL_FIND(lsprs->pw_tree, pwkey);
    if (pwnode == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find lsprs->pw_tree"));
        rv = 102;
        goto EXIT_LABEL;
    }

    routenode = (ROUTNODE *)AVLL_FIND(pwnode->rout_tree, routkey);
    if (routenode == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find pwnode->rout_tree"));
        rv = 103;
        goto EXIT_LABEL;
    }
    routenode = (ROUTNODE *)NBB_MM_ALLOC(sizeof(ROUTNODE),NBB_NORETRY_ACT,MEM_SPM_VRFUROUTE_ROUTNODE_CB);
    if (routenode == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to alloc  ROUTNODE"));
        rv = 104;
        goto EXIT_LABEL;
    }
    
    NBB_MEMSET(routenode, 0, sizeof(ROUTNODE));
    AVLL_INIT_NODE(routenode->rout_node);
    routenode->pwcite = NULL;
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &(routenode->key), routkey, sizeof( VRFUROUTE_KEY) );
    
     if (pwnode != NULL)
     {
        //coverity[no_effect_test]
        AVLL_INSERT(pwnode->rout_tree, routenode->rout_node);
        pwnode->routenum += 1;
        routenode->pwcite = pwnode;
        NBB_TRC_DETAIL((NBB_FORMAT "pwnode->rout_tree AVLL_INSERT  rv= %B", rv));
     }
     
    
    EXIT_LABEL:
    
    NBB_TRC_EXIT();

    return rv;

} 


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_free_route_node(VRFUROUTE_KEY  *routkey, TUNNELCH_KEY *pwkey,LSPPROT_KEY *lspprotkey NBB_CCXT_T NBB_CXT)
{
    NBB_INT         rv = 100;
    ROUTNODE        *routenode;
    PWNODE          *pwnode;
    LSPRSVP             *lsprs;

    NBB_TRC_ENTRY("spm_l3_free_route_node");

    lsprs = (LSPRSVP *)AVLL_FIND(SHARED.rsvpcite, lspprotkey);
    if (lsprs == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find LSPRSVP"));
        rv = 101;
        goto EXIT_LABEL;
    }
    pwnode = (PWNODE *)AVLL_FIND(lsprs->pw_tree, pwkey);
    if (pwnode == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find PWNODE"));
        rv = 102;
        goto EXIT_LABEL;
    }
    routenode = (ROUTNODE *)AVLL_FIND(pwnode->rout_tree, routkey);
    if (routenode == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to find pwnode->rout_tree"));
        rv = 103;
        goto EXIT_LABEL;
    }

    NBB_TRC_DETAIL((NBB_FORMAT "free  for route vrfid=%x",routkey->vrfid));

    AVLL_DELETE(pwnode->rout_tree, routenode->rout_node);
    routenode->pwcite = NULL;
    NBB_MM_FREE(routenode, MEM_SPM_VRFUROUTE_ROUTNODE_CB);
    pwnode->routenum = pwnode->routenum - 1;
    
    EXIT_LABEL:
    
    NBB_TRC_EXIT();
    
    return rv;

}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_getuni( NBB_BYTE flag,NBB_ULONG *ip, NBB_ULONG port,NBB_ULONG *nhi NBB_CCXT_T NBB_CXT)
{
    NBB_INT             rv = 100;
    NBB_INT             rv2 = 100;
    
    L3_UNI_T                outuni;

    UNI_KEY             unikey;

    UNIPORT             *uniports = NULL;

    SPM_PORT_INFO_CB    portfo;

    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS  downqosdata;

    ATG_DCI_LOG_PORT_DIFF_SERV_DATA  logdsdata;

    NBB_TRC_ENTRY("spm_l3_getuni");

    if (port == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getuni port = 0 \n", __LINE__);
        rv = ADD_UCROUTE_L3UNI_PARA_ERR;
        goto  EXIT_LABEL;
    }

    if (ip == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getuni ip = NULL \n", __LINE__);
        rv = ADD_UCROUTE_L3UNI_PARA_ERR;
        goto  EXIT_LABEL;
    }

    if (nhi == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getuni nhi = NULL \n", __LINE__);
        rv = ADD_UCROUTE_L3UNI_PARA_ERR;
        goto  EXIT_LABEL;
    }

    OS_MEMSET(&outuni , 0, sizeof(L3_UNI_T));
    
    OS_MEMSET(&unikey , 0, sizeof(UNI_KEY));
    if( flag == 0 )
    {
        //coverity[bad_sizeof]
        NBB_MEMCPY( &(unikey.nextip[3]), ip, 4 );
        NBB_TRC_DETAIL((NBB_FORMAT " get uni port = %d ", port));
        NBB_TRC_DETAIL((NBB_FORMAT " get uni nextip = %x ", unikey.nextip[3]));
    }
    else if( flag == 1 )
    {
        NBB_MEMCPY( &(unikey.nextip[0]), ip, 16 );
    }
    unikey.nextport = port; 
    OS_MEMSET(&portfo, 0, sizeof(SPM_PORT_INFO_CB));
    uniports =  (UNIPORT *)AVLL_FIND( SHARED.outuni ,  &unikey );
    if ( uniports == NULL )
    {
        rv = spm_get_portid_from_logical_port_index(port, &portfo NBB_CCXT);
        if( rv == 0)    
        {
            #ifdef SPU
            OS_MEMSET(&outuni , 0, sizeof(L3_UNI_T));
            
            outuni.flags = L3_UNI_COUNTER_ON;
            if ( portfo.port_type == ATG_DCI_VE_PORT )
            {
                outuni.flags |= L3_UNI_VEPORT;
            }
            outuni.mPortIndex = unikey.nextport;
            if( flag == 0 )
            {
                outuni.mNhIp = unikey.nextip[3];    
                if( unikey.nextip[3] == 0 )
                {
                    //outuni.flags |= L3_UNI_INDIRECT;
                }
                else
                {
                    outuni.flags |= L3_UNI_INDIRECT;
                }
            }
            else if( flag == 1 )
            {
                outuni.flags |= L3_UNI_IPV6;
                NBB_MEMCPY( &(outuni.mNhIpV6), &(unikey.nextip[0]), 16 );
                if(( unikey.nextip[0] == 0 ) && ( unikey.nextip[1] == 0 )
                                        && ( unikey.nextip[2] == 0 )
                                        && ( unikey.nextip[3] == 0 ))
                {
                    //outuni.flags |= L3_UNI_INDIRECT;
                }
                else
                {
                    outuni.flags |= L3_UNI_INDIRECT;
                }
            }
            outuni.tTxPort.slot = portfo.slot_id;
            outuni.tTxPort.port =   portfo.port_id;
            outuni.tTxPort.vid = portfo.svlan;  

            rv = ApiC3AddL3Uni( BCMC31,  &outuni);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3AddL3Uni( BCMC32,  &outuni);
                rv = (rv|rv2);
            }
            #endif
            #ifdef SRC
                rv = 0;
                outuni.nhIdx = 0xffffffff;
                outuni.posId = 0xffffffff;
            #endif
            #ifdef SNU
                rv = 0;
                outuni.nhIdx = 0xffffffff;
                outuni.posId = 0xffffffff;
            #endif
            if( rv == 0)
            {
                OS_MEMSET(&downqosdata , 0, sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS));
                OS_MEMSET(&logdsdata , 0, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA));
                rv = spm_get_downuserqos_from_logical_port_index(port, &downqosdata NBB_CCXT);
                if(rv != 0)
                {
                    //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR,  "Line=%d e gl2qos rv.posid=%d.%d\n", 
                    //                          __LINE__, rv,outuni.posId);
                }
                else
                {
                   // rv = spm_hqos_set_l3uni(port,outuni.posId,&downqosdata NBB_CCXT);
                    if(rv != 0)
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e suniqos rv.posid=%d.%d\n", 
                                                    __LINE__, rv,outuni.posId);
                    }
                }
                				
		    //刷新uni ds配置
		    rv = spm_get_ds_from_logical_port_index(port,&logdsdata NBB_CCXT);
		    if(rv != 0)
                {
                    //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR,  
                    //"Line=%d e uin ds rv.posid=%d.%d\n", __LINE__, rv,outuni.posId);
                }
                else
                {
                    //rv = spm_ds_set_l3uni(port,outuni.posId,&logdsdata);
                    if(rv != 0 )
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e suni ds rv.posid=%d.%d\n", __LINE__,
							                                                               rv,outuni.posId);
                    }
                }
                
                uniports = (UNIPORT *)NBB_MM_ALLOC(sizeof(UNIPORT), NBB_NORETRY_ACT, 
                                                                                    MEM_SPM_VRFUROUTE_UNI_CB);
                if(NULL == uniports)
                {
                    OS_PRINTF("Line=%d uniports malloc mem NULL \n",__LINE__);
                    rv = 888;
                    goto  EXIT_LABEL;
                }
                NBB_MEMSET(uniports, 0, sizeof(UNIPORT));
                AVLL_INIT_NODE(uniports->uni_node);  
                uniports->key.nextport = unikey.nextport;
                
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(uniports->key.nextip[0]), &(unikey.nextip[0]), 16 );
                uniports->port.slot = portfo.slot_id;
                uniports->port.port = portfo.port_id;
                uniports->port.vlan = portfo.svlan;
            
                uniports->nhid = outuni.nhIdx;
                uniports->posid = outuni.posId;
                uniports->routenum = 1;
                *nhi = outuni.nhIdx;
                
                //coverity[no_effect_test]
                AVLL_INSERT( SHARED.outuni ,  uniports->uni_node);
            }
            else
            {
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e guni dev rv=%d\n", __LINE__, rv);
                rv = ADD_UCROUTE_DRV_L3UNI_ERR;
                goto  EXIT_LABEL;
            }
        } 
        else
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e guni  lport=%d  rv=%d\n", __LINE__,port, rv);
            rv = ADD_UCROUTE_L3UNI_GET_L2PORT_ERR;
            goto  EXIT_LABEL;
        }
    }
    else
    {
        *nhi = uniports->nhid;
        uniports->routenum++;
    }
    rv = 0;
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
    
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_deluni( NBB_BYTE flag,NBB_ULONG *ip, NBB_ULONG port NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 100;
    NBB_INT     rv2 = 100;
    
    UNI_KEY     unikey;

    UNIPORT     *uniports = NULL;

    NBB_TRC_ENTRY("spm_l3_deluni");

    if (port == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e deluni port = 0 \n", __LINE__);
        rv = ADD_UCROUTE_DEL_UNI_PARA_ERR;
        goto  EXIT_LABEL;
    }

    if (ip == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e deluni ip = NULL \n", __LINE__);
        rv = ADD_UCROUTE_DEL_UNI_PARA_ERR;
        goto  EXIT_LABEL;
    }


    OS_MEMSET(&unikey , 0, sizeof(UNI_KEY));
    if( flag == 0 )
    {
        //coverity[bad_sizeof]
        NBB_MEMCPY( &(unikey.nextip[3]), ip, 4 );
        NBB_TRC_DETAIL((NBB_FORMAT " del uni port = %d ", port));
        NBB_TRC_DETAIL((NBB_FORMAT " del uni nextip = %x ", unikey.nextip[3]));
    }
    else if( flag == 1 )
    {
        NBB_MEMCPY( &(unikey.nextip[0]), ip, 16 );
    }
    unikey.nextport = port; 
    uniports =  (UNIPORT*)AVLL_FIND( SHARED.outuni ,  &unikey ); 
    if( uniports == NULL )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d l3_deluni logport=%d  rv=%d\n", __LINE__,port, rv);
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d  ip =%x.%x.%x.%x \n", __LINE__, unikey.nextip[0],
                                                                                unikey.nextip[1],
                                                                                unikey.nextip[2],
                                                                                unikey.nextip[3]);
        rv = ADD_UCROUTE_DEL_UNI_PORT_NULL;
        goto  EXIT_LABEL;
    }
    if( uniports->routenum == 1) 
    {
        #ifdef SPU
        rv = ApiC3DelL3Uni( BCMC31, uniports->posid );
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3DelL3Uni( BCMC32, uniports->posid );
            rv = (rv|rv2);
        }
        #endif
        #ifdef SRC
            rv = 0;
        #endif
        #ifdef SNU
            rv = 0;
        #endif
        AVLL_DELETE( SHARED.outuni , uniports->uni_node);
        NBB_MM_FREE(uniports, MEM_SPM_VRFUROUTE_UNI_CB);
    }
    else
    {
        uniports->routenum--;
    }
    rv = 0;
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_getpwv4( NBB_USHORT vrfid ,NBB_ULONG peerip, NBB_ULONG label,NBB_ULONG *nhi NBB_CCXT_T NBB_CXT)
{
    NBB_INT             rv = 100;
    NBB_INT             rv2 = 100;

    SHASH_NODE          *node = NULL;
    
    TUNNELCH            *tunnels = NULL;

    TUNNELCH_KEY        tunskey;

    PWTXNODE            *pwnode = NULL;

    PW_KEY              pwkey;

    L3_TX_PW_T          txpw;

    VRFINS_KEY          vrfinskey;

    VRFINS              *vrfins = NULL;

    SPM_QOS_VRF_INSTANSE_KEY    qosvrfinskey;

    SPM_QOS_TUNNEL_KEY          qostunl;

    ATG_DCI_VRF_INSTANCE_UP_VPN_QOS vrfupqos;

    NBB_ULONG           nhid = 0;

    CRTXLSP_KEY         txlspkey;

    SPM_PORT_INFO_CB  portinfo;

    FTN_KEY             ftnkey;

    FTN_TAB             *uftn = NULL;


    NBB_TRC_ENTRY("spm_l3_getpwv4");


    if( label == 0xffffffff )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getpwv4 label = 0xffffffff \n", __LINE__);
        rv = ADD_UCROUTE_GET_PWV4_PARA_ERR;
        goto  EXIT_LABEL;
    }
    if( peerip == 0 )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getpwv4 peerip = 0 \n", __LINE__);
        rv = ADD_UCROUTE_GET_PWV4_PARA_ERR;
        goto  EXIT_LABEL;
    }
    
    if (nhi == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getpwv4 nhi = NULL \n", __LINE__);
        rv = ADD_UCROUTE_GET_PWV4_PARA_ERR;
        goto  EXIT_LABEL;
    }

    OS_MEMSET(&txpw , 0, sizeof(L3_TX_PW_T));
    
    tunskey.res = 0;
    tunskey.vrfid = vrfid;
    tunskey.peerip = peerip;
    node =  (SHASH_NODE *)spm_hashtab_search( SHARED.tunnelchhash, &tunskey NBB_CCXT); 
    if( node == NULL)
    {
        #ifdef SPU
            OS_MEMSET(&txpw , 0, sizeof(L3_TX_PW_T));
        
            txpw.mVrf = vrfid;
            txpw.mPeerIp = peerip;
            txpw.mLabel = label;
            
            //txpw.flags |= L3_TX_PW_COUNTER_ON;
            txpw.pushPwNum = 1;
            txpw.eNniMode = PHB_TO_EXP;
            
            //txpw.tunnelIdx = nextindex; 
            
            rv = ApiC3AddL3TxPw( BCMC31, &txpw);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3AddL3TxPw( BCMC32, &txpw);
                rv = (rv|rv2);
            }
        #endif

        #ifdef SRC
            txpw.posId = 0xffff;
            txpw.nhIdx = 0xffffffff;
            rv = 0;
        #endif
        #ifdef SNU
            txpw.posId = 0xffff;
            txpw.nhIdx = 0xffffffff;
            rv = 0;
        #endif
        if( rv == 0)
        {
            tunnels = (TUNNELCH *)NBB_MM_ALLOC(sizeof(TUNNELCH), NBB_NORETRY_ACT, MEM_SPM_TUNNELCH_CB);

            if(NULL == tunnels)
            {
                OS_PRINTF("Line=%d tunnels malloc mem NULL \n",__LINE__);
                rv = ADD_UCROUTE_MALLOC_TUNNEL_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e tunnel null \n", __LINE__);
                goto  EXIT_LABEL;
            }
            NBB_MEMSET(tunnels, 0, sizeof(TUNNELCH));
            AVLL_INIT_TREE(tunnels->pwtree,
                                spm_l3_pwcomp,
                                NBB_OFFSETOF(PWTXNODE, key),
                                NBB_OFFSETOF(PWTXNODE, pw_node));
            tunnels->pwnum = 1;
            tunnels->delflag = 100;
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(tunnels->key), &tunskey, sizeof( TUNNELCH_KEY ) );
            spm_hashtab_insert (SHARED.tunnelchhash ,  &(tunnels->key), tunnels NBB_CCXT);

            pwnode = (PWTXNODE *)NBB_MM_ALLOC(sizeof(PWTXNODE), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_PWNODE_CB);
            if(NULL == pwnode)
            {
                OS_PRINTF("Line=%d pwnode malloc mem NULL \n",__LINE__);
                rv = ADD_UCROUTE_MALLOC_PWNODE_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e malloc pwnode null \n", __LINE__);
                goto  EXIT_LABEL;
            }
            NBB_MEMSET(pwnode, 0, sizeof(PWTXNODE));
            AVLL_INIT_NODE(pwnode->pw_node);
            pwnode->key.label = label;
            pwnode->posid = txpw.posId;
            pwnode->nhi = txpw.nhIdx;   
            pwnode->routenum = 1;
            *nhi = txpw.nhIdx;
            
            //coverity[no_effect_test]
            AVLL_INSERT( tunnels->pwtree ,  pwnode->pw_node);
            nhid = pwnode->posid;

            OS_MEMSET( &vrfinskey, 0, sizeof( VRFINS_KEY ) );
            vrfinskey.vrfid = vrfid;
            vrfinskey.peerip = peerip;
            vrfins =  (VRFINS *)AVLL_FIND( SHARED.vrfins_tree ,  &vrfinskey ); 
            if(NULL == vrfins)
            {
            }
            else
            {
                if (vrfins->difs.dserid != 0)
                {
                    OS_MEMSET( &qosvrfinskey, 0, sizeof( SPM_QOS_VRF_INSTANSE_KEY ) );
                    qosvrfinskey.vrf_id = vrfid;
                    qosvrfinskey.peer_ip = peerip;
                    qosvrfinskey.label = label;
                   // rv = spm_ds_add_vrf_node( vrfins->difs.dserid, &qosvrfinskey, txpw.posId NBB_CCXT);
                    if(rv != 0)
                    {
                        rv = ADD_UCROUTE_DS_VRF_ERR;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e pwds rv.posid=%d.%d\n", __LINE__, rv,txpw.posId);
                    }
                }
                if (vrfins->upqos.nodeindex != 0)
                {
                    OS_MEMSET(&txlspkey, 0, sizeof(CRTXLSP_KEY));
                    OS_MEMSET(&portinfo,0,sizeof(SPM_PORT_INFO_CB));
                    OS_MEMSET( &qostunl, 0, sizeof( SPM_QOS_TUNNEL_KEY ) );
                    OS_MEMCPY( &vrfupqos, &(vrfins->upqos), sizeof( ATG_DCI_VRF_INSTANCE_UP_VPN_QOS ) );
                    OS_MEMSET( &qosvrfinskey, 0, sizeof( SPM_QOS_VRF_INSTANSE_KEY ) );
                    qosvrfinskey.vrf_id = vrfid;
                    qosvrfinskey.peer_ip = peerip;
                    qosvrfinskey.label = label;
                    
                    if(tunnels->base.tunnelid != 0)
                    {
                        rv = spm_l3_peerforhqos(vrfid,peerip,&txlspkey,&portinfo NBB_CCXT);
                        if(rv != 0)
                        {
                           BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, 
                              "spm_l3_peerforhqos err,Line=%d,rv =%d\n",__LINE__,rv);
                        }
                        qostunl.flag = 1;
                        qostunl.type = 1;
                        OS_MEMCPY( &(qostunl.tx_lsp), &txlspkey, sizeof( CRTXLSP_KEY ) );
                    }
                    else
                    {
                         OS_MEMSET(&ftnkey, 0, sizeof(FTN_KEY));
                         ftnkey.vrfid = 0;
                         ftnkey.mask = 32;
                         ftnkey.fec = peerip;
                         ftnkey.res = 0;
                         uftn =  (FTN_TAB *)AVLL_FIND( SHARED.ftn_tree ,  &ftnkey ); 
                         if(uftn == NULL )
                         {
                             rv = ADD_UCROUTE_GET_PWV4_FTN_NULL;
                             BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e uftn null rv =.%d\n", __LINE__, rv);
                             goto  EXIT_LABEL;
                         }
                         else
                         {
                             rv = spm_get_portid_from_logical_port_index(uftn->dclsp[0].nextport,&portinfo NBB_CCXT);
                             if(rv != 0)
                             {
                                
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, 
                                        "spm_get_portid_from_logical_port_index err,Line=%d,rv =%d\n",__LINE__,rv);

                                rv = ADD_UCROUTE_GET_PWV4_L2PORT_ERR;
                             }
                             else
                             {
                                 qostunl.flag = 1;
                                 qostunl.type = 0 ;
                                 OS_MEMCPY( &(qostunl.ftn), &ftnkey, sizeof( FTN_KEY ) );
                             }
                            
                         }
                    }
                   // rv2 = spm_hqos_add_vrf_node( portinfo.slot_id, portinfo.port_id, nhid, 
                   //           &qostunl,&qosvrfinskey,&vrfupqos NBB_CCXT);

                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e pwds rv.posid=%d.%d\n",
                                                __LINE__, rv,txpw.posId);
                }
            }
        }
        else
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getpwv4 vrf.peer.label =%d.%x.%x \n", __LINE__, 
                                                                                vrfid,
                                                                                peerip,
                                                                                label);
            rv = ADD_UCROUTE_GET_PWV4_DRV_ERR;
            goto  EXIT_LABEL;
        }
    }
    else
    {
        tunnels = (TUNNELCH *)(node->data);
        if ( tunnels == NULL )
        {
            rv = ADD_UCROUTE_GET_PWV4_TUNNEL_NULL;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d tunnel null rv =.%d\n", __LINE__, rv);
            goto  EXIT_LABEL;
        } 
        pwkey.label = label;
        pwnode =  (PWTXNODE *)AVLL_FIND( tunnels->pwtree ,  &pwkey );
        if ( pwnode == NULL )
        {
            #ifdef SPU
                OS_MEMSET(&txpw , 0, sizeof(L3_TX_PW_T));
            
                txpw.mVrf = vrfid;
                txpw.mPeerIp = peerip;
                txpw.mLabel = label;
                
                //txpw.flags |= L3_TX_PW_COUNTER_ON;
                txpw.pushPwNum = 1;
                txpw.tunnelIdx = tunnels->tunindex; 
                txpw.eNniMode = PHB_TO_EXP;
                
                rv = ApiC3AddL3TxPw( BCMC31, &txpw);
                if ( SHARED.c3_num == 2 )
                {
                    rv2 = ApiC3AddL3TxPw( BCMC32, &txpw);
                    rv = (rv|rv2);
                }
            #endif

            #ifdef SRC
                txpw.posId = 0xffff;
                txpw.nhIdx = 0xffffffff;
                rv = 0;
            #endif
            #ifdef SNU
                txpw.posId = 0xffff;
                txpw.nhIdx = 0xffffffff;
                rv = 0;
            #endif
            if( rv == 0)
            {
                tunnels->pwnum++;
                pwnode = (PWTXNODE *)NBB_MM_ALLOC(sizeof(PWTXNODE), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_PWNODE_CB);
                if(NULL == pwnode)
                {
                    OS_PRINTF("Line=%d pwnode malloc mem NULL \n",__LINE__);
                    rv = ADD_UCROUTE_MALLOC_PWNODE_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d malloc pwnode null rv =.%d\n", __LINE__, rv);
                    goto  EXIT_LABEL;
                }
                NBB_MEMSET(pwnode, 0, sizeof(PWTXNODE));
                AVLL_INIT_NODE(pwnode->pw_node);
                pwnode->key.label = label;
                pwnode->posid = txpw.posId;
                pwnode->nhi = txpw.nhIdx;   
                pwnode->routenum = 1;
                *nhi = txpw.nhIdx;
                AVLL_INSERT( tunnels->pwtree ,  pwnode->pw_node);
                nhid = pwnode->posid;

                OS_MEMSET( &vrfinskey, 0, sizeof( VRFINS_KEY ) );
                vrfinskey.vrfid = vrfid;
                vrfinskey.peerip = peerip;
                vrfins =  (VRFINS *)AVLL_FIND( SHARED.vrfins_tree ,  &vrfinskey ); 
                if(NULL == vrfins)
                {
                }
                else
                {
                    if (vrfins->difs.dserid != 0)
                    {
                        OS_MEMSET( &qosvrfinskey, 0, sizeof( SPM_QOS_VRF_INSTANSE_KEY ) );
                        qosvrfinskey.vrf_id = vrfid;
                        qosvrfinskey.peer_ip = peerip;
                        qosvrfinskey.label = label;
                       // rv = spm_ds_add_vrf_node( vrfins->difs.dserid, &qosvrfinskey, txpw.posId NBB_CCXT);
                        if(rv != 0)
                        {

                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e pwds rv.posid=%d.%d\n", 
                                                        __LINE__, rv,txpw.posId);
                            rv = ADD_UCROUTE_DS_VRF_ERR;
                        }
                    }
                    if (vrfins->upqos.nodeindex != 0)
                    {
                        OS_MEMSET(&txlspkey, 0, sizeof(CRTXLSP_KEY));
                        OS_MEMSET(&portinfo,0,sizeof(SPM_PORT_INFO_CB));
                        OS_MEMSET( &qostunl, 0, sizeof( SPM_QOS_TUNNEL_KEY ) );
                        OS_MEMCPY( &vrfupqos, &(vrfins->upqos), sizeof( ATG_DCI_VRF_INSTANCE_UP_VPN_QOS ) );
                        OS_MEMSET( &qosvrfinskey, 0, sizeof( SPM_QOS_VRF_INSTANSE_KEY ) );
                        qosvrfinskey.vrf_id = vrfid;
                        qosvrfinskey.peer_ip = peerip;
                        qosvrfinskey.label = label;
                        if(tunnels->base.tunnelid != 0)
                        {
                            rv = spm_l3_peerforhqos(vrfid,peerip,&txlspkey,&portinfo NBB_CCXT);
                            if(rv != 0)
                            {
                               BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, 
                                  "spm_l3_peerforhqos err,Line=%d,rv =%d\n",__LINE__,rv);
                            }
                            qostunl.flag = 1;
                            qostunl.type = 1;
                            OS_MEMCPY( &(qostunl.tx_lsp), &txlspkey, sizeof( CRTXLSP_KEY ) );
                        }
                        else
                        {
                            OS_MEMSET(&ftnkey, 0, sizeof(FTN_KEY));
                            ftnkey.vrfid = 0;
                            ftnkey.mask = 32;
                            ftnkey.fec = peerip;
                            ftnkey.res = 0;
                            uftn =  (FTN_TAB *)AVLL_FIND( SHARED.ftn_tree ,  &ftnkey ); 
                            if(uftn == NULL )
                            {
                                rv = ADD_UCROUTE_GET_PWV4_FTN_NULL;
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e uftn null rv =.%d\n", __LINE__, rv);
                                goto  EXIT_LABEL;
                            }
                            else
                            {
                                rv = spm_get_portid_from_logical_port_index(uftn->dclsp[0].nextport,&portinfo NBB_CCXT);
                                if(rv != 0)
                                {
                                      BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, 
                                         "spm_get_portid_from_logical_port_index err,Line=%d,rv =%d\n",__LINE__,rv);
                                  rv = rv = ADD_UCROUTE_GET_PWV4_L2PORT_ERR;
                                }
                                else
                                {
                                   qostunl.flag = 1;
                                   qostunl.type = 0 ;
                                   OS_MEMCPY( &(qostunl.ftn), &ftnkey, sizeof( FTN_KEY ) );
                                }
                            }
                    }
                   // rv2 = spm_hqos_add_vrf_node( portinfo.slot_id, portinfo.port_id, nhid, 
                   //                                                     &qostunl,&qosvrfinskey,&vrfupqos NBB_CCXT);

                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e pwds rv.posid=%d.%d\n", 
                                                __LINE__, rv,txpw.posId);
                }
                }
            }
            else
            {
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e gpwv4 vrf.peer.label =%d.%x.%x \n",
                                            __LINE__, vrfid,peerip,label);
                 rv = ADD_UCROUTE_GET_PWV4_DRV_ERR;
                goto  EXIT_LABEL;
            }
        } 
        else
        {
            pwnode->routenum++;
            *nhi = pwnode->nhi;
            rv = 0;
        }
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
    
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_delpwv4( NBB_USHORT vrfid ,NBB_ULONG peerip, NBB_ULONG label NBB_CCXT_T NBB_CXT)
{
    NBB_INT             rv = 100;
    NBB_INT             rv2 = 100;
    
    SHASH_NODE          *node = NULL;
    
    TUNNELCH            *tunnels = NULL;

    TUNNELCH_KEY        tunskey;

    PWTXNODE            *pwnode = NULL;

    PW_KEY              pwkey;
	LSPPROT_KEY         tunnelkey;

    NBB_TRC_ENTRY("spm_l3_delpwv4");


    if( label == 0xffffffff )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delpwv4 label = 0xffffffff \n", __LINE__);
        rv = ADD_UCROUTE_DEL_PW_PARA_ERR;
        goto  EXIT_LABEL;
    }
    if( peerip == 0 )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delpwv4 peerip = 0 \n", __LINE__);
        rv = ADD_UCROUTE_DEL_PW_PARA_ERR;
        goto  EXIT_LABEL;
    }

	OS_MEMSET(&tunnelkey , 0, sizeof(LSPPROT_KEY));
    tunskey.res = 0;
    tunskey.vrfid = vrfid;
    tunskey.peerip = peerip;
    node =  (SHASH_NODE *)spm_hashtab_search( SHARED.tunnelchhash ,  &tunskey NBB_CCXT); 
    if( node == NULL )
    {
        NBB_TRC_FLOW((NBB_FORMAT "Del pwv4 null"));
        rv = ADD_UCROUTE_DEL_PW_NODE_NULL;
        goto  EXIT_LABEL;
    }
    if( node->data == NULL )
    {
        NBB_TRC_FLOW((NBB_FORMAT "Del pwv4 null"));
        rv = ADD_UCROUTE_DEL_PW_NODE_DATA_NULL;
        goto  EXIT_LABEL;
    }
    tunnels = (TUNNELCH *)(node->data);
    pwkey.label = label;
    pwnode =  (PWTXNODE *)AVLL_FIND( tunnels->pwtree ,  &pwkey );
    if ( pwnode != NULL )
    {
        if( pwnode->routenum == 1) 
        {
        #ifdef SPU
            rv = ApiC3DelL3TxPw( BCMC31, pwnode->posid );
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3DelL3TxPw( BCMC32, pwnode->posid );
                rv = (rv|rv2);
            }
        #endif
        #ifdef SRC
            rv = 0;
        #endif  
        #ifdef SNU
            rv = 0;
        #endif  
            AVLL_DELETE( tunnels->pwtree , pwnode->pw_node);
            NBB_MM_FREE(pwnode, MEM_SPM_VRFUROUTE_PWNODE_CB);
            tunnels->pwnum--;
        }
        else
        {
            pwnode->routenum--;
            rv = 0;
        }
    }
    else
    {
        rv = ADD_UCROUTE_DEL_PW_TREE_DATA_NULL;

        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e dpwv4 vrf.peer.label =%d.%x.%x \n", __LINE__, vrfid,
                                                                                peerip,
                                                                                label);
        NBB_TRC_FLOW((NBB_FORMAT "Del pwv4 null"));
        goto  EXIT_LABEL;
    }
    if( tunnels->pwnum == 0) 
    {
        if( tunnels->delflag == 100) 
        {

			//删除tunnels
			tunnelkey.ingress = tunnels->base.ingress;
			tunnelkey.egress = tunnels->base.egress;
			tunnelkey.tunnelid = tunnels->base.tunnelid;
            spm_hashtab_del( SHARED.tunnelchhash ,&tunskey NBB_CCXT);
            NBB_MM_FREE(tunnels, MEM_SPM_TUNNELCH_CB);

			/*ldp*/
			if(tunnelkey.egress == 0)
			{
                spm_l3_delvrfldp( peerip, vrfid NBB_CCXT);
			}
			else
			{
                spm_l3_delvrfrsvp(peerip,vrfid,&tunnelkey NBB_CCXT);
			}
        }
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_mllocfrrid( NBB_ULONG mpeer, NBB_ULONG bpeer,NBB_ULONG lgport,NBB_USHORT *frrid NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    NBB_USHORT frrindex = 0;

    PEER_KEY    mpeerkey;
    PEER_KEY    bpeerkey;

    MPEERFRR    *mpeerfrr = NULL;

    BPEERFRR    *bpeerfrr = NULL;

    SPM_LINEAR_PROTECT_SWITCH   stLps;

    NBB_TRC_ENTRY("spm_l3_mllocfrrid");

    if (frrid == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mllocfrrid frrid = NULL \n", __LINE__);
        rv = ADD_UCROUTE_VPN_FRR_PARA_ERR;
        goto  EXIT_LABEL;
    }

    OS_MEMSET(&mpeerkey, 0, sizeof(PEER_KEY));
    mpeerkey.peer = mpeer;
    mpeerfrr =  (MPEERFRR *)AVLL_FIND( SHARED.peer_tree ,  &mpeerkey );
    if( mpeerfrr == NULL )
    {
        #ifdef SPU
            rv = ApiC3CreateFrrGroup( BCMC31, &frrindex, 0, 0);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3CreateFrrGroup( BCMC32, &frrindex, 0, 0);
                rv = (rv|rv2);
            }
            if( rv != 0)   
            {
                //OS_PRINTF("Line=%d error mllocfrrid mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mfrrid n mpeer.b.rv =%x.%x.%d\n", 
                                            __LINE__, mpeer,bpeer,rv);
                rv = ADD_UCROUTE_VPN_FRR_DRV_FRR_ERR;
                goto  EXIT_LABEL;
            }
            else
            {
                //OS_PRINTF("Line=%d ok mllocfrrid mpeer=%x bpeer=%x frr=%d\n",__LINE__, mpeer ,bpeer ,frrindex);
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o mfrrid n mpeer.b.frr =%x.%x.%d\n", 
                                            __LINE__, mpeer,bpeer,frrindex);
            }
        #endif
        #ifdef SRC
            rv = 0;
        #endif
        #ifdef SNU
            rv = 0;
        #endif  
        if( rv == 0)
        {
            mpeerfrr = (MPEERFRR *)NBB_MM_ALLOC(sizeof(MPEERFRR), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_MPEER_CB);
            if(NULL == mpeerfrr)
            {
                OS_PRINTF("Line=%d mpeerfrr malloc mem NULL \n",__LINE__);
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e malloc mpeer null \n", __LINE__);
                rv = ADD_UCROUTE_VPN_FRR_MALLOC_MPEER_NULL;
                goto  EXIT_LABEL;
            }
            NBB_MEMSET(mpeerfrr, 0, sizeof(MPEERFRR));
            AVLL_INIT_NODE(mpeerfrr->mp_node);  
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(mpeerfrr->key), &mpeerkey, sizeof(PEER_KEY));
            
            AVLL_INIT_TREE(mpeerfrr->bpeer_tree,
                                spm_l3_peercomp,
                                NBB_OFFSETOF(BPEERFRR, key),
                                NBB_OFFSETOF(BPEERFRR, bp_node));
            mpeerfrr->bpeernum = 1;
            
            //coverity[no_effect_test]
            AVLL_INSERT( SHARED.peer_tree, mpeerfrr->mp_node);

            OS_MEMSET(&bpeerkey, 0, sizeof(PEER_KEY));
            bpeerkey.peer = bpeer;
            bpeerfrr = (BPEERFRR *)NBB_MM_ALLOC(sizeof(BPEERFRR), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_BPEER_CB);
            if(NULL == bpeerfrr)
            {
                OS_PRINTF("Line=%d bpeerfrr malloc mem NULL \n",__LINE__);
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e malloc bpeer null \n", __LINE__);
                rv = ADD_UCROUTE_VPN_FRR_MALLOC_BPEER_NULL;
                goto  EXIT_LABEL;
            }
            NBB_MEMSET(bpeerfrr, 0, sizeof(BPEERFRR));
            AVLL_INIT_NODE(bpeerfrr->bp_node);  
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(bpeerfrr->key), &bpeerkey, sizeof(PEER_KEY));
            bpeerfrr->frrid = frrindex;
            bpeerfrr->frrnum = 1;
            AVLL_INSERT( mpeerfrr->bpeer_tree, bpeerfrr->bp_node);
            
            *frrid = frrindex;

            OS_MEMSET(&stLps, 0, sizeof(SPM_LINEAR_PROTECT_SWITCH));
            stLps.revertiveType = LPS_REVERTIVE;
            stLps.holdOffTime = 0;
            stLps.wtrTime = 5;

            stLps.workEntity.portIndex = lgport;
            
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o portindex =.%d\n", __LINE__, lgport);
            stLps.workEntity.port = 0;
            stLps.workEntity.slot = 0;
            stLps.workEntity.tpId = 0;
            stLps.workEntity.type = LPS_LDP_LSP;
            stLps.workEntity.ldpKey.vrf_id = 0;
            stLps.workEntity.ldpKey.fec_prefix_len = 32;
            stLps.workEntity.ldpKey.fec_prefix = mpeer;
            if( mpeer == 0 )
            {
                stLps.workEntity.ldpKey.fec_prefix = bpeer;
            }

            stLps.protEntity.port = 0;
            stLps.protEntity.slot = 0;
            stLps.protEntity.tpId = 0;
            stLps.protEntity.type = LPS_LDP_LSP;
            stLps.protEntity.ldpKey.vrf_id = 0;
            stLps.protEntity.ldpKey.fec_prefix_len = 32;
            stLps.protEntity.ldpKey.fec_prefix = bpeer;
            if( bpeer == 0 )
            {
                stLps.protEntity.ldpKey.fec_prefix = mpeer;
            }

            stLps.protType = FRR_TYPE;
            stLps.protGroupId = frrindex;
            stLps.lpsType = FRR_1B1;
            
            spm_lps_add_entity(&stLps);

        }
    }
    else
    {
        OS_MEMSET(&bpeerkey, 0, sizeof(PEER_KEY));
        bpeerkey.peer = bpeer;
        bpeerfrr =  (BPEERFRR *)AVLL_FIND( mpeerfrr->bpeer_tree,  &bpeerkey );
        if( bpeerfrr == NULL )
        {
            #ifdef SPU
                rv = ApiC3CreateFrrGroup( BCMC31, &frrindex, 0, 0);
                if ( SHARED.c3_num == 2 )
                {
                    rv2 = ApiC3CreateFrrGroup( BCMC32, &frrindex, 0, 0);
                    rv = (rv|rv2);
                }
                if( rv != 0)   
                {
                    //OS_PRINTF("Line=%d error mllocfrrid mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e mfrrid n mpeer.b.rv =%x.%x.%d\n", 
                                                __LINE__, mpeer,bpeer,rv);
                    rv = ADD_UCROUTE_VPN_FRR_DRV_FRR_ERR;
                    goto  EXIT_LABEL;
                }
                else
                {
                    //OS_PRINTF("Line=%d ok mllocfrrid mpeer=%x bpeer=%x frr=%d\n",__LINE__, mpeer ,bpeer ,frrindex);
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o mfrrid n mpeer.b.frr =%x.%x.%d\n", 
                                                __LINE__, mpeer,bpeer,frrindex);
                }
            #endif
            #ifdef SRC
                rv = 0;
            #endif  
            #ifdef SNU
                rv = 0;
            #endif  
            if( rv == 0)
            {
                bpeerfrr = (BPEERFRR *)NBB_MM_ALLOC(sizeof(BPEERFRR), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_BPEER_CB);
                if(NULL == bpeerfrr)
                {
                    OS_PRINTF("Line=%d bpeerfrr malloc mem NULL \n",__LINE__);
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e malloc bpeer null \n", __LINE__);
                    rv = ADD_UCROUTE_VPN_FRR_MALLOC_BPEER_NULL;
                    goto  EXIT_LABEL;
                }
                NBB_MEMSET(bpeerfrr, 0, sizeof(BPEERFRR));
                AVLL_INIT_NODE(bpeerfrr->bp_node);  
                
                //coverity[bad_sizeof]
                NBB_MEMCPY( &(bpeerfrr->key), &bpeerkey, sizeof(PEER_KEY));
                bpeerfrr->frrid = frrindex;
                bpeerfrr->frrnum = 1;
                AVLL_INSERT( mpeerfrr->bpeer_tree, bpeerfrr->bp_node);
                
                *frrid = frrindex;
            
                mpeerfrr->bpeernum++;

                OS_MEMSET(&stLps, 0, sizeof(SPM_LINEAR_PROTECT_SWITCH));
                stLps.revertiveType = LPS_REVERTIVE;
                stLps.holdOffTime = 0;
                stLps.wtrTime = 5;

                stLps.workEntity.portIndex = lgport;
                stLps.workEntity.port = 0;
                stLps.workEntity.slot = 0;
                stLps.workEntity.tpId = 0;
                stLps.workEntity.type = LPS_LDP_LSP;
                stLps.workEntity.ldpKey.vrf_id = 0;
                stLps.workEntity.ldpKey.fec_prefix_len = 32;
                stLps.workEntity.ldpKey.fec_prefix = mpeer;
                if( mpeer == 0 )
                {
                    stLps.workEntity.ldpKey.fec_prefix = bpeer;
                }

                stLps.protEntity.port = 0;
                stLps.protEntity.slot = 0;
                stLps.protEntity.tpId = 0;
                stLps.protEntity.type = LPS_LDP_LSP;
                stLps.protEntity.ldpKey.vrf_id = 0;
                stLps.protEntity.ldpKey.fec_prefix_len = 32;
                stLps.protEntity.ldpKey.fec_prefix = bpeer;
                if( bpeer == 0 )
                {
                    stLps.protEntity.ldpKey.fec_prefix = mpeer;
                }

                stLps.protType = FRR_TYPE;
                stLps.protGroupId = frrindex;
                stLps.lpsType = FRR_1B1;
                
                spm_lps_add_entity(&stLps);
            }
        }
        else
        {
            *frrid = bpeerfrr->frrid;
            bpeerfrr->frrnum++;
            
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o mfrrid o mpeer.b.cite =%x.%x.%d\n", 
            //                          __LINE__, mpeer,bpeer,bpeerfrr->frrnum);
        }
    }
    rv = 0;
    
    EXIT_LABEL:
    
    NBB_TRC_EXIT();
    
    return rv;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_freefrrid(NBB_ULONG mpeer, NBB_ULONG bpeer NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    PEER_KEY    mpeerkey;
    PEER_KEY    bpeerkey;

    MPEERFRR    *mpeerfrr = NULL;

    BPEERFRR    *bpeerfrr = NULL;

    NBB_TRC_ENTRY("spm_l3_freefrrid");
        
    mpeerkey.peer = mpeer;
    bpeerkey.peer = bpeer;
    mpeerfrr =  (MPEERFRR *)AVLL_FIND( SHARED.peer_tree ,  &mpeerkey );
    if( mpeerfrr == NULL )
    {
        rv = ADD_UCROUTE_FREE_MPEER_DATA_NULL;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ffrrid mpeer.b =%x.%x \n", __LINE__, mpeer,bpeer);
        
        //OS_PRINTF("Line=%d error freefrrid mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
        goto  EXIT_LABEL;
    }
    else
    {
        bpeerfrr =  (BPEERFRR *)AVLL_FIND( mpeerfrr->bpeer_tree,  &bpeerkey );
        if( bpeerfrr == NULL )
        {
            rv = ADD_UCROUTE_FREE_BPEER_DATA_NULL;
            
            //OS_PRINTF("Line=%d error freefrrid mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ffrrid mpeer.b =%x.%x \n", __LINE__, mpeer,bpeer);
            goto  EXIT_LABEL;
        }
        if( bpeerfrr->frrnum == 1) 
        {
        #ifdef SPU
            rv = ApiC3DeleteFrrGroup( BCMC31, bpeerfrr->frrid);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3DeleteFrrGroup( BCMC32, bpeerfrr->frrid);
                rv = (rv|rv2);
            }
            spm_lps_del_entity( FRR_TYPE, bpeerfrr->frrid);
        #endif
        #ifdef SRC
            rv = 0;
        #endif  
        #ifdef SNU
            rv = 0;
        #endif  
            //OS_PRINTF("Line=%d ok freefrrid frr=%d\n",__LINE__,bpeerfrr->frrid);
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o ffrrid n mpeer.b.frr =%x.%x.%d \n", 
                                        __LINE__,mpeer,bpeer,bpeerfrr->frrid);
            AVLL_DELETE( mpeerfrr->bpeer_tree , bpeerfrr->bp_node);
            NBB_MM_FREE(bpeerfrr, MEM_SPM_VRFUROUTE_BPEER_CB);
            mpeerfrr->bpeernum--;
        }
        else
        {
            bpeerfrr->frrnum--;
            
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o ffrrid o mpeer.b.cite =%x.%x.%d\n", 
            //                          __LINE__, mpeer,bpeer,bpeerfrr->frrnum);
            rv = 0;
        }
    }
    if( mpeerfrr->bpeernum == 0) 
    {
        AVLL_DELETE( SHARED.peer_tree, mpeerfrr->mp_node);
        NBB_MM_FREE(mpeerfrr, MEM_SPM_VRFUROUTE_MPEER_CB);
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_bfdgetfrrid( NBB_USHORT bfdid,NBB_USHORT *frrid, NBB_ULONG logport NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    NBB_USHORT  frrindex = 0;

    BFDFRR_KEY  bfdkey;

    BFDFRR      *bfdfrrp = NULL;

    SPM_LINEAR_PROTECT_SWITCH   stLps;

    NBB_TRC_ENTRY("spm_l3_bfdgetfrrid");


    if( bfdid == 0 )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e bfdgetfrrid bfdid = 0 \n", __LINE__);
        rv = ADD_UCROUTE_BFD_FRR_PARA_ERR;
        goto  EXIT_LABEL;
    }

    if (frrid == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e bfdgetfrrid frrid = NULL \n", __LINE__);
        rv = ADD_UCROUTE_BFD_FRR_PARA_ERR;
        goto  EXIT_LABEL;
    }

    OS_MEMSET(&bfdkey, 0, sizeof(BFDFRR_KEY));
    bfdkey.bfdid = bfdid;
    bfdfrrp =  (BFDFRR *)AVLL_FIND( SHARED.bfdfrr_tree ,  &bfdkey );
    if( bfdfrrp == NULL )
    {
        #ifdef SPU
            rv = ApiC3CreateFrrGroup( BCMC31, &frrindex, 0, 0);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3CreateFrrGroup( BCMC32, &frrindex, 0, 0);
                rv = (rv|rv2);
            }
            if( rv != 0)   
            {
                //OS_PRINTF("Line=%d error mllocfrrid mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e a bfdfrrid bfdid.rv =%d.%d\n", __LINE__, bfdid,rv);
                rv = ADD_UCROUTE_DRV_CREATE_FRR_ERR;
                goto  EXIT_LABEL;
            }
            else
            {
                //OS_PRINTF("Line=%d ok mllocfrrid mpeer=%x bpeer=%x frr=%d\n",__LINE__, mpeer ,bpeer ,frrindex);
                //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o a bfdfrrid bfdid.frr =%d.%d\n", 
                //                          __LINE__, bfdid,frrindex);
            }
        #endif
        #ifdef SRC
            rv = 0;
        #endif
        #ifdef SNU
            rv = 0;
        #endif  
        if( rv == 0)
        {
            bfdfrrp = (BFDFRR *)NBB_MM_ALLOC(sizeof(BFDFRR), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_BFDFRR_CB);
            if(NULL == bfdfrrp)
            {
                OS_PRINTF("Line=%d bfdfrrp malloc mem NULL \n",__LINE__);
                rv = ADD_UCROUTE_BFD_FRR_MALLOC_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e malloc bfdfrr null.rv =%d\n", __LINE__,rv);
                goto  EXIT_LABEL;
            }
            NBB_MEMSET(bfdfrrp, 0, sizeof(BFDFRR));
            AVLL_INIT_NODE(bfdfrrp->bf_node);  
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(bfdfrrp->key), &bfdkey, sizeof(BFDFRR_KEY));
            

            bfdfrrp->citenum = 1;
            
            //coverity[no_effect_test]
            AVLL_INSERT( SHARED.bfdfrr_tree, bfdfrrp->bf_node);

            bfdfrrp->frrid = frrindex;
            *frrid = frrindex;

            OS_MEMSET(&stLps, 0, sizeof(SPM_LINEAR_PROTECT_SWITCH));
            stLps.revertiveType = LPS_REVERTIVE;
            stLps.holdOffTime = 0;
            stLps.wtrTime = 0;

            stLps.workEntity.portIndex = logport;
            stLps.workEntity.tpId = bfdid;
            stLps.workEntity.type = LPS_NONE_TYPE;

            stLps.protEntity.portIndex = logport;
            stLps.protEntity.tpId = 0;
            stLps.protEntity.type = LPS_NONE_TYPE;

            stLps.protType = FRR_TYPE;
            stLps.protGroupId = frrindex;
            stLps.lpsType = FRR_1B1;

            spm_lps_add_entity(&stLps);
        }
    }
    else
    {
        *frrid = bfdfrrp->frrid;
        bfdfrrp->citenum++;
    }
    rv = 0;
    
    EXIT_LABEL:
    
    NBB_TRC_EXIT();
    
    return rv;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_bfddelfrrid(NBB_USHORT bfdid NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    BFDFRR_KEY  bfdkey;

    BFDFRR  *bfdfrrp = NULL;

    NBB_TRC_ENTRY("spm_l3_bfddelfrrid");

    if( bfdid == 0 )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e bfddelfrrid bfdid = 0 \n", __LINE__);
        rv = ADD_UCROUTE_FREE_BFD_FRR_PARA_ERR;
        goto  EXIT_LABEL;
    }
        
    bfdkey.res = 0;
    bfdkey.bfdid = bfdid;
    bfdfrrp =  (BFDFRR *)AVLL_FIND( SHARED.bfdfrr_tree ,  &bfdkey );
    if( bfdfrrp == NULL )
    {
        rv = ADD_UCROUTE_FREE_BFD_FRR_NULL;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d bfdfrrid bfdid.rv =%x.%x \n", __LINE__, bfdid,rv);
        
        //OS_PRINTF("Line=%d error freefrrid mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
        goto  EXIT_LABEL;
    }
    else
    {
        if( bfdfrrp->citenum == 1) 
        {
        #ifdef SPU
            rv = ApiC3DeleteFrrGroup( BCMC31, bfdfrrp->frrid);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3DeleteFrrGroup( BCMC32, bfdfrrp->frrid);
                rv = (rv|rv2);
            }
        #endif
        #ifdef SRC
            rv = 0;
        #endif  
        #ifdef SNU
            rv = 0;
        #endif  
            spm_lps_del_entity( FRR_TYPE, bfdfrrp->frrid);
            
            //OS_PRINTF("Line=%d ok freefrrid frr=%d\n",__LINE__,bpeerfrr->frrid);
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o d bfdfrrid bfdid.frr =%d.%d\n", 
            //                          __LINE__, bfdid,bfdfrrp->frrid);
            AVLL_DELETE(  SHARED.bfdfrr_tree , bfdfrrp->bf_node);
            NBB_MM_FREE(bfdfrrp, MEM_SPM_VRFUROUTE_BFDFRR_CB);
            
        }
        else
        {
            bfdfrrp->citenum--;
            rv = 0;
        }
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_ipgetfrrid( NBB_USHORT vrfid,NBB_ULONG mpeer,NBB_ULONG mport,
                        NBB_ULONG bpeer, NBB_ULONG bport, NBB_USHORT *frrid NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    NBB_USHORT  frrindex = 0;

    IPFRR_KEY  ipfrrkey;

    IPFRR      *ipfrrp = NULL;

    SPM_LINEAR_PROTECT_SWITCH   stLps;

    NBB_TRC_ENTRY("spm_l3_ipgetfrrid");


    if( mport == 0 )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ipgetfrrid mport = 0 \n", __LINE__);
        rv = ADD_UCROUTE_IP_FRR_PARA_ERR;
        goto  EXIT_LABEL;
    }
    
    if( bport == 0 )
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ipgetfrrid bport = 0 \n", __LINE__);
        rv = ADD_UCROUTE_IP_FRR_PARA_ERR;
        goto  EXIT_LABEL;
    }

    if (frrid == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ipgetfrrid frrid = NULL \n", __LINE__);
        rv = ADD_UCROUTE_IP_FRR_PARA_ERR;
        goto  EXIT_LABEL;
    }

    OS_MEMSET(&ipfrrkey, 0, sizeof(IPFRR_KEY));
    ipfrrkey.vrfid = vrfid;
    ipfrrkey.mpeer = mpeer;
    ipfrrkey.bpeer = bpeer;

    ipfrrp =  (IPFRR *)AVLL_FIND( SHARED.ipfrr_tree ,  &ipfrrkey );
    if( ipfrrp == NULL )
    {
        #ifdef SPU
            rv = ApiC3CreateFrrGroup( BCMC31, &frrindex, 0, 0);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3CreateFrrGroup( BCMC32, &frrindex, 0, 0);
                rv = (rv|rv2);
            }
            if( rv != 0)   
            {
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e a ipfrrid vrf.m.b.rv =%d.%d.%d.%d\n", 
                                            __LINE__, vrfid,mpeer,bpeer,rv);
                rv = ADD_UCROUTE_IP_FRR_DRV_FRR_ERR;
                goto  EXIT_LABEL;
            }
            else
            {
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o a ipfrrid vrf.m.b.frr ==%d.%d.%d.%d\n", 
                                            __LINE__, vrfid,mpeer,bpeer,frrindex);
            }
        #endif
        #ifdef SRC
            rv = 0;
        #endif
        #ifdef SNU
            rv = 0;
        #endif  
        if( rv == 0)
        {
            ipfrrp = (IPFRR *)NBB_MM_ALLOC(sizeof(IPFRR), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_IPFRR_CB);
            if(NULL == ipfrrp)
            {
                OS_PRINTF("Line=%d ipfrrp malloc mem NULL \n",__LINE__);
                rv = ADD_UCROUTE_IP_FRR_MALLOC_NULL;
                goto  EXIT_LABEL;
            }
            NBB_MEMSET(ipfrrp, 0, sizeof(IPFRR));
            AVLL_INIT_NODE(ipfrrp->ipf_node);  
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(ipfrrp->key), &ipfrrkey, sizeof(IPFRR_KEY));
            

            ipfrrp->citenum = 1;
            
            //coverity[no_effect_test]
            AVLL_INSERT( SHARED.ipfrr_tree, ipfrrp->ipf_node);

            ipfrrp->frrid = frrindex;
            *frrid = frrindex;

            OS_MEMSET(&stLps, 0, sizeof(SPM_LINEAR_PROTECT_SWITCH));
            stLps.revertiveType = LPS_REVERTIVE;
            stLps.holdOffTime = 0;
            stLps.wtrTime = 5;
            
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
            stLps.workEntity.routeKey.dip[3] = mpeer;
            
            stLps.protEntity.portIndex = bport;
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
            stLps.protEntity.routeKey.dip[3] = bpeer;
            
            stLps.protType = FRR_TYPE;
            stLps.protGroupId = frrindex;
            stLps.lpsType = FRR_1B1;
            
            spm_lps_add_entity(&stLps);
        }
    }
    else
    {
        *frrid = ipfrrp->frrid;
        ipfrrp->citenum++;
    }
    rv = 0;
    
    EXIT_LABEL:
    
    NBB_TRC_EXIT();
    
    return rv;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_ipdelfrrid(NBB_USHORT vrfid,NBB_ULONG mpeer,NBB_ULONG bpeer NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    IPFRR_KEY  ipfrrkey;

    IPFRR      *ipfrrp = NULL;

    NBB_TRC_ENTRY("spm_l3_ipdelfrrid");

    OS_MEMSET(&ipfrrkey, 0, sizeof(IPFRR_KEY));
    ipfrrkey.vrfid = vrfid;
    ipfrrkey.mpeer = mpeer;
    ipfrrkey.bpeer = bpeer;

    ipfrrp =  (IPFRR *)AVLL_FIND( SHARED.ipfrr_tree ,  &ipfrrkey );
    if( ipfrrp == NULL )
    {
        rv = ADD_UCROUTE_FREE_IP_FRR_NULL;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ipfrrid vrf.m.b.rv=%d.%d.%d.%d\n", 
                                    __LINE__, vrfid,mpeer,bpeer,rv);
        goto  EXIT_LABEL;
    }
    else
    {
        if( ipfrrp->citenum == 1) 
        {
        #ifdef SPU
            rv = ApiC3DeleteFrrGroup( BCMC31, ipfrrp->frrid);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3DeleteFrrGroup( BCMC32, ipfrrp->frrid);
                rv = (rv|rv2);
            }
        #endif
        #ifdef SRC
            rv = 0;
        #endif  
        #ifdef SNU
            rv = 0;
        #endif  
            spm_lps_del_entity( FRR_TYPE, ipfrrp->frrid);
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o d ipfrrid vrf.m.b=%d.%d.%d\n", 
                                        __LINE__, vrfid,mpeer,bpeer);
            AVLL_DELETE(  SHARED.ipfrr_tree , ipfrrp->ipf_node);
            NBB_MM_FREE(ipfrrp, MEM_SPM_VRFUROUTE_IPFRR_CB);
            
        }
        else
        {
            ipfrrp->citenum--;
            rv = 0;
        }
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_getecmpid( ECMP_KEY *key,NBB_ULONG *nextp,
                                        NBB_BYTE num,NBB_BYTE pflag,NBB_USHORT *ecmpid NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    NBB_USHORT  index = 0;

    ECMP_KEY    ecmpkey;

    ECMPTAB     *ecmps = NULL;

    APS_L3_ECMP_ST  apsecmp;
    
    L3_ECMP_T l3Ecmp;

    NBB_TRC_ENTRY("spm_l3_getecmpid");

    if (key == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getecmpid key = NULL \n", __LINE__);
        rv = ADD_UCROUTE_ECMP_PARA_ERR;
        goto  EXIT_LABEL;
    }

    if (nextp == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getecmpid nextp = NULL \n", __LINE__);
        rv = ADD_UCROUTE_ECMP_PARA_ERR;
        goto  EXIT_LABEL;
    }
    
    if (ecmpid == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e getecmpid ecmpid = NULL \n", __LINE__);
        rv = ADD_UCROUTE_ECMP_PARA_ERR;
        goto  EXIT_LABEL;
    }

    OS_MEMSET(&ecmpkey, 0, sizeof(ECMP_KEY));
    NBB_MEMCPY( &ecmpkey, key, sizeof(ECMP_KEY) );
    ecmps =  (ECMPTAB *)AVLL_FIND( SHARED.ecmp_tree ,  &ecmpkey );
    if( ecmps == NULL )
    {
        #ifdef SPU
            rv = ApiC3ReqEcmpGroupId( BCMC31, &index );
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3ReqEcmpGroupId( BCMC32, &index );
                rv = (rv|rv2);
            }
            if( rv != 0)   
            {
                //OS_PRINTF("Line=%d error getecmp rv=%d\n",__LINE__, rv);
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e gecmpid dev rv=%d \n", __LINE__, rv);
                rv = ADD_UCROUTE_CREATE_DRV_ECMP_ERR;
                goto  EXIT_LABEL;
            }
            else
            {
                //OS_PRINTF("Line=%d ok getecmp id=%d\n",__LINE__, index);
                //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o gecmpid dev id=%d \n", __LINE__, index);
            }
        #endif
        #ifdef SRC
            rv = 0;
        #endif  
        #ifdef SNU
            rv = 0;
        #endif
        if( rv == 0)
        {
            ecmps = (ECMPTAB *)NBB_MM_ALLOC(sizeof(ECMPTAB), NBB_NORETRY_ACT, MEM_SPM_VRFUROUTE_ECMP_CB);
            if(NULL == ecmps)
            {
                OS_PRINTF("Line=%d ecmp malloc mem NULL \n",__LINE__);
                rv = ADD_UCROUTE_ECMP_MALLOC_NULL;
                goto  EXIT_LABEL;
            }
            NBB_MEMSET(ecmps, 0, sizeof(ECMPTAB));
            AVLL_INIT_NODE(ecmps->ep_node);  
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(ecmps->key), &ecmpkey, sizeof(ECMP_KEY));
            
            ecmps->num = 1;
            
            //coverity[no_effect_test]
            AVLL_INSERT( SHARED.ecmp_tree, ecmps->ep_node);

            ecmps->ecmpid = index;
            *ecmpid = index;

            ecmps->pflag = pflag;
            ecmps->ecmpnum = num;
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(ecmps->peerip[0]), nextp, 4 * 4);

            OS_MEMSET(&l3Ecmp, 0, sizeof(L3_ECMP_T));
            l3Ecmp.ecmpNum = ecmps->ecmpnum;
            if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 0 )
            {                  
                 l3Ecmp.alg = ECMP_HASH_DSIP;
            }
            else if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 1 )
            {
                 l3Ecmp.alg = ECMP_PORT_ROUND_ROBIN;
            }
            l3Ecmp.nhIdxGroup[0] = ecmpkey.nhi1;
            l3Ecmp.nhIdxGroup[1] = ecmpkey.nhi2;
            l3Ecmp.nhIdxGroup[2] = ecmpkey.nhi3;
            l3Ecmp.nhIdxGroup[3] = ecmpkey.nhi4;

            l3Ecmp.ecmpIdx = ecmps->ecmpid;

        #ifdef SPU
            rv = ApiC3SetL3Ecmp( BCMC31, &l3Ecmp);
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3SetL3Ecmp( BCMC32, &l3Ecmp);
                rv = (rv|rv2);
            }

            if( rv != 0)   
            {
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e a l3ecmp ecmp.rv=%d.%d \n", 
                                            __LINE__, ecmps->ecmpid,rv);
                rv = ADD_UCROUTE_SET_DRV_ECMP_ERR;
                goto  EXIT_LABEL;
            }
            else
            {
                //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o a l3ecmp ecmp.num=%d.%d \n", 
                //                          __LINE__, ecmps->ecmpid,ecmps->ecmpnum);
                
            }
        #endif              

            
            //添加保护
            if( pflag == 1)
            {
                OS_MEMSET(&apsecmp, 0, sizeof(APS_L3_ECMP_ST));                
                apsecmp.ecmpNum = ecmps->ecmpnum;                

                if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 0 )
                {
                    apsecmp.alg = ECMP_HASH_DSIP;
          
                }
                else if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 1 )
                {
                    apsecmp.alg = ECMP_PORT_ROUND_ROBIN;
                           
                }
                
                apsecmp.ecmpNextHop[0].nhIdxGroup = ecmpkey.nhi1;
                apsecmp.ecmpNextHop[0].ldpKey.vrf_id = 0;
                apsecmp.ecmpNextHop[0].ldpKey.fec_prefix_len = 32;
                apsecmp.ecmpNextHop[0].ldpKey.fec_prefix = ecmps->peerip[0];                

                apsecmp.ecmpNextHop[1].nhIdxGroup = ecmpkey.nhi2;
                apsecmp.ecmpNextHop[1].ldpKey.vrf_id = 0;
                apsecmp.ecmpNextHop[1].ldpKey.fec_prefix_len = 32;
                apsecmp.ecmpNextHop[1].ldpKey.fec_prefix = ecmps->peerip[1];                
                
                apsecmp.ecmpNextHop[2].nhIdxGroup = ecmpkey.nhi3;
                apsecmp.ecmpNextHop[2].ldpKey.vrf_id = 0;
                apsecmp.ecmpNextHop[2].ldpKey.fec_prefix_len = 32;
                apsecmp.ecmpNextHop[2].ldpKey.fec_prefix = ecmps->peerip[2];               

                apsecmp.ecmpNextHop[3].nhIdxGroup = ecmpkey.nhi4;
                apsecmp.ecmpNextHop[3].ldpKey.vrf_id = 0;
                apsecmp.ecmpNextHop[3].ldpKey.fec_prefix_len = 32;
                apsecmp.ecmpNextHop[3].ldpKey.fec_prefix = ecmps->peerip[3];                

                apsecmp.ecmpIdx = ecmps->ecmpid;                

                aps_add_l3_ecmp( &apsecmp );

            }
        }
    }
    else
    {
        *ecmpid = ecmps->ecmpid;
        ecmps->num++;
        
        //OS_PRINTF("Line=%d ok getecmp rid=%d\n",__LINE__, index);
    }
    rv = 0;
    
    EXIT_LABEL:
    
    NBB_TRC_EXIT();
    
    return rv;
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_delecmpid(ECMP_KEY *key NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    NBB_USHORT  index = 0;

    ECMP_KEY    ecmpkey;

    ECMPTAB     *ecmps = NULL;

    NBB_TRC_ENTRY("spm_l3_delecmpid");

    if (key == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delecmpid key = NULL \n", __LINE__);
        rv = ADD_UCROUTE_FREE_ECMP_PARA_ERR;
        goto  EXIT_LABEL;
    }
        
    OS_MEMSET(&ecmpkey, 0, sizeof(ECMP_KEY));
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &ecmpkey, key, sizeof(ECMP_KEY) );
    ecmps =  (ECMPTAB *)AVLL_FIND( SHARED.ecmp_tree ,  &ecmpkey );
    if( ecmps == NULL )
    {
        rv = ADD_UCROUTE_FREE_ECMP_NULL;
        
        //OS_PRINTF("Line=%d error ecmp NULL rv=%d\n",__LINE__,rv);
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e decmpid NULL rv=%d \n", __LINE__, rv);
        goto  EXIT_LABEL;
    }
    else
    {
        if( ecmps->num == 1) 
        {
        #ifdef SPU
            rv = ApiC3FreeEcmpGroupId( BCMC31, ecmps->ecmpid );
            if ( SHARED.c3_num == 2 )
            {
                rv2 = ApiC3FreeEcmpGroupId( BCMC32, ecmps->ecmpid );
                rv = (rv|rv2);
            }
        #endif
        #ifdef SRC
            rv = 0;
        #endif  
        #ifdef SNU
            rv = 0;
        #endif

            //删除保护
            if( ecmps->pflag == 1)
            {
                aps_del_l3_ecmp( ecmps->ecmpid );
            }

            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o decmpid id=%d \n", __LINE__, ecmps->ecmpid);
            AVLL_DELETE( SHARED.ecmp_tree, ecmps->ep_node);
            NBB_MM_FREE(ecmps, MEM_SPM_VRFUROUTE_ECMP_CB);
        }
        else
        {
            ecmps->num--;
            rv = 0;
        }
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_addpeerecmp(NBB_ULONG *nextp,NBB_ULONG *nhp,
                    NBB_ULONG peer,NBB_BYTE flag,NBB_BYTE num,NBB_USHORT ecid NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    PEEREP_KEY      peerkey;

    PEEREPTAB       *peerp = NULL;

    ECMPEN_KEY      ecmpenkey;

    ECMPNODE        *ecmpnode = NULL;

    NBB_TRC_ENTRY("spm_l3_addpeerecmp");

    if (nextp == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e addpeerecmp nextp = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }

    if (nhp == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e addpeerecmp nhp = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
    
    if (peer == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e addpeerecmp peer = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
    if (ecid == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e addpeerecmp mcid = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
    

    peerkey.peer = peer;
    peerp =  (PEEREPTAB *)AVLL_FIND( SHARED.peecmp_tree, &peerkey);
    if( peerp == NULL )
    {
        peerp = (PEEREPTAB *)NBB_MM_ALLOC(sizeof(PEEREPTAB), NBB_NORETRY_ACT, MEM_SPM_FTN_TAB_PEECMP_CB);
        if(NULL == peerp)
        {
            OS_PRINTF("Line=%d peerp malloc mem NULL \n",__LINE__);
            rv = 888;
            goto  EXIT_LABEL;
        }
        NBB_MEMSET(peerp, 0, sizeof(PEEREPTAB));
        AVLL_INIT_NODE(peerp->pp_node); 
        
        //coverity[bad_sizeof]
        NBB_MEMCPY( &(peerp->key), &peerkey, sizeof(PEEREP_KEY));

        AVLL_INIT_TREE(peerp->ecmpnode_tree,
                            spm_l3_ecmpnodecomp,
                            NBB_OFFSETOF(ECMPNODE, key),/*lint !e413 */
                            NBB_OFFSETOF(ECMPNODE, en_node));/*lint !e413 */

        //coverity[no_effect_test]
        AVLL_INSERT( SHARED.peecmp_tree, peerp->pp_node);
        peerp->num = 1;

        ecmpnode = (ECMPNODE *)NBB_MM_ALLOC(sizeof(ECMPNODE), NBB_NORETRY_ACT, MEM_SPM_FTN_TAB_ECMPNODE_CB);
        if(NULL == ecmpnode)
        {
            OS_PRINTF("Line=%d ecmpnode malloc mem NULL \n",__LINE__);
            rv = 888;
            goto  EXIT_LABEL;
        }
        NBB_MEMSET(ecmpnode, 0, sizeof(ECMPNODE));
        AVLL_INIT_NODE(ecmpnode->en_node);  

        OS_MEMSET(&ecmpenkey, 0, sizeof(ECMPEN_KEY));
        ecmpenkey.ecmpid = ecid;
        
        //coverity[bad_sizeof]
        NBB_MEMCPY( &(ecmpnode->key), &ecmpenkey, sizeof(ECMPEN_KEY));

        AVLL_INSERT( peerp->ecmpnode_tree, ecmpnode->en_node);

        ecmpnode->flag = flag;
        ecmpnode->num = num;
        
        //coverity[bad_sizeof]
        NBB_MEMCPY( &(ecmpnode->peerip[0]), nextp, 4 * 4);
        
        //coverity[bad_sizeof]
        NBB_MEMCPY( &(ecmpnode->nhi[0]), nhp, 4 * 4);
        ecmpnode->citenum = 1;
        
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o addpeerecmp peer.ecid =%x.%d \n", __LINE__, peer,ecid);
    }
    else
    {
        OS_MEMSET(&ecmpenkey, 0, sizeof(ECMPEN_KEY));
        ecmpenkey.ecmpid = ecid;
        ecmpnode =  (ECMPNODE *)AVLL_FIND( peerp->ecmpnode_tree, &ecmpenkey);
        if( ecmpnode == NULL )
        {
            ecmpnode = (ECMPNODE *)NBB_MM_ALLOC(sizeof(ECMPNODE), NBB_NORETRY_ACT, MEM_SPM_FTN_TAB_ECMPNODE_CB);
            if(NULL == ecmpnode)
            {
                OS_PRINTF("Line=%d ecmpnode malloc mem NULL \n",__LINE__);
                rv = 888;
                goto  EXIT_LABEL;
            }
            NBB_MEMSET(ecmpnode, 0, sizeof(ECMPNODE));
            AVLL_INIT_NODE(ecmpnode->en_node);  
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(ecmpnode->key), &ecmpenkey, sizeof(ECMPEN_KEY));

            AVLL_INSERT( peerp->ecmpnode_tree, ecmpnode->en_node);

            ecmpnode->flag = flag;
            ecmpnode->num = num;
            NBB_MEMCPY( &(ecmpnode->peerip[0]), nextp, 4 * 4);
            NBB_MEMCPY( &(ecmpnode->nhi[0]), nhp, 4 * 4);
            ecmpnode->citenum = 1;

            peerp->num++;
            
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o addpeerecmp peer.ecid =%x.%d \n", 
                                                                __LINE__, peer,ecid);
            
        }
        else
        {
            ecmpnode->citenum++;
            
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o addpeerecmp peer.ecid =%x.%d \n", 
            //                          __LINE__, peer,ecid);
        }

    }
    rv = 0;
    
    EXIT_LABEL:
    
    NBB_TRC_EXIT();
    
    return rv;
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_delpeerecmp(NBB_ULONG peer ,NBB_USHORT ecid NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    PEEREP_KEY      peerkey;

    PEEREPTAB       *peerp = NULL;

    ECMPEN_KEY      ecmpenkey;

    ECMPNODE        *ecmpnode = NULL;

    NBB_TRC_ENTRY("spm_l3_delpeerecmp");

    if (peer == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delpeerecmp peer = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
    if (ecid == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delpeerecmp ecid = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
    
    peerkey.peer = peer;
    peerp =  (PEEREPTAB *)AVLL_FIND( SHARED.peecmp_tree, &peerkey);
    if( peerp == NULL )
    {
        rv = 101;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delpeerecmp peer =%x \n", __LINE__, peer);
        goto  EXIT_LABEL;
    }
    else
    {
        OS_MEMSET(&ecmpenkey, 0, sizeof(ECMPEN_KEY));
        ecmpenkey.ecmpid = ecid;
        ecmpnode =  (ECMPNODE *)AVLL_FIND( peerp->ecmpnode_tree, &ecmpenkey);
        if( ecmpnode == NULL )
        {
            rv = 101;
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e delpeerecmp peer.ecid =%x.%d \n", __LINE__, peer,ecid);
            goto  EXIT_LABEL;
        }
        if( ecmpnode->citenum == 1) 
        {
            AVLL_DELETE( peerp->ecmpnode_tree, ecmpnode->en_node);
            NBB_MM_FREE(ecmpnode, MEM_SPM_FTN_TAB_ECMPNODE_CB);
            peerp->num--;
        }
        else
        {
            ecmpnode->citenum--;
        }
    }
    if( peerp->num == 0) 
    {
        AVLL_DELETE( SHARED.peecmp_tree, peerp->pp_node);
        NBB_MM_FREE(peerp, MEM_SPM_FTN_TAB_PEECMP_CB);
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_INT spm_l3_updatepeerecmp(NBB_ULONG peer ,NBB_BYTE flag NBB_CCXT_T NBB_CXT)
{
    NBB_INT     rv = 1;
    NBB_INT     rv2 = 1;

    PEEREP_KEY      peerkey;

    PEEREPTAB       *peerp = NULL;

    ECMPEN_KEY      ecmpenkey;

    ECMPNODE        *ecmpnode = NULL;

    L3_ECMP_T       l3ecmp;

    NBB_TRC_ENTRY("spm_l3_updatepeerecmp");

    if (peer == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e updatepeerecmp peer = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
    
    peerkey.peer = peer;
    peerp =  (PEEREPTAB *)AVLL_FIND( SHARED.peecmp_tree, &peerkey);
    if( peerp == NULL )
    {
        rv = 101;
        
        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e updatepeerecmp peer =%x \n", __LINE__, peer);
        goto  EXIT_LABEL;
    }
    else
    {
        #ifdef SPU

            ecmpnode = (ECMPNODE *)AVLL_FIRST( peerp->ecmpnode_tree);
            while (ecmpnode != NULL)
            {
                OS_MEMSET(&l3ecmp, 0, sizeof(L3_ECMP_T));
            
                if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 0 )
                {
                    l3ecmp.alg = ECMP_HASH_DSIP;
                }
                else if ( SHARED.global_cb.basic_cfg_cb.ecmp_algo == 1 )
                {
                    l3ecmp.alg = ECMP_PORT_ROUND_ROBIN;
                }
                if ( flag == 0 )
                {
                    l3ecmp.ecmpNum = ecmpnode->num - 1;
                    if (( ecmpnode->flag == 1 ) && (ecmpnode->peerip[0] == peer))
                    {
                        l3ecmp.nhIdxGroup[0] = ecmpnode->nhi[1];
                        l3ecmp.nhIdxGroup[1] = ecmpnode->nhi[2];
                        l3ecmp.nhIdxGroup[2] = ecmpnode->nhi[3];
                    }
                    else if (( ecmpnode->flag == 2 ) && (ecmpnode->peerip[1] == peer))
                    {
                        l3ecmp.nhIdxGroup[0] = ecmpnode->nhi[0];
                        l3ecmp.nhIdxGroup[1] = ecmpnode->nhi[2];
                        l3ecmp.nhIdxGroup[2] = ecmpnode->nhi[3];
                    }
                    else if (( ecmpnode->flag == 3 ) && (ecmpnode->peerip[2] == peer))
                    {
                        l3ecmp.nhIdxGroup[0] = ecmpnode->nhi[0];
                        l3ecmp.nhIdxGroup[1] = ecmpnode->nhi[1];
                        l3ecmp.nhIdxGroup[2] = ecmpnode->nhi[3];
                    }   
                    else if (( ecmpnode->flag == 4 ) && (ecmpnode->peerip[3] == peer))
                    {
                        l3ecmp.nhIdxGroup[0] = ecmpnode->nhi[0];
                        l3ecmp.nhIdxGroup[1] = ecmpnode->nhi[1];
                        l3ecmp.nhIdxGroup[2] = ecmpnode->nhi[2];
                    }
                    else 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e updatepeerecmp peer.ecid =%x.%d \n", 
                                                    __LINE__, peer,ecmpnode->key.ecmpid);
                        goto  EXIT_LABEL;
                    }
                }
                else if ( flag == 1 )
                {
                    l3ecmp.ecmpNum = ecmpnode->num;
                    l3ecmp.nhIdxGroup[0] = ecmpnode->nhi[0];
                    l3ecmp.nhIdxGroup[1] = ecmpnode->nhi[1];
                    l3ecmp.nhIdxGroup[2] = ecmpnode->nhi[2];
                    l3ecmp.nhIdxGroup[3] = ecmpnode->nhi[3];
                }
                l3ecmp.ecmpIdx = ecmpnode->key.ecmpid ;
                
                rv = ApiC3ModL3Ecmp( BCMC31, &l3ecmp);
                if ( SHARED.c3_num == 2 )
                {
                    rv2 = ApiC3ModL3Ecmp( BCMC32, &l3ecmp);
                    rv = (rv|rv2);
                }
                if( rv != 0)   
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e updateftnecmp pe.ecid.flag  =%x.%d.%d \n", 
                                                __LINE__, peer,ecmpnode->key.ecmpid,flag);
                    goto  EXIT_LABEL;
                }
                else
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o updateftnecmp pe.ecid.flag =%x.%d.%d \n", 
                                                __LINE__, peer,ecmpnode->key.ecmpid,flag);
                }
                ecmpnode = (ECMPNODE *)AVLL_NEXT( peerp->ecmpnode_tree, ecmpnode->en_node);
            }

        #endif
        #ifdef SRC
            rv = 0;
        #endif  
        #ifdef SNU
            rv = 0;
        #endif  
    }
    
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return rv;
}


/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
//NBB_VOID spm_l3_vrfurouteprocess(ATG_DCI_SET_VRF_UC *vrfuips NBB_CCXT_T NBB_CXT)
NBB_VOID spm_l3_vrfurouteprocess(ATG_DCI_VRFUC_ROUTE_ENTRY *vrfuips NBB_CCXT_T NBB_CXT)
{

    NBB_INT             rv = 100;
    NBB_INT             rv2 = 100;

    NBB_BYTE            *ucdata = NULL;

    NBB_BYTE            flag = 0;
    NBB_BYTE            ipv4hop = 0;
    NBB_BYTE            ipv6hop = 0;

    VRFUROUTE_KEY       vrfukey;

    VRFUROUTE_BASE      base;

    VRFUROUTE           *uroute = NULL;

    ATG_DCI_VRF_UC_IPV4_DATA    *nextipv4 = NULL;
    ATG_DCI_VRF_UC_IPV6_DATA    *nextipv6 = NULL;
    
    VRFUROUTE_NEXT      nexthopcfg[8];
    VRFUROUTE_NEXT      old_nexthopcfg[8];
    
    NBB_BYTE            i ;
    NBB_BYTE            frrtype = 0;
    NBB_BYTE            ecmpflag = 0;
    NBB_BYTE            pflag = 0;

    L3_ROUTE_T          l3route;

    SHASH_NODE          *hnode = NULL;

    NBB_ULONG           uninhi = 0;
    NBB_ULONG           ipv6index = 0;
    NBB_ULONG           nextindex = 0;
    NBB_ULONG           logport = 0;
    NBB_ULONG           mport = 0;
    NBB_ULONG           bport = 0;

    NBB_ULONG           mpeer = 0,bpeer = 0,mpwlabel = 0xffffffff,bpwlabel = 0xffffffff,
                        ompeer = 0,obpeer = 0,ompwlabel = 0xffffffff,obpwlabel = 0xffffffff;

    NBB_USHORT          frrid = 0;
    NBB_USHORT          ecmpid = 0;
    NBB_USHORT          bfdid = 0;
    NBB_USHORT          obfdid = 0;

    DC_UNI                  dchop;
    DC_UNI                  olddc[8];

    NBB_ULONG           oldnextid[8];
    NBB_ULONG            nextip[4];
    NBB_ULONG           nhi[4];

    ECMP_KEY            ecmpkey;

    SPM_LINEAR_PROTECT_SWITCH       stLps;

    FTN_KEY             ftnkey;

    FTN_TAB             *uftn = NULL;

    NBB_CHAR            *errorlog = NULL;
    
    NBB_ULONG           str_len = 0;
    

    NBB_TRC_ENTRY("spm_l3_vrfurouteprocess");
    
    NBB_ASSERT(vrfuips != NULL);    
        
    if(NULL == vrfuips)
    {
        goto EXIT_LABEL;
    }
        
    vrfuips->return_code = ATG_DCI_RC_OK;

    OS_MEMSET( &nexthopcfg[0], 0, 8 * sizeof( VRFUROUTE_NEXT ) );

    OS_MEMSET( &vrfukey, 0, sizeof( VRFUROUTE_KEY ) );
    
    //coverity[bad_sizeof]
    NBB_MEMCPY( &vrfukey, &(vrfuips->key), sizeof( VRFUROUTE_KEY ) );
    if( vrfukey.addrtype == 0 )
    {
        NBB_TRC_DETAIL((NBB_FORMAT "UCroute v4 vrfid=%d mask=%d dip = %x",
                                                    vrfukey.vrfid, vrfukey.mask ,vrfukey.dip[3]));
        if(( vrfukey.dip[3] == 0 ) || ( vrfukey.mask == 0 ))
        {
            if( vrfuips->delete_struct == 1)
            {
                NBB_PRINTF("Del route v4 dip.mask =%x.%d \n",vrfukey.dip[3],vrfukey.mask);
                
                //NBB_EXCEPTION((PCT_SPM| 10, 1,  "s s", " Del route0 ",errorlog));
            }
            else
            {
                NBB_PRINTF("Add route v4 dip.mask =%x.%d \n",vrfukey.dip[3],vrfukey.mask);
                
                //NBB_EXCEPTION((PCT_SPM| 10, 1,  "s s", " Add route0 ",errorlog));
            }
        }
    }
    else if( vrfukey.addrtype == 1 )
    {
        NBB_TRC_DETAIL((NBB_FORMAT "UCroute v6 vrfid=%d   mask=%d  dip = %x.%x.%x.%x",
                        vrfukey.vrfid, vrfukey.mask ,vrfukey.dip[0],vrfukey.dip[1],vrfukey.dip[2],vrfukey.dip[3]));
        if(( vrfukey.dip[3] == 0 ) && (vrfukey.dip[2] == 0 ) && ( vrfukey.dip[1] == 0 ) && ( vrfukey.dip[0] == 0 ))
        {
            NBB_PRINTF("UCroute v6 data dip =0.0.0.0 \n");
        }
    }
    
    //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, errorlog);
    //if( vrfukey.dip[3] == 0x10000100 )
    //{
        //NBB_EXCEPTION((PCT_SPM| 6, 1, "s", "FH-SPM routeprocess test  1 start!"));
    //}
    /*操作之前，查看本地数据是否存在*/
    //非直连路由
    uroute =  (VRFUROUTE *)AVLL_FIND( SHARED.vrfuroute_tree ,  &vrfukey ); 
    if( uroute == NULL )
    {
        uroute =  (VRFUROUTE *)AVLL_FIND( SHARED.dcroute_tree ,  &vrfukey ); 
    }
    if( uroute == NULL )
    {
        uroute =  (VRFUROUTE *)AVLL_FIND( SHARED.lbroute_tree ,  &vrfukey ); 
    }
    if( vrfuips->delete_struct == 1) /*  删除*/
    {
        if( SHARED.ipsroute.delipsnum == 0xffffffff)
        {
            SHARED.ipsroute.delipsnum = 0;
        }
        else
        {
            SHARED.ipsroute.delipsnum++;
        }
        if( uroute == NULL )
        {
            NBB_TRC_FLOW((NBB_FORMAT "Del UCroute Error: data null"));
            vrfuips->return_code = ATG_DCI_RC_OK;  
            rv = 0;
            goto  EXIT_LABEL;
        }
        #ifdef SPU
            if( uroute->flag == 1 )
            {
                rv = spm_l3_ucroute_del_drv_route(&vrfukey);
                if( rv != 0) 
                { 
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d vrf.m.ip.rv=%d.%d.%x.%x.%x.%x.%d\n ",
                                                                                            __LINE__, 
                                                                                            uroute->key.vrfid,
                                                                                            uroute->key.mask,
                                                                                            uroute->key.dip[0],
                                                                                            uroute->key.dip[1],
                                                                                            uroute->key.dip[2],
                                                                                            uroute->key.dip[3],rv);
                    rv = DEL_UCROUTE_DRV_ROUTE;
                    uroute->return_code = rv;
                }
                else
                {
                    uroute->flag = 0;
                }
            }
        #endif
        #ifdef SRC
            uroute->flag = 0;
        #endif
        #ifdef SNU
            uroute->flag = 0;
        #endif
            if( uroute->ecmpflag != 0 )
            {
                OS_MEMSET(&ecmpkey, 0, sizeof(ECMP_KEY));
                ecmpkey.nhi1 = uroute->nextindex[0];
                ecmpkey.nhi2 = uroute->nextindex[1];
                ecmpkey.nhi3 = uroute->nextindex[2];
                ecmpkey.nhi4 = uroute->nextindex[3];
                rv = spm_l3_delecmpid(&ecmpkey NBB_CCXT);
                if( rv == 0)   
                {
                    uroute->ecmpflag = 0;
                    
                    //for ( i = 0; i < uroute->ipv4num; i++)
                    //{
                    //  if (( uroute->key.vrfid != 0 )&&( uroute->nexthop[i].label != 0xffffffff ))
                    //  {
                    //      spm_l3_delpeerecmp( uroute->nexthop[i].nextip[3],uroute->frrecmpid NBB_CCXT);
                    //  }
                    //}
                    //for ( i = 0; i < uroute->ipv6num; i++)
                    //{
                    //  if ((uroute->nexthop[i].nexttype == 0 )&&( uroute->nexthop[i].label != 0xffffffff ))
                    //  {
                    //      spm_l3_delpeerecmp( uroute->nexthop[i].nextip[3],uroute->frrecmpid NBB_CCXT);
                    //  }
                    //}
                }
                else
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ecmpid rv=%d \n", __LINE__, rv);
                    uroute->return_code = rv;
                    //if( rv == 101)
                    //{
                    //    uroute->ecmpflag = 0;
                    //}
                }                   
            }
            for ( i = 0; i < uroute->ipv4num; i++)
            {
                if( uroute->nextindex[i] != 0 )
                {
                    if(( uroute->base.routetype != 2 ) && ( uroute->nexthop[i].label == 0xffffffff )) 
                    {
                        rv = spm_l3_deluni( 0, &(uroute->dchop[i].ntip[3]),uroute->dchop[i].nextport NBB_CCXT);
                        if( rv != 0) 
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d v4uni rv=%d \n", __LINE__, rv);
                            uroute->return_code = rv;
                        }
                        else
                        {
                            uroute->nextindex[i] = 0;
                        }
                    }
                    else if( uroute->base.routetype != 2 )
                    {
                        rv = spm_l3_delpwv4( vrfukey.vrfid, 
                                    uroute->nexthop[i].nextip[3],uroute->nexthop[i].label NBB_CCXT); 
                        if( rv != 0) 
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d v4nni rv=%d \n", __LINE__, rv);
                            uroute->return_code = rv;
                        }
                        else
                        {
                            uroute->nextindex[i] = 0;
                        }
                    }
                    if( uroute->nexthop[i].nexttypeb == 1 )
                    {
                        ompwlabel = uroute->nexthop[i].label;
                        ompeer = uroute->nexthop[i].nextip[3];
                        obfdid = uroute->nexthop[i].bfdid;
                    }
                    else if( uroute->nexthop[i].nexttypeb == 2 )
                    {
                        obpwlabel = uroute->nexthop[i].label;
                        obpeer = uroute->nexthop[i].nextip[3];
                    }
                }
            }
            for ( i = 0; i < uroute->ipv6num; i++)
            {
                if( uroute->nextindex[i] != 0 )
                {
                    if(( uroute->base.routetype != 2 ) && ( uroute->nexthop[i].label == 0xffffffff )) 
                    {
                        rv = spm_l3_deluni( 1, &(uroute->dchop[i].ntip[0]),uroute->dchop[i].nextport NBB_CCXT);
                        if( rv != 0) 
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d v6uni rv=%d \n", __LINE__, rv);
                            uroute->return_code = rv;
                        }
                        else
                        {
                            uroute->nextindex[i] = 0;
                        }
                    }
                    else if( uroute->base.routetype != 2 )
                    {
                        rv = spm_l3_delpwv4( vrfukey.vrfid, 
                                    uroute->nexthop[i].nextip[3],uroute->nexthop[i].label NBB_CCXT); 
                        if( rv != 0) 
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d v6nni rv=%d \n", __LINE__, rv);
                            uroute->return_code = rv;
                        }
                        else
                        {
                            uroute->nextindex[i] = 0;
                        }
                    }
                    if( uroute->nexthop[i].nexttypeb == 1 )
                    {
                        ompwlabel = uroute->nexthop[i].label;
                        ompeer = uroute->nexthop[i].nextip[3];
                        obfdid = uroute->nexthop[i].bfdid;
                    }
                    else if( uroute->nexthop[i].nexttypeb == 2 )
                    {
                        obpwlabel = uroute->nexthop[i].label;
                        obpeer = uroute->nexthop[i].nextip[3];
                    }
                }
            }
            if( uroute->frrtype != 0 ) 
            {
                //rv = ApiC3DeleteFailoverGroup( 0, uroute->frrgroupid);
                //spm_lps_del_entity(   FRR_TYPE, uroute->frrecmpid);
                
            }   
            if( uroute->frrtype != 0 )
            {
                if( uroute->frrtype == 1 )
                {
                    rv = spm_l3_freefrrid( ompeer, obpeer NBB_CCXT);
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ffrrid mpeer.b.r=%x.%x.%d \n", 
                                                    __LINE__,ompeer,obpeer, rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                    }
                }
                else if( uroute->frrtype == 2 )
                {
                    rv = spm_l3_bfddelfrrid(obfdid NBB_CCXT);
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d bfdid.rv=%d.%d \n", __LINE__,obfdid, rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                        
                        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o d bfdid=%d \n", __LINE__,obfdid);
                    }
                }
                else if( uroute->frrtype == 3 )
                {
                    rv = spm_l3_ipdelfrrid(vrfukey.vrfid,ompeer,obpeer NBB_CCXT);
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ipfrr m.p.rv=%d.%d.%d \n", 
                                                    __LINE__,ompeer,obpeer,rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                        
                        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o d bfdid=%d \n", __LINE__,obfdid);
                    }
                }
                else if( uroute->frrtype == 10 )
                {

                    rv = spm_l3_ucroute_del_drv_frrgroup(uroute->frrecmpid ); 
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d frrid.rv=%d.%d \n", 
                                                    __LINE__,uroute->frrecmpid, rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                    }   
                    
                }
            }
        rv = 0; 

        if( uroute->base.routetype == 1 )
        {
            AVLL_DELETE( SHARED.vrfuroute_tree, uroute->route_node);
        }
        else if( uroute->base.routetype == 0 )
        {
            AVLL_DELETE( SHARED.dcroute_tree, uroute->route_node);
        }
        else if( uroute->base.routetype == 2 )
        {
            AVLL_DELETE( SHARED.lbroute_tree, uroute->route_node);
        }
        spm_l3_memfree( uroute NBB_CCXT);
        if( SHARED.ipsroute.delipsok == 0xffffffff)
        {
            SHARED.ipsroute.delipsok = 0;
        }
        else
        {
            SHARED.ipsroute.delipsok++;
        }
    }
    else if( vrfuips->delete_struct == 0) /* 增加*/
    {   
        vrfuips->basic_return_code = ATG_DCI_RC_OK;
        if( SHARED.ipsroute.addipsnum == 0xffffffff)
        {
            SHARED.ipsroute.addipsnum = 0;
        }
        else
        {
            SHARED.ipsroute.addipsnum++;
        }
        if( vrfuips->oper_basic == ATG_DCI_OPER_ADD) /* 增加*/
        {
            if( uroute == NULL )
            {
                uroute = spm_l3_memmalloc(NBB_CXT);
                if ( uroute == NULL )
                {
                    rv = ADD_UCROUTE_MALLOC_NULL;
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute: malloc null ,rv=%d.\n",
                                                                               __LINE__,rv);
                    uroute->return_code = rv;
                    goto  EXIT_LABEL;
                } 
                flag = 1;
            }
            else
            {
                flag = 2;
            }
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(vrfuips, &vrfuips->basic_data); 
            if (ucdata == NULL)
            {
                rv = ADD_UCROUTE_IPS_BASIC_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute: basic ips null ,rv=%d.\n",
                                                                               __LINE__,rv);
                uroute->return_code = rv;
                //spm_l3_memfree( uroute NBB_CCXT);
                goto  EXIT_LABEL;
            }
            
            //此处不调用驱动，暂时只存储数据
            if( flag == 2)
            {
                OS_MEMSET( &base, 0, sizeof( VRFUROUTE_BASE ) );
                
                //coverity[bad_sizeof]
                NBB_MEMCPY( &base, ucdata, sizeof( VRFUROUTE_BASE ) );
                if( base.routetype != uroute->base.routetype)
                {
                    if( uroute->base.routetype == 1 )
                    {
                        AVLL_DELETE( SHARED.vrfuroute_tree, uroute->route_node);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o m rtype d udc ip=%x \n", __LINE__,vrfukey.dip[3]);
                    }
                    else if( uroute->base.routetype == 0 )
                    {
                        AVLL_DELETE( SHARED.dcroute_tree, uroute->route_node);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o m rtype d dc ip=%x \n", __LINE__,vrfukey.dip[3]);
                    }
                    else if( uroute->base.routetype == 2 )
                    {
                        AVLL_DELETE( SHARED.lbroute_tree, uroute->route_node);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o m rtype d lb ip=%x \n", __LINE__,vrfukey.dip[3]);
                    }
                    AVLL_INIT_NODE(uroute->route_node); 
                    if( base.routetype == 1 )
                    {
                        AVLL_INSERT( SHARED.vrfuroute_tree, uroute->route_node);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o m rtype a udc ip=%x \n", __LINE__,vrfukey.dip[3]);
                    }
                    else if( base.routetype == 0 )
                    {
                        //coverity[no_effect_test]
                        AVLL_INSERT( SHARED.dcroute_tree, uroute->route_node);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o m rtype a dc ip=%x \n", __LINE__,vrfukey.dip[3]);
                    }
                    else if( base.routetype == 2 )
                    {
                        AVLL_INSERT( SHARED.lbroute_tree, uroute->route_node);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o m rtype a lb ip=%x \n", __LINE__,vrfukey.dip[3]);
                    }
                }
            }
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(uroute->base), ucdata, sizeof( VRFUROUTE_BASE ) );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(uroute->key), &vrfukey, sizeof( VRFUROUTE_KEY) );
            
            if( uroute->base.routetype == 1 )
            {
                if( flag == 1)
                {
                    AVLL_INSERT(SHARED.vrfuroute_tree, uroute->route_node);
                }
                NBB_TRC_FLOW((NBB_FORMAT "Add UCroute UDC route "));
            }
            else if( uroute->base.routetype == 0 )
            {
                if( flag == 1)
                {
                    AVLL_INSERT(SHARED.dcroute_tree, uroute->route_node);
                }
                NBB_TRC_FLOW((NBB_FORMAT "Add UCroute DC route "));
            }
            else if( uroute->base.routetype == 2 )
            {
            #ifdef SPU
                if( flag == 1)
                {
                    AVLL_INSERT(SHARED.lbroute_tree, uroute->route_node);
                }  
                uroute->flag = 1;
                rv = spm_l3_ucroute_create_drv_hostroute(&vrfukey);
                if( rv != 0) 
                {
                    rv = ADD_UCROUTE_DRV_SET_UCROUTE_ERR;
                    //spm_l3_memfree( uroute NBB_CCXT);
                    uroute->return_code = rv;
                    goto  EXIT_LABEL;
                }
                else
                {
                    //暂时这样
                    if( SHARED.ipsroute.addipsok == 0xffffffff)
                    {
                        SHARED.ipsroute.addipsok = 0;
                    }
                    else
                    {
                        SHARED.ipsroute.addipsok++;
                    }
                    goto  EXIT_LABEL;
                }
            #endif
            #ifdef SRC
                rv = 0;
                if( flag == 1)
                {
                    AVLL_INSERT(SHARED.lbroute_tree, uroute->route_node);
                }
                NBB_TRC_FLOW((NBB_FORMAT "Add UCroute LB route "));
                if( SHARED.ipsroute.addipsok == 0xffffffff)
                {
                    SHARED.ipsroute.addipsok = 0;
                }
                else
                {
                    SHARED.ipsroute.addipsok++;
                }
                goto  EXIT_LABEL;
            #endif
            #ifdef SNU
                rv = 0;
                if( flag == 1)
                {
                    AVLL_INSERT(SHARED.lbroute_tree, uroute->route_node);
                }
                NBB_TRC_FLOW((NBB_FORMAT "Add UCroute LB route "));
                if( SHARED.ipsroute.addipsok == 0xffffffff)
                {
                    SHARED.ipsroute.addipsok = 0;
                }
                else
                {
                    SHARED.ipsroute.addipsok++;
                }
                goto  EXIT_LABEL;
            #endif
            }
            rv = 0;
        }   
        else if( vrfuips->oper_basic == ATG_DCI_OPER_DEL) /*  删除子TLV*/
        {
                NBB_TRC_FLOW((NBB_FORMAT "Del UCroute Error: basic data "));
                vrfuips->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;  
                goto  EXIT_LABEL;
        }
        if( vrfuips->oper_ipv4 == ATG_DCI_OPER_UPDATE) /* 增加*/
        {
            vrfuips->ipv4_return_code = ATG_DCI_RC_OK;
            if ( uroute == NULL )                                   
            {
                rv = ADD_UCROUTE_UCROUTE_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute hop null ,rv=%d.\n",
                                                                               __LINE__,rv);
                goto   EXIT_LABEL;
            }
            ipv4hop = vrfuips->ipv4_num;
            NBB_TRC_DETAIL((NBB_FORMAT "Add UCroute ipv4num =%d ", ipv4hop));
            if (ipv4hop > 4)
            {
                rv = ADD_UCROUTE_NEXTHOP_DATA_EXC;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute hop >4 ,rv=%d.\n",
                                                                               __LINE__,rv);
                uroute->return_code = rv;
                goto  EXIT_LABEL;
            }   
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(vrfuips, &vrfuips->ipv4_data);
            if (ucdata == NULL)
            {
                rv = ADD_UCROUTE_IPS_IPV4_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:ips ipv4 data null ,rv=%d.\n",
                                                                               __LINE__,rv);
                uroute->return_code = rv;
                goto  EXIT_LABEL;
            }
            OS_MEMSET( &(olddc[0]), 0, 8 * sizeof( DC_UNI ) );
            OS_MEMSET(&(old_nexthopcfg[0]),0,8 * sizeof(VRFUROUTE_NEXT));
            NBB_MEMCPY( &(olddc[0]), &(uroute->dchop[0]), 8 * sizeof( DC_UNI ) );           
            OS_MEMSET( &(oldnextid[0]), 0, 8 * 4 );
            NBB_MEMCPY( &(oldnextid[0]), &(uroute->nextindex[0]), 8 * 4 ); 

            /*取上次下一跳的配置*/
            NBB_MEMCPY( &(old_nexthopcfg[0]), &(uroute->nexthop[0]), 8 * sizeof(VRFUROUTE_NEXT)); 
            OS_MEMSET(  &(uroute->nexthop[0]), 0,  8 * sizeof(VRFUROUTE_NEXT) );
            
            //获取配置
            if ( uroute->base.routetype == 1 )
            {
                for ( i = 0; i < ipv4hop; i++)
                {
                    nextipv4 = (ATG_DCI_VRF_UC_IPV4_DATA *)(ucdata + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VRF_UC_IPV4_DATA))) * i);
                    OS_MEMSET( &(nexthopcfg[i]), 0, sizeof( VRFUROUTE_NEXT ) );
                    
                    //coverity[overrun-buffer-arg]
                    OS_MEMCPY( &(nexthopcfg[i].nextip[3]), nextipv4, 16 );

                    /*先保存下一跳配置*/
                    OS_MEMCPY( &(uroute->nexthop[i].nextip[3]), nextipv4, 16 );
                    if ( uroute->key.vrfid == 0 )
                    {
                        if( uroute->base.ecmpmode == 0 )
                        {
                            //rv = spm_l3_checkip( 0, 0, &(nexthopcfg[i].nextip[3]) NBB_CCXT);
                            rv = 0;
                        }
                        else if( uroute->base.ecmpmode == 1 )
                        {
                            rv = 0;
                        }
                        if ( rv == 0 )
                        {
                            uroute->dchop[i].ntip[3] = nexthopcfg[i].nextip[3];
                            uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                            rv = spm_l3_getuni( 0, &(uroute->dchop[i].ntip[3]),
                                                            uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                            if( rv == 0 )
                            {
                                uroute->nextindex[i] = uninhi;
                            }
                            else
                            {
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:get uni ,rv=%d.\n",
                                                                               __LINE__,rv);
                                uroute->return_code = rv;
                                uroute->ipv4num = i;
                                goto  EXIT_LABEL;
                            }
                        }
                        else if ( rv == 1 )
                        {
                            OS_MEMSET(&dchop, 0, sizeof( DC_UNI ));
                            
                            //rv = spm_l3_iteration_vrfuroute( 0, &(nexthopcfg[i].nextip[3]), &dchop NBB_CCXT);
                            //if ( rv == 0 )
                            if( nexthopcfg[i].label == 0xffffffff)
                            {
                                //uroute->dchop[i].ntip[3] = dchop.ntip[3];
                                //uroute->dchop[i].nextport = dchop.nextport;
                                uroute->dchop[i].ntip[3] = nexthopcfg[i].nextip[3];
                                uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                                rv = spm_l3_getuni( 0, &(uroute->dchop[i].ntip[3]),
                                                                uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                                if( rv == 0 )
                                {
                                    uroute->nextindex[i] = uninhi;
                                }
                                else
                                {
                                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:get uni ,rv=%d.\n",
                                                                               __LINE__,rv);
                                    uroute->return_code = rv;
                                    uroute->ipv4num = i;
                                    goto  EXIT_LABEL;
                                }
                            }
                            else
                            {
                                rv = ADD_UCROUTE_NEXTHOP_ITER_ERR;
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:iter route ,rv=%d.\n",
                                                                               __LINE__,rv);
                                uroute->return_code = rv;
                                uroute->ipv4num = i;
                                goto  EXIT_LABEL;
                            }
                        }
                    }
                    else
                    {
                        if( uroute->base.ecmpmode == 0 )
                        {
                            //rv = spm_l3_checkipdirectconnect( 0 ,&(nexthopcfg[i].nextip[3]));
                        }
                        else if( uroute->base.ecmpmode == 1 )
                        {
                            rv = 0;
                        }
                        
                        //if ( rv == 0 )
                        if( nexthopcfg[i].label == 0xffffffff)
                        {
                            uroute->dchop[i].ntip[3] = nexthopcfg[i].nextip[3];
                            uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                            rv = spm_l3_getuni( 0, &(uroute->dchop[i].ntip[3]),
                                                        uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                            if( rv == 0 )
                            {
                                uroute->nextindex[i] = uninhi;
                            }
                            else
                            {
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:get uni ,rv=%d.\n",
                                                                               __LINE__,rv);
                                uroute->return_code = rv;
                                uroute->ipv4num = i;
                                goto  EXIT_LABEL;
                            }
                        }
                        else 
                        {   
                            rv = spm_l3_getpwv4( uroute->key.vrfid, 
                                          nexthopcfg[i].nextip[3], nexthopcfg[i].label,&nextindex NBB_CCXT); 
                            if( rv == 0)
                            {
                                uroute->nextindex[i] = nextindex;
                            }
                            else
                            {
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:get pwv4 ,rv=%d.\n",
                                                                               __LINE__,rv);
                                uroute->return_code = rv;
                                uroute->ipv4num = i;
                                goto  EXIT_LABEL;
                            }
                        }
                    }
                    if( uroute->base.prmode == 3 )
                    {
                        if( nexthopcfg[i].nexttypeb == 1 )
                        {
                            mpwlabel = nexthopcfg[i].label;
                            mpeer = nexthopcfg[i].nextip[3];
                            logport = nexthopcfg[i].nextport;
                            if( mpwlabel == 0xffffffff )
                            {
                                mport = nexthopcfg[i].nextport;
                            }
                        }
                        else if( nexthopcfg[i].nexttypeb == 2 )
                        {
                            bpwlabel = nexthopcfg[i].label;
                            bpeer = nexthopcfg[i].nextip[3];
                            if( bpwlabel == 0xffffffff )
                            {
                                bport = nexthopcfg[i].nextport;
                            }
                        }
                    }
                }
            }
            else
            {
                for ( i = 0; i < ipv4hop; i++)
                {
                    nextipv4 = (ATG_DCI_VRF_UC_IPV4_DATA *)(ucdata + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VRF_UC_IPV4_DATA))) * i);
                    OS_MEMSET( &(nexthopcfg[i]), 0, sizeof( VRFUROUTE_NEXT ) );
                    
                    //coverity[overrun-buffer-arg]
                    OS_MEMCPY( &(nexthopcfg[i].nextip[3]), nextipv4, sizeof( ATG_DCI_VRF_UC_IPV4_DATA ) );

                    if( nexthopcfg[i].label == 0xffffffff )
                    {
                        //uroute->dchop[i].ntip[3] = nexthopcfg[i].nextip[3];
                        uroute->dchop[i].ntip[3] = 0;
                        uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                        rv = spm_l3_getuni( 0, &(uroute->dchop[i].ntip[3]),uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                        if( rv == 0 )
                        {
                            uroute->nextindex[i] = uninhi;
                        }
                        else
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:get uni ,rv=%d.\n",
                                                                               __LINE__,rv);
                            uroute->return_code = rv;
                            uroute->ipv4num = i;
                            goto  EXIT_LABEL;
                        }
                    }
                    else
                    {
                        rv = spm_l3_getpwv4( uroute->key.vrfid, nexthopcfg[i].nextip[3], 
                                                            nexthopcfg[i].label,&nextindex NBB_CCXT); 
                        if( rv == 0)
                        {
                            uroute->nextindex[i] = nextindex;
                        }
                        else
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:get pwv4 ,rv=%d.\n",
                                                                               __LINE__,rv);
                            uroute->return_code = rv;
                            uroute->ipv4num = i;
                            goto  EXIT_LABEL;
                        }
                    }
                    if( uroute->base.prmode == 3 )
                    {
                        if( nexthopcfg[i].nexttypeb == 1 )
                        {
                            mpwlabel = nexthopcfg[i].label;
                            mpeer = nexthopcfg[i].nextip[3];
                            logport = nexthopcfg[i].nextport;
                            bfdid = nexthopcfg[i].bfdid;
                            
                            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o NNIpUNI4 bfdid=%d\n",__LINE__,bfdid);
                        }
                        else if( nexthopcfg[i].nexttypeb == 2 )
                        {
                            bpwlabel = nexthopcfg[i].label;
                            bpeer = nexthopcfg[i].nextip[3];
                        }
                    }
                }
            }

            for ( i = 4; i > ipv4hop; i--)
            {
                uroute->nextindex[i - 1] = 0;
            }
            
            if( uroute->base.prmode == 3 )
            {       
                if(( bfdid != 0 ) && ( mpwlabel == 0xffffffff))
                {   
                    /*主控不能删除主用*/
                    rv = spm_l3_bfdgetfrrid( bfdid,&frrid ,logport NBB_CCXT);
                    if( rv == 0)   
                    {
                        frrtype = 2;
                        
                        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o NNIpUNI bfdid.ffr=%d.%d\n",
                        //                          __LINE__,bfdid,frrid);
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:bfdfrr ,rv=%d.\n",
                                                                               __LINE__,rv);
                        uroute->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else if(( uroute->key.vrfid != 0 )
                    && (( mpwlabel != 0xffffffff) || ( bpwlabel != 0xffffffff))
                    && (( mpeer != 0 ) || ( bpeer != 0 )))
                {
                    rv = spm_l3_mllocfrrid( mpeer, bpeer, logport, &frrid NBB_CCXT);
                    if( rv == 0)   
                    {
                        frrtype = 1;
                        
                        //OS_PRINTF("Line=%d mllocfrrid mpeer=%x bpeer=%x frr=%d\n",__LINE__, mpeer ,bpeer,frrid);
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:vpnfrr ,rv=%d.\n",
                                                                               __LINE__,rv);
                        //OS_PRINTF("Line=%d mlloc error mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
                        uroute->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else if((( mpwlabel == 0xffffffff) && ( bpwlabel == 0xffffffff)) && (( mport != 0 ) || ( bport != 0 )))
                {
                    //直连路由的FRR不在此类
                    rv = spm_l3_ipgetfrrid( uroute->key.vrfid,mpeer,mport,bpeer,bport,&frrid NBB_CCXT);
                    if( rv == 0)   
                    {
                        frrtype = 3;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o ipfrr m.p=%d.%d\n",__LINE__,mpeer,bpeer);
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:ipfrr ,rv=%d.\n",
                                                                               __LINE__,rv);
                        uroute->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else
                {
                    if( uroute->frrtype != 10 )
                    {
                        rv = spm_l3_ucroute_create_drv_frrgroup(&frrid);
                        if( rv == 0)   
                        {
                            frrtype = 10;
                            
                            //OS_PRINTF("Line=%d create frrid mpeer=%x bpeer=%x frr=%d\n",__LINE__, mpeer ,bpeer,frrid);
                        }
                        else
                        {
                            rv = ADD_UCROUTE_DRV_CREATE_FRR_ERR;
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:drv frr ,rv=%d.\n",
                                                                               __LINE__,rv);
                            //OS_PRINTF("Line=%d create error mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
                            uroute->return_code = rv;
                            goto  EXIT_LABEL;
                        }
                    }
                    else
                    {
                        frrid = uroute->frrecmpid;
                        frrtype = 10;
                        
                        //OS_PRINTF("Line=%d  V4 old frrid = %d\n",__LINE__, frrid);
                    }
                }   
            }
            if( uroute->base.prmode == 2 )
            {
                OS_MEMSET(&ecmpkey, 0, sizeof(ECMP_KEY));
                ecmpkey.nhi1 = uroute->nextindex[0];
                ecmpkey.nhi2 = uroute->nextindex[1];
                ecmpkey.nhi3 = uroute->nextindex[2];
                ecmpkey.nhi4 = uroute->nextindex[3];
                for ( i = 0; i < uroute->base.ecmpnum; i++)
                {
                        nextip[i] = nexthopcfg[i].nextip[3];
                }
                pflag = 0;
                if (( uroute->key.vrfid != 0 ) && ( nexthopcfg[0].label != 0xffffffff ))
                {
                    pflag = 1;
                }
                rv = spm_l3_getecmpid(&ecmpkey,&nextip[0],uroute->base.ecmpnum,pflag,&ecmpid NBB_CCXT);
                if( rv == 0)   
                {
                    ecmpflag = 1;
                    
                    //if (( uroute->key.vrfid != 0 )&&( nexthopcfg[0].label != 0xffffffff ))
                    //{
                    //  for ( i = 0; i < uroute->base.ecmpnum; i++)
                    //  {
                    //      nextip[i] = nexthopcfg[i].nextip[3];
                    //      nhi[i] = uroute->nextindex[i];
                    //  }
                    //  for ( i = 0; i < uroute->base.ecmpnum; i++)
                    //  {
                    //      spm_l3_addpeerecmp(&nextip[0], &nhi[0], nextip[i], i+1, 
                    //            uroute->base.ecmpnum, ecmpid NBB_CCXT);
                    //  }
                    //}
                }
                else
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:drv ecmp ,rv=%d.\n",
                                                                               __LINE__,rv);
                    uroute->return_code = rv;
                    goto  EXIT_LABEL;
                }   
            }
            for ( i = 0; i < uroute->ipv4num; i++)
            {
                if( old_nexthopcfg[i].label == 0xffffffff)
                {
                    rv = spm_l3_deluni( 0, &(olddc[i].ntip[3]), olddc[i].nextport NBB_CCXT);
                    if( rv != 0) 
                    {
                        //OS_PRINTF("Line=%d Del old nexthop: v4uni ip=%x rv=%d\n ",__LINE__, olddc[i].ntip[3],rv);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ov4uni ip=%x rv=%d \n", 
                                                    __LINE__,olddc[i].ntip[3], rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        //OS_PRINTF("Line=%d ok Del old nexthop: v4uni ip=%x\n ",__LINE__, olddc[i].ntip[3]);
                    }
                }
                else
                {
                    rv = spm_l3_delpwv4(uroute->key.vrfid, 
                                        old_nexthopcfg[i].nextip[3], old_nexthopcfg[i].label NBB_CCXT);
                    if( rv != 0) 
                    {
                        //OS_PRINTF("Line=%d error Del ohop:v4nni ip=%x rv=%d\n ",
                        //                      __LINE__,uroute->nexthop[i].nextip[3], rv);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ov4nni ip=%x rv=%d \n", 
                                                    __LINE__,old_nexthopcfg[i].nextip[3], rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        //OS_PRINTF("Line=%d ok Del old nexthop: v4nni ip=%x\n ",
                        //                      __LINE__, uroute->nexthop[i].nextip[3]);
                    }
                }
                if( old_nexthopcfg[i].nexttypeb == 1 )
                {
                    ompwlabel = old_nexthopcfg[i].label;
                    ompeer = old_nexthopcfg[i].nextip[3];
                    obfdid = old_nexthopcfg[i].bfdid;
                }
                else if( old_nexthopcfg[i].nexttypeb == 2 )
                {
                    obpwlabel = old_nexthopcfg[i].label;
                    obpeer = old_nexthopcfg[i].nextip[3];
                }
            }
            if( uroute->frrtype != 0 )
            {
                if( uroute->frrtype == 1 )
                {
                    rv = spm_l3_freefrrid( ompeer, obpeer NBB_CCXT);
                    if( rv != 0) 
                    {
                        //OS_PRINTF("Line=%d error free frrid mpeer=%x obpeer=%x\n",__LINE__, ompeer ,obpeer);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ofrrid  mpeer.b=%x.%x rv=%d \n", 
                                                    __LINE__,ompeer ,obpeer, rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                        
                        //OS_PRINTF("Line=%d ok free frrid mpeer=%x obpeer=%x\n",__LINE__, ompeer ,obpeer);
                    }
                }
                else if( uroute->frrtype == 2 )
                {
                    rv = spm_l3_bfddelfrrid(obfdid NBB_CCXT);
                    if( rv != 0) 
                    {
                        //OS_PRINTF("Line=%d error free frrid mpeer=%x obpeer=%x\n",__LINE__, ompeer ,obpeer);
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d NNIpUNI  bfdid.rv=%d.%d \n", 
                                                    __LINE__,obfdid, rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                        
                        //OS_PRINTF("Line=%d ok free frrid mpeer=%x obpeer=%x\n",__LINE__, ompeer ,obpeer);
                        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o d NNIpUNI  bfdid=%d \n", __LINE__,obfdid);
                    }
                }
                else if( uroute->frrtype == 3 )
                {
                    rv = spm_l3_ipdelfrrid(uroute->key.vrfid,ompeer,obpeer NBB_CCXT);
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ipfrr m.p.rv=%d.%d.%d \n", 
                                                    __LINE__,mpeer,bpeer,rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o d ipfrr m.p=%d.%d \n", __LINE__,mpeer,bpeer);
                    }
                }
                else if( uroute->frrtype == 10 )
                {
                
                    if( uroute->frrtype != frrtype )
                    {
                        rv = spm_l3_ucroute_del_drv_frrgroup(uroute->frrecmpid);
                        if( rv != 0) 
                        {
                            rv = ADD_UCROUTE_FREE_DRV_FRR_ERR;
                            uroute->return_code = rv;
                        }
                        else
                        {
                            uroute->frrtype = 0;
                            uroute->frrecmpid = 0;
                        }   
                    }
                }
            }
            if( uroute->ecmpflag != 0 )
            {
                OS_MEMSET(&ecmpkey, 0, sizeof(ECMP_KEY));
                ecmpkey.nhi1 = oldnextid[0];
                ecmpkey.nhi2 = oldnextid[1];
                ecmpkey.nhi3 = oldnextid[2];
                ecmpkey.nhi4 = oldnextid[3];
                rv = spm_l3_delecmpid(&ecmpkey NBB_CCXT);
                if( rv == 0)   
                {
                    uroute->ecmpflag = 0;
                    
                    //for ( i = 0; i < uroute->ipv4num; i++)
                    //{
                    //  if (( uroute->key.vrfid != 0 )&&( uroute->nexthop[i].label != 0xffffffff ))
                    //  {
                    //      spm_l3_delpeerecmp( uroute->nexthop[i].nextip[3],uroute->frrecmpid NBB_CCXT);
                    //  }
                    //}
                }
                else
                {
                    //OS_PRINTF("Line=%d error ov4del ecmpid rv=%d\n",__LINE__,rv);
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d oecmpid id=%d rv=%d \n", 
                                                __LINE__,uroute->frrecmpid, rv);
                    uroute->return_code = rv;
                }                   
            }
            uroute->ipv4num = ipv4hop;
            rv = 0;
            if( rv == 0)/* 迭代成功*/
            {
            #ifdef SPU
                if( uroute->key.vrfid == 0)
                {
                    OS_MEMSET( &ftnkey, 0, sizeof( FTN_KEY ) );
                    ftnkey.vrfid = uroute->key.vrfid;
                    ftnkey.mask = uroute->key.mask;
                    ftnkey.fec = uroute->key.dip[3];
                    uftn =  (FTN_TAB *)AVLL_FIND( SHARED.ftn_tree ,  &ftnkey ); 
                    if( uftn != NULL)
                    {
                        if( uroute->flag == 1)
                        {
                            goto  LABELp1;
                        }
                        else
                        {
                            rv = spm_l3_ucroute_create_drv_route_fec(uroute,uftn);
                            if( rv == 0)   
                            {
                                //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o fib fec=%x \n", 
                                //                          __LINE__, uroute->key.dip[3]);
                                goto  LABELp1;
                            }
                            else
                            {
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e fib fec.rv =%x.%d \n", 
                                                            __LINE__, uroute->key.dip[3],rv);
                                rv = ADD_UCROUTE_DRV_SET_UCROUTE_ERR;
                                uroute->return_code = rv;
                            }
                        }
                    }
                }
                
                if( uroute->base.prmode == 3 )
                {
                    uroute->frrtype = frrtype;
                    uroute->frrecmpid = frrid;
                }
                else  if( uroute->base.prmode == 2 )
                {
                    uroute->frrecmpid = ecmpid;
                    uroute->ecmpflag = ecmpflag;
                }
                uroute->flag = 1;
                
                /*创建驱动路由表*/
                rv = spm_l3_ucroute_create_drv_route(uroute);  
                if(rv != 0)
                {
                    rv = ADD_UCROUTE_DRV_SET_UCROUTE_ERR;
                    uroute->return_code = rv;
                    goto  EXIT_LABEL;

                }
                else
                {
                    if( SHARED.ipsroute.addipsok == 0xffffffff)
                    {
                        SHARED.ipsroute.addipsok = 0;
                    }
                    else
                    {
                        SHARED.ipsroute.addipsok++;
                    }
                }
                //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e a v4 vrf.m.ip.rv=%d.%d.%x.%d\n ",
                //                                                                      __LINE__, 
                //                                                                      uroute->key.vrfid,
                //                                                                      uroute->key.mask,
                //                                                                          uroute->key.dip[3],rv);
           
        LABELp1:  
                rv = 0;
            #endif
            #ifdef SRC
                rv = 0;
            #endif
            #ifdef SNU
                rv = 0;
            #endif
            }
            
        }
        if( vrfuips->oper_ipv6 == ATG_DCI_OPER_UPDATE) 
        {
            vrfuips->ipv6_return_code = ATG_DCI_RC_OK;
            if ( uroute == NULL )                                   
            {
                 rv = ADD_UCROUTE_UCROUTE_DATA_NULL;
                 BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute v6hop null ,rv=%d.\n",
                                                                               __LINE__,rv);
                goto   EXIT_LABEL;
            }
            ipv6hop = vrfuips->ipv6_num;
            NBB_TRC_DETAIL((NBB_FORMAT "Add UCroute ipv6num =%d ", ipv6hop ));
            if ( ipv6hop > 4)
            {
                rv = ADD_UCROUTE_V6NEXTHOP_DATA_EXC;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute v6hop >4 ,rv=%d.\n",
                                                                               __LINE__,rv);
                uroute->return_code = rv;
                goto  EXIT_LABEL;
            }   
            ucdata = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(vrfuips, &vrfuips->ipv6_data);
            if (ucdata == NULL)
            {
                if (ipv6hop == 0)
                {
                    rv = 0;
                    goto  EXIT_LABEL;
                }

                rv = ADD_UCROUTE_IPS_IPV6_DATA_NULL;
                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute v6hop null ,rv=%d.\n",
                                                                               __LINE__,rv);
                uroute->return_code = rv;
                goto  EXIT_LABEL;
            }

            OS_MEMSET( &(olddc[0]), 0, 8 * sizeof( DC_UNI ) );
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(olddc[0]), &(uroute->dchop[0]), 8 * sizeof( DC_UNI ) );

            OS_MEMSET( &(oldnextid[0]), 0, 8 * 4 );
            OS_MEMSET(&(old_nexthopcfg[0]),0,8 * sizeof(VRFUROUTE_NEXT));
            
            //coverity[bad_sizeof]
            NBB_MEMCPY( &(oldnextid[0]), &(uroute->nextindex[0]), 8 * 4 ); 

            /*取上次下发的配置*/
            NBB_MEMCPY( &(old_nexthopcfg[0]), &(uroute->nexthop[0]), 8 * sizeof(VRFUROUTE_NEXT));
            OS_MEMSET( &(uroute->nexthop[0]), 0, 8 * sizeof(VRFUROUTE_NEXT));
            
            //获取配置
            if ( uroute->base.routetype == 1 )
            {
                for ( i = 0; i < ipv6hop; i++)
                {
                    nextipv6 = (ATG_DCI_VRF_UC_IPV6_DATA *)(ucdata + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VRF_UC_IPV6_DATA))) * i);
                    OS_MEMSET( &(nexthopcfg[i]), 0, sizeof(ATG_DCI_VRF_UC_IPV6_DATA));
                    NBB_MEMCPY( &(nexthopcfg[i]), nextipv6, sizeof(ATG_DCI_VRF_UC_IPV6_DATA));

                    /*先保存下一跳配置*/
                    NBB_MEMCPY( &(uroute->nexthop[i]), &(nexthopcfg[i]), sizeof(VRFUROUTE_NEXT));
                    
                    if (( nexthopcfg[i].nexttype == 1 ) && ( uroute->key.vrfid == 0 ))
                    {
                        if(( nexthopcfg[i].nextip[0] == 0)
                            && ( nexthopcfg[i].nextip[1] == 0)
                            && ( nexthopcfg[i].nextip[2] == 0x0000ffff)
                            && ( nexthopcfg[i].label != 0xffffffff))
                        {
                            nexthopcfg[i].nexttype = 0;
                            nexthopcfg[i].nextip[2] = 0;
                        }
                    }
                    if ( nexthopcfg[i].nexttype == 1 )
                    {
                        if ( uroute->key.vrfid == 0 )
                        {
                            //rv = spm_l3_checkipdirectconnect( 1 ,&(nexthopcfg[i].nextip[0]));
                            rv = spm_l3_checkip( 0, 1 ,&(nexthopcfg[i].nextip[0]) NBB_CCXT);
                            if ( rv == 0 )
                            {
                                //rv = spm_l3_getipv6index( nexthopcfg[i].nextport ,
                                //              &(nexthopcfg[i].nextip[0]), &ipv6index);
                                if ( rv == 0 )
                                {
                                    //uroute->dchop[i].nextip = ipv6index;
                                    NBB_MEMCPY( &(uroute->dchop[i].ntip[0]), &(nexthopcfg[i].nextip[0]), 16 );
                                    uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                                    rv = spm_l3_getuni( 1, &(uroute->dchop[i].ntip[0]),
                                                                    uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                                    if( rv == 0 )
                                    {
                                        uroute->nextindex[i] = uninhi;
                                    }
                                    else
                                    {
                                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute v6uni ,rv=%d.\n",
                                                                               __LINE__,rv); 
                                        uroute->return_code = rv;
                                        uroute->ipv6num = i;
                                        goto  EXIT_LABEL;
                                    }
                                }
                                else
                                {
                                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute v6 index ,rv=%d.\n",
                                                                                                                __LINE__,rv);
                                    uroute->return_code = rv;
                                    uroute->ipv6num = i;
                                    goto  EXIT_LABEL;
                                }
                            }
                            else if ( rv == 1 )
                            {
                                OS_MEMSET(&dchop, 0, sizeof( DC_UNI ));
                                
                                //rv = spm_l3_iteration_vrfuroute( 1, &(nexthopcfg[i].nextip[0]), &dchop NBB_CCXT);
                                //if ( rv == 0 )
                                if( nexthopcfg[i].label == 0xffffffff)
                                {
                                    //NBB_MEMCPY( &(uroute->dchop[i].ntip[0]), &(dchop.ntip[0]), 16 );
                                    //uroute->dchop[i].nextport = dchop.nextport;
                                    NBB_MEMCPY( &(uroute->dchop[i].ntip[0]), &(nexthopcfg[i].nextip[0]), 16 );
                                    uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                                    rv = spm_l3_getuni( 1, &(uroute->dchop[i].ntip[0]),
                                                                    uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                                    if( rv == 0 )
                                    {
                                        uroute->nextindex[i] = uninhi;
                                    }
                                    else
                                    {
                                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute v6uni ,rv=%d.\n",
                                                                               __LINE__,rv);
                                        uroute->return_code = rv;
                                        uroute->ipv6num = i;
                                        goto  EXIT_LABEL;
                                    }
                                }
                                else
                                {
                                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:iter uroute  ,rv=%d.\n",
                                                                               __LINE__,rv);
                                    uroute->return_code = rv;;
                                    uroute->ipv6num = i;
                                    goto  EXIT_LABEL;
                                }
                            }
                        }
                        else
                        {
                            //rv = spm_l3_checkipdirectconnect( 1 ,&(nexthopcfg[i].nextip[0]));
                            //rv = spm_l3_checkip( uroute->key.vrfid, 1 ,&(nexthopcfg[i].nextip[0]) NBB_CCXT);
                            //if ( rv == 0 )
                            if( nexthopcfg[i].label == 0xffffffff)
                            {
                                //uroute->dchop[i].nextip = ipv6index;
                                //coverity[bad_sizeof]
                                NBB_MEMCPY( &(uroute->dchop[i].ntip[0]), &(nexthopcfg[i].nextip[0]), 16 );
                                uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                                rv = spm_l3_getuni( 1, &(uroute->dchop[i].ntip[0]),
                                                    uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                                if( rv == 0 )
                                {
                                    uroute->nextindex[i] = uninhi;
                                }
                                else
                                {
                                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:uroute v6uni ,rv=%d.\n",
                                                                                  __LINE__,rv);
                                    uroute->return_code = rv;
                                    uroute->ipv6num = i;
                                    goto  EXIT_LABEL;
                                }
                            }
                            else 
                            {                               
                                BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:mpls  ,rv=%d.\n",
                                                                               __LINE__,rv);
                                uroute->return_code = rv;
                                uroute->ipv6num = i;
                                goto  EXIT_LABEL;       
                            }
                        }
                    }
                    else if ( nexthopcfg[i].nexttype == 0 )
                    {
                        if ( uroute->key.vrfid == 0 )
                        {
                            //rv = spm_l3_checkipdirectconnect( 0 ,&(nexthopcfg[i].nextip[3]));
                            rv = spm_l3_checkip( 0, 0 ,&(nexthopcfg[i].nextip[3]) NBB_CCXT);
                            if ( rv == 0 )
                            {
                                uroute->dchop[i].ntip[3] = nexthopcfg[i].nextip[3];
                                uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                                rv = spm_l3_getuni( 0, &(uroute->dchop[i].ntip[3]),
                                                    uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                                if( rv == 0 )
                                {
                                    uroute->nextindex[i] = uninhi;
                                }
                                else
                                {
                                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 uni  ,rv=%d.\n",
                                                                               __LINE__,rv);
                                    uroute->return_code = rv;
                                    uroute->ipv6num = i;
                                    goto  EXIT_LABEL;
                                }
                            }
                            else if ( rv == 1 )
                            {
                                if ( nexthopcfg[i].label == 0xffffffff )
                                {
                                    OS_MEMSET(&dchop, 0, sizeof( DC_UNI ));
                                    
                                    //rv = spm_l3_iteration_vrfuroute( 0, &(nexthopcfg[i].nextip[3]), &dchop NBB_CCXT);
                                    rv = 0;
                                    if ( rv == 0 )
                                    {
                                        //uroute->dchop[i].ntip[3] = dchop.ntip[3];
                                        //uroute->dchop[i].nextport = dchop.nextport;
                                        uroute->dchop[i].ntip[3] = nexthopcfg[i].nextip[3];
                                        uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                                        rv = spm_l3_getuni( 0, &(uroute->dchop[i].ntip[3]),
                                                                        uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                                        if( rv == 0 )
                                        {
                                            uroute->nextindex[i] = uninhi;
                                        }
                                        else
                                        {
                                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 uni  ,rv=%d.\n",
                                                                               __LINE__,rv);
                                            uroute->return_code = rv;
                                            uroute->ipv6num = i;
                                            goto  EXIT_LABEL;
                                        }
                                    }
                                    else
                                    {
                                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 iter  ,rv=%d.\n",
                                                                               __LINE__,rv);
                                        uroute->return_code = rv; 
                                        uroute->ipv6num = i;
                                        goto  EXIT_LABEL;
                                    }
                                }
                                else
                                {
                                    rv = spm_l3_getpwv4( uroute->key.vrfid, 
                                           nexthopcfg[i].nextip[3], nexthopcfg[i].label,&nextindex NBB_CCXT); 
                                    if( rv == 0)
                                    {
                                        uroute->nextindex[i] = nextindex;
                                    }
                                    else
                                    {
                                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 pw  ,rv=%d.\n",
                                                                               __LINE__,rv);
                                        uroute->return_code = rv;
                                   
                                        uroute->ipv6num = i;
                                        goto  EXIT_LABEL;
                                    }
                                }
                            }
                        }
                        else
                        {
                            //rv = spm_l3_checkipdirectconnect( 0 ,&(nexthopcfg[i].nextip[3]));
                            //if ( rv == 0 )
                            if( nexthopcfg[i].label == 0xffffffff)
                            {
                                uroute->dchop[i].ntip[3] = nexthopcfg[i].nextip[3];
                                uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                                
                                //NBB_TRC_DETAIL((NBB_FORMAT "ipv4 DC nextip=%x ", uroute->dchop[i].nextip));
                                //NBB_TRC_DETAIL((NBB_FORMAT "ipv4 DC nextport=%x ", uroute->dchop[i].nextport));
                                rv = spm_l3_getuni( 0, &(uroute->dchop[i].ntip[3]),
                                                    uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                                if( rv == 0 )
                                {
                                    uroute->nextindex[i] = uninhi;
                                }
                                else
                                {
                                   BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 uni  ,rv=%d.\n",
                                                                               __LINE__,rv);
                                    uroute->return_code = rv;
                                    uroute->ipv6num = i;
                                    goto  EXIT_LABEL;
                                }
                            }
                            else 
                            {   
                                rv = spm_l3_getpwv4( uroute->key.vrfid, 
                                                    nexthopcfg[i].nextip[3], nexthopcfg[i].label,&nextindex NBB_CCXT); 
                                if( rv == 0)
                                {
                                    uroute->nextindex[i] = nextindex;
                                }
                                else
                                {
                                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 pw  ,rv=%d.\n",
                                                                               __LINE__,rv);
                                    uroute->return_code = rv;
                                    uroute->ipv6num = i;
                                    goto  EXIT_LABEL;
                                }
                            }
                        }
                        if( uroute->base.prmode == 3 )
                        {
                            if( nexthopcfg[i].nexttypeb == 1 )
                            {
                                mpwlabel = nexthopcfg[i].label;
                                mpeer = nexthopcfg[i].nextip[3];
                                logport = nexthopcfg[i].nextport;
                                if( mpwlabel == 0xffffffff )
                                {
                                    mport = nexthopcfg[i].nextport;
                                }
                            }
                            else if( nexthopcfg[i].nexttypeb == 2 )
                            {
                                bpwlabel = nexthopcfg[i].label;
                                bpeer = nexthopcfg[i].nextip[3];
                                if( bpwlabel == 0xffffffff )
                                {
                                    bport = nexthopcfg[i].nextport;
                                }
                            }
                        }
                    }
                }
            }
            else
            {
                for ( i = 0; i < ipv6hop; i++)
                {
                    nextipv6 = (ATG_DCI_VRF_UC_IPV6_DATA *)(ucdata + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_VRF_UC_IPV6_DATA))) * i);
                    OS_MEMSET( &(nexthopcfg[i]), 0, sizeof( ATG_DCI_VRF_UC_IPV6_DATA ) );
                    
                    //coverity[bad_sizeof]
                    NBB_MEMCPY( &(nexthopcfg[i]), nextipv6, sizeof( ATG_DCI_VRF_UC_IPV6_DATA ) );

                    /*先保存下一跳配置*/
                    NBB_MEMCPY( &(uroute->nexthop[i]), &(nexthopcfg[i]), sizeof(VRFUROUTE_NEXT));

                    if( nexthopcfg[i].label == 0xffffffff )
                    {
                        //NBB_MEMCPY( &(uroute->dchop[i].ntip[0]), &(nexthopcfg[i].nextip[0]), 16 );
                        uroute->dchop[i].ntip[0] = 0;
                        uroute->dchop[i].ntip[1] = 0;
                        uroute->dchop[i].ntip[2] = 0;
                        uroute->dchop[i].ntip[3] = 0;
                        uroute->dchop[i].nextport = nexthopcfg[i].nextport;
                        rv = spm_l3_getuni( 1, &(uroute->dchop[i].ntip[0]),uroute->dchop[i].nextport, &uninhi NBB_CCXT);
                        if( rv == 0 )
                        {
                            uroute->nextindex[i] = uninhi;
                        }
                        else
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 uni  ,rv=%d.\n",
                                                                               __LINE__,rv);
                            uroute->return_code = rv;
                            uroute->ipv6num = i;
                            goto  EXIT_LABEL;
                        }
                    }
                    else
                    {
                        rv = spm_l3_getpwv4( uroute->key.vrfid, 
                                                nexthopcfg[i].nextip[3], nexthopcfg[i].label,&nextindex NBB_CCXT); 
                        if( rv == 0)
                        {
                            uroute->nextindex[i] = nextindex;
                        }
                        else
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 pw  ,rv=%d.\n",
                                                                               __LINE__,rv);
                            uroute->return_code = rv;
                            uroute->ipv6num = i;
                            goto  EXIT_LABEL;
                        }
                    }
                    if( uroute->base.prmode == 3 )
                    {
                        if( nexthopcfg[i].nexttypeb == 1 )
                        {
                            mpwlabel = nexthopcfg[i].label;
                            mpeer = nexthopcfg[i].nextip[3];
                            logport = nexthopcfg[i].nextport;
                            bfdid = nexthopcfg[i].bfdid;
                            
                            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o NNIpUNI6 bfdid=%d\n",__LINE__,bfdid);
                        }
                        else if( nexthopcfg[i].nexttypeb == 2 )
                        {
                            bpwlabel = nexthopcfg[i].label;
                            bpeer = nexthopcfg[i].nextip[3];
                        }
                    }
                }
            }
            
            for ( i = 4; i > ipv6hop; i--)
            {
                uroute->nextindex[i - 1] = 0;
            }
            
            if( uroute->base.prmode == 3 )
            {       
                if(( bfdid != 0 ) && ( mpwlabel == 0xffffffff))
                {  
                    /*主控不能删除主用*/
                    rv = spm_l3_bfdgetfrrid( bfdid,&frrid,logport NBB_CCXT);
                    if( rv == 0)   
                    {
                        frrtype = 2;
                        
                        //OS_PRINTF("Line=%d mllocfrrid mpeer=%x bpeer=%x frr=%d\n",__LINE__, mpeer ,bpeer,frrid);
                        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o NNIpUNI6 bfdid.ffr=%d.%d\n",
                        //                                                  __LINE__,bfdid,frrid);
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 bfdfrr  ,rv=%d.\n",
                                                                               __LINE__,rv);
                        OS_PRINTF("Line=%d Error NNIpUNI bfdid.rv=%d.%d\n",__LINE__,bfdid ,rv);
                        uroute->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else if(( uroute->key.vrfid != 0 )
                    && (( mpwlabel != 0xffffffff) || ( bpwlabel != 0xffffffff))
                    && (( mpeer != 0 ) || ( bpeer != 0 )))
                {
                    rv = spm_l3_mllocfrrid( mpeer, bpeer, logport, &frrid NBB_CCXT);
                    if( rv == 0)   
                    {
                        frrtype = 1;
                        
                        //OS_PRINTF("Line=%d mllocfrrid mpeer=%x bpeer=%x frr=%d\n",__LINE__, mpeer ,bpeer,frrid);
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:vpn frr  ,rv=%d.\n",
                                                                               __LINE__,rv);
                        OS_PRINTF("Line=%d mlloc error mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
                        uroute->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else if((( mpwlabel == 0xffffffff) && ( bpwlabel == 0xffffffff)) && (( mport != 0 ) || ( bport != 0 )))
                {
                    //直连路由的FRR不在此类
                    rv = spm_l3_ipgetfrrid( uroute->key.vrfid,mpeer,mport,bpeer,bport,&frrid NBB_CCXT);
                    if( rv == 0)   
                    {
                        frrtype = 3;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o ipfrr m.p=%d.%d\n",__LINE__,mpeer,bpeer);
                    }
                    else
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 ipfrr  ,rv=%d.\n",
                                                                               __LINE__,rv);
                        uroute->return_code = rv;
                        goto  EXIT_LABEL;
                    }
                }
                else 
                {
                    if( uroute->frrtype != 10 )
                    {
                        rv = spm_l3_ucroute_create_drv_frrgroup(&frrid);
                        if( rv == 0)   
                        {
                            frrtype = 0;
                            
                            //OS_PRINTF("Line=%d create frrid mpeer=%x bpeer=%x frr=%d\n",__LINE__, mpeer ,bpeer,frrid);
                        }
                        else
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 drv frr  ,rv=%d.\n",
                                                                               __LINE__,rv);
                            OS_PRINTF("Line=%d create error mpeer=%x bpeer=%x rv=%d\n",__LINE__, mpeer ,bpeer ,rv);
                            uroute->return_code = rv;
                            goto  EXIT_LABEL;
                        }
                    }
                    else
                    {
                        frrid = uroute->frrecmpid;
                        frrtype = 10;
                        
                        //OS_PRINTF("Line=%d  V4 old frrid = %d\n",__LINE__, frrid);
                        NBB_TRC_DETAIL((NBB_FORMAT "Add UCroute v6 old frrid = %d ", frrid));
                    }
                }   
            }
            if( uroute->base.prmode == 2 )
            {
                OS_MEMSET(&ecmpkey, 0, sizeof(ECMP_KEY));
                ecmpkey.nhi1 = uroute->nextindex[0];
                ecmpkey.nhi2 = uroute->nextindex[1];
                ecmpkey.nhi3 = uroute->nextindex[2];
                ecmpkey.nhi4 = uroute->nextindex[3];
                for ( i = 0; i < uroute->base.ecmpnum; i++)
                {
                    nextip[i] = nexthopcfg[i].nextip[3];
                }
                pflag = 0;
                if (( nexthopcfg[0].nexttype == 0 ) && ( nexthopcfg[0].label != 0xffffffff ))
                {
                    pflag = 1;
                }
                rv = spm_l3_getecmpid(&ecmpkey,&nextip[0],uroute->base.ecmpnum,pflag,&ecmpid NBB_CCXT);
                if( rv == 0)   
                {
                    ecmpflag = 1;
                    
                    //if (( nexthopcfg[0].nexttype == 0 )&&( nexthopcfg[0].label != 0xffffffff ))
                    //{
                    //  for ( i = 0; i < uroute->base.ecmpnum; i++)
                    //  {
                    //      nextip[i] = nexthopcfg[i].nextip[3];
                    //      nhi[i] = uroute->nextindex[i];
                    //  }
                    //  for ( i = 0; i < uroute->base.ecmpnum; i++)
                    //  {
                    //      spm_l3_addpeerecmp(&nextip[0], &nhi[0], nextip[i],i+1, 
                    //                  uroute->base.ecmpnum, ecmpid NBB_CCXT);
                    //  }
                    //}
                    
                }
                else
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:v6 ecmp  ,rv=%d.\n",
                                                                               __LINE__,rv);
                    OS_PRINTF("Line=%d error v6getecmpid rv=%d\n",__LINE__,rv);
                    uroute->return_code = rv;
                    goto  EXIT_LABEL;
                }   
            }
            for ( i = 0; i < uroute->ipv6num; i++)
            {
                if( old_nexthopcfg[i].label == 0xffffffff)
                {
                    if( old_nexthopcfg[i].nexttype == 1)
                    {
                        rv = spm_l3_deluni( 1, &(olddc[i].ntip[0]), olddc[i].nextport NBB_CCXT);
                        if( rv != 0) 
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d v6uni ip.rv=%x.%x.%x.%x.%d\n ",__LINE__, 
                                                                                            olddc[i].ntip[0],
                                                                                            olddc[i].ntip[1],
                                                                                            olddc[i].ntip[2],
                                                                                                olddc[i].ntip[3],rv);
                            uroute->return_code = rv;
                        }
                    }
                    else if( old_nexthopcfg[i].nexttype == 0)
                    {
                        rv = spm_l3_deluni( 0, &(olddc[i].ntip[3]),olddc[i].nextport NBB_CCXT);
                        if( rv != 0) 
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d v6uni ip.rv=%d\n ",
                                                        __LINE__, olddc[i].ntip[3],rv);
                            uroute->return_code = rv;
                        }
                    }
                }
                else
                {
                    rv = spm_l3_delpwv4(uroute->key.vrfid, 
                                        old_nexthopcfg[i].nextip[3], old_nexthopcfg[i].label NBB_CCXT);
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d v6nni ip.rv=%d\n ",
                                                __LINE__, old_nexthopcfg[i].nextip[3],rv);
                        uroute->return_code = rv;
                    }
                }
                if( old_nexthopcfg[i].nexttype == 0 )
                {
                    if( old_nexthopcfg[i].nexttypeb == 1 )
                    {
                        ompwlabel = old_nexthopcfg[i].label;
                        ompeer = old_nexthopcfg[i].nextip[3];
                        obfdid = old_nexthopcfg[i].bfdid;
                    }
                    else if( old_nexthopcfg[i].nexttypeb == 2 )
                    {
                        obpwlabel = old_nexthopcfg[i].label;
                        obpeer = old_nexthopcfg[i].nextip[3];
                    }
                }
            }
            if( uroute->frrtype != 0 )
            {
                if( uroute->frrtype == 1 )
                {
                    rv = spm_l3_freefrrid( ompeer, obpeer NBB_CCXT);
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d frrid  mpeer.b.rv=%d.%d.%d \n", 
                                                    __LINE__,ompeer,obpeer, rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;

                    }
                }
                else if( uroute->frrtype == 2 )
                {
                    rv = spm_l3_bfddelfrrid(obfdid NBB_CCXT);
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d NNIpUNI6  bfdid.rv=%d.%d \n", 
                                                    __LINE__,obfdid, rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                        
                        //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o d NNIpUNI6  bfdid=%d \n", __LINE__,obfdid);
                    }
                }
                else if( uroute->frrtype == 3 )
                {
                    rv = spm_l3_ipdelfrrid(uroute->key.vrfid,ompeer,obpeer NBB_CCXT);
                    if( rv != 0) 
                    {
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d ipfrr m.p.rv=%d.%d.%d \n", 
                                                    __LINE__,mpeer,bpeer,rv);
                        uroute->return_code = rv;
                    }
                    else
                    {
                        uroute->frrtype = 0;
                        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d o d ipfrr m.p=%d.%d \n", __LINE__,mpeer,bpeer);
                    }
                }
                else if( uroute->frrtype == 10 )
                {
                
                    if( uroute->frrtype != frrtype )
                    {
                        rv =  spm_l3_ucroute_del_drv_frrgroup(uroute->frrecmpid);
                        if( rv != 0) 
                        {
                            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d frrid  id.rv=%d.%d \n", 
                                                        __LINE__,uroute->frrecmpid, rv);
                            uroute->return_code = rv;
                        }
                        else
                        {
                            uroute->frrtype = 0;
                            uroute->frrecmpid = 0;
                        }   
                    }
                }
            }
            if( uroute->ecmpflag != 0 )
            {
                OS_MEMSET(&ecmpkey, 0, sizeof(ECMP_KEY));
                ecmpkey.nhi1 = oldnextid[0];
                ecmpkey.nhi2 = oldnextid[1];
                ecmpkey.nhi3 = oldnextid[2];
                ecmpkey.nhi4 = oldnextid[3];
                rv = spm_l3_delecmpid(&ecmpkey NBB_CCXT);
                if( rv == 0)   
                {
                    uroute->ecmpflag = 0;
                    
                    //for ( i = 0; i < uroute->ipv6num; i++)
                    //{
                    //  if ((uroute->nexthop[i].nexttype == 0 )&&( uroute->nexthop[i].label != 0xffffffff ))
                    //  {
                    //      spm_l3_delpeerecmp( uroute->nexthop[i].nextip[3],uroute->frrecmpid NBB_CCXT);
                    //  }
                    //}
                }
                else
                {
                    BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d v6ecmpid rv=%d \n", __LINE__, rv);
                    uroute->return_code = rv;
                }                   
            }
            uroute->ipv6num = ipv6hop;
           
           
        #ifdef SPU
            rv = 0;
            if( uroute->base.prmode == 3 )
            {
                uroute->frrtype = frrtype;
                uroute->frrecmpid = frrid;
            }
            else  if( uroute->base.prmode == 2 )
            {
                uroute->frrecmpid = ecmpid;
                uroute->ecmpflag = ecmpflag;
            }
            uroute->flag = 1;
            //BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e a v6 vrf.m.ip.rv=%d.%d.%x.%x.%x.%x.%d\n ",
            //                                                                      __LINE__, 
            //                                                                      uroute->key.vrfid,
            //                                                                      uroute->key.mask,
            //                                                                      uroute->key.dip[0],
            //                                                                      uroute->key.dip[1],
            //                                                                      uroute->key.dip[2],
            //                                                                          uroute->key.dip[3],rv);

            /*驱动适配接口，创建路由表*/
            rv = spm_l3_ucroute_create_drv_ipv6route(uroute);
            if(rv != 0)
            {
                rv = ADD_UCROUTE_DRV_SET_UCROUTE_ERR;
                uroute->return_code = rv;
                goto  EXIT_LABEL;
            }
            else
            {
                if( SHARED.ipsroute.addipsok == 0xffffffff)
                {
                     SHARED.ipsroute.addipsok = 0;
                }
                else
               {
                    SHARED.ipsroute.addipsok++;
               }
            }

        
        #endif
        #ifdef SRC
            rv = 0;
        #endif
        #ifdef SNU
            rv = 0;
        #endif
        
            
        }
    }
    EXIT_LABEL:

    NBB_TRC_EXIT();

    /*程序异常时，记录错误条目的key值*/
    if(rv != 0)
    {
        if(vrfukey.addrtype == 0)
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d ucroute:err vrfid=%d.mask=%d.dip=%x\n", 
                                           __LINE__,vrfukey.vrfid,vrfukey.mask,vrfukey.dip[3]);
        }
        else if(vrfukey.addrtype == 1)
        {
            BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d ucroute: err vrfid=%d.mask=%d.dip0.1.2.3=%x.%x.%x.%x\n", 
            __LINE__,vrfukey.vrfid,vrfukey.mask,vrfukey.dip[0],vrfukey.dip[1],vrfukey.dip[2],vrfukey.dip[3]);
        }
    }
     
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_bfdfrrcomp
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_vrfurouteupdate(ATG_DCI_VRFUC_ROUTE_UPDATE* vrfuc_update_ips NBB_CCXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    NBB_ULONG i = 0;
    NBB_BYTE *vrfuc_entry_start = NULL;
    ATG_DCI_VRFUC_ROUTE_ENTRY   *vrfuc_entry = NULL;

    NBB_TRC_ENTRY("spm_l3_vrfurouteupdate");

    NBB_ASSERT(NULL != vrfuc_update_ips);

    //获取单播路由entry地址
    vrfuc_entry_start = NTL_OFFLEN_GET_POINTER(vrfuc_update_ips, &(vrfuc_update_ips->vrfuc_data));
    if (vrfuc_entry_start == NULL)
    {
        NBB_TRC_FLOW((NBB_FORMAT "Failed to get vrfuc_entry_start"));
        goto EXIT_LABEL;
    }

    //逐条进行处理。
    for (i = 0; i < vrfuc_update_ips->vrfuc_num; i++)
    {
        vrfuc_entry = (ATG_DCI_VRFUC_ROUTE_ENTRY *)vrfuc_entry_start;
        spm_l3_vrfurouteprocess(vrfuc_entry NBB_CCXT);
        vrfuc_entry_start += vrfuc_entry->total_length;
    }

EXIT_LABEL:
    NBB_TRC_EXIT();
    return;
}


#endif

