# Usage

## Workflow

The general workflow consists of two steps:

<ol class="my-ol" style="list-style-type: upper-roman">
    <li>recording video and</li>
    <li>analyzing video (i.e., using a trained Deep Neural Network to perform inference).</li>
</ol>

## (I) Record Videos

Each recording session requires the following steps to be performed to record videos:

1. Connect the Basler camera to the computer's USB port.
1. Connect the Basler camera's I/O cable to the *KWA-DLC* control unit (Arduino).
1. Start the [KWA-Controller app](Requirements.md#kwa-controller-app) on the computer which is connected to the system's Basler camera and the Arduino.
1. Start [pylon Viewer](Requirements.md#pylon-viewer) for recording video to storage.

    <ol class="my-ol" style="list-style-type: lower-roman">
        <li><a href="Requirements.html#open-camera">Open the camera device</a>.</li>
        <li><a href="Requirements.html#load-camera-features">Load the camera features</a>.</li>
        <li><a href="Requirements.html#record">Record videos</a> in pylon Viewer.</li>
    </ol>

## (II) Analyze and Label Videos

The following sections explain how to use the trained deep neural network to predict paw locations in recorded video.
The predictions are stored as CSV and HDF5 files.
They can optionally be used to label the analyzed video (i.e., plot the network's predictions for each frame) or in any other software which supports DLC's prediction output format.
For details, see the section on [inference results](#inference-results).

### Create DLC Project for Inference

To run inference, a DLC project must be created and the weights of the trained neural network must be downloaded.

This repository comes with a pre-configured DLC project, located in `DLC-Skeleton`.
Copy the entire folder to a location of your choosing, e.g., `C:\Data\DLC-Skeleton`.

[Download](https://github.com/WinterLab-Berlin/KineWheelSystem/releases) the network weights and save them to `DLC-Skeleton\dlc-models\iteration-2\sa-GA-Basler-acA720-520ucAug23-trainset67shuffle1\train`.

The resulting folder structure of the project should look like this:

```
DLC-Skeleton
    │   config.yaml
    │
    └──dlc-models
        └─iteration-2
            └─sa-GA-Basler-acA720-520ucAug23-trainset67shuffle1
                └─train
                     pose_cfg.yaml
                     snapshot-102500.data-00000-of-00001
                     snapshot-102500.index
                     snapshot-102500.meta
```

Finally, the DLC project configuration, stored in `DLC-Skeleton\config.yaml` must be modified to match your environment.
Open `DLC-Skeleton\config.yaml` with a text editor (e.g., Notepad) and set `project_path` to the absolute path to the `DLC-Skeleton` folder you have copied above, e.g., `C:\Data\DLC-Skeleton`.

*File:* `config.yaml`

```yaml
# Project path (change when moving around)
project_path: C:\Data\DLC-Skeleton
```

### Inference using Deep Neural Network

Open an *Anaconda Prompt* (press Windows key and search for Anaconda Prompt) and activate the *kwa* `conda` environment.

```console
conda activate kwa
```

To infer the location of paws in recorded video, either

<ol class="my-ol" style="list-style-type: upper-alpha">
    <li><i>use your local machine</i> – run the Python command-line script <code>inference.py</code>; or</li>
    <li><i>use a cloud provider</i> – upload <code>inference.ipynb</code> to a cloud provider hosting Jupyter Notebooks, for instance, Google Colab or Paperspace, and run it on their machines.</li>
</ol>

#### (A) Run Inference on a Local Machine

For convenience, this repository provides a Python command-line script `inference.py` and a Jupyter Notebook `inference.ipynb` to run inference on a local machine.
The script as well as the notebook just provide wrapper code which internally uses DLC's `deeplabcut.analyze_videos()` and `deeplabcut.create_labeled_video()` functions.
If you want to have more fine-grained control over both functions, you can write your own scripts.
Refer to the DeepLabCut documentation sections [Novel Video Analysis](https://deeplabcut.github.io/DeepLabCut/docs/standardDeepLabCut_UserGuide.html#i-novel-video-analysis) and [Create Labeled Videos](https://deeplabcut.github.io/DeepLabCut/docs/standardDeepLabCut_UserGuide.html#l-create-labeled-videos).

The command-line script `inference.py` can perform two operations:

1) *predict* mouse paw locations in a video using a deep neural network and
1) *label* a video for visualization purposes, based on its prediction results.

Prediction results are stored as HDF5 and CSV file in the same folder where
the corresponding video resides. The `label` operation requires the prediction
results (specifically, the HDF5 file) to be present in the parent folder of the video to label.

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
This call assumes that the prediction results for `video_file.mp4` are stored in
the same folder where the video resides. The labeled video will be saved to this
folder as well.

```console
python inference.py -c /path/to/dlc/config.yaml label -v /path/to/video_file.mp4
```

#### (B) Run Inference on a Cloud Provider

Upload the Jupyter Notebook `Inference\inference.ipynb` to Google Colab and follow the instructions inside the notebook.

> **TODO** E.g., try out instructions on Google Colab

### Inference Results

Inference results are stored as CSV and HDF5 files. Table 1 shows an extract of such a CSV file.

| bodyparts | MIRROR_VIEW-GREEN | MIRROR_VIEW-GREEN | MIRROR_VIEW-GREEN | ... | SIDE_PROFILE_VIEW-AQUA | SIDE_PROFILE_VIEW-AQUA | SIDE_PROFILE_VIEW-AQUA |
| -------- | ---------------- | ---------------- | ---------------- | -- | --------------------- | --------------------- | --------------------- |
| coords    | x               | y        | likelihood | ... | x        | y        | likelihood |
| |
| 0         | 475.0961609     | 238.942  | 0.025769   | ... | 473.7445 | 241.1117 | 0.419408   |
| ...       | ...             | ...      | ...        | ... | ...      | ...      | ...        |
| 9827      | 326.961853      | 215.4589 | 0.872176   | ... | 471.9395 | 242.7794 | 0.998214   |

*Table 1 – Sample contents of the CSV/HDF5 file generated during inference of a video with 9827 frames.*
*The first two rows are header rows: row one defines label names, each duplicated for one coordinate/likelihood-triple in row two.*
*All remaining rows correspond to exactly one frame in the analyzed video, their respective frame index given in the first column.*
*For each frame and body part, x-/y-coordinates (in # of pixels) and the network's assigned probability of the particular body part being present at these coordinates are given.*
