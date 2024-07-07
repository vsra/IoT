#include <stdio.h>
#include <esp_rom_sys.h>
#include "CustomDelay.h"

//Creacion de las funciones de delay tanto en milisegundos como en segundos que toma por parametro un valor especificado en la variable tiempo.
//Se utiliza la funcion delay 'esp_rom_delay_us' que el ESP-32 ya contiene, para la creacion de los nuevos delays. 

void delay_miliseg(uint32_t tiempo){
    esp_rom_delay_us(tiempo*1000);
    return;
}

void delay_seg(uint32_t tiempo){
    delay_miliseg(tiempo*1000);
    return;
}
