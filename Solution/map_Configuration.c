// Ctrl + Shift + B = Build

// MAX_HEIGHT * MAX_WIDTH = 25 (Limit of rooms)
#define MAX_HEIGHT 5
#define MAX_WIDTH 5

#include <gb/gb.h>
#include <rand.h>
#include <stdio.h>
#include <stdlib.h>

#include "dungeon_Item_Tileset.c"
#include "dungeon_Sprites.c"

#include "map_Library.h"

BOOLEAN can_The_Screen_Transition = FALSE;
BOOLEAN has_Map_Changed = FALSE;

char *map_Array[] = { dungeon_Room_One, dungeon_Room_Two };

const char blankmap[1] = {0x00};

int current_Position_In_Room = 0;
int current_Position_On_Map = 0;
// Will keep track of what door we are currently at
int door_Counter = 0;
// index_Top_Left_X = itlx
int itlx = 0;
// index_Top_Left_Y = itly
int itly = 0;
int position_Marker = 0, temporary_Position_Marker = 0;
int returned_Map_Height = 0;
int returned_Map_Width = 0;
int size_Of_Current_Room = 0;
int size_Of_Map = 0;
int returned_X_Value = 0, temporary_Returned_X_Value = 0, returned_Y_Value = 0, temporary_Returned_Y_Value = 0;
// Width Checker equals one because you can't have a map with zero in any of the dimensions
int width_Checker = 1;

// Maximum number of doors to be positioned in a room.
// Where is this door located corresponding to the room
int door_Position_In_Room[4];
int map_Index_Array[MAX_HEIGHT * MAX_WIDTH];
// Position On Map[0] = X position on map, Position On Map[1] = Y position on map;
int position_On_Map[2] = {1, 1};
// room_Height / room_Width array has a provided height and width, each array will hardcode the height and width for each room
// rHAWA[0] = blank_Dungeon_Room, rHAWA[1] = test_Map
int room_Height_Array[2] = { 32, 32 };
int room_Width_Array[2] = { 32, 32 };

// Takes in the current position on the map (Position On Map[2]), and converts the value into the index corresponding to the Map Index Array
int convert_Map_Position(int _map_Position[]){
    int x_Position_Counter = 0;
    int y_Position_Counter = 0;
    int map_Index_Counter = 0;

    for(y_Position_Counter = 0; y_Position_Counter < _map_Position[1] - 1; y_Position_Counter++){ map_Index_Counter += returned_Map_Width; }
    for(x_Position_Counter = 0; x_Position_Counter <= _map_Position[0] - 1; x_Position_Counter++){ map_Index_Counter += 1; }

    map_Index_Counter -= 1;

    // printf("map_Index_Counter = %d\n", map_Index_Counter); delay(2500);

    return(map_Index_Counter);
}

// Takes in the player's current position, and converts the value into the index corresponding to current tile relative to it's position in the current room
void convert_Room_Position(int _player_Position[], int _width_Of_Room){
    itlx = (_player_Position[0] - 8) / 8;
    itly = (_player_Position[1] - 16) / 8;

    current_Position_In_Room = _width_Of_Room * itly + itlx;

    // printf("position_In_Room = [%d][%d] = current_Position_In_Room(%d)\n", itlx, itly, current_Position_In_Room);
}

// When the map gets initialized, information on door placement will be noted. (Keep track of how many rooms are being created and how many doors are being placed in each specific room)
// Note: I forgot that I am setting the bkg tiles which means that each loop will keep setting the tile. I need to only set it for the current map.
void door_Placement(int *_map_Index_Array, int _returned_Map_Width, unsigned char *_data, unsigned char *_room_Height_Array, unsigned char *_room_Width_Array){
    door_Counter = 0;

    // Will check the current room to see if it's in use
    // Check for rooms below the current room
    if(_map_Index_Array[current_Position_On_Map + _returned_Map_Width] != -1){
        // printf("There is a room below this one");
        
        // Place a door (Bottom Arrow)
        temporary_Returned_X_Value = randw() % _room_Width_Array[current_Position_On_Map]; temporary_Returned_Y_Value = randw() % _room_Height_Array[current_Position_On_Map]; 
        // printf("R.X.V = %d R.Y.V = %d\n", returned_X_Value, returned_Y_Value);
        temporary_Position_Marker = _room_Width_Array[current_Position_On_Map] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
        // printf("Position Marker = %d\n", position_Marker);
        while(_data[temporary_Position_Marker] != blankmap[0]){
            temporary_Returned_X_Value = randw() % _room_Width_Array[current_Position_On_Map]; temporary_Returned_Y_Value = randw() % _room_Height_Array[current_Position_On_Map]; 
            temporary_Position_Marker = _room_Width_Array[current_Position_On_Map] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
            
            // printf("R.X.V = %d R.Y.V = %d\n", temporary_Returned_X_Value, temporary_Returned_Y_Value);
            // printf("Position Marker = %d\n", temporary_Position_Marker);
            // delay(250);
        
            if(temporary_Position_Marker == 0){ break; }

            // For some unknown reason, this loop while repeat forever even though it should break out
            // To fix this, we need to save the last piece of information and set the targets to that information
            returned_X_Value = temporary_Returned_X_Value; returned_Y_Value = temporary_Returned_Y_Value; position_Marker = temporary_Position_Marker;
        }
        door_Position_In_Room[door_Counter] = position_Marker;
        // printf("door_Position_In_Room[%d] = %d\n", door_Counter, door_Position_In_Room[door_Counter]);
        
        // When the map gets set up, this will only place the door tiles on the current map.
        set_bkg_tiles(returned_X_Value, returned_Y_Value, 1, 1, &dungeon_Item_Tileset[/*6*/2]);

        door_Counter += 1;
    }

    // Check for rooms to the left of the current room
    // && _current_Position_On_Map - 1 != -1 (Makes sure that the current_Position_On_Map counter does not go out of bounds)
    if(_map_Index_Array[current_Position_On_Map - 1] != -1 && current_Position_On_Map - 1 != -1){
        // printf("There is a room to the left of this one");

        // Place a door (Left Arrow)
        temporary_Returned_X_Value = randw() % _room_Width_Array[current_Position_On_Map]; temporary_Returned_Y_Value = randw() % _room_Height_Array[current_Position_On_Map]; 
        // printf("R.X.V = %d R.Y.V = %d\n", returned_X_Value, returned_Y_Value);
        temporary_Position_Marker = _room_Width_Array[current_Position_On_Map] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
        // printf("Position Marker = %d\n", position_Marker);
        while(_data[temporary_Position_Marker] != blankmap[0]){
            temporary_Returned_X_Value = randw() % _room_Width_Array[current_Position_On_Map]; temporary_Returned_Y_Value = randw() % _room_Height_Array[current_Position_On_Map]; 
            temporary_Position_Marker = _room_Width_Array[current_Position_On_Map] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
            
            // printf("R.X.V = %d R.Y.V = %d\n", temporary_Returned_X_Value, temporary_Returned_Y_Value);
            // printf("Position Marker = %d\n", temporary_Position_Marker);
            // delay(250);
        
            if(temporary_Position_Marker == 0){ break; }

            // For some unknown reason, this loop while repeat forever even though it should break out
            // To fix this, we need to save the last piece of information and set the targets to that information
            returned_X_Value = temporary_Returned_X_Value; returned_Y_Value = temporary_Returned_Y_Value; position_Marker = temporary_Position_Marker;
        }
        door_Position_In_Room[door_Counter] = position_Marker;
        // printf("door_Position_In_Room[%d] = %d\n", door_Counter, door_Position_In_Room[door_Counter]);
        
        // When the map gets set up, this will only place the door tiles on the current map.
        set_bkg_tiles(returned_X_Value, returned_Y_Value, 1, 1, &dungeon_Item_Tileset[3]);

        door_Counter += 1;
    }

    // Check for rooms to the right of the current room
    if(_map_Index_Array[current_Position_On_Map + 1] != -1 && current_Position_On_Map + 1 < 25 && width_Checker + 1 < _returned_Map_Width){
        // printf("There is a room to the right of this one");

        // Place a door (Right Arrow)
        temporary_Returned_X_Value = randw() % _room_Width_Array[current_Position_On_Map]; temporary_Returned_Y_Value = randw() % _room_Height_Array[current_Position_On_Map]; 
        // printf("R.X.V = %d R.Y.V = %d\n", returned_X_Value, returned_Y_Value);
        temporary_Position_Marker = _room_Width_Array[current_Position_On_Map] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
        // printf("Position Marker = %d\n", position_Marker);
        while(_data[temporary_Position_Marker] != blankmap[0]){
            temporary_Returned_X_Value = randw() % _room_Width_Array[current_Position_On_Map]; temporary_Returned_Y_Value = randw() % _room_Height_Array[current_Position_On_Map]; 
            temporary_Position_Marker = _room_Width_Array[current_Position_On_Map] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
            
            // printf("R.X.V = %d R.Y.V = %d\n", temporary_Returned_X_Value, temporary_Returned_Y_Value);
            // printf("Position Marker = %d\n", temporary_Position_Marker);
            // delay(250);
        
            if(temporary_Position_Marker == 0){ break; }

            // For some unknown reason, this loop while repeat forever even though it should break out
            // To fix this, we need to save the last piece of information and set the targets to that information
            returned_X_Value = temporary_Returned_X_Value; returned_Y_Value = temporary_Returned_Y_Value; position_Marker = temporary_Position_Marker;
        }
        door_Position_In_Room[door_Counter] = position_Marker;
        // printf("door_Position_In_Room[%d] = %d\n", door_Counter, door_Position_In_Room[door_Counter]);
        
        // When the map gets set up, this will only place the door tiles on the current map.
        set_bkg_tiles(returned_X_Value, returned_Y_Value, 1, 1, &dungeon_Item_Tileset[4]);
        
        door_Counter += 1;
    }

    // Check for any rooms to the top of the current room
    if(_map_Index_Array[current_Position_On_Map - _returned_Map_Width] != -1 && current_Position_On_Map - _returned_Map_Width > 0){
        // printf("There is a room on top of this one");

        // Place a door (Top Arrow)
        temporary_Returned_X_Value = randw() % _room_Width_Array[current_Position_On_Map]; temporary_Returned_Y_Value = randw() % _room_Height_Array[current_Position_On_Map]; 
        // printf("R.X.V = %d R.Y.V = %d\n", returned_X_Value, returned_Y_Value);
        temporary_Position_Marker = _room_Width_Array[current_Position_On_Map] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
        // printf("Position Marker = %d\n", position_Marker);
        while(_data[temporary_Position_Marker] != blankmap[0]){
            temporary_Returned_X_Value = randw() % _room_Width_Array[current_Position_On_Map]; temporary_Returned_Y_Value = randw() % _room_Height_Array[current_Position_On_Map]; 
            temporary_Position_Marker = _room_Width_Array[current_Position_On_Map] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
            
            // printf("R.X.V = %d R.Y.V = %d\n", temporary_Returned_X_Value, temporary_Returned_Y_Value);
            // printf("Position Marker = %d\n", temporary_Position_Marker);
            // delay(250);
        
            if(temporary_Position_Marker == 0){ break; }

            // For some unknown reason, this loop while repeat forever even though it should break out
            // To fix this, we need to save the last piece of information and set the targets to that information
            returned_X_Value = temporary_Returned_X_Value; returned_Y_Value = temporary_Returned_Y_Value; position_Marker = temporary_Position_Marker;
        }
        door_Position_In_Room[door_Counter] = position_Marker;
        // printf("door_Position_In_Room[%d] = %d\n", door_Counter, door_Position_In_Room[door_Counter]);
        
        // When the map gets set up, this will only place the door tiles on the current map.
        set_bkg_tiles(returned_X_Value, returned_Y_Value, 1, 1, &dungeon_Item_Tileset[5]);

        door_Counter += 1;
    }
}

// Move the player, then move the background
void move_Background(int _activation_Distance, int *_player_Position, unsigned char *_data){
    int a_D_Counter = 0;

    convert_Room_Position(_player_Position, room_Width_Array[map_Index_Array[current_Position_On_Map]]);
    //printf("current_Position_In_Room = %d\n", current_Position_In_Room);

    wait_vbl_done();

    // Debug
    if(joypad() & J_A){
        // for(a_D_Counter = 0; a_D_Counter < _activation_Distance; a_D_Counter++){
        //     // If the next two tiles ahead of the player are occupied
        //     // if(_data[current_Position_In_Room + a_D_Counter] != blankmap[0]){ printf("There are tiles ahead of the player.\n"); } 
        //     if(_data[current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)] != blankmap[0]){ printf("There are tiles ahead of the player. (titl = %d)\n\n", current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)); }
        //     else{ printf("There are no tiles ahead of the player. (titl = %d)\n\n", current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)); }
        //     delay(500);
        // }
        printf("\nCurrent Position In Room: %d\n\n", current_Position_In_Room);
        // printf("\nindex_Top_Left_X: %d\n\n", index_Top_Left_X);
        // printf("\nindex_Top_Left_Y: %d\n\n", index_Top_Left_Y);
        // printf("Current Position In The Room  = %d\n", current_Position_In_Room);
        // printf("Size of the current room = %d", size_Of_Current_Room);
        // printf ("Is %d bigger than %d(size of the room)?", current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * 2), size_Of_Current_Room);
    }

    switch(joypad()){
        case J_DOWN:
            // If the player is within activation distance, check if any of the tiles ahead of the player, are not being currently occupied
            if(index_Top_Left_Y > 17 - _activation_Distance && index_Top_Left_Y < 18){
                // When checking the tiles in front of the player, make sure the next tile checked, is not bigger than the size of the current map. (If checking outside of the map's range, will always show as there is an empty tile).
                if(current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * 2) < size_Of_Current_Room) { 
                    for(a_D_Counter = 0; a_D_Counter < _activation_Distance; a_D_Counter++){
                        if(_data[current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)] != blankmap[0]){ can_The_Screen_Transition = FALSE; delay(1); }
                        else if(_data[current_Position_In_Room + (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)] == blankmap[0]){ can_The_Screen_Transition = TRUE; }
                    }
                
                    // If the tiles ahead of the player are not occupied at the moment
                    if(!can_The_Screen_Transition){ if(_data[current_Position_In_Room + room_Width_Array[map_Index_Array[current_Position_On_Map]]] == blankmap[0]){ enable_Movement(TRUE); } }
                    else if(can_The_Screen_Transition){ enable_Movement(FALSE); player_Position[1] += 8; scroll_bkg(0, 8); }
                }
            }

            if(_data[current_Position_In_Room + room_Width_Array[map_Index_Array[current_Position_On_Map]]] != blankmap[0]){ enable_Movement(FALSE); }
            else{ enable_Movement(TRUE); }
            
            can_The_Screen_Transition = FALSE;
        break;
        case J_LEFT:
            // If the player is within activation distance, check if any of the tiles ahead of the player, are not being currently occupied
            if(index_Top_Left_X < _activation_Distance){
                for(a_D_Counter = 0; a_D_Counter < _activation_Distance; a_D_Counter++){ 
                    if(_data[current_Position_In_Room - a_D_Counter] != blankmap[0]){ can_The_Screen_Transition = FALSE; delay(1); }
                    else if(_data[current_Position_In_Room - a_D_Counter] == blankmap[0]){ can_The_Screen_Transition = TRUE; }
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
        case J_UP:
            // If the player is within activation distance, check if any of the tiles ahead of the player, are not being currently occupied
            if(index_Top_Left_Y <= _activation_Distance){
                // When checking the tiles in front of the player, make sure the next tile checked, is not bigger than the size of the current map. (If checking outside of the map's range, will always show as there is an empty tile).
                if(current_Position_In_Room - (room_Width_Array[map_Index_Array[current_Position_On_Map]] * 2) > 0) { 
                    for(a_D_Counter = 0; a_D_Counter < _activation_Distance; a_D_Counter++){
                    
                    if(_data[current_Position_In_Room - (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)] != blankmap[0]){ can_The_Screen_Transition = FALSE; delay(1); }
                    else if(_data[current_Position_In_Room - (room_Width_Array[map_Index_Array[current_Position_On_Map]] * a_D_Counter)] == blankmap[0]){ can_The_Screen_Transition = TRUE; }
                }
                
                // If the tiles ahead of the player are not occupied at the moment
                if(!can_The_Screen_Transition){ if(_data[current_Position_In_Room - room_Width_Array[map_Index_Array[current_Position_On_Map]]] == blankmap[0]){ enable_Movement(TRUE); } }
                else if(can_The_Screen_Transition){ enable_Movement(FALSE); player_Position[1] -= 8; scroll_bkg(0, -8); }
                }
            }

            if(_data[current_Position_In_Room - room_Width_Array[map_Index_Array[current_Position_On_Map]]] != blankmap[0]){ enable_Movement(FALSE); }
            else{ enable_Movement(TRUE); }
            
            can_The_Screen_Transition = FALSE;
        break;
    }
}

// _height / _width - how many rooms to create
void set_Map(int _height, int _width){
    int index_Counter = 0;
    int selected_Number = 0;

    // Set all the values in the map_Index_Array to negative one. This will come in handy later on when checking for door placement.
    // This is because when the current room is checking to see if any other rooms are avaliable, if the next room is negative one,
    // that means that the room is not in use.
    for(index_Counter = 0; index_Counter < 25; index_Counter++){ map_Index_Array[index_Counter] = -1; }

    returned_Map_Height = _height;
    returned_Map_Width = _width;
    size_Of_Map = _height * _width;

    // TODO: Make sure the user can not put any negative numbers
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
            map_Index_Array[index_Counter] = selected_Number;
            // map_Index_Array[0] = 1;
            // map_Index_Array[1] = 1;
            // map_Index_Array[2] = 0;
            // map_Index_Array[3] = 1;
            // printf("Selected Number: %d \n", selected_Number);
        }
    }
    size_Of_Current_Room = room_Height_Array[map_Index_Array[0]] * room_Width_Array[map_Index_Array[0]];

    wait_vbl_done();
    disable_interrupts();

    DISPLAY_OFF;
    HIDE_BKG;
    HIDE_SPRITES;
    // HIDE_WIN;

    SWITCH_RAM_MBC1(2);

    set_bkg_data(0, 8, dungeon_Sprites);

    // Start off at the top left of the map
    set_bkg_tiles(0, 0, room_Width_Array[map_Index_Array[0]], room_Height_Array[map_Index_Array[0]], map_Array[map_Index_Array[0]]);

    // After the map has been created, instantiate the player afterwards
    set_sprite_data(0, 2, character_With_A_Hat);
    set_sprite_tile(0, 1);

    instantiate_Player(map_Array[map_Index_Array[0]]);
    update_Player(index_Top_Left_X, index_Top_Left_Y);

    door_Placement(map_Index_Array, returned_Map_Width, map_Array[map_Index_Array[current_Position_On_Map]], room_Height_Array, room_Width_Array);

    DISPLAY_ON;
    SHOW_BKG;
    SHOW_SPRITES;

    enable_interrupts();

    current_Position_On_Map = 0;
    
    // for(index_Counter = 0; index_Counter < 25; index_Counter++){ printf("Map Index Array[%d] = %d\n", index_Counter, map_Index_Array[index_Counter]); delay(250); }
}

// Update: Will no longer take in direction, will always check until the user steps on a specific tile based on their INDEX POSITION
void swap_Room(){
    if(!has_Map_Changed){
        // Down
        if(current_Position_In_Room == door_Position_In_Room[0]){ has_Map_Changed = TRUE; position_On_Map[1] += 1; }
        // Left
        else if(current_Position_In_Room == door_Position_In_Room[1]){ has_Map_Changed = TRUE; position_On_Map[0] -= 1; }
        // Right 
        else if(current_Position_In_Room == door_Position_In_Room[2]){ has_Map_Changed = TRUE; position_On_Map[0] += 1; }
        // Down
        else if(current_Position_In_Room == door_Position_In_Room[3]){ has_Map_Changed = TRUE; position_On_Map[1] -= 1; }
    }
    else{
        has_Empty_Tile_Been_Found = FALSE;
        has_Map_Changed = FALSE;
        has_Players_Position_Been_Set = FALSE;

        current_Position_On_Map = convert_Map_Position(position_On_Map);
        size_Of_Current_Room = room_Height_Array[map_Index_Array[current_Position_On_Map]] * room_Width_Array[map_Index_Array[current_Position_On_Map]];

        door_Placement(map_Index_Array, returned_Map_Width, map_Array[map_Index_Array[current_Position_On_Map]], room_Height_Array, room_Width_Array);
        
        instantiate_Player(map_Array[map_Index_Array[current_Position_On_Map]]);

        set_bkg_tiles(0, 0, room_Width_Array[map_Index_Array[current_Position_On_Map]], room_Height_Array[map_Index_Array[current_Position_On_Map]], map_Array[map_Index_Array[current_Position_On_Map]]);

        // printf("Current Position in Room = %d", current_Position_In_Room);
    }
}