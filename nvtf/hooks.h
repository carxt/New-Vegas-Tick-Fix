#pragma once
#include "SafeWrite.h"

void ResizeHashTables()
{
	if (g_bResizeHashtables) {
		SafeWrite32(0x473F69, 5009); // NiTMap<unsigned int,TESFile *> - Owned Temp ID Map (ONAM)

		SafeWrite32(0x6B5C76, 10037); // NiTPointerMap<unsigned int,NavMeshInfo *> - NavMeshInfoMap
		SafeWrite32(0x6B7A30, 2819); // NiTMap<unsigned int,BSSimpleArray<NavMeshInfo *,1024> *> - NavMeshInfoMap's world NavMeshInfos

		SafeWrite32(0x845558, 7057); // NiTPointerMap<unsigned int,BGSFormChanges *> - BGSSaveLoadChangesMap
		SafeWrite32(0x846FFB, 12049); // BGSSaveLoadFormIDMap - BGSSaveLoadGame's FormID map (constructor)
		SafeWrite32(0x848072, 12049); // BGSSaveLoadFormIDMap - BGSSaveLoadGame's FormID map (save loader)
		SafeWrite8(0x84703E, 59); // BGSSaveLoadFormIDMap - BGSSaveLoadGame's worldspace FormID map
		SafeWrite8(0x8470FA, 59); // NiTMap<unsigned int,unsigned int> - BGSSaveLoadGame's changed FormID map
		SafeWrite8(0x84AB60, 127); // NiTMap<__int64,int> - BGSSaveLoadGame's expired cell map

		SafeWrite8(0x544FA7, 41); // NiTMap<TESObjectREFR *,NiNode *> - TESObjectCELL's animated references
		SafeWrite8(0x544FC9, 29);// NiTMap<TESObjectREFR *,NiNode *> - TESObjectCELL's external emittance objects

		SafeWrite8(0x582CA2, 127); // NiTPointerMap<unsigned int,BSSimpleList<TESObjectREFR *> *> - TESWorldSpace's fixed persistent references
		SafeWrite8(0x582CEF, 53); // NiTMap<TESFile *,TESWorldSpace::OFFSET_DATA *> - TESWorldSpace's file offset map
		SafeWrite32(0x583FF6, 1709); // NiTPointerMap<int,TESObjectCELL *> - TESWorldSpace's cell map (form loader)
		SafeWrite8(0x582D64, 31); // NiTPointerMap<int,TESObjectCELL *> - TESWorldSpace's cell map (constructor)
		SafeWrite8(0x587AC9, 43); // NiTPointerMap<unsigned int,BSSimpleList<TESObjectREFR *> *> - TESWorldSpace's overlapped multibounds

		SafeWrite8(0x6C02F8, 127); // NiTMap<unsigned int,NiPointer<ReferenceObstacleArray>> - NavMeshObstacleManager's obstacle map
		SafeWrite8(0x6C035F, 97); // NiTMap<bhkRigidBody *,NiPointer<ObstacleData>> - NavMeshObstacleManager's obstacle data map
		SafeWrite8(0x6C0397, 97); // NiTMap<unsigned int,NiPointer<ReferenceObstacleArray>> - NavMeshObstacleManager's open doors map
		SafeWrite8(0x6C03AB, 89); // NiTMap<unsigned int,NiPointer<ReferenceObstacleArray>> - NavMeshObstacleManager's closed doors map

		SafeWrite8(0x6E13AF, 53); // NiTMap<unsigned int,unsigned int> - PathingLOSMap

		SafeWrite8(0xAD9169, 113); // NiTMapBase<NiTPointerAllocator<unsigned int>,int,BSGameSound *> - BSAudioManager's playing sounds
		SafeWrite8(0xAD9189, 113); // NiTMapBase<NiTPointerAllocator<unsigned int>,int,BSSoundInfo *> - BSAudioManager's playing sound infos
		SafeWrite8(0xAD91CC, 41); // NiTMapBase<NiTPointerAllocator<unsigned int>,int,NiPointer<NiAVObject>> - BSAudioManager's moving sounds
	}
}