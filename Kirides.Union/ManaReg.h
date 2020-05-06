#include <Union.h>
#include "Plugin.h"

namespace GOTHIC_ENGINE {

    class ManaReg : public IPlugin {
        CInvoke<void(__fastcall*)(oCNpc* _this)> Ivk_oCNpc__ProcessNpc;
        virtual void Init();
        void __fastcall RegeneratePlayerMana_ProcessNpc(oCNpc* _this);
    };
    // Add your code here . . .
    extern void RegeneratePlayerMana_LoadSettings();
}
