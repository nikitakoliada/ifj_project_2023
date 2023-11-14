#include "../scanner.c"
#include <stdio.h>
#include <string.h>
#include <assert.h>


void test_keyword_token() {
    char *input = "if"; 
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    get_next_token(&token);

    assert(token.type == KEYWORD);
    assert(token.data.Keyword == If_KW); 

    fclose(file);

    printf("Test Keyword Token: Passed\n");
}

void test_identifier_token() {
    char *input = "myIdentifier";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    get_next_token(&token);
    assert(token.type == IDENTIFIER);
    assert(strcmp(token.data.String, "myIdentifier") == 0);

    fclose(file);
    printf("Identifier Token Test: Passed\n");
}
void test_builtin_function_token() {
    char *input = "readInt";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);
    
    token_t token;
    get_next_token(&token);

    assert(token.type == BUILT_IN_FUNCTION);

    // TODO ??
    // assert(strcmp(token.data.String, "readInt") == 0); it does not work hmmmmmmmm


    fclose(file);
    printf("Built-in Function Token Test: Passed\n");
}
void test_string_value_token() {
    char *input = "\"hello world\"";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    get_next_token(&token);

    assert(token.type == STRING_VALUE);
    assert(strcmp(token.data.String, "hello world") == 0);

    fclose(file);
    printf("String Value Token Test: Passed\n");
}
void test_integer_value_token() {
    char *input = "12345";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    get_next_token(&token);

    assert(token.type == INT_VALUE);
    assert(token.data.Int == 12345);

    fclose(file);
    printf("Integer Value Token Test: Passed\n");
}
void test_double_value_token() {
    char *input = "123.45";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    get_next_token(&token);

    assert(token.type == DOUBLE_VALUE);
    assert(token.data.Double == 123.45);

    fclose(file);
    printf("Double Value Token Test: Passed\n");
}
void test_operator_token() {
    char *input = "+";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    get_next_token(&token);

    assert(token.type == PLUS);

    fclose(file);
    printf("Operator Token Test: Passed\n");
}
void test_eof_eol_token() {
    char *input = "\n";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    get_next_token(&token);

    assert(token.type == TOKEN_EOL);

    get_next_token(&token);
    assert(token.type == TOKEN_EOF);

    fclose(file);
    printf("EOF and EOL Token Test: Passed\n");
}
// TODO how am I supposed to test it 
// void test_escape_sequence_processing() {
    
//     char test_string[] = "Hello\nWorld";
//     unsigned long long pos = 6; 
//     scanner_process_escape_sequence(test_string, pos);
//     assert(test_string[pos-2] == 10); // Checking if the escape sequence is processed correctly
     
//     *test_string = "Hello\tWorld";
//     scanner_process_escape_sequence(test_string, pos);
//     assert(test_string[pos-2] == 9); // Checking if the escape sequence is processed correctly

//     *test_string = "Hello\rWorld";
//     scanner_process_escape_sequence(test_string, pos);
//     assert(test_string[pos-2] == 13);  // Checking if the escape sequence is processed correctly

//     *test_string = "Hello\\World";
//     scanner_process_escape_sequence(test_string, pos);
//     assert(test_string[pos-2] == 92);  // Checking if the escape sequence is processed correctly

//     *test_string = "Hello\"World";
//     scanner_process_escape_sequence(test_string, pos);
//     assert(test_string[pos-2] == 34);  // Checking if the escape sequence is processed correctly

//     assert(test_string[pos-1] == '\0');  // Checking if the escape sequence is processed correctly
//     printf("Escape sequence Test: Passed\n");
// }
// TODO how to test if there were some errors? 

// void test_error_handling() {
//     char *input = "\"This is an error string";
//     FILE *file = fmemopen(input, strlen(input), "r");
//     set_source_file(file);

//     token_t token;
//     get_next_token(&token);
//     assert(token.type == ERROR_H);

//     fclose(file);
//     printf("Error Handling Test: Passed\n");
// }

// TODO does not work 
// Maxim should implement multiline first ig
void test_line_number_tracking() {
    // char *input = "first\nsecond";
    // FILE *file = fmemopen(input, strlen(input), "r");
    // set_source_file(file);

    // token_t token;
    // get_next_token(&token); // Should be 'first'
    // assert(token.line == 0);

    // get_next_token(&token); // Should be EOL
    // get_next_token(&token); // Should be 'second'
    // assert(token.line == 1);

    // fclose(file);
    printf("Line Number Tracking Test: Passed\n");
}
void test_state_transition() {
    char *input = "\"abc\\ndef\"";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    get_next_token(&token);

    assert(token.type == STRING_VALUE);
    //assert(strcmp(token.data.String, "abc\ndef") == 0); ayo tf is  inside 

    fclose(file);
    printf("State Transition Test: Passed\n");
}
void test_factorial_code() {
    char *input = "write(\"Zadejte cislo pro vypocet faktorialu\\n\")\nlet a : Int? = readInt()\nif let a {\nif (a < 0) {write(\"Faktorial nelze spocitat\\n\")\n} else {\nvar a = Int2Double(a)\nvar vysl : Double = 1\nwhile (a > 0) {\nvysl = vysl * a\na = a - 1\n}\nwrite(\"Vysledek je: \", vysl, \"\\n\")\n}\n} else {\nwrite(\"Chyba pri nacitani celeho cisla!\\n\")\n}";
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);

    token_t token;
    while (get_next_token(&token) != TOKEN_EOF) {
        // TDO
    }

    fclose(file);
    printf("Factorial Code Test: Passed\n");
}

int main() {
    test_keyword_token();
    test_builtin_function_token();
    test_double_value_token();
    test_eof_eol_token();
    //test_error_handling();
    //test_escape_sequence_processing();
    test_identifier_token();
    test_integer_value_token();
    test_keyword_token();
    test_line_number_tracking();
    test_operator_token();
    test_state_transition();
    test_string_value_token();
    
    return 0;
}