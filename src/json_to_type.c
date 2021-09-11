#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "json_to_type.h"
#include "merge_types.h"

char *skip_whitespace(char *json);

char *json_to_unknown_type(char *json, ts_type *type);

char *json_to_string_type(char *json, ts_type *type);

char *json_to_number_type(char *json, ts_type *type);

char *json_to_null_type(char *json, ts_type *type);

char *json_to_true_type(char *json, ts_type *type);

char *json_to_false_type(char *json, ts_type *type);

char *json_to_object_type(char *json, ts_type *type);

char *json_to_array_type(char *json, ts_type *type);

// public contract
ts_type *json_to_type(char *json) {
    ts_type *type = allocate_type();
    json_to_unknown_type(json, type);
    return type;
}

char *json_to_unknown_type(char *json, ts_type *type) {
    // check what type of data we are dealing with based on the first character
    json = skip_whitespace(json);
    switch (json[0]) {
        case '{':
            return json_to_object_type(json, type);
        case '[':
            return json_to_array_type(json, type);
        case '"':
            return json_to_string_type(json, type);
        case 'n':
            return json_to_null_type(json, type);
        case 't':
            return json_to_true_type(json, type);
        case 'f':
            return json_to_false_type(json, type);
        default:
            if (json[0] >= '0' && json[0] <= '9') {
                return json_to_number_type(json, type);
            }

            perror("Error parsing JSON");
            exit(1);
    }
}

// util
ts_type *allocate_type() {
    // calloc so we get a zeroed object
    ts_type *type = calloc(1, sizeof(ts_type));
    if (type == NULL) {
        perror("Error allocating");
        exit(1);
    }
    return type;
}

char *skip_whitespace(char *json) {
    while (isspace(*json)) {
        json++;
    }

    return json;
}

// type parsers
char *json_to_string_type(char *json, ts_type *type) {
    type->possible_types = PT_STRING;

    // move past the open " mark
    json++;

    // move forward until we reach the end of the string
    while (json[0] != '"') {
        if (json[0] == '\\') {
            // skip the next character due to backslash escape
            json++;
        }
        json++;
    }

    // move past the close " mark
    json++;

    return json;
}

char *json_to_number_type(char *json, ts_type *type) {
    type->possible_types = PT_NUMBER;

    // move forward until we reach the end of the number
    // allow decimals in numbers
    while ((json[0] >= '0' && json[0] <= '9') || json[0] == '.') {
        json++;
    }

    return json;
}

char *json_to_null_type(char *json, ts_type *type) {
    type->possible_types = PT_NULL;
    return json + 4; // move past "null"
}

char *json_to_true_type(char *json, ts_type *type) {
    type->possible_types = PT_BOOLEAN;
    return json + 4; // move past "true"
}

char *json_to_false_type(char *json, ts_type *type) {
    type->possible_types = PT_BOOLEAN;
    return json + 5; // move past "false"
}

char *json_to_object_type(char *json, ts_type *type) {
    type->possible_types = PT_OBJECT;

    json++; // move past '{'
    json = skip_whitespace(json);

    object_property* last_property = NULL;

    // read properties from object until we reach a close brace
    while (json[0] != '}') {
        // valid JSON check, we should always start with a property key
        if (json[0] != '"') {
            // wot
            printf("%s\n", json);
            perror("Error parsing JSON (expected '\"' at start of object key)");
            exit(1);
        }

        // create new property
        object_property *new_property = malloc(sizeof(object_property));
        new_property->next = NULL;
        new_property->type = allocate_type();

        // read key string
        json++; // move past '"'
        char *start_of_key = json;
        while (json[0] != '"') {
            json++; // move through key
        }
        size_t key_length = json - start_of_key;

        json++; // move past '"'
        json = skip_whitespace(json);
        json++; // move past ':'
        json = skip_whitespace(json);

        // copy key into new property
        new_property->key = malloc(sizeof(char) * (key_length + 1));
        strncpy(new_property->key, start_of_key, key_length);
        new_property->key[key_length] = '\0';

        // store this property in linked list of properties
        if (last_property == NULL) {
            type->object_properties = new_property;
        } else {
            last_property->next = new_property;
        }
        last_property = new_property;

        // auto detect type of property value
        json = json_to_unknown_type(json, new_property->type);

        // move to next property, skipping comma
        json = skip_whitespace(json);
        if (json[0] == ',') {
            json++; // skip comma
            json = skip_whitespace(json);
        }
    }

    json++; // move past '}'

    // end of object, return
    return json;
}

char *json_to_array_type(char *json, ts_type *type) {
    type->possible_types = PT_ARRAY;

    ts_type array_item_type = {0};

    json++; // move past [
    json = skip_whitespace(json);

    while (json[0] != ']') {
        // only allocate when we actually read our first value
        // no need if this is an empty array
        if (type->array_type == NULL) {
            type->array_type = allocate_type();
        }

        json = json_to_unknown_type(json, &array_item_type);

        // merge type from this object with existing type for array
        merge_types(type->array_type, &array_item_type);

        // move to next property, skipping comma
        json = skip_whitespace(json);
        if (json[0] == ',') {
            json++; // skip comma
            json = skip_whitespace(json);
        }
    }

    json++; // move past ']'

    return json;
}