#include <printf.h>
#include "print_type_as_ts.h"

#define INDENT_SPACES 2

// private methods
void print_type(ts_type* type, bool ignoreUndefined, int indent_size);

// util
void indent(int indent_size) {
    for (int i = 0; i < indent_size; i++) {
        printf(" ");
    }
}

// public
void print_type_as_ts(ts_type* type) {
    printf("export type JsonData = ");
    print_type(type, false, 0);
    printf(";");
}

void print_type(ts_type* type, bool ignoreUndefined, int indent_size) {
    bool orRequired = false;

    if (ts_type_can_be_undefined(type) && !ignoreUndefined) {
        printf("undefined");
        orRequired = true;
    }

    if (ts_type_can_be_null(type)) {
        if (orRequired) {
            printf(" | ");
        }
        printf("null");
        orRequired = true;
    }

    if (ts_type_can_be_boolean(type)) {
        if (orRequired) {
            printf(" | ");
        }
        printf("boolean");
        orRequired = true;
    }

    if (ts_type_can_be_number(type)) {
        if (orRequired) {
            printf(" | ");
        }
        printf("number");
        orRequired = true;
    }

    if (ts_type_can_be_string(type)) {
        if (orRequired) {
            printf(" | ");
        }
        printf("string");
        orRequired = true;
    }

    if (ts_type_can_be_array(type)) {
        if (orRequired) {
            printf(" | ");
        }

        if (type->array_type == NULL) {
            // empty array
            printf("[]");
        } else {
            // not empty array, specify type
            // prefer simple syntax, if reasonable
            if (possible_type_is_complex(type->array_type->possible_types) || ts_type_can_be_object(type->array_type)) {
                printf("Array<");
                print_type(type->array_type, false, indent_size);
                printf(">");
            } else {
                print_type(type->array_type, false, indent_size);
                printf("[]");
            }
        }

        orRequired = true;
    }

    if (ts_type_can_be_object(type)) {
        if (orRequired) {
            printf(" | ");
        }

        if (type->object_properties == NULL) {
            // empty object
            printf("{}");
        } else {
            printf("{");

            object_property* prop = type->object_properties;
            while (prop != NULL) {
                printf("\n");
                indent(indent_size + INDENT_SPACES);

                if (ts_type_can_be_undefined(prop->type)) {
                    printf("%s?: ", prop->key);
                } else {
                    printf("%s: ", prop->key);
                }
                print_type(prop->type, true, indent_size + INDENT_SPACES);
                printf(";");

                prop = prop->next;
            }

            printf("\n");
            indent(indent_size);

            printf("}");
        }
    }
}
