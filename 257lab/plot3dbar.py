import matplotlib.pyplot as plt
from matplotlib import cm
import numpy as np
from mpl_toolkits.mplot3d import Axes3D

fig = plt.figure()
cyl = fig.add_subplot(111, projection='3d')

# Cylinder
x=np.linspace(-1, 1, 100)
z=np.linspace(-12, 12, 100)
Xc, Zc=np.meshgrid(x, z)
Yc = np.sqrt(1-Xc**2)

# Draw parameters
rstride = 20
cstride = 10
cyl.plot_surface(Xc, Yc, Zc, alpha=0.9, rstride=rstride, cstride=cstride, cmap='coolwarm')
cyl.plot_surface(Xc, -Yc, Zc, alpha=0.9, rstride=rstride, cstride=cstride, cmap='coolwarm')

### Begin scaling plot for aesthetics
plt.axis([-3,3,-3,3])

x_scale=3
y_scale=3
z_scale=12

scale=np.diag([x_scale, y_scale, z_scale, 1.0])
scale=scale*(1.0/scale.max())
scale[3,3]=1.0

def short_proj():
  return np.dot(Axes3D.get_proj(cyl), scale)

cyl.get_proj=short_proj
### End scaling

cyl.set_xlabel("X")
cyl.set_ylabel("Y")
cyl.set_zlabel("Z")

plt.show()
