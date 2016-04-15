#if 0
#include <nbase.h>
#include <spmincl.h>
#ifdef SRC
NBB_VOID StartTimeby100ms(NBB_VOID)
{
    return;
}
NBB_VOID StartTxBy1S(NBB_INT unit)
{
    return;
}
NBB_VOID TxLinkOamTask(NBB_INT unit)
{
    return;
}
#endif

#ifdef SPU
NBB_VOID StartTimeby100ms(NBB_VOID)
{
    time_100ms++;
}
/*更新状态周期为1S，此处将部分发包程序放入，不正确*/
NBB_VOID StartTxBy1S(NBB_INT unit) /*unit暂时取为0*/
{
    NBB_BYTE port;
    port_oam_pdu *p_tx;
    oam_control_status *p_ctrl;
    port_oam_pdu *p_rx;
    NBB_BYTE j;
    Time_by_100ms++; /*110516在没到周期时间则直接退出，以支持周期按100毫秒可配*/
    if (Time_by_100ms < Period_100ms)
    {
        return;
    }
    else
    {
        Time_by_100ms = 0;
    }

    //time_1s++;
    OneSecondFlag = 1;
    for (j = 0 ; j < SUB_NUM; j++)
    {
        for (port = 0; port < MAX_PORT_NUM; port++)
        {
            //if (API_PORT_VALID(0, port)) /*验证端口是否在却只范围内R845为0-64*/
            {
                if ((g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[port] == 0)
                    || (g_efm_oam_port_cfg[j].port_oam_x[port].enb == 0))
                    /*OAM不使能 */
                {
                    continue;
                }

                DiscoveryState(j, port); /*OAM Discovery state*/
                p_tx = g_efm_oam_port_cfg[j].port_oam_x[port].p_tx;
                p_rx = g_efm_oam_port_cfg[j].port_oam_x[port].p_rx;
                p_ctrl = g_efm_oam_port_cfg[j].port_oam_x[port].p_ctrl;
                p_ctrl->minute_second++;

                if (p_ctrl->minute_second
                    % g_efm_oam_port_cfg[j].port_oam_sym_period_cfg[port] == 0)
                {
                    DecideSymEvent(j, port); /*判断是否有错误事件发生需发送*/
                }

                if (p_ctrl->minute_second
                    % g_efm_oam_port_cfg[j].port_oam_frm_period_cfg[port] == 0)
                {
                    DecideFrmEvent(j, port);
                }

                if (p_ctrl->minute_second
                    % g_efm_oam_port_cfg[j].port_oam_frm_prd_period_cfg[port] == 0)
                {
                    DecideFrmPeriodEvent(j, port);
                }

                if (p_ctrl->minute_second
                    % g_efm_oam_port_cfg[j].port_oam_frm_snd_period_cfg[port] == 0)
                {
                    DecideFrmSecondEvent(j, port);
                }

                if (p_ctrl->local_pdu == PDU_RX_INFO)
                {
                    p_ctrl->pdu_req = NO_REQUEST;
                    continue;
                }

                p_ctrl->pdu_req = _REQUEST; /*以下都标识有发送请求*/
                p_ctrl->local_sq++; /*更新发送区本端发送的SQ*/
                p_tx->org_inf_tlv_x.local_sq = p_ctrl->local_sq;
                p_tx->org_event_tlv_x.local_sq = p_ctrl->local_sq;
                p_tx->org_inf_tlv_x.remote_sq = p_ctrl->remote_sq;
                /*更新发送区最近收到的远端发送SQ*/
                p_tx->org_event_tlv_x.remote_sq = p_ctrl->remote_sq;

                if (p_ctrl->local_pdu == PDU_LF_INFO)
                    /* 初始化为 OAM LINK FAULT，只发不带TLV的INFO包*/
                {
                    SendInfNoTlv(j, port);
                    continue;
                }

                if (p_ctrl->local_pdu == PDU_INFO)
                    /* 只发INFO包包括带对端和不带对端信息的包*/
                {
                    if (p_ctrl->discovery_status == DISCOVER_ACTIVE) /*激活态*/
                    {
                        SendInfLocalTlv(j, port);
                        continue;
                    }
                    else /*SEND态*/
                    {
                        SendInfLocalRemoteTlv(j, port);
                        continue;
                    }
                }

                if (p_ctrl->local_pdu == PDU_ANY) /*任意OAM包都可发*/
                {
                    if (p_ctrl->unfinished != FINISHED)
                        /*已发送各种报文请求信息，等待响应*/
                    {
                        switch (p_ctrl->unfinished)
                            /* 0/1/2/3/4 对应：没有未完成的/LB_CMD/LB_EXE/VAR_REQ/VAR_RES 未完成*/
                        {
                            case UNF_LB_CMD: /*等待环回命令回应*/
                                if ((p_ctrl->rx_pkt_type == RX_INF_LOCAL_REMOTE)\
                                    && (p_rx->inf_tlv_local_x.par_action == PAR_LB)\
                                    && (p_rx->inf_tlv_local_x.mux_action == MUX_DISCARD))
                                    /*收到OK应答帧*/
                                {
                                    OamCtrlParMux(j, port, MUX_FWD, PAR_DISCARD);
                                    /*还要直接改发送帧内容  */
                                    p_ctrl->unfinished = FINISHED;
                                    p_ctrl->finished_state = FINISH_LB_LOCAL_CMD;
                                    /*完成了 网管环回命令执行 */
                                }

                                SendInfLocalRemoteTlv(j, port);
                                /*REMOTE收到的信息已复制，直接发送返回即可，要改的是本端信息    */
                                break;

                            case UNF_DIS_LB_CMD: /*等待去环回命令回应*/
                                if ((p_ctrl->rx_pkt_type == RX_INF_LOCAL_REMOTE)\
                                    && (p_rx->inf_tlv_local_x.par_action == MUX_FWD) \
                                    && (p_rx->inf_tlv_local_x.mux_action == PAR_FWD))
                                    /*收到对端发包被告知已清返回*/
                                {
                                    OamCtrlParMux(j, port, MUX_FWD, PAR_FWD);
                                    //NBB_PRINTF("Send Remote Disable LB CMD,Then Local OamCtrlParMux %d 0,0\n", port);
                                    p_ctrl->unfinished = FINISHED;
                                    p_ctrl->finished_state = FINISH_NORMAL;
                                    /*完成了 网管环回命令执行 */
                                }

                                SendInfLocalRemoteTlv(j, port);
                                break;

                            case UNF_LB_EXE:

                                /*收到对端环回命令，本端正要应答，但应判断本端此时是否有网管环回命令*/
                                if (p_ctrl->rx_mgr_cmd == MGR_CMD_LB)
                                    /*有网管环回命令*/
                                {
                                    if (NBB_MEMCMP(p_ctrl->src_mac_remote, src_mac_overall, 6) < 0)
                                        /*即对端MAC比本端MAC小，即高为MASTER*/
                                    {
                                        OamCtrlParMux(j, port, MUX_DISCARD, PAR_LB);
                                        /*应答对方命令  */
                                        p_ctrl->unfinished = FINISHED;
                                        p_ctrl->finished_state =
                                            FINISH_LB_REMOTE_EXE;
                                        SendInfLocalRemoteTlv(j, port);
                                    }
                                    else /*本端主动发命令，不响应对端的LB命令*/
                                    {
                                        OamCtrlParMux(j, port, MUX_DISCARD,
                                                      PAR_DISCARD);    /*本端发命令*/
                                        p_ctrl->unfinished = UNF_LB_CMD;
                                        p_ctrl->finished_state = FINISH_FALSE;
                                        SendLbCmd(j, port);
                                    }
                                }
                                else /*无网管环回命令*/
                                {
                                    OamCtrlParMux(j, port, MUX_DISCARD, PAR_LB);
                                    /*应答对方命令*/
                                    p_ctrl->unfinished = FINISHED;
                                    p_ctrl->finished_state = FINISH_LB_REMOTE_EXE;
                                    SendInfLocalRemoteTlv(j, port);
                                }

                                break;

                            case UNF_DIS_LB_EXE:
                                /*收到对端发清环回命令，本端应答*/
                                OamCtrlParMux(j, port, MUX_FWD, PAR_FWD);
                                p_ctrl->unfinished = FINISHED;
                                p_ctrl->finished_state = FINISH_NORMAL;
                                /*完成了对端发的清除环回命令*/
                                SendInfLocalRemoteTlv(j, port);
                                break;

                            case UNF_VAR_REQ:

                                /*本端发出了查询命令，正等应答  */
                                if (p_ctrl->rx_pkt_type == RX_INF_LOCAL_REMOTE)
                                {
                                    p_ctrl->unfinished = FINISHED;
                                    p_ctrl->finished_state = FINISH_NORMAL;
                                }

                                SendInfLocalRemoteTlv(j, port);
                                break;

                            case UNF_VAR_RES:
                                p_ctrl->unfinished = FINISHED;
                                p_ctrl->finished_state = FINISH_NORMAL;
                                SendVarRes(j, port);
                                break;

                            default:
                                break;
                        }
                    }
                    else /*根据各种命令，发送对应报文请求信息*/
                    {
                        switch (p_ctrl->rx_mgr_cmd)
                        {
                            case MGR_CMD_FALSE: /*无网管命令*/
                                if (p_ctrl->sym_period_req || p_ctrl->frm_err_req \
                                    || p_ctrl->frm_period_req || p_ctrl->second_period_req)
                                {
                                    SendEvent(j, port);
                                }
                                else
                                {
                                    SendInfLocalRemoteTlv(j, port);
                                }

                                break;

                            case MGR_CMD_LB:
                                if (p_tx->inf_tlv_local_x.oam_mode == OAM_ACTIVE)
                                {
                                    if (p_ctrl->finished_state == FINISH_NORMAL)
                                    {
                                        OamCtrlParMux(j, port, MUX_DISCARD, PAR_DISCARD);
                                        /*本端发命令，并置位MAC层丢弃*/
                                        p_ctrl->unfinished = UNF_LB_CMD;
                                        p_ctrl->finished_state = FINISH_FALSE;
                                        SendLbCmd(j, port);
                                    }
                                    else if (p_ctrl->finished_state ==
                                             FINISH_LB_REMOTE_EXE)
                                    {
                                        /* 20080830 src_mac_overall 一个盘一个MAC*/
                                        /*即本端MAC为MASTER*/
                                        if (NBB_MEMCMP(p_ctrl->src_mac_remote,
                                                       p_tx->oam_pdu_x.oam_header_x.src_mac, 6) > 0)
                                        {
                                            OamCtrlParMux(j, port, MUX_DISCARD, PAR_DISCARD);
                                            /*本端发命令，并置位MAC层丢弃*/
                                            p_ctrl->unfinished = UNF_LB_CMD;
                                            p_ctrl->finished_state = FINISH_FALSE;
                                            SendLbCmd(j, port);
                                        }
                                        else
                                        {
                                            SendInfLocalRemoteTlv(j, port);
                                        }
                                    }
                                    else
                                    {
                                        SendInfLocalRemoteTlv(j, port);
                                    }
                                }
                                else
                                {
                                    SendInfLocalRemoteTlv(j, port);
                                }

                                break;

                            case MGR_CMD_DIS_LB:
                                if (p_tx->inf_tlv_local_x.oam_mode == OAM_ACTIVE)
                                {
                                    if (p_ctrl->finished_state == FINISH_LB_LOCAL_CMD)
                                    {
                                        OamCtrlParMux(j, port, MUX_DISCARD, PAR_DISCARD);
                                        /*本端发命令,先不更改本端MAC层转发状态*/
                                        p_ctrl->unfinished = UNF_DIS_LB_CMD;
                                        p_ctrl->finished_state = FINISH_FALSE;
                                        SendDisLbCmd(j, port);
                                    }
                                    else/*本端没发过环回命令，所以对清环回命令不予处理*/
                                    {
                                        SendInfLocalRemoteTlv(j, port);
                                    }
                                }

                                break;

                            case MGR_CMD_REQ:
                                /*不管是什么态，都支持查询，即使是环回态*/
                                p_ctrl->unfinished = UNF_VAR_REQ;
                                p_ctrl->finished_state = FINISH_FALSE;
                                SendVarReq(j, port);
                                break;

                            case MGR_CMD_SYM:
                                /*不管是什么态，都支持查询，即使是环回态*/
                                p_ctrl->sym_period_req = 1;
                                p_ctrl->tx_sym_err = 1;
                                p_ctrl->tx_sym_err_cnt = p_tx->err_sym_period_tlv_x.err;
                                SendEvent(j, port);
                                break;

                            case MGR_CMD_FRM:
                                /*不管是什么态，都支持查询，即使是环回态*/
                                p_ctrl->frm_err_req = 1;
                                p_ctrl->tx_frm_err = 1;
                                p_ctrl->tx_frm_err_cnt = p_tx->err_frm_tlv_x.err;
                                SendEvent(j, port);
                                break;

                            case MGR_CMD_FRM_PERIOD:
                                /*不管是什么态，都支持查询，即使是环回态*/
                                p_ctrl->frm_period_req = 1;
                                p_ctrl->tx_frm_prd_err = 1;
                                p_ctrl->tx_frm_prd_err_cnt = p_tx->err_frm_period_tlv_x.err;
                                SendEvent(j, port);
                                break;

                            case MGR_CMD_FRM_SECOND:
                                /*不管是什么态，都支持查询，即使是环回态*/
                                p_ctrl->second_period_req = 1;
                                p_ctrl->tx_frm_snd_err = 1;
                                p_ctrl->tx_frm_snd_err_cnt = p_tx->err_frm_second_tlv_x.err;
                                SendEvent(j, port);
                                break;

                            case MGR_CMD_ORG:
                                /*不管是什么态，都支持查询，即使是环回态*/
                                SendOrgPdu(j, port);
                                break;

                            case MGR_CMD_RST_OAM:  /* 本端复位，不发包*/
                                /* NBB_PRINTF("MGR_CMD_RST_OAM:");看门狗中不能打印   */
                                SendInfLocalRemoteTlv(j, port); /*REMOTE收到的信息已复制，直接发送返回即可，要改的是本端信息    */
                                ResetOamPort(j, port);
                                NBB_PRINTF("MGR_CMD_RST_OAM:");
                                break;

                            default: /*其他命令则都认为是送ORG PDU */
                                SendOrgPdu(j, port);
                                break;
                        } /*end switch*/
                    }

                    p_ctrl->rx_mgr_cmd = 0; /*对网管命令清零*/
                    continue;
                } /* 处于ANY态      */
            } /*判断端口是否有效*/
            GetPmMacPhyFrmErr(j, port);
        }

        /* 端口循环*/

        for (port = 0; port < MAX_PORT_NUM; port++)
        {
            // if (API_PORT_VALID(0, port))
            {
                if ((g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[port] == 0
                     || g_efm_oam_port_cfg[j].port_oam_x[port].enb == 0))
                    /*OAM不使能  */
                {
                    continue;
                }

                p_tx = g_efm_oam_port_cfg[j].port_oam_x[port].p_tx;
                p_rx = g_efm_oam_port_cfg[j].port_oam_x[port].p_rx;
                p_ctrl = g_efm_oam_port_cfg[j].port_oam_x[port].p_ctrl;

                if (g_efm_oam_port_cfg[j].port_oam_x[port].enb
                    && g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[port])
                {
                    p_ctrl->rx_pkt_type = NON_PDU_RX;
                    /*对所有收包类型清零表示该收包已处理完了*/

                    if (p_ctrl->unfinished)
                    {
                        p_ctrl->unfinished_time++;
                        /*NBB_PRINTF("port=%d   p_ctrl->unfinished_time  = %d port_oam_overtime_cfg[port]=%d\n",
                                   port, p_ctrl->unfinished_time,
                                   g_efm_oam_port_cfg[j].port_oam_overtime_cfg[port]);*/

                        if (p_ctrl->unfinished_time
                            > g_efm_oam_port_cfg[j].port_oam_overtime_cfg[port])
                            /*response_time_range  固定值*/
                        {
                            p_ctrl->finish_fail++;

                            /*命令执行失败的次数，只包括VAR/LB两种 命令 */
                            if (p_ctrl->unfinished == UNF_LB_CMD || p_ctrl->unfinished == UNF_DIS_LB_CMD)
                                /*当本端处于发环回或解环回命令等待应答时*/
                            {
                                p_ctrl->lb_exe_fail++; /*环回命令应答失败次数*/
                                p_ctrl->var_lb_fail_ais = 1;
                                /*环回命令失败告警*/
                                //NBB_PRINTF("p_ctrl->var_lb_fail_ais=1  :p_ctrl->lb_exe_fail++\n");
                            }

                            p_ctrl->unfinished_time = 0;
                            p_ctrl->unfinished = FINISHED; /*清除超时或不能应答的命令*/
                            //NBB_PRINTF("p_ctrl->unfinished,Then Local OamCtrlParMux %d 0,0\n", port);
                            p_ctrl->finished_state = FINISH_NORMAL;
                            /*等待应答的命令只有环回/查询，而网管的其他命令不需等待是否应答*/
                            OamCtrlParMux(j, port, MUX_FWD, PAR_FWD); /*110513 100609*/
                        }
                        else
                        {
                            //p_ctrl->unfinished_time=0;
                            //p_ctrl->finished_state=FINISH_NORMAL;/*110428解决第3次环回失败的问题* /
                        }
                    }
                    else
                    {
                        p_ctrl->unfinished_time = 0;
                        //110901 解决环回清除不了的故障
                        p_ctrl->finished_state = FINISH_NORMAL;/*110513
                        // 110428解决第3次环回失败的问题*/
                    }
                }
            }
        }

        five_second_period++;
#if 0   /*2011.11.27 qjwu*/

        //110516无论周期为多少，都固定LINK失效为5秒
        if (five_second_period == LOST_TIME) /*到5秒了*/
        {
            if (five_second_period == ((LOST_TIME * 10) / Period_100ms))
            {
                for (port = 0; port < MAX_PORT_NUM; port++)
                {
                    if (API_PORT_VALID(0, port))
                    {
                        if ((port_oam_enb_cfg_last[port] && port_oam_x[port].enb))
                        {
                            RxOamLostTimer(port); /*判断是否5秒能都未收到包 */
                        }
                    }
                }

                five_second_period = 0; /*清零*/
            }
        }

#endif

        for (port = 0; port < MAX_PORT_NUM; port++)
        {
            //if (API_PORT_VALID(0, port))
            {
                if ((g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[port]
                     && g_efm_oam_port_cfg[j].port_oam_x[port].enb))
                {
                    RxOamLostTimer(j, port); /*判断是否5秒能都未收到包 */
                }
            }
        }

        //if(wdStart(wdog_id_1s, sysClkRateGet(),(FUNCPTR)StartTxBy1S,0)==ERROR)
        //NBB_PRINTF("StartTxBy1S wdStart FAIL  !\n");
        /*TxLinkOamTask(0);放在这运行会死机,即使任务不开启这程序*/
    }
}

NBB_VOID TxLinkOamTask(NBB_INT unit) /*轮询检查是否有端口有OAM包待发送*/
{
    c3_rxtx_pkt_t pSendPkt;
    NBB_BYTE i, j;
    NBB_INT rv;
    NBB_BYTE local_slot;
    NBB_INT iRet = ERROR;
    minm_header_t stminm_head;
    NBB_BYTE data[oam_max_size];
    OS_MEMSET(data, 0, oam_max_size);

    for (j = 0 ; j < SUB_NUM; j++)
    {
        for (i = 0; i < MAX_PORT_NUM; i++)
        {
            iRet = ApiC3GetPhyLink(0, g_efm_oam_port_cfg[j].logic_port[i],
                                   &(phy_link_state[j][i])); /*2011.10.18 qjwu*/

            if (ERROR == iRet)
            {
                return;
            }

            //if (API_PORT_VALID(0, i))
            {
                if ((g_efm_oam_port_cfg[j].port_oam_x[i].enb == 0
                     || g_efm_oam_port_cfg[j].port_oam_enb_cfg_last[i] == 0))
                    /*OAM不使能*/
                {
                    continue;
                }
                else
                {
                    if (g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl->pdu_req == NO_REQUEST)
                        /*无发送请求，则查询下一个端口*/
                    {
                        continue;
                    }
                }

                UpdateFlag(j, i);
                GetPmMacPhyVar(j, i);
                OS_MEMSET(data, 0, oam_max_size);
                /*将包头填入*/
                NBB_MEMCPY(data, &(g_efm_oam_port_cfg[j].port_oam_x[i].p_tx->oam_pdu_x.oam_header_x), OAM_HDR_SIZE);
                NBB_MEMCPY(data + OAM_HDR_SIZE, g_efm_oam_port_cfg[j].port_oam_x[i].p_tx->oam_pdu_x.data,
                           g_efm_oam_port_cfg[j].port_oam_x[i].p_tx->oam_pdu_x.data_size);/*内容填入*/
                data[95] = g_efm_oam_port_cfg[j].port_oam_x[i].p_tx->oam_pdu_x.data_size;
                iRet = spm_hardw_getslot(&local_slot);

                if (SUCCESS != iRet)
                {
                    NBB_PRINTF(("Get slot info failed."));
                    return;
                }

                stminm_head.inner_type = 0x9876;
                stminm_head.port_type = ETH_PORT_TYPE;
                stminm_head.port_instance.eth_port_bits.slots = local_slot;
                stminm_head.port_instance.eth_port_bits.cards =
                    g_efm_oam_port_cfg[j].sub_cards;
                stminm_head.port_instance.eth_port_bits.ports =
                    g_efm_oam_port_cfg[j].logic_port[i];
                pSendPkt.type = 1;//临时用
                pSendPkt.payload = data;
                pSendPkt.len = TX_OAM_SIZE;
                pSendPkt.head = &stminm_head;

                /*NBB_PRINTF("Tx...flags=0x%x :opcode =%d :src_mod %d : dest_mod %d
                   \n",pSendPkt->flags,pSendPkt->opcode,pSendPkt->src_mod,
                pSendPkt->dest_mod);
                   NBB_PRINTF("...src_port %d : cos %d : prio_int %d : dest_port %
                d :
                      \n",pSendPkt->src_port,pSendPkt->cos,pSendPkt->prio_int,
                pSendPkt->dest_port);
                   NBB_PRINTF("tot_len %d : tx_pbmp %d : tx_upbmp %d : rx_port %d :
                      \n\n",pSendPkt->tot_len,pSendPkt->tx_pbmp,pSendPkt->
                tx_upbmp,pSendPkt->rx_port);    */
                if ((rv = ApiC3TxPkt(unit, &pSendPkt)) >= 0)  /*调驱动发送OAM包*/
                {
                    /*NBB_PRINTF("pdu_req is %d..\n",(port_oam_x[i].p_ctrl)->
                    pdu_req);*/
                    rx_tx_oam.tx_cnt_total++;
                    ((g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl)->oam_pkt_tx)++;
                }
                else
                {
                    rx_tx_oam.tx_cnt_err++;
                    ((g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl)->oam_pkt_tx_err)++;
                }

                (g_efm_oam_port_cfg[j].port_oam_x[i].p_ctrl)->pdu_req = NO_REQUEST;
                /*清零，即包发完了暂无请求*/
                ExeMacTxRx(j, i);
#if 0

                if (OneSecondFlag) /* 如下故障性能由看门狗触发1秒采集一次*/
                {
                    //GetPmMacPhySymErr(j,i);
                    GetPmMacPhyFrmErr(j, i);
                }

#endif
            }
        }
    }

    //OneSecondFlag = 0; //110118
}
#endif
#endif
