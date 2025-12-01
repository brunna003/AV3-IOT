/*
INSTRUÇÕES PARA CONFIGURAR:
1) Altere o nome do Bluetooth na linha:
      BluetoothSerial BT;
      BT.begin("NOME_DO_SEU_ESP32");

2) Abra o Serial Monitor em 115200.
3) Procure no celular/PC pelo nome configurado e conecte.
4) Envie mensagens — elas aparecerão no Serial Monitor.

Nada mais precisa ser configurado.
*/

#include <BluetoothSerial.h>

BluetoothSerial BT;

void setup() {
  Serial.begin(115200);

  BT.begin("ESP32_Bluetooth");  // <-- coloque o nome que quiser
  Serial.println("Bluetooth iniciado. Pode conectar!");
}

void loop() {
  // Se receber algo pelo Bluetooth, mostra no serial
  if (BT.available()) {
    String msg = BT.readString();
    Serial.print("Recebido via BT: ");
    Serial.println(msg);
  }

  // Se digitar algo no serial, envia via Bluetooth
  if (Serial.available()) {
    String msg = Serial.readString();
    BT.println(msg);
  }
}
