/*
 * LCD.h
 *
 * Copyright (C) 2017 by Harsh Singhal.
 * Modification or use of this software in source or binary form
 * is permitted as long as files maintain this copyright.
 * This is completely for educational purposes and not commercial use.
 *
 * Created: 15-10-2017 20:28:13
 * Author: Harsh Singhal
 */ 


#ifndef LCD_H_
#define LCD_H_

//MPU : ATmega 8

#define F_CPU 1000000ul		// 1 MHz frequency of micro controller

#include <avr/io.h>

/////////////////////////////////////////////////////////
//// MAKE CHANGES ACCORDING TO DIFFERENT MPU's HERE ////
////////////////////////////////////////////////////////


// BI-DIRECTIONAL DATA BUS ON LCD // 

//Data direction register for Data Bus (DB0 - DB7) of LCD 
#define DataDir			DDRD
#define DataBus			PORTD


// CONTROL LINES FOR LCD //

//Data direction register for control lines (RS, RW, E) of LCD 
#define	ControlDir		DDRC
#define Control			PORTC

// RS = 0 => command mode (blinking cursor , 8 or 4 bit mode operation , clear screen etc.) (Refer datasheet for commands)
// RS = 1 => data mode (send character or data to lcd)
#define RS			PINC0

//R/W = 0 => write mode (we will write or send data to lcd)
//R/W = 1 => read mode (we will read data from lcd (to check busy and other flags))
#define RW			PINC1

//Enable = 1 => preapare data
//Enable = 0 => sends data
#define enable			PINC2


//Basic Command Functions for lcd

/**************************************************************************************************************************************************
* Description : When data_length = 1 (8 Bit Bus mode)		When display_line = 1 (2 Line Mode)		When font_format = 1 (5x11 dots format)
*				When data_length = 0 (4 Bit Bus mode)		When display_line = 0 (1 Line Mode)		When font_format = 0 (5x8 dots format)
*
*				// 4 BIT MODE CURRENTLY NOT SUPPORTED
*
*				Additionally it has starting delay for LCD wake up(warm up) time and defines data direction register for data bus and control lines
***************************************************************************************************************************************************/
void LCD_init(unsigned char data_length, unsigned char display_line, unsigned char font_format);

/*********************************************************************************************
* Description : Write "20H"(Space code) to DDRA and set DDRAM address to "00H" from AC
**********************************************************************************************/
void Clear_display(void);

/************************************************************************************************************************
* Description : When display = 1 (Display ON)		When cursor = 1 (Cursor ON)		When blinking = 1 (Cursor Blink ON)
*				When display = 0 (Display OFF)		When cursor = 0 (Cursor OFF)	When blinking = 0 (Cursor BLink OFF)
*************************************************************************************************************************/
void Display_Control(unsigned char display, unsigned char cursor, unsigned char blinking);

/*********************************************************************************************
* Description :	Set DDRAM address to "00H" from AC and return cursor to its original position
*				if shifted. The contents of DDRAM are not changed.
**********************************************************************************************/
void Return_home(void);

/************************************************************************************************************
* Description :	Shifting of right/left cursor position or display without writing or reading display data
*				This instruction is used to correct or search display data.
*
*				S/C		R/L						Operation
*				0		0				Shift cursor to left, AC is decreased by 1
*				0		1				Shift cursor to right, AC is increased by 1
*				1		0				Shift all display to the left, cursor moves according to the display
*				1		1				Shift all display to the right, cursor moves according to the display
*************************************************************************************************************/
void Display_shift(unsigned char SC, unsigned char RL);

/*********************************************************************************************
* Description :	Set the moving direction of cursor and display. 
*				I/D:increment/decrement of DDRAM address (cursor or blink)
*				I/D = 1 (cursor/blink moves to right and DDRAM address is increased by 1)
*				I/D = 0 (cursor/blink moves to left and DDRAM address is increased by 1)
*
*				SH(shift): shift of entire display)
*				SH = 1 if DDRAM write operation, shift of entire display is performed acc. to I/D
*				SH = 0 shifting of display is not performed.
**********************************************************************************************/
void Entry_mode(unsigned char ID, unsigned char shift);

/*********************************************************************************************
* Description :	Set DDRAM address to AC.
*
*				line = 1 (selects line 1 of lcd)
*				line = 2 (selects line 2 of lcd)
*
*				add = can be between 1 to 40				
*
*				line 1 address varies from 00H to 27H (40 address locations)
*				line 2 address varies from 40H to 67H (40 address locations)
**********************************************************************************************/
void Set_address(unsigned char line, unsigned char add);

//Working Command Function For LCD

/*********************************************************************************************
* Description : Sends a pulse ( makes enable high from low for 600us and then low from high).
*				This is used to transfer data or instruction from MPU to LCD.
*********************************************************************************************/
void Flash_enable(void);

/*******************************************************************************************************************
* Description : Since LCD is slow device each time before sending new instruction or data busy flag must be checked.
*				DB7 is busy flag. If DB7 = 1 LCD is busy & if DB7 = 0 LCD is not busy
*******************************************************************************************************************/
void Check_busy(void);

/*********************************************************************************************
* Description : Used to send various commands to LCD such as working mode , diaplay on/off
*				cursor position and blinking and many more.
**********************************************************************************************/
void Send_command(unsigned char command);

/*********************************************************************************************
* Description : Used to send ASCII code of characters to LCD.
**********************************************************************************************/
void Send_character(unsigned char character);

/*********************************************************************************************
* Description : Used to send whole string of data instead of one character at a time.
**********************************************************************************************/
void Send_string(char *s);


#endif /* LCD_H_ */