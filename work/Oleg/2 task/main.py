from R2Graph import *

def minidisk(points):
    if type(points) != list:
        return None

    def circle(a, b, c):
        A = a
        B = b
        C = c
        AB = A.distance(B)
        AC = A.distance(C)
        BC = B.distance(C)

        if AC > AB:
            B, C = C, B
            AC, AB = AB, AC
        if BC > AB:
            A, C = C, A
            BC, AB = AB, BC
        
        d = [(A+B)*0.5, A.distance(B)/2]
        if d[0].distance(C) <= d[1]:
            return d
        M = (A+C)*0.5
        res = intersectLine(d, (A-B).normal(), M, (A-C).normal())
        O = []
        if res[0]:
            O = res[1]
        else:
            O = None

        return [O, O.distance(O, C)]

    def md(R):
        if len(R) == 0:
            return [R2Point(0, 0), -1]
        elif len(R) == 1:
            return [R[0], 0]
        elif len(R) == 2:
            return circle(R[0], R[1], (R[0]+R[1])*0.5)
        return circle(R[0], R[1], R[2])

    def b_minidisk(P, R):
        if len(P) == 0 or len(R) == 3:
            return md(R)

        p = P.pop()
        D = b_minidisk(P, R)
        if D[0].distance(p) > D[1]:
            R.append(p)
            D = b_minidisk(P, R)
        return D

    return b_minidisk(points, [])

def main():
    points = []
    answer = int(input("Work with file?\n1)Yes\n2)No\n"))
    if answer-1:
        n = int(input("\nEnter number of points: "))
        for i in range(n):
            buff = input()
            buff = list(map(float, buff.split()))
            points.append(R2Point(buff))
    else:
        filename = input("Enter filename: ")
        with open(filename, "r") as file:
            for line in file:
                points.append(R2Point(list(map(float, line.split()))))

    print(minidisk(points))

if __name__ == "__main__":
    main()
