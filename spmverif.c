
/**MOD+***********************************************************************/
/* Module:    spmverif.c                                                     */
/*                                                                           */
/* Purpose:   SPM的内存验证程序。                                            */
/*                                                                           */
/* FiberHome         2003-2012                                               */
/*                                                                           */
/* $Revision: 1.34 $ $Date: 2010/11/22 07:09:09 $               */
/*                                                                           */
/**MOD-***********************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/**PROC+**********************************************************************/
/* Name:      spm_verify_proc                                                */
/*                                                                           */
/* Purpose:   SPM verify procedure.                                          */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    None                                                           */
/*                                                                           */
/* Operation: Verifies all buffers and control blocks.                       */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_verify_proc(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  /***************************************************************************/
  /* Get the lock for the SHARED LOCAL data.                                 */
  /***************************************************************************/
  NBS_GET_SHARED_DATA();

  NBB_TRC_ENTRY("spm_verify_proc");

  /***************************************************************************/
  /* 验证MIB产品支持库。                                                     */
  /***************************************************************************/
  AMBL_VERIFY(&(SHARED.spm_ambl_data));

  /***************************************************************************/
  /* 验证HAF库。                                                             */
  /***************************************************************************/
  AHL_VERIFY(&(SHARED.spm_haf_data));

  /***************************************************************************/
  /* 验证slave join CBs.                                                     */
  /***************************************************************************/
  spm_sbi_join_verify(NBB_CXT);
  
  /***************************************************************************/
  /* 验证所有业务配置相关控制块。                                            */
  /***************************************************************************/
  spm_cfg_cb_verify(NBB_CXT);
  
  /***************************************************************************/
  /* 也检查master joins是在一个有效的状态中。                                */
  /***************************************************************************/
  NBB_ASSERT((SHARED.spm_dci_join_cb.spm_join_status == SPM_MJ_INIT) ||
             (SHARED.spm_dci_join_cb.spm_join_status == SPM_MJ_ACTIVE) ||
             (SHARED.spm_dci_join_cb.spm_join_status == SPM_MJ_ACTIVATING) ||
             (SHARED.spm_dci_join_cb.spm_join_status == SPM_MJ_FOVER) ||
             (SHARED.spm_dci_join_cb.spm_join_status == SPM_MJ_FAILED) ||
             (SHARED.spm_dci_join_cb.spm_join_status == SPM_MJ_DEACTIVATING) ||
             (SHARED.spm_dci_join_cb.spm_join_status == SPM_MJ_UNAVAIL));

  NBB_ASSERT((SHARED.sck_join_cb.join_status == SPM_MJ_INIT) ||
             (SHARED.sck_join_cb.join_status == SPM_MJ_ACTIVE) ||
             (SHARED.sck_join_cb.join_status == SPM_MJ_ACTIVATING) ||
             (SHARED.sck_join_cb.join_status == SPM_MJ_FOVER) ||
             (SHARED.sck_join_cb.join_status == SPM_MJ_FAILED) ||
             (SHARED.sck_join_cb.join_status == SPM_MJ_DEACTIVATING) ||
             (SHARED.sck_join_cb.join_status == SPM_MJ_UNAVAIL));

  NBB_TRC_EXIT();

  /***************************************************************************/
  /* Release the lock for the SHARED LOCAL data.                             */
  /***************************************************************************/
  NBS_RELEASE_SHARED_DATA();

  return;
} /* spm_verify_proc */

/**PROC+**********************************************************************/
/* Name:      spm_sbi_join_verify                                            */
/*                                                                           */
/* Purpose:   验证所有的SBI interface join CBs.                              */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_sbi_join_verify(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  SPM_SJ_JOIN_CB *spm_sj_join_cb;
  SPM_THREAD_SOCKET_CB* thread_socket_cb = NULL;

  NBB_TRC_ENTRY("spm_sbi_join_verify");

  /***************************************************************************/
  /* 遍历SBI joins的树，验证每一个。                                         */
  /***************************************************************************/
  for (spm_sj_join_cb = (SPM_SJ_JOIN_CB*)
                        NBB_NEXT_IN_LIST(SHARED.spm_sbi_join_list);
       spm_sj_join_cb != NULL;
       spm_sj_join_cb = (SPM_SJ_JOIN_CB*) NBB_NEXT_IN_LIST(spm_sj_join_cb->spm_sj_join_list_entry))
  {
    NBB_TRC_FLOW((NBB_FORMAT "Verify join %p", spm_sj_join_cb));

    NBB_VERIFY_MEMORY(spm_sj_join_cb, sizeof(SPM_SJ_JOIN_CB),
                      MEM_SPM_SBI_JOIN_CB);

    /*************************************************************************/
    /* 检查join状态是否为一个允许的值。                                      */
    /*************************************************************************/
    NBB_ASSERT((spm_sj_join_cb->spm_join_status == SPM_SJ_INIT) ||
               (spm_sj_join_cb->spm_join_status == SPM_SJ_ACTIVE) ||
               (spm_sj_join_cb->spm_join_status == SPM_SJ_ACTIVATING) ||
               (spm_sj_join_cb->spm_join_status == SPM_SJ_FOVER) ||
               (spm_sj_join_cb->spm_join_status == SPM_SJ_FAILED) ||
               (spm_sj_join_cb->spm_join_status == SPM_SJ_DEACTIVATING));

  }

  /***************************************************************************/
  /* 验证sck_cb。                                                            */
  /***************************************************************************/
  for (thread_socket_cb = (SPM_THREAD_SOCKET_CB*)
                        NBB_NEXT_IN_LIST(SHARED.sck_cb.thread_list);
       thread_socket_cb != NULL;
       thread_socket_cb = (SPM_THREAD_SOCKET_CB*) NBB_NEXT_IN_LIST(thread_socket_cb->lqe_list))
  {
    NBB_TRC_FLOW((NBB_FORMAT "Verify thread_socket_cb %p", thread_socket_cb));

    NBB_VERIFY_MEMORY(thread_socket_cb, sizeof(SPM_THREAD_SOCKET_CB),
                      MEM_SPM_THREAD_SOCKET_CB);

  }

  NBB_TRC_EXIT();

  return;
} /* spm_sbi_join_verify */


/**PROC+**********************************************************************/
/* Name:      spm_cfg_cb_verify                                              */
/*                                                                           */
/* Purpose:   验证所有保存的业务配置相关的控制块.                            */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/

    NBB_TRC_ENTRY("spm_cfg_cb_verify");

    /*************************************************************************/
    /* 遍历接口物理配置的树逐一进行验证。                                    */
    /*************************************************************************/
    spm_cfg_phy_port_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历端口逻辑配置的树逐一进行验证。                                    */
    /*************************************************************************/
    spm_cfg_log_port_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历链路聚合配置的树逐一进行验证。                                    */
    /*************************************************************************/
    spm_cfg_lag_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历VC表配置的树逐一进行验证。                                        */
    /*************************************************************************/
    spm_cfg_vc_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历VPWS配置的树逐一进行验证。                                        */
    /*************************************************************************/
    spm_cfg_vpws_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历VPLS配置的树逐一进行验证。                                        */
    /*************************************************************************/
    spm_cfg_vpls_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历ARP表配置的树逐一进行验证。                                       */
    /*************************************************************************/
    spm_cfg_arp_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历IPv6 邻居缓存配置的树逐一进行验证。                               */
    /*************************************************************************/
    spm_cfg_ncache_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历端口镜像配置的树逐一进行验证。                                    */
    /*************************************************************************/
    spm_cfg_port_mirror_cb_verify(NBB_CXT);
    
    /*************************************************************************/
    /* 遍历VPLS组播配置的树逐一进行验证。                                    */
    /*************************************************************************/
    spm_cfg_vpls_mc_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历VPLS单播配置的树逐一进行验证。                                    */
    /*************************************************************************/
    spm_cfg_vpls_uc_cb_verify(NBB_CXT);

    /*************************************************************************/
    /* 遍历SWITCH VC表配置的树逐一进行验证。                                 */
    /*************************************************************************/
    spm_cfg_switch_vc_cb_verify(NBB_CXT);

	/*************************************************************************/
    /* 遍历Bypass保护配置的树逐一进行验证。                                 */
    /*************************************************************************/
	spm_cfg_bypass_cb_verify(NBB_CXT);

	/*************************************************************************/
    /* 遍历MSP配置的树逐一进行验证。                                 */
    /*************************************************************************/
	spm_cfg_msp_cb_verify(NBB_CXT);
    
    /*************************************************************************/
    /* 遍历cir_emu配置的树逐一进行验证。                                 */
    /*************************************************************************/
        spm_cfg_cir_emu_cb_verify(NBB_CXT);
    
    /*************************************************************************/
    /* 遍历QOS配置树。                                    */
    /*************************************************************************/
    spm_qos_cfg_cb_verfify(NBB_CXT);

    spm_bfd_cfg_cb_verfify(NBB_CXT);
	
    spm_l3_verify_mem(NBB_CXT);
     /*************************************************************************/
    /* 遍历流采样配置树并进行逐一验证。                                    */
    /*************************************************************************/
    spm_netflow_cfg_cb_verfify(NBB_CXT);
     
	spm_cfg_oam_cb_verify(NBB_CXT);

	spm_associate_if_cb_verify(NBB_CXT);

	/*************************************************************************/
    /* 遍历VPLS_L3组播配置的树逐一进行验证。                                 */
    /*************************************************************************/
    spm_cfg_vpls_l3_mc_cb_verify(NBB_CXT);

    NBB_TRC_EXIT();

    return;
} /* spm_cfg_cb_verify */


/**PROC+**********************************************************************/
/* Name:      spm_cfg_phy_port_cb_verify                                     */
/*                                                                           */
/* Purpose:   遍历接口物理配置的树逐一进行验证。                             */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_phy_port_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_PHYSICAL_PORT_CB *phy_port_cb = NULL;
    ATG_DCI_PHY_PORT_BASIC_DATA *basic_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_PHY_DATA *phy_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_POS_LINK_DATA *pos_link_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_SDH_SOVHD_DATA *sdh_sovhd_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_CES_DATA *ces_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_CEP_OVHD_DATA *cep_ovhd_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_EFM_OAM_DATA *efm_oam_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_TIME_SYNC_DATA *time_sync_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_MSG_SUPPR_DATA *msg_suppr_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_FLOW_SHAPE *flow_shape_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_SCHEDUL_POLICY *schedule_policy_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_CONGEST_POLICY *congest_policy_cfg_cb  = NULL;
    ATG_DCI_PHY_PORT_IPV4NF_DATA *ipv4_nf_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_IPNF_SAMPLER *ipv4_sample_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_IPV6NF_DATA *ipv6_nf_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_COMMON_DATA *common_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_SDH_TOVHD_DATA *sdh_tovhd_cfg_cb = NULL;
    ATG_DCI_PHY_PORT_ALS_DATA *als_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_phy_port_cb_verify");

    /*************************************************************************/
    /* 遍历接口物理配置的树逐一进行验证。                                    */
    /*************************************************************************/
    for (phy_port_cb = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree);
         phy_port_cb != NULL;
         phy_port_cb = (SPM_PHYSICAL_PORT_CB*) AVLL_NEXT(SHARED.physical_port_tree,
                       phy_port_cb->spm_physical_port_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify phy_port_cb %p", phy_port_cb));
        NBB_VERIFY_MEMORY(phy_port_cb, sizeof(SPM_PHYSICAL_PORT_CB), MEM_SPM_PHYSICAL_PORT_CB);

        /*********************************************************************/
        /* 验证接口物理配置控制块中的所有子配置。                            */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基础配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = phy_port_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_PHY_PORT_BASIC_DATA),
                              MEM_SPM_PHY_PORT_BASIC_CB);
        }

        /*************************************************************************/
        /* 对以太网接口物理配置块进行验证。                                      */
        /*************************************************************************/
        phy_cfg_cb = phy_port_cb->phy_cfg_cb;
        if(phy_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify phy_cfg_cb cb %p", phy_cfg_cb));
            NBB_VERIFY_MEMORY(phy_cfg_cb, sizeof(ATG_DCI_PHY_PORT_PHY_DATA),
                              MEM_SPM_PHY_PORT_PHY_CB);
        }

        /*************************************************************************/
        /* 对POS接口链路层配置块进行验证。                                       */
        /*************************************************************************/
        pos_link_cfg_cb = phy_port_cb->pos_link_cfg_cb;
        if(pos_link_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify pos_link_cfg_cb cb %p", pos_link_cfg_cb));
            NBB_VERIFY_MEMORY(pos_link_cfg_cb, sizeof(ATG_DCI_PHY_PORT_POS_LINK_DATA),
                              MEM_SPM_PHY_PORT_POS_CB);
        }

        /*************************************************************************/
        /* 对SDH段开销配置块进行验证。                                       */
        /*************************************************************************/
        sdh_sovhd_cfg_cb = phy_port_cb->stm1_sovhd_cfg_cb;
        if(sdh_sovhd_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify sdh_sovhd_cfg_cb cb %p", sdh_sovhd_cfg_cb));
            NBB_VERIFY_MEMORY(sdh_sovhd_cfg_cb, sizeof(ATG_DCI_PHY_PORT_SDH_SOVHD_DATA),
                              MEM_SPM_PHY_PORT_SDH_SOVHD_CB);
        }

        /*************************************************************************/
        /* 对CES接口配置块进行验证。                                             */
        /*************************************************************************/
        ces_cfg_cb = phy_port_cb->ces_cfg_cb;
        if(ces_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ces_cfg_cb cb %p", ces_cfg_cb));
            NBB_VERIFY_MEMORY(ces_cfg_cb, sizeof(ATG_DCI_PHY_PORT_CES_DATA),
                              MEM_SPM_PHY_PORT_CES_CB);
        }

        /*************************************************************************/
        /* 对CEP接口开销配置块进行验证。                                         */
        /*************************************************************************/
        cep_ovhd_cfg_cb = phy_port_cb->cep_ovhd_cfg_cb;
        if(cep_ovhd_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify cep_ovhd_cfg_cb cb %p", cep_ovhd_cfg_cb));
            NBB_VERIFY_MEMORY(cep_ovhd_cfg_cb, sizeof(ATG_DCI_PHY_PORT_CEP_OVHD_DATA),
                              MEM_SPM_PHY_PORT_CEP_CB);
        }

        /*************************************************************************/
        /* 对以太网EFM OAM配置块进行验证。                                       */
        /*************************************************************************/
        efm_oam_cfg_cb = phy_port_cb->efm_oam_cfg_cb;
        if(efm_oam_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify efm_oam_cfg_cb cb %p", efm_oam_cfg_cb));
            NBB_VERIFY_MEMORY(efm_oam_cfg_cb, sizeof(ATG_DCI_PHY_PORT_EFM_OAM_DATA),
                              MEM_SPM_PHY_PORT_EFM_OAM_CB);
        }

        /*************************************************************************/
        /* 对时间同步端口配置块进行验证。                                        */
        /*************************************************************************/
        time_sync_cfg_cb = phy_port_cb->time_sync_cfg_cb;
        if(time_sync_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify time_sync_cfg_cb cb %p", time_sync_cfg_cb));
            NBB_VERIFY_MEMORY(time_sync_cfg_cb, sizeof(ATG_DCI_PHY_PORT_TIME_SYNC_DATA),
                              MEM_SPM_PHY_PORT_TIME_SYNC_CB);
        }

        /*************************************************************************/
        /* 对以太网接口报文抑制配置块进行验证。                                        */
        /*************************************************************************/
        msg_suppr_cfg_cb = phy_port_cb->msg_suppr_cfg_cb;
        if(msg_suppr_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify msg_suppr_cfg_cb cb %p", msg_suppr_cfg_cb));
            NBB_VERIFY_MEMORY(msg_suppr_cfg_cb, sizeof(ATG_DCI_PHY_PORT_MSG_SUPPR_DATA),
                              MEM_SPM_PHY_PORT_MSG_SUPPR_CB);
        }

        /*************************************************************************/
        /* 对以太网接口流量整形配置块进行验证。                                        */
        /*************************************************************************/
        flow_shape_cfg_cb = phy_port_cb->flow_shape_cfg_cb;
        if(flow_shape_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify flow_shape_cfg_cb cb %p", flow_shape_cfg_cb));
            NBB_VERIFY_MEMORY(flow_shape_cfg_cb, sizeof(ATG_DCI_PHY_PORT_FLOW_SHAPE),
                              MEM_SPM_PHY_PORT_FLOW_SHAPE_CB);
        }

        /*************************************************************************/
        /* 对以太网出口队列调度策略配置块进行验证。                              */
        /*************************************************************************/
        schedule_policy_cfg_cb = phy_port_cb->schedule_policy_cfg_cb;
        if(schedule_policy_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify schedule_policy_cfg_cb cb %p", schedule_policy_cfg_cb));
            NBB_VERIFY_MEMORY(schedule_policy_cfg_cb, sizeof(ATG_DCI_PHY_PORT_SCHEDUL_POLICY),
                              MEM_SPM_PHY_PORT_SCHEDUL_POLICY_CB);
        }

        /*************************************************************************/
        /* 对以太网出口队列拥塞策略配置块进行验证。                              */
        /*************************************************************************/
        congest_policy_cfg_cb = phy_port_cb->congest_policy_cfg_cb;
        if(congest_policy_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify congest_policy_cfg_cb cb %p", congest_policy_cfg_cb));
            NBB_VERIFY_MEMORY(congest_policy_cfg_cb, sizeof(ATG_DCI_PHY_PORT_CONGEST_POLICY),
                              MEM_SPM_PHY_PORT_CONGEST_POLICY_CB);
        }
        
        /*************************************************************************/
        /* 对IPV4流采样使能配置IPV4 NetFlow配置块进行验证。                      */
        /*************************************************************************/
        ipv4_nf_cfg_cb = phy_port_cb->ipv4_nf_cfg_cb;
        if(ipv4_nf_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_nf_cfg_cb cb %p", ipv4_nf_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_nf_cfg_cb, sizeof(ATG_DCI_PHY_PORT_IPV4NF_DATA),
                              MEM_SPM_PHY_PORT_IPV4_NF_CB);
        }

        /*************************************************************************/
        /* 对IPV4流采样镜像 IPV4 NetFlow sampler配置块进行验证。                 */
        /*************************************************************************/
        ipv4_sample_cfg_cb = phy_port_cb->ip_sample_cfg_cb;
        if(ipv4_sample_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_sample_cfg_cb cb %p", ipv4_sample_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_sample_cfg_cb, sizeof(ATG_DCI_PHY_PORT_IPNF_SAMPLER),
                              MEM_SPM_PHY_PORT_IPV4_SAMPLE_CB);
        }

        /*************************************************************************/
        /* 对IPV6流采样使能配置IPV4 NetFlow配置块进行验证。                      */
        /*************************************************************************/
        ipv6_nf_cfg_cb = phy_port_cb->ipv6_nf_cfg_cb;
        if(ipv6_nf_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_nf_cfg_cb cb %p", ipv6_nf_cfg_cb));
            NBB_VERIFY_MEMORY(ipv6_nf_cfg_cb, sizeof(ATG_DCI_PHY_PORT_IPV6NF_DATA),
                              MEM_SPM_PHY_PORT_IPV6_NF_CB);
        }

        /*************************************************************************/
        /* 对接口通用配置块进行验证。                 */
        /*************************************************************************/
        common_cfg_cb = phy_port_cb->common_cfg_cb;
        if(common_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify common_cfg_cb cb %p", common_cfg_cb));
            NBB_VERIFY_MEMORY(common_cfg_cb, sizeof(ATG_DCI_PHY_PORT_COMMON_DATA),
                              MEM_SPM_PHY_PORT_COMMON_CB);
        }

        /*************************************************************************/
        /* 对SDH通道开销配置块进行验证。                                       */
        /*************************************************************************/
        sdh_tovhd_cfg_cb = phy_port_cb->stm1_tovhd_cfg_cb;
        if(sdh_tovhd_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify sdh_tovhd_cfg_cb cb %p", sdh_tovhd_cfg_cb));
            NBB_VERIFY_MEMORY(sdh_tovhd_cfg_cb, sizeof(ATG_DCI_PHY_PORT_SDH_TOVHD_DATA),
                              MEM_SPM_PHY_PORT_SDH_TOVHD_CB);
        }

        /*************************************************************************/
        /* 对ALS通道开销配置块进行验证。                                       */
        /*************************************************************************/
        als_cfg_cb = phy_port_cb->als_cfg_cb;
        if(als_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify als_cfg_cb cb %p", als_cfg_cb));
            NBB_VERIFY_MEMORY(als_cfg_cb, sizeof(ATG_DCI_PHY_PORT_ALS_DATA),
                              MEM_SPM_PHY_PORT_ALS_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:      spm_cfg_log_port_cb_verify                                     */
/*                                                                           */
/* Purpose:   遍历端口逻辑配置的树逐一进行验证.                              */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_log_port_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_LOGICAL_PORT_CB *log_port_cb = NULL;
    SPM_TERMINAL_IF_CB *terminal_if_cb = NULL;
    ATG_DCI_LOG_PORT_BASIC_DATA *basic_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_PHY_DATA *phy_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_LOGIC_L3_DATA *logic_l3_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_LOGIC_L2_DATA *logic_l2_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_FLOW_DATA *flow_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_CES_DATA *ces_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_IPV4_DATA *ipv4_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_IPV6_DATA *ipv6_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_MC_IPV4_DATA *mc_ipv4_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_VIP_VMAC_DATA *vip_vmac_cfg_cb = NULL;
    ATG_DCI_LOG_VRRP_IPV6_MAC_L3 *vipv6_vmac_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_VE_MAC_DATA *ve_mac_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_DIFF_SERV_DATA *diff_serv_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_FLOW_DIFF_SERV *flow_diff_serv_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_INCLASSIFY_QOS *inclassify_qos_cfg_cb = NULL;
    ATG_DCI_LOG_UP_USER_QOS_POLICY *up_user_cfg_cb = NULL;
    ATG_DCI_LOG_UP_USER_GROUP_QOS *up_user_group_cfg_cb = NULL;
    ATG_DCI_LOG_DOWN_USER_QUEUE_QOS *down_user_cfg_cb = NULL;
    ATG_DCI_LOG_DOWN_USER_GROUP_QOS *down_user_group_cfg_cb = NULL;
    ATG_DCI_LOG_FLOW_UP_USER_QOS *flow_up_user_cfg_cb = NULL;
    ATG_DCI_LOG_PORT_TRAFFIC_FILTER *traffic_filter_cfg_cb = NULL;
	ATG_DCI_LOG_PORT_MC_IPV6_DATA *mc_ipv6_cfg_cb = NULL;
	ATG_DCI_LOG_PORT_DS_L2_DATA *ds_l2_cfg_cb = NULL;
    SPM_DIFF_SERV_CB *diff_serv_cb = NULL;    
    SPM_FLOW_DIFF_SERV_CB *flow_diff_serv_cb = NULL;
    SPM_INCLASSIFY_QOS_CB *inclassify_qos_cb = NULL;
    SPM_TRAFFIC_FILTER_CB *traffic_filter_cb = NULL;       
	
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY("spm_cfg_log_port_cb_verify");

    /*************************************************************************/
    /* 遍历端口逻辑配置的树逐一进行验证。                                    */
    /*************************************************************************/
    for (log_port_cb = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree);
         log_port_cb != NULL;
         log_port_cb = (SPM_LOGICAL_PORT_CB*) AVLL_NEXT(SHARED.logical_port_tree,
                       log_port_cb->spm_logical_port_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify log_port_cb %p", log_port_cb));
        NBB_VERIFY_MEMORY(log_port_cb, sizeof(SPM_LOGICAL_PORT_CB), MEM_SPM_LOGICAL_PORT_CB);

        /*********************************************************************/
        /* 验证端口逻辑配置控制块中的所有子配置。                            */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基础配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = log_port_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_LOG_PORT_BASIC_DATA),
                              MEM_SPM_LOG_PORT_BASIC_CB);
        }

        /*************************************************************************/
        /* 对物理配置块进行验证。                                                */
        /*************************************************************************/
        phy_cfg_cb = log_port_cb->phy_cfg_cb;
        if(phy_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify phy_cfg_cb cb %p", phy_cfg_cb));
            NBB_VERIFY_MEMORY(phy_cfg_cb, sizeof(ATG_DCI_LOG_PORT_PHY_DATA),
                              MEM_SPM_LOG_PORT_PHY_CB);
        }

        /*************************************************************************/
        /* 对逻辑配置L3配置块进行验证。                                          */
        /*************************************************************************/
        logic_l3_cfg_cb = log_port_cb->logic_l3_cfg_cb;
        if(logic_l3_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify logic_l3_cfg_cb cb %p", logic_l3_cfg_cb));
            NBB_VERIFY_MEMORY(logic_l3_cfg_cb, sizeof(ATG_DCI_LOG_PORT_LOGIC_L3_DATA),
                              MEM_SPM_LOG_PORT_L3_CB);
        }

        /*************************************************************************/
        /* 对逻辑配置L2配置块进行验证。                                          */
        /*************************************************************************/
        logic_l2_cfg_cb = log_port_cb->logic_l2_cfg_cb;
        if(logic_l2_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify logic_l2_cfg_cb cb %p", logic_l2_cfg_cb));
            NBB_VERIFY_MEMORY(logic_l2_cfg_cb, sizeof(ATG_DCI_LOG_PORT_LOGIC_L2_DATA),
                              MEM_SPM_LOG_PORT_L2_CB);
        }

        /*************************************************************************/
        /* 对流相关配置L2配置块进行验证。                                        */
        /*************************************************************************/
/*
        for (i = 0; i < ATG_DCI_LOG_PORT_FLOW_NUM; i++)
        {
            flow_cfg_cb = log_port_cb->flow_cfg_cb[i];
            if(flow_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify flow_cfg_cb cb %p", flow_cfg_cb));
                NBB_VERIFY_MEMORY(flow_cfg_cb, sizeof(ATG_DCI_LOG_PORT_FLOW_DATA),
                                  MEM_SPM_LOG_PORT_FLOW_CB);
            }
        }
*/
        /*************************************************************************/
        /* 对逻辑配置CES配置块进行验证。                                         */
        /*************************************************************************/
        ces_cfg_cb = log_port_cb->ces_cfg_cb;
        if(ces_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ces_cfg_cb cb %p", ces_cfg_cb));
            NBB_VERIFY_MEMORY(ces_cfg_cb, sizeof(ATG_DCI_LOG_PORT_CES_DATA),
                              MEM_SPM_LOG_PORT_CES_CB);
        }

        /*************************************************************************/
        /* 对Ipv4地址配置L3配置块进行验证。                                      */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_LOG_PORT_IPV4_NUM; i++)
        {
            ipv4_cfg_cb = log_port_cb->ipv4_cfg_cb[i];
            if(ipv4_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_cfg_cb cb %p", ipv4_cfg_cb));
                NBB_VERIFY_MEMORY(ipv4_cfg_cb, sizeof(ATG_DCI_LOG_PORT_IPV4_DATA),
                                  MEM_SPM_LOG_PORT_IPV4_CB);
            }
        }

        /*************************************************************************/
        /* 对Ipv6地址配置L3配置块进行验证。                                      */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_LOG_PORT_IPV6_NUM; i++)
        {
            ipv6_cfg_cb = log_port_cb->ipv6_cfg_cb[i];
            if(ipv6_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_cfg_cb cb %p", ipv6_cfg_cb));
                NBB_VERIFY_MEMORY(ipv6_cfg_cb, sizeof(ATG_DCI_LOG_PORT_IPV6_DATA),
                                  MEM_SPM_LOG_PORT_IPV6_CB);
            }
        }

        /*************************************************************************/
        /* 对组播组地址L3配置块进行验证。                                        */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV4_NUM; i++)
        {
            mc_ipv4_cfg_cb = log_port_cb->mc_ipv4_cfg_cb[i];
            if(mc_ipv4_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify mc_ipv4_cfg_cb cb %p", mc_ipv4_cfg_cb));
                NBB_VERIFY_MEMORY(mc_ipv4_cfg_cb, sizeof(ATG_DCI_LOG_PORT_MC_IPV4_DATA),
                                  MEM_SPM_LOG_PORT_MC_IPV4_CB);
            }
        }

        /*************************************************************************/
        /* 对虚拟MAC和IP地址配置L3配置块进行验证。                               */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
        {
            vip_vmac_cfg_cb = log_port_cb->vip_vmac_cfg_cb[i];
            if(vip_vmac_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify vip_vmac_cfg_cb cb %p", vip_vmac_cfg_cb));
                NBB_VERIFY_MEMORY(vip_vmac_cfg_cb, sizeof(ATG_DCI_LOG_PORT_VIP_VMAC_DATA),
                                  MEM_SPM_LOG_PORT_VIP_VMAC_CB);
            }
        }

        /*************************************************************************/
        /* 对虚拟MAC和IP地址配置L3配置块进行验证。                               */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_LOG_PORT_VIP_VMAC_NUM; i++)
        {
            vipv6_vmac_cfg_cb = log_port_cb->vipv6_vmac_cfg_cb[i];
            if(vipv6_vmac_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify vipv6_vmac_cfg_cb cb %p", vipv6_vmac_cfg_cb));
                NBB_VERIFY_MEMORY(vipv6_vmac_cfg_cb, sizeof(ATG_DCI_LOG_VRRP_IPV6_MAC_L3),
                                  MEM_SPM_LOG_PORT_VIPV6_VMAC_CB);
            }
        }

        /*************************************************************************/
        /* 对VE MAC地址配置块进行验证。                                          */
        /*************************************************************************/
        ve_mac_cfg_cb = log_port_cb->ve_mac_cfg_cb;
        if(ve_mac_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ve_mac_cfg_cb cb %p", ve_mac_cfg_cb));
            NBB_VERIFY_MEMORY(ve_mac_cfg_cb, sizeof(ATG_DCI_LOG_PORT_VE_MAC_DATA),
                              MEM_SPM_LOG_PORT_VE_MAC_CB);
        }
        
        /*************************************************************************/
        /* 对Diff-Serv配置块进行验证。                                          */
        /*************************************************************************/
/*        diff_serv_cfg_cb = log_port_cb->diff_serv_cfg_cb;
        if(diff_serv_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify diff_serv_cfg_cb cb %p", diff_serv_cfg_cb));
            NBB_VERIFY_MEMORY(diff_serv_cfg_cb, sizeof(ATG_DCI_LOG_PORT_DIFF_SERV_DATA),
                              MEM_SPM_LOG_PORT_DIFF_SERV_CB);
        }
*/
        /*************************************************************************/
        /* 对流相关Diff-Serv配置L2块进行验证。                                   */
        /*************************************************************************/
/*
        for (i = 0; i < ATG_DCI_LOG_FLOW_DIFF_SERV_NUM; i++)
        {
            flow_diff_serv_cfg_cb = log_port_cb->flow_diff_serv_cfg_cb[i];
            if(flow_diff_serv_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify flow_diff_serv_cfg_cb cb %p", flow_diff_serv_cfg_cb));
                NBB_VERIFY_MEMORY(flow_diff_serv_cfg_cb, sizeof(ATG_DCI_LOG_PORT_FLOW_DIFF_SERV),
                                  MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
            }
        }
*/
        
        /*************************************************************************/
        /* 对上话复杂流分类QOS策略配置块进行验证。                                          */
        /*************************************************************************/
/*        inclassify_qos_cfg_cb = log_port_cb->inclassify_qos_cfg_cb;
        if(inclassify_qos_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify inclassify_qos_cfg_cb cb %p", inclassify_qos_cfg_cb));
            NBB_VERIFY_MEMORY(inclassify_qos_cfg_cb, sizeof(ATG_DCI_LOG_PORT_INCLASSIFY_QOS),
                              MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
        }
*/
        /*************************************************************************/
        /* 对上话用户QOS策略配置块进行验证。                                          */
        /*************************************************************************/
/*        up_user_cfg_cb = log_port_cb->up_user_qos_cfg_cb;
        if(up_user_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify up_user_cfg_cb cb %p", up_user_cfg_cb));
            NBB_VERIFY_MEMORY(up_user_cfg_cb, sizeof(ATG_DCI_LOG_UP_USER_QOS_POLICY),
                              MEM_SPM_LOG_PORT_UP_USER_QOS_CB);
        }
*/
        /*************************************************************************/
        /* 对上话用户组QOS策略配置块进行验证。                                          */
        /*************************************************************************/
        up_user_group_cfg_cb = log_port_cb->up_user_group_qos_cfg_cb;
        if(up_user_group_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify up_user_group_cfg_cb cb %p", up_user_group_cfg_cb));
            NBB_VERIFY_MEMORY(up_user_group_cfg_cb, sizeof(ATG_DCI_LOG_UP_USER_GROUP_QOS),
                              MEM_SPM_LOG_PORT_UP_GROUP_QOS_CB);
        }

        /*************************************************************************/
        /* 对下话用户队列QOS策略配置块进行验证。                                          */
        /*************************************************************************/
        down_user_cfg_cb = log_port_cb->down_user_qos_cfg_cb;
        if(down_user_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify down_user_cfg_cb cb %p", down_user_cfg_cb));
            NBB_VERIFY_MEMORY(down_user_cfg_cb, sizeof(ATG_DCI_LOG_DOWN_USER_QUEUE_QOS),
                              MEM_SPM_LOG_PORT_DOWN_USER_QOS_CB);
        }

        /*************************************************************************/
        /* 对下话用户组QOS配置块进行验证。                                          */
        /*************************************************************************/
        down_user_group_cfg_cb = log_port_cb->down_user_group_qos_cfg_cb;
        if(down_user_group_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify down_user_group_cfg_cb cb %p", down_user_group_cfg_cb));
            NBB_VERIFY_MEMORY(down_user_group_cfg_cb, sizeof(ATG_DCI_LOG_DOWN_USER_GROUP_QOS),
                              MEM_SPM_LOG_PORT_DOWN_GROUP_QOS_CB);
        }

        /*************************************************************************/
        /* 对包过滤器traffic_filter块进行验证。                               */
        /*************************************************************************/
/*
        for (i = 0; i < ATG_DCI_LOG_TRAFFIC_FILTER_NUM; i++)
        {
            traffic_filter_cfg_cb = log_port_cb->traffic_filter_cfg_cb[i];
            if(traffic_filter_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify traffic_filter_cfg_cb cb %p", traffic_filter_cfg_cb));
                NBB_VERIFY_MEMORY(traffic_filter_cfg_cb, sizeof(ATG_DCI_LOG_PORT_TRAFFIC_FILTER),
                                  MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
            }
        }
*/
            
        /*********************************************************************/
        /* 对终结子接口配置进行验证。                                        */
        /*********************************************************************/
        for (terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_FIRST(log_port_cb->terminal_if_tree);
             terminal_if_cb != NULL;
             terminal_if_cb = (SPM_TERMINAL_IF_CB*) AVLL_NEXT(log_port_cb->terminal_if_tree,
                           terminal_if_cb->spm_terminal_if_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify terminal_if_cb %p", terminal_if_cb));
            NBB_VERIFY_MEMORY(terminal_if_cb, sizeof(SPM_TERMINAL_IF_CB), MEM_SPM_LOG_PORT_TERMINAL_IF_CB);
        }

        /*********************************************************************/
        /* 对diff_serv配置进行验证。                                        */
        /*********************************************************************/
        for (diff_serv_cb = (SPM_DIFF_SERV_CB*) AVLL_FIRST(log_port_cb->diff_serv_tree);
             diff_serv_cb != NULL;
             diff_serv_cb = (SPM_DIFF_SERV_CB*) AVLL_NEXT(log_port_cb->diff_serv_tree,
                           diff_serv_cb->spm_diff_serv_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify diff_serv_cb %p", diff_serv_cb));
            NBB_VERIFY_MEMORY(diff_serv_cb, sizeof(SPM_DIFF_SERV_CB), MEM_SPM_LOG_PORT_DIFF_SERV_CB);
        }
             
        /*********************************************************************/
        /* 对流相关Diff-Serv配置L2 进行验证。                                        */
        /*********************************************************************/
        for (flow_diff_serv_cb = (SPM_FLOW_DIFF_SERV_CB*) AVLL_FIRST(log_port_cb->flow_diff_serv_tree);
             flow_diff_serv_cb != NULL;
             flow_diff_serv_cb = (SPM_FLOW_DIFF_SERV_CB*) AVLL_NEXT(log_port_cb->flow_diff_serv_tree,
                           flow_diff_serv_cb->spm_flow_diff_serv_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify flow_diff_serv_cb %p", flow_diff_serv_cb));
            NBB_VERIFY_MEMORY(flow_diff_serv_cb, sizeof(SPM_FLOW_DIFF_SERV_CB), MEM_SPM_LOG_PORT_FLOW_DIFF_SERV_CB);
        }
             
        /*********************************************************************/
        /* 对上话复杂流分类QOS策略配置进行验证。               */
        /*********************************************************************/
        for (inclassify_qos_cb = (SPM_INCLASSIFY_QOS_CB*) AVLL_FIRST(log_port_cb->inclassify_qos_tree);
             inclassify_qos_cb != NULL;
             inclassify_qos_cb = (SPM_INCLASSIFY_QOS_CB*) AVLL_NEXT(log_port_cb->inclassify_qos_tree,
                           inclassify_qos_cb->spm_inclassify_qos_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify inclassify_qos_cb %p", inclassify_qos_cb));
            NBB_VERIFY_MEMORY(inclassify_qos_cb, sizeof(SPM_INCLASSIFY_QOS_CB), MEM_SPM_LOG_PORT_INCLASSIFY_QOS_CB);
        }  
             
        /*********************************************************************/
        /* 对包过滤器traffic_filter进行验证。               */
        /*********************************************************************/
        for (traffic_filter_cb = (SPM_TRAFFIC_FILTER_CB*) AVLL_FIRST(log_port_cb->traffic_filter_tree);
             traffic_filter_cb != NULL;
             traffic_filter_cb = (SPM_TRAFFIC_FILTER_CB*) AVLL_NEXT(log_port_cb->traffic_filter_tree,
                           traffic_filter_cb->spm_traffic_filter_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify traffic_filter_cb %p", traffic_filter_cb));
            NBB_VERIFY_MEMORY(traffic_filter_cb, sizeof(SPM_TRAFFIC_FILTER_CB), MEM_SPM_LOG_PORT_TRAFFIC_FILTER_CB);
        } 
             
        /*************************************************************************/
        /* 对IPV6组播组地址L3配置块进行验证。                                        */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_LOG_PORT_MC_IPV6_NUM; i++)
        {
            mc_ipv6_cfg_cb = log_port_cb->mc_ipv6_cfg_cb[i];
            if(mc_ipv6_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify mc_ipv6_cfg_cb cb %p", mc_ipv6_cfg_cb));
                NBB_VERIFY_MEMORY(mc_ipv6_cfg_cb, sizeof(ATG_DCI_LOG_PORT_MC_IPV6_DATA),
                                  MEM_SPM_LOG_PORT_MC_IPV6_CB);
            }
        }

        /*************************************************************************/
        /* 对Diff-Serv配置L2（VP下话UNI侧）配置块进行验证。                      */
        /*************************************************************************/
        ds_l2_cfg_cb = log_port_cb->ds_l2_cfg_cb;
        if(ds_l2_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ds_l2_cfg_cb cb %p", ds_l2_cfg_cb));
            NBB_VERIFY_MEMORY(ds_l2_cfg_cb, sizeof(ATG_DCI_LOG_PORT_DS_L2_DATA),
                              MEM_SPM_LOG_PORT_DS_L2_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:      spm_cfg_lag_cb_verify                                    */
/*                                                                           */
/* Purpose:   遍历链路聚合配置的树逐一进行验证.                              */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_lag_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_LAG_CB *lag_cb = NULL;
    ATG_DCI_LINK_AGGR_GLOB_ATTRIB *global_cfg_cb = NULL;
    ATG_DCI_LINK_AGGR_LAG_CONF *lag_cfg_cb = NULL;
    ATG_DCI_LINK_AGGR_MEMBER_PORT *port_cfg_cb = NULL;
	ATG_DCI_LAG_NLB_MEMBER_PORT *nlb_port_cfg_cb = NULL;
    ATG_DCI_LAG_IPV4NF_DATA *ipv4_nf_cb = NULL;
    ATG_DCI_LAG_IPV6NF_DATA *ipv6_nf_cb = NULL;
    ATG_DCI_LAG_IPNF_SAMPLER_DATA *ipnf_sampler_cb = NULL;
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY("spm_cfg_link_aggr_cb_verify");

    /*************************************************************************/
    /* 遍历链路聚合配置的树逐一进行验证。                                    */
    /*************************************************************************/
    for (lag_cb = (SPM_LAG_CB*) AVLL_FIRST(SHARED.lag_tree);
         lag_cb != NULL;
         lag_cb = (SPM_LAG_CB*) AVLL_NEXT(SHARED.lag_tree,
                       lag_cb->spm_lag_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify lag_cb %p", lag_cb));
        NBB_VERIFY_MEMORY(lag_cb, sizeof(SPM_LAG_CB), MEM_SPM_LAG_CB);

        /*********************************************************************/
        /* 验证链路聚合配置控制块中的所有子配置。                            */
        /*********************************************************************/

        /*************************************************************************/
        /* 对全局属性配置块进行验证。                                            */
        /*************************************************************************/
        global_cfg_cb = lag_cb->global_cfg_cb;
        if(global_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify global_cfg_cb cb %p", global_cfg_cb));
            NBB_VERIFY_MEMORY(global_cfg_cb, sizeof(ATG_DCI_LINK_AGGR_GLOB_ATTRIB),
                              MEM_SPM_LAG_GLOBAL_CB);
        }

        /*************************************************************************/
        /* 对LAG配置块进行验证。                                                 */
        /*************************************************************************/
        lag_cfg_cb = lag_cb->lag_cfg_cb;
        if(lag_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify lag_cfg_cb cb %p", lag_cfg_cb));
            NBB_VERIFY_MEMORY(lag_cfg_cb, sizeof(ATG_DCI_LINK_AGGR_LAG_CONF),
                              MEM_SPM_LAG_CONF_CB);
        }

        /*************************************************************************/
        /* 对成员端口配置块进行验证。                                            */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_LAG_MEMBER_PORT_NUM; i++)
        {
            port_cfg_cb = lag_cb->port_cfg_cb[i];
            if(port_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify port_cfg_cb cb %p", port_cfg_cb));
                NBB_VERIFY_MEMORY(port_cfg_cb, sizeof(ATG_DCI_LINK_AGGR_MEMBER_PORT),
                                  MEM_SPM_LAG_PORT_CB);
            }
        }

        /*************************************************************************/
        /* 对非负载分担成员端口配置块进行验证。                                                 */
        /*************************************************************************/
        nlb_port_cfg_cb = lag_cb->nlb_port_cfg_cb;
        if(nlb_port_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify nlb_port_cfg_cb cb %p", lag_cfg_cb));
            NBB_VERIFY_MEMORY(nlb_port_cfg_cb, sizeof(ATG_DCI_LAG_NLB_MEMBER_PORT),
                              MEM_SPM_LAG_NLB_PORT_CB);
        }
        /*************************************************************************/
        /* 对ipv4_nf配置块进行验证。                                            */
        /*************************************************************************/
        ipv4_nf_cb = lag_cb->ipv4_nf_data;
        if(ipv4_nf_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_nf cb %p", ipv4_nf_cb));
            NBB_VERIFY_MEMORY(ipv4_nf_cb, sizeof(ATG_DCI_LAG_IPV4NF_DATA),
                              MEM_SPM_LAG_IPV4_NF_CB);
        }
        /*************************************************************************/
        /* 对ipv6_nf配置块进行验证。                                            */
        /*************************************************************************/
        ipv6_nf_cb = lag_cb->ipv6_nf_data;
        if(ipv6_nf_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_nf cb %p", ipv6_nf_cb));
            NBB_VERIFY_MEMORY(ipv6_nf_cb, sizeof(ATG_DCI_LAG_IPV6NF_DATA),
                              MEM_SPM_LAG_IPV6_NF_CB);
        }
        /*************************************************************************/
        /* 对ipnf_sampler配置块进行验证。                                            */
        /*************************************************************************/
        ipnf_sampler_cb = lag_cb->ip_sample_data;
        if(ipnf_sampler_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipnf_sampler_cb %p", ipnf_sampler_cb));
            NBB_VERIFY_MEMORY(ipnf_sampler_cb, sizeof(ATG_DCI_LAG_IPNF_SAMPLER_DATA),
                              MEM_SPM_LAG_IPNF_SAMPLER);
        }
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:      spm_cfg_vc_cb_verify                                           */
/*                                                                           */
/* Purpose:   遍历VC表配置的树逐一进行验证.                                  */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_vc_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_VC_CB *vc_cb = NULL;
    ATG_DCI_VC_BASIC_DATA *basic_cfg_cb = NULL;
    ATG_DCI_VC_DIFF_SERV_DATA *diff_serv_cfg_cb;
    ATG_DCI_VC_UP_VPN_QOS_POLICY *up_vpn_cfg_cb;
	ATG_DCI_VC_VPN_DATA *vpn_cfg_cb;
	ATG_DCI_VC_CONNECT_DETECT *connect_detect_cfg_cb;
	

    NBB_TRC_ENTRY("spm_cfg_vc_cb_verify");

    /*************************************************************************/
    /* 遍历VC表配置的树逐一进行验证。                                        */
    /*************************************************************************/
    for (vc_cb = (SPM_VC_CB*) AVLL_FIRST(SHARED.vc_tree);
         vc_cb != NULL;
         vc_cb = (SPM_VC_CB*) AVLL_NEXT(SHARED.vc_tree,
                       vc_cb->spm_vc_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify vc_cb %p", vc_cb));
        NBB_VERIFY_MEMORY(vc_cb, sizeof(SPM_VC_CB), MEM_SPM_VC_CB);

        /*********************************************************************/
        /* 验证VC表配置控制块中的所有子配置。                                */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基础配置块进行验证。                                               */
        /*************************************************************************/
        basic_cfg_cb = vc_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_VC_BASIC_DATA),
                              MEM_SPM_VC_BASIC_CB);
        }

        /*************************************************************************/
        /* 对Diff-Serv配置块进行验证。                                         */
        /*************************************************************************/
        diff_serv_cfg_cb = vc_cb->diff_serv_cfg_cb;
        if(diff_serv_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify diff_serv_cfg_cb cb %p", diff_serv_cfg_cb));
            NBB_VERIFY_MEMORY(diff_serv_cfg_cb, sizeof(ATG_DCI_VC_DIFF_SERV_DATA),
                              MEM_SPM_VC_DIFF_SERV_CB);
        }

        /*************************************************************************/
        /* 对上话VPN QOS策略配置块进行验证。                                   */
        /*************************************************************************/
        up_vpn_cfg_cb = vc_cb->up_vpn_cfg_cb;
        if(up_vpn_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify up_vpn_cfg_cb cb %p", up_vpn_cfg_cb));
            NBB_VERIFY_MEMORY(up_vpn_cfg_cb, sizeof(ATG_DCI_VC_UP_VPN_QOS_POLICY),
                              MEM_SPM_VC_UP_VPN_CB);
        }

		/*************************************************************************/
        /* 对VPN 属性配置块进行验证。                                          */
        /*************************************************************************/
        vpn_cfg_cb = vc_cb->vpn_property_cfg_cb;
        if(vpn_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify vpn_cfg cb %p", vpn_cfg_cb));
            NBB_VERIFY_MEMORY(vpn_cfg_cb, sizeof(ATG_DCI_VC_VPN_DATA),
                              MEM_SPM_VC_VPN_PROPER_CB);
        }

		/*************************************************************************/
        /* 对联通新检测配置块进行验证。                                        */
        /*************************************************************************/
		connect_detect_cfg_cb = vc_cb->connect_detect_cfg_cb;
		if(connect_detect_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify connect_detect_cfg_cb %p", connect_detect_cfg_cb));
            NBB_VERIFY_MEMORY(connect_detect_cfg_cb, sizeof(ATG_DCI_VC_CONNECT_DETECT),
                              MEM_SPM_VC_CONNECT_DETECT_CB);
        }
		
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:      spm_cfg_vpws_cb_verify                                         */
/*                                                                           */
/* Purpose:   遍历VPWS配置的树逐一进行验证.                                  */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_vpws_cb_verify(NBB_CXT_T NBB_CXT)
{
	
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_VPWS_CB *vpws_cb = NULL;
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY("spm_cfg_vpws_cb_verify");

    /*************************************************************************/
    /* 遍历VPWS配置的树逐一进行验证。                                        */
    /*************************************************************************/
    for (vpws_cb = (SPM_VPWS_CB*) AVLL_FIRST(SHARED.vpws_tree);
         vpws_cb != NULL;
         vpws_cb = (SPM_VPWS_CB*) AVLL_NEXT(SHARED.vpws_tree,
                   vpws_cb->spm_vpws_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify vpws_cb %p", vpws_cb));
        NBB_VERIFY_MEMORY(vpws_cb, sizeof(SPM_VPWS_CB), MEM_SPM_VPWS_CB);
    }

    NBB_TRC_EXIT();

	

    return;
}


/**PROC+**********************************************************************/
/* Name:      spm_cfg_vpls_cb_verify                                         */
/*                                                                           */
/* Purpose:   遍历VPLS配置的树逐一进行验证.                                  */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_vpls_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_VPLS_CB *vpls_cb = NULL;
    ATG_DCI_VPLS_BASIC_DATA *basic_cfg_cb = NULL;
    SPM_VPLS_NNI_CB *nni_cfg_cb = NULL;
    SPM_VPLS_UNI_CB *uni_cfg_cb = NULL;
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY("spm_cfg_vpls_cb_verify");

    /*************************************************************************/
    /* 遍历VPLS配置的树逐一进行验证。                                        */
    /*************************************************************************/
    for (vpls_cb = (SPM_VPLS_CB*) AVLL_FIRST(SHARED.vpls_tree);
         vpls_cb != NULL;
         vpls_cb = (SPM_VPLS_CB*) AVLL_NEXT(SHARED.vpls_tree,
                   vpls_cb->spm_vpls_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify vpls_cb %p", vpls_cb));
        NBB_VERIFY_MEMORY(vpls_cb, sizeof(SPM_VPLS_CB), MEM_SPM_VPLS_CB);

        /*********************************************************************/
        /* 验证VPLS配置控制块中的所有子配置。                                */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = vpls_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_VPLS_BASIC_DATA),
                              MEM_SPM_VPLS_BASIC_CB);
        }

        /*************************************************************************/
        /* 对NNI接口配置块进行验证。                                             */
        /*************************************************************************/
        for(nni_cfg_cb = (SPM_VPLS_NNI_CB *)AVLL_FIRST(vpls_cb->nni_cfg_tree);
            nni_cfg_cb != NULL;
            nni_cfg_cb = (SPM_VPLS_NNI_CB *)AVLL_NEXT(vpls_cb->nni_cfg_tree, 
                nni_cfg_cb->spm_vpls_nni_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify nni_cfg_cb cb %p", nni_cfg_cb));
            NBB_VERIFY_MEMORY(nni_cfg_cb, sizeof(SPM_VPLS_NNI_CB),
                              MEM_SPM_VPLS_NNI_CB);
        }

        /*************************************************************************/
        /* 对UNI接口配置块进行验证。                                             */
        /*************************************************************************/
        for(uni_cfg_cb = (SPM_VPLS_NNI_CB *)AVLL_FIRST(vpls_cb->uni_cfg_tree);
            uni_cfg_cb != NULL;
            uni_cfg_cb = (SPM_VPLS_NNI_CB *)AVLL_NEXT(vpls_cb->uni_cfg_tree, 
                uni_cfg_cb->spm_vpls_uni_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify uni_cfg_cb cb %p", uni_cfg_cb));
            NBB_VERIFY_MEMORY(uni_cfg_cb, sizeof(SPM_VPLS_UNI_CB),
                              MEM_SPM_VPLS_UNI_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:      spm_cfg_arp_cb_verify                                          */
/*                                                                           */
/* Purpose:   遍历ARP表配置的树逐一进行验证.                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_arp_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_ARP_CB *arp_cb = NULL;
    ATG_DCI_ARP_BASIC_DATA *basic_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_arp_cb_verify");

    /*************************************************************************/
    /* 遍历ARP表配置的树逐一进行验证。                                       */
    /*************************************************************************/
    for (arp_cb = (SPM_ARP_CB*) AVLL_FIRST(SHARED.arp_tree);
         arp_cb != NULL;
         arp_cb = (SPM_ARP_CB*) AVLL_NEXT(SHARED.arp_tree,
                       arp_cb->spm_arp_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify arp_cb %p", arp_cb));
        NBB_VERIFY_MEMORY(arp_cb, sizeof(SPM_ARP_CB), MEM_SPM_ARP_CB);

        /*********************************************************************/
        /* 验证ARP表配置控制块中的所有子配置。                               */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = arp_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_ARP_BASIC_DATA),
                              MEM_SPM_ARP_BASIC_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}

/**PROC+**********************************************************************/
/* Name:      spm_cfg_ncache_cb_verify                                          */
/*                                                                           */
/* Purpose:   遍历IPv6 邻居缓存配置的树逐一进行验证.                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_ncache_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_NCACHE_CB *ncache_cb = NULL;
    ATG_DCI_IPV6_CACHE_BASIC_DATA *basic_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_ncache_cb_verify");

    /*************************************************************************/
    /* 遍历IPv6 邻居缓存配置的树逐一进行验证。                                       */
    /*************************************************************************/
    for (ncache_cb = (SPM_NCACHE_CB*) AVLL_FIRST(SHARED.ncache_tree);
         ncache_cb != NULL;
         ncache_cb = (SPM_NCACHE_CB*) AVLL_NEXT(SHARED.ncache_tree,
                       ncache_cb->spm_ncache_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify ncache_cb %p", ncache_cb));
        NBB_VERIFY_MEMORY(ncache_cb, sizeof(SPM_NCACHE_CB), MEM_SPM_NCACHE_CB);

        /*********************************************************************/
        /* 验证IPv6 邻居缓存配置控制块中的所有子配置。                               */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = ncache_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_IPV6_CACHE_BASIC_DATA),
                              MEM_SPM_NCACHE_BASIC_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}

/**PROC+**********************************************************************/
/* Name:      spm_cfg_port_mirror_cb_verify                                          */
/*                                                                           */
/* Purpose:   遍历端口镜像存配置的树逐一进行验证.                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_port_mirror_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_PORT_MIRROR_CB *port_mirror_cb = NULL;
    ATG_DCI_PORT_MIRROR_BASIC_DATA *basic_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_port_mirror_cb_verify");

    /*************************************************************************/
    /* 遍历端口镜像存配置的树逐一进行验证。                                       */
    /*************************************************************************/
    for (port_mirror_cb = (SPM_PORT_MIRROR_CB*) AVLL_FIRST(SHARED.port_mirror_tree);
         port_mirror_cb != NULL;
         port_mirror_cb = (SPM_PORT_MIRROR_CB*) AVLL_NEXT(SHARED.port_mirror_tree,
                       port_mirror_cb->spm_port_mirror_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify port_mirror_cb %p", port_mirror_cb));
        NBB_VERIFY_MEMORY(port_mirror_cb, sizeof(SPM_PORT_MIRROR_CB), MEM_SPM_PORT_MIRROR_CB);

        /*********************************************************************/
        /* 验证端口镜像配置控制块中的所有子配置。                               */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = port_mirror_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_PORT_MIRROR_BASIC_DATA),
                              MEM_SPM_PORT_MIRROR_BASIC_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:      bmu_cfg_vpls_mc_cb_verify                                      */
/*                                                                           */
/* Purpose:   遍历VPLS组播配置的树逐一进行验证.                              */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_vpls_mc_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_VPLS_MC_CB *vpls_mc_cb = NULL;
    ATG_DCI_VPLS_MC_BASIC_DATA *basic_cfg_cb = NULL;
    ATG_DCI_VPLS_MC_NNI_DATA *nni_cfg_cb = NULL;
    ATG_DCI_VPLS_MC_UNI_DATA *uni_cfg_cb = NULL;
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY("spm_cfg_vpls_mc_cb_verify");

    /*************************************************************************/
    /* 遍历VPLS组播配置的树逐一进行验证。                                    */
    /*************************************************************************/
    for (vpls_mc_cb = (SPM_VPLS_MC_CB*) AVLL_FIRST(SHARED.vpls_mc_tree);
         vpls_mc_cb != NULL;
         vpls_mc_cb = (SPM_VPLS_MC_CB*) AVLL_NEXT(SHARED.vpls_mc_tree,
                     vpls_mc_cb->spm_vpls_mc_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify vpls_mc_cb %p", vpls_mc_cb));
        NBB_VERIFY_MEMORY(vpls_mc_cb, sizeof(SPM_VPLS_MC_CB), MEM_SPM_VPLS_MC_CB);

        /*********************************************************************/
        /* 验证VPLS组播配置控制块中的所有子配置。                            */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = vpls_mc_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_VPLS_MC_BASIC_DATA),
                              MEM_SPM_VPLS_MC_BASIC_CB);
        }

        /*************************************************************************/
        /* 对NNI成员配置块进行验证。                                             */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_VPLS_MC_NNI_NUM; i++)
        {
            nni_cfg_cb = vpls_mc_cb->nni_cfg_cb[i];
            if(nni_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify nni_cfg_cb cb %p", nni_cfg_cb));
                NBB_VERIFY_MEMORY(nni_cfg_cb, sizeof(ATG_DCI_VPLS_MC_NNI_DATA),
                                  MEM_SPM_VPLS_MC_NNI_CB);
            }
        }

        /*************************************************************************/
        /* 对UNI成员配置块进行验证。                                             */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_VPLS_MC_UNI_NUM; i++)
        {
            uni_cfg_cb = vpls_mc_cb->uni_cfg_cb[i];
            if(uni_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify uni_cfg_cb cb %p", uni_cfg_cb));
                NBB_VERIFY_MEMORY(uni_cfg_cb, sizeof(ATG_DCI_VPLS_MC_UNI_DATA),
                                  MEM_SPM_VPLS_MC_UNI_CB);
            }
        }
    }

    NBB_TRC_EXIT();

    return;
}

/**PROC+**********************************************************************/
/* Name:      spm_cfg_vpls_uc_cb_verify                                          */
/*                                                                           */
/* Purpose:   遍历端口镜像存配置的树逐一进行验证.                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_vpls_uc_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_VPLS_UC_CB *vpls_uc_cb = NULL;
    ATG_DCI_VPLS_UC_BASIC_DATA *basic_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_vpls_uc_cb_verify");

    /*************************************************************************/
    /* 遍历端口镜像存配置的树逐一进行验证。                                       */
    /*************************************************************************/
    for (vpls_uc_cb = (SPM_VPLS_UC_CB*) AVLL_FIRST(SHARED.vpls_uc_tree);
         vpls_uc_cb != NULL;
         vpls_uc_cb = (SPM_VPLS_UC_CB*) AVLL_NEXT(SHARED.vpls_uc_tree,
                       vpls_uc_cb->spm_vpls_uc_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify vpls_uc_cb %p", vpls_uc_cb));
        NBB_VERIFY_MEMORY(vpls_uc_cb, sizeof(SPM_VPLS_UC_CB), MEM_SPM_VPLS_UC_CB);

        /*********************************************************************/
        /* 验证端口镜像配置控制块中的所有子配置。                               */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = vpls_uc_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_VPLS_UC_BASIC_DATA),
                              MEM_SPM_VPLS_UC_BASIC_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}

/**PROC+**********************************************************************/
/* Name:      spm_cfg_switch_vc_cb_verify                                    */
/*                                                                           */
/* Purpose:   遍历SWITCH VC表配置的树逐一进行验证.                            */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_switch_vc_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_SWITCH_VC_CB *switch_vc_cb = NULL;
    ATG_DCI_SWITCH_VC_VC_DATA *vc1_cfg_cb = NULL;
    ATG_DCI_SWITCH_VC_VC_DATA *vc2_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_switch_vc_cb_verify");

    /*************************************************************************/
    /* 遍历SWITCH VC表配置的树逐一进行验证。                                       */
    /*************************************************************************/
    for (switch_vc_cb = (SPM_SWITCH_VC_CB*) AVLL_FIRST(SHARED.switch_vc_tree);
         switch_vc_cb != NULL;
         switch_vc_cb = (SPM_SWITCH_VC_CB*) AVLL_NEXT(SHARED.switch_vc_tree,
                       switch_vc_cb->spm_switch_vc_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify switch_vc_cb %p", switch_vc_cb));
        NBB_VERIFY_MEMORY(switch_vc_cb, sizeof(SPM_SWITCH_VC_CB), MEM_SPM_SWITCH_VC_CB);

    }

    NBB_TRC_EXIT();

    return;
}

NBB_VOID spm_associate_if_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_ASSOCIATE_IF_CB *pstAssociateIfCb = NULL;
	ATG_DCI_ASSOCIATE_IF_BASIC *basic_cfg_cb = NULL;
	ATG_DCI_ASSOCIATE_IF_TRIGGER_IF *trigger_if_cfg_cb = NULL;
	ATG_DCI_ASSOCIATE_IF_ACT_IF *act_if_cfg_cb = NULL;
	NBB_ULONG i = 0;

    NBB_TRC_ENTRY("spm_associate_if_cb_verify");

    /*************************************************************************/
    /* 遍历关联接口配置的树逐一进行验证。                                       */
    /*************************************************************************/
    for (pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB*) AVLL_FIRST(SHARED.associate_if_tree);
         pstAssociateIfCb != NULL;
         pstAssociateIfCb = (SPM_ASSOCIATE_IF_CB*) AVLL_NEXT(SHARED.associate_if_tree,
                       pstAssociateIfCb->spm_associate_if_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify pstAssociateIfCb %p", pstAssociateIfCb));
        NBB_VERIFY_MEMORY(pstAssociateIfCb, sizeof(SPM_ASSOCIATE_IF_CB), MEM_SPM_ASSOCIATE_IF_CB);

        /*********************************************************************/
        /* 验证关联接口配置控制块中的所有子配置。                               */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基础配置进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = pstAssociateIfCb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify associate_if_basic cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_ASSOCIATE_IF_BASIC),
                              MEM_SPM_ASSOCIATE_BASIC_CB);
        }

        /*************************************************************************/
        /* 对触发接口配置进行验证。                                                */
        /*************************************************************************/
		for (i=0; i<ATG_DCI_ASSOCIATE_IF_TRIGGER_NUM; i++)
		{
			trigger_if_cfg_cb = pstAssociateIfCb->trigger_if_cfg_cb[i];
			if(trigger_if_cfg_cb != NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "trigger_if_cfg_cb %p", trigger_if_cfg_cb));
            	NBB_VERIFY_MEMORY(trigger_if_cfg_cb, sizeof(ATG_DCI_ASSOCIATE_IF_TRIGGER_IF),
                              MEM_SPM_ASSOCIATE_TRIGGER_IF_CB);
			}
		}
		 /*************************************************************************/
        /* 对动作接口配置进行验证。                                                */
        /*************************************************************************/

		for (i=0; i<ATG_DCI_ASSOCIATE_IF_ACT_NUM; i++)
		{
			act_if_cfg_cb = pstAssociateIfCb->act_if_cfg_cb[i];
			if(act_if_cfg_cb != NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "act_if_cfg_cb %p", act_if_cfg_cb));
            	NBB_VERIFY_MEMORY(act_if_cfg_cb, sizeof(ATG_DCI_ASSOCIATE_IF_ACT_IF),
                              MEM_SPM_ASSOCIATE_ACT_IF_CB);
			}
		}

		
      }

     NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:      spm_cfg_bypass_cb_verify                                    */
/*                                                                           */
/* Purpose:   遍历Bypass双归保护配置的树逐一进行验证.                            */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_bypass_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_BYPASS_CB *bypass_cb = NULL;
    ATG_DCI_BYPASS_BASIC_DATA *basic_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_bypass_cb_verify");

    /*************************************************************************/
    /* 遍历Bypass双归保护配置的树逐一进行验证。                                       */
    /*************************************************************************/
    for (bypass_cb = (SPM_BYPASS_CB*) AVLL_FIRST(SHARED.bypass_tree);
         bypass_cb != NULL;
         bypass_cb = (SPM_BYPASS_CB*) AVLL_NEXT(SHARED.bypass_tree,
                       bypass_cb->spm_bypass_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify bypass_cb %p", bypass_cb));
        NBB_VERIFY_MEMORY(bypass_cb, sizeof(SPM_BYPASS_CB), MEM_SPM_BYPASS_CB);

        /*********************************************************************/
        /* 验证Bypass双归保护配置控制块中的所有子配置。                               */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = bypass_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_BYPASS_BASIC_DATA),
                              MEM_SPM_BYPASS_BASIC_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}

/**PROC+**********************************************************************/
/* Name:      spm_cfg_msp_cb_verify                                          */
/*                                                                           */
/* Purpose:   遍历MSP保护存配置的树逐一进行验证.                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_msp_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_MSP_CB *msp_cb = NULL;
    ATG_DCI_MSP_BASIC *basic_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_msp_cb_verify");

    /*************************************************************************/
    /* 遍历MSP保护存配置的树逐一进行验证。                                       */
    /*************************************************************************/
    for (msp_cb = (SPM_MSP_CB*) AVLL_FIRST(SHARED.msp_tree);
         msp_cb != NULL;
         msp_cb = (SPM_MSP_CB*) AVLL_NEXT(SHARED.msp_tree,
                       msp_cb->spm_msp_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify msp_cb %p", msp_cb));
        NBB_VERIFY_MEMORY(msp_cb, sizeof(SPM_MSP_CB), MEM_SPM_MSP_CB);

        /*********************************************************************/
        /* 验证MSP保护配置控制块中的所有子配置。                               */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = msp_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_MSP_BASIC),
                              MEM_SPM_MSP_BASIC_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}

/**PROC+**********************************************************************/
/* Name:      spm_cfg_cir_emu_cb_verify                                         */
/*                                                                           */
/* Purpose:   遍历cir_emu配置的树逐一进行验证.                                  */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_cir_emu_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_CIR_EMU_CB *cir_emu_cb = NULL;
    ATG_DCI_CIR_EMU_PUB_LIMIT *pstPubLimit = NULL;
    ATG_DCI_CIR_EMU_SYN_CLK *pstSynClk = NULL;
    ATG_DCI_CIR_EMU_ELTRC_BOARD *pstEltrcBoard = NULL;
    ATG_DCI_CIR_EMU_REBTH_LIMIT *pstRebthLimit = NULL;
    ATG_DCI_CIR_EMU_MU_LIMIT *pstMuLimit = NULL;
    ATG_DCI_CIR_EMU_HIGH_ORD_PASS *pstHighOrdPass = NULL;
    ATG_DCI_CIR_EMU_LOW_ORD_PASS *pstLowOrdPass = NULL;

    NBB_TRC_ENTRY("spm_cfg_cir_emu_cb_verify");

    /*************************************************************************/
    /* 遍历VPLS配置的树逐一进行验证。                                        */
    /*************************************************************************/
    for (cir_emu_cb = (SPM_CIR_EMU_CB*) AVLL_FIRST(SHARED.cir_emu_tree);
         cir_emu_cb != NULL;
         cir_emu_cb = (SPM_CIR_EMU_CB*) AVLL_NEXT(SHARED.cir_emu_tree,
                   cir_emu_cb->spm_cir_emu_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cir_emu_cb %p", cir_emu_cb));
        NBB_VERIFY_MEMORY(cir_emu_cb, sizeof(SPM_CIR_EMU_CB), MEM_SPM_CIR_EMU_CB);

        /*********************************************************************/
        /* 验证VPLS配置控制块中的所有子配置。                                */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        pstPubLimit = cir_emu_cb->pub_limit_cb;
        if(pstPubLimit != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify pstPubLimit cb %p", pstPubLimit));
            NBB_VERIFY_MEMORY(pstPubLimit, sizeof(ATG_DCI_CIR_EMU_PUB_LIMIT),
                              MEM_SPM_CIR_EMU_PUB_LIMIT_CB);
        }
        
        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        pstSynClk = cir_emu_cb->syn_clk_cb;
        if(pstSynClk != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify pstSynClk cb %p", pstSynClk));
            NBB_VERIFY_MEMORY(pstSynClk, sizeof(ATG_DCI_CIR_EMU_SYN_CLK),
                              MEM_SPM_CIR_EMU_SYN_CLK_CB);
        }
        
        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        pstEltrcBoard = cir_emu_cb->eltrc_board_cb;
        if(pstEltrcBoard != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify pstEltrcBoard cb %p", pstEltrcBoard));
            NBB_VERIFY_MEMORY(pstEltrcBoard, sizeof(ATG_DCI_CIR_EMU_ELTRC_BOARD),
                              MEM_SPM_CIR_EMU_ELTRC_BOARD_CB);
        }   

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        pstRebthLimit = cir_emu_cb->rebth_limit_cb;
        if(pstRebthLimit != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify pstRebthLimit cb %p", pstRebthLimit));
            NBB_VERIFY_MEMORY(pstRebthLimit, sizeof(ATG_DCI_CIR_EMU_REBTH_LIMIT),
                              MEM_SPM_CIR_EMU_REBTH_LIMIT_CB);
        }
        
        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        pstMuLimit = cir_emu_cb->mu_limit_cb;
        if(pstMuLimit != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify pstMuLimit cb %p", pstMuLimit));
            NBB_VERIFY_MEMORY(pstMuLimit, sizeof(ATG_DCI_CIR_EMU_MU_LIMIT),
                              MEM_SPM_CIR_EMU_MU_LIMIT_CB);
        }
        
        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        pstHighOrdPass = cir_emu_cb->high_ord_pass_cb;
        if(pstHighOrdPass != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify pstHighOrdPass cb %p", pstHighOrdPass));
            NBB_VERIFY_MEMORY(pstHighOrdPass, sizeof(ATG_DCI_CIR_EMU_HIGH_ORD_PASS),
                              MEM_SPM_CIR_EMU_HIGH_ORD_PASS_CB);
        }
        
        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        pstLowOrdPass = cir_emu_cb->low_ord_pass_cb;
        if(pstLowOrdPass != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify pstLowOrdPass cb %p", pstLowOrdPass));
            NBB_VERIFY_MEMORY(pstLowOrdPass, sizeof(ATG_DCI_CIR_EMU_LOW_ORD_PASS),
                              MEM_SPM_CIR_EMU_LOW_ORD_PASS_CB);
        }       

    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:      spm_cfg_vpls_l3_mc_cb_verify                                      */
/*                                                                           */
/* Purpose:   遍历VPLS_L3组播配置的树逐一进行验证.                              */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_vpls_l3_mc_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_VPLS_L3_MC_CB *vpls_l3_mc_cb = NULL;
    ATG_DCI_VPLS_L3_MC_BASIC_DATA *basic_cfg_cb = NULL;
    ATG_DCI_VPLS_L3_MC_NNI_DATA *nni_cfg_cb = NULL;
    ATG_DCI_VPLS_L3_MC_UNI_DATA *uni_cfg_cb = NULL;
    NBB_ULONG i = 0;

    NBB_TRC_ENTRY("spm_cfg_vpls_l3_mc_cb_verify");

    /*************************************************************************/
    /* 遍历VPLS_L3组播配置的树逐一进行验证。                                    */
    /*************************************************************************/
    for (vpls_l3_mc_cb = (SPM_VPLS_L3_MC_CB*) AVLL_FIRST(SHARED.vpls_l3_mc_tree);
        vpls_l3_mc_cb != NULL;
        vpls_l3_mc_cb = (SPM_VPLS_L3_MC_CB*) AVLL_NEXT(SHARED.vpls_l3_mc_tree,
        vpls_l3_mc_cb->spm_vpls_l3_mc_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify vpls_l3_mc_cb %p", vpls_l3_mc_cb));
        NBB_VERIFY_MEMORY(vpls_l3_mc_cb, sizeof(SPM_VPLS_L3_MC_CB), MEM_SPM_VPLS_L3_MC_CFG);

        /*********************************************************************/
        /* 验证VPLS_L3组播配置控制块中的所有子配置。                            */
        /*********************************************************************/

        /*************************************************************************/
        /* 对基本配置块进行验证。                                                */
        /*************************************************************************/
        basic_cfg_cb = vpls_l3_mc_cb->basic_cfg_cb;
        if (basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_VPLS_L3_MC_BASIC_DATA),
                              MEM_SPM_VPLS_L3_MC_BASIC_CFG);
        }

        /*************************************************************************/
        /* 对NNI成员配置块进行验证。                                             */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_VPLS_MC_NNI_NUM; i++)
        {
            nni_cfg_cb = vpls_l3_mc_cb->nni_cfg_cb[i];
            if (nni_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify nni_cfg_cb cb %p", nni_cfg_cb));
                NBB_VERIFY_MEMORY(nni_cfg_cb, sizeof(ATG_DCI_VPLS_L3_MC_NNI_DATA),
                                  MEM_SPM_VPLS_L3_MC_NNI_CFG);
            }
        }

        /*************************************************************************/
        /* 对UNI成员配置块进行验证。                                             */
        /*************************************************************************/
        for (i = 0; i < ATG_DCI_VPLS_MC_UNI_NUM; i++)
        {
            uni_cfg_cb = vpls_l3_mc_cb->uni_cfg_cb[i];
            if(uni_cfg_cb != NULL)
            {
                NBB_TRC_FLOW((NBB_FORMAT "Verify uni_cfg_cb cb %p", uni_cfg_cb));
                NBB_VERIFY_MEMORY(uni_cfg_cb, sizeof(ATG_DCI_VPLS_L3_MC_UNI_DATA),
                                  MEM_SPM_VPLS_L3_MC_UNI_CFG);
            }
        }
    }

    NBB_TRC_EXIT();

    return;
}



#if 0




NBB_VOID spm_cfg_ds_domain_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_DS_DOMAIN_CB *ds_domain_cb = NULL;
    ATG_DCI_DS_DOMAIN_DSCP2PHBMAP *d2p_cfg_cb = NULL;
    ATG_DCI_DS_DOMAIN_V8021P2PHBMAP *vlan2p_cfg_cb = NULL;
    ATG_DCI_DS_DOMAIN_EXP2PHBMAP *exp2p_cfg_cb = NULL;
    ATG_DCI_DS_DOMAIN_PHB2EXPMAP *p2exp_cfg_cb = NULL;
    ATG_DCI_DS_DOMAIN_PHB2V8021PMAP *p2vlan_cfg_cb = NULL;
    ATG_DCI_DS_DOMAIN_PHB2DSCPMAP *p2d_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_ds_domain_cb_verify");

    for (ds_domain_cb = (SPM_DS_DOMAIN_CB*) AVLL_FIRST(SHARED.ds_domain_tree);
         ds_domain_cb != NULL;
         ds_domain_cb = (SPM_DS_DOMAIN_CB*) AVLL_NEXT(SHARED.ds_domain_tree,
                       ds_domain_cb->spm_ds_domain_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify ds_domain_cb %p", ds_domain_cb));
        NBB_VERIFY_MEMORY(ds_domain_cb, sizeof(SPM_DS_DOMAIN_CB), MEM_SPM_DS_DOMAIN_CB);

        d2p_cfg_cb = ds_domain_cb->d2p_cfg_cb;
        if(d2p_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify d2p_cfg_cb cb %p", d2p_cfg_cb));
            NBB_VERIFY_MEMORY(d2p_cfg_cb, sizeof(ATG_DCI_DS_DOMAIN_DSCP2PHBMAP),
                              MEM_SPM_DS_DOMAIN_D2P_CB);
        }


        vlan2p_cfg_cb = ds_domain_cb->vlan2p_cfg_cb;
        if(vlan2p_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify vlan2p_cfg_cb cb %p", vlan2p_cfg_cb));
            NBB_VERIFY_MEMORY(vlan2p_cfg_cb, sizeof(ATG_DCI_DS_DOMAIN_V8021P2PHBMAP),
                              MEM_SPM_DS_DOMAIN_VLAN2P_CB);
        }

        exp2p_cfg_cb = ds_domain_cb->exp2p_cfg_cb;
        if(exp2p_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify exp2p_cfg_cb cb %p", exp2p_cfg_cb));
            NBB_VERIFY_MEMORY(exp2p_cfg_cb, sizeof(ATG_DCI_DS_DOMAIN_EXP2PHBMAP),
                              MEM_SPM_DS_DOMAIN_EXP2P_CB);
        }

        p2d_cfg_cb = ds_domain_cb->p2d_cfg_cb;
        if(p2d_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify p2d_cfg_cb cb %p", p2d_cfg_cb));
            NBB_VERIFY_MEMORY(p2d_cfg_cb, sizeof(ATG_DCI_DS_DOMAIN_PHB2DSCPMAP),
                              MEM_SPM_DS_DOMAIN_P2D_CB);
        }


        p2vlan_cfg_cb = ds_domain_cb->p2vlan_cfg_cb;
        if(p2vlan_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify p2vlan_cfg_cb cb %p", p2vlan_cfg_cb));
            NBB_VERIFY_MEMORY(p2vlan_cfg_cb, sizeof(ATG_DCI_DS_DOMAIN_PHB2V8021PMAP),
                              MEM_SPM_DS_DOMAIN_P2VLAN_CB);
        }

        p2exp_cfg_cb = ds_domain_cb->p2exp_cfg_cb;
        if(p2exp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify p2exp_cfg_cb cb %p", p2exp_cfg_cb));
            NBB_VERIFY_MEMORY(p2exp_cfg_cb, sizeof(ATG_DCI_DS_DOMAIN_PHB2EXPMAP),
                              MEM_SPM_DS_DOMAIN_P2EXP_CB);
        }
      
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:                                                                     */
/*                                                                           */
/* Purpose:                                .                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_acl_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_ACL_CB *acl_cb = NULL;

    ATG_DCI_ACL_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_ACL_PORT_DATA *Port_cfg_cb;

    ATG_DCI_ACL_ETH_HEADER *Eth_cfg_cb;

    ATG_DCI_ACL_IPV4_FRAME *ipv4_cfg_cb;

    ATG_DCI_ACL_IPV4_TCP *ipTcp_cfg_cb;

    ATG_DCI_ACL_IPV6_FRAME *ipv6_cfg_cb;

    ATG_DCI_ACL_NONE_IP *noIp_cfg_cb;

    ATG_DCI_ACL_IPV4_UDP *ipUdp_cfg_cb;
#ifdef R8000_V3R2
    ATG_DCI_ACL_PORT_VLAN *portVlan_cfg_cb;
#endif
    NBB_TRC_ENTRY("spm_cfg_acl_cb_verify");

    for (acl_cb = (SPM_QOS_ACL_CB*) AVLL_FIRST(SHARED.qos_acl_tree);
         acl_cb != NULL;
         acl_cb = (SPM_QOS_ACL_CB*) AVLL_NEXT(SHARED.qos_acl_tree,
                       acl_cb->spm_acl_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify acl_cb %p", acl_cb));
        NBB_VERIFY_MEMORY(acl_cb, sizeof(SPM_QOS_ACL_CB), MEM_SPM_ACL_CB);

        basic_cfg_cb = acl_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_ACL_BASIC_DATA),
                              MEM_SPM_ACL_BASIC_CB);
        }

        Port_cfg_cb = acl_cb->Port_cfg_cb;
        if(Port_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify Port_cfg_cb cb %p", Port_cfg_cb));
            NBB_VERIFY_MEMORY(Port_cfg_cb, sizeof(ATG_DCI_ACL_PORT_DATA),
                              MEM_SPM_ACL_PORT_CB);
        }

        Eth_cfg_cb = acl_cb->Eth_cfg_cb;
        if(Eth_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify Eth_cfg_cb cb %p", Eth_cfg_cb));
            NBB_VERIFY_MEMORY(Eth_cfg_cb, sizeof(ATG_DCI_ACL_ETH_HEADER),
                              MEM_SPM_ACL_ETH_CB);
        }

        ipv4_cfg_cb = acl_cb->ipv4_cfg_cb;
        if(ipv4_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_cfg_cb cb %p", ipv4_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_cfg_cb, sizeof(ATG_DCI_ACL_IPV4_FRAME),
                              MEM_SPM_ACL_IPV4_CB);
        }

        ipTcp_cfg_cb = acl_cb->ipTcp_cfg_cb;
        if(ipTcp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipTcp_cfg_cb cb %p", ipTcp_cfg_cb));
            NBB_VERIFY_MEMORY(ipTcp_cfg_cb, sizeof(ATG_DCI_ACL_IPV4_TCP),
                              MEM_SPM_ACL_IPTCP_CB);
        }

        ipv6_cfg_cb = acl_cb->ipv6_cfg_cb;
        if(ipv6_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_cfg_cb cb %p", ipv6_cfg_cb));
            NBB_VERIFY_MEMORY(ipv6_cfg_cb, sizeof(ATG_DCI_ACL_IPV6_FRAME),
                              MEM_SPM_ACL_IPV6_CB);
        }

        noIp_cfg_cb = acl_cb->noIp_cfg_cb;
        if(noIp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify noIp_cfg_cb cb %p", noIp_cfg_cb));
            NBB_VERIFY_MEMORY(noIp_cfg_cb, sizeof(ATG_DCI_ACL_NONE_IP),
                              MEM_SPM_ACL_NOIP_CB);
        }

        ipUdp_cfg_cb = acl_cb->ipUdp_cfg_cb;
        if(ipUdp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipUdp_cfg_cb cb %p", ipUdp_cfg_cb));
            NBB_VERIFY_MEMORY(ipUdp_cfg_cb, sizeof(ATG_DCI_ACL_IPV4_UDP),
                              MEM_SPM_ACL_IPUDP_CB);
        }
#ifdef R8000_V3R2
        portVlan_cfg_cb = acl_cb->portVlan_cfg_cb;
        if(portVlan_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify portVlan_cfg_cb cb %p", portVlan_cfg_cb));
            NBB_VERIFY_MEMORY(portVlan_cfg_cb, sizeof(ATG_DCI_ACL_PORT_VLAN),
                              MEM_SPM_ACL_PORTVLAN_CB);
        }
#endif        
    }

    NBB_TRC_EXIT();

    return;
}

/**PROC+**********************************************************************/
/* Name:                                                                     */
/*                                                                           */
/* Purpose:                                .                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_policy_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_POLICY_CB *policy_cb = NULL;
    SPM_QOS_POLICY_C2B_CB *c2b_cb = NULL;
    NBB_USHORT i=0;
    ATG_DCI_QOS_POLICY_BASIC_DATA *basic_cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_policy_cb_verify");

    for (policy_cb = (SPM_QOS_POLICY_CB*) AVLL_FIRST(SHARED.qos_policy_tree);
         policy_cb != NULL;
         policy_cb = (SPM_QOS_POLICY_CB*) AVLL_NEXT(SHARED.qos_policy_tree,
                       policy_cb->spm_policy_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify policy_cb %p", policy_cb));
        NBB_VERIFY_MEMORY(policy_cb, sizeof(SPM_QOS_POLICY_CB), MEM_SPM_POLICY_CB);

        basic_cfg_cb = policy_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_QOS_POLICY_BASIC_DATA),
                              MEM_SPM_POLICY_BASIC_CB);
        }

        for (c2b_cb = (SPM_QOS_POLICY_C2B_CB*) AVLL_FIRST(policy_cb->c2b_tree);
             (NULL != c2b_cb);
             c2b_cb = (SPM_QOS_POLICY_C2B_CB*) AVLL_NEXT(policy_cb->c2b_tree,
                           c2b_cb->spm_policy_c2b_node))
         {
            NBB_TRC_FLOW((NBB_FORMAT "Verify c2b_cb %p", c2b_cb));
            NBB_VERIFY_MEMORY(c2b_cb, sizeof(SPM_QOS_POLICY_C2B_CB), MEM_SPM_POLICY_C2B_CB);
         }

        
    }

    NBB_TRC_EXIT();

    return;
}

/**PROC+**********************************************************************/
/* Name:                                                                     */
/*                                                                           */
/* Purpose:                                .                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_classify_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_CLASSIFY_CB *acl_cb = NULL;

    ATG_DCI_QOS_CLASSIFY_BASIC_DATA *basic_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_PORT_DATA *Port_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_ETH_HEADER *Eth_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *ipv4_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_TCP *ipTcp_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *ipv6_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_NONE_IP *noIp_cfg_cb;

    ATG_DCI_QOS_CLASSIFY_IPV4_UDP *ipUdp_cfg_cb;
    
#ifdef R8000_V3R2
    ATG_DCI_QOS_CLASSIFY_PORT_VLAN *portVlan_cfg_cb;
#endif

    NBB_TRC_ENTRY("spm_cfg_classify_cb_verify");

    for (acl_cb = (SPM_QOS_CLASSIFY_CB*) AVLL_FIRST(SHARED.qos_classify_tree);
         acl_cb != NULL;
         acl_cb = (SPM_QOS_CLASSIFY_CB*) AVLL_NEXT(SHARED.qos_classify_tree,
                       acl_cb->spm_classify_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify acl_cb %p", acl_cb));
        NBB_VERIFY_MEMORY(acl_cb, sizeof(SPM_QOS_CLASSIFY_CB), MEM_SPM_CLASSIFY_CB);
#ifdef PTN690
        spm_verify_policy_classify_mem(acl_cb NBB_CCXT);
#endif
        basic_cfg_cb = acl_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_BASIC_DATA),
                              MEM_SPM_CLASSIFY_BASIC_CB);
        }

        Port_cfg_cb = acl_cb->Port_cfg_cb;
        if(Port_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify Port_cfg_cb cb %p", Port_cfg_cb));
            NBB_VERIFY_MEMORY(Port_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_PORT_DATA),
                              MEM_SPM_CLASSIFY_PORT_CB);
        }

        Eth_cfg_cb = acl_cb->Eth_cfg_cb;
        if(Eth_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify Eth_cfg_cb cb %p", Eth_cfg_cb));
            NBB_VERIFY_MEMORY(Eth_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_ETH_HEADER),
                              MEM_SPM_CLASSIFY_ETH_CB);
        }

        ipv4_cfg_cb = acl_cb->ipv4_cfg_cb;
        if(ipv4_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_cfg_cb cb %p", ipv4_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME),
                              MEM_SPM_CLASSIFY_IPV4_CB);
        }

        ipTcp_cfg_cb = acl_cb->ipTcp_cfg_cb;
        if(ipTcp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipTcp_cfg_cb cb %p", ipTcp_cfg_cb));
            NBB_VERIFY_MEMORY(ipTcp_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_TCP),
                              MEM_SPM_CLASSIFY_IPTCP_CB);
        }

        ipv6_cfg_cb = acl_cb->ipv6_cfg_cb;
        if(ipv6_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_cfg_cb cb %p", ipv6_cfg_cb));
            NBB_VERIFY_MEMORY(ipv6_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME),
                              MEM_SPM_CLASSIFY_IPV6_CB);
        }

        noIp_cfg_cb = acl_cb->noIp_cfg_cb;
        if(noIp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify noIp_cfg_cb cb %p", noIp_cfg_cb));
            NBB_VERIFY_MEMORY(noIp_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_NONE_IP),
                              MEM_SPM_CLASSIFY_NOIP_CB);
        }

        ipUdp_cfg_cb = acl_cb->ipUdp_cfg_cb;
        if(ipUdp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipUdp_cfg_cb cb %p", ipUdp_cfg_cb));
            NBB_VERIFY_MEMORY(ipUdp_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_IPV4_UDP),
                              MEM_SPM_CLASSIFY_IPUDP_CB);
        }
        
#ifdef R8000_V3R2
        portVlan_cfg_cb = acl_cb->portVlan_cfg_cb;
        if(portVlan_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipUdp_cfg_cb cb %p", portVlan_cfg_cb));
            NBB_VERIFY_MEMORY(portVlan_cfg_cb, sizeof(ATG_DCI_QOS_CLASSIFY_PORT_VLAN),
                              MEM_SPM_CLASSIFY_PORTVLAN_CB);
        }
#endif
        
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:                                                                     */
/*                                                                           */
/* Purpose:                                .                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_action_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_ACTION_CB *action_cb = NULL;
    ATG_DCI_QOS_BEHAVIOR_BASIC_DATA *basic_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_SUPERVISE *supervise_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_SHAPPING *shaping_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_SCHEDULE *schedule_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *flow_act_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *flow_samping_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_REDIRECT *redirect_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_POLICY_RT *policy_nhi_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_PHB *phb_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_DOMAIN *domain_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_URPF *urpf_cfg_cb;
    ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE que_shaping_cfg_cb[MAX_PHB_NUM];
    ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL que_schedul_cfg_cb[MAX_PHB_NUM];
    ATG_DCI_QOS_BEHAVIOR_QUE_CONGST que_congst_cfg_cb[MAX_PHB_NUM];

    NBB_TRC_ENTRY("spm_cfg_action_cb_verify");

    for (action_cb = (SPM_QOS_ACTION_CB*) AVLL_FIRST(SHARED.qos_action_tree);
         action_cb != NULL;
         action_cb = (SPM_QOS_ACTION_CB*) AVLL_NEXT(SHARED.qos_action_tree,
                       action_cb->spm_action_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify action_cb %p", action_cb));
        NBB_VERIFY_MEMORY(action_cb, sizeof(SPM_QOS_ACTION_CB), MEM_SPM_ACTION_CB);

        basic_cfg_cb = action_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_BASIC_DATA),
                              MEM_SPM_ACTION_BASIC_CB);
        }
#ifdef PTN690
        spm_verify_policy_supervise_mem(action_cb NBB_CCXT);
#endif
        supervise_cfg_cb = action_cb->supervise_cfg_cb;
        if(supervise_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify supervise_cfg_cb cb %p", supervise_cfg_cb));
            NBB_VERIFY_MEMORY(supervise_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_SUPERVISE),
                              MEM_SPM_ACTION_SUPERVISE_CB);
        }

        shaping_cfg_cb = action_cb->shaping_cfg_cb;
        if(shaping_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify shaping_cfg_cb cb %p", shaping_cfg_cb));
            NBB_VERIFY_MEMORY(shaping_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_SHAPPING),
                              MEM_SPM_ACTION_SHAPING_CB);
        }

        schedule_cfg_cb = action_cb->schedule_cfg_cb;
        if(schedule_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify schedule_cfg_cb cb %p", schedule_cfg_cb));
            NBB_VERIFY_MEMORY(schedule_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_SCHEDULE),
                              MEM_SPM_ACTION_SCHEDULE_CB);
        }

        flow_act_cfg_cb = action_cb->flow_act_cfg_cb;
        if(flow_act_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify flow_act_cfg_cb cb %p", flow_act_cfg_cb));
            NBB_VERIFY_MEMORY(flow_act_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_FLOW_ACT),
                              MEM_SPM_ACTION_FLOW_ACT_CB);
        }

        flow_samping_cfg_cb = action_cb->flow_samping_cfg_cb;
        if(flow_samping_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify flow_samping_cfg_cb cb %p", flow_samping_cfg_cb));
            NBB_VERIFY_MEMORY(flow_samping_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL),
                              MEM_SPM_ACTION_FLOW_SAMPL_CB);
        }

        redirect_cfg_cb = action_cb->redirect_cfg_cb;
        if(redirect_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify redirect_cfg_cb cb %p", redirect_cfg_cb));
            NBB_VERIFY_MEMORY(redirect_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_REDIRECT),
                              MEM_SPM_ACTION_REDIRECT_CB);
        }

        policy_nhi_cfg_cb = action_cb->policy_nhi_cfg_cb;
        if(policy_nhi_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify policy_nhi_cfg_cb cb %p", policy_nhi_cfg_cb));
            NBB_VERIFY_MEMORY(policy_nhi_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_POLICY_RT),
                              MEM_SPM_ACTION_POLCY_RT_CB);
        }

        phb_cfg_cb = action_cb->phb_cfg_cb;
        if(phb_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify phb_cfg_cb cb %p", phb_cfg_cb));
            NBB_VERIFY_MEMORY(redirect_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_PHB),
                              MEM_SPM_ACTION_PHB_CB);
        }

        domain_cfg_cb = action_cb->domain_cfg_cb;
        if(domain_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify domain_cfg_cb cb %p", domain_cfg_cb));
            NBB_VERIFY_MEMORY(domain_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_DOMAIN),
                              MEM_SPM_ACTION_DOMAIN_CB);
        }

        urpf_cfg_cb = action_cb->urpf_cfg_cb;
        if(urpf_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify urpf_cfg_cb cb %p", urpf_cfg_cb));
            NBB_VERIFY_MEMORY(urpf_cfg_cb, sizeof(ATG_DCI_QOS_BEHAVIOR_URPF),
                              MEM_SPM_ACTION_URPF_CB);
        }
        
    }

    NBB_TRC_EXIT();

    return;
}



NBB_VOID spm_cfg_hqos_slot_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_HQOS_SLOT_CB *hqos_cfg_cb = NULL;
    ATG_DCI_HQOS_SLOT_DATA *hqos_slot_cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_hqos_slot_cb_verify");

    for (hqos_cfg_cb = (SPM_HQOS_SLOT_CB*) AVLL_FIRST(SHARED.hqos_tree);
         hqos_cfg_cb != NULL;
         hqos_cfg_cb = (SPM_HQOS_SLOT_CB*) AVLL_NEXT(SHARED.hqos_tree,
                       hqos_cfg_cb->spm_hqos_slot_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify hqos_cfg_cb %p", hqos_cfg_cb));
        NBB_VERIFY_MEMORY(hqos_cfg_cb, sizeof(SPM_HQOS_SLOT_CB), MEM_SPM_HQOS_CB);

        hqos_slot_cfg_cb = hqos_cfg_cb->data;
        if(hqos_slot_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify hqos_slot_cfg_cb cb %p", hqos_slot_cfg_cb));
            NBB_VERIFY_MEMORY(hqos_slot_cfg_cb, sizeof(ATG_DCI_HQOS_SLOT_DATA),
                              MEM_SPM_HQOS_SLOT_CB);
        }
        
    }

    NBB_TRC_EXIT();

    return;
}

NBB_VOID spm_cfg_group_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_USER_GROUP_CB *group_cfg_cb = NULL;
    ATG_DCI_USER_GROUP_BASIC_DATA *basic_cfg_cb;
    ATG_DCI_USER_GROUP_UP_QOS *up_cfg_cb;
    ATG_DCI_USER_GROUP_DOWN_QOS *down_cfg_cb;


    NBB_TRC_ENTRY("spm_cfg_group_cb_verify");

    for (group_cfg_cb = (SPM_QOS_USER_GROUP_CB*) AVLL_FIRST(SHARED.qos_user_group_tree);
         group_cfg_cb != NULL;
         group_cfg_cb = (SPM_QOS_USER_GROUP_CB*) AVLL_NEXT(SHARED.qos_user_group_tree,
                       group_cfg_cb->spm_qos_user_group_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify group_cfg_cb %p", group_cfg_cb));
        NBB_VERIFY_MEMORY(group_cfg_cb, sizeof(SPM_QOS_USER_GROUP_CB), MEM_SPM_USER_GROUP_CB);

        basic_cfg_cb = group_cfg_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_USER_GROUP_BASIC_DATA),
                              MEM_SPM_BASIC_USER_GROUP_CB);
        }

        up_cfg_cb = group_cfg_cb->up_cfg_cb;
        if(up_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify up_cfg_cb cb %p", up_cfg_cb));
            NBB_VERIFY_MEMORY(up_cfg_cb, sizeof(ATG_DCI_USER_GROUP_UP_QOS),
                              MEM_SPM_UP_USER_GROUP_CB);
        }

        down_cfg_cb = group_cfg_cb->down_cfg_cb;
        if(down_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify down_cfg_cb cb %p", down_cfg_cb));
            NBB_VERIFY_MEMORY(down_cfg_cb, sizeof(ATG_DCI_USER_GROUP_DOWN_QOS),
                              MEM_SPM_DOWN_USER_GROUP_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}

NBB_VOID spm_cfg_wred_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_WRED_CB *wred_cfg_cb = NULL;
    ATG_DCI_WRED_BASIC_DATA *basic_cfg_cb;



    NBB_TRC_ENTRY("spm_cfg_wred_cb_verify");

    for (wred_cfg_cb = (SPM_QOS_WRED_CB*) AVLL_FIRST(SHARED.qos_wred_tree);
         wred_cfg_cb != NULL;
         wred_cfg_cb = (SPM_QOS_WRED_CB*) AVLL_NEXT(SHARED.qos_wred_tree,
                       wred_cfg_cb->spm_wred_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify wred_cfg_cb %p", wred_cfg_cb));
        NBB_VERIFY_MEMORY(wred_cfg_cb, sizeof(SPM_QOS_WRED_CB), MEM_SPM_WRED_CB);

        basic_cfg_cb = wred_cfg_cb->basic_cfg_cb;
        if(basic_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify basic_cfg_cb cb %p", basic_cfg_cb));
            NBB_VERIFY_MEMORY(basic_cfg_cb, sizeof(ATG_DCI_WRED_BASIC_DATA),
                              MEM_SPM_WRED_BASIC_CB);
        }

    }

    NBB_TRC_EXIT();

    return;
}





#ifdef PTN690
NBB_VOID spm_cfg_txlsp_car_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_TXLSP_CAR_CB *cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_txlsp_car_cb_verify");

    for (cfg_cb = (SPM_QOS_TXLSP_CAR_CB*) AVLL_FIRST(SHARED.qos_txlsp_car_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_TXLSP_CAR_CB*) AVLL_NEXT(SHARED.qos_txlsp_car_tree,
                       cfg_cb->spm_qos_txlsp_car_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_TXLSP_CAR_CB), MEM_SPM_QOS_TXLSP_CAR_CB);
    }

    NBB_TRC_EXIT();

    return;
}


/**PROC+**********************************************************************/
/* Name:                                                                     */
/*                                                                           */
/* Purpose:                                .                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_pw_car_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_PW_CAR_CB  *cfg_cb  = NULL;
    SPM_QOS_INTF_PW_CB *intf_pw = NULL;

    NBB_TRC_ENTRY("spm_cfg_pw_car_cb_verify");

    for (cfg_cb = (SPM_QOS_PW_CAR_CB*) AVLL_FIRST(SHARED.qos_pw_car_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_PW_CAR_CB*) AVLL_NEXT(SHARED.qos_pw_car_tree,
                   cfg_cb->spm_qos_pw_car_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_PW_CAR_CB), MEM_SPM_QOS_PW_CAR_CB);

        for (intf_pw = (SPM_QOS_INTF_PW_CB*)AVLL_FIRST(cfg_cb->pw_instance_tree); NULL != intf_pw;
             intf_pw = (SPM_QOS_INTF_PW_CB*)AVLL_NEXT(cfg_cb->pw_instance_tree, intf_pw->spm_qos_intf_pw_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify intf_pw %p", intf_pw));
            NBB_VERIFY_MEMORY(intf_pw, sizeof(SPM_QOS_INTF_PW_CB), MEM_SPM_QOS_INTF_PW_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}

#endif




NBB_VOID spm_cfg_logic_acl_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_logic_acl_cb_verify");

    for (cfg_cb = (SPM_QOS_LOGIC_ACL_CB*) AVLL_FIRST(SHARED.qos_filter_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_LOGIC_ACL_CB*) AVLL_NEXT(SHARED.qos_filter_tree,
                       cfg_cb->spm_acl_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_LOGIC_ACL_CB), MEM_SPM_QOS_ACL_LOGIC_CB);
    }

    NBB_TRC_EXIT();

    return;
}

NBB_VOID spm_cfg_up_usr_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_UP_USR_CB *cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_logic_acl_cb_verify");

    for (cfg_cb = (SPM_QOS_UP_USR_CB*) AVLL_FIRST(SHARED.qos_up_usr_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_UP_USR_CB*) AVLL_NEXT(SHARED.qos_up_usr_tree,
                       cfg_cb->spm_qos_up_usr_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_UP_USR_CB), MEM_SPM_QOS_UP_USR_CB);
    }

    NBB_TRC_EXIT();
    return;
}


NBB_VOID spm_cfg_flow_up_usr_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_FLOW_UP_USR_CB *cfg_cb = NULL;

    NBB_TRC_ENTRY("spm_cfg_flow_up_usr_verify");

    for (cfg_cb = (SPM_QOS_FLOW_UP_USR_CB*) AVLL_FIRST(SHARED.qos_flow_up_usr_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_FLOW_UP_USR_CB*) AVLL_NEXT(SHARED.qos_flow_up_usr_tree,
                   cfg_cb->spm_qos_flow_up_usr_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_FLOW_UP_USR_CB), MEM_SPM_QOS_FLOW_UP_USR_CB);
    }

    NBB_TRC_EXIT();
    return;
}

NBB_VOID spm_cfg_logic_classify_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_FLOW_CLASSIFY_CB *cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_logic_classify_cb_verify");

    for (cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*) AVLL_FIRST(SHARED.qos_flow_classify_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB*) AVLL_NEXT(SHARED.qos_flow_classify_tree,
                       cfg_cb->spm_qos_flow_classify_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_FLOW_CLASSIFY_CB), MEM_SPM_QOS_FLOW_CLASSIFY_CB);
    }

    NBB_TRC_EXIT();

    return;
}

NBB_VOID spm_cfg_port_wred_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_PORT_WRED_CB *cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_port_wred_cb_verify");

    for (cfg_cb = (SPM_PORT_WRED_CB*) AVLL_FIRST(SHARED.port_wred_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_PORT_WRED_CB*) AVLL_NEXT(SHARED.port_wred_tree,
                       cfg_cb->spm_port_wred_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_PORT_WRED_CB), MEM_SPM_PORT_WRED_CB);
    }

    NBB_TRC_EXIT();

    return;
}


NBB_VOID spm_cfg_defend_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DEFEND_CB *cfg_cb = NULL;


    NBB_TRC_ENTRY("spm_cfg_defend_cb_verify");

    for (cfg_cb = (SPM_QOS_DEFEND_CB*) AVLL_FIRST(SHARED.qos_defend_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DEFEND_CB*) AVLL_NEXT(SHARED.qos_defend_tree,
                       cfg_cb->spm_defend_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify cfg_cb %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_DEFEND_CB), MEM_SPM_DEFEND_CB);
    }

    NBB_TRC_EXIT();

    return;
}


NBB_VOID spm_cfg_defend_policy_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DEFEND_POLICY_CB *cfg_cb = NULL;
    SPM_QOS_DEFEND_APPERC_CB *pcb = NULL;
    SPM_QOS_DEFEND_USER_DEF_CB *npb = NULL;


    NBB_TRC_ENTRY("spm_cfg_defend_policy_cb_verify");

    for (cfg_cb = (SPM_QOS_DEFEND_POLICY_CB*) AVLL_FIRST(SHARED.qos_defend_policy_tree);
         cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DEFEND_POLICY_CB*) AVLL_NEXT(SHARED.qos_defend_policy_tree,
                       cfg_cb->spm_defend_policy_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify SPM_QOS_DEFEND_POLICY_CB %p", cfg_cb));
        NBB_VERIFY_MEMORY(cfg_cb, sizeof(SPM_QOS_DEFEND_POLICY_CB), MEM_SPM_DEFEND_POLICY_CB);


        for (pcb = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_FIRST(cfg_cb->apperc_tree);
            pcb != NULL;
            pcb = (SPM_QOS_DEFEND_APPERC_CB *)AVLL_NEXT(cfg_cb->apperc_tree, pcb->spm_defend_apperc_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify SPM_QOS_DEFEND_APPERC_CB %p", pcb));
            NBB_VERIFY_MEMORY(pcb, sizeof(SPM_QOS_DEFEND_APPERC_CB),MEM_SPM_DEFEND_APPERC_CB);
        }


        for (npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_FIRST(cfg_cb->user_def_tree); npb != NULL;
             npb = (SPM_QOS_DEFEND_USER_DEF_CB *)AVLL_NEXT(cfg_cb->user_def_tree, npb->spm_defend_user_def_node))
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify SPM_QOS_DEFEND_USER_DEF_CB %p", npb));
            NBB_VERIFY_MEMORY(npb, sizeof(SPM_QOS_DEFEND_USER_DEF_CB),MEM_SPM_DEFEND_USR_DEF_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}


#endif

/**PROC+**********************************************************************/
/* Name:                                                                     */
/*                                                                           */
/* Purpose:                                .                                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_qos_cfg_cb_verfify(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_qos_cfg_cb_verfify");


/*

    spm_cfg_ds_domain_cb_verify( NBB_CXT);

    

    spm_cfg_hqos_slot_cb_verify( NBB_CXT);

    spm_cfg_group_cb_verify( NBB_CXT);

    spm_cfg_wred_cb_verify( NBB_CXT);


#ifdef PTN690
    spm_cfg_txlsp_car_cb_verify( NBB_CXT);

    spm_cfg_pw_car_cb_verify( NBB_CXT);
#endif


    spm_cfg_up_usr_verify(NBB_CXT);

    spm_cfg_flow_up_usr_verify(NBB_CXT);

    spm_cfg_port_wred_cb_verify(NBB_CXT);


    spm_cfg_hqos_cb_verify(NBB_CXT);
    
*/
    spm_twamp_ipv4_cb_verfify();
    spm_twamp_ipv6_cb_verfify();
    
    spm_cfg_acl_cb_verify();
    spm_cfg_logic_acl_cb_verify();
    spm_cfg_policy_cb_verify();
    spm_cfg_logic_classify_cb_verify();
    spm_cfg_classify_cb_verify();
    spm_cfg_action_cb_verify();
    
    spm_qos_defend_verify();

    NBB_TRC_EXIT();

    return;   
}



/* bfd全局变量verify函数 */
NBB_VOID spm_cfg_ds_bfd_cb_verify(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_cfg_ds_bfd_cb_verify");

    if (NULL != g_pulBfdTreebase)
    {
        NBB_VERIFY_MEMORY(g_pulBfdTreebase, sizeof(NBB_ULONG)*g_usBfdTreeLevel, MEM_SPM_BFD_PUL_TREE_BASE_CB);
    }

    if (NULL != g_pucSynIdInfo)
    {
        NBB_VERIFY_MEMORY(g_pucSynIdInfo, g_ulSynIdInfoLth, MEM_SPM_BFD_PUC_SYN_ID_INFO_CB);
    }

    if (NULL != g_pucAlarmInfo)
    {
        NBB_VERIFY_MEMORY(g_pucAlarmInfo, g_usAlarmInfoLenth, MEM_SPM_BFD_PUC_ALARM_INFO_CB);
    }

    if (NULL != g_pulFwdTreebase)
    {
        NBB_VERIFY_MEMORY(g_pulFwdTreebase, sizeof(NBB_ULONG)*g_usFwdTreeLevel, MEM_SPM_BFD_PUL_FWD_TREE_BASE_CB);
    }

    if (NULL != g_stTreeFwdNodeSet.pstNodeArr)
    {
        NBB_VERIFY_MEMORY(g_stTreeFwdNodeSet.pstNodeArr, 
            sizeof(SPM_OAMTREE_NODE) * g_usFwdTreeNodeMaxnum, MEM_SPM_TPOAM_TREE_CB);
    }

    if (NULL != g_stTreeBfdNodeSet.pstNodeArr)
    {
        NBB_VERIFY_MEMORY(g_stTreeBfdNodeSet.pstNodeArr, 
            sizeof(SPM_OAMTREE_NODE) * g_usBfdTreeNodeMaxnum, MEM_SPM_TPOAM_TREE_CB);
    }

    
    if (NULL != g_pulNniTreebase)
    {
        NBB_VERIFY_MEMORY(g_pulNniTreebase, sizeof(NBB_ULONG)*g_usNniTreeLevel, MEM_SPM_BFD_PUL_NNI_TREE_BASE_CB);
    }

    
    if (NULL != g_stTreeNniNodeSet.pstNodeArr)
    {
        NBB_VERIFY_MEMORY(g_stTreeNniNodeSet.pstNodeArr, 
            sizeof(SPM_OAMTREE_NODE) * g_usNniTreeNodeMaxnum, MEM_SPM_TPOAM_TREE_CB);
    }
    
    NBB_TRC_EXIT();
    return;
}

/* bfd 配置verify函数 */
NBB_VOID spm_cfg_bfd_cfg_cb_verify(NBB_CXT_T NBB_CXT)
{
    NBB_USHORT usLoopi;
    SPM_BFD_GLOBAL *pstBfdCfg = NULL;
    SPM_FWD_GLOBAL *pstFwdInfo = NULL;
    SPM_BFD_IP_GLOBAL *pstBfdIpCfg = NULL;
    SPM_BFD_IPV6_GLOBAL  *pstBfdIpv6Cfg = NULL;
    BFD_ALARM_FAIL_INFO *alarm_fail_info = NULL;
    
    NBB_TRC_ENTRY("spm_cfg_bfd_cfg_cb_verify");
    
    for (pstBfdCfg = (SPM_BFD_GLOBAL*) AVLL_FIRST(g_stBfdGlbTreeByDis);
         pstBfdCfg != NULL;
         pstBfdCfg = (SPM_BFD_GLOBAL*) AVLL_NEXT(g_stBfdGlbTreeByDis,
                       pstBfdCfg->spm_bfd_node))
    {
        NBB_VERIFY_MEMORY(pstBfdCfg, sizeof(SPM_BFD_GLOBAL), MEM_SPM_BFD_GLOBAL_CB);
    }
         
    for (pstBfdIpCfg = (SPM_BFD_IP_GLOBAL*) AVLL_FIRST(g_stBfdGlbTreeByIp);
      pstBfdIpCfg != NULL;
      pstBfdIpCfg = (SPM_BFD_IP_GLOBAL*) AVLL_NEXT(g_stBfdGlbTreeByIp,
                    pstBfdIpCfg->spm_bfdip_node))
    {
        NBB_VERIFY_MEMORY(pstBfdIpCfg, sizeof(SPM_BFD_IP_GLOBAL), MEM_SPM_BFD_IP_GLOBAL_CB);
    }

    for (pstBfdIpv6Cfg = (SPM_BFD_IPV6_GLOBAL*) AVLL_FIRST(g_stBfdGlbTreeByIpv6);
      pstBfdIpv6Cfg != NULL;
      pstBfdIpv6Cfg = (SPM_BFD_IPV6_GLOBAL*) AVLL_NEXT(g_stBfdGlbTreeByIpv6,
                    pstBfdIpv6Cfg->spm_bfdipv6_node))
    {
        NBB_VERIFY_MEMORY(pstBfdIpv6Cfg, sizeof(SPM_BFD_IPV6_GLOBAL), MEM_SPM_BFD_IPV6_GLOBAL_CB);
    }
         
     for (alarm_fail_info = (BFD_ALARM_FAIL_INFO*) AVLL_FIRST(g_stAlarmFailTree);
       alarm_fail_info != NULL;
       alarm_fail_info = (BFD_ALARM_FAIL_INFO*) AVLL_NEXT(g_stAlarmFailTree,
                     alarm_fail_info->alarm_fail_node))
     {
         NBB_VERIFY_MEMORY(alarm_fail_info, sizeof(BFD_ALARM_FAIL_INFO), MEM_SPM_BFD_ALARM_FAIL_TREE_CB);
     }

    for (usLoopi = 0; usLoopi < BFD_MAX_SESNUM; usLoopi++)
    {
        if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdLsp)
        {
            NBB_VERIFY_MEMORY(g_stBfdlocal[usLoopi].pstBasicBfdLsp, 
                sizeof(ATG_DCI_BFD_LDP_LSP_DATA), MEM_SPM_BFD_LDP_LSP_DATA_CB);
        } 
        
        if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdPw)
        {
            NBB_VERIFY_MEMORY(g_stBfdlocal[usLoopi].pstBasicBfdPw, 
                sizeof(ATG_DCI_BFD_PW_DATA), MEM_SPM_BFD_PW_DATA_CB);
        } 
        
        if (NULL != g_stBfdlocal[usLoopi].pstBasicBfdTunnel)
        {
            NBB_VERIFY_MEMORY(g_stBfdlocal[usLoopi].pstBasicBfdTunnel, 
                sizeof(ATG_DCI_BFD_RSVP_LSP_DATA), MEM_SPM_BFD_RSVP_LSP_DATA_CB);
        } 
    }
    
    NBB_TRC_EXIT();
    return;
}

NBB_VOID spm_bfd_cfg_cb_verfify(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_bfd_cfg_cb_verfify");
    
    spm_cfg_ds_bfd_cb_verify(NBB_CXT);
    spm_cfg_bfd_cfg_cb_verify(NBB_CXT);
    
    NBB_TRC_EXIT();
    return;
}

#if 1



NBB_VOID spm_l3_verify_ftn(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	FTN_TAB		*uftn = NULL;

  	PEERFTN		*peerftn = NULL;

	PWVC		*pwvc = NULL;

	PWVRF		*pwvrf = NULL;

	PWMC		*pwmc = NULL;

	PEEREPTAB	*peerp = NULL;

	ECMPNODE	*ecmpnode = NULL;

	NBB_TRC_ENTRY("spm_l3_verify_ftn");

	uftn = (FTN_TAB *)AVLL_FIRST(SHARED.ftn_tree);
	while (uftn != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify ftn  %p", uftn));
		NBB_VERIFY_MEMORY(uftn, sizeof(FTN_TAB),MEM_SPM_FTN_TAB_NODE_CB);
		uftn = (FTN_TAB *)AVLL_NEXT( SHARED.ftn_tree, uftn->ftn_node);
	}

	peerftn =  (PEERFTN *)AVLL_FIRST( SHARED.peftn_tree );
	while (peerftn != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify peerftn  %p", peerftn));
		NBB_VERIFY_MEMORY(peerftn, sizeof(PEERFTN),MEM_SPM_FTN_TAB_PEER_CB);
		
		pwvc =  (PWVC *)AVLL_FIRST( peerftn->vp_tree );
		while (pwvc != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify pwvc  %p", pwvc));
			NBB_VERIFY_MEMORY(pwvc, sizeof(PWVC),MEM_SPM_FTN_TAB_VC_CB);
			pwvc = (PWVC *)AVLL_NEXT( peerftn->vp_tree, pwvc->vc_node);
		}
		
		pwvrf =  (PWVRF *)AVLL_FIRST( peerftn->vrf_tree );
		while (pwvrf != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify pwvrf  %p", pwvrf));
			NBB_VERIFY_MEMORY(pwvrf, sizeof(PWVRF),MEM_SPM_FTN_TAB_VRF_CB);
			pwvrf = (PWVRF *)AVLL_NEXT( peerftn->vrf_tree, pwvrf->vrf_node);
		}
		
		pwmc =  (PWMC *)AVLL_FIRST( peerftn->mcid_tree );
		while (pwmc != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify pwmc  %p", pwmc));
			NBB_VERIFY_MEMORY(pwmc, sizeof(PWMC),MEM_SPM_FTN_TAB_MC_CB);
			pwmc = (PWMC *)AVLL_NEXT( peerftn->mcid_tree, pwmc->mc_node);
		}
		
		peerftn = (PEERFTN *)AVLL_NEXT( SHARED.peftn_tree, peerftn->pe_node);
	}

	peerp =  (PEEREPTAB *)AVLL_FIRST( SHARED.peecmp_tree );
	while (peerp != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify peerp  %p", peerp));
		NBB_VERIFY_MEMORY(peerp, sizeof(PEEREPTAB),MEM_SPM_FTN_TAB_PEECMP_CB);
		
		ecmpnode =  (ECMPNODE *)AVLL_FIRST( peerp->ecmpnode_tree);
		while (ecmpnode != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify ecmpnode  %p", ecmpnode));
			NBB_VERIFY_MEMORY(ecmpnode, sizeof(ECMPNODE),MEM_SPM_FTN_TAB_ECMPNODE_CB);
			ecmpnode = (ECMPNODE *)AVLL_NEXT( peerp->ecmpnode_tree, ecmpnode->en_node);
		}
	
		peerp = (PEEREPTAB *)AVLL_NEXT( SHARED.peecmp_tree, peerp->pp_node);
	}



	NBB_TRC_EXIT();

	return;
}


NBB_VOID spm_l3_verify_ilm(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	ILM_TAB		*ilms = NULL;
	SHASH_NODE 	*node = NULL;
	NBB_ULONG 	i = 0;

	NBB_TRC_ENTRY("spm_l3_verify_ilm");

	if( SHARED.ilmhash == NULL )
	{
		goto  EXIT_LABEL;
	}
	NBB_VERIFY_MEMORY(SHARED.ilmhash,sizeof(SHASH_TABLE),MEM_SPM_HASH_TABLE_CB);
	NBB_VERIFY_MEMORY(SHARED.ilmhash->index,sizeof(SHASH_TABLE *)*16000,MEM_SPM_HASH_INDEX_CB);
	for (i = 0; i < 16000; i++)
	{
		node = SHARED.ilmhash->index[i];
		while(node != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify ilm node  %p", node));
			NBB_VERIFY_MEMORY(node, sizeof(SHASH_NODE),MEM_SPM_HASH_NODE_CB);
			if(node->data != NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "Verify ilm  %p", node));
				NBB_VERIFY_MEMORY(node->data, sizeof(ILM_TAB),MEM_SPM_ILM_TAB_CB);
			}
			node = node->next;
		}	
	}
	
	EXIT_LABEL:
	NBB_TRC_EXIT();

	return;
}



NBB_VOID spm_l3_verify_crlsprx(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	CRRXLSP		*rxlsp = NULL;
	SHASH_NODE 	*node = NULL;
	NBB_ULONG 	i = 0;

	NBB_TRC_ENTRY("spm_l3_verify_crlsprx");

	if( SHARED.crlsprxhash == NULL )
	{
		goto  EXIT_LABEL;
	}
	NBB_VERIFY_MEMORY(SHARED.crlsprxhash,sizeof(SHASH_TABLE),MEM_SPM_HASH_TABLE_CB);
	NBB_VERIFY_MEMORY(SHARED.crlsprxhash->index,sizeof(SHASH_TABLE *)*16000,MEM_SPM_HASH_INDEX_CB);
	for (i = 0; i < 16000; i++)
	{
		node = SHARED.crlsprxhash->index[i];
		while(node != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify rxlsp node  %p", node));
			NBB_VERIFY_MEMORY(node, sizeof(SHASH_NODE),MEM_SPM_HASH_NODE_CB);
			if(node->data != NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "Verify rxlsp  %p", node));
				NBB_VERIFY_MEMORY(node->data, sizeof(CRRXLSP),MEM_SPM_CR_LSP_RX_CB);
			}
			node = node->next;
		}	
	}
	
	EXIT_LABEL:
	NBB_TRC_EXIT();

	return;
}

/******************************************************************************
 * FunctionName 	: 	spm_l3_verify_crlsptx
 * Author		: 	    wjhe	
 * CreateDate		:	2015-12-1
 * Description		:   
 * InputParam	:	    
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	       
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_l3_verify_crlsptx(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	CRTXLSP		*txlsp = NULL;
	SHASH_NODE 	*node = NULL;
	NBB_ULONG 	i = 0;
	RSVPCRLSP   *rsvplsp = NULL;
	PWVPN       *pw_vpn = NULL;
	PWVC		*pwvc = NULL;
	PWMC		*pwmc = NULL;

	NBB_TRC_ENTRY("spm_l3_verify_crlsptx");

	if( SHARED.crlsptxhash == NULL )
	{
		goto  EXIT_LABEL;
	}
	NBB_VERIFY_MEMORY(SHARED.crlsptxhash,sizeof(SHASH_TABLE),MEM_SPM_HASH_TABLE_CB);
	NBB_VERIFY_MEMORY(SHARED.crlsptxhash->index,sizeof(SHASH_TABLE *)*16000,MEM_SPM_HASH_INDEX_CB);
	for (i = 0; i < 16000; i++)
	{
		node = SHARED.crlsptxhash->index[i];
		while(node != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify txlsp node  %p", node));
			NBB_VERIFY_MEMORY(node, sizeof(SHASH_NODE),MEM_SPM_HASH_NODE_CB);
			if(node->data != NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "Verify txlsp  %p", node));
				NBB_VERIFY_MEMORY(node->data, sizeof(CRTXLSP),MEM_SPM_CR_LSP_TX_CB);
			}
			node = node->next;
		}	
	}
	rsvplsp =  (RSVPCRLSP *)AVLL_FIRST( SHARED.rsvplsp_tree );
	while (rsvplsp != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify rsvplsp  %p", rsvplsp));
		NBB_VERIFY_MEMORY(rsvplsp, sizeof(RSVPCRLSP),MEM_SPM_CR_LSP_TAB_RSVP_CB);
		
		pwvc =  (PWVC *)AVLL_FIRST( rsvplsp->vp_tree );
		while (pwvc != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify pwvc  %p", pwvc));
			NBB_VERIFY_MEMORY(pwvc, sizeof(PWVC),MEM_SPM_FTN_TAB_VC_CB);
			pwvc = (PWVC *)AVLL_NEXT( rsvplsp->vp_tree, pwvc->vc_node);
		}

		pw_vpn =  (PWVPN *)AVLL_FIRST( rsvplsp->vrfpeer_tree);
		while (pw_vpn != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify pw vpn  %p", pw_vpn));
			NBB_VERIFY_MEMORY(pw_vpn, sizeof(PWVPN),MEM_SPM_CR_LSP_TAB_VPN_CB);
			pw_vpn = (PWVPN *)AVLL_NEXT( rsvplsp->vrfpeer_tree, pw_vpn->vrf_node);
		}
		
		pwmc =  (PWMC *)AVLL_FIRST( rsvplsp->mcid_tree );
		while (pwmc != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify pwmc  %p", pwmc));
			NBB_VERIFY_MEMORY(pwmc, sizeof(PWMC),MEM_SPM_FTN_TAB_MC_CB);
			pwmc = (PWMC *)AVLL_NEXT( rsvplsp->mcid_tree, pwmc->mc_node);
		}
		
		rsvplsp = (RSVPCRLSP *)AVLL_NEXT( SHARED.rsvplsp_tree, rsvplsp->rsvp_node);

	}
	
	EXIT_LABEL:
	NBB_TRC_EXIT();

	return;
}

NBB_VOID spm_l3_verify_ucroute(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	UNIPORT		*uniports = NULL;
	MPEERFRR	*mpeerfrr = NULL;
	BPEERFRR	*bpeerfrr = NULL;
	ECMPTAB		*ecmp = NULL;
	BFDFRR		*bfdfrrp = NULL;
	IPFRR		*ipfrrp = NULL;

	NBB_TRC_ENTRY("spm_l3_verify_ucroute");

	uniports =  (UNIPORT *)AVLL_FIRST( SHARED.outuni );
	while (uniports != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify ucroute uni  %p", uniports));
		NBB_VERIFY_MEMORY(uniports, sizeof(UNIPORT),MEM_SPM_VRFUROUTE_UNI_CB);
		uniports = (UNIPORT *)AVLL_NEXT( SHARED.outuni, uniports->uni_node);
	}

	mpeerfrr =  (MPEERFRR *)AVLL_FIRST( SHARED.peer_tree );
	while (mpeerfrr != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify mpeer  %p", mpeerfrr));
		NBB_VERIFY_MEMORY(mpeerfrr, sizeof(MPEERFRR),MEM_SPM_VRFUROUTE_MPEER_CB);
		
		bpeerfrr =  (BPEERFRR *)AVLL_FIRST( mpeerfrr->bpeer_tree );
		while (bpeerfrr != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify bpeer  %p", bpeerfrr));
			NBB_VERIFY_MEMORY(bpeerfrr, sizeof(BPEERFRR),MEM_SPM_VRFUROUTE_BPEER_CB);
			bpeerfrr = (BPEERFRR *)AVLL_NEXT( mpeerfrr->bpeer_tree, bpeerfrr->bp_node);
		}
		mpeerfrr = (MPEERFRR *)AVLL_NEXT( SHARED.peer_tree, mpeerfrr->mp_node);
	}

	ecmp =  (ECMPTAB *)AVLL_FIRST( SHARED.ecmp_tree );
	while (ecmp != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify ucroute ecmp  %p", ecmp));
		NBB_VERIFY_MEMORY(ecmp, sizeof(ECMPTAB),MEM_SPM_VRFUROUTE_ECMP_CB);
		ecmp = (ECMPTAB *)AVLL_NEXT( SHARED.ecmp_tree, ecmp->ep_node);
	}

	bfdfrrp =  (BFDFRR *)AVLL_FIRST( SHARED.bfdfrr_tree );
	while (bfdfrrp != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify ucroute bfdfrrp  %p", bfdfrrp));
		NBB_VERIFY_MEMORY(bfdfrrp, sizeof(BFDFRR),MEM_SPM_VRFUROUTE_BFDFRR_CB);
		bfdfrrp = (BFDFRR *)AVLL_NEXT( SHARED.bfdfrr_tree, bfdfrrp->bf_node);
	}

	ipfrrp =  (IPFRR *)AVLL_FIRST( SHARED.ipfrr_tree );
	while (ipfrrp != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify ucroute ipfrrp  %p", ipfrrp));
		NBB_VERIFY_MEMORY(ipfrrp, sizeof(IPFRR),MEM_SPM_VRFUROUTE_IPFRR_CB);
		ipfrrp = (IPFRR *)AVLL_NEXT( SHARED.ipfrr_tree, ipfrrp->ipf_node);
	}

	NBB_TRC_EXIT();

	return;
}

NBB_VOID spm_l3_verify_mcroute(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	MCROUTE	*mroute = NULL;

	NBB_TRC_ENTRY("spm_l3_verify_mcroute");

	mroute = (MCROUTE *)AVLL_FIRST(SHARED.mcroute_tree);
	while (mroute != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify mcroute  %p", mroute));
		NBB_VERIFY_MEMORY(mroute, sizeof(MCROUTE),MEM_SPM_VRFMROUTE_NODE_CB);
		mroute = (MCROUTE *)AVLL_NEXT( SHARED.mcroute_tree, mroute->route_node);
	}
	
	NBB_TRC_EXIT();

	return;
}


NBB_VOID spm_l3_verify_tunnelch(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	TUNNELCH	*tunnels = NULL;
  	PWTXNODE	*pwnode = NULL;
	SHASH_NODE 	*node = NULL;
	NBB_ULONG 	i = 0;

	NBB_TRC_ENTRY("spm_l3_verify_tunnelch");

	if( SHARED.tunnelchhash == NULL )
	{
		goto  EXIT_LABEL;
	}
	NBB_VERIFY_MEMORY(SHARED.tunnelchhash,sizeof(SHASH_TABLE),MEM_SPM_HASH_TABLE_CB);
	NBB_VERIFY_MEMORY(SHARED.tunnelchhash->index,sizeof(SHASH_TABLE *)*8000,MEM_SPM_HASH_INDEX_CB);
	for (i = 0; i < 8000; i++)
	{
		node = SHARED.tunnelchhash->index[i];
		while(node != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify tunnelch node  %p", node));
			NBB_VERIFY_MEMORY(node, sizeof(SHASH_NODE),MEM_SPM_HASH_NODE_CB);
			if(node->data != NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "Verify tunnelch  %p", node));
				NBB_VERIFY_MEMORY(node->data, sizeof(TUNNELCH),MEM_SPM_TUNNELCH_CB);
				
				tunnels = (TUNNELCH *)(node->data);
				
				pwnode =  (PWTXNODE *)AVLL_FIRST(tunnels->pwtree);
				while (pwnode != NULL)
				{
					NBB_TRC_FLOW((NBB_FORMAT "Verify pwnode  %p", pwnode));
					NBB_VERIFY_MEMORY(pwnode, sizeof(PWTXNODE),MEM_SPM_VRFUROUTE_PWNODE_CB);
					
					pwnode = (PWTXNODE *)AVLL_NEXT( tunnels->pwtree, pwnode->pw_node);
				}
			}
			node = node->next;
		}	
	}
	
	EXIT_LABEL:
	NBB_TRC_EXIT();

	return;
}


NBB_VOID spm_l3_verify_lspprot(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	LSPPROT		*lspprot = NULL;
	SHASH_NODE 	*node = NULL;
	NBB_ULONG 	i = 0;

	NBB_TRC_ENTRY("spm_l3_verify_lspprot");

	if( SHARED.lspprothash == NULL )
	{
		goto  EXIT_LABEL;
	}
	NBB_VERIFY_MEMORY(SHARED.lspprothash,sizeof(SHASH_TABLE),MEM_SPM_HASH_TABLE_CB);
	NBB_VERIFY_MEMORY(SHARED.lspprothash->index,sizeof(SHASH_TABLE *)*8000,MEM_SPM_HASH_INDEX_CB);
	for (i = 0; i < 8000; i++)
	{
		node = SHARED.lspprothash->index[i];
		while(node != NULL)
		{
			NBB_TRC_FLOW((NBB_FORMAT "Verify lspprot node  %p", node));
			NBB_VERIFY_MEMORY(node, sizeof(SHASH_NODE),MEM_SPM_HASH_NODE_CB);
			if(node->data != NULL)
			{
				NBB_TRC_FLOW((NBB_FORMAT "Verify lspprot %p", node));
				NBB_VERIFY_MEMORY(node->data, sizeof(LSPPROT),MEM_SPM_LSPPROT_CB);
			}
			node = node->next;
		}	
	}
	
	EXIT_LABEL:
	NBB_TRC_EXIT();

	return;
}

NBB_VOID spm_l3_verify_vrfins(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
	VRFINS	*vrfins = NULL;

	NBB_TRC_ENTRY("spm_l3_verify_vrfins");

	vrfins = (VRFINS *)AVLL_FIRST(SHARED.vrfins_tree);
	while (vrfins != NULL)
	{
		NBB_TRC_FLOW((NBB_FORMAT "Verify vrfins  %p", vrfins));
		NBB_VERIFY_MEMORY(vrfins, sizeof(VRFINS),MEM_SPM_VRFINS_CB);
		vrfins = (VRFINS *)AVLL_NEXT( SHARED.vrfins_tree, vrfins->ins_node);
	}
	
	NBB_TRC_EXIT();

	return;
}


NBB_VOID spm_l3_verify_mem( NBB_CXT_T NBB_CXT )
{

	NBB_TRC_ENTRY("spm_l3_verify_mem");

	spm_l3_verify_ftn(NBB_CXT);
	
	spm_l3_verify_ilm(NBB_CXT);
	spm_l3_verify_crlsprx(NBB_CXT);
	spm_l3_verify_crlsptx(NBB_CXT);
	
	spm_l3_verify_ucroute(NBB_CXT);
	spm_l3_verify_mcroute(NBB_CXT);
	
	spm_l3_verify_tunnelch(NBB_CXT);
	spm_l3_verify_lspprot(NBB_CXT);

	spm_l3_verify_vrfins(NBB_CXT);

	NBB_TRC_EXIT();

	return;
}


#endif


/*oam全局变量verify函数*/
NBB_VOID spm_cfg_oam_cb_verify(NBB_CXT_T NBB_CXT)
{
    spm_cfg_oam_basic_cb_verify(NBB_CXT);
    spm_cfg_oam_rsvp_cb_verify(NBB_CXT);
    spm_cfg_oam_pw_cb_verify(NBB_CXT);
    spm_cfg_oam_global_cb_verify(NBB_CXT);
    spm_cfg_oam_list_verify(NBB_CXT);
    spm_cfg_oam_vs_cb_verify(NBB_CXT);
    spm_cfg_oam_wrapping_cb_verify(NBB_CXT);
    return;
}
/**PROC+**********************************************************************/
/* Name:      spm_cfg_nf_aggr_cb_verify                                         */
/*                                                                           */
/* Purpose:   遍历nf_aggr配置的树逐一进行验证.                                  */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_nf_aggr_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_NF_AGGR_CB *nf_aggr_cfg_cb = NULL;
    
     /* 聚合模式。*/
    ATG_DCI_NF_AGGR_AGGREGATION *aggregation_cfg;
  
    /* 输出版本。 */
    ATG_DCI_NF_AGGR_EXPORT_VERSION *export_version_cfg;

    /* 掩码. */
    ATG_DCI_NF_AGGR_IPV4_MASK *ipv4mask_cfg;

    /* 掩码. */
    ATG_DCI_NF_AGGR_IPV6_MASK *ipv6mask_cfg;
  
    /* 使能。*/
    ATG_DCI_NF_AGGR_ENABLE *enable_cfg;

    NBB_TRC_ENTRY("spm_cfg_nf_aggr_cb_verify");

    for (nf_aggr_cfg_cb = (SPM_NF_AGGR_CB*) AVLL_FIRST(SHARED.nf_aggr_tree);
         nf_aggr_cfg_cb != NULL;
         nf_aggr_cfg_cb = (SPM_NF_AGGR_CB*) AVLL_NEXT(SHARED.nf_aggr_tree,
                       nf_aggr_cfg_cb->nf_aggr_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify nf_aggr_cfg_cb %p", nf_aggr_cfg_cb));
        NBB_VERIFY_MEMORY(nf_aggr_cfg_cb, sizeof(SPM_NF_AGGR_CB), MEM_SPM_NF_AGGR_CB);

        aggregation_cfg = nf_aggr_cfg_cb->aggregation_cfg;
        if(aggregation_cfg != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify aggregation_cfg cb %p", aggregation_cfg));
            NBB_VERIFY_MEMORY(aggregation_cfg, sizeof(ATG_DCI_NF_AGGR_AGGREGATION),
                              MEM_SPM_NF_AGGR_AGGREGATION_CB);
        }

        export_version_cfg = nf_aggr_cfg_cb->export_version_cfg;
        if(export_version_cfg != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify export_version_cfg cb %p", export_version_cfg));
            NBB_VERIFY_MEMORY(export_version_cfg, sizeof(ATG_DCI_NF_AGGR_EXPORT_VERSION),
                              MEM_SPM_NF_AGGR_EXPORT_VERSION_CB);
        }

        ipv4mask_cfg = nf_aggr_cfg_cb->ipv4mask_cfg;
        if(ipv4mask_cfg != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4mask_cfg cb %p", ipv4mask_cfg));
            NBB_VERIFY_MEMORY(ipv4mask_cfg, sizeof(ATG_DCI_NF_AGGR_IPV4_MASK),
                              MEM_SPM_NF_AGGR_IPV4_MASK_CB);
        }

        ipv6mask_cfg = nf_aggr_cfg_cb->ipv6mask_cfg;
        if(ipv6mask_cfg != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6mask_cfg cb %p", ipv6mask_cfg));
            NBB_VERIFY_MEMORY(ipv6mask_cfg, sizeof(ATG_DCI_NF_AGGR_IPV6_MASK),
                              MEM_SPM_NF_AGGR_IPV6_MASK_CB);
        }
        
        enable_cfg = nf_aggr_cfg_cb->enable_cfg;
        if(enable_cfg != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify enable_cfg cb %p", enable_cfg));
            NBB_VERIFY_MEMORY(enable_cfg, sizeof(ATG_DCI_NF_AGGR_ENABLE),
                              MEM_SPM_NF_AGGR_ENABLE_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}
/**PROC+**********************************************************************/
/* Name:      spm_cfg_ip_nf_cb_verify                                         */
/*                                                                           */
/* Purpose:   遍历ip_nf配置的树逐一进行验证.                                  */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    None.                                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_cfg_ip_nf_cb_verify(NBB_CXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_IP_NF_CB *ip_nf_cfg_cb = NULL;
    /* IPV4NetFlow输出目的主机 */
    ATG_DCI_IP_NF_IPV4_DSTHOST *ipv4_dst_cfg_cb;

    /*IPV4NetFlow源IP地址配置*/
    ATG_DCI_IP_NF_IPV4_SRCIP *ipv4_src_cfg_cb;

    /*IPV4NetFlow输出版本 */
    ATG_DCI_IP_NF_IPV4_VERSION *ipv4_version_cfg_cb;

    /*IPV6输出目的主机*/
    ATG_DCI_IP_NF_IPV6_DSTHOST *ipv6_dst_cfg_cb;

    /*IPV6源IP地址配置*/
    ATG_DCI_IP_NF_IPV6_SRCIP *ipv6_src_cfg_cb;

    /*IPV6输出版本*/
    ATG_DCI_IP_NF_IPV6_VERSION *ipv6_version_cfg_cb;

    /*TCP标志检查 */
    ATG_DCI_IP_NF_CHECK_TCP *tcp_cfg_cb;

    /*老化*/
    ATG_DCI_IP_NF_TIMEOUT *timeout_cfg_cb;

    /*流采样进程控制*/
    ATG_DCI_IP_NF_START *start_cfg_cb;

    /*流采样输出信息控制*/
    ATG_DCI_IP_NF_EXPORT_INFO *info_cfg_cb;

    /* 灵活流采样使能. */
    ATG_DCI_IP_NF_ENABLE *enable_cfg;

    /* 灵活流采样关键字控制. */
    ATG_DCI_IP_NF_FLEXNFKEY *flexnfkey_cfg;

    NBB_TRC_ENTRY("spm_cfg_ip_nf_cb_verify");

    for (ip_nf_cfg_cb = (SPM_IP_NF_CB*) AVLL_FIRST(SHARED.ip_nf_tree);
         ip_nf_cfg_cb != NULL;
         ip_nf_cfg_cb = (SPM_IP_NF_CB*) AVLL_NEXT(SHARED.ip_nf_tree,
                       ip_nf_cfg_cb->ip_nf_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Verify ip_nf_cfg_cb %p", ip_nf_cfg_cb));
        NBB_VERIFY_MEMORY(ip_nf_cfg_cb, sizeof(SPM_IP_NF_CB), MEM_SPM_IP_NF_CB);

        ipv4_dst_cfg_cb = ip_nf_cfg_cb->ipv4_dst_cfg_cb;
        if(ipv4_dst_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_dst_cfg_cb cb %p", ipv4_dst_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_dst_cfg_cb, sizeof(ATG_DCI_IP_NF_IPV4_DSTHOST),
                              MEM_SPM_IP_NF_IPV4_DSTHOST_CB);
        }

        ipv4_src_cfg_cb = ip_nf_cfg_cb->ipv4_src_cfg_cb;
        if(ipv4_src_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_src_cfg_cb cb %p", ipv4_src_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_src_cfg_cb, sizeof(ATG_DCI_IP_NF_IPV4_SRCIP),
                              MEM_SPM_IP_NF_IPV4_SRCIP_CB);
        }

        ipv4_version_cfg_cb = ip_nf_cfg_cb->ipv4_version_cfg_cb;
        if(ipv4_version_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv4_version_cfg_cb cb %p", ipv4_version_cfg_cb));
            NBB_VERIFY_MEMORY(ipv4_version_cfg_cb, sizeof(ATG_DCI_IP_NF_IPV4_VERSION),
                              MEM_SPM_IP_NF_IPV4_VERSION_CB);
        }

        ipv6_dst_cfg_cb = ip_nf_cfg_cb->ipv6_dst_cfg_cb;
        if(ipv6_dst_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_dst_cfg_cb cb %p", ipv6_dst_cfg_cb));
            NBB_VERIFY_MEMORY(ipv6_dst_cfg_cb, sizeof(ATG_DCI_IP_NF_IPV6_DSTHOST),
                              MEM_SPM_IP_NF_IPV6_DSTHOST_CB);
        }

        ipv6_src_cfg_cb = ip_nf_cfg_cb->ipv6_src_cfg_cb;
        if(ipv6_src_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_src_cfg_cb cb %p", ipv6_src_cfg_cb));
            NBB_VERIFY_MEMORY(ipv6_src_cfg_cb, sizeof(ATG_DCI_IP_NF_IPV6_SRCIP),
                              MEM_SPM_IP_NF_IPV6_SRCIP_CB);
        }

        ipv6_version_cfg_cb = ip_nf_cfg_cb->ipv6_version_cfg_cb;
        if(ipv6_version_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify ipv6_version_cfg_cb cb %p", ipv6_version_cfg_cb));
            NBB_VERIFY_MEMORY(ipv6_version_cfg_cb, sizeof(ATG_DCI_IP_NF_IPV6_VERSION),
                              MEM_SPM_IP_NF_IPV6_VERSION_CB);
        }
        tcp_cfg_cb = ip_nf_cfg_cb->tcp_cfg_cb;
        if(tcp_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify tcp_cfg_cb cb %p", tcp_cfg_cb));
            NBB_VERIFY_MEMORY(tcp_cfg_cb, sizeof(ATG_DCI_IP_NF_CHECK_TCP),
                              MEM_SPM_IP_NF_CHECK_TCP_CB);
        }

        timeout_cfg_cb = ip_nf_cfg_cb->timeout_cfg_cb;
        if(timeout_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify timeout_cfg_cb cb %p", timeout_cfg_cb));
            NBB_VERIFY_MEMORY(timeout_cfg_cb, sizeof(ATG_DCI_IP_NF_TIMEOUT),
                              MEM_SPM_IP_NF_TIMEOUT_CB);
        }

        start_cfg_cb = ip_nf_cfg_cb->start_cfg_cb;
        if(start_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify start_cfg_cb cb %p", start_cfg_cb));
            NBB_VERIFY_MEMORY(start_cfg_cb, sizeof(ATG_DCI_IP_NF_START),
                              MEM_SPM_IP_NF_START_CB);
        }

        info_cfg_cb = ip_nf_cfg_cb->info_cfg_cb;
        if(info_cfg_cb != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify info_cfg_cb cb %p", info_cfg_cb));
            NBB_VERIFY_MEMORY(info_cfg_cb, sizeof(ATG_DCI_IP_NF_EXPORT_INFO),
                              MEM_SPM_IP_NF_EXPORT_INFO_CB);
        }

        enable_cfg = ip_nf_cfg_cb->enable_cfg;
        if(enable_cfg != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify enable_cfg_cb cb %p", enable_cfg));
            NBB_VERIFY_MEMORY(enable_cfg, sizeof(ATG_DCI_IP_NF_ENABLE),
                              MEM_SPM_IP_NF_ENABLE_CB);
        }

        flexnfkey_cfg = ip_nf_cfg_cb->flexnfkey_cfg;
        if(flexnfkey_cfg != NULL)
        {
            NBB_TRC_FLOW((NBB_FORMAT "Verify flexnfkey_cfg_cb cb %p", flexnfkey_cfg));
            NBB_VERIFY_MEMORY(flexnfkey_cfg, sizeof(ATG_DCI_IP_NF_FLEXNFKEY),
                              MEM_SPM_IP_NF_FLEXNFKEY_CB);
        }
    }

    NBB_TRC_EXIT();

    return;
}

/*****************************************************************************
 函 数 名  : spm_netflow_cfg_cb_verfify
 功能描述  : netflow verify
 输入参数  : NBB_CXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_netflow_cfg_cb_verfify(NBB_CXT_T NBB_CXT)
{
    NBB_TRC_ENTRY("spm_netflow_cfg_cb_verfify");


    spm_cfg_ip_nf_cb_verify( NBB_CXT);

    spm_cfg_nf_aggr_cb_verify(NBB_CXT);

    NBB_TRC_EXIT();

    return;   
}

/*****************************************************************************
 函 数 名  : spm_cfg_oam_basic_cb_verify
 功能描述  : oamverify
 输入参数  : NBB_CXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_cfg_oam_basic_cb_verify(NBB_CXT_T NBB_CXT)
{
    SPM_OAM_CB *p_oamcfg = NULL;
    p_oamcfg = (SPM_OAM_CB *)AVLL_FIRST(v_spm_shared->g_oamavltreebyoamid);
    while(NULL != p_oamcfg)
    {
        NBB_VERIFY_MEMORY(p_oamcfg,sizeof(SPM_OAM_CB),MEM_SPM_ACTIVE_OAM_CB);
        if (NULL != p_oamcfg->pBasiCfg)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pBasiCfg,sizeof(ATG_DCI_TPOAM_ACTIVE_BASIC_DATA),
                MEM_SPM_ACTIVE_OAM_BASEIC_CB);
        }
        if (NULL != p_oamcfg->pRsvplspCfg)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pRsvplspCfg,sizeof(ATG_DCI_TPOAM_ACTIVE_RSVP_LSP),
                MEM_SPM_ACTIVE_OAM_RSVP_CB);
        }
        if (NULL != p_oamcfg->pApscfg)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pApscfg,sizeof(ATG_DCI_TPOAM_ACTIVE_APS),
                MEM_SPM_ACTIVE_OAM_APS_CB);
        }
        if (NULL != p_oamcfg->pCvcfg)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pCvcfg,sizeof(ATG_DCI_TPOAM_ACTIVE_CV),
                MEM_SPM_ACTIVE_OAM_CV_CB);
        }
        if (NULL != p_oamcfg->pCsfcfg)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pCsfcfg,sizeof(ATG_DCI_TPOAM_ACTIVE_CSF),
                MEM_SPM_ACTIVE_OAM_CSF_CB);
        }
        if (NULL != p_oamcfg->pPwCfg)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pPwCfg,sizeof(ATG_DCI_TPOAM_ACTIVE_PW),
                MEM_SPM_ACTIVE_OAM_PW_CB);
        }
        if (NULL != p_oamcfg->pVsCfg)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pVsCfg,sizeof(ATG_DCI_TPOAM_ACTIVE_VS),
                MEM_SPM_ACTIVE_OAM_VS_CB);
        }
        if (NULL != p_oamcfg->pFdicfg)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pFdicfg,sizeof(ATG_DCI_TPOAM_ACTIVE_FDI),
                MEM_SPM_ACTIVE_OAM_FDI_CB);
        }
        if (NULL != p_oamcfg->pWrapping)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pWrapping,sizeof(ATG_DCI_TPOAM_ACTIVE_WRAPPING),
                MEM_SPM_ACTIVE_OAM_WRAPPING_CB);
        }
        if (NULL != p_oamcfg->pAdjust)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pAdjust,sizeof(ATG_DCI_TPOAM_ACTIVE_ADJUST),
                MEM_SPM_ACTIVE_OAM_ADJUST_CB);
        }
        if (NULL != p_oamcfg->pAlarm)
        {
            NBB_VERIFY_MEMORY(p_oamcfg->pAlarm,sizeof(ATG_DCI_TPOAM_ACTIVE_ALARM),
                MEM_SPM_ACTIVE_OAM_ALARM_CB);
        }
        p_oamcfg = (SPM_OAM_CB *)AVLL_NEXT(v_spm_shared->g_oamavltreebyoamid,
                        p_oamcfg->spm_oam_node);
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_cfg_oam_pw_cb_verify
 功能描述  : oampwverify
 输入参数  : NBB_CXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_cfg_oam_pw_cb_verify(NBB_CXT_T NBB_CXT)
{    
    SPM_OAM_PW_CB *p_pwcfg = NULL;
    p_pwcfg = (SPM_OAM_PW_CB *)AVLL_FIRST(v_spm_shared->g_oamavltreebypw);
    while ( NULL != p_pwcfg )
    {
        NBB_VERIFY_MEMORY(p_pwcfg,sizeof(SPM_OAM_PW_CB),MEM_SPM_ACTIVE_OAM_PW_CB);
        p_pwcfg = (SPM_OAM_PW_CB *)AVLL_NEXT(v_spm_shared->g_oamavltreebypw,
                    p_pwcfg->spm_oam_pw_node);
    }
    return;
}

/*****************************************************************************
 函 数 名  : spm_cfg_oam_vs_cb_verify
 功能描述  : oamvsverify
 输入参数  : NBB_CXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_cfg_oam_vs_cb_verify(NBB_CXT_T NBB_CXT)
{    
    SPM_OAM_VS_CB *p_vscfg = NULL;
    p_vscfg = (SPM_OAM_VS_CB *)AVLL_FIRST(v_spm_shared->g_oamavltreebyindex);
    while ( NULL != p_vscfg )
    {
        NBB_VERIFY_MEMORY(p_vscfg,sizeof(SPM_OAM_VS_CB),MEM_SPM_ACTIVE_OAM_VS_CB);
        p_vscfg = (SPM_OAM_VS_CB *)AVLL_NEXT(v_spm_shared->g_oamavltreebyindex,
                    p_vscfg->spm_vs_node);
    }
    return;
}


/*****************************************************************************
 函 数 名  : spm_cfg_oam_wrapping_cb_verify
 功能描述  : oamwrappingverify
 输入参数  : NBB_CXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_cfg_oam_wrapping_cb_verify(NBB_CXT_T NBB_CXT)
{    
    SPM_OAM_WRAPPING_CB *p_wrappingcfg = NULL;
    p_wrappingcfg = (SPM_OAM_WRAPPING_CB *)AVLL_FIRST(v_spm_shared->g_oamavltreebywrapping);
    while ( NULL != p_wrappingcfg )
    {
        NBB_VERIFY_MEMORY(p_wrappingcfg,sizeof(SPM_OAM_WRAPPING_CB),MEM_SPM_ACTIVE_OAM_PW_CB);
        p_wrappingcfg = (SPM_OAM_WRAPPING_CB *)AVLL_NEXT(v_spm_shared->g_oamavltreebywrapping,
                    p_wrappingcfg->spm_wrapping_node);
    }
    return;
}


/*****************************************************************************
 函 数 名  : spm_cfg_oam_list_verify
 功能描述  : oamlistverif
 输入参数  : NBB_CXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_cfg_oam_list_verify(NBB_CXT_T NBB_CXT)
{    
    LIST_NODE *p_test;
    SD_LIST_NODE *p_roll;
    SD_LIST_NODE *p_wrapping;
    p_test = g_oamid_encsflist;
    p_roll = g_oam_sdlist;
    p_wrapping = g_oam_wrappinglist;
    while ( NULL != p_test )
    {
        NBB_VERIFY_MEMORY(p_test,sizeof(LIST_NODE),MEM_SPM_OAM_LIST);
        p_test = p_test->next;
    }
    while ( NULL != p_roll)
    {
        NBB_VERIFY_MEMORY(p_roll,sizeof(SD_LIST_NODE),MEM_SPM_OAM_SD_LIST);
        p_roll = p_roll->next;
    }
    
    while ( NULL != p_wrapping)
    {
        NBB_VERIFY_MEMORY(p_wrapping,sizeof(SD_LIST_NODE),MEM_SPM_OAM_WRAPPING_LIST);
        p_wrapping = p_wrapping->next;
    }
    
    if ( NULL != g_puloam_treebase )
    {
        NBB_VERIFY_MEMORY(g_puloam_treebase,sizeof(NBB_ULONG)*g_usoam_treelevel
            ,MEM_SPM_OAM_SEARCH_TREE);
    }
    
    if (NULL != g_sttree_oamnodeset0.pstNodeArr)
    {
        NBB_VERIFY_MEMORY(g_sttree_oamnodeset0.pstNodeArr, 
            sizeof(SPM_OAMTREE_NODE) * g_usoam_treenodemaxnum, MEM_SPM_TPOAM_TREE_CB);
    }
    if (NULL != g_sttree_oamnodeset1.pstNodeArr)
    {
        NBB_VERIFY_MEMORY(g_sttree_oamnodeset1.pstNodeArr, 
            sizeof(SPM_OAMTREE_NODE) * g_usoam_treenodemaxnum, MEM_SPM_TPOAM_TREE_CB);
    }
    if (NULL != g_pucoam_synidinfo)
    {
        NBB_VERIFY_MEMORY(g_pucoam_synidinfo, 
            g_uloam_synidinfolth, MEM_SPM_OAM_PUC_SYN_ID_INFO_CB);
    }
    return;
}


/*****************************************************************************
 函 数 名  : spm_cfg_oam_rsvp_cb_verify
 功能描述  : oamrsvpverify
 输入参数  : NBB_CXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_cfg_oam_rsvp_cb_verify(NBB_CXT_T NBB_CXT)
{    
    SPM_OAM_RSVP_CB_RX *p_rsvp_rx = NULL;
    SPM_OAM_RSVP_CB_TX *p_rsvp_tx = NULL;
    p_rsvp_rx = (SPM_OAM_RSVP_CB_RX *)AVLL_FIRST(v_spm_shared->g_oamavltreebyrsvprx);
    p_rsvp_tx = (SPM_OAM_RSVP_CB_TX *)AVLL_FIRST(v_spm_shared->g_oamavltreebyrsvptx);
    while ( NULL != p_rsvp_rx )
    {
        NBB_VERIFY_MEMORY(p_rsvp_rx,sizeof(SPM_OAM_RSVP_CB_RX),
            MEM_SPM_ACTIVE_OAM_RSVP_CB);
        p_rsvp_rx = (SPM_OAM_RSVP_CB_RX *)AVLL_NEXT(v_spm_shared->g_oamavltreebyrsvprx,
            p_rsvp_rx->spm_oam_rsvp_node);
    }
    while ( NULL != p_rsvp_tx )
    {
        NBB_VERIFY_MEMORY(p_rsvp_tx,sizeof(SPM_OAM_RSVP_CB_TX),
            MEM_SPM_ACTIVE_OAM_RSVP_CB);
        p_rsvp_tx = (SPM_OAM_RSVP_CB_TX *)AVLL_NEXT(v_spm_shared->g_oamavltreebyrsvptx,
            p_rsvp_tx->spm_oam_rsvp_node);
    }
    return;    
}

/*****************************************************************************
 函 数 名  : spm_cfg_oam_global_cb_verify
 功能描述  : oamglobverify
 输入参数  : NBB_CXT_T NBB_CXT  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 2015年3月20日
    作    者   : limingming
    审 核 人   : #
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_cfg_oam_global_cb_verify(NBB_CXT_T NBB_CXT)
{
    NBB_BYTE ucLoop = 0;
    for (ucLoop = 0; ucLoop < OAM_MAX_NUM_OD; ucLoop++)
    {
        if (NULL != g_stoam_odlocal[ucLoop].pBasicData)
        {
            NBB_VERIFY_MEMORY(g_stoam_odlocal[ucLoop].pBasicData,
                sizeof(ATG_DCI_TPOAM_OD_BASIC_DATA),MEM_SPM_OD_OAM_BASIC_CB);
        }
        if (NULL != g_stoam_odlocal[ucLoop].pDmFrame)
        {            
            NBB_VERIFY_MEMORY(g_stoam_odlocal[ucLoop].pDmFrame,
                sizeof(ATG_DCI_TPOAM_OD_DM_FRAME),MEM_SPM_OD_OAM_DM_CB);
        }
        if (NULL != g_stoam_odlocal[ucLoop].pLckFrame)
        {            
            NBB_VERIFY_MEMORY(g_stoam_odlocal[ucLoop].pLckFrame,
                sizeof(ATG_DCI_TPOAM_OD_LCK_FRAME),MEM_SPM_OD_OAM_LCK_CB);
        }
        if (NULL != g_stoam_odlocal[ucLoop].pLmFrame)
        {            
            NBB_VERIFY_MEMORY(g_stoam_odlocal[ucLoop].pLmFrame,
                sizeof(ATG_DCI_TPOAM_OD_LM_FRAME),MEM_SPM_OD_OAM_LM_CB);
        }
        if (NULL != g_stoam_odlocal[ucLoop].pLoopFrame)
        {            
            NBB_VERIFY_MEMORY(g_stoam_odlocal[ucLoop].pLoopFrame,
                sizeof(ATG_DCI_TPOAM_OD_LOOP_FRAME),MEM_SPM_OD_OAM_LB_CB);
        }
        if (NULL != g_stoam_odlocal[ucLoop].pMip)
        {            
            NBB_VERIFY_MEMORY(g_stoam_odlocal[ucLoop].pMip,
                sizeof(ATG_DCI_TPOAM_OD_MIP),MEM_SPM_OD_OAM_MIP_CB);
        }
        if (NULL != g_stoam_odlocal[ucLoop].pTestFrame)
        {            
            NBB_VERIFY_MEMORY(g_stoam_odlocal[ucLoop].pTestFrame,
                sizeof(ATG_DCI_TPOAM_OD_TST_FRAME),MEM_SPM_OD_OAM_TST_CB);
        }
    }
    return;
}


