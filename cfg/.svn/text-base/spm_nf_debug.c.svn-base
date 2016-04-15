/******************************************************************************

                  版权所有 (C), 1999-2013, 烽火通信科技股份有限公司

******************************************************************************
   文 件 名   : spm_nf_debug.c
   版 本 号   : 初稿
   作    者   : fansongbo
   生成日期   : 2013年6月18日
   最近修改   :
   功能描述   : QOS处理
   函数列表   :
   修改历史   :
   1.日    期   : 2013年6月18日
    作    者   : zenglu
    修改内容   : 创建文件

******************************************************************************/
#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

extern unsigned char nf_ctrl_cfg_print_setting; //用于控制命令调试
#if 0
//static char* pri2phb_mode_string[] = {"指配PHB","信任DSCP","信任OVLAN PRI","信任端口","信任EXP"};
static char *supervise_mode_string[] = {"不使能", "MEF", "RFC2698", "RFC2697"};
static char *flow_action_mode_string[] = {"Drop", "Permit", "Sample", "Redirect", "PolicyRouting", "URPF"};
static char *sampling_percent_string[] = {"1:1", "1:2", "1:10", "1:100", "1:1000", "1:10000"};
static char *redirect_mode_string[] = {"关闭重定向", "重定向到VRF ", "重定向到端口"};
static char *wred_mode_string[] = {"队尾丢弃", "色盲", "分颜色WRED"};
static char *acl_direct_string[] = {"入", "出"};
static char *intf_ds_string[] = {"pri", "dscp"};
static char *l3uni_ds_string[] = {"no action", "pri", "dscp", "pri+dscp"};

NBB_VOID spm_print_mac(NBB_BYTE *mac)
{

    if (NULL == mac)
    {
        NBB_PRINTF("mac == null\n");
        return;
    }

    NBB_PRINTF("%x,%x,%x,%x,%x,%x\n", mac[0],
        mac[1], mac[2], mac[3], mac[4], mac[5]);
}

NBB_VOID spm_print_ipv4(NBB_ULONG ip)
{

    if (0 == ip)
    {
        NBB_PRINTF("ip == 0\n");
        return;
    }

    NBB_PRINTF("%d,%d,%d,%d\n", ((ip >> 24) & 0x00ff),
        ((ip >> 16) & 0x00ff), ((ip >> 8) & 0x00ff), (ip & 0x00ff));
}

NBB_VOID spm_print_ipv6(NBB_ULONG *ip)
{

    NBB_BYTE i;

    if (NULL == ip)
    {
        NBB_PRINTF("ip == 0\n");
        return;
    }

    for (i = 0; i < 4; i++)
    {
        NBB_PRINTF("%d,%d,%d,%d\n", ((*ip >> 24) & 0x00ff),
            ((*ip >> 16) & 0x00ff), ((*ip >> 8) & 0x00ff), (*ip & 0x00ff));

        ip++;
    }

}

#endif
#if 1

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_ipv4_dst_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_ipv4_dst_cfg(ATG_DCI_IP_NF_IPV4_DSTHOST *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_ipv4_dst_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("ipv4_dst_cfg:\n ");
    NBB_PRINTF("host1_ip_type=%d\n ", cfg->host1_ip_type);
    NBB_PRINTF("host1_enable_flag=%d\n ", cfg->host1_enable_flag);
    NBB_PRINTF("host1_udp_port=%d\n ", cfg->host1_udp_port);
    if (0 == cfg->host1_ip_type) /*IPV4*/
    {
        NBB_PRINTF("host1_ip=%d.%d.%d.%d\n ", ((cfg->host1_ip[3]) >> 24) & 0xff, ((cfg->host1_ip[3]) >> 16) & 0xff,
            ((cfg->host1_ip[3]) >> 8) & 0xff, (cfg->host1_ip[3]) & 0xff);
    }
    else if (1 == cfg->host1_ip_type) /*IPV6*/
    {
        NBB_PRINTF("host1_ip=%x.%x.%x.%x.%x.%x.%x.%x\n ", (short int)((cfg->host1_ip[0]) >> 16) & 0xffff,
            (short int)(cfg->host1_ip[0]) & 0xffff,
            (short int)((cfg->host1_ip[1]) >> 16) & 0xffff, (short int)(cfg->host1_ip[1]) & 0xffff,
            (short int)((cfg->host1_ip[2]) >> 16) & 0xffff, (short int)(cfg->host1_ip[2]) & 0xffff,
            (short int)((cfg->host1_ip[3]) >> 16) & 0xffff, (short int)(cfg->host1_ip[3]) & 0xffff);
    }
    NBB_PRINTF("host1_vrfid=%d\n ", cfg->host1_vrfid);
    
    NBB_PRINTF("host2_ip_type=%d\n ", cfg->host2_ip_type);
    NBB_PRINTF("host2_enable_flag=%d\n ", cfg->host2_enable_flag);
    NBB_PRINTF("host2_udp_port=%d\n ", cfg->host2_udp_port);
    if (0 == cfg->host2_ip_type) /*IPV4*/
    {
        NBB_PRINTF("host2_ip=%d.%d.%d.%d\n ", ((cfg->host2_ip[3]) >> 24) & 0xff, ((cfg->host2_ip[3]) >> 16) & 0xff,
            ((cfg->host2_ip[3]) >> 8) & 0xff, (cfg->host2_ip[3]) & 0xff);
    }
    else if (1 == cfg->host2_ip_type) /*IPV6*/
    {
        NBB_PRINTF("host2_ip=%x.%x.%x.%x.%x.%x.%x.%x\n ", (short int)((cfg->host2_ip[0]) >> 16) & 0xffff,
            (short int)(cfg->host2_ip[0]) & 0xffff,
            (short int)((cfg->host2_ip[1]) >> 16) & 0xffff, (short int)(cfg->host2_ip[1]) & 0xffff,
            (short int)((cfg->host2_ip[2]) >> 16) & 0xffff, (short int)(cfg->host2_ip[2]) & 0xffff,
            (short int)((cfg->host2_ip[3]) >> 16) & 0xffff, (short int)(cfg->host2_ip[3]) & 0xffff);
    }
    NBB_PRINTF("host2_vrfid=%d\n ", cfg->host2_vrfid);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_ipv4_src_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_ipv4_src_cfg(ATG_DCI_IP_NF_IPV4_SRCIP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_ipv4_src_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("ipv4_src_cfg:\n ");
    NBB_PRINTF("src_ipv4_enable=%d\n ", cfg->src_ipv4_enable);
    NBB_PRINTF("src_ipv6_enable=%d\n ", cfg->src_ipv6_enable);
    
    NBB_PRINTF("ipv4_addr=%d.%d.%d.%d\n ", ((cfg->ipv4_addr) >> 24) & 0xff, ((cfg->ipv4_addr) >> 16) & 0xff,
        ((cfg->ipv4_addr) >> 8) & 0xff, (cfg->ipv4_addr) & 0xff);
   
    NBB_PRINTF("ipv6_addr=%x.%x.%x.%x.%x.%x.%x.%x\n ", (short int)((cfg->ipv6_addr[0]) >> 16) & 0xffff,
        (short int)(cfg->ipv6_addr[0]) & 0xffff,
        (short int)((cfg->ipv6_addr[1]) >> 16) & 0xffff, (short int)(cfg->ipv6_addr[1]) & 0xffff,
        (short int)((cfg->ipv6_addr[2]) >> 16) & 0xffff, (short int)(cfg->ipv6_addr[2]) & 0xffff,
        (short int)((cfg->ipv6_addr[3]) >> 16) & 0xffff, (short int)(cfg->ipv6_addr[3]) & 0xffff);
   

    /*
       NBB_PRINTF("src2_ip_type=%d\n ", cfg->src2_ip_type);
       NBB_PRINTF("src2_enable_flag=%d\n ", cfg->src2_enable_flag);
       if (0 == cfg->src2_ip_type)
       {
        NBB_PRINTF("src2_ip=%d.%d.%d.%d\n ", ((cfg->src2_ip[3]) >> 24) & 0xff, ((cfg->src2_ip[3]) >> 16) & 0xff,
            ((cfg->src2_ip[3]) >> 8) & 0xff, (cfg->src2_ip[3]) & 0xff);
       }
       else if (1 == cfg->src2_ip_type)
       {
        NBB_PRINTF("host2_ip=%x.%x.%x.%x.%x.%x.%x.%x\n ", (short int)((cfg->src2_ip[0]) >> 16) & 0xffff,
            (short int)(cfg->src2_ip[0]) & 0xffff,
            (short int)((cfg->src2_ip[1]) >> 16) & 0xffff, (short int)(cfg->src2_ip[1]) & 0xffff,
            (short int)((cfg->src2_ip[2]) >> 16) & 0xffff, (short int)(cfg->src2_ip[2]) & 0xffff,
            (short int)((cfg->src2_ip[3]) >> 16) & 0xffff, (short int)(cfg->src2_ip[3]) & 0xffff);
       }
     */
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_ipv4_version_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_ipv4_version_cfg(ATG_DCI_IP_NF_IPV4_VERSION *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_ipv4_version_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("ipv4_version_cfg:\n ");
    NBB_PRINTF("version=%d\n ", cfg->version);
    NBB_PRINTF("as_number=%d\n ", cfg->as_number);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_ipv6_dst_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_ipv6_dst_cfg(ATG_DCI_IP_NF_IPV6_DSTHOST *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_ipv6_dst_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("ipv6_dst_cfg:\n ");
    NBB_PRINTF("host1_ip_type=%d\n ", cfg->host1_ip_type);
    NBB_PRINTF("host1_enable_flag=%d\n ", cfg->host1_enable_flag);
    NBB_PRINTF("host1_udp_port=%d\n ", cfg->host1_udp_port);
    if (0 == cfg->host1_ip_type) /*IPV4*/
    {
        NBB_PRINTF("host1_ip=%d.%d.%d.%d\n ", ((cfg->host1_ip[3]) >> 24) & 0xff, ((cfg->host1_ip[3]) >> 16) & 0xff,
            ((cfg->host1_ip[3]) >> 8) & 0xff, (cfg->host1_ip[3]) & 0xff);
    }
    else if (1 == cfg->host1_ip_type) /*IPV6*/
    {
        NBB_PRINTF("host1_ip=%x.%x.%x.%x.%x.%x.%x.%x\n ", (short int)((cfg->host1_ip[0]) >> 16) & 0xffff,
            (short int)(cfg->host1_ip[0]) & 0xffff,
            (short int)((cfg->host1_ip[1]) >> 16) & 0xffff, (short int)(cfg->host1_ip[1]) & 0xffff,
            (short int)((cfg->host1_ip[2]) >> 16) & 0xffff, (short int)(cfg->host1_ip[2]) & 0xffff,
            (short int)((cfg->host1_ip[3]) >> 16) & 0xffff, (short int)(cfg->host1_ip[3]) & 0xffff);
    }
    NBB_PRINTF("host1_vrfid=%d\n ", cfg->host1_vrfid);
    
    NBB_PRINTF("host2_ip_type=%d\n ", cfg->host2_ip_type);
    NBB_PRINTF("host2_enable_flag=%d\n ", cfg->host2_enable_flag);
    NBB_PRINTF("host2_udp_port=%d\n ", cfg->host2_udp_port);
    if (0 == cfg->host2_ip_type) /*IPV4*/
    {
        NBB_PRINTF("host2_ip=%d.%d.%d.%d\n ", ((cfg->host2_ip[3]) >> 24) & 0xff, ((cfg->host2_ip[3]) >> 16) & 0xff,
            ((cfg->host2_ip[3]) >> 8) & 0xff, (cfg->host2_ip[3]) & 0xff);
    }
    else if (1 == cfg->host2_ip_type) /*IPV6*/
    {
        NBB_PRINTF("host2_ip=%x.%x.%x.%x.%x.%x.%x.%x\n ", (short int)((cfg->host2_ip[0]) >> 16) & 0xffff,
            (short int)(cfg->host2_ip[0]) & 0xffff,
            (short int)((cfg->host2_ip[1]) >> 16) & 0xffff, (short int)(cfg->host2_ip[1]) & 0xffff,
            (short int)((cfg->host2_ip[2]) >> 16) & 0xffff, (short int)(cfg->host2_ip[2]) & 0xffff,
            (short int)((cfg->host2_ip[3]) >> 16) & 0xffff, (short int)(cfg->host2_ip[3]) & 0xffff);
    }
    NBB_PRINTF("host2_vrfid=%d\n ", cfg->host2_vrfid);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_ipv6_src_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_ipv6_src_cfg(ATG_DCI_IP_NF_IPV6_SRCIP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_ipv6_src_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("ipv6_src_cfg:\n ");
    NBB_PRINTF("src_ipv4_enable=%d\n ", cfg->src_ipv4_enable);
    NBB_PRINTF("src_ipv6_enable=%d\n ", cfg->src_ipv6_enable);
    
    NBB_PRINTF("ipv4_addr=%d.%d.%d.%d\n ", ((cfg->ipv4_addr) >> 24) & 0xff, ((cfg->ipv4_addr) >> 16) & 0xff,
        ((cfg->ipv4_addr) >> 8) & 0xff, (cfg->ipv4_addr) & 0xff);
   
    NBB_PRINTF("ipv6_addr=%x.%x.%x.%x.%x.%x.%x.%x\n ", (short int)((cfg->ipv6_addr[0]) >> 16) & 0xffff,
        (short int)(cfg->ipv6_addr[0]) & 0xffff,
        (short int)((cfg->ipv6_addr[1]) >> 16) & 0xffff, (short int)(cfg->ipv6_addr[1]) & 0xffff,
        (short int)((cfg->ipv6_addr[2]) >> 16) & 0xffff, (short int)(cfg->ipv6_addr[2]) & 0xffff,
        (short int)((cfg->ipv6_addr[3]) >> 16) & 0xffff, (short int)(cfg->ipv6_addr[3]) & 0xffff);

    /*
       NBB_PRINTF("src2_ip_type=%d\n ", cfg->src2_ip_type);
       NBB_PRINTF("src2_enable_flag=%d\n ", cfg->src2_enable_flag);
       if (0 == cfg->src2_ip_type)
       {
        NBB_PRINTF("src2_ip=%d.%d.%d.%d\n ", ((cfg->src2_ip[3]) >> 24) & 0xff, ((cfg->src2_ip[3]) >> 16) & 0xff,
            ((cfg->src2_ip[3]) >> 8) & 0xff, (cfg->src2_ip[3]) & 0xff);
       }
       else if (1 == cfg->src2_ip_type)
       {
        NBB_PRINTF("host2_ip=%x.%x.%x.%x.%x.%x.%x.%x\n ", (short int)((cfg->src2_ip[0]) >> 16) & 0xffff,
            (short int)(cfg->src2_ip[0]) & 0xffff,
            (short int)((cfg->src2_ip[1]) >> 16) & 0xffff, (short int)(cfg->src2_ip[1]) & 0xffff,
            (short int)((cfg->src2_ip[2]) >> 16) & 0xffff, (short int)(cfg->src2_ip[2]) & 0xffff,
            (short int)((cfg->src2_ip[3]) >> 16) & 0xffff, (short int)(cfg->src2_ip[3]) & 0xffff);
       }
     */
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_ipv6_version_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_ipv6_version_cfg(ATG_DCI_IP_NF_IPV6_VERSION *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_ipv6_version_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("ipv6_version_cfg:\n ");
    NBB_PRINTF("version=%d\n ", cfg->version);
    NBB_PRINTF("as_number=%d\n ", cfg->as_number);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_tcp_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_tcp_cfg(ATG_DCI_IP_NF_CHECK_TCP *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_check_tcp_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("check_tcp_cfg:\n ");
    NBB_PRINTF("enable_flag=%d\n ", cfg->enable_flag);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_timeout_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_timeout_cfg(ATG_DCI_IP_NF_TIMEOUT *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_timeout_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("timeout_cfg:\n ");
    NBB_PRINTF("inactive_interval=%d\n ", cfg->inactive_interval);
    NBB_PRINTF("active_interval=%d\n ", cfg->active_interval);
    NBB_PRINTF("bytes_limit=%d\n ", cfg->bytes_limit);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_statrt_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_statrt_cfg(ATG_DCI_IP_NF_START *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_start_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("start_cfg:\n ");
    NBB_PRINTF("enable_flag=%d\n ", cfg->enable_flag);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_info_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_info_cfg(ATG_DCI_IP_NF_EXPORT_INFO *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_export_info_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("export_info_cfg:\n ");
    NBB_PRINTF("vlan_enable_flag=%d\n ", cfg->vlan_enable_flag);
    NBB_PRINTF("mpls_enable_flag=%d\n ", cfg->mpls_enable_flag);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_enable_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_enable_cfg(ATG_DCI_IP_NF_ENABLE *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    #ifdef R8000
    NBB_PRINTF("device type is r8000 !!! \n");
    #endif
    #ifdef PTN690
    NBB_PRINTF("device type is ptn690 !!! \n");
    #endif
    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_enable_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("enable_cfg:\n ");
    NBB_PRINTF("enable_flag=%d\n ", cfg->enable_flag);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_flexnfkey_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_ip_nf_flexnfkey_cfg(ATG_DCI_IP_NF_FLEXNFKEY *cfg)
{

    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    uint32_t key;
    
    if (NULL == cfg)
    {
        NBB_PRINTF("ip_nf_flexnfke_cfg == null\n");
        return;
    }

    NBB_PRINTF("key.src_addr=%d\n ", cfg->key.src_addr);
    NBB_PRINTF("key.dst_addr=%d\n ", cfg->key.dst_addr);
    NBB_PRINTF("key.src_mask=%d\n ", cfg->key.src_mask);
    NBB_PRINTF("key.dst_mask=%d\n ", cfg->key.dst_mask);
    NBB_PRINTF("key.protocol=%d\n ", cfg->key.protocol);
    NBB_PRINTF("key.tos=%d\n ", cfg->key.tos);
    NBB_PRINTF("key.src_port=%d\n ", cfg->key.src_port);
    NBB_PRINTF("key.dst_port=%d\n ", cfg->key.dst_port);
    NBB_PRINTF("key.input_index=%d\n ", cfg->key.input_index);
    NBB_PRINTF("key.output_index=%d\n ", cfg->key.output_index);
    NBB_PRINTF("key.src_as=%d\n ", cfg->key.src_as);
    NBB_PRINTF("key.dst_as=%d\n ", cfg->key.dst_as);
    NBB_PRINTF("key.src_pre=%d\n ", cfg->key.src_pre);
    NBB_PRINTF("key.dst_pre=%d\n ", cfg->key.dst_pre);
    NBB_PRINTF("key.ipv6_flow_label=%d\n ", cfg->key.ipv6_flow_label);
    NBB_PRINTF("key.flow_direction=%d\n ", cfg->key.flow_direction);
    NBB_PRINTF("key.next_hop=%d\n ", cfg->key.next_hop);
    NBB_PRINTF("key.pad=%x\n ", cfg->key.pad);
    key = (uint32_t)((cfg->key.src_addr ) + (cfg->key.dst_addr << 1)
            + (cfg->key.src_mask << 2) + (cfg->key.dst_mask << 3)
            + (cfg->key.protocol << 4) + (cfg->key.tos << 5)
            + (cfg->key.src_port << 6) + (cfg->key.dst_port << 7)
            + (cfg->key.input_index << 8) + (cfg->key.output_index << 9)
            + (cfg->key.src_as << 10) + (cfg->key.dst_as << 11)
            + (cfg->key.src_pre << 12) + (cfg->key.dst_pre << 13)
            + (cfg->key.ipv6_flow_label << 14) + (cfg->key.flow_direction << 15)
            + (cfg->key.next_hop << 16) );
    NBB_PRINTF("key=%08x\n ", key);
    
    //NBB_PRINTF("key.next_hop=%d\n ", cfg->key.next_hop);
    NBB_PRINTF("\n ");
    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_nf_aggr_aggregation_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_nf_aggr_aggregation_cfg(ATG_DCI_NF_AGGR_AGGREGATION *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("nf_aggr_aggregation_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("aggregation_cfg:\n ");
    NBB_PRINTF("aggr_mode=%d\n ", cfg->aggr_mode);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_nf_aggr_export_version_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_nf_aggr_export_version_cfg(ATG_DCI_NF_AGGR_EXPORT_VERSION *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("nf_aggr_export_version_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("export_version_cfg:\n ");
    NBB_PRINTF("version=%d\n ", cfg->version);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_statrt_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_nf_aggr_ipv4mask_cfg(ATG_DCI_NF_AGGR_IPV4_MASK *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("nf_aggr_ipv4mask_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("ipv4mask_cfg:\n ");
    NBB_PRINTF("src_mask_length=%d\n ", cfg->src_mask_length);
    NBB_PRINTF("dst_mask_length=%d\n ", cfg->dst_mask_length);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_ip_nf_statrt_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_nf_aggr_ipv6mask_cfg(ATG_DCI_NF_AGGR_IPV6_MASK *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("nf_aggr_ipv6mask_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("ipv6mask_cfg:\n ");
    NBB_PRINTF("src_mask_length=%d\n ", cfg->src_mask_length);
    NBB_PRINTF("dst_mask_length=%d\n ", cfg->dst_mask_length);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_print_nf_aggr_enable_cfg
   功能描述  : 显示配置
   输入参数  : 子ips消息指针
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期四
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_print_nf_aggr_enable_cfg(ATG_DCI_NF_AGGR_ENABLE *cfg)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    if (NULL == cfg)
    {
        NBB_PRINTF("nf_aggr_enable_cfg == null\n\n");
        return;
    }

    NBB_PRINTF("enable_cfg:\n ");
    NBB_PRINTF("enable_flag=%d\n ", cfg->enable_flag);
    NBB_PRINTF("\n ");

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_show_ip_nf_cfg
   功能描述  : 显示配置
   输入参数  : ip_nf index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_show_ip_nf_cfg(NBB_ULONG index, NBB_BYTE type)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_IP_NF_CB *pcb = NULL;

	NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    if (0 == index) /*非法*/
    {
        NBB_PRINTF("**ERROR** ip_nf index = 0 ");
        goto EXIT_LABEL;
    }

    pcb = AVLL_FIND(v_spm_shared->ip_nf_tree, &index);

    /*查不到acl 配置*/
    if (NULL == pcb)
    {
        NBB_PRINTF("can't find ip nf cfg\n");
        goto EXIT_LABEL;
    }

    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV4_DST == type))
    {
        spm_print_ip_nf_ipv4_dst_cfg(pcb->ipv4_dst_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV4_SRC == type))
    {
        spm_print_ip_nf_ipv4_src_cfg(pcb->ipv4_src_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV4_VERSION == type))
    {
        spm_print_ip_nf_ipv4_version_cfg(pcb->ipv4_version_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV6_DST == type))
    {
        spm_print_ip_nf_ipv6_dst_cfg(pcb->ipv6_dst_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV6_SRC == type))
    {
        spm_print_ip_nf_ipv6_src_cfg(pcb->ipv6_src_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_IPV6_VERSION == type))
    {
        spm_print_ip_nf_ipv6_version_cfg(pcb->ipv6_version_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_CHECK_TCP == type))
    {
        spm_print_ip_nf_tcp_cfg(pcb->tcp_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_TIMEOUT == type))
    {
        spm_print_ip_nf_timeout_cfg(pcb->timeout_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_START == type))
    {
        spm_print_ip_nf_statrt_cfg(pcb->start_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_EXPORT_INFO == type))
    {
        spm_print_ip_nf_info_cfg(pcb->info_cfg_cb);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_ENABLE == type))
    {
        spm_print_ip_nf_enable_cfg(pcb->enable_cfg);
    }
    if ((SPM_NF_ALL == type) || (SPM_IP_NF_FLEXNFKEY == type))
    {
        spm_print_ip_nf_flexnfkey_cfg(pcb->flexnfkey_cfg);
    }

    NBB_PRINTF("ip nf cnt = %ld\n", pcb->cnt);

    EXIT_LABEL:

    return;
}

/*****************************************************************************
   函 数 名  : spm_show_nf_aggr_cfg
   功能描述  : 显示配置
   输入参数  : nf_aggr index
   输出参数  :
   返 回 值  :
   调用函数  :
   被调函数  :

   修改历史      :
   1.日    期   : 2013年5月9日 星期二
    作    者   : fansongbo
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_show_nf_aggr_cfg(NBB_ULONG index, NBB_BYTE type)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_NF_AGGR_CB *pcb = NULL;

    if (0 == index) /*非法*/
    {
        NBB_PRINTF("**ERROR** nf_aggr index = 0 ");
        goto EXIT_LABEL;
    }

	NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    pcb = AVLL_FIND(v_spm_shared->nf_aggr_tree, &index);

    /*查不到acl 配置*/
    if (NULL == pcb)
    {
        NBB_PRINTF("can't find nf aggr cfg\n");
        goto EXIT_LABEL;
    }

    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_MODE == type))
    {
        spm_print_nf_aggr_aggregation_cfg(pcb->aggregation_cfg);
    }
    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_EXPORT_VERSION == type))
    {
        spm_print_nf_aggr_export_version_cfg(pcb->export_version_cfg);
    }
    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_IPV4_MASK == type))
    {
        spm_print_nf_aggr_ipv4mask_cfg(pcb->ipv4mask_cfg);
    }
    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_IPV6_MASK == type))
    {
        spm_print_nf_aggr_ipv6mask_cfg(pcb->ipv6mask_cfg);
    }
    if ((SPM_NF_ALL == type) || (SPM_NF_AGGR_ENABLE == type))
    {
        spm_print_nf_aggr_enable_cfg(pcb->enable_cfg);
    }

    NBB_PRINTF("nf aggr cnt = %ld\n", pcb->cnt);

    EXIT_LABEL:

    return;
}

NBB_VOID show_all_ip_nf_cfg()
{
   SPM_IP_NF_CB *pcb = NULL;
   NBB_BYTE type = 0;

	NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    for (pcb = (SPM_IP_NF_CB *) AVLL_FIRST(v_spm_shared->ip_nf_tree);
         pcb != NULL;
         pcb = (SPM_IP_NF_CB *) AVLL_NEXT(v_spm_shared->ip_nf_tree,pcb->ip_nf_node))
    {
        OS_PRINTF("*****IP_NF (ID = %d)*****\n",pcb->ip_nf_key);
        spm_show_ip_nf_cfg(pcb->ip_nf_key,type);
        
    }

    return;
}

NBB_VOID show_ip_nf_cfg_num()
{
    SPM_IP_NF_CB *pcb = NULL;
    NBB_BYTE ucNum = 0;

	NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    for (pcb = (SPM_IP_NF_CB *) AVLL_FIRST(v_spm_shared->ip_nf_tree);
         pcb != NULL;
         pcb = (SPM_IP_NF_CB *) AVLL_NEXT(v_spm_shared->ip_nf_tree,pcb->ip_nf_node))
    {
        ucNum++;
    }

    OS_PRINTF("IP_NF num = %d\n",ucNum);

    return;
}


NBB_VOID show_all_nf_aggr_cfg()
{
   SPM_NF_AGGR_CB *pcb = NULL;
   NBB_BYTE type = 0;

	NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    for (pcb = (SPM_NF_AGGR_CB *) AVLL_FIRST(v_spm_shared->nf_aggr_tree);
         pcb != NULL;
         pcb = (SPM_NF_AGGR_CB *) AVLL_NEXT(v_spm_shared->nf_aggr_tree,pcb->nf_aggr_node))
    {
        OS_PRINTF("*****NF AGGR (ID = %d)*****\n",pcb->nf_aggr_key);
        spm_show_nf_aggr_cfg(pcb->nf_aggr_key,type);
        
    }

    return;
}

NBB_VOID show_nf_aggr_cfg_num()
{
   SPM_NF_AGGR_CB *pcb = NULL;
   NBB_BYTE ucNum = 0;

	NBB_NULL_THREAD_CONTEXT
    NBB_GET_THREAD_CONTEXT();
    
    for (pcb = (SPM_NF_AGGR_CB *) AVLL_FIRST(v_spm_shared->nf_aggr_tree);
         pcb != NULL;
         pcb = (SPM_NF_AGGR_CB *) AVLL_NEXT(v_spm_shared->nf_aggr_tree,pcb->nf_aggr_node))
    {
        ucNum++;
    }
         
    OS_PRINTF("NF AGGR num = %d\n",ucNum);

    return;
}




NBB_VOID spm_dbg_print_nf_ctrl_set(unsigned char set)
{
    nf_ctrl_cfg_print_setting = set;
}

NBB_VOID spm_dbg_print_nf_ctrl_basic_cfg(ATG_DCI_NF_CTRL_RESET_NF *basic_cfg_cb)
{
    if (basic_cfg_cb != NULL)
    {
        OS_PRINTF("    enable flag = %d\n", basic_cfg_cb->enable_flag);
    }
}

NBB_VOID nfhelp(NBB_VOID)
{
    NBB_CHAR **ppcMsg;

    static NBB_CHAR *pHelpMsg[] =
    {
        "----------------------------------------------------------------------------------", "",
        "spm_show_ip_nf_cfg(index, type)", "type=0-10(all/ipv4dst/../ipv6dst/../tcp/timeout/start/info)",
        "spm_show_nf_aggr_cfg(index, type)", "type=0-4(all/basic/port/eth/ipv4/tcp/ipv6/noip)",
        "show_all_ip_nf_cfg()",              "show all ip_nf cfg",
        "show_all_nf_aggr_cfg()",            "show all nf_aggr_cfg",
        "show_ip_nf_cfg_num()",                "show ip_nf cfg_num",
        "show_nf_aggr_cfg_num()",              "show nf_aggr cfg num",
        
        
        "----------------------------------------------------------------", "",

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

