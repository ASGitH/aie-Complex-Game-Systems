#include <gb/gb.h>
#include <stdio.h>
#include <stdlib.h>

#include "character_With_A_Hat.c"

BOOLEAN can_The_Player_Move = TRUE;

int index_Top_Left_X = 0;
int index_Top_Left_Y = 0;
int tile_Index_Top_Left = 0;

// Player Position[0] = X position in the current room, Player Position[1] = Y position in the current room;
int player_Position[2];
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

void instantiate_Player(int _x_Position, int _y_Position){
    player_Position[0] = _x_Position; player_Starting_Position[0] = _x_Position;
    player_Position[1] = _y_Position; player_Starting_Position[1] = _y_Position;

    set_sprite_data(0, 2, character_With_A_Hat);
    set_sprite_tile(0, 1);

    move_sprite(0, _x_Position, _y_Position);
}

void move_Player(){
    if(can_The_Player_Move){
        direction = joypad();

        if(direction & J_DOWN){ index_Top_Left_Y += 1; player_Position[1] += 8; animate_Sprite(0, 0, 8); }
        else if(direction & J_LEFT){ index_Top_Left_X -= 1; player_Position[0] -= 8; animate_Sprite(0, -8, 0); }
        else if(direction & J_RIGHT){ index_Top_Left_X += 1; player_Position[0] += 8; animate_Sprite(0, 8, 0); }
        else if(direction & J_UP){ index_Top_Left_Y -= 1; player_Position[1] -= 8; animate_Sprite(0, 0, -8); }
    }
}

void update_Player(int _itlx, int _itly){
    _itlx = (player_Position[0] - 8) / 8;
    _itly = (player_Position[1] - 16) / 8;
}