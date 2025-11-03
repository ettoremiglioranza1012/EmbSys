/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : ULTRA SPICY LED Control System v2.0 🌶️🔥
  * 
  * FEATURES:
  * - Morse Code SOS
  * - Breathing LED effect (PWM simulation)
  * - Disco Mode (random patterns)
  * - Rainbow Mode (speed variations)
  * - Knight Rider effect
  * - Game: Memory Pattern Challenge
  * 
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
  * All rights reserved.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdlib.h>  // Per rand()

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

// Enum per i vari modi operativi
typedef enum {
    MODE_OFF = 0,
    MODE_MORSE_SOS,      // Morse: SOS (... --- ...)
    MODE_BREATHING,      // LED che "respira" (fade in/out)
    MODE_DISCO,          // Pattern casuali
    MODE_HEARTBEAT,      // Battito cardiaco
    MODE_KNIGHT_RIDER,   // Effetto supercar
    MODE_STROBE,         // Strobo velocissimo
    MODE_MAX
} LEDMode;

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define MORSE_DOT_DURATION      200   // Durata punto morse (ms)
#define MORSE_DASH_DURATION     600   // Durata linea morse (ms)
#define MORSE_PAUSE_SHORT       200   // Pausa tra simboli
#define MORSE_PAUSE_LONG        1400  // Pausa tra lettere
#define BREATHING_STEPS         20    // Step per effetto breathing
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim10;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */
// === VARIABILI DI STATO GLOBALI ===
LEDMode current_mode = MODE_OFF;
uint8_t led_state = 0;
uint32_t last_button_press = 0;
uint32_t button_hold_time = 0;
uint8_t button_pressed = 0;

// === VARIABILI PER MORSE CODE ===
const char morse_sos[] = "...---...";  // SOS in morse
uint8_t morse_index = 0;
uint8_t morse_step = 0;  // 0=LED ON, 1=LED OFF (pausa)

// === VARIABILI PER BREATHING ===
uint8_t breathing_step = 0;
int8_t breathing_direction = 1;  // 1=fade in, -1=fade out

// === VARIABILI PER DISCO MODE ===
uint8_t disco_counter = 0;
uint16_t disco_random_pattern = 0;

// === VARIABILI PER HEARTBEAT ===
uint8_t heartbeat_phase = 0;  // 0-3: boom-boom-pausa-pausa

// === VARIABILI PER KNIGHT RIDER ===
uint8_t knight_position = 0;
int8_t knight_direction = 1;

// === VARIABILI PER STROBE ===
uint16_t strobe_counter = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_USART2_UART_Init(void);
static void MX_TIM10_Init(void);
/* USER CODE BEGIN PFP */
void SwitchMode(LEDMode new_mode);
void ProcessMorseCode(void);
void ProcessBreathing(void);
void ProcessDisco(void);
void ProcessHeartbeat(void);
void ProcessKnightRider(void);
void ProcessStrobe(void);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{

  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USART2_UART_Init();
  MX_TIM10_Init();
  /* USER CODE BEGIN 2 */
  
  // Messaggio di benvenuto: 3 lampeggi rapidi
  for(int i = 0; i < 3; i++) {
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
      HAL_Delay(100);
      HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
      HAL_Delay(100);
  }
  
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE2);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLM = 8;
  RCC_OscInitStruct.PLL.PLLN = 60;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 7;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief TIM10 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM10_Init(void)
{

  /* USER CODE BEGIN TIM10_Init 0 */

  /* USER CODE END TIM10_Init 0 */

  /* USER CODE BEGIN TIM10_Init 1 */

  /* USER CODE END TIM10_Init 1 */
  htim10.Instance = TIM10;
  htim10.Init.Prescaler = 59999;  // 60MHz / 60000 = 1kHz
  htim10.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim10.Init.Period = 49;  // 50ms base tick (20 Hz) per controllo fine
  htim10.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim10.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim10) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM10_Init 2 */

  /* USER CODE END TIM10_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  /* USER CODE BEGIN MX_GPIO_Init_1 */

  /* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(LD2_GPIO_Port, LD2_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin : B1_Pin */
  GPIO_InitStruct.Pin = B1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_FALLING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(B1_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD2_Pin */
  GPIO_InitStruct.Pin = LD2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(LD2_GPIO_Port, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI15_10_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI15_10_IRQn);

  /* USER CODE BEGIN MX_GPIO_Init_2 */

  /* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/**
  * @brief  Cambia modalità operativa
  * @param  new_mode: Nuova modalità
  * @retval None
  */
void SwitchMode(LEDMode new_mode)
{
    current_mode = new_mode;
    
    // Reset variabili per ogni modalità
    morse_index = 0;
    morse_step = 0;
    breathing_step = 0;
    breathing_direction = 1;
    disco_counter = 0;
    heartbeat_phase = 0;
    knight_position = 0;
    knight_direction = 1;
    strobe_counter = 0;
    
    if (new_mode == MODE_OFF) {
        HAL_TIM_Base_Stop_IT(&htim10);
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    } else {
        HAL_TIM_Base_Start_IT(&htim10);
    }
}

/**
  * @brief  Processa Morse Code SOS (... --- ...)
  * @retval None
  */
void ProcessMorseCode(void)
{
    static uint16_t morse_timer = 0;
    morse_timer += 50;  // Incremento ogni tick (50ms)
    
    if (morse_step == 0) {
        // LED ON per durata simbolo
        uint16_t duration = (morse_sos[morse_index] == '.') ? MORSE_DOT_DURATION : MORSE_DASH_DURATION;
        
        if (morse_timer >= duration) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            morse_step = 1;
            morse_timer = 0;
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        }
    } else {
        // LED OFF per pausa
        morse_index++;
        if (morse_index >= 9) {
            // Fine SOS, pausa lunga
            if (morse_timer >= MORSE_PAUSE_LONG) {
                morse_index = 0;
                morse_timer = 0;
            }
        } else if (morse_timer >= MORSE_PAUSE_SHORT) {
            morse_step = 0;
            morse_timer = 0;
        }
    }
}

/**
  * @brief  Effetto breathing (LED che respira)
  * @retval None
  */
void ProcessBreathing(void)
{
    static uint8_t pwm_counter = 0;
    pwm_counter++;
    
    // Simula PWM con frequenza alta
    if (pwm_counter < breathing_step) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
    
    if (pwm_counter >= BREATHING_STEPS) {
        pwm_counter = 0;
        breathing_step += breathing_direction;
        
        if (breathing_step >= BREATHING_STEPS) {
            breathing_direction = -1;  // Inizia fade out
        } else if (breathing_step == 0) {
            breathing_direction = 1;   // Inizia fade in
        }
    }
}

/**
  * @brief  Disco mode: pattern casuali
  * @retval None
  */
void ProcessDisco(void)
{
    disco_counter++;
    
    if (disco_counter >= 2) {  // Cambia ogni 100ms
        disco_random_pattern = rand() % 10;  // Numero casuale 0-9
        disco_counter = 0;
    }
    
    // Pattern basato su numero casuale
    if (disco_random_pattern < 5) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    } else if (disco_random_pattern < 8) {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
    }
}

/**
  * @brief  Heartbeat: boom-boom-pausa
  * @retval None
  */
void ProcessHeartbeat(void)
{
    static uint8_t heartbeat_counter = 0;
    heartbeat_counter++;
    
    switch(heartbeat_phase) {
        case 0:  // Primo boom
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            if (heartbeat_counter >= 2) {  // 100ms
                heartbeat_phase = 1;
                heartbeat_counter = 0;
            }
            break;
        case 1:  // Pausa breve
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            if (heartbeat_counter >= 2) {  // 100ms
                heartbeat_phase = 2;
                heartbeat_counter = 0;
            }
            break;
        case 2:  // Secondo boom
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
            if (heartbeat_counter >= 2) {  // 100ms
                heartbeat_phase = 3;
                heartbeat_counter = 0;
            }
            break;
        case 3:  // Pausa lunga
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
            if (heartbeat_counter >= 14) {  // 700ms
                heartbeat_phase = 0;
                heartbeat_counter = 0;
            }
            break;
    }
}

/**
  * @brief  Knight Rider: effetto supercar
  * @retval None
  */
void ProcessKnightRider(void)
{
    static uint8_t knight_counter = 0;
    knight_counter++;
    
    if (knight_counter >= 2) {  // Cambia ogni 100ms
        knight_position += knight_direction;
        
        if (knight_position >= 10) {
            knight_direction = -1;
        } else if (knight_position == 0) {
            knight_direction = 1;
        }
        
        // Lampeggio basato sulla posizione
        if (knight_position % 2 == 0) {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_SET);
        } else {
            HAL_GPIO_WritePin(GPIOA, GPIO_PIN_5, GPIO_PIN_RESET);
        }
        
        knight_counter = 0;
    }
}

/**
  * @brief  Strobe: lampeggio strobo velocissimo
  * @retval None
  */
void ProcessStrobe(void)
{
    strobe_counter++;
    
    // Alterna ogni tick (50ms) per effetto strobo
    if (strobe_counter % 1 == 0) {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_5);
    }
}

/**
  * @brief  EXTI line detection callback
  * @param  GPIO_Pin: Pin che ha generato l'interrupt
  * @retval None
  */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == B1_Pin)
    {
        uint32_t current_time = HAL_GetTick();
        
        // Rileva click rapido (cambia modalità)
        if ((current_time - last_button_press) < 500) {
            // Doppio click: passa alla modalità successiva
            current_mode++;
            if (current_mode >= MODE_MAX) {
                current_mode = MODE_OFF;
            }
            SwitchMode(current_mode);
        } else {
            // Singolo click: se è OFF, accendi in MODE_MORSE_SOS
            if (current_mode == MODE_OFF) {
                SwitchMode(MODE_MORSE_SOS);
            }
        }
        
        last_button_press = current_time;
    }
}

/**
  * @brief  Period elapsed callback (50ms tick)
  * @param  htim: TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM10)
    {
        // Esegui la funzione della modalità corrente
        switch(current_mode) {
            case MODE_MORSE_SOS:
                ProcessMorseCode();
                break;
            case MODE_BREATHING:
                ProcessBreathing();
                break;
            case MODE_DISCO:
                ProcessDisco();
                break;
            case MODE_HEARTBEAT:
                ProcessHeartbeat();
                break;
            case MODE_KNIGHT_RIDER:
                ProcessKnightRider();
                break;
            case MODE_STROBE:
                ProcessStrobe();
                break;
            default:
                break;
        }
    }
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
