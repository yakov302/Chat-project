#include "server_manager.h"

int main()
{
    Application* app = CreateServerApplication ();
    RunApplication(app);
}
