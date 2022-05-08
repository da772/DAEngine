#include "dapch.h"
#include "maths.h"
#include <math.h> 

namespace da::core::maths
{
	bool IsPrime(const int& n)
	{
		int sqr = (int)sqrt(n);
		for (int i = 2; i <= sqr; i++) {
			if (n % i == 0)
				return false;
		}
		return true;
	}

	int NextPrime(const int& n)
	{
		int prime = n + 1;
		while (!IsPrime(prime)) {
			prime++;
		}
		return prime;
	}

	int PreviousPrime(const int& n)
	{
		int prime = n - 1;
		while (!IsPrime(prime)) {
			prime--;
		}
		return prime;
	}

}