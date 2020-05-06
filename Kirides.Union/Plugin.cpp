// This file added in headers queue
// File: "Sources.h"
namespace GOTHIC_ENGINE {

    static int g_regRate = 2;
    static float g_regTickRate = 2000.0f;

    void __fastcall RegeneratePlayerMana_ProcessNpc(oCNpc* _this) {
        static CTimer TimerAI;
        static const uint RegenManaID = 1;

        if (_this == player) {
            TimerAI.Suspend(RegenManaID, ogame->singleStep);
            if (_this->attribute[NPC_ATR_MANA] < _this->attribute[NPC_ATR_MANAMAX]) {
                int menge; menge = (_this->attribute[NPC_ATR_MANAMAX] * g_regRate) / 100;

                int ManaIntensity = (g_regTickRate / max(menge, 1));
                if (TimerAI(RegenManaID, ManaIntensity)) {
                    _this->attribute[NPC_ATR_MANA]++;
                }
            }
            TimerAI.Attach();
        }
        // Invoke the "function pointer"
        (*Ivk_oCNpc__ProcessNpc)(_this);
    }

    void RegeneratePlayerMana_LoadSettings() {
        TCHAR NPath[MAX_PATH];
        // Returns Gothic directory.
        int len = GetCurrentDirectory(MAX_PATH, NPath);
        // Get path to Gothic.Ini
        auto ini = std::string(NPath, len).append("\\system\\Gothic.ini");

        g_regRate = GetPrivateProfileInt("UNION_MANAREG", "iVALUE", 2, ini.c_str());
        g_regTickRate = GetPrivateProfileInt("UNION_MANAREG", "iTICKRATE", 2000, ini.c_str());
        Ivk_oCNpc__ProcessNpc = std::make_unique<CInvoke<void(__fastcall*)(oCNpc* _this)>>(vftable_oCNpc::GetTable().names.f74_ProcessNpc, &RegeneratePlayerMana_ProcessNpc);
    }

  void Game_Entry() {
  }

  void Game_Init() {
      RegeneratePlayerMana_LoadSettings();
  }

  void Game_Exit() {
      Ivk_oCNpc__ProcessNpc.reset();
  }

  void Game_Loop() {
  }

  void Game_SaveBegin() {
  }

  void Game_SaveEnd() {
  }

  void LoadBegin() {
  }

  void LoadEnd() {
  }

  void Game_LoadBegin_NewGame() {
    LoadBegin();
  }

  void Game_LoadEnd_NewGame() {
    LoadEnd();
  }

  void Game_LoadBegin_SaveGame() {
    LoadBegin();
  }

  void Game_LoadEnd_SaveGame() {
    LoadEnd();
  }

  void Game_LoadBegin_ChangeLevel() {
    LoadBegin();
  }

  void Game_LoadEnd_ChangeLevel() {
    LoadEnd();
  }

  void Game_LoadBegin_Trigger() {
  }
  
  void Game_LoadEnd_Trigger() {
  }
  
  void Game_Pause() {
  }
  
  void Game_Unpause() {
  }
  
  void Game_DefineExternals() {
  }

  // A multiplatform application reference.
  // please disable unused functions.
  // write 'false' in 'Enabled' macro.
#define AppDefault True
  CApplication* lpApplication = !CHECK_THIS_ENGINE ? Null : CApplication::CreateRefApplication(
    Enabled( False ) Game_Entry,
    Enabled( AppDefault ) Game_Init,
    Enabled( False ) Game_Exit,
    Enabled( False ) Game_Loop,
    Enabled( False ) Game_SaveBegin,
    Enabled( False ) Game_SaveEnd,
    Enabled( False ) Game_LoadBegin_NewGame,
    Enabled( False ) Game_LoadEnd_NewGame,
    Enabled( False ) Game_LoadBegin_SaveGame,
    Enabled( False ) Game_LoadEnd_SaveGame,
    Enabled( False ) Game_LoadBegin_ChangeLevel,
    Enabled( False ) Game_LoadEnd_ChangeLevel,
    Enabled( False ) Game_LoadBegin_Trigger,
    Enabled( False ) Game_LoadEnd_Trigger,
    Enabled( False ) Game_Pause,
    Enabled( False ) Game_Unpause,
    Enabled( False ) Game_DefineExternals
  );
}
