//
//  DeckLinkOutputDelegate.hpp
//  TVshowPlugins
//
//  Created by Zhipeng Sun on 17/3/9.
//  Copyright © 2017年 Zhipeng Sun. All rights reserved.
//

#ifndef DeckLinkOutputDelegate_hpp
#define DeckLinkOutputDelegate_hpp

#include <stdio.h>
#include "DeckLinkAPI_h.h"

class DeckLinkOutputDelegate : public IDeckLinkVideoOutputCallback
{
public:
    DeckLinkOutputDelegate (IDeckLink*	deckLink);
    ~DeckLinkOutputDelegate ();
    
    bool Initialize();
    bool Clearup();
    char* GetFrameBuffer();

	char* BeginSceneUpdate();
	void EndSceneUpdate();

	unsigned int GetFrameWidth() { return m_uiFrameWidth; }
	unsigned int GetFrameHeight() { return m_uiFrameHeight; }
    
    // IUnknown needs only a dummy implementation
	virtual HRESULT	STDMETHODCALLTYPE	QueryInterface(REFIID iid, LPVOID *ppv);
	virtual ULONG	STDMETHODCALLTYPE	AddRef();
	virtual ULONG	STDMETHODCALLTYPE	Release();
    
	virtual HRESULT	STDMETHODCALLTYPE	ScheduledFrameCompleted(IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result);
	virtual HRESULT	STDMETHODCALLTYPE	ScheduledPlaybackHasStopped();

private:
    void ResetFrame();
    void SetPreroll();

private:
#ifdef _WINDOWS
	CRITICAL_SECTION				m_pMutex;
#endif
    IDeckLink*                      m_deckLink;
    IDeckLinkOutput*                m_deckLinkOutput;
    IDeckLinkMutableVideoFrame*     m_pDLVideoFrame;
    BMDTimeValue                    m_frameDuration;
    BMDTimeScale                    m_frameTimescale;
	ULONG							m_refCount;
	unsigned int                    m_uiFrameWidth;
    unsigned int                    m_uiFrameHeight;
	unsigned int                    m_uiFPS;
	unsigned int                    m_uiTotalFrames;
    char*                           m_pFrameBuf;
    bool                            m_IsSceneDirty;
    
};

#endif /* DeckLinkOutputDelegate_hpp */
