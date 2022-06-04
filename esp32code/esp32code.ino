
#include <Arduino.h>
#include <WiFi.h>
#include "AsyncUDP.h"
#include <SoftwareSerial.h>
#include <ArduinoJson.h>
#include "esp_timer.h"
#include "img_converters.h"
#include "Arduino.h"
#include "soc/soc.h"           // Disable brownour problems
#include "soc/rtc_cntl_reg.h"  // Disable brownour problems
#include "driver/rtc_io.h"
#include <ESPAsyncWebServer.h>
#include <StringArray.h>
#include <SPIFFS.h>
#include <FS.h>
//annimation to grab objects
String frames[90] = {
  "0016300113003830028500000",
  "0016400113003830028500000",
  "0016500114003840028500000",
  "0016700114003840028400000",
  "0017000116003850028300000",
  "0017400117003860028100000",
  "0017900119003870028000000",
  "0018400121003890027800000",
  "0019100124003900027500000",
  "0019800127003910027300000",
  "0016300113003830028500000",
  "0018000120003810028900000",
  "0019600128003780029200000",
  "0021200136003740029600000",
  "0022600145003710030000000",
  "0024000153003670030300000",
  "0025200162003620030700011",
  "0026400171003570031100022",
  "0027500181003510031500033",
  "0028600190003440032000044",
  "0033700191003290033400055",
  "0033800200003300032800066",
  "0033800210003310032300077",
  "0033900219003300031800088",
  "0033900228003280031400099",
  "0034000236003260031000110",
  "0034000245003230030700110",
  "0034100253003190030500110",
  "0034100261003150030200110",
  "0034100269003100030100110",
  "0034300288002820030600110",
  "0034300292002810030400110",
  "0034300297002790030300110",
  "0034300302002770030100110",
  "0034300306002740030000110",
  "0034300310002720029900110",
  "0034300313002700029800110",
  "0034300317002670029800110",
  "0034300320002650029700110",
  "0034300323002620029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700110",
  "0034300339002470029700099",
  "0034300339002460029700088",
  "0034300339002450029800077",
  "0034300338002440029900066",
  "0034300338002430030000055",
  "0034300337002410030100044",
  "0034300336002390030300033",
  "0034300335002360030500022",
  "0034300335002330030700011",
  "0034300339002470029700000",
  "0034300337002410030100000",
  "0034200334002370030600000",
  "0034200331002330030900000",
  "0034000328002300031300000",
  "0033900325002290031500000",
  "0033700321002280031800000",
  "0033400317002290031900000",
  "0033200313002310032000000",
  "0032900308002350032100000",
  "0034300336001340036300000",
  "0033600320001720035000000",
  "0032900308001980034200000",
  "0032100298002200033600000",
  "0031300289002370033100000",
  "0030500280002530032700000",
  "0029700271002660032400000",
  "0028900263002780032200000",
  "0028100255002890032100000",
  "0027200247002980032000000",
  "0026300239003060032000000",
  "0025400232003130032000000",
  "0024500224003190032100000",
  "0023600216003240032300000",
  "0022600209003270032500000",
  "0021700201003300032800000",
  "0020700193003310033200000",
  "0019700186003310033600000",
  "0018700179003300034000000",
  "0017700172003290034500000"
};
//annimation to drop objects to the top right storage;
String dropTopRight[50] = {
  "0013200234004050020400000",
  "0013200234004050020400000",
  "0013200234004050020400000",
  "0013200233004060020400000",
  "0013200233004060020400000",
  "0013200232004060020500000",
  "0013200231004070020500000",
  "0013200230004070020500000",
  "0013300228004080020600000",
  "0013300227004090020600000",
  "0013200234004050020400000",
  "0013200225003980022200000",
  "0013300218003890024000000",
  "0013300213003800025700000",
  "0013400209003710027300000",
  "0013400206003610028900000",
  "0013400205003520030300000",
  "0013400205003420031700000",
  "0013500206003330033100000",
  "0013500208003240034400000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900000",
  "0013600203003170035900011",
  "0013600203003170035900022",
  "0013600203003170035900033",
  "0013600203003170035900044",
  "0013600203003170035900055",
  "0013600203003170035900066",
  "0013600203003170035900077",
  "0013600203003170035900088",
  "0013600203003170035900099",
  "0013600203003170035900110",
  "0013600203003170035900110",
  "0013600203003170035900110",
  "0013600203003170035900110",
  "0013600203003170035900110",
  "0013600203003170035900110",
  "0013600203003170035900110",
  "0013600203003170035900110",
  "0013600203003170035900110",
  "0013600203003170035900110"
};
//animation to drop objects to the bootom left storage
String dropBotLeft[70] = {
  "0013900206003990022300000",
  "0013900206003990022300000",
  "0014000206003990022300000",
  "0014000206003990022300000",
  "0014100206003990022300000",
  "0014200206003990022300000",
  "0014400206003990022300000",
  "0014600206003990022300000",
  "0014700206003990022300000",
  "0015000206003990022300000",
  "0013900206003990022300000",
  "0014400203003910023700000",
  "0014900201003820025100000",
  "0015300199003730026400000",
  "0015800199003650027700000",
  "0016100199003560028900000",
  "0016500200003470030100000",
  "0016900202003380031300000",
  "0017200204003300032400000",
  "0017500207003210033500000",
  "0019000210003130034500000",
  "0019000210003130034500000",
  "0019000210003130034500000",
  "0019000211003130034500000",
  "0019000211003130034500000",
  "0019000211003130034500000",
  "0019000212003120034400000",
  "0019000213003120034400000",
  "0019000213003120034400000",
  "0019000214003120034300000",
  "0019000210003130034500000",
  "0019000212003120034400000",
  "0019000214003120034400000",
  "0019000216003110034300000",
  "0019000217003110034200000",
  "0019000219003100034200000",
  "0019000220003100034100000",
  "0019000221003090034100000",
  "0019000223003090034000000",
  "0019000224003080034000000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800000",
  "0019000230003050033800011",
  "0019000230003050033800022",
  "0019000230003050033800033",
  "0019000230003050033800044",
  "0019000230003050033800055",
  "0019000230003050033800066",
  "0019000230003050033800077",
  "0019000230003050033800088",
  "0019000230003050033800099",
  "0019000230003050033800110",
  "0019000230003050033800110",
  "0019000230003050033800110",
  "0019000230003050033800110",
  "0019000230003050033800110",
  "0019000230003050033800110",
  "0019000230003050033800110",
  "0019000230003050033800110",
  "0019000230003050033800110",
  "0019000230003050033800110"
};
//animation to drop objects to the bootom right storage
String dropBotRight[50] = {
  "0018200230003910021400000",
  "0018200230003910021400000",
  "0018200230003920021400000",
  "0018100230003920021400000",
  "0018100231003930021300000",
  "0017900231003940021300000",
  "0017800231003950021200000",
  "0017600231003960021100000",
  "0017500232003970021000000",
  "0017300232003990020900000",
  "0018200230003910021400000",
  "0017800227003860022800000",
  "0017300224003800024200000",
  "0016900222003730025500000",
  "0016500221003660026900000",
  "0016100221003590028200000",
  "0015700221003520029400000",
  "0015400223003440030700000",
  "0015100225003360031800000",
  "0014800228003290033000000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600000",
  "0013300230003310033600011",
  "0013300230003310033600022",
  "0013300230003310033600033",
  "0013300230003310033600044",
  "0013300230003310033600055",
  "0013300230003310033600066",
  "0013300230003310033600077",
  "0013300230003310033600088",
  "0013300230003310033600099",
  "0013300230003310033600110",
  "0013300230003310033600110",
  "0013300230003310033600110",
  "0013300230003310033600110",
  "0013300230003310033600110",
  "0013300230003310033600110",
  "0013300230003310033600110",
  "0013300230003310033600110",
  "0013300230003310033600110",
  "0013300230003310033600110"
};
//animation to drop objects to the top left storage
String dropTopLeft[50] = {
  "0016900240003900020900000",
  "0016900240003910020900000",
  "0016900239003910020900000",
  "0017000239003920020900000",
  "0017000238003930020900000",
  "0017100237003940020900000",
  "0017100236003960020900000",
  "0017200234003970020800000",
  "0017300233004000020800000",
  "0017400231004020020800000",
  "0016900240003900020900000",
  "0017200230003860022700000",
  "0017400222003800024500000",
  "0017600216003720026300000",
  "0017800211003640028100000",
  "0018000208003560029700000",
  "0018200206003470031300000",
  "0018400206003390032800000",
  "0018500207003310034200000",
  "0018700209003230035500000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900000",
  "0019400199003260036900011",
  "0019400199003260036900022",
  "0019400199003260036900033",
  "0019400199003260036900044",
  "0019400199003260036900055",
  "0019400199003260036900066",
  "0019400199003260036900077",
  "0019400199003260036900088",
  "0019400199003260036900099",
  "0019400199003260036900110",
  "0019400199003260036900110",
  "0019400199003260036900110",
  "0019400199003260036900110",
  "0019400199003260036900110",
  "0019400199003260036900110",
  "0019400199003260036900110",
  "0019400199003260036900110", 
  "0019400199003260036900110",
  "0019400199003260036900110"
};

void startCameraServer();
#define CAMERA_MODEL_AI_THINKER // Has PSRAM
#include "camera_pins.h"

const char* ssid = "*****";
const char* password = "******";
//indicate if a qr code has been detected
bool b=false;
//String representing the shoppingList
String serializeShoppingList = "";

StaticJsonDocument<1024> shoppingList;
AsyncUDP udp;

bool a = true;

int delay_message_ard = 50;
void startCameraServer();
int numOfBlock = 0;
bool c=false;
void setup() {
  Serial.begin(9600);
  Serial2.begin(115200, SERIAL_8N1, 12, 13);
  Serial.setDebugOutput(true);
  Serial.println();
  //config of the tcp qrcode image sending protocol
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = Y2_GPIO_NUM;
  config.pin_d1 = Y3_GPIO_NUM;
  config.pin_d2 = Y4_GPIO_NUM;
  config.pin_d3 = Y5_GPIO_NUM;
  config.pin_d4 = Y6_GPIO_NUM;
  config.pin_d5 = Y7_GPIO_NUM;
  config.pin_d6 = Y8_GPIO_NUM;
  config.pin_d7 = Y9_GPIO_NUM;
  config.pin_xclk = XCLK_GPIO_NUM;
  config.pin_pclk = PCLK_GPIO_NUM;
  config.pin_vsync = VSYNC_GPIO_NUM;
  config.pin_href = HREF_GPIO_NUM;
  config.pin_sscb_sda = SIOD_GPIO_NUM;
  config.pin_sscb_scl = SIOC_GPIO_NUM;
  config.pin_pwdn = PWDN_GPIO_NUM;
  config.pin_reset = RESET_GPIO_NUM;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;

  // if PSRAM IC present, init with UXGA resolution and higher JPEG quality
  //                      for larger pre-allocated frame buffer.

  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;


#if defined(CAMERA_MODEL_ESP_EYE)
  pinMode(13, INPUT_PULLUP);
  pinMode(14, INPUT_PULLUP);
#endif

  // camera init
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  sensor_t * s = esp_camera_sensor_get();
  s->set_contrast(s, 2);
  // The contrast is really high in order to see the qrCode better
  if (s->id.PID == OV3660_PID) {
    s->set_vflip(s, 2); // flip it back
    s->set_brightness(s, 1); // up the brightness just a bit
    s->set_saturation(s, -2); // lower the saturation
  }
  // Highest resolution
  s->set_framesize(s, FRAMESIZE_UXGA);

#if defined(CAMERA_MODEL_M5STACK_WIDE) || defined(CAMERA_MODEL_M5STACK_ESP32CAM)
  s->set_vflip(s, 1);
  s->set_hmirror(s, 1);
#endif

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  startCameraServer();

  Serial.print("Camera Ready! Use 'http://");
  Serial.print(WiFi.localIP());
  Serial.println("' to connect");
  //Asychronus listening to udp with port 1234
  if (udp.listen(1234)) {
    Serial.print("UDP Listening on IP: ");
    Serial.println(WiFi.localIP());
    udp.onPacket([](AsyncUDPPacket packet) {
      Serial.print((const char*)packet.data());
      Serial.println();
      //if the message sended by the application start with { then it is considered a Json ans create a shopping list
      if (packet.data()[0] == '{') {
        shoppingList.clear();
        serializeShoppingList = (const char*)packet.data();
        Serial.println(serializeShoppingList);
        deserializeJson(shoppingList, (const char*)packet.data());
        serializeShoppingList = "";
        serializeJson(shoppingList, serializeShoppingList);
        Serial.println(serializeShoppingList);
        udp.broadcast("received");
        a=true;
        //send_arduino("forward");
      }
      // else it is a qrCode
      else {
        c=true;
        Serial.println("qrFound");
        qrCodeResultProcessing((const char *)packet.data());
      }

    });
  }
}
int temp=0;
void qrCodeResultProcessing(const char * payload) {
  Serial.print("Payload: ");
  Serial.println(payload);
  String payloads(payload);      
  //The Udp communication add artefacts to the end of the String so we use 5 to signify the end of the string 
  payloads=payloads.substring(0,payloads.indexOf('5'));
  Serial.println(payloads);
  Serial.println((int)shoppingList[payloads]);
  //Check if the qr code is in the shoppingList
  if (shoppingList.containsKey(payloads)) {
    Serial.println("Item is in the list");
    Serial.println(payloads);
    //number of articles to take
    temp = (int)shoppingList[payloads];
    Serial.println((int)shoppingList[payloads]);
    shoppingList.remove(payloads);
    udp.broadcast(payloads.c_str());
    //indicate that we have to grab the item
    b=true;

   

  }
  else {
    Serial.println("not in the shopping list");
    
  }
  //Update the String representing the shoppingList
    serializeShoppingList = "";
  serializeJson(shoppingList, serializeShoppingList);
  Serial.println(serializeShoppingList);
  
  //Qr code present at the charging station
  if (payload == "end") {
    serializeShoppingList = "";
    shoppingList=0;
    udp.broadcast("end");
    a = false;
  }


  }
int i = 0;
void loop()
{
  //if a shopping list has been received
  if(  serializeShoppingList.length()>2){
    //while qrcode end hasn't been found
    while(a){
      send_arduino("forward");
      //if qrCode in the list has been found
      if(b){
            String grab="grab_begin";
    grab+=temp;
    send_arduino(grab);
    b=false;
      }
      //if qrCode 
        if(c){   
          c=false;
           delay(200);
    send_arduino("restart");
          }
        delay(300);
  }
  serializeShoppingList = "";
    shoppingList=0;
    Serial.println("finished");
  delay(300);
  }


}

//protocole to communicate with the arduino
void send_arduino(const String& message) {
  String oui = "?" + message;
  int t=0;
   Serial2.flush();
   //find the response from the arduino or try to send the message 10 times 
  do{
    t++;
    delay(100);
    Serial2.flush();
    Serial2.println(oui);
    Serial.println(oui);
    delay(100);
  }while(t<10&&Serial2.read() != '!');
  // if themessage is agran (we grab a maximum of 3 items)
  if (oui == "?grab_begin1" || oui == "?grab_begin2" || oui == "?grab_begin3") {
    //send grabing frames
    for(int j=0;j<temp;j++){
      for (int p = 0; p < 90; ++p) {
        String KN = "?frame" + frames[p];
        for(int SUPER=0;SUPER<5;++SUPER){
        Serial2.println(KN);
        Serial.println(KN);
        Serial2.flush();

        }
      }
    }
    
    

    Serial.println("start dropping");
    //send drop frames with position of the drop changing for each articles;
    if (i % 4 == 0) {
      for (int p = 0; p < 70; ++p) {
        String KN = "?frame" + dropBotLeft[p];
        for(int SUPER=0;SUPER<5;++SUPER){
        Serial2.flush();
        Serial2.println(KN);
         Serial.println(KN);}

    }
    }

    if (i % 4 == 1) {
      for (int p = 0; p < 50; ++p) {
        String KN = "?frame" + dropTopLeft[p];
       
        for(int SUPER=0;SUPER<5;++SUPER){
        Serial2.flush();
        Serial2.println(KN);
         Serial.println(KN);}
      }
    }
    if (i % 4 == 2) {
      for (int p = 0; p < 50; ++p) {
        String KN = "?frame" + dropTopRight[p];
        for(int SUPER=0;SUPER<5;++SUPER){
        Serial2.flush();
        Serial2.println(KN);
         Serial.println(KN);}
      }
    }

    if (i % 4 == 3) {
      for (int p = 0; p < 50; ++p) {
        String KN = "?frame" + dropBotRight[p];
        for(int SUPER=0;SUPER<5;++SUPER){
        Serial2.flush();
        Serial2.println(KN);
         Serial.println(KN);}
      }

    }
    //send to the arduino that the drop is finished
    Serial2.println("?drop_end");
    Serial2.flush();
    delay(100);
    
    Serial2.println("?drop_end");
    Serial2.flush();
    delay(100);
    Serial2.flush();
    Serial2.println("?drop_end");
    Serial2.flush();
    delay(100);
    
    Serial2.println("?drop_end");
    Serial2.flush();
    delay(100);
    
    Serial2.println("?drop_end");
    Serial2.flush();
    delay(100);
    
    Serial2.println("?drop_end");
    Serial2.flush();
    i++;
  }
 
Serial.println("communication finished");
}
