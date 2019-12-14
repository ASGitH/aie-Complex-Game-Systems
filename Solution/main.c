#include <gb/gb.h>
#include <stdio.h>

#include "map_Configuration.c"

void main(){
    // Make sure to Instantiate the Player AFTER you Set the Map
    set_Map(1, 3);
    // Original player's position = 16, 24
    instantiate_Player(map_Array[map_Index_Array[0]]);
    update_Player(index_Top_Left_X, index_Top_Left_Y);

    while(TRUE){
        move_Background(3, map_Array[map_Index_Array[current_Position_On_Map]]);
        move_Player();
        delay(99);
    }
}