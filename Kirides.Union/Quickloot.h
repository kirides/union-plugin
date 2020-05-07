namespace Quickloot {
	using namespace GOTHIC_ENGINE;
	unsigned long curFrameTime = 0;
	int textOffset = 0;

	std::unique_ptr<CInvoke<void(__fastcall*)(oCNpc* _this, void*, const zBOOL a_bForce)>> Ivk_oCNpc__CollectFocusVob;

	void PrintScreen(zSTRING& str) {
		auto mainView = Gothic::Game::Session->array_view[0];
		int y = (8192 - mainView->FontY()) / 2;
		int yWithOffset = y + textOffset;
		if (yWithOffset > mainView->py2) {
			yWithOffset = y;
			textOffset = 0;
		}
		textOffset += mainView->FontY();
		auto view = mainView->CreateText(200, yWithOffset, str);
		view->timed = true;
		view->timer = 2000.f;
		view->colored = false;
		curFrameTime = 0;
	}

	void AccessTheftOrUse(oCNpc* plr, zCVob* vob, int perc) {

		if (perc == NPC_PERC_ASSESSUSEMOB) {
			oCMobInter* mob = zDYNAMIC_CAST<oCMobInter>(vob);
			if (mob) plr->AssessUseMob_S(mob);
			return;
		}

		oCNpc* other = NULL;
		oCNpc* found = NULL;
		zREAL thisDist = 0.0f;
		zREAL minDist = zREAL(+FLT_MAX);

		// Find the closest non-friendly NPC
		int max = plr->vobList.GetNumInList();
		for (int i = 0; i < max; i++) {
			other = zDYNAMIC_CAST<oCNpc>(plr->vobList[i]);
			if (other && (other != plr)) {
				if (other->IsInGlobalCutscene()) continue;

				if ((plr->GetAttitude(other) != NPC_ATT_FRIENDLY)
					&& other->IsConditionValid()
					&& (other->FreeLineOfSight(plr) || other->FreeLineOfSight(vob)))
				{
					thisDist = plr->GetDistanceToVob2(*other);
					if (thisDist < minDist) {
						minDist = thisDist;
						found = other;
					}
				}
			}
		}
		if (found) {
			switch (perc)
			{
			case NPC_PERC_ASSESSTHEFT:
				found->AssessTheft_S(plr);
				break;
			}
		}
	}

	void LootInventory(zCListSort<oCItem>* rootNode, oCNpc* receiver, std::function<bool(oCItem*)>* filter = nullptr) {
		auto invContents = rootNode;
		while (invContents) {
			auto invItem = invContents->GetData();
			if (invItem)
			{
				if (!filter || !(*filter)(invItem)) {
					receiver->CreateItems(invItem->GetInstance(), invItem->amount);
					PrintScreen("Erhalten: " + zSTRING(invItem->amount) + "x " + invItem->GetDescription());
					//lootMessages.push_back("Erhalten: " + zSTRING(invItem->amount) + "x " + invItem->GetDescription());
					rootNode->Remove(invItem);

					// Restart loop at position 1
					invContents = rootNode;
					continue;
				}
			}
			invContents = invContents->GetNextInList();
		}
	}

	bool NpcHasValidBodyState(oCNpc* npc) {
		return (npc->HasBodyStateFreeHands()
			|| npc->IsBodyStateInterruptable()
			|| ((npc->GetBodyState() & BS_STAND) > 0)
			|| ((npc->GetBodyState() & BS_WALK) > 0)
			|| ((npc->GetBodyState() & BS_SNEAK) > 0)
			|| ((npc->GetBodyState() & BS_RUN) > 0)
			|| ((npc->GetBodyState() & BS_SPRINT) > 0)
			|| ((npc->GetBodyState() & BS_SWIM) > 0)
			|| ((npc->GetBodyState() & BS_DIVE) > 0)
			);
	}

	void __fastcall Hook_oCNpc__CollectFocusVob(oCNpc* _this, void* unkwn, const zBOOL a_bForce) {

		curFrameTime += Gothic::Game::Timer->frameTime;
		if (curFrameTime >= 2000) {
			textOffset = 0;
			curFrameTime = 0;
		}

		(*Ivk_oCNpc__CollectFocusVob)(_this, unkwn, a_bForce);
		if (_this && _this->IsSelfPlayer() && _this->IsConditionValid()) {
			auto vob = _this->GetFocusVob();
			oCItem* item;
			oCNpc* npc;
			oCMobContainer* chest;

			if (vob
				&& Gothic::Game::Input->GetMouseButtonPressedRight()
				&& NpcHasValidBodyState(_this))
			{
				if (item = zDYNAMIC_CAST<oCItem>(vob))
				{
					PrintScreen("Erhalten: " + zSTRING(item->amount) + "x " + item->GetDescription());
					_this->PutInInv(item);
					_this->SetFocusVob(NULL);
					AccessTheftOrUse(_this, vob, NPC_PERC_ASSESSTHEFT);
				}
				else if (npc = zDYNAMIC_CAST<oCNpc>(vob)) {
					const int NPC_AISTATE_UNCONSCIOUS = -4;
					bool ok = (
						(npc->attribute[NPC_ATR_HITPOINTS] <= 0)
						|| (
							npc->IsAIState(NPC_AISTATE_UNCONSCIOUS)
							&& npc->attribute[NPC_ATR_HITPOINTS] == 1)
						);
					if (!ok) { return; }

					auto rootNode = npc->inventory2.GetContents();

					static std::function<bool(oCItem*)> ActiveArmorFilter = [](oCItem* invItem) {
						const int ITEM_KAT_ARMOR = 1 << 4;
						const int ITM_FLAG_ACTIVE = 1 << 30;
						return invItem->HasFlag(ITEM_KAT_ARMOR | ITM_FLAG_ACTIVE) != 0;
					};

					LootInventory(rootNode, _this, &ActiveArmorFilter);
					_this->SetFocusVob(NULL);
				}
				else if (chest = zDYNAMIC_CAST<oCMobContainer>(vob)) {
					if (!chest->CanOpen(_this)) { return; }
					if (chest->locked) { return; }

					LootInventory(&chest->containList, _this);
					_this->SetFocusVob(NULL);
					AccessTheftOrUse(_this, vob, NPC_PERC_ASSESSUSEMOB);
				}
			}
		}
	}

	void Init() {
		const int adr_oCNpc__CollectFocusVob =
#if ENGINE == Engine_G2A
			0x00733A10
#else
			0 /* Currently doesn't make sense. Right-clicking is jumping. */
#endif
			;
		if (adr_oCNpc__CollectFocusVob)
		{
			Ivk_oCNpc__CollectFocusVob
				= std::make_unique<CInvoke<void(__fastcall*)(oCNpc* _this, void*, const zBOOL a_bForce)>>(
					adr_oCNpc__CollectFocusVob, &Hook_oCNpc__CollectFocusVob
					);
		}
	}

	void Destroy() {
		Ivk_oCNpc__CollectFocusVob.reset();
	}
}