//
//  BMTVCardManager.hpp
//  TVshowPlugins
//
//  Created by Zhipeng Sun on 17/3/9.
//  Copyright © 2017年 Zhipeng Sun. All rights reserved.
//

#ifndef BMTVCardManager_hpp
#define BMTVCardManager_hpp

#include <stdio.h>
#include "DeckLinkAPI_h.h"
//#include "DeckLinkInputDelegate.hpp"
#include "DeckLinkOutputDelegate.hpp"

#ifdef TVSHOWPLUGIN_EXPORTS
#define DLL_EXPORT __declspec(dllexport)
#else
#define DLL_EXPORT 
#endif

class BMTVCardManager
{
	static BMTVCardManager* s_instance;
public:
	static BMTVCardManager* GetBMManager();
    bool Initialize();
    //bool CreateDeckLinkInput();
    bool CreateDeckLinkOutput();
	DeckLinkOutputDelegate* GetOutputDL() { return m_deckLinkOutput; }

    IDeckLink*                      m_deckLink;
    //DeckLinkInputDelegate*          m_deckLinkInput;
    DeckLinkOutputDelegate*          m_deckLinkOutput;
};

extern "C" {
	DLL_EXPORT bool BMD_DLOutput_Initialize()
	{
		if (!BMTVCardManager::GetBMManager()->Initialize())
			return false;
		if (!BMTVCardManager::GetBMManager()->CreateDeckLinkOutput())
			return false;
		return true;
	}

	DLL_EXPORT unsigned int BMD_DLOutput_GetFrameWidth()
	{
		DeckLinkOutputDelegate* outputDL = BMTVCardManager::GetBMManager()->GetOutputDL();
		if (outputDL)
		{
			return outputDL->GetFrameWidth();
		}
		return 0;
	}

	DLL_EXPORT unsigned int BMD_DLOutput_GetFrameHeight()
	{
		DeckLinkOutputDelegate* outputDL = BMTVCardManager::GetBMManager()->GetOutputDL();
		if (outputDL)
		{
			return outputDL->GetFrameHeight();
		}
		return 0;
	}

	DLL_EXPORT char* BMD_DLOutput_BeginSceneUpdate()
	{
		DeckLinkOutputDelegate* outputDL = BMTVCardManager::GetBMManager()->GetOutputDL();
		if (outputDL)
		{
			return outputDL->BeginSceneUpdate();
		}
		return nullptr;
	}
	
	DLL_EXPORT void BMD_DLOutput_EndSceneUpdate()
	{
		DeckLinkOutputDelegate* outputDL = BMTVCardManager::GetBMManager()->GetOutputDL();
		if (outputDL)
		{
			outputDL->EndSceneUpdate();
		}
	}
	
}

#endif /* BMTVCardManager_hpp */
