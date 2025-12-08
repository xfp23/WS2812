#ifndef __WS2812_TYPES_H_
#define __WS2812_TYPES_H_

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#ifdef __cplusplus
extern "C"
{
#endif

typedef enum
{
    WS2812_OK = 0,
    WS2812_ERR = 1,
} WS2812_Status_t;

/**
 * @brief SPI 单字节发送函数，由用户注册
 * return 0/WS2812_OK 表示成功
 */
typedef int (*WS2812_TransmitFunc_t)(uint8_t *data, size_t len);

/* GRB 排列 */
typedef union
{
    struct
    {
        uint8_t g;
        uint8_t r;
        uint8_t b;
    } bits;

    uint8_t data[3];
} WS2812_RGB_t;

/* 配置结构体 */
typedef struct
{
    uint16_t led_num;                     /* 灯珠数量 */
    WS2812_TransmitFunc_t spi_Synctrans;  /*  同步发送 */
    WS2812_TransmitFunc_t spi_AsyncTrans; /* 异步发送  */
} WS2812_Conf_t;

/* 句柄内部对象 */
typedef struct
{
    uint16_t led_num;
    WS2812_RGB_t *buffer;             /* 存所有灯珠的 GRB 数组 */
    WS2812_TransmitFunc_t spi_Synctrans;  /*  同步发送 */
    WS2812_TransmitFunc_t spi_AsyncTrans; /* 异步发送  */

} WS2812_Obj_t;

typedef WS2812_Obj_t *WS2812_Handle_t;

#ifdef __cplusplus
}
#endif

#endif
