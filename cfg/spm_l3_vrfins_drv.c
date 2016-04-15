/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_l3_vrfins_drv.c
  版 本 号   : v0
  作    者   : wjhe
  生成日期   : 2016年4月13日
  最近修改   :
  功能描述   : vrf实例适配程序
  函数列表   :
  修改历史   :

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*************************************************************************

* 函数名   ： spm_l3_vrfins_add_hqos_drv
* 负责人   ： hwenjie
* 创建日期 ： 2016年4月13日
* 函数功能 ： 增加vrf实例qos
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_vrfins_add_hqos_drv(VRFINS *vrfins,NBB_ULONG nhid )
{
    NBB_INT  rv = 1;

	/*入参检查*/
    if (vrfins == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e vrfins_add_hqos_drv vrfins NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 

    rv = spm_hqos_add_vrf_node(vrfins->slot,vrfins->port,nhid,&(vrfins->qos_tunkey),&(vrfins->key),
                                                               &(vrfins->upqos));
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e spm_hqos_add_vrf_node.%d\n", __LINE__,rv);
    }
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* 函数名   ： spm_l3_vrfins_del_hqos_drv
* 负责人   ： hwenjie
* 创建日期 ： 2016年4月13日
* 函数功能 ： 删除vrf实例qos
* 输入参数 ：                            
* 输出参数 ： 无
* 返回值：    
                                    
* 调用关系：
* 其    它： 
*************************************************************************/
NBB_INT spm_l3_vrfins_del_hqos_drv(VRFINS *vrfins,NBB_ULONG nhid )
{
    NBB_INT  rv = 1;

	/*入参检查*/
    if (vrfins == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e vrfins_del_hqos_drv vrfins NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 

    rv = spm_hqos_del_vrf_node(vrfins->slot,vrfins->port,&(vrfins->qos_tunkey),&(vrfins->key),nhid);
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e spm_hqos_del_vrf_node.%d\n", __LINE__,rv);
    }
   EXIT_LABEL:
   return (rv);
}



