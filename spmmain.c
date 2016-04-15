
/**MOD+***********************************************************************/
/* Module:    spmmain.c                                                      */
/*                                                                           */
/* Purpose:   The main procedures for SPM.                                   */
/*                                                                           */
/* FiberHome        2003-2012                                                */
/*                                                                           */
/* $Revision: 1.41 $ $Date: 2010/11/22 07:09:09 $             */
/*                                                                           */
/**MOD-***********************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL
#define SPM_DEFINE_VARS
#define SPMMIB_DEFINE_VARS

#define ATG_DCI_DEFINE_VARS
/*****************************************************************************/
/* We need to define the ATG_XXX_DEFINE_VARS constants here for all          */
/* platforms that use dynamically loaded libraries (WIN32).  This is because */
/* interface global variables need to be defined in all dlls that use them.  */
/*****************************************************************************/
//#ifdef WIN32
#define ATG_SBI_DEFINE_VARS
//#endif

#define SPM_STATE_TLV_MAXNUM 0xFF


#ifdef WIN32
#include <nbntwin.h>
#endif

#include <nbase.h>
#include <spmincl.h>

/**PROC+**********************************************************************/
/* Name:      spm_create_proc                                                */
/*                                                                           */
/* Purpose:   �����ͳ�ʼ��SPM�����                                          */
/*                                                                           */
/* Returns:   TRUE, if the instance is created successfully                  */
/*            FALSE, otherwise                                               */
/*                                                                           */
/* Params:    IN     our_pid      - Process PID                              */
/*            IN     create_parms_rcvd                                       */
/*                                - The received create parameters           */
/*                                                                           */
/* Operation: Set up the local shared data.                                  */
/*            Initialize the AMBL and HAF libraries.                         */
/*                                                                           */
/**PROC-**********************************************************************/
NBB_BOOL spm_create_proc(NBB_PROC_ID our_pid,
                         NBB_VOID *create_parms_rcvd  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  SPM_CREATE_PARMS *create_parms = (SPM_CREATE_PARMS *) create_parms_rcvd;
  NBB_BOOL success = FALSE;
  NBB_ULONG queue_ids, sub_card_num;
  NBB_INT i;
  NBB_BYTE slot_bak;
//  NBB_BUF_SIZE avll_key_offset;  //Added by xiaoxiang, 2012/9/11

  /***************************************************************************/
  /*ΪSHARED LOCAL���ݻ������                                               */
  /***************************************************************************/
  NBS_GET_SHARED_DATA();

  NBB_TRC_ENTRY("spm_create_proc");

  /***************************************************************************/
  /* ���Դ���������Ч��                                                      */
  /***************************************************************************/
  NBB_ASSERT(create_parms->length == sizeof(SPM_CREATE_PARMS));

  /***************************************************************************/
  /* ����debuggingָ�롣                                                     */
  /***************************************************************************/
  v_spm_shared = &SHARED;

  /***************************************************************************/
  /* ��ʼ���������ݡ�                                                        */
  /***************************************************************************/
  NBB_MEMSET(&SHARED, 0, sizeof(SHARED));
  SHARED.spm_our_pid = our_pid;
  SHARED.spm_index = create_parms->spm_index;
  SHARED.sm_pid = create_parms->sm_pid;
  SHARED.sm_handle = create_parms->sm_handle;
  SHARED.ft_capable = ATG_NO;
  SHARED.instr_number = 0;
  
  /* ������ʱ�򣬻������ϱ�һ���Ծ���Ϣ��״̬Ĭ��ΪADD ��λ */
  SHARED.rcu_stat = ATG_MTL_NODE_UP_RESTART;

  /* ��ʼ��ȱʡ��״̬���������� */
  for(i = 0; i < SPM_STATE_TLV_MAXNUM; i++)
  {
    SHARED.state_func[i] = spm_proc_default_state;
  }

  /***************************************************************************/
  /* We do not need to initialize the spm_row_data and spm_top_mib_data on   */
  /* SHARED as this done by spm_init_row_data called by spm_define_mibs      */
  /* below.                                                                  */
  /***************************************************************************/

  /***************************************************************************/
  /* ΪSBI��join CBs��ʼ��LQE��                                              */
  /***************************************************************************/
  NBB_INIT_ROOT(SHARED.spm_sbi_join_list);

  /***************************************************************************/
  /* ��ȡ���ز�λ�� Added by xiaoxiang, 2013/1/19*/
  /***************************************************************************/
  spm_hardw_getslot(&SHARED.local_slot_id);
  printf("slot=%d\n", SHARED.local_slot_id);

  /***************************************************************************/
  /* ��ȡ�忨��C3оƬ���� Added by xiaoxiang, 2013/4/23*/
  /***************************************************************************/
  spm_hardw_getc3chipnum(&SHARED.c3_num);

  /***************************************************************************/
  /* ��ʼ���豸�˿ڹ��� Added by xiaoxiang, 2013/5/13*/
  /***************************************************************************/
  spm_init_dev_port_info(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ��VC��next_hop_id Added by xiaoxiang, 2014/2/17*/
  /***************************************************************************/
  spm_init_vc_nhi_id(NBB_CXT);
  
  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_PHYSICAL_PORT_CB��AVLL�� Added by xiaoxiang, 2012/9/11*/
  /***************************************************************************/
  spm_init_physical_port_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_LOGICAL_PORT_CB��AVLL�� Added by xiaoxiang, 2012/9/11*/
  /***************************************************************************/
  spm_init_logical_port_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_LAG_CB��AVLL�� Added by xiaoxiang, 2013/1/23*/
  /***************************************************************************/
  spm_init_lag_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_VC_CB��AVLL�� Added by xiaoxiang, 2012/10/9*/
  /***************************************************************************/
  spm_init_vc_tree(NBB_CXT);
                
  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_VPWS_CB��AVLL�� Added by xiaoxiang, 2012/10/9*/
  /***************************************************************************/
  spm_init_vpws_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_VPLS_CB��AVLL�� Added by xiaoxiang, 2012/10/9*/
  /***************************************************************************/
  spm_init_vpls_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_VPLS_MC_CB��AVLL�� Added by xiaoxiang, 2013/1/12*/
  /***************************************************************************/
  spm_init_vpls_mc_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_VPLS_UC_CB��AVLL�� Added by xiaoxiang, 2013/4/18*/
  /***************************************************************************/
  spm_init_vpls_uc_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_VPLS_UC_CB��AVLL�� Added by xiaoxiang, 2013/4/18*/
  /***************************************************************************/
  spm_init_associate_if_tree(NBB_CXT);

  
  /* add begin by limingming, 2013/1/17, Mantis��:HSD123456, ԭ��:BFD���� */
  /***************************************************************************/
  /* ��ʼ��BFDģ�� Added by limingming, 2013/1/17*/
  /***************************************************************************/
  (NBB_VOID)spm_bfd_init(NBB_CXT);
  /* add end by limingming, 2013/1/17 */
  
  /***************************************************************************/
  /* ��ʼ��OAMģ�� Added by huxi, 2013/5/2*/
  /***************************************************************************/
  spm_oam_init(NBB_CXT);
  /* add end by huxi, 2013/5/2 */
  
  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_ARP_CB��AVLL�� Added by xiaoxiang, 2012/10/9*/
  /***************************************************************************/
  spm_init_arp_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_NCACHE_CB��AVLL�� Added by xiaoxiang, 2013/4/16*/
  /***************************************************************************/
  spm_init_ncache_tree(NBB_CXT);
  
  /***************************************************************************/
  /* ��ʼ���鲥CUD�ʸ� Added by xiaoxiang, 2013/1/15*/
  /***************************************************************************/
  spm_init_vpls_mc_cud(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_PORT_MIRROR_CB��AVLL�� Added by xiaoxiang, 2013/03/15*/
  /***************************************************************************/
  spm_init_port_mirror_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_SWITCH_VC_CB��AVLL�� Added by xiaoxiang, 2013/03/26*/
  /***************************************************************************/
  spm_init_switch_vc_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_GLOBAL_CB�Ľṹ�� Added by xiaoxiang, 2013/04/12*/
  /***************************************************************************/
  spm_init_global_cb(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_CMD_SWTICH_CB�Ľṹ�� Added by zhangsen, 2014/04/03*/
  /***************************************************************************/
  spm_init_cmd_switch_cb(NBB_CXT);  

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_CIR_EMU_CB�Ľṹ�� Added by xiaoxiang, 2013/06/24*/
  /***************************************************************************/
  //spm_init_cir_emu_cb(NBB_CXT);
  spm_init_cir_emu_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_MAC_RECYCLE_CB�Ľṹ�� Added by xiaoxiang, 2013/04/12*/
  /***************************************************************************/
  spm_init_mac_recycle_cb(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_BYPASS_CB�Ľṹ�� Added by xiaoxiang, 2013/07/16*/
  /***************************************************************************/
  spm_init_bypass_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_MSP_CB�Ľṹ�� Added by xiaoxiang, 2014/04/10*/
  /***************************************************************************/
  spm_init_msp_tree(NBB_CXT);
  
  /***************************************************************************/
  /* ��ʼ��L3
  /***************************************************************************/

  spm_l3_ftntreeinit(NBB_CXT);

  spm_l3_ilmhashtableinit( 16000 NBB_CCXT);

  spm_l3_crlsprxhashtableinit( 16000 NBB_CCXT);

  spm_l3_crlsptxhashtableinit(16000 NBB_CCXT);
 
  spm_l3_vrfuroutetreeinit(NBB_CXT);

  spm_l3_mcroutetreeinit(NBB_CXT);

  spm_l3_tunnelchhashtableinit( 8000 NBB_CCXT);

  spm_l3_lspprothashtableinit( 8000 NBB_CCXT);

  spm_l3_vrfinstreeinit(NBB_CXT);


  //spm_l3_rsvpcitetreeInit();

  /***************************************************************************/

  //spm_ds_init();

  spm_qos_init(NBB_CXT);

  //spm_qos_voq_init();
    /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_IP_NF_CB��AVLL�� Added by fansongbo, 2013/5/14*/
  /***************************************************************************/
  spm_init_ip_nf_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ�����ڴ洢SPM_NF_AGGR_CB��AVLL�� Added by fansongbo, 2013/5/14*/
  /***************************************************************************/
  spm_init_nf_aggr_tree(NBB_CXT);

  /***************************************************************************/
  /* ��ʼ����������������Added by fansongbo, 2013/8/14*/
  /***************************************************************************/
  spm_init_nf_ctrl_cb();

  /***************************************************************************/
  /* ��ʼ��113״̬��Ĳ�ѯ���� Added by fansongbo, 2015/1/13*/
  /***************************************************************************/
  spm_init_mac_vpls_sta_info(NBB_CXT);  

  /***************************************************************************/
  /* ��ʼ��124״̬��Ĳ�ѯ���� Added by fansongbo, 2015/1/13*/
  /***************************************************************************/
  spm_init_mac_vpls_mac_sta_info(NBB_CXT);     
  
  /***************************************************************************/
  /* ��ʼ��apsģ��
  /***************************************************************************/
  //spm_aps_init();
  
  /***************************************************************************/
  /* ΪDCI�ӿڳ�ʼ��master join���ƿ顣                                      */
  /***************************************************************************/
  SHARED.sck_cb.appl_sock_handle =
    NBB_CREATE_HANDLE((SPM_SOCKET_CB *)&SHARED.sck_cb, HDL_SPM_SOCKET_CB);
  NBB_INIT_ROOT(SHARED.sck_cb.thread_list);

  spm_init_mj_join_cb(&(SHARED.spm_dci_join_cb) NBB_CCXT);
  spm_init_sck_join_cb(&(SHARED.sck_join_cb) NBB_CCXT);
  spm_init_mtl_join_cb(&(SHARED.mtl_join_cb) NBB_CCXT);

  /***************************************************************************/
  /* Initialize prod stub timers                                         */
  /***************************************************************************/
  SHARED.proc_timer_active = FALSE;
  SHARED.proc_timer.repeat = TRUE;
  SHARED.proc_timer.duration = PROC_SPM_TIMER_TIMEOUT;
  SHARED.proc_timer.timer_proc = spm_test_timer_function;
  NBB_INIT_TIMER(&(SHARED.proc_timer));

  /***************************************************************************/
  /* Initialize prod stub timers                                         */
  /***************************************************************************/
  SHARED.proc_timer_clearcfg_active = FALSE;
  SHARED.proc_timer_clearcfg.repeat = FALSE;
  SHARED.proc_timer_clearcfg.duration = PROC_TIMER_CLEARCFG_TIMEOUT;
  SHARED.proc_timer_clearcfg.timer_proc = spm_clearcfg_timer_function;
  NBB_INIT_TIMER(&(SHARED.proc_timer_clearcfg));


  /* ���ӿ���ص� join ���ƿ���Ϊ NULL. */
  for(sub_card_num = 0; sub_card_num < MAX_SUB_CARD_NUM; sub_card_num++)
  {
    SHARED.sub_card_cb[sub_card_num] = NULL;
  }

  /***************************************************************************/
  /* ���ñ�־����ʾHAF��AMBL��ʼ�����̡�                                     */
  /***************************************************************************/
  SHARED.spm_ambl_initialized = ATG_NO;
  SHARED.spm_haf_initialized = ATG_NO;

  if (!spm_haf_init(create_parms  NBB_CCXT))
  {
    NBB_TRC_FLOW((NBB_FORMAT "HAF library initialization failed"));
    NBB_PROBLEM((PCT_SPM | 0, 1, "lu", SHARED.spm_index));
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* HAF�������ѳ�ʼ����                                                     */
  /***************************************************************************/
  SHARED.spm_haf_initialized = ATG_YES;

  if (!AMBL_INITIALIZE(&(SHARED.spm_haf_data), &(SHARED.spm_ambl_data),
                       spm_ambl_reg_compl))
  {
    NBB_TRC_FLOW((NBB_FORMAT "MIB support library has failed to initialize"));
    NBB_PROBLEM((PCT_SPM | 3, 0, "lu", SHARED.spm_index));
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* Initialize mtl local path.                                             */
  /***************************************************************************/
  SHARED.local_frame = 1;
  spm_hardw_getslot(&slot_bak);
  SHARED.local_slot = slot_bak;
  SHARED.local_cpu = 0;

  /***************************************************************************/
  /* MIB֧�ֿ������ѳ�ʼ����                                                 */
  /***************************************************************************/
  SHARED.spm_ambl_initialized = ATG_YES;
  SHARED.spm_prod_mib_data = NULL;

  /***************************************************************************/
  /* ��MIB֧�ֿⶨ��MIBs��                                                   */
  /***************************************************************************/
  if (!spm_define_mibs(NBB_CXT))
  {
    NBB_TRC_FLOW((NBB_FORMAT "Failed to define MIBs"));
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* ����N-BASE���ȼ���                                                      */
  /***************************************************************************/
  NBB_SET_PRIORITY(SPM_NBASE_PRIORITY);

  /***************************************************************************/
  /* ���ý��ճ���                                                          */
  /***************************************************************************/
  NBB_SET_RECEIVE(spm_receive_proc);

  /***************************************************************************/
  /* �򿪺�ʹ�ܶ��С�                                                        */
  /***************************************************************************/
  queue_ids = USER_TO_SBI_Q |
              SM_TO_FTI_Q |
              NBB_SEND_Q |
              AMB_MIB_Q |
              DCI_TO_USER_Q |
              SCK_TO_APPL_Q |
              MTL_TO_USER_Q;
  NBB_OPEN_QUEUES(queue_ids);

  queue_ids = USER_TO_SBI_Q |
              SM_TO_FTI_Q |
              NBB_SEND_Q |
              AMB_MIB_Q |
              DCI_TO_USER_Q |
              SCK_TO_APPL_Q |
              MTL_TO_USER_Q;
  NBB_ENABLE_QUEUES(queue_ids);

  /***************************************************************************/
  /* �������ȫ���������Ǿͳɹ��ˡ�                                          */
  /***************************************************************************/
  success = TRUE;

  /***************************************************************************/
  /* ֪ͨHAF�������ѳɹ���ʼ����                                             */
  /***************************************************************************/
  NBB_TRC_FLOW((NBB_FORMAT "Initialisation successful"));
  AHL_FTI_INITIALIZED(&(SHARED.spm_haf_data), ATG_OK);
  AHL_FTI_DO_WORK(&(SHARED.spm_haf_data));

  /***************************************************************************/
  /* ��¼FTI�ѳɹ���ʼ����                                                   */
  /***************************************************************************/
  NBB_AUDIT((PCT_SPM | 1, 0, "lx", SHARED.spm_our_pid));

  /***************************************************************************/    

  EXIT_LABEL : NBB_TRC_DETAIL((NBB_FORMAT"Returning %B", success));

  NBB_TRC_EXIT();

  /***************************************************************************/
  /* �ͷ�SHARED LOCAL���ݵ�����                                              */
  /***************************************************************************/
  NBS_RELEASE_SHARED_DATA();

  return(success);
} /* spm_create_proc */

/**PROC+**********************************************************************/
/* Name:      spm_destroy_proc                                               */
/*                                                                           */
/* Purpose:   ɾ��SPM���̡�                                                  */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    None                                                           */
/*                                                                           */
/* Operation: Terminate the MIB support library.  Terminate the HAF library. */
/*            Free all the join CBs for SBI.                                 */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_destroy_proc(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  /***************************************************************************/
  /* Get the lock for the SHARED LOCAL data.                                 */
  /***************************************************************************/
  NBS_GET_SHARED_DATA();

  NBB_TRC_ENTRY("spm_destroy_proc");

  /***************************************************************************/
  /* ��ֹMIB��Ʒ֧�ֿ⡣                                                     */
  /***************************************************************************/
  if (SHARED.spm_ambl_initialized == ATG_YES)
  {
    NBB_TRC_FLOW((NBB_FORMAT "Terminating the AMBL library"));
    AMBL_TERMINATE(&(SHARED.spm_ambl_data));
  }

  /***************************************************************************/
  /* ��ֹHAF�⡣                                                             */
  /***************************************************************************/
  if (SHARED.spm_haf_initialized == ATG_YES)
  {
    NBB_TRC_FLOW((NBB_FORMAT "Terminating the HAF library"));
    AHL_TERMINATE(&(SHARED.spm_haf_data));
  }

  /***************************************************************************/
  /* ȡ����ʱ����                                                            */
  /***************************************************************************/
  if (SHARED.proc_timer_active)
  {
    NBB_TRC_FLOW((NBB_FORMAT "Cancel proc timer"));
    NBB_CANCEL_TIMER(&(SHARED.proc_timer));
    SHARED.proc_timer_active = FALSE;
  }

  /***************************************************************************/
  /* �ͷ�����SBI join CBs��                                                  */
  /***************************************************************************/
  spm_sbi_join_free_all(NBB_CXT);

  /***************************************************************************/
  /* �ͷ�����ҵ�����ÿ��ƿ顣                                                */
  /***************************************************************************/
  spm_free_all_cfg_cb(NBB_CXT);

  NBB_AUDIT((PCT_SPM | 4, 0, "lu", SHARED.spm_index));

  /***************************************************************************/
  /* SPM�������٣���ӡ��Ϣ��Ϊ�����á�                                  */
  /***************************************************************************/
  NBB_TRC_FLOW((NBB_FORMAT "SPM is being destroyed, just for test!"));
  NBB_PRINTF(" FH-SPM is being destroyed !!!\n");
  NBB_AUDIT((PCT_SPM | 6, 5, "s", "FH-SPM is being destroyed !!!"));
  NBB_PROBLEM((PCT_SPM | 6, 6, "s", "FH-SPM is being destroyed !!!"));

  NBB_TRC_EXIT();

  /***************************************************************************/
  /* Release the lock for the SHARED LOCAL data.                             */
  /***************************************************************************/
  NBS_RELEASE_SHARED_DATA();

  return;
} /* spm_destroy_proc */

/**PROC+**********************************************************************/
/* Name:      spm_ambl_reg_compl                                             */
/*                                                                           */
/* Purpose:   ֪ͨ��ƷMIB��Ʒ֧�ֿ������ע��MIBs��                          */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    IN     amb_reg      - AMB_REGISTER signal as received by the   */
/*                                  product.  This signal remains the        */
/*                                  property of the library.                 */
/*                                                                           */
/*                                  amb_reg is always non-NULL when this     */
/*                                  function is called from the library.     */
/*                                  However, it may be NULL when called from */
/*                                  product code.  In this latter case this  */
/*                                  always signifies successful synchronous  */
/*                                  registration, so respond OK.             */
/*                                                                           */
/* Operation: Signal completion of registration to the HAF library.          */
/*            Continue with post-registration processing.                    */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_ambl_reg_compl(AMB_REGISTER *amb_reg  NBB_CCXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/

  NBB_TRC_ENTRY("spm_ambl_reg_compl");

  /***************************************************************************/
  /* ֪ͨHAF��ע������ɡ�ע��amb_regΪNULLָʾͬ��ע��ɹ���                */
  /***************************************************************************/
  if (((amb_reg == NULL) || ((amb_reg != NULL) && (amb_reg->ret_code == ATG_OK))))
  {
    NBB_TRC_FLOW((NBB_FORMAT "Return success to HAF"));
    AHL_FTI_ACTION_DONE(&SHARED.spm_haf_data, AHL_FTI_ACT_MIB_REG, ATG_OK);
  }
  else
  {
    NBB_TRC_FLOW((NBB_FORMAT "Return failure to HAF"));
    AHL_FTI_ACTION_DONE(&(SHARED.spm_haf_data), AHL_FTI_ACT_MIB_REG,
                        ATG_UNSUCCESSFUL);
  }

  NBB_TRC_EXIT();

  return;
} /* spm_ambl_reg_compl */

/**PROC+**********************************************************************/
/* Name:      spm_define_mibs                                                */
/*                                                                           */
/* Purpose:   ��AMBL MIB֧�ֿⶨ������MIBs��                                 */
/*                                                                           */
/* Returns:   TRUE if successful, FALSE otherwise.                           */
/*                                                                           */
/* Params:    None                                                           */
/*                                                                           */
/* Operation: Call all functions that define MIBS with AMBL.                 */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_BOOL spm_define_mibs(NBB_CXT_T NBB_CXT)
{
  /***************************************************************************/
  /* Local Variables                                                         */
  /***************************************************************************/
  NBB_BOOL return_code = FALSE;

  NBB_TRC_ENTRY("spm_define_mibs");

  if (!spm_prod_do_define(NBB_CXT))
  {
    /*************************************************************************/
    /* ����top level MIBʧ�ܡ�                                               */
    /*************************************************************************/
    NBB_TRC_FLOW((NBB_FORMAT "Top level MIB failed to be defined"));
    goto EXIT_LABEL;
  }

  /***************************************************************************/
  /* MIB֧�ֿⲻ��Ϊtop level MIBs����create_row callback��                  */
  /* ������ڱ����ʼ��Ƕ��������ݡ�                                        */
  /***************************************************************************/
  spm_prod_init_row(NBB_CXT);

  /***************************************************************************/
  /* If we have got this far we are successful.                              */
  /***************************************************************************/
  return_code = TRUE;

  EXIT_LABEL:
  NBB_TRC_DETAIL((NBB_FORMAT"Returning %B", return_code));
  NBB_TRC_EXIT();

  return(return_code);
} /* spm_define_mibs */

/**PROC+**********************************************************************/
/* Name:      spm_free_all_cfg_cb                                            */
/*                                                                           */
/* Purpose:   �ͷ�����ҵ�����ÿ��ƿ顣                                       */
/*                                                                           */
/* Returns:   Nothing                                                        */
/*                                                                           */
/* Params:    None                                                           */
/*                                                                           */
/* Operation: Find all config cb and free them.                              */
/*                                                                           */
/**PROC-**********************************************************************/

NBB_VOID spm_free_all_cfg_cb(NBB_CXT_T NBB_CXT)
{
    /***************************************************************************/
    /* Local Variables                                                         */
    /***************************************************************************/
    SPM_PHYSICAL_PORT_CB *phy_port_cb = NULL;
    SPM_LOGICAL_PORT_CB *log_port_cb = NULL;
    SPM_LAG_CB *lag_cb = NULL;
    SPM_VC_CB *vc_cb = NULL;
    SPM_VPWS_CB *vpws_cb = NULL;
    SPM_VPLS_CB *vpls_cb = NULL;
    SPM_ARP_CB *arp_cb = NULL;
    SPM_VPLS_MC_CB *vpls_mc_cb = NULL;

    NBB_TRC_ENTRY("spm_free_all_cfg_cb");

    /*************************************************************************/
    /* �����ӿ��������õ�����һ����ɾ����                                    */
    /*************************************************************************/
    for (phy_port_cb = (SPM_PHYSICAL_PORT_CB*) AVLL_FIRST(SHARED.physical_port_tree);
         phy_port_cb != NULL;
         phy_port_cb = (SPM_PHYSICAL_PORT_CB*) AVLL_NEXT(SHARED.physical_port_tree,
                       phy_port_cb->spm_physical_port_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Delete phy_port_cb %p", phy_port_cb));
        spm_free_physical_port_cb(phy_port_cb NBB_CCXT);
    }

    /*************************************************************************/
    /* �����˿��߼����õ�����һ����ɾ����                                    */
    /*************************************************************************/
    for (log_port_cb = (SPM_LOGICAL_PORT_CB*) AVLL_FIRST(SHARED.logical_port_tree);
         log_port_cb != NULL;
         log_port_cb = (SPM_LOGICAL_PORT_CB*) AVLL_NEXT(SHARED.logical_port_tree,
                       log_port_cb->spm_logical_port_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Delete log_port_cb %p", log_port_cb));
        spm_free_logical_port_cb(log_port_cb NBB_CCXT);
    }

    /*************************************************************************/
    /* ������·�ۺ����õ�����һ����ɾ����                                    */
    /*************************************************************************/
    for (lag_cb = (SPM_LAG_CB*) AVLL_FIRST(SHARED.lag_tree);
         lag_cb != NULL;
         lag_cb = (SPM_LAG_CB*) AVLL_NEXT(SHARED.lag_tree,
                       lag_cb->spm_lag_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Delete lag_cb %p", lag_cb));
        spm_free_lag_cb(lag_cb NBB_CCXT);
    }

    /*************************************************************************/
    /* ����VC�����õ�����һ����ɾ����                                        */
    /*************************************************************************/
    for (vc_cb = (SPM_VC_CB*) AVLL_FIRST(SHARED.vc_tree);
         vc_cb != NULL;
         vc_cb = (SPM_VC_CB*) AVLL_NEXT(SHARED.vc_tree,
                       vc_cb->spm_vc_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Delete vc_cb %p", vc_cb));
        spm_free_vc_cb(vc_cb NBB_CCXT);
    }

    /*************************************************************************/
    /* ����VPWS���õ�����һ����ɾ����                                        */
    /*************************************************************************/
    for (vpws_cb = (SPM_VPWS_CB*) AVLL_FIRST(SHARED.vpws_tree);
         vpws_cb != NULL;
         vpws_cb = (SPM_VPWS_CB*) AVLL_NEXT(SHARED.vpws_tree,
                   vpws_cb->spm_vpws_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Delete vpws_cb %p", vpws_cb));
        spm_free_vpws_cb(vpws_cb NBB_CCXT);
    }

    /*************************************************************************/
    /* ����VPLS���õ�����һ����ɾ����                                        */
    /*************************************************************************/
    for (vpls_cb = (SPM_VPLS_CB*) AVLL_FIRST(SHARED.vpls_tree);
         vpls_cb != NULL;
         vpls_cb = (SPM_VPLS_CB*) AVLL_NEXT(SHARED.vpls_tree,
                   vpls_cb->spm_vpls_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Delete vpls_cb %p", vpls_cb));
        spm_free_vpls_cb(vpls_cb NBB_CCXT);
    }

    /*************************************************************************/
    /* ����ARP�����õ�����һ����ɾ����                                       */
    /*************************************************************************/
    for (arp_cb = (SPM_ARP_CB*) AVLL_FIRST(SHARED.arp_tree);
         arp_cb != NULL;
         arp_cb = (SPM_ARP_CB*) AVLL_NEXT(SHARED.arp_tree,
                       arp_cb->spm_arp_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Delete arp_cb %p", arp_cb));
        spm_free_arp_cb(arp_cb NBB_CCXT);
    }

    /*************************************************************************/
    /* ����VPLS�鲥���õ�����һ����ɾ����                                    */
    /*************************************************************************/
    for (vpls_mc_cb = (SPM_VPLS_MC_CB*) AVLL_FIRST(SHARED.vpls_mc_tree);
         vpls_mc_cb != NULL;
         vpls_mc_cb = (SPM_VPLS_MC_CB*) AVLL_NEXT(SHARED.vpls_mc_tree,
                     vpls_mc_cb->spm_vpls_mc_node))
    {
        NBB_TRC_FLOW((NBB_FORMAT "Delete vpls_mc_cb %p", vpls_mc_cb));
        spm_free_vpls_mc_cb(vpls_mc_cb NBB_CCXT);
    }

    NBB_TRC_EXIT();

  return;
}
