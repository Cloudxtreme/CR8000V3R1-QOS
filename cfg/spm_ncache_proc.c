/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

 ******************************************************************************
  文 件 名   : spm_ncache_proc.c
  版 本 号   : 初稿
  作    者   : xiaoxiang
  生成日期   : 2013年4月15日
  最近修改   :
  功能描述   : IPv6 邻居缓存配置处理
  函数列表   :
              spm_alloc_arp_cb
              spm_free_arp_cb
              spm_init_arp_tree
              spm_rcv_dci_set_arp
  修改历史   :
  1.日    期   : 2013年4月15日
    作    者   : xiaoxiang
    修改内容   : 创建文件

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

#ifdef NCACHE_CFG

extern unsigned char ncache_cfg_print_setting;


/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_ncache
 功能描述  : 处理IPv6 邻居缓存表的IPS消息
 输入参数  : ATG_DCI_SET_IPV6_CACHE *pstSetNcache  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_rcv_dci_set_ncache(ATG_DCI_SET_IPV6_CACHE *pstSetNcache NBB_CCXT_T NBB_CXT)
{
    NBB_CHAR ucMessage[SPM_MSG_INFO_LEN];
    NBB_BYTE ucMessageLen = 0;
    
    NBB_INT ret = SUCCESS;
    NBB_BYTE ucC3Unit = 0;
    SPM_NCACHE_CB *pstNcache = NULL;
    SPM_LOGICAL_PORT_CB *pstLogicalPort  = NULL;
    ATG_DCI_IPV6_CACHE_BASIC_DATA *pstBasicData = NULL;
    NCACHE_T 	stNcache;
    
    NBB_BYTE *pucBasicDataStart = NULL;
    
    NBB_ULONG ulOperBasic       = ATG_DCI_OPER_NULL;
    
    SPM_PORT_INFO_CB stPortInfo;
    
    ATG_DCI_IPV6_CACHE_KEY stNcacheKey;

    NBB_BYTE ucIfExist = ATG_DCI_EXIST;

    NBB_TRC_ENTRY("spm_rcv_dci_set_ncache");

    /* 输入参数指针必须有效 */
    NBB_ASSERT(pstSetNcache != NULL);

	if (pstSetNcache == NULL)
	{
        NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:spm_rcv_dci_set_ncache(pstSetNcache==NULL)"));

        OS_PRINTF("***ERROR***:spm_rcv_dci_set_ncache(pstSetNcache==NULL)\n");

        OS_SPRINTF(ucMessage, "***ERROR***:spm_rcv_dci_set_ncache(pstSetNcache==NULL)\n");
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | NCACHE_PD, 0, "ld x x x x d s s", 0, 
                                                                     0, 
                                                                     0,
                                                                     0,
                                                                     0,
                                                                     FUNC_IN_PARAM_IS_NULL, 
                                                                     "FUNC_IN_PARAM_IS_NULL", 
                                                                     ucMessage));
        
        goto EXIT_LABEL;
    }

    OS_MEMSET(&stPortInfo, 0, sizeof(SPM_PORT_INFO_CB));
    OS_MEMSET(&stNcache, 0, sizeof(NCACHE_T));

    //首先将IPS消息的返回值设置为OK，如果有一个子配置失败，则置为FAIL
    pstSetNcache->return_code = ATG_DCI_RC_OK;

    stNcacheKey.index   = pstSetNcache->key.index;
    stNcacheKey.ipv6[0] = pstSetNcache->key.ipv6[0];
    stNcacheKey.ipv6[1] = pstSetNcache->key.ipv6[1];
    stNcacheKey.ipv6[2] = pstSetNcache->key.ipv6[2];
    stNcacheKey.ipv6[3] = pstSetNcache->key.ipv6[3];

    pstNcache = AVLL_FIND(SHARED.ncache_tree, &stNcacheKey);
    
    if (pstNcache == NULL)       /* 如果条目不存在*/
    {
        ucIfExist = ATG_DCI_UNEXIST;
    }    

    pstLogicalPort = AVLL_FIND(SHARED.logical_port_tree, &stNcacheKey.index);

    if ((pstLogicalPort == NULL) || (pstLogicalPort->basic_cfg_cb == NULL))
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表所在的逻辑端口配置不存在", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]));

        OS_PRINTF("***ERROR***:INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表所在的逻辑端口配置不存在!\n", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]);

        OS_SPRINTF(ucMessage, "***ERROR***:INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表所在的逻辑端口配置不存在!\n", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]);
        BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

        NBB_EXCEPTION((PCT_SPM | NCACHE_PD, 0, "ld x x x x d s s", stNcacheKey.index, 
                                                                         stNcacheKey.ipv6[0], 
                                                                         stNcacheKey.ipv6[1],
                                                                         stNcacheKey.ipv6[2],
                                                                         stNcacheKey.ipv6[3],
                                                                         LOGICAL_PORT_NOT_EXIST, 
                                                                         "LOGICAL_PORT_NOT_EXIST", 
                                                                         ucMessage));
                                                                         
        pstSetNcache->return_code = ATG_DCI_RC_NCACHE_NO_PORT;
        pstSetNcache->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
        
        goto EXIT_LABEL;
    }
    else    //如果存在且为L2接口，也没有arp
    {
        if (pstLogicalPort->basic_cfg_cb->port_route_type == ATG_DCI_L2)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表配置存在，但不是L3接口", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]));
            OS_PRINTF("INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表配置存在，但不是L3接口!\n", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]);

            pstSetNcache->return_code = ATG_DCI_RC_NCACHE_NO_PORT;
            pstSetNcache->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;
            
            goto EXIT_LABEL;
        }
    }

    /* 获取子配置操作 */
    ulOperBasic = pstSetNcache->oper_basic;

    //根据端口逻辑配置中的端口子类型，获取端口的port_id
    spm_get_portid_from_logical_port_index(pstSetNcache->key.index, &stPortInfo NBB_CCXT);
    
    /* 删除整个条目 */
    if (pstSetNcache->delete_struct == TRUE)
    {
        NBB_TRC_DETAIL((NBB_FORMAT "  删除INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表配置", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]));
        
        /* 如果条目不存在，不删除 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要删除的配置并不存在"));

            OS_PRINTF("***ERROR***:要删除的INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表配置不存在!\n", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]);

            OS_SPRINTF(ucMessage, "***ERROR***:要删除的INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表配置不存在!\n", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | NCACHE_PD, 0, "ld x x x x d s s", stNcacheKey.index, 
                                                                         stNcacheKey.ipv6[0], 
                                                                         stNcacheKey.ipv6[1],
                                                                         stNcacheKey.ipv6[2],
                                                                         stNcacheKey.ipv6[3],
                                                                         DEL_UNEXIST_CONFIG_ERROR, 
                                                                         "DEL_UNEXIST_CONFIG_ERROR", 
                                                                         ucMessage));
                                                                         
            pstSetNcache->return_code = ATG_DCI_RC_OK;
            
            goto EXIT_LABEL;
        }
        /* 存在，删除 */
        else
        {
            NBB_TRC_FLOW((NBB_FORMAT "  查询到此配置，从tree中删除"));
            spm_dbg_print_ncache_head(&stNcacheKey, SPM_OPER_DEL);
            spm_dbg_record_ncache_head(&stNcacheKey, SPM_OPER_DEL);
            
#ifdef SPU
            ret = SUCCESS;

            if (pstNcache == NULL)
            {
            	goto EXIT_LABEL;
            }
            
            //在本槽位的才删除芯片表相
            for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
            {
        		ret += ApiC3DelNCache(ucC3Unit, pstNcache->ncache_pos_id); 
            }
            
            if (ret != SUCCESS)
            {
                NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:ApiC3DelNCache() IPv6 ret = %d", ret));

                OS_PRINTF("***ERROR***:ApiC3DelNCache() IPv6 ret=%d\n", ret);
                                                                     
                OS_SPRINTF(ucMessage, "***ERROR***:ApiC3DelNCache() IPv6 ret=%d\n", ret);
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                NBB_EXCEPTION((PCT_SPM | NCACHE_PD, 0, "ld x x x x d s s", stNcacheKey.index, 
                                                                     stNcacheKey.ipv6[0], 
                                                                     stNcacheKey.ipv6[1],
                                                                     stNcacheKey.ipv6[2],
                                                                     stNcacheKey.ipv6[3],
                                                                     ret, 
                                                                     "CALL_C3_FUNC_ERROR", 
                                                                     ucMessage));
                                                                     
                pstSetNcache->return_code = ATG_DCI_RC_DEL_FAILED;
                
                goto EXIT_LABEL;
            }
#endif
			if (pstNcache != NULL)
			{
	            AVLL_DELETE(SHARED.ncache_tree, pstNcache->spm_ncache_node);  

	            //释放ARP表节点的内存空间
	            spm_free_ncache_cb(pstNcache NBB_CCXT);
			}
        }
    }
    /* 增加或更新条目 */
    else
    {        
                
        /* 如果条目不存在，新申请内存空间保存数据 */
        if (ucIfExist == ATG_DCI_UNEXIST)
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  增加INDEX=%ld IPV6=%x-%x-%x-%x的 IPv6 邻居缓存表配置", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]));
            spm_dbg_print_ncache_head(&stNcacheKey, SPM_OPER_ADD);
            spm_dbg_record_ncache_head(&stNcacheKey, SPM_OPER_ADD);

            /* 申请一个新条目的内存空间 */
            pstNcache = spm_alloc_ncache_cb(NBB_CXT);

			if (pstNcache != NULL)
			{
	            //插入到tree中
	            OS_MEMCPY(&pstNcache->ncache_key, &stNcacheKey, sizeof(ATG_DCI_IPV6_CACHE_KEY));

	            //coverity[no_effect_test]
	            AVLL_INSERT(SHARED.ncache_tree, pstNcache->spm_ncache_node);
			}
        }
        else
        {
            NBB_TRC_DETAIL((NBB_FORMAT "  更新INDEX=%ld IPV6=%x-%x-%x-%x的 IPv6 邻居缓存表配置", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]));
            spm_dbg_print_ncache_head(&stNcacheKey, SPM_OPER_UPD);
            spm_dbg_record_ncache_head(&stNcacheKey, SPM_OPER_UPD);

        }

		//增加此判断，取消PC-Lint告警
        if (pstNcache == NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "  ***ERROR***:要增加的INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表配置未能成功申请内存!", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]));

            OS_PRINTF("***ERROR***:要增加的INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表配置未能成功申请内存!\n", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]);

            OS_SPRINTF(ucMessage, "***ERROR***:要增加的INDEX=%ld IPV6=%x-%x-%x-%x 的IPv6 邻居缓存表配置未能成功申请内存!\n", stNcacheKey.index, 
                                                                                             stNcacheKey.ipv6[0],
                                                                                             stNcacheKey.ipv6[1],
                                                                                             stNcacheKey.ipv6[2],
                                                                                             stNcacheKey.ipv6[3]);
            BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

            NBB_EXCEPTION((PCT_SPM | NCACHE_PD, 0, "ld x x x x d s s", stNcacheKey.index, 
                                                                         stNcacheKey.ipv6[0], 
                                                                         stNcacheKey.ipv6[1],
                                                                         stNcacheKey.ipv6[2],
                                                                         stNcacheKey.ipv6[3],
                                                                         ALLOC_MEMORY_ERROR, 
                                                                         "ALLOC_MEMORY_ERROR", 
                                                                         ucMessage));
                                                                         
            pstSetNcache->return_code = ATG_DCI_RC_UNSUCCESSFUL;
            
            goto EXIT_LABEL;
        }

        /***************************************************************************/
        /*                        获取配置，逐条处理                               */
        /***************************************************************************/
        /* 基本配置 */
        if (ulOperBasic == ATG_DCI_OPER_ADD)
        {

            /* 计算第一个entry的地址。*/
            pucBasicDataStart = (NBB_BYTE *) NTL_OFFLEN_GET_POINTER(pstSetNcache, &pstSetNcache->basic_data);    

            /* 如果指针为NULL，无数据 */
            if (pucBasicDataStart == NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "  NCACHE basic_data is NULL."));   
                NBB_EXCEPTION((PCT_SPM | 7, 0, "lx", SHARED.spm_index));
            }
            else
            {
                pstBasicData = (ATG_DCI_IPV6_CACHE_BASIC_DATA *)pucBasicDataStart;

                if (ncache_cfg_print_setting == SPM_PRINT_CFG)
                {
                    printf("  1）基本配置\n");
                    spm_dbg_print_ncache_basic_cfg(pstBasicData);
                }

                OS_SPRINTF(ucMessage,"  1）基本配置\n");
                BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);
                
                //spm_dbg_record_ncache_basic_cfg(pstBasicData);

                //打印配置
                NBB_TRC_DETAIL((NBB_FORMAT "  目的MAC = %x-%x-%x-%x-%x-%x", pstBasicData->des_mac[0],
                                                                          pstBasicData->des_mac[1],
                                                                          pstBasicData->des_mac[2],
                                                                          pstBasicData->des_mac[3],
                                                                          pstBasicData->des_mac[4],
                                                                          pstBasicData->des_mac[5]));
                //NBB_TRC_DETAIL((NBB_FORMAT "  邻居缓存条目index = %x", pstBasicData->neighbor_index));
#ifdef SPU
        		ret = ApiC3InitNCache(&stNcache);

        		if (ret != SUCCESS)
        		{
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3InitNCache IPv6 ret=%d", ret));

        		    OS_PRINTF("***ERROR***:ApiC3InitNCache() IPv6 ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:ApiC3InitNCache() IPv6 ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | NCACHE_PD, 0, "ld x x x x d s s", stNcacheKey.index, 
                                                                         stNcacheKey.ipv6[0], 
                                                                         stNcacheKey.ipv6[1],
                                                                         stNcacheKey.ipv6[2],
                                                                         stNcacheKey.ipv6[3],
                                                                         ret, 
                                                                         "CALL_C3_FUNC_ERROR", 
                                                                         ucMessage));
                                                                         
        		    //pstSetNcache->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		    pstSetNcache->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

        		    goto EXIT_LABEL;
        		}
#endif

                //如果是虚拟以太网口
        		if ((pstLogicalPort->basic_cfg_cb != NULL) 
        		 && (pstLogicalPort->basic_cfg_cb->port_sub_type == ATG_DCI_VE_PORT))
        		{
        		    stNcache.mVeFlag = 1;
        		}
        		
        		stNcache.mIpv6[0] = stNcacheKey.ipv6[0];
        		stNcache.mIpv6[1] = stNcacheKey.ipv6[1];
        		stNcache.mIpv6[2] = stNcacheKey.ipv6[2];
        		stNcache.mIpv6[3] = stNcacheKey.ipv6[3];
        		stNcache.mCardPort = stPortInfo.port_id;
        		stNcache.mSlot = stPortInfo.slot_id;

        		//if (pstLogicalPort->logic_l3_cfg_cb != NULL)
        		//{
        		//    stNcache.mVid = pstLogicalPort->logic_l3_cfg_cb->svlan;  //先不考虑两层VLAN的情况
        		//}
        		
                if( NULL == pstLogicalPort->basic_cfg_cb)
                {
                    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***:PORT_INDEX=%ld 的基本配置未下，IPv6邻居缓存获取CONTROL VID失败",
                            stNcacheKey.index));

                    OS_PRINTF("***ERROR***:PORT_INDEX=%ld 的基本配置未下，IPv6邻居缓存获取CONTROL VID失败\n",
                            stNcacheKey.index);

                    OS_SPRINTF(ucMessage,
                        "***ERROR***:PORT_INDEX=%ld 的基本配置未下，IPv6邻居缓存获取CONTROL VID失败\n",
                            stNcacheKey.index);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | NCACHE_PD, 0, "ld x x x x d s s", stNcacheKey.index, 
                                                                         stNcacheKey.ipv6[0], 
                                                                         stNcacheKey.ipv6[1],
                                                                         stNcacheKey.ipv6[2],
                                                                         stNcacheKey.ipv6[3],
                                                                         BASIC_CONFIG_NOT_EXIST, 
                                                                         "BASIC_CONFIG_NOT_EXIST", 
                                                                         ucMessage));
				}
		        else
		      	{
					stNcache.mVid= pstLogicalPort->basic_cfg_cb->control_vid;		      	
		      	}

        		stNcache.sVid = pstBasicData->svlan_id;
        		stNcache.cVid = pstBasicData->cvlan_id;
                
        		OS_MEMCPY(stNcache.dmac, pstBasicData->des_mac, ATG_DCI_MAC_LEN);

#ifdef SPU 		
                ret = SUCCESS;
                
                //在本槽位的才建立
                for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
                {
        			ret += ApiC3AddNCache(ucC3Unit, &stNcache);
                }

                /*如果存在，则先删再添加*/
    			if ((ret == API_C3_E_EXISTS) || (ret == API_C3_E_EXISTS + API_C3_E_EXISTS))
    			{
    				//ret = ApiC3DelNCache(stPortInfo.unit_id, pstNcache->ncache_pos_id);
    				//NBB_TRC_DETAIL((NBB_FORMAT "  >>>>  ApiC3AddNCache: IPv6 ret=%d", ret));

    				//ret += ApiC3AddNCache(stPortInfo.unit_id, &stNcache);
    				//NBB_TRC_DETAIL((NBB_FORMAT "  >>>>  ApiC3AddNCache: IPv6 ret=%d", ret));

    				ret = SUCCESS;

    				for (ucC3Unit=0; ucC3Unit<SHARED.c3_num; ucC3Unit++)
    				{
    				       stNcache.posId = pstNcache->ncache_pos_id;
        				ret += ApiC3ModNCache(ucC3Unit, &stNcache);
        				NBB_TRC_DETAIL((NBB_FORMAT "  >>>>  ApiC3ModNCache: IPv6 ret=%d", ret));
    				}
    			}

#endif                
                if (ret == SUCCESS)
                {
                    /* 保存数据 */
                    if (pstNcache->basic_cfg_cb == NULL)
                    {
                        pstNcache->basic_cfg_cb = (ATG_DCI_IPV6_CACHE_BASIC_DATA *)NBB_MM_ALLOC(sizeof(ATG_DCI_IPV6_CACHE_BASIC_DATA),
                                                                NBB_NORETRY_ACT,
                                                                MEM_SPM_NCACHE_BASIC_CB);
                    }  

                    pstNcache->ncache_pos_id = stNcache.posId;
                    
                    OS_MEMCPY(pstNcache->basic_cfg_cb, pstBasicData, sizeof(ATG_DCI_IPV6_CACHE_BASIC_DATA));
                }
                else
                {
                	//coverity[dead_error_begin]
        		    NBB_TRC_DETAIL((NBB_FORMAT "  ***ERROR***: ApiC3AddNCache/ApiC3ModNCache IPv6 ret=%d", ret));

        		    OS_PRINTF("***ERROR***:ApiC3AddNCache/ApiC3ModNCache() IPv6 ret=%d\n", ret);

        		    OS_SPRINTF(ucMessage, "***ERROR***:ApiC3AddNCache/ApiC3ModNCache() IPv6 ret=%d\n", ret);
                    BMU_SLOG(BMU_INFO, SPM_L2_LOG_DIR, ucMessage);

                    NBB_EXCEPTION((PCT_SPM | NCACHE_PD, 0, "ld x x x x d s s", stNcacheKey.index, 
                                                                         stNcacheKey.ipv6[0], 
                                                                         stNcacheKey.ipv6[1],
                                                                         stNcacheKey.ipv6[2],
                                                                         stNcacheKey.ipv6[3],
                                                                         ret, 
                                                                         "CALL_C3_FUNC_ERROR", 
                                                                         ucMessage));
                                                                         
        		    //pstSetNcache->return_code = ATG_DCI_RC_UNSUCCESSFUL;
        		    pstSetNcache->basic_return_code = ATG_DCI_RC_UNSUCCESSFUL;

        		    goto EXIT_LABEL;
        		}
            }                 
        }
        else if (ulOperBasic == ATG_DCI_OPER_DEL)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 1, "lx", SHARED.spm_index));
        }
        else if (ulOperBasic == ATG_DCI_OPER_UPDATE)
        {
            NBB_EXCEPTION((PCT_SPM | 7, 2, "lx", SHARED.spm_index));
        }
    
    }

    //spm_snd_dci_ips((NBB_VOID*)pstSetNcache, &(pstSetNcache->ips_hdr) NBB_CCXT);

    EXIT_LABEL : NBB_TRC_EXIT();

    return;

}

/*****************************************************************************
 函 数 名  : spm_init_ncache_tree
 功能描述  : 初始化存储IPv6 邻居缓存配置的树
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_init_ncache_tree(NBB_CXT_T NBB_CXT)
{
    NBB_BUF_SIZE avll_key_offset;

    NBB_TRC_ENTRY("spm_init_ncache_tree");
    
    avll_key_offset = NBB_OFFSETOF(SPM_NCACHE_CB, ncache_key);/*lint !e413 */

    //NBB_TRC_DETAIL((NBB_FORMAT "Key offset set to %ld", avll_key_offset));

    AVLL_INIT_TREE(SHARED.ncache_tree, spm_ncache_key_compare,
                 (NBB_USHORT) avll_key_offset,
                 (NBB_USHORT) NBB_OFFSETOF(SPM_NCACHE_CB, spm_ncache_node));

    NBB_TRC_EXIT();       
}

/*****************************************************************************
 函 数 名  : spm_alloc_ncache_cb
 功能描述  : 申请IPv6 邻居缓存节点的内存空间
 输入参数  : 无
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2013年4月16日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
SPM_NCACHE_CB* spm_alloc_ncache_cb(NBB_CXT_T NBB_CXT)
{
    SPM_NCACHE_CB *pstNcache = NULL;

    NBB_TRC_ENTRY("spm_alloc_ncache_cb");

    /* 分配一个新的VC表配置条目。*/
    pstNcache = (SPM_NCACHE_CB *)NBB_MM_ALLOC(sizeof(SPM_NCACHE_CB), NBB_NORETRY_ACT, MEM_SPM_NCACHE_CB);
    if (pstNcache == NULL)
    {
        goto EXIT_LABEL;
    }

    /* 初始化VC表配置条目 */
    OS_MEMSET(pstNcache, 0, sizeof(SPM_NCACHE_CB));
    pstNcache->basic_cfg_cb = NULL;

    /* 建立用于该VC表配置条目的句柄，作为异步消息交换的相关器。*/
    pstNcache->spm_ncache_handle = NBB_CREATE_HANDLE(pstNcache, HDL_SPM_NCACHE_CB);

    /* 成功分配一个新的IPv6 邻居缓存配置条目。*/
    NBB_TRC_DETAIL((NBB_FORMAT "SPM_NCACHE_CB allocated at %p with handle %#lx",
                   pstNcache, pstNcache->spm_ncache_handle));   

    /* Initialize the AVLL node. */
    AVLL_INIT_NODE(pstNcache->spm_ncache_node);    

    EXIT_LABEL : NBB_TRC_EXIT();
    
    return(pstNcache);
}

/*****************************************************************************
 函 数 名  : spm_free_arp_cb
 功能描述  : 释放ARP表节点泊内存空间
 输入参数  : SPM_NCACHE_CB *pstNcache NBB_CCXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2012年11月10日
    作    者   : xiaoxiang
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_free_ncache_cb(SPM_NCACHE_CB *pstNcache NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/  
    //NBB_INT i = 0;
    
    NBB_TRC_ENTRY("spm_free_ncache_cb");
    
    /***************************************************************************/
    /* 检查控制块的正确性。                                                    */
    /***************************************************************************/  
    NBB_ASSERT_MEMORY(pstNcache, sizeof(SPM_NCACHE_CB), MEM_SPM_NCACHE_CB); 

    if (pstNcache == NULL)
    {
    	goto EXIT_LABEL;
    }
    
    /***************************************************************************/
    /* 删除单盘信息控制块。                                                    */
    /***************************************************************************/
    //NBB_TRC_FLOW((NBB_FORMAT "Free SPM_NCACHE_CB at %p", pstNcache));  
    NBB_TRC_FLOW((NBB_FORMAT "Free INDEX %d  IPV6 %x-%x-%x-%x", pstNcache->ncache_key.index,
                                                                pstNcache->ncache_key.ipv6[0],
                                                                pstNcache->ncache_key.ipv6[1],
                                                                pstNcache->ncache_key.ipv6[2],
                                                                pstNcache->ncache_key.ipv6[3]));

    /***************************************************************************/
    /* 释放基本配置数据块。                                                */
    /***************************************************************************/
    if (pstNcache->basic_cfg_cb != NULL)
    {
        NBB_MM_FREE(pstNcache->basic_cfg_cb, MEM_SPM_NCACHE_BASIC_CB);  
        pstNcache->basic_cfg_cb = NULL;
    }
    

    /***************************************************************************/
    /* 删除控制块的句柄。                                                      */
    /***************************************************************************/
    NBB_DESTROY_HANDLE(pstNcache->spm_ncache_handle, HDL_SPM_NCACHE_CB);

    /***************************************************************************/
    /* 现在释放单盘信息控制块。                                                */
    /***************************************************************************/
    NBB_MM_FREE(pstNcache, MEM_SPM_NCACHE_CB);  
    pstNcache = NULL;

    EXIT_LABEL: NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
   函 数 名  : spm_free_all_arp_cb
   功能描述  : 释放arp配置所有的内存空间
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

NBB_VOID spm_free_all_ncache_cb(NBB_CXT_T NBB_CXT)
{
	SPM_NCACHE_CB *pstNcache = NULL;
	
    for (pstNcache = (SPM_NCACHE_CB*) AVLL_FIRST(SHARED.ncache_tree);
         pstNcache != NULL;
         pstNcache = (SPM_NCACHE_CB*) AVLL_FIRST(SHARED.ncache_tree))
    {
    	
		AVLL_DELETE(SHARED.ncache_tree, pstNcache->spm_ncache_node);     	
    	spm_free_ncache_cb(pstNcache NBB_CCXT);
    }
}



#endif


