#include<stdio.h>
#include<conio.h>
#include<windows.h>
 
void filetoarray(char *,int);
void changebyte(char *,int);
 
void filetoarray(char *filebytes,int size){
FILE *fp;
fp = fopen("print2.exe","rb");
fread(filebytes, sizeof(filebytes[0]), 29000, fp);
return;
}
 
void changebyte(char *filebytes,int size){
int offset=1844;
filebytes[offset]=0xCC;
return;
}
 
int main(){
char filebytes[29000];
PROCESS_INFORMATION pi;
STARTUPINFO si;
 
printf("\n[*]Writing Bytes to an array");
filetoarray(filebytes,29000);
printf("\n[*]Done");
Sleep(500);
 
printf("\n\n[*]Modifying byte at given offest");
changebyte(filebytes,29000);
printf("\n[*]Done");
Sleep(500);
 
printf("\n\n[*]Writing to new myf.exe");
FILE *fp2;
fp2 = fopen("myf.exe","wb");
fwrite(filebytes,sizeof(filebytes[0]),sizeof(filebytes)/sizeof(filebytes[0]),fp2);
fclose(fp2);
printf("\n[*]Done");
 
Sleep(500);
 
printf("\n\n[*]Starting myf.exe as debuggee");
memset(&si,0,sizeof(si));
memset(&pi,0,sizeof(pi));
si.cb= sizeof(si);
if(CreateProcess("myf.exe",NULL,NULL,NULL,TRUE,DEBUG_PROCESS,NULL,NULL,&si,&pi)){
printf("\n[*]Process Created");
BOOL cont = TRUE;
while (cont)
{
DWORD continueStatus = DBG_CONTINUE;
DEBUG_EVENT de = {0};
if (!WaitForDebugEvent(&de, INFINITE))
{
break;
}
else{
switch (de.dwDebugEventCode)
{
 
case EXCEPTION_DEBUG_EVENT:
switch (de.u.Exception.ExceptionRecord.ExceptionCode)
{
case EXCEPTION_BREAKPOINT:
 
if((*(static_cast<unsigned int*>(de.u.Exception.ExceptionRecord.ExceptionAddress)))== -2082109099){
printf("\n\n[*]Reached Breakpoint");
//FACE THE MUSIC
 
CONTEXT c;
c.ContextFlags = CONTEXT_CONTROL|CONTEXT_FULL;
GetThreadContext(pi.hThread,&c);
printf("\nEIP : %0X",c.Eip);
SuspendThread(pi.hThread);
c.Eip--;
SetThreadContext(pi.hThread,&c);
DWORD sa = 0x401334;
BYTE ins=0x55;
WriteProcessMemory(pi.hProcess, (void*)sa, &ins, 1,NULL);
sa = 0x28FF0C;
ins=0x59;
WriteProcessMemory(pi.hProcess, (void*)sa, &ins, 1,NULL);
sa = 0x28FF10;
ins=0x9A;
WriteProcessMemory(pi.hProcess, (void*)sa, &ins, 1,NULL);
sa = 0x28FF11;
ins=0x13;
WriteProcessMemory(pi.hProcess, (void*)sa, &ins, 1,NULL);
ResumeThread(pi.hThread);
 
 
 
}
break;
default:
break;
}
break;
 
case EXIT_PROCESS_DEBUG_EVENT:
cont = FALSE;
break;
 
case CREATE_THREAD_DEBUG_EVENT:
continueStatus = DBG_CONTINUE;
break;
 
case CREATE_PROCESS_DEBUG_EVENT:
continueStatus = DBG_CONTINUE;
break;
 
case EXIT_THREAD_DEBUG_EVENT:
continueStatus = DBG_CONTINUE;
break;
 
 
}
ContinueDebugEvent(de.dwProcessId,de.dwThreadId,continueStatus);
}
}}
getch();
return 0;
}
