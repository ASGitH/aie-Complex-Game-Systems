// NOTE: When Checking For The Door, Make Sure That You Check If That Current Tile Equals &tileset[6]

#include <gb/gb.h>
#include <rand.h>

#include "dungeon_Item_Tileset.c"
#include "dungeon_Sprites.c"

typedef struct door_Object{
    // Where is this door located corresponding to the map
    UINT8 map_Index;
    // In the room, the door would be position...
    UINT8 door_Position[2];

    // What direction this object will lead to
    unsigned char direction;
} door_Object;

const char blank_Tile[1] = {0x00};

// Current Position On Map = C.P.O.M.
int c_P_O_M = 0;
// Will keep track of what door we are currently at
int door_Counter = 0;
int position_Marker = 0, temporary_Position_Marker = 0;
int returned_X_Value = 0, temporary_Returned_X_Value = 0, returned_Y_Value = 0, temporary_Returned_Y_Value = 0;
// Width Checker equals one because you can't have a map with zero in any of the dimensions
int width_Checker = 1;

// Maximum number of doors to be positioned in a 5 x 5 (25 rooms)map.
door_Object d_O_Array[81];

void door_Initilization(int _current_Position_On_Map, int _position_X, int _position_Y, door_Object *_d_O, unsigned char _direction, unsigned char *_map_Index_Array){
    _d_O->door_Position[0] = _position_X; _d_O->door_Position[1] = _position_Y;
    _d_O->direction = _direction;
    _d_O->map_Index = _map_Index_Array[_current_Position_On_Map];
}

// When the map gets initialized, information on door placement will be noted. (Keep track of how many rooms are being created and how many doors are being placed in each specific room)
// Note: I forgot that I am setting the bkg tiles which means that each loop will keep setting the tile. I need to only set it for the current map.
void door_Placement(int _current_Position_On_Map, int *_map_Index_Array, int _returned_Map_Width, unsigned char *_data, unsigned char *_room_Height_Array, unsigned char *_room_Width_Array){
    c_P_O_M = _current_Position_On_Map;

    for(door_Counter = 0; door_Counter < 81; door_Counter++){
        d_O_Array[door_Counter].door_Position[0] = -1; d_O_Array[door_Counter].door_Position[1] = -1;
        d_O_Array[door_Counter].direction = 'a';
        d_O_Array[door_Counter].map_Index = -1;
    }

    // Set all the values in the d_O_Array to null. This will come in handy later on when checking for door placement.
    // This is because when the current room is checking to see if any other rooms are avaliable, if the next door_Object is null,
    // that means that the door is not in use.
    // Will check the current room to see if it's in use
    while(_map_Index_Array[c_P_O_M] != -1){
        // for(door_Counter = 0; door_Counter < 25; door_Counter++){ printf("Map Index Array[%d] = %d\n", door_Counter, _map_Index_Array[door_Counter]); delay(99); }
    
        // Check for rooms below the current room
        if(_map_Index_Array[c_P_O_M + _returned_Map_Width] != -1 && c_P_O_M + _returned_Map_Width < 25){
            // printf("There is a room below this one");
            // Place a door (Bottom Arrow)
            temporary_Returned_X_Value = randw() % _room_Width_Array[c_P_O_M]; temporary_Returned_Y_Value = randw() % _room_Height_Array[c_P_O_M]; 
            // printf("R.X.V = %d R.Y.V = %d\n", returned_X_Value, returned_Y_Value);
            temporary_Position_Marker = _room_Width_Array[c_P_O_M] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
            // printf("Position Marker = %d\n", position_Marker);
            while(_data[temporary_Position_Marker] != blank_Tile[0]){
                temporary_Returned_X_Value = randw() % _room_Width_Array[c_P_O_M]; temporary_Returned_Y_Value = randw() % _room_Height_Array[c_P_O_M]; 
                temporary_Position_Marker = _room_Width_Array[c_P_O_M] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
                
                // printf("R.X.V = %d R.Y.V = %d\n", temporary_Returned_X_Value, temporary_Returned_Y_Value);
                // printf("Position Marker = %d\n", temporary_Position_Marker);
                // delay(250);
            
                if(temporary_Position_Marker == 0){ break; }

                // For some unknown reason, this loop while repeat forever even though it should break out
                // To fix this, we need to save the last piece of information and set the targets to that information
                returned_X_Value = temporary_Returned_X_Value; returned_Y_Value = temporary_Returned_Y_Value; position_Marker = temporary_Position_Marker;
            }
            if(c_P_O_M < 1){ set_bkg_tiles(returned_X_Value, returned_Y_Value, 1, 1, &dungeon_Item_Tileset[/*6*/2]); }
            
            // Record information on the door's properties
            door_Initilization(c_P_O_M, returned_X_Value, returned_Y_Value, &d_O_Array[door_Counter], 'd', _map_Index_Array[c_P_O_M]);

            // printf("d_O_Array[%d]->door_Position[x] = %d\n", door_Counter, d_O_Array[door_Counter].door_Position[0]);
            // printf("d_O_Array[%d]->door_Position[y] = %d\n", door_Counter, d_O_Array[door_Counter].door_Position[1]);
            // printf("d_O_Array[%d]->map_Index = %d\n", door_Counter, d_O_Array[door_Counter].map_Index);

            door_Counter += 1;
        }

        // Check for rooms to the left of the current room
        // && _current_Position_On_Map - 1 != -1 (Makes sure that the current_Position_On_Map counter does not go out of bounds)
        if(_map_Index_Array[c_P_O_M - 1] != -1 && c_P_O_M - 1 != -1){
            // printf("There is a room to the left of this one");
            // Place a door (Left Arrow)
            temporary_Returned_X_Value = randw() % _room_Width_Array[c_P_O_M]; temporary_Returned_Y_Value = randw() % _room_Height_Array[c_P_O_M]; 
            // printf("R.X.V = %d R.Y.V = %d\n", returned_X_Value, returned_Y_Value);
            temporary_Position_Marker = _room_Width_Array[c_P_O_M] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
            // printf("Position Marker = %d\n", position_Marker);
            while(_data[temporary_Position_Marker] != blank_Tile[0]){
                temporary_Returned_X_Value = randw() % _room_Width_Array[c_P_O_M]; temporary_Returned_Y_Value = randw() % _room_Height_Array[c_P_O_M]; 
                temporary_Position_Marker = _room_Width_Array[c_P_O_M] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
                
                // printf("R.X.V = %d R.Y.V = %d\n", temporary_Returned_X_Value, temporary_Returned_Y_Value);
                // printf("Position Marker = %d\n", temporary_Position_Marker);
                // delay(250);
            
                if(temporary_Position_Marker == 0){ break; }

                // For some unknown reason, this loop while repeat forever even though it should break out
                // To fix this, we need to save the last piece of information and set the targets to that information
                returned_X_Value = temporary_Returned_X_Value; returned_Y_Value = temporary_Returned_Y_Value; position_Marker = temporary_Position_Marker;
            }
            if(c_P_O_M < 1){ set_bkg_tiles(returned_X_Value, returned_Y_Value, 1, 1, &dungeon_Item_Tileset[3]); }
            
            // Record information on the door's properties
            door_Initilization(c_P_O_M, returned_X_Value, returned_Y_Value, &d_O_Array[door_Counter], 'l', _map_Index_Array[c_P_O_M]);

            // printf("d_O_Array[%d]->door_Position[x] = %d\n", door_Counter, d_O_Array[door_Counter].door_Position[0]);
            // printf("d_O_Array[%d]->door_Position[y] = %d\n", door_Counter, d_O_Array[door_Counter].door_Position[1]);
            // printf("d_O_Array[%d]->map_Index = %d\n", door_Counter, d_O_Array[door_Counter].map_Index);

            door_Counter += 1;
        }

        // Check for rooms to the right of the current room
        if(_map_Index_Array[c_P_O_M + 1] != -1 && c_P_O_M + 1 < 25 && width_Checker + 1 < _returned_Map_Width){
            // printf("There is a room to the right of this one");
            // Place a door (Right Arrow)
            temporary_Returned_X_Value = randw() % _room_Width_Array[c_P_O_M]; temporary_Returned_Y_Value = randw() % _room_Height_Array[c_P_O_M]; 
            // printf("R.X.V = %d R.Y.V = %d\n", returned_X_Value, returned_Y_Value);
            temporary_Position_Marker = _room_Width_Array[c_P_O_M] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
            // printf("Position Marker = %d\n", position_Marker);
            while(_data[temporary_Position_Marker] != blank_Tile[0]){
                temporary_Returned_X_Value = randw() % _room_Width_Array[c_P_O_M]; temporary_Returned_Y_Value = randw() % _room_Height_Array[c_P_O_M]; 
                temporary_Position_Marker = _room_Width_Array[c_P_O_M] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
                
                // printf("R.X.V = %d R.Y.V = %d\n", temporary_Returned_X_Value, temporary_Returned_Y_Value);
                // printf("Position Marker = %d\n", temporary_Position_Marker);
                // delay(250);
            
                if(temporary_Position_Marker == 0){ break; }

                // For some unknown reason, this loop while repeat forever even though it should break out
                // To fix this, we need to save the last piece of information and set the targets to that information
                returned_X_Value = temporary_Returned_X_Value; returned_Y_Value = temporary_Returned_Y_Value; position_Marker = temporary_Position_Marker;
            }
            if(c_P_O_M < 1){ set_bkg_tiles(returned_X_Value, returned_Y_Value, 1, 1, &dungeon_Item_Tileset[4]); }

            // Record information on the door's properties
            door_Initilization(c_P_O_M, returned_X_Value, returned_Y_Value, &d_O_Array[door_Counter], 'r', _map_Index_Array[c_P_O_M]);

            // printf("d_O_Array[%d]->door_Position[x] = %d\n", door_Counter, d_O_Array[door_Counter].door_Position[0]);
            // printf("d_O_Array[%d]->door_Position[y] = %d\n", door_Counter, d_O_Array[door_Counter].door_Position[1]);
            // printf("d_O_Array[%d]->map_Index = %d\n", door_Counter, d_O_Array[door_Counter].map_Index);

            door_Counter += 1;
        }

        // Check for any rooms to the top of the current room
        if(_map_Index_Array[c_P_O_M - _returned_Map_Width] != -1 && c_P_O_M - _returned_Map_Width > 0){
            // printf("There is a room on top of this one");
            // Place a door (Top Arrow)
            temporary_Returned_X_Value = randw() % _room_Width_Array[c_P_O_M]; temporary_Returned_Y_Value = randw() % _room_Height_Array[c_P_O_M]; 
            // printf("R.X.V = %d R.Y.V = %d\n", returned_X_Value, returned_Y_Value);
            temporary_Position_Marker = _room_Width_Array[c_P_O_M] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
            // printf("Position Marker = %d\n", position_Marker);
            while(_data[temporary_Position_Marker] != blank_Tile[0]){
                temporary_Returned_X_Value = randw() % _room_Width_Array[c_P_O_M]; temporary_Returned_Y_Value = randw() % _room_Height_Array[c_P_O_M]; 
                temporary_Position_Marker = _room_Width_Array[c_P_O_M] * temporary_Returned_Y_Value + temporary_Returned_X_Value;
                
                // printf("R.X.V = %d R.Y.V = %d\n", temporary_Returned_X_Value, temporary_Returned_Y_Value);
                // printf("Position Marker = %d\n", temporary_Position_Marker);
                // delay(250);
            
                if(temporary_Position_Marker == 0){ break; }

                // For some unknown reason, this loop while repeat forever even though it should break out
                // To fix this, we need to save the last piece of information and set the targets to that information
                returned_X_Value = temporary_Returned_X_Value; returned_Y_Value = temporary_Returned_Y_Value; position_Marker = temporary_Position_Marker;
            }
            if(c_P_O_M < 1){ set_bkg_tiles(returned_X_Value, returned_Y_Value, 1, 1, &dungeon_Item_Tileset[5]); }
            
            // Record information on the door's properties
            door_Initilization(c_P_O_M, returned_X_Value, returned_Y_Value, &d_O_Array[door_Counter], 'u', _map_Index_Array[c_P_O_M]);

            // printf("d_O_Array[%d]->door_Position[x] = %d\n", door_Counter, d_O_Array[door_Counter].door_Position[0]);
            // printf("d_O_Array[%d]->door_Position[y] = %d\n", door_Counter, d_O_Array[door_Counter].door_Position[1]);
            // printf("d_O_Array[%d]->map_Index = %d\n", door_Counter, d_O_Array[door_Counter].map_Index);

            door_Counter += 1;
        }
        c_P_O_M += 1;
        //printf("c.P.O.M = %d\n", c_P_O_M);
    }
    // We are gonna be changing _current_Position_On_Map, so when all the doors have been set up, make sure to reset it back to Zero ( =
    c_P_O_M = 0;
}

// Plan: Make a int array that will take in two ints, x and y and for the current room the player is in, check the to see if the player is in
// the same position (index) as the door(make sure to only grab the doors that belong to the current room)
// ONLY CALL THIS FUNCTION AT THE START OF THE SCENE, AND WHEN YOU CHANGE ROOMS
void retrieve_D_O_Information(int _current_Position_On_Map, int _integer_Array_X[], int _integer_Array_Y[], unsigned char *_directional_Array){
    int index_Counter = 0;
    int integer_Counter = 0;

    // Your gonna need an two-dimensional array. The max amount of columns you can have are 2 (x and y).
    // Your also gonna need another array to keep track of what direction, that door is leading.
    // The max amount of doors per room are four.
    for(index_Counter = 0; index_Counter < 4; index_Counter++){ _integer_Array_X[index_Counter] = -1; _integer_Array_Y[index_Counter] = -1; }

    // Afterwards, your gonna need to check the d_O array and grab the door's information based on the current room
    for(index_Counter = 0; index_Counter < 81; index_Counter++){
        if(d_O_Array[index_Counter].map_Index == _current_Position_On_Map){
            _directional_Array[integer_Counter] = d_O_Array[index_Counter].direction;

            _integer_Array_X[integer_Counter] = d_O_Array[index_Counter].door_Position[0];
            _integer_Array_Y[integer_Counter] = d_O_Array[index_Counter].door_Position[1];
            integer_Counter += 1;
        }
    }
}

// Will grab the door_Object's information and reinstantiate based on the current room
// void[] retrieve_D_O_To_Reinstantiate_Room(){}