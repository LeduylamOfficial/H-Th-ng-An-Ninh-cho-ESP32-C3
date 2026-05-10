# 🚀 TheLeduylamCloudVN - Blynk IoT Control System

Hệ thống quản lý và điều khiển thiết bị qua Internet sử dụng **ESP32** và nền tảng **Blynk IoT**. Dự án hỗ trợ theo dõi nhiệt độ, độ ẩm và điều khiển thiết bị điện (Relay) từ xa.

## 📋 Mục lục
* [Tính năng](#-tính-năng)
* [Linh kiện cần thiết](#-linh-kiện-cần-thiết)
* [Sơ đồ đấu nối](#-sơ-đồ-đấu-nối)
* [Hướng dẫn cài đặt](#-hướng-dẫn-cài-đặt)
* [Thư viện sử dụng](#-thư-viện-sử-dụng)

---

## ✨ Tính năng
- [x] Điều khiển Đèn LED/Relay qua App điện thoại.
- [x] Cập nhật dữ liệu Nhiệt độ/Độ ẩm thời gian thực.
- [x] Giao diện Dashboard hiện đại trên Web và Mobile.
- [x] Tự động kết nối lại khi mất WiFi.

## 📦 Linh kiện cần thiết
| Linh kiện | Số lượng | Chú thích |
| :--- | :---: | :--- |
| **ESP32 DevKit V1** | 01 | Board điều khiển chính |
| **Cảm biến DHT11/22** | 01 | Đo nhiệt độ & độ ẩm |
| **Module Relay 5V** | 01 | Điều khiển thiết bị 220V |
| **LED & Điện trở** | 01 | Hiển thị trạng thái |

## 🔌 Sơ đồ đấu nối


| ESP32 Pin | Thiết bị | Chân thiết bị |
| :--- | :--- | :--- |
| **3V3** | DHT11/Relay | VCC |
| **GND** | Tất cả | GND |
| **GPIO 4** | DHT11 | Data |
| **GPIO 5** | Relay | IN |
| **GPIO 2** | LED | Anode (+) |

## 🛠 Hướng dẫn cài đặt

### 1. Cấu hình Blynk Cloud
1. Truy cập [Blynk.cloud](https://blynk.cloud) tạo tài khoản.
2. Tạo **Template** mới với phần cứng là ESP32.
3. Thiết lập **Datastreams**:
   - V2: Integer (LED)
   - V3: Double (Temperature)
   - V5: Integer (Relay)

### 2. Lập trình Arduino
1. Tải và cài đặt [Arduino IDE](https://www.arduino.cc/en/software).
2. Cài đặt các thư viện cần thiết (xem mục bên dưới).
3. Copy **Auth Token** từ Blynk vào code.
4. Nạp code vào ESP32.

## 📚 Thư viện sử dụng
Dự án sử dụng các thư viện sau (có thể cài đặt qua Library Manager):
* `Blynk` by Volodymyr Shymanskyy
* `DHT sensor library` by Adafruit
* `Adafruit Unified Sensor` by Adafruit

---
© 2026 **TheLeduylamCloudVN**. Được phát triển bởi Duy Lâm.
