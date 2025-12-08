# WS2812 LED Driver Library

基于 SPI 的 WS2812 / WS2812B / SK6812 等灯带驱动库（C 语言）

本库通过 **SPI 6.4 MHz** 输出 WS2812 所需的时序，将 RGB 数据编码为 SPI 字节流，由用户提供底层传输回调函数，库本身不依赖具体芯片，可轻松移植到 STM32 / ESP32 / N32 / SKM32 等任意 MCU。

提供的接口包含单灯设置、批量设置、清空、异步刷新、同步刷新等，API 简洁，适合集成到任何嵌入式项目。

---

## ✨ 特性

* ✔ 使用 **SPI 外设**模拟 WS2812 波形
* ✔ 完全 **非阻塞 API**（由用户底层回调决定是否阻塞）
* ✔ 支持 **任意数量的 LED**
* ✔ 支持 **单灯设置 / 批量设置 / 清空缓存**
* ✔ 支持 **异步刷新（DMA）** 和 **同步刷新（阻塞）**
* ✔ 完全无硬件耦合，轻量可移植
* ✔ 代码结构简单，适合嵌入式量产工程

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
    .send_func = my_spi_send_dma,
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
