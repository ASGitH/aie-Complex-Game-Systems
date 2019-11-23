#include <gb/gb.h>
#include <stdio.h>
#include <stdlib.h>

#include "character_With_A_Hat.c"

int index_Top_Left_X = 0;
int index_Top_Left_Y = 0;
int tile_Index_Top_Left = 0;

// Player Position[0] = X position in the current room, Player Position[1] = Y position in the current room;
int player_Position[2];
int player_Starting_Position[2];

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

void move_Player(int _direction){
    switch(_direction){
        // Down = 0
        case 0: index_Top_Left_Y += 1; player_Position[1] += 8; animate_Sprite(0, 0, 8); break;
        // Left = 1
        case 1: index_Top_Left_X -= 1; player_Position[0] -= 8; animate_Sprite(0, -8, 0); break;
        // Right = 2
        case 2: index_Top_Left_X += 1; player_Position[0] += 8; animate_Sprite(0, 8, 0); break;
        // Up = 4? (3 seems to not function for some reason)
        case 4: index_Top_Left_Y -= 1; player_Position[1] -= 8; animate_Sprite(0, 0, -8); break;
    }
}

void instantiate_Player(int _x_Position, int _y_Position){
    player_Position[0] = _x_Position; player_Starting_Position[0] = _x_Position;
    player_Position[1] = _y_Position; player_Starting_Position[1] = _y_Position;

    set_sprite_data(0, 2, character_With_A_Hat);
    set_sprite_tile(0, 1);

    move_sprite(0, _x_Position, _y_Position);
}