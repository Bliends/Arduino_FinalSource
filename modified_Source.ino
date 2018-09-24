#define S0 2    //컬러센서 S0 == 2
#define S1 3    //컬러센서 S1 == 3
#define S2 4    //컬러센서 S2 == 4
#define S3 5    //컬러센서 S3 == 5
#define sensorOut 6 //컬러센서 out == 6
/*
 * 참고로 컬러센서의 경우 VCC == 5V, GND == GND, OE == GND에 꼽아주세요
 * 블루투스 모듈은 5V == 3.3V, GND == GND로 꼽아주셔도 됩니다.
 * 대신 전력은 PC와 연결해서 계속 공급해주세요
 * 아아! 참고로 제가 드린 아두이노 보드 외에, 저희는 아두이노 프로 미니 라는 제품을 사용할꺼에요! 참고해주셨으면 좋겠습니다.
 * 인식은 현금의 앞쪽(현금의 점자있는곳, 세종대왕있는곳)의 왼쪽 아래 금액써져있는 부분을 인식하시면 됩니다.
 * 점자있는곳을 기준으로 잡는게 편할꺼라 생각해서 이렇게 잡았습니다.
*/


#include <SoftwareSerial.h>

SoftwareSerial BTSerial(12, 13);    //블루투스 모듈 RX, TX 부분 12번,13번에 꼽아주세요

int R, G, B;    
int t1 = 0, t5 = 0, m1 = 0, m5 = 0, N = 0, limit = 0;

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

   
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);

  Serial.begin(9600);
  BTSerial.begin(9600);
}
 
void loop() {
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  R = pulseIn(sensorOut, LOW);
  Serial.print("R= ");
  Serial.print(R);
  Serial.print("  ");
  delay(100);   //R인식 속도 

 
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  G = pulseIn(sensorOut, LOW);
  Serial.print("G= ");
  Serial.print(G);
  Serial.print("  ");
  delay(100);   //G인식 속도
 
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  B = pulseIn(sensorOut, LOW);
  Serial.print("B= ");
  Serial.print(B);
  Serial.println("  ");
  delay(600);   ///B인식 속도

  //이부분 수정하실 때 값이 겹치지 않게 수정해주세요
    if((R > B) && (G > B) && (R >= G) && (R <= 16) && (B < 11)){ //(1000원)이부분 수정해서 인식 값변경, 이황 옆부분 인식
      if((R > 50) && (G > 50) && (B > 50)){
        Serial.println("");
        Serial.print("No");
        Serial.println("");
        ++N;
      }
      else{
        Serial.println("");
        Serial.print("1,000");
        Serial.println("");
        ++t1;
      }
    }
    else if((R > B) && (G >= B) && ((G >= R) || ((R <= 17) && (R >= 12))) && (G >= 12)){  //(5000원)이부분 수정해서 인식 값변경, 이이&이이옆부분 인식
      if((R > 50) && (G > 50) && (B > 50)){
        Serial.println("");
        Serial.print("No");
        Serial.println("");
        ++N;
      }
      else{
        Serial.println("");
        Serial.print("5,000");
        Serial.println("");
        ++t5;
      }
    }
    else if((R > B) && (G >= B) && (R >= G) && (R >= 14) && (B >= 11)){  //(10000원)이부분 수정해서 인식 값변경, 세종대왕 인식
      if((R > 50) && (G > 50) && (B > 50)){
        Serial.println("");
        Serial.print("No");
        Serial.println("");
        ++N;
      }
      else{
        Serial.println("");
        Serial.print("10,000");
        Serial.println("");
        ++m1;
      }
    }
    else if((R >= B) && (B >= G) && (R >= G)){  //(50000원)이부분 수정해서 인식 값변경, 신사임당 옆?부분 인식...
      if((R > 50) && (G > 50) && (B > 50)){
        Serial.println("");
        Serial.print("No");
        Serial.println("");
        ++N;
      }
      else{
        Serial.println("");
        Serial.print("50,000");
        Serial.println("");
        ++m5;
      }
    }


    if(t1+t5+m1+m5+N == 6){ //이부분을 통해 몇번 인식을 했을 때 안드로이드로 값을 보낼것인지 선택
      limit = max(max(t1,t5) , max(m1,m5)); //최댓값 구하기
      limit = max(limit,N);   //최댓값 = limit변수 안으로
      if(limit == t1){
        Serial.println("");
        Serial.print("1t");   //(1000원)BTSerial 은 블루투스 모듈을 통해 안드로이드로 값을 보내는 친구임다. 이부분을 Seiral.print로 바꾸면 안드로이드 말고 시리얼 모니터로 확인 가능합니다.
        Serial.println("");
      }
      else if(limit == t5){   //(5000원)
        Serial.println("");
        Serial.print("5t");
        Serial.println("");
      }
      else if(limit == m1){   //(10000원)
        Serial.println("");
        Serial.print("1m");
        Serial.println("");
      }
      else if(limit == m5){   //(50000원)
        Serial.println("");
        Serial.print("5m");
        Serial.println("");
      }
      else{
        Serial.println("");
        Serial.print("No");
        Serial.println("");
      }
      delay(500);
      t1 = 0;
      t5 = 0;
      m1 = 0;
      m5 = 0;
      N = 0;
    }
}
