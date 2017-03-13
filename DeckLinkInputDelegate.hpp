//
//  DeckLinkInputDelegate.hpp
//  TVshowPlugins
//
//  Created by Zhipeng Sun on 17/3/9.
//  Copyright © 2017年 Zhipeng Sun. All rights reserved.
//

#ifndef DeckLinkInputDelegate_hpp
#define DeckLinkInputDelegate_hpp

#include <stdio.h>
#include "DeckLinkAPI.h"

class DeckLinkInputDelegate : public IDeckLinkInputCallback
{
public:
    DeckLinkInputDelegate(IDeckLink* deckLink);
    ~DeckLinkInputDelegate();
    
    bool Initialize();
    bool Deinitialize();

    //IDeckLinkInputCallback
    virtual HRESULT QueryInterface(REFIID iid, LPVOID *ppv) { return E_NOINTERFACE; }
    virtual ULONG AddRef(void);
    virtual ULONG  Release(void);
    virtual HRESULT VideoInputFormatChanged(BMDVideoInputFormatChangedEvents, IDeckLinkDisplayMode*, BMDDetectedVideoInputFormatFlags);
    virtual HRESULT VideoInputFrameArrived(IDeckLinkVideoInputFrame*, IDeckLinkAudioInputPacket*);
    
private:
    IDeckLink*                      m_deckLink;
    IDeckLinkInput*                 m_deckLinkInput;
    IDeckLinkDisplayMode*           m_displayMode;
    //ULONG				m_refCount;
    //pthread_mutex_t		m_mutex;
};

#endif /* DeckLinkInputDelegate_hpp */
