
#include "CreatureAnimationClipsStore.h"
#include "CreaturePluginPCH.h"
#include "CreatureMeshComponent.h"
#include "CreatureAnimationClip.h"

void UCreatureAnimationClipsStore::LoadAnimationDataToComponent(class UCreatureMeshComponent* MeshComponent)
{

	//////////////////////////////////////////////////////////////////////////
	//警告：新版本中不能够混用两种动画模式！！！
	MeshComponent->collectionClips.Empty();
	//遍历ClipList，准备填充数据到Component
	for (FCreatureAnimationClip Clip:ClipList)
	{
		FCreatureMeshCollectionClip MeshCollectionClip = FCreatureMeshCollectionClip();
		MeshCollectionClip.collection_name = Clip.ClipName;
		//填充FCreatureMeshCollectionToken
		for (FCreatureAnimationShortClip ShortClip:Clip.ShortClipList)
		{
			//根据ShortClip中的引用，构建CollectionData
			FCreatureMeshCollection CollectionData;
			CollectionData.creature_filename = FName(*ShortClip.SourceAsset->GetName());
			//直接给出JsonString的引用，不需要再次载入
			CollectionData.creature_core.pJsonData = &(ShortClip.SourceAsset->GetJsonString());
			
			CollectionData.animation_speed = ShortClip.SourceAsset->animation_speed;
			CollectionData.collection_material = ShortClip.SourceAsset->collection_material;

			//如果当前Data已经存在于CollectionData中则直接返回
			int32 Index = MeshComponent->collectionData.AddUnique(CollectionData);
			FCreatureMeshCollection &addedCollectionData = MeshComponent->collectionData[Index];
			addedCollectionData.creature_core.pJsonData = CollectionData.creature_core.pJsonData;
			addedCollectionData.source_asset = ShortClip.SourceAsset;

			FCreatureMeshCollectionToken Token = FCreatureMeshCollectionToken();
			Token.animation_name = ShortClip.ClipNameInAsset;
			Token.collection_data_index = Index;
			MeshCollectionClip.sequence_clips.Add(Token);
		}

		MeshComponent->collectionClips.AddUnique(MeshCollectionClip);
	}
}
