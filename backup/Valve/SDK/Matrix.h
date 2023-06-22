#pragma once
#include "Vector3.h"
#include "../Engine.h"
#include "../../Header Files/Hazedumper.h"
#include "../../Header Files/memory.hpp"

namespace SDK
{
	matrix_t get_view_matrix()
	{
		return mem.read<matrix_t>(g_client_base + hazedumper::signatures::dwViewMatrix);
	}

}