#define MoterSpeedWaitMsec 2000



/******** pin_assign ********/
int x;

int enable_pin = 12;
int step_pin = 11;
int dir_pin = 10;
int mode_pin = 8;
int mode = 0; //0:ただ回転　１：1回転ずつ

int led_pin = 9;

int interruptlock = 0;

int led_reed_switch = 13;
volatile int led_reed_switch_state = 0;

volatile int zeroflag = LOW;

void DriveXstep(int step_number,int dir);

/*
おおたfabの施策用に作ったプログラム
とりあえず回る
*/

void setup() { 
  pinMode(enable_pin,OUTPUT); // Enable　→　Arduinoのenable_pin番ピンへ
  pinMode(step_pin,OUTPUT); // Step →　Arduinoのstep_pin番ピンへ
  pinMode(dir_pin,OUTPUT); // Dir →　Arduinoのdir_pin番ピンへ
  pinMode(led_reed_switch,OUTPUT); // Dir →　Arduinoのdir_pin番ピンへ
  digitalWrite(enable_pin,LOW); // Set Enable low　→　Low状態でEnable
  attachInterrupt(0, ZeroPoint, RISING);
  
}

void loop() {
  //if(digitalRead(mode_pin) == HIGH){
    //400Stepで半回転
  //  DriveXstep(400,HIGH);
 // }else{
    
    if(zeroflag == HIGH){
     
     delay(2000);
     
     noInterrupts();
     //一定時間割り込み空処理で回す ※リードスイッチの誤検知対策、半回転させてから通常動作に戻す
     DriveXstep(350,HIGH);
     zeroflag = LOW;
     interrupts();
    }else{
      DriveXstep(2,HIGH);
      digitalWrite(led_reed_switch,led_reed_switch_state);
      //delay入れるとガタつく
      //delay(4);
    }
  //}
}


void DriveXstep(int step_number,int dir){
   digitalWrite(dir_pin,HIGH); // Set Dir High　→　回転方向をセット
  for(x = 0; x < step_number; x++) // 括弧内を２００回実行
  {
    digitalWrite(step_pin,HIGH); // step_pin番ピンをHigh状態へ
    delayMicroseconds(MoterSpeedWaitMsec); // 2ｍｓ待って
    digitalWrite(step_pin,LOW); // step_pin番ピンをLow状態へ
    delayMicroseconds(MoterSpeedWaitMsec); // 2ｍｓ待って 
  }
}
void ZeroPoint(){
  if(interruptlock == 0){
    zeroflag = !zeroflag;
    led_reed_switch_state = !led_reed_switch_state;
  }else{
  }
}