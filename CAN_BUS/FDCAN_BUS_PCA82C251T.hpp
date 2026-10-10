#pragma once
#include "main.h"
#include <cstdint>

class FDCAN_BUS_PCA82C251T {
public:
    FDCAN_BUS_PCA82C251T(FDCAN_HandleTypeDef* handle, uint8_t fifoNum,
                         uint32_t filterID1 = 0x321U, uint32_t filterID2 = 0x7FFU,
                         uint32_t txTimeoutMs = 100U)
        : m_handle(handle), m_fifo(fifoNum ? 1U : 0U), m_filterID(filterID1),
          m_filterMask(filterID2), m_txTimeoutMs(txTimeoutMs) {
        if (m_handle == nullptr || m_filterID > 0x7FFU || m_filterMask > 0x7FFU) return;
        m_configured = Configure();
    }

    // Return codes: 0 success, 1 invalid input/configuration, 2 timeout, 3 HAL error.
    uint8_t SendMessage(uint32_t id, uint32_t len, const uint8_t* data) {
        if (!m_configured || id > 0x7FFU || (len != 0U && data == nullptr)) return 1U;
        uint32_t offset = 0U;
        while (len > 0U) {
            const uint32_t start = HAL_GetTick();
            while (HAL_FDCAN_GetTxFifoFreeLevel(m_handle) == 0U) {
                if ((HAL_GetTick() - start) >= m_txTimeoutMs) return 2U;
                HAL_Delay(1U);
            }
            const uint32_t count = len > 8U ? 8U : len;
            FDCAN_TxHeaderTypeDef header{};
            header.Identifier = id;
            header.IdType = FDCAN_STANDARD_ID;
            header.TxFrameType = FDCAN_DATA_FRAME;
            header.DataLength = EncodeDlc(count);
            header.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
            header.BitRateSwitch = FDCAN_BRS_OFF;
            header.FDFormat = FDCAN_CLASSIC_CAN;
            header.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
            header.MessageMarker = 0U;
            if (HAL_FDCAN_AddMessageToTxFifoQ(m_handle, &header, data + offset) != HAL_OK) return 3U;
            offset += count;
            len -= count;
        }
        return 0U;
    }

    void ReceiveMessageCallback(uint32_t rxFifoITs) {
        if (!m_configured) return;
        const uint32_t flag = m_fifo == 0U
            ? FDCAN_IT_RX_FIFO0_NEW_MESSAGE : FDCAN_IT_RX_FIFO1_NEW_MESSAGE;
        if ((rxFifoITs & flag) == 0U) return;
        uint8_t data[8]{};
        FDCAN_RxHeaderTypeDef header{};
        const uint32_t fifo = m_fifo == 0U ? FDCAN_RX_FIFO0 : FDCAN_RX_FIFO1;
        if (HAL_FDCAN_GetRxMessage(m_handle, fifo, &header, data) != HAL_OK) return;
        if (header.IdType != FDCAN_STANDARD_ID || header.RxFrameType != FDCAN_DATA_FRAME ||
            ((header.Identifier & m_filterMask) != (m_filterID & m_filterMask))) return;
        const uint32_t count = DecodeDlc(header.DataLength);
        if (count > sizeof(data)) return; // Classic CAN only.
        for (uint32_t i = 0U; i < count; ++i) Qbuffer_CAN_push(data[i]);
    }

private:
    FDCAN_HandleTypeDef* m_handle;
    uint8_t m_fifo;
    uint32_t m_filterID;
    uint32_t m_filterMask;
    uint32_t m_txTimeoutMs;
    bool m_configured = false;

    static uint32_t EncodeDlc(uint32_t n) {
        switch (n) {
            case 0U: return FDCAN_DLC_BYTES_0;
            case 1U: return FDCAN_DLC_BYTES_1;
            case 2U: return FDCAN_DLC_BYTES_2;
            case 3U: return FDCAN_DLC_BYTES_3;
            case 4U: return FDCAN_DLC_BYTES_4;
            case 5U: return FDCAN_DLC_BYTES_5;
            case 6U: return FDCAN_DLC_BYTES_6;
            case 7U: return FDCAN_DLC_BYTES_7;
            default: return FDCAN_DLC_BYTES_8;
        }
    }
    static uint32_t DecodeDlc(uint32_t dlc) {
        switch (dlc) {
            case FDCAN_DLC_BYTES_0: return 0U;
            case FDCAN_DLC_BYTES_1: return 1U;
            case FDCAN_DLC_BYTES_2: return 2U;
            case FDCAN_DLC_BYTES_3: return 3U;
            case FDCAN_DLC_BYTES_4: return 4U;
            case FDCAN_DLC_BYTES_5: return 5U;
            case FDCAN_DLC_BYTES_6: return 6U;
            case FDCAN_DLC_BYTES_7: return 7U;
            case FDCAN_DLC_BYTES_8: return 8U;
            default: return 0xFFFFFFFFU;
        }
    }
    bool Configure() {
        FDCAN_FilterTypeDef filter{};
        filter.IdType = FDCAN_STANDARD_ID;
        filter.FilterIndex = 0U;
        filter.FilterType = FDCAN_FILTER_MASK;
        filter.FilterConfig = m_fifo ? FDCAN_FILTER_TO_RXFIFO1 : FDCAN_FILTER_TO_RXFIFO0;
        filter.FilterID1 = m_filterID;
        filter.FilterID2 = m_filterMask;
        if (HAL_FDCAN_ConfigFilter(m_handle, &filter) != HAL_OK) return false;
        if (HAL_FDCAN_ConfigGlobalFilter(m_handle, FDCAN_REJECT, FDCAN_REJECT,
                FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE) != HAL_OK) return false;
        if (HAL_FDCAN_Start(m_handle) != HAL_OK) return false;
        const uint32_t notification = m_fifo
            ? FDCAN_IT_RX_FIFO1_NEW_MESSAGE : FDCAN_IT_RX_FIFO0_NEW_MESSAGE;
        return HAL_FDCAN_ActivateNotification(m_handle, notification, 0U) == HAL_OK;
    }
};
