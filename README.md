# Dependencies

- Gothic 1 / Gothic 2 (NotR)
- tested/built with Union 1.0h

# Gothic.INI

```ini
[UNION_KIRIDES]
; Enables ManaReg
bManaReg=1
; Enables QuickLoot
bQuickLoot=1
[UNION_KIRIDES::MANAREG]
; Amount of regeneration in %, or flat if `bFlat=1`
iValue=2
; `bFlat=0` iValue is percentage, else flat amount.
bFlat=0
; Rate of regeneration
iTickRate=2000
```

# How the code is structured

Plugins are registered with the `REGISTER_PLUGIN` macro.

It takes in the Namespace that lies inside the current `GOTHIC_ENGINE` namespace and requires a `void Init()` and a `void Destroy()` function to be present in it.

- `Init()` is called in `GOTHIC_ENGINE::Game_Init()` and ususally does all the `Gothic.ini` and Hooking-buisness. ([example](/Kirides.Union/ManaReg.h))
- `Destroy()` is called in `GOTHIC_ENGINE::Game_Exit()` and cleans up all the used references, unloads hooks (typically not necessery), etc.

## Example

```c++
REGISTER_PLUGIN(ManaReg);
// expands to: { GOTHIC_ENGINE::ManaReg::Init(); cleanup.push_back(GOTHIC_ENGINE::ManaReg::Destroy); }
```

## Example Plugins

PluginX.h
```cpp
namespace PluginX {
    using namespace GOTHIC_ENGINE;

    void Init() {
        Gothic::Game::Session->array_view[0]->PrintCX(200, "Loaded PluginX");
    }

    void Destroy() { }
}
```
Plugin.h
```cpp
#define PLUGIN_INI_MAIN "UNION_MyPlugin"

namespace GOTHIC_ENGINE {
#include "PluginX.h"
}
```
Plugin.cpp
```cpp
// ... omitted
namespace GOTHIC_ENGINE {

    // ... omitted

    void Game_Init() {
        if (Gothic::Options::Gothic->ReadBool(PLUGIN_INI_MAIN /* from Plugin.h */, "bPluginX", true))
            REGISTER_PLUGIN(PluginX);
    }
	
    // ... omitted
}
```