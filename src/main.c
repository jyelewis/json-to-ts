#include <stdlib.h>
#include "types.h"
#include "io.h"
#include "json_to_type.h"
#include "print_type_as_ts.h"

int main() {
    // read input from stdin
     char* input_json = load_from_stdin();

    // parse as ts types
    ts_type* type = json_to_type(input_json);

    // output as valid TypeScript code to stdout
    print_type_as_ts(type);

    // release meee
    free_ts_type(type);
    free(input_json);

    return 0;
}
