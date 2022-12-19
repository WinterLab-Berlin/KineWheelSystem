# Kine Wheel Arena – DLC

*Kine Wheel Arena – DLC* is an open-source experimental setup for markerless paw tracking of head-fixed mice running on a wheel.
The apparatus is based on the open-source motion tracking system [(KineMouse Wheel)](https://hackaday.io/project/160744-kinemouse-wheel) designed by [Richard Warren](https://richard-warren.github.io/). It has been extended with a camera, lighting, and a controller to run both in synchronization.
Tracking is done using a Deep Neural Network trained with [DeepLabCut](https://www.mackenziemathislab.org/deeplabcut) (DLC). The system is available for purchase at [LABmaker](https://www.labmaker.org/).

![Video Clip Annotated by Deep Neural Network](./Docs/Media/Labeled-Video-Sample-LowRes-12FPS.gif)

*Figure 1 – Video clip showing mouse paws annotated by Deep Neural Network.*

## Table of Contents

- [Requirements](#requirements)
    - [Hardware](#hardware)
    - [Software](#software)
        - [KWA-Controller App](#kwa-controller-app)
        - [pylon Viewer](#pylon-viewer)
            - [Install pylon Viewer](#install-pylon-viewer)
            - [Open Camera](#open-camera)
            - [Load Camera Features](#load-camera-features)
            - [Recording Settings](#recording-settings)
            - [Record](#record)
        - [Anaconda/Miniconda](#anacondaminiconda)
- [Repository Contents](#repository-contents)
- [Installation](#installation)
- [Usage](#usage)
    - [Workflow](#workflow)
    - [I Record video](#i-record-video)
    - [II Analyze video](#ii-analyze-and-label-videos)
        - [Prepare DLC Environment for Inference](#prepare-dlc-environment-for-inference)
        - [Inference](#inference-using-deep-neural-network)
            - [A Run Inference on a Local Machine](#a-run-inference-on-a-local-machine)
            - [B Run Inference on a Cloud Provider](#b-run-inference-on-a-cloud-provider)

## Requirements

### Hardware

*Kine Wheel Arena – DLC* (KWA-DLC) consists of a mouse wheel, camera, lighting, a control unit (Arduino) to synchronize camera and lighting, and a surrounding box which shields the system from interference of external light sources.
Furthermore, the complimentary [software](#software) to operate the system requires a Windows machine.

> **TODO**: Replace with photo of final setup.

![Experimental Setup - Camera View](./Docs/Media/Experimental-Setup-Camera-View.png)

*Figure 2 – Photo of the experimental setup from the view of the camera.*

### Software

For running the system, a Windows machine with the following software installed is required:

- [KWA-Controller App](#kwa-controller-app) – A Graphical User Interface (GUI) application to interface with the Basler camera and the lighting of the system by means of an Arduino. Required for preparing the KWA-DLC system for recording.
- [pylon Viewer](#pylon-viewer) – Basler's camera software suit for configuring and recording on Basler cameras. Required for recording video to storage and loading camera configuration.
- [Anaconda/Miniconda](#anacondaminiconda) – A package and environment management system for Python (and other languages). Required for setting up a Python environment needed to perform tracking of mouse paws in recorded videos.

*Note:* All software listed above can be downloaded free of charge.

#### KWA-Controller App

> **TODO**: Are any SDKs necessary to run the program (.NET, etc.)? If so, list them as requirements.

The *KWA-Controller* app is a small Windows application with a graphical user interface, which allows the user to control the lighting and the camera's rate at which it is capturing images.
It is required to get the system ready to record videos using [pylon Viewer](#pylon-viewer).
The application requires no installation and is available in this repository as `Inference/Arduino/KWA-Controller.exe`.

![Graphical user interface of KWA-Controller](./Docs/Media/KWA-Controller-App-1.png)

*Figure 3a – The graphical user interface of the KWA-Controller app.*

##### Usage

First, ensure that the *KWA-DLC* control unit (Arduino) is connected the Windows machine on which to record video.

Next, determine the COM port on which the control unit is connected to by opening the Windows *Device Manager* (type `devmgmt.msc` in the Windows search bar), expanding the *Ports (COM & LPT)* section, and writing down the port name shown in parentheses, right next to a port labelled as *Arduino Nano* (Figure 3b).

![Arduino listed in Device Manager](./Docs/Media/Arduino-Nano-In-Device-Manager.png)

*Figure 3b – Expanded "Ports (COM & LPT)" section in the Windows Device Manager. The selected port, labelled "Arduino Nano", is assigned to port "COM4".*

If necessary, right-click the port label and click on *Properties*, to adjust the port settings of the Arduino in the *Port Settings* tab to match the ones shown in Figure 3c.

![Port Settings of Arduino Nano in Windows Device Manager](./Docs/Media/Arduino-Nano-Port-Settings-Device-Manager.png)

*Figure 3c – Port Settings of the Arduino in Windows Device Manager.*

*Note:* The COM port name and the properties should not change as long as no other COM devices get connected (and the USB port the Arduino is connected to stays the same).

The properties and *Device Manager* window can now be closed.

Then, open `KWA-Controller.exe`, click on the *Port* drop-down menu, and select the port noted in *Device Manager*.
The *FPS* (Frames Per Second) value, which sets the rate at which the camera is triggered to capture a frame, can be set to a value between 1–720, using the edit box or track bar.
Note that higher values result in a larger video file size due to the higher rate at which frames are captured.

> **TODO**: Verify what a reasonable FPS value for inference recordings is. (also note the approx. file size for one min. of video and state it as an example)

Click on *Connect* to establish communication with the Arduino. If no errors icons appear next to the form fields and the button text changes to "Disconnect", lighting and camera trigger can now be switched on/off using the button *Turn lights/cam on*; otherwise, move the mouse pointer over the error icon and read the error message, which should also provide a suggestion on how to fix the error.

![KWA-Controller app communicating over COM4](./Docs/Media/KWA-Controller-App-On-Port-COM4-At-60-FPS.png)

*Figure 3d – The KWA-Controller app and the Arduino are communicating over port COM4. Lights are switched on and the camera is triggered at 60 Hz.*

#### pylon Viewer

##### Install pylon Viewer

Download and install the software suit from the official Basler Website: [pylon Viewer](https://www.baslerweb.com/en/downloads/software-downloads/#type=pylonsoftware;language=all;version=7.2.1)

To record video in MPEG-4 file format, download and install the supplementary package for MPEG-4 from Basler's Website: [pylon Supplementary Package for MPEG-4 Windows](https://www.baslerweb.com/en/downloads/software-downloads/#type=pylonsupplementarypackageformpeg4;language=all;version=all;os=windows)

Note: The rest of this documentation assumes that the package has been installed.

##### Open Camera

Ensure that the camera is connected to the computer and listed in the *Devices* pane in pylon Viewer.
Select the camera and open it by clicking on the *Open Device* button on the toolbar (Figure 4a).

![Devices pane in pylon Viewer](Docs/Media/pylon-Viewer-Open-Device.png)

*Figure 4a – The image shows the Devices pane in pylon Viewer with the Basler acA720-520uc camera selected and ready to be opened by clicking the green Open Device button on the far left on the toolbar on top.*

##### Load Camera Features

To load the provided camera configuration, ensure that the camera device is opened and no recording is running.
Then, in the menu bar, click on *Camera* > *Load Features*.
In the open file dialog box navigate to `Camera/` in the Git repository folder and select `acA720-520uc-inference.pfs`.
Click *Open* to load the camera configuration.

##### Recording Settings

To edit the recording settings, ensure that the camera device is opened and no recording is running. Then, in the menu bar, click on *Window* > *Recording Settings*. In the *Recording Settings* pane, choose the *Video* option.

- Set the *Output Format* to *MP4* to record video in MPEG-4 file format.
- Tick the checkbox *Set Fixed Playback Speed* and set the *FPS* value to the same value set in the KWA-Controller app. This ensures that the playback of the video reflects real time and is not slowed down (FPS pylon Viewer > FPS KWA-Controller) or sped up (FPS pylon Viewer < FPS KWA-Controller). However, a mismatch between both FPS values won't affect the predictions made by the neural network during inference.
- Move the *Quality* slider to the far right position to record videos in the highest quality (lowest compression, but larger file size). Lowering the quality value can ease the system load and help with recording buffer overruns (dropped frames) on slower machines, but might lower the prediction accuracy during inference.
- Set the *Recording Buffer Size* to *5,000*. Should you encounter dropped frames during recording (see status bar of Preview pane), increase the value.
- Set *Record A Frame Every* to *1 Frame(s)*. Each frame captured by the camera will be written to video when recording.

For more details, refer to the [pylon Viewer documentation on recording](https://docs.baslerweb.com/recording).

![Recording Settings pane in pylon Viewer](./Docs/Media/pylon-Viewer-Recording-Settings.png)

*Figure 4b – The image shows the Recording Settings pane in pylon Viewer. The recording to video option is selected. Videos will be recorded in highest quality in MPEG-4 file format with a fixed playback speed of 720 FPS. They will be saved to `C:\Users\KWA\Videos`. Every frame captured by the camera will be written to video. The recording buffer is set to hold 5,000 frames.*

##### Record

To record video, ensure that the camera device is [opened](#open-camera), [configured](#load-camera-features), selected in the *Devices* pane, and the [Recording Settings](#recording-settings) are adjusted.

Enable the camera preview by clicking the *Continuous Shot* button (video camera icon) on the toolbar (Figure 4c)
This previews the frames in the *Preview* pane that the camera is currently capturing.
No video is being recorded yet.

To record a video, click the *Record* button (red recording icon) on the toolbar (Figure 4c).

Note that if the recording buffer size set in [Recording Settings](#recording-settings) is too small and overruns, frames will be dropped and not be part of the recorded video. The number of dropped frames is visible in the *Preview* pane during preview and recording.

To stop a recording, click the *Stop* button (circled square icon) on the toolbar (Figure 4c).

![pylon Viewer toolbar](./Docs/Media/pylon-Viewer-Toolbar.png)

*Figure 4c – Toolbar in pylon Viewer.*

For more details, refer to the [pylon Viewer documentation on recording](https://docs.baslerweb.com/recording).


#### Anaconda/Miniconda

> **TODO** (Maybe not needed, as download page linked above.)

## Repository Contents

> **TODO** Tree view of folders/files (of importance) with short description.

```txt
Arduino
Camera
Docs
Inference
...
```

## Installation

Steps described in this section only need to be done once.

Python (version) fixed in `kine-wheel-arena.yml`.

> **TODO** Create and add `kine-wheel-arena.yaml` to Git repository.

Create a Conda (virtual Python) environment by running the following command:

```console
conda env create -f kine-wheel-arena.yml
```

Activate the Conda environment by running:

```console
conda activate kwa
```

> **TODO** Continue


## Usage

### Workflow

The general workflow consists of two steps:

<ol style="list-style-type: upper-roman">
    <li>recording video and</li>
    <li>analyzing video (i.e., using a trained Deep Neural Network to perform inference).</li>
</ol>

### (I) Record Videos

Each recording session requires the following steps to be performed to record videos:

1. Connect the Basler camera to the computer's USB port.
1. Connect the Basler camera's I/O cable to the *KWA-DLC* control unit (Arduino).
1. Start the [KWA-Controller app](#kwa-controller-app) on the computer which is connected to the system's Basler camera and the Arduino.
1. Start [pylon Viewer](#pylon-viewer) for recording video to storage.
    <ol style="list-style-type: lower-roman">
        <li><a href="#open-camera">Open the camera device</a>.</li>
        <li><a href="#load-camera-features">Load the camera features</a>.</li>
        <li><a href="#record">Record videos</a> in pylon Viewer.</li>
    </ol>

### (II) Analyze and Label Videos

#### Prepare DLC Environment for Inference

> **TODO**: Explain what to do
> * Copy DLC project structure from this repo `DLC-Project/`
> * Download pre-trained NN (from where?) and place it in the correct folder within the copied DLC project
> * 

#### Inference using Deep Neural Network

Open an *Anaconda Prompt* (press Windows key and search for Anaconda Prompt) and activate the *kwa* `conda` environment.

```console
conda activate kwa
```

> **TODO** Does `notebook` package have to be installed in addition to packages in
Conda yml file?

To infer the location of paws in recorded video, either

<ol style="list-style-type: upper-alpha">
    <li><i>use your local machine</i> – run the Python command-line script <code>inference_local.py</code> or start the Jupyter Notebook <code>inference_local.ipynb</code>; or</li>
    <li><i>use a cloud provider</i> – upload <code>inference_cloud.ipynb</code> to a cloud provider hosting Jupyter Notebooks, for instance, Google Colab or Paperspace, and run it on their machines.</li>
</ol>

> **TODO**: Add note about running inference locally (run time, required resources).

##### (A) Run Inference on a Local Machine

For convenience, this repository provides a Python command-line script `inference_local.py` and a Jupyter Notebook `inference_local.ipynb` to run inference on a local machine.
The script as well as the notebook just provide wrapper code which internally uses DLC's `deeplabcut.analyze_videos()` and `deeplabcut.create_labeled_video()` functions.
If you want to have more fine-grained control over both functions, you can write your own scripts.
Refer to the DeepLabCut documentation sections [Novel Video Analysis](https://deeplabcut.github.io/DeepLabCut/docs/standardDeepLabCut_UserGuide.html#i-novel-video-analysis) and [Create Labeled Videos](https://deeplabcut.github.io/DeepLabCut/docs/standardDeepLabCut_UserGuide.html#l-create-labeled-videos).

The command-line script `inference_local.py` can perform two operations:

1) *predict* mouse paw locations in a video using a deep neural network and
1) *label* a video for visualization purposes, based on its prediction results.

Prediction results are stored as HDF5 and CSV file in the same folder where
the corresponding video resides. The `label` operation requires the prediction
results (specifically, the HDF5 file) to be present in the folder of the video
to label.

The script requires as input a single operation and 

- `-c/--config` – the path to the config file of a DLC project and
- `-v/--videos` – the absolute path to a video folder or a list of absolute paths to videos.

Below are 3 examples on how to call the program. The general format is:

```console
python inference.py <operation> <flags>
```

Operations and flags are described in the program's help message.

*Example 1: Print Help*

Prints the program's help text to the console.

```console
python inference.py -h
```

*Example 2: Prediction*

Predict the paw locations for each video in `video_folder`, using the deep
neural network specified in `config.yaml`.

```console
python inference.py -c /path/to/dlc/config.yaml predict -v /path/to/video_folder
```

*Example 3: Video Labelling*

Label the paw locations in `video_file.mp4` as predicted by the neural network.
The `--hq` flag creates the video in high-quality.
This call assumes that the prediction results for `video_file.mp4` are stored in
the same folder where the video resides. The labeled video will be saved to this
folder as well.

```console
python inference.py -c /path/to/dlc/config.yaml label -v /path/to/video_file.mp4
```

> **TODO**: Show sample output files (and content and their locations)

##### (B) Run Inference on a Cloud Provider

> **TODO** E.g., Google Colab

## TODOs and Questions

See OneNote.
