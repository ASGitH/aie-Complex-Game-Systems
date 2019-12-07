// Ctrl + Shift + B = Build

// MAX_HEIGHT * MAX_WIDTH = 25 (Limit of rooms)
#define MAX_HEIGHT 5
#define MAX_WIDTH 5

#include <gb/gb.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>

#include "map_Library.h"

BOOLEAN has_Count_Been_Reset = FALSE;

char *map_Array[] = { blank_Dungeon_Room, background_Scroller_Map };

int current_Position_In_Room = 0;
int current_Position_On_Map = 0;
// index_Top_Left_X = itlx
int itlx = 0;
// index_Top_Left_Y = itly
int itly = 0;
int returned_Map_Height = 0;
int returned_Map_Width = 0;
int size_Of_Map = 0;
// tile_Index_Top_Left = titl
// int titl = 0;

int map_Index_Array[MAX_HEIGHT * MAX_WIDTH];
// Position On Map[0] = X position on map, Position On Map[1] = Y position on map;
int position_On_Map[2] = {1, 1};
// room_Height / room_Width array has a provided height and width, each array will hardcode the height and width for each room
// rHAWA[0] = blank_Dungeon_Room, rHAWA[1] = test_Map
int room_Height_Array[2] = { 18, 18 };
int room_Width_Array[2] = { 20, 100 };

//  **********************---------------------- Writing a Background Scroller Program ----------------------**********************
BYTE tile_Counter = 0;

UBYTE scroll_X = 0;
UBYTE i = 0;
UBYTE temporary_A, temporary_B;
UWORD counter = 20;
// **********************-----------------------------------------------------------------------------------**********************

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

    // printf("position_In_Room = [%d][%d] = room_Tile_Index(%d)\n", itlx, itly, titl);
}

// Move the player, then move the background
void move_Background(int _activation_Distance){
    convert_Room_Position(player_Position, room_Width_Array[map_Index_Array[current_Position_On_Map]]);

    update_Player(itlx, itly);

    wait_vbl_done();

    // if(joypad() & J_RIGHT){
    //     // set_bkg_tiles(19, 0, 1, 1, &(map_Array[map_Index_Array[current_Position_On_Map]] + 41));
    //     // 20, 120, 220, 320, 420, 520, 620
    //     set_bkg_tiles(18, 1, 1, 1, &(map_Array[map_Index_Array[current_Position_On_Map]] + 893));
    //     //counter += 1;
    //     //set_bkg_tiles(19, 2, 1, 1, &(map_Array[map_Index_Array[current_Position_On_Map]] + 39 + 200));
    //     delay(99);
    // }

    switch(joypad()){
        case J_LEFT:
            if(itlx < _activation_Distance){ enable_Movement(FALSE); scroll_bkg( -8, 0 ); }
            else{ enable_Movement(TRUE); }
            // tile_Counter--;
        break;
        case J_RIGHT:
            if(itlx > 20 - _activation_Distance - 1 && itlx < 20){
                enable_Movement(FALSE);
                scroll_bkg(8, 0);
                tile_Counter += 1;
            }
            else{ enable_Movement(TRUE); }
        break;
    }

    if(tile_Counter != 0){
        scroll_X += 1;
        tile_Counter = 0;
        counter = scroll_X + 21;
        temporary_B = counter % 32;
        for(temporary_A = 0; temporary_A != 18; temporary_A++){
            set_bkg_tiles(temporary_B, temporary_A, 1, 1, &(map_Array[map_Index_Array[current_Position_On_Map]] + counter));
            counter += room_Width_Array[map_Index_Array[current_Position_On_Map]];
        }
    }
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
            selected_Number = randw() % 2;
            //map_Index_Array[index_Counter] = selected_Number;
            map_Index_Array[0] = 1;
            map_Index_Array[1] = 1;
            map_Index_Array[2] = 0;
            map_Index_Array[3] = 1;
            //printf("Selected Number: %d \n", selected_Number);
        }
    }

    //  **********************---------------------- Writing a Background Scroller Program ----------------------**********************
    wait_vbl_done();
    disable_interrupts();

    DISPLAY_OFF;
    HIDE_BKG;
    HIDE_SPRITES;
    // HIDE_WIN;

    SWITCH_RAM_MBC1(2);

    set_bkg_data(0, 2, dungeon_Map_Sprites);

    counter = 0;

    // Start off at the top left of the map
    set_bkg_tiles(0, 0, room_Width_Array[map_Index_Array[0]], room_Height_Array[map_Index_Array[0]], map_Array[1]);

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    enable_interrupts();
    // **********************-----------------------------------------------------------------------------------**********************

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
        if(position_On_Map[0] < returned_Map_Width){ position_On_Map[0] += 1; }
    }
    // Up = 3
    else if(_direction == 3){
        // Check to see if next to top edge to map else, move up once
        if(position_On_Map[1] - 1 != 0){ position_On_Map[1] -= 1; }
    }
    current_Position_On_Map = convert_Map_Position(position_On_Map);
    // Here, we would grab an integer value and check if its less than 0, change it to 0.
    set_bkg_tiles(0, 0, 20, 18, map_Array[map_Index_Array[current_Position_On_Map]]);
    delay(99);
}