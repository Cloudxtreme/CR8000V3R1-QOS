#include <nbase.h>
#include <spmincl.h>
#include <c3api.h>
#if 0
#ifndef SNU

#ifdef SRC
NBB_VOID spm_set_efm_oam(EFM_PHY_PORT *p_efm_phy_port, ATG_DCI_PHY_PORT_EFM_OAM_DATA *p_efm_oam)
{
    return;
}
NBB_VOID InitAllPortOamSelfCfg()
{
    return;
}
#endif

#ifdef SPU

/*****************************************************************************
 函 数 名  : spm_rcv_dci_set_efm
 功能描述  : 处理配置模块
 输入参数  : NBB_BYTE *p
 输出参数  : 无
 返 回 值  : NBB_VOID
 调用函数  :
 被调函数  :

 修改历史      :
  1.日    期   : 2013年8月1日
    作    者   : huxi
    修改内容   : 新生成函数

*****************************************************************************/
NBB_VOID spm_set_efm_oam(EFM_PHY_PORT *p_efm_phy_port, ATG_DCI_PHY_PORT_EFM_OAM_DATA *p_efm_oam)
{
    NBB_INT iRet = ERROR;
    NBB_BYTE local_slot;
    NBB_BYTE port_temp;
    NBB_BYTE sub_card;
    /* 获取本盘槽位 */
    iRet = spm_hardw_getslot(&local_slot);

    if (SUCCESS != iRet)
    {
        NBB_PRINTF(("Get slot info failed."));
        return;
    }

    if (local_slot != p_efm_phy_port->slots)
    {
        return;
    }

    sub_card = p_efm_phy_port->cards;

    //如果无子卡
    if (0 == sub_card)
    {
        g_efm_oam_port_cfg[sub_card].sub_cards = sub_card;
        port_temp = p_efm_phy_port->ports - 33;
        g_efm_oam_port_cfg[sub_card].logic_port[port_temp] = p_efm_phy_port->ports;
    }
    else
    {
        if (1 == sub_card) //子卡1
        {
            g_efm_oam_port_cfg[sub_card].sub_cards = sub_card;
            port_temp = p_efm_phy_port->ports - 33; //子卡1的逻辑端口号范围为32+port
            g_efm_oam_port_cfg[sub_card].logic_port[port_temp] = p_efm_phy_port->ports;
        }
        else if (2 == sub_card)
        {
            g_efm_oam_port_cfg[sub_card].sub_cards = sub_card;
            port_temp = p_efm_phy_port->ports - 65;//子卡2的逻辑端口号范围为64+port
            g_efm_oam_port_cfg[sub_card].logic_port[port_temp] = p_efm_phy_port->ports;
        }
    }

    /* 入参判断 */
    if ((NULL == p_efm_oam) || (0 == p_efm_oam->byte1.oam_enable))
    {
        //删除配置存储
        g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg[port_temp] = 0;
        g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg_last[port_temp] = 0;
        g_efm_oam_port_cfg[sub_card].port_oam_mode_cfg[port_temp] = 0;
        g_efm_oam_port_cfg[sub_card].port_oam_maxsize_cfg[port_temp] = oam_max_size;
        g_efm_oam_port_cfg[sub_card].port_oam_overtime_cfg[port_temp] = lb_overtime;
        g_efm_oam_port_cfg[sub_card].port_oam_sym_period_cfg[port_temp] = 1;
        g_efm_oam_port_cfg[sub_card].port_oam_frm_period_cfg[port_temp] = 1;
        g_efm_oam_port_cfg[sub_card].port_oam_frm_prd_period_cfg[port_temp] = 1;
        g_efm_oam_port_cfg[sub_card].port_oam_frm_snd_period_cfg[port_temp] = 60;
        OS_MEMSET(&(g_efm_oam_port_cfg[sub_card].port_oam_x[port_temp]), 0, sizeof(port_oam));
        NBB_PRINTF("spm_set_efm_oam cfg==null\n");
        return;
    }

    //存储配置
    g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg[port_temp] = p_efm_oam->byte1.oam_enable;
    g_efm_oam_port_cfg[sub_card].port_oam_mode_cfg[port_temp] = p_efm_oam->byte1.oam_mode;
    g_efm_oam_port_cfg[sub_card].port_oam_sup_event_cfg[port_temp] = p_efm_oam->byte1.support_error_notify;
    g_efm_oam_port_cfg[sub_card].port_oam_sup_lb_cfg[port_temp] = p_efm_oam->byte1.support_loop;
    g_efm_oam_port_cfg[sub_card].port_oam_sup_var_cfg[port_temp] = p_efm_oam->byte1.support_mib_search;
    g_efm_oam_port_cfg[sub_card].port_oam_sup_uni_cfg[port_temp] = p_efm_oam->byte1.support_unidirectional_oam;
    g_efm_oam_port_cfg[sub_card].port_oam_overtime_cfg[port_temp] = p_efm_oam->reply_overtime_limit;
    g_efm_oam_port_cfg[sub_card].port_oam_maxsize_cfg[port_temp] = p_efm_oam->max_frame_length;
    g_efm_oam_port_cfg[sub_card].port_oam_sym_period_cfg[port_temp] = p_efm_oam->error_symbol_cycle;
    g_efm_oam_port_cfg[sub_card].port_oam_sym_threshold_cfg[port_temp] = p_efm_oam->error_symbol_limit;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_period_cfg[port_temp] = p_efm_oam->error_frame_cycle;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_threshold_cfg[port_temp] = p_efm_oam->error_frame_limit;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_prd_period_cfg[port_temp] = p_efm_oam->error_frame_cycle_cycle;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_prd_threshold_cfg[port_temp] = p_efm_oam->error_frame_cycle_limit;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_snd_period_cfg[port_temp] = p_efm_oam->error_frame_second_cycle;
    g_efm_oam_port_cfg[sub_card].port_oam_frm_snd_threshold_cfg[port_temp] = p_efm_oam->error_frame_second_limit;
    CfgPortOamEnb(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg[port_temp]); //配置端口OAM使能
    CfgPortOamSupportLb(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_sup_lb_cfg[port_temp]); //是否支持环回
    CfgPortOamSupportUnidirection(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_sup_uni_cfg[port_temp]); //是否支持单向
    CfgPortOamSupportVar(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_enb_cfg[port_temp]); //是否支持变量查询
    CfgPortOamSupportEvent(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_sup_var_cfg[port_temp]); //是否支持事件帧收发
    CfgPortOamMode(sub_card, port_temp, g_efm_oam_port_cfg[sub_card].port_oam_mode_cfg[port_temp]); //配置OAM模式
    AllPortOamCfgEnb();/* 配置哪些端口开启OAM，配置是否使能总体OAM收发驱动*/
}
/*************************** 总体说明 20080904  刘怀明
**************************************************
1.用户的所有接口函数简单，且都在本文件，其他文件不需关注！
2.用户应看清本文件中所有注释！用户也只需关注本文件！
3.对OAM的配置应在交换机配置后再做！
4.为了简洁，本程序暂不考虑单盘有多个交换芯片的问题！以后要扩展容易。
5.本程序目前是针对BROADCOM交换芯片，对于以后的ZARLINK芯片，则需  #define
ZARLINK
6.用户应注意输入端口是物理端口，非逻辑端口，应考虑端口映射变换！

*****************************************************************************/


/*****************************************************************************
功能： 对所有LAN端口OAM初始化为不使能OAM，并对全局变量赋初值；应在上电
SELFCONFIG 中初始化时调用
作者： hmliu
时间： 20080904
输入： 无
输出： 无
说明： 用户应自己对所有LAN端口初始化配置PVID流，以备可能用于OAM的信道,
此操作应在配置交换机时完成
修改：
******************************************************************************/
NBB_VOID InitAllPortOamSelfCfg()
{
    unsigned char i, j;
    dst_mac_overall[0] = 0x01;
    dst_mac_overall[1] = 0x80;
    dst_mac_overall[2] = 0xc2;
    dst_mac_overall[3] = 0x00;
    dst_mac_overall[4] = 0x00;
    dst_mac_overall[5] = 0x02;
    len_type_overall[0] = 0x88;
    len_type_overall[1] = 0x09;
    subtype_overall = 0x03;
    wri_fh_oui[0] = 0x00;
    wri_fh_oui[1] = 0x0a;
    wri_fh_oui[2] = 0xc2;
    board_name[0] = 'C';
    board_name[1] = 'R';
    board_name[2] = '8';
    board_name[3] = 'K';
    minFrameSize = 60;
    oam_max_size = 1496;
    lb_overtime = 10;
    lb_overtime = 5; //110513
    Period_100ms = 10;

    for (j = 0 ; j < SUB_NUM; j++)
    {
        for (i = 0; i < MAX_PORT_NUM; i++)
        {
            g_efm_oam_port_cfg[j].port_oam_enb_cfg[i] = 0;
            g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[i] = 0;
            g_efm_oam_port_cfg[j].port_oam_mode_cfg[i] = 0;
            g_efm_oam_port_cfg[j].port_oam_maxsize_cfg[i] = oam_max_size;
            g_efm_oam_port_cfg[j].port_oam_overtime_cfg[i] = lb_overtime;
            g_efm_oam_port_cfg[j].port_oam_sym_period_cfg[i] = 1;
            g_efm_oam_port_cfg[j].port_oam_frm_period_cfg[i] = 1;
            g_efm_oam_port_cfg[j].port_oam_frm_prd_period_cfg[i] = 1;
            g_efm_oam_port_cfg[j].port_oam_frm_snd_period_cfg[i] = 60;
            OS_MEMSET(&(g_efm_oam_port_cfg[j].port_oam_x[i]), 0, sizeof(port_oam));
        }
    }
}

/**************************************************
** 函数名称：RxOamPortResolveHdr
** 功能描述：解析EFM OAM PDU共同的报文头
** 输    入：port:收EFM OAM PDU端口
             p:接收的报文数据指针
** 输    出：无
** 作    者：yy09
** 日    期：2012.06.12
***************************************************/
NBB_BYTE RxOamPortResolveHdr(NBB_BYTE sub_cards, NBB_BYTE port, oam_header *p)
{
    port_oam_pdu *p_rx; /*为可读性好临时用*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (FALSE != NBB_MEMCMP(p->dst_mac, dst_mac_overall, 6))    /*目的地址检查 EFM OAM所用目的MAC为 01-80-c2-00-00-02*/
    {
        NBB_PRINTF("dst_mac_overall FAIL\n");
        goto RxErrorExit;
    }

    if (FALSE != NBB_MEMCMP(p->len_type, len_type_overall, 2))    /*协议类型检查 慢协议共同协议类型 0x8809*/
    {
        NBB_PRINTF("len_typeFAIL\n");
        goto RxErrorExit;
    }

    if ((p->subtype) != subtype_overall)      /*SUBTYPE检查 EFM OAM子协议类型为 0X03*/
    {
        NBB_PRINTF("subtype_overall FAIL\n");
        goto RxErrorExit;
    }

    NBB_MEMCPY(p_ctrl->src_mac_remote, p->src_mac, 6);   /*SA*/
    NBB_MEMCPY(p_rx->oam_pdu_x.oam_header_x.src_mac, p->src_mac, 6);   /*SA*/
    NBB_MEMCPY(&(p_rx->oam_pdu_x.oam_header_x.flags_reserved), &(p->flags_reserved), 2);       /*flag*/
    /*将对端LINK状态获取，其他信息直接存在结构中*/
    p_ctrl->remote_link_status_last = p_rx->oam_pdu_x.oam_header_x.link_fault \
                                      | p_rx->oam_pdu_x.oam_header_x.dying_gasp
                                      | p_rx->oam_pdu_x.oam_header_x.critical_event;
    /*将收区对端发来的本端信息存入发区的远端信息*/
    p_tx->oam_pdu_x.oam_header_x.remote_stable = p_rx->oam_pdu_x.oam_header_x.local_stable;
    p_tx->oam_pdu_x.oam_header_x.remote_discovering = p_rx->oam_pdu_x.oam_header_x.local_discovering;

    if (p_ctrl->src_mac_remote_copied == 0)  /*存下对端的源MAC*/
    {
        NBB_MEMCPY(p_ctrl->src_mac_remote, p_rx->oam_pdu_x.oam_header_x.src_mac, 6);
        p_ctrl->src_mac_remote_copied = 1;

        if (p_ctrl->discovery_status == DISCOVER_ANY)  /* 如果协商已完成，则下面信息不处理*/
        {
            return(0);
        }
    }

    /*  本来还需分析对端的对端信息即本端信息，暂不用，不处理。  */
    return(0);   /*头检验通过的包还不能执行好包累计*/
RxErrorExit:
    rx_tx_oam.rx_cnt_err++; /* 不会改变是否完成以及完成状态，所以此不提*/
    p_ctrl->oam_pkt_rx_err++;
    return(1);
}

/**************************************************
** 函数名称：RxOamPortResolveInf
** 功能描述：解析收到的Information 帧内容
** 输    入：port: 收报文的端口
             p:收包数据指针
             size:收包数据长度
** 输    出：无
** 作    者：yy09
** 日    期：2012.06.12
***************************************************/
NBB_VOID RxOamPortResolveInf(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p, NBB_INT size)
{
    oam_control_status *p_ctrl; /* 为可读性好临时用*/
    port_oam_pdu *p_tx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (*p == INF_TLV_TYPE_END)  /* 收到无TLV的包，*/
    {
        p_ctrl->rx_pkt_type = RX_INF_NO_TLV;
        goto RxValidExit;
    }

    /*解析Information TLV*/
    if (*p == INF_TLV_TYPE_LOCAL)  /*Local Information TLV*/
    {
        if (*(p + 1) == INF_TLV_LEN)    /*TLV length*/
        {
            if (*(p + INF_TLV_LEN) == INF_TLV_TYPE_END)    /*End of TLV marker*/
            {
                p_ctrl->rx_pkt_type = RX_INF_LOCAL;
                goto RxValidExit;
            }

            if (*(p + INF_TLV_LEN) == INF_TLV_TYPE_WITH_REMOTE)    /*Remote Information TLV*/
            {
                if (*(p + INF_TLV_LEN + 1) == INF_TLV_LEN)    /*暂规定在发送带对端信息时必须带上ORG_INF*/
                {
                    if (*(p + INF_TLV_LEN + INF_TLV_LEN) == INF_TLV_TYPE_ORG)
                    {
                        if (*(p + INF_TLV_LEN + INF_TLV_LEN + 1) == INF_ORG_TLV_LEN)
                        {
                            p_ctrl->rx_pkt_type = RX_INF_LOCAL_REMOTE; /*帧格式完全固定，和EVENT变长不同*/
                            goto RxValidExit;
                        }
                    }
                }

                if (*(p + INF_TLV_LEN + 1) == INF_TLV_TYPE_END)    /*兼容标准即对端不带ORG信息*/
                {
                    goto RxValidExit;
                }
            }
        }
    }

    p_ctrl->rx_pkt_type = RX_INF_LOCAL_REMOTE;
    //goto RxValidExit; /*100609*/
    goto RxErrorExit;
RxValidExit:
    p_ctrl->oam_pkt_rx++; /*不执行unfinish操作*/
    p_ctrl->rx_pkt_cnt_in_5s++;
    rx_tx_oam.rx_cnt_total++;
    p_tx->org_pdu_tlv_x.rx_inf++;
    AnalyzeRxInf(sub_cards, port, p_ctrl->rx_pkt_type, p);  /*收到好包，再处理*/
    return;
RxErrorExit:
    NBB_PRINTF("RxOamPortResolveInf:RxErrorExit....\n");
    p_ctrl->rx_pkt_type = NON_PDU_RX; /*收到无效包，等效于没收到包*/
    p_ctrl->oam_pkt_rx_err++;
    rx_tx_oam.rx_cnt_err++;
    return;
}

/**************************************************
** 函数名称：RxOamPortResolveEvent
** 功能描述：解析收到的Event帧内容
** 输    入：port: 收报文的端口
             p1:收包数据指针
             size:收包数据长度
** 输    出：无
** 作    者：yy09
** 日    期：2012.06.12
***************************************************/
NBB_VOID RxOamPortResolveEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size)
{
    oam_control_status *p_ctrl; /* 为可读性好临时用*/
    NBB_BYTE *p;
    NBB_BYTE valid_event;
    port_oam_pdu *p_tx;
    unsigned short event_sq_tmp;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p = p1;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    event_sq_tmp = (*p << 8) + *(p + 1);

    /* 发送SQ从1开始,但考虑计数翻转 SQ为0，也合法*/
    if (event_sq_tmp == p_ctrl->event_rx_sq)
    {
        /* 重复事件通告，则只计数，不予处理 */
        goto RxValidExit;
    }

    p += 2;

    if (*p == EVENT_TLV_TYPE_END)  /* 实际上没有任何事件发生,但也算合格包计数*/
    {
        goto RxValidExit;
    }

    valid_event = 0;

    while (*p)  /* 是否为0即标识数据结束*/
    {
        switch (*p)
        {
            case EVENT_TLV_TYPE_SYM_PERIOD:
                if (*(p + 1) == EVENT_SYM_TLV_LEN)    /* 事件长度合法*/
                {
                    p_ctrl->rx_sym_err = 1;
                    p_ctrl->rx_sym_err_cnt = 1;
                    AnalyzeRxSymErrEvent(sub_cards, port, p);  /* 处理收到的事件*/
                    p = p + EVENT_SYM_TLV_LEN; /* 将指针指向下一块事件数据*/
                    valid_event++;
                }
                else /* 事件长度不合法*/
                {
                    if (valid_event > 0) /* 本次事件长度不合法，不予处理,但整体有合格事件，所以计数*/
                    {
                        goto RxValidExit;
                    }
                    else /* 没有一个合格事件，是错包*/
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            case EVENT_TLV_TYPE_FRM:
                if (*(p + 1) == EVENT_FRM_TLV_LEN)
                {
                    p_ctrl->rx_frm_err = 1;
                    AnalyzeRxFrmErrEvent(sub_cards, port, p);
                    p = p + EVENT_FRM_TLV_LEN;
                    valid_event++;
                }
                else
                {
                    if (valid_event > 0)
                    {
                        goto RxValidExit;
                    }
                    else
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            case EVENT_TLV_TYPE_FRM_PERIOD:
                if (*(p + 1) == EVENT_FRM_PRD_TLV_LEN)
                {
                    p_ctrl->rx_frm_prd_err = 1;
                    AnalyzeRxFrmPeriodEvent(sub_cards, port, p);
                    p = p + EVENT_FRM_PRD_TLV_LEN;
                    valid_event++;
                }
                else
                {
                    if (valid_event > 0)
                    {
                        goto RxValidExit;
                    }
                    else
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            case EVENT_TLV_TYPE_SECOND_SUM:
                if (*(p + 1) == EVENT_FRM_SECOND_TLV_LEN)
                {
                    p_ctrl->rx_frm_snd_err = 1;
                    AnalyzeRxFrmSecondEvent(sub_cards, port, p);
                    p = p + EVENT_FRM_SECOND_TLV_LEN;
                    valid_event++;
                }
                else
                {
                    if (valid_event > 0)
                    {
                        goto RxValidExit;
                    }
                    else
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            case EVENT_TLV_TYPE_ORG: /* ORG事件收判断时并不认为没ORG事件即不合格，这与ORG_INFO不同*/
                if (*(p + 1) == EVENT_ORG_TLV_LEN)
                {
                    AnalyzeRxOrgEvent(sub_cards, port, p);
                    p = p + EVENT_ORG_TLV_LEN;
                    valid_event++;
                }
                else
                {
                    if (valid_event > 0)
                    {
                        goto RxValidExit;
                    }
                    else
                    {
                        goto RxErrorExit;
                    }
                }

                break;

            default: /* 事件类型错*/
                if (valid_event > 0)
                {
                    goto RxValidExit;
                }
                else
                {
                    goto RxErrorExit;
                }
        }
    }

RxValidExit:
    p_ctrl->rx_pkt_type = RX_EVENT; /* 不执行 unfinish 操作*/
    p_ctrl->oam_pkt_rx++; /* 端口总数*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5秒内计数以判断是否超时没收包*/
    rx_tx_oam.rx_cnt_total++; /* 所有端口的总数*/
    p_tx->org_pdu_tlv_x.rx_event++;
    return;
RxErrorExit:
    p_ctrl->rx_pkt_type = NON_PDU_RX;         /* 收到无效包，等效于没收到包*/
    p_ctrl->oam_pkt_rx_err++;
    rx_tx_oam.rx_cnt_err++;
    return;
}

NBB_VOID RxOamPortResolveVarReq(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size) /*
                                                                                SIZE不起作用，通过判断BRANCH=0标识参数结束*/
{
    oam_control_status *p_ctrl; /* 为可读性好临时用     */
    NBB_BYTE valid_var_req;
    port_oam_pdu *p_tx;
    port_oam_pdu *p_rx;
    NBB_BYTE i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    valid_var_req = 0;
    NBB_MEMCPY(&(p_rx->var_req_x[0]), p1, 5 * sizeof(var_req));

    for (i = 0; i < 5; i++)
    {
        if (p_rx->var_req_x[i].branch > 0)
        {
            NBB_MEMCPY(&(p_tx->var_res_x[i]), &(p_rx->var_req_x[i]), sizeof(var_req));         /*为发送应答作准备*/
            valid_var_req++;
        }
        else
        {
            break;
        }
    }

    p_ctrl->unfinished = UNF_VAR_RES;
    p_rx->var_num = valid_var_req;
    p_tx->var_num = valid_var_req;
    ReactRxVarReq(sub_cards, port);  /* 填好应答数据，为发作准备*/
    p_ctrl->rx_pkt_type = RX_REQ;
    p_ctrl->oam_pkt_rx++; /* 端口总数*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5秒内计数以判断是否超时没收包*/
    rx_tx_oam.rx_cnt_total++; /* 所有端口的总数*/
    p_tx->org_pdu_tlv_x.rx_var_req++; /* 即本端计数，用于发送到对端的*/
}

NBB_VOID RxOamPortResolveVarRes(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size)  /* 同查询处理*/
{
    oam_control_status *p_ctrl; /* 为可读性好临时用     */
    NBB_BYTE valid_var_req;
    port_oam_pdu *p_tx;
    port_oam_pdu *p_rx;
    NBB_BYTE i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    valid_var_req = 0;
    NBB_MEMCPY(&(p_rx->var_res_x[0]), p1, 5 * sizeof(var_res));

    for (i = 0; i < 5; i++)
    {
        if (p_rx->var_res_x[i].branch == VALID_BRANCH)
        {
            switch (p_rx->var_res_x[i].leaf)  /* 2/5/6/8/0xe*/
            {
                case 2:
                    p_ctrl->remote_var_cnt[TX_FRM] = p_rx->var_res_x[i].value;
                    valid_var_req++;
                    break;

                case 5:
                    p_ctrl->remote_var_cnt[RX_FRM] = p_rx->var_res_x[i].value;
                    valid_var_req++;
                    break;

                case 6:
                    p_ctrl->remote_var_cnt[RX_CRC] = p_rx->var_res_x[i].value;
                    valid_var_req++;

                case 8:
                    p_ctrl->remote_var_cnt[TX_BYTE] = p_rx->var_res_x[i].value;
                    valid_var_req++;
                    break;

                case 0xe:
                    p_ctrl->remote_var_cnt[RX_BYTE] = p_rx->var_res_x[i].value;
                    valid_var_req++;
                    break;

                default:
                    break;
            }
        }
    }

    p_rx->var_num = valid_var_req; /* 无未完成项即无等待应答需求*/
    p_ctrl->rx_pkt_type = RX_RES;
    p_ctrl->oam_pkt_rx++; /* 端口总数*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5秒内计数以判断是否超时没收包*/
    rx_tx_oam.rx_cnt_total++; /* 所有端口的总数*/
    p_tx->org_pdu_tlv_x.rx_var_res++;
}

NBB_VOID RxOamPortResolveLb(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size)  /*定长*/
{
    port_oam_pdu *p_rx; /* 为可读性好临时用*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    NBB_MEMCPY(&(p_rx->lb_cmd_x), p1, sizeof(lb_cmd));
    p_ctrl->oam_pkt_rx++; /* 端口总数*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5秒内计数以判断是否超时没收包*/
    rx_tx_oam.rx_cnt_total++; /* 所有端口的总数*/

    if (p_tx->inf_tlv_local_x.loopback_spt == 0)
    {
        return;
    }

    if (p_rx->lb_cmd_x.cmd == LB_DISENB)
    {
        p_ctrl->rx_pkt_type = RX_LB_DISENB;       /* 包类型必须给出     */
        p_tx->org_pdu_tlv_x.rx_dislb_cmd++; /* 收到包的类型必须给出并相应累计*/
        p_ctrl->unfinished = UNF_DIS_LB_EXE;
    }

    if (p_rx->lb_cmd_x.cmd == LB_ENB)
    {
        p_ctrl->rx_pkt_type = RX_LB_ENB;
        p_tx->org_pdu_tlv_x.rx_lb_cmd++;

        if (p_ctrl->finished_state == FINISH_NORMAL)  /* 本端处于正常态，才可响应远端发来的环回命令*/
        {
            p_ctrl->unfinished = UNF_LB_EXE; /* 只有在发送完应答帧且完成MAC层操作后才回复该值*/
        }
        else
        {
            if (p_ctrl->unfinished == UNF_LB_CMD || p_ctrl->finished_state == FINISH_LB_LOCAL_CMD)
            {
                if (NBB_MEMCMP(p_ctrl->src_mac_remote, src_mac_overall, 6) < 0)
                {
                    p_ctrl->unfinished = UNF_LB_EXE;
                }
            }
        }
    }
}

NBB_VOID RxOamPortResolveOrg(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p1, NBB_INT size)  /* 定长*/
{
    port_oam_pdu *p_rx; /* 为可读性好临时用*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    NBB_MEMCPY(&(p_rx->org_pdu_tlv_x), p1, sizeof(org_pdu_tlv));
    p_ctrl->rx_pkt_type = RX_ORG_PDU; /* 不涉及  unfinished 的操作              */
    p_tx->org_pdu_tlv_x.rx_org_pdu++;
    p_ctrl->oam_pkt_rx++; /* 端口总数*/
    p_ctrl->rx_pkt_cnt_in_5s++; /* 5秒内计数以判断是否超时没收包*/
    rx_tx_oam.rx_cnt_total++; /* 所有端口的总数*/
}

NBB_VOID RxOamPortResolveCodeErr(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu *p_rx; /* 为可读性好临时用*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_pkt_type = NON_PDU_RX;         /* 收到无效包，等效于没收到包,不作任何处理*/
    p_ctrl->oam_pkt_rx_err++;
    rx_tx_oam.rx_cnt_err++;
}

NBB_BYTE RxOamPortResolve(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *pData, NBB_INT size)  /*各端口收到自己的OAM包后分析处理*/
{
    oam_header *p_oam_header; /* 为方便分析数据而设置的临时指针变量*/
    NBB_BYTE *p_oam_pdu_data;
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->oppsite_active = 1; /*对端激活*/
    p_oam_header = (oam_header *)pData;

    if (TRUE == RxOamPortResolveHdr(sub_cards, port, p_oam_header)) /*如果包头有错则计数并立刻退出*/
    {
        return 1;
    }

    p_oam_pdu_data = pData + OAM_HDR_SIZE; /*p_oam_pdu_data指向出共同CFM PDU Header后的第一字节*/

    switch (*(&(p_oam_header->code)))
    {
        case CODE_INF:
            RxOamPortResolveInf(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_EVENT:
            RxOamPortResolveEvent(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_VAR_REQ:
            RxOamPortResolveVarReq(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_VAR_RES:
            RxOamPortResolveVarRes(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_LOOPBACK:
            RxOamPortResolveLb(sub_cards, port, p_oam_pdu_data, size);
            break;

        case CODE_ORG:
            RxOamPortResolveOrg(sub_cards, port, p_oam_pdu_data, size);
            break;

        default:
            RxOamPortResolveCodeErr(sub_cards, port);
            break;
    }

    return 0;
}

/**************************************************
** 函数名称：RxLinkOam
** 功能描述：收EFM OAM PDU
** 输    入：uint: 收报文芯片单元，目前未0
             pkt:收包信息指针，包括:数据，包长，入端口等信息
** 输    出：无
** 作    者：yy09
** 日    期：2012.06.12
***************************************************/
c3_rx_ret_t RxLinkOam(NBB_INT unit, c3_rxtx_pkt_t *pkt)
{
    NBB_BYTE *pData;
    NBB_BYTE rx_port, rx_card, i;
    NBB_USHORT data_size;
    minm_header_t *p_head;
    rx_pkts++;
    pData = pkt->payload; /*数据从这开始存*/
    data_size = pkt->len;

    if (data_size > (oam_max_size + 0))    /*包长检查*/
    {
        goto RxErrorExit;
    }

    pData = pkt->payload; /*接受数据指针*/
    p_head = pkt->head; //MACINMAC头

    /*解析收包端口*/
    if (ETH_PORT_TYPE != p_head->port_type)
    {
        goto RxErrorExit;//端口类型不正确
    }

    rx_card = p_head->port_instance.eth_port_bits.cards;

    if (2 == rx_card) /*端口检查*/
    {
        rx_port = p_head->port_instance.eth_port_bits.ports - 65;

        if (rx_port > (MAX_PORT_NUM - 1))
        {
            goto RxErrorExit;
        }
    }
    else
    {
        rx_port = p_head->port_instance.eth_port_bits.ports - 33;

        if (rx_port > (MAX_PORT_NUM - 1))
        {
            goto RxErrorExit;
        }
    }

    for (i = 0 ; i < SUB_NUM; i++)
    {
        if (g_efm_oam_port_cfg[i].port_oam_x[rx_port].enb == 0
            || g_efm_oam_port_cfg[i].port_oam_enb_cfg_last[rx_port] == 0)  /*收到的端口未使能OAM*/
        {
            goto RxErrorExit;
        }

        if (RxOamPortResolve(i, rx_port, pData, data_size))   /*报文解析处理*/
        {
            goto RxErrorExit;
        }
    }

    return C3_RX_HANDLED;
RxErrorExit:
    rx_tx_oam.rx_cnt_err++; /*因总长度非法或目的端口未开启OAM而错*/
    return C3_RX_NOT_HANDLED;
}

NBB_VOID AnalyzeRxInf(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE rx_pkt_type, NBB_BYTE *p)
{
    port_oam_pdu *p_rx; /* 只为形式上可读性好临时用*/
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    inf_tlv inf_tlv_tmp; /* 只是为了方便读写而设置的临时变量*/
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    NBB_MEMCPY((char *)(&inf_tlv_tmp), p, INF_TLV_LEN);

    if (rx_pkt_type == RX_INF_NO_TLV)  /* 只提示是否有事件，这在包头已处理了*/
    {
        return;
    }

    NBB_MEMCPY(&(p_rx->inf_tlv_local_x), p, INF_TLV_LEN);     /* 存在接收区的本地*/
    /*存在发送区的对端，为接着的发送作数据准备*/
    NBB_MEMCPY(&(p_tx->inf_tlv_remote_x), p, INF_TLV_LEN);
    /*只将发送区对端信息表的类型改一下*/
    p_tx->inf_tlv_remote_x.type = INF_TLV_TYPE_WITH_REMOTE;

    /*只对收发同步即ORG信息处理，对remote_inf不需处理*/
    if (rx_pkt_type == RX_INF_LOCAL_REMOTE)
    {
        NBB_MEMCPY(&(p_rx->org_inf_tlv_x), p + INF_TLV_LEN + INF_TLV_LEN,
                   INF_ORG_TLV_LEN);
        p_ctrl->remote_sq = p_rx->org_inf_tlv_x.local_sq;
    }
}

NBB_VOID AnalyzeRxSymErrEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)  /* 对端告的错误符号记录*/
{
    port_oam_pdu *p_rx;
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    /*将数据存入接收区 */
    NBB_MEMCPY(&(p_rx->err_sym_period_tlv_x), p, EVENT_SYM_TLV_LEN);
    p_ctrl->rx_sym_err_cnt = p_rx->err_sym_period_tlv_x.err;
}

NBB_VOID AnalyzeRxFrmErrEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)  /* 对端告的错误帧记录*/
{
    port_oam_pdu *p_rx;
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    /* 将数据存入接收区 */
    NBB_MEMCPY(&(p_rx->err_frm_tlv_x), p, EVENT_FRM_TLV_LEN);
    p_ctrl->rx_frm_err_cnt = p_rx->err_frm_tlv_x.err;
}

NBB_VOID AnalyzeRxFrmPeriodEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)
/* 对端告的指定期内错误帧记录*/
{
    port_oam_pdu *p_rx;
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    /* 将数据存入接收区 */
    NBB_MEMCPY(&(p_rx->err_frm_period_tlv_x), p, EVENT_FRM_PRD_TLV_LEN);
    p_ctrl->rx_frm_prd_err_cnt = p_rx->err_frm_period_tlv_x.err;
}

/* 对端告的错误错误帧秒数记录*/
NBB_VOID AnalyzeRxFrmSecondEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)
{
    port_oam_pdu *p_rx; /* 为可读性好临时用*/
    oam_control_status *p_ctrl;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    /* 将数据存入接收区 */
    NBB_MEMCPY(&(p_rx->err_frm_second_tlv_x), p, EVENT_FRM_SECOND_TLV_LEN);
    p_ctrl->rx_frm_snd_err_cnt = p_rx->err_frm_second_tlv_x.err;
}

NBB_VOID AnalyzeRxOrgEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE *p)
{
    port_oam_pdu *p_rx; /* 为可读性好临时用     */
    oam_control_status *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    /* 将数据存入接收区 */
    NBB_MEMCPY(&(p_rx->org_event_tlv_x), p, EVENT_ORG_TLV_LEN);
    p_ctrl->remote_sq = p_rx->org_event_tlv_x.local_sq;
}

NBB_VOID ReactRxVarReq(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status *p_ctrl; /* 为可读性好临时用     */
    port_oam_pdu *p_tx;
    NBB_BYTE i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    for (i = 0; i < p_tx->var_num; i++)
    {
        if (p_tx->var_res_x[i].branch == VALID_BRANCH)
        {
            switch (p_tx->var_res_x[i].leaf)  /* 2/5/6/8/0xe*/
            {
                case 2:
                    /* 暂定义一切回复的变量值占4字节*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[TX_FRM];
                    break;

                case 5:
                    /* 暂定义一切回复的变量值占4字节*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[RX_FRM];
                    break;

                case 6:
                    /* 暂定义一切回复的变量值占4字节*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[RX_CRC];
                    break;

                case 8:
                    /* 暂定义一切回复的变量值占4字节*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[TX_BYTE];
                    break;

                case 0xe:
                    /* 暂定义一切回复的变量值占4字节*/
                    p_tx->var_res_x[i].width = 4;
                    p_tx->var_res_x[i].value = p_ctrl->local_var_cnt[RX_BYTE];
                    break;

                default:
                    p_tx->var_res_x[i].width = 0x81;
                    p_tx->var_res_x[i].value = 0;
                    break;
            }
        }
        else
        {
            /* 本端不支持或别的原因不能应答，要注意此时 value 无数据即不占字节*/
            p_tx->var_res_x[i].width = 0x81;
            p_tx->var_res_x[i].value = 0;
        }
    }
}
/* 初始化oam_cfg_tx oam_pdu_cfg_tx 由网管配置数据*/
NBB_VOID PortOamCtrlInitCfg(oam_control_status* p, NBB_BYTE port)
{
    p->max_size_recept = 1518; /* 协商值为默认的1518*/
}

/* 配置对每个使能OAM的端口发送数据结构初始化,而接收区则不需初始化*/
NBB_VOID PortOamPduInitCfg(NBB_BYTE sub_cards, port_oam_pdu* p, NBB_BYTE port)
{
    NBB_MEMCPY(p->oam_pdu_x.oam_header_x.dst_mac, dst_mac_overall, 6);   /* 包头*/
    NBB_MEMCPY(p->oam_pdu_x.oam_header_x.src_mac, src_mac_overall, 6);
    NBB_MEMCPY(p->oam_pdu_x.oam_header_x.len_type, len_type_overall, 2);
    p->oam_pdu_x.oam_header_x.subtype = subtype_overall; /* 一个字节不好用  OS_MEMSET */
    /* 为包的内容（不包括包头）开辟一段空间，*/
    p->oam_pdu_x.data = (NBB_BYTE*)NBB_MM_ALLOC(oam_max_size, NBB_NORETRY_ACT,
                        MEM_SPM_OAM_CFG_CB);
    p->inf_tlv_local_x.type = INF_TLV_TYPE_LOCAL; /* 本端信息包*/
    p->inf_tlv_local_x.length = INF_TLV_LEN;
    p->inf_tlv_local_x.oam_ver = 0x01; /* 未再赋值的都是取开始设置为0*/
    p->inf_tlv_local_x.revision = 0x00;
    p->inf_tlv_local_x.oam_mode = g_efm_oam_port_cfg[sub_cards].port_oam_mode_cfg[port];
    p->inf_tlv_local_x.unidirection_spt = g_efm_oam_port_cfg[sub_cards].port_oam_sup_uni_cfg[port];
    p->inf_tlv_local_x.loopback_spt = g_efm_oam_port_cfg[sub_cards].port_oam_sup_lb_cfg[port];
    p->inf_tlv_local_x.link_event_spt = g_efm_oam_port_cfg[sub_cards].port_oam_sup_event_cfg[port];
    p->inf_tlv_local_x.var_response_spt = g_efm_oam_port_cfg[sub_cards].port_oam_sup_var_cfg[port];
    p->inf_tlv_local_x.max_size = g_efm_oam_port_cfg[sub_cards].port_oam_maxsize_cfg[port];
    NBB_MEMCPY(p->inf_tlv_local_x.oui, wri_fh_oui, 3);
    NBB_MEMCPY(p->inf_tlv_local_x.ven_spe_inf, board_name, 4);
    p->inf_tlv_remote_x.type = INF_TLV_TYPE_WITH_REMOTE; /* 对端信息包*/
    p->inf_tlv_remote_x.length = INF_TLV_LEN;
    p->inf_tlv_remote_x.oam_ver = 0x01;
    p->org_inf_tlv_x.type = 0xfe; /* 自定义在信息包中传递的收发SQ*/
    p->org_inf_tlv_x.length = EVENT_ORG_TLV_LEN;
    NBB_MEMCPY(p->org_inf_tlv_x.oui, wri_fh_oui, 3);
    p->org_event_tlv_x.type = 0xfe; /* 自定义在事件包中传递的收发SQ*/
    p->org_event_tlv_x.length = EVENT_ORG_TLV_LEN;
    /* 目前我们定义的ORQ对事件和信息包传递的内容其实完全雷同可替*/
    NBB_MEMCPY(p->org_event_tlv_x.oui, wri_fh_oui, 3);
    NBB_MEMCPY(p->org_pdu_tlv_x.oui, wri_fh_oui, 3);   /* 帧长27*/
    p->err_sym_period_tlv_x.type = EVENT_TLV_TYPE_SYM_PERIOD;
    p->err_sym_period_tlv_x.length = EVENT_SYM_TLV_LEN;
    p->err_sym_period_tlv_x.upper_window = 0x0;
    /*暂定义1G的1秒内发的SYMBOL（对应半个字节即4B/5B转换对应一个SYMBOL），
    对应物理层收发SYMBOL的满带宽*/
    p->err_sym_period_tlv_x.window = 0x1DCD6500 * g_efm_oam_port_cfg[sub_cards].port_oam_sym_period_cfg[port];
    /* 获取PHY层工作状态 */
    p->err_sym_period_tlv_x.threshold = g_efm_oam_port_cfg[sub_cards].port_oam_sym_threshold_cfg[port];
    p->err_frm_tlv_x.type = EVENT_TLV_TYPE_FRM;
    p->err_frm_tlv_x.length = EVENT_FRM_TLV_LEN;
    p->err_frm_tlv_x.window = g_efm_oam_port_cfg[sub_cards].port_oam_frm_period_cfg[port]; /* 1秒内*/
    /* 和错误秒计时一样，只是那里统计范围更广为60秒*/
    p->err_frm_tlv_x.threshold = g_efm_oam_port_cfg[sub_cards].port_oam_frm_threshold_cfg[port];
    p->err_frm_period_tlv_x.type = EVENT_TLV_TYPE_FRM_PERIOD;
    p->err_frm_period_tlv_x.length = EVENT_FRM_PRD_TLV_LEN;
    /* 1秒内千兆传64字节包数*/
    p->err_frm_period_tlv_x.window = 1488000 * g_efm_oam_port_cfg[sub_cards].port_oam_frm_prd_period_cfg[port];
    /* 表示这是比上面次要的事情因为在更长的时间内才发生*/
    p->err_frm_period_tlv_x.threshold = g_efm_oam_port_cfg[sub_cards].port_oam_frm_prd_threshold_cfg[port];
    p->err_frm_second_tlv_x.type = EVENT_TLV_TYPE_SECOND_SUM;
    p->err_frm_second_tlv_x.length = EVENT_FRM_SECOND_TLV_LEN;
    p->err_frm_second_tlv_x.window = g_efm_oam_port_cfg[sub_cards].port_oam_frm_snd_period_cfg[port]; /* 1分钟内*/
    /* 至少有1秒有错误帧  这是相对较轻的错误，但统计更全*/
    p->err_frm_second_tlv_x.threshold = g_efm_oam_port_cfg[sub_cards].port_oam_frm_snd_threshold_cfg[port];
    /* var_req_x  var_res_x  lb_cmd_x都开始赋值0即完成了初始化      */
}

/* 配置哪些端口开启OAM，配置是否使能总体OAM收发驱动*/
NBB_VOID AllPortOamCfgEnb(NBB_VOID)
{
    NBB_BYTE enb, i, j;
    enb = 0;

    /*port_oam_enb_cfg[26]=1;
    port_oam_enb_cfg[27]=1;
    lb_overtime=10;
    g_efm_oam_port_cfg[sub_cards].port_oam_mode_cfg[26]=1;
    g_efm_oam_port_cfg[sub_cards].port_oam_mode_cfg[27]=1;
    ApiSetEtherOamPacketToCpu(0, 26, dst_mac_overall, 0x8809, 1);
    ApiSetEtherOamPacketToCpu(0, 27, dst_mac_overall, 0x8809, 1);

    ApiSetPortPvid(0,28,5);
    ApiSetPortPvid(0,29,5);
    ApiSetPortPvid(0,30,5);
    ApiSetPortPvid(0,31,5);

    ApiSetPortPvid(0,26,55);
    ApiSetPortPvid(0,27,33);


    src_mac_overall[3]=0x1;
    src_mac_overall[4]=0x1;
    src_mac_overall[5]=0x1;*/
    for (j = 0 ; j < SUB_NUM; j++)
    {
        for (i = 0; i < MAX_PORT_NUM; i++)
        {
            if (TRUE == g_efm_oam_port_cfg[j].port_oam_enb_cfg[i])
            {
                enb = 1;
            }

            if (g_efm_oam_port_cfg[j].port_oam_enb_cfg[i]
                != g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[i])
            {
                if (TRUE == g_efm_oam_port_cfg[j].port_oam_enb_cfg[i])
                {
                    g_efm_oam_port_cfg[j].port_oam_x[i].enb = 1;
                    g_efm_oam_port_cfg[j].port_oam_x[i].port_num = i;
                    g_efm_oam_port_cfg[j].port_oam_x[i].p_tx = (port_oam_pdu*)NBB_MM_ALLOC(sizeof(port_oam_pdu),
                            NBB_NORETRY_ACT, MEM_SPM_OAM_CFG_CB);
                    g_efm_oam_port_cfg[j].port_oam_x[i].p_rx = (port_oam_pdu*)NBB_MM_ALLOC(sizeof(port_oam_pdu),
                            NBB_NORETRY_ACT, MEM_SPM_OAM_CFG_CB);
                    g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl =
                        (oam_control_status*)NBB_MM_ALLOC(sizeof(oam_control_status),
                                                          NBB_NORETRY_ACT, MEM_SPM_OAM_CFG_CB);
                    OS_MEMSET(g_efm_oam_port_cfg[j].port_oam_x[i].p_tx, 0, sizeof(port_oam_pdu));     /* 全部变量赋值0*/
                    OS_MEMSET(g_efm_oam_port_cfg[j].port_oam_x[i].p_rx, 0, sizeof(port_oam_pdu));
                    OS_MEMSET(g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl, 0, sizeof(oam_control_status));
                    /*为该端口发送数据结构初始化，开辟DATA空间，而接收则清零即可*/
                    PortOamPduInitCfg(j, g_efm_oam_port_cfg[j].port_oam_x[i].p_tx, i);
                    /* 初始化oam_cfg_tx oam_pdu_cfg_tx*/
                    PortOamCtrlInitCfg(g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl, i);
                }
                else
                {
                    // NBB_PRINTF("port %d delete\n",i);
                    g_efm_oam_port_cfg[j].port_oam_x[i].enb = 0;
                    g_efm_oam_port_cfg[j].port_oam_x[i].port_num = i;

                    /* 别的指针可直接赋值给  NBB_VOID*变量或参数*/
                    if (NULL != g_efm_oam_port_cfg[j].port_oam_x[i].p_tx)
                    {
                        NBB_MM_FREE(g_efm_oam_port_cfg[j].port_oam_x[i].p_tx, MEM_SPM_OAM_CFG_CB);
                        g_efm_oam_port_cfg[j].port_oam_x[i].p_tx = NULL;
                    }

                    if (NULL != g_efm_oam_port_cfg[j].port_oam_x[i].p_rx)
                    {
                        NBB_MM_FREE(g_efm_oam_port_cfg[j].port_oam_x[i].p_rx, MEM_SPM_OAM_CFG_CB);
                        g_efm_oam_port_cfg[j].port_oam_x[i].p_rx = NULL;
                    }

                    if (NULL != g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl)
                    {
                        NBB_MM_FREE(g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl, MEM_SPM_OAM_CFG_CB);
                        g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl = NULL;
                    }
                }

                g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[i] = g_efm_oam_port_cfg[j].port_oam_enb_cfg[i];
            }
        }

        PortOamEnableApi(0, enb);
    }
}

NBB_VOID RxTxLinkOamEnbInit(NBB_VOID)/* 对收发OAM结构清零*/
{
    OS_MEMSET(&rx_tx_oam, 0, sizeof(rx_tx_oam));     /* 全部变量赋值0*/
}

/* 执行注册或注销OAM收发驱动*/
NBB_INT PortOamEnableApi(NBB_INT unit, NBB_BYTE enable)
{
    unsigned char funcname[FUNCNMLEN] = {0};
    NBB_INT  rx_error = 0, tx_error = 0;
    c3_rxreg_info_t rxcb;

    if ((enable == 1) && (oam_enb_cfg == 0))
    {
        /*1) 注册接收任务*/
        rxcb.name = funcname;
        rxcb.priority = 0;
        rxcb.callback = RxLinkOam;
        rxcb.type = 18;//临时用
        rx_error = ApiC3RxCbRegister(unit, &rxcb);
        /*2) 启动发送任务*/
        RxTxLinkOamEnbInit();
        oam_enb_cfg = 1;
    }
    else if ((enable == 0) && (oam_enb_cfg == 1))
    {
        oam_enb_cfg = 0;
        /*1) 注销接收任务*/
        rxcb.priority = 0;
        rxcb.callback = RxLinkOam;
        rxcb.type = 18;//临时用
        rx_error = ApiC3RxCbUnRegister(unit, &rxcb);
    }

    rx_tx_oam.rx_error = rx_error;
    rx_tx_oam.tx_error = tx_error;
    return (rx_error < 0) ? rx_error : tx_error;
}

/************************************  响应
****************************************/
NBB_VOID PortOamEnbCfg(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE cfg)
{
    g_efm_oam_port_cfg[sub_cards].port_oam_x[port].enb = cfg;
}

/*****************************************************************************
功能： 执行状态监视查询命令的应答数据组装，用户有新BCT
协议时才依据原协议分析并修改函数。。
作者： hmliu
时间： 20080904
输入： port：端口；
输出： 指向CHAR 数组的指针
说明： 一般用户不需关注或调用此函数，但用户如需灵活修改BCT
协议，则应增删此函数某些行代码
修改：
******************************************************************************/
NBB_BYTE *QueryPortOamApi(NBB_BYTE sub_cards, NBB_BYTE port)
{
    NBB_BYTE i = 0;
    port_oam_pdu                    *p_tx;
    port_oam_pdu                    *p_rx;
    oam_control_status              *p_ctrl;
    OS_MEMSET(lookup, 0, LOOKUP_MAX_SIZE);

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg[port] == 0)
    {
        return(lookup);
    }

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg[port])
    {
        p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
        p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        lookup[i++] = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].enb;
        lookup[i++] = p_tx->inf_tlv_local_x.oam_mode;
        lookup[i++] = p_ctrl->oppsite_active;

        /*110409 在收到对端LINK FAULT时逻辑报SEND态*/
        if (p_rx->oam_pdu_x.oam_header_x.link_fault)
        {
            lookup[i++] = 0;
        }
        else
        {
            if (p_tx->oam_pdu_x.oam_header_x.local_discovering == DISCOVER_UNFINISHED)
            {
                lookup[i++] = 0;
            }
            else
            {
                lookup[i++] = p_tx->oam_pdu_x.oam_header_x.local_stable + 1;
            }
        }

        lookup[i++] = (p_ctrl->max_size_recept >> 8);
        lookup[i++] = (p_ctrl->max_size_recept);

        if (p_rx->oam_pdu_x.oam_header_x.link_fault)  //110409
        {
            lookup[i++] = DISCOVER_SEND;
        }
        else
        {
            lookup[i++] = p_ctrl->discovery_status;
        }

        lookup[i++] = p_ctrl->local_pdu;
        lookup[i++] = p_ctrl->finished_state;
        lookup[i++] = p_tx->inf_tlv_local_x.mux_action;

        /*lookup[i++]=p_tx->inf_tlv_local_x.par_action;100606*/
        if (p_tx->inf_tlv_local_x.par_action == 1)
        {
            lookup[i++] = 2;
        }
        else if (p_tx->inf_tlv_local_x.par_action == 2)
        {
            lookup[i++] = 1;
        }
        else
        {
            lookup[i++] = 0;
        }

        lookup[i++] = p_ctrl->var_lb_fail_ais;
        lookup[i++] = p_ctrl->local_link_status_last;
        lookup[i++] = p_ctrl->remote_link_status_last;
        lookup[i++] = p_ctrl->oppsite_active;
        lookup[i++] = p_rx->inf_tlv_local_x.oam_mode;

        if (p_ctrl->finished_state == FINISH_LB_REMOTE_EXE)
        {
            lookup[i++] = 1;
        }
        else if (p_rx->inf_tlv_local_x.par_action == PAR_LB)
        {
            lookup[i++] = 2;
        }
        else
        {
            lookup[i++] = 0;
        }

        lookup[i++] = p_rx->inf_tlv_local_x.mux_action;

        if (p_rx->inf_tlv_local_x.par_action == 1)
        {
            lookup[i++] = 2;
        }
        else if (p_rx->inf_tlv_local_x.par_action == 2)
        {
            lookup[i++] = 1;
        }
        else
        {
            lookup[i++] = 0;
        }

        /*lookup[i++]=p_rx->inf_tlv_local_x.par_action;100606  原来的LB和DISCARD
        状态搞反了*/
        NBB_MEMCPY(&(lookup[i]), p_ctrl->src_mac_remote, 6);
        i = i + 6;
        NBB_MEMCPY(&(lookup[i]), p_rx->inf_tlv_local_x.oui, 3);
        i = i + 3;
        lookup[i++] = p_rx->var_res_x[0].branch;
        lookup[i++] = p_rx->var_res_x[0].leaf;

        if (p_ctrl->late_rx_var_res)
        {
            lookup[i++] = 1;
        }
        else
        {
            lookup[i++] = ((p_rx->var_res_x[0].width) >> 7) & 0x1;
        }

        lookup[i++] = ((p_rx->var_res_x[0].value) >> 8);
        lookup[i++] = ((p_rx->var_res_x[0].value));
        lookup[i++] = p_ctrl->rx_sym_err;
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_sym_period_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_sym_period_tlv_x.window / 0x1DCD6500);
        lookup[i++] = ((p_ctrl->rx_sym_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_sym_err_cnt));
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_sym_period_tlv_x.event_ttl));
#if 0
        lookup[i++] = p_ctrl->rx_frm_err;
        lookup[i++] = ((p_rx->err_frm_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_tlv_x.window);
        lookup[i++] = ((p_ctrl->rx_frm_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_err_cnt));
        lookup[i++] = ((p_rx->err_frm_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->rx_frm_prd_err;
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_period_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_period_tlv_x.window / 1488000);
        lookup[i++] = ((p_ctrl->rx_frm_prd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_prd_err_cnt));
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.event_ttl));
#endif
        lookup[i++] = p_ctrl->rx_frm_prd_err;/*110616
纠正帧周期事件和错误帧事件搞反错误*/
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_period_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_period_tlv_x.window / 1488000);
        lookup[i++] = ((p_ctrl->rx_frm_prd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_prd_err_cnt));
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_period_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->rx_frm_err;
        lookup[i++] = ((p_rx->err_frm_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_tlv_x.window);
        lookup[i++] = ((p_ctrl->rx_frm_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_err_cnt));
        lookup[i++] = ((p_rx->err_frm_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->rx_frm_snd_err;
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_second_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_rx->err_frm_second_tlv_x.window);
        lookup[i++] = ((p_ctrl->rx_frm_snd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->rx_frm_snd_err_cnt));
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.err_ttl));
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_rx->err_frm_second_tlv_x.event_ttl));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_inf) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_inf));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_lb_cmd) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_lb_cmd));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_dislb_cmd) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_dislb_cmd));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_event) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_event));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_var_req) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_var_req));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_var_res) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.rx_var_res));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_inf) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_inf));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_lb_cmd) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_lb_cmd));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_dislb_cmd) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_dislb_cmd));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_event) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_event));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_var_req) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_var_req));
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_var_res) >> 8);
        lookup[i++] = ((p_tx->org_pdu_tlv_x.tx_var_res));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_inf) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_inf));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_lb_cmd) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_lb_cmd));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_dislb_cmd) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_dislb_cmd));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_event) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_event));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_var_req) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_var_req));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_var_res) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.rx_var_res));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_inf) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_inf));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_lb_cmd) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_lb_cmd));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_dislb_cmd) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_dislb_cmd));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_event) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_event));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_var_req) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_var_req));
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_var_res) >> 8);
        lookup[i++] = ((p_rx->org_pdu_tlv_x.tx_var_res));
        lookup[i++] = p_ctrl->tx_sym_err;
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_tx->err_sym_period_tlv_x.threshold);
        /*1000 Base T 取  0x1DCD6500 对应500M；对于100BaseTx  取 0x7735940 即对应
        125M；对于10Base T取 0x1312d00 即20M  125*/
        lookup[i++] = (NBB_BYTE)(p_tx->err_sym_period_tlv_x.window / 0x1DCD6500);
        lookup[i++] = ((p_ctrl->tx_sym_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->tx_sym_err_cnt));
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.err_ttl));
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_tx->err_sym_period_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->tx_frm_err;
        lookup[i++] = ((p_tx->err_frm_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_tx->err_frm_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_tlv_x.window);
        lookup[i++] = ((p_ctrl->tx_frm_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->tx_frm_err_cnt));
        lookup[i++] = ((p_tx->err_frm_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_tlv_x.err_ttl));
        lookup[i++] = ((p_tx->err_frm_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->tx_frm_prd_err;
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_period_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_period_tlv_x.window / 1488000);
        lookup[i++] = ((p_ctrl->tx_frm_prd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->tx_frm_prd_err_cnt));
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.err_ttl));
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_period_tlv_x.event_ttl));
        lookup[i++] = p_ctrl->tx_frm_snd_err;
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.time_stamp) >> 8);
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.time_stamp));
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_second_tlv_x.threshold);
        lookup[i++] = (NBB_BYTE)(p_tx->err_frm_second_tlv_x.window);
        lookup[i++] = ((p_ctrl->tx_frm_snd_err_cnt) >> 8);
        lookup[i++] = ((p_ctrl->tx_frm_snd_err_cnt));
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.err_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.err_ttl));
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.event_ttl) >> 8);
        lookup[i++] = ((p_tx->err_frm_second_tlv_x.event_ttl));
    }

    p_ctrl->late_rx_var_res = 0;
    p_rx->var_res_x[0].branch = 0;
    p_rx->var_res_x[0].leaf = 0;
    p_rx->var_res_x[0].width = 0;
    p_rx->var_res_x[0].value = 0;
    p_ctrl->rx_sym_err = 0;
    p_ctrl->rx_sym_err_cnt = 0;
    p_ctrl->rx_frm_err = 0;
    p_ctrl->rx_frm_err_cnt = 0;
    p_ctrl->rx_frm_prd_err = 0;
    p_ctrl->rx_frm_prd_err_cnt = 0;
    p_ctrl->rx_frm_snd_err = 0;
    p_ctrl->rx_frm_snd_err_cnt = 0;
    p_ctrl->tx_sym_err = 0;
    p_ctrl->tx_sym_err_cnt = 0;
    p_ctrl->tx_frm_err = 0;
    p_ctrl->tx_frm_err_cnt = 0;
    p_ctrl->tx_frm_prd_err = 0;
    p_ctrl->tx_frm_prd_err_cnt = 0;
    p_ctrl->tx_frm_snd_err = 0;
    p_ctrl->tx_frm_snd_err_cnt = 0;
    /*for(i=0;i<170;i++)
    {
            if(lookup[i]<0x10)
                    NBB_PRINTF("0%x  ", lookup[i]);
                    else
                            NBB_PRINTF("%x  ", lookup[i]);

            if((i+1)%10==0)
                    NBB_PRINTF("....LINE %d \n",(i+1));

    }101229*/
    return(lookup);
}

#if 0
/*****************************************************************************
功能： 获取本芯片交换机的MAC，以做OAM的本端源MAC；应在 CONFIG中
并配置端口前调用
作者： hmliu
时间： 20080904
输入： unit:即芯片号，一般固定输入0，除非单盘有多块芯片同时应用，但暂不考虑
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID SetSrcMac(NBB_INT unit)
{
    if (ApiGetGlobalMacAddr(unit, src_mac_overall))
    {
        NBB_PRINTF("SetSrcMac FAIL\n");
    }
}
#endif

NBB_VOID ExeMacTxRx(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/* 为可读性好临时用*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_ctrl->need_exe_mac == 0)
    {
        return;
    }

    switch (p_ctrl->finished_state)
    {
        case FINISH_NORMAL:
            ExeMacRxTxMod(sub_cards, port, MUX_FWD, PAR_FWD);
            break;

        case FINISH_LB_REMOTE_EXE:
            ExeMacRxTxMod(sub_cards, port, MUX_DISCARD, PAR_LB);
            break;

        case FINISH_LB_LOCAL_CMD:
            ExeMacRxTxMod(sub_cards, port, MUX_FWD, PAR_DISCARD);
            break;

        default:
            break;
    }

    p_ctrl->need_exe_mac = 0;
}

NBB_VOID SendInfNoTlv(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_INF; /*更新CODE*/
    p_ctrl->tx_pkt_type = TX_INF_NO_TLV;
    p_tx->oam_pdu_x.data_size = 0;
    p_tx->org_pdu_tlv_x.tx_inf++;
    //NBB_PRINTF("SendInfNoTlv  %d\n",port);
}

NBB_VOID SendInfLocalTlv(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_INF; /*更新CODE*/
    p_ctrl->tx_pkt_type = TX_INF_LOCAL;
    NBB_MEMCPY(p_tx->oam_pdu_x.data, &(p_tx->inf_tlv_local_x), INF_TLV_LEN);
    p_tx->oam_pdu_x.data_size = INF_TLV_LEN;
    p_tx->org_pdu_tlv_x.tx_inf++;
    //      NBB_PRINTF("SendInfLocalTlv  %d\n",port);
}

NBB_VOID SendInfLocalRemoteTlv(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    NBB_BYTE                        *p;
    //NBB_PRINTF("SendInfLocalRemoteTlv  %d\n",port);
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = p_tx->oam_pdu_x.data;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_INF; /*更新CODE*/
    p_ctrl->tx_pkt_type = TX_INF_LOCAL_REMOTE;
    NBB_MEMCPY(p, &(p_tx->inf_tlv_local_x), INF_TLV_LEN);
    p += INF_TLV_LEN;
    NBB_MEMCPY(p, &(p_tx->inf_tlv_remote_x), INF_TLV_LEN);
    p += INF_TLV_LEN;
    NBB_MEMCPY(p, &(p_tx->org_inf_tlv_x), INF_ORG_TLV_LEN);
    p_tx->oam_pdu_x.data_size = INF_TLV_LEN + INF_TLV_LEN + INF_ORG_TLV_LEN;
    p_tx->org_pdu_tlv_x.tx_inf++;
}

NBB_VOID SendVarRes(NBB_BYTE sub_cards, NBB_BYTE port)  /*应答对端*/
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    NBB_BYTE                        *p;
    NBB_BYTE                        i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = p_tx->oam_pdu_x.data;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_VAR_RES; /*更新CODE*/
    p_ctrl->tx_pkt_type = TX_RES;
    p_tx->oam_pdu_x.data_size = 0;

    for (i = 0; i < p_tx->var_num; i++)
    {
        if (p_tx->var_res_x[i].width & 0x80)
        {
            /*不能满足对方的查询请求即返回不了数据*/
            NBB_MEMCPY(p, &(p_tx->var_res_x[i]), VAR_RES_TLV_LEN);
            /*即使查询失败，暂返回仍是7个数据即VALUE赋值0返，VAR_RES_TLV_FAIL用
            于以后多个变量查询处理，此处应与ReactRxVarReq配合*/
            p += VAR_RES_TLV_LEN;
            p_tx->oam_pdu_x.data_size += VAR_RES_TLV_LEN;
        }
        else
        {
            NBB_MEMCPY(p, &(p_tx->var_res_x[i]), VAR_RES_TLV_LEN);
            p += VAR_RES_TLV_LEN;
            p_tx->oam_pdu_x.data_size += VAR_RES_TLV_LEN;
        }
    }

    p_tx->org_pdu_tlv_x.tx_var_res++;
}

/*固定在最后发ORG事件，（事件值自动更新，网管命令只管发哪种事件）*/
NBB_VOID SendEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    NBB_BYTE                        *p;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = p_tx->oam_pdu_x.data;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_EVENT; /*更新CODE*/
    p_ctrl->tx_pkt_type = TX_EVENT;
    p_tx->oam_pdu_x.data_size = 0;
    p_ctrl->event_tx_sq++;
    NBB_MEMCPY(p, &(p_ctrl->event_tx_sq), 2);     /*更新发送事件SQ*/
    p += 2;
    p_tx->oam_pdu_x.data_size += 2;

    if (p_ctrl->sym_period_req)
    {
        /*有SYM事件发送请求*/
        p_tx->err_sym_period_tlv_x.time_stamp = time_100ms;
        NBB_MEMCPY(p, &(p_tx->err_sym_period_tlv_x), EVENT_SYM_TLV_LEN);     /*装数据*/
        p += EVENT_SYM_TLV_LEN; /*指针往后移定位*/
        p_tx->oam_pdu_x.data_size += EVENT_SYM_TLV_LEN; /*数据尺寸更新*/
        p_tx->err_sym_period_tlv_x.err = 0; /*发送区清除当前错误标记*/
        p_ctrl->sym_period_req = 0; /*请求标识清除因为已处理*/
        NBB_PRINTF("port sym_period_req %d \n", port);
    }

    if (p_ctrl->frm_err_req)
    {
        p_tx->err_frm_tlv_x.time_stamp = time_100ms;
        NBB_MEMCPY(p, &(p_tx->err_frm_tlv_x), EVENT_FRM_TLV_LEN);
        p += EVENT_FRM_TLV_LEN;
        p_tx->oam_pdu_x.data_size += EVENT_FRM_TLV_LEN;
        p_tx->err_frm_tlv_x.err = 0;
        p_ctrl->frm_err_req = 0;
        NBB_PRINTF("port frm_err_req %d \n", port);
    }

    if (p_ctrl->frm_period_req)
    {
        p_tx->err_frm_period_tlv_x.time_stamp = time_100ms;
        NBB_MEMCPY(p, &(p_tx->err_frm_period_tlv_x), EVENT_FRM_PRD_TLV_LEN);
        p += EVENT_FRM_PRD_TLV_LEN;
        p_tx->oam_pdu_x.data_size += EVENT_FRM_PRD_TLV_LEN;
        p_tx->err_frm_period_tlv_x.err = 0;
        p_ctrl->frm_period_req = 0;
        NBB_PRINTF("port frm_period_req %d \n", port);
    }

    if (p_ctrl->second_period_req)
    {
        p_tx->err_frm_second_tlv_x.time_stamp = time_100ms;
        NBB_MEMCPY(p, &(p_tx->err_frm_second_tlv_x), EVENT_FRM_SECOND_TLV_LEN);
        p += EVENT_FRM_SECOND_TLV_LEN;
        p_tx->oam_pdu_x.data_size += EVENT_FRM_SECOND_TLV_LEN;
        p_tx->err_frm_second_tlv_x.err = 0;
        p_ctrl->second_period_req = 0;
        NBB_PRINTF("port second_period_req %d \n", port);
    }

    NBB_MEMCPY(p, &(p_tx->org_event_tlv_x), EVENT_ORG_TLV_LEN);     /*固定发ORG事件*/
    p += EVENT_ORG_TLV_LEN;
    p_tx->oam_pdu_x.data_size += EVENT_ORG_TLV_LEN;
    p_tx->org_pdu_tlv_x.tx_event++;
}

NBB_VOID SendLbCmd(NBB_BYTE sub_cards, NBB_BYTE port)  /*响应网管*/
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->tx_pkt_type = TX_LB_ENB;
    p_tx->lb_cmd_x.cmd = LB_ENB;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_LOOPBACK; /*更新CODE*/
    NBB_MEMCPY(p_tx->oam_pdu_x.data, &(p_tx->lb_cmd_x), sizeof(lb_cmd));
    p_tx->oam_pdu_x.data_size = sizeof(lb_cmd);
    p_tx->org_pdu_tlv_x.tx_lb_cmd++;
    p_ctrl->lb_exe_fail = 0; /*发送了一次新的 命令则清楚命令执行失败的告警*/
    p_ctrl->var_lb_fail_ais = 0;            /* 新来网管需对端应答的命令，从而原告警消失     */
    NBB_PRINTF("SendLbCmd port %d\n", port);
}
NBB_VOID SendDisLbCmd(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_LOOPBACK; /*更新CODE*/
    p_ctrl->tx_pkt_type = TX_LB_DISENB;
    p_tx->lb_cmd_x.cmd = LB_DISENB;
    NBB_MEMCPY(p_tx->oam_pdu_x.data, &(p_tx->lb_cmd_x), sizeof(lb_cmd));
    p_tx->oam_pdu_x.data_size = sizeof(lb_cmd);
    p_tx->org_pdu_tlv_x.tx_dislb_cmd++;
    p_ctrl->lb_exe_fail = 0; /*发送了一次新的 命令则清楚命令执行失败的告警*/
    p_ctrl->var_lb_fail_ais = 0;    /* 新来网管需对端应答的命令，从而原告警消失     */
    NBB_PRINTF("SendDisLbCmd port %d\n", port);
}

NBB_VOID SetVarReqFrmMgr(NBB_BYTE port)   /*响应网管命令,设置发送区变量组*/
{
}

NBB_VOID SendVarReq(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    NBB_BYTE                                                                        *p;
    NBB_BYTE                                                                        i;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = p_tx->oam_pdu_x.data;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_VAR_REQ; /*更新CODE*/
    SetVarReqFrmMgr(port);
    p_ctrl->tx_pkt_type = TX_REQ;
    p_tx->oam_pdu_x.data_size = 0;

    for (i = 0; i < p_tx->var_num; i++)
    {
        NBB_MEMCPY(p, &(p_tx->var_req_x[i]), VAR_REQ_TLV_LEN);
        p += VAR_REQ_TLV_LEN;
        p_tx->oam_pdu_x.data_size += VAR_REQ_TLV_LEN;
    }

    p_tx->org_pdu_tlv_x.tx_var_req++;
    p_ctrl->var_lb_fail_ais = 0;            /* 新来网管需对端应答的命令，从而原告警消失             */
    NBB_PRINTF("SendVarReq port %d\n", port);
}
NBB_VOID SendOrgPdu(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->oam_pdu_x.oam_header_x.code = CODE_ORG; /*更新CODE*/
    p_ctrl->tx_pkt_type = TX_ORG_PDU;
    p_tx->org_pdu_tlv_x.tx_org_pdu++;
    NBB_MEMCPY(p_tx->oam_pdu_x.data, &(p_tx->org_pdu_tlv_x), PDU_ORG_TLV_LEN);
    p_tx->oam_pdu_x.data_size = PDU_ORG_TLV_LEN;
//      NBB_PRINTF("port SendOrgPdu %d \n",port);
}

NBB_VOID OamCtrlParMux(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE mux_action, NBB_BYTE par_action)
/*置位控制信息*/
{
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->inf_tlv_local_x.par_action = par_action;
    p_tx->inf_tlv_local_x.mux_action = mux_action;
    p_ctrl->need_exe_mac = 1;
    p_tx->inf_tlv_local_x.revision++;
    /*每次执行STATE的更新，则本端发送区revision 也更新，接收区的 revision 只是获取值*/
    NBB_PRINTF("OamCtrlParMux port %d : par %d  mux %d \n", port, par_action,
               mux_action);
//      ExeMacRxTxMod(port,par_action,mux_action);/*101229  原来没执行该语句*/
    return;
}

/*实际操作更改MAC层收发模式*/
NBB_VOID ExeMacRxTxMod(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE mux_action, NBB_BYTE par_action)
{
    /*收丢弃，环回，发不使能*/
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    oam_control_status              *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    switch (par_action)
    {
        case PAR_LB:
            ExeMacRxLoopBack(port);
            break;

        case PAR_DISCARD:
            ExeMacRxDiscard(port);
            break;

        case PAR_FWD:
            ExeMacRxFwd(port);
            break;

        default:
            break;
    }

    switch (mux_action)
    {
        case MUX_DISCARD:
            ExeMacTxDiscard(port);
            break;

        case MUX_FWD:
            ExeMacTxFwd(port);
            break;

        default:
            break;
    }
}

NBB_VOID ReturnToLinkFault(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_rx;  /*为可读性好临时用*/
    port_oam_pdu                    *p_tx;
    oam_control_status      *p_ctrl;

    //return;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)  /*未开启OAM*/
    {
        return;
    }

    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    UpdateFlag(sub_cards, port);

    if (p_tx->oam_pdu_x.oam_header_x.link_fault || p_ctrl->local_lost_link_time_down)
    {
        //if(p_tx->oam_pdu_x.oam_header_x.link_fault/*||p_ctrl->local_lost_link_time_down*/){/*110410 不再状态抖动*/
        //  NBB_PRINTF("p_tx->oam_pdu_x.oam_header_x.link_fault=%d\n",p_tx->oam_pdu_x.oam_header_x.link_fault);
        //  NBB_PRINTF("p_ctrl->local_lost_link_time_down=%d\n",p_ctrl->local_lost_link_time_down);
        p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
        p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_UNFINISHED;
        p_ctrl->oppsite_active = 0;
        p_ctrl->discovery_status = DISCOVER_LINK_FAULT;
        p_ctrl->local_pdu = PDU_LF_INFO;
        //NBB_PRINTF("ReturnToLinkFault...\n");
    }
}

NBB_VOID ReturnToLinkFaultDbg(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu            *p_rx;  /*为可读性好临时用*/
    port_oam_pdu            *p_tx;
    oam_control_status      *p_ctrl;

    /*return;*/

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)  /*未开启OAM*/
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl; //110409
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;

    if (p_tx->oam_pdu_x.oam_header_x.link_fault || p_ctrl->local_lost_link_time_down)
    {
        p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
        p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_UNFINISHED;
        p_ctrl->oppsite_active = 0;
        p_ctrl->discovery_status = DISCOVER_LINK_FAULT;
        p_ctrl->local_pdu = PDU_LF_INFO;
        //  NBB_PRINTF("ReturnToLinkFault...\n");
    }
}


/***   110303   增添LINK-OAM状态的提取函数，以供应用层
调用 做相应保护动作。
*******************************************/

NBB_BYTE   LinkOamStatus_Get(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_rx;  /*为可读性好临时用*/
    port_oam_pdu                    *p_tx;
    oam_control_status          *p_ctrl;

    if ((port + 1) > MAX_PORT_NUM)
    {
        //NBB_PRINTF("ERROR:port number is invalid!\n");
        return(0);
    }

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)  /*未开启OAM*/
    {
        return(0);    /*110505 在不开启时返回为正常态。link oam失败*/
    }

    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if ((p_ctrl->discovery_status == DISCOVER_ANY)
        && (p_rx->oam_pdu_x.oam_header_x.link_fault == 0))
        //if(p_ctrl->discovery_status==DISCOVER_ANY) 110409
    {
        return(0);
    }   /*link oam已正常完成*/
    else
    {
        return(1);
    }
}



/*discovery状态*/
/*每秒调1次，和发同周期，*/
NBB_VOID DiscoveryState(NBB_BYTE sub_cards, NBB_BYTE port)
{
    port_oam_pdu                    *p_rx;  /*为可读性好临时用*/
    port_oam_pdu                    *p_tx;
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)  /*未开启OAM*/
    {
        return;
    }

    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    switch (p_ctrl->discovery_status)
    {
        case DISCOVER_LINK_FAULT:
            p_ctrl->local_pdu = PDU_LF_INFO;

            if (p_tx->oam_pdu_x.oam_header_x.link_fault == 0)
            {
                if (p_tx->inf_tlv_local_x.oam_mode == OAM_ACTIVE)
                {
                    p_ctrl->discovery_status = DISCOVER_ACTIVE;
                    p_ctrl->local_pdu = PDU_INFO;
                }

                if (p_tx->inf_tlv_local_x.oam_mode == OAM_PASSIVE)
                {
                    p_ctrl->discovery_status = DISCOVER_WAIT;
                    p_ctrl->local_pdu = PDU_RX_INFO;
                }

                p_tx->oam_pdu_x.oam_header_x.local_discovering =
                    DISCOVER_UNFINISHED; /*协商正在进行*/
            }

            break;

        case DISCOVER_WAIT:

            /*ReturnToLinkFault(port);*/
            if (p_ctrl->rx_pkt_type != NON_PDU_RX)
            {
                p_ctrl->discovery_status = DISCOVER_SEND;
                p_ctrl->local_pdu = PDU_INFO; /*PDU_RX_INFO*/
            }

            break;

        case DISCOVER_ACTIVE:

            /*ReturnToLinkFault(port);*/
            if (p_ctrl->rx_pkt_type != NON_PDU_RX)
            {
                p_ctrl->discovery_status = DISCOVER_SEND;
                p_ctrl->local_pdu = PDU_INFO;
            }

            break;

        case DISCOVER_SEND:
            ReturnToLinkFault(sub_cards, port);

            //if(VerifySatisfied(port)){ 110409
            if ((1 == VerifySatisfied(sub_cards, port))
                && (p_rx->oam_pdu_x.oam_header_x.link_fault == 0)
                && (p_ctrl->local_lost_link_time_down == 0))
            {
                p_ctrl->discovery_status = DISCOVER_ANY;
                p_ctrl->local_pdu = PDU_ANY;
            }

            break;

        case DISCOVER_ANY: /*对于复位命令，则在应答命令区自动复位；配置改为
OAM DISABLE则在配置时注销状态机*/

            //if(VerifySatisfied(port)==0){ 110409
            if ((VerifySatisfied(sub_cards, port) == 0) || (p_rx->oam_pdu_x.oam_header_x.link_fault))
            {
                p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_UNFINISHED;//110409
                p_ctrl->discovery_status = DISCOVER_SEND;
                p_ctrl->local_pdu = PDU_INFO;
            }

            ReturnToLinkFault(sub_cards, port);
            break;

        default:
            break;
    }
}

NBB_VOID ResetOamPort(NBB_BYTE sub_cards, NBB_BYTE port)  /*复位该端口OAM，只保留配置*/
{
    port_oam_pdu                    *p_rx;  /*为可读性好临时用*/
    port_oam_pdu                    *p_tx;
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        //  NBB_PRINTF("This Port Disable Oam\n");
        return;
    }

    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    OS_MEMSET(p_tx, 0, sizeof(port_oam_pdu));     /*全部变量赋值0*/
    OS_MEMSET(p_rx, 0, sizeof(port_oam_pdu));
    OS_MEMSET(p_ctrl, 0, sizeof(oam_control_status));
    /*为该端口发送数据结构初始化，开辟DATA空间，而接收则清零即可 */
    PortOamPduInitCfg(sub_cards, p_tx, port);
    PortOamCtrlInitCfg(p_ctrl, port);   /*初始化oam_cfg_tx oam_pdu_cfg_tx*/
    p_ctrl->need_exe_mac = 1;
    //  NBB_PRINTF("p_tx->err_frm_period_tlv_x.err %d\n",p_tx->err_frm_period_tlv_x.err);
    //  NBB_PRINTF(" Now ResetOamPort PORT %d...........\n",port);
    //  ExeMacRxTxMod(port,PAR_FWD,MUX_FWD);/*101229复位后执行正常转发。即将MAC层初始化为正常收发  * /
    p_ctrl->unfinished_time = 0; /*110516复位将一切复原*/
    p_ctrl->unfinished = FINISHED; /* 清除超时或不能应答的命令*/
    NBB_PRINTF("p_ctrl->unfinished,Then Local OamCtrlParMux %d 0,0\n", port);
    p_ctrl->finished_state = FINISH_NORMAL;/* 等待应答的命令只有环回/查询，而网管的其他命令不需等待是否应答*/
    OamCtrlParMux(sub_cards, port, MUX_FWD, PAR_FWD);  /*110513 100609*/
    return;     /*20080912  22:00 lhm*/
}

#if 0   /*2011.11.27 qjwu*/
NBB_VOID RxOamLostTimer(NBB_BYTE port)/*5秒看门狗触发一次（由1
秒看门狗计时），应调用所有端口*/
{
    port_oam_pdu                    *p_rx;  /*为可读性好临时用*/
    oam_control_status      *p_ctrl;
    //return;//1fcdsf 110111
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_ctrl->rx_pkt_cnt_in_5s == 0)
    {
        p_ctrl->local_lost_link_time_down = 1;
    }
    else
    {
        p_ctrl->local_lost_link_time_down = 0;
        p_ctrl->rx_pkt_cnt_in_5s = 0;
    }
}
#endif

NBB_VOID RxOamLostTimer(NBB_BYTE sub_cards, NBB_BYTE port)/*1秒看门狗触发一次（由1
秒看门狗计时），应调用所有端口*/
{
    port_oam_pdu                *p_rx;  /*为可读性好临时用*/
    oam_control_status      *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (rx_pkt_cnt_in_5s_last[sub_cards][port] != p_ctrl->rx_pkt_cnt_in_5s)
    {
        rx_pkt_cnt_in_5s_timer[sub_cards][port] = 0;
    }
    else
    {
        p_ctrl->rx_pkt_cnt_in_5s = 0;

        if (rx_pkt_cnt_in_5s_timer[sub_cards][port] < 5)
        {
            rx_pkt_cnt_in_5s_timer[sub_cards][port]++;
        }
    }

    rx_pkt_cnt_in_5s_last[sub_cards][port] = p_ctrl->rx_pkt_cnt_in_5s;

    if (rx_pkt_cnt_in_5s_timer[sub_cards][port] >= 5)
    {
        p_ctrl->local_lost_link_time_down = 1;
    }
    else
    {
        p_ctrl->local_lost_link_time_down = 0;
    }
}

NBB_VOID ExeMacRxLoopBack(NBB_BYTE port)   /*对MAC层执行发包环回*/
{
}
NBB_VOID ExeMacRxDiscard(NBB_BYTE port)   /*对MAC层执行收包丢弃*/
{
}
NBB_VOID ExeMacRxFwd(NBB_BYTE port)   /*对MAC层执行收包正常转发*/
{
}

NBB_VOID ExeMacTxDiscard(NBB_BYTE port)   /*对MAC层执行发包丢弃即不使能发*/
{
}

NBB_VOID ExeMacTxFwd(NBB_BYTE port)   /*对MAC层执行发包正常转发*/
{
}

NBB_VOID GetPmMacPhyVar(NBB_BYTE sub_cards, NBB_BYTE port)/*获取性能统计以作事件触发依据
和徐接口*//*放在轮询中调用*/
{
    NBB_ULONG cnt_hi;
    NBB_ULONG cnt_low;
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    port_oam_pdu                    *p_rx;/*为可读性好临时用*/
    oam_control_status      *p_ctrl;
    /*return; DEBUG*/
    cnt_hi = 0;
    //return;/*直接调用性能数据，所以此函数不需要*/
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              T_OkPkts, &cnt_low, &cnt_hi) < 0)
        /*不考虑计数翻转*/
    {
        NBB_PRINTF("Get T_Pkts FAIL\n");
    }

    p_ctrl->local_var_cnt[TX_FRM] = cnt_low;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_OkPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get R_Pkts FAIL\n");
    }

    p_ctrl->local_var_cnt[RX_FRM] = cnt_low;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_CrcPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get R_CrcPkts FAIL\n");
    }

    p_ctrl->local_var_cnt[RX_CRC] = cnt_low;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              T_Octets, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get T_Octets FAIL\n");
    }

    p_ctrl->local_var_cnt[TX_BYTE] = cnt_low;
    cnt_low = 0;
    cnt_hi = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_Octets, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get R_Octets FAIL\n");
    }

    p_ctrl->local_var_cnt[RX_BYTE] = cnt_low;
}

#if 0
NBB_VOID GetPmMacPhySymErrDbg(NBB_BYTE port)/*获取性能统计以作事件触发依据  和徐接口
每秒调一次*/
{
    NBB_INT cnt_hi;
    NBB_INT cnt_low;
    port_oam_pdu                    *p_tx;/*为可读性好临时用*/
    port_oam_pdu                    *p_rx;/*为可读性好临时用*/
    oam_control_status      *p_ctrl;
    cnt_hi = 0;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->sym_err_1s = 0;
    cnt_low = 0;
    cnt_hi = 0; /* R_SymbolError  错暂时获取不到！！ */

    if (ApiC3GetHWStatCounter(0, port, RxSymbErrFram, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get RxSymbErrFram FAIL\n");
    }
    else
    {
        if (p_ctrl->sym_err_last < cnt_low)
        {
            p_ctrl->sym_err_1s += (cnt_low - p_ctrl->sym_err_last);
            /*寄存器累计值增加了，则获取增量*/
        }

        p_ctrl->sym_err_last = cnt_low;
        /*将本次累计值存入为上次值，以备下次应用，在上次值大于当前值，则表明寄存器翻转对这样的数据暂不予记录，这可接受*/
    }

    if (p_ctrl->need_exe_mac)
    {
        p_ctrl->sym_err_1s = 0;
    }

    p_tx->err_sym_period_tlv_x.err += p_ctrl->sym_err_1s;
}

/*获取性能统计以作事件触发依据  和徐接口 每秒调一次*/
NBB_VOID GetPmMacPhySymErr(NBB_BYTE port)
{
    NBB_ULONG            cnt_hi;
    NBB_ULONG            cnt_low;
    port_oam_pdu            *p_tx;/*为可读性好临时用*/
    port_oam_pdu            *p_rx;/*为可读性好临时用*/
    oam_control_status      *p_ctrl;
    cnt_hi = 0;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->sym_err_1s = 0;
    cnt_low = 0;
    cnt_hi = 0; /* R_SymbolError  错暂时获取不到！！ */

    if (ApiC3GetHWStatCounter(0, port, RxSymbErrFram, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get RxSymbErrFram FAIL\n");
    }
    else
    {
        if (p_ctrl->sym_err_last < cnt_low)
        {
            p_ctrl->sym_err_1s += (cnt_low - p_ctrl->sym_err_last);
            /*寄存器累计值增加了，则获取增量*/
        }

        p_ctrl->sym_err_last = cnt_low;
        /*将本次累计值存入为上次值，以备下次应用，在上次值大于当前值，
        则表明寄存器翻转Ｔ这样的数据暂不予记录，这可接受*/
    }

    if (p_ctrl->need_exe_mac)
    {
        p_ctrl->sym_err_1s = 0;
    }

    p_tx->err_sym_period_tlv_x.err += p_ctrl->sym_err_1s;
}
#endif
NBB_VOID GetPmMacPhyFrmErr(NBB_BYTE sub_cards, NBB_BYTE port)/*获取性能统计以作事件触发依据  和徐接口
每秒调一次*/
{
    NBB_ULONG            cnt_hi;
    NBB_ULONG            cnt_low;
    NBB_ULONG            cnt_low_tmp;
    port_oam_pdu            *p_tx;/*为可读性好临时用*/
    port_oam_pdu            *p_rx;/*为可读性好临时用*/
    oam_control_status      *p_ctrl;
    cnt_hi = 0;
    cnt_low_tmp = 0;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->frm_err_1s = 0;
    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_UndersizePkts, &cnt_low, &cnt_hi) < 0)
        /*计数翻转时该次的访问数据无效，翻转后一切继续*/
    {
        NBB_PRINTF("Get R_UndersizePkts FAIL\n");
    }
    else
    {
        if (p_ctrl->undersize_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->undersize_last);
        }

        p_ctrl->undersize_last = cnt_low;
    }

    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_FragmentPkts, &cnt_low, &cnt_hi) < 0)
        /*计数翻转时该次的访问数据无效，翻转后一切继续*/
    {
        NBB_PRINTF("Get R_FragmentPkts FAIL\n");
    }
    else
    {
        if (p_ctrl->fragment_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->fragment_last);
        }

        p_ctrl->fragment_last = cnt_low;
    }

    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_OversizePkts, &cnt_low, &cnt_hi) < 0)
        /*计数翻转时该次的访问数据无效，翻转后一切继续*/
    {
        NBB_PRINTF("Get R_OversizePkts FAIL\n");
    }
    else
    {
        if (p_ctrl->oversize_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->oversize_last);
        }

        p_ctrl->oversize_last = cnt_low;
    }

    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_JabberPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get RxJabberFram FAIL\n");
    }
    else
    {
        if (p_ctrl->jabber_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->jabber_last);
        }

        p_ctrl->jabber_last = cnt_low;
    }

    cnt_low = 0;

    if (ApiC3GetHWStatCounter(0, g_efm_oam_port_cfg[sub_cards].logic_port[port],
                              R_CrcPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get R_CrcPkts FAIL\n");
    }
    else
    {
        if (p_ctrl->crc_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->crc_last);
        }

        p_ctrl->crc_last = cnt_low;
    }

    cnt_low = 0;
#if 0

    if (ApiC3GetHWStatCounter(0, port, R_AlignmentPkts, &cnt_low, &cnt_hi) < 0)
    {
        NBB_PRINTF("Get RxShortErrFram FAIL\n");
    }
    else
    {
        if (p_ctrl->alignment_last < cnt_low)
        {
            p_ctrl->frm_err_1s += (cnt_low - p_ctrl->alignment_last);
        }

        p_ctrl->alignment_last = cnt_low;
    }

#endif

    if (p_ctrl->need_exe_mac)
    {
        p_ctrl->frm_err_1s = 0;
    }

    p_tx->err_frm_tlv_x.err += p_ctrl->frm_err_1s;
    p_tx->err_frm_period_tlv_x.err += p_ctrl->frm_err_1s;

    if (p_ctrl->frm_err_1s)
    {
        NBB_PRINTF("port %d :frm_err_1s=%d\n", port, p_ctrl->frm_err_1s);
        p_tx->err_frm_second_tlv_x.err++;
    }
}


NBB_BYTE GetAisMacPhy(NBB_BYTE sub_card, NBB_BYTE port)/*/获取告警以作FLAG事件，更新包头的 flags
和徐接口*/
{
    int status;
    status = 0x33;
    /*if(ApiGetPhyLink(0,g_efm_oam_port_cfg[sub_cards].logic_port[port],&status))
    {
        NBB_PRINTF("FAIL to GET  LINK STATUS port: %d,  rv = %d\n", port, rv);
        return 0;
    }
    else
    {
        if(status)
            return(0);
        else
            return(1);
    }*//*2011.10.18 qjwu ApiGetPhyLink()使用了互斥信号量*/

    if (phy_link_state[sub_card][port])
    {
        return 0;
    }
    else
    {
        return 1;
    }
}
NBB_VOID UpdateFlag(NBB_BYTE sub_cards, NBB_BYTE port)/* 探测是否有链路端口事故,1秒调1次，在
DiscoveryState 之前运行*/
{
    port_oam_pdu                    *p_tx;/*/为可读性好临时用*/
    oam_control_status              *p_ctrl;
    port_oam_pdu                    *p_rx;//110409
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;

    //if(p_ctrl->enb_link_fault_send||(p_rx->oam_pdu_x.oam_header_x.link_fault))/*110409 实现单向收到对端LINK-FAULT时本地报LINK-FAULT*/

    if (p_ctrl->enb_link_fault_send)
    {
        p_tx->oam_pdu_x.oam_header_x.link_fault = 1;
    }
    else
    {
        p_tx->oam_pdu_x.oam_header_x.link_fault = GetAisMacPhy(sub_cards, port);
    }

    if (p_ctrl->local_link_status_last == p_tx->oam_pdu_x.oam_header_x.link_fault)
    {
        return;
    }

    p_ctrl->local_link_status_last = p_tx->oam_pdu_x.oam_header_x.link_fault;
    /* 虽然 revision 是INFO里的内容，但有LINK故障时将其更新也是合理的*/
    p_tx->inf_tlv_local_x.revision++;
}

NBB_VOID UpdateOamCfgPduCfg(NBB_BYTE sub_cards, NBB_BYTE port)  /*每下一次单盘配置就执行一次，以更新数据，不管是否有更改*/
{
    port_oam_pdu            *p_tx;/* 为可读性好临时用*/
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;/* 赋值 oam_cfg_tx  oam_pdu_cfg_tx，并相应更新状态区发送区*/
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_tx->inf_tlv_local_x.revision++;
}

NBB_VOID DecideSymEvent(NBB_BYTE sub_cards, NBB_BYTE port)  /* 每秒调1次，对网管不做窗口设置响应*/
{
    port_oam_pdu            *p_tx;
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_tx->err_sym_period_tlv_x.err > p_tx->err_sym_period_tlv_x.threshold)
    {
        /* 目前只取低4字节*/
        p_ctrl->sym_period_req = 1;
        p_ctrl->tx_sym_err = 1;
        p_ctrl->tx_sym_err_cnt = p_tx->err_sym_period_tlv_x.err;
        p_tx->err_sym_period_tlv_x.err_ttl += p_tx->err_sym_period_tlv_x.err;
        /*只有越界的错误才会累积到总数中*/
        p_tx->err_sym_period_tlv_x.event_ttl++;

        if (p_ctrl->local_pdu != PDU_ANY)
        {
            p_tx->err_sym_period_tlv_x.err = 0;
        }
    }
    else
    {
        p_tx->err_sym_period_tlv_x.err = 0;
    }

    /*不管本次错误是否要记录到总值中，应清零为下次判决做准备*/
}

NBB_VOID DecideFrmEvent(NBB_BYTE sub_cards, NBB_BYTE port)  /* 每秒调1次，对网管不做窗口设置响应*/
{
    port_oam_pdu            *p_tx;
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_tx->err_frm_tlv_x.err > p_tx->err_frm_tlv_x.threshold)  /* 目前只取低4字节*/
    {
        p_ctrl->frm_err_req = 1;
        p_ctrl->tx_frm_err = 1;
        p_ctrl->tx_frm_err_cnt = p_tx->err_frm_tlv_x.err;
        p_tx->err_frm_tlv_x.err_ttl += p_tx->err_frm_tlv_x.err;
        p_tx->err_frm_tlv_x.event_ttl++;    /* 只需直接更新发送区*/

        if (p_ctrl->local_pdu != PDU_ANY)
        {
            p_tx->err_frm_tlv_x.err = 0;
        }
    }
    else
    {
        p_tx->err_frm_tlv_x.err = 0;
    }
}

NBB_VOID DecideFrmPeriodEvent(NBB_BYTE sub_cards, NBB_BYTE port)  /* 每秒调1次，对网管不做窗口设置响应*/
{
    port_oam_pdu            *p_tx;/* 为可读性好临时用*/
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_tx->err_frm_period_tlv_x.err > p_tx->err_frm_period_tlv_x.threshold)
        /* 目前只取低4字节*/
    {
        p_ctrl->frm_period_req = 1;
        p_ctrl->tx_frm_prd_err = 1;
        p_ctrl->tx_frm_prd_err_cnt = p_tx->err_frm_period_tlv_x.err;
        p_tx->err_frm_period_tlv_x.err_ttl += p_tx->err_frm_period_tlv_x.err;
        p_tx->err_frm_period_tlv_x.event_ttl++;

        if (p_ctrl->local_pdu != PDU_ANY)
        {
            p_tx->err_frm_period_tlv_x.err = 0;
        }
    }
    else
    {
        p_tx->err_frm_period_tlv_x.err = 0;
    }
}

NBB_VOID DecideFrmSecondEvent(NBB_BYTE sub_cards, NBB_BYTE port)  /* 每60秒调1次，对网管不做窗口设置响应*/
{
    port_oam_pdu            *p_tx;/* 为可读性好临时用*/
    oam_control_status      *p_ctrl;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (p_tx->err_frm_second_tlv_x.err > p_tx->err_frm_second_tlv_x.threshold)
        /* 目前只取低4字节*/
    {
        p_ctrl->second_period_req = 1;
        p_ctrl->tx_frm_snd_err = 1;
        p_ctrl->tx_frm_snd_err_cnt = p_tx->err_frm_second_tlv_x.err;
        p_tx->err_frm_second_tlv_x.err_ttl += p_tx->err_frm_second_tlv_x.err;
        p_tx->err_frm_second_tlv_x.event_ttl++;

        if (p_ctrl->local_pdu != PDU_ANY)
        {
            p_tx->err_frm_second_tlv_x.err = 0;
        }
    }
    else
    {
        p_tx->err_frm_second_tlv_x.err = 0;
    }
}

NBB_BYTE VerifySatisfied(NBB_BYTE sub_cards, NBB_BYTE port) /*调用此函数则已收到对端包，因此不存在协商不可能完成态*/
{
    /*  毕竟 oam_cfg_tx/oam_cfg_rx  oam_pdu_cfg_tx/oam_pdu_cfg_rx 是否一致
    是否都为 PASSIVE*/
    /* 更新 发送区 oam_flag  并更新状态区*/
    port_oam_pdu                    *p_rx;  /* 为可读性好临时用*/
    port_oam_pdu                    *p_tx;
    oam_control_status      *p_ctrl;
    p_rx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_rx;
    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    /*if(p_tx->inf_tlv_local_x.var_response_spt==0) goto LinkFail;
    if(p_tx->inf_tlv_local_x.link_event_spt==0) goto LinkFail;100609*/
    /*if(p_tx->inf_tlv_local_x.loopback_spt==0) goto LinkFail;
    *//*环回不看做必须，以满足测试关环回应答*/
    //if(p_tx->inf_tlv_local_x.unidirection_spt==0) goto LinkFail;
    /*if(p_rx->inf_tlv_local_x.var_response_spt==0) goto LinkFail;
    if(p_rx->inf_tlv_local_x.link_event_spt==0) goto LinkFail;100609*/
    /*if(p_rx->inf_tlv_local_x.loopback_spt==0) goto LinkFail;*/
    /*if(p_rx->inf_tlv_local_x.unidirection_spt==0) goto LinkFail;100609*/
    p_ctrl->max_size_recept = (p_tx->inf_tlv_local_x.max_size < p_rx->inf_tlv_local_x.max_size)
                              ?  p_tx->inf_tlv_local_x.max_size : p_rx->inf_tlv_local_x.max_size;
LinkSuccess:
    p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_FINISHED;
    p_tx->oam_pdu_x.oam_header_x.local_stable = STABLE_SATISFIED;
    return(1);
LinkFail:/* 只对Local Discovering/Local Stable 即BIT3/BIT4操作远端的在接收时已处理*/
    p_tx->oam_pdu_x.oam_header_x.local_discovering = DISCOVER_FINISHED;
    p_tx->oam_pdu_x.oam_header_x.local_stable = STABLE_FAIL;
    return(0);
}

#if 0
/*****************************************************************************
功能： 在每次配置所有端口前初始化某些数据；应在配置端口前调用
作者： hmliu
时间： 20080904
输入： 无
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID InitBeforeCfgPort(NBB_VOID)
{
    SetSrcMac(0);
}
#endif

/*****************************************************************************
功能： 配置某端口是否使能OAM
，如使能则开辟空间并初始化参数，不使能则注销空间；应在 CONFIG中调用，
作者： hmliu
时间： 20080904
输入： port：端口；enb：使能
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamEnb(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    g_efm_oam_port_cfg[sub_cards].port_oam_x[port].enb = enb;

    if (enb)
    {
    }
    else
    {
    }
}

/*****************************************************************************
功能： 配置OAM模式；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；enb：是否主动
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamMode(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        p->inf_tlv_local_x.oam_mode = enb;
        p_ctrl->discovery_status = DISCOVER_LINK_FAULT;
        p_ctrl->local_pdu = PDU_LF_INFO;
        p_ctrl->rx_pkt_type = NON_PDU_RX;
        DiscoveryState(sub_cards, port);
        //  printf("p_ctrl->discovery_status=%d\n",p_ctrl->discovery_status);
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_mode_cfg[port] = enb;
}

/*****************************************************************************
功能： 配置OAM是否支持变量查询；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；enb：是否支持
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamSupportVar(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.var_response_spt = enb;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sup_var_cfg[port] = enb;
}


/*****************************************************************************
功能： 配置OAM是否支持事件帧收发；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；enb：是否支持
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamSupportEvent(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.link_event_spt = enb;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sup_event_cfg[port] = enb;
}

/*****************************************************************************
功能： 配置OAM是否支持环回；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；enb：是否支持
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamSupportLb(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.loopback_spt = enb;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sup_lb_cfg[port] = enb;
}

/*****************************************************************************
功能： 配置OAM是否支持单向；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；enb：是否支持
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamSupportUnidirection(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.unidirection_spt = enb;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sup_uni_cfg[port] = enb;

    if (enb)
    {
        ApiC3SetPortAutoNego(0, port, 0);
        taskDelay(10);

        if (GetAisMacPhy(sub_cards, port))   /*万兆不支持非自协商即不支持单向*/
        {
            ApiC3SetPortAutoNego(0, port, 1);
        }
    }
    else
    {
        ApiC3SetPortAutoNego(0, port, 1);
    }
}

/*****************************************************************************
功能： 配置OAM最大帧长；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；max:最大帧长
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamMaxSize(NBB_BYTE sub_cards, NBB_BYTE port, unsigned short max)
{
    port_oam_pdu                    *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->inf_tlv_local_x.max_size = max;
        DiscoveryState(sub_cards, port);
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_maxsize_cfg[port] = max;
}

/*****************************************************************************
功能： 配置OAM应答超时门限；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；max:超是门限
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamOvertimeThreshold(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE max)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
        p_ctrl->overtime_threshold = max;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_overtime_cfg[port] = max;

    if (OvertimeThreshold < max)
    {
        OvertimeThreshold = max;
    }
}

/*****************************************************************************
功能： 配置OAM错误符号事件周期；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；prd：周期
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamSymPrd(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE prd)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_sym_period_tlv_x.window = 0x1DCD6500 * prd;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sym_period_cfg[port] = prd;
}

/*****************************************************************************
功能： 配置OAM错误符号事件门限；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；thr：门限
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamSymThr(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE thr)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_sym_period_tlv_x.threshold = thr;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_sym_threshold_cfg[port] = thr;
}


/*****************************************************************************
功能： 配置OAM错误帧事件周期；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；prd：周期
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamFrmPrd(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE prd)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_tlv_x.window = prd;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_period_cfg[port] = prd;
}

/*****************************************************************************
功能： 配置OAM错误帧事件门限；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；thr：门限
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamFrmThr(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE thr)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_tlv_x.threshold = thr;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_threshold_cfg[port] = thr;
}

/*****************************************************************************
功能： 配置OAM错误帧周期事件周期；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；prd：周期
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamFrmPrdPrd(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE prd)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_period_tlv_x.window = 1488000 * prd;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_prd_period_cfg[port] = prd;
}

/*****************************************************************************
功能： 配置OAM错误帧周期事件门限；应在 CONFIG中解调用
作者： hmliu
时间： 20080904
输入： port：端口；thr：门限
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamFrmPrdThr(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE thr)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_period_tlv_x.threshold = thr;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_prd_threshold_cfg[port] = thr;
}

/****************************************************************************
功能： 配置OAM错误帧秒事件周期；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；prd：周期
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamFrmSndPrd(NBB_BYTE sub_cards, NBB_BYTE port, unsigned short prd)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_second_tlv_x.window = prd;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_snd_period_cfg[port] = prd;
}

/*****************************************************************************
功能： 配置OAM错误帧秒事件门限；应在 CONFIG中调用
作者： hmliu
时间： 20080904
输入： port：端口；thr：门限
输出： 无
说明：
修改：
******************************************************************************/

NBB_VOID CfgPortOamFrmSndThr(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE thr)
{
    port_oam_pdu   *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port])
    {
        p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
        p->err_frm_second_tlv_x.threshold = thr;
    }

    g_efm_oam_port_cfg[sub_cards].port_oam_frm_snd_threshold_cfg[port] = thr;
}

/*****************************************************************************
功能： 执行是否需开启或注销OAM收发任务，在每次新配置完所有端口后调用。
作者： hmliu
时间： 20080904
输入： 无
输出： 无
说明： 用户应注意在配置完所有端口后应调用此函数！
修改：
******************************************************************************/
NBB_VOID BeginOamTxRxTask(NBB_VOID)
{
    AllPortOamCfgEnb();
}


/*****************************************************************************
功能： 执行状态监视查询命令用户接口，在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口
输出： 指向CHAR 数组的指针,或者是 NULL指针，则所有值都返回为0
说明： 用户可用此指针直接拷数据到应答网管命令的发送对列，拷贝长度则由网管协议定
             用户如用不同的BCT
协议则暂时需更改底层代码，这样设计是为了简洁，并考虑用户用新的BCT协议并无意义
修改：
******************************************************************************/
NBB_BYTE *QueryPortOam(NBB_BYTE sub_cards, NBB_BYTE port)
{
    return(QueryPortOamApi(sub_cards, port));
}

/*****************************************************************************
功能： 响应发起符号错事件命令，使得本端发起事件帧；在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口
输出： 无
说明： 此函数并不返回参数，命令效果在状态监视查询里才可看到
修改：
******************************************************************************/

NBB_VOID CmdSymErrEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_SYM;
}

/*****************************************************************************
功能： 响应发起帧错事件命令，使得本端发起事件帧；在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口
输出： 无
说明： 此函数并不返回参数，命令效果在状态监视查询里才可看到
修改：
******************************************************************************/

NBB_VOID CmdFrmErrEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_FRM;
}

/*****************************************************************************
功能： 响应发起帧周期错事件命令，使得本端发起事件帧；在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口
输出： 无
说明： 此函数并不返回参数，命令效果在状态监视查询里才可看到
修改：
******************************************************************************/

NBB_VOID CmdFrmPrdErrEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_FRM_PERIOD;
}

/*****************************************************************************
功能： 响应发起帧秒错事件命令，使得本端发起事件帧；在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口
输出： 无
说明： 此函数并不返回参数，命令效果在状态监视查询里才可看到
修改：
******************************************************************************/

NBB_VOID CmdFrmSndErrEvent(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_FRM_SECOND;
}


/*****************************************************************************
功能： 执行OAM复位，在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口
输出： 无
说明： 复位后该OAM端口保存的性能和状态都初始化
修改：
******************************************************************************/
NBB_VOID CmdResetOam(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_RST_OAM;

    if (p_ctrl->local_pdu != PDU_ANY)
    {
        ResetOamPort(sub_cards, port);
        p_ctrl->rx_mgr_cmd = 0;
    }
}

/*****************************************************************************
功能： 执行OAM环回，在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口；cmd：开/关
输出： 无
说明： 此函数并不返回参数，命令效果在状态监视查询里才可看到
修改：
******************************************************************************/
NBB_VOID CmdSetOamLb(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE cmd)
{
    oam_control_status      *p_ctrl;
    port_oam_pdu            *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;

    if (p->inf_tlv_local_x.loopback_spt == 0)
    {
        return;
    }

    if (p->inf_tlv_local_x.oam_mode == OAM_PASSIVE)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;

    if (cmd)
    {
        p_ctrl->rx_mgr_cmd = MGR_CMD_LB;
    }
    else
    {
        p_ctrl->rx_mgr_cmd = MGR_CMD_DIS_LB;
    }
}

/*****************************************************************************
功能： 执行MIB变量查询，在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口；branch：枝 leaf：叶
输出： 无
说明： 此函数并不返回参数，命令效果在状态监视查询里才可看到
修改：
******************************************************************************/
NBB_VOID CmdQueryMibVar(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE branch, NBB_BYTE leaf)
{
    oam_control_status      *p_ctrl;
    port_oam_pdu            *p;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p->var_req_x[0].branch = branch;
    p->var_req_x[0].leaf = leaf;
    p->var_num = 1;
    p_ctrl->rx_mgr_cmd = MGR_CMD_REQ;
}

/*****************************************************************************
功能： 执行发起自定义查询端口OAM收发统计命令，在每次下查询命令时调用。
作者： hmliu
时间： 20080904
输入： port：端口
输出： 无
说明： 此函数并不返回参数，命令效果在状态监视查询里才可看到
修改：
******************************************************************************/
NBB_VOID CmdOrgSelfDefine(NBB_BYTE sub_cards, NBB_BYTE port)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->rx_mgr_cmd = MGR_CMD_ORG;
}

NBB_VOID PrintfInfo(NBB_BYTE enb)
{
    print_info = enb;
}

NBB_VOID CmdSetLinkFault(NBB_BYTE sub_cards, NBB_BYTE port, NBB_BYTE enb)
{
    oam_control_status      *p_ctrl;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->enb_link_fault_send = enb;
}

NBB_VOID GetMacPmMibVar(NBB_BYTE sub_cards, NBB_BYTE port, int tx_frm, int rx_frm, int rx_crc, int tx_byte,
                        int rx_byte)
{
    oam_control_status      *p_ctrl;/* 为可读性好临时用 */
    port_oam_pdu            *p_tx;

    if (g_efm_oam_port_cfg[sub_cards].port_oam_enb_cfg_last[port] == 0)
    {
        return;
    }

    p_tx = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_tx;
    p_ctrl = g_efm_oam_port_cfg[sub_cards].port_oam_x[port].p_ctrl;
    p_ctrl->local_var_cnt[TX_FRM] = tx_frm;
    p_ctrl->local_var_cnt[RX_FRM] = rx_frm;
    p_ctrl->local_var_cnt[RX_CRC] = rx_crc;
    p_ctrl->local_var_cnt[TX_BYTE] = tx_byte;
    p_ctrl->local_var_cnt[RX_BYTE] = rx_byte;
}
#endif
#else
NBB_VOID spm_set_efm_oam(EFM_PHY_PORT *p_efm_phy_port, ATG_DCI_PHY_PORT_EFM_OAM_DATA *p_efm_oam)
{
    return;
}
#endif

#endif
