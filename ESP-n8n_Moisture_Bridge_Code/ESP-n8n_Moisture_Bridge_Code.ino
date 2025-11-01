#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClientSecureBearSSL.h> 
#include <memory> // std::unique_ptr için (client yönetimini kolaylaştırır)



// Wi-Fi information
const char* ssid = "***";
const char* password = "***";

// Hedef Webhook URL (HTTPS)
const char* serverUrl = "https://n8n.havsan.cloud/webhook/6909ddb3-2484-4e8f-a07a-5fdf0a379263"; 


const int SENSOR_PIN = A0;      //Moistrue Sensor Pın
const long TRANSMISSION_INTERVAL_MS = 500; 


void setup() {
  Serial.begin(115200);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  Serial.print("WiFi'ye bağlanılıyor");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi bağlandı! IP: " + WiFi.localIP().toString());
}

void loop() {
  if (WiFi.status() == WL_CONNECTED) {
    
    
    int raw_moisture = analogRead(SENSOR_PIN); 
    Serial.printf("\nSensörden Okunan Ham Veri (A0): %d\n", raw_moisture);

    
    std::unique_ptr<BearSSL::WiFiClientSecure> client(new BearSSL::WiFiClientSecure);

    
    client->setInsecure();

    HTTPClient http;
    
    if (!http.begin(*client, serverUrl)) {
      Serial.println("HTTP begin hatası");
      http.end();
    
      delay(TRANSMISSION_INTERVAL_MS);
      return; 
    }

    http.addHeader("Content-Type", "application/json");

    String jsonData = "{\"moisture\":" + String(raw_moisture) + ", \"location\":\"bahce_prototip\"}";

    Serial.print("Gönderilen JSON: ");
    Serial.println(jsonData);
    
    int httpResponseCode = http.POST(jsonData);

    if (httpResponseCode > 0) {
      Serial.printf("HTTP cevap kodu: %d\n", httpResponseCode);
      String payload = http.getString();
      Serial.print("Sunucu yanıtı (n8n): ");
      Serial.println(payload);
    } else {
      Serial.printf("HTTP POST hatası, kod: %d\n", httpResponseCode);
    }

    http.end();
    
    
  } else {
    Serial.println("WiFi bağlı değil, tekrar denenecek...");
  }

  delay(TRANSMISSION_INTERVAL_MS); 
}