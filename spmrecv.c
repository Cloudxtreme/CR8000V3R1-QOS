
/**MOD+***********************************************************************/
/* Module:    spmrecv.c                                                      */
/*                                                                           */
/* Purpose:   SPM receive procedure.                                         */
/*                                                                           */
/* FiberHome                            2003-2012                            */
/*                                                                           */
/* $Revision: 1.39 $ $Date: 2007/07/13 03:03:36 $               */
/*                                                                           */
/**MOD-***********************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/**PROC+**********************************************************************/
/* Name:      spm_receive_proc                                               */
/*                                                                           */
/* Purpose:   SPM receive procedure.                                         */
/* Purpose:   SPM 接收程序。                                                 */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     ips          - Received IPS.                            */
/*            IN     queue        - Queue IPS received on.                   */
/*                                                                           */
/* Operation: Fan out IPS by queue.                                          */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_receive_proc(NBB_IPS *ips,
                          NBB_QUEUE_ID queue  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  /***************************************************************************/
  /* Get the lock for the SHARED LOCAL data.                                 */
  /***************************************************************************/
  NBS_GET_SHARED_DATA();

  NBB_TRC_ENTRY("spm_receive_proc");

  NBB_TRC_DETAIL((NBB_FORMAT"Received IPS type: %lx on queue %lx",
                  ips->ips_type, queue));

  /***************************************************************************/
  /* Fan out IPS by queue.                                                   */
  /* 队列中输入的IPS。*/
  /***************************************************************************/
  switch (queue)
  {
    case AMB_MIB_Q:
      /***********************************************************************/
      /* Message received on MIB queue.  Pass to the MIB support library.    */
      /* MIB队列上收到的消息。传递到MIB support library。    */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "MIB Message received"));
      spm_rcv_mib_msg(ips  NBB_CCXT);
      break;

    case SM_TO_FTI_Q:
      /***********************************************************************/
      /* Message received from SM.                                           */
      /* 来自SM的消息。*/
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "SM message received"));
      AHL_RCV_SM_MSG(&(SHARED.spm_haf_data), ips);
      break;

    case NBB_SEND_Q:
      /***********************************************************************/
      /* Message received from the N-BASE.                                   */
      /* 来自N-BASE的消息。                                   */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "N-BASE message received"));
      spm_rcv_nbb_send_q_msg(ips  NBB_CCXT);
      break;

    case DCI_TO_USER_Q:
      /***********************************************************************/
      /* Message received from the DCI.                                       */
      /* 来自DCI的消息。                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "DCI message received"));
      spm_rcv_dci_ips(ips  NBB_CCXT);
      break;

    case USER_TO_SBI_Q:
      /***********************************************************************/
      /* Message received for SBI interface from the user.                   */
      /* SBI接口上来自user的消息。*/
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "Message received for SBI interface"));
      spm_rcv_sbi_ips(ips  NBB_CCXT);
      break;

    case SCK_TO_APPL_Q:
      /***********************************************************************/
      /* Message received from the SCK.                                      */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "SCK message received"));
      spm_rcv_sck_ips(ips  NBB_CCXT);
      break;

    case MTL_TO_USER_Q:
      /***********************************************************************/
      /* Message received from the MTL.                                       */
      /* 来自MTL的消息。                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "MTL message received"));
      spm_rcv_mtl_ips(ips  NBB_CCXT);
      break;
      
    default:
      /***********************************************************************/
      /* Invalid queue, drop message and assert invalid branch.              */
      /* 无效队列，丢弃消息并声明无效分支。*/
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "Invalid queue ID: %lx", queue));
      NBB_FREE_BUFFER(ips);
      NBB_ASSERT_INVALID_BRANCH;
      break;
  }

  /***************************************************************************/
  /* Call HAF Library                                                        */
  /* 调用HAF库。*/
  /***************************************************************************/
  NBB_TRC_FLOW((NBB_FORMAT "Call HAF library"));
  AHL_FTI_DO_WORK(&(SHARED.spm_haf_data));

  NBB_TRC_EXIT();

  /***************************************************************************/
  /* Release the lock on the SHARED LOCAL data.                              */
  /***************************************************************************/
  NBS_RELEASE_SHARED_DATA();

  return;
} /* spm_receive_proc */

/**PROC+**********************************************************************/
/* Name:      spm_rcv_sbi_ips                                                */
/*                                                                           */
/* Purpose:   Fan out the IPS on ips_type.                                   */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     ips          - NHR ips received                         */
/*                                                                           */
/* Operation: Check the OperStatus of SPM is up.  Check the join status is   */
/*            active.  Find the Join CB.  Pass the message to the relevant   */
/*            function.                                                      */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_rcv_sbi_ips(NBB_IPS *ips  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  SPM_SJ_JOIN_CB *join_cb;
  ATG_DCI_RQ *rq;
  ATG_SBI_RS *rs;
  NBB_BYTE buf[24] = {0};

  NBB_TRC_ENTRY("spm_rcv_sbi_ips");

  /***************************************************************************/
  /* Check current oper status is up.                                        */
  /***************************************************************************/
  if (SHARED.spm_row_data.oper_status != spm_prod_product_data.oper_states.up)
  {
    NBB_TRC_FLOW((NBB_FORMAT "SPM is not oper up (%d), message droped",
                  SHARED.spm_row_data.oper_status));
    NBB_FREE_BUFFER(ips);
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Find the CB for this join.                                              */
  /***************************************************************************/
  join_cb = (SPM_SJ_JOIN_CB *) AHL_SJ_CORRELATE_MSG(&(SHARED.spm_haf_data),
                                                    HDL_SPM_SBI_JOIN_CB, ips);

  /***************************************************************************/
  /* If there is no valid join CB, drop the message.                         */
  /***************************************************************************/
  if (join_cb == NULL)
  {
    NBB_TRC_FLOW((NBB_FORMAT "No SBI join CB, message dropped"));
    NBB_FREE_BUFFER(ips);
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Check join status is active.                                            */
  /***************************************************************************/
  NBB_ASSERT(join_cb->spm_join_status != SPM_SJ_INIT);

  /***************************************************************************/
  /* Fan out the IPS on type.                                                */
  /***************************************************************************/
  switch (ips->ips_type)
  {
    case IPS_ATG_SBI_REGISTER:
      /***********************************************************************/
      /* ATG_SBI_REGISTER message.                                           */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_SBI_REGISTER"));
      spm_rcv_sbi_register((ATG_SBI_REGISTER *) ips, join_cb  NBB_CCXT);
      SendData(0x1, 1, 0x00b101, 24, buf);
      break;

    case IPS_ATG_SBI_UNREGISTER:
      /***********************************************************************/
      /* ATG_SBI_UNREGISTER message.                                         */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_SBI_UNREGISTER"));
      spm_rcv_sbi_unregister((ATG_SBI_UNREGISTER *) ips, join_cb  NBB_CCXT);
      break;

    case IPS_ATG_SBI_RQ:
      /***********************************************************************/
      /* ATG_SBI_RQ message.                                         */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_SBI_RQ"));
#if 0
      //spm_rcv_sbi_rq((ATG_SBI_RQ *) ips   NBB_CCXT);
      NBB_AUDIT((PCT_SPM | 6, 0, "s", "FH-SPM receives IPS_ATG_SBI_RQ ips!"));
      NBB_PROBLEM((PCT_SPM | 6, 0, "s", "FH-SPM receives IPS_ATG_SBI_RQ ips!"));
      if(1)
      {
        rs = (ATG_SBI_RS*) NBB_GET_BUFFER(NBB_NULL_HANDLE,
                                           NBB_ALIGN_OFFSET(sizeof(ATG_SBI_RS)),
                                           0,
                                           NBB_RETRY_DATA | NBB_BUF_PKT); 
        /*************************************************************************/
        /* 初始化消息。                                                          */
        /*************************************************************************/
        NBB_ZERO_IPS(rs);

        /*************************************************************************/
        /* 建立消息包头。                                                        */
        /*************************************************************************/
        rs->ips_hdr.ips_type = IPS_ATG_SBI_RS;

        /*************************************************************************/
        /* 发送消息。                                                        */
        /*************************************************************************/
        spm_snd_sbi_ips(join_cb, (NBB_VOID *)rs, &(rs->ips_hdr) NBB_CCXT);
      }
      if(1)
      {
        rq = (ATG_DCI_RQ *) NBB_GET_BUFFER(NBB_NULL_HANDLE,
                                           NBB_ALIGN_OFFSET(sizeof(ATG_DCI_RQ)),
                                           0,
                                           NBB_RETRY_DATA | NBB_BUF_PKT); 
        /*************************************************************************/
        /* 初始化消息。                                                          */
        /*************************************************************************/
        NBB_ZERO_IPS(rq);

        /*************************************************************************/
        /* 建立消息包头。                                                        */
        /*************************************************************************/
        rq->ips_hdr.ips_type = IPS_ATG_DCI_RQ;

        /*************************************************************************/
        /* 发送消息。                                                        */
        /*************************************************************************/
        spm_snd_dci_ips((NBB_VOID *)rq, &(rq->ips_hdr) NBB_CCXT);
      }
#endif
      NBB_FREE_BUFFER(ips);
      break;
      
    case IPS_ATG_SBI_BOARD_INIT_INFO:
      /***********************************************************************/
      /* 获取ATG_SBI_BOARD_INIT_INFO配置.                                    */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_SBI_BOARD_INIT_INFO"));
      spm_rcv_sbi_hardwinit_result( join_cb->slot, (ATG_SBI_BOARD_INIT_INFO *)ips);
      break;

    case IPS_ATG_SBI_PORT_STATE:
      /***********************************************************************/
      /* 获取ATG_SBI_BOARD_INIT_INFO配置.                                    */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_SBI_PORT_STATE"));
      spm_rcv_sbi_linkstate_result( join_cb->slot, (ATG_SBI_PORT_STATE *)ips);
      break;

    case IPS_ATG_SBI_SFP_SPEED:
      /***********************************************************************/
      /* 获取ATG_SBI_SFP_SPEED配置.                                    */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_SBI_SFP_SPEED"));
      spm_rcv_sbi_sfpspeed_result( join_cb->slot, (ATG_SBI_SFP_SPEED *)ips);
      break;

    case IPS_ATG_SBI_SFP_STATE:
      /***********************************************************************/
      /* 获取IPS_ATG_SBI_SFP_STATE配置.                                    */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_SBI_SFP_STATE"));
      spm_rcv_sbi_sfpstate_result( join_cb->slot, (ATG_SBI_SFP_STATE *)ips);
      break;

    case IPS_ATG_SBI_SUBCARD_REPOWER:
    
      /***********************************************************************/
      /* 获取IPS_ATG_SBI_SUBCARD_REPOWER信息.                                */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_SBI_SUBCARD_REPOWER"));
      spm_rcv_sbi_subcardreboot_result( join_cb->slot);
      break;
      
    default:
      /***********************************************************************/
      /* Unknown Message.                                                    */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "Unknown message of type %lx", ips->ips_type));
      NBB_FREE_BUFFER(ips);
      //NBB_ASSERT_INVALID_BRANCH;
      break;
  }

  EXIT_LABEL : NBB_TRC_EXIT();

  return;
} /* spm_rcv_sbi_ips */

/**PROC+**********************************************************************/
/* Name:      spm_rcv_sbi_register                                           */
/*                                                                           */
/* Purpose:   Handle the ATG_SBI_REGISTER message.                           */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     sbi_register - ATG_NHR_REGISTER message.                */
/*            IN     spm_sj_join_cb  - Appropriate join CB.                  */
/*                                                                           */
/* Operation: Updates the fields in the join CB.  Informs the HAF library    */
/*            that the registration for the join CB is completed and send    */
/*            the response to the message.                                   */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_rcv_sbi_register(ATG_SBI_REGISTER *sbi_register,
                              SPM_SJ_JOIN_CB *join_cb  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  NBB_TRC_ENTRY("spm_rcv_sbi_register");

  /***************************************************************************/
  /* Update the join status (we have already asserted that is not in an      */
  /* initialized state in the calling code).                                 */
  /***************************************************************************/
  join_cb->spm_join_status = SPM_SJ_ACTIVE;

  /***************************************************************************/
  /* Inform the HAF library that SBI registration is complete for the SJ.    */
  /***************************************************************************/
  NBB_TRC_FLOW((NBB_FORMAT "SBI registration successful"));
  AHL_SJ_REG_DONE(&(SHARED.spm_haf_data), &(join_cb->spm_join_haf_sj_cb),
                  sbi_register->master_data_pid,
                  sbi_register->ips_hdr.sender_handle, ATG_OK);

  /***************************************************************************/
  /* Update the join CB parameters from the ATG_SBI_REGISTER message.  We do */
  /* this even if the register fails so that we can return the               */
  /* ATG_SBI_REGISTER.                                                       */
  /***************************************************************************/
  join_cb->spm_join_appl_pid = sbi_register->master_data_pid;
  join_cb->spm_join_appl_handle = sbi_register->master_data_handle;
  join_cb->spm_join_appl_qid = SBI_TO_USER_Q;

  /***************************************************************************/
  /* 更新子卡对应的 join_cb 中的槽位信息数据.                                */
  /***************************************************************************/
  if(sbi_register->slot < MAX_SUB_CARD_NUM)
  {
      join_cb->slot = sbi_register->slot;
      SHARED.sub_card_cb[sbi_register->slot] = join_cb;
  }
  else
  {
    NBB_PROBLEM((PCT_SPM | 7, 7, "s", "Sub card slot info error !"));
  }

  sbi_register->mother_board_code = GetBoardCode();
  join_cb->sbi_reg_over = TRUE;

  NBB_PRINTF(" SBM connects SPM through sbi join intface !!!\n");

  /***************************************************************************/
  /* Fill in response.                                                       */
  /***************************************************************************/
  sbi_register->return_code = ATG_SBI_RC_OK;

  /***************************************************************************/
  /* Send the response.                                                      */
  /***************************************************************************/
  spm_snd_sbi_ips(join_cb, (NBB_VOID *) sbi_register,
                  &(sbi_register->ips_hdr) NBB_CCXT);

  NBB_TRC_EXIT();

  return;
} /* spm_rcv_sbi_register */

/**PROC+**********************************************************************/
/* Name:      spm_rcv_sbi_unregister                                         */
/*                                                                           */
/* Purpose:   Handle the ATG_SBI_UNREGISTER message.                         */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     sbi_unreg    - ATG_SBI_UNREGISTER message.              */
/*            IN     spm_sj_join_cb  - Appropriate join CB.                     */
/*                                                                           */
/* Operation: Clear the fields in the join CB and inform the HAF library the */
/*            unregistration has occurred.  Send the response to the         */
/*            message.                                                       */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_rcv_sbi_unregister(ATG_SBI_UNREGISTER *sbi_unreg,
                                SPM_SJ_JOIN_CB *spm_sj_join_cb  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  NBB_TRC_ENTRY("spm_rcv_sbi_unregister");

  /***************************************************************************/
  /* Update the join CB status.                                              */
  /***************************************************************************/
  spm_sj_join_cb->spm_join_status = SPM_SJ_DEACTIVATING;

  /***************************************************************************/
  /* Need to send the response before NULLing out the join CB parameters.    */
  /***************************************************************************/
  spm_snd_sbi_ips(spm_sj_join_cb, (NBB_VOID *) sbi_unreg,
                  &(sbi_unreg->ips_hdr) NBB_CCXT);

  /***************************************************************************/
  /* Update the join CB parameters.                                          */
  /***************************************************************************/
  spm_sj_join_cb->spm_join_appl_pid = NBB_NULL_PROC_ID;
  spm_sj_join_cb->spm_join_appl_handle = NBB_NULL_HANDLE;
  spm_sj_join_cb->spm_join_appl_qid = NBB_NULL_QUEUE_ID;

  /***************************************************************************/
  /* Now inform the HAF library that the SJ has been unregistered.           */
  /***************************************************************************/
  AHL_SJ_UNREG_DONE(&(SHARED.spm_haf_data),
                    &(spm_sj_join_cb->spm_join_haf_sj_cb));

  /***************************************************************************/
  /* 更新子卡对应的 join_cb 中的槽位信息数据.                                */
  /***************************************************************************/
  SHARED.sub_card_cb[spm_sj_join_cb->slot] = NULL;

  NBB_TRC_EXIT();

  return;
} /* spm_rcv_sbi_unregister */

/**PROC+**********************************************************************/
/* Name:      spm_snd_sbi_ips                                                */
/*                                                                           */
/* Purpose:   通过SBI接口发送IPS。                                           */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     join_cb - join控制块                                    */
/*            IN     ips - 要发送的ips消息                                   */
/*            IN     ips_hdr - 要发送的ips消息的包头结构                     */
/*                                                                           */
/* Operation: 设置发送和接收句柄，然后发送到相应的模块。                     */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_snd_sbi_ips(SPM_SJ_JOIN_CB *join_cb,
                         NBB_VOID *ips,
                         NBB_IPS *ips_hdr
                         NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  NBB_TRC_ENTRY("spm_snd_sbi_ips");

  /***************************************************************************/
  /* Check we have been sent sensible values.                                */
  /***************************************************************************/
  //NBB_ASSERT(ips != NULL);
  //NBB_ASSERT(join_cb != NULL);
  if ((ips == NULL) || (join_cb == NULL))
  {
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* 建立发送者和接收者的句柄。                                              */
  /***************************************************************************/
  ips_hdr->receiver_handle = join_cb->spm_join_haf_sj_cb.partner_data_handle;
  ips_hdr->sender_handle = join_cb->spm_join_haf_sj_cb.fti_main_handle;  

  /***************************************************************************/
  /* Trace out PID and QID.                                                  */
  /***************************************************************************/
  NBB_TRC_INTERFACE((NBB_FORMAT "IPS %lx -> SBI", ips_hdr->ips_type));

  /***************************************************************************/
  /* Send the IPS.                                                           */
  /***************************************************************************/
  NBB_SEND_IPS(join_cb->spm_join_appl_pid, join_cb->spm_join_appl_qid, ips);

  EXIT_LABEL: NBB_TRC_EXIT();

  return;
} /* spm_snd_sbi_ips */

/**PROC+**********************************************************************/
/* Name:      spm_rcv_mib_msg                                                */
/*                                                                           */
/* Purpose:   Receives an IPS received on AMB_MIB_Q.                         */
/* Purpose:   在AMB_MIB_Q上收到一个IPS。                  */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     ips          - IPS received                             */
/*                                                                           */
/* Operation: Checks the message is from a valid SM and passes to MIB        */
/*            support library else drops the message.                        */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_rcv_mib_msg(NBB_IPS *ips  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_ULONG pro_type = 0;
  NBB_LONG index = 0;
  AMB_GEN_IPS *v_amb_ips = NULL;
  AMB_CUST_SBM *v_amb_sbm = NULL;
  AMB_STUBS_SCK *v_amb_sck = NULL;

  NBB_TRC_ENTRY("spm_rcv_mib_msg");

  if (AHL_FTI_CHECK_MIB_MESSAGE(&(SHARED.spm_haf_data), ips))
  {
    v_amb_ips = (AMB_GEN_IPS *)ips;
    pro_type = v_amb_ips->correlator.msg_type;
    if (SPM_START_SBM == pro_type || SPM_STOP_SBM == pro_type)
    {
        if (AMB_RC_NO_ERROR == v_amb_ips->ret_code)
        {
            v_amb_sbm = (AMB_CUST_SBM *)((NBB_BYTE *)v_amb_ips +
                                         v_amb_ips->data_offset);
            index = v_amb_sbm->sbm_index;

            /* 收到IPS_AMB_TEST OK消息，继续发IPS_AMB_SET消息。 */
            if (IPS_AMB_TEST == ips->ips_type)
            {
                if (SPM_START_SBM == pro_type)
                {
                    spm_bld_amb_set_sbm(index, AMB_ROW_CREATE_AND_GO,
                                        AMB_ADMIN_STATUS_UP, SPM_START_SBM  NBB_CCXT);
                    NBB_AUDIT((PCT_SPM | 32, 3, "s lu", "AMB_SET active SBM", index));
                }
                else if (SPM_STOP_SBM == pro_type)
                {
                    spm_bld_amb_set_sbm(index, AMB_ROW_DESTROY,
                                        AMB_ADMIN_STATUS_DOWN, SPM_STOP_SBM  NBB_CCXT);
                    NBB_AUDIT((PCT_SPM | 32, 4, "s lu", "AMB_SET deactive SBM", index));
                }
            }
            else if (IPS_AMB_SET == ips->ips_type)
            {
                if (SPM_START_SBM == pro_type)
                {
                    /* 激活SBM模块成功，通过socket发消息告知子卡线程。 */
                    spm_snd_mib_result_to_bmulib(index, pro_type, 0  NBB_CCXT);
                    NBB_AUDIT((PCT_SPM | 32, 5, "s lu", "AMB_SET active SBM over", index));
                }
            }
        }
        else
        {
            /* 出错打印。 */
            NBB_TRC_FLOW((NBB_FORMAT "The SPM has failed to active or deactive SBM!"));
            if (IPS_AMB_TEST == ips->ips_type)
            {
                NBB_AUDIT((PCT_SPM | 32, 6, "s lu", "AMB_TEST active SBM error", index));
            }
            else
            {
                NBB_AUDIT((PCT_SPM | 32, 6, "s lu", "AMB_SET active SBM error", index));
            }
            NBB_PROBLEM((PCT_SPM | 9, 0, "s lx",
                "The SPM has failed to active or deactive SBM!", SHARED.spm_index));
        }

        /* 释放收到的IPS消息。 */
        NBB_TRC_FLOW((NBB_FORMAT "discard MIB IPS received!"));
        NBB_FREE_BUFFER(ips);
    }
    else if (SPM_START_SCK == pro_type || SPM_STOP_SCK == pro_type)
    {
        if (AMB_RC_NO_ERROR == v_amb_ips->ret_code)
        {
            v_amb_sck = (AMB_STUBS_SCK *)((NBB_BYTE *)v_amb_ips +
                                         v_amb_ips->data_offset);
            index = v_amb_sck->index;

            /* 收到IPS_AMB_TEST OK消息，继续发IPS_AMB_SET消息。 */
            if (IPS_AMB_TEST == ips->ips_type)
            {
                if (SPM_START_SCK == pro_type)
                {
                    spm_bld_amb_set_sck(index, AMB_ROW_CREATE_AND_GO,
                                        AMB_ADMIN_STATUS_UP, SPM_START_SCK  NBB_CCXT);
                    NBB_AUDIT((PCT_SPM | 32, 7, "s lu", "AMB_SET active SCK", index));
                }
                else if (SPM_STOP_SCK == pro_type)
                {
                    spm_bld_amb_set_sck(index, AMB_ROW_DESTROY,
                                        AMB_ADMIN_STATUS_DOWN, SPM_STOP_SCK  NBB_CCXT);
                    NBB_AUDIT((PCT_SPM | 32, 8, "s lu", "AMB_SET deactive SCK", index));
                }
            }
            else if (IPS_AMB_SET == ips->ips_type)
            {
                /* 无。 */
                NBB_AUDIT((PCT_SPM | 32, 9, "s lu", "AMB_SET active SCK over", index));
            }
        }
        else
        {
            /* 出错打印。 */
            NBB_TRC_FLOW((NBB_FORMAT "The SPM has failed to active or deactive SCK!"));
            if (IPS_AMB_TEST == ips->ips_type)
            {
                NBB_AUDIT((PCT_SPM | 32, 10, "s lu", "AMB_TEST active SCK error", index));
            }
            else
            {
                NBB_AUDIT((PCT_SPM | 32, 101, "s lu", "AMB_SET active SCK error", index));
            }
            NBB_PROBLEM((PCT_SPM | 9, 1, "s lx",
                "The SPM has failed to active or deactive SCK!", SHARED.spm_index));
        }

        /* 释放收到的IPS消息。 */
        NBB_TRC_FLOW((NBB_FORMAT "discard MIB IPS received!"));
        NBB_FREE_BUFFER(ips);
    }
    else
    {
        /*************************************************************************/
        /* Message is OK.  Pass to MIB Support Library for processing.  The MIB  */
        /* Support Library is responsible for the buffer from now on.            */
        /* 消息正确。传递到MIB支持库进行处理。  */
        /* MIB支持库负责提供buffer。*/
        /*************************************************************************/
        NBB_TRC_FLOW((NBB_FORMAT "pass to AMBL"));
        AMBL_RCV_MIB_MESSAGE(&(SHARED.spm_ambl_data), ips);
    }
  }
  else
  {
    /*************************************************************************/
    /* Discard MIB IPS from old SM.                                          */
    /* 丢弃来自老的SM的MIB IPS。                                          */
    /*************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "discard MIB IPS from old SM"));
    NBB_FREE_BUFFER(ips);
  }

  NBB_TRC_EXIT();

  return;
} /* spm_rcv_mib_msg */

/**PROC+**********************************************************************/
/* Name:      spm_rcv_nbb_send_q_msg                                         */
/*                                                                           */
/* Purpose:   Receives a message on the NBB_SEND_Q                           */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     ips          - IPS received                             */
/*                                                                           */
/* Operation: Fans the message out depending on IPS type.  Currently we only */
/*            care about a posted buffer.                                    */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_rcv_nbb_send_q_msg(NBB_IPS *ips  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  NBB_TRC_ENTRY("spm_rcv_nbb_send_q_msg");

  switch (ips->ips_type)
  {
    case IPS_NBB_POSTED_BUFFER:
      /***********************************************************************/
      /* NBB_POSTED_BUFFER.                                                  */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "Posted buffer from the N-BASE"));
      spm_rcv_posted_buffer((NBB_POSTED_BUFFER *) ips  NBB_CCXT);
      break;

    default:
      /***********************************************************************/
      /* We do not expect to receive any other messages from the N-BASE so   */
      /* we drop it.                                                         */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "Unknown message - Freeing the buffer"));
      NBB_FREE_BUFFER(ips);
      break;
  }

  NBB_TRC_EXIT();

  return;
} /* spm_rcv_nbb_send_q_msg */

/**PROC+**********************************************************************/
/* Name:      spm_rcv_posted_buffer                                          */
/*                                                                           */
/* Purpose:   Pass the posted buffer to HAF or let the FTI process it.       */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     ips          - Posted buffer                            */
/*                                                                           */
/* Operation: Check whether the posted buffer is for the HAF library.  If it */
/*            is pass to the HAF library via the NTL library.  Otherwise     */
/*            process it ourselves.                                          */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_rcv_posted_buffer(NBB_POSTED_BUFFER *ips  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  NBB_TRC_ENTRY("spm_rcv_posted_buffer");

  if (AHL_RCV_POSTED_BUFFER(&(SHARED.spm_haf_data), ips))
  {
    /*************************************************************************/
    /* Posted buffer was for the HAF library.                                */
    /*************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "Posted buffer was for HAF"));
  }
  else
  {
    /*************************************************************************/
    /* The posted buffer must be for SPM.  Call into the NTL library so      */
    /* that the correct callback procedure is called for processing the      */
    /* buffer.                                                               */
    /*************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "Posted buffer arrived for SPM"));
    NBB_TRC_DETAIL((NBB_FORMAT "Buffer arrived from pool %lu", ips->pool_id));
    NTL_BUF_AVAILABLE((NBB_IPS *) ips);
  }

  NBB_TRC_EXIT();

  return;
} /* spm_rcv_posted_buffer */

NBB_VOID spm_recv_boot_stat_msg(NBB_BYTE *buf, NBB_ULONG data_len NBB_CCXT_T NBB_CXT)
{
    NBB_USHORT type = 0;
    NBB_USHORT length = 0;
    NBB_BYTE *buf_end = buf + data_len;

    NBB_TRC_ENTRY("spm_recv_boot_stat_msg");

    /* NOTE:跳过无用的4字节.协议版本和条目数.之前已经检查过了. */
    buf += sizeof(NBB_USHORT) + sizeof(NBB_USHORT);
    //while (data_len > 0)    
    while (buf < buf_end)  /* buf not end. */
    {
        NBB_GET_SHORT(type, buf);
        buf += 2;
        NBB_GET_SHORT(length, buf);
        buf += 2;

        /* 0x0001	单盘自举消息确认 */
        if (type == 1) /* 确保type在范围内. */
        {
            /* 0/1=确认成功/请求重发自举消息 */
            if(buf[0] == 0)
            {
                printf("recv confirm, cancel boot timer.....\n");
                if(TRUE == SHARED.proc_timer_active)
                {
                    NBB_CANCEL_TIMER(&(SHARED.proc_timer));
                    SHARED.proc_timer_active = FALSE;
                }
                break;
            }
            else if(buf[0] == 1)
            {
                if(FALSE == SHARED.proc_timer_active)
                {
                    NBB_SET_TIMER(&(SHARED.proc_timer));
                    SHARED.proc_timer_active = TRUE;
                    printf("request boot, start boot timer.....\n");
                }
                break;
            }
        }
        
        buf += length;
    }

    NBB_TRC_EXIT();
}



NBB_VOID spm_rcv_dci_rq(ATG_DCI_RQ* data_msg NBB_CCXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    NBB_BYTE *conf_data = NULL;
    NBB_ULONG data_len = 0;
    NBB_BYTE inst_type, inst_code, inst_extend, result = 0;

    NBB_TRC_ENTRY("spm_rcv_dci_rq");

    NBB_ASSERT(NULL != data_msg);

    data_len = data_msg->pkt_hdr.data_len;

    conf_data = ((NBB_BYTE *)((NBB_IPS *)data_msg)->data_ptr 
    + (data_msg->pkt_hdr.data_start));

    if(SPM_TCP_RQ_HEAD_DATA_SIZE > data_len)
    {
        printf("spm recv dci data len error %d\n", data_len);
        goto EXIT_LABEL;
    }

    /* 根据关键字，校验数据的合法性，新协议增加了四字节"L3VN"的头. */
    if((conf_data[0] == 'L') && 
       (conf_data[1] == '3') &&
       (conf_data[2] == 'V') && 
       (conf_data[3] == 'N'))
    {
        conf_data += 4;
    }
    else if((conf_data[10] != 'L') || 
            (conf_data[11] != '3') ||
            (conf_data[12] != 'V') || 
            (conf_data[13] != 'N'))
    {
        NBB_EXCEPTION((PCT_SPM | 6, 100, "s", "L3VN head verify error"));
        goto EXIT_LABEL;
    }

    /* 指令类型 */
    inst_type = conf_data[5];
    /* 指令码 */
    inst_code = conf_data[6];
    /* 指令扩展码 */
    inst_extend = conf_data[7];

    /* 收到请求单盘自举消息，通过 RQ 消息，转发给 SPM 模块.  */
    if((inst_type == 0X10) && (inst_code == 0X12) && (inst_extend == 0X00))
    {
        spm_recv_boot_stat_msg(conf_data + SPM_TCP_RQ_HEAD_DATA_SIZE,  
            (data_len - SPM_TCP_RQ_HEAD_DATA_SIZE) NBB_CCXT);
    }
    else 
    {
        printf("spm not support the inst type %d %d %d!\n", 
        inst_type, inst_code, inst_extend);

    }

    EXIT_LABEL : NBB_TRC_EXIT();

    return;
}



/**PROC+**********************************************************************/
/* Name:      spm_rcv_dci_ips                                                 */
/*                                                                           */
/* Purpose:   Passes the received DCI ips to appropriate handling code.       */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     ips          - The IPS received.                        */
/*                                                                           */
/* Operation: Check that the IPS has correctly sent to this join and then    */
/*            process the IPS depending on the IPS type.                     */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_VOID spm_rcv_dci_ips(NBB_IPS *ips  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  SPM_MJ_JOIN_CB *mj_cb;
  AHL_MJ_CB *haf_mj_cb;

  NBB_TRC_ENTRY("spm_rcv_dci_ips");

  /***************************************************************************/
  /* Correlate the message received to the DCI join.                          */
  /***************************************************************************/
  mj_cb = (SPM_MJ_JOIN_CB *) AHL_MJ_CORRELATE_MSG(&(SHARED.spm_haf_data),
                                                  HDL_SPM_DCI_JOIN_CB, ips);

  if ((mj_cb == NULL) || (mj_cb != &(SHARED.spm_dci_join_cb)))
  {
    /*************************************************************************/
    /* The IPS was from an old DCI join.  We drop it and exit.                */
    /*************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "Freeing DCI buffer %p", ips));
    NBB_FREE_BUFFER(ips);
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Switch on IPS type.                                                     */
  /***************************************************************************/
  switch (ips->ips_type)
  {
    case IPS_ATG_DCI_REGISTER:
      /***********************************************************************/
      /* DCI register response.                                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_REGISTER response"));
      spm_rcv_dci_register((ATG_DCI_REGISTER *) ips  NBB_CCXT);
      break;

    case IPS_ATG_DCI_UNREGISTER:
      /***********************************************************************/
      /* DCI unregister response.                                             */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_UNREGISTER response"));
      spm_rcv_dci_unregister((ATG_DCI_UNREGISTER *) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_RP:
      /***********************************************************************/
      /* Get report from the DCI.                                            */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_RP"));
      //spm_rcv_dci_rp((ATG_DCI_RP *) ips   NBB_CCXT);
      break;

    case IPS_ATG_DCI_RQ:
      /***********************************************************************/
      /* Get report from the DCI.                                            */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_RP"));
      spm_rcv_dci_rq((ATG_DCI_RQ *) ips   NBB_CCXT);
      NBB_AUDIT((PCT_SPM | 6, 2, "s", "FH-SPM receives IPS_ATG_DCI_RQ BOOT ips!"));
      break;

    case IPS_ATG_DCI_RS:
      /***********************************************************************/
      /* Get response from the DCI.                                          */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_RS"));
      //spm_rcv_dci_rs((ATG_DCI_RS *) ips   NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_PHYSICAL_PORT:
      /***********************************************************************/
      /* 获取接口物理配置.                                                   */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_PHYSICAL_PORT"));
      spm_rcv_dci_set_physical_port((ATG_DCI_SET_PHYSICAL_PORT *) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_LOGICAL_PORT:
      /***********************************************************************/
      /* 获取端口逻辑配置.                                                   */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_LOGICAL_PORT"));
      spm_rcv_dci_set_logical_port((ATG_DCI_SET_LOGICAL_PORT *) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_LINK_AGGR:
      /***********************************************************************/
      /* 获取链路聚合配置.                                                   */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_LINK_AGGR"));
      spm_rcv_dci_set_link_aggr((ATG_DCI_SET_LINK_AGGR *) ips NBB_CCXT);
      break;
      
    case IPS_ATG_DCI_SET_VC:
      /***********************************************************************/
      /* 获取VC表配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_VC"));
      spm_vc_process((ATG_DCI_SET_VC *) ips NBB_CCXT);
      break;  

    case IPS_ATG_DCI_SET_VPLS_MC:
      /***********************************************************************/
      /* 获取VPLS组播配置.                                                   */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_VPLS_MC"));
      spm_rcv_dci_set_vpls_mc((ATG_DCI_SET_VPLS_MC*) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_VPLS_UC:
      /***********************************************************************/
      /* 获取VPLS静态单播配置.                                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_VPLS_UC"));
      spm_rcv_dci_set_vpls_uc((ATG_DCI_SET_VPLS_UC*) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_ARP:
      /***********************************************************************/
      /* 获取ARP配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_ARP"));
      spm_rcv_dci_set_arp((ATG_DCI_SET_ARP *) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_IPV6_CACHE:
      /***********************************************************************/
      /* 获取ARP配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_IPV6_CACHE"));
      spm_rcv_dci_set_ncache((ATG_DCI_SET_IPV6_CACHE*) ips NBB_CCXT);
      break;
      
    case IPS_ATG_DCI_SET_PORT_MIRROR:
      /***********************************************************************/
      /* 获取端口镜像配置.                                                   */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_PORT_MIRROR"));
      spm_rcv_dci_set_port_mirror((ATG_DCI_SET_PORT_MIRROR*) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_GLOB_CONF:
      /***********************************************************************/
      /* 获取全局配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_GLOB_CONF"));
      spm_rcv_dci_set_global((ATG_DCI_SET_GLOB_CONF*) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_CIR_EMU_BOARD:
      /***********************************************************************/
      /* 获取电路仿真盘全局配置.                                             */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_CIR_EMU_BOARD"));
      spm_rcv_dci_set_cir_emu((ATG_DCI_SET_CIR_EMU_BOARD*) ips NBB_CCXT);
      break;
      
    case IPS_ATG_DCI_CMD_MAC_WITHDRAW:
      /***********************************************************************/
      /* 下发MAC地址撤销命令.                                                */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_CMD_MAC_WITHDRAW"));
      spm_rcv_dci_cmd_mac_withdraw((ATG_DCI_CMD_MAC_WITHDRAW*) ips NBB_CCXT);
      break;
	  
    case IPS_ATG_DCI_SET_BYPASS:
      /***********************************************************************/
      /* 获取Bypass双归保护配置.                                             */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_BYPASS"));
      spm_rcv_dci_set_bypass((ATG_DCI_SET_BYPASS*) ips NBB_CCXT);
      break;
      
    case IPS_ATG_DCI_SET_MSP:
      /***********************************************************************/
      /* 获取端口镜像配置.                                                   */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_MSP"));
      spm_rcv_dci_set_msp((ATG_DCI_SET_MSP*) ips NBB_CCXT);
      break;
      
    case IPS_ATG_DCI_SET_FTN:
      /***********************************************************************/
      /* 获取FTN配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_FTN"));
      spm_l3_ftnprocess((ATG_DCI_SET_FTN *) ips   NBB_CCXT);
      break;    

    case IPS_ATG_DCI_SET_ILM:
      /***********************************************************************/
      /* 获取ILM配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_ILM"));
      spm_l3_ILMprocess(( ATG_DCI_SET_ILM *) ips   NBB_CCXT);
      break;    

    case IPS_ATG_DCI_SET_CR_LSP_RX:
      /***********************************************************************/
      /* 获取CR_LSP_RX配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_CR_LSP_RX"));
      spm_l3_crlsprxprocess(( ATG_DCI_SET_CR_LSP_RX *) ips   NBB_CCXT);
      break;   

    case IPS_ATG_DCI_SET_CR_LSP_TX:
      /***********************************************************************/
      /* 获取CR_LSP_TX配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_CR_LSP_TX"));
      spm_l3_crlsptxprocess((ATG_DCI_SET_CR_LSP_TX*)  ips   NBB_CCXT);
      break;   

    case IPS_ATG_DCI_SET_VRF_UC:
      /***********************************************************************/
      /* 获取VRF UC配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_VRF_UC"));
      //spm_l3_vrfurouteprocess((ATG_DCI_SET_VRF_UC *) ips   NBB_CCXT);
      break;   

    case IPS_ATG_DCI_VRFUC_ROUTE_UPDATE:
      /***********************************************************************/
      /* 获取VRFUC ROUTE UPDATE配置.                                         */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_VRFUC_ROUTE_UPDATE"));
      spm_l3_vrfurouteupdate((ATG_DCI_VRFUC_ROUTE_UPDATE *) ips   NBB_CCXT);
      break;   

    case IPS_ATG_DCI_SET_VRF_MC:
      /***********************************************************************/
      /* 获取VRF MC配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_VRF_MC"));
      spm_l3_mcrouteprocess((ATG_DCI_SET_VRF_MC *) ips   NBB_CCXT);
      break;   
	  
    case IPS_ATG_DCI_SET_TNNL_SELECT:
      /***********************************************************************/
      /* 获取TUNNEL SELECT配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_TNNL_SELECT"));
      spm_l3_tunnelchprocess(( ATG_DCI_SET_TNNL_SELECT *) ips   NBB_CCXT);
      break;   
	  
    case IPS_ATG_DCI_SET_LSP_PROT:
      /***********************************************************************/
      /* 获取LSP_PROT配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_LSP_PROT"));
      spm_l3_lspprotprocess(( ATG_DCI_SET_LSP_PROT *) ips   NBB_CCXT);
      break;   

    case IPS_ATG_DCI_SET_VRF_INSTANCE:
      /***********************************************************************/
      /* 获取VRF_实例配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_VRF_INSTANCE"));
      spm_l3_vrfinsprocess(( ATG_DCI_SET_VRF_INSTANCE *) ips   NBB_CCXT);
      break;   

    case IPS_ATG_DCI_SET_DS_DOMAIN:
      /***********************************************************************/
      /* 获取ATG_DCI_SET_DS_DOMAIN配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_DS_DOMAIN"));
      spm_rcv_dci_set_ds_domain_v1(( ATG_DCI_SET_DS_DOMAIN*) ips);
      break;   

    case IPS_ATG_DCI_SET_DIFF_SERV:
      /***********************************************************************/
      /* 获取ATG_DCI_SET_DS_DOMAIN配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_SET_DIFF_SERV"));
      //spm_rcv_dci_set_ds(( ATG_DCI_SET_DIFF_SERV*) ips   NBB_CCXT);
      break; 

    case IPS_ATG_DCI_SET_ACL:
      /***********************************************************************/
      /* 获取IPS_ATG_DCI_SET_ACL配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_ACL"));
      spm_rcv_dci_set_acl(( ATG_DCI_SET_ACL*) ips);
      break; 

    case IPS_ATG_DCI_SET_QOS_POLICY:
      /***********************************************************************/
      /* 获取ATG_DCI_SET_QOS_POLICY配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_QOS_POLICY"));
      spm_rcv_dci_set_policy(( ATG_DCI_SET_QOS_POLICY*) ips);
      break; 


    case IPS_ATG_DCI_SET_QOS_BEHAVIOR:
      /***********************************************************************/
      /* 获取ATG_DCI_SET_QOS_BEHAVIOR配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_QOS_BEHAVIOR"));
      spm_rcv_dci_set_action(( ATG_DCI_SET_QOS_BEHAVIOR*) ips);
      break;

    case IPS_ATG_DCI_SET_QOS_CLASSIFY:
      /***********************************************************************/
      /* 获取ATG_DCI_SET_QOS_CLASSIFY配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_QOS_CLASSIFY"));
      spm_rcv_dci_set_classify(( ATG_DCI_SET_QOS_CLASSIFY*) ips);
      break;

    case IPS_ATG_DCI_SET_WRED:
      /***********************************************************************/
      /* 获取ATG_DCI_SET_WRED配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_WRED"));
      spm_rcv_dci_set_wred(( ATG_DCI_SET_WRED*) ips);
      break;  
      
    case IPS_ATG_DCI_SET_HQOS:
      /***********************************************************************/
      /* 获取IPS_ATG_DCI_SET_HQOS配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_HQOS"));
      //spm_rcv_dci_set_hqos(( ATG_DCI_SET_HQOS*) ips   NBB_CCXT);
      break; 
      
    case IPS_ATG_DCI_SET_USER_GROUP:
      /***********************************************************************/
      /* 获取ATG_DCI_SET_USR_GROUP配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_USR_GROUP"));
      //spm_rcv_dci_set_user_group(( ATG_DCI_SET_USER_GROUP*) ips   NBB_CCXT);
      break; 

    case IPS_ATG_DCI_SET_BFD_SESSION:
      /***********************************************************************/
      /* 获取ATG_DCI_SET_BFD_SESSION配置.                                                       */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_BFD_SESSION"));
      spm_rcv_dci_set_bfd(( ATG_DCI_SET_BFD_SESSION*) ips   NBB_CCXT);
      break;
      case IPS_ATG_DCI_SET_IP_NF:
	  /***********************************************************************/
      /* 获取IPS_ATG_DCI_SET_IP_NF配置.                                                       */
      /***********************************************************************/	
	  NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_SET_IP_NF"));
      spm_rcv_dci_set_ip_nf(( ATG_DCI_SET_IP_NF *) ips   NBB_CCXT);
	  break;
	case IPS_ATG_DCI_SET_NF_AGGR:
	  /***********************************************************************/
      /* 获取IPS_ATG_DCI_SET_NF_AGGR配置.                                                       */
      /***********************************************************************/	
	  NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_SET_NF_AGGR"));
      spm_rcv_dci_set_nf_aggr(( ATG_DCI_SET_NF_AGGR*) ips   NBB_CCXT);
	  break;	
      case IPS_ATG_DCI_CMD_NF_CTRL:
	  /***********************************************************************/
      /* 获取IPS_ATG_DCI_CMD_NF_CTRL配置.                                                       */
      /***********************************************************************/	
	  NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_CMD_NF_CTRL"));
         spm_rcv_dci_cmd_nf_ctrl(( ATG_DCI_CMD_NF_CTRL*) ips   NBB_CCXT);
	  break;	
    case IPS_ATG_DCI_SET_TPOAM_ACTIVE:
	  /***********************************************************************/
      /* 获取ATG_DCI_SET_TPOAM_ACTIVE配置.                                                       */
      /***********************************************************************/	
	  NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_TPOAM_ACTIVE"));
      spm_rcv_dci_set_active_oam(( ATG_DCI_SET_TPOAM_ACTIVE *) ips   NBB_CCXT);
	  break;
	case IPS_ATG_DCI_SET_TPOAM_ON_DEMAND:
	  /***********************************************************************/
      /* 获取ATG_DCI_SET_TPOAM_ON_DEMAND配置.                                                       */
      /***********************************************************************/	
	  NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_TPOAM_ON_DEMAND"));
      spm_rcv_dci_set_od_oam(( ATG_DCI_SET_TPOAM_ON_DEMAND *) ips   NBB_CCXT);
	  break;
	case IPS_ATG_DCI_SET_CLK_SYNC_BASE:
      /***********************************************************************/
      /* 获取IPS_ATG_DCI_SET_CLK_SYNC_BASE配置.                                                     */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_SET_CLK_SYNC_BASE"));
      spm_rcv_dci_set_clk_sync_base(( ATG_DCI_SET_CLK_SYNC_BASE *) ips );
      break; 
    case IPS_ATG_DCI_SET_GLOB_CLK:
    /***********************************************************************/
    /* 获取IPS_ATG_DCI_SET_GLOB_CLK配置.                                                     */
    /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_SET_GLOB_CLK"));
      spm_rcv_clk_set_glob(( ATG_DCI_SET_GLOB_CLK*) ips   NBB_CCXT);
      break; 
      
     case IPS_ATG_DCI_CMD_SWITCH:
        
    /***********************************************************************/
    /* 获取IPS_ATG_DCI_CMD_SWITCH配置.                                                     */
    /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_CMD_SWITCH")); 
      spm_rcv_dci_set_cmd_swtich(( ATG_DCI_CMD_SWITCH *)ips  NBB_CCXT);
      break;
      
    case IPS_ATG_DCI_CMD_COMMON_CTRL:
      /***********************************************************************/
      /* 处理设置常用控制命令.                                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_CMD_COMMON_CTRL"));
      spm_rcv_dci_cmd_common_ctrl((ATG_DCI_CMD_COMMON_CTRL*) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_STATUS_RQ:
      /***********************************************************************/
      /* 处理查询单盘状态请求.                                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_STATUS_RQ"));
      spm_rcv_dci_status_rq_new((ATG_DCI_STATUS_RQ *) ips);
      break;

    case IPS_ATG_DCI_SET_DEFEND_SWITCH:
      /***********************************************************************/
      /* 处理防攻击开关.                                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_DEFEND_SWITCH"));
      spm_rcv_dci_set_slot_defend((ATG_DCI_SET_DEFEND_SWITCH *) ips);
      break;
      
    case IPS_ATG_DCI_SET_DEFEND_POLICY:
      /***********************************************************************/
      /* 处理防攻击策略模板.                                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_DEFEND_POLICY"));
      spm_rcv_dci_set_defend_policy((ATG_DCI_SET_DEFEND_POLICY *) ips);
      break;

    case IPS_ATG_DCI_SET_TWAMP_IPV4:
      /***********************************************************************/
      /* 处理IPS_ATG_DCI_SET_TWAMP_IPV4模板.                                 */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_SET_TWAMP_IPV4"));
      //spm_rcv_dci_set_twamp_ipv4((ATG_DCI_SET_TWAMP_IPV4*) ips NBB_CCXT);
      break;

    case IPS_ATG_DCI_SET_TWAMP_IPV6:
      /***********************************************************************/
      /* 处理IPS_ATG_DCI_SET_TWAMP_IPV6模板.                                 */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "IPS_ATG_DCI_SET_TWAMP_IPV6"));
      //spm_rcv_dci_set_twamp_ipv6((ATG_DCI_SET_TWAMP_IPV6*) ips NBB_CCXT);
      break;
      
    case IPS_ATG_DCI_SET_SIGNALING_CTRL:
      /***********************************************************************/
      /* 信令控制.                                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_SIGNALING_CTRL"));
      spm_rcv_dci_set_rcu_pir_control((ATG_DCI_SET_SIGNALING_CTRL*)ips);
      break;
       
	 case IPS_ATG_DCI_SET_ASSOCIATE_IF:
	 /***********************************************************************/
      /* 处理关联接口配置.                                               */
      /***********************************************************************/
	  NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_ASSOCIATE_IF"));
      spm_rcv_dci_set_associate_if((ATG_DCI_SET_ASSOCIATE_IF *) ips  NBB_CCXT);
      break;
      
	 case IPS_ATG_DCI_CMD_CLK_CTRL:
	 /***********************************************************************/
      /* 处理关联接口配置.                                               */
      /***********************************************************************/
	  NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_CMD_CLK_CTRL"));
      spm_rcv_dci_cmd_clock_ctrl((ATG_DCI_CMD_CLK_CTRL *) ips  NBB_CCXT);
      break;   

	  case IPS_ATG_DCI_SET_VPLS_L3_MC:

	  /***********************************************************************/
      /* 处理vpls_l3组播配置.                                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "ATG_DCI_SET_VPLS_L3_MC"));
      spm_rcv_dci_set_vpls_l3_mc((ATG_DCI_SET_VPLS_L3_MC *) ips NBB_CCXT);
      
    default:
      /***********************************************************************/
      /* Unknown message type free the buffer.                               */
      /***********************************************************************/
      NBB_TRC_FLOW((NBB_FORMAT "Unknown message of type %lx", ips->ips_type));
      //NBB_ASSERT_INVALID_BRANCH;
      break;
  }

  //业务配置处理需返回(单播路由除外)，其他IPS消息则释放。
  if (IPS_ATG_DCI_SET_PHYSICAL_PORT <= ips->ips_type
      && IPS_ATG_DCI_CMD_MAC_WITHDRAW > ips->ips_type
      && IPS_ATG_DCI_SET_VRF_UC != ips->ips_type)
  {
      spm_snd_dci_ips((NBB_VOID*)ips, ips NBB_CCXT);
/*
可能 linx 接受数据延时，不能取消定时器。
只能在收到确认消息后才能取消自举定时器。
*/
/*
      if(TRUE == SHARED.proc_timer_active)
      {
          NBB_CANCEL_TIMER(&(SHARED.proc_timer));
          SHARED.proc_timer_active = FALSE;
          printf("spm recv config data, cancel timer!\n");
      }
*/
  }
  else
  {
      NBB_FREE_BUFFER(ips);
  }
  
  EXIT_LABEL :
  NBB_TRC_EXIT();

  return;
} /* spm_rcv_dci_ips */

/**PROC+**********************************************************************/
/* Name:      spm_snd_dci_ips                                                */
/*                                                                           */
/* Purpose:   发送IPS消息到dci。                                             */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     rq          - Pointer to IPS header.                   */
/*            IN     dci_cb   - Pointer to control block for the socket      */
/*                                  to use.  This is only valid for          */
/*                                  ATG_DCI_DATA_MSG signals.                */
/*                                                                           */
/* Operation: Fill in sender and receiver handles from join CB.  For data    */
/*            IPSs, fill in application and stub socket handles.  Send IPS   */
/*            across IP Sockets interface.                                   */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_snd_dci_ips(NBB_VOID *ips,
                         NBB_IPS *ips_hdr NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  AHL_MJ_CB *haf_mj_cb;

  NBB_TRC_ENTRY("spm_snd_dci_ips");

  /***************************************************************************/
  /* Assert input is valid.                                                  */
  /***************************************************************************/
  NBB_ASSERT(ips != NULL);
  NBB_ASSERT(ips_hdr != NULL);

  /***************************************************************************/
  /* Set up local pointer to join CB for convenience.                        */
  /***************************************************************************/
  haf_mj_cb = &SHARED.spm_dci_join_cb.spm_haf_cb;

  /***************************************************************************/
  /* Set up the IPS header information.                                      */
  /***************************************************************************/
  ips_hdr->sender_handle = haf_mj_cb->fti_main_handle;
  ips_hdr->receiver_handle = haf_mj_cb->partner_data_handle;

  /***************************************************************************/
  /* Send IPS to part n er.                                                  */
  /***************************************************************************/
  NBB_TRC_INTERFACE((NBB_FORMAT "IPS %lx -> DCI", ips_hdr->ips_type));
  NBB_SEND_IPS(haf_mj_cb->partner_data_pid, USER_TO_DCI_Q, ips);

  NBB_TRC_EXIT();

  return;
} /* spm_snd_dci_ips */

/**PROC+**********************************************************************/
/* Name:      spm_send_dci_register                                          */
/*                                                                           */
/* Purpose:   Sends an DCI register to the slave for the DCI identified by   */
/*            SM.                                                            */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     dci_reg      - The ATG_DCI_REGISTER buffer provided to  */
/*                                  DCI by HAF.                              */
/*                                                                           */
/* Operation: Fills in the fields on the ATG_DCI_REGISTER IPS and sends the  */
/*            message to the join identified by SM for this interface.       */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_send_dci_register(ATG_DCI_REGISTER *dci_reg NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  AHL_MJ_CB *haf_mj_cb = &(SHARED.spm_dci_join_cb.spm_haf_cb);

  NBB_TRC_ENTRY("spm_send_dci_register");

  /***************************************************************************/
  /* Check that the buffer supplied to us by HAF is sensible.                */
  /***************************************************************************/
  NBB_ASSERT(dci_reg != NULL);
  NBB_ASSERT(dci_reg->ips_hdr.ctrl_size >= sizeof(ATG_DCI_REGISTER));

  /***************************************************************************/
  /* Set up the IPS header information.                                      */
  /***************************************************************************/
  dci_reg->ips_hdr.ips_type = IPS_ATG_DCI_REGISTER;

  /***************************************************************************/
  /* Fill in the ATG_DCI_REGISTER data.                                      */
  /***************************************************************************/
  dci_reg->appl_join_handle = haf_mj_cb->fti_main_handle;
  dci_reg->appl_pid = SHARED.spm_our_pid;
  dci_reg->appl_type = TYPE_DCI_RQ;
  dci_reg->appl_qid = DCI_TO_USER_Q;

  /***************************************************************************/
  /* Send the IPS.                                                           */
  /***************************************************************************/
  spm_snd_dci_ips((NBB_VOID *) dci_reg, &(dci_reg->ips_hdr) NBB_CCXT);
  NBB_TRC_EXIT();

  return;
} /* spm_send_dci_register */

/**PROC+**********************************************************************/
/* Name:      tdci_rcv_dci_register                                          */
/*                                                                           */
/* Purpose:   Process the DCI register response and start any DCI processing */
/*            required.                                                      */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     dci_reg       - The DCI register buffer response.       */
/*                                                                           */
/* Operation: Inform the haf library of the outcome of DCI registration.  If */
/*            successful kick off the gets of the INET interfaces that the   */
/*            DCI knows about.                                               */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_rcv_dci_register(ATG_DCI_REGISTER *dci_reg  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local variables.                                                        */
  /***************************************************************************/
  NBB_BOOL initial_reg;

  NBB_TRC_ENTRY("spm_rcv_dci_register");

  /***************************************************************************/
  /* Assert input is valid.                                                  */
  /***************************************************************************/
  NBB_ASSERT(dci_reg != NULL);
  NBB_ASSERT(dci_reg->ips_hdr.ips_type == IPS_ATG_DCI_REGISTER);
  NBB_ASSERT(dci_reg->ips_hdr.ctrl_size >= sizeof(ATG_DCI_REGISTER));

  /***************************************************************************/
  /* Check if registration was successful.                                   */
  /***************************************************************************/
  if (dci_reg->return_code == ATG_DCI_RC_OK)
  {
    /*************************************************************************/
    /* Update join status.                                                   */
    /*************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "DCI registration OK"));

    /*************************************************************************/
    /* Inform the HAF library that IP Data Interface registration completed  */
    /* successfully.  Note that the initial_reg returned parameter is not    */
    /* currently used.                                                       */
    /*************************************************************************/
    AHL_MJ_ACTION_DONE(&(SHARED.spm_haf_data),
                       &(SHARED.spm_dci_join_cb.spm_haf_cb), AHL_MJ_ACT_REG,
                       ATG_OK, &initial_reg);

    /*************************************************************************/
    /* 设置dci注册完毕的标志为TRUE。                                         */
    /*************************************************************************/
    SHARED.spm_dci_join_cb.dci_reg_over = TRUE;

  }
  else
  {
    /*************************************************************************/
    /* DCI registration failed.  Write problem log.                          */
    /*************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "DCI registration has failed Ret code: %ld",
                  dci_reg->return_code));
    NBB_PROBLEM((PCT_SPM | 0, 2, "ld", dci_reg->return_code));

    /*************************************************************************/
    /* Inform the HAF library that IP Sockets Interface registration failed. */
    /* Note that the initial_reg returned parameter is not currently used.   */
    /*************************************************************************/
    AHL_MJ_ACTION_DONE(&(SHARED.spm_haf_data),
                       &(SHARED.spm_dci_join_cb.spm_haf_cb), AHL_MJ_ACT_REG,
                       ATG_RESOURCE_FAILURE, &initial_reg);
  }

  /***************************************************************************/
  /* Allow HAF library to do work.                                           */
  /***************************************************************************/
  AHL_FTI_DO_WORK(&(SHARED.spm_haf_data));

  NBB_TRC_EXIT();

  return;
} /* spm_rcv_dci_register */

/**PROC+**********************************************************************/
/* Name:      spm_send_dci_unregister                                        */
/*                                                                           */
/* Purpose:   Sends an DCI unregister to our part n er for the DCI interface.*/
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     dci_unreg    - the ATG_DCI_UNREGISTER buffer provided   */
/*                                  to RSM by HAF.                           */
/*                                                                           */
/* Operation: Fills in the ATG_DCI_UNREGISTER message and sends it to        */
/*            part n er.                                                     */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_send_dci_unregister(ATG_DCI_UNREGISTER *dci_unreg
                                 NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  NBB_TRC_ENTRY("spm_send_dci_unregister");

  /***************************************************************************/
  /* Check the buffer supplied by the HAF library is suitable and zero out   */
  /* the IPS.                                                                */
  /***************************************************************************/
  NBB_ASSERT(dci_unreg != NULL);
  NBB_ASSERT(dci_unreg->ips_hdr.ctrl_size >= sizeof(ATG_DCI_UNREGISTER));
  NBB_ZERO_IPS(dci_unreg);

  /***************************************************************************/
  /* Set up the IPS fields.                                                  */
  /***************************************************************************/
  dci_unreg->ips_hdr.ips_type = IPS_ATG_DCI_UNREGISTER;

  /***************************************************************************/
  /* Send the UNREGISTER message to our part n er.                           */
  /***************************************************************************/
  spm_snd_dci_ips((NBB_VOID *) dci_unreg, &(dci_unreg->ips_hdr) NBB_CCXT);

  NBB_TRC_EXIT();

  return;
} /* spm_send_dci_unregister */

/**PROC+**********************************************************************/
/* Name:      spm_rcv_dci_unregister                                         */
/*                                                                           */
/* Purpose:   Receives and processes the ATG_DCI_UNREGISTER response.        */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    IN     dci_unreg    - The ATG_DCI_UNREGISTER response.         */
/*                                                                           */
/* Operation: Inform the HAF library that the DCI unregister has completed.  */
/*            Free the unregister buffer.                                    */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_rcv_dci_unregister(ATG_DCI_UNREGISTER *un_reg NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  AHL_MJ_CB *mj_cb = &(SHARED.spm_dci_join_cb.spm_haf_cb);
  NBB_BOOL initial_reg;

  NBB_TRC_ENTRY("spm_rcv_dci_unregister");

  /***************************************************************************/
  /* 防止未使用的参数编译时产生告警。                                        */
  /***************************************************************************/
  NBB_UNREFERENCED_PARAMETER(un_reg);

  /***************************************************************************/
  /* Inform the HAF library we have successfully unregistered with the DCI.  */
  /* This cannot fail.                                                       */
  /***************************************************************************/
  AHL_MJ_ACTION_DONE(&(SHARED.spm_haf_data), mj_cb, AHL_MJ_ACT_UNREG, ATG_OK,
                     &initial_reg);

  NBB_TRC_EXIT();

  return;
} /* spm_rcv_dci_unregister */


/**PROC+**********************************************************************/
/* Name:      spm_init_sck_data_sizes                                    */
/*                                                                           */
/* Purpose:   To initialize the size array used by the NTL_OFFSET code in    */
/*            filling out an ATG_SCK_DATA structure.                     */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    OUT    size_array   - The resulting definition of the buffer   */
/*                                  size array.                              */
/*                                                                           */
/* Operation: Fill in the size array.                                        */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_init_sck_data_sizes(NBB_BUF_SIZE *size_array NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/

    NBB_TRC_ENTRY("spm_init_sck_data_msg_sizes");

    /***************************************************************************/
    /* The first entry in the size array is the size of the fixed structure.   */
    /***************************************************************************/
    size_array[0] = sizeof(ATG_SCK_DATA);
    NBB_TRC_DETAIL((NBB_FORMAT "size_array[0]=%lu", size_array[0]));

    /***************************************************************************/
    /* ATG_SCK_SCTP_SNDRCVINFO.                                                */
    /***************************************************************************/
    size_array[1] = 0;
    NBB_TRC_DETAIL((NBB_FORMAT "size_array[1]=%lu", size_array[1]));

    /***************************************************************************/
    /* Null terminating entry.                                                 */
    /***************************************************************************/
    size_array[2] = NTL_OFF_SIZE_ARRAY_TERMINATOR;
    NBB_TRC_DETAIL((NBB_FORMAT "size_array[2]=%lu", size_array[2]));

    //NBB_ASSERT(OFL_ATG_SCK_DATA == 2);

    NBB_TRC_EXIT();

    return;

} /* spm_init_sck_data_sizes */

/**PROC+**********************************************************************/
/* Name:      spm_fill_sck_data                                         */
/*                                                                           */
/* Purpose:   To fill in the common parts of a SCK_DATA_MSG.                 */
/*                                                                           */
/*            The caller must fill in the remote_inet_addr field and the     */
/*            appended ATG_SCK_IF_INFO structure.  It must also update       */
/*            pkt_hdr.data_len after filling in the data.                    */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    OUT    sck_data_msg - The filled in SCK_DATA_MSG.              */
/*                                                                           */
/* Operation: Fill in the common parts of the SCK_DATA_MSG.                  */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_fill_sck_data(ATG_SCK_DATA *sck_data NBB_CCXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    NBB_BUF_SIZE size_array[OFL_ATG_SCK_DATA + 1];
    NBB_BUF_SIZE ctrl_size;
    NBB_VOID *null_corr = NULL;

    NBB_TRC_ENTRY("spm_fill_sck_data");

    /***************************************************************************/
    /* Initialize the buffer size array for the NTL_OFFSET code.               */
    /***************************************************************************/
    spm_init_sck_data_sizes(size_array NBB_CCXT);

    /***************************************************************************/
    /* Check that the control part of the IPS is large enough for an           */
    /* ATG_SCK_DATA_MSG.                                                       */
    /***************************************************************************/
    ctrl_size = NTL_OFF_CALC_STRUCT_SIZE(size_array);
    NBB_ASSERT(ctrl_size <= sck_data->ips_hdr.ctrl_size);

    /***************************************************************************/
    /* Initialize the message NTL offset structures.                           */
    /***************************************************************************/
    NBB_ZERO_IPS(sck_data);
    NTL_OFF_INIT_STRUCT(sck_data, size_array, off_atg_sck_data);

    /***************************************************************************/
    /* Set up the IPS header.  The join handles are set up by Plumbing when    */
    /* the IPS is sent.                                                        */
    /***************************************************************************/
    sck_data->ips_hdr.ips_type = IPS_ATG_SCK_DATA;

    /***************************************************************************/
    /* Set up the packet header.                                               */
    /***************************************************************************/
    sck_data->pkt_hdr.data_start = (NBB_BUF_SIZE)NULL;
    sck_data->pkt_hdr.data_len = 0;

    /***************************************************************************/
    /* The socket handles are set up by Plumbing when sending the IPS.         */
    /***************************************************************************/
    sck_data->appl_sock_handle = NBB_NULL_HANDLE;
    sck_data->stub_sock_handle = NBB_NULL_HANDLE;

    /***************************************************************************/
    /* The SCK stub should free the buffer rather than returning it to DC-LMP, */
    /* so set the return PID and QID and the application correlator to NULL.   */
    /***************************************************************************/
    sck_data->return_pid = NBB_NULL_PROC_ID;
    sck_data->return_qid = NBB_NULL_QUEUE_ID;
    NBB_CORR_PUT_VALUE(sck_data->appl_correlator, null_corr);

    sck_data->message_type = ATG_SCK_REQUEST;

    /***************************************************************************/
    /* The remote_inet_addr field must be filled in by the caller.             */
    /***************************************************************************/

    //sck_data->remote_inet_addr.type = 0;
    //sck_datag->remote_inet_addr.length = 0;
    //NBB_MEMSET(sck_data->remote_inet_addr.address,
    //          0,
    //         sizeof(sck_data->remote_inet_addr.address));

    /***************************************************************************/
    /* Set the remote port.                                                    */
    /***************************************************************************/

    // sck_data->remote_port = (NBB_USHORT)(SHARED.top_row_cb.spm_snd_port);

    /***************************************************************************/
    /* Reset the protocol_header_offset, which the caller must set up.         */
    /***************************************************************************/
    sck_data->protocol_header_offset = 0;

    /***************************************************************************/
    /* The appended ATG_SCK_IF_INFO structure must be filled in by the caller. */
    /***************************************************************************/

    NBB_TRC_EXIT();

    return;

} /* spm_fill_sck_data */

/**PROC+**********************************************************************/
/* Name:      spm_snd_mib_result_to_bmulib                                   */
/*                                                                           */
/* Purpose:   SPM send test/set mib result to bmulib thread.                 */
/*                                                                           */
/* Returns:   Nothing.                                                       */
/*                                                                           */
/* Params:    NULL   .                  */
/*                                                                           */
/* Operation: SEND in the socket CB.             */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_snd_mib_result_to_bmulib(NBB_LONG index,
                                      NBB_LONG pro_type,
                                      NBB_BYTE result
                                      NBB_CCXT_T NBB_CXT)
{
    /*************************************************************************/
    /* Local Variables                                                       */
    /*************************************************************************/
    NBB_BUF_SIZE size_array[OFL_ATG_SCK_DATA + 1];
    ATG_SCK_DATA *ips_buffer = NULL;
    SPM_THREAD_SOCKET_CB *socket_cb = NULL;
    char buf_temp[28] = {0x82, 0x00, 0x02, 0x20, 0x20, 0x10, 0x12, 0x00, 
                            0xff, 0xff, 'L', '3', 'V', 'N', 0x00, 0x00, 
                            0x00, 0x0A, 0x20, 0x20, 0x00, 0x01, 0x00, 0x0E,
                            0x00, 0x02, 0x01, 0x00};
    NBB_ULONG data_size = 28;
    NBB_ULONG ctrl_size = 0;

    NBB_TRC_ENTRY("spm_snd_mib_result_to_bmulib");

    NBB_PUT_SHORT(buf_temp + 1, index);
    buf_temp[26] = (NBB_BYTE)(index - 1);
    buf_temp[27] = result;
    
    spm_init_sck_data_sizes(size_array NBB_CCXT);
    ctrl_size = NTL_OFF_CALC_STRUCT_SIZE(size_array);

    /***************************************************************************/
    /* Get the ips buffer including packet                                     */
    /***************************************************************************/
    ips_buffer = (ATG_SCK_DATA *)NBB_GET_BUFFER(NBB_NULL_HANDLE,
        ctrl_size,
        data_size,
        (NBB_BUF_PKT | NBB_RETRY_DATA));

    /*判断是否分配内存成功*/
    if (ips_buffer == NULL)
    {
        /*************************************************************************/
        /* Failed to allocate buffer.                                            */
        /* Enhancement: Post for buffer and send data_lost indication.           */
        /*************************************************************************/
        NBB_TRC_FLOW((NBB_FORMAT "Failed to allocate sbm ips buffer"));

        goto EXIT_LABEL;
    }
    spm_fill_sck_data(ips_buffer NBB_CCXT);
    ips_buffer->ips_hdr.ips_type = IPS_ATG_SCK_DATA;
    ips_buffer->pkt_hdr.data_start = 0;
    ips_buffer->pkt_hdr.data_len = data_size;
    NBB_PKT_COPY_IN(ips_buffer, ips_buffer->pkt_hdr.data_start,
        buf_temp, data_size);

    socket_cb = (SPM_THREAD_SOCKET_CB *) NBB_NEXT_IN_LIST(SHARED.sck_cb.thread_list);
    if (NULL != socket_cb)
    {
        spm_snd_sck_ips((NBB_IPS *)ips_buffer, socket_cb NBB_CCXT);
    }
    else
    {
        //待增加打印异常。
        NBB_ASSERT(NULL != socket_cb);
    }
    EXIT_LABEL:

    NBB_TRC_EXIT();

    return;

} /* spm_snd_mib_result_to_bmulib */





