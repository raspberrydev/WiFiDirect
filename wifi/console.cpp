#include "stdafx.h"
#include "console.h"
#include "hostednetwork.h"
#include <string>
#include <fstream>
#include <iostream>
#include <sstream> //std::stringstream
using namespace std;

SimpleConsole::SimpleConsole()
    : _apEvent(CreateEventEx(nullptr, nullptr, 0, WRITE_OWNER | EVENT_ALL_ACCESS))
{
    HRESULT hr = _apEvent.IsValid() ? S_OK : HRESULT_FROM_WIN32(GetLastError());
    if (FAILED(hr))
    {
        std::wcout << "Failed to create AP event: " << hr << std::endl;
        throw WlanHostedNetworkException("Create event failed", hr);
    }

    _hostedNetwork.RegisterListener(this);
}

SimpleConsole::~SimpleConsole()
{
}

std::wstring s2ws(const std::string& str)
{
    int size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
    std::wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

void SimpleConsole::RunConsole(char** argv)
{
    std::wstring command;
    bool r = true;

    std::ifstream inFile;
    inFile.open("ssid"); //open the input file

    std::stringstream strStream;
    strStream << inFile.rdbuf(); //read the file
    std::string ssid = strStream.str(); //str holds the content of the file

    std::ifstream inFilealt;
    inFilealt.open("pass"); //open the input file

    std::stringstream strStreamalt;
    strStreamalt << inFilealt.rdbuf(); //read the file
    std::string pass = strStreamalt.str(); //str holds the content of the file


    r = ExecuteCommand(L"ssid", s2ws(ssid));
    r = ExecuteCommand(L"pass", s2ws(pass));
    r = ExecuteCommand(L"start", L"");
    getline(std::wcin, command);
}

void SimpleConsole::OnDeviceConnected(std::wstring remoteHostName)
{
    std::wcout << std::endl << "Peer connected: " << remoteHostName << std::endl;
}

void SimpleConsole::OnAdvertisementStarted()
{
    std::wcout << "Soft AP started!" << std::endl
        << "Peers can connect to: " << _hostedNetwork.GetSSID() << std::endl
        << "Passphrase: " << _hostedNetwork.GetPassphrase() << std::endl;
    SetEvent(_apEvent.Get());
}

void SimpleConsole::OnAdvertisementStopped(std::wstring message)
{
    std::wcout << "Soft AP stopped." << std::endl;
    SetEvent(_apEvent.Get());
}

void SimpleConsole::OnAdvertisementAborted(std::wstring message)
{
    std::wcout << "Soft AP aborted: " << message << std::endl;
    SetEvent(_apEvent.Get());
}

void SimpleConsole::OnAsyncException(std::wstring message)
{
    std::wcout << std::endl << "Caught exception in asynchronous method: " << message << std::endl;
}

void SimpleConsole::LogMessage(std::wstring message)
{
    std::wcout << std::endl << message << std::endl;
}

bool SimpleConsole::ExecuteCommand(std::wstring command, std::wstring arg)
{
    // Simple command parsing logic

    if (command == L"start")
    {
        std::wcout << std::endl << "Starting soft AP..." << std::endl;
        _hostedNetwork.Start();
        WaitForSingleObjectEx(_apEvent.Get(), INFINITE, FALSE);
    }
    else if (0 == command.compare(0, 4, L"ssid"))
    {
        // Parse the SSID as the first non-space character after ssid
        std::wstring ssid = arg;

        std::wcout << std::endl << "Setting SSID to " << ssid << std::endl;
        _hostedNetwork.SetSSID(ssid);

    }
    else if (0 == command.compare(0, 4, L"pass"))
    {
        // Parse the Passphrase as the first non-space character after pass
        std::wstring passphrase = arg;


        std::wcout << std::endl << "Setting Passphrase to " << passphrase << std::endl;
        _hostedNetwork.SetPassphrase(passphrase);

    }

    return true;
}