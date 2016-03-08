/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_qos_proc.c
   版 本 号   : 初稿
   作    者   : zenglu
   生成日期   : 2012年9月18日
   最近修改   :
   功能描述   : QOS处理
   函数列表   :
   修改历史   :
   1.日    期   : 2012年9月18日
    作    者   : zenglu
    修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

static char *policy_mode_string[] = {"调度节点", "CB对"};
static char *supervise_mode_string[] = {"不使能", "MEF", "RFC2698", "RFC2697"};
static char *supervise_color_mode_string[] = {"色盲", "色敏感"};
static char *supervise_coupling_flag_string[] = {"非联合(MEF trtCM有效)", "联合(MEF trtCM有效)"};

//static char *supervise_markcolor_string[] = {"Green","Yellow","Red","非法"};
static char *supervise_markphb_string[] = {"BE","AF1","AF2","AF3","AF4","EF","CS6","CS7"};
static char *supervise_action_string[] = {"Pass","Drop","MarkPHB","MarkColor","MarkPHB&Color"};

static char *flow_action_mode_string[] = {"Drop", "Permit", "Sample", "Redirect", "PolicyRouting", "URPF"};
static char *sampling_percent_string[] = {"1:1", "1:2", "1:10", "1:100", "1:1000", "1:10000"};
static char *redirect_mode_string[] = {"关闭重定向", "重定向到VRF ", "重定向到端口"};
static char *wred_mode_string[] = {"队尾丢弃", "色盲", "分颜色WRED"};
static char *acl_direct_string[] = {"入", "出"};
static char *acl_flag_string[] = {"端口未实例化", "端口实例化"};
static char *acl_action_string[] = {"丢弃", "通过"};

static char* pri2phb_mode_string[] = {"指配PHB","信任DSCP","信任OVLAN PRI","","","","","","信任EXP"};
static char* phb2pri_mode_string[] = {" 不动作","指配DSCP ","指配OVLAN.PRI","指配 DSCP-TOS&OVLAN.PRI",
    "指配EXP ","继承VPN EXP ","","","","","","","","","","","基于映射"};
static char *intf_ds_string[] = {"空","pri", "dscp"};
static char *l3uni_ds_string[] = {"no action", "pri","dscp+pri"};
static char *flow_ds_string[] = {"SET","pri", "dscp"};
static char *vp_flag_string[] = {"空", "主用","备用","ECMP"};
static char *ilm_action_string[] = {"SWAP", "SWAP+PUSH","LSP_POP_GO","PW_POP_GO"};
static char *rxlsp_ds_pri2phb_string[] = {"NNI_SET_PHB","UNIFORM", "SHORT_PIPE"};
static char *rxlsp_ds_phb2pri_string[] = {"SET_EXP","MAP_EXP",};
static char *egr_nni_lsp_string[] = {"LSP_NO_ACTION","MAP_EXP"};
static char *egr_nni_pw_string[] = {"NNI_NO_ACTION","空","空","PHB_TO_EXP"};
static char *lsp_mode[] = {"基于模板","基于TE"};
static char *lsp_flag[] = {"空","FTN","TXLSP","IP LSP"};

#ifdef R8000

#endif



/* 全局打印变量acl_pri_setting */
NBB_BYTE acl_pri_setting = ATG_DCI_RC_UNSUCCESSFUL;



/* 全局打印变量qos_log_cfg_print */
NBB_BYTE qos_log_cfg_print = ATG_DCI_RC_OK;

/* 全局打印变量qos_vc_cfg_print */
NBB_BYTE qos_vc_cfg_print = ATG_DCI_RC_OK;

/* 全局打印变量qos_vrf_cfg_print */
NBB_BYTE qos_vrf_cfg_print = ATG_DCI_RC_OK;

/* 全局打印变量qos_txlsp_cfg_print */
NBB_BYTE qos_txlsp_cfg_print = ATG_DCI_RC_OK;

/* 全局打印变量qos_pw_cfg_print */
NBB_BYTE qos_pw_cfg_print = ATG_DCI_RC_OK;

/* 全局打印变量qos_defend_cfg_print */
NBB_BYTE qos_defend_cfg_print = ATG_DCI_RC_OK;

/* 全局打印变量qos_acl_action_print */
NBB_BYTE qos_acl_action_print = ATG_DCI_RC_OK;


#if 1

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_print_sub_port(SUB_PORT *sub_port)
{
    if(NULL == sub_port)
    {
        printf("FATAL ERROR NULL == sub_port\n");
    }
    else
    {
       printf("c3unit=%d,slot=%d,port=%d,svlan=%d,ivlan=%d\n",
        sub_port->unit,sub_port->slot,sub_port->port,sub_port->ovlan,sub_port->ivlan); 
    }
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_ds_intf_cfg(NBB_ULONG log_index)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_LOGIC_INTF_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (cfg_cb = (SPM_QOS_DS_LOGIC_INTF_CB *)AVLL_FIRST(v_spm_shared->qos_ds_logic_intf_tree);
        cfg_cb != NULL;
        cfg_cb = (SPM_QOS_DS_LOGIC_INTF_CB *)AVLL_NEXT(v_spm_shared->qos_ds_logic_intf_tree,
            cfg_cb->spm_qos_logic_node))
    {
        if ((0 == log_index) || (cfg_cb->logic_key.index == log_index))
        {
            spm_print_sub_port(&(cfg_cb->sub_port));
            printf("ds_index=%lu,log_key=%lu,posid=%ld,in_mode=%s,in_dsptr=%ld,out_mode=%s,out_dsptr=%ld\n\n",
                cfg_cb->ds_index, cfg_cb->logic_key.index,cfg_cb->posid,intf_ds_string[cfg_cb->in_mode],
                cfg_cb->in_dsptr,l3uni_ds_string[cfg_cb->out_mode],cfg_cb->out_dsptr);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_ds_univp_cfg(NBB_ULONG log_index)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_LOGIC_UNIVP_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for(cfg_cb = (SPM_QOS_DS_LOGIC_UNIVP_CB *)AVLL_FIRST(v_spm_shared->qos_ds_logic_univp_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_DS_LOGIC_UNIVP_CB *)AVLL_NEXT(v_spm_shared->qos_ds_logic_univp_tree,  \
                  cfg_cb->spm_qos_logic_univp_node))
    {
        if ((0 == log_index) || (cfg_cb->logic_index == log_index))
        {
            printf("log_key=%lu,ds_index=%lu,vp_posid=%lu,vp_type=%lu,vpn_id=%lu,out_mode=%d,out_dsptr=%ld.\n\n",
                   cfg_cb->logic_index,cfg_cb->ds_index,cfg_cb->vpuni.vp_posid,
                   cfg_cb->vpuni.vp_type,cfg_cb->vpuni.vpn_id,cfg_cb->out_mode,cfg_cb->out_dsptr);
        }
    }
    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_ds_flow_cfg(NBB_ULONG log_index)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_LOGIC_FLOW_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (cfg_cb = (SPM_QOS_DS_LOGIC_FLOW_CB *)AVLL_FIRST(v_spm_shared->qos_ds_logic_flow_tree); cfg_cb != NULL;
        cfg_cb = (SPM_QOS_DS_LOGIC_FLOW_CB *)AVLL_NEXT(v_spm_shared->qos_ds_logic_flow_tree,   \
                cfg_cb->spm_qos_logic_node))
    {
        if ((0 == log_index) || (cfg_cb->flow_key.index == log_index))
        {
            spm_print_sub_port(&(cfg_cb->sub_port));
            printf("ds_index=%lu,log_key=%lu,posid=%ld,svlan=%d,mode=%s,dsptr=%ld\n\n",
                cfg_cb->ds_index, cfg_cb->flow_key.index,cfg_cb->posid, cfg_cb->flow_key.svlan,
                flow_ds_string[cfg_cb->in_mode],cfg_cb->in_dsptr);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_ds_vc_cfg(NBB_ULONG vc_id)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_VC_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (cfg_cb = (SPM_QOS_DS_VC_CB *)AVLL_FIRST(v_spm_shared->qos_ds_vc_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DS_VC_CB *)AVLL_NEXT(v_spm_shared->qos_ds_vc_tree,cfg_cb->spm_qos_vc_node))
    {
        if((0 == vc_id) || (vc_id == cfg_cb->vc_key.vc_id))
        {
            printf("ds_index=%lu,vc_id=%lu,vc_type=%d,peer_ip=0x%lx,posid=%lu,"
                "flag=%s,in_mode=%s,in_dsptr=%ld,out_dsptr=%ld,out_mode=%s\n\n",
                cfg_cb->ds_index, cfg_cb->vc_key.vc_id, cfg_cb->vc_key.vc_type,
                cfg_cb->vc_key.peer_ip,cfg_cb->posid,vp_flag_string[cfg_cb->flag],
                rxlsp_ds_pri2phb_string[cfg_cb->in_mode],
                cfg_cb->in_dsptr,cfg_cb->out_dsptr,egr_nni_pw_string[cfg_cb->out_mode]);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_ds_vrf_cfg(NBB_USHORT vrfid)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_VRF_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (cfg_cb = (SPM_QOS_DS_VRF_CB *)AVLL_FIRST(v_spm_shared->qos_ds_vrf_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DS_VRF_CB *)AVLL_NEXT(v_spm_shared->qos_ds_vrf_tree,cfg_cb->spm_qos_vrf_node))
    {
        if(vrfid == cfg_cb->vrf_key.vrf_id)
        {
           printf("ds_index=%lu,nhi=%ld,dsptr=%ld,out_mode=%s,vrf_id=%d,peer_ip=0x%lx,label=%lu\n\n",
            cfg_cb->ds_index,cfg_cb->nhi,cfg_cb->dsptr,egr_nni_pw_string[cfg_cb->out_mode],
            cfg_cb->vrf_key.vrf_id,
            cfg_cb->vrf_key.peer_ip,
            cfg_cb->vrf_key.label); 
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_ds_ilm_cfg(NBB_ULONG inlabel )
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_ILM_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (cfg_cb = (SPM_QOS_DS_ILM_CB *)AVLL_FIRST(v_spm_shared->qos_ds_ilm_tree); cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DS_ILM_CB *)AVLL_NEXT(v_spm_shared->qos_ds_ilm_tree,cfg_cb->spm_qos_ilm_node))
    {
        if((0 == inlabel) || (inlabel == cfg_cb->ilm_key.inlabel))
        {
            printf("ds_index=%lu,inlabel=%lu,posid=%lu,flag=%s,"
                "node_action=%s,nextip=0x%lx,nextport=%lu,ds_index2=%lu,"
                "lspid=%lu,tunnelid=%lu,egress=0x%lx,ingress=0x%lx,"
                "in_mode=%s,in_dsptr=%ld,out_mode=%s,out_mode=%ld\n\n",
                cfg_cb->ds_index, cfg_cb->ilm_key.inlabel,cfg_cb->posid,vp_flag_string[cfg_cb->ilm_key.flag],
                ilm_action_string[cfg_cb->node_action],cfg_cb->ilm_key.nextip,cfg_cb->ilm_key.nextport,
                cfg_cb->ds_index2,cfg_cb->key_tun2.tx_lsp.lspid,cfg_cb->key_tun2.tx_lsp.tunnelid,
                cfg_cb->key_tun2.tx_lsp.egress,cfg_cb->key_tun2.tx_lsp.ingress,
                rxlsp_ds_pri2phb_string[cfg_cb->in_mode],cfg_cb->in_dsptr,
                rxlsp_ds_phb2pri_string[cfg_cb->out_mode],cfg_cb->out_dsptr);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_ds_rxlsp_cfg()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_RX_LSP_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (cfg_cb = (SPM_QOS_DS_RX_LSP_CB *)AVLL_FIRST(v_spm_shared->qos_ds_rx_lsp_tree);cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DS_RX_LSP_CB *)AVLL_NEXT(v_spm_shared->qos_ds_rx_lsp_tree,cfg_cb->spm_qos_rx_lsp_node))
    {
        printf("ds_index=%ld,egress=0x%lx,ingress=0x%lx,lspid=%ld,tunnelid=%ld,"
            "flag=%s,posid=%ld,node_action=%s,ds_index2=%ld,"
            "lspid=%ld,tunnelid=%ld,egress=0x%lx,ingress=0x%lx,"
            "in_mode=%s,in_dsptr=%ld,out_mode=%s,out_mode=%ld\n\n",
            cfg_cb->ds_index, cfg_cb->rx_lsp_key.rxlspkey.egress,
            cfg_cb->rx_lsp_key.rxlspkey.ingress, cfg_cb->rx_lsp_key.rxlspkey.lspid,
            cfg_cb->rx_lsp_key.rxlspkey.tunnelid,vp_flag_string[cfg_cb->rx_lsp_key.flag],
            cfg_cb->posid,ilm_action_string[cfg_cb->node_action],
            cfg_cb->ds_index2,cfg_cb->key_tun2.tx_lsp.lspid,cfg_cb->key_tun2.tx_lsp.tunnelid,
            cfg_cb->key_tun2.tx_lsp.egress,cfg_cb->key_tun2.tx_lsp.ingress,
            rxlsp_ds_pri2phb_string[cfg_cb->in_mode],
            cfg_cb->in_dsptr,rxlsp_ds_phb2pri_string[cfg_cb->out_mode],cfg_cb->out_dsptr);
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_ds_tunnel_cfg()
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_DS_TUNNEL_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (cfg_cb = (SPM_QOS_DS_TUNNEL_CB *)AVLL_FIRST(v_spm_shared->qos_ds_tunnel_tree);cfg_cb != NULL;
         cfg_cb = (SPM_QOS_DS_TUNNEL_CB *)AVLL_NEXT(v_spm_shared->qos_ds_tunnel_tree,cfg_cb->spm_qos_tunnel_node))
    {
        printf("ds_index=%ld,type=%s,egress=0x%lx,ingress=0x%lx,lspid=%ld,tunnelid=%ld,posid=%ld\n",
            cfg_cb->ds_index,lsp_flag[cfg_cb->tunnel_key.type], cfg_cb->tunnel_key.tx_lsp.egress, 
            cfg_cb->tunnel_key.tx_lsp.ingress,cfg_cb->tunnel_key.tx_lsp.lspid,
            cfg_cb->tunnel_key.tx_lsp.tunnelid,cfg_cb->posid);
        if(IP_LSP_TYPE == cfg_cb->tunnel_key.flag)
        {
            printf("mode=%s\n\n",egr_nni_pw_string[cfg_cb->out_mode]);
        }
        else
        {
            printf("mode=%s\n\n",egr_nni_lsp_string[cfg_cb->out_mode]);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_ds_domain_dscp2phb_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ds_domain_dscp2phb_cfg(ATG_DCI_DS_DOMAIN_DSCP2PHBMAP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;

    if (NULL == cfg)
    {
        printf("dscp2phb_cfg == null\n");
        return;
    }
    for (i = 0; i < ATG_DCI_DSCP_MAP_NUM; i++)
    {
        printf("dscp = %2d,phb = %d,color = %d\n", i, cfg->dscp_map[i].phb, cfg->dscp_map[i].color);
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_ds_domain_vlan2phb_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ds_domain_vlan2phb_cfg(ATG_DCI_DS_DOMAIN_V8021P2PHBMAP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;

    if (NULL == cfg)
    {
        printf("vlan2phb_cfg == null\n");
        return;
    }
    for (i = 0; i < ATG_DCI_MAX_PHB_NUM; i++)
    {
        printf("vlanpri = %2d,phb = %d,color = %d\n", i, cfg->v8021p_map[i].phb, cfg->v8021p_map[i].color);
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_ds_domain_exp2phb_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ds_domain_exp2phb_cfg(ATG_DCI_DS_DOMAIN_EXP2PHBMAP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;

    if (NULL == cfg)
    {
        printf("exp2phb_cfg == null\n");
        return;
    }
    for (i = 0; i < ATG_DCI_MAX_PHB_NUM; i++)
    {
        printf("exppri = %2d,phb = %d,color = %d\n", i, cfg->exp_map[i].phb, cfg->exp_map[i].color);
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_ds_domain_phb2dscp_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ds_domain_phb2dscp_cfg(ATG_DCI_DS_DOMAIN_PHB2DSCPMAP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;
    NBB_BYTE j = 0;

    if (NULL == cfg)
    {
        printf("phb2dscp_cfg == null\n");
        return;
    }
    for (i = 0; i < ATG_DCI_MAX_PHB_NUM; i++)
    {
        for (j = 0; j < ATG_DCI_MAX_COLOR_NUM; j++)
        {
            printf("phb = %d,color = %d,dscp = %2d\n", i, j, cfg->phb_map_dscp[i][j]);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_ds_domain_phb2vlan_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ds_domain_phb2vlan_cfg(ATG_DCI_DS_DOMAIN_PHB2V8021PMAP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;
    NBB_BYTE j = 0;

    if (NULL == cfg)
    {
        printf("phb2vlan_cfg == null\n");
        return;
    }
    for (i = 0; i < ATG_DCI_MAX_PHB_NUM; i++)
    {
        for (j = 0; j < ATG_DCI_MAX_COLOR_NUM; j++)
        {
            printf("phb = %d,color = %d,vlanpri = %2d\n", i, j, cfg->phb_map_8021p[i][j]);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_ds_domain_phb2exp_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ds_domain_phb2exp_cfg(ATG_DCI_DS_DOMAIN_PHB2EXPMAP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;
    NBB_BYTE j = 0;

    if (NULL == cfg)
    {
        printf("phb2exp_cfg == null\n");
        return;
    }
    for (i = 0; i < ATG_DCI_MAX_PHB_NUM; i++)
    {
        for (j = 0; j < ATG_DCI_MAX_COLOR_NUM; j++)
        {
            printf("phb = %d,color = %d,exppri = %2d\n", i, j, cfg->phb_map_exp[i][j]);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_show_ds_domain_cfg
   功能描述  : 显示配置
   输入参数  : ds域index，配置类型1-6分别是D2P,V2P,E2P,P2D,P2V,P2E,0是全部类型
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID qos_show_dsdomain(NBB_ULONG index, NBB_BYTE type)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_DOMAIN_CB *pcb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    if (0 == index) /*非法*/
    {
        printf("**ERROR** ds_domain index = 0 ");
        goto EXIT_LABEL;
    }
    if (15 < index) /*非法*/
    {
        printf("**ERROR** ds_domain index %ld > 15 ", index);
        goto EXIT_LABEL;
    }
    if (type > 6)
    {
        printf("type %d out of range\n", type);
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(v_spm_shared->ds_domain_tree, &index);

    /*查不到ds 域配置*/
    if (NULL == pcb)
    {
        printf("can't find ds_domain cfg\n");
        goto EXIT_LABEL;
    }
    if ((0 == type) || (1 == type))
    {
        spm_print_ds_domain_dscp2phb_cfg(pcb->d2p_cfg_cb);
    }
    if ((0 == type) || (2 == type))
    {
        spm_print_ds_domain_vlan2phb_cfg(pcb->vlan2p_cfg_cb);
    }
    if ((0 == type) || (3 == type))
    {
        spm_print_ds_domain_exp2phb_cfg(pcb->exp2p_cfg_cb);
    }
    if ((0 == type) || (4 == type))
    {
        spm_print_ds_domain_phb2dscp_cfg(pcb->p2d_cfg_cb);
    }
    if ((0 == type) || (5 == type))
    {
        spm_print_ds_domain_phb2vlan_cfg(pcb->p2vlan_cfg_cb);
    }
    if ((0 == type) || (6 == type))
    {
        spm_print_ds_domain_phb2exp_cfg(pcb->p2exp_cfg_cb);
    }
    printf(" ds_domain cnt = %ld\n", pcb->cnt);

    EXIT_LABEL:
    return;
}


/*****************************************************************************
   函 数 名  : spm_print_ds_pri2phb_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ds_pri2phb_cfg(ATG_DCI_DIFF_SERV_PRI2PHBMAP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        printf("ds_pri2phb_cfg == null\n");
        return;
    }
    printf("mod = %s,phb = %d,color = %d,ds_domain index = %d\n", 
           pri2phb_mode_string[cfg->pri_select], cfg->phb, cfg->color, cfg->index);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_ds_phb2pri_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ds_phb2pri_cfg(ATG_DCI_DIFF_SERV_PHB2PRIMAP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        printf("ds_phb2pri_cfg == null\n");
        return;
    }
    printf("mod = %s,dscp = %d,vlan = %d,exp = %d,ds_domain index = %d\n",
           phb2pri_mode_string[cfg->pri_select], cfg->dscp, cfg->ovlan_pri, 
           cfg->exp, cfg->index);

    return;
}


/*****************************************************************************
   函 数 名  : spm_show_ds_cfg
   功能描述  : 显示配置
   输入参数  : ds index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID qos_show_ds(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_DS_CB *pcb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    if (0 == index) /*非法*/
    {
        printf("**ERROR** ds_domain index = 0 ");
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(v_spm_shared->ds_tree, &index);

    /*查不到ds 域配置*/
    if (NULL == pcb)
    {
        printf("can't find ds cfg\n");
        goto EXIT_LABEL;
    }
    spm_print_ds_pri2phb_cfg(pcb->pri2phb_cfg_cb);
    spm_print_ds_phb2pri_cfg(pcb->phb2pri_cfg_cb);
    printf(" ds cnt = %ld\n", pcb->cnt);

    EXIT_LABEL:
    return;
}

#endif


#if 2

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_print_mac(NBB_BYTE *mac)
{
    if (NULL == mac)
    {
        printf("mac == null\n");
        return;
    }
    printf("%x,%x,%x,%x,%x,%x\n", mac[0],
            mac[1], mac[2], mac[3], mac[4], mac[5]);
}





/*****************************************************************************
   函 数 名  : spm_print_acl_basic_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_basic_cfg(ATG_DCI_ACL_BASIC_DATA *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        printf("**ERROR**acl_basic_cfg == null\n\n");
        return;
    }
    printf(" 0/1/2/3/()/9/10/()/19/()/40/ = NULL/port/ETH Header/IPV4/()\
             / Ipv4TCP/IPV4UDP()/()/IPV6 /NO IP\n");
    printf("basic_cfg match_type = %d,action = %s\n\n",cfg->match_type,acl_action_string[cfg->action]);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_acl_port_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_port_cfg(ATG_DCI_ACL_PORT_DATA *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("port_cfg index = %ld\n\n", cfg->index);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_acl_eth_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_eth_cfg(ATG_DCI_ACL_ETH_HEADER *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("eth_cfg\n");
    printf("dst mac:");
    spm_print_mac(cfg->dst_mac);
    printf("dst mac mask:");
    spm_print_mac(cfg->dst_mac_mask);
    printf("src mac:");
    spm_print_mac(cfg->src_mac);
    printf("src mac mask:");
    spm_print_mac(cfg->src_mac_mask);
    printf("vlan_prio = %d, vlan_prio_mask = 0x%x\n",
           cfg->vlan_prio,cfg->vlan_prio_mask);
#ifdef R8000_V3R2
    printf("cvlan_prio = %d, cvlan_prio_mask = 0x%x.",
           cfg->cvlan_prio,cfg->cvlan_prio_mask);
#endif
    printf("\n");

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_acl_ipv4_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_ipv4_cfg(ATG_DCI_ACL_IPV4_FRAME *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("ipv4_cfg\n");
    printf("dst ip:");
    spm_print_ipv4(cfg->dst_ip);
    printf("dst ip mask:");
    spm_print_ipv4(cfg->dst_ip_mask);
    printf("src ip:");
    spm_print_ipv4(cfg->src_ip);
    printf("src ip mask:");
    spm_print_ipv4(cfg->src_ip_mask);
    printf("tos:%d,tos mask=%x\n", cfg->dscp, cfg->dscp_mask);
    printf("l3_protocol:%d,l3_protocol mask:%x\n", cfg->l3_protocol, cfg->l3_protocol_mask);
    printf("src port=%d,mask=%x,dst port=%d,mask=%x\n\n",cfg->src_port,
            cfg->src_port_mask,cfg->dst_port,cfg->dst_port_mask);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_acl_ipTcp_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_iptcp_cfg(ATG_DCI_ACL_IPV4_TCP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("ipvTcp_cfg\n");
    printf("dst ip:");
    spm_print_ipv4(cfg->dst_ip);
    printf("dst ip mask:");
    spm_print_ipv4(cfg->dst_ip_mask);
    printf("src ip:");
    spm_print_ipv4(cfg->src_ip);
    printf("src ip mask:");
    spm_print_ipv4(cfg->src_ip_mask);
    printf("tos:%d,tos mask=%x\ndst port=%d,mask=%x,src port=%d,mask=%x\n",
            cfg->dscp, cfg->dscp_mask,cfg->dst_port,cfg->dst_port_mask,cfg->src_port,cfg->src_port_mask);
    printf("fin=%d,syn=%d,rst=%d,push=%d,ack=%d,urg=%d,ece=%d,cwr=%d\n",
            cfg->tcp_flag.fin,cfg->tcp_flag.syn,
            cfg->tcp_flag.rst,cfg->tcp_flag.push,
            cfg->tcp_flag.ack,cfg->tcp_flag.urg,
            cfg->tcp_flag.ece,cfg->tcp_flag.cwr);
    printf("tcp_flag_mask=%d\n\n",cfg->tcp_flag_mask);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_acl_ipv6_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_ipv6_cfg(ATG_DCI_ACL_IPV6_FRAME *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("ipv6_cfg\n");
    printf("dst ipv6:\n");
    spm_print_ipv6(cfg->dst_ip);
    printf("dst ipv6 mask:%d\n", cfg->dst_ip_mask_len);
    printf("src ipv6:\n");
    spm_print_ipv6(cfg->src_ip);
    printf("src ipv6 mask:%d\n\n", cfg->src_ip_mask_len);
    printf("l3_protocol:%d,l3_protocol mask:%x,tos=%d,tos mask=%x"
           "src port=%d,src port mask=%x,dst port=%d,dst port mask=%x\n\n",
            cfg->l3_protocol, cfg->l3_protocol_mask,cfg->tos,cfg->tos_mask,
            cfg->src_port,cfg->src_port_mask,cfg->dst_port,cfg->dst_port_mask);
    return;
}

/*****************************************************************************
   函 数 名  : spm_print_acl_noip_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_noip_cfg(ATG_DCI_ACL_NONE_IP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("noip_cfg\n");
    printf("etype:%x,etype mask:%x\n", cfg->etype, cfg->etype_mask);
    printf("payload 0-4=%x,%x,%x,%x\n",cfg->payload[0],cfg->payload[1],cfg->payload[2],cfg->payload[3]);
    printf("payload 4-8=%x,%x,%x,%x\n",cfg->payload[4],cfg->payload[5],cfg->payload[6],cfg->payload[7]);
    printf("payload 9-12=%x,%x,%x,%x\n\n",cfg->payload[8],cfg->payload[9],cfg->payload[10],cfg->payload[11]);
    printf("mask=%x,%x,%x,%x\n%x,%x,%x,%x\n%x,%x,%x,%x\n\n\n",cfg->payload_mask[0],
            cfg->payload_mask[1],cfg->payload_mask[2],cfg->payload_mask[3],cfg->payload_mask[4],
            cfg->payload_mask[5],cfg->payload_mask[6],cfg->payload_mask[7],cfg->payload_mask[8],
            cfg->payload_mask[9],cfg->payload_mask[10],cfg->payload_mask[11]);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_acl_ipUdp_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_ipudp_cfg(ATG_DCI_ACL_IPV4_UDP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("ipvUdp_cfg\n");
    printf("dst ip:");
    spm_print_ipv4(cfg->dst_ip);
    printf("dst ip mask:");
    spm_print_ipv4(cfg->dst_ip_mask);
    printf("src ip:");
    spm_print_ipv4(cfg->src_ip);
    printf("src ip mask:");
    spm_print_ipv4(cfg->src_ip_mask);
    printf("tos:%d,tos mask=%x\ndst port=%d,mask=%x,src port=%d,mask=%x\n\n",
            cfg->dscp, cfg->dscp_mask,cfg->dst_port,cfg->dst_port_mask,cfg->src_port,cfg->src_port_mask);

    return;
}

#ifdef R8000_V3R2

/*****************************************************************************
   函 数 名  : spm_print_acl_portvlan_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2015年8月18日 
    作    者   : tianf
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_acl_portvlan_cfg(ATG_DCI_ACL_PORT_VLAN *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("portVlan_cfg\n");
    printf("svlan_id = %d,svlan_mask = 0x%x;\n",cfg->svlan_id,cfg->svlan_mask);
    printf("cvlan_id = %d,cvlan_mask = 0x%x.",cfg->cvlan_id,cfg->cvlan_mask);
    printf("\n");
    return;
}

#endif

/*****************************************************************************
   函 数 名  : spm_print_classify_basic_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_classify_basic_cfg(ATG_DCI_QOS_CLASSIFY_BASIC_DATA *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        printf("**ERROR**classify_basic_cfg == null\n\n");
        return;
    }
    printf(" 0/1/2/3/()/9/10/()/19/()/40/50 = NULL/port/ETH Header/IPV4 \
             /()/ Ipv4TCP/IPV4UDP()/()/IPV6 /NO IP/ACL\n");
    printf("basic_cfg match_type = %d,acl_id = %ld\n\n",cfg->match_type,cfg->acl_id);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_classify_port_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_classify_port_cfg(ATG_DCI_QOS_CLASSIFY_PORT_DATA *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("port_cfg index = %ld \n\n", cfg->index);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_classify_eth_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_classify_eth_cfg(ATG_DCI_QOS_CLASSIFY_ETH_HEADER *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("eth_cfg\n");
    printf("dst mac:");
    spm_print_mac(cfg->dst_mac);
    printf("dst mac mask:");
    spm_print_mac(cfg->dst_mac_mask);
    printf("src mac:");
    spm_print_mac(cfg->src_mac);
    printf("src mac mask:");
    spm_print_mac(cfg->src_mac_mask);
    printf("vlan_prio = %d, vlan_prio_mask = 0x%x.",
           cfg->vlan_prio,cfg->vlan_prio_mask);
    printf("\n");

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_classify_ipv4_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_classify_ipv4_cfg(ATG_DCI_QOS_CLASSIFY_IPV4_FRAME *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("ipv4_cfg\n");
    printf("dst ip:");
    spm_print_ipv4(cfg->dst_ip);
    printf("dst ip mask:");
    spm_print_ipv4(cfg->dst_ip_mask);
    printf("src ip:");
    spm_print_ipv4(cfg->src_ip);
    printf("src ip mask:");
    spm_print_ipv4(cfg->src_ip_mask);
    printf("tos:%d,tos mask=%x\n", cfg->dscp, cfg->dscp_mask);
    printf("l3_protocol:%d,l3_protocol mask%x:\n", cfg->l3_protocol, cfg->l3_protocol_mask);
    printf("src port=%d,mask=%x,dst port=%d,mask=%x\n\n",cfg->src_port,
            cfg->src_port_mask,cfg->dst_port,cfg->dst_port_mask);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_classify_ipTcp_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_classify_iptcp_cfg(ATG_DCI_QOS_CLASSIFY_IPV4_TCP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("ipvTcp_cfg\n");
    printf("dst ip:");
    spm_print_ipv4(cfg->dst_ip);
    printf("dst ip mask:");
    spm_print_ipv4(cfg->dst_ip_mask);
    printf("src ip:");
    spm_print_ipv4(cfg->src_ip);
    printf("src ip mask:");
    spm_print_ipv4(cfg->src_ip_mask);
    printf("tos:%d,tos mask=%x\ndst port=%d,mask=%x,src port=%d,mask=%x\n",
            cfg->dscp, cfg->dscp_mask,cfg->dst_port,cfg->dst_port_mask,cfg->src_port,cfg->src_port_mask);
    printf("fin=%d,syn=%d,rst=%d,push=%d,ack=%d,urg=%d,ece=%d,cwr=%d\n",
            cfg->tcp_flag.fin,cfg->tcp_flag.syn,cfg->tcp_flag.rst,cfg->tcp_flag.push,
            cfg->tcp_flag.ack,cfg->tcp_flag.urg,cfg->tcp_flag.ece,cfg->tcp_flag.cwr);
    printf("tcp_flag_mask=%d\n\n",cfg->tcp_flag_mask);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_classify_ipv6_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_classify_ipv6_cfg(ATG_DCI_QOS_CLASSIFY_IPV6_FRAME *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("ipv6_cfg\n");
    printf("dst ipv6:\n");
    spm_print_ipv6(cfg->dst_ip);
    printf("dst ipv6 mask:%d\n", cfg->dst_ip_mask_len);
    printf("src ipv6:\n");
    spm_print_ipv6(cfg->src_ip);
    printf("src ipv6 mask:%d\n\n", cfg->src_ip_mask_len);
    printf("l3_protocol:%d,l3_protocol mask:%x,tos=%d,tos mask=%x\n"
           "src port=%d,src port mask=%x,dst port=%d,dst port mask=%x\n\n",
            cfg->l3_protocol, cfg->l3_protocol_mask,
            cfg->tos,cfg->tos_mask,cfg->src_port,cfg->src_port_mask,
            cfg->dst_port,cfg->dst_port_mask);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_classify_noip_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_classify_noip_cfg(ATG_DCI_QOS_CLASSIFY_NONE_IP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        return;
    }
    printf("noip_cfg");
    printf("etype:%x,etype mask:%x\n\n", cfg->etype, cfg->etype_mask);
    printf("payload 0-4=%x,%x,%x,%x\n",cfg->payload[0],cfg->payload[1],cfg->payload[2],cfg->payload[3]);
    printf("payload 4-8=%x,%x,%x,%x\n",cfg->payload[4],cfg->payload[5],cfg->payload[6],cfg->payload[7]);
    printf("payload 9-12=%x,%x,%x,%x\n\n",cfg->payload[8],cfg->payload[9],cfg->payload[10],cfg->payload[11]);
    printf("mask=%x,%x,%x,%x\n%x,%x,%x,%x\n%x,%x,%x,%x\n\n\n",cfg->payload_mask[0],
        cfg->payload_mask[1],cfg->payload_mask[2],cfg->payload_mask[3],cfg->payload_mask[4],
        cfg->payload_mask[5],cfg->payload_mask[6],cfg->payload_mask[7],cfg->payload_mask[8],
        cfg->payload_mask[9],cfg->payload_mask[10],cfg->payload_mask[11]);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_classify_ipUdp_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_classify_ipudp_cfg(ATG_DCI_QOS_CLASSIFY_IPV4_UDP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("ipUdp_cfg\n");
    printf("dst ip:");
    spm_print_ipv4(cfg->dst_ip);
    printf("dst ip mask:");
    spm_print_ipv4(cfg->dst_ip_mask);
    printf("src ip:");
    spm_print_ipv4(cfg->src_ip);
    printf("src ip mask:");
    spm_print_ipv4(cfg->src_ip_mask);
    printf("tos:%d,tos mask=%x\ndst port=%d,mask=%x,src port=%d,mask=%x\n\n",
            cfg->dscp, cfg->dscp_mask,cfg->dst_port,cfg->dst_port_mask,
            cfg->src_port,cfg->src_port_mask);
    return;
}

#ifdef R8000_V3R2

/******************************************************************************
 * FunctionName 	: 	spm_print_classify_portvlan_cfg
 * Author		: 	    dengkai
 * CreateDate		:	2015-3-11
 * Description		:   KEY 
 * InputParam	:	    KEY
 * OutputParam	:	    no
 * ReturnValue	:	    rv
 * Relation		:	    called by L3    
 * OtherInfo		:	
******************************************************************************/
NBB_VOID spm_print_classify_portvlan_cfg(ATG_DCI_QOS_CLASSIFY_PORT_VLAN *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("portVlan_cfg\n");
    printf("svlan_id = %d,svlan_mask = 0x%x;\n",cfg->svlan_id,cfg->svlan_mask);
    printf("cvlan_id = %d,cvlan_mask = 0x%x.",cfg->cvlan_id,cfg->cvlan_mask);
    printf("\n");

    return;
}

#endif 

/*****************************************************************************
   函 数 名  : spm_print_action_basic_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_basic_cfg(ATG_DCI_QOS_BEHAVIOR_BASIC_DATA *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_print_action_supervise_color(ATG_DCI_QOS_BEHAVIOR_SUPERV_ACT cfg)
{
    printf("markphb=%s,action=%s\n",supervise_markphb_string[cfg.markphb],supervise_action_string[cfg.action]);
}



#ifdef PTN690

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_print_policy_supervise_cb(SPM_QOS_POLICY_SUPERVISE_CB *pcb)
{
    if(NULL != pcb)
    {
        printf("policy_id=%ld,classify_id=%ld,meter_bucket_id=%ld\n",
            pcb->policy_supervise_key.policy_id,
            pcb->policy_supervise_key.classify_id,
            pcb->meter_bucket_id);
    }
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_print_action_policy_supervise_cfg(SPM_QOS_ACTION_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_SUPERVISE_CB *cfg_cb = NULL;
        
    if(NULL != pcb)
    {
        printf("\ncolor_action_id=%ld\n\n",pcb->color_action_id);
        for(cfg_cb = (SPM_QOS_POLICY_SUPERVISE_CB*)AVLL_FIRST(pcb->spm_qos_supervise_tree); cfg_cb != NULL;
            cfg_cb = (SPM_QOS_POLICY_SUPERVISE_CB*)AVLL_NEXT(pcb->spm_qos_supervise_tree,cfg_cb->policy_supervise_node))
        {
            spm_print_policy_supervise_cb(cfg_cb);
        }
    }
    printf("\n");
    
    return;
}

#endif

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_print_action_supervise_cfg(ATG_DCI_QOS_BEHAVIOR_SUPERVISE *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("supervise_cfg:mode=%s,cir=%ld,pir=%ld,cbs=%d,pbs=%d,cm_color=%s,"
           "coupling_flag=%s\n", supervise_mode_string[cfg->mode], cfg->cir,
            cfg->pir, cfg->cbs, cfg->pbs,supervise_color_mode_string[cfg->cm_color],
            supervise_coupling_flag_string[cfg->coupling_flag]);
    printf("green action:");
    spm_print_action_supervise_color(cfg->green_action);
    printf("yellow action:");
    spm_print_action_supervise_color(cfg->yellow_action);
    printf("red action:");
    spm_print_action_supervise_color(cfg->red_action);
    printf("\n");
    
    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_shaping_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_shaping_cfg(ATG_DCI_QOS_BEHAVIOR_SHAPPING *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("action_shaping_cfg:cir=%ld,pir=%ld,cbs=%d,pbs=%d\n\n",
           cfg->cir, cfg->cir + cfg->pir, cfg->cbs, cfg->pbs);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_schedule_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_schedule_cfg(ATG_DCI_QOS_BEHAVIOR_SCHEDULE *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("action_schedule_cfg:mode=%s,weight=%d\n\n",
           cfg->mode ? "WFQ" : "SP", cfg->weight);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_flow_act_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_flow_act_cfg(ATG_DCI_QOS_BEHAVIOR_FLOW_ACT *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("action_flow_act_cfg:action=%s\n\n", 
           flow_action_mode_string[cfg->action]);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_flow_samp_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_flow_samp_cfg(ATG_DCI_QOS_BEHAVIOR_FLOW_SAMPL *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("action_flow_samp_cfg:phy_index=%ld\n,sampling_mode=%s\n,sampling_percent=%s\n\n", 
           cfg->phy_index, cfg->sampling_mode ? "随机采样" : "固定比率采样", 
           sampling_percent_string[cfg->sampling_percent]);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_redirect_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_redirect_cfg(ATG_DCI_QOS_BEHAVIOR_REDIRECT *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("action_redirect_cfg:action=%s,vrf_d=%ld,port_index=%ld\n\n", 
           redirect_mode_string[cfg->action], cfg->vrf_d, cfg->out_port_index);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_policy_rt_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_policy_rt_cfg(ATG_DCI_QOS_BEHAVIOR_POLICY_RT *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;

    if (NULL == cfg)
    {
        return;
    }
    printf("action_policy_rt_cfg:nxhop_num=%d\n\n", cfg->nxhop_num);
    for (i = 0; i < cfg->nxhop_num; i++)
    {
        printf("nxhop_port_index=%ld\n", cfg->nxhp[i].nxhop_port_index);
        if(0 == cfg->nxhp[i].ip_type)
        {
            spm_print_ipv4(cfg->nxhp[i].nxhop_ip[3]);
        }
        else
        {
            spm_print_ipv6(&(cfg->nxhp[i].nxhop_ip[0]));
        }  
    }
    printf("\n");

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_phb_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_phb_cfg(ATG_DCI_QOS_BEHAVIOR_PHB *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_domain_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_domain_cfg(ATG_DCI_QOS_BEHAVIOR_DOMAIN *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        return;
    }
    printf("action_domain_cfg:\n");
    printf(
        "ovlan_pri_enable = %s,ovlan_pri=%d,\
        ivlan_pri_enable=%s,ivlan_pri=%d,\
        ipv4_enable=%s,ipv4_dscp_tos=%d,\
        df_enable=%s,df=%d\n\n",
        cfg->ovlan_pri_enable ? "使能" : "不使能",
        cfg->ovlan_pri,
        cfg->ivlan_pri_enable ? "使能" : "不使能",
        cfg->ivlan_pri,
        cfg->ipv4_enable ? "使能" : "不使能",
        cfg->ipv4_dscp_tos,
        cfg->df_enable ? "使能" : "不使能",
        cfg->df);

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_urpf_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_urpf_cfg(ATG_DCI_QOS_BEHAVIOR_URPF *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    if (NULL == cfg)
    {
        return;
    }
    printf("action_urpf_cfg:mode = %s\n\n", cfg->mode ? "松散" : "严格");

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_que_shaping_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_que_shaping_cfg(NBB_USHORT num,ATG_DCI_QOS_BEHAVIOR_QUE_SHAPE *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;

    printf("action_que_shaping_cfg:\n");
    for (i = 0; i < num; i++)
    {
        if (NULL == cfg)
        {
            return;
        }
        printf("queue_id = %d,cir=%ld,pir=%ld,cbs=%d,pbs=%d\n\n", 
                cfg->queue_id, cfg->cir,cfg->cir + cfg->pir, 
                cfg->cbs, cfg->pbs);
        cfg++;
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_que_schedule_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_que_schedule_cfg(NBB_USHORT num,ATG_DCI_QOS_BEHAVIOR_QUE_SCHEDL *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;

    printf("action_que_schedule_cfg:\n");
    for (i = 0; i < num; i++)
    {
        if (NULL == cfg)
        {
            return;
        }
        printf("queue_id = %d,mode=%s,weight=%d\n\n",cfg->queue_id,cfg->mode?"WFQ":"SP",cfg->weight);
        cfg++;
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_print_action_que_congest_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_action_que_congest_cfg(NBB_USHORT num,ATG_DCI_QOS_BEHAVIOR_QUE_CONGST *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BYTE i = 0;

    printf("action_que_congest_cfg:\n");
    for (i = 0; i < num; i++)
    {
        if (NULL == cfg)
        {
            return;
        }
        printf("queue_id = %d,mode=%s,green_wred_id=%d,yellow_wred_id=%d,red_wred_id=%d\n\n",
                cfg->queue_id, wred_mode_string[cfg->mode],cfg->green_wred_id,
                cfg->yellow_wred_id,cfg->red_wred_id);
        cfg++;
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_show_acl_cfg
   功能描述  : 显示配置
   输入参数  : acl index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID qos_show_acl_group(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_ACL_CB *pcb = NULL;
    NBB_ULONG flag = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    if (0 == index) /*非法*/
    {
        printf("**ERROR** acl index = 0 ");
        goto EXIT_LABEL;
    }
    for(pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST((v_spm_shared->g_acl_id_instance[index].acl_group));
        pcb != NULL; pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pcb->spm_acl_group_node))
    {
        flag = ATG_DCI_RC_UNSUCCESSFUL;

        spm_print_acl_basic_cfg(pcb->basic_cfg_cb);
        spm_print_acl_port_cfg(pcb->Port_cfg_cb);
        spm_print_acl_eth_cfg(pcb->Eth_cfg_cb);
        spm_print_acl_ipv4_cfg(pcb->ipv4_cfg_cb);
        spm_print_acl_iptcp_cfg(pcb->ipTcp_cfg_cb);
        spm_print_acl_ipv6_cfg(pcb->ipv6_cfg_cb);
        spm_print_acl_noip_cfg(pcb->noIp_cfg_cb);
        spm_print_acl_ipudp_cfg(pcb->ipUdp_cfg_cb);
#ifdef R8000_V3R2
        spm_print_acl_portvlan_cfg(pcb->portVlan_cfg_cb);    
#endif

        printf("acl id =%ld,rule id =%ld,posid=%ld,flag=%s,pri_id =%d,meter_id=%ld,samp_id=%ld,cnt = %ld\n\n\n",
                pcb->acl_key.acl_id, pcb->acl_key.rule_id,pcb->posid,acl_flag_string[pcb->flag], 
                pcb->pri_id, pcb->meter_id, pcb->samp_id, pcb->cnt);
    }

    /*查不到acl 配置*/
    if (ATG_DCI_RC_OK == flag)
    {
        printf("can't find acl cfg\n");
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_acl(NBB_ULONG acl_id,NBB_ULONG rule_id)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_ACL_CB *pcb = NULL;
    NBB_ULONG flag = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    if (0 == acl_id) /*非法*/
    {
        printf("**ERROR** acl index = 0 ");
        goto EXIT_LABEL;
    }
    for (pcb = (SPM_QOS_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_acl_tree); pcb != NULL;
         pcb = (SPM_QOS_ACL_CB*) AVLL_NEXT(v_spm_shared->qos_acl_tree,pcb->spm_acl_node))
    {
        if((acl_id == pcb->acl_key.acl_id) && (rule_id == pcb->acl_key.rule_id))
        {
            flag = ATG_DCI_RC_UNSUCCESSFUL;
 
            spm_print_acl_basic_cfg(pcb->basic_cfg_cb);
            spm_print_acl_port_cfg(pcb->Port_cfg_cb);
            spm_print_acl_eth_cfg(pcb->Eth_cfg_cb);
            spm_print_acl_ipv4_cfg(pcb->ipv4_cfg_cb);
            spm_print_acl_iptcp_cfg(pcb->ipTcp_cfg_cb);
            spm_print_acl_ipv6_cfg(pcb->ipv6_cfg_cb);
            spm_print_acl_noip_cfg(pcb->noIp_cfg_cb);
            spm_print_acl_ipudp_cfg(pcb->ipUdp_cfg_cb);
#ifdef R8000_V3R2
            spm_print_acl_portvlan_cfg(pcb->portVlan_cfg_cb);    
#endif
            printf("acl id =%ld,rule id =%ld,posid=%ld,flag=%s,pri_id =%d,meter_id=%ld,samp_id=%ld,cnt = %ld\n\n\n",
                    pcb->acl_key.acl_id, pcb->acl_key.rule_id,pcb->posid,acl_flag_string[pcb->flag], 
                    pcb->pri_id, pcb->meter_id, pcb->samp_id, pcb->cnt);
         }
    }

    /*查不到acl 配置*/
    if (ATG_DCI_RC_OK == flag)
    {
        printf("can't find acl cfg\n");
        goto EXIT_LABEL;
    }

    EXIT_LABEL:
    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_acl_pri(NBB_BYTE set)
{
    acl_pri_setting = set;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_phyprint(NBB_BYTE set)
{
    qos_phy_cfg_print = set;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_logprint(NBB_BYTE set)
{
    qos_log_cfg_print = set;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_vcprint(NBB_BYTE set)
{
    qos_vc_cfg_print = set;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_vrfprint(NBB_BYTE set)
{
    qos_vrf_cfg_print = set;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_txlspprint(NBB_BYTE set)
{
    qos_txlsp_cfg_print = set;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_pwprint(NBB_BYTE set)
{
    qos_pw_cfg_print = set;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_defendprint(NBB_BYTE set)
{
    qos_defend_cfg_print = set;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_acl_actionprint(NBB_BYTE set)
{
    qos_acl_action_print = set;
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_set_print(NBB_BYTE set)
{
   qos_set_phyprint(set); 
   qos_set_logprint(set);
   qos_set_vcprint(set);
   qos_set_vrfprint(set);
   qos_set_txlspprint(set);
   qos_set_pwprint(set);
   qos_set_defendprint(set);
   qos_set_acl_actionprint(set);
}

#ifdef PTN690

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_print_policy_classify_cb(SPM_QOS_POLICY_CLASSIFY_CB *pcb)
{
    if(NULL != pcb)
    {
        printf("policy_id=%ld,acl_id=%ld,rule_id=%ld,posid=%ld\n",
            pcb->policy_classify_key.policy_id,pcb->policy_classify_key.acl_id,
            pcb->rule_id,pcb->posid);
    }
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID spm_print_classify_policy_cfg(SPM_QOS_CLASSIFY_CB *pcb)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_CLASSIFY_CB *cfg_cb = NULL;
        
    if(NULL != pcb)
    {
        for(cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_FIRST(pcb->spm_classify_policy_tree); cfg_cb != NULL;
            cfg_cb = (SPM_QOS_POLICY_CLASSIFY_CB*)AVLL_NEXT(pcb->spm_classify_policy_tree,cfg_cb->tree_node))
        {
            spm_print_policy_classify_cb(cfg_cb);
        }
    }
    printf("\n");
    
    return;
}

#endif

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_LONG qos_show_classify_group(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_CLASSIFY_CB *pstClassify = NULL;
    NBB_ULONG flag = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (pstClassify = (SPM_QOS_CLASSIFY_CB*)NBB_NEXT_IN_LIST((  \
                       v_spm_shared->g_classify_id_instance[index].classify_group)); 
                       pstClassify != NULL; 
                       pstClassify = (SPM_QOS_CLASSIFY_CB *)NBB_NEXT_IN_LIST(pstClassify->spm_classify_group_node))
    {
        flag = ATG_DCI_RC_UNSUCCESSFUL;
        printf("classify index=%ld,if_match_id=%ld\n",pstClassify->classify_key.qos_classify_index,
                pstClassify->classify_key.if_match_id);
  
        spm_print_classify_basic_cfg(pstClassify->basic_cfg_cb);

        spm_print_classify_port_cfg(pstClassify->Port_cfg_cb);

        spm_print_classify_eth_cfg(pstClassify->Eth_cfg_cb);

        spm_print_classify_ipv4_cfg(pstClassify->ipv4_cfg_cb);

        spm_print_classify_iptcp_cfg(pstClassify->ipTcp_cfg_cb);

        spm_print_classify_ipv6_cfg(pstClassify->ipv6_cfg_cb);

        spm_print_classify_noip_cfg(pstClassify->noIp_cfg_cb);
 
        spm_print_classify_ipudp_cfg(pstClassify->ipUdp_cfg_cb);
        
#ifdef R8000_V3R2
        spm_print_classify_portvlan_cfg(pstClassify->portVlan_cfg_cb);
#endif

        printf("rule_id=%ld, classify cnt = %ld,match type=%d\n", pstClassify->rule_id,
            pstClassify->cnt,pstClassify->match_type); 
    }
    if (ATG_DCI_RC_OK == flag)
    {
        printf("can't find classify cfg\n");
    }
    
    return flag;
}


/*****************************************************************************
   函 数 名  : spm_show_classify_cfg
   功能描述  : 显示配置
   输入参数  : classify index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID qos_show_classify(NBB_ULONG index,NBB_ULONG match_id)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_CLASSIFY_CB *pcb = NULL;
    NBB_ULONG flag = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for(pcb = (SPM_QOS_CLASSIFY_CB *)AVLL_FIRST(v_spm_shared->qos_classify_tree); pcb != NULL;
        pcb = (SPM_QOS_CLASSIFY_CB *)AVLL_NEXT(v_spm_shared->qos_classify_tree, pcb->spm_classify_node))
    {
        flag = ATG_DCI_RC_UNSUCCESSFUL;
        if((match_id == pcb->classify_key.if_match_id) &&
            (index == pcb->classify_key.qos_classify_index))
        {
            spm_print_classify_basic_cfg(pcb->basic_cfg_cb);
            spm_print_classify_port_cfg(pcb->Port_cfg_cb);
            spm_print_classify_eth_cfg(pcb->Eth_cfg_cb);
            spm_print_classify_ipv4_cfg(pcb->ipv4_cfg_cb);
            spm_print_classify_iptcp_cfg(pcb->ipTcp_cfg_cb);
            spm_print_classify_ipv6_cfg(pcb->ipv6_cfg_cb);
            spm_print_classify_noip_cfg(pcb->noIp_cfg_cb);
            spm_print_classify_ipudp_cfg(pcb->ipUdp_cfg_cb);
#ifdef R8000_V3R2
            spm_print_classify_portvlan_cfg(pcb->portVlan_cfg_cb);
#endif
#ifdef PTN690
            spm_print_classify_policy_cfg(pcb);
#else
            printf("rule_id=%ld, classify cnt = %ld\n", pcb->rule_id, pcb->cnt);
#endif
            
        }
    }
    if (ATG_DCI_RC_OK == flag)
    {
        printf("can't find classify cfg\n");
    }
    
    return;
}


/*****************************************************************************
   函 数 名  : spm_show_ation_cfg
   功能描述  : 显示配置
   输入参数  : action index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID qos_show_behavior(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_ACTION_CB *pcb = NULL;
    NBB_ULONG flag = ATG_DCI_RC_OK;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for(pcb = (SPM_QOS_ACTION_CB *)AVLL_FIRST(v_spm_shared->qos_action_tree); pcb != NULL;
        pcb = (SPM_QOS_ACTION_CB *)AVLL_NEXT(v_spm_shared->qos_action_tree, pcb->spm_action_node))
    {
        flag = ATG_DCI_RC_UNSUCCESSFUL;
        if((0 == index) || (index == pcb->action_key))
        {
            if(0 != index)
            {
                printf("key = %ld\n", pcb->action_key);
            }
            spm_print_action_basic_cfg(pcb->basic_cfg_cb);
#ifdef PTN690
            spm_print_action_policy_supervise_cfg(pcb);
#endif
            spm_print_action_supervise_cfg(pcb->supervise_cfg_cb);
            spm_print_action_shaping_cfg(pcb->shaping_cfg_cb);
            spm_print_action_schedule_cfg(pcb->schedule_cfg_cb);
            spm_print_action_flow_act_cfg(pcb->flow_act_cfg_cb);
            spm_print_action_flow_samp_cfg(pcb->flow_samping_cfg_cb);
            spm_print_action_redirect_cfg(pcb->redirect_cfg_cb);
            spm_print_action_policy_rt_cfg(pcb->policy_nhi_cfg_cb);
            spm_print_action_phb_cfg(pcb->phb_cfg_cb);
            spm_print_action_domain_cfg(pcb->domain_cfg_cb);
            spm_print_action_urpf_cfg(pcb->urpf_cfg_cb);
            printf("que_shape_flag = %d\n", pcb->que_shape_flag);
            spm_print_action_que_shaping_cfg(pcb->que_shape_num,pcb->que_shaping_cfg_cb);
            spm_print_action_que_schedule_cfg(pcb->que_schedul_num,pcb->que_schedul_cfg_cb);
            spm_print_action_que_congest_cfg(pcb->que_congest_num,pcb->que_congst_cfg_cb);
            printf("action cnt = %ld,meter_id=%ld\n", pcb->cnt,pcb->meter_id);
        }
    }
  
    if (ATG_DCI_RC_OK == flag)
    {
        printf("can't find behavior cfg\n");
    }
    
    return;
}


/*****************************************************************************
   函 数 名  : spm_show_policy_cfg
   功能描述  : 显示配置
   输入参数  : policy index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID qos_show_policy(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_POLICY_CB *pcb = NULL;
    SPM_QOS_FLOW_CLASSIFY_CB *pstClassify = NULL;
    SPM_QOS_POLICY_C2B_CB *join_cb = NULL;
    NBB_USHORT i = 0;

    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    /* 非法 */
    if (0 == index) 
    {
        printf("**ERROR** policy index = 0 ");
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(v_spm_shared->qos_policy_tree, &index);

    /* 查不到policy策略模板配置 */
    if (NULL == pcb)
    {
        printf("can't find policy cfg\n");
        goto EXIT_LABEL;
    }

    if(NULL == pcb->basic_cfg_cb)
    {
        printf("**ERROR**basic_cfg_cb == NULL\n");
        goto EXIT_LABEL;
    }

    printf("mode = %s,c2b num=%d\n\n",
           policy_mode_string[pcb->basic_cfg_cb->mode], pcb->c2b_num);
    for (join_cb = (SPM_QOS_POLICY_C2B_CB *)AVLL_FIRST(pcb->c2b_tree); join_cb != NULL;
         join_cb = (SPM_QOS_POLICY_C2B_CB *)AVLL_NEXT(pcb->c2b_tree, join_cb->spm_policy_c2b_node))
    {
        printf("action index = %ld , ", join_cb->c2b_key.qos_behavior_index);
        printf("classify index = %ld\n", join_cb->c2b_key.qos_classify_index);
    }
    printf("policy cnt = %ld\n\n", pcb->cnt);

    /* 调度节点模式 */
    if(0 == pcb->basic_cfg_cb->mode)
    {
        goto EXIT_LABEL;
    }
    
#ifdef PTN690

    /* 非共享模式 */
    if(0 == pcb->basic_cfg_cb->share)
    {   
        printf("no share mode instanse :\n");
        for(i = 0; (i < POLICY_C2B_INSTANCE_MAX) && (0 != pcb->c2b_instance_tree[i].cnt);i++)
        {
            printf("  sz_offset=%d, acl id=%ld, cnt=%ld\n",
                i,pcb->c2b_instance_tree[i].acl_id,pcb->c2b_instance_tree[i].cnt);
            for(pstClassify = (SPM_QOS_FLOW_CLASSIFY_CB *)AVLL_FIRST(
                                    pcb->c2b_instance_tree[i].instance_tree);
                pstClassify != NULL;
                pstClassify = (SPM_QOS_FLOW_CLASSIFY_CB *)AVLL_NEXT(
                                    pcb->instance_tree, pstClassify->spm_classify_id_node))
            {
                printf("  log id=%ld,posid=%ld,acl_id=%ld,intf_offset=%d,sz_offset=%d.\n",
                    pstClassify->key.index,pstClassify->sub_port.posid,pstClassify->acl_id,
                    pstClassify->intf_offset,pstClassify->sz_offset);
            }
            printf("\n");
        }
    }
    else
    {
        printf("share mode instanse:acl id=%ld\n\n",pcb->acl_id);
        for(pstClassify = (SPM_QOS_FLOW_CLASSIFY_CB *)AVLL_FIRST(pcb->instance_tree); pstClassify != NULL;
            pstClassify = (SPM_QOS_FLOW_CLASSIFY_CB *)AVLL_NEXT(pcb->instance_tree, 
                               pstClassify->spm_classify_id_node))
        {       
            printf("\t%ld",pstClassify->key.index);
        }
        printf("\n acl id=%ld,c2b num=%d\n\n",pcb->acl_id,pcb->c2b_num);
    }
#else
    printf("acl id=%ld\n\n",pcb->acl_id);
    for(pstClassify = (SPM_QOS_FLOW_CLASSIFY_CB *)AVLL_FIRST(pcb->instance_tree); pstClassify != NULL;
        pstClassify = (SPM_QOS_FLOW_CLASSIFY_CB *)AVLL_NEXT(pcb->instance_tree, pstClassify->spm_classify_id_node))
    {       
        printf("\tlog key = %ld",pstClassify->key.index);
    }
    
#endif 

    EXIT_LABEL:
    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_upusr(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_UP_USR_CB *pcb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();



    //pcb = AVLL_FIND(v_spm_shared->qos_up_usr_tree, &index);

     for(pcb = (SPM_QOS_UP_USR_CB*)AVLL_FIRST(v_spm_shared->qos_up_usr_tree);
        pcb != NULL;
        pcb = (SPM_QOS_UP_USR_CB*)AVLL_NEXT(v_spm_shared->qos_up_usr_tree, pcb->spm_qos_up_usr_node))
    {
        if((index == pcb->key.index) || (0 == index))
        {
           printf("logic index=%ld,svlan=%d,cvlan=%d,posid=%ld,policy index=%ld\n",
            pcb->key.index,pcb->key.svlan,pcb->key.cvlan,
            pcb->sub_port.posid,pcb->policy_index); 
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_show_wred_cfg
   功能描述  : 显示配置
   输入参数  : wred index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年1月15日 星期二
    作    者   : zenglu
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID qos_show_wred(NBB_ULONG index)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_QOS_WRED_CB *pcb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    if (0 == index) /*非法*/
    {
        printf("**ERROR** wred index = 0 ");
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(v_spm_shared->qos_wred_tree, &index);

    /*查不到wred 配置*/
    if (NULL == pcb)
    {
        printf("can't find wred cfg\n");
        goto EXIT_LABEL;
    }

    if(NULL == pcb->basic_cfg_cb)
    {
        printf("**ERROR**basic_cfg_cb == NULL\n");
        goto EXIT_LABEL;
    }

    printf("start_threshold = %d\n", pcb->basic_cfg_cb->start_threshold);
    printf("end_threshold = %d\n", pcb->basic_cfg_cb->end_threshold);
    printf("discard_percent = %d\n", pcb->basic_cfg_cb->discard_percent);

    printf("wred cnt = %ld\n", pcb->cnt);

    EXIT_LABEL:

    return;

}






/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_voq(NBB_USHORT slot, NBB_USHORT dstport)
{
    NBB_ULONG offset = MAX_SLOT_NUM * NUM_COS;
    NBB_ULONG baseQueue = 0;
    
    baseQueue = offset * dstport + VOQ_OFFSET + NUM_COS * (slot - 1);

    printf("voq =%ld\n", baseQueue);

}



/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_pri2phb_ds_offset(NBB_LONG dsptr, NBB_BYTE pri)
{

    printf("pri2phb and exp2phb offset=%ld\n", dsptr * 8 + pri);
    printf("dscp2phb offset=%ld\n", dsptr * 64 + pri);

}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_phb2pri_ds_offset(NBB_LONG dsptr, NBB_BYTE phb, NBB_BYTE color)
{
    printf(" phb2pri=%ld\n", dsptr * 24 + phb * 3 + color);
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_acl_by_id(NBB_ULONG acl_id,NBB_ULONG rule_id)
{
    SPM_QOS_ACL_CB *pcb = NULL;
    SPM_QOS_LOGIC_ACL_CB *pB = NULL;
    NBB_ULONG flag = ATG_DCI_RC_OK;
    
    //SPM_QOS_LOGIC_ACL_CB *join_cb;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    if(acl_id > 4900)
    {
        printf("acl id too large\n");
        return;
    }

    for (pcb = (SPM_QOS_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_acl_tree);
         pcb != NULL;
         pcb = (SPM_QOS_ACL_CB*) AVLL_NEXT(v_spm_shared->qos_acl_tree,
                       pcb->spm_acl_node))  
    {
        if((0 == acl_id) || (acl_id == pcb->acl_key.acl_id))
        {
            if((0 == rule_id) || (rule_id == pcb->acl_key.rule_id))
            {
                printf("***acl_id=%ld*****rule_id=%ld*****posid=%ld****\n",
                    pcb->acl_key.acl_id,pcb->acl_key.rule_id,
                    pcb->posid);
                if(NULL != pcb->basic_cfg_cb)
                {
                   flag = ATG_DCI_RC_UNSUCCESSFUL;
                   switch(pcb->basic_cfg_cb->match_type)
                   {
                       case MATCH_ERROR:
                        printf("**ERROR**match_type=%d\n",pcb->basic_cfg_cb->match_type);
                        break;
                       case MATCH_PORT:
                        spm_print_acl_port_cfg(pcb->Port_cfg_cb);
                        break;
                       case MATCH_ETH_HEADER:
                        spm_print_acl_eth_cfg(pcb->Eth_cfg_cb);
                        break;
                       case MATCH_IPV4:
                        spm_print_acl_ipv4_cfg(pcb->ipv4_cfg_cb);
                        break;
                       case MATCH_IP_TCP:
                        spm_print_acl_iptcp_cfg(pcb->ipTcp_cfg_cb);
                        break;
                       case MATCH_IP_UDP:
                        spm_print_acl_ipudp_cfg(pcb->ipUdp_cfg_cb);
                        break;
                       case MATCH_IPV6:
                        spm_print_acl_ipv6_cfg(pcb->ipv6_cfg_cb);
                        break;
                       case MATCH_NOIP:
                        spm_print_acl_noip_cfg(pcb->noIp_cfg_cb);
                        break;
#ifdef R8000_V3R2
                       case MATCH_PORT_VLAN:
                        spm_print_acl_portvlan_cfg(pcb->portVlan_cfg_cb);
                        break;
#endif
                       default:
                        printf("**ERROR**match_type=%d\n",pcb->basic_cfg_cb->match_type);
                        break;
                   }
                }
                else
                {
                    printf("basic_cfg_cb== NULL\n");
                }
            }
            
        }
    }

    if(0 != acl_id)
    {
        for (pB = (SPM_QOS_LOGIC_ACL_CB*)AVLL_FIRST((v_spm_shared->g_acl_id_instance[acl_id].instance_tree));
                        pB != NULL;
                        pB = (SPM_QOS_LOGIC_ACL_CB *)AVLL_NEXT(      \
                                v_spm_shared->g_acl_id_instance[acl_id].instance_tree, pB->spm_acl_id_node))
        {
                printf("logic key=%ld,classify id=%d,slot=%d,unit=%d,port=%d,posid=%ld,svlan=%d,cvlan=%d\n\n",
                    pB->key.index,pB->key.id,
                    pB->sub_port.slot,pB->sub_port.unit,
                    pB->sub_port.port,
                    pB->sub_port.posid,
                    pB->sub_port.ovlan,
                    pB->sub_port.ivlan);  
        }
    }
   

    
    if (ATG_DCI_RC_OK == flag)
    {
        printf("can't find acl cfg\n");
    }


}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_acl_summary(NBB_ULONG acl_id)
{
    SPM_QOS_ACL_CB *pcb = NULL;
    NBB_ULONG globle_cnt = 0;
    NBB_ULONG inuse_cnt = 0;
    NBB_ULONG acl_num = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();



    for (pcb = (SPM_QOS_ACL_CB*) AVLL_FIRST(v_spm_shared->qos_acl_tree);
         pcb != NULL;
         pcb = (SPM_QOS_ACL_CB*) AVLL_NEXT(v_spm_shared->qos_acl_tree,
                       pcb->spm_acl_node))   
    {
        globle_cnt++;

        if(MATCH_ERROR != pcb->match_tpye)
        {
            inuse_cnt++;
        }
        if((0 == acl_id) || (acl_id == pcb->acl_key.acl_id))
        {
            acl_num++;
        }
    }         

    printf("acl all cfg num=%ld,in used cfg=%ld,acl_id=%ld,acl cfg num=%ld\n",
        globle_cnt,inuse_cnt,acl_id,acl_num);

}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_acl_by_intf(NBB_ULONG log_index)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_LOGIC_ACL_CB *cfg_cb = NULL;
    SPM_QOS_ACL_CB *pcb = NULL;
    NBB_LONG cnt = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();



    for (cfg_cb = (SPM_QOS_LOGIC_ACL_CB *)AVLL_FIRST(v_spm_shared->qos_filter_tree);
        cfg_cb != NULL;
        cfg_cb = (SPM_QOS_LOGIC_ACL_CB *)AVLL_NEXT(v_spm_shared->qos_filter_tree,
            cfg_cb->spm_acl_node))
    {
        if ((0 == log_index) || (cfg_cb->key.index == log_index))
        {
            cnt = 0;

            printf("acl_index=%ld,direct=%s classify_id=%d ",
                cfg_cb->filter.acl_id, acl_direct_string[cfg_cb->filter.direction],cfg_cb->filter.classify_id);
                for (pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST((     \
                v_spm_shared->g_acl_id_instance[cfg_cb->filter.acl_id].acl_group)); 
                pcb != NULL;
                pcb = (SPM_QOS_ACL_CB *)NBB_NEXT_IN_LIST(pcb->spm_acl_group_node))
            {
                cnt++;
            }
            printf("cnt=%ld\n\n",cnt);
        }
    }

    return;
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_logic_classify_cfg(NBB_ULONG log_index)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_QOS_FLOW_CLASSIFY_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    for (cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB *)AVLL_FIRST(v_spm_shared->qos_flow_classify_tree);
        cfg_cb != NULL;
        cfg_cb = (SPM_QOS_FLOW_CLASSIFY_CB *)AVLL_NEXT(v_spm_shared->qos_flow_classify_tree,
            cfg_cb->spm_qos_flow_classify_node))
    {
        if ((0 == log_index) || (cfg_cb->key.index == log_index))
        {

            printf("policy_index=%ld,svlan=%d,cvlan=%d\n\n",
                cfg_cb->policy_index,cfg_cb->key.svlan,cfg_cb->key.cvlan);
        }
    }

    return;
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_port_wred_cfg(NBB_USHORT port)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    SPM_PORT_WRED_CB *cfg_cb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();


    for (cfg_cb = (SPM_PORT_WRED_CB *)AVLL_FIRST(v_spm_shared->port_wred_tree);
        cfg_cb != NULL;
        cfg_cb = (SPM_PORT_WRED_CB *)AVLL_NEXT(v_spm_shared->port_wred_tree,
            cfg_cb->spm_port_wred_node))
    {
        if ((0 == port) || (cfg_cb->port_wred_key.index == port))
        {

            printf("cos=%d,wred mode=%s,green=%d,yellow=%d,red=%d\n\n",
                cfg_cb->port_wred_key.cos,wred_mode_string[cfg_cb->cfg.mode],
                cfg_cb->cfg.green, cfg_cb->cfg.yellow, cfg_cb->cfg.red);
        }
    }

    return;
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_meter_summary()
{
    NBB_ULONG id = 0;
    NBB_ULONG cnt = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (id = MAX_PHYSIC_PORT_NUM * 3; id < MAX_METER_NUM; id++)
    {
        if (0 != v_spm_shared->flow_meter_id[id])
        {
            cnt++;
        }
    }
    printf("meter resource %ld used\n",cnt);
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_classify_rule_summary()
{
    NBB_ULONG id = 0;
    NBB_ULONG cnt = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (id = 0; id < MAX_FLOW_CLASSIFY_RULE_ID; id++)
    {
        if (0 != v_spm_shared->flow_rule_id[id]) /*有空闲ID*/
        {
            cnt++;
        }
    }
    printf("rule resource %ld used\n",cnt);
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_policy_acl_summary()
{
    NBB_ULONG id = 0;
    NBB_ULONG cnt = 0;
    
    for (id = CLASSIFY_PORT_ACL_ID; id < CLASSIFY_PORT_ACL_ID + MAX_CLASSIFY_PORT_ACL_ID_NUM; id++)
    {
        if (0 != v_spm_shared->g_classify_port_acl_id[id - CLASSIFY_PORT_ACL_ID]) /*有空闲ID*/
        {
            cnt++;
        }
    }
    printf("port acl id resource %ld used all=99\n",cnt);
    for (id = CLASSIFY_HIGH_ACL_ID; id < CLASSIFY_HIGH_ACL_ID + MAX_CLASSIFY_HIGH_ACL_ID_NUM; id++)
    {
        if (0 != v_spm_shared->g_classify_high_acl_id[id - CLASSIFY_HIGH_ACL_ID]) /*有空闲ID*/
        {
            cnt++;
        }
    }
    printf("high acl id resource %ld used all=1000\n",cnt);
    for (id = CLASSIFY_ETH_ACL_ID; id < CLASSIFY_ETH_ACL_ID + MAX_CLASSIFY_ETH_ACL_ID_NUM; id++)
    {
        if (0 != v_spm_shared->g_classify_eth_acl_id[id - CLASSIFY_ETH_ACL_ID]) /*有空闲ID*/
        {
            cnt++;
        }
    }
    printf("eth acl id resource %ld used all=99\n",cnt);
    
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_twamp_ipv4()
{
    SPM_TWAMP_IPV4_CB *pstCb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();

    for (pstCb = (SPM_TWAMP_IPV4_CB*)AVLL_FIRST(v_spm_shared->twamp_ipv4_tree);pstCb != NULL;
         pstCb = (SPM_TWAMP_IPV4_CB*)AVLL_NEXT(v_spm_shared->twamp_ipv4_tree,pstCb->spm_twamp_ipv4_node))
    {
         printf(
            "twamp_ipv4.vrf_id=%d,twamp_ipv4.ethType=0x0800,twamp_ipv4.sip=0x%lx,twamp_ipv4.dip=0x%lx,\n"
            "twamp_ipv4.sport=0x%x,twamp_ipv4.dport=0x%x,"
            "twamp_ipv4.posId=%ld,twamp_ipv4.groupId=%ld,twamp_ipv4.entryId=%ld\n\n",
            pstCb->key.vrf_id,pstCb->key.sip,pstCb->key.dip,pstCb->key.sport,
            pstCb->key.dport,pstCb->posId,pstCb->groupId,pstCb->entryId);
    }
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_twamp_ipv6()
{
   printf("twamp ipv6 currently not supported.\n");
}


#ifdef PTN690

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_txlsp_car(NBB_ULONG tunnelid)
{
    SPM_QOS_TXLSP_CAR_CB *pstCb = NULL;
    NBB_ULONG Txlsp_Flag = 0;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    printf("\n");
    for (pstCb = (SPM_QOS_TXLSP_CAR_CB*)AVLL_FIRST(v_spm_shared->qos_txlsp_car_tree);
         pstCb != NULL; pstCb = (SPM_QOS_TXLSP_CAR_CB*)AVLL_NEXT(v_spm_shared->qos_txlsp_car_tree,
         pstCb->spm_qos_txlsp_car_node))
    {
        if((0 == tunnelid) || (pstCb->txlsp_key.tunnelid == tunnelid))
        {
            Txlsp_Flag = 1;
            printf(
                "txlsp.ingress=0x%lx,txlsp.egress=0x%lx,txlsp.tunnelid=%ld,txlsp.lspid=%ld,txlsp_posid=%ld:\n"
                "       meter_id=%ld,cm_color=%s,coupling_flag=%s,\n"
                "       mode=%s,cir=%ldMbps,pir=%ldMbps,cbs=%dKb,pbs=%dKb,color_action_id=%ld.\n"
                "       green_action=%d,markphb=%d,yellow_action=%d,markphb=%d,red_action=%d.\n\n",
                pstCb->txlsp_key.ingress,pstCb->txlsp_key.egress,pstCb->txlsp_key.tunnelid,pstCb->txlsp_key.lspid,
                pstCb->txlsp_posid,pstCb->meter_id,supervise_color_mode_string[pstCb->txlsp_car.cm_color],
                supervise_coupling_flag_string[pstCb->txlsp_car.coupling_flag],
                supervise_mode_string[pstCb->txlsp_car.mode],(pstCb->txlsp_car.cir/1000),(pstCb->txlsp_car.pir/1000),
                pstCb->txlsp_car.cbs,pstCb->txlsp_car.pbs,pstCb->color_action_id,pstCb->txlsp_car.green_action.action,
                pstCb->txlsp_car.green_action.markphb,pstCb->txlsp_car.yellow_action.action,
                pstCb->txlsp_car.yellow_action.markphb,pstCb->txlsp_car.red_action.action);
        }
    }
    if(0 == Txlsp_Flag)
    {
        printf("can't find txlsp_car cfg.\n\n");
    }
}




#endif


#if defined (CR8000_SMART) || defined (PTN690)
/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qos_show_pw_car(NBB_ULONG logid)
{
    SPM_QOS_PW_CAR_CB  *pstCb  = NULL;
    NBB_ULONG Pw_Flag = 0;
    SPM_LOGICAL_PORT_CB *pstLogicalPort = NULL;
    SPM_TERMINAL_IF_CB *pstTerminalIfCb = NULL;
    NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    printf("\n");
    for (pstCb = (SPM_QOS_PW_CAR_CB*)AVLL_FIRST(v_spm_shared->qos_pw_car_tree);
         pstCb != NULL; pstCb = (SPM_QOS_PW_CAR_CB*)AVLL_NEXT(v_spm_shared->qos_pw_car_tree,
         pstCb->spm_qos_pw_car_node))
    {
        if((0 == logid) || (pstCb->log_key == logid))
        {
            Pw_Flag = 1;
            printf(
                "log_id=%ld,vc_id=%ld,vc_type=%d,peer_ip=0x%lx,meter_id=%ld.\n"
                "mode=%s,cm_color=%s,cir=%ldMbps,pir=%ldMbps,color_action_id=%ld.\n"
                "green_action=%d,markphb=%d,yellow_action=%d,markphb=%d,red_action=%d:\n",
                pstCb->log_key,pstCb->vc_key.vc_id,pstCb->vc_key.vc_type,pstCb->vc_key.peer_ip,
                pstCb->meter_id,supervise_mode_string[pstCb->pw_car.mode],
                supervise_color_mode_string[pstCb->pw_car.cm_color],(pstCb->pw_car.cir/1000),
                (pstCb->pw_car.pir/1000),pstCb->color_action_id,pstCb->pw_car.green_action.action,
                pstCb->pw_car.green_action.markphb,pstCb->pw_car.yellow_action.action,
                pstCb->pw_car.yellow_action.markphb,pstCb->pw_car.red_action.action);

            pstLogicalPort  = AVLL_FIND(v_spm_shared->logical_port_tree, &(pstCb->log_key));
            if((NULL != pstLogicalPort) && (NULL != pstLogicalPort->phy_cfg_cb) && \
               (NULL != pstLogicalPort->basic_cfg_cb))
            {
                printf("    intf = ");
                for(pstTerminalIfCb = (SPM_TERMINAL_IF_CB*)AVLL_FIRST(pstLogicalPort->terminal_if_tree); 
                    pstTerminalIfCb != NULL; pstTerminalIfCb = (SPM_TERMINAL_IF_CB*)AVLL_NEXT(\
                    pstLogicalPort->terminal_if_tree, pstTerminalIfCb->spm_terminal_if_node))
                {
                    printf("%ld, ",pstTerminalIfCb->intf_pos_id);
                }
                printf("\n");
            }
            printf("\n");
        }
    }
    if(0 == Pw_Flag)
    {
        printf("can't find pw_car cfg.\n\n");
    }
}

#endif

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID show_port_qos_info(NBB_LONG desPort)
{
#if defined (SPU) || defined (SRC)
   DbgQosInfoPrint(0 ,desPort);
#endif
}


/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID show_port_wred(NBB_USHORT slot, NBB_USHORT dstport)
{
    NBB_ULONG offset = MAX_SLOT_NUM * NUM_COS;
    NBB_ULONG baseQueue = 0;

    baseQueue = offset * dstport + VOQ_OFFSET + NUM_COS * (slot - 1);
#if defined (SPU) || defined (SRC)
    DbgQueueWredPrint(0,baseQueue);
#endif
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID show_voq_info(NBB_LONG voq_id)
{
#if defined (SPU) || defined (SRC)
  DiagPrintVoqInfo(voq_id);
#endif
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID show_hqos_lsp(NBB_LONG id)
{
#if defined (SPU) || defined (SRC)
  DbgHQosLspInfoPrint(0, id);
#endif
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID show_hqos_pw(NBB_LONG id)
{
#if defined (SPU) || defined (SRC)
  DbgHQosInfoPrint(0, id);
#endif
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID show_arad_packet()
{
#if defined (SPU) || defined (SRC)
    DiagPrintLastPacketInfo(0);
#endif
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID show_arad_noempty_voq()
{
#if defined (SPU) || defined (SRC)
    DiagIpsNonEmptyQ(0);
#endif
}

/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID show_arad_port_bandwith(NBB_LONG port)
{
#if defined (SPU) || defined (SRC)
    DiagOFPRateGet(0,port);
#endif
}
    
/*****************************************************************************
   函 数 名  : spm_qos_find_classify_cb
   功能描述  : 查找classify模板相关配置
   输入参数  : classify模板的index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :
   修改历史  :
   日    期  : 2013年1月15日 星期二
   作    者  : zenglu
   修改内容  : 新生成函数
*****************************************************************************/
NBB_VOID qoshelp(NBB_VOID)
{
    NBB_CHAR **ppcMsg;

    static NBB_CHAR *pHelpMsg[] = {
        "---------------------------------CFG----------------------------------------------------", "",
        "qos_show_acl_group(acl_id)",               "if key==0 all",
        "qos_show_acl(acl_id,rule_id)",             "if key==0 all",
        "qos_show_classify_group(index)",           "show classify group cfg",
        "qos_show_classify(class_id,if_match_id)",           "show classify cfg",
        "qos_show_behavior(key)",                   "if key==0 all",
        "qos_show_policy(key)",                     "show classiyf key and action key",
        "qos_show_wred(key)",                       "show wred cfg",
        "qos_show_usrgroup(key)",                   "show usr group cfg", 
        "qos_show_ds(key)",                         "show ds cfg",
        "qos_show_dsdomain(key,type)",              "type=0-6(all/dscp2phb/vlan2phb/exp2phb"
                                                    "phb2dscp/phb2vlan/phb2exp)",
        "qos_show_twamp_ipv4()",                    "show all twamp ipv4 cfg",
        "qos_show_twamp_ipv6()",                    "show all twamp ipv6 cfg:"
                                                    "***ipv6 currently not supported***",
#ifdef PTN690
        "qos_show_txlsp_car(tunnelid)",             "show txlsp car cfg",   
#endif
#if defined (CR8000_SMART) || defined (PTN690)
        "qos_show_pw_car(logid)",                   "show pw car cfg",
#endif
        
        "---------------------------cfg print------------------------------", "",
        "qos_set_phyprint(set)",                   "set phy cfg print",
        "qos_set_logprint(set)",                   "set log cfg print",
        "qos_set_vcprint(set)",                    "set vc cfg print",
        "qos_set_vrfprint(set)",                   "set vrf cfg print",
        "qos_set_txlspprint(set)",                 "set txlsp cfg print",
        "qos_set_defendprint(set)",                "set defend cfg print",
        "qos_set_acl_actionprint(set)",            "set acl action print",
        "qos_set_print(set)",                      "set all qos cfg print",
        "-------------------------ACL------------------------------------", "",
        "qos_show_acl_by_id(acl_id,rule_id)",           "show acl detail",
        "qos_show_acl_by_intf(log_key)",                "show log acl",
        "qos_show_acl_summary(acl_id)",                 "show acl resouce",
        "qos_show_meter_summary()",                     "show meter used",
        "qos_show_classify_rule_summary()",             "show classify rule used", 
        "qos_show_policy_acl_summary()",                "show policy acl used",
        "qos_show_logic_classify_cfg(log_key)",         "show log classify",
        "qos_show_port_wred_cfg(port_index)",           "show port wred",
        "qos_set_acl_pri(set)",                         "acl priority",

        "---------------------------------------------------------", "",
        "---------------------SDK--------------------------------", "",
        "---------------------------------------------------------", "",
        "show_arad_packet()",               "show last packet ITMH head",
        "show_port_qos_info(desPort)",      "show port qos",
        "show_port_wred(slot,dstport)",     "show port wred",
        "show_hqos_lsp(id)",                "show hqos lsp info",
        "show_hqos_pw(id)",                 "show hqos pw info",
        "show_voq_info(voq_id)",            "show voq info",
        "show_arad_noempty_voq()",          "show no empty que",
        "show_arad_port_bandwith(port)",    "show arad port bandwith",
        
        NULL
    };

    printf("\n");

    for (ppcMsg = pHelpMsg; *ppcMsg != NULL; ppcMsg += 2)
    {
        if (strlen(*(ppcMsg)) > 45)
        {
            printf(" %s %s\n", *ppcMsg, *(ppcMsg + 1));
        }
        else
        {
            printf(" %-40s %s\n", *ppcMsg, *(ppcMsg + 1));
        }
    }

    printf("\n");
}

#endif

