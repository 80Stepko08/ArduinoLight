// ============= НАСТРОЙКИ =============
#define BRIGHTNESS 10         // стандартная маскимальная яркость (0-255)
#define CURRENT_LIMIT 1000    // лимит по току в миллиамперах, автоматически управляет яркостью (пожалей свой блок питания!) 0 - выключить лимит
#define COLOR_ORDER GRB       // порядок цветов на ленте. Если цвет отображается некорректно - меняйте. Начать можно с RGB
#define NUM_LEDS 14           //количество светодиодов на ленте
#define NUM_STRIPS 1          // количество отрезков ленты (в параллели)
#define numHold_Time (1*1000) // время отображения индикатора уровня яркости/скорости/масштаба

// ============= ДЛЯ РАЗРАБОТЧИКОВ =============
#define LED_PIN 6             // пин ленты
#define BTN_PIN 2
#define MODE_AMOUNT 8       //кол-во эффектов
#define SEGMENTS 1            // диодов в одном "пикселе" (для создания матрицы из кусков ленты)
long FIRE_PALETTE = 0;      // разные типы огня (0 - 3). Попробуй их все! =)
// ------------------- ТИПЫ --------------------
CRGB leds[NUM_LEDS];
CRGBPalette16 gPal;
GButton touch(BTN_PIN, HIGH_PULL, NORM_OPEN); //если сенсорна кнопка LOW_PULL
#define RANDOM_DEMO 1                         // 0,1 - включить рандомный выбор режима
#define  DEMOTIMELIMIT ( 1 * 20UL * 1000UL)   //  минуты/секунды/милисекунды время задержка между эффектами
uint32_t DemTimer = 0UL;                      // тут будет храниться время следующего переключения эффекта
bool isDemo = false;                          // вкл/выкл демо при включении
//-----------Переменные-----------------
byte hue;
int16_t position;
boolean direction;
bool gReverseDirection = false;
void dimAll(uint8_t value) { 
    fadeToBlackBy (leds, NUM_LEDS, 255U - value);  //fadeToBlackBy
}
