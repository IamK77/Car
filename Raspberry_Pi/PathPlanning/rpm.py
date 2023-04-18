import math

# 定义变量

class rpm_:
    def __init__(self) -> None:
        self.r = 10 # 物体半径，单位为米
        self.v0 = 5 # 初始速度，单位为米/秒
        self.t = 10 # 时间，单位为秒
        self.dt = 0.05 # 检测间隔，单位为秒
        self.v_list = [self.v0] # 速度列表，初始值为v0
        # 定义一个变量，存储总位移
        self.total_s = 0


    # 定义一个函数，根据rpm计算线速度
    def get_linear_velocity(self, rpm):
        # 使用线速度公式
        v = self.r * rpm * 0.10472
        return v

    # 定义一个函数，根据rpm计算角速度
    def get_angular_velocity(self, rpm):
        # 使用角速度公式
        w = rpm * 0.10472
        return w

    # 定义一个函数，根据速度和时间计算位移
    def get_displacement(self, v):
        # 使用位移公式
        s = v * self.t
        return s

    

    # 使用循环，每隔dt秒检测一次rpm，并计算位移
    for i in range(int(t / dt)):
        # 获取当前rpm（这里可以根据实际情况修改）
        rpm = 100 + i * 10 # 假设rpm是线性增加的
        # 计算当前线速度
        v = self.get_linear_velocity(rpm)
        # 计算当前角速度
        w = self.get_angular_velocity(rpm)
        # 计算当前位移
        s = get_displacement(v, self.dt)
        # 累加总位移
        self.total_s += s
        # 添加当前速度到列表中
        self.v_list.append(v)

    # 计算物体的位置坐标
    x = r * math.cos(total_s / r) # x坐标公式，假设物体沿着x轴正方向运动
    y = r * math.sin(total_s / r) # y坐标公式   

# 输出结果
# print(f"物体在{t}秒后的位移是{total_s:.2f}米")
# print(f"物体在{t}秒后的位置是({x:.2f}, {y:.2f})")