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
    \mathbf{\textcolor{red}x}
    =
    \begin{bmatrix}
    2\\
    1\\
    \end{bmatrix}
    &\hspace{1cm}&
    \mathbf{\textcolor{green}y}
    =
    \begin{bmatrix}
    1\\
    3\\
    \end{bmatrix}


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
    plt.arrow(*axis_x, *axis_y, **arrow_setup, color="k")
    plt.arrow(*axis_y, *axis_x, **arrow_setup, color="k")


Because we used the unit axis vectors of the source system to construct the parallelepiped, the enclosed grey area
is :math:`1` inside of the source coordinate system.
Now remember, that the magnitude of a matix's determinant is the scaling factor by which a volume (or area in this case)
changes its size due to the applied transformation.
Since our original volume is :math:`1`, the volume of the parallelepiped inside the target system must be equal to the
determinants magnitude.

Next, let us calculate the determinant of :math:`\mathbf{M}`.
As specified in the equations section of this document, the determinant of a 2x2 matrix

.. math::
    \mathbf{A}
    =
    \begin{bmatrix}
    a_0&b_0\\
    a_1&b_1\\
    \end{bmatrix}

can be calculated as follows:

.. math::
    \mathrm{det} \left( \mathbf{A} \right)
    = a_0b_1 - a_1b_0

For our matrix

.. math::
    \mathbf{M}
    =
    \begin{bmatrix}
    \color{red}{2} & \color{green}{1}\\
    \color{red}{1} & \color{green}{3}\\
    \end{bmatrix}

we get

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




