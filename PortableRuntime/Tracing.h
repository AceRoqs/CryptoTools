#pragma once

namespace PortableRuntime
{

typedef void(*Output_dprintf)(_In_z_ const char* format) NOEXCEPT;

void dprintf(_In_z_ const char* format, ...) NOEXCEPT;

// TODO: Make this a class, which can restore the previous dprintf on scope exit?
void set_dprintf(const Output_dprintf output_function);

}

