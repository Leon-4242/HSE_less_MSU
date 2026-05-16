from tkinter import *
from tkinter import ttk
import math
import R2Graph

def clear_click():
    for point in canvas.find_withtag("points"):
        canvas.delete(point)
        points.clear()

def print_point(event):
    global points, xc, yc, dx, dy
    x0 = event.x
    y0 = event.y
    vert = canvas.create_line(x0-5, y0, x0+5, y0, fill="red", tags=["points"])
    horz = canvas.create_line(x0, y0-5, x0, y0+5, fill="red", tags=["points"])
    points.append([int(math.ceil(x0-xc/dx)), int(math.ceil(y0-yc/dy)), vert, horz])

def print_grid(holst, xmin, xmax, ymin, ymax):
    x = math.ceil(xmin)
    while x <= xmax:
        holst.create_line()
def reprint(event):

    w = canvas.winfo_width()
    h = canvas.winfo_height()
    if w > 0 and h > 0:
        aspect = h/w
        dy = (xmax-xmin)*aspect
        ymin = (-dy)/2
        ymax = dy/2

    global xc, yc, dx, dy, w, h

    xc1 = canvas.winfo_x()+int(math.ceil(canvas.winfo_width()/2))
    yc1 = canvas.winfo_y()+int(math.ceil(canvas.winfo_height()/2))
    dx1 = canvas.winfo_width()*dx/w
    dy1 = canvas.winfo_height()*dy/h

    for point in points:
        x0, y0, vert, horz = point
        canvas.coords(vert, int(math.ceil(x0*dx1+xc1))-5, int(math.ceil(y0*dy1+yc1)), int(math.ceil(x0*dx1+xc1))+5, int(math.ceil(y0*dy1+yc1)))
        canvas.coords(horz, int(math.ceil(x0*dx1+xc1)), int(math.ceil(y0*dy1+yc1))-5, int(math.ceil(x0*dx1+xc1)), int(math.ceil(y0*dy1+yc1))+5)

    xc, yc, dx, dy = xc1, yc1, dx1, dy1
    w, h = canvas.winfo_width(), canvas.winfo_height()

root = Tk()
root.title("Приложение на Tkinter.")
root.geometry("500x500")

for c in range(4): root.columnconfigure(index=c, weight=1)
for r in range(3): root.rowconfigure(index=r, weight=1)


clear = ttk.Button(text="Clear", command=clear_click)
clear.grid(row=1, column=3, padx=10, pady=10)

canvas = Canvas(bg="Grey", width=400, height=400)
canvas.grid(row=0, column=0, rowspan=3, columnspan=3, padx=50, pady=50, sticky=NSEW)


w = canvas.winfo_width()
h = canvas.winfo_height()
if w > 0 and h > 0:
    aspect = h/w
    dy = (xmax-xmin)*aspect
    ymin = (-dy)/2
    ymax = dy/2
xmin = canvas.winfo_x()+int(math.ceil(canvas.winfo_width()/2))
ymin = canvas.winfo_y()+int(math.ceil(canvas.winfo_height()/2))
xmax = canvas.winfo_x()+int(math.ceil(canvas.winfo_width()/2))
ymax = canvas.winfo_y()+int(math.ceil(canvas.winfo_height()/2))

points = []

canvas.bind("<ButtonPress-1>", print_point)
canvas.bind("<Configure>", reprint)



root.mainloop()
