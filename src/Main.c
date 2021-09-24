#include "RenderGUI.h"
#include "stdlib.h"

int main(int argc, char *argv[])
{
    int res = 300;
    if (argc == 2)
        res = atoi(argv[1]);
    startRender(res);
    return 0;
}
