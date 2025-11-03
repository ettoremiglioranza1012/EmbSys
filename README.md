# Embedded Systems

This repository contains documentation and development files related to the Embedded Systems course and personal projects development.
It includes:
- Reports from laboratory sessions
- Development projects using STM32CubeIDE
- Personal work and progression in embedded programming

---

## Repository Contents

### 1. EmbSys1.ipynb - Power Measurement Lab Report
Jupyter notebook documenting the first laboratory session, focusing on power consumption evaluation of a DWM1001 UWB board. Includes analysis of power characterization during different operational states (transmit, receive, sleep) and data visualization using Python.

### 2. EmbSys4.c - Introduction to STM32CubeIDE
First hands-on experience with STM32CubeIDE and basic embedded C programming. Implements simple LED control using GPIO pins and delay loops. During development, a common mistake with loop counter (`k--` instead of `--k`) was identified and corrected, serving as a learning exercise in C operators.

### 3. EmbSys5_1.0.c - Timer-Based LED Control with Variable Speed
Enhanced LED control using hardware timer (TIM10) with interrupt-based operation. Features START/STOP functionality via button press and four different blinking speeds (2s, 1s, 0.5s, 0.2s). Single click starts the timer, double click cycles through speed modes. Demonstrates dynamic period adjustment and double-click detection.

### 4. EmbSys5_2.0.c - Multi-Mode LED Effects
Advanced implementation featuring six different visual effects controlled by a state machine:
- MORSE_SOS: Transmits SOS in Morse code
- BREATHING: Fade in/out effect using PWM simulation
- DISCO: Random blinking patterns
- HEARTBEAT: Double-beat pulse pattern
- KNIGHT_RIDER: Back-and-forth scanning effect
- STROBE: Fast strobe light

Single click activates the system, double click cycles through modes. Uses 50ms timer tick for precise control of complex patterns.

---

## Development Environment
- **IDE**: STM32CubeIDE
- **Target MCU**: STM32 Nucleo board
- **Programming Language**: C (embedded)
- **HAL Library**: STM32 Hardware Abstraction Layer
- **Analysis Tools**: Jupyter Notebook, Python (pandas, plotly)
