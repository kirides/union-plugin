namespace ManaReg {
    using namespace GOTHIC_ENGINE;

    static int g_regRate = 2;
    static bool g_isFlatRegen = false;
    static float g_regTickRate = 2000.0f;

    std::unique_ptr<CInvoke<void(__fastcall*)(oCNpc* _this)>> Ivk_oCNpc__ProcessNpc;
    std::function<bool()> is_player_obsessed;

    void RegenerateMana(oCNpc* npc) {
        static CTimer TimerAI;
        static const uint RegenManaID = 1;

        TimerAI.Suspend(RegenManaID, ogame->singleStep);
        if (npc->attribute[NPC_ATR_MANA] < npc->attribute[NPC_ATR_MANAMAX]) {
            int menge;
            if (g_isFlatRegen) {
                menge = g_regRate;
            } else {
                menge = (npc->attribute[NPC_ATR_MANAMAX] * g_regRate) / 100;
            }

            int ManaIntensity = (g_regTickRate / max(menge, 1));
            if (TimerAI(RegenManaID, ManaIntensity)) {
                npc->attribute[NPC_ATR_MANA]++;
            }
        }
        TimerAI.Attach();
    }

    void __fastcall Hook_oCNpc__ProcessNpc(oCNpc* _this) {
        // Invoke the "function pointer"
        (*Ivk_oCNpc__ProcessNpc)(_this);

        if (_this != player) return;
        if (is_player_obsessed && is_player_obsessed()) return;
        
        RegenerateMana(_this);
    }

    void Init() {
        // Dementor Obsession cancels regeneration
        auto ps_SC_IsObsessed = Gothic::Parsers::Game->GetSymbol("SC_IsObsessed");
        if (ps_SC_IsObsessed) {
            is_player_obsessed = [ps_SC_IsObsessed]() { return (ps_SC_IsObsessed && ps_SC_IsObsessed->single_intdata != 0); };
        }

        g_regRate = Gothic::Options::Gothic->ReadInt("UNION_MANAREG", "iValue", 2);
        g_isFlatRegen = Gothic::Options::Gothic->ReadInt("UNION_MANAREG", "bFlat", IS_G1 ? 1 : 0);
        g_regTickRate = Gothic::Options::Gothic->ReadInt("UNION_MANAREG", "iTickRate", 2000);

        Ivk_oCNpc__ProcessNpc = std::make_unique<CInvoke<void(__fastcall*)(oCNpc* _this)>>(vftable_oCNpc::GetTable().names.f74_ProcessNpc, &Hook_oCNpc__ProcessNpc);
    }

    void Destroy() {
        Ivk_oCNpc__ProcessNpc.reset();
    }
}
