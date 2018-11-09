#!/usr/bin/env python3.6
import matplotlib.pyplot as plt
import numpy as np
import math
from scipy.optimize import curve_fit

def my_distribution_func(x, alpha, gamma=1):
    return alpha**(gamma+1)/math.gamma(gamma+1)*x**gamma*math.e**(-alpha*x)

def force_func(distance, radius):
    if distance < radius*pow(2, 1/6):
        return 24*10*250*(2*(radius**12)/(distance**13)-(radius**6)/(distance**7))
    else:
        return 0


def plot_my_distribution():
    points = np.linspace(1/8, 4-1/8, 16)
    experimental_values = [2.5, 40, 185, 132.5, 62.5, 25, 17.5, 7.5, 7.5, 7.5, 5, 5, 5, 5, 2.5, 0]
    experimental_values = [num/(sum(experimental_values)*0.25) for num in experimental_values]
    plt.figure(figsize=(8, 8), dpi=80, facecolor='w', edgecolor='k')
    plt.plot(points, experimental_values)

    params, _ = curve_fit(my_distribution_func, points, experimental_values, [1,1])

    print(params)

    points = np.linspace(1/8, 4-1/8, 400)
    my_distribution = [my_distribution_func(point, *params) for point in points]
    plt.plot(points, my_distribution)
    plt.show()
    #plt.savefig('images/'+ name + filename[13:-4] + '.png', bbox_inches='tight')


def plot_force():
    plt.figure(figsize=(8, 8), dpi=80, facecolor='w', edgecolor='k')
    plt.yscale('log')
    points = np.linspace(1, 6, 100)
    body_force = [force_func(distance, 2.5) for distance in points]
    plt.plot(points, body_force)
    flagella_force = [force_func(distance, 5) for distance in points]
    plt.plot(points, flagella_force)
    plt.show()


def plot_radial_probability():
    radialProbability = np.loadtxt('radial_probability.csv', delimiter=',')
    plt.figure(figsize=(8, 8), dpi=80, facecolor='w', edgecolor='k')
    plt.plot(radialProbability[:, 0], radialProbability[:, 1])
    plt.show()


def plot_density():
    probabilityMap = np.loadtxt('probability_map.csv', delimiter=',')
    plt.imshow(probabilityMap, cmap='hot', interpolation='nearest')
    plt.show()


def plot_traj(pos_x, pos_y):
    minx = min(pos_x)
    miny = min(pos_y)
    maxx = max(pos_x)
    maxy = max(pos_y)
    size = max(maxx-minx,maxy-miny)
    fig = plt.figure(figsize=(8, 8), dpi=80, facecolor='w', edgecolor='k')
    ax = fig.add_subplot(111)
    ax.set_xlim(left=(maxx+minx-size)/2, right=(maxx+minx+size)/2)
    ax.set_ylim(bottom=(maxy+miny-size)/2, top=(maxy+miny+size)/2)
    ax.scatter(pos_x, pos_y, [0.01]*len(pos_x))

def main():
    # plot_force()
    plot_radial_probability()
    plot_density()
    # plot_my_distribution()

if __name__ == '__main__':
    main()
