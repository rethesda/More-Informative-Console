#pragma once
#include "RE/Skyrim.h"
#include "ExtraInfoEntry.h"

void ProcessContainerChanges(ExtraInfoEntry* resultArray, RE::BSExtraData* data, RE::TESObjectREFR* refForm);
void GetEquipment(ExtraInfoEntry* resultArray, RE::ExtraContainerChanges* containerChanges, RE::Actor* actor);
RE::TESForm* FindEquipedItemInSlot(int slotMask, RE::ExtraContainerChanges* containerChanges);

void GetInventory(ExtraInfoEntry* resultArray, RE::ExtraContainerChanges* containerChanges, RE::TESContainer* baseContainer);
int GetNumberOfItemInContainer(RE::TESContainer* container, RE::TESForm* item);
bool InventoryChangesContainsItem(RE::InventoryChanges* inventoryChanges, RE::TESForm* item);