#pragma once
#include "main.h"
//-------------------------------------------------------------------------------------------

class FDCAN_BUS_PCA82C251T {
public:
    // Constructor to initialize the CAN interface
    FDCAN_BUS_PCA82C251T(FDCAN_HandleTypeDef *hfdcan, uint8_t fifoNum, uint32_t filterID1 = 0x321, uint32_t filterID2 = 0x7FF)
        : hfdcan(hfdcan), FIFO_Num(fifoNum), defaultFilterID1(filterID1), defaultFilterID2(filterID2) {
        Configure();
    }

    // Method to send a CAN message
    uint8_t SendMessage(uint32_t id, uint32_t len, uint8_t *buf) {
        uint32_t offset = 0;
        FDCAN_TxHeaderTypeDef TxHeader;
        HAL_StatusTypeDef HAL_RetVal;

        TxHeader.Identifier = id;
        TxHeader.IdType = FDCAN_STANDARD_ID;
        TxHeader.TxFrameType = FDCAN_DATA_FRAME;
        TxHeader.DataLength = FDCAN_DLC_BYTES_8;
        TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
        TxHeader.BitRateSwitch = FDCAN_BRS_OFF;
        TxHeader.FDFormat = FDCAN_CLASSIC_CAN;
        TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
        TxHeader.MessageMarker = 0;

        while (len != 0) {
            TxHeader.DataLength = (len > 8 ? 8 : len) * 0x10000; // Data length
            do {
                osDelay(1);
                HAL_RetVal = HAL_FDCAN_AddMessageToTxFifoQ(hfdcan, &TxHeader, buf + offset); // Send a frame of data
            } while (HAL_RetVal != HAL_OK);
            offset += TxHeader.DataLength / 0x10000;
            len -= TxHeader.DataLength / 0x10000;
        }
        return 0;
    }

    // Callback method for receiving messages
    void ReceiveMessageCallback(uint32_t RxFifoITs) {
        if ((RxFifoITs & FDCAN_IT_RX_FIFO0_NEW_MESSAGE) != RESET) {
            uint8_t Rxbuff[8];
            FDCAN_RxHeaderTypeDef RxHeader;

            if (HAL_FDCAN_GetRxMessage(hfdcan, FDCAN_RX_FIFO0, &RxHeader, Rxbuff) == HAL_OK) {
                if ((RxHeader.Identifier == defaultFilterID1) && (RxHeader.IdType == FDCAN_STANDARD_ID)) {
                    for (int i = 0; i < (RxHeader.DataLength / 0x10000); i++) {
                        Can_buff_push(Rxbuff[i]); // Assuming Can_buff_push is accessible here
                    }
                }
            } else {
                Error_Handler(); // Handle reception error
            }
        }
    }

private:
    // Method to configure the CAN settings
    void Configure() {
        FDCAN_FilterTypeDef CAN_FilterType;
        CAN_FilterType.RxBufferIndex = 0;
        CAN_FilterType.IdType = FDCAN_STANDARD_ID;
        CAN_FilterType.FilterIndex = 0;
        CAN_FilterType.FilterType = FDCAN_FILTER_MASK;
        CAN_FilterType.FilterConfig = (FIFO_Num) ? FDCAN_FILTER_TO_RXFIFO1 : FDCAN_FILTER_TO_RXFIFO0;
        CAN_FilterType.FilterID1 = defaultFilterID1;
        CAN_FilterType.FilterID2 = defaultFilterID2;

        if (HAL_FDCAN_ConfigFilter(hfdcan, &CAN_FilterType) != HAL_OK) {
            Error_Handler(); // Handle filter configuration error
        }

        HAL_FDCAN_ConfigGlobalFilter(hfdcan, FDCAN_REJECT, FDCAN_REJECT, FDCAN_REJECT_REMOTE, FDCAN_REJECT_REMOTE);
        
        if (HAL_FDCAN_Start(hfdcan) != HAL_OK) {
            Error_Handler(); // Handle start error
        }

        if (FIFO_Num) {
            HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO1_NEW_MESSAGE, 0);
        } else {
            HAL_FDCAN_ActivateNotification(hfdcan, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0);
        }
    }

    // Private member variables
    FDCAN_HandleTypeDef *hfdcan;       // Pointer to the CAN handle
    uint8_t FIFO_Num;                   // FIFO selection (0 or 1)
    uint32_t defaultFilterID1;          // Default filter ID 1
    uint32_t defaultFilterID2;          // Default filter ID 2
};






