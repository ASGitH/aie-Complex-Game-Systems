#include <gb/gb.h>
#include <gb/console.h>
#include <stdio.h>

#include "map_Configuration.c"

// Columns equal the amount of elements per row (Think of width). Rows equal the amount of groups (Think of height)
BOOLEAN has_Columns_Been_Set = FALSE, has_Rows_Been_Set = FALSE;
BOOLEAN is_the_Player_Ready = FALSE;

int chosen_Height_Value = 1, chosen_Width_Value = 1;

unsigned int r, seed;

// Menu Functions
void title_screen(void) {
    r = 0; seed = 0;

    while(!is_the_Player_Ready){
        seed += 1;

        if(seed >= 255){ seed = 1; }

        // Display the row selection text
        if(!has_Rows_Been_Set)
        {
            gotoxy(0, 0);
            
            // Rows equals height
            printf("How many rows would you like?");

            // The question above will take two spaces (rows), when the application accepts input, offset by one (y-axis), and ten (x-axis)
            gotoxy(10, 1);
            
            printf("%d", chosen_Height_Value);

            switch(joypad()){
                // Confirm the current height value
                case J_A: has_Rows_Been_Set = TRUE; break;

                // Decrement the height value
                case J_DOWN:
                    chosen_Height_Value -= 1; 
            
                    if(chosen_Height_Value < 1) { chosen_Height_Value = 25; }

                    if(chosen_Height_Value < 10) {
                        gotoxy(11, 1);

                        printf(" ");

                        gotoxy(10, 1);
                    }
                break;

                // Increment the height value
                case J_UP:
                    chosen_Height_Value += 1;

                    if(chosen_Height_Value > (MAX_HEIGHT * MAX_WIDTH) + 1) { 
                        chosen_Height_Value = 1; 
                    
                        gotoxy(11, 1);

                        printf(" ");

                        gotoxy(10, 1);
                    }
                break;
            }

            delay(250);
        }
        // Display the column selection text
        else if(!has_Columns_Been_Set)
        {
            gotoxy(0, 3);
            
            // Rows equals height
            printf("How many columns would you like?");

            // The question above will take two spaces (rows), when the application accepts input, offset by one (y-axis), and ten (x-axis)
            gotoxy(13, 4);
            
            printf("%d", chosen_Width_Value);

            switch(joypad()){
                // Confirm the current height value
                case J_A: has_Columns_Been_Set = TRUE; is_the_Player_Ready = TRUE; break;

                // Decrement the height value
                case J_DOWN:
                    chosen_Width_Value -= 1; 
            
                    if(chosen_Width_Value < 1) { chosen_Width_Value = 25; }

                    if(chosen_Width_Value < 10) {
                        gotoxy(14, 4);

                        printf(" ");

                        gotoxy(13, 4);
                    }
                break;

                // Increment the height value
                case J_UP:
                    chosen_Width_Value += 1;

                    if(chosen_Width_Value > MAX_HEIGHT * MAX_WIDTH) { 
                        chosen_Width_Value = 1; 
                    
                        gotoxy(14, 4);

                        printf(" ");

                        gotoxy(13, 4);
                    }
                break;
            }
    
            delay(250);
        }
    }

    // If size_Of_Map is bigger than MAX_HEIGHT * MAX_WIDTH, let the user know to choose a smaller set of dimensions
    if(chosen_Height_Value * chosen_Width_Value > MAX_HEIGHT * MAX_WIDTH){
        printf("\n\nError - Exceeding maximum capacity. Please reconfigure the amount of rooms. (Max amount of rooms equals a total of 25)\n");
        delay(5000);
        reset();
    }
}

void main(){
    title_screen();

    set_Map(chosen_Height_Value, chosen_Width_Value);

    while(TRUE){
        move_Background(3, player_Position, map_Array[map_Index_Array[current_Position_On_Map]]);
        move_Player();
        swap_Room();
    }
}