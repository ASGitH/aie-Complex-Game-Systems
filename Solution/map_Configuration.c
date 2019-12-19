// Ctrl + Shift + B = Build

// MAX_HEIGHT * MAX_WIDTH = 25 (Limit of rooms)
#define MAX_HEIGHT 5
#define MAX_WIDTH 5

#include <gb/gb.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>

#include "map_Library.h"

BOOLEAN can_The_Screen_Transition = FALSE;
BOOLEAN has_Count_Been_Reset = FALSE;
BOOLEAN has_Player_Been_Updated = FALSE;

char *map_Array[] = { blank_Dungeon_Room, dungeon_Room_One, dungeon_Room_Two };

const char blankmap[1] = {0x00};

int current_Position_In_Room = 0;
int current_Position_On_Map = 0;
// index_Top_Left_X = itlx
int itlx = 0;
// index_Top_Left_Y = itly
int itly = 0;
int returned_Map_Height = 0;
int returned_Map_Width = 0;
// Scroll X / Y - Keep track of how many times the player has scrolled the screen on the x / y axis
int scroll_X = 0, scroll_Y = 0;
int size_Of_Map = 0;
// tile_Index_Top_Left = titl
// int titl = 0;

int map_Index_Array[MAX_HEIGHT * MAX_WIDTH];
// Position On Map[0] = X position on map, Position On Map[1] = Y position on map;
int position_On_Map[2] = {1, 1};
// room_Height / room_Width array has a provided height and width, each array will hardcode the height and width for each room
// rHAWA[0] = blank_Dungeon_Room, rHAWA[1] = test_Map
int room_Height_Array[3] = { 18, 32, 32 };
int room_Width_Array[3] = { 20, 32, 32 };

// Takes in the current position on the map (Position On Map[2]), and converts the value into the index corresponding to the Map Index Array
int convert_Map_Position(int _map_Position[]){
    int x_Position_Counter = 0;
    int y_Position_Counter = 0;
    int map_Index_Counter = 0;

    for(y_Position_Counter = 0; y_Position_Counter < _map_Position[1] - 1; y_Position_Counter++){ map_Index_Counter += returned_Map_Width; }
    for(x_Position_Counter = 0; x_Position_Counter <= _map_Position[0] - 1; x_Position_Counter++){ map_Index_Counter += 1; }

    // printf("position_On_Map[%d][%d] = map_Index_Array[%d] = %d\n", position_On_Map[0], position_On_Map[1], map_Index_Counter, map_Index_Array[map_Index_Counter - 1]);

    return(map_Index_Counter);
}

// Takes in the player's current position, and converts the value into the index corresponding to current tile relative to it's position in the current room
void convert_Room_Position(int _player_Position[], int _width_Of_Room){
    itlx = (_player_Position[0] - 8) / 8;
    itly = (_player_Position[1] - 16) / 8;

    current_Position_In_Room = _width_Of_Room * itly + itlx;

    // printf("position_In_Room = [%d][%d] = current_Position_In_Room(%d)\n", itlx, itly, current_Position_In_Room);
}

// Move the player, then move the background
void move_Background(int _activation_Distance, unsigned char *_data){
    // Will check how far the player is between it's current position, to the edge
    int a_D_Counter = 0;

    convert_Room_Position(player_Position, room_Width_Array[map_Index_Array[current_Position_On_Map]]);

    wait_vbl_done();

    if(joypad() & J_A){
        for(a_D_Counter = 0; a_D_Counter < _activation_Distance; a_D_Counter++){
            // If the next two tiles ahead of the player are occupied
            if(_data[current_Position_In_Room + a_D_Counter] != blankmap[0]){ printf("There are tiles ahead of the player.\n"); } 
            else{ printf("There are no tiles ahead of the player.\n\n"); }
            delay(500);
        }
        // printf("\nCurrent Position In Room: %d\n\n", current_Position_In_Room);
        // printf("\nindex_Top_Left_X: %d\n\n", index_Top_Left_X);
        printf("Current Position In The Room  = %d\n", current_Position_In_Room);
    }

    switch(joypad()){
        case J_DOWN:
            // If the player is within activation distance, check if any of the tiles ahead of the player, are not being currently occupied
            if(index_Top_Left_Y > 17 - _activation_Distance && index_Top_Left_Y < 18){
                for(a_D_Counter = 0; a_D_Counter < _activation_Distance; a_D_Counter++){ 
                if(_data[current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)] != blankmap[0]){ can_The_Screen_Transition = FALSE; delay(1); }
                else if(_data[current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)] == blankmap[0] && can_The_Screen_Transition == FALSE){ can_The_Screen_Transition = TRUE; }
                }
                // If the tiles ahead of the player are not occupied at the moment
                // TODO WORK ON THIUS PART
                if(!can_The_Screen_Transition){ if(_data[current_Position_In_Room + room_Width_Array[map_Index_Array[current_Position_On_Map]]] == blankmap[0]){ enable_Movement(TRUE); } }
                else if(can_The_Screen_Transition){ enable_Movement(FALSE); player_Position[1] += 8; scroll_bkg(0, 8); }
            }
            
            if(_data[current_Position_In_Room - 1] != blankmap[0]){ enable_Movement(FALSE); }
            else{ enable_Movement(TRUE); }
            
            can_The_Screen_Transition = FALSE;
        break;
        case J_LEFT:
            // If the player is within activation distance, check if any of the tiles ahead of the player, are not being currently occupied
            if(index_Top_Left_X < _activation_Distance){
                for(a_D_Counter = 0; a_D_Counter < _activation_Distance; a_D_Counter++){ 
                if(_data[current_Position_In_Room - a_D_Counter] != blankmap[0]){ can_The_Screen_Transition = FALSE; delay(1); }
                else if(_data[current_Position_In_Room - a_D_Counter] == blankmap[0] && can_The_Screen_Transition == FALSE){ can_The_Screen_Transition = TRUE; }
                }
                // If the tiles ahead of the player are not occupied at the moment
                if(!can_The_Screen_Transition){ if(_data[current_Position_In_Room - 1] == blankmap[0]){ enable_Movement(TRUE); } }
                else if(can_The_Screen_Transition){ enable_Movement(FALSE); player_Position[0] -= 8; scroll_bkg(-8, 0); }
            }
            
            if(_data[current_Position_In_Room - 1] != blankmap[0]){ enable_Movement(FALSE); }
            else{ enable_Movement(TRUE); }
            
            can_The_Screen_Transition = FALSE;
        break;
        case J_RIGHT:
            // Check if the player is currently at the right edge of the screen first
            if(index_Top_Left_X > 19 - _activation_Distance && index_Top_Left_X < 20){
                // If the player is within activation distance, check if any of the tiles ahead of the player, are not being currently occupied
                for(a_D_Counter = 0; a_D_Counter < _activation_Distance; a_D_Counter++){ 
                    if(_data[current_Position_In_Room + a_D_Counter] != blankmap[0]){ can_The_Screen_Transition = FALSE; delay(1); }
                    else if(_data[current_Position_In_Room + a_D_Counter] == blankmap[0] && can_The_Screen_Transition == FALSE){ can_The_Screen_Transition = TRUE; }
                }
                // If the tiles ahead of the player are not occupied at the moment
                if(!can_The_Screen_Transition){ if(_data[current_Position_In_Room + 1] == blankmap[0]){ enable_Movement(TRUE); } }
                else if(can_The_Screen_Transition){ enable_Movement(FALSE); player_Position[0] += 8; scroll_bkg(8, 0); }
            }
            
            if(_data[current_Position_In_Room + 1] != blankmap[0]){ enable_Movement(FALSE); }
            else{ enable_Movement(TRUE); }
            
            can_The_Screen_Transition = FALSE;
        break;
    }

    // // **********************--------------------------------------------**********************
    // BOOLEAN refresh_Map = FALSE;

    // int size_Of_Current_Room = 0;
    // // Tile Index Top Left = titl (Note: titl's range would only be between 0 - 359)
    // int titl = 0;

    // // Window Position[0] = X Position in current window, Window Position[1] = Y Position in current window
    // int window_Position[2] = { 0, 0 };

    // UINT8 counter_X = 0;
    // UINT8 counter_Y = 0;

    // // One tall and twenty wide
    // unsigned char one_By_Twenty_Tile_Section[20];

    // window_Position[0] = index_Top_Left_X; window_Position[1] = index_Top_Left_Y;

    // // Get the player's current position by converting their position[2] into an integer(tile index)
    // convert_Room_Position(_player_Position, room_Width_Array[map_Index_Array[current_Position_On_Map]]);
    // titl = current_Position_In_Room;

    // // Check if the player's tile index (if we add the width of the current room) is (not?) null in the current room's index
    // switch(_direction){
    //     // TODO: If spawn in a corner and can shift avaliable, since player is too close to edge, will not shift / update.
    //     if(window_Position[0] < 1){ window_Position[0] = 1; } else if(window_Position[0] > 18) { window_Position[0] = 18; }
    //     if(window_Position[1] < 1){ window_Position[1] = 1; } else if(window_Position[1] > 16) { window_Position[1] = 16; }

    //     // Down = 0
    //     case 0:
    //         // Check if the x-axis can be shifted to the bottom
    //         if(_data[titl + room_Width_Array[map_Index_Array[current_Position_On_Map]]] != NULL){ 
    //             // Ways to move the BACKGROUND!!!
    //                 // 1. Call scroll_bkg
    //                     // - Easiest method, but if using a large map, would cut it off (can't find a fix (I have no idea why))
    //                 // 2. Creating a new char array and just having the updated map as what you would like to display
    //                     // - Seems doable but, for some reason, I suppose when you create a map in the GameBoy Map Designer, it adds some special thing when creating a map so when you try to make a duplicate of it, does not work.
    //                 // 3. 
    //             //

    //             set_bkg_tiles(0, 1, 1, 1, _data+20);
    //         }
    //         //else {  };
    //     break;
    //     // Left = 1
    //     case 1:
    //         // Check if the x-axis can be shifted to the left
    //         if(index_Top_Left_X <= 4 && _data[titl - _distance_Before_Activation] != NULL){
    //             printf("Where do you think your going partner?\n");
    //             // for(size_Of_Current_Room = 0; size_Of_Current_Room < 360; size_Of_Current_Room++){
    //             //     set_bkg_tiles(0, 8, 20, 18, _data);
    //             //     //counter_X += 8;
    //             //     //if(counter_X == 160){ counter_X = 8; counter_Y += 8; }
    //             // }
    //         }
    //     break;
    //     // Right = 2
    //     case 2:
    //         // Check if the x-axis can be shifted to the right
    //         if(index_Top_Left_X <= room_Width_Array[map_Index_Array[current_Position_On_Map]] - 5 && _data[titl + _distance_Before_Activation] != NULL){
    //             printf("Where do you think your going partner?\n");
    //         }
    //     break;
    //     // Up = 4
    //     case 4:
    //         // Check if the x-axis can be shifted to the top
    //         if(_data[titl - room_Width_Array[map_Index_Array[current_Position_On_Map]]] != NULL){
    //             printf("Where do you think your going partner?\n");
    //         }
    //     break;
    // }

    // // if(_data[titl + room_Width_Array[map_Index_Array[current_Position_On_Map]]] != NULL){   // Note: NULL = 0x00?
    // //     printf("Where do you think your going partner?\n");
    // // } 
    // // **********************--------------------------------------------**********************
}

// _height / _width - how many rooms to create
void set_Map(int _height, int _width){
    int index_Counter = 0;
    int selected_Number = 0;

    returned_Map_Height = _height;
    returned_Map_Width = _width;
    size_Of_Map = _height * _width;

    // If size_Of_Map is bigger than MAX_HEIGHT * MAX_WIDTH, let the user know to choose a smaller set of dimensions
    if(size_Of_Map > (MAX_HEIGHT * MAX_WIDTH) + 1){
        printf("Error - Exceeding maximum capacity. Please reconfigure the amount of rooms.");
        delay(99);
        reset();
    }
    else if(size_Of_Map == 0){
        printf("Error - Room size must be One or above. Please reconfigure the amount of rooms.");
        delay(99);
        reset();
    }
    else
    {
        for(index_Counter = 0; index_Counter < size_Of_Map; index_Counter++){
            selected_Number = randw() % 3;
            map_Index_Array[index_Counter] = selected_Number;
            // map_Index_Array[0] = 1;
            // map_Index_Array[1] = 1;
            // map_Index_Array[2] = 0;
            // map_Index_Array[3] = 1;
            // printf("Selected Number: %d \n", selected_Number);
        }
    }

    wait_vbl_done();
    disable_interrupts();

    DISPLAY_OFF;
    HIDE_BKG;
    HIDE_SPRITES;
    // HIDE_WIN;

    SWITCH_RAM_MBC1(2);

    set_bkg_data(0, 2, dungeon_Map_Sprites);

    // Start off at the top left of the map
    set_bkg_tiles(0, 0, room_Width_Array[map_Index_Array[0]], room_Height_Array[map_Index_Array[0]], map_Array[map_Index_Array[0]]);

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    enable_interrupts();

    current_Position_On_Map = 0;
}

void swap_Room(int _direction){
    //int
    // Down = 0
    if(_direction == 0){
        // Check to see if next to bottom edge to map else, move down once
        if(position_On_Map[1] < returned_Map_Height){ position_On_Map[1] += 1; }
    }
    // Left = 1
    else if(_direction == 1){
        // Check to see if next to edge to map else, move left once
        if(position_On_Map[0] - 1 != 0){ position_On_Map[0] -= 1; }
    }
    // Right = 2
    else if(_direction == 2){
        // Check to see if next to edge to map else, move right once
        if(position_On_Map[0] < returned_Map_Width + 1){ position_On_Map[0] += 1; }
    }
    // Up = 3
    else if(_direction == 3){
        // Check to see if next to top edge to map else, move up once
        if(position_On_Map[1] - 1 != 0){ position_On_Map[1] -= 1; }
    }
    current_Position_On_Map = convert_Map_Position(position_On_Map);
    // Here, we would grab an integer value and check if its less than 0, change it to 0.
    set_bkg_tiles(0, 0, room_Width_Array[map_Index_Array[current_Position_On_Map]], room_Height_Array[map_Index_Array[current_Position_On_Map]], map_Array[map_Index_Array[current_Position_On_Map]]);
    delay(99);
}

// Note: Somehow, find a way to have rooms within rooms, and before entering the room, be able to make that room not visible.