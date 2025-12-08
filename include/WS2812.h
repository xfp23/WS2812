/**
 * @file WS2812.h
 * @brief WS2812 LED 灯带驱动库公共接口。
 *
 * @note 需要spi 6.4M的spi频率 CPOL = 0 CPHA = 2Edge
 * 提供初始化、颜色设置、批量控制、清除与显示等基本功能。
 * 所有接口均为非阻塞，不负责实际的 SPI/IO 时序，由用户通过
 * WS2812_Conf_t 中的回调函数提供底层传输实现。
 */

#ifndef __WS2812_H_
#define __WS2812_H_

#include "WS2812_types.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 初始化 WS2812 控制句柄。
 *
 * 该函数会分配内部对象并保存底层传输接口。
 *
 * @param[out] handle   WS2812 句柄指针，初始化后返回非 NULL 对象。
 * @param[in]  conf     配置结构体，包含 LED 数量和底层发送函数。
 *
 * @return
 *      - WS2812_OK     成功
 *      - WS2812_ERR    参数错误或内存申请失败
 */
WS2812_Status_t WS2812_Init(WS2812_Handle_t *handle, WS2812_Conf_t *conf);

/**
 * @brief 设置指定灯珠的 RGB 颜色，不会立即刷新。
 *
 * @param[in] handle    WS2812 句柄。
 * @param[in] index     灯珠索引（0 ~ n-1）。
 * @param[in] r         红色分量（0–255）。
 * @param[in] g         绿色分量（0–255）。
 * @param[in] b         蓝色分量（0–255）。
 *
 * @return
 *      - WS2812_OK
 *      - WS2812_ERR
 */
WS2812_Status_t WS2812_SetPixel(WS2812_Handle_t handle,
                                uint16_t index,
                                uint8_t r,
                                uint8_t g,
                                uint8_t b);

/**
 * @brief 批量设置多个灯珠为同一颜色。
 *
 * 其余未在 idx_list 中的灯保持不变。
 *
 * @param[in] handle    WS2812 句柄。
 * @param[in] idx_list  灯号数组。
 * @param[in] count     数组元素数。
 * @param[in] r         红色分量（0–255）。
 * @param[in] g         绿色分量（0–255）。
 * @param[in] b         蓝色分量（0–255）。
 *
 * @return
 *      - WS2812_OK
 *      - WS2812_ERR
 */
WS2812_Status_t WS2812_SetMulti(WS2812_Handle_t handle,
                                const uint16_t *idx_list,
                                size_t count,
                                uint8_t r,
                                uint8_t g,
                                uint8_t b);

/**
 * @brief 清空所有灯珠（全部置零），不会立即刷新。
 *
 * @param[in] handle WS2812 句柄。
 *
 * @return
 *      - WS2812_OK
 *      - WS2812_ERR
 */
WS2812_Status_t WS2812_Clear(WS2812_Handle_t handle);

/**
 * @brief 将内部颜色缓存一次性发送给灯带（异步发送）。
 *
 * 必须调用本函数才能真正点亮 LED。
 *
 * @param[in] handle WS2812 句柄。
 *
 * @return
 *      - WS2812_OK
 *      - WS2812_ERR
 */
WS2812_Status_t WS2812_ShowAsync(WS2812_Handle_t handle);

/**
 * @brief 将内部颜色缓存一次性发送给灯带（同步发送）。
 *
 * 必须调用本函数才能真正点亮 LED。
 *
 * @param[in] handle WS2812 句柄。
 *
 * @return
 *      - WS2812_OK
 *      - WS2812_ERR
 */
WS2812_Status_t WS2812_ShowSync(WS2812_Handle_t handle);

#ifdef __cplusplus
}
#endif

#endif /* __WS2812_H_ */
