#include <stdio.h>
#include <conio.h>
 
 
void func1(){
printf("\nYou are inside func1");
int var1=2;
var1++;
var1--;
var1=var1++;
return;
}
 
void func2(){
printf("\nYou are inside func2");
int var1=3;
var1--;
var1=var1++;
return;
}
 
int main()
{
printf("\n I am inside main, lets call a function");
func1();
getch();
return 0;
}
