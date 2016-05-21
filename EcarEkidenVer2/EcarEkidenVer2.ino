/*
* 駅伝ラジコン用WiFiシステムのプログラム
*/

// ヘッダーのインクルード
#include <signal.h> // you must write down this line to resolve problem between WiFiSocket and Serial communication
#include <WiFi.h>

/* -------------------------------- Wifi Parameters  -------------------------------- */
IPAddress ip(192, 168, 128, 100);
char ssid[] = "304HWa-84F1A0"; // your network SSID (name), nakayama:506A   BUFFALO-4C7A25 iPhone_shinichi
char pass[] = "11237204a"; // your network password (use for WPA, or use as key for WEP), nakayama:12345678 iebiu6ichxufg    252554123sin
int keyIndex = 0; // your network key Index number (needed only for WEP)
int status = WL_IDLE_STATUS;
int serverPort = 7000;
WiFiServer server(serverPort); // 9090番ポートを指定
WiFiClient client;
int socketTimeCount = 0;
const int socketTimeOut = 5; //10 20 30
int clientCheckCount = 0;
boolean connectStatus;
String sendWiFiData = "";
int bufData = 0; // TCP/IPに溜まっているバッファ数
unsigned char bufArray[100];
boolean logout = false;
/* -------------------------------- Wifi Params  -------------------------------- */


/* -------------------------------- Pin Params  -------------------------------- */
// stearing
const int pinL = 11;
const int pinR = 10;
// driving
const int pinMtrPwm1 = 6; // pwm
const int pinMtrPwm2 = 5; // pwm
/* -------------------------------- Pin Params  -------------------------------- */


/* -------------------------------- Control Params  -------------------------------- */
unsigned char ctl[3] = {'0','0','0'};
char steering = 'C';
int dirMotor = 0;
int inputPWM[2] = {0, 0};

void setup() {  
  /* you must write down a following line */
  signal(SIGPIPE, SIG_IGN); // caution !! Please don't erase this line
  
  /*
   * serial port document
   * Serial : /dev/ttyGS0 in Edison(Linux OS), thus your arduino IDE serial monitor
   * Serial1 : /dev/ttyMFD1 in Edison(Linux OS), thus Pin 0(Rx) and 1(Tx) in your arduino breakout board for Edison
   * Serial2 : /dev/ttyMFD2 in Edison(Linux OS), thus a terminal when you login to J3 in arduino breakout board for Edison with microUSB-B   
  */
  
  pinMode(pinL, OUTPUT);
  pinMode(pinR, OUTPUT);
  pinMode(pinMtrPwm1, OUTPUT);
  pinMode(pinMtrPwm2, OUTPUT);
  
  Serial.begin(9600); 
  delay(500); 
    
  // WiFi環境の構築　別途、WiFi.inoプログラムを参照するように
  /***********************************************************************************/
  setWiFi();

  // WiFi接続確認用
  // Right
  Serial.println("Steering RIGHT");
  digitalWrite(pinL, LOW);
  digitalWrite(pinR, HIGH);
  delay(100);
  // Left
  Serial.println("Steering LEFT");
  digitalWrite(pinL, HIGH);
  digitalWrite(pinR, LOW);
  delay(100);
  digitalWrite(pinL, LOW);
  digitalWrite(pinR, LOW);
  
  Serial.println("[[[[[[ loop start ]]]]]]");
  Serial.print("Local IP : ");
  Serial.println(ip);
  Serial.print("Port : ");
  Serial.println(serverPort);
}

void loop() { 
//  digitalWrite(pinL, HIGH);
//  delay(1000);
//  digitalWrite(pinR, HIGH);
//  delay(1000);
//  
//  digitalWrite(pinL, LOW);
//  delay(1000);
//  digitalWrite(pinR, LOW);
//  delay(1000);
 
// Serial.println("Start");
// for (int i = 0; i < 256; i++) {
//  analogWrite(pinMtrPwm1, i);
//  analogWrite(pinMtrPwm2, i);
//  delay(10);
// }
// analogWrite(pinMtrPwm1, 0);
// analogWrite(pinMtrPwm2, 0);
// Serial.println("end");
// delay(1000);

  //　サーバー(Edison)として指定したポートにクライアント(Android)からのアクセスがあるか確認。あれば、接続する 
  client = server.available();
  Serial.print("Client Status : ");
  Serial.println(client);

  // クライアント(Android)が存在する場合
  if (client) { 
    socketTimeCount = 0;
    Serial.println("New Client");
    // クライアント(Android)とサーバー(Edison)
    // 処理に約5秒かかる
    while (true) {//client.connected()) { // 注意!! client.connected()関数にはバグあり!
      //Serial.println("Client Connected");    
      if (true) { // client.available() > 0
        socketTimeCount = 0;
        //Serial.println("Data Exist");

        char recvChar = '\0'; // 受信文字
        char recvStr[32]; // 受信用バッファ

        // 受信用バッファの受信
        for(int i = 0; i < 32; i++){
          recvStr[i] = '\0';
        }

        int i = 0;
        while(recvChar != '\n'){
          recvChar = client.read(); // 文字の受信
          recvStr[i] = recvChar;
          i++;
        }
        Serial.print(":> "); Serial.println(recvStr);
  
        int j = 0;
        while(true) {
          //Serial.println("Break Point_1!");
          if(recvStr[j] == '+') {
            dirMotor = 1;
            inputPWM[0] = 100 * (int)(recvStr[j+1] - '0') + 10 * (recvStr[j+2] - '0') + (recvStr[j+3] - '0'); 
            //Serial.println(inputPWM[0]);
            if(inputPWM[0] == 0)
              dirMotor = 0;
              
            steering = recvStr[j+5]; // R or L or C 
            //Serial.println("BREAK POINT!");            
            break;
          }
          //Serial.println("Break Point_2!");
          if(recvStr[j] == '-') {
            dirMotor = -1;
            inputPWM[1] = 100 * (int)(recvStr[j+1] - '0') + 10 * (recvStr[j+2] - '0') + (recvStr[j+3] - '0');
            //Serial.println(inputPWM[1]);
            if(inputPWM[1] == 0)
              dirMotor = 0;
              
            steering = recvStr[j+5]; // R or L or C
            break;
          }
          //Serial.println("Break Point_3!");
          if(recvStr[j] == 'l') {
            String mark = "";
            for(int k=0; k<6; k++) {
              mark += (String)recvStr[j+k];
            }
            if(mark.equals("logout")) {
              logout = true;
              break;
            }
          }
          //Serial.println("Break Point_4!");
          if(j > 32)
            break;
          //Serial.println("Break Point_5!");
          j++;
        } // endwhile
        //Serial.println("BREAK POINT_endwhile!"); 
//        char revChar = client.read(); // read from TCP buffer
//        Serial.print("Get [");
//        Serial.print(revChar);
//        Serial.print("] -> Send  : ");
        // クライアントからのリクエストの受け取りとその処理
        /***********************************************************************************/
//        switch(recvChar) {  
//          case '+':
//            Serial.println("Pulas");
//            dirMotor = 1;
//            ctl[0] = client.read(); // char pwm1
//            ctl[1] = client.read(); // char pwm1
//            ctl[2] = client.read(); // char pwm1
//            Serial.print("nor");Serial.print(ctl[0]); Serial.print(ctl[1]); Serial.println(ctl[2]); 
//            inputPWM[0] = 100 * (int)(ctl[0] - '0') + 10 * (ctl[1] - '0') + (ctl[2] - '0'); 
//            if (inputPWM[0] == 0) {
//               dirMotor = 0;
//            }
//            Serial.print("pwm+:"); Serial.println(inputPWM[0]);
//            Serial.println(dirMotor);
//            break;
          
//          case '-':
//            Serial.println("Mainus");
//            dirMotor = -1;
//            ctl[0] = client.read(); // char pwm2
//            ctl[1] = client.read(); // char pwm2
//            ctl[2] = client.read(); // char pwm2
//            Serial.print("inv"); Serial.print(ctl[0]); Serial.print(ctl[1]); Serial.println(ctl[2]);  
//            inputPWM[1] = 100 * (int)(ctl[0] - '0') + 10 * (ctl[1] - '0') + (ctl[2] - '0');
//            if (inputPWM[1] == 0) {
//              dirMotor = 0;
//            }
//            Serial.println(dirMotor);
//            Serial.print("pwm-:"); Serial.println(inputPWM[1]);
//            break;
            
//          case ',':
//            steering = client.read();
//            if (steering == 'R') {
//              Serial.println("R");
//            } else if (steering == 'L') {
//              Serial.println("L");
//            } else if (steering == 'C') {
//              Serial.println("C");
//            } else {
              // nothing
//            }
//            break;
//            
//          default:
//            break;
//        } // endswitch
        /***********************************************************************************/
        //Serial.println("Break Point!!!!!!!!!");
        if(logout != true) {
          //Serial.println("Break Point_logout");
          // input steering
          if (steering == 'R') {
            digitalWrite(pinL, LOW);
            digitalWrite(pinR, HIGH);
//              analogWrite(pinL, 1); // pinL 11
//              analogWrite(pinR, 255); // pinR 10
          } else if (steering == 'L') {
            digitalWrite(pinL, HIGH);
            digitalWrite(pinR, LOW);
//              analogWrite(pinL, 255);
//              analogWrite(pinR, 1);
          } else { // C
            digitalWrite(pinL, LOW);
            digitalWrite(pinR, LOW);
//              analogWrite(pinL, 1);
//              analogWrite(pinR, 1); 
          } 
          
          // input pwm
          if (dirMotor > 0) {
  //          Serial.println("dirMotor > 0");
            analogWrite(pinMtrPwm1, inputPWM[0]);
            digitalWrite(pinMtrPwm2, LOW);
//            analogWrite(pinMtrPwm2, 1);
          } else if (dirMotor < 0) {
  //          Serial.println("dirMotor < 0");
//            analogWrite(pinMtrPwm1, 1);
            digitalWrite(pinMtrPwm1, LOW);
            analogWrite(pinMtrPwm2, inputPWM[1]);
          } else { // dirMotor == 0
  //          Serial.println("dirMotor = 0");
//            analogWrite(pinMtrPwm1, 1);
//            analogWrite(pinMtrPwm2, 1);
              digitalWrite(pinMtrPwm1, LOW);
              digitalWrite(pinMtrPwm2, LOW);
          }
        } else {
          //Serial.println("Break Point@");
          socketTimeCount = 0;
          logout = false;
          //client.flush();
          client.stop();
          Serial.println("pass through client.connected()");
          delay(500);
          break;
        }
        //Serial.println("Break point!!!!");
      } else {
        // ソケットタイムアウトカウントをインクリメント
        Serial.println("socketTimeCount increase !");
        socketTimeCount++;
        Serial.print("socketTimeCount : ");
        Serial.println(socketTimeCount);
      } // endif(client.available() > 0)

      // 一定時間クライアントから応答がなければ、サーバー側からSocketを切る * client.connected()のバグ対策
      if (socketTimeCount > socketTimeOut) {
       socketTimeCount = 0;
       Serial.println("Socket TimeOut. close Socket from this server");
       client.flush();
       client.stop();
       delay(500);
       break; // pass while
      }
      
    } // endwhile
  } // endif(client)
} // endloop

