#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "serialize.h"

char ubit[]={'a','b','i','r','a','l','k','h'};

bool test_input(char *input){
    bool x = false;
    int count = 0;
    while(*input != '\0'){
        input++;
        count++;
    }
    if(count <  MAX_MESSAGE_SIZE){
        x = true;
    }
    return x;
}

int target_length(char *input){
    input++;
    int count = 0;
    while(*input != ' '){
        input++;
        count++;
    }
    return count;
}
int message_length_labelled(char *input){
    int count = 0;
    while(*input != ' '){
        input++;
    }
    while(*input == ' '){
        input++;
    }
    while(*input != '\0'){
        input++;
        count++;
    }
    return count;
}
int message_length(char *input){
    int count = 0;
    while(*input != '\0'){
        input++;
        count++;
    }
    return count;
}

/* Pack the user input provided in input into the appropriate message
 * type in the space provided by packed.  You can assume that input is a
 * NUL-terminated string, and that packed is a buffer of size
 * PACKET_SIZE.
 *
 * Returns the packet type for valid input, or -1 for invalid input.
 */

int pack(void *packed, char *input) {
    int packet = -1;
    if(test_input(input) == false){
        return packet;
    }
    else{
    if(*input == '@'){
        packet = LABELED;
        int message = message_length_labelled(input);
        int target = target_length(input);
        if(target > NAME_SIZE || message > MAX_MESSAGE_SIZE){
            return -1;
        }
        *(int*)packed = LABELED;
        packed = packed + sizeof(int);
        for(int i = 0; i != NAME_SIZE; i++){
            if(i < 8){
                *(char*)packed = ubit[i];
            }else{
                *(char*)packed = '\0';
            }
            packed++;
        }
        *(size_t*)packed = message;
        packed=packed + sizeof(size_t);
        *(size_t*)packed = target;
        packed=packed + sizeof(size_t);
        *(size_t*)packed = 0;
        packed=packed + sizeof(size_t);
        char *input2 = input;
        while(*input != ' '){
            input++;
        }
        while(*input == ' '){
            input++;
        }
        while(*input != '\0'){
            *(char*)packed = *input;
            packed++;
            input++;
         }
        input2++;
        while(*input2 != ' '){
            *(char*)packed = *input2;
            input2++;
            packed++;
         }
        *(char*)packed = '\0';
    }
    else if(*input == '/'){
        int count1 = 0;
        char *input_count = input+1;
        char stats[]={'s','t','a','t','s'};
        for(int i=0; i != 5; i++){
            if(stats[i] == *input_count){
            count1++;
            }
            input_count++;
        }
        if(count1 == 5){
            packet = STATISTICS;
            *(int*)packed = STATISTICS;
            packed = packed + sizeof(int);
            for(int i = 0; i != NAME_SIZE; i++){
                if(i < 8){
                    *(char*)packed = ubit[i];
                }else{
                    *(char*)packed = '\0';
                }
                packed++;
            }
        }else{
            packet = STATUS;
            int message = message_length_labelled(input);
            *(int*)packed = STATUS;
            packed = packed + sizeof(int);
            for(int i = 0; i != NAME_SIZE; i++){
                if(i < 8){
                    *(char*)packed = ubit[i];
                }else{
                    *(char*)packed = '\0';
                }
                packed++;
            }
            *(size_t*)packed = message;
            packed = packed+ sizeof(size_t);
            *(size_t*)packed = 0;
            packed = packed+ sizeof(size_t);
            while(*input != ' '){
                input++;
            }
            while(*input == ' '){
                input++;
            }
            while(*input != '\0'){
                *(char*)packed = *input;
                packed++;
                input++;
            }
            *(char*)packed = '\0';
        }
    }
    else{
        packet = MESSAGE;
        int message = message_length(input);
        *(int*)packed = MESSAGE;
        packed = packed + sizeof(int);
        for(int i = 0; i != NAME_SIZE; i++){
            if(i < 8){
                *(char*)packed = ubit[i];
            }else{
                *(char*)packed = '\0';
            }
            packed++;
        }
        *(size_t*)packed = message;
        packed = packed+ sizeof(size_t);
        *(size_t*)packed = 0;
        packed = packed+ sizeof(size_t);
        while(*input != '\0'){
            *(char*)packed = *input;
            packed++;
            input++;
        }
        *(char*)packed = '\0';
    }
    }
    return packet;
}

/* Create a refresh packet for the given message ID.  You can assume
 * that packed is a buffer of size PACKET_SIZE.
 *
 * You should start by implementing this method!
 *
 * Returns the packet type.
 */

int pack_refresh(void *packed, int message_id) {
    *(int*)packed = REFRESH;
    packed = packed + sizeof(int);
    for(int i = 0; i != NAME_SIZE; i++){
        if(i < 8){
        *(char*)packed = ubit[i];
        }else{
            *(char*)packed = '\0';
        }
        packed++;
    }
    *(int*)packed = message_id;
    return REFRESH;
}
