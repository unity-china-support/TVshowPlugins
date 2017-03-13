//
//  DeckLinkOutputDelegate.cpp
//  TVshowPlugins
//
//  Created by Zhipeng Sun on 17/3/9.
//  Copyright © 2017年 Zhipeng Sun. All rights reserved.
//

#include "DeckLinkOutputDelegate.hpp"

DeckLinkOutputDelegate::DeckLinkOutputDelegate (IDeckLink*	deckLink)
: m_deckLink(deckLink)
, m_deckLinkOutput(nullptr)
, m_displayMode(nullptr)
, m_pDLVideoFrame(nullptr)
, m_frameDuration(0)
, m_frameTimescale(0)
, m_uiFrameWidth(0)
, m_uiFrameHeight(0)
, m_uiFPS(0)
, m_uiTotalFrames(0)
, m_pFrameBuf(nullptr)
, m_IsSceneDirty(false)
{
}

DeckLinkOutputDelegate::~DeckLinkOutputDelegate ()
{
    Deinitialize();
}

bool DeckLinkOutputDelegate::Initialize()
{
    IDeckLinkDisplayModeIterator*	displayModeIterator;
    // Get the input (capture) interface of the DeckLink device
    HRESULT result = m_deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&m_deckLinkOutput);
    if (result != S_OK)
        return false;

    if (m_deckLinkOutput->SetScheduledFrameCompletionCallback(this) != S_OK)
        return false;
    
    //start
    result = m_deckLinkOutput->GetDisplayModeIterator(&displayModeIterator);
    if (result != S_OK)
        return false;

    if(displayModeIterator->Next(&m_displayMode) == S_OK)
        return false;
    
    m_uiFrameWidth = (uint32_t)m_displayMode->GetWidth();
    m_uiFrameHeight = (uint32_t)m_displayMode->GetHeight();
    m_displayMode->GetFrameRate(&m_frameDuration, &m_frameTimescale);

    m_uiFPS = (uint32_t)((m_frameTimescale + (m_frameDuration-1))  /  m_frameDuration);

    if (m_deckLinkOutput->EnableVideoOutput(m_displayMode->GetDisplayMode(), bmdVideoOutputFlagDefault) != S_OK)
        return false;
    
    // Flip frame vertical, because OpenGL rendering starts from left bottom corner
    if (m_deckLinkOutput->CreateVideoFrame(m_uiFrameWidth, (int32_t)m_uiFrameHeight, (int32_t)m_uiFrameWidth * 4, bmdFormat8BitBGRA, bmdFrameFlagFlipVertical, &m_pDLVideoFrame) != S_OK)
        return false;
    
    ResetFrame();
    SetPreroll();
    m_pFrameBuf = (char*)malloc(m_pDLVideoFrame->GetRowBytes() * m_uiFrameWidth);
    memset(m_pFrameBuf, 0x00, m_pDLVideoFrame->GetRowBytes() * m_uiFrameWidth);
    
    m_deckLinkOutput->StartScheduledPlayback(0, m_frameTimescale, 1.0);

    return true;
}

char* DeckLinkOutputDelegate::GetFrameBuffer()
{
    return m_pFrameBuf;
}

bool DeckLinkOutputDelegate::Deinitialize()
{
    if (m_deckLinkOutput != NULL)
    {
        m_deckLinkOutput->Release();
        m_deckLinkOutput = NULL;
    }
    if (m_displayMode != NULL)
    {
        m_displayMode->Release();
        m_displayMode = NULL;
    }
    if (m_pDLVideoFrame != NULL)
    {
        m_pDLVideoFrame->Release();
        m_pDLVideoFrame = NULL;
    }
    if (m_pFrameBuf != NULL)
    {
        delete m_pFrameBuf;
        m_pFrameBuf = NULL;
    }

    return true;
}

void DeckLinkOutputDelegate::SceneDirty()
{
    m_IsSceneDirty = true;
}

void DeckLinkOutputDelegate::Update()
{

}

void DeckLinkOutputDelegate::ResetFrame()
{
    // Fill frame with black
    void*	pFrame;
    m_pDLVideoFrame->GetBytes((void**)&pFrame);
    memset(pFrame, 0x00, m_pDLVideoFrame->GetRowBytes() * m_uiFrameHeight);
}

void DeckLinkOutputDelegate::SetPreroll()
{
    // Set 1 second preroll
    for (uint32_t i=0; i < m_uiFPS; i++)
    {
        if (m_deckLinkOutput->ScheduleVideoFrame(m_pDLVideoFrame, (m_uiTotalFrames * m_frameDuration), m_frameDuration, m_frameTimescale) != S_OK)
            return;
        m_uiTotalFrames++;
    }
}

HRESULT	DeckLinkOutputDelegate::ScheduledFrameCompleted (IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result)
{
    if(m_IsSceneDirty)
    {
        void*	pFrame;
    
        m_pDLVideoFrame->GetBytes((void**)&pFrame);
    
        memcpy(pFrame, m_pFrameBuf, m_pDLVideoFrame->GetRowBytes() * m_uiFrameHeight);
    
        if (m_deckLinkOutput->ScheduleVideoFrame(m_pDLVideoFrame, (m_uiTotalFrames * m_frameDuration), m_frameDuration, m_frameTimescale) != S_OK)
        {
            //TODO Mutex.unlock();
            printf("fail to ScheduleVideoFrame");
        }
    }
    else
    {
        if (m_deckLinkOutput->ScheduleVideoFrame(completedFrame, (m_uiTotalFrames * m_frameDuration), m_frameDuration, m_frameTimescale) != S_OK)
        {
            //TODO Mutex.unlock();
            printf("fail to ScheduleVideoFrame");
        }
    }
    m_uiTotalFrames++;
    return S_OK;
}


HRESULT	DeckLinkOutputDelegate::ScheduledPlaybackHasStopped ()
{
    return S_OK;
}
