#pragma once
#include "main.h"
#include <cstdint>

class CAN_BUS_PCA82C251T {
public:
    CAN_BUS_PCA82C251T(CAN_HandleTypeDef* phcan, uint32_t fifoNum = CAN_RX_FIFO0,
                       uint32_t filterID = 0x321U, uint32_t txTimeoutMs = 100U)
        : m_phcan(phcan), m_FIFO_Num(fifoNum == CAN_RX_FIFO1 ? CAN_RX_FIFO1 : CAN_RX_FIFO0),
          m_filterID(filterID), m_txTimeoutMs(txTimeoutMs) {
        if (m_phcan == nullptr || m_filterID > 0x7FFU) return;
        m_configured = CAN_Config();
    }

    // Return codes: 0 success, 1 invalid input/configuration, 2 timeout, 3 HAL error.
    uint8_t SendMessage(uint32_t id, uint32_t len, const uint8_t* buf) {
        if (!m_configured || id > 0x7FFU || (len != 0U && buf == nullptr)) return 1U;
        uint32_t offset = 0U;
        while (len > 0U) {
            const uint32_t start = HAL_GetTick();
            while (HAL_CAN_GetTxMailboxesFreeLevel(m_phcan) == 0U) {
                if ((HAL_GetTick() - start) >= m_txTimeoutMs) return 2U;
            }
            CAN_TxHeaderTypeDef header{};
            header.IDE = CAN_ID_STD;
            header.StdId = id;
            header.RTR = CAN_RTR_DATA;
            header.DLC = len > 8U ? 8U : len;
            header.TransmitGlobalTime = DISABLE;
            uint32_t mailbox = 0U;
            if (HAL_CAN_AddTxMessage(m_phcan, &header, buf + offset, &mailbox) != HAL_OK) return 3U;
            offset += header.DLC;
            len -= header.DLC;
        }
        return 0U;
    }

    void ReceiveMessageCallback(uint32_t rxFifoITs) {
        if (!m_configured) return;
        const uint32_t flag = m_FIFO_Num == CAN_RX_FIFO0
            ? CAN_IT_RX_FIFO0_MSG_PENDING : CAN_IT_RX_FIFO1_MSG_PENDING;
        if ((rxFifoITs & flag) == 0U) return;
        uint8_t data[8]{};
        CAN_RxHeaderTypeDef header{};
        if (HAL_CAN_GetRxMessage(m_phcan, m_FIFO_Num, &header, data) != HAL_OK) return;
        if (header.IDE != CAN_ID_STD || header.RTR != CAN_RTR_DATA ||
            header.StdId != m_filterID || header.DLC > sizeof(data)) return;
        for (uint32_t i = 0U; i < header.DLC; ++i) Qbuffer_CAN_push(data[i]);
    }

private:
    CAN_HandleTypeDef* m_phcan;
    uint32_t m_FIFO_Num;
    uint32_t m_filterID;
    uint32_t m_txTimeoutMs;
    bool m_configured = false;

    bool CAN_Config() {
        CAN_FilterTypeDef filter{};
        filter.FilterBank = 0U;
        filter.SlaveStartFilterBank = 14U;
        filter.FilterIdHigh = m_filterID << 5U;
        filter.FilterIdLow = 0U;
        filter.FilterMaskIdHigh = 0xFFE0U; // Exact 11-bit standard ID match.
        filter.FilterMaskIdLow = 0U;
        filter.FilterFIFOAssignment = m_FIFO_Num;
        filter.FilterMode = CAN_FILTERMODE_IDMASK;
        filter.FilterScale = CAN_FILTERSCALE_32BIT;
        filter.FilterActivation = ENABLE;
        if (HAL_CAN_ConfigFilter(m_phcan, &filter) != HAL_OK) return false;
        const uint32_t notification = m_FIFO_Num == CAN_RX_FIFO0
            ? CAN_IT_RX_FIFO0_MSG_PENDING : CAN_IT_RX_FIFO1_MSG_PENDING;
        if (HAL_CAN_ActivateNotification(m_phcan, notification) != HAL_OK) return false;
        return HAL_CAN_Start(m_phcan) == HAL_OK;
    }
};
