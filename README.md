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

Theory is simple, consider a pair of eyes vieweing with viewing angle ![](https://latex.codecogs.com/svg.latex?%5Calpha), and consider first order approximate for ![](https://latex.codecogs.com/svg.latex?%5Csin%5Calpha) and ![](https://latex.codecogs.com/svg.latex?%5Ctan%5Calpha). We would like to create an illusion that we are actually viewing a 3D object behind the screen, which means the we should let the image perceived by left and right eye the same as when a actual object is present. Especially, if we were viewing a point ![](https://latex.codecogs.com/svg.latex?%28x%2Cy%2Ch%29) with color ![](https://latex.codecogs.com/svg.latex?f%28x%2Cy%2Ch%29) on the object, the light ray from the object to both eyes are represented by blue lines shown on the image. To mimic this point using a 2D image, we only need to set the two pixels on the intersection of light ray and the 2D plane to ![](https://latex.codecogs.com/svg.latex?f%28x%2Cy%2Ch%29). Thus we have the following relationship:

![](https://latex.codecogs.com/svg.latex?g%28x-h%5Calpha%2Cy%29%3Dg%28x%2Bh%5Calpha%2Cy%29%3Df%28x%2Cy%2Ch%29)

where ![](https://latex.codecogs.com/svg.latex?g%28x%2Cy%29) represent the color on point ![](https://latex.codecogs.com/svg.latex?%28x%2Cy%29) on the 2D plane.



In reality, we will need several image and a height map ![](https://latex.codecogs.com/svg.latex?h%28x%2Cy%29).

**Method 1 :** Method 1 is a neat trick, we assume that ![](https://latex.codecogs.com/svg.latex?%5Cfrac%7B%5Cpartial%20h%7D%7B%5Cpartial%20x%7D%5Calpha%3C1). thus ![](https://latex.codecogs.com/svg.latex?x%5Cto%20x-h%5Calpha) and ![](https://latex.codecogs.com/svg.latex?x%5Cto%20x%2Bh%5Calpha) are both injections. Then we can deduct the following algorithm:

for ![](https://latex.codecogs.com/svg.latex?y%3D1%5Cto%20y_%7Bmax%7D)

---for ![](https://latex.codecogs.com/svg.latex?x%3D1%5Cto%20x_%7Bmax%7D)

------![](https://latex.codecogs.com/svg.latex?g%28x%2Bh%28x%2Cy%29%20%5Calpha%2Cy%29%3Dg%28x-h%28x%2Cy%29%5Calpha%2Cy%29)

If nothing is present (only plain background of height ![](https://latex.codecogs.com/svg.latex?h_0)), the image ![](https://latex.codecogs.com/svg.latex?g%28x%2Cy%29) will be a periodic image with period ![](https://latex.codecogs.com/svg.latex?2h_0%5Calpha), thus we might as well use a periodic image at the beginning(better visual effect, less computation), and let the object has height of ![](https://latex.codecogs.com/svg.latex?h%3Dh_0-h_%7Badd%7D), where![](https://latex.codecogs.com/svg.latex?h_%7Badd%7D) is the height map of the object.

*Merits:*

1. Easy to implement.
2. The resulting image will highly resembles the background image, thus the pattern will be highly invisible.

*Drawbacks:*

1. Requires ![](https://latex.codecogs.com/svg.latex?%5Cfrac%7B%5Cpartial%20h%7D%7B%5Cpartial%20x%7D%5Calpha%3C1) , or else will have defects, thus cannot create image with strong 3D feeling.
2. The mapping between original background and the resulting image is unclear, thus cannot effectly map the texture.



**Method 2:** Method 2 is directly derived from the original algorithm, we will directly map the texture ![](https://latex.codecogs.com/svg.latex?f%28x%2Cy%2Ch%28x%2Cy%29%29) to ![](https://latex.codecogs.com/svg.latex?g%28x%2Cy%29) . It is evident that this algorithm can be executed independently for each ![](https://latex.codecogs.com/svg.latex?y), we will omit that in further discussion.

for ![](https://latex.codecogs.com/svg.latex?y%3D1%5Cto%20y_%7Bmax%7D)

1. Create map from edges ![](https://latex.codecogs.com/svg.latex?f%28x%29%5Cleftrightarrow%20g%28x-h%28x%29%5Calpha%29),![](https://latex.codecogs.com/svg.latex?f%28x%29%5Cleftrightarrow%20g%28x%2Bh%28x%29%5Calpha%29).
2. Find all connected components. It should be something like: ![](https://latex.codecogs.com/svg.latex?f%28x_0%29%5Cleftrightarrow%20g%28x_1%29%5Cleftrightarrow%20f%28x_2%29%5Cleftrightarrow%20g%28x_3%29%2C%20f%28x_2%29%5Cleftrightarrow%20g%28x_4%29). This means if we want to create a image with correct height and texture, the texture ![](https://latex.codecogs.com/svg.latex?f%28x_0%29) and ![](https://latex.codecogs.com/svg.latex?f%28x_2%29) should be the same (if not, we can simply substitute ![](https://latex.codecogs.com/svg.latex?f%28x_2%29) by![](https://latex.codecogs.com/svg.latex?f%28x_0%29)), and the points on the image ![](https://latex.codecogs.com/svg.latex?g%28x_1%29%2C%20g%28x_3%29%2Cg%28x_4%29) should be the same.
3.  Now set ![](https://latex.codecogs.com/svg.latex?g%28x_1%29%2C%20g%28x_3%29%2Cg%28x_4%29%3Df%28x_0%29) or something similar for all connected components.

*Merits:*

1. Accurate relationship, especially effective with large![](https://latex.codecogs.com/svg.latex?h_%7Badd%7D), thus can create image with strong 3D feeling.
2. Accurate pixel map, can map textures effectively. 

*Drawbacks:*

1. Less straight forward code.
2. Slow in execution.

## Acknowledgement

Special Thanks to njpipeorgan for his marvelous [wll-interface](https://github.com/njpipeorgan/wll-interface)!
