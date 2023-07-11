// BIBLIOTECAS
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Waveshare4InchTftShield.h>

//PROTOTIPOS 
void drawValvulasMenu(void);
void drawHomeScreen(void);

// CORES 16-BITS
#define	BLACK   0x0000
#define	BLUE    0x001F
#define	RED     0xF800
#define	GREEN   0x07E0
#define CYAN    0x07FF
#define MAGENTA 0xF81F
#define YELLOW  0xFFE0
#define WHITE   0xFFFF
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

void drawValvulasMenu()
{
  // DEFINE O TAMAHO E COR DO TITULO
  tft.setTextSize(3); 
  tft.setTextColor(BLACK);

  // RETANGULO DO TITULO
  tft.fillRect(20, 20, 440, 30, BLUE);
  
  // ALINHA E IMPRIME NO DISPLAY "VALVULAS" 
  int16_t textWidth, textHeight;
  tft.getTextBounds("VALVULAS", 0, 0, nullptr, nullptr, &textWidth, &textHeight);

  int16_t textX = (tft.width() - textWidth) / 2; // Centraliza o texto horizontalmente
  int16_t textY = 25; // Posição vertical do texto

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

void drawHomeScreen()
{
  // DEFINE O TAMAHO E COR DO TITULO
  tft.setTextSize(3); 
  tft.setTextColor(BLACK);

  // RETANGULO DO TITULO
  tft.fillRect(20, 20, 440, 30, BLUE);
  
  // ALINHA E IMPRIME NO DISPLAY "MENU PRINCIPAL" 
  int16_t textWidth, textHeight;
  tft.getTextBounds("MENU PRINCIPAL", 0, 0, nullptr, nullptr, &textWidth, &textHeight);

  int16_t textX = (tft.width() - textWidth) / 2; // Centraliza o texto horizontalmente
  int16_t textY = 25; // Posição vertical do texto

  tft.setCursor(textX, textY);
  tft.print("MENU PRINCIPAL");

  screenWidth = tft.width();
  screenHeight = tft.height();
  buttonWidth = 160;
  buttonHeight = 70;
  buttonX = (screenWidth - buttonWidth) / 2;
  buttonY = (screenHeight - buttonHeight) / 2 - 50;

  // BUTÃO INCIAR PROGRAMA 
  tft.fillRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 5, DARKER_GREEN);
  tft.drawRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 5, WHITE);
  tft.setTextColor(BLACK); // Define a cor do texto para branco
  tft.setTextSize(3); // Define o tamanho do texto para 3
  tft.setCursor(buttonX + (buttonWidth - 30) / 4 - 17, buttonY + (buttonHeight - textHeight) / 2);
  tft.println("INICIAR"); // Escreve o texto no botão

  Serial.print(buttonX + (buttonWidth - 30) / 4 - 17);
  Serial.print("    ");
  Serial.print(buttonY + (buttonHeight - textHeight) / 2);

  // BOTÃO AJUSTAR VALVULAS
  int ajustesButtonY = buttonY + buttonHeight + 20;// POSIÇÃO VERTICAL DO BOTAO VALVULAS
  tft.fillRoundRect(buttonX, ajustesButtonY, buttonWidth, buttonHeight, 5, BLUE);
  tft.drawRoundRect(buttonX, ajustesButtonY, buttonWidth, buttonHeight, 5, WHITE);
  tft.setTextColor(BLACK);
  tft.setTextSize(3);
  tft.setCursor(buttonX + (buttonWidth - 50) / 4 - 17, ajustesButtonY + (buttonHeight - 8) / 3 + 5);
  tft.println("VALVULAS");

  // Informação das válvulas ativas
  int valveInfoWidth = 80;
  int valveInfoHeight = 30;
  int valveInfoX = tft.width() - valveInfoWidth - 10; // Posição X 
  int valveInfoY = 280; // Posição Y 

  tft.fillRoundRect(valveInfoX, valveInfoY, valveInfoWidth, valveInfoHeight, 5, BLUE);
  tft.drawRoundRect(valveInfoX, valveInfoY, valveInfoWidth, valveInfoHeight, 5, WHITE);
  tft.setTextColor(BLACK);
  tft.setTextSize(2);
  tft.setCursor(407, 287);
  tft.println("V: 0");
}

void setup() 
{
  Serial.begin(9600);
  SPI.begin();
  Waveshield.begin();
  tft.fillScreen(BLACK);
  tft.setRotation(3);

  // DESENHA O MENU INCIAL DE COMEÇO
  drawHomeScreen();
  
  screenWidth = tft.width();
  screenHeight = tft.height();
  buttonWidth = 160;
  buttonHeight = 70;
  buttonX = (screenWidth - buttonWidth) / 2;
  buttonY = (screenHeight - buttonHeight) / 2 - 50;
}

void loop() 
{
  // VARIAVEIS PARA O TOUCH
  TSPoint p = Waveshield.getPoint();
  Waveshield.normalizeTsPoint(p); // CALIBRA O TOUCH PARA A RESOLUÇAO E ROTAÇÃO DO DISPLAY

  if (paginaAtual == PAGINA_INICIAL) 
  {
    // VERIFICA SE O TOQUE OCORREU DENTRO DOS LIMITES DO BOTÃO VALVULAS
    if (p.z > 10 && p.x > 160 && p.x < 330 && p.y > 170 && p.y < 230) 
    {
      paginaAtual = PAGINA_VALVULAS;
      tft.fillScreen(BLACK);
      drawValvulasMenu();
    }
    else if (p.z > 10 && p.x > 160 && p.x < 330 && p.y > 70 && p.y < 140)
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
          tft.fillRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 5, RED);
          tft.drawRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 5, WHITE);
          tft.setTextColor(BLACK); // Define a cor do texto para branco
          tft.setTextSize(3); // Define o tamanho do texto para 3
          tft.setCursor(170, 97);
          tft.println("ENCERRAR"); // Escreve o texto no botão
        }
        else
        {
          // BUTÃO INCIAR PROGRAMA 
          tft.fillRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 5, DARKER_GREEN);
          tft.drawRoundRect(buttonX, buttonY, buttonWidth, buttonHeight, 5, WHITE);
          tft.setTextColor(BLACK); // Define a cor do texto para branco
          tft.setTextSize(3); // Define o tamanho do texto para 3
          tft.setCursor(175, 98);
          tft.println("INICIAR"); // Escreve o texto no botão
        }
      }
    }
    else
    {
      // Verifique se o estado do botão mudou
      if (estadoButao)
      {
        // Atualize o estado do botão
        estadoButao = false;

        // Ação quando o botão é liberado
      }
    }
  } 
  else if (paginaAtual == PAGINA_VALVULAS) 
  {
    // VERIFICA SE O TOQUE OCORREU DENTRO DOS LIMITES DO BOTÃO VOLTAR
    if (p.z > 10 && p.x > 170 && p.x < 330 && p.y > 250 && p.y < 300) 
    {
      paginaAtual = PAGINA_INICIAL;
      tft.fillScreen(BLACK);
      drawHomeScreen();
    }
  }

  // DEBUG PARA CALIBRAR TOUCH DOS BOTOES
  // Serial.print("X: ");
  // Serial.print(p.x);
  // Serial.print("    ");
  // Serial.print("Y: ");
  // Serial.print(p.y);
  // Serial.println();
}

