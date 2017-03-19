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
#include "DeckLinkAPI_h.h"

class DeckLinkInputDelegate : public IDeckLinkInputCallback
{
public:
    DeckLinkInputDelegate(IDeckLink* deckLink);
    ~DeckLinkInputDelegate();
    
    bool Initialize();
    bool Deinitialize();

    //IDeckLinkInputCallback
	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface(REFIID iid, LPVOID *ppv);
	virtual ULONG	STDMETHODCALLTYPE	AddRef();
	virtual ULONG	STDMETHODCALLTYPE	Release();

	// IDeckLinkInputCallback interface
	virtual HRESULT STDMETHODCALLTYPE	VideoInputFormatChanged(/* in */ BMDVideoInputFormatChangedEvents notificationEvents, /* in */ IDeckLinkDisplayMode *newDisplayMode, /* in */ BMDDetectedVideoInputFormatFlags detectedSignalFlags);
	virtual HRESULT STDMETHODCALLTYPE	VideoInputFrameArrived(/* in */ IDeckLinkVideoInputFrame* videoFrame, /* in */ IDeckLinkAudioInputPacket* audioPacket);
    
private:
    IDeckLink*                      m_deckLink;
    IDeckLinkInput*                 m_deckLinkInput;
    IDeckLinkDisplayMode*           m_displayMode;
    ULONG							m_refCount;
    //pthread_mutex_t		m_mutex;
};

#endif /* DeckLinkInputDelegate_hpp */
