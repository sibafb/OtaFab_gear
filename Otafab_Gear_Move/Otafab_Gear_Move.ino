/****  ****/

#define MoterSpeedWaitMsec 2000
int x;
/********* Pin_assign *********/
/**** Using TMC2100 Module ****/
//datasheet : https://www.trinamic.com/fileadmin/assets/Products/ICs_Documents/TMC2100_datasheet.pdf
//Ena  : Enable
//Dir  : Motor Direction
//Step : Step Signal
//VIO  : IO VCC
//M1B  :Black
//M1A  :Green
//M2A  :Red
//M2B  : Blue
//VM   :MotorPower
//CFG1 : Config1
//CFG2 : Config2
//CFG3 : Config3
const int enable_pin = 10;
const int step_pin = 8;
const int dir_pin = 9;
const int cfg1_pin = 11;
const int cfg2_pin = 6;
const int cfg3_pin = 7;

/**** Using Some Pins for Config ****/
//const int mode_pin = 8;
//const int mode = 0; //0:ただ回転　１：1回転ずつ

/**** Power LEDs ****/
int pwr_led_pin = 13;
int zero_led_pin = 12;

bool zeroflag = false;

/**** Function Prototyoe ****/
void IO_Initialize();
void DriveXstep(int step_number,int dir);

void setup() { 
  IO_Initialize();
}

void loop() {
  //if(digitalRead(mode_pin) == HIGH){
    //400Stepで半回転
  //  DriveXstep(400,HIGH);
 // }else{
    
    if(zeroflag == true){
     
     noInterrupts();
     //一定時間割り込み空処理で回す ※リードスイッチの誤検知対策、半回転させてから通常動作に戻す
     digitalWrite(zero_led_pin,HIGH); // 
     DriveXstep(350,HIGH);
     zeroflag = LOW;
     digitalWrite(zero_led_pin,LOW); // 
     interrupts();
    }else{
      DriveXstep(2,HIGH);
      //digitalWrite(led_reed_switch,led_reed_switch_state);
      //delay入れるとガタつく
      //delay(4);
    }
  //}
}
void IO_Initialize(){
  pinMode(enable_pin,OUTPUT);  // Enable　→　Arduinoのenable_pin番ピンへ
  pinMode(step_pin,OUTPUT);    // Step →　Arduinoのstep_pin番ピンへ
  pinMode(dir_pin,OUTPUT);     // Dir →　Arduinoのdir_pin番ピンへ
  pinMode(cfg1_pin,OUTPUT);    // CFG1 →　Arduinoのcfg1_pin番ピンへ
  pinMode(cfg2_pin,OUTPUT);    // CFG2 →　Arduinoのcfg2_pin番ピンへ
  pinMode(cfg3_pin,OUTPUT);    // CFG3 →　Arduinoのcfg3_pin番ピンへ

  pinMode(pwr_led_pin,OUTPUT); // 
  pinMode(zero_led_pin,OUTPUT);// 

  digitalWrite(enable_pin,LOW); // Set Enable low　→　Low状態でEnable

  digitalWrite(cfg1_pin,LOW); // Set Enable low　→　Low状態でEnable
  digitalWrite(cfg2_pin,HIGH); // Set Enable low　→　Low状態でEnable
  digitalWrite(cfg3_pin,LOW); // Set Enable low　→　Low状態でEnable

  digitalWrite(pwr_led_pin,HIGH); // Set Enable low　→　Low状態でEnable

  attachInterrupt(0, ZeroPoint, RISING);
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
    zeroflag = true;
    delayMicroseconds(MoterSpeedWaitMsec); // 2ｍｓ待って 
}