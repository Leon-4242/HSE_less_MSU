from tkinter import *
from R2Graph import *
import numpy as np
from scipy.optimize import linprog, minimize

def main():
    xMin = -8
    xMax = 8
    yMin = -6
    yMax = 6
    
    points = []
    objectsIDs = []
    
    def mapToPixels(t):
        w = drawArea.winfo_width()
        h = drawArea.winfo_height()
        if w <= 0 or h <= 0:
            return (0, 0)
        x = w*(t.x - xMin)/(xMax - xMin)
        y = h*(yMax - t.y)/(yMax - yMin)
        return (x, y)
    
    def mapFromPixels(p):
        w = drawArea.winfo_width()
        h = drawArea.winfo_height()
        if w <= 0 or h <= 0:
            return R2Point(0, 0)
        x = xMin + (xMax - xMin)*(p[0]/w)
        y = yMax - (yMax - yMin)*(p[1]/h)
        return R2Point(x, y)
    
    root = Tk()
    root.title("Least Squares")
    root.geometry("800x600")
    panel = Frame(root)

    drawButton = Button(panel, text = "Draw")
    clearButton = Button(panel, text = "Clear")
    drawButton.pack(side = LEFT, fill = X, padx = 4, pady = 4)
    clearButton.pack(side = LEFT, fill = X, padx = 4, pady = 4)

    degree = 1
    degreeLabel = Label(panel, text="Degree:")
    scale = Scale(panel, from_=1, to=10, orient=HORIZONTAL)
    scale.set(degree)
    degreeLabel.pack(side=LEFT, padx=4, pady=4)
    scale.pack(side=LEFT, padx=4, pady=4)

    lossFunctionIdx = IntVar(value=0)
    lossFunctionMSERadio = Radiobutton(
        panel,
        text = "MSE", variable=lossFunctionIdx, value=0,
        fg = "darkBlue"
    )
    lossFunctionMAERadio = Radiobutton(
        panel,
        text = "MAE", variable=lossFunctionIdx, value=1,
        fg = "darkRed"
    )
    lossFunctionHuberRadio = Radiobutton(
        panel,
        text = "Huber", variable=lossFunctionIdx, value=2,
        fg = "darkGreen"
    )
    lossFunctionMSERadio.pack(side=LEFT, padx=4, pady=4)
    lossFunctionMAERadio.pack(side=LEFT, padx=4, pady=4)
    lossFunctionHuberRadio.pack(side=LEFT, padx=4, pady=4)

    panel.pack(side=TOP, fill = X)
    drawArea = Canvas(root, bg = "white")
    drawArea.pack(side = TOP, fill = BOTH, expand = True, padx = 4, pady = 4)
    
    def drawGrid():
        x0 = int(xMin)
        x1 = int(xMax)
        for x in range(x0, x1+1):
            if x == 0:
                continue
            drawArea.create_line(
                mapToPixels(R2Point(x, yMin)),
                mapToPixels(R2Point(x, yMax)),
                fill="gray", width=1
            )                
        y0 = int(yMin)
        y1 = int(yMax)
        for y in range(y0, y1+1):
            if y == 0:
                continue
            drawArea.create_line(
                mapToPixels(R2Point(xMin, y)),
                mapToPixels(R2Point(xMax, y)),
                fill="gray", width=1
            )                
        drawArea.create_line(
            mapToPixels(R2Point(xMin, 0)), 
            mapToPixels(R2Point(xMax, 0)),
            fill="black", width=3
        )
        drawArea.create_line(
            mapToPixels(R2Point(0, yMin)), 
            mapToPixels(R2Point(0, yMax)),
            fill="black", width=3
        )

    def drawPoint(t):
        v = R2Vector(-0.1, 0.1)        
        objectID = drawArea.create_oval(
            mapToPixels(t + v),
            mapToPixels(t - v),
            fill = "red", width=1, outline="red"
        )
        objectsIDs.append(objectID)

    def drawPoints():
        for t in points:
            drawPoint(t)

    def onButtonRelease(e):
        t = mapFromPixels((e.x, e.y))
        points.append(t)
        drawPoint(t)
    
    drawArea.bind("<ButtonRelease-1>", onButtonRelease)
    
    def onConfigure(e):
        nonlocal xMin, xMax, yMin, yMax
        w = drawArea.winfo_width()
        h = drawArea.winfo_height()
        if w > 0 and h > 0:
            aspect = h/w
            yMax = xMax*aspect
            yMin = (-yMax)

        drawArea.delete("all")
        drawGrid()
        drawPoints()        
        if plotID >= 0:
            plotFunction()

    drawArea.bind("<Configure>", onConfigure)

    plotID = (-1)
    degree = 1
    w = np.zeros(degree + 1)

    def func(x):
        return np.polyval(w, x)

    def plotFunction():
        nonlocal w, plotID
        if plotID >= 0:
            drawArea.delete(plotID)

        lossFunc = lossFunctionIdx.get()
        if lossFunc == 0:
            color = "blue"
        elif lossFunc == 1:
            color = "red"
        else:
            color = "darkGreen"
        t = R2Point(xMin, func(xMin))
        dx = 0.05
        path = []
        while t.x <= xMax:
            path.append(mapToPixels(t))
            t.x += dx
            t.y = func(t.x)
        plotID = drawArea.create_line(path, fill=color, width=2)

    def onDraw():
        nonlocal w, degree
        lossFunc = lossFunctionIdx.get()

        if lossFunc == 0:
            onDrawMSE()
        elif lossFunc == 1:
            onDrawMAE()
        elif lossFunc == 2:
            onDrawHuber()

    drawButton.configure(command = onDraw)

    def onDrawMSE():
        nonlocal w, degree

        degree = scale.get()
        m = len(points)
        if m == 0:
            return
        y = np.array([points[i].y for i in range(m)])
        a = np.vander([points[i].x for i in range(m)], degree + 1)
        pinv_a = np.linalg.pinv(a)
        w = pinv_a.dot(y)

        plotFunction()

    def onDrawMAE():
        nonlocal w, degree
        degree = scale.get()
        m = len(points)
        if m == 0:
            return
        
        # Use the Linear programming
        n = degree + 1
        k = n + 2*m
        a_eq = np.zeros(shape=(m, k))
        x = np.zeros(m)
        for i in range(len(points)):
            x[i] = points[i].x
            
        v = np.vander(x, n)
        a_eq[:, :n] = v
        for i in range(m):
            a_eq[i, n+2*i] = (-1.)
            a_eq[i, n+2*i+1] = 1.
        b_eq = np.zeros(m)
        for i in range(m):
            b_eq[i] = points[i].y
        bounds = []
        for i in range(n):
            bounds.append((None, None))
        for i in range(n, k):
            bounds.append((0., None))

        # Target loss function
        c = np.zeros(k)
        for i in range(m):
            c[n + 2*i] = 1.
            c[n + 2*i + 1] = 1.
        res = linprog(c, A_eq = a_eq, b_eq = b_eq, bounds = bounds)
        w = res.x[:n]
        plotFunction()

    def huberFunction(x):
        ax = abs(x)
        if ax < 1.:
            return ax*ax*0.5
        else:
            return ax - 0.5

    class errorFunction:
        def __init__(self, x, degree, y):
            self.x = x
            self.y = y
            self.degree = degree

        def __call__(self, pol):
            m = len(self.x)
            n = self.degree + 1
            res = 0.
            for i in range(m):
                v = np.polyval(pol, self.x[i])
                res += huberFunction(v - self.y[i])
            return res

    def huberPolynomial(x, y, degree):
        m = len(x)
        assert m <= len(y)
        if m == 0:
            return None

            # Initial approximation: polynomial of degree zero
        meanValue = sum(y)/m
        n = degree + 1
        pol = np.zeros(n)
        pol[degree] = meanValue

        res = minimize(fun = errorFunction(x, degree, y), x0 = pol)
        w = res.x
        return w

    def onDrawHuber():
        nonlocal w, degree, plotID
        degree = scale.get()
        m = len(points)
        if m == 0:
            return
         

        # Minimize the Huber loss function
        # . . .
        x = np.array([points[i].x for i in range(m)])
        y = np.array([points[i].y for i in range(m)])
        w = huberPolynomial(x, y, degree)
        
        plotFunction()
        
    def onClear():
        nonlocal plotID
        points.clear()
        for i in objectsIDs:
            drawArea.delete(i)
        objectsIDs.clear()    
        if plotID >= 0:
            drawArea.delete(plotID)
            plotID = (-1)

    clearButton.configure(command = onClear)
        
    root.update()
    
    drawGrid()

    
    root.mainloop()

if __name__ == "__main__":
    main()

