#include "server_manager.h"

int main()
{
    int res;
    char c;

    Application* app = CreateServerApplication ();
    RunApplication(app);

    while (c = getchar())
    {
        switch (c)
        {
            case 'q':
                PauseServer(app->m_server);
                break;
        }
        if(c == 'q')
        {break;}
    }

    DestroyServerApplication(app);
}
