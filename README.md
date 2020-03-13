# Stereogram Generation & Extraction

## Description

This code effectively generates two types of single-image stereogram.

1. Stereogram is hidden inside a distorted image.

2. Stereogram which correctly displays texture of 3D object.

While the first is easy to generate, the second type is more complex for coding, thus is rare on github.

Two sample images are as follows:

![type1](https://raw.githubusercontent.com/wjxway/image-storage/master/stereogram1.png)

![type2](https://raw.githubusercontent.com/wjxway/image-storage/master/stereogram2.png)

If viewed properly, both image should display a rabbit and a rabbit only. Note that in the second image, the color and texture are rendered properly. The first and the second type can be combined to create augmented hidden stereograms with darkened edges (easier and more visually pleasant to observe).

![combined](https://raw.githubusercontent.com/wjxway/image-storage/master/stereogram3.png)

Updated code allows you to extract the height map from a stereogram, though the quality of the output image highly depends on the quality of import image... and the code is not automated enough, it might require some tweaking to work with your own case.

## Theory of Operation

![combined](https://raw.githubusercontent.com/wjxway/image-storage/master/stereogramillus.png)

Theory is simple, consider a pair of eyes vieweing with viewing angle $\alpha$, and consider first order approximate for $\sin\alpha$ and $\tan\alpha$. We would like to create an illusion that we are actually viewing a 3D object behind the screen, which means the we should let the image perceived by left and right eye the same as when a actual object is present. Especially, if we were viewing a point $(x,y,h)$ with color $f(x,y,h)$ on the object, the light ray from the object to both eyes are represented by blue lines shown on the image. To mimic this point using a 2D image, we only need to set the two pixels on the intersection of light ray and the 2D plane to $f(x,y,h)$. Thus we have the following relationship:
$$
g(x-h\alpha,y)=g(x+h\alpha,y)=f(x,y,h)
$$
where $g(x,y)$ represent the color on point $(x,y)$ on the 2D plane.



In reality, we will need several image and a height map $h(x,y)$.

**Method 1 :** Method 1 is a neat trick, we assume that $\frac{\partial h}{\partial x}\alpha<1$. thus $x\to x-h\alpha$ and $x\to x+h\alpha$ are both injections. Then we can deduct the following algorithm:

for $y=1\to y_{max}$

​	for $x=1\to x_{max}$

​		$g(x+h(x,y) \alpha,y)=g(x-h(x,y)\alpha,y)$

If nothing is present (only plain background of height $h_0$), the image $g(x,y)$ will be a periodic image with period $2h_0\alpha$, thus we might as well use a periodic image at the beginning(better visual effect, less computation), and let the object has height of $h=h_0-h_{add}$, where $h_{add}$ is the height map of the object.

*Merits:*

1. Easy to implement.
2. The resulting image will highly resembles the background image, thus the pattern will be highly invisible.

*Drawbacks:*

1. Requires $\frac{\partial h}{\partial x}\alpha<1$, or else will have defects, thus cannot create image with strong 3D feeling.
2. The mapping between original background and the resulting image is unclear, thus cannot effectly map the texture.



**Method 2:** Method 2 is directly derived from the original algorithm, we will directly map the texture $f(x,y,h(x,y))$ to $g(x,y)$. It is evident that this algorithm can be executed independently for each $y$, we will omit that in further discussion.

for $y=1\to y_{max}$

1. Create map from edges $f(x)\leftrightarrow g(x-h(x)\alpha)$, $f(x)\leftrightarrow g(x+h(x)\alpha)$.
2. Find all connected components. It should be something like: $f(x_0)\leftrightarrow g(x_1)\leftrightarrow f(x_2)\leftrightarrow g(x_3), f(x_2)\leftrightarrow g(x_4)$. This means if we want to create a image with correct height and texture, the texture $f(x_0)$ and $f(x_2)$ should be the same (if not, we can simply substitute $f(x_2)$ by $f(x_0)$), and the points on the image $g(x_1), g(x_3),g(x_4)$ should be the same.
3.  Now set $g(x_1), g(x_3),g(x_4)=f(x_0)$ or something similar for all connected components.

*Merits:*

1. Accurate relationship, especially effective with large $h_{add}$, thus can create image with strong 3D feeling.
2. Accurate pixel map, can map textures effectively. 

*Drawbacks:*

1. Less straight forward code.
2. Slow in execution.

## Acknowledgement

Special Thanks to njpipeorgan for his marvelous [wll-interface](https://github.com/njpipeorgan/wll-interface)!
