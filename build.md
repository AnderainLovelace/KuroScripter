# 编译方法
## Visual Studio
先Fork下nSDL，编译，拷贝出生成的SDL.lib,SDLmain.lib至lib文件夹，拷贝SDL.dll至生成目录。  
再下载DSL_image 1.2的VC中间件，将SDL_image.lib放至lib文件夹，将SDL_image.dll与libjpeg8.dll放至生成目录。  
下载nSDL与其nSDL_image插件，解压放至根目录，添加文件，编译即可  

## ndless-sdk
下载nSDL与其nSDL_image插件，解压放至根目录  
按照官网指示安装ndless SDK，在src目录make即可  