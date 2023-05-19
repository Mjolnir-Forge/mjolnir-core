# Determinants

## Introduction

The determinant of a square matrix $\mathbf{A}$, denoted as
$\mathrm{det} \left( \mathbf{A} \right)$, is a number associated with
$\mathbf{A}$. It has several use-cases like determining the solvability of
linear system of equations. Its magnitude is the scaling factor of the
transformation represented by the matrix. If the edges of an arbitrary volume
are multiplied by a matrix with a determinant of value 4, the new volume will be
4 times larger than the original one. If the determinant is negative, it
indicates that the transformation switches the orientation of all transformed
points. If three points originally had a clockwise orientation, then the new
ones will have a counter-clockwise orientation. Positive determinants preserve
the original orientation.

A good tutorial about determinants can be found
[here](https://www.youtube.com/watch?v=Ip3X9LOh2dk&t=13s)

## Properties

$$
\mathrm{det} \left( \mathbf{A} \right)
=
\mathrm{det} \left( \mathbf{A^T} \right)
$$
$$
\mathrm{det} \left( \mathbf{AB} \right)
=
\mathrm{det} \left( \mathbf{A} \right) \cdot
\mathrm{det} \left( \mathbf{B} \right)
$$

## Equations

### 2x2 Determinant

$$
\mathbf{A}
=
\begin{bmatrix}
a_0&b_0\\
a_1&b_1\\
\end{bmatrix}
$$

$$
\mathrm{det} \left( \mathbf{A} \right)
= a_0b_1 - a_1b_0
$$

### 3x3 Determinant

$$
\mathbf{A}
=
\begin{bmatrix}
a_0&b_0&c_0\\
a_1&b_1&c_1\\
a_2&b_2&c_2
\end{bmatrix}
$$

$$
\begin{matrix}
\mathrm{det} \left( \mathbf{A} \right)
&
= a_0b_1c_2 - a_0b_2c_1
\\&
+ a_1b_2c_0 - a_1b_0c_2
\\&
+ a_2b_0c_1 - a_2b_1c_0
\end{matrix}
$$

With the column of $\mathbf{A}$ being the vectors $\mathbf{a}$,
$\mathbf{b}$, and $\mathbf{c}$, this can be written as:

$$
\mathrm{det} \left( \mathbf{A} \right)
=
\mathbf{a} \cdot \left( \mathbf{b} \times \mathbf{c} \right)
$$

The right-hand side of the equation above is also known as the scalar triple
product.

### 4x4 Determinant

$$
\mathbf{A}
=
\begin{bmatrix}
a_0&b_0&c_0&d_0\\
a_1&b_1&c_1&d_1\\
a_2&b_2&c_2&d_2\\
a_3&b_3&c_3&d_3
\end{bmatrix}
$$

$$
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
$$

This can be rearranged to the following equation:

$$
\begin{matrix}
\mathrm{det} \left( \mathbf{A} \right)
&= \left( {a_0b_1 - a_1b_0} \right) \left( {c_2d_3 - c_3d_2} \right) \\
&+ \left( {a_1b_2 - a_2b_1} \right) \left( {c_0d_3 - c_3d_0} \right) \\
&+ \left( {a_2b_3 - a_3b_2} \right) \left( {c_0d_1 - c_1d_0} \right) \\
&+ \left( {a_3b_0 - a_0b_3} \right) \left( {c_2d_1 - c_1d_2} \right) \\
&+ \left( {a_0b_2 - a_2b_0} \right) \left( {c_3d_1 - c_1d_3} \right) \\
&+ \left( {a_1b_3 - a_3b_1} \right) \left( {c_2d_0 - c_0d_2} \right)
\end{matrix}
$$
