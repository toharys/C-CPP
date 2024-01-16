#include "cipher.h"
#include "tests.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define T "test"
#define MAX_ROW_LENGTH 1024
#define TEN_BASE 10

///function do input checks and return 0 if the checks passed and error number if not
int input_checks(char *input_str[], int input_lngth) {

    if (input_lngth > 5 || input_lngth == 0) {
        return 1;
    }
    if ((input_lngth == 1) && (strcmp(input_str[1], "test") != 0)) {   //checks the case of one argument
        return 2;
    }
        if (input_lngth == 4) {
            if (strcmp(input_str[1], "encode") != 0 && strcmp(input_str[1], "decode") != 0) {  //checks the command
                return 3;
            }
            double k = strtod(input_str[2], NULL);
            if ((k - (int) k) != 0) {
                return 4;
            }
        }
        return 0;
    }

///function that print msgs
void error_display(int error) {
    if (error == 1) {
        fprintf(stderr, "The program receives 1 or 4 arguments only.\n");
    }
    if (error == 2) {
        fprintf(stderr, "Usage: cipher test\n");
    }
    if (error == 3) {
        fprintf(stderr, "The given command is invalid.\n");
    }
    if (error == 4) {
        fprintf(stderr, "The given shift value is invalid.\n");
    }
}


int main(int argc, char *argv[])
{
    int error = input_checks(argv, argc);
    if (error!=0) {
        error_display(error);
        return EXIT_FAILURE;
    }
    else if (strcmp(argv[1], T) == 0) {
        if (test_encode_non_cyclic_lower_case_positive_k() ||
        test_encode_cyclic_lower_case_special_char_positive_k() ||
        test_encode_non_cyclic_lower_case_special_char_negative_k() ||
        test_encode_cyclic_lower_case_negative_k() ||
        test_encode_cyclic_upper_case_positive_k() || test_decode_non_cyclic_lower_case_positive_k() ||
        test_decode_cyclic_lower_case_special_char_positive_k() ||
        test_decode_non_cyclic_lower_case_special_char_negative_k() ||
        test_decode_cyclic_lower_case_negative_k() || test_decode_cyclic_upper_case_positive_k()) {
            return EXIT_FAILURE;
        } else {
            return EXIT_SUCCESS;
        }
    } else {   // if there arnt errors and the command isnt test
        FILE *read_f;
        read_f = fopen(argv[3], "r");
        if (!read_f) {

            fprintf(stderr, "The given file is invalid.\n");
            return EXIT_FAILURE;
        } else {
            FILE *write_f;
            write_f = fopen(argv[4], "w");
            long k = strtol(argv[2], NULL, TEN_BASE);
            char temp[MAX_ROW_LENGTH];
            if (strcmp(argv[1], "encode")==0) {
                while (fgets(temp, MAX_ROW_LENGTH, read_f) != NULL) {
                    encode(temp, (int)k);
                    fprintf(write_f, MAX_ROW_LENGTH, temp);
                }
            } else if (strcmp(argv[1], "decode")==0) {
                while (fgets(temp, MAX_ROW_LENGTH, read_f) != NULL) {
                    decode(temp, (int)k);
                    fprintf(write_f, MAX_ROW_LENGTH, temp);
                }
            }
            fclose(read_f);
            fclose(write_f);
            return EXIT_SUCCESS;
        }
    }
}








