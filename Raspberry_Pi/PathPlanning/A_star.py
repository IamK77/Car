import matplotlib.pyplot as plt
import math
import time

show_animation = True


class Dijkstra:

    def __init__(self, ox, oy, resolution, robot_radius):

        self.min_x = None
        self.min_y = None
        self.max_x = None
        self.max_y = None
        self.x_width = None
        self.y_width = None
        self.obstacle_map = None

        self.resolution = resolution
        self.robot_radius = robot_radius
        self.calc_obstacle_map(ox, oy)
        self.motion = self.get_motion_model()

    class Node:  # 定义一个对象Node, 它的attr为   栅格x, 栅格y, 权重, 父节点
        def __init__(self, x, y, cost, parent_index):  # 230, 230, 0, -1
            self.x = x  # 类中的x, y均为栅格图中的值
            self.y = y
            self.cost = cost
            self.parent_index = parent_index

        def __str__(self):
            return str(self.x) + "," + str(self.y) + "," + str(
                self.cost) + "," + str(self.parent_index)

    def planning(self, sx, sy, gx, gy, sc):  # 起点x   起点y     终点x    终点y

        start_node = self.Node(self.calc_xy_index(sx, self.min_x),
                               self.calc_xy_index(sy, self.min_y), 0.0, -1)
        goal_node = self.Node(self.calc_xy_index(gx, self.min_x),
                              self.calc_xy_index(gy, self.min_y), 0.0, -1)

        open_set, closed_set = dict(), dict()  # 字典类型      设定已知节点 和未知节点
        open_set[self.calc_index(start_node)] = start_node

        while True:
            c_id = min(open_set, key=lambda o: open_set[o].cost + self.Manhattan_Distance(node=open_set[o],
                                                                                          goal_node=goal_node,
                                                                                          weight=2))
            current = open_set[c_id]  # 并返回他的key (dict_key, not func_key)

            if show_animation:
                plt.plot(self.calc_position(current.x, self.min_x),  # matplotlib可视化部分 code 50 ~ 57
                         self.calc_position(current.y, self.min_y), sc)

                plt.gcf().canvas.mpl_connect(
                    'key_release_event',
                    lambda event: [exit(0) if event.key == 'escape' else None])
                if len(closed_set.keys()) % 10 == 0:
                    plt.pause(0.001)

            if current.x == goal_node.x and current.y == goal_node.y:  # 如果该节点的x, y坐标与终点相符, 终止循环
                print("Find goal")
                goal_node.parent_index = current.parent_index  # 将该节点的父节点值赋值给终点父节点
                goal_node.cost = current.cost  # ~~~~~~~~权重(代价)~~~~~~~~~~~
                break

            del open_set[c_id]  # 如果不相符，将该节点c_id从未知集合中删去

            closed_set[c_id] = current  # c_id添加到已知集合中

            for move_x, move_y, move_cost in self.motion:  # 尝试移动, 在八种运动方向中尝试
                node = self.Node(current.x + move_x,
                                 current.y + move_y,
                                 current.cost + move_cost, c_id)
                n_id = self.calc_index(node)  # 获取移动后节点在栅格地图中的编号n_id(目前移动为尝试性移动)

                if n_id in closed_set:  # 查询当前编号是否已经在已知集合中
                    continue

                if not self.verify_node(node):  # 验证移动合法性
                    continue

                if n_id not in open_set:  # 检查当前编号n_id是否已在未知编号中被移除
                    open_set[n_id] = node  # 更新状态
                else:
                    if open_set[n_id].cost >= node.cost:  # 如果未知编号中的过时权重大于当前权重, 则更新他的cost
                        open_set[n_id] = node

        rx, ry = self.calc_final_path(goal_node, closed_set)
        plt.plot(rx, ry, "-r")
        # return rx, ry

    def calc_final_path(self, goal_node, closed_set):  # 最终路径

        rx, ry = [self.calc_position(goal_node.x, self.min_x)], [
            self.calc_position(goal_node.y, self.min_y)]
        parent_index = goal_node.parent_index  # 赋值给父节点坐标索引
        while parent_index != -1:  # 由终点被更新的父节点值一路回溯至起点 详见 code 38, 40
            n = closed_set[parent_index]  # 从终点的父节点开始 逆推出第一个路径点n(终点的父节点)
            rx.append(self.calc_position(n.x, self.min_x))  # 路径图列表
            ry.append(self.calc_position(n.y, self.min_y))
            parent_index = n.parent_index  # 将n值的父节点赋值给父节点索引, 在下一次的循环中, 将从n值的父节点开始

        return rx, ry

    @staticmethod
    def Euclidean_Distance(node: Node, goal_node: Node, weight=1):
        x = node.x - goal_node.x
        y = node.y - goal_node.y
        return math.hypot(x, y) * weight

    @staticmethod
    def Manhattan_Distance(node: Node, goal_node: Node, weight=1):
        x = node.x - goal_node.x
        y = node.y - goal_node.y
        return (abs(x) + abs(y)) * weight

    def calc_position(self, index, minp):  # 确定每个栅格的真实坐标值   x * y的格子图中 x乘格子的边长再加格子的x最小值即为x的真实坐标
        pos = index * self.resolution + minp
        return pos

    def calc_xy_index(self, position, minp):  # 起点坐标 - 地图原点坐标 / 栅格尺寸  确定在栅格图中的索引值
        return round((position - minp) / self.resolution)

    def calc_index(self, node: Node):  # 由原点开始栅格图中对每个格子进行编号  1, 2, 3, 4, 5...10 第一行完毕后, 在第二行继续 11, 12...
        return (node.y - self.min_y) * self.x_width + (node.x - self.min_x)  # 230-0 * 48 + 230

    def verify_node(self, node: Node):  # 验证移动是否合法
        px = self.calc_position(node.x, self.min_x)
        py = self.calc_position(node.y, self.min_y)

        if px < self.min_x:  # 是否超出地图
            return False
        if py < self.min_y:
            return False
        if px >= self.max_x:
            return False
        if py >= self.max_y:
            return False

        if self.obstacle_map[node.x][node.y]:  # 是否为不可通行区域
            return False

        return True

    def calc_obstacle_map(self, ox, oy):

        self.min_x = round(min(ox))  # 浮点数处理
        self.min_y = round(min(oy))
        self.max_x = round(max(ox))
        self.max_y = round(max(oy))
        print("min_x:", self.min_x)
        print("min_y:", self.min_y)
        print("max_x:", self.max_x)
        print("max_y:", self.max_y)

        self.x_width = round((self.max_x - self.min_x) / self.resolution)  # 绘制地图大小
        self.y_width = round((self.max_y - self.min_y) / self.resolution)  # y/x的最大值减最小值, 除以每个格子的大小, 得到一张格子图
        print("x_width:", self.x_width)
        print("y_width:", self.y_width)

        self.obstacle_map = [[False for _ in range(self.y_width)]  # 建立一个双层列表list(二维数组)每个值都为False
                             for _ in range(self.x_width)]  # False表示非障碍物
        for ix in range(self.x_width):  # 绘制障碍物
            x = self.calc_position(ix, self.min_x)  # 确定每个障碍物的坐标值
            for iy in range(self.y_width):
                y = self.calc_position(iy, self.min_y)
                for iox, ioy in zip(ox, oy):  # 此时 x, y, ox, oy均为真实坐标
                    d = math.hypot(iox - x, ioy - y)  # 将围墙设为True
                    if d <= self.robot_radius:  # 障碍物与栅格距离小于机器人尺寸, 不可通行
                        self.obstacle_map[ix][iy] = True
                        break

    @staticmethod
    def get_motion_model():

        motion = [[1, 0, 1],  # 运动模式  列表(数组)内
                  [0, 1, 1],  # 第一个值为x方向 1: 向右    -1: 向左         0  1  0    由1移动到2为向下移动, 因此为[0, -1, 1]
                  [-1, 0, 1],  # 第二个值为y方向 1: 向上    -1: 向下        0  2  0    要跨过1个距离, 所以权重是1
                  [0, -1, 1],  # 第三个值为权重(代价)                      0  0  3    2到3同理[1, -1, math.sqrt(2)]
                  [-1, -1, math.sqrt(2)],
                  [-1, 1, math.sqrt(2)],
                  [1, -1, math.sqrt(2)],
                  [1, 1, math.sqrt(2)]]

        return motion


def map_rectangle(max_x_mx, max_y_mn, min_x=None, min_y=None):
    """
    用于生成矩形围墙\n
    如果右上角和左下角的x, y值均相同, 则可以只使用前两种参数\n
    max_x_mx: 正方形右上角坐标值\n
    max_y_mn: 正方形左上角坐标值\n
    :param max_x_mx: 参数应为该矩形右上角的x值
    :param max_y_mn: 参数应为该矩形右上角的y值
    :param min_x: 默认为None值, 参数应为该矩形左下角的x值
    :param min_y: 默认为None值, 参数应为该矩形左下角的y值
    :return:
    """
    if min_x and min_y:
        ox, oy = [min_x for _ in range(min_y, max_y_mn)], [i for i in range(min_y, max_y_mn)]
        ox.extend([i for i in range(min_x, max_x_mx)])
        ox.extend([max_x_mx for _ in range(min_y, max_y_mn)])
        ox.extend([i for i in range(min_x, max_x_mx)])
        oy.extend([min_y for _ in range(min_x, max_x_mx)])
        oy.extend([i for i in range(min_y, max_y_mn)])
        oy.extend([max_y_mn for _ in range(min_x, max_x_mx)])
        return ox, oy
    else:
        ox, oy = [max_y_mn for _ in range(max_y_mn, max_x_mx)], [i for i in range(max_y_mn, max_x_mx)]
        ox.extend((i for i in range(max_y_mn, max_x_mx)))
        oy.extend((max_y_mn for _ in range(max_y_mn, max_x_mx)))
        ox.extend((max_x_mx for _ in range(max_y_mn, max_x_mx)))
        oy.extend((i for i in range(max_y_mn, max_x_mx)))
        ox.extend((i for i in range(max_y_mn, max_x_mx)))
        oy.extend((max_x_mx for _ in range(max_y_mn, max_x_mx)))
        return ox, oy


def map_straight(a_x, a_y, b_x, b_y):
    """
    用于在地图中生成直线围墙
    :param a_x: 起点a的x值
    :param a_y: 起点a的y值
    :param b_x: 终点b的x值
    :param b_y: 终点b的y值
    :return:
    """

    if a_y == b_y:
        ox, oy = [i for i in range(a_x, b_x)], [a_y for _ in range(a_x, b_x)]
        return ox, oy
    elif a_x == b_x:
        ox, oy = [a_x for _ in range(a_y, b_y)], [i for i in range(a_y, b_y)]
        return ox, oy
    else:
        print('暂不支持该法')
        return [], []


def draw():
    start_time = time.time()
    sx = 225.0  # 起点
    sy = 225.0
    qrx = 165.0  # 二维码
    qry = 230.0
    orix = 60.0  # 物料区
    oriy = 230.0
    frix = 10.0  # 粗加工
    friy = 120.0
    halfx = 120.0  # 半成品
    halfy = 10.0
    gx = 230.0  # 终点
    gy = 10.0
    grid_size = 5.0  # 节点(栅格)尺寸
    robot_radius = 15.0

    ox, oy = [], []
    to_ox, to_oy = map_rectangle(240, 0)
    ox.extend(to_ox)
    oy.extend(to_oy)
    to_ox, to_oy = map_rectangle(160, 80)
    ox.extend(to_ox)
    oy.extend(to_oy)
    # to_ox, to_oy = map_straight(50, 0, 50, 200)
    # ox.extend(to_ox)
    # oy.extend(to_oy)

    if show_animation:
        plt.plot(ox, oy, ".k")
        plt.plot(sx, sy, qrx, qry, orix, oriy, frix, friy, halfx, halfy, "og")
        plt.plot(gx, gy, "xb")
        plt.grid(True)
        plt.axis("equal")

    dijkstra = Dijkstra(ox, oy, grid_size, robot_radius)
    dijkstra.planning(sx, sy, qrx, qry, ".c")
    dijkstra.planning(qrx, qry, orix, oriy, ".y")
    dijkstra.planning(orix, oriy, frix, friy, ".b")
    dijkstra.planning(frix, friy, halfx, halfy, ".g")
    dijkstra.planning(halfx, halfy, gx, gy, ".c")
    end_time = time.time()
    print(f'所用时长: {round(end_time - start_time, 3)}')

    plt.pause(0.01)
    plt.show()


if __name__ == '__main__':
    draw()

# TODO 优化性能
# Fork on https://github.com/AtsushiSakai/PythonRobotics


# 基于 https://github.com/AtsushiSakai/PythonRobotics 进行改写
# requests: Python 3.10.x
#           matplotlib      my version: 3.6.2 for example
#           math
# 坐标轴   地图    栅格图

# 流程:
'''
栅格: 节点, 权重: 代价
这个写法中共有三个意义上的地图: 我们定义的地图范围(比赛场地), 坐标轴, 栅格(格子)图
以比赛场地的左下角, 右上角 为坐标轴的最小, 最大值 min, max
栅格图算法:  x轴 (max_x - min_x) / grid_size(格子尺寸) 并通过银行家取舍去除浮点数      y轴算法相同
1. 入口函数draw()
2. 定义起点, 终点...的值, 通过for定义障碍的坐标
3. 实例化对象Dijkstra, 并输入参数初始化 __init__()
4. 调用calc_obstacle_map()函数进行地图的初始化  调用get_motion_model()函数进行机器人运动方式的初始化
5. 定义一个对象Node, 它的attr为 栅格x, 栅格y, 权重cost, 父节点    其中权重为该节点在未知集合中时, 到已知集合中最近点的距离
6. 调用函数planning()
7. planning()中, 首先初始化起点和终点的Node 其中权重为0, 父节点为-1
8. 进入循环
9. 寻找未知集合中权重最小的节点, 并返回它在栅格图中的编号, 首先检查该点是否为终点, 如果是, 终止循环, 否, 继续
10. code 69 对机器人的八种运动方向进行迭代, 检查此次模拟移动是否合法, 如果合法则更新未知集合中的数据
11. 抵达终点后调用calc_final_path(), 通过父节点回溯路径

          .......
          21 22 ..........29 30
          11 12 ...       19 20
栅格编号:   1 2 3 4 5 6 7 8 9 10
'''
