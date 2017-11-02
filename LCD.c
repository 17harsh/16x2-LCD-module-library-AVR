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

#include "LCD.h"
#include <util/delay.h>

//INITIALIZE LCD

void LCD_init(void){
	_delay_ms(15);													//LCD warm up (wake up) time
	DataDir = 0xff;													//configuring data bus to output		 
	ControlDir |= (1<<enable)|(1<<RS)|(1<<RW);						//configuring control lines to output
	unsigned char temp;
	temp = ((0x20) | (Mode<<4) | (Line<<3) | (Format<<2));			//command as specified in datasheet
	Send_command(temp);																						
	_delay_us(50);													//delay as specified in datasheet
	temp = ((0x08) | (Display<<2) | (Cursor<<1) | (CursorBlink));
	Send_command(temp);												//command as specified in datasheet
	_delay_us(5);													//delay as specified in datasheet 
}

//LCD CONTROL COMMANDS

void Clear_display(){
	Send_command(0x01);												//command as specified in datasheet
	_delay_ms(5);													//delay as specified in datasheet
}

void Return_home(){
	Send_command(0x02);												//command as specified in datasheet
	_delay_ms(5);													//delay as specified in datasheet
}

void Display_Control(unsigned char display, unsigned char cursor, unsigned char blinking){
	unsigned char temp;
	temp = ((0x08) | (display<<2) | (cursor<<1) | (blinking));
	Send_command(temp);												//command as specified in datasheet
	_delay_us(5);													//delay as specified in datasheet
}

void Entry_mode(unsigned char ID, unsigned char shift){
	unsigned char temp;
	temp = ((0x08) | (ID<<1) | (shift));
	Send_command(temp);												//command as specified in datasheet
	_delay_us(50);													//delay as specified in datasheet
}

void Display_shift(unsigned char SC, unsigned char RL){
	unsigned char temp;
	temp = ((0x10) | (SC<<3) | (RL<<2));
	Send_command(temp);												//command as specified in datasheet
	_delay_us(50);													//delay as specified in datasheet
}

void Set_address(unsigned char line, unsigned char add){
	if (line == 1){
		Send_command(0x00+(add-1));
		_delay_us(50);
	}
	if (line == 2){
		Send_command(0x40+(add-1));
		_delay_us(50);
	}
}

//LCD NECCESSARY WORKING FUNCTIONS

void Flash_enable(){
	Control |= (1<<enable);
	/*
	asm volatile "nop";
	asm volatile "nop";												// either use this (assembly command of 1 machine cycle each) instead of delay_us
	asm volatile "nop";
	*/
	_delay_us(600);													//delay as specified in datasheet
	Control &= ~(1<<enable);
}

void Check_busy(){
	DataDir = 0x00;
	Control |= (1<<RW);
	Control &= (1<<RS);
	while(DataBus >= 0x80){
		Flash_enable();
	}
	DataBus = 0x00;
}

void Send_command(unsigned char command){
	Check_busy();
	DataDir = 0xff;
	Control &= ~((1<<RS)|(1<<RW));
	DataBus = command;
	Flash_enable();
	DataDir = 0x00;
	DataBus = 0x00;
}

void Send_character(unsigned char character){
	Check_busy();
	DataDir = 0xff;
	Control |= (1<<RS);
	Control &= ~(1<<RW);
	DataBus = character;
	Flash_enable();
	_delay_us(50);
	DataDir = 0x00;
	DataBus = 0x00;
}

void Send_string(char *s){
	while(*s >0){
		Send_character(*s);
		s++;
	}
}

