#pragma once
#include "main.h"
//-------------------------------------------------------------------------------------------

class CAN_BUS_PCA82C251T
{
public:
    CAN_BUS_PCA82C251T(CAN_HandleTypeDef* phcan, uint8_t FIFO_Num, uint32_t filterID = 0x321)
        : m_phcan(phcan), m_FIFO_Num(FIFO_Num), m_filterID(filterID)
    {
        CAN_Config();
    }

    uint8_t SendMessage(uint32_t id, uint32_t len, uint8_t* buf)
    {
        uint32_t offset = 0;
        uint32_t TxMailbox;
        CAN_TxHeaderTypeDef TxHeader;
        HAL_StatusTypeDef HAL_RetVal;
        uint32_t startTime = HAL_GetTick();

        TxHeader.IDE = CAN_ID_STD;
        TxHeader.StdId = id;
        TxHeader.RTR = CAN_RTR_DATA;
        TxHeader.TransmitGlobalTime = DISABLE;

        while (HAL_CAN_GetTxMailboxesFreeLevel(m_phcan) == 0)
        {
            if (HAL_GetTick() - startTime > 1000) // 1 second timeout
                return 1;
        }

        while (len > 0)
        {
            TxHeader.DLC = len > 8 ? 8 : len;
            do
            {
                HAL_RetVal = HAL_CAN_AddTxMessage(m_phcan, &TxHeader, buf + offset, &TxMailbox);
                HAL_Delay(1);
            } while (HAL_RetVal != HAL_OK);

            offset += TxHeader.DLC;
            len -= TxHeader.DLC;
        }
        return 0;
    }

    void ReceiveMessageCallback(uint32_t RxFifoITs)
    {
        if (RxFifoITs & (m_FIFO_Num == 0 ? CAN_IT_RX_FIFO0_MSG_PENDING : CAN_IT_RX_FIFO1_MSG_PENDING))
        {
            uint8_t Rxbuff[8];
            CAN_RxHeaderTypeDef RxHeader;

            if (HAL_CAN_GetRxMessage(m_phcan, CAN_RX_FIFO0, &RxHeader, Rxbuff) == HAL_OK)
            {
                if (RxHeader.StdId == m_filterID)
                {
                    for (int i = 0; i < RxHeader.DLC; i++)
                        Cabin_buff_push(Rxbuff[i]);
                }
            }
        }
    }

private:
    CAN_HandleTypeDef* m_phcan;
    uint8_t m_FIFO_Num;
    uint32_t m_filterID;

    void CAN_Config()
    {
        CAN_FilterTypeDef CAN_FilterType;
        CAN_FilterType.FilterBank = 0;
        CAN_FilterType.SlaveStartFilterBank = 0;
        CAN_FilterType.FilterIdHigh = m_filterID << 5;  // Adjusted for 32-bit filter
        CAN_FilterType.FilterMaskIdHigh = 0x0000;
        CAN_FilterType.FilterFIFOAssignment = m_FIFO_Num;
        CAN_FilterType.FilterMode = CAN_FILTERMODE_IDMASK;
        CAN_FilterType.FilterScale = CAN_FILTERSCALE_32BIT;
        CAN_FilterType.FilterActivation = ENABLE;

        if (HAL_CAN_ConfigFilter(m_phcan, &CAN_FilterType) != HAL_OK)
        {
            Error_Handler();
        }

        if (HAL_CAN_ActivateNotification(m_phcan, m_FIFO_Num == 0 ? CAN_IT_RX_FIFO0_MSG_PENDING : CAN_IT_RX_FIFO1_MSG_PENDING) != HAL_OK)
        {
            Error_Handler();
        }

        if (HAL_CAN_Start(m_phcan) != HAL_OK)
        {
            Error_Handler();
        }
    }
};
