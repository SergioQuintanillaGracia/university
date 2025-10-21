#include <stdio.h> 

int a = 0; /* global variable */ 


// This function increases the value of global variable a by 1
void inc_a(void) {
    a++;
}

// This function returns the previous value 
// and saves the new value v
int former_value(int v) {
    int temp;
    // Declare here static variable s
    static int s;
    temp = s;
    s = v;
    return temp;
}

int main()
{
	int b = 2; /* local variable */
	inc_a();
	former_value(b);  // Save the current value of b (2)
	printf("a= %d, b= %d\n", a, b);
	a++;
	b++;  // b is now 3
	inc_a();
    b = former_value(b);  // Get the previous value of b (2) and save the current one (3)
    printf("a = %d, b = %d \n", a, b);
}
