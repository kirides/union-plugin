namespace ManaReg {
    using namespace GOTHIC_ENGINE;

    static int g_regRate = 2;
    static float g_regTickRate = 2000.0f;

    std::unique_ptr<CInvoke<void(__fastcall*)(oCNpc* _this)>> Ivk_oCNpc__ProcessNpc;

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

    void Init() {
        TCHAR NPath[MAX_PATH];
        // Returns Gothic directory.
        int len = GetCurrentDirectory(MAX_PATH, NPath);
        // Get path to Gothic.Ini
        auto ini = std::string(NPath, len).append("\\system\\Gothic.ini");

        g_regRate = GetPrivateProfileInt("UNION_MANAREG", "iVALUE", 2, ini.c_str());
        g_regTickRate = GetPrivateProfileInt("UNION_MANAREG", "iTICKRATE", 2000, ini.c_str());
        Ivk_oCNpc__ProcessNpc = std::make_unique<CInvoke<void(__fastcall*)(oCNpc* _this)>>(vftable_oCNpc::GetTable().names.f74_ProcessNpc, &RegeneratePlayerMana_ProcessNpc);
    }

    void Destroy() {
        Ivk_oCNpc__ProcessNpc.reset();
    }
}
