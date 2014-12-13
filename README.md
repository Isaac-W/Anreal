Anreal
======

This project is currently under development, and is expected to be finalized by mid-2015.

The Android Virtual Reality system, or Anreal for short, is a system designed to provide an easy way to convert an Android smartphone or tablet into a head-mounted virtual reality display. Originally inspired by the Oculus Rift, a commercial virtual reality headset currently in development, Anreal aims to mimic the Rift's functionality by providing an open-source software-based implementation.

Anreal is designed as two separate applications: the server, which runs on a Windows PC, and the client, which runs on an Android device. The server program streams screen captures of a stereoscopic 3D-enabled PC game to the Android device, which is then displayed on the device screen. The client program sends head-tracking orientation data to the PC, which is converted into camera/view movements in the game.

(Full instructions on how to use Anreal will be posted in the near future.)

To see pictures of me demonstrating Anreal at Seattle Pacific University's Erickson Research Conference, visit http://isaac-w.github.io/Anreal

======

This repository contains the source code for the server application. For the Android client app, go here: https://github.com/Isaac-W/Anreal-Client

======

This project builds under Visual Studio 2012. VS2012 Professional is required, as the project uses .rc resource files, which are not available under the Express editions. WTL, the DirectX SDK, and libjpeg-turbo are required dependencies needed for building.

(Full instructions on how to setup the development environment will be released in the near future.)
