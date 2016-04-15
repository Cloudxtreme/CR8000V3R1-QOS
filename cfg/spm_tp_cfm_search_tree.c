/******************************************************************************

  Copyright (C), 1999-2014, fiberhome Co., Ltd.

 ******************************************************************************
  File Name     : spm_tp_cfm_search_tree.c
  Version       : Initial Draft
  Author        : huxi
  Created       : 2014/9/3
  Last Modified :
  Description   : spm_tp_cfm_search_tree
  Function List :
  History       :
  1.Date        : 2014/9/3
    Author      : huxi
    Modification: Created file

******************************************************************************/
#include <nbase.h>
#include <spmincl.h>

/*****************************************************************************
 �� �� ��  : spm_oam_tree_delete
 ��������  : ɾ�����ã���������ɾ��
 �������  : ��
 �������  : ��
 �� �� ֵ  : NBB_INT
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��10��28��
    ��    ��   : huxi
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_oam_tree_delete(IN SPM_OAMTREE_NODE *pstoamtreenode,
                            IN NBB_ULONG uloamtreebase,
                            IN NBB_BYTE module,
                            IN NBB_BYTE flag)
{
    NBB_INT iret = ERROR;

    if (0x0 == module) //OAMģ��0
    {
        iret = spm_oamtree_nodedel(g_usoam_nomax, pstoamtreenode,
                                   &g_sttree_oamnodeset0);
    }
    else
    {
        iret = spm_oamtree_nodedel(g_usoam_nomax, pstoamtreenode,
                                   &g_sttree_oamnodeset1);
    }

    /* �жϽ�� */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "Del oam tree node failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (0x0 == module)
    {
        /* �ؽ������� */
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset0,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }
    else
    {
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset1,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }

    /* �ж�ִ�н�� */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "Rebuild oam tree failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    /* �������л����� */
    spm_oam_16write(uloamtreebase, TREE_USED_OFFSET , 0X1);

    if (0x0 == module)
    {
        /* д���� */
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset0,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }
    else
    {
        /* д���� */
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset1,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }

    /* �ж�ִ�н�� */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n"
                   "Rebuild oam tree failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

EXIT_LABEL:

    return iret;
}


/*****************************************************************************
   �� �� ��  : spm_oam_searchtree_build
   ��������  : ����������
   �������  : IN SPM_OAMTREE_NEWSET *psttreenodeset:������
             IN NBB_USHORT ustreelevelmax:������������
             IN NBB_ULONG *pultreebaseadd:������ÿ�����ʼ��ַ
             IN NBB_ULONG ulbaseaddress:�������Ļ���ַ
   �������  : ��
   �� �� ֵ  :
   ���ú���  :
   ��������  :

   �޸���ʷ      :
   1.��    ��   : 2012��8��27��
    ��    ��   : limingming
    �� �� ��   : aa
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_oam_searchtree_build(
    IN NBB_USHORT ustreelevelmax,
    IN SPM_OAMTREE_SET *psttreenodeset,
    IN NBB_ULONG *pultreebaseadd,
    IN NBB_ULONG ulbaseaddress,
    IN NBB_BYTE flag
    NBB_CCXT_T NBB_CCXT)
{
    NBB_USHORT ustreelevel = 0; /* �������Ĳ��� */
    NBB_USHORT uslevellength = 0; /* �������Ľڵ�� */
    NBB_USHORT usindex = 0; /* ��Ӧ���������� */
    NBB_ULONG  uladdress = 0;
    NBB_USHORT ustemploop = 0; /* ѭ������ */
    NBB_USHORT ustemplabel = 0;
    NBB_INT    iret = ERROR;
    NBB_USHORT mepi = 0;

    /* �Ϸ��Լ�� */
    if ((NULL == psttreenodeset) || (NULL == pultreebaseadd))
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n input pointer illegal\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        iret = -2;
        goto EXIT_LABEL;
    }

    /* ѭ������ */
    for (ustreelevel = 0; ustreelevel < ustreelevelmax; ustreelevel++)
    {
        pultreebaseadd[ustreelevel] = (((1 << ustreelevelmax) - 2) << (ustreelevel + 2))
                                      & ((1 << ustreelevelmax) * 4 - 1);

        iret = spm_oamtree_gettreelen(ustreelevel, ustreelevelmax, &uslevellength);

        if (SUCCESS != iret) /* �ж�ִ�н�� */
        {
            OS_SPRINTF(g_uc_message, "%s %s,%d\n get tree len error ret=%d\n",
                       ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
            BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
            iret = -3;
            goto EXIT_LABEL;
        }

        /* ѭ�� */
        for (ustemploop = 0; ustemploop < uslevellength; ustemploop = ustemploop + 4)
        {
            iret = spm_oamtree_getarridx(ustreelevel, ustemploop, ustreelevelmax, &usindex);

            if (SUCCESS != iret) /* �ж�ִ�н�� */
            {
                OS_SPRINTF(g_uc_message, "%s %s,%d\n Get tree Index error ret=%d\n",
                           ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
                BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
                iret = -4;
                goto EXIT_LABEL;
            }

            uladdress = pultreebaseadd[ustreelevel] + ustemploop;

            /*  ������ */
            ustemplabel = psttreenodeset->pstNodeArr[usindex].usLspNo;
            spm_oam_16write(ulbaseaddress, uladdress, ustemplabel);
            mepi = ustemplabel;

            if (0 == flag)
            {
                /*  TUNNEL  LABEL    */
                ustemplabel = (NBB_USHORT)((psttreenodeset->pstNodeArr[usindex].ulTunnelId >> 8) & 0x1fff);
                spm_oam_16write(ulbaseaddress, uladdress + 1, ustemplabel);

                ustemplabel = (NBB_USHORT)(((psttreenodeset->pstNodeArr[usindex].ulTunnelId << 8) & 0xff00)
                                           + (NBB_USHORT)((psttreenodeset->pstNodeArr[usindex].ulPwId >> 16) & 0x1f));
                spm_oam_16write(ulbaseaddress, uladdress + 2, ustemplabel);

                /*  PW  LABEL     */
                ustemplabel = (NBB_USHORT)(psttreenodeset->pstNodeArr[usindex].ulPwId & 0xffff);
                spm_oam_16write(ulbaseaddress, uladdress + 3, ustemplabel);
            }
            else
            {
                /*  vid1    */
                ustemplabel = (NBB_USHORT)(psttreenodeset->pstNodeArr[usindex].ulTunnelId & 0x00000fff) + (1 << 12);
                spm_oam_16write(ulbaseaddress, uladdress + 1, ustemplabel);

                /*  vid2    */
                ustemplabel = (NBB_USHORT)(psttreenodeset->pstNodeArr[usindex].ulPwId & 0x00000fff);
                ustemplabel = (ustemplabel & 0x000f) + ((ustemplabel & 0xfff0) << 4);
                spm_oam_16write(ulbaseaddress, uladdress + 2, ustemplabel);

                /*  vid3 + mel */
                //ustemplabel = CFMMEP[mepi].mdlevel;
                //spm_oam_16write(ulbaseaddress, uladdress + 3, ustemplabel);
            }

        }
    }

    iret = SUCCESS;
EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 �� �� ��  : spm_oam_tree_build
 ��������  : ������������ӽڵ�ͽ����Ĺ���
 �������  : ��
 �������  : ��
 �� �� ֵ  : ERROR/SUCCESS
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��10��23��
    ��    ��   : huxi
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_INT spm_oam_tree_build(IN SPM_OAMTREE_NODE *pstoamtreenode,
                           IN NBB_ULONG uloamtreebase,
                           IN NBB_BYTE module,
                           IN NBB_BYTE flag
                           NBB_CCXT_T NBB_CXT)
{
    NBB_INT iret = ERROR;
    
    if (0x0 ==  module) //OAMģ��0
    {
        iret = spm_oamtree_nodeadd(g_usoam_nomax, pstoamtreenode,
                                   &g_sttree_oamnodeset0);
    }
    else //OAMģ��1
    {
        iret = spm_oamtree_nodeadd(g_usoam_nomax, pstoamtreenode,
                                   &g_sttree_oamnodeset1);
    }

    /* ��������ӽڵ� */

    /* �ж�ִ�н�� */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n\n"
                   "Add node failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    if (0x0 == module)
    {
        /* �ؽ������� */
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset0,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }
    else
    {
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset1,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }

    /* �ж�ִ�н�� */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n\n"
                   "Build oam tree failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

    //spm_oam_change_search_tree(uloamtreebase);
    
    /* �������л����� */
    spm_oam_16write(uloamtreebase , TREE_USED_OFFSET , 0X1);

    /* д���� */
    if (0x0 == module)
    {
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset0,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }
    else
    {
        iret = spm_oam_searchtree_build(g_usoam_treelevel, &g_sttree_oamnodeset1,
                                        g_puloam_treebase, uloamtreebase + OAM_TREE_CFG_OFFSET, flag);
    }

    /* �ж�ִ�н�� */
    if (SUCCESS != iret)
    {
        OS_SPRINTF(g_uc_message, "%s %s,%d\n\n"
                   "Build oam tree failed ret=%d\n",
                   ACT_OAM_CFG_STRING, __FUNCTION__, __LINE__, iret);
        BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
        goto EXIT_LABEL;
    }

EXIT_LABEL:

    return iret;
}


/*****************************************************************************
 �� �� ��  : spm_oam_16write
 ��������  : OAMģ��FPGAд����
 �������  : ��
 �������  : ��
 �� �� ֵ  : NBB_VOID
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��12��3��
    ��    ��   : huxi
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_VOID spm_oam_16write(NBB_ULONG base_addr,
                         NBB_ULONG offset_addr,
                         NBB_USHORT data
                         NBB_CCXT_T NBB_CCXT)
{
#if SPU
#if 0
    if (MIX1LP4 == g_stHardwBoardInf.enBoardName
        || MIX10LP2 == g_stHardwBoardInf.enBoardName
        || MIX1OLP2 == g_stHardwBoardInf.enBoardName
        || MSA1LN4  == g_stHardwBoardInf.enBoardName)
    {
        (NBB_VOID)WriteFpga32b(base_addr + offset_addr, data);
    }
    else
    {
        spm_oamtree_s16write(base_addr, offset_addr, data);
    }
#endif
    spm_oamtree_s16write(base_addr, offset_addr, data);

#endif
#if SRC
    spm_ccu_32write(base_addr, offset_addr, data);
#endif
    return;
}



/*****************************************************************************
 �� �� ��  : spm_oam_16read
 ��������  : OAMģ��FPGA������
 �������  : ��
 �������  : ��
 �� �� ֵ  : NBB_USHORT
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2013��12��3��
    ��    ��   : huxi
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_USHORT spm_oam_16read(NBB_ULONG base_addr,
                          NBB_ULONG offset_addr
                          NBB_CCXT_T NBB_CCXT)
{
    NBB_USHORT data = 0;
#if SPU
#if 0
    if (MIX1LP4 == g_stHardwBoardInf.enBoardName
        || MIX10LP2 == g_stHardwBoardInf.enBoardName
        || MIX1OLP2 == g_stHardwBoardInf.enBoardName
        || MSA1LN4  == g_stHardwBoardInf.enBoardName)
    {
        data = spm_oam_32read(base_addr, offset_addr);
    }
    else
    {
        data = spm_oamtree_s16read(base_addr, offset_addr);
    }
#endif
    data = spm_oamtree_s16read(base_addr, offset_addr);
#endif
#if SRC
    data = spm_ccu_32read(base_addr, offset_addr);
#endif
    return data;
}



/*****************************************************************************
 �� �� ��  : spm_oam_32read
 ��������  : FPGA�Ĵ���32λ������
 �������  : ��
 �������  : ��
 �� �� ֵ  : NBB_ULONG
 ���ú���  :
 ��������  :

 �޸���ʷ      :
  1.��    ��   : 2014��1��2��
    ��    ��   : huxi
    �޸�����   : �����ɺ���

*****************************************************************************/
NBB_ULONG spm_oam_32read(NBB_ULONG base_addr,
                         NBB_ULONG offset_addr
                         NBB_CCXT_T NBB_CCXT)
{
    NBB_UINT uldata = 0;
    NBB_UINT uioffset = 0;

    uioffset = (NBB_INT)(base_addr + offset_addr);
    (NBB_VOID)ReadFpga32b(uioffset, &uldata);

    return uldata;
}

/*****************************************************************************
 Prototype    : spm_assign_oam_index
 Description  : ����cfm��tp��FPGA������
 Input        : NBB_INT
 Output       : None
 Return Value : NBB_USHORT *oamindex
 Calls        :
 Called By    :

  History        :
  1.Date         : 2014/9/3
    Author       : huxi
    Modification : Created function
*****************************************************************************/
NBB_INT spm_assign_oam_index(OUT NBB_USHORT *oamindex,
                    OUT NBB_USHORT *moduleindex,
                    OUT NBB_BYTE *module)
{
    NBB_INT iret = ERROR;
    NBB_USHORT uli = 0;
    if ((NULL == oamindex) || (NULL == moduleindex) || (NULL == module))
    {
        goto EXIT_LABEL;
    }
    
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING,
               __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    for (uli = 0; uli < OAM_MAX_NUM_ACTIVE_SPU; uli++)
    {
        if (FALSE == g_tp_cfmindex[uli].isIndexEnable)
        {
            *oamindex = uli;
            g_tp_cfmindex[uli].isIndexEnable = TRUE;
            if (uli < 2047)
            {
                *moduleindex = uli;
                *module = 0;
            }
            else
            {
                *moduleindex = uli - 2048;
                *module = 1;
            }
            iret = SUCCESS;
            break;
        }
    }        
    if(uli == OAM_MAX_NUM_ACTIVE_SPU)
    {
        printf("fpga is full!\n");
        goto EXIT_LABEL;
    }

EXIT_LABEL:

    return iret;
}


/*****************************************************************************
 Prototype    : spm_recycle_oam_index
 Description  : ����tp��cfm��FPGA��������
 Input        : NBB_USHORT oamindex
 Output       : None
 Return Value : NBB_INT
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/9/3
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_recycle_oam_index(NBB_USHORT oamindex)
{
    NBB_INT iret = ERROR;

    if (oamindex >= OAM_MAX_NUM_ACTIVE_SPU)
    {
        goto EXIT_LABEL;
    }
    OS_SPRINTF(g_uc_message, "%s %s,%d\n", ACT_OAM_ENTER_STRING,
               __FUNCTION__, __LINE__);
    BMU_SLOG(BMU_INFO, SPM_OAM_LOG_DIR, g_uc_message);
    if (g_tp_cfmindex[oamindex].isIndexEnable == TRUE)
    {
        g_tp_cfmindex[oamindex].isIndexEnable = FALSE;
        iret = SUCCESS;
    }

EXIT_LABEL:
    return iret;
}



/*****************************************************************************
 Prototype    : spm_oam_change_search_tree
 Description  : FPGA�������б���
 Input        : NBB_ULONG base_addr
 Output       : None
 Return Value : NBB_INT
 Calls        : 
 Called By    : 
 
  History        :
  1.Date         : 2014/10/16
    Author       : huxi
    Modification : Created function

*****************************************************************************/
NBB_INT spm_oam_change_search_tree(NBB_ULONG base_addr)
{
    NBB_USHORT temp = 0;
    NBB_BYTE tree_no_last = 0xff;
    NBB_BYTE tree_no_now = 0xff;
    temp = spm_oam_16read(base_addr, TREE_USED_OFFSET - 1);
    tree_no_last = (temp >> 4) & 0x1;
    do
    {
        /* �������л����� */
        temp = spm_oam_16read(base_addr,TREE_USED_OFFSET);
        temp |= 0x8;
        spm_oam_16write(base_addr , TREE_USED_OFFSET , temp);
        
        temp = spm_oam_16read(base_addr, TREE_USED_OFFSET - 1);
        tree_no_now = (temp >> 4) & 0x1;
    }while(!(tree_no_last ^ tree_no_now));
    return SUCCESS;
}

