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
#include "DeckLinkAPI.h"

class DeckLinkOutputDelegate : public IDeckLinkVideoOutputCallback
{
public:
    DeckLinkOutputDelegate (IDeckLink*	deckLink);
    ~DeckLinkOutputDelegate ();
    
    bool Initialize();
    bool Deinitialize();
    char* GetFrameBuffer();

    void SceneDirty();
    void Update();
    
    // IUnknown needs only a dummy implementation
    virtual HRESULT	QueryInterface (REFIID /*iid*/, LPVOID* /*ppv*/)	{return E_NOINTERFACE;}
    virtual ULONG	AddRef ()									{return 1;}
    virtual ULONG	Release ()									{return 1;}
    
    virtual HRESULT	ScheduledFrameCompleted (IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result);
    virtual HRESULT	ScheduledPlaybackHasStopped ();

private:
    void ResetFrame();
    void SetPreroll();

private:
    IDeckLink*                      m_deckLink;
    IDeckLinkOutput*                m_deckLinkOutput;
    IDeckLinkDisplayMode*           m_displayMode;
    IDeckLinkMutableVideoFrame*     m_pDLVideoFrame;
    BMDTimeValue                    m_frameDuration;
    BMDTimeScale                    m_frameTimescale;
    uint32_t                        m_uiFrameWidth;
    uint32_t                        m_uiFrameHeight;
    uint32_t                        m_uiFPS;
    uint32_t                        m_uiTotalFrames;
    char*                           m_pFrameBuf;
    bool                            m_IsSceneDirty;
    
};

#endif /* DeckLinkOutputDelegate_hpp */
