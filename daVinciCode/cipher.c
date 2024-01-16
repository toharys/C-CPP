#include "cipher.h"
#include "string.h"

#define SML_A 97
#define SML_Z 122
#define BIG_A 65
#define BIG_Z 90
#define SGMA_LNGTH 26

/// IN THIS FILE, IMPLEMENT EVERY FUNCTION THAT'S DECLARED IN cipher.h.


// See full documentation in header file
/// a function that enables sycling sigma
void cycling (char s[], int i, int k, int b_b, int u_b){
    if ((s[i]+k)<b_b){
        k -= (b_b-s[i]);
        s[i] = u_b+(k+1);
    }
    else if (u_b<(s[i]+k)){
        k -= (u_b-s[i]);
        s[i] = b_b+(k-1);
    }
}

/// a function that receives a char and return if its upper/lower case or none
int char_identifier (char const s[], size_t i){
    if (SML_A <= s[i] && s[i] <= SML_Z) {   // checks if lower case
        return 1;
    }
    if (BIG_A <= s[i] && s[i] <= BIG_Z) { // checks if upper case
        return 2;
    }
    return 3; // char not belongs sigma
}

/// a function that return arr with the boarders of curr sigma subset
void get_boarders(int arr[2], size_t i, char s[]){
    int curr_state = char_identifier(s, i);
    if (curr_state == 1){   // enter lower case boarders
        arr[0] = SML_A;
        arr[1] = SML_Z;
    }
    else if (curr_state==2){    // enter upper case boarders
        arr[0] = BIG_A;
        arr[1] = BIG_Z;
    }
}

void encode (char s[], int k) {
    int k_tag = k%SGMA_LNGTH; // if k greater than one sigma cycle, calculates the steps number in one cycle
    for (size_t i = 0; i < strlen(s); i++) { //moves on each char
        int boarders[] = {0,0};
        get_boarders(boarders, i, s);   //gets sigma's subset boarders (upper\lower case or none)
        if (boarders[0]==boarders[1]){  // if char not belongs sigma
            continue;
        }
        if (boarders[0] <= (s[i] + k_tag) && (s[i] + k_tag) <= boarders[1]) {   // if not needed cycling
            s[i] += k_tag;
        }
        else {  // in case cycling needed
            cycling(s, i, k_tag, boarders[0], boarders[1]);
        }
    }
}

// See full documentation in header file
void decode (char s[], int k)
{
    int k_tag = (-k);
    encode(s, k_tag);
}
