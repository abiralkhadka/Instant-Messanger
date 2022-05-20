#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "serialize.h"

/* Unpack the given packet into the buffer unpacked.  You can assume
 * that packed points to a packet buffer large enough to hold the
 * packet described therein, but you cannot assume that the packet is
 * otherwise valid.  You can assume that unpacked points to a character
 * buffer large enough to store the unpacked packet, if it is a valid
 * packet.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */
int unpack(char *unpacked, void *packed) {
    int packet_type=-1;
    if(*(int*)packed == MESSAGE){             //MESSAGE
        packet_type = MESSAGE;
        packed = packed+sizeof(int);
        void* packed1 = packed;
        while( *(char*)packed1 != '\0'){   //output to ubit name
            *unpacked= *(char*)packed1;
            packed1++;
            unpacked++;
        }
        packed=packed + NAME_SIZE;
        int message = *(int*)packed;
        packed=packed + sizeof(size_t);
        int terminator = *(size_t*)packed;
        packed=packed + sizeof(size_t);
        *unpacked= ':';                    //output ':'
        unpacked++;
        *unpacked= ' ';                     // output ' '
        unpacked++;
        int count = 0;
        while(*(char*)packed != '\0'){      //output message
            *unpacked = *(char*)packed;
            packed++;
            unpacked++;
            count++;
        }
        if(count > MAX_MESSAGE_SIZE|| terminator != 0 || message == 0 ||count ==0|| message > MAX_MESSAGE_SIZE ){ //check for invalid message
            packet_type = -1;
        }
    }
    else if(*(int*)packed == LABELED){               //LABELLED
        packet_type = LABELED;
        packed = packed+sizeof(int);
        void* packed1 = packed;
        while( *(char*)packed1 != '\0'){    //output the ubit
            *unpacked=*(char*)packed1;
            packed1++;
            unpacked++;
        }
        packed=packed + NAME_SIZE;
        int message = *(size_t*)packed;
        packed=packed+ sizeof(size_t);
        int target = *(size_t*)packed;
        packed=packed+ sizeof(size_t);
        int terminator = *(size_t*)packed;
        packed=packed + sizeof(size_t);
        *unpacked= ':';    //output the :
        unpacked++;
        *unpacked= ' ';      //output the ' '
        unpacked++;
        *unpacked = '@';     //output the @
        unpacked++;
        void *packed2= packed + message;
        int count1 = 0;
        for(int i=0 ; i != target; i++){ // output target char
            *unpacked =*(char*)packed2;
            packed2++;
            unpacked++;
            count1++;
        }
        *unpacked = ' ';       //output ' '
        unpacked++;
        int count2=0;
        for(int i=0;i != message; i++){     //output the message
            *unpacked =*(char*)packed;
            packed++;
            unpacked++;
            count2++;
        }
        if( terminator != 0  || target > MAX_MESSAGE_SIZE|| message == 0 ||target == 0 ||count1==0|| count2 ==0||message > MAX_MESSAGE_SIZE ){
            packet_type = -1;
        }
    }
    else if(*(int*)packed == STATUS){           //STATUS
        packet_type= STATUS;
        packed = packed+sizeof(int);
        void* packed1 = packed;
        while( *(char*)packed1 != '\0'){    //output the ubit
            *unpacked=*(char*)packed1;
            packed1++;
            unpacked++;
        }
        packed=packed + NAME_SIZE;
        int message = *(size_t*)packed;
        packed=packed + sizeof(size_t);
        int terminator = *(size_t*)packed;
        packed=packed + sizeof(size_t);
        *unpacked= ' ';   //output the space ' '
        unpacked++;
        int count = 0;
        while(*(char*)packed != '\0'){   //output the message
            *unpacked =*(char*)packed;
            packed++;
            unpacked++;
            count++;
        }
        if(count > MAX_MESSAGE_SIZE|| terminator != 0 || message == 0 ||count==0|| message > MAX_MESSAGE_SIZE ){ //check for invalid message
            packet_type = -1;
        }
    }
    *unpacked = '\0';
    return packet_type;
}

/* Unpack the given packed packet into the given statistics structure.
 * You can assume that packed points to a packet buffer large enough to
 * hold the statistics packet, but you cannot assume that it is
 * otherwise valid.  You can assume that statistics points to a
 * statistics structure.
 *
 * Returns the packet type that was unpacked, or -1 if it is invalid.
 */

int unpack_statistics(struct statistics *statistics, void *packed) {
    int packet_type= -1;
    int message = 0;
    int active = 0;
    if(*(int*)packed == STATISTICS){
        packet_type = STATISTICS;
        packed = packed+ sizeof(int);
        void *packed1 = packed; //pointer to unpack the ubit name
        for(int i=0 ; *(char*)packed1 != '\0' ; i++){        //pack sender ubit
            statistics->sender[i] =*(char*)packed1;
            packed1++;
            message++;
        }
        statistics->sender[NAME_SIZE] = '\0';
        packed=packed+ NAME_SIZE;
        void *packed2 = packed;
        for(int i=0 ; *(char*)packed2 != '\0'; i++){        //pack sender ubit
            statistics->most_active[i] =*(char*)packed2;
            packed2++;
            active++;
        }
        statistics->most_active[NAME_SIZE] = '\0';
        packed=packed+NAME_SIZE;
        statistics->most_active_count = *(int*)packed;
        packed=packed+sizeof(int);
        statistics->invalid_count = *(long*)packed;
        packed=packed+sizeof(long);
        statistics->refresh_count = *(long*)packed;
        packed=packed+sizeof(long);
        statistics->messages_count = *(int*)packed;
    }
    return packet_type;
}
