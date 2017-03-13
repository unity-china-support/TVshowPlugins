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
#include "DeckLinkAPI.h"
#include "DeckLinkInputDelegate.hpp"
#include "DeckLinkOutputDelegate.hpp"

class BMTVCardManager
{
    bool Initialize();
    bool CreateDeckLinkInput();
    bool CreateDeckLinkOutput();
    IDeckLinkIterator*				m_deckLinkIterator;
    IDeckLink*                      m_deckLink;
    DeckLinkInputDelegate*          m_deckLinkInput;
    DeckLinkOutputDelegate*          m_deckLinkOutput;
};

extern "C" {
    bool BMD_DLOutput_Initialize ();
    char* BMD_DLOutput_GetFrameBuffer();
    BMD_DLOutput_
}

#endif /* BMTVCardManager_hpp */
