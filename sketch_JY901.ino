/*JY901を使用してyaw軸を読み取る
 *本プログラムはJY901のデータシートをもとに作成されています。 
 *JY901は加速度、ジャイロ、磁気の9軸でも読み取ることが出来ますが、外乱の影響を考えると6軸での使用をお勧めします。
 *自分も9軸での角度の算出を試みたのですが、上手くいきませんでした。
 *何かあったらTwitter@ninjinnokakoiまでよろしくお願いいたします。
 *
 *[URL]
 *データシート:http://wiki.wit-motion.com/english/lib/exe/fetch.php?media=module:wt901:docs:jy901usermanualv4.pdf
 *JY901(Amazon):https://www.amazon.co.jp/DingDong-JY901%E9%9B%BB%E5%AD%90%E3%82%B8%E3%83%A3%E3%82%A4%E3%83%AD%E3%82%B9%E3%82%B3%E3%83%BC%E3%83%97%E4%B8%89%E6%AC%A1%E5%85%83%E8%A7%92%E5%BA%A6%E3%82%BB%E3%83%B3%E3%82%B5%E3%83%A2%E3%82%B8%E3%83%A5%E3%83%BC%E3%83%ABMPU6050-9%E8%BB%B8%E3%82%B7%E3%83%AA%E3%82%A2%E3%83%AB%E3%83%9D%E3%83%BC%E3%83%88%E3%83%A2%E3%82%B8%E3%83%A5%E3%83%BC%E3%83%AB/dp/B06XBXX6F1
 *
 */

#include<Wire.h>

int yawl,yawh,rolll,rollh,pitchl,pitchh;
float yaw,roll,pitch;

void setup() {
Wire.begin();
Serial.begin(9600);


//ジャイロセンサの自動キャリブレーションモード実装
Wire.beginTransmission(0x50); //start communication 通信開始
Wire.write(0x63); //address　アドレスを送る
Wire.write(0x00); //set to automatic calibration　自動キャリブレーション実行モードを選択する
Wire.write(0); //dataH　上位ビット
Wire.endTransmission(); //end communication　通信終了

//JY901のreturn rate変更
Wire.beginTransmission(0x50); //start communication　通信開始
Wire.write(0x03); //address　アドレスを送る
Wire.write(0x09); //set to 100Hz　100Hzに設定する
Wire.write(0); //data H　上位ビット
Wire.endTransmission(); //end communication　通信終了

//JY901の6軸設定
Wire.beginTransmission(0x50); //start communiction　通信開始
Wire.write(0x24); //adrress　アドレスを送る
Wire.write(0x01); //set to 6-axis algorithm 6軸に設定　ここを0x00に変更することで9軸での使用が可能になります
Wire.write(0); //dataH　上位ビット
Wire.endTransmission(); //end communication　通信終了

//Yaw軸の0度設定
Wire.beginTransmission(0x50); //start communication　通信開始
Wire.write(0x01); //addres　アドレス選択
Wire.write(0x04); //set to angle　通信が開始された時点の角度が0度になるように設定
Wire.write(0); //data H　上位ビット
Wire.endTransmission(); //end communication　通信終了
}

void loop() {
//Z軸を読み取るアドレスを送信し、そのデータを読み取る
Wire.beginTransmission(0x50); //start communication　通信開始
Wire.write(0x3f); //address　アドレス選択
Wire.endTransmission(false); //end communication　通信終了
Wire.requestFrom(0x50,2); //request data　データ要求

while(Wire.available()<2){ //waiting data　データ待機
  Wire.beginTransmission(0x50);
  Wire.write(0x3f);
  Wire.endTransmission(false);
  Wire.requestFrom(0x50,2);
}
yawl = Wire.read();
yawh = Wire.read();

/*//X軸を読み取るアドレスを送信し、そのデータを読み取る
Wire.beginTransmission(0x50); //start communication
Wire.write(0x3d); //address
Wire.endTransmission(false); //end communication
Wire.requestFrom(0x50,2); //request data

while(Wire.available()<2){ //waiting data
  Wire.beginTransmission(0x50);
  Wire.write(0x3d);
  Wire.endTransmission(false);
  Wire.requestFrom(0x50,2);
}

rolll = Wire.read();
rollh = Wire.read();

//Y軸を読み取るアドレスを送信し、そのデータを読み取る
Wire.beginTransmission(0x50); //start communication
Wire.write(0x3e); //address
Wire.endTransmission(false); //end communication
Wire.requestFrom(0x50,2); //request data

while(Wire.available()<2){ //waiting data
  Wire.beginTransmission(0x50);
  Wire.write(0x3e);
  Wire.endTransmission(false);
  Wire.requestFrom(0x50,2);
}

pitchl = Wire.read();
pitchh = Wire.read();*/

//角度を算出する この値はデータシートを元にしています。
yaw = (float)((yawh<<8)|yawl)/32768*180;
/*roll = (float)((rollh<<8)|rolll)/32768*180;
pitch = (float)((pitchh<<8)|pitchl)/32768*180;

Serial.print("Roll:");
Serial.print(roll);
Serial.print("Pitch:");
Serial.print(pitch);*/
Serial.print("Yaw:");
Serial.println(yaw);
}
