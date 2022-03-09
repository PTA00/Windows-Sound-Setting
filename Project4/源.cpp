#include <stdio.h>
#include <windows.h> 
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>

/// <summary>
/// 设置音量级别和静音状态
/// </summary>
/// <param name="level">音量级别0~100</param>
/// <param name="mute">1为静音 0为非静音</param>
/// <returns>正确时返回true</returns>
bool SetVolumeLevel(int level,int mute)
{
    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator = 0;
    IMMDevice* pDevice = 0;
    IAudioEndpointVolume* pAudioEndpointVolume = 0;
    IAudioClient* pAudioClient = 0;

    try {
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
        if (FAILED(hr)) throw "CoCreateInstance";
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
        if (FAILED(hr)) throw "GetDefaultAudioEndpoint";
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);
        if (FAILED(hr)) throw "pDevice->Active";
        hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
        if (FAILED(hr)) throw "pDevice->Active";

        hr = pAudioEndpointVolume->SetMute(mute, NULL);
        if (FAILED(hr)) throw "SetMute";

        float fVolume;
        fVolume = level / 100.0f;
        hr = pAudioEndpointVolume->SetMasterVolumeLevelScalar(fVolume, &GUID_NULL);
        if (FAILED(hr)) throw "SetMasterVolumeLevelScalar";

        pAudioClient->Release();
        pAudioEndpointVolume->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();
        return true;

    }
    catch (...) {
        if (pAudioClient) pAudioClient->Release();
        if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
        if (pDevice) pDevice->Release();
        if (pDeviceEnumerator) pDeviceEnumerator->Release();
        throw;
    }
    return false;
}


/// <summary>
/// 获取音量级别和静音状态
/// </summary>
/// <param name="level">音量级别0~100</param>
/// <param name="mute">1为静音 0为非静音</param>
/// <returns>正确时返回true</returns>
bool GetVolumeLevel(int *level,int *mute)
{
    HRESULT hr;
    IMMDeviceEnumerator* pDeviceEnumerator = 0;
    IMMDevice* pDevice = 0;
    IAudioEndpointVolume* pAudioEndpointVolume = 0;
    IAudioClient* pAudioClient = 0;

    try {
        hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), (void**)&pDeviceEnumerator);
        if (FAILED(hr)) throw "CoCreateInstance";
        hr = pDeviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &pDevice);
        if (FAILED(hr)) throw "GetDefaultAudioEndpoint";
        hr = pDevice->Activate(__uuidof(IAudioEndpointVolume), CLSCTX_ALL, NULL, (void**)&pAudioEndpointVolume);
        if (FAILED(hr)) throw "pDevice->Active";
        hr = pDevice->Activate(__uuidof(IAudioClient), CLSCTX_ALL, NULL, (void**)&pAudioClient);
        if (FAILED(hr)) throw "pDevice->Active";

        int pbMute;
        hr = pAudioEndpointVolume->GetMute(&pbMute);
        if (FAILED(hr)) throw "GetMute";

        float pfLevel;
        hr = pAudioEndpointVolume->GetMasterVolumeLevelScalar(&pfLevel);
        if (FAILED(hr)) throw "GetMasterVolumeLevelScalar";

        pAudioClient->Release();
        pAudioEndpointVolume->Release();
        pDevice->Release();
        pDeviceEnumerator->Release();

        *mute = pbMute;
        *level = 100 * pfLevel;
        return true;
        
    }
    catch (...) {
        if (pAudioClient) pAudioClient->Release();
        if (pAudioEndpointVolume) pAudioEndpointVolume->Release();
        if (pDevice) pDevice->Release();
        if (pDeviceEnumerator) pDeviceEnumerator->Release();
        throw;
    }
    return false;
}

int main()
{
    CoInitialize(0);
    try {
        int b, i;//b是静音状态，i是音量
        for (int a = 0; a < 9; ++a) {
            Sleep(500);
            GetVolumeLevel(&i, &b);
            printf("level=%d\nmute=%d\n", i, b);
            SetVolumeLevel(i, !b);
        }

    }
    catch (...) {
        //错误处理...
    }
    CoUninitialize();
}