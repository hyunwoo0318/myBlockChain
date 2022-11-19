#include"IPC.h"

map<string, pair<HANDLE, const TCHAR*>> pipeInfo;

pair<HANDLE,const TCHAR*> createOwnPipe(string node1, string node2)
{
    HANDLE hPipe;
    char buffer[1024];
    DWORD dwRead;

    string pipeName = "\\\\.\\pipe\\" + node1 + "TO" + node2;
    const TCHAR* pipeNameCast = reinterpret_cast<const TCHAR*>(pipeName.c_str());

    hPipe = CreateNamedPipe(pipeNameCast,
        PIPE_ACCESS_DUPLEX,
        PIPE_TYPE_BYTE | PIPE_READMODE_BYTE | PIPE_WAIT,
        1,
        1024 * 16,
        1024 * 16,
        NMPWAIT_USE_DEFAULT_WAIT,
        NULL);
    return { hPipe, pipeNameCast };
}

string readDatas(HANDLE hPipe)
{
    string bufferString;
    char buffer[1024];
    DWORD dwRead;

       
            while (ReadFile(hPipe, buffer, sizeof(buffer) - 1, &dwRead, NULL) != FALSE)
            {
                /* add terminating zero */
                buffer[dwRead] = '\0';

                /* do something with data in buffer */
                bufferString = buffer;
            }
           
    
    return bufferString;
}

//Block -> dataType = B
//TX -> dataType = T
bool sendDatas(const TCHAR* pipeName, char* data, char dataType)
{
    HANDLE hPipe;
    DWORD dwWritten;

    string temp = data;
    int sendSize = (dataType + temp).size();
    const char* sendMsg = (dataType + temp).c_str();

    hPipe = CreateFile(pipeName,
        GENERIC_READ | GENERIC_WRITE,
        0,
        NULL,
        OPEN_EXISTING,
        0,
        NULL);
    if (hPipe != INVALID_HANDLE_VALUE)
    {
        WriteFile(hPipe,
            sendMsg,
            sendSize,  
            &dwWritten,
            NULL);
        if (WriteFile)
            return true;
        else
            return false;
    }

}

//{
//node F0, F1, F2, F3, F4, F5
//node U0, U1, U2, U3
//link U0 - F1, U1 - F2, U2 - F4, U3 - F0
//link F0 - F1, F2 - F1, F2 - F4, F3 - F4, F4 - F0, F4 - F1, F3 - F1, F2 - F5, F4 - F5
//}



/*
pipeName -> node의 종류에 따라 U와 F를 쓰고 번호에따라 (번호+1)번 해당 노드의 종류를 반복함
*/
void connectNodes()
{

    

    //craete pipe
    pair<HANDLE, const TCHAR*> pipeU0F1 = createOwnPipe("U", "FF");
    pair<HANDLE, const TCHAR*> pipeU1F2 = createOwnPipe("UU", "FFF");
    pair<HANDLE, const TCHAR*> pipeU2F3 = createOwnPipe("UUU", "FFFF");
    pair<HANDLE, const TCHAR*> pipeU3F0 = createOwnPipe("UUUU", "F");

    pair<HANDLE, const TCHAR*> pipeF0F1 = createOwnPipe("FF", "FFF");
    pair<HANDLE, const TCHAR*> pipeF1F2 = createOwnPipe("FF", "FFF");
    pair<HANDLE, const TCHAR*> pipeF2F4 = createOwnPipe("FFF", "FFFFF");
    pair<HANDLE, const TCHAR*> pipeF3F4 = createOwnPipe("FFFF", "FFFFF");
    pair<HANDLE, const TCHAR*> pipeF0F4 = createOwnPipe("F", "FFFFF");
    pair<HANDLE, const TCHAR*> pipeF1F4 = createOwnPipe("FF", "FFFFF");
    pair<HANDLE, const TCHAR*> pipeF1F3 = createOwnPipe("FF", "FFFF");
    pair<HANDLE, const TCHAR*> pipeF2F5 = createOwnPipe("FFF", "FFFFFF");
    pair<HANDLE, const TCHAR*> pipeF4F5 = createOwnPipe("FFFFF", "FFFFFF");



    //해당 map에 저장 -> key는 두 노드의 이름
    pipeInfo["U0F1"] = pipeU0F1;
    pipeInfo["U1F2"] = pipeU1F2;
    pipeInfo["U2F3"] = pipeU2F3;
    pipeInfo["U3F0"] = pipeU3F0;

    pipeInfo["F0F1"] = pipeF0F1;
    pipeInfo["F1F2"] = pipeF1F2;
    pipeInfo["F2F4"] = pipeF2F4;
    pipeInfo["F3F4"] = pipeF3F4;
    pipeInfo["F0F4"] = pipeF0F4;
    pipeInfo["F1F4"] = pipeF1F4;
    pipeInfo["F1F3"] = pipeF1F3;
    pipeInfo["F2F5"] = pipeF2F5;
    pipeInfo["F4F5"] = pipeF4F5;

}