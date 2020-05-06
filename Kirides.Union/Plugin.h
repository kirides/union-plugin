
// This file added in headers queue
// File: "Headers.h"
namespace GOTHIC_ENGINE {
  // Add your code here . . .
    void RegeneratePlayerMana_LoadSettings();
    void __fastcall RegeneratePlayerMana_ProcessNpc(oCNpc* _this);
    std::unique_ptr<CInvoke<void(__fastcall*)(oCNpc* _this)>> Ivk_oCNpc__ProcessNpc;
}