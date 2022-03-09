#include <stdio.h>
#include <windows.h> 
#include <mmdeviceapi.h> 
#include <endpointvolume.h>
#include <audioclient.h>

/// <summary>
/// ������������;���״̬
/// </summary>
/// <param name="level">��������0~100</param>
/// <param name="mute">1Ϊ���� 0Ϊ�Ǿ���</param>
/// <returns>��ȷʱ����true</returns>
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
/// ��ȡ��������;���״̬
/// </summary>
/// <param name="level">��������0~100</param>
/// <param name="mute">1Ϊ���� 0Ϊ�Ǿ���</param>
/// <returns>��ȷʱ����true</returns>
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
        int b, i;//b�Ǿ���״̬��i������
        for (int a = 0; a < 9; ++a) {
            Sleep(500);
            GetVolumeLevel(&i, &b);
            printf("level=%d\nmute=%d\n", i, b);
            SetVolumeLevel(i, !b);
        }

    }
    catch (...) {
        //������...
    }
    CoUninitialize();
}