#include <gb/gb.h>
#include <stdio.h>

#include "map_Configuration.c"

void main(){
    set_Map(2, 3);

    while(TRUE){
        move_Background(3, index_Top_Left_X, index_Top_Left_Y, player_Position, map_Array[map_Index_Array[current_Position_On_Map]]);
        move_Player();
        swap_Room();
    }
}