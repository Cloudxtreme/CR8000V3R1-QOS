/******************************************************************************

                  ��Ȩ���� (C), 1999-2013, ���ͨ�ſƼ��ɷ����޹�˾

 ******************************************************************************
  �� �� ��   : spm_l3_vrfins_drv.c
  �� �� ��   : v0
  ��    ��   : wjhe
  ��������   : 2016��4��13��
  ����޸�   :
  ��������   : vrfʵ���������
  �����б�   :
  �޸���ʷ   :

******************************************************************************/

#define SHARED_DATA_TYPE SPM_SHARED_LOCAL

#include <nbase.h>
#include <spmincl.h>

/*************************************************************************

* ������   �� spm_l3_vrfins_add_hqos_drv
* ������   �� hwenjie
* �������� �� 2016��4��13��
* �������� �� ����vrfʵ��qos
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_vrfins_add_hqos_drv(VRFINS *vrfins,NBB_ULONG nhid )
{
    NBB_INT  rv = 1;

	/*��μ��*/
    if (vrfins == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e vrfins_add_hqos_drv vrfins NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 

    rv = spm_hqos_add_vrf_node(vrfins->slot,vrfins->port,nhid,&(vrfins->qos_tunkey),&(vrfins->key),
                                                               &(vrfins->upqos));
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e spm_hqos_add_vrf_node.%d\n", __LINE__,rv);
    }
   EXIT_LABEL:
   return (rv);
}

/*************************************************************************

* ������   �� spm_l3_vrfins_del_hqos_drv
* ������   �� hwenjie
* �������� �� 2016��4��13��
* �������� �� ɾ��vrfʵ��qos
* ������� ��                            
* ������� �� ��
* ����ֵ��    
                                    
* ���ù�ϵ��
* ��    ���� 
*************************************************************************/
NBB_INT spm_l3_vrfins_del_hqos_drv(VRFINS *vrfins,NBB_ULONG nhid )
{
    NBB_INT  rv = 1;

	/*��μ��*/
    if (vrfins == NULL)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e vrfins_del_hqos_drv vrfins NULL \n", __LINE__);
        rv = 104;
        goto  EXIT_LABEL;
    } 

    rv = spm_hqos_del_vrf_node(vrfins->slot,vrfins->port,&(vrfins->qos_tunkey),&(vrfins->key),nhid);
    if(rv != 0)
    {
        BMU_SLOG(BMU_INFO, SPM_L3_LOG_DIR, "Line=%d e spm_hqos_del_vrf_node.%d\n", __LINE__,rv);
    }
   EXIT_LABEL:
   return (rv);
}



