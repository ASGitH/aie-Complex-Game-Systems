// GOAL: MAKE ALL GLOBAL VARIABLES A EXTERN (EXAMPLE: extern player_Position[2]) and put them all into a h file and in the library, put the h file in there.

#include <gb/gb.h>
#include <stdio.h>
#include <stdlib.h>

#include "character_With_A_Hat.c"

BOOLEAN can_The_Player_Move = TRUE;
BOOLEAN has_Players_Position_Been_Set = FALSE;

const char empty_Tile[1] = {0x00};

int index_Top_Left_X = 0;
int index_Top_Left_Y = 0;
int tile_Index_Top_Left = 0;

// Player Position[0] = X position in the current room, Player Position[1] = Y position in the current room;
int player_Position[2] = {0, 0};
int player_Starting_Position[2];

UBYTE direction = 0;

// Have can the player move function here later
void animate_Sprite(UINT8 _sprite_Index, INT8 move_X, INT8 move_Y){
    while(move_X!=0){
        scroll_sprite(_sprite_Index, move_X < 0 ? -1 : 1, 0);

        move_X += (move_X < 0 ? 1 : -1);

        wait_vbl_done();
    }
    while(move_Y!=0){
        scroll_sprite(_sprite_Index, 0, move_Y < 0 ? -1 : 1);

        move_Y += move_Y < 0 ? 1 : -1;

        wait_vbl_done();
    }
}

void enable_Movement(BOOLEAN _can_The_Player_Move){
    switch(_can_The_Player_Move){
        case FALSE: can_The_Player_Move = FALSE; break;
        case TRUE: can_The_Player_Move = TRUE; break;
    }
}

// Original player's position = 16, 24
// Make sure to Instantiate the Player AFTER you Set the Map
void instantiate_Player(const char *_data /*, int _x_Position, int _y_Position --- enable only if you would like to spawn at a set position ---*/){
    BOOLEAN has_Empty_Tile_Been_Found = FALSE;
    int index_Counter = 0, index_Counter_X = 0, index_Counter_Y = 0;

    set_sprite_data(0, 2, character_With_A_Hat);
    set_sprite_tile(0, 1);

    // When the map has been set, check that the current avaliable tile is not being occupied, if so, move on to the next one.
    if(!has_Players_Position_Been_Set){
        for(index_Counter; index_Counter < 360; index_Counter++){
            if(_data[index_Top_Left_X + (32 * index_Top_Left_Y)] != empty_Tile[0] && !has_Empty_Tile_Been_Found){
                // player_Position[0] += 8;
                index_Top_Left_X += 1;
                if(index_Top_Left_X > 20){ index_Top_Left_X = 0; index_Top_Left_Y += 1; }
            }
            else if(_data[index_Top_Left_X + (32 * index_Top_Left_Y)] == empty_Tile[0] && !has_Players_Position_Been_Set){
                // If an empty tile has been found, convert the itlx / itly into the player position
                for(index_Counter_X = 0; index_Counter_X < index_Top_Left_X; index_Counter_X++){ player_Position[0] += 8; }
                for(index_Counter_Y = 0; index_Counter_Y < index_Top_Left_Y; index_Counter_Y++){ player_Position[1] += 8; }
                // Because the GameBoy screen is at an offset, make sure to calculate for the offset by adding 8, to the player's x position,
                // and 16, to the player's y position.
                // Todo - find a way to include the offset without adding them at the end. 
                player_Position[0] += 8; player_Position[1] += 16;

                has_Empty_Tile_Been_Found = TRUE;
                has_Players_Position_Been_Set = TRUE;

                // index_Top_Left_X = (_x_Position - 8) / 8;
                // index_Top_Left_Y = (_y_Position - 16) / 8;
                // Todo - After you find the player's position, it may be nice to reset the itlx / itly
                index_Top_Left_X = (player_Position[0] - 8) / 8;
                index_Top_Left_Y = (player_Position[1] - 16) / 8;

                // player_Position[0] = _x_Position; player_Starting_Position[0] = _x_Position;
                // player_Position[1] = _y_Position; player_Starting_Position[1] = _y_Position;

                player_Starting_Position[0] = player_Position[0];
                player_Starting_Position[1] = player_Position[1];

                set_sprite_data(0, 2, character_With_A_Hat);
                set_sprite_tile(0, 1);

                move_sprite(0, player_Position[0], player_Position[1]);
            }
        }
    }
}

// Note: If two buttons are pressed at the same time (for example: down and left), the player will ignore all collision.
void move_Player(){
    if(can_The_Player_Move){
        direction = joypad();
        if(direction & J_DOWN && index_Top_Left_Y + 1 != 17){ index_Top_Left_Y += 1; player_Position[1] += 8; animate_Sprite(0, 0, 8); }
        else if(direction & J_LEFT && index_Top_Left_X - 1 != 0){ index_Top_Left_X -= 1; player_Position[0] -= 8; animate_Sprite(0, -8, 0); }
        else if(direction & J_RIGHT && index_Top_Left_X + 1 != 19){ index_Top_Left_X += 1; player_Position[0] += 8; animate_Sprite(0, 8, 0); }
        else if(direction & J_UP && index_Top_Left_Y - 1 != 0){ index_Top_Left_Y -= 1; player_Position[1] -= 8; animate_Sprite(0, 0, -8); }
    }
    delay(99);
}

void update_Player(int _itlx, int _itly){
    _itlx = (player_Position[0] - 8) / 8;
    _itly = (player_Position[1] - 16) / 8;
}