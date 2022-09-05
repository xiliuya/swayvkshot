# swayvkshot
Sway vulkan 下的截图工具
Sway screenshots  for vulkan

    基于 wlroots/examples/screencopy-dmabuf 修改
    based on wlroots/examples/screencopy-dmabuf
    保存路径：/home/$USER/Pictures/
    save path :/home/$USER/Pictures/


使用说明：
Use this tool:
    swayvkshot
      获得全屏截图
	    take a full screenshot

    swayvkshot -g "$(slurp)"
      局部截图
	    take a screenshot of a part of the screen

    swayvkshot -o filename.png
      截图保存到指定文件
	    take a screenshot of filename.png

    wl-copy < $(swayvkshot)
      复制到剪贴板
	    put the output into the clipboard

依赖包：
Install dependencies:

* meson
* wayland
* wayland-protocols
* Vulkan loader, headers and glslang (optional, for the Vulkan renderer)
* wlroots
* [libseat]

编译：
Run these commands:

    meson build/
    ninja -C build/

安装：
Install like so:

    sudo cp build/screenshot /usr/bin/



[Wayland]: https://wayland.freedesktop.org/
[Wlroots]: https://gitlab.freedesktop.org/wlroots/wlroots/-/wikis/Getting-started
[#sway-devel on Libera Chat]: https://web.libera.chat/gamja/?channels=#sway-devel
[libseat]: https://git.sr.ht/~kennylevinsen/seatd
