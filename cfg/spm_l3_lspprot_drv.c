/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_l3_lspprot.c
  版 本 号   : v0
  作    者   : wjhe
  生成日期   : 2016年2月29日
  最近修改   :
  功能描述   : LSPPROT配置处理适配程序
  函数列表   :
  修改历史   :

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*************************************************************************

* 函数名   ： spm_l3_lspprot_get_drv_frrstatus
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 获取驱动保护组的状态
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_lspprot_get_drv_frrstatus(NBB_USHORT frrid,NBB_BYTE *workstate NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;

    if (frrid == 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e lspprot_get_drv_frrstatus frrid = 0 \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    if (workstate == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e lspprot_get_drv_frrstatus workstate = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 
    #ifdef SPU
    rv = fhdrv_psn_l3_get_frr_status(BCMC31, frrid, workstate); 
    if ( SHARED.c3_num == 2 )
    {
        rv2 = fhdrv_psn_l3_get_frr_status(BCMC32, frrid, workstate);
        rv = (rv|rv2);
    }

    if(rv != 0)
    {
       BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e lspprot_get_drv_frrstatus drv.%d \n", __LINE__,rv);
    }
    #endif
    EXIT_LABEL:

    return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_lspprot_get_drv_frrstatus
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 获取驱动保护组的状态
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_lspprot_create_drv_tunnel(LSPPROT *lspprot NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    TUNNEL_T        tunnel;
    if (lspprot == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e lspprot_create_drv_tunnel lspprot = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
#ifdef SPU
    OS_MEMSET(&tunnel , 0, sizeof(TUNNEL_T));
    
    if( lspprot->base.prtype == 0)
    {                    
        tunnel.eProt = C3_PROT_FRR;
    }                         
	tunnel.txLspPosId = lspprot->hwposid[0];
	tunnel.backupTxLspPosId = lspprot->hwposid[1];
	
	if (lspprot->index == 0)
	{
		 rv = ApiC3CreateTunnel( BCMC31 ,&tunnel);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3CreateTunnel( BCMC32 ,&tunnel);
            rv = (rv|rv2);
        }                    
	}
    else
    {
        tunnel.tunnelId = lspprot->index;
        rv = ApiC3ModTunnel( BCMC31 ,&tunnel);
        if ( SHARED.c3_num == 2 )
        {
            rv2 = ApiC3ModTunnel( BCMC32 ,&tunnel); 
            rv = (rv|rv2);
        }
    } 
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e  create_drv_tunnel ,rv=%d.\n",
                                                                               __LINE__,rv);
    }
    else
    {
        lspprot->index = tunnel.tunnelId;
        lspprot->frrid = tunnel.frrGroupId;
    }
#endif
#ifdef SRC
    tunnel.tunnelId = 0xffff;
#endif
#ifdef SNU
    tunnel.tunnelId = 0xffff;
#endif
 EXIT_LABEL:
 return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_lspprot_create_tunnelfrr_aps
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 将保护信息传递给aps模块
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_lspprot_create_tunnelfrr_aps(LSPPROT *lspprot NBB_CCXT_T NBB_CXT)
{
     NBB_INT  rv = 1;
     NBB_INT  i = 0;
     SPM_LINEAR_PROTECT_SWITCH       stLps;
     if (lspprot == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e lspprot_create_drv_tunnel lspprot = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
    if( lspprot->base.prtype == 0)
    {
        OS_MEMSET(&stLps, 0, sizeof(SPM_LINEAR_PROTECT_SWITCH));
        stLps.revertiveType = lspprot->base.returntype;
        stLps.holdOffTime = lspprot->base.delaytime;
        stLps.wtrTime = lspprot->base.waittime;

        stLps.reverseFlag = lspprot->reverseflag;
        stLps.hwSwFlag = lspprot->hwstate;

        stLps.workEntity.port = 0;
        stLps.workEntity.slot = 0;
        stLps.workEntity.tpId = 0;
        stLps.workEntity.type = 3;
        stLps.workEntity.rsvpKey.ingress = lspprot->key.ingress;
        stLps.workEntity.rsvpKey.egress = lspprot->key.egress;
        stLps.workEntity.rsvpKey.tunnel_id = lspprot->key.tunnelid;
      
        stLps.protEntity.port = 0;
        stLps.protEntity.slot = 0;
        stLps.protEntity.tpId = 0;
        stLps.protEntity.type = 3;
        stLps.protEntity.rsvpKey.ingress = lspprot->key.ingress;
        stLps.protEntity.rsvpKey.egress = lspprot->key.egress;
        stLps.protEntity.rsvpKey.tunnel_id = lspprot->key.tunnelid;
        for(i = 0; i<lspprot->lspnum;i++)
    	{
    		if((lspprot->lspb.lsp[i].state) == 0)
    		{
    		  stLps.workEntity.rsvpKey.lsp_id = lspprot->lspb.lsp[i].lspid;
    		  stLps.workEntity.portIndex = lspprot->dclsp[i].nextport;
    		}
    		else if((lspprot->lspb.lsp[i].state) == 1)
    		{
    		  stLps.protEntity.rsvpKey.lsp_id = lspprot->lspb.lsp[i].lspid;
    		  stLps.protEntity.portIndex = lspprot->dclsp[i].nextport;
    		}
    	}
        if (lspprot->lspnum == 1)
    	{
    	    if ((stLps.protEntity.rsvpKey.lsp_id == 0)
    	            && (stLps.protEntity.portIndex == 0))
    	    {
    	        stLps.protEntity.rsvpKey.lsp_id = stLps.workEntity.rsvpKey.lsp_id;
    	        stLps.protEntity.portIndex = stLps.workEntity.portIndex;
    	    }
    	    else if ((stLps.workEntity.rsvpKey.lsp_id == 0) 
    	                && (stLps.workEntity.portIndex == 0))
    	    {
    	        stLps.workEntity.rsvpKey.lsp_id = stLps.protEntity.rsvpKey.lsp_id;
    	        stLps.workEntity.portIndex = stLps.protEntity.portIndex;
    	    }
    	}
		
        stLps.protType = TUNNEL_TYPE;
        stLps.protGroupId = lspprot->index;
        stLps.lpsType = FRR_1B1;
        spm_lps_add_entity(&stLps);
        rv = 0;

    }
    EXIT_LABEL:
    return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_lspprot_del_drv_tunnel
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除驱动tunnel配置
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_lspprot_del_drv_tunnel(LSPPROT *lspprot NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    NBB_INT  rv2 = 1;
    if (lspprot == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e lspprot_create_drv_tunnel lspprot = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
#ifdef SPU
    rv = ApiC3DeleteTunnel( BCMC31, lspprot->index);
    if ( SHARED.c3_num == 2 )
    {
        rv2 = ApiC3DeleteTunnel( BCMC32, lspprot->index);
        rv = (rv|rv2);
    }
    if( rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e del lspprot:del drv tunnel ,rv=%d,\n",
                                                                        __LINE__,rv);
        goto  EXIT_LABEL;
    }
    else
    {
        lspprot->index = 0;
    }
#endif
     EXIT_LABEL:
     return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_lspprot_del_drv_tunnel
* 负责人   ： hwenjie
* 创建日期 ： 2016年2月29日
* 函数功能 ： 删除驱动tunnel配置
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_lspprot_del_tunnel_aps(LSPPROT *lspprot NBB_CCXT_T NBB_CXT)
{
    NBB_INT  rv = 1;
    if (lspprot == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e lspprot_create_drv_tunnel lspprot = NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    }
#ifdef SPU
   spm_lps_del_entity( TUNNEL_TYPE, lspprot->index);
   rv = 0;
#endif
     EXIT_LABEL:
     return (rv);
}


