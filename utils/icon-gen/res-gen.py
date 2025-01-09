from PIL import Image

def image_to_c_array(image_path, resource_name, target_width=None, target_height=None):
    # 打开图像
    img = Image.open(image_path).convert("RGBA")

    # 如果没有指定目标尺寸，使用原始尺寸
    if target_width is None or target_height is None:
        width, height = img.size
    else:
        width, height = target_width, target_height
        # 调整图像大小
        img = img.resize((width, height), Image.Resampling.LANCZOS)

    # 获取像素数据
    pixels = img.load()

    header = """#pragma once
#include <stdint.h>
#include <graphics.h>

"""

    # 初始化结果字符串
    result = header + f"pixel_t {resource_name}[{height}][{width}] = {{\n"

    for y in range(height):
        row = "    {"
        for x in range(width):
            r, g, b, a = pixels[x, y]
            alpha = 255 - a
            # 否则，使用0xrrggbb格式
            row += f"0x{alpha:02x}{r:02x}{g:02x}{b:02x}"
            if x < width - 1:
                row += ", "
        row += "},\n"
        result += row

    result += "};"
    return result

# 使用示例
import sys
if len(sys.argv) == 3:
    # 提供图片路径和资源名称
    image_path = sys.argv[1]
    resource_name = sys.argv[2]
    c_array = image_to_c_array(image_path, resource_name)
elif len(sys.argv) == 5:
    # 提供图片路径、资源名称和目标尺寸
    image_path = sys.argv[1]
    resource_name = sys.argv[2]
    width = int(sys.argv[3])
    height = int(sys.argv[4])
    c_array = image_to_c_array(image_path, resource_name, width, height)
else:
    print("Usage: python icon-gen.py <image_path> <resource_name> [width height]")
    sys.exit(1)

# 将结果写入文件
output_file = f"{resource_name}.res.h"
with open(output_file, "w") as f:
    f.write(c_array)
print(f"Generated {output_file}")
