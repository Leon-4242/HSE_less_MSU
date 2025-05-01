import numberth
import random

def LenstraFactor(n, steps = 100, B = 1000000):
    step = 0
    res = 1

    def sumCurve(xP, yP, xQ, yQ, n):
        nonlocal res
        if (xP-xQ)%n == 0 and (yP - yQ)&n == 0:
            if numberth.gcd(2*yP, n) != 1 and numberth.gcd(2*yP, n) != n:
                res = numberth.gcd(2*yP, n)
                raise ValueError("Found1")
            try:
                m = (3*xP*xP%n + a)*numberth.invmod(2*yP, n)%n
            except:
                raise ValueError("Continue")
        else:
            if numberth.gcd(xP-xQ, n) != 1 and numberth.gcd(xP-xQ, n) != n:
                res = numberth.gcd(xP-xQ, n)
                raise ValueError("Found2")
            try:
                m = (yP-yQ)%n * numberth.invmod(xP-xQ, n) %n
            except:
                raise ValueError("Continue")

        m %= n
        xR = numberth.powermod(m, 2, n) - xP - xQ
        xR %= n
        yR = yP + m*(xR-xP)%n
        yR = -yR
        yR = yR%n
        
        return (xR, yR)


    def doubleCurve(xP, yP, n):
        return sumCurve(xP, yP, xP, yP, n)

    while step < steps:
        x0 = random.randrange(1, n-1)
        y0 = random.randrange(1, n-1)
        a = random.randrange(1, n-1)
        b = numberth.powermod(y0, 2, n) - numberth.powermod(x0, 3, n) - a*x0%n
        e = 1
        for i in range(1, B+1): 
            e *= i
            P = (x0, y0)
            tmp = e
            P2 = P
            Q = None
            try:
                while tmp != 0:
                    if tmp % 2 != 0:
                        if Q != None: Q = sumCurve(*Q, *P2, n)
                        else: Q = P2
                    tmp = tmp >> 1
                    P2 = doubleCurve(*P2, n)
            except ValueError as e:
                return res
        step  = step + 1

    return res
