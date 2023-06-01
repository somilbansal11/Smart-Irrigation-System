#include <pgmspace.h>
#define SECRET
#define THINGNAME "irrigator"
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

#define AWS_IOT_PUBLISH_TOPIC "smart/pub"
#define AWS_IOT_SUBSCRIBE_TOPIC "smart/sub"

const char* WIFI_SSID = "Abi";       // Replace with your Wi-Fi network SSID
const char* WIFI_PASSWORD = "qwerty123"; // Replace with your Wi-Fi network password

const char* AWS_IOT_ENDPOINT = "a1kw3sh9yd5a8j-ats.iot.us-east-1.amazonaws.com";
const int AWS_IOT_PORT = 8883;

static const char AWS_CERT_CA[] PROGMEM = R"EOF(
-----BEGIN CERTIFICATE-----
MIIDQTCCAimgAwIBAgITBmyfz5m/jAo54vB4ikPmljZbyjANBgkqhkiG9w0BAQsF
ADA5MQswCQYDVQQGEwJVUzEPMA0GA1UEChMGQW1hem9uMRkwFwYDVQQDExBBbWF6
b24gUm9vdCBDQSAxMB4XDTE1MDUyNjAwMDAwMFoXDTM4MDExNzAwMDAwMFowOTEL
MAkGA1UEBhMCVVMxDzANBgNVBAoTBkFtYXpvbjEZMBcGA1UEAxMQQW1hem9uIFJv
b3QgQ0EgMTCCASIwDQYJKoZIhvcNAQEBBQADggEPADCCAQoCggEBALJ4gHHKeNXj
ca9HgFB0fW7Y14h29Jlo91ghYPl0hAEvrAIthtOgQ3pOsqTQNroBvo3bSMgHFzZM
9O6II8c+6zf1tRn4SWiw3te5djgdYZ6k/oI2peVKVuRF4fn9tBb6dNqcmzU5L/qw
IFAGbHrQgLKm+a/sRxmPUDgH3KKHOVj4utWp+UhnMJbulHheb4mjUcAwhmahRWa6
VOujw5H5SNz/0egwLX0tdHA114gk957EWW67c4cX8jJGKLhD+rcdqsq08p8kDi1L
93FcXmn/6pUCyziKrlA4b9v7LWIbxcceVOF34GfID5yHI9Y/QCB/IIDEgEw+OyQm
jgSubJrIqg0CAwEAAaNCMEAwDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMC
AYYwHQYDVR0OBBYEFIQYzIU07LwMlJQuCFmcx7IQTgoIMA0GCSqGSIb3DQEBCwUA
A4IBAQCY8jdaQZChGsV2USggNiMOruYou6r4lK5IpDB/G/wkjUu0yKGX9rbxenDI
U5PMCCjjmCXPI6T53iHTfIUJrU6adTrCC2qJeHZERxhlbI1Bjjt/msv0tadQ1wUs
N+gDS63pYaACbvXy8MWy7Vu33PqUXHeeE6V/Uq2V8viTO96LXFvKWlJbYK8U90vv
o/ufQJVtMVT8QtPHRh8jrdkPSHCa2XV4cdFyQzR1bldZwgJcJmApzyMZFo6IQ6XU
5MsI+yMRQ+hDKXJioaldXgjUkK642M4UwtBV8ob2xJNDd2ZhwLnoQdeXeGADbkpy
rqXRfboQnoZsG4q5WTP468SQvvG5
-----END CERTIFICATE-----
)EOF";


static const char AWS_CERT_PRIVATE[] PROGMEM = R"KEY(
-----BEGIN RSA PRIVATE KEY-----
MIIEowIBAAKCAQEAseuYsFOece1nKmfw7KXGpUu8d/X8eNYsITBAzy1THdeJjuSn
T2O8Xw0nKGW5Ysha5B3hlX9XGRutqpC6nKCxcxVvPCCDSI61NjpqfsiexPpPii5n
xeLg/5M+W2YgMD9CrN+BgrCcmbDhwWKa3zJINAzrM6x7Ylk5gx3tmmVf3GOzPV9e
9Bk4N2jSwLxZegLrDa9RILB8OsCqEJqDBkbDLzKWh7n39h6A5kZtL+FTtXeU6Kht
2b6IH1Uvp+TYhlgI2ZTZlqOQacd5YopgJYCDrVrRGjx5uKIsmUM7LL4vWrsUOH+P
IAYhvtJ4HV67p/1g1A5Gd2nmVs36Q+1O+gZa2wIDAQABAoIBAFQu9lsP5ZtgSf0N
mzhNo/UYJb/Y+zGtnMnDTBb7CiXHkwWyeNuehEWxMoLxxsrPo/nljZQv4WU27HAU
9MchXXTngm8/rv6Iz4MuFq+Qe1cWXMYz+NPv6qfi4pFafAZ4GDM1udC9ggtSXy8i
WccbwoAmUK9tgB/jucupQIBaUQY0t+0rud9VQtqQCcDHE7Pvdt5ldimubY9yh2oW
s87abbXTuZMgJ34Ywqrrwnk1CZgmRkziYK1eJ+giuLYJ2kTOzBFHj5l+cZ/Lksud
u3POI2kqpHcPlp5rkxHP6GP/f8M3prL1p6LlHE+1NGWVeZjZMhWJHnwYgaAN6R7b
4SRWGKECgYEA5JUZck1hWmJ6ufNBGEC8SA0SUJ7jyzJtC9ydamWP1IQY89/9Fbkc
sgx291+jc/mGdjYLUlbTdKQ7WIXj98HwZw3Ly267WYZBznTnbV0Fx6k4jmDbU2IF
FDfuE1Hd1HwHYqeCRxVVPH++MRnZEZB++qUevPURymMUXwjfurbLBRECgYEAx0La
6gJJ+2evB3vZz6zYe6P5RV3KrvQAHPBhzLh48bAVPXNCP7tO9Yw6dWrduudxK3wD
bG+Al5xEW7qlZPfP86AmI5IVNw9mPeLGpTM+vzmfMlsIhQGJkyWk9AsOsBAAWSKG
j1uhtA+FtZvB+Hfp1Km0PIxM8yf6nkmEWF2wcSsCgYAynKzguS94ItwHtkvQgezu
iy5l+Xh4snagrL2SJGC/bmxT+2F/YNhwt//TcDKx6CsUm4zQM3domUCmria24hNH
zi80oRUMkoSZoRrpBTLVbaufIuUNRmPXgTbOwk660KeV+X8IHDoeRi6nkpCUqgjj
mJIAxsakqGGHVWCsZ/L18QKBgQCEq2O8WOfPRvqHnhOorWm58LRAVjFkCTumCPrk
5pJIQTfzITGj41+wJ7yofw3AquZz0G8rHJZe24G8HWqP7lt7105tco9DiLJHVB10
0sjdY002ptvmSt7BpVfN0SrnWNASiOVUv+rLS2Z4Q4uDlruOh6tkgPmfEChSiZpf
h4m53wKBgHzM/p440Dxmae5arS6uH4OynDRfWrCjlZQ8WzWVL9gWYiTjcYa+Yqwu
jXrMJQP5XBtJW1yD5D6UFR/VT1Y3RNwvv1wApyMa3X3itPqEeQ89T19p0kz2d7g+
KGnXjP9VNW9IeIKfYup6Y8urlYjBT4Zjj7tUji8tblQU2Npmc0cU
-----END RSA PRIVATE KEY-----
)KEY";

static const char AWS_CERT_CRT[] PROGMEM =R"KEY(
-----BEGIN CERTIFICATE-----
MIIDWjCCAkKgAwIBAgIVAPu5c/YdY1X9YGGKP2f5Es4VBSIgMA0GCSqGSIb3DQEB
CwUAME0xSzBJBgNVBAsMQkFtYXpvbiBXZWIgU2VydmljZXMgTz1BbWF6b24uY29t
IEluYy4gTD1TZWF0dGxlIFNUPVdhc2hpbmd0b24gQz1VUzAeFw0yMzA1MzAxODI1
NDFaFw00OTEyMzEyMzU5NTlaMB4xHDAaBgNVBAMME0FXUyBJb1QgQ2VydGlmaWNh
dGUwggEiMA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQCx65iwU55x7WcqZ/Ds
pcalS7x39fx41iwhMEDPLVMd14mO5KdPY7xfDScoZbliyFrkHeGVf1cZG62qkLqc
oLFzFW88IINIjrU2Omp+yJ7E+k+KLmfF4uD/kz5bZiAwP0Ks34GCsJyZsOHBYprf
Mkg0DOszrHtiWTmDHe2aZV/cY7M9X170GTg3aNLAvFl6AusNr1EgsHw6wKoQmoMG
RsMvMpaHuff2HoDmRm0v4VO1d5ToqG3ZvogfVS+n5NiGWAjZlNmWo5Bpx3liimAl
gIOtWtEaPHm4oiyZQzssvi9auxQ4f48gBiG+0ngdXrun/WDUDkZ3aeZWzfpD7U76
BlrbAgMBAAGjYDBeMB8GA1UdIwQYMBaAFFknQ+cV7P8mFYYWStHCksrqPWqmMB0G
A1UdDgQWBBSTeHHHdBtL4jkwkY55RceBXt9sqzAMBgNVHRMBAf8EAjAAMA4GA1Ud
DwEB/wQEAwIHgDANBgkqhkiG9w0BAQsFAAOCAQEAsOaZh5e4tUDr+Uanyc61atK1
IcDYcmNDjJDojVYp8Fvyy7P2gykDpTqdyLRWRoEA3Wd36k6q2CrkEf9FkY14Y6bv
wEzqZ5WjGooV/uCrVGyexdcLDVUN57Ml7JUZf7ZL7ZhJIhr930sPMF383i4fQMSV
zzd6nvA3c3QQfDzDKJcL6LWZWJEpd5S37+xHl2cIBy8/XgYVNCJiZfZPo+NVvESc
pKI27OJZQUowgNagaj+PZX70Ca9SFg4nUX4HvdS36GpqJHj0Xwqb14lWSmiO8EWd
Wv85Cme4Q3OQrvkjZeP+QwRzHL4K+6e680/HBdblYEZIbhfOoB4NURUT4rFBGw==
-----END CERTIFICATE-----

)KEY";

WiFiClientSecure net;
PubSubClient client(net);

void messageHandler(char* topic, byte* payload, unsigned int length)
{
  Serial.print("incoming: ");
  Serial.println(topic);
 
  StaticJsonDocument<200> doc;
  deserializeJson(doc, payload);
  const char* message = doc["message"];
  Serial.println(message);
}

void connectAWS() {
  
  

  Serial.print("Connecting to AWS IoT Core...");
  net.setCACert(AWS_CERT_CA);
  net.setPrivateKey(AWS_CERT_PRIVATE);
  net.setCertificate(AWS_CERT_CRT);
  
  client.setServer(AWS_IOT_ENDPOINT, AWS_IOT_PORT);
  client.setCallback(messageHandler);
  
  while (!client.connected()) {
    if (client.connect("smart")) {
      
      Serial.println("connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" retrying in 5 seconds...");
      delay(5000);
    }
  }
}

void setup() {
  
  Serial.begin(115200);
  Serial.print("Connecting to Wi-Fi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("connected");
  connectAWS();
  client.subscribe(AWS_IOT_SUBSCRIBE_TOPIC);

  if (!client.connected()) {
    connectAWS();
  }
  client.loop();

  // Add your logic or sensor readings here
  

      
delay(1000);

}




int k=0;
int n=0;
void loop() {
 char buffer[200]="";
    float a[4];
    
    String x;
    if(Serial.available()>0){
      Serial.readBytesUntil('&',buffer,200);
      String ab =buffer;
      StaticJsonDocument<200> doc;

      if(k==0){
        Serial.print("Temp = ");
        Serial.print(ab.toFloat());
        Serial.println();
        a[0]=ab.toFloat();
        
       
        }

      if(k==1){
        Serial.print("Humid = ");
        Serial.print(ab.toFloat());
        Serial.println();
        a[1]=ab.toFloat();
        
        
        }

        if(k==2){
          Serial.print("Moist = ");
        Serial.print(ab.toFloat());
        Serial.println();
        a[2]=ab.toFloat();
       
        }

        if(k==3){
        Serial.print("Rain = ");
        Serial.print(ab.toFloat());
        Serial.println();
        a[3]=ab.toFloat();
        doc["temperature"] = a[0];
        doc["humidity"] = a[1];
        doc["moisture"] = a[2];
        doc["rain"] = a[3];
        if(a[0]>31){
          n=n+0.2;
        }
        else if(a[1]<60){
          n=n+0.3;
        }
        else if(a[2]>460){
          n=n+0.5;
        }
        else if(a[3]<700){
          n=0;
        }
        if(n>=5){
          doc["Need"]="Yes";
        }
        else{
          doc["Need"]="No";
        }
        
        char jsonBuffer[512];
        serializeJson(doc, jsonBuffer); // print to client
        
        client.publish(AWS_IOT_PUBLISH_TOPIC, jsonBuffer);
        
        
        memset(a, 0, sizeof(4));
        k=0;
        }
        else{
          k=k+1;
          }
    }
}
