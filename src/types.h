#pragma once

#include <stdbool.h>

typedef unsigned char possible_types;

enum primitive_type {
    PT_STRING =     0b11000000,
    PT_NUMBER =     0b10100000,
    PT_BOOLEAN =    0b10010000,
    PT_NULL =       0b10001000,
    PT_UNDEFINED =  0b10000100,
    PT_ARRAY =      0b10000010,
    PT_OBJECT =     0b10000001,

    PT_VALID_BIT =  0b10000000,
    PT_NONE =       0b00000000
};

typedef struct object_property object_property;
typedef struct ts_type ts_type;

// linked list of all properties for an object
struct object_property {
    char* key;
    ts_type* type;
    object_property* next; // ptr to next property, or NULL if end
};

struct ts_type {
    possible_types possible_types;      // bit field of primitive_type
    ts_type* array_type;                // sub type for elements within this, if an array
    object_property* object_properties; // NULL if no properties
};

// helper methods
bool ts_type_is_valid(ts_type* type);
bool ts_type_can_be_string(ts_type* type);
bool ts_type_can_be_number(ts_type* type);
bool ts_type_can_be_boolean(ts_type* type);
bool ts_type_can_be_null(ts_type* type);
bool ts_type_can_be_undefined(ts_type* type);
bool ts_type_can_be_array(ts_type* type);
bool ts_type_can_be_object(ts_type* type);

bool possible_type_is_complex(possible_types possible_types);

object_property* find_object_property(object_property* object_properties, char* key);

void free_object_property(object_property* object_property);
void free_ts_type(ts_type* type);
