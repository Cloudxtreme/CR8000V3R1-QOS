#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*************************************************************************

* 函数名   ： spm_l3_ucroute_del_drv_route
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除单播路由驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ucroute_del_drv_route(VRFUROUTE_KEY   *vrfukey NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    L3_ROUTE_T          l3route;

    if (vrfukey == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ucroute_del_drv_route vrfukey = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    
#ifdef SPU  

    ApiC3InitL3RouteStruct( &l3route );
    l3route.vrfId = vrfukey->vrfid;
    if( vrfukey->addrtype == 0 )
    {
        l3route.mIpPktType = IPV4_PKT_UNICAST;
        NBB_MEMCPY( &(l3route.mDstIpv4), &(vrfukey->dip[3]), 4 );
        l3route.mDstIpv4Mask = vrfukey->mask;
    }
    else if( vrfukey->addrtype == 1 )
    {
        l3route.mIpPktType = IPV6_PKT_UNICAST;
        NBB_MEMCPY( &(l3route.mDstIpv6), &(vrfukey->dip[0]), 16 );
        l3route.mDstIpv6Mask = vrfukey->mask;
    }
    
    rv = ApiC3DelL3UCRoute( BCMC31, &l3route);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3DelL3UCRoute( BCMC32, &l3route);
        rv = (rv|rv2);
    }
    if( rv != 0) 
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e d vrf.m.ip.rv=%d.%d.%x.%x.%x.%x.%d\n ",
                                                                                __LINE__, 
                                                                                vrfukey->vrfid,
                                                                                vrfukey->mask,
                                                                                vrfukey->dip[0],
                                                                                vrfukey->dip[1],
                                                                                vrfukey->dip[2],
                                                                                vrfukey->dip[3],rv);
        rv = DEL_UCROUTE_DRV_ROUTE;
    }
    

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ucroute_create_drv_hostroute
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建单播主机路由驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ucroute_create_drv_hostroute(VRFUROUTE_KEY   *vrfukey NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    L3_ROUTE_T          l3route;

    if (vrfukey == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ucroute_del_drv_route vrfukey = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    
#ifdef SPU  

    ApiC3InitL3RouteStruct( &l3route );
                
    l3route.vrfId = vrfukey->vrfid;
    if( vrfukey->addrtype == 0 )
    {
        l3route.mIpPktType = IPV4_PKT_UNICAST;
        NBB_MEMCPY( &(l3route.mDstIpv4), &(vrfukey->dip[3]), 4 );
        l3route.mDstIpv4Mask = vrfukey->mask;
    }
    else if( vrfukey->addrtype == 1 )
    {
        l3route.mIpPktType = IPV6_PKT_UNICAST;
        NBB_MEMCPY( &(l3route.mDstIpv6), &(vrfukey->dip[0]), 16 );
        l3route.mDstIpv6Mask = vrfukey->mask;
    }
    l3route.eRouteType = ROUTE_TYPE_LOCAL;
    l3route.eVpnProt = VPN_PROTECT_NONE;
    
    //rv = 0;
    rv = ApiC3SetL3UCRoute( BCMC31, &l3route);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3SetL3UCRoute( BCMC32, &l3route);
        rv = (rv|rv2);
    }
    if( rv != 0) 
    {
        rv = ADD_UCROUTE_DRV_SET_UCROUTE_ERR;
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:drv hostip ,rv=%d.\n",
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

* 函数名   ： spm_l3_ucroute_create_drv_frrgroup
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建frrfroup驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ucroute_create_drv_frrgroup(NBB_USHORT *frrid NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_USHORT  drv_frrid = 0;

    if (frrid == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ucroute_create_drv_frrgroup frrid = NULL \n", __LINE__);
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
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e Add ucroute:drv frr,rv=%d\n",
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

* 函数名   ： spm_l3_ucroute_del_drv_frrgroup
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除frrfroup驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ucroute_del_drv_frrgroup(NBB_USHORT frrid NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (frrid == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ucroute_del_drv_frrgroup frrid = NULL \n", __LINE__);
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
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e ucroute_del_drv_frrgroup frrid=%d rv=%d \n", 
                                   __LINE__,frrid, rv);
    }

#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ucroute_create_drv_route_fec
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建单播路由驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ucroute_create_drv_route_fec(VRFUROUTE   *uroute,FTN_TAB *uftn NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    L3_ROUTE_T          l3route;

    if (uroute == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e create_drv_route_fec uroute = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 

    if (uftn == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e create_drv_route_fec uftn = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
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
    else  if ( uroute->base.routetype == 0 )
    {
        l3route.eRouteType = ROUTE_TYPE_DIRECT;
    }
    else  if ( uroute->base.routetype == 2 )
    {
        l3route.eRouteType = ROUTE_TYPE_LOCAL;
    }
    l3route.eVpnProt = VPN_PROTECT_NONE;
    l3route.nhIdx = uftn->pwnhi;
    rv = ApiC3SetL3UCRoute(BCMC31, &l3route);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3SetL3UCRoute(BCMC32, &l3route);
        rv = (rv|rv2);
    }
    
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_ucroute_create_drv_route
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建单播路由驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ucroute_create_drv_route(VRFUROUTE   *uroute NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    L3_ROUTE_T          l3route;

    if (uroute == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e create_drv_route_fec uroute = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    #ifdef SPU  

    ApiC3InitL3RouteStruct( &l3route );
    l3route.vrfId = uroute->key.vrfid;
    l3route.mIpPktType = IPV4_PKT_UNICAST;
    NBB_MEMCPY( &(l3route.mDstIpv4), &(uroute->key.dip[3]), 4 );
    if( uroute->key.dip[3] == 0 )
    {
        l3route.mDstIpv4 = 0x1;
    }
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
        if ( uroute->base.prmode == 0 )
        {
            l3route.eVpnProt = VPN_PROTECT_NONE;
            l3route.nhIdx = uroute->nextindex[0];
            if ( uroute->nexthop[0].label != 0xffffffff )
            {
             
            }
            else
            {
;
            }
        }
        else if ( uroute->base.prmode == 3 )
        {
            l3route.eVpnProt = VPN_PROTECT_FRR;
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
            }
            if( l3route.nhIdx_p == 0)
            {
                l3route.nhIdx_p = l3route.nhIdx;
            }
            l3route.frrGroupId = uroute->frrecmpid;
            
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
            l3route.tL3Ecmp.ecmpIdx = uroute->frrecmpid;//uroute->frrecmpid;
            for ( i = 0; i < uroute->base.ecmpnum; i++)
            {
                l3route.tL3Ecmp.nhIdxGroup[i] = uroute->nextindex[i];
                if ( uroute->nexthop[i].label != 0xffffffff )
                {
                    
                }
                else
                {
                  
                }
            }
        }
    }

    rv = ApiC3SetL3UCRoute(BCMC31, &l3route);
    if ( SHARED.c3_num == 2 )
    {
        //l3route.tL3Ecmp.ecmpIdx = 0;
        rv2 = ApiC3SetL3UCRoute(BCMC32, &l3route);
        rv = (rv|rv2);
    }
    
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
    

}

/*************************************************************************

* 函数名   ： spm_l3_ucroute_create_drv_ipv6route
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 创建单播路由驱动
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_ucroute_create_drv_ipv6route(VRFUROUTE   *uroute NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    NBB_INT  i = 0;
    L3_ROUTE_T          l3route;

    if (uroute == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e create_drv_route_fec uroute = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    #ifdef SPU  

    ApiC3InitL3RouteStruct( &l3route );
                
    l3route.vrfId = uroute->key.vrfid;
    l3route.mIpPktType = IPV6_PKT_UNICAST;
    NBB_MEMCPY( &(l3route.mDstIpv6), &(uroute->key.dip[0]), 16 );
    l3route.mDstIpv6Mask = uroute->key.mask;
    
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
        if ( uroute->base.prmode == 0 )
        {
            l3route.eVpnProt = VPN_PROTECT_NONE;
            l3route.nhIdx = uroute->nextindex[0];
            if ( uroute->nexthop[0].label != 0xffffffff )
            {
              
            }
            else
            {
                
            }
        }
        else if ( uroute->base.prmode == 3 )
        {
            l3route.eVpnProt = VPN_PROTECT_FRR;
            for ( i = 0; i <  uroute->ipv6num; i++)
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
            }
            if( l3route.nhIdx_p == 0)
            {
                l3route.nhIdx_p = l3route.nhIdx;
            }
            l3route.frrGroupId = uroute->frrecmpid;
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
            l3route.tL3Ecmp.ecmpIdx = uroute->frrecmpid;//uroute->frrecmpid;
            for ( i = 0; i < uroute->base.ecmpnum; i++)
            {
                l3route.tL3Ecmp.nhIdxGroup[i] = uroute->nextindex[i];
                if ( uroute->nexthop[i].label != 0xffffffff )
                {
                   
                }
                else
                {
                   
                }
            }
        }
    }
    rv = ApiC3SetL3UCRoute( BCMC31, &l3route);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3SetL3UCRoute( BCMC32, &l3route);
        rv = (rv|rv2);
    }
    
#else
    rv = 0;
#endif
   EXIT_LABEL:
   return (rv);
    

}

