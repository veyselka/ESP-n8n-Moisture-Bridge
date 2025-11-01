Markdown# 🌉 ESP-n8n Moisture Bridge: Kablosuz Toprak Nem Verisi Aktarım Prototipi

Bu proje, düşük maliyetli donanım (ESP8266) kullanarak bir IoT sensöründen alınan ham verinin, ek bir sunucuya ihtiyaç duymadan doğrudan bir otomasyon platformuna (n8n Webhook) nasıl güvenilir bir şekilde POST edildiğini gösteren bir prototiptir. Proje, geniş ölçekli akıllı tarım uygulamalarına geçiş için temel bir iletişim mimarisi sağlar.

## 🌟 Proje Amacı

Bu prototipin temel amacı, gelecekteki güneş enerjili, Deep Sleep modunda çalışacak çok noktalı kablosuz sulama sistemleri için bir iletişim "köprüsü" oluşturmaktır.

* **Çözülen Problem:** Sensör verisinin karmaşık altyapılar (bulut sunucusu, MQTT broker) olmadan direkt olarak bir karar mekanizmasına (n8n) ulaştırılması.
* **Odak:** Güvenilir Wi-Fi bağlantısı ve HTTP/JSON protokolü üzerinden veri bütünlüğünün sağlanması.

## 🛠️ Kullanılan Teknolojiler

| Kategori | Bileşen | Detay |
| :--- | :--- | :--- |
| **Mikrodenetleyici** | WeMos D1 Mini | ESP8266 tabanlı, dahili Wi-Fi, programlama kolaylığı. |
| **Sensör** | Kapasitif Toprak Nem Sensörü | Korozyona dayanıklı, analog çıkışlı sensör. |
| **Geliştirme Ortamı** | Arduino IDE | ESP8266 Core (C++ tabanlı). |
| **Protokol** | HTTP POST / JSON | Veriyi n8n'e taşımak için. |
| **Otomasyon Platformu** | n8n (Webhook) | Gelen JSON verisini yakalamak ve iş akışını başlatmak için. |

## 🔌 Donanım Konfigürasyonu (Prototip)

Prototip, sadece 3 temel bağlantı kullanır:

| Sensör Pini | WeMos D1 Mini Pini | Açıklama |
| :--- | :--- | :--- |
| **VCC** | **3V3** | Sensöre güç sağlar. |
| **GND** | **GND** | Ortak toprak. |
| **AOUT** | **A0** | Toprak neminin analog (0-1023) verisinin okunduğu pin. |

***💡 Not: Kapasitif sensör seçimi, dirençli sensörlerin topraktaki elektroliz nedeniyle hızla korozyona uğramasını engeller ve uzun ömür sağlar.***

## 💻 Yazılım ve Protokol Detayları (C++ Deep Dive)

Proje, Arduino C++ kullanılarak geliştirilmiştir ve veri bütünlüğünü sağlamak için kritik HTTPClient kütüphanesini kullanır.

### 1. JSON Payload Yapısı

Sensörden okunan ham analog değer, HTTP POST isteği ile n8n'e **JSON formatında** gönderilir:

```json
{
  "moisture": 750,
  "location": "prototype_test"
}
2. HTTP Bağlantı ve Hata YönetimiKod, en yaygın karşılaşılan sunucu ve SSL uyumsuzluklarını aşmak için tasarlanmıştır.KriterDetayKod Parçası (Özet)Wi-Fi YönetimiBağlantı sürekli kontrol edilir ve koparsa yeniden bağlanmayı dener.if (WiFi.status() != WL_CONNECTED) { ... WiFi.begin(ssid, password); }URL KullanımıHTTPS/SSL sorunlarını çözmek için URL tam dize olarak http.begin(client, TAM_URL) formatında kullanılır.http.begin(client, N8N_FULL_URL);Veri ProtokolüVeri, application/json başlığıyla gönderilir.http.addHeader("Content-Type", "application/json");SSL/HTTPS Zorlama(Eğer kütüphane destekliyorsa) N8N'e bağlantı sorunlarını çözmek için http.setInsecure(); kullanılabilir.Not: Kodda, kütüphane uyumsuzluğu nedeniyle bu satır pasif bırakılmıştır, ancak güncel kütüphanede aktive edilmelidir.🚀 Kurulum ve Çalıştırma KılavuzuArduino IDE Hazırlığı: Arduino IDE'ye ESP8266 Board Manager URL'sini ekleyin ve LOLIN(WEMOS) D1 R2 & mini kartını yükleyin/seçin.Bağlantı: Nem sensörünü yukarıdaki şemaya göre WeMos D1 Mini'ye bağlayın.Kod Düzenleme: Koddaki ssid, password ve N8N_FULL_URL sabitlerini kendi bilgilerinizle değiştirin.n8n Hazırlığı: n8n iş akışınızı oluşturun ve Webhook düğmesini POST metodunda ve Test Listener modunda başlatın.Yükleme: Kodu WeMos D1 Mini'ye yükleyin.Test: Seri Monitör'ü (115200 Baud) açarak Wi-Fi bağlantısını ve HTTP Response Code: 200 (Başarılı) çıktısını kontrol edin.🔭 Gelecek Genişletme Alanları (Future Scope)Bu prototip, tam bir IoT çözümüne dönüştürülebilir:Güç Yönetimi: Güneş Paneli, TP4056 ve 18650 pil entegrasyonu.Enerji Verimliliği: Pil ömrünü uzatmak için "Deep Sleep" modu (D0 $\rightarrow$ RST bağlantısı) uygulanması.Akıllı Kontrol: n8n üzerinde gelen nem verisine göre IF (Koşul) düğümleri ile sulama vanası API'sinin otomatik olarak tetiklenmesi.