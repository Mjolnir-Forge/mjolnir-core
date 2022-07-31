# Determinants {#Determinants}

## Introduction

The determinant of a square matrix \f$\mathbf{A}\f$, denoted as \f$\mathrm{det} \left( \mathbf{A} \right)\f$ is a 
number associated with \f$\mathbf{A}\f$. It has several use-cases like determining the solvability of linear system of
equations. Its magnitude is the scaling factor of the transformation represented by the matrix. If the edges of an 
arbitrary volume are multiplied by a matrix with a determinant of value 4, the new volume will be 4 times larger than
the original one. If the determinant is negative, it indicates that the transformation switches the orientation of all
transformed points. If three points originally had a clockwise orientation, then the new ones will have a 
counter-clockwise orientation. Positive determinants preserve the original orientation.

A good tutorial about determinants can be found [here](https://www.youtube.com/watch?v=Ip3X9LOh2dk&t=13s)


## Properties

\f[
\mathrm{det} \left( \mathbf{A} \right)
= 
\mathrm{det} \left( \mathbf{A^T} \right)
\f]
\f[
\mathrm{det} \left( \mathbf{AB} \right)
= 
\mathrm{det} \left( \mathbf{A} \right) \cdot \mathrm{det} \left( \mathbf{B} \right)
\f]

## Equations

### 2x2 Determinant

\f[
\mathbf{A}
=
\begin{bmatrix}
a_0&b_0\\
a_1&b_1\\
\end{bmatrix}
\f]

\f[
\mathrm{det} \left( \mathbf{A} \right)
= a_0b_1 - a_1b_0
\f]


### 3x3 Determinant

\f[
\mathbf{A}
=
\begin{bmatrix}
a_0&b_0&c_0\\
a_1&b_1&c_1\\
a_2&b_2&c_2
\end{bmatrix}
\f]

\f[
\begin{matrix}
\mathrm{det} \left( \mathbf{A} \right)
&
= a_0b_1c_2 - a_0b_2c_1 
\\&
+ a_1b_2c_0 - a_1b_0c_2 
\\&
+ a_2b_0c_1 - a_2b_1c_0
\end{matrix}
\f]

With the column of \f$\mathbf{A}\f$ being the vectors \f$\mathbf{a}\f$, \f$\mathbf{b}\f$, and \f$\mathbf{c}\f$, this can 
be written as:

\f[
\mathrm{det} \left( \mathbf{A} \right)
=
\mathbf{a} \cdot \left( \mathbf{b} \times \mathbf{c} \right)
\f]

The right-hand side of the equation above is also known as the scalar triple product.


### 4x4 Determinant

\f[
\mathbf{A}
=
\begin{bmatrix}
a_0&b_0&c_0&d_0\\
a_1&b_1&c_1&d_1\\
a_2&b_2&c_2&d_2\\
a_3&b_3&c_3&d_3
\end{bmatrix}
\f]

\f[
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
\f]

This can be rearranged to the following equation:

\f[
\begin{matrix}
\mathrm{det} \left( \mathbf{A} \right)
&= \left( {a_0b_1 - a_1b_0} \right) \left( {c_2d_3 - c_3d_2} \right) \\
&+ \left( {a_1b_2 - a_2b_1} \right) \left( {c_0d_3 - c_3d_0} \right) \\
&+ \left( {a_2b_3 - a_3b_2} \right) \left( {c_0d_1 - c_1d_0} \right) \\
&+ \left( {a_3b_0 - a_0b_3} \right) \left( {c_2d_1 - c_1d_2} \right) \\
&+ \left( {a_0b_2 - a_2b_0} \right) \left( {c_3d_1 - c_1d_3} \right) \\
&+ \left( {a_1b_3 - a_3b_1} \right) \left( {c_2d_0 - c_0d_2} \right) 
\end{matrix}
\f]
