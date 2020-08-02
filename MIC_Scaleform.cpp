#include "MIC_Scaleform.h"
#include "Scaleform/MICScaleform_GetReferenceInfo.h"
#include "Scaleform/MICScaleform_GetIniOptions.h"
#include "Scaleform/MICScaleform_RetrieveExtraData.h"
#include "Scaleform/MICScaleform_GetExtraData.h"
#include "Scaleform/MICScaleform_Log.h"
#include "RE/Skyrim.h"
#include "SKSE/API.h"
#include <Windows.h>

MICScaleform_GetReferenceInfo* getReferenceInfo = nullptr;
MICScaleform_GetIniOptions* getIniOptions = nullptr;
MICScaleform_RetrieveExtraData* retrieveExtraData = nullptr;
MICScaleform_GetExtraData* getExtraData = nullptr;
MICScaleform_Log* MICScaleformlog = nullptr;

/*

	
	void GetCharacterData(ExtraInfoEntry * resultArray,  TESForm* pRefForm, TESForm* pBaseForm)
	{
		DebugMessage("GetCharacterData: GetCharacter info start");

		Actor * pActor = nullptr;

		if (pRefForm)
		{
			pActor = DYNAMIC_CAST(pRefForm, TESForm, Actor);
		}

		TESNPC * pNPC = DYNAMIC_CAST(pBaseForm, TESForm, TESNPC);
		PlayerCharacter* pPlayer = nullptr;

		if (pBaseForm->formID == playerBaseFormID)
		{
			pPlayer = DYNAMIC_CAST(pRefForm, TESForm, PlayerCharacter);
		}

		TESActorBase *pActorBase = DYNAMIC_CAST(pNPC, TESNPC, TESActorBase);

		if (pNPC)
		{
			if (pActorBase)
			{
				DebugMessage("GetCharacterData: GetCharacter info casts worked");

				DebugMessage("GetCharacterData: Starting Race");

				//Race
				ExtraInfoEntry * raceEntry;

				TESRace * pRace = pNPC->race.race;

				MICGlobals::filterARMAByRace = pRace;

				std::string raceName = GetName(pRace);

				CreateExtraInfoEntry(raceEntry, "Race", raceName);

				GetFormData(raceEntry, pRace, nullptr);

				resultArray->PushBack(raceEntry);

				DebugMessage("GetCharacterData: Ending Race");

				//Spells
				ExtraInfoEntry * allSpellsEntry;

				CreateExtraInfoEntry(allSpellsEntry, "Spells", "");

				DebugMessage("GetCharacterData: Starting Added Spells");

				if (pActor)
				{
					//Added Spells
					for (int i = 0; i < pActor->addedSpells.Length(); i++)
					{
						ExtraInfoEntry * spellEntry;

						SpellItem* spell = pActor->addedSpells.Get(i);
						GetSpellDataWrapper(spellEntry, spell, "Added Spell");

						allSpellsEntry->PushBack(spellEntry);
					}
				}

				DebugMessage("GetCharacterData: Starting Base Spells");

				//Actor Base Spells
				int numberOfBaseSpells = pActorBase->spellList.GetSpellCount();

				for (int i = 0; i < numberOfBaseSpells; i++)
				{
					ExtraInfoEntry * spellEntry;

					SpellItem* spell = pActorBase->spellList.GetNthSpell(i);
					GetSpellDataWrapper(spellEntry, spell, "Base Spell");

					allSpellsEntry->PushBack(spellEntry);
				}

				resultArray->PushBack(allSpellsEntry);

				DebugMessage("GetCharacterData: GetCharacter Done with spells");

				if (pActor)
				{
					// ActiveEffects as Array
					ExtraInfoEntry * activeEffectsEntry;

					CreateExtraInfoEntry(activeEffectsEntry, "Effects", "");

					tList<ActiveEffect> * effects = pActor->magicTarget.GetActiveEffects();

					DebugMessage("GetCharacterData: Active Effects Gotten");

					if (effects)
					{
						for (int i = 0; i < effects->Count(); i++)
						{
							DebugMessage("GetCharacterData: Starting Active Effect");

							ActiveEffect * pEffect = effects->GetNthItem(i);

							ExtraInfoEntry * effectEntry;

							if (pEffect->effect && pEffect->effect->mgef)
							{
								DebugMessage("GetCharacterData: Active Effect MGEF found");

								std::string effectName, effectActive;

								EffectSetting * mgef = pEffect->effect->mgef;

								if (mgef->fullName.name.data)
								{
									effectName = mgef->fullName.name.data;
								}

								else
								{
									effectName = "Unknown Effect";
								}

								if ((pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive)
								{
									effectActive = "Inactive";
								}

								else
								{
									effectActive = "Active";
								}

								CreateExtraInfoEntry(effectEntry, effectName, effectActive);

								TESForm *effectBaseForm = DYNAMIC_CAST(mgef, EffectSetting, TESForm);

								if (effectBaseForm)
								{
									DebugMessage("GetCharacterData: Active Effect MGEF base form found");

									GetCommonFormData(effectEntry, effectBaseForm, nullptr);

									//Magnitude
									ExtraInfoEntry * magnitudeEntry;

									float magnitude = pEffect->magnitude;
									CreateExtraInfoEntry(magnitudeEntry, "Magnitude", FloatToString(magnitude));
									effectEntry->PushBack(magnitudeEntry);

									//Duration
									ExtraInfoEntry * durationEntry;

									float duration = pEffect->duration;
									CreateExtraInfoEntry(durationEntry, "Duration", FloatToString(duration));
									effectEntry->PushBack(durationEntry);


									//Magnitude
									ExtraInfoEntry * elapsedEntry;

									float elapsed = pEffect->elapsed;
									CreateExtraInfoEntry(elapsedEntry, "Elapsed", FloatToString(elapsed));
									effectEntry->PushBack(elapsedEntry);

									GetMagicEffectData(effectEntry, effectBaseForm);
								}
							}

							else
							{
								CreateExtraInfoEntry(effectEntry, "Unknown Effect Type", "");
							}

							activeEffectsEntry->PushBack(effectEntry);


							DebugMessage("GetCharacterData: Ending Active Effect");

							/*if (pEffect->item)
							scaleformExtend::MagicItemData(effectView, pEffect->item, bRecursive ? bExtra : false, bRecursive); ??? */

							//RegisterBool(effect, "inactive", (pEffect->flags & ActiveEffect::kFlag_Inactive) == ActiveEffect::kFlag_Inactive);

							// ActiveEffect
							//if (pEffect->effect && pEffect->effect->mgef)
							//	scaleformExtend::MagicItemData(effectView, pEffect->effect->mgef, bRecursive ? bExtra : false, bRecursive);

							//activeEffects->PushBack(effect);
							/*
						}
					}

					resultArray->PushBack(activeEffectsEntry);

					DebugMessage("GetExtraData: Active Effects Done");

					ExtraInfoEntry * actorValueHealth;

					GetActorValue(actorValueHealth, pActor, actorValueHealthIndex);
					resultArray->PushBack(actorValueHealth);

					ExtraInfoEntry * actorValueMagicka;

					GetActorValue(actorValueMagicka, pActor, actorValueMagickaIndex);
					resultArray->PushBack(actorValueMagicka);

					ExtraInfoEntry * actorValueStamina;

					GetActorValue(actorValueStamina, pActor, actorValueStaminahIndex);
					resultArray->PushBack(actorValueStamina);

					//Get all actor values in a subarray
					ExtraInfoEntry * actorValueArray;
					CreateExtraInfoEntry(actorValueArray, "Actor Values", "");


					for (int i = 0; i < ActorValueList::kNumActorValues; i++)
					{

						ExtraInfoEntry * actorValue;
						GetActorValue(actorValue, pActor, i);
						actorValueArray->PushBack(actorValue);
					}

					resultArray->PushBack(actorValueArray);

					DebugMessage("GetExtraData: GetCharacter actor values gotten");

					DebugMessage("Before package");

					ActorProcessManager * pProcess = pActor->processManager;

					if (pProcess)
					{
						TESForm * currentPackage = pProcess->unk18.package;

						if (currentPackage)
						{
							DebugMessage("Before getting package name");


							//TESForm * currentPackage = packageData->currentPackage;
							std::string packageName = GetName(currentPackage);

							//Placeholder for seeing what has editor IDs
							ExtraInfoEntry * packageEntry;

							CreateExtraInfoEntry(packageEntry, "Current Package", packageName);


							DebugMessage("Before getting package form data");

							GetFormData(packageEntry, currentPackage, nullptr);

							resultArray->PushBack(packageEntry);

						}
					}

					DebugMessage("After package");

				} //end of a pActor Section

				  //Handle Flags
				int essentialFlag = 0x02;
				int protectedFlag = 0x800;

				ExtraInfoEntry * protectionEntry;


				if ((pNPC->actorData.flags & essentialFlag) == essentialFlag)
				{
					CreateExtraInfoEntry(protectionEntry, "Protection", "Essential");
				}

				else if ((pNPC->actorData.flags & protectedFlag) == protectedFlag)
				{
					CreateExtraInfoEntry(protectionEntry, "Protection", "Protected");
				}

				else
				{
					CreateExtraInfoEntry(protectionEntry, "Protection", "None");
				}

				resultArray->PushBack(protectionEntry);

				//Level stuff

				if (pActor)
				{

					int level = CALL_MEMBER_FN(pActor, GetLevel)();

					ExtraInfoEntry * levelEntry;

					CreateExtraInfoEntry(levelEntry, "Level", IntToString(level));
					resultArray->PushBack(levelEntry);

					DebugMessage("GetExtraData: GetCharacter level gotten");
				}

				ExtraInfoEntry * isPcLeveledEntry;

				bool isLevelMult = (pNPC->actorData.flags & TESActorBaseData::kFlag_PCLevelMult) == TESActorBaseData::kFlag_PCLevelMult;
				if (isLevelMult)
				{
					DebugMessage("GetExtraData: GetCharacter pc level mult set");


					CreateExtraInfoEntry(isPcLeveledEntry, "Is PC Level Mult", "True");

					double levelMult = (double)pNPC->actorData.level / 1000.0;
					int minLevel = pNPC->actorData.minLevel;
					int maxLevel = pNPC->actorData.maxLevel;

					ExtraInfoEntry * levelMultEntry, * minLevelEntry, * maxLevelEntry;

					CreateExtraInfoEntry(levelMultEntry, "Level Mult", DoubleToString(levelMult));
					isPcLeveledEntry->PushBack(levelMultEntry);

					CreateExtraInfoEntry(minLevelEntry, "Min level", IntToString(minLevel));
					isPcLeveledEntry->PushBack(minLevelEntry);

					CreateExtraInfoEntry(maxLevelEntry, "Max Level", IntToString(maxLevel));
					isPcLeveledEntry->PushBack(maxLevelEntry);
				}
				else
				{
					DebugMessage("GetExtraData: GetCharacter pc level mult not set");

					CreateExtraInfoEntry(isPcLeveledEntry, "Is PC Level Mult", "False");
				}

				resultArray->PushBack(isPcLeveledEntry);

				//Perks
				int numPerks = pActorBase->perkRanks.numPerkRanks;

				DebugMessage("GetExtraData: Starting Perks - Total Number" + IntToString(numPerks));

				ExtraInfoEntry * perks;
				CreateExtraInfoEntry(perks, "Perks", "");

				for (int i = 0; i < numPerks; i++)
				{
					DebugMessage("GetExtraData: Starting Perk num" + IntToString(i));

					BGSPerk *perk = pActorBase->perkRanks.perkRanks[i].perk;

					if (perk)
					{
						//int rank = pActorBase->perkRanks.perkRanks[i].rank;

						std::string name = GetName(perk);

						ExtraInfoEntry * perkEntry;

						CreateExtraInfoEntry(perkEntry, name, "");

						GetFormData(perkEntry, perk, nullptr);

						//ExtraInfoEntry * perkEntryRank;

						//CreateExtraInfoEntry(perkEntryRank, "Rank", IntToString(rank) );
						//perkEntrySubArray->PushBack(perkEntryRank);

						perks->PushBack(perkEntry);
					}
				}

				if (pPlayer != nullptr)
				{
					int numPlayerPerks = pPlayer->addedPerks.count;

					DebugMessage("GetExtraData: Starting Player Perks - Total Number" + IntToString(numPlayerPerks));

					for (int i = 0; i < numPlayerPerks; i++)
					{
						DebugMessage("GetExtraData: Starting Player Perk num" + IntToString(i));

						BGSPerk *perk = pPlayer->addedPerks[i]->perk;

						if (perk)
						{
							//int rank = pPlayer->addedPerks[i]->rank;

							std::string name = GetName(perk);

							ExtraInfoEntry * perkEntry;

							CreateExtraInfoEntry(perkEntry, name, "");

							GetFormData(perkEntry, perk, nullptr);

							//ExtraInfoEntry * perkEntryRank;

							//CreateExtraInfoEntry(perkEntryRank, "Rank", IntToString(rank));
							//perkEntrySubArray->PushBack(perkEntryRank);

							perks->PushBack(perkEntry);
						}
					}
				}

				resultArray->PushBack(perks);

				DebugMessage("GetExtraData: Done with perks");



				//apperance - currently height and weight
				DebugMessage("GetExtraData: appearance Started");

				ExtraInfoEntry * appearance;
				CreateExtraInfoEntry(appearance, "Appearance", "");

				float weight = pNPC->weight;

				ExtraInfoEntry * weightEntry;

				CreateExtraInfoEntry(weightEntry, "Weight", FloatToString(weight));
				appearance->PushBack(weightEntry);

				float height = pNPC->height;

				ExtraInfoEntry * heightEntry;

				CreateExtraInfoEntry(heightEntry, "Height", FloatToString(height));
				appearance->PushBack(heightEntry);

				resultArray->PushBack(appearance);

				DebugMessage("GetExtraData: appearance Ended");

				DebugMessage("GetExtraData: factions start");

				ExtraInfoEntry * factionsEntry;

				CreateExtraInfoEntry(factionsEntry, "Factions", "");

				//Factions
				int numFactions = pActorBase->actorData.factions.count;

				if (numFactions > 0)
				{
					for (int i = 0; i < numFactions; i++)
					{
						TESActorBaseData::FactionInfo factionInfo = pActorBase->actorData.factions[i];
						TESFaction *faction = factionInfo.faction;

						if (faction)
						{
							ExtraInfoEntry * factionEntry;

							std::string factionName = GetName(faction);
							int rank = factionInfo.rank;

							CreateExtraInfoEntry(factionEntry, factionName, "Rank: " + IntToString(rank));

							GetFormData(factionEntry, faction, nullptr);

							factionsEntry->PushBack(factionEntry);
						}
					}
				}

				resultArray->PushBack(factionsEntry);

				//need to get factions off of reference as well

				DebugMessage("GetExtraData: factions ended");

			}
		}


		/*PlayerCharacter* pPC = DYNAMIC_CAST(pForm, TESForm, PlayerCharacter);
		if (pPC)
		{
		RegisterNumber(pFxVal, "perkPoints", (double)pPC->numPerkPoints);
		}*/
		/*
		DebugMessage("GetExtraData: GetCharacter End");
	}

	void GetActorValue(ExtraInfoEntry * & resultArray,  Actor * pActor, int id)
	{
		DebugMessage("GetExtraData: GetActover Value Start");

		if (id < ActorValueList::kNumActorValues)
		{
			std::string valueName = GetActorValueName(id);
			float baseValue = pActor->actorValueOwner.GetBase(id);
			float currentValue = pActor->actorValueOwner.GetCurrent(id);
			float maxValue = pActor->actorValueOwner.GetCurrent(id);

			CreateExtraInfoEntry(resultArray, valueName, FloatToString(currentValue));

			//create a subarray for the base  current and maximum

			ExtraInfoEntry * baseValueEntry, * currentValueEntry, * maxValueEntry;

			CreateExtraInfoEntry(baseValueEntry, "Base", FloatToString(baseValue));
			CreateExtraInfoEntry(currentValueEntry, "Current", FloatToString(currentValue));
			CreateExtraInfoEntry(maxValueEntry, "Max", FloatToString(maxValue));

			resultArray->PushBack(baseValueEntry);
			resultArray->PushBack(currentValueEntry);
			resultArray->PushBack(maxValueEntry);
		}

		else
		{
			//the Program will probally crash if we don't create an entry, so add an entry reporting the error in this case
			CreateExtraInfoEntry(resultArray, "Unknown AV id", IntToString(id));
		}

		DebugMessage("GetExtraData: GetActover Value End");
	}

	void GetMagicEffectData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		DebugMessage("GetExtraData: GetMagicEffectData Start");


		EffectSetting * pEffectSetting = DYNAMIC_CAST(pBaseForm, TESForm, EffectSetting);
		if (pEffectSetting)
		{
			DebugMessage("GetExtraData: GetMagicEffectData Cast succeeded");

			//Magic School
			int skill = pEffectSetting->school();
			ExtraInfoEntry * skillEntry;

			CreateExtraInfoEntry(skillEntry, "Magic Skill", GetActorValueName(skill));
			resultArray->PushBack(skillEntry);

			//Minimum Skill Level

			int minimumSkill = pEffectSetting->level();
			ExtraInfoEntry * minimumSkillEntry;

			CreateExtraInfoEntry(minimumSkillEntry, "Minimum Skill", IntToString(minimumSkill));
			resultArray->PushBack(minimumSkillEntry);

			//Effect type 

			int effectType = pEffectSetting->properties.archetype;
			ExtraInfoEntry * effectTypeEntry;

			CreateExtraInfoEntry(effectTypeEntry, "Effect type", GetEffectTypeName(effectType));
			resultArray->PushBack(effectTypeEntry);

			//First AV
			int primaryType = pEffectSetting->properties.primaryValue;
			ExtraInfoEntry * primaryAVEntry;

			CreateExtraInfoEntry(primaryAVEntry, "Primary AV", GetActorValueName(primaryType));
			resultArray->PushBack(primaryAVEntry);

			//Second AV
			int secondaryType = pEffectSetting->properties.secondaryValue;
			ExtraInfoEntry * secondaryAVEntry;

			CreateExtraInfoEntry(secondaryAVEntry, "Secondary AV", GetActorValueName(secondaryType));
			resultArray->PushBack(secondaryAVEntry);

			//Resistence
			int resistence = pEffectSetting->properties.resistance;
			ExtraInfoEntry * resistenceEntry;

			CreateExtraInfoEntry(resistenceEntry, "Primary AV", GetActorValueName(resistence));
			resultArray->PushBack(resistenceEntry);


			//delivery type
			ExtraInfoEntry * deliveryTypeEntry;

			CreateExtraInfoEntry(deliveryTypeEntry, "Delivery Type", GetDeliveryTypeName(pEffectSetting->properties.deliveryType));
			resultArray->PushBack(deliveryTypeEntry);

			//Hostile Flag
			ExtraInfoEntry * hostileEntry;

			bool hostile = (pEffectSetting->properties.flags & EffectSetting::Properties::kEffectType_Hostile) == EffectSetting::Properties::kEffectType_Hostile;

			if (hostile)
			{
				CreateExtraInfoEntry(hostileEntry, "Hostile", "true");
			}

			else
			{
				CreateExtraInfoEntry(hostileEntry, "Hostile", "false");
			}
			resultArray->PushBack(hostileEntry);
			/*



			//RegisterNumber(pFxVal, "effectFlags", pEffectSetting->properties.flags); //I added the hostile flag. Need to check what else to add
			RegisterNumber(pFxVal, "deliveryType", pEffectSetting->properties.deliveryType); //do these next 3 when I add spells
			RegisterNumber(pFxVal, "castTime", pEffectSetting->properties.castingTime);
			RegisterNumber(pFxVal, "delayTime", pEffectSetting->properties.delayTime);
			RegisterNumber(pFxVal, "castType", pEffectSetting->properties.castType);*/
			/*
		}
		DebugMessage("GetExtraData: GetMagicEffectData End");
	}

	void GetSpellData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		/*
		case kFormType_Spell:
		case kFormType_ScrollItem:
		case kFormType_Ingredient:
		case kFormType_Potion:
		case kFormType_Enchantment:
		{ */ /*
		MagicItem * pMagicItem = DYNAMIC_CAST(pBaseForm, TESForm, MagicItem);
		if (pMagicItem)
		{
			DebugMessage("GetSpellData: Starting Effect Data Effect");

			ExtraInfoEntry * magicEffectsEntry;

			CreateExtraInfoEntry(magicEffectsEntry, "Magic Effects", "");

			int numEffects = pMagicItem->effectItemList.count;

			for (int i = 0; i < numEffects; i++)
			{


				MagicItem::EffectItem * pEffect = pMagicItem->effectItemList[i];

				ExtraInfoEntry * effectEntry;

				if (pEffect && pEffect->mgef)
				{
					DebugMessage("GetSpellData: Active Effect MGEF found");

					std::string effectName, effectActive;

					EffectSetting * mgef = pEffect->mgef;

					if (mgef->fullName.name.data)
					{
						effectName = mgef->fullName.name.data;
					}

					else
					{
						effectName = "Unknown Effect";
					}

					CreateExtraInfoEntry(effectEntry, effectName, effectActive);

					TESForm *effectBaseForm = DYNAMIC_CAST(mgef, EffectSetting, TESForm);

					if (effectBaseForm)
					{
						DebugMessage("GetExtraData: Active Effect MGEF base form found");

						GetCommonFormData(effectEntry, effectBaseForm, nullptr);

						//Magnitude
						ExtraInfoEntry * magnitudeEntry;

						float magnitude = pEffect->magnitude;
						CreateExtraInfoEntry(magnitudeEntry, "Magnitude", FloatToString(magnitude));
						effectEntry->PushBack(magnitudeEntry);

						//Duration
						ExtraInfoEntry * durationEntry;

						float duration = pEffect->duration;
						CreateExtraInfoEntry(durationEntry, "Duration", FloatToString(duration));
						effectEntry->PushBack(durationEntry);


						//Magnitude
						ExtraInfoEntry * areaEntry;

						float area = pEffect->area;
						CreateExtraInfoEntry(areaEntry, "Area", FloatToString(area));
						effectEntry->PushBack(areaEntry);

						GetMagicEffectData(effectEntry, effectBaseForm);
					}
				}

				else
				{
					CreateExtraInfoEntry(effectEntry, "Unknown Effect Type", "");
				}

				magicEffectsEntry->PushBack(effectEntry);


				DebugMessage("GetSpellData: Ending Active Effect");


			}

			resultArray->PushBack(magicEffectsEntry);

			DebugMessage("GetSpellData: Done general magic item code");
		}

		SpellItem * pSpellItem = DYNAMIC_CAST(pMagicItem, MagicItem, SpellItem);
		if (pSpellItem)
		{
			DebugMessage("GetSpellData: Starting spell item code");

			//spell type
			ExtraInfoEntry * spellTypeEntry;

			CreateExtraInfoEntry(spellTypeEntry, "Spell Type", GetSpellTypeName(pSpellItem->data.type));
			resultArray->PushBack(spellTypeEntry);

			//casting type
			ExtraInfoEntry * castingTypeEntry;

			CreateExtraInfoEntry(castingTypeEntry, "Cast Type", GetCastingTypeName(pSpellItem->data.castType));
			resultArray->PushBack(castingTypeEntry);

			//spell cost
			ExtraInfoEntry * spellCostEntry;

			CreateExtraInfoEntry(spellCostEntry, "True Cost", IntToString(pSpellItem->GetMagickaCost()));
			resultArray->PushBack(spellCostEntry);

			//cast time
			ExtraInfoEntry * castTimeEntry;

			CreateExtraInfoEntry(castTimeEntry, "Cast Time", FloatToString(pSpellItem->data.castTime));
			resultArray->PushBack(castTimeEntry);

			//RegisterNumber(pFxVal, "spellType", pSpellItem->data.type);
			//RegisterNumber(pFxVal, "trueCost", pSpellItem->GetMagickaCost());

			/*
			BGSEquipSlot * equipSlot = pSpellItem->equipType.GetEquipSlot();
			if (equipSlot)
			RegisterNumber(pFxVal, "equipSlot", equipSlot->formID); */
			/*
			DebugMessage("GetSpellData: Ending spell item code");
		}
		/*
		AlchemyItem * pAlchemyItem = DYNAMIC_CAST(pMagicItem, MagicItem, AlchemyItem);
		if (pAlchemyItem)
		{
		if (pAlchemyItem->itemData.useSound) {
		ExtraInfoEntry * useSound;
		movieView->CreateObject(useSound);
		scaleformExtend::FormData(useSoundView, pAlchemyItem->itemData.useSound, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("useSound", &useSound);
		}
		}

		EnchantmentItem * pEnchantItem = DYNAMIC_CAST(pMagicItem, MagicItem, EnchantmentItem);
		if (pEnchantItem)
		{
		RegisterNumber(pFxVal, "flags", (double)pMagicItem->flags);

		ExtraInfoEntry * baseEnchant;
		movieView->CreateObject(baseEnchant);
		scaleformExtend::FormData(baseEnchantView, pEnchantItem->data.baseEnchantment, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("baseEnchant", &baseEnchant);

		ExtraInfoEntry * restrictions;
		movieView->CreateObject(restrictions);
		scaleformExtend::FormData(restrictionsView, pEnchantItem->data.restrictions, bRecursive ? bExtra : false, bRecursive);
		pFxVal->SetMember("restrictions", &restrictions);
		}
		}	*//*
	}

	//wrapper for getting both the common form data and the spell data for a spell
	void GetSpellDataWrapper(ExtraInfoEntry * & spellEntry,  SpellItem* spell, std::string source)
	{
		DebugMessage("GetSpellDataWrapper: Starting spell");

		TESForm *spellBaseForm = DYNAMIC_CAST(spell, SpellItem, TESForm);

		if (spellBaseForm)
		{
			std::string spellName = "Unknown";

			if (spell->fullName.name.data)
			{
				spellName = spell->fullName.name.data;
			}

			CreateExtraInfoEntry(spellEntry, spellName, source);

			GetCommonFormData(spellEntry, spellBaseForm, nullptr);
			GetSpellData(spellEntry, spellBaseForm);
		}

		else
		{
			CreateExtraInfoEntry(spellEntry, "Unknown Spell", source);
		}

		DebugMessage("GetSpellDataWrapper: Finished spell");
	}

	void GetEquipment(ExtraInfoEntry * resultArray,  ExtraContainerChanges* pContainerChanges, Actor * pActor)
	{
		DebugMessage("GetEquipment: GetEquipment Start");

		ExtraInfoEntry * equipmentEntry;

		CreateExtraInfoEntry(equipmentEntry, "Equipment", "");

		//weapons and shouts

		//left hand
		TESForm * leftHand = pActor->GetEquippedObject(true);

		if (leftHand != nullptr)
		{
			ExtraInfoEntry * leftHandEntry;
			std::string name = GetName(leftHand);

			CreateExtraInfoEntry(leftHandEntry, "Left Hand:", name);


			GetFormData(leftHandEntry, leftHand, nullptr);
			equipmentEntry->PushBack(leftHandEntry);
		}

		//right hand
		TESForm * rightHand = pActor->GetEquippedObject(false);

		if (rightHand != nullptr)
		{
			ExtraInfoEntry * rightHandEntry;
			std::string name = GetName(rightHand);

			CreateExtraInfoEntry(rightHandEntry, "Right Hand:", name);

			GetFormData(rightHandEntry, rightHand, nullptr);
			equipmentEntry->PushBack(rightHandEntry);
		}

		//shout
		TESForm * shout = pActor->equippedShout;

		if (shout != nullptr)
		{
			ExtraInfoEntry * ShoutEntry;
			std::string name = GetName(shout);

			CreateExtraInfoEntry(ShoutEntry, "Shout:", name);

			GetFormData(ShoutEntry, shout, nullptr);
			equipmentEntry->PushBack(ShoutEntry);
		}

		//check each equip slot
		for (int i = 0; i < 32; i++)
		{
			DebugMessage("GetEquipment: Starting EquipSlot item");

			int mask = 1 << i;

			MatchBySlot matcher(mask);
			EquipData eqD = pContainerChanges->FindEquipped(matcher);

			if (eqD.pForm != nullptr)
			{
				DebugMessage("GetEquipment: EquipSlot item Found");

				TESForm * equipedItem = eqD.pForm;

				ExtraInfoEntry * equipedItemEntry;
				std::string name = GetName(equipedItem);
				std::string slotName = GetEquipSlotName(i);

				CreateExtraInfoEntry(equipedItemEntry, slotName, name);

				GetFormData(equipedItemEntry, equipedItem, nullptr);
				equipmentEntry->PushBack(equipedItemEntry);

			}

			DebugMessage("GetEquipment: Ending EquipSlot item");
		}

		resultArray->PushBack(equipmentEntry);

		DebugMessage("GetEquipment: GetEquipment End");
	}

	void GetInventory(ExtraInfoEntry * resultArray,  EntryDataList * inventory, TESContainer * baseContainer)
	{
		DebugMessage("GetInventory: GetInventory Start");

		ExtraInfoEntry * inventoryEntry;

		CreateExtraInfoEntry(inventoryEntry, "Inventory", "");

		/*if (inventory != nullptr)
		{
			MICGlobals::reducedMode = true;
		}*/

		//go through the inventory (these are anything changed from the base form)
		/*
		if (inventory != nullptr
			&& inventory->Count() > 0)
		{
			for (EntryDataList::Iterator i = inventory->Begin(); !i.End(); ++i)
			{
				DebugMessage("GetInventory: Starting inventory item");

				InventoryEntryData * item = i.Get();


				int itemCount = item->countDelta;
				TESForm *itemForm = item->type;

				if (baseContainer != nullptr)
				{
					itemCount = itemCount + NumberOfItemInContainer(itemForm, baseContainer);
				}

				if (itemCount > 0)
				{
					std::string name = GetName(itemForm);

					ExtraInfoEntry * inventoryItemEntry;

					CreateExtraInfoEntry(inventoryItemEntry, name, IntToString(itemCount));

					GetFormData(inventoryItemEntry, itemForm, nullptr);

					inventoryEntry->PushBack(inventoryItemEntry);
				}

				DebugMessage("GetInventory: Ending inventory item");

			}
		}

		//go through the items in the base form
		if (baseContainer != nullptr)
		{
			for (int i = 0; i < baseContainer->numEntries; i++)
			{

				DebugMessage("GetInventory: Starting container item");

				RE::TESForm *itemForm = baseContainer->entries[i]->form;

				//only display items which have not changed from the base form
				bool displayItem = true;

				if (inventory != nullptr)
				{
					displayItem = !HasItem(inventory, itemForm);
				}

				if (displayItem
					&& itemForm->GetFormType() != kFormType_LeveledItem)
				{

					DebugMessage("GetInventory: Displaying container item");
					int itemCount = baseContainer->entries[i]->count;

					std::string name = GetName(itemForm);

					ExtraInfoEntry * inventoryItemEntry;

					if (name != "")
					{
						CreateExtraInfoEntry(inventoryItemEntry, name, IntToString(itemCount));
					}

					else
					{
						CreateExtraInfoEntry(inventoryItemEntry, name, FormIDToString(itemForm->formID));
					}

					GetFormData(inventoryItemEntry, itemForm, nullptr);

					inventoryEntry->PushBack(inventoryItemEntry);
				}


				DebugMessage("GetInventory: Ending container item");
			}
		}

		resultArray->PushBack(inventoryEntry);

		//MICGlobals::reducedMode = false;

		DebugMessage("GetInventory: GetInventory End");
	}

	void GetArmaData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		DebugMessage("GetArmaData: GetArmaData Start");

		TESObjectARMA * pArma = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMA);

		if (pArma)
		{
			DebugMessage("GetArmaData: Before Cast");
			TESForm * maleSkin = (TESForm *)pArma->unk130;

			DebugMessage("GetArmaData: After Cast");
			if (maleSkin)
			{
				ExtraInfoEntry * maleSkinEntry;
				CreateExtraInfoEntry(maleSkinEntry, "Male skin", "");

				GetFormData(maleSkinEntry, maleSkin, nullptr);

				resultArray->PushBack(maleSkinEntry);
			}


			DebugMessage("GetArmaData: Before Cast");
			TESForm * femaleSkin = (TESForm *)pArma->unk138;

			DebugMessage("GetArmaData: After Cast");
			if (femaleSkin)
			{
				ExtraInfoEntry * femaleSkinEntry;
				CreateExtraInfoEntry(femaleSkinEntry, "Female skin", "");

				GetFormData(femaleSkinEntry, femaleSkin, nullptr);

				resultArray->PushBack(femaleSkinEntry);
			}

			//races

			MICGlobals::readRaceSkins = false; //disable reading race skin entries to avoid infinite loops

											   //Primary race
			TESRace * defaultRace = pArma->race.race;

			if (defaultRace)
			{
				std::string defaultRaceName = GetName(defaultRace);

				ExtraInfoEntry * defaultRacesEntry;

				CreateExtraInfoEntry(defaultRacesEntry, "Primary Race", defaultRaceName);

				GetFormData(defaultRacesEntry, defaultRace, nullptr);

				resultArray->PushBack(defaultRacesEntry);
			}
			//Additional races

			if (pArma->additionalRaces.count > 0)
			{
				ExtraInfoEntry * additionalRacesEntry;

				CreateExtraInfoEntry(additionalRacesEntry, "Additional Races", "");

				for (int i = 0; i < pArma->additionalRaces.count; i++)
				{
					TESRace * additionalRace = pArma->additionalRaces[i];

					if (additionalRace)
					{
						std::string additionalRaceName = GetName(additionalRace);

						ExtraInfoEntry * additionalRaceEntry;

						CreateExtraInfoEntry(additionalRaceEntry, additionalRaceName, "");

						GetFormData(additionalRaceEntry, additionalRace, nullptr);

						additionalRacesEntry->PushBack(additionalRaceEntry);
					}
				}

				resultArray->PushBack(additionalRacesEntry);
			}


			MICGlobals::readRaceSkins = true;

			//resultArray->PushBack(racesEntry);

			//Skin Textures

		}

		DebugMessage("GetArmaData: GetArmaData End");
	}

	void GetArmorData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESObjectARMO * pArmor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMO);

		if (pArmor)
		{
			//value
			int armorRating = pArmor->armorValTimes100 / 100;

			ExtraInfoEntry * armorRatingEntry;

			CreateExtraInfoEntry(armorRatingEntry, "Armor Rating", IntToString(armorRating));
			resultArray->PushBack(armorRatingEntry);

			//value
			int value = pArmor->value.value;

			ExtraInfoEntry * valueEntry;

			CreateExtraInfoEntry(valueEntry, "Value", IntToString(value));
			resultArray->PushBack(valueEntry);

			//weight
			int weight = pArmor->weight.weight;

			ExtraInfoEntry * weightEntry;

			CreateExtraInfoEntry(weightEntry, "Weight", IntToString(weight));
			resultArray->PushBack(weightEntry);

			//weight class
			int weightClass = pArmor->bipedObject.data.weightClass;

			ExtraInfoEntry * weightClassEntry;

			CreateExtraInfoEntry(weightClassEntry, "Armor Type", GetArmorWeightClassName(weightClass));
			resultArray->PushBack(weightClassEntry);

			//Equip slots
			int parts = pArmor->bipedObject.data.parts;

			ExtraInfoEntry * equipSlotsEntry;

			CreateExtraInfoEntry(equipSlotsEntry, "Equip Slots", "");

			for (int i = 0; i <= 31; i++)
			{
				int mask = 1 << i;

				if ((parts & mask) == mask)
				{

					std::string slotName = GetEquipSlotName(i);

					ExtraInfoEntry * equipSlotEntry;

					CreateExtraInfoEntry(equipSlotEntry, slotName, "");
					equipSlotsEntry->PushBack(equipSlotEntry);
				}
			}

			resultArray->PushBack(equipSlotsEntry);

			if (!MICGlobals::reducedMode)
			{
				ExtraInfoEntry * armorAddonsEntry;

				CreateExtraInfoEntry(armorAddonsEntry, "Armor Entry", "");

				for (int i = 0; i < pArmor->armorAddons.count; i++)
				{
					bool addEntry = true;

					//if we are filtering by race
					if (MICGlobals::filterARMAByRace != nullptr)
					{
						addEntry = pArmor->armorAddons[i]->isValidRace(MICGlobals::filterARMAByRace);
					}

					if (addEntry)
					{
						ExtraInfoEntry * armorAddonEntry;

						std::string armorAddonName = GetName(pArmor->armorAddons[i]);

						CreateExtraInfoEntry(armorAddonEntry, armorAddonName, "");

						GetFormData(armorAddonEntry, pArmor->armorAddons[i], nullptr);

						armorAddonsEntry->PushBack(armorAddonEntry);
					}
				}

				resultArray->PushBack(armorAddonsEntry);
			}
		}
	}

	void GetWeaponData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESObjectWEAP * pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
		if (pWeapon)
		{
			//animation type
			int animationType = pWeapon->type();

			ExtraInfoEntry * antimationTypeEntry;

			CreateExtraInfoEntry(antimationTypeEntry, "Animation type", GetWeaponAnimationTypeName(animationType));
			resultArray->PushBack(antimationTypeEntry);

			//damage
			int damage = pWeapon->damage.GetAttackDamage();

			ExtraInfoEntry * damageEntry;

			CreateExtraInfoEntry(damageEntry, "Damage", IntToString(damage));
			resultArray->PushBack(damageEntry);

			//speed
			float speed = pWeapon->speed();

			ExtraInfoEntry * speedEntry;

			CreateExtraInfoEntry(speedEntry, "Speed", FloatToString(speed));
			resultArray->PushBack(speedEntry);

			//reach
			float reach = pWeapon->reach();

			ExtraInfoEntry * reachEntry;

			CreateExtraInfoEntry(reachEntry, "Reach", FloatToString(reach));
			resultArray->PushBack(reachEntry);

			//stagger
			float stagger = pWeapon->stagger();

			ExtraInfoEntry * staggerEntry;

			CreateExtraInfoEntry(staggerEntry, "Stagger", FloatToString(stagger));
			resultArray->PushBack(staggerEntry);


			//crit damage
			int critDamage = pWeapon->critDamage();

			ExtraInfoEntry * critDamageEntry;

			CreateExtraInfoEntry(critDamageEntry, "Crit damage", IntToString(critDamage));
			resultArray->PushBack(critDamageEntry);

			/*RegisterNumber(pFxVal, "minRange", pWeapon->minRange());
			RegisterNumber(pFxVal, "maxRange", pWeapon->maxRange()); Data for npc AI*/
			/*
			BGSEquipSlot * equipSlot = pWeapon->equipType.GetEquipSlot();
			if (equipSlot)
			{
				std::string equipSlotName = GetEquipTypeName(equipSlot->formID);

				ExtraInfoEntry * equipSlotEntry;

				CreateExtraInfoEntry(equipSlotEntry, "Equip slot", equipSlotName);
				resultArray->PushBack(equipSlotEntry);
			}

			//value
			int value = pWeapon->value.value;

			ExtraInfoEntry * valueEntry;

			CreateExtraInfoEntry(valueEntry, "Value", IntToString(value));
			resultArray->PushBack(valueEntry);

			//weight
			int weight = pWeapon->weight.weight;

			ExtraInfoEntry * weightEntry;

			CreateExtraInfoEntry(weightEntry, "Weight", IntToString(weight));
			resultArray->PushBack(weightEntry);

		}
	}

	void GetAmmoData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESAmmo * pAmmo = DYNAMIC_CAST(pBaseForm, TESForm, TESAmmo);
		if (pAmmo)
		{
			//damage
			float damage = pAmmo->settings.damage;

			ExtraInfoEntry * damageEntry;

			CreateExtraInfoEntry(damageEntry, "Damage", FloatToString(damage));
			resultArray->PushBack(damageEntry);


			//value
			int value = pAmmo->value.value;

			ExtraInfoEntry * valueEntry;

			CreateExtraInfoEntry(valueEntry, "Value", IntToString(value));
			resultArray->PushBack(valueEntry);

		}
	}

	void GetContainerData(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		TESObjectCONT * pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
		if (pContainer)
		{
			int respawnsFlag = 0x02;

			ExtraInfoEntry * safeContainerEntry;

			if ((pContainer->unkB9 & respawnsFlag) == respawnsFlag)
			{
				CreateExtraInfoEntry(safeContainerEntry, "Safe Container", "No");
			}

			else
			{
				CreateExtraInfoEntry(safeContainerEntry, "Safe Container", "Yes");
			}

			resultArray->PushBack(safeContainerEntry);
		}
	}


	void GetLockData(ExtraInfoEntry* resultArray, ExtraLock* lockData)
	{		
		if (lockData->state)
		{
			//Placeholder for seeing what has editor IDs
			ExtraInfoEntry* lockEntry;

			CreateExtraInfoEntry(lockEntry, "Lock", "");

			bool isLocked = (lockData->state->flags & LockState::Flag::kLocked) == LockState::Flag::kLocked;
			
			ExtraInfoEntry* isLockedEntry;
			CreateExtraInfoEntry(isLockedEntry, "Is locked", BooleanToYesNoString(isLocked));
			lockEntry->PushBack(isLockedEntry);

			int lockLevel = lockData->state->lockLevel;
			std::string lockLevelName = GetLockLevel(lockLevel);

			ExtraInfoEntry * lockLevelEntry;
			CreateExtraInfoEntry(lockLevelEntry, "Lock level", lockLevelName);
			lockEntry->PushBack(lockLevelEntry);

			TESForm* key = lockData->state->key;
			ExtraInfoEntry* keyEntry;

			if (key)
			{
				std::string keyName = GetName(key);

				CreateExtraInfoEntry(keyEntry, "Key", keyName);

				GetFormData(keyEntry, key, nullptr);

				lockEntry->PushBack(keyEntry);
			}

			else
			{
				CreateExtraInfoEntry(keyEntry, "Key", "None");
				lockEntry->PushBack(keyEntry);
			}

			/*
			int unknown1 = lockData->state->unk01;
			ExtraInfoEntry* unk01;
			CreateExtraInfoEntry(unk01, "unk01", IntToString( unknown1) );
			lockEntry->PushBack(unk01);

			int unknown2 = lockData->state->unk02;
			ExtraInfoEntry* unk02;
			CreateExtraInfoEntry(unk02, "unk02", IntToString(unknown2));
			lockEntry->PushBack(unk02);

			int unknown4 = lockData->state->unk04;
			ExtraInfoEntry* unk04;
			CreateExtraInfoEntry(unk04, "unk04", IntToString(unknown4));
			lockEntry->PushBack(unk04);

			int unknown11 = lockData->state->unk11;
			ExtraInfoEntry* unk11;
			CreateExtraInfoEntry(unk11, "unk11", IntToString(unknown11));
			lockEntry->PushBack(unk11);

			int unknown12 = lockData->state->unk12;
			ExtraInfoEntry* unk12;
			CreateExtraInfoEntry(unk12, "unk12", IntToString(unknown12));
			lockEntry->PushBack(unk12);

			int unknown14 = lockData->state->unk14;
			ExtraInfoEntry* unk14;
			CreateExtraInfoEntry(unk14, "unk14", IntToString(unknown14));
			lockEntry->PushBack(unk14);

			int unknown18 = lockData->state->unk18;
			ExtraInfoEntry* unk18;
			CreateExtraInfoEntry(unk18, "unk18", IntToString(unknown18));
			lockEntry->PushBack(unk18);

			int unknown1C = lockData->state->unk1C;
			ExtraInfoEntry* unk1C;
			CreateExtraInfoEntry(unk1C, "unk1C", IntToString(unknown1C));
			lockEntry->PushBack(unk1C);

			int flags = (UInt8)lockData->state->flags;
			ExtraInfoEntry* flagsEntry;
			CreateExtraInfoEntry(flagsEntry, "Flags", IntToString(flags));
			lockEntry->PushBack(flagsEntry); */
			/*
			resultArray->PushBack(lockEntry);
		}

	}

	void GetWorldData(ExtraInfoEntry* resultArray)
	{
		PlayerCharacter* pPC = (*g_thePlayer);

		if (pPC)
		{
			DebugMessage("Starting Worldspace");

			TESWorldSpace* currentWorldSpace = pPC->currentWorldSpace;

			if (currentWorldSpace)
			{
				std::string worldSpaceName = GetName(currentWorldSpace);

				ExtraInfoEntry* worldSpaceEntry;
				CreateExtraInfoEntry(worldSpaceEntry, "World Space", worldSpaceName);

				GetFormData(worldSpaceEntry, currentWorldSpace, nullptr);
				resultArray->PushBack(worldSpaceEntry);
			}

			DebugMessage("Starting Cell");

			TESObjectCELL* currentCell = pPC->parentCell;

			if (currentCell )
			{
				std::string cellName = GetName(currentCell);

				ExtraInfoEntry* cellEntry;
				CreateExtraInfoEntry(cellEntry, "Cell", cellName);

				GetFormData(cellEntry, currentCell, nullptr);

				//BSExtraData * extraData = currentCell->unk048.extraData;

				resultArray->PushBack(cellEntry);
			}
		}

		//Find the current music track
		
		DataHandler* dataHandler = DataHandler::GetSingleton();

		if (dataHandler)
		{
			DebugMessage("Starting Music");
			tArray<BGSMusicTrackFormWrapper*> *	musicTrackArray = (tArray<BGSMusicTrackFormWrapper*>*)(&(dataHandler->arrMUST));

			int numberOfMusicTracks = musicTrackArray->count;

			for (int i = 0; i <= numberOfMusicTracks; i++)
			{
				BGSMusicTrackFormWrapper* wrapper;
				musicTrackArray->GetNthItem(i, wrapper);

				if (wrapper)
				{
					BSIMusicTrack * musicTrack = &(wrapper->track);

					MUSIC_STATUS musicTrackStatus = (MUSIC_STATUS)musicTrack->Unk_08();

					if( musicTrackStatus == MUSIC_STATUS::kPlaying)
					{
						std::string  formIDString = FormIDToString(wrapper->formID);
						ExtraInfoEntry* musicEntry;
						CreateExtraInfoEntry(musicEntry, "Music", formIDString);
						resultArray->PushBack(musicEntry);
					}
				}
			}
		}
	}

	/*
	void StandardItemData(ExtraInfoEntry * pFxVal, TESForm * pForm, InventoryEntryData * pEntry)
	{
	if (!pForm || !pFxVal || !pFxVal->IsObject())
	return;

	switch (pForm->GetFormType())
	{

	break;

	case kFormType_SoulGem:
	{
	TESSoulGem	* soulGem = DYNAMIC_CAST(pForm, TESForm, TESSoulGem);
	if (soulGem)
	{
	RegisterNumber(pFxVal, "gemSize", soulGem->gemSize);
	RegisterNumber(pFxVal, "soulSize", pEntry ? CALL_MEMBER_FN(pEntry, GetSoulLevel)() : soulGem->soulSize);
	}
	}
	break;

	case kFormType_Potion:
	{
	AlchemyItem * pAlchemy = DYNAMIC_CAST(pForm, TESForm, AlchemyItem);
	if (pAlchemy)
	{
	RegisterNumber(pFxVal, "flags", pAlchemy->itemData.flags);
	}
	}
	break;

	case kFormType_Book:
	{
	TESObjectBOOK * pBook = DYNAMIC_CAST(pForm, TESForm, TESObjectBOOK);
	if (pBook)
	{
	RegisterNumber(pFxVal, "flags", pBook->data.flags);
	RegisterNumber(pFxVal, "bookType", pBook->data.type);
	switch (pBook->data.GetSanitizedType())
	{
	case TESObjectBOOK::Data::kType_Skill:
	RegisterNumber(pFxVal, "teachesSkill", pBook->data.teaches.skill);
	break;

	case TESObjectBOOK::Data::kType_Spell:
	{
	double formID = -1;

	if (pBook->data.teaches.spell)
	formID = pBook->data.teaches.spell->formID;

	RegisterNumber(pFxVal, "teachesSpell", formID);
	}
	break;
	}
	}
	}
	break;

	default:
	break;
	}
	}*/
	/*
	void GetModelTextures(ExtraInfoEntry * resultArray,  TESForm* pBaseForm)
	{
		DebugMessage("Starting GetModelTextures " + GetFormTypeName(pBaseForm->formType));
		if (pBaseForm->formType == kFormType_Static)
		{
			TESObjectSTAT *pStatic = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectSTAT);

			if (pStatic)
			{
				TESModelTextureSwap * textSwap = &(pStatic->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_MovableStatic)
		{
			BGSMovableStatic *pMoveStatic = DYNAMIC_CAST(pBaseForm, TESForm, BGSMovableStatic);

			if (pMoveStatic)
			{
				TESModelTextureSwap * textSwap = &(pMoveStatic->staticObj.texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Activator)
		{
			TESObjectACTI *pActivate = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectACTI);

			if (pActivate)
			{
				TESModelTextureSwap * textSwap = &(pActivate->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Tree)
		{
			TESObjectTREE *pTree = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectTREE);

			if (pTree)
			{
				TESModel * model = &(pTree->model);
				AddModelEntry(resultArray, "Model", model);
			}
		}

		else if (pBaseForm->formType == kFormType_Flora)
		{
			TESFlora *pFlora = DYNAMIC_CAST(pBaseForm, TESForm, TESFlora);

			if (pFlora)
			{
				TESModelTextureSwap * textSwap = &(pFlora->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Furniture)
		{
			TESFurniture *pFurniture = DYNAMIC_CAST(pBaseForm, TESForm, TESFurniture);

			if (pFurniture)
			{
				TESModelTextureSwap * textSwap = &(pFurniture->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Door)
		{
			TESObjectDOOR *pDoor = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectDOOR);
			if (pDoor)
			{
				TESModelTextureSwap * textSwap = &(pDoor->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Container)
		{
			TESObjectCONT *pContainer = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCONT);
			if (pContainer)
			{
				TESModelTextureSwap * textSwap = &(pContainer->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Misc)
		{
			TESObjectMISC *pMisc = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectMISC);
			if (pMisc)
			{
				TESModelTextureSwap * textSwap = &(pMisc->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Book)
		{
			TESObjectBOOK *pBook = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectBOOK);
			if (pBook)
			{
				TESModelTextureSwap * textSwap = &(pBook->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Key)
		{
			TESKey *pKey = DYNAMIC_CAST(pBaseForm, TESForm, TESKey);
			if (pKey)
			{
				TESModelTextureSwap * textSwap = &(pKey->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_SoulGem)
		{
			TESSoulGem *pSoulGem = DYNAMIC_CAST(pBaseForm, TESForm, TESSoulGem);
			if (pSoulGem)
			{
				TESModelTextureSwap * textSwap = &(pSoulGem->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Ingredient)
		{
			IngredientItem *pIngredient = DYNAMIC_CAST(pBaseForm, TESForm, IngredientItem);
			if (pIngredient)
			{
				TESModelTextureSwap * textSwap = &(pIngredient->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}

		else if (pBaseForm->formType == kFormType_Potion)
		{
			AlchemyItem *pAlchemy = DYNAMIC_CAST(pBaseForm, TESForm, AlchemyItem);
			if (pAlchemy)
			{
				TESModelTextureSwap * textSwap = &(pAlchemy->texSwap);
				AddModelEntry(resultArray, "Model", textSwap);
			}
		}


		else if (pBaseForm->formType == kFormType_Weapon)
		{
			TESObjectWEAP *pWeapon = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectWEAP);
			if (pWeapon)
			{
				TESModelTextureSwap * textSwap = &(pWeapon->texSwap);

				if (textSwap)
				{
					AddModelEntry(resultArray, "Model", textSwap);
				}

				TESObjectSTAT * firstPersonModel = pWeapon->model;

				if (firstPersonModel)
				{
					TESModelTextureSwap * textSwapFirstPerson = &(firstPersonModel->texSwap);

					if (textSwapFirstPerson)
					{
						AddModelEntry(resultArray, "First Person Model", textSwapFirstPerson);
					}
				}
			}
		}

		else if (pBaseForm->formType == kFormType_ARMA)
		{
			TESObjectARMA *pArma = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectARMA);
			if (pArma)
			{
				//Texture entries are theortical
				TESModelTextureSwap * maleModel = &(pArma->models[0][0]);

				if (maleModel)
				{
					AddModelEntry(resultArray, "Model Male", maleModel);
				}

				TESModelTextureSwap * femaleModel = &(pArma->models[0][1]);
				if (femaleModel)
				{
					AddModelEntry(resultArray, "Model Female", femaleModel);
				}

				TESModelTextureSwap * maleFirstPerson = &(pArma->models[1][0]);
				if (maleFirstPerson)
				{
					AddModelEntry(resultArray, "Model Male 1st Person", maleFirstPerson);
				}

				TESModelTextureSwap * femaleFirstPerson = &(pArma->models[1][1]);
				if (femaleFirstPerson)
				{
					AddModelEntry(resultArray, "Model Female 1st Person", femaleFirstPerson);
				}
			}
		}

		//_MESSAGE( textSwap->GetModelName() ); //example output Architecture\Winterhold\WinterholdExtTowerRing01.nif

		/*
		int count = textSwap->count;

		_MESSAGE(IntToString(count).c_str());

		for (int i = 0; i < count; i++)
		{
		BGSTextureSet * textureSet = textSwap->swaps[i].textureSet;

		for (int i = 0; i < textureSet->kNumTextures; i++)
		{
		_MESSAGE(textureSet->texturePaths[i].str);
		}
		}*/
		/*

		DebugMessage("Ending GetModelTextures");
	}


	void GetRaceEntry(ExtraInfoEntry * resultArray,  TESForm * pBaseForm)
	{
		DebugMessage("Starting GetRaceEntry");

		TESRace * pRace = DYNAMIC_CAST(pBaseForm, TESForm, TESRace);
		if (pRace)
		{
			DebugMessage("Getting Editor ID");

			//editor ID
			std::string editorID = pRace->editorId.Get();

			ExtraInfoEntry * editorIDEntry;

			CreateExtraInfoEntry(editorIDEntry, "EditorID", editorID);
			resultArray->PushBack(editorIDEntry);


			DebugMessage("Getting Models");
			//models
			TESModel * maleModel = &(pRace->models[0]);
			TESModel * femaleModel = &(pRace->models[1]);

			AddModelEntry(resultArray, "Male Skeleton", maleModel);
			AddModelEntry(resultArray, "Female Skeleton", femaleModel);

			//Skins
			if (MICGlobals::readRaceSkins
				&& pRace->skin.skin != nullptr)
			{
				DebugMessage("Getting Skin");
				TESObjectARMO *skin = pRace->skin.skin;

				std::string skinName = GetName(skin);

				ExtraInfoEntry * skinEntry;

				CreateExtraInfoEntry(skinEntry, "Skin", skinName);

				GetFormData(skinEntry, skin, nullptr);

				resultArray->PushBack(skinEntry);

				DebugMessage("Done Getting Skin");
			}

			//Handle Flags
			int playableFlag = 0x00000001;
			int childFlag = 0x00000004;

			ExtraInfoEntry * playableEntry;

			if ((pRace->data.raceFlags & playableFlag) == playableFlag)
			{
				CreateExtraInfoEntry(playableEntry, "Playable", "Yes");
			}

			else
			{
				CreateExtraInfoEntry(playableEntry, "Playable", "No");
			}

			resultArray->PushBack(playableEntry);

			ExtraInfoEntry * childEntry;

			if ((pRace->data.raceFlags & childFlag) == childFlag)
			{
				CreateExtraInfoEntry(childEntry, "Child", "Yes");
			}

			else
			{
				CreateExtraInfoEntry(childEntry, "Child", "No");
			}

			resultArray->PushBack(childEntry);
		}

		DebugMessage("Ending GetRaceEntry");
	}

	void GetCellEntry(ExtraInfoEntry* resultArray, TESForm* pBaseForm)
	{
		DebugMessage("Starting GetCellEntry");

		TESObjectCELL* pCell = DYNAMIC_CAST(pBaseForm, TESForm, TESObjectCELL);
		if (pCell)
		{

			LightingCoordinates * lightningCoordinates = (LightingCoordinates*)(&(pCell->unk060));
			
			if (lightningCoordinates->coordinates)
			{
				
				Coordinates* coordinates = lightningCoordinates->coordinates;

				std::string coordinateString = IntToString(coordinates->cellX) + "," + IntToString(coordinates->cellY);

				ExtraInfoEntry* coordinateEntry;

				CreateExtraInfoEntry(coordinateEntry, "Coordinates", coordinateString);

				resultArray->PushBack(coordinateEntry);
			}

			BSExtraData * extraData = pCell->unk048.extraData;

			while (extraData)
			{
				int extraDataType = extraData->GetType();
				std::string extraDataString = GetExtraDataTypeName(extraDataType);

				ExtraInfoEntry* extraDataTypeEntry;

				CreateExtraInfoEntry(extraDataTypeEntry, "Extra Data", extraDataString);
				resultArray->PushBack(extraDataTypeEntry);

				extraData = extraData->next;
			}
			
		}
	}
};*/

//// core hook
bool moreInformativeConsoleScaleForm::InstallHooks( RE::GFxMovieView* a_view, RE::GFxValue* a_root )
{
	_DMESSAGE("Install Hooks Called");

	if (getReferenceInfo == nullptr)
	{
		getReferenceInfo = new MICScaleform_GetReferenceInfo;
	}

	if (getIniOptions == nullptr)
	{
		getIniOptions = new MICScaleform_GetIniOptions;
	}

	if (retrieveExtraData == nullptr)
	{
		retrieveExtraData = new MICScaleform_RetrieveExtraData;
	}

	if (getExtraData == nullptr)
	{
		getExtraData = new MICScaleform_GetExtraData;
	}

	
	if (MICScaleformlog == nullptr)
	{
		MICScaleformlog = new MICScaleform_Log;
	}

	RE::GFxValue globals;

	_DMESSAGE( a_view->GetMovieDef()->GetFileURL() );

	bool result = a_view->GetVariable(&globals, "_global");
	if (result)
	{
		//Create Object to hold new functions
		RE::GFxValue MIC;
		a_view->CreateObject(&MIC);

		RE::GFxValue	fnValue;
		a_view->CreateFunction(&fnValue, getReferenceInfo);
		MIC.SetMember("MICScaleform_GetReferenceInfo", fnValue);
		
		RE::GFxValue	fnValue2;
		a_view->CreateFunction(&fnValue2, getIniOptions);
		MIC.SetMember("MICScaleform_GetIniOptions", fnValue2);

		RE::GFxValue	fnValue3;
		a_view->CreateFunction(&fnValue3, retrieveExtraData);
		MIC.SetMember("MICScaleform_RetrieveExtraData", fnValue3);

		RE::GFxValue	fnValue4;
		a_view->CreateFunction(&fnValue4, getExtraData);
		MIC.SetMember("MICScaleform_GetExtraData", fnValue4);
		
		RE::GFxValue	fnValue5;
		a_view->CreateFunction(&fnValue5, MICScaleformlog);
		MIC.SetMember("MICScaleform_Log", fnValue5);

		//Store object with functions
		globals.SetMember("MIC", MIC);
	}

	return true;
}