#include <gb/gb.h>
#include <stdio.h>

#include "map_Configuration.c"

void main(){
    instantiate_Player(16, 24);
    set_Map(2, 2);

    while(TRUE){
        move_Background(3);
        move_Player();
        delay(99);
    }
}