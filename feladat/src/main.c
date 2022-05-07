#include "app.h"

#include <stdio.h>

/**
 * Main function
 */
int main(int argc, char *argv[])
{
    printf("INIT: %d %s\n", argc, *argv);
    App app;

    init_app(&app, 1600, 900);
    while (app.is_running)
    {
        handle_app_events(&app);
        update_app(&app);
        render_app(&app);
    }
    destroy_app(&app);

    return 0;
}
