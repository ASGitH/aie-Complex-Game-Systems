#include <gb/gb.h>
#include <stdio.h>

#include "map_Configuration.c"

void main(){
    //move_Background();
    set_Map(2, 2);

    while(1){
        wait_vbl_done();

        switch(joypad()){
            case J_LEFT:
                if(scroll_X != 0){ scroll_bkg( -8, 0 ); tile_Counter--; }
            break;
            case J_RIGHT:
                if(scroll_X < 20 - 20){
                    scroll_bkg(8, 0);
                    tile_Counter++;
                }            
            break;
            // case J_A:
            //     set_bkg_tiles(1, 1, 1, 1, &(blank_Dungeon_Room));
            // break;
        }

        // Tile_Counter == 8 (8 is how many times it would take before the screen would update and refresh)
        if(tile_Counter == /*8*/ 1){
            scroll_X++;
            tile_Counter = 0;

            counter = scroll_X + 21;
            temporary_B = counter % 32;

            for(temporary_A = 0; temporary_A != 18; temporary_A++){
                set_bkg_tiles(temporary_B, temporary_A, 1, 1, &(blank_Dungeon_Room));
                counter = counter + 20;
            }
        }

        delay(99);
    }
}