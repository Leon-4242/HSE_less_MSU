import random

def gcd(m, n):
    (a, b) = (m, n)
    while b != 0:
        (a, b) = (b, a%b)
    if a >= 0:
        return a
    else:
        return (-a)


def extEucl(m, n):
    (a, b) = (m, n)
    u1 = 1; v1 = 0
    u2 = 0; v2 = 1

    while b != 0:
        assert (a == u1*m + v1*n and b == u2*m + v2*n)

        q = a // b; r = a % b
        assert (a == q*b + r)

        (a, b) = (b, r)
        (u1, u2) = (u2, u1 - q*u2)
        (v1, v2) = (v2, v1 - q*v2)
    if a >= 0:
        return (a, u1, v1)
    else:
        return (-a, -u1, -v1)


def invmod(x, m):
    assert(m != 0)
    (d, u, v) = extEucl(m, x)
    if d == 1:
        return v%m
    else:
        raise ValueError("Element is not invertible modulo m")


def powermod(a, n, m):
    p = 1; b = a; k = n
    while (k > 0):
        #Invariant: p*b^k == a^n
        if k%2 == 0:
            k //= 2
            b = b*b%m
        else:
            k -= 1
            p = p*b%m
    return p


def primeTest(m, k = 20):
    if m in [2, 3, 5, 7, 11, 13, 17, 19]:
        return True
    elif m%2 == 0 or m < 20:
        return False


    t = m-1
    s = 0
    while t%2 == 0:
        t = t/2
        s = s+1

    for j in range(k):
        b = random.randrange(2, m-1)
        x = powermod(b, t, m)
        if (x-1)%m == 0 or (x+1)%m == 0: continue
        i = 0
        while i < s:
            x = powermod(x, 2, m)
            i = i +1
            if (x-1)%m == 0:
                return False
            elif (x+1)%m == 0: break

        if i == s and (x-1)%m != 0:
            return False

    return True

