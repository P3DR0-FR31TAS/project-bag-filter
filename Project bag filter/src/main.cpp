// BIBLIOTECAS
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Waveshare4InchTftShield.h>

// PROTOTIPOS
void drawValvesMenu(void);
void drawHomeScreen(bool buttonState);

// CORES 16-BITS
#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF
#define DARKER_GREEN 0x0706

// CRIAÇÃO DO OBJETO WAVESHIELD E REFERENCIA TFT
namespace
{
  // Cria uma instância do objeto Waveshare4InchTftShield chamada Waveshield
  Waveshare4InchTftShield Waveshield;
  // Declara uma referência tft do tipo Adafruit_GFX e associa-a ao objeto Waveshield
  Adafruit_GFX &tft = Waveshield;
}

// VARIAVEIS GLOBAIS
#define MINPRESSURE 300
#define MAXPRESSURE 400

bool iniciarPrograma = false;

int screenWidth;
int screenHeight;
int buttonWidth;
int buttonHeight;
int buttonX;
int buttonY;

// DEFINE CONSTANTES PARA REPRESENTAR OS ESTADOS DAS PAGINAS
const int PAGINA_INICIAL = 0;
const int PAGINA_VALVULAS = 1;

// VARIAVEL PARA ARMAZENAR O ESTADO ATUAL DA PAGINA
int paginaAtual = PAGINA_INICIAL;

// ESTADO DO BUTAO
bool estadoButao = false;

//***************************************************************//
// MENU VALVULAS
//**************************************************************//
void drawValvesMenu()
{
  // DEFINE O TAMAHO E COR DO TITULO
  tft.setTextSize(3);
  tft.setTextColor(BLACK);

  // RETANGULO DO TITULO
  tft.fillRect(0, 0, 480, 50, BLUE);

  // ALINHA E IMPRIME NO DISPLAY "VALVULAS"
  uint16_t textWidth, textHeight;
  tft.getTextBounds("VALVULAS", 0, 0, nullptr, nullptr, &textWidth, &textHeight);

  int16_t textX = (tft.width() - textWidth) / 2; // Centraliza o texto horizontalmente
  int16_t textY = 15;                            // Posição vertical do texto

  tft.setCursor(textX, textY);
  tft.print("VALVULAS");

  screenWidth = tft.width();
  screenHeight = tft.height();
  buttonWidth = 140;
  buttonHeight = 140;
  buttonX = (screenWidth - buttonWidth) / 2;
  buttonY = (screenHeight - buttonHeight) / 2;

  tft.fillRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 5, BLUE);
  tft.drawRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 5, WHITE);
  tft.setCursor(215, 150);
  tft.setTextSize(3);
  tft.print("V:1");

  tft.fillRoundRect(170, 250, 140, 50, 5, RED);
  tft.drawRoundRect(170, 250, 140, 50, 5, WHITE);
  tft.setCursor(187, 265);
  tft.setTextSize(3);
  tft.print("VOLTAR");

  tft.fillTriangle(100, 90, 100, 230, 20, 160, BLUE);
  tft.drawTriangle(100, 90, 100, 230, 20, 160, WHITE);

  tft.fillTriangle(380, 90, 380, 230, 460, 160, BLUE);
  tft.drawTriangle(380, 90, 380, 230, 460, 160, WHITE);
}

//***************************************************************//
// MENU PRINCIPAL
//**************************************************************//
void drawHomeScreen(bool buttonState)
{
  // DEFINE O TAMAHO E COR DO TITULO
  tft.setTextSize(3);
  tft.setTextColor(DARKER_GREEN);

  // RETANGULO DO TITULO
  tft.drawRoundRect(0, 0, 480, 50, 5, DARKER_GREEN);

  // ALINHA E IMPRIME NO DISPLAY "MENU PRINCIPAL"
  uint16_t textWidth, textHeight;
  tft.getTextBounds("MENU PRINCIPAL", 0, 0, nullptr, nullptr, &textWidth, &textHeight);

  int16_t textX = (tft.width() - textWidth) / 2; // Centraliza o texto horizontalmente
  int16_t textY = 15;                            // Posição vertical do texto

  tft.setCursor(textX, textY);
  tft.print("MENU PRINCIPAL");

  screenWidth = tft.width();
  screenHeight = tft.height();
  buttonWidth = 100;
  buttonHeight = 100;
  buttonX = (screenWidth - buttonWidth) / 2;
  buttonY = (screenHeight - buttonHeight) / 2 - 50;

  //INFO
  tft.drawRoundRect(0, 55, 190, 265, 5, DARKER_GREEN);
  tft.setCursor(60, 65);
  tft.setTextSize(3);
  tft.print("INFO");

  //PRESSÃO
  tft.drawRoundRect(11, 120, 60, 30, 0, WHITE);
  tft.setCursor(30, 100);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.print("Kp");
  tft.setTextColor(GREEN);
  tft.setCursor(20, 130);
  tft.print("100");

  // NÚMERO DE CICLOS
  tft.drawRoundRect(120, 120, 60, 30, 0, WHITE);
  tft.setCursor(140, 100);
  tft.setTextColor(WHITE);
  tft.print("Num C");
  tft.setTextColor(GREEN);
  tft.setCursor(135, 130);
  tft.print("100");

  if (buttonState == false)
  {
    // BUTÃO INCIAR PROGRAMA
    tft.drawRoundRect(buttonX + 5, buttonY - 5, buttonWidth, buttonHeight, 5, DARKER_GREEN);
    tft.setTextColor(GREEN); // Define a cor do texto para branco
    tft.setTextSize(3);      // Define o tamanho do texto para 3
    tft.setCursor(225, 95);
    tft.println("ON"); // Escreve o texto no botão
  }
  else if (buttonState == true)
  {
    // BUTÃO ENCERRAR PROGRAMA
    tft.drawRoundRect(buttonX + 5, buttonY - 5, buttonWidth, buttonHeight, 5, RED);
    tft.setTextColor(RED); // Define a cor do texto para branco
    tft.setTextSize(3);      // Define o tamanho do texto para 3
    tft.setCursor(220, 95);
    tft.println("OFF"); // Escreve o texto no botão
  }

  // BOTÃO AJUSTAR VALVULAS
  int ajustesButtonY = buttonY + buttonHeight + 20; // POSIÇÃO VERTICAL DO BOTAO VALVULAS
  tft.drawRoundRect(buttonX + 5, ajustesButtonY - 20, buttonWidth, buttonHeight, 5, DARKER_GREEN);
  tft.setTextColor(DARKER_GREEN);
  tft.setTextSize(3);
  tft.setCursor(205, 200);
  tft.println("VALV.");

}

//***************************************************************//
// SETUP
//**************************************************************//
void setup()
{
  Serial.begin(9600);
  SPI.begin();
  Waveshield.begin();
  tft.setRotation(3);

  // DESENHA O MENU PRINCIPAL AO INICIAR
  drawHomeScreen(false);

  screenWidth = tft.width();
  screenHeight = tft.height();
  buttonWidth = 100;
  buttonHeight = 100;
  buttonX = (screenWidth - buttonWidth) / 2;
  buttonY = (screenHeight - buttonHeight) / 2 - 50;
}

//***************************************************************//
// LOOP
//**************************************************************//
void loop()
{
  // VARIAVEIS PARA LER O TOUCH
  TSPoint p = Waveshield.getPoint();
  Waveshield.normalizeTsPoint(p); // CALIBRA O TOUCH PARA A RESOLUÇAO E ROTAÇÃO DO DISPLAY

  if (paginaAtual == PAGINA_INICIAL)
  {
    // VERIFICA SE O TOQUE OCORREU DENTRO DOS LIMITES DO BOTÃO VALVULAS
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 220 && p.x < 320 && p.y > 160 && p.y < 260)
    {
      paginaAtual = PAGINA_VALVULAS;
      tft.fillScreen(BLACK);
      drawValvesMenu();
    }
    // VERIFICA SE O TOQUE OCORREU DENTRO DOS LIMITES DO BOTÃO INICIAR/ENCERRAR
    else if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 200 && p.x < 300 && p.y > 40 && p.y < 140)
    {
      // Verifique se o botão estava pressionado anteriormente
      if (!estadoButao)
      {
        // ATUALIZA O ESTADO DO BOTAO
        estadoButao = true;

        // AÇÃO QUANDO O BOTÃO É PRECIONADO
        iniciarPrograma = !iniciarPrograma;
        Serial.println(iniciarPrograma);

        // ALTERA A COR E O TEXTO DO BOTAO PARA VERMELHO E "ENCERRAR"
        if (iniciarPrograma)
        {
          // BUTÃO ENCERRAR PROGRAMA
          tft.drawRoundRect(buttonX + 5, buttonY - 5, buttonWidth, buttonHeight, 5, RED);
          tft.fillRoundRect(buttonX+10, buttonY+10, buttonWidth-20, buttonHeight-20, 5, BLACK);
          tft.setTextColor(RED); // Define a cor do texto para branco
          tft.setTextSize(3);      // Define o tamanho do texto para 3
          tft.setCursor(220, 95);
          tft.println("OFF"); // Escreve o texto no botão
        }
        else
        {
          // BUTÃO INCIAR PROGRAMA
          tft.drawRoundRect(buttonX + 5, buttonY - 5, buttonWidth, buttonHeight, 5, DARKER_GREEN);
          tft.fillRoundRect(buttonX+10, buttonY+10, buttonWidth-20, buttonHeight-20, 5, BLACK);
          tft.setTextColor(GREEN); // Define a cor do texto para branco
          tft.setTextSize(3);      // Define o tamanho do texto para 3
          tft.setCursor(225, 95);
          tft.println("ON"); // Escreve o texto no botão
        }
      }
      delay(200);
    }
    else
    {
      // Verifique se o estado do botão mudou
      if (estadoButao)
      {
        // Atualize o estado do botão
        estadoButao = false;

      }
    }
  }
  else if (paginaAtual == PAGINA_VALVULAS)
  {
    // VERIFICA SE O TOQUE OCORREU DENTRO DOS LIMITES DO BOTÃO VOLTAR
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 170 && p.x < 330 && p.y > 250 && p.y < 300)
    {
      paginaAtual = PAGINA_INICIAL;
      tft.fillScreen(BLACK);
      drawHomeScreen(iniciarPrograma);
    }
  }

  // DEBUG PARA CALIBRAR TOUCH DOS BOTOES
  Serial.print("X: ");
  Serial.print(p.x);
  Serial.print("    ");
  Serial.print("Y: ");
  Serial.print(p.y);
  // Serial.print("    ");
  // Serial.print("Z: ");
  // Serial.print(p.z);
  Serial.println();
}