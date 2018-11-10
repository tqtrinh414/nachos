#include "syscall.h"

int main()
{
	int a[100], n, i, j, tmp, q;
PrintString("Mang co bao nhieu phan tu: ");
	n =ReadInt();
while(n <= 0 || n >100)
	{
PrintString("Nhap Sai Moi Nhap Lai \n");
PrintString("Mang co bao nhieu phan tu: ");
	n = ReadInt();
	}
for(i =0;i<n;i++)
	{
	PrintString("Phan tu thu ");
	PrintInt(i);
	PrintString(" la: ");
	a[i] = ReadInt();
	}
for (i = 0; i < n - 1; i++)
	for (j = n - 1; j > i; j--)
		if (a[j] < a[j - 1])
	{
		tmp = a[j];
		a[j] = a[j-1];
		a[j-1] = tmp;
	}
for(q =0;q<n;q++)
	{
		PrintInt(a[q]);
		PrintString("\t");
	}
	Exit(0);
}
