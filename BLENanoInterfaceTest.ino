// Using BLE-Peripheral by Sandeep Mistry, 
// This sketch is supposed to send data via BLE to xdrip+ to check toe BLE-Interface.

// Import libraries (BLEPeripheral depends on SPI)
#include <SPI.h>
#include <BLEPeripheral.h>


//BLE decriptors and Characteristics
   BLEService _uartService = BLEService("0000ffe0-0000-1000-8000-00805f9b34fb");
   BLEDescriptor _uartNameDescriptor = BLEDescriptor("2901", "UART");
   BLECharacteristic _txCharacteristic = BLECharacteristic("0000ffe1-0000-1000-8000-00805f9b34fb",BLEWriteWithoutResponse|BLENotify|BLERead, 20);
   BLEDescriptor _txNameDescriptor = BLEDescriptor("2901", "TX - Transfer Data (Notify)");
   BLEDescriptor _ConfigServiceDescriptor = BLEDescriptor("2902", "3");



// create ble serial instance on BLE nano V2
BLEPeripheral BLESerial(-1, -1,-1);
BLEBondStore                     bleBondStore;

//Build packet from JOERNL's LimiTTer
String Build_Packet(float glucose,int batteryPcnt, long sensorMinutesElapse,int batteryMv) {
  
// Let's build a String which xDrip accepts as a BTWixel packet

      unsigned long raw = glucose*1000; // raw_value
      String packet = "";
      packet = String(raw);
      packet += ' ';
      packet += "216";
      packet += ' ';
      packet += String(batteryPcnt);
      packet += ' ';
      packet += String(sensorMinutesElapse);
      Serial.println("");
      Serial.print("Glucose level: ");
      Serial.print(glucose);
      Serial.println("");
      Serial.print("15 minutes-trend: ");
      Serial.println("");
   /*   for (int i=0; i<16; i++)
      {
        Serial.print(trend[i]);
        Serial.println("");
      }*/
      Serial.print("Battery level: ");
      Serial.print(batteryPcnt);
      Serial.print("%");
      Serial.println("");
      Serial.print("Battery mVolts: ");
      Serial.print(batteryMv);
      Serial.print("mV");
      Serial.println("");
      Serial.print("Sensor lifetime: ");
      Serial.print(sensorMinutesElapse);
      Serial.print(" minutes elapsed");
      Serial.println("");
      return packet;
}


void blePeripheralConnectHandler(BLECentral& central) {
  // central connected event handler
  Serial.print(F("Connected event, central: "));
  Serial.println(central.address());
}

void blePeripheralBondedHandler(BLECentral& central) {
  // central bonded event handler
  Serial.print(F("Remote bonded event, central: "));
  Serial.println(central.address());

 // if (ancsNotificationSourceCharacteristic.canSubscribe()) {
   // ancsNotificationSourceCharacteristic.subscribe();
 // }
}

void setup() {
  // custom services and characteristics can be added as well
  BLESerial.setLocalName("LimiTTerNano");
  BLESerial.setConnectable(true);
  bleBondStore.clearData();
  BLESerial.setBondStore(bleBondStore);
  Serial.begin(9600);
  BLESerial.setEventHandler(BLEConnected, blePeripheralConnectHandler);
  BLESerial.setEventHandler(BLEBonded, blePeripheralBondedHandler);
  BLESerial.setAdvertisedServiceUuid(_uartService.uuid());
  BLESerial.addAttribute(_uartService);
  BLESerial.addAttribute(_txCharacteristic);
  BLESerial.addAttribute(_txNameDescriptor);
  BLESerial.addAttribute(_ConfigServiceDescriptor);
  _txCharacteristic.setValue("200352 216 100 15551");
  
  BLESerial.begin();
}


int secs=0;//timer

void loop() {
  
  //BLESerial.poll();
  
  String tmp=Build_Packet(100,90, 15551+secs/60,1000);//create an xdrip Package with glucose value 100, 90% abttery life, a sensor time of 15551+ elapsed time
  char buf[20];
  tmp.toCharArray(buf, 20);
if(secs%5==0){//send package every 5 seconds for testing purpose
  
_txCharacteristic.setValue(buf);


}
//BLESerial.print("228352 216 100 15551");
BLESerial.poll();
  //forward();
  delay(1000);
  secs=secs+1;
  // loopback();
  // spam();
}

