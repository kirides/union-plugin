// This file added in headers queue
// File: "Sources.h"
#ifndef __PLUGIN__CLEANUP
#define __PLUGIN__CLEANUP
#define REGISTER_PLUGIN(x) { GOTHIC_ENGINE::x::Init(); cleanup.push_back(GOTHIC_ENGINE::x::Destroy); }
std::vector<std::function<void()>> cleanup;
#endif
namespace GOTHIC_ENGINE {

	void Game_Entry() {
	}

	void Game_Init() {
		if (Gothic::Options::Gothic->ReadBool("UNION_KIRIDES", "bManaReg", true))
			REGISTER_PLUGIN(ManaReg);
		if (Gothic::Options::Gothic->ReadBool("UNION_KIRIDES", "bQuickLoot", true))
			REGISTER_PLUGIN(Quickloot);
	}

	void Game_Exit() {
		for (auto fn : cleanup) fn();
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
		Enabled(False) Game_Entry,
		Enabled(AppDefault) Game_Init,
		Enabled(False) Game_Exit,
		Enabled(False) Game_Loop,
		Enabled(False) Game_SaveBegin,
		Enabled(False) Game_SaveEnd,
		Enabled(False) Game_LoadBegin_NewGame,
		Enabled(False) Game_LoadEnd_NewGame,
		Enabled(False) Game_LoadBegin_SaveGame,
		Enabled(False) Game_LoadEnd_SaveGame,
		Enabled(False) Game_LoadBegin_ChangeLevel,
		Enabled(False) Game_LoadEnd_ChangeLevel,
		Enabled(False) Game_LoadBegin_Trigger,
		Enabled(False) Game_LoadEnd_Trigger,
		Enabled(False) Game_Pause,
		Enabled(False) Game_Unpause,
		Enabled(False) Game_DefineExternals
	);
}
