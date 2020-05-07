#include <functional>
#include <vector>

#define IS_G1 (ENGINE == Engine_G1 || ENGINE == Engine_G1A)
#define PLUGIN_INI_MAIN "UNION_KIRIDES"
#define PLUGIN_INI_SECTION(x) ("UNION_KIRIDES::" x) 

// This file added in headers queue
// File: "Headers.h"
namespace GOTHIC_ENGINE {
	// Add your code here . . .
#include "ManaReg.h"
#include "Quickloot.h"
}