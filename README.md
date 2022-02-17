#东方RushBoss 编译说明
************************
本源代码在VS2019环境下编译通过，建议在VS2019或更高版本VS中进行编译。
###准备工作
 * 下载本页面src文件夹中的源代码。
 * 下载东方RushBoss可执行程序，里面有程序需要用到的资源文件。[点击下载](https://tapcat.top/Project/TouhouRushBoss/thrb.aspx)
 * 下载以下程序编译所需的SDK（暂不开源）。
 
 VirtualLib:[点击下载](http://download.tapcat.top/VirtualLib/VirtualLib.zip)

 ManagedStream:[点击下载](http://download.tapcat.top/ManagedStream/ManagedStream.zip)
 * 解压下载的压缩包。

###环境配置
 * 打开东方RushBoss.sln，将解决方案配置切换为Release。（如果已经是Release则不用切换）
 * 点击上方菜单中的项目 - 属性，将配置切换为Release。（如果已经是Release则不用切换）
 * 点击VC++目录，打开包含目录的下拉菜单，删除最上方列表中的两个路径，分别添加以下两个路径：VirtualLib.lib所在的目录和ManagedStream_Static.lib所在的目录。
 * 打开库目录的下拉菜单，重复以上的操作。

###编译代码
 * 在开始编译前，首先将游戏所用到的资源文件（thsrc.smx）复制到东方RushBoss.sln所在的目录下，就可以对程序进行调试了。