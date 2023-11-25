Determinants
============

Introduction
------------

The determinant of a square matrix :math:`\mathbf{A}`, denoted as :math:`\mathrm{det} \left( \mathbf{A} \right)`,
is a number associated with :math:`\mathbf{A}`.
It has several use-cases like determining the solvability of linear system of equations. Its magnitude is the scaling
factor of the transformation represented by the matrix.
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




Graphical explanation:
----------------------

A matrix represents transformation between two coordinate systems.
Its columns are the coordinate axis vectors of the current system inside the system the matrix transforms to.

.. plot::

   import matplotlib.pyplot as plt
   import numpy as np

   plt.arrow(0, 0, 2, 0, length_includes_head=True, head_width=0.08, head_length=0.2, color="g")
   plt.arrow(0, 0, 1, 3, length_includes_head=True, head_width=0.08, head_length=0.2, color="r")
   plt.xlim([-1, 3])
   plt.ylim([-1, 3])
   plt.grid()


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




