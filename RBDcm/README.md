# RBDCM图像分割
RBDcm整个程序架构如下图所示，软件后台图像数据读取依靠DCMTK开源库程序。对图像的处理及分割算法的实现基于OpenCV开源库。整个的软件界面由MFC（vc2010版本）实现（包括GDI绘图，图层管理，用户交互等）。
![image](https://github.com/bzhou830/images/raw/master/1.png)

RBDcm工具目前支持的功能：
1、Dicom图像的查看；
2、对图像的基本处理（均值滤波，中值滤波，Sobel算子，Prewitt算子，Roberts算子，拉普拉斯算子）；
3、阈值分割(手动阈值，熵分割，OSTU分割，矩量分割)；
4、直方图绘制，全局直方图增强；
5、保存处理后图像（8bit bmp格式）。

预览一下工作的界面。
左侧是工作功能选择区，黑底界面就是图像的处理预览界面。悬浮于图像上层的黄色字体为Dcm文件中与Patient相关的数据。
![image](https://github.com/bzhou830/images/raw/master/20151117_2.png)

直方图绘制效果。支持鼠标交互选中灰度值，标注出灰度值的像素数。
![image](https://github.com/bzhou830/images/raw/master/dcm_hist1.jpg)

分割后图像8领域拉普拉斯得到边缘结果：
![image](https://github.com/bzhou830/images/raw/master/dcm_edgs.jpg)

阈值分割处理后效果
![image](https://github.com/bzhou830/images/raw/master/3.jpg)

对图像处理分割后hist增强：
![image](https://github.com/bzhou830/images/raw/master/dcm_seg_hence.jpg)

对比处理前图像，工作区展示的是按照工作区大小自适应调整大小后的图像，相比原图像有尺寸上的变化。
响应的在视觉上也会有清晰度的差别
![image](https://github.com/bzhou830/images/raw/master/dcm_compare.jpg)

分割出肺实质区的效果
![image](https://github.com/bzhou830/images/raw/master/4.jpg)



