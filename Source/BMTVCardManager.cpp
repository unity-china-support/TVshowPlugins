//
//  BMTVCardManager.cpp
//  TVshowPlugins
//
//  Created by Zhipeng Sun on 17/3/9.
//  Copyright © 2017年 Zhipeng Sun. All rights reserved.
//

#include "BMTVCardManager.hpp"

BMTVCardManager* BMTVCardManager::s_instance = nullptr;

BMTVCardManager* BMTVCardManager::GetBMManager()
{
	if (s_instance == nullptr)
	{
		s_instance = new BMTVCardManager();
	}
	return s_instance;
}

bool BMTVCardManager::Initialize()
{
#ifdef _WINDOWS
	bool bSuccess = false;
	IDeckLinkIterator* pDLIterator = NULL;
	// Initialize COM
	HRESULT result;
	result = CoInitialize(NULL);
	if (FAILED(result))
	{
		printf("Initialization of COM failed. Application initialization Error.");
		return false;
	}

	result = CoCreateInstance(CLSID_CDeckLinkIterator, NULL, CLSCTX_ALL, IID_IDeckLinkIterator, (void**)&pDLIterator);
	if (FAILED(result))
	{
		printf("Please install the Blackmagic DeckLink drivers to use the features of this application. This application requires the DeckLink drivers installed.");
		return false;
	}
	if (pDLIterator->Next(&m_deckLink) != S_OK)
	{
		printf("You will not be able to use the features of this application until a DeckLink device is installed. This application requires a DeckLink device.");
		goto error;
	}

	bSuccess = true;
error:
	if (!bSuccess)
	{
		if (m_deckLink != NULL)
		{
			m_deckLink->Release();
			m_deckLink = NULL;
		}
	}

	if (pDLIterator != NULL)
	{
		pDLIterator->Release();
		pDLIterator = NULL;
	}
#else
    m_deckLinkIterator = CreateDeckLinkIteratorInstance();
    while(m_deckLinkIterator->Next(&m_deckLink) != S_OK)
        return false;
#endif
    return bSuccess;
}
/*
bool BMTVCardManager::CreateDeckLinkInput()
{
    m_deckLinkInput = new DeckLinkInputDelegate(m_deckLink);
    m_deckLinkInput->Initialize();
    return true;
}
*/
bool BMTVCardManager::CreateDeckLinkOutput()
{
    m_deckLinkOutput = new DeckLinkOutputDelegate(m_deckLink);
	if (!m_deckLinkOutput->Initialize())
		goto error;

    return true;
error:
	if (m_deckLinkOutput)
	{
		delete m_deckLinkOutput;
		m_deckLinkOutput = nullptr;
	}
	if (m_deckLink)
	{
		m_deckLink->Release();
		m_deckLinkOutput = nullptr;
	}
	return false;
}
