#include "../analysis.h"
#include <stdio.h>
#include <string.h>
#include <assert.h>

int test(char* input){
    int result;
    FILE *file = fmemopen(input, strlen(input), "r");
    set_source_file(file);
    analyse_data_t *data = malloc(sizeof(analyse_data_t));
    
    if (!init_variables(data))
        return INTERNAL_ERROR;

    result = get_next_token(&data->token);

    if (data->token.type == TOKEN_EOF)
        return -1;
    free_variables(data);
    fclose(file);

    result = program(data);
}

void func_test()
{
    char *input = "func decrement(of n: Int, by m: Int) -> Int {\nreturn n - m\n}";
    assert(test(input) == 0);
}


void var_test()
{
    char *input = "var a: Int = 5";
    assert(test(input) == 0);
    char *input = "var a: Double = 5.5";
    assert(test(input) == 0);
    char *input = "var a: String = \"test\"";
    assert(test(input) == 0);
    input = "var a: Int = 5\nvar b: Int = 5";
    assert(test(input) == 0);
    input = "var a: Int = 5\nvar b: Int = 5\nvar c: Int = 5";
    assert(test(input) == 0);
    input = "var a: Int = 5\n a = 4\n let c = 4\n a = c + 3";
       assert(test(input) == 0);

}

void constant_test() {
    char *input = "let constant: Int = 10";
    assert(test(input) == 0);
    input = "let a: Int = 5\n var a: Int = 5";
    assert(test(input) != 0);
    input = "let a: Int = 5\na = 5";
    assert(test(input) != 0);
    input = "var a = 5";
    input = "let constant: Double = 10.5";
    assert(test(input) == 0);
    input = "let constant: String = \"constant\"";
    assert(test(input) == 0);
}

void func_with_multiple_params_test() {
    char *input = "func add(a: Int, b: Int) -> Int {\nreturn a + b\n}";
    assert(test(input) == 0);
    input = "func concatenate(a: String, b: String) -> String {\nreturn a + b\n}";
    assert(test(input) == 0);
    input = "func multiply(a: Double, b: Double) -> Double {\nreturn a * b\n}";
    assert(test(input) == 0);
    input = "func concat(_ x : String, with y : String) -> String {\nlet x = x + y\nreturn x + \" \" + y\n}\nlet a = \"ahoj \"\nvar ct : String\nct = concat(a, with: \"svete\")";
    assert(test(input) == 0);
    input = "let c : Int = 4\nfunc empty(){\n}\nfunc concat(b x : String, with y : String) -> String {\nlet x = y + y\nwhile (c > 3) {\nvar x : Double\n}\nreturn x + \" \" + y\n}\nlet a = \"ahoj \"\nvar ct : String\nconcat(b: a, with: \"svete\")\nempty()\n";
    assert(test(input) == 0);
    input = "let c: Int\nfunc empty(){\n\n}\nfunc concat(b x : String, with y : String) -> String {\n    let x = y + y\n    if (c > 3) {\n        var x : Double\n    }else{\n        var x: String\n    }\n    return x + \" \" + y\n}";
    assert(test(input) == 0);
}
void build_in_funcs_test() {
    char *input = "readInt()\nreadString()\nreadDouble()\nInt2Double(34)\nDouble2Int(34.3)\nlength(\"test\")\nord(\"test\")\nchr(12)\nsubstring(of: \"Something\", startingAt: 1, endingBefore: 12)\n";
    assert(test(input) == 0);
}

int main() {
    func_with_multiple_params_test();
    build_in_funcs_test();
    constant_test();
    var_test();
    func_test();
    return 0;
}