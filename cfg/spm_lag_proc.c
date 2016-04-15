/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************

  文 件 名   : spm_lag_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2012年11月30日
  最近修改   :
  功能描述   : 链路聚合处理文件
  函数列表   :
  修改历史   :
  1.日    期   : 2012年11月30日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char lag_cfg_print_setting;

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_link_aggr
 功能描述  : 链路聚合IPS消息处理函数
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月30日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_link_aggr(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    
    NBB_INT ret = SUCCESS;
    
    SPM_LAG_CB *pstLag = NULL;
	
    /* 子配置的操作模式 */
    NBB_ULONG ulOperGlobal  = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLag     = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperPort    = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperNlbPort = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv4Nf = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpSample = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperIpv6Nf = ATG_DCI_OPER_NULL;
	NBB_ULONG ulOperMc = ATG_DCI_OPER_NULL;
    NBB_ULONG ulOperLinkDetect = ATG_DCI_OPER_NULL;

    /* LAG的键值 */
    NBB_ULONG ulLagIdKey = 0;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_BOOL avll_ret_code;

    NBB_TRC_ENTRY("spm_rcv_dci_set_link_aggr");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetLinkAggr != NULL);

	if (pstSetLinkAggr == NULL)
	{
        SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d\n",LAG_PROC_IPS_NULL);        
        goto EXIT_LABEL;
    }
	

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetLinkAggr->return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->glob_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->lag_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->nlb_port_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->ipv4nf_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->ipnf_sampler_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->ipv6nf_return_code = ATG_DCI_RC_OK;
	pstSetLinkAggr->mc_return_code = ATG_DCI_RC_OK;
    pstSetLinkAggr->link_detect_return_code = ATG_DCI_RC_OK;

    
    ulLagIdKey = pstSetLinkAggr->key;    
    pstLag = AVLL_FIND(SHARED.lag_tree, &ulLagIdKey);
    
    if (pstLag == NULL)    //条目不存在
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }
    
    /* 获取子配置的操作类型 */
    ulOperGlobal    = pstSetLinkAggr->oper_glob_attrib;
    ulOperLag       = pstSetLinkAggr->oper_lag_conf;
    ulOperPort      = pstSetLinkAggr->oper_member_port;
    ulOperNlbPort   = pstSetLinkAggr->oper_nlb_member_port;
    ulOperIpv4Nf    = pstSetLinkAggr->oper_ipv4nf;
    ulOperIpSample  = pstSetLinkAggr->oper_ipnf_sampler;
    ulOperIpv6Nf    = pstSetLinkAggr->oper_ipv6nf;
	ulOperMc        = pstSetLinkAggr->oper_mc;
    ulOperLinkDetect = pstSetLinkAggr->oper_link_detect;

    /* 删除整个条目 */
    if (pstSetLinkAggr->delete_struct == TRUE)
    {   
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {  
            SPM_L2_ERROR_PRINT_LOG(ucMessage,"r1=%d\n",LAG_PROC_DEL_NULL_LOCAL_CFG);
            goto EXIT_LABEL;
        }
        
        /* 存在，删除 */
        else
        {
            spm_dbg_print_lag_head(ulLagIdKey, SPM_OPER_DEL);
            spm_dbg_record_lag_head(ulLagIdKey, SPM_OPER_DEL);

            //删除告警注册
            if((NULL != pstLag->lag_cfg_cb)
                && (NULL != pstLag->nlb_port_cfg_cb)
                && ((ATG_DCI_MANUAL_NONE_LOADBALANCE == pstLag->lag_cfg_cb->mode)
                || (ATG_DCI_LACP_STATIC_NONE_LOADBALANCE == pstLag->lag_cfg_cb->mode)))
            {
                //带添加性能告警上报
                #ifdef SPU
                //spm_del_lag_alarm(ulLagIdKey);
                #endif
            }
            

            //刷新回调函数表
            ret = spm_lag_drv_del_lag_all(ulLagIdKey);
			if(ret != SUCCESS)
            {
                pstLag->total_ret_code = ret;
				pstLag->if_spm_del_failed = SPM_CFG_DEL_FAILED;
                goto EXIT_LABEL;
            }         
			
            //从树中删除节点
            AVLL_DELETE(SHARED.lag_tree, pstLag->spm_lag_node); 

            //释放节点内存空间
            spm_free_lag_cb(pstLag);
            pstLag = NULL;
			
        }
    }
    
    /* 增加或更新条目 */
    else
    {        
        
        /* 如果条目不存在，树中要增加条目 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            spm_dbg_print_lag_head(ulLagIdKey, SPM_OPER_ADD);
            spm_dbg_record_lag_head(ulLagIdKey, SPM_OPER_ADD);
            
            pstLag = spm_alloc_lag_cb(NBB_CXT);

            if(NULL == pstLag)
            {
            	pstLag->total_ret_code = LAG_CFG_ALLOC_MEMORY_FAILD;
                goto EXIT_LABEL;
            }

			if (pstLag != NULL)
			{
	            //申请空间后先插入到tree中
	            pstLag->lag_id_key = ulLagIdKey;
	            avll_ret_code = AVLL_INSERT(SHARED.lag_tree, pstLag->spm_lag_node);
		        NBB_ASSERT(avll_ret_code);
			}
        }
        else
        {
            spm_dbg_print_lag_head(ulLagIdKey, SPM_OPER_UPD);
            spm_dbg_record_lag_head(ulLagIdKey, SPM_OPER_UPD);
        }
       
        /******************************** 全局属性 *********************************/
		if(ATG_DCI_OPER_NULL != ulOperGlobal)
		{
			ret = spm_proc_lag_global(pstSetLinkAggr, pstLag, ulOperGlobal);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process lag global faild ret=%d\n",ret);
	            pstLag->global_cfg_return_code = ret;     
	        }
		}

        /******************************** LAG配置 *********************************/
		if(ATG_DCI_OPER_NULL != ulOperLag)
		{
			ret = spm_proc_lag_conf(pstSetLinkAggr, pstLag, ulOperLag);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process lag conf faild ret=%d\n",ret);
	            pstLag->lag_cfg_return_code = ret;    
	        }
		}
        

        /******************************** 成员端口配置 *********************************/
		if(ATG_DCI_OPER_NULL != ulOperPort)
		{
			ret = spm_proc_lag_member_port(pstSetLinkAggr,pstLag,ulOperPort);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process lag member port faild ret=%d\n",ret);
	            pstLag->port_cfg_return_code = ret;   
	        }
		}
		

        /******************************** 非负载分担成员端口配置 *********************************/
		if(ATG_DCI_OPER_NULL != ulOperNlbPort)
		{
			ret = spm_proc_nlb_member_port(pstSetLinkAggr,pstLag,ulOperNlbPort);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process nlb member port faild ret=%d\n",ret);
	            pstLag->nlb_port_cfg_return_code = ret;  
	        }
		}
		

        /*********************** IPV4流采样使能配置IPV4 NetFlow ********************************/
		if(ATG_DCI_OPER_NULL != ulOperIpv4Nf)
		{
			ret = spm_proc_lag_ipv4_nf(pstSetLinkAggr,pstLag,ulOperIpv4Nf);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process ipv4 nf faild ret=%d\n",ret);
	            pstLag->ipv4_nf_cfg_return_code = ret;  
	        }    
		}
		  

        /*********************** IPV6流采样使能配置IPV6 NetFlow ********************************/
		if(ATG_DCI_OPER_NULL != ulOperIpv6Nf)
		{
			ret = spm_proc_lag_ipv6_nf(pstSetLinkAggr,pstLag,ulOperIpv6Nf);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process ipv6 nf faild ret=%d\n",ret);
	            pstLag->ipv6_nf_cfg_return_code = ret;  
	        } 
		}
		

        /******************************** IP流采样镜像配置IPV4 NetFlow ********************************/
		if(ATG_DCI_OPER_NULL != ulOperIpSample)
		{
			ret = spm_proc_lag_ip_sample(pstSetLinkAggr,pstLag,ulOperIpSample);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process ip sample faild ret=%d\n",ret);
	            pstLag->ip_sample_cfg_return_code = ret;  
	        }
		}
		
        /**************************************链路检测 *********************************/
		if(ATG_DCI_OPER_NULL != ulOperLinkDetect)
		{
			ret = spm_proc_lag_link_detect(pstSetLinkAggr,pstLag,ulOperLinkDetect);
	        if(SUCCESS != ret)
	        {
	            SPM_L2_ERROR_PRINT_LOG(ucMessage,"process ip sample faild ret=%d\n",ret);
	            pstLag->ip_sample_cfg_return_code = ret;  
	        }
		}
		
        
        #if 1       
	    /**************************************关联接口组*********************************/
       
        //应用场景:下关联接口组时，逻辑接口未下发，
        //如果后来下发的逻辑接口为lag，在lag配置块里面反刷
        ret = spm_refresh_associateif_lag(ulLagIdKey);  
        if(SUCCESS != ret)
        {
            SPM_L2_ERROR_PRINT_LOG(ucMessage,"refresh associateif faild ret=%d\n",ret);
            pstLag->ip_sample_cfg_return_code = ret;   
        }
        #endif
    }

    EXIT_LABEL : NBB_TRC_EXIT();

}

/*****************************************************************************
 函 数 名  : spm_proc_lag_global
 功能描述  : LAG的全局配置处理
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperGlobal     
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月22日
    作    者   : huxl
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_lag_global(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperGlobal)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucGlobalDataStart = NULL;
    ATG_DCI_LINK_AGGR_GLOB_ATTRIB *pstGlobalData = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_LAG_GLOBAL_INPUT_POINTER_NULL;
        return ret;
    }

    //判断操作类型，只对增加的操作进行处理
    switch(ulOperGlobal)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
			break;

		case ATG_DCI_OPER_ADD:

            //获取global配置的首地址
            pucGlobalDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                    &pstSetLinkAggr->glob_attrib);
            if(NULL == pucGlobalDataStart)
            {
                ret = LAG_GLOBAL_CFG_NULL;
                return ret;    
            }

            pstGlobalData = (ATG_DCI_LINK_AGGR_GLOB_ATTRIB *)pucGlobalDataStart;

            //控制配置是否打印到屏幕上
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  1）全局属性\n");
                spm_dbg_print_lag_global_cfg(pstGlobalData);
            }

            //把下发的配置记录到日志中
            OS_SPRINTF(ucMessage,"  1）全局属性\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_global_cfg(pstGlobalData);

            //判断内存是否为空，为空的话申请内存
            if(NULL == pstLag->global_cfg_cb)
            {
                pstLag->global_cfg_cb = (ATG_DCI_LINK_AGGR_GLOB_ATTRIB*)NBB_MM_ALLOC(
                                sizeof(ATG_DCI_LINK_AGGR_GLOB_ATTRIB),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LAG_GLOBAL_CB);    
            }

            if(NULL != pstLag->global_cfg_cb)
            {
                //拷贝数据到本地的配置树中
                OS_MEMCPY(pstLag->global_cfg_cb, pstGlobalData, sizeof(ATG_DCI_LINK_AGGR_GLOB_ATTRIB));
            }
            else
            {
                ret = LAG_GLOBAL_CFG_ALLOC_MEMORY_FAILD;
                return ret;    
            }
            break;
            
        default:
            break;
    }
    
    return ret;    

}


/*****************************************************************************
 函 数 名  : spm_proc_lag_conf
 功能描述  : LAG的全局配置处理
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperLag     
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月22日
    作    者   : huxl
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_lag_conf(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperLag)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucLagDataStart = NULL;
    ATG_DCI_LINK_AGGR_LAG_CONF *pstLagData = NULL;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_LAG_CONF_INPUT_POINTER_NULL;
        return ret;
    }

    ulLagKey = pstSetLinkAggr->key;
    
    //判断操作类型，只对增加的操作进行处理
    switch(ulOperLag)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
			break;

		case ATG_DCI_OPER_ADD:

            //获取global配置的首地址
            pucLagDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                    &pstSetLinkAggr->lag_conf);
            if(NULL == pucLagDataStart)
            {
                ret = LAG_CONF_CFG_NULL;
                return ret;    
            }

            pstLagData = (ATG_DCI_LINK_AGGR_LAG_CONF *)pucLagDataStart;

            //控制配置是否打印到屏幕上
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  2）LAG配置\n");
                spm_dbg_print_lag_lagconf_cfg(pstLagData);
            }
            
            //把下发的配置记录到日志中
            OS_SPRINTF(ucMessage,"  2）LAG配置\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_lagconf_cfg(pstLagData);

            //判断内存是否为空，为空的话申请内存
            if(NULL == pstLag->lag_cfg_cb)
            {
                pstLag->lag_cfg_cb = (ATG_DCI_LINK_AGGR_LAG_CONF*)NBB_MM_ALLOC(
                                sizeof(ATG_DCI_LINK_AGGR_LAG_CONF),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LAG_CONF_CB);    
            }

            if(NULL != pstLag->lag_cfg_cb)
            {
                //拷贝数据到本地的配置树中
                OS_MEMCPY(pstLag->lag_cfg_cb, pstLagData, sizeof(ATG_DCI_LINK_AGGR_LAG_CONF));
            }
            else
            {
                ret = LAG_CONF_CFG_ALLOC_MEMORY_FAILD;
                return ret;    
            }
            break;
            
        default:
            break;
    }

    ret = spm_lag_drv_set_lag_attri(ulLagKey, pstLagData);
    
    return ret;    

}

/*****************************************************************************
 函 数 名  : spm_proc_lag_member_port
 功能描述  : LAG成员端口配置处理
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperPort     
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月22日
    作    者   : huxl
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_lag_member_port(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperPort)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucPortDataStart = NULL;
    ATG_DCI_LINK_AGGR_MEMBER_PORT *pstPortData[ATG_DCI_LAG_MEMBER_PORT_NUM];
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;
    NBB_BYTE i = 0;

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_LAG_CONF_INPUT_POINTER_NULL;
        return ret;
    }

    for (i = 0; i < ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
    {
        pstPortData[i] = NULL;
    }
    
    ulLagKey = pstSetLinkAggr->key;
            
    //判断操作类型，只对增加的操作进行处理
    switch(ulOperPort)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  3）成员端口配置(num = %d) UPD\n", pstSetLinkAggr->member_port_num);
            }

            OS_SPRINTF(ucMessage,"  3）成员端口配置(num = %d) UPD\n", pstSetLinkAggr->member_port_num);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            if(0 == pstSetLinkAggr->member_port_num)
            {
                //先清空原有配置，释放掉内存空间
                for (i = 0; i < ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
                {
                    if (pstLag->port_cfg_cb[i] != NULL)
                    {
                        NBB_MM_FREE(pstLag->port_cfg_cb[i], MEM_SPM_LAG_PORT_CB);
                        pstLag->port_cfg_cb[i] = NULL;
                    }
                }

                pstLag->port_num = 0;
                pstLag->lag_slot_id = 0;
                pstLag->lag_port_id = 0;
            }
            else
            {

                /* 计算第一个entry的地址。*/
                pucPortDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, &pstSetLinkAggr->member_port);    

                /* 如果指针为NULL，无数据 */
                if (pucPortDataStart == NULL)
                {
                    ret = LAG_MEMBER_PORT_CFG_NULL;
                    return ret;
                        
                }
                else
                {
                    //先清空原有配置，释放掉内存空间
                    for (i = 0; i<ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
                    {
                        if (pstLag->port_cfg_cb[i] != NULL)
                        {
                            NBB_MM_FREE(pstLag->port_cfg_cb[i], MEM_SPM_LAG_PORT_CB);
                            pstLag->port_cfg_cb[i] = NULL;                            
                        }
                    }
                    
                    for (i = 0; i < pstSetLinkAggr->member_port_num; i++)
                    {
                        pstPortData[i] = (ATG_DCI_LINK_AGGR_MEMBER_PORT *)(pucPortDataStart 
                            + (NBB_ALIGN_OFFSET(sizeof(ATG_DCI_LINK_AGGR_MEMBER_PORT))) * i); 

                        //控制配置打印到屏幕
                        if (lag_cfg_print_setting == SPM_PRINT_CFG)
                        {
                            spm_dbg_print_lag_port_cfg(pstPortData[i]);
                        }

                        //把成员端口的配置记录到日志
                        spm_dbg_record_lag_port_cfg(pstPortData[i]);

                        if (pstLag->port_cfg_cb[i] == NULL)
                        {
                            pstLag->port_cfg_cb[i] = (ATG_DCI_LINK_AGGR_MEMBER_PORT *)NBB_MM_ALLOC(
                                sizeof(ATG_DCI_LINK_AGGR_MEMBER_PORT),
                                NBB_NORETRY_ACT,
                                MEM_SPM_LAG_PORT_CB);

                            if (pstLag->port_cfg_cb[i] == NULL)
                            {
                                ret = LAG_MEMBER_PORT_ALLOC_MEMORY_FAILD;
                                return ret;
                    			
                            }
                        }
 
                        //保存配置到本地
                        OS_MEMCPY(pstLag->port_cfg_cb[i], pstPortData[i], sizeof(ATG_DCI_LINK_AGGR_MEMBER_PORT));

                    } 
                    pstLag->member_port_num = pstSetLinkAggr->member_port_num;
                    pstLag->port_num = pstSetLinkAggr->member_port_num;
                }  
            }
			break;

		case ATG_DCI_OPER_ADD:
            break;
            
        default:
            break;
    }
    if((pstLag->lag_cfg_cb->mode == ATG_DCI_MANUAL_LOADBALANCE)
     ||(pstLag->lag_cfg_cb->mode == ATG_DCI_LACP_STATIC_LOADBALANCE))
    {
        ret = spm_lag_drv_set_port_cfg(pstLag);
    }
    else
    {
        ret = MEMBER_PORT_MOD_IS_ERROR;
    }
     
    return ret;    

}

/*****************************************************************************
 函 数 名  : spm_proc_nlb_member_port
 功能描述  : LAG非负载成员端口配置处理
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperPort     
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月22日
    作    者   : huxl
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_nlb_member_port(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperNlbPort)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucNlbPortDataStart = NULL;
    ATG_DCI_LAG_NLB_MEMBER_PORT *pstNlbPortData;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;    

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = LAG_NLB_PORT_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperNlbPort)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            pucNlbPortDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                     &pstSetLinkAggr->nlb_member_port);    

            /* 首地址为NULL，异常 */
            if (pucNlbPortDataStart == NULL)
            {
                ret = LAG_NLB_PORT_CFG_NULL;
                return ret;
            }

            pstNlbPortData = (ATG_DCI_LAG_NLB_MEMBER_PORT*)pucNlbPortDataStart;

            //控制打印到屏幕
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  4）非负载分担成员端口配置\n");
                spm_dbg_print_lag_nlb_port_cfg(pstNlbPortData);
            }

            //记录配置到日志文件
            OS_SPRINTF(ucMessage,"  4）非负载分担成员端口配置\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_nlb_port_cfg(pstNlbPortData);

            if (pstLag->nlb_port_cfg_cb == NULL)
            {
                //申请内存
                pstLag->nlb_port_cfg_cb = (ATG_DCI_LAG_NLB_MEMBER_PORT*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_NLB_MEMBER_PORT),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_NLB_PORT_CB);
                
                if (pstLag->nlb_port_cfg_cb == NULL)
                {
                    ret = LAG_NLB_PORT_ALLOC_MEMORY_FAILD;
                    return ret;
        			
                }                
            }                

            //保存配置到本地
            if(NULL != pstLag->nlb_port_cfg_cb)
            {
                OS_MEMCPY(pstLag->nlb_port_cfg_cb, pstNlbPortData, sizeof(ATG_DCI_LAG_NLB_MEMBER_PORT));
            }                    
            break;

        default:
            break;        
    }
    if((pstLag->lag_cfg_cb->mode == ATG_DCI_MANUAL_NONE_LOADBALANCE)
     ||(pstLag->lag_cfg_cb->mode == ATG_DCI_LACP_STATIC_NONE_LOADBALANCE))
    {
        ret = spm_lag_drv_set_nlb_member_cfg(pstLag);
    }
    else
    {
        ret = NLB_MEMBER_PORT_MOD_IS_ERROR;
    }
    return ret;
}

/*****************************************************************************
 函 数 名  : spm_proc_lag_ipv4_nf
 功能描述  : LAGIPV4流采样配置处理
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperPort     
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月22日
    作    者   : huxl
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_lag_ipv4_nf(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperIpv4Nf)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucIpv4NfDataStart = NULL;
    ATG_DCI_LAG_IPV4NF_DATA *pstIpv4NfData;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0; 
    
    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_IPV4_NF_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperIpv4Nf)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            
            /* 计算第一个entry的地址。*/
            pucIpv4NfDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->ipv4nf_data);    

            /* 首地址为NULL，异常 */
            if (pucIpv4NfDataStart == NULL)
            {
                ret = IPV4_NF_CFG_NULL;
                return ret;    
            }

            pstIpv4NfData = (ATG_DCI_LAG_IPV4NF_DATA*)pucIpv4NfDataStart;                
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  5）ipv4流采样使能配置\n");
                spm_dbg_print_lag_ipv4_nf_data(pstIpv4NfData);
            }

            OS_SPRINTF(ucMessage,"  5）ipv4流采样使能配置\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_ipv4_nf_data(pstIpv4NfData);

            /* 保存数据 */
            if (pstLag->ipv4_nf_data == NULL)
            {
                pstLag->ipv4_nf_data = (ATG_DCI_LAG_IPV4NF_DATA*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_IPV4NF_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_IPV4_NF_CB);

                if (pstLag->ipv4_nf_data == NULL)
                {
                    ret = IPV4_NF_ALLOC_MEMORY_FAILD;
                    return ret;
                	
                }
            }

            if(NULL != pstLag->ipv4_nf_data)
            {
                OS_MEMCPY(pstLag->ipv4_nf_data, pstIpv4NfData, sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA));
            }
            
            break;
            
        default:
            break;
    }

    ret = spm_lag_drv_set_ipv4_nf_cfg(pstLag);
    return ret; 
}

/*****************************************************************************
 函 数 名  : spm_proc_lag_ipv6_nf
 功能描述  : LAGIPV6流采样配置处理
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperIpv6Nf     
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月22日
    作    者   : huxl
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_lag_ipv6_nf(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperIpv6Nf)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucIpv6NfDataStart = NULL;
    ATG_DCI_LAG_IPV6NF_DATA *pstIpv6NfData;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0; 
    
    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_IPV6_NF_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperIpv6Nf)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            
            /* 计算第一个entry的地址。*/
            pucIpv6NfDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->ipv6nf_data);    

            /* 首地址为NULL，异常 */
            if (pucIpv6NfDataStart == NULL)
            {
                ret = IPV6_NF_CFG_NULL;
                return ret;
            }
            pstIpv6NfData = (ATG_DCI_LAG_IPV6NF_DATA*)pucIpv6NfDataStart;

            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  7）ipv6流采样使能配置\n");
                spm_dbg_print_lag_ipv6_nf_data(pstIpv6NfData);
            }

            OS_SPRINTF(ucMessage,"  7）ipv6流采样使能配置\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_ipv6_nf_data(pstIpv6NfData);

            /* 保存数据 */
            if (pstLag->ipv6_nf_data == NULL)
            {
                pstLag->ipv6_nf_data = (ATG_DCI_LAG_IPV6NF_DATA*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_IPV6NF_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_IPV6_NF_CB);

                if (pstLag->ipv6_nf_data == NULL)
                {                   
        		    ret = IPV6_NF_ALLOC_MEMORY_FAILD;
                    return ret;	
                }
            }
            
            if(pstLag->ipv6_nf_data != NULL)
            {              
                OS_MEMCPY(pstLag->ipv6_nf_data, pstIpv6NfData, sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA));
            }
            
            break;
            
        default:
            break;
    }

    ret = spm_lag_drv_set_ipv6_nf_cfg(pstLag);
    return ret; 
    
}


/*****************************************************************************
 函 数 名  : spm_proc_lag_ipv6_nf
 功能描述  : LAGIP流采样镜像配置处理
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperIpSample     
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月22日
    作    者   : huxl
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_lag_ip_sample(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperIpSample)
{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *pucIpSampleDataStart = NULL;
    ATG_DCI_LAG_IPNF_SAMPLER_DATA *pstIpSampleData;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0; 

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_IP_SAMPLE_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperIpSample)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            /* 计算第一个entry的地址。*/
            pucIpSampleDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->ipnf_sampler_data);    

            /* 首地址为NULL，异常 */
            if (pucIpSampleDataStart == NULL)
            {
                ret = IP_SAMPLE_CFG_NULL;
                return ret;   
            }

            pstIpSampleData = (ATG_DCI_LAG_IPNF_SAMPLER_DATA*)pucIpSampleDataStart; 

            //打印配置到屏幕
            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  6）ip流采样镜像配置\n");
                spm_dbg_print_lag_ip_sampler_data(pstIpSampleData);
            }

            //把配置数据记录到日志文件
            OS_SPRINTF(ucMessage,"  6）ip流采样镜像配置\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_ip_sampler_data(pstIpSampleData);

            if (pstLag->ip_sample_data == NULL)
            {
                //为存储的数据申请内存
                pstLag->ip_sample_data = (ATG_DCI_LAG_IPNF_SAMPLER_DATA*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_IPNF_SAMPLER_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_IPNF_SAMPLER);
                
                if (pstLag->ip_sample_data == NULL)
        		{
        		    ret = IP_SAMPLE_ALLOC_MEMORY_FAILD;
                    return ret;     
        		}
            }  
            if(pstLag->ip_sample_data != NULL)
            {
                //保存数据到本地配置树
                OS_MEMCPY(pstLag->ip_sample_data, pstIpSampleData, sizeof(ATG_DCI_LAG_IPNF_SAMPLER_DATA));
            } 
            
            break;
            
        default:
            break;
    }

    ret = spm_lag_drv_set_ip_sample_cfg(pstLag);
    return ret;    

}



/*****************************************************************************
 函 数 名  : spm_proc_lag_mc_lag
 功能描述  : MC-LAG处理
 输入参数  : 
                
 输出参数  : 
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_lag_mc_lag(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperMc)

{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *puc_mc_start = NULL;
    ATG_DCI_LAG_MC_DATA *pst_mc_data;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;     

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        return PROC_MC_LAG_INPUT_POINTER_NULL;
    }

    switch(ulOperMc)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            
            /* 计算第一个entry的地址。*/
            puc_mc_start = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->mc_data);    

            /* 首地址为NULL，异常 */
            if (puc_mc_start == NULL)
            {
                return MC_LAG_DATA_START_NULL;
                
            }
            pst_mc_data = (ATG_DCI_LAG_MC_DATA *)puc_mc_start;

            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  8）MC-LAG\n");
                spm_dbg_print_lag_mc_lag_data(pst_mc_data);
            }

            OS_SPRINTF(ucMessage,"  8）MC-LAG\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_mc_lag_data(pst_mc_data);

            /* 保存数据 */
            if (pstLag->mc_data == NULL)
            {
                pstLag->mc_data = (ATG_DCI_LAG_MC_DATA *)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_MC_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_MC_LAG_CB);

                if (pstLag->mc_data == NULL)
                {                   
        		    return MC_LAG_ALLOC_MEMORY_FAILD;
                }
            }
            
            if(pstLag->mc_data != NULL)
            {              
                OS_MEMCPY(pstLag->mc_data, pst_mc_data, sizeof(ATG_DCI_LAG_MC_DATA));
            }
            
            break;
            
        default:
            break;
    }

    ret = spm_set_lag_min_member(pstLag);
    return ret; 
    
}


/*****************************************************************************
 函 数 名  : spm_proc_lag_ipv6_nf
 功能描述  : LAGIP流采样镜像配置处理
 输入参数  : ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr  SPM_LAG_CB *pstLag  
             NBB_ULONG ulOperLinkDetect     
 输出参数  : 无
 返 回 值  : SUCCESS/ERROR
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2016年1月22日
    作    者   : huxl
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_proc_lag_link_detect(ATG_DCI_SET_LINK_AGGR *pstSetLinkAggr, 
                                    SPM_LAG_CB *pstLag, NBB_ULONG ulOperLinkDetect)

{
    NBB_INT ret = SUCCESS;
    NBB_BYTE *puc_link_detect = NULL;
    ATG_DCI_LAG_LINK_DETECT_DATA *pst_link_detect;
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN] = {0};
    NBB_ULONG ulLagKey = 0;     

    if((NULL == pstSetLinkAggr) || (NULL == pstLag))
    {
        ret = PROC_IPV6_NF_INPUT_POINTER_NULL;
        return ret;
    }

    switch(ulOperLinkDetect)
    {
        case ATG_DCI_OPER_DEL:	
		    break;

		case ATG_DCI_OPER_UPDATE:
            break;

        case ATG_DCI_OPER_ADD:
            
            /* 计算第一个entry的地址。*/
            puc_link_detect = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetLinkAggr, 
                                                                  &pstSetLinkAggr->link_detect_data);    

            /* 首地址为NULL，异常 */
            if (puc_link_detect == NULL)
            {
                ret = LINK_DETECT_CFG_NULL;
                return ret;
            }
            pst_link_detect = (ATG_DCI_LAG_LINK_DETECT_DATA *)puc_link_detect;

            if (lag_cfg_print_setting == SPM_PRINT_CFG)
            {
                printf("  9）链路检测配置\n");
                spm_dbg_print_lag_link_detect_data(pst_link_detect);
            }

            OS_SPRINTF(ucMessage,"  9）链路检测配置\n");
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
            spm_dbg_record_lag_link_detect_data(pst_link_detect);

            /* 保存数据 */
            if (pstLag->link_detect_cfg == NULL)
            {
                pstLag->link_detect_cfg = (ATG_DCI_LAG_LINK_DETECT_DATA*)NBB_MM_ALLOC(
                    sizeof(ATG_DCI_LAG_LINK_DETECT_DATA),
                    NBB_NORETRY_ACT,
                    MEM_SPM_LAG_LINK_DETECT_CB);

                if (pstLag->link_detect_cfg == NULL)
                {                   
        		    ret = LINK_DETECT_ALLOC_MEMORY_FAILD;
                    return ret;	
                }
            }
            
            if(pstLag->link_detect_cfg != NULL)
            {              
                OS_MEMCPY(pstLag->link_detect_cfg, pst_link_detect, sizeof(ATG_DCI_LAG_LINK_DETECT_DATA));
            }
            
            break;
            
        default:
            break;
    }

   ret = spm_lag_drv_set_ipv6_nf_cfg(pstLag);
    return ret; 
    
}


/*****************************************************************************
 函 数 名  : spm_refresh_associateif_lag
 功能描述  : 应用场景:下关联接口组时，逻辑接口未下发，
             如果后来下发的逻辑接口为lag，在lag配置块里面反刷
 输入参数  : NBB_ULONG ulLagId         
 输出参数  : 无
 返 回 值  : -1/0 = 失败/成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年10月31日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_refresh_associateif_lag(NBB_ULONG lag_key)
{
    NBB_INT ret = SUCCESS;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_ASSOCIATE_IF_CB *pstAssociateIfCb = NULL;
    NBB_ULONG ulPortIndexKey = 0;
    NBB_ULONG ulAssociateIfKey = 0;
    NBB_BYTE trigger_if_flag = 0;
    NBB_BYTE act_if_flag = 0;
    NBB_BYTE ucExistPos_trigger = 0;
    NBB_BYTE ucExistPos_act = 0;
    NBB_BYTE pucPortNum = 0;
    NBB_USHORT usPortId = 0;
    NBB_BYTE usSlotId = 0;
    NBB_BYTE i = 0;
    SPM_LAG_PORT_INFO_CB  stTriggerLagPortInfo[ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM];
    SPM_LAG_PORT_INFO_CB  stActLagPortInfo[ATG_DCI_ASSOCIATE_IF_ACT_NUM];    
    

    OS_MEMSET(stTriggerLagPortInfo,0,sizeof(SPM_LAG_PORT_INFO_CB)*(ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM));
    OS_MEMSET(stActLagPortInfo,0,sizeof(SPM_LAG_PORT_INFO_CB)*(ATG_DCI_ASSOCIATE_IF_ACT_NUM));

    for (pstLogicalPort = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree);
            pstLogicalPort != NULL;
            pstLogicalPort = (SPM_LOGICAL_PORT_CB*) AVLL_NEXT(SHARED.logical_port_tree,pstLogicalPort->spm_logical_port_node))
    {
        if((pstLogicalPort->basic_cfg_cb != NULL)&&(pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_LAG) 
            &&(pstLogicalPort->phy_cfg_cb != NULL)&&(pstLogicalPort->phy_cfg_cb->lag_id == lag_key))
        {
            ulPortIndexKey = pstLogicalPort->port_index_key;
        }
    }
        
     for (pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB *)AVLL_FIRST(v_spm_shared->associate_if_tree);
                 pstAssociateIfCb != NULL;
                 pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB *)AVLL_NEXT(v_spm_shared->associate_if_tree,
                 pstAssociateIfCb->spm_associate_if_node))
    {
        for(i = 0;i < ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM;i++)
        {
           if((pstAssociateIfCb->trigger_if_cfg_cb[i] != NULL)&&(pstAssociateIfCb->trigger_if_cfg_cb[i]->interface_index == ulPortIndexKey) 
            && (pstAssociateIfCb->trigger_if_exist[i] == UNEXIST))
           {
                trigger_if_flag = 1;
                ucExistPos_trigger = i;
                ulAssociateIfKey = pstAssociateIfCb->key;
                break;
           }
            
        }
     
        if(trigger_if_flag != 1)
        {
           for(i = 0;i < ATG_DCI_ASSOCIATE_IF_ACT_NUM;i++)
            {
               if((pstAssociateIfCb->act_if_cfg_cb[i] != NULL)&&(pstAssociateIfCb->act_if_cfg_cb[i]->interface_index == ulPortIndexKey)
                && (pstAssociateIfCb->act_if_exist[i] == UNEXIST))
               {
                    act_if_flag = 1;
                    ucExistPos_act = i;
                    ulAssociateIfKey = pstAssociateIfCb->key;
                    break;
               }
                
            } 
        }

        if((trigger_if_flag == 1) || (act_if_flag == 1))
        {
            break;
        }
        
    }

     /*把lag的成员端口对应的物理口加入关联组的触发接口中*/
    if(trigger_if_flag == 1)
    {
        spm_get_lag_all_ports_info(lag_key,stTriggerLagPortInfo,&pucPortNum NBB_CCXT);

		if(pucPortNum == 0)
		{
   		    ret = REFRESH_ASSOCIATIATE_GET_PORT_INFO_FAILD;
            return ret;
		}

		else
		{

            ret = spm_add_trigger_lag_port_info(pucPortNum, ulAssociateIfKey, stTriggerLagPortInfo);						
			if(ret == SUCCESS)
			{
			    if(pstAssociateIfCb != NULL)
                {
                    pstAssociateIfCb->trigger_if_exist[ucExistPos_trigger] = EXIST;
			        pstAssociateIfCb->trigger_if_num++;  
                }         
                
			} 
            else
            {              
                return ret;    
            }

        }
    }
    
    /*把lag成员端口加入关联组的动作接口*/
    else if(act_if_flag == 1)
    {
       spm_get_lag_all_ports_info(lag_key,stActLagPortInfo,&pucPortNum NBB_CCXT);

    	if(pucPortNum == 0)
    	{   	    
            ret = REFRESH_ASSOCIATIATE_GET_PORT_INFO_FAILD_2;
            return ret;
    	}

    	else
    	{

    	    ret = spm_add_ifgoup_lag_port_info(pucPortNum, ulAssociateIfKey, stActLagPortInfo);   					
    		if(ret == SUCCESS)
    		{
    		    if(pstAssociateIfCb != NULL)
                {
                    pstAssociateIfCb->act_if_exist[ucExistPos_act] = EXIST;
    		        pstAssociateIfCb->act_if_num++;  
                }         
                
    		} 

            else
            {
                return ret;         
            }

        }
    }

    return ret;
}




/*****************************************************************************
 函 数 名  : spm_refresh_vpls_lag
 功能描述  : 刷新vpls的lag
 输入参数  : NBB_ULONG ulLagId         
 输出参数  : 无
 返 回 值  : -1/0 = 失败/成功
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年10月31日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_INT spm_refresh_vpls_lag(NBB_ULONG ulLagId, NBB_ULONG ulPortIndex, 
									NBB_BYTE ucLagSlotCur, NBB_USHORT usLagPortCur,
									NBB_BYTE ucLagSlotPre, NBB_USHORT usLagPortPre
									NBB_CCXT_T NBB_CXT)
{
	#if 0
	NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
	NBB_BYTE ucFlag = 0;
	NBB_INT i = 0;
	NBB_INT ret = SUCCESS;
	NBB_INT iUniPos = -1;
	NBB_INT iVpId = 0;
	SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
	SPM_VPLS_CB *pstVpls = NULL;

	//coverity[returned_pointer]
    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &ulPortIndex);

    /*************************************************************************/
    /* 遍历VPLS 配置的树逐一进行验证。                                        */
    /*************************************************************************/
    for (pstVpls = (SPM_VPLS_CB*) AVLL_FIRST(SHARED.vpls_tree);
         pstVpls != NULL;
         pstVpls = (SPM_VPLS_CB*) AVLL_NEXT(SHARED.vpls_tree,
                   pstVpls->spm_vpls_node))
    {
    	//这里先考虑LAG为uni的情况，nni的情况比较复杂
    	for (i=0; i<ATG_DCI_VPLS_UNI_NUM; i++)
    	{
    		//如果找到逻辑端口所在的VPLS
    		if ((pstVpls->uni_cfg_cb[i] != NULL) && (pstVpls->uni_cfg_cb[i]->port_index == ulPortIndex))
    		{
    			iUniPos = i;
    			iVpId = pstVpls->uni_info_cb[i].vp_idx;

#ifdef SRC
				//小漏洞:成员端口删除干净后，再增加成员口在另外的槽位，那么之前的成员口所在的槽无法删除
				if (ucLagSlotCur != 0)
				{
					//删除SRC盘向老槽位复制包
					ret = ApiDfeMulticastDelete(UNIT_0, pstVpls->vpls_id_key, ucLagSlotPre);			

					//增加SRC盘向新槽位复制包
					ret = ApiDfeMulticastAdd(UNIT_0, pstVpls->vpls_id_key, ucLagSlotCur);
				}				
#endif

#ifdef SPU
				//在老的槽位上删除配置给Arad的包复制
				if (SHARED.local_slot_id == ucLagSlotPre)
				{
					//ret = ApiAradDeleteEngressMcPort(UNIT_0, pstVpls->vpls_id_key, usLagPortPre, iVpId);
					ret = ApiAradDeleteEngressMcPort(UNIT_0, pstVpls->vpls_id_key, SPM_MULTICAST_PORT_0, iVpId);
					pstVpls->uni_info_cb[i].slot_id = 0;
					//pstVpls->uni_info_cb[i].cud = 0;
				}

				//在新的槽位上增加配置给Arad的包复制
				if ((SHARED.local_slot_id == ucLagSlotCur) && (usLagPortCur != 0))
				{
					//ret = ApiAradAddEngressMcPort(UNIT_0, pstVpls->vpls_id_key, usLagPortCur, iVpId);
					ret = ApiAradAddEngressMcPort(UNIT_0, pstVpls->vpls_id_key, SPM_MULTICAST_PORT_0, iVpId);
					pstVpls->uni_info_cb[i].slot_id = ucLagSlotCur;
					//pstVpls->uni_info_cb[i].cud = iVpId;
				}
#endif
						

    			break;
    		}
    	}

#if 0
    	//考虑LAG为nni的情况
    	for (i=0; i<ATG_DCI_VPLS_NNI_NUM; i++)
    	{
    		//如果找到逻辑端口所在的VPLS
    		if ((pstVpls->nni_cfg_cb[i] != NULL) && (pstVpls->nni_info_cb[i].port_index_mc == ulPortIndex))
    		{
    			iUniPos = i;
    			iVpId = pstVpls->nni_info_cb[i].vp_idx;

#ifdef SRC
				//小漏洞:成员端口删除干净后，再增加成员口在另外的槽位，那么之前的成员口所在的槽无法删除
				if (ucLagSlotCur != 0)
				{
					//删除SRC盘向老槽位复制包
					ret = ApiDfeMulticastDelete(UNIT_0, pstVpls->vpls_id_key, ucLagSlotPre);			

					//增加SRC盘向新槽位复制包
					ret = ApiDfeMulticastAdd(UNIT_0, pstVpls->vpls_id_key, ucLagSlotCur);
				}				
#endif

#ifdef SPU
				//在老的槽位上删除配置给Arad的包复制
				if (SHARED.local_slot_id == ucLagSlotPre)
				{
					ret = ApiAradDeleteEngressMcPort(UNIT_0, pstVpls->vpls_id_key, SPM_MULTICAST_PORT_0, iVpId);
					pstVpls->nni_info_cb[i].slot_id = 0;
				}

				//在新的槽位上增加配置给Arad的包复制
				if ((SHARED.local_slot_id == ucLagSlotCur) && (usLagPortCur != 0))
				{
					ret = ApiAradAddEngressMcPort(UNIT_0, pstVpls->vpls_id_key, SPM_MULTICAST_PORT_0, iVpId);
					pstVpls->nni_info_cb[i].slot_id = ucLagSlotCur;
				}
#endif						

    			break;
    		}
    	}
#endif
    	if (iUniPos != -1)
    	{
    		break;
    	}
    }    

	//如果未找到逻辑端口所在的VPLS，直接返回错
    if (iUniPos == -1)
    {
    	OS_PRINTF("***ERROR***: 未找到逻辑端口 PORT_INDEX=%ld 所在的VPLS，直接返回错\n", ulPortIndex);

        OS_SPRINTF(ucMessage, "***ERROR***: 未找到逻辑端口 PORT_INDEX=%ld 所在的VPLS，直接返回错\n", ulPortIndex);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | LAG_PD, 0, "ld d s s", ulLagId, BASIC_CONFIG_NOT_EXIST, "BASIC_CONFIG_NOT_EXIST", ucMessage));
    	return ERROR;    	
    }

	#endif

    return SUCCESS;
}

/*****************************************************************************
 函 数 名  : spm_init_lag_tree
 功能描述  : 初始化LAG配置存储树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年1月23日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_lag_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_lag_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_LAG_CB, lag_id_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.lag_tree, compare_ulong,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_LAG_CB, spm_lag_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_alloc_lag_cb
 功能描述  : 申请链路聚合配置的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : SPM_LAG_CB
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年12月1日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_LAG_CB * spm_alloc_lag_cb(NBB_CXT_T NBB_CXT)
{
    NBB_INT i = 0;
    SPM_LAG_CB *pstLag = NULL;

    NBB_TRC_ENTRY("spm_alloc_lag_cb");

    /* 分配一个新的VPWS配置条目。*/
    pstLag = (SPM_LAG_CB *)NBB_MM_ALLOC(sizeof(SPM_LAG_CB), NBB_NORETRY_ACT, MEM_SPM_LAG_CB);
    if (pstLag == NULL)
    {
        //NBB_TRC_FLOW((NBB_FORMAT "***ERROR***:申请LAG条目空间为NULL!"));
        goto EXIT_LABEL;
    }

    /* 初始化VPWS配置条目 */
    OS_MEMSET(pstLag, 0, sizeof(SPM_LAG_CB));
    pstLag->global_cfg_cb = NULL;
    pstLag->lag_cfg_cb = NULL;
    pstLag->nlb_port_cfg_cb = NULL;
    pstLag->ipv4_nf_data = NULL;
    pstLag->ipv6_nf_data = NULL;
    pstLag->ip_sample_data = NULL;
	pstLag->mc_data = NULL;
    pstLag->link_detect_cfg = NULL;

    for (i = 0; i<ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
    {
        pstLag->port_cfg_cb[i] = NULL;
    }

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pstLag->spm_lag_handle = NBB_CREATE_HANDLE(pstLag, HDL_SPM_LAG_CB);

    /* 成功分配一个新的接口物理配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_LAG_CB allocated at %p with handle %#lx",
                   pstLag, pstLag->spm_lag_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstLag->spm_lag_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstLag);
}

/*****************************************************************************
 函 数 名  : spm_free_lag_cb
 功能描述  : 释放LAG的内存空间
 输入参数  : SPM_LAG_CB *pstLag  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年12月1日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_lag_cb(SPM_LAG_CB *pstLag)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_lag_cb");

    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstLag, sizeof(SPM_LAG_CB), MEM_SPM_LAG_CB); 

    if (pstLag == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    ////NBB_TRC_FLOW((NBB_FORMAT "Free vpws cb at %p", pstLag));  
    //NBB_TRC_FLOW((NBB_FORMAT "Free vpws_id %d", pstLag->lag_id_key));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstLag->global_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLag->global_cfg_cb, MEM_SPM_LAG_GLOBAL_CB);  
        pstLag->global_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放LAG配置数据块。                                                */
    /***************************************************************************/
    if (pstLag->lag_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLag->lag_cfg_cb, MEM_SPM_LAG_CONF_CB);  
        pstLag->lag_cfg_cb = NULL;
    }

    /***************************************************************************/
    /* 释放非负载分担成员端口配置数据块。                                      */
    /***************************************************************************/
    if (pstLag->nlb_port_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstLag->nlb_port_cfg_cb, MEM_SPM_LAG_NLB_PORT_CB);  
        pstLag->nlb_port_cfg_cb = NULL;
    }
    
    /***************************************************************************/
    /* 释放成员端口配置数据块。                                                */
    /***************************************************************************/
    for (i = 0; i<ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
    {
        if (pstLag->port_cfg_cb[i] != NULL)
        {
            NBB_MM_FREE(pstLag->port_cfg_cb[i], MEM_SPM_LAG_PORT_CB);  
            pstLag->port_cfg_cb[i] = NULL;
        }
    }

	pstLag->port_num = 0;
    
    /***************************************************************************/
    /* 释放ipv4_nf_data数据块。                                                */
    /***************************************************************************/
    if(pstLag->ipv4_nf_data != NULL)
     {
        NBB_MM_FREE(pstLag->ipv4_nf_data, MEM_SPM_LAG_IPV4_NF_CB);  
        pstLag->ipv4_nf_data = NULL;
     }
    
    /***************************************************************************/
    /* 释放ipv6_nf_data数据块。                                                */
    /***************************************************************************/
    if(pstLag->ipv6_nf_data != NULL)
     {
        NBB_MM_FREE(pstLag->ipv6_nf_data, MEM_SPM_LAG_IPV6_NF_CB);  
        pstLag->ipv6_nf_data = NULL;
     }
    
    /***************************************************************************/
    /* 释放流采样镜像数据块。                                                */
    /***************************************************************************/
    if(pstLag->ip_sample_data!= NULL)
     {
        NBB_MM_FREE(pstLag->ip_sample_data, MEM_SPM_LAG_IPNF_SAMPLER);  
        pstLag->ip_sample_data = NULL;
     }

	/***************************************************************************/
    /* 释放MC-LAG数据块。                                                */
    /***************************************************************************/
    if(pstLag->mc_data != NULL)
     {
        NBB_MM_FREE(pstLag->mc_data, MEM_SPM_LAG_MC_LAG_CB);  
        pstLag->mc_data = NULL;
     }

    /***************************************************************************/
    /* 释放链路检测数据块。                                                */
    /***************************************************************************/
    if(pstLag->link_detect_cfg!= NULL)
     {
        NBB_MM_FREE(pstLag->link_detect_cfg, MEM_SPM_LAG_LINK_DETECT_CB);  
        pstLag->link_detect_cfg = NULL;
     }
    
    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstLag->spm_lag_handle, HDL_SPM_LAG_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstLag, MEM_SPM_LAG_CB);  
    pstLag = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_lag_cb
   功能描述  : 释放链路聚合配置所有的内存空间
   输入参数  : 无
   输出参数  : 无
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年11月6日
    作    者   : zhangzhm
    修改内容   : 新生成函数

*****************************************************************************/

NBB_VOID spm_free_all_lag_cb(NBB_CXT_T NBB_CXT)
{
	SPM_LAG_CB *pstLag = NULL;
	
    for (pstLag = (SPM_LAG_CB*) AVLL_FIRST(SHARED.lag_tree);
         pstLag != NULL;
         pstLag = (SPM_LAG_CB*) AVLL_FIRST(SHARED.lag_tree))
    {
    	
		AVLL_DELETE(SHARED.lag_tree, pstLag->spm_lag_node);     	
    	spm_free_lag_cb(pstLag);
    }
}


