#pragma once

#include "Kengine/string/string-id.hxx"

namespace Kengine
{
    E_DECLSPEC void push_custom_event(string_id event_id,
                                      void*     data1,
                                      void*     data2);

}; // namespace Kengine
