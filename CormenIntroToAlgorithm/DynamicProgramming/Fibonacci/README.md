Key concepts:
-Algorithm optimization as the first step in optimization
-C++ constexpr
-C++ function return type deduction using auto and decltype
-C++ chrono
-C++ template argument type deduction

Fibonacci sequence algorithm is a very commonly-used introduction into recursion. The standard tail recursive solution is in StandardRecursive.cpp.
The general solution to calculating a fibonacci number is : Fib(N) = Fib(N-1) + Fib(N-2); Fib(0) = 0; Fib(1) = 1;
The main advantage of the general recursive solution is the option of pushing the calculation complexity from runtime to compile-time, if using a constexpr as
the input value. However, you still want to keep in mind that the complexity of the standard naive recursive solution is 2^N. Try to keep your build engineer happy.
Visual Studio compiler, however, refused to calculate the compile-time solution for anything greater than 20. I tried Fib(1000), and after 1.5 hours, no solution.

Fibonacci sequence is also a good problem to introduce dynamic programming. 
Fibonacci calculation exhibits all of the key ingredients that makes dynamic programming a suitable optimization solution approach:
1. Optimal, independent substructure. Calculating Fib(N-2) will not prevent the calculation of Fib(N-1), and each solutions is an optimal solution to its own subproblem
2. Overlapping subproblems. Fib(N-1) contains recalculation of Fib(N-2). And on, and on until reaching multiple leaves of Fib(0) and Fib(1) in the recursion

The memoization-based top-down solution to the problem is in TopDown.cpp. The solution structure retains the top-down structure in StandardRecursive.cpp, so the solution is easier to follow.
Memoization using a vector eliminates all of the overlapping recalculations. Thanks to Abdul Bari (https://www.youtube.com/watch?v=5dRGRueKU3M&list=RDCMUCZCFT11CWBi3MHNlGf019nw&index=7) for the explanation.

My version of bottom-up solution to the problem is in BottomUp.cpp. I take advantage of the following Fibonacci sequence properties:
1. The solution to Fib(N) depends on Fib(N-1) and Fib(N-2). Given 2 starting point for Fib(0) and Fib(1), we can monotonically increase n in Fib(n) from 2 to N to calculate the
solution of Fib(N). Plus, there is a theorem that says any tail recursion solution can always transform into an interative solution, so I can use iteration from 2 to N to calculate
Fib(N).
2. Fib(N) = Fib(N-1) + Fib(N-2) means that the solution to Fib(N) only cares about the previous 2 solutions. Therefore, instead of using N-sized memoization storage like in the top-down solution,
the bottom-up solution, I can get away with 2.

Comparison Result
Enter Fibonacci sequence to evaluate : 55
Fibonacci(55) bottom-up
Fibonacci(55) = 2144908973
Running unsigned int (__cdecl*)(unsigned int) in 1 loops
Total time 0 us
Fibonacci(55) top-down
Fibonacci(55) = 2144908973
Running unsigned int (__cdecl*)(unsigned int) in 1 loops
Total time 1 us
Fibonacci(55) brute-force
Fibonacci(55) = 2144908973
Running unsigned int (__cdecl*)(unsigned int) in 1 loops
Total time 587088 ms

We can se a huge runtime difference among algorithm that has O(2^N) complexity vs even a polynomial O(N^2). A good optimization strategy should always start at choosing an
algorithm that better fulfil the software's constrains. No amount of cache optimization and multi-threading can help the brute-force method, because the ammount of 
repetition is just ridiculous. However, be careful when using O complexity as a metric for choosing algorithm, because the notation tends to hide other forms of 
latency. Depending on the size of the complexity, the latency overhead of an implementation with less complex O-notation can be slower than a more complex one.
There are many examples to illustrate this warning, such as the performance of insertion sort vs quick sort on a small number of data.