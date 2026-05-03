#include <WiFi.h>#include <WebServer.h>#include <WebSocketsServer.h>#include "esp_camera.h"#include <ESP_Mail_Client.h>

// ===================== WiFi =====================#define WIFI_SSID "ceer-2"#define WIFI_PASSWORD "ceer@301"

// ===================== Gmail =====================#define SMTP_HOST "smtp.gmail.com"#define SMTP_PORT 465#define AUTHOR_EMAIL "kanchinikhil594@gmail.com"#define AUTHOR_PASSWORD "cvnb hllt mvfe vccx"#define RECIPIENT1 "kanchinikhil588@gmail.com"#define RECIPIENT2 "24h51a67k2@cmrcet.ac.in"#define RECIPIENT3 "24h51a67d6@cmrcet.ac.in"

// ===================== Button =====================#define BUTTON_PIN 2

// ===================== Camera Pins =====================#define PWDN_GPIO_NUM    -1#define RESET_GPIO_NUM   -1#define XCLK_GPIO_NUM    10#define SIOD_GPIO_NUM    40#define SIOC_GPIO_NUM    39#define Y9_GPIO_NUM      48#define Y8_GPIO_NUM      11#define Y7_GPIO_NUM      12#define Y6_GPIO_NUM      14#define Y5_GPIO_NUM      16#define Y4_GPIO_NUM      18#define Y3_GPIO_NUM      17#define Y2_GPIO_NUM      15#define VSYNC_GPIO_NUM   38#define HREF_GPIO_NUM    47#define PCLK_GPIO_NUM    13

// ===================== Servers =====================WebServer server(80);WebSocketsServer webSocket(81);SMTPSession smtp;

// ===================== Location =====================String latitude = "0.0";String longitude = "0.0";

// ===================== Location Receiver =====================void handleUpdate() {if (server.hasArg("lat") && server.hasArg("lon")) {latitude = server.arg("lat");longitude = server.arg("lon");

Serial.println("📍 LOCATION RECEIVED");
Serial.print("Latitude: ");
Serial.println(latitude);
Serial.print("Longitude: ");
Serial.println(longitude);

server.send(200, "text/plain", "Location Updated");

} else {server.send(400, "text/plain", "Missing Parameters");}}

// ===================== Send Email =====================bool sendEmail() {camera_fb_t *fb = esp_camera_fb_get();if (!fb) {Serial.println("❌ Camera failed!");return false;}

Session_Config config;config.server.host_name = SMTP_HOST;config.server.port = SMTP_PORT;config.login.email = AUTHOR_EMAIL;config.login.password = AUTHOR_PASSWORD;

smtp.connect(&config);

SMTP_Message message;message.sender.name = "SAFON ALERT";message.sender.email = AUTHOR_EMAIL;message.subject = "🚨 Emergency Alert with Location";

message.addRecipient("User1", RECIPIENT1);message.addRecipient("User2", RECIPIENT2);message.addRecipient("User3", RECIPIENT3);

// ---- Attach Image ----SMTP_Attachment att;att.descr.filename = "alert.jpg";att.descr.mime = "image/jpeg";att.blob.data = fb->buf;att.blob.size = fb->len;message.addAttachment(att);

// ---- Google Maps Link (UPDATED) ----String mapLink = "https://maps.google.com/?q=" + latitude + "," + longitude;

String html = "🚨 Emergency Alert";html += "Live Location Link:";html += "" + mapLink + "";

message.html.content = html.c_str();

bool success = MailClient.sendMail(&smtp, &message);

esp_camera_fb_return(fb);

return success;}

// ===================== Setup =====================void setup() {Serial.begin(115200);pinMode(BUTTON_PIN, INPUT_PULLUP);

Serial.println("🚀 Starting...");

WiFi.begin(WIFI_SSID, WIFI_PASSWORD);while (WiFi.status() != WL_CONNECTED) {delay(500);Serial.print(".");}

Serial.println("\n✅ WiFi Connected");Serial.print("🌐 ESP32 IP Address: ");Serial.println(WiFi.localIP());

server.on("/update", handleUpdate);server.begin();webSocket.begin();

// Camera Setupcamera_config_t config;config.ledc_channel = LEDC_CHANNEL_0;config.ledc_timer = LEDC_TIMER_0;config.pin_d0 = Y2_GPIO_NUM;config.pin_d1 = Y3_GPIO_NUM;config.pin_d2 = Y4_GPIO_NUM;config.pin_d3 = Y5_GPIO_NUM;config.pin_d4 = Y6_GPIO_NUM;config.pin_d5 = Y7_GPIO_NUM;config.pin_d6 = Y8_GPIO_NUM;config.pin_d7 = Y9_GPIO_NUM;config.pin_xclk = XCLK_GPIO_NUM;config.pin_pclk = PCLK_GPIO_NUM;config.pin_vsync = VSYNC_GPIO_NUM;config.pin_href = HREF_GPIO_NUM;config.pin_sccb_sda = SIOD_GPIO_NUM;config.pin_sccb_scl = SIOC_GPIO_NUM;config.pin_pwdn = PWDN_GPIO_NUM;config.pin_reset = RESET_GPIO_NUM;config.xclk_freq_hz = 20000000;config.pixel_format = PIXFORMAT_JPEG;config.frame_size = FRAMESIZE_VGA;config.jpeg_quality = 12;config.fb_count = 1;

if (esp_camera_init(&config) != ESP_OK) {Serial.println("❌ Camera init failed!");while (1);}

Serial.println("✅ Camera Ready");}

// ===================== Loop =====================void loop() {server.handleClient();webSocket.loop();

if (digitalRead(BUTTON_PIN) == LOW) {Serial.println("\n🚨 BUTTON PRESSED!");Serial.print("🌐 ESP32 IP: ");Serial.println(WiFi.localIP());

webSocket.broadcastTXT("vacation");

Serial.println("⏳ Waiting for location...");
delay(5000);

bool status = sendEmail();

if (status) {
  Serial.println("✅ Email Sent Successfully!");
} else {
  Serial.println("❌ Email Failed to Send!");
}

Serial.print("📍 Last Location Used: ");
Serial.println(latitude + ", " + longitude);

delay(10000);

}}
