#define BLYNK_TEMPLATE_ID   "TMPL6lwabRHYO"
#define BLYNK_TEMPLATE_NAME "TheLeduylamCloudVN ESP32"
#define BLYNK_AUTH_TOKEN    "Sp-tWtHRjrqfnunqZvcQHzC8cOSDp_0b"

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

char ssid[] = "Leduylam";
char pass[] = "Leduylam@271213";

#define LED_PIN     10
#define RELAY_PIN   7
#define DHTPIN      2
#define SOIL_PIN    3
#define VIB_PIN     5
#define NTC_PIN     6
#define RAIN_PIN    1
#define BUZZ_PIN    20
#define DHTTYPE     DHT11

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);
BlynkTimer timer;

bool relayState = false;
bool manualBuzz = false;
bool buzzEnable = true; // V12: Bật/Tắt còi hệ thống
int lcdPage = 0;
unsigned long lastNotify = 0;

// V12: Nút cho phép còi kêu hay không
BLYNK_WRITE(V12) { buzzEnable = param.asInt(); }
BLYNK_WRITE(V11) { manualBuzz = param.asInt(); }
BLYNK_WRITE(V2)  { digitalWrite(LED_PIN, param.asInt()); }
BLYNK_WRITE(V6)  { relayState = param.asInt(); digitalWrite(RELAY_PIN, relayState); }

void sendNotify(String msg) {
  if (millis() - lastNotify > 60000) { // Giới hạn 1 phút gửi 1 lần tránh spam
    Blynk.logEvent("alarm", msg); 
    lastNotify = millis();
  }
}

void checkAlarms() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  bool isRain = !digitalRead(RAIN_PIN);
  bool isVib = !digitalRead(VIB_PIN);
  bool isHotNTC = !digitalRead(NTC_PIN); // NTC ngưỡng cứng từ biến trở

  bool trigger = false;
  String reason = "";

  // 1. Kiểm tra nhiệt độ NTC hoặc DHT11 > 75C
  if (isHotNTC || t >= 75) { trigger = true; reason = "NHIET DO QUA CAO!"; }
  // 2. Kiểm tra Mưa
  if (isRain) { trigger = true; reason = "TROII DANG MUA!"; }
  // 3. Kiểm tra Rung
  if (isVib) { trigger = true; reason = "CANH BAO RUNG!"; }
  // 4. Kiểm tra Độ ẩm bất thường (ví dụ > 95%)
  if (h > 95) { trigger = true; reason = "DO AM QUA CAO!"; }

  // Xử lý Còi
  if ((trigger || manualBuzz) && buzzEnable) {
    digitalWrite(BUZZ_PIN, HIGH);
    if (trigger) sendNotify(reason);
  } else {
    digitalWrite(BUZZ_PIN, LOW);
  }

  // Cập nhật trạng thái lên App
  Blynk.virtualWrite(V8, isVib ? "RUNG!" : "OK");
  Blynk.virtualWrite(V9, isRain ? "MUA!" : "KHO");
}

void updateLCD() {
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int soil = map(analogRead(SOIL_PIN), 4095, 0, 0, 100);
  
  Blynk.virtualWrite(V3, t);
  Blynk.virtualWrite(V4, h);
  Blynk.virtualWrite(V5, constrain(soil, 0, 100));

  lcd.setCursor(0, 0);
  if (lcdPage == 0) {
    lcd.print("T:"); lcd.print((int)t); lcd.print("C H:"); lcd.print((int)h); lcd.print("%   ");
    lcd.setCursor(0, 1);
    lcd.print("Buzz:"); lcd.print(buzzEnable ? "ON " : "OFF"); 
    lcd.print(" R:"); lcd.print(relayState ? "ON " : "OFF");
    lcdPage = 1;
  } else {
    lcd.print("Soil:"); lcd.print(soil); lcd.print("% Rain:"); lcd.print(!digitalRead(RAIN_PIN)? "Y":"N");
    lcd.setCursor(0, 1);
    lcd.print("System Online   ");
    lcdPage = 0;
  }
}

void setup() {
  Serial.begin(115200);
  Wire.begin(8, 9);
  lcd.init(); lcd.backlight();
  
  pinMode(BUZZ_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT);
  pinMode(VIB_PIN, INPUT_PULLUP);
  pinMode(NTC_PIN, INPUT_PULLUP);
  pinMode(RAIN_PIN, INPUT_PULLUP);

  dht.begin();
  
  // Chống Offline: Cấu hình trước, kết nối sau
  WiFi.begin(ssid, pass);
  Blynk.config(BLYNK_AUTH_TOKEN); 
  
  timer.setInterval(2000L, updateLCD);
  timer.setInterval(500L, checkAlarms);
}

void loop() {
  Blynk.run();
  timer.run();
}