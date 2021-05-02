# MDK wrapper for Qt Quick

MDK wrapper for Qt Quick. Can be used as a normal visual element in .qml files easily.

## Features

- Can be easily embeded into any Qt Quick GUI applications.
- Support both shared and static ways of linking.
- Cross-platform support: Windows, Linux, macOS, Android, iOS, UWP, etc.

## Usage

See [example](/example/qml/main.qml).

## Compilation

**Since this project makes heavy usage of Qt RHI, you'll need at least Qt 5.14 to build this project. It's recommended to use Qt 5.15 or Qt 6.**

1. Checkout source code:

   ```bash
   git clone https://github.com/wangwenx190/mdk-qtquick-plugin.git
   ```

   Note: Please remember to install *Git* yourself. Windows users can download it from: <https://gitforwindows.org/>

2. Setup MDK SDK:

   Download the official MDK SDK from: <https://sourceforge.net/projects/mdk-sdk/files/>. Then extract it to anywhere you like.

   Once everything is ready, then create an environment variable named `MDK_SDK_DIR`, which contains the full absolute path of that location. You can also pass it to CMake as a command line parameter when configuring.

3. Create a directory for building:

   Linux:

   ```bash
   mkdir build
   cd build
   ```

   Windows (cmd):

   ```bat
   md build
   cd build
   ```

   Windows (PowerShell):

   ```powershell
   New-Item -Path "build" -ItemType "Directory"
   Set-Location -Path "build"
   ```

4. Build and Install:

   ```bash
   cmake ..
   cmake --build .
   cmake --install .
   ```

## FAQ

- How to enable hardware decoding?

  ```qml
  MDKPlayer {
      // ...
      hardwareDecoding: true // default is false
      // ...
  }
  ```

  Hardware decoding is disabled by default, you have to enable it manually if you want to use it.

- Why is the playback process not smooth enough or even stuttering?

   If you can insure the video file itself isn't damaged, here are two possible reasons and their corresponding solutions:
   1. You are using **software decoding** instead of hardware decoding.

      MDK will not enable **hardware decoding** by default. You will have to enable it manually if you need it. Please refer to the previous topic to learn about how to enable it.
   2. You need a more powerful GPU, maybe even a better CPU. MDK is never designed to run on too crappy computers.

- How to set the log level of MDK?

    ```qml
   import wangwenx190.MDKWrapper 1.0

   MDKPlayer {
       // ...
       logLevel: MDKPlayer.LogLevel.Debug // type: enum
       // ...
   }
   ```

- Why my application complaints about `failed to create EGL context ... etc` at startup and then crashed?

   ANGLE only supports OpenGL version <= 3.2. Please check whether you are using OpenGL newer than 3.2 through ANGLE or not.

   Desktop OpenGL doesn't have this limit. You can use any version you like. The default version that Qt uses is 2.0, which I think is kind of out-dated.

   Here is how to change the OpenGL version in Qt:

   ```cpp
   QSurfaceFormat surfaceFormat = QSurfaceFormat::defaultFormat();
   // Here we use desktop OpenGL. To switch to OpenGLES, use QSurfaceFormat::OpenGLES
   surfaceFormat.setRenderableType(QSurfaceFormat::OpenGL);
   // Here we use OpenGL version 4.6 for instance.
   // Don't use any versions newer than 3.2 if you are using ANGLE.
   surfaceFormat.setVersion(4, 6);
   // You can also use "QSurfaceFormat::CoreProfile" to disable the using of deprecated OpenGL APIs, however, some deprecated APIs will still be usable.
   surfaceFormat.setProfile(QSurfaceFormat::CompatibilityProfile);
   QSurfaceFormat::setDefaultFormat(surfaceFormat);
   ```
