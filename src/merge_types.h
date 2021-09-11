#pragma once

#include "types.h"

// will mutate base_type to be assignable to second_type
void merge_types(ts_type* base_type, ts_type* second_type);

// will mutate base_object_properties to be assignable to second_object_properties
void merge_object_properties(object_property* base_object_properties, object_property* second_object_properties);
