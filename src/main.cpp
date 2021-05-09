#include <vitasdk.h>
#include <taihen.h>
#include <common/debugScreen.h>

#define printf psvDebugScreenPrintf

int currentSelection = 0;

#define pluginPath "ux0:app/LOADCOMP0/sce_module/loaderCompanion.suprx"
#define kernelPluginPath "ux0:app/LOADCOMP0/sce_module/LoaderCompanionKernel.skprx"

const char *Selections[] = { "Load modules" };
#define SelectionLength sizeof(Selections) / sizeof(const char *)

void printSelection()
{
    psvDebugScreenClear(0);
    printf("Welcome to Loader Companion!\n");
    printf("Use the D Pad to move and X to select, Press start to exit.\n\n");
    for (int i = 0; i < SelectionLength; i++)
    {
        if(i == currentSelection)
            printf("[*]");
        else printf("[]");

        printf(Selections[i]);
        printf("\n");
    }
}

int checkFileExist(const char * file)
{
    SceUID fileo = sceIoOpen(file, SCE_O_RDONLY, 0);
    sceIoClose(fileo);
    if(fileo < 0) return 0;
    return 1;
}


void loadPlugin()
{
    printf("Will load the plugins!\n");
    int res = taiLoadStartKernelModule(kernelPluginPath, 0, NULL, 0);
    if(res > 0)
        printf("Success!\n");

    SceUID id;
    sceAppMgrGetIdByName(&id, "NPXS19999");
    res = taiLoadStartModuleForPid(id, pluginPath, 0, NULL, 0);
    if(res > 0)
        printf("Success!\n");
    
}

int main()
{
    psvDebugScreenInit();
    psvDebugScreenSetFont(psvDebugScreenScaleFont2x(psvDebugScreenGetFont()));
    
    printSelection();
    SceCtrlData data;

    do
    {
        int res = sceCtrlPeekBufferPositive(0, &data, 1);
        if(res < 0)
        {
            printf("ERROR GETTING INPUT: %x\n", res);
            sceKernelDelayThread(10000000);
            sceKernelExitProcess(0);
        }
        if(data.buttons & SCE_CTRL_UP) 
        {
            currentSelection--;
            if(currentSelection > SelectionLength - 1) currentSelection = SelectionLength - 1;
            if(currentSelection < 0) currentSelection = 0;
            printSelection();
            sceKernelDelayThread(100000);
        }
        if(data.buttons & SCE_CTRL_DOWN) 
        {
            currentSelection++;
            if(currentSelection > SelectionLength - 1) currentSelection = SelectionLength - 1;
            if(currentSelection < 0) currentSelection = 0;
            printSelection();
            sceKernelDelayThread(100000);
        }
        if(data.buttons & SCE_CTRL_CROSS)
        {
            if(currentSelection == 0) 
            {
                loadPlugin();
                break;
            }
        }
    }
    while (!(data.buttons & SCE_CTRL_START));

    printf("Exiting...\n");
    sceKernelExitProcess(0);
}