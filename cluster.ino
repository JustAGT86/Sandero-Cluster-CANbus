#include <SPI.h>
#include <mcp_can.h>

const int SPI_CS_PIN = 10;
MCP_CAN CAN(SPI_CS_PIN);

unsigned long intervaloEnvio = 500;
unsigned long ultimoEnvio = 0;

void setup() {
  Serial.begin(115200);

  if (CAN.begin(MCP_ANY, CAN_500KBPS, MCP_8MHZ) == CAN_OK) {
    Serial.println("✅ CAN iniciado com sucesso");
  } else {
    Serial.println("❌ Erro ao iniciar CAN");
    while (1)
      ;
  }

  CAN.setMode(MCP_NORMAL);
}

void loop() {
  unsigned long agora = millis();

  if (agora - ultimoEnvio >= intervaloEnvio) {

    //ligar cluster
    byte pacoteVivo[8] = { 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    CAN.sendMsgBuf(0x350, 0, 8, pacoteVivo);

    // RPM
    byte pacoteRPM[7] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    CAN.sendMsgBuf(0x186, 0, 7, pacoteRPM);

    // Velocidade
    byte pacoteVel[8] = {0xFF, 0x00, 0x00, 0x80, 0x80, 0x00, 0x00, 0x00};
    CAN.sendMsgBuf(0x217, 0, 8, pacoteVel);

    // Luz
    byte pacoteLuz[8] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    CAN.sendMsgBuf(0x5DE, 0, 8, pacoteLuz);

    // Câmbio auto tela
    //byte pacotePRNDL[3] = {0x0F, 0x00, 0x04};
    //CAN.sendMsgBuf(0x3F7, 0, 3, pacotePRNDL);

    // pedir Kilometragem tela
    //byte pacoteKM[8] = {0x06, 0x62, 0x02, 0x07, 0xFF, 0xFF, 0xFF, 0x00};  //km fica localizado nos bytes 5, 6 e 7
    //CAN.sendMsgBuf(0x763, 0, 8, pacoteKM);

    // Luzes secundarias(handbrake, TC off, TC working blinking, mais?)
    byte pacoteTC[4] = {0x30, 0x00, 0x00, 0x00};
    CAN.sendMsgBuf(0x666, 0, 4, pacoteTC);

    // airbag
    byte pacoteAirbag[4] = {0x00, 0x40, 0x08, 0x00};
    CAN.sendMsgBuf(0x653, 0, 4, pacoteAirbag);

    // tela?
    //byte pacoteTeste[2] = {0x32, 0x00};
    //CAN.sendMsgBuf(0x671, 0, 2, pacoteTeste);

    // Exemplo: combustível cheio
    //byte pacoteECU[8] = {0x00, 0x00, 0x00, 0x00, 0x12, 0x00, 0x00, 0x00};
    //CAN.sendMsgBuf(0x365, 0, 8, pacoteECU);

    // testes
    //byte pacoteTeste[8] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    //CAN.sendMsgBuf(0x3df, 0, 8, pacoteTeste);

    ultimoEnvio = agora;
  }
}
