//
//  DeckLinkInputDelegate.cpp
//  TVshowPlugins
//
//  Created by Zhipeng Sun on 17/3/9.
//  Copyright © 2017年 Zhipeng Sun. All rights reserved.
//

#include "DeckLinkInputDelegate.hpp"

DeckLinkInputDelegate::DeckLinkInputDelegate(IDeckLink*	deckLink)
: m_deckLink(deckLink)
{
    //pthread_mutex_init(&m_mutex, NULL);
}

DeckLinkInputDelegate::~DeckLinkInputDelegate()
{
    //pthread_mutex_destroy(&m_mutex);
}

bool DeckLinkInputDelegate::Initialize()
{
    IDeckLinkDisplayModeIterator*	displayModeIterator;
    // Get the input (capture) interface of the DeckLink device
    HRESULT result = m_deckLink->QueryInterface(IID_IDeckLinkInput, (void**)&m_deckLinkInput);
    if (result != S_OK)
        return false;
    
    result = m_deckLinkInput->GetDisplayModeIterator(&displayModeIterator);
    if (result != S_OK)
        return false;
    
    if(displayModeIterator->Next(&m_displayMode) == S_OK)
    if (result != S_OK)
        return false;
    
    return true;
}

bool DeckLinkInputDelegate::Deinitialize()
{
    m_displayMode->Release();
    m_deckLinkInput->Release();
    
    return true;
}

ULONG DeckLinkInputDelegate::AddRef(void)
{
    return 0;
}

ULONG DeckLinkInputDelegate::Release(void)
{
    //pthread_mutex_lock(&m_mutex);
    //m_refCount--;
    //pthread_mutex_unlock(&m_mutex);
    
    //if (m_refCount == 0)
    //{
    //    delete this;
    //    return 0;
    //}
    
    return 0;
}

HRESULT DeckLinkInputDelegate::VideoInputFrameArrived(IDeckLinkVideoInputFrame* videoFrame, IDeckLinkAudioInputPacket* audioFrame)
{
    /*
    IDeckLinkVideoFrame*				rightEyeFrame = NULL;
    IDeckLinkVideoFrame3DExtensions*	threeDExtensions = NULL;
    void*								frameBytes;
    void*								audioFrameBytes;

    // Handle Video Frame
    if (videoFrame)
    {
        // If 3D mode is enabled we retreive the 3D extensions interface which gives.
        // us access to the right eye frame by calling GetFrameForRightEye() .
        if ( (videoFrame->QueryInterface(IID_IDeckLinkVideoFrame3DExtensions, (void **) &threeDExtensions) != S_OK) ||
            (threeDExtensions->GetFrameForRightEye(&rightEyeFrame) != S_OK))
        {
            rightEyeFrame = NULL;
        }
        
        if (threeDExtensions)
            threeDExtensions->Release();

        if (videoFrame->GetFlags() & bmdFrameHasNoInputSource)
        {
            //printf("Frame received (#%lu) - No input signal detected\n", g_frameCount);
        }
        else
        {
            const char *timecodeString = NULL;
            if (g_config.m_timecodeFormat != 0)
            {
                IDeckLinkTimecode *timecode;
                if (videoFrame->GetTimecode(g_config.m_timecodeFormat, &timecode) == S_OK)
                {
                    timecode->GetString(&timecodeString);
                }
            }
            
            printf("Frame received (#%lu) [%s] - %s - Size: %li bytes\n",
                   g_frameCount,
                   timecodeString != NULL ? timecodeString : "No timecode",
                   rightEyeFrame != NULL ? "Valid Frame (3D left/right)" : "Valid Frame",
                   videoFrame->GetRowBytes() * videoFrame->GetHeight());
            
            if (timecodeString)
                free((void*)timecodeString);
            
            if (g_videoOutputFile != -1)
            {
                videoFrame->GetBytes(&frameBytes);
                write(g_videoOutputFile, frameBytes, videoFrame->GetRowBytes() * videoFrame->GetHeight());
                
                if (rightEyeFrame)
                {
                    rightEyeFrame->GetBytes(&frameBytes);
                    write(g_videoOutputFile, frameBytes, videoFrame->GetRowBytes() * videoFrame->GetHeight());
                }
            }
        }
        
        if (rightEyeFrame)
            rightEyeFrame->Release();
        
        g_frameCount++;
    }
    
    // Handle Audio Frame
    if (audioFrame)
    {
        if (g_audioOutputFile != -1)
        {
            audioFrame->GetBytes(&audioFrameBytes);
            write(g_audioOutputFile, audioFrameBytes, audioFrame->GetSampleFrameCount() * g_config.m_audioChannels * (g_config.m_audioSampleDepth / 8));
        }
    }
    
    if (g_config.m_maxFrames > 0 && videoFrame && g_frameCount >= g_config.m_maxFrames)
    {
        g_do_exit = true;
        pthread_cond_signal(&g_sleepCond);
    }
    */
    return S_OK;
}

HRESULT DeckLinkInputDelegate::VideoInputFormatChanged(BMDVideoInputFormatChangedEvents events, IDeckLinkDisplayMode* mode, BMDDetectedVideoInputFormatFlags)
{
    /*
    // This only gets called if bmdVideoInputEnableFormatDetection was set
    // when enabling video input
    HRESULT	result;
    char*	displayModeName = NULL;
    
    if (!(events & bmdVideoInputDisplayModeChanged))
        return S_OK;
    
    mode->GetName((const char**)&displayModeName);
    printf("Video format changed to %s\n", displayModeName);
    
    if (displayModeName)
        free(displayModeName);
    
    if (g_deckLinkInput)
    {
        g_deckLinkInput->StopStreams();
        
        result = g_deckLinkInput->EnableVideoInput(mode->GetDisplayMode(), g_config.m_pixelFormat, g_config.m_inputFlags);
        if (result != S_OK)
        {
            fprintf(stderr, "Failed to switch video mode\n");
            goto bail;
        }
        
        g_deckLinkInput->StartStreams();
    }
    */
    return S_OK;
}

