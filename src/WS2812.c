#include "WS2812.h"

/* SPI Hack：每个 WS2812 bit 需要映射成 3~4bit SPI 数据 */
static const uint8_t WS_HIGH = 0xF0;   /* 代表逻辑 1  0b11110000 624ns (1) 624ns (0)*/
static const uint8_t WS_LOW  = 0xC0;   /* 代表逻辑 0  0b11000000 312ns (1) 936ns (0)*/
// 

/* 写一个字节 → 自动展开 WS2812 的 8bit */
static WS2812_Status_t WS2812_WriteByte(WS2812_Handle_t handle, uint8_t byte)
{
    uint8_t buf[8];
    for (int i = 0; i < 8; i++)
    {
        if (byte & (1 << (7 - i)))
            buf[i] = WS_HIGH;
        else
            buf[i] = WS_LOW;
    }

    if (handle->spi_Synctrans((uint8_t *)buf, 8) != 0)
        return WS2812_ERR;

    return WS2812_OK;
}

WS2812_Status_t WS2812_Init(WS2812_Handle_t *handle, WS2812_Conf_t *conf)
{
    if (handle == NULL || conf == NULL || *handle != NULL)
        return WS2812_ERR;

    if (conf->spi_Synctrans == NULL && conf->spi_AsyncTrans == NULL)
        return WS2812_ERR;

    WS2812_Handle_t obj = (WS2812_Handle_t)calloc(1, sizeof(WS2812_Obj_t));
    if (obj == NULL)
        return WS2812_ERR;

    obj->buffer = (WS2812_RGB_t *)calloc(conf->led_num, sizeof(WS2812_RGB_t));
    if (obj->buffer == NULL)
    {
        free(obj);
        return WS2812_ERR;
    }

    obj->led_num = conf->led_num;
    obj->spi_Synctrans = conf->spi_Synctrans;
	obj->spi_AsyncTrans = conf->spi_AsyncTrans;

    *handle = obj;

    return WS2812_OK;
}

WS2812_Status_t WS2812_SetPixel(WS2812_Handle_t handle, uint16_t index,
                                uint8_t r, uint8_t g, uint8_t b)
{
    if (handle == NULL || index >= handle->led_num)
        return WS2812_ERR;

    memset(handle->buffer,0,sizeof(WS2812_RGB_t) * handle->led_num);
    handle->buffer[index].bits.r = r;
    handle->buffer[index].bits.g = g;
    handle->buffer[index].bits.b = b;

    return WS2812_OK;
}

WS2812_Status_t WS2812_SetMulti(WS2812_Handle_t handle, const uint16_t *idx_list, size_t count,
                                uint8_t r, uint8_t g, uint8_t b)
{
    if (handle == NULL)
        return WS2812_ERR;

    /* 全灭 */
    for (uint16_t i = 0; i < handle->led_num; i++)
    {
        handle->buffer[i].bits.r = 0;
        handle->buffer[i].bits.g = 0;
        handle->buffer[i].bits.b = 0;
    }

    /* 点亮指定的 */
    for (size_t i = 0; i < count; i++)
    {
        uint16_t idx = idx_list[i];
        if (idx < handle->led_num)
        {
            handle->buffer[idx].bits.r = r;
            handle->buffer[idx].bits.g = g;
            handle->buffer[idx].bits.b = b;
        }
    }

    return WS2812_OK;
}

WS2812_Status_t WS2812_Clear(WS2812_Handle_t handle)
{
    if (handle == NULL)
        return WS2812_ERR;

    memset(handle->buffer, 0, handle->led_num * sizeof(WS2812_RGB_t));
    return WS2812_OK;
}

WS2812_Status_t WS2812_ShowSync(WS2812_Handle_t handle)
{
    if (handle == NULL || handle->spi_Synctrans == NULL)
        return WS2812_ERR;

    /* GRB → SPI */
    for (uint16_t i = 0; i < handle->led_num; i++)
    {
        WS2812_WriteByte(handle, handle->buffer[i].bits.g);
        WS2812_WriteByte(handle, handle->buffer[i].bits.r);
        WS2812_WriteByte(handle, handle->buffer[i].bits.b);
    }

    /* Reset > 280us，低电平保持 */
    uint8_t zero = 0;
    for (int i = 0; i < 240; i++)   // 240 × 1.25us = 300us
        handle->spi_Synctrans(&zero, 1);

    return WS2812_OK;
}

WS2812_Status_t WS2812_ShowAsync(WS2812_Handle_t handle)
{
    if (handle == NULL || handle->spi_AsyncTrans == NULL)
        return WS2812_ERR;

    uint16_t led_num = handle->led_num;
    if (led_num == 0)
        return WS2812_OK;

    /* 每个 LED 3 字节 × 每字节 8 映射字节 */
    size_t spi_len = led_num * 3 * 8;
    uint8_t *spi_buf = (uint8_t *)malloc(spi_len);
    if (spi_buf == NULL)
        return WS2812_ERR;

    uint8_t *p = spi_buf;
    for (uint16_t i = 0; i < led_num; i++)
    {
        uint8_t colors[3] = { handle->buffer[i].bits.g,
                              handle->buffer[i].bits.r,
                              handle->buffer[i].bits.b };

        for (int c = 0; c < 3; c++)
        {
            uint8_t byte = colors[c];
            for (int bit = 7; bit >= 0; bit--)
            {
                if (byte & (1 << bit))
                    *p++ = WS_HIGH;
                else
                    *p++ = WS_LOW;
            }
        }
    }

    /* 一次性发送整个 LED 数据 */
    if (handle->spi_AsyncTrans(spi_buf, spi_len) != 0)
    {
        free(spi_buf);
        return WS2812_ERR;
    }

    free(spi_buf);

    /* Reset > 280us，低电平保持 */
    uint8_t zero = 0;
    for (int i = 0; i < 240; i++)   // 240 × 1.25us = 300us
        handle->spi_AsyncTrans(&zero, 1);

    return WS2812_OK;
}
