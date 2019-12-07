#include <gb/gb.h>
#include <stdio.h>

#include "map_Configuration.c"

void main(){
    // 0, 0, 0, 1, 0 = set_Map(1, 5);
    set_bkg_data(0, 2, dungeon_Map_Sprites);

    instantiate_Player(16, 24);
    set_Map(2, 2);

    SHOW_BKG;
    SHOW_SPRITES; 

    while(1){
        // switch(joypad()){
        //     case J_DOWN:
        //     current_Position_On_Map = convert_Map_Position(position_On_Map);
        //     change_Room(0);
        //     break;
        //     case J_LEFT:
        //     current_Position_On_Map = convert_Map_Position(position_On_Map);
        //     change_Room(1);
        //     break;
        //     case J_RIGHT:
        //     current_Position_On_Map = convert_Map_Position(position_On_Map);
        //     change_Room(2);
        //     break;
        //     case J_UP:
        //     current_Position_On_Map = convert_Map_Position(position_On_Map);
        //     change_Room(3);
        //     break;
        // }

        switch(joypad()){
            case J_DOWN:
                move_Player(0);
                move_Background(map_Array[map_Index_Array[current_Position_On_Map]], 0, 5, player_Position);
            break;
            case J_LEFT:
                move_Player(1);
                move_Background(map_Array[map_Index_Array[current_Position_On_Map]], 1, 5, player_Position);
            break;
            case J_RIGHT:
                move_Player(2);
                move_Background(map_Array[map_Index_Array[current_Position_On_Map]], 2, 5, player_Position);            
            break;
            case J_UP:
                move_Player(4);
                move_Background(map_Array[map_Index_Array[current_Position_On_Map]], 4, 5, player_Position);            
            break;
        }
        delay(99);
    }
}