# WS2812 LED Driver Library

基于 SPI 的 WS2812 / WS2812B / SK6812 等灯带驱动库（C 语言）

本库通过 **SPI 6.4 MHz** 输出 WS2812 所需的时序，将 RGB 数据编码为 SPI 字节流，由用户提供底层传输回调函数。

---

## ⚙️ 使用方法

### 1. 定义底层发送函数（SPI/DMA）

```c
// 示例：SPI DMA 发送函数
void my_spi_send_dma(const uint8_t *data, size_t len)
{
    HAL_SPI_Transmit_DMA(&hspi1, (uint8_t *)data, len);
}
```

### 2. 初始化 WS2812

```c
WS2812_Handle_t hws;
WS2812_Conf_t conf = {
    .led_count = 30,
    .spi_AsyncTrans = my_spi_send_dma,
};

WS2812_Init(&hws, &conf);
```

### 3. 设置颜色

```c
WS2812_SetPixel(hws, 0, 255, 0, 0);      // 第0颗 LED 红色
WS2812_SetPixel(hws, 1, 0, 255, 0);      // 绿色
WS2812_SetPixel(hws, 2, 0, 0, 255);      // 蓝色
```

### 4. 刷新显示

```c
WS2812_ShowAsync(hws);   // 异步刷新（推荐，走 DMA）
```

或同步发送：

```c
WS2812_ShowSync(hws);    // 阻塞刷新
```

---

## 🚀 SPI 频率要求

* **必须配置 SPI 为 6.4 MHz（误差 ±1%）**
* SPI 模式：**Mode 0（CPOL=0, CPHA=0）**
* 8 bit data frame
* 空闲时输出应为 **低电平**

---

## 🧩 接口说明（总结）

### `WS2812_Init()`

初始化句柄并分配内部缓存。

### `WS2812_SetPixel()`

设置单个 LED RGB（不会立即更新）。

### `WS2812_SetMulti()`

批量设置多个 LED 为相同颜色。

### `WS2812_Clear()`

将全部 LED 颜色清零（不会立即更新）。

### `WS2812_ShowAsync()`

异步刷新（适合 DMA）。

### `WS2812_ShowSync()`

同步刷新（阻塞方式）。

---

## 📄 许可证

[许可证](./LICENSE)

---
