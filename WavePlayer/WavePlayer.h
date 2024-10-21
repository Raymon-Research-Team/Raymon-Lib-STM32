#pragma once

#include "stm32f1xx_hal.h"
#include "fatfs.h"

#define BUFFER_SIZE 512

class WavePlayer {
public:
    enum OutputType {
        OUTPUT_DAC,
        OUTPUT_PWM
    };

    // سازنده کلاس: تنظیمات تایمر و PWM
    WavePlayer(OutputType output, TIM_HandleTypeDef* playbackTimer = nullptr, TIM_HandleTypeDef* pwmTimer = nullptr, uint32_t pwmChannel = TIM_CHANNEL_1)
        : outputType(output)
#ifdef USE_PWM
        , htim_pwm(pwmTimer)
        , pwmChannel(pwmChannel)
#endif
        , htim_playback(playbackTimer)
        , isPaused(false)
        , isStopped(false)
        , sampleRate(0) {}

    void initDAC(uint32_t sampleRate) {
#ifdef USE_DAC
        if (outputType == OUTPUT_DAC) {
            __HAL_RCC_DAC_CLK_ENABLE();
            DAC_ChannelConfTypeDef sConfig = { 0 };
            hdac.Instance = DAC;
            HAL_DAC_Init(&hdac);
            sConfig.DAC_Trigger = DAC_TRIGGER_T6_TRGO;
            sConfig.DAC_OutputBuffer = DAC_OUTPUTBUFFER_ENABLE;
            HAL_DAC_ConfigChannel(&hdac, &sConfig, DAC_CHANNEL_1);
            HAL_DAC_Start(&hdac, DAC_CHANNEL_1);
        }
#endif
    }

    void initPWM(uint32_t sampleRate) {
#ifdef USE_PWM
        if (outputType == OUTPUT_PWM && htim_pwm != nullptr) {
            htim_pwm->Init.Prescaler = HAL_RCC_GetPCLK1Freq() / (sampleRate * 255) - 1;
            htim_pwm->Init.CounterMode = TIM_COUNTERMODE_UP;
            htim_pwm->Init.Period = 255;
            HAL_TIM_PWM_Init(htim_pwm);

            TIM_OC_InitTypeDef sConfigOC = { 0 };
            sConfigOC.OCMode = TIM_OCMODE_PWM1;
            sConfigOC.Pulse = 0;
            HAL_TIM_PWM_ConfigChannel(htim_pwm, &sConfigOC, pwmChannel);
            HAL_TIM_PWM_Start(htim_pwm, pwmChannel);
        }
#endif
    }

    uint32_t readWavHeader(const char* filename, uint32_t* sampleRate) {
        FIL file;
        if (f_open(&file, filename, FA_READ) == FR_OK) {
            uint8_t header[44];
            UINT bytesRead;
            f_read(&file, header, sizeof(header), &bytesRead);
            *sampleRate = *(uint32_t*)&header[24];
            f_close(&file);
            return 1;
        }
        return 0;
    }

    void play(const char* filename) {
        if (f_open(&file, filename, FA_READ) == FR_OK) {
            if (readWavHeader(filename, &sampleRate)) {
#ifdef USE_DAC
                if (outputType == OUTPUT_DAC) {
                    initDAC(sampleRate);
                }
#endif
#ifdef USE_PWM
                if (outputType == OUTPUT_PWM) {
                    initPWM(sampleRate);
                }
#endif
                f_read(&file, audioBuffer, BUFFER_SIZE, &bytesRead);
#ifdef USE_DAC
                if (outputType == OUTPUT_DAC) {
                    outputToDAC(audioBuffer, bytesRead);
                }
#endif
#ifdef USE_PWM
                if (outputType == OUTPUT_PWM && htim_pwm != nullptr) {
                    outputToPWM(audioBuffer, bytesRead);
                }
#endif
                startPlaybackTimer();
            }
        }
    }

    void pause() {
        isPaused = true;
    }

    void resume() {
        isPaused = false;
    }

    void stop() {
        isStopped = true;
        stopPlaybackTimer(); // توقف تایمر
    }

    void writeToFile(const char* filename, const char* data) {
        FIL fileToWrite;
        f_open(&fileToWrite, filename, FA_WRITE | FA_CREATE_ALWAYS);
        UINT bytesWritten;
        f_write(&fileToWrite, data, strlen(data), &bytesWritten);
        f_close(&fileToWrite);
    }

    void readFromFile(const char* filename, char* buffer, UINT bufferSize) {
        FIL fileToRead;
        f_open(&fileToRead, filename, FA_READ);
        UINT bytesRead;
        f_read(&fileToRead, buffer, bufferSize, &bytesRead);
        f_close(&fileToRead);
    }

#ifdef USE_DAC
    void outputToDAC(uint8_t* data, uint32_t length) {
        if (outputType == OUTPUT_DAC) {
            HAL_DAC_Start_DMA(&hdac, DAC_CHANNEL_1, (uint32_t*)data, length, DAC_ALIGN_8B_R);
        }
    }
#endif

#ifdef USE_PWM
    void outputToPWM(uint8_t* data, uint32_t length) {
        if (outputType == OUTPUT_PWM && htim_pwm != nullptr) {
            for (uint32_t i = 0; i < length; ++i) {
                __HAL_TIM_SET_COMPARE(htim_pwm, pwmChannel, data[i]);
                HAL_Delay(1);
            }
        }
    }
#endif

#ifdef USE_DAC
    void HAL_DAC_ConvCpltCallbackCh1(DAC_HandleTypeDef* hdac) {
        if (!isPaused && !isStopped) {
            f_read(&file, audioBuffer, BUFFER_SIZE, &bytesRead);
            outputToDAC(audioBuffer, bytesRead);
        }
    }
#endif

    void update() {
        if (!isPaused && !isStopped) {
            f_read(&file, audioBuffer, BUFFER_SIZE, &bytesRead);
#ifdef USE_DAC
            if (outputType == OUTPUT_DAC) {
                outputToDAC(audioBuffer, bytesRead);
            }
#endif
#ifdef USE_PWM
            if (outputType == OUTPUT_PWM && htim_pwm != nullptr) {
                outputToPWM(audioBuffer, bytesRead);
            }
#endif
        }
    }

private:
    OutputType outputType;
    volatile bool isPaused;
    volatile bool isStopped;
#ifdef USE_DAC
    DAC_HandleTypeDef hdac;
#endif
#ifdef USE_PWM
    TIM_HandleTypeDef* htim_pwm;
    uint32_t pwmChannel;
#endif
    TIM_HandleTypeDef* htim_playback;
    FIL file;
    uint8_t audioBuffer[BUFFER_SIZE];
    UINT bytesRead;
    uint32_t sampleRate;
    
    void startPlaybackTimer() {
        if (htim_playback != nullptr) {
            HAL_TIM_Base_Start_IT(htim_playback);
        }
    }

    void stopPlaybackTimer() {
        if (htim_playback != nullptr) {
            HAL_TIM_Base_Stop_IT(htim_playback);
        }
    }
    
};

