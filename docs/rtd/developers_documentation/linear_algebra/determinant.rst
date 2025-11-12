Determinants
============

Description
-----------

The determinant of a square matrix :math:`\mathbf{A}`, denoted as :math:`\mathrm{det} \left( \mathbf{A} \right)`,
is a number associated with :math:`\mathbf{A}`.
It has several use-cases like determining the solvability of linear system of equations.
Its magnitude is the scaling factor of the transformation represented by the matrix.
If the edges of an arbitrary volume are multiplied by a matrix with a determinant of value 4, the new volume will be
4 times larger than the original one.
If the determinant is negative, it indicates that the transformation switches the orientation of all transformed
points.
If three points originally had a clockwise orientation, then the new ones will have a counter-clockwise orientation.
Positive determinants preserve the original orientation.

A good tutorial about determinants can be found `here`_.

.. _here: https://www.youtube.com/watch?v=Ip3X9LOh2dk&t=13s









Equations
---------

2x2 Determinant
~~~~~~~~~~~~~~~

.. math::
    \mathbf{A}
    =
    \begin{bmatrix}
    a_0&b_0\\
    a_1&b_1\\
    \end{bmatrix}


.. math::
    \mathrm{det} \left( \mathbf{A} \right)
    = a_0b_1 - a_1b_0


3x3 Determinant
~~~~~~~~~~~~~~~

.. math::
    \mathbf{A}
    =
    \begin{bmatrix}
    a_0&b_0&c_0\\
    a_1&b_1&c_1\\
    a_2&b_2&c_2
    \end{bmatrix}


.. math::
    \begin{matrix}
    \mathrm{det} \left( \mathbf{A} \right)
    &
    = a_0b_1c_2 - a_0b_2c_1
    \\&
    + a_1b_2c_0 - a_1b_0c_2
    \\&
    + a_2b_0c_1 - a_2b_1c_0
    \end{matrix}


With the column of :math:`\mathbf{A}` being the vectors :math:`\mathbf{a}`,
:math:`\mathbf{b}`, and :math:`\mathbf{c}`, this can be written as:

.. math::
    \mathrm{det} \left( \mathbf{A} \right)
    =
    \mathbf{a} \cdot \left( \mathbf{b} \times \mathbf{c} \right)


The right-hand side of the equation above is also known as the scalar triple
product.


4x4 Determinant
~~~~~~~~~~~~~~~

.. math::
    \mathbf{A}
    =
    \begin{bmatrix}
    a_0&b_0&c_0&d_0\\
    a_1&b_1&c_1&d_1\\
    a_2&b_2&c_2&d_2\\
    a_3&b_3&c_3&d_3
    \end{bmatrix}


.. math::
    \begin{matrix}
    \mathrm{det} \left( \mathbf{A} \right)
    &
    = a_0b_1c_2d_3
    + a_0b_2c_3d_1
    + a_0b_3c_1d_2
    - a_0b_1c_3d_2
    - a_0b_2c_1d_3
    - a_0b_3c_2d_1
    \\&
    + a_1b_0c_3d_2
    + a_1b_2c_0d_3
    + a_1b_3c_2d_0
    - a_1b_0c_2d_3
    - a_1b_2c_3d_0
    - a_1b_3c_0d_2
    \\&
    + a_2b_0c_1d_3
    + a_2b_1c_3d_0
    + a_2b_3c_0d_1
    - a_2b_0c_3d_1
    - a_2b_1c_0d_3
    - a_2b_3c_1d_0
    \\&
    + a_3b_0c_2d_1
    + a_3b_1c_0d_2
    + a_3b_2c_1d_0
    - a_3b_0c_1d_2
    - a_3b_1c_2d_0
    - a_3b_2c_0d_1
    \end{matrix}


This can be rearranged to the following equation:

.. math::
    \begin{matrix}
    \mathrm{det} \left( \mathbf{A} \right)
    &= \left( {a_0b_1 - a_1b_0} \right) \left( {c_2d_3 - c_3d_2} \right) \\
    &+ \left( {a_1b_2 - a_2b_1} \right) \left( {c_0d_3 - c_3d_0} \right) \\
    &+ \left( {a_2b_3 - a_3b_2} \right) \left( {c_0d_1 - c_1d_0} \right) \\
    &+ \left( {a_3b_0 - a_0b_3} \right) \left( {c_2d_1 - c_1d_2} \right) \\
    &+ \left( {a_0b_2 - a_2b_0} \right) \left( {c_3d_1 - c_1d_3} \right) \\
    &+ \left( {a_1b_3 - a_3b_1} \right) \left( {c_2d_0 - c_0d_2} \right)
    \end{matrix}




Properties
----------

.. math::

    \mathrm{det} \left( \mathbf{A} \right)
    =
    \mathrm{det} \left( \mathbf{A^T} \right)


.. math::

    \mathrm{det} \left( \mathbf{AB} \right)
    =
    \mathrm{det} \left( \mathbf{A} \right) \cdot
    \mathrm{det} \left( \mathbf{B} \right)



Visualization:
--------------

..
    Basic setup for the following plots

.. plot::
    :context:
    :nofigs:

    import matplotlib.pyplot as plt
    import numpy as np

    arrow_setup = dict(length_includes_head=True, head_width=0.08, head_length=0.2)
    limits = [-1, 5]

    zero   = [0, 0]
    axis_x = [2, 1]
    axis_y = [1, 3]

    def basic_setup():
        plt.plot(limits, zero, color="k")
        plt.plot(zero, limits, color="k")
        plt.xlim(*limits)
        plt.ylim(*limits)
        plt.grid()


As discussed in the document about matrices, a matrix transforms coordinates or vectors from a source coordinate system
to a target coordinate system.
Each of its columns represents one of the source coordinate systems unit axis vectors depicted inside of the target
coordinate system.
We will now have a look at a short 2D example and visualize the relevant correlations between the matrix and its
determinant.


TODO: LINK SECTION


Consider the following matrix:

.. math::
    \mathbf{M}
    =
    \begin{bmatrix}
    2 & 1\\
    1 & 3\\
    \end{bmatrix}


The unit axis vectors of the source coordinate system inside the target coordinate system are directly taken from the
matrix:

.. math::
    \mathbf{\textcolor{red}a}
    =
    \begin{bmatrix}
    2\\
    1\\
    \end{bmatrix}
    &\hspace{1cm}&
    \mathbf{\textcolor{green}b}
    =
    \begin{bmatrix}
    1\\
    3\\
    \end{bmatrix}

The vector :math:`\mathbf{\textcolor{red}a}` is the first matrix column and :math:`\mathbf{\textcolor{green}b}` the
second.
If we plot the axis vectors with the same colors as chosen for their symbols, we get the following plot:

.. plot::
    :context:
    :caption: Source coordinate system axis in target coordinate system

    basic_setup()
    plt.arrow(*zero, *axis_x, **arrow_setup, color="r")
    plt.arrow(*zero, *axis_y, **arrow_setup, color="g")


Next we will create a parallelepiped from the given axis vectors, which results in the following image:


.. plot::
    :context: close-figs
    :caption: Parallelepiped from the coordinate axis

    basic_setup()

    plt.fill(
        [0, axis_x[0], axis_x[0] + axis_y[0], axis_y[0]],
        [0, axis_x[1], axis_x[1] + axis_y[1], axis_y[1]],
        color="lightgrey"
    )

    plt.arrow(*zero, *axis_x, **arrow_setup, color="r")
    plt.arrow(*zero, *axis_y, **arrow_setup, color="g")
    plt.arrow(*axis_x, *axis_y, **arrow_setup, color="g")
    plt.arrow(*axis_y, *axis_x, **arrow_setup, color="r")


Because we used the unit axis vectors of the source system to construct the parallelepiped, the enclosed grey area
is :math:`1` inside of the source coordinate system.
Now remember, that the magnitude of a matix's determinant is the scaling factor by which a volume (or area in this case)
changes its size due to the applied transformation.
Since our original volume is :math:`1`, the volume of the parallelepiped inside the target system must be equal to the
determinants magnitude.

We will now calculate the determinant of :math:`\mathbf{M}`.
As specified in the equations section of this document, the determinant of a 2x2 matrix

.. math::
    \mathbf{A}
    =
    \begin{bmatrix}
    {\color{red}a_x}&{\color{green}b_x}\\
    {\color{red}a_y}&{\color{green}b_y}\\
    \end{bmatrix}

can be calculated as follows:

.. math::
    :label: determinant_2x2_xy

    \mathrm{det} \left( \mathbf{A} \right)
    = {\color{red}a_x}{\color{green}b_y} - {\color{red}a_y}{\color{green}b_x}

Note that we replaced the indices 0 and 1 from the original equation with x and y here, so that the meaning of the
individual components gets a bit clearer.
For our matrix

.. math::
    \mathbf{M}
    =
    \begin{bmatrix}
    \color{red}{2} & \color{green}{1}\\
    \color{red}{1} & \color{green}{3}\\
    \end{bmatrix}

we get:

.. math::
    \mathrm{det} \left( \mathbf{M} \right)
    &= {\color{red}{2}}\cdot{\color{green}{3}} - {\color{red}{1}}\cdot{\color{green}{1}}\\
    &= 5

From this, we can conclude that the previosly constructed parallelepiped must have an area of :math:`5` inside of the
target system.
We could of course use basic trigonometric equations to proove this, but instead, we will proove it visually.
To do so, we will take a closer look at the two products of equation :eq:`determinant_2x2_xy`.
As you can see, every product consists of a single x- and y-component in the target coordinate system and therefore
represent the volume of a rectangle.
The next plot shows the resulting two rectangles in the target coordinate system.


.. plot::
    :context: close-figs
    :caption: Individual determinant products

    basic_setup()

    plt.fill(
        [0, axis_x[0], axis_x[0], 0],
        [0, 0, axis_y[1], axis_y[1]],
        color="lightgreen"
    )

    plt.fill(
    [0, axis_x[1], axis_x[1], 0],
    [0, 0, axis_y[0], axis_y[0]],
        color="lightcoral"
    )

    plt.plot([axis_x[0], axis_x[0]], [0, axis_y[1]], "--", color="k")
    plt.plot([axis_x[1], axis_x[1]], [0, axis_y[1]], "--", color="k")

    plt.plot([0, axis_x[0]], [axis_y[0], axis_y[0]], "--", color="k")
    plt.plot([0, axis_x[0]], [axis_y[1], axis_y[1]], "--", color="k")

    plt.arrow(*zero, *axis_x, **arrow_setup, color="r")
    plt.arrow(*zero, *axis_y, **arrow_setup, color="g")

The dashed lines are just visual aids to identify the individual x- and y- components of the axis vectors.

The rectangular surface that is formed by the combined red and green areas represents the product
:math:`{\color{red}a_x}\cdot {\color{green}b_y}` which is :math:`{\color{red}2}\cdot\color{green}{3}` for the matrix
:math:`\mathbf{M}`.
It's top right point :math:`p_0` has the products components as coordinates:


.. math::
    \mathbf{p_0}
    =
    \begin{bmatrix}
    \color{red}{a_x}\\
    \color{green}{b_y}\\
    \end{bmatrix}
    =
    \begin{bmatrix}
    \color{red}{2}\\
    \color{green}{3}\\
    \end{bmatrix}

The product :math:`{\color{red}a_y}\cdot {\color{green}b_x}` that equals to
:math:`{\color{red}1}\cdot\color{green}{1}` is represented by the rectangle that encloses the red area.
It's top right point is :math:`p_1`:


.. math::
    \mathbf{p_1}
    =
    \begin{bmatrix}
    \color{green}{b_x}\\
    \color{red}{a_y}\\
    \end{bmatrix}
    =
    \begin{bmatrix}
    \color{green}{1}\\
    \color{red}{1}\\
    \end{bmatrix}


Since the determinant is the difference between those two areas, its magnitude is equal to the area of the green
surface.
Counting the number of squares in the plot gets us to the same number :math:`5` we calculated earlier.
We will now get back to our parallelepiped and try to fit it into the green surface be slicing off and rearranging parts
of it.
The following series of plots will proove that the parallelepiped has indeed the same area as the green surface.


.. plot::
    :context: close-figs

    basic_setup()

    area_1_x = [0, axis_x[0], axis_x[0] + axis_y[0], axis_y[0], 0]
    area_1_y = [0, axis_x[1], axis_x[1] + axis_y[1], axis_y[1], 0]

    cut_1_x = [axis_y[0], 2.6666, axis_x[0] + axis_y[0]]
    cut_1_y = [axis_y[1], axis_y[1], axis_x[1] + axis_y[1]]

    add_1_x = [0, 1.6666, axis_x[0]]
    add_1_y = [0, 0, axis_x[1]]

    plt.fill(area_1_x, area_1_y, color="lightgrey")
    plt.fill(cut_1_x, cut_1_y, color="lightcoral")
    plt.fill(add_1_x, add_1_y, color="lightgreen")

    plt.plot(area_1_x, area_1_y, color="k")



.. plot::
    :context: close-figs

    basic_setup()

    area_2_x = area_1_x
    area_2_y = area_1_y

    area_2_x[1] = add_1_x[1]
    area_2_y[1] = add_1_y[1]
    area_2_x[2] = cut_1_x[1]
    area_2_y[2] = cut_1_y[1]

    cut_2_x = [axis_x[0], area_2_x[2], axis_x[0]]
    cut_2_y = [axis_x[1], axis_y[1], axis_y[1]]

    add_2_x = [0, 0, 0.6666]
    add_2_y = [0, 2, 2]

    plt.fill(area_2_x, area_2_y, color="lightgrey")
    plt.fill(cut_2_x, cut_2_y, color="lightcoral")
    plt.fill(add_2_x, add_2_y, color="lightgreen")

    plt.plot(area_2_x, area_2_y, color="k")



.. plot::
    :context: close-figs

    basic_setup()

    area_3_x = [area_2_x[0], area_2_x[1], cut_2_x[0], cut_2_x[2], area_2_x[3], add_2_x[2], add_2_x[1], add_2_x[0]]
    area_3_y = [area_2_y[0], area_2_y[1], cut_2_y[0], cut_2_y[2], area_2_y[3], add_2_y[2], add_2_y[1], add_2_y[0]]


    cut_3_x = [add_2_x[2], area_3_x[4], area_3_x[4]]
    cut_3_y = [2, 3, 2]

    add_3_x = [area_3_x[1], 2, 2]
    add_3_y = [0, 0, 1]

    plt.fill(area_3_x, area_3_y, color="lightgrey")
    plt.fill(cut_3_x, cut_3_y, color="lightcoral")
    plt.fill(add_3_x, add_3_y, color="lightgreen")

    plt.plot(area_3_x, area_3_y, color="k")


.. plot::
    :context: close-figs

    basic_setup()

    area_4_x = [0, 2, 2, 1, 1, 0, 0]
    area_4_y = [0, 0, 3, 3, 2, 2, 0]


    cut_4_x = [0, 0, 1, 1]
    cut_4_y = [0, 1, 1, 0]

    add_4_x = [0, 0, 1, 1]
    add_4_y = [2, 3, 3, 2]

    plt.fill(area_4_x, area_4_y, color="lightgrey")
    plt.fill(cut_4_x, cut_4_y, color="lightcoral")
    plt.fill(add_4_x, add_4_y, color="lightgreen")

    plt.plot(area_4_x, area_4_y, color="k")


.. plot::
    :context: close-figs

    basic_setup()

    area_5_x = [1, 2, 2, 0, 0, 1, 1]
    area_5_y = [0, 0, 3, 3, 1, 1, 0]

    plt.fill(area_5_x, area_5_y, color="lightgrey")

    plt.plot(area_5_x, area_5_y, color="k")
