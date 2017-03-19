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
#ifdef _WINDOWS
	InitializeCriticalSection(&m_pMutex);
#endif

}

DeckLinkOutputDelegate::~DeckLinkOutputDelegate ()
{
    Clearup();
}

HRESULT	DeckLinkOutputDelegate::QueryInterface(REFIID iid, LPVOID *ppv)
{
	*ppv = NULL;
	return E_NOINTERFACE;
}

ULONG	DeckLinkOutputDelegate::AddRef()
{
	return InterlockedIncrement((LONG*)&m_refCount);
}

ULONG	DeckLinkOutputDelegate::Release()
{
	ULONG			newRefValue;

	newRefValue = InterlockedDecrement((LONG*)&m_refCount);
	if (newRefValue == 0)
	{
		delete this;
		return 0;
	}

	return newRefValue;
}

bool DeckLinkOutputDelegate::Initialize()
{
    IDeckLinkDisplayModeIterator*	displayModeIterator = nullptr;
	IDeckLinkDisplayMode*           displayMode = nullptr;
    // Get the input (capture) interface of the DeckLink device
    HRESULT result = m_deckLink->QueryInterface(IID_IDeckLinkOutput, (void**)&m_deckLinkOutput);
    if (result != S_OK)
		goto bail;

    if (m_deckLinkOutput->SetScheduledFrameCompletionCallback(this) != S_OK)
		goto bail;
    
    //start
    result = m_deckLinkOutput->GetDisplayModeIterator(&displayModeIterator);
    if (result != S_OK)
		goto bail;

    if(displayModeIterator->Next(&displayMode) != S_OK)
		goto bail;
    
    m_uiFrameWidth = (unsigned int)displayMode->GetWidth();
    m_uiFrameHeight = (unsigned int)displayMode->GetHeight();
	displayMode->GetFrameRate(&m_frameDuration, &m_frameTimescale);

    m_uiFPS = (unsigned int)((m_frameTimescale + (m_frameDuration-1))  /  m_frameDuration);
	
    if (m_deckLinkOutput->EnableVideoOutput(displayMode->GetDisplayMode(), bmdVideoOutputFlagDefault) != S_OK)
		goto bail;
    
    // Flip frame vertical, because OpenGL rendering starts from left bottom corner
    if (m_deckLinkOutput->CreateVideoFrame(m_uiFrameWidth, (unsigned int)m_uiFrameHeight, (unsigned int)m_uiFrameWidth * 4, bmdFormat8BitBGRA, bmdFrameFlagFlipVertical, &m_pDLVideoFrame) != S_OK)
		goto bail;
    
    ResetFrame();
    SetPreroll();
    m_pFrameBuf = (char*)malloc(m_pDLVideoFrame->GetRowBytes() * m_uiFrameWidth);
    memset(m_pFrameBuf, 0x00, m_pDLVideoFrame->GetRowBytes() * m_uiFrameWidth);
    
    m_deckLinkOutput->StartScheduledPlayback(0, m_frameTimescale, 1.0);

    return true;
bail:
	if (displayModeIterator)
	{
		displayModeIterator->Release();
		displayModeIterator = nullptr;
	}
	if (displayMode)
	{
		displayMode->Release();
		displayMode = nullptr;
	}
	Clearup();
	return false;
}

char* DeckLinkOutputDelegate::GetFrameBuffer()
{
    return m_pFrameBuf;
}

bool DeckLinkOutputDelegate::Clearup()
{
    if (m_deckLinkOutput != NULL)
    {
        m_deckLinkOutput->Release();
        m_deckLinkOutput = NULL;
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
#ifdef _WINDOWS
	DeleteCriticalSection(&m_pMutex);
#endif
    return true;
}

char* DeckLinkOutputDelegate::BeginSceneUpdate()
{
#ifdef _WINDOWS
	EnterCriticalSection(&m_pMutex);
#endif
    m_IsSceneDirty = true;

	return m_pFrameBuf;
}

void DeckLinkOutputDelegate::EndSceneUpdate()
{
#ifdef _WINDOWS
	LeaveCriticalSection(&m_pMutex);
#endif
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
    for (unsigned int i=0; i < m_uiFPS; i++)
    {
        if (m_deckLinkOutput->ScheduleVideoFrame(m_pDLVideoFrame, (m_uiTotalFrames * m_frameDuration), m_frameDuration, m_frameTimescale) != S_OK)
            return;
        m_uiTotalFrames++;
    }
}

HRESULT	DeckLinkOutputDelegate::ScheduledFrameCompleted (IDeckLinkVideoFrame* completedFrame, BMDOutputFrameCompletionResult result)
{
	bool showLastFrame = true;
#ifdef _WINDOWS
	if (TryEnterCriticalSection(&m_pMutex))
	{
#endif
		if (m_IsSceneDirty)
		{

			void*	pFrame;

			m_pDLVideoFrame->GetBytes((void**)&pFrame);

			memcpy(pFrame, m_pFrameBuf, m_pDLVideoFrame->GetRowBytes() * m_uiFrameHeight);

			if (m_deckLinkOutput->ScheduleVideoFrame(m_pDLVideoFrame, (m_uiTotalFrames * m_frameDuration), m_frameDuration, m_frameTimescale) != S_OK)
			{
				//TODO Mutex.unlock();
				printf("fail to ScheduleVideoFrame");
			}
			showLastFrame = false;
		}
#ifdef _WINDOWS
		LeaveCriticalSection(&m_pMutex);
	}
#endif
	if (showLastFrame)
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
