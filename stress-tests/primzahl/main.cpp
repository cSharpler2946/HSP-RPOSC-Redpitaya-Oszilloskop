#include <stdio.h>

int main()
{
    int range = 1000000;
    int x, i;

    // loop until range is reached
    for (x = 2; x <= range; x++)
    {
        // check if x can be divided trough another number without any rest
        for (i = 2; i <= x; i++)
        {
            if (x%i == 0 && x != i) break; // not a prime if division is possible

            if (i == x) printf("%i is prime.\n", x); // if no divisor is found --> prime

        }
    }
    return 0;

}
