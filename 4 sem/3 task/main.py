import lib
import random
import numberth

def main():
    primes = []
    print("primes:")
    for i in range(10):
        p = random.randrange(10**5, 10**7)
        while not numberth.primeTest(p):
            p -= 1 
        primes.append(p)
        print(p)

    print("n:")
    n = 1
    for i in range(10):
        n = n* primes[i]

    print(n)

    res = lib.LenstraFactor(n, 100000)
    print(res)
if __name__ == "__main__":
    main()
