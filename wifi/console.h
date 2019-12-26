#pragma once

#include "hostednetwork.h"

/// A simple console helper to take commands and start the "soft AP"
class SimpleConsole : public IWlanHostedNetworkListener {
public:
    SimpleConsole();
    virtual ~SimpleConsole();


    // IWlanHostedNetworkListener Implementation

    void RunConsole(char** argv);

    virtual void OnDeviceConnected(std::wstring remoteHostName);

    virtual void OnAdvertisementStarted();
    virtual void OnAdvertisementStopped(std::wstring message);
    virtual void OnAdvertisementAborted(std::wstring message);

    virtual void OnAsyncException(std::wstring message);

    virtual void LogMessage(std::wstring message);

private:
    void ShowPrompt();
    void ShowHelp();
    bool ExecuteCommand(std::wstring command, std::wstring arg);

    WlanHostedNetworkHelper _hostedNetwork;

    // Event helper to wait on async operations in console
    Microsoft::WRL::Wrappers::Event _apEvent;
};
