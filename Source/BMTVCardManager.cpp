//
//  BMTVCardManager.cpp
//  TVshowPlugins
//
//  Created by Zhipeng Sun on 17/3/9.
//  Copyright © 2017年 Zhipeng Sun. All rights reserved.
//

#include "BMTVCardManager.hpp"

bool BMTVCardManager::Initialize()
{
    m_deckLinkIterator = CreateDeckLinkIteratorInstance();
    while(m_deckLinkIterator->Next(&m_deckLink) != S_OK)
        return false;
    
    return true;
}

bool BMTVCardManager::CreateDeckLinkInput()
{
    m_deckLinkInput = new DeckLinkInputDelegate(m_deckLink);
    m_deckLinkInput->Initialize();
    return true;
}

bool BMTVCardManager::CreateDeckLinkOutput()
{
    m_deckLinkOutput = new DeckLinkOutputDelegate(m_deckLink);
    m_deckLinkOutput->Initialize();
    return true;
}
