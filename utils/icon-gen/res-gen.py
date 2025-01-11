from PIL import Image
import os

def image_to_c_array(image_path, image_name, target_width=None, target_height=None):
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
    result = header + f"pixel_t {image_name}[{height}][{width}] = {{\n"

    for y in range(height):
        row = "    {"
        for x in range(width):
            r, g, b, a = pixels[x, y]
            alpha = 255 - a
            # 如果alpha是0xff,使用0xff000000,否则使用0xaarrggbb格式
            if alpha == 0xff:
                row += "0xff000000"
            else:
                row += f"0x{alpha:02x}{r:02x}{g:02x}{b:02x}"
            if x < width - 1:
                row += ", "
        row += "},\n"
        result += row

    result += "};"
    return result

# 自动处理所有图像文件
def process_all_images(directory, target_width=None, target_height=None):
    for filename in os.listdir(directory):
        if filename.endswith(".png") or filename.endswith(".jpg"):
            image_path = os.path.join(directory, filename)
            image_name = os.path.splitext(filename)[0]
            c_array = image_to_c_array(image_path, image_name, target_width, target_height)

            # 将结果写入文件
            output_file = f"{image_name}.res.h"
            with open(output_file, "w") as f:
                f.write(c_array)
            print(f"Generated {output_file}")

# 使用示例
import sys
if len(sys.argv) == 2:
    # 提供目录路径
    directory = sys.argv[1]
    process_all_images(directory)
elif len(sys.argv) == 4:
    # 提供目录路径和目标尺寸
    directory = sys.argv[1]
    width = int(sys.argv[2])
    height = int(sys.argv[3])
    process_all_images(directory, width, height)
else:
    print("Usage: python icon-gen.py <directory> [width height]")
    sys.exit(1)
