#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include "types.h"

// bit field helpers
bool ts_type_is_valid(ts_type* type) {
    return (type->possible_types & PT_VALID_BIT) == PT_VALID_BIT;
}

bool ts_type_can_be_string(ts_type* type) {
    return (type->possible_types & PT_STRING) == PT_STRING;
}

bool ts_type_can_be_number(ts_type* type) {
    return (type->possible_types & PT_NUMBER) == PT_NUMBER;
}

bool ts_type_can_be_boolean(ts_type* type) {
    return (type->possible_types & PT_BOOLEAN) == PT_BOOLEAN;
}

bool ts_type_can_be_null(ts_type* type) {
    return (type->possible_types & PT_NULL) == PT_NULL;
}

bool ts_type_can_be_undefined(ts_type* type) {
    return (type->possible_types & PT_UNDEFINED) == PT_UNDEFINED;
}

bool ts_type_can_be_array(ts_type* type) {
    return (type->possible_types & PT_ARRAY) == PT_ARRAY;
}

bool ts_type_can_be_object(ts_type* type) {
    return (type->possible_types & PT_OBJECT) == PT_OBJECT;
}

bool possible_type_is_complex(possible_types possible_types) {
    // a type is complex if it consists of a type union
    // (more than 1 byte, other than the 'valid' byte is set)

    possible_types ^= PT_VALID_BIT; // disable PT_VALID_BIT

    int num_possible_types = 0;
    for (char i = 0; i < 9; i++) {
        if ((possible_types & 1) == 1) {
            num_possible_types++;
        }
        possible_types = possible_types >> 1;
    }

    return num_possible_types > 1;
}

// object property linked list helpers
object_property* find_object_property(object_property* object_property, char* key) {
    // walk the linked list looking for a property that matches key
    while (object_property != NULL) {
        if (strcmp(object_property->key, key) == 0) {
            // found it!
            return object_property;
        }

        // not found, check next property
        object_property = object_property->next;
    }

    // no property found with this key :(
    return NULL;
}

void free_object_property(object_property* object_property) {
    if (object_property->next != NULL) {
        free_object_property(object_property->next);
    }
    free_ts_type(object_property->type);
    free(object_property);
}

void free_ts_type(ts_type* type) {
    if (type->object_properties != NULL) {
        free_object_property(type->object_properties);
    }
    if (type->array_type != NULL) {
        free_ts_type(type->array_type);
    }

    free(type);
}
