// BIBLIOTECAS
#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Waveshare4InchTftShield.h>

// PROTOTIPOS
void drawValvesMenu(void);
void drawHomeScreen(bool buttonState);
void drawKpMenu(void);
void valveButton(int16_t positionX, int16_t positionY, int16_t cursorX, int16_t cursorY, char *button_text);

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

// VARIAVEIS COM OS VALORES DA RESOLUÇÃO DO DISPLAY
int screenWidth = tft.width();
int screenHeight = tft.height();

// TAMANHO GENERICO DOS BUTÕES
int buttonWidth = 100;
int buttonHeight = 100;

bool iniciarPrograma = false; // ESTADO DO PROGRAMA

// DEFINE CONSTANTES PARA REPRESENTAR OS ESTADOS DAS PAGINAS
const int home_screen = 0;
const int valves_menu = 1;
const int kp_menu = 2;

int current_page = home_screen;    // VARIAVEL PARA ARMAZENAR O ESTADO ATUAL DA PAGINA
bool program_button_state = false; // ESTADO DO BUTAO

//***************************************************************//
// FUNÇÃO PARA DESENHAR BUTAO DAS VALVULAS
//**************************************************************//
void valveButton(int16_t positionX, int16_t positionY, int16_t cursorX, int16_t cursorY, char *button_text)
{
  tft.drawRoundRect(positionX, positionY, 92, 92, 5, DARKER_GREEN);
  tft.setCursor(cursorX, cursorY);
  tft.setTextSize(3);
  tft.print(button_text);
}

//***************************************************************//
// MENU VALVULAS
//**************************************************************//
void drawValvesMenu()
{
  // BUTAO MENU PRINCIPAL
  tft.setTextSize(3);
  tft.setTextColor(DARKER_GREEN);
  tft.drawRoundRect(0, 0, 160, 50, 5, DARKER_GREEN);
  tft.fillTriangle(64, 25, 95, 25, 80, 10, DARKER_GREEN);
  tft.fillRect(70, 25, 20, 15, DARKER_GREEN);

  // BUTAO MENU VALVULAS
  tft.fillRoundRect(161, 0, 160, 50, 5, DARKER_GREEN);
  tft.fillTriangle(230 - 5, 15, 245 - 5, 25, 230 - 5, 35, BLACK);
  tft.fillTriangle(260 - 5, 15, 260 - 5, 35, 245 - 5, 25, BLACK);

  // BUTAO MENU PRESSAO
  tft.drawRoundRect(322, 0, 158, 50, 5, DARKER_GREEN);
  tft.drawCircle(401, 25, 18, DARKER_GREEN);
  tft.drawCircle(401, 25, 17, DARKER_GREEN);
  tft.fillCircle(401, 25, 5, DARKER_GREEN);
  tft.fillTriangle(396, 28, 406, 28, 401, 12, DARKER_GREEN);

  // PRIMEIRA FILA DOS BUTOES
  valveButton(0, 55, 35, 90, "V1");
  valveButton(98, 55, 138, 90, "V2");
  valveButton(195, 55, 240, 90, "V3");
  valveButton(291, 55, 330, 90, "V4");
  valveButton(388, 55, 420, 90, "V5");

  // SEGUNDA FILA DOS BUTÕES
  valveButton(0, 152, 35, 190, "V6");
  valveButton(98, 152, 138, 190, "V7");
  valveButton(195, 152, 240, 190, "V8");
  valveButton(291, 152, 330, 190, "V9");
  valveButton(388, 152, 420, 190, "V10");

  // AJUSTAR NUMERO DE VALVULAS
  tft.drawRoundRect(0, 249, 285, 70, 5, DARKER_GREEN);
  tft.setCursor(35, 275);
  tft.print("Valvulas:");
  tft.setTextColor(WHITE);
  tft.print("10");

  tft.setTextColor(GREEN);
  tft.drawRoundRect(291, 249, 92, 70, 5, DARKER_GREEN);
  tft.setCursor(330, 274);
  tft.print("+");

  tft.drawRoundRect(388, 249, 92, 70, 5, DARKER_GREEN);
  tft.setCursor(425, 274);
  tft.print("-");
}

//***************************************************************//
// MENU PRESSAO
//**************************************************************//
void drawKpMenu(void)
{
  // BUTAO MENU PRINCIPAL
  tft.setTextSize(3);
  tft.setTextColor(DARKER_GREEN);
  tft.drawRoundRect(0, 0, 160, 50, 5, DARKER_GREEN);
  tft.fillTriangle(64, 25, 95, 25, 80, 10, DARKER_GREEN);
  tft.fillRect(70, 25, 20, 15, DARKER_GREEN);

  // BUTAO MENU VALVULAS
  tft.drawRoundRect(161, 0, 160, 50, 5, DARKER_GREEN);
  tft.fillTriangle(230 - 5, 15, 245 - 5, 25, 230 - 5, 35, DARKER_GREEN);
  tft.fillTriangle(260 - 5, 15, 260 - 5, 35, 245 - 5, 25, DARKER_GREEN);

  // BUTAO MENU PRESSAO
  tft.fillRoundRect(322, 0, 158, 50, 5, DARKER_GREEN);
  tft.drawCircle(401, 25, 18, BLACK);
  tft.drawCircle(401, 25, 17, BLACK);
  tft.drawCircle(401, 25, 16, BLACK);
  tft.fillCircle(401, 25, 5, BLACK);
  tft.fillTriangle(396, 28, 406, 28, 401, 12, BLACK);
}

//***************************************************************//
// MENU PRINCIPAL
//**************************************************************//
void drawHomeScreen(bool buttonState)
{
  // BUTAO MENU PRINCIPAL
  tft.setTextSize(3);
  tft.setTextColor(DARKER_GREEN);
  tft.fillRoundRect(0, 0, 160, 50, 5, DARKER_GREEN);
  tft.fillTriangle(64, 25, 95, 25, 80, 10, BLACK);
  tft.fillRect(70, 25, 20, 15, BLACK);

  // BUTAO MENU VALVULAS
  tft.drawRoundRect(161, 0, 160, 50, 5, DARKER_GREEN);
  tft.fillTriangle(230 - 5, 15, 245 - 5, 25, 230 - 5, 35, DARKER_GREEN);
  tft.fillTriangle(260 - 5, 15, 260 - 5, 35, 245 - 5, 25, DARKER_GREEN);

  // BUTAO MENU PRESSAO
  tft.drawRoundRect(322, 0, 158, 50, 5, DARKER_GREEN);
  tft.drawCircle(401, 25, 18, DARKER_GREEN);
  tft.drawCircle(401, 25, 17, DARKER_GREEN);
  tft.fillCircle(401, 25, 5, DARKER_GREEN);
  tft.fillTriangle(396, 28, 406, 28, 401, 12, DARKER_GREEN);

  // PAINEL DE INFORMAÇÕES
  tft.drawRoundRect(0, 75, 321, 245, 5, DARKER_GREEN);
  tft.setCursor(130, 65);
  tft.setTextSize(3);
  tft.fillRect(130, 65, 73, 25, BLACK);
  tft.print("INFO");

  // PRESSÃO Kp
  tft.drawRect(136, 130, 60, 30, WHITE);
  tft.setCursor(156, 110);
  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.print("Kp");
  tft.setTextColor(GREEN);
  tft.setCursor(156, 138);
  tft.print("100");

  // NÚMERO DE CICLOS
  tft.setCursor(5, 300);
  tft.setTextColor(DARKER_GREEN);
  tft.print("Ciclos:");
  tft.setTextColor(WHITE);
  tft.print("100");

  // NÚMERO DE VALVULAS
  tft.setCursor(185, 300);
  tft.setTextColor(DARKER_GREEN);
  tft.print("Valvulas:");
  tft.setTextColor(WHITE);
  tft.print("10");

  // PAINEL DAS VALVULAS
  tft.drawRoundRect(325, 190, 155, 130, 5, DARKER_GREEN);
  tft.setCursor(378, 180);
  tft.setTextSize(3);
  tft.setTextColor(DARKER_GREEN);
  tft.fillRect(374, 180, 59, 25, BLACK);
  tft.print("VOn");

  tft.setTextSize(7);
  tft.setCursor(385, 235);
  tft.setTextColor(MAGENTA);
  tft.print("3");

  // PRESSÃO MINIMA

  buttonWidth = 155;
  buttonHeight = 100;

  if (buttonState == false)
  {
    // BUTÃO INCIAR PROGRAMA
    tft.drawRoundRect(325, 75, buttonWidth, buttonHeight, 5, DARKER_GREEN);
    tft.setTextColor(DARKER_GREEN); // Define a cor do texto para branco
    tft.setTextSize(3);             // Define o tamanho do texto para 3
    tft.setCursor(385, 115);
    tft.println("ON"); // Escreve o texto no botão
  }
  else
  {
    // BUTÃO ENCERRAR PROGRAMA
    tft.drawRoundRect(325, 75, buttonWidth, buttonHeight, 5, RED);
    tft.setTextColor(RED); // Define a cor do texto para branco
    tft.setTextSize(3);    // Define o tamanho do texto para 3
    tft.setCursor(375, 115);
    tft.println("OFF"); // Escreve o texto no botão
  }
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
  buttonWidth = 155;
  buttonHeight = 100;
}

//***************************************************************//
// LOOP
//**************************************************************//
void loop()
{
  // VARIAVEIS PARA LER O TOUCH
  TSPoint p = Waveshield.getPoint();
  Waveshield.normalizeTsPoint(p); // CALIBRA O TOUCH PARA A RESOLUÇAO E ROTAÇÃO DO DISPLAY

  if (current_page == home_screen)
  {
    // VERIFICA SE O TOCOU NO MENU VALVULAS
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 161 && p.x < 321 && p.y > 0 && p.y < 50)
    {
      current_page = valves_menu;
      tft.fillScreen(BLACK);
      drawValvesMenu();
    }
    // VERIFICA SE O TOCOU NO MENU Kp
    else if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 322 && p.x < 480 && p.y > 0 && p.y < 50)
    {
      current_page = kp_menu;
      tft.fillScreen(BLACK);
      drawKpMenu();
    }
    // VERIFICA SE O TOQUE OCORREU DENTRO DOS LIMITES DO BOTÃO INICIAR/ENCERRAR
    else if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 200 && p.x < 300 && p.y > 40 && p.y < 140)
    {
      // Verifique se o botão estava pressionado anteriormente
      if (!program_button_state)
      {
        // ATUALIZA O ESTADO DO BOTAO
        program_button_state = true;
        // ALTERA O ESTADO DO PROGRAMA
        iniciarPrograma = !iniciarPrograma;

        // ALTERA O VISUAL DO BUTAO
        if (iniciarPrograma)
        {
          // BUTÃO ENCERRAR PROGRAMA (OFF)
          tft.drawRoundRect(325, 75, buttonWidth, buttonHeight, 5, RED);
          tft.fillRoundRect(374, 115, 55, 22, 5, BLACK);
          tft.setTextColor(RED); // Define a cor do texto para vermelho
          tft.setTextSize(3);    // Define o tamanho do texto para 3
          tft.setCursor(375, 115);
          tft.println("OFF"); // Escreve o texto no botão
        }
        else
        {
          // BUTÃO INCIAR PROGRAMA (ON)
          tft.drawRoundRect(325, 75, buttonWidth, buttonHeight, 5, DARKER_GREEN);
          tft.fillRoundRect(374, 115, 55, 22, 5, BLACK);
          tft.setTextColor(DARKER_GREEN); // Define a cor do texto para branco
          tft.setTextSize(3);             // Define o tamanho do texto para 3
          tft.setCursor(385, 115);
          tft.println("ON"); // Escreve o texto no botão
        }
      }
      delay(200);
    }
    else
    {
      // Verifique se o estado do botão mudou
      if (program_button_state)
      {
        // Atualize o estado do botão
        program_button_state = false;
      }
    }
  }
  else if (current_page == valves_menu)
  {
    // VERIFICA SE TOCOU NO MENU PRINCIPAL
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 0 && p.x < 160 && p.y > 0 && p.y < 50)
    {
      current_page = home_screen;
      tft.fillScreen(BLACK);
      drawHomeScreen(iniciarPrograma);
    }
    // VERIFICA SE O TOCOU NO MENU Kp
    else if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 322 && p.x < 480 && p.y > 0 && p.y < 50)
    {
      current_page = kp_menu;
      tft.fillScreen(BLACK);
      drawKpMenu();
    }
  }
  else if (current_page == kp_menu)
  {
    // VERIFICA SE TOCOU NO MENU PRINCIPAL
    if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 0 && p.x < 160 && p.y > 0 && p.y < 50)
    {
      current_page = home_screen;
      tft.fillScreen(BLACK);
      drawHomeScreen(iniciarPrograma);
    }
    // VERIFICA SE O TOCOU NO MENU VALVULAS
    else if (p.z > MINPRESSURE && p.z < MAXPRESSURE && p.x > 161 && p.x < 321 && p.y > 0 && p.y < 50)
    {
      current_page = valves_menu;
      tft.fillScreen(BLACK);
      drawValvesMenu();
    }
  }

  // DEBUG PARA CALIBRAR TOUCH DOS BOTOES
  // Serial.print("X: ");
  // Serial.print(p.x);
  // Serial.print("    ");
  // Serial.print("Y: ");
  // Serial.print(p.y);
  // Serial.print("    ");
  // Serial.print("Z: ");
  // Serial.print(p.z);
  // Serial.println();
}