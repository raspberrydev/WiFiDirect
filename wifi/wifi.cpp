#include "stdafx.h"
#include "console.h"
#include "hostednetwork.h"

using namespace ABI::Windows::Foundation;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

using namespace std;

int main(int argc, char** argv)
{
    // Initialize the Windows Runtime.
    RoInitializeWrapper initialize(RO_INIT_MULTITHREADED);
    if (FAILED(initialize))
    {
        std::cout << "Failed to initialize Windows Runtime" << std::endl;
        return static_cast<HRESULT>(initialize);
    }

    SimpleConsole console;

    console.RunConsole(argv);

    return 0;
}

