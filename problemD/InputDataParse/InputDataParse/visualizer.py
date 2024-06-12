import matplotlib.pyplot as plt
import numpy as np
from matplotlib.widgets import Button

# 讀取文件並解析數據
blocks = {}
directions = {}
label_lacations = {}

with open('block.txt', 'r') as file:
    for line in file:
        parts = line.split()
        block_name = parts[0]
        direction = parts[-1]
        coordinates = []

        for coord in parts[1:-1]:
            coord = coord.strip('(),')
            x, y = map(int, coord.split(','))
            coordinates.append((x, y))
        
        # 如果只有兩個點，補齊成矩形
        if len(coordinates) == 2:
            x1, y1 = coordinates[0]
            x2, y2 = coordinates[1]
            coordinates.insert(1, (x1, y2))
            coordinates.append((x2, y1))

        blocks[block_name] = coordinates
        directions[block_name] = direction

# 根據方向調整坐標
def rotate_coordinates(points, direction, center_x, center_y, min_x, min_y):
    rotated_points = []
    for x, y in points:
        # 以中心點為原點進行變換
        x -= center_x
        y -= center_y
        nx = 0
        ny = 0
        if direction == 'N':
            nx, ny = x, y
        elif direction == 'FN':
            nx, ny = -x, y
        elif direction == 'S':
            nx, ny = -x, -y
        elif direction == 'FS':
            nx, ny = x, -y
        elif direction == 'W':
            nx, ny = -y, x
        elif direction == 'FW':
            nx, ny = y, x
        elif direction == 'E':
            nx, ny = y, -x
        elif direction == 'FR':
            nx, ny = -y, -x

        rotated_points.append((nx + center_x, ny + center_y))

    # 補上第一個點，使其成為封閉圖形
    rotated_points.append(rotated_points[0])
        
    nmin_x = min(x for x, y in rotated_points)
    nmin_y = min(y for x, y in rotated_points)

    # 調整坐標，使其保持原來的位置
    dx = nmin_x - min_x
    dy = nmin_y - min_y

    rotated_points = [(x - dx, y - dy) for x, y in rotated_points]
    label_lacations[block] = (center_x - dx, center_y - dy)

    return rotated_points

# 創建圖形
fig, ax = plt.subplots(figsize=(12, 10))
plt.subplots_adjust(bottom=0.2, right=0.8)  # 調整子圖布局，給右邊留出空間

# 繪製每個Block並計算中心位置
block_centers = {}
for block, points in blocks.items():
    x, y = zip(*points)  # 拆解坐標
    # 找最左下角的點
    min_x = min(x)
    min_y = min(y)
    # 找最右上角的點
    max_x = max(x)
    max_y = max(y)
    # 計算中心位置
    center_x = (min_x + max_x) / 2
    center_y = (min_y + max_y) / 2
    block_centers[block] = (center_x, center_y)
    
    direction = directions[block]
    rotated_points = rotate_coordinates(points, direction, center_x, center_y, min_x, min_y)
    x, y = zip(*rotated_points)
    
    ax.plot(x, y, label=block)  # 繪製折線
    ax.fill(x, y, alpha=0.3)  # 填充Block內部

    # 初始情況顯示標籤
    ax.text(center_x, center_y, block, ha='center', va='center')

# 按鈕事件處理函數
def toggle_labels(event):
    for block, (center_x, center_y) in label_lacations.items():
        if toggle_labels.display_labels:
            ax.text(center_x, center_y, block, ha='center', va='center')
        else:
            for txt in ax.texts:
                if txt.get_text() == block:
                    txt.set_visible(False)
    toggle_labels.display_labels = not toggle_labels.display_labels
    plt.draw()

toggle_labels.display_labels = True

# 添加按鈕
ax_button = plt.axes([0.8, 0.05, 0.1, 0.075])
button = Button(ax_button, 'Toggle Labels')
button.on_clicked(toggle_labels)

# 設置圖形標題和標籤
ax.set_title("Block Shapes")
ax.set_xlabel("X Coordinate")
ax.set_ylabel("Y Coordinate")

# 顯示圖例，並將其移動到右邊
ax.legend(loc='center left', bbox_to_anchor=(1, 0.5))

# 顯示圖形
plt.show()





