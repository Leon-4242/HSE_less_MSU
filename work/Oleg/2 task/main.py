from random import randint, shuffle
from math import sqrt

def distance(a, b):
    return sqrt((b[0]-a[0])**2 + (b[1]-a[1])**2)


def circle_from_2(a, b):
    """Окружность, проходящая через две точки."""

    center = [(a[i] + b[i]) / 2 for i in [0, 1]]
    radius = distance(center, a)
    return center, radius

def circle_from_3(a, b, c):
    """Окружность, проходящая через три точки (не коллинеарные)."""
    # Формулы для центра описанной окружности
    d = 2 * (
        a[0] * (b[1] - c[1]) +
        b[0] * (c[1] - a[1]) +
        c[0] * (a[1] - b[1])
    )

    if abs(d) < 1e-14:  # коллинеарные точки
        return None

    ux = (
        (a[0]**2 + a[1]**2) * (b[1] - c[1]) +
        (b[0]**2 + b[1]**2) * (c[1] - a[1]) +
        (c[0]**2 + c[1]**2) * (a[1] - b[1])
    ) / d

    uy = (
        (a[0]**2 + a[0]**2) * (c[0] - b[0]) +
        (b[0]**2 + b[0]**2) * (a[0] - c[0]) +
        (c[0]**2 + c[0]**2) * (b[0] - a[0])
    ) / d

    center = [ux, uy]
    radius = distance(center, a)
    return center, radius


def is_inside(point, circle):
    center, r = circle
    return distance(point, center) <= r + 1e-12


def welzl(points, R, n):
    """Рекурсивная часть алгоритма Вейцеля."""
    if n == 0 or len(R) == 3:
        # Строим минимальную окружность по точкам R
        if len(R) == 0:
            return None
        elif len(R) == 1:
            return (R[0], 0.0)
        elif len(R) == 2:
            return circle_from_2(R[0], R[1])
        else:
            return circle_from_3(R[0], R[1], R[2])

    # Случайная точка
    idx = randint(0, n - 1)
    p = points[idx]

    # Перемещаем выбранную точку в конец диапазона
    points[idx], points[n - 1] = points[n - 1], points[idx]

    D = welzl(points, R, n - 1)

    # Точка оказывается вне текущей окружности
    if D is None or not is_inside(p, D):
        R.append(p)
        D = welzl(points, R, n - 1)
        R.pop()

    return D


def minimal_enclosing_circle(points):
    if not points:
        return None

    pts = points[:]
    shuffle(pts)
    return welzl(pts, [], len(pts))

def main():
    points = []
    answer = int(input("Work with file?\n1)Yes\n2)No\n"))
    if answer-1:
        n = int(input("\nEnter number of points: "))
        for i in range(n):
            buff = input()
            buff = list(map(float, buff.split()))
            points.append(buff)
    else:
        filename = input("Enter filename: ")
        with open(filename, "r") as file:
            for line in file:
                points.append(list(map(float, line.split())))

    print(minimal_enclosing_circle(points))

if __name__ == "__main__":
    main()
