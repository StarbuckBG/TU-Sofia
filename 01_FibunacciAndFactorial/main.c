#include <unistd.h>
#include <stdio.h>

#define BAD_FORK_RESULT 69

unsigned long long factorial(unsigned long long number);
unsigned long long fibunacci(unsigned long long number);

int main()
{
	unsigned long long numberToCheck;
	scanf("%llu",&numberToCheck); 
	pid_t forkResult;
	forkResult = fork();
	if(forkResult == 0)
	{
		numberToCheck = factorial(numberToCheck);
		printf("Result of factorial: %llu\n", numberToCheck);
		return 0;
	}
	else if (forkResult < 0)
	{
		return BAD_FORK_RESULT;
	}
	else
	{
		numberToCheck = fibunacci(numberToCheck);
		printf("Result of fibunacci: %llu\n", numberToCheck);
		return 0;
	}

	return 0;
}

unsigned long long factorial(unsigned long long number)
{
	if(number == 0) return 0;
	else if(number == 1) return 1;
	else
	{
		for(unsigned long long i = number-1; i > 1; i--)
		{
			number *= i;
		}
	}
	return number;
}
unsigned long long fibunacci(unsigned long long number)
{
	if(number == 0) return 0;
	else if(number == 1) return 1;
	else if(number == 2) return 2;
	else
	{
		unsigned long long firstNumber = 1;
		unsigned long long secondNumber = 2;
		unsigned long long result;
		for(unsigned long long i = number; i > 2; i--)
		{
			result = firstNumber+secondNumber;
			firstNumber = secondNumber;
			secondNumber = result;
		}
		return result;
	}
}
