#ifndef GPS_H
#define GPS_H

#include <Arduino.h>

// --- Configuração dos pinos do GPS ---
#define SERIAL1_RX 34
#define SERIAL1_TX 12

// --- Buffer de leitura para armazenar sentenças NMEA ---
String gpsBuffer = "";

// --- Inicializa o GPS ---
void setupGPS() {
  Serial1.begin(9600, SERIAL_8N1, SERIAL1_RX, SERIAL1_TX);
  Serial1.setTimeout(100); // Reduz o tempo de bloqueio
  delay(500);
  Serial.println("GPS iniciado e aguardando fix...");
}

// --- Função para separar campos NMEA por vírgulas ---
String sentence_sep(const String &data, int index) {
  int start = 0;
  int end = -1;
  int count = 0;

  while (count <= index) {
    start = end + 1;
    end = data.indexOf(',', start);
    if (end == -1) end = data.length();
    if (count == index) {
      return data.substring(start, end);
    }
    count++;
  }
  return "";
}

// --- Converte coordenadas NMEA (ddmm.mmmm) → graus decimais ---
float convert_gps_coord(float raw, const String &dir) {
  if (raw == 0) return 0.0;

  int deg = int(raw / 100);
  float minutes = raw - (deg * 100);
  float decimal = deg + (minutes / 60.0);

  if (dir == "S" || dir == "W") decimal *= -1;
  return decimal;
}

// --- Lê e converte dados do GPS (não bloqueante) ---
bool readGPS(float &lat, float &lon, float &alt, int &sat) {
  while (Serial1.available()) {
    char c = Serial1.read();
    if (c == '\n') {
      String sentence = gpsBuffer;
      gpsBuffer = "";  // limpa buffer após completar uma linha
      sentence.trim();

      if (!sentence.startsWith("$GPGGA")) continue;  // usa apenas sentenças GGA

      String latRaw = sentence_sep(sentence, 2);
      String latDir = sentence_sep(sentence, 3);
      String lonRaw = sentence_sep(sentence, 4);
      String lonDir = sentence_sep(sentence, 5);
      String satStr = sentence_sep(sentence, 7);
      String altStr = sentence_sep(sentence, 9);

      if (latRaw.isEmpty() || lonRaw.isEmpty()) return false;

      lat = convert_gps_coord(latRaw.toFloat(), latDir);
      lon = convert_gps_coord(lonRaw.toFloat(), lonDir);
      alt = altStr.toFloat();
      sat = satStr.toInt();

      // Evita leituras inválidas (sem fix)
      if (lat == 0 || lon == 0) return false;

      return true;  // dados válidos
    } else {
      gpsBuffer += c;
      if (gpsBuffer.length() > 120) gpsBuffer = ""; // evita overflow
    }
  }

  return false;  // nada novo ainda
}

#endif
