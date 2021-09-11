#include "merge_types.h"
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// will mutate base_type to be assignable to second_type
void merge_types(ts_type *base_type, ts_type *second_type) {
    if (base_type == NULL) {
        perror("base_type cannot be NULL when merging types");
        exit(1);
    }
    if (second_type == NULL) {
        // no second type, base_type is already compatible
        return;
    }

    // base type can now be any of its original possible_types, plus anything second_type could be
    base_type->possible_types = base_type->possible_types | second_type->possible_types;

    // perform for nested array_type
    if (base_type->array_type == NULL) {
        base_type->array_type = second_type->array_type;
    } else {
        merge_types(base_type->array_type, second_type->array_type);
    }

    if (second_type->object_properties == NULL) {
        // no object_properties on second_type, nothing to do!
        return;
    }

    if (base_type->object_properties == NULL) {
        // only second_type has properties, copy them across
        base_type->object_properties = second_type->object_properties;
        return;
    }

    // both objects have different properties, merge them
    merge_object_properties(base_type->object_properties, second_type->object_properties);
}

void merge_object_properties(object_property *base_object_properties, object_property *second_object_properties) {
    // iterate all base properties, look for a corresponding property on second_object_properties
    // if a corresponding property is found, merge the types & update
    object_property *current_base_property = base_object_properties;
    do {
        object_property *matching_second_property = find_object_property(second_object_properties,
                                                                         current_base_property->key);
        if (matching_second_property != NULL) {
            // property exists on both base_object and second_object
            // update base_object with merged version
            merge_types(current_base_property->type, matching_second_property->type);
        } else {
            // property does not exist on second object, mark as possibly undefined
            current_base_property->type->possible_types |= PT_UNDEFINED;
        }

        if (current_base_property->next == NULL) {
            break;
        }
        current_base_property = current_base_property->next;
    } while (1);

    // finally, iterate second_object_properties looking for any properties that don't exist on base_object
    object_property *current_second_property = second_object_properties;
    while (current_second_property != NULL) {
        object_property *matching_base_property = find_object_property(base_object_properties,
                                                                       current_second_property->key);
        if (matching_base_property == NULL) {
            // current_second_property doesn't exist on base property type, append

            // create new property on the heap
            object_property *new_property;
            new_property = malloc(sizeof(object_property));
            new_property->key = malloc(sizeof(char) * (strlen(current_second_property->key) + 1));
            strcpy(new_property->key, current_second_property->key);
            new_property->type = current_second_property->type;
            // mark this newly added property as possibly undefined, as it wasn't present in base_object
            new_property->type->possible_types |= PT_UNDEFINED;
            new_property->next = NULL;

            // append to the existing linked list
            current_base_property->next = new_property;
            current_base_property = new_property;
        }

        current_second_property = current_second_property->next;
    }
}